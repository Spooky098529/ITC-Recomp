// The C++ library's std::list of pointers (see ptr_list.h), store for store
// as the recompiled functions.

#include "std_list/ptr_list.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

#include <cstdlib>

namespace std_list {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// A node allocated after CanAllocate said it could be.
uint32_t NewNode(uint8_t* base) {
  const auto node = AllocateNodes(base, 1);
  if (!node) {
    ITC_LOG_ERROR("PtrList: operator new refused a node after its checks");
    std::abort();
  }
  return *node;
}

}  // namespace

bool CanAllocate(uint8_t* base, uint32_t count) {
  // The original's bad_alloc path: 0xFFFFFFFF / count below 12.
  if (count != 0 && 0xFFFFFFFFu / count < kNodeBytes) {
    return false;
  }
  return count * kNodeBytes <= 0xFFFFF000u && memory::crt::GetProcessHeap(base) != 0;
}

std::optional<uint32_t> AllocateNodes(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82099C08 PtrList::AllocateNodes");
  if (count != 0 && 0xFFFFFFFFu / count < kNodeBytes) {
    return std::nullopt;
  }
  return memory::crt::OperatorNew(base, count * kNodeBytes);
}

bool IncSize(uint8_t* base, uint32_t list, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_820958B8 PtrList::IncSize");
  const uint32_t size = Load(base, list + kSize);
  if (0x3FFFFFFFu - size < count) {
    return false;
  }
  Store(base, list + kSize, size + count);
  return true;
}

bool Construct(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820AD058 PtrList::PtrList");
  if (!CanAllocate(base, 1)) {
    return false;
  }
  const uint32_t node = NewNode(base);
  // The original's null checks on the node and its +4.
  if (node != 0) {
    Store(base, node + kNext, node);
  }
  if (node + kPrevious != 0) {
    Store(base, node + kPrevious, node);
  }
  Store(base, list + kHead, node);
  Store(base, list + kSize, 0);
  return true;
}

bool Insert(uint8_t* base, uint32_t list, uint32_t where, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82095958 PtrList::Insert");
  if (!CanAllocate(base, 1) || 0x3FFFFFFFu - Load(base, list + kSize) < 1) {
    return false;
  }
  const uint32_t previous = Load(base, where + kPrevious);
  const uint32_t node = NewNode(base);
  if (node != 0) {
    Store(base, node + kNext, where);
  }
  if (node + kPrevious != 0) {
    Store(base, node + kPrevious, previous);
  }
  if (node + kValue != 0) {
    Store(base, node + kValue, value);
  }
  IncSize(base, list, 1);
  Store(base, where + kPrevious, node);
  Store(base, Load(base, node + kPrevious), node);
  return true;
}

std::optional<uint32_t> Erase(uint8_t* base, uint32_t list, uint32_t iterator_list, uint32_t node) {
  if (iterator_list == 0 || node == Load(base, iterator_list + kHead)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82098E80 PtrList::Erase");
  const uint32_t next = Load(base, node + kNext);
  if (node != Load(base, list + kHead)) {
    Store(base, Load(base, node + kPrevious) + kNext, Load(base, node + kNext));
    Store(base, Load(base, node + kNext) + kPrevious, Load(base, node + kPrevious));
    if (!memory::crt::OperatorDelete(node)) {
      ITC_LOG_ERROR("PtrList::Erase: operator delete refused {:08X}", node);
    }
    Store(base, list + kSize, Load(base, list + kSize) - 1);
  }
  return next;
}

}  // namespace std_list
