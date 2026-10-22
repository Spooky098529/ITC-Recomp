// The race overlays' update side (src/hooks/race_overlays.cpp) against the
// recompiled functions. Overlays, views and vehicles are live guest objects:
// the cases build their own in a block taken from the system heap for the
// run (a trail alone is 64 KB, more than the scratch area), so nothing the
// game holds is written, except the race data pointer (0x826C5B2C) some
// cases point at the block and put back.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/vehicle.h"
#include "race_overlays/race_overlays.h"

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_overlays);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_vector_library);

namespace harness {

namespace {

namespace ro = race_overlays;

constexpr uint32_t kRaceData = 0x826C5B2C;

// The register steps: the cases run the plain C++ through these (not the
// adapters, so they keep working once an adapter goes), against the
// recompiled versions.
#define ITC_NATIVE_WORD(name, expr)                    \
  void name(PPCContext& ctx, uint8_t* base) {          \
    ctx.r3.u64 = uint32_t(expr);                       \
  }
#define ITC_NATIVE_FLOAT(name, expr)                   \
  void name(PPCContext& ctx, uint8_t* base) {          \
    ctx.fpscr.disableFlushMode();                      \
    ctx.f1.f64 = double(expr);                         \
  }

ITC_NATIVE_WORD(NativeView, ro::View(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeViewportEntry, ro::ViewportEntry(ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeGetSpeedKmh, Vehicle_GetSpeedKmh(base, ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeWheelSpin, ro::WheelSpin(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeWheelValue512, ro::WheelValue512(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeAxleValue3764, ro::AxleValue3764(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_FLOAT(NativeAxleValue3384, ro::AxleValue3384(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeWheelOnGround, ro::WheelOnGround(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeVehicleByte2504, ro::VehicleByte2504(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeVehicleWord2252, ro::VehicleWord2252(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeVehicleWord340, ro::VehicleWord340(base, ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeVehicleFloat6256, ro::VehicleFloat6256(base, ctx.r3.u32))
ITC_NATIVE_FLOAT(NativeVehicleFloat6252, ro::VehicleFloat6252(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeRaceMode, ro::RaceMode(base))
ITC_NATIVE_WORD(NativeRacerFlags, ro::RacerFlags(base, ctx.r3.u32))

// As the adapter: r3 comes back as the view's vehicle (the original's tail
// call leaves it there).
void NativeSpeedKmh(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t view = ctx.r3.u32;
  ctx.r3.u64 = guest::LoadU32(base, view + 4);
  ctx.f1.f64 = double(ro::SpeedKmh(base, view));
}

void NativeRacerId(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = ro::RacerId(base, ctx.r3.u32);
}

void NativeSmokeSetVelocity(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::SmokeSetVelocity(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeSmokeSetRegion(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::SmokeSetRegion(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}
void NativeParticlesBSetPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::ParticlesBSetPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeParticlesCSetPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::ParticlesCSetPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}
void NativeSparksSetPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::SparksSetPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeTrailSetLastPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::TrailSetLastPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeMarkerSetCameraPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::MarkerSetCameraPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeLightSpriteSetPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::LightSpriteSetPosition(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeModelSpritePlaceHidden(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ro::ModelSpritePlaceHidden(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeSetTextures104(PPCContext& ctx, uint8_t* base) {
  ro::SetTextures104(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeSetTextures108(PPCContext& ctx, uint8_t* base) {
  ro::SetTextures108(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeMarkerSetTextures(PPCContext& ctx, uint8_t* base) {
  ro::MarkerSetTextures(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeLightSpriteSetTextures(PPCContext& ctx, uint8_t* base) {
  ro::LightSpriteSetTextures(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeModelSpriteSetTextures(PPCContext& ctx, uint8_t* base) {
  ro::ModelSpriteSetTextures(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_NATIVE_WORD(NativeRand, ro::Rand(base))

void NativeOverlayConstruct(PPCContext& ctx, uint8_t* base) {
  ro::OverlayConstruct(base, ctx.r3.u32);
}
void NativeClearViews(PPCContext& ctx, uint8_t* base) {
  ro::ClearViews(base, ctx.r3.u32);
}
void NativeViewDescClear(PPCContext& ctx, uint8_t* base) {
  ro::ViewDescClear(base, ctx.r3.u32);
}
void NativeOverlayViewConstruct(PPCContext& ctx, uint8_t* base) {
  ro::OverlayViewConstruct(base, ctx.r3.u32);
}
void NativeParticleConstruct(PPCContext& ctx, uint8_t* base) {
  ro::ParticleConstruct(base, ctx.r3.u32);
}
void NativeRaceOverlaysClear(PPCContext& ctx, uint8_t* base) {
  ro::RaceOverlaysClear(base, ctx.r3.u32);
}
void NativeOverlayViewSet(PPCContext& ctx, uint8_t* base) {
  ro::OverlayViewSet(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeBindViewport(PPCContext& ctx, uint8_t* base) {
  ro::BindViewport(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}
void NativeSetRenderTargets(PPCContext& ctx, uint8_t* base) {
  ro::SetRenderTargets(base, ctx.r3.u32, ctx.r4.u32);
}
ITC_NATIVE_WORD(NativeShareTextures, ro::ShareTextures(base, ctx.r3.u32))

void NativeLightSpriteReset(PPCContext& ctx, uint8_t* base) {
  ro::LightSpriteReset(base, ctx.r3.u32);
}
void NativeModelSpriteReset(PPCContext& ctx, uint8_t* base) {
  ro::ModelSpriteReset(base, ctx.r3.u32);
}
void NativeSpriteResetStyle2(PPCContext& ctx, uint8_t* base) {
  ro::SpriteResetStyle2(base, ctx.r3.u32);
}
void NativeTrailReset(PPCContext& ctx, uint8_t* base) {
  ro::TrailReset(base, ctx.r3.u32);
}
void NativeSmokeEmitterReset(PPCContext& ctx, uint8_t* base) {
  ro::SmokeEmitterReset(base, ctx.r4.u32);
}
void NativeSmokeEmitterConstruct(PPCContext& ctx, uint8_t* base) {
  ro::SmokeEmitterConstruct(base, ctx.r3.u32);
}
void NativeParticlesBResetEntry(PPCContext& ctx, uint8_t* base) {
  ro::ParticlesBResetEntry(base, ctx.r4.u32);
}
void NativeParticlesCReset(PPCContext& ctx, uint8_t* base) {
  ro::ParticlesCReset(base, ctx.r3.u32);
}
void NativeMarkerReset(PPCContext& ctx, uint8_t* base) {
  ro::MarkerReset(base, ctx.r3.u32);
}
void NativeSparksReset(PPCContext& ctx, uint8_t* base) {
  ro::SparksReset(base, ctx.r3.u32);
}

void NativeLightSpriteConstruct(PPCContext& ctx, uint8_t* base) {
  ro::LightSpriteConstruct(base, ctx.r3.u32);
}
void NativeModelSpriteConstruct(PPCContext& ctx, uint8_t* base) {
  ro::ModelSpriteConstruct(base, ctx.r3.u32);
}
void NativeTrailConstruct(PPCContext& ctx, uint8_t* base) {
  ro::TrailConstruct(base, ctx.r3.u32);
}
void NativeSmokeConstruct(PPCContext& ctx, uint8_t* base) {
  ro::SmokeConstruct(base, ctx.r3.u32);
}
void NativeParticlesCConstruct(PPCContext& ctx, uint8_t* base) {
  ro::ParticlesCConstruct(base, ctx.r3.u32);
}
void NativeMarkerConstruct(PPCContext& ctx, uint8_t* base) {
  ro::MarkerConstruct(base, ctx.r3.u32);
}
void NativeSparksConstruct(PPCContext& ctx, uint8_t* base) {
  ro::SparksConstruct(base, ctx.r3.u32);
}
void NativeSmokeReset(PPCContext& ctx, uint8_t* base) {
  ro::SmokeReset(base, ctx.r3.u32);
}
void NativeParticlesBReset(PPCContext& ctx, uint8_t* base) {
  ro::ParticlesBReset(base, ctx.r3.u32);
}
ITC_NATIVE_WORD(NativeSmokeInRegion, ro::SmokeInRegion(base, ctx.r3.u32, ctx.r4.u32))

// Where the native version declines, the original runs (as the adapter).
void NativeSparksUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!ro::SparksUpdate(base, ctx.r3.u32)) {
    __imp__sub_8212DDC8(ctx, base);
  }
}

void NativeSmokeUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = ctx.r3.u32;
  if (!ro::SmokeUpdate(base, ctx.r3.u32, &r3)) {
    __imp__sub_8212AA78(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}

void NativeParticlesBUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!ro::ParticlesBUpdate(base, ctx.r3.u32)) {
    __imp__sub_8212B748(ctx, base);
  }
}

void NativeParticlesCUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = ctx.r3.u32;
  if (!ro::ParticlesCUpdate(base, ctx.r3.u32, &r3)) {
    __imp__sub_8212C2F0(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}

ITC_NATIVE_WORD(NativeSlotOf, ro::SlotOf(ctx.r3.u32))

void NativeRandomRange(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t drawn = 0;
  ctx.f1.f64 = double(ro::RandomRange(base, ctx.f1.f64, ctx.f2.f64, &drawn));
  ctx.r3.u64 = drawn;
}

void NativeMarkerSet(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ro::MarkerSet(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r6.u32, ctx.r7.u32);
}
void NativeMarkerUpdate(PPCContext& ctx, uint8_t* base) {
  ro::MarkerUpdate(base, ctx.r3.u32);
}

// Where the native version declines, the original runs (as the adapter).
void NativeTrailUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!ro::TrailUpdate(base, ctx.r3.u32)) {
    __imp__sub_821292E8(ctx, base);
  }
}

void NativeModelSpriteKind(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ro::ModelSpriteKind(ctx.r4.s32);
}
void NativeModelSpriteEntry(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ro::ModelSpriteEntry(base, ctx.r4.u32, ctx.r5.u32);
}
void NativeParticlesCEmit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ro::ParticlesCEmit(base, ctx.r3.u32, ctx.r4.u32);
}

// Where the native version declines, the original runs (as the adapter).
void NativeTrailAddPoint(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t direction[4] = {uint32_t(ctx.r5.u64 >> 32), ctx.r5.u32,
                                 uint32_t(ctx.r6.u64 >> 32), ctx.r6.u32};
  uint32_t result = 0;
  if (!ro::TrailAddPoint(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.f2.f64, direction,
                         ctx.r7.u32, ctx.r10.u32, &result)) {
    __imp__sub_82129598(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

// Where the native version declines, the original runs (as the adapter).
void NativeParticlesBEmit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  double sine = ctx.f1.f64;
  if (!ro::ParticlesBEmit(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, &result, &sine)) {
    __imp__sub_8212B4A8(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
  ctx.f1.f64 = sine;
}

ITC_NATIVE_WORD(NativeModelPartsHasPoint, ro::ModelPartsHasPoint(base, ctx.r3.u32, ctx.r4.s32))
ITC_NATIVE_WORD(NativeModelHasPoint, ro::ModelHasPoint(base, ctx.r3.u32, ctx.r4.s32))
ITC_NATIVE_WORD(NativeOverlayViewHasPoint, ro::OverlayViewHasPoint(base, ctx.r3.u32, ctx.r4.s32))
ITC_NATIVE_WORD(NativeModelPartsPointPosition,
                ro::ModelPartsPointPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32))
ITC_NATIVE_WORD(NativeModelPointPosition,
                ro::ModelPointPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32))
ITC_NATIVE_WORD(NativeOverlayViewPointPosition,
                ro::OverlayViewPointPosition(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32))

void NativeSparksUpdateStreak(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t position[4] = {uint32_t(ctx.r4.u64 >> 32), ctx.r4.u32,
                                uint32_t(ctx.r5.u64 >> 32), ctx.r5.u32};
  uint32_t drawn = ctx.r3.u32;
  double value = ctx.f1.f64;
  ro::SparksUpdateStreak(base, ctx.r3.u32, ctx.f1.f64, position, ctx.r6.u32, ctx.r8.u32, &drawn,
                         &value);
  ctx.r3.u64 = drawn;
  ctx.f1.f64 = value;
}

// Where the native version declines, the original runs (as the adapter).
void NativeSparksEmit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t vector[4] = {uint32_t(ctx.r6.u64 >> 32), ctx.r6.u32,
                              uint32_t(ctx.r7.u64 >> 32), ctx.r7.u32};
  uint32_t result = 0;
  double last = ctx.f1.f64;
  if (!ro::SparksEmit(base, ctx.r3.u32, ctx.r4.u32, ctx.f2.f64, vector, &result, &last)) {
    __imp__sub_8212D818(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
  ctx.f1.f64 = last;
}

// Where the native version declines, the original runs (as the adapter).
void NativeSmokeEmit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  double last = ctx.f1.f64;
  if (!ro::SmokeEmit(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.r5.u32, ctx.r7.u32,
                     ctx.r8.u32, &result, &last)) {
    __imp__sub_8212A350(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
  ctx.f1.f64 = last;
}

// Where the native version declines, the original runs (as the adapter).
void NativeEmitSmoke(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double last = ctx.f1.f64;
  if (!ro::EmitSmoke(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.r5.u32, ctx.r6.u32,
                     ctx.r8.u32, &last)) {
    __imp__sub_82123240(ctx, base);
    return;
  }
  ctx.r3.u64 = 0;
  ctx.f1.f64 = last;
}

// The view's body and wheels (through the vehicle accessors' plain C++).
ITC_NATIVE_WORD(NativeBodyMatrix, ro::BodyMatrix(base, ctx.r3.u32))
ITC_NATIVE_WORD(NativeWheelContact, ro::WheelContact(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeWheelSurfaceMarks, ro::WheelSurfaceMarks(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeBodyPosition, ro::BodyPosition(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_WORD(NativeBodyMatrixCopy, ro::BodyMatrixCopy(base, ctx.r3.u32, ctx.r4.u32))
void NativeIsBehindCamera(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ro::IsBehindCamera(base, ctx.r3.u32, ctx.r4.u32);
}
// As the adapter: r3 comes back as the view's vehicle (not compared: with
// the accessors off, the original leaves it a stack address).
void NativeWheelSlip(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t view = ctx.r3.u32;
  ctx.r3.u64 = guest::LoadU32(base, view + 4);
  ctx.f1.f64 = double(ro::WheelSlip(base, view, ctx.r4.u32, ctx.f1.f64));
}

// Runs the recompiled version with --native_race_overlays off, so the
// functions it calls that are ours run the originals too.
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool native = ITC_FLAG(native_race_overlays);
  ITC_FLAG(native_race_overlays) = false;
  recompiled(t, base);
  ITC_FLAG(native_race_overlays) = native;
}

// The originals of functions that call the vehicle accessors, run with
// those (and the vector library) off too, so the whole chain is the
// recompiled code's.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  const bool accessors = ITC_FLAG(native_vehicle_accessors);
  const bool vectors = ITC_FLAG(native_vector_library);
  ITC_FLAG(native_vehicle_accessors) = false;
  ITC_FLAG(native_vector_library) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_vehicle_accessors) = accessors;
  ITC_FLAG(native_vector_library) = vectors;
}

// The odd float words mixed into fields: NaNs of both kinds and signs,
// infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001, 0x7F800000,
                                   0xFF800000, 0x00000001, 0x807FFFFF, 0x00000000, 0x80000000,
                                   0x7F7FFFFF, 0x3F800000};

uint32_t RandomWord(Random& r) {
  return r.Below(4) == 0 ? kOddFloats[r.Below(std::size(kOddFloats))] : uint32_t(r.Next());
}

// Random words (odd floats among them) over `bytes` at `at`.
void FillRandom(uint8_t* base, Random& r, uint32_t at, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, at + i, RandomWord(r));
  }
}

using Regions = std::vector<std::pair<uint32_t, uint32_t>>;

// One case through both versions: `setup()` writes the inputs (the same
// each time), `args(t)` sets the registers, the function runs; the regions'
// bytes, r3 and f1 are compared.
template <typename Setup, typename Args>
void Compare(Context& h, Results& results, const Regions& regions, Setup setup, Args args,
             GuestFunction recompiled, GuestFunction native, const std::string& what,
             bool compare_r3 = true) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    setup();
    PPCContext t = CallContext(h);
    args(t);
    if (v == 0) {
      RunRecompiled(recompiled, t, h.base);
    } else {
      native(t, h.base);
    }
    for (const auto& [at, bytes] : regions) {
      const std::vector<uint8_t> part = Read(h.base, at, bytes);
      result[v].insert(result[v].end(), part.begin(), part.end());
    }
    for (int i = 0; compare_r3 && i < 4; i++) {
      result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
    }
    for (int i = 0; i < 8; i++) {
      result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
    }
  }
  results.Check(result[0], result[1], what);
}

// The block's layout for the accessors' cases.
constexpr uint32_t kOverlays = 0x0;      // 100 bytes of view slots
constexpr uint32_t kView = 0x100;        // 16 bytes
constexpr uint32_t kVehicle = 0x1000;    // 0x2000 bytes
constexpr uint32_t kRecords = 0x4000;    // 48 + 93 x 3,936 bytes
constexpr uint32_t kBlockBytes = 0x70000;

void AccessorCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t race_data = guest::LoadU32(h.base, kRaceData);

  // The view slots, the view and its vehicle: random words, the view's
  // vehicle the block's.
  auto vehicle_setup = [&](uint64_t seed) {
    return [&h, block, seed] {
      Random fill;
      for (uint64_t i = 0; i < seed % 7; i++) {
        fill.Next();
      }
      FillRandom(h.base, fill, block + kOverlays, 0x100);
      FillRandom(h.base, fill, block + kVehicle, 0x2000);
      guest::StoreU32(h.base, block + kView + 4, block + kVehicle);
    };
  };

  {
    Results results("CarOverlays::View (sub_82126550)");
    for (uint32_t slot = 0; slot < 27; slot++) {
      for (uint64_t seed = 0; seed < 3; seed++) {
        Compare(h, results, {}, vehicle_setup(seed),
                [&](PPCContext& t) {
                  t.r3.u64 = block + kOverlays;
                  t.r4.u64 = slot;
                },
                __imp__sub_82126550, NativeView, fmt::format("slot {} seed {}", slot, seed));
      }
    }
    results.Report();
  }

  {
    Results results("CarOverlays::ViewportEntry (sub_821264A8)");
    std::vector<uint32_t> indices = {0, 1, 2, 3, 4, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF};
    for (int i = 0; i < 40; i++) {
      indices.push_back(uint32_t(r.Next()));
    }
    for (uint32_t index : indices) {
      Compare(h, results, {}, [] {},
              [&](PPCContext& t) {
                t.r3.u64 = r.Next();
                t.r4.u64 = index;
              },
              __imp__sub_821264A8, NativeViewportEntry, fmt::format("index {:X}", index));
    }
    results.Report();
  }

  // OverlayView::SpeedKmh, and the Vehicle::GetSpeedKmh it calls: the
  // recompiled SpeedKmh tail-calls GetSpeedKmh's adapter (always native, no
  // flag), so GetSpeedKmh is checked against its original on its own.
  // Random vehicles, then each odd float at +48.
  {
    const struct {
      const char* name;
      GuestFunction recompiled, native;
      uint32_t r3;
    } speeds[] = {
        {"OverlayView::SpeedKmh (sub_821268C0)", __imp__sub_821268C0, NativeSpeedKmh, block + kView},
        {"Vehicle::GetSpeedKmh (sub_820C0128)", __imp__sub_820C0128, NativeGetSpeedKmh,
         block + kVehicle},
    };
    for (const auto& f : speeds) {
      Results results(f.name);
      for (uint64_t seed = 0; seed < 7; seed++) {
        Compare(h, results, {}, vehicle_setup(seed), [&](PPCContext& t) { t.r3.u64 = f.r3; },
                f.recompiled, f.native, fmt::format("seed {}", seed));
      }
      std::vector<uint32_t> speeds_at_48(std::begin(kOddFloats), std::end(kOddFloats));
      for (int i = 0; i < 30; i++) {
        speeds_at_48.push_back(uint32_t(r.Next()));
      }
      for (uint32_t speed : speeds_at_48) {
        Compare(h, results, {},
                [&, speed] {
                  vehicle_setup(0)();
                  guest::StoreU32(h.base, block + kVehicle + 48, speed);
                },
                [&](PPCContext& t) { t.r3.u64 = f.r3; }, f.recompiled, f.native,
                fmt::format("speed {:08X}", speed));
      }
      results.Report();
    }
  }

  // The wheel accessors: wheels 0 to 5 (4 and 5 past the four the game
  // has), over different vehicles.
  {
    struct Function {
      const char* name;
      GuestFunction recompiled, native;
    };
    const Function wheels[] = {
        {"OverlayView::WheelSpin (sub_821268F0)", __imp__sub_821268F0, NativeWheelSpin},
        {"OverlayView::WheelValue512 (sub_82126908)", __imp__sub_82126908, NativeWheelValue512},
        {"OverlayView::AxleValue3764 (sub_82126920)", __imp__sub_82126920, NativeAxleValue3764},
        {"OverlayView::AxleValue3384 (sub_82126958)", __imp__sub_82126958, NativeAxleValue3384},
        {"OverlayView::WheelOnGround (sub_82126998)", __imp__sub_82126998, NativeWheelOnGround},
    };
    for (const Function& f : wheels) {
      Results results(f.name);
      for (uint32_t wheel = 0; wheel < 6; wheel++) {
        for (uint64_t seed = 0; seed < 7; seed++) {
          Compare(h, results, {}, vehicle_setup(seed),
                  [&](PPCContext& t) {
                    t.r3.u64 = block + kView;
                    t.r4.u64 = wheel;
                  },
                  f.recompiled, f.native, fmt::format("wheel {} seed {}", wheel, seed));
        }
      }
      results.Report();
    }
  }

  // The vehicle's fields, and the race mode.
  {
    struct Function {
      const char* name;
      GuestFunction recompiled, native;
    };
    const Function fields[] = {
        {"OverlayView::VehicleByte2504 (sub_82126A80)", __imp__sub_82126A80, NativeVehicleByte2504},
        {"OverlayView::VehicleWord2252 (sub_82126AA0)", __imp__sub_82126AA0, NativeVehicleWord2252},
        {"OverlayView::VehicleWord340 (sub_82126AB0)", __imp__sub_82126AB0, NativeVehicleWord340},
        {"OverlayView::VehicleFloat6256 (sub_82126AC0)", __imp__sub_82126AC0, NativeVehicleFloat6256},
        {"OverlayView::VehicleFloat6252 (sub_82126AD0)", __imp__sub_82126AD0, NativeVehicleFloat6252},
        {"RaceOverlays::RaceMode (sub_82126D18)", __imp__sub_82126D18, NativeRaceMode},
    };
    for (const Function& f : fields) {
      Results results(f.name);
      for (uint64_t seed = 0; seed < 7; seed++) {
        Compare(h, results, {},
                [&, seed] {
                  vehicle_setup(seed)();
                  // The race mode's word, at the block's vehicle.
                  guest::StoreU32(h.base, kRaceData, block + kVehicle + 0x10 - 0xA3AF4);
                },
                [&](PPCContext& t) { t.r3.u64 = block + kView; }, f.recompiled, f.native,
                fmt::format("seed {}", seed));
      }
      guest::StoreU32(h.base, kRaceData, race_data);
      results.Report();
    }
  }

  // The racer: the vehicle's +5712 one of the 93 records, between two, past
  // the last, or a random word; the records' flags and ids random.
  {
    Results flags("OverlayView::RacerFlags (sub_82127178)");
    Results ids("OverlayView::RacerId (sub_82126AE0)");
    const uint32_t records = block + kRecords;
    for (int i = 0; i < 300; i++) {
      const uint32_t k = r.Below(95);
      const uint32_t kind = r.Below(5);
      uint32_t racer = records + 48 + 3936 * k;
      if (kind == 3) {
        racer += 4 * (1 + r.Below(900));
      } else if (kind == 4) {
        racer = records + 48 + 3936 * r.Below(93) + 3936 - 4;
      }
      const uint64_t seed = r.Next();
      auto setup = [&, racer, seed] {
        Random fill;
        for (uint64_t n = 0; n < seed % 13; n++) {
          fill.Next();
        }
        FillRandom(h.base, fill, block + kVehicle, 0x2000);
        guest::StoreU32(h.base, block + kView + 4, block + kVehicle);
        guest::StoreU32(h.base, block + kVehicle + 5712, racer);
        for (uint32_t n = 0; n < 95; n++) {
          guest::StoreU32(h.base, records + 3936 * n + 3960, RandomWord(fill));
          guest::StoreU32(h.base, records + 48 + 3936 * n + 3888, RandomWord(fill));
        }
        guest::StoreU32(h.base, kRaceData, records);
      };
      auto args = [&](PPCContext& t) { t.r3.u64 = block + kView; };
      const std::string what = fmt::format("record {} kind {}", k, kind);
      Compare(h, ids, {}, setup, args, __imp__sub_82126AE0, NativeRacerId, what);
      if (kind != 3) {  // inside a record: its flags word is the one set
        Compare(h, flags, {}, setup, args, __imp__sub_82127178, NativeRacerFlags, what);
      }
    }
    guest::StoreU32(h.base, kRaceData, race_data);
    flags.Report();
    ids.Report();
  }
}

// The setters: an object of random words (the shown flags among them 0 or
// not), vectors from elsewhere or overlapping the destination, and random
// r5, r6 and f1.
void SetterCases(Context& h, uint32_t block) {
  Random& r = h.random;
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t bytes;      // the object's
    uint32_t to;         // where the vector goes (for overlapping sources)
    uint32_t flag;       // a word the cases set to 0 or not (0: none)
  };
  const Function setters[] = {
      {"Smoke::SetVelocity (sub_8212A840)", __imp__sub_8212A840, NativeSmokeSetVelocity, 32640,
       32544, 0},
      {"Smoke::SetRegion (sub_8212A998)", __imp__sub_8212A998, NativeSmokeSetRegion, 32640, 32608,
       0},
      {"ParticlesB::SetPosition (sub_8212BA70)", __imp__sub_8212BA70, NativeParticlesBSetPosition,
       2816, 2784, 0},
      {"ParticlesC::SetPosition (sub_8212C6A8)", __imp__sub_8212C6A8, NativeParticlesCSetPosition,
       2720, 2672, 0},
      {"Sparks::SetPosition (sub_8212DF90)", __imp__sub_8212DF90, NativeSparksSetPosition, 29824,
       28960, 0},
      {"Trail::SetLastPosition (sub_821299D8)", __imp__sub_821299D8, NativeTrailSetLastPosition,
       64160, 64144, 0},
      {"Marker::SetCameraPosition (sub_8212CE18)", __imp__sub_8212CE18,
       NativeMarkerSetCameraPosition, 304, 272, 0},
      {"LightSprite::SetPosition (sub_8211EBE0)", __imp__sub_8211EBE0,
       NativeLightSpriteSetPosition, 288, 192, 240},
      {"ModelSprite::PlaceHidden (sub_82121560)", __imp__sub_82121560,
       NativeModelSpritePlaceHidden, 576, 272, 0},
      {"Overlay::SetTextures104 (sub_8212C0E0)", __imp__sub_8212C0E0, NativeSetTextures104, 304,
       104, 0},
      {"Overlay::SetTextures108 (sub_8212D6D8)", __imp__sub_8212D6D8, NativeSetTextures108, 304,
       108, 0},
      {"Marker::SetTextures (sub_8212CC10)", __imp__sub_8212CC10, NativeMarkerSetTextures, 304, 104,
       0},
      {"LightSprite::SetTextures (sub_8211E9C8)", __imp__sub_8211E9C8,
       NativeLightSpriteSetTextures, 288, 104, 0},
      {"ModelSprite::SetTextures (sub_8211F408)", __imp__sub_8211F408,
       NativeModelSpriteSetTextures, 576, 108, 0},
  };
  const uint32_t object = block + 0x100;
  const uint32_t source = block + 0x10200;  // past the largest object
  for (const Function& f : setters) {
    Results results(f.name);
    for (int i = 0; i < 40; i++) {
      const uint64_t seed = r.Next();
      const uint32_t kind = r.Below(4);
      // A vector elsewhere, the second one 16 bytes on; or overlapping the
      // destination by a few words either way.
      uint32_t from = source + 4 * r.Below(8);
      if (kind == 3) {
        from = object + f.to + 4 * r.Below(7) - 12;
      }
      const uint32_t r5 = kind == 2 ? object + f.to - 16 + 4 * r.Below(8) : from + 16;
      const uint32_t r6 = uint32_t(r.Next());
      const double f1 = r.Below(4) == 0 ? std::bit_cast<double>(r.Next())
                                         : double(std::bit_cast<float>(RandomWord(r)));
      const uint32_t flag = r.Below(2) ? 0 : RandomWord(r);
      Compare(h, results, {{object, f.bytes}, {source, 64}},
              [&, seed] {
                Random fill;
                for (uint64_t n = 0; n < seed % 17; n++) {
                  fill.Next();
                }
                FillRandom(h.base, fill, object, (f.bytes + 3) & ~3u);
                FillRandom(h.base, fill, source, 64);
                if (f.flag) {
                  guest::StoreU32(h.base, object + f.flag, flag);
                }
              },
              [&](PPCContext& t) {
                t.r3.u64 = object;
                t.r4.u64 = from;
                t.r5.u64 = r5;
                t.r6.u64 = r6;
                t.f1.f64 = f1;
              },
              f.recompiled, f.native, fmt::format("case {} kind {}", i, kind));
    }
    results.Report();
  }
}

// rand and RandomRange: random seeds (the live one put back after), and
// ranges of game-like floats, odd floats and random doubles.
void RandomCases(Context& h) {
  Random& r = h.random;
  constexpr uint32_t kSeed = 0x82638188;
  const uint32_t live = guest::LoadU32(h.base, kSeed);
  {
    Results results("rand (sub_82293568)");
    std::vector<uint32_t> seeds = {0, 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, live};
    for (int i = 0; i < 300; i++) {
      seeds.push_back(uint32_t(r.Next()));
    }
    for (uint32_t seed : seeds) {
      Compare(h, results, {{kSeed, 4}}, [&] { guest::StoreU32(h.base, kSeed, seed); },
              [](PPCContext&) {}, __imp__sub_82293568, NativeRand,
              fmt::format("seed {:08X}", seed));
    }
    results.Report();
  }
  {
    Results results("RandomRange (sub_8211F118)");
    auto value = [&]() -> double {
      switch (r.Below(3)) {
        case 0:
          return double(std::bit_cast<float>(RandomWord(r)));
        case 1:
          return std::bit_cast<double>(r.Next());
        default:
          return double(float(int32_t(r.Below(2001)) - 1000) / 64.0f);
      }
    };
    for (int i = 0; i < 600; i++) {
      const uint32_t seed = uint32_t(r.Next());
      const double low = value(), high = value();
      Compare(h, results, {{kSeed, 4}}, [&] { guest::StoreU32(h.base, kSeed, seed); },
              [&](PPCContext& t) {
                t.f1.f64 = low;
                t.f2.f64 = high;
              },
              __imp__sub_8211F118, NativeRandomRange,
              fmt::format("seed {:08X} low {:016X} high {:016X}", seed,
                          std::bit_cast<uint64_t>(low), std::bit_cast<uint64_t>(high)));
    }
    results.Report();
  }
  guest::StoreU32(h.base, kSeed, live);
}

// The marker: random markers (shown or not), parameter tables with game-like
// and odd floats (inside the block, or overlapping the marker), distances
// of 0, game-like, odd and random doubles, kinds 0-5 and random.
void MarkerCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t marker = block + 0x100;
  const uint32_t table = block + 0x400;
  {
    Results results("Marker::Set (sub_8212CCE0)");
    for (int i = 0; i < 1500; i++) {
      const uint64_t seed = r.Next();
      const uint32_t shown = r.Below(2) ? 0 : RandomWord(r);
      const uint32_t at = r.Below(10) == 0 ? marker + 4 * r.Below(76) : table;
      const uint32_t position = r.Below(10) == 0 ? marker + 176 - 8 + 4 * r.Below(5) : table + 64;
      double distance;
      switch (r.Below(5)) {
        case 0:
          distance = 0.0;
          break;
        case 1:
          distance = double(std::bit_cast<float>(RandomWord(r)));
          break;
        case 2:
          distance = std::bit_cast<double>(r.Next());
          break;
        default:
          distance = double(float(r.Below(400000)) / 1000.0f);
          break;
      }
      const uint32_t kind = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(6);
      const uint32_t mode = uint32_t(r.Next());
      const bool game_table = r.Below(2);
      Compare(h, results, {{marker, 304}, {table, 128}},
              [&, seed] {
                Random fill;
                for (uint64_t n = 0; n < seed % 19; n++) {
                  fill.Next();
                }
                FillRandom(h.base, fill, marker, 304);
                FillRandom(h.base, fill, table, 128);
                if (game_table) {  // life, size, alpha
                  guest::StoreFloat(h.base, table, float(fill.Below(100)));
                  guest::StoreFloat(h.base, table + 4, float(fill.Below(1000)) / 100.0f);
                  guest::StoreFloat(h.base, table + 8, float(fill.Below(256)));
                }
                guest::StoreU32(h.base, marker + 120, at);
                guest::StoreU32(h.base, marker + 224, shown);
              },
              [&](PPCContext& t) {
                t.r3.u64 = marker;
                t.r4.u64 = position;
                t.f1.f64 = distance;
                t.r6.u64 = mode;
                t.r7.u64 = kind;
              },
              __imp__sub_8212CCE0, NativeMarkerSet,
              fmt::format("case {} distance {:016X} kind {:X}", i,
                          std::bit_cast<uint64_t>(distance), kind));
    }
    results.Report();
  }
  {
    Results results("Marker::Update (sub_8212CBE0)");
    const uint32_t counters[] = {0, 1, 0x7FFFFFFE, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF};
    for (int i = 0; i < 200; i++) {
      const uint32_t shown = r.Below(3) == 0 ? 0 : RandomWord(r);
      const uint32_t counter = r.Below(2) ? counters[r.Below(6)] : uint32_t(r.Next());
      Compare(h, results, {{marker, 304}},
              [&] {
                Fill(h.base, marker, 304, 0xCD);
                guest::StoreU32(h.base, marker + 224, shown);
                guest::StoreU32(h.base, marker + 236, counter);
              },
              [&](PPCContext& t) { t.r3.u64 = marker; }, __imp__sub_8212CBE0, NativeMarkerUpdate,
              fmt::format("shown {:X} counter {:X}", shown, counter));
    }
    results.Report();
  }
}

// Trail::Update: capacities of the game's 500, small and larger ones (and 0
// and -1, which the original handles), the oldest point anywhere, counts
// from 0 to past the capacity (and negative), ages around 0, tables with
// game-like and random words.
void TrailCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t trail = block + 0x100;
  const uint32_t table = block + 0x40;
  Results results("Trail::Update (sub_821292E8)");
  for (int i = 0; i < 400; i++) {
    int32_t capacity;
    switch (r.Below(6)) {
      case 0:
      case 1:
        capacity = 500;
        break;
      case 2:
        capacity = int32_t(1 + r.Below(8));
        break;
      case 3:
        capacity = int32_t(1 + r.Below(3000));
        break;
      default:
        capacity = int32_t(1 + r.Below(600));
        break;
    }
    if (i == 0) {
      capacity = 0;
    } else if (i == 1) {
      capacity = -1;
    }
    const uint32_t points = capacity > 0 ? uint32_t(capacity) : 4;
    const uint32_t start = r.Below(points);
    int32_t count;
    switch (r.Below(5)) {
      case 0:
        count = 0;
        break;
      case 1:
        count = -int32_t(r.Below(5));
        break;
      default:
        count = int32_t(r.Below(points + 3));
        break;
    }
    const uint64_t seed = r.Next();
    const bool game_like = r.Below(4) != 0;
    const uint32_t bytes = (points + 2) * 128;
    Compare(h, results, {{block, 0x100}, {trail, bytes}, {trail + 64000, 160}},
            [&, seed] {
              Random fill;
              for (uint64_t n = 0; n < seed % 23; n++) {
                fill.Next();
              }
              FillRandom(h.base, fill, block, 0x100 + bytes);
              FillRandom(h.base, fill, trail + 64000, 160);
              guest::StoreU32(h.base, trail + 104, uint32_t(capacity));
              guest::StoreU32(h.base, trail + 112, table);
              guest::StoreU32(h.base, trail + 64128, start);
              guest::StoreU32(h.base, trail + 64136, uint32_t(count));
              if (game_like) {
                guest::StoreU32(h.base, table + 0, fill.Below(40));
                guest::StoreU32(h.base, table + 12, fill.Below(40));
                for (uint32_t p = 0; p < points; p++) {
                  const uint32_t at = trail + 128 * (p + 1);
                  guest::StoreU32(h.base, at + 60, uint32_t(int32_t(fill.Below(40)) - 3));
                  guest::StoreFloat(h.base, at + 124, float(fill.Below(4000)) / 16.0f);
                }
              }
            },
            [&](PPCContext& t) { t.r3.u64 = trail; }, __imp__sub_821292E8, NativeTrailUpdate,
            fmt::format("capacity {} start {} count {}", capacity, start, count));
  }
  results.Report();
}

// The car-model sprites' kind (racer ids around the ones it names, and
// random) and table entry (kinds and indices around the jump tables' ranges,
// and random).
void ModelSpriteCases(Context& h, uint32_t block) {
  Random& r = h.random;
  {
    Results results("CarOverlays::ModelSpriteKind (sub_82123188)");
    std::vector<uint32_t> ids = {0xFFFFFFFF, 0xFFFFFFFE, 0, 0x7FFFFFFF, 0x80000000};
    for (uint32_t id = 350; id < 410; id++) {
      ids.push_back(id);
    }
    for (int i = 0; i < 40; i++) {
      ids.push_back(uint32_t(r.Next()));
    }
    for (uint32_t id : ids) {
      Compare(h, results, {}, [] {},
              [&](PPCContext& t) {
                t.r3.u64 = block;
                t.r4.u64 = int64_t(int32_t(id));
              },
              __imp__sub_82123188, NativeModelSpriteKind, fmt::format("id {}", int32_t(id)));
    }
    results.Report();
  }
  {
    Results results("ModelSprite::Entry (sub_8211F300)");
    std::vector<uint32_t> values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0xFFFFFFFF, 0x80000000};
    for (int i = 0; i < 4; i++) {
      values.push_back(uint32_t(r.Next()));
    }
    for (uint32_t kind : values) {
      for (uint32_t index : values) {
        Compare(h, results, {}, [] {},
                [&](PPCContext& t) {
                  t.r3.u64 = r.Next();
                  t.r4.u64 = kind;
                  t.r5.u64 = index;
                },
                __imp__sub_8211F300, NativeModelSpriteEntry,
                fmt::format("kind {:X} index {:X}", kind, index));
      }
    }
    results.Report();
  }
}

// ParticlesC::Emit: capacities of the game's 20, small, 0 and huge; counts
// around them; the head anywhere in the 20 entries; tables with game-like
// and odd floats (inside the block, or overlapping the effect); positions
// elsewhere or overlapping the entries.
void ParticlesCCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t effect = block + 0x100;
  const uint32_t table = block + 0x40;
  const uint32_t source = block + 0x10200;
  Results results("ParticlesC::Emit (sub_8212C1C0)");
  for (int i = 0; i < 600; i++) {
    uint32_t capacity;
    switch (r.Below(5)) {
      case 0:
        capacity = 0;
        break;
      case 1:
        capacity = r.Below(2) ? 0xFFFFFFFF : uint32_t(r.Next());
        break;
      case 2:
        capacity = 1 + r.Below(6);
        break;
      default:
        capacity = 20;
        break;
    }
    const uint32_t count = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(24);
    const uint32_t head = r.Below(capacity && capacity < 21 ? capacity : 21);
    const uint32_t at = r.Below(10) == 0 ? effect + 4 * r.Below(600) : table;
    const uint32_t position = r.Below(8) == 0 ? effect + 112 + 4 * r.Below(640) : source + 4 * r.Below(4);
    const uint64_t seed = r.Next();
    const bool game_table = r.Below(2);
    Compare(h, results, {{block, 0x100}, {effect, 2816}, {source, 64}},
            [&, seed] {
              Random fill;
              for (uint64_t n = 0; n < seed % 19; n++) {
                fill.Next();
              }
              FillRandom(h.base, fill, block, 0x100 + 2816);
              FillRandom(h.base, fill, source, 64);
              if (game_table) {  // life, size, alpha
                guest::StoreU32(h.base, table, fill.Below(100));
                guest::StoreFloat(h.base, table + 4, float(fill.Below(1000)) / 100.0f);
                guest::StoreFloat(h.base, table + 8, float(fill.Below(256)));
              }
              guest::StoreU32(h.base, effect + 108, at);
              guest::StoreU32(h.base, effect + 2692, head);
              guest::StoreU32(h.base, effect + 2696, count);
              guest::StoreU32(h.base, effect + 2700, capacity);
            },
            [&](PPCContext& t) {
              t.r3.u64 = effect;
              t.r4.u64 = position;
              t.f1.f64 = 0.0;
            },
            __imp__sub_8212C1C0, NativeParticlesCEmit,
            fmt::format("case {} capacity {:X} count {:X} head {}", i, capacity, count, head));
  }
  results.Report();
}

// Trail::AddPoint: capacities of the game's 500, small ones, 0 and -1 (the
// original runs there); counts around them (and negative); the tail anywhere;
// the state 0, 1, 2 or random; positions at, near (down to denormal steps,
// which flush to zero in the vector length) and away from the last one;
// game-like, odd and random floats for the direction, speed and width; the
// trail aligned or not (the original's lvx rounds the address down).
void TrailAddPointCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t table = block + 0x40;
  const uint32_t source = block + 0x10200;
  // With a NaN or an infinity among the inputs, the NaNs that come out of
  // the side's vector arithmetic can differ in sign and payload: which
  // operand's NaN an SSE instruction passes on depends on the operand order
  // the compiler picked, and it rewrote the original's Newton step (its
  // negated product plus the estimate became the estimate minus the
  // product, seen in its disassembly), so those bits are the compiler's,
  // not the game's.
  Results results("Trail::AddPoint (sub_82129598)");
  Results nan_results("Trail::AddPoint, NaN or infinite inputs (expected to differ)");
  auto not_finite = [](uint32_t word) { return (word & 0x7F800000) == 0x7F800000; };
  auto game_float = [&](float range) -> float {
    return (float(r.Below(20001)) / 10000.0f - 1.0f) * range;
  };
  for (int i = 0; i < 600; i++) {
    const uint32_t trail = block + (r.Below(6) == 0 ? 0x104 + 4 * r.Below(3) : 0x100);
    int32_t capacity;
    switch (r.Below(6)) {
      case 0:
        capacity = int32_t(1 + r.Below(8));
        break;
      case 1:
        capacity = r.Below(2) ? 0 : -1;
        break;
      default:
        capacity = 500;
        break;
    }
    const uint32_t points = capacity > 0 ? uint32_t(capacity) : 4;
    const uint32_t tail = r.Below(points);
    int32_t count;
    switch (r.Below(5)) {
      case 0:
        count = 0;
        break;
      case 1:
        count = -int32_t(r.Below(5));
        break;
      default:
        count = int32_t(r.Below(points + 2));
        break;
    }
    // The position and the last one.
    float position[4], last[4];
    const uint32_t step = r.Below(5);
    for (int k = 0; k < 4; k++) {
      position[k] = game_float(2000.0f);
      switch (step) {
        case 0:
          last[k] = position[k];
          break;
        case 1:  // tiny values: squared, they flush to zero
          position[k] = std::bit_cast<float>(r.Below(0x01000000));
          last[k] = std::bit_cast<float>(r.Below(0x01000000));
          break;
        case 2:
          last[k] = std::bit_cast<float>(RandomWord(r));
          break;
        default:
          last[k] = position[k] + game_float(2.0f);
          break;
      }
    }
    const bool odd_direction = r.Below(4) == 0;
    uint32_t direction[4];
    for (uint32_t& word : direction) {
      word = odd_direction ? RandomWord(r) : std::bit_cast<uint32_t>(game_float(1.0f));
    }
    const double speed = r.Below(5) == 0 ? double(std::bit_cast<float>(RandomWord(r)))
                                          : double(float(r.Below(300)));
    const double width = r.Below(5) == 0 ? double(std::bit_cast<float>(RandomWord(r)))
                                          : double(game_float(0.5f));
    uint32_t state = r.Below(4);
    if (state == 3) {
      state = uint32_t(r.Next());
    }
    const uint32_t value = uint32_t(r.Next());
    const uint64_t seed = r.Next();
    const bool game_table = r.Below(4) != 0;
    bool odd = not_finite(std::bit_cast<uint32_t>(float(speed))) ||
               not_finite(std::bit_cast<uint32_t>(float(width)));
    for (int k = 0; k < 4; k++) {
      odd = odd || not_finite(direction[k]) || not_finite(std::bit_cast<uint32_t>(last[k]));
    }
    Compare(h, odd ? nan_results : results,
            {{block, 0x100}, {block + 0x100, 64160 + 12}, {source, 64}},
            [&, seed] {
              Random fill;
              for (uint64_t n = 0; n < seed % 23; n++) {
                fill.Next();
              }
              FillRandom(h.base, fill, block, 0x100);
              const uint32_t point = trail + 128 * (tail + 1);
              FillRandom(h.base, fill, point & ~0xFu, 160);
              FillRandom(h.base, fill, trail + 64000, 160);
              guest::StoreU32(h.base, trail + 104, uint32_t(capacity));
              guest::StoreU32(h.base, trail + 112, table);
              guest::StoreU32(h.base, trail + 64132, tail);
              guest::StoreU32(h.base, trail + 64136, uint32_t(count));
              for (int k = 0; k < 4; k++) {
                guest::StoreFloat(h.base, source + 4 * k, position[k]);
                guest::StoreFloat(h.base, trail + 64144 + 4 * k, last[k]);
              }
              if (game_table) {  // life, width
                guest::StoreU32(h.base, table + 0, fill.Below(400));
                guest::StoreFloat(h.base, table + 8, float(fill.Below(100)) / 100.0f);
              }
            },
            [&](PPCContext& t) {
              t.r3.u64 = trail;
              t.r4.u64 = source;
              t.r5.u64 = (uint64_t(direction[0]) << 32) | direction[1];
              t.r6.u64 = (uint64_t(direction[2]) << 32) | direction[3];
              t.r7.u64 = state;
              t.r10.u64 = value;
              t.f1.f64 = speed;
              t.f2.f64 = width;
            },
            __imp__sub_82129598, NativeTrailAddPoint,
            fmt::format("case {} capacity {} count {} tail {} step {} state {:X}", i, capacity,
                        count, tail, step, state));
  }
  results.Report();
  nan_results.Report();
}

// ParticlesB::Emit: capacities of the game's 15, small ones, 0 and -1 (the
// original runs there); counts around them (and negative); the head
// anywhere in the 15 entries; random seeds; tables with game-like and odd
// floats (inside the block, or overlapping the effect); positions elsewhere
// or overlapping the entries; game-like, odd and random size factors.
void ParticlesBCases(Context& h, uint32_t block) {
  Random& r = h.random;
  constexpr uint32_t kSeed = 0x82638188;
  const uint32_t live = guest::LoadU32(h.base, kSeed);
  const uint32_t effect = block + 0x100;
  const uint32_t table = block + 0x40;
  const uint32_t source = block + 0x10200;
  Results results("ParticlesB::Emit (sub_8212B4A8)");
  for (int i = 0; i < 600; i++) {
    int32_t capacity;
    switch (r.Below(5)) {
      case 0:
        capacity = r.Below(2) ? 0 : -1;
        break;
      case 1:
        capacity = int32_t(1 + r.Below(6));
        break;
      default:
        capacity = 15;
        break;
    }
    const int32_t count = r.Below(5) == 0 ? -int32_t(r.Below(5)) : int32_t(r.Below(18));
    const uint32_t head = r.Below(capacity > 0 ? uint32_t(capacity) : 15);
    const uint32_t at = r.Below(10) == 0 ? effect + 4 * r.Below(680) : table;
    const uint32_t position =
        r.Below(8) == 0 ? effect + 128 + 4 * r.Below(660) : source + 4 * r.Below(4);
    double factor;
    switch (r.Below(4)) {
      case 0:
        factor = double(std::bit_cast<float>(RandomWord(r)));
        break;
      case 1:
        factor = std::bit_cast<double>(r.Next());
        break;
      default:
        factor = double(float(r.Below(4000)) / 100.0f);
        break;
    }
    const uint32_t seed = uint32_t(r.Next());
    const uint64_t fill_seed = r.Next();
    const bool game_table = r.Below(2);
    Compare(h, results, {{block, 0x100}, {effect, 2816}, {source, 64}, {kSeed, 4}},
            [&, fill_seed] {
              Random fill;
              for (uint64_t n = 0; n < fill_seed % 19; n++) {
                fill.Next();
              }
              FillRandom(h.base, fill, block, 0x100 + 2816);
              FillRandom(h.base, fill, source, 64);
              if (game_table) {  // life, size, size factor
                guest::StoreU32(h.base, table, fill.Below(100));
                guest::StoreFloat(h.base, table + 4, float(fill.Below(1000)) / 100.0f);
                guest::StoreFloat(h.base, table + 8, float(fill.Below(256)) / 16.0f);
              }
              guest::StoreU32(h.base, effect + 104, uint32_t(capacity));
              guest::StoreU32(h.base, effect + 112, at);
              guest::StoreU32(h.base, effect + 2772, head);
              guest::StoreU32(h.base, effect + 2776, uint32_t(count));
              guest::StoreU32(h.base, kSeed, seed);
            },
            [&](PPCContext& t) {
              t.r3.u64 = effect;
              t.r4.u64 = position;
              t.f1.f64 = factor;
            },
            __imp__sub_8212B4A8, NativeParticlesBEmit,
            fmt::format("case {} capacity {} count {} head {} factor {:016X}", i, capacity, count,
                        head, std::bit_cast<uint64_t>(factor)));
  }
  results.Report();
  guest::StoreU32(h.base, kSeed, live);
}

// The model points' checks: a view, its model, the model's point object and
// parts in the block, with the model's four words each sometimes 0 and the
// parts' words often 0; points -3 to 25 and random ones.
constexpr uint32_t kPointView = 0x100;
constexpr uint32_t kPointModel = 0x1000;    // 0x1400 bytes
constexpr uint32_t kPointObject = 0x3000;   // 0x100 bytes
constexpr uint32_t kPointTable = 0x4000;    // 0x2000 bytes
constexpr uint32_t kPointParts = 0x8000;    // 0xA000 bytes
constexpr uint32_t kPointOut = 0x20000;     // 0x100 bytes

// Fills the model's block (the view, the model, its point object, table and
// parts); each of the model's four words is 0 one time in five.
void ModelPointSetup(Context& h, uint32_t block, uint64_t seed) {
  Random fill;
  for (uint64_t i = 0; i < seed % 23; i++) {
    fill.Next();
  }
  for (uint32_t i = 0; i < 0xA000; i += 4) {
    guest::StoreU32(h.base, block + kPointParts + i, fill.Below(3) == 0 ? 0 : RandomWord(fill));
  }
  FillRandom(h.base, fill, block + kPointModel, 0x1400);
  FillRandom(h.base, fill, block + kPointObject, 0x100);
  FillRandom(h.base, fill, block + kPointTable, 0x2000);
  FillRandom(h.base, fill, block + kPointOut, 0x100);
  guest::StoreU32(h.base, block + kPointView + 8, block + kPointModel);
  const uint32_t words[4][2] = {{5048, block + kPointObject},
                                {5052, uint32_t(fill.Next())},
                                {5060, block + kPointParts},
                                {5064, uint32_t(fill.Next())}};
  for (const auto& [offset, value] : words) {
    guest::StoreU32(h.base, block + kPointModel + offset, fill.Below(5) == 0 ? 0 : value);
  }
  guest::StoreU32(h.base, block + kPointObject + 24, block + kPointTable - 6976 + 0x40);
}

std::vector<int32_t> PointIndices(Random& r) {
  std::vector<int32_t> indices;
  for (int32_t i = -3; i <= 25; i++) {
    indices.push_back(i);
  }
  indices.push_back(INT32_MIN);
  indices.push_back(INT32_MAX);
  for (int i = 0; i < 10; i++) {
    indices.push_back(int32_t(r.Next()));
  }
  return indices;
}

void ModelPointCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const std::vector<int32_t> indices = PointIndices(r);
  const Regions regions = {{block + kPointOut, 0x100}};
  {
    Results results("ModelParts::HasPoint (sub_820DF2A0)");
    for (int32_t index : indices) {
      for (uint64_t seed = 0; seed < 6; seed++) {
        Compare(h, results, regions, [&h, block, seed] { ModelPointSetup(h, block, seed); },
                [&](PPCContext& t) {
                  t.r3.u64 = block + kPointParts;
                  t.r4.u64 = uint64_t(int64_t(index));
                },
                __imp__sub_820DF2A0, NativeModelPartsHasPoint,
                fmt::format("point {} seed {}", index, seed));
      }
    }
    results.Report();
  }
  {
    Results results("Model::HasPoint (sub_820D9DB0)");
    for (int32_t index : indices) {
      for (uint64_t seed = 0; seed < 12; seed++) {
        Compare(h, results, regions, [&h, block, seed] { ModelPointSetup(h, block, seed); },
                [&](PPCContext& t) {
                  t.r3.u64 = block + kPointModel;
                  t.r4.u64 = uint64_t(int64_t(index));
                },
                __imp__sub_820D9DB0, NativeModelHasPoint,
                fmt::format("point {} seed {}", index, seed));
      }
    }
    results.Report();
  }
  {
    Results results("OverlayView::HasPoint (sub_82126A78)");
    for (int32_t index : indices) {
      for (uint64_t seed = 0; seed < 12; seed++) {
        Compare(h, results, regions, [&h, block, seed] { ModelPointSetup(h, block, seed); },
                [&](PPCContext& t) {
                  t.r3.u64 = block + kPointView;
                  t.r4.u64 = uint64_t(int64_t(index));
                },
                __imp__sub_82126A78, NativeOverlayViewHasPoint,
                fmt::format("point {} seed {}", index, seed));
      }
    }
    results.Report();
  }
}

// The model points' positions: as the checks' cases, with `out` in its own
// area, or overlapping the first points' table or a part's point (points
// -3 to 25 and the ends of the range: big negative points would read
// outside the block below 4).
void ModelPointPositionCases(Context& h, uint32_t block) {
  Random& r = h.random;
  std::vector<int32_t> indices;
  for (int32_t i = -3; i <= 25; i++) {
    indices.push_back(i);
  }
  indices.push_back(INT32_MIN);
  indices.push_back(INT32_MAX);
  const Regions regions = {{block + kPointOut, 0x100}, {block + kPointTable, 0x100},
                           {block + kPointParts + 23120 + 240 * 8, 0x40}};
  struct Function {
    const char* name;
    uint32_t object;
    uint64_t seeds;
    GuestFunction recompiled, native;
  };
  const Function functions[] = {
      {"ModelParts::PointPosition (sub_820E0910)", block + kPointParts, 6, __imp__sub_820E0910,
       NativeModelPartsPointPosition},
      {"Model::PointPosition (sub_820DA988)", block + kPointModel, 12, __imp__sub_820DA988,
       NativeModelPointPosition},
      {"OverlayView::PointPosition (sub_82127140)", block + kPointView, 12, __imp__sub_82127140,
       NativeOverlayViewPointPosition},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int32_t index : indices) {
      for (uint64_t seed = 0; seed < f.seeds; seed++) {
        uint32_t out = block + kPointOut + 4 * r.Below(8);
        switch (r.Below(6)) {
          case 0:
            out = block + kPointTable + 0x40 + 4 * r.Below(16);
            break;
          case 1:
            out = block + kPointParts + 23120 + 240 * 8 + 4 * r.Below(8);
            break;
          default:
            break;
        }
        Compare(h, results, regions, [&h, block, seed] { ModelPointSetup(h, block, seed); },
                [&](PPCContext& t) {
                  t.r3.u64 = out;
                  t.r4.u64 = f.object;
                  t.r5.u64 = uint64_t(int64_t(index));
                },
                f.recompiled, f.native,
                fmt::format("point {} seed {} out {:X}", index, seed, out - block));
      }
    }
    results.Report();
  }
}

// Sparks::UpdateStreak: capacities of 200 (the room the object has), small
// ones and 0; the head inside them; counts from 0 to past the capacity (and
// negative); each spark's state 1, 2, 0 or random; random words (odd
// floats among them) in the sparks, their limits and the position;
// game-like, odd and random speeds; streaking 0, 1 or random; modes 0, 1, 2
// or random; random seeds.
void SparksUpdateStreakCases(Context& h, uint32_t block) {
  Random& r = h.random;
  constexpr uint32_t kSeed = 0x82638188;
  const uint32_t live = guest::LoadU32(h.base, kSeed);
  const uint32_t sparks = block + 0x100;
  constexpr uint32_t kBytes = 0x10000;
  Results results("Sparks::UpdateStreak (sub_8212E660)");
  for (int i = 0; i < 400; i++) {
    uint32_t capacity;
    switch (r.Below(6)) {
      case 0:
        capacity = 1 + r.Below(8);
        break;
      case 1:
        capacity = 0;
        break;
      default:
        capacity = 200;
        break;
    }
    const uint32_t room = capacity ? capacity : 50;
    const uint32_t head = r.Below(room);
    const int32_t count = r.Below(8) == 0 ? -int32_t(r.Below(3)) : int32_t(r.Below(room + 6));
    double speed;
    switch (r.Below(4)) {
      case 0:
        speed = double(std::bit_cast<float>(RandomWord(r)));
        break;
      case 1:
        speed = std::bit_cast<double>(r.Next());
        break;
      default:
        speed = double(float(r.Below(20000)) / 100.0f);
        break;
    }
    const uint32_t streaking = r.Below(3) == 2 ? uint32_t(r.Next()) : r.Below(2);
    const uint32_t mode = r.Below(4) == 3 ? uint32_t(r.Next()) : r.Below(3);
    const uint64_t r4 = (uint64_t(RandomWord(r)) << 32) | RandomWord(r);
    const uint64_t r5 = (uint64_t(RandomWord(r)) << 32) | RandomWord(r);
    const uint32_t seed = uint32_t(r.Next());
    const uint64_t fill_seed = r.Next();
    Compare(h, results, {{block, kBytes}, {kSeed, 4}},
            [&, fill_seed] {
              Random fill;
              for (uint64_t n = 0; n < fill_seed % 29; n++) {
                fill.Next();
              }
              FillRandom(h.base, fill, block, kBytes);
              guest::StoreU32(h.base, sparks + 104, capacity);
              guest::StoreU32(h.base, sparks + 28928, fill.Below(5));
              guest::StoreU32(h.base, sparks + 28932, head);
              guest::StoreU32(h.base, sparks + 28940, uint32_t(count));
              for (uint32_t k = 0; k < 400; k++) {
                const uint32_t spark = sparks + 128 + 144 * k;
                uint32_t state;
                switch (fill.Below(4)) {
                  case 0:
                    state = 1;
                    break;
                  case 1:
                    state = 2;
                    break;
                  case 2:
                    state = 0;
                    break;
                  default:
                    state = uint32_t(fill.Next());
                    break;
                }
                if (spark + 144 > block + kBytes || (spark >= sparks + 28928 && spark < sparks + 29824)) {
                  continue;
                }
                guest::StoreU32(h.base, spark + 128, state);
                if (fill.Below(2)) {  // a game-like limit
                  guest::StoreFloat(h.base, spark + 136, float(fill.Below(400)) / 100.0f);
                }
              }
              guest::StoreU32(h.base, kSeed, seed);
            },
            [&](PPCContext& t) {
              t.r3.u64 = sparks;
              t.r4.u64 = r4;
              t.r5.u64 = r5;
              t.r6.u64 = streaking;
              t.r8.u64 = mode;
              t.f1.f64 = speed;
            },
            __imp__sub_8212E660, NativeSparksUpdateStreak,
            fmt::format("case {} capacity {} head {} count {} streaking {:X} mode {:X} speed {:016X}",
                        i, capacity, head, count, streaking, mode,
                        std::bit_cast<uint64_t>(speed)));
  }
  results.Report();
  guest::StoreU32(h.base, kSeed, live);
}

// Sparks::Emit: capacities of 200, small ones, 0 and -1 (the original runs
// there); counts around them (and negative); the head inside them; random
// seeds; tables with game-like and odd floats (inside the block, or
// overlapping the sparks); positions elsewhere or overlapping the new
// spark; the sparks' position and the vector with random words (odd floats
// among them) and game-like values; positive, zero, negative, odd and
// random strengths.
void SparksEmitCases(Context& h, uint32_t block) {
  Random& r = h.random;
  constexpr uint32_t kSeed = 0x82638188;
  const uint32_t live = guest::LoadU32(h.base, kSeed);
  const uint32_t sparks = block + 0x100;
  const uint32_t table = block + 0x40;
  const uint32_t source = block + 0x10000;
  constexpr uint32_t kBytes = 0x10100;
  Results results("Sparks::Emit (sub_8212D818)");
  for (int i = 0; i < 1500; i++) {
    int32_t capacity;
    switch (r.Below(6)) {
      case 0:
        capacity = r.Below(2) ? 0 : -1;
        break;
      case 1:
        capacity = int32_t(1 + r.Below(6));
        break;
      default:
        capacity = 200;
        break;
    }
    const int32_t count = r.Below(6) == 0 ? -int32_t(r.Below(5))
                                          : int32_t(r.Below(capacity > 0 ? capacity + 3 : 5));
    const uint32_t head = r.Below(capacity > 0 ? uint32_t(capacity) : 200);
    const uint32_t spark = sparks + 128 + 144 * head;
    const uint32_t at = r.Below(10) == 0 ? spark + 4 * r.Below(36) : table;
    const uint32_t position = r.Below(8) == 0 ? spark + 4 * r.Below(32) : source + 4 * r.Below(4);
    double strength;
    switch (r.Below(5)) {
      case 0:
        strength = double(std::bit_cast<float>(RandomWord(r)));
        break;
      case 1:
        strength = std::bit_cast<double>(r.Next());
        break;
      case 2:
        strength = r.Below(2) ? 0.0 : -double(r.Below(100));
        break;
      default:
        strength = double(float(r.Below(4000)) / 100.0f);
        break;
    }
    const bool game_like = r.Below(2);
    uint32_t vector[4];
    for (uint32_t& word : vector) {
      word = game_like ? std::bit_cast<uint32_t>(float(int32_t(r.Below(2000)) - 1000) / 10.0f)
                       : RandomWord(r);
    }
    const uint32_t seed = uint32_t(r.Next());
    const uint64_t fill_seed = r.Next();
    const bool game_table = r.Below(2);
    Compare(h, results, {{block, kBytes}, {kSeed, 4}},
            [&, fill_seed] {
              Random fill;
              for (uint64_t n = 0; n < fill_seed % 31; n++) {
                fill.Next();
              }
              FillRandom(h.base, fill, block, kBytes);
              if (game_table) {  // life, size, alpha
                guest::StoreU32(h.base, table, fill.Below(100));
                guest::StoreFloat(h.base, table + 4, float(fill.Below(1000)) / 100.0f);
                guest::StoreFloat(h.base, table + 8, float(fill.Below(256)));
              }
              if (game_like) {  // the sparks' position and the source near the vector
                for (uint32_t k = 0; k < 16; k += 4) {
                  const float near =
                      std::bit_cast<float>(vector[k / 4]) + float(int32_t(fill.Below(200)) - 100) / 10.0f;
                  guest::StoreFloat(h.base, sparks + 28960 + k, near);
                  guest::StoreFloat(h.base, source + k, near);
                }
              }
              guest::StoreU32(h.base, sparks + 104, uint32_t(capacity));
              guest::StoreU32(h.base, sparks + 112, at);
              guest::StoreU32(h.base, sparks + 28936, head);
              guest::StoreU32(h.base, sparks + 28940, uint32_t(count));
              guest::StoreU32(h.base, kSeed, seed);
            },
            [&](PPCContext& t) {
              t.r3.u64 = sparks;
              t.r4.u64 = position;
              t.r6.u64 = (uint64_t(vector[0]) << 32) | vector[1];
              t.r7.u64 = (uint64_t(vector[2]) << 32) | vector[3];
              t.f2.f64 = strength;
            },
            __imp__sub_8212D818, NativeSparksEmit,
            fmt::format("case {} capacity {} count {} head {} strength {:016X}", i, capacity,
                        count, head, std::bit_cast<uint64_t>(strength)));
  }
  results.Report();
  guest::StoreU32(h.base, kSeed, live);
}

// The tire smoke's block: the smoke (32,600 bytes, 45 puffs of 720), its
// table, a frame and the car overlays pointing at it.
constexpr uint32_t kSmoke = 0x100;
constexpr uint32_t kSmokeTable = 0x40;
constexpr uint32_t kSmokeFrame = 0x9000;
constexpr uint32_t kSmokeOverlays = 0xA000;
constexpr uint32_t kSmokeBlockBytes = 0xB000;

double RandomSpeed(Random& r) {
  switch (r.Below(5)) {
    case 0:
      return double(std::bit_cast<float>(RandomWord(r)));
    case 1:
      return std::bit_cast<double>(r.Next());
    case 2:
      return -double(r.Below(100));
    default:
      return double(float(r.Below(20000)) / 100.0f);
  }
}

// Fills the smoke's block: random words (odd floats among them), then the
// capacity, head and count, and (sometimes) a game-like table and puff
// lives close to each other.
void SmokeSetup(Context& h, uint32_t block, uint64_t fill_seed, int32_t capacity, uint32_t head,
                int32_t count, uint32_t table, bool game_like) {
  Random fill;
  for (uint64_t n = 0; n < fill_seed % 37; n++) {
    fill.Next();
  }
  FillRandom(h.base, fill, block, kSmokeBlockBytes);
  const uint32_t smoke = block + kSmoke;
  if (game_like) {
    const uint32_t at = block + kSmokeTable;
    guest::StoreU32(h.base, at, fill.Below(100));
    guest::StoreFloat(h.base, at + 4, float(fill.Below(1000)) / 100.0f);
    guest::StoreFloat(h.base, at + 8, float(fill.Below(256)));
    for (uint32_t k = 0; k < 45; k++) {
      guest::StoreU32(h.base, smoke + 128 + 720 * k + 108, 40 + fill.Below(6));
    }
  }
  guest::StoreU32(h.base, smoke + 104, uint32_t(capacity));
  guest::StoreU32(h.base, smoke + 112, table);
  guest::StoreU32(h.base, smoke + 32532, head);
  guest::StoreU32(h.base, smoke + 32536, uint32_t(count));
  for (uint32_t wheel = 0; wheel < 8; wheel++) {
    guest::StoreU32(h.base, block + kSmokeOverlays + 164 + 4 * wheel, smoke);
  }
}

// Smoke::Emit: capacities of 45 (the room the object has), small ones, 0
// and -1 (the original runs there); counts around them (and negative); the
// head anywhere inside them; the table in the block or overlapping the
// smoke; game-like and random puff lives (the particles' two placements);
// sizes, speeds and frames with game-like, odd and random values; the
// region flag 0, 1 or random.
// CarOverlays::EmitSmoke: wheels 0-7; rates of 0.0, game-like, odd and
// random; accumulators game-like or random; the smoke as above.
void SmokeCases(Context& h, uint32_t block) {
  Random& r = h.random;
  constexpr uint32_t kSeed = 0x82638188;
  const uint32_t live = guest::LoadU32(h.base, kSeed);
  const uint32_t smoke = block + kSmoke;
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "Smoke::Emit (sub_8212A350)"
                               : "CarOverlays::EmitSmoke (sub_82123240)");
    for (int i = 0; i < 1200; i++) {
      int32_t capacity;
      switch (r.Below(6)) {
        case 0:
          capacity = r.Below(2) ? 0 : -1;
          break;
        case 1:
          capacity = int32_t(1 + r.Below(6));
          break;
        default:
          capacity = 45;
          break;
      }
      const int32_t count = r.Below(6) == 0 ? -int32_t(r.Below(5))
                                            : int32_t(r.Below(capacity > 0 ? capacity + 3 : 5));
      const uint32_t head = r.Below(capacity > 0 ? uint32_t(capacity) : 45);
      const uint32_t table =
          r.Below(10) == 0 ? smoke + 128 + 720 * head + 4 * r.Below(180) : block + kSmokeTable;
      const bool game_like = r.Below(2);
      const double speed = RandomSpeed(r);
      const uint32_t value = uint32_t(r.Next());
      const uint32_t seed = uint32_t(r.Next());
      const uint64_t fill_seed = r.Next();
      double first;  // the size, or the rate
      switch (r.Below(5)) {
        case 0:
          first = which == 0 ? double(1 + r.Below(4)) : 0.0;
          break;
        case 1:
          first = double(std::bit_cast<float>(RandomWord(r)));
          break;
        case 2:
          first = std::bit_cast<double>(r.Next());
          break;
        default:
          first = double(float(r.Below(300)) / 100.0f);
          break;
      }
      const uint32_t region = r.Below(3) == 2 ? uint32_t(r.Next()) : r.Below(2);
      const uint32_t wheel = r.Below(8);
      const bool game_accumulator = r.Below(2);
      const uint32_t accumulator_word =
          game_accumulator ? std::bit_cast<uint32_t>(float(r.Below(100)) / 100.0f) : RandomWord(r);
      Compare(h, results, {{block, kSmokeBlockBytes}, {kSeed, 4}},
              [&, fill_seed] {
                SmokeSetup(h, block, fill_seed, capacity, head, count, table, game_like);
                if (game_like) {
                  guest::StoreFloat(h.base, block + kSmokeFrame + 84,
                                    float(fill_seed % 1000) / 100.0f);
                }
                guest::StoreU32(h.base, block + kSmokeOverlays + 528 + 48 * wheel,
                                accumulator_word);
                guest::StoreU32(h.base, kSeed, seed);
              },
              [&](PPCContext& t) {
                t.r3.u64 = which == 0 ? smoke : block + kSmokeOverlays;
                t.f1.f64 = first;
                t.f2.f64 = speed;
                t.r5.u64 = block + kSmokeFrame;
                t.r6.u64 = wheel;
                t.r7.u64 = region;
                t.r8.u64 = value;
              },
              which == 0 ? __imp__sub_8212A350 : __imp__sub_82123240,
              which == 0 ? NativeSmokeEmit : NativeEmitSmoke,
              fmt::format("case {} capacity {} count {} head {} first {:016X} speed {:016X} "
                          "region {:X} wheel {}",
                          i, capacity, count, head, std::bit_cast<uint64_t>(first),
                          std::bit_cast<uint64_t>(speed), region, wheel));
    }
    results.Report();
  }
  guest::StoreU32(h.base, kSeed, live);
}


// The set-up's constructors, clears and bindings: objects of random words
// (odd floats among them), compared over the object and around it.
void SetUpCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t object = block + 0x100;
  const Regions around = {{object - 0x10, 0x100}};

  {
    const struct {
      const char* name;
      GuestFunction recompiled, native;
    } plain[] = {
        {"Overlay::Overlay (sub_82126498)", __imp__sub_82126498, NativeOverlayConstruct},
        {"CarOverlays::ClearViews (sub_82126530)", __imp__sub_82126530, NativeClearViews},
        {"ViewDesc::Clear (sub_82126860)", __imp__sub_82126860, NativeViewDescClear},
        {"OverlayView::OverlayView (sub_82126870)", __imp__sub_82126870,
         NativeOverlayViewConstruct},
        {"Particle::Particle (sub_8212B3D0)", __imp__sub_8212B3D0, NativeParticleConstruct},
        {"RaceOverlays::Clear (sub_8215DE80)", __imp__sub_8215DE80, NativeRaceOverlaysClear},
    };
    for (const auto& f : plain) {
      Results results(f.name);
      for (int i = 0; i < 20; i++) {
        const uint64_t seed = r.Next();
        Compare(h, results, around,
                [&, seed] {
                  Random fill;
                  for (uint64_t n = 0; n < seed % 61; n++) {
                    fill.Next();
                  }
                  FillRandom(h.base, fill, object - 0x10, 0x100);
                },
                [&](PPCContext& t) { t.r3.u64 = object; }, f.recompiled, f.native,
                fmt::format("case {}", i));
      }
      results.Report();
    }
  }

  // OverlayView::Set: the description elsewhere, or overlapping the view's
  // words by -8 to +8 bytes.
  {
    Results results("OverlayView::Set (sub_82126890)");
    for (int i = 0; i < 40; i++) {
      const uint64_t seed = r.Next();
      const uint32_t desc = i < 20 ? object + 0x80 : object + 4 + 4 * (i % 5) - 8;
      Compare(h, results, around,
              [&, seed] {
                Random fill;
                for (uint64_t n = 0; n < seed % 61; n++) {
                  fill.Next();
                }
                FillRandom(h.base, fill, object - 0x10, 0x100);
              },
              [&](PPCContext& t) {
                t.r3.u64 = object;
                t.r4.u64 = desc;
              },
              __imp__sub_82126890, NativeOverlayViewSet, fmt::format("case {}", i));
    }
    results.Report();
  }

  // CarOverlays::BindViewport: the viewport entries 0-5 of the game's table,
  // second indices 0-8, the owners' table (0x826C2CE8) pointed at random
  // words in the block and put back.
  {
    constexpr uint32_t kViewportOwners = 0x826C2CE8;
    const uint32_t owners = guest::LoadU32(h.base, kViewportOwners);
    const uint32_t table = block + 0x1000;
    Results results("CarOverlays::BindViewport (sub_821264C0)");
    for (uint32_t index = 0; index < 6; index++) {
      for (uint32_t second = 0; second < 9; second++) {
        const uint64_t seed = r.Next();
        const uint64_t r3 = r.Next();  // not read
        Compare(h, results, around,
                [&, seed] {
                  Random fill;
                  for (uint64_t n = 0; n < seed % 61; n++) {
                    fill.Next();
                  }
                  FillRandom(h.base, fill, object - 0x10, 0x100);
                  FillRandom(h.base, fill, table, 0x1000);
                  guest::StoreU32(h.base, kViewportOwners, table);
                },
                [&](PPCContext& t) {
                  t.r3.u64 = r3;
                  t.r4.u64 = object;
                  t.r5.u64 = index;
                  t.r6.u64 = second;
                },
                __imp__sub_821264C0, NativeBindViewport,
                fmt::format("entry {} second {}", index, second));
      }
    }
    guest::StoreU32(h.base, kViewportOwners, owners);
    results.Report();
  }

  // Overlays::SetRenderTargets: random words, the two globals put back.
  {
    constexpr uint32_t kTargets = 0x826C5D5C;
    const uint32_t queue = guest::LoadU32(h.base, kTargets);
    const uint32_t list = guest::LoadU32(h.base, kTargets + 4);
    Results results("Overlays::SetRenderTargets (sub_82128190)");
    for (int i = 0; i < 20; i++) {
      const uint64_t a = r.Next(), b = r.Next();
      Compare(h, results, {{kTargets - 8, 24}}, [] {},
              [&](PPCContext& t) {
                t.r3.u64 = a;
                t.r4.u64 = b;
              },
              __imp__sub_82128190, NativeSetRenderTargets, fmt::format("case {}", i));
    }
    guest::StoreU32(h.base, kTargets, queue);
    guest::StoreU32(h.base, kTargets + 4, list);
    results.Report();
  }

  // RaceOverlays::ShareTextures: a list whose entries are 0 or overlays in
  // the block (overlay 0 with a random texture table at +104), entry 24 set
  // or not.
  {
    const uint32_t list = block + 0x2000;
    const uint32_t overlays = block + 0x3000;  // 25 of 0x100 bytes
    Results results("RaceOverlays::ShareTextures (sub_8215E000)");
    for (int i = 0; i < 40; i++) {
      const uint64_t seed = r.Next();
      Compare(h, results, {{list, 100}, {overlays, 25 * 0x100}},
              [&, seed, i] {
                Random fill;
                for (uint64_t n = 0; n < seed % 61; n++) {
                  fill.Next();
                }
                FillRandom(h.base, fill, overlays, 25 * 0x100);
                for (uint32_t n = 0; n < 25; n++) {
                  const bool set = n == 0 || (n == 24 ? i % 2 == 0 : fill.Below(3) != 0);
                  guest::StoreU32(h.base, list + 4 * n, set ? overlays + 0x100 * n : 0);
                }
              },
              [&](PPCContext& t) { t.r3.u64 = list; }, __imp__sub_8215E000, NativeShareTextures,
              fmt::format("case {}", i));
    }
    results.Report();
  }
}

// The resets: objects of random words (odd floats among them), at 16-byte
// alignment and 4, 8 and 12 past it (stvx rounds down), the counts the
// loops read from 0 to past the game's capacities where that stays inside
// the object; compared over the object and 32 bytes around it.
void ResetCases(Context& h, uint32_t block) {
  Random& r = h.random;
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t bytes;      // compared
    bool in_r4;          // the object in r4 (r3 an owner, not read)
    uint32_t count;      // the count word the loop reads (0: none)
    uint32_t max_count;  // counts tried up to this
  };
  const Function resets[] = {
      {"LightSprite::Reset (sub_8211E9F0)", __imp__sub_8211E9F0, NativeLightSpriteReset, 288,
       false, 0, 0},
      {"ModelSprite::Reset (sub_8211F430)", __imp__sub_8211F430, NativeModelSpriteReset, 576,
       false, 0, 0},
      {"Sprite::ResetStyle2 (sub_82123A00)", __imp__sub_82123A00, NativeSpriteResetStyle2, 128,
       false, 0, 0},
      {"Trail::Reset (sub_82129498)", __imp__sub_82129498, NativeTrailReset, 66816, false, 104,
       520},
      {"SmokeEmitter::Reset (sub_8212A218)", __imp__sub_8212A218, NativeSmokeEmitterReset, 720,
       true, 0, 0},
      {"SmokeEmitter::SmokeEmitter (sub_8212B230)", __imp__sub_8212B230,
       NativeSmokeEmitterConstruct, 720, false, 0, 0},
      {"ParticlesB::ResetEntry (sub_8212B3E8)", __imp__sub_8212B3E8, NativeParticlesBResetEntry,
       176, true, 0, 0},
      // Entry 20 would overwrite the count (+2,700) with 1.0: both versions
      // would run on, so 20 at most.
      {"ParticlesC::Reset (sub_8212C0E8)", __imp__sub_8212C0E8, NativeParticlesCReset, 2720,
       false, 2700, 20},
      {"Marker::Reset (sub_8212CC38)", __imp__sub_8212CC38, NativeMarkerReset, 304, false, 0, 0},
      {"Sparks::Reset (sub_8212D6E0)", __imp__sub_8212D6E0, NativeSparksReset, 29824, false, 104,
       200},
      {"LightSprite::LightSprite (sub_8211F060)", __imp__sub_8211F060, NativeLightSpriteConstruct,
       288, false, 0, 0},
      {"ModelSprite::ModelSprite (sub_82122880)", __imp__sub_82122880, NativeModelSpriteConstruct,
       576, false, 0, 0},
      {"Trail::Trail (sub_8212A140)", __imp__sub_8212A140, NativeTrailConstruct, 64160, false, 0,
       0},
      {"Smoke::Smoke (sub_8212B2E8)", __imp__sub_8212B2E8, NativeSmokeConstruct, 32640, false, 0,
       0},
      {"ParticlesC::ParticlesC (sub_8212CAF8)", __imp__sub_8212CAF8, NativeParticlesCConstruct,
       2720, false, 0, 0},
      {"Marker::Marker (sub_8212D620)", __imp__sub_8212D620, NativeMarkerConstruct, 304, false, 0,
       0},
      {"Sparks::Sparks (sub_8212E9A0)", __imp__sub_8212E9A0, NativeSparksConstruct, 29824, false,
       0, 0},
      // The smoke holds 45 emitters; ParticlesB's entries run into its
      // header from the 16th on (both versions alike), 20 at most.
      {"Smoke::Reset (sub_8212A9E0)", __imp__sub_8212A9E0, NativeSmokeReset, 32640, false, 104,
       45},
      {"ParticlesB::Reset (sub_8212BA98)", __imp__sub_8212BA98, NativeParticlesBReset, 3648, false,
       104, 20},
  };
  for (const Function& f : resets) {
    Results results(f.name);
    for (int i = 0; i < 24; i++) {
      const uint32_t object = block + 0x100 + 4 * uint32_t(i % 4);
      const uint64_t seed = r.Next();
      uint32_t count = 0;
      if (f.count) {
        count = i < 4 ? uint32_t(i) : i == 4 ? f.max_count : r.Below(f.max_count + 1);
      }
      const uint64_t r3 = r.Next();
      Compare(h, results, {{object - 32, f.bytes + 64}},
              [&, seed, count] {
                Random fill;
                for (uint64_t n = 0; n < seed % 61; n++) {
                  fill.Next();
                }
                FillRandom(h.base, fill, object - 32, f.bytes + 64);
                if (f.count) {
                  guest::StoreU32(h.base, object + f.count, count);
                }
              },
              [&](PPCContext& t) {
                if (f.in_r4) {
                  t.r3.u64 = r3;
                  t.r4.u64 = object;
                } else {
                  t.r3.u64 = object;
                }
              },
              f.recompiled, f.native, fmt::format("case {} count {}", i, count));
    }
    results.Report();
  }
}

// Smoke::InRegion: the region's points and the point random words (odd
// floats among them), or small numbers so the point falls on either side
// of each end and between them.
void SmokeInRegionCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t smoke = block + 0x1000;
  const uint32_t point = block + 0x100;
  Results results("Smoke::InRegion (sub_8212A868)");
  for (int i = 0; i < 400; i++) {
    const uint64_t seed = r.Next();
    const bool small = i % 2 == 0;
    Compare(h, results, {{point, 16}, {smoke + 32592, 32}},
            [&, seed, small] {
              Random fill;
              for (uint64_t n = 0; n < seed % 61; n++) {
                fill.Next();
              }
              const uint32_t at[] = {smoke + 32592, smoke + 32596, smoke + 32600,
                                     smoke + 32608, smoke + 32612, smoke + 32616,
                                     point,         point + 4,     point + 8};
              for (uint32_t a : at) {
                const float value = float(int32_t(fill.Below(21)) - 10) * 0.5f;
                guest::StoreU32(h.base, a,
                                small ? std::bit_cast<uint32_t>(value) : RandomWord(fill));
              }
              guest::StoreU32(h.base, smoke + 32604, RandomWord(fill));
              guest::StoreU32(h.base, point + 12, RandomWord(fill));
            },
            [&](PPCContext& t) {
              t.r3.u64 = smoke;
              t.r4.u64 = point;
            },
            __imp__sub_8212A868, NativeSmokeInRegion, fmt::format("case {}", i));
  }
  results.Report();
}

// Sparks::Update: entries of random words with small lives (0 to 20, some
// negative or huge), a table (+112) with a random +8 (odd floats among
// them) and a +12 of 0 to 30 or random; capacities 1 to 200, heads inside
// and past them, counts up to past the capacity; and the cases where the
// native version declines (a capacity of 0 or -1, a head below 0).
void SparksUpdateCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t sparks = block + 0x1000;
  const uint32_t table = block + 0x100;
  constexpr uint32_t kBytes = 29824;
  Results results("Sparks::Update (sub_8212DDC8)");
  for (int i = 0; i < 300; i++) {
    const uint64_t seed = r.Next();
    int32_t capacity = int32_t(1 + r.Below(200));
    int32_t head = int32_t(r.Below(uint32_t(capacity)));
    int32_t count = int32_t(r.Below(uint32_t(capacity) + 3));
    if (i % 25 == 1) {
      head = capacity + int32_t(r.Below(5));  // past the capacity
      head = std::min(head, 199);
    } else if (i % 25 == 2) {
      capacity = 0;
    } else if (i % 25 == 3) {
      capacity = -1;
    } else if (i % 25 == 4) {
      head = -1 - int32_t(r.Below(3));
    } else if (i % 25 == 5) {
      count = -int32_t(r.Below(3));
    }
    Compare(h, results, {{sparks, kBytes}, {table, 16}},
            [&, seed, capacity, head, count] {
              Random fill;
              for (uint64_t n = 0; n < seed % 61; n++) {
                fill.Next();
              }
              FillRandom(h.base, fill, sparks, kBytes);
              FillRandom(h.base, fill, table, 16);
              const uint32_t ticks = fill.Below(4) == 0 ? uint32_t(fill.Next()) : fill.Below(31);
              guest::StoreU32(h.base, table + 12, ticks);
              guest::StoreU32(h.base, sparks + 104, uint32_t(capacity));
              guest::StoreU32(h.base, sparks + 112, table);
              guest::StoreU32(h.base, sparks + 28932, uint32_t(head));
              guest::StoreU32(h.base, sparks + 28940, uint32_t(count));
              for (uint32_t n = 0; n < 200; n++) {
                const uint32_t kind = fill.Below(10);
                const uint32_t life = kind == 0   ? uint32_t(fill.Next())
                                      : kind == 1 ? uint32_t(-int32_t(fill.Below(3)))
                                                  : fill.Below(21);
                guest::StoreU32(h.base, sparks + 128 + 144 * n + 108, life);
              }
            },
            [&](PPCContext& t) { t.r3.u64 = sparks; }, __imp__sub_8212DDC8, NativeSparksUpdate,
            fmt::format("case {} capacity {} head {} count {}", i, capacity, head, count));
  }
  results.Report();
}

// Smoke::Update: emitters of random words with small lives (0 to 50, some
// negative or huge) and particle counts (0 to 12), a table (+112) with a
// life (+0) of 40 to 50 or random and a +12 of 0 to 30 or random;
// capacities 1 to 45, heads inside and past them (below 45), counts up to
// past the capacity; the region on, off or random, with the region's
// points and the particles small numbers in half the cases (so particles
// fall inside and outside it); and the cases where the native version
// declines (a capacity of 0 or -1). Heads below 0, where it declines too,
// aren't tried: the original then resets emitters below the smoke, over its
// capacity and table pointer, and runs on through random memory.
void SmokeUpdateCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t smoke = block + 0x1000;
  const uint32_t table = block + 0x100;
  // The smoke and five emitters past it: with a capacity of 0 (the
  // original runs, its divisions giving 0) the index runs on past the 45.
  constexpr uint32_t kBytes = 32640 + 5 * 720;
  Results results("Smoke::Update (sub_8212AA78)");
  for (int i = 0; i < 300; i++) {
    const uint64_t seed = r.Next();
    int32_t capacity = int32_t(1 + r.Below(45));
    int32_t head = int32_t(r.Below(uint32_t(capacity)));
    int32_t count = int32_t(r.Below(uint32_t(capacity) + 3));
    if (i % 25 == 1) {
      head = std::min(capacity + int32_t(r.Below(5)), 44);  // past the capacity
    } else if (i % 25 == 2) {
      capacity = 0;
      count = int32_t(r.Below(3));  // the index runs on by one an emitter
    } else if (i % 25 == 3) {
      capacity = -1;
    } else if (i % 25 == 5) {
      count = -int32_t(r.Below(3));
    }
    const bool small = i % 2 == 0;
    const uint32_t region = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2);
    Compare(h, results, {{smoke, kBytes}, {table, 16}},
            [&, seed, capacity, head, count, small, region] {
              Random fill;
              for (uint64_t n = 0; n < seed % 61; n++) {
                fill.Next();
              }
              auto small_float = [&] {
                return std::bit_cast<uint32_t>(float(int32_t(fill.Below(21)) - 10) * 0.5f);
              };
              FillRandom(h.base, fill, smoke, kBytes);
              FillRandom(h.base, fill, table, 16);
              if (fill.Below(4) != 0) {
                guest::StoreU32(h.base, table, 40 + fill.Below(11));
              }
              const uint32_t ticks = fill.Below(4) == 0 ? uint32_t(fill.Next()) : fill.Below(31);
              guest::StoreU32(h.base, table + 12, ticks);
              guest::StoreU32(h.base, smoke + 104, uint32_t(capacity));
              guest::StoreU32(h.base, smoke + 112, table);
              guest::StoreU32(h.base, smoke + 32528, uint32_t(head));
              guest::StoreU32(h.base, smoke + 32536, uint32_t(count));
              guest::StoreU32(h.base, smoke + 32624, region);
              for (uint32_t n = 0; n < 50; n++) {
                const uint32_t emitter = smoke + 128 + 720 * n;
                const uint32_t kind = fill.Below(10);
                const uint32_t life = kind == 0   ? uint32_t(fill.Next())
                                      : kind == 1 ? uint32_t(-int32_t(fill.Below(3)))
                                                  : fill.Below(51);
                guest::StoreU32(h.base, emitter + 108, life);
                guest::StoreU32(h.base, emitter + 128, fill.Below(13));
                if (small) {
                  for (uint32_t p = 0; p < 10; p++) {
                    for (uint32_t k = 0; k < 6; k++) {
                      guest::StoreU32(h.base, emitter + 240 + 48 * p + (k < 3 ? 4 * k : 4 + 4 * k),
                                      small_float());
                    }
                  }
                }
              }
              if (small) {
                for (uint32_t k = 0; k < 3; k++) {
                  guest::StoreU32(h.base, smoke + 32592 + 4 * k, small_float());
                  guest::StoreU32(h.base, smoke + 32608 + 4 * k, small_float());
                }
              }
            },
            [&](PPCContext& t) { t.r3.u64 = smoke; }, __imp__sub_8212AA78, NativeSmokeUpdate,
            fmt::format("case {} capacity {} head {} count {} region {:X}", i, capacity, head,
                        count, region));
  }
  results.Report();
}

// ParticlesB::Update: entries of random words with small lives (0 to 50,
// some negative or huge) and particle counts (0 to 3), a table (+112) with
// a life (+0) of 40 to 50, 0 or random, a +4 small or random and a +12 of 0
// to 30 or random; the effect's +2,800 small numbers (some zero) or random;
// capacities 1 to 15 (the room before the effect's own fields), heads
// inside and past them, counts up to past the capacity; and the cases where
// the native version declines (a capacity of 0 or -1). Heads below 0, where
// it declines too, aren't tried (the original would reset entries over the
// effect's capacity and table pointer).
void ParticlesBUpdateCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t effect = block + 0x1000;
  const uint32_t table = block + 0x100;
  // The effect and five entries past its 15 (the index runs on past the
  // capacity when it's 0: the original's divisions give 0).
  constexpr uint32_t kBytes = 128 + 176 * 21;
  Results results("ParticlesB::Update (sub_8212B748)");
  for (int i = 0; i < 300; i++) {
    const uint64_t seed = r.Next();
    int32_t capacity = int32_t(1 + r.Below(15));
    int32_t head = int32_t(r.Below(uint32_t(capacity)));
    int32_t count = int32_t(r.Below(uint32_t(capacity) + 3));
    if (i % 25 == 1) {
      head = std::min(capacity + int32_t(r.Below(5)), 14);  // past the capacity
    } else if (i % 25 == 2) {
      capacity = 0;
      count = int32_t(r.Below(3));  // the index runs on by one an entry
    } else if (i % 25 == 3) {
      capacity = -1;
    } else if (i % 25 == 5) {
      count = -int32_t(r.Below(3));
    }
    const bool small = i % 2 == 0;
    Compare(h, results, {{effect, kBytes}, {table, 16}},
            [&, seed, capacity, head, count, small] {
              Random fill;
              for (uint64_t n = 0; n < seed % 61; n++) {
                fill.Next();
              }
              auto small_float = [&] {
                return std::bit_cast<uint32_t>(float(int32_t(fill.Below(21)) - 10) * 0.5f);
              };
              FillRandom(h.base, fill, effect, kBytes);
              FillRandom(h.base, fill, table, 16);
              const uint32_t life_kind = fill.Below(5);
              if (life_kind != 0) {
                guest::StoreU32(h.base, table, life_kind == 1 ? 0 : 40 + fill.Below(11));
              }
              if (small) {
                guest::StoreU32(h.base, table + 4, small_float());
              }
              const uint32_t ticks = fill.Below(4) == 0 ? uint32_t(fill.Next()) : fill.Below(31);
              guest::StoreU32(h.base, table + 12, ticks);
              guest::StoreU32(h.base, effect + 104, uint32_t(capacity));
              guest::StoreU32(h.base, effect + 112, table);
              for (uint32_t n = 0; n < 21; n++) {
                const uint32_t entry = effect + 128 + 176 * n;
                const uint32_t kind = fill.Below(10);
                const uint32_t life = kind == 0   ? uint32_t(fill.Next())
                                      : kind == 1 ? uint32_t(-int32_t(fill.Below(3)))
                                                  : fill.Below(51);
                guest::StoreU32(h.base, entry + 108, life);
                guest::StoreU32(h.base, entry + 116, fill.Below(4));
                if (small) {
                  for (uint32_t k = 0; k < 3; k++) {
                    guest::StoreU32(h.base, entry + 128 + 4 * k, small_float());
                  }
                }
              }
              // The effect's own fields, over entry 15's first bytes.
              guest::StoreU32(h.base, effect + 2768, uint32_t(head));
              guest::StoreU32(h.base, effect + 2776, uint32_t(count));
              if (small || fill.Below(2)) {
                for (uint32_t k = 0; k < 3; k++) {
                  guest::StoreU32(h.base, effect + 2800 + 4 * k,
                                  fill.Below(5) == 0 ? 0 : small_float());
                }
              }
            },
            [&](PPCContext& t) { t.r3.u64 = effect; }, __imp__sub_8212B748,
            NativeParticlesBUpdate,
            fmt::format("case {} capacity {} head {} count {}", i, capacity, head, count));
  }
  results.Report();
}

// ParticlesC::Update: entries of random words with small lives (0 to 50,
// some negative or huge), a table (+108) with a +8 small or random, a +4
// small or random and a +12 of 0 to 30 or random; the entries' positions
// (+48) and the effect's +2,672 small numbers (some the same, so lengths of
// 0.0) or random, its +2,704 small or random; capacities 1 to 20 (the
// entries before the effect's own fields), oldest and newest inside and past
// them, counts up to past the capacity and negative; the game's rand seed
// set for each case and put back after; and the case where the native
// version declines (a capacity of 0, with a count of 0 so the original
// doesn't run on).
void ParticlesCUpdateCases(Context& h, uint32_t block) {
  Random& r = h.random;
  constexpr uint32_t kSeed = 0x82638188;
  const uint32_t live = guest::LoadU32(h.base, kSeed);
  const uint32_t effect = block + 0x1000;
  const uint32_t table = block + 0x100;
  constexpr uint32_t kBytes = 2720;
  Results results("ParticlesC::Update (sub_8212C2F0)");
  for (int i = 0; i < 300; i++) {
    const uint64_t seed = r.Next();
    uint32_t capacity = 1 + r.Below(20);
    uint32_t oldest = r.Below(capacity);
    uint32_t newest = r.Below(capacity);
    int32_t count = int32_t(r.Below(capacity + 3));
    if (i % 25 == 1) {
      oldest = std::min(capacity + r.Below(5), 19u);  // past the capacity
      newest = std::min(capacity + r.Below(5), 19u);
    } else if (i % 25 == 2) {
      capacity = 0;
      count = 0;
    } else if (i % 25 == 3) {
      count = -int32_t(r.Below(3));
    }
    const bool small = i % 2 == 0;
    const uint32_t rand_seed = uint32_t(r.Next());
    Compare(h, results, {{effect, kBytes}, {table, 16}, {kSeed, 4}},
            [&, seed, capacity, oldest, newest, count, small, rand_seed] {
              Random fill;
              for (uint64_t n = 0; n < seed % 61; n++) {
                fill.Next();
              }
              auto small_float = [&] {
                return std::bit_cast<uint32_t>(float(int32_t(fill.Below(21)) - 10) * 0.5f);
              };
              FillRandom(h.base, fill, effect, kBytes);
              FillRandom(h.base, fill, table, 16);
              if (small) {
                guest::StoreU32(h.base, table + 4, small_float());
                guest::StoreU32(h.base, table + 8,
                                std::bit_cast<uint32_t>(float(fill.Below(256))));
                guest::StoreU32(h.base, effect + 2704, small_float());
              }
              const uint32_t ticks = fill.Below(4) == 0 ? uint32_t(fill.Next()) : fill.Below(31);
              guest::StoreU32(h.base, table + 12, ticks);
              guest::StoreU32(h.base, effect + 108, table);
              for (uint32_t n = 0; n < 20; n++) {
                const uint32_t entry = effect + 112 + 128 * n;
                const uint32_t kind = fill.Below(10);
                const uint32_t life = kind == 0   ? uint32_t(fill.Next())
                                      : kind == 1 ? uint32_t(-int32_t(fill.Below(3)))
                                                  : fill.Below(51);
                guest::StoreU32(h.base, entry + 108, life);
                if (small) {
                  for (uint32_t k = 0; k < 3; k++) {
                    guest::StoreU32(h.base, entry + 48 + 4 * k,
                                    fill.Below(6) == 0 ? 0 : small_float());
                  }
                }
              }
              if (small) {
                for (uint32_t k = 0; k < 3; k++) {
                  guest::StoreU32(h.base, effect + 2672 + 4 * k, small_float());
                }
              }
              guest::StoreU32(h.base, effect + 2688, oldest);
              guest::StoreU32(h.base, effect + 2692, newest);
              guest::StoreU32(h.base, effect + 2696, uint32_t(count));
              guest::StoreU32(h.base, effect + 2700, capacity);
              guest::StoreU32(h.base, kSeed, rand_seed);
            },
            [&](PPCContext& t) { t.r3.u64 = effect; }, __imp__sub_8212C2F0,
            NativeParticlesCUpdate,
            fmt::format("case {} capacity {} oldest {} newest {} count {}", i, capacity, oldest,
                        newest, count));
  }
  guest::StoreU32(h.base, kSeed, live);
  results.Report();
}

// RaceOverlays::SlotOf: indices 0 to 40, the edges of the 32-bit range and
// random words (with random upper halves of r3, which the original's
// cmplwi doesn't read).
void SlotOfCases(Context& h) {
  Random& r = h.random;
  std::vector<uint32_t> indices = {0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0xFFFFFFE8};
  for (uint32_t i = 0; i <= 40; i++) {
    indices.push_back(i);
  }
  for (int i = 0; i < 40; i++) {
    indices.push_back(uint32_t(r.Next()));
  }
  Results results("RaceOverlays::SlotOf (sub_8215E048)");
  for (uint32_t index : indices) {
    const uint64_t upper = r.Next() << 32;
    Compare(h, results, {}, [] {}, [&](PPCContext& t) { t.r3.u64 = upper | index; },
            __imp__sub_8215E048, NativeSlotOf, fmt::format("index {:X}", index));
  }
  results.Report();
}

// The view's body and wheels: a view, its vehicle, the vehicle's body
// (+336) and wheels' data (+2160 + 4 x wheel) in the block. The originals
// run with the vehicle accessors off too (Original).
constexpr uint32_t kBodyView = 0x60000, kBodyVehicle = 0x61000, kBodyBody = 0x64000,
                   kBodyWheels = 0x64800, kBodyOut = 0x65000;

// Plain numbers (multiples of 1/16 from -64 to 64), or random words with odd
// floats among them.
uint32_t NumberOrWord(Random& r, bool odd) {
  if (odd) {
    return RandomWord(r);
  }
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(2049)) - 1024) / 16.0f);
}

// The view, its vehicle, body and wheels' data, from `seed`: numbers, or
// random words with odd floats among them.
auto ViewBodySetup(Context& h, uint32_t block, uint64_t seed, bool odd) {
  return [&h, block, seed, odd] {
    const uint32_t view = block + kBodyView;
    const uint32_t vehicle = block + kBodyVehicle;
    const uint32_t body = block + kBodyBody;
    Random fill;
    for (uint64_t n = 0; n < seed % 97; n++) {
      fill.Next();
    }
    for (uint32_t i = 0; i < 0x2000; i += 4) {
      guest::StoreU32(h.base, vehicle + i, NumberOrWord(fill, odd));
    }
    for (uint32_t i = 0; i < 0x500; i += 4) {
      guest::StoreU32(h.base, body + i, NumberOrWord(fill, odd));
    }
    for (uint32_t i = 0; i < 5 * 64; i += 4) {
      guest::StoreU32(h.base, block + kBodyWheels + i, NumberOrWord(fill, odd));
    }
    FillRandom(h.base, fill, block + kBodyOut, 64);
    guest::StoreU32(h.base, view + 4, vehicle);
    guest::StoreU32(h.base, vehicle + 336, body);
    for (uint32_t wheel = 0; wheel < 5; wheel++) {
      guest::StoreU32(h.base, vehicle + 2160 + 4 * wheel, block + kBodyWheels + 64 * wheel);
      guest::StoreU8(h.base, vehicle + 433 + 368 * wheel, fill.Below(4) == 0 ? 0 : 1);
    }
  };
}

void ViewBodyCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t view = block + kBodyView;
  const uint32_t vehicle = block + kBodyVehicle;
  const uint32_t body = block + kBodyBody;
  auto setup = [&h, block](uint64_t seed, bool odd) {
    return ViewBodySetup(h, block, seed, odd);
  };

  {
    Results results("OverlayView::BodyMatrix (sub_821268B0)");
    for (uint64_t seed = 0; seed < 10; seed++) {
      Compare(h, results, {}, setup(seed, seed % 2),
              [&](PPCContext& t) { t.r3.u64 = view; }, Original<__imp__sub_821268B0>,
              NativeBodyMatrix, fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("OverlayView::WheelContact (sub_821268C8)");
    for (uint32_t wheel = 0; wheel < 6; wheel++) {
      for (uint64_t seed = 0; seed < 4; seed++) {
        Compare(h, results, {}, setup(seed, false),
                [&](PPCContext& t) {
                  t.r3.u64 = view;
                  t.r4.u64 = wheel;
                },
                Original<__imp__sub_821268C8>, NativeWheelContact,
                fmt::format("wheel {} seed {}", wheel, seed));
      }
    }
    results.Report();
  }
  {
    // Each surface from -2 to 12, the halfword's edges and random ones.
    Results results("OverlayView::WheelSurfaceMarks (sub_821269B0)");
    std::vector<uint16_t> surfaces = {0x7FFF, 0x8000, 0xFFFF, 0x0100, 0x0108};
    for (int32_t s = -2; s <= 12; s++) {
      surfaces.push_back(uint16_t(s));
    }
    for (int i = 0; i < 20; i++) {
      surfaces.push_back(uint16_t(r.Next()));
    }
    for (uint32_t wheel = 0; wheel < 6; wheel++) {
      for (uint16_t surface : surfaces) {
        Compare(h, results, {},
                [&, wheel, surface] {
                  setup(wheel, true)();
                  guest::StoreU16(h.base, vehicle + 5328 + 96 * wheel + 80, surface);
                },
                [&](PPCContext& t) {
                  t.r3.u64 = view;
                  t.r4.u64 = wheel;
                },
                Original<__imp__sub_821269B0>, NativeWheelSurfaceMarks,
                fmt::format("wheel {} surface {:04X}", wheel, surface));
      }
    }
    results.Report();
  }
  {
    // As Vehicle::WheelSlip's cases: with NaNs among the inputs, its NaNs
    // differ in sign and payload between builds of the same operations
    // (vehicle-accessors.md), so those cases are kept apart.
    Results results("OverlayView::WheelSlip (sub_82126A70)");
    Results nan_results("OverlayView::WheelSlip (sub_82126A70), NaN inputs (expected to differ)");
    for (uint64_t seed = 0; seed < 60; seed++) {
      const bool odd = seed % 4 == 3;
      for (uint32_t wheel = 0; wheel < 5; wheel++) {
        const double f1 = seed % 2 ? double(std::bit_cast<float>(NumberOrWord(r, false)))
                                   : double(float(r.Below(100)) / 100.0f);
        Compare(h, odd ? nan_results : results, {}, setup(seed, odd),
                [&](PPCContext& t) {
                  t.r3.u64 = view;
                  t.r4.u64 = wheel;
                  t.f1.f64 = f1;
                },
                Original<__imp__sub_82126A70>, NativeWheelSlip,
                fmt::format("seed {} wheel {} f1 {:016X}", seed, wheel,
                            std::bit_cast<uint64_t>(f1)),
                false);
      }
    }
    results.Report();
    nan_results.Report();
  }
  {
    // Out apart from the body, or over its position or matrix.
    const struct {
      const char* name;
      GuestFunction recompiled, native;
      uint32_t bytes, field;
    } copies[] = {
        {"OverlayView::BodyPosition (sub_82126F80)", Original<__imp__sub_82126F80>,
         NativeBodyPosition, 16, 208},
        {"OverlayView::BodyMatrixCopy (sub_82126FF0)", Original<__imp__sub_82126FF0>,
         NativeBodyMatrixCopy, 64, 288},
    };
    for (const auto& f : copies) {
      Results results(f.name);
      for (uint64_t seed = 0; seed < 30; seed++) {
        const uint32_t kind = seed % 5;
        const uint32_t out = kind == 3   ? body + f.field
                             : kind == 4 ? body + f.field + 4
                                         : block + kBodyOut;
        Compare(h, results, {{block + kBodyOut, 64}, {body, 0x500}}, setup(seed, seed % 2),
                [&](PPCContext& t) {
                  t.r3.u64 = out;
                  t.r4.u64 = view;
                },
                f.recompiled, f.native, fmt::format("seed {} out {:X}", seed, out));
      }
      results.Report();
    }
  }
}

// CarOverlays::IsBehindCamera: the car overlays, a camera pass and a point
// in the block, the view (ViewBodyCases' set-up) in a random slot; numbers
// (some making the dot product 0.0, some tiny, to meet flush-to-zero) or
// random words.
void IsBehindCameraCases(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t overlays = block + 0x66000, pass = block + 0x67000, point = block + 0x68000;
  Results results("CarOverlays::IsBehindCamera (sub_82124630)");
  for (uint64_t seed = 0; seed < 400; seed++) {
    const uint32_t kind = seed % 5;
    const uint32_t slot = r.Below(25);
    uint32_t words[9];
    for (uint32_t& w : words) {
      w = kind == 4 ? RandomWord(r) : NumberOrWord(r, false);
    }
    if (kind == 1) {  // the point on the eye or on the car
      for (uint32_t k = 0; k < 3; k++) {
        words[3 + 3 * r.Below(2) + k] = words[k];
      }
    } else if (kind == 2) {  // differences of tiny numbers
      for (uint32_t& w : words) {
        w = (w & 0x807FFFFF) | (r.Below(3) << 23);
      }
    }
    Compare(h, results, {{point, 16}},
            [&h, overlays, pass, point, slot, seed, words, block] {
              ViewBodySetup(h, block, seed, false)();
              Random fill;
              FillRandom(h.base, fill, overlays, 0x400);
              guest::StoreU32(h.base, overlays + 800, pass);
              guest::StoreU32(h.base, overlays + 128, slot);
              guest::StoreU32(h.base, overlays + 4 * (slot + 1), block + kBodyView);
              for (uint32_t k = 0; k < 3; k++) {
                guest::StoreU32(h.base, point + 4 * k, words[k]);
                guest::StoreU32(h.base, pass + 400 + 4 * k, words[3 + k]);
                guest::StoreU32(h.base, block + kBodyBody + 288 + 48 + 4 * k, words[6 + k]);
              }
            },
            [&](PPCContext& t) {
              t.r3.u64 = overlays;
              t.r4.u64 = point;
            },
            Original<__imp__sub_82124630>, NativeIsBehindCamera,
            fmt::format("seed {} kind {} slot {}", seed, kind, slot));
  }
  results.Report();
}
}  // namespace

void RaceOverlays(Context& h) {
  auto* memory = rex::system::kernel_state()->memory();
  const uint32_t block = memory->SystemHeapAlloc(kBlockBytes);
  if (!block) {
    ITC_LOG_ERROR("harness: race_overlays: no block from the system heap");
    return;
  }
  AccessorCases(h, block);
  SetterCases(h, block);
  RandomCases(h);
  MarkerCases(h, block);
  TrailCases(h, block);
  ModelSpriteCases(h, block);
  ParticlesCCases(h, block);
  TrailAddPointCases(h, block);
  ParticlesBCases(h, block);
  ModelPointCases(h, block);
  ModelPointPositionCases(h, block);
  SparksUpdateStreakCases(h, block);
  SparksEmitCases(h, block);
  SmokeCases(h, block);
  SetUpCases(h, block);
  ResetCases(h, block);
  SmokeInRegionCases(h, block);
  SparksUpdateCases(h, block);
  SmokeUpdateCases(h, block);
  ParticlesBUpdateCases(h, block);
  ParticlesCUpdateCases(h, block);
  SlotOfCases(h);
  ViewBodyCases(h, block);
  IsBehindCameraCases(h, block);
  memory->SystemHeapFree(block);
}

}  // namespace harness
