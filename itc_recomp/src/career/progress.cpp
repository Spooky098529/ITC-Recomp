// The progress after a race (the unlocks, the rewards, the stage reached)
// and the day's advance (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "race_world/race_world.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <climits>

namespace career {

namespace {

constexpr uint32_t kRaceDataSlot = 0x826C5B2C;

bool BitSet(uint8_t* base, uint32_t set, int32_t b) {
  return (guest::LoadU32(base, set + (uint32_t(b >> 5) << 2)) & (1u << (b & 31))) != 0;
}

void SetBit(uint8_t* base, uint32_t set, int32_t b) {
  const uint32_t word = set + (uint32_t(b >> 5) << 2);
  guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (b & 31)));
}

// Over the 400 items with player 0's records: whether `want` holds for
// one (`any`) or, with !any, whether none that `want`s lacks a win.
template <typename Want>
uint32_t ItemsWon(uint8_t* base, bool any, Want want) {
  const uint32_t records = vehicle_accessors::SaveDataOf(base, 0) + kItemRecords;
  for (uint32_t id = 0; id < 400; id++) {
    const uint32_t item = data_tables::Item(base, id);
    if (!want(item)) {
      continue;
    }
    const bool won = guest::LoadU16(base, records + 12 * id + 2) != 0;
    if (any && won) {
      return 1;
    }
    if (!any && !won) {
      return 0;
    }
  }
  return any ? 0 : 1;
}

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}

// RunningMean::Add of an integer (converted through frame +88, as the
// original) on the frame's {0.0, 0} at +80, merged into `into`.
void MergeInt(uint8_t* base, uint32_t frame, uint32_t into, int32_t value) {
  guest::StoreFloat(base, frame + 80, guest::LoadFloat(base, kZeroFloat));
  guest::StoreU32(base, frame + 84, 0);
  guest::StoreU32(base, frame + 88, uint32_t(uint64_t(int64_t(value)) >> 32));
  guest::StoreU32(base, frame + 92, uint32_t(value));
  race_setup::RunningMeanAdd(base, frame + 80, double(float(double(int64_t(value)))), 1);
  RunningMeanMerge(base, into, LoadU64(base, frame + 80), frame);
}

}  // namespace

uint32_t AnyWonKind3Stage2(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82111818 Items::AnyWonKind3Stage2");
  return ItemsWon(base, true, [base](uint32_t item) {
    return guest::LoadU16(base, item + 42) == 2 && guest::LoadU8(base, item + 36) == 1 &&
           guest::LoadU8(base, item + 35) == 3;
  });
}

uint32_t AllWonKind3(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821118B0 Items::AllWonKind3");
  return ItemsWon(base, false, [base](uint32_t item) {
    return guest::LoadU16(base, item + 42) == 1 && guest::LoadU8(base, item + 35) == 3;
  });
}

uint32_t AllWonKind4Stage2(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82111940 Items::AllWonKind4Stage2");
  return ItemsWon(base, false, [base](uint32_t item) {
    return guest::LoadU16(base, item + 42) == 2 && guest::LoadU8(base, item + 35) == 4;
  });
}

uint32_t AllWonKind4Stage3(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821119D0 Items::AllWonKind4Stage3");
  return ItemsWon(base, false, [base](uint32_t item) {
    return guest::LoadU16(base, item + 42) == 3 && guest::LoadU8(base, item + 35) == 4;
  });
}

uint32_t AllWonKind5(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82111A60 Items::AllWonKind5");
  return ItemsWon(base, false,
                  [base](uint32_t item) { return guest::LoadU8(base, item + 35) == 5; });
}

void LocationsUnlock(uint8_t* base, uint32_t save, uint32_t bit) {
  ITC_NATIVE_RUNNING("sub_82111D20 Locations::Unlock");
  if (int32_t(bit) >= 0) {
    SetBit(base, save + 0x199E0, int32_t(bit));
  }
}

void LocationsUnlock2(uint8_t* base, uint32_t save, uint32_t bit) {
  ITC_NATIVE_RUNNING("sub_82111D98 Locations::Unlock2");
  if (int32_t(bit) >= 0) {
    SetBit(base, save + 0x199E8, int32_t(bit));
  }
}

bool AfterRaceNative(uint8_t* base, uint32_t sp) {
  return ConditionTestFits(base, sp - kAfterRaceFrame);
}

