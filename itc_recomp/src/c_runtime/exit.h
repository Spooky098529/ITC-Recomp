// The C runtime's exit table, natively: atexit (sub_825A7068) and _onexit
// (sub_825A6F60) over the game's own table in guest memory (begin at
// 0x82978334, end at 0x82978330: one word a registered function), grown
// with the native _msize and realloc (memory/c_runtime_heap.h), and the exit
// lock (the C runtime's lock 8, _lockexit and _unlockexit) as a host mutex.
// See docs/functions/c-runtime.md.
//
// The exit lock: lock 8 of the C runtime's lock table (0x8266BD40, eight
// bytes a lock: the critical section, then 1 for one made at boot), a kernel
// critical section. In its place, one host mutex (the user's call for a
// subsystem's critical section, 2026-10-24 and 2026-10-25: Edram::Alloc, the
// memory blocks' lock), taken by every user: native _onexit directly, and
// the recompiled ones (the exit path's doexit sub_825AA218 and its unlocks
// sub_825AA318, sub_825AA338) through the _lock and _unlock adapters
// (sub_825B3430, sub_825B32D0, src/hooks/c_runtime.cpp) for lock 8 only.
// Recursive, as a critical section is: the exit path holds it while it calls
// the registered functions, which may call atexit.

#pragma once

#include <cstdint>
#include <mutex>
#include <optional>

namespace c_runtime {

constexpr uint32_t kExitLock = 8;                 // the lock number
constexpr uint32_t kLockTable = 0x8266BD40;       // the C runtime's locks
constexpr uint32_t kExitTableBegin = 0x82978334;  // the table's first word
constexpr uint32_t kExitTableEnd = 0x82978330;    // one past its last

// The host mutex in place of lock 8's critical section.
std::recursive_mutex& ExitMutex();

// sub_825B3430 _lock and sub_825B32D0 _unlock, for lock 8: the host mutex
// taken and given back. Each returns lock 8's critical section (its table
// entry's word), the r3 the originals leave (their Rtl*CriticalSection
// argument).
uint32_t LockExit(uint8_t* base);
uint32_t UnlockExit(uint8_t* base);

// sub_825A6F60 _onexit: with the exit lock held, `function` stored at the
// table's end, the table grown first when its block (_msize) has no room for
// one more word: realloc to its size plus the smaller of its size and 2,048
// bytes, or failing that its size plus 16. Returns `function`, or 0 when the
// table can't take it (end below begin, or both reallocs failing). nullopt,
// with nothing changed, when the native _msize or realloc would take an
// error path (a null table, a size the layer refuses): the caller runs the
// original from the start, which takes it.
std::optional<uint32_t> OnExit(uint8_t* base, uint32_t function);

// sub_825A7068 atexit: _onexit, -1 when it gives 0, else 0; nullopt as
// _onexit's.
std::optional<int32_t> Atexit(uint8_t* base, uint32_t function);

}  // namespace c_runtime
