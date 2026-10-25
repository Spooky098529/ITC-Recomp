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
#include "exit_table.h"

#include "base/log.h"
#include "c_runtime/exit.h"
#include "hooks/c_runtime.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_string/std_string.h"

#include <fmt/format.h>

#include <algorithm>
#include <cstring>
#include <iterator>
#include <mutex>
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

void NativeCopy(PPCContext& ctx, uint8_t* base) {
  const uint32_t string = ctx.r3.u32;
  const bool frees = guest::LoadU32(base, string + ss::kCapacity) >= ss::kInPlace;
  const uint32_t block = guest::LoadU32(base, string + ss::kText);
  if (!ss::Copy(base, string, ctx.r4.u32, ctx.r5.u32)) {
    g_fallbacks++;
    __imp__sub_82091458(ctx, base);
    return;
  }
  if (frees) {
    ctx.r3.u64 = block;
  }
}

void NativeGrow(PPCContext& ctx, uint8_t* base) {
  if (const auto grown = ss::Grow(base, ctx.r3.u32, ctx.r4.u32, (ctx.r5.u32 & 0xFF) != 0)) {
    ctx.r3.u64 = *grown ? 1 : 0;
    return;
  }
  g_fallbacks++;
  __imp__sub_82091540(ctx, base);
}

void NativeAssignFrom(PPCContext& ctx, uint8_t* base) {
  if (!ss::AssignFrom(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32)) {
    g_fallbacks++;
    __imp__sub_82091720(ctx, base);
  }
}

