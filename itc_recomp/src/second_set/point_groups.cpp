// The second set's point groups' resets and inits (see point_groups.h),
// call for call as the recompiled functions.

#include "second_set/point_groups.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "native/buffers.h"
#include "second_set/points.h"
#include "schema/car_draw.h"
#include "schema/second_set.h"

#include <bit>
#include <cstdlib>

namespace second_set {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// `count` records from `group` cleared and named from the words at `table`,
// the first `nodes` as nodes; `sp` the r1 passed on to Clear. The last
// record (SetShape's or SetNode's r3).
uint32_t NameRecords(uint32_t group, uint32_t table, uint32_t count,
                     uint32_t nodes, uint32_t sp) {
  uint32_t record = group;
  for (uint32_t i = 0; i < count; i++) {
    record = group + kPointRecordBytes * i;
    PointRecordClear(record, sp);
    const uint32_t name = Load(table + 4 * i);
    if (i < nodes) {
      PointRecordSetNode(record, name);
    } else {
      PointRecordSetShape(record, name);
    }
  }
  return record;
}

// A single record's init: PointRecord::PointRecord, Clear, SetShape with
// the name at `name_word`.
uint32_t InitRecord(uint32_t record, uint32_t name_word, uint32_t stack) {
  PointRecordConstruct(record, stack);
  PointRecordClear(record, stack);
  PointRecordSetShape(record, Load(name_word));
  return record;
}

// The identity (vupkd3d128's rows, stored aligned) at `at`.
void Identity(uint32_t at) {
  const uint32_t aligned = at & ~0xFu;
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t w = 0; w < 16; w++) {
    Store(aligned + 4 * w, w % 5 == 0 ? one : 0);
  }
}

constexpr uint32_t kFive = schema::SecondSetGlobals::five_address();  // 5.0

}  // namespace

uint32_t PartsResetFrontBumper(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE0E0 PartsPoints::ResetFrontBumper");
  const schema::PartsPoints g(group);
  g.set_front_car_record(0u);
  g.set_front_plate_texture(0u);
  g.set_front_plate_set(1u);
  return NameRecords(group, 0x826025F4, 11, 0, stack);
}

uint32_t PartsResetRearBumper(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE2C8 PartsPoints::ResetRearBumper");
  const schema::PartsPoints g(group);
  g.set_rear_plate_set(1u);
  g.set_rear_plate_texture(0u);
  return NameRecords(group, 0x82602624, 7, 0, stack);
}

uint32_t PartsResetRearSpoiler(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE498 PartsPoints::ResetRearSpoiler");
  return NameRecords(group, 0x82602644, 3, 0, stack);
}

uint32_t PartsResetBonnet(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE548 PartsPoints::ResetBonnet");
  return NameRecords(group, 0x82602650, 2, 1, stack);
}

uint32_t PartsResetLeftDoor(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE600 PartsPoints::ResetLeftDoor");
  schema::PartsPoints(group).set_door_car_record(0u);
  return NameRecords(group, 0x82602658, 25, 2, stack);
}

uint32_t PartsResetRightDoor(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE7E8 PartsPoints::ResetRightDoor");
  NameRecords(group, 0x826026C0, 25, 2, stack);
  // The mirror winkers scaled, though Clear left their models 0.
  uint32_t r3 = 0;
  for (uint32_t i = 7; i <= 10; i++) {
    const schema::SecondSetPointRecord record(group + kPointRecordBytes * i);
    const double five = double(guest::LoadFloat(kFive));
    r3 = material_passes::ScaleBlended(record.model(), record.shape_index(), five, stack);
  }
  return r3;
}

uint32_t PartsResetDoors(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE9D0 PartsPoints::ResetDoors");
  return NameRecords(group, 0x82602728, 23, 0, stack);
}

uint32_t PartsResetHeadlights(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF740 PartsPoints::ResetHeadlights");
  const uint32_t sp = stack - 192;
  NameRecords(group, 0x82602788, 13, 7, sp);
  Identity(sp + 80);
  const uint32_t popup = schema::PartsPoints(group).headlights_popup_matrix_address();
  if (!native::Buffers_Memcpy(popup, sp + 80, 64, 0x820DF7E8)) {
    ITC_LOG_ERROR("Second set: ResetHeadlights' memcpy refused after its checks");
    std::abort();
  }
  return popup;
}

uint32_t PartsResetTailLights(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DEBF0 PartsPoints::ResetTailLights");
  return NameRecords(group, 0x826027BC, 15, 8, stack);
}

