// The AI drivers (src/ai_drivers/, docs/functions/ai-drivers.md) against
// the recompiled functions. Each case fills a block from the allocation
// layer with random words (mostly floats: NaN, infinite and zero ones among
// them): a driver at +0 (6,160 bytes: its path at +512, its skill at
// +6048), a vehicle at +0x2000, the inputs at +0x4000, an output at
// +0x4100, a route file at +0x4200 and the vehicle's box shape at +0x4300;
// then the fields the function reads set to the cases it tells apart. Both
// versions run on a copy of the same bytes; the block (in 64-byte pieces,
// the float outputs as floats), r3 and f1 (for the functions returning a
// float) are compared. The drivers' table ([0x826C6074]) and the course's
// route ([0x826C5B10]) are set for each case and put back after. The
// originals run with --native_ai_drivers off.

#include "harness.h"

#include "ai_drivers/ai_drivers.h"
#include "base/flags.h"
#include "base/log.h"
#include "memory/heap.h"
#include "track_route/track_route.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_ai_drivers);

namespace harness {

namespace {

namespace ai = ai_drivers;

// --- The register steps, as the adapters (src/hooks/ai_drivers.cpp) ---

#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeInitPath, ai::InitPath(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeOf, ctx.r3.u64 = ai::Of(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearCarInput, ai::ClearCarInput(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearFollow, ai::ClearFollow(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearOtherIndexes, ai::ClearOtherIndexes(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearCorners, ai::ClearCorners(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearCornerKind, ai::ClearCornerKind(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeResetSkill, ai::ResetSkill(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeWrapAngle, ai::WrapAngle(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeTimeFromTicks, ctx.r3.u64 = ai::TimeFromTicks(ctx.r3.u32))
ITC_NATIVE_STEP(NativeSetFlag2, ctx.r3.u64 = ai::SetFlag2(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeIsFree, ctx.r3.u64 = ai::IsFree(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSpeedOverDistance,
                ctx.f1.f64 = ai::SpeedOverDistance(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeCornerFactor,
                ctx.f1.f64 = ai::CornerFactor(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeReadCar, ctx.r3.u64 = ai::ReadCar(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateHalfWidth, ai::UpdateHalfWidth(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeTargetPoint,
                ctx.r3.u64 = ai::TargetPoint(base, ctx.r3.u32, ctx.f1.f64, ctx.r4.u32))
ITC_NATIVE_STEP(NativeClearSkill, ai::ClearSkill(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSetSkillSource, ai::SetSkillSource(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeLoadSkill, ai::LoadSkill(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeLimitSpeed,
                ctx.f1.f64 = ai::LimitSpeed(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                            ctx.r7.u32, ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeLimitByCurve, ai::LimitByCurve(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32, ctx.r7.u32))
ITC_NATIVE_STEP(NativeEaseThrottleForSteer, ai::EaseThrottleForSteer(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeHasKind2Ahead, ctx.r3.u64 = ai::HasKind2Ahead(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeIsSlow, ctx.r3.u64 = ai::IsSlow(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativePreferredSections,
                ctx.r3.s64 = int32_t(ai::PreferredSections(base, ctx.r3.u32, ctx.r4.u32)))
ITC_NATIVE_STEP(NativeRaceAllowsSides, ctx.r3.u64 = ai::RaceAllowsSides(base))
ITC_NATIVE_STEP(NativeRefreshPoints, ai::RefreshPoints(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeResetPath, ctx.r3.u64 = ai::ResetPath(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSetLaneOffset, ai::SetLaneOffset(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateOtherFlags, ai::UpdateOtherFlags(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateOtherTimer,
                ai::UpdateOtherTimer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeClearOthers, ai::ClearOthers(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativePickTarget, ai::PickTarget(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativePickTargetBehind, ai::PickTargetBehind(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeKeepsGapFrom, ctx.r3.u64 = ai::KeepsGapFrom(base, ctx.r3.u32, ctx.r4.u32))

// Runs the recompiled version with the native flag off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  const bool saved = ITC_FLAG(native_ai_drivers);
  ITC_FLAG(native_ai_drivers) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_ai_drivers) = saved;
}

// The block's layout.
constexpr uint32_t kAreaBytes = 0x4400;
constexpr uint32_t kDriver = 0, kPath = 512, kSkill = 6048;
constexpr uint32_t kVehicle = 0x2000, kIo = 0x4000, kOut = 0x4100, kFile = 0x4200,
                   kShape = 0x4300, kRace = 0x4380, kRaceState = 0x4390, kInputWords = 0x43C0;
// The race data ([0x826C5B2C], its words +0xA3AF0 and +0xA3AF4 at kRace),
// the race state ([0x826D82E8], its +6100 at kRaceState) and the racers'
// inputs ([0x826C5B30], eight words pointing at kInputWords).
constexpr uint32_t kRaceData = 0x826C5B2C, kRaceStateGlobal = 0x826D82E8, kInputs = 0x826C5B30;
// The drivers' table for Of and KeepsGapFrom: 64 drivers.
constexpr uint32_t kTableDrivers = 64;

// A float: now and then NaN (any sign, payload), an infinity or a zero.
uint32_t RandomFloat(Random& r, float range) {
  switch (r.Below(32)) {
    case 0:
      return 0x7F800001u | (uint32_t(r.Next()) & 0x803FFFFFu) | 0x00400000u;
    case 1:
      return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 2:
      return r.Below(2) ? 0u : 0x80000000u;
    default: {
      const float unit = float(r.Below(1u << 24)) / float(1u << 24);
      return std::bit_cast<uint32_t>((unit * 2.0f - 1.0f) * range);
    }
  }
}
uint32_t FloatBits(float value) {
  return std::bit_cast<uint32_t>(value);
}

enum Kind {
  kInitPath,
  kOf,
  kClearCarInput,
  kClearFollow,
  kClearOtherIndexes,
  kClearCorners,
  kClearCornerKind,
  kResetSkill,
  kWrapAngle,
  kTimeFromTicks,
  kSetFlag2,
  kIsFree,
  kSpeedOverDistance,
  kCornerFactor,
  kReadCar,
  kUpdateHalfWidth,
  kTargetPoint,
  kClearSkill,
  kSetSkillSource,
  kLoadSkill,
  kLimitSpeed,
  kLimitByCurve,
  kEaseThrottleForSteer,
  kHasKind2Ahead,
  kIsSlow,
  kPreferredSections,
  kRaceAllowsSides,
  kRefreshPoints,
  kResetPath,
  kSetLaneOffset,
  kUpdateOtherFlags,
  kUpdateOtherTimer,
  kClearOthers,
  kPickTarget,
  kPickTargetBehind,
  kKeepsGapFrom,
};

}  // namespace

void AiDrivers(Context& h) {
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
    int cases;
  } functions[] = {
      {"AiDriver::InitPath (sub_8213AD48)", Original<__imp__sub_8213AD48>, NativeInitPath,
       kInitPath, 200},
      {"AiDriver::Of (sub_82139728)", Original<__imp__sub_82139728>, NativeOf, kOf, 1000},
      {"CarInput::Clear (sub_820C96E8)", Original<__imp__sub_820C96E8>, NativeClearCarInput,
       kClearCarInput, 200},
      {"AiDriver::ClearFollow (sub_82146508)", Original<__imp__sub_82146508>,
       NativeClearFollow, kClearFollow, 200},
      {"AiDriver::ClearOtherIndexes (sub_82144ED8)", Original<__imp__sub_82144ED8>,
       NativeClearOtherIndexes, kClearOtherIndexes, 200},
      {"AiPath::ClearCorners (sub_8213C720)", Original<__imp__sub_8213C720>,
       NativeClearCorners, kClearCorners, 200},
      {"AiPath::ClearCornerKind (sub_8213C710)", Original<__imp__sub_8213C710>,
       NativeClearCornerKind, kClearCornerKind, 200},
      {"AiSkill::Reset (sub_8213F698)", Original<__imp__sub_8213F698>, NativeResetSkill,
       kResetSkill, 200},
      {"Angle::Wrap (sub_82145408)", Original<__imp__sub_82145408>, NativeWrapAngle,
       kWrapAngle, 3000},
      {"RaceTime::FromTicks (sub_82101760)", Original<__imp__sub_82101760>,
       NativeTimeFromTicks, kTimeFromTicks, 3000},
      {"AiDriver::SetFlag2 (sub_82138D20)", Original<__imp__sub_82138D20>, NativeSetFlag2,
       kSetFlag2, 1000},
      {"AiDriver::IsFree (sub_821464B8)", Original<__imp__sub_821464B8>, NativeIsFree, kIsFree,
       2000},
      {"AiDriver::SpeedOverDistance (sub_821450E0)", Original<__imp__sub_821450E0>,
       NativeSpeedOverDistance, kSpeedOverDistance, 3000},
      {"AiDriver::CornerFactor (sub_821453A0)", Original<__imp__sub_821453A0>,
       NativeCornerFactor, kCornerFactor, 3000},
      {"AiDriver::ReadCar (sub_82144F50)", Original<__imp__sub_82144F50>, NativeReadCar,
       kReadCar, 3000},
      {"AiDriver::UpdateHalfWidth (sub_82145230)", Original<__imp__sub_82145230>,
       NativeUpdateHalfWidth, kUpdateHalfWidth, 3000},
      {"AiPath::TargetPoint (sub_8213B4D8)", Original<__imp__sub_8213B4D8>, NativeTargetPoint,
       kTargetPoint, 3000},
      {"AiSkill::Clear (sub_8213F700)", Original<__imp__sub_8213F700>, NativeClearSkill,
       kClearSkill, 200},
      {"AiSkill::SetSource (sub_8213F900)", Original<__imp__sub_8213F900>,
       NativeSetSkillSource, kSetSkillSource, 500},
      {"AiSkill::Load (sub_8213F730)", Original<__imp__sub_8213F730>, NativeLoadSkill,
       kLoadSkill, 1000},
      {"AiDriver::LimitSpeed (sub_82139248)", Original<__imp__sub_82139248>, NativeLimitSpeed,
       kLimitSpeed, 3000},
      {"AiDriver::LimitByCurve (sub_821390D0)", Original<__imp__sub_821390D0>,
       NativeLimitByCurve, kLimitByCurve, 3000},
      {"AiDriver::EaseThrottleForSteer (sub_82139B98)", Original<__imp__sub_82139B98>,
       NativeEaseThrottleForSteer, kEaseThrottleForSteer, 3000},
      {"AiPath::HasKind2Ahead (sub_8213C148)", Original<__imp__sub_8213C148>,
       NativeHasKind2Ahead, kHasKind2Ahead, 2000},
      {"AiPath::IsSlow (sub_8213C0E8)", Original<__imp__sub_8213C0E8>, NativeIsSlow, kIsSlow,
       2000},
      {"AiPath::PreferredSections (sub_8213C358)", Original<__imp__sub_8213C358>,
       NativePreferredSections, kPreferredSections, 2000},
      {"AiPath::RaceAllowsSides (sub_8213D318)", Original<__imp__sub_8213D318>,
       NativeRaceAllowsSides, kRaceAllowsSides, 1000},
      {"AiPath::RefreshPoints (sub_8213B5D8)", Original<__imp__sub_8213B5D8>,
       NativeRefreshPoints, kRefreshPoints, 2000},
      {"AiPath::Reset (sub_8213D438)", Original<__imp__sub_8213D438>, NativeResetPath,
       kResetPath, 300},
      {"AiPathEntry::SetLaneOffset (sub_8213B390)", Original<__imp__sub_8213B390>,
       NativeSetLaneOffset, kSetLaneOffset, 3000},
      {"AiOther::UpdateFlags (sub_82145450)", Original<__imp__sub_82145450>,
       NativeUpdateOtherFlags, kUpdateOtherFlags, 2000},
      {"AiOther::UpdateTimer (sub_821456A0)", Original<__imp__sub_821456A0>,
       NativeUpdateOtherTimer, kUpdateOtherTimer, 3000},
      {"AiDriver::ClearOthers (sub_821459B0)", Original<__imp__sub_821459B0>,
       NativeClearOthers, kClearOthers, 200},
      {"AiDriver::PickTarget (sub_82145118)", Original<__imp__sub_82145118>, NativePickTarget,
       kPickTarget, 3000},
      {"AiDriver::PickTargetBehind (sub_821451B8)", Original<__imp__sub_821451B8>,
       NativePickTargetBehind, kPickTargetBehind, 3000},
      {"AiDriver::KeepsGapFrom (sub_82145048)", Original<__imp__sub_82145048>,
       NativeKeepsGapFrom, kKeepsGapFrom, 3000},
  };
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  const uint32_t driver = area + kDriver, vehicle = area + kVehicle, io = area + kIo;
  const uint32_t saved_drivers = guest::LoadU32(h.base, ai::kDrivers);
  const uint32_t saved_route = guest::LoadU32(h.base, track_route::kRoute);
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceData);
  const uint32_t saved_state = guest::LoadU32(h.base, kRaceStateGlobal);
  uint32_t saved_inputs[8];
  for (uint32_t i = 0; i < 8; i++) {
    saved_inputs[i] = guest::LoadU32(h.base, kInputs + 4 * i);
  }
  const uint32_t table = memory::Alloc(kTableDrivers * ai::kDriverBytes, 0);
  {
    Random fill;
    for (uint32_t i = 0; i < kTableDrivers * ai::kDriverBytes; i += 4) {
      guest::StoreU32(h.base, table + i, uint32_t(fill.Next()));
    }
  }
  Random r;
  for (const auto& f : functions) {
    Results results(f.name);
    for (int c = 0; c < f.cases; c++) {
      // Random words, mostly floats.
      for (uint32_t i = 0; i < kAreaBytes; i += 4) {
        guest::StoreU32(h.base, area + i,
                        r.Below(4) ? RandomFloat(r, 200.0f) : uint32_t(r.Next()));
      }
      // The driver's vehicle (now and then none) and its box shape.
      guest::StoreU32(h.base, driver + 160, r.Below(8) == 0 ? 0 : vehicle);
      guest::StoreU32(h.base, vehicle + 344, r.Below(4) == 0 ? 0 : area + kShape);
      const uint32_t flags[] = {0, 1, 0x80, 0x81, 0x100, uint32_t(r.Next())};
      guest::StoreU32(h.base, vehicle + 6148, flags[r.Below(6)]);
      guest::StoreU32(h.base, driver + 16, r.Below(2) ? uint32_t(r.Next()) : r.Below(4));
      guest::StoreU32(h.base, driver + 32, r.Below(8) ? r.Below(8) : uint32_t(r.Next()));
      uint32_t drivers = saved_drivers;
      uint64_t r3 = driver, r4 = 0, r5 = 0, r6 = 0, r7 = 0;
      uint64_t f1 = 0x7FF8DEADBEEF0000ull;
      std::vector<std::pair<uint32_t, uint32_t>> floats;  // (offset, count) in the block
      std::string what = fmt::format("case {}", c);
      switch (f.kind) {
        case kInitPath:
        case kClearFollow:
        case kClearOtherIndexes:
          break;
        case kOf: {
          const uint32_t tables[] = {0, table, 0x80000000u, uint32_t(r.Next())};
          drivers = tables[r.Below(4)];
          guest::StoreU32(h.base, io, r.Below(4) ? r.Below(8) : uint32_t(r.Next()));
          r3 = io;
          break;
        }
        case kClearCarInput:
        case kClearCornerKind:
          r3 = io;
          break;
        case kClearCorners:
          r3 = driver + kPath;
          break;
        case kResetSkill:
          r3 = driver + kSkill;
          break;
        case kWrapAngle: {
          const float edges[] = {3.14159265f, -3.14159265f, 3.1415927f, -3.1415927f,
                                 6.2831855f, -6.2831855f, 9.5f, -9.5f, 0.0f};
          guest::StoreU32(h.base, io, r.Below(4) == 0 ? FloatBits(edges[r.Below(9)])
                                                      : RandomFloat(r, 10.0f));
          r3 = io;
          floats.push_back({kIo, 1});
          break;
        }
        case kTimeFromTicks: {
          const uint32_t ticks[] = {0, 1, 29, 30, 1799, 1800, 0xFFFFFFFFu, 0x80000000u,
                                    0x7FFFFFFFu, 1800u * 32768u, 1800u * 32767u + 1799u};
          const uint32_t low = r.Below(4) == 0  ? ticks[r.Below(11)]
                               : r.Below(2) == 0 ? uint32_t(int32_t(r.Below(200000)) - 100000)
                                                 : uint32_t(r.Next());
          r3 = (r.Below(2) ? (r.Next() << 32) : 0) | low;
          break;
        }
        case kSetFlag2:
          guest::StoreU32(h.base, io, r.Below(2) ? 0 : uint32_t(r.Next()));
          r4 = io;
          break;
        case kIsFree:
          break;
        case kSpeedOverDistance:
          guest::StoreU32(h.base, driver + 100,
                          r.Below(8) == 0 ? FloatBits(100.0f) : RandomFloat(r, 300.0f));
          r4 = io;
          break;
        case kCornerFactor:
          guest::StoreU32(h.base, io, r.Below(8) == 0 ? FloatBits(10.0f) : RandomFloat(r, 60.0f));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 10.0f));
          guest::StoreU32(h.base, driver + 128, RandomFloat(r, 10.0f));
          r4 = io;
          r5 = io + 4;
          break;
        case kReadCar:
          floats.push_back({kDriver + 116, 7});
          break;
        case kUpdateHalfWidth:
          guest::StoreU32(h.base, driver + 148, RandomFloat(r, 7.0f));
          floats.push_back({kDriver + 116, 1});
          break;
        case kTargetPoint: {
          guest::StoreU32(h.base, area + kFile + 8,
                          r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                           : FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          const int32_t count = int32_t(r.Below(85)) - 2;
          const int32_t k = r.Below(4) == 0 ? int32_t(r.Below(90)) - 4 : int32_t(r.Below(80));
          const int32_t start = r.Below(8) == 0 ? int32_t(int16_t(r.Next()))
                                                : int32_t(r.Below(200)) - 60;
          guest::StoreU16(h.base, driver + kPath + 5136, uint16_t(count));
          guest::StoreU16(h.base, driver + kPath + 5142, uint16_t(k));
          guest::StoreU16(h.base, driver + kPath + 5138, uint16_t(start));
          r3 = driver + kPath;
          r4 = area + kOut;
          // The distance ahead: near the look-ahead entry's, now and then
          // anywhere (a float, as the callers pass one).
          const float length = std::bit_cast<float>(guest::LoadU32(h.base, area + kFile + 8));
          const float ahead = r.Below(8) == 0
                                  ? std::bit_cast<float>(RandomFloat(r, 100.0f))
                                  : (float(k) + float(r.Below(1200)) / 1000.0f - 0.1f) * length;
          f1 = std::bit_cast<uint64_t>(double(ahead));
          floats.push_back({kOut, 3});
          what += fmt::format(" count {} k {} start {}", count, k, start);
          break;
        }
        case kClearSkill:
          r3 = driver + kSkill;
          break;
        case kSetSkillSource:
          r3 = driver + kSkill;
          r4 = r.Below(2) ? 0 : io;
          break;
        case kLoadSkill:
          r3 = driver + kSkill;
          guest::StoreU32(h.base, driver + kSkill, r.Below(2) ? 0 : io);
          guest::StoreU32(h.base, area + kRace, r.Below(2) ? 0 : uint32_t(r.Next()));
          break;
        case kLimitSpeed:
          guest::StoreU32(h.base, area + kFile + 8,
                          r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                           : FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 60.0f));
          guest::StoreU32(h.base, io + 8, RandomFloat(r, 10.0f));
          guest::StoreU32(h.base, io + 12,
                          r.Below(4) ? uint32_t(int32_t(r.Below(44)) - 3) : uint32_t(r.Next()));
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 300.0f));
          r4 = io;
          r5 = io + 4;
          r6 = io + 8;
          r7 = io + 12;
          floats.push_back({kDriver + 6088, 1});
          break;
        case kLimitByCurve: {
          guest::StoreU32(h.base, io + 0x20, io + 0x30);
          guest::StoreU32(h.base, io + 0x24, io + 0x40);
          guest::StoreU16(h.base, io + 0x30, uint16_t(int32_t(r.Below(10)) - 1));
          guest::StoreU32(h.base, io + 4,
                          r.Below(8) ? uint32_t(int32_t(r.Below(12)) - 2) : uint32_t(r.Next()));
          guest::StoreU32(h.base, driver + 144, RandomFloat(r, 30.0f));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU32(h.base, io + 0x44 + 8 * i, RandomFloat(r, 30.0f));
          }
          guest::StoreU32(h.base, io + 0x80 + 32, RandomFloat(r, 30.0f));
          guest::StoreU32(h.base, io + 0x80 + 36, RandomFloat(r, 30.0f));
          guest::StoreU32(h.base, io, RandomFloat(r, 1.5f));
          r4 = io + 0x20;
          r5 = io;
          r6 = io + 4;
          r7 = r.Below(8) == 0 ? 0 : io + 0x80;
          floats.push_back({kIo, 1});
          break;
        }
        case kEaseThrottleForSteer:
          guest::StoreU32(h.base, driver + 6140, RandomFloat(r, 2.0f));
          guest::StoreU32(h.base, driver + 152, RandomFloat(r, 20.0f));
          floats.push_back({kDriver + 6140, 1});
          break;
        case kHasKind2Ahead:
        case kIsSlow:
          guest::StoreU16(h.base, driver + kPath + 5162, uint16_t(int32_t(r.Below(11)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t record = driver + kPath + 5164 + 44 * i;
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(7)) - 4));
            guest::StoreU32(h.base, record + 40, r.Below(8) ? r.Below(6) : uint32_t(r.Next()));
          }
          r3 = driver + kPath;
          break;
        case kPreferredSections:
          guest::StoreU8(h.base, io, uint8_t(r.Below(4) ? int32_t(r.Below(14)) - 2 : r.Below(256)));
          guest::StoreU8(h.base, io + 1,
                         uint8_t(r.Below(4) ? int32_t(r.Below(16)) - 3 : r.Below(256)));
          r3 = io;
          r4 = io + 1;
          break;
        case kRaceAllowsSides:
          guest::StoreU32(h.base, area + kRace + 4, r.Below(8) ? r.Below(10) : uint32_t(r.Next()));
          guest::StoreU32(h.base, area + kRaceState, r.Below(8) ? r.Below(5) : uint32_t(r.Next()));
          break;
        case kRefreshPoints:
          guest::StoreU16(h.base, driver + kPath + 5140, uint16_t(int32_t(r.Below(84)) - 1));
          guest::StoreU16(h.base, driver + kPath + 5138, uint16_t(r.Below(200)));
          for (uint32_t k = 0; k < 80; k++) {
            const uint32_t entry = driver + kPath + 16 + 64 * k;
            guest::StoreU32(h.base, entry + 52, vehicle + 84 * r.Below(60));
            floats.push_back({kDriver + kPath + 16 + 64 * k, 3});
          }
          r3 = driver + kPath;
          break;
        case kResetPath:
          guest::StoreU32(h.base, driver + kPath, r.Below(4) == 0 ? 0 : driver);
          r3 = driver + kPath;
          break;
        case kSetLaneOffset:
          guest::StoreU32(h.base, io + 0x100 + 52, io + 0x180);
          guest::StoreU32(h.base, io + 0x100 + 20,
                          r.Below(8) ? uint32_t(int32_t(r.Below(22)) - 3) : uint32_t(r.Next()));
          guest::StoreU8(h.base, io + 0x180 + 56, uint8_t(r.Below(256)));
          guest::StoreU32(h.base, io + 0x180 + 12, RandomFloat(r, 20.0f));
          r3 = io + 0x100;
          floats.push_back({kIo + 0x100 + 24, 1});
          floats.push_back({kIo + 0x100 + 48, 1});
          break;
        case kUpdateOtherFlags:
          guest::StoreU16(h.base, io + 24, uint16_t(r.Below(4) ? r.Below(5) : r.Next()));
          r3 = io;
          break;
        case kUpdateOtherTimer:
          guest::StoreU32(h.base, io + 0, RandomFloat(r, 60.0f));
          guest::StoreU32(h.base, io + 0x40, RandomFloat(r, 30.0f));
          guest::StoreU32(h.base, io + 0x44, (uint32_t(r.Next()) & 0xFFFFFF00u) | r.Below(8));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU32(h.base, area + kInputWords + 4 * i,
                            r.Below(2) ? 0x20000u | uint32_t(r.Next()) : uint32_t(r.Next()));
          }
          r3 = io;
          r4 = io + 0x44;
          r5 = io + 0x40;
          break;
        case kClearOthers:
          break;
        case kPickTarget:
        case kPickTargetBehind: {
          guest::StoreU32(h.base, driver + 0, r.Below(8) ? r.Below(8) : uint32_t(r.Next()));
          guest::StoreU16(h.base, driver + 40, uint16_t(r.Below(2) ? 0x7F : r.Next()));
          const uint32_t current[] = {0xFFFFFFFFu, 0, 3, 6, uint32_t(r.Next())};
          guest::StoreU32(h.base, driver + 480, current[r.Below(5)]);
          break;
        }
        case kKeepsGapFrom:
          guest::StoreU32(h.base, driver + 32, r.Below(8) ? r.Below(6) : uint32_t(r.Next()));
          guest::StoreU32(h.base, io, r.Below(8) ? r.Below(7) : 32 + r.Below(8));
          guest::StoreU16(h.base, driver + 40, uint16_t(r.Below(2) ? r.Below(0x80) : r.Next()));
          for (uint32_t i = 0; i < 40; i++) {
            guest::StoreU32(h.base, table + i * ai::kDriverBytes + 32, r.Below(6));
          }
          drivers = table;
          r4 = io;
          break;
      }
      const std::vector<uint8_t> inputs = Read(h.base, area, kAreaBytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, area, inputs.data(), kAreaBytes);
        guest::StoreU32(h.base, ai::kDrivers, drivers);
        guest::StoreU32(h.base, track_route::kRoute, area + kFile);
        guest::StoreU32(h.base, kRaceData, area + kRace - 0xA3AF0);
        guest::StoreU32(h.base, kRaceStateGlobal, area + kRaceState - 6100);
        for (uint32_t i = 0; i < 8; i++) {
          guest::StoreU32(h.base, kInputs + 4 * i, area + kInputWords + 4 * i);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = r3;
        t.r4.u64 = r4;
        t.r5.u64 = r5;
        t.r6.u64 = r6;
        t.r7.u64 = r7;
        t.f1.u64 = f1;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        guest::StoreU32(h.base, ai::kDrivers, saved_drivers);
        guest::StoreU32(h.base, track_route::kRoute, saved_route);
        guest::StoreU32(h.base, kRaceData, saved_race);
        guest::StoreU32(h.base, kRaceStateGlobal, saved_state);
        for (uint32_t i = 0; i < 8; i++) {
          guest::StoreU32(h.base, kInputs + 4 * i, saved_inputs[i]);
        }
        // The float outputs as floats, then the block with them zeroed.
        std::vector<uint8_t> bytes = Read(h.base, area, kAreaBytes);
        for (const auto& [offset, count] : floats) {
          out[v].Floats(h.base, area + offset, count);
          std::memset(bytes.data() + offset, 0, 4 * count);
        }
        for (uint32_t i = 0; i < kAreaBytes; i += 64) {
          out[v].Bytes(std::vector<uint8_t>(bytes.begin() + i, bytes.begin() + i + 64));
        }
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
        if (f.kind == kSpeedOverDistance || f.kind == kCornerFactor || f.kind == kLimitSpeed) {
          out[v].Double(t.f1.u64);
        }
      }
      results.Check(out[0], out[1], what);
    }
    results.Report();
  }
  memory::Free(table);
  memory::Free(area);
}

}  // namespace harness
