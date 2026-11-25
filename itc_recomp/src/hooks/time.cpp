// The game's time functions natively: adapters in front of src/helpers/time.cpp.
// The time zone comes from the console's settings as the SDK answered them
// (src/helpers/console_settings.h; the game's sub_8229B440, B5 in
// docs/rexglue-removal.md). GetLocalTime works out the bias (the game's
// sub_82297678, no longer called) itself.

#include "guest.h"
#include "hooks/native_entry.h"
#include "lifted/sdk.h"
#include "guest_call.h"
#include "last_error.h"

#include "helpers/console_settings.h"
#include "helpers/time.h"

#include <cstring>

namespace {

using guest_call::InFrame;

constexpr uint32_t kStatusInvalidParameter = 0xC000000D;

uint64_t LoadU64(uint32_t address) {
  return uint64_t(guest::LoadU32(address)) << 32 | guest::LoadU32(address + 4);
}

void StoreU64(uint32_t address, uint64_t value) {
  guest::StoreU32(address, uint32_t(value >> 32));
  guest::StoreU32(address + 4, uint32_t(value));
}

helpers::SystemTime LoadSystemTime(uint32_t address) {
  helpers::SystemTime s;
  uint16_t* fields = &s.year;
  for (uint32_t i = 0; i < 8; i++) {
    fields[i] = guest::LoadU16(address + 2 * i);
  }
  return s;
}

void StoreSystemTime(uint32_t address, const helpers::SystemTime& s) {
  const uint16_t* fields = &s.year;
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU16(address + 2 * i, fields[i]);
  }
}

// TIME_ZONE_INFORMATION (172 bytes): bias +0, standard name +4, standard
// date +68, standard bias +84, daylight name +88, daylight date +152,
// daylight bias +168.
helpers::TimeZoneInformation LoadZone(uint32_t address) {
  helpers::TimeZoneInformation zone;
  zone.bias = int32_t(guest::LoadU32(address + 0));
  zone.standard_date = LoadSystemTime(address + 68);
  zone.standard_bias = int32_t(guest::LoadU32(address + 84));
  for (uint32_t i = 0; i < 32; i++) {
    zone.daylight_name[i] = guest::LoadU16(address + 88 + 2 * i);
  }
  zone.daylight_date = LoadSystemTime(address + 152);
  zone.daylight_bias = int32_t(guest::LoadU32(address + 168));
  return zone;
}

void StoreZone(uint32_t address, const helpers::TimeZoneInformation& zone) {
  guest::StoreU32(address + 0, uint32_t(zone.bias));
  StoreSystemTime(address + 68, zone.standard_date);
  guest::StoreU32(address + 84, uint32_t(zone.standard_bias));
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU16(address + 88 + 2 * i, zone.daylight_name[i]);
  }
  StoreSystemTime(address + 152, zone.daylight_date);
  guest::StoreU32(address + 168, uint32_t(zone.daylight_bias));
}

ITC_REGS_STEP(SetInvalidParameter) {
  last_error::SetFromStatus(ctx.r13.u32, kStatusInvalidParameter);
}

// A made-up zone the time harness answers with (time_hooks::SetFakeZone).
struct FakeZone {
  bool active = false;
  bool uses_daylight = false;
  uint8_t bytes[172] = {};
} g_fake_zone;

// The console's time zone into TIME_ZONE_INFORMATION as sub_8229B440 writes
// it: the bias, each name's first four characters (a byte each, widened) and
// a terminator after them (the rest of the name left alone), each date
// cleared and its month, day of week, day and hour set, and the biases.
void StoreConsoleZone(uint32_t address, const helpers::ConsoleTimeZone& zone) {
  auto name = [&](uint32_t at, const uint8_t (&characters)[4]) {
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreU16(at + 2 * i, characters[i]);
    }
    guest::StoreU16(at + 8, 0);
  };
  auto date = [&](uint32_t at, const uint8_t (&fields)[4]) {
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(at + i, 0);
    }
    guest::StoreU16(at + 2, fields[0]);  // month
    guest::StoreU16(at + 6, fields[1]);  // day
    guest::StoreU16(at + 4, fields[2]);  // day of week
    guest::StoreU16(at + 8, fields[3]);  // hour
  };
  guest::StoreU32(address + 0, uint32_t(zone.bias));
  name(address + 4, zone.standard_name);
  name(address + 88, zone.daylight_name);
  date(address + 68, zone.standard_date);
  date(address + 152, zone.daylight_date);
  guest::StoreU32(address + 84, uint32_t(zone.standard_bias));
  guest::StoreU32(address + 168, uint32_t(zone.daylight_bias));
}

}  // namespace

