// The scene models' parts as plain C++ (docs/functions/scene-models.md); see
// scene_models.h.

#include "scene_models/scene_models.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/heap.h"
#include "schema/accessories.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/scene_models.h"

namespace scene_models {

namespace {

// The image's floats the originals load with lfs (1.0, 0.0, -1.0, 0.5 and
// the others) are SceneModelsGlobals'.
using G = schema::SceneModelsGlobals;

// The originals' lfs and stfs copy of a float.
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, guest::LoadFloat(base, from));
}

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

}  // namespace

uint32_t Accessories(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820F0E50 Accessories");
  return G::accessories(base).address;
}

uint32_t Accessory(uint8_t* base, uint32_t interior) {
  ITC_NATIVE_RUNNING("sub_820D97B8 SceneModel::Accessory");
  const uint32_t set = Accessories(base);
  const uint32_t which = guest::LoadU32(base, interior + 140);
  if (which == 3) {
    return 0;
  }
  const uint32_t entry = guest::LoadU32(base, ((which + 1) << 2) + set);
  return entry == 0 ? 0 : entry + 48;
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
  guest::StoreU32(base, wheels + 16776, value);
}

void SetCarModel(uint8_t* base, uint32_t object, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820E70B8 SetCarModel");
  schema::SceneModelsPartLink(base, object).set_car_model(car_model);
}

void PartsLink(uint8_t* base, uint32_t parts, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820DF1A8 SceneParts::Link");
  guest::StoreU32(base, parts + 24, car_model);
  guest::StoreU32(base, parts + 28, guest::LoadU32(base, car_model + 24) + 3760);
}

void PartsSetRecord(uint8_t* base, uint32_t parts, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820DF1C0 SceneParts::SetRecord");
  guest::StoreU32(base, parts + 37248, value);
}

void InteriorLink(uint8_t* base, uint32_t interior, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820E37D0 SceneInterior::Link");
  guest::StoreU32(base, interior + 24, car_model);
  guest::StoreU32(base, interior + 28, guest::LoadU32(base, car_model + 24) + 3760);
  guest::StoreU32(base, interior + 32, guest::LoadU32(base, car_model + 24) + 4048);
}

void SetWord11452(uint8_t* base, uint32_t object, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820E3658 SetWord11452");
  guest::StoreU32(base, object + 11452, value);
}

bool CarModelSetRecord(uint8_t* base, uint32_t car_model, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3C70 CarModel::SetRecord");
  const uint32_t to = car_model + 32;
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
    for (uint32_t field : {0u, 4u, 8u}) {
      guest::StoreU32(base, to + r + field, guest::LoadU32(base, from + r + field));
    }
    CopyDoubleword(base, to + r + 16, from + r + 16);
    CopyDoubleword(base, to + r + 24, from + r + 24);
    CopyFloat(base, to + r + 32, from + r + 32);
    CopyDoubleword(base, to + r + 48, from + r + 48);
    CopyDoubleword(base, to + r + 56, from + r + 56);
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
  guest::StoreU32(base, to, guest::LoadU32(base, from));
  if (!CarRecordCopy(base, to + 16, from + 16)) {
    return false;
  }
  struct Part {
    uint32_t offset, bytes;
  };
  constexpr Part kParts[] = {{3712, 60}, {3772, 120}, {3892, 144}, {4036, 156}, {4192, 52}};
  for (const Part& part : kParts) {
    if (!c_runtime::Memcpy(base, to + part.offset, from + part.offset, part.bytes)) {
      return false;
    }
  }
  return true;
}

