// CarScene::Draw (sub_820C5DE8, src/car_scene_draw/scene_draw.cpp, the car
// scene's draw's root) against the recompiled one, under the car_scene_draw
// harness, with a random generator of its own (it runs last, so its cases
// shift no other function's inputs). The recompiled side runs with
// --native_car_scene_draw off (its callees the originals); the native side
// the register step hooks::CarSceneDrawRoot.
//
// The world, in one allocation (the same addresses for both versions, filled
// with the same bytes before each): the scene ([0x826C2E44]) with its state
// (+0; 4 now and then), path (+4), shared assets' slot (+84) and XPR (+88),
// texture provider (+432: its vtable's slot 3 the game's getter of +284,
// sub_8248CA78, answering an XPR or 0), +5044, +5052, the List at +5028 of
// up to nine scene models (in use or not, with a car model or not, traffic
// or not, low or high beams, their matrices) and the lights and beams
// (+5248 to +5487); the render manager ([0x826BABA0]: its slot 8 and the
// answer's slots 1 and 2 the game's getters) with a command list of 2,048
// slots and a render queue of the real class (its vtable 0x8206CB8C, its
// Add natively on both sides) with 512 items and 2,057 buckets in each pass
// list; the viewports ([0x826C2CE8], view modes 0 to 3); the race data
// ([0x826C5B2C], an allocation of its own: the race kind, the player's word,
// the current stage's word, the flags); [0x826C7824], [0x826032DC] (8 or
// not) and the render queue's keys [0x82601854] and [0x82601858]; r4 a
// matrix or 0. One case in ten has nine scene models in use and none
// traffic (the other cars' headlight slots run out), some the headlight's
// field of view NaN or infinite (Projector::Build's original runs) or
// passes' fields of view NaN (TextureProjection::Fit's). The camera passes
// are the game's (21 at 0x826BAC00, as they
// are when the harness runs), their active flags (+588) and types (+584, 0
// to 5) set at random in the second path (the first path disables and
// enables them itself, then records their begins and ends through the
// render manager: the game's types kept). The scene models' +5028 is 1, so
// Car::DrawUpdate (its own harness, car_draw) only takes off the boost
// (+4992) and returns.
//
// Compared exactly (the copies of floats are copies, the computed ones the
// same operations): the allocation (the scene with its +5072 to +5144, the
// models' +4296 and boosts, the command list and its chunks, the queue's
// state, items and buckets), the globals the root and its callees write
// (0x826BABA0 to the passes' end, with the passes' +144 and what the
// recording writes; 0x8296B8C0 to 0x8296BD40, with the shared texture, the
// keys, the scene lights, the headlight slots and the open chunks' count;
// 0x8265EC80 to 0x8265ED40), the root's frame (its back chain and every word
// up to its saved registers' slots, which the plain C++ doesn't write: the
// top 304 bytes) and the caller's +20 (the scene), and r3. Everything the
// case points at is put back after.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/car_scene_draw.h"
#include "hooks/heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <initializer_list>
#include <cmath>
#include <cstring>
#include <iterator>
#include <limits>
#include <tuple>
#include <optional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_scene_draw);

