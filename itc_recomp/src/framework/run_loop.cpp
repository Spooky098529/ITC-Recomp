// The run loop's containers natively (docs/functions/framework.md).

#include "framework/run_loop.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "std_list/ptr_list.h"

#include <cstdlib>

namespace framework {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Left(uint8_t* base, uint32_t node) { return Load(base, node + kNodeLeft); }
uint32_t Parent(uint8_t* base, uint32_t node) { return Load(base, node + kNodeParent); }
uint32_t Right(uint8_t* base, uint32_t node) { return Load(base, node + kNodeRight); }
bool IsNil(uint8_t* base, uint32_t node) { return guest::LoadU8(base, node + kNodeIsNil) != 0; }
uint8_t Color(uint8_t* base, uint32_t node) { return guest::LoadU8(base, node + kNodeColor); }
void SetColor(uint8_t* base, uint32_t node, uint8_t color) {
  guest::StoreU8(base, node + kNodeColor, color);
}
uint32_t Head(uint8_t* base, uint32_t map) { return Load(base, map + kMapHead); }

// A node allocated after CanAllocate said it could be.
uint32_t NewBlock(uint8_t* base) {
  const auto node = MapAllocateNodes(base, 1);
  if (!node) {
    ITC_LOG_ERROR("HandlerMap: operator new refused a node after its checks");
    std::abort();
  }
  return *node;
}

}  // namespace

bool CanAllocate(uint8_t* base) { return memory::crt::GetProcessHeap(base) != 0; }

bool MapIncrement(uint8_t* base, Iterator& it) {
  ITC_NATIVE_RUNNING("sub_82099B18 HandlerMap::Iterator::operator++");
  if (it.container == 0 || IsNil(base, it.node)) {
    return false;
  }
  uint32_t node = Right(base, it.node);
  if (!IsNil(base, node)) {
    while (!IsNil(base, Left(base, node))) {
      node = Left(base, node);
    }
    it.node = node;
    return true;
  }
  uint32_t current = it.node;
  node = Parent(base, current);
  while (!IsNil(base, node) && current == Right(base, node)) {
    current = node;
    node = Parent(base, node);
  }
  it.node = node;
  return true;
}

bool MapDecrement(uint8_t* base, Iterator& it) {
  ITC_NATIVE_RUNNING("sub_82091AE8 HandlerMap::Iterator::operator--");
  if (it.container == 0) {
    return false;
  }
  if (IsNil(base, it.node)) {
    // The end: the rightmost node, unless the map is empty.
    const uint32_t last = Right(base, it.node);
    if (IsNil(base, last)) {
      return false;
    }
    it.node = last;
    return true;
  }
  uint32_t node = Left(base, it.node);
  if (!IsNil(base, node)) {
    while (!IsNil(base, Right(base, node))) {
      node = Right(base, node);
    }
    it.node = node;
    return true;
  }
  uint32_t current = it.node;
  node = Parent(base, current);
  while (!IsNil(base, node) && current == Left(base, node)) {
    current = node;
    node = Parent(base, node);
  }
  if (IsNil(base, current)) {
    return false;
  }
  it.node = node;
  return true;
}

void MapRotateLeft(uint8_t* base, uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82092B10 HandlerMap::RotateLeft");
  const uint32_t up = Right(base, node);
  Store(base, node + kNodeRight, Left(base, up));
  const uint32_t moved = Left(base, up);
  if (!IsNil(base, moved)) {
    Store(base, moved + kNodeParent, node);
  }
  Store(base, up + kNodeParent, Parent(base, node));
  const uint32_t head = Head(base, map);
  if (node == Parent(base, head)) {
    Store(base, head + kNodeParent, up);
  } else {
    const uint32_t parent = Parent(base, node);
    if (node == Left(base, parent)) {
      Store(base, parent + kNodeLeft, up);
    } else {
      Store(base, parent + kNodeRight, up);
    }
  }
  Store(base, up + kNodeLeft, node);
  Store(base, node + kNodeParent, up);
}

