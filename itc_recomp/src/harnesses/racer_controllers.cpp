// The racer controllers (src/racer_controllers/,
// docs/functions/racer-controllers.md) against the recompiled functions.
// Each case points the globals the function reads (the race data, the cars'
// input records, the recordings, the race) at blocks of its own, filled
// with random words and then the fields the function tells apart; both
// versions run on the same bytes, and the blocks' parts it writes, the
// globals and r3 are compared exactly (the controllers move words and
// integers only). The globals are put back after each version. The
// originals run with --native_racer_controllers off.

#include "harness.h"

#include "ai_drivers/ai_drivers.h"
#include "base/flags.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "hooks/ai_drivers.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "racer_controllers/racer_controllers.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <iterator>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_racer_controllers);

namespace harness {

namespace {

namespace rc = racer_controllers;

// Runs the recompiled version with the native flag off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  const bool saved = ITC_FLAG(native_racer_controllers);
  ITC_FLAG(native_racer_controllers) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_racer_controllers) = saved;
}

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
void NativeAppend(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rc::RecordingAppend(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeHasRoom(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rc::RecordingHasRoom(base, ctx.r3.u32);
}
void NativeOnSetUp(PPCContext& ctx, uint8_t* base) {
  uint32_t r3 = 0;
  if (rc::OnSetUp(base, ctx.r3.u32, &r3)) {
    ctx.r3.u64 = r3;
  } else {
    Original<__imp__sub_8215F728>(ctx, base);
  }
}
void NativeOnEnd(PPCContext& ctx, uint8_t* base) {
  uint32_t last = 0;
  if (rc::DestroyInputs(base, &last)) {
    ctx.r3.u64 = last;
  } else {
    Original<__imp__sub_8215F5E8>(ctx, base);
  }
}

// The AI drivers' stand-ins (hooks/ai_drivers.h), for both versions: each
// call logged (its registers: r3 to r5, r1, the link register) and answered
// with a record of words from the call's count and arguments, f1 = the
// count.
std::vector<uint32_t> g_stand_in_log;
uint32_t g_stand_in_calls = 0;
uint32_t Mix(uint32_t x) {
  x ^= x >> 16;
  x *= 0x7FEB352Du;
  x ^= x >> 15;
  x *= 0x846CA68Bu;
  return x ^ (x >> 16);
}
void StandInDriveCar(PPCContext& ctx, uint8_t* base) {
  g_stand_in_log.insert(g_stand_in_log.end(), {0xD0000000u, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r1.u32, uint32_t(ctx.lr)});
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, ctx.r3.u32 + 4 * i,
                    Mix(g_stand_in_calls * 7 + i + ctx.r4.u32 * 131 + ctx.r5.u32 * 977));
  }
  ctx.f1.f64 = double(g_stand_in_calls);
  g_stand_in_calls++;
}
void StandInUpdateIdle(PPCContext& ctx, uint8_t* base) {
  g_stand_in_log.insert(g_stand_in_log.end(), {0xD1000000u, uint32_t(ctx.r3.u64 >> 32),
                                               ctx.r3.u32, ctx.r1.u32, uint32_t(ctx.lr)});
  ctx.r3.u64 = Mix(g_stand_in_calls++);
}
// While alive, the stand-ins answer and the log starts empty.
struct StandIns {
  StandIns() {
    hooks::g_drive_car_stand_in = StandInDriveCar;
    hooks::g_update_idle_stand_in = StandInUpdateIdle;
    Reset();
  }
  ~StandIns() {
    hooks::g_drive_car_stand_in = nullptr;
    hooks::g_update_idle_stand_in = nullptr;
  }
  static void Reset() {
    g_stand_in_log.clear();
    g_stand_in_calls = 0;
  }
};

// The adapters' calls (hooks/racer_controllers.cpp) for the native side.
struct HarnessCalls {
  PPCContext* ctx;
  uint8_t* base;
};
void CallDriveCar(void* user, uint32_t input, uint32_t index, uint32_t flag, uint32_t frame,
                  uint32_t lr) {
  auto& call = *static_cast<HarnessCalls*>(user);
  PPCContext& ctx = *call.ctx;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = input;
  ctx.r4.u64 = index;
  ctx.r5.u64 = flag;
  hooks::AiDriveCar(ctx, call.base);
  ctx.r1.u64 = saved;
}
uint64_t CallGuest(void* user, uint32_t function, uint64_t r3, uint64_t r4, bool set_r4,
                   uint32_t frame, uint32_t lr, double* f1) {
  auto& call = *static_cast<HarnessCalls*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  if (set_r4) {
    ctx.r4.u64 = r4;
  }
  ctx.ctr.u64 = function;
  REX_CALL_INDIRECT_FUNC(function);
  ctx.r1.u64 = saved;
  if (f1 != nullptr) {
    *f1 = ctx.f1.f64;
  }
  return ctx.r3.u64;
}
void CallUpdateIdle(void* user, uint64_t driver, uint32_t frame, uint32_t lr, const double* f1) {
  auto& call = *static_cast<HarnessCalls*>(user);
  PPCContext& ctx = *call.ctx;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  ctx.r3.u64 = driver;
  if (f1 != nullptr) {
    ctx.f1.f64 = *f1;
  }
  hooks::AiUpdateIdle(ctx, call.base);
  ctx.r1.u64 = saved;
}
// Whether the native side reads the mappers' slots natively (alternated by
// the cases; the adapters use --native_action_mappers).
bool g_native_slots = true;
#define ITC_HARNESS_CALLS          \
  HarnessCalls call{&ctx, base};   \
  const rc::Calls calls { &call, CallDriveCar, CallGuest, CallUpdateIdle, g_native_slots }

