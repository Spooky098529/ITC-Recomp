// The memory blocks' put-off frees (see memory_blocks.h), store for store as
// the recompiled functions.

#include "models/memory_blocks.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/models.h"
#include "std_list/ptr_list.h"

#include <cstdlib>
#include <vector>

namespace models {

namespace {

using Header = schema::MemoryBlockHeader;
using List = schema::PendingFreesList;
using Node = schema::PendingFreesNode;

// The pending frees at 0x8296BD54.
List PendingFrees(uint8_t* base) {
  return List(base, schema::ModelsGlobals::pending_frees_address());
}

// The block's header, when the block is one (not null, `XLMM`); else 0.
uint32_t HeaderOf(uint8_t* base, uint32_t block) {
  if (block == 0) {
    return 0;
  }
  const uint32_t header = block - kBlockHeaderBytes;
  return Header(base, header).tag() == kBlockTag ? header : 0;
}

// The header put at the end of the pending frees, its checks made.
void Queue(uint8_t* base, uint32_t header) {
  const uint32_t head = PendingFrees(base).head().address;
  if (!PendingFreesInsert(base, PendingFrees(base).address, head, header)) {
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
  guest::StoreU32(base, lock, critical_section);
  MemoryBlocksMutex().lock();
}

uint32_t ScopedLockLeave(uint8_t* base, uint32_t lock) {
  ITC_NATIVE_RUNNING("sub_8258E5B8 ScopedLock::~ScopedLock");
  const uint32_t critical_section = guest::LoadU32(base, lock);
  MemoryBlocksMutex().unlock();
  return critical_section;
}

bool PendingFreesIncSize(uint8_t* base, uint32_t list, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8258E2F8 PendingFrees::IncSize");
  const List l(base, list);
  const uint32_t size = l.size();
  if (0x3FFFFFFFu - size < count) {
    return false;
  }
  l.set_size(size + count);
  return true;
}

bool PendingFreesCanInsert(uint8_t* base, uint32_t list) {
  return std_list::CanAllocate(base, 1) && 0x3FFFFFFFu - List(base, list).size() >= 1;
}

bool PendingFreesInsert(uint8_t* base, uint32_t list, uint32_t where, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8258E398 PendingFrees::Insert");
  if (!PendingFreesCanInsert(base, list)) {
    return false;
  }
  const uint32_t previous = Node(base, where).previous().address;
  // A host object (E5 step 5): freed by PendingFrees::FreeDue's operator
  // delete. The head node stays in guest memory (the type is dual).
  const Node node = Node::New(base);
  // The original's null checks on the node, its +4 and its +8.
  if (node.address != 0) {
    node.set_next(where);
  }
  if (node.previous_address() != 0) {
    node.set_previous(previous);
  }
  if (node.header_address() != 0) {
    node.set_header(value);
  }
  PendingFreesIncSize(base, list, 1);
  Node(base, where).set_previous(node);
  // The node before it: its next (+0).
  node.previous().set_next(node);
  return true;
}

bool PendingFreesHaveRoom(uint8_t* base, uint32_t count) {
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  return std_list::CanAllocate(base, 1) &&
         0x3FFFFFFFu - PendingFrees(base).size() >= count;
}

bool FreeLater(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_8258E410 Memory::FreeLater");
  std::lock_guard<std::recursive_mutex> lock(MemoryBlocksMutex());
  const uint32_t header = HeaderOf(base, block);
  if (header == 0) {
    return true;
  }
  if (!PendingFreesCanInsert(base, PendingFrees(base).address)) {
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
  if (!PendingFreesCanInsert(base, PendingFrees(base).address)) {
    return false;
  }
  Header(base, header).set_attributes(attributes);
  Queue(base, header);
  return true;
}

namespace {

uint32_t PagesFor(uint32_t bytes) { return (bytes + 8191) & 0xFFFFF000u; }

// The header written after a successful allocation, in the originals' store
// orders (each maker's own).
uint32_t Made(uint8_t* base, uint32_t header, uint32_t kind) {
  if (header == 0) {
    return 0;
  }
  const Header h(base, header);
  switch (kind) {
    case 0:
      h.set_passes(1u);
      h.set_tag(kBlockTag);
      h.set_kind(0u);
      h.set_attributes(0u);
      break;
    case 1:
      h.set_attributes(0u);
      h.set_tag(kBlockTag);
      h.set_kind(1u);
      h.set_passes(1u);
      break;
    default:
      h.set_kind(2u);
      h.set_tag(kBlockTag);
      h.set_passes(1u);
      h.set_attributes(0u);
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
  const List list = PendingFrees(base);
  const uint32_t head = list.head().address;
  // The checks first, over the due headers.
  std::vector<uint32_t> due;
  for (uint32_t node = Node(base, head).next().address; node != head;
       node = Node(base, node).next().address) {
    const uint32_t header = Node(base, node).header().address;
    if (Header(base, header).passes() != 0) {
      continue;
    }
    for (const uint32_t other : due) {
      if (other == header) {
        return false;
      }
    }
    due.push_back(header);
    const uint32_t kind = Header(base, header).kind();
    if (!memory::IsHeapBlock(node) || (kind == 0 && !memory::IsPages(header)) ||
        (kind == 2 && !(Header(base, header).attributes() & kXMemPhysical) &&
         !memory::IsHeapBlock(header))) {
      return false;
    }
  }
  uint32_t node = Node(base, head).next().address;
  while (list.head().address != node) {
    const Node n(base, node);
    const Header h = n.header();
    const uint32_t header = h.address;
    const uint32_t passes = h.passes();
    if (passes != 0) {
      h.set_passes(passes - 1);
      node = n.next().address;
      continue;
    }
    const uint32_t kind = h.kind();
    h.set_tag(0u);
    if (kind == 0) {
      memory::FreePages(header);
    } else if (kind == 1) {
      memory::FreePhysical(header);
    } else if (kind == 2) {
      memory::crt::XMemFree(header, h.attributes());
    }
    const uint32_t next = n.next().address;
    if (node != list.head().address) {
      n.previous().set_next(next);
      n.next().set_previous(n.previous());
      memory::crt::OperatorDelete(node);
      list.set_size(list.size() - 1);
    }
    node = next;
  }
  return true;
}

}  // namespace models
