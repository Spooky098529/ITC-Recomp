// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 2's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "guest.h"
#include "lifted/originals.h"

#include "data_load/data_load.h"
#include "input/input_manager.h"
#include "models/memory_blocks.h"
#include "models/texture_provider.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

// --- The UI movies ---

// The pairs' capacity change (sub_8211B160): r3 = the vector, r4 = the new
// capacity. Where its allocation or free isn't native, the original.
ITC_DIRECT_ENTRY(8211B160) {
  if (const auto r3 = ui_movies::PairsReserve(ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *r3;
  } else {
    lifted::original::c8211B160(ctx);
  }
}
// DrawContext::Copy: r3 = the new context, r4 = the parent's; returns r3.
ITC_DIRECT_ENTRY(8259D9F8) {
  ctx.r3.u64 = ui_movies::CopyDrawContext(ctx.r3.u32, ctx.r4.u32);
}
// TextItem::CopyQueued: r3 = the destination, r4 = the text item; returns
// r3.
ITC_DIRECT_ENTRY(8259E620) {
  ctx.r3.u64 = ui_movies::CopyQueuedText(ctx.r3.u32, ctx.r4.u32);
}

// --- The data load's records ---

// Member1136::Member1136 and Member4080::Member4080: r3 = the member,
// returned (left unchanged).
ITC_DIRECT_ENTRY(82102888) { ctx.r3.u64 = data_load::Member1136Construct(ctx.r3.u32); }
ITC_DIRECT_ENTRY(82104690) { ctx.r3.u64 = data_load::Member4080Construct(ctx.r3.u32); }

// --- The models' memory blocks and texture provider ---

// PendingFrees::Insert: r3 = the list, r4 = the node to insert before, r5 =
// the value's address. A list at its size limit or no process heap: the
// original from the start. r3 left as it is (the original leaves
// IncSize's).
ITC_DIRECT_ENTRY(8258E398) {
  if (!models::PendingFreesInsert(ctx.r3.u32, ctx.r4.u32, guest::LoadU32(ctx.r5.u32))) {
    lifted::original::c8258E398(ctx);
  }
}
// TextureProvider::UninitializedCopy: r3 to r4 copied to r5; returns the
// destination's end.
ITC_DIRECT_ENTRY(82594380) {
  ctx.r3.u64 = models::PointersUninitializedCopy(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// --- The input processors ---

// InputProcessorBase::InputProcessorBase: r3 = the processor, returned.
ITC_DIRECT_ENTRY(825920D0) { input::InputProcessorBaseConstruct(ctx.r3.u32); }
