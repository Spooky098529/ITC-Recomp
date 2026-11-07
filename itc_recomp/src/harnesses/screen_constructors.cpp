// The type 6 and type 7 modes' screen constructors
// (src/screen_constructors/, docs/functions/screen-constructors.md) against
// the recompiled functions. The originals run with
// --native_screen_constructors off (their callees' originals too, but for
// ScreenMachine::ScreenMachine's, checked by the boot mode's harness), the
// plain C++ directly. Each case fills a world block (the object, and the
// panels a panel set is handed) and the stack around the call's r1 with the
// same random words for both sides; the panel word (0x82815EAC) is random
// before each side and put back after. Compared: the world block, the panel
// word, r3, and the stack words the originals leave (NodeScreen's,
// PanelSetBase's); the plain C++ must leave every other stack word as it
// was (the originals write only their saved registers and back chains
// there besides). The functions store words without float math, so
// everything is compared exactly.

#include "harness.h"

#include "base/flags.h"
#include "screen_constructors/screen_constructors.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <functional>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_screen_constructors);

namespace harness {

namespace {

namespace sc = screen_constructors;

// A block of guest memory for the cases, freed at the end.
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

// The original with --native_screen_constructors off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_screen_constructors) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_screen_constructors) = true;
}

// The stack around a call's r1 the cases fill and compare.
constexpr uint32_t kStackBelow = 2048;
constexpr uint32_t kStackAbove = 64;

// A stack range the original leaves words in: [from, to) from r1 on entry.
struct Leftovers {
  int32_t from;
  int32_t to;
};

// One function's cases.
struct Function {
  const char* name;
  GuestFunction original;
  // The plain C++ on the call's registers (r3 the object).
  std::function<void(PPCContext& t, uint8_t* base)> native;
  // The object's offset in the world block, and the registers past r3
  // (`world` the block's address, `r` the case's random words).
  uint32_t object_at;
  std::function<void(PPCContext& t, uint32_t world, Random& r)> args;
  std::vector<Leftovers> leftovers;
};

bool InLeftovers(const std::vector<Leftovers>& leftovers, uint32_t r1, uint32_t address) {
  for (const Leftovers& l : leftovers) {
    if (address >= r1 + l.from && address < r1 + l.to) {
      return true;
    }
  }
  return false;
}

void Run(Context& h, Random& random, const Block& world, const Function& f, int cases) {
  Results results(f.name);
  for (int c = 0; c < cases; c++) {
    Outputs out[2];
    const uint64_t seed = random.Next();
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      WriteRandom(h.base, world.address, world.bytes, r);
      PPCContext t = CallContext(h);
      const uint32_t r1 = t.r1.u32;
      const uint32_t low = r1 - kStackBelow;
      WriteRandom(h.base, low, kStackBelow + kStackAbove, r);
      const std::vector<uint8_t> stack_before = Read(h.base, low, kStackBelow + kStackAbove);
      const uint32_t saved_word = guest::LoadU32(h.base, sc::kPanelWord);
      guest::StoreU32(h.base, sc::kPanelWord, uint32_t(r.Next()));
      t.r3.u64 = world.address + f.object_at;
      if (f.args) {
        f.args(t, world.address, r);
      }
      if (v == 0) {
        f.original(t, h.base);
      } else {
        f.native(t, h.base);
      }
      out[v].Bytes(Read(h.base, world.address, world.bytes));
      out[v].Word(guest::LoadU32(h.base, sc::kPanelWord));
      guest::StoreU32(h.base, sc::kPanelWord, saved_word);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (const Leftovers& l : f.leftovers) {
        out[v].Bytes(Read(h.base, r1 + l.from, uint32_t(l.to - l.from)));
      }
      if (v == 1) {
        // The plain C++ writes no other stack word.
        const std::vector<uint8_t> after = Read(h.base, low, kStackBelow + kStackAbove);
        uint32_t written = 0;
        for (uint32_t i = 0; i + 4 <= after.size(); i += 4) {
          if (!InLeftovers(f.leftovers, r1, low + i) &&
              !std::equal(after.begin() + i, after.begin() + i + 4, stack_before.begin() + i)) {
            written++;
          }
        }
        out[0].Word(0);
        out[1].Word(written);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// A panel's offset in the world block (past the biggest object a case
// builds), for the panel set's cases.
constexpr uint32_t kPanelsAt = 0x10000;

}  // namespace

void ScreenConstructors(Context& h) {
  Random& random = h.random;
  const Block world(0x18000);
  if (!world.address) return;

  auto r3 = [](void (*f)(uint8_t*, uint32_t)) {
    return [f](PPCContext& t, uint8_t* base) { f(base, t.r3.u32); };
  };
  // Four panels for the panel set: each 320 bytes apart, from kPanelsAt.
  auto panels = [](PPCContext& t, uint32_t world_address, Random& r) {
    t.r4.u64 = world_address + kPanelsAt;
    t.r5.u64 = world_address + kPanelsAt + 320;
    t.r6.u64 = world_address + kPanelsAt + 640;
    t.r7.u64 = world_address + kPanelsAt + 960;
    t.r8.u64 = uint32_t(r.Next());
  };

  const std::vector<Function> functions = {
      {"PanelSet::PanelSet (sub_8224B660)", Original<__imp__sub_8224B660>,
       [](PPCContext& t, uint8_t* base) {
         sc::PanelSet(base, t.r3.u32, t.r4.u32, t.r5.u32, t.r6.u32, t.r7.u32, t.r8.u32,
                      t.r1.u32);
       },
       0, panels,
       // PanelSetBase's eleven words below its r1 (PanelSet's frame).
       {{-int32_t(sc::kPanelSetFrame) - 48, -int32_t(sc::kPanelSetFrame) - 4}}},
      {"PanelE::PanelE (sub_82230800)", Original<__imp__sub_82230800>,
       [](PPCContext& t, uint8_t* base) { sc::PanelE(base, t.r3.u32, t.r4.u32); }, 0,
       [](PPCContext& t, uint32_t, Random& r) { t.r4.u64 = uint32_t(r.Next()); }, {}},
      {"ListScreen::ListScreen (sub_82252290)", Original<__imp__sub_82252290>,
       r3(sc::ListScreen), 0, nullptr, {}},
      {"ListPart::ListPart (sub_82252908)", Original<__imp__sub_82252908>, r3(sc::ListPart), 0,
       nullptr, {}},
      {"PanelCBase::PanelCBase (sub_8224D6D0)", Original<__imp__sub_8224D6D0>,
       r3(sc::PanelCBase), 0, nullptr, {}},
      {"PanelGroupBase::PanelGroupBase (sub_8224EEA8)", Original<__imp__sub_8224EEA8>,
       r3(sc::PanelGroupBase), 0, nullptr, {}},
      {"PanelFBase::PanelFBase (sub_8224FA88)", Original<__imp__sub_8224FA88>,
       r3(sc::PanelFBase), 0, nullptr, {}},
      {"NodeTable::NodeTable (sub_82247BE8)", Original<__imp__sub_82247BE8>, r3(sc::NodeTable),
       0, nullptr, {}},
      {"PanelDBase::PanelDBase (sub_8224CFA0)", Original<__imp__sub_8224CFA0>,
       r3(sc::PanelDBase), 0, nullptr, {}},
      {"MachineScreen::MachineScreen (sub_8224CF58)", Original<__imp__sub_8224CF58>,
       r3(sc::MachineScreen), 0, nullptr, {}},
      {"Type6Screen0::Type6Screen0 (sub_82235BD8)", Original<__imp__sub_82235BD8>,
       r3(sc::Type6Screen0), 0, nullptr, {}},
      {"GroupPair::GroupPair (sub_82249C08)", Original<__imp__sub_82249C08>,
       [](PPCContext& t, uint8_t* base) {
         sc::GroupPair(base, t.r3.u32, t.r4.u32, t.r5.u32);
       },
       0,
       [](PPCContext& t, uint32_t, Random& r) {
         t.r4.u64 = uint32_t(r.Next());
         t.r5.u64 = uint32_t(r.Next());
       },
       {}},
      {"Type7Screen0::Type7Screen0 (sub_82245FF8)", Original<__imp__sub_82245FF8>,
       r3(sc::Type7Screen0), 0, nullptr, {}},
  };
  for (const Function& f : functions) {
    Run(h, random, world, f, 4);
  }
}

}  // namespace harness
