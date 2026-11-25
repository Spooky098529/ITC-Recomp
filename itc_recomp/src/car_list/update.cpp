// PanelSet's update (docs/functions/car-list.md): see car_list.h.

#include "car_list/car_list.h"
#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_makers/car_makers.h"
#include "car_specs/car_specs.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "schema/car_list.h"
#include "schema/career.h"
#include "schema/race_setup.h"
#include "schema/stand_models.h"
#include "schema/tuning_screen.h"
#include "type67_modes/type67_modes.h"
#include "type67_screens/type67_screens.h"

#include "base/float_mode.h"

namespace car_list {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::CarListGlobals;
using Base = schema::PanelSetBase;
using Entry = schema::CarListEntry;
using Stage = schema::GarageStage;  // stand-models.toml

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  StoreU32(sp - bytes, sp);
  return sp - bytes;
}

// The stand's holder in slot `slot` ([0x826D83E0] read each time, as the
// originals do).
uint32_t Holder(uint32_t slot) {
  return LoadU32(G::stand_holders().stage_block_0_address() + 4 * slot);
}

// GarageStage::SetDrawWord's plain C++ (a leaf of its own: no guest calls).
uint64_t SetDrawWord(uint32_t holder, uint32_t word, uint32_t frame) {
  return front_end_car_draw::GarageStageSetDrawWord(holder, word, frame);
}

// Car::Refresh on the frame's car: its plain C++ (flush-to-zero off, as it
// expects), else its original.
void Refresh(uint32_t car, uint32_t frame, uint32_t lr, const Calls& calls) {
  using Fpscr = ::base::FloatMode;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(car, &refreshed)) {
    calls.Call(kCarRefresh, frame, lr, car);
  }
}

// memcpy(to, from, 1884): its plain C++, else the original.
void CopyCar(uint32_t to, uint32_t from, uint32_t frame, uint32_t lr,
             const Calls& calls) {
  if (!c_runtime::Memcpy(to, from, 1884)) {
    calls.Call(kMemcpy, frame, lr, to, from, 1884);
  }
}

}  // namespace

uint64_t DrawAll(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821807D8 GarageStages::DrawAll");
  const uint32_t frame = Frame(sp, 160);
  for (uint32_t i = 0; i < 3; i++) {
    StoreU32(frame + 80 + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 3; i++) {
    StoreU32(frame + 96 + 4 * i, 1);
  }
  // The draw words kept, each stage's set to 0.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t holder = Holder(i);
    const uint32_t stage = LoadU32(holder);
    if (Stage(stage).model_claimed() == 0) {
      StoreU32(frame + 80 + 4 * i, 0);
    } else {
      StoreU32(frame + 80 + 4 * i, Stage(stage).draw_word());
      SetDrawWord(holder, 0, frame);
    }
  }
  // Each stage with a kept word drawn alone.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t holder = Holder(i);
    if (Stage(LoadU32(holder)).model_claimed() == 0 ||
        LoadU32(frame + 80 + 4 * i) == 0) {
      continue;
    }
    SetDrawWord(holder, 1, frame);
    calls.Call(kGarageStageDraw, frame, 0x8218089C, holder, 1,
               LoadU32(frame + 96 + 4 * i));
    SetDrawWord(holder, 0, frame);
  }
  // The words put back.
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t holder = Holder(i);
    r3 = holder;
    if (Stage(LoadU32(holder)).model_claimed() != 0) {
      r3 = SetDrawWord(holder, LoadU32(frame + 80 + 4 * i), frame);
    }
  }
  return r3;
}

