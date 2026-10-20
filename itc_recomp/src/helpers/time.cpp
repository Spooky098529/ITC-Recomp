#include "time.h"

#include <chrono>
#include <cstring>

namespace helpers {

namespace {

using Ticks = std::chrono::duration<int64_t, std::ratio<1, 10000000>>;  // 100 ns
// FILETIME's epoch (1601-01-01) to the Unix epoch.
constexpr int64_t kUnixEpoch = 116444736000000000;

// Minutes to 100 ns units, the minutes' product cut to 32 bits as the
// game's (mulli then extsw).
int64_t MinutesToTicks(int32_t minutes) {
  return int64_t(int32_t(uint32_t(minutes) * 60u)) * 10000000;
}

TimeFields ToFields(const SystemTime& s) {
  return {s.year, s.month, s.day, s.hour, s.minute, s.second, s.milliseconds, s.day_of_week};
}

}  // namespace

uint64_t SystemTimeNow() {
  auto since_unix = std::chrono::duration_cast<Ticks>(
      std::chrono::system_clock::now().time_since_epoch());
  return uint64_t(since_unix.count() + kUnixEpoch);
}

TimeFields TimeToTimeFields(uint64_t time) {
  using namespace std::chrono;
  sys_time<Ticks> tp{Ticks{int64_t(time) - kUnixEpoch}};
  auto day = floor<days>(tp);
  year_month_day ymd{day};
  hh_mm_ss clock{floor<milliseconds>(tp - day)};
  TimeFields f;
  f.year = uint16_t(int(ymd.year()));
  f.month = uint16_t(unsigned(ymd.month()));
  f.day = uint16_t(unsigned(ymd.day()));
  f.hour = uint16_t(clock.hours().count());
  f.minute = uint16_t(clock.minutes().count());
  f.second = uint16_t(clock.seconds().count());
  f.milliseconds = uint16_t(clock.subseconds().count());
  f.weekday = uint16_t(weekday{day}.c_encoding());
  return f;
}

bool TimeFieldsToTime(const TimeFields& f, uint64_t* time) {
  using namespace std::chrono;
  if (f.year < 1601 || f.month < 1 || f.month > 12 || f.day < 1 || f.day > 31 || f.hour > 23 ||
      f.minute > 59 || f.second > 59 || f.milliseconds > 999) {
    return false;
  }
  year_month_day ymd{year{f.year}, month{f.month}, day{f.day}};
  if (!ymd.ok()) {
    return false;
  }
  Ticks t = sys_days{ymd}.time_since_epoch() + hours{f.hour} + minutes{f.minute} +
            seconds{f.second} + milliseconds{f.milliseconds};
  *time = uint64_t(t.count() + kUnixEpoch);
  return true;
}

SystemTime FileTimeToSystemTime(uint64_t time) {
  TimeFields f = TimeToTimeFields(time);
  return {f.year, f.month, f.weekday, f.day, f.hour, f.minute, f.second, f.milliseconds};
}

bool SystemTimeToFileTime(const SystemTime& system_time, uint64_t* time) {
  return TimeFieldsToTime(ToFields(system_time), time);
}

bool CutoverTime(const SystemTime& rule, uint64_t now, bool from_this_year, uint64_t* cutover,
                 bool* invalid_parameter) {
  SystemTime today = FileTimeToSystemTime(now);
  if (rule.year != 0) {
    if (!SystemTimeToFileTime(rule, cutover)) {
      *invalid_parameter = true;
      return false;
    }
    return int64_t(*cutover) >= int64_t(now);
  }

  // A rule: the week'th weekday of the month (the last within it for week 5).
  const int16_t week = int16_t(rule.day);
  if (week > 5 || week == 0) {
    return false;
  }
  const uint16_t month = rule.month;
  int16_t year = int16_t(today.year);
  bool same_month = false;
  if (!from_this_year) {
    if (int16_t(month) < int32_t(today.month)) {
      year = int16_t(today.year + 1);
    } else if (int16_t(month) == int32_t(today.month)) {
      same_month = true;
    }
  }
  TimeFields first{uint16_t(year), month, 1, rule.hour, rule.minute, rule.second,
                   rule.milliseconds, 0};
  uint64_t time;
  if (!TimeFieldsToTime(first, &time)) {
    return false;
  }
  const int32_t weekday = int16_t(rule.day_of_week);
  for (;;) {
    // The month's first such weekday, then a week on for each further week
    // while the date is valid.
    TimeFields f = TimeToTimeFields(time);
    int32_t found = int16_t(f.weekday);
    int32_t day;
    if (found > weekday) {
      day = int16_t(weekday - found + int16_t(first.day) + 7);
    } else if (found < weekday) {
      day = int16_t(weekday - found + first.day);
    } else {
      day = first.day;
    }
    uint16_t last = uint16_t(day);
    for (int16_t n = 1; n < week;) {
      first.day = uint16_t(day + 7);
      if (!TimeFieldsToTime(first, &time)) {
        break;
      }
      n = int16_t(n + 1);
      last = TimeToTimeFields(time).day;
      day = first.day;
    }
    first.day = last;
    if (!TimeFieldsToTime(first, &time)) {
      return false;
    }
    if (same_month) {
      int32_t cutover_day = int16_t(first.day);
      bool passed = cutover_day < int32_t(today.day) ||
                    (cutover_day == int32_t(today.day) && int64_t(time) < int64_t(now));
      if (passed) {
        // This year's has passed: next year's.
        year = int16_t(year + 1);
        same_month = false;
        first = {uint16_t(year), month, 1, rule.hour, rule.minute, rule.second,
                 rule.milliseconds, 0};
        if (!TimeFieldsToTime(first, &time)) {
          return false;
        }
        continue;
      }
    }
    *cutover = time;
    return true;
  }
}

uint32_t TimeZoneState(TimeZoneInformation& zone, bool uses_daylight, uint64_t now,
                       bool* invalid_parameter) {
  if (!uses_daylight) {
    zone.standard_bias = 0;
    zone.standard_date = {};
    zone.daylight_bias = 0;
    zone.daylight_date = {};
    std::memset(zone.daylight_name, 0, sizeof(zone.daylight_name));
  }
  if (zone.standard_date.month == 0 || zone.daylight_date.month == 0) {
    return 0;
  }
  uint64_t standard_cutover, daylight_cutover;
  if (!CutoverTime(zone.standard_date, now, true, &standard_cutover, invalid_parameter) ||
      !CutoverTime(zone.daylight_date, now, true, &daylight_cutover, invalid_parameter)) {
    return 0;
  }
  // The cutovers are local times: daylight time starts in standard time,
  // standard time in daylight time.
  int64_t bias = MinutesToTicks(zone.bias);
  int64_t daylight_starts = MinutesToTicks(zone.standard_bias) + bias + int64_t(daylight_cutover);
  int64_t standard_starts = MinutesToTicks(zone.daylight_bias) + bias + int64_t(standard_cutover);
  int64_t t = int64_t(now);
  if (daylight_starts < standard_starts) {
    return (t < daylight_starts || t >= standard_starts) ? 1 : 2;
  }
  return (t < standard_starts || t >= daylight_starts) ? 2 : 1;
}

int64_t TimeZoneBias(const TimeZoneInformation& zone, uint32_t state) {
  int32_t minutes;
  if (state == 0) {
    minutes = zone.bias;
  } else if (state == 1) {
    minutes = int32_t(uint32_t(zone.bias) + uint32_t(zone.standard_bias));
  } else if (state == 2) {
    minutes = int32_t(uint32_t(zone.bias) + uint32_t(zone.daylight_bias));
  } else {
    minutes = 0;
  }
  return MinutesToTicks(minutes);
}

}  // namespace helpers
