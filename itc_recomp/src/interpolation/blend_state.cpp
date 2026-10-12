// Interpolated drawing, in the usual engine way: game logic ticks at 30 Hz,
// and each frame is drawn from a blend of the last two ticks.
//
// What drawing reads (measured in races):
//
// - Camera passes: 21 objects of 592 bytes at 0x826BAC00, each with a view
//   matrix at +0x10, a projection at +0x50 and their product (view *
//   projection) at +0x90. Pass 0 is the main 1280x720 camera. Draw rebuilds
//   the view from the camera's eye, look-at target and up vector (+0x190,
//   +0x1A0, +0x1B0; sub_820A3978), so those are what's blended.
// - Objects: Draw hands each object's world matrix to the per-object
//   recorder (sub_825953D0) with a camera's view-projection; the recorder
//   records world * view-projection and world as shader constants. Each
//   world matrix is blended with its previous tick's, matched by address and
//   by how many times that address was recorded earlier in the Draw (objects
//   are visited in the same order every tick).
// - Commands recorded once per tick: some (the scenery's per-pass constants)
//   are recorded only on logic passes, and the render thread replays them
//   unchanged on every frame of the tick. Their constants are made with the
//   game's matrix multiply (sub_8209BD50) from a world matrix and a camera
//   pass's view-projection, so each multiply into a camera is noted by its
//   output address; when the render thread replays a constant upload from a
//   noted address, it uploads world * that camera's view-projection for the
//   frame instead.
//
// Drawing runs a tick behind: the logic pass draws the previous tick (amount
// 0) and the passes after it move towards the current one. Blended values are
// put back after Draw, so game logic never sees them.

#include "blend_state.h"

#include "frame_pacing/frame_pacer.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(interpolate_draw_state, true, "ITC",
                    "Smooth motion between 30 Hz ticks in races by blending cameras and "
                    "object transforms");
REXCVAR_DEFINE_BOOL(log_blend_state, false, "ITC",
                    "Log how many cameras and objects were blended, once a second");
REXCVAR_DEFINE_BOOL(log_replayed_constants, false, "ITC",
                    "Log the vertex constant matrices the render thread replays, once a second");

