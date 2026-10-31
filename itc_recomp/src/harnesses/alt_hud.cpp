// The alternate HUD (src/alt_hud/, docs/functions/alt-hud.md) against the
// recompiled functions. Each case fills a world of our own with random words
// from the case's seed: a misc block (the holder's +1004 and its object and
// menu list, the movie table's pairs, the action mappers and their vtables,
// the build, the input records), a UI movie block (movie 22's header and a
// pool of nodes whose child entries point back into the pool, for
// UiMovie::FindNode), the vehicles (pointed at by 0x826C2E34) and the race
// data (its racer records, whose +48 car objects PowerClass sets up), plus
// the stack below the call's stack pointer and the caller's area above it;
// shapes the fields the function tests; runs the original (with
// --native_alt_hud off) or the register steps (hooks/alt_hud.h); and
// compares the blocks (the object's curves and bar with harness::Outputs'
// floats, everything else exactly), the original's frame (its register
// saves left out), r1, r3 and f1. The globals the cases point elsewhere are
// put back after each run.

#include "harness.h"

#include "alt_hud/alt_hud.h"
#include "base/flags.h"
#include "base/log.h"
#include "hooks/alt_hud.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "race_hud_object/race_hud_object.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <cstring>
#include <functional>
#include <limits>
#include <set>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_alt_hud);

namespace harness {

// Vehicle::SetupFromCar's cases (src/harnesses/car_physics.cpp), for the car
// objects PowerClass sets up.
void WriteSetupFromCarCase(uint8_t* base, uint32_t data, uint32_t car, uint32_t r1,
                           uint32_t stack_end, uint64_t seed, int group);
void RunSetupFromCarOriginal(GuestFunction recompiled, PPCContext& ctx, uint8_t* base);

namespace {

namespace ah = alt_hud;
namespace rh = race_hud_object;

constexpr uint32_t kVehicles = 0x826C2E34;  // Vehicle::Get's array
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kVehicleCount = 4;
constexpr uint32_t kMiscBytes = 0x3000;
constexpr uint32_t kRaceBytes = 4 * rh::kRecordBytes + 0x1000;
// The movie block: movie 22's header (0xA000) at +0, then the node pool.
constexpr uint32_t kMovieBytes = 0xA000;
constexpr uint32_t kNodeBytes = 0x800;
constexpr uint32_t kNodeCount = 24;
constexpr uint32_t kMoviesBytes = kMovieBytes + kNodeBytes * kNodeCount;
constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

class SavedBytes {
 public:
  SavedBytes(uint8_t* base, std::vector<std::pair<uint32_t, uint32_t>> ranges)
      : base_(base), ranges_(std::move(ranges)) {
    for (const auto& [address, bytes] : ranges_) {
      saved_.push_back(Read(base_, address, bytes));
    }
  }
  ~SavedBytes() {
    for (size_t i = 0; i < ranges_.size(); i++) {
      std::memcpy(base_ + ranges_[i].first, saved_[i].data(), ranges_[i].second);
    }
  }

 private:
  uint8_t* base_;
  std::vector<std::pair<uint32_t, uint32_t>> ranges_;
  std::vector<std::vector<uint8_t>> saved_;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& random) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(random.Next()));
  }
}

uint32_t Bits(float value) { return std::bit_cast<uint32_t>(value); }

// A float: mostly ordinary values in `range`, sometimes an edge.
float AnyFloat(Random& r, float range) {
  switch (r.Below(16)) {
    case 0:
      return std::numeric_limits<float>::quiet_NaN();
    case 1:
      return std::numeric_limits<float>::infinity();
    case 2:
      return -std::numeric_limits<float>::infinity();
    case 3:
      return 0.0f;
    case 4:
      return -0.0f;
    case 5:
      return std::bit_cast<float>(uint32_t(r.Next()));
    default:
      return (float(r.Below(1u << 20)) / float(1u << 20) * 2.0f - 1.0f) * range;
  }
}

