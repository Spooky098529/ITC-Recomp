// The screen objects' two red-black trees (see uv_trees.h), store for store
// as the recompiled functions. The two instantiations share their code here
// (only the node's size and its color and isnil offsets differ); each
// function the game has keeps its own plain C++ entry, which calls the
// others' entries, so each logs its own ITC_NATIVE_RUNNING.

#include "screen_objects/uv_trees.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

#include <bit>
#include <cstdlib>

namespace screen_objects {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(Load64(base, address));
}

struct Layout {
  uint32_t node_bytes;
  uint32_t color;
  uint32_t is_nil;
  uint32_t max_size;
};
constexpr Layout kColumn{kColumnNodeBytes, kColumnColor, kColumnIsNil, kColumnMaxSize};
constexpr Layout kMap{kMapNodeBytes, kMapColor, kMapIsNil, kMapMaxSize};

bool IsNil(uint8_t* base, const Layout& l, uint32_t node) {
  return guest::LoadU8(base, node + l.is_nil) != 0;
}
uint8_t Color(uint8_t* base, const Layout& l, uint32_t node) {
  return guest::LoadU8(base, node + l.color);
}
void SetColor(uint8_t* base, const Layout& l, uint32_t node, uint8_t color) {
  guest::StoreU8(base, node + l.color, color);
}
uint32_t HeadOf(uint8_t* base, uint32_t tree) { return Load(base, tree + kHead); }

// A node allocated inside a native caller, after its checks.
uint32_t Required(std::optional<uint32_t> block, const char* what) {
  if (!block) {
    ITC_LOG_ERROR("{}: operator new refused a node after its checks", what);
    std::abort();
  }
  return *block;
}

std::optional<uint32_t> AllocateNodes(uint8_t* base, const Layout& l, uint32_t count) {
  // The original's bad_alloc path: 0xFFFFFFFF / count below the node's size.
  if (count != 0 && 0xFFFFFFFFu / count < l.node_bytes) {
    return std::nullopt;
  }
  return memory::crt::OperatorNew(base, count * l.node_bytes);
}

// NewHead's stores, the original's null checks kept.
void InitHead(uint8_t* base, const Layout& l, uint32_t node) {
  if (node != 0) {
    Store(base, node + kLeft, 0);
  }
  if (node + kParent != 0) {
    Store(base, node + kParent, 0);
  }
  if (node + kRight != 0) {
    Store(base, node + kRight, 0);
  }
  guest::StoreU8(base, node + l.color, 1);
  guest::StoreU8(base, node + l.is_nil, 0);
}

void Construct(uint8_t* base, const Layout& l, uint32_t tree, uint32_t head) {
  Store(base, tree + kHead, head);
  guest::StoreU8(base, head + l.is_nil, 1);
  Store(base, head + kParent, head);
  Store(base, head + kLeft, head);
  Store(base, head + kRight, head);
  Store(base, tree + kSize, 0);
}

void RotateLeft(uint8_t* base, const Layout& l, uint32_t tree, uint32_t x) {
  const uint32_t y = Load(base, x + kRight);
  Store(base, x + kRight, Load(base, y + kLeft));
  const uint32_t y_left = Load(base, y + kLeft);
  if (!IsNil(base, l, y_left)) {
    Store(base, y_left + kParent, x);
  }
  Store(base, y + kParent, Load(base, x + kParent));
  const uint32_t head = HeadOf(base, tree);
  if (x == Load(base, head + kParent)) {
    Store(base, head + kParent, y);
  } else {
    const uint32_t parent = Load(base, x + kParent);
    if (x == Load(base, parent + kLeft)) {
      Store(base, parent + kLeft, y);
    } else {
      Store(base, parent + kRight, y);
    }
  }
  Store(base, y + kLeft, x);
  Store(base, x + kParent, y);
}

