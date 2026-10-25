// The wall query (src/track_collision/track_wall_query.cpp) against the
// recompiled functions: part of the collision_world harness
// (collision_world.cpp calls CollisionWorldWalls). A scene is built in the
// area for each case: a car-sized box (its template, its previous and
// current placed copies, the shape record), the box moving by up to 1.5
// m and turning a little, a fake collision world (its +60 the room for hit
// records, its pointer 0x826C2D88) and the hit records, and a wall near the
// box: long or (one time in four) short enough to stand inside it, turned
// any way, its ends' heights around the box's bottom, its normal facing
// either side. The race data (its pointer 0x826C5B2C) is a block of its own
// with the words the wall kinds read. Globals are put back after each case.
// The originals run with the native flags of their callees off, so the
// whole chain is the recompiled code's. Compared: the area's first
// kCompared bytes and r3. A line "harness coverage:" says how many cases
// recorded a hit.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "memory/heap.h"
#include "collision/collision.h"
#include "track_collision/track_collision.h"

#include <fmt/format.h>

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

// --- The register steps: the plain C++ with the registers the adapters
// leave. ---

void NativeModeSkips(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = tc::WallQueryModeSkips(base, ctx.r3.u32);
}
void NativeIgnoresWall(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      tc::WallQueryIgnoresWall(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 96);
}
void NativeCorners(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t hit = 0;
  if (!tc::WallQueryCornersThroughWall(base, ctx.r3.u32, ctx.r1.u32 - 304, &hit)) {
    __imp__sub_820FC3C8(ctx, base);
    return;
  }
  ctx.r3.u64 = hit;
}

void NativeEdges(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t hit = 0;
  if (!tc::WallQueryEdgesAcrossWall(base, ctx.r3.u32, ctx.r1.u32 - 944, &hit)) {
    __imp__sub_820FC900(ctx, base);
    return;
  }
  ctx.r3.u64 = hit;
}

void NativeTestCell(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!tc::WallQueryTestCell(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 256)) {
    __imp__sub_820FDC18(ctx, base);
  }
}
void NativeQueryWalls(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!tc::TrackCollisionQueryWalls(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 128)) {
    __imp__sub_820FDF38(ctx, base);
  }
}
void NativeWallHits(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t count = 0;
  if (!tc::TrackCollisionWallHits(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 1584, &count)) {
    __imp__sub_820FF1B0(ctx, base);
    return;
  }
  ctx.r3.u64 = count;
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

constexpr uint32_t kWorldPointer = 0x826C2D88;
constexpr uint32_t kRaceDataPointer = 0x826C5B2C;
constexpr uint32_t kGameStatePointer = 0x826D82E8;
constexpr uint32_t kRaceDataBytes = 0xA4000;

// The area's layout.
constexpr uint32_t kQueryP = 0x0,  // 1,472 bytes
    kTemplateP = 0x600, kPreviousP = 0x800, kCurrentP = 0xA00,  // 496 bytes each
    kRecordP = 0xC00, kWorldP = 0xC80, kHitsP = 0xD00,           // 32 records of 80
    kCornersP = 0x1700, kMatrixP = 0x1740, kMatrix2P = 0x1780, kPositionsP = 0x1800,
    kKindP = 0x1A00, kHalvesP = 0x1A10;
// The mesh (installed at 0x826C5B1C for WallHits): two sections of four
// slices, each with up to five walls between 20 vertices near the box.
constexpr uint32_t kMeshP = 0x1A40, kFileP = 0x1A60, kTableP = 0x1A80, kSectionP = 0x1C00,
                   kSectionBytes = 0x400, kSlices = 4, kVertices = 20;
constexpr uint32_t kVerticesO = 0x100, kSlicesO = 0x200, kWallsO = 0x280;
constexpr uint32_t kCompared = 0x2C00;
// The game state [0x826D82E8] (its +6100 read), past the compared bytes.
constexpr uint32_t kGameStateP = 0x3000;

float Coordinate(Random& r, float reach) {
  return (float(r.Below(20001)) / 10000.0f - 1.0f) * reach;
}

// The race data's words the wall kinds read: +4, +20, +24, the mode
// +0xA3AF4 (-1 to 10) and the stage list's flags +0x59620 + 1220; the game
// state's +6100 (0 to 4).
void SetRace(uint8_t* base, uint32_t race, uint32_t area, Random& r) {
  guest::StoreU32(base, race + 4, r.Below(26));
  guest::StoreU32(base, race + 20, uint32_t(r.Next()));
  guest::StoreU32(base, race + 24, uint32_t(r.Next()));
  guest::StoreU32(base, race + 0xA3AF4,
                  r.Below(3) == 0 ? 2u : uint32_t(int32_t(r.Below(12)) - 1));
  guest::StoreU32(base, race + 0x59620 + 1220, uint32_t(r.Next()) & 0x70);
  guest::StoreU32(base, kRaceDataPointer, race);
  guest::StoreU32(base, kGameStatePointer, area + kGameStateP);
  guest::StoreU32(base, area + kGameStateP + 6100, r.Below(5));
}

// A box: its template's half extents and the matrix placing a copy.
void PlaceBox(uint8_t* base, uint32_t box, uint32_t matrix, uint32_t source, float x, float y,
              float z, float yaw, float tilt, uint32_t frame) {
  const float c = std::cos(yaw), sn = std::sin(yaw), ct = std::cos(tilt), st = std::sin(tilt);
  const float m[16] = {c,       0.0f, -sn,    0.0f, sn * st, ct, c * st, 0.0f,
                       sn * ct, -st,  c * ct, 0.0f, x,       y,  z,      1.0f};
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreFloat(base, matrix + 4 * i, m[i]);
  }
  collision::BoxPlace(base, box, matrix, source, frame);
}

