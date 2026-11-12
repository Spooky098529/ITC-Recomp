// The type 12 mode's mode select (NetModeSelect::, machine 0, UI movie 35:
// src/network_mode/mode_select.cpp, docs/functions/network-mode.md)
// against the recompiled functions, under the network_mode harness. The
// world is laid as the type 10 mode's harness lays its own
// (type10_mode.cpp): each case fills one block from the allocation layer
// with random words from the case's seed (the mode with its message ring,
// the mode select's machine, its page object, menu and data, screen 1's
// data, a session, movie 35 over a pool of 40 nodes (each child of a node
// another of the pool, all TXTs) and the movie table's pair naming it, the
// players' action mappers and the sound manager, with a string table of our
// own); points the globals into it (put back after); shapes the fields
// each function tests with play's ranges (the page -1 or 0 to 8, the
// transition -1 or 0 to 4, the menu's items 0 and 1); runs the original
// (with --native_network_mode off, so what it calls of the mode's is the
// original too) or the register steps (hooks/network_mode.h); and compares
// the world, the globals, the function's frames without their register
// saves (and without the text setter's parameter area, +48 to +79, where
// its unset varargs land: the user's call of 2026-10-30), the caller's area
// above r1, r1 and r3. UiMovie::Run's update is stood in for (each call's
// return address, stack pointer and arguments kept, answered 0 or 1 from
// the case's generator); UiMovies::Delete's calls are recorded on both
// sides and answered 1; a session Session::Make allocates (the layer's
// pages) is compared by its words and freed.
//
// The Xbox Live calls plugged in: the Live users' mask (0 in play) is given
// bits now and then, so BattlePage and TimeAttackPage reach the privilege
// check: the recompiled side calls sub_8214F620, which goes through the
// SDK's XamGetSystemVersion, XamUserGetSigninState and
// XamUserCheckPrivilege and answers 0 for every user, the native side uses
// that answer (kPrivilege); XShowSigninUI, which the native sign-in pages
// don't call, is our override's 0 on the recompiled side (its answer the r3
// both leave).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/network_mode.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "network_mode/network_mode.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_network_mode);

namespace harness {

namespace {

namespace nm = network_mode;

// The block's layout.
constexpr uint32_t kModeAt = 0x0;            // the mode (852 bytes; its ring at +240)
constexpr uint32_t kMachineAt = 0x400;       // the mode select's machine (9,640 bytes)
constexpr uint32_t kPagesAt = 0x2A00;        // [0x82816190] (28 bytes)
constexpr uint32_t kMenuAt = 0x2A40;         // [0x82816194] (68 bytes)
constexpr uint32_t kDataAt = 0x2A90;         // [0x82816198] (36 bytes)
constexpr uint32_t kScreen1DataAt = 0x2AC0;  // [0x82816180] (its +8)
constexpr uint32_t kPairsAt = 0x2D00;        // the movie table's pair
constexpr uint32_t kMenuWorldAt = 0x2E00;    // the sound manager and its vtable (+0x400)
constexpr uint32_t kMovieAt = 0x3400;        // movie 35 (0xA100 bytes)
constexpr uint32_t kNodesAt = 0xD800;        // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x17800;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // the movie's named animations shaped
constexpr uint32_t kMappersAt = 0x17C00;     // 4 action mappers of 0x200
constexpr uint32_t kMapperVtablesAt = 0x18400;  // their vtables (0x40 each)
constexpr uint32_t kBlockBytes = 0x18600;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
constexpr uint32_t kFades = 0x826A50AC;
constexpr uint32_t kFadeCount0 = 0x826A50E4;
constexpr uint32_t kFadeCount1 = 0x826A50DC;
constexpr uint32_t kMoviesDelete = 0x8211E258;
// The session's words Session::Init writes (to +503) and the two at
// +0x13CD30.
constexpr uint32_t kSessionWords = 512;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {0x82816180, 0x34},  // screen 1's data to dialog 12's reason (the mode select's blocks)
    {nm::kSession, 4},
    {nm::kSessionState, 0x30},  // to the session buffer (0x826C6AD4)
    {nm::kLiveMask, 12},        // the Live mask and the system UI's words
    {nm::kSignInWatch + 16, 4},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {kFades, 64},
    {race_screens::kMapperEntries, 32},
    {race_screens::kControllerEntries, 16},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u), strings(memory::Alloc(kStringsBytes, 0)),
        session(memory::AllocPages(nm::kSessionBytes, 4096)) {
    if (strings != 0) {
      MakeStrings();
    }
  }
  ~World() {
    memory::Free(allocation);
    memory::Free(strings);
    if (session != 0) {
      memory::FreePages(session);
    }
  }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0 && strings != 0 && session != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint32_t machine() const { return at(kMachineAt); }
  uint32_t mode() const { return at(kModeAt); }
  uint32_t dispatcher() const { return mode() + nm::kDispatcher; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  // A string table of our own ([0x826C5C34] pointed at it): its 18 files
  // all one container of 8,192 entries over 16 strings of letters, so any id
  // below 8,192 has a string; no argument codes (the varargs the native
  // callers pass as 0 aren't read).
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;
  // A session's pages ([0x82816E3C] in play once MenuPage made it): the
  // words Session::Init writes random per case and compared.
  uint32_t session;

 private:
  void MakeStrings() {
    Random r(0x4E65744D6F6465ull);  // "NetMode"
    const uint32_t container = strings + 128, text = strings + 128 + 4 + 4 * 8192;
    for (uint32_t k = 0; k < 18; k++) {
      guest::StoreU32(base, strings + 4 * k, container);
    }
    guest::StoreU32(base, container, __builtin_bswap32(8192));
    for (uint32_t k = 0; k < 8192; k++) {
      const uint32_t at = text + 64 * (k % 16);
      guest::StoreU32(base, container + 4 + 4 * k, __builtin_bswap32(at - container));
    }
    for (uint32_t k = 0; k < 16; k++) {
      const uint32_t at = text + 64 * k;
      const uint32_t length = r.Below(30);
      for (uint32_t c = 0; c < length; c++) {
        guest::StoreU16(base, at + 2 * c, uint16_t('a' + r.Below(26)));
      }
      guest::StoreU16(base, at + 2 * length, 0);
    }
  }
};

// A word play gives a flag: 0 or 1 mostly, now and then any.
uint32_t FlagWord(Random& r) { return r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2); }

