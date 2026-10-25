// The title screen and what comes before it (see title_screen.cpp).

#pragma once

namespace hooks {

// Main thread: whether menus should run without the 30 Hz tick wait
// (--skip_intro, until the first race starts).
bool TitleScreen_SkipTickWait(bool in_race);

// Render thread: Movie::RenderFrame (sub_820AE958, hooks/movies.cpp) has
// returned (--skip_intro waits for it before ending the boot movie).
void TitleScreen_MovieRendered();

}  // namespace hooks
