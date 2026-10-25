// The race HUD's movies and the rival logo natively
// (docs/functions/race-hud.md): which UI movie each runs, as plain C++ over
// guest memory (the objects are shared with recompiled code). The movies'
// calls (UiMovie::Run, UiMovie::Draw, UiMovie::FinishNamed) are made by the
// adapters (src/hooks/race_hud.cpp) through their overrides.

#pragma once

#include <cstdint>
#include <optional>

namespace race_hud {

// sub_822141E8 (RaceHud::RunMovie, probably; r3 = the HUD, r4 passed on as
// UiMovie::Run's: 0 to advance, else draw only): by the HUD's state (+448),
// 0 its movie at +604 run (UiMovie::Run) when +456 isn't 0, else drawn
// (UiMovie::Draw); 1 its movie at +624 run (sub_82212908's, merged: its
// RaceTime::FromTicks on the word +3932 of the object at +444 isn't worked
// out, its result unused); 2 and 4 its movie at +632 run; 3 sub_82212958
// (never seen in play: the original); 5 and above nothing.
enum class HudCall { kNone, kRun, kDraw, kRunTimer, kState3 };
struct HudMovie {
  HudCall call = HudCall::kNone;
  uint32_t movie = 0;
};
HudMovie RunMovie(uint8_t* base, uint32_t hud);

// sub_82211A28 (RivalLogo::Draw, probably; r3 = the logo's entry: +4 flags,
// bit 8 shown, bit 16 finished; +8 the movie's id): while shown, the movie
// found by its id in the movie table (UiMovies::Find, the id stored at the
// original's frame + 80 (`frame`) for the lookup, as the original does).
// Nothing when not shown.
std::optional<uint32_t> RivalLogoMovie(uint8_t* base, uint32_t entry, uint32_t frame);
// Once UiMovie::Run returns 0 (the movie ended) and UiMovie::FinishNamed has
// run: the entry's finished flag (16) set.
void RivalLogoFinished(uint8_t* base, uint32_t entry);

}  // namespace race_hud
