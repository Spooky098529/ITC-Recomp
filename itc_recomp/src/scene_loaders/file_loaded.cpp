// The cockpit body's and the traffic body's texture binds (see
// file_loaded.h), load for load and store for store as the recompiled
// functions.

#include "scene_loaders/file_loaded.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "car_models/load_slots.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "matrix_math/matrix_math.h"
#include "model_animation/model_animation.h"
#include "models/part_model.h"
#include "models/xpr.h"
#include "scene_loaders/pools.h"

#include <bit>
#include <cstdlib>

namespace scene_loaders {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kCockpitNames = 0x826018C8;  // the cars' names, by block
constexpr uint32_t kTrafficNames = 0x82601980;  // the traffic cars' names
constexpr uint32_t kTwoStrings = 0x82004FE8;    // "%s%s"
constexpr uint32_t kNumbered = 0x82004FA4;      // "%s%s%d"
constexpr uint32_t kInteOn = 0x82005008;        // "_inte_on"
constexpr uint32_t kInteOnNumbered = 0x82004FFC;  // "_inte_on_0"
constexpr uint32_t kTrafficSuffix = 0x820050AC;  // "_ex_0"

// sprintf into the frame's +80, called with r5 to r7 = `args` (64-bit, as
// the registers hold them; r8 to r10, which the calls don't set, as 0: only
// dead frame words differ), spilled into the frame's parameter area (+32,
// 8-byte slots) as sprintf spills them, its own frames below.
bool Print(uint8_t* base, uint32_t frame, uint32_t format, const uint64_t (&args)[3]) {
  for (uint32_t i = 0; i < 6; i++) {
    const uint64_t value = i < 3 ? args[i] : 0;
    Store(base, frame + 32 + 8 * i, uint32_t(value >> 32));
    Store(base, frame + 36 + 8 * i, uint32_t(value));
  }
  int32_t written;
  return c_runtime::Sprintf(base, frame + 80, format, frame + 32, frame - 144 - 1328, &written);
}

// A lis/addi constant as the register holds it (sign-extended).
uint64_t Constant(uint32_t address) { return uint64_t(int64_t(int32_t(address))); }

bool Loading(uint8_t* base, uint32_t state) { return int32_t(Load(base, state)) == 2; }

using namespace car_physics::ops;

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene loaders: {} refused after its checks", what);
    std::abort();
  }
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
constexpr uint32_t kAccessoryNode = 0x82004FDC;  // "acce_null"

// strcmp(a, b) == 0 as the original's inline loop: byte by byte until a's
// terminator or a difference.
bool SameName(uint8_t* base, uint32_t constant, uint32_t name) {
  for (uint32_t i = 0;; i++) {
    const uint32_t a = guest::LoadU8(base, constant + i);
    const uint32_t b = guest::LoadU8(base, name + i);
    if (a == 0 || a != b) {
      return a == b;
    }
  }
}

// The identity (vupkd3d128's rows, stored aligned) at `at`, copied to `to`
// (eight doublewords), and turned a quarter about y there by the angle
// stored at `angle` (Matrix::RotateY, its turn at `frame` - 80).
void QuarterTurn(uint8_t* base, uint32_t at, uint32_t to, uint32_t angle, uint32_t frame) {
  const uint32_t aligned = at & ~0xFu;
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t w = 0; w < 16; w++) {
    Store(base, aligned + 4 * w, w % 5 == 0 ? one : 0);
  }
  for (uint32_t w = 0; w < 8; w++) {
    Move8(base, at + 8 * w, to + 8 * w);
  }
  CopyWord(base, kQuarterTurn, angle);
  matrix_math::RotateY(base, to, to, angle, frame - 80);
}

