// The shader objects as plain C++ (shader_objects.h), store for store as the
// recompiled functions.

#include "shader_objects/shader_objects.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "models/memory_blocks.h"
#include "native/buffers.h"

#include <cstdlib>
#include <cstring>
#include <string>

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

// The size of the object a file makes: 592 or 56 (by the magic's low 6
// bits) plus the header's.
uint32_t ObjectBytes(uint32_t magic, uint32_t header) {
  return ((magic & 0x3F) != 0 ? kVertexShaderBytes : kPixelShaderBytes) + header;
}

// The header copy from `file` after `fields` cleared bytes, its checks made.
void CopyHeaderFrom(uint8_t* base, uint32_t object, uint32_t file, uint32_t fields);

// The header copy (sub_822A9FD8, sub_822A9EA8) for `fields` bytes.
bool CopyHeader(uint8_t* base, uint32_t object, uint32_t record, uint32_t fields) {
  if (c_runtime::MayShare(record, 4, object, fields)) {
    return false;
  }
  const uint32_t file = Load(base, record);
  const uint32_t bytes = Load(base, file + 4);
  if (!c_runtime::XMemCpyFits(base, object + fields, file, bytes)) {
    return false;
  }
  Clear(base, object, fields);
  c_runtime::XMemCpy(base, object + fields, file, bytes);
  return true;
}

// A step that can't fail after its checks.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("shader objects: {} refused after its checks", what);
    std::abort();
  }
}

}  // namespace

namespace {

// What ParseFile writes in its record.
struct FileRecord {
  uint32_t file, object_bytes, code, code_bytes;
};

FileRecord ReadFile(uint8_t* base, uint32_t file) {
  // ShaderFile::Parse's work (its record kept on the host) for its native
  // callers.
  ITC_NATIVE_RUNNING("sub_822A8568 ShaderFile::Parse");
  const uint32_t header = Load(base, file + 4);
  return {file, ObjectBytes(Load(base, file + 0), header), header + file, Load(base, file + 8)};
}

// A debug message the original prints (sub_825A1318), logged.
// The message without its line end.
std::string TrimmedMessage(const char* host) {
  std::string text(host, strnlen(host, 256));
  while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
    text.pop_back();
  }
  return text;
}

void DebugMessage(uint8_t* base, const char* where, uint32_t text) {
  const char* host = reinterpret_cast<const char*>(REX_RAW_ADDR(text));
  ITC_LOG_WARN("{}: the game's debug message: {}", where, TrimmedMessage(host));
}

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

bool CopyVertexHeader(uint8_t* base, uint32_t object, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_822A9FD8 VertexShader::CopyHeader");
  return CopyHeader(base, object, record, kVertexShaderBytes);
}

bool CopyPixelHeader(uint8_t* base, uint32_t object, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_822A9EA8 PixelShader::CopyHeader");
  return CopyHeader(base, object, record, kPixelShaderBytes);
}

namespace {

// The creates' shared steps: the object made and its header copied after
// `fields` cleared bytes; the microcode made and copied. 0 for the object
// when an allocation fails (the object then freed).
struct Made {
  uint32_t object = 0;
  uint32_t microcode = 0;
};

std::optional<Made> MakeShader(uint8_t* base, uint32_t file, uint32_t fields, uint32_t caller) {
  const uint32_t magic = Load(base, file + 0);
  const uint32_t code_bytes = Load(base, file + 8);
  const uint32_t header = Load(base, file + 4);
  // The header copy's step and memcpy's flag, checked first (the new
  // blocks can't share memory with the file).
  if (!native::Buffers_NativeMemcpy() || !c_runtime::XMemCpyBytes(base, file, header)) {
    return std::nullopt;
  }
  const uint32_t code = header + file;
  Made made;
  made.object = memory::crt::XMemAlloc(base, ObjectBytes(magic, header), kObjectAttributes);
  if (made.object == 0) {
    return made;
  }
  const uint32_t copied = Load(base, file + 4);
  Clear(base, made.object, fields);
  Require(c_runtime::XMemCpy(base, made.object + fields, file, copied), "XMemCpy");
  made.microcode = memory::crt::XMemAlloc(base, code_bytes, kMicrocodeAttributes);
  if (made.microcode == 0) {
    memory::crt::XMemFree(made.object, kObjectAttributes);
    made.object = 0;
    return made;
  }
  Require(native::Buffers_Memcpy(base, made.microcode, code, code_bytes, caller), "memcpy");
  return made;
}

}  // namespace

