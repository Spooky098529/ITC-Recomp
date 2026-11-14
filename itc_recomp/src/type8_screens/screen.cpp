// The type 8 panel screen's constructor, destructors and car pick
// (docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code.

#include "type8_screens/type8_screens.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "career/career.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "save_data/save_data.h"
#include "screen_constructors/screen_constructors.h"

namespace type8_screens {

namespace {

using guest::LoadU16;
using guest::LoadU32;
using guest::StoreU16;
using guest::StoreU32;

namespace ts = type67_screens;

constexpr uint32_t kGarageCarReset = 0x82110448;
constexpr uint32_t kMemcpy = 0x825A7410;
// The two-player save block's halves and a garage car's size.
constexpr uint32_t kHalfBytes = 120108;
constexpr uint32_t kCarBytes = 1968;
constexpr uint32_t kGarageCarBytes = 1972;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint64_t Construct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822386F0 Type8PanelScreen::Type8PanelScreen");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 224);
  screen_constructors::MachineScreen(base, screen);
  const uint32_t group = screen + kGroup;
  StoreU32(base, screen + 0, kScreenVtable);
  for (uint32_t player = 0; player < 2; player++) {
    Panel8FConstruct(base, screen + kPanelsF[player], player, group, frame);
    Panel8EConstruct(base, screen + kPanelsE[player], player, group, frame);
    Panel8CConstruct(base, screen + kPanelsC[player], player, group, frame);
    Panel8DConstruct(base, screen + kPanelsD[player], player, group, frame);
    ConfirmPanel8Construct(base, screen + kConfirms[player], player, group, frame);
  }
  PanelGroup8Construct(base, group, 4, frame);
  // PanelSetPair's constructor: r4 to r10, then its stack words.
  const uint32_t pair = screen + kCarList;
  StoreU32(base, frame + 108, group);
  StoreU32(base, frame + 100, screen + kConfirms[1]);
  StoreU32(base, frame + 92, screen + kConfirms[0]);
  StoreU32(base, frame + 84, screen + kPanelsC[1]);
  car_list::PairConstruct(base, pair, screen + kPanelsF[0], screen + kPanelsF[1],
                          screen + kPanelsE[0], screen + kPanelsE[1], screen + kPanelsD[0],
                          screen + kPanelsD[1], screen + kPanelsC[0], frame);
  const uint32_t holder = screen + kHolder;
  ts::SingleGroupConstruct(base, holder, group);
  StoreU32(base, screen + 11256, 11);
  StoreU32(base, screen + 11260, 4);
  StoreU32(base, screen + 9644, pair);
  StoreU32(base, screen + 9648, holder);
  StoreU32(base, screen + 9652, 1);
  StoreU32(base, screen + 11264, 376);
  StoreU32(base, screen + 11268, 354);
  StoreU32(base, screen + 11396, 0);
  StoreU32(base, screen + 13368, 1);
  StoreU32(base, pair + 1920, 0);
  StoreU32(base, pair + 3892, 0);
  StoreU32(base, pair + 1924, 0);
  StoreU32(base, pair + 3896, 0);
  StoreU32(base, screen + 13236, 360);
  StoreU32(base, screen + 13240, 359);
  StoreU32(base, screen + 13244, 361);
  StoreU32(base, screen + 15208, 363);
  StoreU32(base, screen + 15212, 362);
  StoreU32(base, screen + 15216, 364);
  for (const uint32_t at : {15228u, 15232u, 15236u, 15240u, 9656u}) {
    StoreU32(base, screen + at, 0);
  }
  StoreU32(base, screen + 15244, 0xFFFFFFFFu);
  StoreU32(base, screen + 15248, 0);
  return r3;
}

uint64_t Destruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238368 Type8PanelScreen::~Type8PanelScreen");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  StoreU32(base, screen + 0, kScreenVtable);
  ts::GroupPairDestruct(base, r3 + kHolder);
  car_list::PairDestruct(base, r3 + kCarList, frame, CarListCalls(calls));
  car_list::GroupBaseDestruct(base, r3 + kGroup);
  StoreU32(base, screen + kConfirms[1], kEmptyBaseVtable);
  archive::NodeDestruct(base, screen + kPanelsD[1]);
  car_list::CarPanelBaseDestruct(base, r3 + kPanelsC[1]);
  // The E panels' destructor is the empty sub_82106540.
  archive::NodeDestruct(base, screen + kPanelsF[1]);
  StoreU32(base, screen + kConfirms[0], kEmptyBaseVtable);
  archive::NodeDestruct(base, screen + kPanelsD[0]);
  car_list::CarPanelBaseDestruct(base, r3 + kPanelsC[0]);
  archive::NodeDestruct(base, screen + kPanelsF[0]);
  return ts::MachineScreenDestruct(base, r3);
}

