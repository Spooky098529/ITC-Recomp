// XAudio's voices natively: adapters in front of src/xaudio/
// (docs/functions/xaudio.md), with the register steps the harness shares
// (hooks/xaudio.h). Many are reached through the voices' vtables, so these
// adapters stay until their callers are native.

#include "hooks/xaudio.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "xaudio/voices.h"

ITC_FLAG_BOOL(native_xaudio, true, "ITC",
              "Run the parts of XAudio's voices made native (docs/functions/xaudio.md: "
              "XAudio::SourceOutputFormat sub_8245DC88, XAudioSubmixVoice::Describe "
              "sub_8245F010, XAudio::DefaultMatrix sub_8245EC40, XAudioBuffer::Size "
              "sub_8245FA28 and Clear sub_8245F8E8, XAudioVoice::EngineBuffer sub_8245DEB0, "
              "GetType sub_8245E318, GetFormat sub_8245E328, SetFormatWords sub_82461140, "
              "AddRef sub_8245F860, sub_8245FB00 and XAudio::AddStat sub_8245F640; the interface "
              "thunks sub_8245D610, sub_8245D630, sub_8245F6B0 and sub_8245F6D0, Release "
              "sub_8245F878, XAudioObject::Free sub_8245AAD8, ApplyVolume sub_8245DFC0, "
              "StopAndReset sub_8245DD60, SetEffectParameters sub_82461418, ReleaseOutputs "
              "sub_82460A58, SetOutputMatrices sub_82460AB8 and SetFormat sub_82460778, their "
              "calls into the recompiled objects through the translated code) natively "
              "instead of through the translated code (the same reads and writes; XAudio's two "
              "locks stay the game's; checked by --run_harnesses=xaudio)");

