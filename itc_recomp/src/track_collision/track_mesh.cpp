// The track's collision mesh and route: their lifetime and the mesh's
// lookups (docs/functions/collision-world.md): see track_collision.h.

#include "track_collision/track_collision.h"

#include "guest.h"

#include "base/log.h"
#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"
#include "collision/collision.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/collision_world.h"
#include "schema/race_states.h"
#include "schema/track_route.h"

namespace track_collision {

using namespace car_physics::ops;

namespace {

using G = schema::CollisionWorldGlobals;
using Mesh = schema::TrackMesh;
using Scratch = schema::TrackMeshScratch;

// VirtualAlloc(0, size, MEM_COMMIT | MEM_NOZERO, PAGE_READWRITE) as the
// adapter in src/hooks/heap.cpp answers it.
uint32_t AllocPages(uint32_t size) {
  return memory::AllocPages(size, 4096);
}

}  // namespace

uint32_t TrackMeshInitScratch(uint32_t scratch) {
  ITC_NATIVE_RUNNING("sub_820F6898 TrackMesh::InitScratch");
  const Scratch s(scratch);
  s.set_zero(0);
  const uint32_t block = AllocPages(8192);
  s.set_block(block);
  return block;
}

uint32_t TrackMeshPrepare(uint32_t mesh) {
  ITC_NATIVE_RUNNING("sub_820F68E8 TrackMesh::Prepare");
  const Mesh m(mesh);
  m.set_state(1);
  m.set_file(0);
  const uint32_t scratch = AllocPages(8);
  m.set_scratch(scratch);
  return TrackMeshInitScratch(scratch);
}

bool TrackMeshFreeable(uint32_t mesh) {
  const Mesh m(mesh);
  const uint32_t scratch = m.scratch().address;
  const uint32_t file = m.file().address;
  return !((scratch != 0 && (!memory::IsPages(scratch) ||
                             !memory::IsPages(Scratch(scratch).block()))) ||
           (file != 0 && !memory::IsPages(file)));
}

bool TrackMeshFree(uint32_t mesh, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820F6940 TrackMesh::Free");
  if (!TrackMeshFreeable(mesh)) {
    return false;
  }
  const Mesh m(mesh);
  const uint32_t scratch = m.scratch().address;
  if (scratch != 0) {
    memory::FreePages(Scratch(scratch).block());
    memory::FreePages(m.scratch().address);
    m.set_scratch(0);
  }
  const uint32_t loaded = m.file().address;
  *r3 = loaded;
  if (loaded != 0) {
    *r3 = memory::FreePages(loaded) ? 1 : 0;
    m.set_file(0);
  }
  return true;
}

void TrackMeshOnLoaded(uint32_t mesh) {
  ITC_NATIVE_RUNNING("sub_820F69B0 TrackMesh::OnLoaded");
  const Mesh m(mesh);
  const schema::TrackMeshFile f = m.file();
  const uint32_t file = f.address;
  const uint32_t count = f.section_count();
  f.set_sections(f.sections() + file);
  // The count read again after each entry (unsigned).
  if (count != 0) {
    uint32_t i = 0;
    do {
      const uint32_t entry = f.sections() + 4 * i;
      guest::StoreU32(entry, guest::LoadU32(entry) + file);
      i++;
    } while (i < f.section_count());
  }
  m.set_state(2);
}

uint32_t TrackMeshSliceKind(uint32_t mesh, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F6A08 TrackMesh::SliceKind");
  const schema::TrackMeshFile file = Mesh(mesh).file();
  const schema::TrackPosition p(position);
  const int32_t section_index = int16_t(p.section());
  const int32_t slice = int16_t(p.slice());
  const uint32_t section = guest::LoadU32(uint32_t(section_index) * 4 + file.sections());
  return schema::TrackMeshSlice(uint32_t(slice) * 28 +
                                          schema::TrackMeshSection(section).slices() + section)
      .kind();
}

void TrackRoutePrepare(uint32_t route) {
  ITC_NATIVE_RUNNING("sub_820F8600 TrackRoute::Prepare");
  const schema::TrackRoute r(route);
  r.set_state(1);
  r.set_file(0);
}

bool TrackRouteFreeable(uint32_t route) {
  return memory::IsPages(schema::TrackRoute(route).file().address);
}

bool TrackRouteFree(uint32_t route, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820F8618 TrackRoute::Free");
  if (!TrackRouteFreeable(route)) {
    return false;
  }
  const schema::TrackRoute r(route);
  r.set_state(0);
  *r3 = memory::FreePages(r.file().address) ? 1 : 0;
  r.set_file(0);
  return true;
}

void TrackRouteOnLoaded(uint32_t route) {
  ITC_NATIVE_RUNNING("sub_820F9050 TrackRoute::OnLoaded");
  const schema::TrackRoute r(route);
  const uint32_t file = r.file().address;
  const schema::TrackRouteFile f(file);
  // Offsets made pointers: the tables at +12 (count +0) and +16 (count
  // +4), each count read again after each entry (unsigned), and the 12
  // words at +20.
  auto relocate = [&](uint32_t table, uint32_t count_at) {
    const uint32_t count = guest::LoadU32(count_at);
    guest::StoreU32(table, guest::LoadU32(table) + file);
    if (count == 0) {
      return;
    }
    uint32_t i = 0;
    do {
      const uint32_t entry = guest::LoadU32(table) + 4 * i;
      guest::StoreU32(entry, guest::LoadU32(entry) + file);
      i++;
    } while (i < guest::LoadU32(count_at));
  };
  relocate(f.sections_address(), f.section_count_address());
  relocate(f.corners_address(), f.count_4_address());
  // The route bit tables' word (+20): a pointer to 12 words.
  const uint32_t bit_tables = f.route_bit_tables_address();
  guest::StoreU32(bit_tables, guest::LoadU32(bit_tables) + file);
  for (uint32_t k = 0; k < 48; k += 4) {
    const uint32_t entry = guest::LoadU32(bit_tables) + k;
    guest::StoreU32(entry, guest::LoadU32(entry) + file);
  }
  // For each of the race's route bits (the race data's +24) set, the
  // listed entries' +148 raised or +152 lowered to its values.
  const uint32_t bits = G::race_data().drawn_entries_address();
  for (uint32_t k = 0; k < 12; k++) {
    if ((guest::LoadU32(bits + (k >> 5) * 4) & (1u << (k & 31))) == 0) {
      continue;
    }
    const uint32_t list = guest::LoadU32(guest::LoadU32(bit_tables) + 4 * k);
    if (guest::LoadU16(list + 0) == 0) {
      continue;
    }
    uint32_t item = list + 8;
    for (int32_t i = 0; i < int32_t(guest::LoadU16(list + 0)); i++, item += 8) {
      const int32_t index = int16_t(guest::LoadU16(item - 4));
      const int32_t entry_index = int16_t(guest::LoadU16(item - 2));
      if (index < 0 || entry_index < 0) {
        continue;
      }
      const uint32_t raise = guest::LoadU16(list + 2);
      const double value = Lfs(item + 0);
      const schema::TrackRouteSlice entry(guest::LoadU32(uint32_t(index) * 4 + f.sections()) +
                    uint32_t(entry_index) * 84 + 124);
      if (raise == 0) {
        if (value > Lfs(entry.raised_address())) {
          Stfs(entry.raised_address(), value);
        }
      } else if (value < Lfs(entry.lowered_address())) {
        Stfs(entry.lowered_address(), value);
      }
    }
  }
  // The route's +4: the sum of the first halfwords of the +12 entries.
  r.set_slice_total(0);
  const schema::TrackRouteFile file2 = r.file();
  for (int32_t i = 0; i < int32_t(r.file().section_count()); i++) {
    const schema::TrackRouteSection section(guest::LoadU32(((uint32_t(i) << 2) & 0x3FFFC) + file2.sections()));
    r.set_slice_total(uint32_t(int32_t(int16_t(section.slice_count()))) + r.slice_total());
  }
  r.set_state(2);
}

bool TrackCollisionUnload() {
  ITC_NATIVE_RUNNING("sub_820FEFB0 TrackCollision::Unload");
  const uint32_t object = G::collision_object().address;
  // Everything it frees checked first (a block that isn't the layer's: the
  // original, whose frees take their error paths).
  if (!car_physics::CollisionWorldDestroyable() || !TrackMeshFreeable(kMesh) ||
      !TrackRouteFreeable(kRoute) || !collision::Deletable(object)) {
    return false;
  }
  car_physics::CollisionWorldDestroy();
  uint32_t r3 = 0;
  TrackMeshFree(kMesh, &r3);
  TrackRouteFree(kRoute, &r3);
  memory::crt::OperatorDelete(G::collision_object().address);
  return true;
}

}  // namespace track_collision
