// The title screen and what comes before it (see title_screen.cpp).

#pragma once

namespace hooks {

// Main thread: whether menus should run without the 30 Hz tick wait
// (--skip_intro, until the first race starts).
bool TitleScreen_SkipTickWait(bool in_race);

}  // namespace hooks