void RotateRight(uint8_t* base, const Layout& l, uint32_t tree, uint32_t x) {
  const uint32_t y = Load(base, x + kLeft);
  Store(base, x + kLeft, Load(base, y + kRight));
  const uint32_t y_right = Load(base, y + kRight);
  if (!IsNil(base, l, y_right)) {
    Store(base, y_right + kParent, x);
  }
  Store(base, y + kParent, Load(base, x + kParent));
  const uint32_t head = HeadOf(base, tree);
  if (x == Load(base, head + kParent)) {
    Store(base, head + kParent, y);
  } else {
    const uint32_t parent = Load(base, x + kParent);
    if (x == Load(base, parent + kRight)) {
      Store(base, parent + kRight, y);
    } else {
      Store(base, parent + kLeft, y);
    }
  }
  Store(base, y + kRight, x);
  Store(base, x + kParent, y);
}

uint32_t LowerBoundNode(uint8_t* base, const Layout& l, uint32_t tree, double key) {
  uint32_t result = HeadOf(base, tree);
  uint32_t node = Load(base, result + kParent);
  while (!IsNil(base, l, node)) {
    if (LoadDouble(base, node + kKey) < key) {
      node = Load(base, node + kRight);
    } else {
      result = node;
      node = Load(base, node + kLeft);
    }
  }
  return result;
}

// Find's step after LowerBound: end when it's the head or `key` is below
// its key. nullopt where the original traps (an iterator of another tree).
std::optional<Iterator> FindFrom(uint8_t* base, uint32_t tree, Iterator it, double key) {
  if (it.tree == 0 || it.tree != tree) {
    return std::nullopt;
  }
  const uint32_t head = HeadOf(base, tree);
  if (it.node == head || key < LoadDouble(base, it.node + kKey)) {
    return Iterator{tree, head};
  }
  return it;
}

bool Decrement(uint8_t* base, const Layout& l, Iterator& it) {
  if (it.tree == 0) {
    return false;
  }
  const uint32_t node = it.node;
  if (IsNil(base, l, node)) {
    // end: the rightmost node, a trap when the tree is empty.
    const uint32_t rightmost = Load(base, node + kRight);
    if (IsNil(base, l, rightmost)) {
      return false;
    }
    it.node = rightmost;
    return true;
  }
  uint32_t left = Load(base, node + kLeft);
  if (!IsNil(base, l, left)) {
    while (!IsNil(base, l, Load(base, left + kRight))) {
      left = Load(base, left + kRight);
    }
    it.node = left;
    return true;
  }
  uint32_t current = node;
  uint32_t parent = Load(base, node + kParent);
  while (!IsNil(base, l, parent) && current == Load(base, parent + kLeft)) {
    current = parent;
    parent = Load(base, parent + kParent);
  }
  if (IsNil(base, l, current)) {
    return false;
  }
  it.node = parent;
  return true;
}

