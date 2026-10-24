// The track zones (src/track_zones/, docs/functions/track-zones.md) against
// the recompiled functions. Each case fills blocks from the allocation layer
// with its inputs: lists of 16, 64 and 128 slots in a random state (nodes
// linked in a random order, the rest on the free list, the data slots a
// permutation; now and then a full list or a position past the end, whose
// walk reads the halfwords before the list, -1 there), ranges and zones of
// random bytes. Both versions run on the same bytes; the blocks (with the
// 16 bytes before each list) and r3 are compared exactly (the zones move
// halfwords and words only). The ranges' tests run on a random course's
// route (test_route.h, its file in [0x826C5B10] for each case, put back
// after) with ranges whose ends, states and lists of sections to take and
// to leave out are random; a case whose path walk might not end (a loop on
// the random route without the end's section) is left out, as the original
// would never return. Their outputs are the range, 0x82603274 (the slices
// counted), the out position and r3. The originals run with
// --native_track_zones off.

#include "harness.h"
#include "test_route.h"

#include "base/flags.h"
#include "base/log.h"
#include "memory/heap.h"
#include "track_zones/track_zones.h"

#include <fmt/format.h>

#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_track_zones);

namespace harness {

namespace {

namespace tz = track_zones;

// Runs the recompiled version with the native flag off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  const bool saved = ITC_FLAG(native_track_zones);
  ITC_FLAG(native_track_zones) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_track_zones) = saved;
}

tz::Position LoadPosition(uint8_t* base, uint32_t address) {
  return {guest::LoadU16(base, address), guest::LoadU16(base, address + 2)};
}

// --- The register steps, as the adapters (src/hooks/track_zones.cpp) ---

void NativeList16Link(PPCContext& ctx, uint8_t* base) {
  tz::List16Link(base, ctx.r3.u32, int16_t(ctx.r4.u16));
}
void NativeList64Link(PPCContext& ctx, uint8_t* base) {
  tz::List64Link(base, ctx.r3.u32, int16_t(ctx.r4.u16));
}
void NativeList128Link(PPCContext& ctx, uint8_t* base) {
  tz::List128Link(base, ctx.r3.u32, int16_t(ctx.r4.u16));
}
void NativeList64Insert(PPCContext& ctx, uint8_t* base) {
  tz::List64Insert(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32), ctx.r5.u32);
}
void NativeList128Insert(PPCContext& ctx, uint8_t* base) {
  tz::List128Insert(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32), ctx.r5.u32);
}
void NativeRangeInit(PPCContext& ctx, uint8_t* base) {
  tz::RangeInit(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32),
                LoadPosition(base, ctx.r4.u32 + 4));
}
void NativeRangeAddBranch(PPCContext& ctx, uint8_t* base) {
  tz::RangeAddBranch(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = ctx.r3.u64 + 12;
}
void NativeZoneReset(PPCContext& ctx, uint8_t* base) {
  tz::ZoneReset(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),
                guest::LoadU32(base, ctx.r4.u32 + 4));
}
void NativeZoneIsInside(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tz::ZoneIsInside(base, ctx.r3.u32);
}
// Copies the case's blocks never share memory, so memcpy's fallback isn't
// taken; a case that would is counted.
int g_fallbacks = 0;
void NativeZoneRangesInsert(PPCContext& ctx, uint8_t* base) {
  uint32_t to = 0;
  if (!tz::ZoneRangesInsert(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &to)) {
    g_fallbacks++;
  }
  ctx.r3.u64 = to;
}

// The 16 bytes before a list (its walks past the end read the halfword at
// list - 4): -1 halfwords.
constexpr uint32_t kBefore = 16;

void Halfword(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU16(base, address, uint16_t(value));
}

// A random order of 0 to n - 1.
std::vector<uint32_t> Permutation(Random& r, uint32_t n) {
  std::vector<uint32_t> p(n);
  for (uint32_t i = 0; i < n; i++) {
    p[i] = i;
  }
  for (uint32_t i = n; i > 1; i--) {
    std::swap(p[i - 1], p[r.Below(i)]);
  }
  return p;
}

