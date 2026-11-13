// The type 5 mode and its dialog (src/type5_mode/,
// docs/functions/type5-mode.md) against the recompiled functions, laid as
// the type 2 dialog's and the type 10 mode's harnesses lay their worlds
// (type2_dialog.cpp, type10_mode.cpp). Each case fills a world of its own
// (one block from the allocation layer) with random words from the case's
// seed: the type 5 block (the car settings' machine at +0, the parking
// area's at +9640, the dialog's at +19280), the mode with its message ring
// (its dispatcher without handlers, so GameMessages::Dispatch takes the
// queued messages and delivers none), the dialog's page object, menu and
// data (the globals 0x82814E3C to 0x82814E44 pointed at them, put back
// after), the parking area's object [0x82814E50], the players' action
// mappers and a fake sound manager (as the type 2 dialog's harness shapes
// them), and one UI movie with 8 nodes, named animations and a few playing
// slots, named 16 to 19 in a movie table of our own among other ids. It
// shapes the words each function tests (the page, the step, the queued step
// and state, the cursor, the machines' states and loaded words), runs the
// original (with --native_type5_mode off, so what it calls of the mode's is
// the original too) or the register steps (hooks/type5_mode.h), and
// compares, exactly (nothing here is a float): the calls to UiMovie::Run's
// update (sub_8211D2F8) and UiMovie::ApplyAnimation (sub_825A08E0), stood in
// for and answered from the case's generator; the screen start's and
// Strings::Load's calls (recorded, answered 1); the world; the globals; the
// stack ranges each function names (its frame without its register saves,
// and for the dialog's functions the stack below it but the callees' saves
// of the registers the native caller doesn't hold); the caller's area above
// r1; r1 and r3. Draw and Update run the pages and steps from their tables
// recompiled on both sides (their own cases are here too). ModeType5::Init
// runs over the loading screen's world (loading_world.h, a made-up time for
// the tip) and compares the block VirtualAlloc made by content.

#include "harness.h"

#include "loading_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/type5_mode.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "race_loading/race_loading.h"
#include "type5_mode/type5_mode.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <optional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type5_mode);

namespace time_hooks {
void SetFakeNow(uint64_t now);
void ClearFakeNow();
}  // namespace time_hooks

namespace harness {

namespace {

namespace t5 = type5_mode;

// The world's layout.
constexpr uint32_t kTypeBlockAt = 0x0;      // the type 5 block (28,940 bytes)
constexpr uint32_t kModeAt = 0x7200;        // the mode (852; its dispatcher +36, ring +240)
constexpr uint32_t kPagesAt = 0x7600;       // the page object (28 bytes)
constexpr uint32_t kMenuAt = 0x7640;        // the menu (68)
constexpr uint32_t kDataAt = 0x76C0;        // the data (68)
constexpr uint32_t kPairsAt = 0x7740;       // the movie table's pairs (8)
constexpr uint32_t kAnswerAt = 0x77C0;      // the delegate functions' answer
constexpr uint32_t kMappersAt = 0x7800;     // 4 mappers of 0x200
constexpr uint32_t kVtablesAt = 0x8000;     // their vtables (0x40 each)
constexpr uint32_t kManagerAt = 0x8100;     // the fake sound manager
constexpr uint32_t kManagerVtableAt = 0x8180;
constexpr uint32_t kLocationAt = 0x8200;    // [0x82814E50] (Resume's +900)
constexpr uint32_t kMovieAt = 0x8600;       // the movie (0xA100 bytes)
constexpr uint32_t kNodesAt = 0x12800;      // 8 nodes of 0x800
constexpr uint32_t kNodeBytes = 0x800;
constexpr uint32_t kNodes = 8;
constexpr uint32_t kAnimationsAt = 0x16800;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kBlockBytes = 0x16A00;

constexpr uint32_t kBelow = 2048;  // the stack below the call's r1
constexpr uint32_t kAbove = 256;   // and above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kFades = 0x826A50AC;
constexpr uint32_t kFadesBytes = 64;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {t5::kPages, 12},  // the page object, the menu and the data
    {t5::kLocationScreen, 4},
    {t5::kEntered, 4},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {menu_helpers::kMapperEntries, 32},
    {menu_helpers::kControllerEntries, 16},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
    {race_loading::kLoadingCurrent, 4},
    {kFades, kFadesBytes},
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u) {}
  ~World() { memory::Free(allocation); }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t type_block() const { return at(kTypeBlockAt); }
  uint32_t dialog() const { return type_block() + t5::kDialogAt; }
  uint32_t mode() const { return at(kModeAt); }
  uint32_t dispatcher() const { return mode() + t5::kDispatcher; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
};

// Run's update and ApplyAnimation, stood in for: each call kept and
// answered from the case's generator.
struct DialogCall {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<DialogCall>* g_calls = nullptr;
Random g_answers;

bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_calls == nullptr || (function != 0x8211D2F8 && function != 0x825A08E0)) {
    return false;
  }
  g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = function == 0x8211D2F8 ? g_answers.Below(2) : g_answers.Next();
  return true;
}

