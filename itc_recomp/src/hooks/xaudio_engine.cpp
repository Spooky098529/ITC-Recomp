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
              "XAudio's processor threads stay recompiled) natively instead of through the "
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

}  // namespace hooks

#define ITC_XAUDIO_ENGINE_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {           \
    if (!ITC_FLAG(native_xaudio_engine)) {       \
      __imp__sub_##address(ctx, base);           \
      return;                                    \
    }                                            \
    hooks::step(ctx, base);                      \
  }

ITC_XAUDIO_ENGINE_ADAPTER(8245D9E8, XAudioFactoryTableSize)
ITC_XAUDIO_ENGINE_ADAPTER(8245DBD0, XAudioFactorySizeOf)
ITC_XAUDIO_ENGINE_ADAPTER(8245F6F0, XAudioBufferGetFormat)
ITC_XAUDIO_ENGINE_ADAPTER(8245F718, XAudioBufferGetFormatThunk)
ITC_XAUDIO_ENGINE_ADAPTER(8245F8D0, XAudioBufferInterface)
ITC_XAUDIO_ENGINE_ADAPTER(82460388, XAudioAllocatorRemaining)
ITC_XAUDIO_ENGINE_ADAPTER(82460370, XAudioInterfaceAddRef)
ITC_XAUDIO_ENGINE_ADAPTER(82461EF8, XAudioObjectInfo)
ITC_XAUDIO_ENGINE_ADAPTER(8245F678, XAudioTakeStats)
ITC_XAUDIO_ENGINE_ADAPTER(8245EF90, XAudioFree)
ITC_XAUDIO_ENGINE_ADAPTER(82460288, XAudioAllocatorAlloc)
ITC_XAUDIO_ENGINE_ADAPTER(8245AAB8, XAudioDeleteObject)
ITC_XAUDIO_ENGINE_ADAPTER(8245A940, XAudioBarrier)
ITC_XAUDIO_ENGINE_ADAPTER(8245C098, XAudioEngineConstruct)
ITC_XAUDIO_ENGINE_ADAPTER(8245DA48, XAudioFactoryConstruct)
ITC_XAUDIO_ENGINE_ADAPTER(8245DC10, XAudioFactoryCreateObject)
ITC_XAUDIO_ENGINE_ADAPTER(8245D650, XAudioFactoryCreateSlot)
ITC_XAUDIO_ENGINE_ADAPTER(8245D6D8, XAudioFactorySizeSlot)
ITC_XAUDIO_ENGINE_ADAPTER(8245F588, XAudioRegisterStats)
ITC_XAUDIO_ENGINE_ADAPTER(8245A828, XAudioSetUpProcessors)
ITC_XAUDIO_ENGINE_ADAPTER(8245B2C0, XAudioEngineSize)
