// The GPU packets the graphics library writes for each indexed draw (see
// "The library's packets" in docs/native-renderer.md). In exclusive mode
// nothing reads them but our kick, which only looks for fences and
// interrupts, so the draw (sub_825BE708) is replaced by one that keeps what
// isn't a packet: the shader load (sub_822AED18: SQ_PROGRAM_CNTL and other
// shadow fields, vertex shader patching, shader fences) and the dirty masks
// cleared; the bound shaders' fences are cleared first, so vertex shaders
// are patched in place rather than copied into the command buffer.
// Predicated draws (tiling) still take the library's path, since
// EndTiling works from the packets they record.
//
// --native_renderer_check_draw_writes keeps the library's draw and logs,
// every 5 s, the device fields each packet-writing helper and the draw
// itself change besides the command buffer pointer: what skipping them
// would lose.
//
// --native_renderer_library_packets turns the replacement off.

#include "draw_packets.h"
#include "guest.h"
#include "renderer.h"
#include "state.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <array>
#include <chrono>
#include <cstring>
#include <map>
#include <mutex>
#include <string>
#include <vector>

REXCVAR_DEFINE_BOOL(native_renderer_check_draw_writes, false, "ITC",
                    "Log the device fields the graphics library's draw and its packet-writing "
                    "helpers change besides the command buffer (a debugging aid for the native "
                    "renderer's exclusive mode; keeps the library's draw)");
REXCVAR_DEFINE_BOOL(native_renderer_library_packets, false, "ITC",
                    "In exclusive mode, let the graphics library write its GPU packets for "
                    "indexed draws as before (nothing reads them)");

namespace native {
namespace {

using Clock = std::chrono::steady_clock;

// Device fields (see "The library's packets").
constexpr uint32_t kDeviceSize = 16384;  // past the last field known (+15156)
constexpr uint32_t kDeviceVertexConstantsDirty = 0;
constexpr uint32_t kDevicePixelConstantsDirty = 8;
constexpr uint32_t kDeviceShaderStateDirty = 16;
constexpr uint32_t kDeviceRenderStateDirty = 24;
constexpr uint32_t kDeviceOtherStateDirty = 32;
constexpr uint32_t kDeviceCommandPointer = 40;
constexpr uint32_t kDeviceTilingFlags = 10808;  // bit 1 (0x2): predicated draws
constexpr uint32_t kDeviceHizState = 11584;     // set when render state bit 1 is dirty
constexpr uint32_t kShaderFence = 8;  // in a shader object
// Shader state dirty bits 49-52 (vertex and pixel shaders): sub_822AED18.
constexpr uint64_t kShaderBits = 0xFull << 49;

enum Helper { kConstants, kRegisters, kRegisters2, kFetchConstants, kHiz, kDraw, kHelpers };
const char* const kHelperNames[kHelpers] = {
    "constants (sub_822AE1D0)",     "register blocks (sub_822ADE30)",
    "register blocks 2 (sub_822ADEF0)", "fetch constants (sub_822AE070)",
    "sub_822ADC08",                 "the draw without the shader load (sub_825BE708)",
};

struct HelperStats {
  uint64_t calls = 0, segment_ends = 0;
  std::map<uint32_t, uint64_t> changed;  // device offset: calls changing it
};
struct CheckStats {
  std::array<HelperStats, kHelpers> helpers;
  uint64_t draws = 0, predicated = 0, shader_loads = 0;
};
CheckStats g_check;
std::mutex g_mutex;
Clock::time_point g_last_log = Clock::now();
thread_local uint32_t t_segment_ends = 0;  // sub_8229EF48 calls

bool Checking() {
  return REXCVAR_GET(native_renderer_check_draw_writes);
}

uint8_t* DevicePointer(uint8_t* base, uint32_t device) {
  return REX_RAW_ADDR(device);
}

// Runs `call` and records which device words it changed (the command
// buffer pointer aside), and whether it ended a command segment (which
// kicks, moving fences and positions).
template <typename Call>
void Checked(Helper helper, uint8_t* base, uint32_t device, Call call) {
  std::vector<uint8_t> before(DevicePointer(base, device),
                              DevicePointer(base, device) + kDeviceSize);
  uint32_t segment_ends = t_segment_ends;
  call();
  const uint8_t* after = DevicePointer(base, device);
  std::lock_guard lock(g_mutex);
  HelperStats& s = g_check.helpers[helper];
  s.calls++;
  if (t_segment_ends != segment_ends) {
    s.segment_ends++;
    return;  // the kick's changes would hide the helper's
  }
  for (uint32_t offset = 0; offset < kDeviceSize; offset += 4) {
    if (offset != kDeviceCommandPointer && std::memcmp(&before[offset], after + offset, 4)) {
      s.changed[offset]++;
    }
  }
}

void LogCheck() {
  CheckStats& c = g_check;
  REXLOG_INFO("native renderer: draw writes: {} indexed draws ({} predicated, {} with a shader "
              "load)",
              c.draws, c.predicated, c.shader_loads);
  for (int i = 0; i < kHelpers; i++) {
    HelperStats& s = c.helpers[i];
    std::string changed;
    for (auto& [offset, count] : s.changed) {
      changed += fmt::format(" +{}={}", offset, count);
    }
    REXLOG_INFO("native renderer: draw writes: {}: {} calls ({} ending a segment, not "
                "compared); device fields changed:{}",
                kHelperNames[i], s.calls, s.segment_ends, changed.empty() ? " none" : changed);
  }
  c = {};
}

}  // namespace
}  // namespace native

