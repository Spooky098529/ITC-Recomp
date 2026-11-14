// The type 8 mode (src/type8_mode/, docs/functions/type8-mode.md) against
// the recompiled functions: the originals run with --native_type8_mode off,
// the plain C++ through the register steps (hooks/type8_mode.h). The
// dialog's cases are laid in type67_modes.cpp (Type8Dialogs), over the type
// 6 and 7 dialogs' worlds.
//
// The first screen's cases (Type8Screen0s), laid as the type 8 panel
// screen's (type8_screens.cpp): a world block of random words holding the
// screen (r3 at +0x100, now and then with its high half set) and the
// dispatcher (+0x6000; its ring's count and start small), the UI movie
// world (movies 16 and 24 not in the movie table: movie 0 of the world,
// UiMovies::Find's answer for a missing id; UiMovie::Run's update and
// ApplyAnimation stood in for and recorded), the string table of letters.
// The screen fades' words, the asking player, the mode before, the missing
// id's answer and the Draw's page table with its guard are saved, shaped by
// the case, compared and put back. UiMovies::Open and Delete are recorded
// (their adapters hand the original's calls to hooks::g_start_up_recorder;
// the plain C++'s open and remove record the same words). The menu list's
// item block (+9680 +68) is 0 or the allocation layer's pages where the
// function frees or makes it: a new block's words compared and freed, an
// old one compared by whether it's still pages. Compared exactly (the
// cursor's float is the original's conversion, compared as bits): the
// world, the movies and texts, the globals, the recorded calls, the
// function's own frame (its back chain and the words from +80 up, its
// register saves left out), the caller's area, r1 and r3 (both halves). The
// deleting destructor with bit 0 set runs on a block of operator new's,
// compared by r3 and whether the block is handed out again.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/loading_world.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/type67_screens.h"
#include "hooks/type8_mode.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
#include "strings_and_font/strings_and_font.h"
#include "type8_mode/type8_mode.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type8_mode);

