// The app framework natively: adapters in front of src/framework/
// (docs/functions/framework.md). The getters and the thunks are reached
// through the framework's vtables (and the folded getters also from other
// classes' code), so their adapters stay while recompiled code calls them.
// Word64 (sub_82090778), Word44 (sub_824892F8) and DebugFont::SetPosition
// (sub_8258BE08) have none: in play only the thunks' and
// DebugText::SetPosition's plain C++ call them.

#include "guest.h"

#include "base/flags.h"
#include "framework/framework.h"
#include "framework/run_loop.h"
#include "hooks/heap.h"

#include <optional>

ITC_FLAG_BOOL(native_framework, true, "ITC",
              "Run the app framework (docs/functions/framework.md): the framework's flags "
              "sub_82090760 and sub_82090768, the folded getters sub_82090770, sub_82090788 and "
              "sub_8248F030 (and without adapters, as only the thunks call them, sub_82090778 and "
              "sub_824892F8), the interfaces' thunks sub_82090828, "
              "sub_82090830, sub_82090838, sub_82090860, sub_82090868, sub_820908E0, sub_820908E8 "
              "and sub_82090908, DebugText::SetPosition sub_82091050 with DebugFont::SetPosition "
              "sub_8258BE08 (no adapter: only its plain C++ calls it); the run loop's handler maps "
              "(the iterator's increment sub_82099B18, and with --native_heap "
              "HandlerMap::HandlerMap sub_82099FB0 and Insert sub_82093740; without adapters, as "
              "only those call them: the decrement sub_82091AE8, RotateLeft sub_82092B10, "
              "RotateRight sub_82092A00, AllocateNodes sub_82099C80, NewHead sub_82099E88, NewNode "
              "sub_82099E20 and InsertNode sub_82093400) and request vector (Clear sub_8209A080, "
              "with Erase sub_82092EF8, no adapter; with --native_heap InsertN sub_8209A0E8, "
              "Insert sub_8209A600 and PushBack sub_8209A930), RunLoop::RunLoop sub_8209A9F0 and "
              "ApplyRequest sub_8209A458 (with --native_heap)) natively instead of "
              "through the translated code (the same results; checked by "
              "--run_harnesses=framework)");

namespace fw = framework;

#define ITC_FRAMEWORK_FALLBACK(address)  \
  REX_FUNC_PROLOGUE();                   \
  if (!ITC_FLAG(native_framework)) {     \
    __imp__sub_##address(ctx, base);     \
    return;                              \
  }

// A function of r3 answering a word in r3.
#define ITC_FRAMEWORK_WORD(address, function) \
  extern "C" REX_FUNC(sub_##address) {        \
    ITC_FRAMEWORK_FALLBACK(address)           \
    ctx.r3.u64 = fw::function(base, ctx.r3.u32); \
  }

ITC_FRAMEWORK_WORD(82090760, Flag133)
ITC_FRAMEWORK_WORD(82090768, Flag134)
ITC_FRAMEWORK_WORD(82090770, Word60)
ITC_FRAMEWORK_WORD(82090788, Word88)
ITC_FRAMEWORK_WORD(8248F030, Word68)
ITC_FRAMEWORK_WORD(82090830, InputManagerFrom36)
ITC_FRAMEWORK_WORD(82090828, RendererFrom52)
ITC_FRAMEWORK_WORD(82090838, WorkerPoolFrom52)
ITC_FRAMEWORK_WORD(82090860, RendererOrTextFrom24)
ITC_FRAMEWORK_WORD(82090868, SceneFrom52)
ITC_FRAMEWORK_WORD(820908E0, RendererFrom36)
ITC_FRAMEWORK_WORD(820908E8, WorkerPoolFrom36)
ITC_FRAMEWORK_WORD(82090908, SceneFrom36)

// DebugText::SetPosition: r4, r5 = pointers to x and y; r3, r4 the words
// and r11 as the original leaves them.
extern "C" REX_FUNC(sub_82091050) {
  ITC_FRAMEWORK_FALLBACK(82091050)
  const uint32_t x = guest::LoadU32(base, ctx.r4.u32);
  const uint32_t y = guest::LoadU32(base, ctx.r5.u32);
  fw::DebugTextSetPosition(base, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = x;
  ctx.r4.u64 = y;
  ctx.r11.u64 = fw::kDebugFontPosition;
}

// --- The run loop's containers (src/framework/run_loop.h) ---
//
// Only the functions recompiled code calls in play have adapters (the
// iterator's increment, HandlerMap::HandlerMap, Insert and Clear); the
// decrement, the rotations, AllocateNodes, NewHead, NewNode, InsertNode and
// Erase are reached only from their plain C++ callers, and the flag switches
// those.

namespace {

// The functions that allocate: only over the allocation layer.
bool NativeHeap() { return ITC_FLAG(native_framework) && hooks::NativeHeapActive(); }

fw::Iterator IteratorAt(uint8_t* base, uint32_t address) {
  return {guest::LoadU32(base, address), guest::LoadU32(base, address + 4)};
}

}  // namespace

// The map iterator's operator++: r3 = the iterator; r3 left.
extern "C" REX_FUNC(sub_82099B18) {
  ITC_FRAMEWORK_FALLBACK(82099B18)
  fw::Iterator it = IteratorAt(base, ctx.r3.u32);
  if (!fw::MapIncrement(base, it)) {
    __imp__sub_82099B18(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it.node);
}

// HandlerMap: r3 = the map; r3 the head.
extern "C" REX_FUNC(sub_82099FB0) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() || !fw::CanAllocate(base)) {
    __imp__sub_82099FB0(ctx, base);
    return;
  }
  ctx.r3.u64 = fw::MapConstruct(base, ctx.r3.u32);
}

