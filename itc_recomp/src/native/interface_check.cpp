// --check_conventional_renderer (temporary): at each draw the renderer
// makes from the registers, what it would have from our interface
// (render/commands.h, interface_registers.h) compared with what the game's
// Direct3D left in the device: the bound objects, the registers the draw
// reads, the fetch constants of bound textures and streams, the constants
// and the vertex shader's patched microcode. Differences are counted by what
// differs and logged every 5 s with an example.

#include "interface_check.h"

#include "guest.h"
#include "interface_registers.h"
#include "state.h"

#include "base/log.h"
#include "render/commands.h"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace native {

namespace {

using Clock = std::chrono::steady_clock;

struct Difference {
  uint64_t count = 0;
  uint32_t bits = 0;  // the bits ever differing
  std::vector<std::pair<uint32_t, uint32_t>> examples;  // up to 4 distinct (ours, the game's)
};

struct Check {
  InterfaceRegisters registers;
  rex::graphics::RegisterFile regs{};
  std::map<std::string, Difference> differences;
  uint64_t draws = 0, draws_differing = 0;
  Clock::time_point last_log = Clock::now();
};
Check* g_check = nullptr;

void Note(std::string_view what, uint32_t ours, uint32_t theirs, bool& any) {
  if (ours == theirs) {
    return;
  }
  any = true;
  Difference& d = g_check->differences[std::string(what)];
  if (d.examples.size() < 4 &&
      std::find(d.examples.begin(), d.examples.end(), std::pair{ours, theirs}) == d.examples.end()) {
    d.examples.push_back({ours, theirs});
  }
  d.count++;
  d.bits |= ours ^ theirs;
}

void Log() {
  Check& c = *g_check;
  ITC_LOG_INFO("conventional renderer check: {} draws, {} with differences; {} kinds", c.draws,
               c.draws_differing, c.differences.size());
  for (const auto& [what, d] : c.differences) {
    std::string examples;
    for (auto [ours, theirs] : d.examples) {
      examples += fmt::format(" {:08X}/{:08X}", ours, theirs);
    }
    ITC_LOG_INFO("conventional renderer check:   {}: {} draws, bits {:08X}; ours/the game's:{}",
                 what, d.count, d.bits, examples);
  }
}

}  // namespace

