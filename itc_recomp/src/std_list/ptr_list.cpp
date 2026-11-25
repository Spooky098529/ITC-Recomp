// The C++ library's std::list of pointers (see ptr_list.h), store for store
// as the recompiled functions.

#include "std_list/ptr_list.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/app_states.h"
#include "schema/input.h"

#include <cstdlib>

namespace std_list {

namespace {

// A node allocated after CanAllocate said it could be.
uint32_t NewNode() {
  const auto node = AllocateNodes(1);
  if (!node) {
    ITC_LOG_ERROR("PtrList: operator new refused a node after its checks");
    std::abort();
  }
  return *node;
}

}  // namespace

bool CanAllocate(uint32_t count) {
  // The original's bad_alloc path: 0xFFFFFFFF / count below 12.
  if (count != 0 && 0xFFFFFFFFu / count < schema::PtrListNode::kSize) {
    return false;
  }
  return count * schema::PtrListNode::kSize <= 0xFFFFF000u && memory::crt::GetProcessHeap() != 0;
}

std::optional<uint32_t> AllocateNodes(uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82099C08 PtrList::AllocateNodes");
  if (count != 0 && 0xFFFFFFFFu / count < schema::PtrListNode::kSize) {
    return std::nullopt;
  }
  // In host memory (E5 step 4): one array of `count` nodes behind one handle
  // (a list's node, or RequestVector's block of 12-byte requests), freed by
  // the game's operator delete. Operator new(0) gives a block: one element.
  return schema::PtrListNode::NewArray(count != 0 ? count : 1).address;
}

bool IncSize(uint32_t list, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_820958B8 PtrList::IncSize");
  const schema::PtrList l(list);
  const uint32_t size = l.size();
  if (0x3FFFFFFFu - size < count) {
    return false;
  }
  l.set_size(size + count);
  return true;
}

bool Construct(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820AD058 PtrList::PtrList");
  if (!CanAllocate(1)) {
    return false;
  }
  const schema::PtrListNode node(NewNode());
  // The original's null checks on the node and its +4.
  if (node.address != 0) {
    node.set_next(node);
  }
  if (node.previous_address() != 0) {
    node.set_previous(node);
  }
  const schema::PtrList l(list);
  l.set_head(node.address);
  l.set_size(0);
  return true;
}

bool Insert(uint32_t list, uint32_t where, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82095958 PtrList::Insert");
  if (!CanAllocate(1) || 0x3FFFFFFFu - schema::PtrList(list).size() < 1) {
    return false;
  }
  const schema::PtrListNode at(where);
  const uint32_t previous = at.previous().address;
  const schema::PtrListNode node(NewNode());
  if (node.address != 0) {
    node.set_next(where);
  }
  if (node.previous_address() != 0) {
    node.set_previous(previous);
  }
  if (node.value_address() != 0) {
    node.set_value(value);
  }
  IncSize(list, 1);
  at.set_previous(node);
  node.previous().set_next(node);
  return true;
}

std::optional<uint32_t> Erase(uint32_t list, uint32_t iterator_list, uint32_t node) {
  if (iterator_list == 0 ||
      node == schema::PtrList(iterator_list).head<schema::PtrListNode>().address) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82098E80 PtrList::Erase");
  const schema::PtrListNode n(node);
  const uint32_t next = n.next().address;
  const schema::PtrList l(list);
  if (node != l.head<schema::PtrListNode>().address) {
    const schema::PtrListNode before = n.previous();
    before.set_next(n.next());
    const schema::PtrListNode after = n.next();
    after.set_previous(n.previous());
    if (!memory::crt::OperatorDelete(node)) {
      ITC_LOG_ERROR("PtrList::Erase: operator delete refused {:08X}", node);
    }
    l.set_size(l.size() - 1);
  }
  return next;
}

}  // namespace std_list
