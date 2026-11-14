// The save slots' steps on our own saves/ folder: thin adapters for the
// recompiled callers in front of their plain C++ (src/saves/slot_steps.h,
// B4: no flag). See B4 in docs/rexglue-removal.md and
// docs/functions/files-and-saves.md.

#include "guest.h"

#include "saves/slot_steps.h"

namespace {

// A step's answer in r3, sign-extended as the original's li.
void Answer(PPCContext& ctx, int32_t r3) {
  ctx.r3.s64 = r3;
}

}  // namespace

// SaveSlot::CheckDevice: r3 = slot.
extern "C" REX_FUNC(sub_820A45C0) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = saves::slot::CheckDevice(base, ctx.r3.u32);
}

// SaveSlot::WaitDeviceSelector: r4 = slot.
extern "C" REX_FUNC(sub_820A48B8) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::WaitDeviceSelector(base, ctx.r4.u32));
}

// SaveSlot::ShowDeviceSelector: r3 = owner, r4 = slot.
extern "C" REX_FUNC(sub_820A4960) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::ShowDeviceSelector(base, ctx.r4.u32));
}

// SaveSlot::Enumerate: r3 = owner, r4 = slot.
extern "C" REX_FUNC(sub_820A49D0) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::Enumerate(base, ctx.r3.u32, ctx.r4.u32));
}

// SaveSlot::WaitEnumerate: r4 = slot.
extern "C" REX_FUNC(sub_820A4A88) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::WaitEnumerate(base, ctx.r4.u32));
}

// SaveSlot::OpenItem: r3 = owner, r4 = slot, r5 = the item's index.
extern "C" REX_FUNC(sub_820A4B18) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::OpenItem(base, ctx.r4.u32, ctx.r5.u32));
}

// SaveSlot::WaitOpenItem: r3 = owner, r4 = slot.
extern "C" REX_FUNC(sub_820A4B98) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::WaitOpenItem(base, ctx.r4.u32));
}

// SaveSlot::Create: r3 = owner, r4 = slot, r5 = mode.
extern "C" REX_FUNC(sub_820A4C58) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::Create(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32));
}

// SaveSlot::WriteFile: r4 = slot.
extern "C" REX_FUNC(sub_820A4D68) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::WriteFile(base, ctx.r4.u32));
}

// SaveSlot::ReadFile: r4 = slot.
extern "C" REX_FUNC(sub_820A4EF8) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::ReadFile(base, ctx.r4.u32));
}

// SaveSlot::SetThumbnail: r4 = slot.
extern "C" REX_FUNC(sub_820A5110) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::SetThumbnail(base, ctx.r4.u32));
}

// SaveSlot::Delete: r4 = slot.
extern "C" REX_FUNC(sub_820A51E0) {
  REX_FUNC_PROLOGUE();
  Answer(ctx, saves::slot::Delete(base, ctx.r4.u32));
}

// XContentClose: r3 = the content's root name, r4 = overlapped. Called
// inline by the save and load steps (sub_820A5BE8, sub_820A5E68).
extern "C" REX_FUNC(sub_82295BF0) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = saves::slot::ContentClose(base, ctx.r4.u32);
}
