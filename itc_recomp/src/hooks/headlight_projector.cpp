// The player's headlight projector built for the frame being drawn
// (--native_headlight_projector). See "The headlight projector" in
// docs/functions.md.
//
// sub_82597610(out, eye, target, up, fov, aspect, near, far) builds a
// projector: look-at(eye, target, up) * perspective(fov, aspect, near, far) *
// a fixed bias matrix (0x8265EED0). The render orchestrator (sub_820C5DE8)
// calls it once a view for the player's headlight (return address
// 0x820C6A2C), the eye and target worked out from the car's world matrix, and
// copies the result into its per-view array. Render-only passes see the tick's
// car, so the projector was the tick's; the replay then blended it between
// ticks as if it were a camera, and its projection, read back as a camera's,
// drifts: it was renumbered (unpaired) or taken for a cut (not blended), and
// the beam lagged the car or flickered.
//
// Here the game still builds and draws the tick's projector on every pass (a
// logic pass's drawing is the tick's record, which later frames are blended
// against), and the projector for the frame being drawn goes to the replay
// beside it (BlendState_SetFrameCamera), as the cockpit needles' frame pose
// does: the projector's placement (its eye, and its basis from the view
// direction and up) blended between its last two ticks by the frame's amount,
// positions in a straight line and turns along the shortest arc, in double;
// the tick's projector moved by the motion from the tick's placement to the
// frame's. The replay uses it as is, for the projector's copies and for
// products made with it (their world matrices still blended): never blended
// as a camera, so never refused by the cut check. That included the logic
// pass's own frame (amount 0, the previous tick), which the replay used to
// make by blending the projector as a camera. The other caller
// (sub_820F34C8, up to 5 square projectors in the lights code) is left alone.

#include "frame_pacing/game_messages.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "interpolation/transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <optional>
#include <vector>

REXCVAR_DEFINE_BOOL(native_headlight_projector, false, "ITC",
                    "Give the replay the player's headlight projector (sub_82597610 from "
                    "0x820C6A2C) for the frame being drawn, from its placement blended between "
                    "ticks, instead of the tick's being blended as a camera (refused by the cut "
                    "check as it drifts far from the origin: the beam a tick ahead)");
REXCVAR_DEFINE_BOOL(log_headlight_projector, false, "ITC",
                    "--native_headlight_projector: log the projectors given for the frame "
                    "(blended or not, and why), how far they moved from the tick's, the tick's "
                    "read back as a camera, and the products made with them, every 2 s");

