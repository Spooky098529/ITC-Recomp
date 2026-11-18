// A race's result in the stats record and the items' records, and the next
// pick's gate (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "schema/career.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace career {

using schema::CareerCounter;
using schema::CareerGlobals;
using schema::CareerSave;
using schema::CareerStats;

namespace {

constexpr uint32_t kRaceMode = 0xA3AF4;       // the race data's mode
constexpr uint32_t kRaceResults = 0x59AE8;    // its result records
constexpr uint32_t kRaceStages = 0x59620;     // its stage list

double Lfs(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}
void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

// A float field kept at its largest: the new value when the old one is
// below it (fcmpu and blt, NaNs keep the old one).
void KeepLargest(uint8_t* base, uint32_t address, double value) {
  const double old = Lfs(base, address);
  Stfs(base, address, old < value ? value : old);
}

void CountUp(uint8_t* base, uint32_t address) {
  const int32_t value = int16_t(guest::LoadU16(base, address));
  if (value < 9999) {
    guest::StoreU16(base, address, uint16_t(value + 1));
  }
}

// An integer as the original converts it: its 64 bits stored at frame +88
// (std), loaded as a double (lfd, fcfid) and rounded to a single.
double IntToSingle(uint8_t* base, uint32_t frame, int32_t value) {
  StoreU64(base, frame + 88, uint64_t(int64_t(value)));
  return double(float(double(int64_t(value))));
}

// The frame's running mean at +80 ({0.0, 0}) with one value added, merged
// into `into` (RunningMean::Add's and RunningMean::Merge's plain C++).
void MergeOne(uint8_t* base, uint32_t frame, uint32_t into, double value) {
  guest::StoreFloat(base, frame + 80, CareerGlobals::zero(base));
  guest::StoreU32(base, frame + 84, 0);
  race_setup::RunningMeanAdd(base, frame + 80, value, 1);
  RunningMeanMerge(base, into, LoadU64(base, frame + 80), frame);
}

}  // namespace

uint32_t RunningMeanMergeTwo(uint8_t* base, uint32_t stats, uint64_t packed, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82112148 RunningMean::MergeTwo");
  const uint32_t frame = sp - kMergeTwoFrame;
  const CareerStats s(base, stats);
  RunningMeanMerge(base, CareerCounter(base, s.week_address()).week_mean_796_address(), packed,
                   frame);
  RunningMeanMerge(base, s.mean_796_address(), packed, frame);
  return s.mean_796_address();
}

bool AddRaceResultNative(uint8_t* base, uint32_t save, uint32_t race, uint32_t sp) {
  const uint32_t frame = sp - kAddRaceResultFrame;
  if (frame >= 0xA0000000u) {
    return false;
  }
  if (guest::LoadU32(base, race + kRaceMode) - 2 > 1) {
    return true;
  }
  if (!ResultsPushNative(CareerStats(base, CareerSave(base, save).stats_address())
                             .results_address())) {
    return false;
  }
  const uint32_t result = vehicle_accessors::RacerRecordsAt(base, race + kRaceResults, 0xFFFFFFFFu);
  if (int32_t(guest::LoadU32(base, result + 3924)) != 1) {
    return true;
  }
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t id = guest::LoadU32(base, result + 8 + 972 * k);
    if (int32_t(id) != -1 && !CanCopy(frame + 96, data_tables::Item(base, id), 3008)) {
      return false;
    }
  }
  return true;
}

