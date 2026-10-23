// The course's route natively: adapters in front of src/track_route/
// (docs/functions/track-route.md). Each leaves r3 (and f1 where the
// original leaves a value there) as the original does. Functions reached
// only from native callers have no adapters (the flag switches their
// callers): TrackRoute::StepBack (sub_820F8A58, from NormalizeBack),
// ToIndex and FromIndex (sub_820AB408, sub_820AB498, from ClampPosition;
// FromIndex's other caller, sub_820F9DD0, never ran in play and runs the
// original) and LinkDistance (sub_820F8720, from Distance and itself).

#include "guest.h"

#include "base/flags.h"
#include "track_route/track_route.h"

ITC_FLAG_BOOL(native_track_route, true, "ITC",
              "Run the course's route (docs/functions/track-route.md: LanePoint sub_820F8668, "
              "LaneAt sub_820F8920, Link sub_820F89F0, LinkLane sub_820F8CE8, SpecialSection "
              "sub_820F8D38, SliceTest sub_820F8DE8, IsMainRoad sub_820F8E98, SectionOutcode "
              "sub_820F8EF0, StepForward sub_820F8B48, BranchesMatching sub_820F8BD0, "
              "StepAlongPath sub_820F8C58, NormalizeBack sub_820F9320 with StepBack "
              "sub_820F8A58, ClampPosition sub_820F8420 with ToIndex sub_820AB408 and FromIndex "
              "sub_820AB498, Distance sub_820F9278 with LinkDistance sub_820F8720, SlicePoint "
              "sub_820F9768 with its case 0 sub_820F985C (cases 5 to 16, never seen in play, on "
              "the translated code)) natively instead of through the translated code (the same "
              "words, bit for bit; checked by --run_harnesses=track_route). Those reached only "
              "from native callers have no adapters, and the flag switches their callers: "
              "sub_820F8A58, sub_820AB408, sub_820AB498, sub_820F8720");

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
ITC_TRACK_ROUTE(820F8CE8,
                ctx.r3.s64 = LinkLane(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
ITC_TRACK_ROUTE(820F8D38, ctx.r3.s64 = SpecialSection(base, ctx.r3.u32, ctx.r4.u32))
ITC_TRACK_ROUTE(820F8DE8, ctx.r3.u64 = SliceTest(base, ctx.r3.u32, ctx.r4.u32))
ITC_TRACK_ROUTE(820F8E98, ctx.r3.u64 = IsMainRoad(base, ctx.r3.u32, ctx.r4.u32))
ITC_TRACK_ROUTE(820F8EF0, ctx.r3.u64 = SectionOutcode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// --- Steps between sections ---

ITC_TRACK_ROUTE(820F8B48, ctx.r3.u64 = StepForward(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_TRACK_ROUTE(820F8BD0, ctx.r3.u64 = BranchesMatching(base, ctx.r3.u32, ctx.r4.u32,
                                                        ctx.r5.u32, ctx.r6.u32))
ITC_TRACK_ROUTE(820F8C58, ctx.r3.u64 = StepAlongPath(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32))
ITC_TRACK_ROUTE(820F9320, ctx.r3.u64 = NormalizeBack(base, ctx.r3.u32, ctx.r4.u32))

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
