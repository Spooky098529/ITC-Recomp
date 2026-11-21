// The UI movies' texts drawn (UiMovie::DrawTexts and UiMovie::ScrollOffset)
// as plain C++: see game.h. Float work is the translated code's step for
// step (doubles, std::fma where the PowerPC code fuses), flush-to-zero off
// (the adapter's job). Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt).

#include "ui_movies/game.h"

#include "base/log.h"
#include "debug/frame_state_diff.h"
#include "schema/ui_movies.h"
#include "schema/ui_movies_game.h"
#include "strings_and_font/steps.h"
#include "strings_and_font/strings_and_font.h"

#include <cmath>
#include <cstring>

namespace ui_movies {

using namespace strings_and_font::steps;

namespace {
// The constants DrawTexts loads into f26 to f31 (floats) and ScrollOffset's
// double 1.0.
constexpr uint32_t kF26 = 0x82002A04;
constexpr uint32_t kF27 = 0x820021E8;
constexpr uint32_t kF28 = 0x82000C44;
constexpr uint32_t kF29 = 0x82002200;
constexpr uint32_t kF30 = 0x82000A34;  // 0.0
constexpr uint32_t kF31 = 0x82000A30;
constexpr uint32_t kOne = 0x82002250;  // a double

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

void CopyBytes(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, to + i, guest::LoadU32(base, from + i));
  }
}
}  // namespace

double ScrollOffset(uint8_t* base, uint32_t movie, uint32_t params) {
  ITC_NATIVE_RUNNING("sub_8211B800 UiMovie::ScrollOffset");
  // The text's start x, as Font::SetParams works it out: x - (scale - 1) *
  // the origin's x.
  const schema::UiMovieTextParams p(base, params);
  const double scale = Lfs(base, p.scale_x_address());
  const uint32_t id = p.text_id();
  const double scale_less_one = scale - Lfd(base, kOne);
  const double origin = ToSingle(int32_t(p.origin_x()));
  const double x = ToSingle(int32_t(p.x()));
  const double start = Single(-std::fma(scale_less_one, origin, -x));
  const uint32_t text = TextString(base, movie, id);
  const double width = strings_and_font::FontTextWidth(
      base, guest::LoadU32(base, strings_and_font::kFont), params, text);
  const schema::UiMovieText record(
      base,
      schema::UiMovieWord(
          base, TextMapFindKey(base, schema::UiMovie(base, movie).texts_address(), p.text_id()))
          .value());
  const int32_t counter = int32_t(record.scroll_counter());
  const int32_t delay = int32_t(record.scroll_delay());
  if (counter < delay) {
    return ToSingle(int32_t(p.x()));
  }
  const double speed = Lfs(base, record.scroll_speed_address());
  const double end = Single(start - width);
  const double ticks = ToSingle(int32_t(uint32_t(counter) - uint32_t(delay)));
  double f1 = Single(-std::fma(ticks, speed, -start));
  if (f1 < end) {
    const double step = Single(ToSingle(int32_t(p.gap())) + width);
    do {
      f1 = Single(step + f1);
    } while (f1 < end);
  }
  return f1;
}

uint32_t DrawTextsPop(uint8_t* base, uint32_t frame, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8211CD90 UiMovie::DrawTexts");
  guest::ForEachHostPart(base, frame + kDrawTextsBuffer, 4096,
                         [](uint8_t* host, uint32_t, uint32_t bytes) {
                           std::memset(host, 0, bytes);
                         });
  const schema::UiMovieDrawTextList l(base, list);
  const uint32_t count = l.count() - 1;
  l.set_count(count);
  return l.items().address + (count << 7);
}

