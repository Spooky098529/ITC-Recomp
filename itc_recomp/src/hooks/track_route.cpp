// The course's route natively: adapters in front of src/track_route/
// (docs/functions/track-route.md). Each leaves r3 (and f1 where the
// original leaves a value there) as the original does. Functions reached
// only from native callers have no adapters (the flag switches their
// callers): TrackRoute::StepBack (sub_820F8A58, from NormalizeBack),
// ToIndex and FromIndex (sub_820AB408, sub_820AB498, from ClampPosition;
// FromIndex's other caller, sub_820F9DD0, never ran in play and runs the
// original) and LinkDistance (sub_820F8720, from Distance and itself);
// and, their recompiled callers having been made native, LinkLane
// (sub_820F8CE8, from StepForwardLane and StepForwardLaneMasked),
// BranchesMatching (sub_820F8BD0, from StepForwardMasked and
// StepForwardLaneMasked), StepForwardMasked (sub_820F93A0, from
// NormalizeForward), StepForward (sub_820F8B48, from StepForwardMasked,
// NormalizeForward and NormalizeForwardPath), StepAlongPath (sub_820F8C58,
// from NormalizeForwardPath), NormalizeForwardPath (sub_820F9F58, from
// NormalizePath), NormalizeForward (sub_820F9EA8, from Normalize and
// NormalizeForwardPath), NormalizeBack (sub_820F9320, from Normalize,
// NormalizePath, NormalizeLane and NormalizeLaneMasked), StepForwardLane
// (sub_820F9448, from NormalizeLane), StepForwardLaneMasked (sub_820F9600,
// from NormalizeLaneMasked), AlongSlice (sub_820FAE78, from Locate and
// LocateShoulder), Locate (sub_820FAF88, from OnSlice) and SectionOutcode
// (sub_820F8EF0, from OnSlice and Follow's full search). NoteShoulder
// (sub_820FB100), TakeBranch (sub_820FA4A8) and the full search
// (sub_820FB2B0, never seen in play) are reached only from Follow; and,
// their callers having been made native, IsMainRoad (sub_820F8E98, from
// ToMainRoad), LocateShoulder (sub_820FB030, from NoteShoulder and
// LocateAt), Follow (sub_820FB680, from TrackPosition::Update) and Point
// (sub_820FA860, from TrackPosition::Init; both in src/track_collision/,
// --native_collision_world, which calls them whatever this flag says).

#include "guest.h"

#include "base/flags.h"
#include "track_route/track_route.h"

ITC_FLAG_BOOL(native_track_route, true, "ITC",
              "Run the course's route (docs/functions/track-route.md: LanePoint sub_820F8668, "
              "LaneAt sub_820F8920, Link sub_820F89F0, LinkLane sub_820F8CE8, SpecialSection "
              "sub_820F8D38, SliceTest sub_820F8DE8, IsMainRoad sub_820F8E98, SectionOutcode "
              "sub_820F8EF0, StepForward sub_820F8B48, BranchesMatching sub_820F8BD0, "
              "StepAlongPath sub_820F8C58, NormalizeBack sub_820F9320 with StepBack "
              "sub_820F8A58, StepForwardMasked sub_820F93A0, NormalizeForward sub_820F9EA8, "
              "StepForwardLane sub_820F9448, StepForwardLaneMasked sub_820F9600, Normalize "
              "sub_820F9FE8, NormalizeForwardPath sub_820F9F58, NormalizePath sub_820FA0F8, "
              "NormalizeLane sub_820FA210, NormalizeLaneMasked sub_820FA350, Point "
              "sub_820FA860, AlongSlice sub_820FAE78, Locate sub_820FAF88, LocateShoulder "
              "sub_820FB030, LocateAt sub_820FB888, ToMainRoad sub_820FA728, OnSlice "
              "sub_820FB1B8, Follow sub_820FB680 with NoteShoulder sub_820FB100, TakeBranch "
              "sub_820FA4A8 and its full search sub_820FB2B0, ClampPosition sub_820F8420 with ToIndex sub_820AB408 and FromIndex "
              "sub_820AB498, Distance sub_820F9278 with LinkDistance sub_820F8720, PathDistance sub_820FA9D0, SlicePoint "
              "sub_820F9768 with its case 0 sub_820F985C (cases 5 to 16, never seen in play, on "
              "the translated code)) natively instead of through the translated code (the same "
              "words, bit for bit; checked by --run_harnesses=track_route). Those reached only "
              "from native callers have no adapters, and the flag switches their callers: "
              "sub_820F8A58, sub_820AB408, sub_820AB498, sub_820F8720, sub_820F8CE8, "
              "sub_820F8BD0, sub_820F93A0, sub_820F9EA8, sub_820F9F58, sub_820F8B48, sub_820F8C58, "
              "sub_820F9320, sub_820F9448, sub_820F9600, sub_820FAE78, sub_820FAF88, sub_820F8EF0, "
              "sub_820FB100, sub_820FA4A8, sub_820FB2B0, sub_820F8E98, sub_820FB030 (and through "
              "--native_collision_world's TrackPosition::Update and Init: sub_820FB680, "
              "sub_820FA860)");

