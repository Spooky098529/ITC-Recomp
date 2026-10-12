// Game logic that the game runs from its Draw message, counting frames.
//
// On the original hardware every Draw was a 30 Hz tick, so this logic counts
// ticks; render-only passes (see game_messages.h) draw several times a tick
// and would run it several times faster. Each function here skips its logic
// on render-only passes. Found with --find_per_frame_state.

#include "game_messages.h"
#include "guest.h"

#include <cstring>
#include <unordered_map>
#include <vector>

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

// UI movie update (r3 = movie, r4 = 0 to advance a frame and draw; the HUD,
// including the race start countdown and the markers above opponents). Its
// animation state and its drawing are in the same call, so on a render-only
// pass the call is replayed from the state it had on the logic pass, then
// that pass's resulting state is put back: it draws what the logic pass drew
// and the animation advances once per tick.
namespace {
constexpr uint32_t kMovieBytes = 0xA000;  // furthest field used: ~+0x96C8

struct MovieStates {
  std::vector<uint8_t> before, after;  // around the logic pass's call
};
std::unordered_map<uint32_t, MovieStates> g_movie_states;
}  // namespace

extern "C" REX_FUNC(sub_8211D2F8) {
  REX_FUNC_PROLOGUE();
  uint32_t movie = ctx.r3.u32;
  if (ctx.r4.u32 != 0) {
    __imp__sub_8211D2F8(ctx, base);
    return;
  }
  uint8_t* state = base + movie;
  if (!frame_pacing::GameMessages_IsRenderOnlyPass()) {
    MovieStates& states = g_movie_states[movie];
    states.before.assign(state, state + kMovieBytes);
    __imp__sub_8211D2F8(ctx, base);
    states.after.assign(state, state + kMovieBytes);
    return;
  }
  auto it = g_movie_states.find(movie);
  if (it == g_movie_states.end() || it->second.after.size() != kMovieBytes ||
      std::memcmp(state, it->second.after.data(), kMovieBytes) != 0) {
    __imp__sub_8211D2F8(ctx, base);  // changed since the logic pass: run as is
    return;
  }
  std::memcpy(state, it->second.before.data(), kMovieBytes);
  __imp__sub_8211D2F8(ctx, base);
  std::memcpy(state, it->second.after.data(), kMovieBytes);
}
