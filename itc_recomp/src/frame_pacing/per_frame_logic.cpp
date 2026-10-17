// Game logic that the game runs from its Draw message, counting frames.
//
// On the original hardware every Draw was a 30 Hz tick, so this logic counts
// ticks; render-only passes (see game_messages.h) draw several times a tick
// and would run it several times faster. Each function here skips its logic
// on render-only passes. Found with --find_per_frame_state.

#include "game_messages.h"
#include "guest.h"
#include "frame_pacer.h"
#include "debug/frame_state_diff.h"
#include "interpolation/blend_state.h"

#include <rex/cvar.h>

#include <rex/logging.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <functional>
#include <map>
#include <tuple>
#include <string>
#include <unordered_map>
#include <vector>

REXCVAR_DECLARE(bool, interpolate_draw_state);
REXCVAR_DEFINE_BOOL(per_tick_blink_timers, true, "ITC",
                    "Advance the car blink timers once per tick on render-only passes");
REXCVAR_DEFINE_BOOL(per_tick_ui_movies, true, "ITC",
                    "Advance UI movies once per tick on render-only passes");
REXCVAR_DEFINE_BOOL(per_tick_texture_animations, true, "ITC",
                    "Advance texture animations (flashing lights) once per tick on "
                    "render-only passes");
REXCVAR_DEFINE_BOOL(per_tick_minimap_blink, true, "ITC",
                    "Advance the minimap's blink clock once per tick on render-only passes");
REXCVAR_DEFINE_BOOL(per_tick_play_time, true, "ITC",
                    "Count play time once per tick on render-only passes");
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

