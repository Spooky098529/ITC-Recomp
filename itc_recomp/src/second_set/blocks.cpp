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

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t R = kPointRecordBytes;

// `count` records from `at` constructed, `sp` the r1 passed on.
void Construct(uint8_t* base, uint32_t at, uint32_t count, uint32_t sp) {
  for (uint32_t i = 0; i < count; i++) {
    PointRecordConstruct(base, at + R * i, sp);
  }
}

// A single record cleared and named from the word at `name_word`.
uint32_t ClearAndName(uint8_t* base, uint32_t record, uint32_t name_word, uint32_t sp) {
  PointRecordClear(base, record, sp);
  PointRecordSetShape(base, record, Load(base, name_word));
  return record;
}

// The w's (+12) of the four rows of the matrix at `matrix` = 1.0.
void MatrixWs(uint8_t* base, uint32_t matrix) {
  for (uint32_t row = 0; row < 4; row++) {
    CopyWord(base, kOne, matrix + 12 + 16 * row);
  }
}

}  // namespace

uint32_t PartsInitLeftDoor(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF5B8 PartsPoints::InitLeftDoor");
  Construct(base, group, 25, stack);
  MatrixWs(base, schema::PartsPoints(base, group).door_matrix_address());
  PartsResetLeftDoor(base, group, stack);
  return group;
}

uint32_t PartsInitRightDoor(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF610 PartsPoints::InitRightDoor");
  Construct(base, group, 25, stack);
  MatrixWs(base, schema::PartsPoints(base, group).door_matrix_address());
  PartsResetRightDoor(base, group, stack);
  return group;
}

uint32_t PartsInitHeadlights(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E1388 PartsPoints::InitHeadlights");
  Construct(base, group, 13, stack);
  MatrixWs(base, schema::PartsPoints(base, group).headlights_popup_matrix_address());
  PartsResetHeadlights(base, group, stack);
  return group;
}

uint32_t PartsInitMufflers(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E0010 PartsPoints::InitMufflers");
  for (uint32_t m = 0; m < 4; m++) {
    MatrixWs(base, group + 64 * m);
  }
  Construct(base, group + 256, 5, stack);
  PartsResetMufflers(base, group, stack);
  return group;
}

uint32_t PartsPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E2A10 PartsPoints::PartsPoints");
  Construct(base, block + 0, 11, stack);
  PartsResetFrontBumper(base, block + 0, stack);
  PartsInitCooling(base, block + 2656, stack);
  Construct(base, block + 2896, 7, stack);
  PartsResetRearBumper(base, block + 2896, stack);
  PartsInitSideSkirts(base, block + 4592, stack);
  Construct(base, block + 4832, 3, stack);
  PartsResetRearSpoiler(base, block + 4832, stack);
  Construct(base, block + 5552, 2, stack);
  PartsResetBonnet(base, block + 5552, stack);
  PartsInitLeftDoor(base, block + 6032, stack);
  PartsInitRightDoor(base, block + 12128, stack);
  Construct(base, block + 18224, 23, stack);
  PartsResetDoors(base, block + 18224, stack);
  PartsInitLeftMirror(base, block + 23760, stack);
  PartsInitRightMirror(base, block + 24000, stack);
  PartsInitMirror(base, block + 24240, stack);
  PartsInitHeadlights(base, block + 24480, stack);
  Construct(base, block + 27680, 15, stack);
  PartsResetTailLights(base, block + 27680, stack);
  Construct(base, block + 31280, 7, stack);
  PartsResetFenders(base, block + 31280, stack);
  Construct(base, block + 32960, 5, stack);
  PartsResetCabin(base, block + 32960, stack);
  Construct(base, block + 34160, 2, stack);
  PartsResetEngine(base, block + 34160, stack);
  PartsInitMufflers(base, block + 34640, stack);
  Construct(base, block + 36096, 4, stack);
  PartsResetShadows(base, block + 36096, stack);
  return block;
}

