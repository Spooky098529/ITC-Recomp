// A race's outcome in the stats record and the items' records, the last
// results and the day log, the garage list's insert
// (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "schema/career.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace career {

using schema::CareerCounter;
using schema::CareerItemRecord;
using schema::CareerOpponentItem;
using schema::CareerSave;
using schema::CareerStats;

namespace {

// A halfword counted up while below 9,999 (signed), as the originals' lha,
// cmpwi and sth.
void CountUp(uint8_t* base, uint32_t address) {
  const int32_t value = int16_t(guest::LoadU16(base, address));
  if (value < 9999) {
    guest::StoreU16(base, address, uint16_t(value + 1));
  }
}

// A halfword set to its value plus one, at most 999 (stored either way).
void CountUp999(uint8_t* base, uint32_t address) {
  int32_t value = int16_t(guest::LoadU16(base, address)) + 1;
  if (value >= 999) {
    value = 999;
  }
  guest::StoreU16(base, address, uint16_t(value));
}

// Per outcome: the counter's halfword, the bit set, the item record's
// halfword, the opponent record's halfword (its 8 bytes' +0, +2, +4).
uint32_t CounterField(const CareerCounter& c, Outcome outcome) {
  switch (outcome) {
    case Outcome::kLoss:
      return c.losses_address();
    case Outcome::kWin:
      return c.wins_address();
    default:
      return c.outcome3_address();
  }
}
uint32_t KindsField(const CareerCounter& c, Outcome outcome) {
  switch (outcome) {
    case Outcome::kLoss:
      return c.lost_kinds_address();
    case Outcome::kWin:
      return c.won_kinds_address();
    default:
      return c.outcome3_kinds_address();
  }
}
uint32_t RecordField(const CareerItemRecord& r, Outcome outcome) {
  switch (outcome) {
    case Outcome::kLoss:
      return r.losses_address();
    case Outcome::kWin:
      return r.wins_address();
    default:
      return r.outcome3_address();
  }
}
constexpr uint32_t kOpponentField[3] = {0, 2, 4};  // a loss, a win, outcome 3

// The stats record's by-mode halfword (14 bytes a mode; + field).
uint32_t ByMode(const CareerStats& s, uint32_t mode, uint32_t field) {
  return s.by_mode_address() + 14 * mode + field;
}

}  // namespace

bool CanCopy(uint32_t destination, uint32_t source, uint32_t size) {
  return destination < 0xA0000000u && !c_runtime::MayShare(destination, size, source, size);
}

static uint32_t CounterAdd(uint8_t* base, uint32_t counter, const Opponent& o, Outcome outcome) {
  const CareerCounter c(base, counter);
  CountUp(base, CounterField(c, outcome));
  if (guest::LoadU16(base, RecordField(CareerItemRecord(base, o.record), outcome)) == 0) {
    const int32_t kind = int8_t(CareerOpponentItem(base, o.item).kind());
    if (kind >= 0) {
      const uint32_t word = KindsField(c, outcome) + (uint32_t(kind >> 5) << 2);
      guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (kind & 31)));
    }
  }
  CountUp(base, c.races_address());
  return counter;
}

uint32_t CounterAddWin(uint8_t* base, uint32_t counter, const Opponent& o) {
  ITC_NATIVE_RUNNING("sub_82111E10 Counter::AddWin");
  return CounterAdd(base, counter, o, Outcome::kWin);
}

uint32_t CounterAddLoss(uint8_t* base, uint32_t counter, const Opponent& o) {
  ITC_NATIVE_RUNNING("sub_82111E80 Counter::AddLoss");
  return CounterAdd(base, counter, o, Outcome::kLoss);
}

uint32_t CounterAddOutcome3(uint8_t* base, uint32_t counter, const Opponent& o) {
  ITC_NATIVE_RUNNING("sub_82111EF0 Counter::AddOutcome3");
  return CounterAdd(base, counter, o, Outcome::kOutcome3);
}

uint32_t WeekCounterAddLoss(uint8_t* base, uint32_t week, const Opponent& o) {
  ITC_NATIVE_RUNNING("sub_82111F60 WeekCounter::AddLoss");
  const CareerCounter w(base, week);
  CounterAddLoss(base, week, o);
  const int32_t kind = int8_t(CareerOpponentItem(base, o.item).kind());
  if (kind == 4) {
    CountUp(base, w.week_kind4_losses_address());
  } else if (kind == 5) {
    CountUp(base, w.week_kind5_losses_address());
  }
  return week;
}