namespace interpolation {
namespace {

constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr int kCameraPassCount = 21;
constexpr uint32_t kCameraPassBytes = 592;
constexpr uint32_t kViewOffset = 0x10;
constexpr uint32_t kProjectionOffset = 0x50;
constexpr uint32_t kViewProjectionOffset = 0x90;
// Eye position, look-at target and up vector, 3 floats each.
constexpr uint32_t kLookAtOffsets[] = {0x190, 0x1A0, 0x1B0};

// Moving further than this in one tick is a cut (a camera switch, a reset):
// drawn without blending.
constexpr float kCutDistance = 8.0f;  // metres
constexpr float kCutAngle = 0.5f;     // radians, ~30 degrees

// How long a noted camera product is kept: the command list is double
// buffered, so a recording is replayed for at most the next tick.
constexpr uint32_t kProductLifetimeTicks = 2;

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

Vec3 LoadVec3(uint8_t* base, uint32_t address) {
  return {guest::LoadFloat(base, address), guest::LoadFloat(base, address + 4),
          guest::LoadFloat(base, address + 8)};
}

void StoreVec3(uint8_t* base, uint32_t address, const Vec3& v) {
  for (int k = 0; k < 3; k++) {
    guest::StoreFloat(base, address + 4 * k, v[k]);
  }
}

bool IsFinite(const Vec3& v) {
  return std::all_of(v.begin(), v.end(), [](float x) { return std::isfinite(x); });
}

Matrix4 Load(uint8_t* base, uint32_t address, const Matrix4*) {
  return LoadMatrix(base, address);
}
Vec3 Load(uint8_t* base, uint32_t address, const Vec3*) {
  return LoadVec3(base, address);
}
void Store(uint8_t* base, uint32_t address, const Matrix4& m) {
  StoreMatrix(base, address, m);
}
void Store(uint8_t* base, uint32_t address, const Vec3& v) {
  StoreVec3(base, address, v);
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

// The camera pass whose view-projection is at `address`, or -1.
int PassOfViewProjection(uint32_t address) {
  if (address < kCameraPasses || address >= CameraPass(kCameraPassCount) ||
      (address - kCameraPasses) % kCameraPassBytes != kViewProjectionOffset) {
    return -1;
  }
  return int((address - kCameraPasses) / kCameraPassBytes);
}

// A constant made with the matrix multiply from a world matrix and a camera.
struct CameraProduct {
  int pass;
  Matrix4 world;
  Matrix4 written;  // the product, as written to the output address
  uint32_t tick;
};

// What the render thread needs to redo camera products for a frame:
// published by the main thread after each Draw.
struct ReplayFrame {
  std::array<Matrix4, kCameraPassCount> view_projections{};  // as Draw left them
  std::unordered_map<uint32_t, CameraProduct> products;      // by output address
};

std::mutex g_replay_frame_mutex;
std::shared_ptr<const ReplayFrame> g_replay_frame;

class DrawState {
 public:
  // Main thread, after the matrix multiply wrote out = a * b.
  void OnMultiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b) {
    // Draw and game logic run on the main thread; loading threads multiply too.
    if (std::this_thread::get_id() != main_thread_) {
      return;
    }
    int pass = PassOfViewProjection(b);
    if (pass < 0) {
      return;
    }
    products_[out] = {pass, LoadMatrix(base, a), LoadMatrix(base, out), tick_};
  }

  void BeforeDraw(uint8_t* base, bool logic_pass) {
    main_thread_ = std::this_thread::get_id();
    logic_pass_ = logic_pass;
    if (logic_pass) {
      OnTick(base);
    }
    amount_ = frame_pacing::GetFramePacer().TickProgress();
    recorded_.clear();
    enabled_ = REXCVAR_GET(interpolate_draw_state);
    if (!enabled_) {
      return;
    }
    // Draw rebuilds each camera's view from these; blend them, as positions.
    for (const LookAtBlend& blend : look_at_blends_) {
      for (int i = 0; i < 3; i++) {
        Vec3 v;
        for (int k = 0; k < 3; k++) {
          v[k] = blend.from[i][k] + (blend.to[i][k] - blend.from[i][k]) * amount_;
        }
        Replace(base, saved_vec3_, CameraPass(blend.pass) + kLookAtOffsets[i], v);
      }
      counts_.look_ats++;
    }
    // Cameras Draw doesn't rebuild (the pause menu's): blend their matrices.
    for (const CameraBlend& camera : camera_blends_) {
      uint32_t pass = CameraPass(camera.pass);
      Matrix4 view = ViewMatrixFromTransform(Blend(camera.from, camera.to, amount_));
      Replace(base, saved_, pass + kViewOffset, view);
      Replace(base, saved_, pass + kViewProjectionOffset, Multiply(view, camera.projection));
      counts_.cameras++;
    }
  }

  void AfterDraw(uint8_t* base) {
    PublishReplayFrame(base);
    Restore(base, saved_);
    Restore(base, saved_vec3_);
    counts_.draws++;
    LogCounts();
  }

  void BeforeRecordObject(uint8_t* base, uint32_t view_projection, uint32_t world) {
    object_saved_.clear();
    if (PassOfViewProjection(view_projection) < 0) {
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
    Replace(base, object_saved_, world,
            WorldMatrixFromScaledTransform(Blend(before->second, *transform, amount_)));
    counts_.objects++;
  }

  void AfterRecordObject(uint8_t* base) {
    Restore(base, object_saved_);
  }

  // Render thread diagnostics.
  void CountReplayedProduct() { counts_.replayed_products++; }

 private:
  struct LookAtBlend {
    int pass;
    std::array<Vec3, 3> from;  // eye, target, up
    std::array<Vec3, 3> to;
  };

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

  template <typename T>
  struct Saved {
    uint32_t address;
    T original;
    T written;
  };

  template <typename T>
  static void Replace(uint8_t* base, std::vector<Saved<T>>& saved, uint32_t address,
                      const T& value) {
    saved.push_back({address, Load(base, address, &value), value});
    Store(base, address, value);
  }

  // Puts the originals back, except where the game has since written
  // something else there (the memory may have been freed and reused, as when
  // a race loads).
  template <typename T>
  static void Restore(uint8_t* base, std::vector<Saved<T>>& saved) {
    for (auto it = saved.rbegin(); it != saved.rend(); ++it) {
      if (Load(base, it->address, &it->written) == it->written) {
        Store(base, it->address, it->original);
      }
    }
    saved.clear();
  }

  // Main thread, before the Draw of a logic pass (after its Update).
  void OnTick(uint8_t* base) {
    tick_++;
    camera_blends_.clear();
    look_at_blends_.clear();
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

      std::array<Vec3, 3> look_at;
      bool finite = true;
      for (int i = 0; i < 3; i++) {
        look_at[i] = LoadVec3(base, CameraPass(pass) + kLookAtOffsets[i]);
        finite &= IsFinite(look_at[i]);
      }
      const auto& before = look_ats_[pass];
      if (finite && before && look_at != *before &&
          Distance((*before)[0], look_at[0]) <= kCutDistance &&
          Distance((*before)[1], look_at[1]) <= kCutDistance) {
        look_at_blends_.push_back({pass, *before, look_at});
      }
      look_ats_[pass] = finite ? std::optional(look_at) : std::nullopt;
    }
    objects_last_tick_ = std::move(objects_this_tick_);
    objects_this_tick_.clear();
    std::erase_if(products_, [&](const auto& entry) {
      return tick_ - entry.second.tick > kProductLifetimeTicks;
    });
  }

  void PublishReplayFrame(uint8_t* base) {
    auto frame = std::make_shared<ReplayFrame>();
    for (int pass = 0; pass < kCameraPassCount; pass++) {
      frame->view_projections[pass] = LoadMatrix(base, CameraPass(pass) + kViewProjectionOffset);
    }
    frame->products = products_;
    std::lock_guard lock(g_replay_frame_mutex);
    g_replay_frame = std::move(frame);
  }

  // The camera of a pass whose view-projection is its view * projection.
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
        "blend state: {} draws; per draw {} look-ats, {} cameras, {} objects ({} unmatched, {} "
        "not transforms), {} replayed camera products; {} camera cuts, {} object cuts; {} "
        "products noted",
        counts_.draws, counts_.look_ats / draws, counts_.cameras / draws,
        counts_.objects / draws, counts_.objects_unmatched / draws,
        counts_.objects_not_transforms / draws, counts_.replayed_products.load() / draws,
        counts_.camera_cuts, counts_.object_cuts, products_.size());
    counts_.Reset();
    counts_.last_log = now;
  }

