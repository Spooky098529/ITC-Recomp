// The power graph (src/power_graph/, docs/functions/power-graph.md) against
// the recompiled PowerGraph::Draw (sub_8217C708, with the recompiled
// Line::RecordFlat sub_8217B508 under it). A world of its own in a 48 KiB
// block: the render manager and its getters (the game's: sub_8248CA78
// returns [r3 + 284], sub_82379320 [r3 + 124]), a render queue with the
// game's vtable (RenderQueue::Add records each line in its pass list), the
// line recorder's command list (and now and then another one as
// Primitive::RecordTransform's, or a chunk open: the original's lines
// then), and a car data. The globals they reach ([0x826BABA0],
// [0x826D83D0] to +12, the open chunk count) point into it for the case
// and are put back after. Both versions run on the same bytes; the block,
// Draw's frame (the sprites and the curve, below the saved registers),
// Primitive::RecordTransform's product below Line::RecordFlat's frame, the
// globals and r3 are compared, exactly but for the floats the code stores
// (the sprites' corners, the curve, the lines' transform rows and vertex
// positions). Segment counts stay at 34 or less: a count far past 32 would
// have the original record lines for ever. TuningScreen::DrawPowerGraph
// (sub_821DF350) runs on a second block (the movie table's pairs, movie 2
// and its animation lists, the screen, the front end's car holder, with
// [0x826C5CCC] and [0x826D83E0] pointed into it), the original with
// --native_power_graph off (so its PowerGraph::Draw is the original too).
// CarList::DrawEntryPowerGraph (sub_82250670) runs on a third block: a
// garage car Car::SetModelAndClass, Car::Refresh and Vehicle::SetupFromCar
// can take (a car spec made of car_physics' Vehicle::SetupFromCar cases'
// car, WriteSetupFromCarCase, as the car makers' harness lays it, with the
// tuning words and tires Car::SetModel copies from the spec in range), the
// entry naming it and the race data AddRacer adds to; the original with
// --native_power_graph off (its PowerGraph::Draw the original too; its other
// callees through their adapters, as the car makers' harness runs them, so
// both sides' callees leave the same stack words under Draw's sprites).
// Compared: the render block and Draw's frame as
// above, its own frame (the car data as floats), its saves in its caller's
// frame, the globals, the garage block, the race data, rand's seed, r1, r3
// and r31; not the 64 bytes under its r1 (PowerGraph::Draw's register saves
// over its callees' saves, which the native ones don't write).

#include "harness.h"

#include "base/flags.h"

#include "command_list/command_list.h"
#include "graphics/render_queue.h"
#include "car_makers/car_makers.h"
#include "car_specs/car_specs.h"
#include "career/career.h"
#include "hooks/front_end_car_draw.h"
#include "hooks/heap.h"
#include "hooks/power_graph.h"
#include "memory/heap.h"
#include "power_graph/power_graph.h"
#include "race_setup/race_setup.h"

#include <fmt/format.h>

#include <bit>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_power_graph);