// The screen start's and Strings::Load's calls, recorded and answered 1.
std::vector<uint32_t> g_opens;
uint32_t g_case_sp = 0;
hooks::StartUpRecorder g_chained = nullptr;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address == t5::kScreenStart || address == t5::kStringsLoad) {
    // The start reads r3 only (its r4 a leftover).
    g_opens.insert(g_opens.end(),
                   {address, ctx.r3.u32, address == t5::kScreenStart ? 0u : ctx.r4.u32,
                    g_case_sp - ctx.r1.u32, uint32_t(ctx.lr)});
    ctx.r3.u64 = 1;
    return true;
  }
  return g_chained != nullptr && g_chained(ctx, address);
}

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  auto at = [&](uint32_t offset) { return w.at(offset); };
  WriteRandom(base, w.block, kBlockBytes, r);
  // The movie: its lists, scenes, named animations and choice.
  const uint32_t movie = at(kMovieAt);
  auto node = [&](uint32_t i) { return at(kNodesAt) + kNodeBytes * i; };
  const uint32_t animations = at(kAnimationsAt);
  ui_movies::InitAnimationLists(base, movie);
  guest::StoreU32(base, movie + ui_movies::kMovieLive, node(0));
  guest::StoreU32(base, movie + ui_movies::kMovieSaved, node(1));
  guest::StoreU32(base, movie + ui_movies::kMovieFile, node(2));
  guest::StoreU32(base, movie + 40960, r.Below(2));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(3) == 0 ? 0 : 1);
  static constexpr uint32_t kTags[] = {0x54464D00, 0x50494300, 0x54585400, 0x12345678};
  for (uint32_t i = 0; i < kNodes; i++) {
    guest::StoreU32(base, node(i), kTags[r.Below(4)]);
    guest::StoreU32(base, node(i) + 100, r.Below(4));
    guest::StoreU32(base, node(i) + 104, i < 3 ? r.Below(4) : 0);
    guest::StoreU32(base, node(i) + 108, r.Below(4));
    guest::StoreU32(base, node(i) + 116, 0);  // a PRM's vertex count
    for (uint32_t k = 128; k < 128 + 8 * 12; k += 8) {
      guest::StoreU32(base, node(i) + k, node(3 + r.Below(kNodes - 3)));
    }
  }
  for (uint32_t k = 0; k < 8; k++) {
    for (uint32_t i = 0; i < 64; i += 4) guest::StoreU32(base, animations + 64 * k + i, 0);
    guest::StoreFloat(base, animations + 64 * k + 12, float(r.Below(40)) / 4.0f);
  }
  for (uint32_t k = 0; k < 16; k++) {
    guest::StoreU32(base, movie + 176 + 36 * k + 28, node(0));
    guest::StoreU32(base, movie + 176 + 36 * k + 32, animations + 64 * r.Below(8));
  }
  guest::StoreU32(base, node(0) + 116, r.Below(4));  // the scene's animation index
  for (uint32_t k = r.Below(3); k > 0; k--) {
    ui_movies::Slot slot{};
    slot[0] = r.Below(2);
    slot[1] = 0xFFFFFFFFu;
    static constexpr uint32_t kNamed[] = {1, 2, 3, 5, 6, 0xFFFFFFFFu};
    slot[2] = kNamed[r.Below(6)];
    slot[3] = node(0);
    slot[4] = animations + 64 * r.Below(8);
    slot[5] = std::bit_cast<uint32_t>(float(r.Below(40)) / 4.0f);
    slot[6] = r.Below(2);
    slot[7] = r.Below(2);
    ui_movies::Append(base,
                      movie + (r.Below(2) ? ui_movies::kMovieQueue : ui_movies::kMovieLooping),
                      slot);
  }
  // The movie table: ids 16 to 19 the movie, among others, in a random place
  // (now and then one of them missing: Find then answers the word at
  // 0x826C5CDC, the movie too).
  const uint32_t pairs = at(kPairsAt);
  const uint32_t count = 4 + r.Below(5);
  const uint32_t first = r.Below(count - 3);
  const uint32_t missing = r.Below(4) == 0 ? r.Below(4) : 4;
  for (uint32_t i = 0; i < count; i++) {
    const bool ours = i >= first && i < first + 4;
    const uint32_t id = ours ? 16 + (i - first) : 30 + i;
    guest::StoreU32(base, pairs + 8 * i, ours && i - first == missing ? 99 : id);
    guest::StoreU32(base, pairs + 8 * i + 4, ours ? movie : uint32_t(r.Next()));
  }
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
  guest::StoreU32(base, kMissingMovie, movie);
  // The fake sound manager.
  const uint32_t manager = at(kManagerAt), vtable = at(kManagerVtableAt);
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
  guest::StoreU32(base, vtable + 52, 0x820CE9F8);  // slot 13: r4, r5 to +444, +448
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
  // The players' mappers.
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t vt = at(kVtablesAt) + 0x40 * k;
    const uint32_t mapper = at(kMappersAt) + 0x200 * k;
    guest::StoreU32(base, menu_helpers::kControllerEntries + 4 * k, r.Below(4));
    guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * k, mapper);
    guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * k + 4, r.Below(5) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper, vt);
    guest::StoreU32(base, vt + menu_helpers::kPressedOffset, menu_helpers::kPressedSlot);
    guest::StoreU32(base, vt + menu_helpers::kFiredOffset, menu_helpers::kFiredSlot);
    // Now and then only pressed bits (+456), so the menu doesn't move and
    // the pages' accept and back are read.
    const bool pressed_only = r.Below(3) != 0;
    for (uint32_t i = 0; i < 6; i++) {
      uint32_t bits = r.Below(3) == 0 ? 0 : uint32_t(r.Next() & r.Next());
      if (pressed_only) {
        bits = i == 2 ? uint32_t(r.Next()) : 0;
      }
      guest::StoreU32(base, mapper + 448 + 4 * i, bits);
    }
  }
  // The menu (as menu_helpers' harness shapes it): items 0 to 1.
  const uint32_t menu = at(kMenuAt);
  guest::StoreU32(base, menu + menu_helpers::kPlayer, r.Below(4));
  guest::StoreU32(base, menu + menu_helpers::kFirst, 0);
  guest::StoreU32(base, menu + menu_helpers::kLast, 1);
  guest::StoreU32(base, menu + menu_helpers::kCursor, r.Below(2));
  guest::StoreU32(base, menu + menu_helpers::kPrevious, r.Below(2));
  for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                           menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
    guest::StoreU32(base, menu + o, r.Below(2));
  }
  guest::StoreU32(base, menu + menu_helpers::kMoved, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, menu + menu_helpers::kSide, 0);
  // The page object: the page -1 or 0 to 3, the step (Update: none, 0 to
  // 9, another negative), the queued step, the started word.
  const uint32_t pages = at(kPagesAt);
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, pages + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(4));
  }
  guest::StoreU32(base, pages + 0, r.Below(5) == 0 ? 0xFFFFFFFFu : r.Below(4));
  static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0, 1, 2, 3, 4,
                                        5, 6, 7, 8, 9, 0xFFFFFFFEu};
  guest::StoreU32(base, pages + 12, kSteps[r.Below(13)]);
  guest::StoreU32(base, pages + 16, r.Below(2) ? 0xFFFFFFFFu : r.Below(10));
  guest::StoreU32(base, pages + 24, r.Below(2));
  // The data: loaded, the queued state, the movie shown, interrupted.
  const uint32_t data = at(kDataAt);
  guest::StoreU32(base, data + 0, r.Below(2));
  static constexpr uint32_t kStates[] = {21, 21, 9, 9, 4, 5, 6};
  guest::StoreU32(base, data + 4, kStates[r.Below(7)]);
  guest::StoreU32(base, data + 8, 16 + r.Below(4));
  guest::StoreU32(base, data + 12, r.Below(4) == 0 ? 1 : 0);
  // The three machines: a state, loaded (1 mostly), re-entered.
  static constexpr uint32_t kMachineStates[] = {1, 2, 4, 5, 5, 6, 7, 8, 9, 9,
                                                10, 11, 12, 13, 14, 20};
  for (const uint32_t offset : {t5::kCarSettingsAt, t5::kLocationAt, t5::kDialogAt}) {
    const uint32_t machine = w.type_block() + offset;
    guest::StoreU32(base, machine + t5::kMachineState,
                    kMachineStates[r.Below(std::size(kMachineStates))]);
    guest::StoreU32(base, machine + t5::kMachineLoaded,
                    r.Below(4) == 0 ? (r.Below(2) == 0 ? uint32_t(r.Next()) : 0) : 1);
    guest::StoreU32(base, machine + t5::kMachineReenter,
                    r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
  // The shown machine: one of the three ids (as FindShown and Init leave
  // it in play).
  static constexpr uint32_t kIds[] = {3, 16, 21};
  guest::StoreU32(base, w.type_block() + t5::kShown, kIds[r.Below(3)]);
  // The mode: its block, its ring's count and start small, its dispatcher
  // without handlers.
  guest::StoreU32(base, w.mode() + t5::kBlock, w.type_block());
  guest::StoreU32(base, w.mode() + t5::kRing + 600, r.Below(40));
  guest::StoreU32(base, w.mode() + t5::kRing + 604, r.Below(60));
  guest::StoreU32(base, w.dispatcher() + 200, 0);
  // The globals.
  guest::StoreU32(base, t5::kPages, pages);
  guest::StoreU32(base, t5::kMenu, menu);
  guest::StoreU32(base, t5::kData, data);
  guest::StoreU32(base, t5::kLocationScreen, at(kLocationAt));
  guest::StoreU32(base, t5::kEntered, r.Below(4));
  guest::StoreU32(base, race_loading::kLoadingCurrent, 3);  // LoadingScreen::Run's none
}

// A stack range compared, relative to the call's r1: [from, to).
struct Range {
  int32_t from;
  int32_t to;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;
// Run after the function, before the outputs are read.
using AfterFn = std::function<void(PPCContext&, const World&, Outputs&)>;

struct Spec {
  const char* name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;
  SetupFn setup;
  AfterFn after = nullptr;
  // Draw and Update: the pages and steps from their tables recompiled on
  // both sides (the flag off for the native run too).
  bool callees_recompiled = false;
  // LoadingScreen::Run and Finish over the loading screen's world
  // (loading_world.h): its current movie 0 to 2 (as in play).
  bool loading = false;
};

const loading_world::MovieWorld* g_loading = nullptr;

// The layer's pages a case made (blocks the function frees): after the
// run, whether each is still pages (then freed).
std::vector<uint32_t> g_made;

void Run(Context& h, const World& w, const Spec& spec) {
  uint8_t* const base = h.base;
  Results results(spec.name);
  for (int c = 0; c < spec.cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(base, g.address, g.bytes));
      }
      MakeWorld(base, w, r);
      WriteRandom(base, kFades, kFadesBytes, r);
      std::optional<loading_world::SavedGlobals> loading;
      if (spec.loading) {
        loading.emplace(base);
        loading_world::ShapeMovieWorld(base, *g_loading, r);
        guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
      }
      g_made.clear();
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, sp - kBelow, kBelow + kAbove, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_answers = Random(r.Next());
      std::vector<DialogCall> calls;
      g_calls = &calls;
      g_opens.clear();
      g_case_sp = sp;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      g_chained = recorder;
      hooks::g_start_up_recorder = RecordOpen;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = StandIn;
      const bool flag = ITC_FLAG(native_type5_mode);
      if (v == 0) {
        ITC_FLAG(native_type5_mode) = false;
        spec.recompiled(t, base);
      } else {
        ITC_FLAG(native_type5_mode) = !spec.callees_recompiled;
        spec.native(t, base);
      }
      ITC_FLAG(native_type5_mode) = flag;
      hooks::g_start_up_recorder = recorder;
      g_chained = nullptr;
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      g_calls = nullptr;
      out[v].Word(uint32_t(calls.size()));
      for (const DialogCall& call : calls) {
        for (const uint32_t word : {call.function, call.lr, sp - call.r1, uint32_t(call.r3),
                                    uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                    uint32_t(call.r4 >> 32)}) {
          out[v].Word(word);
        }
      }
      out[v].Word(uint32_t(g_opens.size()));
      for (const uint32_t word : g_opens) {
        out[v].Word(word);
      }
      if (spec.after) {
        spec.after(t, w, out[v]);
      }
      out[v].Word(uint32_t(g_made.size()));
      for (const uint32_t made : g_made) {
        const bool pages = memory::IsPages(made);
        out[v].Word(pages ? 1 : 0);
        if (pages) {
          memory::FreePages(made);
        }
      }
      g_made.clear();
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(base, g.address, g.bytes));
      }
      out[v].Bytes(Read(base, w.block, kBlockBytes));
      for (const Range& range : spec.stack) {
        out[v].Bytes(Read(base, sp + range.from, uint32_t(range.to - range.from)));
      }
      out[v].Bytes(Read(base, sp, kAbove));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      if (loading) {
        out[v].Bytes(Read(base, g_loading->movies.address, g_loading->movies.bytes));
        loading->Restore();
      }
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The dialog's functions' stack: below the frame (without the callees' saves
// of the registers the native caller doesn't hold: `callee_saves` under it)
// and the frame without its own saves.
std::vector<Range> DialogStack(int32_t frame, int32_t saves, int32_t callee_saves = 0) {
  if (frame == 0) {
    return {{-int32_t(kBelow), 0}};
  }
  return {{-int32_t(kBelow), -frame - callee_saves}, {-frame, -saves}};
}

uint64_t High(Random& r) { return uint64_t(r.Below(2)) << 32; }

// r3 the started word (the page object's +24, as Update passes it).
void Started(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = (w.at(kPagesAt) + 24) | High(r);
}
// r3 the dispatcher (the pages).
void DispatcherR3(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = w.dispatcher() | High(r);
}
// r3 the dialog's machine, r4 the dispatcher.
void Machine(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = w.dialog() | High(r);
  t.r4.u64 = w.dispatcher();
}

// OnEnter's blocks: their written words compared, freed, their globals 1;
// r3 (the data block) as 1.
void EnterBlocks(PPCContext& t, const World& w, Outputs& out) {
  uint8_t* base = w.base;
  static constexpr uint32_t kWritten[3] = {28, 68, 16};
  const uint32_t globals[3] = {t5::kPages, t5::kMenu, t5::kData};
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t made = guest::LoadU32(base, globals[k]);
    const bool ours = made != 0 && memory::IsPages(made);
    out.Word(ours ? 1 : made);
    if (!ours) {
      continue;
    }
    std::vector<uint8_t> bytes = Read(base, made, kWritten[k]);
    if (k == 1) {
      // The menu's +44, +56 and +60, not written (the layer's pages'
      // contents).
      for (const uint32_t at : {44u, 56u, 60u}) {
        for (uint32_t i = 0; i < 4; i++) bytes[at + i] = 0;
      }
    } else if (k == 0) {
      for (uint32_t i = 0; i < 4; i++) bytes[16 + i] = 0;  // +16 not written
    }
    out.Bytes(bytes);
    if (t.r3.u32 == made) t.r3.u64 = 1;
    memory::FreePages(made);
    guest::StoreU32(base, globals[k], 1);
  }
}

