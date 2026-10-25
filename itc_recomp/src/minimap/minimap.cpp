// The race's minimap's parts as plain C++ (docs/functions/minimap.md); see
// minimap.h.

#include "minimap/minimap.h"

#include "base/log.h"
#include "c_runtime/exit.h"
#include "c_runtime/strings.h"
#include "command_list/command_list.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace minimap {

namespace {

// The image's 1.0 (0x82000A30), stored by the originals' stfs.
constexpr uint32_t kOne = 0x82000A30;

// The originals' lfs and stfs copy of a float.
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, guest::LoadFloat(base, from));
}

// An 80-slot list's fields.
constexpr uint32_t kFree = 320, kHead = 324, kCount = 328, kData = 336;

int32_t LoadS16(uint8_t* base, uint32_t address) {
  return int16_t(guest::LoadU16(base, address));
}

// Node `index` (sign-extended, as the originals' lha) of the list.
uint32_t NodeAddress(uint32_t list, int32_t index) { return uint32_t(index) * 4 + list; }

// The node `steps` steps from the head (32 bits, as the originals walk it):
// the head's node, then each node's next.
uint32_t NodeAt(uint8_t* base, uint32_t list, uint32_t steps) {
  uint32_t node = NodeAddress(list, LoadS16(base, list + kHead));
  for (uint32_t n = steps; n != 0; n--) {
    node = NodeAddress(list, LoadS16(base, node));
  }
  return node;
}

// The node's data slot's address (its signed halfword +2, 64 bytes a slot).
uint32_t DataAt(uint8_t* base, uint32_t list, uint32_t node) {
  return uint32_t(LoadS16(base, node + 2)) * 64 + list + kData;
}

// An identity's row `row` (the originals' vupkd3d128 and vpermwi128): 1.0
// in word `row`, 0 in the others.
void StoreIdentityRow(uint8_t* base, uint32_t address, uint32_t row) {
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, address + 4 * i, i == row ? 0x3F800000u : 0u);
  }
}

}  // namespace

uint32_t RaceFindPlayerEntry(uint8_t* base, uint32_t manager, uint32_t which) {
  ITC_NATIVE_RUNNING("sub_82170608 Race::FindPlayerEntry");
  const int32_t count = int32_t(guest::LoadU32(base, manager + 2368));
  const uint32_t flag = which != 0 ? 0x4 : 0x8;
  for (int32_t i = 0; i < count; i++) {
    if ((guest::LoadU32(base, manager + 4 + 28 * uint32_t(i)) & flag) != 0) {
      return manager + 4 + 28 * uint32_t(i);
    }
  }
  return 0;
}

uint32_t RaceRacerVehicle(uint8_t* base, uint32_t racer) {
  ITC_NATIVE_RUNNING("sub_82170578 Race::RacerVehicle");
  const uint32_t manager = guest::LoadU32(base, kRaceManager);
  if (manager == 0) {
    return 0;
  }
  const uint32_t record = guest::LoadU32(base, (racer + 577) * 4 + manager);
  return vehicle_accessors::VehicleGet(base, guest::LoadU32(base, record + 20));
}

void SetListPoint(uint8_t* base, uint32_t list, uint32_t out, uint32_t entry, uint32_t point,
                  uint32_t part) {
  ITC_NATIVE_RUNNING("sub_82147D18 SetList::Point");
  const uint32_t from =
      guest::LoadU32(base, (entry + 132) * 4 + list) + ((point * 4 + part) << 4);
  const float x = guest::LoadFloat(base, from + 0);
  const float y = guest::LoadFloat(base, from + 4);
  const float z = guest::LoadFloat(base, from + 8);
  const uint32_t w = guest::LoadU32(base, kOne);
  guest::StoreFloat(base, out + 0, x);
  guest::StoreFloat(base, out + 4, y);
  guest::StoreFloat(base, out + 8, z);
  guest::StoreU32(base, out + 12, w);
}

