// The screen objects' two red-black trees (see uv_trees.h), store for store
// as the recompiled functions. The two instantiations share their code here
// (only the node's size and its color and isnil offsets differ); each
// function the game has keeps its own plain C++ entry, which calls the
// others' entries, so each logs its own ITC_NATIVE_RUNNING.

#include "screen_objects/uv_trees.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "memory/handles.h"
#include "schema/screen_objects.h"

#include <bit>
#include <cstdlib>

namespace screen_objects {

namespace {

using ColumnTree = schema::ScreenObjectUvColumn;
using ColumnNode = schema::ScreenObjectUvColumnNode;
using MapTree = schema::ScreenObjectUvMap;
using MapNode = schema::ScreenObjectUvMapNode;

// A tree's views: the tree, its node, and Insert's size limit.
struct Column {
  using Tree = ColumnTree;
  using Node = ColumnNode;
  static constexpr uint32_t kMaxSize = kColumnMaxSize;
};
struct Map {
  using Tree = MapTree;
  using Node = MapNode;
  static constexpr uint32_t kMaxSize = kMapMaxSize;
};

// A key's double from its two words, high then low (the original's loads).
template <typename V>
double KeyOf(const V& v) {
  const uint64_t high = v.key_high();
  return std::bit_cast<double>((high << 32) | v.key_low());
}

// A node allocated inside a native caller, after its checks.
uint32_t Required(std::optional<uint32_t> block, const char* what) {
  if (!block) {
    ITC_LOG_ERROR("{}: operator new refused a node after its checks", what);
    std::abort();
  }
  return *block;
}

template <typename T>
std::optional<uint32_t> AllocateNodes(uint8_t* base, uint32_t count) {
  constexpr uint32_t node_bytes = T::Node::kSize;
  // The original's bad_alloc path: 0xFFFFFFFF / count below the node's size.
  if (count != 0 && 0xFFFFFFFFu / count < node_bytes) {
    return std::nullopt;
  }
  // In host memory (E5 step 4): `count` nodes in one block, one handle (the
  // original's operator new(count x size); every caller asks for one).
  return T::Node::NewArray(base, count).address;
}

// NewHead's stores, the original's null checks kept.
template <typename T>
void InitHead(uint8_t* base, uint32_t node) {
  const typename T::Node n(base, node);
  if (node != 0) {
    n.set_left(0u);
  }
  if (n.parent_address() != 0) {
    n.set_parent(0u);
  }
  if (n.right_address() != 0) {
    n.set_right(0u);
  }
  n.set_color(1);
  n.set_is_nil(0);
}

template <typename T>
void Construct(uint8_t* base, uint32_t tree, uint32_t head) {
  const typename T::Tree t(base, tree);
  const typename T::Node h(base, head);
  t.set_head(head);
  h.set_is_nil(1);
  h.set_parent(head);
  h.set_left(head);
  h.set_right(head);
  t.set_size(0);
}

template <typename T>
void RotateLeft(uint8_t* base, uint32_t tree, uint32_t x) {
  using N = typename T::Node;
  const N xn(base, x);
  const N y = xn.right();
  xn.set_right(y.left());
  const N y_left = y.left();
  if (!y_left.is_nil()) {
    y_left.set_parent(x);
  }
  y.set_parent(xn.parent());
  const N head = typename T::Tree(base, tree).head();
  if (x == head.parent().address) {
    head.set_parent(y);
  } else {
    const N parent = xn.parent();
    if (x == parent.left().address) {
      parent.set_left(y);
    } else {
      parent.set_right(y);
    }
  }
  y.set_left(x);
  xn.set_parent(y);
}

template <typename T>
void RotateRight(uint8_t* base, uint32_t tree, uint32_t x) {
  using N = typename T::Node;
  const N xn(base, x);
  const N y = xn.left();
  xn.set_left(y.right());
  const N y_right = y.right();
  if (!y_right.is_nil()) {
    y_right.set_parent(x);
  }
  y.set_parent(xn.parent());
  const N head = typename T::Tree(base, tree).head();
  if (x == head.parent().address) {
    head.set_parent(y);
  } else {
    const N parent = xn.parent();
    if (x == parent.right().address) {
      parent.set_right(y);
    } else {
      parent.set_left(y);
    }
  }
  y.set_right(x);
  xn.set_parent(y);
}

template <typename T>
uint32_t LowerBoundNode(uint8_t* base, uint32_t tree, double key) {
  using N = typename T::Node;
  uint32_t result = typename T::Tree(base, tree).head().address;
  N node = N(base, result).parent();
  while (!node.is_nil()) {
    if (KeyOf(node) < key) {
      node = node.right();
    } else {
      result = node.address;
      node = node.left();
    }
  }
  return result;
}

// Find's step after LowerBound: end when it's the head or `key` is below
// its key. nullopt where the original traps (an iterator of another tree).
template <typename T>
std::optional<Iterator> FindFrom(uint8_t* base, uint32_t tree, Iterator it, double key) {
  if (it.tree == 0 || it.tree != tree) {
    return std::nullopt;
  }
  const uint32_t head = typename T::Tree(base, tree).head().address;
  if (it.node == head || key < KeyOf(typename T::Node(base, it.node))) {
    return Iterator{tree, head};
  }
  return it;
}

template <typename T>
bool Decrement(uint8_t* base, Iterator& it) {
  using N = typename T::Node;
  if (it.tree == 0) {
    return false;
  }
  const N node(base, it.node);
  if (node.is_nil()) {
    // end: the rightmost node, a trap when the tree is empty.
    const N rightmost = node.right();
    if (rightmost.is_nil()) {
      return false;
    }
    it.node = rightmost.address;
    return true;
  }
  N left = node.left();
  if (!left.is_nil()) {
    while (!left.right().is_nil()) {
      left = left.right();
    }
    it.node = left.address;
    return true;
  }
  N current = node;
  N parent = node.parent();
  while (!parent.is_nil() && current.address == parent.left().address) {
    current = parent;
    parent = parent.parent();
  }
  if (current.is_nil()) {
    return false;
  }
  it.node = parent.address;
  return true;
}

// Insert's links and rebalance, once the node is made: `rotate_left` and
// `rotate_right` are the tree's own.
template <typename T, typename RotateLeftFn, typename RotateRightFn>
Iterator LinkAndBalance(uint8_t* base, uint32_t tree, bool add_left, uint32_t where,
                        uint32_t node, RotateLeftFn rotate_left, RotateRightFn rotate_right) {
  using N = typename T::Node;
  const typename T::Tree t(base, tree);
  t.set_size(t.size() + 1);
  N head = t.head();
  const N w(base, where);
  if (where == head.address) {
    head.set_parent(node);
    t.head().set_left(node);
    t.head().set_right(node);
  } else if (add_left) {
    w.set_left(node);
    head = t.head();
    if (where == head.left().address) {
      head.set_left(node);
    }
  } else {
    w.set_right(node);
    head = t.head();
    if (where == head.right().address) {
      head.set_right(node);
    }
  }
  N x(base, node);
  while (x.parent().color() == 0) {
    const N parent = x.parent();
    const N grandparent = parent.parent();
    if (parent.address == grandparent.left().address) {
      const N uncle = grandparent.right();
      if (uncle.color() == 0) {
        x.parent().set_color(1);
        uncle.set_color(1);
        x.parent().parent().set_color(0);
        x = x.parent().parent();
      } else {
        if (x.address == parent.right().address) {
          x = parent;
          rotate_left(x.address);
        }
        x.parent().set_color(1);
        x.parent().parent().set_color(0);
        rotate_right(x.parent().parent().address);
      }
    } else {
      const N uncle = grandparent.left();
      if (uncle.color() == 0) {
        x.parent().set_color(1);
        uncle.set_color(1);
        x.parent().parent().set_color(0);
        x = x.parent().parent();
      } else {
        if (x.address == parent.left().address) {
          x = parent;
          rotate_right(x.address);
        }
        x.parent().set_color(1);
        x.parent().parent().set_color(0);
        rotate_left(x.parent().parent().address);
      }
    }
  }
  t.head().parent().set_color(1);
  return Iterator{tree, node};
}

// InsertUnique's search, with no stores: where the key goes, on which side,
// and whether a node with its key is there (the node then). nullopt where
// the original traps (the iterator decrement).
struct Place {
  uint32_t where;
  bool add_left;
  bool exists;
  Iterator existing;
};
template <typename T, typename DecrementFn>
std::optional<Place> FindPlace(uint8_t* base, uint32_t tree, double key,
                               DecrementFn decrement) {
  using N = typename T::Node;
  const N head = typename T::Tree(base, tree).head();
  uint32_t where = head.address;
  bool add_left = true;
  N node = head.parent();
  while (!node.is_nil()) {
    where = node.address;
    add_left = key < KeyOf(node);
    node = add_left ? node.left() : node.right();
  }
  Iterator it{tree, where};
  if (add_left) {
    if (where == head.left().address) {
      return Place{where, true, false, {}};
    }
    if (!decrement(it)) {
      return std::nullopt;
    }
  }
  if (KeyOf(N(base, it.node)) < key) {
    return Place{where, add_left, false, {}};
  }
  return Place{where, add_left, true, it};
}

template <typename T>
void ClearHead(uint8_t* base, uint32_t tree) {
  const typename T::Tree t(base, tree);
  const typename T::Node head = t.head();
  head.set_parent(head);
  t.set_size(0);
  t.head().set_left(t.head());
  t.head().set_right(t.head());
}

// Erase's checks: whether [first, last) is the whole tree (begin, end) and
// no iterator traps.
template <typename T>
bool WholeRange(uint8_t* base, uint32_t tree, Iterator first, Iterator last) {
  const typename T::Node head = typename T::Tree(base, tree).head();
  if (first.tree == 0 || first.tree != tree) {
    return false;
  }
  if (first.node != head.left().address) {
    return false;
  }
  return last.tree != 0 && last.tree == tree && last.node == head.address;
}

// A node's block freed (the original's operator delete): a handle, as
// AllocateNodes makes them.
void Free(uint32_t block, const char* what) {
  if (!memory::FreeHandle(block)) {
    ITC_LOG_ERROR("{}: a node's handle {:08X} not freed", what, block);
    std::abort();
  }
}

}  // namespace

bool CanAllocate(uint8_t* base) { return memory::crt::GetProcessHeap(base) != 0; }

// --- UvColumn ---

std::optional<uint32_t> ColumnAllocateNodes(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8212FBD8 UvColumn::AllocateNodes");
  return AllocateNodes<Column>(base, count);
}

uint32_t ColumnNewHead(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8212FF08 UvColumn::NewHead");
  const uint32_t node = Required(ColumnAllocateNodes(base, 1), "UvColumn::NewHead");
  InitHead<Column>(base, node);
  return node;
}

void ColumnConstruct(uint8_t* base, uint32_t column) {
  ITC_NATIVE_RUNNING("sub_82130140 UvColumn::UvColumn");
  Construct<Column>(base, column, ColumnNewHead(base));
}

uint32_t ColumnNewNode(uint8_t* base, uint32_t left, uint32_t parent, uint32_t right,
                       uint32_t value, uint8_t color) {
  ITC_NATIVE_RUNNING("sub_8212FDB0 UvColumn::NewNode");
  const uint32_t node = Required(ColumnAllocateNodes(base, 1), "UvColumn::NewNode");
  const ColumnNode n(base, node);
  const schema::ScreenObjectUvColumnValue v(base, value);
  n.set_left(left);
  n.set_parent(parent);
  n.set_right(right);
  n.set_key_high(v.key_high());
  n.set_key_low(v.key_low());
  n.set_u(v.u());
  n.set_v(v.v());
  n.set_color(color);
  n.set_is_nil(0);
  return node;
}

uint32_t ColumnCopySubtree(uint8_t* base, uint32_t column, uint32_t source, uint32_t parent) {
  ITC_NATIVE_RUNNING("sub_8212FE80 UvColumn::CopySubtree");
  const ColumnNode head = ColumnTree(base, column).head();
  const ColumnNode src(base, source);
  uint32_t result = head.address;
  if (src.is_nil()) {
    return result;
  }
  const uint32_t node =
      ColumnNewNode(base, head.address, parent, head.address, src.key_address(), src.color());
  if (head.is_nil()) {
    result = node;
  }
  const ColumnNode n(base, node);
  n.set_left(ColumnCopySubtree(base, column, src.left().address, node));
  n.set_right(ColumnCopySubtree(base, column, src.right().address, node));
  return result;
}

uint32_t ColumnCopy(uint8_t* base, uint32_t column, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_82130088 UvColumn::Copy");
  const ColumnTree c(base, column);
  const ColumnTree s(base, source);
  const uint32_t root = ColumnCopySubtree(base, column, s.head().parent().address, c.head().address);
  c.head().set_parent(root);
  c.set_size(s.size());
  const ColumnNode head = c.head();
  ColumnNode node = head.parent();
  if (node.is_nil()) {
    head.set_left(head);
    c.head().set_right(c.head());
    return root;
  }
  while (!node.left().is_nil()) {
    node = node.left();
  }
  head.set_left(node);
  const ColumnNode head_again = c.head();
  node = head_again.parent();
  while (!node.right().is_nil()) {
    node = node.right();
  }
  head_again.set_right(node);
  return root;
}

void ColumnRotateLeft(uint8_t* base, uint32_t column, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212F920 UvColumn::RotateLeft");
  RotateLeft<Column>(base, column, node);
}

void ColumnRotateRight(uint8_t* base, uint32_t column, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212F988 UvColumn::RotateRight");
  RotateRight<Column>(base, column, node);
}

uint32_t ColumnLowerBoundNode(uint8_t* base, uint32_t column, double key) {
  ITC_NATIVE_RUNNING("sub_8212F8C8 UvColumn::LowerBoundNode");
  return LowerBoundNode<Column>(base, column, key);
}

std::optional<Iterator> ColumnLowerBound(uint8_t* base, uint32_t column, double key) {
  ITC_NATIVE_RUNNING("sub_8212FFC8 UvColumn::LowerBound");
  if (column == 0) {
    return std::nullopt;
  }
  return Iterator{column, ColumnLowerBoundNode(base, column, key)};
}

std::optional<Iterator> ColumnFind(uint8_t* base, uint32_t column, double key) {
  ITC_NATIVE_RUNNING("sub_821301F0 UvColumn::Find");
  const auto it = ColumnLowerBound(base, column, key);
  if (!it) {
    return std::nullopt;
  }
  return FindFrom<Column>(base, column, *it, key);
}

bool ColumnDecrement(uint8_t* base, Iterator& it) {
  ITC_NATIVE_RUNNING("sub_8212EEF8 UvColumn's iterator decrement");
  return Decrement<Column>(base, it);
}

std::optional<Iterator> ColumnInsert(uint8_t* base, uint32_t column, bool add_left, uint32_t where,
                                     uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82131680 UvColumn::Insert");
  const ColumnTree c(base, column);
  if (c.size() >= Column::kMaxSize || !CanAllocate(base)) {
    return std::nullopt;
  }
  const uint32_t head = c.head().address;
  const uint32_t node = ColumnNewNode(base, head, where, head, value, 0);
  return LinkAndBalance<Column>(
      base, column, add_left, where, node,
      [&](uint32_t x) { ColumnRotateLeft(base, column, x); },
      [&](uint32_t x) { ColumnRotateRight(base, column, x); });
}

std::optional<InsertResult> ColumnInsertUnique(uint8_t* base, uint32_t column, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82131CB8 UvColumn::InsertUnique");
  const auto place = FindPlace<Column>(base, column,
                                       KeyOf(schema::ScreenObjectUvColumnValue(base, value)),
                                       [&](Iterator& it) { return ColumnDecrement(base, it); });
  if (!place) {
    return std::nullopt;
  }
  if (place->exists) {
    return InsertResult{place->existing, false};
  }
  const auto it = ColumnInsert(base, column, place->add_left, place->where, value);
  if (!it) {
    return std::nullopt;
  }
  return InsertResult{*it, true};
}

uint32_t ColumnEraseSubtree(uint8_t* base, uint32_t column, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212FD50 UvColumn::EraseSubtree");
  uint32_t r3 = column;
  ColumnNode n(base, node);
  while (!n.is_nil()) {
    ColumnEraseSubtree(base, column, n.right().address);
    const ColumnNode next = n.left();
    Free(n.address, "UvColumn::EraseSubtree");
    r3 = n.address;
    n = next;
  }
  return r3;
}

uint32_t ColumnClear(uint8_t* base, uint32_t column) {
  ITC_NATIVE_RUNNING("sub_8212FE28 UvColumn::Clear");
  const uint32_t r3 =
      ColumnEraseSubtree(base, column, ColumnTree(base, column).head().parent().address);
  ClearHead<Column>(base, column);
  return r3;
}

std::optional<Iterator> ColumnErase(uint8_t* base, uint32_t column, Iterator first,
                                    Iterator last) {
  ITC_NATIVE_RUNNING("sub_82131DD8 UvColumn::Erase");
  if (!WholeRange<Column>(base, column, first, last)) {
    return std::nullopt;
  }
  ColumnClear(base, column);
  return Iterator{column, ColumnTree(base, column).head().left().address};
}

std::optional<uint32_t> ColumnDestroy(uint8_t* base, uint32_t column) {
  ITC_NATIVE_RUNNING("sub_82131EE0 UvColumn::~UvColumn");
  if (column == 0) {
    return std::nullopt;
  }
  const ColumnTree c(base, column);
  const ColumnNode head = c.head();
  if (!ColumnErase(base, column, {column, head.left().address}, {column, head.address})) {
    return std::nullopt;
  }
  const uint32_t block = c.head().address;
  Free(block, "UvColumn::~UvColumn");
  c.set_head(0u);
  c.set_size(0);
  return block;
}

// --- UvMap ---

std::optional<uint32_t> MapAllocateNodes(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8212FC50 UvMap::AllocateNodes");
  return AllocateNodes<Map>(base, count);
}

uint32_t MapNewHead(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8212FF68 UvMap::NewHead");
  const uint32_t node = Required(MapAllocateNodes(base, 1), "UvMap::NewHead");
  InitHead<Map>(base, node);
  return node;
}

void MapConstruct(uint8_t* base, uint32_t map) {
  ITC_NATIVE_RUNNING("sub_82130198 UvMap::UvMap");
  Construct<Map>(base, map, MapNewHead(base));
}

uint32_t MapConstructNode(uint8_t* base, uint32_t node, uint32_t left, uint32_t parent,
                          uint32_t right, uint32_t value, uint8_t color) {
  ITC_NATIVE_RUNNING("sub_82131F50 UvMap::ConstructNode");
  const MapNode n(base, node);
  const schema::ScreenObjectUvMapValue v(base, value);
  n.set_left(left);
  n.set_parent(parent);
  n.set_right(right);
  const uint32_t key_high = v.key_high();
  const uint32_t key_low = v.key_low();
  n.set_key_high(key_high);
  n.set_key_low(key_low);
  ColumnConstruct(base, n.column().address);
  ColumnCopy(base, n.column().address, v.column_address());
  n.set_color(color);
  n.set_is_nil(0);
  return node;
}

void MapRotateLeft(uint8_t* base, uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212FA50 UvMap::RotateLeft");
  RotateLeft<Map>(base, map, node);
}

void MapRotateRight(uint8_t* base, uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212FAB8 UvMap::RotateRight");
  RotateRight<Map>(base, map, node);
}

uint32_t MapLowerBoundNode(uint8_t* base, uint32_t map, double key) {
  ITC_NATIVE_RUNNING("sub_8212F9F8 UvMap::LowerBoundNode");
  return LowerBoundNode<Map>(base, map, key);
}

std::optional<Iterator> MapLowerBound(uint8_t* base, uint32_t map, double key) {
  ITC_NATIVE_RUNNING("sub_82130028 UvMap::LowerBound");
  if (map == 0) {
    return std::nullopt;
  }
  return Iterator{map, MapLowerBoundNode(base, map, key)};
}

std::optional<Iterator> MapFind(uint8_t* base, uint32_t map, double key) {
  ITC_NATIVE_RUNNING("sub_82130290 UvMap::Find");
  const auto it = MapLowerBound(base, map, key);
  if (!it) {
    return std::nullopt;
  }
  return FindFrom<Map>(base, map, *it, key);
}

bool MapDecrement(uint8_t* base, Iterator& it) {
  ITC_NATIVE_RUNNING("sub_8212FB28 UvMap::Decrement");
  return Decrement<Map>(base, it);
}

std::optional<Iterator> MapInsert(uint8_t* base, uint32_t map, bool add_left, uint32_t where,
                                  uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82131FB0 UvMap::Insert");
  const MapTree m(base, map);
  if (m.size() >= Map::kMaxSize || !CanAllocate(base)) {
    return std::nullopt;
  }
  const uint32_t head = m.head().address;
  const uint32_t node = Required(MapAllocateNodes(base, 1), "UvMap::Insert");
  MapConstructNode(base, node, head, where, head, value, 0);
  return LinkAndBalance<Map>(
      base, map, add_left, where, node, [&](uint32_t x) { MapRotateLeft(base, map, x); },
      [&](uint32_t x) { MapRotateRight(base, map, x); });
}

std::optional<InsertResult> MapInsertUnique(uint8_t* base, uint32_t map, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_821321F8 UvMap::InsertUnique");
  const auto place = FindPlace<Map>(base, map, KeyOf(schema::ScreenObjectUvMapValue(base, value)),
                                    [&](Iterator& it) { return MapDecrement(base, it); });
  if (!place) {
    return std::nullopt;
  }
  if (place->exists) {
    return InsertResult{place->existing, false};
  }
  const auto it = MapInsert(base, map, place->add_left, place->where, value);
  if (!it) {
    return std::nullopt;
  }
  return InsertResult{*it, true};
}

uint32_t MapEraseSubtree(uint8_t* base, uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_82132720 UvMap::EraseSubtree");
  uint32_t r3 = map;
  MapNode n(base, node);
  while (!n.is_nil()) {
    MapEraseSubtree(base, map, n.right().address);
    const MapNode next = n.left();
    if (!ColumnDestroy(base, n.column().address)) {
      ITC_LOG_ERROR("UvMap::EraseSubtree: a column at {:08X} not destroyed", n.column().address);
      std::abort();
    }
    Free(n.address, "UvMap::EraseSubtree");
    r3 = n.address;
    n = next;
  }
  return r3;
}

uint32_t MapClear(uint8_t* base, uint32_t map) {
  ITC_NATIVE_RUNNING("sub_82132960 UvMap::Clear");
  const uint32_t r3 = MapEraseSubtree(base, map, MapTree(base, map).head().parent().address);
  ClearHead<Map>(base, map);
  return r3;
}

std::optional<Iterator> MapErase(uint8_t* base, uint32_t map, Iterator first, Iterator last) {
  ITC_NATIVE_RUNNING("sub_82132BD8 UvMap::Erase");
  if (!WholeRange<Map>(base, map, first, last)) {
    return std::nullopt;
  }
  MapClear(base, map);
  return Iterator{map, MapTree(base, map).head().left().address};
}

std::optional<uint32_t> MapDestroy(uint8_t* base, uint32_t map) {
  ITC_NATIVE_RUNNING("sub_82132F58 UvMap::~UvMap");
  if (map == 0) {
    return std::nullopt;
  }
  const MapTree m(base, map);
  const MapNode head = m.head();
  if (!MapErase(base, map, {map, head.left().address}, {map, head.address})) {
    return std::nullopt;
  }
  const uint32_t block = m.head().address;
  Free(block, "UvMap::~UvMap");
  m.set_head(0u);
  m.set_size(0);
  return block;
}

}  // namespace screen_objects
