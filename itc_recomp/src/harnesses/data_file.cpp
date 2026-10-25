// The game's container files (src/game_data/data_file.cpp,
// docs/functions/files-and-saves.md) against the recompiled functions.
// DataFile::GetEntry: containers in the scratch area (a little-endian count
// and offsets, entries whose first word is 0 or not), indexes inside and
// past the count (and with the top bit set: the compare is unsigned), r5's
// low byte 0 or not (with higher bits set too); r3 compared exactly.

#include "harness.h"

#include "game_data/data_file.h"

#include <fmt/format.h>

namespace harness {

namespace {

void NativeGetEntry(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = game_data::GetEntry(base, ctx.r3.u32, ctx.r4.u32, (ctx.r5.u32 & 0xFF) != 0);
}

void StoreLittle(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, __builtin_bswap32(value));
}

}  // namespace

void DataFile(Context& h) {
  Results results("DataFile::GetEntry (sub_82294730)");
  const uint32_t container = h.data;
  constexpr uint32_t kBytes = 2048;
  const uint32_t r5s[] = {0, 1, 0xFF, 0x100, 0x80000001, 0x12345600};
  for (int c = 0; c < 400; c++) {
    // The count: small, or huge (the entries still inside the area).
    const uint32_t entries = h.random.Below(9);
    const uint32_t count = c % 25 == 7 ? 0xFFFFFFFFu - h.random.Below(3) : entries;
    uint32_t index;
    switch (h.random.Below(4)) {
      case 0: index = count + h.random.Below(3); break;
      case 1:
        // Only past a small count: below a huge one it'd read anywhere.
        index = count <= 9 ? 0x80000000u | uint32_t(h.random.Next()) : 0;
        break;
      default: index = entries == 0 ? 0 : h.random.Below(entries); break;
    }
    const uint32_t r5 = r5s[h.random.Below(6)];
    uint32_t offsets[9];
    for (uint32_t i = 0; i < 9; i++) {
      offsets[i] = 64 + 16 * h.random.Below((kBytes - 128) / 16);
    }
    std::vector<uint32_t> firsts(9);
    for (auto& f : firsts) f = h.random.Below(3) == 0 ? 0 : uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, container, kBytes, 0);
      for (uint32_t i = 0; i < 9; i++) {
        guest::StoreU32(h.base, container + offsets[i], firsts[i]);
      }
      StoreLittle(h.base, container, count);
      for (uint32_t i = 0; i < 9 && i < (count > 9 ? 9 : count); i++) {
        StoreLittle(h.base, container + 4 + 4 * i, offsets[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = container;
      t.r4.u64 = index;
      t.r5.u64 = r5;
      (v == 0 ? __imp__sub_82294730 : NativeGetEntry)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, container, kBytes));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {} (count {:#x}, index {:#x}, r5 {:#x})", c, count, index, r5));
  }
  results.Report();
}

}  // namespace harness
