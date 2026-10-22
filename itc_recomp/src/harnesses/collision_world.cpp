// The collision world's owner and the track's collision
// (src/track_collision/) against the recompiled functions. The inputs go in
// a block from the allocation layer (an area too large for the scratch
// area: the racer records' count is at +0x595E0), filled anew for each
// case; globals a function reads or writes (the race manager's pointer, the
// collision world's pointer and surface settings) set for each case and
// put back after. The originals run with the native flags of their callees
// off, so the whole chain is the recompiled code's.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "track_collision/track_collision.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <iterator>
#include <string>

ITC_FLAG_DECLARE(bool, native_collision_world);
ITC_FLAG_DECLARE(bool, native_collision);
ITC_FLAG_DECLARE(bool, native_car_physics);
ITC_FLAG_DECLARE(bool, native_vector_library);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_math);

namespace harness {

namespace {

namespace tc = track_collision;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeTimesThreePlus, ctx.r3.u64 = tc::TimesThreePlus(ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeRacerCount, ctx.r3.u64 = tc::RaceRacerCount(base))
ITC_NATIVE_STEP(NativeCarsCollide, ctx.r3.u64 = tc::RacersCarsCollide(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeResetWorld, ctx.r3.u64 = tc::TrackCollisionResetWorld(base))
ITC_NATIVE_STEP(NativeMeshPrepare, ctx.r3.u64 = tc::TrackMeshPrepare(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeMeshInitScratch, ctx.r3.u64 = tc::TrackMeshInitScratch(base, ctx.r3.u32))
void NativeMeshFree(PPCContext& ctx, uint8_t* base) {
  uint32_t r3 = 0;
  if (!tc::TrackMeshFree(base, ctx.r3.u32, &r3)) {
    __imp__sub_820F6940(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
ITC_NATIVE_STEP(NativeMeshOnLoaded, tc::TrackMeshOnLoaded(base, ctx.r6.u32))
ITC_NATIVE_STEP(NativeSliceKind, ctx.r3.u64 = tc::TrackMeshSliceKind(base, ctx.r3.u32,
                                                                     ctx.r4.u32))
ITC_NATIVE_STEP(NativeRoutePrepare, tc::TrackRoutePrepare(base, ctx.r3.u32))
void NativeRouteFree(PPCContext& ctx, uint8_t* base) {
  uint32_t r3 = 0;
  if (!tc::TrackRouteFree(base, ctx.r3.u32, &r3)) {
    __imp__sub_820F8618(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}

// Runs the recompiled version with the native flags off, so the functions
// it calls that are ours run the originals too.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  bool* flags[] = {&ITC_FLAG(native_collision_world), &ITC_FLAG(native_collision),
                   &ITC_FLAG(native_car_physics),     &ITC_FLAG(native_vector_library),
                   &ITC_FLAG(native_matrix_math),     &ITC_FLAG(native_vehicle_accessors),
                   &ITC_FLAG(native_math)};
  bool saved[std::size(flags)];
  for (size_t i = 0; i < std::size(flags); i++) {
    saved[i] = *flags[i];
    *flags[i] = false;
  }
  recompiled(ctx, base);
  for (size_t i = 0; i < std::size(flags); i++) {
    *flags[i] = saved[i];
  }
}

Random Seeded(uint64_t seed) {
  Random r;
  for (uint64_t i = 0; i < seed; i++) {
    r.Next();
  }
  return r;
}

void Append32(std::vector<uint8_t>& out, uint32_t word) {
  for (int i = 0; i < 4; i++) {
    out.push_back(uint8_t(word >> (24 - 8 * i)));
  }
}

constexpr uint32_t kRaceManagerPointer = 0x826D82F4;
constexpr uint32_t kWorldPointer = 0x826C2D88;
constexpr uint32_t kSurfaceGlobals = 0x826C2DF0, kSurfaceBytes = 64;

// The area (a block of the allocation layer).
constexpr uint32_t kAreaBytes = 0x60000;
// The bytes compared after the small functions (the area's start).
constexpr uint32_t kCompared = 0x2000;

// --- The world's owner and the mesh's lookups: no allocations ---

enum Kind {
  kTimesThreePlus,  // r3, r4 random
  kRacerCount,      // [0x826D82F4] = the area, +2364 random
  kCarsCollide,     // r3 = the area (the records), the count at +0x595E0
  kResetWorld,      // the world at the area, its objects at +0x100
  kOnLoaded,        // r6 = the mesh (the area), its file at +0x100
  kSliceKind,       // r3 = the mesh, r4 = a track position
  kRoutePrepare,    // r3 = the route (the area)
};

void SmallCases(Context& h, uint32_t area) {
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } functions[] = {
      {"sub_8228F090", Original<__imp__sub_8228F090>, NativeTimesThreePlus, kTimesThreePlus},
      {"Race::RacerCount (sub_821705F8)", Original<__imp__sub_821705F8>, NativeRacerCount,
       kRacerCount},
      {"Racers::CarsCollide (sub_820FEFF8)", Original<__imp__sub_820FEFF8>, NativeCarsCollide,
       kCarsCollide},
      {"TrackCollision::ResetWorld (sub_820FEAF8)", Original<__imp__sub_820FEAF8>,
       NativeResetWorld, kResetWorld},
      {"TrackMesh::OnLoaded (sub_820F69B0)", Original<__imp__sub_820F69B0>, NativeMeshOnLoaded,
       kOnLoaded},
      {"TrackMesh::SliceKind (sub_820F6A08)", Original<__imp__sub_820F6A08>, NativeSliceKind,
       kSliceKind},
      {"TrackRoute::Prepare (sub_820F8600)", Original<__imp__sub_820F8600>, NativeRoutePrepare,
       kRoutePrepare},
  };
  const uint32_t saved_manager = guest::LoadU32(h.base, kRaceManagerPointer);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  uint8_t saved_surfaces[kSurfaceBytes];
  std::memcpy(saved_surfaces, h.base + kSurfaceGlobals, kSurfaceBytes);
  for (const auto& f : functions) {
    Results results(f.name);
    for (uint64_t seed = 0; seed < 200; seed++) {
      std::vector<uint8_t> result[2];
      std::string what = fmt::format("seed {}", seed);
      for (int v = 0; v < 2; v++) {
        Random r = Seeded(seed * 41 + 3);
        for (uint32_t i = 0; i < kCompared; i += 4) {
          guest::StoreU32(h.base, area + i, uint32_t(r.Next()));
        }
        for (uint32_t i = 0; i < kSurfaceBytes; i += 4) {
          guest::StoreU32(h.base, kSurfaceGlobals + i, uint32_t(r.Next()));
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = uint32_t(r.Next());
        t.r4.u64 = uint32_t(r.Next());
        switch (f.kind) {
          case kTimesThreePlus:
            break;
          case kRacerCount:
            guest::StoreU32(h.base, kRaceManagerPointer, area);
            guest::StoreU32(h.base, area + 2364, uint32_t(r.Next()));
            break;
          case kCarsCollide: {
            // The count: 0 to 6, or a negative one; a record's flags with
            // bit 0x8 one time in eight.
            const uint32_t pick = r.Below(9);
            const uint32_t count = pick < 7 ? pick : uint32_t(-int32_t(1 + r.Below(3)));
            guest::StoreU32(h.base, area + 0x595E0, count);
            for (uint32_t i = 0; i < 6; i++) {
              const uint32_t flags = uint32_t(r.Next()) & ~0x8u;
              guest::StoreU32(h.base, area + 3888 + 3936 * i,
                              flags | (r.Below(8) == 0 ? 0x8u : 0u));
            }
            t.r3.u64 = area;
            what += fmt::format(" count {}", int32_t(count));
            break;
          }
          case kResetWorld: {
            // The world (+0): 1 to 6 objects of 320 bytes at +0x100.
            const uint32_t objects = 1 + r.Below(6);
            guest::StoreU32(h.base, area + 12, objects);
            guest::StoreU32(h.base, area + 16, area + 0x100);
            guest::StoreU32(h.base, kWorldPointer, area);
            break;
          }
          case kOnLoaded: {
            // The file (+0x100): +0 the section count (0 to 8), +4 the
            // table's offset, the table's entries offsets.
            const uint32_t file = area + 0x100;
            const uint32_t count = r.Below(9);
            const uint32_t table = 0x40 + 4 * r.Below(16);
            guest::StoreU32(h.base, area + 4, file);
            guest::StoreU32(h.base, file + 0, count);
            guest::StoreU32(h.base, file + 4, table);
            for (uint32_t i = 0; i < count; i++) {
              guest::StoreU32(h.base, file + table + 4 * i, 0x200 + 4 * r.Below(0x100));
            }
            t.r6.u64 = area;
            break;
          }
          case kSliceKind: {
            // The mesh's file (+0x100), its section table (+0x180, 4
            // sections), the sections (+0x200, 0x300 apart), each slice
            // table at the section's +72 (offset 0x80 to 0xBC).
            const uint32_t file = area + 0x100;
            guest::StoreU32(h.base, area + 4, file);
            guest::StoreU32(h.base, file + 4, area + 0x180);
            for (uint32_t s = 0; s < 4; s++) {
              const uint32_t section = area + 0x200 + 0x300 * s;
              guest::StoreU32(h.base, area + 0x180 + 4 * s, section);
              guest::StoreU32(h.base, section + 72, 0x80 + 4 * r.Below(16));
            }
            const uint32_t position = area + 0x1800;
            guest::StoreU16(h.base, position + 0, uint16_t(r.Below(4)));
            guest::StoreU16(h.base, position + 2, uint16_t(r.Below(16)));
            t.r3.u64 = area;
            t.r4.u64 = position;
            break;
          }
          case kRoutePrepare:
            t.r3.u64 = area;
            break;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        result[v] = Read(h.base, area, kCompared);
        const std::vector<uint8_t> surfaces = Read(h.base, kSurfaceGlobals, kSurfaceBytes);
        result[v].insert(result[v].end(), surfaces.begin(), surfaces.end());
        Append32(result[v], t.r3.u32);
      }
      results.Check(result[0], result[1], what);
    }
    results.Report();
  }
  guest::StoreU32(h.base, kRaceManagerPointer, saved_manager);
  guest::StoreU32(h.base, kWorldPointer, saved_world);
  std::memcpy(h.base + kSurfaceGlobals, saved_surfaces, kSurfaceBytes);
}

// --- The mesh's and the route's lifetime: pages from the allocation layer
// (the same sizes on both sides, at different addresses), so each case
// compares the mesh or route with the pages' addresses in it replaced by
// their number, which of the given pages were freed, and r3 likewise. The
// pages left are freed after each side. Only with --native_heap (the
// adapters' condition): the recompiled side's VirtualAlloc and VirtualFree
// then use the layer too. ---

void LifetimeCases(Context& h, uint32_t area) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: collision_world: the mesh's lifetime skipped (--native_heap off)");
    return;
  }
  enum LifetimeKind { kPrepare, kInitScratch, kMeshFree, kRouteFree };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    LifetimeKind kind;
  } functions[] = {
      {"TrackMesh::Prepare (sub_820F68E8)", Original<__imp__sub_820F68E8>, NativeMeshPrepare,
       kPrepare},
      {"TrackMesh::InitScratch (sub_820F6898)", Original<__imp__sub_820F6898>,
       NativeMeshInitScratch, kInitScratch},
      {"TrackMesh::Free (sub_820F6940)", Original<__imp__sub_820F6940>, NativeMeshFree,
       kMeshFree},
      {"TrackRoute::Free (sub_820F8618)", Original<__imp__sub_820F8618>, NativeRouteFree,
       kRouteFree},
  };
  constexpr uint32_t kObjectBytes = 16;
  for (const auto& f : functions) {
    Results results(f.name);
    for (uint64_t seed = 0; seed < 50; seed++) {
      std::vector<uint8_t> result[2];
      Random pick = Seeded(seed * 43 + 5);
      // Free: the scratch block (with its 8 KB block) and the file each
      // there three times in four.
      const bool has_scratch = pick.Below(4) != 0, has_file = pick.Below(4) != 0;
      for (int v = 0; v < 2; v++) {
        Random r = Seeded(seed * 47 + 7);
        for (uint32_t i = 0; i < kObjectBytes; i += 4) {
          guest::StoreU32(h.base, area + i, uint32_t(r.Next()));
        }
        std::vector<uint32_t> given;
        auto pages = [&](uint32_t size) {
          const uint32_t p = memory::AllocPages(size, 4096);
          given.push_back(p);
          return p;
        };
        switch (f.kind) {
          case kPrepare:
          case kInitScratch:
            break;
          case kMeshFree:
            if (has_scratch) {
              const uint32_t scratch = pages(8);
              guest::StoreU32(h.base, scratch + 0, 0);
              guest::StoreU32(h.base, scratch + 4, pages(8192));
              guest::StoreU32(h.base, area + 0, scratch);
            } else {
              guest::StoreU32(h.base, area + 0, 0);
            }
            guest::StoreU32(h.base, area + 4, has_file ? pages(0x10000) : 0);
            break;
          case kRouteFree:
            // A file always (the adapter runs the original without one).
            guest::StoreU32(h.base, area + 0, pages(0x10000));
            break;
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = area;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        // The pages made: those in the object (and the scratch block's +4)
        // and r3 that the layer gave and that weren't given.
        std::vector<uint32_t> made;
        auto note = [&](uint32_t p) {
          if (memory::IsPages(p) && std::find(given.begin(), given.end(), p) == given.end() &&
              std::find(made.begin(), made.end(), p) == made.end()) {
            made.push_back(p);
          }
        };
        note(guest::LoadU32(h.base, area + 0));
        note(guest::LoadU32(h.base, area + 4));
        if (memory::IsPages(guest::LoadU32(h.base, area + 0))) {
          note(guest::LoadU32(h.base, guest::LoadU32(h.base, area + 0) + 4));
        }
        note(t.r3.u32);
        auto name = [&](uint32_t w) {
          for (size_t i = 0; i < given.size(); i++) {
            if (w == given[i]) {
              return uint32_t(0xB10C0000u + i);
            }
          }
          for (size_t i = 0; i < made.size(); i++) {
            if (w == made[i]) {
              return uint32_t(0x0E550000u + i);
            }
          }
          return w;
        };
        std::vector<uint8_t>& out = result[v];
        for (uint32_t i = 0; i < kObjectBytes; i += 4) {
          Append32(out, name(guest::LoadU32(h.base, area + i)));
        }
        // The made blocks' words (the scratch block's two).
        for (uint32_t p : made) {
          Append32(out, name(guest::LoadU32(h.base, p + 0)));
          Append32(out, name(guest::LoadU32(h.base, p + 4)));
        }
        Append32(out, name(t.r3.u32));
        for (uint32_t p : given) {
          const bool left = memory::IsPages(p);
          out.push_back(left ? 1 : 0);
          if (left) {
            memory::FreePages(p);
          }
        }
        for (uint32_t p : made) {
          memory::FreePages(p);
        }
      }
      results.Check(result[0], result[1],
                    fmt::format("seed {} scratch {} file {}", seed, has_scratch, has_file));
    }
    results.Report();
  }
}

}  // namespace

void CollisionWorld(Context& h) {
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  SmallCases(h, area);
  LifetimeCases(h, area);
  memory::Free(area);
}

}  // namespace harness
