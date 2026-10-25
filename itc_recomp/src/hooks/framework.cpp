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
              "(the iterator's increment sub_82099B18 and decrement sub_82091AE8, RotateLeft "
              "sub_82092B10 and RotateRight sub_82092A00, and with --native_heap AllocateNodes "
              "sub_82099C80, NewHead sub_82099E88, HandlerMap::HandlerMap sub_82099FB0, NewNode "
              "sub_82099E20, InsertNode sub_82093400 and Insert sub_82093740) and request vector "
              "(Erase sub_82092EF8, Clear sub_8209A080)) natively instead of "
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

namespace {

// The functions that allocate: only over the allocation layer.
bool NativeHeap() { return ITC_FLAG(native_framework) && hooks::NativeHeapActive(); }

fw::Iterator IteratorAt(uint8_t* base, uint32_t address) {
  return {guest::LoadU32(base, address), guest::LoadU32(base, address + 4)};
}
fw::Iterator IteratorOf(uint64_t reg) { return {uint32_t(reg >> 32), uint32_t(reg)}; }

}  // namespace

// The map iterator's operator++ and operator--: r3 = the iterator; r3 left.
extern "C" REX_FUNC(sub_82099B18) {
  ITC_FRAMEWORK_FALLBACK(82099B18)
  fw::Iterator it = IteratorAt(base, ctx.r3.u32);
  if (!fw::MapIncrement(base, it)) {
    __imp__sub_82099B18(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it.node);
}

extern "C" REX_FUNC(sub_82091AE8) {
  ITC_FRAMEWORK_FALLBACK(82091AE8)
  fw::Iterator it = IteratorAt(base, ctx.r3.u32);
  if (!fw::MapDecrement(base, it)) {
    __imp__sub_82091AE8(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it.node);
}

// The rotations: r3 = the map, r4 = the node; r3 left.
extern "C" REX_FUNC(sub_82092B10) {
  ITC_FRAMEWORK_FALLBACK(82092B10)
  fw::MapRotateLeft(base, ctx.r3.u32, ctx.r4.u32);
}

extern "C" REX_FUNC(sub_82092A00) {
  ITC_FRAMEWORK_FALLBACK(82092A00)
  fw::MapRotateRight(base, ctx.r3.u32, ctx.r4.u32);
}

// AllocateNodes: r3 = the count; r3 the block.
extern "C" REX_FUNC(sub_82099C80) {
  REX_FUNC_PROLOGUE();
  if (NativeHeap()) {
    if (const auto block = fw::MapAllocateNodes(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_82099C80(ctx, base);
}

// NewHead: r3 the head.
extern "C" REX_FUNC(sub_82099E88) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() || !fw::CanAllocate(base)) {
    __imp__sub_82099E88(ctx, base);
    return;
  }
  ctx.r3.u64 = fw::MapNewHead(base);
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

// NewNode: r4, r5, r6 = left, parent, right, r7 = the value, r8 = the
// color; r3 the node.
extern "C" REX_FUNC(sub_82099E20) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() || !fw::CanAllocate(base)) {
    __imp__sub_82099E20(ctx, base);
    return;
  }
  ctx.r3.u64 = fw::MapNewNode(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u8);
}

// InsertNode: r3 = the result (an iterator), r4 = the map, r5 = add on the
// left (a byte), r6 = where, r7 = the value; r3 the result.
extern "C" REX_FUNC(sub_82093400) {
  REX_FUNC_PROLOGUE();
  std::optional<fw::Iterator> it;
  if (NativeHeap()) {
    it = fw::MapInsert(base, ctx.r4.u32, ctx.r5.u8 != 0, ctx.r6.u32, ctx.r7.u32);
  }
  if (!it) {
    __imp__sub_82093400(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it->node);
  guest::StoreU32(base, ctx.r3.u32 + 0, it->container);
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

// The request vector's erase(first, last): r3 = the result, r4 = the
// vector, r5 and r6 = the iterators (by value: the vector in the high
// word), also stored in the caller's frame (r1 + 32, + 40) as the original
// does; the result is first. r3 left.
extern "C" REX_FUNC(sub_82092EF8) {
  ITC_FRAMEWORK_FALLBACK(82092EF8)
  if (!fw::RequestsErase(base, ctx.r4.u32, IteratorOf(ctx.r5.u64), IteratorOf(ctx.r6.u64))) {
    __imp__sub_82092EF8(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r5.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r6.u64);
  REX_STORE_U64(ctx.r3.u32, ctx.r5.u64);
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
