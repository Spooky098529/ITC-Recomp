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
#include "base/log.h"
#include "command_list/command_list.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/strings_and_font.h"
#include "memory/c_runtime_heap.h"
#include "models/xpr.h"
#include "memory/heap.h"
#include "strings_and_font/strings_and_font.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>
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

void NativeTextWidth(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = sf::FontTextWidth(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
void NativeLineWidth(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = sf::FontLineWidth(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeLineGlyphs(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sf::FontLineGlyphs(base, ctx.r4.u32);
}
void NativeLineX(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = sf::FontLineX(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeLineY(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = sf::FontLineY(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64);
}
void NativeCountGlyphs(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sf::FontCountGlyphs(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
void NativeOutCode(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sf::FontOutCode(base, ctx.r3.u32, ctx.r4.u32);
}

void NativeClipGlyph(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sf::FontClipGlyph(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeAddGlyph(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!sf::FontAddGlyph(base, ctx.r3.u32, ctx.r6.u32, ctx.r7.u32, ctx.f1.f64, ctx.f2.f64)) {
    __imp__sub_82109560(ctx, base);
  }
}
void NativeBuildVertices(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!sf::FontBuildVertices(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_82109AA8(ctx, base);
    return;
  }
  ctx.r3.u64 = 0;
}

void NativeSwprintf(PPCContext& ctx, uint8_t* base) {
  const uint64_t registers[6] = {ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                                 ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  for (uint32_t i = 0; i < 6; i++) {
    StoreU64(base, ctx.r1.u32 + 32 + 8 * i, registers[i]);
  }
  int32_t count;
  if (!sf::Swprintf(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 + 32, ctx.r1.u32 - 96 - 144 - 2336,
                    &count)) {
    __imp__sub_8210A490(ctx, base);
    return;
  }
  ctx.r3.u64 = uint32_t(count);
}

void NativeFormatGrouped(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!sf::StringsFormatGrouped(base, ctx.r4.s32, ctx.r5.u32, ctx.r1.u32 - 640)) {
    __imp__sub_8210A680(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r5.u32;
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

// A font for the layout's cases: the font in `font` (its +16448 and +16452
// spacings random floats), its current page (+36) and the four pages (+20)
// at the scratch area's +2048 (28 bytes each: +0 the glyph table at +2304,
// its cell width and height 16-bit; +4 the records, the block `records` of
// 65,536 x 4 bytes, random), and the draw parameters at the scratch area's
// +2560 (+16 points at them): ints (small, edges or random bits) but the
// floats +16 to +40, the alignments (+76, +80) 0-3 or random.
struct Layout {
  Layout() : font(16592), records(0x40000) {}
  Block font;
  Block records;
};

constexpr uint32_t kPages = 2048, kTable = 2304, kParams = 2560, kTexts = 64;

void SetUpLayout(Context& h, const Layout& l, bool fill_records) {
  if (fill_records) {
    for (uint32_t k = 0; k < l.records.bytes; k += 4) {
      // The advance (x 4) small, the channel 0-3; sometimes any bits.
      const uint32_t word = h.random.Below(8) == 0
                                ? uint32_t(h.random.Next())
                                : (h.random.Below(256) << 16) | (h.random.Below(256) << 2) |
                                      h.random.Below(4);
      guest::StoreU32(h.base, l.records.address + k, word);
    }
  }
  for (uint32_t k : {16u, 36u, 56u, 60u, 16448u, 16452u}) {
    guest::StoreU32(h.base, l.font.address + k, 0xCDCDCDCD);
  }
  guest::StoreU32(h.base, l.font.address + 16448, FloatWord(h.random));
  guest::StoreU32(h.base, l.font.address + 16452, FloatWord(h.random));
  guest::StoreU32(h.base, l.font.address + 56, FloatWord(h.random));
  guest::StoreU32(h.base, l.font.address + 60, FloatWord(h.random));
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t page = h.data + kPages + 28 * p;
    guest::StoreU32(h.base, page + 0, h.data + kTable);
    guest::StoreU32(h.base, page + 4, l.records.address);
    guest::StoreU32(h.base, l.font.address + 20 + 4 * p, page);
  }
  guest::StoreU32(h.base, l.font.address + 36, h.data + kPages + 28 * h.random.Below(4));
  guest::StoreU16(h.base, h.data + kTable + 0, uint16_t(h.random.Below(64)));
  guest::StoreU16(h.base, h.data + kTable + 2,
                  h.random.Below(8) == 0 ? uint16_t(h.random.Next()) : uint16_t(h.random.Below(64)));
  const uint32_t params = h.data + kParams;
  for (uint32_t k = 0; k < 120; k += 4) {
    uint32_t word = k >= 16 && k <= 40 ? FloatWord(h.random) : IntWord(h.random);
    if (k == 60 || k == 108) {
      word = h.random.Below(4);  // the page (any other reads past the font)
    } else if (k == 76 || k == 80) {
      word = h.random.Below(12) == 0 ? uint32_t(h.random.Next()) : h.random.Below(4);
    } else if (k >= 44 && k <= 56 && h.random.Below(3) == 0) {
      word = 0;
    }
    guest::StoreU32(h.base, params + k, word);
  }
  guest::StoreU32(h.base, l.font.address + 16, params);
}

// The font's words the functions store: +16 and +36 exactly, the floats
// with Float.
void FontOutputs(Context& h, const Layout& l, Outputs& out) {
  out.Word(guest::LoadU32(h.base, l.font.address + 16));
  out.Word(guest::LoadU32(h.base, l.font.address + 36));
  for (uint32_t k : {56u, 60u, 16448u, 16452u}) {
    out.Float(guest::LoadU32(h.base, l.font.address + k));
  }
}

void LayoutCases(Context& h, const Layout& l, const char* name, GuestFunction recompiled,
                 GuestFunction native, int count, bool f1_out,
                 const std::function<void(PPCContext&, uint32_t)>& args) {
  Results results(name);
  for (int i = 0; i < count; i++) {
    const uint32_t length = i % 20 == 0 ? 0 : h.random.Below(40);
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      SetUpLayout(h, l, false);
      const uint32_t text = h.data + kTexts + 2 * h.random.Below(4);
      WriteText(h, text, length);
      PPCContext t = CallContext(h);
      t.r3.u64 = l.font.address;
      t.f1.u64 = h.random.Next();
      args(t, text);
      Run(v, recompiled, native, t, h.base);
      FontOutputs(h, l, out[v]);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      if (f1_out) {
        out[v].Double(t.f1.u64);
      } else {
        out[v].Word(t.r3.u32);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

void FontLayout(Context& h) {
  Layout l;
  SetUpLayout(h, l, true);
  LayoutCases(h, l, "sub_82108B30 Font::TextWidth", __imp__sub_82108B30, NativeTextWidth, 1000,
              true, [&](PPCContext& t, uint32_t text) {
                t.r4.u64 = h.data + kParams;
                t.r5.u64 = text;
              });
  LayoutCases(h, l, "sub_82108458 Font::LineWidth", __imp__sub_82108458, NativeLineWidth, 1000,
              true, [&](PPCContext& t, uint32_t text) { t.r4.u64 = text; });
  LayoutCases(h, l, "sub_82108810 (a justified line's glyphs)", __imp__sub_82108810,
              NativeLineGlyphs, 300, false, [&](PPCContext& t, uint32_t text) {
                t.r3.u64 = h.random.Next();
                t.r4.u64 = text;
              });
  LayoutCases(h, l, "sub_82108FE0 Font::LineX", __imp__sub_82108FE0, NativeLineX, 1500, true,
              [&](PPCContext& t, uint32_t text) { t.r4.u64 = text; });
  LayoutCases(h, l, "sub_82108568 Font::LineY", __imp__sub_82108568, NativeLineY, 1500, true,
              [&](PPCContext& t, uint32_t) {
                const uint32_t lines = h.random.Below(6) == 0 ? uint32_t(h.random.Next())
                                                              : h.random.Below(6);
                t.r4.u64 = (uint64_t(h.random.Next()) << 32) | lines;
                t.r5.u64 = (uint64_t(h.random.Next()) << 32) |
                           (h.random.Below(6) == 0 ? uint32_t(h.random.Next()) : h.random.Below(6));
              });
  LayoutCases(h, l, "sub_82108210 Font::CountGlyphs", __imp__sub_82108210, NativeCountGlyphs,
              300, false, [&](PPCContext& t, uint32_t text) {
                t.r4.u64 = h.data + 4 * h.random.Below(8);
                t.r5.u64 = text;
              });
  LayoutCases(h, l, "sub_821082A0 Font::OutCode", __imp__sub_821082A0, NativeOutCode, 1000,
              false, [&](PPCContext& t, uint32_t) {
                const uint32_t vertex = h.data + 3072;
                guest::StoreU32(h.base, vertex + 0,
                                h.random.Below(2) == 0 ? FloatWord(h.random)
                                                       : std::bit_cast<uint32_t>(float(
                                                             int32_t(h.random.Below(2000)) - 400)));
                guest::StoreU32(h.base, vertex + 4,
                                h.random.Below(2) == 0 ? FloatWord(h.random)
                                                       : std::bit_cast<uint32_t>(float(
                                                             int32_t(h.random.Below(2000)) - 400)));
                t.r4.u64 = vertex;
              });
}

// The drawing's globals (0x826C5BCC, 52 bytes), the font's pointer and the
// open chunks' count, kept and put back.
struct DrawGlobals {
  explicit DrawGlobals(uint8_t* base)
      : base(base),
        saved(Read(base, sf::kDrawGlobals, 52)),
        font(guest::LoadU32(base, sf::kFont)),
        open(guest::LoadU32(base, command_list::kOpenChunks)) {}
  ~DrawGlobals() {
    Write(base, sf::kDrawGlobals, saved.data(), 52);
    guest::StoreU32(base, sf::kFont, font);
    guest::StoreU32(base, command_list::kOpenChunks, open);
  }
  uint8_t* base;
  std::vector<uint8_t> saved;
  uint32_t font, open;
};

// Vertices (24 bytes: x, y, z, the color, u, v) for Outputs.
void VertexOutputs(Context& h, Outputs& out, uint32_t address, uint32_t count) {
  for (uint32_t k = 0; k < count; k++) {
    const uint32_t v = address + 24 * k;
    out.Floats(h.base, v, 3);
    out.Word(guest::LoadU32(h.base, v + 12));
    out.Floats(h.base, v + 16, 2);
  }
}

// The drawing's set-up on top of SetUpLayout: the pages' textures (+12 a
// word, +16 and +20 the width and height: powers of two or random), the
// cell's size not 0 (but two AddGlyph cases: the original's tw traps,
// logged, and its divisions give 0), the font's ten colors, shader words
// and command list and queue; the parameters' angle whole turns or 0 (a few
// random: rotated text), the alpha 0-1 and the clip rectangle around the
// text or none; the text drawn mostly letters.
void SetUpDrawing(Context& h, const Layout& l, int i, bool zero_cells = false) {
  SetUpLayout(h, l, false);
  for (uint32_t p = 0; p < 4; p++) {
    const uint32_t page = h.data + kPages + 28 * p;
    guest::StoreU32(h.base, page + 12, uint32_t(h.random.Next()));
    guest::StoreU32(h.base, page + 16,
                    h.random.Below(8) == 0 ? uint32_t(h.random.Next()) : 64u << h.random.Below(4));
    guest::StoreU32(h.base, page + 20,
                    h.random.Below(8) == 0 ? uint32_t(h.random.Next()) : 64u << h.random.Below(4));
  }
  guest::StoreU16(h.base, h.data + kTable + 0,
                  zero_cells && i % 300 == 5 ? 0 : uint16_t(1 + h.random.Below(40)));
  guest::StoreU16(h.base, h.data + kTable + 2, uint16_t(1 + h.random.Below(40)));
  for (uint32_t k = 0; k < 32; k++) {
    guest::StoreU8(h.base, l.font.address + 16528 + k, uint8_t(h.random.Next()));
  }
  for (uint32_t k = 64; k < 400; k += 4) {  // the second pass's positions
    guest::StoreU32(h.base, l.font.address + k, 0xCDCDCDCD);
  }
  for (uint32_t k = 16560; k < 16584; k += 4) {
    guest::StoreU32(h.base, l.font.address + k, uint32_t(h.random.Next()));
  }
  const uint32_t params = h.data + kParams;
  const uint32_t angle = h.random.Below(10) == 0 ? FloatWord(h.random)
                                                  : std::bit_cast<uint32_t>(360.0f * float(h.random.Below(3)));
  guest::StoreU32(h.base, params + 16, angle);
  guest::StoreU32(h.base, params + 24,
                  h.random.Below(6) == 0 ? FloatWord(h.random)
                                         : std::bit_cast<uint32_t>(float(h.random.Below(257)) / 256.0f));
  if (h.random.Below(2) == 0) {
    for (uint32_t k = 44; k <= 56; k += 4) {
      guest::StoreU32(h.base, params + k, 0);
    }
  } else {
    guest::StoreU32(h.base, params + 44, uint32_t(int32_t(h.random.Below(600)) - 100));
    guest::StoreU32(h.base, params + 48, uint32_t(int32_t(h.random.Below(600)) - 100));
    guest::StoreU32(h.base, params + 52, h.random.Below(800));
    guest::StoreU32(h.base, params + 56, h.random.Below(800));
  }
  guest::StoreU32(h.base, params + 112, h.random.Below(10) == 0 ? 1 : 0);
  guest::StoreU32(h.base, params + 116, h.random.Below(8));
  // Scales and positions a screen's.
  for (uint32_t k : {28u, 32u, 36u, 40u}) {
    if (h.random.Below(3) != 0) {
      guest::StoreU32(h.base, params + k, std::bit_cast<uint32_t>(float(1 + h.random.Below(8)) / 4.0f));
    }
  }
}

// A text of mostly glyphs for drawing: letters, spaces, new lines, colors
// and a page break now and then.
void WriteDrawnText(Context& h, uint32_t address, uint32_t length) {
  for (uint32_t k = 0; k < length; k++) {
    const uint32_t pick = h.random.Below(40);
    uint16_t unit = uint16_t('A' + h.random.Below(58));
    if (pick == 0) unit = 0xFFD1;
    else if (pick < 4) unit = '\n';
    else if (pick < 8) unit = ' ';
    else if (pick == 8) unit = 0xFFD6;
    else if (pick == 9) unit = uint16_t(0xFFF6 + h.random.Below(10));
    else if (pick == 10) unit = uint16_t(h.random.Next());
    if (unit == 0) unit = 'A';
    guest::StoreU16(h.base, address + 2 * k, unit);
  }
  guest::StoreU16(h.base, address + 2 * length, 0);
}

void ClipCases(Context& h, const Layout& l) {
  Results results("sub_82108C90 Font::ClipGlyph");
  for (int i = 0; i < 1500; i++) {
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      SetUpDrawing(h, l, i);
      const uint32_t vertices = h.data + 3200;
      for (uint32_t k = 0; k < 36; k++) {
        const uint32_t word = (k % 6) == 3 ? uint32_t(h.random.Next())
                              : h.random.Below(8) == 0
                                  ? FloatWord(h.random)
                                  : std::bit_cast<uint32_t>(float(int32_t(h.random.Below(1000)) - 200) +
                                                            float(h.random.Below(4)) * 0.25f);
        guest::StoreU32(h.base, vertices + 4 * k, word);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = l.font.address;
      t.r4.u64 = vertices;
      Run(v, __imp__sub_82108C90, NativeClipGlyph, t, h.base);
      VertexOutputs(h, out[v], vertices, 6);
      out[v].Bytes(Read(h.base, h.data, 3200));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

void AddGlyphCases(Context& h, const Layout& l) {
  Results results("sub_82109560 Font::AddGlyph");
  for (int i = 0; i < 1500; i++) {
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      SetUpDrawing(h, l, i, true);
      // The counts, and each channel's write pointer at its own 160 bytes.
      const uint32_t area = h.data + 3200;
      for (uint32_t c = 0; c < 4; c++) {
        guest::StoreU32(h.base, sf::kDrawGlobals + 4 * c, h.random.Below(5));
        guest::StoreU32(h.base, sf::kDrawGlobals + 16 + 4 * c, area + 160 * c);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = l.font.address;
      t.r6.u64 = h.random.Next();
      t.r7.u64 = h.random.Next();
      auto coordinate = [&] {
        return h.random.Below(8) == 0
                   ? h.random.Next()
                   : std::bit_cast<uint64_t>(double(float(int32_t(h.random.Below(900)) - 100)));
      };
      t.f1.u64 = coordinate();
      t.f2.u64 = coordinate();
      Run(v, __imp__sub_82109560, NativeAddGlyph, t, h.base);
      VertexOutputs(h, out[v], area, 26);
      out[v].Bytes(Read(h.base, sf::kDrawGlobals, 48));
      out[v].Bytes(Read(h.base, h.data, 3200));
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

void BuildVerticesCases(Context& h, const Layout& l, const Block& area) {
  Results results("sub_82109AA8 Font::BuildVertices");
  for (int i = 0; i < 600; i++) {
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, area.address, area.bytes);
      SetUpDrawing(h, l, i);
      const uint32_t text = h.data + kTexts;
      const uint32_t length = i % 30 == 0 ? 0 : h.random.Below(40);
      WriteDrawnText(h, text, length);
      for (uint32_t c = 0; c < 4; c++) {
        guest::StoreU32(h.base, sf::kDrawGlobals + 4 * c, 40);
        guest::StoreU32(h.base, sf::kDrawGlobals + 16 + 4 * c, area.address + 6000 * c);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = l.font.address;
      t.r4.u64 = text;
      Run(v, __imp__sub_82109AA8, NativeBuildVertices, t, h.base);
      for (uint32_t c = 0; c < 4; c++) {
        VertexOutputs(h, out[v], area.address + 6000 * c, 240);
      }
      out[v].Bytes(Read(h.base, sf::kDrawGlobals, 48));
      out[v].Floats(h.base, l.font.address + 64, 84);
      FontOutputs(h, l, out[v]);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

// Font::DrawPass, Font::Draw and Text::Draw through their adapters (they
// hand the chunk to the render queue, a guest call): a command list at the
// scratch area's +3072 over `slots`, the queue at +3200 (its slot 4
// sub_8209F5C8, which keeps r4 and r5 at its +32 and +36), the open chunks'
// count 0 or 1 (another chunk open: the original), the list's chunk open
// or not. Compared: the slots (the vertices as floats), the list, the
// globals, the queue's words, the font's.
void DrawCases(Context& h, const Layout& l, const Block& slots, const char* name,
               GuestFunction function, GuestFunction original, int count) {
  Results results(name);
  constexpr uint32_t kList = 3072, kQueue = 3200, kQueueVtable = 3264;
  for (int i = 0; i < count; i++) {
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, slots.address, slots.bytes);
      SetUpDrawing(h, l, i);
      const uint32_t text = h.data + kTexts;
      const uint32_t length = i % 30 == 0 ? 0 : h.random.Below(40);
      WriteDrawnText(h, text, length);
      const uint32_t list = h.data + kList;
      const bool open = h.random.Below(6) == 0;
      for (uint32_t k = 0; k < 56; k += 4) {
        guest::StoreU32(h.base, list + k, 0);
      }
      guest::StoreU32(h.base, list + 8, 100000);
      guest::StoreU32(h.base, list + 12, h.random.Below(1000));
      guest::StoreU32(h.base, list + 32, slots.address);
      guest::StoreU32(h.base, list + 44, slots.address + (open ? 64 : 0));
      guest::StoreU8(h.base, list + 52, open ? 1 : 0);
      guest::StoreU32(h.base, command_list::kOpenChunks, h.random.Below(8) == 0 ? 1 : 0);
      guest::StoreU32(h.base, h.data + kQueue, h.data + kQueueVtable);
      guest::StoreU32(h.base, h.data + kQueueVtable + 16, 0x8209F5C8);
      guest::StoreU32(h.base, l.font.address + 4, list);
      guest::StoreU32(h.base, l.font.address + 8, h.data + kQueue);
      guest::StoreU32(h.base, sf::kFont, l.font.address);
      Fill(h.base, sf::kDrawGlobals, 52);
      PPCContext t = CallContext(h);
      t.r3.u64 = l.font.address;
      t.r4.u64 = function == __imp__sub_82109EF0 || original == __imp__sub_82109EF0
                     ? text
                     : h.data + kParams;
      t.r5.u64 = function == __imp__sub_82109EF0 || original == __imp__sub_82109EF0
                     ? (h.random.Below(10) == 0 ? 1 : 0)
                     : text;
      if (v == 0) {
        ITC_FLAG(native_strings_and_font) = false;
        original(t, h.base);
        ITC_FLAG(native_strings_and_font) = true;
      } else {
        function(t, h.base);
      }
      // The slots, with each channel's vertices as floats: all the room
      // the pass took for them (the {21, bytes, 0} slot before them), as
      // clipping lowers the counts after the vertices are built, and the
      // clipped glyphs' vertices stay in the slots.
      std::vector<uint8_t> bytes = Read(h.base, slots.address, slots.bytes);
      for (uint32_t c = 0; c < 4; c++) {
        const uint32_t from = guest::LoadU32(h.base, sf::kDrawGlobals + 32 + 4 * c);
        uint32_t n = guest::LoadU32(h.base, sf::kDrawGlobals + 4 * c) * 6;
        if (from >= slots.address + 16 && from <= slots.address + slots.bytes &&
            guest::LoadU32(h.base, from - 16) == 21) {
          n = std::max(n, guest::LoadU32(h.base, from - 12) / 24);
        }
        if (from >= slots.address && from + 24 * n <= slots.address + slots.bytes) {
          VertexOutputs(h, out[v], from, n);
          std::fill(bytes.begin() + (from - slots.address),
                    bytes.begin() + (from - slots.address) + 24 * n, 0);
        }
      }
      out[v].Bytes(bytes);
      out[v].Bytes(Read(h.base, sf::kDrawGlobals, 52));
      out[v].Word(guest::LoadU32(h.base, command_list::kOpenChunks));
      out[v].Floats(h.base, l.font.address + 64, 84);
      FontOutputs(h, l, out[v]);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
  }
  results.Report();
}

void FontDrawing(Context& h) {
  const DrawGlobals globals(h.base);
  Layout l;
  SetUpLayout(h, l, true);
  Block area(24000);
  ClipCases(h, l);
  AddGlyphCases(h, l);
  BuildVerticesCases(h, l, area);
  // The adapters (the flag on) against the originals (the flag off).
  DrawCases(h, l, area, "sub_82109EF0 Font::DrawPass", sub_82109EF0, __imp__sub_82109EF0, 600);
  DrawCases(h, l, area, "sub_8210A400 Font::Draw", sub_8210A400, __imp__sub_8210A400, 300);
  DrawCases(h, l, area, "sub_8210A758 Text::Draw", sub_8210A758, __imp__sub_8210A758, 300);
}

// swprintf: formats made of literal characters (some 0xFFxx, a few 0xFFFF:
// the original runs) and conversions: flags, a width (digits or '*'), a
// precision (digits or '*'), a size (h, l, ll, w, I64, I32, I) and a type
// (d i u o x X p c C s S f, e and g (the original), '%' and a letter that
// isn't a type); the game's own formats; the arguments to match, in r5 to
// r10 and on the stack: ints (small, edges, random), strings (wide and
// narrow, or null), doubles (odd and ordinary values). The format at the
// scratch area's +512, the strings at +0, the output in a 16 KB block (the
// longest output, six %f of 1e308 with a precision of 163, is ~6 KB);
// compared: the block, the scratch area, the caller's parameter area and
// stack arguments, r3.
struct FormatCase {
  std::u16string format;
  std::vector<uint64_t> slots;
};

void AddInt(Random& r, std::vector<uint64_t>& slots, bool wide64) {
  uint64_t v;
  switch (r.Below(5)) {
    case 0: v = r.Next(); break;
    case 1: {
      constexpr uint64_t kEdges[] = {0, 1, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF, 0xFFFF,
                                     0x8000, 0xFFFFFFFFFFFFFFFFull, 0x8000000000000000ull};
      v = kEdges[r.Below(std::size(kEdges))];
      break;
    }
    default: v = uint64_t(int64_t(int32_t(r.Below(200000)) - 100000)); break;
  }
  if (!wide64) v = uint64_t(int64_t(int32_t(uint32_t(v))));
  slots.push_back(v);
}

uint64_t DoubleBits(Random& r) {
  constexpr double kValues[] = {0.0, -0.0, 0.5, -3.25, 123.456, 1e10, 1e-5, 99.995, 2.5, 1e300,
                                -1e-300, 4294967296.5, 0.1, 7.0};
  switch (r.Below(6)) {
    case 0: return r.Next();
    case 1: {
      constexpr uint64_t kOdd[] = {0x7FF8000000000000ull, 0xFFF0000000000000ull,
                                   0x7FF0000000000000ull, 0x0000000000000001ull,
                                   0x7FF4000000000001ull, 0x8000000000000000ull};
      return kOdd[r.Below(std::size(kOdd))];
    }
    case 2: return std::bit_cast<uint64_t>(kValues[r.Below(std::size(kValues))]);
    default:
      return std::bit_cast<uint64_t>(double(int32_t(r.Below(2000000)) - 1000000) / 100.0);
  }
}

FormatCase MakeFormat(Random& r, uint32_t strings) {
  FormatCase c;
  const uint32_t pieces = 1 + r.Below(6);
  bool bare_i = false;  // the last piece ended in a bare "I" (below)
  for (uint32_t k = 0; k < pieces; k++) {
    // After a bare "I", a literal letter could be an integer type ("Ix"):
    // the next piece is a conversion.
    if (!std::exchange(bare_i, false) && r.Below(3) == 0) {
      const uint32_t pick = r.Below(30);
      c.format += pick == 0 ? char16_t(0xFFFF) : pick == 1 ? char16_t(0xFFD3) : pick == 2 ? u'\n'
                : char16_t('a' + r.Below(26));
      continue;
    }
    c.format += u'%';
    const char16_t* kFlags = u"-+ #0";
    for (uint32_t f = 0; f < 5; f++) {
      if (r.Below(5) == 0) c.format += kFlags[f];
    }
    if (r.Below(3) == 0) {
      if (r.Below(4) == 0) {
        c.format += u'*';  // a small width (a huge one would write gigabytes)
        c.slots.push_back(uint64_t(int64_t(int32_t(r.Below(80)) - 40)));
      } else {
        for (char ch : std::to_string(r.Below(30))) c.format += char16_t(ch);
      }
    }
    bool big_precision = false;
    if (r.Below(3) == 0) {
      c.format += u'.';
      if (r.Below(4) == 0) {
        c.format += u'*';
        c.slots.push_back(uint64_t(int64_t(int32_t(r.Below(60)) - 10)));
      } else if (r.Below(3) != 0) {
        const uint32_t n = r.Below(12) == 0 ? 160 + r.Below(40) : r.Below(25);
        big_precision = n > 100;
        for (char ch : std::to_string(n)) c.format += char16_t(ch);
      }
    }
    const char16_t* kSizes[] = {u"", u"", u"", u"h", u"l", u"ll", u"w", u"I64", u"I32", u"I"};
    const std::u16string size = kSizes[r.Below(std::size(kSizes))];
    c.format += size;
    // ('k' isn't a type: the last conversion's text again. Not 'F' or
    // 'N': the table takes them as sizes, and the arguments after would be
    // read as the wrong kinds.)
    const char16_t* kTypes = u"diuoxXpcCsSfffdegE%kk";
    const char16_t type = kTypes[r.Below(21)];
    // A bare "I" ends the conversion, and a '%' after it starts a new one
    // (the parser goes back to plain text: _woutput_l's state table), so
    // "%I%" isn't a conversion of its own. Leaving the '%' out puts the next
    // piece's '%' (or the format's end) after the "I": that path, with that
    // piece's arguments.
    if (size == u"I" && type == u'%') {
      bare_i = true;
      continue;
    }
    c.format += type;
    const bool wide64 = size == u"ll" || size == u"I64";
    // A bare "I" before anything but an integer type is written out, and
    // the type after it with it: no argument (or the next ones would be
    // read as the wrong kinds).
    if (size == u"I" && std::u16string_view(u"diouxX").find(type) == std::u16string_view::npos) {
      continue;
    }
    switch (type) {
      case u'd': case u'i': case u'u': case u'o': case u'x': case u'X': case u'p': case u'c':
      case u'C':
        AddInt(r, c.slots, wide64);
        break;
      case u's': case u'S':
        c.slots.push_back(r.Below(6) == 0 ? 0 : strings + 64 * r.Below(8));
        break;
      case u'f': case u'e': case u'g': case u'E':
        c.slots.push_back(big_precision && r.Below(2) ? 0x4000000000000000ull : DoubleBits(r));
        break;
      default:
        break;
    }
  }
  return c;
}

void SwprintfCase(Context& h, Results& results, const Block& block, const FormatCase& c,
                  uint32_t format_address, const std::string& what) {
  const uint32_t strings = h.data, output = block.address;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, h.data, kScratchBytes);
    Fill(h.base, block.address, block.bytes);
    // Eight strings, 64 bytes each: narrow and wide text, ending in zeros.
    Random text_random;
    for (uint32_t k = 0; k < 8; k++) {
      const uint32_t at = strings + 64 * k;
      for (uint32_t b = 0; b < 56; b++) {
        guest::StoreU8(h.base, at + b, uint8_t(k % 2 == 0 ? (b % 2 == 0 ? 0 : 'A' + text_random.Below(26))
                                                         : (b < 20 ? 'a' + text_random.Below(26) : 0)));
      }
      for (uint32_t b = 56; b < 64; b++) guest::StoreU8(h.base, at + b, 0);
    }
    if (format_address == 0) {
      for (uint32_t k = 0; k < c.format.size(); k++) {
        guest::StoreU16(h.base, h.data + 512 + 2 * k, uint16_t(c.format[k]));
      }
      guest::StoreU16(h.base, h.data + 512 + 2 * c.format.size(), 0);
    }
    PPCContext t = CallContext(h);
    // The caller's parameter area and stack arguments (r1 + 32 on).
    Fill(h.base, t.r1.u32 + 24, 200);
    uint64_t* registers[6] = {&t.r5.u64, &t.r6.u64, &t.r7.u64, &t.r8.u64, &t.r9.u64, &t.r10.u64};
    for (uint32_t k = 0; k < 6; k++) *registers[k] = k < c.slots.size() ? c.slots[k] : 0x1234567800000000ull + k;
    for (uint32_t k = 6; k < c.slots.size() && k < 20; k++) {
      StoreU64(h.base, t.r1.u32 + 32 + 8 * k, c.slots[k]);
    }
    t.r3.u64 = output;
    t.r4.u64 = format_address != 0 ? format_address : h.data + 512;
    Run(v, __imp__sub_8210A490, NativeSwprintf, t, h.base);
    out[v].Bytes(Read(h.base, block.address, block.bytes));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Bytes(Read(h.base, t.r1.u32 + 24, 200));
    out[v].Word(t.r3.u32);
  }
  results.Check(out[0], out[1], what);
}

std::string Narrow(const std::u16string& s) {
  std::string n;
  for (char16_t ch : s) n += ch < 128 && ch >= 32 ? char(ch) : '?';
  return n;
}

void SwprintfCases(Context& h) {
  Results results("sub_8210A490 swprintf");
  const Block block(16384);
  // The game's formats (Strings::FormatV's and Strings::FormatGrouped's).
  struct Game {
    uint32_t format;
    bool is_double;
  };
  const Game games[] = {{0x82007EC4, false}, {0x82007EB4, false}, {0x82007EA8, false},
                        {0x82007E9C, false}, {0x82007E94, false}};
  for (const Game& g : games) {
    for (int i = 0; i < 40; i++) {
      FormatCase c;
      AddInt(h.random, c.slots, false);
      if (i < 10) c.slots.back() = h.random.Below(12);
      SwprintfCase(h, results, block, c, g.format, fmt::format("format {:08X} case {}", g.format, i));
    }
  }
  {
    // "-%s" with a wide string.
    FormatCase c;
    c.slots.push_back(h.data + 64);
    SwprintfCase(h, results, block, c, 0x82007E8C, "format 82007E8C");
  }
  const char16_t* kBuilt[] = {u"%05d", u"%.2f", u"%3d", u"%.0f", u"%.1f", u"%010d", u",%03d", u"%d"};
  for (const char16_t* f : kBuilt) {
    for (int i = 0; i < 30; i++) {
      FormatCase c;
      c.format = f;
      if (c.format.find(u'f') != std::u16string::npos) {
        c.slots.push_back(DoubleBits(h.random));
      } else {
        AddInt(h.random, c.slots, false);
      }
      SwprintfCase(h, results, block, c, 0, fmt::format("{} case {}", Narrow(c.format), i));
    }
  }
  for (int i = 0; i < 3000; i++) {
    const FormatCase c = MakeFormat(h.random, h.data);
    SwprintfCase(h, results, block, c, 0, fmt::format("\"{}\" case {}", Narrow(c.format), i));
  }
  results.Report();
}

// Strings::FormatGrouped: values small, around the thousands, the edges
// (INT_MIN among them) and random, negative too; the output empty or
// holding text (a few with 0xFFFF or too long for "-%s": the original).
void FormatGroupedCases(Context& h) {
  Results results("sub_8210A680 Strings::FormatGrouped");
  for (int i = 0; i < 1500; i++) {
    int32_t value;
    switch (h.random.Below(5)) {
      case 0: value = int32_t(h.random.Next()); break;
      case 1: {
        constexpr int32_t kEdges[] = {0, 1, -1, 999, 1000, -1000, 999999, 1000000, -999999,
                                      INT32_MIN, INT32_MAX, 1234567, -7654321};
        value = kEdges[h.random.Below(std::size(kEdges))];
        break;
      }
      default: value = int32_t(h.random.Below(4000000)) - 2000000; break;
    }
    const uint32_t prefix = h.random.Below(3) == 0 ? h.random.Below(i % 50 == 7 ? 260 : 20) : 0;
    const bool odd = h.random.Below(40) == 0;
    const Random start = h.random;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      h.random = start;
      Fill(h.base, h.data, kScratchBytes);
      const uint32_t output = h.data + 2048;
      for (uint32_t k = 0; k < prefix; k++) {
        guest::StoreU16(h.base, output + 2 * k,
                        odd && k == 0 ? uint16_t(0xFFFF) : uint16_t('a' + h.random.Below(26)));
      }
      guest::StoreU16(h.base, output + 2 * prefix, 0);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.random.Next();
      t.r4.u64 = (uint64_t(h.random.Next()) << 32) | uint32_t(value);
      t.r5.u64 = output;
      Run(v, __imp__sub_8210A680, NativeFormatGrouped, t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} ({})", i, value));
  }
  results.Report();
}

}  // namespace

namespace {

// Strings::Get: a table of 18 containers in the scratch area (0 to 6
// entries each), ids around each file's bound (the game's table at
// 0x82007E18), past the last and random; r3 and the scratch area exactly.
void StringsGets(Context& h) {
  Results results("Strings::Get (sub_8210A800)");
  Random r(0x737472676574ull);
  constexpr uint32_t kBounds = 0x82007E18;
  const uint32_t table = h.data, containers = h.data + 128;
  for (int c = 0; c < 400; c++) {
    uint32_t id;
    const uint32_t file = r.Below(18);
    const uint32_t bound = guest::LoadU32(h.base, kBounds + 4 * file);
    switch (r.Below(4)) {
      case 0: id = r.Below(0x10000); break;
      case 1: id = guest::LoadU32(h.base, kBounds + 4 * 17) + r.Below(8); break;
      default: id = bound + r.Below(8); break;
    }
    std::vector<uint32_t> counts(18);
    for (auto& n : counts) n = r.Below(7);
    const uint32_t r4_high = uint32_t(r.Next()) & 0xFFFF0000u;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i < 18; i++) {
        const uint32_t container = containers + 64 * i;
        guest::StoreU32(h.base, table + 4 * i, container);
        guest::StoreU32(h.base, container, __builtin_bswap32(counts[i]));
        for (uint32_t k = 0; k < 8; k++) {
          guest::StoreU32(h.base, container + 4 + 4 * k, __builtin_bswap32(36 + 2 * k));
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = table;
      t.r4.u64 = r4_high | id;
      if (v == 0) {
        __imp__sub_8210A800(t, h.base);
      } else if (const auto text = strings_and_font::StringsGet(h.base, table, id)) {
        t.r3.u64 = *text;
      }
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    }
    results.Check(out[0], out[1], fmt::format("case {} (id {})", c, id));
  }
  results.Report();
}

// Strings::FormatV: a table at the scratch area's +0 whose 18 files are one
// container at +128 of 8 strings (from +200, 16 units each with room): 0
// to 3 plain (letters and the control units but 0xFFD7), 4 to 7 with 0xFFD7
// too (its slot's id one of the plain strings', or now and then past the
// last bound).
// 0xFFD2 (the font's number, never in play) isn't used. The id a string's
// (by a file's bound at 0x82007E18) or now and then past the last bound
// (the table read as the text); the va_list at +3072 (16 slots: ints,
// doubles and ids in the order the string's units ask, then random); the
// output in a 32 KB block of its own (a string of doubles printed whole can
// run long), holding a short prefix. Compared: the scratch area, the
// output's block and r3.
void FormatVCases(Context& h) {
  Results results("Strings::FormatV (sub_8210A990)");
  Random r(0x666F726D617476ull);  // "formatv"
  constexpr uint32_t kBounds = 0x82007E18;
  const uint32_t table = h.data, container = h.data + 128, strings = h.data + 200;
  const Block output_block(32768);
  const uint32_t output = output_block.address, args = h.data + 3072;
  constexpr uint32_t kStringUnits = 24;
  for (int c = 0; c < 1500; c++) {
    // The strings: units, and the slot kinds each control unit asks for.
    std::vector<uint16_t> units[8];
    for (uint32_t s = 0; s < 8; s++) {
      const uint32_t length = r.Below(12);
      for (uint32_t k = 0; k < length; k++) {
        uint16_t unit;
        switch (r.Below(8)) {
          case 0: unit = uint16_t(0xFFEC + r.Below(10)); break;  // %<w>d
          case 1: unit = uint16_t(0xFFE2 + r.Below(10)); break;  // %.<p>f
          case 2: unit = uint16_t(0xFFD8 + r.Below(10)); break;  // %0<w>d
          case 3: unit = 0xFFD3; break;                          // grouped
          case 4: unit = s >= 4 ? 0xFFD7 : uint16_t('a' + r.Below(26)); break;
          case 5: {
            constexpr uint16_t kOther[] = {0xFFD1, 0xFFD6, 0xFFF6, 0xFFFF, 0xFFD4, '\n', 0x00E9};
            unit = kOther[r.Below(std::size(kOther))];
            break;
          }
          default: unit = uint16_t('A' + r.Below(26)); break;
        }
        units[s].push_back(unit);
      }
    }
    // A string's id: string `k` of a file whose bound leaves room for it
    // (Strings::Get then finds that file), or past the last bound.
    auto string_id = [&](uint32_t k) {
      for (;;) {
        const uint32_t file = r.Below(17);
        const uint32_t bound = guest::LoadU32(h.base, kBounds + 4 * file);
        const uint32_t next = guest::LoadU32(h.base, kBounds + 4 * (file + 1));
        if (int32_t(bound + k) < int32_t(next)) {
          return bound + k;
        }
      }
    };
    const uint32_t last = guest::LoadU32(h.base, kBounds + 4 * 17);
    const uint32_t pick = r.Below(8);
    const uint32_t id = r.Below(30) == 0 ? last + r.Below(4) : string_id(pick);
    // The slots, in the order the picked string asks (its inner strings
    // read on from there), then random.
    std::vector<uint64_t> slots;
    auto add = [&](uint16_t unit) {
      if (unit >= 0xFFE2 && unit <= 0xFFEB) {
        slots.push_back(r.Below(4) == 0 ? DoubleBits(r)
                                        : std::bit_cast<uint64_t>(
                                              double(int32_t(r.Below(2000000)) - 1000000) / 64.0));
      } else if (unit == 0xFFD7) {
        slots.push_back(((uint64_t(r.Next()) << 16) & ~0xFFFFull) |
                        (r.Below(16) == 0 ? last + r.Below(4) : string_id(r.Below(4))));
      } else if ((unit >= 0xFFD8 && unit <= 0xFFF5) || unit == 0xFFD3) {
        AddInt(r, slots, r.Below(4) == 0);
      }
    };
    for (uint16_t unit : units[pick]) {
      add(unit);
    }
    while (slots.size() < 16) {
      const uint32_t kind = r.Below(3);
      add(kind == 0 ? 0xFFE2 : kind == 1 ? 0xFFEC : 0xFFD7);
    }
    const uint32_t prefix = r.Below(4) == 0 ? r.Below(16) : 0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Fill(h.base, output, output_block.bytes);
      for (uint32_t i = 0; i < 18; i++) {
        guest::StoreU32(h.base, table + 4 * i, container);
      }
      guest::StoreU32(h.base, container, __builtin_bswap32(8));
      for (uint32_t s = 0; s < 8; s++) {
        const uint32_t at = strings + 2 * kStringUnits * s;
        guest::StoreU32(h.base, container + 4 + 4 * s, __builtin_bswap32(at - container));
        for (size_t k = 0; k < units[s].size(); k++) {
          guest::StoreU16(h.base, at + 2 * uint32_t(k), units[s][k]);
        }
        guest::StoreU16(h.base, at + 2 * uint32_t(units[s].size()), 0);
      }
      for (uint32_t i = 0; i < 16; i++) {
        guest::StoreU32(h.base, args + 8 * i, uint32_t(slots[i] >> 32));
        guest::StoreU32(h.base, args + 8 * i + 4, uint32_t(slots[i]));
      }
      for (uint32_t k = 0; k < prefix; k++) {
        guest::StoreU16(h.base, output + 2 * k, uint16_t('a' + k));
      }
      guest::StoreU16(h.base, output + 2 * prefix, 0);
      PPCContext t = CallContext(h);
      t.r3.u64 = table;
      t.r4.u64 = output;
      t.r5.u64 = id;
      t.r6.u64 = args;
      Run(v, __imp__sub_8210A990, hooks::StringsFormatVSteps, t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, output, output_block.bytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {} (id {}, string {})", c, id, pick));
  }
  results.Report();
}

// XPR::Release of a slot (sub_82108860): the slot empty, or an XPR (24
// bytes from operator new, made by XPR::XPR) without blocks. Compared: r3
// (the XPR's address made an offset) and the slot.
void XprSlotReleases(Context& h) {
  Results results("XPR::Release of a slot (sub_82108860)");
  if (!hooks::ModelsNativeLifetime()) {
    ITC_LOG_INFO("harness: XPR::Release of a slot: skipped (the model class isn't native)");
    return;
  }
  const uint32_t slot = h.data;
  for (int c = 0; c < 16; c++) {
    const bool set = c % 4 != 0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      uint32_t xpr = 0;
      if (set) {
        const auto block = memory::crt::OperatorNew(h.base, models::kXprBytes);
        xpr = block ? *block : 0;
        models::XprConstruct(h.base, xpr);
      }
      guest::StoreU32(h.base, slot, xpr);
      PPCContext t = CallContext(h);
      t.r3.u64 = slot;
      if (v == 0) {
        __imp__sub_82108860(t, h.base);
      } else if (const auto r3 = strings_and_font::ReleaseXprSlot(h.base, slot)) {
        t.r3.u64 = *r3;
      } else {
        __imp__sub_82108860(t, h.base);
      }
      out[v].Word(xpr != 0 && t.r3.u32 == xpr ? 0xB0000000u : t.r3.u32);
      out[v].Word(guest::LoadU32(h.base, slot));
    }
    results.Check(out[0], out[1], fmt::format("case {} ({})", c, set ? "set" : "empty"));
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
  FontLayout(h);
  FontDrawing(h);
  SwprintfCases(h);
  FormatGroupedCases(h);
  StringsGets(h);
  FormatVCases(h);
  XprSlotReleases(h);
}

}  // namespace harness
