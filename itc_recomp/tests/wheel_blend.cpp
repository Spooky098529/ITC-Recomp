// Runs the frame blending on a car's wheel and checks every frame against the
// wheel turning evenly between ticks: its spin, its hub's place on the car
// and its rim on screen. The wheel's origin is on its axle and its spin
// follows the car's speed (a 0.32 m wheel: half a turn a tick at ~1 m a tick,
// ~108 km/h), seen from a chase camera. A wheel is measured against its car's
// body as drawn (blended the same way), as it's seen moving on the car.
//
// Besides the game's blends (as in needle_blend.cpp, whose copies of
// blend_state.cpp's PlanProduct, BlendCamera and BlendProduct these are; keep
// them in step), it tests a rule for turns over half a turn a tick: each
// product's turn taken as the one (plus whole turns) nearest its turn the
// tick before ("history"). Build and run (from itc_recomp/):
//   clang++ -std=c++23 -O1 -I src tests/wheel_blend.cpp \
//     src/interpolation/transform.cpp -o /tmp/wheel_blend && /tmp/wheel_blend
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
// A turn about the line through `centre` along `axis` (row vectors).
Matrix4 TurnAbout(Vec3 centre, Vec3 axis, float angle) {
  Matrix4 m = Rigid(AxisAngle(axis, angle), {});
  Vec3 t = Sub(centre, ApplyRotation(m, centre));
  m[12] = t[0];
  m[13] = t[1];
  m[14] = t[2];
  return m;
}
float Wrap(float a) {
  while (a > kPi) a -= 2 * kPi;
  while (a < -kPi) a += 2 * kPi;
  return a;
}
Matrix4 Projection() {
  float n = 0.1f, f = 5000, sy = 2.4f, sx = 1.35f;
  return Matrix4{sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, f / (f - n), 1, 0, 0, -n * f / (f - n), 0};
}
// Screen pixel (1280x720) of a view-space point.
std::array<float, 2> Pixel(Vec3 v) {
  Matrix4 p = Projection();
  return {(v[0] * p[0] / v[2] + 1) * 640, (1 - v[1] * p[5] / v[2]) * 360};
}

// --- Copies of blend_state.cpp, plus the modes tested ---------------------------

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
// kNormal, kScrew: the game's blends (--screw_blend off and on), with no
// angle cut since 2026-10-19 (any turn the short way). kRelative: blended
// relative to the camera (world * view) as a screw, keeping the old angle
// cut (past kCutAngle, the wheel path) for comparison. kRelativeUncut: that
// without the cut. kHistory: that with each turn taken nearest the
// product's turn the tick before.
enum class Mode { kNormal, kScrew, kRelative, kRelativeUncut, kHistory };
const char* kModes[] = {"normal", "screw", "relative", "rel. uncut", "history"};
constexpr int kModeCount = 5;

