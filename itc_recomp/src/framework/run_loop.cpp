// The run loop's containers natively (docs/functions/framework.md).

#include "framework/run_loop.h"

#include "guest.h"
#include "schema/framework.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "std_list/ptr_list.h"

#include <cstdlib>

namespace framework {

namespace {

using Map = schema::HandlerMap;
using Node = schema::HandlerMapNode;
using Vector = schema::RequestVector;
using Request = schema::Request;

uint32_t Left(uint32_t node) { return Node(node).left().address; }
uint32_t Parent(uint32_t node) { return Node(node).parent().address; }
uint32_t Right(uint32_t node) { return Node(node).right().address; }
bool IsNil(uint32_t node) { return Node(node).is_nil() != 0; }
uint8_t Color(uint32_t node) { return Node(node).color(); }
void SetColor(uint32_t node, uint8_t color) { Node(node).set_color(color); }
uint32_t Head(uint32_t map) { return Map(map).head().address; }
uint32_t First(uint32_t vector) { return Vector(vector).first().address; }
uint32_t Last(uint32_t vector) { return Vector(vector).last().address; }
uint32_t End(uint32_t vector) { return Vector(vector).end().address; }
// A request's three words copied, word by word.
void CopyRequest(uint32_t to, uint32_t from) {
  const Request source(from), target(to);
  target.set_type(source.type());
  target.set_key(source.key());
  target.set_delegate(source.delegate());
}

// A node allocated after CanAllocate said it could be.
uint32_t NewBlock() {
  const auto node = MapAllocateNodes(1);
  if (!node) {
    ITC_LOG_ERROR("HandlerMap: operator new refused a node after its checks");
    std::abort();
  }
  return *node;
}

}  // namespace

bool CanAllocate() { return memory::crt::GetProcessHeap() != 0; }

bool MapIncrement(Iterator& it) {
  ITC_NATIVE_RUNNING("sub_82099B18 HandlerMap::Iterator::operator++");
  if (it.container == 0 || IsNil(it.node)) {
    return false;
  }
  uint32_t node = Right(it.node);
  if (!IsNil(node)) {
    while (!IsNil(Left(node))) {
      node = Left(node);
    }
    it.node = node;
    return true;
  }
  uint32_t current = it.node;
  node = Parent(current);
  while (!IsNil(node) && current == Right(node)) {
    current = node;
    node = Parent(node);
  }
  it.node = node;
  return true;
}

bool MapDecrement(Iterator& it) {
  ITC_NATIVE_RUNNING("sub_82091AE8 HandlerMap::Iterator::operator--");
  if (it.container == 0) {
    return false;
  }
  if (IsNil(it.node)) {
    // The end: the rightmost node, unless the map is empty.
    const uint32_t last = Right(it.node);
    if (IsNil(last)) {
      return false;
    }
    it.node = last;
    return true;
  }
  uint32_t node = Left(it.node);
  if (!IsNil(node)) {
    while (!IsNil(Right(node))) {
      node = Right(node);
    }
    it.node = node;
    return true;
  }
  uint32_t current = it.node;
  node = Parent(current);
  while (!IsNil(node) && current == Left(node)) {
    current = node;
    node = Parent(node);
  }
  if (IsNil(current)) {
    return false;
  }
  it.node = node;
  return true;
}

void MapRotateLeft(uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82092B10 HandlerMap::RotateLeft");
  const uint32_t up = Right(node);
  Node(node).set_right(Left(up));
  const uint32_t moved = Left(up);
  if (!IsNil(moved)) {
    Node(moved).set_parent(node);
  }
  Node(up).set_parent(Parent(node));
  const uint32_t head = Head(map);
  if (node == Parent(head)) {
    Node(head).set_parent(up);
  } else {
    const uint32_t parent = Parent(node);
    if (node == Left(parent)) {
      Node(parent).set_left(up);
    } else {
      Node(parent).set_right(up);
    }
  }
  Node(up).set_left(node);
  Node(node).set_parent(up);
}

void MapRotateRight(uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82092A00 HandlerMap::RotateRight");
  const uint32_t up = Left(node);
  Node(node).set_left(Right(up));
  const uint32_t moved = Right(up);
  if (!IsNil(moved)) {
    Node(moved).set_parent(node);
  }
  Node(up).set_parent(Parent(node));
  const uint32_t head = Head(map);
  if (node == Parent(head)) {
    Node(head).set_parent(up);
  } else {
    const uint32_t parent = Parent(node);
    if (node == Right(parent)) {
      Node(parent).set_right(up);
    } else {
      Node(parent).set_left(up);
    }
  }
  Node(up).set_right(node);
  Node(node).set_parent(up);
}

std::optional<uint32_t> MapAllocateNodes(uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82099C80 HandlerMap::AllocateNodes");
  if (count != 0 && 0xFFFFFFFFu / count < Node::kSize) {
    return std::nullopt;
  }
  if (count == 1) {
    // A node in host memory (E5 step 4): the map's only use (NewHead and
    // NewNode ask for one).
    return std::optional<uint32_t>(Node::New().address);
  }
  return memory::crt::OperatorNew(count * Node::kSize);
}

uint32_t MapNewHead() {
  ITC_NATIVE_RUNNING("sub_82099E88 HandlerMap::NewHead");
  const uint32_t node = NewBlock();
  // The original's null checks on the node, +4 and +8.
  if (node != 0) {
    Node(node).set_left(0);
  }
  if (node + 4 != 0) {
    Node(node).set_parent(0);
  }
  if (node + 8 != 0) {
    Node(node).set_right(0);
  }
  SetColor(node, 1);
  Node(node).set_is_nil(0);
  return node;
}

uint32_t MapConstruct(uint32_t map) {
  ITC_NATIVE_RUNNING("sub_82099FB0 HandlerMap::HandlerMap");
  const uint32_t head = MapNewHead();
  Map(map).set_head(head);
  Node(head).set_is_nil(1);
  Node(head).set_parent(head);
  Node(head).set_left(head);
  Node(head).set_right(head);
  Map(map).set_size(0);
  return head;
}

uint32_t MapNewNodeOf(uint32_t left, uint32_t parent, uint32_t right, uint32_t key,
                      uint32_t delegate, uint8_t color) {
  ITC_NATIVE_RUNNING("sub_82099E20 HandlerMap::NewNode");
  const uint32_t node = NewBlock();
  if (node != 0) {
    Node(node).set_left(left);
    Node(node).set_parent(parent);
    Node(node).set_right(right);
    Node(node).set_key(key);
    SetColor(node, color);
    Node(node).set_is_nil(0);
    Node(node).set_delegate(delegate);
  }
  return node;
}

uint32_t MapNewNode(uint32_t left, uint32_t parent, uint32_t right, uint32_t value,
                    uint8_t color) {
  const schema::HandlerMapValue v(value);
  return MapNewNodeOf(left, parent, right, v.key(), v.delegate().address, color);
}

std::optional<Iterator> MapInsert(uint32_t map, bool add_left, uint32_t where,
                                  uint32_t value) {
  const schema::HandlerMapValue v(value);
  return MapInsertOf(map, add_left, where, v.key(), v.delegate().address);
}

std::optional<Iterator> MapInsertOf(uint32_t map, bool add_left, uint32_t where,
                                    uint32_t key, uint32_t delegate) {
  if (Map(map).size() >= kMapMaxSize || !CanAllocate()) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82093400 HandlerMap::InsertNode");
  const uint32_t head = Head(map);
  const uint32_t node = MapNewNodeOf(head, where, head, key, delegate, 0);
  Map(map).set_size(Map(map).size() + 1);
  if (where == head) {
    Node(head).set_parent(node);
    Node(head).set_left(node);
    Node(head).set_right(node);
  } else if (add_left) {
    Node(where).set_left(node);
    if (where == Left(head)) {
      Node(head).set_left(node);
    }
  } else {
    Node(where).set_right(node);
    if (where == Right(head)) {
      Node(head).set_right(node);
    }
  }
  uint32_t x = node;
  while (Color(Parent(x)) == 0) {
    const uint32_t parent = Parent(x);
    const uint32_t grandparent = Parent(parent);
    if (parent == Left(grandparent)) {
      const uint32_t uncle = Right(grandparent);
      if (Color(uncle) == 0) {
        SetColor(parent, 1);
        SetColor(uncle, 1);
        SetColor(Parent(parent), 0);
        x = Parent(parent);
        continue;
      }
      if (x == Right(parent)) {
        x = parent;
        MapRotateLeft(map, x);
      }
      SetColor(Parent(x), 1);
      SetColor(Parent(Parent(x)), 0);
      MapRotateRight(map, Parent(Parent(x)));
    } else {
      const uint32_t uncle = Left(grandparent);
      if (Color(uncle) == 0) {
        SetColor(parent, 1);
        SetColor(uncle, 1);
        SetColor(Parent(parent), 0);
        x = Parent(parent);
        continue;
      }
      if (x == Left(parent)) {
        x = parent;
        MapRotateRight(map, x);
      }
      SetColor(Parent(x), 1);
      SetColor(Parent(Parent(x)), 0);
      MapRotateLeft(map, Parent(Parent(x)));
    }
  }
  SetColor(Parent(Head(map)), 1);
  return Iterator{map, node};
}

std::optional<InsertResult> MapInsertUnique(uint32_t map, uint32_t value) {
  const schema::HandlerMapValue v(value);
  return MapInsertUniqueOf(map, v.key(), v.delegate().address);
}

std::optional<InsertResult> MapInsertUniqueOf(uint32_t map, uint32_t key,
                                              uint32_t delegate) {
  if (Map(map).size() >= kMapMaxSize || !CanAllocate()) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82093740 HandlerMap::Insert");
  const uint32_t head = Head(map);
  uint32_t where = head;
  bool add_left = true;
  for (uint32_t x = Parent(head); !IsNil(x);) {
    where = x;
    add_left = key < Node(x).key();
    x = add_left ? Left(x) : Right(x);
  }
  Iterator it{map, where};
  if (add_left) {
    if (Left(head) == where) {
      return InsertResult{*MapInsertOf(map, true, where, key, delegate), true};
    }
    if (!MapDecrement(it)) {
      return std::nullopt;
    }
  }
  if (Node(it.node).key() < key) {
    return InsertResult{*MapInsertOf(map, add_left, where, key, delegate), true};
  }
  return InsertResult{it, false};
}

bool RequestsErase(uint32_t vector, Iterator first, Iterator last) {
  if (first.container == 0 || first.container != last.container) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82092EF8 RequestVector::Erase");
  if (last.node != first.node) {
    const uint32_t end = Last(vector);
    const int32_t moved = int32_t(end - last.node) / int32_t(kRequestBytes);
    const uint32_t new_last = first.node + uint32_t(moved) * kRequestBytes;
    const uint32_t delta = first.node - last.node;
    for (uint32_t from = last.node; from != end; from += kRequestBytes) {
      CopyRequest(from + delta, from);
    }
    Vector(vector).set_last(new_last);
  }
  return true;
}

bool RequestsClear(uint32_t vector) {
  const uint32_t first = First(vector);
  const uint32_t last = Last(vector);
  if (first > last) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8209A080 RequestVector::Clear");
  return RequestsErase(vector, {vector, first}, {vector, last});
}

namespace {

int32_t Count(uint32_t from, uint32_t to) { return int32_t(to - from) / int32_t(kRequestBytes); }

void StoreRequest(uint32_t to, const uint32_t (&value)[3]) {
  const Request request(to);
  request.set_type(value[0]);
  request.set_key(value[1]);
  request.set_delegate(value[2]);
}

}  // namespace

bool RequestsInsertN(uint32_t vector, uint32_t where, uint32_t count,
                     uint32_t value) {
  const Request source(value);
  const uint32_t request[3] = {source.type(), source.key(), source.delegate().address};
  const uint32_t first = First(vector);
  const uint32_t capacity = first == 0 ? 0 : uint32_t(Count(first, End(vector)));
  if (count == 0) {
    ITC_NATIVE_RUNNING("sub_8209A0E8 RequestVector::InsertN");
    return true;
  }
  const uint32_t last = Last(vector);
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
    if (!std_list::CanAllocate(new_capacity)) {
      return false;
    }
    ITC_NATIVE_RUNNING("sub_8209A0E8 RequestVector::InsertN");
    const auto allocated = std_list::AllocateNodes(new_capacity);
    if (!allocated) {
      ITC_LOG_ERROR("RequestVector: operator new refused a block after its checks");
      std::abort();
    }
    const uint32_t block = *allocated;
    uint32_t to = block;
    for (uint32_t from = first; from != where; from += kRequestBytes, to += kRequestBytes) {
      if (to != 0) {
        CopyRequest(to, from);
      }
    }
    uint32_t fill = to;
    for (uint32_t i = 0; i < count; i++, fill += kRequestBytes) {
      if (fill != 0) {
        StoreRequest(fill, request);
      }
    }
    if (where != last) {
      uint32_t moved = to + count * kRequestBytes;
      for (uint32_t from = where; from != last; from += kRequestBytes, moved += kRequestBytes) {
        if (moved != 0) {
          CopyRequest(moved, from);
        }
      }
    }
    const uint32_t old_first = First(vector);
    const uint32_t new_size =
        (old_first == 0 ? 0 : uint32_t(Count(old_first, Last(vector)))) + count;
    if (old_first != 0) {
      memory::crt::OperatorDelete(old_first);
    }
    Vector(vector).set_first(block);
    Vector(vector).set_end(block + new_capacity * kRequestBytes);
    Vector(vector).set_last(block + new_size * kRequestBytes);
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
      CopyRequest(to, from);
    }
  }
  uint32_t fill = Last(vector);
  for (int32_t left = int32_t(count) - Count(where, fill); left != 0;
       left--, fill += kRequestBytes) {
    if (fill != 0) {
      StoreRequest(fill, request);
    }
  }
  const uint32_t new_last = Last(vector) + count * kRequestBytes;
  const uint32_t old_last = new_last - count * kRequestBytes;
  Vector(vector).set_last(new_last);
  for (uint32_t at = where; at != old_last; at += kRequestBytes) {
    StoreRequest(at, request);
  }
  return true;
}

