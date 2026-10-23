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
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "collision/collision.h"
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

ITC_NATIVE_STEP(NativeTriangleInSlice,
                ctx.r3.u64 = tc::TrackMeshTriangleInSlice(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                          ctx.r6.u32))
ITC_NATIVE_STEP(NativeTriangleNormal,
                tc::TrackMeshTriangleNormal(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                            ctx.r7.u32, ctx.r8.u32))
// r3 comes back as the original's frame + 96 (its second TriangleNormal's
// out).
ITC_NATIVE_STEP(NativeQuadHeight,
                ctx.f1.f64 = tc::TrackMeshQuadHeight(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32, ctx.r1.u32 - 208);
                ctx.r3.u64 = ctx.r1.u32 - 208 + 96)
ITC_NATIVE_STEP(NativeFindTriangle,
                ctx.r3.u64 = tc::TrackMeshFindTriangle(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                       ctx.r6.u32, ctx.r7.u32))
ITC_NATIVE_STEP(NativeColorAt, tc::TrackMeshColorAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                    ctx.r6.u32, ctx.r7.u32))
ITC_NATIVE_STEP(NativeGroundHeight,
                ctx.r3.u64 = tc::TrackMeshGroundHeight(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                       ctx.r6.u32, ctx.r7.u32, ctx.r1.u32 - 240))
ITC_NATIVE_STEP(NativeGround, ctx.r3.u64 = tc::TrackCollisionGround(base, ctx.r3.u32, ctx.r4.u32,
                                                                    ctx.r5.u32, ctx.r1.u32 - 144))
void NativeGroundHits(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t count = 0;
  if (!tc::TrackCollisionGroundHits(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 384, &count)) {
    __imp__sub_820FEBD0(ctx, base);
    return;
  }
  ctx.r3.u64 = count;
}
ITC_NATIVE_STEP(NativeSliceContains,
                ctx.r3.u64 = tc::TrackMeshSliceContains(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeFindSlice,
                ctx.r3.u64 = tc::TrackMeshFindSlice(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeCrossSection,
                ctx.r3.u64 = tc::TrackPositionCrossSection(base, ctx.r3.u32, ctx.r4.u32,
                                                           ctx.r5.u32))
ITC_NATIVE_STEP(NativeSectionOutcode,
                ctx.r3.u64 = tc::TrackMeshSectionOutcode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeNearestSlice,
                ctx.r3.u64 = tc::TrackMeshNearestSlice(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeSearchLinked,
                ctx.r3.u64 = tc::TrackPositionSearchLinked(base, ctx.r3.u32, ctx.r4.u32,
                                                           ctx.r5.u32))
ITC_NATIVE_STEP(NativeSearchAll,
                ctx.r3.u64 = tc::TrackPositionSearchAll(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                        ctx.r1.u32 - 160))
ITC_NATIVE_STEP(NativeFindCell,
                ctx.r3.u64 = tc::TrackPositionFindCell(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                       ctx.r1.u32 - 240))
void NativeGatherCells(PPCContext& ctx, uint8_t* base) {
  uint32_t count = 0;
  if (!tc::WallQueryGatherCells(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192, &count)) {
    __imp__sub_820FC188(ctx, base);
    return;
  }
  ctx.r3.u64 = count;
}
// No frame of its own: the cell goes to the caller's r1 + 28.
ITC_NATIVE_STEP(NativeAddCell,
                ctx.r3.u64 = tc::WallQueryAddCell(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32))
ITC_NATIVE_STEP(NativeAddHit, ctx.r3.u64 = tc::WallQueryAddHit(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeIntersect2D,
                ctx.r3.u64 = tc::SegmentsIntersect2D(base, ctx.r3.u32, ctx.r4.u32))
int unload_native_cases = 0;
void NativeUnload(PPCContext& ctx, uint8_t* base) {
  if (!tc::TrackCollisionUnload(base)) {
    __imp__sub_820FEFB0(ctx, base);
    return;
  }
  unload_native_cases++;
}
ITC_NATIVE_STEP(NativeRouteOnLoaded, tc::TrackRouteOnLoaded(base, ctx.r6.u32); ctx.r3.u64 = 12)
ITC_NATIVE_STEP(NativeGroundCallback,
                ctx.r3.u64 = tc::TrackCollisionGroundCallback(base, ctx.r3.u32, ctx.r4.u32,
                                                              ctx.r5.u32, ctx.r1.u32 - 96))

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

// --- TrackCollision::Unload: a collision world (its objects, 0 to 3 of
// them with their shape records' two box copies, or none; the blocks +36
// and +48), the mesh's and the route's pages and the object at 0x826C5B28,
// all from the allocation layer, built anew for each side and installed
// in the globals (put back after). Compared: the globals with the given
// blocks' addresses replaced by their number, and which blocks were freed;
// the blocks left are freed after each side. Only with --native_heap (the
// adapter's condition). ---

void UnloadCases(Context& h) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: collision_world: Unload skipped (--native_heap off)");
    return;
  }
  constexpr uint32_t kGlobals = 0x826C5B10, kGlobalsBytes = 28;  // route, mesh, the object
  uint8_t saved[kGlobalsBytes];
  std::memcpy(saved, h.base + kGlobals, kGlobalsBytes);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  Results results("TrackCollision::Unload (sub_820FEFB0)");
  for (uint64_t seed = 0; seed < 60; seed++) {
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Random r = Seeded(seed * 61 + 19);
      PPCContext t = CallContext(h);
      std::vector<uint32_t> heap, pages;
      auto block = [&](uint32_t size, bool array) {
        const auto b = array ? memory::crt::OperatorNewArray(h.base, size)
                             : memory::crt::OperatorNew(h.base, size);
        heap.push_back(*b);
        for (uint32_t i = 0; i < size; i += 4) {
          guest::StoreU32(h.base, *b + i, uint32_t(r.Next()));
        }
        return *b;
      };
      auto page = [&](uint32_t size) {
        const uint32_t p = memory::AllocPages(size, 4096);
        pages.push_back(p);
        return p;
      };
      const uint32_t world = block(0x100, false);
      if (r.Below(8) == 0) {
        guest::StoreU32(h.base, world + 16, 0);
      } else {
        const uint32_t count = r.Below(4);
        const uint32_t objects = block(16 + 320 * count, true) + 16;
        guest::StoreU32(h.base, objects - 16, count);
        guest::StoreU32(h.base, world + 16, objects);
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t object = objects + 320 * i;
          guest::StoreU32(h.base, object + 0, 0x82002A94);
          for (uint32_t at : {36u, 40u}) {
            uint32_t copy = 0;
            if (r.Below(4) != 0) {
              copy = block(496, false);
              const uint32_t corners = h.data;
              for (uint32_t k = 0; k < 4; k++) {
                guest::StoreFloat(h.base, corners + 4 * k, k == 3 ? 0.0f : -1.0f);
                guest::StoreFloat(h.base, corners + 16 + 4 * k, k == 3 ? 0.0f : 1.0f);
              }
              collision::BoxConstruct(h.base, copy, corners, corners + 16, t.r1.u32 - 4096);
            }
            guest::StoreU32(h.base, object + at, copy);
          }
        }
      }
      guest::StoreU32(h.base, world + 36, r.Below(4) == 0 ? 0 : block(64, true));
      guest::StoreU32(h.base, world + 48, r.Below(4) == 0 ? 0 : block(64, true));
      guest::StoreU32(h.base, kWorldPointer, world);
      const uint32_t scratch = page(8);
      guest::StoreU32(h.base, scratch + 0, 0);
      guest::StoreU32(h.base, scratch + 4, page(8192));
      guest::StoreU32(h.base, tc::kMesh + 0, scratch);
      guest::StoreU32(h.base, tc::kMesh + 4, page(0x10000));
      guest::StoreU32(h.base, tc::kMesh + 8, 2);
      guest::StoreU32(h.base, tc::kRoute + 0, page(0x10000));
      guest::StoreU32(h.base, tc::kRoute + 4, uint32_t(r.Next()));
      guest::StoreU32(h.base, tc::kRoute + 8, 2);
      guest::StoreU32(h.base, 0x826C5B28, r.Below(4) == 0 ? 0 : block(4, false));
      (v == 0 ? Original<__imp__sub_820FEFB0> : NativeUnload)(t, h.base);
      auto name = [&](uint32_t w) {
        for (size_t i = 0; i < heap.size(); i++) {
          if (w == heap[i]) {
            return uint32_t(0xB10C0000u + i);
          }
        }
        for (size_t i = 0; i < pages.size(); i++) {
          if (w == pages[i]) {
            return uint32_t(0x9A9E0000u + i);
          }
        }
        return w;
      };
      std::vector<uint8_t>& out = result[v];
      Append32(out, name(guest::LoadU32(h.base, kWorldPointer)));
      for (uint32_t i = 0; i < kGlobalsBytes; i += 4) {
        Append32(out, name(guest::LoadU32(h.base, kGlobals + i)));
      }
      for (uint32_t b : heap) {
        const bool left = memory::IsHeapBlock(b);
        out.push_back(left ? 1 : 0);
      }
      for (uint32_t p : pages) {
        const bool left = memory::IsPages(p);
        out.push_back(left ? 1 : 0);
        if (left) {
          memory::FreePages(p);
        }
      }
      for (uint32_t b : heap) {
        if (memory::IsHeapBlock(b)) {
          memory::crt::Free(b);
        }
      }
      guest::StoreU32(h.base, kWorldPointer, saved_world);
      std::memcpy(h.base + kGlobals, saved, kGlobalsBytes);
    }
    results.Check(result[0], result[1], fmt::format("seed {}", seed));
  }
  results.Report();
  ITC_LOG_INFO("harness coverage: TrackCollision::Unload (sub_820FEFB0): {} of 60 cases native",
               unload_native_cases);
}

