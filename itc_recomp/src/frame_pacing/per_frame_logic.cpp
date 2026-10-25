// Game logic that the game runs from its Draw message, counting frames.
//
// On the original hardware every Draw was a 30 Hz tick, so this logic counts
// ticks; render-only passes (see game_messages.h) draw several times a tick
// and would run it several times faster. Each function here skips its logic
// on render-only passes. Found with --find_per_frame_state.

#include "game_messages.h"
#include "guest.h"
#include "frame_pacer.h"
#include "course_scenery/course_scenery.h"
#include "debug/frame_state_diff.h"
#include "debug/pause_movie_log.h"
#include "hooks/cameras.h"
#include "hooks/ui_movies.h"
#include "interpolation/blend_state.h"

#include "base/flags.h"

#include "base/log.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <deque>
#include <functional>
#include <map>
#include <tuple>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

ITC_FLAG_DECLARE(bool, interpolate_draw_state);
ITC_FLAG_DECLARE(bool, native_course_scenery);
ITC_FLAG_DECLARE(bool, log_pause_movie);
ITC_FLAG_BOOL(per_tick_blink_timers, true, "ITC",
              "Advance the car blink timers once per tick on render-only passes");
ITC_FLAG_BOOL(per_tick_ui_movies, true, "ITC",
              "Advance UI movies once per tick on render-only passes");
ITC_FLAG_BOOL(per_tick_ui_movie_tracks, true, "ITC",
              "On render-only passes, don't apply UI movies' animation tracks that add "
              "to a value the update doesn't put back (they moved ~5x as far: the pause "
              "sub-menus' arrow)");
ITC_FLAG_BOOL(per_tick_changed_ui_movies, true, "ITC",
              "On render-only passes, draw UI movies changed since the logic pass "
              "without advancing them (the pause sub-menus' arrow)");
ITC_FLAG_BOOL(per_tick_texture_animations, true, "ITC",
              "Advance texture animations (flashing lights) once per tick on "
              "render-only passes");
ITC_FLAG_BOOL(per_tick_minimap_blink, true, "ITC",
              "Advance the minimap's blink clock once per tick on render-only passes");
ITC_FLAG_BOOL(per_tick_play_time, true, "ITC",
              "Count play time once per tick on render-only passes");
ITC_FLAG_BOOL(log_replays, false, "ITC",
              "Log how replays from the logic pass's state went, every 2 s");

// Blink timers (r3 = timer set, from the car's draw, sub_820DDE58): adds one
// to each of four frame counters (+8, +16, +24, +32, 64-bit) and resets each
// past its duration. Drives the flashing sprites above cars.
extern "C" REX_FUNC(sub_820DC958) {
  REX_FUNC_PROLOGUE();
  if (frame_pacing::GameMessages_IsRenderOnlyPass() && ITC_FLAG(per_tick_blink_timers)) {
    return;
  }
  __imp__sub_820DC958(ctx, base);
}

// Texture animations (sub_820F11A8, from the draw step part sub_820F6068):
// advances every flipbook animation in the set at r3+1060 (sub_82146B20,
// one sub_82146AC0 each). Drives the flashing building lights; the light
// groups pick their frame's texture with sub_82146B80.
extern "C" REX_FUNC(sub_820F11A8) {
  REX_FUNC_PROLOGUE();
  if (frame_pacing::GameMessages_IsRenderOnlyPass() &&
      ITC_FLAG(per_tick_texture_animations)) {
    return;
  }
  // The advance itself is the course scenery's (src/course_scenery/).
  if (ITC_FLAG(native_course_scenery)) {
    ctx.r3.u64 = course_scenery::AdvanceTextureAnimations(base, ctx.r3.u32);
    return;
  }
  __imp__sub_820F11A8(ctx, base);
}

