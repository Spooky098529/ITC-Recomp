// A race's result in the stats record and the items' records, and the next
// pick's gate (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace career {

namespace {

constexpr uint32_t kHundred = 0x82002A0C;     // 100.0
constexpr uint32_t kScale788 = 0x820082A0;    // not read
constexpr uint32_t kScale828 = 0x820029D8;    // not read
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
  guest::StoreFloat(base, frame + 80, guest::LoadFloat(base, kZeroFloat));
  guest::StoreU32(base, frame + 84, 0);
  race_setup::RunningMeanAdd(base, frame + 80, value, 1);
  RunningMeanMerge(base, into, LoadU64(base, frame + 80), frame);
}

}  // namespace

uint32_t RunningMeanMergeTwo(uint8_t* base, uint32_t stats, uint64_t packed, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82112148 RunningMean::MergeTwo");
  const uint32_t frame = sp - kMergeTwoFrame;
  RunningMeanMerge(base, stats + 4892, packed, frame);
  RunningMeanMerge(base, stats + 5904, packed, frame);
  return stats + 5904;
}

bool AddRaceResultNative(uint8_t* base, uint32_t save, uint32_t race, uint32_t sp) {
  const uint32_t frame = sp - kAddRaceResultFrame;
  if (frame >= 0xA0000000u) {
    return false;
  }
  if (guest::LoadU32(base, race + kRaceMode) - 2 > 1) {
    return true;
  }
  if (!ResultsPushNative(save + 4996)) {
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
  const uint32_t stats = save + kStats;
  const double zero = Lfs(base, kZeroFloat);
  if (guest::LoadU32(base, race + kRaceMode) - 2 <= 1) {
    guest::StoreU32(base, save + 4948, guest::LoadU32(base, result + 808));
    guest::StoreU32(base, save + 4960,
                    guest::LoadU32(base, save + 4960) + guest::LoadU32(base, result + 812));
    guest::StoreU32(base, save + 4956,
                    guest::LoadU32(base, save + 4956) + guest::LoadU32(base, result + 816));
    const double best = Lfs(base, result + 804);
    KeepLargest(base, stats + 4888, best);
    KeepLargest(base, stats + 5976, best);
    RunningMeanMergeTwo(base, stats, LoadU64(base, result + 796), frame);
    MergeOne(base, frame, save + 5928,
             IntToSingle(base, frame, int32_t(guest::LoadU32(base, result + 812))));
    MergeOne(base, frame, save + 5936,
             IntToSingle(base, frame, int32_t(guest::LoadU32(base, result + 816))));
    MergeOne(base, frame, save + 5960, Lfs(base, result + 820));
    Stfs(base, stats + 5988, double(float(Lfs(base, result + 820) + Lfs(base, stats + 5988))));
    MergeOne(base, frame, save + 5968,
             guest::LoadU32(base, result + 784) != 0 ? Lfs(base, kHundred) : zero);
    const uint32_t most = guest::LoadU32(base, result + 16);
    const uint32_t old = guest::LoadU32(base, stats + 5996);
    guest::StoreU32(base, stats + 5996, old < most ? most : old);
    ResultsPush(base, save + 4996, race);
    const uint32_t type = vehicle_accessors::StagesType(base, race + kRaceStages, 0xFFFFFFFFu);
    r3 = type;
    if (type == 0) {
      double value = zero;
      if (int32_t(guest::LoadU32(base, result + 3920)) != 3) {
        const double count = IntToSingle(base, frame, int32_t(guest::LoadU32(base, result + 788)));
        value = double(float(count * Lfs(base, kScale788)));
      }
      MergeOne(base, frame, save + 5944, value);
      r3 = save + 5944;
      const double scale = Lfs(base, kScale828);
      KeepLargest(base, stats + 5980, double(float(Lfs(base, result + 828) * scale)));
      KeepLargest(base, stats + 5984, double(float(Lfs(base, result + 828) * scale)));
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
          guest::StoreU32(base, save + 6048, 1);
        }
      } else if (outcome == 3) {
        r3 = ItemRecordsAddOutcome3(base, save, id, frame);
      } else {
        r3 = ItemRecordsAddLoss(base, save, id, frame);
        if (int32_t(guest::LoadU32(base, result + 3920)) == 3) {
          const uint32_t mode = guest::LoadU32(base, save + 6104);
          CountUp(base, stats + 18);
          CountUp(base, stats + 14 * mode + 5796 + 8);
        }
      }
    }
  }
  return r3;
}

bool PickNextSkips(uint8_t* base, uint32_t save) {
  ITC_NATIVE_RUNNING("sub_821159B0 Career::PickNext (its gate)");
  if (int16_t(guest::LoadU16(base, save + 70)) < 15) {
    return true;
  }
  const int32_t day = int32_t(guest::LoadU32(base, save + 6100));
  return day <= 999 && day < 3;
}

}  // namespace career
