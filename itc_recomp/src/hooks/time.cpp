// The game's time functions natively: adapters in front of src/helpers/time.cpp.
// One thing is still a guest call: the time zone read from the console's
// settings (sub_8229B440, ExGetXConfigSetting: B5 in docs/rexglue-removal.md).
// GetLocalTime works out the bias (the game's sub_82297678, no longer
// called) itself.

#include "guest.h"
#include "guest_call.h"
#include "last_error.h"

#include "helpers/time.h"

namespace {

using guest_call::InFrame;

constexpr uint32_t kStatusInvalidParameter = 0xC000000D;

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return uint64_t(guest::LoadU32(base, address)) << 32 | guest::LoadU32(base, address + 4);
}

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

helpers::SystemTime LoadSystemTime(uint8_t* base, uint32_t address) {
  helpers::SystemTime s;
  uint16_t* fields = &s.year;
  for (uint32_t i = 0; i < 8; i++) {
    fields[i] = guest::LoadU16(base, address + 2 * i);
  }
  return s;
}

void StoreSystemTime(uint8_t* base, uint32_t address, const helpers::SystemTime& s) {
  const uint16_t* fields = &s.year;
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU16(base, address + 2 * i, fields[i]);
  }
}

// TIME_ZONE_INFORMATION (172 bytes): bias +0, standard name +4, standard
// date +68, standard bias +84, daylight name +88, daylight date +152,
// daylight bias +168.
helpers::TimeZoneInformation LoadZone(uint8_t* base, uint32_t address) {
  helpers::TimeZoneInformation zone;
  zone.bias = int32_t(guest::LoadU32(base, address + 0));
  zone.standard_date = LoadSystemTime(base, address + 68);
  zone.standard_bias = int32_t(guest::LoadU32(base, address + 84));
  for (uint32_t i = 0; i < 32; i++) {
    zone.daylight_name[i] = guest::LoadU16(base, address + 88 + 2 * i);
  }
  zone.daylight_date = LoadSystemTime(base, address + 152);
  zone.daylight_bias = int32_t(guest::LoadU32(base, address + 168));
  return zone;
}

void StoreZone(uint8_t* base, uint32_t address, const helpers::TimeZoneInformation& zone) {
  guest::StoreU32(base, address + 0, uint32_t(zone.bias));
  StoreSystemTime(base, address + 68, zone.standard_date);
  guest::StoreU32(base, address + 84, uint32_t(zone.standard_bias));
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU16(base, address + 88 + 2 * i, zone.daylight_name[i]);
  }
  StoreSystemTime(base, address + 152, zone.daylight_date);
  guest::StoreU32(base, address + 168, uint32_t(zone.daylight_bias));
}

void SetInvalidParameter(PPCContext& ctx, uint8_t* base) {
  last_error::SetFromStatus(base, ctx.r13.u32, kStatusInvalidParameter);
}

}  // namespace

namespace time_hooks {

// GetTimeZoneInformation's work (sub_82297DC8) on the zone at `address`:
// returns its state (0, 1 or 2), or -1 when the console's settings can't be
// read (the last error set).
uint32_t GetTimeZoneInformation(PPCContext& ctx, uint8_t* base, uint32_t address) {
  uint32_t error = 0, uses_daylight = 0;
  InFrame(ctx, base, 144, 0x82297DEC, [&](uint32_t frame) {
    ctx.r3.u64 = address;
    ctx.r4.u64 = frame + 80;
    sub_8229B440(ctx, base);
    error = ctx.r3.u32;
    uses_daylight = guest::LoadU32(base, frame + 80);
  });
  if (error) {
    last_error::Set(base, ctx.r13.u32, error);
    return 0xFFFFFFFF;
  }
  helpers::TimeZoneInformation zone = LoadZone(base, address);
  bool invalid_parameter = false;
  const uint32_t state =
      helpers::TimeZoneState(zone, uses_daylight != 0, helpers::SystemTimeNow(), &invalid_parameter);
  if (!uses_daylight) {
    StoreZone(base, address, zone);
  }
  if (invalid_parameter) {
    SetInvalidParameter(ctx, base);
  }
  return state;
}

// The time zone's bias now (the game's sub_82297678), in 100 ns units; the
// zone read into a guest buffer in a frame like the game's.
int64_t GetTimeZoneBias(PPCContext& ctx, uint8_t* base) {
  int64_t bias = 0;
  InFrame(ctx, base, 272, 0x822968DC, [&](uint32_t frame) {
    const uint32_t state = GetTimeZoneInformation(ctx, base, frame + 80);
    bias = helpers::TimeZoneBias(LoadZone(base, frame + 80), state);
  });
  return bias;
}

}  // namespace time_hooks

// GetLocalTime: r3 = the SYSTEMTIME.
extern "C" REX_FUNC(sub_822968B8) {
  REX_FUNC_PROLOGUE();
  const uint32_t out = ctx.r3.u32;
  const uint64_t now = helpers::SystemTimeNow();
  const int64_t bias = time_hooks::GetTimeZoneBias(ctx, base);
  StoreSystemTime(base, out, helpers::FileTimeToSystemTime(now - uint64_t(bias)));
}

// GetTimeZoneInformation: r3 = the TIME_ZONE_INFORMATION; returns its state
// (0, 1 or 2), or -1 when the console's settings can't be read.
extern "C" REX_FUNC(sub_82297DC8) {
  REX_FUNC_PROLOGUE();
  const uint32_t state = time_hooks::GetTimeZoneInformation(ctx, base, ctx.r3.u32);
  ctx.r3.s64 = int32_t(state);
}

// The next cutover of a time zone date: r3 = the date, r4 = the cutover (out),
// r5 = now, r6 = from this year (low byte); returns 1 when found.
extern "C" REX_FUNC(sub_8229B1E8) {
  REX_FUNC_PROLOGUE();
  const helpers::SystemTime rule = LoadSystemTime(base, ctx.r3.u32);
  const uint32_t out = ctx.r4.u32;
  const uint64_t now = LoadU64(base, ctx.r5.u32);
  uint64_t cutover = 0;
  bool invalid_parameter = false;
  bool found = helpers::CutoverTime(rule, now, (ctx.r6.u32 & 0xFF) != 0, &cutover,
                                    &invalid_parameter);
  if (found || (rule.year != 0 && !invalid_parameter)) {
    StoreU64(base, out, cutover);
  }
  if (invalid_parameter) {
    SetInvalidParameter(ctx, base);
  }
  ctx.r3.u64 = found ? 1 : 0;
}

// SystemTimeToFileTime: r3 = the SYSTEMTIME, r4 = the FILETIME (out);
// returns 1, or 0 for an invalid date (the last error set).
extern "C" REX_FUNC(sub_8229B680) {
  REX_FUNC_PROLOGUE();
  uint64_t time;
  if (!helpers::SystemTimeToFileTime(LoadSystemTime(base, ctx.r3.u32), &time)) {
    SetInvalidParameter(ctx, base);
    ctx.r3.u64 = 0;
    return;
  }
  StoreU64(base, ctx.r4.u32, time);
  ctx.r3.u64 = 1;
}

// FileTimeToSystemTime: r3 = the FILETIME, r4 = the SYSTEMTIME (out);
// returns 1.
extern "C" REX_FUNC(sub_8229B720) {
  REX_FUNC_PROLOGUE();
  StoreSystemTime(base, ctx.r4.u32, helpers::FileTimeToSystemTime(LoadU64(base, ctx.r3.u32)));
  ctx.r3.u64 = 1;
}
