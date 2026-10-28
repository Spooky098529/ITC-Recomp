// The archive streaming's functions as plain C++: see archive.h.

#include "archive/archive.h"

#include "base/log.h"
#include "guest.h"

namespace archive {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Toc(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209D868 Archive::Toc");
  return LoadU32(base, id < kFirstCrsId ? kBuildToc : kCrsToc);
}

uint32_t AssetSize(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820A0678 Archive::AssetSize");
  const uint32_t index = id < kFirstCrsId ? id : id - kFirstCrsId;
  const uint32_t entry = Toc(base, id) + ((index + 1) << 4);
  const uint32_t unpacked = LoadU32(base, entry + 8);
  return unpacked != 0 ? unpacked : LoadU32(base, entry + 4);
}

// --- The request queue's list and slots ---

void ListConstruct(uint8_t* base, uint32_t list, uint32_t deletes) {
  ITC_NATIVE_RUNNING("sub_82294B80 List::List");
  StoreU32(base, list + 4, deletes);
  StoreU32(base, list + 0, kListVtable);
  StoreU32(base, list + 8, 0);
  StoreU32(base, list + 12, 0);
  StoreU32(base, list + 16, 0);
}

uint32_t ListAppend(uint8_t* base, uint32_t list, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82294BA8 List::Append");
  if (node == 0) {
    return 0;
  }
  const uint32_t tail = LoadU32(base, list + 16);
  if (tail != 0) {
    StoreU32(base, tail + 4, node);
  } else {
    StoreU32(base, list + 12, node);
  }
  StoreU32(base, node + 4, 0);
  StoreU32(base, list + 16, node);
  StoreU32(base, list + 8, LoadU32(base, list + 8) + 1);
  return 1;
}

uint32_t ListRemove(uint8_t* base, uint32_t list, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82294C00 List::Remove");
  if (node == 0) {
    return 0;
  }
  uint32_t previous = 0;
  uint32_t at = LoadU32(base, list + 12);
  while (at != 0 && at != node) {
    previous = at;
    at = LoadU32(base, at + 4);
  }
  if (at == 0) {
    return 0;
  }
  if (previous != 0) {
    StoreU32(base, previous + 4, LoadU32(base, at + 4));
  }
  if (LoadU32(base, list + 12) == at) {
    StoreU32(base, list + 12, LoadU32(base, at + 4));
  }
  if (LoadU32(base, list + 16) == at) {
    StoreU32(base, list + 16, previous);
  }
  StoreU32(base, list + 8, LoadU32(base, list + 8) - 1);
  return 1;
}

void NodeConstruct(uint8_t* base, uint32_t node, uint32_t link) {
  ITC_NATIVE_RUNNING("sub_82294B68 ListNode::ListNode");
  StoreU32(base, node + 4, link);
  StoreU32(base, node + 0, kNodeVtable);
}

uint32_t FindSlot(uint8_t* base, uint32_t serial) {
  ITC_NATIVE_RUNNING("sub_8209D818 Archive::FindSlot");
  for (uint32_t i = 0; i < kSlotCount; i++) {
    const uint32_t slot = kSlots + i * kSlotBytes;
    if (LoadU32(base, slot + 8) == serial) {
      return slot;
    }
  }
  return 0;
}

int32_t QueueStaticInit(uint8_t* base, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D9160 RequestQueue::StaticInit");
  ListConstruct(base, kQueue, 0);
  StoreU32(base, kQueue, kQueueVtable);
  return c_runtime::AtexitFrom(base, kQueueCleanUp, call);
}

int32_t SlotsStaticInit(uint8_t* base, const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825D91B0 RequestSlots::StaticInit");
  for (uint32_t i = 0; i < kSlotCount; i++) {
    const uint32_t slot = kSlots + i * kSlotBytes;
    NodeConstruct(base, slot, 0);
    StoreU32(base, slot, kSlotVtable);
  }
  return c_runtime::AtexitFrom(base, kSlotsCleanUp, call);
}

// --- The loose files ---

uint32_t FindLooseFile(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8209D8D8 Archive::FindLooseFile");
  for (uint32_t step = 0;; step++) {
    const uint32_t entry = (step < kFirstCrsId ? kLooseFiles : kCrsLooseFiles) + 8 * step;
    const uint32_t path = LoadU32(base, entry + 4);
    if (path == 0) {
      return 0;
    }
    if (LoadU32(base, entry) == id) {
      return path;
    }
  }
}

}  // namespace archive
