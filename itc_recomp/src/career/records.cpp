// A race's outcome in the stats record and the items' records, the last
// results and the day log, the garage list's insert
// (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace career {

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

// Per outcome: the counter's halfword, the record's halfword it tests, the
// bit set's offset, the opponents' halfword (+1628 + 8 x id + this).
struct OutcomeFields {
  uint32_t counter, record, bits, opponent;
};
constexpr OutcomeFields kFields[3] = {
    {14, 0, 4, 0},  // a loss
    {12, 2, 0, 2},  // a win
    {16, 4, 8, 4},  // outcome 3
};

// The stats record's halfword +5796 + 14 x mode (+ field).
uint32_t ByMode(uint32_t stats, uint32_t mode, uint32_t field) {
  return stats + 14 * mode + 5796 + field;
}

}  // namespace

bool CanCopy(uint32_t destination, uint32_t source, uint32_t size) {
  return destination < 0xA0000000u && !c_runtime::MayShare(destination, size, source, size);
}

static uint32_t CounterAdd(uint8_t* base, uint32_t counter, const Opponent& o, Outcome outcome) {
  const OutcomeFields& f = kFields[int(outcome)];
  CountUp(base, counter + f.counter);
  if (guest::LoadU16(base, o.record + f.record) == 0) {
    const int32_t kind = int8_t(guest::LoadU8(base, o.item + 35));
    if (kind >= 0) {
      const uint32_t word = counter + f.bits + (uint32_t(kind >> 5) << 2);
      guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (kind & 31)));
    }
  }
  CountUp(base, counter + 22);
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
  CounterAddLoss(base, week, o);
  const int32_t kind = int8_t(guest::LoadU8(base, o.item + 35));
  if (kind == 4) {
    CountUp(base, week + 56);
  } else if (kind == 5) {
    CountUp(base, week + 58);
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
  CountUp999(base, block + (o.id << 3) + 1628 + kFields[int(outcome)].opponent);
  const uint32_t list = block + 24;
  const uint32_t count = guest::LoadU32(base, list + 1600);
  for (uint32_t i = 0; i < count; i++) {
    if (guest::LoadU32(base, list + 4 * i) == o.id) {
      return block;
    }
  }
  const uint32_t at = list + (guest::LoadU32(base, list + 1600) << 2);
  guest::StoreU32(base, at, o.id);
  guest::StoreU32(base, list + 1600, guest::LoadU32(base, list + 1600) + 1);
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
  CounterAddWin(base, stats, o);
  CountUp(base, stats + 5970);
  const uint16_t streak = guest::LoadU16(base, stats + 5970);
  if (int16_t(guest::LoadU16(base, stats + 5968)) < int16_t(streak)) {
    guest::StoreU16(base, stats + 5968, streak);
  }
  const uint32_t day = stats + 24;
  CountUp(base, day + 4828);
  const uint16_t day_streak = guest::LoadU16(base, stats + 4852);
  if (int16_t(guest::LoadU16(base, stats + 4854)) < int16_t(day_streak)) {
    guest::StoreU16(base, stats + 4854, day_streak);
  }
  guest::StoreU16(base, stats + 5972, 0);
  const uint32_t week = stats + 4864;
  CounterAddWin(base, week, o);
  if (guest::LoadU8(base, o.item + 35) == 1) {
    CountUp(base, week + 52);
  }
  OpponentsAddWin(base, day, o);
  CounterAddWin(base, stats + 4924, o);
  CountUp(base, ByMode(stats, mode, 2));
  CountUp(base, ByMode(stats, mode, 10));
  return stats + 4924;
}

uint32_t StatsAddLoss(uint8_t* base, uint32_t stats, const Opponent& o, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82113AB8 Stats::AddLoss");
  CounterAddLoss(base, stats, o);
  CountUp(base, stats + 5972);
  guest::StoreU16(base, stats + 5970, 0);
  guest::StoreU16(base, stats + 4852, 0);
  WeekCounterAddLoss(base, stats + 4864, o);
  OpponentsAddLoss(base, stats + 24, o);
  CounterAddLoss(base, stats + 4924, o);
  CountUp(base, ByMode(stats, mode, 4));
  CountUp(base, ByMode(stats, mode, 10));
  return stats + 4924;
}