#define NATIVE_CHECKED_HELPER(address, helper, mark)                                      \
  extern "C" REX_FUNC(sub_##address) {                                                    \
    REX_FUNC_PROLOGUE();                                                                  \
    mark;                                                                                 \
    if (!native::Checking()) {                                                            \
      __imp__sub_##address(ctx, base);                                                    \
      return;                                                                             \
    }                                                                                     \
    native::Checked(helper, base, ctx.r3.u32, [&] { __imp__sub_##address(ctx, base); }); \
  }

// The draw's packet writers, r3 the device: shader constants into packets,
// register blocks from the shadow, fetch constants, and the HiZ / surface
// set-up (sub_822ADC08). The register blocks' and fetch constants' masks
// (r4; r5 the first register) are what the native renderer is to read
// again (draws other than the indexed draw in exclusive mode flush with
// them).
NATIVE_CHECKED_HELPER(822AE1D0, native::kConstants, )
NATIVE_CHECKED_HELPER(822ADE30, native::kRegisters,
                      native::MarkRegistersDirty(ctx.r5.u32, ctx.r4.u64))
NATIVE_CHECKED_HELPER(822ADEF0, native::kRegisters2, )
NATIVE_CHECKED_HELPER(822AE070, native::kFetchConstants,
                      native::MarkFetchConstantsDirty(ctx.r4.u64))
// sub_822ADC08 writes register 0x2203 (r5) itself.
NATIVE_CHECKED_HELPER(822ADC08, native::kHiz, native::MarkRegistersDirty(0x2203, 1ull << 63))

// Ends the command segment when the buffer is full (kicking it); counted
// so the check can leave those calls out.
extern "C" REX_FUNC(sub_8229EF48) {
  REX_FUNC_PROLOGUE();
  native::t_segment_ends++;
  __imp__sub_8229EF48(ctx, base);
}

// DrawIndexedVertices (sub_825BE708, wrapped in replay_hooks.cpp): r3
// device, r4 primitive type, r5 base vertex, r6 start index, r7 index
// count. Writes the dirty state's packets (five masks at device +0..+32)
// and DRAW_INDX.
void native::DrawPackets_IndexedDraw(PPCContext& ctx, uint8_t* base) {
  uint32_t device = ctx.r3.u32;
  bool predicated = guest::LoadU8(base, device + kDeviceTilingFlags) & 0x2;
  if (Checking()) {
    bool shader_load = REX_LOAD_U64(device + kDeviceShaderStateDirty) & kShaderBits;
    {
      std::lock_guard lock(g_mutex);
      g_check.draws++;
      g_check.predicated += predicated;
      g_check.shader_loads += shader_load;
      if (auto now = Clock::now(); now - g_last_log >= std::chrono::seconds(5)) {
        g_last_log = now;
        LogCheck();
      }
    }
    if (shader_load) {
      __imp__sub_825BE708(ctx, base);  // its writes are expected
    } else {
      Checked(kDraw, base, device, [&] { __imp__sub_825BE708(ctx, base); });
    }
    return;
  }
  if (REXCVAR_GET(native_renderer_library_packets) || predicated) {
    __imp__sub_825BE708(ctx, base);
    return;
  }
  REX_STORE_U64(device + kDeviceVertexConstantsDirty, 0);
  REX_STORE_U64(device + kDevicePixelConstantsDirty, 0);
  uint64_t shader_state = REX_LOAD_U64(device + kDeviceShaderStateDirty);
  if (shader_state & kShaderBits) {
    // A shader's fence (+8) is the command position it was last loaded at;
    // the library patches a vertex shader in place only once the GPU has
    // passed it, and otherwise copies it into the command buffer. Nothing
    // reads the shaders but us, so they're never in use.
    for (uint32_t field : {kDeviceVertexShader, kDevicePixelShader}) {
      if (uint32_t shader = guest::LoadU32(base, device + field)) {
        guest::StoreU32(base, shader + kShaderFence, 0);
      }
    }
    PPCContext saved = ctx;
    ctx.r3.u64 = device;
    ctx.r4.u64 = shader_state;
    sub_822AED18(ctx, base);
    // The mask with the bits for what it wrote in the shadow.
    shader_state |= ctx.r3.u64;
    ctx = saved;
  }
  // What the library's draw would have flushed: read again.
  MarkDeviceDirty(shader_state, REX_LOAD_U64(device + kDeviceRenderStateDirty),
                  REX_LOAD_U64(device + kDeviceOtherStateDirty));
  REX_STORE_U64(device + kDeviceShaderStateDirty, 0);
  if (REX_LOAD_U64(device + kDeviceRenderStateDirty) & 0x2) {
    REX_STORE_U64(device + kDeviceHizState, 0xFFFFFFFFFF000000ull);  // lis r11,-256; std
  }
  REX_STORE_U64(device + kDeviceRenderStateDirty, 0);
  REX_STORE_U64(device + kDeviceOtherStateDirty, 0);
}
