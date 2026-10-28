// The car sounds natively: adapters in front of src/car_sounds/
// (docs/functions/car-sounds.md), with the register steps the harness shares
// (hooks/car_sounds.h).

#include "hooks/car_sounds.h"

#include "base/flags.h"
#include "car_sounds/car_sounds.h"
#include "car_sounds/x3daudio.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/mode_tick.h"
#include "hooks/models.h"
#include "memory/c_runtime_heap.h"

#include <cstring>

ITC_FLAG_DECLARE(bool, native_shader_objects);

ITC_FLAG_BOOL(native_car_sounds, true, "ITC",
              "Run the car sounds (docs/functions/car-sounds.md: the wave bank views' Init "
              "sub_8210C9D8, Format sub_8210C850, Length sub_8210C920, Data sub_8210C958 and "
              "Entry sub_8210C998 (XMemCpy's plain C++); CarSounds::SetBanks sub_8210CB48; "
              "TireSound::Amount sub_8210CA28; CarSound::CarSound sub_8210CC18 with "
              "EngineSound::EngineSound sub_8210CBA0 (no adapter: only its plain C++ calls it); CarSounds::Destroy sub_8210DCA8 (operator "
              "delete through its plain C++); CarSound::SetUp sub_8210DDC8 (operator new[] through "
              "its plain C++); SoundGlobals::Init sub_82110258 (with the memory blocks native: "
              "Memory::AllocPhysicalEx's, Music::Play's and X3DAudioInitialize's plain C++); X3DAudio's "
              "X3DVec::Sub sub_824819E0, Length sub_82481998, Dot sub_824819B8 and Cross "
              "sub_82481A18, X3DAudio::EvaluateCurve sub_82480818 and X3DAudio::Curves "
              "sub_82480918, X3DAudio::Pan sub_82480F80 and X3DAudio::Matrix sub_82481390, X3DAudio::Cone sub_82480BA8, X3DAudio::Doppler sub_82480E90 and X3DAudioCalculate sub_82481880 (the original when its flags ask for EmitterAngle sub_82480780 or Delay sub_82481708) (checked by --run_harnesses=x3daudio)); CarSound::Update3D sub_8210F820 (RigidBody::Position's, Vec4::Copy's and X3DAudioCalculate's plain C++) natively instead of through the translated code "
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

void CarSoundsSetUp(PPCContext& ctx, uint8_t* base) {
  const uint32_t sound = ctx.r3.u32;
  uint32_t matrix;
  if (const auto made = memory::crt::OperatorNewArray(base, 24)) {
    matrix = *made;
  } else {
    ctx.r3.u64 = 24;
    sub_825A6940(ctx, base);
    matrix = ctx.r3.u32;
  }
  ctx.r3.u64 = cs::SetUp(base, sound, matrix);
}

bool CarSoundsGlobalsNative() {
  // The memory blocks' native path, as Memory::AllocPhysicalEx's adapter
  // (src/hooks/shader_objects.cpp) takes it.
  return ITC_FLAG(native_shader_objects) && NativeHeapActive() && ModelsHostLock();
}

void CarSoundsGlobalsInit(PPCContext& ctx, uint8_t* base) {
  cs::SoundGlobalsInit(base, ctx.r3.u32);
  // Music::Play(r3 = a word 0 at +84, r4 = the name, r5 = a byte 0 at +80).
  guest_call::InFrame(ctx, base, 112, 0x82110358, [&](uint32_t frame) {
    guest::StoreU8(base, frame + 80, 0);
    guest::StoreU32(base, frame + 84, 0);
    ctx.r3.u64 = frame + 84;
    ctx.r4.u64 = cs::kMusicName;
    ctx.r5.u64 = frame + 80;
    ModeTickMusicPlay(ctx, base);
  });
  ctx.r3.u64 = cs::SoundGlobalsInit3D(base);
}

void CarSoundsX3DVecSub(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  x3daudio::VecSub(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void CarSoundsX3DVecLength(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(x3daudio::VecLength(base, ctx.r3.u32));
}

void CarSoundsX3DVecDot(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(x3daudio::VecDot(base, ctx.r3.u32, ctx.r4.u32));
}

void CarSoundsX3DVecCross(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  x3daudio::VecCross(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void CarSoundsX3DEvaluateCurve(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(x3daudio::EvaluateCurve(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64));
}

void CarSoundsX3DCurves(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  // The original's prologue: lr at r1 - 8, r31 at r1 - 16, the back chain.
  const uint32_t frame = ctx.r1.u32 - 128;
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(ctx.lr));
  REX_STORE_U64(ctx.r1.u32 - 16, ctx.r31.u64);
  guest::StoreU32(base, frame, ctx.r1.u32);
  if (const uint32_t r3 = x3daudio::Curves(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, frame)) {
    ctx.r3.u64 = r3;
  }
  // The epilogue's r12 and lr: the saved word.
  ctx.r12.u64 = uint32_t(ctx.lr);
  ctx.lr = ctx.r12.u64;
}

namespace {

// tan through the translated code (its adapter, then its original where that
// can't give it), from Pan's frame and return address: r1 and lr put back
// after.
struct GuestTanUser {
  PPCContext* ctx;
  uint8_t* base;
};

x3daudio::TanResult GuestTan(void* user, uint32_t sp, double x) {
  const GuestTanUser& u = *static_cast<GuestTanUser*>(user);
  PPCContext& ctx = *u.ctx;
  const uint64_t stack = ctx.r1.u64;
  const uint64_t lr = ctx.lr;
  ctx.r1.u64 = sp;
  ctx.lr = 0x8248133C;
  ctx.f1.f64 = x;
  sub_825A8A50(ctx, u.base);
  ctx.r1.u64 = stack;
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  return {ctx.f1.f64, ctx.r3.u64};
}

}  // namespace

void CarSoundsX3DPan(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestTanUser user{&ctx, base};
  const x3daudio::PanSaves saves{uint32_t(ctx.lr), ctx.r28.u64, ctx.r29.u64, ctx.r30.u64,
                                 ctx.r31.u64, ctx.f31.u64};
  ctx.r3.u64 = x3daudio::Pan(base, ctx.r3.u64, ctx.r5.u32, ctx.r6.u32, ctx.f1.f64, ctx.r1.u32,
                             saves, {GuestTan, &user});
  // The epilogue's r12 and lr: the saved word.
  ctx.r12.u64 = uint32_t(ctx.lr);
  ctx.lr = ctx.r12.u64;
}

void CarSoundsX3DMatrix(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  // The original's prologue: __savegprlr_26 (r26 to r31 at r1 - 56 to - 16,
  // lr at - 8), __savefpr_23 (f23 to f31 at r1 - 128 to - 64), the back
  // chain.
  const uint32_t sp = ctx.r1.u32;
  const uint64_t gprs[6] = {ctx.r26.u64, ctx.r27.u64, ctx.r28.u64,
                            ctx.r29.u64, ctx.r30.u64, ctx.r31.u64};
  for (uint32_t i = 0; i < 6; i++) {
    REX_STORE_U64(sp - 56 + 8 * i, gprs[i]);
  }
  guest::StoreU32(base, sp - 8, uint32_t(ctx.lr));
  const uint64_t fprs[9] = {ctx.f23.u64, ctx.f24.u64, ctx.f25.u64, ctx.f26.u64, ctx.f27.u64,
                            ctx.f28.u64, ctx.f29.u64, ctx.f30.u64, ctx.f31.u64};
  for (uint32_t i = 0; i < 9; i++) {
    REX_STORE_U64(sp - 128 + 8 * i, fprs[i]);
  }
  const uint32_t frame = sp - 304;
  guest::StoreU32(base, frame, sp);
  GuestTanUser user{&ctx, base};
  ctx.r3.u64 = x3daudio::Matrix(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u64, ctx.r6.u64,
                                ctx.f31.u64, frame, {GuestTan, &user});
  // The epilogue's r12 and lr: the saved word.
  ctx.r12.u64 = uint32_t(ctx.lr);
  ctx.lr = ctx.r12.u64;
}

void CarSoundsX3DCone(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const x3daudio::ConeSaves saves{uint32_t(ctx.lr), ctx.r29.u64, ctx.r30.u64, ctx.r31.u64,
                                  ctx.f29.u64, ctx.f30.u64, ctx.f31.u64};
  ctx.r3.u64 = x3daudio::Cone(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                              ctx.r1.u32, saves);
  // The epilogue's r12 and lr: the saved word.
  ctx.r12.u64 = uint32_t(ctx.lr);
  ctx.lr = ctx.r12.u64;
}

void CarSoundsX3DDoppler(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const x3daudio::DopplerSaves saves{uint32_t(ctx.lr), ctx.r28.u64, ctx.r29.u64, ctx.r30.u64,
                                     ctx.r31.u64, ctx.f31.u64};
  ctx.r3.u64 = x3daudio::Doppler(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                 ctx.r1.u32, saves);
  ctx.r12.u64 = uint32_t(ctx.lr);
  ctx.lr = ctx.r12.u64;
}

void CarSoundsX3DCalculate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestTanUser user{&ctx, base};
  const x3daudio::CalculateArgs args{ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64};
  const x3daudio::CalculateSaves saves{
      uint32_t(ctx.lr), ctx.r26.u64, ctx.r27.u64, ctx.r28.u64, ctx.r29.u64, ctx.r30.u64,
      ctx.r31.u64,      ctx.f23.u64, ctx.f24.u64, ctx.f25.u64, ctx.f26.u64, ctx.f27.u64,
      ctx.f28.u64,      ctx.f29.u64, ctx.f30.u64, ctx.f31.u64};
  const auto r3 = x3daudio::Calculate(base, args, ctx.r1.u32, saves, {GuestTan, &user});
  if (!r3) {
    // EmitterAngle or Delay asked for: the original, its callees through
    // their adapters.
    __imp__sub_82481880(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
  ctx.r12.u64 = uint32_t(ctx.lr);
  ctx.lr = ctx.r12.u64;
}

void CarSoundsUpdate3D(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestTanUser user{&ctx, base};
  cs::Update3DSaves saves{uint32_t(ctx.lr), ctx.r22.u64, ctx.r23.u64, ctx.r24.u64, ctx.r25.u64,
                          ctx.r26.u64,      ctx.r27.u64, ctx.r28.u64, ctx.r29.u64, ctx.r30.u64,
                          ctx.r31.u64,      ctx.f23.u64, ctx.f24.u64, ctx.f25.u64, ctx.f26.u64,
                          ctx.f27.u64,      ctx.f28.u64, ctx.f29.u64, ctx.f30.u64, ctx.f31.u64,
                          {},               {}};
  std::memcpy(saves.v126, ctx.v126.u8, 16);
  std::memcpy(saves.v127, ctx.v127.u8, 16);
  ctx.r3.u64 = cs::Update3D(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32, saves,
                            {GuestTan, &user});
  // The epilogue's r12 and lr: the saved word.
  ctx.r12.u64 = uint32_t(ctx.lr);
  ctx.lr = ctx.r12.u64;
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
// No adapter (removed 2026-10-28: only `CarSound::CarSound`'s plain C++ calls it in play)
// for EngineSound::EngineSound sub_8210CBA0.
ITC_CAR_SOUNDS_ADAPTER(8210CC18, CarSoundsCarSoundConstruct)
ITC_CAR_SOUNDS_ADAPTER(8210DCA8, CarSoundsDestroy)
ITC_CAR_SOUNDS_ADAPTER(8210DDC8, CarSoundsSetUp)
ITC_CAR_SOUNDS_ADAPTER(824819E0, CarSoundsX3DVecSub)
ITC_CAR_SOUNDS_ADAPTER(82481998, CarSoundsX3DVecLength)
ITC_CAR_SOUNDS_ADAPTER(824819B8, CarSoundsX3DVecDot)
ITC_CAR_SOUNDS_ADAPTER(82481A18, CarSoundsX3DVecCross)
ITC_CAR_SOUNDS_ADAPTER(82480818, CarSoundsX3DEvaluateCurve)
ITC_CAR_SOUNDS_ADAPTER(82480918, CarSoundsX3DCurves)
ITC_CAR_SOUNDS_ADAPTER(82480F80, CarSoundsX3DPan)
ITC_CAR_SOUNDS_ADAPTER(82481390, CarSoundsX3DMatrix)
ITC_CAR_SOUNDS_ADAPTER(82480BA8, CarSoundsX3DCone)
ITC_CAR_SOUNDS_ADAPTER(82480E90, CarSoundsX3DDoppler)
ITC_CAR_SOUNDS_ADAPTER(82481880, CarSoundsX3DCalculate)
ITC_CAR_SOUNDS_ADAPTER(8210F820, CarSoundsUpdate3D)

extern "C" REX_FUNC(sub_82110258) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_car_sounds) || !hooks::CarSoundsGlobalsNative()) {
    __imp__sub_82110258(ctx, base);
    return;
  }
  hooks::CarSoundsGlobalsInit(ctx, base);
}