namespace harness {

// Vehicle::SetupFromCar's cases' writer (src/harnesses/car_physics.cpp).
void WriteSetupFromCarCase(uint8_t* base, uint32_t data, uint32_t car, uint32_t r1,
                           uint32_t stack_end, uint64_t seed, int group);

namespace {

namespace pg = power_graph;

constexpr uint32_t kBlockBytes = 0xC000;
// The block's layout.
constexpr uint32_t kManager = 0x0000;  // the render manager (+284 the scene)
constexpr uint32_t kManagerVtable = 0x0200;
constexpr uint32_t kRenderScene = 0x0300;  // +284 the list, +124 the queue
constexpr uint32_t kRenderSceneVtable = 0x0500;
constexpr uint32_t kQueue = 0x0600;
constexpr uint32_t kState = 0x0640;      // 88 bytes
constexpr uint32_t kPassListA = 0x06A0;  // 36 bytes each
constexpr uint32_t kPassListB = 0x06D0;
constexpr uint32_t kItems = 0x0700;      // 64 items of 20 bytes
constexpr uint32_t kItemCount = 64;
constexpr uint32_t kBuckets = 0x0C00;    // 700 buckets of 16 bytes
constexpr uint32_t kBucketCount = 700;
constexpr uint32_t kList = 0x3800;       // the scene's command list, 56 bytes
constexpr uint32_t kOtherList = 0x3840;  // another, now and then RecordTransform's
constexpr uint32_t kCar = 0x3900;        // the car data (to +1424)
constexpr uint32_t kSlots = 0x4000;      // the list's slots (up to 800)
constexpr uint32_t kOtherSlots = 0x8000;

// The globals the world replaces.
constexpr uint32_t kRenderManager = 0x826BABA0;
constexpr uint32_t kLists = 0x826D83D0;  // the rectangles' pair and the lines'
struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {kRenderManager, 4},
    {kLists, 16},
    {command_list::kOpenChunks, 4},
};

// A line's slots, and where its floats are (bytes from its first slot):
// the transform's rows, the two vertices' x, y, z.
constexpr uint32_t kLineSlots = 12;

uint32_t RandomFloat(Random& r) {
  switch (r.Below(12)) {
    case 0: return 0x7FC00000u | uint32_t(r.Next() & 0x3FFFFF);  // a quiet NaN
    case 1: return 0x7F800001u | uint32_t(r.Next() & 0x3FFFFF);  // a signaling NaN
    case 2: return r.Below(2) ? 0x7F800000u : 0xFF800000u;       // infinities
    case 3: return 0x80000000u;                                    // -0.0
    case 4: return uint32_t(r.Next());
    case 5: return std::bit_cast<uint32_t>(1.0f);
    case 6: return 0;
    default: return std::bit_cast<uint32_t>(float(int32_t(r.Below(20001)) - 10000) / 10.0f);
  }
}

// A command list at `list` with its slots at `slots`: `used` slots used and
// room for `room` more; `kind` 0 none open, 1 a chunk open on it, 2 another
// open (the count).
void MakeList(uint8_t* base, uint32_t list, uint32_t slots, uint32_t used, uint32_t room,
              uint32_t kind) {
  guest::StoreU32(base, list + 0, command_list::kVtable);
  guest::StoreU32(base, list + 8, used + room + 1);
  guest::StoreU32(base, list + 12, used);
  guest::StoreU32(base, list + 32, slots);
  guest::StoreU32(base, list + 44, slots + 16 * (kind == 1 ? 1 : 0));
  guest::StoreU8(base, list + 52, kind == 1 ? 1 : 0);
  if (kind != 0) {
    guest::StoreU32(base, command_list::kOpenChunks, 1);
  }
}

// The block filled with `r`'s words, then the world.
void MakeWorld(Context& h, uint32_t block, Random& r) {
  for (uint32_t at = 0; at < kBlockBytes; at += 4) {
    guest::StoreU32(h.base, block + at, uint32_t(r.Next()));
  }
  auto store = [&](uint32_t offset, uint32_t value) {
    guest::StoreU32(h.base, block + offset, value);
  };
  store(kManager, block + kManagerVtable);
  store(kManagerVtable + 32, 0x8248CA78);  // slot 8: [r3 + 284]
  store(kManager + 284, block + kRenderScene);
  store(kRenderScene, block + kRenderSceneVtable);
  store(kRenderSceneVtable + 4, 0x8248CA78);  // slot 1: [r3 + 284]
  store(kRenderSceneVtable + 8, 0x82379320);  // slot 2: [r3 + 124]
  store(kRenderScene + 284, block + kList);
  store(kRenderScene + 124, block + kQueue);
  store(kQueue + 0, graphics::render_queue::kVtable);
  store(kQueue + 4, block + kState);
  store(kQueue + 8, block + kPassListA);
  store(kQueue + 12, block + kPassListB);
  store(kState + 0, r.Below(2));
  store(kState + 84, std::bit_cast<uint32_t>(float(r.Below(1000)) / 10.0f));
  for (const uint32_t pass_list : {kPassListA, kPassListB}) {
    store(pass_list + 8, kItemCount);
    store(pass_list + 12, kBucketCount);
    store(pass_list + 16, r.Below(4));
    store(pass_list + 20, block + kItems);
    store(pass_list + 24, block + kBuckets);
  }
  for (uint32_t at = 0; at < 16 * kBucketCount; at += 4) {
    store(kBuckets + at, 0);
  }
  guest::StoreU32(h.base, kRenderManager, block + kManager);
  // The rectangles' pair: random; RecordTransform's list the scene's.
  guest::StoreU32(h.base, kLists + 0, uint32_t(r.Next()));
  guest::StoreU32(h.base, kLists + 4, block + kList);
  guest::StoreU32(h.base, kLists + 8, uint32_t(r.Next()));
  guest::StoreU32(h.base, kLists + 12, uint32_t(r.Next()));
  guest::StoreU32(h.base, command_list::kOpenChunks, 0);
}


// The car data: the rev limit by `pick` (the count), the torque curve and
// its scale.
void MakeCar(uint8_t* base, uint32_t car, Random& r) {
  uint32_t rev;
  switch (r.Below(16)) {
    case 0: rev = std::bit_cast<uint32_t>(12400.0f); break;  // 32 exactly
    case 1: rev = std::bit_cast<uint32_t>(12800.0f + float(r.Below(1200))); break;  // 33, 34
    case 2: rev = std::bit_cast<uint32_t>(float(r.Below(801))); break;              // 1 to 3
    case 3: rev = std::bit_cast<uint32_t>(-float(r.Below(100000))); break;          // none
    case 4: rev = 0x7FC00000u | uint32_t(r.Next() & 0x3FFFFF); break;              // NaN: none
    case 5: rev = r.Below(2) ? 0x80000000u : 0; break;                              // 1
    case 6: rev = 0xFF800000u; break;                                               // -inf
    default: rev = std::bit_cast<uint32_t>(3000.0f + float(r.Below(940000)) / 100.0f);
  }
  guest::StoreU32(base, car + 1280, rev);
  const bool game = r.Below(3) != 0;
  for (uint32_t i = 0; i < 32; i++) {
    const uint32_t torque =
        game ? std::bit_cast<uint32_t>(float(int32_t(r.Below(70000)) - 5000) / 100.0f)
             : RandomFloat(r);
    guest::StoreU32(base, car + 1292 + 4 * i, torque);
  }
  guest::StoreU32(base, car + 1420,
                  game ? std::bit_cast<uint32_t>(float(50 + r.Below(150)) / 100.0f)
                       : RandomFloat(r));
}

// The lists for a draw with `count` segments: room for its lines (just,
// now and then); now and then a chunk open on the list or another, or
// RecordTransform's list another (the original's lines then). Returns
// whether the lines are all on kSlots, 12 slots each, natively (their
// floats compared as floats); `lines` their number.
bool MakeLists(Context& h, uint32_t block, Random& r, int32_t count, uint32_t* lines) {
  *lines = count > 1 ? 2 * uint32_t(count - 1) : 0;
  const uint32_t used = r.Below(4) == 0 ? r.Below(300) : r.Below(20);
  const uint32_t pick = r.Below(10);
  const uint32_t kind = pick == 0 ? 1 + r.Below(2) : 0;
  const bool other = pick == 1;
  const uint32_t need = kLineSlots * *lines;
  const uint32_t room = r.Below(4) == 0 ? need : need + r.Below(100);
  MakeList(h.base, block + kList, block + kSlots, used, room, kind);
  if (other) {
    MakeList(h.base, block + kOtherList, block + kOtherSlots, r.Below(50),
             5 * *lines + r.Below(20), 0);
    guest::StoreU32(h.base, kLists + 4, block + kOtherList);
  }
  return kind == 0 && !other && count <= 32;
}

// A draw's outputs: the block (the lines' floats as floats when
// `float_slots`), Draw's frame at `frame` up to its saved registers (the
// sprites' corners and the curve as floats), Primitive::RecordTransform's
// product (and RenderQueue::Add's frame words over it) below
// Line::RecordFlat's frame.
void Collect(Context& h, uint32_t block, uint32_t frame, uint32_t lines, bool float_slots,
             Outputs& out) {
  uint32_t at = 0;
  auto floats = [&](uint32_t offset, uint32_t count) {
    out.Bytes(Read(h.base, block + at, offset - at));
    out.Floats(h.base, block + offset, count);
    at = offset + 4 * count;
  };
  if (float_slots) {
    for (uint32_t line = 0; line < lines; line++) {
      const uint32_t first = kSlots + 16 * kLineSlots * line;
      floats(first + 32, 16);
      floats(first + 128, 3);
      floats(first + 152, 3);
    }
  }
  out.Bytes(Read(h.base, block + at, kBlockBytes - at));
  out.Bytes(Read(h.base, frame, 112));
  out.Floats(h.base, frame + 112, 64);
  for (uint32_t i = 0; i < 64; i++) {
    const uint32_t sprite = frame + pg::kPowerSprites + pg::kSpriteBytes * i;
    out.Bytes(Read(h.base, sprite, 80));
    out.Floats(h.base, sprite + 80, 8);
    out.Bytes(Read(h.base, sprite + 112, 32));
  }
  out.Bytes(Read(h.base, frame - pg::kLineFrame - 80, 64));
}

void DrawCases(Context& h, uint32_t block) {
  Results results("PowerGraph::Draw (sub_8217C708) with Line::RecordFlat (sub_8217B508)");
  for (int c = 0; c < 300; c++) {
    std::vector<std::vector<uint8_t>> saved;
    for (const Global& g : kGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random r = start;
      Fill(h.base, h.data, kScratchBytes);
      MakeWorld(h, block, r);
      MakeCar(h.base, block + kCar, r);
      const int32_t count = pg::SegmentCount(h.base, block + kCar);
      uint32_t lines = 0;
      const bool float_slots = MakeLists(h, block, r, count, &lines);
      // The position: the callers' (1042, 520), (1042, 500), (1036, 508),
      // or random floats, or doubles that aren't singles.
      double x, y;
      switch (r.Below(6)) {
        case 0: x = 1042.0; y = 520.0; break;
        case 1: x = 1042.0; y = 500.0; break;
        case 2: x = 1036.0; y = 508.0; break;
        case 3:
          x = double(std::bit_cast<float>(RandomFloat(r)));
          y = double(std::bit_cast<float>(RandomFloat(r)));
          break;
        case 4:
          x = std::bit_cast<double>(r.Next());
          y = std::bit_cast<double>(r.Next());
          break;
        default:
          x = double(r.Below(1280)) + double(r.Below(1000)) / 997.0;
          y = double(r.Below(720)) + double(r.Below(1000)) / 991.0;
      }
      const uint32_t r3 = uint32_t(r.Next());
      after = r;
      PPCContext t = CallContext(h);
      Fill(h.base, t.r1.u32 - 12288, 12288 + 64, 0xA5);
      t.r3.u64 = r3;
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = block + kCar;
      t.f1.f64 = x;
      t.f2.f64 = y;
      (v == 0 ? __imp__sub_8217C708 : hooks::PowerGraphDraw)(t, h.base);
      Collect(h, block, t.r1.u32 - pg::kDrawFrame, lines, float_slots, out[v]);
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    for (size_t i = 0; i < saved.size(); i++) {
      Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The tuning screen's world, in a second block: the movie table's pairs,
// movie 2 (its looping list and queue: empty, or one slot naming animation
// 5 of sub-scene -1 or another, its time above, at or below 22.0, the
// no-time float, or random), the screen (its state, +9656, +9660, its car
// data at +18912) and the front end's car holder.
constexpr uint32_t kTuningBlockBytes = 0x10000;
constexpr uint32_t kMovie = 0x0000;     // 40,976 bytes; its lists at +0x90B4 and +0x9640
constexpr uint32_t kPairs = 0xA040;     // up to 4 pairs
constexpr uint32_t kHolder = 0xA080;    // the car holder's first block
constexpr uint32_t kScreen = 0xA100;    // to +18912 + 1424
constexpr uint32_t kMovieTable = 0x826C5CCC;
constexpr uint32_t kFrontEndCars = 0x826D83E0;
constexpr Global kTuningGlobals[] = {{kMovieTable, 16}, {kFrontEndCars, 4}};

// One animation list at `list`: empty, or one slot (its index, sub-scene
// and time).
void MakeAnimations(uint8_t* base, uint32_t list, bool one, uint32_t index, uint32_t sub_scene,
                    uint32_t time) {
  guest::StoreU16(base, list + 0, 0xFFFF);
  guest::StoreU16(base, list + 2, 0);
  guest::StoreU32(base, list + 140 + 0, index);
  guest::StoreU32(base, list + 140 + 4, sub_scene);
  guest::StoreU32(base, list + 140 + 20, time);
  guest::StoreU16(base, list + 128, 0xFFFF);
  guest::StoreU16(base, list + 130, 0xFFFF);
  guest::StoreU16(base, list + 132, one ? 0 : 0xFFFF);
  guest::StoreU16(base, list + 134, 0xFFFF);
  guest::StoreU16(base, list + 136, one ? 1 : 0);
}

void TuningCases(Context& h, uint32_t block, uint32_t tuning) {
  Results results("TuningScreen::DrawPowerGraph (sub_821DF350)");
  for (int c = 0; c < 300; c++) {
    std::vector<std::vector<uint8_t>> saved;
    for (const Global& g : kGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    for (const Global& g : kTuningGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random r = start;
      Fill(h.base, h.data, kScratchBytes);
      MakeWorld(h, block, r);
      for (uint32_t at = 0; at < kTuningBlockBytes; at += 4) {
        guest::StoreU32(h.base, tuning + at, uint32_t(r.Next()));
      }
      const uint32_t screen = tuning + kScreen;
      MakeCar(h.base, screen + 18912, r);
      const int32_t count = pg::SegmentCount(h.base, screen + 18912);
      uint32_t lines = 0;
      const bool float_slots = MakeLists(h, block, r, count, &lines);
      // The table: movie 2 among up to four pairs.
      const uint32_t pairs = 1 + r.Below(4);
      const uint32_t at = r.Below(pairs);
      for (uint32_t i = 0; i < pairs; i++) {
        guest::StoreU32(h.base, tuning + kPairs + 8 * i, i == at ? 2 : 3 + r.Below(20));
        guest::StoreU32(h.base, tuning + kPairs + 8 * i + 4,
                        i == at ? tuning + kMovie : uint32_t(r.Next()));
      }
      guest::StoreU32(h.base, kMovieTable + 0, tuning + kPairs);
      guest::StoreU32(h.base, kMovieTable + 8, pairs);
      // The lists: animation 5's time in one of them, or neither.
      uint32_t time;
      switch (r.Below(7)) {
        case 0: time = std::bit_cast<uint32_t>(22.0f); break;
        case 1: time = std::bit_cast<uint32_t>(22.5f + float(r.Below(100))); break;
        case 2: time = std::bit_cast<uint32_t>(float(r.Below(22))); break;
        case 3: time = guest::LoadU32(h.base, 0x82001BE0); break;
        case 4: time = 0x7FC00000u | uint32_t(r.Next() & 0x3FFFFF); break;
        default: time = RandomFloat(r);
      }
      const uint32_t where = r.Below(4);  // 0 looping, 1 queue, 2 neither, 3 another key
      const uint32_t sub_scene = where == 3 && r.Below(2) ? 0 : 0xFFFFFFFFu;
      const uint32_t index = where == 3 && sub_scene != 0 ? 4 : 5;
      MakeAnimations(h.base, tuning + kMovie + 0x9640, where == 0 || where == 3, index,
                     sub_scene, time);
      MakeAnimations(h.base, tuning + kMovie + 0x90B4, where == 1, 5, 0xFFFFFFFFu, time);
      // The screen: state 5, 6 or another; +9656 0 mostly; +9660 0 to 3 or
      // any byte.
      const uint32_t state_pick = r.Below(8);
      guest::StoreU32(h.base, screen + 9652,
                      state_pick < 3 ? 5 : state_pick < 6 ? 6 : uint32_t(r.Below(10)));
      guest::StoreU32(h.base, screen + 9656, r.Below(4) == 0 ? uint32_t(r.Next()) : 0);
      guest::StoreU8(h.base, screen + 9660, r.Below(4) == 0 ? uint8_t(r.Next()) : r.Below(4));
      guest::StoreU32(h.base, kFrontEndCars, tuning + kHolder);
      after = r;
      PPCContext t = CallContext(h);
      Fill(h.base, t.r1.u32 - 12288 - 512, 12288 + 512 + 64, 0xA5);
      t.r3.u64 = screen;
      ITC_FLAG(native_power_graph) = v == 1;
      (v == 0 ? __imp__sub_821DF350 : hooks::TuningScreenDrawPowerGraph)(t, h.base);
      ITC_FLAG(native_power_graph) = true;
      // Its frame (the key at +80) with its saves in the caller's (lr, r30,
      // r31), then the draw's; r30 and r31 kept.
      const uint32_t frame = t.r1.u32 - pg::kTuningFrame;
      out[v].Bytes(Read(h.base, frame, pg::kTuningFrame));
      for (const uint64_t kept : {t.r30.u64, t.r31.u64}) {
        out[v].Word(uint32_t(kept >> 32));
        out[v].Word(uint32_t(kept));
      }
      Collect(h, block, frame - pg::kDrawFrame, lines, float_slots, out[v]);
      out[v].Bytes(Read(h.base, tuning, kTuningBlockBytes));
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      for (const Global& g : kTuningGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    size_t k = 0;
    for (const Global& g : kGlobals) {
      Write(h.base, g.address, saved[k++].data(), g.bytes);
    }
    for (const Global& g : kTuningGlobals) {
      Write(h.base, g.address, saved[k++].data(), g.bytes);
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The car list's entry's world, in a third block: the template car (a
// Vehicle::SetupFromCar case's), the case writer's data and stack, the
// parameters and the one car spec, the entry and the front end's car
// holder; the race data in a block of its own.
constexpr uint32_t kGarageBytes = 0x10000;
constexpr uint32_t kTemplateCar = 0x4000;  // 4,096 bytes
constexpr uint32_t kWriterData = 0x5000;   // the writer's car data (1,856 bytes)
constexpr uint32_t kWriterStack = 0x9000;  // the writer's r1 (its stack below)
constexpr uint32_t kGarageParams = 0x9100; // 468 bytes
constexpr uint32_t kSpec = 0xA000;         // 8,432 bytes
constexpr uint32_t kEntry = 0xC400;        // to +8436
constexpr uint32_t kEntryHolder = 0xE600;
constexpr uint32_t kCarId = 5;
constexpr uint32_t kSpecBytes = 8432;
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRaceDataBytes = 0x5A000;
constexpr uint32_t kCarDataBytes = 1856;
constexpr Global kEntryGlobals[] = {{pg::kEntryShown, 4},
                                    {kFrontEndCars, 4},
                                    {car_specs::kParams, 4},
                                    {kRaceData, 4},
                                    {race_setup::kRandSeed, 4}};

// The template car and its spec: a SetupFromCar case's car (group 0
// numbers, 1 one NaN), its engine's +20 giving the graph -1 to 33 points
// (or a NaN: none), the words Car::SetModel copies from the spec (+788,
// +792: the car's +1700 and +1704; +884 to +896: its tires' types and
// grades) in the ranges the case writer gives them; the spec made of the
// car's blocks for stage 0 and those tires. `points_forced`, when not
// negative, sets the points (the count is points + 1) instead of the random pick.
void LayEntryGarage(uint8_t* base, uint32_t garage, Random& r, int c, int points_forced = -1) {
  const uint32_t car = garage + kTemplateCar, spec = garage + kSpec;
  WriteSetupFromCarCase(base, garage + kWriterData, car, garage + kWriterStack,
                        garage + kWriterStack, uint64_t(c) * 11 + 5, c % 2);
  const uint32_t engine = car + 2880;
  if (r.Below(10) == 0 && points_forced < 0) {
    guest::StoreU32(base, engine + 20, 0x7FC00000u);
  } else {
    const int32_t pick = int32_t(r.Below(35)) - 1;
    const int32_t points = points_forced >= 0 ? points_forced : pick;
    const double scale = double(guest::LoadFloat(base, 0x82002CE8));
    guest::StoreU32(base, engine + 20,
                    std::bit_cast<uint32_t>(float((double(points) + 0.5) / scale)));
  }
  const uint32_t block2 = car + 1888;
  const uint32_t picks_1704[] = {0, uint32_t(-1 - int32_t(r.Below(4))), 1, 2, 3};
  guest::StoreU32(base, block2 + 148, r.Below(3));
  guest::StoreU32(base, block2 + 152, picks_1704[r.Below(5)]);
  uint32_t tire[2];
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t type = r.Below(5), grade = 1 + r.Below(5);
    guest::StoreU32(base, block2 + 244 + 8 * k, type);
    guest::StoreU32(base, block2 + 248 + 8 * k, grade);
    tire[k] = (type == 0 ? 0 : type * 5 + grade - 5) * 92 + spec + (k == 0 ? 4244 : 6176);
  }
  auto copy = [&](uint32_t to, uint32_t from, uint32_t bytes) {
    Write(base, to, Read(base, from, bytes).data(), bytes);
  };
  copy(spec, car + 2240, 640);
  copy(spec + 640, car + 1888, 340);
  copy(spec + 980, car + 2880, 204);
  copy(spec + 8108, car + 3364, 272);
  copy(tire[0], car + 3084, 92);
  copy(tire[1], car + 3176, 92);
  copy(spec + 8380, car + 3268, 48);
  const uint32_t params = garage + kGarageParams;
  guest::StoreU32(base, params + 12, spec - kSpecBytes * kCarId);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, params + 16 + 4 * i, spec);
  }
  guest::StoreU32(base, garage + kEntry + 8432, kCarId);
}

// Cases 48 to 55 have the graph shown with 33 or 34 segments: past 32,
// PowerGraph::Draw's lines read its saved registers as sprites, r31 (the
// original's lis, sign-extended) among them.
void EntryCases(Context& h, uint32_t block, uint32_t garage, uint32_t race) {
  Results results("CarList::DrawEntryPowerGraph (sub_82250670)");
  for (int c = 0; c < 56; c++) {
    const bool past_32 = c >= 48;
    std::vector<std::vector<uint8_t>> saved;
    for (const Global& g : kGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    for (const Global& g : kEntryGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random r = start;
      Fill(h.base, h.data, kScratchBytes);
      MakeWorld(h, block, r);
      for (uint32_t at = 0; at < kGarageBytes; at += 4) {
        guest::StoreU32(h.base, garage + at, uint32_t(r.Next()));
      }
      LayEntryGarage(h.base, garage, r, c, past_32 ? 32 + c % 2 : -1);
      const int32_t count =
          pg::SegmentCount(h.base, garage + kTemplateCar + 2880 + 20 - 1280);
      const bool shown = r.Below(8) != 0 || past_32;
      uint32_t lines = 0;
      const bool float_slots = MakeLists(h, block, r, shown ? count : 0, &lines);
      guest::StoreU32(h.base, pg::kEntryShown, shown ? 1 + r.Below(3) : 0);
      guest::StoreU32(h.base, kFrontEndCars, garage + kEntryHolder);
      guest::StoreU32(h.base, car_specs::kParams, garage + kGarageParams);
      Fill(h.base, race, kRaceDataBytes, 0);
      guest::StoreU32(h.base, race + 48 + 0x595E0, r.Below(6));
      guest::StoreU32(h.base, race + 48 + 0x595E4, r.Below(3));
      guest::StoreU32(h.base, kRaceData, race);
      guest::StoreU32(h.base, race_setup::kRandSeed, uint32_t(r.Next()));
      const uint64_t r31 = r.Next();
      after = r;
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      const uint32_t sp = t.r1.u32;
      Fill(h.base, sp - 16384, 16384 + 64, 0xA5);
      t.r3.u64 = garage + kEntry;
      t.r31.u64 = r31;
      t.fpscr.disableFlushModeUnconditional();
      ITC_FLAG(native_power_graph) = v == 1;
      (v == 0 ? __imp__sub_82250670 : hooks::CarListDrawEntryPowerGraph)(t, h.base);
      ITC_FLAG(native_power_graph) = true;
      const uint32_t frame = sp - pg::kEntryFrame;
      Collect(h, block, frame - pg::kDrawFrame, lines, float_slots, out[v]);
      out[v].Bytes(Read(h.base, frame, pg::kEntryCarData));
      out[v].Floats(h.base, frame + pg::kEntryCarData, kCarDataBytes / 4);
      out[v].Bytes(Read(h.base, frame + pg::kEntryCar, pg::kEntryFrame - pg::kEntryCar));
      out[v].Bytes(Read(h.base, sp - 16, 80));
      out[v].Bytes(Read(h.base, garage, kGarageBytes));
      out[v].Bytes(Read(h.base, race, kRaceDataBytes));
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      for (const Global& g : kEntryGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(uint32_t(t.r31.u64));
      out[v].Word(uint32_t(t.r31.u64 >> 32));
    }
    size_t k = 0;
    for (const Global& g : kGlobals) {
      Write(h.base, g.address, saved[k++].data(), g.bytes);
    }
    for (const Global& g : kEntryGlobals) {
      Write(h.base, g.address, saved[k++].data(), g.bytes);
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// CarList::DrawCarPowerGraph (sub_82250700) and PowerGraph::SetFromCar
// (sub_82250488) on the entry's world: the garage car (at the garage
// block's +0xE800) made as DrawEntryPowerGraph makes its own
// (GarageCar::Reset and Car::SetModelAndClass with the entry's id, their
// plain C++), then passed in r3. DrawCarPowerGraph: as the entry's cases,
// the original with --native_power_graph off. SetFromCar: the power graph
// screen's record ([0x82815EBC] + 8, in the garage block at +0xF000) filled
// with r4 a tab word; the original's callees through their adapters, the
// native's through their plain C++ (Native<Name> below: it has no
// adapter); compared: the record's block, its frame (the car object's
// bytes, the car data and the curve's tables as floats), the stack below
// it, the garage block, rand's seed and r3.
constexpr uint32_t kGarageCar = 0xE800;  // 1,968 bytes
constexpr uint32_t kScreenBlock = 0xF000;
constexpr uint32_t kRecordBytes = 92;

void NativeSetFromCar(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const front_end_car_draw::Calls calls = hooks::FrontEndCarDrawCalls(ctx, base);
  const uint32_t curve = pg::SetFromCar(
      base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32,
      [&calls](uint32_t function, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t frame,
               uint32_t back) {
        const uint64_t regs[3] = {r3, r4, r5};
        calls.call(function, regs, 3, nullptr, 0, frame, back, nullptr);
      });
  ctx.r3.u64 = (ctx.r1.u64 & 0xFFFFFFFF00000000ull) + curve;
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
}

// The garage car made from the entry's id (the entry's world laid).
void MakeGarageCar(Context& h, uint32_t garage, uint32_t sp) {
  PPCContext t = CallContext(h);
  const front_end_car_draw::Calls calls = hooks::FrontEndCarDrawCalls(t, h.base);
  const uint32_t car = garage + kGarageCar;
  car_makers::GarageCarReset(h.base, calls, car, sp);
  career::CarSetModelAndClass(h.base, car, kCarId, sp);
}

void CarCases(Context& h, uint32_t block, uint32_t garage, uint32_t race, bool set_from_car) {
  Results results(set_from_car ? "PowerGraph::SetFromCar (sub_82250488)"
                               : "CarList::DrawCarPowerGraph (sub_82250700)");
  constexpr uint32_t kScreen = pg::kPowerGraphScreen;
  for (int c = 0; c < 32; c++) {
    std::vector<std::vector<uint8_t>> saved;
    for (const Global& g : kGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    for (const Global& g : kEntryGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    saved.push_back(Read(h.base, kScreen, 4));
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random r = start;
      Fill(h.base, h.data, kScratchBytes);
      MakeWorld(h, block, r);
      for (uint32_t at = 0; at < kGarageBytes; at += 4) {
        guest::StoreU32(h.base, garage + at, uint32_t(r.Next()));
      }
      LayEntryGarage(h.base, garage, r, c);
      const int32_t count =
          pg::SegmentCount(h.base, garage + kTemplateCar + 2880 + 20 - 1280);
      const bool shown = r.Below(8) != 0;
      uint32_t lines = 0;
      const bool float_slots =
          MakeLists(h, block, r, shown && !set_from_car ? count : 0, &lines);
      guest::StoreU32(h.base, pg::kEntryShown, shown ? 1 + r.Below(3) : 0);
      guest::StoreU32(h.base, kFrontEndCars, garage + kEntryHolder);
      guest::StoreU32(h.base, car_specs::kParams, garage + kGarageParams);
      guest::StoreU32(h.base, kScreen, garage + kScreenBlock);
      Fill(h.base, race, kRaceDataBytes, 0);
      guest::StoreU32(h.base, race + 48 + 0x595E0, r.Below(6));
      guest::StoreU32(h.base, race + 48 + 0x595E4, r.Below(3));
      guest::StoreU32(h.base, kRaceData, race);
      guest::StoreU32(h.base, race_setup::kRandSeed, uint32_t(r.Next()));
      const uint64_t r31 = r.Next();
      const uint32_t tab = r.Below(4) == 0 ? uint32_t(r.Next()) : 2 + r.Below(4);
      after = r;
      PPCContext t = CallContext(h);
      t.r1.u64 &= ~0xFull;
      const uint32_t sp = t.r1.u32;
      Fill(h.base, sp - 16384, 16384 + 64, 0xA5);
      MakeGarageCar(h, garage, sp - 8192);
      Fill(h.base, sp - 16384, 16384 + 64, 0xA5);
      t.r3.u64 = garage + kGarageCar;
      t.r4.u64 = tab;
      t.r31.u64 = r31;
      t.fpscr.disableFlushModeUnconditional();
      if (set_from_car) {
        (v == 0 ? __imp__sub_82250488 : NativeSetFromCar)(t, h.base);
        const uint32_t frame = sp - pg::kSetFromCarFrame;
        out[v].Bytes(Read(h.base, sp - 16384, frame - (sp - 16384)));
        out[v].Bytes(Read(h.base, frame, 80));
        out[v].Floats(h.base, frame + 80, 1024 / 4);  // the curve's tables
        out[v].Bytes(Read(h.base, frame + 80 + 1024, 8));
        out[v].Floats(h.base, frame + 80 + 1032, 8);
        out[v].Bytes(Read(h.base, frame + 80 + 1064, 1152 - 80 - 1064));
        out[v].Floats(h.base, frame + 1152, kCarDataBytes / 4);
        out[v].Bytes(Read(h.base, frame + 3008, pg::kSetFromCarFrame - 3008 - 40));
        const uint32_t record = garage + kScreenBlock + 8;
        out[v].Bytes(Read(h.base, record, 52));
        out[v].Floats(h.base, record + 52, 1);
        out[v].Bytes(Read(h.base, record + 56, kRecordBytes - 56));
      } else {
        ITC_FLAG(native_power_graph) = v == 1;
        (v == 0 ? __imp__sub_82250700 : hooks::CarListDrawCarPowerGraph)(t, h.base);
        ITC_FLAG(native_power_graph) = true;
        const uint32_t frame = sp - pg::kCarFrame;
        Collect(h, block, frame - pg::kDrawFrame, lines, float_slots, out[v]);
        out[v].Bytes(Read(h.base, frame, pg::kEntryCarData));
        out[v].Floats(h.base, frame + pg::kEntryCarData, kCarDataBytes / 4);
        out[v].Bytes(Read(h.base, frame + pg::kEntryCarData + kCarDataBytes,
                          pg::kCarFrame - pg::kEntryCarData - kCarDataBytes - 16));
        out[v].Bytes(Read(h.base, sp - 16, 80));
        out[v].Bytes(Read(h.base, race, kRaceDataBytes));
        out[v].Word(uint32_t(t.r31.u64));
        out[v].Word(uint32_t(t.r31.u64 >> 32));
      }
      out[v].Bytes(Read(h.base, garage, kGarageBytes));
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      for (const Global& g : kEntryGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    size_t k = 0;
    for (const Global& g : kGlobals) {
      Write(h.base, g.address, saved[k++].data(), g.bytes);
    }
    for (const Global& g : kEntryGlobals) {
      Write(h.base, g.address, saved[k++].data(), g.bytes);
    }
    Write(h.base, kScreen, saved[k++].data(), 4);
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void PowerGraph(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const uint32_t block = memory::Alloc(kBlockBytes, memory::kHeapZeroMemory);
  if (block == 0) {
    return;
  }
  DrawCases(h, block);
  const uint32_t tuning = memory::Alloc(kTuningBlockBytes, memory::kHeapZeroMemory);
  if (tuning != 0) {
    TuningCases(h, block, tuning);
    memory::Free(tuning);
  }
  const uint32_t garage = memory::Alloc(kGarageBytes, memory::kHeapZeroMemory);
  const uint32_t race = memory::Alloc(kRaceDataBytes, memory::kHeapZeroMemory);
  if (garage != 0 && race != 0) {
    EntryCases(h, block, garage, race);
    CarCases(h, block, garage, race, false);
    CarCases(h, block, garage, race, true);
  }
  if (race != 0) {
    memory::Free(race);
  }
  if (garage != 0) {
    memory::Free(garage);
  }
  memory::Free(block);
}

}  // namespace harness
