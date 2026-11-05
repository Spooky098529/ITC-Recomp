// FramePasses::Record's recording, split in three parts (record.cpp holds
// the whole; record_part1.cpp to record_part3.cpp the chunks between the
// list's OpenChunk and CloseChunk). Internal to src/frame_passes/.

#pragma once

#include "frame_passes/frame_passes.h"

#include "guest.h"

#include <cstdint>
#include <vector>

namespace frame_passes::record {

// The recording's state, shared by the parts: the list being recorded
// (one chunk open for the whole), FramePasses (this, `passes`), the frame
// (Record's r1), and the kind-5 heads in the order they're made (heads[0]
// the first; 18 in all).
struct Recorder {
  uint8_t* base;
  uint32_t list;
  uint32_t passes;
  uint32_t frame;
  std::vector<uint32_t> heads;

  uint32_t Load(uint32_t address) const { return guest::LoadU32(base, address); }
  void Store(uint32_t address, uint32_t value) const { guest::StoreU32(base, address, value); }
  // Words of Record's frame (its r1 + offset).
  void StoreFrame(uint32_t offset, uint32_t value) const { Store(frame + offset, value); }
  uint32_t LoadFrame(uint32_t offset) const { return Load(frame + offset); }

  // One slot (room checked up front: none fails).
  uint32_t Slot() const;
  // A slot with [a, b, c] (+12 left as it was), [a, b] (+8, +12 left), [a].
  uint32_t Slot3(uint32_t a, uint32_t b, uint32_t c) const;
  uint32_t Slot2(uint32_t a, uint32_t b) const;
  uint32_t Slot1(uint32_t a) const;
  // A kind-5 head: a slot with the halfword +2 = 5 and +4 = 0 (the rest left
  // as it was), kept in `heads`; returns it.
  uint32_t Head();
  // CommandList::AllocateBytes(`bytes`) with no header.
  uint32_t Bytes(uint32_t bytes) const;
  // FramePasses' vtable slots 5 and 6 on the list (this's +20, +24: in play
  // always RecordConstants and RecordSamplers, which Record checks).
  void Constants() const;
  void Samplers() const;
};

// The three parts, each step for step as the recompiled code between its
// marks (the AllocateSlots calls returning to 0x820964A8, 0x82096EA0 and
// 0x82097654, and the CloseChunk returning to 0x82097E8C): part 1 slots 0
// to 153 (heads 0 to 5), part 2 slots 154 to 255 (heads 6 to 10), part 3
// slots 256 to 358 (heads 11 to 17), counting from the chunk's first.
void RecordPart1(Recorder& r);
void RecordPart2(Recorder& r);
void RecordPart3(Recorder& r);

// The slots the parts take (with the recorders' and the Clear's).
constexpr uint32_t kSlots = 359;

}  // namespace frame_passes::record
