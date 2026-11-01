// The cockpit body's and the traffic body's texture binds (see
// file_loaded.h), load for load and store for store as the recompiled
// functions.

#include "scene_loaders/file_loaded.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "car_models/load_slots.h"
#include "car_physics/float_ops.h"
#include "game_data/data_file.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "model_animation/model_animation.h"
#include "models/part_model.h"
#include "models/xpr.h"
#include "native/buffers.h"
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

bool TrafficFileLoadedNative(uint8_t* base, uint32_t request, uint32_t traffic) {
  if (!native::Buffers_NativeMemcpy()) {
    return false;
  }
  if (Load(base, traffic + 3748) != request) {
    return true;
  }
  uint32_t freed = 0;
  return car_models::LoadSlotReleasable(base, traffic + 3744, &freed);
}

std::optional<uint64_t> TrafficFileLoaded(uint8_t* base, uint32_t request, uint32_t traffic,
                                          uint32_t stack, const FileLoadedCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820D8430 TrafficCar::OnFileLoaded");
  constexpr uint32_t kSuffix = 0x820050B8;
  const uint32_t f = stack - kTrafficLoadedFrameBytes;
  // sprintf's call: r5 the name, r6 the suffix (r7 to r10, which the call
  // doesn't set, as 0: only dead frame words differ).
  const uint32_t name = Load(base, (Load(base, traffic + 32) << 2) + kTrafficNames);
  const uint64_t args[6] = {name, Constant(kSuffix), 0, 0, 0, 0};
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, f + 32 + 8 * i, uint32_t(args[i] >> 32));
    Store(base, f + 36 + 8 * i, uint32_t(args[i]));
  }
  int32_t written;
  if (!c_runtime::Sprintf(base, f + 240, kTwoStrings, f + 32, f - 144 - 1328, &written)) {
    return std::nullopt;
  }
  if (Load(base, traffic + 3748) != request) {
    return uint64_t(uint32_t(written));
  }
  const uint32_t file = Load(base, traffic + 3760);
  const auto entry = [&](uint32_t index) {
    game_data::GetEntry(base, file, index, true);  // asked twice, as the original
    return game_data::GetEntry(base, file, index, true);
  };
  const uint32_t xpr = traffic + 4016;
  XprArrived(base, xpr, entry(0), traffic + 4064, calls);
  // The plate: the struct's last 16 bytes into the frame's parameter area.
  Require(native::Buffers_Memcpy(base, f + 80, traffic + 3696, 16, 0x820D84E0), "memcpy");
  const uint32_t plate = traffic + 3664;
  const uint64_t regs[4] = {
      (uint64_t(Load(base, plate + 0)) << 32) | Load(base, plate + 4),
      (uint64_t(Load(base, plate + 8)) << 32) | Load(base, plate + 12),
      (uint64_t(Load(base, plate + 16)) << 32) | Load(base, plate + 20),
      (uint64_t(Load(base, plate + 24)) << 32) | Load(base, plate + 28)};
  Require(car_models::MakePlate(base, traffic + 7040, traffic + 7044, traffic + 7048,
                                traffic + 7052, regs, f, calls.plate_release)
              .has_value(),
          "MakePlate");
  Store(base, traffic + 7056, uint32_t(XprFindByName(base, xpr, f + 240)));
  // The node file: the skeleton bound and posed, the tires' nodes found.
  const uint32_t skeleton = traffic + 3776;
  BindAndPose(base, skeleton, entry(1), f, calls);
  const int32_t nodes = int32_t(model_animation::SkeletonNodeCount(base, skeleton));
  static constexpr uint32_t kTireNames[4] = {0x82004FAC, 0x82004FB4, 0x82004FBC, 0x82004FC4};
  static constexpr uint32_t kTireReturns[4] = {0x820D85EC, 0x820D8630, 0x820D8674, 0x820D86B8};
  if (nodes > 0) {
    for (uint32_t n = 0; n < uint32_t(nodes); n++) {
      const uint32_t node_name = Load(base, traffic + 3860) + 160 * n;
      for (uint32_t k = 0; k < 4; k++) {
        if (SameName(base, kTireNames[k], node_name)) {
          const uint32_t to = traffic + 7136 + 64 * k;
          const uint32_t from = Load(base, traffic + 3924) + 64 * n;
          matrix_math::Copy(base, to, from);
          interpolation::BlendState_OnCopy(base, to, from, kTireReturns[k]);
        }
      }
    }
  }
  const uint32_t turn = f + 176;
  QuarterTurn(base, f + 112, turn, f + 96, f);
  // The tires' positions: their matrices' rows 3, w = 1.0, x, y, z scaled
  // (every word read before it's written: the order of the original's
  // multiplies doesn't matter), turned, then w = 0.0.
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t at = traffic + 7072 + 16 * k;
    Move8(base, traffic + 7136 + 64 * k + 48, at);
    Move8(base, traffic + 7136 + 64 * k + 56, at + 8);
    if (k < 3) {
      CopyWord(base, kOne, at + 12);
    }
  }
  const double tenth = Lfs(base, kTenth);
  for (uint32_t k = 0; k < 4; k++) {
    for (uint32_t w = 0; w < 3; w++) {
      const uint32_t at = traffic + 7072 + 16 * k + 4 * w;
      Stfs(base, at, Fmuls(Lfs(base, at), tenth));
    }
  }
  CopyWord(base, kOne, traffic + 7132);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t at = traffic + 7072 + 16 * k;
    matrix_math::Vec4Transform(base, at, turn, at);
  }
  for (uint32_t k = 0; k < 4; k++) {
    CopyWord(base, kZero, traffic + 7072 + 16 * k + 12);
  }
  // The six part models.
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t model = traffic + 4256 + 464 * i;
    const uint32_t part = entry(2 + i);
    if (!models::ModelLoad(base, model, part, 0, calls.model_load)) {
      calls.model_load_original(model, part);
    }
  }
  const auto r3 = car_models::LoadSlotReleaseBuffer(base, traffic + 3744);
  Require(r3.has_value(), "LoadSlot::ReleaseBuffer");
  Store(base, traffic + 3764, 3);
  return *r3;
}

}  // namespace scene_loaders
