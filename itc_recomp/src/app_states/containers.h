// The containers the app's update and its state stack use, natively
// (docs/functions/app-states.md): the app's message vector
// (std::vector<unsigned>), the layers' operation list (std::list of 8-byte
// values, 16-byte nodes), the generic list clear, the delegate's clone and
// the layer's emptiness test. Plain C++ over the guest layout, store for
// store as the recompiled functions. Adapters: src/hooks/app_states.cpp.
//
// Blocks come from operator new and go back to operator delete (the C
// runtime heap's plain C++, memory/c_runtime_heap.h), so the functions that
// allocate or free run natively only while --native_heap routes the game's
// heap to the allocation layer (the adapters' condition). The paths that
// never ran in play stay on the recompiled code: a function that would take
// one (the allocators' bad_alloc, IncSize's length_error, a checked
// iterator's trap) returns nullopt (or false) with nothing changed, and its
// adapter runs the game's own function from the start.

#pragma once

#include <cstdint>
#include <optional>

namespace app_states {

// The message vector: +4 first, +8 last, +12 end (the app's +76).
constexpr uint32_t kVectorFirst = 4;
constexpr uint32_t kVectorLast = 8;
constexpr uint32_t kVectorEnd = 12;
constexpr uint32_t kMessagesMaxSize = 0x3FFFFFFF;

// The operation list: +4 the head node, +8 the size; a node +0 next, +4
// previous, +8 and +12 the operation (its type, then a state).
constexpr uint32_t kListHead = 4;
constexpr uint32_t kListSize = 8;
constexpr uint32_t kNodeNext = 0;
constexpr uint32_t kNodePrevious = 4;
constexpr uint32_t kNodeValue = 8;
constexpr uint32_t kOpNodeBytes = 16;
constexpr uint32_t kOpListMaxSize = 0x1FFFFFFF;

// The delegate's vtable (slot 1 Delegate::Invoke, 2 Delegate::Clone).
constexpr uint32_t kDelegateVtable = 0x82002850;
constexpr uint32_t kDelegateBytes = 12;

// sub_82092E70 MessageVector::Erase (erase(first, last)): the words after
// `last` moved down to `first`, last set back. `first` and `last` are the
// checked iterators' elements; false, with nothing done, where the original
// traps (`first_vector` null or not `last_vector`).
bool MessagesErase(uint8_t* base, uint32_t vector, uint32_t first_vector, uint32_t first,
                   uint32_t last_vector, uint32_t last);
// sub_82092F98 MessageVector::Clear: erase(first, last) over everything.
// false where the original traps (first past last).
bool MessagesClear(uint8_t* base, uint32_t vector);
// sub_82092B78 MessageVector::Allocate: operator new(count x 4). nullopt
// where the original builds a bad_alloc (count over 0x3FFFFFFF) or operator
// new fails.
std::optional<uint32_t> MessagesAllocate(uint8_t* base, uint32_t count);

// sub_820ACF10 OpList::AllocateNodes: operator new(count x 16). nullopt
// where the original builds a bad_alloc (count over 0x0FFFFFFF) or operator
// new fails.
std::optional<uint32_t> OpAllocateNodes(uint8_t* base, uint32_t count);
// sub_820AD768 OpList::IncSize: the size (+8) up by `count`. False, with
// nothing done, when 0x1FFFFFFF minus the size is below `count` (the
// original's length_error).
bool OpIncSize(uint8_t* base, uint32_t list, uint32_t count);

// sub_820ACF88 List::Clear (any node size): the head linked to itself, the
// size 0, then each old node freed. Returns the head (r3 as the original
// leaves it).
uint32_t ListClear(uint8_t* base, uint32_t list);

// sub_820ACA48 Delegate::Clone: a 12-byte copy (the vtable, the object and
// function at +4 and +8), or 0 when operator new answers 0. nullopt where
// operator new fails (its new handler path).
std::optional<uint32_t> DelegateClone(uint8_t* base, uint32_t delegate);

// sub_820ACE50 StateLayer::IsEmpty (the layer's state slot 11, `state` the
// layer + 4): 1 when the layer holds no states (+32 the states' count) and
// no children (+20 the children's), else 0.
uint32_t LayerIsEmpty(uint8_t* base, uint32_t state);

}  // namespace app_states
