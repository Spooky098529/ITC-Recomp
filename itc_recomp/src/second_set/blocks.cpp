// The second set's point blocks' constructors and resets (see blocks.h),
// call for call as the recompiled functions.

#include "second_set/blocks.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "scene/scene.h"
#include "second_set/point_groups.h"
#include "second_set/points.h"

namespace second_set {

namespace {

using car_physics::ops::CopyWord;
using car_physics::ops::kOne;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t R = kPointRecordBytes;

// `count` records from `at` constructed, from a frame at `sp`.
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
  const uint32_t sp = stack - 112;
  Construct(base, group, 25, sp);
  MatrixWs(base, group + 6016);
  PartsResetLeftDoor(base, group, sp);
  return group;
}

uint32_t PartsInitRightDoor(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF610 PartsPoints::InitRightDoor");
  const uint32_t sp = stack - 112;
  Construct(base, group, 25, sp);
  MatrixWs(base, group + 6016);
  PartsResetRightDoor(base, group, sp);
  return group;
}

uint32_t PartsInitHeadlights(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E1388 PartsPoints::InitHeadlights");
  const uint32_t sp = stack - 112;
  Construct(base, group, 13, sp);
  MatrixWs(base, group + 3120);
  PartsResetHeadlights(base, group, sp);
  return group;
}

uint32_t PartsInitMufflers(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E0010 PartsPoints::InitMufflers");
  const uint32_t sp = stack - 112;
  for (uint32_t m = 0; m < 4; m++) {
    MatrixWs(base, group + 64 * m);
  }
  Construct(base, group + 256, 5, sp);
  PartsResetMufflers(base, group, sp);
  return group;
}

uint32_t PartsPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E2A10 PartsPoints::PartsPoints");
  const uint32_t sp = stack - 128;
  Construct(base, block + 0, 11, sp);
  PartsResetFrontBumper(base, block + 0, sp);
  PartsInitCooling(base, block + 2656, sp);
  Construct(base, block + 2896, 7, sp);
  PartsResetRearBumper(base, block + 2896, sp);
  PartsInitSideSkirts(base, block + 4592, sp);
  Construct(base, block + 4832, 3, sp);
  PartsResetRearSpoiler(base, block + 4832, sp);
  Construct(base, block + 5552, 2, sp);
  PartsResetBonnet(base, block + 5552, sp);
  PartsInitLeftDoor(base, block + 6032, sp);
  PartsInitRightDoor(base, block + 12128, sp);
  Construct(base, block + 18224, 23, sp);
  PartsResetDoors(base, block + 18224, sp);
  PartsInitLeftMirror(base, block + 23760, sp);
  PartsInitRightMirror(base, block + 24000, sp);
  PartsInitMirror(base, block + 24240, sp);
  PartsInitHeadlights(base, block + 24480, sp);
  Construct(base, block + 27680, 15, sp);
  PartsResetTailLights(base, block + 27680, sp);
  Construct(base, block + 31280, 7, sp);
  PartsResetFenders(base, block + 31280, sp);
  Construct(base, block + 32960, 5, sp);
  PartsResetCabin(base, block + 32960, sp);
  Construct(base, block + 34160, 2, sp);
  PartsResetEngine(base, block + 34160, sp);
  PartsInitMufflers(base, block + 34640, sp);
  Construct(base, block + 36096, 4, sp);
  PartsResetShadows(base, block + 36096, sp);
  return block;
}