void Dialog(Context& h, const World& w) {
  struct Case {
    const char* name;
    GuestFunction recompiled, native;
    int32_t frame, saves;
    SetupFn setup;
  };
  const Case steps[] = {
      {"Type5Dialog::OpenConfirmStep (sub_822261E0)", __imp__sub_822261E0,
       hooks::Type5DialogOpenConfirmStep, 128, 32, Started},
      {"Type5Dialog::CloseConfirmStep (sub_822262B8)", __imp__sub_822262B8,
       hooks::Type5DialogCloseConfirmStep, 112, 24, Started},
      {"Type5Dialog::CloseNoticeStep (sub_82226370)", __imp__sub_82226370,
       hooks::Type5DialogCloseNoticeStep, 112, 24, Started},
      {"Type5Dialog::OpenConfirm2Step (sub_82226420)", __imp__sub_82226420,
       hooks::Type5DialogOpenConfirm2Step, 112, 24, Started},
      {"Type5Dialog::CloseConfirm2Step (sub_822264B8)", __imp__sub_822264B8,
       hooks::Type5DialogCloseConfirm2Step, 112, 24, Started},
      {"Type5Dialog::CloseNotice2Step (sub_82226568)", __imp__sub_82226568,
       hooks::Type5DialogCloseNotice2Step, 112, 24, Started},
      {"Type5Dialog::MoveCursorStep (sub_82226618)", __imp__sub_82226618,
       hooks::Type5DialogMoveCursorStep, 128, 40, Started},
      {"Type5Dialog::Step9 (sub_822266D0)", __imp__sub_822266D0, hooks::Type5DialogStep9, 96,
       8, Started},
  };
  for (const Case& c : steps) {
    Run(h, w, {c.name, 32, c.recompiled, c.native, DialogStack(c.frame, c.saves), c.setup});
  }
  const Case others[] = {
      {"Type5Dialog::ConfirmPage (sub_82225D10)", __imp__sub_82225D10,
       hooks::Type5DialogConfirmPage, 144, 64, DispatcherR3},
      {"Type5Dialog::NoticePage (sub_82225F30)", __imp__sub_82225F30,
       hooks::Type5DialogNoticePage, 144, 64, DispatcherR3},
      {"Type5Dialog::Notice2Page (sub_82226088)", __imp__sub_82226088,
       hooks::Type5DialogNotice2Page, 144, 64, DispatcherR3},
      {"Type5Dialog::WaitForMovies (sub_82226978)", __imp__sub_82226978,
       hooks::Type5DialogWaitForMovies, 96, 16, Machine},
      {"Type5Dialog::OnInterrupt (sub_82225860)", __imp__sub_82225860,
       hooks::Type5DialogOnInterrupt, 0, 0, Machine},
      {"Type5Dialog::OpenConfirm (sub_82225A90)", __imp__sub_82225A90,
       hooks::Type5DialogOpenConfirm, 112, 24, Machine},
      {"Type5Dialog::OpenNotice (sub_82225980)", __imp__sub_82225980,
       hooks::Type5DialogOpenNotice, 0, 0, Machine},
      {"Type5Dialog::OpenConfirm2 (sub_82225B88)", __imp__sub_82225B88,
       hooks::Type5DialogOpenConfirm2, 112, 16, Machine},
      {"Type5Dialog::OpenNotice2 (sub_82225A08)", __imp__sub_82225A08,
       hooks::Type5DialogOpenNotice2, 0, 0, Machine},
  };
  for (const Case& c : others) {
    Run(h, w, {c.name, 128, c.recompiled, c.native, DialogStack(c.frame, c.saves), c.setup});
  }
  Run(h, w, {"Type5Dialog::Draw (sub_822266F8)", 128, __imp__sub_822266F8,
             hooks::Type5DialogDraw, DialogStack(112, 8), Machine, nullptr, true});
  // Update: below its frame, the steps' saves of Update's registers (up to
  // MoveCursorStep's 40 bytes) left out.
  Run(h, w, {"Type5Dialog::Update (sub_82226788)", 128, __imp__sub_82226788,
             hooks::Type5DialogUpdate, DialogStack(208, 64, 40), Machine, nullptr, true});
  Run(h, w, {"Type5Dialog::OnEnter (sub_82225878)", 128, __imp__sub_82225878,
             hooks::Type5DialogOnEnter, DialogStack(112, 32), Machine, EnterBlocks});
  // OnLeft: the globals pointing at blocks of the layer's pages the case
  // made (none now and then), whether each was freed compared.
  Run(h, w, {"Type5Dialog::OnLeft (sub_822257A8)", 128, __imp__sub_822257A8,
             hooks::Type5DialogOnLeft, DialogStack(112, 32),
             [](PPCContext& t, Random& r, const World& w) {
               for (const uint32_t global : {t5::kPages, t5::kMenu, t5::kData}) {
                 uint32_t made = 0;
                 if (r.Below(6) != 0) {
                   made = memory::AllocPages(global == t5::kPages ? 28 : 68, 4096);
                   WriteRandom(w.base, made, 64, r);
                   g_made.push_back(made);
                 }
                 guest::StoreU32(w.base, global, made);
               }
               t.r3.u64 = w.dialog() | High(r);
             }});
  // Init: the machine empty or holding random transitions with keys of
  // small numbers (states below 12, messages below 52), so some match the
  // Init's (Add replaces those).
  Run(h, w, {"Type5Dialog::Init (sub_82226A10)", 128, __imp__sub_82226A10,
             hooks::Type5DialogInit, DialogStack(1104, 48),
             [](PPCContext& t, Random& r, const World& w) {
               namespace gm = game_modes;
               uint8_t* base = w.base;
               const uint32_t machine = w.dialog();
               if (r.Below(2) == 0) {
                 gm::StateMachineConstruct(base, machine);
                 guest::StoreU32(base, machine + gm::kTransitionCount, 0);
               } else {
                 const uint32_t count = 8 + r.Below(160);
                 guest::StoreU32(base, machine + gm::kTransitionCount, count);
                 for (uint32_t i = 0; i < gm::kTransitions; i++) {
                   const uint32_t at = machine + gm::kTransitionBytes * i;
                   guest::StoreU32(base, at, r.Below(12));
                   guest::StoreU32(base, at + 4, r.Below(52));
                   guest::StoreU32(base, at + 8, r.Below(8) == 0 ? 1 : 0);
                   guest::StoreU32(base, at + 12, 0);
                 }
               }
               t.r3.u64 = machine | High(r);
             }});
}

