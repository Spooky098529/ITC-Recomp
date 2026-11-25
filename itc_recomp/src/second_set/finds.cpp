// The parts' and the cockpit's point set-ups (see finds.h), call for call as
// the recompiled functions.

#include "second_set/finds.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "material_passes/material_passes.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"
#include "scene/scene.h"
#include "scene_loaders/model_point.h"
#include "second_set/points.h"
#include "schema/second_set.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/model_animation.h"

#include <cstdlib>

namespace second_set {

namespace {

using car_physics::ops::CopyWord;
using car_physics::ops::kOne;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

using G = schema::SecondSetGlobals;
using Record = schema::SecondSetPointRecord;

constexpr uint32_t kFive = G::five_address();    // 5.0
constexpr uint32_t kThree = G::three_address();  // 3.0
constexpr uint32_t R = kPointRecordBytes;

// ModelPoint::Find, `sp` the r1 passed on.
uint32_t NodeFind(uint32_t record, uint32_t model, uint32_t sp, FindCalls& calls) {
  const uint32_t r3 = scene_loaders::ModelPointFind(record, model, sp, [&](uint32_t skeleton) {
        calls.update_matrices(skeleton, sp - scene_loaders::kModelPointFrameBytes);
      });
  calls.flush = r3 == 1;
  return r3;
}

// PartModel::ScaleBlended(record +4, record +228, the float at `factor`).
uint32_t Scale(uint32_t record, uint32_t factor, uint32_t sp, FindCalls& calls) {
  const double f = double(guest::LoadFloat(factor));
  const Record r(record);
  const uint32_t r3 = material_passes::ScaleBlended(r.model(), r.shape_index(), f, sp);
  calls.flush = false;
  return r3;
}

// Records [from, to) of `group`: ModelPoint::Find below `nodes`, FindShape
// from there.
uint32_t FindRecords(uint32_t group, uint32_t model, uint32_t from, uint32_t to,
                     uint32_t nodes, uint32_t sp, FindCalls& calls) {
  uint32_t r3 = 0;
  for (uint32_t i = from; i < to; i++) {
    r3 = i < nodes ? NodeFind(group + R * i, model, sp, calls)
                   : ModelPointFindShape(group + R * i, model, sp);
  }
  return r3;
}

// Records [from, to] scaled by the float at `factor`.
uint32_t ScaleRecords(uint32_t group, uint32_t from, uint32_t to, uint32_t factor,
                      uint32_t sp, FindCalls& calls) {
  uint32_t r3 = 0;
  for (uint32_t i = from; i <= to; i++) {
    r3 = Scale(group + R * i, factor, sp, calls);
  }
  return r3;
}

}  // namespace

uint32_t PartsFindFrontBumper(uint32_t group, uint32_t model, uint32_t stack,
                              FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE140 PartsPoints::FindFrontBumper");
  FindRecords(group, model, 0, 11, 0, stack, calls);
  const uint32_t r3 = ScaleRecords(group, 3, 5, kFive, stack, calls);
  const schema::SecondSetPlateModel found(Record(group + 8 * R).model());
  if (found) {
    found.set_texture(schema::PartsPoints(group).front_plate_texture());
  }
  return r3;
}

uint32_t PartsFindRearBumper(uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE328 PartsPoints::FindRearBumper");
  FindRecords(group, model, 0, 7, 0, stack, calls);
  Scale(group + R, kOne, stack, calls);
  const uint32_t r3 = ScaleRecords(group, 2, 4, kFive, stack, calls);
  const schema::SecondSetPlateModel found(Record(group + 6 * R).model());
  if (found) {
    found.set_texture(schema::PartsPoints(group).rear_plate_texture());
  }
  return r3;
}

uint32_t PartsFindRearSpoiler(uint32_t group, uint32_t model, uint32_t stack,
                              FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE4E8 PartsPoints::FindRearSpoiler");
  FindRecords(group, model, 0, 3, 0, stack, calls);
  Scale(group + 2 * R, kFive, stack, calls);
  return Scale(group + R, kFive, stack, calls);
}

uint32_t PartsFindBonnet(uint32_t group, uint32_t model, uint32_t stack,
                         FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE5B0 PartsPoints::FindBonnet");
  return FindRecords(group, model, 0, 2, 1, stack, calls);
}

uint32_t PartsFindLeftDoor(uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE668 PartsPoints::FindLeftDoor");
  FindRecords(group, model, 0, 25, 2, stack, calls);
  return ScaleRecords(group, 7, 10, kFive, stack, calls);
}

uint32_t PartsFindRightDoor(uint32_t group, uint32_t model, uint32_t stack,
                            FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DE878 PartsPoints::FindRightDoor");
  return FindRecords(group, model, 0, 25, 2, stack, calls);
}

uint32_t PartsFindDoors(uint32_t group, uint32_t model, uint32_t stack,
                        FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DEA20 PartsPoints::FindDoors");
  FindRecords(group, model, 0, 23, 0, stack, calls);
  return ScaleRecords(group, 3, 10, kFive, stack, calls);
}

uint32_t PartsFindHeadlights(uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DF7F0 PartsPoints::FindHeadlights");
  const uint32_t r3 = FindRecords(group, model, 0, 13, 7, stack, calls);
  const Record first(group);
  if (first.model() == 0) {
    return r3;
  }
  const uint32_t entries = schema::Skeleton(first.skeleton()).node_states();
  CopyWord(first.node_index() * 144 + entries + 16,
           schema::PartsPoints(group).headlights_popup_max_address());
  calls.flush = false;
  return ScaleRecords(group, 7, 10, kFive, stack, calls);
}

uint32_t PartsFindTailLights(uint32_t group, uint32_t model, uint32_t stack,
                             FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DEC58 PartsPoints::FindTailLights");
  FindRecords(group, model, 0, 15, 8, stack, calls);
  return ScaleRecords(group, 8, 12, kFive, stack, calls);
}

uint32_t PartsFindFenders(uint32_t group, uint32_t model, uint32_t stack,
                          FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DED20 PartsPoints::FindFenders");
  FindRecords(group, model, 0, 7, 0, stack, calls);
  return ScaleRecords(group, 3, 4, kFive, stack, calls);
}

uint32_t PartsFindCabin(uint32_t group, uint32_t model, uint32_t stack,
                        FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DEED8 PartsPoints::FindCabin");
  FindRecords(group, model, 0, 5, 0, stack, calls);
  return ScaleRecords(group, 0, 1, kFive, stack, calls);
}

uint32_t PartsFindMufflers(uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E0080 PartsPoints::FindMufflers");
  const uint32_t sp = stack - 192;
  const uint32_t records = group + 256;
  for (uint32_t i = 0; i < 4; i++) {
    const Record record(records + R * i);
    NodeFind(record.address, model, sp, calls);
    if (!native::Buffers_Memcpy(sp + 80, record.matrix_address(), 64, 0x820E00C0)) {
      ITC_LOG_ERROR("Second set: FindMufflers' memcpy refused after its checks");
      std::abort();
    }
    const uint32_t matrix = group + 64 * i;
    matrix_math::Copy(matrix, sp + 80);
    interpolation::BlendState_OnCopy(matrix, sp + 80, 0x820E00CC);
    calls.flush = false;
  }
  return ModelPointFindShape(records + 4 * R, model, sp);
}

uint32_t PartsFind(uint32_t block, uint32_t models, uint32_t stack,
                   FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E0258 PartsPoints::Find");
  const schema::PartsPoints b(block);
  const uint32_t w37076 = b.car_record();
  const schema::CarModel car_loads(b.car_model());
  const schema::CarScene scene = b.scene();
  b.set_front_car_record(w37076);
  b.set_front_plate_texture(car_loads.linked_car_model().plate_texture_a());
  b.set_block_rear_plate_texture(car_loads.linked_car_model().plate_texture_b());
  if (scene.model_set() == 0) {
    b.set_block_left_door_car_record(w37076);
    b.set_block_right_door_car_record(w37076);
    const uint32_t w37080 = b.parts_record().address;
    const uint32_t w37092 = b.skeleton();
    b.set_block_left_door_parts_record(w37080);
    b.set_block_left_door_skeleton(w37092);
    b.set_block_right_door_parts_record(w37080);
    b.set_block_right_door_skeleton(w37092);
  } else {
    const uint32_t w37080 = b.parts_record().address;
    b.set_block_doors_parts_record(w37080);
    // The shadows' group's (+36096) +960.
    Store(block + 37056, w37080);
  }
  uint32_t r3 = block;
  auto shapes = [&](uint32_t at, uint32_t count, uint32_t model) {
    for (uint32_t i = 0; i < count; i++) {
      r3 = ModelPointFindShape(block + at + R * i, model, stack);
    }
  };
  for (uint32_t kind = 0; kind < 17; kind++) {
    const uint32_t model = Load(models + 4 * kind);
    if (model == 0) {
      continue;
    }
    const bool set_a = b.scene().model_set() == 0;
    switch (kind) {
      case 0: r3 = PartsFindFrontBumper(block, model, stack, calls); break;
      case 1: shapes(2656, 1, model); break;
      case 2: r3 = PartsFindRearBumper(block + 2896, model, stack, calls); break;
      case 3: shapes(4592, 1, model); break;
      case 4: r3 = PartsFindRearSpoiler(block + 4832, model, stack, calls); break;
      case 5: r3 = PartsFindBonnet(block + 5552, model, stack, calls); break;
      case 6:
        r3 = set_a ? PartsFindLeftDoor(block + 6032, model, stack, calls)
                   : PartsFindDoors(block + 18224, model, stack, calls);
        break;
      case 7:
        if (set_a) {
          r3 = PartsFindRightDoor(block + 12128, model, stack, calls);
        }
        break;
      case 8: shapes(set_a ? 23760 : 24240, 1, model); break;
      case 9:
        if (set_a) {
          shapes(24000, 1, model);
        }
        break;
      case 10: r3 = PartsFindHeadlights(block + 24480, model, stack, calls); break;
      case 11: r3 = PartsFindTailLights(block + 27680, model, stack, calls); break;
      case 12: r3 = PartsFindFenders(block + 31280, model, stack, calls); break;
      case 13: r3 = PartsFindCabin(block + 32960, model, stack, calls); break;
      case 14: shapes(34160, 2, model); break;
      case 15: r3 = PartsFindMufflers(block + 34640, model, stack, calls); break;
      case 16:
        if (!set_a) {
          shapes(36096, 4, model);
        }
        break;
      default: break;
    }
  }
  return r3;
}

uint32_t CockpitFindHeadlights(uint32_t group, uint32_t model, uint32_t stack,
                               FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DBB08 CockpitPoints::FindHeadlights");
  FindRecords(group, model, 0, 8, 4, stack, calls);
  return ScaleRecords(group, 5, 6, kOne, stack, calls);
}

uint32_t CockpitFind(uint32_t block, uint32_t models, uint32_t stack,
                     FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DBC88 CockpitPoints::Find");
  const schema::SecondSetCockpitPoints b(block);
  auto shapes = [&](uint32_t at, uint32_t count, uint32_t model) {
    for (uint32_t i = 0; i < count; i++) {
      ModelPointFindShape(at + R * i, model, stack);
    }
  };
  shapes(b.fenders_address(), 3, Load(models + 16));
  shapes(b.cabin_address(), 2, Load(models + 20));
  ModelPointFindShape(b.bonnet_address(), Load(models + 0), stack);
  shapes(b.doors_address(), 2, Load(models + 4));
  ModelPointFindShape(b.mirror_address(), Load(models + 8), stack);
  return CockpitFindHeadlights(b.headlights_address(), Load(models + 12), stack, calls);
}

uint32_t InteriorFindDials(uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E30C0 InteriorPoints::FindDials");
  const schema::SecondSetInteriorPoints g(group);
  const uint32_t records = g.group_records_address();
  FindRecords(records, model, 0, 12, 2, stack, calls);
  if (material_passes::IsBlockSetB(int32_t(g.group_word_4())) == 0 &&
      material_passes::IsBlockSetA(int32_t(g.group_word_4())) == 0) {
    return 0;
  }
  return ScaleRecords(records, 9, 11, kOne, stack, calls);
}

uint32_t InteriorFindMeters(uint32_t group, uint32_t model, uint32_t stack,
                            FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E3298 InteriorPoints::FindMeters");
  return FindRecords(schema::SecondSetInteriorPoints(group).group_records_address(),
                     model, 0, 22, 7, stack, calls);
}

uint32_t InteriorFindSeats(uint32_t group, uint32_t model, uint32_t stack,
                           FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E34A8 InteriorPoints::FindSeats");
  return FindRecords(schema::SecondSetInteriorPoints(group).group_records_address(),
                     model, 0, 4, 2, stack, calls);
}

uint32_t InteriorFind(uint32_t block, uint32_t models, uint32_t stack,
                      FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E3660 InteriorPoints::Find");
  const schema::SecondSetInteriorPoints b(block);
  // A group's records at its +16.
  auto records = [&](uint32_t group) {
    return schema::SecondSetInteriorPoints(group).group_records_address();
  };
  const schema::InteriorRecord record = b.record();
  const uint32_t w11440 = b.seat_position_source();
  b.set_dials_record(record);
  b.set_seat_position(w11440);
  b.set_meters_record(record);
  b.set_steering_record(record);
  b.set_seats_record(record);
  const uint32_t index = record.seat_model();
  if (index != 0) {
    b.set_seat_model(index * 464 + G::scene<schema::View>().address + 784);
  }
  b.set_roll_cage_record(record);
  ModelPointFindShape(b.body_address(), Load(models + 8), stack);
  InteriorFindDials(b.dials_address(), Load(models + 12), stack, calls);
  InteriorFindMeters(b.meters_address(), Load(models + 0), stack, calls);
  const uint32_t steering = Load(models + 20);
  for (uint32_t i = 0; i < 4; i++) {
    ModelPointFindShape(records(b.steering_address()) + R * i, steering, stack);
  }
  InteriorFindSeats(b.seats_address(), Load(models + 16), stack, calls);
  const uint32_t cage = Load(models + 4);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) {
    r3 = ModelPointFindShape(records(b.roll_cage_address()) + R * i, cage, stack);
  }
  return r3;
}

