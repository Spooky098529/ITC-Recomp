// The course's route (src/track_route/, docs/functions/track-route.md)
// against the recompiled functions. Every fourth case builds a random route
// in a block from the allocation layer: the route (+0: its file), the file
// (+0x40: the sections' count, 1 to 8, the slice length, the table), 8 sections
// (+0x100, 1,000 bytes apart), each 1 to 8 slices with random records,
// bounds, links, lane bytes and point tables (NaN, infinite and zero floats
// among them), then the inputs (+0x2800: positions, a point, a path,
// words). The table is a block of its own with all 65,536 entries (entry i
// the section i modulo 8), so every section number a link or a position
// names is a section. Both versions run on a copy of the same bytes; the
// block up to +0x2900 (the route by its hash, base::Hash64, the inputs in
// 64-byte pieces), r3, an output at +0x2900 (floats) and f1 (a double, for
// Distance) are compared. The functions
// reading the course's route (ToIndex, FromIndex, ClampPosition,
// SlicePoint) get the file in [0x826C5B10] for each case, put back after.
// The originals run with --native_track_route off, so the whole chain is
// the recompiled code's.

#include "harness.h"

#include "base/flags.h"
#include "base/hash.h"
#include "base/log.h"
#include "memory/heap.h"
#include "track_route/track_route.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_track_route);

namespace harness {

namespace {

namespace tr = track_route;

// SlicePoint's cases the plain C++ handed to the original (none expected:
// the cases leave out points 5 to 16).
int g_fallbacks = 0;

// --- The register steps, as the adapters (src/hooks/track_route.cpp) ---

#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeLanePoint,
                tr::LanePoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32))