std::optional<uint32_t> VertexShaderCreate(uint8_t* base, uint32_t file, const CreateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822AA018 VertexShader::Create");
  const auto made = MakeShader(base, file, kVertexShaderBytes, 0x822AA0C0);
  if (!made) {
    return std::nullopt;
  }
  if (made->object == 0) {
    return 0;
  }
  if (!VertexShaderInit(base, made->object, made->microcode)) {
    calls.init(calls.user, made->object, made->microcode);
  }
  Store(base, made->object, Load(base, made->object) | kCreatedFlag);
  return made->object;
}

std::optional<uint32_t> PixelShaderCreate(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_822A9EE8 PixelShader::Create");
  const auto made = MakeShader(base, file, kPixelShaderBytes, 0x822A9F90);
  if (!made) {
    return std::nullopt;
  }
  if (made->object == 0) {
    return 0;
  }
  const uint32_t object = made->object;
  Clear(base, object, kPixelShaderBytes);
  Store(base, object + 24, made->microcode);
  Store(base, object + 0, 7);
  Store(base, object + 4, 1);
  Store(base, object + 20, 0xFFFF0000u);
  Store(base, object + 0, 7 | kCreatedFlag);
  return object;
}

namespace {

void CopyHeaderFrom(uint8_t* base, uint32_t object, uint32_t file, uint32_t fields) {
  // VertexShader::CopyHeader's or PixelShader::CopyHeader's work for their
  // native callers.
  if (fields == kVertexShaderBytes) {
    ITC_NATIVE_RUNNING("sub_822A9FD8 VertexShader::CopyHeader");
  } else {
    ITC_NATIVE_RUNNING("sub_822A9EA8 PixelShader::CopyHeader");
  }
  const uint32_t bytes = Load(base, file + 4);
  Clear(base, object, fields);
  Require(c_runtime::XMemCpy(base, object + fields, file, bytes), "XMemCpy");
}

}  // namespace

