// The wheels' point set-up (see wheels.h), load for load and store for
// store as the recompiled functions.

#include "second_set/wheels.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "native/buffers.h"
#include "scene/scene.h"
#include "scene_loaders/pools.h"
#include "second_set/morph.h"
#include "second_set/points.h"
#include "schema/second_set.h"
#include "schema/scene_models.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"

namespace second_set {

namespace {

using namespace camera_modes::steps;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

using G = schema::SecondSetGlobals;
using Points = schema::SecondSetWheelPoints;
using Record = schema::SecondSetPointRecord;

constexpr uint32_t R = kPointRecordBytes;
constexpr uint32_t kTireTable = G::tire_table_address();    // 24 bytes an entry
constexpr uint32_t kIndexTable = G::index_table_address();  // 44 bytes an entry
constexpr uint32_t kOne = G::one_address();                 // 1.0
constexpr uint32_t kNinth = G::brake_step_address();        // 1/9

// The first set's car model (car-models' CarModel) behind a second set's
// car model (its +24): +32 its car record, +6976 its matrix.
schema::CarModel FirstSetCarModel(uint8_t* base, uint32_t car_model) {
  return schema::SceneCarModel(base, car_model).car_model<schema::CarModel>();
}

void Left(WheelCalls& calls, Flush flush) {
  if (flush != Flush::kUnchanged) {
    calls.flush = flush;
  }
}

// 32 bytes of the matrix at [group + 0] (its first half, or its second) to
// `to`, the matrix's word read again for the second 16.
void Rows(uint8_t* base, uint32_t group, uint32_t to, bool second) {
  const Points g(base, group);
  const uint32_t from = second ? 32 : 0;
  uint32_t m = g.matrix().address;
  Store64(base, to + 0, Load64(base, m + from + 0));
  Store64(base, to + 8, Load64(base, m + from + 8));
  m = g.matrix().address;
  Store64(base, to + 16, Load64(base, m + from + 16));
  Store64(base, to + 24, Load64(base, m + from + 24));
}

void Run(uint8_t* base, uint32_t model, uint32_t record, uint32_t out1, uint32_t out2,
         uint32_t sp, uint32_t lr, WheelCalls& calls) {
  if (const auto result = MorphRun(base, record, out1, out2, sp)) {
    Left(calls, result->flush);
    return;
  }
  Left(calls, calls.morph_run(model, record, out1, out2, sp, lr) ? Flush::kOn : Flush::kOff);
}

void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr,
          WheelCalls& calls) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
}

uint64_t Free(uint8_t* base, uint32_t at, uint32_t sp, uint32_t lr, WheelCalls& calls) {
  if (const auto r3 = scene::FreePagesIn(base, at)) {
    return *r3;
  }
  return calls.free_pages_in(at, sp, lr);
}

// The return addresses of one morph's calls, from its first (StreamSize).
struct Sites {
  uint32_t run, copy1, copy2, free1, free2;
};
constexpr Sites kRimSites{0x6C, 0x84, 0x9C, 0xA4, 0xB0};
constexpr Sites kBrakeSites{0x64, 0x7C, 0x94, 0x9C, 0xA8};

// The rims' and brakes' frame words: the target's index, the weight, the
// morph record.
struct MorphFrame {
  uint32_t weight, record;
};
constexpr MorphFrame kRimFrame{128, 160};
constexpr MorphFrame kBrakeFrame{144, 176};