// The scene: the box moving, its record, the world and the hit records.
// Returns the current copy's center.
struct Scene {
  float x, y, z;
};
Scene BuildScene(uint8_t* base, uint32_t area, Random& r, uint32_t frame) {
  const uint32_t corners = area + kCornersP;
  const float sx = 0.6f + float(r.Below(600)) / 1000.0f;
  const float sy = 0.4f + float(r.Below(400)) / 1000.0f;
  const float sz = 1.5f + float(r.Below(1000)) / 1000.0f;
  const float lo[4] = {-sx, -sy, -sz, 0.0f}, hi[4] = {sx, sy, sz, 0.0f};
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreFloat(base, corners + 4 * i, lo[i]);
    guest::StoreFloat(base, corners + 16 + 4 * i, hi[i]);
  }
  collision::BoxConstruct(base, area + kTemplateP, corners, corners + 16, frame);
  collision::BoxConstruct(base, area + kPreviousP, corners, corners + 16, frame);
  collision::BoxConstruct(base, area + kCurrentP, corners, corners + 16, frame);
  guest::StoreU16(base, area + kTemplateP + 112, uint16_t(r.Below(4)));
  guest::StoreU16(base, area + kTemplateP + 114, uint16_t(r.Below(4)));
  const float x = Coordinate(r, 6.0f), y = 0.5f + Coordinate(r, 0.5f),
              z = 10.0f + Coordinate(r, 8.0f);
  const float yaw = float(r.Below(6283)) / 1000.0f;
  const float tilt = r.Below(2) == 0 ? 0.0f : Coordinate(r, 0.2f);
  // The motion: none in x and z one time in eight, else up to 1.5 m; a
  // turn of up to 0.2 one time in two.
  float mx = 0.0f, mz = 0.0f;
  if (r.Below(8) != 0) {
    const float heading = float(r.Below(6283)) / 1000.0f;
    const float speed = float(r.Below(1500)) / 1000.0f;
    mx = speed * std::cos(heading);
    mz = speed * std::sin(heading);
  }
  const float my = r.Below(2) == 0 ? 0.0f : Coordinate(r, 0.1f);
  const float turn = r.Below(2) == 0 ? 0.0f : Coordinate(r, 0.2f);
  PlaceBox(base, area + kPreviousP, area + kMatrixP, area + kTemplateP, x - mx, y - my, z - mz,
           yaw - turn, tilt, frame);
  PlaceBox(base, area + kCurrentP, area + kMatrix2P, area + kTemplateP, x, y, z, yaw, tilt,
           frame);
  guest::StoreU32(base, area + kRecordP + 0, area + kTemplateP);
  guest::StoreU32(base, area + kRecordP + 4, area + kPreviousP);
  guest::StoreU32(base, area + kRecordP + 8, area + kCurrentP);
  guest::StoreU32(base, kWorldPointer, area + kWorldP);
  guest::StoreU32(base, area + kWorldP + 60, r.Below(4) == 0 ? r.Below(4) : 32);
  return {x, y, z};
}

