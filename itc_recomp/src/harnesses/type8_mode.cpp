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
#include "hooks/type67_screens.h"
#include "hooks/type8_mode.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
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
  g_movies = nullptr;
  saved.Restore();
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
  memory::Free(world);
}

}  // namespace harness