namespace harness {

namespace {

constexpr uint32_t kFrame = 1696;
constexpr uint32_t kSavedTop = 304;  // the saved registers' slots under the caller's r1
constexpr uint32_t kStackBytes = 8192;

// The allocation's layout.
constexpr uint32_t kScene = 0x0000;
constexpr uint32_t kManager = 0x1600, kManagerVtable = 0x1700;
constexpr uint32_t kObject = 0x1800, kObjectVtable = 0x1A00;
constexpr uint32_t kList = 0x1B00;
constexpr uint32_t kProviderVtable = 0x1B80;
constexpr uint32_t kQueue = 0x1C00, kState = 0x1C40, kListA = 0x1CC0, kListB = 0x1D00;
constexpr uint32_t kViewportArray = 0x1D80;
constexpr uint32_t kXpr2 = 0x1E00, kXpr2Records = 0x1E40;
constexpr uint32_t kXprRecords = 0x1F00;
constexpr uint32_t kMatrix = 0x1F80;
constexpr uint32_t kViewport0 = 0x2000, kViewport1 = 0x2800;
constexpr uint32_t kModels = 0x3000, kModelBytes = 0x1480, kMaxModels = 9;
constexpr uint32_t kSlots = 0xF000, kSlotCount = 2048;
constexpr uint32_t kItemsA = 0x17000, kItemsB = 0x19800, kItemCount = 512;
constexpr uint32_t kBucketsA = 0x1C000, kBucketsB = 0x24100, kBucketCount = 2057;
constexpr uint32_t kWorldBytes = 0x2D000;
constexpr uint32_t kRaceBytes = 0xA4000;

constexpr uint32_t kGetter284 = 0x8248CA78;  // returns [r3 + 284]
constexpr uint32_t kGetter124 = 0x82379320;  // returns [r3 + 124]
constexpr uint32_t kQueueVtable = 0x8206CB8C;
constexpr uint32_t kOpenChunks = 0x8296BD28;

// The global areas compared and put back.
struct Area {
  uint32_t from, to;
};
constexpr Area kAreas[] = {
    {0x826BABA0, 0x826BAC00 + 21 * 592},  // the render manager, kMode, the passes
    {0x8296B8C0, 0x8296BD40},             // the shared texture to the open chunks
    {0x8265EC80, 0x8265ED40},             // the view and eye copies, +5280's
};
constexpr uint32_t kWords[] = {0x826C2E44, 0x826C5B2C, 0x826C7820, 0x826C7824,
                               0x826C2CE8, 0x826032DC, 0x82601854, 0x82601858};

struct FlagOff {
  FlagOff() : saved_(ITC_FLAG(native_car_scene_draw)) { ITC_FLAG(native_car_scene_draw) = false; }
  ~FlagOff() { ITC_FLAG(native_car_scene_draw) = saved_; }
  bool saved_;
};

// The global areas and words as they were, put back on Restore and at the
// end.
class Globals {
 public:
  explicit Globals(uint8_t* base) : base_(base) {
    for (const Area& a : kAreas) {
      areas_.push_back(Read(base, a.from, a.to - a.from));
    }
    for (uint32_t w : kWords) {
      words_.push_back(guest::LoadU32(base, w));
    }
  }
  ~Globals() { Restore(); }
  void Restore() const {
    for (size_t i = 0; i < std::size(kAreas); i++) {
      Write(base_, kAreas[i].from, areas_[i].data(), uint32_t(areas_[i].size()));
    }
    for (size_t i = 0; i < std::size(kWords); i++) {
      guest::StoreU32(base_, kWords[i], words_[i]);
    }
  }

