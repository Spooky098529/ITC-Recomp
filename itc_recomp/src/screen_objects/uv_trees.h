// The screen objects' two maps (docs/functions/screen-objects.md, "UvMap::"
// and "UvColumn::"): the C++ library's red-black trees (Dinkumware's
// std::map, with checked iterators) as the game instantiated them, as plain
// C++ over their guest layout, store for store as the recompiled functions.
// Adapters: src/hooks/screen_objects.cpp.
//
// The layouts: a tree (12 bytes) +4 its head node, +8 its size (+0, the
// allocator, is empty). A node: +0 left, +4 parent, +8 right, +16 its key (a
// double), then its value, its color (0 red, 1 black) and its isnil byte
// (1 only for the head, whose parent is the root and whose left and right
// are the leftmost and rightmost nodes; leaves link to the head).
// - UvColumn (std::map<double, {float u, v}>): 40-byte nodes, the two
//   floats' words at +24 and +28, the color at +32, isnil at +33.
// - UvMap (std::map<double, UvColumn>): 48-byte nodes, a UvColumn at +24
//   (its head at +28, its size at +32), the color at +40, isnil at +41.
//
// Nodes come from operator new and go back with operator delete (the C
// runtime heap's plain C++, memory/c_runtime_heap.h), so these run natively
// only while --native_heap routes the game's heap to the allocation layer
// (the adapters' condition). The paths that never ran in play stay on the
// recompiled code: a function that would take one (AllocateNodes' bad_alloc,
// Insert's length_error, an erase of part of a tree, a checked iterator's
// trap) returns nullopt (or false) with nothing changed, and its adapter
// runs the game's own function from the start. Inside a native caller,
// operator new failing after the caller's checks is logged and aborts, as
// the std::list's (std_list/ptr_list.cpp).

#pragma once

#include <cstdint>
#include <optional>

