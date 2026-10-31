// The race HUD machine (src/race_hud_machine/,
// docs/functions/race-hud-machine.md) against the recompiled functions. The
// originals run with --native_race_hud_machine off, the plain C++ through
// the register steps (hooks/race_hud_machine.h). The handlers' callees (the
// race HUD's and the alternate HUD's functions) run for real on both sides:
// the recompiled handlers reach the callees' originals (directly, the
// adapters of the callees only they call in play deleted, 2026-10-31; the
// others' with their flags, --native_race_hud_object, --native_alt_hud and
// --native_race_hud, off), the native ones the callees' plain C++. So each case builds the
// world they read, the race HUD's as the race HUD object's harness builds it
// and the alternate HUD's as the alternate HUD's does (the user's call,
// 2026-10-31: no stand-in for the callees), with the guest calls those
// harnesses record recorded the same way (the harnesses' recorder,
// hooks::g_start_up_recorder, and the UI movies' stand-in), and compares
// the calls, everything the handler and its callees write (the machine,
// the holder with the HUD and the alternate HUD's holder, the dispatcher's
// ring, the movies, the vehicles, the collision object, the save data's HUD
// options, the misc blocks, the race data's records and car objects, the
// track zones), the handler's frame (its register saves left out), r1, r3
// and f1. Kind 4, and kind 0 where the handler draws the holder's +12,
// aren't ported (the adapters run the originals there), so the cases leave
// them out. The helpers and Init have no callees to run. The game's globals
// the cases point at blocks of their own are put back after each side.
// Compared with harness::Outputs.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "game_modes/game_modes.h"
#include "alt_hud/alt_hud.h"
#include "hooks/app_start_up.h"
#include "hooks/race_hud_machine.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_hud_machine/race_hud_machine.h"
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

ITC_FLAG_DECLARE(bool, native_race_hud_machine);
ITC_FLAG_DECLARE(bool, native_race_hud_object);
ITC_FLAG_DECLARE(bool, native_alt_hud);
ITC_FLAG_DECLARE(bool, native_race_hud);
ITC_FLAG_DECLARE(bool, native_race_screens);

