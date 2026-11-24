// The car cards (docs/functions/front-end-screen-helpers.md): see
// front_end_screen_helpers.h.

#include "front_end_screen_helpers/front_end_screen_helpers.h"

#include "base/log.h"
#include "guest.h"
#include "schema/car_dealer.h"
#include "schema/race_screens.h"
#include "schema/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace front_end_screen_helpers {

namespace {

using Dealer = schema::CarDealerGlobals;
using Entry = schema::CarDealerEntry;
using Mode = schema::CarDealerMode;
using Node = schema::UiMovieNode;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// UiMovies::Find(id): the movie. The originals stored the id at a frame
// word only they read and passed its address; passed as a value (E5 step 6).
uint32_t FindMovie(uint8_t* base, uint32_t id) {
  return Load(base, ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id));
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

// The entry's maker byte and id as the originals add them (64-bit sums of
// the zero-extended loads).
uint64_t Maker(uint8_t* base, uint32_t entry) {
  return uint64_t(Entry(base, entry).maker()) + 1934;
}
uint64_t Id(uint8_t* base, uint32_t entry, uint32_t first) {
  return uint64_t(Entry(base, entry).car_id()) + first;
}

}  // namespace

uint64_t CarCardFill(uint8_t* base, uint32_t node, uint32_t entry, uint32_t sp,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821821C8 CarCard::Fill");
  // The frame kept for the text setters' calls (its back chain).
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, 4);
  const uint32_t logo = ui_movies::Child(base, node, 3);
  Node(base, logo).set_texture_entry(Logo(int32_t(Entry(base, entry).car_id())));
  calls.Call(kSetNodeText, frame, 0x82182278, movie, ui_movies::Child(base, node, 5),
             Maker(base, entry));
  calls.Call(kSetNodeText, frame, 0x82182298, movie, ui_movies::Child(base, node, 6),
             Id(base, entry, 1646));
  calls.Call(kSetNodeText, frame, 0x821822B8, movie, ui_movies::Child(base, node, 7),
             Id(base, entry, 1718));
  calls.Call(kSetNodeNumberText, frame, 0x821822D4, movie, ui_movies::Child(base, node, 8),
             Entry(base, entry).price());
  return calls.Call(kSetNodeText, frame, 0x821822F0, movie, ui_movies::Child(base, node, 9),
                    435);
}

uint64_t CarCardsFillAll(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821822F8 CarCards::FillAll");
  // The original's 144-byte frame left out: its one word (the movie's key at
  // +80) passed as a value, and no guest call is made from it (CarCard::Fill
  // makes its own frame below `sp`; E5 step 6).
  const uint32_t frame = sp;
  const uint32_t pair = ui_movies::PairsFindKey(base, ui_movies::kMovieTable, 4);
  const uint32_t movie = Load(base, pair);
  uint64_t r3 = pair;
  const int32_t count = int32_t(Dealer::data(base).shown_list().count());
  for (int32_t i = 0; i < 4; i++) {
    if (i >= count) {
      continue;
    }
    const uint32_t scroll = Dealer::list_menu(base).scroll();
    const uint32_t node = ui_movies::FindNode(base, movie, 7, uint32_t(i) + 5);
    const schema::CarDealerList entries = Dealer::data(base).shown_list();
    const uint32_t entry = uint32_t(entries.load_at((scroll + uint32_t(i)) << 2, 4));
    r3 = CarCardFill(base, node, entry, frame, calls);
  }
  return r3;
}

uint64_t CarCardFillDetail(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821827E0 CarCard::FillDetail");
  // The frame kept for the text setters' calls (its back chain).
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = FindMovie(base, 18);
  const schema::CarDealerData data = Dealer::data(base);
  const uint32_t scroll = data.car_scroll();
  const uint32_t index = (scroll + data.car_selection()) << 2;
  const uint32_t entry = uint32_t(data.shown_list().load_at(index, 4));
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
  calls.Call(kSetNodeNumberText, frame, 0x82182948, movie, a2, Entry(base, entry).price());
  calls.Call(kSetNodeText, frame, 0x82182958, movie, a4, 527);
  calls.Call(kSetNodeText, frame, 0x82182968, movie, a3, 435);
  calls.Call(kSetNodeText, frame, 0x82182978, movie, b3, 2);
  return calls.Call(kSetNodeText, frame, 0x82182988, movie, b4, 3);
}

uint64_t CarDetailOpen(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821830C0 CarDetail::Open");
  // The original's 112-byte frame (only its back chain) left out: no guest
  // call is made from it (CarCard::FillDetail makes its own frame below
  // `sp`; E5 step 6).
  Dealer::data(base).set_detail_open(1);
  const uint64_t r3 = CarCardFillDetail(base, sp, calls);
  Dealer::data(base).set_buy_step(2);
  const Mode m(base, mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  // The ring's bytes, in guest order in either shape (a dual view, E5 step 4).
  uint8_t* slot = m.messages() + 12 * ((start + count - 1) % 50);
  schema::host::StoreU32(slot + 0, 36);
  schema::host::StoreU32(slot + 4, 0);
  schema::host::StoreU32(slot + 8, 0);
  return r3;
}

}  // namespace front_end_screen_helpers
