// The run loop's containers natively (docs/functions/framework.md).

#include "framework/run_loop.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"

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

uint32_t MapNewNode(uint8_t* base, uint32_t left, uint32_t parent, uint32_t right, uint32_t value,
                    uint8_t color) {
  ITC_NATIVE_RUNNING("sub_82099E20 HandlerMap::NewNode");
  const uint32_t node = NewBlock(base);
  if (node != 0) {
    Store(base, node + kNodeLeft, left);
    Store(base, node + kNodeParent, parent);
    Store(base, node + kNodeRight, right);
    Store(base, node + kNodeKey, Load(base, value));
    SetColor(base, node, color);
    guest::StoreU8(base, node + kNodeIsNil, 0);
    Store(base, node + kNodeDelegate, Load(base, value + 4));
  }
  return node;
}

std::optional<Iterator> MapInsert(uint8_t* base, uint32_t map, bool add_left, uint32_t where,
                                  uint32_t value) {
  if (Load(base, map + kMapSize) >= kMapMaxSize || !CanAllocate(base)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82093400 HandlerMap::InsertNode");
  const uint32_t head = Head(base, map);
  const uint32_t node = MapNewNode(base, head, where, head, value, 0);
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
  if (Load(base, map + kMapSize) >= kMapMaxSize || !CanAllocate(base)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82093740 HandlerMap::Insert");
  const uint32_t key = Load(base, value);
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
      return InsertResult{*MapInsert(base, map, true, where, value), true};
    }
    if (!MapDecrement(base, it)) {
      return std::nullopt;
    }
  }
  if (Load(base, it.node + kNodeKey) < key) {
    return InsertResult{*MapInsert(base, map, add_left, where, value), true};
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

}  // namespace framework
