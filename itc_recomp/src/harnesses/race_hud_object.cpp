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
#include "data_tables/data_tables.h"
#include "hooks/app_start_up.h"
#include "hooks/race_hud_object.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
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

ITC_FLAG_DECLARE(bool, native_race_hud_object);

namespace harness {

namespace {

namespace rh = race_hud_object;

constexpr uint32_t kVehicles = 0x826C2E34;  // Vehicle::Get's array
constexpr uint32_t kSaveSlots = 0x826C5B58;  // SaveData::Of's four pointers
constexpr uint32_t kSaveBytes = 0x20000;
constexpr uint32_t kMiscBytes = 0x6000;  // the viewport, the build, the textures, the racers
constexpr uint32_t kRaceBytes = 0xA4000;  // the race data (to the mode, +0xA3AF4)
constexpr uint32_t kOwner = 0x828161EC;   // the track zones' owner
constexpr uint32_t kMirrorTarget = 0x826BABE0;  // render target 11's texture
constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kVehicleCount = 4;
constexpr uint32_t kHudBytes = 0x1000;
// The movie block: nine movies (0xA000 each, their animation lists and
// named animations included) at +0, then the node pool, then 8 animations
// of no tracks (64 bytes each) for the named ones.
constexpr uint32_t kMovieHeaders = 9;
constexpr uint32_t kMovieBytes = 0xA000;
constexpr uint32_t kNodesAt = kMovieHeaders * kMovieBytes;
constexpr uint32_t kNodeBytes = 0x800;
constexpr uint32_t kNodeCount = 24;
constexpr uint32_t kAnimationsAt = kNodesAt + kNodeBytes * kNodeCount;
constexpr uint32_t kMoviesBytes = kAnimationsAt + 8 * 64;
// The track zones' owner (the list's nodes from +0, its head +516, count
// +520, the zones' pointers from +524) at +0 and 70 small zones (16 bytes
// apart: +0 the vtable, +17112 the inside flag) from +0x400.
constexpr uint32_t kZonesAt = 0x400;
constexpr uint32_t kZonesBytes = 0x5000;
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
        misc(kMiscBytes),
        race(kRaceBytes),
        zones(kZonesBytes) {}
  bool ok() const {
    return hud.address && movies.address && vehicles.address && collision.address &&
           save.address && misc.address && race.address &&
           zones.address;
  }
  uint32_t textures() const { return misc.address + 0x1000; }
  uint32_t racers() const { return misc.address + 0x2000; }
  uint32_t progress() const { return misc.address + 0x3000; }
  uint32_t viewport() const { return misc.address + 0x10; }
  uint32_t build() const { return misc.address + 0x800; }
  uint32_t movie(uint32_t i) const { return movies.address + kMovieBytes * i; }
  uint32_t zone(uint32_t i) const { return zones.address + kZonesAt + 16 * i; }
  uint32_t node(uint32_t i) const { return movies.address + kNodesAt + kNodeBytes * i; }
  uint32_t vehicle(uint32_t i) const { return vehicles.address + kVehicleBytes * i; }
  Block hud;
  Block movies;
  Block vehicles;
  Block collision;
  Block save;  // player 0's save data (the HUD options at +0x1F85C)
  Block misc;  // the viewports' pointer (+0), viewport 0 (+0x10), the build (+0x800),
               // the movies' textures (+0x1000), the racers (+0x2000), the race's
               // progress (+0x3000), the fake sound manager (+0x5600, its vtable
               // +0x5800)
  Block race;  // the race data
  Block zones;  // the track zones' owner and its zones
};

// The guest calls the recorder takes (TrackZones::Create, UiMovie::SetText
// and ClearText), answered with a word from the case's generator.
struct Call {
  uint32_t function, lr, r1;
  uint64_t r3, r4, r5, r6;
  // MenuList::Init's r7 to r10 and its stack words (r1 + 84, r1 + 92).
  uint64_t more[6];
};
std::vector<Call>* g_calls = nullptr;
uint8_t* g_base = nullptr;
Random g_answers;
// operator new's answers by the size asked for (264 and 72: RaceScreens::
// Make1004's), blocks of the case's world (0 for a failure).
uint32_t g_new_264 = 0, g_new_72 = 0;

bool Recorder(PPCContext& ctx, uint32_t function) {
  if (g_calls == nullptr) {
    return false;
  }
  // The arguments each reads (TrackZones::Create and the UI movies' start
  // none, ClearText and SetNodeText r3 to r5, SetText r3 to r6, Strings::Load
  // and UiMovies::Open r3 and r4, operator new r3, MenuList::Init r3 to r10
  // and its two stack words; the rest are leftovers, and SetNodeText's r6 to
  // r10 its unset varargs).
  uint32_t reads = 0;
  switch (function) {
    case 0x8211C1F8:
    case 0x8220BAC0:
      reads = 4;
      break;
    case 0x8211C470:
    case 0x8211C7E8:
      reads = 3;
      break;
    case 0x8210A8D0:
    case 0x8211E808:
      reads = 2;
      break;
    case 0x825A6880:
      reads = 1;
      break;
  }
  const uint64_t r[4] = {ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64};
  Call call{function, uint32_t(ctx.lr), ctx.r1.u32, 0, 0, 0, 0, {}};
  uint64_t* to[4] = {&call.r3, &call.r4, &call.r5, &call.r6};
  for (uint32_t i = 0; i < reads; i++) {
    *to[i] = r[i];
  }
  if (function == 0x8220BAC0) {
    call.more[0] = ctx.r7.u64;
    call.more[1] = ctx.r8.u64;
    call.more[2] = ctx.r9.u64;
    call.more[3] = ctx.r10.u64;
    call.more[4] = guest::LoadU32(g_base, ctx.r1.u32 + 84);
    call.more[5] = guest::LoadU32(g_base, ctx.r1.u32 + 92);
  }
  g_calls->push_back(call);
  ctx.r3.u64 = g_answers.Next();
  if (function == 0x825A6880) {
    ctx.r3.u64 = call.r3 == 264 ? g_new_264 : call.r3 == 72 ? g_new_72 : 0;
  }
  return true;
}

// The UI movies' update (which UiMovie::Run jumps to: r3 the movie, r4 draw
// only) and the full-frame effect's recorder call (sub_8259EBE8: r3 the
// quad, r4 its key, f1 its depth, kept in r5's place), recorded as the
// recorder's calls and answered the same way, through the UI movies' stand-in
// (ui_movies_hooks::g_update_callee_stand_in).
bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_calls == nullptr || (function != 0x8211D2F8 && function != 0x8259EBE8)) {
    return false;
  }
  g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64,
                      function == 0x8259EBE8 ? ctx.f1.u64 : 0, 0, {}});
  ctx.r3.u64 = g_answers.Next();
  return true;
}

