// The garage screen's movie 5 helpers (docs/functions/garage-screen.md):
// see garage_screen.h.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "garage_screen/internal.h"
#include "race_screens/race_screens.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace garage_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The maker's logo frame by a car's id: the id below 35 (35 for 2), 35 for
// 43, id - 11 for 47 to 56, 44 for 71, else -1 (compared signed, as the
// original).
uint32_t LogoFrame(uint32_t id) {
  const int32_t s = int32_t(id);
  if (s < 35) {
    return s == 2 ? 35 : id;
  }
  if (s == 43) {
    return 35;
  }
  if (id - 47 <= 9) {
    return id - 11;
  }
  return s == 71 ? 44 : kNone;
}

}  // namespace

uint64_t SetCarRow(uint8_t* base, uint32_t node, uint32_t car, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193180 GarageScreen::SetCarRow");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t pair = FindPair(base, kMovie, frame + 80);
  const uint32_t id = Load(base, car + 12);
  const uint32_t movie = Load(base, pair);
  Store(base, ui_movies::Child(base, node, 3) + 108, LogoFrame(id));
  const uint32_t maker = ui_movies::Child(base, node, 5);
  const uint32_t spec = car_specs::GetCarSpec(base, id);
  calls.Call(kSetNodeText, frame, 0x82193248, movie, maker,
             uint64_t(guest::LoadU8(base, spec + 769)) + 1934);
  calls.Call(kSetNodeText, frame, 0x82193264, movie, ui_movies::Child(base, node, 6),
             uint64_t(id) + 1646);
  return calls.Call(kSetNodeText, frame, 0x82193280, movie, ui_movies::Child(base, node, 7),
                    uint64_t(id) + 1718);
}

uint64_t FillRows(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194CD8 GarageScreen::FillRows");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t pair = FindPair(base, kMovie, frame + 80);
  uint64_t r3 = pair;
  const uint32_t movie = Load(base, pair);
  const int32_t count = int16_t(guest::LoadU16(base, Load(base, Data(base) + 36) + 6328));
  for (int32_t i = 0; i < 4; i++) {
    if (i >= count) {
      continue;
    }
    const uint32_t scroll = Load(base, Load(base, kList) + 4);
    const uint32_t node = ui_movies::FindNode(base, movie, 6, uint32_t(i) + 5);
    const uint32_t record =
        GarageRecord(base, Load(base, Data(base) + 36), scroll + uint32_t(i));
    r3 = SetCarRow(base, node, record + 212, frame, calls);
  }
  return r3;
}

uint32_t SetPrompts(uint8_t* base, const uint32_t args[6], uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821939A0 GarageScreen::SetPrompts");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  // The pairs' first values at +88 (r3, r5, r7), their second at +104 (r4,
  // r6, r8), stored in the original's order.
  Store(base, frame + 92, args[2]);
  Store(base, frame + 104, args[1]);
  Store(base, frame + 108, args[3]);
  Store(base, frame + 88, args[0]);
  Store(base, frame + 96, args[4]);
  Store(base, frame + 112, args[5]);
  uint32_t r3 = 0;
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t prompt = ui_movies::FindNode(base, movie, 16, k);
    const uint32_t first = ui_movies::FindNode(base, movie, 16, Load(base, prompt + 100));
    r3 = ui_movies::FindNode(base, movie, 16, Load(base, prompt + 100) + 1);
    const uint32_t a = Load(base, frame + 88 + 4 * k);
    if (a != kNone) {
      const uint32_t b = Load(base, frame + 104 + 4 * k);
      if (b != kNone) {
        Store(base, prompt + 80, 1);
        Store(base, first + 108, a);
        Store(base, r3 + 108, b);
        continue;
      }
    }
    Store(base, prompt + 80, 0);
  }
  return r3;
}

uint64_t SetHelpLine(uint8_t* base, uint64_t id, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193A90 GarageScreen::SetHelpLine");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  calls.Call(kSetText, frame, 0x82193AD4, movie, 17, 0, id);
  calls.Call(kScrollText, frame, 0x82193AE8, movie, 17, 0, 1);
  return calls.Call(kScrollReset, frame, 0x82193AF8, movie, 17, 0);
}

uint64_t SetUpLists(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821956C0 GarageScreen::SetUpLists");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, kMovie, frame + 96);
  const int32_t count = int16_t(guest::LoadU16(base, Load(base, Data(base) + 36) + 6328));
  const uint64_t n = uint64_t(int64_t(count));
  const uint64_t shown = count < 4 ? n : 4;
  // MenuList::Init over both lists: the accept (159) and back (158) actions
  // at the frame's +84 and +92.
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  calls.CallArgs(kListInit, frame, count < 4 ? 0x8219573C : 0x82195780,
                 {Load(base, kList), n, shown, 1, 154, 153, 155, 156});
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  calls.CallArgs(kListInit, frame, count < 4 ? 0x8219576C : 0x821957B0,
                 {Load(base, kList2), n, shown, 1, 162, 161, 153, 154});
  calls.Call(kListSetSelection, frame, 0x821957C0, Load(base, kList),
             Load(base, Data(base) + 44));
  Store(base, Load(base, kList) + 4, Load(base, Data(base) + 48));
  calls.Call(kListFit, frame, 0x821957DC, Load(base, kList), 1);
  for (int32_t i = 0; i < 4; i++) {
    const uint32_t row = ui_movies::FindNode(base, movie, 6, uint32_t(i) + 5);
    Store(base, row + 80, i < count ? 1 : 0);
  }
  const uint32_t more = ui_movies::FindNode(base, movie, 6, 1);
  if (count > 4) {
    Store(base, more + 80, 1);
    SetScrollArrows(base, Load(base, Load(base, kList) + 4), uint32_t(n), frame);
  } else {
    Store(base, more + 80, 0);
  }
  FillRows(base, frame, calls);
  // The current car's marker: y = (the current car - the scroll) × 84 + 232
  // (a word, its 64-bit sign extension left at the frame's +96).
  const uint32_t current = Load(base, Load(base, Data(base) + 36) + 0x19920);
  const uint32_t marker = ui_movies::FindNode(base, movie, 6, 0);
  const int32_t y = int32_t((current - Load(base, Data(base) + 48)) * 84u + 232u);
  Store(base, frame + 96, y < 0 ? 0xFFFFFFFFu : 0);
  Store(base, frame + 100, uint32_t(y));
  Store(base, marker + 16, std::bit_cast<uint32_t>(float(double(y))));
  static constexpr uint32_t kPrompts[6] = {0, 0, 1, 1, kNone, kNone};
  SetPrompts(base, kPrompts, frame);
  return SetHelpLine(base, 558, frame, calls);
}

}  // namespace garage_screen
