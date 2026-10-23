// XACT natively: adapters in front of src/xact/xact.cpp
// (docs/functions/xact.md). Most of XACT is reached through its objects'
// vtables, so these adapters stay until their callers are native.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "xact/xact.h"

ITC_FLAG_BOOL(native_xact, true, "ITC",
              "Run the parts of XACT made native (docs/functions/xact.md: the name hash, "
              "sub_825BFC50; the node pools, sub_825C2060, sub_825C2C60, sub_825C2CC8 and "
              "sub_825C1F30; XACTEngine::AddCue, sub_825C1518; the global settings' "
              "constructors, sub_825C21B0 and sub_825C3508; XACTVariable::Init, sub_825C1DE0; "
              "the cue's DetachSound and FindVariable, sub_825C4FC0 and sub_825C5758; the "
              "fader's constructor and destructor, sub_825CC0A0 and sub_825CC0D0) natively "
              "instead of through the translated code (same results; checked by "
              "--run_harnesses=xact)");

// The flag's fallback; the floats moved as the original's lfs and stfs
// (no flush to zero).
#define ITC_XACT_FALLBACK(address)    \
  REX_FUNC_PROLOGUE();                \
  if (!ITC_FLAG(native_xact)) {       \
    __imp__sub_##address(ctx, base);  \
    return;                           \
  }                                   \
  ctx.fpscr.disableFlushMode();

// An adapter leaving `r3_expr` in r3 after `call`.
#define ITC_XACT_VOID(address, call, r3_expr) \
  extern "C" REX_FUNC(sub_##address) {       \
    ITC_XACT_FALLBACK(address)               \
    const uint32_t itc_r3_ = (r3_expr);      \
    call;                                    \
    ctx.r3.u64 = itc_r3_;                    \
  }

// An adapter returning a word in r3.
#define ITC_XACT_WORD(address, expr)    \
  extern "C" REX_FUNC(sub_##address) { \
    ITC_XACT_FALLBACK(address)         \
    ctx.r3.u64 = uint32_t(expr);       \
  }

// An adapter for a function allocating or freeing through the native
// layer: the original unless --native_heap routes the game's heap there.
#define ITC_XACT_HEAP_WORD(address, expr)                       \
  extern "C" REX_FUNC(sub_##address) {                         \
    ITC_XACT_FALLBACK(address)                                 \
    if (!hooks::NativeHeapActive()) {                          \
      __imp__sub_##address(ctx, base);                         \
      return;                                                  \
    }                                                          \
    ctx.r3.u64 = uint32_t(expr);                               \
  }

using namespace xact;

// The name hash: the original where it traps (a table size of 0).
extern "C" REX_FUNC(sub_825BFC50) {
  ITC_XACT_FALLBACK(825BFC50)
  uint32_t hash = 0;
  if (!HashName(base, ctx.r4.u32, ctx.r5.u32, &hash)) {
    __imp__sub_825BFC50(ctx, base);
    return;
  }
  ctx.r3.u64 = hash;
}

ITC_XACT_VOID(825C2060, NodePoolConstruct(base, ctx.r3.u32), ctx.r3.u32)
ITC_XACT_VOID(825C2C60, NodePoolFree(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)
ITC_XACT_HEAP_WORD(825C2CC8, NodePoolAlloc(base, ctx.r3.u32))
ITC_XACT_HEAP_WORD(825C1F30, NodePoolFreeChunks(base, ctx.r3.u32))

ITC_XACT_VOID(825C1518, EngineAddCue(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)

// XACTGlobalSettingsBase::XACTGlobalSettingsBase (sub_825C3508) has no
// adapter: its only caller, sub_825C21B0, is native.
ITC_XACT_VOID(825C21B0, GlobalSettingsConstruct(base, ctx.r3.u32, ctx.r4.u32), ctx.r3.u32)

ITC_XACT_VOID(825C1DE0,
              VariableInit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                           ctx.r8.u32),
              ctx.r3.u32)
ITC_XACT_VOID(825C4FC0, CueDetachSound(base, ctx.r3.u32), ctx.r3.u32)
ITC_XACT_WORD(825C5758, CueFindVariable(base, ctx.r3.u32, ctx.r4.u32))
ITC_XACT_VOID(825CC0A0, FaderConstruct(base, ctx.r3.u32), ctx.r3.u32)
ITC_XACT_VOID(825CC0D0, FaderDestruct(base, ctx.r3.u32), ctx.r3.u32)