double AnyDouble(Random& r, float range) {
  if (r.Below(8) == 0) {
    return std::bit_cast<double>(r.Next());
  }
  return double(AnyFloat(r, range)) + (r.Below(2) == 0 ? 0.0 : 1e-9);
}

struct World {
  World()
      : misc(kMiscBytes),
        movies(kMoviesBytes),
        vehicles(kVehicleBytes * kVehicleCount),
        race(kRaceBytes) {}
  bool ok() const { return misc.address && movies.address && vehicles.address && race.address; }
  uint32_t holder() const { return misc.address; }
  uint32_t object() const { return misc.address + 0x100; }
  uint32_t list() const { return misc.address + 0x300; }
  uint32_t pairs() const { return misc.address + 0x400; }
  uint32_t vtable(uint32_t i) const { return misc.address + 0x500 + 0x40 * i; }
  uint32_t mapper(uint32_t i) const { return misc.address + 0x600 + 0x200 * i; }
  uint32_t build() const { return misc.address + 0x1000; }
  uint32_t input(uint32_t i) const { return misc.address + 0x2000 + 0x20 * i; }
  uint32_t movie() const { return movies.address; }
  uint32_t node(uint32_t i) const { return movies.address + kMovieBytes + kNodeBytes * i; }
  uint32_t vehicle(uint32_t i) const { return vehicles.address + kVehicleBytes * i; }
  uint32_t record(uint32_t i) const { return race.address + rh::kRecordBytes * i; }
  Block misc;  // the holder's +1004 (+0), its object (+0x100, 0x200 bytes) and menu list
               // (+0x300), the movie table's pairs (+0x400), the mappers' vtables
               // (+0x500) and the mappers (+0x600), the build (+0x1000), the input
               // records (+0x2000)
  Block movies;
  Block vehicles;
  Block race;  // the racer records (3,936 bytes each) and room for the last one's car
};

// The UI movies' update (which UiMovie::Run jumps to: r3 the movie, r4 draw
// only), recorded and answered with a word from the case's generator,
// through the UI movies' stand-in (ui_movies_hooks::g_update_callee_stand_in).
struct Call {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<Call>* g_calls = nullptr;
Random g_answers;

bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_calls == nullptr || function != 0x8211D2F8) {
    return false;
  }
  g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_answers.Next();
  return true;
}

