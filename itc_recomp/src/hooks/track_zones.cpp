// The track zones natively: adapters in front of src/track_zones/
// (docs/functions/track-zones.md). Each leaves r3 as the original does.
// Functions reached only from native callers have no adapters (the flag
// switches their callers): List64::Link (sub_820FE2C8), from
// List64::Insert; List128::Link (sub_8228B5A0), from List128::Insert.

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "track_zones/track_zones.h"

ITC_FLAG_BOOL(native_track_zones, true, "ITC",
              "Run the track zones (docs/functions/track-zones.md: List16::Link sub_821531C0, "
              "List64::Link sub_820FE2C8, List64::Insert sub_820FE330, List128::Link "
              "sub_8228B5A0, List128::Insert sub_8228BC18, RouteRange::Init sub_820FE058, "
              "RouteRange::AddBranch sub_820FE8C8, Zone::Reset sub_8228B540, Zone::IsInside "
              "sub_8228B4F8, ZoneRanges::Insert sub_8228BBA8, RouteRange::ToNeighbor sub_820FDF98, "
              "RouteRange::WithinEnds sub_820FE120, RouteRange::OnPath sub_820FE398, "
              "RouteRange::Test sub_820FE6F8, RouteRange::Update sub_820FE878, Zone::Update "
              "sub_8228BC80) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=track_zones). "
              "Those reached only from native callers have no adapters, and the flag switches "
              "their callers: sub_820FE2C8, sub_8228B5A0");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_TRACK_ZONES(address, ...)      \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_track_zones)) {   \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
    __VA_ARGS__;                           \
  }

using namespace track_zones;

namespace {

Position LoadPosition(uint8_t* base, uint32_t address) {
  return {guest::LoadU16(base, address), guest::LoadU16(base, address + 2)};
}

}  // namespace

// --- The fixed-size lists: r3 = the list (left), r4 = a position ---

ITC_TRACK_ZONES(821531C0, List16Link(base, ctx.r3.u32, int16_t(ctx.r4.u16)))
// r4 = &the word, r5 = the position.
ITC_TRACK_ZONES(820FE330, List64Insert(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),
                                       ctx.r5.u32))
ITC_TRACK_ZONES(8228BC18, List128Insert(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),
                                        ctx.r5.u32))

// --- The route ranges ---

// RouteRange::Init: r3 = the range (left), r4 = &(the start, the end).
ITC_TRACK_ZONES(820FE058, RangeInit(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32),
                                    LoadPosition(base, ctx.r4.u32 + 4)))
// RouteRange::AddBranch: r3 = the range, r4 = a section; r3 = the range's
// list of sections to take (+12), as List64::Insert leaves it.
ITC_TRACK_ZONES(820FE8C8, RangeAddBranch(base, ctx.r3.u32, ctx.r4.u32);
                ctx.r3.u64 = ctx.r3.u64 + 12)

// RouteRange::ToNeighbor: r3 = out (a section and a slice), r4 = a section,
// r5 = &a position; returns 1 or 0 (out written either way).
extern "C" REX_FUNC(sub_820FDF98) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_track_zones)) {
    __imp__sub_820FDF98(ctx, base);
    return;
  }
  Position out;
  const uint32_t result =
      RangeToNeighbor(base, &out, ctx.r4.u16, LoadPosition(base, ctx.r5.u32));
  guest::StoreU16(base, ctx.r3.u32, out.section);
  guest::StoreU16(base, ctx.r3.u32 + 2, out.slice);
  ctx.r3.u64 = result;
}
// RouteRange::WithinEnds, OnPath, Test and Update: r3 = a range, r4 = &a
// position; each returns its result.
ITC_TRACK_ZONES(820FE120,
                ctx.r3.u64 = RangeWithinEnds(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32)))
ITC_TRACK_ZONES(820FE398,
                ctx.r3.u64 = RangeOnPath(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32)))
ITC_TRACK_ZONES(820FE6F8,
                ctx.r3.u64 = RangeTest(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32)))
ITC_TRACK_ZONES(820FE878,
                ctx.r3.u64 = RangeUpdate(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32)))

// --- The zones ---

// Zone::Update: r3 = a zone (slot 1 of every zone's vtable, from
// TrackZones::Update). r3 = the last range's result, or with no ranges
// where memcpy left it (the copy on the original's stack, r1 - 208 + 80).
ITC_TRACK_ZONES(8228BC80, {
  const int32_t last = ZoneUpdate(base, ctx.r3.u32);
  ctx.r3.u64 = last < 0 ? ctx.r1.u64 - 208 + 80 : uint64_t(last);
})

// Zone::Reset: r3 = the zone (left), r4 = &(its index, its vehicle).
ITC_TRACK_ZONES(8228B540, ZoneReset(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),
                                    guest::LoadU32(base, ctx.r4.u32 + 4)))
// Zone::IsInside: r3 = the zone; returns its flag.
ITC_TRACK_ZONES(8228B4F8, ctx.r3.u64 = ZoneIsInside(base, ctx.r3.u32))

// ZoneRanges::Insert: r3 = the list, r4 = a range, r5 = a position; r3 =
// the copy's destination, as memcpy leaves it. A copy that can't be made
// natively goes through memcpy (sub_825A7410) from a frame like the
// original's.
extern "C" REX_FUNC(sub_8228BBA8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_track_zones)) {
    __imp__sub_8228BBA8(ctx, base);
    return;
  }
  const uint32_t range = ctx.r4.u32;
  uint32_t to = 0;
  if (!ZoneRangesInsert(base, ctx.r3.u32, range, ctx.r5.u32, &to)) {
    const uint64_t lr = ctx.lr;
    guest_call::InFrame(ctx, base, 112, 0x8228BC10, [&](uint32_t) {
      ctx.r3.u64 = to;
      ctx.r4.u64 = range;
      ctx.r5.u64 = kRangeBytes;
      sub_825A7410(ctx, base);
    });
    ctx.lr = lr;
  }
  ctx.r3.u64 = to;
}