// Insert's links and rebalance, once the node is made: `rotate_left` and
// `rotate_right` are the tree's own.
template <typename RotateLeftFn, typename RotateRightFn>
Iterator LinkAndBalance(uint8_t* base, const Layout& l, uint32_t tree, bool add_left,
                        uint32_t where, uint32_t node, RotateLeftFn rotate_left,
                        RotateRightFn rotate_right) {
  Store(base, tree + kSize, Load(base, tree + kSize) + 1);
  uint32_t head = HeadOf(base, tree);
  if (where == head) {
    Store(base, head + kParent, node);
    Store(base, HeadOf(base, tree) + kLeft, node);
    Store(base, HeadOf(base, tree) + kRight, node);
  } else if (add_left) {
    Store(base, where + kLeft, node);
    head = HeadOf(base, tree);
    if (where == Load(base, head + kLeft)) {
      Store(base, head + kLeft, node);
    }
  } else {
    Store(base, where + kRight, node);
    head = HeadOf(base, tree);
    if (where == Load(base, head + kRight)) {
      Store(base, head + kRight, node);
    }
  }
  uint32_t x = node;
  while (Color(base, l, Load(base, x + kParent)) == 0) {
    const uint32_t parent = Load(base, x + kParent);
    const uint32_t grandparent = Load(base, parent + kParent);
    if (parent == Load(base, grandparent + kLeft)) {
      const uint32_t uncle = Load(base, grandparent + kRight);
      if (Color(base, l, uncle) == 0) {
        SetColor(base, l, Load(base, x + kParent), 1);
        SetColor(base, l, uncle, 1);
        SetColor(base, l, Load(base, Load(base, x + kParent) + kParent), 0);
        x = Load(base, Load(base, x + kParent) + kParent);
      } else {
        if (x == Load(base, parent + kRight)) {
          x = parent;
          rotate_left(x);
        }
        SetColor(base, l, Load(base, x + kParent), 1);
        SetColor(base, l, Load(base, Load(base, x + kParent) + kParent), 0);
        rotate_right(Load(base, Load(base, x + kParent) + kParent));
      }
    } else {
      const uint32_t uncle = Load(base, grandparent + kLeft);
      if (Color(base, l, uncle) == 0) {
        SetColor(base, l, Load(base, x + kParent), 1);
        SetColor(base, l, uncle, 1);
        SetColor(base, l, Load(base, Load(base, x + kParent) + kParent), 0);
        x = Load(base, Load(base, x + kParent) + kParent);
      } else {
        if (x == Load(base, parent + kLeft)) {
          x = parent;
          rotate_right(x);
        }
        SetColor(base, l, Load(base, x + kParent), 1);
        SetColor(base, l, Load(base, Load(base, x + kParent) + kParent), 0);
        rotate_left(Load(base, Load(base, x + kParent) + kParent));
      }
    }
  }
  SetColor(base, l, Load(base, HeadOf(base, tree) + kParent), 1);
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
template <typename DecrementFn>
std::optional<Place> FindPlace(uint8_t* base, const Layout& l, uint32_t tree, double key,
                               DecrementFn decrement) {
  const uint32_t head = HeadOf(base, tree);
  uint32_t where = head;
  bool add_left = true;
  uint32_t node = Load(base, head + kParent);
  while (!IsNil(base, l, node)) {
    where = node;
    add_left = key < LoadDouble(base, node + kKey);
    node = Load(base, node + (add_left ? kLeft : kRight));
  }
  Iterator it{tree, where};
  if (add_left) {
    if (where == Load(base, head + kLeft)) {
      return Place{where, true, false, {}};
    }
    if (!decrement(it)) {
      return std::nullopt;
    }
  }
  if (LoadDouble(base, it.node + kKey) < key) {
    return Place{where, add_left, false, {}};
  }
  return Place{where, add_left, true, it};
}

void ClearHead(uint8_t* base, uint32_t tree) {
  const uint32_t head = HeadOf(base, tree);
  Store(base, head + kParent, head);
  Store(base, tree + kSize, 0);
  Store(base, HeadOf(base, tree) + kLeft, HeadOf(base, tree));
  Store(base, HeadOf(base, tree) + kRight, HeadOf(base, tree));
}

// Erase's checks: whether [first, last) is the whole tree (begin, end) and
// no iterator traps.
bool WholeRange(uint8_t* base, uint32_t tree, Iterator first, Iterator last) {
  const uint32_t head = HeadOf(base, tree);
  if (first.tree == 0 || first.tree != tree) {
    return false;
  }
  if (first.node != Load(base, head + kLeft)) {
    return false;
  }
  return last.tree != 0 && last.tree == tree && last.node == head;
}

void Free(uint32_t block, const char* what) {
  if (!memory::crt::OperatorDelete(block)) {
    ITC_LOG_ERROR("{}: operator delete refused {:08X}", what, block);
    std::abort();
  }
}

}  // namespace

bool CanAllocate(uint8_t* base) { return memory::crt::GetProcessHeap(base) != 0; }

// --- UvColumn ---