namespace harness {

// Vehicle::SetupFromCar's cases (src/harnesses/car_physics.cpp), for the car
// object the alternate HUD's SetUp sets up.
void WriteSetupFromCarCase(uint8_t* base, uint32_t data, uint32_t car, uint32_t r1,
                           uint32_t stack_end, uint64_t seed, int group);
void RunSetupFromCarOriginal(GuestFunction recompiled, PPCContext& ctx, uint8_t* base);

namespace {

namespace ah = alt_hud;
namespace rh = race_hud_object;
namespace rhm = race_hud_machine;

constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRacers = 0x826D82F4;
constexpr uint32_t kSaveSlots = 0x826C5B58;
constexpr uint32_t kMachineBytes = 0x2600;
constexpr uint32_t kHolderBytes = 0x500;
constexpr uint32_t kDispatcherBytes = 0x400;
constexpr uint32_t kRaceBytes = 0xA4000;  // to the kind (+0xA3AF0)
constexpr uint32_t kSaveBytes = 0x1B000;  // to the items' records (+0x199F8 + 12 x 409)
constexpr uint32_t kMiscBytes = 0x1000;  // the racers' manager (+0), its entry (+0xA00), the tables (+0xB00)
constexpr uint32_t kFirstItem = 370;      // the items block's first id
constexpr uint32_t kItems = 40;
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

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  World()
      : machine(kMachineBytes),
        holder(kHolderBytes),
        dispatcher(kDispatcherBytes),
        race(kRaceBytes),
        save(kSaveBytes),
        misc(kMiscBytes),
        items(3008 * kItems) {}
  bool ok() const {
    return machine.address && holder.address && dispatcher.address && race.address &&
           save.address && misc.address && items.address;
  }
  Block machine;
  Block holder;
  Block dispatcher;
  Block race;
  Block save;
  Block misc;
  Block items;
};

// A kind for a case: one of `kinds`, or now and then any word.
uint32_t AnyKind(Random& r, const std::vector<int32_t>& kinds) {
  if (r.Below(10) == 0) {
    const uint32_t word = uint32_t(r.Next());
    for (const int32_t k : kinds) {
      if (uint32_t(k) == word) {
        return word;
      }
    }
    return word < 7 ? 7 : word;  // never 0 to 6 by chance
  }
  return uint32_t(kinds[r.Below(uint32_t(kinds.size()))]);
}

// What a helper's case compares besides the blocks: the original's frame
// [sp - frame, sp - saves) (its register saves left out).
struct Spec {
  uint32_t frame = 0;
  uint32_t saves = 0;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// A helper's case (no callees): the blocks the helpers read and write, the
// frame, the caller's area, r1, r3 and f1.
void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const Spec& spec,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{kRaceData, 4},
                              {kRacers, 4},
                              {kSaveSlots, 16},
                              {data_tables::kTables, 4}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    guest::StoreU32(h.base, kRaceData, w.race.address);
    for (uint32_t p = 0; p < 4; p++) {
      guest::StoreU32(h.base, kSaveSlots + 4 * p, w.save.address);
    }
    guest::StoreU32(h.base, data_tables::kTables, w.misc.address + 0xB00);
    guest::StoreU32(h.base, w.misc.address + 0xB00 + 16, w.items.address - 3008 * kFirstItem);
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    t.f1.u64 = r.Next();
    ITC_FLAG(native_race_hud_machine) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_hud_machine) = true;
    out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
    if (spec.frame > spec.saves) {
      out[v].Bytes(Read(h.base, sp - spec.frame, spec.frame - spec.saves));
    }
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    out[v].Double(t.f1.u64);
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

// --- The handlers: their callees run for real on both sides ---
//
// The race HUD's world (the HUD at the holder's +16) as the race HUD
// object's harness builds it, the alternate HUD's (the holder's +1004, its
// object and menu list) as the alternate HUD's does, in one: the movies
// (the HUD's nine and the alternate HUD's movie 22), a node pool, the
// vehicles and their build, a collision object, player 0's save data, the
// race data (the racer records and their car objects), the track zones'
// owner, the racers' manager, the data tables' items, the sound manager,
// the action mappers and the input records.

constexpr uint32_t kVehicles = 0x826C2E34;  // Vehicle::Get's array
constexpr uint32_t kOwner = 0x828161EC;     // the track zones' owner
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kVehicleCount = 4;
constexpr uint32_t kHudHolderBytes = 0x1400;  // the HUD at +16 (0x1000 of it), +1004 the alt's
// The movie block: ten movies (0xA000 each: the HUD's nine, then movie 22)
// at +0, then the node pool, then 8 animations of no tracks (64 bytes each).
constexpr uint32_t kMovieHeaders = 10;
constexpr uint32_t kMovieBytes = 0xA000;
constexpr uint32_t kNodesAt = kMovieHeaders * kMovieBytes;
constexpr uint32_t kNodeBytes = 0x800;
constexpr uint32_t kNodeCount = 24;
constexpr uint32_t kAnimationsAt = kNodesAt + kNodeBytes * kNodeCount;
constexpr uint32_t kMoviesBytes = kAnimationsAt + 8 * 64;
constexpr uint32_t kZonesAt = 0x400;
constexpr uint32_t kZonesBytes = 0x5000;
constexpr uint32_t kCollisionBytes = 0x1000;
constexpr uint32_t kHudSaveBytes = 0x20000;
constexpr uint32_t kHudMiscBytes = 0x6000;
constexpr uint32_t kAltMiscBytes = 0x3000;
constexpr uint32_t kHudRaceBytes = 0xA4000;
constexpr uint32_t kHandlerStackBytes = 0x2000;  // the callees' frames below the handler's

struct HudWorld {
  HudWorld()
      : machine(kMachineBytes),
        holder(kHudHolderBytes),
        dispatcher(kDispatcherBytes),
        movies(kMoviesBytes),
        vehicles(kVehicleBytes * kVehicleCount),
        collision(kCollisionBytes),
        save(kHudSaveBytes),
        misc(kHudMiscBytes),
        alt(kAltMiscBytes),
        race(kHudRaceBytes),
        zones(kZonesBytes) {}
  bool ok() const {
    return machine.address && holder.address && dispatcher.address && movies.address &&
           vehicles.address && collision.address && save.address && misc.address &&
           alt.address && race.address && zones.address;
  }
  uint32_t hud() const { return holder.address + rhm::kHud; }
  uint32_t alt_holder() const { return holder.address + rhm::kAltHud; }
  // The race HUD object's misc block's parts.
  uint32_t viewport() const { return misc.address + 0x10; }
  uint32_t build() const { return misc.address + 0x800; }
  uint32_t textures() const { return misc.address + 0x1000; }
  uint32_t racers() const { return misc.address + 0x2000; }
  uint32_t progress() const { return misc.address + 0x3000; }
  // The alternate HUD's.
  uint32_t object() const { return alt.address + 0x100; }
  uint32_t list() const { return alt.address + 0x300; }
  uint32_t pairs() const { return alt.address + 0x400; }
  uint32_t vtable(uint32_t i) const { return alt.address + 0x500 + 0x40 * i; }
  uint32_t mapper(uint32_t i) const { return alt.address + 0x600 + 0x200 * i; }
  uint32_t input(uint32_t i) const { return alt.address + 0x2000 + 0x20 * i; }
  uint32_t manager() const { return alt.address + 0x1000; }  // the racers' manager
  uint32_t entry() const { return alt.address + 0x1A00; }    // its +2308
  uint32_t movie(uint32_t i) const { return movies.address + kMovieBytes * i; }
  uint32_t alt_movie() const { return movie(9); }
  uint32_t node(uint32_t i) const { return movies.address + kNodesAt + kNodeBytes * i; }
  uint32_t vehicle(uint32_t i) const { return vehicles.address + kVehicleBytes * i; }
  uint32_t zone(uint32_t i) const { return zones.address + kZonesAt + 16 * i; }
  uint32_t record(uint32_t i) const { return race.address + rh::kRecordBytes * i; }
  Block machine;
  Block holder;
  Block dispatcher;
  Block movies;
  Block vehicles;
  Block collision;
  Block save;
  Block misc;  // the race HUD object's: the viewports (+0), the build (+0x800), the
               // textures (+0x1000), the signs' flags (+0x1200), the racers (+0x2000),
               // the race's progress (+0x3000), the data tables (+0x5000, the item
               // +0x5200), the sound manager (+0x5600, its vtable +0x5800)
  Block alt;   // the alternate HUD's: its object (+0x100), menu list (+0x300), the
               // movie table's pairs (+0x400), the mappers' vtables (+0x500) and
               // mappers (+0x600), the racers' manager (+0x1000, its +2308 at
               // +0x1904) and its entry (+0x1A00), the input records (+0x2000)
  Block race;
  Block zones;
};

// The guest calls the harnesses' recorder takes (TrackZones::Create,
// UiMovie::SetText, ClearText, SetNodeText, the UI movies' start,
// Strings::Load, UiMovies::Open, operator new, MenuList::Init: the race HUD
// object's harness's), and the UI movies' update and the full-frame
// effect's recorder call (the UI movies' stand-in), recorded as those
// harnesses record them and answered with words of the case's generator.
struct Call {
  uint32_t function, lr, r1;
  uint64_t r3, r4, r5, r6;
  uint64_t more[6];
};
std::vector<Call>* g_calls = nullptr;
uint8_t* g_base = nullptr;
Random g_answers;

bool Recorder(PPCContext& ctx, uint32_t function) {
  if (g_calls == nullptr) {
    return false;
  }
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
  const uint64_t regs[4] = {ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64};
  Call call{function, uint32_t(ctx.lr), ctx.r1.u32, 0, 0, 0, 0, {}};
  uint64_t* to[4] = {&call.r3, &call.r4, &call.r5, &call.r6};
  for (uint32_t i = 0; i < reads; i++) {
    *to[i] = regs[i];
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
  ctx.r3.u64 = function == 0x825A6880 ? 0 : g_answers.Next();
  return true;
}

// UiMovie::Draw's callees (the movie draw sub_8259FA00, RestorePose
// sub_82119B80, DrawTexts sub_8211CD90 and sub_8259DAC0, which RaceHud::
// RunMovie's draw reaches) are recorded as the race HUD's harness records
// them (r3 to r7, r3 to r5, r3 and r4), the nodes they'd draw being random
// words here.
bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_calls == nullptr) {
    return false;
  }
  switch (function) {
    case 0x8211D2F8:
    case 0x8259EBE8:
      g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64,
                          function == 0x8259EBE8 ? ctx.f1.u64 : 0, 0, {}});
      ctx.r3.u64 = g_answers.Next();
      return true;
    case 0x8259FA00:
    case 0x82119B80:
      g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64,
                          ctx.r5.u64, ctx.r6.u64, {ctx.r7.u64}});
      return true;
    case 0x8211CD90:
      g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64,
                          ctx.r5.u64, 0, {}});
      return true;
    case 0x8259DAC0:
      g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64, 0, 0,
                          {}});
      return true;
    default:
      return false;
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

