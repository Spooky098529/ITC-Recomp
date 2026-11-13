// Scenery::RecordHeadlightProjectors (sub_820F34C8,
// src/course_scenery/headlights.cpp) against the recompiled one, under the
// course_scenery harness, with a random generator of its own (it runs last,
// so its cases shift no other function's inputs). The recompiled side runs
// with --native_course_scenery and --native_car_scene_draw off (its callees
// the originals where those flags switch them); the native side the register
// step hooks::CourseSceneryRecordHeadlightProjectors.
//
// The world, in one allocation (the same addresses for both versions,
// filled with the same bytes before each): the pass description (r4: +12
// the player, +16 the viewport entry, +20 the camera pass, 2 one case in
// three, +24 and +28 the keys); the scene ([0x826C2E44]) with the List at
// +5028 of up to ten scene models (their type +16 3 or not, +32 1, 2 or not,
// +5048 a car or 0, the beam table's +48 and +3776, low or high beams
// +4292, their matrices), its count +5036 (one case in twenty -1 with nine
// or more lit models: the original's unsigned count, left at the ninth),
// the lights and beams (+5248 to +5487, the field of view +5436 NaN or
// infinite now and then: Projector::Build's original runs); the vehicles'
// pointer [0x826C2E34] (0 one case in eight: nothing but the keys and the
// eye); the render manager ([0x826BABA0]: its slot 8 and the answer's slots
// 1 and 2 the game's getters) with a command list of 2,048 slots and a
// render queue of the real class (its vtable 0x8206CB8C) with 512 items and
// 2,057 buckets in each pass list; the viewports ([0x826C2CE8], their
// entries random words or a camera's) for CameraPass::FromViewport; the
// camera pass given random words as Scenery::Update's cases do (for
// CameraPass::Update, a guest call on both sides), the passes' mode
// ([0x826BABC0]) random.
//
// Compared (floats as floats where the original computes them: the frame's
// vectors and the projector, the command list's uploads, the headlight
// slots, the passes' +144 to +207; the rest exactly): the allocation (with
// the command list's slots and the queue's items and buckets), the globals
// (0x826BABA0 to the passes' end, 0x8296B8C0 to 0x8296BD40 with the slots,
// the keys and the open chunks' count, 0x8265EC80 to 0x8265ED40), the frame
// (its back chain and temporaries up to the saved registers' slots) and r3
// (both halves). Everything the case points at is put back after.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/course_scenery.h"
#include "hooks/heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <tuple>
#include <vector>

ITC_FLAG_DECLARE(bool, native_course_scenery);
ITC_FLAG_DECLARE(bool, native_car_scene_draw);