struct ProductPlan {
  enum class World { kNone, kBlend, kSpinning } world = World::kNone;
  bool relative = false;  // wa, wb are world * view
  ScaledTransform wa, wb;
  bool screw = false;
  ScrewMotion screw_motion;
  bool chosen_turn = false;  // kHistory: turn by `turn_angle` about `turn_axis`
  Vec3 turn_axis{};
  float turn_angle = 0;
  bool camera = false;
  PerspectiveCamera ca, cb;
  ScrewMotion camera_screw;
};
// `last_turn`: the product's turn the tick before, axis times angle (kHistory).
ProductPlan PlanProduct(const Matrix4& world_a, const Matrix4& vp_a, const Matrix4& world_b,
                        const Matrix4& vp_b, Mode mode, Vec3& last_turn) {
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
  if (mode >= Mode::kRelative && plan.camera) {
    plan.relative = true;
    in_a = Multiply(world_a, ViewMatrixFromTransform(plan.ca.camera));
    in_b = Multiply(world_b, ViewMatrixFromTransform(plan.cb.camera));
  }
  std::optional<ScaledTransform> wa = ScaledTransformFromWorldMatrix(in_a);
  std::optional<ScaledTransform> wb = ScaledTransformFromWorldMatrix(in_b);
  Vec3 turn{};
  if (wa && wb) {
    plan.wa = *wa;
    plan.wb = *wb;
    bool cut = mode == Mode::kRelative
                   ? IsCut(wa->transform, wb->transform)
                   : Distance(wa->transform.position, wb->transform.position) > kCutDistance;
    if (!cut) {
      plan.world = ProductPlan::World::kBlend;
      if (screw_blend) {
        plan.screw = true;
        plan.screw_motion = ScrewBetween(wa->transform, wb->transform);
      }
      if (mode == Mode::kHistory && plan.screw_motion.turns) {
        // The turn the short way: angle phi in [0, pi] about u. The same
        // matrices come from phi + 2 pi k; take the k nearest last tick's.
        Quaternion q = plan.screw_motion.turn;
        if (q.w < 0) {
          q = {-q.x, -q.y, -q.z, -q.w};
        }
        float sine = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
        Vec3 u{q.x / sine, q.y / sine, q.z / sine};
        float phi = 2 * std::atan2(sine, q.w);
        float previous = Dot(last_turn, u);
        float k = std::round((previous - phi) / (2 * kPi));
        plan.chosen_turn = true;
        plan.turn_axis = u;
        plan.turn_angle = phi + 2 * kPi * k;
        turn = Scale(u, plan.turn_angle);
      }
    } else if (Distance(wa->transform.position, wb->transform.position) <= kCutDistance &&
               BlendSpinning(*wa, *wb, 0, kCutAngle)) {
      plan.world = ProductPlan::World::kSpinning;
    }
  }
  last_turn = turn;
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
// BlendScrew with the turn given: `angle` about `axis` (may be over pi).
ScaledTransform BlendScrewTurn(const ScaledTransform& a, const ScaledTransform& b,
                               const ScrewMotion& screw, Vec3 axis, float angle, float amount) {
  ScaledTransform out;
  for (int k = 0; k < 3; k++) {
    out.scale[k] = a.scale[k] + (b.scale[k] - a.scale[k]) * amount;
  }
  Matrix4 partial = Rigid(AxisAngle(axis, angle * amount), {});
  out.transform.rotation =
      TransformFromWorldMatrix(Multiply(Rigid(a.transform.rotation, {}), partial))->rotation;
  Vec3 turned = ApplyRotation(partial, screw.point);
  for (int k = 0; k < 3; k++) {
    out.transform.position[k] =
        a.transform.position[k] + screw.point[k] - turned[k] + amount * screw.slide[k];
  }
  return out;
}
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
        plan.chosen_turn ? BlendScrewTurn(plan.wa, plan.wb, plan.screw_motion, plan.turn_axis,
                                          plan.turn_angle, amount)
        : plan.screw     ? BlendScrew(plan.wa, plan.wb, plan.screw_motion, amount)
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
    out.world = Multiply(out.world, *Inverse(out.view));
  }
  return out;
}

// --- The scene ----------------------------------------------------------------

// Car frame: x right, y up, z forward.
constexpr float kRadius = 0.32f;
const Vec3 kHub{0.8f, 0.32f, 1.3f};      // front right wheel, in its car's frame
const Vec3 kAxle{1, 0, 0};
const Vec3 kChaseEye{0, 1.6f, -5.5f};    // the camera, on the player's car

struct Car {
  Vec3 start{};
  float speed = 0;     // metres a tick at tick 0
  float accel = 0;     // metres a tick, per tick
  float yaw_rate = 0;  // radians a tick
};
float DistanceAt(const Car& c, float k) {
  float d = c.speed * k + 0.5f * c.accel * k * k;
  if (c.accel < 0) {  // braking to a stop
    float stop = -c.speed / c.accel;
    if (k > stop) d = c.speed * stop + 0.5f * c.accel * stop * stop;
  }
  return d;
}
Matrix4 CarAt(const Car& c, int k) {
  Vec3 p = c.start;
  for (int i = 0; i < k; i++) {
    float step = DistanceAt(c, float(i + 1)) - DistanceAt(c, float(i));
    float h = c.yaw_rate * (i + 0.5f);
    p = Add(p, {std::sin(h) * step, 0, std::cos(h) * step});
  }
  return Rigid(AxisAngle({0, 1, 0}, c.yaw_rate * k), p);
}

struct Scene {
  std::string name;
  Car player;            // carries the camera
  Car target;            // whose wheel is watched (the player's, or another car)
  bool own_wheel = true;  // target is the player's car
  float steer = 0;       // steering amplitude (radians), at 1 Hz
  int ticks = 12;
  int warmup = 1;        // first ticks not measured (all modes)
};

