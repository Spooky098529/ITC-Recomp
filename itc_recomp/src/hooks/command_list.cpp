// The graphics library's command list natively: adapters in front of
// src/command_list/command_list.cpp (docs/functions/command-list.md).
// AllocateSlots's adapter is in src/interpolation/blend_state.cpp. Where the
// original would print its debug message (a nested chunk, a full list),
// the adapters run the original instead (never seen in play).

#include "guest.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "command_list/command_list.h"

ITC_FLAG_BOOL(native_command_list, true, "ITC",
              "Run the graphics library's command list functions (CommandList's constructor "
              "sub_8258C960, Flip sub_8258C9C0, BeginTick sub_8258C998, OpenChunk "
              "sub_8258C9D0, CloseChunk sub_8258CA50, ChunkSlots sub_8258CA88, "
              "AllocateVertices sub_8258CB08, AllocateBytes sub_8258CB98, AllocateResolve "
              "sub_8258CC30) natively instead of through the translated code (same results; "
              "checked by --run_harnesses=command_list)");

// AllocateSlots's diagnostics (blend_state.cpp) see the header slots the
// originals allocate through it: with any of them on, the originals run.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

namespace {
bool SlotDiagnostics() {
  return ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
         ITC_FLAG(log_recording_by_pass);
}
}  // namespace

// CommandList::CommandList: r3 = list.
ITC_NATIVE_ENTRY(8258C960, ITC_FLAG(native_command_list)) {
  command_list::Construct(ctx.r3.u32);
}

// CommandList::Flip: r3 = list.
ITC_NATIVE_ENTRY(8258C9C0, ITC_FLAG(native_command_list)) {
  command_list::Flip(ctx.r3.u32);
}

// CommandList::BeginTick: r3 = list.
ITC_NATIVE_ENTRY(8258C998, ITC_FLAG(native_command_list)) {
  command_list::BeginTick(ctx.r3.u32);
}

// CommandList::OpenChunk: r3 = list.
ITC_NATIVE_ENTRY_ALWAYS(8258C9D0) {
  
  if (!ITC_FLAG(native_command_list) ||
      command_list::OpenChunk(ctx.r3.u32) == command_list::OpenResult::kNested) {
    lifted::original::c8258C9D0(ctx);
  }
}

// CommandList::CloseChunk: r3 = list.
ITC_NATIVE_ENTRY(8258CA50, ITC_FLAG(native_command_list)) {
  command_list::CloseChunk(ctx.r3.u32);
}

// CommandList::ChunkSlots: r3 = list; returns the last chunk's slots.
ITC_NATIVE_ENTRY(8258CA88, ITC_FLAG(native_command_list)) {
  ctx.r3.s64 = command_list::ChunkSlots(ctx.r3.u32);
}

// CommandList::AllocateVertices: r3 = list, r4 = vertex count, r5 (byte) =
// with a header; returns the vertices' first slot.
ITC_NATIVE_ENTRY_ALWAYS(8258CB08) {
  
  const bool header = (ctx.r5.u32 & 0xFF) != 0;
  uint32_t first = 0;
  if (ITC_FLAG(native_command_list) && !(header && SlotDiagnostics())) {
    first = command_list::AllocateVertices(ctx.r3.u32, ctx.r4.u32, header);
  }
  if (first == 0) {
    lifted::original::c8258CB08(ctx);
    return;
  }
  ctx.r3.u64 = first;
}

// CommandList::AllocateBytes: r3 = list, r4 = bytes, r5 (byte) = with a
// header; returns the first slot.
ITC_NATIVE_ENTRY_ALWAYS(8258CB98) {
  
  const bool header = (ctx.r5.u32 & 0xFF) != 0;
  uint32_t first = 0;
  if (ITC_FLAG(native_command_list) && !(header && SlotDiagnostics())) {
    first = command_list::AllocateBytes(ctx.r3.u32, ctx.r4.u32, header);
  }
  if (first == 0) {
    lifted::original::c8258CB98(ctx);
    return;
  }
  ctx.r3.u64 = first;
}

// CommandList::AllocateResolve: r3 = list, r4 (byte) = with a header; returns
// the resolve item's first slot.
ITC_NATIVE_ENTRY_ALWAYS(8258CC30) {
  
  const bool header = (ctx.r4.u32 & 0xFF) != 0;
  uint32_t first = 0;
  if (ITC_FLAG(native_command_list) && !(header && SlotDiagnostics())) {
    first = command_list::AllocateResolve(ctx.r3.u32, header);
  }
  if (first == 0) {
    lifted::original::c8258CC30(ctx);
    return;
  }
  ctx.r3.u64 = first;
}
