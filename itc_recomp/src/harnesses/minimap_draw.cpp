// Minimap::Draw (sub_820F5740, src/minimap/draw.cpp) against the recompiled
// one, under the minimap harness, with a random generator of its own (it
// runs last, so its cases shift no other function's inputs). Its callees run
// natively on both sides (their own harnesses check them).
//
// The world, in one allocation (the same addresses for both versions,
// filled with the same bytes before each): the race data (its kind +0xA3AF0
// 1, 6 or another, its flags +20, the point lists' course, mode and flags,
// the stages with the current one's +0 1 or not); the scenery manager and
// its course set (the list at +1064 of up to four models, their +28 0 so
// the map's draws stop at it: SceneryModel::DrawShape's own harness has its
// draws; their shapes zeros, the records' list ours); the point list at +3180 with up to eight entries of four points);
// the pass description; the race manager with up to eight entries (the
// player's, 0x8 ones, racers with flag 0x10 or no record, skipped ones), the
// racers' records and vehicles (their matrices), or no vehicles; the route's
// four sections of 16 slices; the triangles' render queue (its slot 4
// BillboardMaterial::SetLights, as the marker draws' harness) and command
// list. The car markers' colors made again (their guard cleared) one case
// in three, the blink clock and the human flag ([0x826C7824]) at random.
//
// Compared, exactly: the allocation after the race data, the globals (the
// camera passes, the keys, the view copy, the blink clock, the colors and
// the markers' tables, the lists' pointers), the frame's back chain and
// temporaries (up to its saved registers' slots: +28,128) and the exit
// table's additions. r3 isn't (its caller doesn't read it).

#include "harness.h"
#include "exit_table.h"

#include "base/log.h"
#include "c_runtime/exit.h"
#include "hooks/c_runtime.h"
#include "hooks/heap.h"
#include "hooks/minimap.h"
#include "memory/heap.h"
#include "scenery_lists/scenery_lists.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <mutex>
#include <vector>