namespace harness {

namespace {

constexpr uint32_t kFrame = 640;
// The saved registers' slots under the caller's r1 (r14 to r31, lr, f28 to
// f31), which the plain C++ doesn't write.
constexpr uint32_t kSavedTop = 184;
constexpr uint32_t kStackBytes = 4096;

// The allocation's layout.
constexpr uint32_t kScene = 0x0000;
constexpr uint32_t kDescription = 0x1580;
constexpr uint32_t kManager = 0x1600, kManagerVtable = 0x1700;
constexpr uint32_t kObject = 0x1800, kObjectVtable = 0x1A00;
constexpr uint32_t kList = 0x1B00;
constexpr uint32_t kQueue = 0x1C00, kState = 0x1C40, kListA = 0x1CC0, kListB = 0x1D00;
constexpr uint32_t kViewportArray = 0x1D80;
constexpr uint32_t kViewport0 = 0x2000, kViewport1 = 0x2800;
constexpr uint32_t kVehicles = 0x2F80;
constexpr uint32_t kModels = 0x3000, kModelBytes = 0x1480, kMaxModels = 10;
constexpr uint32_t kSlots = 0x10000, kSlotCount = 2048;
constexpr uint32_t kItemsA = 0x18000, kItemsB = 0x1A800, kItemCount = 512;
constexpr uint32_t kBucketsA = 0x1D000, kBucketsB = 0x25100, kBucketCount = 2057;
constexpr uint32_t kWorldBytes = 0x2E000;

constexpr uint32_t kGetter284 = 0x8248CA78;  // returns [r3 + 284]
constexpr uint32_t kGetter124 = 0x82379320;  // returns [r3 + 124]
constexpr uint32_t kQueueVtable = 0x8206CB8C;
constexpr uint32_t kOpenChunks = 0x8296BD28;
constexpr uint32_t kPassVtable = 0x82001EC0;
constexpr uint32_t kPasses = 0x826BAC00, kPassBytes = 592;
constexpr uint32_t kMode = 0x826BABC0;

struct Area {
  uint32_t from, to;
};
constexpr Area kAreas[] = {
    {0x826BABA0, kPasses + 21 * kPassBytes},  // the render manager, kMode, the passes
    {0x8296B8C0, 0x8296BD40},                 // the keys, the slots, the open chunks
    {0x8265EC80, 0x8265ED40},                 // the eye's and the light's copies
};
constexpr uint32_t kWords[] = {0x826C2E44, 0x826C2E34, 0x826C2CE8};

// The flags off for the recompiled side.
struct FlagsOff {
  FlagsOff()
      : course_(ITC_FLAG(native_course_scenery)), draw_(ITC_FLAG(native_car_scene_draw)) {
    ITC_FLAG(native_course_scenery) = false;
    ITC_FLAG(native_car_scene_draw) = false;
  }
  ~FlagsOff() {
    ITC_FLAG(native_course_scenery) = course_;
    ITC_FLAG(native_car_scene_draw) = draw_;
  }
  bool course_, draw_;
};

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
void Matrix(uint8_t* base, uint32_t at, Random& r) {
  if (r.Below(8) == 0) {
    Floats(base, at, 16, r, 100.0f);
    return;
  }
  const float a = Uniform(r, -3.14159f, 3.14159f);
  const float c = std::cos(a), s = std::sin(a);
  const float m[16] = {c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, Uniform(r, -500, 500),
                       Uniform(r, -5, 20), Uniform(r, -500, 500), 1};
  for (int i = 0; i < 16; i++) {
    StoreFloat(base, at + 4 * i, m[i]);
  }
}
uint32_t FloatWord(Random& r) {
  switch (r.Below(16)) {
    case 0:
      return 0x7FC00000u | (r.Below(2) << 31);
    case 1:
      return 0x7F800000u | (r.Below(2) << 31);
    case 2:
      return uint32_t(r.Next());
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 8192.0f);
  }
}

struct World {
  uint32_t b = 0;
  uint32_t models = 0;
  int32_t count = 0;
  uint32_t pass = 0, player = 0, entry = 0;
  bool vehicles = true, crowd = false, odd_fov = false;
  uint32_t mode = 0;
  uint64_t seed = 0;
};

World Draw(Random& r, uint32_t b, int c) {
  World w;
  w.b = b;
  w.crowd = c % 20 == 7;
  w.models = w.crowd ? 9 + r.Below(2) : r.Below(kMaxModels + 1);
  w.count = w.crowd ? -1
            : w.models == 0
                ? int32_t(r.Below(3))
                : int32_t(r.Below(6) == 0 ? w.models + r.Below(3) : r.Below(w.models + 1));
  if (!w.crowd && r.Below(3) == 0) w.count = int32_t(w.models);
  w.pass = r.Below(3) == 0 ? 2 : r.Below(21);
  w.player = r.Below(2);
  w.entry = r.Below(19);
  w.vehicles = r.Below(8) != 0;
  w.odd_fov = r.Below(12) == 0;
  w.mode = r.Below(4);
  w.seed = r.Next();
  return w;
}

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
  at(kState + 0, r.Below(2));
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

  // The viewports: twenty 80-byte entries each (+16 the eye, +32 the
  // target, +48 the up, +64 the roll, +68 the zoom, +72 the far plane).
  at(kViewportArray + 0, b + kViewport0);
  at(kViewportArray + 4, b + kViewport1);
  at(kViewportArray + 8, r.Below(2));
  for (uint32_t viewport : {kViewport0, kViewport1}) {
    for (uint32_t i = 0; i < 468; i++) at(viewport + 4 * i, FloatWord(r));
    for (uint32_t e = 0; e < 19; e++) {
      const uint32_t entry = viewport + 80 * e;
      if (r.Below(4) == 0) continue;  // random words
      for (uint32_t k = 0; k < 3; k++) {
        const float p = Uniform(r, -500, 500);
        StoreFloat(base, b + entry + 16 + 4 * k, p);
        StoreFloat(base, b + entry + 32 + 4 * k, p + Uniform(r, -30, 30));
        StoreFloat(base, b + entry + 48 + 4 * k, k == 1 ? 1.0f : 0.0f);
      }
      StoreFloat(base, b + entry + 64, Uniform(r, -3, 3));
      StoreFloat(base, b + entry + 68, 666.33f);
      StoreFloat(base, b + entry + 72, Uniform(r, 10, 200));
    }
  }

  // The pass description.
  at(kDescription + 8, uint32_t(r.Next()));
  at(kDescription + 12, w.player);
  at(kDescription + 16, w.entry);
  at(kDescription + 20, w.pass);
  at(kDescription + 24, uint32_t(r.Next()));
  at(kDescription + 28, uint32_t(r.Next()));

  // The camera pass, as Scenery::Update's cases give it.
  const uint32_t pass = kPasses + kPassBytes * w.pass;
  for (uint32_t i = 0; i < 148; i++) guest::StoreU32(base, pass + 4 * i, FloatWord(r));
  guest::StoreU32(base, pass, kPassVtable);
  guest::StoreU32(base, pass + 584, r.Below(22));
  guest::StoreU32(base, pass + 504, 1280);
  guest::StoreU32(base, pass + 508, 720);
  guest::StoreU32(base, pass + 588, r.Below(3) == 0 ? 0 : 1 + r.Below(2));

