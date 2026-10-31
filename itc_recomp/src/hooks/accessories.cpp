// The interior accessories natively: adapters in front of src/accessories/
// (docs/functions/accessories.md), with the register steps the harness
// shares (hooks/accessories.h). Their callers (the scene's constructor, the
// interior's loads, the archive's callbacks) are recompiled, so the adapters
// stay.

#include "hooks/accessories.h"

#include "accessories/accessories.h"
#include "base/flags.h"
#include "hooks/archive.h"
#include "hooks/heap.h"
#include "memory/heap.h"

ITC_FLAG_BOOL(native_accessories, true, "ITC",
              "Run the interior accessories (docs/functions/accessories.md: the set at "
              "[0x826C5510] sub_820F1038, sub_820F0F48, sub_820F0E60, sub_820F07B8, "
              "sub_820F0F00; the file class sub_820F0C50, sub_820F0BF0, sub_820F0C90, "
              "sub_820F0C20, sub_820F08D0) natively instead of through the translated code "
              "(the same words; checked by --run_harnesses=accessories)");

namespace hooks {

bool AccessoriesNativeLoads() {
  return NativeHeapActive() && NativeArchiveQueue();
}

// --- The file ---

void AccessoryFileConstruct(PPCContext& ctx, uint8_t* base) {
  accessories::FileConstruct(base, ctx.r3.u32);
}

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

// --- The set ---

void AccessoriesBaseConstruct(PPCContext& ctx, uint8_t* base) {
  accessories::SetBaseConstruct(base, ctx.r3.u32);
}

void AccessoriesConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();  // as the original's lfs
  ctx.r3.u64 = accessories::SetConstruct(base, ctx.r3.u32);
}

void AccessoriesNew(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive()) {
    __imp__sub_820F0F48(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::SetNew(base);
}

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

ITC_ACCESSORIES(820F0C50, AccessoryFileConstruct)
ITC_ACCESSORIES(820F0BF0, AccessoryFileSetData)
ITC_ACCESSORIES(820F0C90, AccessoryFileLoad)
ITC_ACCESSORIES(820F0C20, AccessoryFileOnArchiveDone)
ITC_ACCESSORIES(820F08D0, AccessoryFileFreeData)
ITC_ACCESSORIES(820F07B8, AccessoriesBaseConstruct)
ITC_ACCESSORIES(820F0E60, AccessoriesConstruct)
ITC_ACCESSORIES(820F0F48, AccessoriesNew)
ITC_ACCESSORIES(820F1038, AccessoriesCreate)
ITC_ACCESSORIES(820F0F00, AccessoriesSetScale)
