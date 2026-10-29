// XAudio's voices natively: adapters in front of src/xaudio/
// (docs/functions/xaudio.md), with the register steps the harness shares
// (hooks/xaudio.h). Many are reached through the voices' vtables, so these
// adapters stay until their callers are native.

#include "hooks/xaudio.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "xaudio/voices.h"

#include <type_traits>

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
              "sub_82460A58, SetOutputMatrices sub_82460AB8 and SetFormat sub_82460778; "
              "XAudioSourceVoice::Describe sub_8245E010 and Reformat sub_8245DCF0, XAudio::Alloc "
              "sub_8245EF80 (XMemAlloc's plain C++ while --native_heap routes the heap), "
              "XAudioBuffer::ClearThunk sub_8245FA10 and XAudioVoice::BaseSize sub_82461158; the "
              "buffer's constructor sub_8245F750 and Create sub_8245FA80, XAudioVoice::Construct "
              "sub_82461500, XAudio::DefaultAllocator sub_824605E8 and GetAllocator sub_82460688 "
              "(both only while --native_heap routes the heap where they allocate); the voice with "
              "outputs' ConstructOutputs sub_82460738, OutputsSize sub_824607C8, SetOutput "
              "sub_82460998 and SetOutputVoices sub_82460BF0 (with sub_8245F228), Pass "
              "sub_82461070, Destroy sub_82461240, CreateObject sub_82461308 and ProcessEffect "
              "sub_82461678; XAudioSourceVoice::Size sub_8245E0D8 and Init sub_8245E348, "
              "XAudioVoice::CreateEffects sub_82461600, InitBase sub_82461D50 and InitOutputs "
              "sub_82460880, XAudioSubmixVoice::Init sub_8245F230 and Pass sub_8245EFA0, "
              "ProcessEffects sub_82461E88 and MixToOutputs sub_82460CB8; their "
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
    const uint64_t sp = ctx.r1.u64;
    if (regs.sp != 0) {
      ctx.r1.u64 = regs.sp;
    } else if (xa::StackFloor() != 0 && xa::StackFloor() < ctx.r1.u32) {
      ctx.r1.u64 = xa::StackFloor();
    }
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = sp;
    return ctx.r3.u32;
  };
}

// A body with calls, run in a frame of the original's size; r3 its result.
template <typename Body>
void Framed(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, Body body) {
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  const uint32_t floor = xa::StackFloor();
  xa::StackFloor() = 0;
  guest_call::InFrame(ctx, base, frame_bytes, uint32_t(lr), [&](uint32_t frame) {
    if constexpr (std::is_invocable_v<Body, const xa::GuestCall&, uint32_t>) {
      r3 = body(Caller(ctx, base), frame);
    } else {
      r3 = body(Caller(ctx, base));
    }
  });
  xa::StackFloor() = floor;
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

void XAudioSourceDescribe(PPCContext& ctx, uint8_t* base) {
  xa::SourceDescribe(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioReformat(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, format = ctx.r4.u32, outputs = ctx.r5.u32;
  const uint64_t r6 = ctx.r6.u64;
  bool refused = false;
  const PPCContext saved = ctx;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    if (const auto r3 = xa::Reformat(base, voice, format, outputs, r6, frame, call)) {
      return *r3;
    }
    refused = true;
    return 0u;
  });
  if (refused) {
    ctx = saved;
    __imp__sub_8245DCF0(ctx, base);
  }
}

void XAudioAlloc(PPCContext& ctx, uint8_t* base) {
  // XMemAlloc (sub_822976F8)'s plain C++ while the heap is the native
  // layer's, else the original (a tail call to XMemAlloc's adapter).
  if (NativeHeapActive()) {
    ctx.r3.u64 = xa::Alloc(base, ctx.r4.u32, ctx.r5.u32);
    return;
  }
  __imp__sub_8245EF80(ctx, base);
}

void XAudioBufferClearThunk(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = xa::BufferClearThunk(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_8245FA10(ctx, base);
}

void XAudioBaseSize(PPCContext& ctx, uint8_t* base) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  Framed(ctx, base, 176, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::BaseSize(base, description, out, frame, call);
  });
}

void XAudioBufferConstruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t buffer = ctx.r3.u32, format = ctx.r4.u32, allocator = ctx.r5.u32;
  bool refused = false;
  const PPCContext saved = ctx;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call) {
    if (const auto r3 = xa::BufferConstruct(base, buffer, format, allocator, call)) {
      return *r3;
    }
    refused = true;
    return 0u;
  });
  if (refused) {
    ctx = saved;
    __imp__sub_8245F750(ctx, base);
  }
}

void XAudioBufferCreate(PPCContext& ctx, uint8_t* base) {
  const uint32_t format = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call) {
    return xa::BufferCreate(base, format, allocator, out, call);
  });
}

void XAudioVoiceConstruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, allocator = ctx.r4.u32, type = ctx.r5.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call) {
    return xa::VoiceConstruct(base, voice, allocator, type, call);
  });
}

void XAudioDefaultAllocator(PPCContext& ctx, uint8_t* base) {
  // XAudio::Alloc's plain C++ (XMemAlloc's) while the heap is the native
  // layer's, else the original.
  if (!NativeHeapActive()) {
    __imp__sub_824605E8(ctx, base);
    return;
  }
  ctx.r3.u64 = xa::DefaultAllocator(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void XAudioGetAllocator(PPCContext& ctx, uint8_t* base) {
  const uint32_t attributes = ctx.r3.u32, size = ctx.r4.u32, inout = ctx.r5.u32;
  bool refused = false;
  const PPCContext saved = ctx;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    if (const auto r3 = xa::GetAllocator(base, attributes, size, inout, frame,
                                         NativeHeapActive(), call)) {
      return *r3;
    }
    refused = true;
    return 0u;
  });
  if (refused) {
    ctx = saved;
    __imp__sub_82460688(ctx, base);
  }
}

void XAudioConstructOutputs(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, allocator = ctx.r4.u32, type = ctx.r5.u32;
  Framed(ctx, base, 96, [&](const xa::GuestCall& call) {
    return xa::ConstructOutputs(base, voice, allocator, type, call);
  });
}

void XAudioOutputsSize(PPCContext& ctx, uint8_t* base) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::OutputsSize(base, description, out, frame, call);
  });
}

void XAudioSetOutput(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, entry = ctx.r4.u32, slot = ctx.r5.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SetOutput(base, voice, entry, slot, frame, call);
  });
}

void XAudioSetOutputVoices(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, outputs = ctx.r4.u32;
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SetOutputVoices(base, voice, outputs, frame, call);
  });
}

void XAudioPass(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::Pass(base, voice, frame, call);
  });
}

void XAudioDestroy(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, base, 128,
         [&](const xa::GuestCall& call) { return xa::Destroy(base, voice, call); });
}

void XAudioCreateObject(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, entry = ctx.r4.u32, description = ctx.r5.u32,
                 flag = ctx.r6.u32;
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CreateObject(base, voice, entry, description, flag, frame, call);
  });
}

void XAudioProcessEffect(PPCContext& ctx, uint8_t* base) {
  const uint32_t entry = ctx.r4.u32, inout = ctx.r5.u32;
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  Framed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ProcessEffect(base, entry, inout, processor, frame, call);
  });
}

void XAudioSourceSize(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, base, 240, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SourceSize(base, r3, r4, frame, call);
  });
}

void XAudioCreateEffects(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CreateEffects(base, r3, r4, frame, call);
  });
}

void XAudioInitBase(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::InitBase(base, r3, r4, frame, call);
  });
}

void XAudioInitOutputs(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::InitOutputs(base, r3, r4, frame, call);
  });
}

void XAudioSourceInit(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, base, 224, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SourceInit(base, r3, r4, frame, call);
  });
}

void XAudioSubmixInit(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SubmixInit(base, r3, r4, frame, call);
  });
}

void XAudioProcessEffects(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  Framed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ProcessEffects(base, r3, r4, processor, frame, call);
  });
}