// An n-slot list at `list` in a random state with `count` nodes (see the
// top), and -1 halfwords before it.
void RandomList(uint8_t* base, uint32_t list, uint32_t n, uint32_t count, Random& r) {
  for (uint32_t i = 0; i < kBefore; i += 2) {
    Halfword(base, list - kBefore + i, 0xFFFF);
  }
  const std::vector<uint32_t> order = Permutation(r, n);
  const std::vector<uint32_t> slots = Permutation(r, n);
  for (uint32_t i = 0; i < n; i++) {
    const uint32_t node = order[i];
    const bool last = i + 1 == count || i + 1 == n;
    Halfword(base, list + 4 * node, last ? 0xFFFF : order[i + 1]);
    Halfword(base, list + 4 * node + 2, slots[node]);
  }
  Halfword(base, list + 4 * n, count < n ? order[count] : 0xFFFF);
  Halfword(base, list + 4 * n + 2, uint32_t(r.Next()));
  Halfword(base, list + 4 * n + 4, count > 0 ? order[0] : 0xFFFF);
  Halfword(base, list + 4 * n + 6, uint32_t(r.Next()));
  Halfword(base, list + 4 * n + 8, count);
}

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, uint8_t(r.Next()));
  }
}

// A count for an n-slot list: below n mostly, now and then n (full).
uint32_t RandomCount(Random& r, uint32_t n) {
  return r.Below(16) == 0 ? n : r.Below(n);
}

// A position for a list of `count` nodes: 0 to count, now and then past it.
uint32_t RandomPosition(Random& r, uint32_t count) {
  return r.Below(16) == 0 ? count + 1 + r.Below(3) : r.Below(count + 1);
}

// --- The lists ---

void Lists(Context& h, Random& r) {
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t n;
    bool insert;
  } kFunctions[] = {
      {"List16::Link (sub_821531C0)", Original<__imp__sub_821531C0>, NativeList16Link, 16, false},
      {"List64::Link (sub_820FE2C8)", Original<__imp__sub_820FE2C8>, NativeList64Link, 64, false},
      {"List128::Link (sub_8228B5A0)", Original<__imp__sub_8228B5A0>, NativeList128Link, 128,
       false},
      {"List64::Insert (sub_820FE330)", Original<__imp__sub_820FE330>, NativeList64Insert, 64,
       true},
      {"List128::Insert (sub_8228BC18)", Original<__imp__sub_8228BC18>, NativeList128Insert, 128,
       true},
  };
  const uint32_t block = memory::Alloc(0x1000, 0);
  const uint32_t list = block + 0x100;
  const uint32_t word = block + 0x40;
  for (const auto& f : kFunctions) {
    Results results(f.name);
    const uint32_t bytes = 4 * f.n + 12 + (f.insert ? 4 * f.n : 0);
    for (int c = 0; c < 400; c++) {
      // A full list only for Link (Insert's walk then runs off the list).
      const uint32_t count = f.insert ? r.Below(f.n) : RandomCount(r, f.n);
      const uint32_t position = f.insert ? r.Below(count + 1) : RandomPosition(r, count);
      // Link reads r4's low halfword, Insert r5's low word: the bits above
      // them random now and then.
      const uint64_t high = r.Below(4) == 0 ? r.Next() : 0;
      const uint64_t r4 = f.insert ? (high << 32) | position : (high & ~0xFFFFull) | position;
      RandomBytes(h.base, block, 0x1000, r);
      RandomList(h.base, list, f.n, count, r);
      const std::vector<uint8_t> inputs = Read(h.base, block, 0x1000);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), 0x1000);
        PPCContext t = CallContext(h);
        t.r3.u64 = list;
        if (f.insert) {
          t.r4.u64 = word;
          t.r5.u64 = r4;
        } else {
          t.r4.u64 = r4;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        out[v].Bytes(Read(h.base, list - kBefore, kBefore + bytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {} (count {}, position {:#x})", c, count, r4));
    }
    results.Report();
  }
  memory::Free(block);
}

// --- The ranges ---

void Ranges(Context& h, Random& r) {
  const uint32_t block = memory::Alloc(0x800, 0);
  const uint32_t range = block + 0x100;
  const uint32_t ends = block + 0x40;
  {
    Results results("RouteRange::Init (sub_820FE058)");
    for (int c = 0; c < 200; c++) {
      RandomBytes(h.base, block, 0x800, r);
      const std::vector<uint8_t> inputs = Read(h.base, block, 0x800);
      const uint64_t r5 = r.Next();  // not read
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), 0x800);
        PPCContext t = CallContext(h);
        t.r3.u64 = range;
        t.r4.u64 = ends;
        t.r5.u64 = r5;
        (v == 0 ? Original<__imp__sub_820FE058> : NativeRangeInit)(t, h.base);
        out[v].Bytes(Read(h.base, block, 0x800));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("RouteRange::AddBranch (sub_820FE8C8)");
    for (int c = 0; c < 400; c++) {
      const uint32_t count = r.Below(64);
      const uint64_t section = r.Below(2) ? r.Below(300) : r.Next();
      RandomBytes(h.base, block, 0x800, r);
      RandomList(h.base, range + 12, 64, count, r);
      const std::vector<uint8_t> inputs = Read(h.base, block, 0x800);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), 0x800);
        PPCContext t = CallContext(h);
        t.r3.u64 = range;
        t.r4.u64 = section;
        (v == 0 ? Original<__imp__sub_820FE8C8> : NativeRangeAddBranch)(t, h.base);
        out[v].Bytes(Read(h.base, block, 0x800));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {} (count {})", c, count));
    }
    results.Report();
  }
  memory::Free(block);
}