void NativeDriveByAi(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  rc::DriveByAi(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls, &ctx.r3.u64);
}
void NativeHoldAll(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  ctx.r3.u64 = rc::HoldAll(base, ctx.r1.u32, calls);
}
void NativeOnState16(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  ctx.r3.u64 = rc::OnState16(base, ctx.r1.u32, calls);
}
void NativeClearAllRecords(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rc::ClearAllRecords(base, ctx.r1.u32);
}
void SetRegs(PPCContext& ctx, const rc::Regs& regs) {
  ctx.r3.u64 = regs.r3;
  if (regs.f1_set) {
    ctx.f1.f64 = regs.f1;
  }
}
void NativeRead(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  rc::Regs regs{ctx.r3.u64, 0, false};
  rc::Read(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32, calls, &regs);
  SetRegs(ctx, regs);
}
void NativeControl(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  rc::Regs regs{ctx.r3.u64, 0, false};
  const uint32_t answer =
      rc::Control(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32, calls, &regs);
  SetRegs(ctx, regs);
  ctx.r3.u64 = answer;
}
void NativeIsConnected(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  ctx.r3.u64 = rc::PadIsConnected(base, ctx.r3.u32, ctx.r1.u32, calls);
}
void NativeIsWheel(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  ctx.r3.u64 = rc::PadIsWheel(base, ctx.r3.u32, ctx.r1.u32, calls);
}
void NativeReadPad(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rc::ReadPad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                           ctx.r1.u32, calls);
}
void NativePlayerUpdate(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rc::PlayerUpdate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                ctx.r1.u32, calls);
}
void NativeDriveByAiWithPad(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  rc::Regs regs{ctx.r3.u64, 0, false};
  rc::DriveByAiWithPad(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32, calls, &regs);
  ctx.r3.u64 = regs.r3;
}
void NativeAllByAi(PPCContext& ctx, uint8_t* base) {
  ITC_HARNESS_CALLS;
  ctx.r3.u64 = rc::AllByAi(base, ctx.r3.u32, ctx.r1.u32, calls);
}
void NativeNext(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rc::RecordingNext(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeSetup(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rc::Setup(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

std::vector<uint8_t> Bytes(uint8_t* base, uint32_t address, uint32_t size) {
  return std::vector<uint8_t>(base + address, base + address + size);
}

// The globals a case may point elsewhere, saved and put back.
struct Globals {
  uint8_t* base;
  uint32_t race_data, recordings, race, drivers, tables, inputs[8];
  explicit Globals(uint8_t* b) : base(b) {
    race_data = guest::LoadU32(base, rc::kRaceData);
    recordings = guest::LoadU32(base, rc::kRecordings);
    race = guest::LoadU32(base, rc::kRace);
    drivers = guest::LoadU32(base, ai_drivers::kDrivers);
    tables = guest::LoadU32(base, data_tables::kTables);
    for (uint32_t i = 0; i < 8; i++) {
      inputs[i] = guest::LoadU32(base, rc::kInputs + 4 * i);
    }
  }
  void Restore() const {
    guest::StoreU32(base, rc::kRaceData, race_data);
    guest::StoreU32(base, rc::kRecordings, recordings);
    guest::StoreU32(base, rc::kRace, race);
    guest::StoreU32(base, ai_drivers::kDrivers, drivers);
    guest::StoreU32(base, data_tables::kTables, tables);
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU32(base, rc::kInputs + 4 * i, inputs[i]);
    }
  }
};

void RandomWords(uint8_t* base, Random& r, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// --- The input recording ---

void Recording(Context& h, Random& r) {
  const Globals saved(h.base);
  const uint32_t recordings = memory::Alloc(2 * rc::kRecordingBytes, 0);
  const uint32_t record = h.data;
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "InputRecording::Append (sub_821029A8)"
                               : "InputRecording::HasRoom (sub_82102968)");
    for (int c = 0; c < 400; c++) {
      // Counts at, below and above the limit (now and then any word), a
      // player 0 or 1, a record of random words.
      uint32_t counts[2];
      for (uint32_t& count : counts) {
        switch (r.Below(4)) {
          case 0:
            count = r.Below(rc::kRecordingMax);
            break;
          case 1:
            count = rc::kRecordingMax - 3 + r.Below(6);
            break;
          case 2:
            count = r.Below(4);
            break;
          default:
            count = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(rc::kRecordingMax + 10);
        }
      }
      const uint32_t player = r.Below(2);
      uint32_t words[5];
      for (uint32_t& word : words) {
        word = uint32_t(r.Next());
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t p = 0; p < 2; p++) {
          const uint32_t at = recordings + p * rc::kRecordingBytes;
          guest::StoreU32(h.base, at + rc::kRecordingCount, counts[p]);
          if (counts[p] < rc::kRecordingMax) {
            Fill(h.base, at + 20 * counts[p], 20);
          }
        }
        for (uint32_t i = 0; i < 5; i++) {
          guest::StoreU32(h.base, record + 4 * i, words[i]);
        }
        guest::StoreU32(h.base, rc::kRecordings, recordings);
        PPCContext t = CallContext(h);
        t.r3.u64 = which == 0 ? record : player;
        t.r4.u64 = player;
        if (which == 0) {
          (v == 0 ? Original<__imp__sub_821029A8> : NativeAppend)(t, h.base);
        } else {
          (v == 0 ? Original<__imp__sub_82102968> : NativeHasRoom)(t, h.base);
        }
        saved.Restore();
        for (uint32_t p = 0; p < 2; p++) {
          const uint32_t at = recordings + p * rc::kRecordingBytes;
          out[v].Word(guest::LoadU32(h.base, at + rc::kRecordingCount));
          if (counts[p] < rc::kRecordingMax) {
            out[v].Bytes(Bytes(h.base, at + 20 * counts[p], 20));
          }
        }
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  memory::Free(recordings);
}

// --- The cars' input records ---

// The race data (its stage list, the current stage's word +0, the mode)
// for the stage checks: the current stage (+804, 0 to 9) of type 0 to 4 or
// any word, the mode 6, 1, 0 or any word.
void RandomStage(uint8_t* base, Random& r, uint32_t race) {
  const uint32_t stages = race + rc::kStages;
  const uint32_t current = r.Below(10);
  guest::StoreU32(base, stages + 804, current);
  const uint32_t types[] = {0, 1, 2, 3, 4, uint32_t(r.Next())};
  guest::StoreU32(base, stages + 80 * current, types[r.Below(6)]);
  const uint32_t modes[] = {6, 6, 1, 0, uint32_t(r.Next())};
  guest::StoreU32(base, race + rc::kMode, modes[r.Below(5)]);
}

void Inputs(Context& h, Random& r) {
  const Globals saved(h.base);
  const uint32_t race = memory::Alloc(0xA3B00, 0);
  const uint32_t flow = h.data;
  // The race flow: only its +9608 is read and written (in the scratch area).
  const uint32_t kFlow = flow - 9600;
  {
    // RaceFlow::OnSetUp: each version makes its own records, compared by
    // their cleared bytes (+0 to +17; +18 and +19 aren't written) and freed.
    Results results("RaceFlow::OnSetUp (sub_8215F728)");
    for (int c = 0; c < 300; c++) {
      RandomStage(h.base, r, race);
      const uint32_t flags = r.Below(2) ? uint32_t(r.Next()) : r.Below(4);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, flow, 0x40);
        guest::StoreU32(h.base, kFlow + 9608, flags);
        guest::StoreU32(h.base, rc::kRaceData, race);
        PPCContext t = CallContext(h);
        t.r3.u64 = kFlow;
        (v == 0 ? Original<__imp__sub_8215F728> : NativeOnSetUp)(t, h.base);
        uint32_t made[7];
        for (uint32_t i = 0; i < 7; i++) {
          made[i] = guest::LoadU32(h.base, rc::kInputs + 4 * i);
        }
        out[v].Word(guest::LoadU32(h.base, rc::kInputs + 28) == saved.inputs[7] ? 1 : 0);
        saved.Restore();
        for (uint32_t i = 0; i < 7; i++) {
          out[v].Word(made[i] != 0 && memory::IsHeapBlock(made[i]) ? 1 : 0);
          out[v].Bytes(Bytes(h.base, made[i], 18));
        }
        out[v].Word(guest::LoadU32(h.base, kFlow + 9608));
        out[v].Word(t.r3.u32 == made[6] ? 0xA1000006u : t.r3.u32);
        for (uint32_t block : made) {
          memory::crt::OperatorDelete(block);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // RaceFlow::OnEnd: seven records from the layer (now and then a null
    // one), freed by each version: whether each is still a block, the
    // pointers (as indexes) and r3 compared.
    Results results("RaceFlow::OnEnd (sub_8215F5E8)");
    for (int c = 0; c < 100; c++) {
      bool null[7];
      for (bool& n : null) {
        n = r.Below(8) == 0;
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        uint32_t made[7];
        for (uint32_t i = 0; i < 7; i++) {
          made[i] = null[i] ? 0 : *memory::crt::OperatorNew(h.base, 20);
          guest::StoreU32(h.base, rc::kInputs + 4 * i, made[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = flow;
        (v == 0 ? Original<__imp__sub_8215F5E8> : NativeOnEnd)(t, h.base);
        for (uint32_t i = 0; i < 7; i++) {
          out[v].Word(guest::LoadU32(h.base, rc::kInputs + 4 * i) == made[i] ? 1 : 0);
          out[v].Word(made[i] != 0 && memory::IsHeapBlock(made[i]) ? 1 : 0);
        }
        saved.Restore();
        out[v].Word(t.r3.u32 == made[6] ? 0xA1000006u : t.r3.u32);
        for (uint32_t block : made) {
          if (block != 0 && memory::IsHeapBlock(block)) {
            memory::crt::OperatorDelete(block);
          }
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  memory::Free(race);
}

// --- The AI drivers' hand-off ---

// The racers ([0x826D82F4] at `race`: +2308 the records' pointers, +2364
// their count) and their input records (seven of 20 bytes at `records`,
// [0x826C5B30] pointing at them, the eighth word kept).
struct Racers {
  uint32_t race, racers, records;
};

// Up to seven racers (now and then none, or a count above seven: the
// records' list runs into the race's words after it) with indexes 0 to 6
// (now and then with high bits, which the records' lookup masks), the input
// records random, the race data's flags with or without 0x100.
void RandomRacers(uint8_t* base, Random& r, const Racers& at, uint32_t race_data) {
  RandomWords(base, r, at.records, 7 * 20);
  const uint32_t count = r.Below(10) == 0 ? 0 : 1 + r.Below(7);
  guest::StoreU32(base, at.race + 2364, r.Below(32) == 0 ? 0xFFFFFFFFu : count);
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t racer = at.racers + 32 * i;
    guest::StoreU32(base, at.race + 2308 + 4 * i, racer);
    RandomWords(base, r, racer, 32);
    const uint32_t index = r.Below(7);
    guest::StoreU32(base, racer + 20, r.Below(16) == 0 ? index | (r.Below(4) << 8) : index);
  }
  const uint32_t flags = uint32_t(r.Next());
  guest::StoreU32(base, race_data + rc::kFlags, r.Below(4) == 0 ? flags | 0x100 : flags & ~0x100u);
}

void Handoff(Context& h, Random& r) {
  const Globals saved(h.base);
  const uint32_t race_data = memory::Alloc(0xA3B00, 0);
  const uint32_t block = memory::Alloc(0x1000, 0);
  const Racers at{block, block + 0xA00, block + 0xE00};
  const StandIns stand_ins;
  enum Kind { kDriveByAi, kHoldAll, kOnHold, kOnState16, kClearAllRecords };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
    int cases;
  } functions[] = {
      {"RacerControl::DriveByAi (sub_8215F968)", Original<__imp__sub_8215F968>, NativeDriveByAi,
       kDriveByAi, 300},
      {"RaceFlow::HoldAll (sub_8215FA10)", Original<__imp__sub_8215FA10>, NativeHoldAll, kHoldAll,
       300},
      {"RaceFlow::OnHold (sub_8215FC28)", Original<__imp__sub_8215FC28>, NativeHoldAll, kOnHold,
       100},
      {"RaceFlow::OnState16 (sub_8215FB90)", Original<__imp__sub_8215FB90>, NativeOnState16,
       kOnState16, 300},
      {"sub_8215F5F0 (never seen in play)", Original<__imp__sub_8215F5F0>, NativeClearAllRecords,
       kClearAllRecords, 100},
  };
  for (const auto& f : functions) {
    Results results(f.name);
    for (int c = 0; c < f.cases; c++) {
      RandomRacers(h.base, r, at, race_data);
      if (f.kind == kOnState16) {
        RandomStage(h.base, r, race_data);
      }
      const std::vector<uint8_t> records = Bytes(h.base, at.records, 7 * 20);
      const uint32_t index = r.Below(8) == 0 ? r.Below(7) | 0x300 : r.Below(7);
      const uint32_t hold = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, at.records, records.data(), 7 * 20);
        // The stack below the scratch area (the frames), filled the same.
        Fill(h.base, h.data - 1024 - 0x400, 0x400, 0xA5);
        guest::StoreU32(h.base, rc::kRaceData, race_data);
        guest::StoreU32(h.base, rc::kRace, at.race);
        for (uint32_t i = 0; i < 7; i++) {
          guest::StoreU32(h.base, rc::kInputs + 4 * i, at.records + 20 * i);
        }
        StandIns::Reset();
        PPCContext t = CallContext(h);
        t.r3.u64 = index;
        t.r4.u64 = hold;
        t.f1.u64 = 0x7FF8DEADBEEF0000ull;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        saved.Restore();
        out[v].Bytes(Bytes(h.base, at.records, 7 * 20));
        for (uint32_t word : g_stand_in_log) {
          out[v].Word(word);
        }
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  memory::Free(block);
  memory::Free(race_data);
}

// --- The AI drivers' set-up ---

// AiDriver::Setup on a table of seven drivers (random words, then what the
// resets read: each driver's index +0, a vehicle +160 with its box shape
// +344 and flags +6148, the path's driver +512 and modes +5520 to +5528),
// an item from made-up data tables ([0x826C5CA0]: +16 the items, 3,008
// bytes each, +120 the skill's seven halfwords) and the race data's mode
// (+0xA3AF0, the skill's load). The table, the vehicle, the shape and r3
// are compared.
void SetUp(Context& h, Random& r) {
  const Globals saved(h.base);
  constexpr uint32_t kTableBytes = 7 * ai_drivers::kDriverBytes;
  const uint32_t table = memory::Alloc(kTableBytes, 0);
  const uint32_t vehicle = memory::Alloc(6272 + 0x100, 0);
  const uint32_t shape = vehicle + 6272;
  const uint32_t race_data = memory::Alloc(0xA3B00, 0);
  const uint32_t tables = memory::Alloc(0x40 + 4 * 3008, 0);
  const uint32_t items = tables + 0x40;
  const uint32_t args = h.data;  // the index, the item id and the hold flag
  Results results("AiDriver::Setup (sub_82170FF8)");
  for (int c = 0; c < 300; c++) {
    RandomWords(h.base, r, table, kTableBytes);
    RandomWords(h.base, r, vehicle, 6272 + 0x100);
    for (uint32_t i = 0; i < 7; i++) {
      const uint32_t driver = table + ai_drivers::kDriverBytes * i;
      guest::StoreU32(h.base, driver + 0, i);
      guest::StoreU32(h.base, driver + 160, vehicle);
      guest::StoreU32(h.base, driver + 16, r.Below(2) ? uint32_t(r.Next()) : r.Below(8));
      guest::StoreU32(h.base, driver + 512, r.Below(4) == 0 ? 0 : driver);
      guest::StoreU32(h.base, driver + 512 + 5520, r.Below(3));
      guest::StoreU32(h.base, driver + 512 + 5524, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
      guest::StoreU32(h.base, driver + 512 + 5528, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
    }
    guest::StoreU32(h.base, vehicle + 344, r.Below(4) == 0 ? 0 : shape);
    const uint32_t vflags[] = {0, 1, 0x80, 0x81, uint32_t(r.Next())};
    guest::StoreU32(h.base, vehicle + 6148, vflags[r.Below(5)]);
    guest::StoreU32(h.base, tables + 16, items);
    for (uint32_t k = 0; k < 4; k++) {
      for (uint32_t i = 0; i < 7; i++) {
        guest::StoreU16(h.base, items + 3008 * k + 120 + 2 * i,
                        uint16_t(int32_t(r.Below(300)) - 100));
      }
    }
    guest::StoreU32(h.base, race_data + rc::kMode, r.Below(3));
    const std::vector<uint8_t> before = Bytes(h.base, table, kTableBytes);
    const std::vector<uint8_t> vehicle_before = Bytes(h.base, vehicle, 6272 + 0x100);
    const uint32_t index = r.Below(7);
    const uint32_t id = r.Below(4) == 0 ? 0xFFFFFFFFu - r.Below(3) : r.Below(4);
    const uint32_t hold = r.Below(2) ? r.Below(2) : uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, table, before.data(), kTableBytes);
      Write(h.base, vehicle, vehicle_before.data(), 6272 + 0x100);
      Fill(h.base, h.data - 1024 - 0x400, 0x400, 0xA5);
      guest::StoreU32(h.base, args + 0, index);
      guest::StoreU32(h.base, args + 4, id);
      guest::StoreU32(h.base, args + 8, hold);
      guest::StoreU32(h.base, ai_drivers::kDrivers, table);
      guest::StoreU32(h.base, data_tables::kTables, tables);
      guest::StoreU32(h.base, rc::kRaceData, race_data);
      PPCContext t = CallContext(h);
      t.fpscr.disableFlushModeUnconditional();
      t.r3.u64 = args + 0;
      t.r4.u64 = args + 4;
      t.r5.u64 = args + 8;
      (v == 0 ? Original<__imp__sub_82170FF8> : NativeSetup)(t, h.base);
      saved.Restore();
      for (uint32_t i = 0; i < kTableBytes; i += 256) {
        out[v].Bytes(Bytes(h.base, table + i, std::min<uint32_t>(256, kTableBytes - i)));
      }
      out[v].Bytes(Bytes(h.base, vehicle, 6272 + 0x100));
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  memory::Free(tables);
  memory::Free(race_data);
  memory::Free(vehicle);
  memory::Free(table);
}

// --- The player's controls ---

// Four mappers of our own (2,896 bytes each, the game's vtable 0x82007F78,
// their held, pressed and fired words and analog values random, the player
// +2888 0 to 3) in the mappers' table (its entries' word +4 1 now and then:
// not read), each player's entry index random; the table, the indexes and
// the input manager's real pads (Pad::IsConnected and IsWheel ask them).
// The table and the indexes are put back after each version.
constexpr uint32_t kMapperBytes = 2896;
constexpr uint32_t kTableBytes = rc::kPlayerEntries + 16 - rc::kMapperEntries;

void RandomMappers(uint8_t* base, Random& r, uint32_t mappers) {
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t mapper = mappers + kMapperBytes * j;
    RandomWords(base, r, mapper, kMapperBytes);
    guest::StoreU32(base, mapper + 0, 0x82007F78);
    guest::StoreU32(base, mapper + 2888, r.Below(4));
    for (uint32_t i = 0; i < 30; i++) {
      // Analog values: now and then 0 (the axes' second action).
      if (r.Below(3) == 0) {
        guest::StoreU16(base, mapper + 1072 + 2 * i, 0);
      }
    }
    for (uint32_t w = 448; w < 472; w += 4) {
      if (r.Below(3) == 0) {
        guest::StoreU32(base, mapper + w, 0);
      }
    }
    guest::StoreU32(base, rc::kMapperEntries + 8 * j, mapper);
    guest::StoreU32(base, rc::kMapperEntries + 8 * j + 4, r.Below(4) == 0 ? 1 : r.Below(2) * 2);
    guest::StoreU32(base, rc::kPlayerEntries + 4 * j, r.Below(4));
  }
}

void Controls(Context& h, Random& r) {
  const std::vector<uint8_t> table = Bytes(h.base, rc::kMapperEntries, kTableBytes);
  const uint32_t mappers = memory::Alloc(4 * kMapperBytes, 0);
  {
    std::string slots;
    for (uint32_t k = 0; k < 16; k++) {
      slots += fmt::format(" {:08X}", guest::LoadU32(h.base, 0x82007F78 + 4 * k));
    }
    ITC_LOG_INFO("harness: racer_controllers: the mappers' vtable slots:{}", slots);
  }
  enum Kind { kRead, kControl, kIsConnected, kIsWheel };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
    int cases;
  } functions[] = {
      {"PlayerInput::Read (sub_82102350)", Original<__imp__sub_82102350>, NativeRead, kRead, 1000},
      {"PlayerInput::Control (sub_82102488)", Original<__imp__sub_82102488>, NativeControl,
       kControl, 2000},
      {"Pad::IsConnected (sub_8209F340)", Original<__imp__sub_8209F340>, NativeIsConnected,
       kIsConnected, 100},
      {"Pad::IsWheel (sub_8209F3B8)", Original<__imp__sub_8209F3B8>, NativeIsWheel, kIsWheel, 100},
  };
  const uint32_t out = h.data + 0x100;
  for (const auto& f : functions) {
    Results results(f.name);
    for (int c = 0; c < f.cases; c++) {
      RandomMappers(h.base, r, mappers);
      const std::vector<uint8_t> state = Bytes(h.base, mappers, 4 * kMapperBytes);
      const std::vector<uint8_t> entries = Bytes(h.base, rc::kMapperEntries, kTableBytes);
      const uint32_t player = r.Below(4);
      // Ids 151 to 180, now and then outside; kinds 0 to 3 mostly (the
      // ones the game reads), 4 (a float, slot 5) and 5 now and then.
      const uint32_t id = r.Below(8) == 0 ? 140 + r.Below(60) : 151 + r.Below(30);
      const uint32_t kind = r.Below(8) == 0 ? 4 + r.Below(2) : r.Below(4);
      const uint32_t control = r.Below(16) == 0 ? 31 + r.Below(4) : r.Below(31);
      const uint32_t old_out = uint32_t(r.Next());
      const uint32_t entry = rc::kMapperEntries + 8 * r.Below(4);
      g_native_slots = r.Below(4) != 0;
      Outputs out_[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, mappers, state.data(), 4 * kMapperBytes);
        Write(h.base, rc::kMapperEntries, entries.data(), kTableBytes);
        Fill(h.base, h.data - 1024 - 0x400, 0x400, 0xA5);
        guest::StoreU32(h.base, out, old_out);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.f1.u64 = 0x7FF8DEADBEEF0000ull;
        switch (f.kind) {
          case kRead:
            t.r3.u64 = player;
            t.r4.u64 = id;
            t.r5.u64 = kind;
            t.r6.u64 = out;
            break;
          case kControl:
            t.r3.u64 = player;
            t.r4.u64 = control;
            t.r5.u64 = kind;
            t.r6.u64 = out;
            break;
          default:
            t.r3.u64 = entry;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        Write(h.base, rc::kMapperEntries, table.data(), kTableBytes);
        if (kind == 4 && f.kind != kIsConnected && f.kind != kIsWheel) {
          out_[v].Float(guest::LoadU32(h.base, out));
          out_[v].Double(t.f1.u64);
        } else {
          out_[v].Word(guest::LoadU32(h.base, out));
        }
        out_[v].Bytes(Bytes(h.base, mappers, 4 * kMapperBytes) == state ? std::vector<uint8_t>{1}
                                                                        : std::vector<uint8_t>{0});
        out_[v].Word(uint32_t(t.r3.u64 >> 32));
        out_[v].Word(t.r3.u32);
      }
      results.Check(out_[0], out_[1], fmt::format("case {} (control {}, kind {}, id {})", c,
                                                  control, kind, id));
    }
    results.Report();
  }
  g_native_slots = true;
  memory::Free(mappers);
}

// --- The pad reader and the human cars ---

// Everything the pad reader and its callers read, in blocks of our own: the
// race data (its mode, stage, flags and racers' records), the race and its
// racers, the vehicles, the save data (the control maps and settings), the
// recordings, the mappers and the globals 0x826C7960 and 0x826C7964.
constexpr uint32_t kVehiclesGlobal = 0x826C2E34, kSaveDataGlobal = 0x826C5B58;
constexpr uint32_t kPadGlobals = 0x826C7960;
constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kSaveBytes = 0x1F888 + 120 * 4 + 64;

void Pads(Context& h, Random& r) {
  const Globals saved(h.base);
  const uint32_t saved_vehicles = guest::LoadU32(h.base, kVehiclesGlobal);
  const uint32_t saved_save = guest::LoadU32(h.base, kSaveDataGlobal);
  const uint32_t saved_pads[2] = {guest::LoadU32(h.base, kPadGlobals),
                                  guest::LoadU32(h.base, kPadGlobals + 4)};
  const std::vector<uint8_t> table = Bytes(h.base, rc::kMapperEntries, kTableBytes);
  const uint32_t race_data = memory::Alloc(0xA3B00, 0);
  const uint32_t block = memory::Alloc(0x1000, 0);
  const Racers at{block, block + 0xA00, block + 0xE00};
  const uint32_t vehicles = memory::Alloc(7 * kVehicleBytes, 0);
  const uint32_t save = memory::Alloc(kSaveBytes, 0);
  const uint32_t recordings = memory::Alloc(4 * rc::kRecordingBytes, 0);
  const uint32_t mappers = memory::Alloc(4 * kMapperBytes, 0);
  const uint32_t record = h.data + 0x100;
  const uint32_t owner = h.data + 0x200;  // the ring at +204 to +812
  const uint32_t flow = h.data + 0x600 - 9600;  // +9612 and +9616 in the scratch area
  const StandIns stand_ins;
  enum Kind { kReadPad, kUpdate, kWithPad, kAllByAi, kNext };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
    int cases;
  } functions[] = {
      {"PlayerControl::ReadPad (sub_8215FCA8)", Original<__imp__sub_8215FCA8>, NativeReadPad,
       kReadPad, 2000},
      {"PlayerControl::Update (sub_821604C8)", Original<__imp__sub_821604C8>, NativePlayerUpdate,
       kUpdate, 2000},
      {"RacerControl::DriveByAiWithPad (sub_8215F7E8)", Original<__imp__sub_8215F7E8>,
       NativeDriveByAiWithPad, kWithPad, 1000},
      {"RaceFlow::AllByAi (sub_8215FA68)", Original<__imp__sub_8215FA68>, NativeAllByAi, kAllByAi,
       500},
      {"InputRecording::Next (sub_82102A28, never seen in play)", Original<__imp__sub_82102A28>,
       NativeNext, kNext, 300},
  };
  for (const auto& f : functions) {
    Results results(f.name);
    for (int c = 0; c < f.cases; c++) {
      // The racers and their records; the race data's flags (0x100 now and
      // then, 0x20, 0x40, 0x80 in any mix), mode (1 often: the counting;
      // 6 with a stage not 1 often: the pad read without the connection
      // check, whose pads the input manager's real ones are), stage and
      // racers' records (+3936 flags with 0x400 now and then, +3940).
      RandomRacers(h.base, r, at, race_data);
      RandomStage(h.base, r, race_data);
      if (r.Below(2) == 0) {
        guest::StoreU32(h.base, race_data + rc::kMode, r.Below(2) ? 6 : 1);
        const uint32_t stages = race_data + rc::kStages;
        guest::StoreU32(h.base, stages + 80 * guest::LoadU32(h.base, stages + 804),
                        r.Below(4) == 0 ? 1 : 2 + r.Below(3));
      }
      uint32_t flags = guest::LoadU32(h.base, race_data + rc::kFlags) & ~0x1E0u;
      flags |= (r.Below(3) == 0 ? 0x20u : 0u) | (r.Below(3) == 0 ? 0x40u : 0u) |
               (r.Below(4) == 0 ? 0x80u : 0u) | (r.Below(4) == 0 ? 0x100u : 0u);
      guest::StoreU32(h.base, race_data + rc::kFlags, flags);
      guest::StoreU32(h.base, race_data + rc::kCount, uint32_t(r.Next()));
      for (uint32_t i = 0; i < 7; i++) {
        const uint32_t racer = race_data + 3936 * i;
        guest::StoreU32(h.base, racer + 3936,
                        r.Below(6) == 0 ? 0x400u | uint32_t(r.Next()) : uint32_t(r.Next()) & ~0x400u);
        guest::StoreU32(h.base, racer + 3940, r.Below(2) ? 0 : uint32_t(r.Next()));
      }
      // The vehicles: +2249, +2252 (-1 half the time), the speed +48 (m/s:
      // around the brake's threshold), the flags +6148.
      RandomWords(h.base, r, vehicles, 7 * kVehicleBytes);
      for (uint32_t i = 0; i < 7; i++) {
        const uint32_t vehicle = vehicles + kVehicleBytes * i;
        guest::StoreU8(h.base, vehicle + 2249, uint8_t(r.Below(2)));
        guest::StoreU32(h.base, vehicle + 2252, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
        const float speed = float(r.Below(400)) / 10.0f;
        guest::StoreU32(h.base, vehicle + 48,
                        r.Below(16) == 0 ? 0x7FC00000u : std::bit_cast<uint32_t>(speed));
        const uint32_t vflags[] = {0, 1, 0x80, 0x81, uint32_t(r.Next())};
        guest::StoreU32(h.base, vehicle + 6148, vflags[r.Below(5)]);
      }
      // The save data: the control maps (controls 0 to 30, now and then
      // above), the settings bytes.
      RandomWords(h.base, r, save + 0x1F880, kSaveBytes - 0x1F880);
      for (uint32_t i = 0; i < (120 * 4 + 64) / 4; i++) {
        guest::StoreU32(h.base, save + 0x1F888 + 4 * i, r.Below(16) == 0 ? 31 + r.Below(4)
                                                                          : r.Below(31));
      }
      // The recordings' counts (positions).
      for (uint32_t p = 0; p < 4; p++) {
        const uint32_t count = r.Below(4) == 0 ? rc::kRecordingMax - 1 + r.Below(3)
                                               : r.Below(rc::kRecordingMax);
        guest::StoreU32(h.base, recordings + p * rc::kRecordingBytes + rc::kRecordingCount, count);
        RandomWords(h.base, r, recordings + p * rc::kRecordingBytes + 20 * std::min(count, 53999u),
                    20);
      }
      RandomMappers(h.base, r, mappers);
      RandomWords(h.base, r, record, 20);
      RandomWords(h.base, r, owner + 204, 608);
      guest::StoreU32(h.base, owner + 804, r.Below(60));
      guest::StoreU32(h.base, owner + 808, r.Below(2) ? r.Below(50) : uint32_t(r.Next()));
      guest::StoreU32(h.base, flow + 9612, r.Below(4));
      guest::StoreU32(h.base, flow + 9616, r.Below(4));
      const uint32_t pads[2] = {r.Below(8) == 0 ? 1u : 0u, r.Below(8) == 0 ? 1u : 0u};
      const std::vector<uint8_t> state[] = {
          Bytes(h.base, race_data + 0x59000, 0xA3B00 - 0x59000),
          Bytes(h.base, race_data, 7 * 3936 + 3936),
          Bytes(h.base, block, 0x1000),
          Bytes(h.base, vehicles, 7 * kVehicleBytes),
          Bytes(h.base, save + 0x1F880, kSaveBytes - 0x1F880),
          Bytes(h.base, mappers, 4 * kMapperBytes),
          Bytes(h.base, rc::kMapperEntries, kTableBytes),
          Bytes(h.base, h.data, kScratchBytes),
      };
      const uint32_t addresses[] = {race_data + 0x59000, race_data, block, vehicles,
                                    save + 0x1F880, mappers, rc::kMapperEntries, h.data};
      // Each recording's count and the two records at it (the one Next
      // reads or Append writes, and the one after).
      std::vector<uint8_t> recorded[4];
      uint32_t counts[4];
      for (uint32_t p = 0; p < 4; p++) {
        counts[p] =
            guest::LoadU32(h.base, recordings + p * rc::kRecordingBytes + rc::kRecordingCount);
        recorded[p] = Bytes(
            h.base, recordings + p * rc::kRecordingBytes + 20 * std::min(counts[p], 53998u), 40);
      }
      const uint32_t slot = r.Below(7);
      const uint32_t index = r.Below(7);
      const uint32_t player = r.Below(4);
      const uint32_t may_pause = r.Below(4) == 0 ? 0 : 1;
      const uint32_t hold = r.Below(2);
      g_native_slots = r.Below(4) != 0;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (size_t k = 0; k < std::size(addresses); k++) {
          Write(h.base, addresses[k], state[k].data(), uint32_t(state[k].size()));
        }
        for (uint32_t p = 0; p < 4; p++) {
          const uint32_t at_p = recordings + p * rc::kRecordingBytes;
          guest::StoreU32(h.base, at_p + rc::kRecordingCount, counts[p]);
          Write(h.base, at_p + 20 * std::min(counts[p], 53998u), recorded[p].data(), 40);
        }
        Fill(h.base, h.data - 1024 - 0x800, 0x800, 0xA5);
        guest::StoreU32(h.base, rc::kRaceData, race_data);
        guest::StoreU32(h.base, rc::kRace, at.race);
        guest::StoreU32(h.base, rc::kRecordings, recordings);
        guest::StoreU32(h.base, kVehiclesGlobal, vehicles);
        guest::StoreU32(h.base, kSaveDataGlobal, save);
        guest::StoreU32(h.base, ai_drivers::kDrivers, 0x40000000);
        guest::StoreU32(h.base, kPadGlobals, pads[0]);
        guest::StoreU32(h.base, kPadGlobals + 4, pads[1]);
        for (uint32_t i = 0; i < 7; i++) {
          guest::StoreU32(h.base, rc::kInputs + 4 * i, at.records + 20 * i);
        }
        StandIns::Reset();
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.f1.u64 = 0x7FF8DEADBEEF0000ull;
        switch (f.kind) {
          case kReadPad:
            t.r3.u64 = record;
            t.r4.u64 = index;
            t.r5.u64 = player;
            t.r6.u64 = owner;
            t.r7.u64 = may_pause;
            break;
          case kUpdate:
            t.r3.u64 = slot;
            t.r4.u64 = index;
            t.r5.u64 = player;
            t.r6.u64 = owner;
            t.r7.u64 = may_pause;
            break;
          case kWithPad:
            t.r3.u64 = 0x12345678;
            t.r4.u64 = index;
            t.r5.u64 = hold;
            t.r6.u64 = player;
            break;
          case kAllByAi:
            t.r3.u64 = flow;
            t.r4.u64 = 0x12345678;
            break;
          case kNext:
            t.r3.u64 = record;
            t.r4.u64 = player;
            break;
        }
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        saved.Restore();
        guest::StoreU32(h.base, kVehiclesGlobal, saved_vehicles);
        guest::StoreU32(h.base, kSaveDataGlobal, saved_save);
        guest::StoreU32(h.base, kPadGlobals, saved_pads[0]);
        guest::StoreU32(h.base, kPadGlobals + 4, saved_pads[1]);
        const std::vector<uint8_t> mapper_table = Bytes(h.base, rc::kMapperEntries, kTableBytes);
        Write(h.base, rc::kMapperEntries, table.data(), kTableBytes);
        // Everything the case set up, in pieces (what changed shows where).
        for (size_t k = 0; k < std::size(addresses); k++) {
          const std::vector<uint8_t> now =
              k == 6 ? mapper_table : Bytes(h.base, addresses[k], uint32_t(state[k].size()));
          for (size_t i = 0; i < now.size(); i += 4096) {
            out[v].Bytes(std::vector<uint8_t>(now.begin() + i,
                                              now.begin() + std::min(now.size(), i + 4096)));
          }
        }
        for (uint32_t p = 0; p < 4; p++) {
          const uint32_t at_p = recordings + p * rc::kRecordingBytes;
          out[v].Word(guest::LoadU32(h.base, at_p + rc::kRecordingCount));
          out[v].Bytes(Bytes(h.base, at_p + 20 * std::min(counts[p], 53998u), 40));
        }
        for (uint32_t word : g_stand_in_log) {
          out[v].Word(word);
        }
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  g_native_slots = true;
  memory::Free(mappers);
  memory::Free(recordings);
  memory::Free(save);
  memory::Free(vehicles);
  memory::Free(block);
  memory::Free(race_data);
}

}  // namespace

void RacerControllers(Context& h) {
  Random r;
  Recording(h, r);
  if (hooks::NativeHeapActive()) {
    Inputs(h, r);
  }
  Handoff(h, r);
  SetUp(h, r);
  Controls(h, r);
  Pads(h, r);
}

}  // namespace harness
