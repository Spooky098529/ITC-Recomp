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
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "strings_and_font/steps.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstdint>

namespace race_event {

namespace {

using G = schema::RaceEventGlobals;
using Cond = schema::EventCondition;
using ItemV = schema::OpponentItem;
using SaveV = schema::SaveData;
using Spec = schema::CarSpecCopies;
using Event = schema::RaceEvent;
using Record = schema::CareerItemRecord;
using GarageList = schema::CareerGarageList;
using CarV = schema::CarMakersCar;
using GarageCarV = schema::GarageCar;
using Decal = schema::CarMakersCarDecal;
using Tuning = schema::CarTuning;
// A tuning word's getter (the tables kinds 41, 42 and the aero test walk).
using TuningWord = uint32_t (Tuning::*)() const;

int32_t S32(uint32_t v) { return int32_t(v); }
int32_t LoadS32(uint32_t a) { return int32_t(guest::LoadU32(a)); }
int32_t LoadS16(uint32_t a) { return int16_t(guest::LoadU16(a)); }
int32_t LoadS8(uint32_t a) { return int8_t(guest::LoadU8(a)); }

uint32_t Save() { return vehicle_accessors::SaveDataOf(0); }
uint32_t Item(uint32_t id) { return data_tables::Item(id); }
// An item's record (save + 0x199F8 + 12 x id).
Record ItemRecord(uint32_t save, uint32_t id) {
  return Record(SaveV(save).item_records_address() + Record::kSize * id);
}
// An item's record's wins (save + 0x199FA + 12 x id).
uint32_t Wins(uint32_t save, uint32_t id) {
  return ItemRecord(save, id).wins();
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
bool EveryArgument(uint32_t condition, Test test) {
  for (uint32_t k = 0; k < 4; k++) {
    const int32_t a = LoadS32(Cond(condition).arg0_address() + 4 * k);
    if (a >= 0 && !test(a)) {
      return false;
    }
  }
  return true;
}
// `value` at least every argument (signed).
uint32_t AtLeastAll(uint32_t condition, int32_t value) {
  return EveryArgument(condition, [&](int32_t a) { return value >= a; }) ? 1 : 0;
}
// `value` one of the arguments.
uint32_t OneOf(uint32_t condition, int32_t value) {
  for (uint32_t k = 0; k < 4; k++) {
    const int32_t a = LoadS32(Cond(condition).arg0_address() + 4 * k);
    if (a >= 0 && value == a) {
      return 1;
    }
  }
  return 0;
}

// A garage list node's address (4-byte nodes {+0 the next, +2 the car's
// slot}; CareerGarageList's nodes) and its car (a GarageCar, 1,968 bytes
// apart from the list's +212).
uint32_t GarageNode(const GarageList& list, int32_t index) {
  return uint32_t(index) * 4 + list.nodes_address();
}
uint32_t GarageSlotCar(const GarageList& list, uint32_t node) {
  return uint32_t(int64_t(LoadS16(node + 2)) * GarageCarV::kSize) + list.cars_address();
}
// The garage's selected car (the save data's +0x19920-th node of the list at
// +6120 + 204, followed as the original does): a GarageCar (a car object's
// head: its id at +12, its tuning at +1648).
uint32_t GarageCar(uint32_t save) {
  const SaveV sv(save);
  const GarageList list(sv.garage_address());
  uint32_t n = sv.selected_car();
  uint32_t node = GarageNode(list, int16_t(sv.garage_head()));
  while (n != 0) {
    node = GarageNode(list, LoadS16(node + 0));
    n--;
  }
  return GarageSlotCar(list, node);
}
// A car object's tuning (+1648, a CarTuning; CarMakersCar names no field there).
constexpr uint32_t kCarTuning = 1648;
uint32_t RaceData() { return G::race_data().address; }

// The car's spec sheet: the garage car's (Params::GetCarSpec + 640) when
// Params::Exists, else the race data's first racer's (sub_820C4550); 0 for
// none.
uint32_t CarSpec() {
  if (car_specs::Exists() != 0) {
    const CarV car(GarageCar(Save()));
    const uint32_t spec = car_specs::GetCarSpec(car.car_id());
    return spec == 0 ? 0 : spec + 640;
  }
  const uint32_t race = RaceData();
  if (race == 0) {
    return 0;
  }
  return vehicle_accessors::ObjectRecord1888(race + 48);
}
// The car's tuning: the garage car's +1648 when Params::Exists, else the
// race data's +1696 (its first racer's car +1648).
uint32_t CarTuning() {
  if (car_specs::Exists() != 0) {
    return GarageCar(Save()) + kCarTuning;
  }
  return RaceData() + 1696;
}

// Kind 1 to 5: the day (+6100); from day 999 the answer wanted.
uint32_t DayAtLeast(uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_82140458 EventCondition::DayAtLeast");
  const int32_t day = int32_t(SaveV(Save()).day());
  if (day >= 999) {
    return uint32_t(Cond(condition).answer());
  }
  return AtLeastAll(condition, day);
}

uint32_t Weekday(uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_821404D8 EventCondition::Weekday");
  const int32_t day = int32_t(SaveV(Save()).day());
  if (day >= 999) {
    return uint32_t(Cond(condition).answer());
  }
  return OneOf(condition, day - (day / 7) * 7);
}

uint32_t DayDivisible(uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_8213FE00 EventCondition::DayDivisible");
  const int32_t day = int32_t(SaveV(Save()).day());
  if (day >= 999) {
    return uint32_t(Cond(condition).answer());
  }
  return EveryArgument(condition,
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

uint32_t DayDigit(uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_82140568 EventCondition::DayDigit");
  int32_t day = int32_t(SaveV(Save()).day());
  if (day >= 999) {
    return uint32_t(Cond(condition).answer());
  }
  do {
    if (OneOf(condition, day - (day / 10) * 10)) {
      return 1;
    }
    day /= 10;
  } while (day >= 1);
  return 0;
}

uint32_t DayDigitSum(uint32_t condition) {
  ITC_NATIVE_RUNNING("sub_82140608 EventCondition::DayDigitSum");
  int32_t day = int32_t(SaveV(Save()).day());
  if (day >= 999) {
    return uint32_t(Cond(condition).answer());
  }
  uint32_t sum = 0;
  do {
    sum += uint32_t(day - (day / 10) * 10);
    day /= 10;
  } while (day >= 1);
  return OneOf(condition, S32(sum));
}

// Kinds 6, 7, 8, 27, 28, 30: a save data halfword (signed) at least every
// argument.
uint32_t SaveHalfword(uint32_t condition, uint16_t (SaveV::*field)() const) {
  return AtLeastAll(condition, int16_t((SaveV(Save()).*field)()));
}
uint32_t Races22(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140718 EventCondition::Races22");
  return SaveHalfword(c, &SaveV::races_22);
}
uint32_t Races20(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140788 EventCondition::Races20");
  return SaveHalfword(c, &SaveV::races_20);
}
uint32_t WinsAtLeast(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821407F8 EventCondition::Wins");
  return SaveHalfword(c, &SaveV::wins);
}
uint32_t Save6052(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140E58 EventCondition::Save6052");
  return SaveHalfword(c, &SaveV::cars_bought);
}
uint32_t Save6054(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140EC8 EventCondition::Save6054");
  return SaveHalfword(c, &SaveV::cars_sold);
}
uint32_t Save5910(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140F38 EventCondition::Save5910");
  return SaveHalfword(c, &SaveV::location_count_12);
}

// The categories (+34) with every item won (kinds 9 and 19).
uint64_t WonCategories(uint32_t save) {
  uint64_t mask = ~0ull;
  for (uint32_t id = 0; id < 400; id++) {
    if (Wins(save, id) == 0) {
      mask &= ~Slw1(uint32_t(int8_t(ItemV(Item(id)).category())));
    }
  }
  return mask;
}

uint32_t WonCategoryCount(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140868 EventCondition::WonCategoryCount");
  return AtLeastAll(c, Count48(WonCategories(Save())));
}

uint32_t Money(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821406A8 EventCondition::Money");
  return AtLeastAll(c, S32(SaveV(Save()).money() * 1000));
}

uint32_t Save6024(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140938 EventCondition::Save6024");
  // The original copies the float to its frame and points FloatAtLeast at
  // the copy.
  return FloatAtLeast(c, SaveV(Save()).best_804());
}
uint32_t Save5952(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140980 EventCondition::Save5952");
  // The original copies the float to its frame and points FloatAtLeast at
  // the copy.
  return FloatAtLeast(c, SaveV(Save()).mean_796());
}
uint32_t Save4908(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140FA8 EventCondition::Save4908");
  // The original copies the float to its frame and points FloatAtLeast at
  // the copy.
  return FloatAtLeast(c, SaveV(Save()).day_float_4836());
}

// An item's record's three halfwords summed, to 16 bits.
int32_t RecordSum(uint32_t save, uint32_t id) {
  // (id + 8746) x 12 + the save data: the item's record.
  const Record record = ItemRecord(save, id);
  return int16_t(uint16_t(record.losses() + int16_t(record.wins()) +
                          int16_t(record.outcome3())));
}

uint32_t RacedAll(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_8213FE98 EventCondition::RacedAll");
  const uint32_t save = Save();
  return EveryArgument(c, [&](int32_t a) { return RecordSum(save, a) > 0; }) ? 1 : 0;
}

uint32_t RacedCount(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_8213FF38 EventCondition::RacedCount");
  const Cond cv(c);
  if (cv.arg0() < 0 || cv.arg1() <= 0) {
    return 1;
  }
  const uint32_t save = Save();
  return RecordSum(save, uint32_t(cv.arg0())) >= cv.arg1() ? 1 : 0;
}

uint32_t Save6032(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821409C8 EventCondition::Save6032");
  const int32_t value =
      strings_and_font::steps::Fctiwz(double(SaveV(Save()).best_828_2()));
  return AtLeastAll(c, value);
}

uint32_t BestTime(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140A48 EventCondition::BestTime");
  const uint32_t time = ai_drivers::TimeFromTicks(SaveV(Save()).best_16());
  const uint64_t minutes = (time >> 16) & 0x7FFF;
  const uint64_t seconds = (time >> 10) & 0x3F;
  return AtLeastAll(c, S32(uint32_t(minutes * 60 + seconds)));
}

uint32_t WonAll(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_8213FFC8 EventCondition::WonAll");
  const uint32_t save = Save();
  return EveryArgument(c, [&](int32_t a) { return Wins(save, uint32_t(a)) != 0; })
             ? 1
             : 0;
}

uint32_t WinCount(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140048 EventCondition::WinCount");
  const Cond cv(c);
  if (cv.arg0() < 0 || cv.arg1() <= 0) {
    return 1;
  }
  const uint32_t save = Save();
  const uint32_t id = uint32_t(cv.arg0());
  return int16_t(ItemRecord(save, id).wins()) >= cv.arg1() ? 1
                                                                                            : 0;
}

uint32_t WonCategoriesTest(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140AC8 EventCondition::WonCategories");
  const uint64_t mask = WonCategories(Save());
  for (uint32_t k = 0; k < 4; k++) {
    const uint64_t a = uint64_t(int64_t(LoadS32(Cond(c).arg0_address() + 4 * k)));
    if (a < 48 && (Slw1(uint32_t(a)) & mask) == 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t Won383To391(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140B80 EventCondition::Won383To391");
  // The original lists the ids on its frame first.
  const uint32_t save = Save();
  int32_t won = 0;
  for (uint32_t k = 0; k < 9; k++) {
    if (Wins(save, 383 + k) != 0) {
      won++;
    }
  }
  return AtLeastAll(c, won);
}

uint32_t FullCategories(uint32_t c, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_82140C70 EventCondition::FullCategories");
  const Cond cv(c);
  if (cv.arg0() < 0 || cv.arg1() < 0 || cv.arg2() <= 0) {
    return 1;
  }
  const uint64_t mask = CategoryMask(guest::LoadU16(level), uint32_t(cv.arg0()),
                                     uint32_t(cv.arg1()));
  return Count48(mask) >= cv.arg2() ? 1 : 0;
}

uint32_t FullCategoriesAny(uint32_t c, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_82140D18 EventCondition::FullCategoriesAny");
  const Cond cv(c);
  if (cv.arg0() < 0 || cv.arg1() < 0) {
    return 1;
  }
  // The mode mask 7 (on its frame in the original).
  const uint64_t mask = CategoryMask(guest::LoadU16(level), uint32_t(cv.arg0()), 7);
  return Count48(mask) >= cv.arg1() ? 1 : 0;
}

uint32_t LocationCleared(uint32_t c, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_821400C8 EventCondition::LocationCleared");
  const uint32_t save = Save();
  const Cond cv(c);
  if (cv.arg0() < 0) {
    return 1;
  }
  for (uint32_t id = 0; id < 400; id++) {
    const ItemV item(Item(id));
    const int32_t group = LoadS16(G::location_groups_address() + (uint32_t(int8_t(item.location())) << 1));
    if (cv.arg0() != group) {
      continue;
    }
    if (LoadS16(level) < int16_t(item.level())) {
      continue;
    }
    if (Wins(save, id) != 0) {
      continue;
    }
    const int32_t kind = int8_t(item.kind());
    if (kind == 1) {
      continue;
    }
    if (uint32_t(kind - 3) <= 2) {
      return 0;
    }
  }
  return 1;
}

uint32_t RecordWord6(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140188 EventCondition::RecordWord6");
  const Cond cv(c);
  if (cv.arg0() < 0 || cv.arg1() <= 0) {
    return 1;
  }
  const uint32_t save = Save();
  const uint32_t id = uint32_t(cv.arg0());
  return int16_t(ItemRecord(save, id).rival_wins()) >= cv.arg1()
             ? 1
             : 0;
}

uint32_t Table8D94(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140DB8 EventCondition::Table8D94");
  // The largest of the stats record's three mode counts is worked out and
  // not used; only loads, left out.
  Save();
  return OneOf(c, LoadS32(G::mode_words_address() + 12));
}

uint32_t ModeStreak(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140208 EventCondition::ModeStreak");
  const uint32_t event = race_setup::EventCurrent();
  if (event == 0) {
    return 1;
  }
  const Cond cv(c);
  const int32_t mode_word = cv.arg0();
  if (mode_word < 0) {
    return 1;
  }
  const int32_t streak = cv.arg1();
  if (streak <= 0) {
    return 1;
  }
  const Event ev(event);
  const uint32_t mode = uint32_t(int32_t(int16_t(ev.mode())));
  if (mode_word != LoadS32((mode << 2) + G::mode_words_address())) {
    return 0;
  }
  return int16_t(ev.mode_streak()) >= streak ? 1 : 0;
}

uint32_t CarIs(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141060 EventCondition::CarIs");
  int32_t id;
  if (car_specs::Exists() != 0) {
    id = S32(CarV(GarageCar(Save())).car_id());
  } else {
    // The race data's first racer record (+48; sub_82106540 on it does
    // nothing), its +12.
    id = S32(schema::RaceData(RaceData()).record0_place());
  }
  return OneOf(c, id);
}

uint32_t CarSpec129(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141158 EventCondition::CarSpec129");
  const uint32_t spec = CarSpec();
  if (spec == 0) {
    return 0;
  }
  return OneOf(c, S32(Spec(spec).spec_byte_129()));
}

uint32_t Tuning4() {
  ITC_NATIVE_RUNNING("sub_82141248 EventCondition::Tuning4");
  const uint32_t t = CarTuning();
  return t != 0 && Tuning(t).spec_flag() == 1 ? 1 : 0;
}

uint32_t EngineType7() {
  ITC_NATIVE_RUNNING("sub_821412E8 EventCondition::EngineType7");
  const uint32_t spec = CarSpec();
  return spec != 0 && Spec(spec).engine_type() == 7 ? 1 : 0;
}

uint32_t CarList1() {
  ITC_NATIVE_RUNNING("sub_821413A8 EventCondition::CarList1");
  const uint32_t spec = CarSpec();
  return spec != 0 && int8_t(Spec(spec).list_kind()) == 1 ? 1 : 0;
}

uint32_t Nineties() {
  ITC_NATIVE_RUNNING("sub_82141468 EventCondition::Nineties");
  const uint32_t spec = CarSpec();
  if (spec == 0) {
    return 0;
  }
  const uint32_t year = Spec(spec).model_year();
  return year >= 1990 && year < 2000 ? 1 : 0;
}

uint32_t Spec92() {
  ITC_NATIVE_RUNNING("sub_82141528 EventCondition::Spec92");
  const uint32_t spec = CarSpec();
  return spec != 0 && Spec(spec).spec_92() == 4 && Spec(spec).spec_96() == 0
             ? 1
             : 0;
}

uint32_t CarFloat(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821415F0 EventCondition::CarFloat");
  return FloatAtLeast(c, GarageCarV(GarageCar(Save())).racer_float());
}

// The tuning words kinds 41 and 42 test (+8 to +76).
constexpr TuningWord kParts[] = {
    &Tuning::engine_stage,  &Tuning::power_part_a,  &Tuning::power_part_b,
    &Tuning::nitrous,       &Tuning::drivetrain_32, &Tuning::drivetrain_36,
    &Tuning::drivetrain_40, &Tuning::drivetrain_48, &Tuning::lsd_b,
    &Tuning::body_level,    &Tuning::front_tire_grade, &Tuning::rear_tire_grade};

uint32_t StockParts() {
  ITC_NATIVE_RUNNING("sub_82141738 EventCondition::StockParts");
  const uint32_t t = CarTuning();
  if (t == 0) {
    return 0;
  }
  const Tuning tuning(t);
  for (TuningWord word : kParts) {
    if ((tuning.*word)() != 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t PartsAtLeast(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141858 EventCondition::PartsAtLeast");
  const uint32_t t = CarTuning();
  if (t == 0) {
    return 0;
  }
  const int32_t a = Cond(c).arg0();
  if (a < 0) {
    return 0;
  }
  const Tuning tuning(t);
  for (TuningWord word : kParts) {
    if (S32((tuning.*word)()) < a) {
      return 0;
    }
  }
  return 1;
}

uint32_t TuningAtLeast(uint32_t c, TuningWord word) {
  const uint32_t t = CarTuning();
  if (t == 0) {
    return 0;
  }
  const Tuning tuning(t);
  return EveryArgument(c, [&](int32_t a) { return S32((tuning.*word)()) >= a; }) ? 1 : 0;
}
uint32_t Tuning60(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141990 EventCondition::Tuning60");
  return TuningAtLeast(c, &Tuning::body_level);
}
uint32_t Tuning24(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82141A60 EventCondition::Tuning24");
  return TuningAtLeast(c, &Tuning::power_part_b);
}

uint32_t TuningSet(TuningWord word) {
  const uint32_t t = CarTuning();
  return t != 0 && (Tuning(t).*word)() != 0 ? 1 : 0;
}
uint32_t Tuning128() {
  ITC_NATIVE_RUNNING("sub_82141B30 EventCondition::Tuning128");
  return TuningSet(&Tuning::aero_8);
}
uint32_t Tuning148() {
  ITC_NATIVE_RUNNING("sub_82141BD0 EventCondition::Tuning148");
  return TuningSet(&Tuning::exterior_item_3);
}
uint32_t Tuning144() {
  ITC_NATIVE_RUNNING("sub_82141C70 EventCondition::Tuning144");
  return TuningSet(&Tuning::exterior_item_1);
}
uint32_t Tuning28() {
  ITC_NATIVE_RUNNING("sub_82142070 EventCondition::Tuning28");
  return TuningSet(&Tuning::nitrous);
}

uint32_t AeroFitted() {
  ITC_NATIVE_RUNNING("sub_82141D10 EventCondition::AeroFitted");
  const uint32_t t = CarTuning();
  if (t == 0) {
    return 0;
  }
  const Tuning tuning(t);
  for (TuningWord word : {&Tuning::aero_0, &Tuning::aero_1, &Tuning::aero_2, &Tuning::aero_3,
                          &Tuning::bonnet, &Tuning::aero_6, &Tuning::aero_7, &Tuning::aero_8,
                          &Tuning::aero_9}) {
    if ((tuning.*word)() != 0) {
      return 1;
    }
  }
  return 0;
}

uint32_t Tuning108() {
  ITC_NATIVE_RUNNING("sub_82141E10 EventCondition::Tuning108");
  uint32_t spec = 0;
  uint32_t t;
  if (car_specs::Exists() != 0) {
    const CarV car(GarageCar(Save()));
    const uint32_t sheet = car_specs::GetCarSpec(car.car_id());
    if (sheet != 0) {
      spec = sheet + 640;
    }
    t = car.address + kCarTuning;
  } else {
    const uint32_t race = RaceData();
    if (race == 0) {
      return 0;
    }
    spec = vehicle_accessors::ObjectRecord1888(race + 48);
    t = race + 1696;
  }
  if (spec == 0 || t == 0) {
    return 0;
  }
  const int32_t v = S32(Tuning(t).aero_3());
  if (Spec(spec).spec_120() == 0) {
    return v != 0 ? 1 : 0;
  }
  return v >= 0 ? 1 : 0;
}

uint32_t Tuning64To76() {
  ITC_NATIVE_RUNNING("sub_82141F08 EventCondition::Tuning64To76");
  const uint32_t t = CarTuning();
  if (t == 0) {
    return 0;
  }
  const Tuning tuning(t);
  for (TuningWord word : {&Tuning::front_tire_type, &Tuning::front_tire_grade,
                          &Tuning::rear_tire_type, &Tuning::rear_tire_grade}) {
    if ((tuning.*word)() != 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t Tuning80Not88() {
  ITC_NATIVE_RUNNING("sub_82141FC8 EventCondition::Tuning80Not88");
  const uint32_t t = CarTuning();
  return t != 0 && Tuning(t).front_wheel() != Tuning(t).rear_wheel() ? 1 : 0;
}

uint32_t Tuning184() {
  ITC_NATIVE_RUNNING("sub_82142110 EventCondition::Tuning184");
  const uint32_t t = CarTuning();
  if (t == 0) {
    return 0;
  }
  const Tuning tuning(t);
  for (TuningWord word : {&Tuning::choice0_c, &Tuning::choice1_c, &Tuning::choice2_c,
                          &Tuning::choice3_c, &Tuning::choice4_c}) {
    const int32_t v = S32((tuning.*word)());
    if (v > 0 && v < 5) {
      return 1;
    }
  }
  return 0;
}

uint32_t Decals() {
  ITC_NATIVE_RUNNING("sub_821421D8 EventCondition::Decals");
  // The car's decals (+96): decal k's image, then decal k + 3's.
  const uint32_t at = CarV(GarageCar(Save())).decals_address();
  if (at == 0) {
    return 0;
  }
  for (uint32_t k = 0; k < 3; k++) {
    if (S32(Decal(at + Decal::kSize * k).image()) >= 0 ||
        S32(Decal(at + Decal::kSize * (k + 3)).image()) >= 0) {
      return 1;
    }
  }
  return 0;
}

uint32_t Tuning116() {
  ITC_NATIVE_RUNNING("sub_82142280 EventCondition::Tuning116");
  const uint32_t t = CarTuning();
  if (t == 0) {
    return 0;
  }
  const int32_t v = S32(Tuning(t).bonnet_color_kind());
  return v == 1 || v == 2 ? 1 : 0;
}

uint32_t Car1588() {
  ITC_NATIVE_RUNNING("sub_82142328 EventCondition::Car1588");
  const CarV car(GarageCar(Save()));
  return car.layer0_a() != 0 || car.layer1_a() != 0 || car.layer2_a() != 0 ? 1 : 0;
}

uint32_t Sticker101() {
  ITC_NATIVE_RUNNING("sub_821423C0 EventCondition::Sticker101");
  // The forty decals after the first six (+288): eight runs of five.
  const uint32_t at = CarV(GarageCar(Save())).decals_address() + Decal::kSize * 6;
  if (at == 0) {
    return 0;
  }
  for (uint32_t run = 0; run < 8; run++) {
    for (uint32_t k = 0; k < 5; k++) {
      if (S32(Decal(at + Decal::kSize * (5 * run + k)).image()) == 101) {
        return 1;
      }
    }
  }
  return 0;
}

uint32_t CarBytes(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82142558 EventCondition::CarBytes");
  const uint32_t at = CarV(GarageCar(Save())).setting_bytes_address();
  const uint32_t value = uint32_t(Cond(c).arg0());
  if (at == 0 || value > 9) {
    return 0;
  }
  const int32_t wanted = Cond(c).arg1();
  if (wanted <= 0) {
    return 0;
  }
  int32_t n = 0;
  for (uint32_t k = 0; k < 7; k++) {
    if (LoadS8(at + 5 + k) == S32(value)) {
      n++;
    }
  }
  for (uint32_t k = 0; k < 3; k++) {
    if (LoadS8(at + 2 + k) == S32(value)) {
      n++;
    }
  }
  return n >= wanted ? 1 : 0;
}

uint32_t Setting88() {
  ITC_NATIVE_RUNNING("sub_82142648 EventCondition::Setting88");
  // The car's +16 (the original's base), then its bytes +88 and +89
  // (CarMakersCar names no field there).
  const uint32_t at = CarV(GarageCar(Save())).address + 16;
  if (at == 0) {
    return 0;
  }
  return LoadS8(at + 72) <= -5 && LoadS8(at + 73) <= -5 ? 1 : 0;
}

uint32_t GarageCount(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82140FF0 EventCondition::GarageCount");
  const uint32_t count = uint32_t(int32_t(int16_t(SaveV(Save()).garage_count())));
  return EveryArgument(c, [&](int32_t a) { return count >= uint32_t(a); }) ? 1 : 0;
}

uint32_t SameCars(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_821426D8 EventCondition::SameCars");
  const SaveV sv(Save());
  const GarageList list(sv.garage_address());
  const int32_t count = int16_t(sv.garage_count());
  const uint32_t head = GarageNode(list, int16_t(sv.garage_head()));
  // The n-th car of the list, its id (+12).
  const auto id_at = [&](uint32_t n) {
    uint32_t node = head;
    for (; n != 0; n--) {
      node = GarageNode(list, LoadS16(node + 0));
    }
    return S32(CarV(GarageSlotCar(list, node)).car_id());
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
  return AtLeastAll(c, most);
}

uint32_t SectionRivalTest() {
  ITC_NATIVE_RUNNING("sub_82140290 EventCondition::SectionRival");
  const uint32_t save = Save();
  if (int16_t(career::DayBlockBestStreak(save + 72)) < 3) {
    return 0;
  }
  // RaceEvent::Current, read without a check (as the original): with no
  // event, guest memory's words at +4 to +16 of address 0 as the original
  // reads them (the event itself is a host object, E5 step 4).
  const uint32_t event = race_setup::EventCurrent();
  const Event ev(event);
  const auto word = [&](uint32_t (Event::*field)() const, uint32_t address) {
    return event != 0 ? (ev.*field)() : guest::LoadU32(address);
  };
  if (word(&Event::on_section_4, ev.on_section_4_address()) == 0 ||
      word(&Event::on_section_5, ev.on_section_5_address()) == 0) {
    if (word(&Event::on_section_13, ev.on_section_13_address()) == 0 ||
        word(&Event::on_section_14, ev.on_section_14_address()) == 0) {
      return 0;
    }
  }
  return 1;
}

}  // namespace

uint32_t FloatAtLeast(uint32_t condition, float value) {
  ITC_NATIVE_RUNNING("sub_82140400 EventCondition::FloatAtLeast");
  for (uint32_t k = 0; k < 4; k++) {
    const int32_t a = LoadS32(Cond(condition).arg0_address() + 4 * k);
    if (a < 0) {
      continue;
    }
    if (double(value) < double(float(double(int64_t(a))))) {
      return 0;
    }
  }
  return 1;
}

uint64_t CategoryMask(uint32_t level, uint32_t location, uint32_t modes) {
  ITC_NATIVE_RUNNING("sub_82140340 EventCondition::CategoryMask");
  const uint32_t save = Save();
  const int32_t group = LoadS16((location << 1) + G::location_groups_address());
  uint64_t mask = ~0ull;
  for (uint32_t id = 0; id < 400; id++) {
    const ItemV item(Item(id));
    const bool keep =
        uint16_t(level) == item.level() &&
        LoadS16((uint32_t(int8_t(item.location())) << 1) + G::location_groups_address()) ==
            group &&
        (uint32_t(int32_t(int16_t(item.mode_mask()))) & modes) != 0 && Wins(save, id) != 0;
    if (!keep) {
      mask &= ~Sld1(item.category());
    }
  }
  return mask;
}

uint32_t ConditionMet(uint32_t condition, uint32_t level, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142820 EventCondition::Met");
  const uint32_t kind = uint32_t(int32_t(Cond(condition).kind()));
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
      r3 = DayAtLeast(condition);
      break;
    case 2: r3 = Weekday(condition); break;
    case 3: r3 = DayDivisible(condition); break;
    case 4: r3 = DayDigit(condition); break;
    case 5: r3 = DayDigitSum(condition); break;
    case 6: r3 = Races22(condition); break;
    case 7: r3 = Races20(condition); break;
    case 8: r3 = WinsAtLeast(condition); break;
    case 9: r3 = WonCategoryCount(condition); break;
    case 10: r3 = Money(condition); break;
    case 11: r3 = Save6024(condition); break;
    case 12: r3 = Save5952(condition); break;
    case 13: r3 = RacedAll(condition); break;
    case 14: r3 = RacedCount(condition); break;
    case 15: r3 = Save6032(condition); break;
    case 16: r3 = BestTime(condition); break;
    case 17: r3 = WonAll(condition); break;
    case 18: r3 = WinCount(condition); break;
    case 19: r3 = WonCategoriesTest(condition); break;
    case 20: r3 = Won383To391(condition); break;
    case 21: r3 = FullCategories(condition, level); break;
    case 22: r3 = FullCategoriesAny(condition, level); break;
    case 23: r3 = LocationCleared(condition, level); break;
    case 24: r3 = RecordWord6(condition); break;
    case 25: r3 = Table8D94(condition); break;
    case 26: r3 = ModeStreak(condition); break;
    case 27: r3 = Save6052(condition); break;
    case 28: r3 = Save6054(condition); break;
    case 29: {
      const uint32_t event = race_setup::EventCurrent();
      r3 = event != 0 && int16_t(Event(event).location()) == 6 ? 1 : 0;
      break;
    }
    case 30: r3 = Save5910(condition); break;
    case 31: r3 = CarIs(condition); break;
    case 32: r3 = CarSpec129(condition); break;
    case 33: r3 = Tuning4(); break;
    case 34: r3 = EngineType7(); break;
    case 35: r3 = CarList1(); break;
    case 36: r3 = Nineties(); break;
    case 37: r3 = Spec92(); break;
    case 38: r3 = CarFloat(condition); break;
    case 39: r3 = Save4908(condition); break;
    case 40:  // EventCondition::Power: the recompiled test (Guest)
      r3 = guest.test(guest.user, 0x82141668, condition, level, guest.depth + 96);
      break;
    case 41: r3 = StockParts(); break;
    case 42: r3 = PartsAtLeast(condition); break;
    case 43: r3 = Tuning60(condition); break;
    case 44: r3 = Tuning24(condition); break;
    case 45: r3 = Tuning128(); break;
    case 46: r3 = Tuning148(); break;
    case 47: r3 = Tuning144(); break;
    case 48: r3 = AeroFitted(); break;
    case 49: r3 = Tuning108(); break;
    case 50: r3 = Tuning64To76(); break;
    case 51: r3 = Tuning80Not88(); break;
    case 52: r3 = Tuning28(); break;
    case 53: r3 = Tuning184(); break;
    case 54: r3 = Decals(); break;
    case 55: r3 = Tuning116(); break;
    case 56: r3 = Car1588(); break;
    case 57: r3 = Sticker101(); break;
    case 58:  // always met (SaveData::Of called, its answer not used)
      r3 = uint32_t(int32_t(Cond(condition).answer()));
      break;
    case 59: r3 = CarBytes(condition); break;
    case 60: r3 = Setting88(); break;
    case 61: r3 = 1; break;
    case 62: r3 = GarageCount(condition); break;
    case 63: r3 = SameCars(condition); break;
    case 64:
      r3 = (SaveV(Save()).item_flags_word_11() & 0x20) != 0 ? 1 : 0;
      break;
    case 65: r3 = SectionRivalTest(); break;
    case 66: r3 = int8_t(SaveV(Save()).effects_volume()) > 0 ? 0 : 1; break;
    default: break;
  }
  return r3 - uint32_t(int32_t(Cond(condition).answer())) == 0 ? 1 : 0;
}

uint32_t ConditionState(uint32_t condition, uint32_t level, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142C00 EventCondition::State");
  if (ConditionMet(condition, level, guest.Down(96)) != 0) {
    return 1;
  }
  const int32_t kind = Cond(condition).kind();
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

uint32_t UnlockAll(uint32_t block, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142C80 Unlock::All");
  uint32_t r = 1;
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t s = ConditionState(block + 8 + 20 * k, block + 2, guest.Down(128));
    if (s == 0) {
      return 0;
    }
    if (s == 2) {
      r = 2;
    }
  }
  return r;
}

uint32_t UnlockAny(uint32_t block, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142CE0 Unlock::Any");
  uint32_t r = 0;
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t s = ConditionState(block + 8 + 20 * k, block + 2, guest.Down(128));
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
uint32_t AllOrAny(uint32_t item, const Guest& guest) {
  const uint32_t block = item + 40;
  return ItemV(item).any() == 0 ? UnlockAll(block, guest)
                                      : UnlockAny(block, guest);
}

}  // namespace

uint32_t UnlockStateKind2(uint32_t item, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142D40 Unlock::StateKind2");
  const uint32_t save = Save();
  for (uint32_t id = 0; id < 400; id++) {
    const ItemV other(Item(id));
    if (ItemV(item).category() != other.category() || other.kind() != 0 ||
        Wins(save, id) != 0) {
      continue;
    }
    bool rival = false;
    for (uint32_t k = 0; k < 3; k++) {
      if (LoadS16(ItemV(item).rival_0_address() + 2 * k) == S32(id)) {
        rival = true;
        break;
      }
    }
    if (rival) {
      continue;
    }
    const uint32_t r = AllOrAny(item, guest.Down(112));
    return r == 1 ? 2 : r;
  }
  return AllOrAny(item, guest.Down(112));
}

uint32_t UnlockStateKind5(uint32_t item, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142E18 Unlock::StateKind5");
  const uint32_t save = Save();
  for (uint32_t id = 0; id < 400; id++) {
    if (Wins(save, id) != 0) {
      continue;
    }
    if (ItemV(Item(id)).kind() != 5) {
      const uint32_t r = AllOrAny(item, guest.Down(112));
      return r == 1 ? 2 : r;
    }
  }
  return AllOrAny(item, guest.Down(112));
}

uint32_t UnlockState(uint32_t item, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_82142EC0 Unlock::State");
  const int32_t kind = int8_t(ItemV(item).kind());
  if (kind == 2) {
    return UnlockStateKind2(item, guest.Down(96));
  }
  if (kind == 5) {
    return UnlockStateKind5(item, guest.Down(96));
  }
  return AllOrAny(item, guest.Down(96));
}

uint32_t HasSectionCondition(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82140308 Unlock::HasSectionCondition");
  for (uint32_t k = 0; k < 2; k++) {
    // The block's (an item's +40) conditions at +8 and +28: their kinds.
    if (uint16_t(Cond(block + 8 + 20 * k).kind()) == 65) {
      return 1;
    }
  }
  return 0;
}

uint32_t ItemsAvailable(uint32_t id, const Guest& guest) {
  ITC_NATIVE_RUNNING("sub_821435C8 Items::Available");
  const ItemV item(Item(id));
  if (item.listed() == 2) {
    return 0;
  }
  return ConditionMet(item.available_address(), item.level_address(), guest.Down(96));
}

}  // namespace race_event