std::optional<int32_t> RacerMarkerType(uint8_t* base, uint32_t entry, int32_t branch) {
  ITC_NATIVE_RUNNING("sub_820F2D28 Minimap::RacerMarkerType");
  int32_t type;
  const uint32_t record = guest::LoadU32(base, kRaceData) + 48 +
                          3936 * guest::LoadU32(base, entry + 24);
  if ((guest::LoadU32(base, entry) & 0x10) != 0) {
    type = 1;
  } else if (int32_t id = int32_t(guest::LoadU32(base, record + 3912)); id == -1) {
    type = 1;
  } else {
    // The id's three halfwords in player 0's save data.
    const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
    const uint32_t item = data_tables::RecordsItem(base, uint32_t(id));
    const uint32_t counts = (uint32_t(id) + 8746) * 12 + save;
    type = -1;
    if (guest::LoadU16(base, counts + 2) != 0) {
      type = 4;
      if (guest::LoadU16(base, item + 88) == 3) {
        return std::nullopt;  // sub_820F2CA8, never seen in play
      }
    }
    const int32_t second = LoadS16(base, counts + 2);
    if (second == 0) {
      type = 3;
    }
    const int32_t sum = int16_t(LoadS16(base, counts + 4) + LoadS16(base, counts + 0) + second);
    if (sum <= 0) {
      type = 2;
    }
  }
  const uint16_t section = guest::LoadU16(base, entry + 8);
  if (int16_t(section) <= -1 || LoadS16(base, entry + 10) <= -1) {
    return -1;
  }
  if (type == 1) {
    return 1;
  }
  const uint32_t sections = guest::LoadU32(base, guest::LoadU32(base, kRoute) + 12);
  const int32_t own = guest::LoadU8(base, guest::LoadU32(base, sections + 4 * section) + 3);
  // The branches the player's shows (the original's switch on branch - 1;
  // case 1 is sub_820F2E68 in the generated code), else only its own.
  auto shows = [&]() {
    auto any = [&](std::initializer_list<int32_t> branches) {
      for (int32_t b : branches) {
        if (own == b) {
          return true;
        }
      }
      return false;
    };
    switch (uint32_t(branch - 1) > 13 ? 0 : branch) {
      case 1: return any({1, 5});
      case 2: return any({2, 6});
      case 3: return any({3, 6});
      case 4: return any({4, 5});
      case 5: return any({1, 4, 5});
      case 6: return any({2, 3, 6});
      case 7: return any({7, 14});
      case 8: return any({8, 13});
      case 13: return any({3, 8, 13});
      case 14: return any({4, 7, 14});
      default: return branch == own;
    }
  };
  return shows() ? type : -1;
}

bool Scaling(uint8_t* base, uint32_t out, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820F2EF8 Minimap::Scaling");
  if (out >= 0xA0000000u) {
    return false;
  }
  // The identity made on the original's stack, then memcpy'd to out.
  for (uint32_t row = 0; row < 4; row++) {
    StoreIdentityRow(base, out + 16 * row, row);
  }
  CopyFloat(base, out + 0, vector + 0);
  CopyFloat(base, out + 20, vector + 4);
  CopyFloat(base, out + 40, vector + 8);
  return true;
}

void TriangleInit(uint8_t* base, uint32_t triangle) {
  ITC_NATIVE_RUNNING("sub_820F4040 Triangle::Init");
  const uint32_t one = guest::LoadU32(base, kOne);
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    guest::StoreU32(base, triangle + offset, one);
  }
  guest::StoreU32(base, triangle + 64, 0);
  guest::StoreU32(base, triangle + 68, 0);
  guest::StoreU32(base, triangle + 72, 2);
  for (uint32_t row = 0; row < 4; row++) {
    StoreIdentityRow(base, (triangle + 16 * row) & ~0xFu, row);
  }
  for (uint32_t offset : {92u, 108u, 124u}) {
    guest::StoreU32(base, triangle + offset, one);
  }
}

