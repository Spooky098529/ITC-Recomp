// The game's time functions (its Win32 time code over the kernel's time
// calls) as plain C++: the clock, the calendar math, local time and the time
// zone's daylight saving state. Times are FILETIMEs (100 ns units since
// 1601-01-01). src/hooks/time.cpp is the adapter for the recompiled callers.
// See B6 in docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace helpers {

// The kernel's TIME_FIELDS (RtlTimeToTimeFields, RtlTimeFieldsToTime).
struct TimeFields {
  uint16_t year, month, day, hour, minute, second, milliseconds, weekday;
};

// Win32's SYSTEMTIME (note the order: the weekday comes before the day).
struct SystemTime {
  uint16_t year, month, day_of_week, day, hour, minute, second, milliseconds;
};

// The parts of Win32's TIME_ZONE_INFORMATION the game's code touches. Biases
// are minutes. A date with year 0 is a rule: `day` is the week of the month
// (5 for the last), `day_of_week` the weekday.
struct TimeZoneInformation {
  int32_t bias;
  SystemTime standard_date;
  int32_t standard_bias;
  uint16_t daylight_name[32];
  SystemTime daylight_date;
  int32_t daylight_bias;
};

// KeQuerySystemTime: the time now (the host's, UTC).
uint64_t SystemTimeNow();

// RtlTimeToTimeFields (weekday 0 = Sunday).
TimeFields TimeToTimeFields(uint64_t time);
// RtlTimeFieldsToTime: false for fields out of range (the weekday is
// ignored).
bool TimeFieldsToTime(const TimeFields& fields, uint64_t* time);

// FileTimeToSystemTime (sub_8229B720; never fails).
SystemTime FileTimeToSystemTime(uint64_t time);
// SystemTimeToFileTime (sub_8229B680): false for an invalid date, when the
// caller sets the last error to STATUS_INVALID_PARAMETER.
bool SystemTimeToFileTime(const SystemTime& system_time, uint64_t* time);

// The next cutover of a time zone date (sub_8229B1E8): `rule` an absolute
// date or a rule (see TimeZoneInformation), `now` the time it's measured
// from, `from_this_year` whether a rule starts from now's year (the time
// zone state's way) or the next date on or after now. True with `*cutover`
// set when there is one; an absolute date's time is stored even when it's
// before now (false then). `*invalid_parameter` is set when an absolute date
// was invalid (the caller sets the last error).
bool CutoverTime(const SystemTime& rule, uint64_t now, bool from_this_year, uint64_t* cutover,
                 bool* invalid_parameter);

// GetTimeZoneInformation's state (sub_82297DC8) after the zone was read:
// with `uses_daylight` false the daylight fields are cleared first. Returns
// 0 (unknown: no dates, or no cutover), 1 (standard time) or 2 (daylight
// time) at `now`.
uint32_t TimeZoneState(TimeZoneInformation& zone, bool uses_daylight, uint64_t now,
                       bool* invalid_parameter);

// The time zone's bias (GetTimeZoneBias, sub_82297678) for a zone in a
// TimeZoneState: the zone's bias plus its standard bias (state 1) or
// daylight bias (2), nothing else for 0, and no bias at all for an error
// (-1); in 100 ns units, to subtract from UTC.
int64_t TimeZoneBias(const TimeZoneInformation& zone, uint32_t state);

}  // namespace helpers
