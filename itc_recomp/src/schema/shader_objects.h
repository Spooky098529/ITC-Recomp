// The views of docs/functions/shader-objects.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/shader-objects.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: VertexShader, new<?>@VertexShader::Create
struct VertexShader : View {
  using View::View;
  static constexpr uint32_t kSize = 592;
  // 6 (the type), bit 0x00100000 set by VertexShader::Create; Create's loads here are the shader file's magic (a measure fault)
  uint32_t common() const { return guest::LoadU32(base, address + 0); }
  void set_common(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t common_address() const { return address + 0; }
  // 1 when made; Create's loads here are the shader file's header size (a measure fault)
  uint32_t reference_count() const { return guest::LoadU32(base, address + 4); }
  void set_reference_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t reference_count_address() const { return address + 4; }
  // probably (D3D's resource header); only Create's load of the shader file's microcode size reaches it (a measure fault)
  uint32_t fence() const { return guest::LoadU32(base, address + 8); }
  void set_fence(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fence_address() const { return address + 8; }
  // 0xFFFF0000 when made (probably D3D's BaseFlush)
  uint32_t base_flush() const { return guest::LoadU32(base, address + 20); }
  void set_base_flush(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t base_flush_address() const { return address + 20; }
  // -1 when made (VertexShader::Init)
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // the microcode (physical memory)
  View microcode() const { return View(base, guest::LoadU32(base, address + 48)); }
  void set_microcode(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t microcode_address() const { return address + 48; }
  // the fetch table's count (the table's +28), at most 32
  uint32_t fetch_count() const { return guest::LoadU32(base, address + 60); }
  void set_fetch_count(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t fetch_count_address() const { return address + 60; }
  // the fetch table's words (u32 each, up to 32)
  uint32_t fetch_words_address() const { return address + 64; }
  // 12 bytes a fetch word from the microcode at 12 x its low 12 bits, probably the vertex fetch instructions
  uint32_t fetch_instructions_address() const { return address + 192; }
  // the copied file header's +20: the fetch table's offset from the header (+592)
  uint32_t header_fetch_table() const { return guest::LoadU32(base, address + 612); }
  void set_header_fetch_table(uint32_t v) const { guest::StoreU32(base, address + 612, v); }
  uint32_t header_fetch_table_address() const { return address + 612; }
};

// allocated; measure: PixelShader, new<?>@PixelShader::Create
struct PixelShader : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // 7 (the type), bit 0x00100000 set by PixelShader::Create; Create's loads here are the shader file's magic (a measure fault)
  uint32_t common() const { return guest::LoadU32(base, address + 0); }
  void set_common(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t common_address() const { return address + 0; }
  // 1 when made; Create's loads here are the shader file's header size (a measure fault)
  uint32_t reference_count() const { return guest::LoadU32(base, address + 4); }
  void set_reference_count(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t reference_count_address() const { return address + 4; }
  // probably (D3D's resource header); only Create's load of the shader file's microcode size reaches it (a measure fault)
  uint32_t fence() const { return guest::LoadU32(base, address + 8); }
  void set_fence(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fence_address() const { return address + 8; }
  // 0xFFFF0000 when made (probably D3D's BaseFlush)
  uint32_t base_flush() const { return guest::LoadU32(base, address + 20); }
  void set_base_flush(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t base_flush_address() const { return address + 20; }
  // the microcode (physical memory)
  View microcode() const { return View(base, guest::LoadU32(base, address + 24)); }
  void set_microcode(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t microcode_address() const { return address + 24; }
};

}  // namespace schema