// The movies: each header's live scene (+16) a pool node and its textures
// (+28) the misc block's; each node's children's range (+100, +104: up to
// 4 from below 100), child index (+108) 0 to 3 and its child entries (+128
// on) pool nodes. The HUD's
// nine movie pointers (+604 to +636) the headers; its vehicle (+440) one of
// the vehicles; the vehicles' collision objects (+340) 0 or the collision
// block's, whose contacts (+240, linked by +68) have flags (+4) with or
// without the mask's bits.
void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  for (uint32_t m = 0; m < kMovieHeaders; m++) {
    const uint32_t movie = w.movie(m);
    ui_movies::InitAnimationLists(base, movie);
    guest::StoreU32(base, movie + 16, w.node(r.Below(kNodeCount)));
    guest::StoreU32(base, movie + 20, guest::LoadU32(base, movie + 16));
    guest::StoreU32(base, movie + 24, guest::LoadU32(base, movie + 16));
    guest::StoreU32(base, movie + 28, w.textures());
    for (uint32_t k = 0; k < 16; k++) {
      const uint32_t named = movie + 176 + 36 * k;
      guest::StoreU32(base, named + 28, guest::LoadU32(base, movie + 16));
      guest::StoreU32(base, named + 32, w.movies.address + kAnimationsAt + 64 * r.Below(8));
    }
    guest::StoreU32(base, w.hud.address + 604 + 4 * m, w.movie(m));
  }
  for (uint32_t k = 0; k < kNodeCount; k++) {
    const uint32_t node = w.node(k);
    guest::StoreU32(base, node + 100, r.Below(100));  // the children's first index
    guest::StoreU32(base, node + 104, r.Below(5));    // and count
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
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t animation = w.movies.address + kAnimationsAt + 64 * k;
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, animation + i, 0);
    }
    guest::StoreFloat(base, animation + 12, float(r.Below(100)) / 4.0f);
  }
  // The track zones: the owner's list of 128 in order (node i: next i + 1,
  // data i), no zones to update (+520 = 0), zone i's vtable the game's and
  // its inside flag set now and then.
  const uint32_t owner = w.zones.address;
  for (uint32_t i = 0; i < 128; i++) {
    guest::StoreU16(base, owner + 4 * i, uint16_t(i + 1));
    guest::StoreU16(base, owner + 4 * i + 2, uint16_t(i));
  }
  guest::StoreU16(base, owner + 516, 0);
  guest::StoreU16(base, owner + 520, 0);
  for (uint32_t k = 0; k < 70; k++) {
    guest::StoreU32(base, owner + 524 + 4 * k, w.zone(k));
    guest::StoreU32(base, w.zone(k), 0x82011AF0 + 12 * k);
    guest::StoreU32(base, w.zone(k) + 17112, r.Below(12) == 0 ? 1 : 0);
  }
  guest::StoreU32(base, kOwner, owner);
  guest::StoreU32(base, rh::kViewports, w.misc.address);
  guest::StoreU32(base, w.misc.address, w.viewport());
  guest::StoreU32(base, w.misc.address + 4, w.viewport() + 0x100);
  for (uint32_t v = 0; v < kVehicleCount; v++) {
    guest::StoreU32(base, w.vehicle(v) + 5712, w.build());
  }
  // The race data: the course (+4) 0 to 24, the flags (+20), the mode
  // (+0xA3AF4) 0 to 9, the current stage (+0x59620 + 804) 0 to 9 and the
  // stages' types (80 bytes each) 0 to 2.
  const uint32_t race = w.race.address;
  guest::StoreU32(base, rh::kRaceData, r.Below(16) == 0 ? 0 : race);
  guest::StoreU32(base, race + 4, r.Below(25));
  guest::StoreU32(base, race + 20, r.Below(64));
  guest::StoreU32(base, race + rh::kRaceMode, r.Below(10));
  guest::StoreU32(base, race + 0x59620 + 804, r.Below(10));
  for (uint32_t k = 0; k < 10; k++) {
    guest::StoreU32(base, race + 0x59620 + 80 * k, r.Below(3));
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
    SavedBytes saved(h.base, {{kVehicles, 4},
                              {kSaveSlots, 16},
                              {rh::kViewports, 4},
                              {rh::kRaceData, 4},
                              {kOwner, 4},
                              {rh::kRaceProgress, 4},
                              {ui_movies::kMovieTable, 16},
                              {data_tables::kTables, 4},
                              {menu_sounds::kSoundManager, 4}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.hud.address, w.hud.bytes, r);
    WriteRandom(h.base, w.movies.address, w.movies.bytes, r);
    WriteRandom(h.base, w.vehicles.address, w.vehicles.bytes, r);
    WriteRandom(h.base, w.collision.address, w.collision.bytes, r);
    WriteRandom(h.base, w.save.address + 0x1F800, 0x100, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    WriteRandom(h.base, w.race.address, 4 * rh::kRecordBytes, r);
    WriteRandom(h.base, w.race.address + 0x59600, 0x400, r);
    WriteRandom(h.base, w.zones.address, w.zones.bytes, r);
    ShapeWorld(h.base, w, r);
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    g_answers = Random(r.Next());
    std::vector<Call> calls;
    g_calls = &calls;
    g_base = h.base;
    const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
    hooks::g_start_up_recorder = Recorder;
    const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
        ui_movies_hooks::g_update_callee_stand_in;
    ui_movies_hooks::g_update_callee_stand_in = StandIn;
    ITC_FLAG(native_race_hud_object) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_hud_object) = true;
    ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
    hooks::g_start_up_recorder = saved_recorder;
    g_calls = nullptr;
    out[v].Word(uint32_t(calls.size()));
    for (const Call& call : calls) {
      out[v].Word(call.function);
      out[v].Word(call.lr);
      out[v].Word(call.r1);
      for (const uint64_t r : {call.r3, call.r4, call.r5, call.r6, call.more[0], call.more[1],
                               call.more[2], call.more[3], call.more[4], call.more[5]}) {
        out[v].Word(uint32_t(r >> 32));
        out[v].Word(uint32_t(r));
      }
    }
    out[v].Word(guest::LoadU32(h.base, kOwner));
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
    out[v].Bytes(Read(h.base, w.race.address, 4 * rh::kRecordBytes));
    out[v].Bytes(Read(h.base, w.zones.address, w.zones.bytes));
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

void SetUps(Context& h, const World& w) {
  {
    // The player 0 to 3; its record's +60 an index whose table entry
    // (0x8200D3B8 + 16 x index) picks one of the three curves, its +1704 any.
    Spec spec;
    spec.frame = 480;
    spec.saves = 56;
    spec.Floats(20, 56);
    Group(
        h, w, "RaceHud::SetUpGauges (sub_82213C90)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t player = r.Below(4);
          guest::StoreU32(h.base, w.hud.address + rh::kHudPlayer, player);
          guest::StoreU32(h.base, rh::kRaceData, w.race.address);
          const uint32_t record = w.race.address + rh::kRecordBytes * player;
          uint32_t index = r.Below(8);
          if (guest::LoadU32(h.base, 0x8200D3B8 + 16 * index) >= 3) {
            index = 0;
          }
          guest::StoreU32(h.base, record + 60, index);
          guest::StoreU32(h.base, record + 1704,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(10));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82213C90, hooks::RaceHudObjectSetUpGauges, spec);
  }
  {
    // Render target 11 the game's own (or none at the harness's time).
    Spec spec;
    spec.frame = 144;
    spec.saves = 24;
    ITC_LOG_INFO("harness: race_hud_object: render target 11's texture {:08X}",
                 guest::LoadU32(h.base, kMirrorTarget));
    Group(
        h, w, "RaceHud::SetUpMirror (sub_822135B0)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          guest::StoreU32(h.base, w.hud.address + 588, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_822135B0, hooks::RaceHudObjectSetUpMirror, spec);
  }
  {
    // No track zones' owner (nothing to delete).
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::TearDown (sub_822141A0)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          guest::StoreU32(h.base, kOwner, 0);
          guest::StoreU32(h.base, w.hud.address + 588, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_822141A0, hooks::RaceHudObjectTearDown, spec);
  }
  {
    // TrackZones::Create recorded (its r1 and lr compared).
    Spec spec;
    spec.frame = 320;
    spec.saves = 144;
    Group(
        h, w, "RaceHud::SetUpSigns (sub_822122B0)", 300,
        [&](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.hud.address; },
        __imp__sub_822122B0, hooks::RaceHudObjectSetUpSigns, spec);
  }
  {
    // The slots 0 to 2 or other words; the digits' originals run.
    Spec spec;
    spec.frame = 144;
    spec.saves = 56;
    Group(
        h, w, "RaceHud::UpdateExtraGauges (sub_82215160)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          for (uint32_t k = 0; k < 3; k++) {
            guest::StoreU32(h.base, w.hud.address + 480 + 4 * k,
                            r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(4) - 1);
          }
          t.r3.u64 = w.hud.address;
          t.f1.f64 = AnyDouble(r, 300.0f);
          t.r5.u64 = r.Below(3) == 0 ? r.Next() : r.Below(200);
          t.r6.u64 = r.Below(3) == 0 ? r.Next() : r.Below(200);
        },
        __imp__sub_82215160, hooks::RaceHudObjectUpdateExtraGauges, spec);
  }
}

// The signs' fields: the zone (+416) none, 0 to 31 or any; the place's
// zone (+428) none, 32 to 69 or any; signs on (+424); the course's flags
// (+420) a table of 0s and 1s in the misc block (+0x1200, with the entry
// before it); the car's +6168; the race mode 6 now and then; the course
// (+4) 8 to 10 now and then.
void ShapeSigns(uint8_t* base, const World& w, Random& r) {
  const uint32_t hud = w.hud.address;
  const uint32_t pick = r.Below(8);
  guest::StoreU32(base, hud + 416, pick < 3 ? 0xFFFFFFFFu : pick < 7 ? r.Below(32) : r.Below(70));
  const uint32_t place = r.Below(8);
  guest::StoreU32(base, hud + 428,
                  place < 3 ? 0xFFFFFFFFu : place < 7 ? 32 + r.Below(38) : r.Below(70));
  guest::StoreU32(base, hud + 424, r.Below(4) == 0 ? 0 : 1);
  const uint32_t flags = w.misc.address + 0x1200;
  for (uint32_t i = 0; i < 27 * 3; i++) {
    guest::StoreU32(base, flags - 12 + 4 * i, r.Below(2));
  }
  guest::StoreU32(base, hud + 420, flags);
  const uint32_t car = guest::LoadU32(base, hud + rh::kHudVehicle);
  guest::StoreU32(base, car + 6168, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, rh::kRaceData, w.race.address);
  guest::StoreU32(base, w.race.address + rh::kRaceMode, r.Below(3) == 0 ? 6 : r.Below(10));
  guest::StoreU32(base, w.race.address + 4, r.Below(4) == 0 ? 8 + r.Below(3) : r.Below(25));
}

void Ticks(Context& h, const World& w) {
  {
    // The car's values (a speed in m/s, the extra gauges' floats, the gear,
    // the tachometer's +2224, the bar, the boost, the build's +3892), the
    // curves, the digits' groups, the extra gauges' slots and the signs.
    Spec spec;
    spec.frame = 128;
    spec.saves = 32;
    spec.Floats(20, 56);
    spec.Floats(404, 3);
    Group(
        h, w, "RaceHud::Tick (sub_822153A0)", 400,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeSigns(h.base, w, r);
          const uint32_t hud = w.hud.address;
          const uint32_t car = guest::LoadU32(h.base, hud + rh::kHudVehicle);
          const bool ordinary = r.Below(4) != 0;
          guest::StoreFloat(h.base, car + 48, ordinary ? float(r.Below(90)) : AnyFloat(r, 400.0f));
          for (const uint32_t offset : {6252u, 6256u, 6260u, 2224u, 2420u, 2944u}) {
            guest::StoreFloat(h.base, car + offset,
                              ordinary ? float(r.Below(200)) / 2.0f : AnyFloat(r, 300.0f));
          }
          guest::StoreU32(h.base, car + 2252, r.Below(9) - 1);
          ShapeCurve(h.base, hud + 20, r);
          ShapeCurve(h.base, hud + 132, r);
          ShapeGroup(h.base, DashboardNode(h.base, w, 0xFFFFFFFFu, 9), r);
          for (uint32_t k = 0; k < 3; k++) {
            guest::StoreU32(h.base, hud + 480 + 4 * k, r.Below(4) - 1);
          }
          t.r3.u64 = hud;
          t.r4.u64 = r.Below(3) == 0 ? 1 : 0;
          t.r7.u64 = t.r8.u64 = t.r9.u64 = t.r10.u64 = 0;
        },
        __imp__sub_822153A0, hooks::RaceHudObjectTick, spec);
  }
  {
    // The racers (+444): the ticks (+3932), the start's float (+828), the
    // animation (+12) -1 to 5; the race's progress (+7156); +464 the last
    // animation now and then the same.
    Spec spec;
    spec.frame = 128;
    spec.saves = 32;
    Group(
        h, w, "RaceHud::TickTimer (sub_82214D78)", 400,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t hud = w.hud.address;
          const uint32_t racers = w.racers();
          guest::StoreU32(h.base, hud + 444, racers);
          guest::StoreU32(h.base, rh::kRaceProgress, w.progress());
          guest::StoreU32(h.base, racers + 3932,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(200000));
          // Finite values only: a NaN distance (or infinity less infinity)
          // makes SetDistance write negative digits
          // into the nodes' child indices, and the lookups after it read
          // outside the pool (the game's own behavior; the harness's world
          // can't hold it).
          const bool ordinary = r.Below(4) != 0;
          auto any = [&](float range) {
            const float value = AnyFloat(r, range);
            return std::isfinite(value) ? value : 0.0f;
          };
          guest::StoreFloat(h.base, racers + 828, ordinary ? float(r.Below(10000)) : any(1e5f));
          guest::StoreFloat(h.base, w.progress() + 7156,
                            ordinary ? float(r.Below(20000)) : any(1e5f));
          const uint32_t animation = r.Below(7) - 1;
          guest::StoreU32(h.base, racers + 12, animation);
          guest::StoreU32(h.base, hud + 464, r.Below(3) == 0 ? animation : r.Below(7) - 1);
          t.r3.u64 = hud;
        },
        __imp__sub_82214D78, hooks::RaceHudObjectTickTimer, spec);
  }
}