  std::thread::id main_thread_;
  bool logic_pass_ = false;
  bool enabled_ = false;
  float amount_ = 0;
  uint32_t tick_ = 0;

  std::array<std::optional<Transform>, kCameraPassCount> cameras_;
  std::vector<CameraBlend> camera_blends_;
  std::array<std::optional<std::array<Vec3, 3>>, kCameraPassCount> look_ats_;
  std::vector<LookAtBlend> look_at_blends_;

  std::unordered_map<ObjectKey, ScaledTransform, ObjectKeyHash> objects_last_tick_;
  std::unordered_map<ObjectKey, ScaledTransform, ObjectKeyHash> objects_this_tick_;
  std::unordered_map<uint32_t, int> recorded_;

  std::unordered_map<uint32_t, CameraProduct> products_;  // by output address

  std::vector<Saved<Matrix4>> saved_;
  std::vector<Saved<Vec3>> saved_vec3_;
  std::vector<Saved<Matrix4>> object_saved_;

  struct Counts {
    int draws = 0;
    int look_ats = 0;
    int cameras = 0;
    int objects = 0;
    int objects_unmatched = 0;
    int objects_not_transforms = 0;
    int camera_cuts = 0;
    int object_cuts = 0;
    std::atomic<int> replayed_products = 0;  // render thread
    std::chrono::steady_clock::time_point last_log{};

    void Reset() {
      draws = look_ats = cameras = objects = objects_unmatched = objects_not_transforms =
          camera_cuts = object_cuts = 0;
      replayed_products = 0;
    }
  } counts_;
};

DrawState& GetDrawState() {
  static DrawState state;
  return state;
}

// Render thread: redoes noted camera products with the frame's cameras.
class ReplayCameras {
 public:
  void OnFrameStart() {
    std::lock_guard lock(g_replay_frame_mutex);
    frame_ = g_replay_frame;
  }

  // The value to upload from `payload` in place of `m`, if it's a noted
  // camera product still as written.
  std::optional<Matrix4> Substitute(uint32_t payload, const Matrix4& m) const {
    if (!frame_) {
      return std::nullopt;
    }
    auto it = frame_->products.find(payload);
    if (it == frame_->products.end() || it->second.written != m) {
      return std::nullopt;
    }
    return Multiply(it->second.world, frame_->view_projections[it->second.pass]);
  }

