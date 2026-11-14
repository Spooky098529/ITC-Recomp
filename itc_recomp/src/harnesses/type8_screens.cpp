// The type 8 mode's panel screen (src/type8_screens/,
// docs/functions/type8-screens.md) against the recompiled functions. The
// originals run with --native_type8_screens off, the plain C++ through the
// register steps (hooks/type8_screens.h) with the calls
// hooks::Type67ScreensCalls makes. Panel8C's SetStats and ShowArrows run in
// the car_list harness, for its movie and text world.
//
// A world block of random words holds the object (r3 at +0x100, now and
// then with its high half set), the dispatcher (+0x6000) and the stand-ins
// a case needs. The screen fades' counts, the dialog's player, the mode
// before and the movie table's missing word are saved, shaped by the case,
// compared and put back. UiMovies::Open and Delete are recorded (their
// adapters hand the original's calls to hooks::g_start_up_recorder; the
// plain C++'s open and remove record the same words), each call's id, asset,
// frame and return address compared; Open answers 1, Delete 0x5A5A.
// Compared exactly (no float math): the world, the globals, the recorded
// calls, the function's own frame (its back chain and the words above its
// outgoing area, its register saves left out), the caller's area, r1 and r3
// (both halves). The deleting destructors with bit 0 set run on blocks of
// operator new's, compared by r3 and whether the block is handed out
// again.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/app_start_up.h"
#include "hooks/type67_screens.h"
#include "hooks/type8_screens.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "type67_screens/type67_screens.h"
#include "type8_screens/type8_screens.h"

#include <fmt/format.h>

#include <functional>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type8_screens);

