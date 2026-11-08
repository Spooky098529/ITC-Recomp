// The options screen (src/options_screen/, docs/functions/options-screen.md)
// against the recompiled functions. Each case fills a world of our own with
// random words from the case's seed (the settings block, the front-end menu,
// the page block), plus the stack below the call's stack pointer and the
// caller's area above it; points the screen's globals at the world; shapes
// the fields the function tests; runs the original (with
// --native_options_screen off) or the register steps
// (hooks/options_screen.h); and compares the world, the originals' frames
// (their register saves left out), the caller's area, r1 and r3, all
// exactly (nothing here is a float). The globals are put back after each
// run.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/options_screen.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "options_screen/options_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_options_screen);

namespace harness {

namespace {

namespace os = options_screen;

constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kGlobals = os::kPage;  // the screen's seven globals
constexpr uint32_t kGlobalsBytes = 28;

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

// A small signed word around [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// The world: the seven blocks the screen's globals point at (0x400 bytes
// each: the page block, the menu, the settings, then four more); player 0's
// save data (its options at +0x1F85C); a misc block (the machine at +0, its
// words up to +9616; the dispatcher at +0x2600, its ring at +204; the fake
// sound manager at +0x3000 and its vtable at +0x3400; the movie table's
// pairs at +0x3800); UI movie 7 (its loaded flag at +0x9FD0).
constexpr uint32_t kBlockBytes = 0x400;
constexpr uint32_t kSaveBytes = 0x20000;
constexpr uint32_t kMiscBytes = 0x4000;
constexpr uint32_t kMovieBytes = 0xA000;
constexpr uint32_t kSaveDataPointers = 0x826C5B58;
constexpr uint32_t kMissingMovie = 0x826C5CDC;  // UiMovies::Find's answer when not found
// The getter sub_82090770 (r3 = +60), the sound manager's slots 29 and 30.
constexpr uint32_t kGet60 = 0x82090770;
struct World {
  World() : blocks(7 * kBlockBytes), save(kSaveBytes), misc(kMiscBytes), movie(kMovieBytes) {}
  bool ok() const {
    return blocks.address != 0 && save.address != 0 && misc.address != 0 && movie.address != 0;
  }
  uint32_t machine() const { return misc.address; }
  uint32_t dispatcher() const { return misc.address + 0x2600; }
  uint32_t manager() const { return misc.address + 0x3000; }
  uint32_t options() const { return save.address + os::kSaveOptions; }
  uint32_t block(uint32_t global) const {
    return blocks.address + (global - kGlobals) / 4 * kBlockBytes;
  }
  uint32_t page() const { return block(os::kPage); }
  uint32_t menu() const { return block(os::kMenu); }
  uint32_t settings() const { return block(os::kSettings); }
  Block blocks;
  Block save;
  Block misc;
  Block movie;
};

// The globals a case points at its world, put back after.
const std::vector<std::pair<uint32_t, uint32_t>> kSavedGlobals = {
    {kGlobals, kGlobalsBytes},
    {kSaveDataPointers, 4},
    {os::kModeType, 4},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4}};

// The mode type: 2, 4, 6, 7 or 8, now and then another.
uint32_t ModeType(Random& r) {
  static constexpr uint32_t kTypes[6] = {2, 4, 6, 7, 8, 3};
  return kTypes[r.Below(6)];
}

void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.blocks.address, w.blocks.bytes, r);
  WriteRandom(base, w.misc.address, w.misc.bytes, r);
  // Only the save data's parts the functions read or write (+0 to +0x2000
  // and the options), not all of it, and the movie's flag.
  WriteRandom(base, w.save.address, 0x2000, r);
  WriteRandom(base, w.options() - 0x40, 0x200, r);
  for (uint32_t g = kGlobals; g < kGlobals + kGlobalsBytes; g += 4) {
    guest::StoreU32(base, g, w.block(g));
  }
  guest::StoreU32(base, kSaveDataPointers, w.save.address);
  guest::StoreU32(base, os::kModeType, ModeType(r));
  // The sound manager: slots 29 and 30 the getter; its +60 -1 now and then.
  const uint32_t manager = w.manager(), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, manager + 60, r.Below(4) == 0 ? 0xFFFFFFFFu : uint32_t(r.Next()));
  guest::StoreU32(base, vtable + menu_sounds::kGetCategorySlot, kGet60);
  guest::StoreU32(base, vtable + menu_sounds::kSetCategoryVolumeSlot, kGet60);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
  // The movie table: movie 7 (its loaded flag 0 or 1) or none.
  const uint32_t pairs = w.misc.address + 0x3800;
  guest::StoreU32(base, pairs, r.Below(4) == 0 ? 9 : 7);
  guest::StoreU32(base, pairs + 4, w.movie.address);
  guest::StoreU32(base, w.movie.address + ui_movies::kMovieLoaded, r.Below(2));
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 4);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, 1);
  guest::StoreU32(base, kMissingMovie, w.movie.address);
  // The machine's words: +9608, +9612 0 or 1 or any.
  for (const uint32_t field : {9608u, 9612u}) {
    guest::StoreU32(base, w.machine() + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
  // The dispatcher's ring: its count and start small.
  guest::StoreU32(base, w.dispatcher() + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.dispatcher() + 204 + 604, r.Below(60));
}

// UiMovies::Open's calls, recorded on both sides (its adapter hands them to
// the start-up recorder) and answered 1.
std::vector<uint32_t> g_opens;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address != 0x8211E808) return false;
  g_opens.insert(g_opens.end(), {ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;
// Run after the function, before the outputs are read (the allocated
// blocks compared and freed).
using AfterFn = std::function<void(PPCContext&, const World&, Outputs&)>;

// The original's frames compared, as [sp - from, sp - to) ranges (its
// register saves left out).
using Frames = std::vector<std::pair<uint32_t, uint32_t>>;

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const Frames& frames,
          const std::string& what, const AfterFn& after) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, kSavedGlobals);
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    ShapeWorld(h.base, w, r);
    t.r3.u64 = r.Next();
    t.r4.u64 = r.Next();
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    g_opens.clear();
    const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
    hooks::g_start_up_recorder = RecordOpen;
    ITC_FLAG(native_options_screen) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_options_screen) = true;
    hooks::g_start_up_recorder = saved_recorder;
    if (after) {
      after(t, w, out[v]);
    }
    for (const uint32_t word : g_opens) out[v].Word(word);
    out[v].Bytes(Read(h.base, w.blocks.address, w.blocks.bytes));
    out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
    out[v].Bytes(Read(h.base, w.save.address, 0x2000));
    out[v].Bytes(Read(h.base, w.options() - 0x40, 0x200));
    for (const auto& [address, bytes] : kSavedGlobals) {
      out[v].Bytes(Read(h.base, address, bytes));
    }
    for (const auto& [from, to] : frames) {
      out[v].Bytes(Read(h.base, sp - from, from - to));
    }
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, const Frames& frames = {},
           const AfterFn& after = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, frames,
         fmt::format("case {}", c), after);
  }
  results.Report();
}

