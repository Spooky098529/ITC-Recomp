// The power graph screen (src/power_graph_screen/,
// docs/functions/power-graph-screen.md) against the recompiled functions.
// Each case fills a world of its own with random words from the case's
// seed: one block from the allocation layer (the member with its vtable of
// type 6's or type 7's slots, the mode with its message ring, the page
// object, the menu and the data with the record, the players' action
// mappers and a fake sound manager, as the type 2 dialog's harness shapes
// them; the globals 0x82815EB4 to 0x82815EC0 pointed at them, put back
// after) and the UI movie world the loading screen's harness builds
// (loading_world.h: movie 9 its first movie, in a movie table of our own
// among other ids; TXT leaves, texts ids 1 to 7, the string table of
// letters only, no argument codes: SetText's unset r7 to r10 are 0 from
// ours, the original's leftovers, the user's call of 2026-10-30). It
// shapes the words each function tests (the page, the step, the queued step
// and state, the tab, the record), runs the original (with
// --native_power_graph_screen off) or the register steps
// (hooks/power_graph_screen.h), and compares, exactly: the calls to
// UiMovie::Run's update (sub_8211D2F8) and UiMovie::ApplyAnimation
// (sub_825A08E0) and UiMovies::Delete, stood in for and answered; the
// block; the movie world; the globals; the stack around the call (but the
// originals' register saves, theirs and their callees', and the words
// SetText's varargs store in SetTexts' frame, +48 to +79); r1 and r3. Draw
// and Update run the pages and steps from their tables recompiled on both
// sides (their own cases are here too).

#include "harness.h"
#include "loading_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/app_start_up.h"
#include "hooks/power_graph_screen.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "power_graph_screen/power_graph_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_power_graph_screen);

namespace harness {

namespace {

namespace pgs = power_graph_screen;
namespace lw = loading_world;

// The block's layout.
constexpr uint32_t kMemberAt = 0x0;      // the member (9,620 bytes)
constexpr uint32_t kModeAt = 0x2600;     // the mode (its ring at +204)
constexpr uint32_t kPagesAt = 0x2A00;    // the page object (28 bytes)
constexpr uint32_t kMenuAt = 0x2A40;     // the menu (68)
constexpr uint32_t kDataAt = 0x2AC0;     // the data (92)
constexpr uint32_t kPairsAt = 0x2B40;    // the movie table's pairs (8)
constexpr uint32_t kMappersAt = 0x2C00;  // 4 mappers of 0x200
constexpr uint32_t kVtablesAt = 0x3400;  // their vtables (0x40 each)
constexpr uint32_t kManagerAt = 0x3500;  // the fake sound manager
constexpr uint32_t kManagerVtableAt = 0x3580;
constexpr uint32_t kMemberVtableAt = 0x3600;  // the member's vtable
constexpr uint32_t kArraysAt = 0x3700;        // SetParts' record, the rows' arrays
constexpr uint32_t kBlockBytes = 0x3800;

constexpr uint32_t kBelow = 1024;  // the stack compared below the call's r1
constexpr uint32_t kAbove = 256;   // and above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {pgs::kPages, 16},  // the page object, the menu, the data and the shown word
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {menu_helpers::kMapperEntries, 32},
    {menu_helpers::kControllerEntries, 16},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The calls stood in for: UiMovie::Run's update and ApplyAnimation, and
// UiMovies::Delete (its adapter hands it to the start-up recorder: it would
// free our movie), each recorded and answered.
struct StoodCall {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<StoodCall>* g_calls = nullptr;
Random g_answers;

bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_calls == nullptr || (function != 0x8211D2F8 && function != 0x825A08E0)) {
    return false;
  }
  g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = function == 0x8211D2F8 ? g_answers.Below(2) : g_answers.Next();
  return true;
}

bool RecordDelete(PPCContext& ctx, uint32_t address) {
  if (g_calls == nullptr || address != pgs::kMoviesDelete) return false;
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_calls->push_back({address, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, 0});
  ctx.r3.u64 = 1;
  return true;
}

// What a case shapes and passes.
// kSlot, kHandler, kPage, kDraw, kUpdate: r3 the member, r4 the mode;
// kOnEnter: the blocks VirtualAlloc makes compared by content where their
// pointers were stored, then freed; kOnLeft: the globals pointing at blocks
// of the layer's pages the case made, whether each was freed compared;
// kStep: r4 the page object's started word; kSetTexts: r3 the member;
// kSetParts: r4 a record of part bytes; kIcon: r4 a value; kRow: r4 a word
// array.
enum class Kind {
  kSlot,
  kHandler,
  kOnEnter,
  kOnLeft,
  kStep,
  kSetTexts,
  kSetParts,
  kIcon,
  kRow,
  kPage,
  kDraw,
  kUpdate
};

struct Function {
  const char* name;
  uint32_t address;
  GuestFunction recompiled, native;
  Kind kind;
  // The stack below r1 left out, as [r1 - from, r1 - to) ranges: the
  // originals' register saves (theirs and their callees') and SetText's
  // varargs words.
  std::vector<std::pair<uint32_t, uint32_t>> skipped;
};

// Movie 9's id among others in the movie table (now and then missing: Find
// then answers the word at 0x826C5CDC, the movie too).
void ShapeMovies(uint8_t* base, uint32_t block, const lw::MovieWorld& w, Random& r) {
  const uint32_t movie = w.movie(0);
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(3) == 0 ? 0 : 1);
  const uint32_t pairs = block + kPairsAt;
  const uint32_t count = 2 + r.Below(6);
  const uint32_t at = r.Below(count);
  const bool missing = r.Below(8) == 0;
  for (uint32_t i = 0; i < count; i++) {
    const bool ours = i == at;
    guest::StoreU32(base, pairs + 8 * i, ours ? (missing ? 99 : pgs::kMovie) : 30 + i);
    guest::StoreU32(base, pairs + 8 * i + 4, ours ? movie : w.movie(1 + r.Below(2)));
  }
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
  guest::StoreU32(base, kMissingMovie, movie);
}