// The wheel's mesh: origin at the hub, z along the axle, x out to the rim.
Quaternion WheelBase() { return AxisAngle({0, 1, 0}, -kPi / 2); }  // z -> x (row vectors)
float SpinAt(const Scene& s, float k) { return DistanceAt(s.target, k) / kRadius; }
float SteerAt(const Scene& s, float k) { return s.steer * std::sin(2 * kPi * k / 30); }
// Wheel mesh -> car: spun about the axle, then steered about the vertical
// through the hub.
Matrix4 WheelInCar(float spin, float steer) {
  Matrix4 m = Multiply(Rigid(WheelBase(), {}), Rigid(AxisAngle(kAxle, spin), {}));
  m[12] = kHub[0];
  m[13] = kHub[1];
  m[14] = kHub[2];
  return Multiply(m, TurnAbout(kHub, {0, 1, 0}, steer));
}
// The spin of a wheel-in-car matrix with its steering taken out.
float SpinOf(const Matrix4& in_car, float steer) {
  Matrix4 unsteered = Multiply(in_car, *Inverse(TurnAbout(kHub, {0, 1, 0}, steer)));
  unsteered[12] = unsteered[13] = unsteered[14] = 0;
  Quaternion b = WheelBase();
  Matrix4 turn = Multiply(Rigid({-b.x, -b.y, -b.z, b.w}, {}), unsteered);
  Quaternion q = TransformFromWorldMatrix(turn)->rotation;
  return Wrap(2 * std::atan2(Dot({q.x, q.y, q.z}, kAxle), q.w));
}

struct Result {
  float worst_hub = 0, worst_rim = 0, worst_spin = 0;
  int wrong = 0, frames = 0;  // wrong: spin over 0.3 rad off, or hub over 2 cm
};

Result Run(const Scene& s, Mode mode, bool verbose) {
  Result r;
  Matrix4 cam_in_car = Rigid(Quaternion{}, kChaseEye);
  Matrix4 projection = Projection();
  struct Tick {
    Matrix4 wheel, body, view, vp;
  };
  auto at = [&](int k) {
    Tick t;
    Matrix4 car = CarAt(s.target, k);
    t.body = car;
    t.wheel = Multiply(WheelInCar(SpinAt(s, float(k)), SteerAt(s, float(k))), car);
    t.view = *Inverse(Multiply(cam_in_car, CarAt(s.player, k)));
    t.vp = Multiply(t.view, projection);
    return t;
  };
  Vec3 wheel_turn{}, body_turn{};
  Tick a = at(0);
  for (int k = 1; k <= s.ticks; k++) {
    Tick b = at(k);
    ProductPlan wheel = PlanProduct(a.wheel, a.vp, b.wheel, b.vp, mode, wheel_turn);
    ProductPlan body = PlanProduct(a.body, a.vp, b.body, b.vp, mode, body_turn);
    float spin_a = SpinAt(s, float(k - 1)), spin_b = SpinAt(s, float(k));
    float steer_a = SteerAt(s, float(k - 1)), steer_b = SteerAt(s, float(k));
    for (float amount : {0.0f, 0.2f, 0.4f, 0.6f, 0.8f}) {
      if (k <= s.warmup) break;
      Blended w = BlendProduct(wheel, b.wheel, b.view, amount);
      Blended c = BlendProduct(body, b.body, b.view, amount);
      Matrix4 wheel_in_camera = Multiply(w.world, w.view);
      Matrix4 body_in_camera = Multiply(c.world, c.view);
      Matrix4 in_car = Multiply(wheel_in_camera, *Inverse(body_in_camera));
      float spin_ideal = spin_a + (spin_b - spin_a) * amount;
      float steer_ideal = steer_a + (steer_b - steer_a) * amount;
      float spin = SpinOf(in_car, steer_ideal);
      float spin_error = std::abs(Wrap(spin - spin_ideal));
      float hub = Distance(Apply(in_car, {0, 0, 0}), kHub);
      Vec3 rim{kRadius, 0, 0};
      Matrix4 ideal_in_camera = Multiply(WheelInCar(spin_ideal, steer_ideal), body_in_camera);
      std::array<float, 2> drawn = Pixel(Apply(wheel_in_camera, rim));
      std::array<float, 2> ideal = Pixel(Apply(ideal_in_camera, rim));
      float rim_px = std::hypot(drawn[0] - ideal[0], drawn[1] - ideal[1]);
      bool wrong = spin_error > 0.3f || hub > 0.02f;
      r.frames++;
      r.wrong += wrong;
      r.worst_hub = std::max(r.worst_hub, hub);
      r.worst_rim = std::max(r.worst_rim, rim_px);
      r.worst_spin = std::max(r.worst_spin, spin_error);
      if (verbose) {
        std::printf("    tick %2d amount %.1f spin %7.3f ideal %7.3f (turn %.3f a tick) err %.3f, "
                    "hub %.1f mm, rim %.1f px%s\n",
                    k, amount, spin, Wrap(spin_ideal), spin_b - spin_a, spin_error, hub * 1000,
                    rim_px, wrong ? "  WRONG" : "");
      }
    }
    a = b;
  }
  return r;
}

}  // namespace

