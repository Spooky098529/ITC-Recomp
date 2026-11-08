// The car dealer's menu openers (docs/functions/car-dealer.md): see
// car_dealer.h.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"
#include "ui_movies/game.h"

#include <rex/platform/fpscr.h>

#include <climits>
#include <cmath>

namespace car_dealer {

namespace {

constexpr uint32_t kAll = 0xFFFFFFFFu;  // -1 (the sub-scene of the root)

// MenuList::Init over `list` with the menu actions the dealer passes: r7 to
// r10 given, 159 and 158 stored at the frame's +84 and +92 first.
void InitList(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr, uint32_t list,
              uint64_t count, uint64_t shown, uint32_t r7, uint32_t r8, uint32_t r9,
              uint32_t r10) {
  Store(base, frame + 92, 158);
  Store(base, frame + 84, 159);
  const uint64_t args[8] = {list, count, shown, 1, r7, r8, r9, r10};
  calls.CallArgs(kListInit, frame, lr, args);
}

// fctiwz as the translated code does it: NaN and below INT_MIN 0x80000000,
// INT_MAX and above INT_MAX, else truncated; the low word.
uint32_t Fctiwz(double value) {
  if (std::isnan(value)) return 0x80000000u;
  if (value >= double(INT32_MAX)) return uint32_t(INT32_MAX);
  if (value < double(INT32_MIN)) return 0x80000000u;
  return uint32_t(int32_t(value));
}

// A 64-bit integer stored big-endian at `at` (the original's std before
// each conversion).
void StoreWide(uint8_t* base, uint32_t at, int64_t value) {
  Store(base, at, uint32_t(uint64_t(value) >> 32));
  Store(base, at + 4, uint32_t(uint64_t(value)));
}

uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint64_t sub_scene, uint64_t child, uint64_t text) {
  return calls.Call(kSetText, frame, lr, movie, sub_scene, child, text);
}

}  // namespace

uint64_t OpenListMenu(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184338 CarDealer::OpenListMenu");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = FindMovie(base, 4, frame + 96);
  InitList(base, calls, frame, 0x8218439C, Load(base, kListMenu), 2, 2, 154, 153, 155, 156);
  calls.Call(kListSetSelection, frame, 0x821843B0, Load(base, kListMenu),
             Load(base, Load(base, kData) + 40));
  SetKindBar(base, uint64_t(Load(base, Load(base, kData) + 40)) * 40, frame);
  SetText(calls, frame, 0x821843DC, movie, 8, 5, 124);
  SetText(calls, frame, 0x821843DC, movie, 8, 6, 125);
  const uint32_t second = ui_movies::FindNode(base, movie, 11, 1);
  const uint32_t first = ui_movies::FindNode(base, movie, 11, 0);
  const uint32_t picked = uint32_t(calls.Call(kListSelection, frame, 0x82184418, Load(base, kListMenu)));
  if (picked <= 1) {
    Store(base, second + 108, picked);
    Store(base, first + 108, picked);
  }
  const uint64_t selection = calls.Call(kListSelection, frame, 0x82184440, Load(base, kListMenu));
  uint64_t text = selection + 550;
  if (uint32_t(selection) <= 1) {
    const uint32_t count = Load(base, Load(base, kData) + (uint32_t(selection) == 0 ? 616 : 1132));
    if (count == 0) {
      text = 583;  // the list empty
    }
  }
  return HelpLineSet(base, text, frame, calls);
}

