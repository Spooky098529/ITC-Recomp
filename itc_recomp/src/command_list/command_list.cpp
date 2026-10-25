// The graphics library's command list as plain C++: see command_list.h.

#include "command_list/command_list.h"

namespace command_list {

void Construct(uint8_t* base, uint32_t list) {
  guest::StoreU32(base, list + 0, kVtable);
  for (uint32_t offset : {4u, 8u, 12u, 32u, 20u}) {
    guest::StoreU32(base, list + offset, 0);
  }
  guest::StoreU8(base, list + 52, 0);
  guest::StoreU32(base, list + 24, 0);
  guest::StoreU32(base, list + 28, 0);
}

void Flip(uint8_t* base, uint32_t list) {
  guest::StoreU32(base, list + 4, guest::LoadU32(base, list + 4) ^ 1);
}

void BeginTick(uint8_t* base, uint32_t list) {
  const uint32_t buffer = (guest::LoadU32(base, list + 4) + 6) << 2;
  guest::StoreU32(base, list + 32, guest::LoadU32(base, buffer + list));
  const uint32_t used = guest::LoadU32(base, list + 12);
  guest::StoreU32(base, list + 12, 0);
  guest::StoreU32(base, list + 16, used);
}

OpenResult OpenChunk(uint8_t* base, uint32_t list) {
  if (guest::LoadU8(base, list + 52) != 0) {
    return OpenResult::kAlreadyOpen;
  }
  const uint32_t open = guest::LoadU32(base, kOpenChunks);
  if (open != 0) {
    return OpenResult::kNested;
  }
  const uint32_t start = guest::LoadU32(base, list + 32);
  guest::StoreU32(base, list + 36, start);
  guest::StoreU32(base, list + 44, start);
  guest::StoreU32(base, list + 48, start);
  guest::StoreU32(base, list + 40, start);
  guest::StoreU8(base, list + 52, 1);
  guest::StoreU32(base, kOpenChunks, open + 1);
  return OpenResult::kOpened;
}

void CloseChunk(uint8_t* base, uint32_t list) {
  if (guest::LoadU8(base, list + 52) == 0) {
    return;
  }
  const uint32_t end = guest::LoadU32(base, list + 44);
  guest::StoreU8(base, list + 52, 0);
  guest::StoreU32(base, list + 32, end);
  guest::StoreU32(base, list + 40, end);
  guest::StoreU32(base, kOpenChunks, guest::LoadU32(base, kOpenChunks) - 1);
}

int32_t ChunkSlots(uint8_t* base, uint32_t list) {
  const uint32_t bytes = guest::LoadU32(base, list + 40) - guest::LoadU32(base, list + 36);
  return int32_t(bytes) >> 4;
}

namespace {

// A header slot (when asked for) and `slots` payload slots, both checked
// before anything is written.
uint32_t AllocateWithHeader(uint8_t* base, uint32_t list, uint32_t slots, bool header,
                            uint32_t id, uint32_t arg1) {
  const uint32_t used = guest::LoadU32(base, list + 12);
  const uint32_t limit = guest::LoadU32(base, list + 8);
  const uint32_t header_slots = header ? 1 : 0;
  if ((header && used + 1 >= limit) || used + header_slots + slots >= limit) {
    return 0;
  }
  if (header) {
    const uint32_t slot = AllocateSlots(base, list, 1);
    guest::StoreU32(base, slot + 4, arg1);
    guest::StoreU32(base, slot + 0, id);
  }
  return AllocateSlots(base, list, slots);
}

}  // namespace

uint32_t AllocateVertices(uint8_t* base, uint32_t list, uint32_t count, bool header) {
  return AllocateWithHeader(base, list, (count * 24 + 15) >> 4, header, 5, count);
}

uint32_t AllocateBytes(uint8_t* base, uint32_t list, uint32_t bytes, bool header) {
  const uint32_t slots = (bytes + 15) >> 4;
  return AllocateWithHeader(base, list, slots, header, 21, slots);
}

uint32_t AllocateResolve(uint8_t* base, uint32_t list, bool header) {
  return AllocateWithHeader(base, list, 5, header, 25, 5);
}

}  // namespace command_list
