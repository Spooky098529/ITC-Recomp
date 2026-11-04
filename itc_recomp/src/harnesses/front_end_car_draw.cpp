// The front end's car draw (src/front_end_car_draw/,
// docs/functions/front-end-car-draw.md) against the recompiled functions.
// Each case fills a block of guest memory with random bytes, lays the
// garage stage's world there (the holder, the stage with its garage camera
// and animation list, the car scene's pool 7 with two scene models, the
// parts, the camera presets) and points the car scene [0x826C2E44] at it,
// then runs the original and the register step (hooks/front_end_car_draw.h)
// from the same inputs, and compares the block, the stack below the call
// (the frames' temporaries, the top function's saved registers left out:
// the adapters don't save them) and r3. The callees (this subsystem's under
// its flag, and ScenePools::GetB, SceneModel::PollLoad, tan and memcpy) run
// as their adapters run them on both sides (their own cases check them), so
// the original's callees' frames are the plain C++'s too. Floats are compared as
// floats where the originals compute or copy them with float instructions,
// everything else exactly. The globals are put back after each case.

#include "harness.h"
#include "harnesses/car_draw_shared.h"

#include "hooks/front_end_car_draw.h"
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

constexpr uint32_t kBlockBytes = 0x10000;
constexpr uint32_t kStackBytes = 2048;  // below the call's r1
constexpr uint32_t kArgBytes = 64;      // above it
constexpr uint32_t kScenePointer = 0x826C2E44;

// The world's layout in the block.
constexpr uint32_t kHolder = 0x0;
constexpr uint32_t kStage = 0x100;     // 6,208 bytes
constexpr uint32_t kScene = 0x2000;    // pool 7's list at +5028
constexpr uint32_t kModel0 = 0x4000;   // 5,120 bytes each
constexpr uint32_t kModel1 = 0x5800;
constexpr uint32_t kParts = 0x7000;    // +176 + 4576
constexpr uint32_t kTable = 0x9000;    // the presets' indexes (4 x 290 words)
constexpr uint32_t kPresets = 0xA400;  // 8 presets of 64 bytes
constexpr uint32_t kPresetCount = 8;
constexpr uint32_t kKit = 0xBC00;      // a light kit (13,160 bytes)
constexpr uint32_t kPool4 = 0xF000;    // its pool 4 object

// Absolute guest ranges compared as floats: (address, count).
using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
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
// An angle near -π to π, now and then right at an end or beyond, or not a
// finite number.
uint32_t RandomAngle(Random& r) {
  switch (r.Below(10)) {
    case 0: return RandomFloat(r);
    case 1: return std::bit_cast<uint32_t>(3.14159265f - float(r.Below(3)) * 0.004f);
    case 2: return std::bit_cast<uint32_t>(-3.14159265f + float(r.Below(3)) * 0.004f);
    case 3: return std::bit_cast<uint32_t>(0.0f);
    default: return std::bit_cast<uint32_t>((float(r.Below(20001)) - 10000.0f) / 3000.0f);
  }
}

void AddRanges(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
               FloatRanges floats) {
  std::sort(floats.begin(), floats.end());
  uint32_t at = address;
  const uint32_t end = address + bytes;
  for (const auto& [start, count] : floats) {
    if (start < at || start + 4 * count > end) {
      continue;
    }
    if (start > at) {
      out.Bytes(Read(base, at, start - at));
    }
    out.Floats(base, start, count);
    at = start + 4 * count;
  }
  if (at < end) {
    out.Bytes(Read(base, at, end - at));
  }
}

// The sound manager MenuSound::Play calls through.
constexpr uint32_t kSoundManagerPointer = 0x826C5C80;
constexpr uint32_t kManager = 0xB000;  // the fake manager, its vtable at +0x400
// A leaf that stores r4's low byte, r5 and r6 at r3 + 320, 332 and 336 (the
// menu sounds' harness's recorder): the manager's slot 10, PlayCue2D.
constexpr uint32_t kRecordR4R6 = 0x820A8420;

