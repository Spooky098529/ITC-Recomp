// The archive streaming natively: adapters in front of src/archive/archive.cpp
// (docs/functions/archive.md). What waits on, signals or reads the count of
// the archive's four semaphores (Archive::Load, the queueing, WaitIdle,
// QueueLength, CancelQueued, the picker, the loader thread) stays recompiled.

#include "guest.h"

#include "archive/archive.h"
#include "base/flags.h"

ITC_FLAG_BOOL(native_archive, true, "ITC",
              "Run the archive's functions (docs/functions/archive.md: Archive::Toc, "
              "sub_8209D868; Archive::AssetSize, sub_820A0678) natively instead of through "
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
