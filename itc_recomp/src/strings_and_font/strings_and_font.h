// The string table and the font as plain C++ (docs/functions/strings-and-font.md):
// the game's string table (4,304 bytes at [0x826C5C34]) and its bitmap font
// (16,592 bytes at [0x826C5C00]). Both are guest objects shared with the
// recompiled code (the archive's loads fill them, the UI movies read them),
// so these work on guest memory, with the originals' loads and stores in
// their order. Float work is the translated code's step for step (doubles,
// std::fma where the PowerPC code fuses), flush-to-zero off (the adapters'
// job). Built with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).
// Adapters: src/hooks/strings_and_font.cpp.
//
// The string table: +0 the 17 files' pointers, +68 their loaded flags, +136
// the mask of files asked for, +4236 the 17 counts. The font: +12 the mask
// of pages asked for, +16 the draw parameters, +20 the four pages (28 bytes
// each: +24 loaded), +40 their files, +56 and +60 the start point (floats),
// +16448 and +16452 the letter and line spacing (floats).

#pragma once

#include <cstdint>
#include <optional>

namespace strings_and_font {

constexpr uint32_t kFont = 0x826C5C00;     // the font's pointer
constexpr uint32_t kStrings = 0x826C5C34;  // the string table's pointer
constexpr uint32_t kShaderEntries = 0x826C5C04;  // the four pixel shaders' sources

constexpr uint32_t kStringFiles = 17;
constexpr uint32_t kStringsBytes = 4304;

// Strings::Init (sub_8210A860): when [0x826C5C34] is 0, 4,304 bytes of pages
// (VirtualAlloc: the allocation layer's AllocPages) stored there and their
// 17 file pointers, 17 counts and mask cleared; returns them (what the
// original leaves in r3), or 0 with nothing done when the global was set.
uint32_t StringsInit(uint8_t* base);

// Strings::OnLoaded (sub_8210A850): the file's loaded flag (`flag`) set to 1.
void StringsOnLoaded(uint8_t* base, uint32_t flag);

// Strings::Release (sub_8210A538): for each file in `mask` that's loaded,
// its count down by one; at 0, the file freed (VirtualFree: the allocation
// layer's FreePages), its pointer and loaded flag cleared and its bit taken
// out of the mask. False, with nothing done, when a file it would free isn't
// the layer's pages (the original runs: its VirtualFree goes to the SDK).
bool StringsRelease(uint8_t* base, uint32_t table, uint32_t mask);

// Strings::IsLoaded (sub_8210A768): 1 when at least one file is asked for,
// each asked for is loaded, and Font::IsLoaded([0x826C5C00]); else 0.
uint32_t StringsIsLoaded(uint8_t* base, uint32_t table);

// Font::IsLoaded (sub_821081A0): 1 when at least one page is asked for and
// each asked for is loaded (its +24), else 0.
uint32_t FontIsLoaded(uint8_t* base, uint32_t font);

// Text::IsLoaded (sub_82211C90): Strings::IsLoaded([0x826C5C34]) and
// Font::IsLoaded([0x826C5C00]).
uint32_t TextIsLoaded(uint8_t* base);

// The game's swprintf (sub_8210A490): vswprintf (sub_825A9A80, merged: it
// only passes no locale on) of `format` with the arguments at `args` (the
// va_list: the caller's parameter area from r5 on, 8-byte slots) into
// `buffer`, through c_runtime::Vswprintf in `frame` (where the original's
// _woutput_l frame would be). False, with nothing written to `buffer`, where
// the original must run (c_runtime::Vswprintf's cases); else `*count` the
// characters written.
bool Swprintf(uint8_t* base, uint32_t buffer, uint32_t format, uint32_t args, uint32_t frame,
              int32_t* count);

// Strings::FormatGrouped (sub_8210A680): `value`'s magnitude with
// thousands separators appended to `out` (itself on the thousands first,
// then ",%03d" of the rest, or "%d" below 1,000, through swprintf into a
// 512-byte buffer at `frame` + 96), then for a negative value `out`
// rewritten as "-%s". `frame` is the original's r1 (its 640-byte frame;
// deeper calls below it, as the original's). False, with nothing written,
// where the original must run: a negative value whose rewrite swprintf
// wouldn't do natively (`out` holding the unit 0xFFFF) or wouldn't fit its
// buffer (the original would overrun its frame).
bool StringsFormatGrouped(uint8_t* base, int32_t value, uint32_t out, uint32_t frame);

// Strings::Get (sub_8210A800): the file i of `table` (its words) whose
// bound (0x82007E18 + 4i, signed) is at most `id` and the next above it,
// and its entry id - bound i (DataFile::GetEntry, r5 = 0: the UTF-16
// string). nullopt for an id at or past the last bound (the original
// returns with r3 unchanged).
std::optional<uint32_t> StringsGet(uint8_t* base, uint32_t table, uint32_t id);

// Strings::FormatV's calls into code that still takes the guest context:
// `function` with r3, r4 and r5 (all 64 bits of it) from a frame at `frame`
// (r1) with the return address `lr`; returns its r3.
struct FormatCalls {
  void* user;
  uint32_t (*call)(void* user, uint32_t function, uint32_t r3, uint32_t r4, uint64_t r5,
                   uint32_t frame, uint32_t lr);
};

// Strings::FormatV (sub_8210A990): string `id` of `table` (Strings::Get;
// the table itself read as the string for an id past the last bound, as
// the original's r3 left) appended to `out` a unit at a time (wcscat), the
// control units replaced: 0xFFD7 the string whose 16-bit id is in the next
// 8-byte slot of `args` (the va_list), formatted recursively with the slots
// after it; 0xFFEC-0xFFF5 the slot's int with "%<0xFFF5 - c>d", 0xFFE2-
// 0xFFEB its double, rounded to a float, with "%.<0xFFEB - c>f", 0xFFD8-
// 0xFFE1 its int with "%0<0xFFE1 - c>d" (the format made by swprintf from
// "%%%dd", "%%.%df" or "%%0%dd" first); 0xFFD3 the int with separators
// (Strings::FormatGrouped); 0xFFD2 a number through the font (sub_82108A70,
// never ran in play: a guest call). Its 4,800-byte frame below `stack` (the
// back chain written): the 4 KB buffer at +608 (cleared before each slot),
// the made format at +96, the one-unit string at +80, swprintf's argument
// at +32 (as the originals store them). swprintf and FormatGrouped natively,
// where they decline swprintf's adapter and FormatGrouped's original through
// `calls`. Returns `out`.
uint32_t StringsFormatV(uint8_t* base, const FormatCalls& calls, uint32_t stack, uint32_t table,
                        uint32_t out, uint32_t id, uint32_t args);

// Strings::Format (sub_8210ABC0): FormatV with the arguments as varargs:
// `varargs` (r6 to r10) stored in the caller's parameter area (`stack` + 40
// to + 79) and the va_list from there, kept at its frame's +80 (a 4,784-byte
// frame); 0xFFD2 isn't a case (appended as itself); 0xFFD7's strings
// through FormatV. Returns `out`.
uint32_t StringsFormat(uint8_t* base, const FormatCalls& calls, uint32_t stack, uint32_t table,
                       uint32_t out, uint32_t id, const uint64_t varargs[5]);

// sub_82108860, probably XPR::Release of a slot: the XPR at `slot` (when
// set) destroyed (XPR::~XPR), freed (operator delete) and the slot cleared.
// Returns the r3 the original leaves (the XPR, else the slot); nullopt,
// with nothing done, where the original must run (a block the layer didn't
// give, or XPR::~XPR's plain C++ declines). Only over the allocation layer
// with the memory blocks' host lock (the adapter's condition).
std::optional<uint32_t> ReleaseXprSlot(uint8_t* base, uint32_t slot);

// Strings::PageCount (sub_8210A5C0): the page breaks (0xFFD1) in `text`.
uint32_t StringsPageCount(uint8_t* base, uint32_t text);

// Strings::CopyPage (sub_8210A5E8): past the `page`-th page break in `text`,
// the characters up to the next page break or the end appended to `out`
// one at a time (wcscat of a one-character string), the first `skip` lines
// left out. Returns `out`.
uint32_t StringsCopyPage(uint8_t* base, uint32_t out, uint32_t text, int32_t page, int32_t skip);

// The static initializer sub_825D9590: the four pixel shader entries at
// 0x826C5C04 (12 bytes each: the source, its strlen, 0) from the sources at
// [0x8260333C]..[0x82603348].
void InitShaderEntries(uint8_t* base);

// Font::SetParams (sub_82108368): `params` kept at the font's +16, the start
// point (+56, +60) and the letter and line spacing (+16448, +16452).
void FontSetParams(uint8_t* base, uint32_t font, uint32_t params);

// The font's layout (font.cpp). The text drawn is UTF-16; 0xFFD1 a page
// break, '\n' a new line, 0xFFD6 and 0xFFF6-0xFFFF colors (not drawn). A
// glyph's advance is its record's +2 >> 2 (the page's +4 records, 4 bytes a
// code) times the glyph scale (the parameters' +36), truncated to 64 bits
// and kept to its low 16 (fctidz, then a halfword load), as a float.

// Font::TextWidth (sub_82108B30): Font::SetParams, the page (+36) from the
// parameters' +60, and the widest line's width less the letter spacing,
// times the scale (+28): f1.
double FontTextWidth(uint8_t* base, uint32_t font, uint32_t params, uint32_t text);

// Font::LineWidth (sub_82108458): one line's width (to '\n', a page break or
// the end); less the letter spacing and times the scale, except justified.
double FontLineWidth(uint8_t* base, uint32_t font, uint32_t text);

// sub_82108810: the glyphs drawn in a line (16-bit), for justified text.
uint32_t FontLineGlyphs(uint8_t* base, uint32_t text);

// Font::LineX (sub_82108FE0): a line's start x by the horizontal
// alignment; justified text sets the letter spacing (+16448).
double FontLineX(uint8_t* base, uint32_t font, uint32_t text);

// Font::LineY (sub_82108568): line `line` of `lines`' y by the vertical
// alignment; justified text sets the line spacing (+16452). An alignment
// above 3 returns `f1` (the register as it came).
double FontLineY(uint8_t* base, uint32_t font, uint32_t lines, uint32_t line, double f1);

// Font::CountGlyphs (sub_82108210): each drawn character's channel count
// (the four words at `counts`) up by one; returns the text's wcslen.
uint32_t FontCountGlyphs(uint8_t* base, uint32_t font, uint32_t counts, uint32_t text);

// Font::OutCode (sub_821082A0): the vertex's (x, y floats at `vertex`) bits
// against the clip rectangle (the parameters' +44..+56): x 1 left, 2 right,
// 4 inside; y 8 above, 16 below, 32 inside.
uint32_t FontOutCode(uint8_t* base, uint32_t font, uint32_t vertex);

// The font's drawing (font.cpp). The globals at 0x826C5BCC: +0 the four
// channels' glyph counts, +16 their vertex write pointers, +32 their
// vertices' starts, +48 the chunk's first slot while drawing.
constexpr uint32_t kDrawGlobals = 0x826C5BCC;

// Whether the text is rotated (the parameters' +16 angle / 360, through a
// float, not whole; a NaN is): Font::BuildVertices and Font::AddGlyph then
// call the rotation functions (sub_821097F0, sub_82108F68), which never ran
// in play and stay recompiled.
bool FontRotated(uint8_t* base, uint32_t font);

// Font::ClipGlyph (sub_82108C90): the glyph's six vertices at `vertices`
// against the clip rectangle: 1 with none or when kept (the vertices outside
// moved to its edges, their u or v with them), 0 when wholly outside.
uint32_t FontClipGlyph(uint8_t* base, uint32_t font, uint32_t vertices);

// Font::AddGlyph (sub_82109560): the glyph of `c` (its low 16 bits) as six
// vertices at its channel's write pointer, at (x, y) in `color`; clipped,
// the pointer moved on 144 bytes, or the channel's count down by one. False,
// with nothing done, for rotated text (the original runs).
bool FontAddGlyph(uint8_t* base, uint32_t font, uint32_t c, uint32_t color, double x, double y);

// Font::BuildVertices (sub_82109AA8): the text's glyphs' vertices, line by
// line. False, with nothing done, for rotated text (the original runs).
bool FontBuildVertices(uint8_t* base, uint32_t font, uint32_t text);

// Font::DrawPass (sub_82109EF0) for pass 0, up to the render queue: the
// counts, the chunk of render states, vertices and draws on the font's
// command list (+4). `kind` says what was done.
struct DrawPassResult {
  enum Kind {
    kOriginal,  // nothing done that the original won't redo: it must run
                // (rotated text, another chunk open, the list too full)
    kNothing,   // no text or no glyphs: returned before the list
    kRecorded,  // the chunk recorded; its first slot is at 0x826C5BFC
  } kind;
  uint32_t chunk;  // kRecorded: the chunk's first slot (0x826C5BFC's word)
};
DrawPassResult FontDrawPass(uint8_t* base, uint32_t font, uint32_t text);

// Font::Draw (sub_8210A400)'s set-up: Font::SetParams and the page (+36)
// from the parameters' +60.
void FontDrawSetUp(uint8_t* base, uint32_t font, uint32_t params);

// Text::Draw (sub_8210A758)'s set-up: Font::Draw's on [0x826C5C00]; returns
// the font.
uint32_t TextDrawSetUp(uint8_t* base, uint32_t params);

}  // namespace strings_and_font