// A finite float (TickTimer's: a NaN distance writes negative digits into
// the nodes' child indices, the game's own behavior the world can't hold).
float FiniteFloat(Random& r, float range) {
  const float value = AnyFloat(r, range);
  return std::isfinite(value) ? value : 0.0f;
}

// The race HUD's world as the race HUD object's harness shapes it (its
// ShapeWorld), the race data always ours.
void ShapeHud(uint8_t* base, const HudWorld& w, Random& r) {
  const uint32_t hud = w.hud();
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
    if (m < 9) {
      guest::StoreU32(base, hud + 604 + 4 * m, w.movie(m));
    }
  }
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
  guest::StoreU32(base, hud + rh::kHudVehicle, w.vehicle(r.Below(kVehicleCount)));
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
  const uint32_t race = w.race.address;
  guest::StoreU32(base, rh::kRaceData, race);
  guest::StoreU32(base, race + 4, r.Below(25));
  guest::StoreU32(base, race + 20, r.Below(64));
  guest::StoreU32(base, race + rh::kRaceMode, r.Below(10));
  guest::StoreU32(base, race + 0x59620 + 804, r.Below(10));
  for (uint32_t k = 0; k < 10; k++) {
    guest::StoreU32(base, race + 0x59620 + 80 * k, r.Below(3));
  }
}

// The alternate HUD's world as its harness shapes it (its ShapeWorld): the
// holder's +1004 (+0 the object, +4 the menu list), movie 22 in the movie
// table, the object's car and player, the menu list's controller, the
// mappers and the input records.
void ShapeAlt(uint8_t* base, const HudWorld& w, Random& r) {
  guest::StoreU32(base, w.alt_holder(), w.object());
  guest::StoreU32(base, w.alt_holder() + 4, w.list());
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
      guest::StoreU32(base, w.pairs() + 8 * i + 4, w.alt_movie());
    }
  }
  guest::StoreU32(base, ui_movies::kMovieTable, w.pairs());
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 24);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
  guest::StoreU32(base, w.object() + ah::kCar, w.vehicle(r.Below(kVehicleCount)));
  guest::StoreU32(base, w.object() + ah::kPlayer,
                  r.Below(8) == 0 ? 0x100 + r.Below(4) : r.Below(4));
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
}

// A curve: up to 12 points with x ascending, +88 the largest x and +92 the
// smallest (or random bounds), as the HUDs' harnesses shape them.
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

void ShapeGroup(uint8_t* base, uint32_t node, Random& r) {
  guest::StoreU32(base, node + 100, r.Below(100));
  guest::StoreU32(base, node + 104, r.Below(8) == 0 ? uint32_t(-int32_t(r.Below(3))) : r.Below(5));
}

// A movie's animations (the race HUD object's harness's ShapeAnimations).
void ShapeAnimations(uint8_t* base, const HudWorld& w, uint32_t movie, Random& r) {
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

// The countdown's animation on movie 2 (RunCountdown's cases).
void ShapeCountdown(uint8_t* base, const HudWorld& w, Random& r) {
  const uint32_t movie = w.movie(2);
  const uint32_t scene = guest::LoadU32(base, movie + ui_movies::kMovieLive);
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
    ui_movies::Append(base, movie + (r.Below(2) == 0 ? ui_movies::kMovieQueue
                                                     : ui_movies::kMovieLooping),
                      slot);
  }
  if (r.Below(3) == 0) {
    ShapeAnimations(base, w, movie, r);
  }
}

// The racers' movie's live scene as a chunk list (ShowItem's).
void ShapeChunks(uint8_t* base, const HudWorld& w, Random& r) {
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

// The sound manager fake (the race HUD object's harness's).
void ShapeSoundManager(uint8_t* base, const HudWorld& w, Random& r) {
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

// The signs' fields (the race HUD object's harness's ShapeSigns).
void ShapeSigns(uint8_t* base, const HudWorld& w, Random& r) {
  const uint32_t hud = w.hud();
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
  guest::StoreU32(base, w.race.address + rh::kRaceMode, r.Below(3) == 0 ? 6 : r.Below(10));
  guest::StoreU32(base, w.race.address + 4, r.Below(4) == 0 ? 8 + r.Below(3) : r.Below(25));
}

// A racer bar of the HUD's four (UpdateRacerBar's cases).
void ShapeRacerBar(uint8_t* base, const HudWorld& w, uint32_t index, uint32_t racer, Random& r) {
  const uint32_t hud = w.hud();
  const uint32_t bar = hud + 244 + 40 * index;
  const bool ordinary = r.Below(4) != 0;
  auto value = [&](float range) {
    return ordinary ? float(r.Below(int(range) + 1)) : AnyFloat(r, range);
  };
  guest::StoreFloat(base, bar + 0, value(200.0f));
  guest::StoreFloat(base, bar + 4, value(20.0f));
  guest::StoreFloat(base, bar + 8, value(200.0f));
  guest::StoreFloat(base, bar + 12, value(20.0f));
  guest::StoreFloat(base, bar + 16, ordinary ? 0.0f : AnyFloat(r, 50.0f));
  guest::StoreFloat(base, bar + 20, ordinary ? 100.0f + float(r.Below(100)) : AnyFloat(r, 300.0f));
  if (r.Below(4) == 0) {
    guest::StoreU32(base, bar + 8, guest::LoadU32(base, bar + 0));
  }
  guest::StoreU32(base, bar + 28, r.Below(2));
  guest::StoreU32(base, bar + 32, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, bar + 36, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, hud + 444, w.racers());
  guest::StoreU32(base, w.racers() + 972 * racer + 788,
                  r.Below(6) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(300)) - 30));
}

