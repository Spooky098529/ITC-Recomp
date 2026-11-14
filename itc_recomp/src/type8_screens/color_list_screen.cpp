// The type 8 color list screen (Type8ColorListScreen over ColorListScreen;
// docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code. ColorListScreen's constructor and destructor, the color
// list part's and memcpy by their plain C++; UiMovies::Open through
// Calls::open; operator delete and memcpy where its plain C++ declines guest
// calls.

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "native/buffers.h"
#include "screen_constructors/screen_constructors.h"
#include "type67_screens/list_pages.h"
#include "type8_screens/part_helpers.h"

namespace type8_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

namespace ts = type67_screens;
namespace pg = type67_screens::pages;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemcpy = 0x825A7410;
// The item records (28 bytes each: +8 a count, +12 and +16 the details'
// indexes, +20 and +24 the colors' bits).
constexpr uint32_t kItemRecords = 0x82010BE0;
constexpr uint32_t kRecordBytes = 28;

// slw: 1 shifted left by `amount`'s low six bits (0 from 32 up).
uint32_t Bit(uint32_t amount) {
  return (amount & 0x20) != 0 ? 0 : 1u << (amount & 0x1F);
}

}  // namespace

// --- The constructor and destructor ---

uint64_t ColorList8Construct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223DFC0 Type8ColorListScreen::Type8ColorListScreen");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 144);
  screen_constructors::ColorListScreen(base, s);
  StoreU32(base, s, kColorListVtable);
  type8_screens::ColorPartConstruct(base, r3 + kColorListPart, frame);
  StoreU32(base, s + 9676, s + kColorListPart);
  auto store = [&](uint32_t at, uint32_t value) { StoreU32(base, s + at, value); };
  // The words in the original's order (the node table's +8 and +12 stored
  // directly as +10440 and +10444).
  static constexpr uint32_t kFirst[][2] = {
      {9908, 140},   {9912, 100},   {9916, 101},   {9832, 165},   {9856, 170},   {9900, 87},
      {9840, 166},   {9844, 168},   {10440, 12},   {10952, 67},   {10916, 79},   {9864, kNone},
      {11036, 13},   {9848, 167},   {9852, 169},   {9868, 47},    {9872, 171},   {9888, kNone},
      {9904, 139},   {10920, 76},   {10924, 77},   {10932, 78},   {10968, 66},   {9892, 6},
      {9860, 0},     {9876, 5},     {9920, 20},    {9924, 21},    {9896, 7},     {9728, 6},
      {9824, 1},     {9820, 7},     {10444, 11},   {10956, 64},   {10960, 65},   {10936, 80},
      {9720, 11},    {10972, 68},   {10940, 81},   {10852, 5},
  };
  for (const auto& w : kFirst) store(w[0], w[1]);
  // The node table's address, loaded here (its +296 store uses it).
  const uint32_t table = LoadU32(base, s + 9672);
  static constexpr uint32_t kSecond[][2] = {
      {10976, 69}, {9768, 3}, {10856, 20}, {9724, 16}, {9716, 10}, {9712, 0},
      {9752, 8},   {9756, 9}, {9800, 0},   {9804, 1},  {9748, 0},
  };
  for (const auto& w : kSecond) store(w[0], w[1]);
  StoreU32(base, table + 296, 6);
  for (uint32_t i = 1; i < 4; i++) {
    StoreU32(base, LoadU32(base, s + 9672) + 296 + 4 * i, 6 + i);  // +300 to +308: 7 to 9
  }
  store(9784, 2);
  store(9788, 1);
  // 15 items through the table at +10432 directly: +16 + 4i = 48 + i,
  // +156 + 4i = 1 + i, +312 + 4i = the record's color mask.
  for (uint32_t i = 0; i < 15; i++) {
    const uint32_t item = s + 10448 + 4 * i;
    const uint32_t count_word = kItemRecords + 8 + kRecordBytes * i;
    StoreU32(base, item, 48 + i);
    StoreU32(base, item + 140, 1 + i);
    const int32_t count = int32_t(LoadU32(base, count_word));
    uint32_t mask = 0;
    if (count >= 2) {
      const uint32_t high = Bit(LoadU32(base, count_word + 16));
      mask = high | Bit(LoadU32(base, count_word + 12));
    } else if (count > 0) {
      mask = Bit(LoadU32(base, count_word + 12));
    }
    StoreU32(base, item + 296, mask);
  }
  // Three runs: +9936 + 4i = 129 + i, +9976 + 4i = 90 + i, +10016 + 4i =
  // 10 + i.
  for (uint32_t i = 0; i < 3; i++) {
    store(9936 + 4 * i, 129 + i);
    store(9976 + 4 * i, 90 + i);
    store(10016 + 4 * i, 10 + i);
  }
  static constexpr uint32_t kLast[][2] = {
      {9736, 9},      {9684, 0},    {9808, 0},    {9732, 2},    {9740, 1},    {10056, 141},
      {9816, 0},      {10060, 142}, {9776, 0x8263700C}, {10064, 143}, {10104, 157},
      {10096, 155},   {10100, 156}, {9708, 1},    {9812, 4},    {9928, 152},  {9932, 151},
      {9744, 21},     {9828, 0},
  };
  for (const auto& w : kLast) store(w[0], w[1]);
  return r3;
}

uint64_t ColorList8DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E2B8 Type8ColorListScreen::DeletingDestructor");
  const uint32_t frame = part_detail::Frame(base, sp, 112);
  type8_screens::ColorPartDestruct(base, r3 + kColorListPart, frame, calls);
  ts::ColorListScreenDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    pg::Call1(calls, car_list::kOperatorDelete, r3, frame, 0x8223E2F4);
  }
  return r3;
}

// --- The screen's slots ---

uint64_t ColorList8OpenMovie(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E310 Type8ColorListScreen::OpenMovie");
  return calls.open(LoadU32(base, uint32_t(r3) + 9680), 22585, sp, lr);
}

uint64_t ColorList8ItemInfo(uint8_t* base, uint64_t r4, uint64_t r5, uint32_t sp, uint32_t lr,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E4D0 Type8ColorListScreen::ItemInfo");
  if (uint32_t(r4) > 8) return r5;
  // memcpy(r5, the item's record, 28), a tail call.
  const uint32_t from = uint32_t(r4 * kRecordBytes) + kItemRecords;
  if (native::Buffers_Memcpy(base, uint32_t(r5), from, kRecordBytes, lr)) {
    return r5;
  }
  const uint64_t args[6] = {r5, from, kRecordBytes, 0, 0, 0};
  return calls.call_set(kMemcpy, args, 3, sp, lr);
}

}  // namespace type8_screens
