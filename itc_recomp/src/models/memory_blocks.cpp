// The memory blocks' put-off frees (see memory_blocks.h), store for store as
// the recompiled functions.

#include "models/memory_blocks.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_list/ptr_list.h"

#include <cstdlib>
#include <vector>

namespace models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// The block's header, when the block is one (not null, `XLMM`); else 0.
uint32_t HeaderOf(uint8_t* base, uint32_t block) {
  if (block == 0) {
    return 0;
  }
  const uint32_t header = block - kBlockHeaderBytes;
  return Load(base, header) == kBlockTag ? header : 0;
}

// The header put at the end of the pending frees, its checks made.
void Queue(uint8_t* base, uint32_t header) {
  const uint32_t head = Load(base, kPendingFrees + kListHead);
  if (!PendingFreesInsert(base, kPendingFrees, head, header)) {
    ITC_LOG_ERROR("Memory::FreeLater: the pending frees' insert refused after its checks");
    std::abort();
  }
}

}  // namespace

std::recursive_mutex& MemoryBlocksMutex() {
  static std::recursive_mutex mutex;
  return mutex;
}

void ScopedLockEnter(uint8_t* base, uint32_t lock, uint32_t critical_section) {
  ITC_NATIVE_RUNNING("sub_8258E580 ScopedLock::ScopedLock");
  Store(base, lock, critical_section);
  MemoryBlocksMutex().lock();
}

uint32_t ScopedLockLeave(uint8_t* base, uint32_t lock) {
  ITC_NATIVE_RUNNING("sub_8258E5B8 ScopedLock::~ScopedLock");
  const uint32_t critical_section = Load(base, lock);
  MemoryBlocksMutex().unlock();
  return critical_section;
}

bool PendingFreesIncSize(uint8_t* base, uint32_t list, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8258E2F8 PendingFrees::IncSize");
  const uint32_t size = Load(base, list + kListSize);
  if (0x3FFFFFFFu - size < count) {
    return false;
  }
  Store(base, list + kListSize, size + count);
  return true;
}

bool PendingFreesCanInsert(uint8_t* base, uint32_t list) {
  return std_list::CanAllocate(base, 1) && 0x3FFFFFFFu - Load(base, list + kListSize) >= 1;
}

bool PendingFreesInsert(uint8_t* base, uint32_t list, uint32_t where, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8258E398 PendingFrees::Insert");
  if (!PendingFreesCanInsert(base, list)) {
    return false;
  }
  const uint32_t previous = Load(base, where + kNodePrevious);
  const auto made = memory::crt::OperatorNew(base, kNodeBytes);
  if (!made) {
    ITC_LOG_ERROR("PendingFrees::Insert: operator new refused a node after its checks");
    std::abort();
  }
  const uint32_t node = *made;
  // The original's null checks on the node, its +4 and its +8.
  if (node != 0) {
    Store(base, node + kNodeNext, where);
  }
  if (node + kNodePrevious != 0) {
    Store(base, node + kNodePrevious, previous);
  }
  if (node + kNodeValue != 0) {
    Store(base, node + kNodeValue, value);
  }
  PendingFreesIncSize(base, list, 1);
  Store(base, where + kNodePrevious, node);
  Store(base, Load(base, node + kNodePrevious), node);
  return true;
}

bool PendingFreesHaveRoom(uint8_t* base, uint32_t count) {
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  return std_list::CanAllocate(base, 1) &&
         0x3FFFFFFFu - Load(base, kPendingFrees + kListSize) >= count;
}

bool FreeLater(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_8258E410 Memory::FreeLater");
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  const uint32_t header = HeaderOf(base, block);
  if (header == 0) {
    return true;
  }
  if (!PendingFreesCanInsert(base, kPendingFrees)) {
    return false;
  }
  Queue(base, header);
  return true;
}

bool FreePhysicalLater(uint8_t* base, uint32_t block, uint32_t attributes) {
  ITC_NATIVE_RUNNING("sub_8258E498 Memory::FreePhysicalLater");
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  const uint32_t header = HeaderOf(base, block);
  if (header == 0) {
    return true;
  }
  if (!PendingFreesCanInsert(base, kPendingFrees)) {
    return false;
  }
  Store(base, header + kBlockAttributes, attributes);
  Queue(base, header);
  return true;
}