void DrawTextsItem(uint8_t* base, uint32_t frame, uint32_t movie, bool draw_only) {
  const double f26 = Lfs(base, kF26), f27 = Lfs(base, kF27), f28 = Lfs(base, kF28),
               f29 = Lfs(base, kF29), f30 = Lfs(base, kF30), f31 = Lfs(base, kF31);
  const schema::UiMovieTextItem item(base, frame + kDrawTextsItem);
  const schema::UiMovieTextParams p(base, frame + kDrawTextsParams);
  const uint32_t node = item.node();
  // The item's matrix (+16) to +304, the origin (0, 0, 0, f31) at +96, the
  // item's color (+84) at +240 and its clip rectangle (+100) at +256; the
  // origin transformed in place.
  CopyBytes(base, frame + 304, item.matrix_address(), 64);
  Stfs(base, frame + 96, f30);
  Stfs(base, frame + 100, f30);
  Stfs(base, frame + 104, f30);
  Stfs(base, frame + 108, f31);
  CopyBytes(base, frame + 240, item.red_address(), 16);
  CopyBytes(base, frame + 256, item.clip_0_address(), 16);
  debug::Vec4Transform(base, frame + 96, frame + 304, frame + 96);
  // The node: the matrix's scale (+28, +32) and the clip rectangle (+40).
  const uint32_t clip[4] = {guest::LoadU32(base, frame + 256), guest::LoadU32(base, frame + 260),
                            guest::LoadU32(base, frame + 264), guest::LoadU32(base, frame + 268)};
  const double scale_x = Lfs(base, frame + 304);
  const double scale_y = Lfs(base, frame + 324);
  Stfs(base, node + 32, scale_y);
  Stfs(base, node + 28, scale_x);
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, node + 40 + 4 * k, clip[k]);
  }
  // The draw parameters: the position, the color and alpha, the layout.
  const double item_alpha = Lfs(base, frame + 252);
  const double red_scale = Lfs(base, frame + 240);
  p.set_x(uint32_t(Fctiwz(Lfs(base, frame + 96))));
  const double alpha = Single(Lfs(base, node + 68) * item_alpha);
  const int64_t red = int32_t(guest::LoadU32(base, node + 56));
  const int64_t green = int32_t(guest::LoadU32(base, node + 60));
  const int64_t blue = int32_t(guest::LoadU32(base, node + 64));
  p.set_y(uint32_t(Fctiwz(Lfs(base, frame + 100))));
  StoreU64(base, frame + 288, uint64_t(red));
  StoreU64(base, frame + 272, uint64_t(green));
  StoreU64(base, frame + 280, uint64_t(blue));
  const int32_t text_x = Fctiwz(Lfs(base, node + 100));
  double r = ToSingle(red), g = ToSingle(green), b = ToSingle(blue);
  r = Single(r * f29);
  g = Single(g * f29);
  b = Single(b * f29);
  r = Single(r * red_scale);
  g = Single(g * Lfs(base, frame + 244));
  b = Single(b * Lfs(base, frame + 248));
  p.set_gap(uint32_t(text_x));
  p.set_unknown_72(uint32_t(Fctiwz(Lfs(base, node + 104))));
  p.set_unknown_76(guest::LoadU32(base, node + 124));
  r = Single(r * f28);
  g = Single(g * f28);
  b = Single(b * f28);
  p.set_unknown_80(guest::LoadU32(base, node + 128));
  const int64_t red_byte = Fctidz(r);
  p.set_unknown_84(uint32_t(Fctiwz(Lfs(base, node + 116))));
  const int64_t green_byte = Fctidz(g);
  p.set_unknown_88(uint32_t(Fctiwz(Lfs(base, node + 120))));
  const int64_t blue_byte = Fctidz(b);
  guest::StoreU32(base, frame + 84, uint32_t(red_byte));
  p.set_origin_x(guest::LoadU32(base, node + 20));
  p.set_origin_y(guest::LoadU32(base, node + 24));
  Stfs(base, p.alpha_address(), alpha);
  guest::StoreU32(base, frame + 80, uint32_t(green_byte));
  p.set_red(uint8_t(red_byte));
  guest::StoreU32(base, frame + 88, uint32_t(blue_byte));
  p.set_green(uint8_t(green_byte));
  p.set_blue(uint8_t(blue_byte));
  Stfs(base, p.scale_x_address(), Lfs(base, node + 28));
  Stfs(base, p.scale_y_address(), Lfs(base, node + 32));
  const double angle = Lfs(base, node + 36);
  p.set_unknown_64(24);
  Stfs(base, p.angle_address(), angle);
  const uint32_t layer = guest::LoadU32(base, node + 72);
  Stfs(base, p.glyph_scale_x_address(), f31);
  p.set_unknown_92(128);
  Stfs(base, p.glyph_scale_y_address(), f31);
  p.set_unknown_93(128);
  Stfs(base, p.unknown_104_address(), f31);
  p.set_unknown_94(128);
  p.set_layer(layer);
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, p.clip_0_address() + 4 * k, guest::LoadU32(base, node + 40 + 4 * k));
  }
  Stfs(base, p.unknown_96_address(), f27);
  Stfs(base, p.unknown_100_address(), f27);
  p.set_unknown_112(0);
  p.set_text_id(guest::LoadU32(base, node + 108));
  // The alignment from the node's flags (+76): bit 0 left (0), bit 1 (1),
  // bit 3 (3), else 2; bit 4 (with none of the others) also sets the glyph
  // scales to f26.
  const uint32_t flags = guest::LoadU32(base, node + 76);
  uint32_t alignment = 2;
  if (flags & 0x1) {
    alignment = 0;
  } else if (flags & 0x2) {
    alignment = 1;
  } else if (flags & 0x4) {
    alignment = 2;
  } else if (flags & 0x8) {
    alignment = 3;
  } else if (flags & 0x10) {
    Stfs(base, p.glyph_scale_x_address(), f26);
    Stfs(base, p.glyph_scale_y_address(), f26);
  }
  p.set_alignment(alignment);
  // A scrolling text's x, its counter stepped unless only drawing.
  if (TextField(base, movie, guest::LoadU32(base, node + 108), 4) != 0) {
    const double x = ScrollOffset(base, movie, p.address);
    p.set_x(uint32_t(Fctiwz(x)));
    if (!draw_only) {
      AdvanceTextScroll(base, movie, guest::LoadU32(base, node + 108));
    }
  }
  // The text's page, formatted into the buffer.
  const uint32_t id = guest::LoadU32(base, node + 108);
  const uint32_t skip = TextField(base, movie, id, 24);
  const uint32_t page = TextField(base, movie, id, 20);
  const uint32_t text = TextString(base, movie, id);
  strings_and_font::StringsCopyPage(base, frame + kDrawTextsBuffer, text, int32_t(page),
                                    int32_t(skip));
}

}  // namespace ui_movies