uint32_t PartsPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E00F0 PartsPoints::Reset");
  const uint32_t sp = stack - 112;
  Store(base, block + 37088, Load(base, scene::kScene));
  Store(base, block + 37076, 0);
  Store(base, block + 37092, 0);
  PartsResetFrontBumper(base, block + 0, sp);
  ClearAndName(base, block + 2656, 0x82602620, sp);
  PartsResetRearBumper(base, block + 2896, sp);
  ClearAndName(base, block + 4592, 0x82602640, sp);
  PartsResetRearSpoiler(base, block + 4832, sp);
  PartsResetBonnet(base, block + 5552, sp);
  PartsResetLeftDoor(base, block + 6032, sp);
  PartsResetRightDoor(base, block + 12128, sp);
  PartsResetDoors(base, block + 18224, sp);
  ClearAndName(base, block + 23760, 0x826026BC, sp);
  ClearAndName(base, block + 24000, 0x82602724, sp);
  ClearAndName(base, block + 24240, 0x82602784, sp);
  PartsResetHeadlights(base, block + 24480, sp);
  PartsResetTailLights(base, block + 27680, sp);
  PartsResetFenders(base, block + 31280, sp);
  PartsResetCabin(base, block + 32960, sp);
  PartsResetEngine(base, block + 34160, sp);
  PartsResetMufflers(base, block + 34640, sp);
  return PartsResetShadows(base, block + 36096, sp);
}

uint32_t CockpitPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC190 CockpitPoints::CockpitPoints");
  const uint32_t sp = stack - 128;
  Construct(base, block + 0, 3, sp);
  CockpitResetFenders(base, block + 0, sp);
  Construct(base, block + 720, 2, sp);
  CockpitResetCabin(base, block + 720, sp);
  CockpitInitBonnet(base, block + 1200, sp);
  Construct(base, block + 1440, 2, sp);
  CockpitResetDoors(base, block + 1440, sp);
  CockpitInitMirror(base, block + 1920, sp);
  Construct(base, block + 2160, 8, sp);
  CockpitResetHeadlights(base, block + 2160, sp);
  return block;
}

uint32_t CockpitPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DBC10 CockpitPoints::Reset");
  const uint32_t sp = stack - 112;
  CockpitResetFenders(base, block + 0, sp);
  CockpitResetCabin(base, block + 720, sp);
  ClearAndName(base, block + 1200, 0x8200524C, sp);
  CockpitResetDoors(base, block + 1440, sp);
  ClearAndName(base, block + 1920, 0x82005258, sp);
  const uint32_t r3 = CockpitResetHeadlights(base, block + 2160, sp);
  Store(base, block + 4080, 0);
  return r3;
}

uint32_t InteriorPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E4FD8 InteriorPoints::InteriorPoints");
  const uint32_t sp = stack - 128;
  InteriorInitBody(base, block + 0, sp);
  Construct(base, block + 240 + 16, 12, sp);
  InteriorResetDials(base, block + 240, sp);
  Construct(base, block + 3152 + 16, 22, sp);
  InteriorResetMeters(base, block + 3152, sp);
  Construct(base, block + 8512 + 16, 4, sp);
  InteriorResetSteering(base, block + 8512, sp);
  Construct(base, block + 9488 + 16, 4, sp);
  InteriorResetSeats(base, block + 9488, sp);
  Construct(base, block + 10464 + 16, 4, sp);
  InteriorResetRollCage(base, block + 10464, sp);
  return block;
}

uint32_t InteriorPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E35D8 InteriorPoints::Reset");
  const uint32_t sp = stack - 96;
  for (const uint32_t at : {11440u, 11444u, 11448u, 11452u}) {
    Store(base, block + at, 0);
  }
  Store(base, block + 11456, 1);
  ClearAndName(base, block + 0, 0x82602864, sp);
  InteriorResetDials(base, block + 240, sp);
  InteriorResetMeters(base, block + 3152, sp);
  InteriorResetSteering(base, block + 8512, sp);
  InteriorResetSeats(base, block + 9488, sp);
  return InteriorResetRollCage(base, block + 10464, sp);
}

uint32_t LightPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DDCF8 LightPoints::LightPoints");
  const uint32_t sp = stack - 128;
  Construct(base, block + 48 + 16, 18, sp);
  LightsResetFront(base, block + 48, sp);
  Construct(base, block + 4384 + 16, 18, sp);
  LightsResetSides(base, block + 4384, sp);
  Construct(base, block + 8720 + 16, 18, sp);
  LightsResetRear(base, block + 8720, sp);
  return block;
}

