// The graphics library's command list as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/command-list.md). The list is a guest object (the scene's,
// at scene + 8) that recompiled recorders write into and the render thread
// replays, so these work on guest memory (level 1 of "The boundary rule").
// Adapters: src/hooks/command_list.cpp (and AllocateSlots's in
// src/interpolation/blend_state.cpp).
//
// The list's fields (`list` is its guest address):
//   +0 vtable, +4 the buffer being recorded (0 or 1), +8 each buffer's
//   capacity in slots, +12 the slots used this tick, +16 the slots the last
//   tick used, +20 the allocation, +24 and +28 the two buffers, +32 where
//   the next chunk starts, +36 and +40 the last chunk's start and end, +44
//   the write pointer, +48 the open chunk's start again, byte +52 whether a
//   chunk is open.

#pragma once

#include "guest.h"

#include <cstdint>

namespace command_list {

// The count of chunks open (all lists), a guest global.
constexpr uint32_t kOpenChunks = 0x8296BD28;
// CommandList's vtable.
constexpr uint32_t kVtable = 0x8206C080;

// CommandList::AllocateSlots (sub_8258CAA0): `count` 16-byte slots at the
// write pointer; the first, or 0 with nothing changed when the list would be
// full (used + count not below the capacity, 32-bit), where the original
// prints a message (sub_825A1318) and returns 0.
inline uint32_t AllocateSlots(uint8_t* base, uint32_t list, uint32_t count) {
  const uint32_t used = guest::LoadU32(base, list + 12) + count;
  if (used >= guest::LoadU32(base, list + 8)) {
    return 0;
  }
  const uint32_t first = guest::LoadU32(base, list + 44);
  guest::StoreU32(base, list + 12, used);
  guest::StoreU32(base, list + 44, first + (count << 4));
  return first;
}

// CommandList::CommandList (sub_8258C960): the vtable, everything else 0.
void Construct(uint8_t* base, uint32_t list);

// CommandList::Flip (sub_8258C9C0): +4 ^= 1.
void Flip(uint8_t* base, uint32_t list);

// CommandList::BeginTick (sub_8258C998): +32 = the buffer +4 selects, +16 =
// +12, +12 = 0.
void BeginTick(uint8_t* base, uint32_t list);

enum class OpenResult {
  kOpened,       // a chunk opened at +32
  kAlreadyOpen,  // nothing done (the list's chunk was open)
  kNested,       // nothing done: another chunk is open (the open count isn't
                 // 0), where the original prints a message before opening;
                 // never seen in play, left to the original
};

// CommandList::OpenChunk (sub_8258C9D0).
OpenResult OpenChunk(uint8_t* base, uint32_t list);

// CommandList::CloseChunk (sub_8258CA50): when a chunk is open, closes it at
// the write pointer (+32 = +40 = +44) and the open count goes down by 1.
void CloseChunk(uint8_t* base, uint32_t list);

// CommandList::ChunkSlots (sub_8258CA88): (+40 - +36) >> 4, signed.
int32_t ChunkSlots(uint8_t* base, uint32_t list);

// The allocations with an optional header slot ([id, arg1] written, +8 and
// +12 left as they were), then `slots` slots for the payload; return the
// payload's first slot, or 0 with nothing changed when either allocation
// would find the list full, where the original prints a message (and, for
// a full header, writes the header at guest address 0): left to it.
//
// CommandList::AllocateVertices (sub_8258CB08): (count × 24 + 15) >> 4
// slots, header command 5 with arg1 = count.
uint32_t AllocateVertices(uint8_t* base, uint32_t list, uint32_t count, bool header);
// CommandList::AllocateBytes (sub_8258CB98): (bytes + 15) >> 4 slots, header
// command 21 with arg1 = that slot count (the game's own: command 21's
// handler reads it as bytes).
uint32_t AllocateBytes(uint8_t* base, uint32_t list, uint32_t bytes, bool header);
// CommandList::AllocateResolve (sub_8258CC30): 5 slots, header command 25
// with arg1 = 5.
uint32_t AllocateResolve(uint8_t* base, uint32_t list, bool header);

}  // namespace command_list
