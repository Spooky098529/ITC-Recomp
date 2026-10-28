// The C++ library's std::list of pointers (Dinkumware's, as instantiated
// across the game) as plain C++ over its guest layout
// (docs/functions/input.md, "PtrList::"). Shared: the input manager's
// listener lists, the sound manager's (sound-cues.md) and others, so any
// native caller can use these. Adapters: src/hooks/input_manager.cpp (Erase's:
// src/hooks/app_states.cpp).
//
// The layout: the list (12 bytes) +4 its head node, +8 its size (+0, the
// allocator, is empty); a node (12 bytes) +0 the next node, +4 the previous,
// +8 the value. The head node links to itself when the list is empty.
//
// Nodes come from operator new (the C runtime heap's plain C++,
// memory/c_runtime_heap.h), so the functions that allocate run natively only
// while --native_heap routes the game's heap to the allocation layer (the
// adapters' condition, hooks::NativeHeapActive). The paths that never ran in
// play stay on the recompiled code: a function that would take one
// (AllocateNodes' bad_alloc for counts over 0x15555555, IncSize's
// length_error for lists over 0x3FFFFFFF, operator new's new handler)
// returns false (or nullopt) with nothing changed, and its adapter runs the
// game's own function from the start.

#pragma once

#include <cstdint>
#include <optional>

namespace std_list {

constexpr uint32_t kHead = 4;
constexpr uint32_t kSize = 8;
constexpr uint32_t kNext = 0;
constexpr uint32_t kPrevious = 4;
constexpr uint32_t kValue = 8;
constexpr uint32_t kNodeBytes = 12;

// Whether `count` nodes can be allocated natively (no bad_alloc path; a size
// malloc takes, with a process heap).
bool CanAllocate(uint8_t* base, uint32_t count);

// PtrList::AllocateNodes (sub_82099C08, the allocator's allocate): `count`
// x 12 bytes from operator new. nullopt where the original must run.
std::optional<uint32_t> AllocateNodes(uint8_t* base, uint32_t count);

// PtrList::IncSize (sub_820958B8, _Incsize): the size (+8) up by `count`.
// False, with nothing done, when 0x3FFFFFFF minus the size is below `count`
// (the original's length_error).
bool IncSize(uint8_t* base, uint32_t list, uint32_t count);

// PtrList::PtrList (sub_820AD058): a head node linked to itself at +4, the
// size 0. False, with nothing done, where the original must run.
bool Construct(uint8_t* base, uint32_t list);

// PtrList::Insert (sub_82095958): a node holding `value` linked in before
// the node `where`, the size up by one. False, with nothing done, where the
// original must run.
bool Insert(uint8_t* base, uint32_t list, uint32_t where, uint32_t value);

// PtrList::Erase (sub_82098E80, erase(where), docs/functions/app-states.md):
// the iterator's node (`node`, of the list `iterator_list`) unlinked and
// freed and the size down by one, unless it's `list`'s head; returns the
// next node (the result iterator's). nullopt, with nothing done, where the
// original traps (a null iterator list, or `node` its head).
std::optional<uint32_t> Erase(uint8_t* base, uint32_t list, uint32_t iterator_list, uint32_t node);

}  // namespace std_list
