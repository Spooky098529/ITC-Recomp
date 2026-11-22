// The views of docs/functions/file-layer.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/file-layer.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: GameFile, GameFile.r3, AsyncFile, AsyncFile.r3, sub_8258C5B0.r3
struct GameFile : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr uint32_t kVtable = 0x82002270;
  // 0x82002270 GameFile, 0x8206CC0C AsyncFile, 0x8206C05C File, 0x8206BFF8 the base stream
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // -1 when closed (AsyncFile::Detach returns it and sets -1)
  uint32_t handle() const { return guest::LoadU32(base, address + 4); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t handle_address() const { return address + 4; }
  uint32_t position() const { return guest::LoadU32(base, address + 8); }
  void set_position(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t position_address() const { return address + 8; }
  uint32_t size() const { return guest::LoadU32(base, address + 12); }
  void set_size(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t size_address() const { return address + 12; }
};

// file; measure: Path, Path.r3
struct Path : View {
  using View::View;
  static constexpr uint32_t kSize = 516;
  static constexpr uint32_t kVtable = 0x8206CC3C;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the full path's first character (probably a 512-byte buffer; Path::Build's sprintf writes it)
  uint8_t text() const { return guest::LoadU8(base, address + 4); }
  void set_text(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t text_address() const { return address + 4; }
};

// file; measure: Path::Build.ret
struct PathText : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t first_char() const { return guest::LoadU8(base, address + 0); }
  void set_first_char(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t first_char_address() const { return address + 0; }
};

// file; measure: GamePath, GamePath.r3
struct GamePath : View {
  using View::View;
  static constexpr uint32_t kSize = 5;
  static constexpr uint32_t kVtable = 0x820022A0;
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the path's first character (its size not known)
  uint8_t text() const { return guest::LoadU8(base, address + 4); }
  void set_text(uint8_t v) const { guest::StoreU8(base, address + 4, v); }
  uint32_t text_address() const { return address + 4; }
};

// The image words docs/functions/file-layer.md's code uses.
struct FileLayerGlobals {
  // Path::Build's kind 0 format, "game:\%s"
  static constexpr uint32_t format_kind0_address() { return 0x8206CC48u; }
  // Path::Build's kind 1 format (not read)
  static constexpr uint32_t format_kind1_address() { return 0x8206CC54u; }
  // Path::Build's kind 2 format (not read; with the two strings below and the name)
  static constexpr uint32_t format_kind2_address() { return 0x8206CC60u; }
  // a std::string (std-string.md), Path::Build's kind 2's first argument
  static constexpr uint32_t prefix_string_address() { return 0x8265EE7Cu; }
  // a std::string, XENON_PRJ\TSP (probably a development path), kind 2's second argument
  static constexpr uint32_t project_string_address() { return 0x8265EE98u; }
};

}  // namespace schema