// One of the rims' or brakes' morphs: record 2k's shape toward record 2k +
// 1's (one target, its index at the frame's +96) into the pages at the
// frame's +80 and +84, copied over the shape's streams (the second by the
// first stream's size where `second_by_first`), the pages freed (+80 then 0).
// `weight_from` the float the rims copy to the frame's weight each time (0
// for the brakes'). Returns the last free's r3.
uint64_t MorphPair(uint8_t* base, uint32_t model, uint32_t group, uint32_t k, uint32_t sp,
                   const MorphFrame& frame, uint32_t site, const Sites& sites,
                   uint32_t weight_from, bool second_by_first, WheelCalls& calls) {
  // Record 2k (the shape) and 2k + 1 (the target).
  const Record shape(base, Points(base, group).group_records_address() + 2 * R * k);
  const Record target(base, shape.address + R);
  const uint32_t size1 = ModelStreamSize(base, model, shape.shape_index());
  const uint32_t size2 = ModelStreamSize2(base, model, shape.shape_index());
  scene_loaders::AllocPages(base, sp + 80, size1);
  scene_loaders::AllocPages(base, sp + 84, size2);
  const uint32_t index = target.shape_index();
  if (weight_from != 0) {
    Stfs(base, sp + frame.weight, Lfs(base, weight_from));
    calls.flush = Flush::kOff;
  }
  Store(base, sp + 96, index);
  Left(calls, MorphSetUp(base, model, sp + frame.record, shape.shape_index(), sp + 96,
                         sp + frame.weight, 1));
  Run(base, model, sp + frame.record, Load(base, sp + 80), Load(base, sp + 84), sp,
      site + sites.run, calls);
  Copy(base, ModelStreamData(base, model, shape.shape_index()), Load(base, sp + 80), size1, sp,
       site + sites.copy1, calls);
  Copy(base, ModelStreamData2(base, model, shape.shape_index()), Load(base, sp + 84),
       second_by_first ? size1 : size2, sp, site + sites.copy2, calls);
  Free(base, sp + 80, sp, site + sites.free1, calls);
  Store(base, sp + 80, 0);
  return Free(base, sp + 84, sp, site + sites.free2, calls);
}

// FindShape on `count` records from `records`; the last one's r3.
uint32_t FindShapes(uint8_t* base, uint32_t records, uint32_t count, uint32_t model,
                    uint32_t sp) {
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < count; i++) {
    r3 = ModelPointFindShape(base, records + R * i, model, sp);
  }
  return r3;
}

uint64_t SetUpRim(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                  WheelCalls& calls, bool rim_a) {
  const uint32_t sp = stack - 336;
  const Points g(base, group);
  Store(base, sp, stack);
  uint64_t r3 = FindShapes(base, g.group_records_address(), 7, model, sp);
  Rows(base, group, g.rim_rows_address(), int32_t(g.word_12()) == 0);
  if (int32_t(g.wheels_record().morph_each_draw()) != 0) {
    return r3;
  }
  Store(base, sp + 80, 0);
  Store(base, sp + 84, 0);
  if (rim_a) {
    if (Record(base, g.group_records_address()).model() != 0) {
      MorphPair(base, model, group, 0, sp, kRimFrame, 0x820E7A8C, kRimSites, g.weight_address(),
                false, calls);
      Store(base, sp + 84, 0);
    }
    // The second morph's second copy by the first stream's size, as the
    // original does.
    return MorphPair(base, model, group, 1, sp, kRimFrame, 0x820E7B4C, kRimSites,
                     g.weight_address(), true, calls);
  }
  static constexpr uint32_t kSites[] = {0x820E7D58, 0x820E7E18, 0x820E7ED8};
  for (uint32_t k = 0; k < 3; k++) {
    r3 = MorphPair(base, model, group, k, sp, kRimFrame, kSites[k], kRimSites, g.weight_address(),
                   false, calls);
    if (k != 2) {
      Store(base, sp + 84, 0);
    }
  }
  return r3;
}

// The tires' and brakes' +4, +0 and +8 from the block: the car model's
// loading object's words when [+0] is set, else +4 and +8; +8 = +12.
void GroupWords(uint8_t* base, uint32_t block, uint32_t front, uint32_t rear) {
  const Points b(base, block);
  const Points f(base, front);
  const Points r(base, rear);
  const uint32_t loading = b.car_model();
  if (loading != 0) {
    f.set_car_record(FirstSetCarModel(base, loading).record_address());
    f.set_matrix(FirstSetCarModel(base, loading).tire_positions_address());
    r.set_car_record(FirstSetCarModel(base, loading).record_address());
    r.set_matrix(FirstSetCarModel(base, loading).tire_positions_address());
  } else {
    const uint32_t a = b.car_record().address;
    const uint32_t m = b.wheels_record().address;
    f.set_car_record(a);
    f.set_matrix(m);
    r.set_car_record(a);
    r.set_matrix(m);
  }
}

}  // namespace