 private:
  uint8_t* base_;
  std::vector<std::vector<uint8_t>> areas_;
  std::vector<uint32_t> words_;
};

void StoreFloat(uint8_t* base, uint32_t address, float value) {
  guest::StoreU32(base, address, std::bit_cast<uint32_t>(value));
}
float Uniform(Random& r, float low, float high) {
  return low + (high - low) * float(r.Below(1u << 24)) / float(1u << 24);
}
// Mostly ordinary, now and then a special value.
float Value(Random& r, float scale) {
  switch (r.Below(60)) {
    case 0:
      return 0.0f;
    case 1:
      return -0.0f;
    case 2:
      return std::numeric_limits<float>::quiet_NaN();
    case 3:
      return std::numeric_limits<float>::infinity();
    case 4:
      return std::bit_cast<float>(uint32_t(r.Below(0x7FFFFF) + 1));
    default:
      return Uniform(r, -scale, scale);
  }
}
void Floats(uint8_t* base, uint32_t at, uint32_t count, Random& r, float scale) {
  for (uint32_t i = 0; i < count; i++) {
    StoreFloat(base, at + 4 * i, Value(r, scale));
  }
}
// A matrix as the scene models' (a turn about y, a position), sometimes
// random floats.
void Matrix(uint8_t* base, uint32_t at, Random& r) {
  if (r.Below(8) == 0) {
    Floats(base, at, 16, r, 100.0f);
    return;
  }
  const float a = Uniform(r, -3.14159f, 3.14159f);
  const float c = std::cos(a), s = std::sin(a);
  const float m[16] = {c,    0, -s,   0, 0, 1, 0, 0, s, 0, c, 0, Uniform(r, -500, 500),
                       Uniform(r, -5, 20), Uniform(r, -500, 500), 1};
  for (int i = 0; i < 16; i++) {
    StoreFloat(base, at + 4 * i, m[i]);
  }
}

// One case's choices (drawn once, written the same for both versions).
struct World {
  uint32_t b = 0, race = 0;
  uint32_t state = 0, path = 0, slot0 = 0, shared = 0, parking = 0;
  uint32_t models = 0, count = 0;
  bool matrix = false;
  uint32_t mode_type = 0, second_human = 0, kind = 0, player_word = 0, stage = 0,
           stage_word = 0, flags = 0, queue_index = 0;
  uint32_t viewports[3] = {};
  uint32_t view_mode[2] = {};
  bool provider_xpr = false;
  bool crowd = false;  // nine models in use, none traffic (the headlight slots run out)
  bool odd_fov = false;  // the headlight's field of view NaN or infinite
  bool odd_passes = false;  // passes' fields of view (+368) NaN now and then
  uint64_t seed = 0;
};

World Draw(Random& r, uint32_t b, uint32_t race, int c) {
  World w;
  w.b = b;
  w.race = race;
  w.state = r.Below(16) == 0 ? 4 : r.Below(4);
  if (w.state == 4 && r.Below(2)) w.state = 1;
  w.path = c % 2 == 0 ? 0 : (r.Below(8) == 0 ? uint32_t(r.Next()) | 1 : 1);
  w.slot0 = r.Below(3) == 0 ? r.Below(5) : 3;
  w.shared = r.Below(2);
  w.parking = r.Below(3) == 0 ? 1 : 0;
  w.models = r.Below(kMaxModels + 1);
  w.count = w.models == 0 ? 0 : (r.Below(6) == 0 ? w.models + r.Below(3) : r.Below(w.models + 1));
  if (r.Below(3) == 0) w.count = w.models;
  w.matrix = r.Below(3) == 0;
  w.mode_type = r.Below(4) == 0 ? 8 : r.Below(8);
  w.second_human = r.Below(2);
  w.kind = r.Below(2) ? 6 : r.Below(8);
  w.player_word = r.Below(2) ? 2 : r.Below(4);
  w.stage = r.Below(10);
  w.stage_word = r.Below(2) ? 1 : r.Below(4);
  w.flags = uint32_t(r.Next()) & (r.Below(2) ? 0xFFFFFF7Fu : 0xFFFFFFFFu);
  w.queue_index = r.Below(2);
  w.viewports[0] = (w.path == 0 || r.Below(6) != 0) ? b + kViewport0 : 0;
  w.viewports[1] = r.Below(4) != 0 ? b + kViewport1 : 0;
  w.viewports[2] = r.Below(2);
  w.view_mode[0] = r.Below(4);
  w.view_mode[1] = r.Below(4);
  w.provider_xpr = r.Below(3) != 0;
  w.crowd = c % 10 == 5;
  if (w.crowd) {
    w.models = kMaxModels;
    w.count = kMaxModels;
  }
  w.odd_fov = r.Below(12) == 0;
  w.odd_passes = r.Below(8) == 0;
  w.seed = r.Next();
  return w;
}

// Writes the world (from the case's own generator, the same both times).
void Build(Context& h, const World& w) {
  uint8_t* base = h.base;
  Random r(w.seed);
  const uint32_t b = w.b;
  Fill(base, b, kWorldBytes);
  auto at = [&](uint32_t offset, uint32_t value) { guest::StoreU32(base, b + offset, value); };

  // The render manager, its getters, the command list and the queue.
  at(kManager, b + kManagerVtable);
  at(kManagerVtable + 32, kGetter284);
  at(kManager + 284, b + kObject);
  at(kObject, b + kObjectVtable);
  at(kObjectVtable + 4, kGetter284);
  at(kObjectVtable + 8, kGetter124);
  at(kObject + 284, b + kList);
  at(kObject + 124, b + kQueue);
  at(kList + 8, kSlotCount);
  at(kList + 12, 0);
  at(kList + 32, b + kSlots);
  guest::StoreU8(base, b + kList + 52, 0);
  at(kQueue + 0, kQueueVtable);
  at(kQueue + 4, b + kState);
  at(kQueue + 8, b + kListA);
  at(kQueue + 12, b + kListB);
  at(kState + 0, w.queue_index);
  StoreFloat(base, b + kState + 84, float(r.Below(100)));
  for (const auto& [list, items, buckets] :
       {std::tuple{kListA, kItemsA, kBucketsA}, std::tuple{kListB, kItemsB, kBucketsB}}) {
    at(list + 8, kItemCount);
    at(list + 16, r.Below(4) == 0 ? r.Below(20) : 0);
    at(list + 20, b + items);
    at(list + 12, kBucketCount);
    at(list + 24, b + buckets);
    for (uint32_t k = 0; k < kBucketCount; k++) {
      at(buckets + 16 * k + 0, 0);
      at(buckets + 16 * k + 12, 0);
    }
  }

  // The viewports.
  at(kViewportArray + 0, w.viewports[0]);
  at(kViewportArray + 4, w.viewports[1]);
  at(kViewportArray + 8, w.viewports[2]);
  at(kViewport0 + 1860, w.view_mode[0]);
  at(kViewport1 + 1860, w.view_mode[1]);

  // The scene.
  const uint32_t scene = b + kScene;
  at(kScene + 0, w.state);
  at(kScene + 4, w.path);
  at(kScene + 84, w.slot0);
  // Its XPR at +88 (+16 the records, +20 their count) and the provider's.
  at(kScene + 88 + 16, b + kXprRecords);
  at(kScene + 88 + 20, r.Below(3));
  for (uint32_t k = 0; k < 4; k++) {
    at(kXprRecords + 16 * k + 4, uint32_t(r.Next()));
    at(kXpr2Records + 16 * k + 4, r.Below(4) == 0 ? 0 : uint32_t(r.Next()));
  }
  at(kXpr2 + 0, r.Below(4) == 0 ? 0 : uint32_t(r.Next()));
  at(kXpr2 + 16, b + kXpr2Records);
  at(kXpr2 + 20, r.Below(3));
  at(kScene + 432, b + kProviderVtable);
  at(kProviderVtable + 12, kGetter284);
  at(kScene + 432 + 284, w.provider_xpr ? b + kXpr2 : 0);
  at(kScene + 5028, w.models != 0 ? b + kModels : 0);
  at(kScene + 5036, w.count);
  at(kScene + 5044, w.shared);
  at(kScene + 5052, w.parking);
  // The lights, the beams and their shapes, the headlight's eye (+5376)
  // and field of view (+5436).
  Floats(base, scene + 5248, 64, r, 2.0f);
  StoreFloat(base, scene + 5436,
             w.odd_fov ? (r.Below(2) ? std::numeric_limits<float>::quiet_NaN()
                                     : std::numeric_limits<float>::infinity())
             : r.Below(10) == 0 ? Value(r, 3.0f)
                                : Uniform(r, 0.3f, 1.5f));

  // The scene models.
  for (uint32_t i = 0; i < w.models; i++) {
    const uint32_t m = kModels + i * kModelBytes;
    at(m + 8, i + 1 < w.models ? b + kModels + (i + 1) * kModelBytes : 0);
    at(m + 16, w.crowd || r.Below(4) != 0 ? 3 : r.Below(5));
    at(m + 32, w.crowd ? r.Below(3) : r.Below(3) == 0 ? 3 : r.Below(5));
    at(m + 48, r.Below(16));
    at(m + 3776, r.Below(4));
    at(m + 4292, uint32_t(r.Next()));
    at(m + 4296, r.Below(2));
    Matrix(base, b + m + 4304, r);
    StoreFloat(base, b + m + 4992, Uniform(r, -5.0f, 5.0f));
    at(m + 5028, 1);
    at(m + 5048, w.crowd || r.Below(5) != 0 ? 1 + r.Below(1000) : 0);
  }
  if (w.matrix) {
    Matrix(base, b + kMatrix, r);
  }

  // The race data.
  guest::StoreU32(base, w.race + 0xA3AF0, w.kind);
  guest::StoreU32(base, w.race + 0x59614, w.player_word);
  guest::StoreU32(base, w.race + 0x59620 + 804, w.stage);
  for (uint32_t k = 0; k < 80; k += 4) {
    guest::StoreU32(base, w.race + 0x59620 + 80 * w.stage + k, uint32_t(r.Next()));
  }
  guest::StoreU32(base, w.race + 0x59620 + 80 * w.stage, w.stage_word);
  guest::StoreU32(base, w.race + 0x59AE4, w.flags);

  // The camera passes in the second path: active or not, types 0 to 5.
  if (w.path != 0) {
    for (uint32_t p = 0; p < 21; p++) {
      const uint32_t pass = 0x826BAC00 + 592 * p;
      guest::StoreU32(base, pass + 588, r.Below(3) != 0 ? 1 : 0);
      guest::StoreU32(base, pass + 584, r.Below(8) == 0 ? 5 : r.Below(5));
      if (w.odd_passes && r.Below(3) == 0) {
        StoreFloat(base, pass + 368, std::numeric_limits<float>::quiet_NaN());
      }
    }
  }

  // The globals.
  guest::StoreU32(base, 0x826C2E44, scene);
  guest::StoreU32(base, 0x826C5B2C, w.race);
  guest::StoreU32(base, 0x826C7824, w.second_human);
  guest::StoreU32(base, 0x826C2CE8, b + kViewportArray);
  guest::StoreU32(base, 0x826032DC, w.mode_type);
  guest::StoreU32(base, 0x82601854, uint32_t(r.Next()));
  guest::StoreU32(base, 0x82601858, uint32_t(r.Next()));
  guest::StoreU32(base, 0x826BABA0, b + kManager);
  guest::StoreU32(base, kOpenChunks, 0);
}

// `bytes` at `from` into `out`, the words `is_float(offset)` names as floats
// (runs of each kind together), the rest exactly.
template <typename IsFloat>
void Compare(uint8_t* base, Outputs& out, uint32_t from, uint32_t bytes, IsFloat is_float) {
  uint32_t run = 0;
  while (run < bytes) {
    const bool kind = is_float(run);
    uint32_t end = run + 4;
    while (end < bytes && is_float(end) == kind) {
      end += 4;
    }
    if (kind) {
      out.Floats(base, from + run, (end - run) / 4);
    } else {
      out.Bytes(Read(base, from + run, end - run));
    }
    run = end;
  }
}

bool In(uint32_t offset, std::initializer_list<Area> ranges) {
  for (const Area& a : ranges) {
    if (offset >= a.from && offset < a.to) {
      return true;
    }
  }
  return false;
}

// What the case leaves, floats as floats where they are: the frame's vectors
// and matrices (stvx and stfs results and copies of them), the command
// list's uploads (the slots after a command 7, w1 of them), the passes'
// view-projections (+144) and the scene lights' and headlight slots' arrays
// (0x8296B950 to 0x8296BCE0); the rest exactly.
void Collect(uint8_t* base, const World& w, const PPCContext& t, uint32_t stack, Outputs& out) {
  out.Word(t.r3.u32);
  const uint32_t f = stack - kFrame;
  Compare(base, out, f, kFrame - kSavedTop, [](uint32_t o) {
    return In(o, {{112, 160}, {176, 192}, {208, 224}, {272, 288}, {304, 336}, {352, 368},
                  {384, 1120}, {1200, 1392}});
  });
  out.Word(guest::LoadU32(base, stack + 20));
  // The uploads' slots in the command list.
  std::vector<bool> upload(kSlotCount, false);
  const uint32_t used = std::min<uint32_t>(guest::LoadU32(base, w.b + kList + 12), kSlotCount);
  for (uint32_t s = 0; s < used; s++) {
    const uint32_t slot = w.b + kSlots + 16 * s;
    if (guest::LoadU32(base, slot) == 7) {
      const uint32_t n = guest::LoadU32(base, slot + 4);
      for (uint32_t k = 1; k <= n && s + k < kSlotCount; k++) {
        upload[s + k] = true;
      }
      s += std::min<uint32_t>(n, 8);
    }
  }
  Compare(base, out, w.b, kWorldBytes, [&](uint32_t o) {
    return o >= kSlots && o < kSlots + 16 * kSlotCount && upload[(o - kSlots) / 16];
  });
  Compare(base, out, kAreas[0].from, kAreas[0].to - kAreas[0].from, [](uint32_t o) {
    const uint32_t from_passes = o - (0x826BAC00 - 0x826BABA0);
    return o >= 0x826BAC00 - 0x826BABA0 && from_passes % 592 >= 144 && from_passes % 592 < 208;
  });
  Compare(base, out, kAreas[1].from, kAreas[1].to - kAreas[1].from, [](uint32_t o) {
    return o >= 0x8296B950 - 0x8296B8C0 && o < 0x8296BCE0 - 0x8296B8C0;
  });
  Compare(base, out, kAreas[2].from, kAreas[2].to - kAreas[2].from, [](uint32_t) { return false; });
}

}  // namespace

// The root's cases (called by CarSceneDraw in car_scene_draw.cpp).
void CarSceneDrawRoot(Context& h) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: CarScene::Draw: skipped (no native heap)");
    return;
  }
  Random r(0x5CE7EDA4ull);
  const uint32_t b = memory::AllocPages(kWorldBytes, 4096);
  const uint32_t race = memory::AllocPages(kRaceBytes, 4096);
  if (b == 0 || race == 0) {
    ITC_LOG_ERROR("harness: CarScene::Draw: no memory for the world");
    return;
  }
  const Globals globals(h.base);
  Results results("CarScene::Draw (sub_820C5DE8)");
  int paths[2] = {}, queued = 0, slots = 0;
  for (int c = 0; c < 400; c++) {
    const World w = Draw(r, b, race, c);
    std::vector<uint8_t> stack_fill(kStackBytes + 64);
    for (uint8_t& byte : stack_fill) byte = uint8_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      globals.Restore();
      PPCContext t = CallContext(h);
      t.fpscr.disableFlushModeUnconditional();
      const uint32_t stack = t.r1.u32;
      Write(h.base, stack - kStackBytes, stack_fill.data(), kStackBytes + 64);
      Build(h, w);
      t.r3.u64 = b + kScene;
      t.r4.u64 = w.matrix ? b + kMatrix : 0;
      t.lr = 0x8215B59C;
      {
        std::optional<FlagOff> off;
        if (v == 0) {
          off.emplace();
        }
        (v == 0 ? __imp__sub_820C5DE8 : hooks::CarSceneDrawRoot)(t, h.base);
      }
      Collect(h.base, w, t, stack, out[v]);
      if (v == 1) {
        paths[w.path != 0 ? 1 : 0]++;
        slots += int(guest::LoadU32(h.base, b + kList + 12));
        queued += int(guest::LoadU32(h.base, b + kListA + 16) +
                      guest::LoadU32(h.base, b + kListB + 16));
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: state {} path {} slot0 {} shared {} parking {} models {} "
                              "count {} matrix {} mode {} kind {} word {} stage word {} flags "
                              "{:08X} crowd {} odd fov {} odd passes {} viewports {:08X} {:08X} {} modes {} {}",
                              c, w.state, w.path, w.slot0, w.shared, w.parking, w.models, w.count,
                              w.matrix, w.mode_type, w.kind, w.player_word, w.stage_word, w.flags, w.crowd, w.odd_fov, w.odd_passes,
                              w.viewports[0], w.viewports[1], w.viewports[2], w.view_mode[0],
                              w.view_mode[1]));
  }
  globals.Restore();
  ITC_LOG_INFO("harness: CarScene::Draw: {} first-path and {} second-path cases, {} command "
               "list slots, {} render queue entries (the native side's, with the queue's "
               "starting ones)",
               paths[0], paths[1], slots, queued);
  results.Report();
  memory::FreePages(b);
  memory::FreePages(race);
}

}  // namespace harness
