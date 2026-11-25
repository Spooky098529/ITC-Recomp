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
#include "schema/career.h"
#include "schema/command_list.h"
#include "schema/minimap.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/scenery_lists.h"
#include "schema/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace minimap {

namespace {

using G = schema::MinimapGlobals;
using List = schema::MinimapList80;
using Entry = schema::MinimapRacerEntry;
using TriangleView = schema::MinimapTriangle;

// The originals' lfs and stfs copy of a float.
void CopyFloat(uint32_t to, uint32_t from) {
  guest::StoreFloat(to, guest::LoadFloat(from));
}

int32_t LoadS16(uint32_t address) {
  return int16_t(guest::LoadU16(address));
}

// Node `index` (sign-extended, as the originals' lha) of the list.
uint32_t NodeAddress(uint32_t list, int32_t index) { return uint32_t(index) * 4 + list; }

// The node `steps` steps from the head (32 bits, as the originals walk it):
// the head's node, then each node's next.
uint32_t NodeAt(uint32_t list, uint32_t steps) {
  uint32_t node = NodeAddress(list, int16_t(List(list).head()));
  for (uint32_t n = steps; n != 0; n--) {
    node = NodeAddress(list, LoadS16(node));
  }
  return node;
}

// The node's data slot's address (its signed halfword +2, 64 bytes a slot).
uint32_t DataAt(uint32_t list, uint32_t node) {
  return uint32_t(LoadS16(node + 2)) * 64 + List(list).data_address();
}

// An identity's row `row` (the originals' vupkd3d128 and vpermwi128): 1.0
// in word `row`, 0 in the others.
void StoreIdentityRow(uint32_t address, uint32_t row) {
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(address + 4 * i, i == row ? 0x3F800000u : 0u);
  }
}

}  // namespace

uint32_t RaceFindPlayerEntry(uint32_t manager, uint32_t which) {
  ITC_NATIVE_RUNNING("sub_82170608 Race::FindPlayerEntry");
  const schema::RaceManager m(manager);
  const int32_t count = int32_t(m.entry_count());
  const uint32_t flag = which != 0 ? 0x4 : 0x8;
  for (int32_t i = 0; i < count; i++) {
    if ((Entry(m.entries_address() + 28 * uint32_t(i)).flags() & flag) != 0) {
      return m.entries_address() + 28 * uint32_t(i);
    }
  }
  return 0;
}

uint32_t RaceRacerVehicle(uint32_t racer) {
  ITC_NATIVE_RUNNING("sub_82170578 Race::RacerVehicle");
  const uint32_t manager = G::racers_manager();
  if (manager == 0) {
    return 0;
  }
  // The racer's entry (the manager's racers, +2308).
  const uint32_t entry =
      guest::LoadU32(schema::RaceManager(manager).racers_address() + racer * 4);
  return vehicle_accessors::VehicleGet(Entry(entry).vehicle_index());
}

void SetListPoint(uint32_t list, uint32_t out, uint32_t entry, uint32_t point,
                  uint32_t part) {
  ITC_NATIVE_RUNNING("sub_82147D18 SetList::Point");
  const uint32_t from =
      guest::LoadU32(schema::ModelList(list).point_groups_address() + entry * 4) +
      ((point * 4 + part) << 4);
  const float x = guest::LoadFloat(from + 0);
  const float y = guest::LoadFloat(from + 4);
  const float z = guest::LoadFloat(from + 8);
  const float w = G::one();
  const schema::MinimapPoint to(out);
  to.set_x(x);
  to.set_y(y);
  to.set_z(z);
  to.set_w(w);
}

