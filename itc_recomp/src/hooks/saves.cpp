// The save slots' steps on our own saves/ folder: thin adapters for the
// recompiled callers in front of their plain C++ (src/saves/slot_steps.h,
// B4: no flag). See B4 in docs/rexglue-removal.md and
// docs/functions/files-and-saves.md.

#include "guest.h"
#include "hooks/native_entry.h"

#include "saves/slot_steps.h"

namespace {

// A step's answer in r3, sign-extended as the original's li.
void Answer(lifted::Regs& ctx, int32_t r3) {
  ctx.r3.s64 = r3;
}

}  // namespace

// SaveSlot::CheckDevice: r3 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A45C0) {
  
  ctx.r3.u64 = saves::slot::CheckDevice(ctx.r3.u32);
}

// SaveSlot::WaitDeviceSelector: r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A48B8) {
  
  Answer(ctx, saves::slot::WaitDeviceSelector(ctx.r4.u32));
}

// SaveSlot::ShowDeviceSelector: r3 = owner, r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A4960) {
  
  Answer(ctx, saves::slot::ShowDeviceSelector(ctx.r4.u32));
}

// SaveSlot::Enumerate: r3 = owner, r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A49D0) {
  
  Answer(ctx, saves::slot::Enumerate(ctx.r3.u32, ctx.r4.u32));
}

// SaveSlot::WaitEnumerate: r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A4A88) {
  
  Answer(ctx, saves::slot::WaitEnumerate(ctx.r4.u32));
}

// SaveSlot::OpenItem: r3 = owner, r4 = slot, r5 = the item's index.
ITC_NATIVE_ENTRY_ALWAYS(820A4B18) {
  
  Answer(ctx, saves::slot::OpenItem(ctx.r4.u32, ctx.r5.u32));
}

// SaveSlot::WaitOpenItem: r3 = owner, r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A4B98) {
  
  Answer(ctx, saves::slot::WaitOpenItem(ctx.r4.u32));
}

// SaveSlot::Create: r3 = owner, r4 = slot, r5 = mode.
ITC_NATIVE_ENTRY_ALWAYS(820A4C58) {
  
  Answer(ctx, saves::slot::Create(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32));
}

// SaveSlot::WriteFile: r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A4D68) {
  
  Answer(ctx, saves::slot::WriteFile(ctx.r4.u32));
}

// SaveSlot::ReadFile: r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A4EF8) {
  
  Answer(ctx, saves::slot::ReadFile(ctx.r4.u32));
}

// SaveSlot::SetThumbnail: r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A5110) {
  
  Answer(ctx, saves::slot::SetThumbnail(ctx.r4.u32));
}

// SaveSlot::Delete: r4 = slot.
ITC_NATIVE_ENTRY_ALWAYS(820A51E0) {
  
  Answer(ctx, saves::slot::Delete(ctx.r4.u32));
}

// XContentClose: r3 = the content's root name, r4 = overlapped. Called
// inline by the save and load steps (sub_820A5BE8, sub_820A5E68).
ITC_NATIVE_ENTRY_ALWAYS(82295BF0) {
  
  ctx.r3.u64 = saves::slot::ContentClose(ctx.r4.u32);
}