void List80Link(uint8_t* base, uint32_t list, int16_t position) {
  ITC_NATIVE_RUNNING("sub_820F40C0 List80::Link");
  uint32_t after;
  if (position == 0) {
    after = list + kHead;
  } else {
    int32_t index = LoadS16(base, list + kHead);
    uint32_t n = uint32_t(int32_t(position));
    for (;;) {
      after = NodeAddress(list, index);
      if (--n == 0) {
        break;
      }
      index = LoadS16(base, after);
    }
  }
  const uint32_t free_node = NodeAddress(list, LoadS16(base, list + kFree));
  const uint16_t free_index = guest::LoadU16(base, list + kFree);
  guest::StoreU16(base, list + kFree, guest::LoadU16(base, free_node));
  guest::StoreU16(base, free_node, free_index);
  guest::StoreU16(base, free_node, guest::LoadU16(base, after));
  guest::StoreU16(base, after, free_index);
  guest::StoreU16(base, list + kCount, guest::LoadU16(base, list + kCount) + 1);
}

uint32_t List80Insert(uint8_t* base, uint32_t list, uint32_t matrix, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F5238 List80::Insert");
  List80Link(base, list, int16_t(position));
  const uint32_t to = DataAt(base, list, NodeAt(base, list, position));
  matrix_math::Copy(base, to, matrix);
  interpolation::BlendState_OnCopy(base, to, matrix, 0x820F529C);
  return to;
}

bool List80Copy(uint8_t* base, uint32_t out, uint32_t list, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F51A8 List80::Copy");
  if (out >= 0xA0000000u) {
    return false;
  }
  const uint32_t from = DataAt(base, list, NodeAt(base, list, position));
  const uint32_t one = guest::LoadU32(base, kOne);
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    guest::StoreU32(base, out + offset, one);
  }
  return c_runtime::Memcpy(base, out, from, 64);
}

std::optional<uint32_t> RecordTransform(uint8_t* base, uint32_t pass, uint32_t primitive,
                                        uint32_t product) {
  ITC_NATIVE_RUNNING("sub_8217AEA8 Primitive::RecordTransform");
  const uint32_t list = guest::LoadU32(base, kTransformList);
  const uint32_t used = guest::LoadU32(base, list + 12);
  if (used + 5 < used || used + 5 >= guest::LoadU32(base, list + 8)) {
    return std::nullopt;
  }
  // The original's 1.0 stores before the multiply (overwritten by it).
  const uint32_t one = guest::LoadU32(base, kOne);
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    guest::StoreU32(base, product + offset, one);
  }
  matrix_math::Multiply(base, product, primitive, pass + 144);
  interpolation::BlendState_OnMultiply(base, product, primitive, pass + 144, 0x8217AEDC);
  const uint32_t header = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, header + 4, 0);
  guest::StoreU32(base, header + 0, 7);
  guest::StoreU32(base, header + 8, 1);
  guest::StoreU8(base, header + 12, 0);
  guest::StoreU8(base, header + 13, 4);
  uint32_t slot = 0;
  for (uint32_t row = 0; row < 4; row++) {
    slot = command_list::AllocateSlots(base, list, 1);
    for (uint32_t i = 0; i < 4; i++) {
      CopyFloat(base, slot + 4 * i, product + 16 * row + 4 * i);
    }
  }
  return slot;
}

TriangleRecording TriangleRecord(uint8_t* base, uint32_t triangle) {
  ITC_NATIVE_RUNNING("sub_8217DAA0 Triangle::Record");
  const uint32_t kind = guest::LoadU32(base, triangle + 64);
  return kind == 0 ? TriangleRecording::kFlat
                   : kind == 1 ? TriangleRecording::kOther : TriangleRecording::kNone;
}