// A car's values (a speed in m/s, the extra gauges' floats, the gear, the
// tachometer's +2224, the bar, the boost), as the HUDs' Tick cases shape them.
void ShapeCar(uint8_t* base, uint32_t car, Random& r) {
  const bool ordinary = r.Below(4) != 0;
  guest::StoreFloat(base, car + 48, ordinary ? float(r.Below(90)) : AnyFloat(r, 400.0f));
  for (const uint32_t offset : {6252u, 6256u, 6260u, 2224u, 2420u, 2944u}) {
    guest::StoreFloat(base, car + offset,
                      ordinary ? float(r.Below(200)) / 2.0f : AnyFloat(r, 300.0f));
  }
  guest::StoreU32(base, car + 2252, r.Below(9) - 1);
}

// The record indices both gauges' set-ups take: the race HUD's table
// (0x8200D3B8 + 16 x index, of 8) and the alternate HUD's (0x8200FEE8 + 16 x
// index, of 16), each naming one of the three curves.
std::vector<uint32_t> CurveIndices(uint8_t* base) {
  std::vector<uint32_t> indices;
  for (uint32_t i = 0; i < 8; i++) {
    if (guest::LoadU32(base, 0x8200D3B8 + 16 * i) < 3 &&
        guest::LoadU32(base, 0x8200FEE8 + 16 * i) < 3) {
      indices.push_back(i);
    }
  }
  return indices;
}

// The car object AltHud::PowerClass sets up (a racer record's +48), as the
// alternate HUD's harness shapes one (its ShapePowerCar): Vehicle::
// SetupFromCar's case in PowerClass's frame (`power_sp` - 3040).
void ShapePowerCar(uint8_t* base, uint32_t car, uint32_t power_sp,
                   const std::vector<uint32_t>& indices, Random& r) {
  const uint32_t frame = power_sp - 3040;
  WriteSetupFromCarCase(base, frame + 1152, car, frame, power_sp, r.Below(10000),
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

// What the handlers' callees read, shaped as each callee's own harness
// shapes it (the fields two callees share given values both take): the
// race HUD's Tick (the signs, the car, the curves, the speed's digits, the
// extra gauges' slots), UpdateView, UpdateWarning, UpdateItem (the item and
// its chunks), TickPanel (its mode 0, 1, 3, 5 or -1, the racers and their
// bars, the timer), Draw (the mirror), RunWarning, RunItem, RunMovie (its
// state: +448, TickPanel's mode, 3 left out with the draws, 2 and 4 with
// TickPanel), RunCountdown, the intro movie (movie 6), Reset, SetUpGauges
// (records 0 to 3), SetUpExtraGauges, SetUpMirror and ApplyOptions; the
// alternate HUD's Tick (its car, curves, digits, slots and bar).
void ShapeCallees(uint8_t* base, const HudWorld& w, Random& r, bool draws,
                  const std::vector<uint32_t>& indices) {
  const uint32_t hud = w.hud();
  auto dashboard = [&](uint32_t child) {
    return ui_movies::FindNode(base, guest::LoadU32(base, hud + rh::kHudDashboard), 0xFFFFFFFFu,
                               child);
  };
  ShapeSigns(base, w, r);
  ShapeCar(base, guest::LoadU32(base, hud + rh::kHudVehicle), r);
  ShapeCurve(base, hud + 20, r);
  ShapeCurve(base, hud + 132, r);
  ShapeGroup(base, dashboard(9), r);
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, hud + 480 + 4 * k, r.Below(4) - 1);
  }
  guest::StoreU32(base, w.viewport() + 1860, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(4));
  guest::StoreU32(base, hud + 12, r.Below(2));
  ShapeAnimations(base, w, w.movie(3), r);
  const uint32_t car = guest::LoadU32(base, hud + rh::kHudVehicle);
  guest::StoreU32(base, car + 6168, r.Below(2) == 0 ? 0 : r.Below(3) == 0 ? r.Next() : 1);
  guest::StoreU32(base, hud + 468,
                  r.Below(3) == 0 ? 0 : r.Below(2) == 0 ? 1 : uint32_t(r.Next()));
  // UpdateItem's and RunItem's item.
  ShapeChunks(base, w, r);
  guest::StoreU32(base, rh::kRaceProgress, w.progress());
  const uint32_t item = r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(400);
  guest::StoreU32(base, w.progress() + 7240, item);
  const uint32_t tables = w.misc.address + 0x5000;
  guest::StoreU32(base, data_tables::kTables, tables);
  guest::StoreU32(base, tables + 16, w.misc.address + 0x5200 - 3008 * item);
  guest::StoreU32(base, hud + 460,
                  r.Below(3) == 0 ? item : r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(400));
  // TickPanel's mode and RunMovie's state (+448).
  static constexpr uint32_t kTickModes[] = {0, 0, 1, 1, 3, 3, 5, 0xFFFFFFFFu};
  static constexpr uint32_t kDrawModes[] = {0, 0, 1, 1, 5, 5, 0xFFFFFFFFu, 0x10};
  guest::StoreU32(base, hud + 448, draws ? kDrawModes[r.Below(8)] : kTickModes[r.Below(8)]);
  guest::StoreU32(base, hud + 452, r.Below(5));
  guest::StoreU32(base, hud + rh::kHudPlayer, r.Below(4));
  for (uint32_t i = 0; i < 4; i++) {
    ShapeRacerBar(base, w, i, i, r);
  }
  const uint32_t racers = w.racers();
  guest::StoreU32(base, racers + 3932, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(200000));
  guest::StoreFloat(base, racers + 828,
                    r.Below(4) != 0 ? float(r.Below(10000)) : FiniteFloat(r, 1e5f));
  guest::StoreFloat(base, w.progress() + 7156,
                    r.Below(4) != 0 ? float(r.Below(20000)) : FiniteFloat(r, 1e5f));
  const uint32_t animation = r.Below(7) - 1;
  guest::StoreU32(base, racers + 12, animation);
  guest::StoreU32(base, hud + 464, r.Below(3) == 0 ? animation : r.Below(7) - 1);
  ShapeAnimations(base, w, w.movie(0), r);
  ShapeAnimations(base, w, w.movie(1), r);
  ShapeAnimations(base, w, w.movie(5), r);
  guest::StoreU32(base, hud + 492, r.Below(2) == 0 ? 0 : 1);
  ShapeSoundManager(base, w, r);
  // Draw's mirror and options; RunMovie's +456.
  guest::StoreU32(base, hud + 472, r.Below(3) == 0 ? 0 : r.Below(3));
  const uint32_t mirror = dashboard(19);
  guest::StoreU32(base, mirror + 112, r.Below(4));
  guest::StoreU32(base, mirror + 116, r.Below(5));
  guest::StoreU32(base, hud + 456, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  ShapeCountdown(base, w, r);
  ShapeAnimations(base, w, w.movie(6), r);
  // The set-ups: Reset's group, the records' curve indices, the build's
  // extra gauges, the mirror's target.
  ShapeGroup(base, dashboard(1), r);
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t record = w.record(p);
    guest::StoreU32(base, record + 60,
                    indices.empty() ? 0 : indices[r.Below(uint32_t(indices.size()))]);
    guest::StoreU32(base, record + 1704, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(10));
  }
  for (uint32_t k = 0; k < 5; k++) {
    guest::StoreU32(base, w.build() + 1832 + 12 * k, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(6));
  }
  guest::StoreU32(base, w.build() + 12, r.Below(3) == 0 ? 10 : r.Below(12));
  guest::StoreU32(base, hud + 8, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, hud + 588, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
  // The alternate HUD's Tick.
  ShapeCar(base, guest::LoadU32(base, w.object() + ah::kCar), r);
  ShapeCurve(base, w.object() + ah::kTach, r);
  ShapeCurve(base, w.object() + ah::kBoost, r);
  ShapeGroup(base, ui_movies::FindNode(base, w.alt_movie(), 0xFFFFFFFFu, 21), r);
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, w.object() + ah::kSlots + 4 * k, r.Below(4) - 1);
  }
  guest::StoreU32(base, w.object() + 232, Bits(AnyFloat(r, 100.0f)));
  guest::StoreU32(base, w.object() + 236, Bits(AnyFloat(r, 100.0f)));
}

