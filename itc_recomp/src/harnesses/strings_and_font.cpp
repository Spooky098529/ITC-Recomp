// The string table and the font (src/strings_and_font/,
// docs/functions/strings-and-font.md) against the recompiled functions. The
// objects are built in the scratch area, or in a block of guest memory
// allocated for the harness where they're bigger (the string table's 4,304
// bytes, the font's 16,592), and the globals ([0x826C5C34], [0x826C5C00])
// pointed at them for a case and put back. What the functions store is
// compared: the words stored as floats with harness::Outputs' Float (two
// NaNs match), the rest exactly. The originals run with
// --native_strings_and_font off.

#include "harness.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "strings_and_font/strings_and_font.h"

#include <fmt/format.h>

#include <bit>
#include <iterator>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_strings_and_font);

namespace harness {

namespace {

namespace sf = strings_and_font;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
void NativeStringsInit(PPCContext& ctx, uint8_t* base) {
  const uint32_t table = sf::StringsInit(base);
  if (table != 0) {
    ctx.r3.u64 = table;
  }
}
void NativeStringsOnLoaded(PPCContext& ctx, uint8_t* base) {
  sf::StringsOnLoaded(base, ctx.r6.u32);
}
void NativeStringsRelease(PPCContext& ctx, uint8_t* base) {
  if (!sf::StringsRelease(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_8210A538(ctx, base);
  }
}
void NativeTextIsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sf::TextIsLoaded(base);
}
void NativeStringsIsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sf::StringsIsLoaded(base, ctx.r3.u32);
}
void NativeFontIsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sf::FontIsLoaded(base, ctx.r3.u32);
}
void NativePageCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sf::StringsPageCount(base, ctx.r4.u32);
}
void NativeCopyPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sf::StringsCopyPage(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32, ctx.r7.s32);
}
void NativeInitShaderEntries(PPCContext& ctx, uint8_t* base) {
  sf::InitShaderEntries(base);
}
void NativeSetParams(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sf::FontSetParams(base, ctx.r3.u32, ctx.r4.u32);
}

// Runs the original with the flag off (its callees' adapters fall back) or
// the native step.
void Run(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t, uint8_t* base) {
  if (v == 0) {
    ITC_FLAG(native_strings_and_font) = false;
    recompiled(t, base);
    ITC_FLAG(native_strings_and_font) = true;
  } else {
    native(t, base);
  }
}

// Holds a global's value and puts it back.
struct SavedWord {
  SavedWord(uint8_t* base, uint32_t address)
      : base(base), address(address), value(guest::LoadU32(base, address)) {}
  ~SavedWord() { guest::StoreU32(base, address, value); }
  uint8_t* base;
  uint32_t address;
  uint32_t value;
};

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  uint32_t bytes;
  uint32_t address;
};

// NaNs of both kinds and signs, infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001,
                                   0x7F800000, 0xFF800000, 0x00000001, 0x807FFFFF,
                                   0x00000000, 0x80000000, 0x7F7FFFFF, 0x3F800000};

uint32_t FloatWord(Random& r) {
  switch (r.Below(6)) {
    case 0:
      return kOddFloats[r.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(r.Next());
    case 2:
      return std::bit_cast<uint32_t>(float(r.Below(5)) * 0.5f);  // the game's scales
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 8192.0f);
  }
}

// An int: small (screen coordinates), at the edges, or random bits.
uint32_t IntWord(Random& r) {
  switch (r.Below(6)) {
    case 0:
      return uint32_t(r.Next());
    case 1: {
      constexpr uint32_t kEdges[] = {0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 0, 1, 0x01000001};
      return kEdges[r.Below(std::size(kEdges))];
    }
    default:
      return uint32_t(int32_t(r.Below(2600)) - 600);
  }
}

// A UTF-16 text of `length` units at `address` with its terminator: letters,
// page breaks, new lines and the control characters.
void WriteText(Context& h, uint32_t address, uint32_t length) {
  constexpr uint16_t kUnits[] = {0xFFD1, '\n', ' ', 0xFFD6, 0xFFF6, 0xFFFF, 0xFFD3, 0x00E9};
  for (uint32_t i = 0; i < length; i++) {
    const uint32_t pick = h.random.Below(10);
    const uint16_t unit = pick < std::size(kUnits) ? kUnits[pick] : uint16_t('A' + h.random.Below(26));
    guest::StoreU16(h.base, address + 2 * i, unit);
  }
  guest::StoreU16(h.base, address + 2 * length, 0);
}

