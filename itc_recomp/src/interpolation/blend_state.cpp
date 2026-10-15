// Interpolated drawing, in the usual engine way: game logic ticks at 30 Hz,
// and each frame is drawn from a blend of the last two ticks. See
// "Interpolation" in docs/functions.md.
//
// Almost everything drawn in 3D gets its transform from the game's matrix
// multiply (sub_8209BD50): world * a camera's view-projection, written into a
// shader constant command (command 7) that the render thread replays.
//
// - Main thread: each multiply by a camera is noted by its output address. On
//   a logic pass, a copy of both inputs is kept, keyed by the operands'
//   addresses, which camera it is (CameraIds) and order within the pass, and
//   paired with the previous tick's copy under the same key.
// - Render thread: when a constant upload is replayed from a noted address
//   that still holds what was written, the frame's blend of the two ticks'
//   inputs is uploaded instead (world as position, rotation and scale; the
//   camera as placement plus projection), and the command list put back.
//
// Every pass draws the tick's real state, so the game culls and records the
// same objects in the same order and the pairing holds. (Blending the camera
// and objects while drawing is still here behind --blend_while_drawing.)
//
// Drawing runs a tick behind: the logic pass's frame shows the previous tick
// (amount 0) and the frames after it move towards the current one.

#include "blend_state.h"
#include "debug/replay_stats.h"
#include "native/renderer.h"

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
#include <cstring>
#include <map>
#include <memory>
#include <tuple>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

REXCVAR_DEFINE_BOOL(interpolate_draw_state, true, "ITC",
                    "Smooth motion between 30 Hz ticks in races by blending cameras and "
                    "object transforms");
REXCVAR_DEFINE_BOOL(blend_while_drawing, false, "ITC",
                    "Also blend cameras and objects while render-only passes draw (changes "
                    "what the game culls, so pairings with the logic pass can slip)");
REXCVAR_DEFINE_BOOL(blend_pixel_constants, true, "ITC",
                    "Blend per-object pixel shader constants (lighting) between ticks");
REXCVAR_DEFINE_BOOL(log_stepping_constants, false, "ITC",
                    "Log constant uploads that change at the tick rate rather than every "
                    "frame (left at 30 Hz), with the code that recorded them; needs "
                    "--log_slot_callers");
REXCVAR_DEFINE_BOOL(log_overlay_products, false, "ITC",
                    "Log the race overlays' camera products as replayed, 12 frames every 3 s");
REXCVAR_DEFINE_BOOL(log_overlay_uploads, false, "ITC",
                    "Log every shader constant upload the race overlays recorded, as replayed, "
                    "12 frames every 3 s");
REXCVAR_DECLARE(bool, log_frame_times);
REXCVAR_DEFINE_BOOL(strict_pixel_pairing, true, "ITC",
                    "Leave per-object pixel constants unblended after a draw that isn't a known "
                    "camera product, instead of pairing them by the product before it");
REXCVAR_DEFINE_BOOL(blend_object_extras, true, "ITC",
                    "Blend the per-object uploads' world matrix (c4-c7) and camera position "
                    "(c8) along with c0-c3 (the paint's reflections)");
REXCVAR_DEFINE_BOOL(log_object_extras, false, "ITC",
                    "Log how often c4-c8 of substituted per-object uploads change, with "
                    "examples, every 2 s");
REXCVAR_DEFINE_STRING(debug_zero_owners, "", "ITC",
                      "Testing: vertex or pixel constant uploads recorded by these callers (hex, "
                      "comma-separated; needs --log_slot_callers) are replayed as zeros, so "
                      "their draws vanish");
REXCVAR_DEFINE_BOOL(log_upload_owners, false, "ITC",
                    "Log the callers recording vertex constant uploads, with uploads per "
                    "frame, every 5 s (needs --log_slot_callers)");
REXCVAR_DEFINE_BOOL(log_blend_state, false, "ITC",
                    "Log how many cameras and objects were blended, once a second");
REXCVAR_DEFINE_BOOL(log_replayed_constants, false, "ITC",
                    "Log the vertex constant matrices the render thread replays, once a second");
REXCVAR_DEFINE_BOOL(pixel_blend_from_latest, true, "ITC",
                    "Blend per-object lighting from the last value recorded in the previous "
                    "tick to this frame's (false: only frames recording the tick's first value "
                    "are blended)");
REXCVAR_DEFINE_BOOL(blend_only_light_slots, false, "ITC",
                    "Blend only the per-object lighting's light positions and directions (the "
                    "headlights) and leave the rest as recorded (default: blend all of it)");
REXCVAR_DEFINE_BOOL(blend_lighting_camera, true, "ITC",
                    "Blend the per-object lighting's camera (c0-c3 view, c4 position) as a whole "
                    "(false: by component, with c1.x, c3.xyz and c4.x as switches)");
REXCVAR_DEFINE_BOOL(switch_light_slots, true, "ITC",
                    "A per-object light slot whose light changed between ticks takes the tick's "
                    "light and the rest of the lighting blends (false: the whole upload is left "
                    "as recorded)");
REXCVAR_DEFINE_BOOL(pair_by_object, true, "ITC",
                    "Pair the per-object recorder's products between ticks by the part being "
                    "drawn (its caller's r31), not only by order, so culling or reordering "
                    "doesn't pair a wheel with another wheel or car");
REXCVAR_DEFINE_BOOL(log_moving_products, false, "ITC",
                    "Log moving objects drawn out of step: render-only mismatches, products not "
                    "blended or unpaired, pairings that move unlike their neighbours, and jumps "
                    "between frames");
REXCVAR_DEFINE_BOOL(log_pixel_outcomes, false, "ITC",
                    "Log what happened to the per-object lighting uploads (blended, unpaired, "
                    "rejected...), for moving and still objects, every 2 s");

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

uint64_t HashMatrix(const Matrix4& m) {
  uint64_t h = 1469598103934665603ull;
  for (float f : m) {
    uint32_t bits;
    std::memcpy(&bits, &f, 4);
    h = (h ^ bits) * 1099511628211ull;
  }
  return h;
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
  // For the per-object recorder's multiply, its caller (lr) and the
  // caller's r24-r31 at the call: r31 identifies the part (--pair_by_object).
  std::array<uint32_t, 9> context{};
};

// Main thread: the per-object recorder's caller context while it runs.
std::array<uint32_t, 9> g_record_context{};

// A multiply made during a logic pass, as the render thread redoes it.
struct ReplayedProduct {
  uint64_t identity = 0;  // the same multiply in every tick (its pairing key)
  uint32_t handler = 0;   // the numbering context it was made in
  Matrix4 written;  // the product, as written to the output address
  ProductInputs now;
  std::optional<ProductInputs> before;  // the previous tick's, if paired
};

// What the render thread needs to blend camera products for a frame:
// published by the main thread after each Draw.
struct ReplayFrame {
  std::unordered_map<uint32_t, ReplayedProduct> products;  // by output address
  float amount = 1;
  uint64_t tick = 0;  // which tick the frame draws
};

std::mutex g_replay_mutex;
std::shared_ptr<const ReplayFrame> g_replay_frame;
// Frames the render thread is done with, freed by the main thread: a frame
// holds ~1,500 products, each a node to free.
std::vector<std::shared_ptr<const ReplayFrame>> g_retired_replay_frames;

// The matrix multiply's return address in the per-object recorder
// (sub_825953D0): out = the command's constants, a = world, b = camera.
constexpr uint32_t kRecorderMultiplyReturn = 0x82595438;

// The Update handler that records the race overlays (see per_frame_logic.cpp).
constexpr uint32_t kRaceOverlaysHandler = 0x8215E150;

// Blends a multiply's result between ticks: the world matrix and the camera
// each as a rigid motion (positions straight, rotations along the shortest
// arc), falling back to the current tick's where they aren't.
// Whether two perspective cameras from consecutive ticks can be the same
// camera: placement within a cut, and nearly the same projection (field of
// view changes slowly, aspect and depth range don't). Stack copies of
// cameras are shared by several views, so a pairing can be two different
// cameras.
bool IsSameCameraCheck(const PerspectiveCamera& a, const PerspectiveCamera& b);

std::mutex g_camera_rejects_mutex;
std::string g_camera_rejects;  // a sample, for --log_blend_state

bool IsSameCamera(const PerspectiveCamera& a, const PerspectiveCamera& b) {
  bool same = IsSameCameraCheck(a, b);
  if (!same && REXCVAR_GET(log_blend_state)) {
    std::lock_guard lock(g_camera_rejects_mutex);
    if (g_camera_rejects.size() < 600) {
      g_camera_rejects += fmt::format(
          "\n  moved {:.2f} m, turned {:.3f} rad, x {:.4g}->{:.4g}, y {:.4g}->{:.4g}, w {:.4g}->{:.4g}, "
          "depth {:.4g}->{:.4g} {:.4g}->{:.4g}",
          Distance(a.camera.position, b.camera.position),
          AngleBetween(a.camera.rotation, b.camera.rotation), a.x_scale, b.x_scale, a.y_scale,
          b.y_scale, a.w_scale, b.w_scale, a.depth_scale, b.depth_scale, a.depth_offset,
          b.depth_offset);
    }
  }
  return same;
}

bool IsSameCameraCheck(const PerspectiveCamera& a, const PerspectiveCamera& b) {
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
  int spinning = 0;
};

// What went into a blended product, for the per-object recorder's other
// registers (c4-c7, the world matrix; c8, the camera's position).
struct ProductBlendParts {
  Matrix4 world_now, world;          // the tick's world matrix, and the blend
  std::optional<Vec3> eye_now, eye;  // the camera's position, if it was blended
  // Diagnostics: the product it was paired with.
  const ReplayedProduct* product = nullptr;
};

Matrix4 BlendProduct(const ReplayedProduct& product, float amount, BlendStats& stats,
                     ProductBlendParts* parts = nullptr) {
  if (parts) {
    *parts = {product.now.world, product.now.world, std::nullopt, std::nullopt, &product};
  }
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
  } else if (std::optional<ScaledTransform> spinning;
             wa && wb && Distance(wa->transform.position, wb->transform.position) <= kCutDistance &&
             (spinning = BlendSpinning(*wa, *wb, amount, kCutAngle))) {
    // A wheel: too much spin to blend, but its position and steering can be.
    world = WorldMatrixFromScaledTransform(*spinning);
    stats.spinning++;
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
      PerspectiveCamera blended = Blend(*ca, *cb, amount);
      view_projection = ViewProjectionFromCamera(blended);
      if (parts) {
        parts->eye_now = cb->camera.position;
        parts->eye = blended.camera.position;
      }
    }
  }
  if (parts) {
    parts->world = world;
  }
  // Other cameras (orthographic: the shadow map's, refitted around the car
  // every tick) stay at the tick's, so everything using one agrees within a
  // frame; only the objects move.
  return Multiply(world, view_projection);
}

// Which camera a view-projection belongs to, as far as can be told from
// its projection: aspect ratio and depth mapping stay the same for a view
// while its field of view changes. Several views share stack copies of their
// cameras, so this tells them apart. Each distinct camera seen gets a
// permanent number (matched within a tolerance, so rounding can't split one
// camera); 0 for a non-perspective one. Main thread.
class CameraIds {
 public:
  uint64_t Of(const Matrix4& view_projection) {
    std::optional<PerspectiveCamera> camera = SplitViewProjection(view_projection);
    if (!camera || !(camera->y_scale != 0) || !(camera->depth_offset != 0)) {
      return 0;
    }
    Signature signature{camera->x_scale / camera->y_scale, camera->depth_offset,
                        camera->depth_scale / camera->depth_offset};
    // Look in the signature's bucket and its neighbours, so values either
    // side of a bucket boundary still match.
    std::array<int32_t, 3> bucket = BucketOf(signature);
    for (int d0 = -1; d0 <= 1; d0++) {
      for (int d1 = -1; d1 <= 1; d1++) {
        for (int d2 = -1; d2 <= 1; d2++) {
          auto it = buckets_.find(Key({bucket[0] + d0, bucket[1] + d1, bucket[2] + d2}));
          if (it == buckets_.end()) {
            continue;
          }
          for (const auto& [known, id] : it->second) {
            if (IsClose(known, signature)) {
              return id;
            }
          }
        }
      }
    }
    uint64_t id = ++count_;
    buckets_[Key(bucket)].push_back({signature, id});
    return id;
  }

