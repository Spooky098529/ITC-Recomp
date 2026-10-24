// The input manager and its pads and processors natively, and the C++
// library's std::list of pointers its listener lists are: adapters in front
// of src/input/input_manager.cpp and src/std_list/ptr_list.cpp
// (docs/functions/input.md). The functions that allocate run natively only
// while --native_heap routes the game's heap to the allocation layer, and
// hand the paths left to the original (see ptr_list.h) back to it from the
// start, nothing having been done. B1's input update is in input_update.cpp.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "std_list/ptr_list.h"

ITC_FLAG_BOOL(native_input, true, "ITC",
              "Run the input manager's set-up and the C++ library's std::list of pointers "
              "(docs/functions/input.md: PtrList::AllocateNodes sub_82099C08, IncSize "
              "sub_820958B8, PtrList sub_820AD058 and Insert sub_82095958, with --native_heap) "
              "natively instead of through the translated code (same results; checked by "
              "--run_harnesses=input)");

namespace {

// The functions that allocate: only over the allocation layer.
bool NativeLifetime() {
  return ITC_FLAG(native_input) && hooks::NativeHeapActive();
}

}  // namespace

// --- std::list of pointers ---

// AllocateNodes: r3 = the count; r3 the block.
extern "C" REX_FUNC(sub_82099C08) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    if (const auto block = std_list::AllocateNodes(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_82099C08(ctx, base);
}

// IncSize: r3 = the list, r4 = the count; r3 left as it came.
extern "C" REX_FUNC(sub_820958B8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_input) || !std_list::IncSize(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820958B8(ctx, base);
  }
}

// PtrList: r3 = the list; r3 left as it came.
extern "C" REX_FUNC(sub_820AD058) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !std_list::Construct(base, ctx.r3.u32)) {
    __imp__sub_820AD058(ctx, base);
  }
}

// Insert: r3 = the list, r4 = the iterator by value (its node in the low
// word), r5 = the value's address; r3 left as it came (the list).
extern "C" REX_FUNC(sub_82095958) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !std_list::Insert(base, ctx.r3.u32, ctx.r4.u32,
                                             guest::LoadU32(base, ctx.r5.u32))) {
    __imp__sub_82095958(ctx, base);
  }
}
