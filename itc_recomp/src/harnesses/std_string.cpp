// The C++ library's narrow string (src/std_string/, docs/functions/std-string.md)
// against the recompiled functions. Each case fills the scratch area, writes
// strings into it (in place or with a heap block from operator new, random
// text, the capacities and lengths the functions branch on), runs the
// original (__imp__) or the plain C++ (through the register steps below,
// which hand what the plain C++ refuses to the original as the adapters do),
// and compares the whole scratch area, r3, and each heap block involved
// (whether it's still a block, its size and contents), all exactly: the
// string holds no floats. Blocks are compared by contents, not address: a
// block's address in the scratch area or r3 is replaced by its number in
// the case. The functions allocate and free through the C runtime's heap,
// so they run only with --native_heap (the adapters' condition).

#include "harness.h"

#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_string/std_string.h"

#include <fmt/format.h>

#include <iterator>
#include <string>
#include <vector>

namespace harness {

namespace {

namespace ss = std_string;

// Cases the plain C++ handed to the original (none expected: the cases keep
// to the paths it takes).
int g_fallbacks = 0;

// --- The register steps, as the adapters (src/hooks/std_string.cpp). ---

void NativeTidy(PPCContext& ctx, uint8_t* base) {
  const uint32_t string = ctx.r3.u32;
  const bool built = (ctx.r4.u32 & 0xFF) != 0;
  const bool frees = built && guest::LoadU32(base, string + ss::kCapacity) >= ss::kInPlace;
  const uint32_t block = guest::LoadU32(base, string + ss::kText);
  if (!ss::Tidy(base, string, built, ctx.r5.u32)) {
    g_fallbacks++;
    __imp__sub_82091328(ctx, base);
    return;
  }
  if (frees) {
    ctx.r3.u64 = block;
  }
}

void NativeAllocate(PPCContext& ctx, uint8_t* base) {
  if (const auto block = ss::Allocate(base, ctx.r3.u32)) {
    ctx.r3.u64 = *block;
    return;
  }
  g_fallbacks++;
  __imp__sub_820912B0(ctx, base);
}

// --- Writing strings and reading back what a case left. ---

// A string at `string`: `capacity` (16 or more: a heap block of capacity + 1
// bytes from operator new, else the text in place), `length` characters of
// random text (letters, so a 0 byte only ends it) and its 0, the rest of the
// in-place area or block random. Returns the block, or 0.
uint32_t WriteString(Context& h, uint32_t string, uint32_t capacity, uint32_t length) {
  guest::StoreU32(h.base, string, h.random.Below(2) ? 0 : uint32_t(h.random.Next()));
  uint32_t text = string + ss::kText;
  uint32_t block = 0;
  uint32_t room = 16;
  if (capacity >= ss::kInPlace) {
    block = *memory::crt::OperatorNew(h.base, capacity + 1);
    guest::StoreU32(h.base, string + ss::kText, block);
    text = block;
    room = capacity + 1;
  }
  for (uint32_t i = 0; i < room; i++) {
    guest::StoreU8(h.base, text + i, uint8_t(h.random.Next()));
  }
  for (uint32_t i = 0; i < length && i < room; i++) {
    guest::StoreU8(h.base, text + i, uint8_t('a' + h.random.Below(26)));
  }
  if (length < room) {
    guest::StoreU8(h.base, text + length, 0);
  }
  guest::StoreU32(h.base, string + ss::kLength, length);
  guest::StoreU32(h.base, string + ss::kCapacity, capacity);
  return block;
}

// The blocks a case involves: those it started with, and those the strings
// hold afterwards.
struct Blocks {
  std::vector<uint32_t> list;
  void Add(uint32_t block) {
    if (block == 0) {
      return;
    }
    for (uint32_t b : list) {
      if (b == block) {
        return;
      }
    }
    list.push_back(block);
  }
  // A word with blocks replaced by their number.
  uint32_t Normalize(uint32_t word) const {
    for (size_t i = 0; i < list.size(); i++) {
      if (word == list[i]) {
        return 0xB10C0000u + uint32_t(i);
      }
    }
    return word;
  }
};

// After a side ran: the scratch area (aligned words that are blocks
// replaced), r3 (likewise), and each block: whether it's still one, its size
// and contents; then the blocks left are freed. `strings` are the strings
// whose heap blocks (capacity 16 or more) join the list.
void Collect(Context& h, Outputs& out, const PPCContext& t, Blocks blocks,
             const std::vector<uint32_t>& strings) {
  for (uint32_t s : strings) {
    if (guest::LoadU32(h.base, s + ss::kCapacity) >= ss::kInPlace) {
      blocks.Add(guest::LoadU32(h.base, s + ss::kText));
    }
  }
  std::vector<uint8_t> area = Read(h.base, h.data, kScratchBytes);
  for (uint32_t i = 0; i + 4 <= kScratchBytes; i += 4) {
    const uint32_t word = guest::LoadU32(h.base, h.data + i);
    const uint32_t normalized = blocks.Normalize(word);
    for (int k = 0; k < 4; k++) {
      area[i + k] = uint8_t(normalized >> (24 - 8 * k));
    }
  }
  out.Bytes(area);
  out.Word(blocks.Normalize(t.r3.u32));
  for (uint32_t block : blocks.list) {
    const bool alive = memory::IsHeapBlock(block);
    out.Word(alive);
    if (alive) {
      const uint32_t size = memory::Size(block);
      out.Word(size);
      out.Bytes(Read(h.base, block, size));
      memory::crt::OperatorDelete(block);
    }
  }
}

// A capacity: in place (15, as the game's, or another under 16) or a heap
// block's (16 up).
uint32_t PickCapacity(Random& random) {
  switch (random.Below(4)) {
    case 0:
      return 15;
    case 1:
      return random.Below(16);
    case 2:
      return 16 + random.Below(48);
    default:
      return 16 + random.Below(400);
  }
}

// --- _Tidy. ---

void TidyCases(Context& h) {
  Results results("sub_82091328 std::string::_Tidy");
  const uint32_t string = h.data + 64;
  for (int i = 0; i < 2000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const uint32_t capacity = PickCapacity(h.random);
      const uint32_t length = h.random.Below(capacity + 1);
      const uint32_t block = WriteString(h, string, capacity, length);
      // built: 0, 1, or other values with the low byte 0 or not (the
      // original tests the low byte).
      static const uint32_t kBuilt[] = {0, 1, 0x100, 0x2FF, 0xFFFFFF00u};
      const uint32_t built = kBuilt[h.random.Below(5)];
      // The new length: 0 (the destructor and constructors), up to 16 for a
      // heap string being built (memcpy_s's room; more is its error path),
      // else up to 40 (only the 0 byte is written).
      uint32_t new_length = 0;
      if (h.random.Below(3) != 0) {
        const bool copies = (built & 0xFF) != 0 && capacity >= ss::kInPlace;
        new_length = copies ? h.random.Below(17) : h.random.Below(41);
      }
      Blocks blocks;
      blocks.Add(block);
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = built;
      t.r5.u64 = new_length;
      (v == 0 ? __imp__sub_82091328 : NativeTidy)(t, h.base);
      Collect(h, out[v], t, blocks, {});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// --- allocator<char>::allocate. ---

void AllocateCases(Context& h) {
  Results results("sub_820912B0 std::allocator<char>::allocate");
  static const uint32_t kCounts[] = {0, 1, 2, 15, 16, 17, 31, 32, 33, 47, 48, 64, 100, 4096};
  for (int i = 0; i < 300; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const uint32_t count = i < int(std::size(kCounts)) ? kCounts[i] : h.random.Below(5000);
      PPCContext t = CallContext(h);
      t.r3.u64 = count;
      (v == 0 ? __imp__sub_820912B0 : NativeAllocate)(t, h.base);
      Blocks blocks;
      blocks.Add(t.r3.u32);
      Collect(h, out[v], t, blocks, {});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

}  // namespace

void StdString(Context& h) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: std_string: skipped (--native_heap off)");
    return;
  }
  g_fallbacks = 0;
  TidyCases(h);
  AllocateCases(h);
  if (g_fallbacks != 0) {
    ITC_LOG_INFO("harness: std_string: {} cases handed to the original by the plain C++",
                 g_fallbacks);
  }
}

}  // namespace harness
