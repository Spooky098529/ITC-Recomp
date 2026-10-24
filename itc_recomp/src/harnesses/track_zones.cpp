// The track zones (src/track_zones/, docs/functions/track-zones.md) against
// the recompiled functions. Each case fills blocks from the allocation layer
// with its inputs: lists of 16, 64 and 128 slots in a random state (nodes
// linked in a random order, the rest on the free list, the data slots a
// permutation; now and then a full list or a position past the end, whose
// walk reads the halfwords before the list, -1 there), ranges and zones of
// random bytes. Both versions run on the same bytes; the blocks (with the
// 16 bytes before each list) and r3 are compared exactly (the zones move
// halfwords and words only). The originals run with --native_track_zones
// off.

#include "harness.h"

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

}  // namespace

void TrackZones(Context& h) {
  Random r;
  Lists(h, r);
  Ranges(h, r);
  Zones(h, r);
}

}  // namespace harness
