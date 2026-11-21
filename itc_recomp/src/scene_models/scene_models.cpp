// The scene models' parts as plain C++ (docs/functions/scene-models.md); see
// scene_models.h.

#include "scene_models/scene_models.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/heap.h"
#include "schema/accessories.h"
#include "schema/car_draw.h"
#include "schema/car_makers.h"
#include "schema/car_models.h"
#include "schema/load_poll.h"
#include "schema/scene_models.h"
#include "schema/second_set.h"

namespace scene_models {

namespace {

// The image's floats the originals load with lfs (1.0, 0.0, -1.0, 0.5 and
// the others) are SceneModelsGlobals'.
using G = schema::SceneModelsGlobals;

// A doubleword's ld and std, as two words.
void CopyDoubleword(uint8_t* base, uint32_t to, uint32_t from) {
  const uint32_t high = guest::LoadU32(base, from);
  const uint32_t low = guest::LoadU32(base, from + 4);
  guest::StoreU32(base, to, high);
  guest::StoreU32(base, to + 4, low);
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, 0);
  }
}

// The physical views, where memcpy's override records copies for the
// renderer's mirrors.
constexpr uint32_t kPhysical = 0xA0000000u;

// The 64-byte vinyl layer and sticker records of a car record, in order:
// the three of vinyl_layers_a, the three of vinyl_layers_b, then the 48
// stickers (eight groups of six). `f` gets each one's offset in the record.
constexpr uint32_t kLayerBytes = 64;
template <typename F>
void ForEachLayer(const schema::CarRecord& record, F f) {
  const uint32_t a = record.vinyl_layers_a_address() - record.address;
  const uint32_t b = record.vinyl_layers_b_address() - record.address;
  const uint32_t stickers = record.stickers_address() - record.address;
  for (uint32_t i = 0; i < 3; i++) {
    f(a + kLayerBytes * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    f(b + kLayerBytes * i);
  }
  for (uint32_t i = 0; i < 48; i++) {
    f(stickers + kLayerBytes * i);
  }
}

// The scene model record's interior record (+3772) and wheels record
// (+4036): car-makers' SceneModelRecord names no field at their starts.
uint32_t InteriorRecordOf(const schema::SceneModelRecord& r) {
  return r.interior_engine_grade_address() - 4;
}
uint32_t WheelsRecordOf(const schema::SceneModelRecord& r) {
  return r.wheels_painted_address() - 4;
}

}  // namespace

uint32_t Accessories(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820F0E50 Accessories");
  return G::accessories(base).address;
}

uint32_t Accessory(uint8_t* base, uint32_t interior) {
  ITC_NATIVE_RUNNING("sub_820D97B8 SceneModel::Accessory");
  const schema::Accessories set(base, Accessories(base));
  const uint32_t which = schema::CarInterior(base, interior).accessory();
  if (which == 3) {
    return 0;
  }
  // The set's entries (+4, +8, +12) by the accessory picked.
  const uint32_t entry = guest::LoadU32(base, set.entry_0_address() + (which << 2));
  return entry == 0 ? 0 : schema::AccessoryEntry(base, entry).accessory_address();
}

void OnSoundLoaded(uint8_t* base, uint32_t context) {
  ITC_NATIVE_RUNNING("sub_82175EE0 SceneModel::OnSoundLoaded");
  const schema::SceneModelsSoundContext ctx(base, context);
  const uint32_t arrived = G::sound_files_arrived_address();
  uint32_t slot = ctx.slot();
  guest::StoreU8(base, slot + arrived, uint8_t(guest::LoadU8(base, slot + arrived) + 1));
  slot = ctx.slot();
  if (guest::LoadU8(base, slot + arrived) != 2) {
    return;
  }
  guest::StoreU32(base, (slot << 2) + G::sound_loaded_address(), 1);
}

void WheelsSetRecord(uint8_t* base, uint32_t wheels, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820E7378 SceneWheels::SetRecord");
  schema::SceneWheels(base, wheels).set_traffic_record(value);
}

void SetCarModel(uint8_t* base, uint32_t object, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820E70B8 SetCarModel");
  schema::SceneModelsPartLink(base, object).set_car_model(car_model);
}

void PartsLink(uint8_t* base, uint32_t parts, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820DF1A8 SceneParts::Link");
  const schema::SceneParts p(base, parts);
  p.set_scene_car_model(car_model);
  p.set_skeleton(
      schema::CarModelsSceneCarModel(base, car_model).car_model().skeleton_address());
}

