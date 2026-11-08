// The car dealer's car texts and swatches (docs/functions/car-dealer.md):
// see car_dealer.h.

#include "car_dealer/car_dealer.h"

#include <bit>
#include <climits>
#include <cmath>

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace car_dealer {

namespace {

constexpr uint64_t kAll = 0xFFFFFFFFFFFFFFFFull;  // -1 (the sub-scene of the root)

// fctiwz as the translated code does it: NaN 0x80000000, at or above
// INT_MAX INT_MAX, else truncated (0x80000000 below INT_MIN).
uint32_t Fctiwz(double d) {
  if (std::isnan(d)) {
    return 0x80000000u;
  }
  if (d >= double(INT_MAX)) {
    return uint32_t(INT_MAX);
  }
  if (!(d > -2147483649.0)) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(d));
}

uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint64_t sub_scene, uint64_t child, uint64_t text, uint64_t r7 = 0) {
  const uint64_t args[8] = {movie, sub_scene, child, text, r7, 0, 0, 0};
  return calls.CallArgs(kSetText, frame, lr, args);
}

}  // namespace

uint64_t SetNameTexts(uint8_t* base, uint64_t id, uint64_t maker, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82182388 CarDealer::SetNameTexts");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  SetText(calls, frame, 0x821823D4, movie, kAll, 20, maker + 1934);
  SetText(calls, frame, 0x821823E8, movie, kAll, 21, id + 1646);
  return SetText(calls, frame, 0x821823FC, movie, kAll, 22, id + 1718);
}

uint64_t SetPriceTexts(uint8_t* base, uint64_t price, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82182418 CarDealer::SetPriceTexts");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  calls.Call(kSetNumberText, frame, 0x82182460, movie, 19, 2, price);
  SetText(calls, frame, 0x82182474, movie, 19, 3, 435);
  return SetText(calls, frame, 0x8218248C, movie, 19, 4, 527, price);
}

uint64_t SetSpecTexts(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821824A8 CarDealer::SetSpecTexts");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  SetText(calls, frame, 0x821824F4, movie, 18, 5, uint64_t(Load(base, record + 0)) + 421);
  SetText(calls, frame, 0x8218250C, movie, 18, 6, uint64_t(Load(base, record + 4)) + 426);
  SetText(calls, frame, 0x82182524, movie, 18, 7, 8, Load(base, record + 12));
  {
    // The weight's integer through a doubleword at +80, converted, scaled by
    // the double at 0x82009F00 in single precision, truncated into +80.
    const int64_t wide = int64_t(int32_t(Load(base, record + 8)));
    Store(base, frame + 80, uint32_t(uint64_t(wide) >> 32));
    Store(base, frame + 84, uint32_t(uint64_t(wide)));
    const double f13 = double(float(double(wide)));
    const double scale = std::bit_cast<double>(
        (uint64_t(Load(base, 0x82009F00)) << 32) | Load(base, 0x82009F04));
    const double f0 = double(float(f13 * scale));
    Store(base, frame + 80, Fctiwz(f0));
  }
  SetText(calls, frame, 0x8218256C, movie, 18, 8, 8, Load(base, frame + 80));
  SetText(calls, frame, 0x82182584, movie, 18, 11, 8, Load(base, record + 16));
  {
    const double f13 = double(std::bit_cast<float>(Load(base, record + 20)));
    const double scale = std::bit_cast<double>(
        (uint64_t(Load(base, 0x82009F08)) << 32) | Load(base, 0x82009F0C));
    const double f1 = double(float(f13 * scale));
    const uint64_t bits = std::bit_cast<uint64_t>(f1);
    Store(base, frame + 48, uint32_t(bits >> 32));
    Store(base, frame + 52, uint32_t(bits));
    const uint64_t args[8] = {movie, 18, 12, 436, bits, 0, 0, 0};
    calls.call(calls.user, kSetText, args, &f1, frame, 0x821825B4);
  }
  SetText(calls, frame, 0x821825C8, movie, 18, 9, 438);
  SetText(calls, frame, 0x821825DC, movie, 18, 10, 440);
  SetText(calls, frame, 0x821825F0, movie, 18, 13, 437);
  return SetText(calls, frame, 0x82182604, movie, 18, 14, 439);
}