struct Saved {
  explicit Saved(uint8_t* base)
      : base(base),
        scene(guest::LoadU32(base, kScenePointer)),
        manager(guest::LoadU32(base, kSoundManagerPointer)) {}
  void Restore() const {
    guest::StoreU32(base, kScenePointer, scene);
    guest::StoreU32(base, kSoundManagerPointer, manager);
  }
  uint8_t* base;
  uint32_t scene;
  uint32_t manager;
};

// One case through both versions: the block filled at random, `setup(t)`
// laying the world and the registers (its inputs written back for the
// second run); the block and the stack compared (from `*floor` up when
// given, set by `setup`), the top `saves` bytes of the top function's frame
// and the `skips` ranges (address, bytes) left out, `floats` as floats, then
// r3.
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, uint32_t saves, const FloatRanges& floats,
          const std::string& what, const uint32_t* floor = nullptr,
          const FloatRanges& skips = {},
          const std::function<void(Outputs&)>& extra = {}) {
  const Saved saved(h.base);
  RandomBytes(h.base, block, kBlockBytes, r);
  guest::StoreU32(h.base, kScenePointer, block + kScene);
  guest::StoreU32(h.base, block + kManager, block + kManager + 0x400);
  guest::StoreU32(h.base, block + kManager + 0x400 + 40, kRecordR4R6);
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, kBlockBytes);
  const uint32_t sp = start.r1.u32;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), kBlockBytes);
    guest::StoreU32(h.base, kScenePointer, block + kScene);
    guest::StoreU32(h.base, kSoundManagerPointer, block + kManager);
    Fill(h.base, sp - kStackBytes, kStackBytes + kArgBytes);
    PPCContext t = start;
    t.fpscr.disableFlushModeUnconditional();
    (v == 0 ? recompiled : native)(t, h.base);
    for (const auto& [address, bytes] : skips) {
      Fill(h.base, address, bytes, 0);
    }
    AddRanges(out[v], h.base, block, kBlockBytes, floats);
    const uint32_t low = floor != nullptr ? *floor : sp - kStackBytes;
    AddRanges(out[v], h.base, low, sp - saves - low, floats);
    out[v].Bytes(Read(h.base, sp, kArgBytes));
    if (extra) {
      extra(out[v]);
    }
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  saved.Restore();
  results.Check(out[0], out[1], what);
}

// --- The world ---

struct World {
  uint32_t block;
  uint32_t holder() const { return block + kHolder; }
  uint32_t stage() const { return block + kStage; }
  uint32_t model(int i) const { return block + (i == 0 ? kModel0 : kModel1); }
  uint32_t parts() const { return block + kParts; }
};

// How SceneModel::PollLoad answers for a model: 0 (+5028 or +5040 set), or
// 1 at once (traffic with no body or wheels; a car with no parts, interior,
// cockpit, light kit or wheels,
// unless `keep_parts`).
void PollAnswer(uint8_t* base, uint32_t model, Random& r, bool keep_parts) {
  for (const uint32_t at : {5028u, 5040u, 5092u, 5100u}) {
    guest::StoreU32(base, model + at, 0);
  }
  switch (r.Below(keep_parts ? 3 : 4)) {
    case 0:
      guest::StoreU32(base, model + 5028, 1 + r.Below(5));
      break;
    case 1:
      guest::StoreU32(base, model + 5040, uint32_t(r.Next()) | 1);
      break;
    case 2:
      guest::StoreU32(base, model + 32, 3);
      break;
    default:
      guest::StoreU32(base, model + 32, r.Below(3));
      for (const uint32_t at : {5048u, 5060u, 5068u, 5076u, 5084u}) {
        guest::StoreU32(base, model + at, 0);
      }
      break;
  }
}