uint32_t StatsAddOutcome3(uint8_t* base, uint32_t stats, const Opponent& o, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82113B50 Stats::AddOutcome3");
  CounterAddOutcome3(base, stats, o);
  guest::StoreU16(base, stats + 5970, 0);
  guest::StoreU16(base, stats + 4852, 0);
  guest::StoreU16(base, stats + 5972, 0);
  CounterAddOutcome3(base, stats + 4864, o);
  OpponentsAddOutcome3(base, stats + 24, o);
  CounterAddOutcome3(base, stats + 4924, o);
  CountUp(base, ByMode(stats, mode, 6));
  CountUp(base, ByMode(stats, mode, 10));
  return stats + 4924;
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
  o.record = save + kItemRecords + 12 * id;
  guest::StoreU32(base, frame + 84, o.record);
  data_tables::RecordsItem(base, id);
  const uint32_t stats = save + kStats;
  const uint32_t mode = guest::LoadU32(base, save + 6104);
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
  CountUp999(base, o.record + kFields[int(outcome)].record);
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
  const uint32_t result = vehicle_accessors::RacerRecordsAt(base, race + 0x59AE8, 0xFFFFFFFFu);
  guest::StoreU32(base, out + 0, guest::LoadU32(base, result + 812));
  guest::StoreU32(base, out + 4, guest::LoadU32(base, result + 816));
  guest::StoreU32(base, out + 8, guest::LoadU32(base, result + 796));
  guest::StoreU32(base, out + 12, guest::LoadU32(base, result + 800));
  const uint32_t racer = race + 3936 * guest::LoadU32(base, result + 4);
  guest::StoreU32(base, out + 40, guest::LoadU32(base, racer + 60));
  guest::StoreU32(base, out + 16, int32_t(guest::LoadU32(base, result + 3924)) == 1 ? 1 : 0);
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, out + 20 + 4 * i, guest::LoadU32(base, result + 980 + 972 * i));
  }
  guest::StoreU32(base, out + 44, uint32_t(int32_t(int16_t(guest::LoadU16(base, result + 968)))));
  guest::StoreU32(base, out + 32, guest::LoadU32(base, result + 972));
  guest::StoreU32(base, out + 36, guest::LoadU32(base, race + 32));
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
  const uint32_t r3 = ResultRecordFill(base, list + 4, race);
  const int32_t count = int32_t(guest::LoadU32(base, list));
  if (count < 15) {
    guest::StoreU32(base, list, uint32_t(count + 1));
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
  guest::StoreU32(base, log + 4, uint32_t(int32_t(int16_t(guest::LoadU16(base, day + 22)))));
  // lfs and stfs: the float through a double.
  guest::StoreFloat(base, log + 8, float(double(guest::LoadFloat(base, day + 4836))));
  guest::StoreU32(base, log + 12, uint32_t(int32_t(int16_t(guest::LoadU16(base, day + 18)))));
  const int32_t count = int32_t(guest::LoadU32(base, log));
  if (count < 10) {
    guest::StoreU32(base, log, uint32_t(count + 1));
  }
  return log;
}

uint32_t StatsStartDay(uint8_t* base, uint32_t stats, uint32_t weekday) {
  ITC_NATIVE_RUNNING("sub_82112088 Stats::StartDay");
  const float zero = guest::LoadFloat(base, kZeroFloat);
  if (weekday == 0) {
    const uint32_t week = stats + 4864;
    Clear(base, week, 24);
    guest::StoreFloat(base, week + 24, zero);
    guest::StoreFloat(base, week + 28, zero);
    guest::StoreU32(base, week + 32, 0);
    guest::StoreU32(base, week + 36, 0);
    guest::StoreU32(base, week + 48, 0);
    guest::StoreU32(base, week + 44, 0);
    for (uint32_t k = 52; k <= 58; k += 2) {
      guest::StoreU16(base, week + k, 0);
    }
  }
  const uint32_t day = stats + 24;
  DayLogPush(base, stats + 5672, day);
  Clear(base, day, 4828);
  guest::StoreFloat(base, day + 4836, zero);
  guest::StoreU16(base, day + 4830, 0);
  guest::StoreU16(base, day + 4828, 0);
  guest::StoreU32(base, day + 4832, 0);
  guest::StoreU16(base, stats + 5970, 0);
  guest::StoreU16(base, stats + 5972, 0);
  guest::StoreU32(base, stats + 5960, 0);
  guest::StoreU32(base, stats + 5964, 0);
  guest::StoreU32(base, stats + 5952, 0);
  guest::StoreU32(base, stats + 5956, 1);
  return day;
}

// --- The garage's list ---

uint32_t GarageListInsert(uint8_t* base, uint32_t list, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82113468 GarageList::Insert");
  uint32_t n = uint32_t(int32_t(int16_t(position)));
  uint32_t link = list + 204;
  if (n != 0) {
    uint32_t node = uint32_t(int32_t(int16_t(guest::LoadU16(base, list + 204))));
    for (;;) {
      link = (node << 2) + list;
      if (--n == 0) {
        break;
      }
      node = uint32_t(int32_t(int16_t(guest::LoadU16(base, link))));
    }
  }
  const uint16_t free = guest::LoadU16(base, list + 200);
  const uint32_t slot = uint32_t(int32_t(int16_t(free))) << 2;
  guest::StoreU16(base, list + 200, guest::LoadU16(base, slot + list));
  guest::StoreU16(base, slot + list, free);
  guest::StoreU16(base, slot + list, guest::LoadU16(base, link));
  guest::StoreU16(base, link, free);
  guest::StoreU16(base, list + 208, uint16_t(guest::LoadU16(base, list + 208) + 1));
  return list;
}

bool GarageInsertCar(uint8_t* base, uint32_t garage, uint32_t car, uint32_t position,
                     uint32_t* destination) {
  ITC_NATIVE_RUNNING("sub_821154E0 Garage::InsertCar");
  GarageListInsert(base, garage, uint32_t(int32_t(int16_t(position))));
  uint32_t node = (uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204)))) << 2) + garage;
  for (uint32_t n = position; n != 0; n--) {
    node = (uint32_t(int32_t(int16_t(guest::LoadU16(base, node)))) << 2) + garage;
  }
  const uint32_t slot = uint32_t(int32_t(int16_t(guest::LoadU16(base, node + 2))));
  *destination = slot * 1968 + garage + 212;
  if (!CanCopy(*destination, car, 1968)) {
    return false;
  }
  c_runtime::Memcpy(base, *destination, car, 1968);
  return true;
}

}  // namespace career
