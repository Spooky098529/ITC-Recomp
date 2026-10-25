// The action mappers' update (sub_8210B018) once a tick
// (--input_mappers_by_tick). See docs/pause-menu-fix.md.
//
// sub_8210B018 updates every action mapper (sub_8210C370 each), which
// rebuilds the held, pressed and fired words the menus and the car read and
// steps each action's repeat object. It's run from the app's update on every
// pass, render-only ones too, so the repeat objects count frames: at 144 fps
// the pause menu's first repeat comes after ~2 ticks instead of ~11, and then
// it moves one item every tick. Every reader runs on logic passes only
// (checked with --log_pause_menu), so skipping the update on render-only
// passes leaves what the logic pass set in place for them, as at 30 fps.

#include "action_mappers/action_mappers.h"
#include "debug/pause_menu_log.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"

#include "base/flags.h"

ITC_FLAG_DECLARE(bool, native_action_mappers);

ITC_FLAG_BOOL(input_mappers_by_tick, true, "ITC",
              "Update the action mappers (sub_8210B018) on logic passes only, so menu "
              "input repeats at the game's rate at any frame rate");

extern "C" REX_FUNC(sub_8210B018) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(input_mappers_by_tick) && frame_pacing::GameMessages_IsRenderOnlyPass()) {
    return;
  }
  debug::PauseMenuLog_NoteMapperUpdate(uint32_t(ctx.lr));
  // Natively (--native_action_mappers, src/action_mappers/) when every
  // mapper's repeat and curve objects are the ones seen in play.
  if (ITC_FLAG(native_action_mappers) && action_mappers::UpdateAllKnown(base, ctx.r3.u32)) {
    ctx.fpscr.disableFlushMode();
    double f1 = ctx.f1.f64;
    ctx.r3.u64 =
        action_mappers::UpdateAll(base, ctx.r3.u32, uint32_t(ctx.r25.u64 >> 32), &f1);
    ctx.f1.f64 = f1;
    return;
  }
  __imp__sub_8210B018(ctx, base);
}