 private:
  using Signature = std::array<float, 3>;
  static bool IsClose(const Signature& a, const Signature& b) {
    for (int i = 0; i < 3; i++) {
      if (!(std::abs(a[i] - b[i]) <= 0.01f * std::max({std::abs(a[i]), std::abs(b[i]), 1e-3f}))) {
        return false;
      }
    }
    return true;
  }
  // Buckets about 1% wide, on a log scale, with the sign kept.
  static std::array<int32_t, 3> BucketOf(const Signature& s) {
    std::array<int32_t, 3> b;
    for (int i = 0; i < 3; i++) {
      float magnitude = std::max(std::abs(s[i]), 1e-3f);
      int32_t k = int32_t(std::lround(std::log(magnitude) / 0.01f));
      b[i] = s[i] < 0 ? -k - 100000 : k;
    }
    return b;
  }
  static uint64_t Key(const std::array<int32_t, 3>& b) {
    return (uint64_t(uint32_t(b[0])) * 0x9E3779B97F4A7C15ull) ^ (uint64_t(uint32_t(b[1])) << 21) ^
           (uint64_t(uint32_t(b[2])) * 0xC2B2AE3D27D4EB4Full);
  }
  std::unordered_map<uint64_t, std::vector<std::pair<Signature, uint64_t>>> buckets_;
  uint64_t count_ = 0;
};

}  // namespace

// Diagnostics (--log_slot_callers): which code allocated each command slot.
std::mutex g_slot_owners_mutex;
std::unordered_map<uint32_t, uint32_t> g_slot_owners;

uint32_t SlotOwner(uint32_t address) {
  std::lock_guard lock(g_slot_owners_mutex);
  auto it = g_slot_owners.find(address);
  return it != g_slot_owners.end() ? it->second : 0;
}

// Diagnostics (--log_overlay_uploads): command slots allocated while the race
// overlays ran (by slot address), with the pass that recorded them.
struct OverlaySlot {
  uint64_t pass;
  bool render_only;
};
std::mutex g_overlay_slots_mutex;
std::unordered_map<uint32_t, std::optional<OverlaySlot>> g_overlay_slots;

// Allocations made while the race overlays record (logic passes), for
// OverlayAllocations_Log.
struct OverlayAllocation {
  uint32_t address, count, caller;
};
std::vector<OverlayAllocation> g_overlay_allocations;

std::optional<OverlaySlot> OverlaySlotOf(uint32_t address) {
  std::lock_guard lock(g_overlay_slots_mutex);
  auto it = g_overlay_slots.find(address);
  return it != g_overlay_slots.end() ? it->second : std::nullopt;
}

namespace {

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
    ProductInputs inputs{LoadMatrix(base, a), LoadMatrix(base, b)};
    if (recorder) {
      inputs.context = g_record_context;
    }
    // Any multiply by a camera: the recorder's, one by a camera pass, or one
    // by a perspective view-projection wherever it's kept (the light
    // billboards' recorder, sub_8209C1A0, builds one on its stack).
    if (!recorder && PassOfViewProjection(b) < 0 && camera_ids_.Of(inputs.view_projection) == 0) {
      return;
    }
    Note(base, out, a, b, inputs, render_only);
  }

  // Main thread, after the matrix copy dst = src. A copy of a camera (as
  // identity * camera) or of a noted product carries on as that product.
  void OnCopy(uint8_t* base, uint32_t dst, uint32_t src) {
    if (std::this_thread::get_id() != main_thread_) {
      return;
    }
    bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
    Matrix4 value = LoadMatrix(base, src);
    if (PassOfViewProjection(src) >= 0 || camera_ids_.Of(value) != 0) {
      // A copy of a camera (a camera pass's, or a perspective one built on
      // the stack, like the headlight projector's).
      Matrix4 identity{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
      Note(base, dst, 0, src, {identity, value}, render_only);
      counts_.copies++;
      return;
    }
    auto& outputs = render_only ? pass_outputs_ : tick_outputs_;
    auto it = outputs.find(src);
    if (render_only && it == outputs.end()) {
      // A product made only on the logic pass (the race overlays compute
      // the marker above opponents in their update, which render-only
      // passes skip), copied again for drawing: carry on as the logic
      // pass's product, or the copy went up unblended on render-only
      // passes and blended on logic passes (two markers).
      it = tick_outputs_.find(src);
      if (it == tick_outputs_.end()) {
        return;
      }
    }
    if (it == outputs.end() && !render_only) {
      return;
    }
    if (it->second.written != LoadMatrix(base, src)) {
      return;
    }
    outputs[dst] = {it->second.key, LoadMatrix(base, dst)};
    counts_.copies++;
  }

  // A product out = world * camera, by the operands' addresses (world 0 for
  // an identity) and their values.
  void Note(uint8_t* base, uint32_t out, uint32_t world, uint32_t camera,
            const ProductInputs& inputs, bool render_only) {

    // The race overlays read the camera passes as game logic left them (put
    // back for their recording on render-only passes), so the address alone
    // says which camera; their projection there drifts past CameraIds'
    // tolerance between ticks, which renumbered them and left them unpaired.
    uint64_t signature =
        handler_ == kRaceOverlaysHandler ? 0 : camera_ids_.Of(inputs.view_projection);
    // The per-object recorder's caller (0x820D0564 for cars' parts) keeps a
    // pointer to the part it draws in r31. Numbering products by order alone
    // paired a wheel with another wheel, or another car, whenever a part was
    // culled or the cars were drawn in another order.
    uint64_t object = REXCVAR_GET(pair_by_object) && inputs.context[0] != 0
                          ? (uint64_t(inputs.context[0]) << 32) | inputs.context[8]
                          : 0;
    ProductKey key{camera, world, signature, handler_, object,
                   product_occurrences_[{camera, world, signature, handler_, object, 0}]++};
    if (object != 0 && !render_only) {
      object_keys_.keyed++;
      object_keys_.shared += key.occurrence > 0;
    }
    if (REXCVAR_GET(log_overlay_products) && handler_ == kRaceOverlaysHandler &&
        overlay_keys_logged_ < 24 &&
        (!render_only || previous_tick_.count(key) == 0)) {
      overlay_keys_logged_++;
      REXLOG_INFO("overlay key ({}{}): out {:08X} camera {:08X} world {:08X} id {} occurrence {} "
                  "(tick {}) in current tick {} previous {}",
                  render_only ? "render-only" : "logic", world == 0 ? ", copy" : "", out,
                  key.camera, key.world,
                  key.signature, key.occurrence, ticks_, current_tick_.count(key),
                  previous_tick_.count(key));
    }
    if (handler_ == kRaceOverlaysHandler) {
      // The race overlays' recording on a render-only pass replays the logic
      // pass's exactly (same state, the logic pass's cameras put back), but
      // numbers its products differently, so each is matched to the logic
      // pass's product with the same value instead.
      Matrix4 written = LoadMatrix(base, out);
      uint64_t value = HashMatrix(written);
      if (!render_only) {
        overlay_keys_this_tick_[value] = key;
      } else {
        auto by_value = overlay_keys_.find(value);
        if (by_value == overlay_keys_.end()) {
          counts_.mismatched_products++;
          return;
        }
        pass_outputs_[out] = {by_value->second, written};
        return;
      }
    }
    if (!render_only) {
      collecting_[key] = inputs;
    } else if (!enabled_) {
      // A render-only pass draws the same state as the logic pass, so the
      // same multiply should have the same inputs; if not, the pairing
      // slipped (something was culled differently) and it's left alone.
      auto tick = current_tick_.find(key);
      // The race overlays (the marker above opponents, Update handler
      // sub_8215E150) are recorded again on render-only passes with slightly
      // different inputs; they're blended from the logic pass's inputs under
      // the same key, so they match the logic pass's frame (otherwise the
      // marker drew a tick apart on the two: two markers).
      bool overlay = handler_ == kRaceOverlaysHandler && tick != current_tick_.end();
      if (!overlay && (tick == current_tick_.end() || tick->second.world != inputs.world ||
                       tick->second.view_projection != inputs.view_projection)) {
        counts_.mismatched_products++;
        if (REXCVAR_GET(log_moving_products)) {
          LogMismatch(key, tick == current_tick_.end() ? nullptr : &tick->second, inputs);
        }
        return;
      }
    }
    auto& outputs = render_only ? pass_outputs_ : tick_outputs_;
    outputs[out] = {key, LoadMatrix(base, out)};
  }

  // Main thread: which message handler is running (0 between handlers).
  // Products are numbered per handler, so a handler run alone on a
  // render-only pass (the race overlays) pairs with its logic pass self.
  void SetHandler(uint32_t handler) { handler_ = handler; }
  uint32_t handler() const { return handler_; }
  std::thread::id main_thread() const { return main_thread_; }

  void BeforeDraw(uint8_t* base, bool logic_pass) {
    drawing_ = true;
    main_thread_ = std::this_thread::get_id();
    logic_pass_ = logic_pass;
    if (logic_pass) {
      OnTick(base);
    }
    amount_ = frame_pacing::GetFramePacer().TickProgress();
    recorded_.clear();
    // A logic pass draws the tick as it is (its uploads are blended at replay).
    enabled_ = REXCVAR_GET(interpolate_draw_state) && REXCVAR_GET(blend_while_drawing) &&
               !logic_pass;
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
    drawing_ = false;
    base_ = base;
    if (logic_pass_) {
      ticks_++;
      if (ticks_ % 90 == 0 || ticks_ % 90 == 1) {
        overlay_keys_logged_ = 0;  // log two consecutive ticks every 3 s
      }
      // This tick's products are complete (its Update and Draw).
      previous_tick_ = std::move(current_tick_);
      current_tick_ = std::move(collecting_);
      collecting_.clear();
      MatchTick();
      overlay_keys_ = std::move(overlay_keys_this_tick_);
      overlay_keys_this_tick_.clear();
    }
    PublishReplayFrame();
    pass_outputs_.clear();
    product_occurrences_.clear();
    Restore(base, saved_);
    Restore(base, saved_vec3_);
    counts_.draws++;
    LogCounts();
    if (REXCVAR_GET(log_moving_products) && logic_pass_) {
      object_keys_.ticks++;
      auto now = std::chrono::steady_clock::now();
      if (now - object_keys_.last_log >= std::chrono::seconds(2)) {
        REXLOG_INFO("moving: per tick, {:.1f} products keyed by object, {:.1f} sharing one",
                    double(object_keys_.keyed) / std::max(object_keys_.ticks, 1),
                    double(object_keys_.shared) / std::max(object_keys_.ticks, 1));
        object_keys_ = {};
        object_keys_.last_log = now;
      }
    }
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

  // Main thread, around the light billboards' vertex generation
  // (sub_8209C6E0), which turns each quad to face the camera pass at
  // `pass_address`: give it the frame's blended camera, so the quads face the
  // camera they're drawn from (their camera constants are blended at replay).
  void BeforeBillboards(uint8_t* base, uint32_t pass_address) {
    billboard_saved_.clear();
    billboard_saved_vec3_.clear();
    // Only while Draw runs, where amount_ is this frame's: billboards built
    // during Update (the marker above opponents, by the race overlays) are
    // left facing the tick's camera, the same on every pass.
    if (!REXCVAR_GET(interpolate_draw_state) || !drawing_) {
      return;
    }
    for (const LookAtBlend& blend : look_at_blends_) {
      if (CameraPass(blend.pass) != pass_address) {
        continue;
      }
      for (int i = 0; i < 3; i++) {
        Vec3 v;
        for (int k = 0; k < 3; k++) {
          v[k] = blend.from[i][k] + (blend.to[i][k] - blend.from[i][k]) * amount_;
        }
        Replace(base, billboard_saved_vec3_, pass_address + kLookAtOffsets[i], v);
      }
    }
    for (const CameraBlend& camera : camera_blends_) {
      if (CameraPass(camera.pass) != pass_address) {
        continue;
      }
      Matrix4 view = ViewMatrixFromTransform(Blend(camera.from, camera.to, amount_));
      Replace(base, billboard_saved_, pass_address + kViewOffset, view);
      Replace(base, billboard_saved_, pass_address + kViewProjectionOffset,
              Multiply(view, camera.projection));
    }
  }

  void AfterBillboards(uint8_t* base) {
    Restore(base, billboard_saved_);
    Restore(base, billboard_saved_vec3_);
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
    frame->tick = ticks_;
    for (const auto* outputs : {&tick_outputs_, &pass_outputs_}) {
      for (const auto& [out, output] : *outputs) {
        auto now = current_tick_.find(output.key);
        if (now == current_tick_.end()) {
          continue;
        }
        auto paired = paired_.find(output.key);
        auto before = paired != paired_.end() ? previous_tick_.find(paired->second)
                                              : previous_tick_.end();
        auto identity = identities_.find(output.key);
        frame->products[out] = {identity != identities_.end() ? identity->second
                                                              : ProductKeyHash()(output.key),
                                output.key.handler, output.written, now->second,
                                before != previous_tick_.end() ? std::optional(before->second)
                                                               : std::nullopt};
      }
    }
    counts_.products_published += int(frame->products.size());
    std::shared_ptr<const ReplayFrame> replaced = std::move(frame);
    {
      std::lock_guard lock(g_replay_mutex);
      std::swap(g_replay_frame, replaced);
      retired_frames_.swap(g_retired_replay_frames);  // hands back an emptied one
    }
    retired_frames_.clear();  // the frames are freed here, outside the lock
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
        "products published per draw, {} mismatched, {} copies",
        counts_.draws, counts_.look_ats / draws, counts_.cameras / draws,
        counts_.objects / draws, counts_.objects_unmatched / draws,
        counts_.objects_not_transforms / draws, counts_.replayed_products.load() / draws,
        counts_.camera_cuts, counts_.object_cuts, counts_.products_published / draws,
        counts_.mismatched_products / draws, counts_.copies / draws);
    if (REXCVAR_GET(log_replayed_constants)) {
      auto print = [&](const char* name, uint32_t offset) {
        Matrix4 m = LoadMatrix(base_, CameraPass(0) + offset);
        REXLOG_INFO("pass 0 {}: {:.4g} {:.4g} {:.4g} {:.4g} | {:.4g} {:.4g} {:.4g} {:.4g} | {:.4g} "
                    "{:.4g} {:.4g} {:.4g} | {:.4g} {:.4g} {:.4g} {:.4g}",
                    name, m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10],
                    m[11], m[12], m[13], m[14], m[15]);
      };
      print("view", kViewOffset);
      print("projection", kProjectionOffset);
      print("view-projection", kViewProjectionOffset);
    }
    counts_.Reset();
    counts_.last_log = now;
  }

  std::thread::id main_thread_;
  uint32_t handler_ = 0;
  bool drawing_ = false;  // between BeforeDraw and AfterDraw
  int overlay_keys_logged_ = 0;
  uint8_t* base_ = nullptr;
  uint64_t ticks_ = 0;
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
  // Exchanged with g_retired_replay_frames, so the render thread's list keeps
  // its capacity.
  std::vector<std::shared_ptr<const ReplayFrame>> retired_frames_;

  // A multiply, identified across ticks by its operands' addresses, which
  // camera it was (see CameraIds), and how many times that combination
  // was multiplied earlier in the pass.
  struct ProductKey {
    uint32_t camera;     // address
    uint32_t world;      // address
    uint64_t signature;  // CameraIds number
    uint32_t handler;    // the message handler running (0 for none)
    uint64_t object;     // the part being drawn, if known (--pair_by_object)
    int occurrence;
    bool operator==(const ProductKey&) const = default;
  };
  struct ProductKeyHash {
    size_t operator()(const ProductKey& key) const {
      return std::hash<uint64_t>()(((uint64_t(key.world) << 32) | key.camera) ^
                                   (uint64_t(key.occurrence) << 7) ^ (key.signature * 31) ^
                                   (uint64_t(key.handler) * 0x9E3779B97F4A7C15ull) ^
                                   (key.object * 0xC2B2AE3D27D4EB4Full));
    }
  };
  using ProductInputsByKey = std::unordered_map<ProductKey, ProductInputs, ProductKeyHash>;
  ProductInputsByKey collecting_;  // this logic pass's
  ProductInputsByKey current_tick_;
  ProductInputsByKey previous_tick_;
  std::unordered_map<ProductKey, int, ProductKeyHash> product_occurrences_;
  // Each of this tick's products' previous-tick counterpart, and a number
  // that stays with an object from tick to tick (carried along the pairing),
  // which the pixel constants are paired by.
  std::unordered_map<ProductKey, ProductKey, ProductKeyHash> paired_;
  std::unordered_map<ProductKey, uint64_t, ProductKeyHash> identities_;
  uint64_t next_identity_ = 1;

  // Once a tick: pairs this tick's products with the previous tick's.
  // Products are numbered by order among those with the same operands, and
  // the order changes when something is culled or the cars are drawn in
  // another order. For the per-object recorder's products (keyed by the
  // part's mesh, which cars of a model and a car's wheels share), each group
  // with the same key but for that number is matched by position instead:
  // first the parts with no sibling within kCutDistance (a car's body),
  // whose movement then predicts the rest (its wheels, matched to the
  // previous tick's nearest after moving back by it). Others pair by key.
  void MatchTick() {
    paired_.clear();
    auto previous_identities = std::move(identities_);
    identities_.clear();
    using Entry = std::pair<const ProductKey, ProductInputs>;
    auto group_of = [](ProductKey key) {
      key.occurrence = 0;
      return key;
    };
    auto position = [](const ProductInputs& inputs) {
      return Vec3{inputs.world[12], inputs.world[13], inputs.world[14]};
    };
    std::unordered_map<ProductKey, std::vector<const Entry*>, ProductKeyHash> now_groups,
        before_groups;
    for (const Entry& entry : current_tick_) {
      if (entry.first.object != 0) {
        now_groups[group_of(entry.first)].push_back(&entry);
      } else if (previous_tick_.count(entry.first)) {
        paired_[entry.first] = entry.first;
      }
    }
    for (const Entry& entry : previous_tick_) {
      if (entry.first.object != 0) {
        before_groups[group_of(entry.first)].push_back(&entry);
      }
    }
    auto has_sibling = [&](const std::vector<const Entry*>& group, const Entry* entry) {
      for (const Entry* other : group) {
        if (other != entry &&
            Distance(position(other->second), position(entry->second)) <= kCutDistance) {
          return true;
        }
      }
      return false;
    };
    // Anchors: alone in their group within kCutDistance in both ticks.
    struct Anchor {
      Vec3 now, moved;
    };
    std::vector<Anchor> anchors;
    std::unordered_set<const Entry*> used;
    for (const auto& [group, now] : now_groups) {
      auto before = before_groups.find(group);
      if (before == before_groups.end()) {
        continue;
      }
      for (const Entry* n : now) {
        if (has_sibling(now, n)) {
          continue;
        }
        const Entry* match = nullptr;
        int candidates = 0;
        for (const Entry* b : before->second) {
          if (Distance(position(b->second), position(n->second)) <= kCutDistance) {
            match = b;
            candidates++;
          }
        }
        if (candidates == 1 && !has_sibling(before->second, match)) {
          paired_[n->first] = match->first;
          used.insert(match);
          Vec3 p = position(n->second), q = position(match->second);
          anchors.push_back({p, {p[0] - q[0], p[1] - q[1], p[2] - q[2]}});
        }
      }
    }
    // The rest: nearest after moving back by the nearest anchor's movement.
    for (const auto& [group, now] : now_groups) {
      auto before = before_groups.find(group);
      if (before == before_groups.end()) {
        continue;
      }
      struct Candidate {
        float cost;
        const Entry* n;
        const Entry* b;
      };
      std::vector<Candidate> candidates;
      for (const Entry* n : now) {
        if (paired_.count(n->first)) {
          continue;
        }
        Vec3 p = position(n->second);
        Vec3 moved{};
        float nearest = kCutDistance;
        for (const Anchor& anchor : anchors) {
          float d = Distance(anchor.now, p);
          if (d <= nearest) {
            nearest = d;
            moved = anchor.moved;
          }
        }
        Vec3 predicted{p[0] - moved[0], p[1] - moved[1], p[2] - moved[2]};
        for (const Entry* b : before->second) {
          if (used.count(b)) {
            continue;
          }
          float cost = Distance(position(b->second), predicted);
          if (cost <= kCutDistance) {
            // Ties (several draws of one object) keep the order.
            candidates.push_back(
                {cost + (b->first.occurrence != n->first.occurrence ? 1e-3f : 0.0f), n, b});
          }
        }
      }
      std::sort(candidates.begin(), candidates.end(),
                [](const Candidate& x, const Candidate& y) { return x.cost < y.cost; });
      for (const Candidate& c : candidates) {
        if (paired_.count(c.n->first) || used.count(c.b)) {
          continue;
        }
        paired_[c.n->first] = c.b->first;
        used.insert(c.b);
      }
    }
    for (const Entry& entry : current_tick_) {
      auto paired = paired_.find(entry.first);
      auto identity = paired != paired_.end() ? previous_identities.find(paired->second)
                                              : previous_identities.end();
      identities_[entry.first] =
          identity != previous_identities.end() ? identity->second : next_identity_++;
    }
  }
  struct {
    int ticks = 0, keyed = 0, shared = 0;
    std::chrono::steady_clock::time_point last_log{};
  } object_keys_;  // --log_moving_products

  // Diagnostics (--log_moving_products): a render-only pass's multiply whose
  // inputs differ from the logic pass's under the same key. It isn't
  // published, so its upload is drawn as recorded (the tick's state).
  void LogMismatch(const ProductKey& key, const ProductInputs* tick, const ProductInputs& now) {
    static auto last = std::chrono::steady_clock::now();
    static int lines = 0, total = 0;
    total++;
    auto t = std::chrono::steady_clock::now();
    if (t - last >= std::chrono::seconds(2)) {
      REXLOG_INFO("moving: {} render-only mismatches in the last 2 s", total);
      last = t;
      lines = 0;
      total = 0;
    }
    if (lines++ >= 12) {
      return;
    }
    const Matrix4& w = now.world;
    if (!tick) {
      REXLOG_INFO("moving: render-only mismatch, key not in the tick: tick {} camera {:08X} world "
                  "{:08X} id {} handler {:08X} occurrence {}, world at {:.3f} {:.3f} {:.3f}",
                  ticks_, key.camera, key.world, key.signature, key.handler, key.occurrence,
                  w[12], w[13], w[14]);
      return;
    }
    const Matrix4& tw = tick->world;
    REXLOG_INFO("moving: render-only mismatch: tick {} camera {:08X} world {:08X} id {} handler "
                "{:08X} occurrence {}, world at {:.3f} {:.3f} {:.3f}, logic pass's {:.3f} {:.3f} "
                "{:.3f}; camera {}",
                ticks_, key.camera, key.world, key.signature, key.handler, key.occurrence, w[12],
                w[13], w[14], tw[12], tw[13], tw[14],
                tick->view_projection == now.view_projection ? "same" : "differs");
  }
  CameraIds camera_ids_;

  struct Output {
    ProductKey key;
    Matrix4 written;
  };
  std::unordered_map<uint32_t, Output> tick_outputs_;  // by address, from the logic pass
  std::unordered_map<uint32_t, Output> pass_outputs_;  // from this render-only pass
  // The race overlays' products on the logic pass, by value (see Note).
  std::unordered_map<uint64_t, ProductKey> overlay_keys_this_tick_, overlay_keys_;

  std::vector<Saved<Matrix4>> saved_;
  std::vector<Saved<Vec3>> saved_vec3_;
  std::vector<Saved<Matrix4>> object_saved_;
  std::vector<Saved<Matrix4>> billboard_saved_;
  std::vector<Saved<Vec3>> billboard_saved_vec3_;

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
    int mismatched_products = 0;
    int copies = 0;
    std::atomic<int> replayed_products = 0;  // render thread
    std::chrono::steady_clock::time_point last_log{};

    void Reset() {
      draws = look_ats = cameras = objects = objects_unmatched = objects_not_transforms =
          camera_cuts = object_cuts = products_published = mismatched_products = copies = 0;
      replayed_products = 0;
    }
  } counts_;
};

