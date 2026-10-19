// The cockpit dials posed natively (--native_cockpit_dials): the tachometer
// and speedometer (sub_820E42E8) and the dash's other needles, the turbo gauge
// among them (sub_820E45A8). See docs/cockpit-dial-interpolation.md.
//
// Both are called from Draw's per-car update (sub_820DAC68), on every pass.
// Each needle's world matrix is (B * RotZ(angle)) * A * the car's world
// matrix (car + 16), A and B the needle's two local matrices, and its angle a
// stateless map of one of the car's gauge values (car + 696 speed, + 700 revs,
// + 704 boost, + 712, + 716, + 720). Render-only passes see the tick's car
// world, but the rest of the car is drawn blended between ticks, and the old
// way (the needle's whole matrix blended between ticks) cut across the dial on
// fast swings. So here:
//
// - the gauge values are the logic pass's, read on it and kept, so the
//   needles step once a tick as on the 360 (car + 704 also drifts on
//   render-only passes: sub_820DAC68 lowers it every call);
// - the car's world matrix is blended between its last two ticks by the
//   frame's amount, as the car's parts are (blend_state.cpp's PlanProduct and
//   BlendProduct);
// - the needle is drawn with its tick pose, as the original draws it (the
//   game culls it with the tick's camera: posed on the blended car, it was
//   culled on logic passes at speed, behind the cockpit camera), and its
//   pose on the blended car goes to the replay beside it
//   (BlendState_SetLivePose), which gives it the frame's camera.
//
// So the needle's turn (B * RotZ(angle) * A, the needle on the dash) is the
// tick's, never blended, and only the car under it is: the needle stays on
// its spindle while the dial moves with the car.
//
// Possible improvement: needles that sweep instead of stepping at 30 Hz. In
// InputsFor, blend the kept gauge values between the last two ticks by the
// frame's amount (keeping the previous tick's as the world matrix's are) and
// work the angle out from them for the frame pose; the tick pose handed to the
// game stays on the tick's values. The turn would still be RotZ about the
// spindle, so the needle wouldn't cut across the dial as the old matrix blend
// did: that blended the finished matrix, not the angle.
//
// The angle maps (sub_820D3688, sub_820D3740) use the game's operations in its
// order; the rotation's sine and cosine come from the C library rather than
// the game's polynomial. --check_native_cockpit_dials compares the angles
// and poses with the translated code's on logic passes.

#include "frame_pacing/game_messages.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "interpolation/transform.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <smmintrin.h>

#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <unordered_map>

REXCVAR_DEFINE_BOOL(native_cockpit_dials, true, "ITC",
                    "Pose the cockpit dials natively (sub_820E42E8, sub_820E45A8) from the "
                    "tick's gauge values and the car's blended world matrix, instead of "
                    "through the translated code with the needles' matrices blended");
REXCVAR_DEFINE_BOOL(check_native_cockpit_dials, false, "ITC",
                    "--native_cockpit_dials: compare its angles and poses with the translated "
                    "code's on logic passes, logged every 2 s");