// Minimap blink clock (sub_820F5740, from the draw step part sub_820F6068):
// after drawing, adds one to 0x826C5AA4, which wraps after 4 and then adds
// one to 0x826C5AA8, which wraps after 5. 0x826C5AA8 is the phase of the
// player's triangle flashing where it overlaps other cars' on the minimap.
// On a render-only pass, both are put back after the call, so it draws with
// the logic pass's phase and the clock advances once per tick.
extern "C" REX_FUNC(sub_820F5740) {
  REX_FUNC_PROLOGUE();
  constexpr uint32_t kBlinkFrames = 0x826C5AA4, kBlinkPhase = 0x826C5AA8;
  if (!frame_pacing::GameMessages_IsRenderOnlyPass() || !ITC_FLAG(per_tick_minimap_blink)) {
    __imp__sub_820F5740(ctx, base);
    return;
  }
  uint32_t frames = guest::LoadU32(base, kBlinkFrames);
  uint32_t phase = guest::LoadU32(base, kBlinkPhase);
  __imp__sub_820F5740(ctx, base);
  guest::StoreU32(base, kBlinkFrames, frames);
  guest::StoreU32(base, kBlinkPhase, phase);
}

// Play time (sub_82107910, from Draw): adds one to the frame counter at
// 0x826C5B90, which sub_821109B8 turns into hours, minutes and seconds at
// 30 a second. On a render-only pass, take the one back first, so the
// counter advances once per tick and the call sees the logic pass's value.
extern "C" REX_FUNC(sub_82107910) {
  REX_FUNC_PROLOGUE();
  constexpr uint32_t kPlayTimeFrames = 0x826C5B90;
  if (frame_pacing::GameMessages_IsRenderOnlyPass() && ITC_FLAG(per_tick_play_time)) {
    guest::StoreU32(base, kPlayTimeFrames, guest::LoadU32(base, kPlayTimeFrames) - 1);
  }
  __imp__sub_82107910(ctx, base);
}

// Calls whose animation state and drawing are one: on a render-only pass,
// replay the call from the state its object had before the logic pass's same
// call, then put back the state after it. It draws what the logic pass drew,
// and the state advances once per tick. Calls are matched by object and by
// order within the pass (an object can be drawn once per view). If the
// object changed since the logic pass, or the logic pass had no such call,
// the call runs from the object's current state, which is then put back.
namespace {
// What the last ReplayFromLogicPass::Run did, for --log_pause_movie.
const char* g_replay_outcome = "";
int g_replay_occurrence = -1;

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
    g_replay_occurrence = key.occurrence;
    uint8_t* state = base + object;
    if (!frame_pacing::GameMessages_IsRenderOnlyPass()) {
      States& states = states_[key];
      g_replay_outcome = "logic pass";
      states.before.assign(state, state + bytes_);
      call();
      states.after.assign(state, state + bytes_);
      return;
    }
    auto it = states_.find(key);
    if (it == states_.end() || it->second.after.size() != bytes_) {
      Count("no logic pass state", -1);
      g_replay_outcome = "no logic pass state";
      CallFromCurrentState(state, call);
      return;
    }
    if (std::memcmp(state, it->second.after.data(), bytes_) != 0) {
      int first = 0;
      while (state[first] == it->second.after[first]) {
        first++;
      }
      Count("changed since the logic pass", first);
      g_replay_outcome = "changed since the logic pass";
      CallFromCurrentState(state, call);
      return;
    }
    Count("replayed", -1);
    g_replay_outcome = "replayed";
    std::memcpy(state, it->second.before.data(), bytes_);
    call();
    std::memcpy(state, it->second.after.data(), bytes_);
  }

  // The object changed after the logic pass's call (an animation started
  // by code later in that pass: the pause sub-menus' arrow), or has no call
  // there to match: run from the state it has now and put that back, so it
  // still advances once a tick (--per_tick_changed_ui_movies; without it the
  // call just runs, advancing on every pass).
  template <typename Call>
  void CallFromCurrentState(uint8_t* state, Call&& call) {
    if (!ITC_FLAG(per_tick_changed_ui_movies)) {
      call();
      return;
    }
    current_.assign(state, state + bytes_);
    call();
    std::memcpy(state, current_.data(), bytes_);
  }

  void Count(const char* outcome, int offset) {
    if (!ITC_FLAG(log_replays)) {
      return;
    }
    counts_[{outcome, offset}]++;
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ < std::chrono::seconds(2)) {
      return;
    }
    last_log_ = now;
    for (const auto& [key, n] : counts_) {
      ITC_LOG_INFO("replay {}: {} {} (first differing byte +{:X})", name_, key.first, n,
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
  std::vector<uint8_t> current_;
};

// UI movies (sub_8211D2F8 with r4 = 0 advances a frame and draws; the HUD,
// including the race start countdown). Furthest field used: ~+0x96C8.
ReplayFromLogicPass g_ui_movies("UI movies", 0xA000);

}  // namespace

