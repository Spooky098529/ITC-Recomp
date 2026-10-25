// Diagnostics (--dump_shader_compiles): what the game's HLSL compiler
// (D3DXCompileShader, sub_822B8F10) makes, written to a directory (one
// shader_<source>_<profile>.bin a compile) and logged with its arguments,
// result, errors and the buffer's vtable. How src/graphics/compiled_shaders.h
// was made (tools/compiled_shaders.py); kept for checking other builds of
// the game. The adapter in src/hooks/shader_compiles.cpp calls it.

#pragma once

#include <cstdint>

#include "guest.h"

namespace debug {

// Whether --dump_shader_compiles names a directory.
bool ShaderCompileDump_Active();

// Compiles through the recompiled sub_822B8F10 (ctx as the game called it;
// r3 its result), then dumps and logs the shader.
void ShaderCompileDump_Compile(PPCContext& ctx, uint8_t* base);

}  // namespace debug
