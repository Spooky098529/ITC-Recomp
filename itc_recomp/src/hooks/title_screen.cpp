// The boot up to the title screen, and the title screen's idle wait before
// the attract race: --skip_intro and --attract_after_s for automated runs,
// --log_boot_modes to see the sequence.
//
// The title screen's update (sub_822221A0) counts idle ticks in +36 of the
// object at [0x82814E34] and, at 600 (20 s at 30 Hz), sets the object's
// state (+4) to 5, which loads an attract-mode race. See
// docs/title-screen.md.

#include <format>
#include "title_screen.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "hooks/boot_mode.h"
#include "hooks/frame_driver.h"
#include "hooks/title_screen_native.h"

#include "guest.h"
#include "schema/boot_mode.h"
#include "schema/race_overlays.h"
#include "schema/title_screen.h"

#include "base/flags.h"
#include "base/log.h"

#include <atomic>
#include <string>

ITC_FLAG_BOOL(log_boot_modes, false, "ITC",
              "Log the front end's state objects as they change, and each tick and "
              "message handler the first time it runs");
ITC_FLAG_BOOL(skip_intro, false, "ITC",
              "Testing: skip the Ubisoft logo movie, and run the Genki logo and the "
              "title screen's intro without the 30 Hz tick wait (until the first race)");
ITC_FLAG_INT32(attract_after_s, 0, "ITC",
               "Testing: seconds the title screen waits before the attract race "
               "(0 = the game's 20)");
ITC_FLAG_INT32(attract_seed, -1, "ITC",
               "Testing: the game's random seed set to this when the attract race is "
               "about to be set up and again when a race starts, so the attract race "
               "comes out the same every run (with --max_fps=30 and no interpolation: "
               "frame dumps compared between builds); -1 leaves it");

namespace {

using G = schema::TitleScreenGlobals;
constexpr uint32_t kTitleScreenPointer = G::data_address();
constexpr int32_t kAttractTicks = 600;
constexpr int32_t kTicksPerSecond = 30;

bool g_first_race_started = false;
bool g_in_movie_wait = false;
// Set by the render thread once it has started the boot movie (see
// sub_820AE958).
std::atomic<bool> g_movie_started{false};

// --skip_intro: the movie step's Movies::State answers 1 (finished) once the
// render thread has started the movie.
bool MovieFinishedEarly() {
  return g_in_movie_wait && g_movie_started;
}

}  // namespace

namespace hooks {

bool TitleScreen_SkipTickWait(bool in_race) {
  if (in_race) {
    g_first_race_started = true;
  }
  return ITC_FLAG(skip_intro) && !g_first_race_started;
}

void TitleScreen_RaceStarted() {
  if (int32_t seed = ITC_FLAG(attract_seed); seed >= 0) {
    schema::RaceOverlaysGlobals::set_rand_seed(uint32_t(seed));
  }
}

}  // namespace hooks

// TitleScreen_Update: its plain C++ (src/title_screen/press_start.cpp) with
// --native_title_screen, the original without; the idle wait shortened
// around either.
ITC_NATIVE_ENTRY_ALWAYS(822221A0) {
  
  int32_t after = ITC_FLAG(attract_after_s) * kTicksPerSecond;
  const schema::TitleScreenData title = G::data();
  int32_t before = title ? int32_t(title.idle_ticks()) : 0;
  if (hooks::TitleScreenNative()) {
    hooks::TitleScreenPressStart(ctx);  // src/title_screen/press_start.cpp
  } else {
    lifted::original::c822221A0(ctx);
  }
  if (!title || G::data().address != title.address) {
    return;
  }
  // --attract_seed: at the tick before the game's limit (the next one sets
  // up the attract race).
  if (int32_t seed = ITC_FLAG(attract_seed); seed >= 0 &&
      int32_t(title.idle_ticks()) == kAttractTicks - 1 && before != kAttractTicks - 1) {
    schema::RaceOverlaysGlobals::set_rand_seed(uint32_t(seed));
  }
  if (after <= 0) {
    return;
  }
  // Only after a tick that counted (the counter went up by one): the next
  // one reaches the game's limit.
  int32_t now = int32_t(title.idle_ticks());
  if (now == before + 1 && now >= after && now < kAttractTicks - 1) {
    title.set_idle_ticks(uint32_t(kAttractTicks - 1));
    if (int32_t seed = ITC_FLAG(attract_seed); seed >= 0) {
      schema::RaceOverlaysGlobals::set_rand_seed(uint32_t(seed));
    }
  }
}