// The query's wall as TestCell sets it (in host floats): ends A (+16) and B
// (+32), the direction +1408 (and negated +1424), the normal +1440 facing
// either side, the ends along the direction +1456, +1460, the normal's
// distance +1464.
void SetWall(uint8_t* base, uint32_t query, float ax, float ay, float az, float bx, float by,
             float bz, bool flip) {
  const float a[4] = {ax, ay, az, 0.0f}, b[4] = {bx, by, bz, 0.0f};
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreFloat(base, query + 16 + 4 * i, a[i]);
    guest::StoreFloat(base, query + 32 + 4 * i, b[i]);
  }
  float dx = ax - bx, dy = ay - by, dz = az - bz;
  const float length = std::sqrt(dx * dx + dy * dy + dz * dz);
  if (length > 0.0f) {
    dx /= length;
    dy /= length;
    dz /= length;
  }
  // The direction from B to A one time in two (TestCell's is from B to A,
  // so +1456 is above +1460 in play).
  if ((std::bit_cast<uint32_t>(ax) & 1) != 0) {
    dx = -dx;
    dy = -dy;
    dz = -dz;
  }
  const float s = flip ? -1.0f : 1.0f;
  const float n[3] = {-dz * s, 0.0f, dx * s};
  const float d[3] = {dx, dy, dz};
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreFloat(base, query + 1408 + 4 * i, d[i]);
    guest::StoreFloat(base, query + 1424 + 4 * i, -d[i]);
    guest::StoreFloat(base, query + 1440 + 4 * i, n[i]);
  }
  guest::StoreFloat(base, query + 1420, 1.0f);
  guest::StoreFloat(base, query + 1452, 1.0f);
  guest::StoreFloat(base, query + 1456, d[0] * ax + d[1] * ay + d[2] * az);
  guest::StoreFloat(base, query + 1460, d[0] * bx + d[1] * by + d[2] * bz);
  guest::StoreFloat(base, query + 1464, n[0] * bx + n[1] * by + n[2] * bz);
}

// A wall near the box at `s`: long, or short one time in four.
void NearWall(uint8_t* base, uint32_t query, Random& r, const Scene& s) {
  const float cx = s.x + Coordinate(r, 2.5f), cz = s.z + Coordinate(r, 3.0f);
  const float angle = float(r.Below(6283)) / 1000.0f;
  const float length =
      r.Below(4) == 0 ? 0.2f + float(r.Below(1300)) / 1000.0f : 2.0f + float(r.Below(10000)) / 1000.0f;
  const float hx = 0.5f * length * std::cos(angle), hz = 0.5f * length * std::sin(angle);
  const float bottom = s.y - 0.5f;
  SetWall(base, query, cx + hx, bottom + Coordinate(r, 1.0f), cz + hz, cx - hx,
          bottom + Coordinate(r, 1.0f), cz - hz, r.Below(2) == 0);
}

// The query: +0 its hit count, +4 2.0, +8 the hit records, +12 the record,
// +624 the walls tested so far.
void SetQuery(uint8_t* base, uint32_t area, Random& r) {
  const uint32_t query = area + kQueryP;
  guest::StoreU32(base, query + 0, r.Below(3));
  guest::StoreFloat(base, query + 4, 2.0f);
  guest::StoreU32(base, query + 8, area + kHitsP);
  guest::StoreU32(base, query + 12, area + kRecordP);
  guest::StoreU32(base, query + 624, r.Below(6));
}

// A quiet NaN with a random sign and payload bit.
uint32_t NaN(Random& r) {
  return 0x7FC00000u | (uint32_t(r.Next()) & 0x80000001u);
}

void FillArea(uint8_t* base, uint32_t area, Random& r) {
  std::vector<uint32_t> words(kCompared / 4);
  for (uint32_t& w : words) {
    w = __builtin_bswap32(uint32_t(r.Next()));
  }
  std::memcpy(base + area, words.data(), kCompared);
}

