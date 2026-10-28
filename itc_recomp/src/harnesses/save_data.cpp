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

#include <iterator>
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

// A block of the save data's size from operator new's plain C++, filled from
// `random` and freed again: the allocation layer's first fit hands the same
// address to the next allocation of that size (SaveData::Create's), so each
// version's unwritten bytes start the same. The address, or 0.
uint32_t Prefill(Context& h, Random& random) {
  const auto block = memory::crt::OperatorNew(h.base, save_data::kSaveDataBytes);
  if (!block || *block == 0) {
    return 0;
  }
  WriteRandom(h.base, *block, save_data::kSaveDataBytes / 4, random);
  memory::crt::OperatorDelete(*block);
  return *block;
}

// SaveData::Create on a slot in the scratch area (as Delete's cases). The
// new block is compared by its bytes, whether it's the prefilled one, and r3
// against it; then freed.
void Create(Context& h) {
  Results results("SaveData::Create (sub_821057C8)");
  int reused = 0;
  for (int c = 0; c < 6; c++) {
    const uint32_t slot = h.data + 4 * h.random.Below(kScratchBytes / 4);
    const uint32_t player = (slot - save_data::kSlots) >> 2;
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, h.data, kScratchBytes / 4, random);
      const uint32_t prefilled = Prefill(h, random);
      after = random;
      PPCContext t = CallContext(h);
      t.r3.u64 = player;
      ITC_FLAG(native_save_data) = v == 1;
      (v == 0 ? __imp__sub_821057C8 : hooks::SaveDataCreate)(t, h.base);
      ITC_FLAG(native_save_data) = true;
      const uint32_t block = guest::LoadU32(h.base, slot);
      out[v].Word(block == prefilled && block != 0 ? 1 : 0);
      reused += block == prefilled && block != 0;
      out[v].Bytes(Read(h.base, block, save_data::kSaveDataBytes));
      guest::StoreU32(h.base, slot, 0);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32 - block);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      memory::crt::OperatorDelete(block);
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}: slot +{:X}", c, slot - h.data));
  }
  ITC_LOG_INFO("harness: SaveData::Create: {} of 12 runs got the prefilled block back", reused);
  results.Report();
}

// The list's flags block (+68): `old` pages of `old_count` words, or 0.
uint32_t MakeFlags(uint32_t old_count) {
  return old_count == 0 ? 0 : memory::AllocPages(old_count * 4, 4096);
}

// Records a menu list at `list` after MenuList::Init: its words with +68
// left out, the new block's words (count of them), whether the old block was
// freed and r3 against the new block; the new block freed.
void RecordList(Outputs& out, Context& h, uint32_t list, uint32_t old, const PPCContext& t) {
  const uint32_t flags = guest::LoadU32(h.base, list + 68);
  const int32_t count = int32_t(guest::LoadU32(h.base, list + 0));
  out.Word(flags != 0 && memory::IsPages(flags) ? 1 : 0);
  if (flags != 0 && count > 0) {
    out.Bytes(Read(h.base, flags, uint32_t(count) * 4));
  }
  out.Word(old == 0 || !memory::IsPages(old) ? 1 : 0);
  out.Word(t.r3.u32 == flags ? 1 : 0);
  out.Word(uint32_t(t.r3.u64 >> 32));
  if (old != 0 && memory::IsPages(old)) {
    memory::FreePages(old);
  }
  if (flags != 0 && memory::IsPages(flags)) {
    memory::FreePages(flags);
  }
  guest::StoreU32(h.base, list + 68, 0);
}

void MenuListInit(Context& h) {
  Results results("MenuList::Init (sub_8220BAC0)");
  for (int c = 0; c < 40; c++) {
    const uint32_t list = h.data + 4 * h.random.Below((kScratchBytes - 72) / 4);
    const uint32_t old_count = c % 3 == 0 ? 0 : 1 + h.random.Below(2000);
    const uint32_t count = c % 10 == 1 ? 0 : h.random.Below(c % 5 == 2 ? 3000 : 40);
    const uint32_t shown = h.random.Below(c % 7 == 3 ? 0xFFFFFFFF : 40);
    uint32_t ids[7];
    for (uint32_t& id : ids) {
      id = uint32_t(h.random.Next());
    }
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, h.data, kScratchBytes / 4, random);
      after = random;
      const uint32_t old = MakeFlags(old_count);
      guest::StoreU32(h.base, list + 68, old);
      PPCContext t = CallContext(h);
      t.r3.u64 = list;
      t.r4.u64 = count;
      t.r5.u64 = shown;
      t.r6.u64 = ids[0];
      t.r7.u64 = ids[1];
      t.r8.u64 = ids[2];
      t.r9.u64 = ids[3];
      t.r10.u64 = ids[4];
      guest::StoreU32(h.base, t.r1.u32 + 84, ids[5]);
      guest::StoreU32(h.base, t.r1.u32 + 92, ids[6]);
      ITC_FLAG(native_save_data) = v == 1;
      (v == 0 ? __imp__sub_8220BAC0 : hooks::SaveDataMenuListInit)(t, h.base);
      ITC_FLAG(native_save_data) = true;
      RecordList(out[v], h, list, old, t);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    }
    h.random = after;
    results.Check(out[0], out[1],
                  fmt::format("case {}: count {}, shown {}, old {}", c, count, int32_t(shown),
                              old_count));
  }
  results.Report();
}

