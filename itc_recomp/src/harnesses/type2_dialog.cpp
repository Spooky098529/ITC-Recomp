// The type 2 dialog (src/type2_dialog/, docs/functions/type2-dialog.md)
// against the recompiled functions. Each case fills a world of its own (one
// block from the allocation layer) with random words from the case's seed:
// the dialog's machine, the mode with its message ring, the page object, the
// menu and the data (the globals 0x826D83F8 to 0x826D8400 pointed at them,
// put back after), the players' action mappers and a fake sound manager (as
// the type 6 dialog's harness shapes them), and one UI movie with 8 nodes,
// named animations and a few playing slots, named 16 to 19 in a movie table
// of our own among other ids. It shapes the words each function tests (the
// page, the step, the queued step and state, the cursor, the status words),
// runs the original (with --native_type2_dialog off) or the register steps
// (hooks/type2_dialog.h), and compares, exactly (nothing here is a float):
// the calls to UiMovie::Run's update (sub_8211D2F8) and
// UiMovie::ApplyAnimation (sub_825A08E0), stood in for and answered from the
// case's generator; the block; the globals; the stack below the call (but
// the original's register saves, and below Update's frame the step's saves
// of the Update's registers); the caller's area above r1; r1 and r3. Draw and
// Update run the pages and steps from their tables recompiled on both sides
// (their own cases are here too).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/type2_dialog.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "type2_dialog/type2_dialog.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type2_dialog);