 private:
  std::shared_ptr<const ReplayFrame> frame_;
};

ReplayCameras& GetReplayCameras() {
  static ReplayCameras cameras;
  return cameras;
}

// Diagnostics on the render thread: the vertex shader constant uploads it
// replays each frame (4+ registers), compared with the same upload (by
// order) in the previous frame.
class ReplayedConstantsLog {
 public:
  void OnFrameStart() {
    if (!REXCVAR_GET(log_replayed_constants)) {
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (logging_) {
      logging_ = false;
      REXLOG_INFO("replayed constants: frame with {} uploads, {} changed since the last frame",
                  frame_.size(), changed_);
      for (const auto& line : lines_) {
        REXLOG_INFO("replayed constants: {}", line);
      }
    }
    if (now - last_log_ >= std::chrono::seconds(1) && frames_since_tick_ == 2) {
      logging_ = true;
      last_log_ = now;
    }
    frames_since_tick_++;
    previous_ = std::move(frame_);
    frame_.clear();
    lines_.clear();
    changed_ = 0;
  }

  void OnTick() { frames_since_tick_ = 0; }

  void OnUpload(uint8_t* base, uint32_t payload, uint32_t packed, bool substituted) {
    if (!REXCVAR_GET(log_replayed_constants)) {
      return;
    }
    uint32_t stage = packed >> 24, start = (packed >> 8) & 0xFF, count = packed & 0xFF;
    if (stage != 0 || count < 4) {
      return;
    }
    Matrix4 m = LoadMatrix(base, payload);
    size_t index = frame_.size();
    bool changed = index >= previous_.size() || previous_[index] != m;
    changed_ += changed;
    frame_.push_back(m);
    if (logging_ && lines_.size() < 120) {
      lines_.push_back(fmt::format(
          "#{} c{} x{} from {:08X} {}{} [{:.3g} {:.3g} {:.3g} {:.3g} | ... | {:.4g} {:.4g} "
          "{:.4g} {:.4g}]",
          index, start, count, payload, changed ? "changed" : "same",
          substituted ? " substituted" : "", m[0], m[1], m[2], m[3], m[12], m[13], m[14],
          m[15]));
    }
  }

 private:
  std::vector<Matrix4> frame_, previous_;
  std::vector<std::string> lines_;
  int changed_ = 0;
  int frames_since_tick_ = 0;
  bool logging_ = false;
  std::chrono::steady_clock::time_point last_log_{};
};

ReplayedConstantsLog& GetReplayedConstantsLog() {
  static ReplayedConstantsLog log;
  return log;
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

void BlendState_OnReplayFrameStart() {
  GetReplayedConstantsLog().OnFrameStart();
  GetReplayCameras().OnFrameStart();
}

void BlendState_OnReplayTick() {
  GetReplayedConstantsLog().OnTick();
}

void BlendState_ReplayConstants(uint8_t* base, uint32_t payload, uint32_t packed,
                                const std::function<void()>& upload) {
  uint32_t stage = packed >> 24, count = packed & 0xFF;
  std::optional<Matrix4> original;
  if (REXCVAR_GET(interpolate_draw_state) && stage == 0 && count >= 4) {
    Matrix4 m = LoadMatrix(base, payload);
    if (std::optional<Matrix4> substitute = GetReplayCameras().Substitute(payload, m)) {
      original = m;
      StoreMatrix(base, payload, *substitute);
      GetDrawState().CountReplayedProduct();
    }
  }
  GetReplayedConstantsLog().OnUpload(base, payload, packed, original.has_value());
  upload();
  if (original) {
    StoreMatrix(base, payload, *original);
  }
}

}  // namespace interpolation

// Matrix_Multiply: r3 = out, r4 = a, r5 = b; out = a * b (4x4).
extern "C" REX_FUNC(sub_8209BD50) {
  REX_FUNC_PROLOGUE();
  uint32_t out = ctx.r3.u32, a = ctx.r4.u32, b = ctx.r5.u32;
  __imp__sub_8209BD50(ctx, base);
  interpolation::GetDrawState().OnMultiply(base, out, a, b);
}
