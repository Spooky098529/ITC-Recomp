// The track zones natively: adapters in front of src/track_zones/
// (docs/functions/track-zones.md). Each leaves r3 as the original does.

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "track_zones/track_zones.h"

ITC_FLAG_BOOL(native_track_zones, true, "ITC",
              "Run the track zones (docs/functions/track-zones.md: List16::Link sub_821531C0, "
              "List64::Link sub_820FE2C8, List64::Insert sub_820FE330, List128::Link "
              "sub_8228B5A0, List128::Insert sub_8228BC18, RouteRange::Init sub_820FE058, "
              "RouteRange::AddBranch sub_820FE8C8, Zone::Reset sub_8228B540, Zone::IsInside "
              "sub_8228B4F8, ZoneRanges::Insert sub_8228BBA8) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=track_zones)");

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
ITC_TRACK_ZONES(820FE2C8, List64Link(base, ctx.r3.u32, int16_t(ctx.r4.u16)))
ITC_TRACK_ZONES(8228B5A0, List128Link(base, ctx.r3.u32, int16_t(ctx.r4.u16)))
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

// --- The zones ---

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