DrawState& GetDrawState() {
  static DrawState state;
  return state;
}

// Render thread: blends the logic pass's camera products for each frame.
// Diagnostics on the render thread (--log_moving_products): each frame's
// replayed products, checked once the frame is done for moving objects
// drawn out of step: not blended or unpaired; paired with something that
// moved unlike everything near it in the tick (another wheel, say); or drawn
// somewhere its motion doesn't explain since the last frame.
class MovingProductsLog {
 public:
  enum class Outcome { kBlended, kSpinning, kWorldNotBlended, kUnpaired };

  void OnProduct(uint32_t payload, const ReplayedProduct& product, const Matrix4& world_shown,
                 Outcome outcome) {
    Record r;
    r.identity = product.identity;
    r.payload = payload;
    r.handler = product.handler;
    r.now = Translation(product.now.world);
    r.shown = Translation(world_shown);
    r.outcome = outcome;
    r.context_now = product.now.context;
    if (product.before) {
      r.context_before = product.before->context;
      r.before = Translation(product.before->world);
      std::optional<ScaledTransform> a = ScaledTransformFromWorldMatrix(product.before->world);
      std::optional<ScaledTransform> b = ScaledTransformFromWorldMatrix(product.now.world);
      r.angle = a && b ? AngleBetween(a->transform.rotation, b->transform.rotation) : -1;
    }
    records_.push_back(r);
  }

