// The data load at boot (src/data_load/, docs/functions/data-load.md)
// against the recompiled functions. The globals the functions write (the save
// defaults' bit sets, the session objects' slots) are the game's live ones:
// saved first and put back after the cases. Blocks the makers allocate are
// prefilled (as SaveData::Create's cases: the allocation layer's first fit
// hands the freed block back), compared by their bytes and freed. Everything
// is compared exactly (the records' floats are .rdata's 1.0 and 0.0 copied).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "data_load/data_load.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"

#include <fmt/format.h>

#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_data_load);

namespace harness {

namespace {

namespace dl = data_load;

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& random) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(random.Next()));
  }
}

// The register steps of the functions without adapters.
void NativeBits224Fill(PPCContext& t, uint8_t* base) { dl::Bits224Fill(base, t.r3.u32); }
void NativeBits480Fill(PPCContext& t, uint8_t* base) { dl::Bits480Fill(base, t.r3.u32); }
void NativePart464(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::Part464Construct(base, t.r3.u32);
}
void NativeMember4080(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::Member4080Construct(base, t.r3.u32);
}
void NativeMember1136(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::Member1136Construct(base, t.r3.u32);
}
void NativeInputRecordingsConstruct(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::InputRecordingsConstruct(base, t.r3.u32);
}

// A fill (r3 = a set in the scratch area, random words around it).
void FillCase(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
          uint32_t words) {
  Results results(name);
  for (int c = 0; c < 8; c++) {
    const uint32_t set = h.data + 4 * h.random.Below((kScratchBytes - 4 * words) / 4);
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, h.data, kScratchBytes, random);
      if (v == 1) {
        h.random = random;
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = set;
      (v == 0 ? recompiled : native)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("set +{:X}", set - h.data));
  }
  results.Report();
}

void DefaultsInit(Context& h) {
  Results results("Defaults::Init (sub_820CCDC0)");
  const std::vector<uint8_t> saved = Read(h.base, dl::kDefaultsFirst, dl::kDefaultsBytes);
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      if (c == 0) {
        harness::Fill(h.base, dl::kDefaultsFirst, dl::kDefaultsBytes, 0);
      } else {
        WriteRandom(h.base, dl::kDefaultsFirst, dl::kDefaultsBytes, random);
      }
      WriteRandom(h.base, h.data, kScratchBytes, random);
      if (v == 1) {
        h.random = random;
      }
      PPCContext t = CallContext(h);
      ITC_FLAG(native_data_load) = v == 1;
      (v == 0 ? __imp__sub_820CCDC0 : sub_820CCDC0)(t, h.base);
      ITC_FLAG(native_data_load) = true;
      out[v].Bytes(Read(h.base, dl::kDefaultsFirst, dl::kDefaultsBytes));
      // The frame's five indices (the stack below the scratch area).
      out[v].Bytes(Read(h.base, t.r1.u32 - 128 + 80, 20));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], c == 0 ? "cleared" : fmt::format("random {}", c));
  }
  results.Report();
  Write(h.base, dl::kDefaultsFirst, saved.data(), uint32_t(saved.size()));
}

// A constructor on a heap block of `bytes` (random words), at `offset` in it.
void Constructor(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
                 uint32_t bytes) {
  Results results(name);
  const uint32_t room = bytes + 256;
  const auto block = memory::crt::OperatorNew(h.base, room);
  if (!block || *block == 0) {
    ITC_LOG_WARN("harness: data_load: {}: no block", name);
    return;
  }
  for (int c = 0; c < 6; c++) {
    const uint32_t offset = 4 * h.random.Below(64);
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, *block, room, random);
      if (v == 1) {
        h.random = random;
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = *block + offset;
      ITC_FLAG(native_data_load) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_data_load) = true;
      out[v].Bytes(Read(h.base, *block, room));
      out[v].Word(t.r3.u32 - *block);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("at +{:X}", offset));
  }
  results.Report();
  memory::crt::OperatorDelete(*block);
}

// A block of `bytes` from operator new's plain C++, filled from `random` and
// freed again, for the next allocation of that size. The address, or 0.
uint32_t Prefill(Context& h, uint32_t bytes, Random& random) {
  const auto block = memory::crt::OperatorNew(h.base, bytes);
  if (!block || *block == 0) {
    return 0;
  }
  WriteRandom(h.base, *block, bytes, random);
  memory::crt::OperatorDelete(*block);
  return *block;
}

// A maker of a session object: `slot` its global, `bytes` the block's size.
void Maker(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
           uint32_t slot, uint32_t bytes) {
  Results results(name);
  const uint32_t saved = guest::LoadU32(h.base, slot);
  int reused = 0;
  for (int c = 0; c < 3; c++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      const uint32_t prefilled = Prefill(h, bytes, random);
      if (v == 1) {
        h.random = random;
      }
      guest::StoreU32(h.base, slot, 0xA5A5A5A5u);
      PPCContext t = CallContext(h);
      ITC_FLAG(native_data_load) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_data_load) = true;
      const uint32_t made = guest::LoadU32(h.base, slot);
      out[v].Word(made == prefilled && made != 0 ? 1 : 0);
      reused += made == prefilled && made != 0;
      if (made != 0) {
        out[v].Bytes(Read(h.base, made, bytes));
      }
      out[v].Word(t.r3.u32 - made);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      if (made != 0) {
        memory::crt::OperatorDelete(made);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  ITC_LOG_INFO("harness: {}: {} of 6 runs got the prefilled block back", name, reused);
  results.Report();
  guest::StoreU32(h.base, slot, saved);
}

}  // namespace

void DataLoad(Context& h) {
  FillCase(h, "Bits224::Fill (sub_820CCD10)", __imp__sub_820CCD10, NativeBits224Fill, 7);
  FillCase(h, "Bits480::Fill (sub_820CCD68)", __imp__sub_820CCD68, NativeBits480Fill, 15);
  DefaultsInit(h);
  Constructor(h, "Part464::Part464 (sub_821042A0)", __imp__sub_821042A0, NativePart464, 464);
  Constructor(h, "Member4080::Member4080 (sub_82104690)", __imp__sub_82104690, NativeMember4080,
              1152);
  Constructor(h, "Member1136::Member1136 (sub_82102888)", __imp__sub_82102888, NativeMember1136,
              2944);
  Constructor(h, "Record5232::Record5232 (sub_82104C20)", __imp__sub_82104C20, sub_82104C20,
              dl::kRecordBytes);
  Constructor(h, "InputRecordings::InputRecordings (sub_821054A8)", __imp__sub_821054A8,
              NativeInputRecordingsConstruct, dl::kInputRecordingsBytes);
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_WARN("harness: data_load: the makers need --native_heap");
    return;
  }
  Maker(h, "RaceData::Create (sub_82102208)", __imp__sub_82102208, sub_82102208,
        dl::kRaceDataSlot, dl::kRaceDataBytes);
  Maker(h, "InputRecordings::Create (sub_82105508)", __imp__sub_82105508, sub_82105508,
        dl::kInputRecordingsSlot, dl::kInputRecordingsBytes);
  Maker(h, "Replay::Create (sub_82105560)", __imp__sub_82105560, sub_82105560, dl::kReplaySlot,
        dl::kReplayBytes);
}

}  // namespace harness
