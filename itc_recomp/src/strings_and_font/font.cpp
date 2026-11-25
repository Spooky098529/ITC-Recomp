// The font's layout as plain C++: see strings_and_font.h.

#include "strings_and_font/strings_and_font.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "command_list/command_list.h"
#include "guest.h"
#include "schema/command_list.h"
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

uint32_t Params(uint32_t font) { return Font(font).params().address; }

// The glyph record of `c` in `page`'s records.
FontGlyphRecord Record(uint32_t page, uint32_t c) {
  return FontGlyphRecord(FontPage(page).records().address + (c << 2));
}

// A 16-bit integer kept from fctidz's result, as a float.
double Low16(double value) { return ToSingle(uint16_t(Fctidz(value))); }

// A glyph's advance: its record's +2 >> 2 times the glyph scale, truncated
// to 16 bits, plus the letter spacing unless justified (alignment 3).
double Advance(uint32_t font, uint32_t params, uint32_t page, uint32_t c) {
  const bool justified = FontDrawParams(params).align_x() == 3;
  const double glyph_scale = FontDrawParams(params).glyph_scale_x();
  const uint32_t advance = uint32_t(Record(page, c).advance_and_channel()) >> 2;
  const double width = Low16(Single(ToSingle(advance) * glyph_scale));
  if (justified) {
    return width;
  }
  return Single(width + Font(font).letter_spacing());
}

}  // namespace

double FontTextWidth(uint32_t font, uint32_t params, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108B30 Font::TextWidth");
  const double zero = Lfs(kZero);
  double line = zero, widest = zero;
  FontSetParams(font, params);
  params = Params(font);
  // The font is a host object (E5 step 5): its pages (+20 + 4i) by offset.
  const uint32_t page = uint32_t(
      Font(font).load_at(20 + (FontDrawParams(params).page() << 2), 4));
  Font(font).set_page(page);
  uint32_t c = FontText(text).unit();
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
        line = Single(Advance(font, params, page, c) + line);
      } else if (!IsColor(c)) {
        line = Single(Advance(font, params, page, c) + line);
      }
      text += 2;
      c = FontText(text).unit();
      if (c == 0) {
        break;
      }
    }
    if (line > widest) {
      widest = line;
    }
  }
  const double spacing = Font(font).letter_spacing();
  return Single(Single(widest - spacing) * FontDrawParams(params).scale_x());
}

double FontLineWidth(uint32_t font, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108458 Font::LineWidth");
  double width = Lfs(kZero);
  for (uint32_t c; (c = FontText(text).unit()) != 0; text += 2) {
    if (c == kPageBreak || c == '\n') {
      break;
    }
    if (IsColor(c)) {
      continue;
    }
    const uint32_t params = Params(font);
    const uint32_t page = Font(font).page().address;
    width = Single(Advance(font, params, page, c) + width);
  }
  const FontDrawParams params(Params(font));
  if (params.align_x() == 3) {
    return width;
  }
  return Single(Single(width - Font(font).letter_spacing()) * params.scale_x());
}

uint32_t FontLineGlyphs(uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108810 Font::LineGlyphs");
  uint32_t count = 0;
  for (uint32_t c; (c = FontText(text).unit()) != 0; text += 2) {
    if (c == kPageBreak || c == '\n') {
      break;
    }
    if (!IsColor(c)) {
      count = (count + 1) & 0xFFFF;
    }
  }
  return count;
}

