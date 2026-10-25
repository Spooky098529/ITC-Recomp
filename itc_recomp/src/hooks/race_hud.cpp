// The race HUD's movies and the rival logo natively: adapters in front of
// src/race_hud/ (docs/functions/race-hud.md). The movies' calls go through
// their overrides (UiMovie::Run, UiMovie::Draw, UiMovie::FinishNamed) with
// the originals' return addresses, from frames like theirs.

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/race_hud.h"
#include "race_hud/race_hud.h"

ITC_FLAG_BOOL(native_race_hud, true, "ITC",
              "Run the race HUD's movie choice RaceHud::RunMovie sub_822141E8 (with "
              "sub_82212908 merged into it) and the rival logo's draw sub_82211A28 natively "
              "instead of through the translated code (docs/functions/race-hud.md; their movie "
              "calls through their overrides; same results, checked by "
              "--run_harnesses=race_hud)");

// RaceHud::RunMovie: r3 = the HUD, r4 = UiMovie::Run's (0 to advance, else
// draw only). The original's tail calls keep lr, r1 and r4; state 1's
// (sub_82212908, merged) runs UiMovie::Run from its 112-byte frame.
extern "C" REX_FUNC(sub_822141E8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_hud)) {
    __imp__sub_822141E8(ctx, base);
    return;
  }
  const race_hud::HudMovie movie = race_hud::RunMovie(base, ctx.r3.u32);
  switch (movie.call) {
    case race_hud::HudCall::kNone:
      return;
    case race_hud::HudCall::kRun:
      ctx.r3.u64 = movie.movie;
      sub_8211E1B8(ctx, base);
      return;
    case race_hud::HudCall::kDraw:
      ctx.r3.u64 = movie.movie;
      sub_8211D280(ctx, base);
      return;
    case race_hud::HudCall::kState3:
      // Never seen in play: the original, r3 and r4 as they came.
      sub_82212958(ctx, base);
      return;
    case race_hud::HudCall::kRunTimer: {
      const uint64_t lr = ctx.lr;
      guest_call::InFrame(ctx, base, 112, 0x8221293C, [&](uint32_t) {
        ctx.r3.u64 = movie.movie;
        sub_8211E1B8(ctx, base);
      });
      ctx.lr = lr;
      return;
    }
  }
}

namespace race_hud_hooks {
// The logo's movie run (UiMovie::Run, r4 = 0) while shown; once it has
// ended, UiMovie::FinishNamed on the logo's named animation (its id, +8)
// and the finished flag set. From a frame like the original's (112 bytes).
void DrawRivalLogo(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_race_hud)) {
    __imp__sub_82211A28(ctx, base);
    return;
  }
  const uint32_t entry = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 112, 0x82211A64, [&](uint32_t frame) {
    const std::optional<uint32_t> movie = race_hud::RivalLogoMovie(base, entry, frame);
    if (!movie) {
      return;
    }
    ctx.r3.u64 = *movie;
    ctx.r4.u64 = 0;
    ctx.lr = 0x82211A74;
    sub_8211E1B8(ctx, base);
    if (ctx.r3.u32 != 0) {
      return;
    }
    ctx.r3.u64 = *movie;
    ctx.r4.u64 = guest::LoadU32(base, entry + 8);
    ctx.lr = 0x82211A88;
    sub_8211DF20(ctx, base);
    race_hud::RivalLogoFinished(base, entry);
  });
  ctx.lr = lr;
}
}  // namespace race_hud_hooks
