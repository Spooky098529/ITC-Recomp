// XAudio's voices natively: adapters in front of src/xaudio/
// (docs/functions/xaudio.md), with the register steps the harness shares
// (hooks/xaudio.h). Many are reached through the voices' vtables, so these
// adapters stay until their callers are native.

#include "hooks/xaudio.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "xaudio/engine.h"
#include "xaudio/voices.h"

#include <type_traits>
#include "lifted/sdk.h"

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
              "ProcessEffects sub_82461E88 and MixToOutputs sub_82460CB8; under the voice lists' "
              "spin lock (taken as the original does), XAudioSourceVoice::GetState sub_8245DDC8, "
              "StartLater sub_8245E7D0 and CancelPendingStart sub_8245E428, XAudioVoice::Play "
              "sub_82461798, Activate sub_82460EF0, Deactivate sub_82460D50 and StopOrTail "
              "sub_824619B0, XAudio::WaitForVoice sub_8245AE60; XAudioSourceVoice::Start "
              "sub_8245EBA0, Stop sub_8245E9F0 and Pass sub_8245DED0, the voices' constructors "
              "sub_8245E6A8 and sub_8245F298, destructors sub_82460B50, sub_8245E1B8, "
              "sub_8245E7A0, sub_8245F108 and sub_8245F3A0, XAudioVoice::CreateSource "
              "sub_8245EAA0 and CreateSubmix sub_8245F3D0; the API entry points sub_82459DB0 to "
              "sub_8245A7C8 but XAudioShutDown, with the callback's registration sub_8245A5E8, the commit sub_8245A6E8 and XAudioInitialize sub_8245A760 with sub_8245A410 (no adapter of its own) (under the API lock, the game's "
              "critical section taken as the original does); their "
              "calls into the recompiled objects through the translated code) natively "
              "instead of through the translated code (the same reads and writes; XAudio's two "
              "locks are the game's, taken as the original does; checked by --run_harnesses=xaudio)");