double FontLineX(uint32_t font, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108FE0 Font::LineX");
  const double width = FontLineWidth(font, text);
  const uint32_t params = Params(font);
  const int32_t w = Fctiwz(width);
  const uint32_t align = FontDrawParams(params).align_x();
  switch (align) {
    case 0:
      return double(Font(font).start_x());
    case 1: {
      // start + box width x scale x 0.5 - w x 0.5, truncated.
      const int32_t box = int32_t(FontDrawParams(params).box_width());
      const double scale = double(FontDrawParams(params).scale_x());
      const double start = double(Font(font).start_x());
      const double half = Lfs(kHalf);
      const double scaled = Single(ToSingle(box) * scale);
      const double center = Single(std::fma(scaled, half, start));
      const double x = Single(-std::fma(ToSingle(w), half, -center));
      return ToSingle(Fctiwz(x));
    }
    case 2: {
      // start + box width x scale - w, truncated.
      const int32_t box = int32_t(FontDrawParams(params).box_width());
      const double scale = double(FontDrawParams(params).scale_x());
      const double start = double(Font(font).start_x());
      const double right = Single(std::fma(ToSingle(box), scale, start));
      return ToSingle(Fctiwz(Single(right - ToSingle(w))));
    }
    case 3: {
      const uint32_t glyphs = FontLineGlyphs(text);
      const double start = double(Font(font).start_x());
      if (glyphs > 1) {
        const int32_t box = int32_t(FontDrawParams(params).box_width());
        const double gaps = ToSingle(int32_t(glyphs - 1));
        const double spacing = Single(Single(ToSingle(box) - ToSingle(w)) / gaps);
        Font(font).set_letter_spacing(float(spacing));
      }
      return start;
    }
    default:
      return width;
  }
}

double FontLineY(uint32_t font, uint32_t lines, uint32_t line, double f1) {
  ITC_NATIVE_RUNNING("sub_82108568 Font::LineY");
  lines &= 0xFFFF;
  line &= 0xFFFF;
  const uint32_t page = Font(font).page().address;
  const uint32_t params = Params(font);
  const double spacing = double(Font(font).line_spacing());
  const uint32_t table = FontPage(page).glyph_table().address;
  const double glyph_scale = double(FontDrawParams(params).glyph_scale_y());
  const uint32_t align = FontDrawParams(params).align_y();
  const double scale = double(FontDrawParams(params).scale_y());
  const uint32_t cell = FontGlyphTable(table).cell_height();
  // The line height (16 bits) and the block's height, scaled.
  const uint32_t height = uint16_t(Fctidz(Single(ToSingle(cell) * glyph_scale)));
  double block = Single(ToSingle(height) + spacing);
  block = Single(std::fma(block, ToSingle(lines), -spacing));
  block = Single(block * scale);
  switch (align) {
    case 0: {
      const double start = double(Font(font).start_y());
      const double step = Single(ToSingle(height) + spacing);
      return Single(std::fma(Single(step * ToSingle(line)), scale, start));
    }
    case 1: {
      const int32_t box = int32_t(FontDrawParams(params).box_height());
      const double s = double(FontDrawParams(params).scale_y());
      const double start = double(Font(font).start_y());
      const double line_spacing = double(Font(font).line_spacing());
      const double half = Lfs(kHalf);
      const double step = Single(ToSingle(height) + line_spacing);
      const double middle = Single(std::fma(ToSingle(box / 2), s, start));
      const double first = Single(-std::fma(block, half, -middle));
      return Single(std::fma(Single(step * ToSingle(line)), s, first));
    }
    case 2: {
      const int32_t box = int32_t(FontDrawParams(params).box_height());
      const double s = double(FontDrawParams(params).scale_y());
      const double start = double(Font(font).start_y());
      const double line_spacing = double(Font(font).line_spacing());
      const double step = Single(ToSingle(height) + line_spacing);
      const double bottom = Single(std::fma(ToSingle(box), s, start));
      const double first = Single(bottom - block);
      return Single(std::fma(Single(step * ToSingle(line)), s, first));
    }
    case 3: {
      if (lines <= 1) {
        return double(Font(font).start_y());
      }
      const uint32_t table_again = FontPage(page).glyph_table().address;
      const double glyph_scale_again = double(FontDrawParams(params).glyph_scale_y());
      const uint32_t box = FontDrawParams(params).box_height();
      const double start = double(Font(font).start_y());
      const uint32_t cell_again = FontGlyphTable(table_again).cell_height();
      const uint32_t h =
          uint16_t(Fctidz(Single(ToSingle(cell_again) * glyph_scale_again)));
      const int32_t left = int32_t(box - h * lines);
      const int32_t gaps = int32_t(lines - 1);
      const double line_spacing = ToSingle(left / gaps);
      Font(font).set_line_spacing(float(line_spacing));
      const double step = Single(ToSingle(height) + line_spacing);
      const double s = double(FontDrawParams(params).scale_y());
      return Single(std::fma(Single(step * ToSingle(line)), s, start));
    }
    default:
      return f1;
  }
}