// --- The zones ---

void Zones(Context& h, Random& r) {
  constexpr uint32_t kZoneBytes = 17116;
  const uint32_t block = memory::Alloc(kZoneBytes + 0x2000, 0);
  const uint32_t zone = block + 0x100;
  const uint32_t args = block + 0x40;
  const uint32_t source = zone + kZoneBytes + 0x100;
  constexpr uint32_t kBytes = kZoneBytes + 0x2000;
  {
    Results results("Zone::Reset (sub_8228B540)");
    for (int c = 0; c < 100; c++) {
      const uint32_t index = r.Below(2) ? r.Below(70) : uint32_t(r.Next());
      const uint32_t vehicle = uint32_t(r.Next());
      RandomBytes(h.base, block, kBytes, r);
      guest::StoreU32(h.base, args, index);
      guest::StoreU32(h.base, args + 4, vehicle);
      const std::vector<uint8_t> inputs = Read(h.base, block, kBytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), kBytes);
        PPCContext t = CallContext(h);
        t.r3.u64 = zone;
        t.r4.u64 = args;
        (v == 0 ? Original<__imp__sub_8228B540> : NativeZoneReset)(t, h.base);
        out[v].Bytes(Read(h.base, block, kBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Zone::IsInside (sub_8228B4F8)");
    for (int c = 0; c < 100; c++) {
      const uint32_t flag = r.Below(2) ? r.Below(2) : uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32(h.base, zone + tz::kZoneInside, flag);
        PPCContext t = CallContext(h);
        t.r3.u64 = zone;
        (v == 0 ? Original<__imp__sub_8228B4F8> : NativeZoneIsInside)(t, h.base);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("ZoneRanges::Insert (sub_8228BBA8)");
    g_fallbacks = 0;
    for (int c = 0; c < 200; c++) {
      const uint32_t count = r.Below(16);
      const uint32_t position = r.Below(count + 1);
      const uint64_t high = r.Below(4) == 0 ? r.Next() << 32 : 0;
      RandomBytes(h.base, block, kBytes, r);
      RandomList(h.base, zone + tz::kZoneRanges, 16, count, r);
      const std::vector<uint8_t> inputs = Read(h.base, block, kBytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), kBytes);
        PPCContext t = CallContext(h);
        t.r3.u64 = zone + tz::kZoneRanges;
        t.r4.u64 = source;
        t.r5.u64 = high | position;
        (v == 0 ? Original<__imp__sub_8228BBA8> : NativeZoneRangesInsert)(t, h.base);
        out[v].Bytes(Read(h.base, block, kBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {} (count {}, position {})", c, count,
                                                position));
    }
    results.Report();
    if (g_fallbacks != 0) {
      ITC_LOG_INFO("harness: ZoneRanges::Insert: {} copies left to memcpy", g_fallbacks);
    }
  }
  memory::Free(block);
}

// --- The ranges' tests ---

void NativeToNeighbor(PPCContext& ctx, uint8_t* base) {
  tz::Position out;
  const uint32_t result =
      tz::RangeToNeighbor(base, &out, ctx.r4.u16, LoadPosition(base, ctx.r5.u32));
  guest::StoreU16(base, ctx.r3.u32, out.section);
  guest::StoreU16(base, ctx.r3.u32 + 2, out.slice);
  ctx.r3.u64 = result;
}
void NativeWithinEnds(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tz::RangeWithinEnds(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32));
}
void NativeOnPath(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tz::RangeOnPath(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32));
}
void NativeTest(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tz::RangeTest(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32));
}
void NativeUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tz::RangeUpdate(base, ctx.r3.u32, LoadPosition(base, ctx.r4.u32));
}
void NativeZoneUpdate(PPCContext& ctx, uint8_t* base) {
  const int32_t last = tz::ZoneUpdate(base, ctx.r3.u32);
  ctx.r3.u64 = last < 0 ? ctx.r1.u64 - 208 + 80 : uint64_t(last);
}

