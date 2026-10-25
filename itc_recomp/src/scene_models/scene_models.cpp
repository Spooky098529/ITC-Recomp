// The scene models' parts as plain C++ (docs/functions/scene-models.md); see
// scene_models.h.

#include "scene_models/scene_models.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/heap.h"

namespace scene_models {

namespace {

// The image's floats the originals load with lfs.
constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kMinusOne = 0x82001BE0;  // probably -1.0
constexpr uint32_t kHalf = 0x82001BE4;      // 0.5

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

// The 64-byte vinyl layer and sticker records of a car record: `count` at
// each offset.
struct LayerBlock {
  uint32_t offset;
  uint32_t count;
};
constexpr LayerBlock kLayerBlocks[] = {
    {176, 3},  {368, 3},  {560, 6},  {944, 6},  {1328, 6},
    {1712, 6}, {2096, 6}, {2480, 6}, {2864, 6}, {3248, 6},
};

}  // namespace

uint32_t Accessories(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820F0E50 Accessories");
  return guest::LoadU32(base, kAccessories);
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
  uint32_t slot = guest::LoadU32(base, context);
  guest::StoreU8(base, slot + kSoundLoadsDone, uint8_t(guest::LoadU8(base, slot + kSoundLoadsDone) + 1));
  slot = guest::LoadU32(base, context);
  if (guest::LoadU8(base, slot + kSoundLoadsDone) != 2) {
    return;
  }
  guest::StoreU32(base, (slot << 2) + kSoundLoaded, 1);
}

void WheelsSetRecord(uint8_t* base, uint32_t wheels, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820E7378 SceneWheels::SetRecord");
  guest::StoreU32(base, wheels + 16776, value);
}

void SetCarModel(uint8_t* base, uint32_t object, uint32_t car_model) {
  ITC_NATIVE_RUNNING("sub_820E70B8 SetCarModel");
  guest::StoreU32(base, object + 24, car_model);
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
  return c_runtime::Memcpy(base, to, record, kCarRecordBytes);
}

bool CarRecordCopy(uint8_t* base, uint32_t to, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820D7EE8 CarRecord::Copy");
  if (to >= kPhysical || from >= kPhysical || to - from < kCarRecordBytes ||
      from - to < kCarRecordBytes) {
    return false;
  }
  guest::StoreU32(base, to, guest::LoadU32(base, from));
  for (uint32_t offset = 16; offset < 160; offset += 8) {
    CopyDoubleword(base, to + offset, from + offset);
  }
  guest::StoreU32(base, to + 160, guest::LoadU32(base, from + 160));
  guest::StoreU32(base, to + 164, guest::LoadU32(base, from + 164));
  for (const LayerBlock& block : kLayerBlocks) {
    for (uint32_t i = 0; i < block.count; i++) {
      const uint32_t r = block.offset + 64 * i;
      for (uint32_t field : {0u, 4u, 8u}) {
        guest::StoreU32(base, to + r + field, guest::LoadU32(base, from + r + field));
      }
      CopyDoubleword(base, to + r + 16, from + r + 16);
      CopyDoubleword(base, to + r + 24, from + r + 24);
      CopyFloat(base, to + r + 32, from + r + 32);
      CopyDoubleword(base, to + r + 48, from + r + 48);
      CopyDoubleword(base, to + r + 56, from + r + 56);
    }
  }
  if (!c_runtime::Memcpy(base, to + 3632, from + 3632, 48)) {
    return false;
  }
  for (uint32_t offset = 3680; offset <= 3692; offset += 4) {
    guest::StoreU32(base, to + offset, guest::LoadU32(base, from + offset));
  }
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
  for (uint32_t block : {16u, 64u, 112u}) {
    for (uint32_t row = 0; row < 3; row++) {
      CopyFloat(base, record + block + 16 * row + 12, kOne);
    }
  }
  for (const LayerBlock& block : kLayerBlocks) {
    for (uint32_t i = 0; i < block.count; i++) {
      const uint32_t r = record + block.offset + 64 * i;
      CopyFloat(base, r + 28, kOne);
      CopyFloat(base, r + 60, kOne);
    }
  }
}

void Record12Reset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3B38 Record12::Reset");
  CopyFloat(base, record + 4, kMinusOne);
  guest::StoreU32(base, record + 0, 0xFFFFFFFFu);
  CopyFloat(base, record + 8, kMinusOne);
}

void LayerReset(uint8_t* base, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_820D0A20 Layer::Reset");
  guest::StoreU32(base, layer + 4, 0);
  guest::StoreU32(base, layer + 8, 8);
  guest::StoreU32(base, layer + 0, 0xFFFFFFFFu);
  // The offset (0, 0, 0, 1), built in the original's stack and copied as
  // two doublewords.
  CopyFloat(base, layer + 16, kZero);
  CopyFloat(base, layer + 20, kZero);
  CopyFloat(base, layer + 24, kZero);
  CopyFloat(base, layer + 28, kOne);
  CopyFloat(base, layer + 32, kZero);
  for (uint32_t offset = 48; offset < 64; offset += 4) {
    CopyFloat(base, layer + offset, kOne);
  }
}

