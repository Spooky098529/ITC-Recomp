// The archive streaming natively: adapters in front of src/archive/archive.cpp
// (docs/functions/archive.md). The archive's four semaphores are host ones
// (src/archive/semaphores.h), routed here for the recompiled code still
// taking them (Semaphore::Wait and Signal, sub_82592670 and sub_825926E8:
// the loose read, the harness's recompiled sides); the game's Semaphore class
// stays for every other semaphore.

#include "guest.h"

#include "hooks/archive.h"

#include "archive/archive.h"
#include "archive/semaphores.h"
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
              "LoaderFunctor::Invoke, sub_8209DD80; Ucl::Unpack, sub_82294610, with its "
              "block's NRV2B unpacking, sub_822947F8; Archive::Load, sub_8209DE48, with the "
              "queueing, sub_8209DC18; Archive::WaitIdle, QueueLength and CancelQueued, "
              "sub_8209D730, sub_8209D7B0 and sub_8209DB90; the cancel of a request, "
              "sub_8209DB20; the loader's next request, sub_8209DD98, with the picker, "
              "sub_8209D950, and the set-up, sub_8209D490; the loader thread's and "
              "Archive::Init's waits and signals) natively instead of through the "
              "translated code, the archive's four semaphores (0x826BAAE0, 0x826BAAB8, "
              "0x826BAA68, 0x826BAA90) as host semaphores for every user (read at start-up "
              "only; Semaphore::Wait and Signal, sub_82592670 and sub_825926E8, route them; "
              "same results; checked by --run_harnesses=archive)");

bool hooks::ArchiveHostSemaphores() {
  static const bool on = ITC_FLAG(native_archive);
  return on;
}

archive::HostSemaphore* hooks::ArchiveSemaphore(uint32_t address) {
  if (archive::Semaphores* test = archive::TestSemaphores()) {
    return test->At(address);
  }
  return ArchiveHostSemaphores() ? archive::LiveSemaphores().At(address) : nullptr;
}

bool hooks::NativeArchiveQueue() {
  return ITC_FLAG(native_archive) && ArchiveHostSemaphores();
}

// Semaphore::Wait and Signal: r3 = the semaphore. The archive's four are
// taken as host semaphores (r3 left as it is: read by no caller, the
// originals leave their last call's); every other one is the game's.
extern "C" REX_FUNC(sub_82592670) {
  REX_FUNC_PROLOGUE();
  if (archive::HostSemaphore* s = hooks::ArchiveSemaphore(ctx.r3.u32)) {
    s->Wait(base);
    return;
  }
  __imp__sub_82592670(ctx, base);
}

extern "C" REX_FUNC(sub_825926E8) {
  REX_FUNC_PROLOGUE();
  if (archive::HostSemaphore* s = hooks::ArchiveSemaphore(ctx.r3.u32)) {
    s->Signal(base);
    return;
  }
  __imp__sub_825926E8(ctx, base);
}

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

// Ucl::Unpack: r3 = the destination, r4 = the stream, r5 = the expected
// size; r3 1 or 0. Its r25 to r31 are restored; the words it leaves in its
// own frame (its byte swaps' and the block's output size) are read by
// nothing. memcpy, where the native copy can't, through `ctx` in a frame as
// the original's (160 bytes).
void hooks::ArchiveUclUnpack(PPCContext& ctx, uint8_t* base) {
  struct Guest {
    PPCContext& ctx;
    uint8_t* base;
    static void Memcpy(void* self, uint32_t destination, uint32_t source, uint32_t size) {
      auto& g = *static_cast<Guest*>(self);
      PPCContext& ctx = g.ctx;
      guest_call::InFrame(ctx, g.base, 160, 0x822946E4, [&](uint32_t) {
        ctx.r3.u64 = destination;
        ctx.r4.u64 = source;
        ctx.r5.u64 = size;
        sub_825A7410(ctx, g.base);
      });
    }
  } guest{ctx, base};
  ctx.r3.u64 =
      archive::UclUnpack(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, {&Guest::Memcpy, &guest});
}

extern "C" REX_FUNC(sub_82294610) {
  ITC_ARCHIVE_FALLBACK(82294610)
  hooks::ArchiveUclUnpack(ctx, base);
}

// --- The queue under the four semaphores ---

#define ITC_ARCHIVE_QUEUE_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                      \
  if (!hooks::NativeArchiveQueue()) {       \
    __imp__sub_##address(ctx, base);        \
    return;                                 \
  }

