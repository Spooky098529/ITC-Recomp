// The data tables as plain C++: see data_tables.h.

#include "data_tables/data_tables.h"

#include "base/log.h"
#include "guest.h"

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

}  // namespace data_tables