uint32_t StatsAddRaceResult(uint8_t* base, uint32_t save, uint32_t race, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82115190 Stats::AddRaceResult");
  const uint32_t frame = sp - kAddRaceResultFrame;
  const uint32_t result = vehicle_accessors::RacerRecordsAt(base, race + kRaceResults, 0xFFFFFFFFu);
  uint32_t r3 = result;
  const CareerSave sv(base, save);
  const uint32_t stats = sv.stats_address();
  const CareerStats st(base, stats);
  const CareerCounter week(base, st.week_address());
  const double zero = double(CareerGlobals::zero(base));
  if (guest::LoadU32(base, race + kRaceMode) - 2 <= 1) {
    week.set_week_808(guest::LoadU32(base, result + 808));
    week.set_week_sum_812(week.week_sum_812() + guest::LoadU32(base, result + 812));
    week.set_week_sum_816(week.week_sum_816() + guest::LoadU32(base, result + 816));
    const double best = Lfs(base, result + 804);
    KeepLargest(base, week.week_best_804_address(), best);
    KeepLargest(base, st.best_804_address(), best);
    RunningMeanMergeTwo(base, stats, LoadU64(base, result + 796), frame);
    MergeOne(base, frame, st.mean_812_address(),
             IntToSingle(base, frame, int32_t(guest::LoadU32(base, result + 812))));
    MergeOne(base, frame, st.mean_816_address(),
             IntToSingle(base, frame, int32_t(guest::LoadU32(base, result + 816))));
    MergeOne(base, frame, st.mean_820_address(), Lfs(base, result + 820));
    st.set_sum_820(float(double(float(Lfs(base, result + 820) + double(st.sum_820())))));
    MergeOne(base, frame, st.mean_won_address(),
             guest::LoadU32(base, result + 784) != 0 ? double(CareerGlobals::hundred(base))
                                                     : zero);
    const uint32_t most = guest::LoadU32(base, result + 16);
    const uint32_t old = st.best_16();
    st.set_best_16(old < most ? most : old);
    ResultsPush(base, st.results_address(), race);
    const uint32_t type = vehicle_accessors::StagesType(base, race + kRaceStages, 0xFFFFFFFFu);
    r3 = type;
    if (type == 0) {
      double value = zero;
      if (int32_t(guest::LoadU32(base, result + 3920)) != 3) {
        const double count = IntToSingle(base, frame, int32_t(guest::LoadU32(base, result + 788)));
        value = double(float(count * double(CareerGlobals::scale_788(base))));
      }
      MergeOne(base, frame, st.mean_788_address(), value);
      r3 = st.mean_788_address();
      const double scale = double(CareerGlobals::scale_828(base));
      KeepLargest(base, st.best_828_address(), double(float(Lfs(base, result + 828) * scale)));
      KeepLargest(base, st.best_828_2_address(), double(float(Lfs(base, result + 828) * scale)));
    }
  }
  if (guest::LoadU32(base, race + kRaceMode) - 2 <= 1) {
    for (uint32_t k = 0; k < 4; k++) {
      const uint32_t id = guest::LoadU32(base, result + 8 + 972 * k);
      if (int32_t(id) == -1) {
        continue;
      }
      const int32_t outcome = int32_t(guest::LoadU32(base, result + 3924));
      if (outcome == 1) {
        ItemRecordsAddWin(base, save, id, frame);
        c_runtime::Memcpy(base, frame + 96, data_tables::Item(base, id), 3008);
        r3 = frame + 96;
        if (guest::LoadU8(base, frame + 96 + 35) == 5) {
          sv.set_kind5_won(1);
        }
      } else if (outcome == 3) {
        r3 = ItemRecordsAddOutcome3(base, save, id, frame);
      } else {
        r3 = ItemRecordsAddLoss(base, save, id, frame);
        if (int32_t(guest::LoadU32(base, result + 3920)) == 3) {
          const uint32_t mode = sv.mode();
          CountUp(base, CareerCounter(base, st.counter_address()).unknown_18_address());
          CountUp(base, st.by_mode_address() + 14 * mode + 8);
        }
      }
    }
  }
  return r3;
}

bool PickNextSkips(uint8_t* base, uint32_t save) {
  ITC_NATIVE_RUNNING("sub_821159B0 Career::PickNext (its gate)");
  const CareerSave sv(base, save);
  if (int16_t(CareerCounter(base, sv.stats_address()).races()) < 15) {
    return true;
  }
  const int32_t day = int32_t(sv.day());
  return day <= 999 && day < 3;
}

}  // namespace career
