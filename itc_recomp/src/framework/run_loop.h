// The run loop's containers natively (docs/functions/framework.md): the
// handler maps (Dinkumware's std::map<unsigned, Delegate*>, with checked
// iterators) and the request vector (std::vector of 12-byte requests), as
// plain C++ over their guest layout, store for store as the recompiled
// functions. Adapters: src/hooks/framework.cpp.
//
// A map (12 bytes): +4 its head node, +8 its size. A node (24 bytes): +0
// left, +4 parent, +8 right, +12 the key, +16 the delegate, +20 the color
// (0 red, 1 black), +21 isnil (1 only for the head, whose parent is the root
// and whose left and right are the leftmost and rightmost nodes; leaves link
// to the head). A checked iterator: the map, then the node.
//
// The vector: +4 first, +8 last, +12 end; its checked iterators the vector,
// then the element.
//
// Nodes come from operator new (the C runtime heap's plain C++,
// memory/c_runtime_heap.h), so the functions that allocate run natively only
// while --native_heap routes the game's heap to the allocation layer (the
// adapters' condition). The paths that never ran in play stay on the
// recompiled code: a function that would take one (AllocateNodes' bad_alloc,
// Insert's length_error, a checked iterator's trap) returns nullopt (or
// false) with nothing changed, and its adapter runs the game's own function
// from the start.

#pragma once

#include <cstdint>
#include <optional>

namespace framework {

constexpr uint32_t kMapHead = 4;
constexpr uint32_t kMapSize = 8;
constexpr uint32_t kNodeLeft = 0;
constexpr uint32_t kNodeParent = 4;
constexpr uint32_t kNodeRight = 8;
constexpr uint32_t kNodeKey = 12;
constexpr uint32_t kNodeDelegate = 16;
constexpr uint32_t kNodeColor = 20;
constexpr uint32_t kNodeIsNil = 21;
constexpr uint32_t kNodeBytes = 24;
constexpr uint32_t kMapMaxSize = 0x1FFFFFFE;  // Insert's length_error from here

constexpr uint32_t kRequestBytes = 12;
constexpr uint32_t kVectorFirst = 4;
constexpr uint32_t kVectorLast = 8;

struct Iterator {
  uint32_t container;
  uint32_t node;
};
struct InsertResult {
  Iterator where;
  bool inserted;
};

// Whether nodes can be allocated natively (a process heap for operator new).
bool CanAllocate(uint8_t* base);

// sub_82099B18: the map iterator's operator++ (in key order); false, with
// the iterator unchanged, where the original traps (a null map, or at the
// end).
bool MapIncrement(uint8_t* base, Iterator& it);
// sub_82091AE8: operator--; false where the original traps (a null map, or
// the end of an empty map).
bool MapDecrement(uint8_t* base, Iterator& it);
// sub_82092B10 (_Lrotate) and sub_82092A00 (_Rrotate): `node` rotated
// left (its right child up) or right (its left child up).
void MapRotateLeft(uint8_t* base, uint32_t map, uint32_t node);
void MapRotateRight(uint8_t* base, uint32_t map, uint32_t node);
// sub_82099C80: operator new(count x 24); nullopt where the original throws
// bad_alloc (count over 0xFFFFFFFF / 24) or operator new fails.
std::optional<uint32_t> MapAllocateNodes(uint8_t* base, uint32_t count);
// sub_82099E88: the head node (links 0, black, not nil yet).
uint32_t MapNewHead(uint8_t* base);
// sub_82099FB0 HandlerMap::HandlerMap: the head made, nil, linked to
// itself; size 0. Returns the head (r3 as the original leaves it).
uint32_t MapConstruct(uint8_t* base, uint32_t map);
// sub_82099E20 (_Buynode): a node with the links, the key and delegate at
// `value` and the color.
uint32_t MapNewNode(uint8_t* base, uint32_t left, uint32_t parent, uint32_t right, uint32_t value,
                    uint8_t color);
// sub_82093400 (_Insert): a node for the 8-byte value at `value` linked
// under `where` (on its left when `add_left`) and the tree rebalanced.
// nullopt, with nothing done, at the size limit (the original's
// length_error) or without a heap.
std::optional<Iterator> MapInsert(uint8_t* base, uint32_t map, bool add_left, uint32_t where,
                                  uint32_t value);
// sub_82093740 HandlerMap::Insert (std::map::insert): the value at `value`
// (its key first) added unless its key is there. nullopt, with nothing done,
// where the original would throw or trap.
std::optional<InsertResult> MapInsertUnique(uint8_t* base, uint32_t map, uint32_t value);

// sub_82092EF8: the vector's erase(first, last), 12-byte elements: the
// elements after the range moved down, last set. false where the original
// traps (a null or mismatched iterator).
bool RequestsErase(uint8_t* base, uint32_t vector, Iterator first, Iterator last);
// sub_8209A080 RequestVector::Clear: erase(first, last) over everything.
// false where the original traps (first past last).
bool RequestsClear(uint8_t* base, uint32_t vector);

}  // namespace framework
