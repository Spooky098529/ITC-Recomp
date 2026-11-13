// The type 6 and 7 modes' list, panel and node screens (src/type67_screens/,
// docs/functions/type67-screens.md) against the recompiled functions. The
// originals run with --native_type67_screens off, the plain C++ with the
// calls hooks::Type67ScreensCalls makes.
//
// A world block of random words holds the screen (r3 at its start), its
// vtable (+0x3800), the dispatcher (+0x3200) and the car list's and group
// pair's objects and vtables. Each vtable word is picked per case from
// stand-ins (the empty handler sub_82106540; sub_820CC470, which stores r4
// at r3 + 20, so a call and its r3 and r4 show; sub_82090780, which answers
// r3's +80, so a slot's answer shows) and the screens' own slots (their
// plain C++ on the native side). UiMovies::Open and Delete are recorded
// (their adapters hand the original's calls to hooks::g_start_up_recorder;
// the plain C++'s open and remove record the same words), each call's id,
// asset, frame and return address compared; Open answers 1, Delete
// 0x5A5A. UiMovies::IsLoaded reads the harness's movie block (the ids used,
// 16 to 19, aren't in the movie table, so it reads the word at 0x826C5CDC,
// planted for the case), its loaded flag random. The menu lists' blocks
// (+68) are the layer's pages made per side or 0, compared by whether
// they're still pages after (the word replaced by a marker). The world,
// the records and r3 (both halves) compared with harness::Outputs, all
// exactly (no float math).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/app_start_up.h"
#include "hooks/type67_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "type67_screens/type67_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type67_screens);