void Signs(Context& h, const World& w) {
  // UiMovie::SetText and ClearText recorded (r3 to r6; r7 to r10 0 from
  // the native callers, the user's call, 2026-10-30: the cases start with
  // them 0, which the originals leave there).
  auto setup = [&](PPCContext& t, Random& r, const World& w) {
    ShapeSigns(h.base, w, r);
    t.r3.u64 = w.hud.address;
    t.r4.u64 = r.Below(3) == 0 ? 1 : 0;
    t.r7.u64 = t.r8.u64 = t.r9.u64 = t.r10.u64 = 0;
  };
  {
    Spec spec;
    spec.frame = 224;
    spec.saves = 144;
    Group(h, w, "RaceHud::UpdateSign (sub_82212F00)", 800, setup, __imp__sub_82212F00,
          hooks::RaceHudObjectUpdateSign, spec);
  }
  {
    Spec spec;
    spec.frame = 128;
    spec.saves = 40;
    Group(h, w, "RaceHud::UpdatePlaceName (sub_82213510)", 400, setup, __imp__sub_82213510,
          hooks::RaceHudObjectUpdatePlaceName, spec);
  }
  {
    Spec spec;
    spec.frame = 112;
    spec.saves = 24;
    Group(h, w, "RaceHud::UpdateSigns (sub_82214718)", 400, setup, __imp__sub_82214718,
          hooks::RaceHudObjectUpdateSigns, spec);
  }
}


