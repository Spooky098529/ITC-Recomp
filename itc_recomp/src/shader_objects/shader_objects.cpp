// The shader objects as plain C++ (shader_objects.h), store for store as the
// recompiled functions.

#include "shader_objects/shader_objects.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "models/memory_blocks.h"
#include "native/buffers.h"
#include "schema/models.h"
#include "schema/shader_objects.h"

#include <cstdlib>
#include <cstring>
#include <string>

namespace shader_objects {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(address, bytes,
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
void CopyHeaderFrom(uint32_t object, uint32_t file, uint32_t fields);

// The header copy (sub_822A9FD8, sub_822A9EA8) for `fields` bytes.
bool CopyHeader(uint32_t object, uint32_t record, uint32_t fields) {
  if (c_runtime::MayShare(record, 4, object, fields)) {
    return false;
  }
  const schema::ShaderFile file = schema::ShaderFileRecord(record).file();
  const uint32_t bytes = file.header_size();
  if (!c_runtime::XMemCpyFits(object + fields, file.address, bytes)) {
    return false;
  }
  Clear(object, fields);
  c_runtime::XMemCpy(object + fields, file.address, bytes);
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

FileRecord ReadFile(uint32_t file) {
  // ShaderFile::Parse's work (its record kept on the host) for its native
  // callers.
  ITC_NATIVE_RUNNING("sub_822A8568 ShaderFile::Parse");
  const schema::ShaderFile view(file);
  const uint32_t header = view.header_size();
  return {file, ObjectBytes(view.magic(), header), header + file, view.microcode_size()};
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

void DebugMessage(const char* where, uint32_t text) {
  const char* host = reinterpret_cast<const char*>(guest::Pointer(text));
  ITC_LOG_WARN("{}: the game's debug message: {}", where, TrimmedMessage(host));
}

}  // namespace

void ParseFile(uint32_t file, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_822A8568 ShaderFile::Parse");
  const schema::ShaderFileRecord out(record);
  const schema::ShaderFile in(file);
  out.set_file(0u);
  out.set_object_bytes(0);
  out.set_microcode(0);
  out.set_microcode_bytes(0);
  const uint32_t magic = in.magic();
  const uint32_t header = in.header_size();
  out.set_file(file);
  out.set_microcode(header + file);
  const uint32_t fields = (magic & 0x3F) != 0 ? kVertexShaderBytes : kPixelShaderBytes;
  out.set_object_bytes(fields + header);
  out.set_microcode_bytes(in.microcode_size());
}

bool VertexShaderInit(uint32_t object, uint32_t microcode) {
  ITC_NATIVE_RUNNING("sub_822A8740 VertexShader::Init");
  if (object >= kPhysical || !c_runtime::InOneView(object, kVertexShaderBytes)) {
    return false;
  }
  const schema::VertexShader shader(object);
  // Read before anything is written: with the checks below, nothing the
  // function writes is read again.
  const uint32_t table = object + kVertexShaderBytes + shader.header_fetch_table();
  const uint32_t count = Load(table + 28);
  const uint32_t words = table + 4 * (Load(table + 24) + 10);
  if (count > kMaxFetches || c_runtime::MayShare(table + 24, 8, object, kVertexShaderBytes)) {
    return false;
  }
  if (count != 0 &&
      c_runtime::MayShare(words, 4 * count, object, kVertexShaderBytes)) {
    return false;
  }
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t from = microcode + 12 * (Load(words + 4 * i) & 0xFFF);
    if (c_runtime::MayShare(from, 12, object, kVertexShaderBytes)) {
      return false;
    }
  }
  Clear(object, kVertexShaderBytes);
  shader.set_microcode(microcode);
  shader.set_common(6);
  shader.set_reference_count(1);
  shader.set_base_flush(0xFFFF0000u);
  shader.set_unknown_24(0xFFFFFFFFu);
  shader.set_fetch_count(count);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t word = Load(words + 4 * i);
    Store(shader.fetch_words_address() + 4 * i, word);
    const uint32_t from = microcode + 12 * (word & 0xFFF);
    const uint32_t to = shader.fetch_instructions_address() + 12 * i;
    if (to != from) {
      std::memcpy(guest::Pointer(to), guest::Pointer(from), 12);
    }
  }
  return true;
}

void PixelShaderInit(uint32_t object, uint32_t microcode) {
  ITC_NATIVE_RUNNING("sub_822A85B8 PixelShader::Init");
  const schema::PixelShader shader(object);
  Clear(object, kPixelShaderBytes);
  shader.set_microcode(microcode);
  shader.set_common(7);
  shader.set_reference_count(1);
  shader.set_base_flush(0xFFFF0000u);
}

bool CopyVertexHeader(uint32_t object, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_822A9FD8 VertexShader::CopyHeader");
  return CopyHeader(object, record, kVertexShaderBytes);
}

bool CopyPixelHeader(uint32_t object, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_822A9EA8 PixelShader::CopyHeader");
  return CopyHeader(object, record, kPixelShaderBytes);
}

namespace {

// The creates' shared steps: the object made and its header copied after
// `fields` cleared bytes; the microcode made and copied. 0 for the object
// when an allocation fails (the object then freed).
struct Made {
  uint32_t object = 0;
  uint32_t microcode = 0;
};

std::optional<Made> MakeShader(uint32_t file, uint32_t fields, uint32_t caller) {
  const schema::ShaderFile view(file);
  const uint32_t magic = view.magic();
  const uint32_t code_bytes = view.microcode_size();
  const uint32_t header = view.header_size();
  // The header copy's step and memcpy's flag, checked first (the new
  // blocks can't share memory with the file).
  if (!native::Buffers_NativeMemcpy() || !c_runtime::XMemCpyBytes(file, header)) {
    return std::nullopt;
  }
  const uint32_t code = header + file;
  Made made;
  made.object = memory::crt::XMemAlloc(ObjectBytes(magic, header), kObjectAttributes);
  if (made.object == 0) {
    return made;
  }
  const uint32_t copied = view.header_size();
  Clear(made.object, fields);
  Require(c_runtime::XMemCpy(made.object + fields, file, copied), "XMemCpy");
  made.microcode = memory::crt::XMemAlloc(code_bytes, kMicrocodeAttributes);
  if (made.microcode == 0) {
    memory::crt::XMemFree(made.object, kObjectAttributes);
    made.object = 0;
    return made;
  }
  Require(native::Buffers_Memcpy(made.microcode, code, code_bytes, caller), "memcpy");
  return made;
}

}  // namespace

std::optional<uint32_t> VertexShaderCreate(uint32_t file, const CreateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822AA018 VertexShader::Create");
  const auto made = MakeShader(file, kVertexShaderBytes, 0x822AA0C0);
  if (!made) {
    return std::nullopt;
  }
  if (made->object == 0) {
    return 0;
  }
  if (!VertexShaderInit(made->object, made->microcode)) {
    calls.init(calls.user, made->object, made->microcode);
  }
  const schema::VertexShader shader(made->object);
  shader.set_common(shader.common() | kCreatedFlag);
  return made->object;
}

std::optional<uint32_t> PixelShaderCreate(uint32_t file) {
  ITC_NATIVE_RUNNING("sub_822A9EE8 PixelShader::Create");
  const auto made = MakeShader(file, kPixelShaderBytes, 0x822A9F90);
  if (!made) {
    return std::nullopt;
  }
  if (made->object == 0) {
    return 0;
  }
  const uint32_t object = made->object;
  const schema::PixelShader shader(object);
  Clear(object, kPixelShaderBytes);
  shader.set_microcode(made->microcode);
  shader.set_common(7);
  shader.set_reference_count(1);
  shader.set_base_flush(0xFFFF0000u);
  shader.set_common(7 | kCreatedFlag);
  return object;
}

namespace {

void CopyHeaderFrom(uint32_t object, uint32_t file, uint32_t fields) {
  // VertexShader::CopyHeader's or PixelShader::CopyHeader's work for their
  // native callers.
  if (fields == kVertexShaderBytes) {
    ITC_NATIVE_RUNNING("sub_822A9FD8 VertexShader::CopyHeader");
  } else {
    ITC_NATIVE_RUNNING("sub_822A9EA8 PixelShader::CopyHeader");
  }
  const uint32_t bytes = schema::ShaderFile(file).header_size();
  Clear(object, fields);
  Require(c_runtime::XMemCpy(object + fields, file, bytes), "XMemCpy");
}

}  // namespace

bool LoadShaders(uint32_t model, uint32_t section, const CreateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82594D20 Mesh::LoadShaders");
  const schema::Model mesh(model);
  const uint32_t count = schema::ShaderSection(section).pair_count();
  // The checks first: memcpy's flag, each header copy's byte loop (the
  // new blocks can't share memory with the files).
  if (!native::Buffers_NativeMemcpy()) {
    return false;
  }
  uint32_t pair = section + 32;
  for (uint32_t i = 0; i < count; i++) {
    const schema::ShaderPairEntry entry(pair);
    const uint32_t vertex = pair + 32;
    const uint32_t pixel = entry.vertex_size() + vertex;
    if (!c_runtime::XMemCpyBytes(vertex, schema::ShaderFile(vertex).header_size()) ||
        !c_runtime::XMemCpyBytes(pixel, schema::ShaderFile(pixel).header_size())) {
      return false;
    }
    pair = entry.pixel_size() + pixel;
  }
  mesh.set_shader_pair_count(count);
  if (count == 0) {
    return true;
  }
  mesh.set_shader_pairs(models::AllocVirtual(count << 4));
  pair = section + 32;
  for (uint32_t i = 0; i < mesh.shader_pair_count(); i++) {
    const schema::ShaderPairEntry entry(pair);
    const uint32_t vertex = pair + 32;
    const uint32_t pixel = entry.vertex_size() + vertex;
    const schema::ModelShaderPair shaders(mesh.shader_pairs().address + 16 * i);
    const uint32_t next = entry.pixel_size() + pixel;
    // The vertex shader.
    FileRecord record = ReadFile(vertex);
    shaders.set_vertex_shader(models::AllocVirtual(record.object_bytes));
    CopyHeaderFrom(shaders.vertex_shader(), vertex, kVertexShaderBytes);
    shaders.set_vertex_microcode(
        models::AllocPhysicalEx(record.code_bytes, 0, 0xFFFFFFFFu, 32, 0x404));
    Require(native::Buffers_Memcpy(shaders.vertex_microcode(), record.code,
                                   record.code_bytes, 0x82594DE0),
            "memcpy");
    if (!VertexShaderInit(shaders.vertex_shader(), shaders.vertex_microcode())) {
      calls.init(calls.user, shaders.vertex_shader(), shaders.vertex_microcode());
    }
    // The pixel shader.
    record = ReadFile(pixel);
    shaders.set_pixel_shader(models::AllocVirtual(record.object_bytes));
    CopyHeaderFrom(shaders.pixel_shader(), pixel, kPixelShaderBytes);
    shaders.set_pixel_microcode(
        models::AllocPhysicalEx(record.code_bytes, 0, 0xFFFFFFFFu, 32, 0x404));
    Require(native::Buffers_Memcpy(shaders.pixel_microcode(), record.code,
                                   record.code_bytes, 0x82594E38),
            "memcpy");
    PixelShaderInit(shaders.pixel_shader(), shaders.pixel_microcode());
    if (shaders.vertex_microcode() == 0) {
      DebugMessage("Mesh::LoadShaders", 0x8206CD7C);
    }
    if (shaders.pixel_microcode() == 0) {
      DebugMessage("Mesh::LoadShaders", 0x8206CDA0);
    }
    pair = next;
  }
  return true;
}

std::optional<bool> PairLoaded(uint32_t context, const CreateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820A06D0 Shaders::OnPairLoaded");
  constexpr uint32_t kVertexTable = schema::ModelsGlobals::shared_vertex_table_address();
  constexpr uint32_t kPixelTable = schema::ModelsGlobals::shared_pixel_table_address();
  const schema::ShaderPairLoad load(context);
  const uint32_t vertex = load.vertex_file();
  const uint32_t pixel = load.pixel_file();
  // The checks first: memcpy's flag, each header copy's byte loop (the new
  // blocks can't share memory with the files).
  if (!native::Buffers_NativeMemcpy() ||
      !c_runtime::XMemCpyBytes(vertex, schema::ShaderFile(vertex).header_size()) ||
      !c_runtime::XMemCpyBytes(pixel, schema::ShaderFile(pixel).header_size())) {
    return std::nullopt;
  }
  // The shared tables are bytes fields (2,056 pointers each): their slots by
  // the pair index.
  const auto slot = [&](uint32_t table) { return table + (load.pair() << 2); };
  // The vertex shader.
  FileRecord record = ReadFile(vertex);
  Store(slot(kVertexTable), models::AllocVirtual(record.object_bytes));
  CopyHeaderFrom(Load(slot(kVertexTable)), vertex, kVertexShaderBytes);
  uint32_t code = models::AllocPhysicalEx(record.code_bytes, 0, 0xFFFFFFFFu, 32, 0x404);
  Require(native::Buffers_Memcpy(code, record.code, record.code_bytes, 0x820A074C),
          "memcpy");
  const uint32_t vertex_object = Load(slot(kVertexTable));
  if (!VertexShaderInit(vertex_object, code)) {
    calls.init(calls.user, vertex_object, code);
  }
  // The pixel shader (its file read again: nothing above writes it).
  record = ReadFile(load.pixel_file());
  Store(slot(kPixelTable), models::AllocVirtual(record.object_bytes));
  CopyHeaderFrom(Load(slot(kPixelTable)), record.file, kPixelShaderBytes);
  code = models::AllocPhysicalEx(record.code_bytes, 0, 0xFFFFFFFFu, 32, 0x404);
  Require(native::Buffers_Memcpy(code, record.code, record.code_bytes, 0x820A07C8),
          "memcpy");
  PixelShaderInit(Load(slot(kPixelTable)), code);
  return memory::crt::OperatorDelete(context);
}

}  // namespace shader_objects
