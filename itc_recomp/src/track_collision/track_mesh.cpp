// The track's collision mesh and route: their lifetime and the mesh's
// lookups (docs/functions/collision-world.md): see track_collision.h.

#include "track_collision/track_collision.h"

#include "guest.h"

#include "base/log.h"
#include "memory/heap.h"

namespace track_collision {

namespace {

// VirtualAlloc(0, size, MEM_COMMIT | MEM_NOZERO, PAGE_READWRITE) as the
// adapter in src/hooks/heap.cpp answers it.
uint32_t AllocPages(uint32_t size) {
  return memory::AllocPages(size, 4096);
}

}  // namespace

uint32_t TrackMeshInitScratch(uint8_t* base, uint32_t scratch) {
  ITC_NATIVE_RUNNING("sub_820F6898 TrackMesh::InitScratch");
  guest::StoreU32(base, scratch + 0, 0);
  const uint32_t block = AllocPages(8192);
  guest::StoreU32(base, scratch + 4, block);
  return block;
}

uint32_t TrackMeshPrepare(uint8_t* base, uint32_t mesh) {
  ITC_NATIVE_RUNNING("sub_820F68E8 TrackMesh::Prepare");
  guest::StoreU32(base, mesh + 8, 1);
  guest::StoreU32(base, mesh + 4, 0);
  const uint32_t scratch = AllocPages(8);
  guest::StoreU32(base, mesh + 0, scratch);
  return TrackMeshInitScratch(base, scratch);
}

bool TrackMeshFree(uint8_t* base, uint32_t mesh, uint32_t* r3) {
  const uint32_t scratch = guest::LoadU32(base, mesh + 0);
  const uint32_t file = guest::LoadU32(base, mesh + 4);
  if ((scratch != 0 && (!memory::IsPages(scratch) ||
                        !memory::IsPages(guest::LoadU32(base, scratch + 4)))) ||
      (file != 0 && !memory::IsPages(file))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820F6940 TrackMesh::Free");
  if (scratch != 0) {
    memory::FreePages(guest::LoadU32(base, scratch + 4));
    memory::FreePages(guest::LoadU32(base, mesh + 0));
    guest::StoreU32(base, mesh + 0, 0);
  }
  const uint32_t loaded = guest::LoadU32(base, mesh + 4);
  *r3 = loaded;
  if (loaded != 0) {
    *r3 = memory::FreePages(loaded) ? 1 : 0;
    guest::StoreU32(base, mesh + 4, 0);
  }
  return true;
}

void TrackMeshOnLoaded(uint8_t* base, uint32_t mesh) {
  ITC_NATIVE_RUNNING("sub_820F69B0 TrackMesh::OnLoaded");
  const uint32_t file = guest::LoadU32(base, mesh + 4);
  const uint32_t count = guest::LoadU32(base, file + 0);
  guest::StoreU32(base, file + 4, guest::LoadU32(base, file + 4) + file);
  // The count read again after each entry (unsigned).
  if (count != 0) {
    uint32_t i = 0;
    do {
      const uint32_t entry = guest::LoadU32(base, file + 4) + 4 * i;
      guest::StoreU32(base, entry, guest::LoadU32(base, entry) + file);
      i++;
    } while (i < guest::LoadU32(base, file + 0));
  }
  guest::StoreU32(base, mesh + 8, 2);
}

uint32_t TrackMeshSliceKind(uint8_t* base, uint32_t mesh, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F6A08 TrackMesh::SliceKind");
  const uint32_t file = guest::LoadU32(base, mesh + 4);
  const int32_t section_index = int16_t(guest::LoadU16(base, position + 0));
  const int32_t slice = int16_t(guest::LoadU16(base, position + 2));
  const uint32_t section =
      guest::LoadU32(base, uint32_t(section_index) * 4 + guest::LoadU32(base, file + 4));
  return guest::LoadU16(base, uint32_t(slice) * 28 + guest::LoadU32(base, section + 72) + section +
                                  24);
}

void TrackRoutePrepare(uint8_t* base, uint32_t route) {
  ITC_NATIVE_RUNNING("sub_820F8600 TrackRoute::Prepare");
  guest::StoreU32(base, route + 8, 1);
  guest::StoreU32(base, route + 0, 0);
}

bool TrackRouteFree(uint8_t* base, uint32_t route, uint32_t* r3) {
  const uint32_t file = guest::LoadU32(base, route + 0);
  if (!memory::IsPages(file)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820F8618 TrackRoute::Free");
  guest::StoreU32(base, route + 8, 0);
  *r3 = memory::FreePages(guest::LoadU32(base, route + 0)) ? 1 : 0;
  guest::StoreU32(base, route + 0, 0);
  return true;
}

}  // namespace track_collision