namespace harness {

void Type8Dialogs(Context& h);

namespace {

namespace lw = loading_world;
namespace t8m = type8_mode;

constexpr uint32_t kWorldBytes = 0x8000;
constexpr uint32_t kObjectAt = 0x100;
constexpr uint32_t kDispatcherAt = 0x6000;
constexpr uint32_t kStackBytes = 0x400;   // below the call's r1
constexpr uint32_t kCallerBytes = 0x100;  // above it
constexpr uint32_t kOpen = 0x8211E808, kDelete = 0x8211E258;
constexpr uint32_t kFadeCountA = 0x826A50E4, kFadeCountB = 0x826A50DC;
constexpr uint32_t kModeBefore = 0x826032E0;
constexpr uint32_t kPageTable = 0x82814E94, kPageGuard = 0x82814EB4;
constexpr uint32_t kList = 9680;

const std::vector<std::pair<uint32_t, uint32_t>> kGlobals = {
    {0x826A50AC, 0x40},  // the screen fades' words (the counts among them)
    {t8m::kDialogPlayer, 4},
    {kModeBefore, 4},
    {0x826C5CDC, 4},  // UiMovies::Find's answer for a missing id
    {kPageTable, 0x24},  // Type8Screen0::Draw's table and its guard
    {0x82814E70, 0x24},  // Type8Screen0::Update's step table and its guard
    {race_screens::kMapperEntries, 32},
    {race_screens::kControllerEntries, 16},
    {menu_sounds::kSoundManager, 4},
    {0x826D83E0, 8},     // the stand's holder and the reload count
    {0x826C2E40, 8},     // the car scene's first word and the scene
    {0x82814EB8, 3944},  // the two players' garage cars
    {0x826C2E48, 4},     // the parameters (Params::GetCarSpec)
};

uint64_t Seed(uint64_t z) {
  z += 0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return z ^ (z >> 31);
}

const lw::MovieWorld* g_movies = nullptr;
struct Recorded {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<Recorded> g_recorded;
Random g_answers;
bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (function != 0x8211D2F8 && function != 0x825A08E0) {
    return false;
  }
  g_recorded.push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = function == 0x8211D2F8 ? g_answers.Below(2) : g_answers.Next();
  return true;
}

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// UiMovies::Open and Delete recorded: (address, id, asset for Open, r1, lr).
std::vector<uint32_t> g_records;
bool Record(PPCContext& ctx, uint32_t address) {
  if (address == kOpen) {
    g_records.insert(g_records.end(),
                     {kOpen, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, uint32_t(ctx.lr)});
    ctx.r3.u64 = 1;
    return true;
  }
  if (address == kDelete) {
    g_records.insert(g_records.end(), {kDelete, ctx.r3.u32, ctx.r1.u32, uint32_t(ctx.lr)});
    ctx.r3.u64 = 0x5A5A;
    return true;
  }
  return false;
}

t8m::ScreenCalls RecordingCalls(PPCContext& t, uint8_t* base) {
  t8m::ScreenCalls calls = hooks::Type67ScreensCalls(t, base);
  calls.open = [](uint32_t id, uint32_t asset, uint32_t sp, uint32_t lr) -> uint64_t {
    g_records.insert(g_records.end(), {kOpen, id, asset, sp, lr});
    return 1;
  };
  calls.remove = [](uint32_t id, uint32_t sp, uint32_t lr) -> uint64_t {
    g_records.insert(g_records.end(), {kDelete, id, sp, lr});
    return 0x5A5A;
  };
  return calls;
}

// A function's frame: its size and the register saves at its top (0 for a
// leaf or a tail call: the stack below compared whole unless `below` is
// false, for a tail call into a function saving registers).
struct Frame {
  uint32_t size, saves;
  bool below = true;
};

using Setup = std::function<void(PPCContext&, Random&, uint32_t world)>;
using Native = std::function<void(PPCContext&, uint8_t*)>;
using After = std::function<void(Outputs&)>;

void Group(Context& h, uint32_t world, const char* name, uint32_t address, int cases,
           Frame frame, const Setup& setup, GuestFunction recompiled, const Native& native,
           const After& after = {}) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{} (sub_{:08X})", name, address));
  for (int c = 0; c < cases; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed((uint64_t(address) << 16) ^ uint64_t(c)));
      std::vector<std::vector<uint8_t>> saved;
      for (const auto& [at, bytes] : kGlobals) saved.push_back(Read(base, at, bytes));
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      WriteRandom(base, world, kWorldBytes, r);
      for (const auto& [at, bytes] : kGlobals) WriteRandom(base, at, bytes, r);
      lw::ShapeMovieWorld(base, *g_movies, r);
      for (uint32_t k = 0; k < lw::kLeaves; k++) {
        guest::StoreU32(base, g_movies->leaf(k) + 108, 1 + r.Below(7));
      }
      guest::StoreU32(base, 0x826C5CDC, g_movies->movie(0));
      for (const uint32_t at : {kFadeCountA, kFadeCountB}) {
        guest::StoreU32(base, at, r.Below(2) == 0 ? 0 : r.Below(4));
      }
      // The Draw's table not built yet, or built as the original builds it.
      guest::StoreU32(base, kPageGuard, r.Below(2) == 0 ? 0 : uint32_t(r.Next()) | 1);
      if ((guest::LoadU32(base, kPageGuard) & 1) != 0) {
        const uint32_t table[8] = {0x8223C6B8, 0, 0x8223B5C0, 0, 0x82106540, 0, 0x8223C0C0, 0};
        for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, kPageTable + 4 * i, table[i]);
      }
      guest::StoreU32(base, kModeBefore, r.Below(3));
      guest::StoreU32(base, world + kDispatcherAt + 204 + 600, r.Below(50));
      guest::StoreU32(base, world + kDispatcherAt + 204 + 604, r.Below(60));
      t.r3.u64 = (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0) | (world + kObjectAt);
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      setup(t, r, world);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_records.clear();
      g_recorded.clear();
      g_answers = Random(r.Next());
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = StandIn;
      if (v == 0) {
        const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
        hooks::g_start_up_recorder = Record;
        ITC_FLAG(native_type8_mode) = false;
        recompiled(t, base);
        ITC_FLAG(native_type8_mode) = true;
        hooks::g_start_up_recorder = recorder;
      } else {
        native(t, base);
      }
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      if (after) after(out[v]);
      out[v].Word(uint32_t(g_records.size()));
      for (const uint32_t word : g_records) out[v].Word(word);
      out[v].Word(uint32_t(g_recorded.size()));
      for (const Recorded& call : g_recorded) {
        for (const uint32_t word : {call.function, call.lr, call.r1, uint32_t(call.r3),
                                    uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                    uint32_t(call.r4 >> 32)}) {
          out[v].Word(word);
        }
      }
      out[v].Bytes(Read(base, g_movies->movies.address, g_movies->movies.bytes));
      out[v].Bytes(Read(base, g_movies->texts.address, g_movies->texts.bytes));
      out[v].Bytes(Read(base, world, kWorldBytes));
      for (const auto& [at, bytes] : kGlobals) out[v].Bytes(Read(base, at, bytes));
      if (frame.size != 0) {
        const uint32_t bottom = sp - frame.size;
        out[v].Bytes(Read(base, bottom, 4));
        out[v].Bytes(Read(base, bottom + 80, frame.size - 80 - frame.saves));
      } else if (frame.below) {
        out[v].Bytes(Read(base, sp - kStackBytes, kStackBytes));
      }
      out[v].Bytes(Read(base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].first, saved[i].data(), kGlobals[i].second);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The menu list's item block (+9680 +68): 0 or the layer's pages (a new
// block on each side, the same words).
struct ItemBlock {
  uint32_t list, old;
};
std::vector<ItemBlock> g_items;
void LayItems(uint8_t* base, uint32_t screen, Random& r) {
  const uint32_t list = screen + kList;
  uint32_t block = 0;
  if (r.Below(3) != 0) {
    block = memory::AllocPages(64, 4096);
    WriteRandom(base, block, 64, r);
  }
  guest::StoreU32(base, list + 68, block);
  g_items.push_back({list, block});
}
void ItemOutputs(uint8_t* base, Outputs& out) {
  for (const ItemBlock& item : g_items) {
    const uint32_t now = guest::LoadU32(base, item.list + 68);
    out.Word(now == item.old ? 1 : 0);
    out.Word(now == 0 ? 0 : 1);
    if (now != 0 && now != item.old && memory::IsPages(now)) {
      out.Bytes(Read(base, now, 4 * 5));
      memory::FreePages(now);
      guest::StoreU32(base, item.list + 68, 1);
    }
  }
  for (const ItemBlock& item : g_items) {
    if (item.old == 0) continue;
    const bool still = memory::IsPages(item.old);
    out.Word(still ? 1 : 0);
    if (still) memory::FreePages(item.old);
  }
  g_items.clear();
}

void Deleting(Context& h, const char* name, uint32_t address, uint32_t bytes,
              GuestFunction recompiled, GuestFunction native) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{}, deleting (sub_{:08X})", name, address));
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed(uint64_t(address) << 20 ^ uint64_t(c)));
      const auto object = memory::crt::OperatorNew(base, bytes);
      if (!object) {
        ITC_LOG_ERROR("harness: type8_mode: operator new failed");
        return;
      }
      WriteRandom(base, *object, bytes, r);
      guest::StoreU32(base, *object + kList + 68, 0);
      PPCContext t = CallContext(h);
      t.r3.u64 = *object;
      t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
      t.lr = 0x82000000 + 4 * r.Below(1024);
      if (v == 0) {
        ITC_FLAG(native_type8_mode) = false;
        recompiled(t, base);
        ITC_FLAG(native_type8_mode) = true;
      } else {
        native(t, base);
      }
      out[v].Word(t.r3.u32 == *object ? 1 : 0);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      const auto probe = memory::crt::OperatorNew(base, bytes);
      out[v].Word(probe && *probe == *object ? 1 : 0);
      if (probe) memory::crt::OperatorDelete(*probe);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void NoSetup(PPCContext&, Random&, uint32_t) {}

Native WithRecording(uint64_t (*step)(uint8_t*, uint64_t, uint32_t, const t8m::ScreenCalls&)) {
  return [step](PPCContext& t, uint8_t* b) {
    const t8m::ScreenCalls c = RecordingCalls(t, b);
    t.r3.u64 = step(b, t.r3.u64, t.r1.u32, c);
  };
}

// --- The menu page, the steps, Update, UpdateStand and Show ---
//
// The menu list (+9680) laid as the type 8 list part's harness lays its
// menus (five items, its player 0 to 3, its actions mostly the menus', the
// move word 0 mostly, its item block at the world's +0x2800), the four
// players' action mappers (+0x2900 their vtables, +0x2A00 the mappers) and a
// fake sound manager (+0x3200; PlayCue2D the recorder sub_820A8420, StopCue
// sub_820CE9F8, IsCuePlaying the getter sub_82090770 answering its +60, 0
// or 1). The stand in a block of its own as Type6Screen0::Update's cases lay
// it (type67_modes.cpp), with three stages (slots 1 and 2 the players'):
// their garage cameras not set up (GarageStage::Draw does nothing, Command's
// presets kept at +6088), +4 -1 or not, +8 0, the car scene's one pool 7
// model, the reload count 0 to 2 (GarageStage::ReloadModel answers 0: its
// later branch, the car object and GarageStage::SetCar, isn't laid, as
// there). The garage cars' model ids below 80 (Params::GetCarSpec's table),
// the stand blocks' named animations below 256 (the movie world's). The
// stand block compared after each case.

constexpr uint32_t kItemsAt = 0x2800, kMapperVtablesAt = 0x2900, kMappersAt = 0x2A00;
constexpr uint32_t kManagerAt = 0x3200, kManagerVtableAt = 0x3280;
constexpr uint32_t kStandBytes = 0x8000;
constexpr uint32_t kStepTable = 0x82814E70, kStepGuard = 0x82814E90;
constexpr uint32_t kGarageCars = 0x82814EB8;

uint32_t g_stand = 0;

void LayMenuWorld(uint8_t* base, uint32_t w, Random& r) {
  namespace rs = race_screens;
  const uint32_t list = w + kObjectAt + kList;
  guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
  guest::StoreU32(base, list + rs::kListCount, 5);
  guest::StoreU32(base, list + rs::kListShown, 5);
  guest::StoreU32(base, list + rs::kListScroll, r.Below(4) == 0 ? 0xFFFFFFFFu : 0);
  guest::StoreU32(base, list + rs::kListSelection, r.Below(5));
  guest::StoreU32(base, list + rs::kListSelectionBefore, r.Below(5));
  for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
    guest::StoreU32(base, list + field,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30));
  }
  guest::StoreU32(base, list + rs::kListWraps, r.Below(2));
  guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  guest::StoreU32(base, list + rs::kListBlock, w + kItemsAt);
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t vtable = w + kMapperVtablesAt + 0x40 * c;
    const uint32_t mapper = w + kMappersAt + 0x200 * c;
    guest::StoreU32(base, rs::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, rs::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, rs::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + 40, rs::kPressedSlot);
    guest::StoreU32(base, vtable + 48, rs::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
}

