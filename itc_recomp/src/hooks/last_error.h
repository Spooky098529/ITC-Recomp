// The game's last error (Win32 SetLastError), kept per thread in guest
// memory: `[[PCR+256]+352]`, the PCR's current thread's +352, left alone
// while `[PCR+336]` is set (probably: in a DPC). r13 holds the PCR. Used by
// the overrides in src/hooks/ in place of guest calls to the game's
// SetLastError. See B6 in docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace last_error {

// SetLastError (sub_82296E70, sub_82296FD0).
void Set(uint8_t* base, uint32_t pcr, uint32_t error);

// SetLastError from an NTSTATUS (sub_82296E88); returns the DOS error. The
// mapping is the SDK's (xeRtlNtStatusToDosError) until the import's last
// recompiled caller, sub_822953E8 (probably a file I/O completion routine,
// B4), is native: then it needs a table of our own.
uint32_t SetFromStatus(uint8_t* base, uint32_t pcr, uint32_t status);

}  // namespace last_error
