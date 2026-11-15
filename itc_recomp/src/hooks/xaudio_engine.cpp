// XAudio's engine and mixer natively: adapters in front of
// src/xaudio/engine.h (docs/functions/xaudio-engine.md), with the register
// steps the harness shares (hooks/xaudio_engine.h). Many of these are reached
// through vtables, so the adapters stay until their callers are native.

#include "hooks/xaudio_engine.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/xaudio.h"
#include "xaudio/engine.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_xaudio_engine, true, "ITC",
              "Run the parts of XAudio's engine and mixer made native "
              "(docs/functions/xaudio-engine.md: XAudioFactory::TableSize sub_8245D9E8 and "
              "SizeOf sub_8245DBD0, XAudioBuffer::GetFormat sub_8245F6F0 and its thunk "
              "sub_8245F718, sub_8245F8D0, XAudioAllocator::Remaining sub_82460388 and Alloc "
              "sub_82460288, sub_82460370, sub_82461EF8, XAudioEngine::TakeStats sub_8245F678 "
              "and Barrier sub_8245A940, XAudio::Free sub_8245EF90 (XMemFree's plain C++ while "
              "--native_heap routes the heap) and sub_8245AAB8; the engine's constructor "
              "sub_8245C098 and Size sub_8245B2C0, XAudio::SetUpProcessors sub_8245A828 (the "
              "kernel's KeGetCurrentProcessType through the translated code) and RegisterStats "
              "sub_8245F588, the factory's constructor sub_8245DA48, Create sub_8245DC10 and its "
              "slots sub_8245D650 and sub_8245D6D8; XAudio::CreateEngine sub_82459D60 with "
              "XAudioEngine::Create sub_8245C538 and Init sub_8245C198 (its kernel objects and "
              "threads through the kernel's imports, as the original); the mastering voice's Size sub_8245FF68, "
              "Init sub_8245FCA0, Create sub_824600F0, Start sub_8245FE20, Pass sub_8245FDC8 and "
              "Process sub_8245FB80, the render driver's constructor sub_82465938; under the voice "
              "lists' spin lock (taken as the original does) XAudioStage::Swap sub_8245B208, "
              "XAudioEngine::SwapStageLists sub_8245B720, RunStage sub_8245B480, RunCallbacks "
              "sub_8245ACE0, RegisterCallback sub_8245AB60 and Commit sub_8245B638, StageOf "
              "sub_8245FBF0 and sub_82460000; the allocators' destructors sub_824602F8, sub_824604E8 "
              "and its thunk sub_824603A0, and the interface's free sub_824603A8; XAudioEngine::Mix "
              "sub_8245BF80 with XAudio::MixerCallback sub_8245FBE0 (the mixer's lock entered as "
              "the original does), XAudio::MasterThread sub_8245BE08 and WorkerThread "
              "sub_8245BED0 (their events and semaphore through the kernel's imports, as the "
              "original)) natively instead of through the "
              "translated code (the same reads and writes; checked by "
              "--run_harnesses=xaudio_engine)");

