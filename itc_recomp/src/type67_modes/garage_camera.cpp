// The garage camera's reset and its quaternion
// (docs/functions/type6-type7-modes.md: Quaternion::FromAngles sub_8217C120,
// GarageCamera::Reset sub_8217D888) as plain C++: the translated code's
// float steps (src/xaudio/ppc_float.h), so the results are the original's
// bit for bit, and its frame's stores replayed.

#include "base/float_mode.h"
#include "type67_modes/type67_modes.h"
#include "type67_modes/views.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "matrix_math/matrix_math.h"
#include "schema/front_end_car_draw.h"
#include "xaudio/ppc_float.h"

#include <bit>

namespace type67_modes {

namespace {

namespace pf = xaudio::ppc_float;
using pf::Vec;
using FCD = schema::FrontEndCarDrawGlobals;

// lfs and stfs as the translated code's doubles.
double Lfs(uint32_t address) { return double(guest::LoadFloat(address)); }
void Stfs(uint32_t address, double value) {
  guest::StoreFloat(address, float(value));
}

Vec Splat(Vec v, int word) {
  switch (word) {
    case 0: return simde_mm_shuffle_epi32(v, 0xFF);
    case 1: return simde_mm_shuffle_epi32(v, 0xAA);
    case 2: return simde_mm_shuffle_epi32(v, 0x55);
    default: return simde_mm_shuffle_epi32(v, 0x00);
  }
}
// vrlimi128 with no rotation: the lanes `mask` picks (the host blend's bits)
// from `b`.
template <int kMask>
Vec Insert(Vec a, Vec b) {
  return pf::I(simde_mm_blend_ps(pf::F(a), pf::F(b), kMask));
}

uint64_t LoadU64(uint32_t address) {
  return (uint64_t(guest::LoadU32(address)) << 32) | guest::LoadU32(address + 4);
}
void StoreU64(uint32_t address, uint64_t value) {
  guest::StoreU32(address, uint32_t(value >> 32));
  guest::StoreU32(address + 4, uint32_t(value));
}

// A function's guest frame as a C++ local (E5 step 6): `kSize` bytes in the
// guest's byte order, by the frame's offsets, 16-byte aligned as the guest
// stack is, so the original's stfs, ld, lvlx and stvx on its slots are the
// same bytes and lanes.
template <uint32_t kSize>
struct Scratch {
  alignas(16) uint8_t bytes[kSize] = {};