namespace hooks {

namespace xa = xaudio;

ITC_REGS_STEP(XAudioSourceOutputFormat) {
  xa::SourceOutputFormat(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioSubmixDescribe) {
  xa::SubmixDescribe(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioDefaultMatrix) {
  ctx.r3.u64 = xa::DefaultMatrix(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(XAudioBufferSize) {
  if (const auto r3 = xa::BufferSize(ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  lifted::original::c8245FA28(ctx);
}

ITC_REGS_STEP(XAudioBufferClear) {
  if (const auto r3 = xa::BufferClear(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    ctx.r3.s64 = int32_t(*r3);  // the errors as the original's lis/ori leave them
    return;
  }
  lifted::original::c8245F8E8(ctx);
}

ITC_REGS_STEP(XAudioEngineBuffer) {
  const uint32_t processor = guest::LoadU8(ctx.r13.u32 + xa::kPcrProcessor);
  ctx.r3.u64 = xa::EngineBuffer(processor);
}

ITC_REGS_STEP(XAudioGetType) {
  ctx.r3.u64 = xa::GetType(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioGetFormat) {
  ctx.r3.u64 = xa::GetFormat(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioSetFormatWords) {
  xa::SetFormatWords(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioAddRef) {
  ctx.r3.u64 = xa::AddRef(ctx.r3.u32);
}

ITC_REGS_STEP(XAudioSetRootVtable) {
  xa::SetRootVtable(ctx.r3.u32);
}

ITC_REGS_STEP(XAudioAddStat) {
  ctx.r3.u64 = xa::AddStat(ctx.r3.u32, ctx.r4.u32);
}

namespace {

xa::GuestCall Caller(lifted::Regs& ctx) {
  return [&ctx](uint32_t function, uint32_t return_address, const xa::Regs& regs) {
    lifted::Temp* const r[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                               &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    for (uint32_t i = 0; i < regs.count; i++) {
      r[i]->u64 = regs.r[i];
    }
    if (regs.f1_set) {
      ctx.f1.f64 = regs.f1;
    }
    if (return_address != 0) {
      ctx.lr = return_address;
    }
    if (function == 0) {
      // Only the registers, as the original sets them before calling a
      // function whose plain C++ the caller runs itself.
      return ctx.r3.u32;
    }
    const uint64_t sp = ctx.r1.u64;
    if (regs.sp != 0) {
      ctx.r1.u64 = regs.sp;
    } else if (xa::StackFloor() != 0 && xa::StackFloor() < ctx.r1.u32) {
      ctx.r1.u64 = xa::StackFloor();
    }
    switch (function) {
      // The kernel imports, at their thunks' guest addresses.
      case xa::kKeRaiseIrqlToDpcLevel:
        lifted::Sdk(ctx, __imp__KeRaiseIrqlToDpcLevel);
        break;
      case xa::kKeAcquireSpinLockAtRaisedIrql:
        lifted::Sdk(ctx, __imp__KeAcquireSpinLockAtRaisedIrql);
        break;
      case xa::kKeReleaseSpinLockFromRaisedIrql:
        lifted::Sdk(ctx, __imp__KeReleaseSpinLockFromRaisedIrql);
        break;
      case xa::kKfLowerIrql:
        lifted::Sdk(ctx, __imp__KfLowerIrql);
        break;
      case xa::kRtlEnterCriticalSection:
        lifted::Sdk(ctx, __imp__RtlEnterCriticalSection);
        break;
      case xa::kRtlLeaveCriticalSection:
        lifted::Sdk(ctx, __imp__RtlLeaveCriticalSection);
        break;
      default:
        lifted::CallFromContext(ctx, function);
        break;
    }
    ctx.r1.u64 = sp;
    xa::LastCallR3() = ctx.r3.u64;
    return ctx.r3.u32;
  };
}

// A body with calls, run in a frame of the original's size; r3 its result.
template <typename Body>
void Framed(lifted::Regs& ctx, uint32_t frame_bytes, Body body) {
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  const uint32_t floor = xa::StackFloor();
  xa::StackFloor() = 0;
  guest_call::InFrame(ctx, frame_bytes, uint32_t(lr), [&](uint32_t frame) {
    if constexpr (std::is_invocable_v<Body, const xa::GuestCall&, uint32_t>) {
      r3 = body(Caller(ctx), frame);
    } else {
      r3 = body(Caller(ctx));
    }
  });
  xa::StackFloor() = floor;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

using ThunkFn = uint32_t (*)(uint8_t*, uint32_t, uint32_t*);

void Thunk(lifted::Regs& ctx, ThunkFn thunk) {
  uint32_t function = 0;
  ctx.r3.u64 = thunk(ctx.r3.u32, &function);
  lifted::CallFromContext(ctx, function);
}

}  // namespace

ITC_REGS_STEP(XAudioFactorySize) { Thunk(ctx, xa::FactorySizeThunk); }
ITC_REGS_STEP(XAudioFactoryCreate) { Thunk(ctx, xa::FactoryCreateThunk); }
ITC_REGS_STEP(XAudioBufferAddRefThunk) { Thunk(ctx, xa::BufferAddRefThunk); }
ITC_REGS_STEP(XAudioBufferReleaseThunk) { Thunk(ctx, xa::BufferReleaseThunk); }

ITC_REGS_STEP(XAudioRelease) {
  const uint32_t object = ctx.r3.u32;
  Framed(ctx, 96, [&](const xa::GuestCall& call) { return xa::Release(object, call); });
}

ITC_REGS_STEP(XAudioObjectFree) {
  const uint32_t object = ctx.r3.u32;
  Framed(ctx, 112,
         [&](const xa::GuestCall& call) { return xa::ObjectFree(object, call); });
}

ITC_REGS_STEP(XAudioApplyVolume) {
  // A tail call: the codec's method runs at the caller's frame.
  ctx.fpscr.disableFlushMode();
  const uint32_t voice = ctx.r3.u32;
  ctx.r3.u64 = xa::ApplyVolume(voice, Caller(ctx));
}

ITC_REGS_STEP(XAudioStopAndReset) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, 96,
         [&](const xa::GuestCall& call) { return xa::StopAndReset(voice, call); });
}

ITC_REGS_STEP(XAudioSetEffectParameters) {
  const uint32_t voice = ctx.r3.u32;
  const uint32_t index = ctx.r4.u32;
  const uint64_t r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 96, [&](const xa::GuestCall& call) {
    return xa::SetEffectParameters(voice, index, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioReleaseOutputs) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, 128,
         [&](const xa::GuestCall& call) { return xa::ReleaseOutputs(voice, call); });
}

ITC_REGS_STEP(XAudioSetOutputMatrices) {
  const uint32_t voice = ctx.r3.u32;
  const uint32_t matrices = ctx.r4.u32;
  Framed(ctx, 128, [&](const xa::GuestCall& call) {
    return xa::SetOutputMatrices(voice, matrices, call);
  });
}

ITC_REGS_STEP(XAudioSetFormat) {
  const uint32_t voice = ctx.r3.u32;
  const uint32_t format = ctx.r4.u32;
  const uint32_t outputs = ctx.r5.u32;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::SetFormat(voice, format, outputs, call);
  });
}

ITC_REGS_STEP(XAudioSourceDescribe) {
  xa::SourceDescribe(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioReformat) {
  const uint32_t voice = ctx.r3.u32, format = ctx.r4.u32, outputs = ctx.r5.u32;
  const uint64_t r6 = ctx.r6.u64;
  bool refused = false;
  const lifted::Regs saved = ctx;
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    if (const auto r3 = xa::Reformat(voice, format, outputs, r6, frame, call)) {
      return *r3;
    }
    refused = true;
    return 0u;
  });
  if (refused) {
    ctx = saved;
    lifted::original::c8245DCF0(ctx);
  }
}

ITC_REGS_STEP(XAudioAlloc) {
  // XMemAlloc (sub_822976F8)'s plain C++ while the heap is the native
  // layer's, else the original (a tail call to XMemAlloc's adapter).
  if (NativeHeapActive()) {
    ctx.r3.u64 = xa::Alloc(ctx.r4.u32, ctx.r5.u32);
    return;
  }
  lifted::original::c8245EF80(ctx);
}

ITC_REGS_STEP(XAudioBufferClearThunk) {
  if (const auto r3 = xa::BufferClearThunk(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    ctx.r3.s64 = int32_t(*r3);  // the errors as the original's lis/ori leave them
    return;
  }
  lifted::original::c8245FA10(ctx);
}

ITC_REGS_STEP(XAudioBaseSize) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  Framed(ctx, 176, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::BaseSize(description, out, frame, call);
  });
}

ITC_REGS_STEP(XAudioBufferConstruct) {
  const uint32_t buffer = ctx.r3.u32, format = ctx.r4.u32, allocator = ctx.r5.u32;
  bool refused = false;
  const lifted::Regs saved = ctx;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    if (const auto r3 = xa::BufferConstruct(buffer, format, allocator, call)) {
      return *r3;
    }
    refused = true;
    return 0u;
  });
  if (refused) {
    ctx = saved;
    lifted::original::c8245F750(ctx);
  }
}

ITC_REGS_STEP(XAudioBufferCreate) {
  const uint32_t format = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::BufferCreate(format, allocator, out, call);
  });
}

ITC_REGS_STEP(XAudioVoiceConstruct) {
  const uint32_t voice = ctx.r3.u32, allocator = ctx.r4.u32, type = ctx.r5.u32;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::VoiceConstruct(voice, allocator, type, call);
  });
}

ITC_REGS_STEP(XAudioDefaultAllocator) {
  // XAudio::Alloc's plain C++ (XMemAlloc's) while the heap is the native
  // layer's, else the original.
  if (!NativeHeapActive()) {
    lifted::original::c824605E8(ctx);
    return;
  }
  ctx.r3.u64 = xa::DefaultAllocator(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(XAudioGetAllocator) {
  const uint32_t attributes = ctx.r3.u32, size = ctx.r4.u32, inout = ctx.r5.u32;
  bool refused = false;
  const lifted::Regs saved = ctx;
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    if (const auto r3 = xa::GetAllocator(attributes, size, inout, frame,
                                         NativeHeapActive(), call)) {
      return *r3;
    }
    refused = true;
    return 0u;
  });
  if (refused) {
    ctx = saved;
    lifted::original::c82460688(ctx);
  }
}

ITC_REGS_STEP(XAudioConstructOutputs) {
  const uint32_t voice = ctx.r3.u32, allocator = ctx.r4.u32, type = ctx.r5.u32;
  Framed(ctx, 96, [&](const xa::GuestCall& call) {
    return xa::ConstructOutputs(voice, allocator, type, call);
  });
}

ITC_REGS_STEP(XAudioOutputsSize) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  Framed(ctx, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::OutputsSize(description, out, frame, call);
  });
}