// The skeleton bound to `file` and posed: Skeleton::Bind (r5 = 0),
// ResetPose, LocalMatrices (its frame below `frame`), UpdateMatrices; the
// originals where the plain C++ declines.
void BindAndPose(uint8_t* base, uint32_t skeleton, uint32_t file, uint32_t frame,
                 const FileLoadedCalls& calls) {
  if (!model_animation::SkeletonBind(base, skeleton, file, 0)) {
    calls.skeleton_bind(skeleton, file);
  }
  model_animation::SkeletonResetPose(base, skeleton);
  model_animation::SkeletonLocalMatrices(base, skeleton, frame);
  if (model_animation::SkeletonUpdateMatrices(base, skeleton) ==
      model_animation::MatricesResult::kOriginal) {
    calls.update_matrices(skeleton);
  }
}

// A part model's file arrived: Model::Load (its original where the plain
// C++ declines), LoadSlot::ReleaseBuffer, the state 3. ReleaseBuffer's r3.
uint32_t PartArrived(uint8_t* base, uint32_t slot, uint32_t model, const FileLoadedCalls& calls) {
  const uint32_t file = Load(base, slot + 16);
  if (!models::ModelLoad(base, model, file, 0, calls.model_load)) {
    calls.model_load_original(model, file);
  }
  const auto r3 = car_models::LoadSlotReleaseBuffer(base, slot);
  Require(r3.has_value(), "LoadSlot::ReleaseBuffer");
  Store(base, slot + 20, 3);
  return *r3;
}

// An XPR2 arrived: XPR::Create (its original where the plain C++ declines),
// the provider's slot +16. Its r3.
uint64_t XprArrived(uint8_t* base, uint32_t xpr, uint32_t file, uint32_t provider,
                    const FileLoadedCalls& calls) {
  if (!models::XprCreate(base, xpr, file)) {
    calls.xpr_create(xpr, file);
  }
  return calls.provider_set(provider, xpr);
}

}  // namespace

std::optional<uint32_t> CockpitBindTextures(uint8_t* base, uint32_t cockpit, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D6108 Cockpit::BindTextures");
  for (uint32_t i = 0; i < 6; i++) {
    if (Loading(base, cockpit + 556 + 24 * i)) {
      return 0;
    }
  }
  for (uint32_t i = 0; i < 2; i++) {
    if (Loading(base, cockpit + 420 + 24 * i)) {
      return 0;
    }
  }
  if (Loading(base, cockpit + 196)) {
    return 0;
  }
  const uint32_t frame = stack - kCockpitBindFrameBytes;
  const uint32_t number = Load(base, cockpit + 48);
  const int32_t block = int32_t(Load(base, Load(base, cockpit + 24) + 32));
  const uint32_t name = Load(base, (uint32_t(block) << 2) + kCockpitNames);
  const bool printed =
      number == 0 ? Print(base, frame, kTwoStrings, {name, Constant(kInteOn), number})
                  : Print(base, frame, kNumbered, {name, Constant(kInteOnNumbered), number});
  if (!printed) {
    return std::nullopt;
  }
  const uint32_t xpr = cockpit + 448;
  int32_t index = -1;
  if (block < 18) {
    index = XprFindByName(base, xpr, frame + 80);
    Store(base, cockpit + 3504, uint32_t(models::XprGetTexture(base, xpr, uint32_t(index))));
  }
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t part = cockpit + 688 + 464 * i;
    Store(base, part + 48, cockpit + 496);
    if (block < 18) {
      Store(base, part + 452, uint32_t(models::XprGetTexture(base, xpr, uint32_t(index))));
    }
  }
  return 1;
}