void LaySoundManager(uint8_t* base, uint32_t w, Random& r) {
  const uint32_t manager = w + kManagerAt, vtable = w + kManagerVtableAt;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, manager + 60, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
  guest::StoreU32(base, vtable + 52, 0x820CE9F8);
  guest::StoreU32(base, vtable + 56, 0x82090770);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

// The page block (+9652 to +9676): small pages and steps.
void LayPages(uint8_t* base, uint32_t screen, Random& r) {
  const auto small = [&r](uint32_t n) { return r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(n); };
  guest::StoreU32(base, screen + 9644, r.Below(3) == 0 ? 0 : 1);
  guest::StoreU32(base, screen + 9652, small(4));
  guest::StoreU32(base, screen + 9656, small(4));
  guest::StoreU32(base, screen + 9664, small(4));
  guest::StoreU32(base, screen + 9668, small(4));
  guest::StoreU32(base, screen + 9676, r.Below(2));
}

void LayCars(uint8_t* base, Random& r) {
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t car = kGarageCars + 1972 * p;
    guest::StoreU32(base, car + 12, r.Below(80));
  }
}

// The two players' stand blocks (+9752, +9772): the shown word, the named
// animations, the model made.
void LayStandBlocks(uint8_t* base, uint32_t screen, Random& r) {
  for (uint32_t p = 0; p < 2; p++) {
    const uint32_t b = screen + 9752 + 20 * p;
    guest::StoreU32(base, b + 0, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
    for (const uint32_t at : {4u, 8u, 12u}) guest::StoreU32(base, b + at, r.Below(256));
    guest::StoreU32(base, b + 16, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
  }
}

void LayStand(uint8_t* base, Random& r) {
  constexpr uint32_t kSceneAt = 0x4C00, kModel = 0x6000;
  const uint32_t b = g_stand;
  WriteRandom(base, b, kStandBytes, r);
  const uint32_t scene = b + kSceneAt, model = b + kModel;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = b + 0x40 + 4 * i;
    const uint32_t stage = b + 0x100 + 0x1900 * i;
    guest::StoreU32(base, b + 4 * i, slot);
    guest::StoreU32(base, slot, stage);
    guest::StoreU32(base, stage + 4, r.Below(2) ? 0xFFFFFFFFu : uint32_t(r.Next()));
    guest::StoreU32(base, stage + 8, 0);
    guest::StoreU32(base, stage + 16, r.Below(2));
    guest::StoreU32(base, stage + 112, 0);
  }
  guest::StoreU32(base, 0x826D83E0, b);
  guest::StoreU32(base, 0x826C2E44, scene);
  guest::StoreU32(base, 0x826C2E40, 0);
  guest::StoreU32(base, 0x826D83E4, r.Below(3));
  guest::StoreU32(base, scene + 0, r.Below(2) == 0 ? 4 : r.Below(4));
  guest::StoreU32(base, scene + 48, 3 + r.Below(4));
  guest::StoreU32(base, scene + 4, 0);
  guest::StoreU32(base, scene + 5048, 1);
  guest::StoreU32(base, scene + 5028, model);
  guest::StoreU32(base, scene + 5036, 1);
  guest::StoreU32(base, model + 8, 0);
  guest::StoreU32(base, model + 16, 3);
  guest::StoreU32(base, model + 20, r.Below(6));
  guest::StoreU32(base, model + 32, 1);
  guest::StoreU32(base, model + 5028, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, model + 5040, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, model + 5044, 3 + r.Below(3));
  for (const uint32_t word : {5048u, 5060u, 5068u, 5076u, 5084u, 5100u}) {
    guest::StoreU32(base, model + word, 0);
  }
  guest::StoreU32(base, model + 5116, scene);
}

void StandOutputs(uint8_t* base, Outputs& out) { out.Bytes(Read(base, g_stand, kStandBytes)); }

// Update's step table not built yet, or built as the original builds it.
void LaySteps(uint8_t* base, Random& r) {
  guest::StoreU32(base, kStepGuard, r.Below(2) == 0 ? 0 : uint32_t(r.Next()) | 1);
  if ((guest::LoadU32(base, kStepGuard) & 1) != 0) {
    const uint32_t table[8] = {0x8223C1A0, 0, 0x8223BBC8, 0, 0x8223C330, 0, 0x8223BAF0, 0};
    for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, kStepTable + 4 * i, table[i]);
  }
}

