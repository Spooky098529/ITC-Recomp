// The user info screen's page, transitions and texts
// (docs/functions/user-info.md): see user_info.h.

#include "user_info/user_info.h"

#include "base/log.h"
#include "guest.h"
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
// The double 0.62138 (kilometers to miles) the texts multiply by.
constexpr uint32_t kMiles = 0x8200A510;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

uint32_t FindMovie(uint8_t* base, uint32_t key) {
  StoreU32(base, key, kMovie);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

uint32_t Data(uint8_t* base) { return LoadU32(base, kData); }

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
  const uint32_t data = Data(base);
  uint32_t hours = LoadU32(base, data + 44);
  if (hours > 9999) {
    hours = 9999;
  }
  return SetText(calls, frame, lr, movie, 23, 529, hours, guest::LoadU8(base, data + 48),
                 guest::LoadU8(base, data + 49));
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
double Lfs(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(LoadU32(base, address)));
}
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
  const uint32_t time =
      uint32_t(calls.Call(kPlayTimeAdd, frame, 0x8220B008, frame + 80, uint64_t(save) + 48));
  const uint32_t data = Data(base);
  StoreU32(base, data + 44, LoadU32(base, time + 0));
  StoreU32(base, data + 48, LoadU32(base, time + 4));
  return PlayTimeText(base, calls, frame, 0x8220B058, movie);
}

uint64_t MenuPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B0D8 UserInfo::MenuPage");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuStep, frame, 0x8220B0FC, LoadU32(base, kMenu));
  SetPlayTimeText(base, frame, calls);
  uint64_t r3 = calls.Call(kMenuPressed, frame, 0x8220B10C, LoadU32(base, kMenu), 158);
  if (uint32_t(r3) == 0) {
    r3 = calls.Call(kMenuPressed, frame, 0x8220B120, LoadU32(base, kMenu), 164);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t page = LoadU32(base, pages + 0);
  if (page != 0) {
    StoreU32(base, pages + 8, page);
  }
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 16, kNone);
  StoreU32(base, pages + 0, kNone);
  StoreU32(base, pages + 12, 1);
  const uint32_t ring = mode + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t start = LoadU32(base, ring + 604);
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  StoreU32(base, slot + 0, 6);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
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
  SetText(calls, frame, 0x8220AE28, movie, 8, 8, LoadU32(base, Data(base) + 8));
  SetText(calls, frame, 0x8220AE44, movie, 9, 8, LoadU32(base, Data(base) + 12));
  SetText(calls, frame, 0x8220AE60, movie, 10, 8, LoadU32(base, Data(base) + 16));
  // A double in f1 and in r7 through the frame's +48 (stfd, ld).
  auto with_double = [&](uint32_t lr, uint64_t node, uint64_t string, double value) {
    const uint64_t bits = std::bit_cast<uint64_t>(value);
    StoreU32(base, frame + 48, uint32_t(bits >> 32));
    StoreU32(base, frame + 52, uint32_t(bits));
    SetText(calls, frame, lr, movie, node, string, bits, 0, 0, &value);
  };
  with_double(0x8220AE84, 11, 453, Lfs(base, Data(base) + 20));
  SetText(calls, frame, 0x8220AEA0, movie, 12, 8, LoadU32(base, Data(base) + 24));
  const double miles = std::bit_cast<double>((uint64_t(LoadU32(base, kMiles)) << 32) |
                                             LoadU32(base, kMiles + 4));
  with_double(0x8220AED4, 19, 452, Single(Lfs(base, Data(base) + 28) * miles));
  with_double(0x8220AF00, 20, 452, Single(Lfs(base, Data(base) + 32) * miles));
  {
    // lwa, std at +80, fcfid, frsp, times the double, frsp, fctiwz, stfiwx
    // at +80, lwz.
    const int32_t word = int32_t(LoadU32(base, Data(base) + 36));
    const uint64_t wide = uint64_t(int64_t(word));
    StoreU32(base, frame + 80, uint32_t(wide >> 32));
    StoreU32(base, frame + 84, uint32_t(wide));
    const double value = Single(Single(double(int64_t(word))) * miles);
    StoreU32(base, frame + 80, uint32_t(TruncateWord(value)));
    SetText(calls, frame, 0x8220AF44, movie, 21, 8, LoadU32(base, frame + 80));
  }
  calls.Call(kSetNumberText, frame, 0x8220AF5C, movie, kAll, 22, LoadU32(base, Data(base) + 40));
  PlayTimeText(base, calls, frame, 0x8220AF90, movie);
  return SetText(calls, frame, 0x8220AFB0, movie, 24, 444, LoadU32(base, Data(base) + 52),
                 LoadU32(base, Data(base) + 56));
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
    StoreU32(base, Data(base) + 4, 9);
  }
  return done;
}

}  // namespace user_info