// --- The ground: a mesh built in the area (two sections of kSlices
// slices along z, each slice two 10 x 5 cells across x, each cell a quad,
// one time in three with its own height, or two triangles; vertices'
// heights 0 to 1, random surface kinds, 4 and 20 among them; a link from
// each section's end to the other's start and a few more), installed as
// the game's mesh (0x826C5B1C, put back after) for TrackCollision::Ground.
// Points over it and around it, at heights within the catch distance and
// beyond; track positions on the point's cell, near it, elsewhere, -1.
// Compared: the area, r3 and (QuadHeight) f1. Two groups expected to
// differ, where the vector unit's normalize meets a NaN or an infinity: a
// NaN in a vertex (for the functions whose results go through a normal:
// TriangleNormal, QuadHeight, GroundHeight, Ground, GroundCallback,
// GroundHits; the others' NaN groups must match), and an out not 16-aligned (the normalize then takes the
// 16 bytes around it, random words among them). The translated code leaves
// such a NaN's sign as the compiler ordered its operations (in this build
// it keeps the NaN's sign where the source negates it: -(x - 0.5) built as
// 0.5 - x), ours as the source has it; the game's normals are numbers in
// 16-aligned temporaries. ---

constexpr uint32_t kSlices = 6;
constexpr uint32_t kMeshP = 0x0, kFileP = 0x40, kTableP = 0x60, kPositionP = 0x200,
                   kPointP = 0x280, kResultP = 0x300, kColorP = 0x340, kNormalP = 0x360,
                   kOutsP = 0x3A0, kSectionP = 0x1000, kSectionBytes = 0x4000;
constexpr uint32_t kVerticesO = 0x100, kColorsO = 0x400, kSlicesO = 0x600, kTrianglesO = 0x800,
                   kOutlineO = 0xC00;
// SearchLinked's list of linked sections tried (and, past its six, the
// route's first halfword).
constexpr uint32_t kLinkedTried = 0x826C5B04, kLinkedTriedBytes = 16;
// For GroundHits: a box (its template, the matrix placing it), its four
// track positions (96 bytes each), a fake collision world (its +60 the
// room for hits) and the hit record.
constexpr uint32_t kBoxP = kSectionP + 2 * kSectionBytes, kTemplateP = kBoxP + 0x200,
                   kMatrixP = kBoxP + 0x400, kPositionsP = kBoxP + 0x500,
                   kWorldP = kBoxP + 0x700, kHitsP = kBoxP + 0x800;