uint64_t OpenCarList(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184488 CarDealer::OpenCarList");
  const uint32_t frame = Frame(base, sp, 176);
  const uint32_t movie = FindMovie(base, 4, frame + 96);
  const uint32_t n = Load(base, Load(base, Load(base, kData) + 1136) + 512);
  const bool few = int32_t(n) < 4;
  const uint64_t shown = few ? n : 4;
  InitList(base, calls, frame, few ? 0x82184504 : 0x82184548, Load(base, kListMenu), n, shown,
           154, 153, 155, 156);
  InitList(base, calls, frame, few ? 0x82184534 : 0x82184578, Load(base, kColorMenu), n, shown,
           162, 161, 153, 154);
  calls.Call(kListSetSelection, frame, 0x82184588, Load(base, kListMenu),
             Load(base, Load(base, kData) + 48));
  Store(base, Load(base, kListMenu) + 4, Load(base, Load(base, kData) + 52));
  calls.Call(kListFit, frame, 0x821845A4, Load(base, kListMenu), 1);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t card = ui_movies::FindNode(base, movie, 7, i + 5);
    Store(base, card + 80, int32_t(i) < int32_t(n) ? 1 : 0);
  }
  const uint32_t more = ui_movies::FindNode(base, movie, 7, 1);
  if (int32_t(n) > 4) {
    Store(base, more + 80, 1);
    SetScrollArrows(base, Load(base, Load(base, kListMenu) + 4), n, frame);
  } else {
    Store(base, more + 80, 0);
  }
  calls.Call(kCarCardsFillAll, frame, 0x82184618);
  const uint64_t selection = calls.Call(kListSelection, frame, 0x82184620, Load(base, kListMenu));
  const uint32_t index = (Load(base, Load(base, kListMenu) + 4) + uint32_t(selection)) << 2;
  const uint32_t entry = Load(base, index + Load(base, Load(base, kData) + 1136));
  return HelpLineSet(base, uint64_t(Load(base, entry + 8432)) + 1862, frame, calls);
}

uint64_t OpenCar(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82184650 CarDealer::OpenCar");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, 4, frame + 96);
  InitList(base, calls, frame, 0x821846B4, Load(base, kListMenu), 2, 2, 154, 153, 155, 156);
  calls.Call(kListSetSelection, frame, 0x821846CC, Load(base, kColorMenu),
             Load(base, Load(base, kData) + 48));
  Store(base, Load(base, kColorMenu) + 4, Load(base, Load(base, kData) + 52));
  calls.Call(kListFit, frame, 0x821846E8, Load(base, kColorMenu), 1);
  const uint64_t selection = calls.Call(kListSelection, frame, 0x821846F0, Load(base, kListMenu));
  SetKindBar(base, selection * 40, frame);
  SetText(calls, frame, 0x8218470C, movie, 8, 5, 127);
  SetText(calls, frame, 0x82184720, movie, 8, 6, 112);
  {
    const uint32_t data = Load(base, kData);
    ShowCarInfo(base, uint64_t(Load(base, data + 52)) + Load(base, data + 48), frame, calls);
  }
  const uint32_t shown = Load(base, Load(base, Load(base, kData) + 1136) + 512) == 1 ? 0 : 1;
  Store(base, ui_movies::FindNode(base, movie, kAll, 23) + 80, shown);
  Store(base, ui_movies::FindNode(base, movie, kAll, 24) + 80, shown);
  return calls.Call(kStageSetPlates, frame, 0x82184798, Load(base, Load(base, kStandHolder)), 0);
}

