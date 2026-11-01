// The second set's point groups' resets and inits (see point_groups.h),
// call for call as the recompiled functions.

#include "second_set/point_groups.h"

#include "base/log.h"
#include "guest.h"
#include "material_passes/material_passes.h"
#include "native/buffers.h"
#include "second_set/points.h"

#include <bit>
#include <cstdlib>

namespace second_set {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kInitFrameBytes = 96;

// `count` records from `group` cleared and named from the words at `table`,
// the first `nodes` as nodes; `sp` the reset's r1 (its frame made). The last
// record (SetShape's or SetNode's r3).
uint32_t NameRecords(uint8_t* base, uint32_t group, uint32_t table, uint32_t count,
                     uint32_t nodes, uint32_t sp) {
  uint32_t record = group;
  for (uint32_t i = 0; i < count; i++) {
    record = group + kPointRecordBytes * i;
    PointRecordClear(base, record, sp);
    const uint32_t name = Load(base, table + 4 * i);
    if (i < nodes) {
      PointRecordSetNode(base, record, name);
    } else {
      PointRecordSetShape(base, record, name);
    }
  }
  return record;
}

// A single record's init: PointRecord::PointRecord, Clear, SetShape with
// the name at `name_word`.
uint32_t InitRecord(uint8_t* base, uint32_t record, uint32_t name_word, uint32_t stack) {
  const uint32_t sp = stack - kInitFrameBytes;
  PointRecordConstruct(base, record, sp);
  PointRecordClear(base, record, sp);
  PointRecordSetShape(base, record, Load(base, name_word));
  return record;
}

// The identity (vupkd3d128's rows, stored aligned) at `at`.
void Identity(uint8_t* base, uint32_t at) {
  const uint32_t aligned = at & ~0xFu;
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t w = 0; w < 16; w++) {
    Store(base, aligned + 4 * w, w % 5 == 0 ? one : 0);
  }
}

constexpr uint32_t kFive = 0x82005FE4;  // 5.0

}  // namespace

uint32_t PartsResetFrontBumper(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE0E0 PartsPoints::ResetFrontBumper");
  Store(base, group + 2648, 0);
  Store(base, group + 2644, 0);
  Store(base, group + 2640, 1);
  return NameRecords(base, group, 0x826025F4, 11, 0, stack - 112);
}

uint32_t PartsResetRearBumper(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE2C8 PartsPoints::ResetRearBumper");
  Store(base, group + 1680, 1);
  Store(base, group + 1684, 0);
  return NameRecords(base, group, 0x82602624, 7, 0, stack - 112);
}

uint32_t PartsResetRearSpoiler(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE498 PartsPoints::ResetRearSpoiler");
  return NameRecords(base, group, 0x82602644, 3, 0, stack - 112);
}

uint32_t PartsResetBonnet(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE548 PartsPoints::ResetBonnet");
  return NameRecords(base, group, 0x82602650, 2, 1, stack - 128);
}

uint32_t PartsResetLeftDoor(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE600 PartsPoints::ResetLeftDoor");
  Store(base, group + 6080, 0);
  return NameRecords(base, group, 0x82602658, 25, 2, stack - 128);
}

uint32_t PartsResetRightDoor(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE7E8 PartsPoints::ResetRightDoor");
  const uint32_t sp = stack - 128;
  NameRecords(base, group, 0x826026C0, 25, 2, sp);
  // The mirror winkers scaled, though Clear left their models 0.
  uint32_t r3 = 0;
  for (uint32_t i = 7; i <= 10; i++) {
    const uint32_t record = group + kPointRecordBytes * i;
    const double five = double(guest::LoadFloat(base, kFive));
    r3 = material_passes::ScaleBlended(base, Load(base, record + 4), Load(base, record + 228),
                                       five, sp);
  }
  return r3;
}

uint32_t PartsResetDoors(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DE9D0 PartsPoints::ResetDoors");
  return NameRecords(base, group, 0x82602728, 23, 0, stack - 112);
}

uint32_t PartsResetHeadlights(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF740 PartsPoints::ResetHeadlights");
  const uint32_t sp = stack - 192;
  NameRecords(base, group, 0x82602788, 13, 7, sp);
  Identity(base, sp + 80);
  if (!native::Buffers_Memcpy(base, group + 3120, sp + 80, 64, 0x820DF7E8)) {
    ITC_LOG_ERROR("Second set: ResetHeadlights' memcpy refused after its checks");
    std::abort();
  }
  return group + 3120;
}

uint32_t PartsResetTailLights(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DEBF0 PartsPoints::ResetTailLights");
  return NameRecords(base, group, 0x826027BC, 15, 8, stack - 128);
}

uint32_t PartsResetFenders(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DECD0 PartsPoints::ResetFenders");
  return NameRecords(base, group, 0x826027F8, 7, 0, stack - 112);
}

uint32_t PartsResetCabin(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DEE88 PartsPoints::ResetCabin");
  return NameRecords(base, group, 0x82602814, 5, 0, stack - 112);
}