namespace time_hooks {

void SetFakeZone(const uint8_t (&bytes)[172], bool uses_daylight) {
  std::memcpy(g_fake_zone.bytes, bytes, sizeof(bytes));
  g_fake_zone.uses_daylight = uses_daylight;
  g_fake_zone.active = true;
}

void ClearFakeZone() {
  g_fake_zone.active = false;
}

// A made-up time GetLocalTime answers with while a harness asks (the race's
// loading's: LoadingScreen::Show picks its tip by the milliseconds), in
// 100 ns units since 1601 (UTC); 0 for the host's.
namespace {
uint64_t g_fake_now = 0;
}  // namespace

void SetFakeNow(uint64_t now) {
  g_fake_now = now;
}

void ClearFakeNow() {
  g_fake_now = 0;
}

// The console's time zone read (sub_8229B440) into the TIME_ZONE_INFORMATION
// at `address`; returns whether it uses daylight saving. It never fails (the
// original's error, from ExGetXConfigSetting, can't happen).
bool ReadTimeZoneSettings(uint32_t address) {
  if (g_fake_zone.active) {
    for (uint32_t i = 0; i < 172; i++) {
      guest::StoreU8(address + i, g_fake_zone.bytes[i]);
    }
    return g_fake_zone.uses_daylight;
  }
  StoreConsoleZone(address, helpers::kConsoleTimeZone);
  return helpers::UsesDaylight(helpers::kRetailFlags);
}

// GetTimeZoneInformation's work (sub_82297DC8) on the zone at `address`:
// returns its state (0, 1 or 2).
uint32_t GetTimeZoneInformation(lifted::Regs& ctx, uint32_t address) {
  const bool uses_daylight = ReadTimeZoneSettings(address);
  helpers::TimeZoneInformation zone = LoadZone(address);
  bool invalid_parameter = false;
  const uint32_t state =
      helpers::TimeZoneState(zone, uses_daylight, helpers::SystemTimeNow(), &invalid_parameter);
  if (!uses_daylight) {
    StoreZone(address, zone);
  }
  if (invalid_parameter) {
    SetInvalidParameter(ctx);
  }
  return state;
}

// The time zone's bias now (the game's sub_82297678), in 100 ns units; the
// zone read into a guest buffer in a frame like the game's.
int64_t GetTimeZoneBias(lifted::Regs& ctx) {
  int64_t bias = 0;
  InFrame(ctx, 272, 0x822968DC, [&](uint32_t frame) {
    const uint32_t state = GetTimeZoneInformation(ctx, frame + 80);
    bias = helpers::TimeZoneBias(LoadZone(frame + 80), state);
  });
  return bias;
}

}  // namespace time_hooks

// GetLocalTime: r3 = the SYSTEMTIME.
ITC_NATIVE_ENTRY_ALWAYS(822968B8) {
  
  const uint32_t out = ctx.r3.u32;
  const uint64_t now = time_hooks::g_fake_now != 0 ? time_hooks::g_fake_now
                                                    : helpers::SystemTimeNow();
  const int64_t bias = time_hooks::GetTimeZoneBias(ctx);
  StoreSystemTime(out, helpers::FileTimeToSystemTime(now - uint64_t(bias)));
}

// GetTimeZoneInformation: r3 = the TIME_ZONE_INFORMATION; returns its state
// (0, 1 or 2).
ITC_NATIVE_ENTRY_ALWAYS(82297DC8) {
  
  const uint32_t state = time_hooks::GetTimeZoneInformation(ctx, ctx.r3.u32);
  ctx.r3.s64 = int32_t(state);
}

// The console's time zone read: r3 = the TIME_ZONE_INFORMATION (out), r4 =
// "uses daylight saving" (out, or 0); returns 0. Only the recompiled time
// functions the time harness runs still call it; ours call
// ReadTimeZoneSettings.
ITC_NATIVE_ENTRY_ALWAYS(8229B440) {
  
  const bool uses_daylight = time_hooks::ReadTimeZoneSettings(ctx.r3.u32);
  if (ctx.r4.u32) {
    guest::StoreU32(ctx.r4.u32, uses_daylight ? 1 : 0);
  }
  ctx.r3.u64 = 0;
}

// The next cutover of a time zone date: r3 = the date, r4 = the cutover (out),
// r5 = now, r6 = from this year (low byte); returns 1 when found.
ITC_NATIVE_ENTRY_ALWAYS(8229B1E8) {
  
  const helpers::SystemTime rule = LoadSystemTime(ctx.r3.u32);
  const uint32_t out = ctx.r4.u32;
  const uint64_t now = LoadU64(ctx.r5.u32);
  uint64_t cutover = 0;
  bool invalid_parameter = false;
  bool found = helpers::CutoverTime(rule, now, (ctx.r6.u32 & 0xFF) != 0, &cutover,
                                    &invalid_parameter);
  if (found || (rule.year != 0 && !invalid_parameter)) {
    StoreU64(out, cutover);
  }
  if (invalid_parameter) {
    SetInvalidParameter(ctx);
  }
  ctx.r3.u64 = found ? 1 : 0;
}

// SystemTimeToFileTime: r3 = the SYSTEMTIME, r4 = the FILETIME (out);
// returns 1, or 0 for an invalid date (the last error set).
ITC_NATIVE_ENTRY_ALWAYS(8229B680) {
  
  uint64_t time;
  if (!helpers::SystemTimeToFileTime(LoadSystemTime(ctx.r3.u32), &time)) {
    SetInvalidParameter(ctx);
    ctx.r3.u64 = 0;
    return;
  }
  StoreU64(ctx.r4.u32, time);
  ctx.r3.u64 = 1;
}

// FileTimeToSystemTime: r3 = the FILETIME, r4 = the SYSTEMTIME (out);
// returns 1.
ITC_NATIVE_ENTRY_ALWAYS(8229B720) {
  
  StoreSystemTime(ctx.r4.u32, helpers::FileTimeToSystemTime(LoadU64(ctx.r3.u32)));
  ctx.r3.u64 = 1;
}
