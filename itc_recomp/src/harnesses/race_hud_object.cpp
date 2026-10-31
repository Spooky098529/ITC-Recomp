// The race HUD object (src/race_hud_object/,
// docs/functions/race-hud-object.md) against the recompiled functions. Each
// case fills a world of our own with random words from the case's seed: a
// HUD block (the HUD, its curves and bars), a UI movie block (the movie
// headers and a pool of nodes whose child entries point back into the pool,
// for UiMovie::FindNode), the vehicles (pointed at by 0x826C2E34) and a
// collision block (a collision object and its contacts), plus the stack
// below the call's stack pointer and the caller's area above it; shapes the
// fields the function tests; runs the original (with
// --native_race_hud_object off) or the register steps
// (hooks/race_hud_object.h); and compares the blocks (the fields the
// functions load and store as floats with harness::Outputs' floats), the
// original's frame (its register saves left out), r3 and f1. The globals
// the cases point elsewhere are put back after each run.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/race_hud_object.h"
#include "memory/heap.h"
#include "race_hud_object/race_hud_object.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <functional>
#include <limits>
#include <set>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_hud_object);

namespace harness {

namespace {

namespace rh = race_hud_object;

constexpr uint32_t kVehicles = 0x826C2E34;  // Vehicle::Get's array
constexpr uint32_t kSaveSlots = 0x826C5B58;  // SaveData::Of's four pointers
constexpr uint32_t kSaveBytes = 0x20000;
constexpr uint32_t kMiscBytes = 0x2000;  // the viewport, the build
constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kVehicleCount = 4;
constexpr uint32_t kHudBytes = 0x1000;
// The movie block: nine movie headers (0x100 each) at +0, then the node pool.
constexpr uint32_t kMovieHeaders = 9;
constexpr uint32_t kNodesAt = 0x1000;
constexpr uint32_t kNodeBytes = 0x800;
constexpr uint32_t kNodeCount = 24;
constexpr uint32_t kMoviesBytes = kNodesAt + kNodeBytes * kNodeCount;
constexpr uint32_t kCollisionBytes = 0x1000;
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
      : hud(kHudBytes),
        movies(kMoviesBytes),
        vehicles(kVehicleBytes * kVehicleCount),
        collision(kCollisionBytes),
        save(kSaveBytes),
        misc(kMiscBytes) {}
  bool ok() const {
    return hud.address && movies.address && vehicles.address && collision.address &&
           save.address && misc.address;
  }
  uint32_t viewport() const { return misc.address + 0x10; }
  uint32_t build() const { return misc.address + 0x800; }
  uint32_t movie(uint32_t i) const { return movies.address + 0x100 * i; }
  uint32_t node(uint32_t i) const { return movies.address + kNodesAt + kNodeBytes * i; }
  uint32_t vehicle(uint32_t i) const { return vehicles.address + kVehicleBytes * i; }
  Block hud;
  Block movies;
  Block vehicles;
  Block collision;
  Block save;  // player 0's save data (the HUD options at +0x1F85C)
  Block misc;  // the viewports' pointer (+0), viewport 0 (+0x10), the build (+0x800)
};

// The movies: each header's live scene (+16) a pool node; each node's child
// index (+108) 0 to 3 and its child entries (+128 on) pool nodes. The HUD's
// nine movie pointers (+604 to +636) the headers; its vehicle (+440) one of
// the vehicles; the vehicles' collision objects (+340) 0 or the collision
// block's, whose contacts (+240, linked by +68) have flags (+4) with or
// without the mask's bits.
void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  for (uint32_t m = 0; m < kMovieHeaders; m++) {
    guest::StoreU32(base, w.movie(m) + 16, w.node(r.Below(kNodeCount)));
    guest::StoreU32(base, w.hud.address + 604 + 4 * m, w.movie(m));
  }
  for (uint32_t k = 0; k < kNodeCount; k++) {
    const uint32_t node = w.node(k);
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t i = 128; i < kNodeBytes; i += 8) {
      guest::StoreU32(base, node + i, w.node(r.Below(kNodeCount)));
    }
  }
  guest::StoreU32(base, kVehicles, w.vehicles.address);
  guest::StoreU32(base, w.hud.address + rh::kHudVehicle, w.vehicle(r.Below(kVehicleCount)));
  const uint32_t object = w.collision.address;
  for (uint32_t v = 0; v < kVehicleCount; v++) {
    guest::StoreU32(base, w.vehicle(v) + 340, r.Below(4) == 0 ? 0 : object);
  }
  uint32_t link = object + 240 - 68;
  const uint32_t contacts = r.Below(6);
  for (uint32_t k = 0; k < contacts; k++) {
    const uint32_t contact = object + 0x200 + 0x80 * k;
    guest::StoreU32(base, link + 68, contact);
    static constexpr uint32_t kFlags[] = {0, 0x01000000, 0x02000000, 0x04000000, 0x08000000,
                                          0x00000001};
    guest::StoreU32(base, contact + 4, r.Below(8) < 6 ? kFlags[r.Below(6)] : uint32_t(r.Next()));
    link = contact;
  }
  guest::StoreU32(base, link + 68, 0);
  for (uint32_t p = 0; p < 4; p++) {
    guest::StoreU32(base, kSaveSlots + 4 * p, w.save.address);
  }
  guest::StoreU32(base, rh::kViewports, w.misc.address);
  guest::StoreU32(base, w.misc.address, w.viewport());
  guest::StoreU32(base, w.misc.address + 4, w.viewport() + 0x100);
  for (uint32_t v = 0; v < kVehicleCount; v++) {
    guest::StoreU32(base, w.vehicle(v) + 5712, w.build());
  }
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// What a case compares besides the blocks: the original's frame
// [sp - frame, sp - saves) (its register saves left out), and the HUD
// block's float words at `floats` (offsets).
struct Spec {
  uint32_t frame = 0;
  uint32_t saves = 0;
  std::set<uint32_t> floats;
  bool f1 = false;
  void Floats(uint32_t offset, uint32_t words) {
    for (uint32_t i = 0; i < words; i++) {
      floats.insert(offset + 4 * i);
    }
  }
};

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const Spec& spec,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{kVehicles, 4}, {kSaveSlots, 16}, {rh::kViewports, 4}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.hud.address, w.hud.bytes, r);
    WriteRandom(h.base, w.movies.address, w.movies.bytes, r);
    WriteRandom(h.base, w.vehicles.address, w.vehicles.bytes, r);
    WriteRandom(h.base, w.collision.address, w.collision.bytes, r);
    WriteRandom(h.base, w.save.address + 0x1F800, 0x100, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    ShapeWorld(h.base, w, r);
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    ITC_FLAG(native_race_hud_object) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_hud_object) = true;
    uint32_t at = w.hud.address;
    for (const uint32_t offset : spec.floats) {
      const uint32_t address = w.hud.address + offset;
      if (address > at) {
        out[v].Bytes(Read(h.base, at, address - at));
      }
      out[v].Floats(h.base, address, 1);
      at = address + 4;
    }
    out[v].Bytes(Read(h.base, at, w.hud.address + w.hud.bytes - at));
    out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
    out[v].Bytes(Read(h.base, w.vehicles.address, w.vehicles.bytes));
    out[v].Bytes(Read(h.base, w.collision.address, w.collision.bytes));
    out[v].Bytes(Read(h.base, w.save.address + 0x1F800, 0x100));
    out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
    if (spec.frame > spec.saves) {
      out[v].Bytes(Read(h.base, sp - spec.frame, spec.frame - spec.saves));
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

// A curve at the HUD's +20: up to 12 points (the count 0 to 12 at +108,
// sometimes any word) with x ascending, +88 the largest x and +92 the
// smallest (or random bounds).
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

void Helpers(Context& h, const World& w) {
  {
    Spec spec;
    spec.Floats(20, 28);
    Group(
        h, w, "HudCurve::Eval (sub_82212118)", 600,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t curve = w.hud.address + 20;
          ShapeCurve(h.base, curve, r);
          t.r3.u64 = curve;
          // x inside the curve's points mostly, else anything.
          const uint32_t count = guest::LoadU32(h.base, curve + 108);
          if (r.Below(3) != 0 && count > 0 && count <= 11) {
            const float a = guest::LoadFloat(h.base, curve);
            const float b = guest::LoadFloat(h.base, curve + 8 * (count - 1));
            t.f1.f64 = double(a) + (double(b) - double(a)) * double(r.Below(1001)) / 1000.0;
          } else {
            t.f1.f64 = AnyDouble(r, 300.0f);
          }
        },
        __imp__sub_82212118, hooks::RaceHudObjectCurveEval, spec);
  }
  {
    Spec spec;
    spec.f1 = true;
    Group(
        h, w, "PowInt (sub_82213C40)", 400,
        [&](PPCContext& t, Random& r, const World&) {
          t.f1.f64 = r.Below(2) == 0 ? double(r.Below(21)) / 2.0 - 5.0 : AnyDouble(r, 20.0f);
          switch (r.Below(4)) {
            case 0:
              t.r4.u64 = uint64_t(int64_t(int32_t(r.Below(9)) - 4));
              break;
            case 1:
              t.r4.u64 = r.Next();
              break;
            case 2:
              t.r4.u64 = r.Below(2) == 0 ? 0x80000000u : 0x7FFFFFFFu;
              break;
            default:
              t.r4.u64 = r.Below(12);
              break;
          }
        },
        __imp__sub_82213C40, hooks::RaceHudObjectPowInt, spec);
  }
  {
    // A bar at the HUD's +244 (the racers' first): value, steps, trail,
    // min and max mostly ordinary, the target an int.
    Spec spec;
    spec.frame = 16;
    spec.Floats(244, 7);
    Group(
        h, w, "HudBar::Step (sub_82211FB8)", 800,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t bar = w.hud.address + 244;
          const bool ordinary = r.Below(4) != 0;
          auto value = [&](float range) {
            return ordinary ? float(r.Below(int(range) + 1)) : AnyFloat(r, range);
          };
          guest::StoreFloat(h.base, bar + 0, value(200.0f));
          guest::StoreFloat(h.base, bar + 4, value(20.0f));
          guest::StoreFloat(h.base, bar + 8, value(200.0f));
          guest::StoreFloat(h.base, bar + 12, value(20.0f));
          guest::StoreFloat(h.base, bar + 16, ordinary ? 0.0f : AnyFloat(r, 50.0f));
          guest::StoreFloat(h.base, bar + 20, ordinary ? 100.0f + float(r.Below(100))
                                                           : AnyFloat(r, 300.0f));
          if (r.Below(4) == 0) {
            // value and trail equal (+36 cleared).
            guest::StoreU32(h.base, bar + 8, guest::LoadU32(h.base, bar + 0));
          }
          t.r3.u64 = bar;
          t.r4.u64 = r.Below(4) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(260)) - 30));
        },
        __imp__sub_82211FB8, hooks::RaceHudObjectBarStep, spec);
  }
}

