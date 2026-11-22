// The user info screen's page, transitions and texts
// (docs/functions/user-info.md): see user_info.h.

#include "user_info/user_info.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/user_info.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <climits>
#include <cmath>

namespace user_info {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint64_t kAll = ~uint64_t(0);  // li r4,-1

using G = schema::UserInfoGlobals;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

uint32_t FindMovie(uint8_t* base, uint32_t key) {
  StoreU32(base, key, kMovie);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

schema::UserInfoData Data(uint8_t* base) { return G::data(base); }

// UiMovie::SetText(movie, -1, node, string, r7, r8, r9) through its
// override, the varargs the original leaves as leftovers 0 (the user's
// call for the parking area, 2026-10-30); f1 passed where given.
uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t node,
                 uint64_t string, uint64_t r7 = 0, uint64_t r8 = 0, uint64_t r9 = 0,
                 const double* f1 = nullptr) {
  const uint64_t args[8] = {movie, kAll, node, string, r7, r8, r9, 0};
  return calls.call(calls.user, kSetText, args, f1, frame, lr);
}

// The play time's text (node 23, string 529): the hours (the data's +44, at
// most 9999), the minutes and seconds (its bytes +48 and +49).
uint64_t PlayTimeText(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr,
                      uint32_t movie) {
  const schema::UserInfoData data = Data(base);
  uint32_t hours = data.play_hours();
  if (hours > 9999) {
    hours = 9999;
  }
  return SetText(calls, frame, lr, movie, 23, 529, hours, data.play_minutes(),
                 data.play_seconds());
}

// fctiwz as the recompiled code does it: NaN 0x80000000, from INT_MAX
// saturated, below INT_MIN 0x80000000.
int32_t TruncateWord(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value < -2147483648.0) {
    return INT32_MIN;
  }
  return int32_t(value);
}

// lfs: a float field as a double.
double Lfs(float value) { return double(value); }
// frsp: rounded to single.
double Single(double value) { return double(float(value)); }

// A transition: the first call `start` and UiMovie::Play(movie, -1, index,
// 0, 0, 0, 0), the word 1; then UiMovie::Run(movie, 0); 1 when it answers 0.
template <typename Start>
uint64_t Transition(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls,
                    uint64_t index, uint32_t play_lr, uint32_t run_lr, Start start) {
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  if (LoadU32(base, started) == 0) {
    start(frame);
    const uint64_t args[8] = {movie, kAll, index, 0, 0, 0, 0, 0};
    calls.CallArgs(kPlay, frame, play_lr, args);
    StoreU32(base, started, 1);
  }
  return uint32_t(calls.Call(kRun, frame, run_lr, movie, 0)) == 0 ? 1 : 0;
}

}  // namespace

uint64_t SetPlayTimeText(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AFC0 UserInfo::SetPlayTimeText");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const schema::PlayTimeAnswer time(
      base,
      uint32_t(calls.Call(kPlayTimeAdd, frame, 0x8220B008, frame + 80, uint64_t(save) + 48)));
  const schema::UserInfoData data = Data(base);
  data.set_play_hours(time.hours());
  // The word's bytes in guest order (the data is a host object since E5
  // step 4; play_minutes and play_seconds read its bytes +48 and +49).
  const uint32_t word = time.time_word();
  uint8_t* const bytes = data.play_time();
  bytes[0] = uint8_t(word >> 24);
  bytes[1] = uint8_t(word >> 16);
  bytes[2] = uint8_t(word >> 8);
  bytes[3] = uint8_t(word);
  return PlayTimeText(base, calls, frame, 0x8220B058, movie);
}