void XAudioMixToOutputs(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  Framed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MixToOutputs(base, r3, r4, processor, frame, call);
  });
}

void XAudioSubmixPass(PPCContext& ctx, uint8_t* base) {
  const uint32_t r3 = ctx.r3.u32;
  Framed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SubmixPass(base, r3, frame, call);
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

ITC_XAUDIO_ADAPTER(8245F010, XAudioSubmixDescribe)
ITC_XAUDIO_ADAPTER(8245FA28, XAudioBufferSize)
ITC_XAUDIO_ADAPTER(8245F8E8, XAudioBufferClear)
ITC_XAUDIO_ADAPTER(8245DEB0, XAudioEngineBuffer)
ITC_XAUDIO_ADAPTER(8245E318, XAudioGetType)
ITC_XAUDIO_ADAPTER(8245E328, XAudioGetFormat)
ITC_XAUDIO_ADAPTER(8245F860, XAudioAddRef)
ITC_XAUDIO_ADAPTER(8245FB00, XAudioSetRootVtable)
ITC_XAUDIO_ADAPTER(8245F640, XAudioAddStat)
ITC_XAUDIO_ADAPTER(8245D610, XAudioFactorySize)
ITC_XAUDIO_ADAPTER(8245F6B0, XAudioBufferAddRefThunk)
ITC_XAUDIO_ADAPTER(8245F878, XAudioRelease)
ITC_XAUDIO_ADAPTER(8245AAD8, XAudioObjectFree)
ITC_XAUDIO_ADAPTER(8245DFC0, XAudioApplyVolume)
ITC_XAUDIO_ADAPTER(8245DD60, XAudioStopAndReset)
ITC_XAUDIO_ADAPTER(82461418, XAudioSetEffectParameters)
ITC_XAUDIO_ADAPTER(82460A58, XAudioReleaseOutputs)
ITC_XAUDIO_ADAPTER(82460AB8, XAudioSetOutputMatrices)
ITC_XAUDIO_ADAPTER(8245E010, XAudioSourceDescribe)
ITC_XAUDIO_ADAPTER(8245DCF0, XAudioReformat)
ITC_XAUDIO_ADAPTER(8245FA10, XAudioBufferClearThunk)
ITC_XAUDIO_ADAPTER(82461158, XAudioBaseSize)
ITC_XAUDIO_ADAPTER(8245FA80, XAudioBufferCreate)
ITC_XAUDIO_ADAPTER(82461500, XAudioVoiceConstruct)
ITC_XAUDIO_ADAPTER(82460688, XAudioGetAllocator)
ITC_XAUDIO_ADAPTER(82460738, XAudioConstructOutputs)
ITC_XAUDIO_ADAPTER(824607C8, XAudioOutputsSize)
ITC_XAUDIO_ADAPTER(82460BF0, XAudioSetOutputVoices)
ITC_XAUDIO_ADAPTER(8245F228, XAudioSetOutputVoices)
ITC_XAUDIO_ADAPTER(82461070, XAudioPass)
ITC_XAUDIO_ADAPTER(82461240, XAudioDestroy)
ITC_XAUDIO_ADAPTER(82461308, XAudioCreateObject)
ITC_XAUDIO_ADAPTER(82461678, XAudioProcessEffect)
ITC_XAUDIO_ADAPTER(8245E0D8, XAudioSourceSize)
ITC_XAUDIO_ADAPTER(82461600, XAudioCreateEffects)
ITC_XAUDIO_ADAPTER(82461D50, XAudioInitBase)
ITC_XAUDIO_ADAPTER(82460880, XAudioInitOutputs)
ITC_XAUDIO_ADAPTER(8245E348, XAudioSourceInit)
ITC_XAUDIO_ADAPTER(8245F230, XAudioSubmixInit)
ITC_XAUDIO_ADAPTER(82461E88, XAudioProcessEffects)
ITC_XAUDIO_ADAPTER(82460CB8, XAudioMixToOutputs)
ITC_XAUDIO_ADAPTER(8245EFA0, XAudioSubmixPass)
