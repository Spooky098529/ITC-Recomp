// The graphics library's indexed draw without its GPU packets
// (docs/pc-renderer.md, step 3): no GPU runs the command buffer, so the
// draw (sub_825BE708) keeps only what isn't a packet: the shader load
// (sub_822AED18: SQ_PROGRAM_CNTL and the other shadow fields the renderer
// reads, the vertex shader's patch for the bound declaration) and the
// dirty masks cleared. The packet writers the original draw calls write
// only packets and the command buffer's position: their work is Dropped
// (the graphics rule; "The indexed draw without its packets" in
// docs/native-renderer.md found no other device field changed). Tiled
// draws (predicated tiling) take the same path: their packets went into
// chains only the tiling worker's job reads, which never runs.

#include "draw_packets.h"
#include "hooks/native_entry.h"
#include "guest.h"
#include "state.h"

#include "base/log.h"
#include "hooks/command_buffer.h"
#include "hooks/shader_load.h"

namespace native {
namespace {

// Device fields (see "The library's packets").
constexpr uint32_t kDeviceVertexConstantsDirty = 0;
constexpr uint32_t kDevicePixelConstantsDirty = 8;
constexpr uint32_t kDeviceShaderStateDirty = 16;
constexpr uint32_t kDeviceRenderStateDirty = 24;
constexpr uint32_t kDeviceOtherStateDirty = 32;
constexpr uint32_t kDeviceHizState = 11584;  // set when render state bit 1 is dirty
constexpr uint32_t kShaderFence = 8;         // in a shader object
// Shader state dirty bits 49-52 (vertex and pixel shaders): sub_822AED18.
constexpr uint64_t kShaderBits = 0xFull << 49;

}  // namespace
}  // namespace native

// The draw's packet writers, r3 the device: shader constants (sub_822AE1D0),
// register blocks from the shadow (sub_822ADE30, sub_822ADEF0), fetch
// constants (sub_822AE070), and the HiZ / surface set-up (sub_822ADC08).
// Dropped: r3 left as the device (their callers don't read it);
// sub_822ADC08's is its mask (r4) with bit 8 cleared, as the original's.
ITC_NATIVE_ENTRY_ALWAYS(822AE1D0) { ITC_NATIVE_RUNNING("sub_822AE1D0 (dropped)"); }
ITC_NATIVE_ENTRY_ALWAYS(822ADE30) { ITC_NATIVE_RUNNING("sub_822ADE30 (dropped)"); }
ITC_NATIVE_ENTRY_ALWAYS(822ADEF0) { ITC_NATIVE_RUNNING("sub_822ADEF0 (dropped)"); }
ITC_NATIVE_ENTRY_ALWAYS(822AE070) { ITC_NATIVE_RUNNING("sub_822AE070 (dropped)"); }
ITC_NATIVE_ENTRY_ALWAYS(822ADC08) {
  ITC_NATIVE_RUNNING("sub_822ADC08 (dropped)");
  ctx.r3.u64 = ctx.r4.u64 & ~0x100ull;
}

// Ends the command segment when the buffer is full: the command buffer's
// plain C++ (src/hooks/command_buffer.cpp).
ITC_NATIVE_ENTRY_ALWAYS(8229EF48) {
  hooks::EndCommandSegment(ctx);
}

// DrawIndexedVertices (sub_825BE708, wrapped in replay_hooks.cpp): r3
// device, r4 primitive type, r5 base vertex, r6 start index, r7 index
// count. The original writes the dirty state's packets (five masks at
// device +0..+32) and DRAW_INDX.
void native::DrawPackets_IndexedDraw(lifted::Regs& ctx) {
  uint32_t device = ctx.r3.u32;
  guest::StoreU64(device + kDeviceVertexConstantsDirty, 0);
  guest::StoreU64(device + kDevicePixelConstantsDirty, 0);
  uint64_t shader_state = guest::LoadU64(device + kDeviceShaderStateDirty);
  if (shader_state & kShaderBits) {
    // A shader's fence (+8) is the command position it was last loaded at;
    // the library patches a vertex shader in place only once the GPU has
    // passed it, and otherwise copies it into the command buffer. Nothing
    // reads the shaders but us, so they're never in use.
    for (uint32_t field : {kDeviceVertexShader, kDevicePixelShader}) {
      if (uint32_t shader = guest::LoadU32(device + field)) {
        guest::StoreU32(shader + kShaderFence, 0);
      }
    }
    hooks::LoadShaders(device, shader_state, ctx.r1.u32, ctx.r13.u64);
  }
  guest::StoreU64(device + kDeviceShaderStateDirty, 0);
  if (guest::LoadU64(device + kDeviceRenderStateDirty) & 0x2) {
    guest::StoreU64(device + kDeviceHizState, 0xFFFFFFFFFF000000ull);  // lis r11,-256; std
  }
  guest::StoreU64(device + kDeviceRenderStateDirty, 0);
  guest::StoreU64(device + kDeviceOtherStateDirty, 0);
}