// The mesh: the sections' links (none, or to the other section's slices),
// vertices near the box, each slice's walls (a wall again one time in
// eight, a zero-length one in sixteen; a kind one time in four: 2 to 63,
// or any halfword). The box's four track positions (its cache words) on
// cells of the mesh.
void BuildWallMesh(uint8_t* base, uint32_t area, Random& r, const Scene& s) {
  guest::StoreU32(base, area + kMeshP + 0, 0);
  guest::StoreU32(base, area + kMeshP + 4, area + kFileP);
  guest::StoreU32(base, area + kMeshP + 8, 2);
  guest::StoreU32(base, area + kFileP + 0, 2);
  guest::StoreU32(base, area + kFileP + 4, area + kTableP);
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t section = area + kSectionP + kSectionBytes * k;
    guest::StoreU32(base, area + kTableP + 4 * k, section);
    guest::StoreU16(base, section + 0, uint16_t(kSlices));
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t link = section + 4 + 6 * i;
      const bool linked = r.Below(3) == 0;
      guest::StoreU16(base, link + 0, linked ? uint16_t(r.Below(kSlices)) : uint16_t(0xFFFF));
      guest::StoreU16(base, link + 2, uint16_t(1 - k));
      guest::StoreU16(base, link + 4, uint16_t(r.Below(kSlices)));
    }
    guest::StoreU32(base, section + 64, kVerticesO);
    guest::StoreU32(base, section + 72, kSlicesO);
    guest::StoreU32(base, section + 80, kWallsO);
    for (uint32_t v = 0; v < kVertices; v++) {
      const uint32_t at = section + kVerticesO + 12 * v;
      guest::StoreFloat(base, at + 0, s.x + Coordinate(r, 4.0f));
      guest::StoreFloat(base, at + 4, s.y - 0.5f + Coordinate(r, 1.0f));
      guest::StoreFloat(base, at + 8, s.z + Coordinate(r, 4.0f));
    }
    uint32_t count = 0;
    for (uint32_t slice = 0; slice < kSlices; slice++) {
      const uint32_t at = section + kSlicesO + 28 * slice;
      const uint32_t walls = r.Below(6);
      guest::StoreU16(base, at + 4, uint16_t(walls));
      guest::StoreU16(base, at + 6, uint16_t(count));
      for (uint32_t w = 0; w < walls; w++, count++) {
        const uint32_t wall = section + kWallsO + 8 * count;
        uint32_t a = r.Below(kVertices), b = r.Below(kVertices);
        if (count > 0 && r.Below(8) == 0) {
          a = guest::LoadU16(base, wall - 8 + 0);
          b = guest::LoadU16(base, wall - 8 + 2);
        } else if (r.Below(16) == 0) {
          b = a;
        }
        guest::StoreU16(base, wall + 0, uint16_t(a));
        guest::StoreU16(base, wall + 2, uint16_t(b));
        uint32_t kind = 0;
        if (r.Below(4) == 0) {
          kind = r.Below(8) == 0 ? r.Below(0x10000) : 2 + r.Below(62);
        }
        guest::StoreU16(base, wall + 4, uint16_t(kind));
        guest::StoreU16(base, wall + 6, uint16_t(r.Next()));
      }
    }
  }
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t position = area + kPositionsP + 96 * i;
    guest::StoreU16(base, position + 0, uint16_t(r.Below(2)));
    guest::StoreU16(base, position + 2, uint16_t(r.Below(kSlices)));
    collision::BoxSetCacheWord(base, area + kCurrentP, i, position);
  }
}

// --- The wall kinds: ModeSkips and IgnoresWall ---

