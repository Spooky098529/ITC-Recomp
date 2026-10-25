// The string table and the font natively: adapters in front of
// src/strings_and_font/strings_and_font.cpp (docs/functions/strings-and-font.md).
// Strings::OnLoaded is reached through Archive::Load's function pointer and
// sub_825D9590 through the C runtime's start-up table, so their adapters
// stay. Strings::IsLoaded (sub_8210A768) and Font::IsLoaded (sub_821081A0)
// have no adapters: only Text::IsLoaded's plain C++ calls them in play (a
// recompiled caller reaching them runs the original, and the flag switches
// Text::IsLoaded). Font::LineWidth (sub_82108458) and the justified line's
// glyph count (sub_82108810) have none either: only Font::LineX's plain C++
// calls them. Nor have Font::SetParams (sub_82108368), Font::LineX
// (sub_82108FE0), Font::LineY (sub_82108568), Font::CountGlyphs
// (sub_82108210), Font::BuildVertices (sub_82109AA8), Font::AddGlyph
// (sub_82109560), Font::ClipGlyph (sub_82108C90) and Font::OutCode
// (sub_821082A0) any more: only their native callers' plain C++ calls them in
// play (Font::TextWidth's, Font::Draw's and Font::DrawPass's down), and the
// flag switches those callers. Font::DrawPass's and Font::Draw's adapters
// stay: they make the render queue's guest call. The registers the
// originals leave that a caller could read (r3) are left the same.

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/strings_and_font.h"
#include "strings_and_font/strings_and_font.h"

ITC_FLAG_BOOL(native_strings_and_font, true, "ITC",
              "Run the string table and the font (docs/functions/strings-and-font.md: "
              "Strings::Init sub_8210A860 (with --native_heap), Strings::OnLoaded sub_8210A850, "
              "Strings::Release sub_8210A538 (with --native_heap), Text::IsLoaded "
              "sub_82211C90 with Strings::IsLoaded sub_8210A768 and Font::IsLoaded "
              "sub_821081A0, swprintf sub_8210A490 (with vswprintf sub_825A9A80, _vswprintf_l "
              "sub_825A9980 and _woutput_l sub_825B13D0, no adapters of their own; the original "
              "for %n, %Z, floats but %f and the unit 0xFFFF), Strings::FormatGrouped "
              "sub_8210A680, Strings::PageCount sub_8210A5C0, Strings::CopyPage sub_8210A5E8, "
              "the font's pixel shader entries sub_825D9590, Font::TextWidth sub_82108B30 "
              "with Font::SetParams sub_82108368, Font::DrawPass sub_82109EF0 (without the "
              "command list's diagnostics; with Font::CountGlyphs sub_82108210, "
              "Font::BuildVertices sub_82109AA8, Font::LineX sub_82108FE0 with Font::LineWidth "
              "sub_82108458 and sub_82108810, Font::LineY sub_82108568, Font::AddGlyph "
              "sub_82109560, Font::ClipGlyph sub_82108C90 and Font::OutCode sub_821082A0, no "
              "adapters of their own), Font::Draw sub_8210A400 and Text::Draw sub_8210A758; "
              "rotated text and the second pass, never in play, on the translated code) "
              "natively instead of through the translated code (same results; checked by "
              "--run_harnesses=strings_and_font)");

using namespace strings_and_font;

hooks::GuestStandIn hooks::g_text_draw_stand_in = nullptr;

// The command list's diagnostics (blend_state.cpp) see the slots the
// originals allocate through AllocateSlots: with any of them on, the
// originals run.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

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

// swprintf: r3 = the buffer, r4 = the format, r5 to r10 and the stack the
// arguments: stored into the caller's parameter area (r1 + 32 to + 72) as
// the original does, the va_list from there; _woutput_l's frame where the
// original's would be (96, 144 and 2,336 bytes down). r3 the count
// (zero-extended; the original's can carry -1's upper bits).
extern "C" REX_FUNC(sub_8210A490) {
  ITC_STRINGS_AND_FONT_FALLBACK(8210A490)
  const uint64_t registers[6] = {ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                                 ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, ctx.r1.u32 + 32 + 8 * i, uint32_t(registers[i] >> 32));
    guest::StoreU32(base, ctx.r1.u32 + 36 + 8 * i, uint32_t(registers[i]));
  }
  int32_t count;
  if (!Swprintf(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 + 32, ctx.r1.u32 - 96 - 144 - 2336,
                &count)) {
    __imp__sub_8210A490(ctx, base);
    return;
  }
  ctx.r3.u64 = uint32_t(count);
}

