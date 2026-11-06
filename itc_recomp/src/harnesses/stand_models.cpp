// The stand's scene model holders (src/stand_models/,
// docs/functions/stand-models.md) against the recompiled functions. Each
// case fills a block from the allocation layer with random words from the
// case's seed, lays the world the function reads there (the holder, the
// stage, the second set [0x826C2E44] with its pool 7 list and the stage's
// scene model, the parts' point block; [0x826D83E0] pointed at a holder
// word; both globals put back after), runs the original and the register
// step (hooks/stand_models.h) from the same inputs and compares the block,
// the top function's frame below the call (its back chain and
// temporaries; its saved registers left out: the adapters don't save
// them), r3 and the float registers the original sets. Floats the
// originals store with float instructions are compared as floats,
// everything else exactly. Callees run as their adapters run them on both
// sides (their own harnesses check them). Most of these run in the front
// end, which attract races don't reach: the harness is their check.

#include "harness.h"

#include "base/log.h"
#include "hooks/stand_models.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace harness {

namespace {

namespace sm = stand_models;

constexpr uint32_t kBytes = 0x14000;

// The block's layout.
constexpr uint32_t kHolderAt = 0x40;   // the holder (its +0 the stage)
constexpr uint32_t kHolderWord = 0x60; // [0x826D83E0] points here (its +20)
constexpr uint32_t kScene = 0x100;     // the second set (5,100 bytes)
constexpr uint32_t kModel = 0x1600;    // the stage's scene model (5,120 bytes)
constexpr uint32_t kModel2 = 0x2A00;   // a second pool 7 object
constexpr uint32_t kStage = 0x4000;    // the stage (6,208 bytes)
constexpr uint32_t kParts = 0x6000;    // a parts object (its point block at +176)
constexpr uint32_t kWheels = 0xD000;   // a pool 5 wheels object (its +28 the next)
constexpr uint32_t kFirstWheels = 0xD100;  // the first set's wheels (tires at +240, +704)
constexpr uint32_t kShapes = 0x10000;  // 4 shape areas of 0x1000

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    Put(base, address + i, uint32_t(r.Next()));
  }
}