std::optional<int32_t> RacerMarkerType(uint32_t entry, int32_t branch) {
  ITC_NATIVE_RUNNING("sub_820F2D28 Minimap::RacerMarkerType");
  int32_t type;
  const Entry e(entry);
  const uint32_t record =
      schema::RaceData(G::race_data()).racer_records_address() + 3936 * e.record();
  if ((e.flags() & 0x10) != 0) {
    type = 1;
  } else if (int32_t id = int32_t(schema::Car(record).item_id()); id == -1) {
    type = 1;
  } else {
    // The id's three halfwords in player 0's save data.
    const uint32_t save = vehicle_accessors::SaveDataOf(0);
    const uint32_t item = data_tables::RecordsItem(uint32_t(id));
    const schema::CareerItemRecord counts(schema::CareerSave(save).item_records_address() + uint32_t(id) * 12);
    type = -1;
    if (counts.wins() != 0) {
      type = 4;
      if (schema::OpponentItem(item).race_kind() == 3) {
        return std::nullopt;  // sub_820F2CA8, never seen in play
      }
    }
    const int32_t second = int16_t(counts.wins());
    if (second == 0) {
      type = 3;
    }
    const int32_t sum = int16_t(int16_t(counts.outcome3()) + int16_t(counts.losses()) + second);
    if (sum <= 0) {
      type = 2;
    }
  }
  const uint16_t section = e.section();
  if (int16_t(section) <= -1 || int16_t(e.slice()) <= -1) {
    return -1;
  }
  if (type == 1) {
    return 1;
  }
  const uint32_t sections = schema::TrackRouteFile(G::route()).sections();
  const int32_t own =
      schema::TrackRouteSection(guest::LoadU32(sections + 4 * section)).branch();
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

bool Scaling(uint32_t out, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820F2EF8 Minimap::Scaling");
  if (out >= 0xA0000000u) {
    return false;
  }
  // The identity made on the original's stack, then memcpy'd to out.
  for (uint32_t row = 0; row < 4; row++) {
    StoreIdentityRow(out + 16 * row, row);
  }
  const schema::MinimapScale scale(vector);
  guest::StoreFloat(out + 0, scale.x());
  guest::StoreFloat(out + 20, scale.y());
  guest::StoreFloat(out + 40, scale.z());
  return true;
}

void TriangleInit(uint32_t triangle) {
  ITC_NATIVE_RUNNING("sub_820F4040 Triangle::Init");
  const TriangleView t(triangle);
  const float one = G::one();
  t.set_world_row0_w(one);
  t.set_world_row1_w(one);
  t.set_world_row2_w(one);
  t.set_world_row3_w(one);
  t.set_kind(0);
  t.set_mode_a(0);
  t.set_mode_b(2);
  for (uint32_t row = 0; row < 4; row++) {
    StoreIdentityRow((t.world_address() + 16 * row) & ~0xFu, row);
  }
  t.set_vertex0_w(one);
  t.set_vertex1_w(one);
  t.set_vertex2_w(one);
}

void List80Link(uint32_t list, int16_t position) {
  ITC_NATIVE_RUNNING("sub_820F40C0 List80::Link");
  const List l(list);
  uint32_t after;
  if (position == 0) {
    after = l.head_address();
  } else {
    int32_t index = int16_t(l.head());
    uint32_t n = uint32_t(int32_t(position));
    for (;;) {
      after = NodeAddress(list, index);
      if (--n == 0) {
        break;
      }
      index = LoadS16(after);
    }
  }
  const uint32_t free_node = NodeAddress(list, int16_t(l.free_head()));
  const uint16_t free_index = l.free_head();
  l.set_free_head(guest::LoadU16(free_node));
  guest::StoreU16(free_node, free_index);
  guest::StoreU16(free_node, guest::LoadU16(after));
  guest::StoreU16(after, free_index);
  l.set_count(uint16_t(l.count() + 1));
}

uint32_t List80Insert(uint32_t list, uint32_t matrix, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F5238 List80::Insert");
  List80Link(list, int16_t(position));
  const uint32_t to = DataAt(list, NodeAt(list, position));
  matrix_math::Copy(to, matrix);
  interpolation::BlendState_OnCopy(to, matrix, 0x820F529C);
  return to;
}

bool List80Copy(uint32_t out, uint32_t list, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F51A8 List80::Copy");
  if (out >= 0xA0000000u) {
    return false;
  }
  const uint32_t from = DataAt(list, NodeAt(list, position));
  const uint32_t one = guest::LoadU32(G::one_address());
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    guest::StoreU32(out + offset, one);
  }
  return c_runtime::Memcpy(out, from, 64);
}

std::optional<uint32_t> RecordTransform(uint32_t pass, uint32_t primitive,
                                        uint32_t product) {
  ITC_NATIVE_RUNNING("sub_8217AEA8 Primitive::RecordTransform");
  const uint32_t list = G::triangle_list();
  const schema::CommandList l(list);
  const uint32_t used = l.used();
  if (used + 5 < used || used + 5 >= l.capacity()) {
    return std::nullopt;
  }
  // The original's 1.0 stores before the multiply (overwritten by it).
  const uint32_t one = guest::LoadU32(G::one_address());
  for (uint32_t offset : {12u, 28u, 44u, 60u}) {
    guest::StoreU32(product + offset, one);
  }
  matrix_math::Multiply(product, primitive, pass + 144);
  interpolation::BlendState_OnMultiply(product, primitive, pass + 144, 0x8217AEDC);
  const uint32_t header = command_list::AllocateSlots(list, 1);
  guest::StoreU32(header + 4, 0);
  guest::StoreU32(header + 0, 7);
  guest::StoreU32(header + 8, 1);
  guest::StoreU8(header + 12, 0);
  guest::StoreU8(header + 13, 4);
  uint32_t slot = 0;
  for (uint32_t row = 0; row < 4; row++) {
    slot = command_list::AllocateSlots(list, 1);
    for (uint32_t i = 0; i < 4; i++) {
      CopyFloat(slot + 4 * i, product + 16 * row + 4 * i);
    }
  }
  return slot;
}

TriangleRecording TriangleRecord(uint32_t triangle) {
  ITC_NATIVE_RUNNING("sub_8217DAA0 Triangle::Record");
  const uint32_t kind = TriangleView(triangle).kind();
  return kind == 0 ? TriangleRecording::kFlat
                   : kind == 1 ? TriangleRecording::kOther : TriangleRecording::kNone;
}