// The holder (+0 the object, +4 the menu list); movie 22 in the movie table
// (up to 24 pairs of the ids 0 to 23 in a random order, 22 always there, its
// value the movie); the movie's live scene (+16) a pool node; each node's
// children's range (+100, +104: up to 4 from below 100), child index (+108)
// 0 to 3 and its child entries (+128 on) pool nodes; the object's car (+240)
// one of the vehicles, its player (+260) 0 to 3; the vehicles' build
// (+5712); the menu list's controller (+60) 0 to 3, each controller's entry
// 0 to 3, each entry's mapper one of ours (its vtable's slot 10 Pressed,
// now and then Held) and +4 now and then 1; the input records.
void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, w.holder(), w.object());
  guest::StoreU32(base, w.holder() + 4, w.list());
  uint32_t ids[24];
  for (uint32_t i = 0; i < 24; i++) {
    ids[i] = i;
  }
  for (uint32_t i = 23; i > 0; i--) {
    std::swap(ids[i], ids[r.Below(i + 1)]);
  }
  const uint32_t count = 1 + r.Below(24);
  bool has22 = false;
  for (uint32_t i = 0; i < count; i++) {
    has22 = has22 || ids[i] == ah::kMovieId;
  }
  if (!has22) {
    ids[r.Below(count)] = ah::kMovieId;
  }
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, w.pairs() + 8 * i, ids[i]);
    if (ids[i] == ah::kMovieId) {
      guest::StoreU32(base, w.pairs() + 8 * i + 4, w.movie());
    }
  }
  guest::StoreU32(base, ui_movies::kMovieTable, w.pairs());
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 24);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
  guest::StoreU32(base, w.movie() + ui_movies::kMovieLive, w.node(r.Below(kNodeCount)));
  for (uint32_t k = 0; k < kNodeCount; k++) {
    const uint32_t node = w.node(k);
    guest::StoreU32(base, node + 100, r.Below(100));
    guest::StoreU32(base, node + 104, r.Below(5));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t i = 128; i < kNodeBytes; i += 8) {
      guest::StoreU32(base, node + i, w.node(r.Below(kNodeCount)));
    }
  }
  guest::StoreU32(base, kVehicles, w.vehicles.address);
  guest::StoreU32(base, w.object() + ah::kCar, w.vehicle(r.Below(kVehicleCount)));
  guest::StoreU32(base, w.object() + ah::kPlayer, r.Below(8) == 0 ? 0x100 + r.Below(4) : r.Below(4));
  for (uint32_t v = 0; v < kVehicleCount; v++) {
    guest::StoreU32(base, w.vehicle(v) + 5712, w.build());
  }
  guest::StoreU32(base, w.list() + 60, r.Below(4));
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreU32(base, kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, kMapperEntries + 8 * c, w.mapper(c));
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, w.mapper(c), w.vtable(c));
    guest::StoreU32(base, w.vtable(c) + 40, r.Below(5) == 0 ? 0x8210BE70 : 0x8210BF40);
    guest::StoreU32(base, ah::kInputRecords + 4 * c, w.input(c));
  }
  guest::StoreU32(base, w.list() + 24, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  guest::StoreU32(base, rh::kRaceData, w.race.address);
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// What a case compares besides the blocks: the original's frame
// [sp - frame, sp - saves) (its register saves left out), and f1.
struct Spec {
  uint32_t frame = 0;
  uint32_t saves = 0;
  bool f1 = false;
  // The original runs with its callees' native flags off (PowerClass's
  // Vehicle::SetupFromCar).
  bool setup_from_car = false;
  // The frame's float words (their offsets from its bottom, sp - frame):
  // the ones written with stfs (PowerClass's power curve and car data, as
  // their harnesses compare them).
  std::set<uint32_t> frame_floats;
  void FrameFloats(uint32_t offset, uint32_t words) {
    for (uint32_t i = 0; i < words; i++) {
      frame_floats.insert(offset + 4 * i);
    }
  }
};

// The object's float words: the two curves (+4 to +228) and the bar's
// value (+228); everything else exactly.
void ObjectOutputs(Outputs& out, uint8_t* base, const World& w) {
  out.Bytes(Read(base, w.misc.address, w.object() + 4 - w.misc.address));
  out.Floats(base, w.object() + 4, 57);
  out.Bytes(Read(base, w.object() + 232, w.misc.address + w.misc.bytes - (w.object() + 232)));
}

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const Spec& spec,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{kVehicles, 4},
                              {rh::kRaceData, 4},
                              {ui_movies::kMovieTable, 16},
                              {ah::kInputRecords, 16},
                              {kMapperEntries, 32},
                              {kControllerEntries, 16}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    WriteRandom(h.base, w.movies.address, w.movies.bytes, r);
    WriteRandom(h.base, w.vehicles.address, w.vehicles.bytes, r);
    WriteRandom(h.base, w.race.address, w.race.bytes, r);
    ShapeWorld(h.base, w, r);
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    g_answers = Random(r.Next());
    std::vector<Call> calls;
    g_calls = &calls;
    const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
        ui_movies_hooks::g_update_callee_stand_in;
    ui_movies_hooks::g_update_callee_stand_in = StandIn;
    ITC_FLAG(native_alt_hud) = v == 1;
    if (v == 0 && spec.setup_from_car) {
      RunSetupFromCarOriginal(recompiled, t, h.base);
    } else {
      (v == 0 ? recompiled : native)(t, h.base);
    }
    ITC_FLAG(native_alt_hud) = true;
    ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
    g_calls = nullptr;
    out[v].Word(uint32_t(calls.size()));
    for (const Call& call : calls) {
      out[v].Word(call.function);
      out[v].Word(call.lr);
      out[v].Word(call.r1);
      for (const uint64_t x : {call.r3, call.r4}) {
        out[v].Word(uint32_t(x >> 32));
        out[v].Word(uint32_t(x));
      }
    }
    ObjectOutputs(out[v], h.base, w);
    out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
    out[v].Bytes(Read(h.base, w.vehicles.address, w.vehicles.bytes));
    out[v].Bytes(Read(h.base, w.race.address, w.race.bytes));
    if (spec.frame > spec.saves) {
      const uint32_t bottom = sp - spec.frame;
      uint32_t at = bottom;
      for (const uint32_t offset : spec.frame_floats) {
        if (bottom + offset > at) {
          out[v].Bytes(Read(h.base, at, bottom + offset - at));
        }
        out[v].Floats(h.base, bottom + offset, 1);
        at = bottom + offset + 4;
      }
      out[v].Bytes(Read(h.base, at, sp - spec.saves - at));
    }
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    if (spec.f1) {
      out[v].Double(t.f1.u64);
    }
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, const Spec& spec) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, spec,
         fmt::format("case {}", c));
  }
  results.Report();
}

