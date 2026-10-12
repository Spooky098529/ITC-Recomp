// Game logic that the game runs from its Draw message, counting frames.
//
// On the original hardware every Draw was a 30 Hz tick, so this logic counts
// ticks; render-only passes (see game_messages.h) draw several times a tick
// and would run it several times faster. Each function here skips its logic
// on render-only passes. Found with --find_per_frame_state.

#include "game_messages.h"
#include "guest.h"

#include <rex/cvar.h>

#include <rex/logging.h>

#include <chrono>
#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(per_tick_blink_timers, true, "ITC",
                    "Advance the car blink timers once per tick on render-only passes");
REXCVAR_DEFINE_BOOL(per_tick_ui_movies, true, "ITC",
                    "Advance UI movies once per tick on render-only passes");
REXCVAR_DEFINE_BOOL(redraw_opponent_markers, true, "ITC",
                    "Draw the markers above opponent cars on render-only passes too");
REXCVAR_DEFINE_BOOL(log_replays, false, "ITC",
                    "Log how replays from the logic pass's state went, every 2 s");

// Blink timers (r3 = timer set, from the car's draw, sub_820DDE58): adds one
// to each of four frame counters (+8, +16, +24, +32, 64-bit) and resets each
// past its duration. Drives the flashing sprites above cars.
extern "C" REX_FUNC(sub_820DC958) {
  REX_FUNC_PROLOGUE();
  if (frame_pacing::GameMessages_IsRenderOnlyPass() && REXCVAR_GET(per_tick_blink_timers)) {
    return;
  }
  __imp__sub_820DC958(ctx, base);
}

// Calls whose animation state and drawing are one: on a render-only pass,
// replay the call from the state its object had before the logic pass's same
// call, then put back the state after it. It draws what the logic pass drew,
// and the state advances once per tick. Calls are matched by object and by
// order within the pass (an object can be drawn once per view). If the
// object changed since the logic pass, the call just runs.
namespace {
class ReplayFromLogicPass {
 public:
  ReplayFromLogicPass(const char* name, uint32_t bytes) : name_(name), bytes_(bytes) {}

  template <typename Call>
  void Run(uint8_t* base, uint32_t object, Call&& call) {
    uint64_t pass = frame_pacing::GameMessages_PassNumber();
    if (pass != pass_) {
      pass_ = pass;
      occurrences_.clear();
    }
    Key key{object, occurrences_[object]++};
    uint8_t* state = base + object;
    if (!frame_pacing::GameMessages_IsRenderOnlyPass()) {
      States& states = states_[key];
      states.before.assign(state, state + bytes_);
      call();
      states.after.assign(state, state + bytes_);
      return;
    }
    auto it = states_.find(key);
    if (it == states_.end() || it->second.after.size() != bytes_) {
      Count("no logic pass state", -1);
      call();
      return;
    }
    if (std::memcmp(state, it->second.after.data(), bytes_) != 0) {
      int first = 0;
      while (state[first] == it->second.after[first]) {
        first++;
      }
      Count("changed since the logic pass", first);
      call();
      return;
    }
    Count("replayed", -1);
    std::memcpy(state, it->second.before.data(), bytes_);
    call();
    std::memcpy(state, it->second.after.data(), bytes_);
  }

  void Count(const char* outcome, int offset) {
    if (!REXCVAR_GET(log_replays)) {
      return;
    }
    counts_[{outcome, offset}]++;
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ < std::chrono::seconds(2)) {
      return;
    }
    last_log_ = now;
    for (const auto& [key, n] : counts_) {
      REXLOG_INFO("replay {}: {} {} (first differing byte +{:X})", name_, key.first, n,
                  key.second);
    }
    counts_.clear();
  }

 private:
  struct Key {
    uint32_t object;
    int occurrence;
    bool operator==(const Key&) const = default;
  };
  struct KeyHash {
    size_t operator()(const Key& k) const { return (size_t(k.object) << 8) ^ size_t(k.occurrence); }
  };
  struct States {
    std::vector<uint8_t> before, after;
  };
  const char* name_;
  uint32_t bytes_;
  uint64_t pass_ = ~0ull;
  std::map<std::pair<std::string, int>, int> counts_;
  std::chrono::steady_clock::time_point last_log_{};
  std::unordered_map<Key, States, KeyHash> states_;
  std::unordered_map<uint32_t, int> occurrences_;
};

// UI movies (sub_8211D2F8 with r4 = 0 advances a frame and draws; the HUD,
// including the race start countdown). Furthest field used: ~+0x96C8.
ReplayFromLogicPass g_ui_movies("UI movies", 0xA000);

// Markers above opponent cars (sub_821225D8, r3 = marker, r4 = camera pass):
// a state machine stepped (a counter at +0x150 counts down) and drawn per
// view, which made the marker flicker.
ReplayFromLogicPass g_opponent_markers("opponent markers", 0x400);
}  // namespace

extern "C" REX_FUNC(sub_8211D2F8) {
  REX_FUNC_PROLOGUE();
  if (ctx.r4.u32 != 0 || !REXCVAR_GET(per_tick_ui_movies)) {
    __imp__sub_8211D2F8(ctx, base);
    return;
  }
  g_ui_movies.Run(base, ctx.r3.u32, [&] { __imp__sub_8211D2F8(ctx, base); });
}

namespace {
// The markers drawn on the last logic pass: (marker, camera pass). They're
// updated and recorded during Update, which render-only passes skip, and
// what they record lasts one frame, so render-only passes draw them again,
// before Draw as Update would.
std::vector<std::pair<uint32_t, uint32_t>> g_marker_calls;
uint64_t g_marker_calls_pass = ~0ull;
bool g_redrawing_markers = false;
}  // namespace

extern "C" REX_FUNC(sub_821225D8) {
  REX_FUNC_PROLOGUE();
  if (!frame_pacing::GameMessages_IsRenderOnlyPass() && !g_redrawing_markers) {
    uint64_t pass = frame_pacing::GameMessages_PassNumber();
    if (pass != g_marker_calls_pass) {
      g_marker_calls_pass = pass;
      g_marker_calls.clear();
    }
    g_marker_calls.push_back({ctx.r3.u32, ctx.r4.u32});
  }
  g_opponent_markers.Run(base, ctx.r3.u32, [&] { __imp__sub_821225D8(ctx, base); });
}

namespace frame_pacing {

void PerFrameLogic_RenderOnlyRedraws(PPCContext& ctx, uint8_t* base) {
  if (!REXCVAR_GET(redraw_opponent_markers)) {
    return;
  }
  g_redrawing_markers = true;
  for (const auto& [marker, pass] : g_marker_calls) {
    ctx.r3.u64 = marker;
    ctx.r4.u64 = pass;
    sub_821225D8(ctx, base);
  }
  g_redrawing_markers = false;
}

}  // namespace frame_pacing
