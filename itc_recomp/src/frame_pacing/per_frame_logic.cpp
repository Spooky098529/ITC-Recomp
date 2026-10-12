// Game logic that the game runs from its Draw message, counting frames.
//
// On the original hardware every Draw was a 30 Hz tick, so this logic counts
// ticks; render-only passes (see game_messages.h) draw several times a tick
// and would run it several times faster. Each function here skips its logic
// on render-only passes. Found with --find_per_frame_state.

#include "game_messages.h"
#include "guest.h"
#include "frame_pacer.h"
#include "interpolation/blend_state.h"

#include <rex/cvar.h>

#include <rex/logging.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

REXCVAR_DECLARE(bool, interpolate_draw_state);
REXCVAR_DEFINE_BOOL(blend_race_overlays, true, "ITC",
                    "Blend the race overlays' state (the marker above opponents) between ticks "
                    "while they record");
REXCVAR_DEFINE_BOOL(per_tick_blink_timers, true, "ITC",
                    "Advance the car blink timers once per tick on render-only passes");
REXCVAR_DEFINE_BOOL(per_tick_ui_movies, true, "ITC",
                    "Advance UI movies once per tick on render-only passes");
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

// The overlays keep where to draw in their own state, set when updated
// (slot 4). So that the marker above an opponent moves smoothly, each
// overlay's state is blended between its last two ticks while it records:
// 32-bit words that hold floats and changed by a modest amount are
// interpolated to the frame's amount; anything else (pointers, flags,
// counters, unchanged values) keeps the newest tick's. Then it's put back.
namespace {
constexpr uint32_t kOverlayBytes = 0x1000;
struct OverlayStates {
  std::vector<uint8_t> before, now;  // after the last two ticks' updates
};
std::unordered_map<uint32_t, OverlayStates> g_overlay_states;

float WordAsFloat(const uint8_t* p) {
  uint32_t bits = (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | p[3];
  float f;
  std::memcpy(&f, &bits, 4);
  return f;
}

void StoreFloatWord(uint8_t* p, float f) {
  uint32_t bits;
  std::memcpy(&bits, &f, 4);
  p[0] = uint8_t(bits >> 24);
  p[1] = uint8_t(bits >> 16);
  p[2] = uint8_t(bits >> 8);
  p[3] = uint8_t(bits);
}

bool IsPlainFloat(float f) {
  float a = std::abs(f);
  return std::isfinite(f) && (a == 0 || (a > 1e-6f && a < 1e7f));
}

// Writes the blend of `states` into the overlay at `state`.
void BlendOverlayState(uint8_t* state, const OverlayStates& states, float amount) {
  for (uint32_t i = 0; i + 4 <= kOverlayBytes; i += 4) {
    const uint8_t* a = states.before.data() + i;
    const uint8_t* b = states.now.data() + i;
    if (std::memcmp(a, b, 4) == 0 || std::memcmp(state + i, b, 4) != 0) {
      continue;
    }
    float fa = WordAsFloat(a), fb = WordAsFloat(b);
    if (!IsPlainFloat(fa) || !IsPlainFloat(fb) ||
        !(std::abs(fb - fa) <= 0.5f * std::max(std::abs(fa), std::abs(fb)) + 2.0f)) {
      continue;
    }
    StoreFloatWord(state + i, fa + (fb - fa) * amount);
  }
}
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
  std::vector<uint32_t> overlays;
  for (uint32_t i = 0; i < kOverlayCount; i++) {
    if (uint32_t overlay = guest::LoadU32(base, list + 4 * i)) {
      overlays.push_back(overlay);
    }
  }
  if (!render_only) {
    interpolation::BlendState_SetHandler(kUpdateContext);
    for_each_overlay(kUpdateSlot);
    for (uint32_t overlay : overlays) {
      OverlayStates& states = g_overlay_states[overlay];
      states.before = std::move(states.now);
      states.now.assign(base + overlay, base + overlay + kOverlayBytes);
    }
  }
  std::vector<std::pair<uint32_t, std::vector<uint8_t>>> saved;
  if (REXCVAR_GET(interpolate_draw_state) && REXCVAR_GET(blend_race_overlays)) {
    float amount = frame_pacing::GetFramePacer().TickProgress();
    for (uint32_t overlay : overlays) {
      auto it = g_overlay_states.find(overlay);
      if (it == g_overlay_states.end() || it->second.before.size() != kOverlayBytes) {
        continue;
      }
      saved.push_back({overlay, std::vector<uint8_t>(base + overlay, base + overlay + kOverlayBytes)});
      BlendOverlayState(base + overlay, it->second, amount);
    }
  }
  interpolation::BlendState_SetHandler(kRecordContext);
  for_each_overlay(kRecordSlot);
  interpolation::BlendState_LogOverlayAllocations(base);
  for (const auto& [overlay, state] : saved) {
    std::memcpy(base + overlay, state.data(), state.size());
  }
  if (render_only) {
    std::memcpy(cameras, current.data(), kCameraPassesBytes);
  }
}

// World-space sprite lists (sub_82128750: r3 = list, r4 = camera; from the
// race overlays, including the marker above opponents). The list holds up to
// 500 sprites, count at +0x109F0; sprite i's four corners (x, y, z, pad) at
// +80 + 64 * i, written by game logic once a tick. The recorder writes them
// into the command list as vertices, drawn through the camera, which is
// blended between ticks; left at the tick's corners, a sprite was ahead of
// its blended car early in each tick and caught up by the end (two markers).
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
                    "Blend world-space sprites (the marker above opponents) between ticks");

// Wrapped in draw_tags.cpp (sub_82128750); `record` runs the original.
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