int main(int argc, char** argv) {
  bool verbose = argc > 1 && std::strcmp(argv[1], "-v") == 0;
  std::vector<Scene> scenes;
  auto name = [](const char* what, float speed) {
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "%s %3.0f km/h (%.2f/tick)", what, speed * 108,
                  speed / kRadius);
    return std::string(buffer);
  };
  // Speeds in metres a tick (x 108 = km/h); spin a tick = speed / 0.32 m.
  for (float speed : {0.2f, 0.5f, 0.9f, 1.0f, 1.2f, 1.6f, 2.0f, 2.5f}) {
    Car straight{{}, speed, 0, 0};
    Car turning{{}, speed, 0, 0.05f};
    scenes.push_back({name("own, straight", speed), straight, straight});
    scenes.push_back({name("own, turning, steering", speed), turning, turning, true, 0.25f});
    // Another car alongside, a little faster and turning differently.
    Car player{{}, speed, 0, 0.02f};
    Car other{{3, 0, 6}, speed + 0.1f, 0, 0.04f};
    scenes.push_back({name("other car", speed), player, other, false, 0.15f});
    // Seen first at this speed (a wheel appearing, or pairing lost): the
    // history has nothing to go on for the first measured tick.
    Scene fresh{name("own, appearing at", speed), straight, straight};
    fresh.warmup = 0;
    scenes.push_back(fresh);
  }
  // From rest to ~270 km/h at 0.03 m a tick per tick (~27 m/s^2), and
  // braking hard from it to a stop.
  {
    Car accelerating{{}, 0, 0.03f, 0.01f};
    Scene s{"own, accelerating from rest to 270", accelerating, accelerating};
    s.ticks = 84;
    scenes.push_back(s);
    Car braking{{}, 2.5f, -0.05f, 0.01f};
    Scene t{"own, braking from 270 to rest", braking, braking};
    t.ticks = 55;
    scenes.push_back(t);
  }
  std::printf("worst a frame: hub off its place on the car (mm) / rim off on screen (px, "
              "1280x720) / wrong frames (spin over 0.3 rad off, or hub over 2 cm)\n");
  std::printf("%-40s|", "scene");
  for (const char* m : kModes) std::printf(" %-18s|", m);
  std::printf("\n");
  int failures[kModeCount] = {};
  for (const Scene& s : scenes) {
    std::printf("%-40s|", s.name.c_str());
    for (int m = 0; m < kModeCount; m++) {
      Result r = Run(s, Mode(m), false);
      bool bad = r.wrong > 0 || r.worst_rim > 3;
      failures[m] += bad;
      std::printf("%c%5.1f %5.0f %2d/%-3d|", bad ? '*' : ' ', r.worst_hub * 1000, r.worst_rim,
                  r.wrong, r.frames);
    }
    std::printf("\n");
    if (verbose) {
      for (int m = 0; m < kModeCount; m++) {
        std::printf("  %s:\n", kModes[m]);
        Run(s, Mode(m), true);
      }
    }
  }
  std::printf("cases over 3 px or wrong, of %zu:", scenes.size());
  for (int m = 0; m < kModeCount; m++) std::printf(" %s %d,", kModes[m], failures[m]);
  std::printf("\n");
}