ITC_REGS_STEP(XAudioSetOutput) {
  const uint32_t voice = ctx.r3.u32, entry = ctx.r4.u32, slot = ctx.r5.u32;
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SetOutput(voice, entry, slot, frame, call);
  });
}

ITC_REGS_STEP(XAudioSetOutputVoices) {
  const uint32_t voice = ctx.r3.u32, outputs = ctx.r4.u32;
  Framed(ctx, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SetOutputVoices(voice, outputs, frame, call);
  });
}

ITC_REGS_STEP(XAudioPass) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::Pass(voice, frame, call);
  });
}

ITC_REGS_STEP(XAudioDestroy) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, 128,
         [&](const xa::GuestCall& call) { return xa::Destroy(voice, call); });
}

ITC_REGS_STEP(XAudioCreateObject) {
  const uint32_t voice = ctx.r3.u32, entry = ctx.r4.u32, description = ctx.r5.u32,
                 flag = ctx.r6.u32;
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CreateObject(voice, entry, description, flag, frame, call);
  });
}

ITC_REGS_STEP(XAudioProcessEffect) {
  const uint32_t entry = ctx.r4.u32, inout = ctx.r5.u32;
  const uint32_t processor = guest::LoadU8(ctx.r13.u32 + xa::kPcrProcessor);
  Framed(ctx, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ProcessEffect(entry, inout, processor, frame, call);
  });
}