namespace {
// The update's work: natively with --native_ui_movies (src/hooks/ui_movies.cpp).
void UpdateUiMovie(PPCContext& ctx, uint8_t* base) {
  if (ctx.r4.u32 != 0 || !ITC_FLAG(per_tick_ui_movies)) {
    ui_movies_hooks::Update(ctx, base);
    return;
  }
  g_ui_movies.Run(base, ctx.r3.u32, [&] { ui_movies_hooks::Update(ctx, base); });
}
}  // namespace

// UI movie animation tracks: each movie update applies every playing
// animation (sub_825A08E0, keyframe interpolation at the animation's time),
// and sub_825A0548 writes one track's value into its node, outside the bytes
// the replay restores. A track whose descriptor's byte +8 is 0 adds its
// value to the node instead of setting it. Most nodes are put back to the
// movie's rest pose at the end of the update (sub_82119B80, from 8211D7CC),
// so their adds start again from it every update; skipping those on
// render-only passes drew the rest pose there (the arrow ghosting between
// its slide and its rest position). Adds on values the update doesn't put
// back were applied again on every render-only update: at 144 fps the pause
// sub-menus' arrow slid ~5x as far, off its item. So on a logic pass each
// adding track's target is noted as put back or not (its value at the end
// of the update is the one before the track), and on render-only passes
// only adds on targets not seen put back are skipped.
//
// sub_825A0548: r3 = the node, r4 = the track, whose word +0 is a property
// id; the property's descriptor (from sub_8259FE68) is entry id of a 12-byte
// table at 0x82970C28, filled on first use (byte 0x82970C20 set): +0 the node
// offset, +4 the type (0 float, 1 and 2 integer, 3 and up write nothing),
// byte +8 set (non-zero) or add (0), byte +9 indirect. The one word written
// is node + offset, plus, when indirect, sub_8259D600([node + 112], [track +
// 12]) = 16 * ([node + 112] + 2 * ([track + 12] + 4)).
namespace {
uint32_t g_ui_movie = 0;  // the UI movie being updated, for --log_pause_movie
constexpr uint32_t kTrackPropertiesReady = 0x82970C20;
constexpr uint32_t kTrackProperties = 0x82970C28;
constexpr uint32_t kTrackPropertyCount = 64;

struct AddedTarget {
  uint32_t target, before, property;
};
// The adding tracks' targets of each UI movie update in progress (updates
// can nest), with their values before the track; null outside an update.
std::deque<std::vector<AddedTarget>> g_added_by_depth;
size_t g_update_depth = 0;
std::vector<AddedTarget>* g_added = nullptr;
// Targets of adding tracks the last logic pass's update put back.
std::unordered_set<uint32_t> g_put_back_targets;

uint32_t TrackTarget(uint8_t* base, uint32_t node, uint32_t track, uint32_t descriptor) {
  uint32_t target = node + guest::LoadU32(base, descriptor);
  if (guest::LoadU8(base, descriptor + 9) != 0) {
    target += 16 * (guest::LoadU32(base, node + 112) +
                    2 * (guest::LoadU32(base, track + 12) + 4));
  }
  return target;
}

// At the end of a logic pass's update: a target is put back when it holds
// its value from before its first track in the update (walked from the
// end, so that track decides).
void NotePutBackTargets(uint8_t* base, uint32_t movie, const std::vector<AddedTarget>& added) {
  for (auto it = added.rbegin(); it != added.rend(); ++it) {
    bool put_back = guest::LoadU32(base, it->target) == it->before;
    bool changed = put_back ? g_put_back_targets.insert(it->target).second
                            : g_put_back_targets.erase(it->target) != 0;
    if (changed && ITC_FLAG(log_pause_movie)) {
      debug::PauseMovieLog_PutBack(movie, it->property, it->target, put_back);
    }
  }
}
}  // namespace

