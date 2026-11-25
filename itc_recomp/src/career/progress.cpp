// The progress after a race (the unlocks, the rewards, the stage reached)
// and the day's advance (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "race_world/race_world.h"
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/race_event.h"
#include "schema/race_states.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <climits>

namespace career {

using schema::CareerCondition;
using schema::CareerEntry40;
using schema::CareerEntry56;
using schema::CareerGlobals;
using schema::CareerSave;
using schema::CareerStats;
using schema::OpponentItem;

namespace {

bool BitSet(uint32_t set, int32_t b) {
  return (guest::LoadU32(set + (uint32_t(b >> 5) << 2)) & (1u << (b & 31))) != 0;
}

void SetBit(uint32_t set, int32_t b) {
  const uint32_t word = set + (uint32_t(b >> 5) << 2);
  guest::StoreU32(word, guest::LoadU32(word) | (1u << (b & 31)));
}

// Over the 400 items with player 0's records: whether `want` holds for
// one (`any`) or, with !any, whether none that `want`s lacks a win.
template <typename Want>
uint32_t ItemsWon(bool any, Want want) {
  const uint32_t records =
      CareerSave(vehicle_accessors::SaveDataOf(0)).item_records_address();
  for (uint32_t id = 0; id < 400; id++) {
    const uint32_t item = data_tables::Item(id);
    if (!want(item)) {
      continue;
    }
    const bool won = schema::CareerItemRecord(records + 12 * id).wins() != 0;
    if (any && won) {
      return 1;
    }
    if (!any && !won) {
      return 0;
    }
  }
  return any ? 0 : 1;
}

// RunningMean::Add of an integer (converted as the original does, std, lfd,
// fcfid, frsp) on {0.0, 0}, merged into `into` (RunningMean::Merge's). The
// original's frame +80 and the conversion's +88 are values since E5 step 6.
void MergeInt(uint32_t into, int32_t value) {
  const float mean = race_setup::MeanAfterAdd(CareerGlobals::zero(), 0,
                                              double(float(double(int64_t(value)))), 1);
  RunningMeanMergeInto(into, (uint64_t(std::bit_cast<uint32_t>(mean)) << 32) | 1);
}

// The 40-byte rewards whose conditions pass Condition::Test(c, 0, entry +0)
// granted (Rewards::Grant), as Progress::AfterRace and CheckUnlocks run them;
// Condition::Test from `frame` (the caller's).
void RewardsPass(uint32_t save, uint32_t frame) {
  const CareerSave sv(save);
  for (int32_t i = 0;; i++) {
    const CareerEntry40 e(data_tables::Entry40(uint32_t(i)));
    if (int32_t(CareerCondition(e.conditions_address()).kind()) == -1) {
      break;
    }
    if (i >= 0 && BitSet(sv.rewards_granted_address(), i)) {
      continue;
    }
    bool grant = false;
    const bool all = e.all_conditions() != 0;
    for (int32_t k = 0; k < 3; k++) {
      const uint32_t condition = e.conditions_address() + 8 * uint32_t(k);
      const CareerCondition c(condition);
      if (int32_t(c.kind()) == -1 || int32_t(c.value()) == -1) {
        break;
      }
      const uint32_t pass = ConditionTest(condition, 0, e.kind_mask(), frame).value_or(0);
      if (!all) {
        if (pass != 0) {
          grant = true;
          break;
        }
      } else {
        if (pass == 0) {
          grant = false;
          break;
        }
        grant = true;
      }
    }
    if (grant) {
      RewardsGrant(save, uint32_t(i), e.reward_kind(), e.reward_value());
    }
  }
}

}  // namespace

uint32_t AnyWonKind3Stage2() {
  ITC_NATIVE_RUNNING("sub_82111818 Items::AnyWonKind3Stage2");
  return ItemsWon(true, [](uint32_t item) {
    const OpponentItem it(item);
    return it.level() == 2 && it.location() == 1 && it.kind() == 3;
  });
}

uint32_t AllWonKind3() {
  ITC_NATIVE_RUNNING("sub_821118B0 Items::AllWonKind3");
  return ItemsWon(false, [](uint32_t item) {
    const OpponentItem it(item);
    return it.level() == 1 && it.kind() == 3;
  });
}

uint32_t AllWonKind4Stage2() {
  ITC_NATIVE_RUNNING("sub_82111940 Items::AllWonKind4Stage2");
  return ItemsWon(false, [](uint32_t item) {
    const OpponentItem it(item);
    return it.level() == 2 && it.kind() == 4;
  });
}

uint32_t AllWonKind4Stage3() {
  ITC_NATIVE_RUNNING("sub_821119D0 Items::AllWonKind4Stage3");
  return ItemsWon(false, [](uint32_t item) {
    const OpponentItem it(item);
    return it.level() == 3 && it.kind() == 4;
  });
}

uint32_t AllWonKind5() {
  ITC_NATIVE_RUNNING("sub_82111A60 Items::AllWonKind5");
  return ItemsWon(false,
                  [](uint32_t item) { return OpponentItem(item).kind() == 5; });
}

void LocationsUnlock(uint32_t save, uint32_t bit) {
  ITC_NATIVE_RUNNING("sub_82111D20 Locations::Unlock");
  if (int32_t(bit) >= 0) {
    SetBit(CareerSave(save).locations_address(), int32_t(bit));
  }
}

void LocationsUnlock2(uint32_t save, uint32_t bit) {
  ITC_NATIVE_RUNNING("sub_82111D98 Locations::Unlock2");
  if (int32_t(bit) >= 0) {
    SetBit(CareerSave(save).locations_2_address(), int32_t(bit));
  }
}

bool AfterRaceNative(uint32_t sp) {
  return ConditionTestFits(sp - kAfterRaceFrame);
}

uint32_t ProgressAfterRace(uint32_t save, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82116588 Progress::AfterRace");
  const uint32_t frame = sp - kAfterRaceFrame;
  const CareerSave sv(save);
  const CareerStats stats(sv.stats_address());
  const auto totals = CareerGlobals::race_data();
  stats.set_day_total_4(totals.stat_1() + stats.day_total_4());
  stats.set_day_total_0(totals.stat_0() + stats.day_total_0());
  stats.set_day_total_8(totals.finish_count() + stats.day_total_8());
  stats.set_day_total_12(uint32_t(int32_t(totals.stat_3()) / 50) +
                         stats.day_total_12());
  // The location: +6112 unless 48; else +6108 for a rival race (the race
  // data's flags +0x59AE4, 0x10), else 48.
  uint32_t location = sv.next_location();
  if (int32_t(location) == 48) {
    const uint32_t flags = CareerGlobals::race_data().flags();
    location = (flags & 0x10) != 0 ? sv.location() : 48;
  }
  // The table-0 unlocks.
  for (int32_t i = 0;; i++) {
    const uint32_t entry = data_tables::Entry56(0, uint32_t(i));
    const CareerEntry56 e(entry);
    const int32_t kind = int32_t(e.kind());
    if (kind == -1) {
      break;
    }
    if (kind == 4 || (i >= 0 && BitSet(sv.table0_done_address(), i))) {
      continue;
    }
    if (Entry56Allows(entry, sv.mode(), location, StageOf(save), 1) == 0) {
      continue;
    }
    bool grant = false;
    const bool all = e.all_conditions() != 0;
    for (int32_t k = 0; k < 3; k++) {
      const uint32_t condition = e.conditions_address() + 8 * uint32_t(k);
      const CareerCondition c(condition);
      if (int32_t(c.kind()) == -1 || int32_t(c.value()) == -1) {
        if (k == 0) {
          grant = true;
        }
        break;
      }
      const uint32_t pass = ConditionTest(condition, 0, 0, frame).value_or(0);
      if (!all) {
        if (pass != 0) {
          grant = true;
          break;
        }
      } else {
        if (pass == 0) {
          grant = false;
          break;
        }
        grant = true;
      }
    }
    if (!grant) {
      continue;
    }
    if (i >= 0) {
      SetBit(sv.table0_done_address(), i);
    }
    const uint32_t list = sv.unlocks_shown_address();
    const uint32_t at = list + 12 * sv.unlocks_shown_count();
    guest::StoreU32(at + 0, 0);
    guest::StoreU32(at + 4, 0);
    guest::StoreU32(at + 8, uint32_t(i));
    sv.set_unlocks_shown_count(sv.unlocks_shown_count() + 1);
  }
  // The 40-byte rewards.
  RewardsPass(save, frame);
  // The stage reached.
  uint32_t reached = 0xFFFFFFFFu;
  const auto stage_bit = [&](uint32_t bit) {
    sv.set_stages(sv.stages() | bit);
  };
  switch (race_world::FirstUnsetFlag(save)) {
    case 0:
      if (AnyWonKind3Stage1() != 0) {
        stage_bit(1);
        for (uint32_t bit : {4u, 19u, 20u}) {
          LocationsUnlock(save, bit);
        }
        for (uint32_t bit : {12u, 18u}) {
          LocationsUnlock2(save, bit);
        }
        reached = 2;
      }
      break;
    case 1:
      if (AllWonKind3() != 0) {
        stage_bit(2);
        for (uint32_t bit : {2u, 3u, 15u, 16u, 17u, 18u}) {
          LocationsUnlock(save, bit);
        }
        for (uint32_t bit : {4u, 5u, 6u, 7u, 16u, 17u}) {
          LocationsUnlock2(save, bit);
        }
        reached = 3;
      }
      break;
    case 2:
      if (AnyWonKind3Stage2() != 0) {
        stage_bit(4);
        for (uint32_t bit : {5u, 21u, 22u}) {
          LocationsUnlock(save, bit);
        }
        for (uint32_t bit : {13u, 19u}) {
          LocationsUnlock2(save, bit);
        }
        reached = 4;
      }
      break;
    case 3:
      if (AllWonKind4Stage2() != 0) {
        stage_bit(8);
        reached = 5;
      }
      break;
    case 4:
      if (AllWonKind4Stage3() != 0) {
        stage_bit(16);
        reached = 6;
      }
      break;
    case 5:
      if (AllWonKind5() != 0) {
        stage_bit(32);
        reached = 8;
      }
      break;
    default:
      break;
  }
  const uint32_t next = sv.next_location();
  if (int32_t(next) != 48) {
    sv.set_location(next);
    sv.set_next_location(48);
  }
  return reached;
}

bool CheckUnlocksNative(uint32_t sp) {
  return ConditionTestFits(sp - kCheckUnlocksFrame);
}

uint32_t ProgressCheckUnlocks(uint32_t save, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821162B0 Progress::CheckUnlocks");
  const uint32_t frame = sp - kCheckUnlocksFrame;
  const CareerSave sv(save);
  // The table-0 unlocks: the entry's flags (+4) instead of Entry56::Allows.
  for (int32_t i = 0;; i++) {
    const uint32_t entry = data_tables::Entry56(0, uint32_t(i));
    const CareerEntry56 e(entry);
    const int32_t kind = int32_t(e.kind());
    if (kind == -1) {
      break;
    }
    if (kind == 4 || (i >= 0 && BitSet(sv.table0_done_address(), i))) {
      continue;
    }
    const uint32_t stage = StageOf(save);
    const uint32_t flags = e.mask();
    if ((flags & 0x40) == 0) {
      continue;
    }
    if ((flags & 0x7C00) != 0) {
      if (int32_t(stage) == 4) {
        if ((flags & 0x2000) == 0) {
          continue;
        }
      } else {
        // slw: the shift's low six bits, 32 and above giving 0.
        const uint32_t shift = (stage + 10) & 0x3F;
        const uint32_t bit = (shift & 0x20) != 0 ? 0 : 1u << shift;
        if ((flags & bit) == 0) {
          continue;
        }
      }
    }
    bool grant = false;
    const bool all = e.all_conditions() != 0;
    for (int32_t k = 0; k < 3; k++) {
      const uint32_t condition = e.conditions_address() + 8 * uint32_t(k);
      const CareerCondition c(condition);
      if (int32_t(c.kind()) == -1 || int32_t(c.value()) == -1) {
        if (k == 0) {
          grant = true;
        }
        break;
      }
      const uint32_t pass = ConditionTest(condition, 0, 0, frame).value_or(0);
      if (!all) {
        if (pass != 0) {
          grant = true;
          break;
        }
      } else {
        if (pass == 0) {
          grant = false;
          break;
        }
        grant = true;
      }
    }
    if (!grant) {
      continue;
    }
    if (i >= 0) {
      SetBit(sv.table0_done_address(), i);
    }
    const uint32_t list = sv.unlocks_shown_address();
    const uint32_t at = list + 12 * sv.unlocks_shown_count();
    guest::StoreU32(at + 0, 0);
    guest::StoreU32(at + 4, 0);
    guest::StoreU32(at + 8, uint32_t(i));
    sv.set_unlocks_shown_count(sv.unlocks_shown_count() + 1);
  }
  RewardsPass(save, frame);
  return 0;
}

uint32_t StatsCloseDay(uint32_t stats, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82113898 Stats::CloseDay");
  // The original's 128-byte frame held only the running means' temporaries
  // (values since E5 step 6).
  (void)sp;
  const CareerStats s(stats);
  const int32_t days = int16_t(s.days());
  if (days < 9999) {
    s.set_days(uint16_t(days + 1));
  }
  MergeInt(s.mean_day_total_8_address(), int32_t(s.day_total_8()));
  MergeInt(s.mean_day_total_12_address(), int32_t(s.day_total_12()));
  const int32_t numerator = int32_t(s.day_total_4() * 100u);
  const int32_t divisor = int32_t(s.day_total_0());
  int32_t quotient = 0;
  if (divisor == 0 || (numerator == INT_MIN && divisor == -1)) {
    // The original's tw traps; the translated code logs them and runs on.
    ITC_LOG_WARN("Stats::CloseDay: the division's trap ({} / {}), 0 as the original", numerator,
                 divisor);
  } else {
    quotient = numerator / divisor;
  }
  MergeInt(s.mean_day_ratio_address(), quotient);
  return s.mean_day_ratio_address();
}

uint32_t CareerNextDay(uint32_t save, uint32_t sp, PickNextCall pick_next,
                       void* user) {
  ITC_NATIVE_RUNNING("sub_82116AF0 Career::NextDay");
  const CareerSave sv(save);
  sv.set_day(sv.day() + 1);
  const uint32_t garage = sv.garage_address();
  const schema::CareerGarageList gl(garage);
  for (uint32_t i = 0; i < uint32_t(int32_t(int16_t(sv.garage_count()))); i++) {
    uint32_t node = (uint32_t(int32_t(int16_t(gl.head()))) << 2) + garage;
    for (uint32_t n = i; n != 0; n--) {
      node = (uint32_t(int32_t(int16_t(guest::LoadU16(node)))) << 2) + garage;
    }
    const uint32_t car =
        uint32_t(int32_t(int16_t(guest::LoadU16(node + 2)))) * 1968 + gl.cars_address();
    const schema::GarageCar gc(car);
    gc.set_unknown_1896(gc.unknown_1896() + 1);
  }
  sv.set_unknown_109792(0);
  const uint32_t stats = sv.stats_address();
  // (The original's 112-byte frame is left out, E5 step 6: Stats::CloseDay
  // no longer makes one, and Career::PickNext's adapter makes its own.)
  StatsCloseDay(stats, sp);
  if (!PickNextSkips(save)) {
    pick_next(user, save);
  }
  const int32_t day = int32_t(sv.day());
  return StatsStartDay(stats, uint32_t(day % 7));
}

}  // namespace career