ITC_REGS_STEP(XAudioSourceSize) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, 240, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SourceSize(r3, r4, frame, call);
  });
}

ITC_REGS_STEP(XAudioCreateEffects) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CreateEffects(r3, r4, frame, call);
  });
}

ITC_REGS_STEP(XAudioInitBase) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::InitBase(r3, r4, frame, call);
  });
}

ITC_REGS_STEP(XAudioInitOutputs) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::InitOutputs(r3, r4, frame, call);
  });
}

ITC_REGS_STEP(XAudioSourceInit) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, 224, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SourceInit(r3, r4, frame, call);
  });
}

ITC_REGS_STEP(XAudioSubmixInit) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  Framed(ctx, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SubmixInit(r3, r4, frame, call);
  });
}

ITC_REGS_STEP(XAudioProcessEffects) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  const uint32_t processor = guest::LoadU8(ctx.r13.u32 + xa::kPcrProcessor);
  Framed(ctx, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ProcessEffects(r3, r4, processor, frame, call);
  });
}

ITC_REGS_STEP(XAudioMixToOutputs) {
  const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32;
  const uint32_t processor = guest::LoadU8(ctx.r13.u32 + xa::kPcrProcessor);
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MixToOutputs(r3, r4, processor, frame, call);
  });
}

ITC_REGS_STEP(XAudioSubmixPass) {
  const uint32_t r3 = ctx.r3.u32;
  Framed(ctx, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SubmixPass(r3, frame, call);
  });
}

// The thread's r13 (the spin lock's owner), its +256 (the mixer's thread
// check) and +268 (the processor).
xa::Thread ThreadOf(lifted::Regs& ctx) {
  xa::Thread thread;
  thread.r13 = ctx.r13.u32;
  thread.mixer = guest::LoadU32(ctx.r13.u32 + 256);
  thread.processor = guest::LoadU8(ctx.r13.u32 + xa::kPcrProcessor);
  return thread;
}

xa::GuestCall XAudioCaller(lifted::Regs& ctx) { return Caller(ctx); }
xa::Thread XAudioThreadOf(lifted::Regs& ctx) { return ThreadOf(ctx); }