std::optional<uint32_t> TrafficBindTextures(uint8_t* base, uint32_t traffic, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D7D38 TrafficCar::BindTextures");
  if (Loading(base, traffic + 3764)) {
    return 0;
  }
  const uint32_t frame = stack - kTrafficBindFrameBytes;
  const uint32_t xpr = traffic + 4016;
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t part = traffic + 4256 + 464 * i;
    Store(base, part + 48, traffic + 4064);
    models::PartModelSetVinylTextures(base, part, Load(base, traffic + 3728), 0);
    const uint32_t number = Load(base, traffic + 192);
    if (int32_t(number) <= 0 || int32_t(Load(base, traffic + 7056)) == -1) {
      continue;
    }
    const uint32_t name = Load(base, (Load(base, traffic + 32) << 2) + kTrafficNames);
    if (!Print(base, frame, kNumbered, {name, Constant(kTrafficSuffix), number})) {
      return std::nullopt;
    }
    const int32_t index = XprFindByName(base, xpr, frame + 80);
    Store(base, traffic + 7060, uint32_t(index));
    if (index == -1) {
      continue;
    }
    Store(base, part + 460, uint32_t(models::XprGetTexture(base, xpr, uint32_t(index))));
    Store(base, part + 436, Load(base, traffic + 7056));
  }
  return 1;
}

bool CockpitFileLoadedNative(uint8_t* base, uint32_t request, uint32_t cockpit) {
  for (uint32_t i = 0; i < 6; i++) {
    if (Load(base, cockpit + 540 + 24 * i) == request) {
      uint32_t freed = 0;
      return car_models::LoadSlotReleasable(base, cockpit + 536 + 24 * i, &freed);
    }
  }
  return true;
}

uint64_t CockpitFileLoaded(uint8_t* base, uint32_t request, uint32_t buffer, uint32_t cockpit,
                           uint32_t stack, const FileLoadedCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820D6528 Cockpit::OnFileLoaded");
  const uint32_t f = stack - kCockpitLoadedFrameBytes;
  for (uint32_t i = 0; i < 6; i++) {
    if (Load(base, cockpit + 540 + 24 * i) == request) {
      return PartArrived(base, cockpit + 536 + 24 * i, cockpit + 688 + 464 * i, calls);
    }
  }
  for (uint32_t i = 0; i < 2; i++) {
    if (Load(base, cockpit + 404 + 24 * i) == request) {
      const uint64_t r3 =
          XprArrived(base, cockpit + 448 + 24 * i, buffer, cockpit + 496 + 20 * i, calls);
      Store(base, cockpit + 420 + 24 * i, 3);
      return r3;
    }
  }
  if (Load(base, cockpit + 180) != request) {
    return request;
  }
  const uint32_t skeleton = cockpit + 208;
  BindAndPose(base, skeleton, Load(base, cockpit + 192), f, calls);
  Store(base, cockpit + 196, 3);
  const int32_t nodes = int32_t(model_animation::SkeletonNodeCount(base, skeleton));
  uint64_t r3 = uint32_t(nodes);
  if (nodes > 0) {
    const uint32_t names = Load(base, cockpit + 292);
    for (uint32_t n = 0; n < uint32_t(nodes); n++) {
      if (SameName(base, kAccessoryNode, names + 160 * n)) {
        Store(base, cockpit + 3472, Load(base, cockpit + 356) + 64 * n);
      }
    }
    r3 = 0;
  }
  const uint32_t node = Load(base, cockpit + 3472);
  if (node == 0) {
    return r3;
  }
  const uint32_t turn = f + 160;
  QuarterTurn(base, f + 96, turn, f + 80, f);
  const uint32_t point = cockpit + 3488;
  Move8(base, node + 48, point);
  Move8(base, node + 56, point + 8);
  const double x = Lfs(base, point + 0);
  const double y = Lfs(base, point + 4);
  const double z = Lfs(base, point + 8);
  CopyWord(base, kOne, cockpit + 3500);
  const double tenth = Lfs(base, kTenth);
  Stfs(base, point + 0, Fmuls(x, tenth));
  Stfs(base, point + 4, Fmuls(y, tenth));
  Stfs(base, point + 8, Fmuls(z, tenth));
  matrix_math::Vec4Transform(base, point, turn, point);
  return point;
}

}  // namespace scene_loaders