namespace harness {

namespace {

constexpr uint32_t kFrame = 28336, kTemps = 28336 - 208;
constexpr uint32_t kRaceBytes = 0xA4000;
// After the race data.
constexpr uint32_t kManager = 0x0000, kSet = 0x0100, kModels = 0x1000, kPoints = 0x2000;
constexpr uint32_t kDescription = 0x2400, kRaceManager = 0x3000, kRacers = 0x3C00;
constexpr uint32_t kVehicles = 0x4000, kVehicleBytes = 6272;
constexpr uint32_t kRoute = 0x10800, kSections = 0x10900, kSlices = 0x11000;
constexpr uint32_t kQueue = 0x14000, kList = 0x14100, kSlots = 0x20000, kSlotCount = 8192;
constexpr uint32_t kShapes = 0x12000, kRecordList = 0x14200, kRecordSlots = 0x18000;
constexpr uint32_t kOtherBytes = 0x40000;
constexpr uint32_t kCameraPasses = 0x826BAC00;

struct Area {
  uint32_t from, bytes;
};
constexpr Area kGlobals[] = {
    {kCameraPasses, 21 * 592}, {0x8296B8F0, 0x30}, {0x8265ECA0, 0x40}, {0x826C5AA0, 0x70},
    {0x826C5520, 0x160},       {0x826C5680, 0x430}, {0x826D82F0, 0x10}, {0x826D83D0, 0x10},
    {0x826C2E30, 0x10},        {0x826C5B10, 0x60}, {0x826C7820, 0x10}, {0x8296BD20, 0x10},
};

float Uniform(Random& r, float low, float high) {
  return low + (high - low) * float(r.Below(1u << 24)) / float(1u << 24);
}
void StoreFloat(uint8_t* base, uint32_t address, float value) {
  guest::StoreU32(base, address, std::bit_cast<uint32_t>(value));
}
void Floats(uint8_t* base, uint32_t at, uint32_t count, Random& r, float scale) {
  for (uint32_t i = 0; i < count; i++) {
    StoreFloat(base, at + 4 * i, Uniform(r, -scale, scale));
  }
}

struct World {
  uint32_t race = 0, other = 0;
  uint32_t pass = 0, entries = 0, points = 0;
  bool vehicles = true, colors_again = false;
  uint32_t r24 = 0;
  uint64_t seed = 0;
};

void Build(Context& h, const World& w) {
  uint8_t* base = h.base;
  Random r(w.seed);
  const uint32_t race = w.race, o = w.other;
  Fill(base, race, kRaceBytes + kOtherBytes);
  auto at = [&](uint32_t offset, uint32_t value) { guest::StoreU32(base, o + offset, value); };

  // The race data.
  static constexpr uint32_t kKinds[] = {1, 1, 6, 6, 2, 0};
  guest::StoreU32(base, race + 0xA3AF0, kKinds[r.Below(6)]);
  guest::StoreU32(base, race + 20, uint32_t(r.Next()));
  guest::StoreU32(base, race + 4, r.Below(8));
  static constexpr uint32_t kModes[] = {4, 4, 7, 3, 1, 0};
  guest::StoreU32(base, race + scenery_lists::kRaceMode, kModes[r.Below(6)]);
  guest::StoreU32(base, race + scenery_lists::kRaceFlags, r.Below(6) == 0 ? 0x80 : 0);
  const uint32_t stages = race + scenery_lists::kStages;
  const uint32_t current = r.Below(10);
  guest::StoreU32(base, stages + 804, current);
  guest::StoreU32(base, stages + 80 * current, r.Below(3));

  // The scenery manager, the course set: its map list, its point list.
  const uint32_t set = o + kSet;
  at(kManager + 112, set);
  const uint32_t maps = r.Below(5);
  guest::StoreU32(base, set + 1064, maps);
  const uint32_t nodes = set + 1068;
  guest::StoreU16(base, nodes + 260, 1);
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU16(base, nodes + 4 * (1 + i), uint16_t(2 + i));
    guest::StoreU16(base, nodes + 4 * (1 + i) + 2, uint16_t(3 - i));
    const uint32_t model = o + kModels + 0x200 * i;
    guest::StoreU32(base, nodes + 268 + 4 * i, model);
    guest::StoreU32(base, model + 28, 0);
    guest::StoreU32(base, model + 412, r.Below(4));
    guest::StoreU32(base, model + 40, o + kShapes);
  }
  // The models' shapes (what SceneryModel::DrawShape counts its slots from
  // before it tests +28): zeros, their materials' counts 0.
  Fill(base, o + kShapes, 4 * 544 + 64, 0);
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, o + kShapes + 544 * k + 396, o + kShapes + 4 * 544);
  }
  // The scenery records' command list ([0x8296B918]), empty.
  at(kRecordList + 8, 1000);
  at(kRecordList + 12, 0);
  at(kRecordList + 32, o + kRecordSlots);
  at(kRecordList + 44, o + kRecordSlots);
  guest::StoreU8(base, o + kRecordList + 52, 0);
  guest::StoreU32(base, scenery_lists::kRecordList, o + kRecordList);
  const uint32_t list = set + 3180;
  guest::StoreU32(base, list + 620, w.points);
  for (uint32_t e = 0; e < 8; e++) {
    const uint32_t points = o + kPoints + 0x100 * (e % 4);
    guest::StoreU32(base, list + 528 + 4 * e, points);
  }
  for (uint32_t p = 0; p < 64; p++) {
    StoreFloat(base, o + kPoints + 4 * p,
               p % 4 == 1 ? (r.Below(4) == 0 ? -2000.0f : Uniform(r, -50, 50))
                          : Uniform(r, -500, 500));
  }

  // The pass description.
  at(kDescription + 20, w.pass);
  at(kDescription + 24, uint32_t(r.Next()));
  at(kDescription + 28, uint32_t(r.Next()));

  // The race manager: the player's entry first, then others.
  const uint32_t manager = o + kRaceManager;
  at(kRaceManager + 2368, w.entries);
  for (uint32_t i = 0; i < w.entries; i++) {
    const uint32_t entry = manager + 4 + 28 * i;
    static constexpr uint32_t kFlags[] = {0x8, 0x8, 0x11, 0x11, 0x1, 0x21, 0x0, 0x4};
    guest::StoreU32(base, entry, i == 0 ? 0x4 : kFlags[r.Below(8)]);
    guest::StoreU16(base, entry + 8, r.Below(10) == 0 ? 0xFFFF : r.Below(4));
    guest::StoreU16(base, entry + 10, r.Below(10) == 0 ? 0xFFFF : r.Below(16));
    const uint32_t record = r.Below(4);
    guest::StoreU32(base, entry + 24, record);
    // No record (-1): RacerMarkerType's type 1 without the save data.
    guest::StoreU32(base, race + 48 + 3936 * record + 3912, 0xFFFFFFFFu);
    const uint32_t racer = o + kRacers + 32 * i;
    guest::StoreU32(base, manager + 2308 + 4 * i, racer);
    guest::StoreU32(base, racer + 20, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(8));
  }
  // The player's section in its word +8's high halfword.
  guest::StoreU16(base, manager + 4 + 8, r.Below(4));
  for (uint32_t v = 0; v < 8; v++) {
    const uint32_t m = o + kVehicles + kVehicleBytes * v + 3312;
    const float a = Uniform(r, -3.14159f, 3.14159f);
    const float values[16] = {std::cos(a), 0, -std::sin(a), 0, 0, 1, 0, 0, std::sin(a), 0,
                              std::cos(a), 0, Uniform(r, -500, 500), Uniform(r, -5, 5),
                              Uniform(r, -500, 500), 1};
    for (uint32_t k = 0; k < 16; k++) {
      StoreFloat(base, m + 4 * k, values[k]);
    }
  }

  // The route: four sections of 16 slices (84 bytes), the branch at +3.
  at(kRoute + 12, o + kSections);
  for (uint32_t s = 0; s < 4; s++) {
    const uint32_t section = o + kSlices + 0x600 * s;
    at(kSections + 4 * s, section);
    for (uint32_t k = 0; k < 16; k++) {
      Floats(base, section + 84 * k + 124, 13, r, 300.0f);
    }
    guest::StoreU8(base, section + 3, uint8_t(1 + r.Below(4)));
  }

  // The triangles' render queue and command list.
  at(kQueue, o + kQueue + 0x40);
  at(kQueue + 0x40 + 16, 0x8209F5C8);
  at(kList + 8, kSlotCount);
  at(kList + 12, 0);
  at(kList + 32, o + kSlots);
  at(kList + 44, o + kSlots);
  guest::StoreU8(base, o + kList + 52, 0);

  // The camera pass.
  const uint32_t pass = kCameraPasses + 592 * w.pass;
  Floats(base, pass + 16, 16, r, 2.0f);
  Floats(base, pass + 144, 16, r, 2.0f);

  // The globals.
  guest::StoreU32(base, 0x826C5B2C, race);
  guest::StoreU32(base, 0x826D82F4, manager);
  guest::StoreU32(base, 0x826C2E34, w.vehicles ? o + kVehicles : 0);
  guest::StoreU32(base, 0x826C5B10, o + kRoute);
  guest::StoreU32(base, 0x826D83D0, o + kQueue);
  guest::StoreU32(base, 0x826D83D4, o + kList);
  guest::StoreU32(base, 0x8296BD28, 0);
  guest::StoreU32(base, 0x826C7824, r.Below(2));
  guest::StoreU32(base, 0x826C5AA4, r.Below(6));
  guest::StoreU32(base, 0x826C5AA8, r.Below(7));
  if (w.colors_again) {
    guest::StoreU32(base, 0x826C5B00, guest::LoadU32(base, 0x826C5B00) & ~1u);
  }
}