void Setters(Context& h, const World& w) {
  {
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::SetGear (sub_82212DB0)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          t.r3.u64 = w.hud.address;
          t.r4.u64 = r.Below(4) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(12)) - 3));
        },
        __imp__sub_82212DB0, hooks::RaceHudObjectSetGear, spec);
  }
  {
    Spec spec;
    spec.frame = 112;
    spec.saves = 24;
    spec.Floats(404, 3);
    Group(
        h, w, "RaceHud::SetBar (sub_82214698)", 400,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t hud = w.hud.address;
          const bool ordinary = r.Below(4) != 0;
          guest::StoreFloat(h.base, hud + 408,
                            ordinary ? 50.0f + float(r.Below(100)) : AnyFloat(r, 100.0f));
          guest::StoreFloat(h.base, hud + 412,
                            ordinary ? float(r.Below(3)) : AnyFloat(r, 100.0f));
          t.r3.u64 = hud;
          t.f1.f64 = ordinary ? double(r.Below(150)) - 10.0 : AnyDouble(r, 200.0f);
        },
        __imp__sub_82214698, hooks::RaceHudObjectSetBar, spec);
  }
  {
    Spec spec;
    spec.Floats(404, 3);
    Group(
        h, w, "RaceHud::ResetBar (sub_82212668)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t car = guest::LoadU32(h.base, w.hud.address + rh::kHudVehicle);
          guest::StoreFloat(h.base, car + 2420, AnyFloat(r, 300.0f));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82212668, hooks::RaceHudObjectResetBar, spec);
  }
  Group(
      h, w, "RaceHud::ClearWarning (sub_822122A0)", 20,
      [&](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.hud.address; },
      __imp__sub_822122A0, hooks::RaceHudObjectClearWarning, Spec{});
  {
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::SetPlayer (sub_82212690)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          t.r3.u64 = w.hud.address;
          t.r4.u64 = r.Below(kVehicleCount);
          if (r.Below(8) == 0) {
            guest::StoreU32(h.base, kVehicles, 0);  // no vehicles: 0
          }
        },
        __imp__sub_82212690, hooks::RaceHudObjectSetPlayer, spec);
  }
  {
    Spec spec;
    spec.frame = 96;
    spec.saves = 8;
    Group(
        h, w, "RaceHud::Touching (sub_82215820)", 200,
        [&](PPCContext& t, Random& r, const World&) {
          t.r3.u64 = r.Next();
          t.r4.u64 = r.Below(kVehicleCount);
        },
        __imp__sub_82215820, hooks::RaceHudObjectTouching, spec);
  }
}