void InterfaceCheck_Draw(uint8_t* base, uint32_t device, const rex::graphics::RegisterFile& regs,
                         bool inline_draw, int32_t base_vertex, uint32_t inline_stride) {
  render::Commands* commands = render::GetCommands();
  if (!commands || render::ConventionalRenderer()) {
    return;
  }
  if (!g_check) {
    g_check = new Check;
  }
  Check& c = *g_check;
  render::State& state = commands->state();
  const uint32_t textures_dirty = state.dirty & render::kDirtyTextures;
  (void)textures_dirty;
  c.registers.Build(base, state, base_vertex, c.regs);
  bool any = false;
  const bool in_drawer = g_current_command == 15;
  auto u32 = [&](uint32_t address) { return guest::LoadU32(base, address); };

  // The bound objects.
  for (uint32_t i = 0; i < render::kTargets; i++) {
    Note("render target " + std::to_string(i), state.targets[i].address,
         u32(device + kDeviceRenderTarget0 + 4 * i), any);
  }
  Note("depth surface", state.depth.address, u32(device + kDeviceRenderTarget0 + 16), any);
  Note("vertex shader", state.vertex_shader.address, u32(device + kDeviceVertexShader), any);
  Note("pixel shader", state.pixel_shader.address, u32(device + kDevicePixelShader), any);
  Note("declaration", state.declaration.address, u32(device + kDeviceVertexDeclaration), any);
  if (!inline_draw) {
    Note("index buffer", state.indices.address, u32(device + kDeviceIndexBuffer), any);
  }
  Note("scissor enable", state.scissor, u32(device + kDeviceScissorEnable) != 0, any);
  if (state.scissor) {
    const int32_t* r = &state.scissor_rect.left;
    for (uint32_t k = 0; k < 4; k++) {
      Note("scissor rect " + std::to_string(k), uint32_t(r[k]), u32(device + kDeviceScissorRect + 4 * k),
           any);
    }
  }

  // The registers.
  for (uint32_t index : InterfaceRegisters::CheckedRegisters()) {
    if (inline_draw && index == 0x2102) {
      continue;  // set by the inline draw itself
    }
    // The drawer's draws (command 15) set the viewport enable around their
    // draw: our handler has put it back by the time the game's draws.
    if (in_drawer && (index == 0x2204 || index == 0x2206)) {
      continue;
    }
    // RB_COLOR_MASK: target 0's, the part the renderer reads.
    const uint32_t mask = index == 0x2104 ? 0xF : ~0u;
    Note(fmt::format("register {:04X}", index), c.regs.values[index] & mask,
         regs.values[index] & mask, any);
  }
  // Fetch constants: bound textures (device +12288) and streams (+12204).
  // (Not the device's texture and stream slots: the game's Direct3D unbinds
  // and binds them in its own clears and resolves, which the renderer never
  // read; the fetch constants it reads stay.)
  for (uint32_t stage = 0; stage < render::kStages; stage++) {
    for (uint32_t w = 0; w < 6; w++) {
      Note("texture fetch word " + std::to_string(w), c.regs.values[0x4800 + 6 * stage + w],
           regs.values[0x4800 + 6 * stage + w], any);
    }
  }
  for (uint32_t stream = 0; stream < render::kStreams; stream++) {
    if ((inline_draw && stream < 3) || !u32(device + 12204 + 4 * stream)) {
      continue;  // fetch constants 93-95, set by the inline draw; or unbound (read by no draw)
    }
    for (uint32_t w = 0; w < 2; w++) {
      Note("vertex fetch word " + std::to_string(w), c.regs.values[0x4800 + 2 * (95 - stream) + w],
           regs.values[0x4800 + 2 * (95 - stream) + w], any);
    }
  }
  for (uint32_t w = 0; w < 40; w++) {
    Note(w < 8 ? "bool constants" : "loop constants", c.regs.values[0x4900 + w],
         regs.values[0x4900 + w], any);
  }
  // The float constants.
  for (uint32_t stage = 0; stage < 2; stage++) {
    const auto& file = stage ? state.constants.pixel : state.constants.vertex;
    const uint32_t at = device + (stage ? kDevicePixelFloatConstants : kDeviceVertexFloatConstants);
    for (uint32_t r = 0; r < render::kFloatConstants; r++) {
      for (uint32_t k = 0; k < 4; k++) {
        uint32_t ours;
        std::memcpy(&ours, &file[r][k], 4);
        Note(stage ? "pixel float constants" : "vertex float constants", ours,
             u32(at + 16 * r + 4 * k), any);
      }
    }
  }
  // The patched vertex microcode the game's draw ran.
  if (const uint32_t vs = state.vertex_shader.address; vs && vs == u32(device + kDeviceVertexShader)) {
    const uint32_t declaration = u32(device + kDeviceVertexDeclaration);
    const uint32_t declaration_id = declaration ? u32(declaration + 48) : 0;
    const uint32_t size = u32(vs + 600);
    const uint8_t* theirs = nullptr;
    const LoadedVertexShader& loaded = g_loaded_vertex_shader;
    if (loaded.object == vs && loaded.declaration_id == declaration_id &&
        loaded.microcode.size() == size) {
      theirs = loaded.microcode.data();
    } else if (u32(vs + 24) == declaration_id) {
      theirs = PhysicalHostAddress(base, GpuPhysicalAddress(u32(vs + 48)));
    }
    const InterfaceRegisters::PatchedShader* patched =
        c.registers.PatchedVertexShader(base, state, inline_stride);
    const std::vector<uint8_t>* ours = patched ? &patched->microcode : nullptr;
    if (theirs && ours) {
      uint32_t first = UINT32_MAX;
      for (uint32_t i = 0; i < size && i < ours->size(); i += 4) {
        if (std::memcmp(ours->data() + i, theirs + i, 4)) {
          first = i / 4;
          break;
        }
      }
      Note("vertex shader microcode (first word differing)", first == UINT32_MAX ? 0 : first + 1, 0,
           any);
    } else {
      Note("vertex shader microcode not compared", 1, 0, any);
    }
  }

  c.draws++;
  c.draws_differing += any;
  if (Clock::now() - c.last_log >= std::chrono::seconds(5)) {
    c.last_log = Clock::now();
    Log();
  }
}

}  // namespace native