namespace {

uint32_t OpponentsAdd(uint8_t* base, uint32_t block, const Opponent& o, Outcome outcome) {
  switch (outcome) {
    case Outcome::kLoss:
      CounterAddLoss(base, block, o);
      break;
    case Outcome::kWin:
      CounterAddWin(base, block, o);
      break;
    case Outcome::kOutcome3:
      CounterAddOutcome3(base, block, o);
      break;
  }
  const CareerCounter day(base, block);
  CountUp999(base, day.day_opponent_records_address() + (o.id << 3) +
                       kOpponentField[int(outcome)]);
  const uint32_t list = day.day_opponents_address();
  const uint32_t count = day.day_opponent_count();
  for (uint32_t i = 0; i < count; i++) {
    if (guest::LoadU32(base, list + 4 * i) == o.id) {
      return block;
    }
  }
  const uint32_t at = list + (day.day_opponent_count() << 2);
  guest::StoreU32(base, at, o.id);
  day.set_day_opponent_count(day.day_opponent_count() + 1);
  return block;
}

}  // namespace

uint32_t OpponentsAddWin(uint8_t* base, uint32_t block, const Opponent& o) {
  ITC_NATIVE_RUNNING("sub_821134D0 Opponents::AddWin");
  return OpponentsAdd(base, block, o, Outcome::kWin);
}

uint32_t OpponentsAddLoss(uint8_t* base, uint32_t block, const Opponent& o) {
  ITC_NATIVE_RUNNING("sub_82113580 Opponents::AddLoss");
  return OpponentsAdd(base, block, o, Outcome::kLoss);
}

uint32_t OpponentsAddOutcome3(uint8_t* base, uint32_t block, const Opponent& o) {
  ITC_NATIVE_RUNNING("sub_82113628 Opponents::AddOutcome3");
  return OpponentsAdd(base, block, o, Outcome::kOutcome3);
}

uint32_t StatsAddWin(uint8_t* base, uint32_t stats, const Opponent& o, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821139B8 Stats::AddWin");
  const CareerStats s(base, stats);
  CounterAddWin(base, s.counter_address(), o);
  CountUp(base, s.win_streak_address());
  const uint16_t streak = s.win_streak();
  if (int16_t(s.best_win_streak()) < int16_t(streak)) {
    s.set_best_win_streak(streak);
  }
  const uint32_t day = s.day_address();
  const CareerCounter d(base, day);
  CountUp(base, d.day_win_streak_address());
  const uint16_t day_streak = d.day_win_streak();
  if (int16_t(d.day_best_streak()) < int16_t(day_streak)) {
    d.set_day_best_streak(day_streak);
  }
  s.set_loss_streak(0);
  const uint32_t week = s.week_address();
  CounterAddWin(base, week, o);
  if (CareerOpponentItem(base, o.item).kind() == 1) {
    CountUp(base, CareerCounter(base, week).week_kind1_wins_address());
  }
  OpponentsAddWin(base, day, o);
  CounterAddWin(base, s.other_counter_address(), o);
  CountUp(base, ByMode(s, mode, 2));
  CountUp(base, ByMode(s, mode, 10));
  return s.other_counter_address();
}

uint32_t StatsAddLoss(uint8_t* base, uint32_t stats, const Opponent& o, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82113AB8 Stats::AddLoss");
  const CareerStats s(base, stats);
  CounterAddLoss(base, s.counter_address(), o);
  CountUp(base, s.loss_streak_address());
  s.set_win_streak(0);
  CareerCounter(base, s.day_address()).set_day_win_streak(0);
  WeekCounterAddLoss(base, s.week_address(), o);
  OpponentsAddLoss(base, s.day_address(), o);
  CounterAddLoss(base, s.other_counter_address(), o);
  CountUp(base, ByMode(s, mode, 4));
  CountUp(base, ByMode(s, mode, 10));
  return s.other_counter_address();
}

uint32_t StatsAddOutcome3(uint8_t* base, uint32_t stats, const Opponent& o, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82113B50 Stats::AddOutcome3");
  const CareerStats s(base, stats);
  CounterAddOutcome3(base, s.counter_address(), o);
  s.set_win_streak(0);
  CareerCounter(base, s.day_address()).set_day_win_streak(0);
  s.set_loss_streak(0);
  CounterAddOutcome3(base, s.week_address(), o);
  OpponentsAddOutcome3(base, s.day_address(), o);
  CounterAddOutcome3(base, s.other_counter_address(), o);
  CountUp(base, ByMode(s, mode, 6));
  CountUp(base, ByMode(s, mode, 10));
  return s.other_counter_address();
}

