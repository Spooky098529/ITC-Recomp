// The string table and the font natively: adapters in front of
// src/strings_and_font/strings_and_font.cpp (docs/functions/strings-and-font.md).
// Strings::OnLoaded is reached through Archive::Load's function pointer and
// sub_825D9590 through the C runtime's start-up table, so their adapters
// stay. Strings::IsLoaded (sub_8210A768) and Font::IsLoaded (sub_821081A0)
// have no adapters: only Text::IsLoaded's plain C++ calls them in play (a
// recompiled caller reaching them runs the original, and the flag switches
// Text::IsLoaded). Font::LineWidth (sub_82108458) and the justified line's
// glyph count (sub_82108810) have none either: only Font::LineX's plain C++
// calls them. The registers the originals leave that a caller could read
// (r3) are left the same.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "strings_and_font/strings_and_font.h"

ITC_FLAG_BOOL(native_strings_and_font, true, "ITC",
              "Run the string table and the font (docs/functions/strings-and-font.md: "
              "Strings::Init sub_8210A860 (with --native_heap), Strings::OnLoaded sub_8210A850, "
              "Strings::Release sub_8210A538 (with --native_heap), Text::IsLoaded "
              "sub_82211C90 with Strings::IsLoaded sub_8210A768 and Font::IsLoaded "
              "sub_821081A0, Strings::PageCount sub_8210A5C0, Strings::CopyPage sub_8210A5E8, "
              "the font's pixel shader entries sub_825D9590, Font::SetParams sub_82108368, "
              "Font::TextWidth sub_82108B30, Font::LineX sub_82108FE0 with Font::LineWidth "
              "sub_82108458 and sub_82108810, Font::LineY sub_82108568, Font::CountGlyphs "
              "sub_82108210 and Font::OutCode sub_821082A0) "
              "natively instead of through the translated code (same results; checked by "
              "--run_harnesses=strings_and_font)");

using namespace strings_and_font;

#define ITC_STRINGS_AND_FONT_FALLBACK(address) \
  REX_FUNC_PROLOGUE();                         \
  if (!ITC_FLAG(native_strings_and_font)) {    \
    __imp__sub_##address(ctx, base);           \
    return;                                    \
  }

// Strings::Init: no arguments; r3 the table when it allocates it, else left.
// Natively only while --native_heap routes VirtualAlloc to the allocation
// layer.
extern "C" REX_FUNC(sub_8210A860) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_strings_and_font) || !hooks::NativeHeapActive()) {
    __imp__sub_8210A860(ctx, base);
    return;
  }
  const uint32_t table = StringsInit(base);
  if (table != 0) {
    ctx.r3.u64 = table;
  }
}

// Strings::OnLoaded: r6 = the file's loaded flag.
extern "C" REX_FUNC(sub_8210A850) {
  ITC_STRINGS_AND_FONT_FALLBACK(8210A850)
  StringsOnLoaded(base, ctx.r6.u32);
}

// Strings::Release: r3 = the table, r4 = the mask. Natively only while
// --native_heap routes VirtualFree to the allocation layer; the original
// when a file it would free isn't the layer's. No caller reads r3 after it
// (each calls sub_8211E370, which takes no arguments).
extern "C" REX_FUNC(sub_8210A538) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_strings_and_font) || !hooks::NativeHeapActive() ||
      !StringsRelease(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_8210A538(ctx, base);
  }
}

// Text::IsLoaded: no arguments; r3 0 or 1.
extern "C" REX_FUNC(sub_82211C90) {
  ITC_STRINGS_AND_FONT_FALLBACK(82211C90)
  ctx.r3.u64 = TextIsLoaded(base);
}

// Strings::PageCount: r4 = the text; r3 the count.
extern "C" REX_FUNC(sub_8210A5C0) {
  ITC_STRINGS_AND_FONT_FALLBACK(8210A5C0)
  ctx.r3.u64 = StringsPageCount(base, ctx.r4.u32);
}

// Strings::CopyPage: r4 = the output, r5 = the text, r6 = the page, r7 =
// the lines to skip; r3 the output.
extern "C" REX_FUNC(sub_8210A5E8) {
  ITC_STRINGS_AND_FONT_FALLBACK(8210A5E8)
  ctx.r3.u64 = StringsCopyPage(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32, ctx.r7.s32);
}

// The font's pixel shader entries: no arguments, from the C runtime's
// start-up table.
extern "C" REX_FUNC(sub_825D9590) {
  ITC_STRINGS_AND_FONT_FALLBACK(825D9590)
  InitShaderEntries(base);
}

// Font::SetParams: r3 = the font, r4 = the draw parameters. Flush-to-zero
// off first, as its lfs sets it.
extern "C" REX_FUNC(sub_82108368) {
  ITC_STRINGS_AND_FONT_FALLBACK(82108368)
  ctx.fpscr.disableFlushMode();
  FontSetParams(base, ctx.r3.u32, ctx.r4.u32);
}

// Font::TextWidth: r3 = the font, r4 = the draw parameters, r5 = the text;
// f1 the width.
extern "C" REX_FUNC(sub_82108B30) {
  ITC_STRINGS_AND_FONT_FALLBACK(82108B30)
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = FontTextWidth(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// Font::LineX: r3 = the font, r4 = the line; f1 its x. (The original's r3
// after justified text is the glyph count; no caller reads it.)
extern "C" REX_FUNC(sub_82108FE0) {
  ITC_STRINGS_AND_FONT_FALLBACK(82108FE0)
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = FontLineX(base, ctx.r3.u32, ctx.r4.u32);
}

// Font::LineY: r3 = the font, r4 = the line count, r5 = the line; f1 its y
// (as it came for an alignment above 3).
extern "C" REX_FUNC(sub_82108568) {
  ITC_STRINGS_AND_FONT_FALLBACK(82108568)
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = FontLineY(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64);
}

// Font::CountGlyphs: r3 = the font, r4 = the four counts, r5 = the text; r3
// the text's length.
extern "C" REX_FUNC(sub_82108210) {
  ITC_STRINGS_AND_FONT_FALLBACK(82108210)
  ctx.r3.u64 = FontCountGlyphs(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// Font::OutCode: r3 = the font, r4 = the vertex; r3 the bits.
extern "C" REX_FUNC(sub_821082A0) {
  ITC_STRINGS_AND_FONT_FALLBACK(821082A0)
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = FontOutCode(base, ctx.r3.u32, ctx.r4.u32);
}