// For the wall query's cells: a query (its +12 a shape record whose
// template and current copy are the box above, +108 the cell count, +112
// the cells) and the record.
constexpr uint32_t kQueryP = kBoxP + 0x900, kRecordP = kBoxP + 0xBC0;
constexpr uint32_t kGroundCompared = kBoxP + 0xC00;

uint32_t SectionAt(uint32_t area, uint32_t k) {
  return area + kSectionP + kSectionBytes * k;
}

// Builds the mesh; returns the triangles' and quads' addresses.
void BuildMesh(uint8_t* base, uint32_t area, Random& r, std::vector<uint32_t>& triangles,
               std::vector<uint32_t>& quads, bool one_nan) {
  guest::StoreU32(base, area + kMeshP + 0, 0);
  guest::StoreU32(base, area + kMeshP + 4, area + kFileP);
  guest::StoreU32(base, area + kMeshP + 8, 2);
  guest::StoreU32(base, area + kFileP + 0, 2);
  guest::StoreU32(base, area + kFileP + 4, area + kTableP);
  const uint8_t kKinds[] = {0, 1, 2, 3, 4, 4, 5, 9, 12, 20, 20, 30};
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t section = SectionAt(area, k);
    guest::StoreU32(base, area + kTableP + 4 * k, section);
    guest::StoreU16(base, section + 0, uint16_t(kSlices));
    // The links: none, then the end's to the other section, and one or two
    // random ones to the other section's slices.
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t link = section + 4 + 6 * i;
      guest::StoreU16(base, link + 0, 0xFFFF);
      guest::StoreU16(base, link + 2, uint16_t(r.Below(2)));
      guest::StoreU16(base, link + 4, uint16_t(r.Below(kSlices)));
    }
    const uint32_t end = section + 4 + 6 * r.Below(6);
    guest::StoreU16(base, end + 0, uint16_t(k == 0 ? kSlices - 1 : 0));
    guest::StoreU16(base, end + 2, uint16_t(1 - k));
    guest::StoreU16(base, end + 4, uint16_t(k == 0 ? 0 : kSlices - 1));
    for (uint32_t n = r.Below(3); n > 0; n--) {
      const uint32_t link = section + 4 + 6 * r.Below(6);
      if (link == end) {
        continue;
      }
      guest::StoreU16(base, link + 0, uint16_t(r.Below(kSlices)));
      guest::StoreU16(base, link + 2, uint16_t(1 - k));
      guest::StoreU16(base, link + 4, uint16_t(r.Below(kSlices)));
    }
    guest::StoreU32(base, section + 64, kVerticesO);
    guest::StoreU32(base, section + 68, kColorsO);
    guest::StoreU32(base, section + 72, kSlicesO);
    guest::StoreU32(base, section + 76, kTrianglesO);
    // The outline points (x, z) at the section's halfword +2: the left and
    // right edges of each row; the section's bounds (+40 the half height,
    // +44 the middle height, +48/+56 x, +52/+60 z), a little off one time
    // in four.
    guest::StoreU16(base, section + 2, uint16_t(kOutlineO));
    for (uint32_t row = 0; row <= kSlices; row++) {
      for (uint32_t side = 0; side < 2; side++) {
        const uint32_t q = section + kOutlineO + 8 * (row * 2 + side);
        guest::StoreFloat(base, q + 0, side == 0 ? -10.0f : 10.0f);
        guest::StoreFloat(base, q + 4, 5.0f * float(row + kSlices * k));
      }
    }
    const float off = r.Below(4) == 0 ? float(int32_t(r.Below(400)) - 200) / 100.0f : 0.0f;
    guest::StoreFloat(base, section + 40, 2.0f + off);
    guest::StoreFloat(base, section + 44, 0.5f);
    guest::StoreFloat(base, section + 48, -10.0f + off);
    guest::StoreFloat(base, section + 52, 5.0f * float(kSlices * k) + off);
    guest::StoreFloat(base, section + 56, 10.0f - off);
    guest::StoreFloat(base, section + 60, 5.0f * float(kSlices * (k + 1)) - off);
    // The vertices: rows along z, three columns across x.
    for (uint32_t row = 0; row <= kSlices; row++) {
      for (uint32_t col = 0; col < 3; col++) {
        const uint32_t v = section + kVerticesO + 12 * (row * 3 + col);
        guest::StoreFloat(base, v + 0, -10.0f + 10.0f * float(col));
        guest::StoreFloat(base, v + 4, float(r.Below(1000)) / 1000.0f);
        guest::StoreFloat(base, v + 8, 5.0f * float(row + kSlices * k));
        guest::StoreU16(base, section + kColorsO + 2 * (row * 3 + col), uint16_t(r.Next()));
      }
    }
    uint32_t count = 0;
    for (uint32_t s = 0; s < kSlices; s++) {
      const uint32_t slice = section + kSlicesO + 28 * s;
      for (uint32_t i = 4; i < 28; i += 2) {
        guest::StoreU16(base, slice + i, uint16_t(r.Next()));
      }
      guest::StoreU16(base, slice + 2, uint16_t(count));
      // Its outline: the row's two outline points and the next row's (a
      // triangle one time in six), its height near the vertices'.
      guest::StoreU16(base, slice + 12, uint16_t(s * 2 + 0));
      guest::StoreU16(base, slice + 14, uint16_t(s * 2 + 1));
      guest::StoreU16(base, slice + 16, uint16_t(s * 2 + 3));
      guest::StoreU16(base, slice + 18, r.Below(6) == 0 ? uint16_t(0xFFFF) : uint16_t(s * 2 + 2));
      guest::StoreFloat(base, slice + 20,
                        r.Below(8) == 0 ? 6.0f : 0.5f + float(int32_t(r.Below(200)) - 100) / 1000.0f);
      for (uint32_t cell = 0; cell < 2; cell++) {
        const uint16_t a = uint16_t(s * 3 + cell), b = uint16_t(a + 1), c = uint16_t(a + 4),
                       d = uint16_t(a + 3);
        auto add = [&](uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3) {
          const uint32_t t = section + kTrianglesO + 12 * count++;
          guest::StoreU16(base, t + 0, v0);
          guest::StoreU16(base, t + 2, v1);
          guest::StoreU16(base, t + 4, v2);
          guest::StoreU16(base, t + 6, v3);
          guest::StoreU8(base, t + 8, uint8_t(r.Next()));
          guest::StoreU8(base, t + 9, kKinds[r.Below(sizeof(kKinds))]);
          guest::StoreU16(base, t + 10, uint16_t(r.Next()));
          (v3 == 0xFFFF ? triangles : quads).push_back(t);
        };
        if (r.Below(3) == 0) {
          add(a, b, c, d);
        } else {
          add(a, b, c, 0xFFFF);
          add(a, c, d, 0xFFFF);
        }
      }
      guest::StoreU16(base, slice + 0, uint16_t(count - guest::LoadU16(base, slice + 2)));
    }
    if (one_nan && r.Below(2) == 0) {
      const uint32_t v = section + kVerticesO + 12 * r.Below(3 * (kSlices + 1)) + 4 * r.Below(3);
      guest::StoreU32(base, v, 0x7FC00000u | (uint32_t(r.Next()) & 0x80000001u));
    }
  }
}

