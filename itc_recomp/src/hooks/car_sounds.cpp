// The car sounds natively: adapters in front of src/car_sounds/
// (docs/functions/car-sounds.md), with the register steps the harness shares
// (hooks/car_sounds.h).

#include "hooks/car_sounds.h"

#include "base/flags.h"
#include "car_sounds/car_sounds.h"
#include "memory/c_runtime_heap.h"

ITC_FLAG_BOOL(native_car_sounds, true, "ITC",
              "Run the car sounds (docs/functions/car-sounds.md: the wave bank views' Init "
              "sub_8210C9D8, Format sub_8210C850, Length sub_8210C920, Data sub_8210C958 and "
              "Entry sub_8210C998 (XMemCpy's plain C++); CarSounds::SetBanks sub_8210CB48; "
              "TireSound::Amount sub_8210CA28; CarSound::CarSound sub_8210CC18 with "
              "EngineSound::EngineSound sub_8210CBA0; CarSounds::Destroy sub_8210DCA8 (operator "
              "delete through its plain C++)) natively instead of through the translated code "
              "(the same reads and writes; XAudio's voice methods stay the game's; checked by "
              "--run_harnesses=car_sounds)");

namespace hooks {

namespace cs = car_sounds;

void CarSoundsWaveBankViewInit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cs::WaveBankViewInit(base, ctx.r3.u32, ctx.r4.u32);
}

void CarSoundsWaveBankFormat(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cs::WaveBankFormat(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void CarSoundsWaveBankLength(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cs::WaveBankLength(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void CarSoundsWaveBankData(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cs::WaveBankData(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void CarSoundsWaveBankEntry(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = cs::WaveBankEntry(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_8210C998(ctx, base);
}

void CarSoundsSetBanks(PPCContext& ctx, uint8_t* base) {
  cs::SetBanks(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void CarSoundsTireSoundAmount(PPCContext& ctx, uint8_t* base) {
  cs::TireSoundAmount(base, ctx.r3.u32, ctx.f1.f64);
}

void CarSoundsEngineSoundConstruct(PPCContext& ctx, uint8_t* base) {
  cs::EngineSoundConstruct(base, ctx.r3.u32);
}

void CarSoundsCarSoundConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cs::CarSoundConstruct(base, ctx.r3.u32);
}

void CarSoundsDestroy(PPCContext& ctx, uint8_t* base) {
  const cs::Destroyed done = cs::CarSoundsDestroy(base, ctx.r3.u32, ctx.r4.u32);
  if (done.to_free != 0) {
    const bool freed = done.array ? memory::crt::OperatorDeleteArray(done.to_free)
                                  : memory::crt::OperatorDelete(done.to_free);
    if (!freed) {
      ctx.r3.u64 = done.to_free;
      (done.array ? sub_825A71C0 : sub_825A70E8)(ctx, base);
    }
  }
  ctx.r3.u64 = done.r3;
}

}  // namespace hooks

#define ITC_CAR_SOUNDS_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_car_sounds)) {       \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
  }

ITC_CAR_SOUNDS_ADAPTER(8210C9D8, CarSoundsWaveBankViewInit)
ITC_CAR_SOUNDS_ADAPTER(8210C850, CarSoundsWaveBankFormat)
ITC_CAR_SOUNDS_ADAPTER(8210C920, CarSoundsWaveBankLength)
ITC_CAR_SOUNDS_ADAPTER(8210C958, CarSoundsWaveBankData)
ITC_CAR_SOUNDS_ADAPTER(8210C998, CarSoundsWaveBankEntry)
ITC_CAR_SOUNDS_ADAPTER(8210CB48, CarSoundsSetBanks)
ITC_CAR_SOUNDS_ADAPTER(8210CA28, CarSoundsTireSoundAmount)
ITC_CAR_SOUNDS_ADAPTER(8210CBA0, CarSoundsEngineSoundConstruct)
ITC_CAR_SOUNDS_ADAPTER(8210CC18, CarSoundsCarSoundConstruct)
ITC_CAR_SOUNDS_ADAPTER(8210DCA8, CarSoundsDestroy)
