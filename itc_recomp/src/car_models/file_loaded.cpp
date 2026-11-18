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
#include "schema/car_models.h"

#include <bit>
#include <cstdlib>

namespace car_models {

namespace {

using namespace car_physics::ops;
using schema::CarModel;
using G = schema::CarModelsGlobals;

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

constexpr uint32_t kTwoStrings = 0x82004FE8;   // "%s%s"
constexpr uint32_t kNumbered = 0x82004FA4;     // "%s%s%d"
// The nodes' names: tire_fl, tire_fr, tire_rl, tire_rr, seat_L, seat_R,
// acce_null.
constexpr uint32_t kNodeNames[7] = {0x82004FAC, 0x82004FB4, 0x82004FBC, 0x82004FC4,
                                    0x82004FCC, 0x82004FD4, 0x82004FDC};
// The six nodes' matrices in the car model: the tires', then the seats'.
uint32_t NodeMatrix(const CarModel& car, int k) {
  return k < 4 ? car.tire_matrices_address() + 64 * uint32_t(k)
               : car.seat_matrices_address() + 64 * uint32_t(k - 4);
}

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
  const CarModel c(base, car);
  for (int i = 0; i < 2; i++) {
    if (schema::LoadSlot(base, c.xpr_slots_address() + 24 * uint32_t(i)).request() == request) {
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
  const CarModel c(base, car);
  if (i == 0 && int32_t(c.extra_image()) == 1) {
    return false;
  }
  if (set == FileLoadedSet::kA) {
    const uint32_t buffer =
        schema::LoadSlot(base, c.xpr_slots_address() + 24 * uint32_t(i)).buffer();
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
  const CarModel c(base, car);
  const uint32_t f = stack - kFileLoadedFrameBytes;
  const auto name = [&] { return Load(base, (c.block() << 2) + G::car_names_address()); };
  Require(Print(base, f, kTwoStrings, name(), variant.suffix, 0, false), "sprintf");
  const int texture_set = TextureSet(base, request, car);
  if (texture_set >= 0) {
    const uint32_t i = uint32_t(texture_set);
    const schema::LoadSlot slot(base, c.xpr_slots_address() + 24 * i);
    const uint32_t xpr = c.xpr_objects_address() + 24 * i;
    const uint32_t file = set == FileLoadedSet::kA ? slot.buffer() : buffer;
    if (!models::XprCreate(base, xpr, file)) {
      calls.xpr_create(xpr, file);
    }
    calls.provider_set(c.texture_providers_address() + 20 * i, xpr);
    if (set == FileLoadedSet::kA) {
      slot.set_state(3);
      Require(FreePagesAt(base, slot.buffer_address()).has_value(), "the buffer's release");
      slot.set_buffer(0);
    } else {
      slot.set_buffer(0);
      slot.set_state(3);
    }
    if (i != 0) {
      return;
    }
    // (+3724 isn't 1: FileLoadedNative.)
    Require(native::Buffers_Memcpy(base, f + 80, c.plate_w8_w11_address(), 16,
                                   variant.plate_memcpy),
            "memcpy");
    const uint64_t regs[4] = {c.plate_w0_w1(), c.plate_w2_w3(), c.plate_w4_w5(), c.plate_w6_w7()};
    Require(MakePlate(base, c.plate_texture_a_address(), c.plate_texture_a_data_address(),
                      c.plate_texture_b_address(), c.plate_texture_b_data_address(), regs, f,
                      calls.plate_release)
                .has_value(),
            "MakePlate");
    Store(base, c.xpr_indices_address(),
          uint32_t(scene_loaders::XprFindByName(base, c.xpr_objects_address(), f + 384)));
    for (int32_t k = 1; k < 6; k++) {
      Require(Print(base, f, kNumbered, name(), variant.numbered_suffix, k, true), "sprintf");
      Store(base, c.xpr_indices_address() + 4 * uint32_t(k),
            uint32_t(scene_loaders::XprFindByName(base, c.xpr_objects_address(), f + 384)));
    }
    return;
  }
  if (c.node_file_request() != request) {
    return;
  }
  // The node file: the skeleton bound and posed.
  const uint32_t skeleton = c.skeleton_address();
  const uint32_t node_file = c.node_file_buffer();
  if (!model_animation::SkeletonBind(base, skeleton, node_file, 0)) {
    calls.skeleton_bind(skeleton, node_file);
  }
  model_animation::SkeletonResetPose(base, skeleton);
  model_animation::SkeletonLocalMatrices(base, skeleton, f);
  if (model_animation::SkeletonUpdateMatrices(base, skeleton) ==
      model_animation::MatricesResult::kOriginal) {
    calls.update_matrices(skeleton);
  }
  c.set_node_file_state(3);
  int32_t found = 0;
  int32_t nodes = int32_t(model_animation::SkeletonNodeCount(base, skeleton));
  if (nodes > 0) {
    uint32_t record = 0, matrix = 0;
    for (; nodes != 0; nodes--, record += 160, matrix += 64) {
      const uint32_t node_name = c.skeleton_nodes<schema::View>().address + record;
      for (int k = 0; k < 6; k++) {
        if (SameName(base, kNodeNames[k], node_name)) {
          const uint32_t to = NodeMatrix(c, k);
          const uint32_t from = c.skeleton_node_matrices<schema::View>().address + matrix;
          matrix_math::Copy(base, to, from);
          interpolation::BlendState_OnCopy(base, to, from, variant.copies[k]);
          found++;
        }
      }
      if (SameName(base, kNodeNames[6], node_name)) {
        found++;
        c.set_accessory_matrix(c.skeleton_node_matrices<schema::View>().address + matrix);
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
  CopyWord(base, G::quarter_turn_address(), f + 144);
  matrix_math::RotateY(base, turn, turn, f + 144, f - 80);
  // The four tires' positions (their matrices' rows 3), w = 1.0, scaled by
  // 0.1 and turned; then w = 0.0.
  const double tenth = Lfs(base, G::float_tenth_address());
  const auto scale = [&](uint32_t at) { Stfs(base, at, Fmuls(Lfs(base, at), tenth)); };
  const auto tire = [&](int k) { return c.tire_positions_address() + 16 * uint32_t(k); };
  for (int k = 0; k < 4; k++) {
    PositionRow(base, NodeMatrix(c, k) + 48, tire(k));
    if (k < 3) {
      CopyWord(base, kOne, tire(k) + 12);
    }
  }
  // (Every word read before it's written: the order of the original's
  // multiplies doesn't matter.)
  for (int k = 0; k < 4; k++) {
    for (uint32_t w = 0; w < 3; w++) {
      scale(tire(k) + 4 * w);
    }
  }
  CopyWord(base, kOne, tire(3) + 12);
  for (int k = 0; k < 4; k++) {
    matrix_math::Vec4Transform(base, tire(k), turn, tire(k));
  }
  for (int k = 0; k < 4; k++) {
    CopyWord(base, kZero, tire(k) + 12);
  }
  if (found == 7) {
    const uint32_t seat_l = c.seat_positions_address(), seat_r = seat_l + 16;
    PositionRow(base, NodeMatrix(c, 4) + 48, seat_l);
    CopyWord(base, kOne, seat_l + 12);
    PositionRow(base, NodeMatrix(c, 5) + 48, seat_r);
    for (const uint32_t at : {seat_l, seat_r}) {
      for (uint32_t w = 0; w < 3; w++) {
        scale(at + 4 * w);
      }
    }
    CopyWord(base, kOne, seat_r + 12);
    matrix_math::Vec4Transform(base, seat_l, turn, seat_l);
    matrix_math::Vec4Transform(base, seat_r, turn, seat_r);
    CopyWord(base, kZero, seat_l + 12);
    CopyWord(base, kZero, seat_r + 12);
  }
  const uint32_t accessory = c.accessory_matrix<schema::View>().address;
  if (accessory != 0) {
    const uint32_t position = c.accessory_position_address();
    Move8(base, accessory + 48, position);
    Move8(base, accessory + 56, position + 8);
    for (uint32_t w = 0; w < 3; w++) {
      scale(position + 4 * w);
    }
    CopyWord(base, kOne, position + 12);
    matrix_math::Vec4Transform(base, position, turn, position);
  }
}

}  // namespace car_models