extern "C" REX_FUNC(sub_825A0548) {
  REX_FUNC_PROLOGUE();
  if (guest::LoadU8(base, kTrackPropertiesReady) == 0) {
    ui_movies_hooks::ApplyTrack(ctx, base);
    return;
  }
  uint32_t node = ctx.r3.u32, track = ctx.r4.u32;
  uint32_t property = guest::LoadU32(base, track);
  uint32_t descriptor = kTrackProperties + 12 * property;
  bool adds = property < kTrackPropertyCount && guest::LoadU8(base, descriptor + 8) == 0;
  uint32_t type = property < kTrackPropertyCount ? guest::LoadU32(base, descriptor + 4) : 3;
  bool noted = adds && g_added != nullptr;
  bool log = ITC_FLAG(log_pause_movie) && debug::PauseMovieLog_TracksWanted();
  if (type >= 3 || (!noted && !log)) {
    ui_movies_hooks::ApplyTrack(ctx, base);
    return;
  }
  uint32_t target = TrackTarget(base, node, track, descriptor);
  uint32_t before = guest::LoadU32(base, target);
  bool skip = false;
  if (noted) {
    if (frame_pacing::GameMessages_IsRenderOnlyPass()) {
      skip = !g_put_back_targets.contains(target);
    } else {
      g_added->push_back({target, before, property});
    }
  }
  if (skip) {
    if (ITC_FLAG(log_pause_movie)) {
      debug::PauseMovieLog_Skipped(g_ui_movie, property, target);
    }
  } else {
    ui_movies_hooks::ApplyTrack(ctx, base);
  }
  if (log) {
    debug::PauseMovieLog_Track(g_ui_movie, node, property, type, adds, target, before,
                               guest::LoadU32(base, target), skip);
  }
}

extern "C" REX_FUNC(sub_8211D2F8) {
  REX_FUNC_PROLOGUE();
  if (ui_movies_hooks::g_update_callee_stand_in != nullptr &&
      ui_movies_hooks::g_update_callee_stand_in(0x8211D2F8, ctx, base)) {
    return;
  }
  uint32_t movie = ctx.r3.u32, r4 = ctx.r4.u32, caller = uint32_t(ctx.lr);
  uint32_t movie_before = g_ui_movie;
  std::vector<AddedTarget>* added_before = g_added;
  bool tracks = ITC_FLAG(per_tick_ui_movie_tracks);
  if (tracks) {
    if (g_update_depth == g_added_by_depth.size()) {
      g_added_by_depth.emplace_back();
    }
    g_added = &g_added_by_depth[g_update_depth++];
    g_added->clear();
  }
  bool log = ITC_FLAG(log_pause_movie);
  if (log) {
    g_ui_movie = movie;
    debug::PauseMovieLog_Before(base, movie, r4, caller);
    g_replay_outcome = "not replayed";
    g_replay_occurrence = -1;
  }
  UpdateUiMovie(ctx, base);
  if (log) {
    debug::PauseMovieLog_After(base, movie, caller, g_replay_outcome, g_replay_occurrence);
  }
  if (tracks) {
    if (!frame_pacing::GameMessages_IsRenderOnlyPass()) {
      NotePutBackTargets(base, movie, *g_added);
    }
    g_update_depth--;
  }
  g_added = added_before;
  g_ui_movie = movie_before;
}

// Race overlays (sub_8215DEA0, from Update handler sub_8215E150; r3 = a list
// of 25 objects): updates each (vtable slot 4), then records its drawing
// (slot 5). The marker above opponent cars is one, recorded only during
// Update and lasting a frame, so it showed on logic passes only (flicker).
// Render-only passes deliver Update to this handler alone (see
// game_messages.cpp), and here only the recording runs. Both loops are run
// here (as the original does) to give each its own numbering for camera
// products.
//
// The overlays place themselves with the camera passes as game logic left
// them. By a render-only pass's Update, Draw has reused the passes for other
// views, so the logic pass's camera passes are put back around the
// recording.
namespace {
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kCameraPassesBytes = 21 * 592;
std::vector<uint8_t> g_overlay_cameras;  // as of the logic pass's overlays
}  // namespace

