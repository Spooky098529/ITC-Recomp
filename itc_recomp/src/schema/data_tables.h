// The views of docs/functions/data-tables.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/data-tables.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: new<64>@DataTables::Init, DataTables::OnItemsLoaded.r6
#pragma pack(push, 1)
struct DataTablesData {
  uint32_t last_serial = 0;  // +0
  uint32_t pending = 0;  // +4
  uint32_t item_count = 0;  // +8
  uint32_t block = 0;  // +12
  uint32_t items = 0;  // +16
  uint8_t tables56[32] = {};  // +20, guest order
  uint32_t table20 = 0;  // +52
  uint32_t table40 = 0;  // +56
  uint32_t asset84 = 0;  // +60
};
#pragma pack(pop)
static_assert(sizeof(DataTablesData) == 64);
inline constexpr memory::LayoutField kDataTablesFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {52, 4}, {56, 4}, {60, 4}};
inline constexpr memory::Layout kDataTablesLayout = {kDataTablesFields, 8};

struct DataTables : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  DataTablesData* data() const { return static_cast<DataTablesData*>(memory::HandleObject(address)); }
  static DataTables New() {
    return DataTables(memory::MakeHandle(new DataTablesData(), sizeof(DataTablesData), "DataTables", &kDataTablesLayout));
  }
  template <uint32_t Stride = kSize>
  static DataTables NewArray(uint32_t count) {
    static_assert(sizeof(DataTablesData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<DataTablesData*>(::operator new(sizeof(DataTablesData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) DataTablesData();
    return DataTables(memory::MakeHandle(p, sizeof(DataTablesData) * count, "DataTables", &kDataTablesLayout, sizeof(DataTablesData)));
  }
  DataTables at(uint32_t i) const { return DataTables(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  DataTablesData* direct() const { return static_cast<DataTablesData*>(memory::HandleOwn(address, &kDataTablesLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kDataTablesLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kDataTablesLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the last load's serial (Archive::Load's; not read)
  uint32_t last_serial_address() const { return address + 0; }
  uint32_t last_serial_bits() const { if (auto* d = direct()) return d->last_serial; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_last_serial_bits(uint32_t v) const { if (auto* d = direct()) { d->last_serial = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t last_serial() const { if (auto* d = direct()) return d->last_serial; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_last_serial(uint32_t v) const { if (auto* d = direct()) { d->last_serial = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the loads pending: 12 at boot, each callback counts down (DataTables::IsLoading)
  uint32_t pending_address() const { return address + 4; }
  uint32_t pending_bits() const { if (auto* d = direct()) return d->pending; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_pending_bits(uint32_t v) const { if (auto* d = direct()) { d->pending = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t pending() const { if (auto* d = direct()) return d->pending; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_pending(uint32_t v) const { if (auto* d = direct()) { d->pending = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // 400, set by asset 94's callback
  uint32_t item_count_address() const { return address + 8; }
  uint32_t item_count_bits() const { if (auto* d = direct()) return d->item_count; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_item_count_bits(uint32_t v) const { if (auto* d = direct()) { d->item_count = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t item_count() const { if (auto* d = direct()) return d->item_count; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_item_count(uint32_t v) const { if (auto* d = direct()) { d->item_count = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // the block all the tables are in (operator new[]; a blob)
  uint32_t block_address() const { return address + 12; }
  uint32_t block_bits() const { if (auto* d = direct()) return d->block; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_block_bits(uint32_t v) const { if (auto* d = direct()) { d->block = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  View block() const { return View(block_bits()); }
  void set_block(uint32_t v) const { set_block_bits(v); }
  // asset 94: 400 entries of 3,008 bytes (DataTables::Item)
  uint32_t items_address() const { return address + 16; }
  uint32_t items_bits() const { if (auto* d = direct()) return d->items; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_items_bits(uint32_t v) const { if (auto* d = direct()) { d->items = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  View items() const { return View(items_bits()); }
  void set_items(uint32_t v) const { set_items_bits(v); }
  // eight pointers: the 56-byte tables (asset ids at 0x82616C64; DataTables::Entry56)
  uint32_t tables56_address() const { return address + 20; }
  uint8_t* tables56() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 20 : guest::Pointer(address + 20); }
  // asset 95: 20-byte entries, -1 at +0 ends it
  uint32_t table20_address() const { return address + 52; }
  uint32_t table20_bits() const { if (auto* d = direct()) return d->table20; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_table20_bits(uint32_t v) const { if (auto* d = direct()) { d->table20 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  template <typename T = DataTablesEntry20>
  T table20() const { return T(table20_bits()); }
  void set_table20(const View& w) const { set_table20_bits(w.address); }
  void set_table20(uint32_t v) const { set_table20_bits(v); }
  // asset 96: 40-byte entries, -1 at +4 ends it
  uint32_t table40_address() const { return address + 56; }
  uint32_t table40_bits() const { if (auto* d = direct()) return d->table40; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_table40_bits(uint32_t v) const { if (auto* d = direct()) { d->table40 = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  template <typename T = DataTablesEntry40>
  T table40() const { return T(table40_bits()); }
  void set_table40(const View& w) const { set_table40_bits(w.address); }
  void set_table40(uint32_t v) const { set_table40_bits(v); }
  // asset 84 (no reader found besides the load)
  uint32_t asset84_address() const { return address + 60; }
  uint32_t asset84_bits() const { if (auto* d = direct()) return d->asset84; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_asset84_bits(uint32_t v) const { if (auto* d = direct()) { d->asset84 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  View asset84() const { return View(asset84_bits()); }
  void set_asset84(uint32_t v) const { set_asset84_bits(v); }
};

// allocated; measure: -
struct DataTablesBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static DataTablesBlock New(uint32_t size) { return DataTablesBlock(memory::NewBlob(size, "DataTablesBlock")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// file; measure: new<64>@DataTables::Init.+52, DataTables::On20Loaded.r6
struct DataTablesEntry20 : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // -1 ends the table; Find20's second key
  uint32_t key0() const { return guest::LoadU32(address + 0); }
  void set_key0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t key0_address() const { return address + 0; }
  // Find20's first key
  uint32_t key8() const { return guest::LoadU32(address + 8); }
  void set_key8(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t key8_address() const { return address + 8; }
  // the next entry's key0 (the walks read it from the entry before)
  uint32_t next_key0() const { return guest::LoadU32(address + 20); }
  void set_next_key0(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t next_key0_address() const { return address + 20; }
};

// file; measure: DataTables::On40Loaded.r6
struct DataTablesEntry40 : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // -1 ends the table (after the swap)
  uint32_t end_mark() const { return guest::LoadU32(address + 4); }
  void set_end_mark(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t end_mark_address() const { return address + 4; }
  // the next entry's end_mark (the walk reads it from the entry before)
  uint32_t next_end_mark() const { return guest::LoadU32(address + 44); }
  void set_next_end_mark(uint32_t v) const { guest::StoreU32(address + 44, v); }
  uint32_t next_end_mark_address() const { return address + 44; }
};

// file; measure: DataTables::On56Loaded.r6
struct DataTablesEntry56 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // -1 ends the table (after the swap)
  uint32_t end_mark() const { return guest::LoadU32(address + 0); }
  void set_end_mark(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t end_mark_address() const { return address + 0; }
  // the next entry's end_mark (the walk reads it from the entry before)
  uint32_t next_end_mark() const { return guest::LoadU32(address + 56); }
  void set_next_end_mark(uint32_t v) const { guest::StoreU32(address + 56, v); }
  uint32_t next_end_mark_address() const { return address + 56; }
};

// file; measure: SwapWord.r3
struct DataTablesSwappedWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // read as a single (the translated code's lfs)
  float value() const { return guest::LoadFloat(address + 0); }
  void set_value(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
  // the word as the hand port loads and stores it whole
  uint32_t word() const { return guest::LoadU32(address + 0); }
  void set_word(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t word_address() const { return address + 0; }
  uint8_t byte0() const { return guest::LoadU8(address + 0); }
  void set_byte0(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t byte0_address() const { return address + 0; }
  uint8_t byte1() const { return guest::LoadU8(address + 1); }
  void set_byte1(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t byte1_address() const { return address + 1; }
  uint8_t byte2() const { return guest::LoadU8(address + 2); }
  void set_byte2(uint8_t v) const { guest::StoreU8(address + 2, v); }
  uint32_t byte2_address() const { return address + 2; }
  uint8_t byte3() const { return guest::LoadU8(address + 3); }
  void set_byte3(uint8_t v) const { guest::StoreU8(address + 3, v); }
  uint32_t byte3_address() const { return address + 3; }
};

// The image words docs/functions/data-tables.md's code uses.
struct DataTablesGlobals {
  // the data tables (DataTables::Init)
  template <typename T = DataTables>
  static T tables() { return T(guest::LoadU32(0x826C5CA0u)); }
  static void set_tables(const View& v) { guest::StoreU32(0x826C5CA0u, v.address); }
  static void set_tables(uint32_t v) { guest::StoreU32(0x826C5CA0u, v); }
  static constexpr uint32_t tables_address() { return 0x826C5CA0u; }
  // the eight 56-byte tables' asset ids: 86, 87, 91, 88, 92, 89, 90, 93
  static constexpr uint32_t table_ids_address() { return 0x82616C64u; }
};

}  // namespace schema
