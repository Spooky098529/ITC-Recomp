// PanelSet's update (docs/functions/car-list.md): see car_list.h.

#include "car_list/car_list.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_makers/car_makers.h"
#include "car_specs/car_specs.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "type67_modes/type67_modes.h"
#include "type67_screens/type67_screens.h"

#include <rex/platform/fpscr.h>

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The stand's holder in slot `slot` ([0x826D83E0] read each time, as the
// originals do).
uint32_t Holder(uint8_t* base, uint32_t slot) {
  return LoadU32(base, LoadU32(base, kStandHolders) + 4 * slot);
}

// GarageStage::SetDrawWord's plain C++ (a leaf of its own: no guest calls).
uint64_t SetDrawWord(uint8_t* base, uint32_t holder, uint32_t word, uint32_t frame) {
  return front_end_car_draw::GarageStageSetDrawWord(base, holder, word, frame);
}

// Car::Refresh on the frame's car: its plain C++ (flush-to-zero off, as it
// expects), else its original.
void Refresh(uint8_t* base, uint32_t car, uint32_t frame, uint32_t lr, const Calls& calls) {
  using Fpscr = rex::platform::FPSCRPlatform;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, car, &refreshed)) {
    calls.Call(kCarRefresh, frame, lr, car);
  }
}

// memcpy(to, from, 1884): its plain C++, else the original.
void CopyCar(uint8_t* base, uint32_t to, uint32_t from, uint32_t frame, uint32_t lr,
             const Calls& calls) {
  if (!c_runtime::Memcpy(base, to, from, 1884)) {
    calls.Call(kMemcpy, frame, lr, to, from, 1884);
  }
}

}  // namespace

uint64_t DrawAll(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821807D8 GarageStages::DrawAll");
  const uint32_t frame = Frame(base, sp, 160);
  for (uint32_t i = 0; i < 3; i++) {
    StoreU32(base, frame + 80 + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 3; i++) {
    StoreU32(base, frame + 96 + 4 * i, 1);
  }
  // The draw words kept, each stage's set to 0.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t holder = Holder(base, i);
    const uint32_t stage = LoadU32(base, holder);
    if (LoadU32(base, stage + 16) == 0) {
      StoreU32(base, frame + 80 + 4 * i, 0);
    } else {
      StoreU32(base, frame + 80 + 4 * i, LoadU32(base, stage + 6056));
      SetDrawWord(base, holder, 0, frame);
    }
  }
  // Each stage with a kept word drawn alone.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t holder = Holder(base, i);
    if (LoadU32(base, LoadU32(base, holder) + 16) == 0 ||
        LoadU32(base, frame + 80 + 4 * i) == 0) {
      continue;
    }
    SetDrawWord(base, holder, 1, frame);
    calls.Call(kGarageStageDraw, frame, 0x8218089C, holder, 1,
               LoadU32(base, frame + 96 + 4 * i));
    SetDrawWord(base, holder, 0, frame);
  }
  // The words put back.
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t holder = Holder(base, i);
    r3 = holder;
    if (LoadU32(base, LoadU32(base, holder) + 16) != 0) {
      r3 = SetDrawWord(base, holder, LoadU32(base, frame + 80 + 4 * i), frame);
    }
  }
  return r3;
}