// The route's record for `section` (the table's entry).
uint32_t Record(uint8_t* base, uint32_t section) {
  return guest::LoadU32(base, guest::LoadU32(base, guest::LoadU32(base, tz::kRoute) + 12) +
                               4 * (section & 0xFFFF));
}

int32_t S16(uint8_t* base, uint32_t address) {
  return int16_t(guest::LoadU16(base, address));
}

// Whether a word is in a range's 64-slot list at `list` (as the originals
// look: the count's nodes from the head).
bool Listed(uint8_t* base, uint32_t list, int32_t value) {
  const uint32_t count = uint32_t(S16(base, list + 264));
  for (uint32_t i = 0; i < count; i++) {
    uint32_t node = uint32_t(S16(base, list + 260)) * 4 + list;
    for (uint32_t k = 0; k < i; k++) {
      node = uint32_t(S16(base, node)) * 4 + list;
    }
    if (int32_t(guest::LoadU32(base, uint32_t(S16(base, node + 2) + 67) * 4 + list)) == value) {
      return true;
    }
  }
  return false;
}

// Whether OnPath's walk from the range's start ends within 64 steps for a
// position on section `on`, whatever WithinEnds answers (when it says yes,
// the walk only ends sooner): its steps, as RouteRange::OnPath takes them.
bool WalkEnds(uint8_t* base, uint32_t range, int32_t on) {
  int32_t at = S16(base, range + 4);
  const int32_t end = S16(base, range + 8);
  if (on == at || end == at) {
    return true;
  }
  for (int step = 0; step < 64; step++) {
    const uint32_t record = Record(base, uint32_t(at));
    int32_t links = 0;
    const int32_t was = at;
    for (uint32_t i = 0; i < 3; i++) {
      const int32_t next = S16(base, record + 28 + 4 * i);
      if (on == next) {
        break;
      }
      if (next != -1) {
        links++;
      }
    }
    if (links == 1) {
      uint32_t i = 0;
      while (i < 3 && guest::LoadU16(base, record + 28 + 4 * i) == 0xFFFF) {
        i++;
      }
      if (i == 3) {
        return true;
      }
      at = S16(base, record + 28 + 4 * i);
    } else if (links > 1) {
      int32_t previous = -1;
      for (uint32_t i = 0; i < 3; i++) {
        if (at == previous) {
          break;
        }
        previous = S16(base, record + 28 + 4 * i);
        if (Listed(base, range + 12, previous)) {
          at = previous;
        }
      }
    } else {
      return true;
    }
    if (was == at || end == at) {
      return true;
    }
  }
  return false;
}

// Whether every walk a range's Test (or Update) makes for `position` ends:
// the position's own and the sections beside it (links 6 to 9).
bool TestEnds(uint8_t* base, uint32_t range, tz::Position position) {
  if (!WalkEnds(base, range, int16_t(position.section))) {
    return false;
  }
  const uint32_t record = Record(base, position.section);
  for (uint32_t k = 6; k <= 9; k++) {
    if (!WalkEnds(base, range, S16(base, record + 28 + 4 * k))) {
      return false;
    }
  }
  return true;
}

// A section: one of the route's mostly, now and then any.
uint32_t RandomSection(Random& r, const test_route::Route& route) {
  return r.Below(8) == 0 ? r.Below(test_route::kTableEntries) : r.Below(route.sections);
}

// A position on the route: its section's slices, now and then a few past
// either end, or any halfword.
tz::Position RandomRoutePosition(Random& r, const test_route::Route& route) {
  const uint32_t section = RandomSection(r, route);
  const int32_t n = int32_t(route.Slices(section));
  const int32_t slice = r.Below(16) == 0  ? int32_t(r.Next())
                        : r.Below(4) == 0 ? int32_t(r.Below(uint32_t(n) + 6)) - 3
                                          : int32_t(r.Below(uint32_t(n)));
  return {uint16_t(section), uint16_t(slice)};
}

void StorePosition(uint8_t* base, uint32_t address, tz::Position p) {
  guest::StoreU16(base, address, p.section);
  guest::StoreU16(base, address + 2, p.slice);
}