// --- The mode ---

// r3 the answer, r4 the mode; now and then the parking area in the states
// NextMode answers for.
void Delegate(PPCContext& t, Random& r, const World& w) {
  uint8_t* base = w.base;
  if (r.Below(3) == 0) {
    static constexpr uint32_t kStates[] = {8, 10, 11, 13};
    guest::StoreU32(base, w.type_block() + t5::kLocationAt + t5::kMachineState,
                    kStates[r.Below(4)]);
  }
  if (r.Below(4) == 0) {
    for (const uint32_t offset : {t5::kCarSettingsAt, t5::kLocationAt, t5::kDialogAt}) {
      guest::StoreU32(base, w.type_block() + offset + t5::kMachineLoaded, 1);
    }
  }
  t.r3.u64 = w.at(kAnswerAt);
  t.r4.u64 = w.mode();
}
// r3 the mode + 32 (r4 a number for ForceExit).
void Interface(PPCContext& t, Random& r, const World& w) {
  if (r.Below(2) == 0) {
    for (const uint32_t offset : {t5::kCarSettingsAt, t5::kLocationAt, t5::kDialogAt}) {
      guest::StoreU32(w.base, w.type_block() + offset + t5::kMachineLoaded, 1);
    }
  }
  t.r3.u64 = w.mode() + 32;
  t.r4.u64 = r.Below(6);
}

