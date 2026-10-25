// The stored shaders (src/hooks/shader_compiles.cpp) against the game's HLSL
// compiler: each of the 12 sources in the image compiled by the recompiled
// D3DXCompileShader (sub_822B8F10) and answered by ours, with the callers'
// arguments ("main", the profile, no defines, include handler or flags);
// the shader buffer's bytes and r3 compared. The error buffer isn't: the
// compiler's warnings go to OutputDebugStringA (dropped), and ours makes none.

#include "harness.h"

#include <format>

#include "graphics/compiled_shaders.h"

namespace harness {
namespace {

// The shader buffer's bytes (D3DXCreateBuffer's: +8 the size, +12 the data)
// and r3, then the buffers released through vtable slot 2.
Outputs Compile(Context& h, const graphics::compiled_shaders::Entry& shader,
                GuestFunction compile) {
  const uint32_t entry = h.data, profile = h.data + 16;
  const uint32_t shader_out = h.data + 32, errors_out = h.data + 36, table_out = h.data + 40;
  Fill(h.base, h.data, kScratchBytes);
  Write(h.base, entry, "main", 5);
  Write(h.base, profile, shader.profile, 7);
  guest::StoreU32(h.base, shader_out, 0);
  guest::StoreU32(h.base, errors_out, 0);
  PPCContext ctx = CallContext(h);
  uint8_t* base = h.base;
  guest::StoreU32(base, ctx.r1.u32 + 84, errors_out);
  guest::StoreU32(base, ctx.r1.u32 + 92, 0);
  ctx.r3.u64 = shader.source;
  ctx.r4.u64 = shader.length;
  ctx.r5.u64 = 0;
  ctx.r6.u64 = 0;
  ctx.r7.u64 = entry;
  ctx.r8.u64 = profile;
  ctx.r9.u64 = 0;
  ctx.r10.u64 = shader_out;
  compile(ctx, base);
  Outputs out;
  out.Word(ctx.r3.u32);
  for (uint32_t at : {shader_out, errors_out}) {
    const uint32_t buffer = guest::LoadU32(base, at);
    if (buffer == 0) {
      continue;
    }
    if (at == shader_out) {
      out.Bytes(Read(base, guest::LoadU32(base, buffer + 12), guest::LoadU32(base, buffer + 8)));
    }
    ctx.r3.u64 = buffer;
    REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, buffer) + 8));
  }
  return out;
}

}  // namespace

void HardcodedShaders(Context& h) {
  // The HSV gradient's literal constants come out in another order when
  // compiled during a race than at boot (every boot compile gave the stored
  // order): the same program, its constants at other registers.
  constexpr uint32_t kOrderDiffers = 0x82071528;
  Results results("D3DXCompileShader (sub_822B8F10)");
  Results order("D3DXCompileShader (sub_822B8F10), the HSV gradient's constant order "
                "(expected to differ)");
  for (const auto& shader : graphics::compiled_shaders::kShaders) {
    (shader.source == kOrderDiffers ? order : results)
        .Check(Compile(h, shader, __imp__sub_822B8F10), Compile(h, shader, sub_822B8F10),
               std::format("source {:08X} {}", shader.source, shader.profile));
  }
  results.Report();
  order.Report();
}

}  // namespace harness