namespace {

// r3 the serial (lwz's, zero-extended) or -1 (li's).
void SetSerial(PPCContext& ctx, int64_t serial) {
  if (serial < 0) {
    ctx.r3.s64 = -1;
  } else {
    ctx.r3.u64 = uint64_t(serial);
  }
}

// The set-up's VirtualAlloc where the native layer is off, from inside a
// frame as the original's (112 bytes).
struct SetUpGuestCalls {
  PPCContext& ctx;
  uint8_t* base;
  static uint32_t Alloc(void* self, uint32_t size) {
    auto& c = *static_cast<SetUpGuestCalls*>(self);
    PPCContext& ctx = c.ctx;
    guest_call::InFrame(ctx, c.base, 112, 0x8209D604, [&](uint32_t) {
      ctx.r3.u64 = 0;
      ctx.r4.u64 = size;
      ctx.r5.u64 = 4096;
      ctx.r6.u64 = 4;
      sub_82295398(ctx, c.base);
    });
    return ctx.r3.u32;
  }
  archive::SetUpCalls Calls() { return {&Alloc, this}; }
};

}  // namespace

// Archive::Load: r3 = an asset id, r4 = the destination, r5 = a callback, r6
// = its context, r7, r8 = a size; r3 the serial or -1. Its r26 to r31 are
// restored; the request it leaves in its own frame is read by nothing.
extern "C" REX_FUNC(sub_8209DE48) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209DE48)
  SetSerial(ctx, archive::Queue(base, archive::BuildLoad(base, ctx.r3.u32, ctx.r4.u32,
                                                         ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                                         ctx.r8.u32)));
}

// The queueing: r3 = the 40-byte request; r3 the serial or -1.
extern "C" REX_FUNC(sub_8209DC18) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209DC18)
  archive::LoadRequest request;
  for (uint32_t i = 0; i < 40; i++) {
    request[i] = guest::LoadU8(base, ctx.r3.u32 + i);
  }
  SetSerial(ctx, archive::Queue(base, request));
}

// Archive::WaitIdle, Archive::CancelQueued (r3 = whether to wait): r3 read by
// no caller.
extern "C" REX_FUNC(sub_8209D730) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209D730)
  archive::WaitIdle(base);
}

extern "C" REX_FUNC(sub_8209DB90) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209DB90)
  archive::CancelQueued(base, ctx.r3.s32 != 0);
}

// Archive::QueueLength: r3 the count (lwz's).
extern "C" REX_FUNC(sub_8209D7B0) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209D7B0)
  ctx.r3.u64 = archive::QueueLength(base);
}

// The cancel of a request: r3 = its serial; r3 1 or 0.
extern "C" REX_FUNC(sub_8209DB20) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209DB20)
  ctx.r3.u64 = archive::Cancel(base, ctx.r3.u32);
}

// The picker: r3 = the current request, r4 and r5 the finished ones; r3 the
// next (lwz's).
extern "C" REX_FUNC(sub_8209D950) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209D950)
  ctx.r3.u64 = archive::PickNext(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// The set-up: r3 = the request; r3 read by no caller (the next request sets
// its own).
void hooks::ArchiveSetUp(PPCContext& ctx, uint8_t* base) {
  SetUpGuestCalls calls{ctx, base};
  archive::SetUp(base, ctx.r3.u32, calls.Calls());
}

extern "C" REX_FUNC(sub_8209D490) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209D490)
  hooks::ArchiveSetUp(ctx, base);
}

// The loader's next request: r3 = the current request or 0; r3 the next. Its
// calls from inside a frame as the original's (112 bytes), the finish's and
// the set-up's own below it.
void hooks::ArchiveNextRequest(PPCContext& ctx, uint8_t* base) {
  const uint32_t current = ctx.r3.u32;
  uint32_t next = 0;
  guest_call::InFrame(ctx, base, 112, uint32_t(ctx.lr), [&](uint32_t) {
    FinishGuestCalls finish{ctx, base};
    SetUpGuestCalls set_up{ctx, base};
    next = archive::NextRequest(base, current, {finish.Calls(), set_up.Calls()});
  });
  ctx.r3.u64 = next;
}

extern "C" REX_FUNC(sub_8209DD98) {
  ITC_ARCHIVE_QUEUE_FALLBACK(8209DD98)
  hooks::ArchiveNextRequest(ctx, base);
}
