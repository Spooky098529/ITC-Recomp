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

#include "base/flags.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "racer_controllers/racer_controllers.h"

#include <fmt/format.h>

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

std::vector<uint8_t> Bytes(uint8_t* base, uint32_t address, uint32_t size) {
  return std::vector<uint8_t>(base + address, base + address + size);
}

// The globals a case may point elsewhere, saved and put back.
struct Globals {
  uint8_t* base;
  uint32_t race_data, recordings, inputs[8];
  explicit Globals(uint8_t* b) : base(b) {
    race_data = guest::LoadU32(base, rc::kRaceData);
    recordings = guest::LoadU32(base, rc::kRecordings);
    for (uint32_t i = 0; i < 8; i++) {
      inputs[i] = guest::LoadU32(base, rc::kInputs + 4 * i);
    }
  }
  void Restore() const {
    guest::StoreU32(base, rc::kRaceData, race_data);
    guest::StoreU32(base, rc::kRecordings, recordings);
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

}  // namespace

void RacerControllers(Context& h) {
  Random r;
  Recording(h, r);
  if (hooks::NativeHeapActive()) {
    Inputs(h, r);
  }
}

}  // namespace harness