// The globals Players::ResetControllers and TitleScreen::ResetPlayer write,
// kept and put back around each version.
struct Globals {
  static constexpr uint32_t kAddresses[] = {
      save_data::kControllers,      save_data::kControllers + 4,  save_data::kControllers + 8,
      save_data::kControllers + 12, save_data::kProfileReadAsked, save_data::kModeChecks,
      save_data::kModeChecks + 4,   save_data::kModeChecks + 8,   save_data::kModeChecks + 12,
      save_data::kTitleMenuList,    save_data::kTitleScreen,      save_data::kSlots};
  uint32_t saved[std::size(kAddresses)];
  uint8_t* base;
  explicit Globals(uint8_t* base) : base(base) {
    for (size_t i = 0; i < std::size(kAddresses); i++) {
      saved[i] = guest::LoadU32(base, kAddresses[i]);
    }
  }
  ~Globals() {
    for (size_t i = 0; i < std::size(kAddresses); i++) {
      guest::StoreU32(base, kAddresses[i], saved[i]);
    }
  }
  // The words written, recorded (the slot and the two pointers left out).
  void Record(Outputs& out) const {
    for (size_t i = 0; i < 9; i++) {
      out.Word(guest::LoadU32(base, kAddresses[i]));
    }
  }
  void Randomize(Random& random) const {
    for (size_t i = 0; i < 9; i++) {
      guest::StoreU32(base, kAddresses[i], uint32_t(random.Next()));
    }
  }
};

void ResetControllers(Context& h) {
  Results results("Players::ResetControllers (sub_8209F5E0)");
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      const Globals globals(h.base);
      Random random = start;
      WriteRandom(h.base, h.data, kScratchBytes / 4, random);
      globals.Randomize(random);
      after = random;
      PPCContext t = CallContext(h);
      t.r3.u64 = random.Next();
      ITC_FLAG(native_save_data) = v == 1;
      (v == 0 ? __imp__sub_8209F5E0 : hooks::SaveDataResetControllers)(t, h.base);
      ITC_FLAG(native_save_data) = true;
      globals.Record(out[v]);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      for (const auto& r : {t.r3, t.r9, t.r10, t.r11}) {
        out[v].Word(r.u32);
        out[v].Word(uint32_t(r.u64 >> 32));
      }
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// TitleScreen::ResetPlayer with the title's list and screen in the scratch
// area and player 0's slot holding a block of the test's, the game's put
// back after each version. The old save data block is a prefilled one, held
// until Delete frees it, so SaveData::Create gets it back (first fit: no
// hole below it is big enough, and the list's blocks are smaller); an empty
// slot isn't tried here (the list's new block could take the hole first),
// SaveData::Delete's and Create's own cases cover it.
void ResetPlayer(Context& h) {
  Results results("TitleScreen::ResetPlayer (sub_82221D00)");
  int reused = 0;
  for (int c = 0; c < 6; c++) {
    const uint32_t list = h.data + 4 * h.random.Below(256);
    const uint32_t screen = h.data + 2048 + 4 * h.random.Below(256);
    const uint32_t old_count = c % 2 == 0 ? 0 : 1 + h.random.Below(100);
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      const Globals globals(h.base);
      Random random = start;
      WriteRandom(h.base, h.data, kScratchBytes / 4, random);
      globals.Randomize(random);
      uint32_t prefilled = 0;
      if (const auto block = memory::crt::OperatorNew(h.base, save_data::kSaveDataBytes)) {
        prefilled = *block;
        WriteRandom(h.base, prefilled, save_data::kSaveDataBytes / 4, random);
      }
      after = random;
      const uint32_t old = MakeFlags(old_count);
      guest::StoreU32(h.base, list + 68, old);
      guest::StoreU32(h.base, save_data::kTitleMenuList, list);
      guest::StoreU32(h.base, save_data::kTitleScreen, screen);
      guest::StoreU32(h.base, save_data::kSlots, prefilled);
      PPCContext t = CallContext(h);
      ITC_FLAG(native_save_data) = v == 1;
      (v == 0 ? __imp__sub_82221D00 : hooks::SaveDataResetPlayer)(t, h.base);
      ITC_FLAG(native_save_data) = true;
      const uint32_t block = guest::LoadU32(h.base, save_data::kSlots);
      globals.Record(out[v]);
      out[v].Word(block == prefilled && block != 0 ? 1 : 0);
      reused += block == prefilled && block != 0;
      out[v].Bytes(Read(h.base, block, save_data::kSaveDataBytes));
      out[v].Word(t.r3.u32 - block);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      memory::crt::OperatorDelete(block);
      RecordList(out[v], h, list, old, t);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    }
    h.random = after;
    results.Check(out[0], out[1],
                  fmt::format("case {}: old list block {}", c, old_count));
  }
  ITC_LOG_INFO("harness: TitleScreen::ResetPlayer: {} of 12 runs got the prefilled block back",
               reused);
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
  Create(h);
  MenuListInit(h);
  ResetControllers(h);
  ResetPlayer(h);
}

}  // namespace harness