ITC_REGS_STEP(XAudioSpinTake) {
  const xa::Thread th = ThreadOf(ctx);
  xa::SpinTake(th.r13, Caller(ctx));
}

ITC_REGS_STEP(XAudioSpinRelease) {
  const xa::Thread th = ThreadOf(ctx);
  xa::SpinRelease(th.r13, Caller(ctx));
}

ITC_REGS_STEP(XAudioSourceGetState) {
  const uint32_t voice = ctx.r3.u32, out = ctx.r4.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SourceGetState(voice, out, th.r13, frame, call);
  });
}

ITC_REGS_STEP(XAudioPlay) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128,
         [&](const xa::GuestCall& call) { return xa::Play(voice, th.r13, call); });
}

ITC_REGS_STEP(XAudioActivate) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::Activate(voice, th.r13, frame, call);
  });
}

ITC_REGS_STEP(XAudioDeactivate) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128,
         [&](const xa::GuestCall& call) { return xa::Deactivate(voice, th.r13, call); });
}

ITC_REGS_STEP(XAudioStartLater) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 144,
         [&](const xa::GuestCall& call) { return xa::StartLater(voice, th.r13, call); });
}

ITC_REGS_STEP(XAudioWaitForVoice) {
  // Leaves r3 the engine, as the original's early returns and loop do.
  const uint32_t engine = ctx.r3.u32, voice = ctx.r4.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 144, [&](const xa::GuestCall& call) {
    xa::WaitForVoice(engine, voice, th.r13, th.mixer, th.processor, call);
    return 0u;
  });
  ctx.r3.u64 = engine;
}

ITC_REGS_STEP(XAudioCancelPendingStart) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call) {
    return xa::CancelPendingStart(voice, th.r13, th.mixer, th.processor, call);
  });
}

ITC_REGS_STEP(XAudioStopOrTail) {
  const uint32_t voice = ctx.r3.u32, flags = ctx.r4.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call) {
    return xa::StopOrTail(voice, flags, th.r13, th.mixer, th.processor, call);
  });
}

ITC_REGS_STEP(XAudioSourceStart) {
  const uint32_t voice = ctx.r3.u32, flags = ctx.r4.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 96,
         [&](const xa::GuestCall& call) { return xa::SourceStart(voice, flags, th, call); });
}

ITC_REGS_STEP(XAudioSourceStop) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t flags = ctx.r4.u64;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SourceStop(voice, flags, th, frame, call);
  });
}

ITC_REGS_STEP(XAudioSourcePass) {
  const uint32_t voice = ctx.r3.u32, inout = ctx.r4.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SourcePass(voice, inout, th, frame, call);
  });
}

ITC_REGS_STEP(XAudioSourceConstruct) {
  const uint32_t voice = ctx.r3.u32, allocator = ctx.r4.u32, type = ctx.r5.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call) {
    return xa::SourceConstruct(voice, allocator, type, th, call);
  });
}

ITC_REGS_STEP(XAudioSubmixConstruct) {
  const uint32_t voice = ctx.r3.u32, description = ctx.r4.u32, allocator = ctx.r5.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call) {
    return xa::SubmixConstruct(voice, description, allocator, th, call);
  });
}

#define ITC_XAUDIO_VOICE_STEP(name, fn, bytes)                                     \
  ITC_REGS_STEP(name) {                                      \
    const uint32_t voice = ctx.r3.u32;                                             \
    const xa::Thread th = ThreadOf(ctx);                                     \
    Framed(ctx, bytes,                                                       \
           [&](const xa::GuestCall& call) { return xa::fn(voice, th, call); }); \
  }
ITC_XAUDIO_VOICE_STEP(XAudioOutputsDestroy, OutputsDestroy, 128)
ITC_XAUDIO_VOICE_STEP(XAudioSourceDestroy, SourceDestroy, 128)
ITC_XAUDIO_VOICE_STEP(XAudioSourceDeletingDestructor, SourceDeletingDestructor, 96)
ITC_XAUDIO_VOICE_STEP(XAudioSubmixDestroy, SubmixDestroy, 128)
ITC_XAUDIO_VOICE_STEP(XAudioSubmixDeletingDestructor, SubmixDeletingDestructor, 96)
#undef ITC_XAUDIO_VOICE_STEP

