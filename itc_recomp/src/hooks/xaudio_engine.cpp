// XAudio's engine and mixer natively: adapters in front of
// src/xaudio/engine.h (docs/functions/xaudio-engine.md), with the register
// steps the harness shares (hooks/xaudio_engine.h). The engine's set-up, the
// mixer's callback and XAudio's processor threads stay recompiled (their
// kernel objects wait on the user's call), and many of these are reached
// through vtables, so the adapters stay until their callers are native.

#include "hooks/xaudio_engine.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/xaudio.h"
#include "xaudio/engine.h"

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
              "slots sub_8245D650 and sub_8245D6D8; the engine's set-up, the mixer's callback and "
              "XAudio's processor threads stay recompiled; the mastering voice's Size sub_8245FF68, "
              "Init sub_8245FCA0, Create sub_824600F0, Start sub_8245FE20, Pass sub_8245FDC8 and "
              "Process sub_8245FB80, the render driver's constructor sub_82465938; under the voice "
              "lists' spin lock (taken as the original does) XAudioStage::Swap sub_8245B208, "
              "XAudioEngine::SwapStageLists sub_8245B720, RunStage sub_8245B480, RunCallbacks "
              "sub_8245ACE0, RegisterCallback sub_8245AB60 and Commit sub_8245B638, StageOf "
              "sub_8245FBF0 and sub_82460000; the allocators' destructors sub_824602F8, sub_824604E8 "
              "and its thunk sub_824603A0, and the interface's free sub_824603A8) natively instead of through the "
              "translated code (the same reads and writes; checked by "
              "--run_harnesses=xaudio_engine)");

namespace hooks {

namespace xa = xaudio;

void XAudioFactoryTableSize(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::FactoryTableSize(base, ctx.r3.u32);
}

void XAudioFactorySizeOf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::FactorySizeOf(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioBufferGetFormat(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::BufferGetFormat(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioBufferGetFormatThunk(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::BufferGetFormatThunk(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioBufferInterface(PPCContext& ctx, uint8_t* base) {
  xa::BufferInterface(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioAllocatorRemaining(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::AllocatorRemaining(base, ctx.r3.u32);
}

void XAudioInterfaceAddRef(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::InterfaceAddRef(base, ctx.r3.u32);
}

void XAudioObjectInfo(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = xa::ObjectInfo(base, ctx.r4.u32);
}

void XAudioTakeStats(PPCContext& ctx, uint8_t* base) { ctx.r3.u64 = xa::TakeStats(base); }

void XAudioFree(PPCContext& ctx, uint8_t* base) {
  // XMemFree (sub_82297790)'s plain C++ while the heap is the native
  // layer's, else the original (a tail call to XMemFree's adapter).
  if (NativeHeapActive()) {
    ctx.r3.u64 = xa::Free(ctx.r4.u32, ctx.r5.u32);
    return;
  }
  __imp__sub_8245EF90(ctx, base);
}

void XAudioAllocatorAlloc(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocator = ctx.r3.u32, size = ctx.r4.u32;
  uint64_t r3 = 0;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t) {
    r3 = xa::AllocatorAlloc(base, allocator, size, call);
    return 0u;
  });
  ctx.r3.u64 = r3;
}

void XAudioDeleteObject(PPCContext& ctx, uint8_t* base) {
  uint32_t function = 0;
  xa::DeleteObject(base, ctx.r3.u32, &function);
  if (function == 0) {
    return;
  }
  // A tail call: the destructor runs at the caller's frame, lr the caller's.
  ctx.r4.s64 = 1;
  REX_CALL_INDIRECT_FUNC(function);
}

void XAudioBarrier(PPCContext& ctx, uint8_t* base) {
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  xa::Barrier(base, ctx.r3.u32, ctx.r4.u32, processor);
}

void XAudioEngineConstruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32, allocator = ctx.r4.u32;
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t) {
    return xa::EngineConstruct(base, engine, allocator, call);
  });
}

void XAudioFactoryConstruct(PPCContext& ctx, uint8_t* base) {
  const uint32_t factory = ctx.r3.u32, table = ctx.r4.u32, allocator = ctx.r5.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::FactoryConstruct(base, factory, table, allocator, frame, call);
  });
}

void XAudioFactoryCreateObject(PPCContext& ctx, uint8_t* base) {
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
void FactorySlot(PPCContext& ctx, uint8_t* base, uint32_t which) {
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
  REX_CALL_INDIRECT_FUNC(function);
}

}  // namespace

void XAudioFactoryCreateSlot(PPCContext& ctx, uint8_t* base) { FactorySlot(ctx, base, 0); }
void XAudioFactorySizeSlot(PPCContext& ctx, uint8_t* base) { FactorySlot(ctx, base, 1); }

void XAudioRegisterStats(PPCContext& ctx, uint8_t* base) {
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::RegisterStats(base, frame, call);
  });
}