void RecompiledDraw(PPCContext& ctx, uint8_t* base) { __imp__sub_820F5740(ctx, base); }

}  // namespace

// The cases (called by Minimap in minimap.cpp).
void MinimapDrawCases(Context& h) {
  if (!hooks::NativeHeapActive() || !hooks::MinimapDrawNative()) {
    ITC_LOG_INFO("harness: Minimap::Draw: skipped (no native heap or its flags off)");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  Random r(0x4D494E494D415044ull);  // "MINIMAPD"
  const uint32_t race = memory::AllocPages(kRaceBytes + kOtherBytes, 4096);
  if (race == 0) {
    ITC_LOG_ERROR("harness: Minimap::Draw: no memory for the world");
    return;
  }
  std::vector<std::vector<uint8_t>> saved;
  for (const Area& a : kGlobals) {
    saved.push_back(Read(h.base, a.from, a.bytes));
  }
  Results results("Minimap::Draw (sub_820F5740)");
  int markers = 0;
  for (int c = 0; c < 120; c++) {
    World w;
    w.race = race;
    w.other = race + kRaceBytes;
    w.pass = r.Below(21);
    w.entries = r.Below(9);
    w.points = r.Below(9);
    w.vehicles = r.Below(8) != 0 && w.entries != 0;
    w.colors_again = r.Below(3) == 0;
    w.r24 = uint32_t(r.Next());
    w.seed = r.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (size_t i = 0; i < saved.size(); i++) {
        Write(h.base, kGlobals[i].from, saved[i].data(), kGlobals[i].bytes);
      }
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      t.fpscr.disableFlushModeUnconditional();
      const uint32_t stack = t.r1.u32;
      Fill(h.base, stack - kFrame - 1024, kFrame + 1024);
      Build(h, w);
      t.r3.u64 = w.other + kManager;
      t.r4.u64 = w.other + kDescription;
      t.r24.u64 = w.r24;
      const uint32_t mark = exit_table::Mark(h.base);
      (v == 0 ? RecompiledDraw : hooks::MinimapDraw)(t, h.base);
      out[v].Bytes(Read(h.base, w.other, kOtherBytes));
      for (const Area& a : kGlobals) {
        out[v].Bytes(Read(h.base, a.from, a.bytes));
      }
      out[v].Bytes(Read(h.base, stack - kFrame, kTemps));
      exit_table::Take(h.base, mark, out[v]);
      if (v == 1) {
        markers += int(guest::LoadU32(h.base, w.other + kList + 12));
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: pass {} entries {} points {} vehicles {} colors {}", c,
                              w.pass, w.entries, w.points, w.vehicles, w.colors_again));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kGlobals[i].from, saved[i].data(), kGlobals[i].bytes);
  }
  ITC_LOG_INFO("harness: Minimap::Draw: {} triangle slots recorded (the native side's)", markers);
  results.Report();
  memory::FreePages(race);
}

}  // namespace harness
