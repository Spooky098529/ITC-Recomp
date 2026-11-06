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

#include "harness.h"

#include "base/flags.h"

#include "command_list/command_list.h"
#include "graphics/render_queue.h"
#include "hooks/heap.h"
#include "hooks/power_graph.h"
#include "memory/heap.h"
#include "power_graph/power_graph.h"

#include <fmt/format.h>

#include <bit>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_power_graph);

namespace harness {

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
  memory::Free(block);
}

}  // namespace harness