namespace hooks {

namespace xa = xaudio;

ITC_REGS_STEP(XAudioFactoryTableSize) {
  ctx.r3.u64 = xa::FactoryTableSize(base, ctx.r3.u32);
}

ITC_REGS_STEP(XAudioFactorySizeOf) {
  ctx.r3.u64 = xa::FactorySizeOf(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioBufferGetFormat) {
  ctx.r3.u64 = xa::BufferGetFormat(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioBufferGetFormatThunk) {
  ctx.r3.u64 = xa::BufferGetFormatThunk(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioBufferInterface) {
  xa::BufferInterface(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioAllocatorRemaining) {
  ctx.r3.u64 = xa::AllocatorRemaining(base, ctx.r3.u32);
}

ITC_REGS_STEP(XAudioInterfaceAddRef) {
  ctx.r3.u64 = xa::InterfaceAddRef(base, ctx.r3.u32);
}

ITC_REGS_STEP(XAudioObjectInfo) {
  ctx.r3.u64 = xa::ObjectInfo(base, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioTakeStats) { ctx.r3.u64 = xa::TakeStats(base); }

ITC_REGS_STEP(XAudioFree) {
  // XMemFree (sub_82297790)'s plain C++ while the heap is the native
  // layer's, else the original (a tail call to XMemFree's adapter).
  if (NativeHeapActive()) {
    ctx.r3.u64 = xa::Free(ctx.r4.u32, ctx.r5.u32);
    return;
  }
  lifted::Sdk(ctx, base, __imp__sub_8245EF90);
}

ITC_REGS_STEP(XAudioAllocatorAlloc) {
  const uint32_t allocator = ctx.r3.u32, size = ctx.r4.u32;
  uint64_t r3 = 0;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t) {
    r3 = xa::AllocatorAlloc(base, allocator, size, call);
    return 0u;
  });
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(XAudioDeleteObject) {
  uint32_t function = 0;
  xa::DeleteObject(base, ctx.r3.u32, &function);
  if (function == 0) {
    return;
  }
  // A tail call: the destructor runs at the caller's frame, lr the caller's.
  ctx.r4.s64 = 1;
  lifted::CallFromContext(ctx, base, function);
}

ITC_REGS_STEP(XAudioBarrier) {
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  xa::Barrier(base, ctx.r3.u32, ctx.r4.u32, processor);
}

ITC_REGS_STEP(XAudioEngineConstruct) {
  const uint32_t engine = ctx.r3.u32, allocator = ctx.r4.u32;
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t) {
    return xa::EngineConstruct(base, engine, allocator, call);
  });
}

ITC_REGS_STEP(XAudioFactoryConstruct) {
  const uint32_t factory = ctx.r3.u32, table = ctx.r4.u32, allocator = ctx.r5.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::FactoryConstruct(base, factory, table, allocator, frame, call);
  });
}

ITC_REGS_STEP(XAudioFactoryCreateObject) {
  const uint32_t table = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::FactoryCreate(base, table, allocator, out, frame, call);
  });
  ctx.r3.s64 = int32_t(ctx.r3.u32);  // E_OUTOFMEMORY as the original's lis/ori leave it
}

namespace {

// The factory's slots: the class's function tail-called with the
// descriptor in r3 and the other arguments one register down, as the
// original moves them; E_NOTIMPL (its lis/ori, sign-extended) without one.
void FactorySlot(lifted::Regs& ctx, uint8_t* base, uint32_t which) {
  uint32_t function = 0;
  const uint32_t r3 = xa::FactoryEntry(base, ctx.r3.u32, ctx.r4.u32, which, &function);
  // The create slot moves r5 to r4 only for the call; the size slot moves
  // it first, and r6 to r5 for the call.
  ctx.r3.u64 = ctx.r4.u64;
  if (which == 1) {
    ctx.r4.u64 = ctx.r5.u64;
  }
  if (function == 0) {
    ctx.r3.s64 = int32_t(r3);
    return;
  }
  if (which == 0) {
    ctx.r4.u64 = ctx.r5.u64;
  } else {
    ctx.r5.u64 = ctx.r6.u64;
  }
  lifted::CallFromContext(ctx, base, function);
}

}  // namespace

ITC_REGS_STEP(XAudioFactoryCreateSlot) { FactorySlot(ctx, base, 0); }
ITC_REGS_STEP(XAudioFactorySizeSlot) { FactorySlot(ctx, base, 1); }

ITC_REGS_STEP(XAudioRegisterStats) {
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::RegisterStats(base, frame, call);
  });
}

ITC_REGS_STEP(XAudioSetUpProcessors) {
  const uint32_t mask = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SetUpProcessors(base, mask, out, frame, call);
  });
}

ITC_REGS_STEP(XAudioEngineSize) {
  const uint32_t params = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 176, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::EngineSize(base, params, out, frame, call);
  });
}

ITC_REGS_STEP(XAudioMasteringSize) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringSize(base, description, out, frame, call);
  });
}

ITC_REGS_STEP(XAudioRenderDriverConstruct) {
  xa::RenderDriverConstruct(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(XAudioMasteringInit) {
  const uint32_t voice = ctx.r3.u32, description = ctx.r4.u32;
  XAudioFramed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringInit(base, voice, description, frame, call);
  });
}