void Type8Screen0Updates(Context& h, uint32_t world) {
  uint8_t* const base = h.base;
  g_stand = memory::Alloc(kStandBytes, 0);
  if (g_stand == 0) {
    ITC_LOG_ERROR("harness: type8_mode: no stand block");
    return;
  }
  const auto menu = [base](PPCContext& t, Random& r, uint32_t w) {
    LayMenuWorld(base, w, r);
    LaySoundManager(base, w, r);
    LayPages(base, w + kObjectAt, r);
    t.r4.u64 = w + kDispatcherAt;
  };
  Group(h, world, "Type8Screen0::MenuPage", 0x8223C6B8, 12, {144, 48}, menu, __imp__sub_8223C6B8,
        hooks::Type8Screen0MenuPage);
  const auto steps = [base](PPCContext&, Random& r, uint32_t w) {
    LayMenuWorld(base, w, r);
    LayPages(base, w + kObjectAt, r);
  };
  Group(h, world, "Type8Screen0::OpeningStep", 0x8223C1A0, 8, {128, 32}, steps,
        __imp__sub_8223C1A0, hooks::Type8Screen0OpeningStep);
  Group(h, world, "Type8Screen0::ItemStep", 0x8223BBC8, 6, {128, 32}, steps, __imp__sub_8223BBC8,
        hooks::Type8Screen0ItemStep);
  Group(h, world, "Type8Screen0::CursorStep", 0x8223C330, 8, {128, 40}, steps,
        __imp__sub_8223C330, hooks::Type8Screen0CursorStep);
  Group(h, world, "Type8Screen0::FadeOutStep", 0x8223BAF0, 8, {112, 24}, steps,
        __imp__sub_8223BAF0, hooks::Type8Screen0FadeOutStep);
  const auto stands = [base](PPCContext&, Random& r, uint32_t w) {
    LayCars(base, r);
    LayStandBlocks(base, w + kObjectAt, r);
    LayStand(base, r);
  };
  const auto stand_outputs = [base](Outputs& out) { StandOutputs(base, out); };
  Group(h, world, "Type8Screen0::UpdateStand", 0x8223B808, 10, {4096, 96}, stands,
        __imp__sub_8223B808, hooks::Type8Screen0UpdateStand, stand_outputs);
  Group(
      h, world, "Type8Screen0::Update", 0x8223C588, 12, {160, 40},
      [base, steps, stands](PPCContext& t, Random& r, uint32_t w) {
        steps(t, r, w);
        LaySteps(base, r);
        stands(t, r, w);
      },
      __imp__sub_8223C588, hooks::Type8Screen0Update, stand_outputs);
  Group(h, world, "Type8Screen0::SetTexts", 0x8223B760, 4, {160, 32}, NoSetup,
        __imp__sub_8223B760, hooks::Type8Screen0SetTexts);
  Group(
      h, world, "Type8Screen0::CarCard", 0x8223BC68, 6, {144, 40},
      [](PPCContext& t, Random& r, uint32_t) {
        t.r4.u64 = r.Below(2);
        t.r5.u64 = r.Below(80);
        t.r6.u64 = r.Below(256);
      },
      __imp__sub_8223BC68, hooks::Type8Screen0CarCard);
  Group(
      h, world, "Type8Screen0::Show", 0x8223BF18, 8, {176, 64},
      [base](PPCContext&, Random& r, uint32_t w) {
        LaySoundManager(base, w, r);
        // The parameters in the stand block: the specs from +0x100, the
        // cars' ids 0 to 2 (each spec 8,432 bytes of random words).
        WriteRandom(base, g_stand, kStandBytes, r);
        guest::StoreU32(base, 0x826C2E48, g_stand);
        guest::StoreU32(base, g_stand + 12, g_stand + 0x100);
        for (uint32_t p = 0; p < 2; p++) {
          guest::StoreU32(base, kGarageCars + 1972 * p + 12, r.Below(3));
        }
        LayStandBlocks(base, w + kObjectAt, r);
        guest::StoreU32(base, w + kObjectAt + 9652, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(4));
      },
      __imp__sub_8223BF18, hooks::Type8Screen0Show);
  memory::Free(g_stand);
  g_stand = 0;
}