std::optional<uint32_t> TriangleRecordFlat(uint8_t* base, uint32_t triangle, uint32_t pass,
                                           uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8217C370 Triangle::RecordFlat");
  const uint32_t list = guest::LoadU32(base, kTriangleLists + 4);
  // The header, the transform's five, [13], the vertices' header and five,
  // and [15]: 14 slots, with no chunk open on any list.
  const uint32_t used = guest::LoadU32(base, list + 12);
  if (guest::LoadU8(base, list + 52) != 0 ||
      guest::LoadU32(base, command_list::kOpenChunks) != 0 || used + 14 < used ||
      used + 14 >= guest::LoadU32(base, list + 8)) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  const uint32_t header = command_list::AllocateSlots(base, list, 1);
  RecordTransform(base, pass, triangle, frame - kTransformProduct);
  const uint32_t slot = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, slot + 0, 13);
  guest::StoreU32(base, slot + 4, 4);
  guest::StoreU32(base, slot + 8, 0);
  guest::StoreU32(base, slot + 12, 0);
  uint32_t vertex = command_list::AllocateVertices(base, list, 3, true);
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t from = triangle + 80 + 16 * i;
    CopyFloat(base, vertex + 0, from + 0);
    CopyFloat(base, vertex + 4, from + 4);
    CopyFloat(base, vertex + 8, from + 8);
    const uint32_t color = (guest::LoadU32(base, from + 60) << 24) |
                           (uint32_t(guest::LoadU8(base, from + 51)) << 16) |
                           (uint32_t(guest::LoadU8(base, from + 55)) << 8) |
                           guest::LoadU8(base, from + 59);
    guest::StoreU32(base, vertex + 12, color);
    vertex += 24;
  }
  guest::StoreU32(base, command_list::AllocateSlots(base, list, 1), 15);
  command_list::CloseChunk(base, list);
  const int32_t slots = command_list::ChunkSlots(base, list);
  guest::StoreU16(base, header + 0, 0);
  guest::StoreU16(base, header + 2, 0);
  guest::StoreU32(base, header + 4, 0);
  guest::StoreU32(base, header + 12, 0);
  guest::StoreU32(base, header + 8, uint32_t(slots - 1));
  return header;
}

// --- The markers' draws ---