// A random range at `range` on the route: random bytes, then the state
// (1 mostly), the ends, the lists of sections to take (a few, sections of
// the route mostly) and to leave out (empty mostly), and +1060 (1 mostly).
void RandomRange(uint8_t* base, uint32_t range, const test_route::Route& route, Random& r) {
  RandomBytes(base, range, tz::kRangeBytes, r);
  const uint32_t states[] = {1, 1, 1, 2, 0};
  guest::StoreU32(base, range, r.Below(16) == 0 ? uint32_t(r.Next()) : states[r.Below(5)]);
  StorePosition(base, range + 4, RandomRoutePosition(r, route));
  StorePosition(base, range + 8, RandomRoutePosition(r, route));
  for (uint32_t list : {range + 12, range + 536}) {
    const bool take = list == range + 12;
    const uint32_t count = take ? (r.Below(8) == 0 ? r.Below(64) : r.Below(4))
                                : (r.Below(4) == 0 ? r.Below(4) : 0);
    // The halfwords before the list are the range's (or the first list's
    // data): the walks stay on the list.
    const std::vector<uint8_t> before = Read(base, list - kBefore, kBefore);
    RandomList(base, list, 64, count, r);
    Write(base, list - kBefore, before.data(), kBefore);
    for (uint32_t i = 0; i < 64; i++) {
      guest::StoreU32(base, list + 268 + 4 * i,
                      r.Below(8) == 0 ? uint32_t(r.Next()) : RandomSection(r, route));
    }
  }
  guest::StoreU32(base, range + 1060,
                  r.Below(16) == 0 ? uint32_t(r.Next()) : (r.Below(4) == 0 ? 0 : 1));
}