void PartsSetRecord(uint8_t* base, uint32_t parts, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820DF1C0 SceneParts::SetRecord");
  // The point block's scene model record.
  schema::PartsPoints(base, schema::SceneParts(base, parts).points_address())
      .set_scene_model_record(value);
}

void InteriorLink(uint8_t* base, uint32_t interior, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820E37D0 SceneInterior::Link");
  const schema::SceneInterior i(base, interior);
  const schema::CarModelsSceneCarModel model(base, car_model);
  i.set_car_model(car_model);
  // The first set's car model, read twice as the original does.
  i.set_skeleton(model.car_model().skeleton_address());
  i.set_texture_set(model.car_model().texture_providers_address());
}

void SetWord11452(uint8_t* base, uint32_t object, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820E3658 SetWord11452");
  // r3 is an interior's point block (the interior + 160).
  schema::SecondSetInteriorPoints(base, object).set_cockpit(value);
}

bool CarModelSetRecord(uint8_t* base, uint32_t car_model, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3C70 CarModel::SetRecord");
  const uint32_t to = schema::CarModel(base, car_model).record_address();
  if (to >= kPhysical) {
    return false;
  }
  return c_runtime::Memcpy(base, to, record, schema::CarRecord::kSize);
}

bool CarRecordCopy(uint8_t* base, uint32_t to, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820D7EE8 CarRecord::Copy");
  constexpr uint32_t kBytes = schema::CarRecord::kSize;
  if (to >= kPhysical || from >= kPhysical || to - from < kBytes || from - to < kBytes) {
    return false;
  }
  const schema::CarRecord t(base, to), f(base, from);
  t.set_block(f.block());
  // The paint and the wheels' paint (+16 to +159, one run of rows) as
  // doublewords.
  for (uint32_t offset = 0; offset < 144; offset += 8) {
    CopyDoubleword(base, t.paint_address() + offset, f.paint_address() + offset);
  }
  t.set_emblem_choice(f.emblem_choice());
  t.set_emblem_word(f.emblem_word());
  ForEachLayer(t, [&](uint32_t r) {
    const schema::CarModelDecalRecord tl(base, to + r), fl(base, from + r);
    tl.set_index(fl.index());
    tl.set_mirrored(fl.mirrored());
    tl.set_region(fl.region());
    CopyDoubleword(base, tl.offset_xy_address(), fl.offset_xy_address());
    CopyDoubleword(base, tl.offset_zw_address(), fl.offset_zw_address());
    tl.set_rotation(fl.rotation());
    CopyDoubleword(base, tl.scale_address(), fl.scale_address());
    CopyDoubleword(base, tl.scale_address() + 8, fl.scale_address() + 8);
  });
  if (!c_runtime::Memcpy(base, t.plate_address(), f.plate_address(), 48)) {
    return false;
  }
  t.set_selected_texture(f.selected_texture());
  t.set_unknown_3684(f.unknown_3684());
  t.set_door_kind(f.door_kind());
  t.set_unknown_3692(f.unknown_3692());
  return true;
}

bool SceneModelRecordCopy(uint8_t* base, uint32_t to, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820DB290 SceneModelRecord::Copy");
  if (to >= kPhysical || from >= kPhysical || to - from < kSceneModelRecordBytes ||
      from - to < kSceneModelRecordBytes) {
    return false;
  }
  const schema::SceneModelRecord t(base, to), f(base, from);
  t.set_kind(f.kind());
  if (!CarRecordCopy(base, t.block_address(), f.block_address())) {
    return false;
  }
  // The parts', interior's, cockpit's, wheels' and light kit's records, in
  // order.
  const uint32_t kParts[][2] = {
      {t.parts_aero0_address(), f.parts_aero0_address()},
      {InteriorRecordOf(t), InteriorRecordOf(f)},
      {t.cockpit_bonnet_address(), f.cockpit_bonnet_address()},
      {WheelsRecordOf(t), WheelsRecordOf(f)},
      {t.layer0_b_address(), f.layer0_b_address()},
  };
  constexpr uint32_t kBytes[] = {schema::PartsRecord::kSize, schema::InteriorRecord::kSize,
                                 schema::SceneModelsCockpitRecord::kSize,
                                 schema::SceneModelsWheelsRecord::kSize,
                                 schema::LightKitRecord::kSize};
  for (uint32_t i = 0; i < 5; i++) {
    if (!c_runtime::Memcpy(base, kParts[i][0], kParts[i][1], kBytes[i])) {
      return false;
    }
  }
  return true;
}