uint64_t UpdateStand(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224C008 PanelSetBase::UpdateStand");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 9952);
  uint64_t r3 = 0;
  // The entry's row (+240 + +236), its car id, and the stage's model
  // reloaded for it (CarModels::BlockOf's plain C++, GarageStage::
  // ReloadModel through its override; +24 its answer).
  uint64_t row = 0;
  uint32_t id = 0;
  const auto reload = [&](uint32_t holder, uint32_t lr) {
    row = uint64_t(LoadU32(base, b + 240)) + uint64_t(LoadU32(base, b + 236));
    const uint32_t entry = LoadU32(base, LoadU32(base, b + 1820) + (uint32_t(row) << 2));
    id = LoadU32(base, entry + 8432);
    const uint64_t answer = calls.Call(kReloadModel, frame, lr, holder, car_makers::BlockOf(id));
    StoreU32(base, b + 24, uint32_t(answer));
    return answer;
  };
  const bool two = LoadU32(base, kTwoPlayers) != 0;
  const bool second = two && LoadU32(base, b + 1952) != 0;
  const uint32_t holder = Holder(base, two ? (second ? 2 : 1) : 0);
  if (!two) {
    // One player: stage 0.
    r3 = SetDrawWord(base, holder, 0, frame);
    if (LoadU32(base, b + 20) != 0) {
      if (LoadU32(base, b + 24) != 0) {
        r3 = SetDrawWord(base, holder, 1, frame);
      } else {
        r3 = reload(holder, 0x8224C358);
        if (int32_t(uint32_t(r3)) == 1) {
          const uint64_t pick =
              uint64_t(LoadU32(base, b + 248)) + uint64_t(LoadU32(base, b + 244));
          const uint32_t car = frame + 3984;
          race_setup::CarInit(base, car);
          uint32_t plates = 0;
          if (LoadU32(base, b + 232) == 2) {
            // The garage tab's car (sub_82192770, never seen in play: the
            // original), copied in.
            const uint64_t save = SaveData(base, b, frame);
            const uint64_t from =
                calls.Call(kGarageTabCar, frame, 0x8224C3A0, frame + 7888, save + 6120, row);
            CopyCar(base, car, uint32_t(from), frame, 0x8224C3B0, calls);
            plates = 1;
          } else {
            race_setup::SetModel(base, car, id);
          }
          type67_modes::GarageStageSetPlates(base, holder, plates);
          StoreU32(base, frame + 7872, LoadU32(base, frame + 7872) | 1);
          Refresh(base, car, frame, 0x8224C3E4, calls);
          r3 = calls.Call(kSetCar, frame, 0x8224C404, holder, car,
                          LoadU32(base, b + 232) == 0 ? pick : 0xFFFFFFFFFFFFFFFFull);
        }
      }
    }
  } else {
    // Two players (never seen in play): stage 1 or 2 by the player.
    const uint32_t flag = second ? 128 : 1;
    const uint32_t command = second ? 57 : 56;
    r3 = SetDrawWord(base, holder, 0, frame);
    if (LoadU32(base, b + 20) != 0) {
      if (LoadU32(base, b + 24) != 0) {
        if (LoadU32(base, kStandWait) != 0) {
          StoreU32(base, LoadU32(base, holder) + 6052, 0);
          calls.Call(kGarageStageCommand, frame, 0x8224C210, holder, command);
          r3 = SetDrawWord(base, holder, 1, frame);
        }
      } else {
        r3 = reload(holder, 0x8224C0C4);
        if (int32_t(uint32_t(r3)) == 1) {
          uint64_t pick = 0;
          if (LoadU32(base, b + 32) == 3) {
            const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224C0E8, b_r3 + 60);
            pick = uint64_t(LoadU32(base, b + 64)) + item;
          }
          const uint32_t car = frame + 80;
          race_setup::CarInit(base, car);
          uint32_t plates = 0;
          if (LoadU32(base, b + 232) != 2) {
            race_setup::SetModel(base, car, id);
          } else {
            // The player's garage (sub_82105678's save data pointer: ported,
            // as SaveData): the row's car by the halfword links from +204.
            const uint32_t save = LoadU32(base, kPairSaveData);
            const int64_t offset = int64_t(int32_t(LoadU32(base, b + 1952))) * int64_t(120108);
            const uint64_t garage = uint64_t(offset) + uint64_t(save) + 6120;
            const auto link = [&](uint32_t at) {
              const int32_t next = int16_t(guest::LoadU16(base, at));
              return uint64_t(uint32_t(next) << 2) + garage;
            };
            uint64_t node = link(uint32_t(garage) + 204);
            for (uint32_t n = uint32_t(row); n != 0; n--) {
              node = link(uint32_t(node));
            }
            const int64_t slot = int16_t(guest::LoadU16(base, uint32_t(node) + 2));
            const uint64_t record = uint64_t(slot) * 1968 + garage;
            CopyCar(base, car, uint32_t(record + 212), frame, 0x8224C17C, calls);
            plates = 1;
          }
          type67_modes::GarageStageSetPlates(base, holder, plates);
          StoreU32(base, frame + 3968, LoadU32(base, frame + 3968) | flag);
          Refresh(base, car, frame, 0x8224C19C, calls);
          calls.Call(kSetCar, frame, 0x8224C1BC, holder, car,
                     LoadU32(base, b + 232) == 0 ? pick : 0xFFFFFFFFFFFFFFFFull);
          SetDrawWord(base, holder, 0, frame);
        }
        // The wait word cleared, stages 1 and 2's draw words 0.
        StoreU32(base, kStandWait, 0);
        SetDrawWord(base, Holder(base, 1), 0, frame);
        r3 = SetDrawWord(base, Holder(base, 2), 0, frame);
      }
    }
  }
  // The stages drawn: always with one player, else for player 1 with the
  // wait word set.
  if (LoadU32(base, kTwoPlayers) == 0 ||
      (LoadU32(base, kStandWait) != 0 && LoadU32(base, b + 1952) == 1)) {
    r3 = DrawAll(base, frame, calls);
  }
  // Group B's movie: the busy animations while the stage is busy, the done
  // one once it isn't.
  const uint32_t group = LoadU32(base, b + 1956);
  if (group == 0) {
    return group;
  }
  const uint32_t movie = uint32_t(type67_screens::PanelGroupMovie(base, group, frame));
  r3 = movie;
  bool busy = false;
  if (LoadU32(base, b + 20) != 0) {
    r3 = calls.Call(kGarageStageIsBusy, frame, 0x8224C26C, holder);
    busy = int32_t(uint32_t(r3)) != 0;
  }
  const auto play = [&](uint32_t at, uint64_t looped, uint32_t lr_finish, uint32_t lr_play) {
    calls.Call(kFinishNamed, frame, lr_finish, movie, LoadU32(base, b + at));
    const uint64_t args[8] = {movie, LoadU32(base, b + at), looped, 0, 0, 0, 0, 0};
    return calls.CallArgs(kPlayNamed, frame, lr_play, args);
  };
  if (busy) {
    if (LoadU32(base, b + 28) != 0) {
      return r3;
    }
    if (int32_t(LoadU32(base, b + 1960)) >= 0) {
      r3 = play(1960, 0, 0x8224C294, 0x8224C2B0);
    }
    if (int32_t(LoadU32(base, b + 1964)) >= 0) {
      play(1964, 1, 0x8224C2C4, 0x8224C2E0);
      r3 = calls.Call(kLoopNamed, frame, 0x8224C2EC, movie, LoadU32(base, b + 1964));
    }
    StoreU32(base, b + 28, 1);
    return r3;
  }
  if (LoadU32(base, b + 28) == 0) {
    return r3;
  }
  if (int32_t(LoadU32(base, b + 1968)) >= 0) {
    r3 = play(1968, 0, 0x8224C428, 0x8224C444);
  }
  StoreU32(base, b + 28, 0);
  return r3;
}