void RangeTests(Context& h, Random& r) {
  enum Kind { kToNeighbor, kWithinEnds, kOnPath, kTest, kUpdate };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } kFunctions[] = {
      {"RouteRange::ToNeighbor (sub_820FDF98)", Original<__imp__sub_820FDF98>, NativeToNeighbor,
       kToNeighbor},
      {"RouteRange::WithinEnds (sub_820FE120)", Original<__imp__sub_820FE120>, NativeWithinEnds,
       kWithinEnds},
      {"RouteRange::OnPath (sub_820FE398)", Original<__imp__sub_820FE398>, NativeOnPath, kOnPath},
      {"RouteRange::Test (sub_820FE6F8)", Original<__imp__sub_820FE6F8>, NativeTest, kTest},
      {"RouteRange::Update (sub_820FE878)", Original<__imp__sub_820FE878>, NativeUpdate,
       kUpdate},
  };
  constexpr uint32_t kAreaBytes = 0x3000;
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  const uint32_t table = memory::Alloc(4 * test_route::kTableEntries, 0);
  test_route::FillTable(h.base, table, area);
  const uint32_t range = area + 0x2400 - tz::kRangeBytes;
  const uint32_t io = area + 0x2800;
  const uint32_t saved_route = guest::LoadU32(h.base, tz::kRoute);
  const uint32_t saved_count = guest::LoadU32(h.base, tz::kSliceCount);
  for (const auto& f : kFunctions) {
    Results results(f.name);
    int left_out = 0;
    int answers[3] = {};  // the native results 0, 1 and 2 (or above)
    for (int c = 0; c < 1000; c++) {
      if (c % 4 == 0) {
        RandomBytes(h.base, area, kAreaBytes, r);
      }
      const test_route::Route route = test_route::BuildRoute(h.base, area, table, r);
      // The route in [0x826C5B10] while the case is made (Record reads it).
      guest::StoreU32(h.base, tz::kRoute, area + test_route::kFile);
      RandomRange(h.base, range, route, r);
      tz::Position position = RandomRoutePosition(r, route);
      // Positions on the ends' sections, or a link of theirs, now and then.
      if (r.Below(3) == 0) {
        const uint32_t end = range + 4 + 4 * r.Below(2);
        position.section = guest::LoadU16(h.base, end);
        if (r.Below(2) == 0) {
          position.slice = guest::LoadU16(h.base, end + 2) + uint16_t(r.Below(5)) - 2;
        }
        if (r.Below(3) == 0) {
          position.section =
              guest::LoadU16(h.base, Record(h.base, position.section) + 28 + 4 * r.Below(12));
        }
      }
      if (f.kind == kUpdate && r.Below(8) == 0) {
        position.section = uint16_t(0x8000 + r.Below(0x8000));
      }
      StorePosition(h.base, io, position);
      const uint16_t section = r.Below(4) == 0 ? uint16_t(r.Next()) : uint16_t(RandomSection(r, route));
      const uint64_t r4 = f.kind == kToNeighbor ? (r.Next() & ~0xFFFFull) | section : io;
      const bool ends = f.kind == kToNeighbor || f.kind == kWithinEnds ||
                        (f.kind == kOnPath ? WalkEnds(h.base, range, int16_t(position.section))
                                           : TestEnds(h.base, range, position));
      if (!ends) {
        left_out++;
        guest::StoreU32(h.base, tz::kRoute, saved_route);
        continue;
      }
      guest::StoreU32(h.base, tz::kRoute, saved_route);
      const uint32_t count = uint32_t(r.Next());
      const std::vector<uint8_t> inputs = Read(h.base, area, kAreaBytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, area, inputs.data(), kAreaBytes);
        guest::StoreU32(h.base, tz::kRoute, area + test_route::kFile);
        guest::StoreU32(h.base, tz::kSliceCount, count);
        PPCContext t = CallContext(h);
        if (f.kind == kToNeighbor) {
          t.r3.u64 = io + 8;
          t.r4.u64 = r4;
          t.r5.u64 = io;
        } else {
          t.r3.u64 = range;
          t.r4.u64 = io;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        if (v == 1) {
          answers[t.r3.u32 < 2 ? t.r3.u32 : 2]++;
        }
        out[v].Bytes(Read(h.base, range, tz::kRangeBytes));
        out[v].Bytes(Read(h.base, io, 16));
        out[v].Word(guest::LoadU32(h.base, tz::kSliceCount));
        out[v].Word(t.r3.u32);
        guest::StoreU32(h.base, tz::kRoute, saved_route);
        guest::StoreU32(h.base, tz::kSliceCount, saved_count);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
    ITC_LOG_INFO("harness: {}: {} cases left out (a walk that might not end); results 0, 1, "
                 "2: {}, {}, {}",
                 f.name, left_out, answers[0], answers[1], answers[2]);
  }
  memory::Free(table);
  memory::Free(area);
}

// Zone::Update: a zone of up to 16 random ranges (at their list slots) and
// a vehicle whose track position (+5232, Vehicle::Identity) is on the route.
void ZoneUpdates(Context& h, Random& r) {
  constexpr uint32_t kZoneBytes = 17116;
  constexpr uint32_t kAreaBytes = 0x3000;
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  const uint32_t table = memory::Alloc(4 * test_route::kTableEntries, 0);
  const uint32_t block = memory::Alloc(kZoneBytes + 0x100 + 5232 + 96, 0);
  const uint32_t zone = block + 0x100;
  const uint32_t vehicle = zone + kZoneBytes;
  const uint32_t block_bytes = kZoneBytes + 0x100 + 5232 + 96;
  test_route::FillTable(h.base, table, area);
  const uint32_t saved_route = guest::LoadU32(h.base, tz::kRoute);
  const uint32_t saved_count = guest::LoadU32(h.base, tz::kSliceCount);
  Results results("Zone::Update (sub_8228BC80)");
  int left_out = 0;
  int inside = 0;
  for (int c = 0; c < 300; c++) {
    RandomBytes(h.base, area, kAreaBytes, r);
    const test_route::Route route = test_route::BuildRoute(h.base, area, table, r);
    RandomBytes(h.base, block, block_bytes, r);
    const uint32_t count = r.Below(8) == 0 ? 0 : 1 + r.Below(r.Below(2) ? 16 : 3);
    const uint32_t list = zone + tz::kZoneRanges;
    RandomList(h.base, list, 16, count, r);
    for (uint32_t i = 0; i < 16; i++) {
      RandomRange(h.base, list + 76 + tz::kRangeBytes * i, route, r);
    }
    guest::StoreU32(h.base, zone + tz::kZoneVehicle, vehicle);
    tz::Position position = RandomRoutePosition(r, route);
    if (count != 0 && r.Below(2) == 0) {
      // On a range's start or end section.
      const uint32_t at = list + 76 + tz::kRangeBytes * r.Below(16) + 4 + 4 * r.Below(2);
      position = {guest::LoadU16(h.base, at), guest::LoadU16(h.base, at + 2)};
    }
    if (r.Below(16) == 0) {
      position.section = uint16_t(0x8000 + r.Below(0x8000));
    }
    StorePosition(h.base, vehicle + 5232 + 4, position);
    guest::StoreU32(h.base, tz::kRoute, area + test_route::kFile);
    bool ends = true;
    for (uint32_t i = 0; i < 16 && ends; i++) {
      ends = TestEnds(h.base, list + 76 + tz::kRangeBytes * i, position);
    }
    guest::StoreU32(h.base, tz::kRoute, saved_route);
    if (!ends) {
      left_out++;
      continue;
    }
    const uint32_t slices = uint32_t(r.Next());
    const std::vector<uint8_t> inputs = Read(h.base, block, block_bytes);
    const std::vector<uint8_t> route_bytes = Read(h.base, area, kAreaBytes);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, block, inputs.data(), block_bytes);
      Write(h.base, area, route_bytes.data(), kAreaBytes);
      guest::StoreU32(h.base, tz::kRoute, area + test_route::kFile);
      guest::StoreU32(h.base, tz::kSliceCount, slices);
      PPCContext t = CallContext(h);
      t.r3.u64 = zone;
      (v == 0 ? Original<__imp__sub_8228BC80> : NativeZoneUpdate)(t, h.base);
      if (v == 1 && guest::LoadU32(h.base, zone + tz::kZoneInside) != 0) {
        inside++;
      }
      out[v].Bytes(Read(h.base, block, block_bytes));
      out[v].Word(guest::LoadU32(h.base, tz::kSliceCount));
      out[v].Word(t.r3.u32);
      guest::StoreU32(h.base, tz::kRoute, saved_route);
      guest::StoreU32(h.base, tz::kSliceCount, saved_count);
    }
    results.Check(out[0], out[1], fmt::format("case {} ({} ranges)", c, count));
  }
  results.Report();
  ITC_LOG_INFO("harness: Zone::Update: {} cases left out (a walk that might not end), {} "
               "inside",
               left_out, inside);
  memory::Free(block);
  memory::Free(table);
  memory::Free(area);
}

