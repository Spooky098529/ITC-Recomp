// The C runtime's core (docs/functions/c-runtime-core.md): the wrappers of
// the game's statically linked C runtime and XAPI over kernel imports, as
// plain C++ over the same guest words the recompiled code uses (the TLS
// indexes, the per-thread data, the lock table, the stream and handle
// tables), one state for recompiled and native callers. By "B7's goal"
// (docs/loops/b7-loop.md, the user's call, 2026-11-14) they call the kernel
// imports exactly as the originals do, through Calls (guest calls the
// adapters make, src/hooks/c_runtime_core.cpp); only the imports wait for
// the core.

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>

namespace c_runtime::core {

// The globals worked out (docs/known-data.md).
constexpr uint32_t kFlsIndex = 0x8266B7B4;       // __flsindex: the data's TLS index
constexpr uint32_t kGetValueIndex = 0x8266B7B8;  // __getvalueindex: the getter's TLS index
constexpr uint32_t kFlsAlloc = 0x8297115C;       // the FLS stand-ins _mtinit sets
constexpr uint32_t kFlsGetValue = 0x82971160;
constexpr uint32_t kFlsSetValue = 0x82971164;
constexpr uint32_t kFlsFree = 0x82971168;
constexpr uint32_t kXcptActTab = 0x8266BE60;     // _XcptActTab
constexpr uint32_t kThreadDataBytes = 196;
constexpr uint32_t kLockTable = 0x8266BD40;      // 36 entries of 8 bytes
constexpr uint32_t kLockCount = 36;

enum class Import {
  kKeTlsAlloc,
  kKeTlsGetValue,
  kKeTlsSetValue,
  kKeTlsFree,
  kRtlInitializeCriticalSection,
  kRtlEnterCriticalSection,
  kRtlLeaveCriticalSection,
  kNtClose,
  kNtClearEvent,
  kRtlRaiseException,
  kKeBugCheck,
};

struct Calls {
  // A kernel import with r3 onward set to `args`; its r3.
  std::function<uint64_t(Import import, std::initializer_list<uint64_t> args)> import;
  // A guest function by its address (a function pointer the game stores:
  // an import's thunk, recompiled code or an adapter) with r3 onward set to
  // `args`; its r3.
  std::function<uint64_t(uint32_t function, std::initializer_list<uint64_t> args)> guest;
  // GetLastError and SetLastError on the calling thread (hooks/last_error.h).
  std::function<uint32_t()> get_last_error;
  std::function<void(uint32_t error)> set_last_error;
  // calloc (sub_825AC320), malloc (sub_825A9A90) and free (sub_825A9B58):
  // the native heap's plain C++ when it's on the layer, else the game's.
  std::function<uint32_t(uint32_t count, uint32_t size)> calloc;
  std::function<uint32_t(uint32_t size)> malloc;
  std::function<void(uint32_t block)> free;
  // The calling guest thread's id (sub_825B9A60).
  std::function<uint32_t()> thread_id;
};

// sub_825AA3F8 __crtTlsAlloc (probably): KeTlsAlloc (the FLS callback in r3
// ignored, as the original branches with it left in r3).
uint32_t CrtTlsAlloc(uint32_t callback, const Calls& calls);

// sub_825AA400 __set_flsgetvalue: the TLS slot [kGetValueIndex] given the
// getter [kFlsGetValue] on this thread when it reads 0; returns the value
// read, or KeTlsSetValue's result when it stored (r3 as the original leaves
// it; _getptd_noexit doesn't read it).
uint32_t SetFlsGetValue(uint8_t* base, const Calls& calls);

// sub_825AA4C0 _getptd_noexit: the calling thread's C runtime data (196
// bytes), made and set up when missing (calloc, stored through
// [kFlsSetValue]: +20 = 1, +92 = _XcptActTab, +0 the thread's id, +4 = -1;
// freed when the store fails), or 0. The last error kept around it.
uint32_t GetptdNoexit(uint8_t* base, const Calls& calls);

// sub_825AA5C0 _freefls: frees a thread's data (0: nothing) and its buffers
// at +36, +44, +52, +60, +68, +72 when set, +92 unless it's _XcptActTab.
void FreeFls(uint8_t* base, uint32_t data, const Calls& calls);

// sub_825B3260 _mtdeletelocks: frees every lock table entry's critical
// section that is set and wasn't made at boot (+4 not 1), clearing +0.
void MtDeleteLocks(uint8_t* base, const Calls& calls);

// sub_825AA448 _mtterm: __flsindex freed through [kFlsFree] and -1,
// __getvalueindex through KeTlsFree and -1 (each unless already -1), then
// _mtdeletelocks.
void MtTerm(uint8_t* base, const Calls& calls);

}  // namespace c_runtime::core