uint64_t MenuPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B0D8 UserInfo::MenuPage");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, 0x8220B0FC, G::menu(base).address);
  SetPlayTimeText(base, frame, calls);
  uint64_t r3 = calls.Call(kMenuPressed, frame, 0x8220B10C, G::menu(base).address, 158);
  if (uint32_t(r3) == 0) {
    r3 = calls.Call(kMenuPressed, frame, 0x8220B120, G::menu(base).address, 164);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const schema::PageObject pages = G::pages(base);
  const uint32_t page = pages.page();
  if (page != 0) {
    pages.set_previous_page(page);
  }
  pages.set_next_page(0);
  pages.set_queued_step(kNone);
  pages.set_page(kNone);
  pages.set_step(1);
  const schema::UserInfoMode m(base, mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  uint8_t* slot = m.messages() + 12 * ((start + count - 1) % 50);  // guest order (E5 step 4)
  schema::host::StoreU32(slot + 0, 6);
  schema::host::StoreU32(slot + 4, 0);
  schema::host::StoreU32(slot + 8, 0);
  return calls.Call(kMenuSoundPlay, frame, 0x8220B1AC, 2);
}

uint64_t SetTexts(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AD08 UserInfo::SetTexts");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = FindMovie(base, frame + 80);
  // The labels' nodes, a table on the frame (+96), strings 284 to 294.
  constexpr uint32_t kLabels[11] = {3, 4, 5, 6, 7, 13, 14, 15, 16, 17, 18};
  for (uint32_t i = 0; i < 11; i++) {
    StoreU32(base, frame + 96 + 4 * i, kLabels[i]);
  }
  for (uint32_t i = 0; i < 11; i++) {
    SetText(calls, frame, 0x8220ADA8, movie, LoadU32(base, frame + 96 + 4 * i), 284 + i);
  }
  SetText(calls, frame, 0x8220ADCC, movie, 25, 441);
  SetText(calls, frame, 0x8220ADE0, movie, 26, 441);
  SetText(calls, frame, 0x8220ADF4, movie, 27, 446);
  SetText(calls, frame, 0x8220AE08, movie, 28, 435);
  SetText(calls, frame, 0x8220AE28, movie, 8, 8, Data(base).count_12());
  SetText(calls, frame, 0x8220AE44, movie, 9, 8, Data(base).count_14());
  SetText(calls, frame, 0x8220AE60, movie, 10, 8, Data(base).count_16());
  // A double in f1 and in r7 through the frame's +48 (stfd, ld).
  auto with_double = [&](uint32_t lr, uint64_t node, uint64_t string, double value) {
    const uint64_t bits = std::bit_cast<uint64_t>(value);
    StoreU32(base, frame + 48, uint32_t(bits >> 32));
    StoreU32(base, frame + 52, uint32_t(bits));
    SetText(calls, frame, lr, movie, node, string, bits, 0, 0, &value);
  };
  with_double(0x8220AE84, 11, 453, Lfs(Data(base).ratio()));
  SetText(calls, frame, 0x8220AEA0, movie, 12, 8, Data(base).stats_5968());
  const double miles = G::miles_per_km(base);
  with_double(0x8220AED4, 19, 452, Single(Lfs(Data(base).distance_5976()) * miles));
  with_double(0x8220AF00, 20, 452, Single(Lfs(Data(base).distance_5904()) * miles));
  {
    // lwa, std at +80, fcfid, frsp, times the double, frsp, fctiwz, stfiwx
    // at +80, lwz.
    const int32_t word = Data(base).distance_5992();
    const uint64_t wide = uint64_t(int64_t(word));
    StoreU32(base, frame + 80, uint32_t(wide >> 32));
    StoreU32(base, frame + 84, uint32_t(wide));
    const double value = Single(Single(double(int64_t(word))) * miles);
    StoreU32(base, frame + 80, uint32_t(TruncateWord(value)));
    SetText(calls, frame, 0x8220AF44, movie, 21, 8, LoadU32(base, frame + 80));
  }
  calls.Call(kSetNumberText, frame, 0x8220AF5C, movie, kAll, 22, Data(base).stats_6024());
  PlayTimeText(base, calls, frame, 0x8220AF90, movie);
  return SetText(calls, frame, 0x8220AFB0, movie, 24, 444, Data(base).garage_count(),
                 Data(base).save_1991C());
}

uint64_t Opening(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B1C8 UserInfo::Opening");
  return Transition(base, started, sp, calls, 0, 0x8220B22C, 0x8220B240,
                    [&](uint32_t frame) { SetTexts(base, frame, calls); });
}

uint64_t Closing(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AC60 UserInfo::Closing");
  const uint64_t done =
      Transition(base, started, sp, calls, 1, 0x8220ACC0, 0x8220ACD4, [](uint32_t) {});
  if (done != 0) {
    Data(base).set_queued_state(9);
  }
  return done;
}

}  // namespace user_info
