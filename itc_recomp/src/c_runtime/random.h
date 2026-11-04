// The C runtime's rand (docs/functions/c-runtime.md), over the calling
// thread's guest C runtime data: the seed is the same word recompiled and
// native callers use (+20 of _getptd's block), found with the reads the
// original's _getptd makes (the user's call, 2026-11-04: _getptd and
// _mtinit stay recompiled, for the core).

#pragma once

#include <cstdint>

namespace c_runtime {

// A guest call for the per-thread data's lookup: `function` with r3 (and r4
// when `set` is 2) from the frame at `r1`, return address `lr`; its r3. The
// adapter's job (through the function table: the kernel's TLS imports at
// their thunks, GetLastError and SetLastError, the TLS slot's getter).
struct ThreadDataCall {
  uint64_t (*guest)(void* user, uint32_t function, uint32_t r1, uint32_t lr, int set,
                    uint64_t r3, uint64_t r4) = nullptr;
  void* user = nullptr;
  uint64_t Call(uint32_t function, uint32_t r1, uint32_t lr, int set, uint64_t r3,
                uint64_t r4 = 0) const {
    return guest(this->user, function, r1, lr, set, r3, r4);
  }
};

// The C runtime's per-thread data as _getptd (sub_825AA580) finds it from a
// caller whose stack pointer is `sp` (_getptd's frame and _getptd_noexit's,
// sub_825AA4C0, below it, their back chains written): the last error kept
// (GetLastError), the TLS slot [0x8266B7B8] given the getter [0x82971160]
// when empty (sub_825AA400), the getter from that slot called with the index
// [0x8266B7B4], the last error put back (SetLastError). A thread without its
// data yet (the original allocates it there; never seen in play): _getptd's
// original through `call`, from `sp` with return address `lr`.
uint32_t ThreadData(uint8_t* base, uint32_t sp, uint32_t lr, const ThreadDataCall& call);

// sub_825A9BB8 rand (96 bytes): the seed (+20 of the thread's data) =
// seed x 214013 + 2531011; returns its bits 16 to 30.
uint64_t Rand(uint8_t* base, uint32_t sp, const ThreadDataCall& call);

}  // namespace c_runtime