void Type8Screen0s(Context& h, uint32_t world) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {16u, 24u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != 0x826C5CDC) {
      ITC_LOG_ERROR("harness: type8_mode: movie {} is in the table; the first screen skipped",
                    id);
      return;
    }
  }
  const lw::MovieWorld w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: type8_mode: no movie world");
    return;
  }
  const lw::SavedGlobals saved(base);
  lw::BuildStringTable(base, w);
  g_movies = &w;
  const auto items = [base](PPCContext&, Random& r, uint32_t w) {
    LayItems(base, w + kObjectAt, r);
  };
  const auto item_outputs = [base](Outputs& out) { ItemOutputs(base, out); };

  Group(h, world, "Type8Screen0::Type8Screen0", 0x8223D2F0, 4, {96, 16}, NoSetup,
        __imp__sub_8223D2F0, hooks::Type8Screen0Construct);
  Group(h, world, "Type8Screen0::~Type8Screen0", 0x8223BF00, 6, {0, 0, false}, items,
        __imp__sub_8223BF00, hooks::Type8Screen0Destruct, item_outputs);
  Group(
      h, world, "Type8Screen0::DeletingDestructor", 0x8223C528, 6, {112, 24},
      [base](PPCContext& t, Random& r, uint32_t w) {
        LayItems(base, w + kObjectAt, r);
        t.r4.u64 = r.Next() & ~1ull;
      },
      __imp__sub_8223C528, hooks::Type8Screen0DeletingDestruct, item_outputs);
  Deleting(h, "Type8Screen0::DeletingDestructor", 0x8223C528, 9800, __imp__sub_8223C528,
           hooks::Type8Screen0DeletingDestruct);
  Group(
      h, world, "Type8Screen0::OnEnter", 0x8223B630, 8, {144, 40},
      [base](PPCContext& t, Random& r, uint32_t w) {
        LayItems(base, w + kObjectAt, r);
        guest::StoreU32(base, w + kObjectAt + 9616, r.Below(2));
        (void)t;
      },
      __imp__sub_8223B630, hooks::Type8Screen0OnEnter, item_outputs);
  Group(h, world, "Type8Screen0::OpenMovie", 0x8223B530, 3, {0, 0}, NoSetup, __imp__sub_8223B530,
        [](PPCContext& t, uint8_t* b) {
          const t8m::ScreenCalls c = RecordingCalls(t, b);
          t.r3.u64 = t8m::Screen0OpenMovie(t.r1.u32, uint32_t(t.lr), c);
        });
  Group(
      h, world, "Type8Screen0::WaitForMovie", 0x8223B6D8, 6, {96, 16},
      [base](PPCContext&, Random& r, uint32_t w) {
        guest::StoreU32(base, w + kObjectAt + 9612, r.Below(2));
      },
      __imp__sub_8223B6D8, hooks::Type8Screen0WaitForMovie);
  Group(
      h, world, "Type8Screen0::Suspend", 0x8223B540, 6, {96, 16},
      [base](PPCContext&, Random& r, uint32_t w) {
        guest::StoreU32(base, w + kObjectAt + 9620, r.Below(2));
      },
      __imp__sub_8223B540, WithRecording(t8m::Screen0Suspend));
  Group(h, world, "Type8Screen0::PickYes", 0x8223B590, 3, {0, 0}, NoSetup, __imp__sub_8223B590,
        hooks::Type8Screen0PickYes);
  Group(h, world, "Type8Screen0::PickNo", 0x8223B5A0, 3, {0, 0}, NoSetup, __imp__sub_8223B5A0,
        hooks::Type8Screen0PickNo);
  Group(h, world, "Type8Screen0::PickBack", 0x8223B5B0, 3, {0, 0}, NoSetup, __imp__sub_8223B5B0,
        hooks::Type8Screen0PickBack);
  const auto flags = [](Random& r) -> uint64_t {
    switch (r.Below(4)) {
      case 0: return 0;
      case 1: return 1;
      case 2: return r.Next() & 0xFFFFFFFF00000000ull;
      default: return r.Next();
    }
  };
  Group(
      h, world, "Type8Screen0::ShowPage", 0x8223BD20, 10, {128, 32},
      [flags](PPCContext& t, Random& r, uint32_t) {
        t.r4.u64 = flags(r);
        t.r5.u64 = flags(r);
      },
      __imp__sub_8223BD20, hooks::Type8Screen0ShowPage);
  // A leaf with a tail call: ShowPage's frame compared.
  Group(
      h, world, "Type8Screen0::OnDialogClosed", 0x8223C098, 8, {128, 32},
      [base](PPCContext&, Random& r, uint32_t w) {
        guest::StoreU32(base, w + kObjectAt + 9648, r.Below(2) == 0 ? 1 : r.Below(4) - 1);
      },
      __imp__sub_8223C098, hooks::Type8Screen0OnDialogClosed);
  Group(
      h, world, "Type8Screen0::Leave", 0x8223BA48, 8, {112, 24},
      [base](PPCContext& t, Random& r, uint32_t w) {
        t.r4.u64 = w + kDispatcherAt;
        if (r.Below(4) == 0) guest::StoreU32(base, 0x826C5CDC, 0);
      },
      __imp__sub_8223BA48,
      [](PPCContext& t, uint8_t* b) {
        const t8m::ScreenCalls c = RecordingCalls(t, b);
        t.r3.u64 = t8m::Screen0Leave(b, t.r4.u32, t.r1.u32, c);
      });
  Group(h, world, "Type8Screen0::OnLeft", 0x8223B588, 6, {0, 0, false}, items,
        __imp__sub_8223B588, hooks::Type8Screen0OnLeft, item_outputs);
  Group(
      h, world, "Type8Screen0::Confirm", 0x8223C430, 8, {128, 32},
      [](PPCContext& t, Random&, uint32_t w) { t.r5.u64 = w + kDispatcherAt; },
      __imp__sub_8223C430, hooks::Type8Screen0Confirm);
  Group(
      h, world, "Type8Screen0::ConfirmPage", 0x8223B5C0, 10, {0, 0},
      [base](PPCContext&, Random& r, uint32_t w) {
        static constexpr uint32_t kAnswers[] = {0, 1, 2, 3, 0xFFFFFFFFu};
        guest::StoreU32(base, w + kObjectAt + 9648, kAnswers[r.Below(5)]);
        guest::StoreU32(base, w + kObjectAt + 9652, r.Below(5));
      },
      __imp__sub_8223B5C0, hooks::Type8Screen0ConfirmPage);
  const auto chosen = [base](PPCContext& t, Random& r, uint32_t w) {
    t.r4.u64 = w + kDispatcherAt;
    guest::StoreU32(base, w + kObjectAt + kList + 16,
                    r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(6));
  };
  Group(h, world, "Type8Screen0::ChosenPage", 0x8223C0C0, 10, {112, 24}, chosen,
        __imp__sub_8223C0C0, hooks::Type8Screen0ChosenPage);
  Group(
      h, world, "Type8Screen0::PlaceCursor", 0x8223BE88, 8, {112, 24},
      [](PPCContext& t, Random& r, uint32_t) {
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(5);
      },
      __imp__sub_8223BE88, hooks::Type8Screen0PlaceCursor);
  // Pages -1, 1 (ConfirmPage), 2 (the empty handler) and 3 (ChosenPage);
  // MenuPage (page 0) has no world here.
  Group(
      h, world, "Type8Screen0::Draw", 0x8223C888, 12, {128, 8},
      [base, chosen](PPCContext& t, Random& r, uint32_t w) {
        chosen(t, r, w);
        static constexpr uint32_t kPages[] = {0xFFFFFFFFu, 1, 2, 3};
        guest::StoreU32(base, w + kObjectAt + 9652, kPages[r.Below(4)]);
        static constexpr uint32_t kAnswers[] = {0, 1, 2, 0xFFFFFFFFu};
        guest::StoreU32(base, w + kObjectAt + 9648, kAnswers[r.Below(4)]);
      },
      __imp__sub_8223C888, hooks::Type8Screen0Draw);
  Type8Screen0Updates(h, world);
  g_movies = nullptr;
  saved.Restore();
}

