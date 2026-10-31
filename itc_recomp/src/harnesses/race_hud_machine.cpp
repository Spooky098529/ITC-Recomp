// The race HUD machine (src/race_hud_machine/,
// docs/functions/race-hud-machine.md) against the recompiled functions. The
// originals run with --native_race_hud_machine off, the plain C++ through
// the register steps (hooks/race_hud_machine.h). The handlers' callees (the
// race HUD's and the alternate HUD's functions, checked by their own
// harnesses) are stood in for on both sides (hooks/callee_stand_in.h: the
// recompiled handlers reach the stand-in through the callees' adapters, the
// native ones through their calls of the callees' plain C++): each call's
// function, return address, r1, r3 and (for the callees that read it) r4 is
// recorded and answered with an r3 (0 a third of the time) and an f1 from
// the case's generator. So a case compares the calls each side makes, in
// order, and what each side writes: the machine, the holder, the
// dispatcher's ring, the original's frame (its register saves left out), r1,
// r3 and f1. Kind 4, and kind 0 where the handler draws the holder's +12,
// aren't ported (the adapters run the originals there), so the cases leave
// them out. The game's globals the cases point at blocks of their own are
// put back after each side. Compared with harness::Outputs.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "game_modes/game_modes.h"
#include "hooks/callee_stand_in.h"
#include "hooks/race_hud_machine.h"
#include "memory/heap.h"
#include "race_hud_machine/race_hud_machine.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_hud_machine);