uint64_t ShowCarInfo(uint8_t* base, uint64_t index, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82182620 CarDealer::ShowCarInfo");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t list = Load(base, Load(base, kData) + 1136);
  const uint32_t entry = Load(base, (uint32_t(index) << 2) + list);
  Store(base, frame + 80, guest::LoadU8(base, entry + 770));
  Store(base, frame + 88, Load(base, entry + 712));
  Store(base, frame + 84, guest::LoadU8(base, entry + 709) != 0 ? 1 : 0);
  Store(base, frame + 92, Load(base, entry + 648));
  Store(base, frame + 96, Fctiwz(double(std::bit_cast<float>(Load(base, entry + 652)))));
  // lfs then frsp: the translated code leaves the word as it was (a
  // signaling NaN too).
  Store(base, frame + 100, Load(base, entry + 660));
  SetSpecTexts(base, frame + 80, frame, calls);
  SetNameTexts(base, Load(base, entry + 8432), guest::LoadU8(base, entry + 769), frame, calls);
  return SetPriceTexts(base, Load(base, entry + 744), frame, calls);
}

uint64_t SetSwatchColor(uint8_t* base, uint32_t node, uint32_t record, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82181270 CarDealer::SetSwatchColor");
  Frame(base, sp, 96);
  const uint32_t child = ui_movies::Child(base, node, 2);
  Store(base, child + 56, Load(base, record + 0));
  Store(base, child + 60, Load(base, record + 4));
  Store(base, child + 64, Load(base, record + 8));
  return child;
}

uint64_t ShowSwatches(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821826C0 CarDealer::ShowSwatches");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  const uint32_t row = ui_movies::FindNode(base, movie, 26, 1);
  uint64_t r3 = row;
  const int32_t count = int32_t(Load(base, Load(base, kData) + 1192));
  const uint32_t scroll = Load(base, Load(base, kListMenu) + 4);
  const int32_t shown = count < 9 ? count : 9;
  uint32_t at = (scroll + 285) << 2;
  for (int32_t i = 0; i < shown; i++) {
    const uint32_t node = ui_movies::FindNode(base, movie, 26, Load(base, row + 100) + uint32_t(i));
    const uint32_t record = Load(base, at + Load(base, kData));
    r3 = SetSwatchColor(base, node, record, frame);
    at += 4;
  }
  return r3;
}

uint64_t FillStatus(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82183AC8 CarDealer::FillStatus");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  calls.Call(kResetPose, frame, 0x82183B00, movie);
  uint32_t data = Load(base, kData);
  if (Load(base, data + 4) == 0) {
    calls.Call(kSetChildNameText, frame, 0x82183B28, movie, 15, 6, data + 64);
    data = Load(base, kData);
  }
  calls.Call(kSetNumberText, frame, 0x82183B40, movie, 15, 9, Load(base, data + 84));
  SetText(calls, frame, 0x82183B5C, movie, 15, 13, 8, Load(base, Load(base, kData) + 92));
  const uint32_t day = ui_movies::FindNode(base, movie, 15, 11);
  Store(base, day + 108, Load(base, Load(base, kData) + 88));
  const uint32_t prompts[6] = {0, 0, 1, 1, 0xFFFFFFFFu, 0xFFFFFFFFu};
  const uint64_t r3 = SetPrompts(base, prompts, frame);
  Store(base, Load(base, kData) + 8, 0);
  Store(base, Load(base, kData) + 28, 0);
  Store(base, Load(base, kData) + 0, 1);
  Store(base, Load(base, kData) + 32, 0);
  return r3;
}

}  // namespace car_dealer