void SceneModelRecordConstruct(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820DB240 SceneModelRecord::SceneModelRecord");
  const schema::SceneModelRecord r(base, record);
  CarRecordSetW(base, r.block_address());
  // The wheels record's front and rear sizes (+44, +56).
  Record12Reset(base, r.front_size_address());
  Record12Reset(base, r.rear_size_address());
}

void CarRecordSetW(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D45B0 CarRecord::SetW");
  const schema::CarRecord r(base, record);
  // The w of the three blocks of three rows from the paint's start (+16,
  // +64, +112).
  for (uint32_t block = 0; block < 3; block++) {
    for (uint32_t row = 0; row < 3; row++) {
      guest::StoreFloat(base, r.paint_address() + 48 * block + 16 * row + 12, G::one(base));
    }
  }
  ForEachLayer(r, [&](uint32_t layer) {
    const schema::CarModelDecalRecord l(base, record + layer);
    // The offset's w (+28) and the scale's (+60).
    guest::StoreFloat(base, l.offset_zw_address() + 4, G::one(base));
    guest::StoreFloat(base, l.scale_address() + 12, G::one(base));
  });
}

void Record12Reset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3B38 Record12::Reset");
  const schema::SceneModelsWheelSize size(base, record);
  size.set_a(G::minus_one(base));
  size.set_index(0xFFFFFFFFu);
  size.set_b(G::minus_one(base));
}

void LayerReset(uint8_t* base, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_820D0A20 Layer::Reset");
  const schema::CarModelDecalRecord l(base, layer);
  l.set_mirrored(0);
  l.set_region(8);
  l.set_index(0xFFFFFFFFu);
  // The offset (0, 0, 0, 1), built in the original's stack and copied as
  // two doublewords.
  guest::StoreFloat(base, l.offset_xy_address(), G::zero(base));
  guest::StoreFloat(base, l.offset_xy_address() + 4, G::zero(base));
  guest::StoreFloat(base, l.offset_zw_address(), G::zero(base));
  guest::StoreFloat(base, l.offset_zw_address() + 4, G::one(base));
  l.set_rotation(G::zero(base));
  for (uint32_t offset = 0; offset < 16; offset += 4) {
    guest::StoreFloat(base, l.scale_address() + offset, G::one(base));
  }
}

uint32_t CarRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D0A98 CarRecord::Reset");
  const schema::CarRecord r(base, record);
  r.set_block(0xFFFFFFFFu);
  // Three blocks of three 16-byte rows from the paint's start (built in the
  // original's stack and copied as doublewords): x, y = 0.0, z = 1.0 or the
  // block's last row's constant, w = 1.0.
  for (uint32_t block = 0; block < 3; block++) {
    for (uint32_t row = 0; row < 3; row++) {
      const uint32_t at = r.paint_address() + 48 * block + 16 * row;
      guest::StoreFloat(base, at + 0, G::zero(base));
      guest::StoreFloat(base, at + 4, G::zero(base));
      guest::StoreFloat(base, at + 8,
                        row != 2      ? G::one(base)
                        : block == 0 ? G::car_record_z0(base)
                                     : G::car_record_z1(base));
      guest::StoreFloat(base, at + 12, G::one(base));
    }
  }
  r.set_emblem_choice(0);
  r.set_emblem_word(0);
  // The sticker group's size.
  constexpr uint32_t kGroupBytes = 6 * kLayerBytes;
  for (uint32_t i = 0; i < 3; i++) {
    const schema::CarModelDecalRecord a(base, r.vinyl_layers_a_address() + kLayerBytes * i);
    LayerReset(base, a.address);
    a.set_region(8);
    const schema::CarModelDecalRecord b(base, r.vinyl_layers_b_address() + kLayerBytes * i);
    LayerReset(base, b.address);
    b.set_region(0);
  }
  // The stickers' eight groups: the region at +8, and +4 = 1 in the third
  // and the last.
  constexpr uint32_t kRegions[8] = {8, 2, 1, 3, 4, 7, 6, 5};
  for (uint32_t j = 0; j < 6; j++) {
    for (uint32_t group = 0; group < 8; group++) {
      const schema::CarModelDecalRecord layer(
          base, r.stickers_address() + kGroupBytes * group + kLayerBytes * j);
      LayerReset(base, layer.address);
      if (group == 7) {
        layer.set_mirrored(1);
        layer.set_region(kRegions[group]);
      } else {
        layer.set_region(kRegions[group]);
        if (group == 2) {
          layer.set_mirrored(1);
        }
      }
    }
  }
  // The plate's twelve words, in the original's store order.
  struct Word {
    uint32_t offset, value;
  };
  constexpr Word kPlate[] = {
      {0, 0},   {4, 0},   {8, 1},   {12, 2},  {16, 3},  {20, 29},
      {24, 36}, {36, 37}, {40, 34}, {44, 35}, {28, 31}, {32, 32},
  };
  for (const Word& word : kPlate) {
    guest::StoreU32(base, r.plate_address() + word.offset, word.value);
  }
  r.set_selected_texture(0);
  r.set_unknown_3684(0);
  r.set_door_kind(0);
  r.set_unknown_3692(0);
  return 32;
}

void PartsRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3558 PartsRecord::Reset");
  const schema::PartsRecord p(base, record);
  Clear(base, record, schema::PartsRecord::kSize);
  p.set_unknown_0(0);
  p.set_unknown_4(0);
  p.set_unknown_8(0);
  p.set_doors_body_kind(0);
  p.set_unknown_16(0);
  p.set_unknown_20(0);
  p.set_unknown_24(0);
  p.set_door_panel_kind(0);
  p.set_headlight_color(0);
  p.set_unknown_36(0);
  p.set_unknown_40(0);
  p.set_unknown_44(0);
  p.set_unknown_48(0);
  p.set_unknown_52(3);
  p.set_unknown_56(0);
}

void InteriorRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3840 InteriorRecord::Reset");
  const schema::InteriorRecord i(base, record);
  Clear(base, record, schema::InteriorRecord::kSize);
  // The original's store order.
  i.set_speedometer(0);
  i.set_tachometer(0);
  i.set_steering_wheel(0);
  i.set_seat_texture(0);
  i.set_seat_model(0);
  i.set_roll_cage(0);
  i.set_strut_bar(0);
  i.set_accessory(3);
  i.set_unknown_116(0);
  i.set_gauge0_present(0);
  i.set_gauge0_kind(0);
  i.set_unknown_16(0);
  i.set_gauge1_present(0);
  i.set_gauge1_kind(0);
  i.set_unknown_28(1);
  i.set_gauge2_present(0);
  i.set_gauge2_kind(0);
  i.set_unknown_40(2);
  i.set_gauge3_present(0);
  i.set_gauge3_kind(0);
  i.set_unknown_52(3);
  i.set_gauge4_present(0);
  i.set_gauge4_kind(0);
  i.set_unknown_64(4);
  i.set_gauge5_present(0);
  i.set_gauge5_kind(1);
  i.set_unknown_76(0);
  i.set_gauge6_present(0);
  i.set_gauge6_kind(1);
  i.set_unknown_88(1);
}

uint32_t CockpitRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D34B8 CockpitRecord::Reset");
  const schema::SceneModelsCockpitRecord cockpit(base, record);
  cockpit.set_bonnet(0);
  cockpit.set_aero_kit(0);
  cockpit.set_aero6(0);
  cockpit.set_aero7(0);
  cockpit.set_aero9(0);
  InteriorRecordReset(base, cockpit.interior_record_address());
  return cockpit.interior_record_address();
}

void WheelsRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3900 WheelsRecord::Reset");
  const schema::SceneModelsWheelsRecord w(base, record);
  w.set_kind(1);
  w.set_front_f20(G::wheel_default(base));
  w.set_front_wheel(0xFFFFFFFFu);
  w.set_rear_f36(G::wheel_default(base));
  w.set_rear_wheel(0xFFFFFFFFu);
  w.set_front_f24(G::half(base));
  w.set_rear_f40(G::half(base));
  w.set_painted(0);
  w.set_morph_each_draw(0);
  w.set_unknown_12(0);
  w.set_unknown_28(0);
  w.set_front_size_a(G::minus_one(base));
  w.set_front_size_b(G::minus_one(base));
  w.set_front_size_index(0xFFFFFFFFu);
  w.set_rear_size_a(G::minus_one(base));
  w.set_rear_size_index(0xFFFFFFFFu);
  w.set_rear_size_b(G::minus_one(base));
  w.set_brake_kind(0);
  w.set_unknown_128(G::zero(base));
  w.set_unknown_124(0xFFFFFFFFu);
  w.set_unknown_132(G::zero(base));
  w.set_front_index_add(0);
  w.set_unknown_136(G::zero(base));
  w.set_rear_index_add(0);
  w.set_unknown_140(G::zero(base));
  w.set_unknown_80(0);
  w.set_unknown_144(G::zero(base));
  w.set_caliper_a(0);
  w.set_unknown_148(G::zero(base));
  w.set_caliper_texture(0);
  w.set_unknown_152(G::zero(base));
  w.set_front_brake_turn(G::zero(base));
  w.set_unknown_92(0);
  w.set_unknown_116(0);
  w.set_front_brake_size(11);
  w.set_rear_brake_turn(G::zero(base));
  w.set_unknown_96(0);
  w.set_unknown_120(0);
  w.set_rear_brake_size(11);
}

