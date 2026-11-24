// The stage's small setters (docs/functions/stand-models.md), step for step
// as the translated code (see stand_models.h).

#include "stand_models/stand_models.h"

#include <bit>

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "car_specs/car_specs.h"
#include "debug/frame_state_diff.h"
#include "guest.h"
#include "load_poll/load_poll.h"
#include "native/buffers.h"
#include "schema/action_mappers.h"
#include "schema/car_draw.h"
#include "schema/car_makers.h"
#include "schema/car_models.h"
#include "schema/car_specs.h"
#include "schema/scene_loaders.h"
#include "schema/scene_models.h"
#include "schema/second_set.h"
#include "schema/stand_models.h"
#include "schema/tuning_screen.h"
#include "scene_models/apply.h"
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace stand_models {

namespace {

using namespace car_scene_draw::ops;
using Globals = schema::StandModelsGlobals;
using Model = schema::SceneModel;
using Record = schema::SceneModelRecord;

// The stage's model's record (the model's +32).
Record RecordOf(uint8_t* base, uint32_t model) {
  return Record(base, Model(base, model).record_address());
}

constexpr uint32_t kSetUpB = 0x820C5B28;
constexpr uint32_t kApplyRecord = 0x820DA758;
constexpr uint32_t kWheelsSetRecord = 0x820D8EC0;
constexpr uint32_t kWheelsSetCaliperTexture = 0x820D8ED0;
constexpr uint32_t kSetTexture = 0x820D17D8;
constexpr uint32_t kApplyWheelPaint = 0x820D9C50;
constexpr uint32_t kApplyPaint = 0x820D9B68;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kGet82 = 0x820CBC90;      // AeroParts::Get82(out, parts, level)
constexpr uint32_t kAeroSets = 0x82009A50;   // the aero sets' 24-byte entries
// The image's floats 1.0, 0.0 and -1.0 (the decal scale).
using Image = schema::ActionMappersGlobals;

// Flush-to-zero off, as the callees' plain C++ expects at entry.
void FlushOff() {
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}

// A doubleword (ld, std): its high word first.
uint64_t Load64(uint8_t* base, uint32_t address) {
  return uint64_t(Load(base, address)) << 32 | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// SceneModel::ApplyRecord from `frame`: its plain C++, or where it declines
// (the race's path) its adapter, which runs the original.
uint64_t ApplyRecord(uint8_t* base, const GuestCall& call, uint32_t model, uint32_t frame,
                     uint32_t lr) {
  if (const auto r3 = scene_models::SceneModelApplyRecord(base, call, model, frame)) {
    return *r3;
  }
  return call(kApplyRecord, {model}, frame, lr);
}

}  // namespace

uint32_t StageModel(uint8_t* base, uint32_t holder) {
  const schema::GarageStage stage(base, Load(base, holder));
  return vehicle_accessors::ScenePoolsGetB(base, Globals::scene<schema::View>(base).address, 7,
                                           stage.model_index());
}

uint64_t MakeModel(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t block,
                   uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217FCB0 GarageStage::MakeModel");
  const uint32_t frame = Frame(base, stack, 4384);
  const uint32_t holder = uint32_t(r3);
  // The holder block's +0 (the stage) loaded again for each access, as the
  // original.
  const auto stage = [&] { return schema::GarageStage(base, Load(base, holder)); };
  const uint32_t scene = Globals::scene<schema::View>(base).address;
  if (stage().model_claimed() != 0) {
    return r3;
  }
  const uint32_t record = frame + 80;
  scene_models::SceneModelRecordConstruct(base, record);
  scene_models::SceneModelRecordReset(base, record);
  Record(base, record).set_block(block);
  const int32_t index = vehicle_accessors::ScenePoolsClaimB(base, scene, 7);
  stage().set_model_index(uint32_t(index));
  const uint32_t at = stage().model_index();
  const uint32_t model =
      uint32_t(call(kSetUpB, {scene, 7, at, record}, frame, 0x8217FD2C));
  Model(base, model).set_draw_enabled(word);
  stage().set_draw_word(word);
  stage().set_model_claimed(1);
  Store(base, Globals::stand_holder(base).address + 20, 1);
  return model;
}

uint32_t SetInteriorClass(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178FE8 GarageStage::SetInteriorClass");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)stack;
  const uint32_t model = StageModel(base, holder);
  const int32_t v = int32_t(value);
  RecordOf(base, model).set_interior_engine_grade(v >= 7 ? 2 : v >= 3 ? 1 : 0);
  return model;
}

uint32_t SetWord5016(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179598 GarageStage::SetWord5016");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)stack;
  const uint32_t model = StageModel(base, holder);
  schema::CarDrawBlock(base, Model(base, model).draw_block_address()).set_word_728(value);
  return model;
}

uint32_t SetWheelWord(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179A18 GarageStage::SetWheelWord");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)stack;
  const uint32_t model = StageModel(base, holder);
  schema::SceneModelsWheelsRecord(base, Model(base, model).wheels_record_address())
      .set_morph_each_draw(value);
  return model;
}

double LightValue(uint8_t* base, uint32_t points) {
  ITC_NATIVE_RUNNING("sub_820DF1A0 SceneParts::LightValue");
  return Lfs(base, points + 27664);
}

uint32_t SetLightsOn(uint8_t* base, uint32_t holder, uint32_t flag, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_82179420 GarageStage::SetLightsOn");
  // The original's 112-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)stack;
  const uint32_t model = StageModel(base, holder);
  // The draw block's pop-up (its +688 and +692).
  const schema::CarDrawBlock draw(base, Model(base, model).draw_block_address());
  if (flag != 0) {
    draw.set_popup_set(1);
    draw.set_popup_angle(Image::zero(base));
  } else {
    draw.set_popup_set(0);
    const uint32_t points = Model(base, model).parts<schema::View>().address + 176;
    const double value = LightValue(base, points);
    draw.set_popup_angle(float(value));
    *f1 = value;
    return points;  // r3 as LightValue (a leaf) leaves it
  }
  return model;
}

uint32_t SetDecal(uint8_t* base, uint32_t holder, uint32_t entry, uint32_t a, uint32_t b,
                  double f1, double f2, double f3, double f4, uint32_t stack, uint32_t at) {
  if (at == kDecalA) {
    ITC_NATIVE_RUNNING("sub_82179818 GarageStage::SetDecalA");
  } else {
    ITC_NATIVE_RUNNING("sub_821798D8 GarageStage::SetDecalB");
  }
  // The table {2, 1, 0} the original builds at its 160-byte frame's +80 is
  // a value since E5 step 6 (only it reads it); no guest call is made.
  static constexpr uint32_t kTable[3] = {2, 1, 0};
  const uint32_t model = StageModel(base, holder);
  // The table read unchecked, as the original (rlwinm: the index's low 30
  // bits times 4, wrapping): past it, the stack's word where the original
  // reads it.
  const uint32_t word =
      entry < 3 ? kTable[entry] : Load(base, stack - 160 + 80 + (entry << 2));
  // The record's decal `word` of the three at +192 (A, the model's +224) or
  // +384 (B, +416).
  const schema::CarMakersRecordDecal e(base, model + (word << 6) + at);
  e.set_x(float(f1));
  e.set_image(a);
  e.set_y(float(f2));
  e.set_mirrored(b);
  e.set_angle(float(f3));
  e.set_scale(float(f4));
  e.set_flag_scale(float(b != 0 ? Fmuls(f4, double(Image::minus_one(base))) : f4));
  return model;
}

uint64_t SetPart(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t slot,
                 uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178F00 GarageStage::SetPart");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  // The jump table (case 0 the split piece sub_82178F68): the parts
  // record's words, none for 13.
  using Set = void (Record::*)(uint32_t) const;
  static constexpr Set kSet[16] = {
      &Record::set_parts_aero0,  &Record::set_parts_power,        &Record::set_parts_aero2,
      &Record::set_parts_aero1,  &Record::set_parts_aero3,        &Record::set_parts_bonnet,
      &Record::set_parts_aero_kit, &Record::set_parts_aero_kit,   &Record::set_parts_aero6,
      &Record::set_parts_aero6,  &Record::set_parts_aero7,        &Record::set_parts_aero8,
      &Record::set_parts_aero9,  nullptr,                         &Record::set_parts_engine_stage,
      &Record::set_parts_cooling};
  if (slot <= 15 && kSet[slot] != nullptr) {
    (RecordOf(base, model).*kSet[slot])(word);
  }
  return ApplyRecord(base, call, model, stack, 0x82178FD0);
}

uint64_t SetInteriorWord(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t kind,
                         uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179050 GarageStage::SetInteriorWord");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  const int32_t k = int32_t(kind);
  const Record rec = RecordOf(base, model);
  if (k == 1) {
    rec.set_interior_seats(word);
  } else if (k == 4) {
    rec.set_interior_item1(word);
  } else if (k == 5) {
    rec.set_interior_item2(word);
  }
  return ApplyRecord(base, call, model, stack, 0x821790B8);
}

uint64_t SetInteriorLevel(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t value,
                          uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821791C0 GarageStage::SetInteriorLevel");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  const int32_t v = int32_t(value);
  const Record rec = RecordOf(base, model);
  rec.set_interior_seats(v < 3 ? 0 : v < 6 ? 1 : 2);
  rec.set_interior_has_seats(v < 1 ? 0 : 1);
  return ApplyRecord(base, call, model, stack, 0x82179238);
}

uint64_t SetInteriorPair(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t a,
                         uint32_t b, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179250 GarageStage::SetInteriorPair");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  const Record rec = RecordOf(base, model);
  rec.set_interior_item0(a);
  rec.set_interior_item1(b);
  return ApplyRecord(base, call, model, stack, 0x82179294);
}

uint64_t ClearChoice(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t choice,
                     uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179378 GarageStage::ClearChoice");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  // The record's interior choice `choice` (+3780 + 12 x it): its set word.
  schema::CarMakersRecordChoice(base, choice * 12 + RecordOf(base, model).interior_choices_address())
      .set_set(0);
  return ApplyRecord(base, call, model, stack, 0x821793BC);
}

uint64_t SetAero(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t word,
                 uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179548 GarageStage::SetAero");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  Model(base, model).set_selected_texture(word);
  return ApplyRecord(base, call, model, stack, 0x82179580);
}

uint64_t ReloadDecals(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178EC0 GarageStage::ReloadDecals");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  return scene_models::SceneModelReloadDecals(base, call, StageModel(base, holder), stack);
}

uint64_t SetDecalWord(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t word,
                      uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821793D0 GarageStage::SetDecalWord");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  RecordOf(base, StageModel(base, holder)).set_bonnet_painted(word);
  return scene_models::SceneModelReloadDecals(base, call, StageModel(base, holder), stack);
}

uint32_t SetSticker(uint8_t* base, uint32_t holder, uint32_t area, uint32_t slot, uint32_t word,
                    uint32_t flag, uint64_t r8, uint64_t r9, double f1, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217B3D8 GarageStage::SetSticker");
  // The original's 128-byte frame (only its back chain) left out, and r8
  // and r9, which it stores into the caller's parameter area (their home
  // slots) and reads back, kept as values (E5 step 6: only it reads them).
  // The size's doublewords below are the caller's stack arguments.
  const uint32_t model = StageModel(base, holder);
  // The record index's base and the code by the area (signed; any other
  // area 51 and 5).
  static constexpr uint32_t kIndex[7] = {9, 15, 21, 27, 33, 39, 45};
  static constexpr uint32_t kCode[7] = {8, 2, 1, 3, 4, 7, 6};
  const int32_t a = int32_t(area);
  const bool known = a >= 0 && a <= 6;
  const uint32_t index = slot + (known ? kIndex[a] : 51);
  // The record's decal entry `index` (CarMakersRecordDecal; its +8 and the
  // doublewords at +16, +24, +48 and +56 have no field: offsets).
  const schema::CarMakersRecordDecal d(base, Model(base, model).record_address() + (index << 6));
  const uint32_t e = d.address;
  Store(base, e + 8, known ? kCode[a] : 5);
  const uint64_t position0 = r8;
  const uint64_t position1 = r9;
  d.set_image(word);
  d.set_mirrored(flag);
  const uint64_t size0 = Load64(base, stack + 96);
  const uint64_t size1 = Load64(base, stack + 104);
  Store64(base, e + 16, position0);
  Store64(base, e + 24, position1);
  d.set_angle(float(f1));
  Store64(base, e + 48, size0);
  Store64(base, e + 56, size1);
  return model;
}

uint64_t SetParts(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t a, uint32_t b,
                  uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821797A8 GarageStage::SetParts");
  // The original's 144-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  const Record rec = RecordOf(base, model);
  rec.set_parts_aero0(a);
  rec.set_parts_aero2(b);
  rec.set_parts_aero1(c);
  rec.set_parts_aero9(d);
  rec.set_parts_bonnet(e);
  rec.set_parts_aero3(f);
  rec.set_parts_aero_kit(0);
  return ApplyRecord(base, call, model, stack, 0x8217980C);
}

uint64_t SetAeroSet(uint8_t* base, const GuestCall& call, uint32_t holder, uint64_t set,
                    uint32_t fallback5, uint32_t fallback6, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217DBC0 GarageStage::SetAeroSet");
  const uint32_t frame = Frame(base, stack, 224);
  const uint32_t model = StageModel(base, holder);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t id = schema::CarMakersCar(base, SelectedGarageCar(base, save)).car_id();
  const uint32_t out = frame + 80, copy = frame + 128;
  uint32_t parts[9];
  car_specs::GetCarParts(base, id, parts);
  for (uint32_t i = 0; i < 9; i++) {
    Store(base, out + 4 * i, parts[i]);
  }
  if (!native::Buffers_Memcpy(base, copy, out, 36, 0x8217DC58)) {
    call(kMemcpy, {copy, out, 36}, frame, 0x8217DC58);
  }
  const uint64_t level = set + 1;
  for (uint32_t i = 0; i < 9; i++) {
    parts[i] = Load(base, copy + 4 * i);
  }
  uint32_t kit = out;
  if (!car_specs::AeroPart82(base, out, parts, int32_t(level))) {
    kit = uint32_t(call(kGet82, {out, copy, level}, frame, 0x8217DC6C));
  }
  const uint32_t w = schema::AeroPartRecord(base, kit).flags();
  const uint32_t entry =
      (uint32_t(Model(base, model).block() << 1) + uint32_t(set)) * 24 + kAeroSets;
  if (!native::Buffers_Memcpy(base, out, entry, 24, 0x8217DC9C)) {
    call(kMemcpy, {out, entry, 24}, frame, 0x8217DC9C);
  }
  auto word = [&](uint32_t at) { return Load(base, frame + at); };
  const Record rec = RecordOf(base, model);
  if (int32_t(word(80)) != -1) {
    rec.set_parts_aero0(word(80));
  }
  if (int32_t(word(84)) != -1) {
    rec.set_parts_aero2(word(84));
  }
  if (int32_t(word(88)) != -1) {
    rec.set_parts_aero1(word(88));
  }
  if (int32_t(word(92)) != -1) {
    rec.set_parts_aero9(word(92));
  }
  rec.set_parts_bonnet(int32_t(word(96)) != -1 ? word(96) : fallback5);
  rec.set_parts_aero3(int32_t(word(100)) != -1 ? word(100) : fallback6);
  rec.set_parts_aero_kit((w & 8) != 0 ? uint32_t(level) : 0);
  return ApplyRecord(base, call, model, frame, 0x8217DD2C);
}

uint64_t SetChoice(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t choice,
                   uint32_t a, uint32_t b, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179310 GarageStage::SetChoice");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  // The record's interior choice `choice` (+3780 + 12 x it): its three
  // words, the index unchecked (wrapping).
  const schema::CarMakersRecordChoice c(
      base, choice * 12 + RecordOf(base, model).interior_choices_address());
  c.set_set(1);
  c.set_index(a);
  c.set_variant(b);
  return ApplyRecord(base, call, model, stack, 0x8217936C);
}

uint64_t SetAero2(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t word,
                  uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821794A8 GarageStage::SetAero2");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  // The record's +3700: SceneModelRecord has no field there (an offset).
  Store(base, model + 3732, word);
  return ApplyRecord(base, call, model, stack, 0x821794E0);
}

uint64_t SetDoorKind(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t word,
                     uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821794F8 GarageStage::SetDoorKind");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  RecordOf(base, model).set_exterior_item1(word);
  return ApplyRecord(base, call, model, stack, 0x82179530);
}

uint64_t SceneInteriorSetRecord(uint8_t* base, const GuestCall& call, uint32_t interior,
                                uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E38E0 SceneInterior::SetRecord");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const schema::SceneInterior si(base, interior);
  const uint32_t first = si.record_address();
  if (!native::Buffers_Memcpy(base, first, record, 120, 0x820E3908)) {
    call(kMemcpy, {first, record, 120}, stack, 0x820E3908);
  }
  const uint32_t second = si.first_set_interior().record_address();
  if (!native::Buffers_Memcpy(base, second, record, 120, 0x820E391C)) {
    return call(kMemcpy, {second, record, 120}, stack, 0x820E391C);
  }
  return second;
}

uint64_t SceneModelApplyInterior(uint8_t* base, const GuestCall& call, uint32_t model,
                                 uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9F40 SceneModel::ApplyInterior");
  const Model m(base, model);
  return SceneInteriorSetRecord(base, call, m.interior(), m.interior_record_address(), stack);
}

uint64_t SetAccessory(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t a,
                      uint32_t b, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821792B0 GarageStage::SetAccessory");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  const Record rec = RecordOf(base, model);
  rec.set_accessory(a);
  rec.set_accessory_variant(b);
  return SceneModelApplyInterior(base, call, model, stack);
}

namespace {

// Wheels::SetRecord(wheels, record) from `frame`: with `poll` its plain
// C++, else its adapter through `call` with `lr`.
uint64_t WheelsSetRecord(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                         uint32_t wheels, uint32_t record, uint32_t frame, uint32_t lr) {
  if (poll != nullptr) {
    FlushOff();
    const uint64_t r3 = load_poll::WheelsSetRecord(base, wheels, record, frame, *poll);
    FlushOff();
    return r3;
  }
  return call(kWheelsSetRecord, {wheels, record}, frame, lr);
}

}  // namespace

uint64_t SceneWheelsSetWord68(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                              uint32_t wheels, uint32_t word, uint32_t stack, uint32_t lr) {
  ITC_NATIVE_RUNNING("sub_820E7410 SceneWheels::SetWord68");
  const schema::SceneWheels sw(base, wheels);
  const uint32_t first = sw.first_set_wheels();
  sw.set_brake_kind(word);
  return WheelsSetRecord(base, call, poll, first, sw.record_address(), stack, lr);
}

uint64_t SceneWheelsSetCaliperTexture(uint8_t* base, const GuestCall& call,
                                      load_poll::Calls* poll, uint32_t wheels,
                                      uint32_t texture, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7428 SceneWheels::SetCaliperTexture");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const schema::SceneWheels sw(base, wheels);
  const uint32_t first = sw.first_set_wheels();
  sw.set_caliper_texture(texture);
  WheelsSetRecord(base, call, poll, first, sw.record_address(), stack, 0x820E7450);
  const uint32_t again = sw.first_set_wheels();
  if (poll != nullptr) {
    FlushOff();
    const uint64_t r3 = load_poll::WheelsSetCaliperTexture(base, again, stack);
    FlushOff();
    return r3;
  }
  return call(kWheelsSetCaliperTexture, {again}, stack, 0x820E7458);
}

uint64_t SetWheelExtras(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                        uint32_t holder, uint32_t a, uint32_t b, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821790D0 GarageStage::SetWheelExtras");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  const Model m(base, model);
  const Record rec = RecordOf(base, model);
  rec.set_wheel_extra(a);
  rec.set_caliper_texture(b);
  SceneWheelsSetWord68(base, call, poll, m.wheels(), a, stack, 0x82179118);
  return SceneWheelsSetCaliperTexture(base, call, poll, m.wheels(), rec.caliper_texture(), stack);
}

uint64_t WheelsSetTireTextures(uint8_t* base, const GuestCall& call, uint32_t wheels,
                               uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D8F18 Wheels::SetTireTextures");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  constexpr uint64_t kEveryShape = ~0ull;
  const schema::CarWheels w(base, wheels);
  call(kSetTexture, {w.front_tire_model_address(), w.front_tire_texture(), kEveryShape}, stack,
       0x820D8F3C);
  return call(kSetTexture, {w.rear_tire_model_address(), w.rear_tire_texture(), kEveryShape},
              stack, 0x820D8F4C);
}

uint64_t SceneWheelsSetTireGrades(uint8_t* base, const GuestCall& call, uint32_t wheels,
                                  uint32_t front, uint32_t rear, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7470 SceneWheels::SetTireGrades");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const schema::SceneWheels sw(base, wheels);
  const uint32_t first = sw.first_set_wheels();
  sw.set_front_tire_grade(front);
  sw.set_rear_tire_grade(rear);
  call(kWheelsSetRecord, {first, sw.record_address()}, stack, 0x820E749C);
  return WheelsSetTireTextures(base, call, sw.first_set_wheels(), stack);
}

uint64_t SetTireGrade(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t which,
                      uint32_t grade, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179130 GarageStage::SetTireGrade");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  const int32_t w = int32_t(which);
  const Model m(base, model);
  const Record rec = RecordOf(base, model);
  if (w == 0 || w == 1) {
    rec.set_front_tire_grade(grade);
  }
  if (w == 0 || w == 2) {
    rec.set_rear_tire_grade(grade);
  }
  return SceneWheelsSetTireGrades(base, call, m.wheels(), rec.front_tire_grade(),
                                  rec.rear_tire_grade(), stack);
}

uint64_t TuningScreenSetTireGrade(uint8_t* base, const GuestCall& call, uint64_t r3,
                                  uint32_t which, uint32_t index, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821CCA38 TuningScreen::SetTireGrade");
  // The grades' table the original builds at its 112-byte frame's +80 is a
  // value since E5 step 6 (only it reads it); SetTireGrade runs from
  // `stack`.
  static constexpr uint32_t kGrades[5] = {0, 1, 1, 2, 2};
  const int32_t w = int32_t(which);
  if (w != 0 && w != 1 && w != 2) {
    return r3;
  }
  // The table read unchecked, as the original (the index's low 30 bits
  // times 4, wrapping): past it, the stack's word where the original reads
  // it.
  const uint32_t grade =
      index < 5 ? kGrades[index] : Load(base, stack - 112 + 80 + (index << 2));
  return SetTireGrade(base, call, Load(base, Globals::stand_holder(base).address), which, grade,
                      stack);
}

uint32_t SecondWheelsPending(uint8_t* base, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179C48 GarageStage::SecondWheelsPending");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)stack;
  return Model(base, StageModel(base, holder)).second_wheels_claimed();
}

uint64_t AddSecondWheels(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179998 GarageStage::AddSecondWheels");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  return scene_models::SceneModelAddSecondWheels(base, call, StageModel(base, holder), stack);
}

namespace {

// SceneModel::ApplyPaint (`wheels` false) or ApplyWheelPaint on `model`
// from `frame` with r3 the model: with `poll` (the load poll's flags on),
// its plain C++ inside its diagnostics' wrap (--debug_hide, nothing then,
// r3 the model as set for the call; --find_per_frame_state, as
// src/debug/frame_state_diff.cpp wrapped it); else its original through
// `call` with `lr`.
uint64_t Paint(uint8_t* base, const GuestCall& call, load_poll::Calls* poll, uint32_t model,
               bool wheels, uint32_t frame, uint32_t lr) {
  const uint32_t function = wheels ? kApplyWheelPaint : kApplyPaint;
  if (poll == nullptr) {
    return call(function, {model}, frame, lr);
  }
  if (debug::IsHidden(function)) {
    return model;
  }
  debug::FrameStateDiff_Before(base);
  FlushOff();
  const uint64_t r3 =
      wheels ? load_poll::SceneModelApplyWheelPaint(base, model, model, frame, *poll)
             : load_poll::SceneModelApplyPaint(base, model, model, frame);
  FlushOff();
  debug::FrameStateDiff_After(base, "draw step part", function);
  return r3;
}

}  // namespace

uint64_t ApplyWheelPaint(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                         uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821799D8 GarageStage::ApplyWheelPaint");
  // The original's 96-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const uint32_t model = StageModel(base, holder);
  return Paint(base, call, poll, model, true, stack, 0x82179A04);
}

uint64_t SetColorPiece(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                       uint32_t holder, uint32_t piece, double f1, double f2, double f3,
                       uint32_t stack) {
  switch (piece) {
    case 0: ITC_NATIVE_RUNNING("sub_8217B078 GarageStage::SetColor0"); break;
    case 1: ITC_NATIVE_RUNNING("sub_8217B108 GarageStage::SetColor1"); break;
    case 2: ITC_NATIVE_RUNNING("sub_8217B198 GarageStage::SetColor2"); break;
    case 3: ITC_NATIVE_RUNNING("sub_8217B228 GarageStage::SetColor3"); break;
    case 4: ITC_NATIVE_RUNNING("sub_8217B2B8 GarageStage::SetColor4"); break;
    default: ITC_NATIVE_RUNNING("sub_8217B348 GarageStage::SetColor5"); break;
  }
  // The float4 the original builds at its 128-byte frame's +80 (stfs) and
  // copies into the record is a value since E5 step 6 (only it reads it);
  // the paint runs from `stack`.
  const uint32_t model = StageModel(base, holder);
  const uint32_t color[4] = {
      std::bit_cast<uint32_t>(float(f1)), std::bit_cast<uint32_t>(float(f2)),
      std::bit_cast<uint32_t>(float(f3)), std::bit_cast<uint32_t>(float(double(Image::one(base))))};
  // The record's paint value 0 to 2 (+64, +80, +96): pieces 3 to 5 write
  // the same body values as 0 to 2, then apply the wheels' paint, which
  // reads +112 on (the original's, kept). Stored word by word, as Copy16.
  const uint32_t paint = Model(base, model).paint_address() + 16 * (piece % 3);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, paint + 4 * i, color[i]);
  }
  const uint32_t lr = 0x8217B0E8 + 0x90 * piece;  // each piece's call + 4
  return Paint(base, call, poll, model, piece >= 3, stack, lr);
}