uint32_t LightPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC8A8 LightPoints::Reset");
  const uint32_t sp = stack - 96;
  LightsResetFront(base, block + 48, sp);
  LightsResetSides(base, block + 4384, sp);
  const uint32_t r3 = LightsResetRear(base, block + 8720, sp);
  for (uint32_t at = 8; at < 40; at += 4) {
    Store(base, block + at, 0);
  }
  return r3;
}

uint32_t TrafficPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E68E8 TrafficPoints::TrafficPoints");
  const uint32_t sp = stack - 128;
  Construct(base, block + 16, 6, sp);
  TrafficResetBody(base, block + 16, sp);
  Construct(base, block + 1472, 7, sp);
  TrafficResetHeadlights(base, block + 1472, sp);
  Construct(base, block + 3152, 12, sp);
  TrafficResetTailLights(base, block + 3152, sp);
  return block;
}

uint32_t TrafficPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E5880 TrafficPoints::Reset");
  const uint32_t sp = stack - 96;
  Store(base, block + 0, 0);
  TrafficResetBody(base, block + 16, sp);
  TrafficResetHeadlights(base, block + 1472, sp);
  return TrafficResetTailLights(base, block + 3152, sp);
}

namespace {

// A wheels group's init: `count` records at `records` constructed, the w's
// of the two rows at `rows` = 1.0, then `reset`.
uint32_t InitWheelsGroup(uint8_t* base, uint32_t group, uint32_t records, uint32_t count,
                         uint32_t rows, uint32_t stack,
                         uint32_t (*reset)(uint8_t*, uint32_t, uint32_t)) {
  const uint32_t sp = stack - 112;
  Construct(base, records, count, sp);
  CopyWord(base, kOne, rows + 12);
  CopyWord(base, kOne, rows + 28);
  reset(base, group, sp);
  return group;
}

}  // namespace

uint32_t WheelsInitTire(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E8A80 WheelPoints::InitTire");
  return InitWheelsGroup(base, group, group + 368, 6, group + 1808, stack, WheelsResetTire);
}

uint32_t WheelsInitRimA(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E99C0 WheelPoints::InitRimA");
  return InitWheelsGroup(base, group, group + 32, 7, group + 1712, stack, WheelsResetRimA);
}

uint32_t WheelsInitRimB(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EA8E8 WheelPoints::InitRimB");
  return InitWheelsGroup(base, group, group + 32, 7, group + 1712, stack, WheelsResetRimB);
}

uint32_t WheelsInitBrakes(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EB320 WheelPoints::InitBrakes");
  return InitWheelsGroup(base, group, group + 32, 12, group + 2912, stack, WheelsResetBrakes);
}

uint32_t WheelPointsConstruct(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ECC48 WheelPoints::WheelPoints");
  const uint32_t sp = stack - 96;
  WheelsInitTire(base, block + 32, sp);
  WheelsInitTire(base, block + 1872, sp);
  WheelsInitRimA(base, block + 3712, sp);
  WheelsInitRimA(base, block + 5456, sp);
  WheelsInitRimB(base, block + 7200, sp);
  WheelsInitRimB(base, block + 8944, sp);
  WheelsInitBrakes(base, block + 10688, sp);
  WheelsInitBrakes(base, block + 13632, sp);
  return block;
}

uint32_t WheelPointsReset(uint8_t* base, uint32_t block, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E85C8 WheelPoints::Reset");
  const uint32_t sp = stack - 112;
  for (uint32_t at = 0; at <= 24; at += 4) {
    Store(base, block + at, 0);
  }
  WheelsResetTire(base, block + 32, sp);
  WheelsResetTire(base, block + 1872, sp);
  WheelsResetRimA(base, block + 3712, sp);
  WheelsResetRimA(base, block + 5456, sp);
  WheelsResetRimB(base, block + 7200, sp);
  WheelsResetRimB(base, block + 8944, sp);
  WheelsResetBrakes(base, block + 10688, sp);
  const uint32_t r3 = WheelsResetBrakes(base, block + 13632, sp);
  for (const uint32_t at : {2172u, 5468u, 8956u, 13644u}) {
    Store(base, block + at, 0);
  }
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