namespace harness {

// The stand's car world (src/harnesses/car_makers.cpp): a spec and
// parameters for one car id in a 0x10000-byte block. (The same definition
// is in car_makers.cpp.)
struct StandWorld {
  uint32_t params, holder_word, list;
};
StandWorld LayShowCarWorld(uint8_t* base, uint32_t block, Random& r, int c, uint32_t holder);
// The node tables' cases (src/harnesses/type67_screen_tables.cpp).
void Type67ScreenTables(Context& h);
void Type67ListPart(Context& h);
void Type67ListHelpers(Context& h);
void Type67TitledList(Context& h);
void Type67ColorList(Context& h);
void Type67NodeScreen(Context& h);
// Type6ListScreen::SetUpRace's cases (src/harnesses/race_setup.cpp).
void Type67SetUpRace(Context& h);
void Type67Type7SetUpRace(Context& h);

namespace {

namespace ts = type67_screens;

constexpr uint32_t kWorldBytes = 0x4000;
constexpr uint32_t kVtable = 0x3800;
constexpr uint32_t kDispatcher = 0x3200;
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMovieLoaded = 0x9FD0;
constexpr uint32_t kOpen = 0x8211E808, kDelete = 0x8211E258;
constexpr uint32_t kSetR4 = 0x820CC470, kGetter = 0x82090780;

uint64_t SplitMix(uint64_t z) {
  z += 0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return z ^ (z >> 31);
}

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The UI movie calls recorded: (address, r3, r4 for Open, r1, lr).
std::vector<uint32_t> g_records;
bool Record(PPCContext& ctx, uint32_t address) {
  if (address == kOpen) {
    g_records.insert(g_records.end(), {kOpen, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, uint32_t(ctx.lr)});
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

// UiMovie::Update (UiMovie::Run's jump) recorded through the UI movies'
// stand-in: (address, r3, r4, r1, lr), answered 0.
bool RecordUpdate(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (function != 0x8211D2F8) return false;
  g_records.insert(g_records.end(),
                   {function, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, uint32_t(ctx.lr)});
  ctx.r3.u64 = 0;
  return true;
}

uint32_t g_movie = 0;  // the harness's movie block (kMovieBytes)
constexpr uint32_t kMovieBytes = 0xA100;
constexpr uint32_t kFadeWords[3] = {ts::kFadeWait, ts::kFadeCountA, ts::kFadeCountB};
// The screen fades' globals (ScreenFades::FadeIn writes them under
// PanelFade::In): the durations, colors, alphas and counts, compared and put
// back after.
constexpr uint32_t kFades = 0x826A50AC, kFadesBytes = 0x3C;

// The plain C++ with the recording calls.
using NativeCall = std::function<uint64_t(PPCContext&, uint8_t*, const ts::Calls&)>;

ts::Calls RecordingCalls(PPCContext& t, uint8_t* base) {
  ts::Calls calls = hooks::Type67ScreensCalls(t, base);
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

// What a case sets up beyond the random world: `setup(r, world)`.
using Setup = std::function<void(Random&, uint32_t)>;

// Runs `cases` cases of a function: the world random, the vtable picked
// from `targets`, the lists' blocks made, then `setup`; r3 the screen (plus
// `r3_offset`, an object in the world; now and then with its high half
// set), r4 the dispatcher, lr a return address of the harness's.
void Cases(Context& h, const Block& world, const char* name, uint32_t address, int cases,
           const std::vector<uint32_t>& targets, const std::vector<uint32_t>& lists,
           const Setup& setup, GuestFunction recompiled, const NativeCall& native,
           uint32_t r3_offset = 0) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{} (sub_{:08X})", name, address));
  for (int c = 0; c < cases; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(SplitMix((uint64_t(address) << 16) ^ uint64_t(c)));
      const uint32_t w = world.address;
      WriteRandom(base, w, world.bytes, r);
      guest::StoreU32(base, w, w + kVtable);
      for (uint32_t i = 0; i < 32; i++) {
        guest::StoreU32(base, w + kVtable + 4 * i, targets[r.Below(uint32_t(targets.size()))]);
      }
      std::vector<uint32_t> blocks;
      for (const uint32_t list : lists) {
        uint32_t block = 0;
        if (r.Below(2) == 0) {
          block = memory::AllocPages(64, 4096);
        }
        guest::StoreU32(base, w + list + 68, block);
        blocks.push_back(block);
      }
      setup(r, w);
      PPCContext t = CallContext(h);
      t.r3.u64 = ((r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0) | w) + r3_offset;
      t.r4.u64 = w + kDispatcher;
      t.lr = 0x82000000 + 4 * r.Below(1024);
      g_records.clear();
      const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = RecordUpdate;
      if (v == 0) {
        const hooks::StartUpRecorder saved = hooks::g_start_up_recorder;
        hooks::g_start_up_recorder = Record;
        ITC_FLAG(native_type67_screens) = false;
        recompiled(t, base);
        ITC_FLAG(native_type67_screens) = true;
        hooks::g_start_up_recorder = saved;
      } else {
        const ts::Calls calls = RecordingCalls(t, base);
        t.r3.u64 = native(t, base, calls);
      }
      ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
      for (size_t i = 0; i < lists.size(); i++) {
        const uint32_t block = blocks[i];
        if (block == 0) continue;
        const bool pages = memory::IsPages(block);
        out[v].Word(pages ? 1 : 0);
        if (guest::LoadU32(base, w + lists[i] + 68) == block) {
          guest::StoreU32(base, w + lists[i] + 68, 0xB10C0000 + uint32_t(i));
        }
        if (pages) memory::FreePages(block);
      }
      for (const uint32_t word : g_records) out[v].Word(word);
      out[v].Word(0xFFFFFFFF);
      out[v].Bytes(Read(base, w, world.bytes));
      out[v].Bytes(Read(base, g_movie, kMovieBytes));
      for (const uint32_t g : kFadeWords) out[v].Word(guest::LoadU32(base, g));
      out[v].Bytes(Read(base, kFades, kFadesBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void ListAndNodeScreens(Context& h, const Block& world, uint32_t movie) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != kMissingMovie) {
      ITC_LOG_ERROR("harness: type67_screens: movie {} is in the table; the screens skipped", id);
      return;
    }
  }
  const uint32_t saved_missing = guest::LoadU32(base, kMissingMovie);
  const std::vector<uint32_t> targets = {
      ts::kEmpty, kSetR4,     kGetter,    ts::kEmpty, kSetR4,     kGetter,
      0x82242F70, 0x82233320, 0x82242F10, 0x82233330, 0x82242F20, 0x82236FD8,
      0x82236EB0, 0x82236EC0};
  const std::vector<uint32_t> list_lists = {10144, 10216, 10288, 10360};
  const std::vector<uint32_t> node_lists = {10132, 10204};
  auto screen = [base, movie](std::initializer_list<uint32_t> words) {
    const std::vector<uint32_t> tested(words);
    return [base, movie, tested](Random& r, uint32_t w) {
      for (const uint32_t at : tested) {
        guest::StoreU32(base, w + at, r.Below(2) == 0 ? 0 : r.Below(3));
      }
      guest::StoreU32(base, w + 9680, 16 + r.Below(4));
      guest::StoreU32(base, w + 10100, 16 + r.Below(4));
      guest::StoreU32(base, kMissingMovie, movie);
      guest::StoreU32(base, movie + kMovieLoaded, r.Below(3) ? 1 : 0);
    };
  };
  const Setup list = screen({9612, 9616, 9688});
  const Setup node = screen({10068, 10072});
  std::vector<uint32_t> lists = list_lists;
  lists.insert(lists.end(), node_lists.begin(), node_lists.end());
  const int n = 24;
  Cases(h, world, "ListScreen::OnEnter", 0x82251618, n, targets, lists, list,
        __imp__sub_82251618, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenOnEnter(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
        });
  Cases(h, world, "ListScreen::OpenMovie", 0x82251508, n, targets, lists, list,
        __imp__sub_82251508, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenOpenMovie(b, t.r3.u64, t.r4.u64, t.r1.u32, uint32_t(t.lr), c);
        });
  Cases(h, world, "ListScreen::WaitForMovie", 0x82251678, n, targets, lists, list,
        __imp__sub_82251678, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenWaitForMovie(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
        });
  Cases(h, world, "ListScreen::Leave", 0x82251760, n, targets, lists, list,
        __imp__sub_82251760, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenLeave(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
        });
  Cases(h, world, "ListScreen::OnLeft", 0x82251710, n, targets, lists, list,
        __imp__sub_82251710, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenOnLeft(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
        });
  Cases(h, world, "ListScreen::IsLoaded", 0x82242F70, 8, targets, {}, list, __imp__sub_82242F70,
        [](PPCContext& t, uint8_t* b, const ts::Calls&) {
          return ts::ListScreenIsLoaded(b, t.r3.u64, t.r1.u32);
        });
  Cases(h, world, "Type6ListScreen::OpenMovie", 0x82233320, 4, targets, {}, list,
        __imp__sub_82233320, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenOpenAsset(b, t.r3.u64, false, t.r1.u32, uint32_t(t.lr), c);
        });
  Cases(h, world, "Type7ListScreen::OpenMovie", 0x82242F10, 4, targets, {}, list,
        __imp__sub_82242F10, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenOpenAsset(b, t.r3.u64, true, t.r1.u32, uint32_t(t.lr), c);
        });
  Cases(h, world, "Type6ListScreen::Release", 0x82233330, 12, targets, lists, list,
        __imp__sub_82233330, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenRelease(b, t.r3.u64, false, t.r1.u32, c);
        });
  Cases(h, world, "Type7ListScreen::Release", 0x82242F20, 12, targets, lists, list,
        __imp__sub_82242F20, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::ListScreenRelease(b, t.r3.u64, true, t.r1.u32, c);
        });