void Init(Context& h) {
  Results results("sub_8210A860 Strings::Init");
  if (!hooks::NativeHeapActive()) {
    return;
  }
  SavedWord global(h.base, sf::kStrings);
  for (int i = 0; i < 8; i++) {
    const uint32_t set = i % 2 == 0 ? 0 : uint32_t(h.random.Next()) | 1;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      guest::StoreU32(h.base, sf::kStrings, set);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data;
      Run(v, __imp__sub_8210A860, NativeStringsInit, t, h.base);
      const uint32_t table = guest::LoadU32(h.base, sf::kStrings);
      const bool made = set == 0;
      out[v].Word(made ? table == t.r3.u32 : t.r3.u32);
      out[v].Word(made ? (memory::IsPages(table) ? 1 : 0) : table);
      if (made) {
        out[v].Bytes(Read(h.base, table, 68));
        out[v].Bytes(Read(h.base, table + 136, 4));
        out[v].Bytes(Read(h.base, table + 4236, 68));
        memory::FreePages(table);
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    }
    results.Check(out[0], out[1], fmt::format("case {} (global {:08X})", i, set));
  }
  results.Report();
}

void OnLoaded(Context& h) {
  Results results("sub_8210A850 Strings::OnLoaded");
  for (int i = 0; i < 16; i++) {
    const uint32_t flag = h.data + 4 * h.random.Below(64) + (i % 4 == 3 ? 2 : 0);
    Both(
        h, results, [] {}, [&](PPCContext& t) { t.r6.u64 = flag; },
        __imp__sub_8210A850, NativeStringsOnLoaded, h.data, 512, fmt::format("case {}", i));
  }
  results.Report();
}

