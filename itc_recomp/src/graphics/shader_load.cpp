// The graphics library's shader load (see shader_load.h), from the lifted
// sub_822AED18 and its copy path sub_822AEBF0, read 2026-11-26.

#include "graphics/shader_load.h"

#include "guest.h"
#include "memory/heap.h"

#include "base/log.h"

#include <cstring>
#include <mutex>

namespace graphics::shader_load {

namespace {

// The device's fields.
constexpr uint32_t kTiling = 10808;   // byte: bit 7 predicated tiling under way, bit 6 a tiling
constexpr uint32_t kFlags = 10809;    // byte: bit 2 a depth write against HiZ's direction
constexpr uint32_t kFlags2 = 10810;   // byte: bit 7 the last load a remapped copy, 6 HiZ on, 4 export mode
constexpr uint32_t kProgramControl = 10400, kContextMisc = 10404, kInterpolatorControl = 10408;
constexpr uint32_t kDepthControl = 10420, kColorControl = 10428, kHizControl = 10432;
constexpr uint32_t kModeControl = 10452;    // RB_MODECONTROL: bits 0-2 the EDRAM mode
constexpr uint32_t kShaderStrides = 11568;  // two u64: the strides the shader is loaded with
constexpr uint32_t kDeclaration = 11552;
constexpr uint32_t kStreamStrides = 12272;  // two u64: a stride byte a stream
constexpr uint32_t kLastEdramMode = 12440;  // the tiling's
constexpr uint32_t kPixelShader = 12432, kVertexShader = 12436;
// The global counter the declarations' ids come from.
constexpr uint32_t kDeclarationIds = 0x829537F0;

constexpr uint64_t Bit(uint32_t n) { return 1ull << n; }

// The copy a vertex shader is patched in when it can't be in place (the
// original's is in the command buffer): grown as needed, kept.
uint32_t g_copy = 0, g_copy_size = 0;
std::mutex g_ids_mutex;

// sub_822ADA98: whether HiZ can be used for this depth state (`ps_flag`
// the pixel shader's header word 0 bit 27, probably that it writes depth).
bool HizUsable(uint32_t depth, uint32_t hiz, bool ps_flag) {
  const uint32_t func = (depth >> 4) & 7;
  const bool direction = (hiz & 0x10) ? (func == 2 || func == 4 || func == 6)
                                      : (func == 1 || func == 2 || func == 3);
  bool usable = !ps_flag && (depth & 2) && (hiz & 1) && direction;
  if (depth & 1) {  // stencil
    usable = usable && !(depth & 0x3800) && !(depth & 0xE0000);
    if (depth & 0x80) {  // its back faces
      usable = usable && !(depth & 0xE0000000) && !(depth & 0x3800000);
    }
  }
  return usable;
}

// sub_822ADA28: a depth write against HiZ's direction.
bool WritesAgainstHiz(uint32_t depth, uint32_t hiz) {
  if (!(depth & 2) || !(depth & 4)) {
    return false;
  }
  const uint32_t func = (depth >> 4) & 7;
  return func == 5 || func == 7 || (!(hiz & 0x10) && (func == 4 || func == 6)) ||
         ((hiz & 0x10) && (func == 1 || func == 3));
}

void LoadStrides(uint32_t device) {
  guest::StoreU64(device + kShaderStrides, guest::LoadU64(device + kStreamStrides));
  guest::StoreU64(device + kShaderStrides + 8, guest::LoadU64(device + kStreamStrides + 8));
}

}  // namespace

uint64_t LoadShaders(uint32_t device, uint64_t mask, uint32_t scratch, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822AED18 Device::LoadShaders");
  const uint32_t vs = guest::LoadU32(device + kVertexShader);
  const uint32_t ps = guest::LoadU32(device + kPixelShader);
  const uint32_t declaration = guest::LoadU32(device + kDeclaration);
  // Each shader's program control block: the vertex shader's +592 plus its
  // +612, the pixel shader's +56 plus its +76.
  const uint32_t vs_header = vs + 592 + guest::LoadU32(vs + 612);
  const uint32_t ps_header = ps ? ps + 56 + guest::LoadU32(ps + 76) : 0;
  // The program control word, where the exports' remap can change it.
  const uint32_t program_control = scratch + 128;
  guest::StoreU32(program_control, guest::LoadU32(vs_header));
  uint32_t misc = guest::LoadU32(vs_header + 4);
  // The vertex shader's outputs not those the pixel shader reads.
  const uint32_t differs =
      ps_header ? (guest::LoadU32(ps_header + 16) ^ guest::LoadU32(vs_header + 16)) & 0x7FFFF
                : guest::LoadU32(vs_header + 16) & 0x1F;

  // The copy path (sub_822AEBF0): the microcode patched in a copy, its
  // exports remapped first when they differ; the object stays as it was.
  auto patch_copy = [&](bool remap) {
    const uint32_t size = guest::LoadU32(vs + 600);
    if (size > g_copy_size) {
      memory::Free(g_copy);
      g_copy = memory::Alloc(size, 0);
      g_copy_size = size;
    }
    std::memcpy(guest::Pointer(g_copy), guest::Pointer(guest::LoadU32(vs + 48)), size);
    if (remap) {
      calls.remap_exports(scratch, vs_header, g_copy, program_control, ps_header);
    }
    guest::StoreU8(device + kFlags2,
                   uint8_t((guest::LoadU8(device + kFlags2) & 0x7F) | (remap ? 0x80 : 0)));
    // The original patches with no declaration too, reading its +24 at 0:
    // never seen; nothing to patch for here.
    if (declaration != 0) {
      calls.patch(vs, g_copy, declaration, device + kStreamStrides);
    }
    LoadStrides(device);
  };

  if ((guest::LoadU8(device + kFlags2) & 0x80) || differs) {
    patch_copy(differs != 0);
  } else if (mask & Bit(51)) {
    const uint32_t microcode = guest::LoadU32(vs + 48);
    // Patched for this declaration and these strides already (the
    // declaration's +32 and +40: a byte mask of the streams it uses).
    auto patched_for_this = [&] {
      return guest::LoadU32(declaration + 48) == guest::LoadU32(vs + 24) &&
             (((guest::LoadU64(device + kStreamStrides + 8) ^ guest::LoadU64(vs + 40)) &
               guest::LoadU64(declaration + 40)) |
              ((guest::LoadU64(device + kStreamStrides) ^ guest::LoadU64(vs + 32)) &
               guest::LoadU64(declaration + 32))) == 0;
    };
    if (declaration == 0 || (guest::LoadU32(vs + 592) & 0x40)) {
      LoadStrides(device);
    } else if (guest::LoadU8(device + kTiling) & 0x80) {
      patch_copy(false);
    } else if (patched_for_this()) {
      LoadStrides(device);
    } else {
      // In place (the original copies when the GPU may still be reading the
      // shader, by its fence: no GPU reads it).
      calls.patch(vs, microcode, declaration, device + kStreamStrides);
      if (guest::LoadU32(declaration + 48) == 0) {
        std::lock_guard lock(g_ids_mutex);
        uint32_t id;
        do {
          id = guest::LoadU32(kDeclarationIds) + 1;
          guest::StoreU32(kDeclarationIds, id);
        } while (id == 0 || id == 0xFFFFFFFFu);
        guest::StoreU32(declaration + 48, id);
      }
      guest::StoreU32(vs + 24, guest::LoadU32(declaration + 48));
      guest::StoreU64(vs + 32, guest::LoadU64(device + kStreamStrides));
      guest::StoreU64(vs + 40, guest::LoadU64(device + kStreamStrides + 8));
      LoadStrides(device);
    }
  }
  uint32_t cntl = guest::LoadU32(program_control);

  // The pixel shader: none draws depth only; one draws color and depth.
  if (!ps_header) {
    guest::StoreU32(device + kModeControl, (guest::LoadU32(device + kModeControl) & ~7u) | 5);
    mask |= 8;
  } else {
    cntl |= guest::LoadU32(ps_header);
    misc |= guest::LoadU32(ps_header + 4);
    uint8_t flags2 = guest::LoadU8(device + kFlags2);
    if ((cntl & 0x70000000u) > 0x10000000u) {
      flags2 |= 0x10;
    }
    guest::StoreU8(device + kFlags2, flags2);
    if (mask & Bit(52)) {
      guest::StoreU32(device + kInterpolatorControl, guest::LoadU32(ps_header + 24));
      mask |= Bit(46) | Bit(47) | Bit(48) | 8;
      guest::StoreU32(device + kModeControl, (guest::LoadU32(device + kModeControl) & ~7u) | 4);
    }
  }

  guest::StoreU32(device + kProgramControl, cntl);
  guest::StoreU32(device + kContextMisc, misc);
  uint64_t result = mask | Bit(47) | Bit(48);

  if (mask & Bit(49)) {
    const bool ps_flag = ps_header && ((guest::LoadU32(ps_header) >> 27) & 1);
    const uint32_t depth = guest::LoadU32(device + kDepthControl);
    uint32_t hiz = guest::LoadU32(device + kHizControl);
    const uint32_t on = ((guest::LoadU8(device + kFlags2) >> 6) & 1) & ((depth >> 1) & 1);
    hiz = (hiz & ~1u) | on;
    const bool usable = HizUsable(depth, hiz, ps_flag);
    hiz = (hiz & ~2u) | (usable ? 2u : 0u);
    guest::StoreU32(device + kHizControl, hiz);
    const uint8_t flags = guest::LoadU8(device + kFlags);
    if (usable) {
      // Its EVENT_WRITE (15) left out with the packets.
      guest::StoreU8(device + kFlags, uint8_t(flags & ~4));
    } else if (WritesAgainstHiz(depth, hiz)) {
      guest::StoreU8(device + kFlags, uint8_t(flags | 4));
    }
    result |= 0x100;
  }

  if (guest::LoadU8(device + kTiling) & 0x40) {
    uint32_t mode = guest::LoadU32(device + kModeControl) & 7;
    if (!ps_header ||
        (!(guest::LoadU32(device + kColorControl) & 8) &&
         !(guest::LoadU32(ps_header + 28) & 0x30))) {
      mode = 5;
    }
    // Its register packets left out; the mode it set kept.
    if (mode != guest::LoadU32(device + kLastEdramMode) || (result & 8)) {
      guest::StoreU32(device + kLastEdramMode, mode);
      result &= ~8ull;
    }
  }
  return result;
}

}  // namespace graphics::shader_load