namespace {

uint32_t ItemRecordsAdd(uint8_t* base, uint32_t save, uint32_t id, uint32_t sp,
                        Outcome outcome) {
  const uint32_t frame = sp - kItemRecordsFrame;
  guest::StoreU32(base, frame + 80, id);
  Opponent o;
  o.id = id;
  o.item = data_tables::RecordsItem(base, id);
  guest::StoreU32(base, frame + 88, o.item);
  const CareerSave s(base, save);
  o.record = s.item_records_address() + 12 * id;
  guest::StoreU32(base, frame + 84, o.record);
  data_tables::RecordsItem(base, id);
  const uint32_t stats = s.stats_address();
  const uint32_t mode = s.mode();
  uint32_t r3 = 0;
  switch (outcome) {
    case Outcome::kLoss:
      r3 = StatsAddLoss(base, stats, o, mode);
      break;
    case Outcome::kWin:
      r3 = StatsAddWin(base, stats, o, mode);
      break;
    case Outcome::kOutcome3:
      r3 = StatsAddOutcome3(base, stats, o, mode);
      break;
  }
  CountUp999(base, RecordField(CareerItemRecord(base, o.record), outcome));
  return r3;
}

}  // namespace

uint32_t ItemRecordsAddWin(uint8_t* base, uint32_t save, uint32_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82114B88 ItemRecords::AddWin");
  return ItemRecordsAdd(base, save, id, sp, Outcome::kWin);
}

uint32_t ItemRecordsAddLoss(uint8_t* base, uint32_t save, uint32_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82114B08 ItemRecords::AddLoss");
  return ItemRecordsAdd(base, save, id, sp, Outcome::kLoss);
}

uint32_t ItemRecordsAddOutcome3(uint8_t* base, uint32_t save, uint32_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82114A88 ItemRecords::AddOutcome3");
  return ItemRecordsAdd(base, save, id, sp, Outcome::kOutcome3);
}

// --- The last results ---

uint32_t ResultRecordFill(uint8_t* base, uint32_t out, uint32_t race) {
  ITC_NATIVE_RUNNING("sub_82110850 ResultRecord::Fill");
  const schema::CareerResultRecord r(base, out);
  const uint32_t result = vehicle_accessors::RacerRecordsAt(base, race + 0x59AE8, 0xFFFFFFFFu);
  r.set_value_812(guest::LoadU32(base, result + 812));
  r.set_value_816(guest::LoadU32(base, result + 816));
  r.set_mean_796(guest::LoadU32(base, result + 796));
  r.set_mean_796_count(guest::LoadU32(base, result + 800));
  const uint32_t racer = race + 3936 * guest::LoadU32(base, result + 4);
  r.set_racer_60(guest::LoadU32(base, racer + 60));
  r.set_won(int32_t(guest::LoadU32(base, result + 3924)) == 1 ? 1 : 0);
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, r.opponent_1_address() + 4 * i,
                    guest::LoadU32(base, result + 980 + 972 * i));
  }
  r.set_value_968(uint32_t(int32_t(int16_t(guest::LoadU16(base, result + 968)))));
  r.set_value_972(guest::LoadU32(base, result + 972));
  r.set_race_mode(schema::CareerFillRaceData(base, race).mode());
  return result;
}

bool ResultsPushNative(uint32_t list) {
  for (uint32_t to = list + 676; to != list + 4; to -= 48) {
    if (!CanCopy(to, to - 48, 48)) {
      return false;
    }
  }
  return true;
}

uint32_t ResultsPush(uint8_t* base, uint32_t list, uint32_t race) {
  ITC_NATIVE_RUNNING("sub_82111FC8 Results::Push");
  for (uint32_t to = list + 676; to != list + 4; to -= 48) {
    c_runtime::Memcpy(base, to, to - 48, 48);
  }
  const schema::CareerResults results(base, list);
  const uint32_t r3 = ResultRecordFill(base, results.records_address(), race);
  const int32_t count = int32_t(results.count());
  if (count < 15) {
    results.set_count(uint32_t(count + 1));
  }
  return r3;
}

// --- The day ---

namespace {

void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

}  // namespace