// A curve at `curve`: up to 12 points (the count 0 to 12 at +108, sometimes
// any word) with x ascending, +88 the largest x and +92 the smallest (or
// random bounds).
void ShapeCurve(uint8_t* base, uint32_t curve, Random& r) {
  const uint32_t count = r.Below(8) == 0 ? uint32_t(r.Next()) % 40 : r.Below(13);
  float x = AnyFloat(r, 100.0f);
  if (x != x || x == std::numeric_limits<float>::infinity()) {
    x = -50.0f;
  }
  const uint32_t points = count < 11 ? count : 11;
  for (uint32_t i = 0; i < points; i++) {
    guest::StoreFloat(base, curve + 8 * i, x);
    guest::StoreU32(base, curve + 8 * i + 4, Bits(AnyFloat(r, 500.0f)));
    x += r.Below(4) == 0 ? 0.0f : float(r.Below(1000)) / 10.0f;
  }
  guest::StoreU32(base, curve + 108, count);
  if (r.Below(4) != 0 && points > 0) {
    guest::StoreU32(base, curve + 88, guest::LoadU32(base, curve + 8 * (points - 1)));
    guest::StoreU32(base, curve + 92, guest::LoadU32(base, curve));
  } else {
    guest::StoreU32(base, curve + 88, Bits(AnyFloat(r, 200.0f)));
    guest::StoreU32(base, curve + 92, Bits(AnyFloat(r, 200.0f)));
  }
}

// A node's children's range: its first child index (+100) and count (+104).
void ShapeGroup(uint8_t* base, uint32_t node, Random& r) {
  guest::StoreU32(base, node + 100, r.Below(100));
  guest::StoreU32(base, node + 104, r.Below(8) == 0 ? uint32_t(-int32_t(r.Below(3))) : r.Below(5));
}

uint32_t MovieNode(uint8_t* base, const World& w, uint32_t sub_scene, uint32_t child) {
  return ui_movies::FindNode(base, w.movie(), sub_scene, child);
}

// The extra gauges' slots: 0 to 2, -1, now and then any word.
void ShapeSlots(uint8_t* base, const World& w, Random& r) {
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, w.object() + ah::kSlots + 4 * k,
                    r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(4) - 1);
  }
}

// The table's indices whose curve is one of the three (the words at
// 0x8200FEE8 + 16 x index), for the racer records' +60.
std::vector<uint32_t> CurveIndices(uint8_t* base) {
  std::vector<uint32_t> indices;
  for (uint32_t i = 0; i < 16; i++) {
    if (guest::LoadU32(base, 0x8200FEE8 + 16 * i) < 3) {
      indices.push_back(i);
    }
  }
  return indices;
}