// The stage's holder, the stage, the car scene's pool 7 (two models in use;
// the stage's index 0 to 2, 2 past the end giving the first), the parts.
World Lay(uint8_t* base, uint32_t block, Random& r, bool keep_parts) {
  World w{block};
  guest::StoreU32(base, w.holder(), w.stage());
  guest::StoreU32(base, w.stage() + 8, r.Below(3));
  guest::StoreU32(base, w.stage() + 4, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(4));
  const uint32_t scene = block + kScene;
  guest::StoreU32(base, scene + 5028, w.model(0));
  guest::StoreU32(base, w.model(0) + 8, w.model(1));
  guest::StoreU32(base, w.model(1) + 8, 0);
  for (int i = 0; i < 2; i++) {
    guest::StoreU32(base, w.model(i) + 16, 3);
    guest::StoreU32(base, w.model(i) + 48, r.Below(4));
    guest::StoreU32(base, w.model(i) + 5060, w.parts());
    PollAnswer(base, w.model(i), r, keep_parts);
  }
  const uint32_t choices[3] = {0, 1, uint32_t(r.Next())};
  guest::StoreU32(base, w.stage() + 6092, choices[r.Below(3)]);
  guest::StoreU32(base, w.parts() + 2816, choices[r.Below(3)]);
  return w;
}

// The garage camera (stage + 112): set up or not, its preset table and
// presets, the limit.
void LayCamera(uint8_t* base, const World& w, Random& r) {
  const uint32_t camera = w.stage() + 112;
  guest::StoreU32(base, camera, r.Below(5) == 0 ? 0 : uint32_t(r.Next()) | 1);
  guest::StoreU32(base, camera + 292, w.block + kTable);
  // Every index below the limit: one at or past it gives the preset 0,
  // which the original reads from guest address 4 on (unmapped; never seen
  // in play).
  guest::StoreU32(base, camera + 4392, kPresetCount);
  for (uint32_t i = 0; i < 4 * 290; i++) {
    guest::StoreU32(base, w.block + kTable + 4 * i, r.Below(kPresetCount));
  }
  for (uint32_t i = 0; i < kPresetCount; i++) {
    const uint32_t preset = w.block + kPresets + 64 * i;
    guest::StoreU32(base, camera + 296 + 4 * i, preset);
    for (uint32_t k = 0; k < 16; k++) {
      guest::StoreU32(base, preset + 4 * k, RandomFloat(r));
    }
    // The field of view, mostly a camera's (degrees).
    if (r.Below(4) != 0) {
      guest::StoreFloat(base, preset + 12, 20.0f + float(r.Below(800)) / 10.0f);
    }
  }
}

// The animation list (stage + 5600): `count` entries of random types 0 to 7
// (now and then any word).
void LayList(uint8_t* base, const World& w, Random& r, uint32_t count) {
  const uint32_t list = w.stage() + 5600;
  guest::StoreU32(base, list + 448, count);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t e = list + 28 * i;
    guest::StoreU32(base, e, r.Below(10) == 0 ? uint32_t(r.Next()) : r.Below(8));
    guest::StoreU32(base, e + 4, RandomFloat(r));
    guest::StoreU32(base, e + 24, RandomFloat(r));
  }
}

FloatRanges StageFloats(const World& w) {
  const uint32_t s = w.stage(), c = s + 112;
  return {{s + 80, 3}, {c + 100, 3}, {c + 180, 1}, {c + 224, 1}};
}

// The list's entries' floats (+4 and +24) and the frame's entry at its +80
// (Add's 656-byte frame below sp: +84 and +104).
FloatRanges ListFloats(const World& w, uint32_t sp) {
  FloatRanges floats;
  const uint32_t list = w.stage() + 5600;
  for (uint32_t i = 0; i < 16; i++) {
    floats.push_back({list + 28 * i + 4, 1});
    floats.push_back({list + 28 * i + 24, 1});
  }
  for (const uint32_t frame : {sp - 656, sp - 112 - 656}) {
    floats.push_back({frame + 84, 1});
    floats.push_back({frame + 104, 1});
    for (uint32_t i = 0; i < 16; i++) {
      floats.push_back({frame + 112 + 28 * i + 4, 1});
      floats.push_back({frame + 112 + 28 * i + 24, 1});
    }
  }
  return floats;
}

