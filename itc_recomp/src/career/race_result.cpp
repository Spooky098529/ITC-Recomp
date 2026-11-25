// A race's result in the stats record and the items' records, and the next
// pick's gate (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "schema/career.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/race_states.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cstring>

namespace career {

using schema::CareerCounter;
using schema::CareerGlobals;
using schema::CareerSave;
using schema::CareerStats;
using schema::OpponentItem;
using schema::RaceData;
using schema::RacerResultsRecord;

namespace {

double Lfs(uint32_t address) { return double(guest::LoadFloat(address)); }
void Stfs(uint32_t address, double value) {
  guest::StoreFloat(address, float(value));
}

uint64_t LoadU64(uint32_t address) {
  return (uint64_t(guest::LoadU32(address)) << 32) | guest::LoadU32(address + 4);
}

// A float field kept at its largest: the new value when the old one is
// below it (fcmpu and blt, NaNs keep the old one).
void KeepLargest(uint32_t address, double value) {
  const double old = Lfs(address);
  Stfs(address, old < value ? value : old);
}

void CountUp(uint32_t address) {
  const int32_t value = int16_t(guest::LoadU16(address));
  if (value < 9999) {
    guest::StoreU16(address, uint16_t(value + 1));
  }
}

// An integer as the original converts it (std, lfd, fcfid, frsp; the
// doubleword went through the frame's +88, a value since E5 step 6).
double IntToSingle(int32_t value) { return double(float(double(int64_t(value)))); }

// The running mean {0.0, 0} (the original's frame +80, a value since E5
// step 6) with one value added (RunningMean::Add's arithmetic), merged into
// `into` (RunningMean::Merge's).
void MergeOne(uint32_t into, double value) {
  const float mean = race_setup::MeanAfterAdd(CareerGlobals::zero(), 0, value, 1);
  RunningMeanMergeInto(into, (uint64_t(std::bit_cast<uint32_t>(mean)) << 32) | 1);
}

}  // namespace

uint32_t RunningMeanMergeTwo(uint32_t stats, uint64_t packed, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82112148 RunningMean::MergeTwo");
  // The original's 112-byte frame held nothing but RunningMean::Merge's
  // stores into it (E5 step 6: left out).
  (void)sp;
  const CareerStats s(stats);
  RunningMeanMergeInto(CareerCounter(s.week_address()).week_mean_796_address(),
                       packed);
  RunningMeanMergeInto(s.mean_796_address(), packed);
  return s.mean_796_address();
}

bool AddRaceResultNative(uint32_t save, uint32_t race, uint32_t sp) {
  // The items' copies onto the frame are gone (E5 step 6): only
  // Results::Push's copies are checked.
  (void)sp;
  const RaceData rd(race);
  if (rd.race_mode() - 2 > 1) {
    return true;
  }
  return ResultsPushNative(
      CareerStats(CareerSave(save).stats_address()).results_address());
}

uint32_t StatsAddRaceResult(uint32_t save, uint32_t race, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82115190 Stats::AddRaceResult");
  // The original's 3,184-byte frame (E5 step 6): its running means and
  // conversions are values, the item copy at +96 read in place; only its
  // address is kept, as the r3 the original leaves.
  const uint32_t frame = sp - kAddRaceResultFrame;
  const RaceData rd(race);
  const uint32_t result = vehicle_accessors::RacerRecordsAt(rd.results_address(), 0xFFFFFFFFu);
  const RacerResultsRecord rr(result);
  uint32_t r3 = result;
  const CareerSave sv(save);
  const uint32_t stats = sv.stats_address();
  const CareerStats st(stats);
  const CareerCounter week(st.week_address());
  const double zero = double(CareerGlobals::zero());
  if (rd.race_mode() - 2 <= 1) {
    week.set_week_808(rr.e0_hard_hit());
    week.set_week_sum_812(week.week_sum_812() + rr.e0_track_hits());
    week.set_week_sum_816(week.week_sum_816() + rr.e0_car_hits());
    const double best = double(rr.e0_top_speed());
    KeepLargest(week.week_best_804_address(), best);
    KeepLargest(st.best_804_address(), best);
    RunningMeanMergeTwo(stats, LoadU64(rr.e0_mean_speed_pair_address()), frame);
    MergeOne(st.mean_812_address(), IntToSingle(int32_t(rr.e0_track_hits())));
    MergeOne(st.mean_816_address(), IntToSingle(int32_t(rr.e0_car_hits())));
    MergeOne(st.mean_820_address(), double(rr.e0_distance()));
    st.set_sum_820(float(double(float(double(rr.e0_distance()) + double(st.sum_820())))));
    MergeOne(st.mean_won_address(),
             rr.e0_flag_ticks() != 0 ? double(CareerGlobals::hundred())
                                                     : zero);
    const uint32_t most = rr.e0_ticks();
    const uint32_t old = st.best_16();
    st.set_best_16(old < most ? most : old);
    ResultsPush(st.results_address(), race);
    const uint32_t type = vehicle_accessors::StagesType(rd.stages_address(), 0xFFFFFFFFu);
    r3 = type;
    if (type == 0) {
      double value = zero;
      if (int32_t(rr.outcome()) != 3) {
        const double count = IntToSingle(int32_t(rr.e0_sp()));
        value = double(float(count * double(CareerGlobals::scale_788())));
      }
      MergeOne(st.mean_788_address(), value);
      r3 = st.mean_788_address();
      const double scale = double(CareerGlobals::scale_828());
      KeepLargest(st.best_828_address(), double(float(double(rr.e0_route_distance()) * scale)));
      KeepLargest(st.best_828_2_address(), double(float(double(rr.e0_route_distance()) * scale)));
    }
  }
  if (rd.race_mode() - 2 <= 1) {
    for (uint32_t k = 0; k < 4; k++) {
      const uint32_t id = guest::LoadU32(rr.e0_item_address() + 972 * k);
      if (int32_t(id) == -1) {
        continue;
      }
      const int32_t outcome = int32_t(rr.finish());
      if (outcome == 1) {
        ItemRecordsAddWin(save, id, frame);
        // The original copies the item onto its frame (+96) and reads its
        // kind there; read in place since E5 step 6 (r3 still the copy's
        // address, as the original leaves it).
        r3 = frame + 96;
        if (OpponentItem(data_tables::Item(id)).kind() == 5) {
          sv.set_kind5_won(1);
        }
      } else if (outcome == 3) {
        r3 = ItemRecordsAddOutcome3(save, id, frame);
      } else {
        r3 = ItemRecordsAddLoss(save, id, frame);
        if (int32_t(rr.outcome()) == 3) {
          const uint32_t mode = sv.mode();
          CountUp(CareerCounter(st.counter_address()).unknown_18_address());
          CountUp(st.by_mode_address() + 14 * mode + 8);
        }
      }
    }
  }
  return r3;
}

bool PickNextSkips(uint32_t save) {
  ITC_NATIVE_RUNNING("sub_821159B0 Career::PickNext (its gate)");
  const CareerSave sv(save);
  if (int16_t(CareerCounter(sv.stats_address()).races()) < 15) {
    return true;
  }
  const int32_t day = int32_t(sv.day());
  return day <= 999 && day < 3;
}

}  // namespace career
