// The players' save data (src/save_data/, docs/functions/save-data.md)
// against the recompiled functions. Each case fills the scratch area and a
// block from the allocation layer (the save data is 0x1F978 bytes, more than
// the scratch area holds) with random words, runs the original (with
// --native_save_data off, so its callees are the originals too) or the plain
// C++ through the register steps (hooks/save_data.h), and compares the
// block, the scratch area and r3 with harness::Outputs. Every value is
// compared exactly: the only floats written are the 0.0 at 0x82000A34,
// copied.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/save_data.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "save_data/save_data.h"

#include <fmt/format.h>

#include <string>

ITC_FLAG_DECLARE(bool, native_save_data);

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x20000;

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t words, Random& random) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, address + 4 * i, uint32_t(random.Next()));
  }
}

// One case through both versions: `args(t)` sets the registers; `r11` also
// compares r11.
template <typename Args>
void Case(Context& h, Results& results, const Block& block, Args args, GuestFunction recompiled,
          GuestFunction native, const std::string& what, bool r11 = false) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random random = start;
    WriteRandom(h.base, h.data, kScratchBytes / 4, random);
    WriteRandom(h.base, block.address, block.bytes / 4, random);
    after = random;
    PPCContext t = CallContext(h);
    args(t);
    ITC_FLAG(native_save_data) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_save_data) = true;
    out[v].Bytes(Read(h.base, block.address, block.bytes));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
    if (r11) {  // the getters' lis leaves it
      out[v].Word(t.r11.u32);
      out[v].Word(uint32_t(t.r11.u64 >> 32));
    }
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// A function of one pointer (r3) at a random word in the block, `bytes`
// from it fitting; the first case at the block's start.
void OnePointer(Context& h, const Block& block, const char* name, GuestFunction recompiled,
                GuestFunction native, uint32_t bytes, int cases) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    const uint32_t at =
        block.address + (c == 0 ? 0 : 4 * h.random.Below((block.bytes - bytes) / 4 + 1));
    Case(
        h, results, block, [&](PPCContext& t) { t.r3.u64 = at; }, recompiled, native,
        fmt::format("case {}: +{:X}", c, at - block.address));
  }
  results.Report();
}

void Getter(Context& h, const Block& block, const char* name, GuestFunction recompiled,
            GuestFunction native) {
  Results results(name);
  for (int c = 0; c < 2; c++) {
    Case(
        h, results, block,
        [&](PPCContext& t) {
          t.r3.u64 = h.random.Next();
          t.r11.u64 = h.random.Next();
        },
        recompiled, native, fmt::format("case {}", c), true);
  }
  results.Report();
}

// SaveData::Delete on a slot in the scratch area (the player number picked so
// that the slot's address lands there, so the game's slots aren't touched),
// holding 0 or a block from operator new's plain C++ (each version its own).
// The block's address is compared as whether r3 is it, and whether it was
// freed.
void Delete(Context& h) {
  Results results("SaveData::Delete (sub_821055D8)");
  for (int c = 0; c < 16; c++) {
    const uint32_t slot = h.data + 4 * h.random.Below(kScratchBytes / 4);
    const uint32_t player = (slot - save_data::kSlots) >> 2;
    const bool empty = c % 4 == 0;
    const uint32_t bytes = 16 + 16 * h.random.Below(64);
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, h.data, kScratchBytes / 4, random);
      after = random;
      uint32_t block = 0;
      if (!empty) {
        const auto made = memory::crt::OperatorNew(h.base, bytes);
        block = made ? *made : 0;
      }
      guest::StoreU32(h.base, slot, block);
      PPCContext t = CallContext(h);
      t.r3.u64 = player;
      ITC_FLAG(native_save_data) = v == 1;
      (v == 0 ? __imp__sub_821055D8 : hooks::SaveDataDelete)(t, h.base);
      ITC_FLAG(native_save_data) = true;
      const bool freed = block == 0 || !memory::IsHeapBlock(block);
      if (!freed) {
        memory::crt::OperatorDelete(block);
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32 == block ? 1 : 0);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(freed ? 1 : 0);
    }
    h.random = after;
    results.Check(out[0], out[1],
                  fmt::format("case {}: slot +{:X}, {}", c, slot - h.data, empty ? "empty" : "a block"));
  }
  results.Report();
}

}  // namespace

void SaveData(Context& h) {
  const Block block(kBlockBytes);
  if (!block.address) {
    ITC_LOG_ERROR("harness: save_data: no block from the allocation layer");
    return;
  }
  OnePointer(h, block, "Stats::Stats (sub_821056C0)", __imp__sub_821056C0,
             hooks::SaveDataStatsConstruct, save_data::kStatsBytes, 12);
  OnePointer(h, block, "Stats::ResetRecords (sub_821136D8)", __imp__sub_821136D8,
             hooks::SaveDataStatsResetRecords, 4 + 15 * 48, 12);
  OnePointer(h, block, "Stats::Reset (sub_82113730)", __imp__sub_82113730,
             hooks::SaveDataStatsReset, save_data::kStatsBytes, 12);
  OnePointer(h, block, "Progress::Clear (sub_82113BD8)", __imp__sub_82113BD8,
             hooks::SaveDataProgressClear, save_data::kProgressBytes, 8);
  OnePointer(h, block, "BitSet400::SetAll (sub_821133A8)", __imp__sub_821133A8,
             hooks::SaveDataBitSetAll, 52, 12);
  Getter(h, block, "Defaults::Table50A8 (sub_820CCCC0)", __imp__sub_820CCCC0,
         hooks::SaveDataTable50A8);
  Getter(h, block, "Defaults::Table3044 (sub_820CCCD0)", __imp__sub_820CCCD0,
         hooks::SaveDataTable3044);
  Getter(h, block, "Defaults::Table3E34 (sub_820CCCE0)", __imp__sub_820CCCE0,
         hooks::SaveDataTable3E34);
  Getter(h, block, "Defaults::Table3C24 (sub_820CCCF0)", __imp__sub_820CCCF0,
         hooks::SaveDataTable3C24);
  Getter(h, block, "Defaults::Table50E4 (sub_820CCD00)", __imp__sub_820CCD00,
         hooks::SaveDataTable50E4);
  OnePointer(h, block, "Progress::SetDefaults (sub_82113E60)", __imp__sub_82113E60,
             hooks::SaveDataProgressSetDefaults, save_data::kProgressBytes, 8);
  OnePointer(h, block, "ControlMap::SetDefault (sub_821058A8)", __imp__sub_821058A8,
             hooks::SaveDataControlMapSetDefault, 60, 12);
  OnePointer(h, block, "Options::SetDefaults (sub_82105940)", __imp__sub_82105940,
             hooks::SaveDataOptionsSetDefaults, 284, 12);
  OnePointer(h, block, "CourseRecords::Reset (sub_82105D08)", __imp__sub_82105D08,
             hooks::SaveDataCourseRecordsReset, 20 + 22 * 408, 12);
  Delete(h);
}

}  // namespace harness