// --- The zones' set-ups ---

// The set-ups' originals by zone (slot 0 of each zone's vtable).
GuestFunction SetupOriginal(uint32_t number) {
  static const GuestFunction kOriginals[tz::kZones] = {
#define ITC_ZONE_SETUP_ORIGINAL(address) Original<__imp__sub_##address>,
      ITC_ZONE_SETUP_ORIGINAL(8228BE20) ITC_ZONE_SETUP_ORIGINAL(8228BE88)
      ITC_ZONE_SETUP_ORIGINAL(8228BEF0) ITC_ZONE_SETUP_ORIGINAL(8228BF58)
      ITC_ZONE_SETUP_ORIGINAL(8228BFC0) ITC_ZONE_SETUP_ORIGINAL(8228C028)
      ITC_ZONE_SETUP_ORIGINAL(8228C090) ITC_ZONE_SETUP_ORIGINAL(8228C0F8)
      ITC_ZONE_SETUP_ORIGINAL(8228C160) ITC_ZONE_SETUP_ORIGINAL(8228C1C8)
      ITC_ZONE_SETUP_ORIGINAL(8228C230) ITC_ZONE_SETUP_ORIGINAL(8228C298)
      ITC_ZONE_SETUP_ORIGINAL(8228C300) ITC_ZONE_SETUP_ORIGINAL(8228C368)
      ITC_ZONE_SETUP_ORIGINAL(8228C3D0) ITC_ZONE_SETUP_ORIGINAL(8228C438)
      ITC_ZONE_SETUP_ORIGINAL(8228C4A0) ITC_ZONE_SETUP_ORIGINAL(8228C508)
      ITC_ZONE_SETUP_ORIGINAL(8228C570) ITC_ZONE_SETUP_ORIGINAL(8228C5D8)
      ITC_ZONE_SETUP_ORIGINAL(8228C640) ITC_ZONE_SETUP_ORIGINAL(8228C6A8)
      ITC_ZONE_SETUP_ORIGINAL(8228C710) ITC_ZONE_SETUP_ORIGINAL(8228C778)
      ITC_ZONE_SETUP_ORIGINAL(8228C7E0) ITC_ZONE_SETUP_ORIGINAL(8228C848)
      ITC_ZONE_SETUP_ORIGINAL(8228C8B0) ITC_ZONE_SETUP_ORIGINAL(8228C918)
      ITC_ZONE_SETUP_ORIGINAL(8228C980) ITC_ZONE_SETUP_ORIGINAL(8228C9E8)
      ITC_ZONE_SETUP_ORIGINAL(8228CA50) ITC_ZONE_SETUP_ORIGINAL(8228CAB8)
      ITC_ZONE_SETUP_ORIGINAL(8228CB20) ITC_ZONE_SETUP_ORIGINAL(8228CBC8)
      ITC_ZONE_SETUP_ORIGINAL(8228CC70) ITC_ZONE_SETUP_ORIGINAL(8228CD18)
      ITC_ZONE_SETUP_ORIGINAL(8228CD80) ITC_ZONE_SETUP_ORIGINAL(8228CE20)
      ITC_ZONE_SETUP_ORIGINAL(8228CFC8) ITC_ZONE_SETUP_ORIGINAL(8228D070)
      ITC_ZONE_SETUP_ORIGINAL(8228D220) ITC_ZONE_SETUP_ORIGINAL(8228D2C8)
      ITC_ZONE_SETUP_ORIGINAL(8228D370) ITC_ZONE_SETUP_ORIGINAL(8228D418)
      ITC_ZONE_SETUP_ORIGINAL(8228D5C0) ITC_ZONE_SETUP_ORIGINAL(8228D668)
      ITC_ZONE_SETUP_ORIGINAL(8228D710) ITC_ZONE_SETUP_ORIGINAL(8228D778)
      ITC_ZONE_SETUP_ORIGINAL(8228D870) ITC_ZONE_SETUP_ORIGINAL(8228D968)
      ITC_ZONE_SETUP_ORIGINAL(8228DA08) ITC_ZONE_SETUP_ORIGINAL(8228DBA8)
      ITC_ZONE_SETUP_ORIGINAL(8228DC60) ITC_ZONE_SETUP_ORIGINAL(8228DD00)
      ITC_ZONE_SETUP_ORIGINAL(8228DDE8) ITC_ZONE_SETUP_ORIGINAL(8228DE50)
      ITC_ZONE_SETUP_ORIGINAL(8228DEB0) ITC_ZONE_SETUP_ORIGINAL(8228DF58)
      ITC_ZONE_SETUP_ORIGINAL(8228E000) ITC_ZONE_SETUP_ORIGINAL(8228E068)
      ITC_ZONE_SETUP_ORIGINAL(8228E0D0) ITC_ZONE_SETUP_ORIGINAL(8228E190)
      ITC_ZONE_SETUP_ORIGINAL(8228E238) ITC_ZONE_SETUP_ORIGINAL(8228E2E8)
      ITC_ZONE_SETUP_ORIGINAL(8228E390) ITC_ZONE_SETUP_ORIGINAL(8228E3F8)
      ITC_ZONE_SETUP_ORIGINAL(8228E498) ITC_ZONE_SETUP_ORIGINAL(8228E550)
      ITC_ZONE_SETUP_ORIGINAL(8228E5F0) ITC_ZONE_SETUP_ORIGINAL(8228E6A0)
#undef ITC_ZONE_SETUP_ORIGINAL
  };
  return kOriginals[number];
}

