// The font's layout as plain C++: see strings_and_font.h.

#include "strings_and_font/strings_and_font.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "command_list/command_list.h"
#include "guest.h"
#include "schema/strings_and_font.h"
#include "strings_and_font/steps.h"

#include <cmath>

namespace strings_and_font {

using namespace steps;

namespace {

using G = schema::StringsAndFontGlobals;
using schema::Font;
using schema::FontDrawParams;
using schema::FontGlyphRecord;
using schema::FontGlyphTable;
using schema::FontPage;
using schema::FontText;
using schema::FontVertex;

constexpr uint32_t kPageBreak = 0xFFD1;
constexpr uint32_t kColor = 0xFFD6;  // back to the text's color
constexpr uint32_t kZero = G::zero_address();  // the float 0.0
constexpr uint32_t kHalf = G::half_address();  // the float 0.5

// The color codes (0xFFD6 and 0xFFF6-0xFFFF): not drawn, no width.
bool IsColor(uint32_t c) { return (c > 0xFFF5 && c <= 0xFFFF) || c == kColor; }

uint32_t Params(uint8_t* base, uint32_t font) { return Font(base, font).params().address; }

// The glyph record of `c` in `page`'s records.
FontGlyphRecord Record(uint8_t* base, uint32_t page, uint32_t c) {
  return FontGlyphRecord(base, FontPage(base, page).records().address + (c << 2));
}

// A 16-bit integer kept from fctidz's result, as a float.
double Low16(double value) { return ToSingle(uint16_t(Fctidz(value))); }

// A glyph's advance: its record's +2 >> 2 times the glyph scale, truncated
// to 16 bits, plus the letter spacing unless justified (alignment 3).
double Advance(uint8_t* base, uint32_t font, uint32_t params, uint32_t page, uint32_t c) {
  const bool justified = FontDrawParams(base, params).align_x() == 3;
  const double glyph_scale = FontDrawParams(base, params).glyph_scale_x();
  const uint32_t advance = uint32_t(Record(base, page, c).advance_and_channel()) >> 2;
  const double width = Low16(Single(ToSingle(advance) * glyph_scale));
  if (justified) {
    return width;
  }
  return Single(width + Font(base, font).letter_spacing());
}

}  // namespace

double FontTextWidth(uint8_t* base, uint32_t font, uint32_t params, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108B30 Font::TextWidth");
  const double zero = Lfs(base, kZero);
  double line = zero, widest = zero;
  FontSetParams(base, font, params);
  params = Params(base, font);
  const uint32_t page = guest::LoadU32(
      base, Font(base, font).pages_address() + (FontDrawParams(base, params).page() << 2));
  Font(base, font).set_page(page);
  uint32_t c = FontText(base, text).unit();
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
      c = FontText(base, text).unit();
      if (c == 0) {
        break;
      }
    }
    if (line > widest) {
      widest = line;
    }
  }
  const double spacing = Font(base, font).letter_spacing();
  return Single(Single(widest - spacing) * FontDrawParams(base, params).scale_x());
}

double FontLineWidth(uint8_t* base, uint32_t font, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108458 Font::LineWidth");
  double width = Lfs(base, kZero);
  for (uint32_t c; (c = FontText(base, text).unit()) != 0; text += 2) {
    if (c == kPageBreak || c == '\n') {
      break;
    }
    if (IsColor(c)) {
      continue;
    }
    const uint32_t params = Params(base, font);
    const uint32_t page = Font(base, font).page().address;
    width = Single(Advance(base, font, params, page, c) + width);
  }
  const FontDrawParams params(base, Params(base, font));
  if (params.align_x() == 3) {
    return width;
  }
  return Single(Single(width - Font(base, font).letter_spacing()) * params.scale_x());
}

