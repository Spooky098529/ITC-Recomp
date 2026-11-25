// The second set's point blocks' constructors and resets (see blocks.h),
// call for call as the recompiled functions.

#include "second_set/blocks.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "scene/scene.h"
#include "second_set/point_groups.h"
#include "second_set/points.h"
#include "schema/car_draw.h"
#include "schema/second_set.h"

namespace second_set {

namespace {

using car_physics::ops::CopyWord;
using car_physics::ops::kOne;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

constexpr uint32_t R = kPointRecordBytes;

// `count` records from `at` constructed, `sp` the r1 passed on.
void Construct(uint32_t at, uint32_t count, uint32_t sp) {
  for (uint32_t i = 0; i < count; i++) {
    PointRecordConstruct(at + R * i, sp);
  }
}

// A single record cleared and named from the word at `name_word`.
uint32_t ClearAndName(uint32_t record, uint32_t name_word, uint32_t sp) {
  PointRecordClear(record, sp);
  PointRecordSetShape(record, Load(name_word));
  return record;
}

// The w's (+12) of the four rows of the matrix at `matrix` = 1.0.
void MatrixWs(uint32_t matrix) {
  for (uint32_t row = 0; row < 4; row++) {
    CopyWord(kOne, matrix + 12 + 16 * row);
  }
}

}  // namespace

uint32_t PartsInitLeftDoor(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF5B8 PartsPoints::InitLeftDoor");
  Construct(group, 25, stack);
  MatrixWs(schema::PartsPoints(group).door_matrix_address());
  PartsResetLeftDoor(group, stack);
  return group;
}

uint32_t PartsInitRightDoor(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF610 PartsPoints::InitRightDoor");
  Construct(group, 25, stack);
  MatrixWs(schema::PartsPoints(group).door_matrix_address());
  PartsResetRightDoor(group, stack);
  return group;
}

uint32_t PartsInitHeadlights(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E1388 PartsPoints::InitHeadlights");
  Construct(group, 13, stack);
  MatrixWs(schema::PartsPoints(group).headlights_popup_matrix_address());
  PartsResetHeadlights(group, stack);
  return group;
}

uint32_t PartsInitMufflers(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E0010 PartsPoints::InitMufflers");
  for (uint32_t m = 0; m < 4; m++) {
    MatrixWs(group + 64 * m);
  }
  Construct(group + 256, 5, stack);
  PartsResetMufflers(group, stack);
  return group;
}

uint32_t PartsPointsConstruct(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E2A10 PartsPoints::PartsPoints");
  Construct(block + 0, 11, stack);
  PartsResetFrontBumper(block + 0, stack);
  PartsInitCooling(block + 2656, stack);
  Construct(block + 2896, 7, stack);
  PartsResetRearBumper(block + 2896, stack);
  PartsInitSideSkirts(block + 4592, stack);
  Construct(block + 4832, 3, stack);
  PartsResetRearSpoiler(block + 4832, stack);
  Construct(block + 5552, 2, stack);
  PartsResetBonnet(block + 5552, stack);
  PartsInitLeftDoor(block + 6032, stack);
  PartsInitRightDoor(block + 12128, stack);
  Construct(block + 18224, 23, stack);
  PartsResetDoors(block + 18224, stack);
  PartsInitLeftMirror(block + 23760, stack);
  PartsInitRightMirror(block + 24000, stack);
  PartsInitMirror(block + 24240, stack);
  PartsInitHeadlights(block + 24480, stack);
  Construct(block + 27680, 15, stack);
  PartsResetTailLights(block + 27680, stack);
  Construct(block + 31280, 7, stack);
  PartsResetFenders(block + 31280, stack);
  Construct(block + 32960, 5, stack);
  PartsResetCabin(block + 32960, stack);
  Construct(block + 34160, 2, stack);
  PartsResetEngine(block + 34160, stack);
  PartsInitMufflers(block + 34640, stack);
  Construct(block + 36096, 4, stack);
  PartsResetShadows(block + 36096, stack);
  return block;
}

uint32_t PartsPointsReset(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E00F0 PartsPoints::Reset");
  const schema::PartsPoints b(block);
  b.set_scene(schema::SecondSetGlobals::scene<schema::View>());
  b.set_car_record(0u);
  b.set_skeleton(0u);
  PartsResetFrontBumper(block + 0, stack);
  ClearAndName(block + 2656, 0x82602620, stack);
  PartsResetRearBumper(block + 2896, stack);
  ClearAndName(block + 4592, 0x82602640, stack);
  PartsResetRearSpoiler(block + 4832, stack);
  PartsResetBonnet(block + 5552, stack);
  PartsResetLeftDoor(block + 6032, stack);
  PartsResetRightDoor(block + 12128, stack);
  PartsResetDoors(block + 18224, stack);
  ClearAndName(block + 23760, 0x826026BC, stack);
  ClearAndName(block + 24000, 0x82602724, stack);
  ClearAndName(block + 24240, 0x82602784, stack);
  PartsResetHeadlights(block + 24480, stack);
  PartsResetTailLights(block + 27680, stack);
  PartsResetFenders(block + 31280, stack);
  PartsResetCabin(block + 32960, stack);
  PartsResetEngine(block + 34160, stack);
  PartsResetMufflers(block + 34640, stack);
  return PartsResetShadows(block + 36096, stack);
}

uint32_t CockpitPointsConstruct(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC190 CockpitPoints::CockpitPoints");
  const schema::SecondSetCockpitPoints b(block);
  Construct(b.fenders_address(), 3, stack);
  CockpitResetFenders(b.fenders_address(), stack);
  Construct(b.cabin_address(), 2, stack);
  CockpitResetCabin(b.cabin_address(), stack);
  CockpitInitBonnet(b.bonnet_address(), stack);
  Construct(b.doors_address(), 2, stack);
  CockpitResetDoors(b.doors_address(), stack);
  CockpitInitMirror(b.mirror_address(), stack);
  Construct(b.headlights_address(), 8, stack);
  CockpitResetHeadlights(b.headlights_address(), stack);
  return block;
}

uint32_t CockpitPointsReset(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DBC10 CockpitPoints::Reset");
  const schema::SecondSetCockpitPoints b(block);
  CockpitResetFenders(b.fenders_address(), stack);
  CockpitResetCabin(b.cabin_address(), stack);
  ClearAndName(b.bonnet_address(), 0x8200524C, stack);
  CockpitResetDoors(b.doors_address(), stack);
  ClearAndName(b.mirror_address(), 0x82005258, stack);
  const uint32_t r3 = CockpitResetHeadlights(b.headlights_address(), stack);
  b.set_word_4080(0);
  return r3;
}

uint32_t InteriorPointsConstruct(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E4FD8 InteriorPoints::InteriorPoints");
  const schema::SecondSetInteriorPoints b(block);
  // A group's records at its +16.
  auto records = [&](uint32_t group) {
    return schema::SecondSetInteriorPoints(group).group_records_address();
  };
  InteriorInitBody(b.body_address(), stack);
  Construct(records(b.dials_address()), 12, stack);
  InteriorResetDials(b.dials_address(), stack);
  Construct(records(b.meters_address()), 22, stack);
  InteriorResetMeters(b.meters_address(), stack);
  Construct(records(b.steering_address()), 4, stack);
  InteriorResetSteering(b.steering_address(), stack);
  Construct(records(b.seats_address()), 4, stack);
  InteriorResetSeats(b.seats_address(), stack);
  Construct(records(b.roll_cage_address()), 4, stack);
  InteriorResetRollCage(b.roll_cage_address(), stack);
  return block;
}

uint32_t InteriorPointsReset(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E35D8 InteriorPoints::Reset");
  const schema::SecondSetInteriorPoints b(block);
  b.set_seat_position_source(0);
  b.set_record(0u);
  b.set_skeleton(0);
  b.set_cockpit(0);
  b.set_accessory_shown(1);
  ClearAndName(b.body_address(), 0x82602864, stack);
  InteriorResetDials(b.dials_address(), stack);
  InteriorResetMeters(b.meters_address(), stack);
  InteriorResetSteering(b.steering_address(), stack);
  InteriorResetSeats(b.seats_address(), stack);
  return InteriorResetRollCage(b.roll_cage_address(), stack);
}

uint32_t LightPointsConstruct(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DDCF8 LightPoints::LightPoints");
  Construct(block + 48 + 16, 18, stack);
  LightsResetFront(block + 48, stack);
  Construct(block + 4384 + 16, 18, stack);
  LightsResetSides(block + 4384, stack);
  Construct(block + 8720 + 16, 18, stack);
  LightsResetRear(block + 8720, stack);
  return block;
}

uint32_t LightPointsReset(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC8A8 LightPoints::Reset");
  LightsResetFront(block + 48, stack);
  LightsResetSides(block + 4384, stack);
  const uint32_t r3 = LightsResetRear(block + 8720, stack);
  for (uint32_t at = 8; at < 40; at += 4) {
    Store(block + at, 0);
  }
  return r3;
}

uint32_t TrafficPointsConstruct(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E68E8 TrafficPoints::TrafficPoints");
  Construct(block + 16, 6, stack);
  TrafficResetBody(block + 16, stack);
  Construct(block + 1472, 7, stack);
  TrafficResetHeadlights(block + 1472, stack);
  Construct(block + 3152, 12, stack);
  TrafficResetTailLights(block + 3152, stack);
  return block;
}

uint32_t TrafficPointsReset(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5880 TrafficPoints::Reset");
  schema::TrafficPoints(block).set_word_0(0u);
  TrafficResetBody(block + 16, stack);
  TrafficResetHeadlights(block + 1472, stack);
  return TrafficResetTailLights(block + 3152, stack);
}

namespace {

// A wheels group's init: `count` records at `records` constructed, the w's
// of the two rows at `rows` = 1.0, then `reset`.
uint32_t InitWheelsGroup(uint32_t group, uint32_t records, uint32_t count,
                         uint32_t rows, uint32_t stack,
                         uint32_t (*reset)(uint32_t, uint32_t)) {
  Construct(records, count, stack);
  CopyWord(kOne, rows + 12);
  CopyWord(kOne, rows + 28);
  reset(group, stack);
  return group;
}

}  // namespace

uint32_t WheelsInitTire(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E8A80 WheelPoints::InitTire");
  const schema::SecondSetWheelPoints g(group);
  return InitWheelsGroup(group, g.tire_records_address(), 6, g.tire_rows_address(), stack,
                         WheelsResetTire);
}

uint32_t WheelsInitRimA(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E99C0 WheelPoints::InitRimA");
  const schema::SecondSetWheelPoints g(group);
  return InitWheelsGroup(group, g.group_records_address(), 7, g.rim_rows_address(), stack,
                         WheelsResetRimA);
}

uint32_t WheelsInitRimB(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EA8E8 WheelPoints::InitRimB");
  const schema::SecondSetWheelPoints g(group);
  return InitWheelsGroup(group, g.group_records_address(), 7, g.rim_rows_address(), stack,
                         WheelsResetRimB);
}

uint32_t WheelsInitBrakes(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EB320 WheelPoints::InitBrakes");
  const schema::SecondSetWheelPoints g(group);
  return InitWheelsGroup(group, g.group_records_address(), 12, g.brake_rows_address(), stack,
                         WheelsResetBrakes);
}

uint32_t WheelPointsConstruct(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ECC48 WheelPoints::WheelPoints");
  const schema::SecondSetWheelPoints b(block);
  WheelsInitTire(b.front_tire_address(), stack);
  WheelsInitTire(b.rear_tire_address(), stack);
  WheelsInitRimA(b.rim_a_front_address(), stack);
  WheelsInitRimA(b.rim_a_rear_address(), stack);
  WheelsInitRimB(b.rim_b_front_address(), stack);
  WheelsInitRimB(b.rim_b_rear_address(), stack);
  WheelsInitBrakes(b.brakes_front_address(), stack);
  WheelsInitBrakes(b.brakes_rear_address(), stack);
  return block;
}

uint32_t WheelPointsReset(uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E85C8 WheelPoints::Reset");
  const schema::SecondSetWheelPoints b(block);
  b.set_car_model(0);
  b.set_car_record(0u);
  b.set_wheels_record(0u);
  b.set_word_12(0);
  b.set_word_16(0);
  b.set_word_20(0);
  b.set_brake_model(0);
  WheelsResetTire(b.front_tire_address(), stack);
  WheelsResetTire(b.rear_tire_address(), stack);
  WheelsResetRimA(b.rim_a_front_address(), stack);
  WheelsResetRimA(b.rim_a_rear_address(), stack);
  WheelsResetRimB(b.rim_b_front_address(), stack);
  WheelsResetRimB(b.rim_b_rear_address(), stack);
  WheelsResetBrakes(b.brakes_front_address(), stack);
  const uint32_t r3 = WheelsResetBrakes(b.brakes_rear_address(), stack);
  b.set_rear_tire_front(0);
  b.set_rim_a_rear_front(0);
  b.set_rim_b_rear_front(0);
  b.set_brakes_rear_front(0);
  return r3;
}

uint32_t PartsPointsDestruct(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820E2C10 PartsPoints::~PartsPoints");
  return block;
}

uint32_t CockpitPointsDestruct(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820DC258 CockpitPoints::~CockpitPoints");
  return block;
}

uint32_t InteriorPointsDestruct(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820E50C0 InteriorPoints::~InteriorPoints");
  return block;
}

uint32_t LightPointsDestruct(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820DDD90 LightPoints::~LightPoints");
  return block + 64;
}

uint32_t WheelPointsDestruct(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820ECCB8 WheelPoints::~WheelPoints");
  return block + 400;
}

uint32_t TrafficPointsDestruct(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820E6980 TrafficPoints::~TrafficPoints");
  return block + 16;
}

}  // namespace second_set