void Small(Context& h, uint32_t block, Random& r) {
  {
    Results results("Plates::Hide (sub_820DF190) and Show (sub_820DF180)");
    for (int c = 0; c < 40; c++) {
      const bool show = c % 2 == 1;
      Case(
          h, results, block, r,
          [&](PPCContext& t) { t.r3.u64 = block + kParts + 176 + 4 * r.Below(16); },
          show ? __imp__sub_820DF180 : __imp__sub_820DF190,
          show ? hooks::FrontEndCarDrawPlatesShow : hooks::FrontEndCarDrawPlatesHide, 0, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageStage::IsBusy (sub_82179BE0)");
    for (int c = 0; c < 80; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const World w = Lay(h.base, block, r, false);
            t.r3.u64 = w.holder();
          },
          __imp__sub_82179BE0, hooks::FrontEndCarDrawIsBusy, 16, {}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageStage::SyncPlates (sub_8217AFE8)");
    for (int c = 0; c < 120; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const World w = Lay(h.base, block, r, true);
            t.r3.u64 = w.holder();
          },
          __imp__sub_8217AFE8, hooks::FrontEndCarDrawSyncPlates, 24, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageStage::SetDrawWord (sub_82178B40)");
    for (int c = 0; c < 40; c++) {
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            const World w = Lay(h.base, block, r, true);
            t.r3.u64 = w.holder();
            t.r4.u64 = r.Next();
          },
          __imp__sub_82178B40, hooks::FrontEndCarDrawSetDrawWord, 24, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

void Commands(Context& h, uint32_t block, Random& r) {
  {
    Results results("GarageStage::StepAngles (sub_82179C88)");
    for (int c = 0; c < 120; c++) {
      World w{block};
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            w = Lay(h.base, block, r, true);
            for (uint32_t k = 0; k < 3; k++) {
              guest::StoreU32(h.base, w.stage() + 80 + 4 * k, RandomAngle(r));
            }
            t.r3.u64 = w.stage() + 5600;
            t.r4.u64 = r.Below(8) == 0 ? r.Next() : 7 + r.Below(9);
            t.r5.u64 = w.stage() + 32;
          },
          __imp__sub_82179C88, hooks::FrontEndCarDrawStepAngles, 0, StageFloats(w),
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageAnims::Add (sub_8217F9E0)");
    for (int c = 0; c < 200; c++) {
      World w{block};
      const uint32_t sp = CallContext(h).r1.u32 & ~0xFu;
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            w = Lay(h.base, block, r, true);
            LayList(h.base, w, r, r.Below(4) == 0 ? 0 : r.Below(14));
            guest::StoreU32(h.base, w.stage() + 112 + 176, RandomFloat(r));
            t.r3.u64 = w.stage() + 5600;
            t.r4.u64 = r.Below(10) == 0 ? r.Next() : r.Below(8);
            t.r5.u64 = r.Below(2) == 0 ? w.stage() + 112 : w.stage() + 32;
            t.r6.u64 = r.Next();
            t.r7.u64 = r.Next();
            const float steps[] = {60.0f, 20.0f, 0.0f, -1.0f};
            t.f1.f64 = r.Below(6) == 0 ? double(std::bit_cast<float>(RandomFloat(r)))
                                       : double(steps[r.Below(4)]);
          },
          __imp__sub_8217F9E0, hooks::FrontEndCarDrawAnimsAdd, 88, ListFloats(w, sp),
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageStage::ApplyPreset (sub_8217D2B0)");
    // The stack from ApplyPreset's frame up: below it is tan's (where tan
    // declines, a NaN, infinite or huge field of view, its original runs
    // there on both sides, saving the callers' registers, which differ;
    // where it doesn't, neither side writes it).
    const uint32_t floor = (CallContext(h).r1.u32 & ~0xFu) - 128;
    for (int c = 0; c < 150; c++) {
      World w{block};
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            w = Lay(h.base, block, r, true);
            LayCamera(h.base, w, r);
            t.r3.u64 = r.Next();
            t.r4.u64 = (r.Next() << 32) | r.Below(51);
            t.r5.u64 = w.stage();
            t.r6.u64 = r.Below(3);
          },
          __imp__sub_8217D2B0, hooks::FrontEndCarDrawApplyPreset, 40, StageFloats(w),
          fmt::format("case {}", c), &floor);
    }
    results.Report();
  }
  {
    Results results("GarageStage::Command (sub_82180108)");
    for (int c = 0; c < 250; c++) {
      World w{block};
      const uint32_t sp = CallContext(h).r1.u32 & ~0xFu;
      uint32_t floor_of = 0;
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            w = Lay(h.base, block, r, true);
            LayCamera(h.base, w, r);
            LayList(h.base, w, r, r.Below(4) == 0 ? 0 : r.Below(14));
            for (uint32_t k = 0; k < 3; k++) {
              guest::StoreU32(h.base, w.stage() + 80 + 4 * k, RandomAngle(r));
            }
            t.r3.u64 = (r.Next() << 32) | w.holder();
            t.r4.u64 = r.Below(10) == 0 ? r.Next() : r.Below(68);
            // The presets: from ApplyPreset's frame up (see its cases).
            const bool preset = int32_t(t.r4.u32) >= 14 && int32_t(t.r4.u32) <= 64;
            floor_of = preset ? sp - 112 - 128 : sp - kStackBytes;
          },
          __imp__sub_82180108, hooks::FrontEndCarDrawCommand, 32,
          [&] {
            FloatRanges floats = ListFloats(w, sp);
            const FloatRanges stage = StageFloats(w);
            floats.insert(floats.end(), stage.begin(), stage.end());
            return floats;
          }(),
          fmt::format("case {}", c), &floor_of,
          // GarageAnims::Add's saved registers: its original (no adapter
          // since its caller is native) saves them on the recompiled side.
          {{sp - 112 - 88, 88}});
    }
    results.Report();
  }
}

