// The time functions (src/hooks/time.cpp) against the recompiled ones:
// FileTimeToSystemTime, SystemTimeToFileTime, the cutover dates, the time
// zone state and bias on made-up zones, and GetLocalTime.
//
// Made-up zones come from the override of the console's time zone read
// below, which answers with g_fake_zone while a harness sets it.

#include "harness.h"

#include "helpers/time.h"
#include "hooks/system_services_log.h"  // TEMPORARY (B5)

#include <rex/logging.h>

namespace time_hooks {
int64_t GetTimeZoneBias(PPCContext& ctx, uint8_t* base);
}

namespace {

struct FakeZone {
  bool active = false;
  uint32_t uses_daylight = 0;
  uint8_t bytes[172] = {};
} g_fake_zone;

// A zone in TIME_ZONE_INFORMATION's layout (see src/hooks/time.cpp).
struct Zone {
  int32_t bias;
  uint16_t standard_date[8];
  int32_t standard_bias;
  uint16_t daylight_date[8];
  int32_t daylight_bias;
  uint32_t uses_daylight;
  const char* name;
};

void SetFakeZone(const Zone& z) {
  uint8_t* b = g_fake_zone.bytes;
  for (int i = 0; i < 172; i++) {
    b[i] = uint8_t(0x40 + i);  // the names: a pattern
  }
  auto be32 = [&](int at, uint32_t v) {
    b[at] = uint8_t(v >> 24);
    b[at + 1] = uint8_t(v >> 16);
    b[at + 2] = uint8_t(v >> 8);
    b[at + 3] = uint8_t(v);
  };
  auto date = [&](int at, const uint16_t* f) {
    for (int i = 0; i < 8; i++) {
      b[at + 2 * i] = uint8_t(f[i] >> 8);
      b[at + 2 * i + 1] = uint8_t(f[i]);
    }
  };
  be32(0, uint32_t(z.bias));
  date(68, z.standard_date);
  be32(84, uint32_t(z.standard_bias));
  date(152, z.daylight_date);
  be32(168, uint32_t(z.daylight_bias));
  g_fake_zone.uses_daylight = z.uses_daylight;
  g_fake_zone.active = true;
}

void StoreDate(uint8_t* base, uint32_t address, const uint16_t f[8]) {
  for (int i = 0; i < 8; i++) {
    guest::StoreU16(base, address + 2 * i, f[i]);
  }
}

}  // namespace

// The console's time zone read (r3 = the zone, r4 = "uses daylight saving";
// returns 0 or a DOS error): the recompiled one, or the harness's made-up
// zone while one is set.
extern "C" REX_FUNC(sub_8229B440) {
  REX_FUNC_PROLOGUE();
  if (!g_fake_zone.active) {
    const uint32_t lr = uint32_t(ctx.lr), zone = ctx.r3.u32, uses_daylight = ctx.r4.u32;
    __imp__sub_8229B440(ctx, base);
    system_services_log::TimeZoneRead(base, lr, zone, uses_daylight, ctx.r3.u32);  // TEMPORARY (B5)
    return;
  }
  harness::Write(base, ctx.r3.u32, g_fake_zone.bytes, 172);
  guest::StoreU32(base, ctx.r4.u32, g_fake_zone.uses_daylight);
  ctx.r3.u64 = 0;
}

