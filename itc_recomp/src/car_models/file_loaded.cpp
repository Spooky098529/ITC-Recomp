// The car model's file-loaded callbacks (see file_loaded.h), call for call
// as the recompiled functions.

#include "car_models/file_loaded.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "car_models/load_slots.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "models/xpr.h"
#include "native/buffers.h"
#include "scene_loaders/pools.h"

#include <bit>
#include <cstdlib>

namespace car_models {

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

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Car models: {} refused after its checks", what);
    std::abort();
  }
}

constexpr uint32_t kNames = 0x826018C8;       // the cars' names, by block
constexpr uint32_t kQuarterTurn = 0x82002A50;  // -pi / 2
constexpr uint32_t kTenth = 0x82001FBC;        // 0.1
constexpr uint32_t kTwoStrings = 0x82004FE8;   // "%s%s"
constexpr uint32_t kNumbered = 0x82004FA4;     // "%s%s%d"
// The nodes' names: tire_fl, tire_fr, tire_rl, tire_rr, seat_L, seat_R,
// acce_null.
constexpr uint32_t kNodeNames[7] = {0x82004FAC, 0x82004FB4, 0x82004FBC, 0x82004FC4,
                                    0x82004FCC, 0x82004FD4, 0x82004FDC};
constexpr uint32_t kNodeMatrices[6] = {6720, 6784, 6848, 6912, 7040, 7104};

// What differs between the two: the suffixes and the return addresses.
struct Variant {
  uint32_t suffix;           // "_inte_off" / "_inte_on"
  uint32_t numbered_suffix;  // "_inte_off_0" / "_inte_on_0"
  uint32_t copies[6];        // Matrix::Copy's returns
  uint32_t plate_memcpy;     // memcpy's return before MakePlate
};
constexpr Variant kVariants[2] = {
    {0x82004FF0,
     0x82004F98,
     {0x820D48FC, 0x820D4944, 0x820D498C, 0x820D49D4, 0x820D4A1C, 0x820D4A64},
     0x820D4EEC},
    {0x82005008,
     0x82004FFC,
     {0x820D50D8, 0x820D5120, 0x820D5168, 0x820D51B0, 0x820D51F8, 0x820D5240},
     0x820D56B8},
};

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

// sprintf of the car's name and a suffix (and a number) into the frame's
// +384: its spills into the frame's parameter area (+32, 8-byte slots; the
// arguments the call doesn't set as 0: only dead frame words differ), its
// own frames below.
bool Print(uint8_t* base, uint32_t f, uint32_t format, uint32_t name, uint32_t suffix,
           int32_t number, bool numbered) {
  const uint32_t args[6] = {name, suffix, numbered ? uint32_t(number) : 0, 0, 0, 0};
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, f + 32 + 8 * i, 0);
    Store(base, f + 36 + 8 * i, args[i]);
  }
  int32_t written;
  return c_runtime::Sprintf(base, f + 384, format, f + 32, f - 144 - 1328, &written);
}

// The texture set the request is for (0 or 1), or -1.
int TextureSet(uint8_t* base, uint32_t request, uint32_t car) {
  for (int i = 0; i < 2; i++) {
    if (Load(base, car + 3956 + 24 * uint32_t(i)) == request) {
      return i;
    }
  }
  return -1;
}

// Four words from `from` to `to` (two doubleword moves) with `to` + 12 =
// 1.0 after the first.
void PositionRow(uint8_t* base, uint32_t from, uint32_t to) {
  Move8(base, from, to);
  Move8(base, from + 8, to + 8);
}

}  // namespace

bool FileLoadedNative(uint8_t* base, FileLoadedSet set, uint32_t request, uint32_t car) {
  const int i = TextureSet(base, request, car);
  if (i < 0) {
    return true;
  }
  if (i == 0 && int32_t(Load(base, car + 3724)) == 1) {
    return false;
  }
  if (set == FileLoadedSet::kA) {
    const uint32_t buffer = Load(base, car + 3968 + 24 * uint32_t(i));
    return buffer != 0 && memory::IsPages(buffer);
  }
  return true;
}

