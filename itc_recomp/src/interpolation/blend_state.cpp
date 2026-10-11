#include "blend_state.h"

#include "frame_pacing/frame_pacer.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <optional>
#include <vector>

REXCVAR_DEFINE_BOOL(interpolate_camera, true, "ITC",
                    "Smooth camera motion between 30 Hz ticks in races");
REXCVAR_DEFINE_BOOL(log_blend_state, false, "ITC",
                    "Log how many camera passes were blended, once a second");
REXCVAR_DEFINE_DOUBLE(debug_draw_camera_lift, 0.0, "ITC",
                      "Debug: raise the camera by this many metres in render-only passes");

namespace interpolation {
namespace {

// The camera passes (see "Known data" in docs/functions.md): 21 objects of
// 592 bytes. Game logic computes each pass's view-projection matrix at +0x90
// (from the view at +0x10); the draw step reads only that product.
constexpr uint32_t kCameraPassArray = 0x826BAC00;
constexpr uint32_t kCameraPassCount = 21;
constexpr uint32_t kCameraPassBytes = 592;
constexpr uint32_t kViewProjectionOffset = 0x90;

// A pass's camera moving further than this in one tick is a cut (a camera
// switch), drawn without blending.
constexpr float kCutDistance = 4.0f;  // metres
constexpr float kCutAngle = 0.5f;     // radians, ~30 degrees

uint32_t ViewProjectionAddress(uint32_t pass) {
  return kCameraPassArray + pass * kCameraPassBytes + kViewProjectionOffset;
}

Matrix4 LoadMatrix(uint8_t* base, uint32_t address) {
  Matrix4 m;
  for (int i = 0; i < 16; i++) {
    m[i] = guest::LoadFloat(base, address + 4 * i);
  }
  return m;
}

void StoreMatrix(uint8_t* base, uint32_t address, const Matrix4& m) {
  for (int i = 0; i < 16; i++) {
    guest::StoreFloat(base, address + 4 * i, m[i]);
  }
}

bool IsFinite(const Matrix4& m) {
  return std::all_of(m.begin(), m.end(), [](float v) { return std::isfinite(v); });
}

// Each pass's view-projection at the last two ticks, and how to blend them.
struct CameraPasses {
  std::array<Matrix4, kCameraPassCount> previous{};
  std::array<Matrix4, kCameraPassCount> current{};
  std::array<std::optional<RigidMotion>, kCameraPassCount> motion{};
  bool have_previous = false;
};

CameraPasses g_passes;

// A matrix replaced for one Draw, and its real value.
struct Saved {
  uint32_t address;
  Matrix4 value;
};
std::vector<Saved> g_saved;

struct Counts {
  int draws = 0;
  int blended = 0;
  int cuts = 0;
  std::chrono::steady_clock::time_point last_log{};
} g_counts;

void Replace(uint8_t* base, uint32_t address, const Matrix4& value) {
  g_saved.push_back({address, LoadMatrix(base, address)});
  StoreMatrix(base, address, value);
}

// After a tick's logic: the passes' new matrices become current.
void OnTick(uint8_t* base) {
  g_passes.previous = g_passes.current;
  for (uint32_t p = 0; p < kCameraPassCount; p++) {
    g_passes.current[p] = LoadMatrix(base, ViewProjectionAddress(p));
  }
  bool had_previous = g_passes.have_previous;
  g_passes.have_previous = true;
  for (uint32_t p = 0; p < kCameraPassCount; p++) {
    g_passes.motion[p].reset();
    const Matrix4& a = g_passes.previous[p];
    const Matrix4& b = g_passes.current[p];
    if (!had_previous || a == b || !IsFinite(a) || !IsFinite(b)) {
      continue;
    }
    std::optional<RigidMotion> motion = FindRigidMotion(a, b);
    if (!motion) {
      continue;
    }
    float moved = Distance(motion->motion.position, Vec3{});
    if (motion->pivot) {
      // With a pivot, the camera's own movement is what counts: its eye moves
      // from (pivot moved by D) to the pivot.
      Matrix4 full = WorldMatrixFromTransform(motion->motion);
      const Vec3& p0 = *motion->pivot;
      Vec3 q;
      for (int k = 0; k < 3; k++) {
        q[k] = p0[0] * full[k] + p0[1] * full[4 + k] + p0[2] * full[8 + k] + full[12 + k];
      }
      moved = Distance(p0, q);
    }
    float turned = AngleBetween(motion->motion.rotation, Quaternion{});
    if (moved > kCutDistance || turned > kCutAngle) {
      g_counts.cuts++;
      continue;
    }
    g_passes.motion[p] = motion;
  }
}

void LogCounts() {
  if (!REXCVAR_GET(log_blend_state)) {
    return;
  }
  auto now = std::chrono::steady_clock::now();
  if (now - g_counts.last_log < std::chrono::seconds(1)) {
    return;
  }
  REXLOG_INFO("blend state: {} draws, {} pass blends, {} cuts", g_counts.draws, g_counts.blended,
              g_counts.cuts);
  g_counts = {};
  g_counts.last_log = now;
}

}  // namespace

void BlendState_OnVehicleSeen(uint32_t) {}

void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass) {
  g_saved.clear();
  if (!render_only_pass) {
    OnTick(base);
  }
  g_counts.draws++;

  if (REXCVAR_GET(interpolate_camera)) {
    // Drawing runs a tick behind: the logic pass shows the previous tick
    // (amount 0) and the passes after it move towards the current one.
    float amount = frame_pacing::GetFramePacer().TickProgress();
    for (uint32_t p = 0; p < kCameraPassCount; p++) {
      if (!g_passes.motion[p]) {
        continue;
      }
      Replace(base, ViewProjectionAddress(p), BlendRigidMotion(*g_passes.motion[p], amount));
      g_counts.blended++;
    }
  }

  float lift = float(REXCVAR_GET(debug_draw_camera_lift));
  if (render_only_pass && lift != 0) {
    // Raise by moving the world down in front of the projection.
    uint32_t address = ViewProjectionAddress(0);
    Matrix4 vp = LoadMatrix(base, address);
    Matrix4 down{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -lift, 0, 1};
    Replace(base, address, Multiply(down, vp));
  }
  LogCounts();
}

void BlendState_AfterDraw(uint8_t* base) {
  for (auto it = g_saved.rbegin(); it != g_saved.rend(); ++it) {
    StoreMatrix(base, it->address, it->value);
  }
  g_saved.clear();
}

}  // namespace interpolation
