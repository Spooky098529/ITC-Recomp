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

// The message vector (the app's +76), the operation list and its nodes, the
// delegate: schema/app_states.h (itc_recomp/schema/app-states.toml).
constexpr uint32_t kMessagesMaxSize = 0x3FFFFFFF;

constexpr uint32_t kOpListMaxSize = 0x1FFFFFFF;

// The delegate's vtable (slot 1 Delegate::Invoke, 2 Delegate::Clone).
constexpr uint32_t kDelegateVtable = 0x82002850;

// sub_82092E70 MessageVector::Erase (erase(first, last)): the words after
// `last` moved down to `first`, last set back. `first` and `last` are the
// checked iterators' elements; false, with nothing done, where the original
// traps (`first_vector` null or not `last_vector`).
bool MessagesErase(uint32_t vector, uint32_t first_vector, uint32_t first,
                   uint32_t last_vector, uint32_t last);
// sub_82092F98 MessageVector::Clear: erase(first, last) over everything.
// false where the original traps (first past last).
bool MessagesClear(uint32_t vector);
// sub_82092B78 MessageVector::Allocate: operator new(count x 4). nullopt
// where the original builds a bad_alloc (count over 0x3FFFFFFF) or operator
// new fails.
std::optional<uint32_t> MessagesAllocate(uint32_t count);

// sub_820ACF10 OpList::AllocateNodes: operator new(count x 16). nullopt
// where the original builds a bad_alloc (count over 0x0FFFFFFF) or operator
// new fails.
std::optional<uint32_t> OpAllocateNodes(uint32_t count);
// sub_820AD768 OpList::IncSize: the size (+8) up by `count`. False, with
// nothing done, when 0x1FFFFFFF minus the size is below `count` (the
// original's length_error).
bool OpIncSize(uint32_t list, uint32_t count);

// sub_820AD0B8 OpList::OpList: a head node (OpList::AllocateNodes with 1)
// linked to itself (+0 and +4) at +4, the size 0. False, with nothing done,
// where the original must run (no heap, or operator new failing).
bool OpConstruct(uint32_t list);
// sub_820ACFF8 OpList::BuyNode (_Buynode): a node with +0 `next`, +4
// `previous` and the operation (`type`, `state`) at +8. The node (0 when
// operator new answers 0), or nullopt where the original must run.
std::optional<uint32_t> OpBuyNode(uint32_t next, uint32_t previous, uint32_t type,
                                  uint32_t state);
// sub_820ADAD0 StateLayer::Push (the layer's slot 3): the operation {2,
// `state`} appended to the layer's queue (+36). False, with nothing done,
// where the original must run (IncSize's length_error, no heap).
bool LayerPush(uint32_t layer, uint32_t state);

// sub_820938C0 MessageVector::InsertN (_Insert_n): `count` copies of
// `value` inserted before the element `where` (a new block from
// MessageVector::Allocate when it doesn't fit, the old one freed). False,
// with nothing done, for the length_error (sub_820A92D0) and an allocation
// the heap can't give.
bool MessagesInsertN(uint32_t vector, uint32_t where, uint32_t count,
                     uint32_t value);
// sub_82094110 MessageVector::Insert (insert(where, value)): InsertN of
// one, then the new element's address (the result iterator's). nullopt
// where the original traps (the iterator's vector `where_vector` not the
// vector, first past last, the result out of range) or InsertN hands back.
std::optional<uint32_t> MessagesInsert(uint32_t vector, uint32_t where_vector,
                                       uint32_t where, uint32_t value);
// sub_820944E0 MessageVector::PushBack: the word at `value` stored at the
// end, or Insert at the end when full. Whether it took Insert (the
// original's r3 differs), or nullopt where it traps or Insert hands back.
std::optional<bool> MessagesPushBack(uint32_t vector, uint32_t value);

// sub_820ACF88 List::Clear (any node size): the head linked to itself, the
// size 0, then each old node freed. Returns the head (r3 as the original
// leaves it).
uint32_t ListClear(uint32_t list);

// sub_820ACA48 Delegate::Clone: a 12-byte copy (the vtable, the object and
// function at +4 and +8), or 0 when operator new answers 0. nullopt where
// operator new fails (its new handler path).
std::optional<uint32_t> DelegateClone(uint32_t delegate);

// sub_820ACE50 StateLayer::IsEmpty (the layer's state slot 11, `state` the
// layer + 4): 1 when the layer holds no states (+32 the states' count) and
// no children (+20 the children's), else 0.
uint32_t LayerIsEmpty(uint32_t state);

}  // namespace app_states