uint32_t PartsPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E00F0 PartsPoints::Reset");
  const schema::PartsPoints b(base, block);
  b.set_scene(schema::SecondSetGlobals::scene<schema::View>(base));
  b.set_car_record(0u);
  b.set_skeleton(0u);
  PartsResetFrontBumper(base, block + 0, stack);
  ClearAndName(base, block + 2656, 0x82602620, stack);
  PartsResetRearBumper(base, block + 2896, stack);
  ClearAndName(base, block + 4592, 0x82602640, stack);
  PartsResetRearSpoiler(base, block + 4832, stack);
  PartsResetBonnet(base, block + 5552, stack);
  PartsResetLeftDoor(base, block + 6032, stack);
  PartsResetRightDoor(base, block + 12128, stack);
  PartsResetDoors(base, block + 18224, stack);
  ClearAndName(base, block + 23760, 0x826026BC, stack);
  ClearAndName(base, block + 24000, 0x82602724, stack);
  ClearAndName(base, block + 24240, 0x82602784, stack);
  PartsResetHeadlights(base, block + 24480, stack);
  PartsResetTailLights(base, block + 27680, stack);
  PartsResetFenders(base, block + 31280, stack);
  PartsResetCabin(base, block + 32960, stack);
  PartsResetEngine(base, block + 34160, stack);
  PartsResetMufflers(base, block + 34640, stack);
  return PartsResetShadows(base, block + 36096, stack);
}

uint32_t CockpitPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC190 CockpitPoints::CockpitPoints");
  const schema::SecondSetCockpitPoints b(base, block);
  Construct(base, b.fenders_address(), 3, stack);
  CockpitResetFenders(base, b.fenders_address(), stack);
  Construct(base, b.cabin_address(), 2, stack);
  CockpitResetCabin(base, b.cabin_address(), stack);
  CockpitInitBonnet(base, b.bonnet_address(), stack);
  Construct(base, b.doors_address(), 2, stack);
  CockpitResetDoors(base, b.doors_address(), stack);
  CockpitInitMirror(base, b.mirror_address(), stack);
  Construct(base, b.headlights_address(), 8, stack);
  CockpitResetHeadlights(base, b.headlights_address(), stack);
  return block;
}

uint32_t CockpitPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DBC10 CockpitPoints::Reset");
  const schema::SecondSetCockpitPoints b(base, block);
  CockpitResetFenders(base, b.fenders_address(), stack);
  CockpitResetCabin(base, b.cabin_address(), stack);
  ClearAndName(base, b.bonnet_address(), 0x8200524C, stack);
  CockpitResetDoors(base, b.doors_address(), stack);
  ClearAndName(base, b.mirror_address(), 0x82005258, stack);
  const uint32_t r3 = CockpitResetHeadlights(base, b.headlights_address(), stack);
  b.set_word_4080(0);
  return r3;
}

uint32_t InteriorPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E4FD8 InteriorPoints::InteriorPoints");
  const schema::SecondSetInteriorPoints b(base, block);
  // A group's records at its +16.
  auto records = [&](uint32_t group) {
    return schema::SecondSetInteriorPoints(base, group).group_records_address();
  };
  InteriorInitBody(base, b.body_address(), stack);
  Construct(base, records(b.dials_address()), 12, stack);
  InteriorResetDials(base, b.dials_address(), stack);
  Construct(base, records(b.meters_address()), 22, stack);
  InteriorResetMeters(base, b.meters_address(), stack);
  Construct(base, records(b.steering_address()), 4, stack);
  InteriorResetSteering(base, b.steering_address(), stack);
  Construct(base, records(b.seats_address()), 4, stack);
  InteriorResetSeats(base, b.seats_address(), stack);
  Construct(base, records(b.roll_cage_address()), 4, stack);
  InteriorResetRollCage(base, b.roll_cage_address(), stack);
  return block;
}

uint32_t InteriorPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E35D8 InteriorPoints::Reset");
  const schema::SecondSetInteriorPoints b(base, block);
  b.set_seat_position_source(0);
  b.set_record(0u);
  b.set_skeleton(0);
  b.set_cockpit(0);
  b.set_accessory_shown(1);
  ClearAndName(base, b.body_address(), 0x82602864, stack);
  InteriorResetDials(base, b.dials_address(), stack);
  InteriorResetMeters(base, b.meters_address(), stack);
  InteriorResetSteering(base, b.steering_address(), stack);
  InteriorResetSeats(base, b.seats_address(), stack);
  return InteriorResetRollCage(base, b.roll_cage_address(), stack);
}

uint32_t LightPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DDCF8 LightPoints::LightPoints");
  Construct(base, block + 48 + 16, 18, stack);
  LightsResetFront(base, block + 48, stack);
  Construct(base, block + 4384 + 16, 18, stack);
  LightsResetSides(base, block + 4384, stack);
  Construct(base, block + 8720 + 16, 18, stack);
  LightsResetRear(base, block + 8720, stack);
  return block;
}