void XAudioSetUpProcessors(PPCContext& ctx, uint8_t* base) {
  const uint32_t mask = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::SetUpProcessors(base, mask, out, frame, call);
  });
}

void XAudioEngineSize(PPCContext& ctx, uint8_t* base) {
  const uint32_t params = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 176, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::EngineSize(base, params, out, frame, call);
  });
}

void XAudioMasteringSize(PPCContext& ctx, uint8_t* base) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringSize(base, description, out, frame, call);
  });
}

void XAudioRenderDriverConstruct(PPCContext& ctx, uint8_t* base) {
  xa::RenderDriverConstruct(base, ctx.r3.u32, ctx.r4.u32);
}

void XAudioMasteringInit(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, description = ctx.r4.u32;
  XAudioFramed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringInit(base, voice, description, frame, call);
  });
}

void XAudioMasteringCreate(PPCContext& ctx, uint8_t* base) {
  const uint32_t description = ctx.r3.u32, out = ctx.r4.u32;
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringCreate(base, description, out, NativeHeapActive(), frame, call);
  });
}

void XAudioMasteringStart(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringStart(base, voice, frame, thread, call);
  });
}

void XAudioMasteringPass(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringPass(base, voice, frame, call);
  });
}

void XAudioMasteringProcess(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32, inout = ctx.r4.u32;
  const uint32_t processor = guest::LoadU8(base, ctx.r13.u32 + xa::kPcrProcessor);
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::MasteringProcess(base, voice, inout, processor, frame, call);
  });
}

// The functions under the spin lock whose r3 is what their last kernel call
// left (none sets it): the body's result is the context's r3 after it.
void XAudioStageSwap(PPCContext& ctx, uint8_t* base) {
  const uint32_t stage = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    xa::StageSwap(base, stage, thread.r13, call);
    return ctx.r3.u32;
  });
}

void XAudioSwapStageLists(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    xa::SwapStageLists(base, engine, thread.r13, call);
    return ctx.r3.u32;
  });
}

void XAudioRunStage(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32, stage = ctx.r4.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t) {
    xa::RunStage(base, engine, stage, thread.processor, thread.r13, call);
    return ctx.r3.u32;
  });
}

void XAudioRunCallbacks(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32, stage = ctx.r4.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 144, [&](const xa::GuestCall& call, uint32_t) {
    xa::RunCallbacks(base, engine, stage, thread.r13, call);
    return ctx.r3.u32;
  });
}

void XAudioRegisterCallback(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32, stage = ctx.r4.u32, function = ctx.r5.u32,
                 context = ctx.r6.u32, out = ctx.r7.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 160, [&](const xa::GuestCall& call, uint32_t) {
    return xa::RegisterCallback(base, engine, stage, function, context, out, thread.r13, call);
  });
  ctx.r3.s64 = int32_t(ctx.r3.u32);  // E_OUTOFMEMORY as the original's lis/ori leave it
}

void XAudioEngineCommit(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t) {
    return xa::Commit(base, engine, thread, call);
  });
}

void XAudioStageOf(PPCContext& ctx, uint8_t* base) {
  const uint32_t engine = ctx.r3.u32, voice = ctx.r4.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::StageOf(base, engine, voice, frame, call);
  });
}

void XAudioJoinMix(PPCContext& ctx, uint8_t* base) {
  const uint32_t voice = ctx.r3.u32;
  const xa::Thread thread = XAudioThreadOf(ctx, base);
  XAudioFramed(ctx, base, 128, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::JoinMix(base, voice, thread.r13, frame, call);
  });
}

void XAudioAllocatorDestroy(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocator = ctx.r3.u32;
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::AllocatorDestroy(base, allocator, NativeHeapActive(), frame, call);
  });
}

void XAudioDefaultAllocatorDestroy(PPCContext& ctx, uint8_t* base) {
  const uint32_t allocator = ctx.r3.u32;
  XAudioFramed(ctx, base, 96, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::DefaultAllocatorDestroy(base, allocator, NativeHeapActive(), frame, call);
  });
}

void XAudioDefaultAllocatorThunk(PPCContext& ctx, uint8_t* base) {
  // this - 4, then the destructor's tail call (its frame at the caller's).
  ITC_NATIVE_RUNNING("sub_824603A0 XAudioDefaultAllocator::~XAudioDefaultAllocator (thunk)");
  ctx.r3.s64 = ctx.r3.s64 - 4;
  XAudioDefaultAllocatorDestroy(ctx, base);
}

