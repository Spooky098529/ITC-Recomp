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
#include "memory/heap.h"
#include "type67_screens/type67_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type67_screens);

namespace harness {

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
// from `targets`, the lists' blocks made, then `setup`; r3 the screen, r4
// the dispatcher, lr a return address of the harness's.
void Cases(Context& h, const Block& world, const char* name, uint32_t address, int cases,
           const std::vector<uint32_t>& targets, const std::vector<uint32_t>& lists,
           const Setup& setup, GuestFunction recompiled, const NativeCall& native) {
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
      t.r3.u64 = (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0) | w;
      t.r4.u64 = w + kDispatcher;
      t.lr = 0x82000000 + 4 * r.Below(1024);
      g_records.clear();
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

}  // namespace

void Type67Screens(Context& h) {
  const Block world(kWorldBytes);
  const Block movie(0xA000);
  if (!world.address || !movie.address) {
    ITC_LOG_ERROR("harness: type67_screens: no block from the allocation layer");
    return;
  }
  ListAndNodeScreens(h, world, movie.address);
}

}  // namespace harness
