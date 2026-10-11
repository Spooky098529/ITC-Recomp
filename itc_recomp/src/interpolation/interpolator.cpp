#include "interpolator.h"

#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <chrono>
#include <mutex>
#include <optional>
#include <vector>

// Off by default: drawing replays render commands recorded during the tick,
// so changing these matrices between ticks has no visible effect yet.
REXCVAR_DEFINE_BOOL(interpolate, false, "ITC",
                    "Smooth camera and car motion between 30 Hz game ticks");
REXCVAR_DEFINE_DOUBLE(interpolate_debug_camera_lift, 0.0, "ITC",
                      "Debug: raise the drawn camera by this many metres, to check that "
                      "drawing reads the interpolated camera");
REXCVAR_DEFINE_BOOL(log_interpolation, false, "ITC",
                    "Log interpolation counts once a second");

namespace interpolation {
namespace {

// See "Known data" in docs/functions.md.
constexpr uint32_t kCameraViewMatrixAddress = 0x826BAC10;
constexpr uint32_t kVehicleWorldMatrixOffset = 0xD0;
constexpr uint32_t kVehicleWorldMatrixCopyOffset = 0xCF0;

// Movement in one tick beyond which a change is drawn as a cut. The camera
// is judged relative to the player's car, which can itself cover ~2 m a tick.
constexpr float kCameraCutDistance = 1.0f;     // metres, relative to the car
constexpr float kCameraCutAngle = 0.5f;        // radians, ~30 degrees
constexpr float kVehicleCutDistance = 15.0f;   // metres (a reset or teleport)
constexpr float kVehicleCutAngle = 1.0f;       // radians

constexpr size_t kMaxVehicles = 16;

enum class MatrixKind { kWorld, kView };

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

std::optional<Transform> TransformFromMatrix(MatrixKind kind, const Matrix4& m) {
  return kind == MatrixKind::kView ? TransformFromViewMatrix(m) : TransformFromWorldMatrix(m);
}

Matrix4 MatrixFromTransform(MatrixKind kind, const Transform& t) {
  return kind == MatrixKind::kView ? ViewMatrixFromTransform(t) : WorldMatrixFromTransform(t);
}

// One guest matrix tracked across ticks.
struct TrackedMatrix {
  uint32_t address = 0;
  MatrixKind kind = MatrixKind::kWorld;

  Matrix4 current_matrix{};
  std::optional<Transform> previous;
  std::optional<Transform> current;
  bool cut = false;
  bool seen_this_tick = false;
  bool written = false;
};

class Interpolator {
 public:
  void OnVehicleSeen(uint32_t vehicle) {
    std::lock_guard lock(vehicles_mutex_);
    if (std::find(vehicles_seen_.begin(), vehicles_seen_.end(), vehicle) ==
            vehicles_seen_.end() &&
        vehicles_seen_.size() < kMaxVehicles) {
      vehicles_seen_.push_back(vehicle);
    }
  }

  void OnTickDone(uint8_t* base) {
    std::vector<uint32_t> vehicles;
    {
      std::lock_guard lock(vehicles_mutex_);
      vehicles.swap(vehicles_seen_);
    }
    if (!REXCVAR_GET(interpolate)) {
      tracked_.clear();
      return;
    }

    for (TrackedMatrix& tracked : tracked_) {
      tracked.seen_this_tick = false;
    }
    // Car movement this tick, for judging camera cuts. The first vehicle the
    // speed getter sees each tick is the player's.
    std::optional<Vec3> player_movement;
    for (size_t i = 0; i < vehicles.size(); i++) {
      for (uint32_t offset : {kVehicleWorldMatrixOffset, kVehicleWorldMatrixCopyOffset}) {
        TrackedMatrix& tracked = Update(base, vehicles[i] + offset, MatrixKind::kWorld,
                                        kVehicleCutDistance, kVehicleCutAngle, std::nullopt);
        if (i == 0 && offset == kVehicleWorldMatrixOffset && tracked.previous &&
            tracked.current) {
          Vec3 movement;
          for (int k = 0; k < 3; k++) {
            movement[k] = tracked.current->position[k] - tracked.previous->position[k];
          }
          player_movement = movement;
        }
      }
    }
    Update(base, kCameraViewMatrixAddress, MatrixKind::kView, kCameraCutDistance,
           kCameraCutAngle, player_movement);

    // Forget vehicles that weren't seen this tick; their memory may be gone.
    std::erase_if(tracked_, [](const TrackedMatrix& t) { return !t.seen_this_tick; });
  }