uint64_t UpdateStand(uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224C008 PanelSetBase::UpdateStand");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(sp, 9952);
  uint64_t r3 = 0;
  // The entry's row (+240 + +236), its car id, and the stage's model
  // reloaded for it (CarModels::BlockOf's plain C++, GarageStage::
  // ReloadModel through its override; +24 its answer).
  uint64_t row = 0;
  uint32_t id = 0;
  const auto reload = [&](uint32_t holder, uint32_t lr) {
    row = uint64_t(Base(b).scroll()) + uint64_t(Base(b).row());
    const uint32_t entry =
        LoadU32(Base(b).current_list().entries_address() + (uint32_t(row) << 2));
    id = Entry(entry).car_id();
    const uint64_t answer = calls.Call(kReloadModel, frame, lr, holder, car_makers::BlockOf(id));
    Base(b).set_stand_model_loaded(uint32_t(answer));
    return answer;
  };
  const bool two = G::two_players() != 0;
  const bool second = two && Base(b).player() != 0;
  const uint32_t holder = Holder(two ? (second ? 2 : 1) : 0);
  if (!two) {
    // One player: stage 0.
    r3 = SetDrawWord(holder, 0, frame);
    if (Base(b).show_stand_car() != 0) {
      if (Base(b).stand_model_loaded() != 0) {
        r3 = SetDrawWord(holder, 1, frame);
      } else {
        r3 = reload(holder, 0x8224C358);
        if (int32_t(uint32_t(r3)) == 1) {
          const uint64_t pick =
              uint64_t(Base(b).color_scroll()) + uint64_t(Base(b).color());
          const uint32_t car = frame + 3984;
          race_setup::CarInit(car);
          uint32_t plates = 0;
          if (Base(b).tab() == 2) {
            // The garage tab's car (GarageScreen::CopyGarageCar's plain
            // C++), copied in.
            const uint64_t save = SaveData(b, frame);
            const uint64_t from = garage_screen::CopyGarageCar(frame + 7888, schema::CareerSave(uint32_t(save)).garage_address(),
                uint32_t(row), frame, calls);
            CopyCar(car, uint32_t(from), frame, 0x8224C3B0, calls);
            plates = 1;
          } else {
            race_setup::SetModel(car, id);
          }
          type67_modes::GarageStageSetPlates(holder, plates);
          schema::Car(car).set_flags(schema::Car(car).flags() | 1);
          Refresh(car, frame, 0x8224C3E4, calls);
          r3 = calls.Call(kSetCar, frame, 0x8224C404, holder, car,
                          Base(b).tab() == 0 ? pick : 0xFFFFFFFFFFFFFFFFull);
        }
      }
    }
  } else {
    // Two players (never seen in play): stage 1 or 2 by the player.
    const uint32_t flag = second ? 128 : 1;
    const uint32_t command = second ? 57 : 56;
    r3 = SetDrawWord(holder, 0, frame);
    if (Base(b).show_stand_car() != 0) {
      if (Base(b).stand_model_loaded() != 0) {
        if (G::stand_wait() != 0) {
          StoreU32(LoadU32(holder) + 6052, 0);
          calls.Call(kGarageStageCommand, frame, 0x8224C210, holder, command);
          r3 = SetDrawWord(holder, 1, frame);
        }
      } else {
        r3 = reload(holder, 0x8224C0C4);
        if (int32_t(uint32_t(r3)) == 1) {
          uint64_t pick = 0;
          if (Base(b).page() == 3) {
            const uint64_t item = calls.Call(kMenuListSelection, frame, 0x8224C0E8, b_r3 + 60);
            pick = uint64_t(Base(b).page_menu_scroll()) + item;
          }
          const uint32_t car = frame + 80;
          race_setup::CarInit(car);
          uint32_t plates = 0;
          if (Base(b).tab() != 2) {
            race_setup::SetModel(car, id);
          } else {
            // The player's garage (sub_82105678's save data pointer: ported,
            // as SaveData): the row's car by the halfword links from +204.
            const uint32_t save = G::pair_save_data().address;
            const int64_t offset = int64_t(int32_t(Base(b).player())) * int64_t(120108);
            const uint64_t garage = uint64_t(offset) + uint64_t(save) + 6120;
            const auto link = [&](uint32_t at) {
              const int32_t next = int16_t(guest::LoadU16(at));
              return uint64_t(uint32_t(next) << 2) + garage;
            };
            uint64_t node = link(schema::CareerGarageList(uint32_t(garage)).head_address());
            for (uint32_t n = uint32_t(row); n != 0; n--) {
              node = link(uint32_t(node));
            }
            const int64_t slot = int16_t(guest::LoadU16(uint32_t(node) + 2));
            const uint64_t record = uint64_t(slot) * 1968 + garage;
            CopyCar(car, uint32_t(record + 212), frame, 0x8224C17C, calls);
            plates = 1;
          }
          type67_modes::GarageStageSetPlates(holder, plates);
          schema::Car(car).set_flags(schema::Car(car).flags() | flag);
          Refresh(car, frame, 0x8224C19C, calls);
          calls.Call(kSetCar, frame, 0x8224C1BC, holder, car,
                     Base(b).tab() == 0 ? pick : 0xFFFFFFFFFFFFFFFFull);
          SetDrawWord(holder, 0, frame);
        }
        // The wait word cleared, stages 1 and 2's draw words 0.
        G::set_stand_wait(0);
        SetDrawWord(Holder(1), 0, frame);
        r3 = SetDrawWord(Holder(2), 0, frame);
      }
    }
  }
  // The stages drawn: always with one player, else for player 1 with the
  // wait word set.
  if (G::two_players() == 0 || (G::stand_wait() != 0 && Base(b).player() == 1)) {
    r3 = DrawAll(frame, calls);
  }
  // Group B's movie: the busy animations while the stage is busy, the done
  // one once it isn't.
  const uint32_t group = Base(b).group_b().address;
  if (group == 0) {
    return group;
  }
  const uint32_t movie = uint32_t(type67_screens::PanelGroupMovie(group, frame));
  r3 = movie;
  bool busy = false;
  if (Base(b).show_stand_car() != 0) {
    r3 = calls.Call(kGarageStageIsBusy, frame, 0x8224C26C, holder);
    busy = int32_t(uint32_t(r3)) != 0;
  }
  const auto play = [&](uint32_t at, uint64_t looped, uint32_t lr_finish, uint32_t lr_play) {
    calls.Call(kFinishNamed, frame, lr_finish, movie, LoadU32(at));
    const uint64_t args[8] = {movie, LoadU32(at), looped, 0, 0, 0, 0, 0};
    return calls.CallArgs(kPlayNamed, frame, lr_play, args);
  };
  if (busy) {
    if (Base(b).busy_playing() != 0) {
      return r3;
    }
    if (int32_t(Base(b).busy_anim_a()) >= 0) {
      r3 = play(Base(b).busy_anim_a_address(), 0, 0x8224C294, 0x8224C2B0);
    }
    if (int32_t(Base(b).busy_anim_b()) >= 0) {
      play(Base(b).busy_anim_b_address(), 1, 0x8224C2C4, 0x8224C2E0);
      r3 = calls.Call(kLoopNamed, frame, 0x8224C2EC, movie, Base(b).busy_anim_b());
    }
    Base(b).set_busy_playing(1);
    return r3;
  }
  if (Base(b).busy_playing() == 0) {
    return r3;
  }
  if (int32_t(Base(b).busy_anim_done()) >= 0) {
    r3 = play(Base(b).busy_anim_done_address(), 0, 0x8224C428, 0x8224C444);
  }
  Base(b).set_busy_playing(0);
  return r3;
}