// The car object PowerClass sets up (`car`, a racer record's +48), as
// Vehicle::SetupFromCar's cases shape one, its car data in PowerClass's
// frame (`power_sp` - 3040 + 1152) and the stack below it filled; block
// 2's +12 (the stock power) now and then 0.0 or huge; the record's index
// (+60, the car's +12) one of `indices`.
void ShapePowerCar(uint8_t* base, uint32_t car, uint32_t power_sp, uint32_t stack_end,
                   const std::vector<uint32_t>& indices, Random& r) {
  const uint32_t frame = power_sp - 3040;
  WriteSetupFromCarCase(base, frame + 1152, car, frame, stack_end, r.Below(10000),
                        r.Below(4) == 0 ? 1 : 0);
  switch (r.Below(4)) {
    case 0:
      guest::StoreFloat(base, car + 1900, 0.0f);
      break;
    case 1:
      guest::StoreFloat(base, car + 1900, 1e9f);
      break;
    case 2:
      guest::StoreFloat(base, car + 1900, float(r.Below(4000)) / 10.0f);
      break;
    default:
      break;
  }
  if (!indices.empty()) {
    guest::StoreU32(base, car + 12, indices[r.Below(uint32_t(indices.size()))]);
  }
}

void SetUps(Context& h, const World& w) {
  const std::vector<uint32_t> indices = CurveIndices(h.base);
  ITC_LOG_INFO("harness: alt_hud: {} of the curve table's first 16 indices", indices.size());
  {
    // The car a racer record's; its frame and SetupFromCar's below it
    // compared as the frame (the callees' frames below are their harnesses').
    Spec spec;
    spec.frame = 3040;
    spec.saves = 24;
    spec.setup_from_car = true;
    spec.FrameFloats(80, 256);       // the power curve (PowerCurve::FromEngine's)
    spec.FrameFloats(80 + 1040, 6);
    spec.FrameFloats(1152, 0x740 / 4);  // the car data (Vehicle::SetupFromCar's)
    Group(
        h, w, "AltHud::PowerClass (sub_82216B08)", 150,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t car = w.record(r.Below(4)) + 48;
          ShapePowerCar(h.base, car, t.r1.u32, t.r1.u32, indices, r);
          t.r3.u64 = r.Next();
          t.r4.u64 = car;
        },
        __imp__sub_82216B08, hooks::AltHudPowerClass, spec);
  }
  {
    Spec spec;
    spec.frame = 496;
    spec.saves = 56;
    spec.f1 = true;
    spec.setup_from_car = true;
    Group(
        h, w, "AltHud::SetUpGauges (sub_82216C40)", 150,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t player = r.Below(4);
          guest::StoreU32(h.base, w.object() + ah::kPlayer, player);
          ShapePowerCar(h.base, w.record(player) + 48, t.r1.u32 - 496, t.r1.u32 - 496, indices,
                        r);
          t.r3.u64 = w.holder();
          t.f1.f64 = AnyDouble(r, 10.0f);
        },
        __imp__sub_82216C40, hooks::AltHudSetUpGauges, spec);
  }
  {
    // The build's five entries' types 0 to 5 (or any) and its +12 now and
    // then 10.
    Spec spec;
    spec.frame = 160;
    spec.saves = 48;
    Group(
        h, w, "AltHud::SetUpExtraGauges (sub_822178A0)", 300,
        [&](PPCContext& t, Random& r, const World& w) {
          for (uint32_t k = 0; k < 5; k++) {
            guest::StoreU32(h.base, w.build() + 1832 + 12 * k,
                            r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(6));
          }
          guest::StoreU32(h.base, w.build() + 12, r.Below(4) == 0 ? 10 : r.Below(12));
          t.r3.u64 = w.holder();
        },
        __imp__sub_822178A0, hooks::AltHudSetUpExtraGauges, spec);
  }
  {
    Spec spec;
    spec.frame = 128;
    spec.saves = 40;
    Group(
        h, w, "AltHud::ShowGauge (sub_82217380)", 100,
        [&](PPCContext& t, Random& r, const World&) {
          t.r3.u64 = r.Next();
          t.r4.u64 = r.Below(3);
          t.r5.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
        },
        __imp__sub_82217380, hooks::AltHudShowGauge, spec);
  }
  {
    Spec spec;
    spec.frame = 112;
    spec.saves = 24;
    Group(
        h, w, "AltHud::ShowPanels (sub_82216D78)", 100,
        [&](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.holder(); },
        __imp__sub_82216D78, hooks::AltHudShowPanels, spec);
  }
  {
    // The player 0 to 3 (r4); the record's car as PowerClass's cases, the
    // build as SetUpExtraGauges'.
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    spec.f1 = true;
    spec.setup_from_car = true;
    Group(
        h, w, "AltHud::SetUp (sub_82217C70)", 150,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t player = r.Below(4);
          ShapePowerCar(h.base, w.record(player) + 48, t.r1.u32 - 96 - 496,
                        t.r1.u32 - 96 - 496, indices, r);
          for (uint32_t k = 0; k < 5; k++) {
            guest::StoreU32(h.base, w.build() + 1832 + 12 * k, r.Below(6));
          }
          guest::StoreU32(h.base, w.build() + 12, r.Below(4) == 0 ? 10 : r.Below(12));
          t.r3.u64 = w.holder();
          t.r4.u64 = player;
          t.f1.f64 = AnyDouble(r, 10.0f);
        },
        __imp__sub_82217C70, hooks::AltHudSetUp, spec);
  }
}