  Cases(h, world, "NodeScreen::OnEnter", 0x822591B0, n, targets, lists, node,
        __imp__sub_822591B0, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::NodeScreenOnEnter(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
        });
  Cases(h, world, "NodeScreen::OpenMovie", 0x822590D0, n, targets, lists, node,
        __imp__sub_822590D0, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::NodeScreenOpenMovie(b, t.r3.u64, t.r4.u64, t.r1.u32, uint32_t(t.lr), c);
        });
  Cases(h, world, "NodeScreen::WaitForMovie", 0x82259208, n, targets, lists, node,
        __imp__sub_82259208, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::NodeScreenWaitForMovie(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
        });
  Cases(h, world, "NodeScreen::Leave", 0x822592D8, n, targets, lists, node,
        __imp__sub_822592D8, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::NodeScreenLeave(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
        });
  Cases(h, world, "NodeScreen::OnLeft", 0x82259298, n, targets, lists, node,
        __imp__sub_82259298, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::NodeScreenOnLeft(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
        });
  Cases(h, world, "NodeScreen::Open", 0x82259318, 4, targets, {}, node, __imp__sub_82259318,
        [](PPCContext& t, uint8_t* b, const ts::Calls&) {
          return ts::NodeScreenOpen(b, t.r3.u64);
        });
  Cases(h, world, "Type6NodeScreen::IsLoaded", 0x82236FD8, 8, targets, {}, node,
        __imp__sub_82236FD8, [](PPCContext& t, uint8_t* b, const ts::Calls&) {
          return ts::NodeScreenIsLoaded(b, t.r3.u64, t.r1.u32);
        });
  Cases(h, world, "Type6NodeScreen::OpenMovie", 0x82236EB0, 4, targets, {}, node,
        __imp__sub_82236EB0, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::NodeScreenOpenAsset(b, t.r3.u64, t.r1.u32, uint32_t(t.lr), c);
        });
  Cases(h, world, "Type6NodeScreen::Release", 0x82236EC0, 12, targets, lists, node,
        __imp__sub_82236EC0, [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
          return ts::NodeScreenRelease(b, t.r3.u64, t.r1.u32, c);
        });
  guest::StoreU32(base, kMissingMovie, saved_missing);
}