// Defines an adapter: `body` uses `ctx` and `base`.
#define ITC_TRACK_ROUTE(address, body)   \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_track_route)) { \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    ctx.fpscr.disableFlushMode();        \
    body;                                \
  }

using namespace track_route;

// --- Lookups ---

// r3 = the route, r4 = out, r5 = the position, r6 = the lane (r3 kept).
ITC_TRACK_ROUTE(820F8668, LanePoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32))
ITC_TRACK_ROUTE(820F8920, ctx.r3.u64 = LaneAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_TRACK_ROUTE(820F89F0, ctx.r3.s64 = Link(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_TRACK_ROUTE(820F8D38, ctx.r3.s64 = SpecialSection(base, ctx.r3.u32, ctx.r4.u32))
ITC_TRACK_ROUTE(820F8DE8, ctx.r3.u64 = SliceTest(base, ctx.r3.u32, ctx.r4.u32))

// --- Steps between sections ---

// r5 = the masks or 0.
ITC_TRACK_ROUTE(820F9FE8, ctx.r3.u64 = Normalize(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r5 = the path or 0, r6 = the masks or 0.
ITC_TRACK_ROUTE(820FA0F8, ctx.r3.u64 = NormalizePath(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32))
// r5 = the lane (a halfword, changed).
ITC_TRACK_ROUTE(820FA210, ctx.r3.u64 = NormalizeLane(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r6 = the masks.
ITC_TRACK_ROUTE(820FA350, ctx.r3.u64 = NormalizeLaneMasked(base, ctx.r3.u32, ctx.r4.u32,
                                                           ctx.r5.u32, ctx.r6.u32))

// --- Points and slices ---

// r4 = the point, r5 = the position (a tail call: LocateShoulder's frame
// 112 bytes down).
ITC_TRACK_ROUTE(820FB888, ctx.r3.u64 = LocateAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                ctx.r1.u32 - 112))
// r4 = the position (changed), r5 = the flag; the copy in the original's
// frame (144 bytes down).
ITC_TRACK_ROUTE(820FA728, ctx.r3.u64 = ToMainRoad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                  ctx.r1.u32 - 144))
// r4 = the point, r5 = the position, r6 = the score or 0; the original's
// frame (128 bytes down).
ITC_TRACK_ROUTE(820FB1B8, ctx.r3.u64 = OnSlice(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r6.u32, ctx.r1.u32 - 128))

// --- The linear index ---

// r3 = out, r4 = the packed position; r3 the out (left as it came).
ITC_TRACK_ROUTE(820F8420, ClampPosition(base, ctx.r3.u32, ctx.r4.u32))

// --- Distances ---

// f1 the distance; r3 LinkDistance's answer for positions in different
// sections, else the route (left as it came).
ITC_TRACK_ROUTE(820F9278, bool linked = false;
                const bool apart = guest::LoadU16(base, ctx.r4.u32) !=
                                   guest::LoadU16(base, ctx.r5.u32);
                ctx.f1.f64 = Distance(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &linked);
                if (apart) { ctx.r3.u64 = linked ? 1 : 0; })

// r4, r5 = the positions (packed), r6 = the path, r7 = its length; f1 the
// distance; the original's frame (288 bytes down). r3 left as it came (no
// caller reads it).
ITC_TRACK_ROUTE(820FA9D0, ctx.f1.f64 = PathDistance(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                    ctx.r6.u32, ctx.r7.s32, ctx.r1.u32 - 288))

// --- Points of a slice ---

// r3 = out, r4 = which, r5 = the section, r6 = the slice (r3 kept). Points
// 5 to 16 (sub_820F8FA0, never seen in play): the original.
extern "C" REX_FUNC(sub_820F9768) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_track_route)) {
    ctx.fpscr.disableFlushMode();
    if (SlicePoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32)) {
      return;
    }
  }
  __imp__sub_820F9768(ctx, base);
}