void Pool4RecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D34E0 Pool4Record::Reset");
  const schema::LightKitRecord k(base, record);
  Clear(base, record, schema::LightKitRecord::kSize);
  k.set_texture(0);
  k.set_front_on(G::one(base));
  k.set_front_fade(G::one(base));
  k.set_front_mode(0);
  k.set_front_rate(G::one(base));
  k.set_sides_mode(0);
  k.set_sides_on(G::one(base));
  k.set_sides_fade(G::one(base));
  k.set_sides_rate(G::one(base));
  k.set_rear_mode(0);
  k.set_rear_on(G::one(base));
  k.set_rear_fade(G::one(base));
  k.set_rear_rate(G::one(base));
}

uint32_t SceneModelRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D9810 SceneModelRecord::Reset");
  const schema::SceneModelRecord r(base, record);
  r.set_kind(1);
  CarRecordReset(base, r.block_address());
  PartsRecordReset(base, r.parts_aero0_address());
  InteriorRecordReset(base, InteriorRecordOf(r));
  CockpitRecordReset(base, r.cockpit_bonnet_address());
  WheelsRecordReset(base, WheelsRecordOf(r));
  Pool4RecordReset(base, r.layer0_b_address());
  return r.layer0_b_address();
}

uint32_t AllocPages(uint8_t* base, uint32_t out, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_820C3CA0 CarModel::AllocPages");
  const uint32_t pages = memory::AllocPages(size, 4096);
  guest::StoreU32(base, out, pages);
  return pages;
}


std::optional<uint64_t> ModelUpdate(uint8_t* base, uint32_t model, uint32_t sp,
                                    const ModelUpdateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820DA180 SceneModel::Update");
  const uint32_t frame = sp - 240;
  std::optional<uint64_t> r3;
  const schema::SceneModel m(base, model);
  const int32_t count = int32_t(m.build_delay());
  if (count > 0) {
    const uint32_t pending = m.build_pending();
    m.set_build_delay(uint32_t(count - 1));
    if (pending != 0 && count - 1 == 0) {
      guest::StoreU32(base, frame, sp);  // the back chain, for Build's guest call
      r3 = calls.build(model, frame);
      m.set_build_pending(0);
    }
  }
  const schema::CarScene scene = m.scene();
  if (scene.blink_timers_held() != 0) {
    return r3;
  }
  const bool cockpit = scene.model_set() != 0;
  if (m.interior_ready() == 0 || m.car_model().address == 0 ||
      (cockpit ? m.cockpit() : m.draw_enabled()) == 0) {
    return r3;
  }
  // The scene interior's first-set interior (scene-loaders' Interior, read
  // here for its accessory as car-models' CarInterior).
  const uint32_t accessory = Accessory(
      base,
      schema::SceneInterior(base, m.interior()).first_set_interior<schema::CarInterior>().address);
  if (accessory == 0) {
    return 0;
  }
  // Never seen in play: the identity built at +80 and copied to +144 (the
  // vupkd3d128 and vpermwi128 rows), row 3 the node's position.
  guest::StoreU32(base, frame, sp);
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      const uint32_t value = row == column ? 0x3F800000u : 0;
      guest::StoreU32(base, frame + 80 + 16 * row + 4 * column, value);
      guest::StoreU32(base, frame + 144 + 16 * row + 4 * column, value);
    }
  }
  // The first set's car model's accessory node's position.
  const uint32_t node = m.car_model().car_model().accessory_position_address();
  for (uint32_t w = 0; w < 4; w++) {
    guest::StoreU32(base, frame + 192 + 4 * w, guest::LoadU32(base, node + 4 * w));
  }
  calls.multiply(frame + 144, frame + 144, m.matrix_address(), frame);
  calls.set_anchor(accessory, frame + 144, frame);
  return calls.step(accessory, frame);
}

}  // namespace scene_models