// --- The mode's slots and +32 interface. The mode a block of its own
// (kModeBytes, random words): its dispatcher with no handlers (+236 0), its
// ring's count 0 to 3 and start below 200, the members' loaded words
// (AllLoaded's) nonzero or now and then 0, the machines NextMode reads in
// states 6, 11 or others. r3 the answer in the world (or the mode + 32 for
// the +32 slots), r4 the mode. The globals they use (the pad blocked words,
// the leaving flag, the two-player block, player 0's save data slot, the
// loading screen's, the string table, the movie table, the viewports and
// the accessories' set) saved, laid per case, compared and put back.
// WaitForLoads: the loading screen's movie a harness movie (its update
// recorded), the fake sound manager (slot 6 the getter of its +60), the
// stand as Type8Screen0::Update's with the two players' garage cameras set
// up now and then (one preset, the scene model's car 0) so
// GarageStage::Command applies presets 42 and 43. NextMode: the two-player
// block operator new's (240,512 bytes) or none, player 0's save data a
// block of the harness's. PollTeardown: the stand as StandScene::
// PollTeardown's cases lay it (type67_modes.cpp). TearDown: the string
// table a block of the case's (files 0 and 1 none, counts 2 to 4: nothing
// freed) and the movie table 0 to 3 keys of no movie (UiMovies::Find's
// answer for a missing id 0: no movie world movie deleted). Compared besides
// Group's outputs: the mode block, the stand, the globals.

constexpr uint32_t kModeBytes = 0x15800;
constexpr uint32_t kSaveBytes = 0x1FA00;
constexpr uint32_t kTwoPlayerBytes = 0x3AC00;
constexpr uint32_t kTwoPlayerBlock = 0x826C5B5C, kSaveSlots = 0x826C5B58;
constexpr uint32_t kLoadingCurrent = 0x82815E64, kLoadingMovies = 0x82815E68;
constexpr uint32_t kStringTable = 0x826C5C34, kAccessorySet = 0x826C5510;
constexpr uint32_t kViewports = 0x826C2CE8;
constexpr uint32_t kLoadedWords[] = {9612, 29156, 40524, 19400, 52352, 67600, 86880};

uint32_t g_mode = 0, g_save = 0, g_loading_movie = 0, g_strings = 0;

const std::vector<std::pair<uint32_t, uint32_t>> kModeGlobals = {
    {0x826C7960, 8},       // the pad blocked words
    {0x826364EC, 4},       // the leaving flag
    {kTwoPlayerBlock, 4},  {kSaveSlots, 4},       {kLoadingCurrent, 16},
    {kStringTable, 4},     {ui_movies::kMovieTable, 12}, {kViewports, 4},
    {kAccessorySet, 4},
};

void LayMode(uint8_t* base, Random& r) {
  WriteRandom(base, g_mode, kModeBytes, r);
  guest::StoreU32(base, g_mode + 236, 0);
  guest::StoreU32(base, g_mode + 840, r.Below(4));
  guest::StoreU32(base, g_mode + 844, r.Below(200));
  const bool all = r.Below(3) != 0;
  for (const uint32_t word : kLoadedWords) {
    guest::StoreU32(base, g_mode + 848 + word,
                    all || r.Below(4) != 0 ? 1 + r.Below(3) : 0);
  }
  guest::StoreU32(base, 0x826C7960, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, 0x826C7964, r.Below(4) == 0 ? 1 : 0);
}

// The stand's world as StandScene::PollTeardown's cases lay it
// (type67_modes.cpp's LayStand, its block the stand block here).
void LayTeardownStand(uint8_t* base, Random& r) {
  constexpr uint32_t kScene = 0x1000, kModels = 0x3000, kModelBytes = 0x1400;
  const uint32_t b = g_stand;
  WriteRandom(base, b, kStandBytes, r);
  const uint32_t holder = b, scene = b + kScene;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = b + 0x40 + 4 * i, stage = b + 0x100 + 0x100 * i;
    guest::StoreU32(base, holder + 4 * i, slot);
    guest::StoreU32(base, slot, stage);
    guest::StoreU32(base, stage + 8, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(3));
  }
  const uint32_t under_way = r.Below(2), made = r.Below(3) == 0 ? 0 : 1 + r.Below(5);
  guest::StoreU32(base, holder + 12, under_way);
  guest::StoreU32(base, holder + 16, uint32_t(int32_t(r.Below(5)) - 1));
  guest::StoreU32(base, holder + 20, made);
  bool loading = false;
  guest::StoreU32(base, scene + 0, 4);
  guest::StoreU32(base, scene + 48, 4 + r.Below(8));
  for (const uint32_t slot : {84u, 132u, 156u, 268u, 472u, 964u, 4156u, 4764u}) {
    const uint32_t state = r.Below(12) == 0 ? 2 : r.Below(2);
    loading |= state == 2;
    guest::StoreU32(base, scene + slot, state);
  }
  for (uint32_t pool = 0; pool < 7; pool++) {
    guest::StoreU32(base, scene + 4944 + 12 * pool + 8, 0);
  }
  guest::StoreU32(base, scene + 5028, b + kModels);
  guest::StoreU32(base, scene + 5036, 3);
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t model = b + kModels + kModelBytes * k;
    guest::StoreU32(base, model + 8, k < 2 ? model + kModelBytes : 0);
    guest::StoreU32(base, model + 16, 3);
    const uint32_t deferred = r.Below(3) == 0 ? 1 : 0;
    loading |= deferred != 0;
    guest::StoreU32(base, model + 5040, deferred);
  }
  const uint32_t set = b + 0x7000;
  for (uint32_t i = 0; i < 4; i++) guest::StoreU32(base, set + 4 * i, 0);
  guest::StoreU32(base, 0x826D83E0, holder);
  guest::StoreU32(base, 0x826C2E44, scene);
  const bool tear_down = under_way == 0 && (made == 0 || !loading);
  guest::StoreU32(base, 0x826C2E40, tear_down ? b + 0x7800 : 0);
  guest::StoreU32(base, kAccessorySet, set);
}

// The players' garage cameras (stages 1 and 2) set up or not: one preset
// (its entry 0 for presets 42 and 43 of the scene model's car 0).
void LayCameras(uint8_t* base, Random& r) {
  const uint32_t b = g_stand, table = b + 0x7400, preset = b + 0x7600;
  WriteRandom(base, preset, 64, r);
  for (uint32_t k = 0; k < 48; k++) guest::StoreU32(base, table + 4 * k, 0);
  guest::StoreU32(base, b + 0x6000 + 48, 0);  // the scene model's car
  const bool set_up = r.Below(4) != 0;
  for (uint32_t i = 1; i < 3; i++) {
    const uint32_t camera = b + 0x100 + 0x1900 * i + 112;
    guest::StoreU32(base, camera, set_up || r.Below(2) == 0 ? 1 : 0);
    guest::StoreU32(base, camera + 292, table);
    guest::StoreU32(base, camera + 4392, 1);
    guest::StoreU32(base, camera + 296, preset);
  }
}