void Release(Context& h) {
  Results results("sub_8210A538 Strings::Release");
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Block table(sf::kStringsBytes);
  for (int i = 0; i < 300; i++) {
    // The case: for each file, none, pages, or a pointer never freed (its
    // count not 1); counts 0 to 3 (or random); random flags and mask.
    uint32_t kind[sf::kStringFiles], count[sf::kStringFiles], other[sf::kStringFiles];
    for (uint32_t f = 0; f < sf::kStringFiles; f++) {
      kind[f] = h.random.Below(3);
      count[f] = h.random.Below(8) == 0 ? uint32_t(h.random.Next()) : h.random.Below(4);
      other[f] = uint32_t(h.random.Next()) | 1;
      if (kind[f] == 2 && count[f] == 1) {
        count[f] = 2;
      }
    }
    const uint32_t mask = i % 10 == 0 ? 0x1FFFF : uint32_t(h.random.Next());
    const uint32_t asked = uint32_t(h.random.Next());
    std::vector<uint32_t> flags(sf::kStringFiles);
    for (auto& flag : flags) {
      flag = h.random.Below(2);
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, table.address, table.bytes);
      uint32_t files[sf::kStringFiles];
      for (uint32_t f = 0; f < sf::kStringFiles; f++) {
        files[f] = kind[f] == 0 ? 0 : kind[f] == 1 ? memory::AllocPages(64, 4096) : other[f];
        guest::StoreU32(h.base, table.address + 4 * f, files[f]);
        guest::StoreU32(h.base, table.address + 68 + 4 * f, flags[f]);
        guest::StoreU32(h.base, table.address + 4236 + 4 * f, count[f]);
      }
      guest::StoreU32(h.base, table.address + 136, asked);
      PPCContext t = CallContext(h);
      t.r3.u64 = table.address;
      t.r4.u64 = (uint64_t(h.random.Next()) << 32) | mask;
      Run(v, __imp__sub_8210A538, NativeStringsRelease, t, h.base);
      for (uint32_t f = 0; f < sf::kStringFiles; f++) {
        const uint32_t now = guest::LoadU32(h.base, table.address + 4 * f);
        out[v].Word(now == files[f] ? 1 : now);
        if (kind[f] == 1) {
          const bool left = memory::IsPages(files[f]);
          out[v].Word(left ? 1 : 0);
          if (left) {
            memory::FreePages(files[f]);
          }
        }
      }
      out[v].Bytes(Read(h.base, table.address + 68, table.bytes - 68));
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// A string table at the scratch area's start (its +68 flags and +136 mask)
// and a font at +512 (+12 the mask, +20 the pages) with its four pages at
// +1024 (28 bytes each).
void SetUpLoaded(Context& h) {
  const uint32_t table = h.data, font = h.data + 512;
  const bool all = h.random.Below(2) == 0;
  guest::StoreU32(h.base, table + 136,
                  h.random.Below(6) == 0 ? 0 : uint32_t(h.random.Next()) & (all ? 0x1FFFF : ~0u));
  for (uint32_t f = 0; f < sf::kStringFiles; f++) {
    guest::StoreU32(h.base, table + 68 + 4 * f,
                    all || h.random.Below(8) != 0 ? h.random.Below(3) + 1 : 0);
  }
  guest::StoreU32(h.base, font + 12, h.random.Below(6) == 0 ? 0 : uint32_t(h.random.Next()) & 0x1F);
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t page = h.data + 1024 + 28 * p;
    guest::StoreU32(h.base, font + 20 + 4 * p, page);
    guest::StoreU32(h.base, page + 24,
                    all || h.random.Below(4) != 0 ? uint32_t(h.random.Next()) | 1 : 0);
  }
}

void Loaded(Context& h) {
  SavedWord strings(h.base, sf::kStrings), font(h.base, sf::kFont);
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Function functions[] = {
      {"sub_82211C90 Text::IsLoaded", __imp__sub_82211C90, NativeTextIsLoaded},
      {"sub_8210A768 Strings::IsLoaded", __imp__sub_8210A768, NativeStringsIsLoaded},
      {"sub_821081A0 Font::IsLoaded", __imp__sub_821081A0, NativeFontIsLoaded},
  };
  for (const Function& function : functions) {
    Results results(function.name);
    for (int i = 0; i < 300; i++) {
      const Random start = h.random;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        h.random = start;
        Fill(h.base, h.data, kScratchBytes);
        SetUpLoaded(h);
        guest::StoreU32(h.base, sf::kStrings, h.data);
        guest::StoreU32(h.base, sf::kFont, h.data + 512);
        PPCContext t = CallContext(h);
        t.r3.u64 = function.native == NativeFontIsLoaded ? h.data + 512 : h.data;
        Run(v, function.recompiled, function.native, t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
}

void PageCount(Context& h) {
  Results results("sub_8210A5C0 Strings::PageCount");
  for (int i = 0; i < 200; i++) {
    const uint32_t length = i % 20 == 0 ? 0 : h.random.Below(1000);
    const uint32_t at = h.data + 2 * h.random.Below(8);
    const uint64_t r3 = h.random.Next();
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      WriteText(h, at, length);
      PPCContext t = CallContext(h);
      t.r3.u64 = r3;
      t.r4.u64 = at;
      Run(v, __imp__sub_8210A5C0, NativePageCount, t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

void CopyPage(Context& h) {
  Results results("sub_8210A5E8 Strings::CopyPage");
  for (int i = 0; i < 400; i++) {
    const uint32_t length = i % 25 == 0 ? 0 : h.random.Below(600);
    const uint32_t prefix = h.random.Below(3) == 0 ? h.random.Below(40) : 0;
    const int32_t page = int32_t(h.random.Below(6)) - 1;
    const int32_t skip = int32_t(h.random.Below(5)) - 1;
    const uint32_t text = h.data + 2 * h.random.Below(4);
    const uint32_t output = h.data + 1400 + 2 * h.random.Below(4);
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      WriteText(h, text, length);
      for (uint32_t k = 0; k < prefix; k++) {
        guest::StoreU16(h.base, output + 2 * k, uint16_t('a' + h.random.Below(26)));
      }
      guest::StoreU16(h.base, output + 2 * prefix, 0);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.random.Next();
      t.r4.u64 = output;
      t.r5.u64 = text;
      t.r6.u64 = (uint64_t(h.random.Next()) << 32) | uint32_t(page);
      t.r7.u64 = (uint64_t(h.random.Next()) << 32) | uint32_t(skip);
      Run(v, __imp__sub_8210A5E8, NativeCopyPage, t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} (page {}, skip {})", i, page, skip));
  }
  results.Report();
}

void ShaderEntries(Context& h) {
  Results results("sub_825D9590 (the font's pixel shader entries)");
  const std::vector<uint8_t> saved = Read(h.base, sf::kShaderEntries, 48);
  for (int i = 0; i < 4; i++) {
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      for (uint32_t k = 0; k < 12; k++) {
        guest::StoreU32(h.base, sf::kShaderEntries + 4 * k, i == 0 ? 0 : uint32_t(h.random.Next()));
      }
      PPCContext t = CallContext(h);
      Run(v, __imp__sub_825D9590, NativeInitShaderEntries, t, h.base);
      out[v].Bytes(Read(h.base, sf::kShaderEntries, 48));
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  Write(h.base, sf::kShaderEntries, saved.data(), 48);
  results.Report();
}

void SetParams(Context& h) {
  Results results("sub_82108368 Font::SetParams");
  Block font(16592);
  constexpr uint32_t kFloats[] = {56, 60, 16448, 16452};
  for (int i = 0; i < 1000; i++) {
    const uint32_t params = h.data + 4 * h.random.Below(16);
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t k : kFloats) {
        guest::StoreU32(h.base, font.address + k, 0xCDCDCDCD);
      }
      guest::StoreU32(h.base, font.address + 16, 0xCDCDCDCD);
      for (uint32_t k = 0; k < 128; k += 4) {
        guest::StoreU32(h.base, params + k,
                        k == 28 || k == 32 ? FloatWord(h.random) : IntWord(h.random));
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = font.address;
      t.r4.u64 = params;
      Run(v, __imp__sub_82108368, NativeSetParams, t, h.base);
      out[v].Word(guest::LoadU32(h.base, font.address + 16));
      for (uint32_t k : kFloats) {
        out[v].Float(guest::LoadU32(h.base, font.address + k));
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

}  // namespace

void StringsAndFont(Context& h) {
  Init(h);
  OnLoaded(h);
  Release(h);
  Loaded(h);
  PageCount(h);
  CopyPage(h);
  ShaderEntries(h);
  SetParams(h);
}

}  // namespace harness
