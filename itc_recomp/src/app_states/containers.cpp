// The containers the app's update and its state stack use, natively
// (containers.h, docs/functions/app-states.md).

#include "app_states/containers.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "schema/app_states.h"

namespace app_states {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void Delete(uint32_t block, const char* what) {
  if (!memory::crt::OperatorDelete(block)) {
    ITC_LOG_ERROR("{}: operator delete refused {:08X}", what, block);
  }
}

}  // namespace

bool MessagesErase(uint8_t* base, uint32_t vector, uint32_t first_vector, uint32_t first,
                   uint32_t last_vector, uint32_t last) {
  if (first_vector == 0 || first_vector != last_vector) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82092E70 MessageVector::Erase");
  const schema::MessageVector v(base, vector);
  if (first != last) {
    const uint32_t end = v.last().address;
    const uint32_t moved = uint32_t(int32_t(end - last) >> 2) << 2;
    const uint32_t new_last = first + moved;
    if (last != end) {
      const uint32_t delta = first - last;
      for (uint32_t from = last; from != end; from += 4) {
        Store(base, from + delta, Load(base, from));
      }
    }
    v.set_last(new_last);
  }
  return true;
}

bool MessagesClear(uint8_t* base, uint32_t vector) {
  const schema::MessageVector v(base, vector);
  const uint32_t first = v.first().address;
  const uint32_t last = v.last().address;
  if (first > last || vector == 0) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82092F98 MessageVector::Clear");
  return MessagesErase(base, vector, vector, first, vector, last);
}