// A movie's animations for UiMovie::Play, PlayNamed, Loop, LoopNamed,
// Finish and FinishNamed: its live scene's animation index (+116) small (the
// scene's entries are pool nodes), and now and then a slot in the queue or
// the looping list naming animation 0 or another, of the live scene (or a
// sub-scene's -1 or 0), by name 0 or not (its scene the live one, its
// animation one of the block's).
void ShapeAnimations(uint8_t* base, const World& w, uint32_t movie, Random& r) {
  const uint32_t scene = guest::LoadU32(base, movie + ui_movies::kMovieLive);
  guest::StoreU32(base, scene + 116, r.Below(4));
  const uint32_t slots = r.Below(3);
  for (uint32_t k = 0; k < slots; k++) {
    ui_movies::Slot slot{};
    slot[0] = r.Below(3) == 0 ? 1 : 0;
    slot[1] = r.Below(3) == 0 ? 0 : 0xFFFFFFFFu;
    slot[2] = r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(2);
    slot[3] = scene;
    slot[4] = w.movies.address + kAnimationsAt + 64 * r.Below(8);
    slot[5] = Bits(float(r.Below(40)) / 4.0f);
    slot[6] = r.Below(2);
    slot[7] = r.Below(2);
    ui_movies::Append(base, movie + (r.Below(2) == 0 ? ui_movies::kMovieQueue
                                                     : ui_movies::kMovieLooping),
                      slot);
  }
}

