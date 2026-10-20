// The boot up to the title screen, and the title screen's idle wait before
// the attract race: --skip_intro and --attract_after_s for automated runs,
// --log_boot_modes to see the sequence.
//
// The title screen's update (sub_822221A0) counts idle ticks in +36 of the
// object at [0x82814E34] and, at 600 (20 s at 30 Hz), sets the object's
// state (+4) to 5, which loads an attract-mode race. See "The title screen"
// in docs/functions/functions.md.

#include "title_screen.h"

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <atomic>
#include <string>

REXCVAR_DEFINE_BOOL(log_boot_modes, false, "ITC",
                    "Log the front end's state objects as they change, and each tick and "
                    "message handler the first time it runs");
REXCVAR_DEFINE_BOOL(skip_intro, false, "ITC",
                    "Testing: skip the Ubisoft logo movie, and run the Genki logo and the "
                    "title screen's intro without the 30 Hz tick wait (until the first race)");
REXCVAR_DEFINE_INT32(attract_after_s, 0, "ITC",
                     "Testing: seconds the title screen waits before the attract race "
                     "(0 = the game's 20)");

namespace {

constexpr uint32_t kTitleScreenPointer = 0x82814E34;
constexpr uint32_t kIdleTicksOffset = 36;
constexpr int32_t kAttractTicks = 600;
constexpr int32_t kTicksPerSecond = 30;

bool g_first_race_started = false;
bool g_in_movie_wait = false;
// Set by the render thread once it has started the boot movie (see
// sub_820AE958).
std::atomic<bool> g_movie_started{false};

}  // namespace

namespace hooks {

bool TitleScreen_SkipTickWait(bool in_race) {
  if (in_race) {
    g_first_race_started = true;
  }
  return REXCVAR_GET(skip_intro) && !g_first_race_started;
}

}  // namespace hooks

// TitleScreen_Update.
extern "C" REX_FUNC(sub_822221A0) {
  REX_FUNC_PROLOGUE();
  int32_t after = REXCVAR_GET(attract_after_s) * kTicksPerSecond;
  uint32_t title = guest::LoadU32(base, kTitleScreenPointer);
  int32_t before = title ? int32_t(guest::LoadU32(base, title + kIdleTicksOffset)) : 0;
  __imp__sub_822221A0(ctx, base);
  if (after <= 0 || !title || guest::LoadU32(base, kTitleScreenPointer) != title) {
    return;
  }
  // Only after a tick that counted (the counter went up by one): the next
  // one reaches the game's limit.
  int32_t now = int32_t(guest::LoadU32(base, title + kIdleTicksOffset));
  if (now == before + 1 && now >= after && now < kAttractTicks - 1) {
    guest::StoreU32(base, title + kIdleTicksOffset, uint32_t(kAttractTicks - 1));
  }
}

// Scene::Update: r3 = scene. Hooked for --log_boot_modes: logs the front
// end's state objects at [0x82814E2C] and [0x82814E34] as they change.
extern "C" REX_FUNC(sub_820988C8) {
  REX_FUNC_PROLOGUE();
  if (REXCVAR_GET(log_boot_modes)) {
    static std::string last;
    std::string now;
    for (uint32_t pointer : {0x82814E2Cu, 0x82814E30u, kTitleScreenPointer}) {
      uint32_t object = guest::LoadU32(base, pointer);
      now += fmt::format(" [{:08X}]={:08X}:", pointer, object);
      for (uint32_t i = 0; object && i < 24; i += 4) {
        now += fmt::format(" {}", int32_t(guest::LoadU32(base, object + i)));
      }
    }
    if (now != last) {
      REXLOG_INFO("boot modes:{}", now);
      last = now;
    }
  }
  __imp__sub_820988C8(ctx, base);
}

// The boot's movie step, run every tick while a movie plays (the Ubisoft
// logo): ends the step when the movie is skippable (0x826C5BC4) and a button
// is pressed, or when the movie has finished (sub_820A4450 returns 1) and
// this+9608 is 1. With --skip_intro the movie reports finished as soon as
// the render thread has started it.
extern "C" REX_FUNC(sub_82107C58) {
  REX_FUNC_PROLOGUE();
  if (REXCVAR_GET(log_boot_modes)) {
    REXLOG_INFO("boot modes: movie wait, this {:08X} +9608 {}", ctx.r3.u32,
                guest::LoadU32(base, ctx.r3.u32 + 9608));
  }
  g_in_movie_wait = REXCVAR_GET(skip_intro);
  __imp__sub_82107C58(ctx, base);
  g_in_movie_wait = false;
}

// Whether the playing movie has finished (1 = finished).
extern "C" REX_FUNC(sub_820A4450) {
  REX_FUNC_PROLOGUE();
  if (g_in_movie_wait && g_movie_started) {
    ctx.r3.u64 = 1;
    return;
  }
  __imp__sub_820A4450(ctx, base);
}

// The render thread's command that starts the boot movie (1280x720, through
// sub_82481A70); it waits in the movie player (sub_82491C50) for the first
// decoded frame. If the movie step ends the movie before then, the main
// thread waits on the render thread in Boot::EndMovie and the decoder never
// delivers: --skip_intro stalled about 1 boot in 6 that way without the GPU
// plugin. So --skip_intro waits for this to return.
extern "C" REX_FUNC(sub_820AE958) {
  REX_FUNC_PROLOGUE();
  __imp__sub_820AE958(ctx, base);
  g_movie_started = true;
}