void MapRotateRight(uint8_t* base, uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82092A00 HandlerMap::RotateRight");
  const uint32_t up = Left(base, node);
  Store(base, node + kNodeLeft, Right(base, up));
  const uint32_t moved = Right(base, up);
  if (!IsNil(base, moved)) {
    Store(base, moved + kNodeParent, node);
  }
  Store(base, up + kNodeParent, Parent(base, node));
  const uint32_t head = Head(base, map);
  if (node == Parent(base, head)) {
    Store(base, head + kNodeParent, up);
  } else {
    const uint32_t parent = Parent(base, node);
    if (node == Right(base, parent)) {
      Store(base, parent + kNodeRight, up);
    } else {
      Store(base, parent + kNodeLeft, up);
    }
  }
  Store(base, up + kNodeRight, node);
  Store(base, node + kNodeParent, up);
}

std::optional<uint32_t> MapAllocateNodes(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82099C80 HandlerMap::AllocateNodes");
  if (count != 0 && 0xFFFFFFFFu / count < kNodeBytes) {
    return std::nullopt;
  }
  return memory::crt::OperatorNew(base, count * kNodeBytes);
}

uint32_t MapNewHead(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82099E88 HandlerMap::NewHead");
  const uint32_t node = NewBlock(base);
  // The original's null checks on the node, +4 and +8.
  if (node != 0) {
    Store(base, node + kNodeLeft, 0);
  }
  if (node + 4 != 0) {
    Store(base, node + kNodeParent, 0);
  }
  if (node + 8 != 0) {
    Store(base, node + kNodeRight, 0);
  }
  SetColor(base, node, 1);
  guest::StoreU8(base, node + kNodeIsNil, 0);
  return node;
}

uint32_t MapConstruct(uint8_t* base, uint32_t map) {
  ITC_NATIVE_RUNNING("sub_82099FB0 HandlerMap::HandlerMap");
  const uint32_t head = MapNewHead(base);
  Store(base, map + kMapHead, head);
  guest::StoreU8(base, head + kNodeIsNil, 1);
  Store(base, head + kNodeParent, head);
  Store(base, head + kNodeLeft, head);
  Store(base, head + kNodeRight, head);
  Store(base, map + kMapSize, 0);
  return head;
}

uint32_t MapNewNodeOf(uint8_t* base, uint32_t left, uint32_t parent, uint32_t right, uint32_t key,
                      uint32_t delegate, uint8_t color) {
  ITC_NATIVE_RUNNING("sub_82099E20 HandlerMap::NewNode");
  const uint32_t node = NewBlock(base);
  if (node != 0) {
    Store(base, node + kNodeLeft, left);
    Store(base, node + kNodeParent, parent);
    Store(base, node + kNodeRight, right);
    Store(base, node + kNodeKey, key);
    SetColor(base, node, color);
    guest::StoreU8(base, node + kNodeIsNil, 0);
    Store(base, node + kNodeDelegate, delegate);
  }
  return node;
}

uint32_t MapNewNode(uint8_t* base, uint32_t left, uint32_t parent, uint32_t right, uint32_t value,
                    uint8_t color) {
  return MapNewNodeOf(base, left, parent, right, Load(base, value), Load(base, value + 4), color);
}

std::optional<Iterator> MapInsert(uint8_t* base, uint32_t map, bool add_left, uint32_t where,
                                  uint32_t value) {
  return MapInsertOf(base, map, add_left, where, Load(base, value), Load(base, value + 4));
}

