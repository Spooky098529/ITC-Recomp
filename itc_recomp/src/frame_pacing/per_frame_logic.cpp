// Game logic that runs on every main loop pass (from the Draw message or a
// tick handler), counting frames.
//
// On the original hardware every pass was a 30 Hz tick, so this logic counts
// ticks; render-only passes (see game_messages.h) run several times a tick
// and would run it several times faster. Each function here skips its logic
// on render-only passes. Found with --find_per_frame_state.

#include "game_messages.h"
#include "guest.h"

// Blink timers (r3 = timer set, from the car's draw, sub_820DDE58): adds one
// to each of four frame counters (+8, +16, +24, +32, 64-bit) and resets each
// past its duration. Drives the flashing sprites above cars.
extern "C" REX_FUNC(sub_820DC958) {
  REX_FUNC_PROLOGUE();
  if (frame_pacing::GameMessages_IsRenderOnlyPass()) {
    return;
  }
  __imp__sub_820DC958(ctx, base);
}