extern "C" REX_FUNC(sub_8215DEA0) {
  REX_FUNC_PROLOGUE();
  constexpr uint32_t kOverlayCount = 25;
  constexpr uint32_t kUpdateSlot = 16;  // vtable offset of slot 4
  constexpr uint32_t kRecordSlot = 20;  // vtable offset of slot 5
  // Numbering contexts for camera products (blend_state.h): the recording
  // loop numbers its products the same on every pass, whether or not the
  // update loop ran before it, so they pair with the logic pass's.
  constexpr uint32_t kUpdateContext = 0x8215E151;
  constexpr uint32_t kRecordContext = 0x8215E150;
  uint32_t list = ctx.r3.u32;
  bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
  uint8_t* cameras = base + kCameraPasses;
  std::vector<uint8_t> current;
  if (!render_only) {
    g_overlay_cameras.assign(cameras, cameras + kCameraPassesBytes);
  } else {
    current.assign(cameras, cameras + kCameraPassesBytes);
    if (g_overlay_cameras.size() == kCameraPassesBytes) {
      std::memcpy(cameras, g_overlay_cameras.data(), kCameraPassesBytes);
    }
  }
  // The original's stack frame (stwu r1,-112(r1)), so the overlays' stack
  // temporaries are at the same addresses on every pass (camera products are
  // keyed by their operands' addresses).
  guest::StackFrame frame(ctx, 112);
  auto for_each_overlay = [&](uint32_t slot) {
    for (uint32_t i = 0; i < kOverlayCount; i++) {
      uint32_t overlay = guest::LoadU32(base, list + 4 * i);
      if (overlay == 0) {
        continue;
      }
      ctx.r3.u64 = overlay;
      REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, overlay) + slot));
    }
  };
  if (!render_only) {
    interpolation::BlendState_SetHandler(kUpdateContext);
    for_each_overlay(kUpdateSlot);
  }
  interpolation::BlendState_SetHandler(kRecordContext);
  for_each_overlay(kRecordSlot);
  interpolation::BlendState_LogOverlayAllocations(base);
  if (render_only) {
    std::memcpy(cameras, current.data(), kCameraPassesBytes);
  }
}

// The rival intro cutscene (Update handler 0x82159948, a jump to
// sub_821594B8): it steps the cutscene and, deep inside (a virtual call,
// sub_821561F8, sub_82155A28), draws the rival's logo with sub_82211A28 (r3
// = the logo's movie entry: +4 flags, bit 8 shown, bit 16 finished; +8 the
// movie's id), which looks up the movie and advances and draws it
// (sub_8211E1B8, r4 = 0, from 0x82211A74), and when it has ended calls
// sub_8211DF20 and sets bit 16. During Update only, so the logo showed on
// logic passes only (flicker). Render-only passes deliver Update to this
// handler too (--redraw_rival_logo, game_messages.cpp); there the cutscene
// isn't stepped, only the logo's draw is run, with the entry the logic pass
// drew. The UI movie replay (sub_8211D2F8 above) keeps the movie advancing
// once a tick.
namespace {
bool g_in_rival_cutscene = false;
uint32_t g_rival_logo = 0;  // the entry the last logic pass drew, or 0
}  // namespace

extern "C" REX_FUNC(sub_82211A28) {
  REX_FUNC_PROLOGUE();
  if (g_in_rival_cutscene) {
    g_rival_logo = ctx.r3.u32;
  }
  __imp__sub_82211A28(ctx, base);
}

extern "C" REX_FUNC(sub_821594B8) {
  REX_FUNC_PROLOGUE();
  if (!frame_pacing::GameMessages_IsRenderOnlyPass()) {
    g_rival_logo = 0;
    g_in_rival_cutscene = true;
    __imp__sub_821594B8(ctx, base);
    g_in_rival_cutscene = false;
    return;
  }
  if (g_rival_logo == 0) {
    return;
  }
  // The original's stack frame (stwu r1,-176(r1)).
  guest::StackFrame frame(ctx, 176);
  ctx.r3.u64 = g_rival_logo;
  ctx.lr = 0x82155BE0;
  sub_82211A28(ctx, base);
}

// World-space sprite lists (sub_82128750: r3 = list, r4 = camera; from the
// race overlays: the light trails behind headlights and tail lights). The list holds up to
// 500 sprites, count at +0x109F0; sprite i's four corners (x, y, z, pad) at
// +80 + 64 * i, written by game logic once a tick. The recorder writes them
// into the command list as vertices, drawn through the camera, which is
// blended between ticks; left at the tick's corners, the trails were missing
// at high frame rates.
// The corners are blended between their last two ticks, by index, while it
// records, then put back. The list is shared: each overlay fills it and
// records it in turn (the marker, then particles), so each recording is
// matched with the logic pass's by its order in the pass.
namespace {
constexpr uint32_t kSpriteCountOffset = 0x109F0;
constexpr uint32_t kSpriteCornersOffset = 80;
constexpr uint32_t kSpriteStride = 64;
constexpr uint32_t kMaxSprites = 500;
constexpr float kMaxSpriteStep = 10.0f;  // further in a tick is a new sprite

struct SpriteList {
  std::vector<float> before, now;  // corners after the last two ticks
};
struct SpriteListKey {
  uint32_t list;
  int occurrence;  // recordings of the list earlier in the pass
  bool operator==(const SpriteListKey&) const = default;
};
struct SpriteListKeyHash {
  size_t operator()(const SpriteListKey& k) const {
    return (size_t(k.list) << 10) ^ size_t(k.occurrence);
  }
};
std::unordered_map<SpriteListKey, SpriteList, SpriteListKeyHash> g_sprite_lists;
uint64_t g_sprite_pass = ~0ull;
std::unordered_map<uint32_t, int> g_sprite_occurrences;
}  // namespace