uint32_t DayLogPush(uint8_t* base, uint32_t log, uint32_t day) {
  ITC_NATIVE_RUNNING("sub_82112028 DayLog::Push");
  for (uint32_t to = log + 112; to != log + 4; to -= 12) {
    const uint32_t a = guest::LoadU32(base, to - 12);
    const uint32_t b = guest::LoadU32(base, to - 8);
    const uint32_t c = guest::LoadU32(base, to - 4);
    guest::StoreU32(base, to + 0, a);
    guest::StoreU32(base, to + 4, b);
    guest::StoreU32(base, to + 8, c);
  }
  const schema::CareerDayLog l(base, log);
  const CareerCounter d(base, day);
  l.set_newest_races(uint32_t(int32_t(int16_t(d.races()))));
  // lfs and stfs: the float through a double.
  l.set_newest_4836(float(double(d.unknown_4836())));
  l.set_newest_18(uint32_t(int32_t(int16_t(d.unknown_18()))));
  const int32_t count = int32_t(l.count());
  if (count < 10) {
    l.set_count(uint32_t(count + 1));
  }
  return log;
}

uint32_t StatsStartDay(uint8_t* base, uint32_t stats, uint32_t weekday) {
  ITC_NATIVE_RUNNING("sub_82112088 Stats::StartDay");
  const CareerStats s(base, stats);
  const float zero = schema::CareerGlobals::zero(base);
  if (weekday == 0) {
    const uint32_t week = s.week_address();
    const CareerCounter w(base, week);
    Clear(base, week, 24);
    w.set_week_best_804(zero);
    w.set_week_mean_796(zero);
    w.set_week_mean_796_count(0);
    w.set_week_808(0);
    w.set_week_sum_812(0);
    w.set_week_sum_816(0);
    w.set_week_kind1_wins(0);
    w.set_unknown_54(0);
    w.set_week_kind4_losses(0);
    w.set_week_kind5_losses(0);
  }
  const uint32_t day = s.day_address();
  const CareerCounter d(base, day);
  DayLogPush(base, s.day_log_address(), day);
  Clear(base, day, 4828);
  d.set_unknown_4836(zero);
  d.set_day_best_streak(0);
  d.set_day_win_streak(0);
  d.set_day_points(0);
  s.set_win_streak(0);
  s.set_loss_streak(0);
  s.set_day_total_8(0);
  s.set_day_total_12(0);
  s.set_day_total_4(0);
  s.set_day_total_0(1);
  return day;
}

// --- The garage's list ---

uint32_t GarageListInsert(uint8_t* base, uint32_t list, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82113468 GarageList::Insert");
  const schema::CareerGarageList gl(base, list);
  uint32_t n = uint32_t(int32_t(int16_t(position)));
  uint32_t link = gl.head_address();
  if (n != 0) {
    uint32_t node = uint32_t(int32_t(int16_t(gl.head())));
    for (;;) {
      link = (node << 2) + list;
      if (--n == 0) {
        break;
      }
      node = uint32_t(int32_t(int16_t(guest::LoadU16(base, link))));
    }
  }
  const uint16_t free = gl.free();
  const uint32_t slot = uint32_t(int32_t(int16_t(free))) << 2;
  gl.set_free(guest::LoadU16(base, slot + list));
  guest::StoreU16(base, slot + list, free);
  guest::StoreU16(base, slot + list, guest::LoadU16(base, link));
  guest::StoreU16(base, link, free);
  gl.set_count(uint16_t(gl.count() + 1));
  return list;
}

bool GarageInsertCar(uint8_t* base, uint32_t garage, uint32_t car, uint32_t position,
                     uint32_t* destination) {
  ITC_NATIVE_RUNNING("sub_821154E0 Garage::InsertCar");
  const schema::CareerGarageList gl(base, garage);
  GarageListInsert(base, garage, uint32_t(int32_t(int16_t(position))));
  uint32_t node = (uint32_t(int32_t(int16_t(gl.head()))) << 2) + garage;
  for (uint32_t n = position; n != 0; n--) {
    node = (uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) << 2) + garage;
  }
  const uint32_t slot = uint32_t(int32_t(int16_t(guest::LoadU16(base, node + 2))));
  *destination = slot * 1968 + gl.cars_address();
  if (!CanCopy(*destination, car, 1968)) {
    return false;
  }
  c_runtime::Memcpy(base, *destination, car, 1968);
  return true;
}

}  // namespace career
