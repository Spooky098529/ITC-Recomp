// The data tables as plain C++: see data_tables.h.

#include "data_tables/data_tables.h"

#include "base/log.h"
#include "archive/archive.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

#include <bit>

namespace data_tables {

namespace {

// The loads pending (+4 of the table object) down by one.
void CountDown(uint8_t* base) {
  const uint32_t object = guest::LoadU32(base, kTables);
  guest::StoreU32(base, object + 4, guest::LoadU32(base, object + 4) - 1);
}

// A table's callback: each entry swapped, up to and including the first
// whose word at `end` (after the swap) is -1; then the count down.
template <uint32_t (*Swap)(uint8_t*, uint32_t)>
uint32_t SwapTable(uint8_t* base, uint32_t table, uint32_t size, uint32_t end) {
  uint32_t entry = table;
  uint32_t last;
  for (;;) {
    last = Swap(base, entry);
    if (guest::LoadU32(base, entry + end) == 0xFFFFFFFFu) {
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
  const float single = std::bit_cast<float>(guest::LoadU32(base, address));
  const double widened = single;
  const uint32_t word = std::bit_cast<uint32_t>(float(widened));
  guest::StoreU32(base, address, std::byteswap(word));
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
  guest::StoreU32(base, object + 8, 400);
  guest::StoreU32(base, object + 4, guest::LoadU32(base, object + 4) - 1);
}

void OnLoaded84(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821170C0 DataTables::OnLoaded84");
  CountDown(base);
}

uint32_t On56Loaded(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_821170F0 DataTables::On56Loaded");
  return SwapTable<SwapEntry56>(base, table, 56, 0);
}

uint32_t On20Loaded(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82116FF0 DataTables::On20Loaded");
  return SwapTable<SwapEntry20>(base, table, 20, 0);
}

uint32_t On40Loaded(uint8_t* base, uint32_t table) {
  ITC_NATIVE_RUNNING("sub_82117058 DataTables::On40Loaded");
  return SwapTable<SwapEntry40>(base, table, 40, 4);
}

uint32_t IsLoading(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82117158 DataTables::IsLoading");
  return guest::LoadU32(base, guest::LoadU32(base, kTables) + 4) != 0 ? 1 : 0;
}

uint32_t Item(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82117178 DataTables::Item");
  return guest::LoadU32(base, guest::LoadU32(base, kTables) + 16) + 3008 * id;
}

uint32_t RecordsItem(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82116DA8 ItemRecords::Item");
  return Item(base, id);
}

uint32_t Entry56(uint8_t* base, uint32_t table, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82117190 DataTables::Entry56");
  return guest::LoadU32(base, guest::LoadU32(base, kTables) + 4 * (table + 5)) + 56 * entry;
}

int32_t Find20(uint8_t* base, uint32_t key8, uint32_t key0) {
  ITC_NATIVE_RUNNING("sub_821171B0 DataTables::Find20");
  uint32_t entry = guest::LoadU32(base, guest::LoadU32(base, kTables) + 52);
  for (int32_t index = 0;; index++, entry += 20) {
    const uint32_t first = guest::LoadU32(base, entry);
    if (first == 0xFFFFFFFFu) {
      return -1;
    }
    if (guest::LoadU32(base, entry + 8) == key8 && first == key0) {
      return index;
    }
  }
}

uint32_t Entry20(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82117210 DataTables::Entry20");
  return guest::LoadU32(base, guest::LoadU32(base, kTables) + 52) + 20 * index;
}

uint32_t Entry40(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82117228 DataTables::Entry40");
  return guest::LoadU32(base, guest::LoadU32(base, kTables) + 56) + 40 * index;
}

namespace {

// The TOC's size for `id` as Init reads it: the unpacked size, or the stored
// size when that's 0. `toc` is Archive::Toc's answer.
uint32_t TocSize(uint8_t* base, uint32_t toc, uint32_t id) {
  const uint32_t unpacked = guest::LoadU32(base, toc + 16 * id + 24);
  return unpacked != 0 ? unpacked : guest::LoadU32(base, toc + 16 * id + 20);
}

// +4 (the loads pending) up by one.
void CountUp(uint8_t* base, uint32_t object) {
  guest::StoreU32(base, object + 4, guest::LoadU32(base, object + 4) + 1);
}

}  // namespace

std::optional<uint32_t> Init(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82116DB0 DataTables::Init");
  const auto made = memory::crt::OperatorNew(base, 64);
  if (!made) {
    return std::nullopt;
  }
  const uint32_t object = *made;
  guest::StoreU32(base, kTables, object);
  guest::StoreU32(base, object + 12, 0);
  guest::StoreU32(base, object + 4, 0);
  guest::StoreU32(base, object + 8, 0);
  if (guest::LoadU32(base, object + 12) != 0) {
    return uint32_t(object);  // never: the store above
  }
  uint32_t total = 0;
  uint32_t toc = 0;
  for (;;) {
    if (total != 0) {
      const auto block = memory::crt::OperatorNewArray(base, total);
      if (!block) {
        return std::nullopt;
      }
      guest::StoreU32(base, object + 12, *block);
    }
    const uint32_t block = guest::LoadU32(base, object + 12);
    // Each table pointer: the block plus the sizes so far, or 0 when the
    // one before was 0 (the first: the block itself).
    uint32_t pointer = block;
    const auto next = [&](uint32_t size) { pointer = pointer != 0 ? block + size : 0u; };
    guest::StoreU32(base, object + 4, 0);
    CountUp(base, object);
    guest::StoreU32(base, object + 16, pointer);
    toc = archive::Toc(base, 94);
    uint32_t size = TocSize(base, toc, 94);
    next(size);
    CountUp(base, object);
    guest::StoreU32(base, object + 60, pointer);
    toc = archive::Toc(base, 84);
    size += TocSize(base, toc, 84);
    next(size);
    for (uint32_t i = 0; i < 8; i++) {
      CountUp(base, object);
      guest::StoreU32(base, object + 20 + 4 * i, pointer);
      size += archive::AssetSize(base, guest::LoadU32(base, kTableIds + 4 * i));
      next(size);
    }
    CountUp(base, object);
    guest::StoreU32(base, object + 52, pointer);
    toc = archive::Toc(base, 95);
    size += TocSize(base, toc, 95);
    next(size);
    CountUp(base, object);
    guest::StoreU32(base, object + 56, pointer);
    toc = archive::Toc(base, 96);
    total = size + TocSize(base, toc, 96);
    if (guest::LoadU32(base, object + 12) != 0) {
      return toc;
    }
  }
}

int64_t Load(uint8_t* base, const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82117240 DataTables::Load");
  int64_t serial = 0;
  const auto one = [&](uint32_t id, uint32_t offset, uint32_t callback, bool table_context) {
    const uint32_t object = guest::LoadU32(base, kTables);
    const uint32_t table = guest::LoadU32(base, object + offset);
    serial = load(id, table, callback, table_context ? table : object);
    guest::StoreU32(base, guest::LoadU32(base, kTables), uint32_t(serial));
  };
  one(94, 16, 0x821170D8, false);
  one(84, 60, 0x821170C0, false);
  for (uint32_t i = 0; i < 8; i++) {
    one(guest::LoadU32(base, kTableIds + 4 * i), 20 + 4 * i, 0x821170F0, true);
  }
  one(95, 52, 0x82116FF0, true);
  one(96, 56, 0x82117058, true);
  return serial;
}

}  // namespace data_tables