ITC_FLAG_BOOL(blend_world_sprites, true, "ITC",
              "Blend world-space sprites (light trails) between ticks");

ITC_FLAG_BOOL(log_world_sprites, false, "ITC",
              "Log the world-space sprite lists as recorded, every 2 s");

void PerFrameLogic_RecordWorldSprites(uint8_t* base, uint32_t list,
                                      const std::function<void()>& record) {
  if (ITC_FLAG(log_world_sprites)) {
    static std::chrono::steady_clock::time_point last{};
    static int lines = 0;
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::seconds(2)) {
      last = now;
      lines = 0;
    }
    if (lines < 40) {
      lines++;
      uint32_t n = guest::LoadU32(base, list + kSpriteCountOffset);
      auto f = [&](uint32_t a) { return guest::LoadFloat(base, a); };
      auto it = g_sprite_lists.find({list, g_sprite_occurrences[list]});
      SpriteList s = it != g_sprite_lists.end() ? it->second : SpriteList{};
      ITC_LOG_INFO("world sprites: list {:08X} pass {} {} in race {} amount {:.2f} count {} "
                   "matrix row3 ({:.5g} {:.5g} {:.5g}) corner0 ({:.5g} {:.5g} {:.5g}) "
                   "corner1 ({:.5g} {:.5g} {:.5g}) kept {}/{}",
                   list, frame_pacing::GameMessages_PassNumber(),
                   frame_pacing::GameMessages_IsRenderOnlyPass() ? "render-only" : "logic",
                   frame_pacing::GameMessages_InRace(),
                   frame_pacing::GetFramePacer().TickProgress(), n, f(list + 48), f(list + 52),
                   f(list + 56), f(list + 80), f(list + 84), f(list + 88), f(list + 96),
                   f(list + 100), f(list + 104), s.before.size() / 16, s.now.size() / 16);
    }
  }
  if (!ITC_FLAG(interpolate_draw_state) || !ITC_FLAG(blend_world_sprites) ||
      !frame_pacing::GameMessages_InRace()) {
    record();
    return;
  }
  uint32_t count = std::min(guest::LoadU32(base, list + kSpriteCountOffset), kMaxSprites);
  auto corner = [&](uint32_t i, uint32_t k) {
    return list + kSpriteCornersOffset + kSpriteStride * i + 4 * k;  // k: 16 floats
  };
  uint64_t pass = frame_pacing::GameMessages_PassNumber();
  if (pass != g_sprite_pass) {
    g_sprite_pass = pass;
    g_sprite_occurrences.clear();
  }
  SpriteList& sprites = g_sprite_lists[{list, g_sprite_occurrences[list]++}];
  if (!frame_pacing::GameMessages_IsRenderOnlyPass()) {
    sprites.before = std::move(sprites.now);
    sprites.now.resize(count * 16);
    for (uint32_t i = 0; i < count; i++) {
      for (uint32_t k = 0; k < 16; k++) {
        sprites.now[i * 16 + k] = guest::LoadFloat(base, corner(i, k));
      }
    }
  }
  float amount = frame_pacing::GetFramePacer().TickProgress();
  std::vector<std::pair<uint32_t, float>> saved;
  uint32_t paired = std::min<uint32_t>(
      count, uint32_t(std::min(sprites.before.size(), sprites.now.size()) / 16));
  for (uint32_t i = 0; i < paired; i++) {
    const float* a = &sprites.before[i * 16];
    const float* b = &sprites.now[i * 16];
    bool same = true, near = true;
    for (uint32_t k = 0; k < 16; k++) {
      // Only while the list holds this tick's corners.
      same = same && guest::LoadFloat(base, corner(i, k)) == b[k];
      near = near && std::isfinite(a[k]) && std::abs(b[k] - a[k]) <= kMaxSpriteStep;
    }
    if (!same || !near) {
      continue;
    }
    for (uint32_t k = 0; k < 16; k++) {
      if (k % 4 == 3) {
        continue;
      }
      saved.push_back({corner(i, k), b[k]});
      guest::StoreFloat(base, corner(i, k), a[k] + (b[k] - a[k]) * amount);
    }
  }
  if (ITC_FLAG(log_world_sprites) && !saved.empty()) {
    static std::chrono::steady_clock::time_point last{};
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::milliseconds(500)) {
      last = now;
      for (uint32_t i = 0; i < paired; i++) {
        const float* a = &sprites.before[i * 16];
        const float* b = &sprites.now[i * 16];
        ITC_LOG_INFO("world sprites: list {:08X} sprite {}/{} amount {:.2f} before ({:.5g} {:.5g} "
                     "{:.5g}) ({:.5g} {:.5g} {:.5g}) now ({:.5g} {:.5g} {:.5g}) ({:.5g} {:.5g} {:.5g})",
                     list, i, count, amount, a[0], a[1], a[2], a[4], a[5], a[6], b[0], b[1], b[2],
                     b[4], b[5], b[6]);
      }
      auto f = [&](uint32_t o) { return guest::LoadFloat(base, list + o); };
      ITC_LOG_INFO("world sprites: list {:08X} matrix {:.4g} {:.4g} {:.4g} | {:.4g} {:.4g} {:.4g} "
                   "| {:.4g} {:.4g} {:.4g} | {:.5g} {:.5g} {:.5g}",
                   list, f(0), f(4), f(8), f(16), f(20), f(24), f(32), f(36), f(40), f(48), f(52),
                   f(56));
    }
  }
  record();
  for (const auto& [address, value] : saved) {
    guest::StoreFloat(base, address, value);
  }
}

