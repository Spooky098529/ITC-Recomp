// GlobalMemoryStatus (sub_82294DA0), dropped: its callers (sub_82106548,
// sub_82106618 and sub_82107910) pass it a buffer in their frame and never
// read it back, so it writes nothing (the SDK's MmQueryStatistics isn't
// called). See B6 in docs/rexglue-removal.md.

#include "guest.h"

// r3 = the MEMORYSTATUS (out), unread.
extern "C" REX_FUNC(sub_82294DA0) {
  REX_FUNC_PROLOGUE();
}