ITC_REGS_STEP(XAudioCreateSource) {
  const uint32_t description = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CreateSource(description, allocator, out, NativeHeapActive(), th, frame, call);
  });
}

ITC_REGS_STEP(XAudioCreateSubmix) {
  const uint32_t description = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 176, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CreateSubmix(description, allocator, out, NativeHeapActive(), th, frame, call);
  });
}

ITC_REGS_STEP(XAudioApiAddRef) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiAddRef(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiRelease) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiRelease(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiSetEffectParameters) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 144, [&](const xa::GuestCall& call) {
    return xa::ApiSetEffectParameters(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiSetOutputVoices) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiSetOutputVoices(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiSetOutputMatrix) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiSetOutputMatrix(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiGetState) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiGetState(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiStop) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiStop(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiGetOutputFormat) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiGetOutputFormat(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiSubmitPacket) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 128, [&](const xa::GuestCall& call) {
    return xa::ApiSubmitPacket(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiCodec36) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 128, [&](const xa::GuestCall& call) {
    return xa::ApiCodec36(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiFlushPackets) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiFlushPackets(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiCodec72) {
  const uint32_t voice = ctx.r3.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64, r7 = ctx.r7.u64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiCodec72(voice, r4, r5, r6, r7, call);
  });
}

ITC_REGS_STEP(XAudioApiSetFormat) {
  const uint32_t voice = ctx.r3.u32, format = ctx.r4.u32, outputs = ctx.r5.u32;
  const uint64_t r6 = ctx.r6.u64;
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ApiSetFormat(voice, format, outputs, r6, frame, call);
  });
}

ITC_REGS_STEP(XAudioApiReset) {
  const uint32_t voice = ctx.r3.u32;
  Framed(ctx, 112, [&](const xa::GuestCall& call) { return xa::ApiReset(voice, call); });
}

ITC_REGS_STEP(XAudioApiStart) {
  const uint32_t voice = ctx.r3.u32, flags = ctx.r4.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 112,
         [&](const xa::GuestCall& call) { return xa::ApiStart(voice, flags, th, call); });
}

ITC_REGS_STEP(XAudioApiSetVolume) {
  ctx.fpscr.disableFlushMode();
  const uint32_t voice = ctx.r3.u32;
  const double volume = ctx.f1.f64;
  Framed(ctx, 112,
         [&](const xa::GuestCall& call) { return xa::ApiSetVolume(voice, volume, call); });
}

ITC_REGS_STEP(XAudioApiSetFrequencyRatio) {
  const uint32_t voice = ctx.r3.u32;
  const double ratio = ctx.f1.f64;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    return xa::ApiSetFrequencyRatio(voice, ratio, call);
  });
}

ITC_REGS_STEP(XAudioApiRegisterCallback) {
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64;
  const uint32_t thread = ThreadOf(ctx).r13;
  Framed(ctx, 128, [&](const xa::GuestCall& call) {
    return uint32_t(xa::ApiRegisterCallback(r3, r4, r5, r6, thread, call));
  });
  ctx.r3.s64 = int32_t(ctx.r3.u32);  // 0, E_FAIL or E_OUTOFMEMORY as lis/ori leave them
}