namespace {

using interpolation::Matrix4;
using interpolation::Transform;
using Vec3d = std::array<double, 3>;

constexpr uint32_t kHeadlightReturn = 0x820C6A2C;
// blend_state.cpp's kCutDistance: moving further in a tick is a cut.
constexpr double kCutDistance = 8.0;

// The builder's vector arguments as stored (x, y, z and the fourth word).
struct Vector4 {
  std::array<float, 4> v;
  Vec3d xyz() const { return {v[0], v[1], v[2]}; }
};
Vector4 LoadVector(uint8_t* base, uint32_t address) {
  Vector4 out;
  for (int i = 0; i < 4; i++) {
    out.v[i] = guest::LoadFloat(base, address + 4 * i);
  }
  return out;
}

Vec3d Sub(const Vec3d& a, const Vec3d& b) { return {a[0] - b[0], a[1] - b[1], a[2] - b[2]}; }
double Dot(const Vec3d& a, const Vec3d& b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
Vec3d Cross(const Vec3d& a, const Vec3d& b) {
  return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}
std::optional<Vec3d> Normalized(const Vec3d& a) {
  double length = std::sqrt(Dot(a, a));
  if (!(length > 1e-9)) {
    return std::nullopt;
  }
  return Vec3d{a[0] / length, a[1] / length, a[2] / length};
}

// The projector's placement: its eye, and a right-handed basis from its view
// direction and up (rows right, up, forward). Any fixed choice of basis does,
// as only the motion from one tick's to the frame's is used.
struct Placement {
  Vec3d eye;
  std::array<Vec3d, 3> basis;
};
std::optional<Placement> PlacementOf(const Vector4& eye, const Vector4& target,
                                     const Vector4& up) {
  auto forward = Normalized(Sub(target.xyz(), eye.xyz()));  // the game's r5 - r4
  if (!forward) {
    return std::nullopt;
  }
  auto right = Normalized(Cross(up.xyz(), *forward));
  if (!right) {
    return std::nullopt;
  }
  return Placement{eye.xyz(), {*right, Cross(*forward, *right), *forward}};
}
Transform RotationOf(const Placement& p) {
  Matrix4 m{};
  for (int row = 0; row < 3; row++) {
    for (int k = 0; k < 3; k++) {
      m[row * 4 + k] = float(p.basis[size_t(row)][size_t(k)]);
    }
  }
  m[15] = 1;
  return interpolation::TransformFromWorldMatrix(m).value_or(Transform{});
}

// Each view's projector inputs at its last two logic passes.
struct Slot {
  uint64_t tick = 0;  // the logic pass count they were read at (0: never)
  bool has_before = false;
  std::array<Vector4, 3> before{}, now{};  // eye, target, up
};
std::vector<Slot> g_slots;
uint64_t g_pass = 0;   // the pass the slot count is for
size_t g_calls = 0;    // calls from the headlight's caller this pass

Matrix4 LoadMatrix(uint8_t* base, uint32_t address) {
  Matrix4 m;
  for (int i = 0; i < 16; i++) {
    m[size_t(i)] = guest::LoadFloat(base, address + 4 * i);
  }
  return m;
}

// The projector for the frame: the tick's, applied after moving a point from
// the frame placement's frame to the tick placement's (a rigid motion, row
// vectors), worked out in double.
Matrix4 MoveProjector(const Matrix4& tick, const Placement& from, const Placement& to) {
  // p -> to.eye + ((p - from.eye) * from^T) * to: rotation from^T * to, and
  // translation to.eye - from.eye * (from^T * to).
  double m[4][4] = {};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      double sum = 0;
      for (int k = 0; k < 3; k++) {
        sum += from.basis[size_t(k)][size_t(i)] * to.basis[size_t(k)][size_t(j)];
      }
      m[i][j] = sum;
    }
  }
  for (int j = 0; j < 3; j++) {
    double sum = 0;
    for (int i = 0; i < 3; i++) {
      sum += from.eye[size_t(i)] * m[i][j];
    }
    m[3][j] = to.eye[size_t(j)] - sum;
  }
  m[3][3] = 1;
  Matrix4 out;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      double sum = 0;
      for (int k = 0; k < 4; k++) {
        sum += m[i][k] * double(tick[size_t(k * 4 + j)]);
      }
      out[size_t(i * 4 + j)] = float(sum);
    }
  }
  return out;
}

struct Log {
  int logic = 0, render_only = 0, blended = 0, no_before = 0, cut = 0, degenerate = 0,
      at_tick = 0;
  double max_moved = 0, max_turned = 0;
  Matrix4 last_projector{};
  bool has_projector = false;
  std::chrono::steady_clock::time_point logged = std::chrono::steady_clock::now();

  void MaybeLog() {
    auto now = std::chrono::steady_clock::now();
    if (now - logged < std::chrono::seconds(2)) {
      return;
    }
    std::string camera = "none";
    if (has_projector) {
      if (auto split = interpolation::SplitViewProjection(last_projector)) {
        camera = fmt::format("x {:.4f} y {:.4f} (aspect {:.4f}) w {:.4f} depth {:.5g} {:.5g}",
                             split->x_scale, split->y_scale, split->x_scale / split->y_scale,
                             split->w_scale, split->depth_scale, split->depth_offset);
      } else {
        camera = "doesn't split";
      }
    }
    REXLOG_INFO("headlight projector: last 2 s, {} on logic passes and {} on render-only passes; "
                "{} blended, {} at the tick ({} without a previous tick, {} cut, {} degenerate, "
                "{} at amount 1); frame from tick at most {:.3f} m, {:.4f} rad; {} products made "
                "with it; tick's read as a camera: {}",
                logic, render_only, blended, no_before + cut + degenerate + at_tick, no_before, cut,
                degenerate, at_tick, max_moved, max_turned,
                interpolation::BlendState_TakeFrameCameraUses(), camera);
    *this = Log{};
    logged = now;
  }
};
Log g_log;

}  // namespace

