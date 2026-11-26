// From SBX-Recomp (src/renderer/vulkan/ at b128953), used with its author's OK
// (docs/shaders.md, "In this repository"); changed here: our namespace and
// paths, and the vertex fetches' remap and swizzle (docs/pc-renderer.md, step 2).

// Fitting a ported game shader's SPIR-V to a draw: the values of its
// specialization constants, and the locations of its interpolators.
//
// A port is compiled once (src/native/glsl/game). What changes between
// draws is written into a copy of its SPIR-V here, so the pipeline needs no
// VkSpecializationInfo and the shader module is made the usual way.

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace native {

struct PortSpecialization {
  // (SpecId, value): a bool constant is true when its value isn't 0.
  std::vector<std::pair<uint32_t, uint32_t>> constants;
  // The interpolators the vertex shader writes and the pixel shader reads,
  // a bit each. A port's interpolator n is at location n; in the shader
  // made from it, an interpolator in the mask is at its index among the
  // mask's (the translator's locations), and one that isn't is at 16 + n,
  // where nothing reads or writes it.
  uint32_t interpolator_mask = 0;
  // Which variables are the interpolators: the pixel shader's inputs or the
  // vertex shader's outputs.
  bool vertex = false;
};

// The port's SPIR-V with the constants' values and the locations set.
// Returns an empty vector if the SPIR-V can't be read.
std::vector<uint32_t> SpecializePort(const uint32_t* spirv, size_t word_count,
                                     const PortSpecialization& specialization);

}  // namespace native