bool LoadShaders(uint8_t* base, uint32_t model, uint32_t section, const CreateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82594D20 Mesh::LoadShaders");
  const uint32_t count = Load(base, section);
  // The checks first: memcpy's flag, each header copy's byte loop (the
  // new blocks can't share memory with the files).
  if (!native::Buffers_NativeMemcpy()) {
    return false;
  }
  uint32_t pair = section + 32;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t vertex = pair + 32;
    const uint32_t pixel = Load(base, pair + 4) + vertex;
    if (!c_runtime::XMemCpyBytes(base, vertex, Load(base, vertex + 4)) ||
        !c_runtime::XMemCpyBytes(base, pixel, Load(base, pixel + 4))) {
      return false;
    }
    pair = Load(base, pair + 8) + pixel;
  }
  Store(base, model + 420, count);
  if (count == 0) {
    return true;
  }
  Store(base, model + 24, models::AllocVirtual(base, count << 4));
  pair = section + 32;
  for (uint32_t i = 0; i < Load(base, model + 420); i++) {
    const uint32_t vertex = pair + 32;
    const uint32_t pixel = Load(base, pair + 4) + vertex;
    const uint32_t shaders = Load(base, model + 24) + 16 * i;
    const uint32_t next = Load(base, pair + 8) + pixel;
    // The vertex shader.
    FileRecord record = ReadFile(base, vertex);
    Store(base, shaders + 0, models::AllocVirtual(base, record.object_bytes));
    CopyHeaderFrom(base, Load(base, shaders + 0), vertex, kVertexShaderBytes);
    Store(base, shaders + 8,
          models::AllocPhysicalEx(base, record.code_bytes, 0, 0xFFFFFFFFu, 32, 0x404));
    Require(native::Buffers_Memcpy(base, Load(base, shaders + 8), record.code, record.code_bytes,
                                   0x82594DE0),
            "memcpy");
    if (!VertexShaderInit(base, Load(base, shaders + 0), Load(base, shaders + 8))) {
      calls.init(calls.user, Load(base, shaders + 0), Load(base, shaders + 8));
    }
    // The pixel shader.
    record = ReadFile(base, pixel);
    Store(base, shaders + 4, models::AllocVirtual(base, record.object_bytes));
    CopyHeaderFrom(base, Load(base, shaders + 4), pixel, kPixelShaderBytes);
    Store(base, shaders + 12,
          models::AllocPhysicalEx(base, record.code_bytes, 0, 0xFFFFFFFFu, 32, 0x404));
    Require(native::Buffers_Memcpy(base, Load(base, shaders + 12), record.code,
                                   record.code_bytes, 0x82594E38),
            "memcpy");
    PixelShaderInit(base, Load(base, shaders + 4), Load(base, shaders + 12));
    if (Load(base, shaders + 8) == 0) {
      DebugMessage(base, "Mesh::LoadShaders", 0x8206CD7C);
    }
    if (Load(base, shaders + 12) == 0) {
      DebugMessage(base, "Mesh::LoadShaders", 0x8206CDA0);
    }
    pair = next;
  }
  return true;
}

std::optional<bool> PairLoaded(uint8_t* base, uint32_t context, const CreateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820A06D0 Shaders::OnPairLoaded");
  constexpr uint32_t kVertexTable = 0x826BFCB8;
  constexpr uint32_t kPixelTable = 0x826BDC98;
  const uint32_t vertex = Load(base, context + 0);
  const uint32_t pixel = Load(base, context + 4);
  // The checks first: memcpy's flag, each header copy's byte loop (the new
  // blocks can't share memory with the files).
  if (!native::Buffers_NativeMemcpy() ||
      !c_runtime::XMemCpyBytes(base, vertex, Load(base, vertex + 4)) ||
      !c_runtime::XMemCpyBytes(base, pixel, Load(base, pixel + 4))) {
    return std::nullopt;
  }
  const auto slot = [&](uint32_t table) { return table + (Load(base, context + 8) << 2); };
  // The vertex shader.
  FileRecord record = ReadFile(base, vertex);
  Store(base, slot(kVertexTable), models::AllocVirtual(base, record.object_bytes));
  CopyHeaderFrom(base, Load(base, slot(kVertexTable)), vertex, kVertexShaderBytes);
  uint32_t code = models::AllocPhysicalEx(base, record.code_bytes, 0, 0xFFFFFFFFu, 32, 0x404);
  Require(native::Buffers_Memcpy(base, code, record.code, record.code_bytes, 0x820A074C),
          "memcpy");
  const uint32_t vertex_object = Load(base, slot(kVertexTable));
  if (!VertexShaderInit(base, vertex_object, code)) {
    calls.init(calls.user, vertex_object, code);
  }
  // The pixel shader (its file read again: nothing above writes it).
  record = ReadFile(base, Load(base, context + 4));
  Store(base, slot(kPixelTable), models::AllocVirtual(base, record.object_bytes));
  CopyHeaderFrom(base, Load(base, slot(kPixelTable)), record.file, kPixelShaderBytes);
  code = models::AllocPhysicalEx(base, record.code_bytes, 0, 0xFFFFFFFFu, 32, 0x404);
  Require(native::Buffers_Memcpy(base, code, record.code, record.code_bytes, 0x820A07C8),
          "memcpy");
  PixelShaderInit(base, Load(base, slot(kPixelTable)), code);
  return memory::crt::OperatorDelete(context);
}

}  // namespace shader_objects