namespace harness {

void Time(Context& h) {
  uint8_t* base = h.base;
  const uint32_t data = h.data;
  Random& r = h.random;

  // FileTimeToSystemTime: within the years the SDK's conversions can hold
  // (they overflow system_clock's nanoseconds outside 1678-2261), then
  // outside them, reported apart: there ours are right and the SDK's not.
  const uint64_t lo = 24299136000000000ull, hi = 208591200000000000ull;  // 1678, 2262
  for (int range = 0; range < 2; range++) {
    Results results(range == 0 ? "FileTimeToSystemTime (sub_8229B720)"
                               : "FileTimeToSystemTime, outside 1678-2261 (expected to differ)");
    for (int i = 0; i < (range == 0 ? 2000 : 50); i++) {
      uint64_t time = range == 0 ? (i == 0 ? helpers::SystemTimeNow() : i == 1 ? lo : lo + r.Next() % (hi - lo))
                                 : (r.Below(2) ? r.Next() % lo : hi + (r.Next() >> 2));
      Both(h, results, [&] { StoreU64(base, data, time); },
           [&](PPCContext& t) {
             t.r3.u64 = data;
             t.r4.u64 = data + 16;
           },
           __imp__sub_8229B720, sub_8229B720, data + 16, 16, fmt::format("{:016X}", time));
    }
    results.Report();
  }

  // SystemTimeToFileTime, valid and invalid dates, the same two ranges.
  for (int range = 0; range < 2; range++) {
    Results results(range == 0 ? "SystemTimeToFileTime (sub_8229B680)"
                               : "SystemTimeToFileTime, outside 1678-2261 (expected to differ)");
    for (int i = 0; i < (range == 0 ? 2000 : 50); i++) {
      uint16_t f[8];
      f[0] = uint16_t(range == 0 ? 1678 + r.Below(584)
                                 : (r.Below(2) ? 1600 + r.Below(78) : 2262 + r.Below(100)));
      bool valid = r.Below(4) != 0;
      f[1] = uint16_t(valid ? 1 + r.Below(12) : r.Below(14));
      f[2] = uint16_t(r.Below(7));
      f[3] = uint16_t(valid ? 1 + r.Below(31) : r.Below(33));
      f[4] = uint16_t(valid ? r.Below(24) : r.Below(26));
      f[5] = uint16_t(valid ? r.Below(60) : r.Below(62));
      f[6] = uint16_t(valid ? r.Below(60) : r.Below(62));
      f[7] = uint16_t(valid ? r.Below(1000) : r.Below(1002));
      Both(h, results, [&] { StoreDate(base, data, f); },
           [&](PPCContext& t) {
             t.r3.u64 = data;
             t.r4.u64 = data + 16;
           },
           __imp__sub_8229B680, sub_8229B680, data + 16, 8,
           fmt::format("{}-{}-{} {}:{}:{}.{} weekday {}", f[0], f[1], f[3], f[4], f[5], f[6], f[7], f[2]));
    }
    results.Report();
  }

  // The cutover dates: rules (week 1-5, some invalid) and absolute dates,
  // from this year or not, against random times in 2000-2099 and now.
  {
    Results results("CutoverTime (sub_8229B1E8)");
    const uint64_t year_2000 = 125911584000000000ull;
    for (int i = 0; i < 4000; i++) {
      uint16_t f[8] = {};
      if (r.Below(5) == 0) {
        f[0] = uint16_t(1990 + r.Below(120));
        f[1] = uint16_t(r.Below(14));
        f[3] = uint16_t(r.Below(32));
      } else {
        f[1] = uint16_t(1 + r.Below(12));
        f[3] = uint16_t(r.Below(8) == 0 ? r.Below(8) : 1 + r.Below(5));
      }
      f[2] = uint16_t(r.Below(7));
      f[4] = uint16_t(r.Below(24));
      f[5] = uint16_t(r.Below(60));
      uint64_t now = r.Below(10) == 0 ? helpers::SystemTimeNow()
                                      : year_2000 + r.Next() % (100ull * 365 * 864000000000ull);
      uint32_t from_this_year = r.Below(2);
      Both(h, results,
           [&] {
             StoreDate(base, data, f);
             StoreU64(base, data + 16, now);
           },
           [&](PPCContext& t) {
             t.r3.u64 = data;
             t.r4.u64 = data + 32;
             t.r5.u64 = data + 16;
             t.r6.u64 = from_this_year;
           },
           __imp__sub_8229B1E8, sub_8229B1E8, data + 32, 8,
           fmt::format("rule {}-{}-{} weekday {} {}:{} now {:016X} from this year {}", f[0], f[1], f[3],
                       f[2], f[4], f[5], now, from_this_year));
    }
    results.Report();
  }

  // The time zone state and bias on made-up zones.
  const helpers::SystemTime today = helpers::FileTimeToSystemTime(helpers::SystemTimeNow());
  const uint16_t y = today.year;
  const Zone zones[] = {
      {480, {0, 11, 0, 1, 2, 0, 0, 0}, 0, {0, 3, 0, 2, 2, 0, 0, 0}, -60, 1, "US Pacific"},
      {-600, {0, 4, 0, 1, 3, 0, 0, 0}, 0, {0, 10, 0, 1, 2, 0, 0, 0}, -60, 1, "Sydney"},
      {0, {0, 10, 0, 5, 2, 0, 0, 0}, 30, {0, 3, 0, 5, 1, 0, 0, 0}, -60, 1, "London, standard bias 30"},
      {300, {0, 11, 0, 1, 2, 0, 0, 0}, 0, {0, 3, 0, 2, 2, 0, 0, 0}, -60, 0, "rules but no daylight"},
      {-60, {uint16_t(y + 1), 1, 0, 1, 0, 0, 0, 0}, 0, {uint16_t(y - 1), 6, 0, 1, 0, 0, 0, 0}, -60, 1,
       "absolute, one past"},
      {-60, {uint16_t(y + 1), 1, 0, 1, 0, 0, 0, 0}, 0, {uint16_t(y + 1), 6, 0, 1, 0, 0, 0, 0}, -60, 1,
       "absolute, both future"},
      {-60, {uint16_t(y + 1), 13, 0, 1, 0, 0, 0, 0}, 0, {uint16_t(y + 1), 6, 0, 1, 0, 0, 0, 0}, -60, 1,
       "absolute, invalid"},
      {0, {0, 0, 0, 0, 0, 0, 0, 0}, 0, {0, 3, 0, 2, 2, 0, 0, 0}, -60, 1, "no standard date"},
      {0, {0, 11, 0, 7, 2, 0, 0, 0}, 0, {0, 3, 0, 2, 2, 0, 0, 0}, -60, 1, "week 7"},
      {0, {0, today.month, today.day_of_week, 1, 0, 0, 0, 0}, 0,
       {0, uint16_t(today.month % 12 + 1), 0, 5, 23, 59, 0, 0}, 120, 1, "this month"},
      {0x7FFFFFFF, {0, 11, 0, 1, 2, 0, 0, 0}, 5, {0, 3, 0, 2, 2, 0, 0, 0}, 0x40000000, 1,
       "biases overflowing 32 bits"},
  };
  {
    Results results("TimeZoneState (sub_82297DC8)");
    for (const Zone& z : zones) {
      SetFakeZone(z);
      Both(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = data; }, __imp__sub_82297DC8,
           sub_82297DC8, data, 172, z.name);
      g_fake_zone.active = false;
    }
    results.Report();
  }
  {
    Results results("GetTimeZoneBias (sub_82297678)");
    for (const Zone& z : zones) {
      SetFakeZone(z);
      // The recompiled one leaves the bias at data; ours returns it.
      std::vector<uint8_t> result[2];
      Fill(base, data, kScratchBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = data;
      __imp__sub_82297678(t, base);
      result[0] = Read(base, data, 8);
      t = CallContext(h);
      const uint64_t ours = uint64_t(time_hooks::GetTimeZoneBias(t, base));
      for (int i = 0; i < 8; i++) {
        result[1].push_back(uint8_t(ours >> (56 - 8 * i)));
      }
      g_fake_zone.active = false;
      results.Check(result[0], result[1], z.name);
    }
    results.Report();
  }

  // GetLocalTime: the two read the clock apart, so the milliseconds aren't
  // compared.
  {
    Results results("GetLocalTime (sub_822968B8), to the second");
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(base, data, kScratchBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = data;
      (v == 0 ? __imp__sub_822968B8 : sub_822968B8)(t, base);
      result[v] = Read(base, data, 14);
    }
    results.Check(result[0], result[1], "now");
    results.Report();
  }
}

}  // namespace harness