void XAudioInterfaceFree(PPCContext& ctx, uint8_t* base) {
  const uint32_t iface = ctx.r3.u32;
  XAudioFramed(ctx, base, 112, [&](const xa::GuestCall& call, uint32_t frame) {
    return xa::InterfaceFree(base, iface, NativeHeapActive(), frame, call);
  });
}

}  // namespace hooks

#define ITC_XAUDIO_ENGINE_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {           \
    if (!ITC_FLAG(native_xaudio_engine)) {       \
      __imp__sub_##address(ctx, base);           \
      return;                                    \
    }                                            \
    hooks::step(ctx, base);                      \
  }

// No adapter (removed 2026-10-29: only XAudioFactory::SizeOf's plain C++ calls it in play) for XAudioFactory::TableSize sub_8245D9E8.
// No adapter (removed 2026-10-29: only XAudioEngine::Size's plain C++ calls it in play) for XAudioFactory::SizeOf sub_8245DBD0.
ITC_XAUDIO_ENGINE_ADAPTER(8245F6F0, XAudioBufferGetFormat)
ITC_XAUDIO_ENGINE_ADAPTER(8245F718, XAudioBufferGetFormatThunk)
ITC_XAUDIO_ENGINE_ADAPTER(8245F8D0, XAudioBufferInterface)
ITC_XAUDIO_ENGINE_ADAPTER(82460388, XAudioAllocatorRemaining)
ITC_XAUDIO_ENGINE_ADAPTER(82460370, XAudioInterfaceAddRef)
ITC_XAUDIO_ENGINE_ADAPTER(82461EF8, XAudioObjectInfo)
ITC_XAUDIO_ENGINE_ADAPTER(8245F678, XAudioTakeStats)
// No adapter (removed 2026-10-29: only the allocators' destructors' plain C++ (sub_824602F8, sub_824603A8) calls it in play) for XAudio::Free sub_8245EF90.
ITC_XAUDIO_ENGINE_ADAPTER(82460288, XAudioAllocatorAlloc)
ITC_XAUDIO_ENGINE_ADAPTER(8245AAB8, XAudioDeleteObject)
ITC_XAUDIO_ENGINE_ADAPTER(8245A940, XAudioBarrier)
ITC_XAUDIO_ENGINE_ADAPTER(8245C098, XAudioEngineConstruct)
// No adapter (removed 2026-10-29: only XAudioFactory::Create's plain C++ calls it in play) for XAudioFactory::XAudioFactory sub_8245DA48.
ITC_XAUDIO_ENGINE_ADAPTER(8245DC10, XAudioFactoryCreateObject)
ITC_XAUDIO_ENGINE_ADAPTER(8245D650, XAudioFactoryCreateSlot)
ITC_XAUDIO_ENGINE_ADAPTER(8245D6D8, XAudioFactorySizeSlot)
ITC_XAUDIO_ENGINE_ADAPTER(8245F588, XAudioRegisterStats)
ITC_XAUDIO_ENGINE_ADAPTER(8245A828, XAudioSetUpProcessors)
ITC_XAUDIO_ENGINE_ADAPTER(8245B2C0, XAudioEngineSize)
// No adapter (removed 2026-10-29: only XAudioMasteringVoice::Create's plain C++ calls it in play) for XAudioMasteringVoice::Size sub_8245FF68.
// No adapter (removed 2026-10-29: only XAudioMasteringVoice::Init's plain C++ calls it in play) for AudioRenderDriver::AudioRenderDriver sub_82465938.
// No adapter (removed 2026-10-29: only XAudioMasteringVoice::Create's plain C++ calls it in play) for XAudioMasteringVoice::Init sub_8245FCA0.
ITC_XAUDIO_ENGINE_ADAPTER(824600F0, XAudioMasteringCreate)
ITC_XAUDIO_ENGINE_ADAPTER(8245FE20, XAudioMasteringStart)
ITC_XAUDIO_ENGINE_ADAPTER(8245FDC8, XAudioMasteringPass)
ITC_XAUDIO_ENGINE_ADAPTER(8245FB80, XAudioMasteringProcess)
// No adapter (removed 2026-10-29: only XAudioEngine::SwapStageLists' plain C++ calls it in play) for XAudioStage::Swap sub_8245B208.
ITC_XAUDIO_ENGINE_ADAPTER(8245B720, XAudioSwapStageLists)
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
