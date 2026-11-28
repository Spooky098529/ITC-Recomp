// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 31's shims that need nothing private to an area's hooks file.

#include "base/flags.h"
#include "hooks/native_entry.h"
#include "hooks/second_set.h"
#include "lifted/originals.h"
#include "native/buffers.h"
#include "second_set/blocks.h"
#include "second_set/finds.h"

ITC_FLAG_DECLARE(bool, native_material_passes);

namespace {

namespace ss = second_set;

// A part's or the interior's point set-up (finds.h): r3 = the group, r4 =
// the part model; r3 as the original leaves it, the flush mode as its last
// float step; the original where the set-ups aren't native.
void FindEntry(lifted::Regs& ctx, lifted::RegsFunction* original,
               hooks::FindFunction find) {
  if (!hooks::SecondSetFindsNative()) {
    original(ctx);
    return;
  }
  hooks::SecondSetFindStep(ctx, find);
}

// A parts group's init with no step (blocks.h): r3 = the group, r3 = the
// group as the plain C++ returns it; the original where memcpy isn't native
// (as the point groups' GroupStep in hooks/second_set.cpp).
void InitEntry(lifted::Regs& ctx, lifted::RegsFunction* original,
               uint32_t (*init)(uint32_t, uint32_t)) {
  if (!native::Buffers_NativeMemcpy()) {
    original(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = init(ctx.r3.u32, ctx.r1.u32);
}

}  // namespace

// The point groups' resets and inits (their register steps).
ITC_DIRECT_ENTRY(820DE600) { hooks::SecondSetPartsResetLeftDoor(ctx); }
ITC_DIRECT_ENTRY(820DE7E8) { hooks::SecondSetPartsResetRightDoor(ctx); }
ITC_DIRECT_ENTRY(820DE9D0) { hooks::SecondSetPartsResetDoors(ctx); }
ITC_DIRECT_ENTRY(820DEBF0) { hooks::SecondSetPartsResetTailLights(ctx); }
ITC_DIRECT_ENTRY(820DECD0) { hooks::SecondSetPartsResetFenders(ctx); }
ITC_DIRECT_ENTRY(820DEE88) { hooks::SecondSetPartsResetCabin(ctx); }
ITC_DIRECT_ENTRY(820DF020) { hooks::SecondSetPartsResetEngine(ctx); }
ITC_DIRECT_ENTRY(820DF0C0) { hooks::SecondSetPartsResetMufflers(ctx); }
ITC_DIRECT_ENTRY(820DF128) { hooks::SecondSetPartsResetShadows(ctx); }
ITC_DIRECT_ENTRY(820DF410) { hooks::SecondSetPartsInitCooling(ctx); }
ITC_DIRECT_ENTRY(820DF570) { hooks::SecondSetPartsInitSideSkirts(ctx); }
ITC_DIRECT_ENTRY(820DF668) { hooks::SecondSetPartsInitLeftMirror(ctx); }
ITC_DIRECT_ENTRY(820DF6B0) { hooks::SecondSetPartsInitRightMirror(ctx); }
ITC_DIRECT_ENTRY(820DF6F8) { hooks::SecondSetPartsInitMirror(ctx); }
ITC_DIRECT_ENTRY(820DF740) { hooks::SecondSetPartsResetHeadlights(ctx); }
ITC_DIRECT_ENTRY(820E2FC8) { hooks::SecondSetInteriorResetDials(ctx); }
ITC_DIRECT_ENTRY(820E3200) { hooks::SecondSetInteriorResetMeters(ctx); }
ITC_DIRECT_ENTRY(820E33F0) { hooks::SecondSetInteriorResetSteering(ctx); }
ITC_DIRECT_ENTRY(820E3440) { hooks::SecondSetInteriorResetSeats(ctx); }
ITC_DIRECT_ENTRY(820E34F8) { hooks::SecondSetInteriorResetRollCage(ctx); }

// The parts block's group inits (no steps: only PartsPoints::PartsPoints
// called them).
ITC_DIRECT_ENTRY(820DF5B8) { InitEntry(ctx, lifted::original::c820DF5B8, ss::PartsInitLeftDoor); }
// InitRightDoor runs ResetRightDoor, natively only with
// --native_material_passes (as its step and PartsPoints::PartsPoints'):
ITC_DIRECT_ENTRY(820DF610) {
  if (!ITC_FLAG(native_material_passes)) {
    lifted::original::c820DF610(ctx);
    return;
  }
  InitEntry(ctx, lifted::original::c820DF610, ss::PartsInitRightDoor);
}
ITC_DIRECT_ENTRY(820E1388) { InitEntry(ctx, lifted::original::c820E1388, ss::PartsInitHeadlights); }
ITC_DIRECT_ENTRY(820E0010) { InitEntry(ctx, lifted::original::c820E0010, ss::PartsInitMufflers); }

// The point set-ups (finds.h).
ITC_DIRECT_ENTRY(820DE668) { FindEntry(ctx, lifted::original::c820DE668, ss::PartsFindLeftDoor); }
ITC_DIRECT_ENTRY(820DE878) { FindEntry(ctx, lifted::original::c820DE878, ss::PartsFindRightDoor); }
ITC_DIRECT_ENTRY(820DEA20) { FindEntry(ctx, lifted::original::c820DEA20, ss::PartsFindDoors); }
ITC_DIRECT_ENTRY(820DEC58) { FindEntry(ctx, lifted::original::c820DEC58, ss::PartsFindTailLights); }
ITC_DIRECT_ENTRY(820DED20) { FindEntry(ctx, lifted::original::c820DED20, ss::PartsFindFenders); }
ITC_DIRECT_ENTRY(820DEED8) { FindEntry(ctx, lifted::original::c820DEED8, ss::PartsFindCabin); }
ITC_DIRECT_ENTRY(820DF7F0) { FindEntry(ctx, lifted::original::c820DF7F0, ss::PartsFindHeadlights); }
ITC_DIRECT_ENTRY(820E0080) { FindEntry(ctx, lifted::original::c820E0080, ss::PartsFindMufflers); }
ITC_DIRECT_ENTRY(820E30C0) { FindEntry(ctx, lifted::original::c820E30C0, ss::InteriorFindDials); }
ITC_DIRECT_ENTRY(820E3298) { FindEntry(ctx, lifted::original::c820E3298, ss::InteriorFindMeters); }
ITC_DIRECT_ENTRY(820E34A8) { FindEntry(ctx, lifted::original::c820E34A8, ss::InteriorFindSeats); }

// The parts' block and object (their register steps).
ITC_DIRECT_ENTRY(820E2A10) { hooks::SecondSetPartsPointsConstruct(ctx); }
ITC_DIRECT_ENTRY(820E2C10) { hooks::SecondSetPartsPointsDestruct(ctx); }
ITC_DIRECT_ENTRY(820E0510) { hooks::SecondSetScenePartsReset(ctx); }
ITC_DIRECT_ENTRY(820E0590) { hooks::SecondSetScenePartsRelease(ctx); }
ITC_DIRECT_ENTRY(820E2E10) { hooks::SecondSetScenePartsDestruct(ctx); }