  void BeginFrame(uint8_t* base, float tick_progress) {
    if (!REXCVAR_GET(interpolate)) {
      return;
    }
    float lift = float(REXCVAR_GET(interpolate_debug_camera_lift));
    for (TrackedMatrix& tracked : tracked_) {
      bool is_camera = tracked.address == kCameraViewMatrixAddress;
      std::optional<Transform> drawn;
      if (!tracked.current) {
        if (is_camera) stats_.camera_invalid++;
      } else if (tracked.cut || !tracked.previous) {
        if (is_camera) stats_.camera_cut++;
        drawn = *tracked.current;
      } else {
        if (is_camera) stats_.camera_blended++;
        drawn = Blend(*tracked.previous, *tracked.current, tick_progress);
      }
      if (!drawn) {
        continue;
      }
      if (is_camera) {
        drawn->position[1] += lift;
      } else if (tracked.cut || !tracked.previous) {
        continue;  // nothing to change
      }
      StoreMatrix(base, tracked.address, MatrixFromTransform(tracked.kind, *drawn));
      tracked.written = true;
    }
    stats_.frames++;
    LogStats();
  }

  void EndFrame(uint8_t* base) {
    for (TrackedMatrix& tracked : tracked_) {
      if (tracked.written) {
        StoreMatrix(base, tracked.address, tracked.current_matrix);
        tracked.written = false;
      }
    }
  }

 private:
  TrackedMatrix& Find(uint32_t address, MatrixKind kind) {
    for (TrackedMatrix& tracked : tracked_) {
      if (tracked.address == address) {
        return tracked;
      }
    }
    tracked_.push_back({.address = address, .kind = kind});
    return tracked_.back();
  }

  // Reads the matrix's value for this tick and decides whether the change
  // since the last tick is smooth motion or a cut. `reference_movement` is
  // subtracted before judging distance (the camera rides along with the car).
  TrackedMatrix& Update(uint8_t* base, uint32_t address, MatrixKind kind, float cut_distance,
                        float cut_angle, const std::optional<Vec3>& reference_movement) {
    TrackedMatrix& tracked = Find(address, kind);
    tracked.seen_this_tick = true;
    tracked.current_matrix = LoadMatrix(base, address);
    tracked.previous = tracked.current;
    tracked.current = TransformFromMatrix(kind, tracked.current_matrix);

    tracked.cut = true;
    if (tracked.previous && tracked.current) {
      Vec3 expected = tracked.previous->position;
      if (reference_movement) {
        for (int k = 0; k < 3; k++) {
          expected[k] += (*reference_movement)[k];
        }
      }
      float moved = Distance(expected, tracked.current->position);
      float turned = AngleBetween(tracked.previous->rotation, tracked.current->rotation);
      tracked.cut = moved > cut_distance || turned > cut_angle;
    }
    return tracked;
  }

  struct Stats {
    int frames = 0;
    int camera_blended = 0;
    int camera_cut = 0;
    int camera_invalid = 0;
  };

  void LogStats() {
    if (!REXCVAR_GET(log_interpolation)) {
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - last_stats_log_ < std::chrono::seconds(1)) {
      return;
    }
    size_t vehicles = std::count_if(tracked_.begin(), tracked_.end(), [](const TrackedMatrix& t) {
      return t.kind == MatrixKind::kWorld;
    });
    REXLOG_INFO("interpolation: {} frames, camera blended {} cut {} invalid {}, {} car matrices",
                stats_.frames, stats_.camera_blended, stats_.camera_cut, stats_.camera_invalid,
                vehicles);
    stats_ = {};
    last_stats_log_ = now;
  }

  Stats stats_;
  std::chrono::steady_clock::time_point last_stats_log_{};

  std::mutex vehicles_mutex_;
  std::vector<uint32_t> vehicles_seen_;

  // Render thread only.
  std::vector<TrackedMatrix> tracked_;
};

Interpolator& GetInterpolator() {
  static Interpolator interpolator;
  return interpolator;
}

}  // namespace

void Interpolation_OnVehicleSeen(uint32_t vehicle) {
  GetInterpolator().OnVehicleSeen(vehicle);
}

void Interpolation_OnTickDone(uint8_t* base) {
  GetInterpolator().OnTickDone(base);
}

void Interpolation_BeginFrame(uint8_t* base, float tick_progress) {
  GetInterpolator().BeginFrame(base, tick_progress);
}

void Interpolation_EndFrame(uint8_t* base) {
  GetInterpolator().EndFrame(base);
}

}  // namespace interpolation