std::optional<uint32_t> TriangleRecordFlat(uint32_t triangle, uint32_t pass,
                                           uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8217C370 Triangle::RecordFlat");
  const uint32_t list = G::triangle_list();
  // The header, the transform's five, [13], the vertices' header and five,
  // and [15]: 14 slots, with no chunk open on any list.
  const schema::CommandList l(list);
  const uint32_t used = l.used();
  if (l.chunk_open() != 0 || schema::CommandListGlobals::open_chunks() != 0 ||
      used + 14 < used || used + 14 >= l.capacity()) {
    return std::nullopt;
  }
  command_list::OpenChunk(list);
  const uint32_t header = command_list::AllocateSlots(list, 1);
  RecordTransform(pass, triangle, frame - kTransformProduct);
  const uint32_t slot = command_list::AllocateSlots(list, 1);
  guest::StoreU32(slot + 0, 13);
  guest::StoreU32(slot + 4, 4);
  guest::StoreU32(slot + 8, 0);
  guest::StoreU32(slot + 12, 0);
  uint32_t vertex = command_list::AllocateVertices(list, 3, true);
  for (uint32_t i = 0; i < 3; i++) {
    // The view moved 16 bytes a vertex: its vertex 0 and color 0 are vertex
    // i's.
    const TriangleView from(triangle + 16 * i);
    guest::StoreFloat(vertex + 0, from.vertex0_x());
    guest::StoreFloat(vertex + 4, from.vertex0_y());
    guest::StoreFloat(vertex + 8, from.vertex0_z());
    const uint32_t color = (from.color0_alpha() << 24) |
                           (uint32_t(from.color0_red_byte()) << 16) |
                           (uint32_t(from.color0_green_byte()) << 8) |
                           from.color0_blue_byte();
    guest::StoreU32(vertex + 12, color);
    vertex += 24;
  }
  guest::StoreU32(command_list::AllocateSlots(list, 1), 15);
  command_list::CloseChunk(list);
  const int32_t slots = command_list::ChunkSlots(list);
  guest::StoreU16(header + 0, 0);
  guest::StoreU16(header + 2, 0);
  guest::StoreU32(header + 4, 0);
  guest::StoreU32(header + 12, 0);
  guest::StoreU32(header + 8, uint32_t(slots - 1));
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
constexpr Marker kCarMarker = {G::car_marker_table_address(), G::car_marker_guard_address(),
                               0x825DAEF8, kCarMarkerSources, std::size(kCarMarkerSources), 7,
                               1};
constexpr Marker kPointMarker = {G::point_marker_table_address(),
                                 G::point_marker_guard_address(), 0x825DAF00,
                                 kPointMarkerSources, std::size(kPointMarkerSources), 22, 6};
static_assert(std::size(kCarMarkerSources) == 7 * 12);
static_assert(std::size(kPointMarkerSources) == 22 * 12);

void DrawMarker(const Marker& marker, uint32_t pass, uint32_t color,
                uint32_t matrix, uint32_t frame, const MarkerCalls& calls) {
  const uint32_t guard = guest::LoadU32(marker.guard);
  if ((guard & 1) == 0) {
    guest::StoreU32(marker.guard, guard | 1);
    for (uint32_t i = 0; i < marker.words; i++) {
      CopyFloat(marker.table + 4 * i, marker.sources[i]);
    }
    c_runtime::AtexitFrom(marker.clean_up, calls.atexit);
  }
  const uint32_t triangle = frame + kMarkerTriangle;
  const TriangleView t(triangle);
  for (uint32_t i = 0; i < marker.triangles; i++) {
    TriangleInit(triangle);
    // The vertices, lfs and stfs.
    for (uint32_t k = 0; k < 12; k++) {
      CopyFloat(t.vertices_address() + 4 * k, marker.table + 48 * i + 4 * k);
    }
    // The three vertices' colors: the color's four words, or (0, 0, 0, 255).
    uint32_t words[4] = {0, 0, 0, 255};
    if (i < marker.colored) {
      const schema::MinimapColor c(color);
      words[0] = c.red();
      words[1] = c.green();
      words[2] = c.blue();
      words[3] = c.alpha();
    }
    for (uint32_t v = 0; v < 3; v++) {
      // The view moved 16 bytes a vertex: its color 0 is vertex v's.
      const TriangleView to(triangle + 16 * v);
      to.set_color0_red(words[0]);
      to.set_color0_green(words[1]);
      to.set_color0_blue(words[2]);
      to.set_color0_alpha(words[3]);
    }
    if (!c_runtime::Memcpy(t.world_address(), matrix, 64)) {
      calls.memcpy(calls.user, triangle, matrix, 64);
    }
    calls.record(calls.user, triangle, pass, kMarkerKey);
  }
}

}  // namespace

void DrawCarMarker(uint32_t pass, uint32_t color, uint32_t matrix,
                   uint32_t frame, const MarkerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820F4888 Minimap::DrawCarMarker");
  DrawMarker(kCarMarker, pass, color, matrix, frame, calls);
}

void DrawPointMarker(uint32_t pass, uint32_t color, uint32_t matrix,
                     uint32_t frame, const MarkerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820F4BA8 Minimap::DrawPointMarker");
  DrawMarker(kPointMarker, pass, color, matrix, frame, calls);
}

}  // namespace minimap
