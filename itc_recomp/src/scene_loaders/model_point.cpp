// ModelPoint::Find (see model_point.h), call for call as the recompiled
// function.

#include "scene_loaders/model_point.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "model_animation/model_animation.h"
#include "native/buffers.h"
#include "scene_loaders/pools.h"

#include <bit>
#include <cstdlib>

namespace scene_loaders {

namespace {

using namespace car_physics::ops;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// ld then std: a doubleword moved.
void Move8(uint8_t* base, uint32_t from, uint32_t to) {
  const uint32_t hi = Load(base, from);
  const uint32_t lo = Load(base, from + 4);
  Store(base, to, hi);
  Store(base, to + 4, lo);
}

constexpr uint32_t kQuarterTurn = 0x82002A50;  // -pi / 2
constexpr uint32_t kTenth = 0x82001FBC;        // 0.1

// The identity (vupkd3d128's rows, stored aligned) at `at`.
void Identity(uint8_t* base, uint32_t at) {
  const uint32_t aligned = at & ~0xFu;
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t w = 0; w < 16; w++) {
    Store(base, aligned + 4 * w, w % 5 == 0 ? one : 0);
  }
}

}  // namespace

uint32_t ModelPointFind(uint8_t* base, uint32_t point, uint32_t model, uint32_t stack,
                        const UpdateMatricesCall& update_matrices) {
  ITC_NATIVE_RUNNING("sub_820D02D0 ModelPoint::Find");
  const uint32_t f = stack - kModelPointFrameBytes;
  Store(base, point + 228, 0xFFFFFFFFu);
  Store(base, point + 0, 0);
  Store(base, point + 8, 0);
  Store(base, point + 4, 0);
  Identity(base, f + 80);
  if (!native::Buffers_Memcpy(base, point + 16, f + 80, 64, 0x820D0350)) {
    ITC_LOG_ERROR("Scene loaders: ModelPoint::Find's memcpy refused after its checks");
    std::abort();
  }
  if (Load(base, model + 408) == 0) {
    return 0;
  }
  Identity(base, f + 80);
  for (uint32_t w = 0; w < 8; w++) {
    Move8(base, f + 80 + 8 * w, f + 160 + 8 * w);
  }
  const uint32_t turn = f + 160;
  CopyWord(base, kQuarterTurn, f + 144);
  matrix_math::RotateY(base, turn, turn, f + 144, f - 80);
  const uint32_t skeleton = model + 112;
  model_animation::SkeletonResetPose(base, skeleton);
  model_animation::SkeletonLocalMatrices(base, skeleton, f);
  if (model_animation::SkeletonUpdateMatrices(base, skeleton) ==
      model_animation::MatricesResult::kOriginal) {
    update_matrices(skeleton);
  }
  const int32_t nodes = int32_t(model_animation::SkeletonNodeCount(base, skeleton));
  int32_t n = 0;
  for (; n < nodes; n++) {
    if (NameMatches(base, Load(base, skeleton + 84) + 160 * uint32_t(n), point + 160) != 0) {
      break;
    }
  }
  if (n >= nodes) {
    return 0;
  }
  Store(base, point + 4, model);
  Store(base, point + 0, skeleton);
  const uint32_t matrix = point + 16;
  const uint32_t from = Load(base, skeleton + 148) + (uint32_t(n) << 6);
  matrix_math::Copy(base, matrix, from);
  interpolation::BlendState_OnCopy(base, matrix, from, 0x820D045C);
  const double x = Lfs(base, point + 64);
  const double y = Lfs(base, point + 68);
  const double z = Lfs(base, point + 72);
  for (const uint32_t at : {92u, 108u, 124u, 140u}) {
    CopyWord(base, kOne, f + at);
  }
  const double tenth = Lfs(base, kTenth);
  Stfs(base, point + 64, Fmuls(x, tenth));
  Stfs(base, point + 68, Fmuls(y, tenth));
  Stfs(base, point + 72, Fmuls(z, tenth));
  matrix_math::Multiply(base, f + 80, matrix, turn);
  interpolation::BlendState_OnMultiply(base, f + 80, matrix, turn, 0x820D04B0);
  matrix_math::Copy(base, matrix, f + 80);
  interpolation::BlendState_OnCopy(base, matrix, f + 80, 0x820D04BC);
  Move8(base, point + 64, point + 144);
  Move8(base, point + 72, point + 152);
  matrix_math::Inverse(base, point + 80, matrix);
  Store(base, point + 224, uint32_t(n));
  return 1;
}

}  // namespace scene_loaders
