// The game's pending frees (0x8296BD54, models/memory_blocks.h) in the
// harnesses: the list saved before a case and put back after it, the nodes
// a case added kept by their contents, and blocks with or without the
// XLMM header for the put-off frees. Shared by the models and game_modes
// harnesses.

#pragma once

#include "harness.h"

#include "memory/c_runtime_heap.h"
#include "models/memory_blocks.h"

#include <vector>

namespace harness::pending_frees {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The game's pending frees as they were before a case.
struct ListState {
  uint32_t head;
  uint32_t previous;  // the head's previous (the last node)
  uint32_t size;
};

inline ListState SaveList(uint8_t* base) {
  const uint32_t head = Load(base, models::kPendingFrees + models::kListHead);
  return {head, Load(base, head + models::kNodePrevious),
          Load(base, models::kPendingFrees + models::kListSize)};
}

// After a case: the nodes added at the end (oldest first) kept by their
// values and links, the list put back as it was, the nodes freed.
inline void TakeAddedNodes(uint8_t* base, const ListState& was, Outputs& out) {
  std::vector<uint32_t> added;
  for (uint32_t node = Load(base, was.head + models::kNodePrevious);
       node != was.previous && added.size() < 8;
       node = Load(base, node + models::kNodePrevious)) {
    added.insert(added.begin(), node);
  }
  out.Word(Load(base, models::kPendingFrees + models::kListSize) - was.size);
  out.Word(uint32_t(added.size()));
  for (size_t i = 0; i < added.size(); i++) {
    const uint32_t node = added[i];
    const uint32_t next = i + 1 < added.size() ? added[i + 1] : was.head;
    const uint32_t previous = i > 0 ? added[i - 1] : was.previous;
    out.Word(Load(base, node + models::kNodeValue));
    out.Word(Load(base, node + models::kNodeNext) == next);
    out.Word(Load(base, node + models::kNodePrevious) == previous);
  }
  out.Word(Load(base, was.previous + models::kNodeNext) ==
           (added.empty() ? was.head : added[0]));
  Store(base, was.head + models::kNodePrevious, was.previous);
  Store(base, was.previous + models::kNodeNext, was.head);
  Store(base, models::kPendingFrees + models::kListSize, was.size);
  for (const uint32_t node : added) {
    memory::crt::OperatorDelete(node);
  }
}

// A block's pointer for a case: 0 (none), an untagged header's block, or a
// tagged one's; the header at `header`.
inline uint32_t WriteBlock(uint8_t* base, uint32_t kind, uint32_t header, const uint32_t words[3]) {
  if (kind == 0) {
    return 0;
  }
  Store(base, header, kind == 1 ? 0x584C4D4C : models::kBlockTag);
  Store(base, header + 4, words[0]);
  Store(base, header + 8, words[1]);
  Store(base, header + 12, words[2]);
  return header + models::kBlockHeaderBytes;
}

}  // namespace harness::pending_frees
