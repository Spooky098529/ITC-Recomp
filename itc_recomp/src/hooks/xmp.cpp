// XMP (the player's own music on the 360), dropped: the boot movie took the
// playback from the player's music at its start and handed it back at its
// end, through XMP's playback controller (sub_82589408, sub_82589360), which
// are no longer called. Xbox-only: see B5 in docs/rexglue-removal.md.

#include "guest.h"
#include "last_error.h"

// Takes the playback (from Boot::StartMovie, which ignores the answer). The
// SDK answered 0 and left the last error cleared.
extern "C" REX_FUNC(sub_825894C8) {
  REX_FUNC_PROLOGUE();
  last_error::Set(base, ctx.r13.u32, 0);
  ctx.r3.u64 = 0;
}

// Hands it back (from Boot::EndMovie, which returns the answer); the same.
extern "C" REX_FUNC(sub_825895A0) {
  REX_FUNC_PROLOGUE();
  last_error::Set(base, ctx.r13.u32, 0);
  ctx.r3.u64 = 0;
}
