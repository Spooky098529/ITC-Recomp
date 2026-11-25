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
// there besides; in Type6Screens' cases one of NodeScreen's words ends as
// Type6PanelScreen's back chain, left out). operator new's four panels are
// blocks of the world (by size) on both sides: the plain C++'s through its
// New, the originals' through operator new's adapter's recorder
// (hooks::g_start_up_recorder). The functions store words without float
// math, so everything is compared exactly.

#include "harness.h"

#include "base/flags.h"
#include "hooks/app_start_up.h"
#include "memory/heap.h"
#include "screen_constructors/screen_constructors.h"

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
  // Stack words the plain C++ writes that end as the originals' back chains.
  std::vector<Leftovers> ignored = {};
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
          if (!InLeftovers(f.leftovers, r1, low + i) && !InLeftovers(f.ignored, r1, low + i) &&
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
// builds, the type 6 block), for the panel set's cases and operator new's
// answers.
constexpr uint32_t kPanelsAt = 0x17000;
constexpr uint32_t kWorldBytes = 0x18000;

// operator new's answer for a panel of `size` bytes: its block in the world
// at `world` (0 for other sizes).
uint32_t PanelBlock(uint32_t world, uint32_t size) {
  switch (size) {
    case 320:
      return world + kPanelsAt;
    case 92:
      return world + kPanelsAt + 320;
    case 160:
      return world + kPanelsAt + 448;
    case 144:
      return world + kPanelsAt + 640;
  }
  return 0;
}

uint32_t g_world = 0;

// operator new's adapter's calls on the originals' side.
bool NewRecorder(PPCContext& ctx, uint32_t function) {
  if (function != 0x825A6880) {
    return false;
  }
  ctx.r3.u64 = PanelBlock(g_world, ctx.r3.u32);
  return true;
}

// The original with --native_screen_constructors off and operator new
// answered from the world.
template <GuestFunction recompiled>
void OriginalNew(PPCContext& ctx, uint8_t* base) {
  const hooks::StartUpRecorder saved = hooks::g_start_up_recorder;
  hooks::g_start_up_recorder = NewRecorder;
  Original<recompiled>(ctx, base);
  hooks::g_start_up_recorder = saved;
}

// The plain C++'s New: the same blocks.
sc::New WorldNew() {
  return {nullptr, [](void*, uint32_t size, uint32_t) { return PanelBlock(g_world, size); }};
}

}  // namespace

