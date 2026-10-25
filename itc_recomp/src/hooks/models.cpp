// The model class natively: adapters in front of src/models/
// (docs/functions/models.md). The functions that allocate or free run
// natively only while --native_heap routes the game's heap to the allocation
// layer, and hand the paths left to the original back to it from the start,
// nothing having been done. Functions reached only from native callers have
// no adapters (the flag switches their callers): PendingFrees::IncSize
// (sub_8258E2F8), from PendingFrees::Insert; PendingFrees::Insert
// (sub_8258E398), from Memory::FreeLater and FreePhysicalLater.

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "models/memory_blocks.h"
#include "models/xpr.h"

ITC_FLAG_BOOL(native_models, true, "ITC",
              "Run the model class's pieces (docs/functions/models.md: the memory blocks' "
              "lock, the critical section at 0x8296BD38, as a host mutex for every user, "
              "through ScopedLock sub_8258E580 and sub_8258E5B8 for that critical section "
              "(chosen at start-up, other critical sections the game's); with --native_heap "
              "Memory::FreeLater sub_8258E410, FreePhysicalLater sub_8258E498, the pending "
              "frees' Insert sub_8258E398 and IncSize sub_8258E2F8 (no adapters: only the "
              "put-off frees call Insert, and only Insert IncSize); XPR::XPR sub_82596E68, and with --native_heap XPR::Release "
              "sub_825970A8 and XPR::~XPR sub_82597148) natively instead of through the "
              "translated code (the same writes; "
              "checked by --run_harnesses=models)");

bool hooks::ModelsHostLock() {
  static const bool host = ITC_FLAG(native_models);
  return host;
}

namespace {

// The functions that allocate or free: only over the allocation layer, and
// with the host lock.
bool NativeLifetime() {
  return ITC_FLAG(native_models) && hooks::NativeHeapActive() && hooks::ModelsHostLock();
}

}  // namespace

// --- The memory blocks' lock ---
//
// For the critical section at 0x8296BD38 only, whatever --native_models says
// now (hooks::ModelsHostLock is fixed for the run): every user of it takes
// the host mutex, the recompiled ones here. Other critical sections go to
// the originals.

// ScopedLock::ScopedLock: r3 = the lock word, r4 = the critical section; r3
// left as it came.
extern "C" REX_FUNC(sub_8258E580) {
  REX_FUNC_PROLOGUE();
  if (ctx.r4.u32 == models::kMemoryBlocksLock && hooks::ModelsHostLock()) {
    models::ScopedLockEnter(base, ctx.r3.u32, ctx.r4.u32);
    return;
  }
  __imp__sub_8258E580(ctx, base);
}

// ScopedLock::~ScopedLock: r3 = the lock word; r3 = the critical section (the
// original's load before its LeaveCriticalSection).
extern "C" REX_FUNC(sub_8258E5B8) {
  REX_FUNC_PROLOGUE();
  if (hooks::ModelsHostLock() &&
      guest::LoadU32(base, ctx.r3.u32) == models::kMemoryBlocksLock) {
    ctx.r3.u64 = models::ScopedLockLeave(base, ctx.r3.u32);
    return;
  }
  __imp__sub_8258E5B8(ctx, base);
}

// --- The pending frees ---

// Memory::FreeLater: r3 = the block; r3 = the critical section, as
// ScopedLock::~ScopedLock leaves it.
extern "C" REX_FUNC(sub_8258E410) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime() && models::FreeLater(base, ctx.r3.u32)) {
    ctx.r3.u64 = models::kMemoryBlocksLock;
    return;
  }
  __imp__sub_8258E410(ctx, base);
}

// Memory::FreePhysicalLater: r3 = the block, r4 = the attributes; r3 as
// FreeLater's.
extern "C" REX_FUNC(sub_8258E498) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime() && models::FreePhysicalLater(base, ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = models::kMemoryBlocksLock;
    return;
  }
  __imp__sub_8258E498(ctx, base);
}

// --- The XPR object ---

// XPR::XPR: r3 = the object (left).
extern "C" REX_FUNC(sub_82596E68) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_82596E68(ctx, base);
    return;
  }
  models::XprConstruct(base, ctx.r3.u32);
}

// XPR::Release and XPR::~XPR: r3 = the object; r3 as the original leaves it.
#define ITC_MODELS_LIFETIME(address, function)                       \
  extern "C" REX_FUNC(sub_##address) {                               \
    REX_FUNC_PROLOGUE();                                             \
    if (NativeLifetime()) {                                          \
      if (const auto r3 = models::function(base, ctx.r3.u32)) {      \
        ctx.r3.u64 = *r3;                                            \
        return;                                                      \
      }                                                              \
    }                                                                \
    __imp__sub_##address(ctx, base);                                 \
  }

ITC_MODELS_LIFETIME(825970A8, XprRelease)
ITC_MODELS_LIFETIME(82597148, XprDestruct)
