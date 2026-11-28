// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 7's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "app_start_up/start_up.h"
#include "car_makers/car_makers.h"
#include "guest.h"
#include "hooks/app_start_up.h"
#include "hooks/race_states.h"
#include "hooks/race_world.h"
#include "hooks/xact_tracks.h"
#include "hooks/xact_waves.h"
#include "track_zones/track_zones.h"
#include "xact/xact.h"

// --- XACT ---

// XACTVariable::Init: r3 = the variable (left), r4 = the engine, r5 = the
// cue (or 0), r6 = the index, r7 = its record, r8 (+240).
ITC_DIRECT_ENTRY(825C1DE0) {
  ctx.fpscr.disableFlushMode();
  xact::VariableInit(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32);
}
// XACTVariable::XACTVariable: r3 = the variable; r3 = it.
ITC_DIRECT_ENTRY(825C20B0) {
  const uint64_t variable = ctx.r3.u64;
  xact::VariableConstruct(ctx.r3.u32);
  ctx.r3.u64 = variable;
}
// XACTWaveEntry::FindXmaBlock: r3 = the entry, r4 = the sample position,
// r5 = the seek table, r6 = the block offset's word, r7 = the samples
// into it's word (r3 left).
ITC_DIRECT_ENTRY(825CEF50) {
  xact::WaveEntryFindXmaBlock(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}
ITC_DIRECT_ENTRY(825D2030) { hooks::XactWaveNotify(ctx); }
ITC_DIRECT_ENTRY(825D5520) { hooks::XactTrackInstanceCancelEvents(ctx); }
ITC_DIRECT_ENTRY(825D5A48) { hooks::XactTrackInstancePrepare(ctx); }

// --- The app's start-up ---

// Tiling::LargestTile: r3 = the tiling (left), r4 = the two words out.
ITC_DIRECT_ENTRY(8209ADD8) { app_start_up::TilingLargestTile(ctx.r3.u32, ctx.r4.u32); }
// Font::Font: r3 = the font; r3 = 14.
ITC_DIRECT_ENTRY(82109228) { hooks::FontConstructSteps(ctx); }
// AudioEngine::StartXAudio: r3 (the engine) not read.
ITC_DIRECT_ENTRY(8259CD50) { hooks::StartXAudioSteps(ctx); }

// --- The race world and the race's states ---

ITC_DIRECT_ENTRY(82143508) { hooks::RaceWorldRaceEventEntry(ctx); }
ITC_DIRECT_ENTRY(82144A38) { hooks::RaceWorldPlaceStartsCurrent(ctx); }
ITC_DIRECT_ENTRY(82170CB8) { hooks::RaceWorldBuildNeighbors(ctx); }
ITC_DIRECT_ENTRY(82171070) { hooks::RaceWorldPickModel(ctx); }
ITC_DIRECT_ENTRY(82162E70) { hooks::RaceStatesSectionsBefore(ctx); }
ITC_DIRECT_ENTRY(82163000) { hooks::RaceStatesSectionsJoin(ctx); }

// --- The car makers and the track zones ---

// CarRecord::ResetBytes: r3 = the bytes (left).
ITC_DIRECT_ENTRY(820D0900) { car_makers::ResetBytes(ctx.r3.u32); }
// List128::Link: r3 = the list (left), r4 = the position (16 bits).
ITC_DIRECT_ENTRY(8228B5A0) { track_zones::List128Link(ctx.r3.u32, int16_t(ctx.r4.u16)); }