void NoCopy(void*, uint32_t, uint32_t, uint32_t) {
  g_fallbacks++;
}

// Each set-up on a zone of random bytes, with its stack (the frame and
// what's below, where the range is built) random too: the bytes RouteRange
// ::Init leaves unwritten are copied into the zone as they are. The zone
// and r3 are compared, and the table's ranges are checked against the
// original's by the whole zone.
void Setups(Context& h, Random& r) {
  constexpr uint32_t kZoneBytes = 17116;
  constexpr uint32_t kStackBytes = 0x800;
  const uint32_t block = memory::Alloc(kZoneBytes + 0x100, 0);
  const uint32_t zone = block + 0x80;
  const uint32_t args = block + 0x40;
  const uint32_t block_bytes = kZoneBytes + 0x100;
  g_fallbacks = 0;
  for (uint32_t number = 0; number < tz::kZones; number++) {
    Results results(fmt::format("Zone{}::Setup (sub_{:08X})", number, tz::SetupAddress(number)));
    for (int c = 0; c < 4; c++) {
      RandomBytes(h.base, block, block_bytes, r);
      guest::StoreU32(h.base, args, c == 0 ? number : uint32_t(r.Next()));
      const std::vector<uint8_t> inputs = Read(h.base, block, block_bytes);
      const uint32_t stack = CallContext(h).r1.u32 - kStackBytes;
      RandomBytes(h.base, stack, kStackBytes, r);
      const std::vector<uint8_t> stack_bytes = Read(h.base, stack, kStackBytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, block, inputs.data(), block_bytes);
        Write(h.base, stack, stack_bytes.data(), kStackBytes);
        PPCContext t = CallContext(h);
        t.r3.u64 = zone;
        t.r4.u64 = args;
        if (v == 0) {
          SetupOriginal(number)(t, h.base);
        } else {
          const tz::SetupCalls calls{nullptr, NoCopy};
          t.r3.u64 = tz::ZoneSetup(h.base, number, zone, guest::LoadU32(h.base, args),
                                   guest::LoadU32(h.base, args + 4), t.r1.u32, calls);
        }
        out[v].Bytes(Read(h.base, block, block_bytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  if (g_fallbacks != 0) {
    ITC_LOG_INFO("harness: the set-ups: {} copies left to memcpy", g_fallbacks);
  }
  memory::Free(block);
}

}  // namespace

void TrackZones(Context& h) {
  Random r;
  Lists(h, r);
  Ranges(h, r);
  Zones(h, r);
  RangeTests(h, r);
  ZoneUpdates(h, r);
  Setups(h, r);
}

}  // namespace harness
