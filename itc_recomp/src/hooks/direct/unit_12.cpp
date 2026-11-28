// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 12's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "ai_drivers/ai_drivers.h"
#include "models/memory_blocks.h"
#include "std_string/std_string.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vehicle_accessors.h"

// --- track_collision: the wall query's parts. ---

// WallQuery::AddCell: the cell is stored at the caller's frame + 28 (the
// stack pointer on entry), as the original does.
ITC_DIRECT_ENTRY(820FC110) {
  ctx.r3.u64 = track_collision::WallQueryAddCell(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
// WallQuery::CornersThroughWall: its frame 304 bytes down; the original
// where the shape record isn't a box.
ITC_DIRECT_ENTRY(820FC3C8) {
  uint32_t hit = 0;
  if (!track_collision::WallQueryCornersThroughWall(ctx.r3.u32, ctx.r1.u32 - 304, &hit)) {
    lifted::original::c820FC3C8(ctx);
    return;
  }
  ctx.r3.u64 = hit;
}
// WallQuery::EdgesAcrossWall: its frame 944 bytes down; the original where
// the shape record isn't a box.
ITC_DIRECT_ENTRY(820FC900) {
  uint32_t hit = 0;
  if (!track_collision::WallQueryEdgesAcrossWall(ctx.r3.u32, ctx.r1.u32 - 944, &hit)) {
    lifted::original::c820FC900(ctx);
    return;
  }
  ctx.r3.u64 = hit;
}
// WallQuery::IgnoresWall: its frame 96 bytes down.
ITC_DIRECT_ENTRY(820FD6D8) {
  ctx.r3.u64 = track_collision::WallQueryIgnoresWall(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r1.u32 - 96);
}

// --- ai_drivers: the path's parts. ---

// AiPath::DropPassedSections: r3 left as the original leaves it on its
// early returns (the path).
ITC_DIRECT_ENTRY(8213BA48) { ai_drivers::DropPassedSections(ctx.r3.u32); }
// AiPath::AdvanceCorners: the original where its moves can't be native.
ITC_DIRECT_ENTRY(8213D020) {
  uint32_t r3 = 0;
  if (!ai_drivers::AdvanceCorners(ctx.r3.u32, ctx.r4.u32, &r3)) {
    lifted::original::c8213D020(ctx);
    return;
  }
  ctx.r3.u64 = r3;
}
// AiPath::Reset.
ITC_DIRECT_ENTRY(8213D438) { ctx.r3.u64 = ai_drivers::ResetPath(ctx.r3.u32); }
// AiPath::LeftEntry: its frame 160 bytes down.
ITC_DIRECT_ENTRY(8213F140) {
  ctx.r3.u64 = ai_drivers::LeftEntry(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 160);
}

// --- std_string ---

// std::allocator<char>::allocate: the original where malloc refuses.
ITC_DIRECT_ENTRY(820912B0) {
  if (const auto block = std_string::Allocate(ctx.r3.u32)) {
    ctx.r3.u64 = *block;
    return;
  }
  lifted::original::c820912B0(ctx);
}

// --- vehicle_accessors ---

// CollisionObject::SetWord264 (r3 left as the object, as the original).
ITC_DIRECT_ENTRY(820B7CC0) {
  vehicle_accessors::CollisionObjectSetWord264(ctx.r3.u32, ctx.r4.u32);
}

// --- models ---

// PendingFrees::IncSize: r3 left as the list, as the original; the original
// (its length_error) where the size would pass the limit.
ITC_DIRECT_ENTRY(8258E2F8) {
  if (!models::PendingFreesIncSize(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c8258E2F8(ctx);
  }
}