std::optional<Iterator> RequestsInsert(uint32_t vector, Iterator where,
                                       uint32_t value) {
  const uint32_t first = First(vector);
  const uint32_t last = Last(vector);
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
  if (!RequestsInsertN(vector, where.node, 1, value)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8209A600 RequestVector::Insert");
  return Iterator{vector, First(vector) + uint32_t(offset) * kRequestBytes};
}

std::optional<bool> RequestsPushBack(uint32_t vector, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8209A930 RequestVector::PushBack");
  const uint32_t first = First(vector);
  const uint32_t last = Last(vector);
  if (first != 0 &&
      uint32_t(Count(first, last)) < uint32_t(Count(first, End(vector)))) {
    if (last != 0) {
      CopyRequest(last, value);
    }
    Vector(vector).set_last(last + kRequestBytes);
    return false;
  }
  if (first > last || !RequestsInsert(vector, {vector, last}, value)) {
    return std::nullopt;
  }
  return true;
}

uint32_t RunLoopConstruct(uint32_t loop) {
  ITC_NATIVE_RUNNING("sub_8209A9F0 RunLoop::RunLoop");
  const schema::RunLoop run_loop(loop);
  run_loop.set_vtable(schema::RunLoop::kVtable);
  MapConstruct(run_loop.tick_handlers_address());
  MapConstruct(run_loop.listeners_address());
  const Vector requests(run_loop.requests_address());
  requests.set_first(0u);
  requests.set_last(0u);
  requests.set_end(0u);
  run_loop.set_unknown_48(0);
  run_loop.set_unknown_49(0);
  run_loop.set_unknown_50(0);
  return loop;
}

std::optional<bool> RunLoopApplyRequest(uint32_t loop, uint32_t type, uint32_t key,
                                        uint32_t delegate) {
  if (type == 1 || type == 3) {
    return std::nullopt;  // a handler's removal: never seen in play
  }
  if (type != 0 && type != 2) {
    ITC_NATIVE_RUNNING("sub_8209A458 RunLoop::ApplyRequest");
    return false;
  }
  const schema::RunLoop run_loop(loop);
  const uint32_t map = type == 0 ? run_loop.tick_handlers_address() : run_loop.listeners_address();
  if (!MapInsertUniqueOf(map, key, delegate)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8209A458 RunLoop::ApplyRequest");
  return true;
}

}  // namespace framework