void GroundCases(Context& h, uint32_t area) {
  enum GroundKind {
    kInSlice,       // r3 = a section, r4 = a slice, r5 = the point, r6 = 0 or 1
    kNormal,        // r3 = out, r4 = a section, r5 = a triangle, r6-r8 = its vertices
    kQuad,          // r3 = out, r4 = a section, r5 = a quad, r6 = the point
    kFind,          // r3 = the mesh, r4, r5 = outs, r6 = the point, r7 = the position
    kColor,         // r3 = out, r4 = the point, r5 = a triangle, r6, r7 = its section's arrays
    kGroundHeight,  // r3 = the mesh, r4 = the point, r5 = the position, r6, r7 = outs or 0
    kGround,        // r3 = the result, r4 = the point, r5 = the position or 0
    kHits,          // r3 = the hit record, r4 = the box
    kContains,      // r3 = a slice, r4 = its section's outline, r5 = the point
    kFindSlice,     // r3 = a section, r4 = a slice or -1, r5 = the point
    kCross,         // r3 = the position, r4 = the mesh, r5 = the point
    kOutcode,       // r3 = the mesh, r4 = the point, r5 = a section
    kNearest,       // r3 = a section, r4 = the point, r5 = the best distance
    kSearch,        // r3 = the position, r4 = the point, r5 = the mesh
    kFindCell,      // r3 = the mesh, r4 = the position, r5 = the point
    kGather,        // r3 = the query (its box as for GroundHits), r4 = the mesh
    kAddCell,       // r3 = the query (some cells listed), r4 = a cell
  };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    GroundKind kind;
  } functions[] = {
      {"TrackMesh::TriangleInSlice (sub_820F72F8)", Original<__imp__sub_820F72F8>,
       NativeTriangleInSlice, kInSlice},
      {"TrackMesh::TriangleNormal (sub_820F7468)", Original<__imp__sub_820F7468>,
       NativeTriangleNormal, kNormal},
      {"TrackMesh::QuadHeight (sub_820F7590)", Original<__imp__sub_820F7590>, NativeQuadHeight,
       kQuad},
      {"TrackMesh::FindTriangle (sub_820F7818)", Original<__imp__sub_820F7818>,
       NativeFindTriangle, kFind},
      {"TrackMesh::ColorAt (sub_820F6E18)", Original<__imp__sub_820F6E18>, NativeColorAt,
       kColor},
      {"TrackMesh::GroundHeight (sub_820F7AA0)", Original<__imp__sub_820F7AA0>,
       NativeGroundHeight, kGroundHeight},
      {"TrackCollision::Ground (sub_820FE8F8)", Original<__imp__sub_820FE8F8>, NativeGround,
       kGround},
      {"TrackCollision::GroundCallback (sub_820FEAB0)", Original<__imp__sub_820FEAB0>,
       NativeGroundCallback, kGround},
      {"TrackCollision::GroundHits (sub_820FEBD0)", Original<__imp__sub_820FEBD0>,
       NativeGroundHits, kHits},
      {"TrackMesh::SliceContains (sub_820F6A38)", Original<__imp__sub_820F6A38>,
       NativeSliceContains, kContains},
      {"TrackMesh::FindSlice (sub_820F6BE8)", Original<__imp__sub_820F6BE8>, NativeFindSlice,
       kFindSlice},
      {"TrackPosition::CrossSection (sub_820F71B0)", Original<__imp__sub_820F71B0>,
       NativeCrossSection, kCross},
      {"TrackMesh::SectionOutcode (sub_820F7D70)", Original<__imp__sub_820F7D70>,
       NativeSectionOutcode, kOutcode},
      {"TrackMesh::NearestSlice (sub_820F6C88)", Original<__imp__sub_820F6C88>,
       NativeNearestSlice, kNearest},
      {"TrackPosition::SearchLinked (sub_820F7EE8)", Original<__imp__sub_820F7EE8>,
       NativeSearchLinked, kSearch},
      {"TrackPosition::SearchAll (sub_820F7E30)", Original<__imp__sub_820F7E30>,
       NativeSearchAll, kSearch},
      {"TrackPosition::FindCell (sub_820F8090)", Original<__imp__sub_820F8090>, NativeFindCell,
       kFindCell},
      {"WallQuery::GatherCells (sub_820FC188)", Original<__imp__sub_820FC188>, NativeGatherCells,
       kGather},
      {"WallQuery::AddCell (sub_820FC110)", Original<__imp__sub_820FC110>, NativeAddCell,
       kAddCell},
  };
  uint8_t saved_mesh[12];
  std::memcpy(saved_mesh, h.base + tc::kMesh, sizeof(saved_mesh));
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  uint8_t saved_tried[kLinkedTriedBytes];
  std::memcpy(saved_tried, h.base + kLinkedTried, kLinkedTriedBytes);
  const float tolerance = guest::LoadFloat(h.base, 0x82001BDC);
  ITC_LOG_INFO("harness: collision_world: a slice's height tolerance {}, its center's reach {} "
               "(squared {}), the sections' margin {}",
               tolerance, guest::LoadFloat(h.base, 0x82002A28),
               guest::LoadFloat(h.base, 0x8200681C), guest::LoadFloat(h.base, 0x82001FD8));
  const float catch_distance = guest::LoadFloat(h.base, 0x82002A0C);
  ITC_LOG_INFO("harness: collision_world: the ground's catch distance {}, its offset {}, the "
               "slice offsets {} {} {} {}",
               catch_distance, guest::LoadFloat(h.base, 0x82002A08),
               int32_t(guest::LoadU32(h.base, 0x8200680C)), int32_t(guest::LoadU32(h.base, 0x82006810)),
               int32_t(guest::LoadU32(h.base, 0x82006814)), int32_t(guest::LoadU32(h.base, 0x82006818)));
  enum Group { kNumbers, kOneNaN };
  for (const auto& f : functions) {
    // A NaN reaches the vector unit's normalize only through the normals.
    const bool normalizes = f.kind == kNormal || f.kind == kQuad || f.kind == kGroundHeight ||
                            f.kind == kGround || f.kind == kHits;
    const std::string nan_group = std::string(", one NaN in a vertex") +
                                  (normalizes ? " (expected to differ)" : "");
    for (Group group : {kNumbers, kOneNaN}) {
      Results results(f.name + (group == kNumbers ? std::string() : nan_group));
      Results unaligned(std::string(f.name) + (group == kNumbers ? "" : ", one NaN in a vertex") +
                        ", out not 16-aligned (expected to differ)");
      bool out_unaligned = false;
      for (uint64_t seed = 0; seed < (group == kNumbers ? 200u : 100u); seed++) {
        std::vector<uint8_t> result[2];
        std::string what = fmt::format("seed {}", seed);
        for (int v = 0; v < 2; v++) {
          Random r = Seeded(seed * 53 + 17 * uint64_t(group) + 1);
          // The area is the allocation layer's (virtual memory, host
          // contiguous): filled and read back with memcpy.
          {
            std::vector<uint32_t> words(kGroundCompared / 4);
            for (uint32_t& w : words) {
              w = __builtin_bswap32(uint32_t(r.Next()));
            }
            std::memcpy(h.base + area, words.data(), kGroundCompared);
          }
          std::vector<uint32_t> triangles, quads;
          BuildMesh(h.base, area, r, triangles, quads, group == kOneNaN);
          // The point: over the mesh most of the time, its height near the
          // ground's (0 to 1) within twice the catch distance.
          const uint32_t point = area + kPointP;
          const float px = float(int32_t(r.Below(2600)) - 1300) / 100.0f;
          const float pz = float(int32_t(r.Below(6400)) - 200) / 100.0f;
          const float py = 0.5f + (float(r.Below(2001)) / 1000.0f - 1.0f) *
                                      (2.0f * catch_distance + 0.5f);
          guest::StoreFloat(h.base, point + 0, px);
          guest::StoreFloat(h.base, point + 4, py);
          guest::StoreFloat(h.base, point + 8, pz);
          // The position: the point's cell, near it, the other section, or -1.
          const int32_t true_section = pz >= 5.0f * kSlices ? 1 : 0;
          int32_t true_slice = int32_t(std::floor(pz / 5.0f)) - int32_t(kSlices) * true_section;
          true_slice = std::clamp(true_slice, 0, int32_t(kSlices) - 1);
          const uint32_t position = area + kPositionP;
          int32_t section = true_section, slice = true_slice;
          switch (r.Below(6)) {
            case 0:
            case 1:
              break;
            case 2:
              slice = std::clamp(slice + int32_t(r.Below(7)) - 3, 0, int32_t(kSlices) - 1);
              break;
            case 3:
              section = 1 - section;
              slice = int32_t(r.Below(kSlices));
              break;
            case 4:
              slice = int32_t(r.Below(kSlices));
              break;
            default:
              if (r.Below(2) == 0) {
                section = -1;
              } else {
                slice = -1;
              }
              break;
          }
          guest::StoreU16(h.base, position + 0, uint16_t(section));
          guest::StoreU16(h.base, position + 2, uint16_t(slice));
          // Its last point: the point's x and z one time in three.
          if (r.Below(3) == 0) {
            guest::StoreFloat(h.base, position + 16, px);
            guest::StoreFloat(h.base, position + 24, pz);
          }
          guest::StoreU16(h.base, position + 80, uint16_t(int32_t(r.Below(8)) - 2));
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          const uint32_t k = r.Below(2);
          const uint32_t sec = SectionAt(area, k);
          // Outs aligned, or (one time in eight) 8 bytes past.
          out_unaligned = r.Below(8) == 0;
          const uint32_t out = area + kNormalP + (out_unaligned ? 8 : 0);
          bool compare_f1 = false;
          switch (f.kind) {
            case kInSlice:
              out_unaligned = false;
              t.r3.u64 = sec;
              t.r4.u64 = r.Below(kSlices);
              t.r5.u64 = point;
              t.r6.u64 = r.Below(3) == 0 ? 1 : 0;
              break;
            case kNormal: {
              const bool quad = r.Below(3) == 0 && !quads.empty();
              const std::vector<uint32_t>& list = quad ? quads : triangles;
              uint32_t order[4] = {0, 1, 2, 3};
              const uint32_t n = quad ? 4 : 3;
              for (uint32_t i = n - 1; i > 0; i--) {
                std::swap(order[i], order[r.Below(i + 1)]);
              }
              t.r3.u64 = out;
              const uint32_t tri = list[r.Below(uint32_t(list.size()))];
              t.r4.u64 = SectionAt(area, (tri - area - kSectionP) / kSectionBytes);
              t.r5.u64 = tri;
              t.r6.u64 = order[0];
              t.r7.u64 = order[1];
              t.r8.u64 = order[2];
              break;
            }
            case kQuad: {
              if (quads.empty()) {
                result[v].clear();
                continue;
              }
              const uint32_t quad = quads[r.Below(uint32_t(quads.size()))];
              t.r3.u64 = out;
              t.r4.u64 = SectionAt(area, (quad - area - kSectionP) / kSectionBytes);
              t.r5.u64 = quad;
              t.r6.u64 = point;
              compare_f1 = true;
              break;
            }
            case kFind:
              out_unaligned = false;
              t.r3.u64 = area + kMeshP;
              t.r4.u64 = area + kOutsP;
              t.r5.u64 = area + kOutsP + 4;
              t.r6.u64 = point;
              t.r7.u64 = position;
              break;
            case kColor: {
              out_unaligned = false;
              std::vector<uint32_t> all = triangles;
              all.insert(all.end(), quads.begin(), quads.end());
              const uint32_t tri = all[r.Below(uint32_t(all.size()))];
              const uint32_t s = SectionAt(area, (tri - area - kSectionP) / kSectionBytes);
              t.r3.u64 = area + kColorP;
              t.r4.u64 = point;
              t.r5.u64 = tri;
              t.r6.u64 = s + kVerticesO;
              t.r7.u64 = s + kColorsO;
              break;
            }
            case kGroundHeight:
              t.r3.u64 = area + kMeshP;
              t.r4.u64 = point;
              t.r5.u64 = position;
              t.r6.u64 = r.Below(4) == 0 ? 0 : area + kColorP;
              t.r7.u64 = r.Below(4) == 0 ? 0 : out;
              break;
            case kGather:
            case kAddCell:
            case kHits: {
              out_unaligned = false;
              std::memcpy(h.base + tc::kMesh, h.base + area + kMeshP, 12);
              guest::StoreU32(h.base, kWorldPointer, area + kWorldP);
              guest::StoreU32(h.base, area + kWorldP + 60, r.Below(4) == 0 ? 0 : 32);
              // A car-sized box at the point, turned about y, tilted a
              // little one time in two.
              const uint32_t corners = area + kOutsP;
              const float sx = 0.5f + float(r.Below(1000)) / 1000.0f;
              const float sy = 0.3f + float(r.Below(500)) / 1000.0f;
              const float sz = 1.0f + float(r.Below(1500)) / 1000.0f;
              const float lo[4] = {-sx, -sy, -sz, 0.0f}, hi[4] = {sx, sy, sz, 0.0f};
              for (uint32_t i = 0; i < 4; i++) {
                guest::StoreFloat(h.base, corners + 4 * i, lo[i]);
                guest::StoreFloat(h.base, corners + 16 + 4 * i, hi[i]);
              }
              const uint32_t frame = t.r1.u32 - 2048;
              collision::BoxConstruct(h.base, area + kTemplateP, corners, corners + 16, frame);
              const float yaw = float(r.Below(6283)) / 1000.0f;
              const float tilt = r.Below(2) == 0 ? 0.0f : float(int32_t(r.Below(400)) - 200) / 1000.0f;
              const float c = std::cos(yaw), sn = std::sin(yaw), ct = std::cos(tilt),
                          st = std::sin(tilt);
              const float m[16] = {c, 0.0f, -sn, 0.0f, sn * st, ct, c * st, 0.0f,
                                   sn * ct, -st, c * ct, 0.0f, px, py, pz, 1.0f};
              for (uint32_t i = 0; i < 16; i++) {
                guest::StoreFloat(h.base, area + kMatrixP + 4 * i, m[i]);
              }
              collision::BoxConstruct(h.base, area + kBoxP, corners, corners + 16, frame);
              collision::BoxPlace(h.base, area + kBoxP, area + kMatrixP, area + kTemplateP, frame);
              // The positions: the point's cell or near it, their last
              // height near the point's (their x and z the corner's one
              // time in four).
              for (uint32_t i = 0; i < 4; i++) {
                const uint32_t p = area + kPositionsP + 96 * i;
                for (uint32_t w = 0; w < 96; w += 4) {
                  guest::StoreU32(h.base, p + w, guest::LoadU32(h.base, position + w));
                }
                if (r.Below(3) == 0) {
                  guest::StoreU16(h.base, p + 2, uint16_t(std::clamp(
                      true_slice + int32_t(r.Below(3)) - 1, 0, int32_t(kSlices) - 1)));
                }
                guest::StoreFloat(h.base, p + 20, py + float(int32_t(r.Below(2001)) - 1000) / 1000.0f);
                if (r.Below(4) == 0) {
                  const uint32_t corner = collision::BoxLowerCorner(h.base, area + kBoxP, i);
                  guest::StoreU32(h.base, p + 16, guest::LoadU32(h.base, corner + 0));
                  guest::StoreU32(h.base, p + 24, guest::LoadU32(h.base, corner + 8));
                }
                collision::BoxSetCacheWord(h.base, area + kBoxP, i, p);
              }
              t.r3.u64 = area + kHitsP;
              t.r4.u64 = area + kBoxP;
              if (f.kind == kGather) {
                // The record: the template and both copies the box.
                guest::StoreU32(h.base, area + kRecordP + 0, area + kBoxP);
                guest::StoreU32(h.base, area + kRecordP + 4, area + kBoxP);
                guest::StoreU32(h.base, area + kRecordP + 8, area + kBoxP);
                guest::StoreU32(h.base, area + kQueryP + 12, area + kRecordP);
                // The positions' cells within the mesh.
                for (uint32_t i = 0; i < 4; i++) {
                  const uint32_t p = area + kPositionsP + 96 * i;
                  guest::StoreU16(h.base, p + 0, uint16_t(r.Below(2)));
                  guest::StoreU16(h.base, p + 2, uint16_t(r.Below(kSlices)));
                }
                t.r3.u64 = area + kQueryP;
                t.r4.u64 = area + kMeshP;
              } else if (f.kind == kAddCell) {
                const uint32_t listed = r.Below(6);
                guest::StoreU32(h.base, area + kQueryP + 108, listed);
                for (uint32_t i = 0; i < listed; i++) {
                  guest::StoreU16(h.base, area + kQueryP + 112 + 4 * i, uint16_t(r.Below(2)));
                  guest::StoreU16(h.base, area + kQueryP + 114 + 4 * i, uint16_t(r.Below(4)));
                }
                t.r3.u64 = area + kQueryP;
                t.r4.u64 = (r.Below(2) << 16) | r.Below(4);
              }
              break;
            }
            case kContains:
            case kFindSlice:
            case kCross:
            case kOutcode:
            case kNearest:
            case kSearch:
            case kFindCell: {
              out_unaligned = false;
              // The point's height near the slices' (within twice the
              // tolerance), the position's slice in range (or, for
              // CrossSection, one past either end).
              const float y = 0.5f + (float(r.Below(2001)) / 1000.0f - 1.0f) * 2.0f * tolerance;
              guest::StoreFloat(h.base, point + 4, y);
              what = fmt::format("seed {} point ({}, {}, {}) position ({}, {})", seed, px, y, pz,
                                 section, slice);
              if (slice < 0 && section >= 0) {
                slice = int32_t(r.Below(kSlices));
                guest::StoreU16(h.base, position + 2, uint16_t(slice));
              }
              if (f.kind == kCross && r.Below(3) == 0) {
                guest::StoreU16(h.base, position + 2, uint16_t(r.Below(2) == 0 ? -1 : kSlices));
              }
              const uint32_t sec_slice = sec + kSlicesO + 28 * r.Below(kSlices);
              switch (f.kind) {
                case kContains:
                  t.r3.u64 = sec_slice;
                  t.r4.u64 = sec + kOutlineO;
                  t.r5.u64 = point;
                  break;
                case kFindSlice:
                  t.r3.u64 = sec;
                  t.r4.u64 = r.Below(3) == 0 ? uint32_t(-1) : r.Below(kSlices + 4);
                  t.r5.u64 = point;
                  break;
                case kCross:
                case kSearch:
                  if (section < 0 && f.kind == kCross) {
                    guest::StoreU16(h.base, position + 0, uint16_t(r.Below(2)));
                  }
                  t.r3.u64 = position;
                  t.r4.u64 = f.kind == kCross ? area + kMeshP : point;
                  t.r5.u64 = f.kind == kCross ? point : area + kMeshP;
                  break;
                case kOutcode:
                  t.r3.u64 = area + kMeshP;
                  t.r4.u64 = point;
                  t.r5.u64 = r.Below(2);
                  break;
                case kNearest:
                  guest::StoreFloat(h.base, area + kOutsP,
                                    r.Below(3) == 0 ? 1e30f : float(r.Below(100000)) / 100.0f);
                  t.r3.u64 = sec;
                  t.r4.u64 = point;
                  t.r5.u64 = area + kOutsP;
                  break;
                default:
                  t.r3.u64 = area + kMeshP;
                  t.r4.u64 = position;
                  t.r5.u64 = point;
                  break;
              }
              break;
            }
            case kGround:
              out_unaligned = false;
              std::memcpy(h.base + tc::kMesh, h.base + area + kMeshP, 12);
              t.r3.u64 = area + kResultP;
              t.r4.u64 = point;
              t.r5.u64 = r.Below(8) == 0 ? 0 : position;
              break;
          }
          what = fmt::format("seed {} point ({}, {}, {}) position ({}, {})", seed, px, py, pz,
                             section, slice);
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v].assign(h.base + area, h.base + area + kGroundCompared);
          {
            const std::vector<uint8_t> tried = Read(h.base, kLinkedTried, kLinkedTriedBytes);
            result[v].insert(result[v].end(), tried.begin(), tried.end());
            std::memcpy(h.base + kLinkedTried, saved_tried, kLinkedTriedBytes);
          }
          Append32(result[v], t.r3.u32);
          if (compare_f1) {
            const uint64_t bits = std::bit_cast<uint64_t>(t.f1.f64);
            Append32(result[v], uint32_t(bits >> 32));
            Append32(result[v], uint32_t(bits));
          }
          if (f.kind == kGround || f.kind == kHits) {
            std::memcpy(h.base + tc::kMesh, saved_mesh, sizeof(saved_mesh));
            guest::StoreU32(h.base, kWorldPointer, saved_world);
          }
        }
        if (!result[0].empty() || !result[1].empty()) {
          (out_unaligned ? unaligned : results).Check(result[0], result[1], what);
        }
      }
      results.Report();
      if (f.kind == kNormal || f.kind == kQuad || f.kind == kGroundHeight) {
        unaligned.Report();
      }
    }
  }
  std::memcpy(h.base + tc::kMesh, saved_mesh, sizeof(saved_mesh));
}