uint64_t Update(uint64_t b_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224CD00 PanelSetBase::Update");
  const uint32_t b = uint32_t(b_r3);
  const uint32_t frame = Frame(sp, 368);
  // The steps' table on the frame (+96 to +200), and the two words the
  // original's lis leaves at +80 and +84 on the way.
  static constexpr uint32_t kSteps[27] = {
      0x8224A570, 0x8224A6A0, 0x82248F00, 0x8224A718, 0x8224B078, 0x8224B138, 0x82248F70,
      0x82249098, 0x82248FE0, 0x8224CB28, 0x8224CC00, 0x8224A7E0, 0x82249128, 0x8224A850,
      0x8224B1C8, 0x82249198, 0x822491F8, 0x8224C450, 0x82249258, 0x822492C8, 0x8224CC78,
      0x822493D8, 0x82249458, 0x822494A8, 0x82249510, 0x8224C4D8, 0x8224A8E0};
  StoreU32(frame + 80, 0x82250000);
  StoreU32(frame + 84, 0x82250000);
  for (uint32_t i = 0; i < 27; i++) {
    StoreU32(frame + 96 + 4 * i, kSteps[i]);
  }
  const int32_t step = int32_t(Base(b).step());
  bool advance = false;
  if (step < -1) {
    advance = true;
  } else if (step >= 0) {
    const uint32_t handler = LoadU32(frame + 96 + (uint32_t(step) << 2));
    const uint32_t word = Base(b).first_pass_address();
    uint64_t answer = 0;
    switch (handler) {
      case 0x82248F00:
        answer = Step2(b, word, frame, calls);
        break;
      case 0x8224A718:
        answer = Step3(b, word, frame, calls);
        break;
      case 0x82249098:
        answer = Step7(b, word, frame, calls);
        break;
      case 0x8224A7E0:
        answer = Step11(b, word, frame, calls);
        break;
      case 0x82249258:
        answer = Step18(b, word, frame, calls);
        break;
      case 0x8224A8E0:
        answer = Step26(b, word, frame, calls);
        break;
      case 0x8224A570:
        answer = Step0(b_r3, word, frame, calls);
        break;
      case 0x8224A6A0:
        answer = Step1(b_r3, word, frame, calls);
        break;
      case 0x8224B078:
        answer = Step4(b_r3, word, frame, calls);
        break;
      case 0x82248F70:
        answer = Step6(b_r3, word, frame, calls);
        break;
      case 0x82248FE0:
        answer = Step8(b_r3, word, frame, calls);
        break;
      case 0x8224CB28:
        answer = Step9(b_r3, word, frame, calls);
        break;
      case 0x82249128:
        answer = Step12(b_r3, word, frame, calls);
        break;
      case 0x822492C8:
        answer = Step19(b_r3, word, frame, calls);
        break;
      case 0x8224C450:
        answer = Step17(b_r3, word, frame, calls);
        break;
      default:
        answer = calls.Call(handler, frame, 0x8224CE94, b_r3, b_r3 + 56);
        break;
    }
    advance = int32_t(uint32_t(answer)) != 0 && G::fade_wait() == 0;
  }
  if (advance) {
    const uint32_t queued = Base(b).queued_step();
    if (queued == 0xFFFFFFFFu) {
      Base(b).set_page(Base(b).next_page());
      Base(b).set_step(0xFFFFFFFFu);
    } else {
      Base(b).set_step_cleared(0);
      Base(b).set_queued_step(0xFFFFFFFFu);
      Base(b).set_step(queued);
    }
    Base(b).set_first_pass(0);
  }
  UpdateStand(b_r3, frame, calls);
  return 1;
}

}  // namespace car_list