// The movie table (UiMovies::Find's pairs) in the misc block (+0x4000):
// up to 24 pairs of the ids 0 to 23 in a random order (some missing), each
// value a word of the block.
void ShapeMovieTable(uint8_t* base, const World& w, Random& r) {
  const uint32_t pairs = w.misc.address + 0x4000;
  uint32_t ids[24];
  for (uint32_t i = 0; i < 24; i++) {
    ids[i] = i;
  }
  for (uint32_t i = 23; i > 0; i--) {
    std::swap(ids[i], ids[r.Below(i + 1)]);
  }
  const uint32_t count = r.Below(4) == 0 ? r.Below(25) : 18 + r.Below(7);
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, pairs + 8 * i, ids[i]);
  }
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 24);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
}

// The racers' movie's (movie 0) live scene as a chunk list for
// RaceHud::ShowItem: chunk 0 at the scene's +0 (0x180 bytes, holding the
// child entries FindNode reads), then chunks of 0x60 to 0xC0 bytes tagged
// PRM, PIC, TXT or else, to a TRM before the node's end (or TRM at once).
void ShapeChunks(uint8_t* base, const World& w, Random& r) {
  static constexpr uint32_t kTags[] = {0x50524D00, 0x50494300, 0x54585400, 0x53434500};
  const uint32_t scene = guest::LoadU32(base, w.movie(0) + ui_movies::kMovieLive);
  if (r.Below(10) == 0) {
    guest::StoreU32(base, scene, 0x54524D00);
    return;
  }
  uint32_t at = 0;
  uint32_t size = 0x180;
  while (at + size < 0x700) {
    guest::StoreU32(base, scene + at, r.Below(5) == 0 ? uint32_t(r.Next()) : kTags[r.Below(4)]);
    guest::StoreU32(base, scene + at + 4, size);
    at += size;
    size = 0x60 + 0x20 * r.Below(4);
  }
  guest::StoreU32(base, scene + at, 0x54524D00);
}

