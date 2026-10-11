// What the draw step reads (measured in a race, --dump_draw_state):
//
// - Camera passes: 21 objects of 592 bytes at 0x826BAC00, each with a view
//   matrix at +0x10, a projection at +0x50 and their product (view *
//   projection) at +0x90. Pass 0 is the main 1280x720 camera; game logic
//   sets it during Update. Some passes (the car reflection and shadow maps)
//   are recomputed inside Draw from other state.
// - Objects: Draw works out each object's world matrix (often into a
//   temporary on its stack) and hands it to the per-object recorder
//   (sub_825953D0) with the camera's view-projection; the recorder copies
//   world * view-projection and world into the command list.
//
// So, the usual engine interpolation: before Draw, each camera whose
// view-projection is view * projection is moved to a blend of its last two
// ticks' positions and rotations, and its view and view-projection are
// rebuilt. In the recorder, each object's world matrix is blended with its
// previous tick's, matched by address and by how many times that address has
// been recorded in this Draw (the draw step visits objects in the same order
// every tick). Everything is put back afterwards.
//
// Drawing runs a tick behind: the logic pass draws the previous tick (amount
// 0) and the passes after it move towards the current one.

#include "blend_state.h"

#include "frame_pacing/frame_pacer.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(interpolate_draw_state, true, "ITC",
                    "Smooth motion between 30 Hz ticks in races by blending cameras and "
                    "object transforms");
REXCVAR_DEFINE_BOOL(log_blend_state, false, "ITC",
                    "Log how many cameras and objects were blended, once a second");
REXCVAR_DEFINE_STRING(dump_draw_state, "", "ITC",
                      "Folder to dump the camera passes and the draw step's stack to, before "
                      "and after Draw, for 60 race ticks starting 20 s into the first race");

namespace interpolation {
namespace {

constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr int kCameraPassCount = 21;
constexpr uint32_t kCameraPassBytes = 592;
constexpr uint32_t kViewOffset = 0x10;
constexpr uint32_t kProjectionOffset = 0x50;
constexpr uint32_t kViewProjectionOffset = 0x90;

// Where the main thread's stack is during Draw (dumped for analysis).
constexpr uint32_t kDrawStack = 0x70180000;
constexpr uint32_t kDrawStackBytes = 0x10000;

// Moving further than this in one tick is a cut (a camera switch, a reset):
// drawn without blending.
constexpr float kCutDistance = 8.0f;  // metres
constexpr float kCutAngle = 0.5f;     // radians, ~30 degrees

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

bool IsCut(const Transform& a, const Transform& b) {
  return Distance(a.position, b.position) > kCutDistance ||
         AngleBetween(a.rotation, b.rotation) > kCutAngle;
}

// Whether `product` is `a * b`, to float precision.
bool IsProduct(const Matrix4& product, const Matrix4& a, const Matrix4& b) {
  Matrix4 expected = Multiply(a, b);
  for (int i = 0; i < 16; i++) {
    float tolerance = 1e-3f * std::max(1.0f, std::abs(expected[i]));
    if (!(std::abs(product[i] - expected[i]) <= tolerance)) {
      return false;
    }
  }
  return true;
}

uint32_t CameraPass(int pass) {
  return kCameraPasses + pass * kCameraPassBytes;
}

// Writes the camera passes and the draw step's stack to
// <folder>/tick<N>_<when>.bin as (u32 address, u32 size, bytes) records,
// little endian, with the bytes as in guest memory (big endian).
class DrawStateDump {
 public:
  void OnDraw(uint8_t* base, bool logic_pass, const char* when) {
    std::string folder = REXCVAR_GET(dump_draw_state);
    if (folder.empty() || !frame_pacing::GameMessages_InRace() || !logic_pass) {
      return;
    }
    if (std::string(when) == "pre") {
      race_ticks_++;
    }
    int first = 20 * 30;
    if (race_ticks_ <= first || race_ticks_ > first + 60) {
      return;
    }
    std::ofstream out(fmt::format("{}/tick{:03}_{}.bin", folder, race_ticks_ - first, when),
                      std::ios::binary);
    auto write = [&](uint32_t address, uint32_t size) {
      out.write(reinterpret_cast<const char*>(&address), 4);
      out.write(reinterpret_cast<const char*>(&size), 4);
      out.write(reinterpret_cast<const char*>(base + address), size);
    };
    write(kCameraPasses, kCameraPassCount * kCameraPassBytes);
    write(kDrawStack, kDrawStackBytes);
  }