namespace hooks {

namespace xa = xaudio;

void XAudioSourceOutputFormat(PPCContext& ctx, uint8_t* base) {
  xa::SourceOutputFormat(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioSubmixDescribe(PPCContext& ctx, uint8_t* base) {
  xa::SubmixDescribe(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioDefaultMatrix(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::DefaultMatrix(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void XAudioBufferSize(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = xa::BufferSize(base, ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_8245FA28(ctx, base);
}

void XAudioBufferClear(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = xa::BufferClear(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_8245F8E8(ctx, base);
}

void XAudioEngineBuffer(PPCContext& ctx, uint8_t* base) {
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  ctx.r3.u64 = xa::EngineBuffer(base, processor);
}

void XAudioGetType(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::GetType(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioGetFormat(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::GetFormat(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioSetFormatWords(PPCContext& ctx, uint8_t* base) {
  xa::SetFormatWords(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioAddRef(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::AddRef(base, ctx.r3.u32);
}

void XAudioSetRootVtable(PPCContext& ctx, uint8_t* base) {
  xa::SetRootVtable(base, ctx.r3.u32);
}

void XAudioAddStat(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::AddStat(base, ctx.r3.u32, ctx.r4.u32);
}

namespace {

xa::GuestCall Caller(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t function, uint32_t return_address, const xa::Regs& regs) {
    PPCRegister* const r[5] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7};
    for (uint32_t i = 0; i < regs.count; i++) {
      r[i]->u64 = regs.r[i];
    }
    if (regs.f1_set) {
      ctx.f1.f64 = regs.f1;
    }
    if (return_address != 0) {
      ctx.lr = return_address;
    }
    REX_CALL_INDIRECT_FUNC(function);
    return ctx.r3.u32;
  };
}

// A body with calls, run in a frame of the original's size; r3 its result.
template <typename Body>
void Framed(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, Body body) {
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, frame_bytes, uint32_t(lr),
                      [&](uint32_t) { r3 = body(Caller(ctx, base)); });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

using ThunkFn = uint32_t (*)(uint8_t*, uint32_t, uint32_t*);

void Thunk(PPCContext& ctx, uint8_t* base, ThunkFn thunk) {
  uint32_t function = 0;
  ctx.r3.u64 = thunk(base, ctx.r3.u32, &function);
  REX_CALL_INDIRECT_FUNC(function);
}

}  // namespace

void XAudioFactorySize(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, xa::FactorySizeThunk); }
void XAudioFactoryCreate(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, xa::FactoryCreateThunk); }
void XAudioBufferAddRefThunk(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, xa::BufferAddRefThunk); }
void XAudioBufferReleaseThunk(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, xa::BufferReleaseThunk); }

void XAudioRelease(PPCContext& ctx, uint8_t* base) {
  const uint32_t object = ctx.r3.u32;
  Framed(ctx, base, 96, [&](const xa::GuestCall& call) { return xa::Release(base, object, call); });
}

void XAudioObjectFree(PPCContext& ctx, uint8_t* base) {
  const uint32_t object = ctx.r3.u32;
  Framed(ctx, base, 112,
         [&](const xa::GuestCall& call) { return xa::ObjectFree(base, object, call); });
}

void XAudioApplyVolume(PPCContext& ctx, uint8_t* base) {
  // A tail call: the codec's method runs at the caller's frame.
  ctx.fpscr.disableFlushMode();
  const uint32_t voice = ctx.r3.u32;
  ctx.r3.u64 = xa::ApplyVolume(base, voice, Caller(ctx, base));
}

void XAudioStopAndReset(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, base, 96,
         [&](const xa::GuestCall& call) { return xa::StopAndReset(base, voice, call); });
}

void XAudioSetEffectParameters(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  const uint32_t index = ctx.r4.u32;
  const uint64_t r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, base, 96, [&](const xa::GuestCall& call) {
    return xa::SetEffectParameters(base, voice, index, r5, r6, r7, call);
  });
}

void XAudioReleaseOutputs(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, base, 128,
         [&](const xa::GuestCall& call) { return xa::ReleaseOutputs(base, voice, call); });
}

void XAudioSetOutputMatrices(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  const uint32_t matrices = ctx.r4.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call) {
    return xa::SetOutputMatrices(base, voice, matrices, call);
  });
}

void XAudioSetFormat(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  const uint32_t format = ctx.r4.u32;
  const uint32_t outputs = ctx.r5.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call) {
    return xa::SetFormat(base, voice, format, outputs, call);
  });
}

}  // namespace hooks

#define ITC_XAUDIO_ADAPTER(address, step)   \
  extern "C" REX_FUNC(sub_##address) {      \
    if (!ITC_FLAG(native_xaudio)) {         \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

ITC_XAUDIO_ADAPTER(8245DC88, XAudioSourceOutputFormat)
ITC_XAUDIO_ADAPTER(8245F010, XAudioSubmixDescribe)
ITC_XAUDIO_ADAPTER(8245EC40, XAudioDefaultMatrix)
ITC_XAUDIO_ADAPTER(8245FA28, XAudioBufferSize)
ITC_XAUDIO_ADAPTER(8245F8E8, XAudioBufferClear)
ITC_XAUDIO_ADAPTER(8245DEB0, XAudioEngineBuffer)
ITC_XAUDIO_ADAPTER(8245E318, XAudioGetType)
ITC_XAUDIO_ADAPTER(8245E328, XAudioGetFormat)
ITC_XAUDIO_ADAPTER(82461140, XAudioSetFormatWords)
ITC_XAUDIO_ADAPTER(8245F860, XAudioAddRef)
ITC_XAUDIO_ADAPTER(8245FB00, XAudioSetRootVtable)
ITC_XAUDIO_ADAPTER(8245F640, XAudioAddStat)
ITC_XAUDIO_ADAPTER(8245D610, XAudioFactorySize)
ITC_XAUDIO_ADAPTER(8245D630, XAudioFactoryCreate)
ITC_XAUDIO_ADAPTER(8245F6B0, XAudioBufferAddRefThunk)
ITC_XAUDIO_ADAPTER(8245F6D0, XAudioBufferReleaseThunk)
ITC_XAUDIO_ADAPTER(8245F878, XAudioRelease)
ITC_XAUDIO_ADAPTER(8245AAD8, XAudioObjectFree)
ITC_XAUDIO_ADAPTER(8245DFC0, XAudioApplyVolume)
ITC_XAUDIO_ADAPTER(8245DD60, XAudioStopAndReset)
ITC_XAUDIO_ADAPTER(82461418, XAudioSetEffectParameters)
ITC_XAUDIO_ADAPTER(82460A58, XAudioReleaseOutputs)
ITC_XAUDIO_ADAPTER(82460AB8, XAudioSetOutputMatrices)
ITC_XAUDIO_ADAPTER(82460778, XAudioSetFormat)