uint32_t LightPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC8A8 LightPoints::Reset");
  LightsResetFront(base, block + 48, stack);
  LightsResetSides(base, block + 4384, stack);
  const uint32_t r3 = LightsResetRear(base, block + 8720, stack);
  for (uint32_t at = 8; at < 40; at += 4) {
    Store(base, block + at, 0);
  }
  return r3;
}

uint32_t TrafficPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E68E8 TrafficPoints::TrafficPoints");
  Construct(base, block + 16, 6, stack);
  TrafficResetBody(base, block + 16, stack);
  Construct(base, block + 1472, 7, stack);
  TrafficResetHeadlights(base, block + 1472, stack);
  Construct(base, block + 3152, 12, stack);
  TrafficResetTailLights(base, block + 3152, stack);
  return block;
}

uint32_t TrafficPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5880 TrafficPoints::Reset");
  schema::TrafficPoints(base, block).set_word_0(0u);
  TrafficResetBody(base, block + 16, stack);
  TrafficResetHeadlights(base, block + 1472, stack);
  return TrafficResetTailLights(base, block + 3152, stack);
}

namespace {

// A wheels group's init: `count` records at `records` constructed, the w's
// of the two rows at `rows` = 1.0, then `reset`.
uint32_t InitWheelsGroup(uint8_t* base, uint32_t group, uint32_t records, uint32_t count,
                         uint32_t rows, uint32_t stack,
                         uint32_t (*reset)(uint8_t*, uint32_t, uint32_t)) {
  Construct(base, records, count, stack);
  CopyWord(base, kOne, rows + 12);
  CopyWord(base, kOne, rows + 28);
  reset(base, group, stack);
  return group;
}

}  // namespace

uint32_t WheelsInitTire(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E8A80 WheelPoints::InitTire");
  const schema::SecondSetWheelPoints g(base, group);
  return InitWheelsGroup(base, group, g.tire_records_address(), 6, g.tire_rows_address(), stack,
                         WheelsResetTire);
}

uint32_t WheelsInitRimA(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E99C0 WheelPoints::InitRimA");
  const schema::SecondSetWheelPoints g(base, group);
  return InitWheelsGroup(base, group, g.group_records_address(), 7, g.rim_rows_address(), stack,
                         WheelsResetRimA);
}

uint32_t WheelsInitRimB(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EA8E8 WheelPoints::InitRimB");
  const schema::SecondSetWheelPoints g(base, group);
  return InitWheelsGroup(base, group, g.group_records_address(), 7, g.rim_rows_address(), stack,
                         WheelsResetRimB);
}

uint32_t WheelsInitBrakes(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EB320 WheelPoints::InitBrakes");
  const schema::SecondSetWheelPoints g(base, group);
  return InitWheelsGroup(base, group, g.group_records_address(), 12, g.brake_rows_address(), stack,
                         WheelsResetBrakes);
}

uint32_t WheelPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ECC48 WheelPoints::WheelPoints");
  const schema::SecondSetWheelPoints b(base, block);
  WheelsInitTire(base, b.front_tire_address(), stack);
  WheelsInitTire(base, b.rear_tire_address(), stack);
  WheelsInitRimA(base, b.rim_a_front_address(), stack);
  WheelsInitRimA(base, b.rim_a_rear_address(), stack);
  WheelsInitRimB(base, b.rim_b_front_address(), stack);
  WheelsInitRimB(base, b.rim_b_rear_address(), stack);
  WheelsInitBrakes(base, b.brakes_front_address(), stack);
  WheelsInitBrakes(base, b.brakes_rear_address(), stack);
  return block;
}

uint32_t WheelPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E85C8 WheelPoints::Reset");
  const schema::SecondSetWheelPoints b(base, block);
  b.set_car_model(0);
  b.set_car_record(0u);
  b.set_wheels_record(0u);
  b.set_word_12(0);
  b.set_word_16(0);
  b.set_word_20(0);
  b.set_brake_model(0);
  WheelsResetTire(base, b.front_tire_address(), stack);
  WheelsResetTire(base, b.rear_tire_address(), stack);
  WheelsResetRimA(base, b.rim_a_front_address(), stack);
  WheelsResetRimA(base, b.rim_a_rear_address(), stack);
  WheelsResetRimB(base, b.rim_b_front_address(), stack);
  WheelsResetRimB(base, b.rim_b_rear_address(), stack);
  WheelsResetBrakes(base, b.brakes_front_address(), stack);
  const uint32_t r3 = WheelsResetBrakes(base, b.brakes_rear_address(), stack);
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
