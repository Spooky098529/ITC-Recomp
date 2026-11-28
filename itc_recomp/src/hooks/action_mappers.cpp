// The action mappers natively: adapters in front of
// src/action_mappers/action_mappers.cpp (docs/functions/action-mappers.md).
// The repeat and curve objects' slots are reached through their vtables, so
// their adapters stay until every caller is native.

#include "guest.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "action_mappers/action_mappers.h"
#include "base/flags.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "frame_pacing/game_messages.h"
#include "hooks/app_start_up.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "lifted/sdk.h"
#include "schema/action_mappers.h"

ITC_FLAG_BOOL(native_action_mappers, true, "ITC",
              "Run the action mappers (docs/functions/action-mappers.md: the repeat and "
              "curve objects' updates, sub_8210B568 and sub_8210B5B8; IndexOf, sub_8210B140; "
              "the listeners' held test, sub_8258CF10 and sub_825924E0; the client list, "
              "sub_8259C6D0 to sub_8259C960; the update, sub_8210B018, sub_8210C370, "
              "sub_8210C0E0, sub_8210B7D0, sub_8210B4A8, sub_8210B500, sub_8210B718 and "
              "sub_8210BBA0; the read slots, sub_8210BE70, sub_8210BF40, sub_8210C010, "
              "sub_8210BA28, sub_8210BED8, sub_8210BFA8 and sub_8210BA68; the setters, sub_8210B698, sub_8210BDF0 and sub_8210BE30; "
              "ActionMappers::Configure, sub_8209F228; the set-up, sub_8210C078, sub_8210BB38, "
              "sub_8210BAB0, sub_8210B8A8, sub_8210B438, sub_8210AEB0, sub_8210AE60 and "
              "sub_8210AF00; the mapper's constructor, sub_8210C180; ActionMapper::Bind, "
              "sub_8210B270 (no adapter), and InputClientList::BindAll, sub_8210AF08, with --native_heap; "
              "ActionMappers::Init, sub_8209F430, with --native_heap) "
              "natively instead of through the translated "
              "code (same results; checked by --run_harnesses=action_mappers)");

// The flag's fallback; the floats as the original's (no flush to zero).
#define ITC_MAPPERS_FALLBACK(address)       \
  REX_FUNC_PROLOGUE();                      \
  if (!ITC_FLAG(native_action_mappers)) {   \
    lifted::original::c##address(ctx); \
    return;                                 \
  }                                         \
  ctx.fpscr.disableFlushMode();

// An adapter returning a word in r3.
#define ITC_MAPPERS_WORD(address, expr)  \
  ITC_NATIVE_ENTRY_ALWAYS(address) {  \
    ITC_MAPPERS_FALLBACK(address)       \
    ctx.r3.u64 = uint32_t(expr);        \
  }

using namespace action_mappers;

ITC_NATIVE_ENTRY_ALWAYS(8210B5B8) {
  ITC_MAPPERS_FALLBACK(8210B5B8)
  ctx.f1.f64 = CurveApply(ctx.r3.u32, ctx.f1.f64);
}
ITC_MAPPERS_WORD(8210B568, RepeatUpdate(ctx.r3.u32, ctx.r4.u32))

// The held tests, also counted for --log_input_reads (as the wrappers in
// src/frame_pacing/game_messages.cpp count the other button tests).
ITC_NATIVE_ENTRY_ALWAYS(825924E0) {
  
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t listener = ctx.r3.u32;
  const uint32_t mask = schema::ActionButtonMask(ctx.r4.u32).mask();
  if (ITC_FLAG(native_action_mappers)) {
    ctx.r3.u64 = ListenerHeld(listener, mask);
  } else {
    lifted::original::c825924E0(ctx);
  }
  if (frame_pacing::GameMessages_LogInputReads()) {
    frame_pacing::GameMessages_NoteInputRead(0x825924E0, caller, listener, mask,
                                             ctx.r3.u32 & 0xFF);
  }
}
ITC_NATIVE_ENTRY_ALWAYS(8258CF10) {
  
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t listener = ctx.r3.u32;
  const uint32_t mask = schema::ActionButtonMask(ctx.r4.u32).mask();
  if (!ITC_FLAG(native_action_mappers)) {
    lifted::original::c8258CF10(ctx);  // its call to sub_825924E0 is counted there
    if (frame_pacing::GameMessages_LogInputReads()) {
      frame_pacing::GameMessages_NoteInputRead(0x8258CF10, caller, listener, mask,
                                               ctx.r3.u32 & 0xFF);
    }
    return;
  }
  ctx.r3.u64 = ListenerTest(listener, ctx.r4.u32);
  NoteListenerTest(caller, listener, mask, ctx.r3.u32);
}