uint32_t ProgressAfterRace(uint8_t* base, uint32_t save, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82116588 Progress::AfterRace");
  const uint32_t frame = sp - kAfterRaceFrame;
  const uint32_t stats = save + kStats;
  const uint32_t totals = guest::LoadU32(base, kRaceDataSlot) + 0xA3AD0;
  guest::StoreU32(base, stats + 5952,
                  guest::LoadU32(base, totals + 4) + guest::LoadU32(base, stats + 5952));
  guest::StoreU32(base, stats + 5956,
                  guest::LoadU32(base, totals + 0) + guest::LoadU32(base, stats + 5956));
  guest::StoreU32(base, stats + 5960,
                  guest::LoadU32(base, totals + 8) + guest::LoadU32(base, stats + 5960));
  guest::StoreU32(base, stats + 5964,
                  uint32_t(int32_t(guest::LoadU32(base, totals + 12)) / 50) +
                      guest::LoadU32(base, stats + 5964));
  // The location: +6112 unless 48; else +6108 for a rival race (the race
  // data's flags +0x59AE4, 0x10), else 48.
  uint32_t location = guest::LoadU32(base, save + 6112);
  if (int32_t(location) == 48) {
    const uint32_t flags =
        guest::LoadU32(base, guest::LoadU32(base, kRaceDataSlot) + 0x59AE4);
    location = (flags & 0x10) != 0 ? guest::LoadU32(base, save + 6108) : 48;
  }
  // The table-0 unlocks.
  for (int32_t i = 0;; i++) {
    const uint32_t entry = data_tables::Entry56(base, 0, uint32_t(i));
    const int32_t kind = int32_t(guest::LoadU32(base, entry + 0));
    if (kind == -1) {
      break;
    }
    if (kind == 4 || (i >= 0 && BitSet(base, save + 0x1CA7C, i))) {
      continue;
    }
    if (Entry56Allows(base, entry, guest::LoadU32(base, save + 6104), location,
                      StageOf(base, save), 1) == 0) {
      continue;
    }
    bool grant = false;
    const bool all = guest::LoadU32(base, entry + 20) != 0;
    for (int32_t k = 0; k < 3; k++) {
      const uint32_t condition = entry + 24 + 8 * uint32_t(k);
      if (int32_t(guest::LoadU32(base, condition + 0)) == -1 ||
          int32_t(guest::LoadU32(base, condition + 4)) == -1) {
        if (k == 0) {
          grant = true;
        }
        break;
      }
      const uint32_t pass = ConditionTest(base, condition, 0, 0, frame).value_or(0);
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
      SetBit(base, save + 0x1CA7C, i);
    }
    const uint32_t list = save + 0x1C778;
    const uint32_t at = list + 12 * guest::LoadU32(base, list + 768);
    guest::StoreU32(base, at + 0, 0);
    guest::StoreU32(base, at + 4, 0);
    guest::StoreU32(base, at + 8, uint32_t(i));
    guest::StoreU32(base, list + 768, guest::LoadU32(base, list + 768) + 1);
  }
  // The 40-byte rewards.
  for (int32_t i = 0;; i++) {
    const uint32_t entry = data_tables::Entry40(base, uint32_t(i));
    if (int32_t(guest::LoadU32(base, entry + 16)) == -1) {
      break;
    }
    if (i >= 0 && BitSet(base, save + 0x1D508, i)) {
      continue;
    }
    bool grant = false;
    const bool all = guest::LoadU32(base, entry + 12) != 0;
    for (int32_t k = 0; k < 3; k++) {
      const uint32_t condition = entry + 16 + 8 * uint32_t(k);
      if (int32_t(guest::LoadU32(base, condition + 0)) == -1 ||
          int32_t(guest::LoadU32(base, condition + 4)) == -1) {
        break;
      }
      const uint32_t pass =
          ConditionTest(base, condition, 0, guest::LoadU32(base, entry + 0), frame).value_or(0);
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
      RewardsGrant(base, save, uint32_t(i), guest::LoadU32(base, entry + 4),
                   guest::LoadU32(base, entry + 8));
    }
  }
  // The stage reached.
  uint32_t reached = 0xFFFFFFFFu;
  const auto stage_bit = [&](uint32_t bit) {
    guest::StoreU32(base, save + 6092, guest::LoadU32(base, save + 6092) | bit);
  };
  switch (race_world::FirstUnsetFlag(base, save)) {
    case 0:
      if (AnyWonKind3Stage1(base) != 0) {
        stage_bit(1);
        for (uint32_t bit : {4u, 19u, 20u}) {
          LocationsUnlock(base, save, bit);
        }
        for (uint32_t bit : {12u, 18u}) {
          LocationsUnlock2(base, save, bit);
        }
        reached = 2;
      }
      break;
    case 1:
      if (AllWonKind3(base) != 0) {
        stage_bit(2);
        for (uint32_t bit : {2u, 3u, 15u, 16u, 17u, 18u}) {
          LocationsUnlock(base, save, bit);
        }
        for (uint32_t bit : {4u, 5u, 6u, 7u, 16u, 17u}) {
          LocationsUnlock2(base, save, bit);
        }
        reached = 3;
      }
      break;
    case 2:
      if (AnyWonKind3Stage2(base) != 0) {
        stage_bit(4);
        for (uint32_t bit : {5u, 21u, 22u}) {
          LocationsUnlock(base, save, bit);
        }
        for (uint32_t bit : {13u, 19u}) {
          LocationsUnlock2(base, save, bit);
        }
        reached = 4;
      }
      break;
    case 3:
      if (AllWonKind4Stage2(base) != 0) {
        stage_bit(8);
        reached = 5;
      }
      break;
    case 4:
      if (AllWonKind4Stage3(base) != 0) {
        stage_bit(16);
        reached = 6;
      }
      break;
    case 5:
      if (AllWonKind5(base) != 0) {
        stage_bit(32);
        reached = 8;
      }
      break;
    default:
      break;
  }
  const uint32_t next = guest::LoadU32(base, save + 6112);
  if (int32_t(next) != 48) {
    guest::StoreU32(base, save + 6108, next);
    guest::StoreU32(base, save + 6112, 48);
  }
  return reached;
}