// The holder's float words (the HUD's curves +20 to +244, its racer bars'
// +0, +8 and +24, its bar +404 to +416), as the race HUD object's harness
// compares them; the alternate HUD's object's (its curves and bar, +4 to
// +232), as its harness does.
std::set<uint32_t> HolderFloats() {
  std::set<uint32_t> floats;
  const uint32_t hud = rhm::kHud;
  for (uint32_t i = 0; i < 56; i++) {
    floats.insert(hud + 20 + 4 * i);
  }
  for (uint32_t b = 0; b < 4; b++) {
    floats.insert(hud + 244 + 40 * b);
    floats.insert(hud + 244 + 40 * b + 8);
    floats.insert(hud + 244 + 40 * b + 24);
  }
  for (uint32_t i = 0; i < 3; i++) {
    floats.insert(hud + 404 + 4 * i);
  }
  return floats;
}

void BlockOutputs(Outputs& out, uint8_t* base, uint32_t block, uint32_t bytes,
                  const std::set<uint32_t>& floats) {
  uint32_t at = 0;
  for (const uint32_t offset : floats) {
    if (offset > at) {
      out.Bytes(Read(base, block + at, offset - at));
    }
    out.Floats(base, block + offset, 1);
    at = offset + 4;
  }
  out.Bytes(Read(base, block + at, bytes - at));
}

// A handler's case: the handler's frame [sp - frame, sp - saves) (its
// register saves left out) compared; `setup_from_car`: the original runs
// with Vehicle::SetupFromCar's callees' native flags off, as the alternate
// HUD's harness runs its SetUp.
// The menu after the race's menu list ([0x826D851C], AfterRaceMenu::Close's).
constexpr uint32_t kAfterRaceList = 0x826D851C;
// The career's results' screen ([0x826D8500], CareerResults::Start's).
constexpr uint32_t kCareerResults = 0x826D8500;

struct HandlerSpec {
  uint32_t frame = 0;
  uint32_t saves = 0;
  bool setup_from_car = false;
};

// Sets up a case's own inputs (the kind and the rest) after the world.
using HandlerSetupFn = std::function<void(PPCContext&, Random&, const HudWorld&)>;

