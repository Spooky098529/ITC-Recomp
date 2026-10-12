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
#include "interpolation/transform.h"

#include <rex/cvar.h>

#include <rex/logging.h>

#include <chrono>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

REXCVAR_DECLARE(bool, interpolate_draw_state);
REXCVAR_DEFINE_DOUBLE(debug_overlay_vehicle_lift, 0.0, "ITC",
                      "Testing: raise the cars this many metres while the race overlays "
                      "record");
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

// The race overlays build the marker above each opponent on the CPU, at
// that car's position. So that it sits over the car as drawn (blended
// between ticks), each car's world matrix is set to the frame's blend while
// the overlays record, then put back.
namespace {
constexpr uint32_t kVehicleWorldOffset = 0xD0;
constexpr uint32_t kVehicleWorldCopyOffset = 0xCF0;  // identical every tick

struct VehiclePoses {
  std::optional<interpolation::Matrix4> before, now;  // world, last two ticks
};
std::set<uint32_t> g_vehicles;  // seen by the speed getter
std::unordered_map<uint32_t, VehiclePoses> g_vehicle_poses;

interpolation::Matrix4 LoadMatrix(uint8_t* base, uint32_t address) {
  interpolation::Matrix4 m;
  for (int i = 0; i < 16; i++) {
    m[i] = guest::LoadFloat(base, address + 4 * i);
  }
  return m;
}

void StoreMatrix(uint8_t* base, uint32_t address, const interpolation::Matrix4& m) {
  for (int i = 0; i < 16; i++) {
    guest::StoreFloat(base, address + 4 * i, m[i]);
  }
}

// Logic pass: the cars' poses this tick.
void RecordVehiclePoses(uint8_t* base) {
  for (uint32_t vehicle : g_vehicles) {
    VehiclePoses& poses = g_vehicle_poses[vehicle];
    poses.before = poses.now;
    poses.now = LoadMatrix(base, vehicle + kVehicleWorldOffset);
  }
}

// Sets each car's world matrix to the frame's blend; returns what to put back.
std::vector<std::pair<uint32_t, interpolation::Matrix4>> BlendVehiclePoses(uint8_t* base) {
  std::vector<std::pair<uint32_t, interpolation::Matrix4>> saved;
  if (!REXCVAR_GET(interpolate_draw_state) && REXCVAR_GET(debug_overlay_vehicle_lift) == 0) {
    return saved;
  }
  float amount = frame_pacing::GetFramePacer().TickProgress();
  float lift = float(REXCVAR_GET(debug_overlay_vehicle_lift));
  if (lift != 0) {
    for (uint32_t vehicle : g_vehicles) {
      for (uint32_t offset : {kVehicleWorldOffset, kVehicleWorldCopyOffset}) {
        interpolation::Matrix4 m = LoadMatrix(base, vehicle + offset);
        saved.push_back({vehicle + offset, m});
        m[13] += lift;
        StoreMatrix(base, vehicle + offset, m);
      }
    }
    return saved;
  }
  for (const auto& [vehicle, poses] : g_vehicle_poses) {
    if (!poses.before || !poses.now) {
      continue;
    }
    auto a = interpolation::ScaledTransformFromWorldMatrix(*poses.before);
    auto b = interpolation::ScaledTransformFromWorldMatrix(*poses.now);
    if (!a || !b ||
        interpolation::Distance(a->transform.position, b->transform.position) > 8.0f) {
      continue;
    }
    interpolation::Matrix4 blended =
        interpolation::WorldMatrixFromScaledTransform(interpolation::Blend(*a, *b, amount));
    for (uint32_t offset : {kVehicleWorldOffset, kVehicleWorldCopyOffset}) {
      if (LoadMatrix(base, vehicle + offset) == *poses.now) {
        saved.push_back({vehicle + offset, *poses.now});
        StoreMatrix(base, vehicle + offset, blended);
      }
    }
  }
  return saved;
}
}  // namespace

namespace frame_pacing {
void PerFrameLogic_OnVehicleSeen(uint32_t vehicle) {
  if (g_vehicles.size() < 64) {
    g_vehicles.insert(vehicle);
  }
}
}  // namespace frame_pacing

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
  if (!render_only) {
    RecordVehiclePoses(base);
  }
  auto vehicles = BlendVehiclePoses(base);
  interpolation::BlendState_SetHandler(kRecordContext);
  for_each_overlay(kRecordSlot);
  for (const auto& [address, matrix] : vehicles) {
    StoreMatrix(base, address, matrix);
  }
  if (render_only) {
    std::memcpy(cameras, current.data(), kCameraPassesBytes);
  }
}