void SceneModelRecordConstruct(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820DB240 SceneModelRecord::SceneModelRecord");
  CarRecordSetW(base, record + 16);
  Record12Reset(base, record + 4036 + 44);
  Record12Reset(base, record + 4036 + 56);
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
    guest::StoreFloat(base, record + layer + 28, G::one(base));
    guest::StoreFloat(base, record + layer + 60, G::one(base));
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
  guest::StoreU32(base, layer + 4, 0);
  guest::StoreU32(base, layer + 8, 8);
  guest::StoreU32(base, layer + 0, 0xFFFFFFFFu);
  // The offset (0, 0, 0, 1), built in the original's stack and copied as
  // two doublewords.
  guest::StoreFloat(base, layer + 16, G::zero(base));
  guest::StoreFloat(base, layer + 20, G::zero(base));
  guest::StoreFloat(base, layer + 24, G::zero(base));
  guest::StoreFloat(base, layer + 28, G::one(base));
  guest::StoreFloat(base, layer + 32, G::zero(base));
  for (uint32_t offset = 48; offset < 64; offset += 4) {
    guest::StoreFloat(base, layer + offset, G::one(base));
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
  // A layer's +8 (the region) and the sticker group's size.
  constexpr uint32_t kRegion = 8, kGroupBytes = 6 * kLayerBytes;
  for (uint32_t i = 0; i < 3; i++) {
    LayerReset(base, r.vinyl_layers_a_address() + kLayerBytes * i);
    guest::StoreU32(base, r.vinyl_layers_a_address() + kLayerBytes * i + kRegion, 8);
    LayerReset(base, r.vinyl_layers_b_address() + kLayerBytes * i);
    guest::StoreU32(base, r.vinyl_layers_b_address() + kLayerBytes * i + kRegion, 0);
  }
  // The stickers' eight groups: the region at +8, and +4 = 1 in the third
  // and the last.
  constexpr uint32_t kRegions[8] = {8, 2, 1, 3, 4, 7, 6, 5};
  for (uint32_t j = 0; j < 6; j++) {
    for (uint32_t group = 0; group < 8; group++) {
      const uint32_t layer = r.stickers_address() + kGroupBytes * group + kLayerBytes * j;
      LayerReset(base, layer);
      if (group == 7) {
        guest::StoreU32(base, layer + 4, 1);
        guest::StoreU32(base, layer + 8, kRegions[group]);
      } else {
        guest::StoreU32(base, layer + 8, kRegions[group]);
        if (group == 2) {
          guest::StoreU32(base, layer + 4, 1);
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
  Clear(base, record, 60);
  for (uint32_t offset = 0; offset < 60; offset += 4) {
    guest::StoreU32(base, record + offset, offset == 52 ? 3 : 0);
  }
}

void InteriorRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3840 InteriorRecord::Reset");
  Clear(base, record, 120);
  struct Word {
    uint32_t offset, value;
  };
  // The original's store order.
  constexpr Word kWords[] = {
      {0, 0},  {4, 0},  {92, 0}, {96, 0}, {100, 0}, {104, 0}, {108, 0}, {112, 3},
      {116, 0}, {8, 0}, {12, 0}, {16, 0}, {20, 0},  {24, 0},  {28, 1},  {32, 0},
      {36, 0}, {40, 2}, {44, 0}, {48, 0}, {52, 3},  {56, 0},  {60, 0},  {64, 4},
      {68, 0}, {72, 1}, {76, 0}, {80, 0}, {84, 1},  {88, 1},
  };
  for (const Word& word : kWords) {
    guest::StoreU32(base, record + word.offset, word.value);
  }
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
  w.set_unknown_72(0);
  w.set_unknown_136(G::zero(base));
  w.set_unknown_76(0);
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
  Clear(base, record, 52);
  guest::StoreU32(base, record + 0, 0);
  guest::StoreFloat(base, record + 8, G::one(base));
  guest::StoreFloat(base, record + 12, G::one(base));
  guest::StoreU32(base, record + 4, 0);
  guest::StoreFloat(base, record + 16, G::one(base));
  guest::StoreU32(base, record + 20, 0);
  guest::StoreFloat(base, record + 24, G::one(base));
  guest::StoreFloat(base, record + 28, G::one(base));
  guest::StoreFloat(base, record + 32, G::one(base));
  guest::StoreU32(base, record + 36, 0);
  guest::StoreFloat(base, record + 40, G::one(base));
  guest::StoreFloat(base, record + 44, G::one(base));
  guest::StoreFloat(base, record + 48, G::one(base));
}

uint32_t SceneModelRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D9810 SceneModelRecord::Reset");
  guest::StoreU32(base, record, 1);
  CarRecordReset(base, record + 16);
  PartsRecordReset(base, record + 3712);
  InteriorRecordReset(base, record + 3772);
  CockpitRecordReset(base, record + 3892);
  WheelsRecordReset(base, record + 4036);
  Pool4RecordReset(base, record + 4192);
  return record + 4192;
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
  const uint32_t scene = m.scene().address;
  if (guest::LoadU32(base, scene + 5048) != 0) {
    return r3;
  }
  const bool cockpit = guest::LoadU32(base, scene + 4) != 0;
  if (m.interior_ready() == 0 || m.car_model().address == 0 ||
      (cockpit ? m.cockpit() : m.draw_enabled()) == 0) {
    return r3;
  }
  // The interior's +36: its first-set interior.
  const uint32_t accessory = Accessory(base, guest::LoadU32(base, m.interior() + 36));
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
  // The car model's +24 (the first set's car model), its accessory node's
  // position at +7216.
  const uint32_t node = guest::LoadU32(base, m.car_model().address + 24) + 7216;
  for (uint32_t w = 0; w < 4; w++) {
    guest::StoreU32(base, frame + 192 + 4 * w, guest::LoadU32(base, node + 4 * w));
  }
  calls.multiply(frame + 144, frame + 144, m.matrix_address(), frame);
  calls.set_anchor(accessory, frame + 144, frame);
  return calls.step(accessory, frame);
}

}  // namespace scene_models
