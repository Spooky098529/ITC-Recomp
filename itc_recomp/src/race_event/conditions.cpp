// The race event's unlock conditions (docs/functions/race-event.md):
// EventCondition::Met and its 61 kinds of test, EventCondition::State and
// the items' unlock states. Store for store (and load for load where it
// matters) as the recompiled functions; the tests never seen in play are
// ported as read (only kinds 21 and 31 ran), except kind 40, handed back to
// the recompiled code (race_event.h's Guest).

#include "race_event/race_event.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_specs/car_specs.h"
#include "career/career.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "strings_and_font/steps.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstdint>

namespace race_event {

namespace {

int32_t S32(uint32_t v) { return int32_t(v); }
int32_t LoadS32(uint8_t* base, uint32_t a) { return int32_t(guest::LoadU32(base, a)); }
int32_t LoadS16(uint8_t* base, uint32_t a) { return int16_t(guest::LoadU16(base, a)); }
int32_t LoadS8(uint8_t* base, uint32_t a) { return int8_t(guest::LoadU8(base, a)); }

uint32_t Save(uint8_t* base) { return vehicle_accessors::SaveDataOf(base, 0); }
uint32_t Item(uint8_t* base, uint32_t id) { return data_tables::Item(base, id); }
// An item's record's wins (save + 0x199FA + 12 x id).
uint32_t Wins(uint8_t* base, uint32_t save, uint32_t id) {
  return guest::LoadU16(base, save + kItemRecords + 2 + 12 * id);
}

// slw 1, n (n's low 6 bits), sign-extended to 64 bits (extsw).
uint64_t Slw1(uint32_t n) {
  const uint32_t word = (n & 0x20) ? 0 : (1u << (n & 0x1F));
  return uint64_t(int64_t(int32_t(word)));
}
// sld 1, n (n's low 7 bits).
uint64_t Sld1(uint32_t n) { return (n & 0x40) ? 0 : (1ull << (n & 0x3F)); }
// The bits 0 to 47 set.
int32_t Count48(uint64_t mask) {
  int32_t n = 0;
  for (uint32_t i = 0; i < 48; i++) {
    if (mask & (1ull << i)) {
      n++;
    }
  }
  return n;
}

// The arguments (+4 to +16; those below 0 skipped).
template <typename Test>
bool EveryArgument(uint8_t* base, uint32_t condition, Test test) {
  for (uint32_t k = 0; k < 4; k++) {
    const int32_t a = LoadS32(base, condition + 4 + 4 * k);
    if (a >= 0 && !test(a)) {
      return false;
    }
  }
  return true;
}
// `value` at least every argument (signed).
uint32_t AtLeastAll(uint8_t* base, uint32_t condition, int32_t value) {
  return EveryArgument(base, condition, [&](int32_t a) { return value >= a; }) ? 1 : 0;
}
// `value` one of the arguments.
uint32_t OneOf(uint8_t* base, uint32_t condition, int32_t value) {
  for (uint32_t k = 0; k < 4; k++) {
    const int32_t a = LoadS32(base, condition + 4 + 4 * k);
    if (a >= 0 && value == a) {
      return 1;
    }
  }
  return 0;
}

// The garage's selected car (the save data's +0x19920-th node of the list at
// +6120 + 204, followed as the original does): the garage + 1,968 x its car
// (the car record at +212, its id at +224, its tuning at +1860).
uint32_t GarageCar(uint8_t* base, uint32_t save) {
  const uint32_t garage = save + kGarage;
  uint32_t n = guest::LoadU32(base, save + kSelectedCar);
  uint32_t node = uint32_t(LoadS16(base, garage + 204)) * 4 + garage;
  while (n != 0) {
    node = uint32_t(LoadS16(base, node + 0)) * 4 + garage;
    n--;
  }
  return uint32_t(int64_t(LoadS16(base, node + 2)) * 1968) + garage;
}
uint32_t RaceData(uint8_t* base) { return guest::LoadU32(base, kRaceData); }

// The car's spec sheet: the garage car's (Params::GetCarSpec + 640) when
// Params::Exists, else the race data's first racer's (sub_820C4550); 0 for
// none.
uint32_t CarSpec(uint8_t* base) {
  if (car_specs::Exists(base) != 0) {
    const uint32_t car = GarageCar(base, Save(base));
    const uint32_t spec = car_specs::GetCarSpec(base, guest::LoadU32(base, car + 224));
    return spec == 0 ? 0 : spec + 640;
  }
  const uint32_t race = RaceData(base);
  if (race == 0) {
    return 0;
  }
  return vehicle_accessors::ObjectRecord1888(race + 48);
}
// The car's tuning: the garage car's +1860 when Params::Exists, else the
// race data's +1696 (its first racer's car +1648).
uint32_t CarTuning(uint8_t* base) {
  if (car_specs::Exists(base) != 0) {
    return GarageCar(base, Save(base)) + 1860;
  }
  return RaceData(base) + 1696;
}

// Kind 1 to 5: the day (+6100); from day 999 the answer wanted.
uint32_t DayAtLeast(uint8_t* base, uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_82140458 EventCondition::DayAtLeast");
  const int32_t day = LoadS32(base, Save(base) + kDay);
  if (day >= 999) {
    return uint32_t(LoadS16(base, condition + 0));
  }
  return AtLeastAll(base, condition, day);
}

uint32_t Weekday(uint8_t* base, uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_821404D8 EventCondition::Weekday");
  const int32_t day = LoadS32(base, Save(base) + kDay);
  if (day >= 999) {
    return uint32_t(LoadS16(base, condition + 0));
  }
  return OneOf(base, condition, day - (day / 7) * 7);
}

uint32_t DayDivisible(uint8_t* base, uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_8213FE00 EventCondition::DayDivisible");
  const int32_t day = LoadS32(base, Save(base) + kDay);
  if (day >= 999) {
    return uint32_t(LoadS16(base, condition + 0));
  }
  return EveryArgument(base, condition,
                       [&](int32_t a) {
                         if (a == 0) {
                           // The original's tw traps; the translated code logs
                           // it and runs on with the quotient 0.
                           ITC_LOG_WARN("EventCondition::DayDivisible: the division's trap "
                                        "({} / 0), 0 as the original",
                                        day);
                           return day == 0;
                         }
                         return day - (day / a) * a == 0;
                       })
             ? 1
             : 0;
}

uint32_t DayDigit(uint8_t* base, uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_82140568 EventCondition::DayDigit");
  int32_t day = LoadS32(base, Save(base) + kDay);
  if (day >= 999) {
    return uint32_t(LoadS16(base, condition + 0));
  }
  do {
    if (OneOf(base, condition, day - (day / 10) * 10)) {
      return 1;
    }
    day /= 10;
  } while (day >= 1);
  return 0;
}

uint32_t DayDigitSum(uint8_t* base, uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_82140608 EventCondition::DayDigitSum");
  int32_t day = LoadS32(base, Save(base) + kDay);
  if (day >= 999) {
    return uint32_t(LoadS16(base, condition + 0));
  }
  uint32_t sum = 0;
  do {
    sum += uint32_t(day - (day / 10) * 10);
    day /= 10;
  } while (day >= 1);
  return OneOf(base, condition, S32(sum));
}

// Kinds 6, 7, 8, 27, 28, 30: a save data halfword at least every argument.
uint32_t SaveHalfword(uint8_t* base, uint32_t condition, uint32_t offset) {
  return AtLeastAll(base, condition, LoadS16(base, Save(base) + offset));
}
uint32_t Races22(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140718 EventCondition::Races22");
  return SaveHalfword(base, c, 70);
}
uint32_t Races20(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140788 EventCondition::Races20");
  return SaveHalfword(base, c, 68);
}
uint32_t WinsAtLeast(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821407F8 EventCondition::Wins");
  return SaveHalfword(base, c, 60);
}
uint32_t Save6052(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140E58 EventCondition::Save6052");
  return SaveHalfword(base, c, 6052);
}
uint32_t Save6054(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140EC8 EventCondition::Save6054");
  return SaveHalfword(base, c, 6054);
}
uint32_t Save5910(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140F38 EventCondition::Save5910");
  return SaveHalfword(base, c, 5910);
}

// The categories (+34) with every item won (kinds 9 and 19).
uint64_t WonCategories(uint8_t* base, uint32_t save) {
  uint64_t mask = ~0ull;
  for (uint32_t id = 0; id < 400; id++) {
    if (Wins(base, save, id) == 0) {
      mask &= ~Slw1(uint32_t(LoadS8(base, Item(base, id) + 34)));
    }
  }
  return mask;
}

uint32_t WonCategoryCount(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140868 EventCondition::WonCategoryCount");
  return AtLeastAll(base, c, Count48(WonCategories(base, Save(base))));
}

uint32_t Money(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821406A8 EventCondition::Money");
  return AtLeastAll(base, c, S32(guest::LoadU32(base, Save(base) + 6116) * 1000));
}

uint32_t Save6024(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140938 EventCondition::Save6024");
  // The original copies the float to its frame and points FloatAtLeast at
  // the copy.
  return FloatAtLeast(base, c, guest::LoadFloat(base, Save(base) + 6024));
}
uint32_t Save5952(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140980 EventCondition::Save5952");
  // The original copies the float to its frame and points FloatAtLeast at
  // the copy.
  return FloatAtLeast(base, c, guest::LoadFloat(base, Save(base) + 5952));
}
uint32_t Save4908(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140FA8 EventCondition::Save4908");
  // The original copies the float to its frame and points FloatAtLeast at
  // the copy.
  return FloatAtLeast(base, c, guest::LoadFloat(base, Save(base) + 4908));
}

// An item's record's three halfwords summed, to 16 bits.
int32_t RecordSum(uint8_t* base, uint32_t save, uint32_t id) {
  const uint32_t record = uint32_t(uint64_t(id + 8746) * 12) + save;
  return int16_t(uint16_t(guest::LoadU16(base, record + 0) + LoadS16(base, record + 2) +
                          LoadS16(base, record + 4)));
}

uint32_t RacedAll(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_8213FE98 EventCondition::RacedAll");
  const uint32_t save = Save(base);
  return EveryArgument(base, c, [&](int32_t a) { return RecordSum(base, save, a) > 0; }) ? 1 : 0;
}

uint32_t RacedCount(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_8213FF38 EventCondition::RacedCount");
  if (LoadS32(base, c + 4) < 0 || LoadS32(base, c + 8) <= 0) {
    return 1;
  }
  const uint32_t save = Save(base);
  return RecordSum(base, save, guest::LoadU32(base, c + 4)) >= LoadS32(base, c + 8) ? 1 : 0;
}

uint32_t Save6032(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821409C8 EventCondition::Save6032");
  const int32_t value =
      strings_and_font::steps::Fctiwz(double(guest::LoadFloat(base, Save(base) + 6032)));
  return AtLeastAll(base, c, value);
}

uint32_t BestTime(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140A48 EventCondition::BestTime");
  const uint32_t time = ai_drivers::TimeFromTicks(guest::LoadU32(base, Save(base) + 6044));
  const uint64_t minutes = (time >> 16) & 0x7FFF;
  const uint64_t seconds = (time >> 10) & 0x3F;
  return AtLeastAll(base, c, S32(uint32_t(minutes * 60 + seconds)));
}

uint32_t WonAll(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_8213FFC8 EventCondition::WonAll");
  const uint32_t save = Save(base);
  return EveryArgument(base, c, [&](int32_t a) { return Wins(base, save, uint32_t(a)) != 0; })
             ? 1
             : 0;
}

uint32_t WinCount(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140048 EventCondition::WinCount");
  if (LoadS32(base, c + 4) < 0 || LoadS32(base, c + 8) <= 0) {
    return 1;
  }
  const uint32_t save = Save(base);
  const uint32_t id = guest::LoadU32(base, c + 4);
  return LoadS16(base, save + kItemRecords + 2 + 12 * id) >= LoadS32(base, c + 8) ? 1 : 0;
}

uint32_t WonCategoriesTest(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140AC8 EventCondition::WonCategories");
  const uint64_t mask = WonCategories(base, Save(base));
  for (uint32_t k = 0; k < 4; k++) {
    const uint64_t a = uint64_t(int64_t(LoadS32(base, c + 4 + 4 * k)));
    if (a < 48 && (Slw1(uint32_t(a)) & mask) == 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t Won383To391(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140B80 EventCondition::Won383To391");
  // The original lists the ids on its frame first.
  const uint32_t save = Save(base);
  int32_t won = 0;
  for (uint32_t k = 0; k < 9; k++) {
    if (Wins(base, save, 383 + k) != 0) {
      won++;
    }
  }
  return AtLeastAll(base, c, won);
}

uint32_t FullCategories(uint8_t* base, uint32_t c, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_82140C70 EventCondition::FullCategories");
  if (LoadS32(base, c + 4) < 0 || LoadS32(base, c + 8) < 0 || LoadS32(base, c + 12) <= 0) {
    return 1;
  }
  const uint64_t mask = CategoryMask(base, guest::LoadU16(base, level),
                                     guest::LoadU32(base, c + 4), guest::LoadU32(base, c + 8));
  return Count48(mask) >= LoadS32(base, c + 12) ? 1 : 0;
}

uint32_t FullCategoriesAny(uint8_t* base, uint32_t c, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_82140D18 EventCondition::FullCategoriesAny");
  if (LoadS32(base, c + 4) < 0 || LoadS32(base, c + 8) < 0) {
    return 1;
  }
  // The mode mask 7 (on its frame in the original).
  const uint64_t mask =
      CategoryMask(base, guest::LoadU16(base, level), guest::LoadU32(base, c + 4), 7);
  return Count48(mask) >= LoadS32(base, c + 8) ? 1 : 0;
}

uint32_t LocationCleared(uint8_t* base, uint32_t c, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_821400C8 EventCondition::LocationCleared");
  const uint32_t save = Save(base);
  if (LoadS32(base, c + 4) < 0) {
    return 1;
  }
  for (uint32_t id = 0; id < 400; id++) {
    const uint32_t item = Item(base, id);
    const int32_t group = LoadS16(
        base, kLocationGroups + (uint32_t(LoadS8(base, item + 36)) << 1));
    if (LoadS32(base, c + 4) != group) {
      continue;
    }
    if (LoadS16(base, level) < LoadS16(base, item + 42)) {
      continue;
    }
    if (Wins(base, save, id) != 0) {
      continue;
    }
    const int32_t kind = LoadS8(base, item + 35);
    if (kind == 1) {
      continue;
    }
    if (uint32_t(kind - 3) <= 2) {
      return 0;
    }
  }
  return 1;
}

uint32_t RecordWord6(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140188 EventCondition::RecordWord6");
  if (LoadS32(base, c + 4) < 0 || LoadS32(base, c + 8) <= 0) {
    return 1;
  }
  const uint32_t save = Save(base);
  const uint32_t id = guest::LoadU32(base, c + 4);
  return LoadS16(base, save + kItemRecords + 6 + 12 * id) >= LoadS32(base, c + 8) ? 1 : 0;
}

uint32_t Table8D94(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140DB8 EventCondition::Table8D94");
  // The largest of the stats record's three mode counts is worked out and
  // not used; only loads, left out.
  Save(base);
  return OneOf(base, c, LoadS32(base, kModeWords + 12));
}

uint32_t ModeStreak(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140208 EventCondition::ModeStreak");
  const uint32_t event = race_setup::EventCurrent(base);
  if (event == 0) {
    return 1;
  }
  const int32_t mode_word = LoadS32(base, c + 4);
  if (mode_word < 0) {
    return 1;
  }
  const int32_t streak = LoadS32(base, c + 8);
  if (streak <= 0) {
    return 1;
  }
  const uint32_t mode = uint32_t(LoadS16(base, event + 30));
  if (mode_word != LoadS32(base, (mode << 2) + kModeWords)) {
    return 0;
  }
  return LoadS16(base, event + 32) >= streak ? 1 : 0;
}

uint32_t CarIs(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141060 EventCondition::CarIs");
  int32_t id;
  if (car_specs::Exists(base) != 0) {
    id = LoadS32(base, GarageCar(base, Save(base)) + 224);
  } else {
    // The race data's first racer record (+48; sub_82106540 on it does
    // nothing), its +12.
    id = LoadS32(base, RaceData(base) + 48 + 12);
  }
  return OneOf(base, c, id);
}

uint32_t CarSpec129(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141158 EventCondition::CarSpec129");
  const uint32_t spec = CarSpec(base);
  if (spec == 0) {
    return 0;
  }
  return OneOf(base, c, S32(guest::LoadU8(base, spec + 129)));
}

uint32_t Tuning4(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141248 EventCondition::Tuning4");
  const uint32_t t = CarTuning(base);
  return t != 0 && guest::LoadU32(base, t + 4) == 1 ? 1 : 0;
}

uint32_t EngineType7(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821412E8 EventCondition::EngineType7");
  const uint32_t spec = CarSpec(base);
  return spec != 0 && guest::LoadU8(base, spec + 68) == 7 ? 1 : 0;
}

uint32_t CarList1(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821413A8 EventCondition::CarList1");
  const uint32_t spec = CarSpec(base);
  return spec != 0 && LoadS8(base, spec + 131) == 1 ? 1 : 0;
}

uint32_t Nineties(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141468 EventCondition::Nineties");
  const uint32_t spec = CarSpec(base);
  if (spec == 0) {
    return 0;
  }
  const uint32_t year = guest::LoadU32(base, spec + 88);
  return year >= 1990 && year < 2000 ? 1 : 0;
}

uint32_t Spec92(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141528 EventCondition::Spec92");
  const uint32_t spec = CarSpec(base);
  return spec != 0 && guest::LoadU32(base, spec + 92) == 4 && guest::LoadU32(base, spec + 96) == 0
             ? 1
             : 0;
}

uint32_t CarFloat(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821415F0 EventCondition::CarFloat");
  return FloatAtLeast(base, c, guest::LoadFloat(base, GarageCar(base, Save(base)) + 2104));
}

// The tuning words kinds 41 and 42 test.
constexpr uint32_t kParts[] = {8, 16, 24, 28, 32, 36, 40, 48, 56, 60, 68, 76};

uint32_t StockParts(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141738 EventCondition::StockParts");
  const uint32_t t = CarTuning(base);
  if (t == 0) {
    return 0;
  }
  for (uint32_t offset : kParts) {
    if (guest::LoadU32(base, t + offset) != 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t PartsAtLeast(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141858 EventCondition::PartsAtLeast");
  const uint32_t t = CarTuning(base);
  if (t == 0) {
    return 0;
  }
  const int32_t a = LoadS32(base, c + 4);
  if (a < 0) {
    return 0;
  }
  for (uint32_t offset : kParts) {
    if (LoadS32(base, t + offset) < a) {
      return 0;
    }
  }
  return 1;
}

uint32_t TuningAtLeast(uint8_t* base, uint32_t c, uint32_t offset) {
  const uint32_t t = CarTuning(base);
  if (t == 0) {
    return 0;
  }
  return EveryArgument(base, c, [&](int32_t a) { return LoadS32(base, t + offset) >= a; }) ? 1 : 0;
}
uint32_t Tuning60(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141990 EventCondition::Tuning60");
  return TuningAtLeast(base, c, 60);
}
uint32_t Tuning24(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141A60 EventCondition::Tuning24");
  return TuningAtLeast(base, c, 24);
}

uint32_t TuningSet(uint8_t* base, uint32_t offset) {
  const uint32_t t = CarTuning(base);
  return t != 0 && guest::LoadU32(base, t + offset) != 0 ? 1 : 0;
}
uint32_t Tuning128(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141B30 EventCondition::Tuning128");
  return TuningSet(base, 128);
}
uint32_t Tuning148(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141BD0 EventCondition::Tuning148");
  return TuningSet(base, 148);
}
uint32_t Tuning144(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141C70 EventCondition::Tuning144");
  return TuningSet(base, 144);
}
uint32_t Tuning28(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82142070 EventCondition::Tuning28");
  return TuningSet(base, 28);
}

uint32_t AeroFitted(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141D10 EventCondition::AeroFitted");
  const uint32_t t = CarTuning(base);
  if (t == 0) {
    return 0;
  }
  for (uint32_t offset : {96u, 100u, 104u, 108u, 112u, 120u, 124u, 128u, 132u}) {
    if (guest::LoadU32(base, t + offset) != 0) {
      return 1;
    }
  }
  return 0;
}

uint32_t Tuning108(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141E10 EventCondition::Tuning108");
  uint32_t spec = 0;
  uint32_t t;
  if (car_specs::Exists(base) != 0) {
    const uint32_t car = GarageCar(base, Save(base)) + 212;
    const uint32_t sheet = car_specs::GetCarSpec(base, guest::LoadU32(base, car + 12));
    if (sheet != 0) {
      spec = sheet + 640;
    }
    t = car + 1648;
  } else {
    const uint32_t race = RaceData(base);
    if (race == 0) {
      return 0;
    }
    spec = vehicle_accessors::ObjectRecord1888(race + 48);
    t = race + 1696;
  }
  if (spec == 0 || t == 0) {
    return 0;
  }
  const int32_t v = LoadS32(base, t + 108);
  if (guest::LoadU32(base, spec + 120) == 0) {
    return v != 0 ? 1 : 0;
  }
  return v >= 0 ? 1 : 0;
}

uint32_t Tuning64To76(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141F08 EventCondition::Tuning64To76");
  const uint32_t t = CarTuning(base);
  if (t == 0) {
    return 0;
  }
  for (uint32_t offset : {64u, 68u, 72u, 76u}) {
    if (guest::LoadU32(base, t + offset) != 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t Tuning80Not88(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82141FC8 EventCondition::Tuning80Not88");
  const uint32_t t = CarTuning(base);
  return t != 0 && guest::LoadU32(base, t + 80) != guest::LoadU32(base, t + 88) ? 1 : 0;
}

uint32_t Tuning184(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82142110 EventCondition::Tuning184");
  const uint32_t t = CarTuning(base);
  if (t == 0) {
    return 0;
  }
  for (uint32_t k = 0; k < 5; k++) {
    const int32_t v = LoadS32(base, t + 168 + 16 + 12 * k);
    if (v > 0 && v < 5) {
      return 1;
    }
  }
  return 0;
}

uint32_t Decals(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821421D8 EventCondition::Decals");
  const uint32_t at = GarageCar(base, Save(base)) + 308;
  if (at == 0) {
    return 0;
  }
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t p = at + 120 + 32 * k;
    if (LoadS32(base, p - 96) >= 0 || LoadS32(base, p) >= 0) {
      return 1;
    }
  }
  return 0;
}

uint32_t Tuning116(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82142280 EventCondition::Tuning116");
  const uint32_t t = CarTuning(base);
  if (t == 0) {
    return 0;
  }
  const int32_t v = LoadS32(base, t + 116);
  return v == 1 || v == 2 ? 1 : 0;
}

uint32_t Car1588(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82142328 EventCondition::Car1588");
  const uint32_t at = GarageCar(base, Save(base)) + 212 + 1588;
  for (uint32_t k = 0; k < 3; k++) {
    if (guest::LoadU32(base, at + 20 * k) != 0) {
      return 1;
    }
  }
  return 0;
}

uint32_t Sticker101(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821423C0 EventCondition::Sticker101");
  const uint32_t at = GarageCar(base, Save(base)) + 500;
  if (at == 0) {
    return 0;
  }
  for (uint32_t run = 0; run < 8; run++) {
    for (uint32_t k = 0; k < 5; k++) {
      if (LoadS32(base, at + 24 + 160 * run + 32 * k) == 101) {
        return 1;
      }
    }
  }
  return 0;
}

uint32_t CarBytes(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82142558 EventCondition::CarBytes");
  const uint32_t at = GarageCar(base, Save(base)) + 1780;
  const uint32_t value = guest::LoadU32(base, c + 4);
  if (at == 0 || value > 9) {
    return 0;
  }
  const int32_t wanted = LoadS32(base, c + 8);
  if (wanted <= 0) {
    return 0;
  }
  int32_t n = 0;
  for (uint32_t k = 0; k < 7; k++) {
    if (LoadS8(base, at + 5 + k) == S32(value)) {
      n++;
    }
  }
  for (uint32_t k = 0; k < 3; k++) {
    if (LoadS8(base, at + 2 + k) == S32(value)) {
      n++;
    }
  }
  return n >= wanted ? 1 : 0;
}

uint32_t Setting88(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82142648 EventCondition::Setting88");
  const uint32_t at = GarageCar(base, Save(base)) + 228;
  if (at == 0) {
    return 0;
  }
  return LoadS8(base, at + 72) <= -5 && LoadS8(base, at + 73) <= -5 ? 1 : 0;
}

uint32_t GarageCount(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140FF0 EventCondition::GarageCount");
  const uint32_t count = uint32_t(LoadS16(base, Save(base) + 6328));
  return EveryArgument(base, c, [&](int32_t a) { return count >= uint32_t(a); }) ? 1 : 0;
}

uint32_t SameCars(uint8_t* base, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821426D8 EventCondition::SameCars");
  const uint32_t garage = Save(base) + kGarage;
  const int32_t count = LoadS16(base, garage + 208);
  const uint32_t head = uint32_t(LoadS16(base, garage + 204)) * 4 + garage;
  // The n-th car of the list, its id (+224).
  const auto id_at = [&](uint32_t n) {
    uint32_t node = head;
    for (; n != 0; n--) {
      node = uint32_t(LoadS16(base, node + 0)) * 4 + garage;
    }
    const uint32_t car = uint32_t(int64_t(LoadS16(base, node + 2)) * 1968) + garage;
    return LoadS32(base, car + 224);
  };
  int32_t most = 0;
  for (int32_t i = 0; i < count; i++) {
    const int32_t id = id_at(uint32_t(i));
    if (id < 0) {
      continue;
    }
    int32_t same = 0;
    for (int32_t j = 0; j < count; j++) {
      if (id == id_at(uint32_t(j))) {
        same++;
      }
    }
    if (same > most) {
      most = same;
    }
  }
  return AtLeastAll(base, c, most);
}

uint32_t SectionRivalTest(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82140290 EventCondition::SectionRival");
  const uint32_t save = Save(base);
  if (int16_t(career::DayBlockBestStreak(base, save + 72)) < 3) {
    return 0;
  }
  // RaceEvent::Current, read without a check (as the original).
  const uint32_t event = race_setup::EventCurrent(base);
  if (guest::LoadU32(base, event + 12) == 0 || guest::LoadU32(base, event + 16) == 0) {
    if (guest::LoadU32(base, event + 4) == 0 || guest::LoadU32(base, event + 8) == 0) {
      return 0;
    }
  }
  return 1;
}

}  // namespace

uint32_t FloatAtLeast(uint8_t* base, uint32_t condition, float value) {
  ITC_NATIVE_RUNNING("sub_82140400 EventCondition::FloatAtLeast");
  for (uint32_t k = 0; k < 4; k++) {
    const int32_t a = LoadS32(base, condition + 4 + 4 * k);
    if (a < 0) {
      continue;
    }
    if (double(value) < double(float(double(int64_t(a))))) {
      return 0;
    }
  }
  return 1;
}

uint64_t CategoryMask(uint8_t* base, uint32_t level, uint32_t location, uint32_t modes) {
  ITC_NATIVE_RUNNING("sub_82140340 EventCondition::CategoryMask");
  const uint32_t save = Save(base);
  const int32_t group = LoadS16(base, (location << 1) + kLocationGroups);
  uint64_t mask = ~0ull;
  for (uint32_t id = 0; id < 400; id++) {
    const uint32_t item = Item(base, id);
    const bool keep =
        uint16_t(level) == guest::LoadU16(base, item + 42) &&
        LoadS16(base, (uint32_t(LoadS8(base, item + 36)) << 1) + kLocationGroups) == group &&
        (uint32_t(LoadS16(base, item + 40)) & modes) != 0 && Wins(base, save, id) != 0;
    if (!keep) {
      mask &= ~Sld1(guest::LoadU8(base, item + 34));
    }
  }
  return mask;
}

uint32_t ConditionMet(uint8_t* base, uint32_t condition, uint32_t level, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142820 EventCondition::Met");
  const uint32_t kind = uint32_t(LoadS16(base, condition + 2));
  if (kind > 67 || kind == 0) {
    return 1;
  }
  if (kind == 67) {
    return 0;
  }
  // The tests' stores to their own frames (copies only they read) are left
  // out.
  uint32_t r3 = 0;
  switch (kind) {
    case 1:  // sub_82142868, the recompiler's split of this case: merged here
      r3 = DayAtLeast(base, condition);
      break;
    case 2: r3 = Weekday(base, condition); break;
    case 3: r3 = DayDivisible(base, condition); break;
    case 4: r3 = DayDigit(base, condition); break;
    case 5: r3 = DayDigitSum(base, condition); break;
    case 6: r3 = Races22(base, condition); break;
    case 7: r3 = Races20(base, condition); break;
    case 8: r3 = WinsAtLeast(base, condition); break;
    case 9: r3 = WonCategoryCount(base, condition); break;
    case 10: r3 = Money(base, condition); break;
    case 11: r3 = Save6024(base, condition); break;
    case 12: r3 = Save5952(base, condition); break;
    case 13: r3 = RacedAll(base, condition); break;
    case 14: r3 = RacedCount(base, condition); break;
    case 15: r3 = Save6032(base, condition); break;
    case 16: r3 = BestTime(base, condition); break;
    case 17: r3 = WonAll(base, condition); break;
    case 18: r3 = WinCount(base, condition); break;
    case 19: r3 = WonCategoriesTest(base, condition); break;
    case 20: r3 = Won383To391(base, condition); break;
    case 21: r3 = FullCategories(base, condition, level); break;
    case 22: r3 = FullCategoriesAny(base, condition, level); break;
    case 23: r3 = LocationCleared(base, condition, level); break;
    case 24: r3 = RecordWord6(base, condition); break;
    case 25: r3 = Table8D94(base, condition); break;
    case 26: r3 = ModeStreak(base, condition); break;
    case 27: r3 = Save6052(base, condition); break;
    case 28: r3 = Save6054(base, condition); break;
    case 29: {
      const uint32_t event = race_setup::EventCurrent(base);
      r3 = event != 0 && LoadS16(base, event + 34) == 6 ? 1 : 0;
      break;
    }
    case 30: r3 = Save5910(base, condition); break;
    case 31: r3 = CarIs(base, condition); break;
    case 32: r3 = CarSpec129(base, condition); break;
    case 33: r3 = Tuning4(base); break;
    case 34: r3 = EngineType7(base); break;
    case 35: r3 = CarList1(base); break;
    case 36: r3 = Nineties(base); break;
    case 37: r3 = Spec92(base); break;
    case 38: r3 = CarFloat(base, condition); break;
    case 39: r3 = Save4908(base, condition); break;
    case 40:  // EventCondition::Power: the recompiled test (Guest)
      r3 = guest.test(guest.user, 0x82141668, condition, level, guest.depth + 96);
      break;
    case 41: r3 = StockParts(base); break;
    case 42: r3 = PartsAtLeast(base, condition); break;
    case 43: r3 = Tuning60(base, condition); break;
    case 44: r3 = Tuning24(base, condition); break;
    case 45: r3 = Tuning128(base); break;
    case 46: r3 = Tuning148(base); break;
    case 47: r3 = Tuning144(base); break;
    case 48: r3 = AeroFitted(base); break;
    case 49: r3 = Tuning108(base); break;
    case 50: r3 = Tuning64To76(base); break;
    case 51: r3 = Tuning80Not88(base); break;
    case 52: r3 = Tuning28(base); break;
    case 53: r3 = Tuning184(base); break;
    case 54: r3 = Decals(base); break;
    case 55: r3 = Tuning116(base); break;
    case 56: r3 = Car1588(base); break;
    case 57: r3 = Sticker101(base); break;
    case 58:  // always met (SaveData::Of called, its answer not used)
      r3 = uint32_t(LoadS16(base, condition + 0));
      break;
    case 59: r3 = CarBytes(base, condition); break;
    case 60: r3 = Setting88(base); break;
    case 61: r3 = 1; break;
    case 62: r3 = GarageCount(base, condition); break;
    case 63: r3 = SameCars(base, condition); break;
    case 64:
      r3 = (guest::LoadU32(base, Save(base) + 0x1AD10) & 0x20) != 0 ? 1 : 0;
      break;
    case 65: r3 = SectionRivalTest(base); break;
    case 66: r3 = LoadS8(base, Save(base) + 0x1F878) > 0 ? 0 : 1; break;
    default: break;
  }
  return r3 - uint32_t(LoadS16(base, condition + 0)) == 0 ? 1 : 0;
}

uint32_t ConditionState(uint8_t* base, uint32_t condition, uint32_t level, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142C00 EventCondition::State");
  if (ConditionMet(base, condition, level, guest.Down(96)) != 0) {
    return 1;
  }
  const int32_t kind = LoadS16(base, condition + 2);
  if (kind < 6) {
    return 0;
  }
  if (kind <= 21) {
    return 2;
  }
  if (kind <= 22) {
    return 0;
  }
  if (kind <= 24 || kind == 65) {
    return 2;
  }
  return kind == 67 ? 1 : 0;
}

uint32_t UnlockAll(uint8_t* base, uint32_t block, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142C80 Unlock::All");
  uint32_t r = 1;
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t s = ConditionState(base, block + 8 + 20 * k, block + 2, guest.Down(128));
    if (s == 0) {
      return 0;
    }
    if (s == 2) {
      r = 2;
    }
  }
  return r;
}

uint32_t UnlockAny(uint8_t* base, uint32_t block, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142CE0 Unlock::Any");
  uint32_t r = 0;
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t s = ConditionState(base, block + 8 + 20 * k, block + 2, guest.Down(128));
    if (s == 1) {
      return 1;
    }
    if (s == 2) {
      r = 2;
    }
  }
  return r;
}

namespace {

// All or any by the block's +7.
uint32_t AllOrAny(uint8_t* base, uint32_t item, const Guest& guest) {
  const uint32_t block = item + 40;
  return guest::LoadU8(base, block + 7) == 0 ? UnlockAll(base, block, guest)
                                             : UnlockAny(base, block, guest);
}

}  // namespace

uint32_t UnlockStateKind2(uint8_t* base, uint32_t item, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142D40 Unlock::StateKind2");
  const uint32_t save = Save(base);
  for (uint32_t id = 0; id < 400; id++) {
    const uint32_t other = Item(base, id);
    if (guest::LoadU8(base, item + 34) != guest::LoadU8(base, other + 34) ||
        guest::LoadU8(base, other + 35) != 0 || Wins(base, save, id) != 0) {
      continue;
    }
    bool rival = false;
    for (uint32_t k = 0; k < 3; k++) {
      if (LoadS16(base, item + 92 + 2 * k) == S32(id)) {
        rival = true;
        break;
      }
    }
    if (rival) {
      continue;
    }
    const uint32_t r = AllOrAny(base, item, guest.Down(112));
    return r == 1 ? 2 : r;
  }
  return AllOrAny(base, item, guest.Down(112));
}

uint32_t UnlockStateKind5(uint8_t* base, uint32_t item, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142E18 Unlock::StateKind5");
  const uint32_t save = Save(base);
  for (uint32_t id = 0; id < 400; id++) {
    if (Wins(base, save, id) != 0) {
      continue;
    }
    if (guest::LoadU8(base, Item(base, id) + 35) != 5) {
      const uint32_t r = AllOrAny(base, item, guest.Down(112));
      return r == 1 ? 2 : r;
    }
  }
  return AllOrAny(base, item, guest.Down(112));
}

uint32_t UnlockState(uint8_t* base, uint32_t item, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142EC0 Unlock::State");
  const int32_t kind = LoadS8(base, item + 35);
  if (kind == 2) {
    return UnlockStateKind2(base, item, guest.Down(96));
  }
  if (kind == 5) {
    return UnlockStateKind5(base, item, guest.Down(96));
  }
  return AllOrAny(base, item, guest.Down(96));
}

uint32_t HasSectionCondition(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82140308 Unlock::HasSectionCondition");
  for (uint32_t k = 0; k < 2; k++) {
    if (guest::LoadU16(base, block + 10 + 20 * k) == 65) {
      return 1;
    }
  }
  return 0;
}

uint32_t ItemsAvailable(uint8_t* base, uint32_t id, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_821435C8 Items::Available");
  const uint32_t item = Item(base, id);
  if (guest::LoadU8(base, item + 46) == 2) {
    return 0;
  }
  return ConditionMet(base, item + 100, item + 42, guest.Down(96));
}

}  // namespace race_event
