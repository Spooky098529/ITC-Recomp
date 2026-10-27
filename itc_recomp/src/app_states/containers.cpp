// The containers the app's update and its state stack use, natively
// (containers.h, docs/functions/app-states.md).

#include "app_states/containers.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"

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
  if (first != last) {
    const uint32_t end = Load(base, vector + kVectorLast);
    const uint32_t moved = uint32_t(int32_t(end - last) >> 2) << 2;
    const uint32_t new_last = first + moved;
    if (last != end) {
      const uint32_t delta = first - last;
      for (uint32_t from = last; from != end; from += 4) {
        Store(base, from + delta, Load(base, from));
      }
    }
    Store(base, vector + kVectorLast, new_last);
  }
  return true;
}

bool MessagesClear(uint8_t* base, uint32_t vector) {
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t last = Load(base, vector + kVectorLast);
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
  if (count != 0 && 0xFFFFFFFFu / count < kOpNodeBytes) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ACF10 OpList::AllocateNodes");
  return memory::crt::OperatorNew(base, count * kOpNodeBytes);
}

bool OpIncSize(uint8_t* base, uint32_t list, uint32_t count) {
  const uint32_t size = Load(base, list + kListSize);
  if (kOpListMaxSize - size < count) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AD768 OpList::IncSize");
  Store(base, list + kListSize, size + count);
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
  if (*head != 0) {
    Store(base, *head + kNodeNext, *head);
  }
  if (*head + kNodePrevious != 0) {
    Store(base, *head + kNodePrevious, *head);
  }
  Store(base, list + kListHead, *head);
  Store(base, list + kListSize, 0);
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
  if (*node != 0) {
    Store(base, *node + kNodeNext, next);
  }
  if (*node + kNodePrevious != 0) {
    Store(base, *node + kNodePrevious, previous);
  }
  if (*node + kNodeValue != 0) {
    Store(base, *node + kNodeValue, type);
    Store(base, *node + kNodeValue + 4, state);
  }
  return *node;
}

bool LayerPush(uint8_t* base, uint32_t layer, uint32_t state) {
  const uint32_t list = layer + kLayerOps;
  if (kOpListMaxSize - Load(base, list + kListSize) < 1) {
    return false;
  }
  const uint32_t head = Load(base, list + kListHead);
  const auto node = OpBuyNode(base, head, Load(base, head + kNodePrevious), 2, state);
  if (!node) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820ADAD0 StateLayer::Push");
  OpIncSize(base, list, 1);
  Store(base, head + kNodePrevious, *node);
  Store(base, Load(base, *node + kNodePrevious) + kNodeNext, *node);
  return true;
}

namespace {

// (to - from) / 4 as the original's srawi.
uint32_t Words(uint32_t from, uint32_t to) { return uint32_t(int32_t(to - from) >> 2); }

}  // namespace

bool MessagesInsertN(uint8_t* base, uint32_t vector, uint32_t where, uint32_t count,
                     uint32_t value) {
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t capacity = first == 0 ? 0 : Words(first, Load(base, vector + kVectorEnd));
  if (count == 0) {
    ITC_NATIVE_RUNNING("sub_820938C0 MessageVector::InsertN");
    return true;
  }
  const uint32_t last = Load(base, vector + kVectorLast);
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
    const uint32_t old_last = Load(base, vector + kVectorLast);
    if (where != old_last) {
      uint32_t moved = to + 4 * count;
      for (uint32_t from = where; from != old_last; from += 4, moved += 4) {
        if (moved != 0) {
          Store(base, moved, Load(base, from));
        }
      }
    }
    const uint32_t old_first = Load(base, vector + kVectorFirst);
    const uint32_t new_size =
        (old_first == 0 ? 0 : Words(old_first, Load(base, vector + kVectorLast))) + count;
    if (old_first != 0) {
      Delete(old_first, "MessageVector::InsertN");
    }
    Store(base, vector + kVectorFirst, block);
    Store(base, vector + kVectorEnd, block + 4 * new_capacity);
    Store(base, vector + kVectorLast, block + 4 * new_size);
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
    uint32_t fill = Load(base, vector + kVectorLast);
    for (uint32_t left = count - Words(where, fill); left != 0; left--, fill += 4) {
      if (fill != 0) {
        Store(base, fill, value);
      }
    }
    const uint32_t new_last = Load(base, vector + kVectorLast) + 4 * count;
    Store(base, vector + kVectorLast, new_last);
    for (uint32_t at = where; at != new_last - 4 * count; at += 4) {
      Store(base, at, value);
    }
    return true;
  }
  // The last `count` words copied past the end, the rest of the tail moved
  // back by `count` (copy_backward), then the gap filled.
  const uint32_t bytes = 4 * count;
  const uint32_t split = last - bytes;
  uint32_t to = Load(base, vector + kVectorLast);
  for (uint32_t from = split; from != last; from += 4, to += 4) {
    if (to != 0) {
      Store(base, to, Load(base, from));
    }
  }
  Store(base, vector + kVectorLast, to);
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
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t last = Load(base, vector + kVectorLast);
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
  if (!MessagesInsertN(base, vector, where, 1, Load(base, value))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82094110 MessageVector::Insert");
  return Load(base, vector + kVectorFirst) + uint32_t(offset) * 4;
}

std::optional<bool> MessagesPushBack(uint8_t* base, uint32_t vector, uint32_t value) {
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t last = Load(base, vector + kVectorLast);
  if (first != 0 && Words(first, last) < Words(first, Load(base, vector + kVectorEnd))) {
    ITC_NATIVE_RUNNING("sub_820944E0 MessageVector::PushBack");
    if (last != 0) {
      Store(base, last, Load(base, value));
    }
    Store(base, vector + kVectorLast, last + 4);
    return false;
  }
  if (first > last || !MessagesInsert(base, vector, vector, last, value)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820944E0 MessageVector::PushBack");
  return true;
}

uint32_t ListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820ACF88 List::Clear");
  uint32_t head = Load(base, list + kListHead);
  uint32_t node = Load(base, head + kNodeNext);
  Store(base, head + kNodeNext, head);
  head = Load(base, list + kListHead);
  Store(base, head + kNodePrevious, head);
  head = Load(base, list + kListHead);
  Store(base, list + kListSize, 0);
  while (node != head) {
    const uint32_t next = Load(base, node + kNodeNext);
    Delete(node, "List::Clear");
    head = Load(base, list + kListHead);
    node = next;
  }
  return node;
}

std::optional<uint32_t> DelegateClone(uint8_t* base, uint32_t delegate) {
  const auto block = memory::crt::OperatorNew(base, kDelegateBytes);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ACA48 Delegate::Clone");
  if (*block == 0) {
    return 0;
  }
  Store(base, *block + 0, kDelegateVtable);
  Store(base, *block + 4, Load(base, delegate + 4));
  Store(base, *block + 8, Load(base, delegate + 8));
  return *block;
}

uint32_t LayerIsEmpty(uint8_t* base, uint32_t state) {
  ITC_NATIVE_RUNNING("sub_820ACE50 StateLayer::IsEmpty");
  return Load(base, state + 28) == 0 && Load(base, state + 16) == 0 ? 1 : 0;
}

}  // namespace app_states