// An animation entry at `e`: `type`, its value the stage's +32 (the angles
// at +80 for the turns), the steps mostly a command's, the index 0 to 2,
// the word and done flag 0 or 1.
void LayEntry(uint8_t* base, const World& w, Random& r, uint32_t e, uint32_t type) {
  guest::StoreU32(base, e, type);
  const float steps[] = {60.0f, 20.0f, 1.0f, 0.0f, -60.0f};
  guest::StoreU32(base, e + 4,
                  r.Below(6) == 0 ? RandomFloat(r)
                                  : std::bit_cast<uint32_t>(steps[r.Below(5)]));
  guest::StoreU32(base, e + 8, r.Below(2));
  guest::StoreU32(base, e + 12, w.stage() + 32);
  guest::StoreU32(base, e + 16, r.Below(3));
  guest::StoreU32(base, e + 20, r.Below(2));
}

// The opening values: the bonnet's (+32), the doors' (+52, +56), mostly
// within 0 to a little over the cars' maxima, now and then any float; the
// angles; the doors' kind on the models (+3736).
void LayValues(uint8_t* base, const World& w, Random& r) {
  for (const uint32_t at : {32u, 52u, 56u}) {
    guest::StoreU32(base, w.stage() + at,
                    r.Below(5) == 0 ? RandomFloat(r)
                                    : std::bit_cast<uint32_t>(float(r.Below(130)) / 100.0f));
  }
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, w.stage() + 80 + 4 * k, RandomAngle(r));
  }
  if (r.Below(3) == 0) {
    guest::StoreFloat(base, w.stage() + 84, 0.0f);
  }
  for (int i = 0; i < 2; i++) {
    guest::StoreU32(base, w.model(i) + 3736, r.Below(3));
  }
}

FloatRanges ValueFloats(const World& w) {
  const uint32_t s = w.stage();
  return {{s + 32, 1}, {s + 52, 2}, {s + 80, 3}};
}