// Texture animations (sub_820F11A8, from the draw step part sub_820F6068):
// advances every flipbook animation in the set at r3+1060 (sub_82146B20,
// one sub_82146AC0 each). Drives the flashing building lights; the light
// groups pick their frame's texture with sub_82146B80.
extern "C" REX_FUNC(sub_820F11A8) {
  REX_FUNC_PROLOGUE();
  if (frame_pacing::GameMessages_IsRenderOnlyPass() &&
      REXCVAR_GET(per_tick_texture_animations)) {
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
  if (!frame_pacing::GameMessages_IsRenderOnlyPass() || !REXCVAR_GET(per_tick_minimap_blink)) {
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
  if (frame_pacing::GameMessages_IsRenderOnlyPass() && REXCVAR_GET(per_tick_play_time)) {
    guest::StoreU32(base, kPlayTimeFrames, guest::LoadU32(base, kPlayTimeFrames) - 1);
  }
  __imp__sub_82107910(ctx, base);
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

}  // namespace

extern "C" REX_FUNC(sub_8211D2F8) {
  REX_FUNC_PROLOGUE();
  if (ctx.r4.u32 != 0 || !REXCVAR_GET(per_tick_ui_movies)) {
    __imp__sub_8211D2F8(ctx, base);
    return;
  }
  g_ui_movies.Run(base, ctx.r3.u32, [&] { __imp__sub_8211D2F8(ctx, base); });
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

REXCVAR_DEFINE_BOOL(blend_world_sprites, true, "ITC",
                    "Blend world-space sprites (light trails) between ticks");

REXCVAR_DEFINE_BOOL(log_world_sprites, false, "ITC",
                    "Log the world-space sprite lists as recorded, every 2 s");

void PerFrameLogic_RecordWorldSprites(uint8_t* base, uint32_t list,
                                      const std::function<void()>& record) {
  if (REXCVAR_GET(log_world_sprites)) {
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
      REXLOG_INFO("world sprites: list {:08X} pass {} {} in race {} amount {:.2f} count {} "
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
  if (!REXCVAR_GET(interpolate_draw_state) || !REXCVAR_GET(blend_world_sprites) ||
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
  if (REXCVAR_GET(log_world_sprites) && !saved.empty()) {
    static std::chrono::steady_clock::time_point last{};
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::milliseconds(500)) {
      last = now;
      for (uint32_t i = 0; i < paired; i++) {
        const float* a = &sprites.before[i * 16];
        const float* b = &sprites.now[i * 16];
        REXLOG_INFO("world sprites: list {:08X} sprite {}/{} amount {:.2f} before ({:.5g} {:.5g} "
                    "{:.5g}) ({:.5g} {:.5g} {:.5g}) now ({:.5g} {:.5g} {:.5g}) ({:.5g} {:.5g} {:.5g})",
                    list, i, count, amount, a[0], a[1], a[2], a[4], a[5], a[6], b[0], b[1], b[2],
                    b[4], b[5], b[6]);
      }
      auto f = [&](uint32_t o) { return guest::LoadFloat(base, list + o); };
      REXLOG_INFO("world sprites: list {:08X} matrix {:.4g} {:.4g} {:.4g} | {:.4g} {:.4g} {:.4g} | "
                  "{:.4g} {:.4g} {:.4g} | {:.5g} {:.5g} {:.5g}",
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
REXCVAR_DEFINE_BOOL(log_camera_pass_records, false, "ITC",
                    "Log which camera passes are recorded on logic and render-only passes");

extern "C" REX_FUNC(sub_820A3978) {
  REX_FUNC_PROLOGUE();
  if (REXCVAR_GET(log_camera_pass_records) && frame_pacing::GameMessages_InRace()) {
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
        REXLOG_INFO("camera pass record: pass {:08X} (type {}) in handler {:08X} message {}: {} "
                    "on {} logic, {} on {} render-only passes",
                    pass, guest::LoadU32(base, pass + 584), handler, int32_t(message), n.first,
                    logic - logic_start, n.second, render - render_start);
      }
      counts.clear();
      logic_start = logic;
      render_start = render;
    }
  }
  __imp__sub_820A3978(ctx, base);
}

// Single world-space sprites (sub_821281A8: r3 = sprite, r4 = camera; the
// marker above opponents, from the race overlays). Like the sprite lists
// above: c0 is the sprite's matrix (+0) times the camera, and its four
// corners (x, y, z, pad) at +80 become the vertices. The corners are
// blended between their last two ticks while it records, matched by sprite
// and order in the pass, then put back.
REXCVAR_DEFINE_BOOL(log_single_sprites, false, "ITC",
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
  bool blend = REXCVAR_GET(interpolate_draw_state) && REXCVAR_GET(blend_world_sprites) &&
               frame_pacing::GameMessages_InRace() && state.ticks >= 2 && corners == state.now;
  for (uint32_t k = 0; blend && k < 16; k++) {
    blend = std::isfinite(state.before[k]) &&
            std::abs(state.now[k] - state.before[k]) <= kMaxSpriteStep;
  }
  if (REXCVAR_GET(log_single_sprites)) {
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
      REXLOG_INFO("single sprite: pass {} {} amount {:.2f} sprite {:08X} from {:08X} #{} handler "
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

// Cockpit gauges (sub_820DBD38: r3 = the cockpit, r4 = the car + 4288). Its
// gauges (sub_820DB980 and sub_820DBA38) set their needle animations' frame
// (sub_820D1B38) straight from the float at r4 + 628 (the car's +4916),
// which game logic sets once a tick. Drawn at that value, the tachometer's
// needle stepped at 30 Hz against the blended cockpit (jumping when braking
// hard, as the value falls fast). The value is blended between its last two
// ticks while the cockpit draws, then put back.
REXCVAR_DEFINE_BOOL(blend_cockpit_gauges, true, "ITC",
                    "Blend the cockpit gauges' needle value between ticks");
REXCVAR_DEFINE_BOOL(log_cockpit_gauges, false, "ITC",
                    "Log the cockpit gauges' needle value as drawn, 60 lines every 2 s");

namespace {
struct GaugeValue {
  float before = 0, now = 0;
  int ticks = 0;
  uint64_t pass = ~0ull;  // the logic pass last recorded (the cockpit may draw more than once)
};
std::unordered_map<uint32_t, GaugeValue> g_gauge_values;  // by the car + 4288
}  // namespace

// --log_cockpit_changes: the words of the cockpit, the car + 4288 and the
// two gauge models (at [cockpit + 964] and [cockpit + 1444]) that change
// inside the cockpit's draw, and those that changed since the last draw (by
// game logic or other drawing), by pass, for finding what drives the
// needles.
REXCVAR_DEFINE_BOOL(log_cockpit_changes, false, "ITC",
                    "Log the cockpit's words changed in and between its draws, every 2 s");

namespace {
class CockpitChanges {
 public:
  CockpitChanges(uint8_t* base, uint32_t cockpit, uint32_t car) {
    if (!REXCVAR_GET(log_cockpit_changes)) {
      return;
    }
    active_ = true;
    // The gauge models' per-material shader values (vec4s at [model + 320],
    // uploaded as vertex c9-c12 by sub_820CECA0) and the models themselves.
    uint32_t gauge_a = guest::LoadU32(base, cockpit + 720 + 244);
    uint32_t gauge_b = guest::LoadU32(base, cockpit + 1440 + 4);
    auto params = [&](uint32_t model) { return model ? guest::LoadU32(base, model + 320) : 0; };
    ranges_ = {{"gauge A values", params(gauge_a), 0x200},
               {"gauge B values", params(gauge_b), 0x200},
               {"gauge A", gauge_a, 0x200},
               {"gauge B", gauge_b, 0x200}};
    (void)car;
    static auto last = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::seconds(2)) {
      last = now;
      lines_ = 0;
    }
    before_ = Read(base);
    if (previous_.size() == before_.size()) {
      Log(previous_, before_, "since the last draw");
    }
  }

  void After(uint8_t* base) {
    if (!active_) {
      return;
    }
    std::vector<uint32_t> after = Read(base);
    Log(before_, after, "in the draw");
    previous_ = std::move(after);
  }

 private:
  struct Range {
    const char* name;
    uint32_t start, size;
  };

  std::vector<uint32_t> Read(uint8_t* base) {
    std::vector<uint32_t> words;
    for (const Range& r : ranges_) {
      for (uint32_t i = 0; i < r.size; i += 4) {
        words.push_back(r.start ? guest::LoadU32(base, r.start + i) : 0);
      }
    }
    return words;
  }

  void Log(const std::vector<uint32_t>& a, const std::vector<uint32_t>& b, const char* when) {
    size_t k = 0;
    std::string line;
    for (const Range& r : ranges_) {
      for (uint32_t i = 0; i < r.size; i += 4, k++) {
        if (a[k] != b[k] && line.size() < 1500) {
          float fa, fb;
          std::memcpy(&fa, &a[k], 4);
          std::memcpy(&fb, &b[k], 4);
          line += fmt::format(" {}+{}:{:08X}->{:08X}({:.5g}->{:.5g})", r.name, i, a[k], b[k], fa,
                              fb);
        }
      }
    }
    if (!line.empty() && lines_++ < 80) {
      REXLOG_INFO("cockpit changes: pass {} {} amount {:.2f} {}:{}",
                  frame_pacing::GameMessages_PassNumber(),
                  frame_pacing::GameMessages_IsRenderOnlyPass() ? "render-only" : "logic",
                  frame_pacing::GetFramePacer().TickProgress(), when, line);
    }
  }

  bool active_ = false;
  std::vector<Range> ranges_;
  std::vector<uint32_t> before_;
  static inline std::vector<uint32_t> previous_;
  static inline int lines_ = 0;
};
}  // namespace

extern "C" REX_FUNC(sub_820DBD38) {
  REX_FUNC_PROLOGUE();
  constexpr uint32_t kGaugeValueOffset = 628;
  uint32_t address = ctx.r4.u32 + kGaugeValueOffset;
  float value = guest::LoadFloat(base, address);
  bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
  GaugeValue& gauge = g_gauge_values[ctx.r4.u32];
  uint64_t pass = frame_pacing::GameMessages_PassNumber();
  if (!render_only && pass != gauge.pass) {
    gauge.pass = pass;
    gauge.before = gauge.ticks > 0 ? gauge.now : value;
    gauge.now = value;
    gauge.ticks++;
  }
  float amount = frame_pacing::GetFramePacer().TickProgress();
  bool blend = REXCVAR_GET(interpolate_draw_state) && REXCVAR_GET(blend_cockpit_gauges) &&
               frame_pacing::GameMessages_InRace() && gauge.ticks >= 2 && value == gauge.now &&
               std::isfinite(gauge.before);
  float drawn = blend ? gauge.before + (gauge.now - gauge.before) * amount : value;
  if (REXCVAR_GET(log_cockpit_gauges)) {
    static std::chrono::steady_clock::time_point last{};
    static int lines = 0;
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::seconds(2)) {
      last = now;
      lines = 0;
    }
    if (lines++ < 60) {
      REXLOG_INFO("cockpit gauges: pass {} {} car {:08X} amount {:.2f} value {} ticks {} -> {} "
                  "drawn {} blend {}",
                  frame_pacing::GameMessages_PassNumber(), render_only ? "render-only" : "logic",
                  ctx.r4.u32, amount, value, gauge.before, gauge.now, drawn, blend);
    }
  }
  CockpitChanges changes(base, ctx.r3.u32, ctx.r4.u32);
  // The cockpit's parts are drawn with the car's world matrix (r4 + 16):
  // their products blend in its frame, about their pivots (blend_state.cpp).
  constexpr uint32_t kCarWorldOffset = 16;
  interpolation::BlendState_SetParentWorld(base, ctx.r4.u32 + kCarWorldOffset);
  if (!blend) {
    __imp__sub_820DBD38(ctx, base);
  } else {
    guest::StoreFloat(base, address, drawn);
    __imp__sub_820DBD38(ctx, base);
    guest::StoreFloat(base, address, value);
  }
  interpolation::BlendState_SetParentWorld(base, 0);
  changes.After(base);
}

// Diagnostics (--log_gauge_constants): the vertex constants c9-c12 that a
// gauge model's materials are drawn with (sub_820CECA0: r3 = model, r4 =
// mesh, r5 = material pass). The gauges (sub_820DB980, sub_820DBA38) set
// the model's +440 around their draw, which selects vertex shader 1005.
// Each material (512 bytes, from [[model + 40] + 544 * mesh + 396] + 16)
// takes up to 4 (+128) vec4s: xyz from the model's array at [model + 320]
// by the indices at +208, w from +240; an index of -1 gives (0, 0, 0, 1).
// Logged per call, 80 lines every 2 s.
REXCVAR_DEFINE_BOOL(log_gauge_constants, false, "ITC",
                    "Log the gauge models' per-material vertex constants c9-c12 as drawn");

extern "C" REX_FUNC(sub_820CECA0) {
  REX_FUNC_PROLOGUE();
  uint32_t model = ctx.r3.u32, mesh = ctx.r4.u32, pass = ctx.r5.u32;
  if (REXCVAR_GET(log_gauge_constants) && model && guest::LoadU32(base, model + 440) != 0) {
    static std::chrono::steady_clock::time_point last{};
    static int lines = 0;
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::seconds(2)) {
      last = now;
      lines = 0;
    }
    uint32_t mesh_entry = guest::LoadU32(base, model + 40) + 544 * mesh;
    uint32_t set = guest::LoadU32(base, mesh_entry + 396);
    uint32_t first = guest::LoadU32(base, set + 8);
    uint32_t count = guest::LoadU32(base, set + (pass ? 12 : 8));
    uint32_t table = guest::LoadU32(base, model + 320);
    for (uint32_t i = 0; i < std::min<uint32_t>(count, 16) && lines < 80; i++, lines++) {
      uint32_t material = set + 16 + 512 * ((pass ? first : 0) + i);
      uint32_t n = std::min<uint32_t>(guest::LoadU32(base, material + 128), 4);
      std::string values;
      for (uint32_t k = 0; k < n; k++) {
        int32_t index = int32_t(guest::LoadU32(base, material + 208 + 4 * k));
        float w = guest::LoadFloat(base, material + 240 + 4 * k);
        if (index == -1) {
          values += fmt::format(" c{}[-1] w {:.5g}", 9 + k, w);
        } else {
          uint32_t v = table + 16 * uint32_t(index);
          values += fmt::format(" c{}[{}] ({:.5g} {:.5g} {:.5g}) w {:.5g}", 9 + k, index,
                                guest::LoadFloat(base, v), guest::LoadFloat(base, v + 4),
                                guest::LoadFloat(base, v + 8), w);
        }
      }
      REXLOG_INFO("gauge constants: pass {} {} model {:08X} flag {} mesh {} material pass {} #{} "
                  "type {} table {:08X} c13.w {:.5g}:{}",
                  frame_pacing::GameMessages_PassNumber(),
                  frame_pacing::GameMessages_IsRenderOnlyPass() ? "render-only" : "logic", model,
                  guest::LoadU32(base, model + 440), mesh, pass, i,
                  guest::LoadU32(base, material + 392), table, guest::LoadFloat(base, material + 12),
                  values);
    }
  }
  __imp__sub_820CECA0(ctx, base);
}