  void SetU32(uint32_t offset, uint32_t value) {
    bytes[offset] = uint8_t(value >> 24);
    bytes[offset + 1] = uint8_t(value >> 16);
    bytes[offset + 2] = uint8_t(value >> 8);
    bytes[offset + 3] = uint8_t(value);
  }
  uint32_t U32(uint32_t offset) const {
    return (uint32_t(bytes[offset]) << 24) | (uint32_t(bytes[offset + 1]) << 16) |
           (uint32_t(bytes[offset + 2]) << 8) | uint32_t(bytes[offset + 3]);
  }
  // stfs: the double rounded to a float, as Stfs above.
  void Stfs(uint32_t offset, double value) { SetU32(offset, std::bit_cast<uint32_t>(float(value))); }
  uint64_t U64(uint32_t offset) const { return (uint64_t(U32(offset)) << 32) | U32(offset + 4); }
  void SetU64(uint32_t offset, uint64_t value) {
    SetU32(offset, uint32_t(value >> 32));
    SetU32(offset + 4, uint32_t(value));
  }
  // pf::Lvlx and pf::Stvx over these bytes.
  Vec Lvlx(uint32_t offset) const {
    return simde_mm_shuffle_epi8(
        simde_mm_load_si128(reinterpret_cast<const Vec*>(bytes + (offset & ~0xFu))),
        simde_mm_load_si128(
            reinterpret_cast<const Vec*>(&vector_helpers::VectorMaskL[(offset & 0xF) * 16])));
  }
  void Stvx(uint32_t offset, Vec value) {
    simde_mm_store_si128(
        reinterpret_cast<Vec*>(bytes + (offset & ~0xFu)),
        simde_mm_shuffle_epi8(
            value, simde_mm_load_si128(reinterpret_cast<const Vec*>(vector_helpers::VectorMaskL))));
  }
};

// The identity matrix stored at `out` a word at a time in address order
// (the original builds it in its frame and memcpys it: the same bytes).
void StoreIdentity(uint32_t out) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(out + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
}

}  // namespace

void QuaternionFromAngles(uint32_t out, double f1, double f2, double f3,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8217C120 Quaternion::FromAngles");
  pf::Mode mode;
  mode.Scalar();
  // The angles below the stack pointer (f3, f1, 0.0, f2), read back as
  // vectors with lvlx: sp -32 to -17 in the original (sp 16-byte aligned), a
  // C++ local since E5 step 6.
  (void)sp;
  Scratch<16> below;
  below.Stfs(0, f3);
  below.Stfs(4, f1);
  below.Stfs(12, f2);
  below.Stfs(8, Lfs(FCD::zero_address()));
  const Vec v5 = simde_mm_setzero_si128();
  Vec v9 = below.Lvlx(4);
  Vec v0 = below.Lvlx(12);
  Vec v8 = below.Lvlx(8);
  v0 = pf::I(simde_mm_blend_ps(pf::F(v0), simde_mm_permute_ps(pf::F(v9), 57), 4));
  Vec v13 = below.Lvlx(0);
  v13 = pf::I(simde_mm_blend_ps(pf::F(v13), simde_mm_permute_ps(pf::F(v8), 57), 4));
  Vec v10 = pf::Lvx(0x82000A20);
  Vec v6 = Splat(v10, 3);
  v10 = Splat(v10, 1);
  v0 = pf::I(simde_mm_blend_ps(pf::F(v0), simde_mm_permute_ps(pf::F(v13), 78), 3));
  Vec v12 = pf::Lvx(0x820009F0);
  Vec v4 = Splat(v12, 1), v2 = Splat(v12, 2), v31 = Splat(v12, 3);
  mode.Vector();
  // vcfsx of 1 scaled by 2^-1: 0.5 in every lane.
  v12 = pf::I(simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                              simde_mm_castsi128_ps(simde_mm_set1_epi32(0x3F000000))));
  Vec v11 = pf::Lvx(0x820009C0);
  Vec v3 = Splat(v11, 1), v1 = Splat(v11, 2), v30 = Splat(v11, 3);
  // vupkd3d128 of zero: (3.0, 3.0, 0.0, 1.0).
  v11 = simde_mm_castps_si128(simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f));
  v0 = pf::Mul(v0, v12);
  v9 = Splat(v11, 3);
  v13 = pf::Mul(v0, v6);
  v13 = pf::I(simde_mm_round_ps(pf::F(v13), SIMDE_MM_FROUND_TO_NEAREST_INT | SIMDE_MM_FROUND_NO_EXC));
  // vnmsubfp: -(v10 v13 - v0).
  v0 = pf::I(simde_mm_xor_ps(pf::F(pf::Sub(pf::Mul(v10, v13), v0)),
                             simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  v10 = pf::Lvx(0x82000A00);
  v6 = Splat(v10, 0);
  const Vec v29 = Splat(v10, 1), v27 = Splat(v10, 2), v25 = Splat(v10, 3);
  v13 = pf::Mul(v0, v0);
  v12 = pf::Mul(v13, v0);
  Vec v7 = pf::Madd(v4, v13, v9);
  v9 = pf::Lvx(0x820009D0);
  v11 = pf::Mul(v13, v13);
  v4 = Splat(v9, 0);
  const Vec v28 = Splat(v9, 1), v26 = Splat(v9, 2), v24 = Splat(v9, 3);
  v8 = pf::Lvx(0x82000A10);
  const Vec v23 = Splat(v8, 0), v21 = Splat(v8, 1);
  v9 = pf::Madd(v3, v12, v0);
  const Vec v19 = Splat(v8, 2);
  v13 = pf::Mul(v12, v13);
  const Vec v17 = Splat(v8, 3);
  v8 = pf::Madd(v2, v11, v7);
  v0 = pf::Mul(v12, v12);
  v10 = pf::Lvx(0x820009E0);
  v12 = pf::Mul(v11, v12);
  const Vec v22 = Splat(v10, 0), v20 = Splat(v10, 1), v18 = Splat(v10, 2);
  v2 = Splat(v10, 3);
  v7 = pf::Madd(v1, v13, v9);
  v3 = pf::Madd(v31, v0, v8);
  v10 = pf::Mul(v11, v11);
  v11 = pf::Mul(v13, v11);
  v7 = pf::Madd(v30, v12, v7);
  v9 = pf::Mul(v13, v13);
  v13 = pf::Mul(v0, v13);
  v8 = pf::Mul(v0, v0);
  v0 = pf::Mul(v12, v0);
  v1 = pf::Mul(v12, v12);
  v6 = pf::Madd(v6, v10, v3);
  v12 = pf::Mul(v10, v12);
  v7 = pf::Madd(v4, v11, v7);
  v3 = pf::Mul(v9, v9);
  v6 = pf::Madd(v29, v9, v6);
  v7 = pf::Madd(v28, v13, v7);
  v4 = pf::Madd(v27, v8, v6);
  v0 = pf::Madd(v26, v0, v7);
  v7 = pf::Mul(v10, v10);
  v10 = pf::Mul(v11, v10);
  // The sign mask: -1 shifted left by 31, words 0 and 3 cleared.
  v6 = Insert<9>(simde_mm_set1_epi32(int(0x80000000)), v5);
  v4 = pf::Madd(v25, v1, v4);
  v1 = pf::Mul(v13, v13);
  v0 = pf::Madd(v24, v12, v0);
  v12 = pf::Mul(v11, v11);
  v11 = pf::Mul(v9, v11);
  v9 = pf::Mul(v13, v9);
  v13 = pf::Mul(v8, v13);
  v8 = pf::Madd(v23, v7, v4);
  v0 = pf::Madd(v22, v10, v0);
  v12 = pf::Madd(v21, v12, v8);
  v0 = pf::Madd(v20, v11, v0);
  v12 = pf::Madd(v19, v3, v12);
  v11 = pf::Madd(v18, v9, v0);
  v0 = pf::Madd(v17, v1, v12);
  v13 = pf::Madd(v2, v13, v11);
  // The sines (v0) and cosines (v13) into the quaternion.
  v12 = Splat(v0, 0);
  v11 = Splat(v0, 1);
  v10 = Splat(v13, 0);
  v8 = Splat(v13, 1);
  v7 = Splat(v13, 2);
  v12 = Insert<8>(v12, v13);
  v11 = Insert<4>(v11, v13);
  v9 = Splat(v0, 2);
  v10 = Insert<8>(v10, v0);
  v8 = Insert<4>(v8, v0);
  v7 = Insert<2>(v7, v0);
  v0 = pf::Mul(v12, v11);
  v9 = Insert<2>(v9, v13);
  v13 = simde_mm_xor_si128(v10, v6);
  v13 = pf::Mul(v13, v8);
  v0 = pf::Mul(v0, v9);
  v0 = pf::Madd(v13, v7, v0);
  pf::Stvx(out, v0);
}

void GarageCameraReset(uint32_t camera, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8217D888 GarageCamera::Reset");
  const schema::GarageCamera cam(camera);
  pf::Mode mode;
  mode.Scalar();
  // Its frame's slots (+80 to +191, read back only here) a C++ local
  // since E5 step 6.
  (void)frame;
  Scratch<192> fr;
  const double zero = Lfs(FCD::zero_address()), one = Lfs(FCD::one_address());
  // The vectors built in the frame, in the original's store order.
  fr.Stfs(100, Lfs(FCD::pi_address()));
  fr.Stfs(144, zero);
  fr.Stfs(148, zero);
  fr.Stfs(152, zero);
  fr.Stfs(156, one);
  fr.Stfs(112, zero);
  fr.Stfs(116, zero);
  fr.Stfs(120, zero);
  fr.Stfs(124, one);
  fr.Stfs(160, Lfs(0x82009EE4));
  fr.Stfs(140, one);
  // The eye's start (+16) and the target (+32).
  StoreU64(cam.eye_address(), fr.U64(144));
  fr.Stfs(176, zero);
  StoreU64(cam.eye_address() + 8, fr.U64(152));
  StoreU64(cam.target_address(), fr.U64(112));
  fr.Stfs(180, zero);
  StoreU64(cam.target_address() + 8, fr.U64(120));
  fr.Stfs(128, double(float(Lfs(cam.eye_x_address()) - Lfs(cam.target_x_address()))));
  fr.Stfs(132, double(float(Lfs(cam.eye_y_address()) - Lfs(cam.target_y_address()))));
  fr.Stfs(136, double(float(Lfs(cam.eye_z_address()) - Lfs(cam.target_z_address()))));
  const uint64_t d0 = fr.U64(128);
  fr.Stfs(184, zero);
  const uint64_t d1 = fr.U64(136);
  fr.Stfs(188, one);
  const uint64_t e0 = fr.U64(176);
  fr.Stfs(80, zero);
  const uint64_t e1 = fr.U64(184);
  fr.Stfs(84, zero);
  fr.Stfs(88, zero);
  fr.Stfs(92, one);
  StoreU64(cam.eye_offset_address(), d0);
  const uint64_t g0 = fr.U64(80);
  fr.Stfs(96, zero);
  StoreU64(cam.eye_offset_address() + 8, d1);
  const uint64_t g1 = fr.U64(88);
  fr.Stfs(104, zero);
  fr.Stfs(108, one);
  StoreU64(cam.vector_64_address(), e0);
  const uint64_t a0 = fr.U64(96);
  StoreU64(cam.vector_64_address() + 8, e1);
  const uint64_t a1 = fr.U64(104);
  StoreU64(cam.vector_80_address(), g0);
  StoreU64(cam.vector_80_address() + 8, g1);
  StoreU64(cam.angles_address(), a0);
  StoreU64(cam.angles_address() + 8, a1);
  fr.Stfs(172, one);
  fr.Stfs(164, Lfs(0x82002A10));
  fr.Stfs(168, zero);
  StoreU64(cam.translation_address(), fr.U64(160));
  StoreU64(cam.translation_address() + 8, fr.U64(168));
  Stfs(cam.unknown_96_address(), zero);
  Stfs(cam.tilt_address(), Lfs(0x82009EE0));
  Stfs(cam.unknown_104_address(), zero);
  Stfs(cam.fov_address(), Lfs(FCD::sixty_address()));
  // The zoom: 640 / tan(pi / 3), tan in nearest rounding as its _ctrlfp
  // sets it (put back after, as the original's second _ctrlfp does).
  double tangent = 0.0;
  {
    using Fpscr = ::base::FloatMode;
    const uint32_t csr = Fpscr::getcsr();
    Fpscr::setcsr(csr & ~uint32_t(Fpscr::RoundMaskVal));
    if (!c_runtime::Tan(std::bit_cast<double>(LoadU64(0x82009ED8)), &tangent)) {
      ITC_LOG_ERROR("type67_modes: GarageCamera::Reset: tan refused its constant");
    }
    Fpscr::setcsr(csr);
  }
  mode.Scalar();
  Stfs(cam.zoom_address(), double(float(Lfs(FCD::focal_address()) / double(float(tangent)))));
  // The identity (built at the frame's +192 and memcpy'd in the original)
  // at +112.
  StoreIdentity(cam.rotation_address());
  Stfs(cam.orbit_address(), zero);
  Stfs(cam.distance_address(), Lfs(0x82009ED0));
  QuaternionFromAngles(cam.quaternion_address(), zero, zero, zero, frame);
}

namespace {

// GarageCamera::Command's constants (floats).
constexpr uint32_t kTiltStep = 0x82001FBC;   // the tilt's step and its low bound
constexpr uint32_t kTiltHigh = FCD::one_address();   // the tilt's high bound
constexpr uint32_t kStep = FCD::degree_address();       // the turns' step
constexpr uint32_t kTurnLow = FCD::minus_pi_address();    // a turn's low bound (wraps to the high)
constexpr uint32_t kTurnHigh = FCD::pi_address();   // its high bound (wraps to the low)
constexpr uint32_t kZoomStep = 0x8200222C;   // +180's step
constexpr uint32_t kZoomLow = 0x820021F8;
constexpr uint32_t kZoomHigh = 0x820035B0;
constexpr uint32_t kFovLow = 0x82001FD8;     // +224's bounds
constexpr uint32_t kFovHigh = 0x82002A0C;
constexpr uint32_t kMoveStep = 0x82002204;   // +256, +260 and +264's step

// The clamp the original applies: below `low` gives `low`, else above
// `high` gives `high`.
double Clamp(double v, uint32_t low, uint32_t high) {
  const double lo = Lfs(low);
  if (v < lo) {
    return lo;
  }
  const double hi = Lfs(high);
  return v > hi ? hi : v;
}
// The turns' wrap: below the low bound gives the high one, else above the
// high one the low one.
double Wrap(double v) {
  const double lo = Lfs(kTurnLow);
  if (v < lo) {
    return Lfs(kTurnHigh);
  }
  return v > Lfs(kTurnHigh) ? lo : v;
}

}  // namespace

uint64_t GarageCameraCommand(uint32_t camera, uint32_t command, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82180358 GarageCamera::Command");
  pf::Mode mode;
  mode.Scalar();
  const uint32_t c = camera;
  const schema::GarageCamera cam(camera);
  // Its frame stays only for RotateX's angle (+80) and turn (below it),
  // which take guest addresses; no guest code is called from it, so no back
  // chain (E5 step 6).
  const uint32_t frame = sp - 192;
  // `field` the field's address.
  auto add = [&](uint32_t field, uint32_t step, bool up) {
    const double v = Lfs(field), d = Lfs(step);
    return pf::Round(up ? v + d : v - d);
  };
  // The commands that mark the camera moved (+272 = 1).
  bool moved = true;
  switch (command) {
    case 0:  // the tilt up and down (sub_821803A0, the recompiler's piece, for 0)
    case 1:
      Stfs(cam.tilt_address(),
           Clamp(add(cam.tilt_address(), kStep, command == 0), kTiltStep, kTiltHigh));
      break;
    case 2:  // the turn about the car
    case 3:
      Stfs(cam.orbit_address(), Wrap(add(cam.orbit_address(), kStep, command == 2)));
      break;
    case 4:  // the distance
    case 5:
      Stfs(cam.distance_address(),
           Clamp(add(cam.distance_address(), kZoomStep, command == 5), kZoomLow, kZoomHigh));
      break;
    case 6:  // the field of view
    case 7:
      Stfs(cam.fov_address(),
           Clamp(add(cam.fov_address(), kTiltStep, command == 6), kFovLow, kFovHigh));
      moved = false;
      break;
    case 8: case 9: case 10: case 11: case 12: case 13: {  // the angles +240 to +248
      const uint32_t field = cam.angle_x_address() + 4 * ((command - 8) / 2);
      Stfs(field, Wrap(add(field, kStep, command % 2 == 0)));
      moved = false;
      break;
    }
    case 14: case 15: case 16: case 17: case 18: case 19: {  // the offsets
      const uint32_t fields[3] = {cam.translation_y_address(), cam.translation_x_address(),
                                  cam.translation_z_address()};
      const uint32_t field = fields[(command - 14) / 2];
      Stfs(field, add(field, kMoveStep, command % 2 == 0));
      moved = false;
      break;
    }
    case 20: {  // reset
      if (int32_t(cam.moved()) == 1) {
        cam.set_moved(2);
      }
      GarageCameraReset(c, frame - kGarageCameraResetFrame);
      mode.Scalar();
      cam.set_moved(0);
      moved = false;
      break;
    }
    default:  // above 20: only the matrices
      moved = false;
      break;
  }
  if (moved) {
    cam.set_moved(1);
  }
  // The quaternion from the turn (+176) and the tilt (+100); the rotation
  // (+112) the identity (built at the frame's +96 and memcpy'd in the
  // original; stored directly since E5 step 6) turned about x
  // by the tilt (the angle at the frame's +80).
  QuaternionFromAngles(cam.quaternion_address(), Lfs(cam.orbit_address()),
                       Lfs(cam.tilt_address()), Lfs(FCD::zero_address()), frame);
  mode.Scalar();
  StoreIdentity(cam.rotation_address());
  Stfs(frame + 80, Lfs(cam.tilt_address()));
  matrix_math::RotateX(cam.rotation_address(), cam.rotation_address(), frame + 80,
                       frame - 80);
  return cam.rotation_address();
}

uint64_t GarageStageCameraCommand(uint64_t r3, uint32_t command, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821808F0 GarageStage::CameraCommand");
  const schema::GarageStage stage = schema::StandSlot(uint32_t(r3)).stage();
  const schema::GarageCamera camera(stage.camera_address());
  if (int32_t(camera.moved()) == 2 && int32_t(command) != 20) {
    return r3;
  }
  return GarageCameraCommand(camera.address, command, sp);
}

void GarageLightsInit(uint32_t lights, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8217D3C8 GarageLights::Init");
  pf::Mode mode;
  // Its frame's slots (+80 to +127, read back only here) a C++ local
  // since E5 step 6.
  (void)frame;
  Scratch<128> fr;
  // The 11 records (96 bytes each) from the table, a doubleword at a time.
  for (uint32_t i = 0; i < 11 * 96; i += 8) {
    StoreU64(lights + 16 + i, LoadU64(kGarageLightsTable + i));
  }
  // The ambient (0.08, 0.08, 0.08, 1.0) through the frame to +1072 and
  // 0x8265ED00 (front-end-car-draw's ambient).
  mode.Scalar();
  const double ambient = Lfs(0x82002A1C);
  const double one = Lfs(FCD::one_address());
  fr.Stfs(80, ambient);
  fr.Stfs(84, ambient);
  fr.Stfs(88, ambient);
  fr.Stfs(92, one);
  for (uint32_t i = 0; i < 16; i += 8) {
    StoreU64(lights + 1072 + i, fr.U64(80 + i));
  }
  for (uint32_t i = 0; i < 16; i += 8) {
    StoreU64(FCD::ambient_address() + i, LoadU64(lights + 1072 + i));
  }
  // The three lights' constants.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t light = lights + 32 + 96 * i;
    for (uint32_t w = 0; w < 16; w += 8) {
      StoreU64(FCD::light_a_address() + 16 * i + w, LoadU64(light + w));
      StoreU64(FCD::light_b_address() + 16 * i + w, LoadU64(light + 16 + w));
      StoreU64(FCD::light_c_address() + 16 * i + w, LoadU64(light + 32 + w));
    }
  }
  // The four spots (q = +304 + 96i).
  mode.Vector();
  const Vec half = pf::I(simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                                         simde_mm_castsi128_ps(simde_mm_set1_epi32(0x3F000000))));
  const Vec zeros = simde_mm_setzero_si128();
  mode.Scalar();
  fr.Stfs(108, Lfs(FCD::third_pi_address()));
  const double radians = Lfs(FCD::degree_address()), scale = Lfs(FCD::half_address());
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t q = lights + 304 + 96 * i, at = 16 * i;
    mode.Scalar();
    fr.Stfs(124, one);
    fr.Stfs(92, one);
    auto cosine = [&](uint32_t address) {
      mode.Scalar();
      const double angle = double(float(double(float(Lfs(address) * scale)) * radians));
      const double c = c_runtime::Cos(angle);
      mode.Scalar();
      return c;
    };
    const double c0 = double(float(cosine(q + 64)));
    const double c1 = double(float(cosine(q + 68)));
    const double c2 = cosine(q + 68);
    const double c3 = cosine(q + 64);
    const double difference = double(float(c0 - c1));
    fr.Stfs(100, double(float(c2)));
    const Vec v12 = pf::Lvx(q + 16);
    mode.Vector();
    const Vec v0 = pf::I(simde_mm_dp_ps(pf::F(v12), pf::F(v12), 0xEF));
    const uint64_t p0 = LoadU64(q + 80), p1 = LoadU64(q + 88);
    fr.SetU64(112, p0);
    fr.SetU64(120, p1);
    mode.Scalar();
    fr.Stfs(104, double(float(one / difference)));
    fr.Stfs(96, double(float(c3)));
    StoreU64(FCD::spot_positions_address() + at, LoadU64(q));
    mode.Vector();
    Vec v13 = pf::I(simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(pf::F(v0))));
    const Vec v9 = pf::Mul(v0, half);
    const Vec equal = pf::I(simde_mm_cmpeq_ps(pf::F(v0), pf::F(zeros)));
    StoreU64(FCD::spot_positions_address() + at + 8, LoadU64(q + 8));
    const Vec v8 = pf::Mul(v13, v13);
    // vnmsubfp: -(v9 v8 - 0.5), one Newton step.
    const Vec v10 = pf::I(simde_mm_xor_ps(pf::F(pf::Sub(pf::Mul(v9, v8), half)),
                                          simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
    v13 = pf::Madd(v13, v10, v13);
    v13 = pf::Mul(v12, v13);
    const Vec v11 = simde_mm_or_si128(simde_mm_andnot_si128(equal, v13), simde_mm_and_si128(equal, v0));
    fr.Stvx(80, v11);
    mode.Scalar();
    StoreU64(FCD::spot_directions_address() + at, fr.U64(80));
    StoreU64(FCD::spot_directions_address() + at + 8, fr.U64(88));
    StoreU64(FCD::spot_a_address() + at, LoadU64(q + 32));
    StoreU64(FCD::spot_a_address() + at + 8, LoadU64(q + 40));
    StoreU64(FCD::spot_b_address() + at, LoadU64(q + 48));
    StoreU64(FCD::spot_b_address() + at + 8, LoadU64(q + 56));
    StoreU64(FCD::spot_cones_address() + at, fr.U64(96));
    StoreU64(FCD::spot_colors_address() + at, fr.U64(112));
    StoreU64(FCD::spot_cones_address() + at + 8, fr.U64(104));
    StoreU64(FCD::spot_colors_address() + at + 8, fr.U64(120));
  }
}

}  // namespace type67_modes