void Anims(Context& h, uint32_t block, Random& r) {
  const uint32_t sp = CallContext(h).r1.u32 & ~0xFu;
  struct Runner {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t type, saves, count;
  };
  const Runner runners[] = {
      {"GarageAnims::Turn (sub_82179D78)", __imp__sub_82179D78, hooks::FrontEndCarDrawAnimsTurn,
       0, 0, 150},
      {"GarageAnims::Open (sub_82179DF0)", __imp__sub_82179DF0, hooks::FrontEndCarDrawAnimsOpen,
       2, 40, 150},
      {"GarageAnims::Close (sub_82179EA8)", __imp__sub_82179EA8,
       hooks::FrontEndCarDrawAnimsClose, 3, 24, 150},
  };
  for (const Runner& runner : runners) {
    Results results(runner.name);
    for (uint32_t c = 0; c < runner.count; c++) {
      World w{block};
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            w = Lay(h.base, block, r, true);
            LayValues(h.base, w, r);
            const uint32_t list = w.stage() + 5600;
            LayList(h.base, w, r, 4);
            const uint32_t e = list + 28 * r.Below(4);
            LayEntry(h.base, w, r, e, runner.type);
            t.r3.u64 = list;
            t.r4.u64 = e;
          },
          runner.recompiled, runner.native, runner.saves,
          [&] {
            FloatRanges floats = ListFloats(w, sp);
            const FloatRanges values = ValueFloats(w);
            floats.insert(floats.end(), values.begin(), values.end());
            return floats;
          }(),
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageAnims::Run (sub_8217D120)");
    for (int c = 0; c < 200; c++) {
      World w{block};
      uint32_t floor_of = 0;
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            w = Lay(h.base, block, r, true);
            LayValues(h.base, w, r);
            const uint32_t list = w.stage() + 5600;
            const uint32_t count = r.Below(5) == 0 ? 0 : 1 + r.Below(12);
            LayList(h.base, w, r, count);
            bool doors = false;
            for (uint32_t i = 0; i < count; i++) {
              const uint32_t type = r.Below(8);
              doors = doors || type == 4 || type == 5;
              LayEntry(h.base, w, r, list + 28 * i, type);
            }
            // With the doors' runners (their originals on both sides,
            // saving the callers' registers, which differ), the stack from
            // Run's frame up.
            floor_of = doors ? sp - 624 : sp - kStackBytes;
            t.r3.u64 = list;
          },
          __imp__sub_8217D120, hooks::FrontEndCarDrawAnimsRun, 48,
          [&] {
            FloatRanges floats = ListFloats(w, sp);
            const FloatRanges values = ValueFloats(w);
            floats.insert(floats.end(), values.begin(), values.end());
            for (const uint32_t frame : {sp - 624}) {
              floats.push_back({frame + 84, 1});
              floats.push_back({frame + 104, 1});
              for (uint32_t i = 0; i < 16; i++) {
                floats.push_back({frame + 112 + 28 * i + 4, 1});
                floats.push_back({frame + 112 + 28 * i + 24, 1});
              }
            }
            return floats;
          }(),
          fmt::format("case {}", c), &floor_of,
          // GarageAnims::Open's and Close's saved registers (their
          // originals run on the recompiled side: no adapters).
          {{sp - 624 - 40, 40}});
    }
    results.Report();
  }
}

// The light kit: the scene models ready or not, traffic or not, with or
// without a light kit (its pool 4 object, no model, so
// PartModel::SetTexture does nothing).
void LayKit(uint8_t* base, const World& w, Random& r) {
  for (int i = 0; i < 2; i++) {
    const uint32_t m = w.model(i);
    guest::StoreU32(base, m + 5036, r.Below(4) == 0 ? 0 : 1 + r.Below(3));
    guest::StoreU32(base, m + 32, r.Below(4) == 0 ? 3 : r.Below(3));
    guest::StoreU32(base, m + 5084, r.Below(5) == 0 ? 0 : w.block + kKit);
  }
  guest::StoreU32(base, w.block + kKit + 28, w.block + kPool4);
  guest::StoreU32(base, w.block + kKit + 13152, 0);
}

FloatRanges KitFloats(const World& w) {
  FloatRanges floats;
  for (int i = 0; i < 2; i++) {
    const uint32_t m = w.model(i);
    floats.push_back({m + 4232, 3});
    floats.push_back({m + 4248, 3});
    floats.push_back({m + 4264, 3});
  }
  return floats;
}