// The data tables' object ([0x826C5CA0], misc block +0x5000) for
// DataTables::Item: its items (+16) placed so item `id`'s entry is the misc
// block's +0x5200 (its byte +34 the case's random one).
void PointItems(uint8_t* base, const World& w, uint32_t id) {
  const uint32_t object = w.misc.address + 0x5000;
  guest::StoreU32(base, data_tables::kTables, object);
  guest::StoreU32(base, object + 16, w.misc.address + 0x5200 - 3008 * id);
}

void Movies(Context& h, const World& w) {
  // The HUD's movies: +604 is movie 0, +608 1 ... +636 8.
  auto hud_movie = [&](uint32_t field) { return w.movie((field - 604) / 4); };
  {
    Spec spec;
    Group(
        h, w, "RaceHud::StartCountdown (sub_82212270)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeAnimations(h.base, w, hud_movie(612), r);
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82212270, hooks::RaceHudObjectStartCountdown, spec);
  }
  {
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::StartMovie1 (sub_82212718)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeAnimations(h.base, w, hud_movie(628), r);
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82212718, hooks::RaceHudObjectStartMovie1, spec);
  }
  {
    Spec spec;
    Group(
        h, w, "RaceHud::FinishMovie1 (sub_82212878)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeAnimations(h.base, w, hud_movie(628), r);
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82212878, hooks::RaceHudObjectFinishMovie1, spec);
  }
  {
    Spec spec;
    Group(
        h, w, "RaceHud::DrawWarning (sub_82212AC8)", 60,
        [&](PPCContext& t, Random& r, const World& w) {
          guest::StoreU32(h.base, w.hud.address + 468, r.Below(2) == 0 ? 0 : r.Below(3));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82212AC8, hooks::RaceHudObjectDrawWarning, spec);
  }
  {
    Spec spec;
    Group(
        h, w, "RaceHud::RunMovie1 (sub_82212AF8)", 40,
        [&](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.hud.address; },
        __imp__sub_82212AF8, hooks::RaceHudObjectRunMovie1, spec);
  }
  {
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::RunItem (sub_82212B08)", 60,
        [&](PPCContext& t, Random& r, const World& w) {
          guest::StoreU32(h.base, w.hud.address + 460,
                          r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(3) == 0 ? r.Next() : r.Below(40));
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82212B08, hooks::RaceHudObjectRunItem, spec);
  }
  {
    Spec spec;
    Group(
        h, w, "RaceScreens::MovieMask (sub_82211BE8)", 60,
        [&](PPCContext& t, Random& r, const World& w) {
          t.r3.u64 = w.hud.address;
          t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(13);
        },
        __imp__sub_82211BE8, hooks::RaceHudObjectMovieMask, spec);
  }
  {
    Spec spec;
    spec.frame = 128;
    spec.saves = 32;
    Group(
        h, w, "RaceHud::Init (sub_82215260)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeMovieTable(h.base, w, r);
          t.r3.u64 = w.hud.address;
        },
        __imp__sub_82215260, hooks::RaceHudObjectInit, spec);
  }
  {
    // The mirror's node (19) a primitive of up to 4 vertices from up to 3
    // texture entries (sub_8211BEE8 runs: its recorder call is the
    // stand-in's).
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::Draw (sub_822128A8)", 100,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t hud = w.hud.address;
          guest::StoreU32(h.base, hud + 472, r.Below(3) == 0 ? 0 : r.Below(3));
          guest::StoreU32(h.base, hud + 12, r.Below(3) == 0 ? 0 : 1);
          const uint32_t mirror = DashboardNode(h.base, w, 0xFFFFFFFFu, 19);
          guest::StoreU32(h.base, mirror + 112, r.Below(4));
          guest::StoreU32(h.base, mirror + 116, r.Below(5));
          t.r3.u64 = hud;
        },
        __imp__sub_822128A8, hooks::RaceHudObjectDraw, spec);
  }
  {
    // The car's +6168 against +468, each 0 or not.
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::UpdateWarning (sub_82212C50)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t hud = w.hud.address;
          ShapeAnimations(h.base, w, hud_movie(616), r);
          const uint32_t car = guest::LoadU32(h.base, hud + rh::kHudVehicle);
          guest::StoreU32(h.base, car + 6168, r.Below(2) == 0 ? 0 : r.Below(3) == 0 ? r.Next() : 1);
          guest::StoreU32(h.base, hud + 468, r.Below(2) == 0 ? 0 : 1);
          t.r3.u64 = hud;
        },
        __imp__sub_82212C50, hooks::RaceHudObjectUpdateWarning, spec);
  }
  {
    // HasRoom's 0, 1 or anything; +492 0 or not.
    Spec spec;
    spec.frame = 96;
    spec.saves = 16;
    Group(
        h, w, "RaceHud::UpdateRecordingFull (sub_82212E60)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t hud = w.hud.address;
          ShapeAnimations(h.base, w, hud_movie(608), r);
          guest::StoreU32(h.base, hud + 492, r.Below(2) == 0 ? 0 : 1);
          t.r3.u64 = hud;
          t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
        },
        __imp__sub_82212E60, hooks::RaceHudObjectUpdateRecordingFull, spec);
  }
  {
    // The racers' movie's live scene a chunk list (ShapeChunks); the item
    // -1, an id of the item table (0 to 399) or any; SetNodeText recorded.
    Spec spec;
    spec.frame = 128;
    spec.saves = 48;
    Group(
        h, w, "RaceHud::ShowItem (sub_82213638)", 200,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeChunks(h.base, w, r);
          t.r3.u64 = w.hud.address;
          t.r4.u64 = r.Below(3) == 0 ? 0xFFFFFFFFFFFFFFFFull : r.Below(400);
          PointItems(h.base, w, t.r4.u32);
          t.r7.u64 = t.r8.u64 = t.r9.u64 = t.r10.u64 = 0;
        },
        __imp__sub_82213638, hooks::RaceHudObjectShowItem, spec);
  }
  {
    // The race progress's +7240 against +460 (now and then the same).
    Spec spec;
    spec.frame = 128;
    spec.saves = 48;
    Group(
        h, w, "RaceHud::UpdateItem (sub_82214238)", 150,
        [&](PPCContext& t, Random& r, const World& w) {
          ShapeChunks(h.base, w, r);
          guest::StoreU32(h.base, rh::kRaceProgress, w.progress());
          const uint32_t item = r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(400);
          guest::StoreU32(h.base, w.progress() + 7240, item);
          PointItems(h.base, w, item);
          guest::StoreU32(h.base, w.hud.address + 460, r.Below(3) == 0 ? item : r.Below(400));
          t.r3.u64 = w.hud.address;
          t.r7.u64 = t.r8.u64 = t.r9.u64 = t.r10.u64 = 0;
        },
        __imp__sub_82214238, hooks::RaceHudObjectUpdateItem, spec);
  }
  {
    // operator new answered with blocks of the misc block (the list's now
    // and then 0); the UI movies' start, Strings::Load, UiMovies::Open and
    // MenuList::Init recorded; memset runs.
    auto news = [&](Random& r, const World& w) {
      g_new_264 = w.misc.address + 0x5400;
      g_new_72 = r.Below(6) == 0 ? 0 : w.misc.address + 0x5600;
    };
    {
      Spec spec;
      spec.frame = 128;
      spec.saves = 32;
      Group(
          h, w, "RaceScreens::Make1004 (sub_82216A60)", 60,
          [&](PPCContext& t, Random& r, const World& w) {
            news(r, w);
            t.r3.u64 = w.hud.address + 1004;
          },
          __imp__sub_82216A60, hooks::RaceHudObjectMake1004, spec);
    }
    {
      // The holder the HUD block (the HUD at its +16), the race kind 0 to
      // 12 or any, the movie table for RaceHud::Init.
      Spec spec;
      spec.frame = 128;
      spec.saves = 48;
      Group(
          h, w, "RaceScreens::Init (sub_82211DA0)", 100,
          [&](PPCContext& t, Random& r, const World& w) {
            news(r, w);
            ShapeMovieTable(h.base, w, r);
            t.r3.u64 = w.hud.address;
            t.r4.u64 = r.Below(5) == 0 ? r.Next() : r.Below(13);
          },
          __imp__sub_82211DA0, hooks::RaceHudObjectScreensInit, spec);
    }
  }
}