ITC_REGS_STEP(XAudioMasteringCreate) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringCreate(base, description, out, NativeHeapActive(), frame, call);
  });
}

ITC_REGS_STEP(XAudioMasteringStart) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringStart(base, voice, frame, thread, call);
  });
}

ITC_REGS_STEP(XAudioMasteringPass) {
  const uint32_t voice = ctx.r3.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringPass(base, voice, frame, call);
  });
}

ITC_REGS_STEP(XAudioMasteringProcess) {
  const uint32_t voice = ctx.r3.u32, inout = ctx.r4.u32;
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringProcess(base, voice, inout, processor, frame, call);
  });
}

// The functions under the spin lock whose r3 is what their last kernel call
// left (none sets it): the body's result is the context's r3 after it.
ITC_REGS_STEP(XAudioStageSwap) {
  const uint32_t stage = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    xa::StageSwap(base, stage, thread.r13, call);
    return ctx.r3.u32;
  });
}

ITC_REGS_STEP(XAudioSwapStageLists) {
  const uint32_t engine = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    xa::SwapStageLists(base, engine, thread.r13, call);
    return ctx.r3.u32;
  });
}

ITC_REGS_STEP(XAudioRunStage) {
  const uint32_t engine = ctx.r3.u32, stage = ctx.r4.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t) {
    xa::RunStage(base, engine, stage, thread.processor, thread.r13, call);
    return ctx.r3.u32;
  });
}

ITC_REGS_STEP(XAudioRunCallbacks) {
  const uint32_t engine = ctx.r3.u32, stage = ctx.r4.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t) {
    xa::RunCallbacks(base, engine, stage, thread.r13, call);
    return ctx.r3.u32;
  });
}

ITC_REGS_STEP(XAudioRegisterCallback) {
  const uint32_t engine = ctx.r3.u32, stage = ctx.r4.u32, function = ctx.r5.u32,
                 context = ctx.r6.u32, out = ctx.r7.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 160, [&](const xa::GuestCall& call, uint32_t) {
    return xa::RegisterCallback(base, engine, stage, function, context, out, thread.r13, call);
  });
  ctx.r3.s64 = int32_t(ctx.r3.u32);  // E_OUTOFMEMORY as the original's lis/ori leave it
}

ITC_REGS_STEP(XAudioEngineCommit) {
  const uint32_t engine = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    return xa::Commit(base, engine, thread, call);
  });
}

ITC_REGS_STEP(XAudioStageOf) {
  const uint32_t engine = ctx.r3.u32, voice = ctx.r4.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::StageOf(base, engine, voice, frame, call);
  });
}

ITC_REGS_STEP(XAudioJoinMix) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::JoinMix(base, voice, thread.r13, frame, call);
  });
}

ITC_REGS_STEP(XAudioAllocatorDestroy) {
  const uint32_t allocator = ctx.r3.u32;
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::AllocatorDestroy(base, allocator, NativeHeapActive(), frame, call);
  });
}

ITC_REGS_STEP(XAudioDefaultAllocatorDestroy) {
  const uint32_t allocator = ctx.r3.u32;
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::DefaultAllocatorDestroy(base, allocator, NativeHeapActive(), frame, call);
  });
}

ITC_REGS_STEP(XAudioDefaultAllocatorThunk) {
  // this - 4, then the destructor's tail call (its frame at the caller's).
  ITC_NATIVE_RUNNING("sub_824603A0 XAudioDefaultAllocator::~XAudioDefaultAllocator (thunk)");
  ctx.r3.s64 = ctx.r3.s64 - 4;
  XAudioDefaultAllocatorDestroy(ctx, base);
}

ITC_REGS_STEP(XAudioInterfaceFree) {
  const uint32_t iface = ctx.r3.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::InterfaceFree(base, iface, NativeHeapActive(), frame, call);
  });
}

ITC_REGS_STEP(XAudioMix) {
  const uint32_t engine = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 192, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::Mix(base, engine, frame, thread, call);
  });
}

ITC_REGS_STEP(XAudioMixerCallback) {
  // Mix's frame, made where the callback's caller called it (a tail call).
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 192, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MixerCallback(base, frame, thread, call);
  });
}