// --- The controls' tables and the settings' leaves ---

void Leaves(Context& h, const World& w) {
  // The tables: every code from -2 to 33, then any words.
  const auto code = [](PPCContext& t, Random& r, const World&) {
    t.r3.u64 = r.Below(4) == 0 ? r.Next() : uint64_t(int64_t(Around(r, -2, 33)) & 0xFFFFFFFF) |
                                                (uint64_t(r.Next()) << 32);
  };
  Group(h, w, "Controls::ButtonIndex (sub_821BA950)", 120, code, __imp__sub_821BA950,
        hooks::OptionsButtonIndex);
  Group(h, w, "Controls::ButtonCode (sub_821BAA18)", 120, code, __imp__sub_821BAA18,
        hooks::OptionsButtonCode);
  Group(h, w, "Controls::AnalogIndex (sub_821BAAE0)", 120, code, __imp__sub_821BAAE0,
        hooks::OptionsAnalogIndex);
  Group(h, w, "Controls::AnalogPicture (sub_821BAD08)", 120, code, __imp__sub_821BAD08,
        hooks::OptionsAnalogPicture);
  // The edits: values 0 to 5 (clashes likely), now and then any word; the
  // item 0 to 11 (or 0 to 5), now and then -1 to 16 (or 10): the frame's
  // neighbors, not any word (the original reads at sp - 48 + 4 item).
  const auto edits = [&h](const World& w, Random& r, uint32_t from, uint32_t to) {
    for (uint32_t f = from; f <= to; f += 4) {
      guest::StoreU32(h.base, w.settings() + f, r.Below(10) == 0 ? uint32_t(r.Next()) : r.Below(6));
    }
  };
  Group(
      h, w, "Options::IsButtonTaken (sub_821BA7E0)", 150,
      [&](PPCContext& t, Random& r, const World& w) {
        edits(w, r, 284, 328);
        t.r3.u64 = r.Below(8) == 0 ? uint64_t(int64_t(r.Below(18)) - 1) : r.Below(12);
      },
      __imp__sub_821BA7E0, hooks::OptionsIsButtonTaken, {{48, 0}});
  Group(
      h, w, "Options::IsAnalogTaken (sub_821BAB58)", 150,
      [&](PPCContext& t, Random& r, const World& w) {
        edits(w, r, 380, 400);
        t.r3.u64 = r.Below(8) == 0 ? uint64_t(int64_t(r.Below(12)) - 1) : r.Below(6);
      },
      __imp__sub_821BAB58, hooks::OptionsIsAnalogTaken, {{32, 0}});
  // The steps: the menu's side -1, 0, 1 or any, its cursor around the
  // items, the OK/Default word 0, 1 or any.
  const auto step = [&h](int32_t last) {
    return [&h, last](PPCContext&, Random& r, const World& w) {
      const uint32_t sides[4] = {0xFFFFFFFFu, 0, 1, uint32_t(r.Next())};
      guest::StoreU32(h.base, w.menu() + 36, sides[r.Below(4)]);
      guest::StoreU32(h.base, w.menu() + 0,
                      r.Below(2) == 0 ? uint32_t(last) : Around(r, -1, last + 1));
      guest::StoreU32(h.base, w.settings() + 660, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
    };
  };
  Group(h, w, "Options::StepButtons (sub_821BA550)", 120, step(12), __imp__sub_821BA550,
        hooks::OptionsStepButtons);
  Group(h, w, "Options::StepAnalog (sub_821BABD8)", 120, step(6), __imp__sub_821BABD8,
        hooks::OptionsStepAnalog);
  Group(
      h, w, "Settings::Reset (sub_821B9DB8)", 30,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.settings(); },
      __imp__sub_821B9DB8, hooks::OptionsSettingsReset, {{112, 24}});
}