// Mostly a finite float a few hundred either way, now and then a NaN, an
// infinity, a zero or a denormal.
uint32_t RandomFloat(Random& r) {
  switch (r.Below(12)) {
    case 0: return 0x7FC00000u | (r.Below(2) << 31) | r.Below(0x400000);
    case 1: return 0x7F800001u | (r.Below(2) << 31) | r.Below(0x3FFFFF);
    case 2: return 0x7F800000u | (r.Below(2) << 31);
    case 3: return r.Below(2) << 31;
    case 4: return r.Below(0x800000) | (r.Below(2) << 31);
    default: {
      const float f = (float(r.Below(200000)) - 100000.0f) / 256.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}

// A float register: a single's value (the callers' floats are singles).
double RandomDouble(Random& r) { return double(std::bit_cast<float>(RandomFloat(r))); }

// A value: small, an edge, or any word.
uint32_t RandomValue(Random& r) {
  switch (r.Below(4)) {
    case 0: return r.Below(10);
    case 1: {
      constexpr uint32_t kEdges[] = {0, 1, 2, 3, 6, 7, 0xFFFFFFFFu, 0x80000000u, 0x7FFFFFFFu};
      return kEdges[r.Below(sizeof(kEdges) / sizeof(kEdges[0]))];
    }
    case 2: return uint32_t(r.Next());
    default: return r.Below(3);
  }
}

// The block compared, `floats` (absolute addresses, each a word) as floats.
void AddBlock(Outputs& out, uint8_t* base, uint32_t block, std::vector<uint32_t> floats) {
  std::sort(floats.begin(), floats.end());
  floats.erase(std::unique(floats.begin(), floats.end()), floats.end());
  uint32_t at = block;
  const uint32_t end = block + kBytes;
  for (const uint32_t f : floats) {
    if (f < at || f + 4 > end) {
      continue;
    }
    if (f > at) {
      out.Bytes(Read(base, at, f - at));
    }
    out.Floats(base, f, 1);
    at = f + 4;
  }
  if (at < end) {
    out.Bytes(Read(base, at, end - at));
  }
}

// The stage's world: the holder, the stage (its +8 the model's index, its
// +16 claimed or not), the second set with pool 7's list of one or two
// objects (the model at index 0, in use, or past the end: the first node),
// the scene's +4 0 (the front end's), the parts at the model's +5060.
void LayStage(uint8_t* base, uint32_t b, Random& r) {
  Put(base, b + kHolderAt, b + kStage);
  Put(base, sm::kScene, b + kScene);
  Put(base, sm::kHolder, b + kHolderWord);
  Put(base, b + kScene + 4, 0);
  const uint32_t list = b + kScene + 4944 + 12 * 7;
  const bool two = r.Below(2) == 0;
  Put(base, list + 0, b + kModel);
  Put(base, list + 8, two ? 2 : 1);
  Put(base, b + kModel + 8, two ? b + kModel2 : 0);
  Put(base, b + kModel2 + 8, 0);
  Put(base, b + kModel + 16, 3);
  Put(base, b + kModel2 + 16, r.Below(2) == 0 ? 3 : 0);
  Put(base, b + kStage + 8, r.Below(4) == 0 ? (two ? 1 : 5) : 0);
  if (two && guest::LoadU32(base, b + kStage + 8) == 1) {
    Put(base, b + kModel2 + 16, 3);
  }
  Put(base, b + kModel + 5060, b + kParts);
  Put(base, b + kModel2 + 5060, b + kParts);
}

struct Saved {
  explicit Saved(uint8_t* base) : base(base) {
    scene = guest::LoadU32(base, sm::kScene);
    holder = guest::LoadU32(base, sm::kHolder);
  }
  ~Saved() {
    guest::StoreU32(base, sm::kScene, scene);
    guest::StoreU32(base, sm::kHolder, holder);
  }
  uint8_t* base;
  uint32_t scene, holder;
};

// One case: the block from `seed`, `setup` laying the world and the
// registers (its own generator, seeded the same for both), the function
// run; the block (`floats` as floats), the frame (`frame` bytes below the
// call's r1, its top `saves` left out), r3 and, when `f1`, f1.
void Case(Context& h, Results& results, uint32_t b, uint64_t seed,
          const std::function<void(PPCContext&, Random&, std::vector<uint32_t>&)>& setup,
          GuestFunction recompiled, GuestFunction native, uint32_t frame, uint32_t saves,
          bool f1, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    RandomBytes(h.base, b, kBytes, r);
    PPCContext t = CallContext(h);
    t.r1.u64 &= ~0xFull;
    const uint32_t sp = t.r1.u32;
    RandomBytes(h.base, sp - 8192, 8192, r);
    std::vector<uint32_t> floats;
    setup(t, r, floats);
    t.fpscr.disableFlushModeUnconditional();
    (v == 0 ? recompiled : native)(t, h.base);
    AddBlock(out[v], h.base, b, floats);
    out[v].Bytes(Read(h.base, sp - frame, frame - saves));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    if (f1) {
      out[v].Double(t.f1.u64);
    }
  }
  results.Check(out[0], out[1], what);
}

void Setters(Context& h, uint32_t b) {
  struct Setter {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Setter setters[] = {
      {"GarageStage::SetInteriorClass (sub_82178FE8)", __imp__sub_82178FE8,
       hooks::StandModelsSetInteriorClass},
      {"GarageStage::SetWord5016 (sub_82179598)", __imp__sub_82179598,
       hooks::StandModelsSetWord5016},
      {"GarageStage::SetWheelWord (sub_82179A18)", __imp__sub_82179A18,
       hooks::StandModelsSetWheelWord},
  };
  for (const Setter& s : setters) {
    Results results(s.name);
    for (int c = 0; c < 64; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>&) {
             LayStage(h.base, b, r);
             t.r3.u64 = b + kHolderAt;
             t.r4.u64 = uint64_t(r.Next() & 0xFFFFFFFF00000000ull) | RandomValue(r);
           },
           s.recompiled, s.native, 96, 16, false, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneParts::LightValue (sub_820DF1A0)");
    for (int c = 0; c < 32; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>&) {
             Put(h.base, b + kParts + 176 + 27664, RandomFloat(r));
             t.r3.u64 = b + kParts + 176;
           },
           __imp__sub_820DF1A0, hooks::StandModelsLightValue, 16, 0, true,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageStage::SetLightsOn (sub_82179420)");
    for (int c = 0; c < 64; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>& floats) {
             LayStage(h.base, b, r);
             Put(h.base, b + kParts + 176 + 27664, RandomFloat(r));
             const uint32_t model = sm::StageModel(h.base, b + kHolderAt);
             floats.push_back(model + 4980);
             t.r3.u64 = b + kHolderAt;
             t.r4.u64 = r.Below(2) == 0 ? 0 : RandomValue(r);
             t.f1.f64 = RandomDouble(r);
           },
           __imp__sub_82179420, hooks::StandModelsSetLightsOn, 112, 24, true,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "GarageStage::SetDecalA (sub_82179818)"
                               : "GarageStage::SetDecalB (sub_821798D8)");
    const uint32_t at = which == 0 ? sm::kDecalA : sm::kDecalB;
    for (int c = 0; c < 64; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>& floats) {
             LayStage(h.base, b, r);
             // The entries 0 to 2 (the table's), or 3 (the frame's word
             // past the table, as the stack holds it: an entry in the
             // model's first 64 x 64 bytes when it's small).
             const uint32_t entry = r.Below(8) == 0 ? 3 : r.Below(3);
             const uint32_t sp = t.r1.u32;
             Put(h.base, sp - 160 + 92, r.Below(16));
             const uint32_t model = sm::StageModel(h.base, b + kHolderAt);
             const uint32_t word = entry < 3 ? 2 - entry : guest::LoadU32(h.base, sp - 160 + 92);
             const uint32_t e = model + 64 * word + at;
             for (const uint32_t f : {16u, 20u, 32u, 48u, 52u}) {
               floats.push_back(e + f);
             }
             t.r3.u64 = b + kHolderAt;
             t.r4.u64 = entry;
             t.r5.u64 = uint32_t(r.Next());
             t.r6.u64 = r.Below(2) == 0 ? 0 : RandomValue(r);
             t.f1.f64 = RandomDouble(r);
             t.f2.f64 = RandomDouble(r);
             t.f3.f64 = RandomDouble(r);
             t.f4.f64 = RandomDouble(r);
           },
           which == 0 ? __imp__sub_82179818 : __imp__sub_821798D8,
           which == 0 ? hooks::StandModelsSetDecalA : hooks::StandModelsSetDecalB, 160, 64,
           false, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// GarageStage::MakeModel: the stage claimed (+16 set: nothing) or not; the
// pool 7 objects free or in use (none free: ClaimB's -1, SetUpB then on the
// first node); the model's build deferred (+5044 1 to 3) so SetUpB's
// SceneModel::SetUp only copies the record (as RaceScene::RemakeCarModel's
// cases), the scene's +4 0.
void MakeModels(Context& h, uint32_t b) {
  Results results("GarageStage::MakeModel (sub_8217FCB0)");
  for (int c = 0; c < 64; c++) {
    Case(h, results, b, h.random.Next(),
         [&](PPCContext& t, Random& r, std::vector<uint32_t>&) {
           LayStage(h.base, b, r);
           Put(h.base, b + kStage + 16, r.Below(5) == 0 ? 1 : 0);
           for (const uint32_t m : {kModel, kModel2}) {
             Put(h.base, b + m + 16, r.Below(3) == 0 ? 3 : 0);
             Put(h.base, b + m + 20, r.Below(6));
             Put(h.base, b + m + 5028, 0);
             Put(h.base, b + m + 5040, 0);
             Put(h.base, b + m + 5044, 1 + r.Below(3));
             for (const uint32_t at : {5048u, 5060u, 5068u, 5076u, 5084u, 5100u}) {
               Put(h.base, b + m + at, 0);
             }
             Put(h.base, b + m + 5116, b + kScene);
           }
           t.r3.u64 = b + kHolderAt;
           t.r4.u64 = r.Below(45);
           t.r5.u64 = RandomValue(r);
         },
         __imp__sub_8217FCB0, hooks::StandModelsMakeModel, 4384, 40, false,
         fmt::format("case {}", c));
  }
  results.Report();
}

// The setters that re-apply the record: the stage's world, the model
// still loading (+5028 set: SceneModel::PollLoad answers 0, so ApplyRecord
// stops there; its own cases, load_poll's, take it further), its scene the
// front end's (+4 0, ApplyRecord's plain C++ path); ReloadDecals' model not
// ready (+5036 0) or traffic (its own cases, car_models', take it further).
// The slots, kinds, values and choices small, at the edges, or any word
// where the original checks them.
void Reapplies(Context& h, uint32_t b) {
  struct Setter {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t frame, saves;
    std::function<void(PPCContext&, Random&)> args;
  };
  const Setter setters[] = {
      {"GarageStage::SetPart (sub_82178F00)", __imp__sub_82178F00, hooks::StandModelsSetPart, 112,
       24,
       [](PPCContext& t, Random& r) {
         t.r4.u64 = r.Below(4) == 0 ? RandomValue(r) : r.Below(18);
         t.r5.u64 = uint32_t(r.Next());
       }},
      {"GarageStage::SetInteriorWord (sub_82179050)", __imp__sub_82179050,
       hooks::StandModelsSetInteriorWord, 112, 24,
       [](PPCContext& t, Random& r) {
         t.r4.u64 = r.Below(4) == 0 ? RandomValue(r) : r.Below(7);
         t.r5.u64 = uint32_t(r.Next());
       }},
      {"GarageStage::SetInteriorLevel (sub_821791C0)", __imp__sub_821791C0,
       hooks::StandModelsSetInteriorLevel, 96, 16,
       [](PPCContext& t, Random& r) { t.r4.u64 = RandomValue(r); }},
      {"GarageStage::SetInteriorPair (sub_82179250)", __imp__sub_82179250,
       hooks::StandModelsSetInteriorPair, 112, 24,
       [](PPCContext& t, Random& r) {
         t.r4.u64 = uint32_t(r.Next());
         t.r5.u64 = uint32_t(r.Next());
       }},
      {"GarageStage::ClearChoice (sub_82179378)", __imp__sub_82179378,
       hooks::StandModelsClearChoice, 96, 16,
       [](PPCContext& t, Random& r) { t.r4.u64 = r.Below(4) == 0 ? r.Below(40) : r.Below(6); }},
      {"GarageStage::SetAero (sub_82179548)", __imp__sub_82179548, hooks::StandModelsSetAero, 96,
       16, [](PPCContext& t, Random& r) { t.r4.u64 = uint32_t(r.Next()); }},
      {"GarageStage::ReloadDecals (sub_82178EC0)", __imp__sub_82178EC0,
       hooks::StandModelsReloadDecals, 96, 16, [](PPCContext&, Random&) {}},
  };
  for (const Setter& s : setters) {
    Results results(s.name);
    for (int c = 0; c < 48; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>&) {
             LayStage(h.base, b, r);
             for (const uint32_t m : {kModel, kModel2}) {
               Put(h.base, b + m + 5028, 1);
               Put(h.base, b + m + 5116, b + kScene);
               const bool traffic = r.Below(3) == 0;
               Put(h.base, b + m + 32, traffic ? 3 : r.Below(3));
               Put(h.base, b + m + 5036, traffic ? r.Below(2) : 0);
             }
             t.r3.u64 = b + kHolderAt;
             s.args(t, r);
           },
           s.recompiled, s.native, s.frame, s.saves, false, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// A part model at `model` as the load poll's cases make them: no nodes
// (+408), zero to two shapes (+412) in shape area `area`, each with a
// material block of up to one material a group.
void PartModel(uint8_t* base, uint32_t b, uint32_t model, uint32_t area, Random& r) {
  const uint32_t shapes = b + kShapes + 0x1000 * (area % 4);
  Put(base, model + 408, 0);
  Put(base, model + 412, r.Below(3));
  Put(base, model + 40, shapes);
  for (uint32_t s = 0; s < 2; s++) {
    const uint32_t materials = shapes + 0x480 + 0x600 * s;
    Put(base, shapes + 544 * s + 396, materials);
    const uint32_t first = r.Below(2), second = r.Below(2);
    Put(base, materials + 0, first + second);
    Put(base, materials + 8, first);
    Put(base, materials + 12, second);
  }
}

// The wheels: the model's +5100 the pool 5 object, its +28 the first set's
// wheels with their two tire models (+240, +704) and textures (+40, +56).
void LayWheels(uint8_t* base, uint32_t b, Random& r) {
  for (const uint32_t m : {kModel, kModel2}) {
    Put(base, b + m + 5100, b + kWheels);
  }
  Put(base, b + kWheels + 28, b + kFirstWheels);
  PartModel(base, b, b + kFirstWheels + 240, 0, r);
  PartModel(base, b, b + kFirstWheels + 704, 1, r);
  Put(base, b + kFirstWheels + 40, r.Below(3));
  Put(base, b + kFirstWheels + 56, r.Below(3));
}

// The tire grades: the wheels as LayWheels lays them; the grades 0 to 2 (or
// any word now and then), `which` -1 to 3 or any word; TuningScreen's
// index 0 to 4, or 5 (the frame's word past the table).
void TireGrades(Context& h, uint32_t b) {
  {
    Results results("Wheels::SetTireTextures (sub_820D8F18)");
    for (int c = 0; c < 32; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>&) {
             LayWheels(h.base, b, r);
             t.r3.u64 = b + kFirstWheels;
           },
           __imp__sub_820D8F18, hooks::StandModelsSetTireTextures, 96, 16, false,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("SceneWheels::SetTireGrades (sub_820E7470)");
    for (int c = 0; c < 32; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>&) {
             LayWheels(h.base, b, r);
             t.r3.u64 = b + kWheels;
             t.r4.u64 = r.Below(3);
             t.r5.u64 = r.Below(3);
           },
           __imp__sub_820E7470, hooks::StandModelsSetTireGrades, 96, 16, false,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageStage::SetTireGrade (sub_82179130)");
    for (int c = 0; c < 48; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>&) {
             LayStage(h.base, b, r);
             LayWheels(h.base, b, r);
             for (const uint32_t m : {kModel, kModel2}) {
               Put(h.base, b + m + 4080, r.Below(3));
               Put(h.base, b + m + 4096, r.Below(3));
             }
             t.r3.u64 = b + kHolderAt;
             t.r4.u64 = r.Below(6) == 0 ? RandomValue(r) : uint32_t(int32_t(r.Below(5)) - 1);
             t.r5.u64 = r.Below(3);
           },
           __imp__sub_82179130, hooks::StandModelsSetTireGrade, 112, 24, false,
           fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("TuningScreen::SetTireGrade (sub_821CCA38)");
    for (int c = 0; c < 48; c++) {
      Case(h, results, b, h.random.Next(),
           [&](PPCContext& t, Random& r, std::vector<uint32_t>&) {
             LayStage(h.base, b, r);
             LayWheels(h.base, b, r);
             Put(h.base, b + kHolderWord, b + kHolderAt);
             for (const uint32_t m : {kModel, kModel2}) {
               Put(h.base, b + m + 4080, r.Below(3));
               Put(h.base, b + m + 4096, r.Below(3));
             }
             // The frame's word past the table: a grade.
             Put(h.base, t.r1.u32 - 112 + 100, r.Below(3));
             t.r3.u64 = r.Next();
             t.r4.u64 = r.Below(6) == 0 ? RandomValue(r) : uint32_t(int32_t(r.Below(5)) - 1);
             t.r5.u64 = r.Below(8) == 0 ? 5 : r.Below(5);
           },
           __imp__sub_821CCA38, hooks::StandModelsTuningSetTireGrade, 112, 8, false,
           fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void StandModels(Context& h) {
  const uint32_t allocation = memory::Alloc(kBytes + 16, 0);
  if (allocation == 0) {
    ITC_LOG_WARN("harness: stand_models: no block from the allocation layer");
    return;
  }
  const uint32_t b = (allocation + 15) & ~15u;
  {
    const Saved saved(h.base);
    Setters(h, b);
    MakeModels(h, b);
    Reapplies(h, b);
    TireGrades(h, b);
  }
  memory::Free(allocation);
}

}  // namespace harness