namespace harness {

namespace {

namespace rhm = race_hud_machine;

constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRacers = 0x826D82F4;
constexpr uint32_t kSaveSlots = 0x826C5B58;
constexpr uint32_t kMachineBytes = 0x2600;
constexpr uint32_t kHolderBytes = 0x500;
constexpr uint32_t kDispatcherBytes = 0x400;
constexpr uint32_t kRaceBytes = 0xA4000;  // to the kind (+0xA3AF0)
constexpr uint32_t kSaveBytes = 0x1B000;  // to the items' records (+0x199F8 + 12 x 409)
constexpr uint32_t kMiscBytes = 0x1000;  // the racers' manager (+0), its entry (+0xA00), the tables (+0xB00)
constexpr uint32_t kFirstItem = 370;      // the items block's first id
constexpr uint32_t kItems = 40;
constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it

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

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  World()
      : machine(kMachineBytes),
        holder(kHolderBytes),
        dispatcher(kDispatcherBytes),
        race(kRaceBytes),
        save(kSaveBytes),
        misc(kMiscBytes),
        items(3008 * kItems) {}
  bool ok() const {
    return machine.address && holder.address && dispatcher.address && race.address &&
           save.address && misc.address && items.address;
  }
  Block machine;
  Block holder;
  Block dispatcher;
  Block race;
  Block save;
  Block misc;
  Block items;
};

// The stand-in's record of a callee's call.
struct Call {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<Call>* g_calls = nullptr;
Random g_answers;

// The callees the stand-in takes (every callee of the handlers), and those
// that read r4.
bool Takes(uint32_t function, bool* reads_r4) {
  switch (function) {
    case rhm::kHudTick:
    case rhm::kHudRunMovie:
    case rhm::kHudSetPlayer:
    case rhm::kAltSetUp:
      *reads_r4 = true;
      return true;
    case rhm::kHudUpdateView:
    case rhm::kHudUpdateWarning:
    case rhm::kHudUpdateItem:
    case rhm::kHudTickPanel:
    case rhm::kHudDraw:
    case rhm::kHudRunWarning:
    case rhm::kHudRunItem:
    case rhm::kHudRunCountdown:
    case rhm::kHudStartMovie1:
    case rhm::kHudFinishMovie1:
    case rhm::kHudRunMovie1:
    case rhm::kHudReset:
    case rhm::kHudClearWarning:
    case rhm::kHudSetUpGauges:
    case rhm::kHudResetBar:
    case rhm::kHudSetUpExtraGauges:
    case rhm::kHudSetUpMirror:
    case rhm::kHudApplyOptions:
    case rhm::kHudSetUpSigns:
    case rhm::kAltTick:
    case rhm::kAltRunMovie:
      *reads_r4 = false;
      return true;
    default:
      return false;
  }
}

bool StandIn(PPCContext& ctx, uint32_t function) {
  bool reads_r4 = false;
  if (g_calls == nullptr || !Takes(function, &reads_r4)) {
    return false;
  }
  g_calls->push_back(
      {function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, reads_r4 ? ctx.r4.u64 : 0});
  ctx.r3.u64 = g_answers.Below(3) == 0 ? 0 : g_answers.Next();
  ctx.f1.u64 = g_answers.Next();
  return true;
}

// A kind for a case: one of `kinds`, or now and then any word.
uint32_t AnyKind(Random& r, const std::vector<int32_t>& kinds) {
  if (r.Below(10) == 0) {
    const uint32_t word = uint32_t(r.Next());
    for (const int32_t k : kinds) {
      if (uint32_t(k) == word) {
        return word;
      }
    }
    return word < 7 ? 7 : word;  // never 0 to 6 by chance
  }
  return uint32_t(kinds[r.Below(uint32_t(kinds.size()))]);
}

// What a case compares besides the blocks: the original's frame
// [sp - frame, sp - saves) (its register saves left out).
struct Spec {
  uint32_t frame = 0;
  uint32_t saves = 0;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const Spec& spec,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{kRaceData, 4},
                              {kRacers, 4},
                              {kSaveSlots, 16},
                              {data_tables::kTables, 4}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.machine.address + 9600, 32, r);
    WriteRandom(h.base, w.holder.address, w.holder.bytes, r);
    WriteRandom(h.base, w.dispatcher.address, w.dispatcher.bytes, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    guest::StoreU32(h.base, w.machine.address + rhm::kHolder, w.holder.address);
    guest::StoreU32(h.base, kRaceData, w.race.address);
    guest::StoreU32(h.base, kRacers, w.misc.address);
    guest::StoreU32(h.base, w.misc.address + 2308, w.misc.address + 0xA00);
    for (uint32_t p = 0; p < 4; p++) {
      guest::StoreU32(h.base, kSaveSlots + 4 * p, w.save.address);
    }
    guest::StoreU32(h.base, data_tables::kTables, w.misc.address + 0xB00);
    guest::StoreU32(h.base, w.misc.address + 0xB00 + 16, w.items.address - 3008 * kFirstItem);
    // The ring's count and start small (its entries within the block).
    guest::StoreU32(h.base, w.dispatcher.address + 804, r.Below(60));
    guest::StoreU32(h.base, w.dispatcher.address + 808, r.Below(60));
    t.r3.u64 = w.machine.address;
    t.r4.u64 = w.dispatcher.address;
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    t.f1.u64 = r.Next();
    g_answers = Random(r.Next());
    std::vector<Call> calls;
    g_calls = &calls;
    const hooks::CalleeStandIn saved_stand_in = hooks::g_callee_stand_in;
    hooks::g_callee_stand_in = StandIn;
    ITC_FLAG(native_race_hud_machine) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_hud_machine) = true;
    hooks::g_callee_stand_in = saved_stand_in;
    g_calls = nullptr;
    out[v].Word(uint32_t(calls.size()));
    for (const Call& call : calls) {
      out[v].Word(call.function);
      out[v].Word(call.lr);
      out[v].Word(call.r1);
      for (const uint64_t x : {call.r3, call.r4}) {
        out[v].Word(uint32_t(x >> 32));
        out[v].Word(uint32_t(x));
      }
    }
    out[v].Bytes(Read(h.base, w.machine.address + 9600, 32));
    out[v].Bytes(Read(h.base, w.holder.address, w.holder.bytes));
    out[v].Bytes(Read(h.base, w.dispatcher.address, w.dispatcher.bytes));
    out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
    if (spec.frame > spec.saves) {
      out[v].Bytes(Read(h.base, sp - spec.frame, spec.frame - spec.saves));
    }
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    out[v].Double(t.f1.u64);
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, const Spec& spec) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, spec,
         fmt::format("case {}", c));
  }
  results.Report();
}