uint64_t WheelsSetUpTire(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E7598 WheelPoints::SetUpTire");
  const uint32_t sp = stack - 288;
  const Points g(base, group);
  Store(base, sp, stack);
  FindShapes(base, g.tire_records_address(), 6, model, sp);
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, g.tire_shapes_address() + 4 * k,
          Record(base, g.tire_records_address() + R * (k + 1)).shape_index());
  }
  const bool front = int32_t(g.tire_front()) != 0;
  schema::SceneModelsWheelsRecord wheels = g.wheels_record();
  // The record's index, value and float (its +44 or +56 size), or the
  // table's by the car (+1080 on for traffic, kind 3).
  auto own_index = [&](const schema::SceneModelsWheelsRecord& w) {
    return front ? w.front_size_index() : w.rear_size_index();
  };
  uint32_t index, value;
  double f1;
  if (int32_t(own_index(wheels)) != -1) {
    wheels = g.wheels_record();
    value = front ? wheels.front_size_a_address() : wheels.rear_size_a_address();
    f1 = Lfs(base, front ? wheels.front_size_b_address() : wheels.rear_size_b_address());
    index = own_index(wheels);
  } else {
    const uint32_t car = g.car_record().block() * 24;
    const uint32_t at = kTireTable + (int32_t(wheels.kind()) == 3 ? 1080 : 0) + (front ? 0 : 12);
    index = Load(base, car + at);
    value = car + at + 4;
    f1 = Lfs(base, car + at + 8);
  }
  TireFillEntries(base, g.tire_entries_address(), index, value, f1, sp);
  Rows(base, group, g.tire_rows_address(), !front);
  // sub_820E7010's index: the record's, else the table's (+1980 for
  // traffic, else plus the record's +72 or +76).
  wheels = g.wheels_record();
  uint64_t read;
  const int32_t flag = int32_t(g.tire_front());
  if (int32_t(wheels.kind()) == 3) {
    if (flag != 0 && int32_t(wheels.front_size_index()) != -1) {
      read = wheels.front_size_index();
    } else if (flag == 0 && int32_t(wheels.rear_size_index()) != -1) {
      read = wheels.rear_size_index();
    } else {
      read = Load(base, g.car_record().block() * 44 + kIndexTable + 1980);
    }
  } else {
    if (int32_t(flag != 0 ? wheels.front_size_index() : wheels.rear_size_index()) != -1) {
      read = flag != 0 ? wheels.front_size_index() : wheels.rear_size_index();
    } else {
      const uint32_t add = flag != 0 ? wheels.front_index_add() : wheels.rear_index_add();
      read = uint64_t(Load(base, g.car_record().block() * 44 + kIndexTable)) + uint64_t(add);
    }
  }
  TireReadEntry(base, uint32_t(read), g.tire_entries_address(), g.tire_weights_address(),
                g.tire_out1_address(), g.tire_out2_address(), sp);
  calls.flush = Flush::kOff;
  if (int32_t(g.wheels_record().morph_each_draw()) != 0) {
    return read;
  }
  Store(base, sp + 84, 0);
  Store(base, sp + 80, 0);
  const uint32_t size1 = ModelStreamSize(base, model, g.tire_base_shape());
  const uint32_t size2 = ModelStreamSize2(base, model, g.tire_base_shape());
  scene_loaders::AllocPages(base, sp + 84, size1);
  scene_loaders::AllocPages(base, sp + 80, size2);
  Left(calls, MorphSetUp(base, model, sp + 96, g.tire_base_shape(), g.tire_shapes_address(),
                         g.tire_weights_address(), 5));
  Run(base, model, sp + 96, Load(base, sp + 84), Load(base, sp + 80), sp, 0x820E78D0, calls);
  Copy(base, ModelStreamData(base, model, g.tire_base_shape()), Load(base, sp + 84), size1, sp,
       0x820E78E8, calls);
  Copy(base, ModelStreamData2(base, model, g.tire_base_shape()), Load(base, sp + 80), size2, sp,
       0x820E7900, calls);
  Free(base, sp + 84, sp, 0x820E7908, calls);
  return Free(base, sp + 80, sp, 0x820E7910, calls);
}

uint64_t WheelsSetUpRimA(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E79C8 WheelPoints::SetUpRimA");
  return SetUpRim(base, group, model, stack, calls, true);
}

uint64_t WheelsSetUpRimB(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E7CA0 WheelPoints::SetUpRimB");
  return SetUpRim(base, group, model, stack, calls, false);
}