void ModeOutputs(uint8_t* base, Outputs& out) {
  out.Bytes(Read(base, g_mode, kModeBytes));
  for (const auto& [at, bytes] : kModeGlobals) {
    if (at == kTwoPlayerBlock) {
      out.Word(guest::LoadU32(base, at) == 0 ? 0 : 1);
    } else if (at != ui_movies::kMovieTable) {
      out.Bytes(Read(base, at, bytes));
    }
  }
}

void Type8ModeSlots(Context& h, uint32_t world) {
  uint8_t* const base = h.base;
  g_stand = memory::Alloc(kStandBytes, 0);
  g_mode = memory::Alloc(kModeBytes, 0);
  g_save = memory::Alloc(kSaveBytes, 0);
  g_loading_movie = memory::Alloc(0xA100, 0);
  g_strings = memory::Alloc(strings_and_font::kStringsBytes, 0);
  const lw::MovieWorld w;
  if (g_stand == 0 || g_mode == 0 || g_save == 0 || g_loading_movie == 0 || g_strings == 0 ||
      !w.ok()) {
    ITC_LOG_ERROR("harness: type8_mode: no blocks for the mode's slots");
    return;
  }
  std::vector<std::vector<uint8_t>> saved;
  for (const auto& [at, bytes] : kModeGlobals) saved.push_back(Read(base, at, bytes));
  const lw::SavedGlobals saved_world(base);
  lw::BuildStringTable(base, w);
  g_movies = &w;
  const auto mode_outputs = [base](Outputs& out) { ModeOutputs(base, out); };
  const auto answer = [base](PPCContext& t, Random& r, uint32_t) {
    LayMode(base, r);
    t.r4.u64 = g_mode;
  };

  Group(
      h, world, "ModeType8::AllLoaded", 0x8223D3C0, 6, {0, 0},
      [base](PPCContext& t, Random& r, uint32_t) {
        LayMode(base, r);
        t.r3.u64 = (uint64_t(r.Below(2)) << 32) | (g_mode + 848);
      },
      __imp__sub_8223D3C0, hooks::Type8ModeAllLoaded, mode_outputs);
  Group(
      h, world, "ModeType8::Interrupt", 0x8223D960, 8, {96, 8},
      [base](PPCContext& t, Random& r, uint32_t) {
        LayMode(base, r);
        guest::StoreU32(base, g_mode + 87724, r.Below(2) == 0 ? 5 : r.Below(8));
        t.r3.u64 = g_mode + 32;
      },
      __imp__sub_8223D960, hooks::Type8ModeInterrupt, mode_outputs);
  Group(
      h, world, "ModeType8::NextMode", 0x8223DD90, 10, {144, 48},
      [base, answer](PPCContext& t, Random& r, uint32_t w) {
        answer(t, r, w);
        const uint32_t member = g_mode + 848;
        static constexpr uint32_t kStates[] = {6, 11, 5, 4};
        guest::StoreU32(base, member + 30916 + 9604, r.Below(4) == 0 ? 11 : kStates[r.Below(4)]);
        guest::StoreU32(base, member + 19548 + 9604, r.Below(4) == 0 ? 11 : kStates[r.Below(4)]);
        guest::StoreU32(base, member + 4 + 9604, r.Below(2) == 0 ? 6 : kStates[r.Below(4)]);
        WriteRandom(base, g_save + 0x1F85C, 284, r);
        guest::StoreU32(base, kSaveSlots, g_save);
        uint32_t block = 0;
        if (r.Below(5) != 0) {
          block = memory::crt::OperatorNew(base, kTwoPlayerBytes).value_or(0);
          if (block != 0) WriteRandom(base, block + 0x3AA58, 284, r);
        }
        guest::StoreU32(base, kTwoPlayerBlock, block);
      },
      __imp__sub_8223DD90, hooks::Type8ModeNextMode, [base](Outputs& out) {
        ModeOutputs(base, out);
        out.Bytes(Read(base, g_save + 0x1F85C, 284));
        const uint32_t block = guest::LoadU32(base, kTwoPlayerBlock);
        if (block != 0) {
          memory::crt::OperatorDelete(block);
          guest::StoreU32(base, kTwoPlayerBlock, 0);
        }
      });
  Group(
      h, world, "ModeType8::WaitForLoads", 0x8223DBC0, 12, {160, 56},
      [base, answer](PPCContext& t, Random& r, uint32_t w) {
        answer(t, r, w);
        LaySoundManager(base, w, r);
        guest::StoreU32(base, w + kManagerVtableAt + 24, 0x82090770);
        // Mostly loaded: both fades' counts 0, bank 1 not loading.
        if (r.Below(4) != 0) {
          guest::StoreU32(base, kFadeCountA, 0);
          guest::StoreU32(base, kFadeCountB, 0);
          guest::StoreU32(base, w + kManagerAt + 60, 0);
        }
        LayStand(base, r);
        LayCameras(base, r);
        guest::StoreU32(base, kLoadingCurrent, r.Below(3));
        WriteRandom(base, g_loading_movie, 0xA100, r);
        ui_movies::InitAnimationLists(base, g_loading_movie);
        for (uint32_t k = 0; k < 3; k++) {
          guest::StoreU32(base, kLoadingMovies + 4 * k, g_loading_movie);
        }
      },
      __imp__sub_8223DBC0, hooks::Type8ModeWaitForLoads, [base](Outputs& out) {
        ModeOutputs(base, out);
        StandOutputs(base, out);
        out.Bytes(Read(base, g_loading_movie, 0xA100));
      });
  Group(
      h, world, "ModeType8::PollTeardown", 0x8223D8E8, 8, {128, 32},
      [base, answer](PPCContext& t, Random& r, uint32_t w) {
        answer(t, r, w);
        LayTeardownStand(base, r);
      },
      __imp__sub_8223D8E8, hooks::Type8ModePollTeardown, [base](Outputs& out) {
        ModeOutputs(base, out);
        StandOutputs(base, out);
      });
  Group(
      h, world, "ModeType8::TearDown", 0x8223D4D0, 8, {160, 48},
      [base, answer](PPCContext& t, Random& r, uint32_t w) {
        answer(t, r, w);
        LaySoundManager(base, w, r);
        WriteRandom(base, g_strings, strings_and_font::kStringsBytes, r);
        for (uint32_t f = 0; f < 2; f++) {
          guest::StoreU32(base, g_strings + 4 * f, 0);
          guest::StoreU32(base, g_strings + 68 + 4 * f, r.Below(2));
          guest::StoreU32(base, g_strings + 4236 + 4 * f, 2 + r.Below(3));
        }
        guest::StoreU32(base, kStringTable, g_strings);
        const uint32_t capacity = 1 + r.Below(3), count = r.Below(capacity + 1);
        const uint32_t array = memory::crt::OperatorNewArray(base, 8 * capacity).value_or(0);
        for (uint32_t k = 0; k < capacity; k++) {
          guest::StoreU32(base, array + 8 * k, 100 + k);
          guest::StoreU32(base, array + 8 * k + 4, 0);
        }
        guest::StoreU32(base, ui_movies::kMovieTable, array);
        guest::StoreU32(base, ui_movies::kMovieTable + 4, capacity);
        guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
        // No movie for a missing id (UiMovies::Find's answer), so DeleteAll
        // deletes none (the movie world's movie 0 isn't a heap block).
        guest::StoreU32(base, 0x826C5CDC, 0);
      },
      __imp__sub_8223D4D0, hooks::Type8ModeTearDown, [base](Outputs& out) {
        ModeOutputs(base, out);
        out.Bytes(Read(base, g_strings, strings_and_font::kStringsBytes));
        out.Bytes(Read(base, ui_movies::kMovieTable + 4, 8));
      });
  Group(
      h, world, "ModeType8::ForceExit", 0x8223DA00, 10, {128, 40},
      [base](PPCContext& t, Random& r, uint32_t) {
        LayMode(base, r);
        LayStand(base, r);
        t.r3.u64 = (uint64_t(r.Below(4) == 0) << 32) | (g_mode + 32);
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(6);
      },
      __imp__sub_8223DA00, hooks::Type8ModeForceExit, [base](Outputs& out) {
        ModeOutputs(base, out);
        StandOutputs(base, out);
      });
  for (size_t i = 0; i < saved.size(); i++) {
    Write(base, kModeGlobals[i].first, saved[i].data(), kModeGlobals[i].second);
  }

  // SaveData::FreeTwoPlayerBlock: the block none or operator new's;
  // compared: the word after, whether the block is free (handed out again)
  // and the frame's back chain.
  {
    Results results("SaveData::FreeTwoPlayerBlock (sub_82105638)");
    const uint32_t kept = guest::LoadU32(base, kTwoPlayerBlock);
    for (int c = 0; c < 6; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(Seed(0x82105638ull << 16 ^ uint64_t(c)));
        uint32_t block = 0;
        if (r.Below(3) != 0) block = memory::crt::OperatorNew(base, 64).value_or(0);
        guest::StoreU32(base, kTwoPlayerBlock, block);
        PPCContext t = CallContext(h);
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        if (v == 0) {
          ITC_FLAG(native_type8_mode) = false;
          __imp__sub_82105638(t, base);
          ITC_FLAG(native_type8_mode) = true;
        } else {
          hooks::Type8FreeTwoPlayerBlock(t, base);
        }
        out[v].Word(guest::LoadU32(base, kTwoPlayerBlock));
        out[v].Word(block == 0 ? 0 : 1);
        out[v].Bytes(Read(base, t.r1.u32 - 96, 4));
        out[v].Word(t.r1.u32);
        if (block != 0) {
          const auto probe = memory::crt::OperatorNew(base, 64);
          out[v].Word(probe && *probe == block ? 1 : 0);
          if (probe) memory::crt::OperatorDelete(*probe);
        } else {
          out[v].Word(t.r3.u32);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
    guest::StoreU32(base, kTwoPlayerBlock, kept);
  }

  // ModeType8::Release: StandScene::Release's world (with --native_heap):
  // the holder, its slots and the viewports the layer's pages; compared:
  // which blocks are still pages, the two globals, r1 and r3.
  if (hooks::NativeHeapActive()) {
    Results results("ModeType8::Release (sub_8223D3B8)");
    const uint32_t kept_holder = guest::LoadU32(base, 0x826D83E0);
    const uint32_t kept_viewports = guest::LoadU32(base, kViewports);
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(Seed(0x8223D3B8ull << 16 ^ uint64_t(c)));
        std::vector<uint32_t> made;
        auto pages = [&](uint32_t bytes) {
          const uint32_t block = memory::AllocPages(bytes, 4096);
          WriteRandom(base, block, bytes < 64 ? bytes : 64, r);
          made.push_back(block);
          return block;
        };
        const uint32_t viewports = pages(12);
        guest::StoreU32(base, viewports + 0, pages(1872));
        guest::StoreU32(base, viewports + 4, pages(1872));
        guest::StoreU32(base, kViewports, viewports);
        const uint32_t holder = pages(12);
        guest::StoreU32(base, 0x826D83E0, holder);
        for (uint32_t i = 0; i < 3; i++) {
          uint32_t slot = 0;
          if (r.Below(2) != 0) {
            slot = pages(64);
            guest::StoreU32(base, slot, 0);
          }
          guest::StoreU32(base, holder + 4 * i, slot);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = r.Next();
        t.r4.u64 = r.Next();
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        if (v == 0) {
          ITC_FLAG(native_type8_mode) = false;
          __imp__sub_8223D3B8(t, base);
          ITC_FLAG(native_type8_mode) = true;
        } else {
          hooks::Type8ModeRelease(t, base);
        }
        out[v].Word(uint32_t(made.size()));
        for (const uint32_t block : made) {
          out[v].Word(memory::IsPages(block));
          if (memory::IsPages(block)) memory::FreePages(block);
        }
        out[v].Word(guest::LoadU32(base, 0x826D83E0));
        out[v].Word(guest::LoadU32(base, kViewports));
        out[v].Word(t.r1.u32);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
    guest::StoreU32(base, 0x826D83E0, kept_holder);
    guest::StoreU32(base, kViewports, kept_viewports);
  }
  g_movies = nullptr;
  saved_world.Restore();
  memory::Free(g_strings);
  memory::Free(g_loading_movie);
  memory::Free(g_save);
  memory::Free(g_mode);
  memory::Free(g_stand);
  g_stand = 0;
}

}  // namespace

void Type8Mode(Context& h) {
  Type8Dialogs(h);
  const uint32_t world = memory::Alloc(kWorldBytes, 0);
  if (world == 0) {
    ITC_LOG_ERROR("harness: type8_mode: no world block");
    return;
  }
  Type8Screen0s(h, world);
  Type8ModeSlots(h, world);
  memory::Free(world);
}

}  // namespace harness