// The handlers' cases: the kind (signed handlers: 0 does nothing; the
// draws: 0 is the +12 screen's, left out), the replay bit (+8 bit 0) and
// the countdown's word (+452) 0 half the time, the kind 6 racer record's
// word +3888 with or without bit 0x400.
void Handlers(Context& h, const World& w) {
  const std::vector<int32_t> signed_kinds = {-1, 0, 1, 2, 3, 5, 6, 7, 8};
  const std::vector<int32_t> draw_kinds = {-1, 1, 2, 3, 5, 6, 7, 8};
  auto setup = [&h](std::vector<int32_t> kinds) -> SetupFn {
    return [&h, kinds](PPCContext&, Random& r, const World& w) {
      guest::StoreU32(h.base, w.machine.address + rhm::kKind, AnyKind(r, kinds));
      guest::StoreU32(h.base, w.holder.address + rhm::kHolderFlags,
                      (uint32_t(r.Next()) & ~1u) | r.Below(2));
      guest::StoreU32(h.base, w.holder.address + rhm::kCountdownRunning,
                      r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
      const uint32_t index = r.Below(4);
      guest::StoreU32(h.base, w.misc.address + 0xA00 + 24, index);
      const uint32_t record_word = w.race.address + 3936 * (index + 1);
      guest::StoreU32(h.base, record_word,
                      (uint32_t(r.Next()) & ~0x400u) | (r.Below(2) == 0 ? 0x400u : 0));
    };
  };
  const SetupFn by_signed = setup(signed_kinds);
  const SetupFn by_draw = setup(draw_kinds);
  const Spec f96{96, 16};
  const Spec f112{112, 24};
  Group(h, w, "RaceHudMachine::PreRaceUpdate (sub_8216C210)", 60, by_signed, __imp__sub_8216C210,
        hooks::RaceHudMachinePreRaceUpdate, f96);
  Group(h, w, "RaceHudMachine::PreRaceDraw (sub_8216C2A0)", 60, by_draw, __imp__sub_8216C2A0,
        hooks::RaceHudMachinePreRaceDraw, f96);
  Group(h, w, "RaceHudMachine::CountdownDraw (sub_8216C330)", 60, by_draw, __imp__sub_8216C330,
        hooks::RaceHudMachineCountdownDraw, f96);
  Group(h, w, "RaceHudMachine::StartIntro (sub_8216C510)", 10, by_signed, __imp__sub_8216C510,
        hooks::RaceHudMachineStartIntro, Spec{});
  Group(h, w, "RaceHudMachine::EndIntro (sub_8216C520)", 10, by_signed, __imp__sub_8216C520,
        hooks::RaceHudMachineEndIntro, Spec{});
  Group(h, w, "RaceHudMachine::IntroUpdate (sub_8216C530)", 60, by_signed, __imp__sub_8216C530,
        hooks::RaceHudMachineIntroUpdate, f96);
  Group(h, w, "RaceHudMachine::IntroDraw (sub_8216C5B8)", 60, by_draw, __imp__sub_8216C5B8,
        hooks::RaceHudMachineIntroDraw, f96);
  Group(h, w, "RaceHudMachine::CountdownUpdate (sub_8216CB98)", 60, by_signed,
        __imp__sub_8216CB98, hooks::RaceHudMachineCountdownUpdate, f112);
  Group(h, w, "RaceHudMachine::RaceDraw (sub_8216CE00)", 60, by_draw, __imp__sub_8216CE00,
        hooks::RaceHudMachineRaceDraw, f96);
  Group(h, w, "RaceHudMachine::RaceUpdate (sub_8216D918)", 80, by_signed, __imp__sub_8216D918,
        hooks::RaceHudMachineRaceUpdate, f112);
  // __savegprlr_24: r24 to r31 and lr in the 80 bytes below sp.
  Group(h, w, "RaceHudMachine::SetUp (sub_8216D450)", 80, by_signed, __imp__sub_8216D450,
        hooks::RaceHudMachineSetUp, Spec{160, 80});
}

// The helpers: IsFirstDay by the race data's kind (0 to 2, now and then
// any word) and player 0's day (0, 1, 2, 999, 1,000, -1 or any word);
// OpponentPrize and ItemBonus on ids -1 and 370 to 409 with the items'
// bytes +35 (-2 to 7, or any byte) and +37 and the save data's records'
// halfwords +0, +2 (1 half the time) and +4 random.
void Helpers(Context& h, const World& w) {
  auto prizes = [&h](PPCContext& t, Random& r, const World& w) {
    const uint32_t id = r.Below(12) == 0 ? 0xFFFFFFFFu : kFirstItem + r.Below(kItems);
    t.r4.u64 = r.Below(8) == 0 ? (uint64_t(r.Next()) << 32) | id : id;
    for (uint32_t i = 0; i < kItems; i++) {
      const uint32_t item = w.items.address + 3008 * i;
      guest::StoreU8(h.base, item + 35,
                     r.Below(8) == 0 ? uint8_t(r.Next()) : uint8_t(int8_t(r.Below(10)) - 2));
      guest::StoreU8(h.base, item + 37, uint8_t(r.Next()));
      const uint32_t record = w.save.address + 0x199F8 + 12 * (kFirstItem + i);
      guest::StoreU16(h.base, record, r.Below(2) == 0 ? 0 : uint16_t(r.Next()));
      guest::StoreU16(h.base, record + 2, r.Below(2) == 0 ? 1 : uint16_t(r.Below(4)));
      guest::StoreU16(h.base, record + 4, r.Below(2) == 0 ? 0 : uint16_t(r.Next()));
    }
  };
  Group(h, w, "RaceHudMachine::OpponentPrize (sub_8216C820)", 80, prizes, __imp__sub_8216C820,
        hooks::RaceHudMachineOpponentPrize, Spec{112, 24});
  Group(h, w, "RaceHudMachine::ItemBonus (sub_8216C938, with sub_8216C9B8)", 80, prizes,
        __imp__sub_8216C938, hooks::RaceHudMachineItemBonus, Spec{96, 16});
  auto first_day = [&h](PPCContext&, Random& r, const World& w) {
    guest::StoreU32(h.base, w.race.address + 0xA3AF0,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3));
    static constexpr uint32_t kDays[] = {0, 1, 2, 999, 1000, 0xFFFFFFFFu};
    guest::StoreU32(h.base, w.save.address + 6100,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : kDays[r.Below(6)]);
  };
  Group(h, w, "RaceHudMachine::IsFirstDay (sub_8216D3F0)", 40, first_day, __imp__sub_8216D3F0,
        hooks::RaceHudMachineIsFirstDay, Spec{96, 8});
}

