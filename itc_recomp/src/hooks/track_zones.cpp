// The track zones natively: adapters in front of src/track_zones/
// (docs/functions/track-zones.md). Each leaves r3 as the original does.
// Functions reached only from native callers have no adapters (the flag
// switches their callers): List64::Link (sub_820FE2C8), from
// List64::Insert; List128::Link (sub_8228B5A0), from List128::Insert;
// RouteRange::ToNeighbor (sub_820FDF98), from WithinEnds and Test;
// RouteRange::WithinEnds (sub_820FE120), from OnPath; RouteRange::OnPath
// (sub_820FE398), from Test; Zone::Reset (sub_8228B540) and
// ZoneRanges::Insert (sub_8228BBA8), from the zones' set-ups.

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
              "sub_8228BC80, and the 70 zones' set-ups ZoneN::Setup sub_8228BE20 to sub_8228E6A0) "
              "natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=track_zones). "
              "Those reached only from native callers have no adapters, and the flag switches "
              "their callers: sub_820FE2C8, sub_8228B5A0, sub_820FDF98, sub_820FE120, sub_820FE398, "
              "sub_8228B540, sub_8228BBA8");

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

// RouteRange::Test and Update: r3 = a range, r4 = &a position; each
// returns its result. Their adapters stay: in attract races, Update is
// called from the camera modes (sub_821562F8, return address 0x821563C0)
// and Test from sub_820FE830 (0x820FE854), both recompiled.
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

// Zone::IsInside: r3 = the zone; returns its flag.
ITC_TRACK_ZONES(8228B4F8, ctx.r3.u64 = ZoneIsInside(base, ctx.r3.u32))

// --- The zones' set-ups ---

namespace {

struct GuestCalls {
  PPCContext* ctx;
  uint8_t* base;
};

// A copy ZoneRanges::Insert can't make natively: memcpy from the set-up's
// frame and one like Insert's.
void CopyRange(void* user, uint32_t to, uint32_t from, uint32_t frame) {
  auto& call = *static_cast<GuestCalls*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  const uint64_t stack = ctx.r1.u64;
  const uint64_t lr = ctx.lr;
  ctx.r1.u64 = frame;
  guest_call::InFrame(ctx, base, 112, 0x8228BC10, [&](uint32_t) {
    ctx.r3.u64 = to;
    ctx.r4.u64 = from;
    ctx.r5.u64 = kRangeBytes;
    sub_825A7410(ctx, base);
  });
  ctx.r1.u64 = stack;
  ctx.lr = lr;
}

}  // namespace

// ZoneN::Setup: r3 = the zone, r4 = &(its index, the vehicle); r3 = the
// last copy's destination. Reached through the zones' vtables (from
// TrackZones::Create), so the adapters stay.
#define ITC_ZONE_SETUP(address, number)                                                 \
  extern "C" REX_FUNC(sub_##address) {                                                  \
    REX_FUNC_PROLOGUE();                                                                \
    if (!ITC_FLAG(native_track_zones)) {                                                \
      __imp__sub_##address(ctx, base);                                                  \
      return;                                                                           \
    }                                                                                   \
    GuestCalls user{&ctx, base};                                                        \
    const SetupCalls calls{&user, CopyRange};                                           \
    ctx.r3.u64 = ZoneSetup(base, number, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),  \
                           guest::LoadU32(base, ctx.r4.u32 + 4), ctx.r1.u32, calls);    \
  }

ITC_ZONE_SETUP(8228BE20, 0)
ITC_ZONE_SETUP(8228BE88, 1)
ITC_ZONE_SETUP(8228BEF0, 2)
ITC_ZONE_SETUP(8228BF58, 3)
ITC_ZONE_SETUP(8228BFC0, 4)
ITC_ZONE_SETUP(8228C028, 5)
ITC_ZONE_SETUP(8228C090, 6)
ITC_ZONE_SETUP(8228C0F8, 7)
ITC_ZONE_SETUP(8228C160, 8)
ITC_ZONE_SETUP(8228C1C8, 9)
ITC_ZONE_SETUP(8228C230, 10)
ITC_ZONE_SETUP(8228C298, 11)
ITC_ZONE_SETUP(8228C300, 12)
ITC_ZONE_SETUP(8228C368, 13)
ITC_ZONE_SETUP(8228C3D0, 14)
ITC_ZONE_SETUP(8228C438, 15)
ITC_ZONE_SETUP(8228C4A0, 16)
ITC_ZONE_SETUP(8228C508, 17)
ITC_ZONE_SETUP(8228C570, 18)
ITC_ZONE_SETUP(8228C5D8, 19)
ITC_ZONE_SETUP(8228C640, 20)
ITC_ZONE_SETUP(8228C6A8, 21)
ITC_ZONE_SETUP(8228C710, 22)
ITC_ZONE_SETUP(8228C778, 23)
ITC_ZONE_SETUP(8228C7E0, 24)
ITC_ZONE_SETUP(8228C848, 25)
ITC_ZONE_SETUP(8228C8B0, 26)
ITC_ZONE_SETUP(8228C918, 27)
ITC_ZONE_SETUP(8228C980, 28)
ITC_ZONE_SETUP(8228C9E8, 29)
ITC_ZONE_SETUP(8228CA50, 30)
ITC_ZONE_SETUP(8228CAB8, 31)
ITC_ZONE_SETUP(8228CB20, 32)
ITC_ZONE_SETUP(8228CBC8, 33)
ITC_ZONE_SETUP(8228CC70, 34)
ITC_ZONE_SETUP(8228CD18, 35)
ITC_ZONE_SETUP(8228CD80, 36)
ITC_ZONE_SETUP(8228CE20, 37)
ITC_ZONE_SETUP(8228CFC8, 38)
ITC_ZONE_SETUP(8228D070, 39)
ITC_ZONE_SETUP(8228D220, 40)
ITC_ZONE_SETUP(8228D2C8, 41)
ITC_ZONE_SETUP(8228D370, 42)
ITC_ZONE_SETUP(8228D418, 43)
ITC_ZONE_SETUP(8228D5C0, 44)
ITC_ZONE_SETUP(8228D668, 45)
ITC_ZONE_SETUP(8228D710, 46)
ITC_ZONE_SETUP(8228D778, 47)
ITC_ZONE_SETUP(8228D870, 48)
ITC_ZONE_SETUP(8228D968, 49)
ITC_ZONE_SETUP(8228DA08, 50)
ITC_ZONE_SETUP(8228DBA8, 51)
ITC_ZONE_SETUP(8228DC60, 52)
ITC_ZONE_SETUP(8228DD00, 53)
ITC_ZONE_SETUP(8228DDE8, 54)
ITC_ZONE_SETUP(8228DE50, 55)
ITC_ZONE_SETUP(8228DEB0, 56)
ITC_ZONE_SETUP(8228DF58, 57)
ITC_ZONE_SETUP(8228E000, 58)
ITC_ZONE_SETUP(8228E068, 59)
ITC_ZONE_SETUP(8228E0D0, 60)
ITC_ZONE_SETUP(8228E190, 61)
ITC_ZONE_SETUP(8228E238, 62)
ITC_ZONE_SETUP(8228E2E8, 63)
ITC_ZONE_SETUP(8228E390, 64)
ITC_ZONE_SETUP(8228E3F8, 65)
ITC_ZONE_SETUP(8228E498, 66)
ITC_ZONE_SETUP(8228E550, 67)
ITC_ZONE_SETUP(8228E5F0, 68)
ITC_ZONE_SETUP(8228E6A0, 69)