// r3 = out, r4 = eye, r5 = target, r6 = up; f1-f4 = field of view, aspect,
// near, far.
extern "C" REX_FUNC(sub_82597610) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(native_headlight_projector) || ctx.lr != kHeadlightReturn) {
    __imp__sub_82597610(ctx, base);
    return;
  }
  bool log = REXCVAR_GET(log_headlight_projector);
  uint64_t logic_passes = 0, render_only_passes = 0;
  frame_pacing::GameMessages_PassCounts(logic_passes, render_only_passes);
  if (logic_passes + render_only_passes != g_pass) {
    g_pass = logic_passes + render_only_passes;
    g_calls = 0;
  }
  uint32_t slot_index = uint32_t(g_calls++);
  if (g_slots.size() <= slot_index) {
    g_slots.resize(slot_index + 1);
  }
  Slot& slot = g_slots[slot_index];
  // The tick's inputs (on a render-only pass, the same as its logic pass's).
  std::array<Vector4, 3> inputs = {LoadVector(base, ctx.r4.u32), LoadVector(base, ctx.r5.u32),
                                   LoadVector(base, ctx.r6.u32)};
  bool render_only = frame_pacing::GameMessages_IsRenderOnlyPass();
  if (!render_only && slot.tick != logic_passes) {
    slot.has_before = slot.tick != 0 && slot.tick + 1 == logic_passes;
    slot.before = slot.now;
    slot.tick = logic_passes;
    slot.now = inputs;
  }
  uint32_t out = ctx.r3.u32;
  __imp__sub_82597610(ctx, base);
  Matrix4 tick = LoadMatrix(base, out);

  // The frame's placement: the eye in a straight line (in double), the basis
  // turned along the shortest arc; the tick's where there's no blend.
  Matrix4 frame = tick;
  float amount = interpolation::BlendState_Amount();
  std::optional<Placement> a, b;
  if (slot.has_before && amount < 1.0f) {
    a = PlacementOf(slot.before[0], slot.before[1], slot.before[2]);
    b = PlacementOf(inputs[0], inputs[1], inputs[2]);
  }
  if (!slot.has_before) {
    g_log.no_before += log;
  } else if (!(amount < 1.0f)) {
    g_log.at_tick += log;
  } else if (!a || !b) {
    g_log.degenerate += log;
  } else if (std::sqrt(Dot(Sub(b->eye, a->eye), Sub(b->eye, a->eye))) > kCutDistance) {
    g_log.cut += log;
  } else {
    Transform rf = interpolation::Blend(RotationOf(*a), RotationOf(*b), amount);
    Matrix4 mf = interpolation::WorldMatrixFromTransform(rf);
    Placement f;
    for (int row = 0; row < 3; row++) {
      f.basis[size_t(row)] = {mf[size_t(row * 4)], mf[size_t(row * 4 + 1)],
                              mf[size_t(row * 4 + 2)]};
    }
    for (int k = 0; k < 3; k++) {
      f.eye[size_t(k)] = a->eye[size_t(k)] + (b->eye[size_t(k)] - a->eye[size_t(k)]) * amount;
    }
    frame = MoveProjector(tick, f, *b);
    if (log) {
      g_log.blended++;
      g_log.max_moved = std::max(g_log.max_moved, std::sqrt(Dot(Sub(f.eye, b->eye),
                                                                 Sub(f.eye, b->eye))));
      g_log.max_turned = std::max(
          g_log.max_turned, double(interpolation::AngleBetween(rf.rotation, RotationOf(*b).rotation)));
    }
  }
  interpolation::BlendState_SetFrameCamera(tick.data(), frame.data(), slot_index);
  if (log) {
    (render_only ? g_log.render_only : g_log.logic)++;
    if (!render_only) {
      g_log.last_projector = tick;
      g_log.has_projector = true;
    }
    g_log.MaybeLog();
  }
}