// XAudioInitialize: r3 = the parameters (sub_8245A760 sets r4 = 0 and
// tail-calls sub_8245A410, which has no adapter: only this one reaches it in
// play); sub_8245A410's frame below the caller's, lr the caller's.
// sub_8245A760 has no plain C++ of its own (r4 = 0 and the tail call): its
// print is here.
ITC_REGS_STEP(XAudioApiInitialize) {
  ITC_NATIVE_RUNNING("sub_8245A760 XAudioInitialize (r4 = 0, tail call)");
  const uint32_t params = ctx.r3.u32;
  ctx.r4.u64 = 0;
  uint64_t r3 = 0;
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    r3 = xa::ApiInitialize(params, 0, frame, call);
    return uint32_t(r3);
  });
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(XAudioApiCommit) {
  const xa::Thread th = ThreadOf(ctx);
  uint64_t r3 = 0;
  Framed(ctx, 112, [&](const xa::GuestCall& call) {
    r3 = xa::ApiCommit(th, call);
    return uint32_t(r3);
  });
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(XAudioApiGetMasteringVoice) {
  const uint32_t out = ctx.r3.u32;
  Framed(ctx, 112,
         [&](const xa::GuestCall& call) { return xa::ApiGetMasteringVoice(out, call); });
}

ITC_REGS_STEP(XAudioApiCreateSourceVoice) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ApiCreateSourceVoice(description, out, NativeHeapActive(), th, frame, call);
  });
}

ITC_REGS_STEP(XAudioApiCreateSubmixVoice) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  const xa::Thread th = ThreadOf(ctx);
  Framed(ctx, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::ApiCreateSubmixVoice(description, out, NativeHeapActive(), th, frame, call);
  });
}

}  // namespace hooks

#define ITC_XAUDIO_ADAPTER(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_xaudio)) { \
    hooks::step(ctx);                 \
  }