// The sound manager ([0x826C5C80]) for MenuSound::Play and the
// IsCuePlaying slot: a fake one in the misc block (+0x5600, its words the
// case's random ones) whose vtable (+0x5800) has leaf recorders, recompiled
// and the same on both sides, as the menu sounds' harness has them: slot 10
// (PlayCue2D) sub_820A8420 (r4's low byte to +320, r5 to +332, r6 to +336;
// r3 left) or the getter sub_82090770 (r3 = +60); slot 14 (IsCuePlaying,
// which reads r3 to r5 only) the getter, its answer at +60 0, 1 or any word
// (the low byte tested).
void ShapeSoundManager(uint8_t* base, const World& w, Random& r) {
  constexpr uint32_t kRecordR4R6 = 0x820A8420, kGet60 = 0x82090770;
  const uint32_t manager = w.misc.address + 0x5600;
  const uint32_t vtable = w.misc.address + 0x5800;
  guest::StoreU32(base, manager, vtable);
  const uint32_t answer = r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2);
  guest::StoreU32(base, manager + 60, answer);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot,
                  r.Below(3) == 0 ? kGet60 : kRecordR4R6);
  guest::StoreU32(base, vtable + 56, kGet60);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

void Sounds(Context& h, const World& w) {
  {
    // The countdown's animation (index 0, sub-scene -1) at one of the times
    // the sounds play at, near them or any, in the looping list or the
    // queue, sometimes after other slots or none; the sound manager fake.
    Spec spec;
    spec.frame = 112;
    spec.saves = 16;
    spec.f1 = true;
    Group(
        h, w, "RaceHud::RunCountdown (sub_822129A8)", 300,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t movie = w.movie((612 - 604) / 4);
          const uint32_t scene = guest::LoadU32(h.base, movie + ui_movies::kMovieLive);
          if (r.Below(6) != 0) {
            static constexpr float kTimes[] = {60.0f, 120.0f, 180.0f, 240.0f, 60.75f,
                                               119.99f, 239.5f, 0.0f,  300.0f, -60.0f};
            float time = kTimes[r.Below(10)];
            switch (r.Below(10)) {
              case 0:
                time = AnyFloat(r, 400.0f);
                break;
              case 1:
                time = std::numeric_limits<float>::quiet_NaN();
                break;
              case 2:
                time = 3.0e9f;
                break;
            }
            ui_movies::Slot slot{};
            slot[0] = 0;
            slot[1] = 0xFFFFFFFFu;
            slot[2] = 0xFFFFFFFFu;
            slot[3] = scene;
            slot[4] = w.movies.address + kAnimationsAt + 64 * r.Below(8);
            slot[5] = Bits(time);
            ui_movies::Append(h.base, movie + (r.Below(2) == 0 ? ui_movies::kMovieQueue
                                                               : ui_movies::kMovieLooping),
                              slot);
          }
          if (r.Below(3) == 0) {
            ShapeAnimations(h.base, w, movie, r);
          }
          ShapeSoundManager(h.base, w, r);
          t.r3.u64 = w.hud.address;
          t.f1.f64 = AnyDouble(r, 10.0f);
        },
        __imp__sub_822129A8, hooks::RaceHudObjectRunCountdown, spec);
  }
  {
    // A bar (0 to 3, mostly 0 for the sound) of the HUD's four at +244:
    // value, steps, trail, min and max mostly ordinary, +32 and +36 0 or 1;
    // the racers ([+444], the misc block's) and the racer's +788 an int
    // around the bar's range; the racers' movie's animations; the sound
    // manager fake.
    Spec spec;
    spec.frame = 176;
    spec.saves = 80;
    for (uint32_t b = 0; b < 4; b++) {
      spec.Floats(244 + 40 * b, 1);
      spec.Floats(244 + 40 * b + 8, 1);
      spec.Floats(244 + 40 * b + 24, 1);
    }
    Group(
        h, w, "RaceHud::UpdateRacerBar (sub_82214790)", 800,
        [&](PPCContext& t, Random& r, const World& w) {
          const uint32_t hud = w.hud.address;
          const uint32_t index = r.Below(2) == 0 ? 0 : r.Below(4);
          const uint32_t racer = r.Below(kVehicleCount);
          const uint32_t bar = hud + 244 + 40 * index;
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
            guest::StoreU32(h.base, bar + 8, guest::LoadU32(h.base, bar + 0));
          }
          guest::StoreU32(h.base, bar + 28, r.Below(2));
          guest::StoreU32(h.base, bar + 32, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
          guest::StoreU32(h.base, bar + 36, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
          guest::StoreU32(h.base, hud + 444, w.racers());
          guest::StoreU32(h.base, w.racers() + 972 * racer + 788,
                          r.Below(6) == 0 ? uint32_t(r.Next())
                                          : uint32_t(int32_t(r.Below(300)) - 30));
          ShapeAnimations(h.base, w, w.movie(0), r);
          ShapeSoundManager(h.base, w, r);
          t.r3.u64 = hud;
          t.r4.u64 = index;
          t.r5.u64 = racer;
        },
        __imp__sub_82214790, hooks::RaceHudObjectUpdateRacerBar, spec);
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
  SetUps(h, w);
  Signs(h, w);
  Ticks(h, w);
  Movies(h, w);
  Sounds(h, w);
}

}  // namespace harness