ITC_REGS_STEP(XAudioMasterThread) {
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasterThread(base, frame, thread, call);
  });
}

ITC_REGS_STEP(XAudioWorkerThread) {
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t) {
    return xa::WorkerThread(base, thread, call);
  });
}

// The engine's making: r3 in full, as the original leaves it.
template <typename Body>
void FramedFull(lifted::Regs& ctx, uint8_t* base, uint32_t frame_bytes, Body body) {
  uint64_t r3 = 0;
  XAudioFramed(ctx, base, frame_bytes, [&](const xa::GuestCall& call, uint32_t frame) {
    r3 = body(call, frame);
    return uint32_t(r3);
  });
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(XAudioEngineInit) {
  const uint32_t engine = ctx.r3.u32, params = ctx.r4.u32;
  FramedFull(ctx, base, 208, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::EngineInit(base, engine, params, NativeHeapActive(), frame, call);
  });
}

ITC_REGS_STEP(XAudioEngineCreate) {
  const uint32_t params = ctx.r3.u32, allocator = ctx.r4.u32, out = ctx.r5.u32;
  FramedFull(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::EngineCreate(base, params, allocator, out, NativeHeapActive(), frame, call);
  });
}

ITC_REGS_STEP(XAudioCreateEngine) {
  const uint32_t params = ctx.r5.u32, allocator = ctx.r6.u32;
  FramedFull(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::CreateEngine(base, params, allocator, NativeHeapActive(), frame, call);
  });
}

}  // namespace hooks

#define ITC_XAUDIO_ENGINE_ADAPTER(address, step) \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_xaudio_engine)) { \
    hooks::step(ctx, base);                      \
  }