void Setters(Context& h, const World& w) {
  {
    Spec spec;
    spec.frame = 160;
    spec.saves = 72;
    spec.f1 = true;
    Group(
        h, w, "AltHud::SetSpeed (sub_82216E00)", 400,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeGroup(h.base, MovieNode(h.base, w, 0xFFFFFFFFu, 21), r);
          t.r3.u64 = r.Next();
          t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(1100);
          t.f1.f64 = AnyDouble(r, 10.0f);
        },
        __imp__sub_82216E00, hooks::AltHudSetSpeed, spec);
  }
  {
    // The slots 0 to 2 or other words; the digits' originals run.
    Spec spec;
    spec.frame = 144;
    spec.saves = 56;
    Group(
        h, w, "AltHud::UpdateExtraGauges (sub_82217A30)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeSlots(h.base, w, r);
          t.r3.u64 = w.holder();
          t.f1.f64 = AnyDouble(r, 300.0f);
          t.r5.u64 = r.Below(3) == 0 ? r.Next() : r.Below(200);
          t.r6.u64 = r.Below(3) == 0 ? r.Next() : r.Below(200);
        },
        __imp__sub_82217A30, hooks::AltHudUpdateExtraGauges, spec);
  }
  for (int which = 0; which < 2; which++) {
    Spec spec;
    spec.frame = 112;
    spec.saves = 16;
    Group(
        h, w, which ? "AltHud::SetNode25 (sub_822174E0)" : "AltHud::SetGear (sub_82217460)",
        150,
        [&](PPCContext& t, Random& r, const World&) {
          t.r3.u64 = r.Next();
          t.r4.u64 = r.Below(4) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(10)) - 2));
        },
        which ? __imp__sub_822174E0 : __imp__sub_82217460,
        which ? hooks::AltHudSetNode25 : hooks::AltHudSetGear, spec);
  }
  for (int boost = 0; boost < 2; boost++) {
    Spec spec;
    spec.frame = 112;
    spec.saves = 16;
    Group(
        h, w, boost ? "AltHud::SetBoost (sub_82217538)" : "AltHud::SetTach (sub_822173E8)", 300,
        [&, boost](PPCContext& t, Random& r, const World& w) {
          ShapeCurve(h.base, w.object() + (boost ? ah::kBoost : ah::kTach), r);
          t.r3.u64 = w.holder();
          t.f1.f64 = AnyDouble(r, 120.0f);
        },
        boost ? __imp__sub_82217538 : __imp__sub_822173E8,
        boost ? hooks::AltHudSetBoost : hooks::AltHudSetTach, spec);
  }
  {
    // +232 and +236 (the range) floats or anything.
    Spec spec;
    spec.frame = 112;
    spec.saves = 24;
    Group(
        h, w, "AltHud::SetBar (sub_822175A0)", 300,
        [&](PPCContext& t, Random& r, const World& w) {
          guest::StoreU32(h.base, w.object() + 232, Bits(AnyFloat(r, 100.0f)));
          guest::StoreU32(h.base, w.object() + 236, Bits(AnyFloat(r, 100.0f)));
          t.r3.u64 = w.holder();
          t.f1.f64 = AnyDouble(r, 100.0f);
        },
        __imp__sub_822175A0, hooks::AltHudSetBar, spec);
  }
  {
    static constexpr struct {
      const char* name;
      GuestFunction recompiled, native;
    } kInputs[3] = {
        {"AltHud::SetThrottle (sub_82217638)", __imp__sub_82217638, hooks::AltHudSetThrottle},
        {"AltHud::SetBrake (sub_82217690)", __imp__sub_82217690, hooks::AltHudSetBrake},
        {"AltHud::SetSteering (sub_822176E8)", __imp__sub_822176E8, hooks::AltHudSetSteering},
    };
    for (const auto& in : kInputs) {
      Spec spec;
      spec.frame = 112;
      spec.saves = 16;
      Group(
          h, w, in.name, 200,
          [&](PPCContext& t, Random& r, const World&) {
            t.r3.u64 = r.Next();
            t.f1.f64 = r.Below(3) == 0 ? AnyDouble(r, 2.0f)
                                       : double(float(int32_t(r.Below(65536)) - 32768) /
                                                32767.0f);
          },
          in.recompiled, in.native, spec);
    }
  }
}