// World-space sprite recorder (sub_82128750: r3 = sprite list, r4 = camera;
// the light trails): blends the corners around the original.
extern "C" REX_FUNC(sub_82128750) {
  REX_FUNC_PROLOGUE();
  if (debug::IsHidden(0x82128750)) {
    return;
  }
  PerFrameLogic_RecordWorldSprites(base, ctx.r3.u32, [&] { __imp__sub_82128750(ctx, base); });
}

// Diagnostics (--log_camera_pass_records): which camera passes are recorded
// (sub_820A3978: r3 = camera pass) on logic and on render-only passes, and
// from where, every 2 s, in races.
ITC_FLAG_BOOL(log_camera_pass_records, false, "ITC",
              "Log which camera passes are recorded on logic and render-only passes");

extern "C" REX_FUNC(sub_820A3978) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(log_camera_pass_records) && frame_pacing::GameMessages_InRace()) {
    static std::map<std::tuple<uint32_t, uint32_t, uint32_t>, std::pair<int, int>> counts;
    static auto last = std::chrono::steady_clock::now();
    static uint64_t logic_start = 0, render_start = 0;
    uint32_t handler, message;
    frame_pacing::GameMessages_CurrentHandler(handler, message);
    auto& entry = counts[{ctx.r3.u32, handler, message}];
    (frame_pacing::GameMessages_IsRenderOnlyPass() ? entry.second : entry.first)++;
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::seconds(2)) {
      last = now;
      uint64_t logic, render;
      frame_pacing::GameMessages_PassCounts(logic, render);
      for (const auto& [key, n] : counts) {
        auto [pass, handler, message] = key;
        ITC_LOG_INFO("camera pass record: pass {:08X} (type {}) in handler {:08X} message {}: {} "
                     "on {} logic, {} on {} render-only passes",
                     pass, guest::LoadU32(base, pass + 584), handler, int32_t(message), n.first,
                     logic - logic_start, n.second, render - render_start);
      }
      counts.clear();
      logic_start = logic;
      render_start = render;
    }
  }
  // The work natively (src/cameras/cameras.cpp, --native_cameras) or
  // translated.
  if (!hooks::CameraPassUpdate(ctx, base)) {
    __imp__sub_820A3978(ctx, base);
  }
}

