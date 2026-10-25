// The course set's lists of models and the model draws as plain C++
// (docs/functions/scenery-lists.md); see scenery_lists.h.

#include "scenery_lists/scenery_lists.h"

#include "base/log.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace scenery_lists {

namespace {

// A List64's node at `index` (the next node at +0, the data slot at +2,
// halfwords), from the list's +4.
uint32_t Node(uint32_t nodes, int16_t index) {
  return uint32_t(int32_t(index) << 2) + nodes;
}

}  // namespace

uint32_t ModelListRelease(uint8_t* base, uint32_t list, const ReleaseCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82147FB8 ModelList::Release");
  uint32_t r3 = list;
  if (int32_t(guest::LoadU32(base, list)) <= 0) {
    return r3;
  }
  const uint32_t nodes = list + 4;
  int32_t i = 0;
  do {
    uint32_t node = Node(nodes, int16_t(guest::LoadU16(base, nodes + 260)));
    for (uint32_t steps = uint32_t(i); steps != 0; steps--) {
      node = Node(nodes, int16_t(guest::LoadU16(base, node)));
    }
    const int32_t slot = int16_t(guest::LoadU16(base, node + 2));
    const uint32_t model = guest::LoadU32(base, uint32_t((slot + 67) << 2) + nodes);
    r3 = calls.release(calls.user, model);
    if (model != 0) {
      r3 = calls.destroy(calls.user, model);
    }
    i++;
  } while (i < int32_t(guest::LoadU32(base, list)));
  return r3;
}

void ResetDrawState(uint8_t* base, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_82146AA8 SceneryModel::ResetDrawState");
  guest::StoreU32(base, model + 436, 0);
  guest::StoreU32(base, model + 432, 1);
  guest::StoreU32(base, model + 440, 0);
}

uint32_t AnimationCurrentFrame(uint8_t* base, uint32_t set, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82146B80 AnimationSet::CurrentFrame");
  const uint32_t offset = guest::LoadU32(base, ((index + 1) << 2) + set);
  const uint32_t animation = offset + ((guest::LoadU32(base, set) + 1) << 2) + set;
  const uint32_t current = guest::LoadU32(base, animation + 4);
  const uint32_t count = guest::LoadU32(base, animation);
  if (int32_t(current) >= int32_t(count)) {
    return 0;
  }
  return ((((current + 2) << 1) + count) << 2) + animation;
}

uint32_t IsBEntryOn(uint8_t* base, int32_t entry) {
  ITC_NATIVE_RUNNING("sub_82147850 ModelList::IsBEntryOn");
  // The flag's entry, bit by bit from 0x1.
  static constexpr int32_t kEntries[12] = {3, 4, 5, 6, 7, 0, 1, 2, 8, 9, 10, 11};
  const uint32_t flags = guest::LoadU32(base, guest::LoadU32(base, kRaceData) + 24);
  uint32_t on = 0;
  for (int bit = 0; bit < 12; bit++) {
    if ((flags >> bit & 1) != 0 && entry == kEntries[bit]) {
      on = 1;
    }
  }
  return on;
}

int32_t CourseLayout(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8228F408 Race::CourseLayout");
  const uint32_t race = guest::LoadU32(base, kRaceData);
  if (race == 0) {
    return -1;
  }
  const uint32_t course = guest::LoadU32(base, race + 4);
  const uint32_t flags = guest::LoadU32(base, race + 20);
  const int32_t mode = int32_t(guest::LoadU32(base, race + kRaceMode));
  const int32_t stage_type =
      int32_t(vehicle_accessors::StagesType(base, race + kStages, 0xFFFFFFFFu));
  if (mode == 8) {
    // The switch's table at 0x82011E58.
    static constexpr int32_t kLayouts[8] = {16, 17, 18, 19, 23, 23, 22, 23};
    return course <= 7 ? kLayouts[course] : -1;
  }
  // sub_8228F484 from here.
  if (mode == 3 || mode == 4 || mode == 7 || stage_type == 1) {
    // The switch's table at 0x82011E40 (8 to 10 none).
    static constexpr int32_t kLayouts[23] = {16, 17, 18, 19, 20, 21, 22, 23, -1, -1, -1, 4,
                                             5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15};
    return course <= 22 ? kLayouts[course] : -1;
  }
  if (mode != 2) {
    return -1;
  }
  if ((flags & 0x20) != 0) {
    return 2;
  }
  if ((flags & 0x4) != 0 && (flags & 0x8) != 0) {
    return 3;
  }
  return (flags & 0x10) != 0 ? 1 : 0;
}

