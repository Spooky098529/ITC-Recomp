// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 1's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "memory/heap.h"

// XPhysicalSize (MmQueryAllocationSize's wrapper): r3 = the address; r3 = the
// block's size.
ITC_DIRECT_ENTRY(822965C0) { ctx.r3.u64 = memory::PhysicalSize(ctx.r3.u32); }