// Single world-space sprites (sub_821281A8: r3 = sprite, r4 = camera; the
// marker above opponents, from the race overlays). Like the sprite lists
// above: c0 is the sprite's matrix (+0) times the camera, and its four
// corners (x, y, z, pad) at +80 become the vertices. The corners are
// blended between their last two ticks while it records, matched by sprite
// and order in the pass, then put back.
ITC_FLAG_BOOL(log_single_sprites, false, "ITC",
              "Log each single world-space sprite recorded, 40 lines every 2 s");

namespace {
struct SingleSpriteKey {
  uint32_t sprite;
  uint32_t caller;
  int occurrence;
  bool operator==(const SingleSpriteKey&) const = default;
};
struct SingleSpriteKeyHash {
  size_t operator()(const SingleSpriteKey& k) const {
    return (size_t(k.sprite) << 20) ^ (size_t(k.caller) << 4) ^ size_t(k.occurrence);
  }
};
struct SingleSprite {
  std::array<float, 16> before{}, now{};
  int ticks = 0;
};
std::unordered_map<SingleSpriteKey, SingleSprite, SingleSpriteKeyHash> g_single_sprites;
uint64_t g_single_sprite_pass = ~0ull;
std::map<std::pair<uint32_t, uint32_t>, int> g_single_sprite_occurrences;
}  // namespace

extern "C" REX_FUNC(sub_821281A8) {
  REX_FUNC_PROLOGUE();
  uint32_t sprite = ctx.r3.u32, caller = uint32_t(ctx.lr);
  bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
  uint64_t pass = frame_pacing::GameMessages_PassNumber();
  if (pass != g_single_sprite_pass) {
    g_single_sprite_pass = pass;
    g_single_sprite_occurrences.clear();
  }
  SingleSpriteKey key{sprite, caller, g_single_sprite_occurrences[{sprite, caller}]++};
  std::array<float, 16> corners;
  for (uint32_t k = 0; k < 16; k++) {
    corners[k] = guest::LoadFloat(base, sprite + kSpriteCornersOffset + 4 * k);
  }
  SingleSprite& state = g_single_sprites[key];
  if (!render_only) {
    state.before = state.now;
    state.now = corners;
    state.ticks++;
  }
  float amount = frame_pacing::GetFramePacer().TickProgress();
  bool blend = ITC_FLAG(interpolate_draw_state) && ITC_FLAG(blend_world_sprites) &&
               frame_pacing::GameMessages_InRace() && state.ticks >= 2 && corners == state.now;
  for (uint32_t k = 0; blend && k < 16; k++) {
    blend = std::isfinite(state.before[k]) &&
            std::abs(state.now[k] - state.before[k]) <= kMaxSpriteStep;
  }
  if (ITC_FLAG(log_single_sprites)) {
    static std::chrono::steady_clock::time_point last{};
    static int lines = 0;
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::seconds(2)) {
      last = now;
      lines = 0;
    }
    if (lines++ < 40) {
      uint32_t handler, message;
      frame_pacing::GameMessages_CurrentHandler(handler, message);
      auto f = [&](uint32_t o) { return guest::LoadFloat(base, sprite + o); };
      ITC_LOG_INFO("single sprite: pass {} {} amount {:.2f} sprite {:08X} from {:08X} #{} handler "
                   "{:08X} message {} camera {:08X} matrix row3 ({:.5g} {:.5g} {:.5g}) corner0 "
                   "({:.5g} {:.5g} {:.5g}) before ({:.5g} {:.5g} {:.5g}) blend {}",
                   pass, render_only ? "render-only" : "logic", amount, sprite, caller,
                   key.occurrence, handler, int32_t(message), ctx.r4.u32, f(48), f(52), f(56),
                   corners[0], corners[1], corners[2], state.before[0], state.before[1],
                   state.before[2], blend);
    }
  }
  if (!blend) {
    __imp__sub_821281A8(ctx, base);
    return;
  }
  for (uint32_t k = 0; k < 16; k++) {
    if (k % 4 != 3) {
      guest::StoreFloat(base, sprite + kSpriteCornersOffset + 4 * k,
                        state.before[k] + (state.now[k] - state.before[k]) * amount);
    }
  }
  __imp__sub_821281A8(ctx, base);
  for (uint32_t k = 0; k < 16; k++) {
    guest::StoreFloat(base, sprite + kSpriteCornersOffset + 4 * k, corners[k]);
  }
}