// Scene::Update: r3 = scene (its plain C++ with --native_frame_driver,
// src/frame_driver/). Hooked for --log_boot_modes: logs the front
// end's state objects at [0x82814E2C] and [0x82814E34] as they change.
ITC_NATIVE_ENTRY_ALWAYS(820988C8) {
  
  if (ITC_FLAG(log_boot_modes)) {
    static std::string last;
    std::string now;
    for (uint32_t pointer : {G::pages_address(), G::list_address(), kTitleScreenPointer}) {
      uint32_t object = guest::LoadU32(pointer);
      now += std::format(" [{:08X}]={:08X}:", pointer, object);
      for (uint32_t i = 0; object && i < 24; i += 4) {
        now += std::format(" {}", int32_t(guest::LoadU32(object + i)));
      }
    }
    if (now != last) {
      ITC_LOG_INFO("boot modes:{}", now);
      last = now;
    }
  }
  if (hooks::NativeFrameDriver()) {
    hooks::AppSceneUpdate(ctx);  // src/frame_driver/
  } else {
    lifted::original::c820988C8(ctx);
  }
}

// Boot::WaitForMovie, the boot's movie step, run every tick while a movie
// plays (the Ubisoft logo; its plain C++ with --native_boot_mode,
// src/boot_mode/): ends the step when the movie is skippable (0x826C5BC4) and a button
// is pressed, or when the movie has finished (sub_820A4450 returns 1) and
// this+9608 is 1. With --skip_intro the movie reports finished as soon as
// the render thread has started it.
ITC_NATIVE_ENTRY_ALWAYS(82107C58) {
  
  if (ITC_FLAG(log_boot_modes)) {
    ITC_LOG_INFO("boot modes: movie wait, this {:08X} +9608 {}", ctx.r3.u32,
                 schema::BootMachine(ctx.r3.u32).movie_started());
  }
  g_in_movie_wait = ITC_FLAG(skip_intro);
  if (hooks::BootModeNative()) {
    hooks::BootWaitForMovie(ctx, MovieFinishedEarly);
  } else {
    lifted::original::c82107C58(ctx);
  }
  g_in_movie_wait = false;
}

// Whether the playing movie has finished (1 = finished). With
// --native_boot_mode, Boot::WaitForMovie's plain C++ asks the same through
// MovieFinishedEarly; this wrap is reached by the recompiled callers only.
ITC_NATIVE_ENTRY_ALWAYS(820A4450) {
  
  if (hooks::BootModeNative()) {
    hooks::BootMoviesState(ctx, MovieFinishedEarly);
    return;
  }
  if (MovieFinishedEarly()) {
    ctx.r3.u64 = 1;
    return;
  }
  lifted::original::c820A4450(ctx);
}

namespace hooks {

// Movie::RenderFrame (sub_820AE958, hooks/movies.cpp) has returned. With the
// movie library (--play_movies_with_ffmpeg=false), its first call waits in
// the player (sub_82491C50) for the first decoded frame; if the movie step
// ended the movie before then, the main thread waited on the render thread
// in Boot::EndMovie and the decoder never delivered: --skip_intro stalled
// about 1 boot in 6 that way without the GPU plugin. So --skip_intro waits
// for it.
void TitleScreen_MovieRendered() {
  g_movie_started = true;
}

}  // namespace hooks