namespace harness {

namespace {

namespace t2d = type2_dialog;

// The block's layout.
constexpr uint32_t kMachineAt = 0x0;        // the machine (9,620 bytes)
constexpr uint32_t kModeAt = 0x2600;        // the mode (its ring at +204)
constexpr uint32_t kPagesAt = 0x2A00;       // the page object (28 bytes)
constexpr uint32_t kMenuAt = 0x2A40;        // the menu (68)
constexpr uint32_t kDataAt = 0x2AC0;        // the data (68)
constexpr uint32_t kPairsAt = 0x2B40;       // the movie table's pairs (8)
constexpr uint32_t kMappersAt = 0x2C00;     // 4 mappers of 0x200
constexpr uint32_t kVtablesAt = 0x3400;     // their vtables (0x40 each)
constexpr uint32_t kManagerAt = 0x3500;     // the fake sound manager
constexpr uint32_t kManagerVtableAt = 0x3580;
constexpr uint32_t kMovieAt = 0x4000;       // the movie (0xA100 bytes)
constexpr uint32_t kNodesAt = 0xE400;       // 8 nodes of 0x800
constexpr uint32_t kNodeBytes = 0x800;
constexpr uint32_t kNodes = 8;
constexpr uint32_t kAnimationsAt = 0x12400;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kBlockBytes = 0x12800;

constexpr uint32_t kBelow = 1024;  // the stack compared below the call's r1
constexpr uint32_t kAbove = 256;   // and above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {t2d::kPages, 12},  // the page object, the menu and the data
    {t2d::kShowing, 8},  // the dialog showing and its status
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

// What a case shapes and passes.
// kHandler: r3 the machine, r4 the mode; kLeaf: no arguments read;
// kOnEnter: the blocks VirtualAlloc makes compared by content where their
// pointers were stored, then freed; kOnLeft: the globals pointing at blocks
// of the layer's pages the case made, whether each was freed compared.
enum class Kind { kStep, kPage, kDraw, kUpdate, kHandler, kLeaf, kOnEnter, kOnLeft };

struct Function {
  const char* name;
  uint32_t address;
  uint32_t frame;  // the original's frame
  uint32_t saves;  // its register saves at the frame's top
  GuestFunction recompiled, native;
  Kind kind;
  // The callee's saves below the frame (Update's steps save the Update's
  // registers, which the native one doesn't hold).
  uint32_t callee_saves = 0;
};

void MakeWorld(uint8_t* base, uint32_t block, Random& r, Kind kind) {
  auto at = [&](uint32_t offset) { return block + offset; };
  WriteRandom(base, block, kBlockBytes, r);
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
  // The data: loaded, the queued state, the movie shown.
  const uint32_t data = at(kDataAt);
  guest::StoreU32(base, data + 0, r.Below(2));
  static constexpr uint32_t kStates[] = {21, 21, 9, 9, 4, 5, 6};
  guest::StoreU32(base, data + 4, kStates[r.Below(7)]);
  guest::StoreU32(base, data + 8, 16 + r.Below(4));
  // The machine: loaded, re-entered.
  guest::StoreU32(base, at(kMachineAt) + t2d::kMachineLoaded,
                  r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, at(kMachineAt) + t2d::kMachineReenter,
                  r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  // The mode's ring: its count and start small.
  guest::StoreU32(base, at(kModeAt) + 204 + 600, r.Below(50));
  guest::StoreU32(base, at(kModeAt) + 204 + 604, r.Below(60));
  // The globals.
  guest::StoreU32(base, t2d::kPages, pages);
  guest::StoreU32(base, t2d::kMenu, menu);
  guest::StoreU32(base, t2d::kData, data);
  guest::StoreU32(base, t2d::kShowing, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(4));
  static constexpr uint32_t kStatus[] = {4, 4, 1, 5, 3, 0, 2, 0xFFFFFFFFu};
  guest::StoreU32(base, t2d::kStatus, kStatus[r.Below(8)]);
  (void)kind;
}

void Run(Context& h, uint32_t block, const Function& f, int cases) {
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
      MakeWorld(base, block, r, f.kind);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      std::vector<uint32_t> pages;
      WriteRandom(base, sp - kBelow, kBelow + kAbove, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      const uint32_t machine = block + kMachineAt, mode = block + kModeAt;
      const uint64_t high = uint64_t(r.Below(2)) << 32;
      switch (f.kind) {
        case Kind::kStep:
          // The started word: the page object's +24 (as Update passes it).
          t.r3.u64 = (block + kPagesAt + 24) | high;
          break;
        case Kind::kPage:
          t.r3.u64 = mode | high;
          break;
        case Kind::kDraw:
        case Kind::kUpdate:
        case Kind::kHandler:
        case Kind::kOnEnter:
          t.r3.u64 = machine | high;
          t.r4.u64 = mode;
          break;
        case Kind::kLeaf:
          break;
        case Kind::kOnLeft:
          // The blocks to free: the layer's pages (none now and then).
          for (const uint32_t global : {t2d::kPages, t2d::kMenu, t2d::kData}) {
            uint32_t made = 0;
            if (r.Below(6) != 0) {
              made = memory::AllocPages(global == t2d::kPages ? 28 : 68, 4096);
              WriteRandom(base, made, 64, r);
              pages.push_back(made);
            }
            guest::StoreU32(base, global, made);
          }
          t.r3.u64 = machine | high;
          break;
      }
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_answers = Random(r.Next());
      std::vector<DialogCall> calls;
      g_calls = &calls;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = StandIn;
      const bool flag = ITC_FLAG(native_type2_dialog);
      if (v == 0) {
        ITC_FLAG(native_type2_dialog) = false;
        f.recompiled(t, base);
      } else if (f.kind == Kind::kDraw || f.kind == Kind::kUpdate) {
        // The pages and steps from the tables recompiled on both sides.
        ITC_FLAG(native_type2_dialog) = false;
        f.native(t, base);
      } else {
        f.native(t, base);
      }
      ITC_FLAG(native_type2_dialog) = flag;
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      g_calls = nullptr;
      out[v].Word(uint32_t(calls.size()));
      for (const DialogCall& call : calls) {
        for (const uint32_t w : {call.function, call.lr, call.r1, uint32_t(call.r3),
                                 uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                 uint32_t(call.r4 >> 32)}) {
          out[v].Word(w);
        }
      }
      uint64_t r3 = t.r3.u64;
      if (f.kind == Kind::kOnEnter) {
        // The blocks made: their first words (the rest not written)
        // compared, freed, their globals 1; r3 (the data block) as 1.
        static constexpr uint32_t kWritten[3] = {28, 68, 12};
        const uint32_t globals[3] = {t2d::kPages, t2d::kMenu, t2d::kData};
        for (uint32_t k = 0; k < 3; k++) {
          const uint32_t made = guest::LoadU32(base, globals[k]);
          const bool ours = made != 0 && memory::IsPages(made);
          out[v].Word(ours ? 1 : made);
          if (ours) {
            std::vector<uint8_t> bytes = Read(base, made, kWritten[k]);
            if (k == 1) {
              // The menu's +44, +56 and +60, not written (the layer's
              // pages' contents).
              for (const uint32_t at : {44u, 56u, 60u}) {
                for (uint32_t i = 0; i < 4; i++) bytes[at + i] = 0;
              }
            } else if (k == 0) {
              for (uint32_t i = 0; i < 4; i++) bytes[16 + i] = 0;  // +16 not written
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
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(base, g.address, g.bytes));
      }
      const uint32_t below = kBelow - f.frame - f.callee_saves;
      out[v].Bytes(Read(base, sp - kBelow, below));
      out[v].Bytes(Read(base, sp - f.frame, f.frame - f.saves));
      out[v].Bytes(Read(base, sp, kAbove));
      out[v].Word(t.r1.u32);
      out[v].Word(uint32_t(r3));
      out[v].Word(uint32_t(r3 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void Type2Dialog(Context& h) {
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  if (allocation == 0) {
    ITC_LOG_INFO("harness: type2_dialog: no block");
    return;
  }
  const uint32_t block = (allocation + 15) & ~15u;
  const Function functions[] = {
      {"Type2Dialog::OpenConfirmStep", 0x82187728, 128, 32, __imp__sub_82187728,
       hooks::Type2DialogOpenConfirmStep, Kind::kStep},
      {"Type2Dialog::CloseConfirmStep", 0x821877F8, 112, 24, __imp__sub_821877F8,
       hooks::Type2DialogCloseConfirmStep, Kind::kStep},
      {"Type2Dialog::OpenNoticeStep", 0x821878B0, 112, 24, __imp__sub_821878B0,
       hooks::Type2DialogOpenNoticeStep, Kind::kStep},
      {"Type2Dialog::CloseNoticeStep", 0x82187948, 112, 24, __imp__sub_82187948,
       hooks::Type2DialogCloseNoticeStep, Kind::kStep},
      {"Type2Dialog::OpenConfirm2Step", 0x821879F8, 128, 32, __imp__sub_821879F8,
       hooks::Type2DialogOpenConfirm2Step, Kind::kStep},
      {"Type2Dialog::CloseConfirm2Step", 0x82187AC8, 112, 24, __imp__sub_82187AC8,
       hooks::Type2DialogCloseConfirm2Step, Kind::kStep},
      {"Type2Dialog::OpenNotice2Step", 0x82187B80, 112, 24, __imp__sub_82187B80,
       hooks::Type2DialogOpenNotice2Step, Kind::kStep},
      {"Type2Dialog::CloseNotice2Step", 0x82187C18, 112, 24, __imp__sub_82187C18,
       hooks::Type2DialogCloseNotice2Step, Kind::kStep},
      {"Type2Dialog::MoveCursorStep", 0x82187CC8, 128, 40, __imp__sub_82187CC8,
       hooks::Type2DialogMoveCursorStep, Kind::kStep},
      {"Type2Dialog::MoveCursor2Step", 0x82187D80, 128, 40, __imp__sub_82187D80,
       hooks::Type2DialogMoveCursor2Step, Kind::kStep},
      {"Type2Dialog::ConfirmPage", 0x82187160, 144, 64, __imp__sub_82187160,
       hooks::Type2DialogConfirmPage, Kind::kPage},
      {"Type2Dialog::NoticePage", 0x82187508, 112, 24, __imp__sub_82187508,
       hooks::Type2DialogNoticePage, Kind::kPage},
      {"Type2Dialog::Confirm2Page", 0x82187330, 112, 32, __imp__sub_82187330,
       hooks::Type2DialogConfirm2Page, Kind::kPage},
      {"Type2Dialog::Notice2Page", 0x82187618, 112, 24, __imp__sub_82187618,
       hooks::Type2DialogNotice2Page, Kind::kPage},
      {"Type2Dialog::Draw", 0x82187E50, 112, 8, __imp__sub_82187E50, hooks::Type2DialogDraw,
       Kind::kDraw},
      {"Type2Dialog::Update", 0x82187EC8, 208, 64, __imp__sub_82187EC8,
       hooks::Type2DialogUpdate, Kind::kUpdate, 40},
      {"Type2Dialog::OnEnter", 0x821869B8, 128, 40, __imp__sub_821869B8,
       hooks::Type2DialogOnEnter, Kind::kOnEnter},
      {"Type2Dialog::WaitForMovies", 0x821880D0, 96, 16, __imp__sub_821880D0,
       hooks::Type2DialogWaitForMovies, Kind::kHandler},
      {"Type2Dialog::Shown", 0x82186490, 0, 0, __imp__sub_82186490, hooks::Type2DialogShown,
       Kind::kLeaf},
      {"Type2Dialog::Wait", 0x82186AD8, 0, 0, __imp__sub_82186AD8, hooks::Type2DialogWait,
       Kind::kHandler},
      {"Type2Dialog::Cancel", 0x82186E40, 40, 40, __imp__sub_82186E40, hooks::Type2DialogCancel,
       Kind::kHandler},
      {"Type2Dialog::OpenConfirm", 0x82186C30, 112, 24, __imp__sub_82186C30,
       hooks::Type2DialogOpenConfirm, Kind::kHandler},
      {"Type2Dialog::OpenNotice", 0x82186AF0, 0, 0, __imp__sub_82186AF0,
       hooks::Type2DialogOpenNotice, Kind::kHandler},
      {"Type2Dialog::OpenConfirm2", 0x82186D40, 112, 16, __imp__sub_82186D40,
       hooks::Type2DialogOpenConfirm2, Kind::kHandler},
      {"Type2Dialog::OpenNotice2", 0x82186B90, 0, 0, __imp__sub_82186B90,
       hooks::Type2DialogOpenNotice2, Kind::kHandler},
      {"Type2Dialog::OnLeft", 0x82186428, 112, 32, __imp__sub_82186428,
       hooks::Type2DialogOnLeft, Kind::kOnLeft},
      {"Type2Dialog::TakesInput", 0x821864A0, 0, 0, __imp__sub_821864A0,
       hooks::Type2DialogTakesInput, Kind::kLeaf},
  };
  for (const Function& f : functions) {
    Run(h, block, f, f.kind == Kind::kStep ? 32 : 128);
  }
  memory::Free(allocation);
}

}  // namespace harness