void FileLoaded(uint8_t* base, FileLoadedSet set, uint32_t request, uint32_t buffer,
                uint32_t car, uint32_t stack, const FileLoadedCalls& calls) {
  if (set == FileLoadedSet::kA) {
    ITC_NATIVE_RUNNING("sub_820D47A8 CarModel::OnFileLoaded");
  } else {
    ITC_NATIVE_RUNNING("sub_820D4F80 CarModel::OnFileLoadedB");
  }
  const Variant& variant = kVariants[set == FileLoadedSet::kA ? 0 : 1];
  const uint32_t f = stack - kFileLoadedFrameBytes;
  const auto name = [&] { return Load(base, (Load(base, car + 32) << 2) + kNames); };
  Require(Print(base, f, kTwoStrings, name(), variant.suffix, 0, false), "sprintf");
  const int texture_set = TextureSet(base, request, car);
  if (texture_set >= 0) {
    const uint32_t i = uint32_t(texture_set);
    const uint32_t slot = car + 24 * i;
    const uint32_t xpr = slot + 4000;
    const uint32_t file = set == FileLoadedSet::kA ? Load(base, slot + 3968) : buffer;
    if (!models::XprCreate(base, xpr, file)) {
      calls.xpr_create(xpr, file);
    }
    calls.provider_set(car + 20 * i + 4048, xpr);
    if (set == FileLoadedSet::kA) {
      Store(base, slot + 3972, 3);
      Require(FreePagesAt(base, slot + 3968).has_value(), "the buffer's release");
      Store(base, slot + 3968, 0);
    } else {
      Store(base, slot + 3968, 0);
      Store(base, slot + 3972, 3);
    }
    if (i != 0) {
      return;
    }
    // (+3724 isn't 1: FileLoadedNative.)
    Require(native::Buffers_Memcpy(base, f + 80, car + 3696, 16, variant.plate_memcpy),
            "memcpy");
    const uint32_t plate = car + 3664;
    const uint64_t regs[4] = {
        (uint64_t(Load(base, plate + 0)) << 32) | Load(base, plate + 4),
        (uint64_t(Load(base, plate + 8)) << 32) | Load(base, plate + 12),
        (uint64_t(Load(base, plate + 16)) << 32) | Load(base, plate + 20),
        (uint64_t(Load(base, plate + 24)) << 32) | Load(base, plate + 28)};
    Require(MakePlate(base, car + 6700, car + 6704, car + 6708, car + 6712, regs, f,
                      calls.plate_release)
                .has_value(),
            "MakePlate");
    Store(base, car + 7232, uint32_t(scene_loaders::XprFindByName(base, car + 4000, f + 384)));
    for (int32_t k = 1; k < 6; k++) {
      Require(Print(base, f, kNumbered, name(), variant.numbered_suffix, k, true), "sprintf");
      Store(base, car + 7232 + 4 * uint32_t(k),
            uint32_t(scene_loaders::XprFindByName(base, car + 4000, f + 384)));
    }
    return;
  }
  if (Load(base, car + 3732) != request) {
    return;
  }
  // The node file: the skeleton bound and posed.
  const uint32_t skeleton = car + 3760;
  const uint32_t node_file = Load(base, car + 3744);
  if (!model_animation::SkeletonBind(base, skeleton, node_file, 0)) {
    calls.skeleton_bind(skeleton, node_file);
  }
  model_animation::SkeletonResetPose(base, skeleton);
  model_animation::SkeletonLocalMatrices(base, skeleton, f);
  if (model_animation::SkeletonUpdateMatrices(base, skeleton) ==
      model_animation::MatricesResult::kOriginal) {
    calls.update_matrices(skeleton);
  }
  Store(base, car + 3748, 3);
  int32_t found = 0;
  int32_t nodes = int32_t(model_animation::SkeletonNodeCount(base, skeleton));
  if (nodes > 0) {
    uint32_t record = 0, matrix = 0;
    for (; nodes != 0; nodes--, record += 160, matrix += 64) {
      const uint32_t node_name = Load(base, car + 3844) + record;
      for (int k = 0; k < 6; k++) {
        if (SameName(base, kNodeNames[k], node_name)) {
          const uint32_t to = car + kNodeMatrices[k];
          const uint32_t from = Load(base, car + 3908) + matrix;
          matrix_math::Copy(base, to, from);
          interpolation::BlendState_OnCopy(base, to, from, variant.copies[k]);
          found++;
        }
      }
      if (SameName(base, kNodeNames[6], node_name)) {
        found++;
        Store(base, car + 7200, Load(base, car + 3908) + matrix);
      }
    }
  }
  // The quarter turn about y: the identity (vupkd3d128's, stored aligned
  // at +224) copied to +288 and turned by the angle at +144.
  {
    const uint32_t aligned = (f + 224) & ~0xFu;
    const uint32_t one = std::bit_cast<uint32_t>(1.0f);
    for (uint32_t w = 0; w < 16; w++) {
      Store(base, aligned + 4 * w, w % 5 == 0 ? one : 0);
    }
    for (uint32_t w = 0; w < 8; w++) {
      Move8(base, f + 224 + 8 * w, f + 288 + 8 * w);
    }
  }
  const uint32_t turn = f + 288;
  CopyWord(base, kQuarterTurn, f + 144);
  matrix_math::RotateY(base, turn, turn, f + 144, f - 80);
  // The four tires' positions (their matrices' rows 3), w = 1.0, scaled by
  // 0.1 and turned; then w = 0.0.
  const double tenth = Lfs(base, kTenth);
  const auto scale = [&](uint32_t at) { Stfs(base, at, Fmuls(Lfs(base, at), tenth)); };
  static constexpr uint32_t kTires[4] = {6976, 6992, 7008, 7024};
  for (int k = 0; k < 4; k++) {
    PositionRow(base, car + kNodeMatrices[k] + 48, car + kTires[k]);
    if (k < 3) {
      CopyWord(base, kOne, car + kTires[k] + 12);
    }
  }
  // (Every word read before it's written: the order of the original's
  // multiplies doesn't matter.)
  for (int k = 0; k < 4; k++) {
    for (uint32_t w = 0; w < 3; w++) {
      scale(car + kTires[k] + 4 * w);
    }
  }
  CopyWord(base, kOne, car + 7036);
  for (int k = 0; k < 4; k++) {
    matrix_math::Vec4Transform(base, car + kTires[k], turn, car + kTires[k]);
  }
  for (int k = 0; k < 4; k++) {
    CopyWord(base, kZero, car + kTires[k] + 12);
  }
  if (found == 7) {
    PositionRow(base, car + 7088, car + 7168);
    CopyWord(base, kOne, car + 7180);
    PositionRow(base, car + 7152, car + 7184);
    for (const uint32_t at : {7168u, 7184u}) {
      for (uint32_t w = 0; w < 3; w++) {
        scale(car + at + 4 * w);
      }
    }
    CopyWord(base, kOne, car + 7196);
    matrix_math::Vec4Transform(base, car + 7168, turn, car + 7168);
    matrix_math::Vec4Transform(base, car + 7184, turn, car + 7184);
    CopyWord(base, kZero, car + 7180);
    CopyWord(base, kZero, car + 7196);
  }
  const uint32_t accessory = Load(base, car + 7200);
  if (accessory != 0) {
    Move8(base, accessory + 48, car + 7216);
    Move8(base, accessory + 56, car + 7224);
    for (uint32_t w = 0; w < 3; w++) {
      scale(car + 7216 + 4 * w);
    }
    CopyWord(base, kOne, car + 7228);
    matrix_math::Vec4Transform(base, car + 7216, turn, car + 7216);
  }
}

}  // namespace car_models