// Strings::FormatGrouped: r4 = the value, r5 = the output; r3 the output
// (wcscat's or wcscpy's), its frame where the original's would be (640
// bytes down). Flush-to-zero off, as its lfd leaves it.
extern "C" REX_FUNC(sub_8210A680) {
  ITC_STRINGS_AND_FONT_FALLBACK(8210A680)
  ctx.fpscr.disableFlushMode();
  if (!StringsFormatGrouped(base, ctx.r4.s32, ctx.r5.u32, ctx.r1.u32 - 640)) {
    __imp__sub_8210A680(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r5.u32;
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

// Font::TextWidth: r3 = the font, r4 = the draw parameters, r5 = the text;
// f1 the width.
extern "C" REX_FUNC(sub_82108B30) {
  ITC_STRINGS_AND_FONT_FALLBACK(82108B30)
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = FontTextWidth(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

namespace {

bool SlotDiagnostics() {
  return ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
         ITC_FLAG(log_recording_by_pass);
}

// Font::DrawPass for pass 0 from a frame like the original's (192 bytes):
// the plain C++ up to the render queue, then the chunk handed to it (the
// font's +8, its vtable slot 4: r4 = the chunk, r5 = the parameters' +116
// + 1, f1 = 0.0) and 0x826C5BFC cleared, as the original. False, with
// nothing done that the original won't redo, where it must run.
bool DrawPass(PPCContext& ctx, uint8_t* base, uint32_t font, uint32_t text) {
  const DrawPassResult result = FontDrawPass(base, font, text);
  if (result.kind == DrawPassResult::kOriginal) {
    return false;
  }
  if (result.kind == DrawPassResult::kNothing) {
    return true;
  }
  if (result.chunk != 0) {
    guest_call::InFrame(ctx, base, 192, 0x8210A3F0, [&](uint32_t) {
      const uint32_t params = guest::LoadU32(base, font + 16);
      const uint32_t queue = guest::LoadU32(base, font + 8);
      ctx.fpscr.disableFlushMode();
      ctx.f1.f64 = double(guest::LoadFloat(base, 0x82000A34));
      ctx.r3.u64 = queue;
      ctx.r4.u64 = result.chunk;
      ctx.r5.s64 = int64_t(int32_t(guest::LoadU32(base, params + 116))) + 1;
      REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, queue) + 16));
    });
  }
  guest::StoreU32(base, kDrawGlobals + 48, 0);
  return true;
}

// Font::Draw on `font` (`set_up` its set-up: Font::Draw's or Text::Draw's
// plain C++), then pass 0 (the original DrawPass
// where it must run), from a frame like the original's (112 bytes). The
// whole original where the second pass is on (never in play).
template <typename SetUp>
void FontDraw(PPCContext& ctx, uint8_t* base, SetUp set_up) {
  const uint32_t params = ctx.r4.u32, text = ctx.r5.u32;
  ctx.fpscr.disableFlushMode();
  const uint32_t font = set_up(params);
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 112, 0x8210A444, [&](uint32_t) {
    if (!DrawPass(ctx, base, font, text)) {
      ctx.r3.u64 = font;
      ctx.r4.u64 = text;
      ctx.r5.u64 = 0;
      __imp__sub_82109EF0(ctx, base);
    }
  });
  ctx.lr = lr;
}

}  // namespace

// Font::DrawPass: r3 = the font, r4 = the text, r5 = the pass. The original
// for pass 1 (never in play), with the command list's diagnostics on, and
// where FontDrawPass says it must run.
extern "C" REX_FUNC(sub_82109EF0) {
  ITC_STRINGS_AND_FONT_FALLBACK(82109EF0)
  if (ctx.r5.u32 != 0 || SlotDiagnostics()) {
    __imp__sub_82109EF0(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  if (!DrawPass(ctx, base, ctx.r3.u32, ctx.r4.u32)) {
    ctx.lr = lr;
    __imp__sub_82109EF0(ctx, base);
    return;
  }
  ctx.lr = lr;
}

// Font::Draw: r3 = the font, r4 = the draw parameters, r5 = the text.
extern "C" REX_FUNC(sub_8210A400) {
  ITC_STRINGS_AND_FONT_FALLBACK(8210A400)
  if (guest::LoadU32(base, ctx.r4.u32 + 112) != 0 || SlotDiagnostics()) {
    __imp__sub_8210A400(ctx, base);
    return;
  }
  const uint32_t font = ctx.r3.u32;
  FontDraw(ctx, base, [&](uint32_t params) {
    FontDrawSetUp(base, font, params);
    return font;
  });
}

// Text::Draw: r4 = the draw parameters, r5 = the text: Font::Draw on
// [0x826C5C00].
extern "C" REX_FUNC(sub_8210A758) {
  REX_FUNC_PROLOGUE();
  if (hooks::g_text_draw_stand_in != nullptr) {
    hooks::g_text_draw_stand_in(ctx, base);
    return;
  }
  if (!ITC_FLAG(native_strings_and_font)) {
    __imp__sub_8210A758(ctx, base);
    return;
  }
  if (guest::LoadU32(base, ctx.r4.u32 + 112) != 0 || SlotDiagnostics()) {
    __imp__sub_8210A758(ctx, base);
    return;
  }
  FontDraw(ctx, base, [&](uint32_t params) { return TextDrawSetUp(base, params); });
}