// The words of [at, at + bytes) that point into [lo, lo + span) made their
// offsets with a mark (the block VirtualAlloc made differs by side).
void NormalizePointers(uint8_t* base, uint32_t at, uint32_t bytes, uint32_t lo, uint32_t span) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, at + i);
    if (word >= lo && word < lo + span) {
      guest::StoreU32(base, at + i, 0xB10C0000u ^ (word - lo));
    }
  }
}

void Mode(Context& h, const World& w) {
  Run(h, w, {"ModeType5::FindShown (sub_8222FD08)", 60, __imp__sub_8222FD08,
             hooks::ModeType5FindShown, {{-int32_t(kBelow), 0}},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = w.type_block() | High(r);
             }});
  Run(h, w, {"ModeType5::WaitForLoads (sub_82230050)", 100, __imp__sub_82230050,
             hooks::ModeType5WaitForLoads, {{-128, -32}}, Delegate, nullptr, false, true});
  Run(h, w, {"ModeType5::NextMode (sub_82230458)", 300, __imp__sub_82230458,
             hooks::ModeType5NextMode, {{-160, -64}}, Delegate});
  Run(h, w, {"ModeType5::LeaveScreen (sub_82230260)", 60, __imp__sub_82230260,
             hooks::ModeType5LeaveScreen, {{-128, -32}}, Delegate});
  Run(h, w, {"ModeType5::WaitForShown (sub_82230168)", 100, __imp__sub_82230168,
             hooks::ModeType5WaitForShown, {{-112, -16}}, Delegate});
  Run(h, w, {"ModeType5::Leaving (sub_82230328)", 100, __imp__sub_82230328,
             hooks::ModeType5Leaving, {{-128, -32}}, Delegate});
  // The slot 14 thunk: the mode's vtable this mode's (its slot 14 Leaving).
  Run(h, w, {"Mode::Slot14Thunk (sub_8222FC50)", 60, __imp__sub_8222FC50, hooks::ModeSlot14Thunk,
             {{-128, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               Delegate(t, r, w);
               guest::StoreU32(w.base, w.mode(), 0x82006C24);
             }});
  Run(h, w, {"ModeType5::Interrupt (sub_8222FED0)", 150, __imp__sub_8222FED0,
             hooks::ModeType5Interrupt, {{-int32_t(kBelow), 0}}, Interface});
  Run(h, w, {"ModeType5::ForceExit (sub_8222FF60)", 60, __imp__sub_8222FF60,
             hooks::ModeType5ForceExit, {{-int32_t(kBelow), 0}}, Interface});
  Run(h, w, {"ModeType5::Resume (sub_8222FC38)", 20, __imp__sub_8222FC38,
             hooks::ModeType5Resume, {{-int32_t(kBelow), 0}}, Interface});
}

