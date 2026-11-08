// The car cards (docs/functions/front-end-screen-helpers.md): see
// front_end_screen_helpers.h.

#include "front_end_screen_helpers/front_end_screen_helpers.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace front_end_screen_helpers {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

uint32_t FindMovie(uint8_t* base, uint32_t id, uint32_t key) {
  Store(base, key, id);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

// The maker's logo (a frame choice) for a car id.
uint32_t Logo(int32_t id) {
  if (id < 35) {
    return id == 2 ? 35 : uint32_t(id);
  }
  if (id == 43) {
    return 35;
  }
  if (uint32_t(id - 47) <= 9) {
    return uint32_t(id - 11);
  }
  return id == 71 ? 44 : 0xFFFFFFFFu;
}

// The entry's maker byte (+769) and id (+8432) as the originals add them
// (64-bit sums of the zero-extended loads).
uint64_t Maker(uint8_t* base, uint32_t entry) {
  return uint64_t(guest::LoadU8(base, entry + 769)) + 1934;
}
uint64_t Id(uint8_t* base, uint32_t entry, uint32_t first) {
  return uint64_t(Load(base, entry + 8432)) + first;
}

}  // namespace

uint64_t CarCardFill(uint8_t* base, uint32_t node, uint32_t entry, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821821C8 CarCard::Fill");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  const uint32_t logo = ui_movies::Child(base, node, 3);
  Store(base, logo + 108, Logo(int32_t(Load(base, entry + 8432))));
  calls.Call(kSetNodeText, frame, 0x82182278, movie, ui_movies::Child(base, node, 5),
             Maker(base, entry));
  calls.Call(kSetNodeText, frame, 0x82182298, movie, ui_movies::Child(base, node, 6),
             Id(base, entry, 1646));
  calls.Call(kSetNodeText, frame, 0x821822B8, movie, ui_movies::Child(base, node, 7),
             Id(base, entry, 1718));
  calls.Call(kSetNodeNumberText, frame, 0x821822D4, movie, ui_movies::Child(base, node, 8),
             Load(base, entry + 744));
  return calls.Call(kSetNodeText, frame, 0x821822F0, movie, ui_movies::Child(base, node, 9),
                    435);
}

uint64_t CarCardsFillAll(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821822F8 CarCards::FillAll");
  const uint32_t frame = Frame(base, sp, 144);
  Store(base, frame + 80, 4);
  const uint32_t pair = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = Load(base, pair);
  uint64_t r3 = pair;
  const int32_t count = int32_t(Load(base, Load(base, Load(base, kCarData) + 1136) + 512));
  for (int32_t i = 0; i < 4; i++) {
    if (i >= count) {
      continue;
    }
    const uint32_t scroll = Load(base, Load(base, kCarList) + 4);
    const uint32_t node = ui_movies::FindNode(base, movie, 7, uint32_t(i) + 5);
    const uint32_t entries = Load(base, Load(base, kCarData) + 1136);
    const uint32_t entry = Load(base, ((scroll + uint32_t(i)) << 2) + entries);
    r3 = CarCardFill(base, node, entry, frame, calls);
  }
  return r3;
}

uint64_t CarCardFillDetail(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821827E0 CarCard::FillDetail");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = FindMovie(base, 18, frame + 80);
  const uint32_t data = Load(base, kCarData);
  const uint32_t index = (Load(base, data + 52) + Load(base, data + 48)) << 2;
  const uint32_t entries = Load(base, data + 1136);
  const uint32_t entry = Load(base, index + entries);
  Store(base, movie + 0xA000, 0);
  using ui_movies::FindNode;
  const uint32_t n4 = FindNode(base, movie, 0xFFFFFFFFu, 4);
  const uint32_t n5 = FindNode(base, movie, 0xFFFFFFFFu, 5);
  const uint32_t n6 = FindNode(base, movie, 0xFFFFFFFFu, 6);
  const uint32_t n7 = FindNode(base, movie, 0xFFFFFFFFu, 7);
  const uint32_t a4 = FindNode(base, movie, 1, 4);
  const uint32_t a2 = FindNode(base, movie, 1, 2);
  const uint32_t a3 = FindNode(base, movie, 1, 3);
  const uint32_t b3 = FindNode(base, movie, 2, 3);
  const uint32_t b4 = FindNode(base, movie, 2, 4);
  calls.Call(kSetNodeText, frame, 0x821828FC, movie, n4, 20);
  calls.Call(kSetNodeText, frame, 0x82182910, movie, n5, Maker(base, entry));
  calls.Call(kSetNodeText, frame, 0x82182924, movie, n6, Id(base, entry, 1646));
  calls.Call(kSetNodeText, frame, 0x82182938, movie, n7, Id(base, entry, 1718));
  calls.Call(kSetNodeNumberText, frame, 0x82182948, movie, a2, Load(base, entry + 744));
  calls.Call(kSetNodeText, frame, 0x82182958, movie, a4, 527);
  calls.Call(kSetNodeText, frame, 0x82182968, movie, a3, 435);
  calls.Call(kSetNodeText, frame, 0x82182978, movie, b3, 2);
  return calls.Call(kSetNodeText, frame, 0x82182988, movie, b4, 3);
}

uint64_t CarDetailOpen(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821830C0 CarDetail::Open");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, Load(base, kCarData) + 12, 1);
  const uint64_t r3 = CarCardFillDetail(base, frame, calls);
  Store(base, Load(base, kCarData) + 36, 2);
  const uint32_t ring = mode + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  Store(base, slot + 0, 36);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
  return r3;
}

}  // namespace front_end_screen_helpers