namespace screen_objects {

constexpr uint32_t kLeft = 0;
constexpr uint32_t kParent = 4;
constexpr uint32_t kRight = 8;
constexpr uint32_t kKey = 16;
constexpr uint32_t kHead = 4;
constexpr uint32_t kSize = 8;

constexpr uint32_t kColumnNodeBytes = 40;
constexpr uint32_t kColumnColor = 32;
constexpr uint32_t kColumnIsNil = 33;
constexpr uint32_t kColumnValueU = 24;
constexpr uint32_t kColumnValueV = 28;
constexpr uint32_t kColumnMaxSize = 0x0FFFFFFE;  // Insert's length_error from here

constexpr uint32_t kMapNodeBytes = 48;
constexpr uint32_t kMapColumn = 24;
constexpr uint32_t kMapColor = 40;
constexpr uint32_t kMapIsNil = 41;
constexpr uint32_t kMapMaxSize = 0x0AAAAAA9;

// A checked iterator: the tree it belongs to, then its node.
struct Iterator {
  uint32_t tree;
  uint32_t node;
};
// std::map::insert's pair: where the key is, and whether it was added.
struct InsertResult {
  Iterator where;
  bool inserted;
};

// Whether nodes can be allocated natively (a process heap for malloc).
bool CanAllocate(uint8_t* base);

// --- UvColumn: std::map<double, {float u, v}> ---

// sub_8212FBD8 UvColumn::AllocateNodes: operator new(count x 40) (0 too);
// nullopt where the original throws bad_alloc (count over 0xFFFFFFFF / 40)
// or operator new fails.
std::optional<uint32_t> ColumnAllocateNodes(uint8_t* base, uint32_t count);
// sub_8212FF08 UvColumn::NewHead: one node, its links 0, black, not nil.
uint32_t ColumnNewHead(uint8_t* base);
// sub_82130140 UvColumn::UvColumn: the head made, nil, linked to itself;
// size 0.
void ColumnConstruct(uint8_t* base, uint32_t column);
// sub_8212FDB0 UvColumn::NewNode (_Buynode): a node with the links, the 16
// bytes at `value` and the color. Returns it.
uint32_t ColumnNewNode(uint8_t* base, uint32_t left, uint32_t parent, uint32_t right,
                       uint32_t value, uint8_t color);
// sub_8212FE80 UvColumn::CopySubtree (_Copy): the subtree at `source` copied
// under `parent`; returns its root (the column's head for a nil source).
uint32_t ColumnCopySubtree(uint8_t* base, uint32_t column, uint32_t source, uint32_t parent);
// sub_82130088 UvColumn::Copy: a constructed column made a copy of
// `source`'s nodes, size, leftmost and rightmost. Returns the new root (r3
// as the original leaves it).
uint32_t ColumnCopy(uint8_t* base, uint32_t column, uint32_t source);
// sub_8212F920 UvColumn::RotateLeft (_Lrotate), sub_8212F988 RotateRight.
void ColumnRotateLeft(uint8_t* base, uint32_t column, uint32_t node);
void ColumnRotateRight(uint8_t* base, uint32_t column, uint32_t node);
// sub_8212F8C8 UvColumn::LowerBoundNode: the first node whose key isn't
// below `key`, or the head.
uint32_t ColumnLowerBoundNode(uint8_t* base, uint32_t column, double key);
// sub_8212FFC8 UvColumn::LowerBound: {column, LowerBoundNode}; nullopt for
// a null column (the original traps).
std::optional<Iterator> ColumnLowerBound(uint8_t* base, uint32_t column, double key);
// sub_821301F0 UvColumn::Find: the key's node, or {column, head}; nullopt
// where the original traps.
std::optional<Iterator> ColumnFind(uint8_t* base, uint32_t column, double key);
// sub_8212EEF8 (UvColumn's iterator decrement, never seen in play: no row,
// ported for InsertUnique): false where the original traps (a null tree,
// before begin), with the iterator unchanged.
bool ColumnDecrement(uint8_t* base, Iterator& it);
// sub_82131680 UvColumn::Insert (_Insert): a node for the 16-byte value at
// `value` linked under `where` (on its left when `add_left`) and the tree
// rebalanced. nullopt, with nothing done, at the size limit (the original's
// length_error).
std::optional<Iterator> ColumnInsert(uint8_t* base, uint32_t column, bool add_left, uint32_t where,
                                     uint32_t value);
// sub_82131CB8 UvColumn::InsertUnique (std::map::insert): the value at
// `value` (its double key first) added unless its key is there. nullopt,
// with nothing done, where the original would throw or trap.
std::optional<InsertResult> ColumnInsertUnique(uint8_t* base, uint32_t column, uint32_t value);
// sub_8212FD50 UvColumn::EraseSubtree (_Erase): the subtree's nodes freed.
// Returns r3 as the original leaves it: the last node freed, or `column`
// when `node` is nil.
uint32_t ColumnEraseSubtree(uint8_t* base, uint32_t column, uint32_t node);
// sub_8212FE28 UvColumn::Clear: every node freed, the head linked to
// itself, size 0. Returns EraseSubtree's r3.
uint32_t ColumnClear(uint8_t* base, uint32_t column);
// sub_82131DD8 UvColumn::Erase (erase(first, last)): only the whole column
// (Clear; returns {column, the head's left}); nullopt for anything else (an
// erase node by node, never seen in play, or a trap).
std::optional<Iterator> ColumnErase(uint8_t* base, uint32_t column, Iterator first, Iterator last);
// sub_82131EE0 UvColumn::~UvColumn: Erase(begin, end), the head freed, +4 and
// +8 = 0. Returns the head (r3 as the original leaves it). nullopt for a
// null column (the original traps).
std::optional<uint32_t> ColumnDestroy(uint8_t* base, uint32_t column);

// --- UvMap: std::map<double, UvColumn> ---

// sub_8212FC50 UvMap::AllocateNodes: as ColumnAllocateNodes, 48 bytes.
std::optional<uint32_t> MapAllocateNodes(uint8_t* base, uint32_t count);
// sub_8212FF68 UvMap::NewHead.
uint32_t MapNewHead(uint8_t* base);
// sub_82130198 UvMap::UvMap.
void MapConstruct(uint8_t* base, uint32_t map);
// sub_82131F50 UvMap::ConstructNode: `node`'s links, the key at `value`,
// its column made a copy of the one at `value` + 8, the color. Returns node.
uint32_t MapConstructNode(uint8_t* base, uint32_t node, uint32_t left, uint32_t parent,
                          uint32_t right, uint32_t value, uint8_t color);
// sub_8212FA50 UvMap::RotateLeft, sub_8212FAB8 RotateRight.
void MapRotateLeft(uint8_t* base, uint32_t map, uint32_t node);
void MapRotateRight(uint8_t* base, uint32_t map, uint32_t node);
// sub_8212F9F8 UvMap::LowerBoundNode, sub_82130028 LowerBound,
// sub_82130290 Find: as the column's.
uint32_t MapLowerBoundNode(uint8_t* base, uint32_t map, double key);
std::optional<Iterator> MapLowerBound(uint8_t* base, uint32_t map, double key);
std::optional<Iterator> MapFind(uint8_t* base, uint32_t map, double key);
// sub_8212FB28 UvMap::Decrement: as ColumnDecrement.
bool MapDecrement(uint8_t* base, Iterator& it);
// sub_82131FB0 UvMap::Insert: as ColumnInsert (the node from
// MapAllocateNodes and MapConstructNode; the value a key and a column).
std::optional<Iterator> MapInsert(uint8_t* base, uint32_t map, bool add_left, uint32_t where,
                                  uint32_t value);
// sub_821321F8 UvMap::InsertUnique: as ColumnInsertUnique.
std::optional<InsertResult> MapInsertUnique(uint8_t* base, uint32_t map, uint32_t value);
// sub_82132720 UvMap::EraseSubtree: each node's column destroyed
// (ColumnDestroy) and the node freed. Returns r3 as the column's.
uint32_t MapEraseSubtree(uint8_t* base, uint32_t map, uint32_t node);
// sub_82132960 UvMap::Clear, sub_82132BD8 Erase, sub_82132F58 ~UvMap: as the
// column's.
uint32_t MapClear(uint8_t* base, uint32_t map);
std::optional<Iterator> MapErase(uint8_t* base, uint32_t map, Iterator first, Iterator last);
std::optional<uint32_t> MapDestroy(uint8_t* base, uint32_t map);

}  // namespace screen_objects