uint32_t FontLineGlyphs(uint8_t* base, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108810 Font::LineGlyphs");
  uint32_t count = 0;
  for (uint32_t c; (c = FontText(base, text).unit()) != 0; text += 2) {
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
  const uint32_t align = FontDrawParams(base, params).align_x();
  switch (align) {
    case 0:
      return double(Font(base, font).start_x());
    case 1: {
      // start + box width x scale x 0.5 - w x 0.5, truncated.
      const int32_t box = int32_t(FontDrawParams(base, params).box_width());
      const double scale = double(FontDrawParams(base, params).scale_x());
      const double start = double(Font(base, font).start_x());
      const double half = Lfs(base, kHalf);
      const double scaled = Single(ToSingle(box) * scale);
      const double center = Single(std::fma(scaled, half, start));
      const double x = Single(-std::fma(ToSingle(w), half, -center));
      return ToSingle(Fctiwz(x));
    }
    case 2: {
      // start + box width x scale - w, truncated.
      const int32_t box = int32_t(FontDrawParams(base, params).box_width());
      const double scale = double(FontDrawParams(base, params).scale_x());
      const double start = double(Font(base, font).start_x());
      const double right = Single(std::fma(ToSingle(box), scale, start));
      return ToSingle(Fctiwz(Single(right - ToSingle(w))));
    }
    case 3: {
      const uint32_t glyphs = FontLineGlyphs(base, text);
      const double start = double(Font(base, font).start_x());
      if (glyphs > 1) {
        const int32_t box = int32_t(FontDrawParams(base, params).box_width());
        const double gaps = ToSingle(int32_t(glyphs - 1));
        const double spacing = Single(Single(ToSingle(box) - ToSingle(w)) / gaps);
        Font(base, font).set_letter_spacing(float(spacing));
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
  const uint32_t page = Font(base, font).page().address;
  const uint32_t params = Params(base, font);
  const double spacing = double(Font(base, font).line_spacing());
  const uint32_t table = FontPage(base, page).glyph_table().address;
  const double glyph_scale = double(FontDrawParams(base, params).glyph_scale_y());
  const uint32_t align = FontDrawParams(base, params).align_y();
  const double scale = double(FontDrawParams(base, params).scale_y());
  const uint32_t cell = FontGlyphTable(base, table).cell_height();
  // The line height (16 bits) and the block's height, scaled.
  const uint32_t height = uint16_t(Fctidz(Single(ToSingle(cell) * glyph_scale)));
  double block = Single(ToSingle(height) + spacing);
  block = Single(std::fma(block, ToSingle(lines), -spacing));
  block = Single(block * scale);
  switch (align) {
    case 0: {
      const double start = double(Font(base, font).start_y());
      const double step = Single(ToSingle(height) + spacing);
      return Single(std::fma(Single(step * ToSingle(line)), scale, start));
    }
    case 1: {
      const int32_t box = int32_t(FontDrawParams(base, params).box_height());
      const double s = double(FontDrawParams(base, params).scale_y());
      const double start = double(Font(base, font).start_y());
      const double line_spacing = double(Font(base, font).line_spacing());
      const double half = Lfs(base, kHalf);
      const double step = Single(ToSingle(height) + line_spacing);
      const double middle = Single(std::fma(ToSingle(box / 2), s, start));
      const double first = Single(-std::fma(block, half, -middle));
      return Single(std::fma(Single(step * ToSingle(line)), s, first));
    }
    case 2: {
      const int32_t box = int32_t(FontDrawParams(base, params).box_height());
      const double s = double(FontDrawParams(base, params).scale_y());
      const double start = double(Font(base, font).start_y());
      const double line_spacing = double(Font(base, font).line_spacing());
      const double step = Single(ToSingle(height) + line_spacing);
      const double bottom = Single(std::fma(ToSingle(box), s, start));
      const double first = Single(bottom - block);
      return Single(std::fma(Single(step * ToSingle(line)), s, first));
    }
    case 3: {
      if (lines <= 1) {
        return double(Font(base, font).start_y());
      }
      const uint32_t table_again = FontPage(base, page).glyph_table().address;
      const double glyph_scale_again = double(FontDrawParams(base, params).glyph_scale_y());
      const uint32_t box = FontDrawParams(base, params).box_height();
      const double start = double(Font(base, font).start_y());
      const uint32_t cell_again = FontGlyphTable(base, table_again).cell_height();
      const uint32_t h =
          uint16_t(Fctidz(Single(ToSingle(cell_again) * glyph_scale_again)));
      const int32_t left = int32_t(box - h * lines);
      const int32_t gaps = int32_t(lines - 1);
      const double line_spacing = ToSingle(left / gaps);
      Font(base, font).set_line_spacing(float(line_spacing));
      const double step = Single(ToSingle(height) + line_spacing);
      const double s = double(FontDrawParams(base, params).scale_y());
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
    const uint32_t c = FontText(base, text).unit();
    if (c == '\n' || c == ' ' || IsColor(c)) {
      continue;
    }
    const uint32_t page = Font(base, font).page().address;
    const uint32_t channel = (uint32_t(Record(base, page, c).advance_and_channel()) << 2) & 0xC;
    guest::StoreU32(base, counts + channel, guest::LoadU32(base, counts + channel) + 1);
  }
  return length;
}

uint32_t FontOutCode(uint8_t* base, uint32_t font, uint32_t vertex) {
  ITC_NATIVE_RUNNING("sub_821082A0 Font::OutCode");
  const uint32_t params = Params(base, font);
  uint32_t code;
  const double x = FontVertex(base, vertex).x();
  if (x < ToSingle(int32_t(FontDrawParams(base, params).clip_x()))) {
    code = 1;
  } else {
    const uint32_t p = Params(base, font);
    const int32_t right =
        int32_t(FontDrawParams(base, p).clip_width() + FontDrawParams(base, p).clip_x());
    code = x > ToSingle(right) ? 2 : 4;
  }
  const double y = FontVertex(base, vertex).y();
  if (y < ToSingle(int32_t(FontDrawParams(base, params).clip_y()))) {
    return code | 8;
  }
  const uint32_t p = Params(base, font);
  const int32_t bottom =
      int32_t(FontDrawParams(base, p).clip_height() + FontDrawParams(base, p).clip_y());
  return y > ToSingle(bottom) ? code | 16 : code | 32;
}

namespace {

// divwu and divw as the translation does them (0 for a zero divisor; divw
// also for INT_MIN / -1); the original's tw traps there only log.
uint32_t Divwu(uint32_t a, uint32_t b) { return b != 0 ? a / b : 0; }
int32_t Divw(int32_t a, int32_t b) {
  return (b != 0 && !(a == INT32_MIN && b == -1)) ? a / b : 0;
}

// An unsigned 32-bit integer through the stack and fcfid, then frsp.
double UToSingle(uint32_t value) { return ToSingle(int64_t(value)); }

constexpr uint32_t kCounts = G::glyph_counts_address();   // the four counts
constexpr uint32_t kWrite = G::write_pointers_address();   // the write pointers
constexpr uint32_t kStarts = G::vertex_starts_address();   // the vertices' starts

}  // namespace

bool FontRotated(uint8_t* base, uint32_t font) {
  const double angle = double(FontDrawParams(base, Params(base, font)).angle());
  const double turns = Single(angle * Lfd(base, G::turn_address()));
  return !(turns == ToSingle(Fctiwz(turns)));
}

uint32_t FontClipGlyph(uint8_t* base, uint32_t font, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_82108C90 Font::ClipGlyph");
  // The glyph's six vertices.
  auto vertex_at = [&](uint32_t k) {
    return FontVertex(base, schema::FontGlyphQuad(base, v).vertices_address() + 24 * k);
  };
  const uint32_t params = Params(base, font);
  const int32_t left = int32_t(FontDrawParams(base, params).clip_x());
  if (left == 0 && FontDrawParams(base, params).clip_y() == 0 &&
      FontDrawParams(base, params).clip_width() == 0 &&
      FontDrawParams(base, params).clip_height() == 0) {
    return 1;
  }
  // Wholly outside: the first vertex below the bottom, the third above the
  // top, the first right of the right edge, the second left of the left.
  auto bottom = [&] {
    const FontDrawParams p(base, Params(base, font));
    return ToSingle(int32_t(p.clip_height() + p.clip_y()));
  };
  auto right = [&] {
    const FontDrawParams p(base, Params(base, font));
    return ToSingle(int32_t(p.clip_width() + p.clip_x()));
  };
  if (double(vertex_at(0).y()) > bottom()) {
    return 0;
  }
  if (double(vertex_at(2).y()) < ToSingle(int32_t(FontDrawParams(base, params).clip_y()))) {
    return 0;
  }
  if (double(vertex_at(0).x()) > right()) {
    return 0;
  }
  if (double(vertex_at(1).x()) < ToSingle(left)) {
    return 0;
  }
  for (uint32_t k = 0; k < 6; k++) {
    const FontVertex vertex = vertex_at(k);
    const uint32_t code = FontOutCode(base, font, vertex.address) & 0xFF;
    if ((code & 4) && (code & 0x20)) {
      continue;
    }
    // x to the edge, u by the same distance / the scale / the texture's width.
    if (code & 3) {
      const double x = vertex.x();
      const double u = vertex.u();
      const double edge =
          (code & 1) ? ToSingle(int32_t(FontDrawParams(base, Params(base, font)).clip_x()))
                     : right();
      vertex.set_x(float(edge));
      const uint32_t width = Font(base, font).page().texture_width();
      const double scale = double(FontDrawParams(base, Params(base, font)).scale_x());
      double d = Single((code & 1) ? edge - x : x - edge);
      d = Single(d / scale);
      d = Single(d / UToSingle(width));
      vertex.set_u(float(Single((code & 1) ? d + u : u - d)));
    }
    if (code & 0x18) {
      const double y = vertex.y();
      const double tv = vertex.v();
      const double edge =
          (code & 8) ? ToSingle(int32_t(FontDrawParams(base, Params(base, font)).clip_y()))
                     : bottom();
      vertex.set_y(float(edge));
      const uint32_t height = Font(base, font).page().texture_height();
      const double scale = double(FontDrawParams(base, Params(base, font)).scale_y());
      double d = Single((code & 8) ? edge - y : y - edge);
      d = Single(d / scale);
      d = Single(d / UToSingle(height));
      vertex.set_v(float(Single((code & 8) ? d + tv : tv - d)));
    }
  }
  return 1;
}

bool FontAddGlyph(uint8_t* base, uint32_t font, uint32_t c, uint32_t color, double x, double y) {
  if (FontRotated(base, font)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82109560 Font::AddGlyph");
  const uint32_t page = Font(base, font).page().address;
  const FontPage page_view(base, page);
  const uint32_t texture_height = page_view.texture_height();
  const FontGlyphRecord record(base, page_view.records().address + ((c << 2) & 0x3FFFC));
  const FontGlyphTable table = page_view.glyph_table();
  const uint32_t texture_width = page_view.texture_width();
  const uint32_t params = Params(base, font);
  const uint32_t cell_width = table.cell_width();
  const uint32_t channel = (uint32_t(record.advance_and_channel()) << 2) & 0xC;
  const uint32_t cell = record.cell();
  const double glyph_scale_x = double(FontDrawParams(base, params).glyph_scale_x());
  const uint32_t cell_height = table.cell_height();
  const double glyph_scale_y = double(FontDrawParams(base, params).glyph_scale_y());
  const double scale_y = double(FontDrawParams(base, params).scale_y());
  const double scale_x = double(FontDrawParams(base, params).scale_x());
  // The cell's place in the sheet: columns = texture width / cell width.
  const uint32_t columns = Divwu(texture_width, cell_width) & 0xFFFF;
  const double size_y = Single(glyph_scale_y * scale_y);
  const double size_x = Single(glyph_scale_x * scale_x);
  const int32_t row = Divw(int32_t(cell), int32_t(columns));
  const uint32_t write = guest::LoadU32(base, kWrite + channel);
  const uint32_t column = cell - uint32_t(row) * columns;
  const uint32_t u_pixels = column * cell_width;
  const uint32_t v_pixels = uint32_t(row) * cell_height;
  const double width = UToSingle(texture_width);
  const double height = UToSingle(texture_height);
  const double cw = UToSingle(cell_width);
  const double w = Single(size_x * cw);
  const double ch = UToSingle(cell_height);
  const double h = Single(size_y * ch);
  const double u0 = Single(UToSingle(u_pixels) / width);
  const double du = Single(cw / width);
  const double v0 = Single(UToSingle(v_pixels) / height);
  const double dv = Single(ch / height);
  const double x1 = Single(w + x);
  const double y1 = Single(h + y);
  const double u1 = Single(du + u0);
  const double v1 = Single(dv + v0);
  const double z = Lfs(base, kZero);
  // The six vertices (two triangles), stored in the original's order.
  struct Store {
    uint32_t offset;
    bool word;
    double value;
  };
  const Store stores[] = {
      {16, false, u0},  {0, false, x},    {12, true, 0},    {4, false, y},    {20, false, v0},
      {8, false, z},    {36, true, 0},    {32, false, z},   {28, false, y},   {24, false, x1},
      {44, false, v0},  {40, false, u1},  {60, true, 0},    {64, false, u0},  {56, false, z},
      {48, false, x},   {52, false, y1},  {68, false, v1},  {84, true, 0},    {88, false, u0},
      {80, false, z},   {72, false, x},   {76, false, y1},  {92, false, v1},  {108, true, 0},
      {104, false, z},  {96, false, x1},  {100, false, y},  {112, false, u1}, {116, false, v0},
      {132, true, 0},   {128, false, z},  {120, false, x1}, {124, false, y1}, {136, false, u1},
      {140, false, v1},
  };
  for (const Store& store : stores) {
    if (store.word) {
      guest::StoreU32(base, write + store.offset, color);
    } else {
      Stfs(base, write + store.offset, store.value);
    }
  }
  if (FontClipGlyph(base, font, write) != 0) {
    guest::StoreU32(base, kWrite + channel, guest::LoadU32(base, kWrite + channel) + 144);
  } else {
    guest::StoreU32(base, kCounts + channel, guest::LoadU32(base, kCounts + channel) - 1);
  }
  return true;
}

bool FontBuildVertices(uint8_t* base, uint32_t font, uint32_t text) {
  if (FontRotated(base, font)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82109AA8 Font::BuildVertices");
  const uint32_t length = c_runtime::Wcslen(base, text);
  uint32_t lines = 1;
  for (uint32_t at = text, c; (c = FontText(base, at).unit()) != 0; at += 2) {
    if (c == kPageBreak) {
      break;
    }
    if (c == '\n') {
      lines = (lines + 1) & 0xFFFF;
    }
  }
  // The color: alpha (+24 x 255, the byte fctidz leaves lowest), then the
  // red, green and blue bytes (+20..+22).
  uint32_t params = Params(base, font);
  const double alpha = double(FontDrawParams(base, params).alpha());
  const uint32_t blue = FontDrawParams(base, params).blue();
  const double full = Lfs(base, G::full_alpha_address());
  const uint32_t red = FontDrawParams(base, params).red();
  const uint32_t a = uint8_t(Fctidz(Single(alpha * full)));
  const uint32_t green = FontDrawParams(base, params).green();
  const uint32_t alpha_bits = a << 8;
  uint32_t color = ((((red | alpha_bits) << 8) | green) << 8) | blue;
  uint32_t line = 0;
  double x = FontLineX(base, font, text);
  double y = FontLineY(base, font, lines, 0, x);
  // The second pass's positions, 8 bytes a character (pair - 4 the x).
  uint32_t pair = Font(base, font).second_pass_positions_address() + 4;
  for (uint32_t left = length; left != 0; left--, text += 2, pair += 8) {
    const uint32_t page = Font(base, font).page().address;
    const uint32_t c = FontText(base, text).unit();
    params = Params(base, font);
    const FontGlyphRecord record = Record(base, page, c);
    const double glyph_scale = double(FontDrawParams(base, params).glyph_scale_x());
    const uint32_t advance =
        uint16_t(Fctidz(Single(UToSingle(uint32_t(record.advance_and_channel()) >> 2) *
                               glyph_scale)));
    if (c == '\n') {
      line = (line + 1) & 0xFFFF;
      x = FontLineX(base, font, text + 2);
      y = FontLineY(base, font, lines, line, x);
      continue;
    }
    if (c == kColor) {
      const uint32_t p = Params(base, font);
      const uint32_t r = FontDrawParams(base, p).red();
      const uint32_t g = FontDrawParams(base, p).green();
      const uint32_t b = FontDrawParams(base, p).blue();
      color = ((((r | alpha_bits) << 8) | g) << 8) | b;
      continue;
    }
    if (c > 0xFFF5 && c <= 0xFFFF) {
      // A color from the font's ten (3 bytes each at +16528).
      const uint32_t k = (0xFFFF - c) & 0xFFFF;
      const uint32_t colors = Font(base, font).colors_address();
      const uint32_t r = guest::LoadU8(base, colors + 3 * k);
      const uint32_t g = guest::LoadU8(base, colors + 1 + 3 * k);
      const uint32_t b = guest::LoadU8(base, colors + 2 + 3 * k);
      color = ((((r | alpha_bits) << 8) | g) << 8) | b;
      continue;
    }
    if (c != ' ') {
      FontAddGlyph(base, font, c, color, x, y);
      if (FontDrawParams(base, Params(base, font)).second_pass() != 0) {
        // The second pass's position (never in play).
        uint32_t p = Params(base, font);
        const double sx = double(FontDrawParams(base, p).scale_x());
        const double ox = double(FontDrawParams(base, p).second_offset_x());
        Stfs(base, pair - 4, Single(std::fma(ox, sx, x)));
        p = Params(base, font);
        const double oy = double(FontDrawParams(base, p).second_offset_y());
        Stfs(base, pair, Single(std::fma(oy, double(FontDrawParams(base, p).scale_y()), y)));
      }
      params = Params(base, font);
    }
    // x moves on by (the letter spacing + the advance) x the scale.
    const double step = Single(double(Font(base, font).letter_spacing()) + ToSingle(advance));
    x = Single(std::fma(step, double(FontDrawParams(base, params).scale_x()), x));
  }
  return true;
}

DrawPassResult FontDrawPass(uint8_t* base, uint32_t font, uint32_t text) {
  if (text == 0) {
    return {DrawPassResult::kNothing, 0};
  }
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, kCounts + 4 * i, 0);
  }
  if (FontCountGlyphs(base, font, kCounts, text) == 0) {
    return {DrawPassResult::kNothing, 0};
  }
  const uint32_t list = Font(base, font).command_list<schema::View>().address;
  // What the original would do differently: rotated text (sub_821097F0),
  // or its command list calls printing a message (another chunk open, the
  // list full: the slots counted at most, as clipping only lowers the
  // counts). The counts are redone by the original, the same.
  if (FontRotated(base, font)) {
    return {DrawPassResult::kOriginal, 0};
  }
  if (guest::LoadU8(base, list + 52) == 0 &&
      guest::LoadU32(base, command_list::kOpenChunks) != 0) {
    return {DrawPassResult::kOriginal, 0};
  }
  uint64_t slots = 19 + 3;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t vertices = guest::LoadU32(base, kCounts + 4 * i) * 6;
    if (vertices != 0) {
      slots += 1 + ((vertices * 24 + 15) >> 4) + 4;
    }
  }
  if (uint64_t(guest::LoadU32(base, list + 12)) + slots >= guest::LoadU32(base, list + 8)) {
    return {DrawPassResult::kOriginal, 0};
  }
  ITC_NATIVE_RUNNING("sub_82109EF0 Font::DrawPass");
  command_list::OpenChunk(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  // Each command a slot of {command, count, 0} and its payload's slots.
  auto command = [&](uint32_t id, uint32_t count) {
    const uint32_t at = slot();
    guest::StoreU32(base, at + 0, id);
    guest::StoreU32(base, at + 4, count);
    guest::StoreU32(base, at + 8, 0);
  };
  auto pair = [&](uint32_t a, uint32_t b) {
    const uint32_t at = slot();
    guest::StoreU32(base, at + 0, a);
    guest::StoreU32(base, at + 4, b);
  };
  const uint32_t chunk = slot();
  G::set_chunk(base, chunk);
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  command(4, 1);  // SetTexture: sampler 0, the page's texture
  {
    const uint32_t at = slot();
    guest::StoreU32(base, at + 0, 0);
    guest::StoreU32(base, at + 4, Font(base, font).page().texture().address);
  }
  command(1, 5);  // SetRenderState x 5
  pair(60, 1);    // blend enable
  pair(72, 6);    // source blend
  pair(76, 7);    // destination blend
  pair(304, 0);   // viewport transform off
  pair(56, 0);    // cull mode
  const uint32_t params = Params(base, font);
  if (FontDrawParams(base, params).clip_x() == 0 && FontDrawParams(base, params).clip_y() == 0 &&
      FontDrawParams(base, params).clip_width() == 0 &&
      FontDrawParams(base, params).clip_height() == 0) {
    command(1, 1);
    pair(200, 0);  // scissor off
  } else {
    command(31, 1);  // SetScissorRect: the clip rectangle as halfwords
    const uint32_t at = slot();
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU16(base, at + 2 * k,
                      uint16_t(guest::LoadU32(
                          base,
                          FontDrawParams(base, Params(base, font)).clip_x_address() + 4 * k)));
    }
  }
  {
    const uint32_t at = slot();  // SetSamplerState x 3 on sampler 0
    guest::StoreU32(base, at + 8, 0);
    guest::StoreU32(base, at + 4, 3);
    guest::StoreU32(base, at + 0, 2);
  }
  auto triple = [&](uint32_t a, uint32_t b, uint32_t c) {
    const uint32_t at = slot();
    guest::StoreU32(base, at + 0, a);
    guest::StoreU32(base, at + 4, b);
    guest::StoreU32(base, at + 8, c);
  };
  triple(0, 24, 2);  // mip filter 2
  triple(0, 20, 1);  // min filter 1
  triple(0, 16, 1);  // mag filter 1
  command(10, 1);    // SetVertexDeclaration
  guest::StoreU32(base, slot() + 0, Font(base, font).vertex_declaration<schema::View>().address);
  command(8, 1);     // SetVertexShader
  guest::StoreU32(base, slot() + 0, Font(base, font).vertex_shader<schema::View>().address);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t vertices = guest::LoadU32(base, kCounts + 4 * i) * 6;
    uint32_t start = 0;
    if (vertices != 0) {
      const uint32_t at = slot();
      guest::StoreU32(base, at + 8, 0);
      guest::StoreU32(base, at + 4, vertices * 24);
      guest::StoreU32(base, at + 0, 21);
      start = command_list::AllocateBytes(base, list, vertices * 24, false);
    }
    guest::StoreU32(base, kWrite + 4 * i, start);
    guest::StoreU32(base, kStarts + 4 * i, start);
  }
  FontBuildVertices(base, font, text);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t vertices = guest::LoadU32(base, kCounts + 4 * i) * 6;
    if (vertices == 0) {
      continue;
    }
    command(9, 1);  // SetPixelShader: the channel's
    guest::StoreU32(base, slot() + 0,
                    guest::LoadU32(base, Font(base, font).pixel_shaders_address() + 4 * i));
    command(19, 1);  // DrawPrimitiveUP: a triangle list, 24-byte vertices
    const uint32_t at = slot();
    guest::StoreU32(base, at + 0, 4);
    guest::StoreU16(base, at + 4, uint16_t(vertices));
    guest::StoreU16(base, at + 6, 24);
    guest::StoreU32(base, at + 8, guest::LoadU32(base, kStarts + 4 * i));
  }
  command(1, 1);
  pair(304, 1);   // viewport transform back on
  command(36, 0);
  command_list::CloseChunk(base, list);
  return {DrawPassResult::kRecorded, G::chunk(base)};
}

void FontDrawSetUp(uint8_t* base, uint32_t font, uint32_t params) {
  ITC_NATIVE_RUNNING("sub_8210A400 Font::Draw");
  FontSetParams(base, font, params);
  const uint32_t page = FontDrawParams(base, Params(base, font)).page();
  Font(base, font).set_page(guest::LoadU32(base, Font(base, font).pages_address() + (page << 2)));
}

uint32_t TextDrawSetUp(uint8_t* base, uint32_t params) {
  ITC_NATIVE_RUNNING("sub_8210A758 Text::Draw");
  const uint32_t font = G::font(base).address;
  FontDrawSetUp(base, font, params);
  return font;
}

}  // namespace strings_and_font