// The world as play has it: the nodes, the movie, the page object's,
// menu's and data's words in their ranges, the fades, the Live words, the
// machine's words, the players' action mappers and the sound manager.
constexpr uint32_t kHeldSlot = 0x8210BE70;
constexpr uint32_t kGet60 = 0x82090770;  // the getter (r3 = +60)
void MakeWorld(uint8_t* base, const World& w, Random& r) {
  namespace rs = race_screens;
  namespace mh = menu_helpers;
  WriteRandom(base, w.block, kBlockBytes, r);
  WriteRandom(base, w.session, kSessionWords, r);
  WriteRandom(base, w.session + nm::kSessionRows - 4, 8, r);
  // The mode's ring (the dispatcher's, +204): count and start small; no
  // handlers.
  guest::StoreU32(base, w.mode() + nm::kRing + 600, r.Below(40));
  guest::StoreU32(base, w.mode() + nm::kRing + 604, r.Below(60));
  guest::StoreU32(base, w.dispatcher() + 200, 0);
  // The nodes: all TXTs, each node's children the pool's.
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 0, kTxtTag);
    guest::StoreU32(base, node + 100, r.Below(30));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
  }
  // The scenes' animations: the 8 of no tracks.
  for (uint32_t n = 0; n < kNodeCount; n++) {
    guest::StoreU32(base, w.node(n) + 116, 40 + r.Below(8));
    for (uint32_t e = 40; e < 64; e++) {
      guest::StoreU32(base, w.node(n) + 128 + 8 * e,
                      w.at(kAnimationsAt) + 64 * r.Below(kAnimations));
    }
  }
  for (uint32_t k = 0; k < kAnimations; k++) {
    const uint32_t animation = w.at(kAnimationsAt) + 64 * k;
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, animation + i, 0);
    }
    guest::StoreFloat(base, animation + 12, float(r.Below(100)) / 4.0f);
  }
  // Movie 35 over the pool: its scene nodes node 0, the named animations
  // over the root; its loaded flag set or not.
  const uint32_t movie = w.at(kMovieAt);
  ui_movies::InitAnimationLists(base, movie);
  for (const uint32_t field : {16u, 20u, 24u}) {
    guest::StoreU32(base, movie + field, w.node(0));
  }
  for (uint32_t k = 0; k < kNamed; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, w.node(0));
    guest::StoreU32(base, named + 32, w.at(kAnimationsAt) + 64 * r.Below(kAnimations));
  }
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, movie + 40960, r.Below(3));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(2));
  // The root's own children none (+104 0).
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, w.at(kPairsAt), nm::kSelectMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 4, movie);
  // The page object: the page -1 or 0 to 8, the page after and before 0 to
  // 8, the transition -1 or 0 to 4 (now and then another negative), the
  // queued one -1 or 0 to 4, the started word 0 or not.
  const uint32_t pages = w.at(kPagesAt);
  guest::StoreU32(base, pages + 0, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(9));
  guest::StoreU32(base, pages + 4, r.Below(9));
  guest::StoreU32(base, pages + 8, r.Below(9));
  const uint32_t pick = r.Below(5);
  guest::StoreU32(base, pages + 12,
                  pick == 0   ? 0xFFFFFFFFu
                  : pick == 1 ? uint32_t(-int32_t(2 + r.Below(5)))
                              : r.Below(5));
  guest::StoreU32(base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(5));
  guest::StoreU32(base, pages + 24, r.Below(3) == 0 ? 1 : 0);
  // The menu (items 0 and 1), as the title screen's harness shapes it; its
  // move -1, 0 or 1.
  const uint32_t menu = w.at(kMenuAt);
  guest::StoreU32(base, menu + mh::kPlayer, r.Below(4));
  guest::StoreU32(base, menu + mh::kFirst, 0);
  guest::StoreU32(base, menu + mh::kLast, 1);
  guest::StoreU32(base, menu + mh::kCursor, r.Below(2));
  guest::StoreU32(base, menu + mh::kPrevious, r.Below(2));
  for (const uint32_t o : {mh::kWraps, mh::kUpDownLocked, mh::kSidesStep, mh::kShouldersStep}) {
    guest::StoreU32(base, menu + o, r.Below(2));
  }
  static constexpr uint32_t kMoves[] = {0xFFFFFFFFu, 0, 1};
  guest::StoreU32(base, menu + mh::kMoved, kMoves[r.Below(3)]);
  guest::StoreU32(base, menu + mh::kSide, 0);
  // The data: +0 to +16 flags, +20 a page 0 to 8, +24 -1 or a transition 0
  // to 4, +28 the count 0 to 12 (now and then any), +32 a flag.
  const uint32_t data = w.at(kDataAt);
  for (const uint32_t field : {0u, 4u, 8u, 12u, 16u, 32u}) {
    guest::StoreU32(base, data + field, FlagWord(r));
  }
  guest::StoreU32(base, data + 20, r.Below(9));
  guest::StoreU32(base, data + 24, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(5));
  guest::StoreU32(base, data + 28, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(13));
  // The fades' words: their counts 0 or not.
  WriteRandom(base, kFades, 64, r);
  for (const uint32_t count : {kFadeCount0, kFadeCount1}) {
    guest::StoreU32(base, count, r.Below(2) == 0 ? 0 : r.Below(30));
  }
  // The Live mask (0 in play) given bits now and then; the system UI's
  // words 0 mostly.
  guest::StoreU32(base, nm::kLiveMask, r.Below(2) == 0 ? 0 : r.Below(16));
  guest::StoreU32(base, nm::kSystemUi0, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, nm::kSystemUi1, r.Below(4) == 0 ? 1 : 0);
  // The machine's words: the state, its movie loaded and deleted flags.
  const uint32_t machine = w.machine();
  guest::StoreU32(base, machine + nm::kMachineState, r.Below(16));
  guest::StoreU32(base, machine + nm::kMachineLoaded, FlagWord(r));
  guest::StoreU32(base, machine + 9616, FlagWord(r));
  // The players' action mappers (each controller's entry 0 to 3, each
  // entry's mapper one of ours, its slots 10 and 12 Pressed and Fired, now
  // and then Held; blocked now and then; a few bits set).
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t mapper = w.at(kMappersAt) + 0x200 * c;
    const uint32_t vtable = w.at(kMapperVtablesAt) + 0x40 * c;
    guest::StoreU32(base, rs::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, rs::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, rs::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + 40, r.Below(5) == 0 ? kHeldSlot : rs::kPressedSlot);
    guest::StoreU32(base, vtable + 48, r.Below(5) == 0 ? kHeldSlot : rs::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
  // The sound manager: PlayCue2D (slot 10, MenuSound::Play's) the getter.
  const uint32_t manager = w.at(kMenuWorldAt), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, manager + 60,
                  r.Below(2) == 0 ? uint32_t(r.Next()) & ~0xFFu : uint32_t(r.Next()));
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, kGet60);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, nm::kSelectPages, w.at(kPagesAt));
  guest::StoreU32(base, nm::kSelectMenu, w.at(kMenuAt));
  guest::StoreU32(base, nm::kSelectData, w.at(kDataAt));
  guest::StoreU32(base, nm::kScreen1Data, w.at(kScreen1DataAt));
  // The session there (play's, once MenuPage made it), now and then none
  // (Session::Make makes one; TimeAttackPage isn't given none).
  guest::StoreU32(base, nm::kSession, r.Below(4) == 0 ? 0 : w.session);
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, w.at(kPairsAt) + 8);
  // Now and then the pair not there (UiMovies::Find answers the missing
  // movie, ours).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 1);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
}

