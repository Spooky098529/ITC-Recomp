// Runs the frame blending on a cockpit dial's needle and checks every frame
// against the needle turning evenly between ticks: its angle on the dial, how
// far its spindle moves and how far it tilts off the dial. The needle's mesh
// origin is away from its spindle, as in the game's cockpits (~0.8 m), and
// the car carrying the dial and the camera can drive, turn and roll.
//
// The plan and blend steps are copies of blend_state.cpp's PlanProduct,
// BlendCamera and BlendProduct (which need the runtime and its cvars); keep
// them in step. The maths is the real transform.cpp. Build and run (from
// itc_recomp/):
//   clang++ -std=c++23 -O1 -I src tests/needle_blend.cpp \
//     src/interpolation/transform.cpp -o /tmp/needle_blend && /tmp/needle_blend
// `-v` prints every frame.

#include "interpolation/transform.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <optional>
#include <string>
#include <vector>

using namespace interpolation;

namespace {

constexpr float kPi = 3.14159265358979f;
// blend_state.cpp's constants.
constexpr float kCutDistance = 8.0f;
constexpr float kCutAngle = 0.5f;

Vec3 Add(Vec3 a, Vec3 b) { return {a[0] + b[0], a[1] + b[1], a[2] + b[2]}; }
Vec3 Sub(Vec3 a, Vec3 b) { return {a[0] - b[0], a[1] - b[1], a[2] - b[2]}; }
Vec3 Scale(Vec3 a, float s) { return {a[0] * s, a[1] * s, a[2] * s}; }
float Dot(Vec3 a, Vec3 b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
Vec3 Normalize(Vec3 a) { return Scale(a, 1 / std::sqrt(Dot(a, a))); }

// Row vector times matrix (p' = p * M).
Vec3 Apply(const Matrix4& m, Vec3 p) {
  return {p[0] * m[0] + p[1] * m[4] + p[2] * m[8] + m[12],
          p[0] * m[1] + p[1] * m[5] + p[2] * m[9] + m[13],
          p[0] * m[2] + p[1] * m[6] + p[2] * m[10] + m[14]};
}
Vec3 ApplyRotation(const Matrix4& m, Vec3 p) {
  return {p[0] * m[0] + p[1] * m[4] + p[2] * m[8], p[0] * m[1] + p[1] * m[5] + p[2] * m[9],
          p[0] * m[2] + p[1] * m[6] + p[2] * m[10]};
}

Quaternion AxisAngle(Vec3 axis, float angle) {
  axis = Normalize(axis);
  float s = std::sin(angle / 2);
  return {axis[0] * s, axis[1] * s, axis[2] * s, std::cos(angle / 2)};
}
Matrix4 Rigid(Quaternion q, Vec3 p) { return WorldMatrixFromTransform(Transform{q, p}); }

// The game's projection (as in interpolation_screen_error.cpp).
Matrix4 Projection() {
  float n = 0.1f, f = 5000, sy = 2.4f, sx = 1.35f;
  return Matrix4{sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, f / (f - n), 1, 0, 0, -n * f / (f - n), 0};
}

// --- Copies of blend_state.cpp ----------------------------------------------

bool IsCut(const Transform& a, const Transform& b) {
  return Distance(a.position, b.position) > kCutDistance ||
         AngleBetween(a.rotation, b.rotation) > kCutAngle;
}
bool IsSameCamera(const PerspectiveCamera& a, const PerspectiveCamera& b) {
  auto close = [](float x, float y, float tolerance) {
    return std::abs(x - y) <= tolerance * std::max(std::abs(x), std::abs(y));
  };
  return !IsCut(a.camera, b.camera) && close(a.x_scale, b.x_scale, 0.05f) &&
         close(a.y_scale, b.y_scale, 0.05f) && close(a.w_scale, b.w_scale, 0.01f) &&
         close(a.x_scale / a.y_scale, b.x_scale / b.y_scale, 0.01f) &&
         close(a.depth_scale, b.depth_scale, 0.01f) && close(a.depth_offset, b.depth_offset, 0.01f);
}
// kNormal and kScrew are the game's blends (--screw_blend off and on);
// kRelative is a proposal: the product blended relative to its camera
// (world * view, as a screw), then drawn with the blended camera.
// kRelativeUncut is that without kCutAngle: any turn blends, the short way.
enum class Mode { kNormal, kScrew, kRelative, kRelativeUncut };
struct ProductPlan {
  enum class World { kNone, kBlend, kSpinning } world = World::kNone;
  bool relative = false;  // wa, wb are world * view
  ScaledTransform wa, wb;
  bool screw = false;
  ScrewMotion screw_motion;
  bool camera = false;
  PerspectiveCamera ca, cb;
  ScrewMotion camera_screw;
};
ProductPlan PlanProduct(const Matrix4& world_a, const Matrix4& vp_a, const Matrix4& world_b,
                        const Matrix4& vp_b, Mode mode) {
  ProductPlan plan;
  bool screw_blend = mode != Mode::kNormal;
  if (vp_a != vp_b) {
    std::optional<PerspectiveCamera> ca = SplitViewProjection(vp_a);
    std::optional<PerspectiveCamera> cb = SplitViewProjection(vp_b);
    if (ca && cb && IsSameCamera(*ca, *cb)) {
      plan.camera = true;
      plan.ca = *ca;
      plan.cb = *cb;
      if (screw_blend) {
        plan.camera_screw = ScrewBetween(ca->camera, cb->camera);
      }
    }
  }
  Matrix4 in_a = world_a, in_b = world_b;
  if ((mode == Mode::kRelative || mode == Mode::kRelativeUncut) && plan.camera) {
    plan.relative = true;
    in_a = Multiply(world_a, ViewMatrixFromTransform(plan.ca.camera));
    in_b = Multiply(world_b, ViewMatrixFromTransform(plan.cb.camera));
  }
  std::optional<ScaledTransform> wa = ScaledTransformFromWorldMatrix(in_a);
  std::optional<ScaledTransform> wb = ScaledTransformFromWorldMatrix(in_b);
  if (wa && wb) {
    plan.wa = *wa;
    plan.wb = *wb;
    bool cut = mode == Mode::kRelativeUncut
                   ? Distance(wa->transform.position, wb->transform.position) > kCutDistance
                   : IsCut(wa->transform, wb->transform);
    if (!cut) {
      plan.world = ProductPlan::World::kBlend;
      if (screw_blend) {
        plan.screw = true;
        plan.screw_motion = ScrewBetween(wa->transform, wb->transform);
      }
    } else if (Distance(wa->transform.position, wb->transform.position) <= kCutDistance &&
               BlendSpinning(*wa, *wb, 0, kCutAngle)) {
      plan.world = ProductPlan::World::kSpinning;
    }
  }
  return plan;
}
PerspectiveCamera BlendCamera(const ProductPlan& plan, float amount) {
  PerspectiveCamera out = Blend(plan.ca, plan.cb, amount);
  if (plan.camera_screw.turns) {
    out.camera = BlendScrew(ScaledTransform{plan.ca.camera}, ScaledTransform{plan.cb.camera},
                            plan.camera_screw, amount)
                     .transform;
  }
  return out;
}
// The blended world matrix and the camera's view matrix, as BlendProduct
// would multiply them (world * view * projection).
struct Blended {
  Matrix4 world, view;
};
Blended BlendProduct(const ProductPlan& plan, const Matrix4& world_b, const Matrix4& view_b,
                     float amount) {
  Blended out{world_b, view_b};
  if (plan.relative) {
    out.world = WorldMatrixFromScaledTransform(plan.wb);
  }
  if (plan.world == ProductPlan::World::kBlend) {
    out.world = WorldMatrixFromScaledTransform(
        plan.screw ? BlendScrew(plan.wa, plan.wb, plan.screw_motion, amount)
                   : Blend(plan.wa, plan.wb, amount));
  } else if (plan.world == ProductPlan::World::kSpinning) {
    if (std::optional<ScaledTransform> s = BlendSpinning(plan.wa, plan.wb, amount, kCutAngle)) {
      out.world = WorldMatrixFromScaledTransform(*s);
    }
  }
  if (plan.camera) {
    out.view = ViewMatrixFromTransform(BlendCamera(plan, amount).camera);
  }
  if (plan.relative) {
    // Blended in the camera's frame: back to the world through the blended
    // camera, so world * view is the blend.
    out.world = Multiply(out.world, *Inverse(out.view));
  }
  return out;
}

// --- The scene ----------------------------------------------------------------

// Car frame: x right, y up, z forward. The camera sits at the driver's eye
// looking forward (+z), the dial ahead of it, low and facing back at it.
struct Scene {
  const char* name;
  Vec3 start{0, 0, 0};      // the car's position at tick 0
  float speed = 0;          // metres a tick, forward
  float yaw_rate = 0;       // radians a tick
  float roll_amplitude = 0;  // radians, rolling at 2 Hz
  Vec3 dial_axis{0, 0.3f, -1};  // in the car's frame, facing the driver
  // The needle's angle (radians) at tick k.
  float (*needle)(float k, float rate);
  float rate;
  int ticks = 12;
  bool mesh_axis_aligned = true;  // the dial axis is the mesh's local z
};

const Vec3 kEye{-0.4f, 1.1f, 0.0f};      // the camera, in the car's frame
const Vec3 kSpindle{-0.35f, 0.9f, 0.7f};  // the dial's centre, in the car's frame

Transform CarAt(const Scene& s, float k) {
  float yaw = s.yaw_rate * k;
  float roll = s.roll_amplitude * std::sin(2 * kPi * 2 * k / 30);
  Quaternion q = AxisAngle({0, 0, 1}, roll);
  Quaternion y = AxisAngle({0, 1, 0}, yaw);
  // Roll in the car's frame, then yaw in the world (row vectors: local * R).
  Matrix4 r = Multiply(Rigid(q, {}), Rigid(y, {}));
  // Drive along the heading, integrated per tick (an arc when turning).
  Vec3 p = s.start;
  for (int i = 0; i < int(k); i++) {
    float h = s.yaw_rate * (i + 0.5f);
    p = Add(p, {std::sin(h) * s.speed, 0, std::cos(h) * s.speed});
  }
  return Transform{TransformFromWorldMatrix(r)->rotation, p};
}

// The needle's mesh space: its spindle at `mesh_spindle`, ~0.8 m from the
// mesh origin (as measured in the game). Mesh -> car:
// x -> (x - mesh_spindle) * base * turn(angle) + spindle.
struct Needle {
  Vec3 mesh_spindle;
  Quaternion base;  // the mesh's orientation in the car at angle 0
  Vec3 axis;        // the dial axis in the car's frame (unit)
};
Needle MakeNeedle(const Scene& s) {
  Needle n;
  n.axis = Normalize(s.dial_axis);
  if (s.mesh_axis_aligned) {
    // Mesh z along the dial axis: a rotation taking (0,0,1) to the axis.
    Vec3 z{0, 0, 1};
    Vec3 c{z[1] * n.axis[2] - z[2] * n.axis[1], z[2] * n.axis[0] - z[0] * n.axis[2],
           z[0] * n.axis[1] - z[1] * n.axis[0]};
    float sine = std::sqrt(Dot(c, c)), cosine = Dot(z, n.axis);
    // Row vectors (v * M) turn the other way from the quaternion's angle.
    n.base = sine > 1e-6f ? AxisAngle(c, -std::atan2(sine, cosine)) : Quaternion{};
  } else {
    n.base = AxisAngle({0.3f, 1, 0.2f}, 0.7f);
  }
  n.mesh_spindle = {0.25f, -0.55f, 0.5f};  // |.| = 0.78 m
  return n;
}
Matrix4 NeedleInCar(const Needle& n, float angle) {
  Matrix4 r = Multiply(Rigid(n.base, {}), Rigid(AxisAngle(n.axis, angle), {}));
  Vec3 t = Sub(kSpindle, ApplyRotation(r, n.mesh_spindle));
  r[12] = t[0];
  r[13] = t[1];
  r[14] = t[2];
  return r;
}
// The needle's signed angle about the dial axis from its rotation in the car,
// and how far that rotation tilts off the axis.
void AngleOf(const Needle& n, const Matrix4& in_car, float& angle, float& tilt) {
  Matrix4 base_inverse = WorldMatrixFromTransform(Transform{{-n.base.x, -n.base.y, -n.base.z, n.base.w}, {}});
  Matrix4 r = in_car;
  r[12] = r[13] = r[14] = 0;
  Matrix4 turn = Multiply(base_inverse, r);
  Quaternion q = TransformFromWorldMatrix(turn)->rotation;
  Vec3 v{q.x, q.y, q.z};
  float along = Dot(v, n.axis);
  angle = 2 * std::atan2(along, q.w);
  if (angle > kPi) angle -= 2 * kPi;
  if (angle < -kPi) angle += 2 * kPi;
  Vec3 off = Sub(v, Scale(n.axis, along));
  tilt = 2 * std::asin(std::min(1.0f, std::sqrt(Dot(off, off))));
}
float Wrap(float a) {
  while (a > kPi) a -= 2 * kPi;
  while (a < -kPi) a += 2 * kPi;
  return a;
}

// Screen pixel (1280x720) of a car-frame point seen from the eye.
std::array<float, 2> Pixel(Vec3 in_camera) {
  Matrix4 p = Projection();
  float x = in_camera[0] * p[0], y = in_camera[1] * p[5], w = in_camera[2];
  return {(x / w + 1) * 640, (1 - y / w) * 360};
}

struct Result {
  float worst_angle = 0, worst_spindle = 0, worst_tilt = 0, worst_pixels = 0;
  int flips = 0, frames = 0;  // flips: drawn turned the wrong way, or past the newer tick
  int plans[3] = {0, 0, 0};
};

Result Run(const Scene& s, Mode mode, bool verbose) {
  Result r;
  Needle n = MakeNeedle(s);
  Matrix4 cam_in_car = Rigid(Quaternion{}, kEye);  // the camera in the car's frame
  Matrix4 projection = Projection();
  auto world_at = [&](int k, Matrix4& needle_world, Matrix4& view, Matrix4& vp) {
    Matrix4 car = WorldMatrixFromTransform(CarAt(s, float(k)));
    needle_world = Multiply(NeedleInCar(n, s.needle(float(k), s.rate)), car);
    Matrix4 camera_world = Multiply(cam_in_car, car);
    view = *Inverse(camera_world);
    vp = Multiply(view, projection);
  };
  Matrix4 wa, va, vpa;
  world_at(0, wa, va, vpa);
  for (int k = 1; k <= s.ticks; k++) {
    Matrix4 wb, vb, vpb;
    world_at(k, wb, vb, vpb);
    ProductPlan plan = PlanProduct(wa, vpa, wb, vpb, mode);
    r.plans[int(plan.world)]++;
    float angle_a = s.needle(float(k - 1), s.rate), angle_b = s.needle(float(k), s.rate);
    for (float amount : {0.0f, 0.2f, 0.4f, 0.6f, 0.8f}) {
      Blended b = BlendProduct(plan, wb, vb, amount);
      // The needle as drawn, relative to the drawn camera, in the car's frame.
      Matrix4 in_camera = Multiply(b.world, b.view);
      Matrix4 in_car = Multiply(in_camera, cam_in_car);
      float angle, tilt;
      AngleOf(n, in_car, angle, tilt);
      float ideal = angle_a + (angle_b - angle_a) * amount;
      float angle_error = std::abs(Wrap(angle - ideal));
      Vec3 spindle = Apply(in_car, n.mesh_spindle);
      float spindle_error = Distance(spindle, kSpindle);
      // On screen: the needle's tip, 6 cm out from the spindle along the
      // drawn angle, against the ideal one.
      Vec3 tip_mesh = Add(n.mesh_spindle, ApplyRotation(*Inverse(Rigid(n.base, {})), {0.06f, 0, 0}));
      Matrix4 ideal_in_car = NeedleInCar(n, ideal);
      std::array<float, 2> drawn_px = Pixel(Sub(Apply(in_car, tip_mesh), kEye));
      std::array<float, 2> ideal_px = Pixel(Sub(Apply(ideal_in_car, tip_mesh), kEye));
      float pixels = std::min(9999.0f, std::hypot(drawn_px[0] - ideal_px[0], drawn_px[1] - ideal_px[1]));
      // A flip: from the older tick's angle, drawn the other way from the
      // turn, or further than it (a turn over pi a tick is drawn the short
      // way by any blend, so it counts too).
      float turn = angle_b - angle_a, moved = Wrap(angle - angle_a);
      bool flip = (std::abs(moved) > 0.05f && (moved > 0) != (turn > 0)) ||
                  std::abs(moved) > std::abs(turn) + 0.05f;
      r.frames++;
      r.flips += flip;
      r.worst_angle = std::max(r.worst_angle, angle_error);
      r.worst_spindle = std::max(r.worst_spindle, spindle_error);
      r.worst_tilt = std::max(r.worst_tilt, tilt);
      r.worst_pixels = std::max(r.worst_pixels, pixels);
      if (verbose) {
        static const char* kPlans[] = {"none", "blend", "spinning"};
        std::printf("    tick %2d amount %.1f %-8s angle %7.3f ideal %7.3f (ticks %.3f to %.3f) "
                    "err %.3f rad, spindle %.1f mm, tilt %.3f, tip %.1f px%s\n",
                    k, amount, kPlans[int(plan.world)], angle, Wrap(ideal), Wrap(angle_a),
                    Wrap(angle_b), angle_error, spindle_error * 1000, tilt, pixels,
                    flip ? "  FLIP" : "");
      }
    }
    wa = wb;
    va = vb;
    vpa = vpb;
  }
  return r;
}

float Steady(float k, float rate) { return -2.0f + rate * k; }
float Sine(float k, float rate) {  // swinging +-rate*3 about the middle
  return 3 * rate * std::sin(k / 3);
}
float Rev(float k, float rate) {  // flat out up, then a hard drop, then up
  float cycle = std::fmod(k, 8.0f);
  return cycle < 5 ? -2.0f + rate * cycle : -2.0f + rate * 5 - 1.6f * rate * (cycle - 5);
}

}  // namespace

int main(int argc, char** argv) {
  bool verbose = argc > 1 && std::strcmp(argv[1], "-v") == 0;
  std::vector<Scene> scenes;
  const Vec3 far{-2178, 16.7f, 919};  // world coordinates seen in the game
  // The needle's speed (radians a tick, 30 ticks a second): from slow to
  // past half a turn a tick, densely around kCutAngle (0.5) and pi.
  for (float rate : {0.05f, 0.1f, 0.2f, 0.3f, 0.4f, 0.45f, 0.49f, 0.51f, 0.6f, 0.8f, 1.0f, 1.5f,
                     2.0f, 2.5f, 3.0f, 3.3f}) {
    for (auto f : {Steady, Rev}) {
      scenes.push_back({"parked", {}, 0, 0, 0, {0, 0.3f, -1}, f, rate});
      scenes.push_back({"straight 1 m/tick", {}, 1.0f, 0, 0, {0, 0.3f, -1}, f, rate});
      // The car's own turn speed, radians a tick.
      for (float yaw : {0.02f, 0.05f, 0.1f, 0.2f, 0.4f}) {
        static char names[5][40];
        int i = yaw == 0.02f ? 0 : yaw == 0.05f ? 1 : yaw == 0.1f ? 2 : yaw == 0.2f ? 3 : 4;
        std::snprintf(names[i], sizeof(names[i]), "car turning %.2f/tick", yaw);
        scenes.push_back({names[i], {}, 1.0f, yaw, 0, {0, 0.3f, -1}, f, rate});
      }
      scenes.push_back({"far, turning 0.1, rolling", far, 1.0f, 0.1f, 0.05f, {0, 0.3f, -1}, f, rate});
      scenes.push_back({"far, turning 0.1, tilted mesh", far, 1.0f, 0.1f, 0.03f, {0.2f, 0.5f, -1},
                        f, rate, 12, false});
    }
  }
  static const char* kModes[] = {"normal", "screw", "relative", "relative, uncut"};
  std::printf("worst a frame: spindle off its place (mm) / needle tip off on screen (px, 1280x720) / "
              "flipped frames, of %d; ticks blended b, spinning s, not blended n\n",
              12 * 5);
  std::printf("%-30s %-6s %4s |", "scene", "needle", "rate");
  for (const char* m : kModes) std::printf(" %-25s|", m);
  std::printf("\n");
  int failures[4] = {0, 0, 0, 0};
  for (const Scene& s : scenes) {
    const char* kind = s.needle == Steady ? "steady" : s.needle == Sine ? "sine" : "rev";
    std::printf("%-30s %-6s %4.2f |", s.name, kind, s.rate);
    for (int m = 0; m < 4; m++) {
      Result r = Run(s, Mode(m), false);
      bool bad = r.flips > 0 || r.worst_pixels > 3 || r.worst_tilt > 0.02f;
      failures[m] += bad;
      char plans[16];
      std::snprintf(plans, sizeof(plans), "%db%ds%dn", r.plans[1], r.plans[2], r.plans[0]);
      std::printf("%c%6.1f %6.0f %2d %-8s|", bad ? '*' : ' ', r.worst_spindle * 1000,
                  r.worst_pixels, r.flips, plans);
    }
    std::printf("\n");
    if (verbose) {
      for (int m = 0; m < 4; m++) {
        std::printf("  %s:\n", kModes[m]);
        Run(s, Mode(m), true);
      }
    }
  }
  std::printf("cases over 3 px, tilted or flipped, of %zu: normal %d, screw %d, relative %d, "
              "relative uncut %d\n",
              scenes.size(), failures[0], failures[1], failures[2], failures[3]);
}
