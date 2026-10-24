// The shaders the game's HLSL compiler (D3DXCompileShader, sub_822B8F10)
// makes at boot, stored (compiled_shaders.h) so the compiler doesn't run:
// 14 compiles of 12 sources in the image, from Font::CreateShaders
// (sub_821088B8), PrimitiveDrawer::Init (sub_825A1438) and sub_8209BE88,
// none with defines, an include handler or flags. See sub_822B8F10 in
// docs/functions/functions.md.

#pragma once

#include <cstdint>
#include <string_view>

#include "graphics/compiled_shaders.h"

namespace graphics::hardcoded_shaders {

using Shader = compiled_shaders::Entry;

// The stored shader for a compile of `length` bytes of HLSL at `source`
// for `profile`, or null when it isn't one of the game's.
inline const Shader* Find(uint32_t source, uint32_t length, std::string_view profile) {
  for (const Shader& shader : compiled_shaders::kShaders) {
    if (shader.source == source && shader.length == length && profile == shader.profile) {
      return &shader;
    }
  }
  return nullptr;
}

}  // namespace graphics::hardcoded_shaders