// Movie 9's texts: 21 records (the world's first seven of each movie, not
// the missing text), ids 1 to 21, and every leaf's text id one of them
// (leaves 0 to 20 each its own), so SetTexts' 39 texts land in many records
// (with the world's 7, the last few calls overwrote the others' records)
// and none makes a record of its own (id 0).
void ShapeTexts(uint8_t* base, const lw::MovieWorld& w, Random& r) {
  constexpr uint32_t kRecords = 21;
  const uint32_t movie = w.movie(0);
  for (uint32_t k = 0; k < kRecords; k++) {
    const uint32_t record = w.texts.address + 0x100 * (k / 7) + 32 * (k % 7);
    guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k, k + 1);
    guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k + 4, record);
  }
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, kRecords);
  guest::StoreU32(base, movie + ui_movies::kMovieTextCount, kRecords + 1);
  for (uint32_t k = 0; k < lw::kLeaves; k++) {
    guest::StoreU32(base, w.leaf(k) + 108, k < kRecords ? k + 1 : 1 + r.Below(kRecords));
  }
  // Sub-scene 3 (the parts) a leaf of its own, the last: the root's entry 3
  // and no other entry. SetParts' helpers store their values into the +108
  // (the first child's index) of sub-scene 3's children, so their later
  // lookups through sub-scene 3 stay in the tree only when it isn't among
  // them (in the game they're pictures, not scenes).
  constexpr uint32_t kEntries = (lw::kTreeNodeBytes - 128) / 8;
  const uint32_t parts = w.leaf(lw::kLeaves - 1);
  const uint32_t three = guest::LoadU32(base, w.root() + 108) + 3;
  for (uint32_t i = 0; i < kEntries; i++) {
    guest::StoreU32(base, w.root() + 128 + 8 * i,
                    i == three ? parts : w.leaf(r.Below(lw::kLeaves - 1)));
    for (uint32_t k = 0; k < lw::kLeaves; k++) {
      guest::StoreU32(base, w.leaf(k) + 128 + 8 * i, w.leaf(r.Below(lw::kLeaves - 1)));
    }
  }
}

