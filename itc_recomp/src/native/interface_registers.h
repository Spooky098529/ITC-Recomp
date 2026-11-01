// The registers the Vulkan renderer's draw path and the SDK's shader
// translator read, made from our interface's state (render/device.h): the
// Vulkan backend's own, as the translator works from the 360's register
// file. Each is made as the game's Direct3D made it from the same state
// (src/graphics/device_state.cpp is the reference); only what the renderer
// reads is made. Also the vertex shader's microcode patched for the bound
// declaration and strides (vertex_fetch_patch.h).

#pragma once

#include "render/device.h"
#include "vertex_fetch_patch.h"

#include <rex/graphics/register_file.h>

#include <cstdint>
#include <map>
#include <span>
#include <tuple>
#include <vector>

namespace native {

class InterfaceRegisters {
 public:
  // Writes the registers from `state` into `regs` (the parts `state.dirty`
  // says changed; all the first time), clearing the bits it read.
  // `base_vertex` is the draw's. Inline draws set fetch constants 93-95
  // themselves afterwards, as now.
  void Build(uint8_t* base, render::State& state, int32_t base_vertex,
             rex::graphics::RegisterFile& regs);

  // The vertex shader's microcode patched for the declaration and stream
  // strides (stride / 4 a stream; `inline_stride` replaces stream 0's for an
  // inline draw), kept by all three, with a number of its own (1 up). Null
  // without a vertex shader.
  struct PatchedShader {
    std::vector<uint8_t> microcode;
    uint32_t id;
  };
  const PatchedShader* PatchedVertexShader(uint8_t* base, const render::State& state,
                                           uint32_t inline_stride);

  // The registers compared by the check, and their names.
  static std::span<const uint32_t> CheckedRegisters();

  // The texture whose words the stage's fetch constant holds: the one
  // bound, or the last one bound while none is (0 for none yet, or a stage
  // past the interface's).
  uint32_t StageTexture(uint32_t stage) const {
    return stage < render::kStages ? texture_handles_[stage] : 0;
  }

 private:
  void BuildTextureFetch(uint8_t* base, const render::State& state, uint32_t stage,
                         uint32_t* words);

  bool built_ = false;
  VertexFetchTables tables_{};
  bool tables_loaded_ = false;
  // The texture's part of each stage's fetch constant as last bound (kept
  // while no texture is), and each stream's vertex fetch constant.
  uint32_t texture_words_[render::kStages][6] = {};
  uint32_t texture_handles_[render::kStages] = {};
  uint32_t stream_words_[render::kStreams][2] = {};
  std::map<std::tuple<uint32_t, uint32_t, uint32_t, std::vector<uint8_t>>, PatchedShader>
      patched_;
};

}  // namespace native
