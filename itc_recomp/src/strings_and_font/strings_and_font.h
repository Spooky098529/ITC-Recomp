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

}  // namespace strings_and_font