  // The frame drawn since the last call is complete; `tick` and `amount`
  // are the next frame's.
  void OnFrameStart(uint64_t tick, float amount) {
    Check();
    std::unordered_map<uint64_t, Shown> shown;
    for (const Record& r : records_) {
      shown[r.identity] = {r.shown, tick_, amount_};
    }
    last_shown_ = std::move(shown);
    records_.clear();
    tick_ = tick;
    amount_ = amount;
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ >= std::chrono::seconds(2)) {
      if (frames_ > 0) {
        REXLOG_INFO("moving: last 2 s, {} frames, {:.1f} moving products a frame; {} not "
                    "blended, {} unpaired near moving, {} unlike their neighbours, {} jumps",
                    frames_, double(moving_) / frames_, not_blended_, unpaired_, unlike_, jumps_);
      }
      last_log_ = now;
      lines_ = 0;
      frames_ = moving_ = not_blended_ = unpaired_ = unlike_ = jumps_ = 0;
    }
    frames_++;
  }

 private:
  struct Record {
    uint64_t identity = 0;
    uint32_t payload = 0, handler = 0;
    Vec3 now{}, shown{};
    std::optional<Vec3> before;
    float angle = -1;
    Outcome outcome = Outcome::kBlended;
    std::array<uint32_t, 9> context_now{}, context_before{};
    float Moved() const { return before ? Distance(*before, now) : 0.0f; }
  };
  struct Shown {
    Vec3 position;
    uint64_t tick;
    float amount;
  };

  static Vec3 Translation(const Matrix4& m) { return {m[12], m[13], m[14]}; }
  static const char* Name(Outcome o) {
    switch (o) {
      case Outcome::kBlended: return "blended";
      case Outcome::kSpinning: return "spinning";
      case Outcome::kWorldNotBlended: return "world not blended";
      case Outcome::kUnpaired: return "unpaired";
    }
    return "?";
  }
  bool Line() { return lines_++ < 40; }
  std::string Describe(const Record& r) const {
    std::string before = r.before ? fmt::format("{:.3f} {:.3f} {:.3f}", (*r.before)[0],
                                                (*r.before)[1], (*r.before)[2])
                                  : std::string("-");
    auto context = [](const std::array<uint32_t, 9>& c) {
      return fmt::format("lr {:08X} r24-r31 {:08X} {:08X} {:08X} {:08X} {:08X} {:08X} {:08X} {:08X}",
                         c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);
    };
    return fmt::format("id {:016X} payload {:08X} handler {:08X} {}: before {} now {:.3f} {:.3f} "
                       "{:.3f} shown {:.3f} {:.3f} {:.3f}, moved {:.3f} turned {:.3f}; now {}; "
                       "before {}",
                       r.identity, r.payload, r.handler, Name(r.outcome), before, r.now[0],
                       r.now[1], r.now[2], r.shown[0], r.shown[1], r.shown[2], r.Moved(), r.angle,
                       context(r.context_now), r.before ? context(r.context_before) : "-");
  }

  void Check() {
    if (records_.empty()) {
      return;
    }
    std::vector<const Record*> moving;
    for (const Record& r : records_) {
      if (r.before && r.Moved() > 0.01f) {
        moving.push_back(&r);
      }
    }
    moving_ += int(moving.size());
    auto near_moving = [&](const Vec3& p) {
      for (const Record* m : moving) {
        if (Distance(m->now, p) < 4.0f) {
          return true;
        }
      }
      return false;
    };
    for (const Record& r : records_) {
      // Drawn at the tick (ahead of the blended objects around it).
      if (r.outcome == Outcome::kWorldNotBlended && r.Moved() > 0.01f) {
        not_blended_++;
        if (Line()) {
          REXLOG_INFO("moving: not blended, tick {} amount {:.2f}: {}", tick_, amount_,
                      Describe(r));
        }
      } else if (r.outcome == Outcome::kUnpaired && near_moving(r.now)) {
        unpaired_++;
        if (Line()) {
          REXLOG_INFO("moving: unpaired near moving, tick {} amount {:.2f}: {}", tick_, amount_,
                      Describe(r));
        }
      }
      // Drawn somewhere its own motion doesn't explain since the last frame.
      auto last = last_shown_.find(r.identity);
      if (last != last_shown_.end() && r.before) {
        float frames_apart = float(tick_ - last->second.tick) + amount_ - last->second.amount;
        float expected = r.Moved() * std::abs(frames_apart);
        float moved = Distance(last->second.position, r.shown);
        if (moved > 2 * expected + 0.2f) {
          jumps_++;
          if (Line()) {
            REXLOG_INFO("moving: jump of {:.3f} (expected {:.3f}), tick {} amount {:.2f}, last "
                        "frame tick {} amount {:.2f} at {:.3f} {:.3f} {:.3f}: {}",
                        moved, expected, tick_, amount_, last->second.tick, last->second.amount,
                        last->second.position[0], last->second.position[1],
                        last->second.position[2], Describe(r));
          }
        }
      }
    }
    // Paired with something else: moved unlike every moving product near it.
    for (const Record* r : moving) {
      Vec3 d = {r->now[0] - (*r->before)[0], r->now[1] - (*r->before)[1],
                r->now[2] - (*r->before)[2]};
      int neighbours = 0, agreeing = 0;
      for (const Record* o : moving) {
        if (o == r || Distance(o->now, r->now) >= 4.0f) {
          continue;
        }
        neighbours++;
        Vec3 e = {o->now[0] - (*o->before)[0], o->now[1] - (*o->before)[1],
                  o->now[2] - (*o->before)[2]};
        if (Distance(d, e) < 0.3f) {
          agreeing++;
        }
      }
      if (neighbours >= 2 && agreeing == 0) {
        unlike_++;
        if (Line()) {
          REXLOG_INFO("moving: unlike its {} neighbours, tick {} amount {:.2f}: {}", neighbours,
                      tick_, amount_, Describe(*r));
        }
      }
    }
  }

  std::vector<Record> records_;
  std::unordered_map<uint64_t, Shown> last_shown_;
  uint64_t tick_ = 0;
  float amount_ = 0;
  int frames_ = 0, moving_ = 0, not_blended_ = 0, unpaired_ = 0, unlike_ = 0, jumps_ = 0;
  int lines_ = 0;
  std::chrono::steady_clock::time_point last_log_ = std::chrono::steady_clock::now();
};

MovingProductsLog& GetMovingProductsLog() {
  static MovingProductsLog log;
  return log;
}

class ReplayCameras {
 public:
  void OnFrameStart() {
    LogStats();
    if (overlay_frames_left_ > 0) {
      overlay_frames_left_--;
      if (REXCVAR_GET(log_overlay_products)) {
        REXLOG_INFO("overlay: --- frame ---");
      }
    }
    last_identity_ = 0;
    std::lock_guard lock(g_replay_mutex);
    if (frame_ != g_replay_frame) {
      if (frame_) {
        g_retired_replay_frames.push_back(std::move(frame_));
      }
      frame_ = g_replay_frame;
    }
    if (REXCVAR_GET(log_moving_products) && frame_) {
      GetMovingProductsLog().OnFrameStart(frame_->tick, frame_->amount);
    }
  }

  uint64_t tick() const { return frame_ ? frame_->tick : 0; }

  // Diagnostics (--log_overlay_products): every race overlay product of 12
  // frames in a row, every 3 s.
  void LogOverlay(uint32_t payload, const ReplayedProduct& product, const Matrix4& result) {
    auto now = std::chrono::steady_clock::now();
    if (overlay_frames_left_ == 0 && now - overlay_last_ >= std::chrono::seconds(3)) {
      overlay_frames_left_ = 12;
      overlay_last_ = now;
    }
    if (overlay_frames_left_ == 0) {
      return;
    }
    const Matrix4& w = product.written;
    REXLOG_INFO("overlay: tick {} amount {:.2f} payload {:08X} paired {} written row3 ({:.4g} {:.4g} "
                "{:.4g} {:.4g}) now ({:.4g} {:.4g} {:.4g} {:.4g}) result ({:.4g} {:.4g} {:.4g} {:.4g})",
                frame_->tick, frame_->amount, payload, product.before.has_value(), w[12], w[13],
                w[14], w[15], product.now.world[12], product.now.world[13], product.now.world[14],
                product.now.world[15], result[12], result[13], result[14], result[15]);
  }
  int overlay_frames_left_ = 0;
  std::chrono::steady_clock::time_point overlay_last_{};
  // Diagnostics: uploads from addresses that held a race overlay product,
  // left as recorded.
  void LogOverlayMiss(uint32_t payload, const Matrix4& m) {
    if (!overlay_payloads_.count(payload)) {
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - miss_last_ >= std::chrono::seconds(1)) {
      miss_last_ = now;
      miss_lines_ = 0;
    }
    if (miss_lines_++ >= 30) {
      return;
    }
    REXLOG_INFO("overlay miss: tick {} amount {:.2f} payload {:08X} {}: row3 ({:.5g} {:.5g} "
                "{:.5g} {:.5g})",
                tick(), amount(), payload, Status(payload, m), m[12], m[13], m[14], m[15]);
  }
  std::unordered_set<uint32_t> overlay_payloads_;
  std::chrono::steady_clock::time_point miss_last_{};
  int miss_lines_ = 0;
  // The last camera product replayed: identifies the object being drawn.
  uint64_t last_identity() const { return last_identity_; }
  // What went into the last substituted product.
  const ProductBlendParts& last_parts() const { return last_parts_; }
  ProductBlendParts last_parts_;
  float amount() const { return frame_ ? frame_->amount : 1.0f; }

  // Render thread diagnostics: why an upload was or wasn't substituted.
  std::string Status(uint32_t payload, const Matrix4& m) const {
    if (!frame_) {
      return "no frame";
    }
    auto it = frame_->products.find(payload);
    if (it == frame_->products.end()) {
      return "not published";
    }
    if (it->second.written != m) {
      return "published, value differs";
    }
    return it->second.before ? "substituted" : "published, unpaired";
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
      // A draw that isn't a known product (world geometry's matrix often
      // isn't): the pixel constants after it belong to it, not to the last
      // product, so they mustn't be paired by that product's identity (that
      // blended one piece of world geometry's lighting with another's).
      if (REXCVAR_GET(strict_pixel_pairing)) {
        last_identity_ = 0;
      }
      return std::nullopt;
    }
    last_identity_ = it->second.identity;
    if (it->second.handler == kRaceOverlaysHandler) {
      overlay_payloads_.insert(payload);
    }
    BlendStats before = stats_.blend;
    Matrix4 result = BlendProduct(it->second, frame_->amount, stats_.blend, &last_parts_);
    if (REXCVAR_GET(log_moving_products)) {
      using Outcome = MovingProductsLog::Outcome;
      Outcome outcome = stats_.blend.unpaired != before.unpaired       ? Outcome::kUnpaired
                        : stats_.blend.spinning != before.spinning     ? Outcome::kSpinning
                        : stats_.blend.world_not_blended != before.world_not_blended
                            ? Outcome::kWorldNotBlended
                            : Outcome::kBlended;
      GetMovingProductsLog().OnProduct(payload, it->second, last_parts_.world, outcome);
    }
    if (REXCVAR_GET(log_overlay_products) && it->second.handler == kRaceOverlaysHandler) {
      LogOverlay(payload, it->second, result);
    }
    return result;
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
    {
      std::lock_guard lock(g_camera_rejects_mutex);
      if (!g_camera_rejects.empty()) {
        REXLOG_INFO("replay: cameras not blended:{}", g_camera_rejects);
        g_camera_rejects.clear();
      }
    }
    REXLOG_INFO(
        "replay: {} frames; per frame {} uploads, {} blended ({} spinning, {} world, {} camera not "
        "blended), "
        "{} unpaired, not replaced: {} unchanged since last frame, {} changed",
        stats_.frames, stats_.uploads / f, stats_.blend.blended / f, stats_.blend.spinning / f,
        stats_.blend.world_not_blended / f, stats_.blend.camera_not_blended / f,
        stats_.blend.unpaired / f, stats_.not_substituted_same / f,
        stats_.not_substituted_changed / f);
    stats_ = {};
    last_log_ = now;
  }

  std::shared_ptr<const ReplayFrame> frame_;
  uint64_t last_identity_ = 0;
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

// Render thread: per-object pixel shader constants (sub_82595490's 63
// registers: lighting, including the headlights' position and direction)
// are recorded with the tick's values. Each upload is paired with the same
// one as of the previous tick, identified by the object's camera product
// replayed just before it (which is paired reliably), and given the frame's
// linear blend of the two. Per-object light switches that jump keep the
// tick's value; any other jump (a cut, a different object) leaves the upload
// as recorded.
class PixelConstantBlender {
 public:
  void OnFrameStart(uint64_t tick) {
    frame_++;
    recorded_ = &now_;  // entries may go below
    if (tick != tick_) {
      tick_ = tick;
      // A new tick: what was current becomes the previous tick's. Objects
      // not seen for two ticks are gone (identities aren't reused).
      for (auto it = entries_.begin(); it != entries_.end();) {
        Entry& entry = it->second;
        if (entry.current_tick + 2 < tick) {
          it = entries_.erase(it);
          continue;
        }
        if (entry.current_tick + 1 == tick) {
          entry.previous.swap(entry.current);  // current is rewritten before it's read
          entry.previous_tick = entry.current_tick;
        }
        ++it;
      }
      for (auto it = occurrences_.begin(); it != occurrences_.end();) {
        it = it->second.tick + 2 < tick ? occurrences_.erase(it) : std::next(it);
      }
    }
  }

