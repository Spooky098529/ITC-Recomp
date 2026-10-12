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
//   pass's view-projection. During each logic pass, a copy of both inputs of
//   every such multiply is kept, keyed by camera, world matrix address and
//   order, and paired with the previous tick's. When the render thread
//   replays a constant upload from a multiply's output, it uploads the
//   frame's blend of the two ticks instead.
//
//   The per-object recorder (sub_825953D0) multiplies too, every pass, often
//   with a copy of a camera on the stack that only changes on ticks (the
//   scenery's); its products are handled the same way, keyed by both
//   operands' addresses and order within the pass.
//
// So that those copies hold the tick's own state, a logic pass draws without
// blending, and all its uploads are blended at replay; the render-only passes
// after it blend as they draw.
//
// Drawing runs a tick behind: the logic pass's frame shows the previous tick
// (amount 0) and the frames after it move towards the current one. Blended
// values are put back after Draw, so game logic never sees them.

#include "blend_state.h"

#include "frame_pacing/frame_pacer.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <map>
#include <memory>
#include <tuple>
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

// A matrix multiply's inputs: a world matrix and a camera's view-projection.
struct ProductInputs {
  Matrix4 world;
  Matrix4 view_projection;
};

// A multiply made during a logic pass, as the render thread redoes it.
struct ReplayedProduct {
  Matrix4 written;  // the product, as written to the output address
  ProductInputs now;
  std::optional<ProductInputs> before;  // the previous tick's, if paired
};

// What the render thread needs to blend camera products for a frame:
// published by the main thread after each Draw.
struct ReplayFrame {
  std::unordered_map<uint32_t, ReplayedProduct> products;  // by output address
  float amount = 1;
};

std::mutex g_replay_mutex;
std::shared_ptr<const ReplayFrame> g_replay_frame;

// The matrix multiply's return address in the per-object recorder
// (sub_825953D0): out = the command's constants, a = world, b = camera.
constexpr uint32_t kRecorderMultiplyReturn = 0x82595438;

// Blends a multiply's result between ticks: the world matrix and the camera
// each as a rigid motion (positions straight, rotations along the shortest
// arc), falling back to the current tick's where they aren't.
// Whether two perspective cameras from consecutive ticks can be the same
// camera: placement within a cut, and nearly the same projection (field of
// view changes slowly, aspect and depth range don't). Stack copies of
// cameras are shared by several views, so a pairing can be two different
// cameras.
bool IsSameCamera(const PerspectiveCamera& a, const PerspectiveCamera& b) {
  auto close = [](float x, float y, float tolerance) {
    return std::abs(x - y) <= tolerance * std::max(std::abs(x), std::abs(y));
  };
  return !IsCut(a.camera, b.camera) && close(a.x_scale, b.x_scale, 0.05f) &&
         close(a.y_scale, b.y_scale, 0.05f) && close(a.w_scale, b.w_scale, 0.01f) &&
         close(a.x_scale / a.y_scale, b.x_scale / b.y_scale, 0.01f) &&
         close(a.depth_scale, b.depth_scale, 0.01f) && close(a.depth_offset, b.depth_offset, 0.01f);
}

struct BlendStats {
  int blended = 0;
  int unpaired = 0;
  int world_not_blended = 0;
  int camera_not_blended = 0;
};

