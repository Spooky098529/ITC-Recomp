// D3DXCompileShader (sub_822B8F10) answered with the stored shaders
// (src/graphics/hardcoded_shaders.h) instead of compiling: an adapter that
// makes the shader's buffer with the game's own D3DXCreateBuffer
// (sub_822BE450, its vtable at 0x82019DC0) and copies the microcode in. The
// callers read the buffer through slot 3 and release it through slot 2, as
// before. No error buffer or constant table: none is asked for but the
// primitive drawer's errors, whose only text was the compiler's "unused
// inputs" warning, printed through OutputDebugStringA (dropped). See
// sub_822B8F10 in docs/functions/functions.md. --dump_shader_compiles
// compiles instead, dumping what's made (src/debug/shader_compile_dump.h).

#include "guest.h"

#include <string>

#include "base/flags.h"
#include "base/log.h"
#include "debug/shader_compile_dump.h"
#include "graphics/hardcoded_shaders.h"

ITC_FLAG_BOOL(use_hardcoded_shaders, true, "ITC",
              "Use the shaders the game's HLSL compiler made from the HLSL in the image "
              "(src/graphics/compiled_shaders.h) instead of compiling them at boot "
              "(D3DXCompileShader sub_822B8F10; the bytes the boot's compiles make, "
              "checked by --run_harnesses=hardcoded_shaders)");

namespace {

std::string LoadProfile(uint8_t* base, uint32_t address) {
  std::string s;
  for (uint8_t c; s.size() < 16 && (c = guest::LoadU8(base, address + s.size())) != 0;) {
    s.push_back(static_cast<char>(c));
  }
  return s;
}

}  // namespace

// r3 the source, r4 its length, r5 the defines, r6 the include handler, r7
// the entry point, r8 the profile, r9 the flags, r10 where the shader buffer
// goes; the caller's stack words +84 and +92 where the errors and the
// constant table go. Returns 0, or D3DXCreateBuffer's failure.
extern "C" REX_FUNC(sub_822B8F10) {
  REX_FUNC_PROLOGUE();
  if (debug::ShaderCompileDump_Active()) {
    debug::ShaderCompileDump_Compile(ctx, base);
    return;
  }
  if (!ITC_FLAG(use_hardcoded_shaders)) {
    __imp__sub_822B8F10(ctx, base);
    return;
  }
  namespace hs = graphics::hardcoded_shaders;
  const hs::Shader* shader = hs::Find(ctx.r3.u32, ctx.r4.u32, LoadProfile(base, ctx.r8.u32));
  if (!shader || ctx.r5.u32 != 0 || ctx.r6.u32 != 0 || ctx.r9.u32 != 0) {
    ITC_LOG_ERROR("hardcoded shaders: no stored shader for source {:08X} length {}, compiled",
                  ctx.r3.u32, ctx.r4.u32);
    __imp__sub_822B8F10(ctx, base);
    return;
  }
  ITC_NATIVE_RUNNING("sub_822B8F10 D3DXCompileShader");
  const uint32_t shader_out = ctx.r10.u32;
  const uint32_t errors_out = guest::LoadU32(base, ctx.r1.u32 + 84);
  const uint32_t table_out = guest::LoadU32(base, ctx.r1.u32 + 92);
  ctx.r3.u64 = shader->size;
  ctx.r4.u64 = shader_out;
  sub_822BE450(ctx, base);
  if (ctx.r3.s32 < 0) {
    return;
  }
  const uint32_t data = guest::LoadU32(base, guest::LoadU32(base, shader_out) + 12);
  for (uint32_t i = 0; i < shader->size; i++) {
    guest::StoreU8(base, data + i, shader->data[i]);
  }
  if (errors_out) {
    guest::StoreU32(base, errors_out, 0);
  }
  if (table_out) {
    guest::StoreU32(base, table_out, 0);
  }
  ctx.r3.u64 = 0;
}