  // Blends the upload in place and returns true; recorded() then holds the
  // upload as recorded, to put back after.
  bool Apply(uint8_t* base, uint32_t payload, uint32_t packed, float amount, uint64_t object) {
    outcome_ = Outcome::kNoObject;
    switch_jumps_ = 0;
    if (object == 0) {
      return false;  // no object to pair by
    }
    uint32_t start = (packed >> 8) & 0xFF, count = packed & 0xFF;
    // How many uploads of this object, start and count came earlier in the
    // frame (counts from earlier frames are stale, not cleared).
    Occurrence& seen = occurrences_[{object, start, count, 0}];
    if (seen.frame != frame_) {
      seen = {frame_, tick_, 0};
    }
    Key key{object, start, count, seen.count++};
    Entry& entry = entries_[key];
    // The light slots (cars' headlights among them) are recorded on a tick's
    // logic pass before the lights move, so they hold the previous tick's
    // lights there, and the tick's own on the frames after. Blending from
    // the last value recorded in the previous tick to this frame's gives the
    // previous tick's lights at the tick (amount about 0) and a blend after.
    bool from_latest = REXCVAR_GET(pixel_blend_from_latest);
    bool first = entry.current_tick != tick_;  // the tick's own value
    // The upload, loaded straight into the entry when it becomes its current
    // value (the entry's node stays put until OnFrameStart).
    std::vector<float>& now = first || from_latest ? entry.current : now_;
    now.resize(count * 4);
    guest::CopyFloats(base, payload, now.data(), count * 4);
    entry.current_tick = tick_;
    recorded_ = &now;
    last_key_ = {object, key.occurrence};
    keep_blended_ = REXCVAR_GET(log_pixel_outcomes);
    if (keep_blended_) {
      last_blended_ = now;
    }
    if (entry.previous_tick + 1 != tick_ || entry.previous.size() != now.size()) {
      outcome_ = Outcome::kUnpaired;
      return false;
    }
    // Loaded into current, it's the same; otherwise compare with the tick's
    // first value.
    if (&now == &now_ && entry.current != now) {
      outcome_ = Outcome::kNotAsRecorded;  // differs from the tick's first value
      differing_.clear();
      for (uint32_t i = 0; i < now.size(); i++) {
        if (i < entry.current.size() && entry.current[i] != now[i]) {
          differing_.push_back({i, entry.current[i], now[i]});
        }
      }
      return false;
    }
    // The same as last tick, bit for bit: nothing to blend.
    if (std::memcmp(entry.previous.data(), now.data(), now.size() * sizeof(float)) == 0) {
      outcome_ = Outcome::kUnchanged;
      return false;
    }
    // Some components are per-object switches that legitimately jump
    // between ticks (c1.x, c3.xyz, c4.x of an upload: which lights reach the
    // object, and how strongly); they take the tick's value and the rest blends. A
    // jump anywhere else means this isn't the same object (or something cut),
    // so the whole upload is left as recorded rather than mix two objects.
    auto is_switch = [&](uint32_t i) {
      // Register * 4 + component, from the start of the upload (the same
      // layout is uploaded at c0 and at c63).
      return i == 1 * 4 + 0 || i == 3 * 4 + 0 || i == 3 * 4 + 1 || i == 3 * 4 + 2 || i == 4 * 4 + 0;
    };
    auto jumped = [&](uint32_t i) {
      float a = entry.previous[i], b = now[i];
      return a != b && (!std::isfinite(a) || !std::isfinite(b) ||
                        !(std::abs(b - a) <= 0.3f * std::max(std::abs(a), std::abs(b)) + 0.2f));
    };
    // The 8 light slots: slot k is registers 8+k, 16+k, 24+k, 32+k, 47+k and
    // 55+k (position, direction, colour and the like; see light_setup_log).
    // A slot whose light changed between ticks (a streetlight in place of a
    // car's headlight) takes the tick's light, as the game switches it, and
    // the rest of the upload still blends; rejecting the whole upload left
    // the car's lighting at the tick for a tick (a flicker).
    // A slot's light position (8+k) is a world position, where the relative
    // tolerance allows hundreds of metres: a slot taking the next street lamp
    // along was blended as one light sliding 30 m across the car in a tick
    // (a flash passing under lamps). It's another light if it moved more than
    // kSlotLightMove; cars' headlights move at most about 3 m a tick.
    constexpr float kSlotLightMove = 4.0f;  // metres
    std::array<bool, 8> slot_switched{};
    if (count == 63 && REXCVAR_GET(switch_light_slots)) {
      for (uint32_t k = 0; k < 8; k++) {
        uint32_t p = (8 + k) * 4;
        Vec3 before{entry.previous[p], entry.previous[p + 1], entry.previous[p + 2]};
        Vec3 after{now[p], now[p + 1], now[p + 2]};
        slot_switched[k] = !(Distance(before, after) <= kSlotLightMove);
        for (uint32_t r : {8u, 16u, 24u, 32u, 47u, 55u}) {
          for (uint32_t c = 0; c < 4; c++) {
            slot_switched[k] = slot_switched[k] || jumped((r + k) * 4 + c);
          }
        }
      }
    }
    auto in_switched_slot = [&](uint32_t i) {
      if (count != 63) {
        return false;
      }
      uint32_t r = i / 4;
      for (uint32_t base : {8u, 16u, 24u, 32u, 47u, 55u}) {
        if (r >= base && r < base + 8 && slot_switched[r - base]) {
          return true;
        }
      }
      return false;
    };
    // c0-c3 are the view matrix and c4 the position of the camera the
    // lighting is worked out against (copied from the camera pass by the
    // render orchestrator): they blend together as a camera, or on a cut
    // (the camera moved more than kCutDistance) take the tick's together.
    // Judged by component, a view matrix entry that moved more than the
    // tolerance kept the tick's value while the rest blended, so the car was
    // lit against an inconsistent camera for a tick (a flash).
    bool camera = count == 63 && REXCVAR_GET(blend_lighting_camera);
    bool camera_cut = false;
    if (camera) {
      Vec3 eye_before{entry.previous[16], entry.previous[17], entry.previous[18]};
      Vec3 eye_now{now[16], now[17], now[18]};
      camera_cut = !IsFinite(eye_before) || !IsFinite(eye_now) ||
                   !(Distance(eye_before, eye_now) <= kCutDistance);
      for (uint32_t i = 0; i < 20; i++) {
        camera_cut = camera_cut || !std::isfinite(entry.previous[i]) || !std::isfinite(now[i]);
      }
    }
    // --blend_only_light_slots: only the light slots' positions (c8-c15) and
    // directions (c24-c31) blend, which move the cars' headlights' light with
    // the cars, and the rest is left as recorded. Added while opponents' cars
    // flashed passing under lamps, before the cause (slots taking the next
    // lamp, see slot_switched) was found; blending all of it is fine since.
    bool only_lights = count == 63 && REXCVAR_GET(blend_only_light_slots);
    auto blends = [&](uint32_t i) {
      uint32_t r = i / 4;
      return !only_lights || (r >= 8 && r < 16) || (r >= 24 && r < 32);
    };
    std::vector<uint8_t>& jumps = jumps_;
    jumps.assign(now.size(), 0);
    bool changed = false;
    for (uint32_t i = 0; i < now.size(); i++) {
      float a = entry.previous[i], b = now[i];
      if (a == b) {
        continue;
      }
      if (!blends(i)) {
        jumps[i] = true;  // as recorded
        continue;
      }
      if (camera && i < 20) {
        if (camera_cut) {
          jumps[i] = true;  // the tick's camera
        } else {
          changed = true;
        }
        continue;
      }
      if (in_switched_slot(i)) {
        jumps[i] = true;  // the tick's light
        switch_jumps_++;
        continue;
      }
      if (jumped(i)) {
        if (!is_switch(i)) {
          if (REXCVAR_GET(log_blend_state)) {
            LogRejection(start * 4 + i, a, b);
          }
          outcome_ = Outcome::kRejected;
          rejected_ = i;
          rejected_values_ = {a, b};
          return false;
        }
        jumps[i] = true;
        switch_jumps_++;
        continue;
      }
      changed = true;
    }
    if (!changed) {
      outcome_ = Outcome::kUnchanged;
      return false;
    }
    outcome_ = Outcome::kBlended;
    for (uint32_t i = 0; i < now.size(); i++) {
      float a = entry.previous[i], b = now[i];
      if (a != b && !jumps[i]) {
        float blended = a + (b - a) * amount;
        guest::StoreFloat(base, payload + 4 * i, blended);
        if (keep_blended_) {
          last_blended_[i] = blended;
        }
      }
    }
    return true;
  }

  // The last upload as recorded (what Apply blended over).
  const std::vector<float>& recorded() const { return *recorded_; }

  // What the last Apply did, for --log_pixel_outcomes.
  enum class Outcome { kNoObject, kUnpaired, kNotAsRecorded, kRejected, kUnchanged, kBlended };
  Outcome outcome() const { return outcome_; }
  int switch_jumps() const { return switch_jumps_; }
  // The rejected component (from the upload's start) and its two ticks' values.
  uint32_t rejected() const { return rejected_; }
  std::pair<float, float> rejected_values() const { return rejected_values_; }
  // For kNotAsRecorded: the components (from the upload's start) that differ
  // from the tick's first value, with both values.
  struct Difference {
    uint32_t i;
    float first, now;
  };
  const std::vector<Difference>& differing() const { return differing_; }
  // The last upload's object and occurrence, and its values as recorded.
  std::pair<uint64_t, int> last_key() const { return last_key_; }
  const std::vector<float>& last_values() const { return *recorded_; }
  // ...and as uploaded (the blend, or as recorded); kept only with
  // --log_pixel_outcomes.
  const std::vector<float>& last_blended() const { return last_blended_; }

 private:
  std::vector<Difference> differing_;
  std::pair<uint64_t, int> last_key_{};
  // The upload being blended, as recorded, and which components keep the
  // tick's value: kept between uploads so as not to allocate each time.
  std::vector<float> now_;
  const std::vector<float>* recorded_ = &now_;  // now_ or an entry's current
  std::vector<uint8_t> jumps_;
  std::vector<float> last_blended_;
  bool keep_blended_ = false;
  static void LogRejection(uint32_t i, float a, float b);

  Outcome outcome_ = Outcome::kNoObject;
  int switch_jumps_ = 0;
  uint32_t rejected_ = 0;
  std::pair<float, float> rejected_values_{};

  struct Key {
    uint64_t object;
    uint32_t start, count;
    int occurrence;
    bool operator==(const Key&) const = default;
  };
  struct KeyHash {
    size_t operator()(const Key& key) const {
      uint64_t h = key.object * 0x9E3779B97F4A7C15ull;
      h ^= (uint64_t(key.start) << 40 | uint64_t(key.count) << 32 | uint32_t(key.occurrence)) +
           0x632BE59BD9B4E019ull + (h << 6) + (h >> 2);
      return size_t(h ^ (h >> 29));
    }
  };
  struct Entry {
    std::vector<float> current, previous;
    uint64_t current_tick = 0, previous_tick = 0;
  };
  struct Occurrence {
    uint64_t frame = 0, tick = 0;  // when last counted
    int count = 0;
  };
  std::unordered_map<Key, Entry, KeyHash> entries_;
  // Uploads of an object, start and count so far in the frame (occurrence 0
  // in the key).
  std::unordered_map<Key, Occurrence, KeyHash> occurrences_;
  uint64_t tick_ = 0;
  uint64_t frame_ = 0;
};

void PixelConstantBlender::LogRejection(uint32_t i, float a, float b) {
  static std::map<uint32_t, int> rejections;  // by register * 4 + component
  static auto last = std::chrono::steady_clock::now();
  rejections[i]++;
  auto now = std::chrono::steady_clock::now();
  if (now - last < std::chrono::seconds(2)) {
    return;
  }
  last = now;
  std::string line;
  for (const auto& [index, n] : rejections) {
    line += fmt::format(" c{}.{}:{}", index / 4, "xyzw"[index % 4], n);
  }
  REXLOG_INFO("pixel constants rejected (register.component:count):{}; e.g. c{}.{} {} -> {}", line,
              i / 4, "xyzw"[i % 4], a, b);
  rejections.clear();
}

PixelConstantBlender& GetPixelConstantBlender() {
  static PixelConstantBlender blender;
  return blender;
}