 private:
  int race_ticks_ = 0;
};

class DrawState {
 public:
  void BeforeDraw(uint8_t* base, bool logic_pass) {
    logic_pass_ = logic_pass;
    dump_.OnDraw(base, logic_pass, "pre");
    if (logic_pass) {
      OnTick(base);
    }
    amount_ = frame_pacing::GetFramePacer().TickProgress();
    recorded_.clear();
    saved_.clear();
    enabled_ = REXCVAR_GET(interpolate_draw_state);
    if (!enabled_) {
      return;
    }
    for (const CameraBlend& camera : camera_blends_) {
      uint32_t pass = CameraPass(camera.pass);
      Matrix4 view = ViewMatrixFromTransform(Blend(camera.from, camera.to, amount_));
      Save(base, pass + kViewOffset);
      Save(base, pass + kViewProjectionOffset);
      StoreMatrix(base, pass + kViewOffset, view);
      StoreMatrix(base, pass + kViewProjectionOffset, Multiply(view, camera.projection));
      counts_.cameras++;
    }
  }

  void AfterDraw(uint8_t* base) {
    Restore(base, saved_);
    dump_.OnDraw(base, logic_pass_, "post");
    counts_.draws++;
    LogCounts();
  }

  void BeforeRecordObject(uint8_t* base, uint32_t view_projection, uint32_t world) {
    object_saved_.clear();
    if (!IsCameraViewProjection(view_projection)) {
      return;
    }
    ObjectKey key{world, recorded_[world]++};
    Matrix4 now = LoadMatrix(base, world);
    std::optional<ScaledTransform> transform;
    if (IsFinite(now)) {
      transform = ScaledTransformFromWorldMatrix(now);
    }
    if (!transform) {
      counts_.objects_not_transforms++;
      return;
    }
    if (logic_pass_) {
      objects_this_tick_[key] = *transform;
    }
    if (!enabled_) {
      return;
    }
    auto before = objects_last_tick_.find(key);
    if (before == objects_last_tick_.end()) {
      counts_.objects_unmatched++;
      return;
    }
    if (IsCut(before->second.transform, transform->transform)) {
      counts_.object_cuts++;
      return;
    }
    object_saved_.push_back({world, now});
    StoreMatrix(base, world,
                WorldMatrixFromScaledTransform(Blend(before->second, *transform, amount_)));
    counts_.objects++;
  }

  void AfterRecordObject(uint8_t* base) {
    Restore(base, object_saved_);
  }

 private:
  struct CameraBlend {
    int pass;
    Transform from;
    Transform to;
    Matrix4 projection;
  };

  struct ObjectKey {
    uint32_t address;
    int occurrence;  // times this address was recorded earlier in the Draw
    bool operator==(const ObjectKey&) const = default;
  };
  struct ObjectKeyHash {
    size_t operator()(const ObjectKey& key) const {
      return std::hash<uint64_t>()(uint64_t(key.address) << 20 ^ uint64_t(key.occurrence));
    }
  };

  // Main thread, before the Draw of a logic pass (after its Update).
  void OnTick(uint8_t* base) {
    camera_blends_.clear();
    for (int pass = 0; pass < kCameraPassCount; pass++) {
      std::optional<Transform> camera = CameraOf(base, pass);
      if (camera && cameras_[pass]) {
        if (IsCut(*cameras_[pass], *camera)) {
          counts_.camera_cuts++;
        } else {
          camera_blends_.push_back({pass, *cameras_[pass], *camera,
                                    LoadMatrix(base, CameraPass(pass) + kProjectionOffset)});
        }
      }
      cameras_[pass] = camera;
    }
    objects_last_tick_ = std::move(objects_this_tick_);
    objects_this_tick_.clear();
  }