// IndexOf (sub_8210B140), ClientNode::ClientNode, ClientList::ClientList,
// ClientList::Add, Count, BoundCount and Find (sub_8259C6D0, sub_8259C7B8,
// sub_8259C960, sub_8259C868, sub_8259C898, sub_8259C818) have no adapters:
// their callers in play (BindAll and ActionMappers::Update) are native and
// call their plain C++.

// The update. sub_8210C0E0, sub_8210B7D0, sub_8210B4A8, sub_8210B500,
// sub_8210B718 and sub_8210BBA0 have no adapters: their only callers in
// play (sub_8210C370 and below) are native, and call their plain C++. The
// update runs the original unless every repeat and curve object's slot is
// the function seen in play. The held word's bits 30 and 31 come from the
// stack word the original builds it in (see action_mappers.h): the high
// half of r25 at sub_8210C370.

// The mapper's update, also logged for --log_pause_menu (moved from
// src/debug/pause_menu_log.cpp). The held word's bits 30 and 31 (no
// action's; the original's come from an uninitialized stack word, the high
// half of its caller's r25) are 0 (the rewiring, stage 3).
ITC_NATIVE_ENTRY_ALWAYS(8210C370) {
  const uint32_t mapper = ctx.r3.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t fired_before = debug::PauseMenuLog_BeforeMapperUpdate(mapper);
  if (ITC_FLAG(native_action_mappers) && MapperCallsKnown(mapper)) {
    ctx.fpscr.disableFlushMode();
    double f1 = 0;
    ctx.r3.u64 = MapperUpdate(mapper, 0, &f1);
    ctx.f1.f64 = f1;
  } else {
    lifted::original::c8210C370(ctx);
  }
  debug::PauseMenuLog_AfterMapperUpdate(mapper, caller, fired_before);
}

// The read slots, also logged for --log_pause_menu (moved from
// src/debug/pause_menu_log.cpp): reached through the mapper's vtable from
// the menus, the pause menu and the car.
#define ITC_MAPPERS_READ_SLOT(address, slot, function)                          \
  ITC_NATIVE_ENTRY_ALWAYS(address) {                                         \
    \
    const uint32_t mapper = ctx.r3.u32;                                        \
    const uint32_t action = ctx.r4.u32;                                        \
    const uint32_t caller = uint32_t(ctx.lr);                                  \
    if (ITC_FLAG(native_action_mappers)) {                                     \
      ctx.r3.u64 = function(mapper, action);                             \
    } else {                                                                   \
      lifted::original::c##address(ctx);                            \
    }                                                                          \
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, action, caller,             \
                                     double(slot == 14 ? ctx.r3.u32 & 0xFFFF   \
                                                       : ctx.r3.u32));         \
  }
ITC_MAPPERS_READ_SLOT(8210BE70, 8, Held)
ITC_MAPPERS_READ_SLOT(8210BF40, 10, Pressed)
ITC_MAPPERS_READ_SLOT(8210C010, 12, Fired)
ITC_MAPPERS_READ_SLOT(8210BA28, 14, AnalogRaw)
ITC_MAPPERS_READ_SLOT(8210BED8, 9, HeldBefore)
ITC_MAPPERS_READ_SLOT(8210BFA8, 11, Released)
#undef ITC_MAPPERS_READ_SLOT