void NativeAssignText(PPCContext& ctx, uint8_t* base) {
  if (!ss::AssignText(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    g_fallbacks++;
    __imp__sub_82091808(ctx, base);
  }
}

void NativeAppend(PPCContext& ctx, uint8_t* base) {
  if (!ss::Append(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    g_fallbacks++;
    __imp__sub_820A6F60(ctx, base);
  }
}

void NativeConstruct(PPCContext& ctx, uint8_t* base) {
  ss::Construct(base, ctx.r3.u32);
}

void NativeConstructFromText(PPCContext& ctx, uint8_t* base) {
  if (!ss::ConstructFromText(base, ctx.r3.u32, ctx.r4.u32)) {
    g_fallbacks++;
    __imp__sub_825931D0(ctx, base);
  }
}

void NativeConstructFromTextWithAllocator(PPCContext& ctx, uint8_t* base) {
  if (!ss::ConstructFromTextWithAllocator(base, ctx.r3.u32, ctx.r4.u32)) {
    g_fallbacks++;
    __imp__sub_82593248(ctx, base);
  }
}

void NativeConstructFrom(PPCContext& ctx, uint8_t* base) {
  if (!ss::ConstructFrom(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32)) {
    g_fallbacks++;
    __imp__sub_82593040(ctx, base);
  }
}

// The wildcard replace's step, as its adapter; a match (replace) is the
// original's.
void NativeReplaceAll(PPCContext& ctx, uint8_t* base) {
  if (const auto text = ss::ReplaceAll(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 144)) {
    ctx.r3.u64 = *text;
    return;
  }
  __imp__sub_82593088(ctx, base);
}

// The find and the search have no adapters: their plain C++ with the
// registers they take.
void NativeFind(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ss::Find(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}

void NativeWildcardSearch(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ss::WildcardSearch(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
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

// A C string of `length` characters from `alphabet`.
void WriteText(Context& h, uint32_t address, uint32_t length, const char* alphabet) {
  const uint32_t n = uint32_t(std::strlen(alphabet));
  for (uint32_t i = 0; i < length; i++) {
    guest::StoreU8(h.base, address + i, uint8_t(alphabet[h.random.Below(n)]));
  }
  guest::StoreU8(h.base, address + length, 0);
}

// --- _Copy and _Grow. ---

// A new length: 0, under 16, about the capacity, or well past it (no more
// than a few thousand: lengths near 0xFFFFFFFF are _Xlen's and the new
// handler's paths).
uint32_t PickNewLength(Random& random, uint32_t capacity) {
  switch (random.Below(5)) {
    case 0:
      return 0;
    case 1:
      return random.Below(16);
    case 2:
      return capacity + random.Below(3) - (capacity > 0 ? 1 : 0);
    case 3:
      return random.Below(capacity + 40);
    default:
      return random.Below(3000);
  }
}

void CopyCases(Context& h) {
  Results results("sub_82091458 std::string::_Copy");
  const uint32_t string = h.data + 64;
  for (int i = 0; i < 1500; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const uint32_t capacity = PickCapacity(h.random);
      const uint32_t length = h.random.Below(capacity + 1);
      const uint32_t block = WriteString(h, string, capacity, length);
      // The game's: the length kept, the new length past the capacity.
      // Also less kept, and new lengths at or under it (_Copy doesn't
      // check).
      // A new capacity under 16 (a new length under 16, which the game's
      // _Grow never asks for: its capacities are 15 and up) would keep the
      // block's address in the text area, where it can't be compared.
      uint32_t new_length = h.random.Below(3) != 0 ? capacity + 1 + h.random.Below(200)
                                                   : PickNewLength(h.random, capacity);
      if (new_length < 16) {
        new_length += 16;
      }
      uint32_t keep = h.random.Below(3) != 0 ? length : h.random.Below(length + 1);
      if (keep > (new_length | 15) + 1) {
        keep = 0;  // more than the new block holds is memcpy_s's error
      }
      Blocks blocks;
      blocks.Add(block);
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = new_length;
      t.r5.u64 = keep;
      (v == 0 ? __imp__sub_82091458 : NativeCopy)(t, h.base);
      Collect(h, out[v], t, blocks, {string});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

void GrowCases(Context& h) {
  Results results("sub_82091540 std::string::_Grow");
  const uint32_t string = h.data + 64;
  for (int i = 0; i < 2000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      // Capacities of 15 and up, as the game's (under 15, _Copy for a new
      // length under 16 keeps the block's address in the text area).
      const uint32_t capacity = std::max(PickCapacity(h.random), 15u);
      const uint32_t length = h.random.Below(capacity + 1);
      const uint32_t block = WriteString(h, string, capacity, length);
      const uint32_t new_length = PickNewLength(h.random, capacity);
      static const uint32_t kTrim[] = {0, 1, 0x100, 0x3FF};
      Blocks blocks;
      blocks.Add(block);
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = new_length;
      t.r5.u64 = kTrim[h.random.Below(4)];
      (v == 0 ? __imp__sub_82091540 : NativeGrow)(t, h.base);
      Collect(h, out[v], t, blocks, {string});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// --- assign, append and string(). ---

// A string as the game's: capacity 15 in place, or a heap block of 16 up.
uint32_t PickGameCapacity(Random& random) {
  return random.Below(3) == 0 ? 15 : PickCapacity(random) | 15;
}

void AssignFromCases(Context& h) {
  Results results("sub_82091720 std::string::assign(const string&, size_t, size_t)");
  const uint32_t string = h.data + 64;
  const uint32_t source = h.data + 128;
  for (int i = 0; i < 2000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const uint32_t capacity = PickGameCapacity(h.random);
      const uint32_t block = WriteString(h, string, capacity, h.random.Below(capacity + 1));
      const uint32_t source_capacity = PickGameCapacity(h.random);
      const uint32_t source_length = h.random.Below(source_capacity + 1);
      const uint32_t source_block = WriteString(h, source, source_capacity, source_length);
      // The offset within the source (past it is _Xran's out_of_range); the
      // count -1 (all, the game's), or any.
      const uint32_t offset = h.random.Below(3) == 0 ? 0 : h.random.Below(source_length + 1);
      static const uint32_t kCounts[] = {0xFFFFFFFFu, 0, 1, 15, 16};
      const uint32_t count =
          h.random.Below(2) == 0 ? kCounts[h.random.Below(5)] : h.random.Below(500);
      Blocks blocks;
      blocks.Add(block);
      blocks.Add(source_block);
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = source;
      t.r5.u64 = offset;
      t.r6.u64 = count;
      (v == 0 ? __imp__sub_82091720 : NativeAssignFrom)(t, h.base);
      Collect(h, out[v], t, blocks, {string, source});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// assign and append from a C string: the text in the scratch area (not in
// the string's own text: that's the originals' other paths, never in play).
void FromTextCases(Context& h, bool append) {
  Results results(append ? "sub_820A6F60 std::string::append(const char*, size_t)"
                         : "sub_82091808 std::string::assign(const char*, size_t)");
  const uint32_t string = h.data + 64;
  const uint32_t text = h.data + 1024;
  for (int i = 0; i < 2000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const uint32_t capacity = PickGameCapacity(h.random);
      const uint32_t block = WriteString(h, string, capacity, h.random.Below(capacity + 1));
      const uint32_t count = h.random.Below(4) == 0 ? h.random.Below(17) : h.random.Below(600);
      WriteText(h, text, count, "abcdefghijklmnopqrstuvwxyz0123456789");
      Blocks blocks;
      blocks.Add(block);
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = text;
      t.r5.u64 = count;
      if (append) {
        (v == 0 ? __imp__sub_820A6F60 : NativeAppend)(t, h.base);
      } else {
        (v == 0 ? __imp__sub_82091808 : NativeAssignText)(t, h.base);
      }
      Collect(h, out[v], t, blocks, {string});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

void ConstructCases(Context& h) {
  Results results("sub_825929E8 std::string::string");
  const uint32_t string = h.data + 64;
  for (int i = 0; i < 200; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t k = 0; k < 28; k++) {
        guest::StoreU8(h.base, string + k, uint8_t(h.random.Next()));  // unbuilt memory
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = h.random.Next();
      (v == 0 ? __imp__sub_825929E8 : NativeConstruct)(t, h.base);
      Collect(h, out[v], t, {}, {});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// The constructors from a C string: unbuilt memory, a text of 0 to a few
// hundred characters.
void ConstructFromTextCases(Context& h, bool allocator) {
  Results results(allocator ? "sub_82593248 std::string::string(const char*, const allocator&)"
                            : "sub_825931D0 std::string::string(const char*)");
  const uint32_t string = h.data + 64;
  const uint32_t text = h.data + 1024;
  for (int i = 0; i < 1000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t k = 0; k < 28; k++) {
        guest::StoreU8(h.base, string + k, uint8_t(h.random.Next()));
      }
      const uint32_t length = h.random.Below(3) == 0 ? h.random.Below(17) : h.random.Below(400);
      WriteText(h, text, length, "abcdefghijklmnopqrstuvwxyz_.0123456789");
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = text;
      t.r5.u64 = h.random.Next();
      if (allocator) {
        (v == 0 ? __imp__sub_82593248 : NativeConstructFromTextWithAllocator)(t, h.base);
      } else {
        (v == 0 ? __imp__sub_825931D0 : NativeConstructFromText)(t, h.base);
      }
      Collect(h, out[v], t, {}, {string});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// The copy constructor from part of a string: unbuilt memory, a source as
// the game's, the offset within it, the count -1 (the game's) or any.
void ConstructFromCases(Context& h) {
  Results results("sub_82593040 std::string::string(const string&, size_t, size_t)");
  const uint32_t string = h.data + 64;
  const uint32_t source = h.data + 128;
  for (int i = 0; i < 1000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t k = 0; k < 28; k++) {
        guest::StoreU8(h.base, string + k, uint8_t(h.random.Next()));
      }
      const uint32_t source_capacity = PickGameCapacity(h.random);
      const uint32_t source_length = h.random.Below(source_capacity + 1);
      const uint32_t source_block = WriteString(h, source, source_capacity, source_length);
      const uint32_t offset = h.random.Below(2) == 0 ? 0 : h.random.Below(source_length + 1);
      const uint32_t count = h.random.Below(2) == 0 ? 0xFFFFFFFFu : h.random.Below(500);
      Blocks blocks;
      blocks.Add(source_block);
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = source;
      t.r5.u64 = offset;
      t.r6.u64 = count;
      t.r7.u64 = h.random.Next();
      (v == 0 ? __imp__sub_82593040 : NativeConstructFrom)(t, h.base);
      Collect(h, out[v], t, blocks, {string, source});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// --- The wildcard search, find and replace. ---

// The game's pattern for the loader's loose read (sub_8209DFA8): "/".
constexpr uint32_t kGamePattern = 0x82001DE8;

// A pattern: a few characters from the text's alphabet, '?' and '*', maybe
// anchored at the start (2) or the end (3), never empty (an empty pattern
// tries every start up to the end of memory); or the game's.
uint32_t WritePattern(Context& h, uint32_t address, const char* alphabet) {
  if (h.random.Below(8) == 0) {
    return kGamePattern;
  }
  const uint32_t n = uint32_t(std::strlen(alphabet));
  uint32_t at = address;
  if (h.random.Below(4) == 0) {
    guest::StoreU8(h.base, at++, 2);
  }
  const uint32_t length = 1 + h.random.Below(6);
  for (uint32_t i = 0; i < length; i++) {
    const uint32_t pick = h.random.Below(n + 3);
    uint8_t c = pick < n ? uint8_t(alphabet[pick]) : pick == n ? '?' : '*';
    if (pick == n + 2 && h.random.Below(6) == 0) {
      c = 3;  // an end anchor in the middle
    }
    guest::StoreU8(h.base, at++, c);
  }
  if (h.random.Below(4) == 0) {
    guest::StoreU8(h.base, at++, 3);
  }
  guest::StoreU8(h.base, at, 0);
  return address;
}

const char* PickAlphabet(Random& random) {
  static const char* kAlphabets[] = {"ab", "abc", "a", "abcdefgh", "game:\\data/._"};
  return kAlphabets[random.Below(5)];
}

void WildcardSearchCases(Context& h) {
  Results results("sub_82592740 WildcardSearch");
  const uint32_t text = h.data + 1024;
  const uint32_t pattern = h.data + 2048;
  const uint32_t outs = h.data + 3072;
  for (int i = 0; i < 3000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const char* alphabet = PickAlphabet(h.random);
      const uint32_t length = h.random.Below(24);
      WriteText(h, text, length, alphabet);
      if (h.random.Below(2) == 0) {
        Fill(h.base, text + length + 1, 64, 0);  // zeros after the text
      }
      const uint32_t p = WritePattern(h, pattern, alphabet);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.random.Below(20) == 0 ? 0 : text + h.random.Below(length + 1);
      t.r4.u64 = p;
      t.r5.u64 = h.random.Below(4) == 0 ? 0 : outs;
      t.r6.u64 = h.random.Below(4) == 0 ? 0 : outs + 4;
      (v == 0 ? __imp__sub_82592740 : NativeWildcardSearch)(t, h.base);
      Collect(h, out[v], t, {}, {});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

void FindCases(Context& h) {
  Results results("sub_82592970 String::Find");
  const uint32_t string = h.data + 64;
  const uint32_t pattern = h.data + 2048;
  const uint32_t outs = h.data + 3072;
  for (int i = 0; i < 2000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const char* alphabet = PickAlphabet(h.random);
      const uint32_t capacity = PickCapacity(h.random);
      const uint32_t length = h.random.Below(std::min(capacity, 40u) + 1);
      const uint32_t block = WriteString(h, string, capacity, length);
      WriteText(h, ss::Text(h.base, string), length, alphabet);
      const uint32_t p = WritePattern(h, pattern, alphabet);
      Blocks blocks;
      blocks.Add(block);
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = p;
      t.r5.u64 = h.random.Below(4) == 0 ? 0 : outs;
      t.r6.u64 = h.random.Below(4) == 0 ? 0 : outs + 4;
      t.r7.u64 = h.random.Below(length + 1);
      guest::StoreU32(h.base, outs, h.random.Below(3) == 0 ? 0 : uint32_t(h.random.Next()));
      (v == 0 ? __imp__sub_82592970 : NativeFind)(t, h.base);
      Collect(h, out[v], t, blocks, {string});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// The replace's cases have no match (the pattern holds a character the text
// doesn't: the path the game takes), except with the game's pattern ("/",
// replaced with "\\") over path-like text, which may match and run the
// original on both sides. Capacities as the game's (15 up): with one under
// 15, a match's replace grows the string through _Copy to a capacity under
// 16, which keeps the new block's address in the text area (the original's
// own quirk, on both sides), where it can't be compared.
void ReplaceAllCases(Context& h) {
  Results results("sub_82593088 String::ReplaceAll");
  const uint32_t string = h.data + 64;
  const uint32_t pattern = h.data + 2048;
  const uint32_t replacement = h.data + 2560;
  for (int i = 0; i < 1000; i++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const char* alphabet = PickAlphabet(h.random);
      const uint32_t capacity = PickGameCapacity(h.random);
      const uint32_t length = h.random.Below(std::min(capacity, 40u) + 1);
      const uint32_t block = WriteString(h, string, capacity, length);
      WriteText(h, ss::Text(h.base, string), length, alphabet);
      uint32_t p = WritePattern(h, pattern, alphabet);
      if (p != kGamePattern) {
        // No match: 'Z' (in no alphabet) as the first character after an
        // anchor. A match runs the original, whose replace loops for ever on
        // a pattern that matches anything (such as "*?*").
        const uint32_t at = guest::LoadU8(h.base, pattern) == 2 ? pattern + 1 : pattern;
        guest::StoreU8(h.base, at, 'Z');
      }
      WriteText(h, replacement, h.random.Below(4), "xyz");
      Blocks blocks;
      blocks.Add(block);
      PPCContext t = CallContext(h);
      t.r3.u64 = string;
      t.r4.u64 = p;
      t.r5.u64 = replacement;
      t.r6.u64 = h.random.Below(3);
      (v == 0 ? __imp__sub_82593088 : NativeReplaceAll)(t, h.base);
      Collect(h, out[v], t, blocks, {string});
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

}  // namespace

// --- The global strings' static set-ups (on the game's strings and exit
// table, each put back after a side). ---

void NativeStaticInit(PPCContext& ctx, uint8_t* base, bool second) {
  hooks::GuestAtexit guest{ctx, base};
  const auto result = second ? ss::InitGlobalString8265EE98(base, guest.Call())
                             : ss::InitGlobalString8265EE7C(base, guest.Call());
  if (result) {
    ctx.r3.s64 = *result;
  } else {
    g_fallbacks++;
    (second ? __imp__sub_825DA580 : __imp__sub_825DA540)(ctx, base);
  }
}

void StaticInitCases(Context& h) {
  Results results("sub_825DA540, sub_825DA580 (the global strings' static set-ups)");
  if (!hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: std_string: native atexit off: the static set-ups' cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  for (int c = 0; c < 16; c++) {
    const bool second = (c & 1) != 0;
    const uint32_t string = second ? 0x8265EE98 : 0x8265EE7C;
    const auto saved = Read(h.base, string, 28);
    // The game's string as it is, or another in place (its capacity 15).
    std::vector<uint8_t> start = saved;
    if (c >= 2) {
      for (auto& byte : start) {
        byte = uint8_t(h.random.Next());
      }
      const uint32_t length = h.random.Below(16);
      for (uint32_t i = 0; i < 16; i++) {
        start[4 + i] = i < length ? uint8_t('a' + h.random.Below(26)) : 0;
      }
      for (int k = 0; k < 4; k++) {
        start[20 + k] = uint8_t(length >> (24 - 8 * k));
        start[24 + k] = uint8_t(15 >> (24 - 8 * k));
      }
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, string, start.data(), 28);
      const uint32_t mark = exit_table::Mark(h.base);
      PPCContext t = CallContext(h);
      if (v == 0) {
        (second ? __imp__sub_825DA580 : __imp__sub_825DA540)(t, h.base);
      } else {
        NativeStaticInit(t, h.base, second);
      }
      out[v].Bytes(Read(h.base, string, 28));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      exit_table::Take(h.base, mark, out[v]);
    }
    Write(h.base, string, saved.data(), 28);
    results.Check(out[0], out[1], fmt::format("case {} ({})", c, second ? "0x8265EE98" : "0x8265EE7C"));
  }
  results.Report();
}

void StdString(Context& h) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: std_string: skipped (--native_heap off)");
    return;
  }
  g_fallbacks = 0;
  TidyCases(h);
  AllocateCases(h);
  CopyCases(h);
  GrowCases(h);
  AssignFromCases(h);
  FromTextCases(h, false);
  FromTextCases(h, true);
  ConstructCases(h);
  ConstructFromTextCases(h, false);
  ConstructFromTextCases(h, true);
  ConstructFromCases(h);
  WildcardSearchCases(h);
  FindCases(h);
  ReplaceAllCases(h);
  StaticInitCases(h);
  if (g_fallbacks != 0) {
    ITC_LOG_INFO("harness: std_string: {} cases handed to the original by the plain C++",
                 g_fallbacks);
  }
}

}  // namespace harness
