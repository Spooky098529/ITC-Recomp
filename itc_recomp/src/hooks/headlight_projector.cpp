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
// Here, on render-only passes, the eye, target and up are moved with the
// projector's placement blended between its last two ticks (positions in a
// straight line, in double; turns along the shortest arc), and the game's own
// builder makes the projector from them. Logic passes build the tick's, as
// before. The other caller (sub_820F34C8, up to 5 square projectors in the
// lights code) is left alone.

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
                    "Build the player's headlight projector (sub_82597610 from 0x820C6A2C) "
                    "for the frame being drawn on render-only passes, from its placement "
                    "blended between ticks, instead of the tick's being blended as a camera");
REXCVAR_DEFINE_BOOL(log_headlight_projector, false, "ITC",
                    "--native_headlight_projector: log the projectors built (blended or not, "
                    "and why), how far the frame's moved from the tick's, and the tick's "
                    "projector read back as a camera, every 2 s");

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
void StoreVector(uint8_t* base, uint32_t address, const Vector4& vector) {
  for (int i = 0; i < 4; i++) {
    guest::StoreFloat(base, address + 4 * i, vector.v[i]);
  }
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

struct Log {
  int logic = 0, blended = 0, no_before = 0, cut = 0, degenerate = 0, unchanged = 0;
  double max_moved = 0, max_turned = 0;
  Matrix4 last_projector{};
  bool has_projector = false;
  std::chrono::steady_clock::time_point logged = std::chrono::steady_clock::now();

  void MaybeLog() {
    auto now = std::chrono::steady_clock::now();
    if (now - logged < std::chrono::seconds(2)) {
      return;
    }
    logged = now;
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
    REXLOG_INFO("headlight projector: last 2 s, {} built on logic passes, {} blended on "
                "render-only passes ({} without a previous tick, {} cut, {} degenerate, {} at the "
                "tick); frame from tick at most {:.3f} m, {:.4f} rad; tick's read as a camera: {}",
                logic, blended, no_before, cut, degenerate, unchanged, max_moved, max_turned,
                camera);
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
  size_t index = g_calls++;
  if (g_slots.size() <= index) {
    g_slots.resize(index + 1);
  }
  Slot& slot = g_slots[index];
  std::array<Vector4, 3> inputs = {LoadVector(base, ctx.r4.u32), LoadVector(base, ctx.r5.u32),
                                   LoadVector(base, ctx.r6.u32)};
  uint32_t out = ctx.r3.u32;

  if (!frame_pacing::GameMessages_IsRenderOnlyPass()) {
    if (slot.tick != logic_passes) {
      slot.has_before = slot.tick != 0 && slot.tick + 1 == logic_passes;
      slot.before = slot.now;
      slot.tick = logic_passes;
    }
    slot.now = inputs;
    __imp__sub_82597610(ctx, base);
    if (log) {
      g_log.logic++;
      g_log.last_projector = {};
      for (int i = 0; i < 16; i++) {
        g_log.last_projector[size_t(i)] = guest::LoadFloat(base, out + 4 * i);
      }
      g_log.has_projector = true;
      g_log.MaybeLog();
    }
    return;
  }

  // A render-only pass: the inputs are the tick's (slot.now).
  float amount = interpolation::BlendState_Amount();
  std::optional<Placement> a, b;
  bool blend = false;
  if (!slot.has_before) {
    g_log.no_before += log;
  } else if (!(amount < 1.0f)) {
    g_log.unchanged += log;
  } else {
    a = PlacementOf(slot.before[0], slot.before[1], slot.before[2]);
    b = PlacementOf(inputs[0], inputs[1], inputs[2]);
    if (!a || !b) {
      g_log.degenerate += log;
    } else if (std::sqrt(Dot(Sub(b->eye, a->eye), Sub(b->eye, a->eye))) > kCutDistance) {
      g_log.cut += log;
    } else {
      blend = true;
    }
  }
  if (!blend) {
    __imp__sub_82597610(ctx, base);
    if (log) {
      g_log.MaybeLog();
    }
    return;
  }

  // The frame's placement: the eye in a straight line (in double), the basis
  // turned along the shortest arc.
  Transform ra = RotationOf(*a), rb = RotationOf(*b);
  Transform rf = interpolation::Blend(ra, rb, amount);
  Matrix4 mf = interpolation::WorldMatrixFromTransform(rf);
  std::array<Vec3d, 3> basis_f;
  for (int row = 0; row < 3; row++) {
    basis_f[size_t(row)] = {mf[size_t(row * 4)], mf[size_t(row * 4 + 1)],
                            mf[size_t(row * 4 + 2)]};
  }
  Vec3d eye_f;
  for (int k = 0; k < 3; k++) {
    eye_f[size_t(k)] = a->eye[size_t(k)] + (b->eye[size_t(k)] - a->eye[size_t(k)]) * amount;
  }
  // Each tick vector, in the tick placement's frame, put in the frame's.
  auto move = [&](const Vector4& v, bool point) {
    Vec3d p = point ? Sub(v.xyz(), b->eye) : v.xyz();
    Vec3d local = {Dot(p, b->basis[0]), Dot(p, b->basis[1]), Dot(p, b->basis[2])};
    Vector4 moved = v;
    for (int k = 0; k < 3; k++) {
      double value = local[0] * basis_f[0][size_t(k)] + local[1] * basis_f[1][size_t(k)] +
                     local[2] * basis_f[2][size_t(k)];
      moved.v[size_t(k)] = float(point ? eye_f[size_t(k)] + value : value);
    }
    return moved;
  };
  // 16-byte aligned (the builder loads them with lvx), in a frame of ours
  // above the builder's.
  uint32_t r1 = ctx.r1.u32;
  uint32_t frame = (r1 - 80) & ~15u;
  guest::StoreU32(base, frame, r1);  // the back chain, as stwu leaves it
  uint32_t eye_at = frame + 16, target_at = frame + 32, up_at = frame + 48;
  StoreVector(base, eye_at, move(inputs[0], true));
  StoreVector(base, target_at, move(inputs[1], true));
  StoreVector(base, up_at, move(inputs[2], false));
  ctx.r1.u32 = frame;
  ctx.r3.u32 = out, ctx.r4.u32 = eye_at, ctx.r5.u32 = target_at, ctx.r6.u32 = up_at;
  __imp__sub_82597610(ctx, base);
  ctx.r1.u32 = r1;
  if (log) {
    g_log.blended++;
    g_log.max_moved = std::max(g_log.max_moved, std::sqrt(Dot(Sub(eye_f, b->eye),
                                                               Sub(eye_f, b->eye))));
    g_log.max_turned =
        std::max(g_log.max_turned, double(interpolation::AngleBetween(rf.rotation, rb.rotation)));
    g_log.MaybeLog();
  }
}
