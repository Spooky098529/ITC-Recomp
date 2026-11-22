// The views of docs/functions/data-tables.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/data-tables.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<64>@DataTables::Init, DataTables::OnItemsLoaded.r6
struct DataTables : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the last load's serial (Archive::Load's; not read)
  uint32_t last_serial() const { return guest::LoadU32(base, address + 0); }
  void set_last_serial(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t last_serial_address() const { return address + 0; }
  // the loads pending: 12 at boot, each callback counts down (DataTables::IsLoading)
  uint32_t pending() const { return guest::LoadU32(base, address + 4); }
  void set_pending(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t pending_address() const { return address + 4; }
  // 400, set by asset 94's callback
  uint32_t item_count() const { return guest::LoadU32(base, address + 8); }
  void set_item_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t item_count_address() const { return address + 8; }
  // the block all the tables are in (operator new[])
  View block() const { return View(base, guest::LoadU32(base, address + 12)); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t block_address() const { return address + 12; }
  // asset 94: 400 entries of 3,008 bytes (DataTables::Item)
  View items() const { return View(base, guest::LoadU32(base, address + 16)); }
  void set_items(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t items_address() const { return address + 16; }
  // eight pointers: the 56-byte tables (asset ids at 0x82616C64; DataTables::Entry56)
  uint32_t tables56_address() const { return address + 20; }
  // asset 95: 20-byte entries, -1 at +0 ends it
  template <typename T = DataTablesEntry20>
  T table20() const { return T(base, guest::LoadU32(base, address + 52)); }
  void set_table20(const View& v) const { guest::StoreU32(base, address + 52, v.address); }
  void set_table20(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t table20_address() const { return address + 52; }
  // asset 96: 40-byte entries, -1 at +4 ends it
  template <typename T = DataTablesEntry40>
  T table40() const { return T(base, guest::LoadU32(base, address + 56)); }
  void set_table40(const View& v) const { guest::StoreU32(base, address + 56, v.address); }
  void set_table40(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t table40_address() const { return address + 56; }
  // asset 84 (no reader found besides the load)
  View asset84() const { return View(base, guest::LoadU32(base, address + 60)); }
  void set_asset84(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t asset84_address() const { return address + 60; }
};

// file; measure: new<64>@DataTables::Init.+52, DataTables::On20Loaded.r6
struct DataTablesEntry20 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // -1 ends the table; Find20's second key
  uint32_t key0() const { return guest::LoadU32(base, address + 0); }
  void set_key0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t key0_address() const { return address + 0; }
  // Find20's first key
  uint32_t key8() const { return guest::LoadU32(base, address + 8); }
  void set_key8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t key8_address() const { return address + 8; }
  // the next entry's key0 (the walks read it from the entry before)
  uint32_t next_key0() const { return guest::LoadU32(base, address + 20); }
  void set_next_key0(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t next_key0_address() const { return address + 20; }
};

// file; measure: DataTables::On40Loaded.r6
struct DataTablesEntry40 : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // -1 ends the table (after the swap)
  uint32_t end_mark() const { return guest::LoadU32(base, address + 4); }
  void set_end_mark(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t end_mark_address() const { return address + 4; }
  // the next entry's end_mark (the walk reads it from the entry before)
  uint32_t next_end_mark() const { return guest::LoadU32(base, address + 44); }
  void set_next_end_mark(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t next_end_mark_address() const { return address + 44; }
};

// file; measure: DataTables::On56Loaded.r6
struct DataTablesEntry56 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // -1 ends the table (after the swap)
  uint32_t end_mark() const { return guest::LoadU32(base, address + 0); }
  void set_end_mark(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t end_mark_address() const { return address + 0; }
  // the next entry's end_mark (the walk reads it from the entry before)
  uint32_t next_end_mark() const { return guest::LoadU32(base, address + 56); }
  void set_next_end_mark(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t next_end_mark_address() const { return address + 56; }
};

// file; measure: SwapWord.r3
struct DataTablesSwappedWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // read as a single (the translated code's lfs)
  float value() const { return guest::LoadFloat(base, address + 0); }
  void set_value(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
  // the word as the hand port loads and stores it whole
  uint32_t word() const { return guest::LoadU32(base, address + 0); }
  void set_word(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_address() const { return address + 0; }
  uint8_t byte0() const { return guest::LoadU8(base, address + 0); }
  void set_byte0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t byte0_address() const { return address + 0; }
  uint8_t byte1() const { return guest::LoadU8(base, address + 1); }
  void set_byte1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t byte1_address() const { return address + 1; }
  uint8_t byte2() const { return guest::LoadU8(base, address + 2); }
  void set_byte2(uint8_t v) const { guest::StoreU8(base, address + 2, v); }
  uint32_t byte2_address() const { return address + 2; }
  uint8_t byte3() const { return guest::LoadU8(base, address + 3); }
  void set_byte3(uint8_t v) const { guest::StoreU8(base, address + 3, v); }
  uint32_t byte3_address() const { return address + 3; }
};

// The image words docs/functions/data-tables.md's code uses.
struct DataTablesGlobals {
  // the data tables (DataTables::Init)
  template <typename T = DataTables>
  static T tables(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5CA0u)); }
  static void set_tables(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5CA0u, v.address); }
  static void set_tables(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5CA0u, v); }
  static constexpr uint32_t tables_address() { return 0x826C5CA0u; }
  // the eight 56-byte tables' asset ids: 86, 87, 91, 88, 92, 89, 90, 93
  static constexpr uint32_t table_ids_address() { return 0x82616C64u; }
};

}  // namespace schema
