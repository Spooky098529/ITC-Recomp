// --dump_shader_compiles: captures what the game's HLSL compiler
// (D3DXCompileShader, sub_822B8F10) makes, so the 14 shaders it compiles at
// boot from source in the image can be stored and plugged in instead (see
// sub_822B8F10 in docs/functions/functions.md). Temporary: goes once the
// stored shaders replace the compiles.

#include "guest.h"

#include <cstdio>
#include <filesystem>
#include <format>
#include <string>
#include <vector>

#include "base/flags.h"
#include "base/hash.h"
#include "base/log.h"

ITC_FLAG_STRING(dump_shader_compiles, "", "ITC",
                "Write each shader the game's HLSL compiler (sub_822B8F10) makes to "
                "this directory, and log each compile");

namespace {

std::string LoadString(uint8_t* base, uint32_t address) {
  std::string s;
  if (address == 0) {
    return s;
  }
  for (uint8_t c; (c = guest::LoadU8(base, address)) != 0; address++) {
    s.push_back(static_cast<char>(c));
  }
  return s;
}

// An ID3DXBuffer's bytes, through its vtable: slot 3 GetBufferPointer, slot
// 4 GetBufferSize (the callers release it through slot 2).
std::vector<uint8_t> LoadBuffer(PPCContext& ctx, uint8_t* base, uint32_t buffer) {
  uint32_t vtable = guest::LoadU32(base, buffer);
  ctx.r3.u64 = buffer;
  REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, vtable + 12));
  uint32_t data = ctx.r3.u32;
  ctx.r3.u64 = buffer;
  REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, vtable + 16));
  uint32_t size = ctx.r3.u32;
  std::vector<uint8_t> bytes(size);
  for (uint32_t i = 0; i < size; i++) {
    bytes[i] = guest::LoadU8(base, data + i);
  }
  return bytes;
}

void WriteFile(const std::filesystem::path& path, const std::vector<uint8_t>& bytes) {
  FILE* f = std::fopen(path.string().c_str(), "wb");
  if (!f || std::fwrite(bytes.data(), 1, bytes.size(), f) != bytes.size()) {
    ITC_LOG_ERROR("dump_shader_compiles: can't write {}", path.string());
  }
  if (f) {
    std::fclose(f);
  }
}

}  // namespace

// D3DXCompileShader: r3 the source, r4 its length, r5 the defines, r6 the
// include handler, r7 the entry point, r8 the profile, r9 the flags, r10
// where the shader buffer goes; the caller's stack words +84 and +92 where
// the errors and the constant table go.
extern "C" REX_FUNC(sub_822B8F10) {
  REX_FUNC_PROLOGUE();
  std::string dir = ITC_FLAG(dump_shader_compiles);
  if (dir.empty()) {
    __imp__sub_822B8F10(ctx, base);
    return;
  }
  uint32_t caller = ctx.lr;
  uint32_t source = ctx.r3.u32;
  uint32_t length = ctx.r4.u32;
  uint32_t defines = ctx.r5.u32;
  uint32_t include = ctx.r6.u32;
  std::string entry = LoadString(base, ctx.r7.u32);
  std::string profile = LoadString(base, ctx.r8.u32);
  uint32_t flags = ctx.r9.u32;
  uint32_t shader_out = ctx.r10.u32;
  uint32_t errors_out = guest::LoadU32(base, ctx.r1.u32 + 84);
  uint32_t table_out = guest::LoadU32(base, ctx.r1.u32 + 92);
  std::vector<uint8_t> source_bytes(length);
  for (uint32_t i = 0; i < length; i++) {
    source_bytes[i] = guest::LoadU8(base, source + i);
  }

  __imp__sub_822B8F10(ctx, base);
  uint32_t result = ctx.r3.u32;

  std::filesystem::create_directories(dir);
  std::string name = std::format("shader_{:08X}_{}", source, profile);
  uint32_t shader = shader_out ? guest::LoadU32(base, shader_out) : 0;
  std::vector<uint8_t> shader_bytes;
  if (shader) {
    shader_bytes = LoadBuffer(ctx, base, shader);
    WriteFile(std::filesystem::path(dir) / (name + ".bin"), shader_bytes);
  }
  uint32_t table = table_out ? guest::LoadU32(base, table_out) : 0;
  uint32_t errors = errors_out ? guest::LoadU32(base, errors_out) : 0;
  std::string error_text;
  if (errors) {
    std::vector<uint8_t> bytes = LoadBuffer(ctx, base, errors);
    error_text.assign(bytes.begin(), bytes.end());
    while (!error_text.empty() && (error_text.back() == '\0' || error_text.back() == '\n')) {
      error_text.pop_back();
    }
  }
  ITC_LOG_INFO(
      "dump_shader_compiles: caller {:08X} source {:08X} length {} (hash {:016X}) entry {} "
      "profile {} flags {:08X} defines {:08X} include {:08X} -> result {:08X} shader {:08X} "
      "({} bytes, hash {:016X}, vtable {:08X}) constant table asked {} made {:08X} errors {:08X} '{}'",
      caller, source, length, base::Hash64(source_bytes.data(), source_bytes.size()), entry,
      profile, flags, defines, include, result, shader, shader_bytes.size(),
      base::Hash64(shader_bytes.data(), shader_bytes.size()),
      shader ? guest::LoadU32(base, shader) : 0, table_out != 0, table, errors,
      error_text);
  ctx.r3.u64 = result;
}