uint32_t FontCountGlyphs(uint32_t font, uint32_t counts, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82108210 Font::CountGlyphs");
  const uint32_t length = c_runtime::Wcslen(text);
  for (uint32_t left = length; left != 0; left--, text += 2) {
    const uint32_t c = FontText(text).unit();
    if (c == '\n' || c == ' ' || IsColor(c)) {
      continue;
    }
    const uint32_t page = Font(font).page().address;
    const uint32_t channel = (uint32_t(Record(page, c).advance_and_channel()) << 2) & 0xC;
    guest::StoreU32(counts + channel, guest::LoadU32(counts + channel) + 1);
  }
  return length;
}

uint32_t FontOutCode(uint32_t font, uint32_t vertex) {
  ITC_NATIVE_RUNNING("sub_821082A0 Font::OutCode");
  const uint32_t params = Params(font);
  uint32_t code;
  const double x = FontVertex(vertex).x();
  if (x < ToSingle(int32_t(FontDrawParams(params).clip_x()))) {
    code = 1;
  } else {
    const uint32_t p = Params(font);
    const int32_t right =
        int32_t(FontDrawParams(p).clip_width() + FontDrawParams(p).clip_x());
    code = x > ToSingle(right) ? 2 : 4;
  }
  const double y = FontVertex(vertex).y();
  if (y < ToSingle(int32_t(FontDrawParams(params).clip_y()))) {
    return code | 8;
  }
  const uint32_t p = Params(font);
  const int32_t bottom =
      int32_t(FontDrawParams(p).clip_height() + FontDrawParams(p).clip_y());
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

bool FontRotated(uint32_t font) {
  const double angle = double(FontDrawParams(Params(font)).angle());
  const double turns = Single(angle * Lfd(G::turn_address()));
  return !(turns == ToSingle(Fctiwz(turns)));
}

uint32_t FontClipGlyph(uint32_t font, uint32_t v) {
  ITC_NATIVE_RUNNING("sub_82108C90 Font::ClipGlyph");
  // The glyph's six vertices.
  auto vertex_at = [&](uint32_t k) {
    return FontVertex(schema::FontGlyphQuad(v).vertices_address() + 24 * k);
  };
  const uint32_t params = Params(font);
  const int32_t left = int32_t(FontDrawParams(params).clip_x());
  if (left == 0 && FontDrawParams(params).clip_y() == 0 &&
      FontDrawParams(params).clip_width() == 0 &&
      FontDrawParams(params).clip_height() == 0) {
    return 1;
  }
  // Wholly outside: the first vertex below the bottom, the third above the
  // top, the first right of the right edge, the second left of the left.
  auto bottom = [&] {
    const FontDrawParams p(Params(font));
    return ToSingle(int32_t(p.clip_height() + p.clip_y()));
  };
  auto right = [&] {
    const FontDrawParams p(Params(font));
    return ToSingle(int32_t(p.clip_width() + p.clip_x()));
  };
  if (double(vertex_at(0).y()) > bottom()) {
    return 0;
  }
  if (double(vertex_at(2).y()) < ToSingle(int32_t(FontDrawParams(params).clip_y()))) {
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
    const uint32_t code = FontOutCode(font, vertex.address) & 0xFF;
    if ((code & 4) && (code & 0x20)) {
      continue;
    }
    // x to the edge, u by the same distance / the scale / the texture's width.
    if (code & 3) {
      const double x = vertex.x();
      const double u = vertex.u();
      const double edge =
          (code & 1) ? ToSingle(int32_t(FontDrawParams(Params(font)).clip_x()))
                     : right();
      vertex.set_x(float(edge));
      const uint32_t width = Font(font).page().texture_width();
      const double scale = double(FontDrawParams(Params(font)).scale_x());
      double d = Single((code & 1) ? edge - x : x - edge);
      d = Single(d / scale);
      d = Single(d / UToSingle(width));
      vertex.set_u(float(Single((code & 1) ? d + u : u - d)));
    }
    if (code & 0x18) {
      const double y = vertex.y();
      const double tv = vertex.v();
      const double edge =
          (code & 8) ? ToSingle(int32_t(FontDrawParams(Params(font)).clip_y()))
                     : bottom();
      vertex.set_y(float(edge));
      const uint32_t height = Font(font).page().texture_height();
      const double scale = double(FontDrawParams(Params(font)).scale_y());
      double d = Single((code & 8) ? edge - y : y - edge);
      d = Single(d / scale);
      d = Single(d / UToSingle(height));
      vertex.set_v(float(Single((code & 8) ? d + tv : tv - d)));
    }
  }
  return 1;
}

bool FontAddGlyph(uint32_t font, uint32_t c, uint32_t color, double x, double y) {
  if (FontRotated(font)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82109560 Font::AddGlyph");
  const uint32_t page = Font(font).page().address;
  const FontPage page_view(page);
  const uint32_t texture_height = page_view.texture_height();
  const FontGlyphRecord record(page_view.records().address + ((c << 2) & 0x3FFFC));
  const FontGlyphTable table = page_view.glyph_table();
  const uint32_t texture_width = page_view.texture_width();
  const uint32_t params = Params(font);
  const uint32_t cell_width = table.cell_width();
  const uint32_t channel = (uint32_t(record.advance_and_channel()) << 2) & 0xC;
  const uint32_t cell = record.cell();
  const double glyph_scale_x = double(FontDrawParams(params).glyph_scale_x());
  const uint32_t cell_height = table.cell_height();
  const double glyph_scale_y = double(FontDrawParams(params).glyph_scale_y());
  const double scale_y = double(FontDrawParams(params).scale_y());
  const double scale_x = double(FontDrawParams(params).scale_x());
  // The cell's place in the sheet: columns = texture width / cell width.
  const uint32_t columns = Divwu(texture_width, cell_width) & 0xFFFF;
  const double size_y = Single(glyph_scale_y * scale_y);
  const double size_x = Single(glyph_scale_x * scale_x);
  const int32_t row = Divw(int32_t(cell), int32_t(columns));
  const uint32_t write = guest::LoadU32(kWrite + channel);
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
  const double z = Lfs(kZero);
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
      guest::StoreU32(write + store.offset, color);
    } else {
      Stfs(write + store.offset, store.value);
    }
  }
  if (FontClipGlyph(font, write) != 0) {
    guest::StoreU32(kWrite + channel, guest::LoadU32(kWrite + channel) + 144);
  } else {
    guest::StoreU32(kCounts + channel, guest::LoadU32(kCounts + channel) - 1);
  }
  return true;
}