namespace {

// The floats each marker's table is filled from, word by word (the
// originals' lfs and stfs, in their order: by offset).
constexpr uint32_t kCarMarkerSources[] = {
    0x820067D8, 0x82000A34, 0x820067D4, 0x82000A30, 0x820067D0, 0x82000A34,
    0x820067D4, 0x82000A30, 0x82000A34, 0x82000A34, 0x820067CC, 0x82000A30,
    0x820067D8, 0x82000A34, 0x820067D4, 0x82000A30, 0x820067D0, 0x82000A34,
    0x820067D4, 0x82000A30, 0x820067C8, 0x82000A34, 0x820067C4, 0x82000A30,
    0x820067C8, 0x82000A34, 0x820067C4, 0x82000A30, 0x820067C0, 0x82000A34,
    0x820067C4, 0x82000A30, 0x820067D8, 0x82000A34, 0x820067D4, 0x82000A30,
    0x820067D0, 0x82000A34, 0x820067D4, 0x82000A30, 0x82000A34, 0x82000A34,
    0x820067CC, 0x82000A30, 0x82000A34, 0x82000A34, 0x820067BC, 0x82000A30,
    0x82000A34, 0x82000A34, 0x820067BC, 0x82000A30, 0x820067C8, 0x82000A34,
    0x820067C4, 0x82000A30, 0x820067D0, 0x82000A34, 0x820067D4, 0x82000A30,
    0x82000A34, 0x82000A34, 0x820067CC, 0x82000A30, 0x820067D8, 0x82000A34,
    0x820067D4, 0x82000A30, 0x820067C0, 0x82000A34, 0x820067C4, 0x82000A30,
    0x820067C0, 0x82000A34, 0x820067C4, 0x82000A30, 0x82000A34, 0x82000A34,
    0x820067BC, 0x82000A30, 0x82000A34, 0x82000A34, 0x820067CC, 0x82000A30,
};
constexpr uint32_t kPointMarkerSources[] = {
    0x82006800, 0x82000A34, 0x820067FC, 0x82000A30, 0x820067F8, 0x82000A34,
    0x82000A34, 0x82000A30, 0x82006800, 0x82000A34, 0x82006800, 0x82000A30,
    0x82006800, 0x82000A34, 0x82006800, 0x82000A30, 0x82000A34, 0x82000A34,
    0x820067F8, 0x82000A30, 0x82006800, 0x82000A34, 0x820067FC, 0x82000A30,
    0x82002C1C, 0x82000A34, 0x82000A34, 0x82000A30, 0x820067FC, 0x82000A34,
    0x820067FC, 0x82000A30, 0x82000A34, 0x82000A34, 0x82002C1C, 0x82000A30,
    0x82000A34, 0x82000A34, 0x82002C1C, 0x82000A30, 0x820067FC, 0x82000A34,
    0x82006800, 0x82000A30, 0x82002C1C, 0x82000A34, 0x82000A34, 0x82000A30,
    0x82000A34, 0x82000A34, 0x82002C1C, 0x82000A30, 0x82006800, 0x82000A34,
    0x820067FC, 0x82000A30, 0x82000A34, 0x82000A34, 0x820067F8, 0x82000A30,
    0x82000A34, 0x82000A34, 0x820067F8, 0x82000A30, 0x820067FC, 0x82000A34,
    0x82006800, 0x82000A30, 0x82000A34, 0x82000A34, 0x82002C1C, 0x82000A30,
    0x820067F4, 0x82000A34, 0x82000A34, 0x82000A30, 0x820067F0, 0x82000A34,
    0x820067EC, 0x82000A30, 0x820067FC, 0x82000A34, 0x820067FC, 0x82000A30,
    0x820067FC, 0x82000A34, 0x820067FC, 0x82000A30, 0x82002C1C, 0x82000A34,
    0x82000A34, 0x82000A30, 0x820067F4, 0x82000A34, 0x82000A34, 0x82000A30,
    0x820067F0, 0x82000A34, 0x820067EC, 0x82000A30, 0x82000A34, 0x82000A34,
    0x820067F4, 0x82000A30, 0x82000A34, 0x82000A34, 0x82002C1C, 0x82000A30,
    0x82000A34, 0x82000A34, 0x82002C1C, 0x82000A30, 0x820067FC, 0x82000A34,
    0x820067FC, 0x82000A30, 0x820067F0, 0x82000A34, 0x820067EC, 0x82000A30,
    0x82000A34, 0x82000A34, 0x820067F4, 0x82000A30, 0x820067E8, 0x82000A34,
    0x820067EC, 0x82000A30, 0x82006800, 0x82000A34, 0x820067FC, 0x82000A30,
    0x82006800, 0x82000A34, 0x820067FC, 0x82000A30, 0x82000A34, 0x82000A34,
    0x82002C1C, 0x82000A30, 0x82000A34, 0x82000A34, 0x820067F4, 0x82000A30,
    0x820067E8, 0x82000A34, 0x820067EC, 0x82000A30, 0x820067E4, 0x82000A34,
    0x82000A34, 0x82000A30, 0x820067F8, 0x82000A34, 0x82000A34, 0x82000A30,
    0x820067F8, 0x82000A34, 0x82000A34, 0x82000A30, 0x82006800, 0x82000A34,
    0x820067FC, 0x82000A30, 0x820067E8, 0x82000A34, 0x820067EC, 0x82000A30,
    0x820067E4, 0x82000A34, 0x82000A34, 0x82000A30, 0x820067E8, 0x82000A34,
    0x820067E0, 0x82000A30, 0x82006800, 0x82000A34, 0x82006800, 0x82000A30,
    0x82006800, 0x82000A34, 0x82006800, 0x82000A30, 0x820067F8, 0x82000A34,
    0x82000A34, 0x82000A30, 0x820067E4, 0x82000A34, 0x82000A34, 0x82000A30,
    0x820067E8, 0x82000A34, 0x820067E0, 0x82000A30, 0x82000A34, 0x82000A34,
    0x820067DC, 0x82000A30, 0x82000A34, 0x82000A34, 0x820067F8, 0x82000A30,
    0x82000A34, 0x82000A34, 0x820067F8, 0x82000A30, 0x82006800, 0x82000A34,
    0x82006800, 0x82000A30, 0x820067E8, 0x82000A34, 0x820067E0, 0x82000A30,
    0x82000A34, 0x82000A34, 0x820067DC, 0x82000A30, 0x820067F0, 0x82000A34,
    0x820067E0, 0x82000A30, 0x820067FC, 0x82000A34, 0x82006800, 0x82000A30,
    0x820067FC, 0x82000A34, 0x82006800, 0x82000A30, 0x82000A34, 0x82000A34,
    0x820067F8, 0x82000A30, 0x82000A34, 0x82000A34, 0x820067DC, 0x82000A30,
    0x820067F0, 0x82000A34, 0x820067E0, 0x82000A30, 0x820067F4, 0x82000A34,
    0x82000A34, 0x82000A30, 0x82002C1C, 0x82000A34, 0x82000A34, 0x82000A30,
    0x82002C1C, 0x82000A34, 0x82000A34, 0x82000A30, 0x820067FC, 0x82000A34,
    0x82006800, 0x82000A30, 0x820067F0, 0x82000A34, 0x820067E0, 0x82000A30,
};

struct Marker {
  uint32_t table;  // 48 bytes a triangle: three (x, y, z, w) vertices
  uint32_t guard;  // bit 0: the table made
  uint32_t clean_up;
  const uint32_t* sources;
  uint32_t words;
  uint32_t triangles;
  uint32_t colored;  // triangles 0 to colored - 1 in the color, the rest black
};
constexpr Marker kCarMarker = {0x826C5520, 0x826C5670, 0x825DAEF8, kCarMarkerSources,
                               std::size(kCarMarkerSources), 7, 1};
constexpr Marker kPointMarker = {0x826C5680, 0x826C5AA0, 0x825DAF00, kPointMarkerSources,
                                 std::size(kPointMarkerSources), 22, 6};
static_assert(std::size(kCarMarkerSources) == 7 * 12);
static_assert(std::size(kPointMarkerSources) == 22 * 12);

void DrawMarker(uint8_t* base, const Marker& marker, uint32_t pass, uint32_t color,
                uint32_t matrix, uint32_t frame, const MarkerCalls& calls) {
  const uint32_t guard = guest::LoadU32(base, marker.guard);
  if ((guard & 1) == 0) {
    guest::StoreU32(base, marker.guard, guard | 1);
    for (uint32_t i = 0; i < marker.words; i++) {
      CopyFloat(base, marker.table + 4 * i, marker.sources[i]);
    }
    c_runtime::AtexitFrom(base, marker.clean_up, calls.atexit);
  }
  const uint32_t triangle = frame + kMarkerTriangle;
  for (uint32_t i = 0; i < marker.triangles; i++) {
    TriangleInit(base, triangle);
    // The vertices (+80 to +127), lfs and stfs.
    for (uint32_t k = 0; k < 12; k++) {
      CopyFloat(base, triangle + 80 + 4 * k, marker.table + 48 * i + 4 * k);
    }
    // The three vertices' colors (+128 to +175): the color's four words, or
    // (0, 0, 0, 255).
    uint32_t words[4] = {0, 0, 0, 255};
    if (i < marker.colored) {
      for (uint32_t k = 0; k < 4; k++) {
        words[k] = guest::LoadU32(base, color + 4 * k);
      }
    }
    for (uint32_t v = 0; v < 3; v++) {
      for (uint32_t k = 0; k < 4; k++) {
        guest::StoreU32(base, triangle + 128 + 16 * v + 4 * k, words[k]);
      }
    }
    if (!c_runtime::Memcpy(base, triangle, matrix, 64)) {
      calls.memcpy(calls.user, triangle, matrix, 64);
    }
    calls.record(calls.user, triangle, pass, kMarkerKey);
  }
}

}  // namespace

void DrawCarMarker(uint8_t* base, uint32_t pass, uint32_t color, uint32_t matrix,
                   uint32_t frame, const MarkerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820F4888 Minimap::DrawCarMarker");
  DrawMarker(base, kCarMarker, pass, color, matrix, frame, calls);
}

void DrawPointMarker(uint8_t* base, uint32_t pass, uint32_t color, uint32_t matrix,
                     uint32_t frame, const MarkerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820F4BA8 Minimap::DrawPointMarker");
  DrawMarker(base, kPointMarker, pass, color, matrix, frame, calls);
}

}  // namespace minimap
