// The action mappers natively: adapters in front of
// src/action_mappers/action_mappers.cpp (docs/functions/action-mappers.md).
// The repeat and curve objects' slots are reached through their vtables, so
// their adapters stay until every caller is native.

#include "guest.h"

#include "action_mappers/action_mappers.h"
#include "base/flags.h"
#include "debug/pause_menu_log.h"
#include "frame_pacing/game_messages.h"

ITC_FLAG_BOOL(native_action_mappers, true, "ITC",
              "Run the action mappers (docs/functions/action-mappers.md: the repeat and "
              "curve objects' updates, sub_8210B568 and sub_8210B5B8; IndexOf, sub_8210B140; "
              "the listeners' held test, sub_8258CF10 and sub_825924E0; the client list, "
              "sub_8259C6D0 to sub_8259C960; the update, sub_8210B018, sub_8210C370, "
              "sub_8210C0E0, sub_8210B7D0, sub_8210B4A8, sub_8210B500, sub_8210B718 and "
              "sub_8210BBA0) natively instead of through the translated "
              "code (same results; checked by --run_harnesses=action_mappers)");

// The flag's fallback; the floats as the original's (no flush to zero).
#define ITC_MAPPERS_FALLBACK(address)       \
  REX_FUNC_PROLOGUE();                      \
  if (!ITC_FLAG(native_action_mappers)) {   \
    __imp__sub_##address(ctx, base);        \
    return;                                 \
  }                                         \
  ctx.fpscr.disableFlushMode();

// An adapter returning a word in r3.
#define ITC_MAPPERS_WORD(address, expr)  \
  extern "C" REX_FUNC(sub_##address) {  \
    ITC_MAPPERS_FALLBACK(address)       \
    ctx.r3.u64 = uint32_t(expr);        \
  }

// An adapter leaving r3 as it was (the originals don't change it).
#define ITC_MAPPERS_VOID(address, call) \
  extern "C" REX_FUNC(sub_##address) { \
    ITC_MAPPERS_FALLBACK(address)      \
    call;                              \
  }

using namespace action_mappers;

extern "C" REX_FUNC(sub_8210B5B8) {
  ITC_MAPPERS_FALLBACK(8210B5B8)
  ctx.f1.f64 = CurveApply(base, ctx.r3.u32, ctx.f1.f64);
}
ITC_MAPPERS_WORD(8210B568, RepeatUpdate(base, ctx.r3.u32, ctx.r4.u32))
ITC_MAPPERS_WORD(8210B140, IndexOf(ctx.r4.u32))

// The held tests, also counted for --log_input_reads (as the wrappers in
// src/frame_pacing/game_messages.cpp count the other button tests).
extern "C" REX_FUNC(sub_825924E0) {
  REX_FUNC_PROLOGUE();
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t listener = ctx.r3.u32;
  const uint32_t mask = guest::LoadU32(base, ctx.r4.u32);
  if (ITC_FLAG(native_action_mappers)) {
    ctx.r3.u64 = ListenerHeld(base, listener, mask);
  } else {
    __imp__sub_825924E0(ctx, base);
  }
  if (frame_pacing::GameMessages_LogInputReads()) {
    frame_pacing::GameMessages_NoteInputRead(0x825924E0, caller, listener, mask,
                                             ctx.r3.u32 & 0xFF);
  }
}
extern "C" REX_FUNC(sub_8258CF10) {
  REX_FUNC_PROLOGUE();
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t listener = ctx.r3.u32;
  const uint32_t mask = guest::LoadU32(base, ctx.r4.u32);
  if (!ITC_FLAG(native_action_mappers)) {
    __imp__sub_8258CF10(ctx, base);  // its call to sub_825924E0 is counted there
    if (frame_pacing::GameMessages_LogInputReads()) {
      frame_pacing::GameMessages_NoteInputRead(0x8258CF10, caller, listener, mask,
                                               ctx.r3.u32 & 0xFF);
    }
    return;
  }
  ctx.r3.u64 = ListenerTest(base, listener, ctx.r4.u32);
  NoteListenerTest(caller, listener, mask, ctx.r3.u32);
}

ITC_MAPPERS_VOID(8259C6D0, ClientNodeConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_MAPPERS_VOID(8259C7B8, ClientListConstruct(base, ctx.r3.u32))
ITC_MAPPERS_VOID(8259C960, ClientListAdd(base, ctx.r3.u32))
ITC_MAPPERS_WORD(8259C868, ClientListCount(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_MAPPERS_WORD(8259C898, ClientListBoundCount(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_MAPPERS_WORD(8259C818, ClientListFind(base, ctx.r3.u32, ctx.r4.u32))

// The update. sub_8210C0E0, sub_8210B7D0, sub_8210B4A8, sub_8210B500,
// sub_8210B718 and sub_8210BBA0 have no adapters: their only callers in
// play (sub_8210C370 and below) are native, and call their plain C++. The
// update runs the original unless every repeat and curve object's slot is
// the function seen in play. The held word's bits 30 and 31 come from the
// stack word the original builds it in (see action_mappers.h): the high
// half of r25 at sub_8210C370.

// The mapper's update, also logged for --log_pause_menu (moved from
// src/debug/pause_menu_log.cpp).
extern "C" REX_FUNC(sub_8210C370) {
  REX_FUNC_PROLOGUE();
  const uint32_t mapper = ctx.r3.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t fired_before = debug::PauseMenuLog_BeforeMapperUpdate(base, mapper);
  if (ITC_FLAG(native_action_mappers) && MapperCallsKnown(base, mapper)) {
    ctx.fpscr.disableFlushMode();
    double f1 = 0;
    ctx.r3.u64 = MapperUpdate(base, mapper, uint32_t(ctx.r25.u64 >> 32), &f1);
    ctx.f1.f64 = f1;
  } else {
    __imp__sub_8210C370(ctx, base);
  }
  debug::PauseMenuLog_AfterMapperUpdate(base, mapper, caller, fired_before);
}