namespace {

using interpolation::Matrix4;

constexpr uint32_t kConstants = 0x82000000;  // lis r11,-32256
constexpr uint32_t kZero = 2612, kOne = 2608, kAngleScale = 10568;
constexpr uint32_t kCarWorld = 16;
// The gauge values the angle maps read, in the order kept.
constexpr std::array<uint32_t, 6> kGaugeOffsets = {696, 700, 704, 712, 716, 720};
enum Gauge { kSpeed, kRevs, kBoost, k712, k716, k720 };
// blend_state.cpp's kCutDistance: a car world moving further in a tick is
// taken as the newer tick's.
constexpr float kCutDistance = 8.0f;

float Constant(uint8_t* base, uint32_t offset) {
  return guest::LoadFloat(base, kConstants + offset);
}
double ConstantDouble(uint8_t* base, uint32_t offset) {
  uint64_t bits = (uint64_t(guest::LoadU32(base, kConstants + offset)) << 32) |
                  guest::LoadU32(base, kConstants + offset + 4);
  return std::bit_cast<double>(bits);
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

// sub_8209BD50: a * b, each element a row of a dotted with a column of b
// (vmsum4fp128, which the translation does with dpps).
Matrix4 Multiply(const Matrix4& a, const Matrix4& b) {
  __m128 columns[4];
  for (int j = 0; j < 4; j++) {
    columns[j] = _mm_setr_ps(b[j], b[4 + j], b[8 + j], b[12 + j]);
  }
  Matrix4 out;
  for (int i = 0; i < 4; i++) {
    __m128 row = _mm_loadu_ps(&a[4 * i]);
    for (int j = 0; j < 4; j++) {
      out[4 * i + j] = _mm_cvtss_f32(_mm_dp_ps(row, columns[j], 0xFF));
    }
  }
  return out;
}

// sub_820A2D30 turning the identity: a turn by `angle` about z.
Matrix4 RotationZ(float angle) {
  float c = float(std::cos(double(angle))), s = float(std::sin(double(angle)));
  return {c, s, 0, 0, -s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
}

// A needle's pose: (B * RotZ(angle)) * A * the car's world matrix.
Matrix4 Pose(uint8_t* base, uint32_t locals, float angle, const Matrix4& car_world) {
  Matrix4 a = LoadMatrix(base, locals), b = LoadMatrix(base, locals + 64);
  return Multiply(Multiply(Multiply(b, RotationZ(angle)), a), car_world);
}

// sub_820D3688: a dial's angle from gauge `kind` (0-3: car + 704, 712, 716,
// 720) mapped from [range + 0, + 4] to [range + 8, + 12], clamped. Its middle
// piece doesn't add range + 8 back (the game's own).
float DialAngle(uint8_t* base, uint32_t range, uint32_t kind, const std::array<float, 6>& gauges) {
  float zero = Constant(base, kZero);
  if (range == 0) {
    return zero;
  }
  float value = kind < 1    ? gauges[kBoost]
                : kind == 1 ? gauges[k712]
                : kind < 3  ? gauges[k716]
                : kind == 3 ? gauges[k720]
                            : zero;
  float from = guest::LoadFloat(base, range), to = guest::LoadFloat(base, range + 4);
  float angle_from = guest::LoadFloat(base, range + 8);
  float angle_to = guest::LoadFloat(base, range + 12);
  float angle;
  if (from > value) {
    angle = angle_from;
  } else if (to < value) {
    angle = angle_to;
  } else {
    angle = ((value - from) / (to - from)) * (angle_to - angle_from);
  }
  return angle * Constant(base, kAngleScale);
}

// sub_820D3740: the tachometer dash's two angles. `a` from the speed: 0
// below the constant at 2612; up to from (table + 0, or + 8 when
// `alternate`) value / from * table[16]; then linear to table[20] at to
// (table + 4 or + 12); above, table[20]. `b` from the revs: table[28]
// below table[24], linear over table[24..36] then table[32..44], clamped at
// table[44].
std::array<float, 2> TachAngles(uint8_t* base, uint32_t table, bool alternate,
                                const std::array<float, 6>& gauges) {
  auto at = [&](uint32_t offset) { return guest::LoadFloat(base, table + offset); };
  auto fused = [](float x, float y, float z) {  // fmadds
    return float(std::fma(double(x), double(y), double(z)));
  };
  float speed = gauges[kSpeed], revs = gauges[kRevs];
  float from = at(alternate ? 8 : 0), to = at(alternate ? 12 : 4);
  float angle_from = at(16), angle_to = at(20);
  float zero = Constant(base, kZero);
  float a;
  if (speed < zero) {
    a = zero;
  } else if (to < speed) {
    a = angle_to;
  } else if (speed < from) {
    a = speed / from * angle_from;
  } else {
    a = fused((speed - from) / (to - from), angle_to - angle_from, angle_from);
  }
  float low = at(24), low_angle = at(28), mid = at(32), mid_angle = at(36);
  float high = at(40), high_angle = at(44);
  float b;
  if (low > revs) {
    b = low_angle;
  } else if (high < revs) {
    b = high_angle;
  } else if (mid > revs) {
    b = fused((revs - low) / (mid - low), mid_angle - low_angle, low_angle);
  } else {
    b = fused((revs - mid) / (high - mid), high_angle - mid_angle, mid_angle);
  }
  return {a, b};
}

// sub_820E45A8's mode-5 angle for its second needle (the turbo gauge): from
// the boost, clamped.
float BoostAngle(uint8_t* base, float boost) {
  if (boost > Constant(base, kZero)) {
    return boost > Constant(base, kOne) ? Constant(base, 25312) : boost * Constant(base, 25312);
  }
  if (double(boost) < ConstantDouble(base, 25304)) {
    return Constant(base, 25296);
  }
  return boost * Constant(base, 25292) * Constant(base, 25296);
}

// What a car's dials are posed from: the gauge values and world matrix of its
// last two logic passes.
struct CarDials {
  uint64_t tick = 0;  // the logic pass count they were read at (0: never)
  std::array<float, 6> gauges{};
  Matrix4 world_before{}, world_now{};
  bool has_before = false;
};
std::unordered_map<uint32_t, CarDials> g_cars;

struct PoseInputs {
  std::array<float, 6> gauges;
  Matrix4 world;    // the car's, blended for the frame being drawn
  Matrix4 tick;     // the car's at the tick (car + 16 now)
};

// Main thread, from a dial function: on a logic pass the gauges as they are
// now (and the car's world on its first call this pass, the earlier one kept),
// on a render-only pass the logic pass's; the car's world blended by the
// frame's amount.
PoseInputs InputsFor(uint8_t* base, uint32_t car) {
  uint64_t logic_passes = 0, render_only_passes = 0;
  frame_pacing::GameMessages_PassCounts(logic_passes, render_only_passes);
  CarDials& dials = g_cars[car];
  if (!frame_pacing::GameMessages_IsRenderOnlyPass() || dials.tick == 0) {
    for (size_t i = 0; i < kGaugeOffsets.size(); i++) {
      dials.gauges[i] = guest::LoadFloat(base, car + kGaugeOffsets[i]);
    }
    if (dials.tick != logic_passes) {
      Matrix4 world = LoadMatrix(base, car + kCarWorld);
      dials.has_before = dials.tick != 0 && dials.tick + 1 == logic_passes;
      dials.world_before = dials.world_now;
      dials.world_now = world;
      dials.tick = logic_passes;
    }
  }
  PoseInputs inputs{dials.gauges, dials.world_now, dials.world_now};
  float amount = interpolation::BlendState_Amount();
  if (dials.has_before && amount < 1.0f) {
    auto a = interpolation::ScaledTransformFromWorldMatrix(dials.world_before);
    auto b = interpolation::ScaledTransformFromWorldMatrix(dials.world_now);
    if (a && b && interpolation::Distance(a->transform.position, b->transform.position) <=
                      kCutDistance) {
      inputs.world =
          interpolation::WorldMatrixFromScaledTransform(interpolation::Blend(*a, *b, amount));
    }
  }
  return inputs;
}

// --check_native_cockpit_dials: the translated code's results for the same
// inputs, on logic passes (when the gauges kept are memory's). Scratch space
// and the callees' frames are below `frame`, our stand-in for the original's.
struct Check {
  uint64_t angles = 0, poses = 0;
  float angle_error = 0, pose_error = 0;
  std::chrono::steady_clock::time_point logged = std::chrono::steady_clock::now();

  bool Wanted() const {
    return REXCVAR_GET(check_native_cockpit_dials) &&
           !frame_pacing::GameMessages_IsRenderOnlyPass();
  }
  void Angle(float native, float game) {
    angles++;
    angle_error = std::max(angle_error, std::fabs(native - game));
  }
  void Pose(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t locals, float angle,
            const Matrix4& car_world, const Matrix4& native) {
    uint32_t r1 = ctx.r1.u32;
    uint32_t scratch = frame - 0x200;  // four matrices and the angle
    uint32_t m0 = scratch, m1 = scratch + 64, m2 = scratch + 128, angle_at = scratch + 192;
    ctx.r1.u32 = frame - 0x280;
    StoreMatrix(base, m1, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    guest::StoreFloat(base, angle_at, angle);
    ctx.r3.u32 = m0, ctx.r4.u32 = m1, ctx.r5.u32 = angle_at;
    __imp__sub_820A2D30(ctx, base);  // m0 = RotZ
    ctx.r3.u32 = m1, ctx.r4.u32 = locals + 64, ctx.r5.u32 = m0;
    __imp__sub_8209BD50(ctx, base);  // m1 = B * R
    ctx.r3.u32 = m2, ctx.r4.u32 = m1, ctx.r5.u32 = locals;
    __imp__sub_8209BD50(ctx, base);  // m2 = (B * R) * A
    StoreMatrix(base, m1, car_world);
    ctx.r3.u32 = m0, ctx.r4.u32 = m2, ctx.r5.u32 = m1;
    __imp__sub_8209BD50(ctx, base);  // m0 = ... * car
    Matrix4 game = LoadMatrix(base, m0);
    ctx.r1.u32 = r1;
    poses++;
    for (int i = 0; i < 16; i++) {
      pose_error = std::max(pose_error, std::fabs(native[i] - game[i]));
    }
  }
  float GameDialAngle(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t range,
                      uint32_t kind, uint32_t car) {
    uint32_t r1 = ctx.r1.u32;
    ctx.r1.u32 = frame - 0x280;
    ctx.r3.u32 = range, ctx.r4.u32 = kind, ctx.r5.u32 = car;
    __imp__sub_820D3688(ctx, base);
    ctx.r1.u32 = r1;
    return float(ctx.f1.f64);
  }
  std::array<float, 2> GameTachAngles(PPCContext& ctx, uint8_t* base, uint32_t frame,
                                      uint32_t table, uint32_t car, uint32_t dash_type) {
    uint32_t r1 = ctx.r1.u32;
    uint32_t out = frame - 0x200;
    ctx.r1.u32 = frame - 0x280;
    ctx.r3.u32 = table, ctx.r4.u32 = car, ctx.r5.u32 = dash_type;
    ctx.r6.u32 = out, ctx.r7.u32 = out + 4;
    __imp__sub_820D3740(ctx, base);
    ctx.r1.u32 = r1;
    return {guest::LoadFloat(base, out), guest::LoadFloat(base, out + 4)};
  }
  void MaybeLog() {
    auto now = std::chrono::steady_clock::now();
    if (now - logged >= std::chrono::seconds(2)) {
      logged = now;
      REXLOG_INFO("cockpit dials check: {} angles (max error {:.7f}), {} poses (max error {:.7f})",
                  angles, angle_error, poses, pose_error);
      angles = poses = 0;
      angle_error = pose_error = 0;
    }
  }
};
Check g_check;

// Draws a needle as the original's sub_820D06F0 calls do: its tick pose
// `drawn` (on the tick's car, as the original's: the game culls it with the
// tick's camera, and products pair across passes by it) at the original's
// frame + 160, `lr` the original's return address (for diagnostics keyed by
// caller); `world`, its pose for the frame, goes to the replay.
void DrawNeedle(PPCContext& ctx, uint8_t* base, uint32_t frame, uint32_t part, uint32_t camera,
                const Matrix4& drawn, const Matrix4& world, uint32_t lr) {
  uint32_t r1 = ctx.r1.u32;
  uint32_t world_at = frame + 160;
  StoreMatrix(base, world_at, drawn);
  ctx.r1.u32 = frame;
  ctx.r3.u32 = part, ctx.r4.u32 = camera, ctx.r5.u32 = world_at;
  ctx.r6.u64 = 0, ctx.r7.u64 = 0;
  ctx.lr = lr;
  interpolation::BlendState_SetLivePose(world.data());
  sub_820D06F0(ctx, base);
  interpolation::BlendState_SetLivePose(nullptr);
  ctx.r1.u32 = r1;
}

// The original's stack frame, below the caller's: the same addresses for the
// world matrix as the original's, so draws pair across passes as before.
uint32_t MakeFrame(uint8_t* base, uint32_t r1, uint32_t size) {
  uint32_t frame = r1 - size;
  guest::StoreU32(base, frame, r1);  // the back chain, as stwu leaves it
  return frame;
}

// sub_820E45A8's needles: whether it's drawn ([[dash] + enable] and
// [dash + model]), what's stored into the model + 48 first, its angle's
// range (sub_820D3688's r3, [dash + range]) and gauge ([[dash] + kind]), its
// local matrices (dash + locals, + 64), the part drawn (dash + part) and the
// original's return address from drawing it.
struct Needle {
  uint32_t enable, kind, range, model_value, model, locals, part, lr;
};
constexpr std::array<Needle, 7> kNeedles = {{
    {8, 12, 5328, 5296, 1700, 32, 2176, 0x820E4734},
    {20, 24, 5332, 5300, 1940, 272, 2416, 0x820E4914},  // mode 5: the turbo gauge
    {32, 36, 5336, 5304, 2660, 512, 3136, 0x820E4A6C},
    {44, 48, 5340, 5308, 2900, 752, 3376, 0x820E4BC4},
    {56, 60, 5344, 5312, 3620, 992, 4336, 0x820E4D1C},
    {68, 72, 5348, 5316, 3860, 1232, 4576, 0x820E4E74},
    {80, 84, 5352, 5320, 4100, 1472, 4816, 0x820E4FCC},
}};
constexpr uint32_t kDialsFrame = 1680, kTachFrame = 736;
constexpr uint32_t kCameraOffset = 144;  // into the camera passes (r5)
constexpr uint32_t kMode = 4, kMode5Value = 5324;
constexpr uint32_t kTachTable = 2896;
constexpr std::array<std::array<uint32_t, 3>, 2> kTachNeedles = {{
    {32, 496, 0x820E4474},   // a: the speedometer (locals, part, return address)
    {272, 736, 0x820E4594},  // b: the tachometer
}};

}  // namespace

// The dash's needles: r3 = the dash, r4 = the car, r5 = the camera passes.
// In mode 5 ([dash + 4]) the second needle is the turbo gauge, from car + 704.
extern "C" REX_FUNC(sub_820E45A8) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(native_cockpit_dials)) {
    __imp__sub_820E45A8(ctx, base);
    return;
  }
  uint32_t dash = ctx.r3.u32, car = ctx.r4.u32, camera = ctx.r5.u32 + kCameraOffset;
  uint32_t type = guest::LoadU32(base, dash);
  uint32_t frame = MakeFrame(base, ctx.r1.u32, kDialsFrame);
  PoseInputs inputs = InputsFor(base, car);
  bool check = g_check.Wanted();
  for (size_t k = 0; k < kNeedles.size(); k++) {
    const Needle& needle = kNeedles[k];
    uint32_t model = guest::LoadU32(base, dash + needle.model);
    if (guest::LoadU32(base, type + needle.enable) == 0 || model == 0) {
      continue;
    }
    bool boost = k == 1 && guest::LoadU32(base, dash + kMode) == 5;
    guest::StoreU32(base, model + 48,
                    guest::LoadU32(base, dash + (boost ? kMode5Value : needle.model_value)));
    float angle;
    if (boost) {
      angle = BoostAngle(base, inputs.gauges[kBoost]);
    } else {
      uint32_t range = guest::LoadU32(base, dash + needle.range);
      uint32_t kind = guest::LoadU32(base, type + needle.kind);
      angle = DialAngle(base, range, kind, inputs.gauges);
      if (check) {
        g_check.Angle(angle, g_check.GameDialAngle(ctx, base, frame, range, kind, car));
      }
    }
    Matrix4 drawn = Pose(base, dash + needle.locals, angle, inputs.tick);
    Matrix4 world = Pose(base, dash + needle.locals, angle, inputs.world);
    if (check) {
      g_check.Pose(ctx, base, frame, dash + needle.locals, angle, inputs.tick, drawn);
    }
    DrawNeedle(ctx, base, frame, dash + needle.part, camera, drawn, world, needle.lr);
  }
  if (check) {
    g_check.MaybeLog();
  }
}

// The tachometer's dash (one caller, sub_820E51D0): r3 = the dash, r4 = the
// car, r5 = the camera passes. Its two angles from sub_820D3740: the
// speedometer's and the tachometer's.
extern "C" REX_FUNC(sub_820E42E8) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(native_cockpit_dials)) {
    __imp__sub_820E42E8(ctx, base);
    return;
  }
  uint32_t dash = ctx.r3.u32, car = ctx.r4.u32, camera = ctx.r5.u32 + kCameraOffset;
  uint32_t type = guest::LoadU32(base, dash);
  uint32_t table = guest::LoadU32(base, dash + kTachTable);
  uint32_t frame = MakeFrame(base, ctx.r1.u32, kTachFrame);
  PoseInputs inputs = InputsFor(base, car);
  std::array<float, 2> angles = TachAngles(base, table, guest::LoadU32(base, type) != 0,
                                           inputs.gauges);
  bool check = g_check.Wanted();
  if (check) {
    std::array<float, 2> game = g_check.GameTachAngles(ctx, base, frame, table, car, type);
    g_check.Angle(angles[0], game[0]);
    g_check.Angle(angles[1], game[1]);
  }
  for (size_t i = 0; i < kTachNeedles.size(); i++) {
    auto [locals, part, lr] = kTachNeedles[i];
    Matrix4 drawn = Pose(base, dash + locals, angles[i], inputs.tick);
    Matrix4 world = Pose(base, dash + locals, angles[i], inputs.world);
    if (check) {
      g_check.Pose(ctx, base, frame, dash + locals, angles[i], inputs.tick, drawn);
    }
    DrawNeedle(ctx, base, frame, dash + part, camera, drawn, world, lr);
  }
  if (check) {
    g_check.MaybeLog();
  }
}