Matrix4 BlendProduct(const ReplayedProduct& product, float amount, BlendStats& stats) {
  if (!product.before) {
    stats.unpaired++;
    return product.written;
  }
  stats.blended++;
  const ProductInputs& a = *product.before;
  const ProductInputs& b = product.now;
  Matrix4 world = b.world;
  std::optional<ScaledTransform> wa = ScaledTransformFromWorldMatrix(a.world);
  std::optional<ScaledTransform> wb = ScaledTransformFromWorldMatrix(b.world);
  if (wa && wb && !IsCut(wa->transform, wb->transform)) {
    world = WorldMatrixFromScaledTransform(Blend(*wa, *wb, amount));
  } else if (a.world != b.world) {
    stats.world_not_blended++;
  }
  Matrix4 view_projection = b.view_projection;
  std::optional<PerspectiveCamera> ca, cb;
  if (a.view_projection != b.view_projection) {
    ca = SplitViewProjection(a.view_projection);
    cb = SplitViewProjection(b.view_projection);
  }
  if (ca && cb) {
    // A perspective camera: blend its placement and projection separately.
    if (!IsSameCamera(*ca, *cb)) {
      stats.camera_not_blended++;
    } else {
      view_projection = ViewProjectionFromCamera(Blend(*ca, *cb, amount));
    }
  } else if (a.view_projection != b.view_projection) {
    if (std::optional<RigidMotion> motion = FindRigidMotion(a.view_projection, b.view_projection);
        motion && Distance(motion->motion.position, Vec3{}) < 1e6f &&
        AngleBetween(motion->motion.rotation, Quaternion{}) <= kCutAngle) {
      view_projection = BlendRigidMotion(*motion, amount);
    } else {
      stats.camera_not_blended++;
    }
  }
  return Multiply(world, view_projection);
}

class DrawState {
 public:
  // Main thread, after the matrix multiply wrote out = a * b.
  void OnMultiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t caller) {
    // Draw and game logic run on the main thread; loading threads multiply too.
    if (std::this_thread::get_id() != main_thread_) {
      return;
    }
    bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
    bool recorder = caller == kRecorderMultiplyReturn;
    // Other multiplies into a camera matter when made by a logic pass (the
    // scenery's, during Update); render-only passes' are already blended.
    if (!recorder && (render_only || PassOfViewProjection(b) < 0)) {
      return;
    }
    ProductKey key{b, a, product_occurrences_[{b, a, 0}]++};
    if (!render_only) {
      collecting_[key] = {LoadMatrix(base, a), LoadMatrix(base, b)};
    }
    auto& outputs = render_only ? pass_outputs_ : tick_outputs_;
    outputs[out] = {key, LoadMatrix(base, out)};
  }

  void BeforeDraw(uint8_t* base, bool logic_pass) {
    main_thread_ = std::this_thread::get_id();
    logic_pass_ = logic_pass;
    if (logic_pass) {
      OnTick(base);
    }
    amount_ = frame_pacing::GetFramePacer().TickProgress();
    recorded_.clear();
    // A logic pass draws the tick as it is (its uploads are blended at replay).
    enabled_ = REXCVAR_GET(interpolate_draw_state) && !logic_pass;
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
    if (logic_pass_) {
      // This tick's products are complete (its Update and Draw).
      previous_tick_ = std::move(current_tick_);
      current_tick_ = std::move(collecting_);
      collecting_.clear();
    }
    PublishReplayFrame();
    pass_outputs_.clear();
    product_occurrences_.clear();
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

  // Main thread, before a logic pass's Update: a new tick's products start.
  void OnLogicPassStart() { tick_outputs_.clear(); }

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
  }

  void PublishReplayFrame() {
    auto frame = std::make_shared<ReplayFrame>();
    frame->amount = REXCVAR_GET(interpolate_draw_state) ? amount_ : 1.0f;
    for (const auto* outputs : {&tick_outputs_, &pass_outputs_}) {
      for (const auto& [out, output] : *outputs) {
        auto now = current_tick_.find(output.key);
        if (now == current_tick_.end()) {
          continue;
        }
        auto before = previous_tick_.find(output.key);
        frame->products[out] = {output.written, now->second,
                                before != previous_tick_.end() ? std::optional(before->second)
                                                               : std::nullopt};
      }
    }
    counts_.products_published += int(frame->products.size());
    std::lock_guard lock(g_replay_mutex);
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
        "products published per draw",
        counts_.draws, counts_.look_ats / draws, counts_.cameras / draws,
        counts_.objects / draws, counts_.objects_unmatched / draws,
        counts_.objects_not_transforms / draws, counts_.replayed_products.load() / draws,
        counts_.camera_cuts, counts_.object_cuts, counts_.products_published / draws);
    counts_.Reset();
    counts_.last_log = now;
  }

  std::thread::id main_thread_;
  bool logic_pass_ = false;
  bool enabled_ = false;
  float amount_ = 0;

  std::array<std::optional<Transform>, kCameraPassCount> cameras_;
  std::vector<CameraBlend> camera_blends_;
  std::array<std::optional<std::array<Vec3, 3>>, kCameraPassCount> look_ats_;
  std::vector<LookAtBlend> look_at_blends_;

  std::unordered_map<ObjectKey, ScaledTransform, ObjectKeyHash> objects_last_tick_;
  std::unordered_map<ObjectKey, ScaledTransform, ObjectKeyHash> objects_this_tick_;
  std::unordered_map<uint32_t, int> recorded_;

  // A multiply, identified across ticks by its operands' addresses and how
  // many times they were multiplied earlier in the pass.
  struct ProductKey {
    uint32_t camera;  // address
    uint32_t world;   // address
    int occurrence;
    bool operator==(const ProductKey&) const = default;
  };
  struct ProductKeyHash {
    size_t operator()(const ProductKey& key) const {
      return std::hash<uint64_t>()(((uint64_t(key.world) << 32) | key.camera) ^
                                   (uint64_t(key.occurrence) << 7));
    }
  };
  using ProductInputsByKey = std::unordered_map<ProductKey, ProductInputs, ProductKeyHash>;
  ProductInputsByKey collecting_;  // this logic pass's
  ProductInputsByKey current_tick_;
  ProductInputsByKey previous_tick_;
  std::unordered_map<ProductKey, int, ProductKeyHash> product_occurrences_;

  struct Output {
    ProductKey key;
    Matrix4 written;
  };
  std::unordered_map<uint32_t, Output> tick_outputs_;  // by address, from the logic pass
  std::unordered_map<uint32_t, Output> pass_outputs_;  // from this render-only pass

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
    int products_published = 0;
    std::atomic<int> replayed_products = 0;  // render thread
    std::chrono::steady_clock::time_point last_log{};

    void Reset() {
      draws = look_ats = cameras = objects = objects_unmatched = objects_not_transforms =
          camera_cuts = object_cuts = products_published = 0;
      replayed_products = 0;
    }
  } counts_;
};

