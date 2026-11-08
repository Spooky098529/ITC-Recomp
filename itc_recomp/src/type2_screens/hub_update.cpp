// The hub's update and the car it puts on the stand
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "type2_screens/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint64_t kAll = 0xFFFFFFFFFFFFFFFFull;  // li rN,-1

// The hub's transitions, in Update's table (its frame's +96 on).
constexpr uint32_t kHubTransitions[13] = {
    0x821901C0, 0x8218EC78, 0x8218EE48, 0x8218ED98, 0x8218EB78, 0x82190548, 0x8218F098,
    0x821902C8, 0x82190690, 0x8218F150, 0x82190760, 0x8218F220, 0x8218F2B8};
// The debug prints' formats and the menu's item names (0x82636264, 8 words).
constexpr uint32_t kTitleFormat = 0x8200A204;
constexpr uint32_t kItemFormat = 0x8200A040;
constexpr uint32_t kCursorFormat = 0x8200A044;
constexpr uint32_t kItemNames = 0x82636264;

// The stand's holder ([0x826D83E0]'s +0 word) and the garage stage's calls
// (through their overrides).
constexpr uint32_t kStandHolder = 0x826D83E0;
constexpr uint32_t kIsBusy = 0x82179BE0;       // GarageStage::IsBusy
constexpr uint32_t kStageDraw = 0x8217FE60;    // GarageStage::Draw
constexpr uint32_t kBlockOf = 0x8215AAD0;      // CarModels::BlockOf
constexpr uint32_t kReloadModel = 0x8217F880;  // GarageStage::ReloadModel
constexpr uint32_t kStageCommand = 0x82180108; // GarageStage::Command
constexpr uint32_t kSetCar = 0x8217FD58;       // GarageStage::SetCar
constexpr uint32_t kCarInit = 0x820CD5D8;      // Car::Init
constexpr uint32_t kCarRefresh = 0x820C4998;   // Car::Refresh

uint32_t Data(uint8_t* base) { return Load(base, kHubData); }
uint32_t Holder(uint8_t* base) { return Load(base, Load(base, kStandHolder)); }

}  // namespace

uint64_t HubPutCarOnStand(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218DB60 Type2Hub::PutCarOnStand");
  const uint32_t frame = Frame(base, sp, 4000);
  const uint32_t object = frame + 80;
  calls.Call(kCarInit, frame, 0x8218DB7C, object);
  calls.Call(kMemcpy, frame, 0x8218DB8C, object, car, 1884);
  Store(base, frame + 3968, Load(base, frame + 3968) | 1);
  calls.Call(kCarRefresh, frame, 0x8218DBA0, object);
  return calls.Call(kSetCar, frame, 0x8218DBB8, Holder(base), object, kAll);
}

uint64_t HubUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821907F8 Type2Hub::Update");
  const uint32_t frame = Frame(base, sp, 2208);
  // The debug prints (the empty sub_82106540): the title, the menu's item
  // names and the cursor.
  calls.Call(kEmpty, frame, 0x8219081C, 100, 30, 0, kTitleFormat);
  for (uint32_t k = 0; k < 8; k++) {
    calls.CallArgs(kEmpty, frame, 0x8219084C,
                   {150, 50 + 15 * k, 0, kItemFormat, Load(base, kItemNames + 4 * k), 0, 0, 0});
  }
  calls.Call(kEmpty, frame, 0x82190888, 135,
             uint64_t(Load(base, Load(base, kHubMenu) + 0)) * 15 + 50, 0, kCursorFormat);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  Store(base, frame + 144, kHubTransitions[12]);
  for (uint32_t i = 0; i < 12; i++) {
    Store(base, frame + 96 + 4 * i, kHubTransitions[i]);
  }
  // The running transition: its handler from the table on the page
  // object's +24; a nonzero answer (or none below -1) starts the queued one
  // or, with none, the page becomes +4.
  uint32_t pages = Load(base, kHubPages);
  const uint32_t running = Load(base, pages + 12);
  bool finished = false;
  if (running == kNone) {
    if (Load(base, pages + 0) != 3) {
      calls.Call(kRun, frame, 0x821909E8, movie, 0);
    }
  } else if (int32_t(running) >= 0) {
    const uint64_t r3 =
        calls.Call(Load(base, frame + 96 + (running << 2)), frame, 0x82190974, pages + 24);
    pages = Load(base, kHubPages);
    finished = uint32_t(r3) != 0;
  } else {
    finished = true;
  }
  if (finished) {
    const uint32_t queued = Load(base, pages + 16);
    if (queued == kNone) {
      Store(base, pages + 0, Load(base, pages + 4));
      Store(base, pages + 12, kNone);
    } else {
      Store(base, pages + 16, kNone);
      Store(base, pages + 20, 0);
      Store(base, pages + 12, queued);
    }
    Store(base, pages + 24, 0);
  }
  // The stand: the busy marker shown while it's busy; unless the page is
  // 3, the stand drawn once its model is made, else the selected garage
  // car's model remade and, once made, the car put on it and preset 0.
  const uint32_t holder = Holder(base);
  uint64_t r3 = calls.Call(kIsBusy, frame, 0x821909FC, holder);
  if (uint32_t(r3) != 0) {
    if (Load(base, Data(base) + 16) == 0) {
      r3 = HubShowBusy(base, frame, calls);
      Store(base, Data(base) + 16, 1);
    }
  } else if (Load(base, Data(base) + 16) != 0) {
    r3 = HubHideBusy(base, frame, calls);
    Store(base, Data(base) + 16, 0);
  }
  if (Load(base, Load(base, kHubPages) + 0) == 3) {
    return r3;
  }
  if (Load(base, Data(base) + 8) != 0) {
    return calls.Call(kStageDraw, frame, 0x82190A70, holder, 0, 1);
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t car = frame + 160;
  calls.Call(kMemcpy, frame, 0x82190AC8, car, SelectedGarageCar(base, save) + 212, 1968);
  const uint64_t block = calls.Call(kBlockOf, frame, 0x82190AD0, Load(base, car + 12));
  r3 = calls.Call(kReloadModel, frame, 0x82190ADC, holder, block);
  Store(base, Data(base) + 8, uint32_t(r3));
  if (Load(base, Data(base) + 8) == 0) {
    return r3;
  }
  HubPutCarOnStand(base, car, frame, calls);
  return calls.Call(kStageCommand, frame, 0x82190B0C, Holder(base), 14);
}

}  // namespace type2_screens