std::optional<uint32_t> ColumnAllocateNodes(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8212FBD8 UvColumn::AllocateNodes");
  return AllocateNodes(base, kColumn, count);
}

uint32_t ColumnNewHead(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8212FF08 UvColumn::NewHead");
  const uint32_t node = Required(ColumnAllocateNodes(base, 1), "UvColumn::NewHead");
  InitHead(base, kColumn, node);
  return node;
}

void ColumnConstruct(uint8_t* base, uint32_t column) {
  ITC_NATIVE_RUNNING("sub_82130140 UvColumn::UvColumn");
  Construct(base, kColumn, column, ColumnNewHead(base));
}

uint32_t ColumnNewNode(uint8_t* base, uint32_t left, uint32_t parent, uint32_t right,
                       uint32_t value, uint8_t color) {
  ITC_NATIVE_RUNNING("sub_8212FDB0 UvColumn::NewNode");
  const uint32_t node = Required(ColumnAllocateNodes(base, 1), "UvColumn::NewNode");
  Store(base, node + kLeft, left);
  Store(base, node + kParent, parent);
  Store(base, node + kRight, right);
  for (uint32_t i = 0; i < 16; i += 4) {
    Store(base, node + kKey + i, Load(base, value + i));
  }
  guest::StoreU8(base, node + kColumnColor, color);
  guest::StoreU8(base, node + kColumnIsNil, 0);
  return node;
}

uint32_t ColumnCopySubtree(uint8_t* base, uint32_t column, uint32_t source, uint32_t parent) {
  ITC_NATIVE_RUNNING("sub_8212FE80 UvColumn::CopySubtree");
  const uint32_t head = HeadOf(base, column);
  uint32_t result = head;
  if (IsNil(base, kColumn, source)) {
    return result;
  }
  const uint32_t node = ColumnNewNode(base, head, parent, head, source + kKey,
                                      guest::LoadU8(base, source + kColumnColor));
  if (IsNil(base, kColumn, head)) {
    result = node;
  }
  Store(base, node + kLeft, ColumnCopySubtree(base, column, Load(base, source + kLeft), node));
  Store(base, node + kRight, ColumnCopySubtree(base, column, Load(base, source + kRight), node));
  return result;
}

uint32_t ColumnCopy(uint8_t* base, uint32_t column, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_82130088 UvColumn::Copy");
  const uint32_t root = ColumnCopySubtree(base, column, Load(base, HeadOf(base, source) + kParent),
                                          HeadOf(base, column));
  Store(base, HeadOf(base, column) + kParent, root);
  Store(base, column + kSize, Load(base, source + kSize));
  const uint32_t head = HeadOf(base, column);
  uint32_t node = Load(base, head + kParent);
  if (IsNil(base, kColumn, node)) {
    Store(base, head + kLeft, head);
    Store(base, HeadOf(base, column) + kRight, HeadOf(base, column));
    return root;
  }
  while (!IsNil(base, kColumn, Load(base, node + kLeft))) {
    node = Load(base, node + kLeft);
  }
  Store(base, head + kLeft, node);
  const uint32_t head_again = HeadOf(base, column);
  node = Load(base, head_again + kParent);
  while (!IsNil(base, kColumn, Load(base, node + kRight))) {
    node = Load(base, node + kRight);
  }
  Store(base, head_again + kRight, node);
  return root;
}

void ColumnRotateLeft(uint8_t* base, uint32_t column, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212F920 UvColumn::RotateLeft");
  RotateLeft(base, kColumn, column, node);
}

void ColumnRotateRight(uint8_t* base, uint32_t column, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212F988 UvColumn::RotateRight");
  RotateRight(base, kColumn, column, node);
}

uint32_t ColumnLowerBoundNode(uint8_t* base, uint32_t column, double key) {
  ITC_NATIVE_RUNNING("sub_8212F8C8 UvColumn::LowerBoundNode");
  return LowerBoundNode(base, kColumn, column, key);
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
  return FindFrom(base, column, *it, key);
}