uint64_t Update(uint8_t* base, uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CD00 PanelSetBase::Update");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(base, sp, 368);
  // The steps' table on the frame (+96 to +200), and the two words the
  // original's lis leaves at +80 and +84 on the way.
  static constexpr uint32_t kSteps[27] = {
      0x8224A570, 0x8224A6A0, 0x82248F00, 0x8224A718, 0x8224B078, 0x8224B138, 0x82248F70,
      0x82249098, 0x82248FE0, 0x8224CB28, 0x8224CC00, 0x8224A7E0, 0x82249128, 0x8224A850,
      0x8224B1C8, 0x82249198, 0x822491F8, 0x8224C450, 0x82249258, 0x822492C8, 0x8224CC78,
      0x822493D8, 0x82249458, 0x822494A8, 0x82249510, 0x8224C4D8, 0x8224A8E0};
  StoreU32(base, frame + 80, 0x82250000);
  StoreU32(base, frame + 84, 0x82250000);
  for (uint32_t i = 0; i < 27; i++) {
    StoreU32(base, frame + 96 + 4 * i, kSteps[i]);
  }
  const int32_t step = int32_t(LoadU32(base, b + 44));
  bool advance = false;
  if (step < -1) {
    advance = true;
  } else if (step >= 0) {
    const uint32_t handler = LoadU32(base, frame + 96 + (uint32_t(step) << 2));
    const uint32_t word = b + 56;
    uint64_t answer = 0;
    switch (handler) {
      case 0x82248F00:
        answer = Step2(base, b, word, frame, calls);
        break;
      case 0x8224A718:
        answer = Step3(base, b, word, frame, calls);
        break;
      case 0x82249098:
        answer = Step7(base, b, word, frame, calls);
        break;
      case 0x8224A7E0:
        answer = Step11(base, b, word, frame, calls);
        break;
      case 0x82249258:
        answer = Step18(base, b, word, frame, calls);
        break;
      case 0x8224A8E0:
        answer = Step26(base, b, word, frame, calls);
        break;
      case 0x8224A570:
        answer = Step0(base, b_r3, word, frame, calls);
        break;
      case 0x8224A6A0:
        answer = Step1(base, b_r3, word, frame, calls);
        break;
      case 0x8224B078:
        answer = Step4(base, b_r3, word, frame, calls);
        break;
      case 0x82248F70:
        answer = Step6(base, b_r3, word, frame, calls);
        break;
      case 0x82248FE0:
        answer = Step8(base, b_r3, word, frame, calls);
        break;
      case 0x8224CB28:
        answer = Step9(base, b_r3, word, frame, calls);
        break;
      case 0x82249128:
        answer = Step12(base, b_r3, word, frame, calls);
        break;
      case 0x822492C8:
        answer = Step19(base, b_r3, word, frame, calls);
        break;
      case 0x8224C450:
        answer = Step17(base, b_r3, word, frame, calls);
        break;
      default:
        answer = calls.Call(handler, frame, 0x8224CE94, b_r3, b_r3 + 56);
        break;
    }
    advance = int32_t(uint32_t(answer)) != 0 && LoadU32(base, kFadeWait) == 0;
  }
  if (advance) {
    const uint32_t queued = LoadU32(base, b + 48);
    if (queued == 0xFFFFFFFFu) {
      StoreU32(base, b + 32, LoadU32(base, b + 36));
      StoreU32(base, b + 44, 0xFFFFFFFFu);
    } else {
      StoreU32(base, b + 52, 0);
      StoreU32(base, b + 48, 0xFFFFFFFFu);
      StoreU32(base, b + 44, queued);
    }
    StoreU32(base, b + 56, 0);
  }
  UpdateStand(base, b_r3, frame, calls);
  return 1;
}

}  // namespace car_list