void Ticks(Context& h, const World& w) {
  {
    // The menu list's X press (the mapper's pressed words, the entry's +4,
    // the list's +24); the car's values (a speed in m/s, the extra gauges'
    // floats, the gear, the tachometer's +2224, the bar, the boost, the
    // build's +3892), the curves, the digits' group, the slots and the
    // input record's halfwords.
    Spec spec;
    spec.frame = 128;
    spec.saves = 40;
    spec.f1 = true;
    Group(
        h, w, "AltHud::Tick (sub_82217AC8)", 400,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t car = guest::LoadU32(h.base, w.object() + ah::kCar);
          const bool ordinary = r.Below(4) != 0;
          guest::StoreFloat(h.base, car + 48, ordinary ? float(r.Below(90)) : AnyFloat(r, 400.0f));
          for (const uint32_t offset : {6252u, 6256u, 6260u, 2224u, 2420u, 2944u}) {
            guest::StoreFloat(h.base, car + offset,
                              ordinary ? float(r.Below(200)) / 2.0f : AnyFloat(r, 300.0f));
          }
          guest::StoreU32(h.base, car + 2252, r.Below(9) - 1);
          ShapeCurve(h.base, w.object() + ah::kTach, r);
          ShapeCurve(h.base, w.object() + ah::kBoost, r);
          ShapeGroup(h.base, MovieNode(h.base, w, 0xFFFFFFFFu, 21), r);
          for (uint32_t k = 0; k < 3; k++) {
            guest::StoreU32(h.base, w.object() + ah::kSlots + 4 * k, r.Below(4) - 1);
          }
          guest::StoreU32(h.base, w.object() + 232, Bits(AnyFloat(r, 100.0f)));
          guest::StoreU32(h.base, w.object() + 236, Bits(AnyFloat(r, 100.0f)));
          t.r3.u64 = w.holder();
        },
        __imp__sub_82217AC8, hooks::AltHudTick, spec);
  }
  {
    // UiMovie::Run's update recorded (its r1, lr, r3 and r4 compared).
    Spec spec;
    spec.frame = 96;
    spec.saves = 8;
    Group(
        h, w, "AltHud::RunMovie (sub_82216C00)", 100,
        [&](PPCContext& t, Random& r, const World&) { t.r3.u64 = r.Next(); },
        __imp__sub_82216C00, hooks::AltHudRunMovie, spec);
  }
  {
    // MenuList::PressedX alone.
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "MenuList::PressedX (sub_8220BF00)", 300,
        [&](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.list(); },
        __imp__sub_8220BF00, hooks::AltHudMenuListPressedX, spec);
  }
}

