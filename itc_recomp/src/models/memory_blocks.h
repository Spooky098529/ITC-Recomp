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

// sub_8258E410 Memory::FreeLater: under the lock, a block (not null, its
// header starting `XLMM`) put at the end of the pending frees. False, with
// nothing done, where the original must run.
bool FreeLater(uint8_t* base, uint32_t block);

// sub_8258E498 Memory::FreePhysicalLater: as FreeLater, with `attributes`
// at the header's +12 first.
bool FreePhysicalLater(uint8_t* base, uint32_t block, uint32_t attributes);

}  // namespace models