// Diagnostics (--log_pixel_outcomes): what the pixel constant blender did with
// each lighting upload, by the object it was paired by: moving (its world
// matrix blends, so a car, a wheel, a car body) or still (road, buildings),
// or no product before it. An upload that isn't blended on a moving object
// shows the tick's lighting on a blended object.
class PixelOutcomesLog {
 public:
  void OnUpload(uint32_t start, bool has_object, bool moving, uint64_t tick, float amount,
                const Vec3& world_now, const Vec3& world) {
    using Outcome = PixelConstantBlender::Outcome;
    const PixelConstantBlender& blender = GetPixelConstantBlender();
    Outcome outcome = blender.outcome();
    Trace(start, moving, tick, amount, world_now, world);
    if (moving) {
      FrameChanges(start, tick, amount);
    }
    const char* kind = !has_object ? "no product" : moving ? "moving" : "still";
    counts_[{kind, start, int(outcome)}]++;
    if (outcome == Outcome::kBlended && blender.switch_jumps() > 0) {
      switch_jumps_[{kind, start}]++;
    }
    if (outcome == Outcome::kNotAsRecorded && moving) {
      std::string components;
      for (const auto& d : blender.differing()) {
        differing_[d.i]++;
        if (components.size() < 160) {
          components += fmt::format(" c{}.{} {:.4g}->{:.4g}", start + d.i / 4, "xyzw"[d.i % 4],
                                    d.first, d.now);
        }
      }
      if (not_as_recorded_examples_.size() < 6) {
        not_as_recorded_examples_.push_back(
            fmt::format("{} differ:{}", blender.differing().size(), components));
      }
    }
    if (outcome == Outcome::kRejected && moving) {
      auto [a, b] = blender.rejected_values();
      rejected_[blender.rejected()]++;
      if (rejected_examples_.size() < 400) {
        rejected_examples_ += fmt::format(" c{}.{} {:.4g}->{:.4g}", start + blender.rejected() / 4,
                                          "xyzw"[blender.rejected() % 4], a, b);
      }
      const ReplayedProduct* product = GetReplayCameras().last_parts().product;
      if (product && rejected_lines_++ < 12) {
        const Matrix4& w = product->now.world;
        const auto& c = product->now.context;
        std::string before = "-";
        if (product->before) {
          const Matrix4& v = product->before->world;
          const auto& d = product->before->context;
          before = fmt::format("{:.3f} {:.3f} {:.3f} (lr {:08X} r31 {:08X})", v[12], v[13], v[14],
                               d[0], d[8]);
        }
        REXLOG_INFO("pixel outcomes: rejected c{}.{} {:.4g}->{:.4g}, object {:016X}: now {:.3f} "
                    "{:.3f} {:.3f} (lr {:08X} r31 {:08X}), before {}",
                    start + blender.rejected() / 4, "xyzw"[blender.rejected() % 4], a, b,
                    product->identity, w[12], w[13], w[14], c[0], c[8], before);
      }
    }
  }

  void OnFrameStart() {
    frames_++;
    frame_counter_++;
    auto now = std::chrono::steady_clock::now();
    if (now - last_ < std::chrono::seconds(2)) {
      return;
    }
    last_ = now;
    static const char* kNames[] = {"no object", "unpaired", "not as recorded",
                                   "rejected", "unchanged", "blended"};
    std::map<std::pair<std::string, uint32_t>, std::string> lines;
    for (const auto& [key, n] : counts_) {
      const auto& [kind, start, outcome] = key;
      lines[{kind, start}] += fmt::format(" {} {:.1f}", kNames[outcome], double(n) / frames_);
    }
    for (const auto& [key, line] : lines) {
      auto jumps = switch_jumps_.find(key);
      REXLOG_INFO("pixel outcomes per frame, {} objects, c{}:{}; blended with switches jumping {:.1f}",
                  key.first, key.second, line,
                  jumps == switch_jumps_.end() ? 0.0 : double(jumps->second) / frames_);
    }
    if (!rejected_.empty()) {
      std::string line;
      for (const auto& [i, n] : rejected_) {
        line += fmt::format(" +{}.{}:{}", i / 4, "xyzw"[i % 4], n);
      }
      REXLOG_INFO("pixel outcomes: moving objects rejected by (register from start.component:count):{}; e.g.{}",
                  line, rejected_examples_);
    }
    if (!differing_.empty()) {
      std::string line;
      for (const auto& [i, n] : differing_) {
        line += fmt::format(" +{}.{}:{}", i / 4, "xyzw"[i % 4], n);
      }
      REXLOG_INFO("pixel outcomes: moving objects not as recorded, differing (register from "
                  "start.component:count):{}", line);
      for (const std::string& example : not_as_recorded_examples_) {
        REXLOG_INFO("pixel outcomes: not as recorded, e.g. {}", example);
      }
    }
    for (const auto* changes : {&frame_jumps_, &frame_flips_}) {
      if (changes->empty()) {
        continue;
      }
      std::string line;
      for (const auto& [i, n] : *changes) {
        line += fmt::format(" c{}.{}:{}", i / 4, "xyzw"[i % 4], n);
      }
      REXLOG_INFO("pixel outcomes: moving objects' lighting, frame-to-frame {} (register from "
                  "start.component:count):{}",
                  changes == &frame_jumps_ ? "jumps" : "flips", line);
    }
    frame_jumps_.clear();
    frame_flips_.clear();
    change_lines_ = 0;
    history_.clear();
    differing_.clear();
    not_as_recorded_examples_.clear();
    rejected_lines_ = 0;
    counts_.clear();
    switch_jumps_.clear();
    rejected_.clear();
    rejected_examples_.clear();
    frames_ = 0;
  }

 private:
  std::map<std::tuple<std::string, uint32_t, int>, int> counts_;
  std::map<std::pair<std::string, uint32_t>, int> switch_jumps_;
  std::map<uint32_t, int> rejected_;
  std::string rejected_examples_;
  // A moving object's lighting as uploaded, frame to frame: registers that
  // jump between consecutive frames, and flips (back to two frames ago).
  void FrameChanges(uint32_t start, uint64_t tick, float amount) {
    const PixelConstantBlender& blender = GetPixelConstantBlender();
    const std::vector<float>& now = blender.last_blended();
    if (now.size() != 63 * 4) {
      return;
    }
    auto [object, occurrence] = blender.last_key();
    History& h = history_[{object, occurrence, start}];
    bool consecutive = h.frame + 1 == frame_counter_;
    if (consecutive && h.previous.size() == now.size()) {
      bool has_older = h.older.size() == now.size() && h.older_frame + 2 == frame_counter_;
      for (uint32_t i = 0; i < now.size(); i++) {
        float a = h.previous[i], b = now[i];
        if (a == b || std::abs(b - a) <= 0.3f * std::max(std::abs(a), std::abs(b)) + 0.2f) {
          continue;
        }
        uint32_t r = i / 4;
        bool slot = (r >= 8 && r < 40) || (r >= 47 && r < 63);
        float o = has_older ? h.older[i] : 0;
        bool flip = has_older && std::abs(b - o) <= 0.1f * std::max(std::abs(b), std::abs(o)) + 0.05f;
        (flip ? frame_flips_ : frame_jumps_)[i]++;
        // Followed: jumps in the lighting's camera (c0-c4).
        if (follow_frames_ == 0 && i < 20 &&
            std::chrono::steady_clock::now() - follow_last_ >= std::chrono::seconds(3)) {
          follow_last_ = std::chrono::steady_clock::now();
          follow_key_ = {object, occurrence, start};
          follow_frames_ = 10;
          auto line = [&](const std::vector<float>& v) {
            return fmt::format("c3 {:.4g} {:.4g} {:.4g} {:.4g} | c4 {:.4g} {:.4g} {:.4g} {:.4g} | "
                               "c8 {:.4g} {:.4g} {:.4g} | c9 {:.4g} {:.4g} {:.4g} | c47 {:.4g} "
                               "{:.4g} {:.4g} {:.4g} | c48 {:.4g} {:.4g} {:.4g} {:.4g}",
                               v[12], v[13], v[14], v[15], v[16], v[17], v[18], v[19], v[32],
                               v[33], v[34], v[36], v[37], v[38], v[188], v[189], v[190], v[191],
                               v[192], v[193], v[194], v[195]);
          };
          REXLOG_INFO("pixel follow: object {:016X} c{} jumped at c{}.{}; two frames ago "
                      "uploaded {}",
                      object, start, start + r, "xyzw"[i % 4],
                      has_older ? line(h.older) : std::string("-"));
          REXLOG_INFO("pixel follow: last frame (tick {} amount {:.2f}) uploaded {}", h.tick,
                      h.amount, line(h.previous));
        }
        if (change_lines_++ < 16) {
          REXLOG_INFO("pixel outcomes: frame {} c{}.{} ({}), object {:016X}: {} -> {:.4g} -> "
                      "{:.4g}, tick {} amount {:.2f} (last frame tick {} amount {:.2f}), {}",
                      flip ? "flip" : "jump", start + r, "xyzw"[i % 4],
                      slot ? "light slot" : "other", object,
                      has_older ? fmt::format("{:.4g}", o) : std::string("-"), a, b, tick, amount,
                      h.tick, h.amount, kOutcomeNames[int(blender.outcome())]);
        }
      }
    }
    if (follow_frames_ > 0 && follow_key_ == std::make_tuple(object, occurrence, start) &&
        follow_frame_ != frame_counter_) {
      follow_frame_ = frame_counter_;
      follow_frames_--;
      const std::vector<float>& rec = blender.last_values();
      auto line = [&](const std::vector<float>& v) {
        return fmt::format("c3 {:.4g} {:.4g} {:.4g} {:.4g} | c4 {:.4g} {:.4g} {:.4g} {:.4g} | c8 "
                           "{:.4g} {:.4g} {:.4g} | c9 {:.4g} {:.4g} {:.4g} | c47 {:.4g} {:.4g} "
                           "{:.4g} {:.4g} | c48 {:.4g} {:.4g} {:.4g} {:.4g}",
                           v[12], v[13], v[14], v[15], v[16], v[17], v[18], v[19], v[32], v[33],
                           v[34], v[36], v[37], v[38], v[188], v[189], v[190], v[191], v[192],
                           v[193], v[194], v[195]);
      };
      REXLOG_INFO("pixel follow: tick {} amount {:.2f} {}: recorded {}", tick, amount,
                  kOutcomeNames[int(blender.outcome())], line(rec));
      REXLOG_INFO("pixel follow:                       uploaded {}", line(now));
    }
    h.older = std::move(h.previous);
    h.older_frame = h.frame;
    h.previous = now;
    h.frame = frame_counter_;
    h.tick = tick;
    h.amount = amount;
  }

  static constexpr const char* kOutcomeNames[] = {"no object", "unpaired", "not as recorded",
                                                  "rejected",  "unchanged", "blended"};
  struct History {
    std::vector<float> previous, older;
    uint64_t frame = 0, older_frame = 0, tick = 0;
    float amount = 0;
  };
  std::map<std::tuple<uint64_t, int, uint32_t>, History> history_;
  uint64_t frame_counter_ = 0;
  std::map<uint32_t, int> frame_jumps_, frame_flips_;
  int change_lines_ = 0;
  std::tuple<uint64_t, int, uint32_t> follow_key_{};
  int follow_frames_ = 0;
  uint64_t follow_frame_ = 0;
  std::chrono::steady_clock::time_point follow_last_{};

  // Every 5 s, one moving object's light slots (c8-c11 positions, c24-c27
  // directions) followed upload by upload for 4 ticks, from the first one
  // that isn't as recorded or is blended.
  void Trace(uint32_t start, bool moving, uint64_t tick, float amount, const Vec3& world_now,
             const Vec3& world) {
    const PixelConstantBlender& blender = GetPixelConstantBlender();
    auto now = std::chrono::steady_clock::now();
    if (trace_ticks_left_ == 0) {
      if (moving && start == 0 &&
          (blender.outcome() == PixelConstantBlender::Outcome::kNotAsRecorded ||
           blender.outcome() == PixelConstantBlender::Outcome::kBlended) &&
          now - last_trace_ >= std::chrono::seconds(5)) {
        last_trace_ = now;
        trace_key_ = blender.last_key();
        trace_tick_ = tick;
        trace_ticks_left_ = 4;
        REXLOG_INFO("pixel trace: following object {:016X} occurrence {}", trace_key_.first,
                    trace_key_.second);
      } else {
        return;
      }
    }
    if (blender.last_key() != trace_key_ || start != 0) {
      return;
    }
    if (tick != trace_tick_) {
      trace_tick_ = tick;
      if (--trace_ticks_left_ == 0) {
        return;
      }
    }
    static const char* kNames[] = {"no object", "unpaired", "not as recorded",
                                   "rejected", "unchanged", "blended"};
    const std::vector<float>& v = blender.last_values();
    const std::vector<float>& u = blender.last_blended();
    auto uploaded = [&](uint32_t r) {
      return r * 4 + 2 < u.size()
                 ? fmt::format("{:.3f} {:.3f} {:.3f}", u[r * 4], u[r * 4 + 1], u[r * 4 + 2])
                 : std::string("-");
    };
    auto reg = [&](uint32_t r) {
      return r * 4 + 2 < v.size()
                 ? fmt::format("{:.3f} {:.3f} {:.3f}", v[r * 4], v[r * 4 + 1], v[r * 4 + 2])
                 : std::string("-");
    };
    REXLOG_INFO("pixel trace: tick {} amount {:.2f} {} | world now {:.3f} {:.3f} {:.3f} blend "
                "{:.3f} {:.3f} {:.3f} | c8 {} | c9 {} | c11 {} | c24 {} | c25 {} | c27 {} | "
                "uploaded c9 {} | c25 {}",
                tick, amount, kNames[int(blender.outcome())], world_now[0], world_now[1],
                world_now[2], world[0], world[1], world[2], reg(8), reg(9), reg(11), reg(24),
                reg(25), reg(27), uploaded(9), uploaded(25));
  }