uint32_t PartsResetFenders(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DECD0 PartsPoints::ResetFenders");
  return NameRecords(group, 0x826027F8, 7, 0, stack);
}

uint32_t PartsResetCabin(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DEE88 PartsPoints::ResetCabin");
  return NameRecords(group, 0x82602814, 5, 0, stack);
}

uint32_t PartsResetEngine(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF020 PartsPoints::ResetEngine");
  return NameRecords(group, 0x82602828, 2, 0, stack);
}

uint32_t PartsResetMufflers(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF0C0 PartsPoints::ResetMufflers");
  return NameRecords(group + 256, 0x82602830, 5, 4, stack);
}

uint32_t PartsResetShadows(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF128 PartsPoints::ResetShadows");
  schema::PartsPoints(group).set_shadows_word_960(0u);
  return NameRecords(group, 0x82602844, 4, 0, stack);
}

uint32_t PartsInitCooling(uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF410 PartsPoints::InitCooling");
  return InitRecord(record, 0x82602620, stack);
}

uint32_t PartsInitSideSkirts(uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF570 PartsPoints::InitSideSkirts");
  return InitRecord(record, 0x82602640, stack);
}

uint32_t PartsInitLeftMirror(uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF668 PartsPoints::InitLeftMirror");
  return InitRecord(record, 0x826026BC, stack);
}

uint32_t PartsInitRightMirror(uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF6B0 PartsPoints::InitRightMirror");
  return InitRecord(record, 0x82602724, stack);
}

uint32_t PartsInitMirror(uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF6F8 PartsPoints::InitMirror");
  return InitRecord(record, 0x82602784, stack);
}

uint32_t CockpitInitBonnet(uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC0A8 CockpitPoints::InitBonnet");
  return InitRecord(record, 0x8200524C, stack);
}

uint32_t CockpitInitMirror(uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC0F0 CockpitPoints::InitMirror");
  return InitRecord(record, 0x82005258, stack);
}

uint32_t CockpitResetFenders(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB878 CockpitPoints::ResetFenders");
  return NameRecords(group, 0x82005238, 3, 0, stack);
}

uint32_t CockpitResetCabin(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB930 CockpitPoints::ResetCabin");
  return NameRecords(group, 0x82005244, 2, 0, stack);
}

uint32_t CockpitResetDoors(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB9E8 CockpitPoints::ResetDoors");
  return NameRecords(group, 0x82005250, 2, 0, stack);
}

uint32_t CockpitResetHeadlights(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DBAA0 CockpitPoints::ResetHeadlights");
  return NameRecords(group, 0x8200525C, 8, 4, stack);
}

uint32_t InteriorInitBody(uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E39B0 InteriorPoints::InitBody");
  return InitRecord(record, 0x82602864, stack);
}

uint32_t InteriorResetDials(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E2FC8 InteriorPoints::ResetDials");
  const schema::SecondSetInteriorPoints g(group);
  g.set_group_record(0u);
  g.set_group_word_8(0);
  g.set_group_word_4(0xFFFFFFFFu);
  return NameRecords(g.group_records_address(), 0x82602868, 12, 2, stack);
}

uint32_t InteriorResetMeters(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3200 InteriorPoints::ResetMeters");
  const schema::SecondSetInteriorPoints g(group);
  g.set_group_record(0u);
  g.set_group_word_4(0xFFFFFFFFu);
  g.set_turbo_provider(0);
  const uint32_t r3 = NameRecords(g.group_records_address(), 0x82602898, 22, 7, stack);
  for (uint32_t w = 0; w < 7; w++) {
    Store(g.gauge_providers_address() + 4 * w, 0);
  }
  return r3;
}

uint32_t InteriorResetSteering(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E33F0 InteriorPoints::ResetSteering");
  return NameRecords(schema::SecondSetInteriorPoints(group).group_records_address(),
                     0x826028F0, 4, 0, stack);
}

uint32_t InteriorResetSeats(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3440 InteriorPoints::ResetSeats");
  return NameRecords(schema::SecondSetInteriorPoints(group).group_records_address(),
                     0x82602900, 4, 2, stack);
}

uint32_t InteriorResetRollCage(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E34F8 InteriorPoints::ResetRollCage");
  return NameRecords(schema::SecondSetInteriorPoints(group).group_records_address(),
                     0x82602910, 4, 0, stack);
}

