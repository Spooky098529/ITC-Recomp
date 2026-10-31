// The interior accessories natively: adapters in front of src/accessories/
// (docs/functions/accessories.md), with the register steps the harness
// shares (hooks/accessories.h). The adapters left are those recompiled code
// (the scene's constructor and teardown, the interior's loads, the
// archive's callbacks) still calls; the functions only native code calls
// have none (their register steps are the harness's).

#include "hooks/accessories.h"

#include "accessories/accessories.h"
#include "base/flags.h"
#include "hooks/archive.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "memory/heap.h"
#include "native/buffers.h"

#include <functional>

ITC_FLAG_BOOL(native_accessories, true, "ITC",
              "Run the interior accessories (docs/functions/accessories.md: the set at "
              "[0x826C5510] sub_820F1038, sub_820F0F48, sub_820F0E60, sub_820F07B8, "
              "sub_820F0F00; the file class sub_820F0C50, sub_820F0BF0, sub_820F0C90, "
              "sub_820F0C20, sub_820F08D0, sub_820F0D30; Accessories::Free and Destroy "
              "sub_820F0F90, sub_820F1078, the set's destructor through its vtable; the entry "
              "and the accessory's construction sub_820F0920, sub_820EF258, sub_820F03F0; their "
              "destructors sub_820F0988, sub_820EF310, sub_820EFFD8, sub_820F0DB8, "
              "Accessories::ReleaseAll sub_820F0AD8; the skeleton's set-up sub_820F0070, "
              "sub_820EFB30) natively instead of through the translated code "
              "(the same words; checked by --run_harnesses=accessories)");

namespace hooks {

namespace {

// A guest call the plain C++ hands back: `function` through the function
// table (its override when it has one) with r3, r4 set, from the frame at
// `r1` with the link register `lr` (the original's return address); r1 and
// lr put back after.
void GuestCall(PPCContext& ctx, uint8_t* base, uint32_t function, uint32_t r1, uint32_t lr,
               uint64_t r3, uint64_t r4) {
  const uint64_t stack = ctx.r1.u64, link = ctx.lr;
  ctx.r1.u64 = r1;
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.lr = lr;
  ctx.ctr.u64 = function;
  REX_CALL_INDIRECT_FUNC(function);
  ctx.r1.u64 = stack;
  ctx.lr = link;
}

}  // namespace

std::function<void(uint32_t, uint32_t)> AccessoriesSetDestructor(PPCContext& ctx,
                                                                 uint8_t* base) {
  return [&ctx, base](uint32_t set, uint32_t frame) {
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, set));
    GuestCall(ctx, base, function, frame, 0x820F0FC4, set, 0);
  };
}

bool AccessoriesSetFreeable(uint8_t* base, uint32_t slot) {
  const uint32_t set = guest::LoadU32(base, slot);
  return set == 0 || memory::IsPages(set);
}

bool AccessoriesNativeLoads() {
  return NativeHeapActive() && NativeArchiveQueue();
}

// --- The file ---