DrawState& GetDrawState() {
  static DrawState state;
  return state;
}

// Render thread: blends the logic pass's camera products for each frame.
class ReplayCameras {
 public:
  void OnFrameStart() {
    LogStats();
    std::lock_guard lock(g_replay_mutex);
    frame_ = g_replay_frame;
  }

  // Render thread, for each vertex constant upload of 4+ registers.
  void CountUpload(uint8_t* base, uint32_t payload, bool substituted) {
    if (!REXCVAR_GET(log_blend_state)) {
      return;
    }
    stats_.uploads++;
    if (substituted) {
      return;
    }
    Matrix4 m = LoadMatrix(base, payload);
    auto it = last_values_.find(payload);
    if (it != last_values_.end() && it->second == m) {
      stats_.not_substituted_same++;
    } else {
      stats_.not_substituted_changed++;
    }
    last_values_[payload] = m;
  }

  // The value to upload from `payload` in place of `m`, if it's a camera
  // product of the tick still as written.
  std::optional<Matrix4> Substitute(uint32_t payload, const Matrix4& m) {
    if (!frame_) {
      return std::nullopt;
    }
    auto it = frame_->products.find(payload);
    if (it == frame_->products.end() || it->second.written != m) {
      return std::nullopt;
    }
    return BlendProduct(it->second, frame_->amount, stats_.blend);
  }

