// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 4's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "accessories/accessories.h"
#include "hooks/archive.h"
#include "hooks/billboards.h"
#include "hooks/heap.h"
#include "hooks/race_setup.h"
#include "hooks/tuning_pages.h"
#include "hooks/xaudio.h"
#include "hooks/xaudio_engine.h"
#include "replay_snapshots/replay_snapshots.h"
#include "screen_constructors/screen_constructors.h"

// --- The type 6 and 7 modes' screen constructors
// (src/screen_constructors/): each leaves r3 = `this` as the original does
// (GroupPair's r3 untouched; NodeScreen's the screen as read back) ---

namespace sc = screen_constructors;

// PanelE::PanelE: r3 = the panel, r4 = the group.
ITC_DIRECT_ENTRY(82230800) {
  const uint64_t self = ctx.r3.u64;
  sc::PanelE(ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(822308E8) {
  const uint64_t self = ctx.r3.u64;
  sc::Panel6F(ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(82231C68) {
  const uint64_t self = ctx.r3.u64;
  sc::PanelGroup6B(ctx.r3.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(82231CD0) {
  const uint64_t self = ctx.r3.u64;
  sc::Panel6C(ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(82231E40) {
  const uint64_t self = ctx.r3.u64;
  sc::Panel6D(ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(82240110) {
  const uint64_t self = ctx.r3.u64;
  sc::PanelGroup(ctx.r3.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(82240268) {
  const uint64_t self = ctx.r3.u64;
  sc::Panel7F(ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(82241680) {
  const uint64_t self = ctx.r3.u64;
  sc::PanelGroup7B(ctx.r3.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(822416D8) {
  const uint64_t self = ctx.r3.u64;
  sc::Panel7C(ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(822418B8) {
  const uint64_t self = ctx.r3.u64;
  sc::Panel7D(ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = self;
}

// GroupPair::GroupPair: r3 = the pair, r4 and r5 the groups (a leaf, r3 left).
ITC_DIRECT_ENTRY(82249C08) {
  sc::GroupPair(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// PanelSet::PanelSet: r3 = the set, r4 to r7 the panels F, E, D, C, r8 group
// B; its stack words from r1.
ITC_DIRECT_ENTRY(8224B660) {
  const uint64_t self = ctx.r3.u64;
  sc::PanelSet(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32,
               ctx.r1.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(8224CF58) {
  const uint64_t self = ctx.r3.u64;
  sc::MachineScreen(ctx.r3.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(82252908) {
  const uint64_t self = ctx.r3.u64;
  sc::ListPart(ctx.r3.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(822550F8) {
  const uint64_t self = ctx.r3.u64;
  sc::ColorListScreen(ctx.r3.u32);
  ctx.r3.u64 = self;
}

ITC_DIRECT_ENTRY(822570F0) {
  const uint64_t self = ctx.r3.u64;
  sc::TitledListScreen(ctx.r3.u32);
  ctx.r3.u64 = self;
}

// NodeScreen::NodeScreen: r3 = the screen, its stack words from r1.
ITC_DIRECT_ENTRY(8225A3D8) {
  ctx.r3.u64 = sc::NodeScreen(ctx.r3.u32, ctx.r1.u32);
}

// --- The replay's snapshots (src/replay_snapshots/) ---

namespace rsn = replay_snapshots;

// Member4080::Restore: r3 = the record part, r4 = the object.
ITC_DIRECT_ENTRY(82102B98) {
  ctx.r3.u64 = rsn::Member4080Restore(ctx.r3.u32, ctx.r4.u32);
}

// Member1136::Restore: r3 = the record part, r4 = the vehicle.
ITC_DIRECT_ENTRY(82102C60) {
  rsn::Member1136Restore(ctx.r3.u32, ctx.r4.u32);
}

// BodyState::Restore: r3 = the state, r4 = the rigid body.
ITC_DIRECT_ENTRY(821031A0) {
  rsn::BodyStateRestore(ctx.r3.u32, ctx.r4.u32);
}

// Record5232::RestoreTrack: r3 = the record, r4 = the vehicle.
ITC_DIRECT_ENTRY(82104310) {
  rsn::Record5232RestoreTrack(ctx.r3.u32, ctx.r4.u32);
}

// Record5232::Save: r3 = the record, r4 = the vehicle.
ITC_DIRECT_ENTRY(82104538) {
  ctx.r3.u64 = rsn::Record5232Save(ctx.r3.u32, ctx.r4.u32);
}

// Record5232::Copy: r3 = out, r4 = the record.
ITC_DIRECT_ENTRY(82104BC0) {
  ctx.r3.u64 = rsn::Record5232Copy(ctx.r3.u32, ctx.r4.u32);
}

// --- The tuning pages (their register steps, hooks/tuning_pages.h) ---

ITC_DIRECT_ENTRY(821DE160) { hooks::TuningPagesSetAeroAvailability(ctx); }
ITC_DIRECT_ENTRY(821F89B8) { hooks::TuningPagesRestoreStickerTexture(ctx); }
ITC_DIRECT_ENTRY(821FE6A0) { hooks::TuningPagesFreeTexture(ctx); }
ITC_DIRECT_ENTRY(822081A0) { hooks::TuningPagesRestoreMovieTexture(ctx); }
// Color::HsvToRgb (the tuning pages' copy): r4 = out, r5 = the color.
ITC_DIRECT_ENTRY(8224AB50) { hooks::TuningPagesHsvToRgb(ctx); }

// --- The interior accessories (src/accessories/) ---

// AccessoryFile::FreeData: r3 = the slot. The original for a block the
// allocation layer didn't give (or without the layer).
ITC_DIRECT_ENTRY(820F08D0) {
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = accessories::FileFreeData(ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c820F08D0(ctx);
}

// AccessoryEntry::New: no arguments (its frame below r1); r3 the entry or 0.
ITC_DIRECT_ENTRY(820F0920) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = accessories::EntryNew(ctx.r1.u32);
}

// AccessoryFile::Load: r3 = the file, r4 = the id, r5 = a buffer (or 0), r6
// = the callback, r7 = its context; r3 the serial.
ITC_DIRECT_ENTRY(820F0C90) {
  const int64_t serial = accessories::FileLoad(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                               ctx.r7.u32, hooks::NativeArchiveLoad());
  hooks::SetArchiveSerial(ctx, serial);
}

// Accessories::Accessories: r3 = the set; returns it.
ITC_DIRECT_ENTRY(820F0E60) {
  ctx.fpscr.disableFlushMode();  // as the original's lfs
  ctx.r3.u64 = accessories::SetConstruct(ctx.r3.u32);
}

// --- XAudio (their register steps) ---

ITC_DIRECT_ENTRY(8245B208) { hooks::XAudioStageSwap(ctx); }
ITC_DIRECT_ENTRY(82460A58) { hooks::XAudioReleaseOutputs(ctx); }
ITC_DIRECT_ENTRY(82461140) { hooks::XAudioSetFormatWords(ctx); }

// --- The light glows' billboards (its register step, hooks/billboards.h) ---

// Billboards::Draw: r3 = the set, r4 = the material; r3 = 0 or 1.
ITC_DIRECT_ENTRY(8209CDB0) { hooks::BillboardsDraw(ctx); }

// --- The race's set-up (its register step, hooks/race_setup.h) ---

ITC_DIRECT_ENTRY(820FF558) { hooks::RaceSetupLapRecordReset(ctx); }