void LightKit(Context& h, uint32_t block, Random& r) {
  {
    Results results("SceneModel::ApplyLightKit (sub_820D9D10)");
    for (int c = 0; c < 80; c++) {
      World w{block};
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            w = Lay(h.base, block, r, true);
            LayKit(h.base, w, r);
            t.r3.u64 = w.model(int(r.Below(2)));
          },
          __imp__sub_820D9D10, hooks::FrontEndCarDrawApplyLightKit, 0, KitFloats(w),
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("GarageStage::SetLightKit (sub_82179678)");
    for (int c = 0; c < 150; c++) {
      World w{block};
      Case(
          h, results, block, r,
          [&](PPCContext& t) {
            w = Lay(h.base, block, r, true);
            LayKit(h.base, w, r);
            t.r3.u64 = w.holder();
            t.r4.u64 = r.Below(8) == 0 ? r.Next() : r.Below(5);
            t.r5.u64 = r.Next();
            // The table's 8 words, or the word after them (unwritten): an
            // index past 8 reads the original's saved registers (f29 on),
            // which the plain C++ doesn't save (no caller in play passes one).
            t.r6.u64 = r.Below(9);
            t.f1.f64 = double(std::bit_cast<float>(RandomFloat(r)));
            t.f2.f64 = r.Below(10) == 0 ? 1e300 : double(std::bit_cast<float>(RandomFloat(r)));
            t.f3.f64 = double(std::bit_cast<float>(RandomFloat(r)));
          },
          __imp__sub_82179678, hooks::FrontEndCarDrawSetLightKit, 56, KitFloats(w),
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// GarageStage::SetLights: the stage's lights (stage + 4512, 1,088 bytes)
// random, the spots' cone angles mostly in degrees; the scene lights'
// constants compared (as floats where computed: the lights' and spots'
// positions, the spots' directions and cones; the copies exactly) and put
// back after each case, its frame's temporaries (floats at +80 to +143).
void Lights(Context& h, uint32_t block, Random& r) {
  constexpr uint32_t kGlobals = 0x8296B950, kGlobalBytes = 0x8296BCE0 - 0x8296B950;
  constexpr uint32_t kAmbient = 0x8265ED00;
  const std::vector<uint8_t> globals = Read(h.base, kGlobals, kGlobalBytes);
  const std::vector<uint8_t> ambient = Read(h.base, kAmbient, 16);
  const uint32_t sp = CallContext(h).r1.u32 & ~0xFu;
  Results results("GarageStage::SetLights (sub_8217BA20)");
  for (int c = 0; c < 200; c++) {
    World w{block};
    const uint32_t lights = block + kStage + 4512;
    Case(
        h, results, block, r,
        [&](PPCContext& t) {
          w = Lay(h.base, block, r, true);
          // Mostly finite values, a case in four with NaNs, infinities and
          // denormals among them.
          const bool any = r.Below(4) == 0;
          for (uint32_t k = 0; k < 1088 / 4; k++) {
            guest::StoreU32(h.base, lights + 4 * k,
                            any ? RandomFloat(r)
                                : std::bit_cast<uint32_t>(car_draw_shared::Spread(r, 50.0f)));
          }
          for (uint32_t i = 0; i < 4; i++) {
            const uint32_t q = lights + 304 + 96 * i;
            if (r.Below(4) != 0) {
              guest::StoreFloat(h.base, q + 64, float(r.Below(90)));
              guest::StoreFloat(h.base, q + 68, float(r.Below(120)));
            }
            if (r.Below(8) == 0) {
              // A zero-length direction.
              for (uint32_t k = 0; k < 3; k++) {
                guest::StoreU32(h.base, q + 16 + 4 * k, guest::LoadU32(h.base, q + 4 * k));
              }
            }
          }
          for (uint32_t k = 0; k < kGlobalBytes / 4; k++) {
            guest::StoreU32(h.base, kGlobals + 4 * k, uint32_t(r.Next()));
          }
          t.r3.u64 = lights;
          t.r4.u64 = r.Below(3) == 0 ? r.Next() : r.Below(2);
        },
        __imp__sub_8217BA20, hooks::FrontEndCarDrawSetLights, 208,
        {{sp - 368 + 80, 16}},
        fmt::format("case {}", c), nullptr, {},
        [&](Outputs& o) {
          o.Bytes(Read(h.base, kAmbient, 16));
          o.Floats(h.base, 0x8296B950, 12);       // the lights' +32 (y negated)
          o.Bytes(Read(h.base, 0x8296B980, 96));  // their +48 and +64
          o.Floats(h.base, 0x8296B9E0, 16);       // the spots' positions
          o.Bytes(Read(h.base, 0x8296BA20, 0x8296BBE0 - 0x8296BA20));
          o.Floats(h.base, 0x8296BBE0, 16);       // the directions
          o.Bytes(Read(h.base, 0x8296BC20, 0x40));
          o.Floats(h.base, 0x8296BC60, 16);       // the cones
          o.Bytes(Read(h.base, 0x8296BCA0, 0x8296BCE0 - 0x8296BCA0));
        });
  }
  Write(h.base, kGlobals, globals.data(), kGlobalBytes);
  Write(h.base, kAmbient, ambient.data(), 16);
  results.Report();
}

// Floor::Draw over the model draws' world of the car draw's harness
// (car_draw_shared: the part model at its block's +0x100, shape 1 a copy
// of shape 0),
// the scene 4,160 bytes before the model (so its +4572 is the model's shape
// count, +412), a texture provider always, camera pass 0's view-projection
// the identity (put back
// after, as the car draw's globals). Floor::Draw's frame compared (its
// saved registers left out) and Model::DrawShape's back chain; not
// Model::DrawShape's frame and below: where its plain C++ declines, its
// original runs on both sides and saves Floor::Draw's registers there,
// which differ (Model::DrawShape's own cases compare its frames).
void Floors(Context& h, Random& r) {
  constexpr uint32_t kBefore = 0x2000;
  const uint32_t allocation = memory::Alloc(kBefore + kBlockBytes + 16, 0);
  const uint32_t block = (allocation + kBefore + 15) & ~15u;
  const uint32_t model = block + 0x100;
  const uint32_t scene = model - 4160;
  Results results("Floor::Draw (sub_820C8480)");
  car_draw_shared::FloatRanges floats = car_draw_shared::AlphaFloats();
  floats.push_back({0x500 + 48, 4});
  for (int c = 0; c < 60; c++) {
    car_draw_shared::Case(
        h, results, block, r,
        [&](PPCContext& t) {
          car_draw_shared::ModelWorld(h, r, block, 0);
          // Shape 1 a copy of shape 0's record (544 bytes at the model's
          // +40).
          const std::vector<uint8_t> shape0 = Read(h.base, block + 0x2000, 544);
          Write(h.base, block + 0x2000 + 544, shape0.data(), 544);
          // Always a texture provider (the world's at +0x700): Model::DrawShape
          // binds its textures through it.
          guest::StoreU32(h.base, model + 48, block + 0x700);
          const uint32_t counts[] = {0, 1, 2, 2, 0xFFFFFFFFu};
          guest::StoreU32(h.base, model + 412, counts[r.Below(5)]);
          guest::StoreU32(h.base, scene + 4, r.Below(4) == 0 ? 1 : 0);
          car_draw_shared::StoreFloats(h.base, 0x826BAC00 + 144,
                                       {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
          t.r3.u64 = scene;
          t.r4.u64 = block + 0x500;
          t.r5.u64 = r.Below(5) == 0 ? 0 : r.Next() | 1;
          t.r24.u64 = r.Next();
        },
        __imp__sub_820C8480, hooks::FrontEndCarDrawFloorDraw, floats, fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& o) {
          const uint32_t sp = t.r1.u32;
          o.Bytes(Read(h.base, sp - 144, 144 - 64));
          o.Floats(h.base, sp - 64, 4);
          o.Word(guest::LoadU32(h.base, sp - 144 - 208));
          o.Word(guest::LoadU32(h.base, scene + 4));
        });
  }
  results.Report();
  memory::Free(allocation);
}

}  // namespace

void FrontEndCarDraw(Context& h) {
  Random r(0x66726f6e74656e64ull);  // "frontend"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Small(h, block, r);
  Commands(h, block, r);
  Anims(h, block, r);
  LightKit(h, block, r);
  Lights(h, block, r);
  memory::Free(allocation);
  Floors(h, r);
}

}  // namespace harness