bool ColumnDecrement(uint8_t* base, Iterator& it) {
  ITC_NATIVE_RUNNING("sub_8212EEF8 UvColumn's iterator decrement");
  return Decrement(base, kColumn, it);
}

std::optional<Iterator> ColumnInsert(uint8_t* base, uint32_t column, bool add_left, uint32_t where,
                                     uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82131680 UvColumn::Insert");
  if (Load(base, column + kSize) >= kColumnMaxSize || !CanAllocate(base)) {
    return std::nullopt;
  }
  const uint32_t head = HeadOf(base, column);
  const uint32_t node = ColumnNewNode(base, head, where, head, value, 0);
  return LinkAndBalance(
      base, kColumn, column, add_left, where, node,
      [&](uint32_t x) { ColumnRotateLeft(base, column, x); },
      [&](uint32_t x) { ColumnRotateRight(base, column, x); });
}

std::optional<InsertResult> ColumnInsertUnique(uint8_t* base, uint32_t column, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82131CB8 UvColumn::InsertUnique");
  const auto place = FindPlace(base, kColumn, column, LoadDouble(base, value),
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
  while (!IsNil(base, kColumn, node)) {
    ColumnEraseSubtree(base, column, Load(base, node + kRight));
    const uint32_t next = Load(base, node + kLeft);
    Free(node, "UvColumn::EraseSubtree");
    r3 = node;
    node = next;
  }
  return r3;
}

uint32_t ColumnClear(uint8_t* base, uint32_t column) {
  ITC_NATIVE_RUNNING("sub_8212FE28 UvColumn::Clear");
  const uint32_t r3 =
      ColumnEraseSubtree(base, column, Load(base, HeadOf(base, column) + kParent));
  ClearHead(base, column);
  return r3;
}

std::optional<Iterator> ColumnErase(uint8_t* base, uint32_t column, Iterator first,
                                    Iterator last) {
  ITC_NATIVE_RUNNING("sub_82131DD8 UvColumn::Erase");
  if (!WholeRange(base, column, first, last)) {
    return std::nullopt;
  }
  ColumnClear(base, column);
  return Iterator{column, Load(base, HeadOf(base, column) + kLeft)};
}

std::optional<uint32_t> ColumnDestroy(uint8_t* base, uint32_t column) {
  ITC_NATIVE_RUNNING("sub_82131EE0 UvColumn::~UvColumn");
  if (column == 0) {
    return std::nullopt;
  }
  const uint32_t head = HeadOf(base, column);
  if (!ColumnErase(base, column, {column, Load(base, head + kLeft)}, {column, head})) {
    return std::nullopt;
  }
  const uint32_t block = HeadOf(base, column);
  Free(block, "UvColumn::~UvColumn");
  Store(base, column + kHead, 0);
  Store(base, column + kSize, 0);
  return block;
}

// --- UvMap ---

std::optional<uint32_t> MapAllocateNodes(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8212FC50 UvMap::AllocateNodes");
  return AllocateNodes(base, kMap, count);
}

uint32_t MapNewHead(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8212FF68 UvMap::NewHead");
  const uint32_t node = Required(MapAllocateNodes(base, 1), "UvMap::NewHead");
  InitHead(base, kMap, node);
  return node;
}

void MapConstruct(uint8_t* base, uint32_t map) {
  ITC_NATIVE_RUNNING("sub_82130198 UvMap::UvMap");
  Construct(base, kMap, map, MapNewHead(base));
}

uint32_t MapConstructNode(uint8_t* base, uint32_t node, uint32_t left, uint32_t parent,
                          uint32_t right, uint32_t value, uint8_t color) {
  ITC_NATIVE_RUNNING("sub_82131F50 UvMap::ConstructNode");
  Store(base, node + kLeft, left);
  Store(base, node + kParent, parent);
  Store(base, node + kRight, right);
  Store64(base, node + kKey, Load64(base, value));
  ColumnConstruct(base, node + kMapColumn);
  ColumnCopy(base, node + kMapColumn, value + 8);
  guest::StoreU8(base, node + kMapColor, color);
  guest::StoreU8(base, node + kMapIsNil, 0);
  return node;
}

void MapRotateLeft(uint8_t* base, uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212FA50 UvMap::RotateLeft");
  RotateLeft(base, kMap, map, node);
}

void MapRotateRight(uint8_t* base, uint32_t map, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8212FAB8 UvMap::RotateRight");
  RotateRight(base, kMap, map, node);
}

uint32_t MapLowerBoundNode(uint8_t* base, uint32_t map, double key) {
  ITC_NATIVE_RUNNING("sub_8212F9F8 UvMap::LowerBoundNode");
  return LowerBoundNode(base, kMap, map, key);
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
  return FindFrom(base, map, *it, key);
}

bool MapDecrement(uint8_t* base, Iterator& it) {
  ITC_NATIVE_RUNNING("sub_8212FB28 UvMap::Decrement");
  return Decrement(base, kMap, it);
}

std::optional<Iterator> MapInsert(uint8_t* base, uint32_t map, bool add_left, uint32_t where,
                                  uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82131FB0 UvMap::Insert");
  if (Load(base, map + kSize) >= kMapMaxSize || !CanAllocate(base)) {
    return std::nullopt;
  }
  const uint32_t head = HeadOf(base, map);
  const uint32_t node = Required(MapAllocateNodes(base, 1), "UvMap::Insert");
  MapConstructNode(base, node, head, where, head, value, 0);
  return LinkAndBalance(
      base, kMap, map, add_left, where, node, [&](uint32_t x) { MapRotateLeft(base, map, x); },
      [&](uint32_t x) { MapRotateRight(base, map, x); });
}

std::optional<InsertResult> MapInsertUnique(uint8_t* base, uint32_t map, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_821321F8 UvMap::InsertUnique");
  const auto place = FindPlace(base, kMap, map, LoadDouble(base, value),
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
  while (!IsNil(base, kMap, node)) {
    MapEraseSubtree(base, map, Load(base, node + kRight));
    const uint32_t next = Load(base, node + kLeft);
    if (!ColumnDestroy(base, node + kMapColumn)) {
      ITC_LOG_ERROR("UvMap::EraseSubtree: a column at {:08X} not destroyed", node + kMapColumn);
      std::abort();
    }
    Free(node, "UvMap::EraseSubtree");
    r3 = node;
    node = next;
  }
  return r3;
}

uint32_t MapClear(uint8_t* base, uint32_t map) {
  ITC_NATIVE_RUNNING("sub_82132960 UvMap::Clear");
  const uint32_t r3 = MapEraseSubtree(base, map, Load(base, HeadOf(base, map) + kParent));
  ClearHead(base, map);
  return r3;
}

std::optional<Iterator> MapErase(uint8_t* base, uint32_t map, Iterator first, Iterator last) {
  ITC_NATIVE_RUNNING("sub_82132BD8 UvMap::Erase");
  if (!WholeRange(base, map, first, last)) {
    return std::nullopt;
  }
  MapClear(base, map);
  return Iterator{map, Load(base, HeadOf(base, map) + kLeft)};
}

std::optional<uint32_t> MapDestroy(uint8_t* base, uint32_t map) {
  ITC_NATIVE_RUNNING("sub_82132F58 UvMap::~UvMap");
  if (map == 0) {
    return std::nullopt;
  }
  const uint32_t head = HeadOf(base, map);
  if (!MapErase(base, map, {map, Load(base, head + kLeft)}, {map, head})) {
    return std::nullopt;
  }
  const uint32_t block = HeadOf(base, map);
  Free(block, "UvMap::~UvMap");
  Store(base, map + kHead, 0);
  Store(base, map + kSize, 0);
  return block;
}

}  // namespace screen_objects
