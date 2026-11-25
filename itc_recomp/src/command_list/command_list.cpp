// The graphics library's command list as plain C++: see command_list.h.

#include "command_list/command_list.h"

namespace command_list {

using G = schema::CommandListGlobals;

void Construct(uint32_t list) {
  const schema::CommandList l(list);
  l.set_vtable(schema::CommandList::kVtable);
  l.set_buffer(0);
  l.set_capacity(0);
  l.set_used(0);
  l.set_next_chunk(0);
  l.set_allocation(0);
  l.set_chunk_open(0);
  l.set_buffer0(0);
  l.set_buffer1(0);
}

void Flip(uint32_t list) {
  const schema::CommandList l(list);
  l.set_buffer(l.buffer() ^ 1);
}

void BeginTick(uint32_t list) {
  const schema::CommandList l(list);
  // buffer0 or buffer1, as the buffer selects (+24 + 4 × buffer).
  const uint32_t buffer = l.buffer0_address() + (l.buffer() << 2);
  l.set_next_chunk(guest::LoadU32(buffer));
  const uint32_t used = l.used();
  l.set_used(0);
  l.set_last_used(used);
}

OpenResult OpenChunk(uint32_t list) {
  const schema::CommandList l(list);
  if (l.chunk_open() != 0) {
    return OpenResult::kAlreadyOpen;
  }
  const uint32_t open = G::open_chunks();
  if (open != 0) {
    return OpenResult::kNested;
  }
  const uint32_t start = l.next_chunk();
  l.set_chunk_start(start);
  l.set_write(start);
  l.set_open_start(start);
  l.set_chunk_end(start);
  l.set_chunk_open(1);
  G::set_open_chunks(open + 1);
  return OpenResult::kOpened;
}

void CloseChunk(uint32_t list) {
  const schema::CommandList l(list);
  if (l.chunk_open() == 0) {
    return;
  }
  const uint32_t end = l.write();
  l.set_chunk_open(0);
  l.set_next_chunk(end);
  l.set_chunk_end(end);
  G::set_open_chunks(G::open_chunks() - 1);
}

int32_t ChunkSlots(uint32_t list) {
  const schema::CommandList l(list);
  const uint32_t bytes = l.chunk_end() - l.chunk_start();
  return int32_t(bytes) >> 4;
}

namespace {

// A header slot (when asked for) and `slots` payload slots, both checked
// before anything is written.
uint32_t AllocateWithHeader(uint32_t list, uint32_t slots, bool header,
                            uint32_t id, uint32_t arg1) {
  const schema::CommandList l(list);
  const uint32_t used = l.used();
  const uint32_t limit = l.capacity();
  const uint32_t header_slots = header ? 1 : 0;
  if ((header && used + 1 >= limit) || used + header_slots + slots >= limit) {
    return 0;
  }
  if (header) {
    const uint32_t slot = AllocateSlots(list, 1);
    guest::StoreU32(slot + 4, arg1);
    guest::StoreU32(slot + 0, id);
  }
  return AllocateSlots(list, slots);
}

}  // namespace

uint32_t AllocateVertices(uint32_t list, uint32_t count, bool header) {
  return AllocateWithHeader(list, (count * 24 + 15) >> 4, header, 5, count);
}

uint32_t AllocateBytes(uint32_t list, uint32_t bytes, bool header) {
  const uint32_t slots = (bytes + 15) >> 4;
  return AllocateWithHeader(list, slots, header, 21, slots);
}

uint32_t AllocateResolve(uint32_t list, bool header) {
  return AllocateWithHeader(list, 5, header, 25, 5);
}

}  // namespace command_list