void MakeWorld(uint8_t* base, uint32_t block, Random& r) {
  auto at = [&](uint32_t offset) { return block + offset; };
  WriteRandom(base, block, kBlockBytes, r);
  // The member's vtable: slots 1 to 3, type 6's or type 7's.
  const uint32_t vtable = at(kMemberVtableAt);
  static constexpr uint32_t kSlots[2][3] = {{0x82237A88, 0x82237AD8, 0x82237B28},
                                            {0x82246FB8, 0x82247008, 0x82247058}};
  const uint32_t type = r.Below(2);
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, vtable + 4 + 4 * k, kSlots[type][k]);
  }
  guest::StoreU32(base, at(kMemberAt), vtable);
  // The fake sound manager.
  const uint32_t manager = at(kManagerAt), manager_vtable = at(kManagerVtableAt);
  guest::StoreU32(base, manager, manager_vtable);
  guest::StoreU32(base, manager_vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
  guest::StoreU32(base, manager_vtable + 52, 0x820CE9F8);  // slot 13: r4, r5 to +444, +448
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
    // the page's buttons are read.
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
  // The page object: the page -1, 0 or 1 (now and then 2), the step (none,
  // 0, 1, another negative), the queued step, the started word.
  const uint32_t pages = at(kPagesAt);
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, pages + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(2));
  }
  static constexpr uint32_t kPagesShown[] = {0xFFFFFFFFu, 0, 0, 1, 1, 2};
  guest::StoreU32(base, pages + 0, kPagesShown[r.Below(6)]);
  static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0, 1, 1, 0xFFFFFFFEu};
  guest::StoreU32(base, pages + 12, kSteps[r.Below(7)]);
  guest::StoreU32(base, pages + 16, r.Below(2) ? 0xFFFFFFFFu : r.Below(2));
  guest::StoreU32(base, pages + 24, r.Below(2));
  // The data: loaded, the queued state, the record (in play's ranges: the
  // numbers any word now and then, the ids small, the float a speed).
  const uint32_t data = at(kDataAt);
  guest::StoreU32(base, data + 0, r.Below(2));
  static constexpr uint32_t kStates[] = {21, 21, 21, 9, 9, 4, 1};
  guest::StoreU32(base, data + 4, kStates[r.Below(7)]);
  for (uint32_t o = 8; o <= 24; o += 4) {
    guest::StoreU32(base, data + o, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(1000));
  }
  guest::StoreU32(base, data + 28, r.Below(72));  // the car id
  guest::StoreU32(base, data + 32, r.Below(30));  // the maker
  guest::StoreU32(base, data + 36, r.Below(7));
  guest::StoreU32(base, data + 40, r.Below(7));
  guest::StoreU32(base, data + 44, r.Below(2));
  for (const uint32_t o : {48u, 52u, 56u, 64u}) {
    guest::StoreU32(base, data + o, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(100000));
  }
  static constexpr uint32_t kOdd[] = {0x7FC00000u, 0xFFC00001u, 0x7F800000u, 0xFF800000u};
  guest::StoreU32(base, data + 60,
                  r.Below(8) == 0 ? kOdd[r.Below(4)]
                                  : std::bit_cast<uint32_t>(float(r.Below(40000)) / 100.0f));
  for (uint32_t o = 68; o < 80; o++) {
    guest::StoreU8(base, data + o, uint8_t(r.Below(5) == 0 ? r.Next() : r.Below(42) - 1));
  }
  static constexpr uint32_t kTabs[] = {0, 1, 2, 3, 4, 5, 2, 5, 6, 7};
  guest::StoreU32(base, data + 88, r.Below(12) == 0 ? uint32_t(r.Next()) : kTabs[r.Below(10)]);
  // SetParts' record and the rows' arrays: values around 10 and 37.
  for (uint32_t o = 0; o < 0x40; o++) {
    guest::StoreU8(base, at(kArraysAt) + o, uint8_t(r.Below(5) == 0 ? r.Next() : r.Below(42) - 1));
  }
  for (uint32_t k = 0; k < 16; k++) {
    static constexpr uint32_t kValues[] = {10, 10, 37, 36, 0, 5, 9, 11, 0xFFFFFFFFu, 40};
    guest::StoreU32(base, at(kArraysAt) + 0x40 + 4 * k,
                    r.Below(6) == 0 ? uint32_t(r.Next()) : kValues[r.Below(10)]);
  }
  // The member: its state, loaded and re-entered words.
  guest::StoreU32(base, at(kMemberAt) + pgs::kMachineState, r.Below(11));
  guest::StoreU32(base, at(kMemberAt) + pgs::kMachineLoaded,
                  r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, at(kMemberAt) + pgs::kMachineReenter,
                  r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  // The mode's ring: its count and start small.
  guest::StoreU32(base, at(kModeAt) + 204 + 600, r.Below(50));
  guest::StoreU32(base, at(kModeAt) + 204 + 604, r.Below(60));
  // The globals.
  guest::StoreU32(base, pgs::kPages, pages);
  guest::StoreU32(base, pgs::kMenu, menu);
  guest::StoreU32(base, pgs::kData, data);
  guest::StoreU32(base, pgs::kShown, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
}

void Run(Context& h, uint32_t block, const lw::MovieWorld& w, const Function& f, int cases) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(base, g.address, g.bytes));
      }
      const lw::SavedGlobals loading(base);
      lw::ShapeMovieWorld(base, w, r);
      lw::BuildStringTable(base, w);
      ShapeTexts(base, w, r);
      MakeWorld(base, block, r);
      ShapeMovies(base, block, w, r);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      std::vector<uint32_t> pages;
      WriteRandom(base, sp - kBelow, kBelow + kAbove, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      const uint32_t member = block + kMemberAt, mode = block + kModeAt;
      const uint64_t high = uint64_t(r.Below(2)) << 32;
      switch (f.kind) {
        case Kind::kSlot:
        case Kind::kHandler:
        case Kind::kOnEnter:
        case Kind::kPage:
        case Kind::kDraw:
        case Kind::kUpdate:
        case Kind::kSetTexts:
          t.r3.u64 = member | high;
          t.r4.u64 = mode;
          break;
        case Kind::kStep:
          // The started word: the page object's +24 (as Update passes it).
          t.r3.u64 = member | high;
          t.r4.u64 = block + kPagesAt + 24;
          break;
        case Kind::kSetParts:
          t.r3.u64 = member | high;
          t.r4.u64 = block + kArraysAt + r.Below(0x20);
          break;
        case Kind::kIcon:
          t.r3.u64 = member | high;
          t.r4.u64 = r.Below(3) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(42)) - 1));
          break;
        case Kind::kRow:
          t.r3.u64 = member | high;
          t.r4.u64 = block + kArraysAt + 0x40 + 4 * r.Below(8);
          break;
        case Kind::kOnLeft:
          // The blocks to free: the layer's pages (none now and then).
          for (const uint32_t global : {pgs::kPages, pgs::kMenu, pgs::kData}) {
            uint32_t made = 0;
            if (r.Below(6) != 0) {
              made = memory::AllocPages(global == pgs::kData ? 92 : global == pgs::kMenu ? 68 : 28,
                                        4096);
              WriteRandom(base, made, 64, r);
              pages.push_back(made);
            }
            guest::StoreU32(base, global, made);
          }
          t.r3.u64 = member | high;
          break;
      }
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_answers = Random(r.Next());
      std::vector<StoodCall> calls;
      g_calls = &calls;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = StandIn;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = RecordDelete;
      const bool flag = ITC_FLAG(native_power_graph_screen);
      if (v == 0) {
        ITC_FLAG(native_power_graph_screen) = false;
        f.recompiled(t, base);
      } else if (f.kind == Kind::kDraw || f.kind == Kind::kUpdate) {
        // The pages and steps from the tables recompiled on both sides.
        ITC_FLAG(native_power_graph_screen) = false;
        f.native(t, base);
      } else {
        f.native(t, base);
      }
      ITC_FLAG(native_power_graph_screen) = flag;
      hooks::g_start_up_recorder = recorder;
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      g_calls = nullptr;
      out[v].Word(uint32_t(calls.size()));
      for (const StoodCall& call : calls) {
        for (const uint32_t word : {call.function, call.lr, call.r1, uint32_t(call.r3),
                                    uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                    uint32_t(call.r4 >> 32)}) {
          out[v].Word(word);
        }
      }
      uint64_t r3 = t.r3.u64;
      if (f.kind == Kind::kOnEnter) {
        // The blocks made: their written words (the menu's +44, +56 and
        // +60 and the page object's +16 not written; the data's first 8)
        // compared, freed, their globals 1; r3 (the data block) as 1.
        static constexpr uint32_t kWritten[3] = {28, 68, 8};
        const uint32_t globals[3] = {pgs::kPages, pgs::kMenu, pgs::kData};
        for (uint32_t k = 0; k < 3; k++) {
          const uint32_t made = guest::LoadU32(base, globals[k]);
          const bool ours = made != 0 && memory::IsPages(made);
          out[v].Word(ours ? 1 : made);
          if (ours) {
            std::vector<uint8_t> bytes = Read(base, made, kWritten[k]);
            if (k == 1) {
              for (const uint32_t o : {44u, 56u, 60u}) {
                for (uint32_t i = 0; i < 4; i++) bytes[o + i] = 0;
              }
            } else if (k == 0) {
              for (uint32_t i = 0; i < 4; i++) bytes[16 + i] = 0;
            }
            out[v].Bytes(bytes);
            if (uint32_t(r3) == made) r3 = 1;
            memory::FreePages(made);
            guest::StoreU32(base, globals[k], 1);
          }
        }
      }
      if (f.kind == Kind::kOnLeft) {
        for (const uint32_t made : pages) {
          const bool still = memory::IsPages(made);
          out[v].Word(still ? 1 : 0);
          if (still) memory::FreePages(made);
        }
      }
      out[v].Bytes(Read(base, block, kBlockBytes));
      out[v].Bytes(Read(base, w.movies.address, w.movies.bytes));
      out[v].Bytes(Read(base, w.texts.address, w.texts.bytes));
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(base, g.address, g.bytes));
      }
      std::vector<uint8_t> stack = Read(base, sp - kBelow, kBelow + kAbove);
      for (const auto& [from, to] : f.skipped) {
        for (uint32_t o = kBelow - from; o < kBelow - to; o++) stack[o] = 0;
      }
      out[v].Bytes(stack);
      out[v].Word(t.r1.u32);
      out[v].Word(uint32_t(r3));
      out[v].Word(uint32_t(r3 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
      loading.Restore();
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void PowerGraphScreen(Context& h) {
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const lw::MovieWorld w;
  if (allocation == 0 || !w.ok()) {
    ITC_LOG_INFO("harness: power_graph_screen: no block");
    if (allocation != 0) memory::Free(allocation);
    return;
  }
  const uint32_t block = (allocation + 15) & ~15u;
  // The skipped stack: [r1 - from, r1 - to). Step0 runs SetTexts (128-byte
  // frame, 40 bytes of saves, varargs at its +48 to +79) below its own,
  // SetParts (144, 16) below that and a helper (112 or 144, 16 or 48)
  // below that.
  const Function functions[] = {
      {"PowerGraphScreen6::PostLeave", 0x82237A88, __imp__sub_82237A88,
       hooks::PowerGraphScreen6PostLeave, Kind::kSlot, {}},
      {"PowerGraphScreen6::PostClosing", 0x82237AD8, __imp__sub_82237AD8,
       hooks::PowerGraphScreen6PostClosing, Kind::kSlot, {}},
      {"PowerGraphScreen6::PostClosed", 0x82237B28, __imp__sub_82237B28,
       hooks::PowerGraphScreen6PostClosed, Kind::kSlot, {}},
      {"PowerGraphScreen7::PostLeave", 0x82246FB8, __imp__sub_82246FB8,
       hooks::PowerGraphScreen7PostLeave, Kind::kSlot, {}},
      {"PowerGraphScreen7::PostClosing", 0x82247008, __imp__sub_82247008,
       hooks::PowerGraphScreen7PostClosing, Kind::kSlot, {}},
      {"PowerGraphScreen67::OnEnter", 0x82250778, __imp__sub_82250778,
       hooks::PowerGraphScreenOnEnter, Kind::kOnEnter, {{24, 0}}},
      {"PowerGraphScreen67::WaitForLoad", 0x82250D70, __imp__sub_82250D70,
       hooks::PowerGraphScreenWaitForLoad, Kind::kHandler, {{16, 0}}},
      {"PowerGraphScreen67::Shown", 0x82250338, __imp__sub_82250338,
       hooks::PowerGraphScreenShown, Kind::kHandler, {}},
      {"PowerGraphScreen67::Open", 0x82250890, __imp__sub_82250890, hooks::PowerGraphScreenOpen,
       Kind::kHandler, {}},
      {"PowerGraphScreen67::Leave", 0x82250900, __imp__sub_82250900,
       hooks::PowerGraphScreenLeave, Kind::kHandler, {{32, 0}}},
      {"PowerGraphScreen67::MenuPage", 0x82250948, __imp__sub_82250948,
       hooks::PowerGraphScreenMenuPage, Kind::kPage, {{32, 0}}},
      {"PowerGraphScreen67::ClosedPage", 0x82250348, __imp__sub_82250348,
       hooks::PowerGraphScreenClosedPage, Kind::kPage, {}},
      {"PowerGraphScreen67::OnLeft", 0x822502C0, __imp__sub_822502C0,
       hooks::PowerGraphScreenOnLeft, Kind::kOnLeft, {{32, 0}}},
      {"PowerGraphScreen67::SetPartIcon", 0x82250B48, __imp__sub_82250B48,
       hooks::PowerGraphScreenSetPartIcon, Kind::kIcon, {{16, 0}}},
      {"PowerGraphScreen67::SetPartRow2", 0x82250BA0, __imp__sub_82250BA0,
       hooks::PowerGraphScreenSetPartRow2, Kind::kRow, {{48, 0}}},
      {"PowerGraphScreen67::SetPartRow1", 0x82250C40, __imp__sub_82250C40,
       hooks::PowerGraphScreenSetPartRow1, Kind::kRow, {{48, 0}}},
      {"PowerGraphScreen67::SetParts", 0x82250CD0, __imp__sub_82250CD0,
       hooks::PowerGraphScreenSetParts, Kind::kSetParts, {{16, 0}, {192, 144}}},
      {"PowerGraphScreen67::SetTexts", 0x82250DD8, __imp__sub_82250DD8,
       hooks::PowerGraphScreenSetTexts, Kind::kSetTexts,
       {{40, 0}, {80, 48}, {144, 128}, {320, 272}}},
      {"PowerGraphScreen67::Step0", 0x82251320, __imp__sub_82251320, hooks::PowerGraphScreenStep0,
       Kind::kStep, {{32, 0}, {168, 128}, {208, 176}, {272, 256}, {448, 400}}},
      {"PowerGraphScreen67::Step1", 0x82250AA8, __imp__sub_82250AA8, hooks::PowerGraphScreenStep1,
       Kind::kStep, {{24, 0}}},
      // Draw and Update: the pages and steps recompiled below (their saves of
      // the caller's registers; Step0's SetTexts' varargs words, SetParts'
      // and its helpers' saves).
      {"PowerGraphScreen67::Draw", 0x82250A20, __imp__sub_82250A20, hooks::PowerGraphScreenDraw,
       Kind::kDraw, {{8, 0}}},
      {"PowerGraphScreen67::Update", 0x822513B8, __imp__sub_822513B8,
       hooks::PowerGraphScreenUpdate, Kind::kUpdate,
       {{32, 0}, {176, 144}, {352, 320}, {416, 400}, {592, 544}}},
  };
  for (const Function& f : functions) {
    Run(h, block, w, f, f.kind == Kind::kSlot ? 32 : 96);
  }
  memory::Free(allocation);
}

}  // namespace harness