// The panel screens: the car list at the world's +0x3600 (its vtable at
// +0x3900), the group pair at +0x3A00 (vtable +0x3B00; its groups at
// +0x3C00 and +0x3C40, their ids 16 to 19), the fade words random (their
// counts mostly 0) and put back after; +11700 0 (Type7PanelScreen::AskQuit
// never ran in play: its branch is the original's, not taken here).
void PanelScreens(Context& h, const Block& world, uint32_t movie) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {4u, 28u, 16u, 17u, 18u, 19u}) {
    if (id >= 16 && ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != kMissingMovie) {
      ITC_LOG_ERROR("harness: type67_screens: movie {} is in the table; the panels skipped", id);
      return;
    }
  }
  const uint32_t saved_missing = guest::LoadU32(base, kMissingMovie);
  constexpr uint32_t kList = 0x3600, kListVtable = 0x3900, kPair = 0x3A00, kPairVtable = 0x3B00;
  const std::vector<uint32_t> stand_ins = {ts::kEmpty, kSetR4, kGetter};
  // The screen's slots: the stand-ins and the panel screens' slots (the
  // posts only where r4 is the dispatcher: not slots 6 to 8, which Show
  // calls with r4 left by the call before).
  const std::vector<uint32_t> screen_targets = {
      ts::kEmpty, kSetR4,     kGetter,    ts::kEmpty, kSetR4,     kGetter,    0x82230790,
      0x82230758, 0x822401D0, 0x82240208, 0x82240860, 0x82247058, 0x822408B8, 0x82240238,
      0x82240248, 0x82240258, 0x8223E708, 0x822307D0, 0x822307E0, 0x822307F0};
  const std::vector<uint32_t> no_posts = {ts::kEmpty, kSetR4,     kGetter,    0x82240238,
                                          0x82240248, 0x82240258, 0x82230790, 0x822307D0,
                                          0x822307E0, 0x822307F0};
  const std::vector<uint32_t> pair_targets = {ts::kEmpty, kSetR4,     kGetter,    0x82249C30,
                                              0x82249C40, 0x82249BE0, 0x82249BA8, 0x82249C70};
  // Update and DrawList test their pointers; the other handlers read them
  // as set.
  static bool nulls = false;
  const Setup panel = [base, movie, stand_ins, no_posts, pair_targets](Random& r, uint32_t w) {
    for (const uint32_t at : {9612u, 9616u, 9652u}) {
      guest::StoreU32(base, w + at, r.Below(2) == 0 ? 0 : r.Below(3));
    }
    guest::StoreU32(base, w + 9656, r.Below(4));
    guest::StoreU32(base, w + 11700, 0);
    guest::StoreU32(base, w + 9644, nulls && r.Below(4) == 0 ? 0 : w + kList);
    guest::StoreU32(base, w + 9648, nulls && r.Below(4) == 0 ? 0 : w + kPair);
    for (const uint32_t slot : {24u, 28u, 32u}) {
      guest::StoreU32(base, w + kVtable + slot, no_posts[r.Below(uint32_t(no_posts.size()))]);
    }
    guest::StoreU32(base, w + kList, w + kListVtable);
    guest::StoreU32(base, w + kList + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    for (uint32_t i = 0; i < 20; i++) {
      guest::StoreU32(base, w + kListVtable + 4 * i,
                      stand_ins[r.Below(uint32_t(stand_ins.size()))]);
    }
    guest::StoreU32(base, w + kPair, w + kPairVtable);
    guest::StoreU32(base, w + kPair + 8, w + 0x3C00);
    guest::StoreU32(base, w + kPair + 12, w + 0x3C40);
    guest::StoreU32(base, w + kPair + 4, r.Below(2) == 0 ? w + 0x3C00 : w + 0x3C40);
    guest::StoreU32(base, w + kPair + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU32(base, w + kPairVtable + 4 * i,
                      pair_targets[r.Below(uint32_t(pair_targets.size()))]);
    }
    guest::StoreU32(base, w + 0x3C00 + 4, 16 + r.Below(4));
    guest::StoreU32(base, w + 0x3C40 + 4, 16 + r.Below(4));
    guest::StoreU32(base, kMissingMovie, movie);
    guest::StoreU32(base, movie + kMovieLoaded, r.Below(3) ? 1 : 0);
    guest::StoreU32(base, ts::kFadeWait, r.Below(2));
    guest::StoreU32(base, ts::kFadeCountA, r.Below(3) == 0 ? r.Below(3) : 0);
    guest::StoreU32(base, ts::kFadeCountB, r.Below(3) == 0 ? r.Below(3) : 0);
  };
  const int n = 24;
  struct Function {
    const char* name;
    uint32_t address;
    int cases;
    uint32_t r3_offset;  // r3 = the world + this (the screen at 0)
    GuestFunction recompiled;
    NativeCall native;
  };
  const Function functions[] = {
      {"PanelGroup::Movie", 0x82258FC8, 8, 0x3C00, __imp__sub_82258FC8,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelGroupMovie(b, t.r3.u64, t.r1.u32);
       }},
      {"PanelGroup::Run", 0x82259008, 8, 0x3C40, __imp__sub_82259008,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelGroupRun(b, t.r3.u64, t.r1.u32, c);
       }},
      {"GroupPair::SelectA", 0x82249C30, 4, kPair, __imp__sub_82249C30,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) { return ts::GroupPairSelectA(b, t.r3.u64); }},
      {"GroupPair::Reset", 0x82249C40, 8, kPair, __imp__sub_82249C40,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::GroupPairReset(b, t.r3.u64, t.r1.u32, c);
       }},
      {"GroupPair::Run", 0x82249BE0, 8, kPair, __imp__sub_82249BE0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::GroupPairRun(b, t.r3.u64, t.r1.u32, c);
       }},
      {"GroupPair::WaitForFade", 0x82249BA8, 12, kPair, __imp__sub_82249BA8,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::GroupPairWaitForFade(b, t.r3.u64);
       }},
      {"GroupPair::FinishAll", 0x82249C70, 8, kPair, __imp__sub_82249C70,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::GroupPairFinishAll(b, t.r3.u64, t.r1.u32, c);
       }},
      {"GroupPair::~GroupPair", 0x82249C20, 4, kPair, __imp__sub_82249C20,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::GroupPairDestruct(b, t.r3.u64);
       }},
      {"SingleGroup::SingleGroup", 0x82249CB0, 4, kPair, __imp__sub_82249CB0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::SingleGroupConstruct(b, t.r3.u64, t.r4.u32);
       }},
      {"SingleGroup::Reset", 0x82249CC8, 8, kPair, __imp__sub_82249CC8,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::SingleGroupReset(b, t.r3.u64, t.r1.u32, c);
       }},
      {"PanelFade::In", 0x82249BE8, 8, 0, __imp__sub_82249BE8,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelFadeIn(b, t.r3.u64, t.r1.u32, uint32_t(t.lr), c);
       }},
      {"MachineScreen::~MachineScreen", 0x8224CB18, 4, 0, __imp__sub_8224CB18,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::MachineScreenDestruct(b, t.r3.u64);
       }},
      {"PanelScreen::IsLoaded", 0x82230790, 8, 0, __imp__sub_82230790,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelScreenIsLoaded(b, t.r1.u32);
       }},
      {"Type6PanelScreen::OpenMovies", 0x82230758, 2, 0, __imp__sub_82230758,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOpenMovies(b, t.r1.u32, false, c);
       }},
      {"Type7PanelScreen::OpenMovies", 0x822401D0, 2, 0, __imp__sub_822401D0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOpenMovies(b, t.r1.u32, true, c);
       }},
      {"PanelScreen::DeleteMovies", 0x82240208, 2, 0, __imp__sub_82240208,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenDeleteMovies(b, t.r1.u32, c);
       }},
      {"Type7PanelScreen::Back", 0x82240860, 8, 0, __imp__sub_82240860,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelScreenPost(b, t.r3.u64, t.r4.u64, 0x82240860);
       }},
      {"Type6PanelScreen::OpenPowerGraph", 0x82247058, 8, 0, __imp__sub_82247058,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelScreenPost(b, t.r3.u64, t.r4.u64, 0x82247058);
       }},
      {"Type7PanelScreen::OpenPowerGraph", 0x822408B8, 8, 0, __imp__sub_822408B8,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelScreenPost(b, t.r3.u64, t.r4.u64, 0x822408B8);
       }},
      {"Type7PanelScreen::CarList", 0x82240238, 2, 0, __imp__sub_82240238,
       [](PPCContext&, uint8_t* b, const ts::Calls&) { return ts::Type7CarWord(b, 1972); }},
      {"Type7PanelScreen::CarEntry", 0x82240248, 2, 0, __imp__sub_82240248,
       [](PPCContext&, uint8_t* b, const ts::Calls&) { return ts::Type7CarWord(b, 1976); }},
      {"Type7PanelScreen::CarSlot", 0x82240258, 2, 0, __imp__sub_82240258,
       [](PPCContext&, uint8_t* b, const ts::Calls&) { return ts::Type7CarWord(b, 1980); }},
      // The type 6 list batch's leaves (type6_list.cpp).
      {"PanelScreen::OnYes", 0x822609E0, 2, 0, __imp__sub_822609E0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelScreenSetAnswer(b, t.r3.u64, 0);
       }},
      {"PanelScreen::OnNo", 0x822609D0, 2, 0, __imp__sub_822609D0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelScreenSetAnswer(b, t.r3.u64, 1);
       }},
      {"PanelScreen::OnBack", 0x822306D0, 2, 0, __imp__sub_822306D0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelScreenSetAnswer(b, t.r3.u64, 2);
       }},
      {"PanelScreen::OnDialogClosed", 0x822306E0, 12, 0, __imp__sub_822306E0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOnDialogClosed(b, t.r3.u64, t.r1.u32, c);
       }},
      {"PanelScreen::Suspend", 0x82248DE0, 8, 0, __imp__sub_82248DE0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenSuspend(b, t.r3.u64, t.r1.u32, uint32_t(t.lr), c);
       }},
      {"PanelScreen::OpenEmpty", 0x822404D8, 8, 0, __imp__sub_822404D8,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOpenEmpty(b, t.r3.u64, t.r1.u32, uint32_t(t.lr), c);
       }},
      {"Type6PanelScreen::Back", 0x8223E708, 8, 0, __imp__sub_8223E708,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelScreenPost(b, t.r3.u64, t.r4.u64, 0x8223E708);
       }},
      {"Type6PanelScreen::CarList", 0x822307D0, 2, 0, __imp__sub_822307D0,
       [](PPCContext&, uint8_t* b, const ts::Calls&) { return ts::Type6CarWord(b, 1972); }},
      {"Type6PanelScreen::CarEntry", 0x822307E0, 2, 0, __imp__sub_822307E0,
       [](PPCContext&, uint8_t* b, const ts::Calls&) { return ts::Type6CarWord(b, 1976); }},
      {"Type6PanelScreen::CarSlot", 0x822307F0, 2, 0, __imp__sub_822307F0,
       [](PPCContext&, uint8_t* b, const ts::Calls&) { return ts::Type6CarWord(b, 1980); }},
      {"GroupPair::DeletingDestructor", 0x8224ACE0, 4, kPair, __imp__sub_8224ACE0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::GroupPairDeletingDestruct(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"PanelScreen::OnEnter", 0x82249D18, n, 0, __imp__sub_82249D18,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOnEnter(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"PanelScreen::OpenMovie", 0x82248CD0, n, 0, __imp__sub_82248CD0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOpenMovie(b, t.r3.u64, t.r4.u64, t.r1.u32, uint32_t(t.lr), c);
       }},
      {"PanelScreen::WaitForMovie", 0x82249D88, n, 0, __imp__sub_82249D88,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenWaitForMovie(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"PanelScreen::Open", 0x82249DF8, n, 0, __imp__sub_82249DF8,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOpen(b, t.r3.u64, t.r1.u32, uint32_t(t.lr), c);
       }},
      {"PanelScreen::Show", 0x82248CE0, n, 0, __imp__sub_82248CE0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenShow(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"PanelScreen::Update", 0x82248C48, n, 0, __imp__sub_82248C48,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenUpdate(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"PanelScreen::OnMessage24", 0x82248DF8, 8, 0, __imp__sub_82248DF8,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOnMessage24(b, t.r3.u64, t.r1.u32, uint32_t(t.lr), c);
       }},
      {"PanelScreen::Leave", 0x82249E38, n, 0, __imp__sub_82249E38,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenLeave(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"PanelScreen::OnLeft", 0x82248B08, n, 0, __imp__sub_82248B08,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenOnLeft(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"PanelScreen::DrawList", 0x82248B58, 32, 0, __imp__sub_82248B58,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenDrawList(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"Type7PanelScreen::DrawList", 0x82240908, n, 0, __imp__sub_82240908,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::Type7PanelScreenDrawList(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"Type7PanelScreen::Draw", 0x82240958, 32, 0, __imp__sub_82240958,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::Type7PanelScreenDraw(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
  };
  for (const Function& f : functions) {
    nulls = f.address == 0x82248C48 || f.address == 0x82248B58 || f.address == 0x82240908 ||
            f.address == 0x82240958;
    Cases(h, world, f.name, f.address, f.cases, screen_targets, {}, panel, f.recompiled, f.native,
          f.r3_offset);
  }
  // The destructors (r4 the dispatcher: bit 0 clear, nothing deleted; the
  // deleting cases are in Deleting): the car list's two menus (+9752 + 4 +
  // 132 and + 60) with their blocks pages or 0.
  nulls = false;
  const std::vector<uint32_t> menus = {9888, 9816};
  const Function destructors[] = {
      {"PanelGroup::~PanelGroup", 0x82231CC0, 4, 0x3C00, __imp__sub_82231CC0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PanelGroupDestruct(b, t.r3.u64);
       }},
      {"PanelGroup::DeletingDestructor", 0x82240348, 4, 0x3C00, __imp__sub_82240348,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelGroupDeletingDestruct(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"Type6PanelScreen::~Type6PanelScreen", 0x82230668, 8, 0, __imp__sub_82230668,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenDestruct(b, t.r3.u64, false, t.r1.u32, c);
       }},
      {"Type7PanelScreen::~Type7PanelScreen", 0x82240168, 8, 0, __imp__sub_82240168,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelScreenDestruct(b, t.r3.u64, true, t.r1.u32, c);
       }},
      {"Type6PanelScreen::DeletingDestructor", 0x82230B00, 4, 0, __imp__sub_82230B00,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::Type6PanelScreenDeletingDestruct(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
  };
  for (const Function& f : destructors) {
    Cases(h, world, f.name, f.address, f.cases, screen_targets,
          f.r3_offset == 0 ? menus : std::vector<uint32_t>{}, panel, f.recompiled, f.native,
          f.r3_offset);
  }
  guest::StoreU32(base, kMissingMovie, saved_missing);
}

// Type7PanelScreen::PickCar: the car list at the world's +0x3600 with its
// real slot 18 (sub_820B67D8, the list + 208: the selection), the
// selection's tab (+28) 2 (the garage) or not, its steps (+32, +36) small,
// its +40 and +44 random, its entry table (+1616) at the world's +0x3E00
// naming an entry whose car id (+8432) is LayShowCarWorld's (the case's
// spec and parameters, [0x826C2E48] pointed at them; rand's seed set per
// case for Car::SetModel's picks); the save data a
// harness block ([0x826C5B58]) with a garage list (the type 6 and 7 modes'
// harness's shape) walked the selection's steps to a slot of random words;
// the type 7 car random, its +4 0xFFFF now and then. GarageCar::Reset runs
// through its adapter on both sides. Compared: the world, the save block,
// the type 7 car, the stack below the call (but the original's register
// saves) and r3 (both halves); the globals put back after.
void PickCar(Context& h, const Block& world) {
  uint8_t* const base = h.base;
  constexpr uint32_t kSaveBytes = 0x1A000, kSavePointer = 0x826C5B58, kParams = 0x826C2E48;
  constexpr uint32_t kCarBytes = 1984, kFrame = 128, kSaves = 48;
  const Block save_block(kSaveBytes), stand(0x10000), entry_block(0x2100);
  if (!save_block.address || !stand.address || !entry_block.address) return;
  const uint32_t save = save_block.address, entry = entry_block.address;
  const uint32_t sp = CallContext(h).r1.u32;
  const uint32_t saved_save = guest::LoadU32(base, kSavePointer);
  const uint32_t saved_params = guest::LoadU32(base, kParams);
  // rand's seed (Car::SetModel's picks), set per case and put back after.
  constexpr uint32_t kRandSeed = 0x82638188;
  const uint32_t saved_seed = guest::LoadU32(base, kRandSeed);
  const std::vector<uint8_t> saved_car = Read(base, ts::kType7Car, kCarBytes);
  Results results("Type7PanelScreen::PickCar (sub_822405F8)");
  for (int c = 0; c < 32; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(SplitMix(0x822405F8ull << 16 ^ uint64_t(c)));
      const uint32_t w = world.address;
      WriteRandom(base, w, world.bytes, r);
      WriteRandom(base, save, 0x400, r);
      WriteRandom(base, save + 6120, 0x400, r);
      WriteRandom(base, sp - 2048, 2048 + 256, r);
      const StandWorld stand_world = LayShowCarWorld(base, stand.address, r, c, 0);
      const uint32_t array = guest::LoadU32(base, stand_world.list + 1136);
      const uint32_t at = guest::LoadU32(base, stand_world.list + 48) +
                          guest::LoadU32(base, stand_world.list + 52);
      const uint32_t car_id = guest::LoadU32(base, guest::LoadU32(base, array + 4 * at) + 8432);
      guest::StoreU32(base, kParams, stand_world.params);
      const uint32_t list = w + 0x3600, selection = list + 208;
      guest::StoreU32(base, w + 9644, list);
      guest::StoreU32(base, list, w + 0x3900);
      guest::StoreU32(base, w + 0x3900 + 72, 0x820B67D8);
      guest::StoreU32(base, selection + 28, r.Below(2) == 0 ? 2 : r.Below(3));
      const uint32_t a = r.Below(3), b = r.Below(3);
      guest::StoreU32(base, selection + 32, a);
      guest::StoreU32(base, selection + 36, b);
      guest::StoreU32(base, selection + 1616, w + 0x3E00);
      guest::StoreU32(base, w + 0x3E00 + 4 * (a + b), entry);
      WriteRandom(base, entry + 8400, 64, r);
      guest::StoreU32(base, entry + 8432, car_id);
      // The garage list: from +204, a + b steps through distinct nodes.
      const uint32_t nodes = save + 6120;
      uint16_t node = uint16_t(r.Below(10));
      guest::StoreU16(base, nodes + 204, node);
      for (uint32_t i = 0; i < a + b; i++) {
        const uint16_t next = uint16_t(10 * (i + 1) + r.Below(10));
        guest::StoreU16(base, nodes + 4 * node, next);
        node = next;
      }
      const uint32_t slot = r.Below(3);
      guest::StoreU16(base, nodes + 4 * node + 2, uint16_t(slot));
      WriteRandom(base, nodes + 212 + 1968 * slot, 1968, r);
      guest::StoreU32(base, kSavePointer, save);
      WriteRandom(base, ts::kType7Car, kCarBytes, r);
      if (r.Below(2) == 0) guest::StoreU16(base, ts::kType7Car + 4, 0xFFFF);
      guest::StoreU32(base, kRandSeed, uint32_t(r.Next()));
      guest::StoreU32(base, w + kDispatcher + 204 + 600, r.Below(50));
      guest::StoreU32(base, w + kDispatcher + 204 + 604, r.Below(60));
      PPCContext t = CallContext(h);
      t.r3.u64 = w | (uint64_t(r.Below(2)) << 32);
      t.r4.u64 = w + kDispatcher;
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        ITC_FLAG(native_type67_screens) = false;
        __imp__sub_822405F8(t, base);
        ITC_FLAG(native_type67_screens) = true;
      } else {
        const ts::Calls calls = hooks::Type67ScreensCalls(t, base);
        t.r3.u64 = ts::Type7PanelScreenPickCar(base, t.r3.u64, t.r4.u64, t.r1.u32, calls);
      }
      out[v].Bytes(Read(base, w, world.bytes));
      out[v].Bytes(Read(base, save, kSaveBytes));
      out[v].Bytes(Read(base, ts::kType7Car, kCarBytes));
      out[v].Word(guest::LoadU32(base, kRandSeed));
      out[v].Bytes(Read(base, sp - 2048, 2048 - kFrame));
      out[v].Bytes(Read(base, sp - kFrame, kFrame - kSaves));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  guest::StoreU32(base, kSavePointer, saved_save);
  guest::StoreU32(base, kParams, saved_params);
  guest::StoreU32(base, kRandSeed, saved_seed);
  Write(base, ts::kType7Car, saved_car.data(), kCarBytes);
}

// The deleting destructors with bit 0 of r4 set: a block of operator new's
// (random words; a screen's menus' blocks 0, so MenuList::Release frees
// nothing) destroyed and deleted, compared by r3 and whether operator new
// hands the block out again.
void Deleting(Context& h) {
  uint8_t* const base = h.base;
  struct Kind {
    const char* name;
    uint32_t bytes;
    GuestFunction recompiled;
    NativeCall native;
  };
  const Kind kinds[] = {
      {"PanelGroup::DeletingDestructor, deleting (sub_82240348)", 64, __imp__sub_82240348,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PanelGroupDeletingDestruct(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"GroupPair::DeletingDestructor, deleting (sub_8224ACE0)", 64, __imp__sub_8224ACE0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::GroupPairDeletingDestruct(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"Type6PanelScreen::DeletingDestructor, deleting (sub_82230B00)", 11744,
       __imp__sub_82230B00,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::Type6PanelScreenDeletingDestruct(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
  };
  for (const Kind& kind : kinds) {
    Results results(kind.name);
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(SplitMix(0xDE1E7Eull << 16 ^ uint64_t(c) ^ kind.bytes));
        const auto block = memory::crt::OperatorNew(base, kind.bytes);
        if (!block) {
          ITC_LOG_ERROR("harness: type67_screens: operator new failed");
          return;
        }
        WriteRandom(base, *block, kind.bytes, r);
        for (const uint32_t list : {9888u, 9816u}) {
          if (list + 72 <= kind.bytes) guest::StoreU32(base, *block + list + 68, 0);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = *block;
        t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
        t.lr = 0x82000000 + 4 * r.Below(1024);
        if (v == 0) {
          ITC_FLAG(native_type67_screens) = false;
          kind.recompiled(t, base);
          ITC_FLAG(native_type67_screens) = true;
        } else {
          const ts::Calls calls = RecordingCalls(t, base);
          t.r3.u64 = kind.native(t, base, calls);
        }
        out[v].Word(t.r3.u32 == *block ? 1 : 0);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        const auto probe = memory::crt::OperatorNew(base, kind.bytes);
        out[v].Word(probe && *probe == *block ? 1 : 0);
        if (probe) memory::crt::OperatorDelete(*probe);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void Type67Screens(Context& h) {
  const Block world(kWorldBytes);
  const Block movie(kMovieBytes);
  if (!world.address || !movie.address) {
    ITC_LOG_ERROR("harness: type67_screens: no block from the allocation layer");
    return;
  }
  g_movie = movie.address;
  // A movie with nothing queued or looping and empty scenes (its live,
  // saved and file scenes a node at +0xA000 without children): ResetPose
  // and FinishAll leave it as it is.
  Fill(h.base, movie.address, kMovieBytes, 0);
  for (const uint32_t at : {16u, 20u, 24u}) {
    guest::StoreU32(h.base, movie.address + at, movie.address + 0xA000);
  }
  uint32_t saved_fades[3];
  for (int i = 0; i < 3; i++) saved_fades[i] = guest::LoadU32(h.base, kFadeWords[i]);
  const std::vector<uint8_t> saved_fade_block = Read(h.base, kFades, kFadesBytes);
  ListAndNodeScreens(h, world, movie.address);
  PanelScreens(h, world, movie.address);
  PickCar(h, world);
  Deleting(h);
  Type67ScreenTables(h);
  Type67ListPart(h);
  Type67ListHelpers(h);
  Type67TitledList(h);
  Type67ColorList(h);
  Type67NodeScreen(h);
  Type67SetUpRace(h);
  Type67Type7SetUpRace(h);
  Write(h.base, kFades, saved_fade_block.data(), kFadesBytes);
  for (int i = 0; i < 3; i++) guest::StoreU32(h.base, kFadeWords[i], saved_fades[i]);
}

}  // namespace harness