void AccessoryFileSetData(PPCContext& ctx, uint8_t* base) {
  accessories::FileSetData(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void AccessoryFileLoad(PPCContext& ctx, uint8_t* base) {
  if (!AccessoriesNativeLoads()) {
    __imp__sub_820F0C90(ctx, base);
    return;
  }
  const int64_t serial = accessories::FileLoad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r6.u32, ctx.r7.u32, NativeArchiveLoad(base));
  SetArchiveSerial(ctx, serial);
}

// The tail call to the file's callback (r3 to r5 as given, r6 its context,
// lr the caller's): the guest function through the function table.
void AccessoryFileOnArchiveDone(PPCContext& ctx, uint8_t* base) {
  const accessories::Callback callback =
      accessories::FileOnArchiveDone(base, ctx.r3.u32, ctx.r6.u32);
  if (callback.function == 0) {
    return;
  }
  ctx.r6.u64 = callback.context;
  ctx.ctr.u64 = callback.function;
  REX_CALL_INDIRECT_FUNC(callback.function);
}

void AccessoryFileFreeData(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive()) {
    __imp__sub_820F08D0(ctx, base);
    return;
  }
  const auto r3 = accessories::FileFreeData(base, ctx.r3.u32);
  if (!r3) {
    __imp__sub_820F08D0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void AccessoryFileReset(PPCContext& ctx, uint8_t* base) {
  if (!AccessoriesNativeLoads() || !accessories::FileResetNative(base, ctx.r3.u32)) {
    __imp__sub_820F0D30(ctx, base);
    return;
  }
  if (const auto r3 = accessories::FileReset(base, ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
  }
}

// --- The set ---




void AccessoriesCreate(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive()) {
    __imp__sub_820F1038(ctx, base);
    return;
  }
  if (const auto r3 = accessories::SetCreate(base)) {
    ctx.fpscr.disableFlushMode();
    ctx.r3.u64 = *r3;
  }
}

void AccessoriesSetScale(PPCContext& ctx, uint8_t* base) {
  accessories::SetScale(base, ctx.r3.u32, ctx.r4.u32);
}


void AccessoriesDestroy(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive() || !AccessoriesSetFreeable(base, accessories::kSet)) {
    __imp__sub_820F1078(ctx, base);
    return;
  }
  ctx.r3.u64 = accessories::SetDestroy(base, ctx.r1.u32, AccessoriesSetDestructor(ctx, base));
}

// --- The entry and the accessory ---



void AccessoryEntryNew(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive() || !ModelsNativeLifetime() || !native::Buffers_NativeMemcpy() ||
      !accessories::AccessoryConstructible(base)) {
    __imp__sub_820F0920(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::EntryNew(base, ctx.r1.u32);
}

// --- The destructors ---

void AccessoryFileDestruct(PPCContext& ctx, uint8_t* base) {
  if (!AccessoriesNativeLoads() || !accessories::FileDestructible(base, ctx.r3.u32)) {
    __imp__sub_820F0DB8(ctx, base);
    return;
  }
  ctx.r3.u64 = accessories::FileDestruct(base, ctx.r3.u32);
}

void AccessorySkeletonDestruct(PPCContext& ctx, uint8_t* base) {
  if (!AccessoriesNativeLoads() || !ModelsNativeLifetime() ||
      !accessories::SkeletonDestructible(base, ctx.r3.u32)) {
    __imp__sub_820EFFD8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();  // as SkeletonBase::~SkeletonBase's adapter
  ctx.r3.u64 = accessories::SkeletonDestruct(base, ctx.r3.u32);
}

void AccessoryDestruct(PPCContext& ctx, uint8_t* base) {
  if (!AccessoriesNativeLoads() || !ModelsNativeLifetime() ||
      !accessories::AccessoryDestructible(base, ctx.r3.u32)) {
    __imp__sub_820EF310(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::AccessoryDestruct(base, ctx.r3.u32);
}

void AccessoryEntryDestroy(PPCContext& ctx, uint8_t* base) {
  if (!AccessoriesNativeLoads() || !ModelsNativeLifetime() ||
      !accessories::EntryDestroyable(base, ctx.r3.u32)) {
    __imp__sub_820F0988(ctx, base);
    return;
  }
  if (guest::LoadU32(base, ctx.r3.u32) != 0) {
    ctx.fpscr.disableFlushMode();
  }
  ctx.r3.u64 = accessories::EntryDestroy(base, ctx.r3.u32);
}

void AccessoriesReleaseAll(PPCContext& ctx, uint8_t* base) {
  if (!AccessoriesNativeLoads() || !ModelsNativeLifetime() ||
      !accessories::SetReleasable(base, ctx.r3.u32)) {
    __imp__sub_820F0AD8(ctx, base);
    return;
  }
  if (const auto r3 = accessories::SetReleaseAll(base, ctx.r3.u32)) {
    ctx.fpscr.disableFlushMode();
    ctx.r3.u64 = *r3;
  }
}

// --- The skeleton's set-up ---

void AccessorySkeletonReset(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_820EFB30(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::SkeletonReset(base, ctx.r3.u32, ctx.r1.u32);
}

void AccessorySkeletonSetUp(PPCContext& ctx, uint8_t* base) {
  if (!ModelsNativeLifetime() || !native::Buffers_NativeMemcpy()) {
    __imp__sub_820F0070(ctx, base);
    return;
  }
  const auto r3 = accessories::SkeletonSetUp(base, ctx.r3.u32, ctx.r1.u32);
  if (!r3) {
    __imp__sub_820F0070(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *r3;
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_ACCESSORIES(address, step)   \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_accessories)) { \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

ITC_ACCESSORIES(820F0BF0, AccessoryFileSetData)
ITC_ACCESSORIES(820F0C90, AccessoryFileLoad)
ITC_ACCESSORIES(820F0C20, AccessoryFileOnArchiveDone)
ITC_ACCESSORIES(820F08D0, AccessoryFileFreeData)
ITC_ACCESSORIES(820F1038, AccessoriesCreate)
ITC_ACCESSORIES(820F0F00, AccessoriesSetScale)
ITC_ACCESSORIES(820F0D30, AccessoryFileReset)
ITC_ACCESSORIES(820F1078, AccessoriesDestroy)
ITC_ACCESSORIES(820F0920, AccessoryEntryNew)
ITC_ACCESSORIES(820F0DB8, AccessoryFileDestruct)
ITC_ACCESSORIES(820EFFD8, AccessorySkeletonDestruct)
ITC_ACCESSORIES(820EF310, AccessoryDestruct)
ITC_ACCESSORIES(820F0988, AccessoryEntryDestroy)
ITC_ACCESSORIES(820F0AD8, AccessoriesReleaseAll)
ITC_ACCESSORIES(820EFB30, AccessorySkeletonReset)
ITC_ACCESSORIES(820F0070, AccessorySkeletonSetUp)
