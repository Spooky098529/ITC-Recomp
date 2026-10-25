// The memory blocks' put-off frees (see memory_blocks.h), store for store as
// the recompiled functions.

#include "models/memory_blocks.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "std_list/ptr_list.h"

#include <cstdlib>

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

}  // namespace models