uint64_t SetColor(uint8_t* base, const GuestCall& call, load_poll::Calls* poll, uint64_t r3,
                  uint32_t which, double f1, double f2, double f3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217C6C8 GarageStage::SetColor");
  // Unsigned: above 5 nothing (r3 left); else the piece's tail call from
  // the same stack pointer.
  if (which > 5) {
    return r3;
  }
  return SetColorPiece(base, call, poll, uint32_t(r3), which, f1, f2, f3, stack);
}

uint64_t StepSecondWheels(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                          uint32_t screen, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821CC890 TuningScreen::StepSecondWheels");
  // The original's 112-byte frame (only its back chain) left out (E5 step
  // 6): its callees run from `stack`.
  const auto holder = [&] { return Load(base, Globals::stand_holder(base).address); };
  const schema::TuningScreen ts(base, screen);
  if (SecondWheelsPending(base, holder(), stack) == 0 && int32_t(ts.second_wheels_step()) == 1) {
    const uint64_t r3 = AddSecondWheels(base, call, holder(), stack);
    ts.set_second_wheels_step(2);
    return r3;
  }
  const uint32_t pending = SecondWheelsPending(base, holder(), stack);
  if (pending != 0 || int32_t(ts.second_wheels_step()) != 2) {
    return pending;
  }
  const uint64_t r3 = ApplyWheelPaint(base, call, poll, holder(), stack);
  ts.set_second_wheels_step(0);
  return r3;
}

}  // namespace stand_models