std::optional<uint32_t> MessagesAllocate(uint8_t* base, uint32_t count) {
  if (count != 0 && 0xFFFFFFFFu / count < 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82092B78 MessageVector::Allocate");
  return memory::crt::OperatorNew(base, count * 4);
}

std::optional<uint32_t> OpAllocateNodes(uint8_t* base, uint32_t count) {
  constexpr uint32_t kNodeBytes = schema::AppOpListNode::kSize;
  if (count != 0 && 0xFFFFFFFFu / count < kNodeBytes) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ACF10 OpList::AllocateNodes");
  if (count == 1) {
    // A node in host memory (E5 step 4): the list's only use (OpList::OpList
    // and BuyNode ask for one).
    return std::optional<uint32_t>(schema::AppOpListNode::New(base).address);
  }
  return memory::crt::OperatorNew(base, count * kNodeBytes);
}

bool OpIncSize(uint8_t* base, uint32_t list, uint32_t count) {
  const schema::OpList ops(base, list);
  const uint32_t size = ops.size();
  if (kOpListMaxSize - size < count) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AD768 OpList::IncSize");
  ops.set_size(size + count);
  return true;
}

bool OpConstruct(uint8_t* base, uint32_t list) {
  if (memory::crt::GetProcessHeap(base) == 0) {
    return false;
  }
  const auto head = OpAllocateNodes(base, 1);
  if (!head) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AD0B8 OpList::OpList");
  // The original's null checks on the node and its +4.
  const schema::AppOpListNode node(base, *head);
  if (*head != 0) {
    node.set_next(*head);
  }
  if (*head + 4 != 0) {
    node.set_previous(*head);
  }
  const schema::OpList ops(base, list);
  ops.set_head(*head);
  ops.set_size(0);
  return true;
}

std::optional<uint32_t> OpBuyNode(uint8_t* base, uint32_t next, uint32_t previous, uint32_t type,
                                  uint32_t state) {
  if (memory::crt::GetProcessHeap(base) == 0) {
    return std::nullopt;
  }
  const auto node = OpAllocateNodes(base, 1);
  if (!node) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ACFF8 OpList::BuyNode");
  const schema::AppOpListNode n(base, *node);
  if (*node != 0) {
    n.set_next(next);
  }
  if (*node + 4 != 0) {
    n.set_previous(previous);
  }
  if (*node + 8 != 0) {
    n.set_op_type(type);
    n.set_op_state(state);
  }
  return *node;
}

bool LayerPush(uint8_t* base, uint32_t layer, uint32_t state) {
  const uint32_t list = schema::StateLayer(base, layer).ops_list().address;
  const schema::OpList ops(base, list);
  if (kOpListMaxSize - ops.size() < 1) {
    return false;
  }
  const auto head = ops.head();
  const auto node = OpBuyNode(base, head.address, head.previous().address, 2, state);
  if (!node) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820ADAD0 StateLayer::Push");
  OpIncSize(base, list, 1);
  head.set_previous(*node);
  schema::AppOpListNode(base, *node).previous().set_next(*node);
  return true;
}

namespace {

// (to - from) / 4 as the original's srawi.
uint32_t Words(uint32_t from, uint32_t to) { return uint32_t(int32_t(to - from) >> 2); }

}  // namespace

bool MessagesInsertN(uint8_t* base, uint32_t vector, uint32_t where, uint32_t count,
                     uint32_t value) {
  const schema::MessageVector v(base, vector);
  const uint32_t first = v.first().address;
  const uint32_t capacity = first == 0 ? 0 : Words(first, v.end().address);
  if (count == 0) {
    ITC_NATIVE_RUNNING("sub_820938C0 MessageVector::InsertN");
    return true;
  }
  const uint32_t last = v.last().address;
  const uint32_t size = first == 0 ? 0 : Words(first, last);
  if (kMessagesMaxSize - size < count) {
    return false;  // the length_error
  }
  if (capacity < size + count) {
    uint32_t new_capacity =
        kMessagesMaxSize - (capacity >> 1) < capacity ? 0 : capacity + (capacity >> 1);
    if (new_capacity < size + count) {
      new_capacity = size + count;
    }
    if (memory::crt::GetProcessHeap(base) == 0) {
      return false;
    }
    const auto allocated = MessagesAllocate(base, new_capacity);
    if (!allocated) {
      return false;
    }
    ITC_NATIVE_RUNNING("sub_820938C0 MessageVector::InsertN");
    const uint32_t block = *allocated;
    uint32_t to = block;
    for (uint32_t from = first; from != where; from += 4, to += 4) {
      if (to != 0) {
        Store(base, to, Load(base, from));
      }
    }
    uint32_t fill = to;
    for (uint32_t i = count; i != 0; i--, fill += 4) {
      if (fill != 0) {
        Store(base, fill, value);
      }
    }
    const uint32_t old_last = v.last().address;
    if (where != old_last) {
      uint32_t moved = to + 4 * count;
      for (uint32_t from = where; from != old_last; from += 4, moved += 4) {
        if (moved != 0) {
          Store(base, moved, Load(base, from));
        }
      }
    }
    const uint32_t old_first = v.first().address;
    const uint32_t new_size = (old_first == 0 ? 0 : Words(old_first, v.last().address)) + count;
    if (old_first != 0) {
      Delete(old_first, "MessageVector::InsertN");
    }
    v.set_first(block);
    v.set_end(block + 4 * new_capacity);
    v.set_last(block + 4 * new_size);
    return true;
  }
  ITC_NATIVE_RUNNING("sub_820938C0 MessageVector::InsertN");
  if (Words(where, last) < count) {
    // The tail moved past the new words, the rest of them after the old
    // last, then the tail's old place filled.
    uint32_t to = where + 4 * count;
    for (uint32_t from = where; from != last; from += 4, to += 4) {
      if (to != 0) {
        Store(base, to, Load(base, from));
      }
    }
    uint32_t fill = v.last().address;
    for (uint32_t left = count - Words(where, fill); left != 0; left--, fill += 4) {
      if (fill != 0) {
        Store(base, fill, value);
      }
    }
    const uint32_t new_last = v.last().address + 4 * count;
    v.set_last(new_last);
    for (uint32_t at = where; at != new_last - 4 * count; at += 4) {
      Store(base, at, value);
    }
    return true;
  }
  // The last `count` words copied past the end, the rest of the tail moved
  // back by `count` (copy_backward), then the gap filled.
  const uint32_t bytes = 4 * count;
  const uint32_t split = last - bytes;
  uint32_t to = v.last().address;
  for (uint32_t from = split; from != last; from += 4, to += 4) {
    if (to != 0) {
      Store(base, to, Load(base, from));
    }
  }
  v.set_last(to);
  if (where != split) {
    uint32_t at = split;
    do {
      at -= 4;
      Store(base, at + bytes, Load(base, at));
    } while (at != where);
  }
  for (uint32_t at = where; at != where + bytes; at += 4) {
    Store(base, at, value);
  }
  return true;
}

std::optional<uint32_t> MessagesInsert(uint8_t* base, uint32_t vector, uint32_t where_vector,
                                       uint32_t where, uint32_t value) {
  const schema::MessageVector v(base, vector);
  const uint32_t first = v.first().address;
  const uint32_t last = v.last().address;
  int32_t offset = 0;
  if (first != 0 && Words(first, last) != 0) {
    if (first > last || where_vector == 0 || where_vector != vector) {
      return std::nullopt;  // the traps
    }
    offset = int32_t(Words(first, where));
  }
  const int32_t size = first == 0 ? 0 : int32_t(Words(first, last));
  if (offset < 0 || offset > size + 1) {
    return std::nullopt;  // the result's traps, after the insert
  }
  if (!MessagesInsertN(base, vector, where, 1, schema::AppMessageWord(base, value).value())) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82094110 MessageVector::Insert");
  return v.first().address + uint32_t(offset) * 4;
}

std::optional<bool> MessagesPushBack(uint8_t* base, uint32_t vector, uint32_t value) {
  const schema::MessageVector v(base, vector);
  const uint32_t first = v.first().address;
  const uint32_t last = v.last().address;
  if (first != 0 && Words(first, last) < Words(first, v.end().address)) {
    ITC_NATIVE_RUNNING("sub_820944E0 MessageVector::PushBack");
    if (last != 0) {
      schema::AppMessageWord(base, last).set_value(schema::AppMessageWord(base, value).value());
    }
    v.set_last(last + 4);
    return false;
  }
  if (first > last || !MessagesInsert(base, vector, vector, last, value)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820944E0 MessageVector::PushBack");
  return true;
}

namespace {

// List::Clear over nodes of type `Node` (a list of pointers' StateListNode in
// guest memory, or an operation list's AppOpListNode in host memory).
template <typename Node>
uint32_t ClearNodes(uint8_t* base, const schema::PtrList& l) {
  auto head = l.head<Node>();
  uint32_t node = head.next().address;
  head.set_next(head.address);
  head = l.head<Node>();
  head.set_previous(head.address);
  head = l.head<Node>();
  l.set_size(0);
  while (node != head.address) {
    const uint32_t next = Node(base, node).next().address;
    Delete(node, "List::Clear");
    head = l.head<Node>();
    node = next;
  }
  return node;
}

}  // namespace

uint32_t ListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820ACF88 List::Clear");
  // Any list with 16-byte or other nodes: the layout of a list of pointers
  // (+4 the head, +8 the size; a node +0 next, +4 previous). The operation
  // lists' nodes are in host memory (E5 step 4), told by their handles.
  const schema::PtrList l(base, list);
  if (memory::IsHandle(l.head<schema::View>().address)) {
    return ClearNodes<schema::AppOpListNode>(base, l);
  }
  return ClearNodes<schema::StateListNode>(base, l);
}

std::optional<uint32_t> DelegateClone(uint8_t* base, uint32_t delegate) {
  const auto block = std::optional<uint32_t>(schema::AppDelegate::New(base).address);  // host memory (E5 step 4)
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ACA48 Delegate::Clone");
  if (*block == 0) {
    return 0;
  }
  const schema::AppDelegate copy(base, *block), from(base, delegate);
  copy.set_vtable(kDelegateVtable);
  copy.set_object(from.object());
  copy.set_function(from.function());
  return *block;
}

uint32_t LayerIsEmpty(uint8_t* base, uint32_t state) {
  ITC_NATIVE_RUNNING("sub_820ACE50 StateLayer::IsEmpty");
  const schema::StateLayer layer(base, state - 4);
  return layer.states_size() == 0 && layer.children_size() == 0 ? 1 : 0;
}

}  // namespace app_states