  std::chrono::steady_clock::time_point last_trace_{};
  std::pair<uint64_t, int> trace_key_{};
  uint64_t trace_tick_ = 0;
  int trace_ticks_left_ = 0;
  std::map<uint32_t, int> differing_;
  std::vector<std::string> not_as_recorded_examples_;
  int rejected_lines_ = 0;
  int frames_ = 0;
  std::chrono::steady_clock::time_point last_ = std::chrono::steady_clock::now();
};

PixelOutcomesLog& GetPixelOutcomesLog() {
  static PixelOutcomesLog log;
  return log;
}

// Diagnostics on the render thread (--log_stepping_constants): constant
// uploads that weren't substituted, grouped by the code that recorded them,
// register and order in the frame, with how often their values change. One
// that changes about 30 times a second while frames run at 150 is stuck to
// the game tick.
class SteppingConstantsLog {
 public:
  void OnFrameStart() {
    if (!REXCVAR_GET(log_stepping_constants)) {
      return;
    }
    frames_++;
    occurrences_.clear();
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ < std::chrono::seconds(2)) {
      return;
    }
    float seconds = std::chrono::duration<float>(now - last_log_).count();
    last_log_ = now;
    int logged = 0;
    for (const auto& [key, entry] : entries_) {
      float rate = entry.changes / seconds;
      if (entry.frames * 2 > frames_ && rate > 15 && rate < 45 && logged < 60) {
        logged++;
        REXLOG_INFO("stepping constants: {} c{} x{} #{} by {:08X}: {:.0f} changes/s, e.g. {}",
                    key.stage ? "pixel" : "vertex", key.start, key.count, key.occurrence,
                    key.owner, rate, entry.sample);
      }
    }
    // Uploads blended on some frames and not others, or recorded on only
    // some frames (a logic pass only, say): candidates for a sprite drawn
    // in two places.
    int mixed = 0;
    for (const auto& [key, entry] : entries_) {
      bool sometimes_blended = entry.substituted > 0 && entry.frames > 0 &&
                               std::min(entry.substituted, entry.frames) * 20 > entry.present;
      bool sometimes_present = entry.present * 10 > frames_ && entry.present * 10 < frames_ * 9;
      if ((sometimes_blended || sometimes_present) && mixed < 60) {
        mixed++;
        REXLOG_INFO("mixed constants: {} c{} x{} #{} by {:08X}: in {} of {} frames, blended in {}, "
                    "{} changes/s unblended, e.g. {}",
                    key.stage ? "pixel" : "vertex", key.start, key.count, key.occurrence,
                    key.owner, entry.present, frames_, entry.substituted, entry.changes / seconds,
                    entry.sample);
      }
    }
    REXLOG_INFO("stepping constants: {} logged of {} upload groups, {} frames", logged,
                entries_.size(), frames_);
    entries_.clear();
    frames_ = 0;
  }

  void OnUpload(uint8_t* base, uint32_t payload, uint32_t packed, bool substituted) {
    if (!REXCVAR_GET(log_stepping_constants)) {
      return;
    }
    Key key{packed >> 24, (packed >> 8) & 0xFF, packed & 0xFF, SlotOwner(payload - 16), 0};
    key.occurrence = occurrences_[key]++;
    Entry& counted = entries_[key];
    counted.present++;
    if (substituted) {
      counted.substituted++;
      return;
    }
    uint32_t words = std::min<uint32_t>(key.count * 4, 16);
    std::array<float, 16> value{};
    for (uint32_t i = 0; i < words; i++) {
      value[i] = guest::LoadFloat(base, payload + 4 * i);
    }
    Entry& entry = entries_[key];
    entry.frames++;
    if (entry.seen && entry.last != value) {
      entry.changes++;
    }
    entry.seen = true;
    entry.last = value;
    if (entry.sample.empty()) {
      for (uint32_t i = 0; i < std::min<uint32_t>(words, 8); i++) {
        entry.sample += fmt::format("{:.4g} ", value[i]);
      }
    }
  }

 private:
  struct Key {
    uint32_t stage, start, count, owner;
    int occurrence;
    auto operator<=>(const Key&) const = default;
  };
  struct Entry {
    int frames = 0;  // not substituted
    int changes = 0;
    int present = 0;
    int substituted = 0;
    bool seen = false;
    std::array<float, 16> last{};
    std::string sample;
  };
  std::map<Key, Entry> entries_;
  std::map<Key, int> occurrences_;
  int frames_ = 0;
  std::chrono::steady_clock::time_point last_log_{};
};

SteppingConstantsLog& GetSteppingConstantsLog() {
  static SteppingConstantsLog log;
  return log;
}

// Diagnostics (--log_overlay_uploads): every constant upload the race
// overlays recorded, as replayed, for 12 frames in a row every 3 s.
class OverlayUploadsLog {
 public:
  void OnFrameStart() {
    if (!REXCVAR_GET(log_overlay_uploads)) {
      return;
    }
    frame_++;
    if (frames_left_ > 0) {
      frames_left_--;
      index_ = 0;
      return;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - last_ >= std::chrono::seconds(3)) {
      last_ = now;
      frames_left_ = 12;
      index_ = 0;
    }
  }

  void OnUpload(uint8_t* base, uint32_t payload, uint32_t packed, bool substituted,
                const std::optional<Matrix4>& original) {
    if (!REXCVAR_GET(log_overlay_uploads) || frames_left_ == 0) {
      return;
    }
    std::optional<OverlaySlot> slot = OverlaySlotOf(payload - 16);
    if (!slot) {
      slot = OverlaySlotOf(payload);
    }
    if (!slot) {
      return;
    }
    uint32_t stage = packed >> 24, start = (packed >> 8) & 0xFF, count = packed & 0xFF;
    std::string values;
    for (uint32_t i = 0; i < std::min<uint32_t>(count * 4, 20); i++) {
      values += fmt::format("{}{:.5g}", i % 4 == 0 ? (i ? " | " : "") : " ",
                            guest::LoadFloat(base, payload + 4 * i));
    }
    std::string was;
    if (original) {
      was = fmt::format(" (was row3 {:.5g} {:.5g} {:.5g} {:.5g})", (*original)[12],
                        (*original)[13], (*original)[14], (*original)[15]);
    }
    REXLOG_INFO("overlay upload: frame {} tick {} amount {:.2f} #{} recorded on {} pass {} "
                "{} c{} x{} {}: {}{}",
                frame_, GetReplayCameras().tick(), GetReplayCameras().amount(), index_++,
                slot->render_only ? "render-only" : "logic", slot->pass,
                stage == 0 ? "vs" : "ps", start, count, substituted ? "substituted" : "as recorded",
                values, was);
  }

 private:
  uint64_t frame_ = 0;
  int frames_left_ = 0;
  int index_ = 0;
  std::chrono::steady_clock::time_point last_{};
};

OverlayUploadsLog& GetOverlayUploadsLog() {
  static OverlayUploadsLog log;
  return log;
}

}  // namespace

void BlendState_LogOverlayAllocations(uint8_t* base) {
  static auto last = std::chrono::steady_clock::now();
  auto now = std::chrono::steady_clock::now();
  if (!REXCVAR_GET(log_overlay_uploads) || frame_pacing::GameMessages_IsRenderOnlyPass()) {
    return;
  }
  if (now - last >= std::chrono::seconds(3)) {
    last = now;
    for (const OverlayAllocation& a : g_overlay_allocations) {
      std::string words;
      for (uint32_t i = 0; i < std::min<uint32_t>(a.count * 4, 24); i++) {
        uint32_t w = guest::LoadU32(base, a.address + 4 * i);
        float f;
        std::memcpy(&f, &w, 4);
        words += (i % 4 == 0 && i) ? " | " : " ";
        words += (std::isfinite(f) && std::abs(f) > 1e-4f && std::abs(f) < 1e6f)
                     ? fmt::format("{:.5g}", f)
                     : fmt::format("{:08X}", w);
      }
      REXLOG_INFO("overlay alloc: {:08X} x{} by {:08X}:{}", a.address, a.count, a.caller, words);
    }
  }
  g_overlay_allocations.clear();
}

void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass) {
  GetDrawState().BeforeDraw(base, !render_only_pass);
}

void BlendState_OnLogicPassStart() {
  GetDrawState().OnLogicPassStart();
}

void BlendState_SetHandler(uint32_t handler) {
  GetDrawState().SetHandler(handler);
}

void BlendState_AfterDraw(uint8_t* base) {
  GetDrawState().AfterDraw(base);
}

// Diagnostics (--log_object_extras): the per-object uploads (vertex c0 x9,
// sub_825953D0) whose c0-c3 were substituted: how often their c4-c7 (the
// world matrix) and c8 change from frame to frame, with examples, every 2 s.
class ObjectExtrasLog {
 public:
  void OnFrameStart() {
    frames_++;
    auto now = std::chrono::steady_clock::now();
    if (now - last_log_ < std::chrono::seconds(2)) {
      return;
    }
    float seconds = std::chrono::duration<float>(now - last_log_).count();
    last_log_ = now;
    REXLOG_INFO("object extras: {} frames, {} substituted x9 uploads; c4-c7 changed in {} "
                "({:.1f}/s per upload address), c8 changed in {}",
                frames_, uploads_, world_changes_,
                world_changes_ / std::max<float>(seconds * std::max<size_t>(last_.size(), 1), 1),
                c8_changes_);
    for (const std::string& example : examples_) {
      REXLOG_INFO("object extras: {}", example);
    }
    REXLOG_INFO("object extras: replaced c4-c7 in {}, c8 in {}; camera blended but c8 not its "
                "position in {} (e.g. {:.3g} m away)",
                world_replaced_, eye_replaced_, eye_missed_, eye_miss_example_);
    frames_ = uploads_ = world_changes_ = c8_changes_ = 0;
    world_replaced_ = eye_replaced_ = eye_missed_ = 0;
    examples_.clear();
  }

  // eye_distance: from c8 to the camera's position, or -1 if the camera
  // wasn't blended.
  void OnReplaced(bool world, float eye_distance) {
    world_replaced_ += world;
    if (eye_distance >= 0.05f) {
      eye_missed_++;
      eye_miss_example_ = eye_distance;
    } else if (eye_distance >= 0) {
      eye_replaced_++;
    }
  }

  void OnUpload(uint8_t* base, uint32_t payload) {
    uploads_++;
    std::array<float, 20> extras;
    for (uint32_t i = 0; i < 20; i++) {
      extras[i] = guest::LoadFloat(base, payload + 64 + 4 * i);
    }
    auto it = last_.find(payload);
    if (it != last_.end()) {
      bool world = !std::equal(extras.begin(), extras.begin() + 16, it->second.begin());
      bool c8 = !std::equal(extras.begin() + 16, extras.end(), it->second.begin() + 16);
      world_changes_ += world;
      c8_changes_ += c8;
      if ((world || c8) && examples_.size() < 6) {
        examples_.push_back(fmt::format(
            "{:08X} c7 {:.4g} {:.4g} {:.4g} {:.4g} -> {:.4g} {:.4g} {:.4g} {:.4g}; "
            "c8 {:.4g} {:.4g} {:.4g} {:.4g} -> {:.4g} {:.4g} {:.4g} {:.4g}",
            payload, it->second[12], it->second[13], it->second[14], it->second[15], extras[12],
            extras[13], extras[14], extras[15], it->second[16], it->second[17], it->second[18],
            it->second[19], extras[16], extras[17], extras[18], extras[19]));
      }
    }
    last_[payload] = extras;
    if (last_.size() > 100000) {
      last_.clear();
    }
  }

 private:
  std::chrono::steady_clock::time_point last_log_ = std::chrono::steady_clock::now();
  int frames_ = 0, uploads_ = 0, world_changes_ = 0, c8_changes_ = 0;
  int world_replaced_ = 0, eye_replaced_ = 0, eye_missed_ = 0;
  float eye_miss_example_ = 0;
  std::vector<std::string> examples_;
  std::unordered_map<uint32_t, std::array<float, 20>> last_;
};

ObjectExtrasLog& GetObjectExtrasLog() {
  static ObjectExtrasLog log;
  return log;
}

void BlendState_BeforeRecordObject(uint8_t* base, uint32_t view_projection, uint32_t world) {
  GetDrawState().BeforeRecordObject(base, view_projection, world);
}

void BlendState_AfterRecordObject(uint8_t* base) {
  GetDrawState().AfterRecordObject(base);
}

