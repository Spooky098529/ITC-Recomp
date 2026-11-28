// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 35's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "data_load/data_load.h"

namespace dl = data_load;

// For the fallback originals (docs/missing-functions.md): the stale
// originals' round (lifted code calling these by address ran the stale
// originals).

// Bits224::Fill and Bits480::Fill: r3 = the set (left as it is: leaves).
ITC_DIRECT_ENTRY(820CCD10) { dl::Bits224Fill(ctx.r3.u32); }
ITC_DIRECT_ENTRY(820CCD68) { dl::Bits480Fill(ctx.r3.u32); }
// Record5232::Record5232: r3 = the record; returns it.
ITC_DIRECT_ENTRY(82104C20) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = dl::Record5232Construct(ctx.r3.u32);
}
// InputRecordings::InputRecordings: r3 = the block; returns it.
ITC_DIRECT_ENTRY(821054A8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = dl::InputRecordingsConstruct(ctx.r3.u32);
}
