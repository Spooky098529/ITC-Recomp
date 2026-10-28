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
#include "hooks/heap.h"

#include "base/log.h"
#include "file_layer/file_layer.h"
#include "std_string/std_string.h"


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
              "sub_8209D950, and the set-up, sub_8209D490; the loose read, sub_8209DFA8, with "
              "--native_std_string and --native_file_layer; the loader thread's and "
              "Archive::Init's waits and signals) natively instead of through the "
              "translated code, the archive's four semaphores (0x826BAAE0, 0x826BAAB8, "
              "0x826BAA68, 0x826BAA90) as host semaphores for every user (read at start-up "
              "only; Semaphore::Wait and Signal, sub_82592670 and sub_825926E8, route them; "
              "same results; checked by --run_harnesses=archive)");

ITC_FLAG_DECLARE(bool, native_std_string);
ITC_FLAG_DECLARE(bool, native_file_layer);

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

// The set-up: r3 = the request; r3 read by no caller (the next request sets
// its own).
void hooks::ArchiveSetUp(PPCContext& ctx, uint8_t* base) {
  SetUpGuestCalls calls{ctx, base};
  archive::SetUp(base, ctx.r3.u32, calls.Calls());
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

// The loader's loose read: r3 = the request (+20 its path, +64 where to
// seek, +68 the size, +76 the buffer). Its own frame (240 bytes) as the
// original's: the file object at +96, the path's strings at +112, +144 and
// +176, the open's byte at +80 and the seek's words at +84 and +88. The file
// object, the strings and the request-done signal natively (each string step
// on the original where its plain C++ declines, which leaves nothing done);
// the open by name, File::Seek, Read and Close (B4's file class, through
// ctx) and the read error's report (recompiled) called from the frame with
// the original's return addresses. r3 the file object (as the original's
// last call leaves it). The original runs unless --native_archive,
// --native_std_string over the allocation layer and --native_file_layer are
// all on.
extern "C" REX_FUNC(sub_8209DFA8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_archive) || !ITC_FLAG(native_std_string) || !hooks::NativeHeapActive() ||
      !ITC_FLAG(native_file_layer)) {
    __imp__sub_8209DFA8(ctx, base);
    return;
  }
  ITC_NATIVE_RUNNING("sub_8209DFA8 Archive::ReadLoose");
  const uint32_t request = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 240;
  const uint32_t file = frame + 96, path = frame + 112, name = frame + 144, fixed = frame + 176;
  auto call = [&](uint32_t return_address, auto&& body) {
    guest_call::InFrame(ctx, base, 240, return_address, [&](uint32_t) { body(); });
  };
  auto from_text = [&](uint32_t string, uint32_t text, uint32_t allocator,
                       uint32_t return_address) {
    if (!std_string::ConstructFromTextWithAllocator(base, string, text)) {
      call(return_address, [&] {
        ctx.r3.u64 = string;
        ctx.r4.u64 = text;
        ctx.r5.u64 = allocator;
        __imp__sub_82593248(ctx, base);
      });
    }
  };
  auto assign = [&](uint32_t source, uint32_t return_address) {
    if (!std_string::AssignFrom(base, path, source, 0, 0xFFFFFFFF)) {
      call(return_address, [&] {
        ctx.r3.u64 = path;
        ctx.r4.u64 = source;
        ctx.r5.u64 = 0;
        ctx.r6.s64 = -1;
        __imp__sub_82091720(ctx, base);
      });
    }
  };
  auto release = [&](uint32_t string, uint32_t return_address) {
    if (!std_string::Tidy(base, string, true, 0)) {
      call(return_address, [&] {
        ctx.r3.u64 = string;
        ctx.r4.u64 = 1;
        ctx.r5.u64 = 0;
        __imp__sub_82091328(ctx, base);
      });
    }
  };
  file_layer::GameFileConstruct(base, file);
  std_string::Construct(base, path);
  from_text(name, guest::LoadU32(base, request + 20), frame + 82, 0x8209DFE4);
  assign(name, 0x8209DFFC);
  release(name, 0x8209E00C);
  // The wildcard replace of "/" (0x82001DE8) by "\\" (0x82001DEC): the text.
  uint32_t text;
  if (const auto replaced = std_string::ReplaceAll(base, path, 0x82001DE8, frame - 144)) {
    text = *replaced;
  } else {
    call(0x8209E028, [&] {
      ctx.r3.u64 = path;
      ctx.r4.u64 = 0x82001DE8;
      ctx.r5.u64 = 0x82001DEC;
      ctx.r6.u64 = 0;
      __imp__sub_82593088(ctx, base);
    });
    text = ctx.r3.u32;
  }
  from_text(fixed, text, frame + 83, 0x8209E038);
  assign(fixed, 0x8209E04C);
  release(fixed, 0x8209E05C);
  guest::StoreU8(base, frame + 80, 1);
  call(0x8209E084, [&] {
    ctx.r3.u64 = file;
    ctx.r4.u64 = std_string::Text(base, path);
    ctx.r5.u64 = frame + 80;
    sub_820A6AB8(ctx, base);
  });
  if ((ctx.r3.u32 & 0xFF) != 0) {
    guest::StoreU32(base, frame + 84, guest::LoadU32(base, request + 64));
    guest::StoreU32(base, frame + 88, 0);
    call(0x8209E0AC, [&] {
      ctx.r3.u64 = file;
      ctx.r4.u64 = frame + 84;
      ctx.r5.u64 = frame + 88;
      sub_8258C768(ctx, base);
    });
    call(0x8209E0BC, [&] {
      ctx.r3.u64 = file;
      ctx.r4.u64 = guest::LoadU32(base, request + 76);
      ctx.r5.u64 = request + 68;
      sub_8258C820(ctx, base);
    });
    if (ctx.r3.u32 == 0) {
      call(0x8209E0C8, [&] { sub_8209AC78(ctx, base); });
    }
    call(0x8209E0D0, [&] {
      ctx.r3.u64 = file;
      sub_8258C718(ctx, base);
    });
  } else {
    call(0x8209E0D8, [&] { sub_8209AC78(ctx, base); });
  }
  if (archive::HostSemaphore* done = hooks::ArchiveSemaphore(archive::kRequestDone)) {
    done->Signal(base);
  } else {
    call(0x8209E0E4, [&] {
      ctx.r3.u64 = archive::kRequestDone;
      sub_825926E8(ctx, base);
    });
  }
  release(path, 0x8209E0F4);
  file_layer::GameFileDestruct(base, file);
  ctx.r3.u64 = file;
}