// Slot 5 (the analog value in f1; r3 left as IndexOf's result), logged the
// same way with its f1.
ITC_NATIVE_ENTRY_ALWAYS(8210BA68) {
  
  const uint32_t mapper = ctx.r3.u32;
  const uint32_t action = ctx.r4.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  if (ITC_FLAG(native_action_mappers)) {
    ctx.fpscr.disableFlushMode();
    double f1 = 0;
    ctx.r3.u64 = AnalogValue(mapper, action, &f1);
    ctx.f1.f64 = f1;
  } else {
    lifted::original::c8210BA68(ctx);
  }
  debug::PauseMenuLog_NoteSlotRead(5, mapper, action, caller, ctx.f1.f64);
}

// The setters (slots 2, 4 and 7), from ActionMappers::Configure at boot.
ITC_MAPPERS_WORD(8210B698, SetThreshold(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.f2.f64,
                                        ctx.r7.u32))
ITC_MAPPERS_WORD(8210BDF0, SetRepeat(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
ITC_MAPPERS_WORD(8210BE30, SetCurve(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.f2.f64,
                                    ctx.f3.f64))

// ActionMappers::Configure (sub_8209F228) has no adapter: its only caller
// in play, ActionMappers::Init below, calls its plain C++ (the original when
// ConfigureKnown fails).

// The set-up. ResetWords, ResetAnalog, InitRepeats, InitCurves and
// ResetStages (sub_8210B8A8, sub_8210B438, sub_8210BB38, sub_8210BAB0,
// sub_8210C078) and InputClient::InputClient (sub_8210AE60) have no adapters:
// their only callers in play (the mapper's constructor and below) are
// native and call their plain C++ (a recompiled caller reaching them runs
// the original, and the flag switches the constructor).

// InputClientList::InputClientList, InputClientList::Add and
// ActionMapper::ActionMapper (sub_8210AEB0, sub_8210AF00, sub_8210C180) have
// no adapters: their only caller in play, ActionMappers::Init below, calls
// their plain C++ (a recompiled caller reaching them runs the original, and
// the flag switches Init).

// ActionMapper::Bind (sub_8210B270) has no adapter: its only caller in
// play, BindAll below, calls its plain C++ (a recompiled caller reaching it
// runs the original, and the flag switches BindAll).

// InputClientList::BindAll: r3 = the list; r3 = 1. For each mapper (the
// nodes of type 2), the input object from the list's owner (+36) through
// its vtable slot +36 (a guest call into the app: sub_820944C8 in play,
// which reaches the input manager through the framework), the mapper bound
// (natively when MapperBindKnown, else the original Bind) and the bound
// count up. Run in the original's frame (112 bytes); its own work is the
// client list's plain C++.
ITC_NATIVE_ENTRY_ALWAYS(8210AF08) {
  ITC_MAPPERS_FALLBACK(8210AF08)
  ITC_NATIVE_RUNNING("sub_8210AF08 InputClientList::BindAll");
  const uint32_t list = ctx.r3.u32;
  const uint64_t saved_lr = ctx.lr;
  guest_call::InFrame(ctx, 112, 0x8210AF58, [&](uint32_t) {
    for (uint32_t i = 0; i < ClientListCount(list, 2, 0); i++) {
      const uint32_t mapper = ClientListFind(2, i);
      const schema::ActionMappersOwner owner = schema::InputClientList(list).manager();
      ctx.r3.u64 = owner.address;
      ctx.lr = 0x8210AF58;
      lifted::CallFromContext(ctx, owner.vtable().get_input_manager());
      const uint32_t object = ctx.r3.u32;
      if (hooks::NativeHeapActive() && MapperBindKnown(mapper, object)) {
        MapperBind(mapper, object);
      } else {
        ctx.r3.u64 = mapper;
        ctx.r4.u64 = object;
        ctx.lr = 0x8210AF64;
        lifted::original::c8210B270(ctx);
      }
      ClientListBoundCount(list, 2, 1);
    }
  });
  ctx.lr = saved_lr;
  ctx.r3.u64 = 1;
}

// ActionMappers::Init: r3 = the manager (the app object), r4 = the settings'
// index. The list and the mappers made natively (InitCreate), then BindAll
// through its adapter (it makes a guest call to the owner's slot +36) and
// Configure, in the original's frame (112 bytes). The original unless the
// heap is the allocation layer's, the mappers' constructor runs natively and
// operator new gives every block. r3 and f1-f3 are Configure's.
ITC_NATIVE_ENTRY_ALWAYS(8209F430) {
  ITC_MAPPERS_FALLBACK(8209F430)
  if (hooks::StartUpRecorded(ctx, 0x8209F430)) {
    return;
  }
  uint32_t list = 0;
  if (!hooks::NativeHeapActive() || !MapperConstructKnown() ||
      !InitCreate(ctx.r3.u32, &list)) {
    lifted::original::c8209F430(ctx);
    return;
  }
  const uint32_t index = ctx.r4.u32;
  const uint64_t saved_lr = ctx.lr;
  guest_call::InFrame(ctx, 112, 0x8209F56C, [&](uint32_t) {
    ctx.r3.u64 = list;
    lifted::Enter(ctx, game::InputClientList::BindAll);
    if (ConfigureKnown()) {
      double f[3] = {ctx.f1.f64, ctx.f2.f64, ctx.f3.f64};
      ctx.r3.u64 = Configure(index, f);
      ctx.f1.f64 = f[0];
      ctx.f2.f64 = f[1];
      ctx.f3.f64 = f[2];
    } else {
      ctx.r3.u64 = index;
      ctx.lr = 0x8209F574;
      lifted::CallFromContext(ctx, 0x8209F228);
    }
  });
  ctx.lr = saved_lr;
}

// For the fallback originals (docs/missing-functions.md): hand ports with
// no adapter that lifted/originals.cpp calls by address. The floats as the
// originals' (no flush to zero).
// InputClient::InputClient: r3 = the node (left), r4 = the type.
ITC_DIRECT_ENTRY(8210AE60) { InputClientConstruct(ctx.r3.u32, ctx.r4.u32); }
// ActionMapper::Hysteresis: r3 = the words (mapper + 448), r4 = the index,
// f1 = the value, r6 = the config.
ITC_DIRECT_ENTRY(8210B718) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = Hysteresis(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r6.u32);
}
// ActionMapper::Analog: r3 = mapper + 1072, r4 = the config, r5 = the raw
// values; f1 left as the last curve's result. The original where a curve
// object's slot isn't the one seen in play.
ITC_DIRECT_ENTRY(8210B7D0) {
  if (!CurvesKnown(ctx.r3.u32 - 1072)) {
    lifted::original::c8210B7D0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  double f1 = ctx.f1.f64;
  ctx.r3.u64 = Analog(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &f1);
  ctx.f1.f64 = f1;
}
// ActionMapper::Words: r3 = mapper + 448, r4 = the config, r5 = the
// states' word's address; the held word's stack bits from r1 - 64 (its
// frame's +80). The original where a repeat object's slot isn't known.
ITC_DIRECT_ENTRY(8210BBA0) {
  if (!RepeatsKnown(ctx.r3.u32 - 448)) {
    lifted::original::c8210BBA0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = Words(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, guest::LoadU32(ctx.r1.u32 - 64));
}
// ActionMapper::ResetStages: r3 = mapper + 448.
ITC_DIRECT_ENTRY(8210C078) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ResetStages(ctx.r3.u32);
}
// ClientList::ClientList and ClientList::Add: r3 = the list or the node
// (left).
ITC_DIRECT_ENTRY(8259C7B8) { ClientListConstruct(ctx.r3.u32); }
ITC_DIRECT_ENTRY(8259C960) { ClientListAdd(ctx.r3.u32); }