uint32_t LightsResetFront(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC748 LightPoints::ResetFront");
  schema::LightPoints(group).set_record(0u);
  return NameRecords(group + 16, 0x820055F0, 18, 0, stack);
}

uint32_t LightsResetSides(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC7A0 LightPoints::ResetSides");
  schema::LightPoints(group).set_record(0u);
  return NameRecords(group + 16, 0x82005638, 18, 0, stack);
}

uint32_t LightsResetRear(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC7F8 LightPoints::ResetRear");
  schema::LightPoints(group).set_record(0u);
  return NameRecords(group + 16, 0x82005680, 18, 0, stack);
}

uint32_t TrafficResetBody(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E54E8 TrafficPoints::ResetBody");
  return NameRecords(group, 0x8260292C, 6, 0, stack);
}

uint32_t TrafficResetHeadlights(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E56D0 TrafficPoints::ResetHeadlights");
  return NameRecords(group, 0x82602944, 7, 4, stack);
}

uint32_t TrafficResetTailLights(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E57A0 TrafficPoints::ResetTailLights");
  return NameRecords(group, 0x82602960, 12, 8, stack);
}

namespace {

using car_physics::ops::CopyWord;
using car_physics::ops::kOne;
using car_physics::ops::kZero;

// Two rows at `rows` (16 bytes apart) = (0.0, 0.0, 0.0, 1.0), each stored
// +4, +0, -4 (from +4), then +8.
void ZeroOneRows(uint32_t rows) {
  for (uint32_t row = 0; row < 2; row++) {
    const uint32_t at = rows + 4 + 16 * row;
    CopyWord(kZero, at + 4);
    CopyWord(kZero, at + 0);
    CopyWord(kZero, at - 4);
    CopyWord(kOne, at + 8);
  }
}

}  // namespace

uint32_t WheelsResetTire(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E74B8 WheelPoints::ResetTire");
  const schema::SecondSetWheelPoints g(group);
  CopyWord(kZero, g.tire_out1_address());
  CopyWord(kZero, g.tire_out2_address());
  g.set_matrix(0u);
  g.set_car_record(0u);
  g.set_wheels_record(0u);
  g.set_tire_front(1);
  NameRecords(g.tire_records_address(), 0x8260299C, 6, 0, stack);
  for (uint32_t i = 0; i < 6; i++) {
    CopyWord(kZero, g.tire_weights_address() + 4 * i);
    Store(g.tire_shapes_address() + 4 * i, 0xFFFFFFFFu);
  }
  ZeroOneRows(g.tire_rows_address());
  for (uint32_t i = 0; i < 288; i++) {
    guest::StoreU8(g.tire_entries_address() + i, 0);
  }
  return g.tire_entries_address();
}

uint32_t WheelsResetRimA(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7918 WheelPoints::ResetRimA");
  const schema::SecondSetWheelPoints g(group);
  g.set_matrix(0u);
  g.set_wheels_record(0u);
  g.set_word_12(1);
  const uint32_t r3 = NameRecords(g.group_records_address(), 0x826029B4, 7, 0, stack);
  // The original's frame's float4 (0.0, 0.0, 0.0, 1.0) at +80, read back
  // only by it: a C++ array.
  uint32_t row_words[4];
  row_words[0] = Load(kZero);
  row_words[1] = Load(kZero);
  row_words[2] = Load(kZero);
  row_words[3] = Load(kOne);
  for (uint32_t row = 0; row < 2; row++) {
    for (uint32_t w = 0; w < 4; w++) {
      Store(g.rim_rows_address() + 16 * row + 4 * w, row_words[w]);
    }
  }
  return r3;
}

uint32_t WheelsResetRimB(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7C08 WheelPoints::ResetRimB");
  const schema::SecondSetWheelPoints g(group);
  g.set_matrix(0u);
  g.set_wheels_record(0u);
  g.set_word_12(1);
  const uint32_t r3 = NameRecords(g.group_records_address(), 0x826029D0, 7, 0, stack);
  ZeroOneRows(g.rim_rows_address());
  return r3;
}

uint32_t WheelsResetBrakes(uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7F90 WheelPoints::ResetBrakes");
  const schema::SecondSetWheelPoints g(group);
  g.set_matrix(0u);
  g.set_wheels_record(0u);
  g.set_word_12(1);
  const uint32_t r3 = NameRecords(g.group_records_address(), 0x826029EC, 12, 0, stack);
  ZeroOneRows(g.brake_rows_address());
  return r3;
}

}  // namespace second_set