// --- The wall query's hit records, the segments' crossing and the route's
// loading: the area filled anew for each case, globals they read (the
// collision world's pointer, the race data's pointer 0x826C5B2C) set for
// each case and put back after. ---

void WallCases(Context& h, uint32_t area) {
  enum WallKind {
    kAddHit,     // r3 = a query (+0 its count, +8 the records), r4 = surface bits
    kIntersect,  // r3 = four point pointers, r4 = out
    kRoute,      // r6 = the route, its file built at +0x100
  };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    WallKind kind;
  } functions[] = {
      {"WallQuery::AddHit (sub_820FC2E0)", Original<__imp__sub_820FC2E0>, NativeAddHit, kAddHit},
      {"Segments::Intersect2D (sub_820FC658)", Original<__imp__sub_820FC658>, NativeIntersect2D,
       kIntersect},
      {"TrackRoute::OnLoaded (sub_820F9050)", Original<__imp__sub_820F9050>, NativeRouteOnLoaded,
       kRoute},
  };
  constexpr uint32_t kRaceDataPointer = 0x826C5B2C;
  constexpr uint32_t kWallCompared = 0x2000;
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceDataPointer);
  enum Group { kNumbers, kOneNaN };
  const char* const kGroupNames[] = {"", ", one NaN"};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN}) {
      if (group == kOneNaN && f.kind == kAddHit) {
        continue;  // copies and products only
      }
      Results results(std::string(f.name) + kGroupNames[group]);
      for (uint64_t seed = 0; seed < 200; seed++) {
        std::vector<uint8_t> result[2];
        for (int v = 0; v < 2; v++) {
          Random r = Seeded(seed * 59 + 23 * uint64_t(group) + 5);
          {
            std::vector<uint32_t> words(kWallCompared / 4);
            for (uint32_t& w : words) {
              w = __builtin_bswap32(uint32_t(r.Next()));
            }
            std::memcpy(h.base + area, words.data(), kWallCompared);
          }
          auto coordinate = [&r]() { return float(int32_t(r.Below(2001)) - 1000) / 200.0f; };
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          switch (f.kind) {
            case kAddHit:
              guest::StoreU32(h.base, kWorldPointer, area + 0x1000);
              guest::StoreU32(h.base, area + 0x1000 + 60, r.Below(5));
              guest::StoreU32(h.base, area + 0, r.Below(6));
              guest::StoreU32(h.base, area + 8, area + 0x200);
              for (uint32_t i = 48; i < 104; i += 4) {
                guest::StoreFloat(h.base, area + i, coordinate());
              }
              t.r3.u64 = area;
              t.r4.u64 = r.Below(2) == 0 ? 0x01000000 : uint32_t(r.Next());
              break;
            case kIntersect: {
              // Four points (x, z), a point shared or the segments
              // parallel one time in eight each.
              for (uint32_t i = 0; i < 4; i++) {
                const uint32_t p = area + 0x100 + 16 * i;
                guest::StoreFloat(h.base, p + 0, coordinate());
                guest::StoreFloat(h.base, p + 8, coordinate());
                guest::StoreU32(h.base, area + 4 * i, p);
              }
              switch (r.Below(8)) {
                case 0:
                  guest::StoreU32(h.base, area + 4 * (2 + r.Below(2)),
                                  guest::LoadU32(h.base, area + 4 * r.Below(2)));
                  break;
                case 1: {
                  const float dx = coordinate(), dz = coordinate();
                  const uint32_t a = area + 0x100, c = area + 0x120;
                  guest::StoreFloat(h.base, a + 16, guest::LoadFloat(h.base, a + 0) + dx);
                  guest::StoreFloat(h.base, a + 24, guest::LoadFloat(h.base, a + 8) + dz);
                  guest::StoreFloat(h.base, c + 16, guest::LoadFloat(h.base, c + 0) + 2.0f * dx);
                  guest::StoreFloat(h.base, c + 24, guest::LoadFloat(h.base, c + 8) + 2.0f * dz);
                  break;
                }
                default:
                  break;
              }
              if (group == kOneNaN) {
                guest::StoreU32(h.base, area + 0x100 + 16 * r.Below(4) + 8 * r.Below(2),
                                0x7FC00000u | (uint32_t(r.Next()) & 0x80000001u));
              }
              t.r3.u64 = area;
              t.r4.u64 = area + 0x180;
              break;
            }
            case kRoute: {
              const uint32_t file = area + 0x100;
              const uint32_t n0 = 1 + r.Below(4), n1 = r.Below(4);
              guest::StoreU32(h.base, area + 0, file);
              guest::StoreU32(h.base, file + 0, n0);
              guest::StoreU32(h.base, file + 4, n1);
              guest::StoreU32(h.base, file + 12, 0x40);
              guest::StoreU32(h.base, file + 16, 0x60);
              guest::StoreU32(h.base, file + 20, 0x80);
              for (uint32_t i = 0; i < n0; i++) {
                guest::StoreU32(h.base, file + 0x40 + 4 * i, 0x200 + 0x200 * i);
                for (uint32_t k = 0; k < 3; k++) {
                  const uint32_t e = file + 0x200 + 0x200 * i + 84 * k;
                  guest::StoreFloat(h.base, e + 148, coordinate());
                  guest::StoreFloat(h.base, e + 152, coordinate());
                }
              }
              for (uint32_t i = 0; i < n1; i++) {
                guest::StoreU32(h.base, file + 0x60 + 4 * i, 4 * r.Below(0x100));
              }
              for (uint32_t k = 0; k < 12; k++) {
                const uint32_t list = 0xC00 + 0x40 * k;
                guest::StoreU32(h.base, file + 0x80 + 4 * k, list);
                const uint32_t items = r.Below(4);
                guest::StoreU16(h.base, file + list + 0, uint16_t(items));
                guest::StoreU16(h.base, file + list + 2, uint16_t(r.Below(2)));
                for (uint32_t i = 0; i < items; i++) {
                  const uint32_t item = file + list + 4 + 8 * i;
                  guest::StoreU16(h.base, item + 0,
                                  uint16_t(r.Below(8) == 0 ? -1 : int32_t(r.Below(n0))));
                  guest::StoreU16(h.base, item + 2,
                                  uint16_t(r.Below(8) == 0 ? -1 : int32_t(r.Below(3))));
                  guest::StoreFloat(h.base, item + 4, coordinate());
                }
              }
              if (group == kOneNaN) {
                guest::StoreU32(h.base, file + 0xC08 + 0x40 * r.Below(12) + 8 * r.Below(3),
                                0x7FC00000u | (uint32_t(r.Next()) & 0x80000001u));
              }
              guest::StoreU32(h.base, kRaceDataPointer, area + 0x1800);
              guest::StoreU32(h.base, area + 0x1800 + 24, uint32_t(r.Next()));
              t.r6.u64 = area;
              break;
            }
          }
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v].assign(h.base + area, h.base + area + kWallCompared);
          Append32(result[v], t.r3.u32);
          guest::StoreU32(h.base, kWorldPointer, saved_world);
          guest::StoreU32(h.base, kRaceDataPointer, saved_race);
        }
        results.Check(result[0], result[1], fmt::format("seed {}", seed));
      }
      results.Report();
    }
  }
}

}  // namespace

// The wall query's cases (collision_world_walls.cpp).
void CollisionWorldWalls(Context& h, uint32_t area);

void CollisionWorld(Context& h) {
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  SmallCases(h, area);
  LifetimeCases(h, area);
  UnloadCases(h);
  WallCases(h, area);
  GroundCases(h, area);
  CollisionWorldWalls(h, area);
  memory::Free(area);
}

}  // namespace harness