// Insert: r3 = the result (an iterator and the inserted byte), r4 = the
// map, r5 = the value; r3 the result.
extern "C" REX_FUNC(sub_82093740) {
  REX_FUNC_PROLOGUE();
  std::optional<fw::InsertResult> result;
  if (NativeHeap()) {
    result = fw::MapInsertUnique(base, ctx.r4.u32, ctx.r5.u32);
  }
  if (!result) {
    __imp__sub_82093740(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 0, result->where.container);
  guest::StoreU32(base, ctx.r3.u32 + 4, result->where.node);
  guest::StoreU8(base, ctx.r3.u32 + 8, result->inserted ? 1 : 0);
}

// RequestVector::Clear: r3 = the vector; r3 the result in its 96-byte frame
// (r1 - 16), as the original leaves it.
extern "C" REX_FUNC(sub_8209A080) {
  ITC_FRAMEWORK_FALLBACK(8209A080)
  if (!fw::RequestsClear(base, ctx.r3.u32)) {
    __imp__sub_8209A080(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u32 - 16;
}

// RequestVector::InsertN: r3 = the vector, r4 = where (by value: the
// vector in the high word, also stored in the caller's frame at r1 + 24 as
// the original does), r5 = the count, r6 = the request. r3 isn't the
// original's (a load's or operator delete's leftover, which its callers
// don't read).
extern "C" REX_FUNC(sub_8209A0E8) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() ||
      !fw::RequestsInsertN(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32)) {
    __imp__sub_8209A0E8(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 24, ctx.r4.u64);
}

// RequestVector::Insert: r3 = the result, r4 = the vector, r5 = where (by
// value, also stored at the caller's r1 + 32), r6 = the request; r3 the
// result.
extern "C" REX_FUNC(sub_8209A600) {
  REX_FUNC_PROLOGUE();
  std::optional<fw::Iterator> it;
  if (NativeHeap()) {
    it = fw::RequestsInsert(base, ctx.r4.u32, {uint32_t(ctx.r5.u64 >> 32), ctx.r5.u32},
                            ctx.r6.u32);
  }
  if (!it) {
    __imp__sub_8209A600(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r5.u64);
  guest::StoreU32(base, ctx.r3.u32 + 0, it->container);
  guest::StoreU32(base, ctx.r3.u32 + 4, it->node);
}

// RequestVector::PushBack: r3 = the vector, r4 = the request; r3 the
// vector, or Insert's result in its 96-byte frame (r1 - 16) when it took
// Insert, as the original leaves it.
extern "C" REX_FUNC(sub_8209A930) {
  REX_FUNC_PROLOGUE();
  std::optional<bool> inserted;
  if (NativeHeap()) {
    inserted = fw::RequestsPushBack(base, ctx.r3.u32, ctx.r4.u32);
  }
  if (!inserted) {
    __imp__sub_8209A930(ctx, base);
    return;
  }
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 16;
  }
}

// RunLoop::RunLoop: r3 = the run loop; r3 left.
extern "C" REX_FUNC(sub_8209A9F0) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() || !fw::CanAllocate(base)) {
    __imp__sub_8209A9F0(ctx, base);
    return;
  }
  fw::RunLoopConstruct(base, ctx.r3.u32);
}

// RunLoop::ApplyRequest: r3 = the run loop, r4 = the type (high word) and
// key, r5 = the delegate (high word), both also stored in the caller's
// frame (r1 + 32, + 40) as the original does; r3 HandlerMap::Insert's
// result in its 128-byte frame (r1 - 32) when it inserted, else left.
extern "C" REX_FUNC(sub_8209A458) {
  REX_FUNC_PROLOGUE();
  std::optional<bool> inserted;
  if (NativeHeap()) {
    inserted = fw::RunLoopApplyRequest(base, ctx.r3.u32, uint32_t(ctx.r4.u64 >> 32), ctx.r4.u32,
                                       uint32_t(ctx.r5.u64 >> 32));
  }
  if (!inserted) {
    __imp__sub_8209A458(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r4.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r5.u64);
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 32;
  }
}