// --- The settings' read and write, and the machine's handlers ---

void Handlers(Context& h, const World& w) {
  // The settings: the controls' slots +92 to +232 indexes -2 to 19, now and
  // then any; the volumes and sliders any words (their low bytes written).
  const auto slots = [&h](const World& w, Random& r) {
    for (uint32_t f = 92; f <= 232; f += 4) {
      guest::StoreU32(h.base, w.settings() + f,
                      r.Below(10) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(22)) - 2));
    }
  };
  // The save's records: codes 0 to 33, now and then any word.
  const auto records = [&h](const World& w, Random& r) {
    for (uint32_t f = 44; f < 284; f += 4) {
      guest::StoreU32(h.base, w.options() + f,
                      r.Below(10) == 0 ? uint32_t(r.Next()) : r.Below(34));
    }
    // The day: small, negative, or past 999.
    const uint32_t days[4] = {r.Below(400), uint32_t(-int32_t(r.Below(50))), 900 + r.Below(300),
                              uint32_t(r.Next())};
    guest::StoreU32(h.base, w.save.address + 6100, days[r.Below(4)]);
  };
  Group(
      h, w, "Settings::Read (sub_821BBF00)", 120,
      [&](PPCContext&, Random& r, const World& w) {
        records(w, r);
        // The context +4: 1 half the time (type 2's career info read).
        if (r.Below(2) == 0) {
          guest::StoreU32(h.base, os::kModeType, 2);
        }
      },
      __imp__sub_821BBF00, hooks::OptionsSettingsRead, {{272, 56}});
  Group(
      h, w, "Settings::Write (sub_821BAE10)", 120,
      [&](PPCContext&, Random& r, const World& w) { slots(w, r); }, __imp__sub_821BAE10,
      hooks::OptionsSettingsWrite, {{176, 32}});
  Group(
      h, w, "Options::OnClosed (sub_821BCE38)", 80,
      [&](PPCContext& t, Random& r, const World& w) {
        slots(w, r);
        t.r3.u64 = w.machine();
        t.r4.u64 = w.dispatcher();
      },
      __imp__sub_821BCE38, hooks::OptionsOnClosed, {{112, 24}, {288, 144}});
  Group(
      h, w, "Options::WaitForMovie (sub_821BBBA0)", 80,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.machine(); }, __imp__sub_821BBBA0,
      hooks::OptionsWaitForMovie, {{96, 16}});
  Group(
      h, w, "Options::OpenMovie (sub_821B9F88)", 10, [](PPCContext&, Random&, const World&) {},
      __imp__sub_821B9F88, hooks::OptionsOpenMovie);
  // OnEnter allocates over the layer, as its adapter runs it: the seven
  // blocks compared and freed.
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_ERROR("harness: options_screen: the native heap isn't active; OnEnter skipped");
    return;
  }
  Group(
      h, w, "Options::OnEnter (sub_821BB9A8)", 40,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.machine(); }, __imp__sub_821BB9A8,
      hooks::OptionsOnEnter, {{128, 40}},
      [&h](PPCContext& t, const World& w, Outputs& out) {
        static constexpr uint32_t kBytes[7] = {28, 68, 716, 28, 28, 68, 28};
        out.Word(t.r3.u32 == guest::LoadU32(h.base, 0x826D8488));
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t slot = kGlobals + 4 * i;
          const uint32_t block = guest::LoadU32(h.base, slot);
          if (block == 0 || !memory::IsPages(block)) {
            out.Word(0xDEAD0000 | (block != 0));
            continue;
          }
          out.Bytes(Read(h.base, block, kBytes[i]));
          memory::FreePages(block);
          guest::StoreU32(h.base, slot, w.block(slot));
        }
        t.r3.u64 = 0;
      });
}

}  // namespace

void OptionsScreen(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: options_screen: no memory for the world");
    return;
  }
  Leaves(h, w);
  Handlers(h, w);
}

}  // namespace harness
