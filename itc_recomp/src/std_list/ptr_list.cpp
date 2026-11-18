// The C++ library's std::list of pointers (see ptr_list.h), store for store
// as the recompiled functions.

#include "std_list/ptr_list.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/input.h"

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
  if (count != 0 && 0xFFFFFFFFu / count < schema::PtrListNode::kSize) {
    return false;
  }
  return count * schema::PtrListNode::kSize <= 0xFFFFF000u && memory::crt::GetProcessHeap(base) != 0;
}

std::optional<uint32_t> AllocateNodes(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82099C08 PtrList::AllocateNodes");
  if (count != 0 && 0xFFFFFFFFu / count < schema::PtrListNode::kSize) {
    return std::nullopt;
  }
  return memory::crt::OperatorNew(base, count * schema::PtrListNode::kSize);
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
  const schema::PtrListNode node(base, NewNode(base));
  // The original's null checks on the node and its +4.
  if (node.address != 0) {
    node.set_next(node);
  }
  if (node.previous_address() != 0) {
    node.set_previous(node);
  }
  Store(base, list + kHead, node.address);
  Store(base, list + kSize, 0);
  return true;
}

bool Insert(uint8_t* base, uint32_t list, uint32_t where, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82095958 PtrList::Insert");
  if (!CanAllocate(base, 1) || 0x3FFFFFFFu - Load(base, list + kSize) < 1) {
    return false;
  }
  const schema::PtrListNode at(base, where);
  const uint32_t previous = at.previous().address;
  const schema::PtrListNode node(base, NewNode(base));
  if (node.address != 0) {
    node.set_next(where);
  }
  if (node.previous_address() != 0) {
    node.set_previous(previous);
  }
  if (node.value_address() != 0) {
    node.set_value(value);
  }
  IncSize(base, list, 1);
  at.set_previous(node);
  node.previous().set_next(node);
  return true;
}

std::optional<uint32_t> Erase(uint8_t* base, uint32_t list, uint32_t iterator_list, uint32_t node) {
  if (iterator_list == 0 || node == Load(base, iterator_list + kHead)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82098E80 PtrList::Erase");
  const schema::PtrListNode n(base, node);
  const uint32_t next = n.next().address;
  if (node != Load(base, list + kHead)) {
    const schema::PtrListNode before = n.previous();
    before.set_next(n.next());
    const schema::PtrListNode after = n.next();
    after.set_previous(n.previous());
    if (!memory::crt::OperatorDelete(node)) {
      ITC_LOG_ERROR("PtrList::Erase: operator delete refused {:08X}", node);
    }
    Store(base, list + kSize, Load(base, list + kSize) - 1);
  }
  return next;
}

}  // namespace std_list