uint64_t WheelsSetUpBrakes(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E8028 WheelPoints::SetUpBrakes");
  const uint32_t sp = stack - 368;
  const Points g(base, group);
  Store(base, sp, stack);
  FindShapes(base, g.group_records_address(), 12, model, sp);
  const double one = Lfs(base, kOne);
  material_passes::ScaleBlended(base, g.brake_record10_model(), g.brake_record10_shape(), one, sp);
  material_passes::ScaleBlended(base, g.brake_record11_model(), g.brake_record11_shape(), one, sp);
  uint32_t count;
  if (int32_t(g.word_12()) != 0) {
    Rows(base, group, g.brake_rows_address(), false);
    count = g.wheels_record().front_brake_size();
  } else {
    Rows(base, group, g.brake_rows_address(), true);
    count = g.wheels_record().rear_brake_size();
  }
  uint64_t r3 =
      material_passes::SetTexture(base, model, g.wheels_record().caliper_texture(), -1, sp);
  // The original's epilogue turns the flush mode off either way.
  calls.flush = Flush::kOff;
  const schema::SceneModelsWheelsRecord wheels = g.wheels_record();
  if (int32_t(wheels.morph_each_draw()) != 0) {
    return r3;
  }
  Store(base, sp + 80, 0);
  Store(base, sp + 84, 0);
  const int32_t kind = int32_t(wheels.brake_kind());
  if (kind >= 3 && kind < 6) {
    count += 1;
  }
  if (kind == 6) {
    count += 2;
  }
  const int64_t steps = int64_t(int32_t(count - 9));
  Store64(base, sp + 128, uint64_t(steps));
  const double weight = Fmuls(Single(double(steps)), Lfs(base, kNinth));
  Stfs(base, sp + 144, weight);
  Stfs(base, g.weight_address(), weight);
  for (uint32_t k = 0; k < 6; k++) {
    r3 = MorphPair(base, model, group, k, sp, kBrakeFrame, 0x820E817C + 0xB8 * k, kBrakeSites, 0,
                   false, calls);
    if (k != 5) {
      Store(base, sp + 84, 0);
    }
  }
  calls.flush = Flush::kOff;
  return r3;
}

uint64_t WheelPointsSetUp(uint8_t* base, uint32_t block, uint32_t stack, WheelCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E8668 WheelPoints::SetUp");
  const uint32_t sp = stack - 112;
  const Points b(base, block);
  Store(base, sp, stack);
  GroupWords(base, block, b.front_tire_address(), b.rear_tire_address());
  b.set_front_tire_wheels_record(b.word_12());
  WheelsSetUpTire(base, b.front_tire_address(), b.word_16(), sp, calls);
  b.set_rear_tire_wheels_record(b.word_12());
  WheelsSetUpTire(base, b.rear_tire_address(), b.word_16() + 464, sp, calls);
  const double f0 = Lfs(base, b.front_tire_out1_address());
  const double f13 = Lfs(base, b.front_tire_out2_address());
  const double f12 = Lfs(base, b.rear_tire_out1_address());
  const double f11 = Lfs(base, b.rear_tire_out2_address());
  calls.flush = Flush::kOff;
  const bool set_a = int32_t(G::scene<schema::CarScene>(base).model_set()) == 0;
  const uint32_t loading = b.car_model();
  const Points front(base, set_a ? b.rim_a_front_address() : b.rim_b_front_address());
  const Points rear(base, set_a ? b.rim_a_rear_address() : b.rim_b_rear_address());
  Stfs(base, front.weight_address(), f0);
  Stfs(base, front.float_20_address(), f13);
  Stfs(base, rear.weight_address(), f12);
  Stfs(base, rear.float_20_address(), f11);
  uint32_t matrix;
  if (loading != 0) {
    front.set_matrix(FirstSetCarModel(base, loading).tire_positions_address());
    matrix = FirstSetCarModel(base, loading).tire_positions_address();
  } else {
    matrix = b.wheels_record().address;
    front.set_matrix(matrix);
  }
  rear.set_matrix(matrix);
  auto rim = set_a ? WheelsSetUpRimA : WheelsSetUpRimB;
  front.set_wheels_record(b.word_12());
  rim(base, front.address, b.word_20(), sp, calls);
  rear.set_wheels_record(b.word_12());
  rim(base, rear.address, b.word_20() + 464, sp, calls);
  GroupWords(base, block, b.brakes_front_address(), b.brakes_rear_address());
  b.set_brakes_front_wheels_record(b.word_12());
  WheelsSetUpBrakes(base, b.brakes_front_address(), b.brake_model(), sp, calls);
  b.set_brakes_rear_wheels_record(b.word_12());
  return WheelsSetUpBrakes(base, b.brakes_rear_address(), b.brake_model() + 464, sp, calls);
}

}  // namespace second_set