// MenuList::Release: the list's block (+68) 0 or pages of the allocation
// layer's (made for each run; the r3 compared, and whether the pages were
// freed).
void Releases(Context& h) {
  const Block list(0x80);
  if (!list.address) {
    ITC_LOG_ERROR("harness: alt_hud: no block from the allocation layer");
    return;
  }
  Results results("MenuList::Release (sub_8220BBB0)");
  for (uint64_t c = 0; c < 100; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(c * 131 + 7);
      WriteRandom(h.base, list.address, list.bytes, r);
      uint32_t pages = 0;
      if (r.Below(4) != 0) {
        pages = memory::AllocPages(0x1000 * (1 + r.Below(4)), 4096);
      }
      guest::StoreU32(h.base, list.address + 68, pages);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes, r);
      t.r3.u64 = list.address;
      ITC_FLAG(native_alt_hud) = v == 1;
      (v == 0 ? __imp__sub_8220BBB0 : hooks::AltHudMenuListRelease)(t, h.base);
      ITC_FLAG(native_alt_hud) = true;
      out[v].Word(pages != 0 ? 1 : 0);
      out[v].Word(pages != 0 && memory::IsPages(pages) ? 1 : 0);
      out[v].Bytes(Read(h.base, list.address, list.bytes));
      out[v].Bytes(Read(h.base, sp - 96, 80));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// AltHud::Destroy: the object (+0) and the menu list (+4) operator new's
// blocks, made for each run (the object now and then 0), the list's block
// (+68) 0 or the layer's pages. Compared: which block r3 is, the holder,
// whether the pages were freed, Destroy's and Release's frames (their
// register saves left out) and r1.
void Destroys(Context& h) {
  const Block holder(0x10);
  if (!holder.address) {
    ITC_LOG_ERROR("harness: alt_hud: no block from the allocation layer");
    return;
  }
  Results results("AltHud::Destroy (sub_82216BA8)");
  for (uint64_t c = 0; c < 100; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(c * 137 + 11);
      const uint32_t object = r.Below(5) == 0 ? 0 : memory::crt::OperatorNew(h.base, 264).value_or(0);
      const uint32_t list = memory::crt::OperatorNew(h.base, 72).value_or(0);
      if (list == 0) {
        ITC_LOG_ERROR("harness: alt_hud: operator new failed");
        return;
      }
      if (object != 0) {
        WriteRandom(h.base, object, 264, r);
      }
      WriteRandom(h.base, list, 72, r);
      uint32_t pages = 0;
      if (r.Below(4) != 0) {
        pages = memory::AllocPages(0x1000 * (1 + r.Below(4)), 4096);
      }
      guest::StoreU32(h.base, list + 68, pages);
      guest::StoreU32(h.base, holder.address, object);
      guest::StoreU32(h.base, holder.address + 4, list);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes, r);
      t.r3.u64 = holder.address;
      ITC_FLAG(native_alt_hud) = v == 1;
      (v == 0 ? __imp__sub_82216BA8 : hooks::AltHudDestroy)(t, h.base);
      ITC_FLAG(native_alt_hud) = true;
      out[v].Word(object != 0 ? 1 : 0);
      out[v].Word(pages != 0 ? 1 : 0);
      out[v].Word(pages != 0 && memory::IsPages(pages) ? 1 : 0);
      out[v].Bytes(Read(h.base, holder.address, 8));
      out[v].Bytes(Read(h.base, sp - 96, 80));
      out[v].Bytes(Read(h.base, sp - 192, 80));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32 == list ? 1 : t.r3.u32 == object ? 2 : t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void AltHud(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: alt_hud: no block from the allocation layer");
    return;
  }
  SetUps(h, w);
  Setters(h, w);
  Ticks(h, w);
  Releases(h);
  Destroys(h);
}

}  // namespace harness