// A node's children's range: its first child index (+100) and count (+104).
void ShapeGroup(uint8_t* base, uint32_t node, Random& r) {
  guest::StoreU32(base, node + 100, r.Below(100));
  guest::StoreU32(base, node + 104, r.Below(8) == 0 ? uint32_t(-int32_t(r.Below(3))) : r.Below(5));
}

uint32_t DashboardNode(uint8_t* base, const World& w, uint32_t sub_scene, uint32_t child) {
  return ui_movies::FindNode(base, guest::LoadU32(base, w.hud.address + rh::kHudDashboard),
                             sub_scene, child);
}

void Dashboard(Context& h, const World& w) {
  for (int boost = 0; boost < 2; boost++) {
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    spec.Floats(20, 56);
    Group(
        h, w, boost ? "RaceHud::SetBoost (sub_82212E18)" : "RaceHud::SetTach (sub_82212D58)",
        300,
        [&, boost](PPCContext& t, Random& r, const World& w) {
          const uint32_t curve = w.hud.address + (boost ? 132 : 20);
          ShapeCurve(h.base, curve, r);
          t.r3.u64 = w.hud.address;
          t.f1.f64 = AnyDouble(r, 120.0f);
        },
        boost ? __imp__sub_82212E18 : __imp__sub_82212D58,
        boost ? hooks::RaceHudObjectSetBoost : hooks::RaceHudObjectSetTach, spec);
  }
  {
    Spec spec;
    spec.frame = 160;
    spec.saves = 72;
    spec.f1 = true;
    Group(
        h, w, "RaceHud::SetSpeed (sub_82214260)", 400,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeGroup(h.base, DashboardNode(h.base, w, 0xFFFFFFFFu, 9), r);
          t.r3.u64 = w.hud.address;
          t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(1100);
          t.f1.f64 = AnyDouble(r, 10.0f);
        },
        __imp__sub_82214260, hooks::RaceHudObjectSetSpeed, spec);
  }
  {
    Spec spec;
    spec.frame = 144;
    spec.saves = 56;
    Group(
        h, w, "RaceHud::SetTime (sub_82213750)", 300,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t time = w.hud.address + 0x900;
          uint32_t packed;
          switch (r.Below(3)) {
            case 0:
              packed = uint32_t(r.Next());
              break;
            case 1:
              packed = (r.Below(70) << 16) | (r.Below(64) << 10) | r.Below(1024);
              break;
            default:
              packed = (r.Below(60) << 16) | (r.Below(60) << 10) | r.Below(1000);
              break;
          }
          guest::StoreU32(h.base, time, packed);
          t.r3.u64 = w.hud.address;
          t.r4.u64 = time;
        },
        __imp__sub_82213750, hooks::RaceHudObjectSetTime, spec);
  }
  {
    Spec spec;
    spec.frame = 160;
    spec.saves = 72;
    spec.f1 = true;
    Group(
        h, w, "RaceHud::SetDistance (sub_82214A50)", 400,
        [&](PPCContext& t, Random& r, const World& w) {
          t.r3.u64 = w.hud.address;
          t.f1.f64 = r.Below(2) == 0 ? double(r.Below(20000)) / 100.0 - 5.0
                                     : AnyDouble(r, 2000.0f);
        },
        __imp__sub_82214A50, hooks::RaceHudObjectSetDistance, spec);
  }
  {
    Spec spec;
    spec.frame = 128;
    spec.saves = 48;
    Group(
        h, w, "RaceHud::Reset (sub_822121C0)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeGroup(h.base, DashboardNode(h.base, w, 0xFFFFFFFFu, 1), r);
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_822121C0, hooks::RaceHudObjectReset, spec);
  }
  {
    Spec spec;
    spec.frame = 112;
    spec.saves = 32;
    Group(
        h, w, "RaceHud::ShowGauge (sub_82212D08)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          t.r3.u64 = w.hud.address;
          t.r4.u64 = r.Below(3);
          t.r5.u64 = r.Below(3) == 0 ? r.Next() : r.Below(2);
        },
        __imp__sub_82212D08, hooks::RaceHudObjectShowGauge, spec);
  }
  {
    // ShowGauge's frames below its own are compared too (their back chains
    // and register saves aside: 112 bytes each, the saves 32).
    Spec spec;
    spec.frame = 144;
    spec.saves = 32;
    Group(
        h, w, "RaceHud::SetUpExtraGauges (sub_82213DB0)", 400,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t build = w.build();
          for (uint32_t k = 0; k < 5; k++) {
            guest::StoreU32(h.base, build + 1832 + 12 * k,
                            r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(6));
          }
          guest::StoreU32(h.base, build + 12, r.Below(3) == 0 ? 10 : r.Below(12));
          guest::StoreU32(h.base, w.hud.address + 8, r.Below(3) == 0 ? uint32_t(r.Next())
                                                                     : r.Below(2));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82213DB0, hooks::RaceHudObjectSetUpExtraGauges, spec);
  }
  {
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::ApplyOptions (sub_82212768)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          guest::StoreU32(h.base, w.hud.address + 472, r.Below(2));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82212768, hooks::RaceHudObjectApplyOptions, spec);
  }
  {
    Spec spec;
    spec.frame = 112;
    spec.saves = 24;
    Group(
        h, w, "RaceHud::UpdateView (sub_82212B68)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          guest::StoreU32(h.base, w.viewport() + 1860,
                          r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(4));
          guest::StoreU32(h.base, w.hud.address + 12, r.Below(2));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82212B68, hooks::RaceHudObjectUpdateView, spec);
  }
}

}  // namespace

void RaceHudObject(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_hud_object: no block from the allocation layer");
    return;
  }
  Helpers(h, w);
  Setters(h, w);
  Dashboard(h, w);
}

}  // namespace harness