uint32_t StatsCloseDay(uint8_t* base, uint32_t stats, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82113898 Stats::CloseDay");
  const uint32_t frame = sp - kCloseDayFrame;
  const int32_t days = int16_t(guest::LoadU16(base, stats + 6028));
  if (days < 9999) {
    guest::StoreU16(base, stats + 6028, uint16_t(days + 1));
  }
  MergeInt(base, frame, stats + 5936, int32_t(guest::LoadU32(base, stats + 5960)));
  MergeInt(base, frame, stats + 5944, int32_t(guest::LoadU32(base, stats + 5964)));
  const int32_t numerator = int32_t(guest::LoadU32(base, stats + 5952) * 100u);
  const int32_t divisor = int32_t(guest::LoadU32(base, stats + 5956));
  int32_t quotient = 0;
  if (divisor == 0 || (numerator == INT_MIN && divisor == -1)) {
    // The original's tw traps; the translated code logs them and runs on.
    ITC_LOG_WARN("Stats::CloseDay: the division's trap ({} / {}), 0 as the original", numerator,
                 divisor);
  } else {
    quotient = numerator / divisor;
  }
  MergeInt(base, frame, stats + 5928, quotient);
  return stats + 5928;
}

uint32_t CareerNextDay(uint8_t* base, uint32_t save, uint32_t sp, PickNextCall pick_next,
                       void* user) {
  ITC_NATIVE_RUNNING("sub_82116AF0 Career::NextDay");
  const uint32_t frame = sp - kNextDayFrame;
  guest::StoreU32(base, save + 6100, guest::LoadU32(base, save + 6100) + 1);
  const uint32_t garage = save + kGarage;
  for (uint32_t i = 0; i < uint32_t(int32_t(int16_t(guest::LoadU16(base, save + 6328)))); i++) {
    uint32_t node =
        (uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204)))) << 2) + garage;
    for (uint32_t n = i; n != 0; n--) {
      node = (uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) << 2) + garage;
    }
    const uint32_t car =
        uint32_t(int32_t(int16_t(guest::LoadU16(base, node + 2)))) * 1968 + garage + 212;
    guest::StoreU32(base, car + 1896, guest::LoadU32(base, car + 1896) + 1);
  }
  guest::StoreU32(base, save + 0x1ACE0, 0);
  const uint32_t stats = save + kStats;
  StatsCloseDay(base, stats, frame);
  if (!PickNextSkips(base, save)) {
    pick_next(user, save);
  }
  const int32_t day = int32_t(guest::LoadU32(base, save + 6100));
  return StatsStartDay(base, stats, uint32_t(day % 7));
}

}  // namespace career