// --- RaceHudMachine::Init: an empty state machine or one with random
// transitions already (fewer than 130, so the 65 fit), some with the keys
// Init adds (the last, (24, 50), among them: its r3 is then the index); the
// original's frame filled with the same random words for both (the words it
// never writes go into the transitions); the machine, the frame (but its
// back chain and saved registers) and r3 compared ---

void Inits(Context& h, const World& w) {
  namespace gm = game_modes;
  constexpr uint32_t kSaves = 144;  // __savegprlr_15: r15 at -144, lr at -8
  Results results("RaceHudMachine::Init (sub_8216E6E8)");
  const uint32_t machine = w.machine.address;
  const uint32_t frame = CallContext(h).r1.u32 - rhm::kInitFrame;
  constexpr uint32_t kPlanted[][2] = {{0, 0}, {9, 29}, {22, 49}, {24, 50}};
  for (uint64_t seed = 0; seed < 8; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 157 + 41);
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, machine, kMachineBytes, r);
      WriteRandom(h.base, frame, rhm::kInitFrame, r);
      if (seed % 2 == 0) {
        gm::StateMachineConstruct(h.base, machine);
        guest::StoreU32(h.base, machine + gm::kTransitionCount, 0);
      } else {
        const uint32_t count = 8 + r.Below(120);
        guest::StoreU32(h.base, machine + gm::kTransitionCount, count);
        for (uint32_t i = 0; i < 200; i++) {  // keys of small numbers, some matching
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(h.base, machine + 48 * i + 4 * k, r.Below(k < 2 ? 60 : 2));
          }
        }
        for (const auto& key : kPlanted) {
          if (seed % 4 == 1 && key[0] == 24) {
            continue;  // seeds 1 and 5: the last Add makes a new transition
          }
          const uint32_t at = machine + 48 * r.Below(count);
          guest::StoreU32(h.base, at, key[0]);
          guest::StoreU32(h.base, at + 4, key[1]);
          guest::StoreU32(h.base, at + 8, 0);
          guest::StoreU32(h.base, at + 12, 0);
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = machine;
      if (v == 0) {
        ITC_FLAG(native_race_hud_machine) = false;
        __imp__sub_8216E6E8(t, h.base);
        ITC_FLAG(native_race_hud_machine) = true;
      } else {
        hooks::RaceHudMachineInit(t, h.base);
      }
      out[v].Bytes(Read(h.base, machine, kMachineBytes));
      out[v].Bytes(Read(h.base, frame + 80, rhm::kInitFrame - 80 - kSaves));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

}  // namespace

void RaceHudMachine(Context& h) {
  const World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_hud_machine: no blocks from the allocation layer");
    return;
  }
  Inits(h, w);
  Handlers(h, w);
  Helpers(h, w);
}

}  // namespace harness
