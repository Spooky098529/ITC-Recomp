// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 48's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "hooks/load_poll.h"
#include "load_poll/load_poll.h"

// --- The scene models' load poll (src/hooks/load_poll.cpp's register
// steps; each runs the original unless its callees are native) ---

// sub_820D3658: r3 = the index, r4 = the value; r3 the table entry.
ITC_DIRECT_ENTRY(820D3658) { ctx.r3.u64 = load_poll::GaugeTableEntry(ctx.r3.u64, ctx.r4.s32); }
ITC_DIRECT_ENTRY(820D7E18) { hooks::LoadPollTrafficCarSetPaint(ctx); }
ITC_DIRECT_ENTRY(820D8ED0) { hooks::LoadPollWheelsSetCaliperTexture(ctx); }
ITC_DIRECT_ENTRY(820D91E0) { hooks::LoadPollWheelsApplyPaint(ctx); }
ITC_DIRECT_ENTRY(820D9638) { hooks::LoadPollWheelsBindTextures(ctx); }
ITC_DIRECT_ENTRY(820D9B68) { hooks::LoadPollSceneModelApplyPaint(ctx); }
ITC_DIRECT_ENTRY(820D9C50) { hooks::LoadPollSceneModelApplyWheelPaint(ctx); }
ITC_DIRECT_ENTRY(820DC138) { hooks::LoadPollSceneCockpitApplyPaint(ctx); }
ITC_DIRECT_ENTRY(820DCB70) { hooks::LoadPollSceneLightsSetRecord(ctx); }
ITC_DIRECT_ENTRY(820DF250) { hooks::LoadPollScenePartsSetSelectedTexture(ctx); }
ITC_DIRECT_ENTRY(820E0908) { hooks::LoadPollScenePartsApplyPaint(ctx); }
ITC_DIRECT_ENTRY(820E3040) { hooks::LoadPollInteriorDialsSetBlock(ctx); }
ITC_DIRECT_ENTRY(820E32E8) { hooks::LoadPollInteriorMetersSetGauge(ctx); }
ITC_DIRECT_ENTRY(820E4250) { hooks::LoadPollSceneInteriorApplyPaint(ctx); }
ITC_DIRECT_ENTRY(820E61D0) { hooks::LoadPollSceneTrafficApplyPaint(ctx); }
ITC_DIRECT_ENTRY(820E8A00) { hooks::LoadPollSceneWheelsApplyPaint(ctx); }