namespace {

constexpr uint32_t kBlockKind = 4;
constexpr uint32_t kBlockPasses = 8;

uint32_t PagesFor(uint32_t bytes) { return (bytes + 8191) & 0xFFFFF000u; }

// The header written after a successful allocation, in the originals' store
// orders (each maker's own).
uint32_t Made(uint8_t* base, uint32_t header, uint32_t kind) {
  if (header == 0) {
    return 0;
  }
  switch (kind) {
    case 0:
      Store(base, header + kBlockPasses, 1);
      Store(base, header + 0, kBlockTag);
      Store(base, header + kBlockKind, 0);
      Store(base, header + kBlockAttributes, 0);
      break;
    case 1:
      Store(base, header + kBlockAttributes, 0);
      Store(base, header + 0, kBlockTag);
      Store(base, header + kBlockKind, 1);
      Store(base, header + kBlockPasses, 1);
      break;
    default:
      Store(base, header + kBlockKind, 2);
      Store(base, header + 0, kBlockTag);
      Store(base, header + kBlockPasses, 1);
      Store(base, header + kBlockAttributes, 0);
      break;
  }
  return header + kBlockHeaderBytes;
}

constexpr uint32_t kXMemPhysical = 0x80000000u;  // XMemAlloc's physical bit

}  // namespace

uint32_t AllocVirtual(uint8_t* base, uint32_t bytes) {
  ITC_NATIVE_RUNNING("sub_8258DFF0 Memory::AllocVirtual");
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  return Made(base, memory::AllocPages(PagesFor(bytes), 4096), 0);
}

uint32_t AllocPhysicalEx(uint8_t* base, uint32_t bytes, uint32_t low, uint32_t high,
                         uint32_t alignment, uint32_t protection) {
  ITC_NATIVE_RUNNING("sub_8258E098 Memory::AllocPhysicalEx");
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  return Made(base,
              memory::crt::XPhysicalAllocEx(base, PagesFor(bytes), low, high, alignment,
                                            protection),
              1);
}

uint32_t AllocPhysical(uint8_t* base, uint32_t bytes, uint32_t attributes) {
  ITC_NATIVE_RUNNING("sub_8258E138 Memory::AllocPhysical");
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  return Made(base, memory::crt::XMemAlloc(base, PagesFor(bytes), attributes), 2);
}

bool FreeDue(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8258E1D8 PendingFrees::FreeDue");
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  const uint32_t head = Load(base, kPendingFrees + kListHead);
  // The checks first, over the due headers.
  std::vector<uint32_t> due;
  for (uint32_t node = Load(base, head + kNodeNext); node != head;
       node = Load(base, node + kNodeNext)) {
    const uint32_t header = Load(base, node + kNodeValue);
    if (Load(base, header + kBlockPasses) != 0) {
      continue;
    }
    for (const uint32_t other : due) {
      if (other == header) {
        return false;
      }
    }
    due.push_back(header);
    const uint32_t kind = Load(base, header + kBlockKind);
    if (!memory::IsHeapBlock(node) || (kind == 0 && !memory::IsPages(header)) ||
        (kind == 2 && !(Load(base, header + kBlockAttributes) & kXMemPhysical) &&
         !memory::IsHeapBlock(header))) {
      return false;
    }
  }
  uint32_t node = Load(base, head + kNodeNext);
  while (Load(base, kPendingFrees + kListHead) != node) {
    const uint32_t header = Load(base, node + kNodeValue);
    const uint32_t passes = Load(base, header + kBlockPasses);
    if (passes != 0) {
      Store(base, header + kBlockPasses, passes - 1);
      node = Load(base, node + kNodeNext);
      continue;
    }
    const uint32_t kind = Load(base, header + kBlockKind);
    Store(base, header + 0, 0);
    if (kind == 0) {
      memory::FreePages(header);
    } else if (kind == 1) {
      memory::FreePhysical(header);
    } else if (kind == 2) {
      memory::crt::XMemFree(header, Load(base, header + kBlockAttributes));
    }
    const uint32_t next = Load(base, node + kNodeNext);
    if (node != Load(base, kPendingFrees + kListHead)) {
      Store(base, Load(base, node + kNodePrevious) + kNodeNext, next);
      Store(base, Load(base, node + kNodeNext) + kNodePrevious, Load(base, node + kNodePrevious));
      memory::crt::OperatorDelete(node);
      Store(base, kPendingFrees + kListSize, Load(base, kPendingFrees + kListSize) - 1);
    }
    node = next;
  }
  return true;
}

}  // namespace models