// ModeType5::Init: the mode random words (its dispatcher's count 0 to 7),
// the screen start and Strings::Load recorded, LoadingScreen::Show over the
// loading screen's world (loading_world.h, a made-up time for the tip), the
// fades saved and put back. Compared: the recorded calls, the mode, the
// block VirtualAlloc made (by content, its addresses in it and in the mode
// made offsets, then freed), the loading world, the fades, the frame's +80
// to +87 (the machines' Inits' frames below hold the machines' addresses)
// and r3.
void Init(Context& h, const World& w, const loading_world::MovieWorld& movies) {
  namespace lw = loading_world;
  uint8_t* const base = h.base;
  Results results("ModeType5::Init (sub_8222FD68)");
  for (int c = 0; c < 12; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(base, g.address, g.bytes));
      }
      const lw::SavedGlobals loading(base);
      lw::ShapeMovieWorld(base, movies, r);
      lw::BuildStringTable(base, movies);
      guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
      guest::StoreU32(base, race_loading::kLoadingShown, uint32_t(r.Next()));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(base, race_loading::kLoadingAnimations + 4 * i,
                        r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
      }
      WriteRandom(base, w.block, kBlockBytes, r);
      WriteRandom(base, kFades, kFadesBytes, r);
      guest::StoreU32(base, w.dispatcher() + 200, r.Below(8));
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, sp - kBelow, kBelow + kAbove, r);
      t.r3.u64 = w.mode();
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      time_hooks::SetFakeNow(0x01D0000000000000ull + r.Next() % 0x0010000000000000ull);
      g_opens.clear();
      g_case_sp = sp;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      g_chained = recorder;
      hooks::g_start_up_recorder = RecordOpen;
      if (v == 0) {
        const bool native = ITC_FLAG(native_type5_mode);
        ITC_FLAG(native_type5_mode) = false;
        __imp__sub_8222FD68(t, base);
        ITC_FLAG(native_type5_mode) = native;
      } else {
        hooks::ModeType5Init(t, base);
      }
      hooks::g_start_up_recorder = recorder;
      g_chained = nullptr;
      time_hooks::ClearFakeNow();
      out[v].Word(uint32_t(g_opens.size()));
      for (const uint32_t word : g_opens) {
        out[v].Word(word);
      }
      const uint32_t block = guest::LoadU32(base, w.mode() + t5::kBlock);
      const bool made = memory::IsPages(block);
      out[v].Word(made ? 1 : 0);
      if (made) {
        NormalizePointers(base, block, t5::kBlockBytes, block, t5::kBlockBytes);
        out[v].Bytes(Read(base, block, t5::kBlockBytes));
        NormalizePointers(base, w.mode(), 1024, block, t5::kBlockBytes);
        memory::FreePages(block);
        guest::StoreU32(base, w.mode() + t5::kBlock, 1);
      }
      out[v].Bytes(Read(base, w.block, kBlockBytes));
      out[v].Bytes(Read(base, movies.movies.address, movies.movies.bytes));
      out[v].Bytes(Read(base, movies.texts.address, movies.texts.bytes));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingShown));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
      out[v].Bytes(Read(base, kFades, kFadesBytes));
      out[v].Bytes(Read(base, sp - 128 + 80, 8));
      out[v].Bytes(Read(base, sp, kAbove));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      loading.Restore();
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void Type5Mode(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: type5_mode: no block from the allocation layer");
    return;
  }
  const loading_world::MovieWorld loading;
  if (!loading.ok()) {
    ITC_LOG_ERROR("harness: type5_mode: no loading world");
    return;
  }
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_WARN("harness: type5_mode: the allocation layer isn't native: skipped");
    return;
  }
  g_loading = &loading;
  Dialog(h, w);
  Mode(h, w);
  Init(h, w, loading);
  g_loading = nullptr;
}

}  // namespace harness
