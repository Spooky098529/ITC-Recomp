// The font's layout as plain C++: see strings_and_font.h.

#include "strings_and_font/strings_and_font.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "strings_and_font/steps.h"

#include <cmath>

namespace strings_and_font {

using namespace steps;

namespace {

constexpr uint32_t kPageBreak = 0xFFD1;
constexpr uint32_t kColor = 0xFFD6;  // back to the text's color
constexpr uint32_t kZero = 0x82000A34;  // the float 0.0
constexpr uint32_t kHalf = 0x82001BE4;  // the float 0.5

// The color codes (0xFFD6 and 0xFFF6-0xFFFF): not drawn, no width.
bool IsColor(uint32_t c) { return (c > 0xFFF5 && c <= 0xFFFF) || c == kColor; }

uint32_t Params(uint8_t* base, uint32_t font) { return guest::LoadU32(base, font + 16); }

// A 16-bit integer kept from fctidz's result, as a float.
double Low16(double value) { return ToSingle(uint16_t(Fctidz(value))); }

// A glyph's advance: its record's +2 >> 2 times the glyph scale, truncated
// to 16 bits, plus the letter spacing unless justified (alignment 3).
double Advance(uint8_t* base, uint32_t font, uint32_t params, uint32_t page, uint32_t c) {
  const bool justified = guest::LoadU32(base, params + 76) == 3;
  const double glyph_scale = Lfs(base, params + 36);
  const uint32_t record = guest::LoadU32(base, page + 4) + (c << 2);
  const uint32_t advance = uint32_t(guest::LoadU16(base, record + 2)) >> 2;
  const double width = Low16(Single(ToSingle(advance) * glyph_scale));
  if (justified) {
    return width;
  }
  return Single(width + Lfs(base, font + 16448));
}

}  // namespace

double FontTextWidth(uint8_t* base, uint32_t font, uint32_t params, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108B30 Font::TextWidth");
  const double zero = Lfs(base, kZero);
  double line = zero, widest = zero;
  FontSetParams(base, font, params);
  params = Params(base, font);
  const uint32_t page =
      guest::LoadU32(base, font + ((guest::LoadU32(base, params + 60) + 5) << 2));
  guest::StoreU32(base, font + 36, page);
  uint32_t c = guest::LoadU16(base, text);
  if (c != 0) {
    for (;;) {
      if (c == kPageBreak) {
        break;
      }
      if (c == '\n') {
        if (line > widest) {
          widest = line;
        }
        line = zero;
        // The new line's own advance is added to the next line.
        line = Single(Advance(base, font, params, page, c) + line);
      } else if (!IsColor(c)) {
        line = Single(Advance(base, font, params, page, c) + line);
      }
      text += 2;
      c = guest::LoadU16(base, text);
      if (c == 0) {
        break;
      }
    }
    if (line > widest) {
      widest = line;
    }
  }
  const double spacing = Lfs(base, font + 16448);
  return Single(Single(widest - spacing) * Lfs(base, params + 28));
}

double FontLineWidth(uint8_t* base, uint32_t font, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108458 Font::LineWidth");
  double width = Lfs(base, kZero);
  for (uint32_t c; (c = guest::LoadU16(base, text)) != 0; text += 2) {
    if (c == kPageBreak || c == '\n') {
      break;
    }
    if (IsColor(c)) {
      continue;
    }
    const uint32_t params = Params(base, font);
    const uint32_t page = guest::LoadU32(base, font + 36);
    width = Single(Advance(base, font, params, page, c) + width);
  }
  const uint32_t params = Params(base, font);
  if (guest::LoadU32(base, params + 76) == 3) {
    return width;
  }
  return Single(Single(width - Lfs(base, font + 16448)) * Lfs(base, params + 28));
}

uint32_t FontLineGlyphs(uint8_t* base, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108810 Font::LineGlyphs");
  uint32_t count = 0;
  for (uint32_t c; (c = guest::LoadU16(base, text)) != 0; text += 2) {
    if (c == kPageBreak || c == '\n') {
      break;
    }
    if (!IsColor(c)) {
      count = (count + 1) & 0xFFFF;
    }
  }
  return count;
}

double FontLineX(uint8_t* base, uint32_t font, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108FE0 Font::LineX");
  const double width = FontLineWidth(base, font, text);
  const uint32_t params = Params(base, font);
  const int32_t w = Fctiwz(width);
  const uint32_t align = guest::LoadU32(base, params + 76);
  switch (align) {
    case 0:
      return Lfs(base, font + 56);
    case 1: {
      // start + box width x scale x 0.5 - w x 0.5, truncated.
      const int32_t box = int32_t(guest::LoadU32(base, params + 68));
      const double scale = Lfs(base, params + 28);
      const double start = Lfs(base, font + 56);
      const double half = Lfs(base, kHalf);
      const double scaled = Single(ToSingle(box) * scale);
      const double center = Single(std::fma(scaled, half, start));
      const double x = Single(-std::fma(ToSingle(w), half, -center));
      return ToSingle(Fctiwz(x));
    }
    case 2: {
      // start + box width x scale - w, truncated.
      const int32_t box = int32_t(guest::LoadU32(base, params + 68));
      const double scale = Lfs(base, params + 28);
      const double start = Lfs(base, font + 56);
      const double right = Single(std::fma(ToSingle(box), scale, start));
      return ToSingle(Fctiwz(Single(right - ToSingle(w))));
    }
    case 3: {
      const uint32_t glyphs = FontLineGlyphs(base, text);
      const double start = Lfs(base, font + 56);
      if (glyphs > 1) {
        const int32_t box = int32_t(guest::LoadU32(base, params + 68));
        const double gaps = ToSingle(int32_t(glyphs - 1));
        const double spacing = Single(Single(ToSingle(box) - ToSingle(w)) / gaps);
        Stfs(base, font + 16448, spacing);
      }
      return start;
    }
    default:
      return width;
  }
}

double FontLineY(uint8_t* base, uint32_t font, uint32_t lines, uint32_t line, double f1) {
  ITC_NATIVE_RUNNING("sub_82108568 Font::LineY");
  lines &= 0xFFFF;
  line &= 0xFFFF;
  const uint32_t page = guest::LoadU32(base, font + 36);
  const uint32_t params = Params(base, font);
  const double spacing = Lfs(base, font + 16452);
  const uint32_t table = guest::LoadU32(base, page + 0);
  const double glyph_scale = Lfs(base, params + 40);
  const uint32_t align = guest::LoadU32(base, params + 80);
  const double scale = Lfs(base, params + 32);
  const uint32_t cell = guest::LoadU16(base, table + 2);
  // The line height (16 bits) and the block's height, scaled.
  const uint32_t height = uint16_t(Fctidz(Single(ToSingle(cell) * glyph_scale)));
  double block = Single(ToSingle(height) + spacing);
  block = Single(std::fma(block, ToSingle(lines), -spacing));
  block = Single(block * scale);
  switch (align) {
    case 0: {
      const double start = Lfs(base, font + 60);
      const double step = Single(ToSingle(height) + spacing);
      return Single(std::fma(Single(step * ToSingle(line)), scale, start));
    }
    case 1: {
      const int32_t box = int32_t(guest::LoadU32(base, params + 72));
      const double s = Lfs(base, params + 32);
      const double start = Lfs(base, font + 60);
      const double line_spacing = Lfs(base, font + 16452);
      const double half = Lfs(base, kHalf);
      const double step = Single(ToSingle(height) + line_spacing);
      const double middle = Single(std::fma(ToSingle(box / 2), s, start));
      const double first = Single(-std::fma(block, half, -middle));
      return Single(std::fma(Single(step * ToSingle(line)), s, first));
    }
    case 2: {
      const int32_t box = int32_t(guest::LoadU32(base, params + 72));
      const double s = Lfs(base, params + 32);
      const double start = Lfs(base, font + 60);
      const double line_spacing = Lfs(base, font + 16452);
      const double step = Single(ToSingle(height) + line_spacing);
      const double bottom = Single(std::fma(ToSingle(box), s, start));
      const double first = Single(bottom - block);
      return Single(std::fma(Single(step * ToSingle(line)), s, first));
    }
    case 3: {
      if (lines <= 1) {
        return Lfs(base, font + 60);
      }
      const uint32_t table_again = guest::LoadU32(base, page + 0);
      const double glyph_scale_again = Lfs(base, params + 40);
      const uint32_t box = guest::LoadU32(base, params + 72);
      const double start = Lfs(base, font + 60);
      const uint32_t cell_again = guest::LoadU16(base, table_again + 2);
      const uint32_t h =
          uint16_t(Fctidz(Single(ToSingle(cell_again) * glyph_scale_again)));
      const int32_t left = int32_t(box - h * lines);
      const int32_t gaps = int32_t(lines - 1);
      const double line_spacing = ToSingle(left / gaps);
      Stfs(base, font + 16452, line_spacing);
      const double step = Single(ToSingle(height) + line_spacing);
      const double s = Lfs(base, params + 32);
      return Single(std::fma(Single(step * ToSingle(line)), s, start));
    }
    default:
      return f1;
  }
}

uint32_t FontCountGlyphs(uint8_t* base, uint32_t font, uint32_t counts, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108210 Font::CountGlyphs");
  const uint32_t length = c_runtime::Wcslen(base, text);
  for (uint32_t left = length; left != 0; left--, text += 2) {
    const uint32_t c = guest::LoadU16(base, text);
    if (c == '\n' || c == ' ' || IsColor(c)) {
      continue;
    }
    const uint32_t page = guest::LoadU32(base, font + 36);
    const uint32_t record = guest::LoadU32(base, page + 4) + (c << 2);
    const uint32_t channel = (uint32_t(guest::LoadU16(base, record + 2)) << 2) & 0xC;
    guest::StoreU32(base, counts + channel, guest::LoadU32(base, counts + channel) + 1);
  }
  return length;
}

uint32_t FontOutCode(uint8_t* base, uint32_t font, uint32_t vertex) {
  ITC_NATIVE_RUNNING("sub_821082A0 Font::OutCode");
  const uint32_t params = Params(base, font);
  uint32_t code;
  const double x = Lfs(base, vertex + 0);
  if (x < ToSingle(int32_t(guest::LoadU32(base, params + 44)))) {
    code = 1;
  } else {
    const uint32_t p = Params(base, font);
    const int32_t right = int32_t(guest::LoadU32(base, p + 52) + guest::LoadU32(base, p + 44));
    code = x > ToSingle(right) ? 2 : 4;
  }
  const double y = Lfs(base, vertex + 4);
  if (y < ToSingle(int32_t(guest::LoadU32(base, params + 48)))) {
    return code | 8;
  }
  const uint32_t p = Params(base, font);
  const int32_t bottom = int32_t(guest::LoadU32(base, p + 56) + guest::LoadU32(base, p + 48));
  return y > ToSingle(bottom) ? code | 16 : code | 32;
}

}  // namespace strings_and_font