 private:
  void LogStats() {
    if (!REXCVAR_GET(log_blend_state)) {
      return;
    }
    stats_.frames++;
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ < std::chrono::seconds(1)) {
      return;
    }
    int f = std::max(stats_.frames, 1);
    REXLOG_INFO(
        "replay: {} frames; per frame {} uploads, {} blended ({} world, {} camera not blended), "
        "{} unpaired, not replaced: {} unchanged since last frame, {} changed",
        stats_.frames, stats_.uploads / f, stats_.blend.blended / f,
        stats_.blend.world_not_blended / f, stats_.blend.camera_not_blended / f,
        stats_.blend.unpaired / f, stats_.not_substituted_same / f,
        stats_.not_substituted_changed / f);
    stats_ = {};
    last_log_ = now;
  }

  std::shared_ptr<const ReplayFrame> frame_;
  struct {
    int frames = 0;
    int uploads = 0;
    int not_substituted_same = 0;
    int not_substituted_changed = 0;
    BlendStats blend;
  } stats_;
  std::unordered_map<uint32_t, Matrix4> last_values_;
  std::chrono::steady_clock::time_point last_log_{};
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
    full_lines_ = 0;
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
    if (logging_ && start == 9 && count == 5 && full_lines_ < 12) {
      full_lines_++;
      std::string values;
      for (uint32_t i = 0; i < 20; i++) {
        values += fmt::format("{}{:.4g}", i % 4 == 0 ? (i ? " | " : "") : " ",
                              guest::LoadFloat(base, payload + 4 * i));
      }
      lines_.push_back(fmt::format("#{} c9 x5 from {:08X} {}: {}", index, payload,
                                   changed ? "changed" : "same", values));
    }
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
  int full_lines_ = 0;
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

void BlendState_OnLogicPassStart() {
  GetDrawState().OnLogicPassStart();
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
  if (stage == 0 && count >= 4) {
    GetReplayCameras().CountUpload(base, payload, original.has_value());
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
  uint32_t caller = uint32_t(ctx.lr);
  __imp__sub_8209BD50(ctx, base);
  interpolation::GetDrawState().OnMultiply(base, out, a, b, caller);
}

// Diagnostics (--log_slot_callers): who allocates command list slots for
// shader constants (4 or 9 registers) on the main thread, once a second.
REXCVAR_DEFINE_BOOL(log_slot_callers, false, "ITC",
                    "Log which code allocates command list slots for constants, once a second");

namespace {
std::mutex g_slot_callers_mutex;
std::map<std::tuple<uint32_t, uint32_t, bool>, int> g_slot_callers;
std::chrono::steady_clock::time_point g_slot_callers_logged{};
}  // namespace

// CommandList_AllocateSlots: r3 = list, r4 = slot count; returns r3 = the
// first slot (16 bytes each), or 0 if the list is full.
extern "C" REX_FUNC(sub_8258CAA0) {
  REX_FUNC_PROLOGUE();
  uint32_t count = ctx.r4.u32, caller = uint32_t(ctx.lr);
  __imp__sub_8258CAA0(ctx, base);
  if (!REXCVAR_GET(log_slot_callers) || (count != 4 && count != 9)) {
    return;
  }
  std::lock_guard lock(g_slot_callers_mutex);
  g_slot_callers[{caller, count, frame_pacing::GameMessages_IsRenderOnlyPass()}]++;
  auto now = std::chrono::steady_clock::now();
  if (now - g_slot_callers_logged >= std::chrono::seconds(1)) {
    for (const auto& [key, n] : g_slot_callers) {
      REXLOG_INFO("slot callers: {:08X} x{} {} pass: {} calls", std::get<0>(key), std::get<1>(key),
                  std::get<2>(key) ? "render-only" : "logic", n);
    }
    g_slot_callers.clear();
    g_slot_callers_logged = now;
  }
}