uint32_t IsCEntryOn(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82147958 ModelList::IsCEntryOn");
  const uint32_t race = guest::LoadU32(base, kRaceData);
  const int32_t kind = int32_t(guest::LoadU32(base, race + kRaceKind));
  if (kind == 5) {
    return 0;
  }
  if (kind == 3 ||
      (kind == 4 && vehicle_accessors::StagesType(base, race + kStages, 0xFFFFFFFFu) == 0)) {
    return entry == 24 ? 1 : 0;
  }
  const int32_t layout = CourseLayout(base);
  if (layout == -1) {
    return 0;
  }
  const uint32_t again = guest::LoadU32(base, kRaceData);
  if ((guest::LoadU32(base, again + kRaceFlags) & 0x80) != 0) {
    return 0;
  }
  uint32_t on = 0;
  if (entry <= 23) {
    // The switch's table at 0x82008E80 (case 0 is sub_82147A20).
    static constexpr int32_t kLayouts[24] = {4,  5,  6,  7,  8,  9,  10, 11, 14, 15, 12, 13,
                                             0,  1,  3,  2,  16, 17, 18, 19, 20, 21, 22, 23};
    if (layout == kLayouts[entry]) {
      on = 1;
    }
  }
  if (entry - 16 <= 7) {
    const uint32_t stage_flags = guest::LoadU32(base, again + kStages + 1220);
    if ((stage_flags & 0x20) == 0 && (stage_flags & 0x40) == 0) {
      on = 0;
    }
  }
  return on;
}

uint32_t PointListIsEntryOn(uint8_t* base, int32_t entry) {
  ITC_NATIVE_RUNNING("sub_82147B18 PointList::IsEntryOn");
  const uint32_t race = guest::LoadU32(base, kRaceData);
  if (race == 0) {
    return 0;
  }
  const int32_t course = int32_t(guest::LoadU32(base, race + 4));
  const uint32_t flags = guest::LoadU32(base, race + kRaceFlags);
  const int32_t mode = int32_t(guest::LoadU32(base, race + kRaceMode));
  if ((flags & 0x80) != 0) {
    return 0;
  }
  if (int32_t(guest::LoadU32(base, race + kRaceKind)) == 5) {
    return 0;
  }
  if (mode == 4 || mode == 7) {
    return entry >= 0 && entry <= 7 && course == entry ? 1 : 0;
  }
  if (mode != 3) {
    return 0;
  }
  // Entries 8 to 19: the course each goes with.
  static constexpr int32_t kCourses[12] = {11, 12, 13, 14, 15, 16, 17, 18, 21, 22, 19, 20};
  return entry >= 8 && entry <= 19 && course == kCourses[entry - 8] ? 1 : 0;
}

void SetBillboardPass(uint8_t* base, uint32_t description, uint32_t pass, uint32_t word28) {
  ITC_NATIVE_RUNNING("sub_82148650 PassDescription::SetBillboardPass");
  guest::StoreU32(base, description, pass);
  guest::StoreU32(base, description + 4, word28);
}

uint32_t DepthVertexDeclaration(uint8_t* base, int32_t stream_type) {
  ITC_NATIVE_RUNNING("sub_82589970 VertexDeclarations::ForDepth");
  return guest::LoadU32(base, stream_type == 11 ? 0x8296B8D0u : 0x8296B8CCu);
}

void TextureProviderClearAll(uint8_t* base, uint32_t provider) {
  ITC_NATIVE_RUNNING("sub_82594328 TextureProvider::ClearAll");
  uint32_t slot = guest::LoadU32(base, provider + 8);
  if (slot == guest::LoadU32(base, provider + 12)) {
    return;
  }
  do {
    guest::StoreU32(base, slot, 0);
    slot += 4;
  } while (slot != guest::LoadU32(base, provider + 12));
}

bool TextureProviderSet(uint8_t* base, uint32_t provider, uint32_t slot, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82594718 TextureProvider::Set");
  const uint32_t vector = provider + 4;
  const uint32_t first = guest::LoadU32(base, vector + 4);
  if (first == 0) {
    return false;
  }
  const uint32_t capacity = uint32_t(int32_t(guest::LoadU32(base, vector + 12) - first) >> 2);
  if (slot >= capacity) {
    return false;
  }
  const uint32_t size = uint32_t(int32_t(guest::LoadU32(base, vector + 8) - first) >> 2);
  if (size <= slot) {
    return false;
  }
  guest::StoreU32(base, (slot << 2) + guest::LoadU32(base, vector + 4), value);
  return true;
}

}  // namespace scenery_lists