std::optional<Iterator> MapInsertOf(uint8_t* base, uint32_t map, bool add_left, uint32_t where,
                                    uint32_t key, uint32_t delegate) {
  if (Load(base, map + kMapSize) >= kMapMaxSize || !CanAllocate(base)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82093400 HandlerMap::InsertNode");
  const uint32_t head = Head(base, map);
  const uint32_t node = MapNewNodeOf(base, head, where, head, key, delegate, 0);
  Store(base, map + kMapSize, Load(base, map + kMapSize) + 1);
  if (where == head) {
    Store(base, head + kNodeParent, node);
    Store(base, head + kNodeLeft, node);
    Store(base, head + kNodeRight, node);
  } else if (add_left) {
    Store(base, where + kNodeLeft, node);
    if (where == Left(base, head)) {
      Store(base, head + kNodeLeft, node);
    }
  } else {
    Store(base, where + kNodeRight, node);
    if (where == Right(base, head)) {
      Store(base, head + kNodeRight, node);
    }
  }
  uint32_t x = node;
  while (Color(base, Parent(base, x)) == 0) {
    const uint32_t parent = Parent(base, x);
    const uint32_t grandparent = Parent(base, parent);
    if (parent == Left(base, grandparent)) {
      const uint32_t uncle = Right(base, grandparent);
      if (Color(base, uncle) == 0) {
        SetColor(base, parent, 1);
        SetColor(base, uncle, 1);
        SetColor(base, Parent(base, parent), 0);
        x = Parent(base, parent);
        continue;
      }
      if (x == Right(base, parent)) {
        x = parent;
        MapRotateLeft(base, map, x);
      }
      SetColor(base, Parent(base, x), 1);
      SetColor(base, Parent(base, Parent(base, x)), 0);
      MapRotateRight(base, map, Parent(base, Parent(base, x)));
    } else {
      const uint32_t uncle = Left(base, grandparent);
      if (Color(base, uncle) == 0) {
        SetColor(base, parent, 1);
        SetColor(base, uncle, 1);
        SetColor(base, Parent(base, parent), 0);
        x = Parent(base, parent);
        continue;
      }
      if (x == Left(base, parent)) {
        x = parent;
        MapRotateRight(base, map, x);
      }
      SetColor(base, Parent(base, x), 1);
      SetColor(base, Parent(base, Parent(base, x)), 0);
      MapRotateLeft(base, map, Parent(base, Parent(base, x)));
    }
  }
  SetColor(base, Parent(base, Head(base, map)), 1);
  return Iterator{map, node};
}

std::optional<InsertResult> MapInsertUnique(uint8_t* base, uint32_t map, uint32_t value) {
  return MapInsertUniqueOf(base, map, Load(base, value), Load(base, value + 4));
}

std::optional<InsertResult> MapInsertUniqueOf(uint8_t* base, uint32_t map, uint32_t key,
                                              uint32_t delegate) {
  if (Load(base, map + kMapSize) >= kMapMaxSize || !CanAllocate(base)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82093740 HandlerMap::Insert");
  const uint32_t head = Head(base, map);
  uint32_t where = head;
  bool add_left = true;
  for (uint32_t x = Parent(base, head); !IsNil(base, x);) {
    where = x;
    add_left = key < Load(base, x + kNodeKey);
    x = add_left ? Left(base, x) : Right(base, x);
  }
  Iterator it{map, where};
  if (add_left) {
    if (Left(base, head) == where) {
      return InsertResult{*MapInsertOf(base, map, true, where, key, delegate), true};
    }
    if (!MapDecrement(base, it)) {
      return std::nullopt;
    }
  }
  if (Load(base, it.node + kNodeKey) < key) {
    return InsertResult{*MapInsertOf(base, map, add_left, where, key, delegate), true};
  }
  return InsertResult{it, false};
}

bool RequestsErase(uint8_t* base, uint32_t vector, Iterator first, Iterator last) {
  if (first.container == 0 || first.container != last.container) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82092EF8 RequestVector::Erase");
  if (last.node != first.node) {
    const uint32_t end = Load(base, vector + kVectorLast);
    const int32_t moved = int32_t(end - last.node) / int32_t(kRequestBytes);
    const uint32_t new_last = first.node + uint32_t(moved) * kRequestBytes;
    const uint32_t delta = first.node - last.node;
    for (uint32_t from = last.node; from != end; from += kRequestBytes) {
      for (uint32_t word = 0; word < kRequestBytes; word += 4) {
        Store(base, from + delta + word, Load(base, from + word));
      }
    }
    Store(base, vector + kVectorLast, new_last);
  }
  return true;
}

bool RequestsClear(uint8_t* base, uint32_t vector) {
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t last = Load(base, vector + kVectorLast);
  if (first > last) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8209A080 RequestVector::Clear");
  return RequestsErase(base, vector, {vector, first}, {vector, last});
}

namespace {

int32_t Count(uint32_t from, uint32_t to) { return int32_t(to - from) / int32_t(kRequestBytes); }

void CopyRequest(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t word = 0; word < kRequestBytes; word += 4) {
    Store(base, to + word, Load(base, from + word));
  }
}

void StoreRequest(uint8_t* base, uint32_t to, const uint32_t (&value)[3]) {
  Store(base, to + 0, value[0]);
  Store(base, to + 4, value[1]);
  Store(base, to + 8, value[2]);
}

}  // namespace

bool RequestsInsertN(uint8_t* base, uint32_t vector, uint32_t where, uint32_t count,
                     uint32_t value) {
  const uint32_t request[3] = {Load(base, value), Load(base, value + 4), Load(base, value + 8)};
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t capacity = first == 0 ? 0 : uint32_t(Count(first, Load(base, vector + kVectorEnd)));
  if (count == 0) {
    ITC_NATIVE_RUNNING("sub_8209A0E8 RequestVector::InsertN");
    return true;
  }
  const uint32_t last = Load(base, vector + kVectorLast);
  const uint32_t size = first == 0 ? 0 : uint32_t(Count(first, last));
  if (kRequestsMaxSize - size < count) {
    return false;  // the length_error
  }
  if (capacity < size + count) {
    uint32_t new_capacity =
        kRequestsMaxSize - (capacity >> 1) < capacity ? 0 : capacity + (capacity >> 1);
    if (new_capacity < size + count) {
      new_capacity = size + count;
    }
    if (!std_list::CanAllocate(base, new_capacity)) {
      return false;
    }
    ITC_NATIVE_RUNNING("sub_8209A0E8 RequestVector::InsertN");
    const auto allocated = std_list::AllocateNodes(base, new_capacity);
    if (!allocated) {
      ITC_LOG_ERROR("RequestVector: operator new refused a block after its checks");
      std::abort();
    }
    const uint32_t block = *allocated;
    uint32_t to = block;
    for (uint32_t from = first; from != where; from += kRequestBytes, to += kRequestBytes) {
      if (to != 0) {
        CopyRequest(base, to, from);
      }
    }
    uint32_t fill = to;
    for (uint32_t i = 0; i < count; i++, fill += kRequestBytes) {
      if (fill != 0) {
        StoreRequest(base, fill, request);
      }
    }
    if (where != last) {
      uint32_t moved = to + count * kRequestBytes;
      for (uint32_t from = where; from != last; from += kRequestBytes, moved += kRequestBytes) {
        if (moved != 0) {
          CopyRequest(base, moved, from);
        }
      }
    }
    const uint32_t old_first = Load(base, vector + kVectorFirst);
    const uint32_t new_size =
        (old_first == 0 ? 0 : uint32_t(Count(old_first, Load(base, vector + kVectorLast)))) + count;
    if (old_first != 0) {
      memory::crt::OperatorDelete(old_first);
    }
    Store(base, vector + kVectorFirst, block);
    Store(base, vector + kVectorEnd, block + new_capacity * kRequestBytes);
    Store(base, vector + kVectorLast, block + new_size * kRequestBytes);
    return true;
  }
  if (uint32_t(Count(where, last)) >= count) {
    return false;  // the tail at least `count` long: copy_backward, never seen in play
  }
  ITC_NATIVE_RUNNING("sub_8209A0E8 RequestVector::InsertN");
  // The tail moved past the new elements, the rest of them after the old
  // last, then the tail's old place filled.
  uint32_t to = where + count * kRequestBytes;
  for (uint32_t from = where; from != last; from += kRequestBytes, to += kRequestBytes) {
    if (to != 0) {
      CopyRequest(base, to, from);
    }
  }
  uint32_t fill = Load(base, vector + kVectorLast);
  for (int32_t left = int32_t(count) - Count(where, fill); left != 0;
       left--, fill += kRequestBytes) {
    if (fill != 0) {
      StoreRequest(base, fill, request);
    }
  }
  const uint32_t new_last = Load(base, vector + kVectorLast) + count * kRequestBytes;
  const uint32_t old_last = new_last - count * kRequestBytes;
  Store(base, vector + kVectorLast, new_last);
  for (uint32_t at = where; at != old_last; at += kRequestBytes) {
    StoreRequest(base, at, request);
  }
  return true;
}

std::optional<Iterator> RequestsInsert(uint8_t* base, uint32_t vector, Iterator where,
                                       uint32_t value) {
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t last = Load(base, vector + kVectorLast);
  int32_t offset = 0;
  if (first != 0 && Count(first, last) != 0) {
    if (first > last || where.container == 0 || where.container != vector) {
      return std::nullopt;  // the traps
    }
    offset = Count(first, where.node);
  }
  const int32_t size = first == 0 ? 0 : Count(first, last);
  if (offset < 0 || offset > size + 1) {
    return std::nullopt;  // the result's traps, after the insert
  }
  if (!RequestsInsertN(base, vector, where.node, 1, value)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8209A600 RequestVector::Insert");
  return Iterator{vector, Load(base, vector + kVectorFirst) + uint32_t(offset) * kRequestBytes};
}

std::optional<bool> RequestsPushBack(uint8_t* base, uint32_t vector, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8209A930 RequestVector::PushBack");
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t last = Load(base, vector + kVectorLast);
  if (first != 0 &&
      uint32_t(Count(first, last)) < uint32_t(Count(first, Load(base, vector + kVectorEnd)))) {
    if (last != 0) {
      CopyRequest(base, last, value);
    }
    Store(base, vector + kVectorLast, last + kRequestBytes);
    return false;
  }
  if (first > last || !RequestsInsert(base, vector, {vector, last}, value)) {
    return std::nullopt;
  }
  return true;
}

uint32_t RunLoopConstruct(uint8_t* base, uint32_t loop) {
  ITC_NATIVE_RUNNING("sub_8209A9F0 RunLoop::RunLoop");
  Store(base, loop + 0, kRunLoopVtable);
  MapConstruct(base, loop + kTickHandlers);
  MapConstruct(base, loop + kListeners);
  Store(base, loop + kRequests + kVectorFirst, 0);
  Store(base, loop + kRequests + kVectorLast, 0);
  Store(base, loop + kRequests + kVectorEnd, 0);
  guest::StoreU8(base, loop + 48, 0);
  guest::StoreU8(base, loop + 49, 0);
  guest::StoreU8(base, loop + 50, 0);
  return loop;
}

std::optional<bool> RunLoopApplyRequest(uint8_t* base, uint32_t loop, uint32_t type, uint32_t key,
                                        uint32_t delegate) {
  if (type == 1 || type == 3) {
    return std::nullopt;  // a handler's removal: never seen in play
  }
  if (type != 0 && type != 2) {
    ITC_NATIVE_RUNNING("sub_8209A458 RunLoop::ApplyRequest");
    return false;
  }
  const uint32_t map = loop + (type == 0 ? kTickHandlers : kListeners);
  if (!MapInsertUniqueOf(base, map, key, delegate)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8209A458 RunLoop::ApplyRequest");
  return true;
}

}  // namespace framework