// UiMovies::Delete's calls, recorded on both sides and answered 1 (Delete
// would free our movie). Other addresses go to the recorder set before the
// run, when there's one.
std::vector<uint32_t> g_deletes;
hooks::StartUpRecorder g_chained = nullptr;
uint32_t g_case_sp = 0;
bool RecordDelete(PPCContext& ctx, uint32_t address) {
  if (address != kMoviesDelete) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  g_deletes.insert(g_deletes.end(), {ctx.r3.u32, g_case_sp - ctx.r1.u32, uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

// The text records the setters made (stored in the movie's map): their
// words and buffers compared, freed, their map words 0.
void TextRecords(uint8_t* base, uint32_t movie, Outputs& out) {
  const uint32_t map = movie + ui_movies::kMovieTexts;
  const uint32_t pairs = guest::LoadU32(base, map + 1024);
  for (uint32_t k = 0; k < pairs && k < 128; k++) {
    const uint32_t value = guest::LoadU32(base, map + 8 * k + 4);
    if (value != 0 && memory::IsHeapBlock(value)) {
      const uint32_t buffer = guest::LoadU32(base, value);
      out.Word(k);
      out.Bytes(Read(base, value + 4, 24));
      out.Word(memory::IsPages(buffer) ? 1 : 0);
      if (memory::IsPages(buffer)) {
        out.Bytes(Read(base, buffer, 4096));
        memory::FreePages(buffer);
      }
      memory::crt::OperatorDelete(value);
      guest::StoreU32(base, map + 8 * k + 4, 0);
    }
  }
}

// A session Session::Make allocated (the layer's pages, not the world's): its words up to
// +504 and +0x13CD30 to +0x13CD37 compared, freed, the global 1.
void SessionMade(uint8_t* base, uint32_t world_session, Outputs& out) {
  const uint32_t session = guest::LoadU32(base, nm::kSession);
  if (session == 0 || session == world_session || !memory::IsPages(session)) {
    return;
  }
  out.Word(1);
  out.Bytes(Read(base, session, 504));
  out.Bytes(Read(base, session + 0x13CD30, 8));
  memory::FreePages(session);
  guest::StoreU32(base, nm::kSession, 1);
}

// UiMovie::Run's update (sub_8211D2F8), stood in for: each call's return
// address, stack pointer (from the case's), r3 and r4 kept and answered 0
// or 1 from the case's generator (the update would draw the movie).
struct UpdateCall {
  uint32_t lr, r1;
  uint64_t r3, r4;
};
std::vector<UpdateCall> g_updates;
Random g_answers;
bool UpdateStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (function != 0x8211D2F8) {
    return false;
  }
  g_updates.push_back({uint32_t(ctx.lr), g_case_sp - ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_answers.Below(2);
  return true;
}

// A stack range compared, relative to the call's r1: [from, to).
struct Range {
  int32_t from;
  int32_t to;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

struct Spec {
  std::string name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1 (frames without their saves)
  SetupFn setup;
};

void Run(Context& h, const World& w, const Spec& spec) {
  Results results(spec.name);
  for (int c = 0; c < spec.cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(h.base, g.address, g.bytes));
      }
      MakeWorld(h.base, w, r);
      PointGlobals(h.base, w, r);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_deletes.clear();
      g_updates.clear();
      g_answers = Random(r.Next());
      g_case_sp = sp;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      g_chained = recorder;
      hooks::g_start_up_recorder = RecordDelete;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      if (v == 0) {
        const bool native = ITC_FLAG(native_network_mode);
        ITC_FLAG(native_network_mode) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_network_mode) = native;
      } else {
        spec.native(t, h.base);
      }
      hooks::g_start_up_recorder = recorder;
      g_chained = nullptr;
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      out[v].Word(uint32_t(g_updates.size()));
      for (const UpdateCall& call : g_updates) {
        out[v].Word(call.lr);
        out[v].Word(call.r1);
        out[v].Word(uint32_t(call.r3 >> 32));
        out[v].Word(uint32_t(call.r3));
        out[v].Word(uint32_t(call.r4 >> 32));
        out[v].Word(uint32_t(call.r4));
      }
      out[v].Word(uint32_t(g_deletes.size()));
      for (const uint32_t word : g_deletes) {
        out[v].Word(word);
      }
      // Session::Make's r3 (the new session + 248) by its offset.
      const uint32_t made = guest::LoadU32(h.base, nm::kSession);
      if (made != 0 && made != w.session && memory::IsPages(made) &&
          t.r3.u32 == made + 248) {
        t.r3.u64 = (t.r3.u64 & ~uint64_t(0xFFFFFFFF)) | 0x248;
      }
      SessionMade(h.base, w.session, out[v]);
      TextRecords(h.base, w.at(kMovieAt), out[v]);
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
      out[v].Bytes(Read(h.base, w.session, kSessionWords));
      out[v].Bytes(Read(h.base, w.session + nm::kSessionRows - 4, 8));
      for (const Range& range : spec.stack) {
        out[v].Bytes(Read(h.base, sp + range.from, uint32_t(range.to - range.from)));
      }
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// r3 the machine (now and then with high bits), r4 the dispatcher.
void Machine(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = w.machine() | (uint64_t(r.Below(2)) << 32);
  t.r4.u64 = w.dispatcher();
}
// r3 the dispatcher.
void Dispatcher(PPCContext& t, Random&, const World& w) { t.r3.u64 = w.dispatcher(); }
// The session there (TimeAttackPage writes its +228 unchecked).
void WithSession(PPCContext& t, Random& r, const World& w) {
  guest::StoreU32(w.base, nm::kSession, w.session);
  Dispatcher(t, r, w);
}
// r3 the started word (the page object's +24): 0 mostly, 1 or any.
void Started(PPCContext& t, Random& r, const World& w) {
  const uint32_t at = w.at(kPagesAt) + 24;
  guest::StoreU32(w.base, at, r.Below(3) == 0 ? 1 : r.Below(5) == 0 ? uint32_t(r.Next()) : 0);
  t.r3.u64 = at;
}
void Nothing(PPCContext&, Random&, const World&) {}

void ModeSelect(Context& h, const World& w) {
  // Start with SetUp: their frames without the saves and SetUp's text
  // setter's parameter area.
  Run(h, w, {"NetModeSelect::Start (sub_8227D300; sub_8227CCE0)", 100, __imp__sub_8227D300,
             hooks::NetModeSelectStart, {{-96, -8}, {-224, -176}, {-144, -128}}, Machine});
  Run(h, w, {"NetModeSelect::SetUp (sub_8227CCE0)", 60, __imp__sub_8227CCE0,
             hooks::NetModeSelectSetUp, {{-128, -80}, {-48, -32}}, Nothing});
  Run(h, w, {"NetModeSelect::Open (sub_8227C5C8)", 100, __imp__sub_8227C5C8,
             hooks::NetModeSelectOpen, {}, Machine});
  // Draw: the page -1 or 0 to 8 (each page has its own cases too, run here
  // through Draw's table with the session there); only its own frame
  // compared.
  Run(h, w, {"NetModeSelect::Draw (sub_8227D248)", 400, __imp__sub_8227D248,
             hooks::NetModeSelectDraw, {{-160, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               guest::StoreU32(w.base, nm::kSession, w.session);
               Machine(t, r, w);
             }});
  // Update: the running transition -1, another negative or 0 to 4 (the
  // world's); only its own frame compared.
  Run(h, w, {"NetModeSelect::Update (sub_8227D4E8)", 400, __imp__sub_8227D4E8,
             hooks::NetModeSelectUpdate, {{-160, -40}}, Machine});
  // MenuPage with Session::Make and Session::Init below it.
  Run(h, w, {"NetModeSelect::MenuPage (sub_8227C658; sub_822926B8, sub_822915C0)", 400,
             __imp__sub_8227C658, hooks::NetModeSelectMenuPage,
             {{-112, -32}, {-208, -128}, {-320, -240}}, Dispatcher});
  Run(h, w, {"NetModeSelect::LeavePage (sub_8227C970)", 60, __imp__sub_8227C970,
             hooks::NetModeSelectLeavePage, {}, Dispatcher});
  Run(h, w, {"NetModeSelect::BattlePage (sub_8227C9F0)", 300, __imp__sub_8227C9F0,
             hooks::NetModeSelectBattlePage, {{-112, -24}}, Dispatcher});
  Run(h, w, {"NetModeSelect::TimeAttackPage (sub_8227CB38)", 300, __imp__sub_8227CB38,
             hooks::NetModeSelectTimeAttackPage, {{-112, -24}}, WithSession});
  Run(h, w, {"NetModeSelect::SignInPage (sub_8227C088)", 300, __imp__sub_8227C088,
             hooks::NetModeSelectSignInPage, {{-96, -16}}, Dispatcher});
  Run(h, w, {"NetModeSelect::SignInPage2 (sub_8227C210)", 300, __imp__sub_8227C210,
             hooks::NetModeSelectSignInPage2, {{-96, -16}}, Dispatcher});
  Run(h, w, {"NetModeSelect::BackPage (sub_8227CC78)", 60, __imp__sub_8227CC78,
             hooks::NetModeSelectBackPage, {}, Dispatcher});
  Run(h, w, {"NetModeSelect::Intro (sub_8227CE50)", 200, __imp__sub_8227CE50,
             hooks::NetModeSelectIntro, {{-128, -32}}, Started});
  Run(h, w, {"NetModeSelect::ShowCursor (sub_8227D138)", 150, __imp__sub_8227D138,
             hooks::NetModeSelectShowCursor, {{-112, -24}}, Started});
  // MoveCursor: without SetText's parameter area and its saves (r26 on);
  // SetCursorY's frame below without its saves (r31, f31).
  Run(h, w, {"NetModeSelect::MoveCursor (sub_8227D380; sub_8227D1D8)", 300, __imp__sub_8227D380,
             hooks::NetModeSelectMoveCursor, {{-144, -96}, {-64, -56}, {-256, -168}}, Started});
  // SetCursorY: r3 0 or 20 (play's), now and then any.
  Run(h, w, {"NetModeSelect::SetCursorY (sub_8227D1D8)", 100, __imp__sub_8227D1D8,
             hooks::NetModeSelectSetCursorY, {{-112, -24}},
             [](PPCContext& t, Random& r, const World&) {
               t.r3.u64 = r.Below(4) == 0 ? r.Next() : (r.Below(2) == 0 ? 0 : 20);
             }});
  Run(h, w, {"NetModeSelect::FadeOut (sub_8227D088)", 150, __imp__sub_8227D088,
             hooks::NetModeSelectFadeOut, {{-112, -24}}, Started});
  Run(h, w, {"NetModeSelect::FadeIn (sub_8227CFA8)", 150, __imp__sub_8227CFA8,
             hooks::NetModeSelectFadeIn, {{-128, -32}}, Started});
  Run(h, w, {"NetModeSelect::OnNo (sub_8227BF80)", 40, __imp__sub_8227BF80,
             hooks::NetModeSelectOnNo, {}, Machine});
  Run(h, w, {"NetModeSelect::OnYes (sub_8227BFB8)", 20, __imp__sub_8227BFB8,
             hooks::NetModeSelectOnYes, {}, Machine});
  Run(h, w, {"NetModeSelect::OnDialogClosed (sub_8227BFD8)", 150, __imp__sub_8227BFD8,
             hooks::NetModeSelectOnDialogClosed, {}, Machine});
  Run(h, w, {"NetModeSelect::Unload (sub_8227C7A8)", 60, __imp__sub_8227C7A8,
             hooks::NetModeSelectUnload, {{-112, -16}}, Machine});
  // Unloaded: the menu's item 0 to 2 (2 never in play: the caller's word
  // posted).
  Run(h, w, {"NetModeSelect::Unloaded (sub_8227C810)", 100, __imp__sub_8227C810,
             hooks::NetModeSelectUnloaded, {},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.at(kMenuAt), r.Below(8) == 0 ? 2 : r.Below(2));
             }});
  // Session::Make (the session there or none) and Session::Init (on the
  // world's session block).
  Run(h, w, {"Session::Make (sub_822926B8; sub_822915C0)", 60, __imp__sub_822926B8,
             hooks::SessionMake, {{-96, -16}, {-208, -128}}, Nothing});
  Run(h, w, {"Session::Init (sub_822915C0)", 40, __imp__sub_822915C0, hooks::SessionInit,
             {{-112, -32}},
             [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.session; }});
}

}  // namespace

void NetworkModeSelect(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: network_mode: no block for the mode select's world");
    return;
  }
  ModeSelect(h, w);
}

}  // namespace harness