uint32_t PartsResetEngine(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF020 PartsPoints::ResetEngine");
  return NameRecords(base, group, 0x82602828, 2, 0, stack - 112);
}

uint32_t PartsResetMufflers(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF0C0 PartsPoints::ResetMufflers");
  return NameRecords(base, group + 256, 0x82602830, 5, 4, stack - 128);
}

uint32_t PartsResetShadows(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF128 PartsPoints::ResetShadows");
  Store(base, group + 960, 0);
  return NameRecords(base, group, 0x82602844, 4, 0, stack - 112);
}

uint32_t PartsInitCooling(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF410 PartsPoints::InitCooling");
  return InitRecord(base, record, 0x82602620, stack);
}

uint32_t PartsInitSideSkirts(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF570 PartsPoints::InitSideSkirts");
  return InitRecord(base, record, 0x82602640, stack);
}

uint32_t PartsInitLeftMirror(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF668 PartsPoints::InitLeftMirror");
  return InitRecord(base, record, 0x826026BC, stack);
}

uint32_t PartsInitRightMirror(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF6B0 PartsPoints::InitRightMirror");
  return InitRecord(base, record, 0x82602724, stack);
}

uint32_t PartsInitMirror(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DF6F8 PartsPoints::InitMirror");
  return InitRecord(base, record, 0x82602784, stack);
}

uint32_t CockpitInitBonnet(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC0A8 CockpitPoints::InitBonnet");
  return InitRecord(base, record, 0x8200524C, stack);
}

uint32_t CockpitInitMirror(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC0F0 CockpitPoints::InitMirror");
  return InitRecord(base, record, 0x82005258, stack);
}

uint32_t CockpitResetFenders(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB878 CockpitPoints::ResetFenders");
  return NameRecords(base, group, 0x82005238, 3, 0, stack - 112);
}

uint32_t CockpitResetCabin(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB930 CockpitPoints::ResetCabin");
  return NameRecords(base, group, 0x82005244, 2, 0, stack - 112);
}

uint32_t CockpitResetDoors(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DB9E8 CockpitPoints::ResetDoors");
  return NameRecords(base, group, 0x82005250, 2, 0, stack - 112);
}

uint32_t CockpitResetHeadlights(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DBAA0 CockpitPoints::ResetHeadlights");
  return NameRecords(base, group, 0x8200525C, 8, 4, stack - 128);
}

uint32_t InteriorInitBody(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E39B0 InteriorPoints::InitBody");
  return InitRecord(base, record, 0x82602864, stack);
}

uint32_t InteriorResetDials(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E2FC8 InteriorPoints::ResetDials");
  Store(base, group + 0, 0);
  Store(base, group + 8, 0);
  Store(base, group + 4, 0xFFFFFFFFu);
  return NameRecords(base, group + 16, 0x82602868, 12, 2, stack - 128);
}

uint32_t InteriorResetMeters(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3200 InteriorPoints::ResetMeters");
  Store(base, group + 0, 0);
  Store(base, group + 4, 0xFFFFFFFFu);
  Store(base, group + 5324, 0);
  const uint32_t r3 = NameRecords(base, group + 16, 0x82602898, 22, 7, stack - 144);
  for (uint32_t w = 0; w < 7; w++) {
    Store(base, group + 5296 + 4 * w, 0);
  }
  return r3;
}

uint32_t InteriorResetSteering(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E33F0 InteriorPoints::ResetSteering");
  return NameRecords(base, group + 16, 0x826028F0, 4, 0, stack - 112);
}

uint32_t InteriorResetSeats(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E3440 InteriorPoints::ResetSeats");
  return NameRecords(base, group + 16, 0x82602900, 4, 2, stack - 128);
}

uint32_t InteriorResetRollCage(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E34F8 InteriorPoints::ResetRollCage");
  return NameRecords(base, group + 16, 0x82602910, 4, 0, stack - 112);
}

uint32_t LightsResetFront(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC748 LightPoints::ResetFront");
  Store(base, group + 0, 0);
  return NameRecords(base, group + 16, 0x820055F0, 18, 0, stack - 112);
}

uint32_t LightsResetSides(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC7A0 LightPoints::ResetSides");
  Store(base, group + 0, 0);
  return NameRecords(base, group + 16, 0x82005638, 18, 0, stack - 112);
}

uint32_t LightsResetRear(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DC7F8 LightPoints::ResetRear");
  Store(base, group + 0, 0);
  return NameRecords(base, group + 16, 0x82005680, 18, 0, stack - 112);
}

uint32_t TrafficResetBody(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E54E8 TrafficPoints::ResetBody");
  return NameRecords(base, group, 0x8260292C, 6, 0, stack - 112);
}

uint32_t TrafficResetHeadlights(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E56D0 TrafficPoints::ResetHeadlights");
  return NameRecords(base, group, 0x82602944, 7, 4, stack - 128);
}

uint32_t TrafficResetTailLights(uint8_t* base, uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E57A0 TrafficPoints::ResetTailLights");
  return NameRecords(base, group, 0x82602960, 12, 8, stack - 128);
}

}  // namespace second_set
