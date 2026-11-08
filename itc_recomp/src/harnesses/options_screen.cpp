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
#include "hooks/options_screen.h"
#include "memory/heap.h"
#include "options_screen/options_screen.h"

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
// each: the page block, the menu, the settings, then four more).
constexpr uint32_t kBlockBytes = 0x400;
struct World {
  World() : blocks(7 * kBlockBytes) {}
  bool ok() const { return blocks.address != 0; }
  uint32_t block(uint32_t global) const {
    return blocks.address + (global - kGlobals) / 4 * kBlockBytes;
  }
  uint32_t page() const { return block(os::kPage); }
  uint32_t menu() const { return block(os::kMenu); }
  uint32_t settings() const { return block(os::kSettings); }
  Block blocks;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// The original's frames compared, as [sp - from, sp - to) ranges (its
// register saves left out).
using Frames = std::vector<std::pair<uint32_t, uint32_t>>;

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const Frames& frames,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{kGlobals, kGlobalsBytes}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.blocks.address, w.blocks.bytes, r);
    for (uint32_t g = kGlobals; g < kGlobals + kGlobalsBytes; g += 4) {
      guest::StoreU32(h.base, g, w.block(g));
    }
    t.r3.u64 = r.Next();
    t.r4.u64 = r.Next();
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    ITC_FLAG(native_options_screen) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_options_screen) = true;
    out[v].Bytes(Read(h.base, w.blocks.address, w.blocks.bytes));
    out[v].Bytes(Read(h.base, kGlobals, kGlobalsBytes));
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
           GuestFunction recompiled, GuestFunction native, const Frames& frames = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, frames,
         fmt::format("case {}", c));
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

}  // namespace

void OptionsScreen(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: options_screen: no memory for the world");
    return;
  }
  Leaves(h, w);
}

}  // namespace harness