void KindCases(Context& h, uint32_t area, uint32_t race) {
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    bool ignores;
  } functions[] = {
      {"WallQuery::ModeSkips (sub_820FC870)", Original<__imp__sub_820FC870>, NativeModeSkips,
       false},
      {"WallQuery::IgnoresWall (sub_820FD6D8)", Original<__imp__sub_820FD6D8>, NativeIgnoresWall,
       true},
  };
  for (const auto& f : functions) {
    Results results(f.name);
    for (uint64_t seed = 0; seed < 600; seed++) {
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Random r = Seeded(seed * 31 + 7);
        FillArea(h.base, area, r);
        SetRace(h.base, race, area, r);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        // The kind: 0 to 63 (2 to 61 the switch's), or any halfword one
        // time in eight; the box's halfwords 0 to 3.
        const uint32_t kind = r.Below(8) == 0 ? r.Below(0x10000) : r.Below(64);
        guest::StoreU16(h.base, area + kKindP, uint16_t(kind));
        guest::StoreU16(h.base, area + kHalvesP + 0, uint16_t(r.Below(4)));
        guest::StoreU16(h.base, area + kHalvesP + 2, uint16_t(r.Below(4)));
        guest::StoreU32(h.base, area + kHalvesP + 4, r.Below(4));
        if (f.ignores) {
          t.r3.u64 = area + kKindP;
          t.r4.u64 = area + kHalvesP + 0;
          t.r5.u64 = area + kHalvesP + 2;
        } else {
          t.r3.u64 = area + kHalvesP + 4;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        result[v].assign(h.base + area, h.base + area + kCompared);
        Append32(result[v], t.r3.u32);
      }
      results.Check(result[0], result[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The box's corners and edges through the query's wall ---

void ThroughCases(Context& h, uint32_t area, uint32_t race) {
  const struct {
    const char* name;
    GuestFunction recompiled, native;
  } functions[] = {
      {"WallQuery::CornersThroughWall (sub_820FC3C8)", Original<__imp__sub_820FC3C8>,
       NativeCorners},
      {"WallQuery::EdgesAcrossWall (sub_820FC900)", Original<__imp__sub_820FC900>, NativeEdges},
  };
  enum Group { kNumbers, kOneNaN };
  const char* const kGroupNames[] = {"", ", one NaN"};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      int hits = 0;
      for (uint64_t seed = 0; seed < 400; seed++) {
        std::vector<uint8_t> result[2];
        for (int v = 0; v < 2; v++) {
          Random r = Seeded(seed * 41 + 13 * uint64_t(group) + 3);
          FillArea(h.base, area, r);
          SetRace(h.base, race, area, r);
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          const Scene s = BuildScene(h.base, area, r, t.r1.u32 - 4096);
          SetQuery(h.base, area, r);
          NearWall(h.base, area + kQueryP, r, s);
          if (group == kOneNaN && r.Below(2) == 0) {
            const uint32_t at = r.Below(3) == 0
                                    ? area + kQueryP + 16 + 16 * r.Below(2) + 4 * r.Below(3)
                                    : area + (r.Below(2) == 0 ? kPreviousP : kCurrentP) + 176 +
                                          16 * r.Below(8) + 4 * r.Below(3);
            guest::StoreU32(h.base, at, NaN(r));
          }
          t.r3.u64 = area + kQueryP;
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v].assign(h.base + area, h.base + area + kCompared);
          Append32(result[v], t.r3.u32);
          if (v == 0 && t.r3.u32 != 0) {
            hits++;
          }
        }
        results.Check(result[0], result[1], fmt::format("seed {}", seed));
      }
      results.Report();
      ITC_LOG_INFO("harness coverage: {}{}: {} of 400 cases with a hit", f.name,
                   kGroupNames[group], hits);
    }
  }
}

// --- The cells' walls: TestCell, QueryWalls and WallHits on the mesh ---

void QueryCases(Context& h, uint32_t area, uint32_t race) {
  enum QueryKind { kTestCell, kQueryWalls, kWallHits };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    QueryKind kind;
  } functions[] = {
      {"WallQuery::TestCell (sub_820FDC18)", Original<__imp__sub_820FDC18>, NativeTestCell,
       kTestCell},
      {"TrackCollision::QueryWalls (sub_820FDF38)", Original<__imp__sub_820FDF38>,
       NativeQueryWalls, kQueryWalls},
      {"TrackCollision::WallHits (sub_820FF1B0)", Original<__imp__sub_820FF1B0>, NativeWallHits,
       kWallHits},
  };
  uint8_t saved_mesh[12];
  std::memcpy(saved_mesh, h.base + tc::kMesh, sizeof(saved_mesh));
  // A NaN in a vertex (expected to differ): it makes the whole wall's
  // direction and normal NaNs (the normalize), which then meet the vertex's
  // own NaN in the normal's distance (+1464, normal . the wall's end): two
  // NaNs in one product, the one that comes out following the operand
  // order the compiler picked ("Two NaNs" in
  // docs/loops/expected-differences.md). The game's vertices are numbers.
  enum Group { kNumbers, kOneNaN };
  const char* const kGroupNames[] = {"", ", one NaN in a vertex (expected to differ)"};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      int hits = 0;
      for (uint64_t seed = 0; seed < 300; seed++) {
        std::vector<uint8_t> result[2];
        for (int v = 0; v < 2; v++) {
          Random r = Seeded(seed * 43 + 11 * uint64_t(group) + 9);
          FillArea(h.base, area, r);
          SetRace(h.base, race, area, r);
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          const Scene s = BuildScene(h.base, area, r, t.r1.u32 - 8192);
          SetQuery(h.base, area, r);
          BuildWallMesh(h.base, area, r, s);
          if (group == kOneNaN && r.Below(2) == 0) {
            guest::StoreU32(h.base,
                            area + kSectionP + kSectionBytes * r.Below(2) + kVerticesO +
                                12 * r.Below(kVertices) + 4 * r.Below(3),
                            NaN(r));
          }
          const uint32_t query = area + kQueryP;
          switch (f.kind) {
            case kTestCell: {
              // The walls tested so far: a few, or near the 128 kept, the
              // list holding some of the mesh's walls.
              const uint32_t tested = r.Below(4) == 0 ? 125 + r.Below(6) : r.Below(8);
              guest::StoreU32(h.base, query + 624, tested);
              for (uint32_t i = 0; i < tested && i < 128; i++) {
                if (r.Below(3) == 0) {
                  const uint32_t k = r.Below(2);
                  const uint32_t wall = area + kSectionP + kSectionBytes * k + kWallsO + 8 * r.Below(8);
                  guest::StoreU16(h.base, query + 628 + 6 * i, uint16_t(k));
                  guest::StoreU16(h.base, query + 630 + 6 * i, guest::LoadU16(h.base, wall + 0));
                  guest::StoreU16(h.base, query + 632 + 6 * i, guest::LoadU16(h.base, wall + 2));
                }
              }
              t.r3.u64 = query;
              t.r4.u64 = area + kMeshP;
              t.r5.u64 = (r.Below(2) << 16) | r.Below(kSlices);
              t.r6.u64 = 2;
              break;
            }
            case kQueryWalls:
              t.r3.u64 = query;
              t.r4.u64 = area + kMeshP;
              break;
            case kWallHits:
              std::memcpy(h.base + tc::kMesh, h.base + area + kMeshP, 12);
              t.r3.u64 = area + kHitsP;
              t.r4.u64 = area + kRecordP;
              break;
          }
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v].assign(h.base + area, h.base + area + kCompared);
          // TestCell's and QueryWalls' r3 isn't kept (their callers don't
          // read it); WallHits' is the hit count.
          if (f.kind == kWallHits) {
            Append32(result[v], t.r3.u32);
            std::memcpy(h.base + tc::kMesh, saved_mesh, sizeof(saved_mesh));
            if (v == 0 && t.r3.u32 != 0) {
              hits++;
            }
          } else if (v == 0) {
            // A hit recorded: a record's +56 the walls' surface bits.
            for (uint32_t i = 0; i < 32; i++) {
              if (guest::LoadU32(h.base, area + kHitsP + 80 * i + 56) == 0x01000000) {
                hits++;
                break;
              }
            }
          }
        }
        results.Check(result[0], result[1], fmt::format("seed {}", seed));
      }
      results.Report();
      ITC_LOG_INFO("harness coverage: {}{}: {} of 300 cases with a hit", f.name,
                   kGroupNames[group], hits);
    }
  }
  std::memcpy(h.base + tc::kMesh, saved_mesh, sizeof(saved_mesh));
}

}  // namespace

void CollisionWorldWalls(Context& h, uint32_t area) {
  const uint32_t race = memory::Alloc(kRaceDataBytes, 0);
  const uint32_t saved_world = guest::LoadU32(h.base, kWorldPointer);
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceDataPointer);
  const uint32_t saved_state = guest::LoadU32(h.base, kGameStatePointer);
  KindCases(h, area, race);
  guest::StoreU32(h.base, kWorldPointer, saved_world);
  guest::StoreU32(h.base, kRaceDataPointer, saved_race);
  guest::StoreU32(h.base, kGameStatePointer, saved_state);
  ThroughCases(h, area, race);
  QueryCases(h, area, race);
  guest::StoreU32(h.base, kWorldPointer, saved_world);
  guest::StoreU32(h.base, kRaceDataPointer, saved_race);
  guest::StoreU32(h.base, kGameStatePointer, saved_state);
  memory::Free(race);
}

}  // namespace harness