void ScreenConstructors(Context& h) {
  Random& random = h.random;
  const Block world(kWorldBytes);
  if (!world.address) return;
  g_world = world.address;

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
  auto with_group = [](void (*f)(uint8_t*, uint32_t, uint32_t)) {
    return [f](PPCContext& t, uint8_t* base) { f(base, t.r3.u32, t.r4.u32); };
  };
  auto group = [](PPCContext& t, uint32_t, Random& r) { t.r4.u64 = uint32_t(r.Next()); };
  const std::vector<Function> more = {
      {"Panel6C::Panel6C (sub_82231CD0)", Original<__imp__sub_82231CD0>,
       with_group(sc::Panel6C), 0, group, {}},
      {"Panel7C::Panel7C (sub_822416D8)", Original<__imp__sub_822416D8>,
       with_group(sc::Panel7C), 0, group, {}},
      {"Panel6D::Panel6D (sub_82231E40)", Original<__imp__sub_82231E40>,
       with_group(sc::Panel6D), 0, group, {}},
      {"Panel7D::Panel7D (sub_822418B8)", Original<__imp__sub_822418B8>,
       with_group(sc::Panel7D), 0, group, {}},
      {"Panel6F::Panel6F (sub_822308E8)", Original<__imp__sub_822308E8>,
       with_group(sc::Panel6F), 0, group, {}},
      {"Panel7F::Panel7F (sub_82240268)", Original<__imp__sub_82240268>,
       with_group(sc::Panel7F), 0, group, {}},
      {"PanelGroup::PanelGroup (sub_82240110)", Original<__imp__sub_82240110>,
       r3(sc::PanelGroup), 0, nullptr, {}},
      {"PanelGroup6B::PanelGroup6B (sub_82231C68)", Original<__imp__sub_82231C68>,
       r3(sc::PanelGroup6B), 0, nullptr, {}},
      {"PanelGroup7B::PanelGroup7B (sub_82241680)", Original<__imp__sub_82241680>,
       r3(sc::PanelGroup7B), 0, nullptr, {}},
      {"PairedNodeTable::PairedNodeTable (sub_82247C60)", Original<__imp__sub_82247C60>,
       r3(sc::PairedNodeTable), 0, nullptr, {}},
      // Its items' masks read the game's records at 0x826370A0 (both sides).
      {"Type7ListScreen::Type7ListScreen (sub_82242BE8)", Original<__imp__sub_82242BE8>,
       r3(sc::Type7ListScreen), 0, nullptr, {}},
  };
  auto with_new = [](void (*f)(uint8_t*, uint32_t, uint32_t, const sc::New&)) {
    return [f](PPCContext& t, uint8_t* base) { f(base, t.r3.u32, t.r1.u32, WorldNew()); };
  };
  // PanelSetBase's words from a panel screen's r1, and those of a block's.
  const int32_t panel_set_base =
      -int32_t(sc::kPanelScreenFrame + sc::kPanelSetFrame);  // PanelSetBase's r1
  const Leftovers panel_words{panel_set_base - 48, panel_set_base - 4};
  const int32_t members = -int32_t(sc::kScreensFrame);
  const Leftovers block_panel_words{members + panel_words.from, members + panel_words.to};
  // NodeScreen's r1 in a Type6NodeScreen's call, and in a block's.
  const int32_t node = -int32_t(sc::kType6NodeScreenFrame);
  const int32_t block_node = members + node;
  const std::vector<Function> screens = {
      {"NodeListScreen::NodeListScreen (sub_82256F68)", Original<__imp__sub_82256F68>,
       r3(sc::NodeListScreen), 0, nullptr, {}},
      {"TitledListScreen::TitledListScreen (sub_822570F0)", Original<__imp__sub_822570F0>,
       r3(sc::TitledListScreen), 0, nullptr, {}},
      {"Type6ListScreen::Type6ListScreen (sub_822330F0)", Original<__imp__sub_822330F0>,
       r3(sc::Type6ListScreen), 0, nullptr, {}},
      {"ColorNodeTable::ColorNodeTable (sub_82247D08)", Original<__imp__sub_82247D08>,
       r3(sc::ColorNodeTable), 0, nullptr, {}},
      {"ColorListScreen::ColorListScreen (sub_822550F8)", Original<__imp__sub_822550F8>,
       r3(sc::ColorListScreen), 0, nullptr, {}},
      // Its caller's frame word (r1 + 20) and its frame's +80 to +100.
      {"NodeScreen::NodeScreen (sub_8225A3D8)", Original<__imp__sub_8225A3D8>,
       [](PPCContext& t, uint8_t* base) {
         t.r3.u64 = sc::NodeScreen(base, t.r3.u32, t.r1.u32);
       },
       0, nullptr, {{20, 24}, {-48, -24}}},
      {"Type6NodeScreen::Type6NodeScreen (sub_82236C30)", Original<__imp__sub_82236C30>,
       [](PPCContext& t, uint8_t* base) { sc::Type6NodeScreen(base, t.r3.u32, t.r1.u32); }, 0,
       nullptr, {{node + 20, node + 24}, {node - 48, node - 24}}},
      {"Type6PanelScreen::Type6PanelScreen (sub_822309C8)", OriginalNew<__imp__sub_822309C8>,
       with_new(sc::Type6PanelScreen), 0, nullptr, {panel_words}},
      {"Type7PanelScreen::Type7PanelScreen (sub_822403A0)", OriginalNew<__imp__sub_822403A0>,
       with_new(sc::Type7PanelScreen), 0, nullptr, {panel_words}},
      // NodeScreen's frame's +80 ends as Type6PanelScreen's back chain.
      {"Type6Screens::Type6Screens (sub_822366F0)", OriginalNew<__imp__sub_822366F0>,
       with_new(sc::Type6Screens), 0, nullptr,
       {{block_node + 20, block_node + 24}, {block_node - 44, block_node - 24}, block_panel_words},
       {{block_node - 48, block_node - 44}}},
      {"Type7Screens::Type7Screens (sub_82246B28)", OriginalNew<__imp__sub_82246B28>,
       with_new(sc::Type7Screens), 0, nullptr, {block_panel_words}},
  };
  for (const Function& f : functions) {
    Run(h, random, world, f, 4);
  }
  for (const Function& f : screens) {
    Run(h, random, world, f, 4);
  }
  for (const Function& f : more) {
    Run(h, random, world, f, 4);
  }
}

}  // namespace harness
