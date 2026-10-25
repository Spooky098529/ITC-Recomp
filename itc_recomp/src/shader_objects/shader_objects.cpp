// The shader objects as plain C++ (shader_objects.h), store for store as the
// recompiled functions.

#include "shader_objects/shader_objects.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"

#include <cstring>

namespace shader_objects {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

constexpr uint32_t kPhysical = 0xA0000000u;
constexpr uint32_t kMaxFetches = 32;  // 64 + 4 x 32 = 192, 192 + 12 x 32 = 576

}  // namespace

void ParseFile(uint8_t* base, uint32_t file, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_822A8568 ShaderFile::Parse");
  Store(base, record + 0, 0);
  Store(base, record + 4, 0);
  Store(base, record + 8, 0);
  Store(base, record + 12, 0);
  const uint32_t magic = Load(base, file + 0);
  const uint32_t header = Load(base, file + 4);
  Store(base, record + 0, file);
  Store(base, record + 8, header + file);
  const uint32_t fields = (magic & 0x3F) != 0 ? kVertexShaderBytes : kPixelShaderBytes;
  Store(base, record + 4, fields + header);
  Store(base, record + 12, Load(base, file + 8));
}

bool VertexShaderInit(uint8_t* base, uint32_t object, uint32_t microcode) {
  ITC_NATIVE_RUNNING("sub_822A8740 VertexShader::Init");
  if (object >= kPhysical || !c_runtime::InOneView(object, kVertexShaderBytes)) {
    return false;
  }
  // Read before anything is written: with the checks below, nothing the
  // function writes is read again.
  const uint32_t table = object + kVertexShaderBytes + Load(base, object + 612);
  const uint32_t count = Load(base, table + 28);
  const uint32_t words = table + 4 * (Load(base, table + 24) + 10);
  if (count > kMaxFetches || c_runtime::MayShare(table + 24, 8, object, kVertexShaderBytes)) {
    return false;
  }
  if (count != 0 &&
      c_runtime::MayShare(words, 4 * count, object, kVertexShaderBytes)) {
    return false;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t from = microcode + 12 * (Load(base, words + 4 * i) & 0xFFF);
    if (c_runtime::MayShare(from, 12, object, kVertexShaderBytes)) {
      return false;
    }
  }
  Clear(base, object, kVertexShaderBytes);
  Store(base, object + 48, microcode);
  Store(base, object + 0, 6);
  Store(base, object + 4, 1);
  Store(base, object + 20, 0xFFFF0000u);
  Store(base, object + 24, 0xFFFFFFFFu);
  Store(base, object + 60, count);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t word = Load(base, words + 4 * i);
    Store(base, object + 64 + 4 * i, word);
    const uint32_t from = microcode + 12 * (word & 0xFFF);
    const uint32_t to = object + 192 + 12 * i;
    if (to != from) {
      std::memcpy(REX_RAW_ADDR(to), REX_RAW_ADDR(from), 12);
    }
  }
  return true;
}

void PixelShaderInit(uint8_t* base, uint32_t object, uint32_t microcode) {
  ITC_NATIVE_RUNNING("sub_822A85B8 PixelShader::Init");
  Clear(base, object, kPixelShaderBytes);
  Store(base, object + 24, microcode);
  Store(base, object + 0, 7);
  Store(base, object + 4, 1);
  Store(base, object + 20, 0xFFFF0000u);
}

}  // namespace shader_objects