uint64_t OpenColors(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821847A0 CarDealer::OpenColors");
  using Fpscr = rex::platform::FPSCRPlatform;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  const uint32_t frame = Frame(base, sp, 240);
  const uint32_t movie = FindMovie(base, 4, frame + 96);
  uint32_t entry;
  {
    const uint32_t data = Load(base, kData);
    const uint32_t index = (Load(base, data + 52) + Load(base, data + 48)) << 2;
    entry = Load(base, index + Load(base, data + 1136));
  }
  const uint32_t n = Load(base, entry + 8376);
  const bool over = int32_t(n) > 9;
  InitList(base, calls, frame, over ? 0x82184838 : 0x82184858, Load(base, kListMenu), n,
           over ? 9 : n, 154, 153, 155, 156);
  calls.Call(kListSetSelection, frame, 0x82184868, Load(base, kListMenu),
             Load(base, Load(base, kData) + 56));
  Store(base, Load(base, kListMenu) + 4, Load(base, Load(base, kData) + 60));
  calls.Call(kListFit, frame, 0x82184884, Load(base, kListMenu), 1);
  const uint64_t selection = calls.Call(kListSelection, frame, 0x8218488C, Load(base, kListMenu));
  SetColorBar(base, uint64_t(uint32_t(selection) << 5) + 3, frame);
  Store(base, ui_movies::FindNode(base, movie, kAll, 25) + 80, over ? 1 : 0);
  // The old color records freed, from the last.
  while (Load(base, Load(base, kData) + 1192) != 0) {
    const uint32_t colors = Load(base, kData) + 1140;
    const uint32_t last = Load(base, colors + 52) - 1;
    Store(base, colors + 52, last);
    calls.Call(kVirtualFree, frame, 0x821848F0, Load(base, (last << 2) + colors), 0, 0x8000);
  }
  if (int32_t(n) > 0) {
    // Each color (20-byte records from +8116: +0 the kind, +4 the hue, +6
    // the saturation, +7 the value) into a 32-byte record: +16 to +24 the
    // HSV floats, +0 to +8 the RGB integers.
    const double scale = double(guest::LoadFloat(base, 0x82000C44));
    const double unit = double(guest::LoadFloat(base, 0x82002200));
    uint32_t source = entry + 8116;
    for (uint32_t left = n; left != 0; left--, source += 20) {
      const uint32_t record = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8218493C, 0, 32, 4096, 4));
      const int64_t hue = int64_t(int32_t(guest::LoadU16(base, source + 4)));
      StoreWide(base, frame + 96, hue);
      guest::StoreFloat(base, record + 16, float(double(float(double(hue)))));
      const int64_t saturation = guest::LoadU8(base, source + 6);
      StoreWide(base, frame + 104, saturation);
      const double s = double(float(double(saturation)));
      guest::StoreFloat(base, record + 20, float(double(float(s * unit))));
      const uint32_t kind = guest::LoadU16(base, source + 0);
      const uint32_t add = Load(base, kind == 1 ? 0x82002FF4 : 0x82002FF8);
      int32_t value = int32_t(guest::LoadU8(base, source + 7) + add);
      if (value < 0) {
        value = 0;
      } else if (value > 255) {
        value = 255;
      }
      StoreWide(base, kind == 1 ? frame + 112 : frame + 120, value);
      const double v = double(float(double(int64_t(value))));
      guest::StoreFloat(base, record + 24, float(double(float(v * unit))));
      calls.Call(kHsvToRgb, frame, 0x82184A00, frame + 128, record + 16);
      const double red = double(float(double(guest::LoadFloat(base, frame + 128)) * scale));
      const double green = double(float(double(guest::LoadFloat(base, frame + 132)) * scale));
      const double blue = double(float(double(guest::LoadFloat(base, frame + 136)) * scale));
      Store(base, record + 0, Fctiwz(red));
      Store(base, record + 4, Fctiwz(green));
      Store(base, record + 8, Fctiwz(blue));
      const uint32_t colors = Load(base, kData) + 1140;
      const uint32_t count = Load(base, colors + 52);
      Store(base, colors + 52, count + 1);
      Store(base, (count << 2) + colors, record);
    }
  }
  // Sub-scene 26's swatches shown for those below n.
  const uint32_t swatches = ui_movies::FindNode(base, movie, 26, 1);
  for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, swatches + 104)); i++) {
    const uint32_t swatch = ui_movies::FindNode(base, movie, 26, i + Load(base, swatches + 100));
    Store(base, swatch + 80, int32_t(i) < int32_t(n) ? 1 : 0);
  }
  ShowSwatches(base, frame);
  {
    const uint32_t data = Load(base, kData);
    calls.Call(kShowCarById, frame, 0x82184AD4,
               uint64_t(Load(base, data + 60)) + Load(base, data + 56));
  }
  return HelpLineSet(base, 552, frame, calls);
}

}  // namespace car_dealer