uint32_t LightsFindGroup(uint32_t group, uint32_t model, uint32_t stack,
                         FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DC850 LightPoints::FindGroup");
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 18; i++) {
    const uint32_t record = group + 16 + R * i;
    ModelPointFindShape(record, model, stack);
    r3 = Scale(record, kOne, stack, calls);
  }
  return r3;
}

uint32_t LightsFind(uint32_t block, uint32_t model, uint32_t stack,
                    FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DC900 LightPoints::Find");
  LightsFindGroup(block + 48, model, stack, calls);
  LightsFindGroup(block + 4384, model, stack, calls);
  return LightsFindGroup(block + 8720, model, stack, calls);
}

uint32_t TrafficFindBody(uint32_t group, uint32_t m0, uint32_t m1, uint32_t m2,
                         uint32_t m3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5538 TrafficPoints::FindBody");
  const uint32_t models[6] = {m3, m0, m2, m2, m0, m1};
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 6; i++) {
    r3 = ModelPointFindShape(group + R * i, models[i], stack);
  }
  return r3;
}

uint32_t TrafficFindHeadlights(uint32_t group, uint32_t model, uint32_t stack,
                               FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E5738 TrafficPoints::FindHeadlights");
  FindRecords(group, model, 0, 7, 4, stack, calls);
  return Scale(group + 5 * R, kThree, stack, calls);
}

uint32_t TrafficFindTailLights(uint32_t group, uint32_t model, uint32_t stack,
                               FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E5808 TrafficPoints::FindTailLights");
  FindRecords(group, model, 0, 12, 8, stack, calls);
  return ScaleRecords(group, 9, 10, kThree, stack, calls);
}

uint32_t TrafficFind(uint32_t block, uint32_t models, uint32_t stack,
                     FindCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820E58C8 TrafficPoints::Find");
  TrafficFindBody(block + 16, Load(models + 0), Load(models + 4),
                  Load(models + 8), Load(models + 12), stack);
  TrafficFindHeadlights(block + 1472, Load(models + 16), stack, calls);
  return TrafficFindTailLights(block + 3152, Load(models + 20), stack, calls);
}

}  // namespace second_set
