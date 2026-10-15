// The GPU state of a draw, read from the graphics library's register and
// constant shadow in the device (see "The register shadow as a whole" in
// docs/gpu-functions.md). Read after the library's draw call, when it holds
// everything the draw used.

#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <rex/graphics/register_file.h>

namespace native {

// The device pointer and fields read directly.
constexpr uint32_t kDevicePointer = 0x8296BD90;
constexpr uint32_t kDevicePixelShader = 12432, kDeviceVertexShader = 12436;
constexpr uint32_t kDeviceIndexBuffer = 12180;
// The vertex declaration (its id at +48; a vertex shader object's +24 holds
// the id of the one its microcode was last patched for).
constexpr uint32_t kDeviceVertexDeclaration = 11552;
// How often the library patched a vertex shader's microcode for a
// declaration (sub_822AE340; render thread).
extern uint32_t g_vertex_shader_patches;
// The last patch's shader object, declaration and the declaration's id.
extern uint32_t g_last_patch[3];
// The last vertex shader the library patched as a copy in the command
// buffer (see sub_822AE340's wrapper in replay_hooks.cpp): its object, the
// declaration id it's patched for, and the patched microcode. object is 0
// after a patch in place.
struct LoadedVertexShader {
  uint32_t object = 0, declaration_id = 0;
  std::vector<uint8_t> microcode;
};
extern LoadedVertexShader g_loaded_vertex_shader;
// The command whose handler is running (0: none).
extern uint32_t g_current_command;
// Bumped by each command handler call (to tell one command's library calls from
// the next's).
extern uint32_t g_command_serial;
constexpr uint32_t kDeviceRenderTarget0 = 12184;
// Scissor: enable (render state 200) and rectangle (left, top, right,
// bottom) from SetScissorRect.
constexpr uint32_t kDeviceScissorEnable = 11600, kDeviceScissorRect = 12416;
// Float constants c0-c255, 16 bytes each: vertex shader's, then pixel
// shader's.
constexpr uint32_t kDeviceVertexFloatConstants = 1792, kDevicePixelFloatConstants = 5888;

// Fills regs (host byte order) from the device's shadow: the register
// blocks the draw path flushes, and the fetch, bool and loop constants
// (not the float constants). Only the words the graphics library marked
// dirty since the last call (MarkDeviceDirty and the others below; all
// before the first call) are copied: the rest of regs is as it was. Zero
// regs once before the first call (clearing all 80 KB every draw was ~10%
// of the render thread).
void ReadRegisters(uint8_t* base, uint32_t device, rex::graphics::RegisterFile& regs);

// What ReadRegisters is to copy next (render thread). The library marks
// what its setters write in the shadow as dirty in three masks of the
// device (+16 shader state, +24 render state, +32 other state), and its
// draws clear them after flushing. MarkDeviceDirty takes the masks before
// a draw clears them; MarkRegistersDirty and MarkFetchConstantsDirty take
// the masks the library's flush helpers sub_822ADE30 (first register, top
// bit first) and sub_822AE070 (top bit fetch constant 0) are given.
// MarkRegistersDirty also marks words changed in regs by hand.
void MarkDeviceDirty(uint64_t shader_state, uint64_t render_state, uint64_t other_state);
void MarkRegistersDirty(uint32_t first_register, uint64_t mask);
void MarkFetchConstantsDirty(uint64_t mask);
void MarkAllDirty();

// ReadRegisters, counted (--native_renderer_log_register_reads): calls,
// calls where anything changed since the last, words in the blocks, words
// copied (dirty) and changed, calls changing each block (7 register blocks,
// then fetch, bool, loop constants), and time stamp counter cycles of the
// copy. Each call is checked against reading every block whole (as before
// only dirty words were read): words differing, by block, and the first
// (register, value read, value in the shadow).
constexpr size_t kRegisterReadBlocks = 10;
struct RegisterReadStats {
  uint64_t calls = 0, calls_changed = 0, words = 0, words_copied = 0, words_changed = 0;
  uint64_t blocks_changed[kRegisterReadBlocks] = {};
  uint64_t copy_cycles = 0;
  uint64_t mismatched_words = 0;
  uint64_t blocks_mismatched[kRegisterReadBlocks] = {};
  std::array<uint32_t, 3> mismatch_sample{};
};
void ReadRegisters(uint8_t* base, uint32_t device, rex::graphics::RegisterFile& regs,
                   RegisterReadStats& stats);

// The GPU's physical address for a guest address, as the graphics library
// converts it (IM_LOAD, SetTexture, SetStreamSource).
inline uint32_t GpuPhysicalAddress(uint32_t address) {
  return (address & 0x1FFFFFFFu) + (((address >> 20) + 0x200u) & 0x1000u);
}

// Guest physical memory is one 512 MB mapping 4 GB above the virtual base
// (rex::memory::Memory: physical membase = mapping base + 4 GB).
constexpr uint64_t kPhysicalMemoryOffset = 0x100000000ull;
constexpr uint32_t kPhysicalMemorySize = 0x20000000u;

inline uint8_t* PhysicalHostAddress(uint8_t* base, uint32_t physical) {
  return base + kPhysicalMemoryOffset + physical;
}

}  // namespace native