void HandlerCase(Context& h, Results& results, const HudWorld& w, uint64_t seed,
                 const HandlerSetupFn& setup, GuestFunction recompiled, GuestFunction native,
                 const HandlerSpec& spec, bool draws, const std::vector<uint32_t>& indices,
                 bool (*f1)(int32_t kind, bool replay), const std::string& what) {
  static const std::set<uint32_t> holder_floats = HolderFloats();
  static const std::set<uint32_t> object_floats = [] {
    std::set<uint32_t> floats;
    for (uint32_t i = 0; i < 57; i++) {
      floats.insert(0x100 + 4 + 4 * i);
    }
    return floats;
  }();
  // The seed mixed (splitmix64's finalizer): the cases' seeds are one
  // generator's consecutive states, and runs of the same generator shifted
  // by one step fall into step with each other where the shaping draws a
  // varying number of words.
  uint64_t mixed = seed + 0x9E3779B97F4A7C15ull;
  mixed = (mixed ^ (mixed >> 30)) * 0xBF58476D1CE4E5B9ull;
  mixed = (mixed ^ (mixed >> 27)) * 0x94D049BB133111EBull;
  mixed ^= mixed >> 31;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(mixed);
    SavedBytes saved(h.base, {{kVehicles, 4},
                              {kSaveSlots, 16},
                              {rh::kViewports, 4},
                              {rh::kRaceData, 4},
                              {ah::kInputRecords, 16},
                              {kOwner, 4},
                              {rh::kRaceProgress, 4},
                              {kRacers, 4},
                              {ui_movies::kMovieTable, 16},
                              {data_tables::kTables, 4},
                              {menu_sounds::kSoundManager, 4},
                              {kMapperEntries, 32},
                              {kControllerEntries, 16},
                              {kAfterRaceList, 4},
                              {kCareerResults, 4}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kHandlerStackBytes, kHandlerStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.machine.address + 9600, 32, r);
    WriteRandom(h.base, w.holder.address, w.holder.bytes, r);
    WriteRandom(h.base, w.dispatcher.address, w.dispatcher.bytes, r);
    WriteRandom(h.base, w.movies.address, w.movies.bytes, r);
    WriteRandom(h.base, w.vehicles.address, w.vehicles.bytes, r);
    WriteRandom(h.base, w.collision.address, w.collision.bytes, r);
    WriteRandom(h.base, w.save.address + 0x1F800, 0x100, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    WriteRandom(h.base, w.alt.address, w.alt.bytes, r);
    WriteRandom(h.base, w.race.address, 4 * rh::kRecordBytes + 0x1000, r);
    WriteRandom(h.base, w.race.address + 0x59600, 0x400, r);
    WriteRandom(h.base, w.zones.address, w.zones.bytes, r);
    ShapeHud(h.base, w, r);
    ShapeAlt(h.base, w, r);
    ShapeCallees(h.base, w, r, draws, indices);
    guest::StoreU32(h.base, w.machine.address + rhm::kHolder, w.holder.address);
    guest::StoreU32(h.base, kRacers, w.manager());
    guest::StoreU32(h.base, w.manager() + 2308, w.entry());
    // The ring's count and start small (its entries within the block).
    guest::StoreU32(h.base, w.dispatcher.address + 804, r.Below(60));
    guest::StoreU32(h.base, w.dispatcher.address + 808, r.Below(60));
    t.r3.u64 = w.machine.address;
    t.r4.u64 = w.dispatcher.address;
    t.r7.u64 = t.r8.u64 = t.r9.u64 = t.r10.u64 = 0;
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    t.f1.u64 = r.Next();
    g_answers = Random(r.Next());
    std::vector<Call> calls;
    g_calls = &calls;
    g_base = h.base;
    const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
    hooks::g_start_up_recorder = Recorder;
    const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
        ui_movies_hooks::g_update_callee_stand_in;
    ui_movies_hooks::g_update_callee_stand_in = StandIn;
    // The original reaches its callees' originals (their adapters' flags
    // off); the plain C++ calls theirs.
    bool* flags[] = {&ITC_FLAG(native_race_hud_machine), &ITC_FLAG(native_race_hud_object),
                     &ITC_FLAG(native_alt_hud), &ITC_FLAG(native_race_hud),
                     &ITC_FLAG(native_race_screens)};
    for (bool* flag : flags) {
      *flag = v == 1;
    }
    const int32_t kind = int32_t(guest::LoadU32(h.base, w.machine.address + rhm::kKind));
    const bool replay = (guest::LoadU32(h.base, w.holder.address + rhm::kHolderFlags) & 1) != 0;
    if (v == 0 && spec.setup_from_car) {
      RunSetupFromCarOriginal(recompiled, t, h.base);
    } else {
      (v == 0 ? recompiled : native)(t, h.base);
    }
    for (bool* flag : flags) {
      *flag = true;
    }
    ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
    hooks::g_start_up_recorder = saved_recorder;
    g_calls = nullptr;
    out[v].Word(uint32_t(calls.size()));
    for (const Call& call : calls) {
      out[v].Word(call.function);
      out[v].Word(call.lr);
      out[v].Word(call.r1);
      for (const uint64_t x : {call.r3, call.r4, call.r5, call.r6, call.more[0], call.more[1],
                               call.more[2], call.more[3], call.more[4], call.more[5]}) {
        out[v].Word(uint32_t(x >> 32));
        out[v].Word(uint32_t(x));
      }
    }
    out[v].Word(guest::LoadU32(h.base, kOwner));
    out[v].Bytes(Read(h.base, w.machine.address + 9600, 32));
    BlockOutputs(out[v], h.base, w.holder.address, w.holder.bytes, holder_floats);
    out[v].Bytes(Read(h.base, w.dispatcher.address, w.dispatcher.bytes));
    out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
    out[v].Bytes(Read(h.base, w.vehicles.address, w.vehicles.bytes));
    out[v].Bytes(Read(h.base, w.collision.address, w.collision.bytes));
    out[v].Bytes(Read(h.base, w.save.address + 0x1F800, 0x100));
    out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
    BlockOutputs(out[v], h.base, w.alt.address, w.alt.bytes, object_floats);
    out[v].Bytes(Read(h.base, w.race.address, 4 * rh::kRecordBytes + 0x1000));
    out[v].Bytes(Read(h.base, w.zones.address, w.zones.bytes));
    if (spec.frame > spec.saves) {
      out[v].Bytes(Read(h.base, sp - spec.frame, spec.frame - spec.saves));
    }
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    if (f1 == nullptr || f1(kind, replay)) {
      out[v].Double(t.f1.u64);
    }
  }
  results.Check(out[0], out[1], what);
}

// The handlers' cases: the kind (signed handlers: 0 does nothing; the
// draws: 0 is the +12 screen's, left out), the replay bit (+8 bit 0) and
// the countdown's word (+452) 0 half the time; SetUp's kind 6 player's car
// object's word +3888 with or without bit 0x400, and the car object the
// alternate HUD's SetUp sets up (player 0, or kind 6's) shaped in its
// PowerClass's frame.
void Handlers(Context& h) {
  const HudWorld w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_hud_machine: no blocks from the allocation layer");
    return;
  }
  const std::vector<uint32_t> indices = CurveIndices(h.base);
  ITC_LOG_INFO("harness: race_hud_machine: {} curve indices both HUDs take", indices.size());
  const std::vector<int32_t> signed_kinds = {-1, 0, 1, 2, 3, 5, 6, 7, 8};
  const std::vector<int32_t> draw_kinds = {-1, 1, 2, 3, 5, 6, 7, 8};
  auto kinds = [&h](std::vector<int32_t> list) -> HandlerSetupFn {
    return [&h, list](PPCContext&, Random& r, const HudWorld& w) {
      guest::StoreU32(h.base, w.machine.address + rhm::kKind, AnyKind(r, list));
      guest::StoreU32(h.base, w.holder.address + rhm::kHolderFlags,
                      (uint32_t(r.Next()) & ~1u) | r.Below(2));
      guest::StoreU32(h.base, w.holder.address + rhm::kCountdownRunning,
                      r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    };
  };
  const HandlerSetupFn by_signed = kinds(signed_kinds);
  const HandlerSetupFn by_draw = kinds(draw_kinds);
  // SetUp: the alternate HUD's SetUp runs from the handler's 160-byte frame,
  // its SetUpGauges from SetUp's 96 and PowerClass from that one's 496.
  const HandlerSetupFn set_up = [&h, &by_signed, &indices](PPCContext& t, Random& r,
                                                           const HudWorld& w) {
    by_signed(t, r, w);
    const int32_t kind = int32_t(guest::LoadU32(h.base, w.machine.address + rhm::kKind));
    const uint32_t index = r.Below(4);
    guest::StoreU32(h.base, w.entry() + 24, index);
    const bool bit = r.Below(2) == 0;
    const uint32_t player = kind == 6 && bit ? 1 : 0;
    ShapePowerCar(h.base, w.record(player) + 48, t.r1.u32 - 160 - 96 - 496, indices, r);
    const uint32_t word = w.race.address + rh::kRecordBytes * (index + 1);
    guest::StoreU32(h.base, word,
                    (guest::LoadU32(h.base, word) & ~0x400u) | (bit ? 0x400u : 0));
  };
  // CloseResults: the race data's kind (+0xA3AF0: -1 to 7 but 6, now and
  // then any word; 6 runs the original, whose sub_8221C168 needs kind 6's
  // screen, never seen in play) and mode (+0xA3AF4: 0 half the
  // time); the menu after the race's list in the misc block (+0x4000)
  // whose block (+68) is 0 or the layer's pages (freed by the release).
  const HandlerSetupFn close_results = [&h, &by_signed](PPCContext& t, Random& r,
                                                        const HudWorld& w) {
    by_signed(t, r, w);
    uint32_t race_kind = r.Below(8) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(9)) - 1);
    if (race_kind == 6) {
      race_kind = 5;
    }
    guest::StoreU32(h.base, w.race.address + 0xA3AF0, race_kind);
    guest::StoreU32(h.base, w.race.address + 0xA3AF4, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    const uint32_t list = w.misc.address + 0x4000;
    guest::StoreU32(h.base, kAfterRaceList, list);
    guest::StoreU32(h.base, list + 68,
                    r.Below(3) == 0 ? 0 : memory::AllocPages(0x1000 * (1 + r.Below(2)), 4096));
  };
  // ResultsUpdate: the race data's kind -1, 0, 1, 3, 5 or 7 (2, 4, 6 and 1
  // with +0xA3AF4 0 run the original, their results never seen in play),
  // its mode (+0xA3AF4) not 0 for kind 1; player 0's day (the save data's
  // +6100, IsFirstDay's) 1 or not; the career's results (holder +1024) a
  // block of the misc one (+0x4100, also [0x826D8500], which the start
  // reads) whose page (+184) is none (-1), 0 (the start: ScreenPages::Go)
  // or 4 (the shared "returns 1").
  const HandlerSetupFn results_update = [&h, &by_signed](PPCContext& t, Random& r,
                                                         const HudWorld& w) {
    by_signed(t, r, w);
    const int32_t kinds[6] = {-1, 0, 1, 3, 5, 7};
    const int32_t race_kind = kinds[r.Below(6)];
    guest::StoreU32(h.base, w.race.address + 0xA3AF0, uint32_t(race_kind));
    guest::StoreU32(h.base, w.race.address + 0xA3AF4, 1 + r.Below(3));
    const uint32_t save = guest::LoadU32(h.base, 0x826C5B58);
    if (save != 0) {
      guest::StoreU32(h.base, save + 6100, r.Below(2) == 0 ? 1 : r.Below(4));
    }
    const uint32_t screen = w.misc.address + 0x4100;
    guest::StoreU32(h.base, w.holder.address + 1024, screen);
    guest::StoreU32(h.base, kCareerResults, screen);
    const int32_t pages[3] = {-1, 0, 4};
    guest::StoreU32(h.base, screen + 184, uint32_t(pages[r.Below(3)]));
  };
  struct G {
    const char* name;
    int cases;
    const HandlerSetupFn* setup;
    GuestFunction recompiled, native;
    HandlerSpec spec;
    bool draws;
    // Whether f1 is compared, by the kind and the replay bit: only where the
    // handler's last call is to a callee whose own harness compares it
    // (RunCountdown, the alternate HUD's Tick and SetUp), or it calls none.
    bool (*f1)(int32_t kind, bool replay);
  };
  using F1 = bool (*)(int32_t, bool);
  const F1 hud_last = [](int32_t k, bool) { return !(k == 1 || k == 2 || k == 3 || k == 6); };
  const F1 countdown_draw = [](int32_t k, bool) {
    return !(k == 1 || k == 2 || k == 3 || k == 5 || k == 6);
  };
  const F1 never = [](int32_t, bool) { return false; };
  const HandlerSpec f96{96, 16};
  const HandlerSpec f112{112, 24};
  const G groups[] = {
      {"RaceHudMachine::PreRaceUpdate (sub_8216C210)", 80, &by_signed, __imp__sub_8216C210,
       hooks::RaceHudMachinePreRaceUpdate, f96, false, hud_last},
      {"RaceHudMachine::PreRaceDraw (sub_8216C2A0)", 80, &by_draw, __imp__sub_8216C2A0,
       hooks::RaceHudMachinePreRaceDraw, f96, true, hud_last},
      {"RaceHudMachine::CountdownDraw (sub_8216C330)", 80, &by_draw, __imp__sub_8216C330,
       hooks::RaceHudMachineCountdownDraw, f96, true, countdown_draw},
      {"RaceHudMachine::StartIntro (sub_8216C510)", 30, &by_signed, __imp__sub_8216C510,
       hooks::RaceHudMachineStartIntro, HandlerSpec{}, false, never},
      {"RaceHudMachine::EndIntro (sub_8216C520)", 30, &by_signed, __imp__sub_8216C520,
       hooks::RaceHudMachineEndIntro, HandlerSpec{}, false, never},
      {"RaceHudMachine::IntroUpdate (sub_8216C530)", 80, &by_signed, __imp__sub_8216C530,
       hooks::RaceHudMachineIntroUpdate, f96, false, hud_last},
      {"RaceHudMachine::IntroDraw (sub_8216C5B8)", 80, &by_draw, __imp__sub_8216C5B8,
       hooks::RaceHudMachineIntroDraw, f96, true, hud_last},
      {"RaceHudMachine::CountdownUpdate (sub_8216CB98)", 80, &by_signed, __imp__sub_8216CB98,
       hooks::RaceHudMachineCountdownUpdate, f112, false, hud_last},
      {"RaceHudMachine::RaceDraw (sub_8216CE00)", 80, &by_draw, __imp__sub_8216CE00,
       hooks::RaceHudMachineRaceDraw, f96, true, countdown_draw},
      {"RaceHudMachine::RaceUpdate (sub_8216D918)", 100, &by_signed, __imp__sub_8216D918,
       hooks::RaceHudMachineRaceUpdate, f112, false,
       [](int32_t k, bool replay) { return !((k == 1 || k == 2 || k == 3 || k == 6) && !replay); }},
      // __savegprlr_24: r24 to r31 and lr in the 80 bytes below sp.
      {"RaceHudMachine::SetUp (sub_8216D450)", 100, &set_up, __imp__sub_8216D450,
       hooks::RaceHudMachineSetUp, HandlerSpec{160, 80, true}, false, hud_last},
      // MenuList::Release's frame (below the handler's) is its own harness's.
      {"RaceHudMachine::ResultsUpdate (sub_8216DFD0)", 120, &results_update,
       __imp__sub_8216DFD0, hooks::RaceHudMachineResultsUpdate, HandlerSpec{144, 56}, false,
       never},
      {"RaceHudMachine::CloseResults (sub_8216D198)", 80, &close_results, __imp__sub_8216D198,
       hooks::RaceHudMachineCloseResults, f96, false, never},
  };
  for (const G& g : groups) {
    Results results(g.name);
    for (int c = 0; c < g.cases; c++) {
      HandlerCase(h, results, w, h.random.Next(), *g.setup, g.recompiled, g.native, g.spec,
                  g.draws, indices, g.f1, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The helpers: IsFirstDay by the race data's kind (0 to 2, now and then
// any word) and player 0's day (0, 1, 2, 999, 1,000, -1 or any word);
// OpponentPrize and ItemBonus on ids -1 and 370 to 409 with the items'
// bytes +35 (-2 to 7, or any byte) and +37 and the save data's records'
// halfwords +0, +2 (1 half the time) and +4 random.
void Helpers(Context& h, const World& w) {
  auto prizes = [&h](PPCContext& t, Random& r, const World& w) {
    const uint32_t id = r.Below(12) == 0 ? 0xFFFFFFFFu : kFirstItem + r.Below(kItems);
    t.r4.u64 = r.Below(8) == 0 ? (uint64_t(r.Next()) << 32) | id : id;
    for (uint32_t i = 0; i < kItems; i++) {
      const uint32_t item = w.items.address + 3008 * i;
      guest::StoreU8(h.base, item + 35,
                     r.Below(8) == 0 ? uint8_t(r.Next()) : uint8_t(int8_t(r.Below(10)) - 2));
      guest::StoreU8(h.base, item + 37, uint8_t(r.Next()));
      const uint32_t record = w.save.address + 0x199F8 + 12 * (kFirstItem + i);
      guest::StoreU16(h.base, record, r.Below(2) == 0 ? 0 : uint16_t(r.Next()));
      guest::StoreU16(h.base, record + 2, r.Below(2) == 0 ? 1 : uint16_t(r.Below(4)));
      guest::StoreU16(h.base, record + 4, r.Below(2) == 0 ? 0 : uint16_t(r.Next()));
    }
  };
  Group(h, w, "RaceHudMachine::OpponentPrize (sub_8216C820)", 80, prizes, __imp__sub_8216C820,
        hooks::RaceHudMachineOpponentPrize, Spec{112, 24});
  Group(h, w, "RaceHudMachine::ItemBonus (sub_8216C938, with sub_8216C9B8)", 80, prizes,
        __imp__sub_8216C938, hooks::RaceHudMachineItemBonus, Spec{96, 16});
  auto first_day = [&h](PPCContext&, Random& r, const World& w) {
    guest::StoreU32(h.base, w.race.address + 0xA3AF0,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3));
    static constexpr uint32_t kDays[] = {0, 1, 2, 999, 1000, 0xFFFFFFFFu};
    guest::StoreU32(h.base, w.save.address + 6100,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : kDays[r.Below(6)]);
  };
  Group(h, w, "RaceHudMachine::IsFirstDay (sub_8216D3F0)", 40, first_day, __imp__sub_8216D3F0,
        hooks::RaceHudMachineIsFirstDay, Spec{96, 8});
}

// --- RaceHudMachine::Init: an empty state machine or one with random
// transitions already (fewer than 130, so the 65 fit), some with the keys
// Init adds (the last, (24, 50), among them: its r3 is then the index); the
// original's frame filled with the same random words for both (the words it
// never writes go into the transitions); the machine, the frame (but its
// back chain and saved registers) and r3 compared ---

void Inits(Context& h, const World& w) {
  namespace gm = game_modes;
  constexpr uint32_t kSaves = 144;  // __savegprlr_15: r15 at -144, lr at -8
  Results results("RaceHudMachine::Init (sub_8216E6E8)");
  const uint32_t machine = w.machine.address;
  const uint32_t frame = CallContext(h).r1.u32 - rhm::kInitFrame;
  constexpr uint32_t kPlanted[][2] = {{0, 0}, {9, 29}, {22, 49}, {24, 50}};
  for (uint64_t seed = 0; seed < 8; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 157 + 41);
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, machine, kMachineBytes, r);
      WriteRandom(h.base, frame, rhm::kInitFrame, r);
      if (seed % 2 == 0) {
        gm::StateMachineConstruct(h.base, machine);
        guest::StoreU32(h.base, machine + gm::kTransitionCount, 0);
      } else {
        const uint32_t count = 8 + r.Below(120);
        guest::StoreU32(h.base, machine + gm::kTransitionCount, count);
        for (uint32_t i = 0; i < 200; i++) {  // keys of small numbers, some matching
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(h.base, machine + 48 * i + 4 * k, r.Below(k < 2 ? 60 : 2));
          }
        }
        for (const auto& key : kPlanted) {
          if (seed % 4 == 1 && key[0] == 24) {
            continue;  // seeds 1 and 5: the last Add makes a new transition
          }
          const uint32_t at = machine + 48 * r.Below(count);
          guest::StoreU32(h.base, at, key[0]);
          guest::StoreU32(h.base, at + 4, key[1]);
          guest::StoreU32(h.base, at + 8, 0);
          guest::StoreU32(h.base, at + 12, 0);
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = machine;
      if (v == 0) {
        ITC_FLAG(native_race_hud_machine) = false;
        __imp__sub_8216E6E8(t, h.base);
        ITC_FLAG(native_race_hud_machine) = true;
      } else {
        hooks::RaceHudMachineInit(t, h.base);
      }
      out[v].Bytes(Read(h.base, machine, kMachineBytes));
      out[v].Bytes(Read(h.base, frame + 80, rhm::kInitFrame - 80 - kSaves));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

}  // namespace

void RaceHudMachine(Context& h) {
  const World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_hud_machine: no blocks from the allocation layer");
    return;
  }
  Inits(h, w);
  Handlers(h);
  Helpers(h, w);
}

}  // namespace harness
