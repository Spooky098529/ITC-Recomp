// The data tables as plain C++: see data_tables.h.

#include "data_tables/data_tables.h"

#include "base/log.h"
#include "archive/archive.h"
#include "guest.h"
#include "schema/archive.h"
#include "schema/data_tables.h"

#include <bit>

namespace data_tables {

namespace {

using G = schema::DataTablesGlobals;

// The loads pending down by one.
void CountDown(uint8_t* base) {
  const schema::DataTables object = G::tables(base);
  object.set_pending(object.pending() - 1);
}

// The entries' end marks, after the swap: -1 ends the table.
uint32_t EndMark20(uint8_t* base, uint32_t entry) { return schema::DataTablesEntry20(base, entry).key0(); }
uint32_t EndMark56(uint8_t* base, uint32_t entry) { return schema::DataTablesEntry56(base, entry).end_mark(); }
uint32_t EndMark40(uint8_t* base, uint32_t entry) { return schema::DataTablesEntry40(base, entry).end_mark(); }

// A table's callback: each entry swapped, up to and including the first
// whose end mark (after the swap) is -1; then the count down.
template <uint32_t (*Swap)(uint8_t*, uint32_t), uint32_t (*EndMark)(uint8_t*, uint32_t)>
uint32_t SwapTable(uint8_t* base, uint32_t table, uint32_t size) {
  uint32_t entry = table;
  uint32_t last;
  for (;;) {
    last = Swap(base, entry);
    if (EndMark(base, entry) == 0xFFFFFFFFu) {
      break;
    }
    entry += size;
  }
  CountDown(base);
  return last;
}

}  // namespace

void SwapWord(uint8_t* base, uint32_t address) {
  ITC_NATIVE_RUNNING("sub_820C97D8 SwapWord");
  // The translated code's steps: lfs (the single widened to double), stfs
  // (narrowed back), on the default floating-point model as generated/ is
  // built with. In the release build the compiler folds the round trip
  // there (the word is stored as it was loaded, seen in the disassembly),
  // and folds it here the same way, so a signaling NaN's bits come through
  // unchanged in both; checked exactly by the harness.
  const schema::DataTablesSwappedWord swapped(base, address);
  const float single = std::bit_cast<float>(swapped.word());
  const double widened = single;
  const uint32_t word = std::bit_cast<uint32_t>(float(widened));
  swapped.set_word(std::byteswap(word));
}

uint32_t SwapEntry20(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82111AE0 Entry20::Swap");
  for (uint32_t offset = 0; offset < 20; offset += 4) {
    SwapWord(base, entry + offset);
  }
  return entry + 16;
}

uint32_t SwapEntry56(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82111B30 Entry56::Swap");
  for (uint32_t offset = 0; offset < 56; offset += 4) {
    SwapWord(base, entry + offset);
  }
  return entry + 52;
}

uint32_t SwapEntry40(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_821128E0 Entry40::Swap");
  for (uint32_t offset = 0; offset < 40; offset += 4) {
    SwapWord(base, entry + offset);
  }
  return entry + 36;
}

void OnItemsLoaded(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_821170D8 DataTables::OnItemsLoaded");
  const schema::DataTables tables(base, object);
  tables.set_item_count(400);
  tables.set_pending(tables.pending() - 1);
}

void OnLoaded84(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821170C0 DataTables::OnLoaded84");
  CountDown(base);
}

uint32_t On56Loaded(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_821170F0 DataTables::On56Loaded");
  return SwapTable<SwapEntry56, EndMark56>(base, table, 56);
}

uint32_t On20Loaded(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82116FF0 DataTables::On20Loaded");
  return SwapTable<SwapEntry20, EndMark20>(base, table, 20);
}

uint32_t On40Loaded(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82117058 DataTables::On40Loaded");
  return SwapTable<SwapEntry40, EndMark40>(base, table, 40);
}

uint32_t IsLoading(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82117158 DataTables::IsLoading");
  return G::tables(base).pending() != 0 ? 1 : 0;
}

uint32_t Item(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82117178 DataTables::Item");
  return G::tables(base).items().address + 3008 * id;
}

uint32_t RecordsItem(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82116DA8 ItemRecords::Item");
  return Item(base, id);
}

uint32_t Entry56(uint8_t* base, uint32_t table, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82117190 DataTables::Entry56");
  return uint32_t(G::tables(base).load_at(20 + 4 * table, 4)) + 56 * entry;
}

int32_t Find20(uint8_t* base, uint32_t key8, uint32_t key0) {
  ITC_NATIVE_RUNNING("sub_821171B0 DataTables::Find20");
  uint32_t entry = G::tables(base).table20().address;
  for (int32_t index = 0;; index++, entry += 20) {
    const schema::DataTablesEntry20 e(base, entry);
    const uint32_t first = e.key0();
    if (first == 0xFFFFFFFFu) {
      return -1;
    }
    if (e.key8() == key8 && first == key0) {
      return index;
    }
  }
}

uint32_t Entry20(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82117210 DataTables::Entry20");
  return G::tables(base).table20().address + 20 * index;
}

uint32_t Entry40(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82117228 DataTables::Entry40");
  return G::tables(base).table40().address + 40 * index;
}

namespace {

// The TOC's size for `id` as Init reads it: the unpacked size, or the stored
// size when that's 0. `toc` is Archive::Toc's answer.
uint32_t TocSize(uint8_t* base, uint32_t toc, uint32_t id) {
  // The TOC's 16-byte header, then id's entry at (id + 1) x 16.
  const schema::ArchiveTocEntry entry(base, toc + 16 * (id + 1));
  const uint32_t unpacked = entry.unpacked_size();
  return unpacked != 0 ? unpacked : entry.stored_size();
}

// The loads pending up by one.
void CountUp(const schema::DataTables& object) {
  object.set_pending(object.pending() + 1);
}

}  // namespace

std::optional<uint32_t> Init(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82116DB0 DataTables::Init");
  // In host memory (E5 step 5): the original's operator new(64).
  const schema::DataTables object = schema::DataTables::New(base);
  G::set_tables(base, object);
  object.set_block(0);
  object.set_pending(0);
  object.set_item_count(0);
  if (object.block().address != 0) {
    return object.address;  // never: the store above
  }
  uint32_t total = 0;
  uint32_t toc = 0;
  for (;;) {
    if (total != 0) {
      // A blob (E5 step 5): the files' bytes in PC memory, read by address
      // as guest memory is; the original's operator new[](total). 2,048
      // bytes more: the archive reads an asset stored unpacked in whole
      // 2,048-byte sectors straight into its destination
      // (archive_streaming.cpp), so the last table's read runs up to a
      // sector past the block's end (into the guest heap's next bytes in
      // the original; past a blob's end, a host heap overrun).
      object.set_block(schema::DataTablesBlock::New(base, total + 2048));
    }
    const uint32_t block = object.block().address;
    // Each table pointer: the block plus the sizes so far, or 0 when the
    // one before was 0 (the first: the block itself).
    uint32_t pointer = block;
    const auto next = [&](uint32_t size) { pointer = pointer != 0 ? block + size : 0u; };
    object.set_pending(0);
    CountUp(object);
    object.set_items(pointer);
    toc = archive::Toc(base, 94);
    uint32_t size = TocSize(base, toc, 94);
    next(size);
    CountUp(object);
    object.set_asset84(pointer);
    toc = archive::Toc(base, 84);
    size += TocSize(base, toc, 84);
    next(size);
    for (uint32_t i = 0; i < 8; i++) {
      CountUp(object);
      object.store_at(20 + 4 * i, 4, pointer);  // tables56's i-th pointer
      size += archive::AssetSize(base, guest::LoadU32(base, G::table_ids_address() + 4 * i));
      next(size);
    }
    CountUp(object);
    object.set_table20(pointer);
    toc = archive::Toc(base, 95);
    size += TocSize(base, toc, 95);
    next(size);
    CountUp(object);
    object.set_table40(pointer);
    toc = archive::Toc(base, 96);
    total = size + TocSize(base, toc, 96);
    if (object.block().address != 0) {
      return toc;
    }
  }
}

int64_t Load(uint8_t* base, const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82117240 DataTables::Load");
  int64_t serial = 0;
  // `offset`: the table's pointer's offset in the object (+16 items, +60
  // asset 84, +20 to +48 the 56-byte tables, +52 table20, +56 table40).
  const auto one = [&](uint32_t id, uint32_t offset, uint32_t callback, bool table_context) {
    const schema::DataTables object = G::tables(base);
    const uint32_t table = uint32_t(object.load_at(offset, 4));
    serial = load(id, table, callback, table_context ? table : object.address);
    G::tables(base).set_last_serial(uint32_t(serial));
  };
  one(94, 16, 0x821170D8, false);
  one(84, 60, 0x821170C0, false);
  for (uint32_t i = 0; i < 8; i++) {
    one(guest::LoadU32(base, G::table_ids_address() + 4 * i), 20 + 4 * i, 0x821170F0, true);
  }
  one(95, 52, 0x82116FF0, true);
  one(96, 56, 0x82117058, true);
  return serial;
}

}  // namespace data_tables