bool FontBuildVertices(uint32_t font, uint32_t text) {
  if (FontRotated(font)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82109AA8 Font::BuildVertices");
  const uint32_t length = c_runtime::Wcslen(text);
  uint32_t lines = 1;
  for (uint32_t at = text, c; (c = FontText(at).unit()) != 0; at += 2) {
    if (c == kPageBreak) {
      break;
    }
    if (c == '\n') {
      lines = (lines + 1) & 0xFFFF;
    }
  }
  // The color: alpha (+24 x 255, the byte fctidz leaves lowest), then the
  // red, green and blue bytes (+20..+22).
  uint32_t params = Params(font);
  const double alpha = double(FontDrawParams(params).alpha());
  const uint32_t blue = FontDrawParams(params).blue();
  const double full = Lfs(G::full_alpha_address());
  const uint32_t red = FontDrawParams(params).red();
  const uint32_t a = uint8_t(Fctidz(Single(alpha * full)));
  const uint32_t green = FontDrawParams(params).green();
  const uint32_t alpha_bits = a << 8;
  uint32_t color = ((((red | alpha_bits) << 8) | green) << 8) | blue;
  uint32_t line = 0;
  double x = FontLineX(font, text);
  double y = FontLineY(font, lines, 0, x);
  // The second pass's positions, 8 bytes a character (pair - 4 the x).
  uint32_t pair = Font(font).second_pass_positions_address() + 4;
  for (uint32_t left = length; left != 0; left--, text += 2, pair += 8) {
    const uint32_t page = Font(font).page().address;
    const uint32_t c = FontText(text).unit();
    params = Params(font);
    const FontGlyphRecord record = Record(page, c);
    const double glyph_scale = double(FontDrawParams(params).glyph_scale_x());
    const uint32_t advance =
        uint16_t(Fctidz(Single(UToSingle(uint32_t(record.advance_and_channel()) >> 2) *
                               glyph_scale)));
    if (c == '\n') {
      line = (line + 1) & 0xFFFF;
      x = FontLineX(font, text + 2);
      y = FontLineY(font, lines, line, x);
      continue;
    }
    if (c == kColor) {
      const uint32_t p = Params(font);
      const uint32_t r = FontDrawParams(p).red();
      const uint32_t g = FontDrawParams(p).green();
      const uint32_t b = FontDrawParams(p).blue();
      color = ((((r | alpha_bits) << 8) | g) << 8) | b;
      continue;
    }
    if (c > 0xFFF5 && c <= 0xFFFF) {
      // A color from the font's ten (3 bytes each at +16528).
      const uint32_t k = (0xFFFF - c) & 0xFFFF;
      const Font f(font);
      const uint32_t r = uint32_t(f.load_at(16528 + 3 * k, 1));
      const uint32_t g = uint32_t(f.load_at(16528 + 1 + 3 * k, 1));
      const uint32_t b = uint32_t(f.load_at(16528 + 2 + 3 * k, 1));
      color = ((((r | alpha_bits) << 8) | g) << 8) | b;
      continue;
    }
    if (c != ' ') {
      FontAddGlyph(font, c, color, x, y);
      if (FontDrawParams(Params(font)).second_pass() != 0) {
        // The second pass's position (never in play).
        uint32_t p = Params(font);
        const double sx = double(FontDrawParams(p).scale_x());
        const double ox = double(FontDrawParams(p).second_offset_x());
        Stfs(pair - 4, Single(std::fma(ox, sx, x)));
        p = Params(font);
        const double oy = double(FontDrawParams(p).second_offset_y());
        Stfs(pair, Single(std::fma(oy, double(FontDrawParams(p).scale_y()), y)));
      }
      params = Params(font);
    }
    // x moves on by (the letter spacing + the advance) x the scale.
    const double step = Single(double(Font(font).letter_spacing()) + ToSingle(advance));
    x = Single(std::fma(step, double(FontDrawParams(params).scale_x()), x));
  }
  return true;
}

DrawPassResult FontDrawPass(uint32_t font, uint32_t text) {
  if (text == 0) {
    return {DrawPassResult::kNothing, 0};
  }
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(kCounts + 4 * i, 0);
  }
  if (FontCountGlyphs(font, kCounts, text) == 0) {
    return {DrawPassResult::kNothing, 0};
  }
  const schema::CommandList list_view = Font(font).command_list();
  const uint32_t list = list_view.address;
  // What the original would do differently: rotated text (sub_821097F0),
  // or its command list calls printing a message (another chunk open, the
  // list full: the slots counted at most, as clipping only lowers the
  // counts). The counts are redone by the original, the same.
  if (FontRotated(font)) {
    return {DrawPassResult::kOriginal, 0};
  }
  if (list_view.chunk_open() == 0 && schema::CommandListGlobals::open_chunks() != 0) {
    return {DrawPassResult::kOriginal, 0};
  }
  uint64_t slots = 19 + 3;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t vertices = guest::LoadU32(kCounts + 4 * i) * 6;
    if (vertices != 0) {
      slots += 1 + ((vertices * 24 + 15) >> 4) + 4;
    }
  }
  if (uint64_t(list_view.used()) + slots >= list_view.capacity()) {
    return {DrawPassResult::kOriginal, 0};
  }
  ITC_NATIVE_RUNNING("sub_82109EF0 Font::DrawPass");
  command_list::OpenChunk(list);
  auto slot = [&] { return command_list::AllocateSlots(list, 1); };
  // Each command a slot of {command, count, 0} and its payload's slots.
  auto command = [&](uint32_t id, uint32_t count) {
    const uint32_t at = slot();
    guest::StoreU32(at + 0, id);
    guest::StoreU32(at + 4, count);
    guest::StoreU32(at + 8, 0);
  };
  auto pair = [&](uint32_t a, uint32_t b) {
    const uint32_t at = slot();
    guest::StoreU32(at + 0, a);
    guest::StoreU32(at + 4, b);
  };
  const uint32_t chunk = slot();
  G::set_chunk(chunk);
  guest::StoreU16(chunk + 2, 5);
  guest::StoreU32(chunk + 4, 0);
  command(4, 1);  // SetTexture: sampler 0, the page's texture
  {
    const uint32_t at = slot();
    guest::StoreU32(at + 0, 0);
    guest::StoreU32(at + 4, Font(font).page().texture().address);
  }
  command(1, 5);  // SetRenderState x 5
  pair(60, 1);    // blend enable
  pair(72, 6);    // source blend
  pair(76, 7);    // destination blend
  pair(304, 0);   // viewport transform off
  pair(56, 0);    // cull mode
  const uint32_t params = Params(font);
  if (FontDrawParams(params).clip_x() == 0 && FontDrawParams(params).clip_y() == 0 &&
      FontDrawParams(params).clip_width() == 0 &&
      FontDrawParams(params).clip_height() == 0) {
    command(1, 1);
    pair(200, 0);  // scissor off
  } else {
    command(31, 1);  // SetScissorRect: the clip rectangle as halfwords
    const uint32_t at = slot();
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU16(at + 2 * k,
                      uint16_t(guest::LoadU32(FontDrawParams(Params(font)).clip_x_address() + 4 * k)));
    }
  }
  {
    const uint32_t at = slot();  // SetSamplerState x 3 on sampler 0
    guest::StoreU32(at + 8, 0);
    guest::StoreU32(at + 4, 3);
    guest::StoreU32(at + 0, 2);
  }
  auto triple = [&](uint32_t a, uint32_t b, uint32_t c) {
    const uint32_t at = slot();
    guest::StoreU32(at + 0, a);
    guest::StoreU32(at + 4, b);
    guest::StoreU32(at + 8, c);
  };
  triple(0, 24, 2);  // mip filter 2
  triple(0, 20, 1);  // min filter 1
  triple(0, 16, 1);  // mag filter 1
  command(10, 1);    // SetVertexDeclaration
  guest::StoreU32(slot() + 0, Font(font).vertex_declaration<schema::View>().address);
  command(8, 1);     // SetVertexShader
  guest::StoreU32(slot() + 0, Font(font).vertex_shader<schema::View>().address);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t vertices = guest::LoadU32(kCounts + 4 * i) * 6;
    uint32_t start = 0;
    if (vertices != 0) {
      const uint32_t at = slot();
      guest::StoreU32(at + 8, 0);
      guest::StoreU32(at + 4, vertices * 24);
      guest::StoreU32(at + 0, 21);
      start = command_list::AllocateBytes(list, vertices * 24, false);
    }
    guest::StoreU32(kWrite + 4 * i, start);
    guest::StoreU32(kStarts + 4 * i, start);
  }
  FontBuildVertices(font, text);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t vertices = guest::LoadU32(kCounts + 4 * i) * 6;
    if (vertices == 0) {
      continue;
    }
    command(9, 1);  // SetPixelShader: the channel's
    guest::StoreU32(slot() + 0, uint32_t(Font(font).load_at(16568 + 4 * i, 4)));
    command(19, 1);  // DrawPrimitiveUP: a triangle list, 24-byte vertices
    const uint32_t at = slot();
    guest::StoreU32(at + 0, 4);
    guest::StoreU16(at + 4, uint16_t(vertices));
    guest::StoreU16(at + 6, 24);
    guest::StoreU32(at + 8, guest::LoadU32(kStarts + 4 * i));
  }
  command(1, 1);
  pair(304, 1);   // viewport transform back on
  command(36, 0);
  command_list::CloseChunk(list);
  return {DrawPassResult::kRecorded, G::chunk()};
}

void FontDrawSetUp(uint32_t font, uint32_t params) {
  ITC_NATIVE_RUNNING("sub_8210A400 Font::Draw");
  FontSetParams(font, params);
  const uint32_t page = FontDrawParams(Params(font)).page();
  Font(font).set_page(uint32_t(Font(font).load_at(20 + (page << 2), 4)));
}

uint32_t TextDrawSetUp(uint32_t params) {
  ITC_NATIVE_RUNNING("sub_8210A758 Text::Draw");
  const uint32_t font = G::font().address;
  FontDrawSetUp(font, params);
  return font;
}

}  // namespace strings_and_font