uint32_t CarRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D0A98 CarRecord::Reset");
  constexpr uint32_t kC1 = 0x82002A10, kC2 = 0x82002A88;
  guest::StoreU32(base, record + 0, 0xFFFFFFFFu);
  // Three blocks of three 16-byte rows (built in the original's stack and
  // copied as doublewords): x, y = 0.0, z = 1.0 or the block's last row's
  // constant, w = 1.0.
  constexpr uint32_t kThirds[3] = {kC1, kC2, kC2};
  for (uint32_t block = 0; block < 3; block++) {
    for (uint32_t row = 0; row < 3; row++) {
      const uint32_t at = record + 16 + 48 * block + 16 * row;
      CopyFloat(base, at + 0, kZero);
      CopyFloat(base, at + 4, kZero);
      CopyFloat(base, at + 8, row == 2 ? kThirds[block] : kOne);
      CopyFloat(base, at + 12, kOne);
    }
  }
  guest::StoreU32(base, record + 160, 0);
  guest::StoreU32(base, record + 164, 0);
  for (uint32_t i = 0; i < 3; i++) {
    LayerReset(base, record + 176 + 64 * i);
    guest::StoreU32(base, record + 184 + 64 * i, 8);
    LayerReset(base, record + 368 + 64 * i);
    guest::StoreU32(base, record + 376 + 64 * i, 0);
  }
  // The stickers' eight groups: the region at +8, and +4 = 1 in the third
  // and the last.
  constexpr uint32_t kRegions[8] = {8, 2, 1, 3, 4, 7, 6, 5};
  for (uint32_t j = 0; j < 6; j++) {
    for (uint32_t group = 0; group < 8; group++) {
      const uint32_t layer = record + 560 + 384 * group + 64 * j;
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
  struct Word {
    uint32_t offset, value;
  };
  constexpr Word kTail[] = {
      {3632, 0},  {3636, 0},  {3640, 1},  {3644, 2},  {3648, 3},  {3652, 29},
      {3656, 36}, {3668, 37}, {3672, 34}, {3676, 35}, {3660, 31}, {3664, 32},
      {3680, 0},  {3684, 0},  {3688, 0},  {3692, 0},
  };
  for (const Word& word : kTail) {
    guest::StoreU32(base, record + word.offset, word.value);
  }
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
  for (uint32_t offset = 0; offset <= 16; offset += 4) {
    guest::StoreU32(base, record + offset, 0);
  }
  InteriorRecordReset(base, record + 24);
  return record + 24;
}

void WheelsRecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D3900 WheelsRecord::Reset");
  constexpr uint32_t kWheelValue = 0x82002568;
  guest::StoreU32(base, record + 0, 1);
  CopyFloat(base, record + 20, kWheelValue);
  guest::StoreU32(base, record + 16, 0xFFFFFFFFu);
  CopyFloat(base, record + 36, kWheelValue);
  guest::StoreU32(base, record + 32, 0xFFFFFFFFu);
  CopyFloat(base, record + 24, kHalf);
  CopyFloat(base, record + 40, kHalf);
  for (uint32_t offset : {4u, 8u, 12u, 28u}) {
    guest::StoreU32(base, record + offset, 0);
  }
  CopyFloat(base, record + 48, kMinusOne);
  CopyFloat(base, record + 52, kMinusOne);
  guest::StoreU32(base, record + 44, 0xFFFFFFFFu);
  CopyFloat(base, record + 60, kMinusOne);
  guest::StoreU32(base, record + 56, 0xFFFFFFFFu);
  CopyFloat(base, record + 64, kMinusOne);
  guest::StoreU32(base, record + 68, 0);
  CopyFloat(base, record + 128, kZero);
  guest::StoreU32(base, record + 124, 0xFFFFFFFFu);
  for (uint32_t i = 0; i < 5; i++) {
    CopyFloat(base, record + 132 + 4 * i, kZero);
    guest::StoreU32(base, record + 72 + 4 * i, 0);
  }
  CopyFloat(base, record + 152, kZero);
  for (uint32_t k = 0; k < 2; k++) {
    CopyFloat(base, record + 100 + 4 * k, kZero);
    guest::StoreU32(base, record + 92 + 4 * k, 0);
    guest::StoreU32(base, record + 116 + 4 * k, 0);
    guest::StoreU32(base, record + 108 + 4 * k, 11);
  }
}

void Pool4RecordReset(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D34E0 Pool4Record::Reset");
  Clear(base, record, 52);
  guest::StoreU32(base, record + 0, 0);
  CopyFloat(base, record + 8, kOne);
  CopyFloat(base, record + 12, kOne);
  guest::StoreU32(base, record + 4, 0);
  CopyFloat(base, record + 16, kOne);
  guest::StoreU32(base, record + 20, 0);
  CopyFloat(base, record + 24, kOne);
  CopyFloat(base, record + 28, kOne);
  CopyFloat(base, record + 32, kOne);
  guest::StoreU32(base, record + 36, 0);
  CopyFloat(base, record + 40, kOne);
  CopyFloat(base, record + 44, kOne);
  CopyFloat(base, record + 48, kOne);
}

uint32_t AllocPages(uint8_t* base, uint32_t out, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_820C3CA0 CarModel::AllocPages");
  const uint32_t pages = memory::AllocPages(size, 4096);
  guest::StoreU32(base, out, pages);
  return pages;
}

}  // namespace scene_models