uint64_t DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822388D8 Type8PanelScreen::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  Destruct(base, r3, frame, calls);
  if (flags & 1) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x82238908);
  }
  return r3;
}

uint64_t PickCar(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238C88 Type8PanelScreen::PickCar");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 160);
  for (uint32_t player = 0; player < 2; player++) {
    // The car list base's words from its +236 (r30 in the original).
    const uint32_t words = screen + 11512 + kGarageCarBytes * player;
    const uint32_t car = kGarageCars + kGarageCarBytes * player;
    const uint32_t step = LoadU32(base, words + 4) + LoadU32(base, words + 0);
    const uint32_t entries = LoadU32(base, words + 1584);
    const uint32_t id = LoadU32(base, LoadU32(base, entries + (step << 2)) + 8432);
    // GarageCar::Reset sets r4 before reading it: only r3 passed.
    const uint64_t reset[6] = {car, 0, 0, 0, 0, 0};
    calls.call_set(kGarageCarReset, reset, 1, frame, 0x82238CD4);
    const uint32_t tab = LoadU32(base, words - 4);
    if (tab < 2) {
      const uint32_t index = LoadU32(base, words + 12) + LoadU32(base, words + 8);
      career::CarSetModelAndClass(base, car, id, frame);
      const uint16_t half = uint16_t(index);
      if (LoadU16(base, car + 4) == 0xFFFF) {
        StoreU16(base, car + 4, half);
      }
      const uint16_t old = LoadU16(base, car + 0);
      StoreU16(base, car + 0, half);
      StoreU32(base, car + kCarBytes, 0);
      StoreU16(base, car + 2, old);
    } else if (tab == 2) {
      // The player's half of the two-player save block: its garage's car
      // `step` links on from the first.
      const uint32_t garage = save_data::TwoPlayerBlock(base) + kHalfBytes * player + 6120;
      uint32_t node = garage + (uint32_t(int32_t(int16_t(LoadU16(base, garage + 204)))) << 2);
      for (uint32_t n = step; n != 0; n--) {
        node = garage + (uint32_t(int32_t(int16_t(LoadU16(base, node)))) << 2);
      }
      const uint32_t slot = uint32_t(int32_t(int16_t(LoadU16(base, node + 2))));
      const uint32_t from = slot * kCarBytes + garage + 212;
      if (!c_runtime::Memcpy(base, car, from, kCarBytes)) {
        const uint64_t args[6] = {car, from, kCarBytes, 0, 0, 0};
        calls.call_set(kMemcpy, args, 3, frame, 0x82238D38);
      }
      StoreU32(base, car + kCarBytes, 1);
    }
  }
  const uint32_t passes = LoadU32(base, kFadeOutPasses);
  mode_tick::ScreenFadesFadeOut(base, passes, 0, 0, 0, 0);
  calls.set_r4(0);
  StoreU32(base, screen + 15240, 0);
  StoreU32(base, screen + 9656, 2);
  return passes;
}

}  // namespace type8_screens