void BlendState_OnReplayFrameStart() {
  if (REXCVAR_GET(log_object_extras)) {
    GetObjectExtrasLog().OnFrameStart();
  }
  GetReplayedConstantsLog().OnFrameStart();
  GetSteppingConstantsLog().OnFrameStart();
  GetReplayCameras().OnFrameStart();
  GetOverlayUploadsLog().OnFrameStart();
  GetPixelConstantBlender().OnFrameStart(GetReplayCameras().tick());
  if (REXCVAR_GET(log_pixel_outcomes)) {
    GetPixelOutcomesLog().OnFrameStart();
  }
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
  // The per-object recorder's upload (c0 x9, sub_825953D0) also holds the
  // world matrix (c4-c7) and the camera's position (c8), which the paint's
  // reflections use: give them the same blend as c0-c3, when they hold
  // what the product was made from.
  std::array<float, 20> original_extras;
  bool extras = original && count >= 9 && REXCVAR_GET(blend_object_extras);
  if (extras) {
    const ProductBlendParts& parts = GetReplayCameras().last_parts();
    for (uint32_t i = 0; i < 20; i++) {
      original_extras[i] = guest::LoadFloat(base, payload + 64 + 4 * i);
    }
    bool world = LoadMatrix(base, payload + 64) == parts.world_now;
    if (world) {
      StoreMatrix(base, payload + 64, parts.world);
    }
    float eye_distance = -1;
    if (parts.eye_now && parts.eye) {
      float* c8 = &original_extras[16];
      eye_distance = Distance({c8[0], c8[1], c8[2]}, *parts.eye_now);
      if (eye_distance < 0.05f) {
        for (uint32_t i = 0; i < 3; i++) {
          guest::StoreFloat(base, payload + 128 + 4 * i, (*parts.eye)[i]);
        }
      }
    }
    if (REXCVAR_GET(log_object_extras)) {
      GetObjectExtrasLog().OnReplaced(world, eye_distance);
    }
  }
  bool pixel_blended = false;
  if (REXCVAR_GET(interpolate_draw_state) && REXCVAR_GET(blend_pixel_constants) && stage == 1 &&
      ((packed >> 16) & 0xFF) == 1) {
    pixel_blended =
        GetPixelConstantBlender().Apply(base, payload, packed, GetReplayCameras().amount(),
                                        GetReplayCameras().last_identity());
    if (REXCVAR_GET(log_pixel_outcomes)) {
      const ProductBlendParts& parts = GetReplayCameras().last_parts();
      GetPixelOutcomesLog().OnUpload(
          (packed >> 8) & 0xFF, GetReplayCameras().last_identity() != 0,
          parts.world != parts.world_now, GetReplayCameras().tick(), GetReplayCameras().amount(),
          {parts.world_now[12], parts.world_now[13], parts.world_now[14]},
          {parts.world[12], parts.world[13], parts.world[14]});
    }
  }
  GetSteppingConstantsLog().OnUpload(base, payload, packed,
                                     original.has_value() || pixel_blended);
  if (REXCVAR_GET(log_object_extras) && stage == 0 && count == 9 && original) {
    GetObjectExtrasLog().OnUpload(base, payload);
  }
  if (stage == 0 && count >= 4) {
    GetReplayCameras().CountUpload(base, payload, original.has_value());
    if (!original && REXCVAR_GET(log_overlay_products)) {
      GetReplayCameras().LogOverlayMiss(payload, LoadMatrix(base, payload));
    }
    // Diagnostics (--log_replayed_constants): main-camera matrices uploaded
    // to c0, with their status.
    if (REXCVAR_GET(log_replayed_constants)) {
      uint32_t start = (packed >> 8) & 0xFF;
      Matrix4 m = original ? *original : LoadMatrix(base, payload);
      static auto last = std::chrono::steady_clock::now();
      static int logged = 0;
      auto now = std::chrono::steady_clock::now();
      if (now - last > std::chrono::seconds(1)) {
        last = now;
        logged = 0;
      }
      std::string status = GetReplayCameras().Status(payload, m);
      if (start == 0 && status != "substituted" && logged < 400) {
        logged++;
        REXLOG_INFO("c0 upload t={} from {:08X} (slot by {:08X}, header by {:08X}) x{} {}: {:.4g} {:.4g} {:.4g} {:.4g} ... {:.4g} {:.4g} {:.4g} {:.4g}",
                    std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count() % 100000000,
                    payload, SlotOwner(payload), SlotOwner(payload - 16), count, status, m[0], m[1], m[2], m[3],
                    m[12], m[13], m[14], m[15]);
      }
    }
  }
  GetReplayedConstantsLog().OnUpload(base, payload, packed, original.has_value());
  std::vector<uint32_t> zeroed;
  if (stage == 0) {
    if (REXCVAR_GET(log_upload_owners)) {
      uint32_t owner = SlotOwner(payload - 16);
      static std::map<uint32_t, int> owners;
      static int frames = 0;
      static uint64_t last_tick = 0;
      static auto last = std::chrono::steady_clock::now();
      owners[owner]++;
      if (GetReplayCameras().tick() != last_tick) {
        last_tick = GetReplayCameras().tick();
        frames++;
      }
      auto now = std::chrono::steady_clock::now();
      if (now - last >= std::chrono::seconds(5)) {
        last = now;
        std::string line;
        for (const auto& [who, n] : owners) {
          line += fmt::format(" {:08X}:{}", who, n / std::max(frames, 1));
        }
        REXLOG_INFO("upload owners (caller:uploads per tick):{}", line);
        owners.clear();
        frames = 0;
      }
    }
  }
  // Vertex or pixel constants, by the code that allocated the header.
  if (const std::string& list = REXCVAR_GET(debug_zero_owners); !list.empty()) {
    uint32_t owner = SlotOwner(payload - 16);
    if (owner != 0 && list.find(fmt::format("{:08X}", owner)) != std::string::npos) {
      for (uint32_t i = 0; i < count * 4; i++) {
        zeroed.push_back(guest::LoadU32(base, payload + 4 * i));
        guest::StoreU32(base, payload + 4 * i, 0);
      }
    }
  }
  GetOverlayUploadsLog().OnUpload(base, payload, packed,
                                  original.has_value() || pixel_blended, original);
  upload();
  for (uint32_t i = 0; i < zeroed.size(); i++) {
    guest::StoreU32(base, payload + 4 * i, zeroed[i]);
  }
  if (original) {
    StoreMatrix(base, payload, *original);
  }
  if (extras) {
    for (uint32_t i = 0; i < 20; i++) {
      guest::StoreFloat(base, payload + 64 + 4 * i, original_extras[i]);
    }
  }
  if (pixel_blended) {
    const std::vector<float>& recorded = GetPixelConstantBlender().recorded();
    for (uint32_t i = 0; i < recorded.size(); i++) {
      guest::StoreFloat(base, payload + 4 * i, recorded[i]);
    }
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
REXCVAR_DEFINE_BOOL(log_recording_by_pass, false, "ITC",
                    "Log, for each code allocating command slots, its allocations per logic "
                    "pass and per render-only pass (in races), every 2 s");
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
  if (REXCVAR_GET(log_overlay_uploads) && ctx.r3.u32 != 0 &&
      std::this_thread::get_id() == interpolation::GetDrawState().main_thread()) {
    uint32_t handler = interpolation::GetDrawState().handler() & ~1u;
    std::optional<interpolation::OverlaySlot> slot;
    if (handler == interpolation::kRaceOverlaysHandler) {
      if (!frame_pacing::GameMessages_IsRenderOnlyPass() &&
          interpolation::g_overlay_allocations.size() < 4096) {
        interpolation::g_overlay_allocations.push_back({ctx.r3.u32, count, caller});
      }
      slot = interpolation::OverlaySlot{frame_pacing::GameMessages_PassNumber(),
                                        frame_pacing::GameMessages_IsRenderOnlyPass()};
    }
    std::lock_guard lock(interpolation::g_overlay_slots_mutex);
    if (interpolation::g_overlay_slots.size() > 400000) {
      interpolation::g_overlay_slots.clear();
    }
    for (uint32_t i = 0; i < count; i++) {
      interpolation::g_overlay_slots[ctx.r3.u32 + 16 * i] = slot;
    }
  }
  if (REXCVAR_GET(log_slot_callers) && ctx.r3.u32 != 0) {
    std::lock_guard lock(interpolation::g_slot_owners_mutex);
    if (interpolation::g_slot_owners.size() > 200000) {
      interpolation::g_slot_owners.clear();
    }
    for (uint32_t i = 0; i < count; i++) {
      interpolation::g_slot_owners[ctx.r3.u32 + 16 * i] = caller;
    }
  }
  if (REXCVAR_GET(log_recording_by_pass) && frame_pacing::GameMessages_InRace()) {
    static std::map<uint32_t, std::pair<uint64_t, uint64_t>> by_caller;  // logic, render-only
    static uint64_t logic_start = 0, render_start = 0;
    static auto last = std::chrono::steady_clock::now();
    auto& entry = by_caller[caller];
    (frame_pacing::GameMessages_IsRenderOnlyPass() ? entry.second : entry.first)++;
    auto now = std::chrono::steady_clock::now();
    if (now - last >= std::chrono::seconds(2)) {
      last = now;
      uint64_t logic, render;
      frame_pacing::GameMessages_PassCounts(logic, render);
      double logic_passes = double(std::max<uint64_t>(logic - logic_start, 1));
      double render_passes = double(std::max<uint64_t>(render - render_start, 1));
      for (const auto& [who, n] : by_caller) {
        double per_logic = n.first / logic_passes, per_render = n.second / render_passes;
        if (per_logic > per_render + 0.5) {
          REXLOG_INFO("recording by pass: {:08X}: {:.1f} per logic pass, {:.1f} per render-only pass",
                      who, per_logic, per_render);
        }
      }
      REXLOG_INFO("recording by pass: {} logic, {} render-only passes", logic - logic_start,
                  render - render_start);
      by_caller.clear();
      logic_start = logic;
      render_start = render;
    }
  }
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

// Matrix_Copy: r3 = destination, r4 = source; copies a 4x4 matrix.
extern "C" REX_FUNC(sub_820A0AC0) {
  REX_FUNC_PROLOGUE();
  uint32_t dst = ctx.r3.u32, src = ctx.r4.u32;
  __imp__sub_820A0AC0(ctx, base);
  interpolation::GetDrawState().OnCopy(base, dst, src);
}

// Billboards_BuildVertices: r3 = billboard set (its +0 is the camera pass it
// faces), r4 = command list, r5 = the sprites. Writes each light's quad,
// turned to face that camera.
extern "C" REX_FUNC(sub_8209C6E0) {
  REX_FUNC_PROLOGUE();
  uint32_t pass = guest::LoadU32(base, ctx.r3.u32);
  interpolation::GetDrawState().BeforeBillboards(base, pass);
  __imp__sub_8209C6E0(ctx, base);
  interpolation::GetDrawState().AfterBillboards(base);
}

// Command 7, set shader constants (render thread): r3 = payload, r4 =
// register count, r5 = packed stage/type/start/count. Uploads the frame's
// blend in place of the tick's camera products and lighting.
namespace {
std::atomic<int64_t> g_replay_total_ns = 0, g_replay_upload_ns = 0;
}  // namespace

void interpolation::BlendState_TakeReplayTimes(int64_t& blend_ns, int64_t& upload_ns) {
  int64_t total = g_replay_total_ns.exchange(0), upload = g_replay_upload_ns.exchange(0);
  blend_ns = total - upload;
  upload_ns = upload;
}

extern "C" REX_FUNC(sub_8258FD48) {
  REX_FUNC_PROLOGUE();
  uint32_t payload = ctx.r3.u32, packed = ctx.r5.u32;
  debug::ReplayStats_OnCommand(base, 7, payload, ctx.r4.u32, packed);
  native::Renderer_OnCommand(base, 7, payload, ctx.r4.u32, packed);
  if (!REXCVAR_GET(log_frame_times)) {
    interpolation::BlendState_ReplayConstants(base, payload, packed,
                                              [&] { __imp__sub_8258FD48(ctx, base); });
    return;
  }
  using Clock = std::chrono::steady_clock;
  Clock::time_point start = Clock::now();
  interpolation::BlendState_ReplayConstants(base, payload, packed, [&] {
    Clock::time_point upload_start = Clock::now();
    __imp__sub_8258FD48(ctx, base);
    g_replay_upload_ns += (Clock::now() - upload_start).count();
  });
  g_replay_total_ns += (Clock::now() - start).count();
}

// The per-object recorder (~150 calls a pass in a race): r4 = camera
// view-projection, r5 = the object's world matrix.
extern "C" REX_FUNC(sub_825953D0) {
  REX_FUNC_PROLOGUE();
  interpolation::BlendState_BeforeRecordObject(base, ctx.r4.u32, ctx.r5.u32);
  interpolation::g_record_context = {uint32_t(ctx.lr), ctx.r24.u32, ctx.r25.u32, ctx.r26.u32,
                                     ctx.r27.u32,      ctx.r28.u32, ctx.r29.u32, ctx.r30.u32,
                                     ctx.r31.u32};
  __imp__sub_825953D0(ctx, base);
  interpolation::g_record_context = {};
  interpolation::BlendState_AfterRecordObject(base);
}