// No adapter (removed 2026-10-29: only XAudioFactory::SizeOf's plain C++ calls it in play) for XAudioFactory::TableSize sub_8245D9E8.
// No adapter (removed 2026-10-29: only XAudioEngine::Size's plain C++ calls it in play) for XAudioFactory::SizeOf sub_8245DBD0.
// No adapter (removed 2026-10-29: only XAudioMatrix::Mix's and PcmCodec::Process's plain C++ calls it in play) for XAudioBuffer::GetFormat sub_8245F6F0.
ITC_XAUDIO_ENGINE_ADAPTER(8245F718, XAudioBufferGetFormatThunk)
ITC_XAUDIO_ENGINE_ADAPTER(8245F8D0, XAudioBufferInterface)
ITC_XAUDIO_ENGINE_ADAPTER(82460388, XAudioAllocatorRemaining)
ITC_XAUDIO_ENGINE_ADAPTER(82460370, XAudioInterfaceAddRef)
ITC_XAUDIO_ENGINE_ADAPTER(82461EF8, XAudioObjectInfo)
// No adapter (removed 2026-10-29: only XAudioEngine::Mix's plain C++ calls it in play) for XAudioEngine::TakeStats sub_8245F678.
// No adapter (removed 2026-10-29: only the allocators' destructors' plain C++ (sub_824602F8, sub_824603A8) calls it in play) for XAudio::Free sub_8245EF90.
ITC_XAUDIO_ENGINE_ADAPTER(82460288, XAudioAllocatorAlloc)
ITC_XAUDIO_ENGINE_ADAPTER(8245AAB8, XAudioDeleteObject)
ITC_XAUDIO_ENGINE_ADAPTER(8245A940, XAudioBarrier)
// No adapter (removed 2026-10-29: only XAudioEngine::Create's plain C++ calls it in play) for XAudioEngine::XAudioEngine sub_8245C098.
// No adapter (removed 2026-10-29: only XAudioFactory::Create's plain C++ calls it in play) for XAudioFactory::XAudioFactory sub_8245DA48.
// No adapter (removed 2026-10-29: only XAudioEngine::Init's plain C++ calls it in play) for XAudioFactory::Create sub_8245DC10.
ITC_XAUDIO_ENGINE_ADAPTER(8245D650, XAudioFactoryCreateSlot)
ITC_XAUDIO_ENGINE_ADAPTER(8245D6D8, XAudioFactorySizeSlot)
// No adapter (removed 2026-10-29: only XAudioEngine::Init's plain C++ calls it in play) for XAudio::RegisterStats sub_8245F588.
// Kept while XAudioEngine::Init's harness runs its original: the stand-in
// point (xaudio::SetMakingForHarness) its calls reach.
ITC_XAUDIO_ENGINE_ADAPTER(8245A828, XAudioSetUpProcessors)
// No adapter (removed 2026-10-29: only XAudioEngine::Create's plain C++ calls it in play) for XAudioEngine::Size sub_8245B2C0.
// No adapter (removed 2026-10-29: only XAudioMasteringVoice::Create's plain C++ calls it in play) for XAudioMasteringVoice::Size sub_8245FF68.
// No adapter (removed 2026-10-29: only XAudioMasteringVoice::Init's plain C++ calls it in play) for AudioRenderDriver::AudioRenderDriver sub_82465938.
// No adapter (removed 2026-10-29: only XAudioMasteringVoice::Create's plain C++ calls it in play) for XAudioMasteringVoice::Init sub_8245FCA0.
// Kept as SetUpProcessors' (the harness's stand-in point for Init's
// original).
ITC_XAUDIO_ENGINE_ADAPTER(824600F0, XAudioMasteringCreate)
ITC_XAUDIO_ENGINE_ADAPTER(8245FE20, XAudioMasteringStart)
ITC_XAUDIO_ENGINE_ADAPTER(8245FDC8, XAudioMasteringPass)
ITC_XAUDIO_ENGINE_ADAPTER(8245FB80, XAudioMasteringProcess)
// No adapter (removed 2026-10-29: only XAudioEngine::SwapStageLists' plain C++ calls it in play) for XAudioStage::Swap sub_8245B208.
// No adapter (removed 2026-10-29: only XAudioEngine::Mix's and XAudio::MasterThread's plain C++ call it in play) for XAudioEngine::SwapStageLists sub_8245B720.
ITC_XAUDIO_ENGINE_ADAPTER(8245B480, XAudioRunStage)
ITC_XAUDIO_ENGINE_ADAPTER(8245ACE0, XAudioRunCallbacks)
ITC_XAUDIO_ENGINE_ADAPTER(8245AB60, XAudioRegisterCallback)
ITC_XAUDIO_ENGINE_ADAPTER(8245B638, XAudioEngineCommit)
// No adapter (removed 2026-10-29: only sub_82460000's plain C++ calls it in play) for XAudioEngine::StageOf sub_8245FBF0.
ITC_XAUDIO_ENGINE_ADAPTER(82460000, XAudioJoinMix)
// No adapter (removed 2026-10-29: only sub_824604E8's plain C++ calls it in play) for XAudioAllocator::~XAudioAllocator sub_824602F8.
// No adapter (removed 2026-10-29: only sub_824603A0's adapter (its register step) calls it in play) for the default allocator's destructor sub_824604E8.
ITC_XAUDIO_ENGINE_ADAPTER(824603A0, XAudioDefaultAllocatorThunk)
ITC_XAUDIO_ENGINE_ADAPTER(824603A8, XAudioInterfaceFree)
// The mix and XAudio's threads (their kernel objects through the imports, as
// the original; the threads' entry points stay these addresses).
// No adapter (removed 2026-10-29: only XAudio::MixerCallback's plain C++ calls it in play) for XAudioEngine::Mix sub_8245BF80.
ITC_XAUDIO_ENGINE_ADAPTER(8245FBE0, XAudioMixerCallback)
ITC_XAUDIO_ENGINE_ADAPTER(8245BE08, XAudioMasterThread)
ITC_XAUDIO_ENGINE_ADAPTER(8245BED0, XAudioWorkerThread)
// The engine's making (its kernel objects through the imports, as the
// original).
// No adapter (removed 2026-10-29: only XAudioEngine::Create's plain C++ calls it in play) for XAudioEngine::Init sub_8245C198.
// No adapter (removed 2026-10-29: only XAudio::CreateEngine's plain C++ calls it in play) for XAudioEngine::Create sub_8245C538.
ITC_XAUDIO_ENGINE_ADAPTER(82459D60, XAudioCreateEngine)