ITC_NATIVE_STEP(NativeLaneAt, ctx.r3.u64 = tr::LaneAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeLink, ctx.r3.s64 = tr::Link(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeLinkLane, ctx.r3.s64 = tr::LinkLane(base, ctx.r3.u32, ctx.r4.u32,
                                                          ctx.r5.u32, ctx.r6.u32))
ITC_NATIVE_STEP(NativeSpecialSection,
                ctx.r3.s64 = tr::SpecialSection(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSliceTest, ctx.r3.u64 = tr::SliceTest(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeIsMainRoad, ctx.r3.u64 = tr::IsMainRoad(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSectionOutcode,
                ctx.r3.u64 = tr::SectionOutcode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeStepForward,
                ctx.r3.u64 = tr::StepForward(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeBranchesMatching,
                ctx.r3.u64 = tr::BranchesMatching(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                  ctx.r6.u32))
ITC_NATIVE_STEP(NativeStepAlongPath,
                ctx.r3.u64 = tr::StepAlongPath(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r6.u32))
ITC_NATIVE_STEP(NativeStepForwardMasked,
                ctx.r3.u64 = tr::StepForwardMasked(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r6.u32))
ITC_NATIVE_STEP(NativeNormalizeForward,
                ctx.r3.u64 = tr::NormalizeForward(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeStepForwardLane,
                ctx.r3.u64 = tr::StepForwardLane(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                 ctx.r6.u32))
ITC_NATIVE_STEP(NativeStepForwardLaneMasked,
                ctx.r3.u64 = tr::StepForwardLaneMasked(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                       ctx.r6.u32, ctx.r7.u32))
ITC_NATIVE_STEP(NativeStepBack, ctx.r3.u64 = tr::StepBack(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeNormalizeBack,
                ctx.r3.u64 = tr::NormalizeBack(base, ctx.r3.u32, ctx.r4.u32))
// r3 sign-extended, as the original's 64-bit sum of signed halfwords.
ITC_NATIVE_STEP(NativeToIndex, ctx.r3.s64 = int32_t(tr::ToIndex(base, ctx.r3.u32)))
ITC_NATIVE_STEP(NativeFromIndex, ctx.r3.u64 = tr::FromIndex(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClampPosition, tr::ClampPosition(base, ctx.r3.u32, ctx.r4.u32))
void NativeDistance(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  bool linked = false;
  const bool apart = guest::LoadU16(base, ctx.r4.u32) != guest::LoadU16(base, ctx.r5.u32);
  ctx.f1.f64 = tr::Distance(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &linked);
  if (apart) {
    ctx.r3.u64 = linked ? 1 : 0;
  }
}
// LinkDistance on its own: r6 = a guest float, written when found (as the
// original does), r7 = the depth.
void NativeLinkDistance(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  float distance = guest::LoadFloat(base, ctx.r6.u32);
  if (tr::LinkDistance(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, distance, ctx.r7.s32)) {
    guest::StoreFloat(base, ctx.r6.u32, distance);
    ctx.r3.u64 = 1;
  } else {
    ctx.r3.u64 = 0;
  }
}
void NativeSlicePoint(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!tr::SlicePoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32)) {
    g_fallbacks++;
    __imp__sub_820F9768(ctx, base);
  }
}

// Runs the recompiled version with the native flag off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  const bool saved = ITC_FLAG(native_track_route);
  ITC_FLAG(native_track_route) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_track_route) = saved;
}

// The block's layout.
constexpr uint32_t kAreaBytes = 0x3000;
constexpr uint32_t kFile = 0x40, kSections = 0x100, kSectionStride = 1000;
constexpr uint32_t kTableEntries = 0x10000;
constexpr uint32_t kIo = 0x2800;
constexpr uint32_t kA = kIo + 0, kB = kIo + 4, kLinkIndex = kIo + 8, kLane = kIo + 10,
                   kCount = kIo + 12, kMask = kIo + 16, kFloat = kIo + 20, kPath = kIo + 0x40,
                   kPoint = kIo + 0x80, kBranches = kIo + 0xC0, kMasks = kIo + 0x24;
constexpr uint32_t kCompared = 0x2900, kOut = 0x2900;

// A float: now and then NaN (any sign, payload), an infinity or a zero.
uint32_t RandomFloat(Random& r, float range) {
  switch (r.Below(32)) {
    case 0:
      return 0x7F800001u | (uint32_t(r.Next()) & 0x803FFFFFu) | 0x00400000u;
    case 1:
      return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 2:
      return r.Below(2) ? 0u : 0x80000000u;
    default: {
      const float unit = float(r.Below(1u << 24)) / float(1u << 24);
      return std::bit_cast<uint32_t>((unit * 2.0f - 1.0f) * range);
    }
  }
}

struct Route {
  uint32_t sections = 0;  // the file's count
  uint32_t slices[8] = {};
  uint32_t address[8] = {};
  uint32_t Slices(uint32_t section) const { return slices[section % 8]; }
  uint32_t Address(uint32_t section) const { return address[section % 8]; }
};

// The table (see the top), filled once.
void FillTable(uint8_t* base, uint32_t table, uint32_t area) {
  for (uint32_t i = 0; i < kTableEntries; i++) {
    guest::StoreU32(base, table + 4 * i, area + kSections + kSectionStride * (i % 8));
  }
}

// A random route in the block (see the top).
Route BuildRoute(uint8_t* base, uint32_t area, uint32_t table, Random& r) {
  Route route;
  route.sections = 1 + r.Below(8);
  guest::StoreU32(base, area + 0, area + kFile);
  guest::StoreU32(base, area + kFile + 0, route.sections);
  guest::StoreU32(base, area + kFile + 4, uint32_t(r.Next()));
  guest::StoreU32(base, area + kFile + 8,
                  r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                   : std::bit_cast<uint32_t>(0.5f + float(r.Below(80)) / 8.0f));
  guest::StoreU32(base, area + kFile + 12, table);
  for (uint32_t s = 0; s < 8; s++) {
    const uint32_t at = area + kSections + kSectionStride * s;
    const uint32_t n = 1 + r.Below(8);
    route.slices[s] = n;
    route.address[s] = at;
    guest::StoreU16(base, at + 0, uint16_t(n));
    // The kind: 0 to 5 mostly (the low four bits), high bits random.
    guest::StoreU8(base, at + 2, uint8_t((r.Below(8) << 4) | (r.Below(4) ? r.Below(6) : r.Below(16))));
    // The branch number: 7, 8, 13 and 29 often, now and then above 31.
    const uint32_t branches[] = {7, 8, 13, 29};
    guest::StoreU8(base, at + 3, uint8_t(r.Below(3) == 0 ? branches[r.Below(4)]
                                         : r.Below(8) == 0 ? r.Below(256)
                                                           : r.Below(32)));
    for (uint32_t i = 0; i < 6; i++) {
      guest::StoreU32(base, at + 4 + 4 * i, RandomFloat(r, 200.0f));
    }
    // The links: -1 a third of the time, else a section (now and then
    // beyond the count, or above 0x8000) and a slice.
    for (uint32_t i = 0; i < 12; i++) {
      const bool none = r.Below(3) == 0;
      guest::StoreU16(base, at + 28 + 4 * i,
                      none ? 0xFFFF
                           : uint16_t(r.Below(4)    ? r.Below(route.sections)
                                      : r.Below(2) ? r.Below(16)
                                                   : r.Below(0xFFFF)));
      guest::StoreU16(base, at + 30 + 4 * i, uint16_t(int32_t(r.Below(12)) - 2));
    }
    for (uint32_t i = 0; i < 48; i++) {
      guest::StoreU8(base, at + 76 + i, uint8_t(r.Below(4) == 0 ? 0xFF : r.Below(8)));
    }
    for (uint32_t k = 0; k < n; k++) {
      const uint32_t record = at + 124 + 84 * k;
      for (uint32_t i = 0; i < 84; i += 4) {
        guest::StoreU32(base, record + i, RandomFloat(r, 100.0f));
      }
      guest::StoreU32(base, record + 12,
                      r.Below(16) == 0 ? RandomFloat(r, 10.0f)
                                       : std::bit_cast<uint32_t>(1.0f + float(r.Below(40)) / 4.0f));
      guest::StoreU16(base, record + 32, uint16_t(r.Below(3)));
      guest::StoreU16(base, record + 34, uint16_t(r.Next()));
      guest::StoreU8(base, record + 56, uint8_t((r.Below(16) << 4) | (r.Below(4) ? r.Below(5)
                                                                                 : r.Below(16))));
      const uint8_t kinds[] = {0, 3, 4};
      guest::StoreU8(base, record + 57, uint8_t(r.Below(4) ? kinds[r.Below(3)] : r.Below(256)));
    }
    for (uint32_t i = 0; i < 24 * n; i += 4) {
      guest::StoreU32(base, at + 124 + 84 * n + i, RandomFloat(r, 100.0f));
    }
  }
  return route;
}

// A position: a section of the route (now and then one of the table's
// others) and a slice in it, now and then a few past either end.
void RandomPosition(uint8_t* base, uint32_t address, const Route& route, Random& r,
                    uint32_t sections_up_to = 0) {
  const uint32_t section = sections_up_to != 0 ? r.Below(sections_up_to)
                           : r.Below(8) == 0    ? r.Below(kTableEntries)
                                                : r.Below(route.sections);
  const int32_t n = int32_t(route.Slices(section));
  const int32_t slice = r.Below(4) == 0 ? int32_t(r.Below(uint32_t(n) + 6)) - 3
                                        : int32_t(r.Below(uint32_t(n)));
  guest::StoreU16(base, address + 0, uint16_t(section));
  guest::StoreU16(base, address + 2, uint16_t(slice));
}

enum Kind {
  kLanePoint,
  kLaneAt,
  kLink,
  kLinkLane,
  kSpecialSection,
  kSliceTest,
  kIsMainRoad,
  kSectionOutcode,
  kStepForward,
  kBranchesMatching,
  kStepAlongPath,
  kStepForwardMasked,
  kNormalizeForward,
  kStepForwardLane,
  kStepForwardLaneMasked,
  kStepBack,
  kNormalizeBack,
  kToIndex,
  kFromIndex,
  kClampPosition,
  kDistance,
  kLinkDistance,
  kSlicePoint,
};

}  // namespace

void TrackRoute(Context& h) {
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
    int cases;
  } functions[] = {
      {"TrackRoute::LanePoint (sub_820F8668)", Original<__imp__sub_820F8668>, NativeLanePoint,
       kLanePoint, 2000},
      {"TrackRoute::LaneAt (sub_820F8920)", Original<__imp__sub_820F8920>, NativeLaneAt, kLaneAt,
       3000},
      {"TrackRoute::Link (sub_820F89F0)", Original<__imp__sub_820F89F0>, NativeLink, kLink, 2000},
      {"TrackRoute::LinkLane (sub_820F8CE8)", Original<__imp__sub_820F8CE8>, NativeLinkLane,
       kLinkLane, 2000},
      {"TrackRoute::SpecialSection (sub_820F8D38)", Original<__imp__sub_820F8D38>,
       NativeSpecialSection, kSpecialSection, 2000},
      {"TrackRoute::SliceTest (sub_820F8DE8)", Original<__imp__sub_820F8DE8>, NativeSliceTest,
       kSliceTest, 2000},
      {"TrackRoute::IsMainRoad (sub_820F8E98)", Original<__imp__sub_820F8E98>, NativeIsMainRoad,
       kIsMainRoad, 2000},
      {"TrackRoute::SectionOutcode (sub_820F8EF0)", Original<__imp__sub_820F8EF0>,
       NativeSectionOutcode, kSectionOutcode, 3000},
      {"TrackRoute::StepForward (sub_820F8B48)", Original<__imp__sub_820F8B48>,
       NativeStepForward, kStepForward, 2000},
      {"TrackRoute::BranchesMatching (sub_820F8BD0)", Original<__imp__sub_820F8BD0>,
       NativeBranchesMatching, kBranchesMatching, 2000},
      {"TrackRoute::StepAlongPath (sub_820F8C58)", Original<__imp__sub_820F8C58>,
       NativeStepAlongPath, kStepAlongPath, 2000},
      {"TrackRoute::StepForwardMasked (sub_820F93A0)", Original<__imp__sub_820F93A0>,
       NativeStepForwardMasked, kStepForwardMasked, 2000},
      {"TrackRoute::NormalizeForward (sub_820F9EA8)", Original<__imp__sub_820F9EA8>,
       NativeNormalizeForward, kNormalizeForward, 2000},
      {"TrackRoute::StepForwardLane (sub_820F9448)", Original<__imp__sub_820F9448>,
       NativeStepForwardLane, kStepForwardLane, 2000},
      {"TrackRoute::StepForwardLaneMasked (sub_820F9600)", Original<__imp__sub_820F9600>,
       NativeStepForwardLaneMasked, kStepForwardLaneMasked, 2000},
      {"TrackRoute::StepBack (sub_820F8A58)", Original<__imp__sub_820F8A58>, NativeStepBack,
       kStepBack, 2000},
      {"TrackRoute::NormalizeBack (sub_820F9320)", Original<__imp__sub_820F9320>,
       NativeNormalizeBack, kNormalizeBack, 2000},
      {"TrackRoute::ToIndex (sub_820AB408)", Original<__imp__sub_820AB408>, NativeToIndex,
       kToIndex, 2000},
      {"TrackRoute::FromIndex (sub_820AB498)", Original<__imp__sub_820AB498>, NativeFromIndex,
       kFromIndex, 2000},
      {"TrackRoute::ClampPosition (sub_820F8420)", Original<__imp__sub_820F8420>,
       NativeClampPosition, kClampPosition, 2000},
      {"TrackRoute::Distance (sub_820F9278)", Original<__imp__sub_820F9278>, NativeDistance,
       kDistance, 3000},
      {"TrackRoute::LinkDistance (sub_820F8720)", Original<__imp__sub_820F8720>,
       NativeLinkDistance, kLinkDistance, 3000},
      {"TrackRoute::SlicePoint (sub_820F9768)", Original<__imp__sub_820F9768>, NativeSlicePoint,
       kSlicePoint, 2000},
  };
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  const uint32_t table = memory::Alloc(4 * kTableEntries, 0);
  FillTable(h.base, table, area);
  const uint32_t saved_route = guest::LoadU32(h.base, tr::kRoute);
  Random r;
  for (const auto& f : functions) {
    Results results(f.name);
    Route route;
    std::vector<uint8_t> built;
    for (int c = 0; c < f.cases; c++) {
      // The case's bytes and registers, the same for both versions: a new
      // route every fourth case (the inputs differ in each).
      if (c % 4 == 0) {
        std::memset(h.base + area, 0xCD, kAreaBytes);
        route = BuildRoute(h.base, area, table, r);
        built = Read(h.base, area, kIo);
      } else {
        Write(h.base, area, built.data(), kIo);
        std::memset(h.base + area + kIo, 0xCD, kAreaBytes - kIo);
      }
      RandomPosition(h.base, area + kA, route, r);
      RandomPosition(h.base, area + kB, route, r);
      if (r.Below(4) == 0) {  // the same section now and then
        guest::StoreU16(h.base, area + kB, guest::LoadU16(h.base, area + kA));
      }
      guest::StoreU16(h.base, area + kLinkIndex, uint16_t(r.Below(12)));
      guest::StoreU16(h.base, area + kLane, uint16_t(r.Below(8)));
      guest::StoreU32(h.base, area + kCount, 1 + r.Below(12));
      guest::StoreU32(h.base, area + kMask,
                      r.Below(4) == 0 ? 0xDFFFFE7Fu : uint32_t(r.Next()));
      guest::StoreU32(h.base, area + kFloat, RandomFloat(r, 50.0f));
      // The masks: 0 to 4 words (none zero), then 0.
      const uint32_t masks = r.Below(5);
      for (uint32_t i = 0; i < masks; i++) {
        const uint32_t word = r.Below(3) == 0 ? 0xDFFFFE7Fu : uint32_t(r.Next());
        guest::StoreU32(h.base, area + kMasks + 4 * i, word != 0 ? word : 1u);
      }
      guest::StoreU32(h.base, area + kMasks + 4 * masks, 0);
      const uint32_t path_length = r.Below(6);
      for (uint32_t i = 0; i < path_length; i++) {
        guest::StoreU16(h.base, area + kPath + 2 * i, uint16_t(r.Below(route.sections + 2)));
      }
      guest::StoreU16(h.base, area + kPath + 2 * path_length, uint16_t(0x8000 | r.Below(0x8000)));
      // The point: near a section's bounds.
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(h.base, area + kPoint + 4 * i, RandomFloat(r, 250.0f));
      }
      const uint32_t junk[4] = {uint32_t(r.Next()), uint32_t(r.Next()), uint32_t(r.Next()),
                                uint32_t(r.Next())};
      uint64_t r3 = area, r4 = 0, r5 = 0, r6 = 0, r7 = 0;
      bool global = false;
      std::string what = fmt::format("case {} ({} sections)", c, route.sections);
      switch (f.kind) {
        case kLanePoint:
          r4 = area + kOut;
          r5 = area + kA;
          r6 = uint64_t(int64_t(int32_t(r.Below(20)) - 3));
          break;
        case kLaneAt:
        case kSectionOutcode: {
          // The point near the position's slice, or anywhere.
          const uint32_t section = guest::LoadU16(h.base, area + kA);
          const uint32_t record =
              route.Address(section) + 124 + 84 * r.Below(route.Slices(section));
          if (r.Below(2)) {
            for (uint32_t i = 0; i < 3; i++) {
              const float c0 = guest::LoadFloat(h.base, record + 4 * i);
              const float spread = float(r.Below(400)) / 10.0f - 20.0f;
              guest::StoreFloat(h.base, area + kPoint + 4 * i, c0 + spread);
            }
          }
          r4 = area + kPoint;
          r5 = f.kind == kLaneAt ? area + kA
                                 : (junk[0] & 0xFFFF0000u) | r.Below(kTableEntries);
          break;
        }
        case kLink:
          r4 = (junk[0] & 0xFFFF0000u) | r.Below(kTableEntries);
          r5 = (junk[1] & 0xFFFF0000u) | r.Below(12);
          break;
        case kLinkLane:
          r4 = area + kA;
          r5 = area + kLinkIndex;
          r6 = area + kLane;
          break;
        case kSpecialSection: {
          const uint16_t numbers[] = {20, 39, 77, 156, 230, 293};
          if (r.Below(4)) {
            guest::StoreU16(h.base, area + kA, numbers[r.Below(6)]);
          }
          r4 = area + kA;
          break;
        }
        case kSliceTest:
        case kIsMainRoad:
        case kStepBack:
        case kNormalizeBack:
          r4 = area + kA;
          if (f.kind == kNormalizeBack && r.Below(2)) {  // before the start
            guest::StoreU16(h.base, area + kA + 2, uint16_t(-int32_t(1 + r.Below(20))));
          }
          if (f.kind == kNormalizeBack && r.Below(8) == 0) {  // a section above 0x8000
            guest::StoreU16(h.base, area + kA, uint16_t(0x8000 | r.Below(0x8000)));
          }
          break;
        case kStepForward:
        case kStepAlongPath:
          r4 = area + kA;
          r5 = area + kCount;
          r6 = area + kPath;
          break;
        case kStepForwardMasked:
          r4 = area + kA;
          r5 = area + kCount;
          r6 = area + kMasks;
          break;
        case kNormalizeForward: {
          // Past the section's end mostly; with masks or none.
          if (r.Below(4)) {
            const uint32_t section = guest::LoadU16(h.base, area + kA);
            guest::StoreU16(h.base, area + kA + 2,
                            uint16_t(route.Slices(section) + r.Below(24)));
          }
          if (r.Below(8) == 0) {  // a section above 0x7FFF
            guest::StoreU16(h.base, area + kA, uint16_t(0x8000 | r.Below(0x8000)));
          }
          r4 = area + kA;
          r5 = r.Below(3) == 0 ? 0 : area + kMasks;
          break;
        }
        case kStepForwardLane:
        case kStepForwardLaneMasked:
          guest::StoreU16(h.base, area + kLane, uint16_t(int32_t(r.Below(14)) - 2));
          r4 = area + kA;
          r5 = area + kLane;
          r6 = area + kCount;
          r7 = area + kMasks;
          break;
        case kBranchesMatching:
          r4 = area + kBranches;
          r5 = route.address[r.Below(route.sections)];
          r6 = area + kMask;
          break;
        case kToIndex:
        case kFromIndex:
        case kClampPosition: {
          global = true;
          const int32_t section = int32_t(r.Below(route.sections + 4)) - 2;
          const int32_t slice = int32_t(r.Below(20)) - 6;
          const uint32_t packed = (uint32_t(uint16_t(section)) << 16) | uint16_t(slice);
          if (f.kind == kToIndex) {
            r3 = packed;
          } else if (f.kind == kFromIndex) {
            r3 = uint64_t(int64_t(int32_t(r.Below(80)) - 10));
          } else {
            r3 = area + kA;
            r4 = packed;
          }
          break;
        }
        case kDistance:
          r4 = area + kA;
          r5 = area + kB;
          break;
        case kLinkDistance:
          r4 = area + kA;
          r5 = area + kB;
          r6 = area + kFloat;
          r7 = r.Below(4);
          break;
        case kSlicePoint: {
          global = true;
          const uint32_t which[] = {0, 1, 2, 3, 4, 0, 1, 2, 17, 100, 0xFFFFFFFFu};
          r3 = area + kOut;
          r4 = which[r.Below(11)];
          const uint32_t section = r.Below(route.sections);
          r5 = (junk[0] & 0xFFFF0000u) | section;
          r6 = r.Below(route.slices[section]);
          what += fmt::format(" point {}", uint32_t(r4));
          break;
        }
      }
      const std::vector<uint8_t> inputs = Read(h.base, area, kCompared + 16);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, area, inputs.data(), kCompared + 16);
        if (global) {
          guest::StoreU32(h.base, tr::kRoute, area + kFile);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = r3;
        t.r4.u64 = r4;
        t.r5.u64 = r5;
        t.r6.u64 = r6;
        t.r7.u64 = r7;
        t.f1.u64 = 0x7FF8DEADBEEF0000ull;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        guest::StoreU32(h.base, tr::kRoute, saved_route);
        // The route (unchanged by these functions) by its hash, the inputs
        // in 64-byte pieces.
        const uint64_t hash = base::Hash64(h.base + area, kIo);
        out[v].Word(uint32_t(hash >> 32));
        out[v].Word(uint32_t(hash));
        for (uint32_t i = kIo; i < kCompared; i += 64) {
          out[v].Bytes(Read(h.base, area + i, 64));
        }
        out[v].Floats(h.base, area + kOut, 4);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
        if (f.kind == kDistance) {
          out[v].Double(t.f1.u64);
        }
      }
      results.Check(out[0], out[1], what);
    }
    results.Report();
  }
  if (g_fallbacks != 0) {
    ITC_LOG_INFO("harness: track_route: {} cases handed to the original", g_fallbacks);
  }
  memory::Free(table);
  memory::Free(area);
}

}  // namespace harness