namespace harness {

namespace {

namespace t8 = type8_screens;
namespace ts = type67_screens;

constexpr uint32_t kWorldBytes = 0x8000;
constexpr uint32_t kObjectAt = 0x100;
constexpr uint32_t kDispatcherAt = 0x6000;
constexpr uint32_t kStackBytes = 0x400;   // below the call's r1
constexpr uint32_t kCallerBytes = 0x100;  // above it
constexpr uint32_t kOpen = 0x8211E808, kDelete = 0x8211E258;

const std::vector<std::pair<uint32_t, uint32_t>> kGlobals = {
    {0x826A50AC, 0x40},  // the screen fades' words (the counts among them)
    {ts::kFadeWait, 4},       {t8::kDialogPlayer, 4}, {t8::kModeBefore, 4},
    {0x826C5CDC, 4},  // UiMovies::Find's answer for a missing id
};

uint64_t Seed(uint64_t z) {
  z += 0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return z ^ (z >> 31);
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

// The adapters' calls with UiMovies::Open and Delete recorded.
t8::Calls RecordingCalls(PPCContext& t, uint8_t* base) {
  t8::Calls calls = hooks::Type67ScreensCalls(t, base);
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
// leaf or a tail call: the stack below compared whole, nothing written).
struct Frame {
  uint32_t size, saves;
};

using Setup = std::function<void(PPCContext&, Random&, uint32_t world)>;
using Native = std::function<void(PPCContext&, uint8_t*)>;

void Group(Context& h, uint32_t world, const char* name, uint32_t address, int cases,
           Frame frame, const Setup& setup, GuestFunction recompiled, const Native& native) {
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
      // The fade counts 0 mostly (Leave waits on both).
      for (const uint32_t at : {ts::kFadeCountA, ts::kFadeCountB}) {
        guest::StoreU32(base, at, r.Below(2) == 0 ? 0 : r.Below(4));
      }
      // The dispatcher's ring: its count and start small.
      guest::StoreU32(base, world + kDispatcherAt + 204 + 600, r.Below(50));
      guest::StoreU32(base, world + kDispatcherAt + 204 + 604, r.Below(60));
      t.r3.u64 = (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0) | (world + kObjectAt);
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      setup(t, r, world);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_records.clear();
      if (v == 0) {
        const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
        hooks::g_start_up_recorder = Record;
        ITC_FLAG(native_type8_screens) = false;
        recompiled(t, base);
        ITC_FLAG(native_type8_screens) = true;
        hooks::g_start_up_recorder = recorder;
      } else {
        native(t, base);
      }
      out[v].Word(uint32_t(g_records.size()));
      for (const uint32_t word : g_records) out[v].Word(word);
      out[v].Bytes(Read(base, world, kWorldBytes));
      for (const auto& [at, bytes] : kGlobals) out[v].Bytes(Read(base, at, bytes));
      if (frame.size != 0) {
        const uint32_t bottom = sp - frame.size;
        out[v].Bytes(Read(base, bottom, 4));
        out[v].Bytes(Read(base, bottom + 80, frame.size - 80 - frame.saves));
      } else {
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

// A deleting destructor with bit 0 of r4 set: a block of operator new's
// (`bytes`, shaped by `shape`) destroyed and deleted, compared by r3 and
// whether operator new hands the block out again.
void Deleting(Context& h, const char* name, uint32_t address, uint32_t bytes,
              const std::function<void(uint32_t, Random&)>& shape, GuestFunction recompiled,
              GuestFunction native) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{}, deleting (sub_{:08X})", name, address));
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed(uint64_t(address) << 20 ^ uint64_t(c)));
      const auto object = memory::crt::OperatorNew(base, bytes);
      if (!object) {
        ITC_LOG_ERROR("harness: type8_screens: operator new failed");
        return;
      }
      WriteRandom(base, *object, bytes, r);
      shape(*object, r);
      PPCContext t = CallContext(h);
      t.r3.u64 = *object;
      t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
      t.lr = 0x82000000 + 4 * r.Below(1024);
      if (v == 0) {
        ITC_FLAG(native_type8_screens) = false;
        recompiled(t, base);
        ITC_FLAG(native_type8_screens) = true;
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

// The player: 0 or 1, now and then any word.
uint64_t Player(Random& r) {
  return r.Below(4) == 0 ? r.Next() : r.Below(2);
}

void NoSetup(PPCContext&, Random&, uint32_t) {}

// The step with the recording calls (UiMovies::Open and Delete).
Native Recording(uint64_t (*step)(uint32_t, uint32_t, const t8::Calls&)) {
  return [step](PPCContext& t, uint8_t* base) {
    const t8::Calls calls = RecordingCalls(t, base);
    t.r3.u64 = step(t.r1.u32, uint32_t(t.lr), calls);
  };
}

// --- The panels, the group and the screen's leaves ---

void Panels(Context& h, uint32_t world) {
  const auto ctor = [](PPCContext& t, Random& r, uint32_t w) {
    t.r4.u64 = Player(r);
    t.r5.u64 = r.Below(2) == 0 ? r.Next() : w + 0x2000;
  };
  Group(
      h, world, "PanelGroup8::PanelGroup8", 0x82239C10, 6, {112, 24},
      [](PPCContext& t, Random& r, uint32_t) { t.r4.u64 = r.Below(2) == 0 ? 4 : r.Next(); },
      __imp__sub_82239C10, hooks::Type8PanelGroupConstruct);
  Group(h, world, "Panel8F::Panel8F", 0x82239C70, 6, {112, 32}, ctor, __imp__sub_82239C70,
        hooks::Type8Panel8FConstruct);
  Group(h, world, "Panel8E::Panel8E", 0x82239D58, 6, {112, 32}, ctor, __imp__sub_82239D58,
        hooks::Type8Panel8EConstruct);
  Group(h, world, "Panel8C::Panel8C", 0x82239E78, 6, {128, 48}, ctor, __imp__sub_82239E78,
        hooks::Type8Panel8CConstruct);
  Group(h, world, "Panel8D::Panel8D", 0x8223A100, 6, {112, 32}, ctor, __imp__sub_8223A100,
        hooks::Type8Panel8DConstruct);
  Group(h, world, "ConfirmPanel8::ConfirmPanel8", 0x8223A208, 6, {112, 32}, ctor,
        __imp__sub_8223A208, hooks::Type8ConfirmPanelConstruct);
  // The deleting destructors with bit 0 of r4 clear, then set.
  const auto keep = [](PPCContext& t, Random& r, uint32_t) { t.r4.u64 = r.Next() & ~1ull; };
  Group(h, world, "PanelGroup8::DeletingDestructor", 0x8223A2A8, 4, {112, 24}, keep,
        __imp__sub_8223A2A8, hooks::Type8PanelGroupDeletingDestruct);
  Group(h, world, "Panel8C::DeletingDestructor", 0x8223A348, 4, {112, 24}, keep,
        __imp__sub_8223A348, hooks::Type8Panel8CDeletingDestruct);
  const auto random = [](uint32_t, Random&) {};
  Deleting(h, "PanelGroup8::DeletingDestructor", 0x8223A2A8, 44, random, __imp__sub_8223A2A8,
           hooks::Type8PanelGroupDeletingDestruct);
  Deleting(h, "Panel8C::DeletingDestructor", 0x8223A348, 324, random, __imp__sub_8223A348,
           hooks::Type8Panel8CDeletingDestruct);
}

void Leaves(Context& h, uint32_t world) {
  Group(h, world, "Type8PanelScreen::InitWords", 0x82238420, 4, {0, 0}, NoSetup,
        __imp__sub_82238420, hooks::Type8InitWords);
  Group(h, world, "Type8PanelScreen::IsLoaded", 0x82238418, 2, {0, 0}, NoSetup,
        __imp__sub_82238418, hooks::Type8IsLoaded);
  Group(h, world, "Type8PanelScreen::OpenMovies", 0x82238440, 4, {0, 0}, NoSetup,
        __imp__sub_82238440, Recording(t8::OpenMovies));
  Group(h, world, "Type8PanelScreen::DeleteMovies", 0x82238450, 4, {0, 0}, NoSetup,
        __imp__sub_82238450, Recording(t8::DeleteMovies));
  for (const auto& [name, address, recompiled, native] :
       {std::tuple{"Type8PanelScreen::OnMessage2", 0x82238458u, __imp__sub_82238458,
                   hooks::Type8OnMessage2},
        std::tuple{"Type8PanelScreen::OnSaveLoaded", 0x82238530u, __imp__sub_82238530,
                   hooks::Type8OnSaveLoaded},
        std::tuple{"Type8PanelScreen::OnAnswer0", 0x82238540u, __imp__sub_82238540,
                   hooks::Type8OnAnswer0},
        std::tuple{"Type8PanelScreen::OnAnswer1", 0x82238550u, __imp__sub_82238550,
                   hooks::Type8OnAnswer1},
        std::tuple{"Type8PanelScreen::OnAnswer2", 0x82238560u, __imp__sub_82238560,
                   hooks::Type8OnAnswer2}}) {
    Group(h, world, name, address, 3, {0, 0}, NoSetup, recompiled, native);
  }
  Group(
      h, world, "Type8PanelScreen::Leave", 0x82238B10, 10, {0, 0},
      [base = h.base](PPCContext& t, Random& r, uint32_t w) {
        guest::StoreU32(base, w + kObjectAt + 15240, r.Below(2));
        t.r4.u64 = w + kDispatcherAt;
      },
      __imp__sub_82238B10, hooks::Type8Leave);
}

}  // namespace

void Type8Screens(Context& h) {
  const uint32_t world = memory::Alloc(kWorldBytes, 0);
  if (world == 0) {
    ITC_LOG_ERROR("harness: type8_screens: no world block");
    return;
  }
  Panels(h, world);
  Leaves(h, world);
  memory::Free(world);
}

}  // namespace harness