ITC_XAUDIO_ADAPTER(8245FA28, XAudioBufferSize)
// No adapter (removed 2026-10-29: only XAudioSubmixVoice::Pass's, XAudioBuffer::ClearThunk's, XAudioMatrix::Mix's and PcmCodec::Process's plain C++ calls it in play) for XAudioBuffer::Clear sub_8245F8E8.
ITC_XAUDIO_ADAPTER(8245DEB0, XAudioEngineBuffer)
ITC_XAUDIO_ADAPTER(8245E318, XAudioGetType)
ITC_XAUDIO_ADAPTER(8245E328, XAudioGetFormat)
ITC_XAUDIO_ADAPTER(8245F860, XAudioAddRef)
ITC_XAUDIO_ADAPTER(8245FB00, XAudioSetRootVtable)
ITC_XAUDIO_ADAPTER(8245F640, XAudioAddStat)
ITC_XAUDIO_ADAPTER(8245F878, XAudioRelease)
ITC_XAUDIO_ADAPTER(8245AAD8, XAudioObjectFree)
ITC_XAUDIO_ADAPTER(8245DFC0, XAudioApplyVolume)
ITC_XAUDIO_ADAPTER(82461418, XAudioSetEffectParameters)
ITC_XAUDIO_ADAPTER(82460AB8, XAudioSetOutputMatrices)
ITC_XAUDIO_ADAPTER(8245FA10, XAudioBufferClearThunk)
ITC_XAUDIO_ADAPTER(82461158, XAudioBaseSize)
ITC_XAUDIO_ADAPTER(8245FA80, XAudioBufferCreate)
ITC_XAUDIO_ADAPTER(82461500, XAudioVoiceConstruct)
ITC_XAUDIO_ADAPTER(82460688, XAudioGetAllocator)
ITC_XAUDIO_ADAPTER(82460BF0, XAudioSetOutputVoices)
// sub_8245F228 is only a jump to SetOutputVoices, with no plain C++ of its
// own: its print is here.
ITC_NATIVE_ENTRY(8245F228, ITC_FLAG(native_xaudio)) {
  ITC_NATIVE_RUNNING("sub_8245F228 XAudioSubmixVoice::SetOutputVoices (jump)");
  hooks::XAudioSetOutputVoices(ctx);
}
ITC_XAUDIO_ADAPTER(82461070, XAudioPass)
ITC_XAUDIO_ADAPTER(82461D50, XAudioInitBase)
ITC_XAUDIO_ADAPTER(82461E88, XAudioProcessEffects)
ITC_XAUDIO_ADAPTER(82460CB8, XAudioMixToOutputs)
ITC_XAUDIO_ADAPTER(8245EFA0, XAudioSubmixPass)
ITC_XAUDIO_ADAPTER(8245DDC8, XAudioSourceGetState)
ITC_XAUDIO_ADAPTER(82461798, XAudioPlay)
ITC_XAUDIO_ADAPTER(82460EF0, XAudioActivate)
ITC_XAUDIO_ADAPTER(82460D50, XAudioDeactivate)
ITC_XAUDIO_ADAPTER(824619B0, XAudioStopOrTail)
ITC_XAUDIO_ADAPTER(8245EBA0, XAudioSourceStart)
ITC_XAUDIO_ADAPTER(8245E9F0, XAudioSourceStop)
ITC_XAUDIO_ADAPTER(8245DED0, XAudioSourcePass)
ITC_XAUDIO_ADAPTER(8245E7A0, XAudioSourceDeletingDestructor)
ITC_XAUDIO_ADAPTER(8245F3A0, XAudioSubmixDeletingDestructor)
ITC_XAUDIO_ADAPTER(82459DB0, XAudioApiAddRef)
ITC_XAUDIO_ADAPTER(82459E18, XAudioApiRelease)
// No adapter (removed 2026-10-29: only CarSounds::Init's and CarSound::Update's plain C++ calls it in play) for XAudioVoice::SetEffectParameters, the API's, sub_82459E80.
ITC_XAUDIO_ADAPTER(82459EF0, XAudioApiSetOutputVoices)
ITC_XAUDIO_ADAPTER(82459F48, XAudioApiSetOutputMatrix)
// No adapter (removed 2026-10-29: only the car sounds' plain C++ (IsPlaying, Pause, UpdateVoices, EngineLayer::Update, RollSound::Update, TireSound::Update) and XACTVoice::GetState's plain C++ calls it in play) for XAudioVoice::GetState, the API's sub_82459FA0.
// No adapter (removed 2026-10-29: only the car sounds' plain C++ (Pause, Clear, UpdateVoices, the parts' updates) and XACTVoice::Stop's plain C++ calls it in play) for XAudioVoice::Stop, the API's sub_8245A1B0.
ITC_XAUDIO_ADAPTER(8245A3B8, XAudioApiGetOutputFormat)
// No adapter (removed 2026-10-29: only CarSound::AddVoice's plain C++ calls it in play) for XAudioSourceVoice::SubmitPacket sub_8245A050.
// No adapter (removed 2026-10-29: only XACTVoice::SubmitPacket's plain C++ calls it in play) for XAudioSourceVoice, the codec's +36 sub_8245A108.
// No adapter (removed 2026-10-29: only XACTVoice::FlushPackets' plain C++ calls it in play) for XAudioSourceVoice::FlushPackets sub_8245A208.
ITC_XAUDIO_ADAPTER(8245A270, XAudioApiCodec72)
ITC_XAUDIO_ADAPTER(82459FF8, XAudioApiSetFormat)
ITC_XAUDIO_ADAPTER(8245A0B0, XAudioApiReset)
// No adapter (removed 2026-10-29: only the car sounds' plain C++ (Resume, EngineLayer::Update, TireLayer::Update, RollSound::Update, PlayOnce) and XACTVoice::Start's plain C++ calls it in play) for XAudioVoice::Start, the API's sub_8245A168.
// No adapter (removed 2026-10-29: only the car sounds' plain C++ (the parts' updates, PlayOnce) and XACTVoice::SetVolume's plain C++ calls it in play) for XAudioSourceVoice::SetVolume sub_8245A2C8.
// No adapter (removed 2026-10-29: only CarSound::SetPitch's, SetPitchScaled's and XACTVoice::SetPitch's plain C++ calls it in play) for XAudioSourceVoice::SetFrequencyRatio sub_8245A340.
ITC_XAUDIO_ADAPTER(8245A568, XAudioApiGetMasteringVoice)
ITC_XAUDIO_ADAPTER(8245A5E8, XAudioApiRegisterCallback)
ITC_XAUDIO_ADAPTER(8245A6E8, XAudioApiCommit)
ITC_XAUDIO_ADAPTER(8245A760, XAudioApiInitialize)
ITC_XAUDIO_ADAPTER(8245A768, XAudioApiCreateSourceVoice)
ITC_XAUDIO_ADAPTER(8245A7C8, XAudioApiCreateSubmixVoice)