  // The scene: its models, count, lights and beams.
  const uint32_t scene = b + kScene;
  at(kScene + 5028, w.models != 0 ? b + kModels : 0);
  at(kScene + 5036, uint32_t(w.count));
  Floats(base, scene + 5248, 64, r, 2.0f);
  StoreFloat(base, scene + 5436,
             w.odd_fov ? (r.Below(2) ? std::numeric_limits<float>::quiet_NaN()
                                     : std::numeric_limits<float>::infinity())
             : r.Below(10) == 0 ? Value(r, 3.0f)
                                : Uniform(r, 0.3f, 1.5f));
  for (uint32_t i = 0; i < w.models; i++) {
    const uint32_t m = kModels + i * kModelBytes;
    const bool lit = w.crowd || r.Below(3) != 0;
    at(m + 8, i + 1 < w.models ? b + kModels + (i + 1) * kModelBytes : 0);
    at(m + 16, lit ? 3 : r.Below(5));
    at(m + 32, lit ? 1 + r.Below(2) : r.Below(3) == 0 ? 3 : r.Below(5));
    at(m + 48, r.Below(16));
    at(m + 3776, r.Below(4));
    at(m + 4292, uint32_t(r.Next()));
    Matrix(base, b + m + 4304, r);
    at(m + 5048, lit || r.Below(5) != 0 ? 1 + r.Below(1000) : 0);
  }

  // The globals.
  guest::StoreU32(base, 0x826C2E44, scene);
  guest::StoreU32(base, 0x826C2E34, w.vehicles ? b + kVehicles : 0);
  guest::StoreU32(base, 0x826C2CE8, b + kViewportArray);
  guest::StoreU32(base, 0x826BABA0, b + kManager);
  guest::StoreU32(base, kMode, w.mode);
  guest::StoreU32(base, kOpenChunks, 0);
}

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

void Collect(uint8_t* base, const World& w, const PPCContext& t, uint32_t stack, Outputs& out) {
  out.Word(t.r3.u32);
  out.Word(uint32_t(t.r3.u64 >> 32));
  const uint32_t f = stack - kFrame;
  Compare(base, out, f, kFrame - kSavedTop,
          [](uint32_t o) { return In(o, {{80, 208}, {336, 400}, {416, 448}}); });
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
    const uint32_t from_passes = o - (kPasses - 0x826BABA0);
    return o >= kPasses - 0x826BABA0 && from_passes % kPassBytes >= 144 &&
           from_passes % kPassBytes < 208;
  });
  Compare(base, out, kAreas[1].from, kAreas[1].to - kAreas[1].from, [](uint32_t o) {
    return o >= 0x8296B9E0 - 0x8296B8C0 && o < 0x8296BCE0 - 0x8296B8C0;
  });
  Compare(base, out, kAreas[2].from, kAreas[2].to - kAreas[2].from,
          [](uint32_t) { return false; });
}

}  // namespace

// The cases (called by CourseScenery in course_scenery.cpp).
void HeadlightProjectorCases(Context& h) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: Scenery::RecordHeadlightProjectors: skipped (no native heap)");
    return;
  }
  Random r(0x4845414447485453ull);
  const uint32_t b = memory::AllocPages(kWorldBytes, 4096);
  if (b == 0) {
    ITC_LOG_ERROR("harness: Scenery::RecordHeadlightProjectors: no memory for the world");
    return;
  }
  const Globals globals(h.base);
  Results results("Scenery::RecordHeadlightProjectors (sub_820F34C8)");
  int chunks = 0, no_vehicles = 0;
  for (int c = 0; c < 400; c++) {
    const World w = Draw(r, b, c);
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
      t.r4.u64 = b + kDescription;
      t.lr = 0x820F6888;
      if (v == 0) {
        FlagsOff off;
        __imp__sub_820F34C8(t, h.base);
      } else {
        hooks::CourseSceneryRecordHeadlightProjectors(t, h.base);
      }
      Collect(h.base, w, t, stack, out[v]);
      if (v == 1) {
        chunks += int(guest::LoadU32(h.base, b + kList + 12));
        no_vehicles += w.vehicles ? 0 : 1;
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: models {} count {} pass {} player {} entry {} vehicles "
                              "{} crowd {} odd fov {} mode {}",
                              c, w.models, w.count, w.pass, w.player, w.entry, w.vehicles,
                              w.crowd, w.odd_fov, w.mode));
  }
  globals.Restore();
  ITC_LOG_INFO("harness: Scenery::RecordHeadlightProjectors: {} command list slots (the native "
               "side's), {} cases without vehicles",
               chunks, no_vehicles);
  results.Report();
  memory::FreePages(b);
}

}  // namespace harness