  // The camera of a pass whose view-projection is its view * projection (set
  // by game logic, rather than recomputed in Draw).
  static std::optional<Transform> CameraOf(uint8_t* base, int pass) {
    uint32_t address = CameraPass(pass);
    Matrix4 view = LoadMatrix(base, address + kViewOffset);
    Matrix4 projection = LoadMatrix(base, address + kProjectionOffset);
    Matrix4 view_projection = LoadMatrix(base, address + kViewProjectionOffset);
    if (!IsFinite(view) || !IsFinite(projection) || !IsFinite(view_projection) ||
        !IsProduct(view_projection, view, projection)) {
      return std::nullopt;
    }
    return TransformFromViewMatrix(view);
  }

  static bool IsCameraViewProjection(uint32_t address) {
    return address >= kCameraPasses && address < CameraPass(kCameraPassCount) &&
           (address - kCameraPasses) % kCameraPassBytes == kViewProjectionOffset;
  }

  void Save(uint8_t* base, uint32_t address) {
    saved_.push_back({address, LoadMatrix(base, address)});
  }

  static void Restore(uint8_t* base, std::vector<std::pair<uint32_t, Matrix4>>& saved) {
    for (auto it = saved.rbegin(); it != saved.rend(); ++it) {
      StoreMatrix(base, it->first, it->second);
    }
    saved.clear();
  }

  void LogCounts() {
    if (!REXCVAR_GET(log_blend_state)) {
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - counts_.last_log < std::chrono::seconds(1)) {
      return;
    }
    int draws = std::max(counts_.draws, 1);
    REXLOG_INFO(
        "blend state: {} draws; per draw {} cameras, {} objects ({} unmatched, {} not "
        "transforms); {} camera cuts, {} object cuts",
        counts_.draws, counts_.cameras / draws, counts_.objects / draws,
        counts_.objects_unmatched / draws, counts_.objects_not_transforms / draws,
        counts_.camera_cuts, counts_.object_cuts);
    counts_ = {};
    counts_.last_log = now;
  }

  bool logic_pass_ = false;
  bool enabled_ = false;
  float amount_ = 0;

  std::array<std::optional<Transform>, kCameraPassCount> cameras_;
  std::vector<CameraBlend> camera_blends_;

  std::unordered_map<ObjectKey, ScaledTransform, ObjectKeyHash> objects_last_tick_;
  std::unordered_map<ObjectKey, ScaledTransform, ObjectKeyHash> objects_this_tick_;
  std::unordered_map<uint32_t, int> recorded_;

  std::vector<std::pair<uint32_t, Matrix4>> saved_;
  std::vector<std::pair<uint32_t, Matrix4>> object_saved_;

  DrawStateDump dump_;

  struct Counts {
    int draws = 0;
    int cameras = 0;
    int objects = 0;
    int objects_unmatched = 0;
    int objects_not_transforms = 0;
    int camera_cuts = 0;
    int object_cuts = 0;
    std::chrono::steady_clock::time_point last_log{};
  } counts_;
};

DrawState& GetDrawState() {
  static DrawState state;
  return state;
}

}  // namespace

void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass) {
  GetDrawState().BeforeDraw(base, !render_only_pass);
}

void BlendState_AfterDraw(uint8_t* base) {
  GetDrawState().AfterDraw(base);
}

void BlendState_BeforeRecordObject(uint8_t* base, uint32_t view_projection, uint32_t world) {
  GetDrawState().BeforeRecordObject(base, view_projection, world);
}

void BlendState_AfterRecordObject(uint8_t* base) {
  GetDrawState().AfterRecordObject(base);
}

}  // namespace interpolation
