// The archive streaming natively: adapters in front of src/archive/archive.cpp
// (docs/functions/archive.md). What waits on, signals or reads the count of
// the archive's four semaphores (Archive::Load, the queueing, WaitIdle,
// QueueLength, CancelQueued, the picker, the loader thread) stays recompiled.

#include "guest.h"

#include "hooks/archive.h"

#include "archive/archive.h"
#include "base/flags.h"
#include "hooks/c_runtime.h"
#include "hooks/guest_call.h"

ITC_FLAG_BOOL(native_archive, true, "ITC",
              "Run the archive's functions (docs/functions/archive.md: Archive::Toc, "
              "sub_8209D868; Archive::AssetSize, sub_820A0678; List::Append and Remove, "
              "sub_82294BA8 and sub_82294C00; the slot lookup, sub_8209D818; "
              "Archive::FindLooseFile, sub_8209D8D8; the queue's and the slots' static "
              "set-ups, sub_825D9160 and sub_825D91B0, with List::List, sub_82294B80, and "
              "ListNode::ListNode, sub_82294B68; the request finish, sub_8209D688; "
              "LoaderFunctor::Invoke, sub_8209DD80) natively instead of through "
              "the translated code (same results; checked by --run_harnesses=archive)");

#define ITC_ARCHIVE_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                \
  if (!ITC_FLAG(native_archive)) {    \
    __imp__sub_##address(ctx, base);  \
    return;                           \
  }

// r3 the answer (zero-extended: the originals' lwz).
#define ITC_ARCHIVE_WORD(address, expr)  \
  extern "C" REX_FUNC(sub_##address) {  \
    ITC_ARCHIVE_FALLBACK(address)       \
    ctx.r3.u64 = uint32_t(expr);        \
  }

// Archive::Toc: r3 = an asset id. A leaf: only r3 (and r11, the global's
// high half, read by no caller) changes.
ITC_ARCHIVE_WORD(8209D868, archive::Toc(base, ctx.r3.u32))
// Archive::AssetSize: r3 = an asset id. Its r31 is restored and r11 read by
// no caller.
ITC_ARCHIVE_WORD(820A0678, archive::AssetSize(base, ctx.r3.u32))
// List::Append, List::Remove: r3 = the list, r4 = a node; r3 1 or 0. The
// registers they use besides (r8 to r11) are read by no caller.
ITC_ARCHIVE_WORD(82294BA8, archive::ListAppend(base, ctx.r3.u32, ctx.r4.u32))
ITC_ARCHIVE_WORD(82294C00, archive::ListRemove(base, ctx.r3.u32, ctx.r4.u32))
// The slot lookup: r3 = a serial (-1 for a free slot); r3 the slot,
// sign-extended (the original adds to the table's address from lis), or 0.
extern "C" REX_FUNC(sub_8209D818) {
  ITC_ARCHIVE_FALLBACK(8209D818)
  ctx.r3.s64 = int32_t(archive::FindSlot(base, ctx.r3.u32));
}
// Archive::FindLooseFile: r3 = an asset id; r3 its path or 0.
ITC_ARCHIVE_WORD(8209D8D8, archive::FindLooseFile(base, ctx.r3.u32))

// The static set-ups: no arguments; r3 = atexit's. The original runs when
// the native atexit can't (c_runtime's flags), as SignInWatch::StaticInit's.
#define ITC_ARCHIVE_STATIC_INIT(address, function)                 \
  extern "C" REX_FUNC(sub_##address) {                            \
    REX_FUNC_PROLOGUE();                                          \
    if (!ITC_FLAG(native_archive) || !hooks::NativeAtexit()) {    \
      __imp__sub_##address(ctx, base);                            \
      return;                                                     \
    }                                                             \
    hooks::GuestAtexit guest{ctx, base};                          \
    ctx.r3.s64 = archive::function(base, guest.Call());           \
  }

ITC_ARCHIVE_STATIC_INIT(825D9160, QueueStaticInit)
ITC_ARCHIVE_STATIC_INIT(825D91B0, SlotsStaticInit)

namespace {

// The request finish's calls, from inside a frame as the original's (96
// bytes), with its return addresses.
struct FinishGuestCalls {
  PPCContext& ctx;
  uint8_t* base;

  static void Callback(void* self, uint32_t function, uint32_t serial, uint32_t data,
                       uint32_t size, uint32_t context) {
    auto& c = *static_cast<FinishGuestCalls*>(self);
    PPCContext& ctx = c.ctx;
    uint8_t* base = c.base;
    guest_call::InFrame(ctx, base, 96, 0x8209D6F8, [&](uint32_t) {
      ctx.r3.u64 = serial;
      ctx.r4.u64 = data;
      ctx.r5.u64 = size;
      ctx.r6.u64 = context;
      ctx.r11.u64 = function;
      ctx.ctr.u64 = function;
      REX_CALL_INDIRECT_FUNC(function);
    });
  }
  static void GuestFree(void* self, uint32_t address, uint32_t size, uint32_t type) {
    auto& c = *static_cast<FinishGuestCalls*>(self);
    PPCContext& ctx = c.ctx;
    guest_call::InFrame(ctx, c.base, 96, 0x8209D71C, [&](uint32_t) {
      ctx.r3.u64 = address;
      ctx.r4.u64 = size;
      ctx.r5.u64 = type;
      __imp__sub_82295330(ctx, c.base);
    });
  }
  archive::FinishCalls Calls() { return {&Callback, &GuestFree, this}; }
};

}  // namespace

// The request finish: r3 = the request. Its r31 is restored; r3 and the
// other volatile registers are whatever its calls leave (read by no
// caller).
void hooks::ArchiveFinishRequest(PPCContext& ctx, uint8_t* base) {
  FinishGuestCalls calls{ctx, base};
  if (auto r3 = archive::FinishRequest(base, ctx.r3.u32, calls.Calls())) {
    ctx.r3.u64 = *r3;
  }
}

// LoaderFunctor::Invoke: r3 = the functor, r4 = the argument: a tail call of
// the functor's +4 with r4 as r3 (r11 and ctr the target, as the original's
// mtctr; lr the caller's).
void hooks::ArchiveInvokeFunctor(PPCContext& ctx, uint8_t* base) {
  const uint32_t target = archive::LoaderFunctorTarget(base, ctx.r3.u32);
  ctx.r3.u64 = ctx.r4.u64;
  ctx.r11.u64 = target;
  ctx.ctr.u64 = target;
  REX_CALL_INDIRECT_FUNC(target);
}

extern "C" REX_FUNC(sub_8209D688) {
  ITC_ARCHIVE_FALLBACK(8209D688)
  hooks::ArchiveFinishRequest(ctx, base);
}

extern "C" REX_FUNC(sub_8209DD80) {
  ITC_ARCHIVE_FALLBACK(8209DD80)
  hooks::ArchiveInvokeFunctor(ctx, base);
}
