// The graphics library's memory blocks' put-off frees as plain C++
// (docs/functions/models.md: Memory::FreeLater, FreePhysicalLater and the
// pending frees' std::list; the layouts in docs/known-data.md). A block from
// sub_8258DFF0 or sub_8258E138 has a 4 KiB header before it, starting `XLMM`;
// freeing it puts the header at the end of the pending frees (the std::list
// at 0x8296BD54: +4 its head node, +8 its size; a node is 12 bytes, +0 the
// next, +4 the previous, +8 the header), which sub_8258E1D8 frees at the
// pass's start (FrameStart::OnTick).
//
// The lock: the game guards the list (and the blocks' allocations) with the
// critical section at 0x8296BD38, taken through ScopedLock (sub_8258E580,
// sub_8258E5B8). In its place, one host mutex (the user's call, 2026-10-25,
// as Edram::Alloc got): the native functions take it directly, and the
// ScopedLock adapters (src/hooks/models.cpp) take it for that critical
// section, so the recompiled users (sub_8258DFF0, sub_8258E098,
// sub_8258E138, sub_8258E1D8) take the same lock. A recursive mutex, as the
// critical section it replaces is (no user in play takes it twice, but the
// harness holds it around its cases). Other critical sections keep the
// game's ScopedLock.
//
// Nodes come from operator new (the C runtime heap's plain C++,
// memory/c_runtime_heap.h), so the functions that allocate run natively only
// while --native_heap routes the game's heap to the allocation layer (the
// adapters' condition). The paths that never ran in play stay on the
// recompiled code: a function that would take one (IncSize's length_error
// for lists over 0x3FFFFFFF, operator new's new handler) returns false with
// nothing changed, and its adapter runs the game's own function from the
// start. Adapters: src/hooks/models.cpp.

#pragma once

#include <cstdint>
#include <mutex>

namespace models {

constexpr uint32_t kMemoryBlocksLock = 0x8296BD38;  // the critical section
constexpr uint32_t kPendingFrees = 0x8296BD54;      // the std::list
constexpr uint32_t kBlockHeaderBytes = 4096;
constexpr uint32_t kBlockTag = 0x584C4D4D;  // 'XLMM'
constexpr uint32_t kBlockAttributes = 12;   // the header's XMemFree attributes

// The list's and nodes' layout (as std_list/ptr_list.h's).
constexpr uint32_t kListHead = 4;
constexpr uint32_t kListSize = 8;
constexpr uint32_t kNodeNext = 0;
constexpr uint32_t kNodePrevious = 4;
constexpr uint32_t kNodeValue = 8;
constexpr uint32_t kNodeBytes = 12;

// The host mutex in place of the critical section at 0x8296BD38.
std::recursive_mutex& MemoryBlocksMutex();

// sub_8258E580 ScopedLock::ScopedLock, for the critical section at
// 0x8296BD38: the lock word (+0) = the critical section, then the mutex
// taken.
void ScopedLockEnter(uint8_t* base, uint32_t lock, uint32_t critical_section);
// sub_8258E5B8 ScopedLock::~ScopedLock, for a lock word holding 0x8296BD38:
// the mutex given back. Returns the word (the original's r3 before its
// LeaveCriticalSection).
uint32_t ScopedLockLeave(uint8_t* base, uint32_t lock);

// sub_8258E2F8 PendingFrees::IncSize (std::list's _Incsize): the size (+8)
// up by `count`. False, with nothing done, when 0x3FFFFFFF minus the size is
// below `count` (the original's length_error).
bool PendingFreesIncSize(uint8_t* base, uint32_t list, uint32_t count);

// Whether PendingFreesInsert can run natively on `list` (a node allocatable,
// the size below the limit).
bool PendingFreesCanInsert(uint8_t* base, uint32_t list);

// sub_8258E398 PendingFrees::Insert (std::list's _Insert): a node holding
// `value` linked in before the node `where`, the size up by one. False, with
// nothing done, where the original must run.
bool PendingFreesInsert(uint8_t* base, uint32_t list, uint32_t where, uint32_t value);

// For native callers checking before they put blocks off: whether the
// pending frees take `count` more nodes natively (a process heap, the size
// limit), read under the lock.
bool PendingFreesHaveRoom(uint8_t* base, uint32_t count);

// sub_8258E410 Memory::FreeLater: under the lock, a block (not null, its
// header starting `XLMM`) put at the end of the pending frees. False, with
// nothing done, where the original must run.
bool FreeLater(uint8_t* base, uint32_t block);

// sub_8258E498 Memory::FreePhysicalLater: as FreeLater, with `attributes`
// at the header's +12 first.
bool FreePhysicalLater(uint8_t* base, uint32_t block, uint32_t attributes);

// The memory blocks' makers (docs/functions/shader-objects.md): under the
// lock, the block's pages, `bytes` + the header rounded up to 4 KiB
// ((bytes + 8191) & ~0xFFF), from the allocation layer; the header's +0 =
// `XLMM`, +4 = the kind, +8 = 1 (the passes until it's freed once put off),
// +12 = 0. They return the block (the header + 4096), or 0 when the
// allocation gives 0. Only over the allocation layer (--native_heap), as the
// adapters check.
//
// sub_8258DFF0 Memory::AllocVirtual: kind 0, VirtualAlloc(0, size,
// MEM_COMMIT | MEM_NOZERO, PAGE_READWRITE) (the layer's pages).
uint32_t AllocVirtual(uint8_t* base, uint32_t bytes);
// sub_8258E098 Memory::AllocPhysicalEx: kind 1, XPhysicalAllocEx(size,
// `low`, `high`, `alignment`, `protection`).
uint32_t AllocPhysicalEx(uint8_t* base, uint32_t bytes, uint32_t low, uint32_t high,
                         uint32_t alignment, uint32_t protection);
// sub_8258E138 Memory::AllocPhysical (probably): kind 2, XMemAlloc(size,
// `attributes`).
uint32_t AllocPhysical(uint8_t* base, uint32_t bytes, uint32_t attributes);

// sub_8258E1D8 PendingFrees::FreeDue: under the lock, the pending frees
// walked from the first node; a header whose +8 is 0 has its +0 cleared and
// is freed by its kind (+4: 0 VirtualFree, 1 XPhysicalFree, 2 XMemFree with
// its +12 attributes, 3 and above nothing), and its node unlinked and
// deleted (the size down by 1); any other header's +8 goes down by 1. False,
// with nothing done, where the original must run: a due header the layer
// can't free natively (kind 0 not the layer's pages, kind 2 not physical and
// not a heap block), a node not a heap block, or a header queued twice.
bool FreeDue(uint8_t* base);

}  // namespace models
