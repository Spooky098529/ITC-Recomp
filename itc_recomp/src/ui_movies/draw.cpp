// The UI movie library's drawing walk natively (ui_movies.h): the nodes'
// transforms. The draw contexts and nodes are guest objects; the matrices go
// through the matrix math (src/matrix_math/) and are noted for frame
// interpolation as the originals' calls are, with their return addresses.
// Built with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).

#include "ui_movies/ui_movies.h"

#include "schema/tuning_pages.h"
#include "schema/ui_movies_game.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "debug/frame_state_diff.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "ui_movies/game.h"

#include <simde/x86/sse2.h>

#include <climits>
#include <cmath>

namespace ui_movies {

using namespace car_physics::ops;

namespace {

using G = schema::UiMoviesGlobals;
using Context = schema::UiMovieDrawContext;
using Quad = schema::UiMovieQuad;
using Node = schema::UiMovieNode;
using Vertex = schema::UiMovieVertex;

// fctiwz, as the translation does it: NaN gives INT_MIN, INT_MAX and above
// INT_MAX, below INT_MIN INT_MIN.
int32_t Fctiwz(double x) {
  if (std::isnan(x)) {
    return INT32_MIN;
  }
  if (x >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  return simde_mm_cvttsd_si32(simde_mm_set_sd(x));
}

// An integer as lfd of its std then fcfid then frsp.
double IntToFloat(int32_t n) {
  return double(float(double(int64_t(n))));
}

// The identity at `m` (64 bytes of guest memory), as the original builds it
// from the vector unit's 1.0 and 0.0 and copies it there.
void Identity(uint32_t m) {
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(m + 4 * i, i % 5 == 0 ? 0x3F800000u : 0);
  }
}

// ctx = local * ctx, noted as the original's call returning to `caller`.
void MultiplyInto(uint32_t context, uint32_t local, uint32_t caller) {
  matrix_math::Multiply(context, local, context);
  interpolation::BlendState_OnMultiply(context, local, context, caller);
}

// (x > 0.0 ? x : -x) > the threshold, as the original's compares (a NaN
// fails it).
bool BeyondThreshold(double x, double zero, double threshold) {
  return (x > zero ? x : Fneg(x)) > threshold;
}

}  // namespace

uint32_t ApplyTransform(uint32_t context, uint32_t node, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8259E6C8 UiMovie::ApplyTransform");
  const Context c(context);
  uint32_t r3 = context;
  // The clip: the node's rectangle offset by the context's +32 and +36 (as
  // integers), kept or intersected with the context's (sub_8259D730,
  // merged).
  const uint32_t n40 = Node(node).clip_0();
  if (n40 != 0 || Node(node).clip_1() != 0 || Node(node).clip_2() != 0 ||
      Node(node).clip_3() != 0) {
    const uint32_t x = uint32_t(Fctiwz(Lfs(c.matrix_address() + 32)));
    const uint32_t n48 = Node(node).clip_2();
    const uint32_t y = uint32_t(Fctiwz(Lfs(c.matrix_address() + 36)));
    const uint32_t n44 = Node(node).clip_1();
    const uint32_t n52 = Node(node).clip_3();
    const uint32_t rect[4] = {x + n40, y + n44, x + n48, y + n52};
    r3 = c.clip_left_address();
    if (c.clip_left() == 0 && c.clip_top() == 0 && c.clip_right() == 0 && c.clip_bottom() == 0) {
      c.set_clip_left(rect[0]);
      c.set_clip_top(rect[1]);
      c.set_clip_right(rect[2]);
      c.set_clip_bottom(rect[3]);
    } else {
      ITC_NATIVE_RUNNING("sub_8259D730 (merged into UiMovie::ApplyTransform)");
      // +0 and +12 the larger, +4 and +8 the smaller (signed), as read.
      const int32_t a0 = int32_t(c.clip_left());
      c.set_clip_left(uint32_t(a0 > int32_t(rect[0]) ? a0 : int32_t(rect[0])));
      const int32_t a1 = int32_t(c.clip_top());
      c.set_clip_top(uint32_t(a1 < int32_t(rect[1]) ? a1 : int32_t(rect[1])));
      const int32_t a2 = int32_t(c.clip_right());
      c.set_clip_right(uint32_t(a2 < int32_t(rect[2]) ? a2 : int32_t(rect[2])));
      const int32_t a3 = int32_t(c.clip_bottom());
      c.set_clip_bottom(uint32_t(a3 > int32_t(rect[3]) ? a3 : int32_t(rect[3])));
    }
  }
  // The color: r, g, b times the node's integers times the scale, alpha
  // times the node's float.
  {
    const double r = double(c.red());
    const double g = double(c.green());
    const double b = double(c.blue());
    const double a = double(c.alpha());
    const double nr = IntToFloat(int32_t(Node(node).red()));
    const double scale = double(G::color_scale());
    c.set_red(float(Fmuls(Fmuls(nr, r), scale)));
    const double ng = IntToFloat(int32_t(Node(node).green()));
    c.set_green(float(Fmuls(Fmuls(ng, g), scale)));
    const double nb = IntToFloat(int32_t(Node(node).blue()));
    c.set_blue(float(Fmuls(Fmuls(nb, b), scale)));
    c.set_alpha(float(Fmuls(Lfs(Node(node).alpha_address()), a)));
  }
  const double zero = double(G::zero());
  const uint32_t local = frame + 176;
  // Translated by the position plus the pivot.
  {
    const uint32_t n20 = Node(node).pivot_x();
    if (n20 != 0 || !(Lfs(Node(node).x_address()) == zero) ||
        Node(node).pivot_y() != 0 ||
        !(Lfs(Node(node).y_address()) == zero)) {
      Identity(local);
      const double px = Lfs(Node(node).x_address());
      const double py = Lfs(Node(node).y_address());
      Stfs(local + 56, zero);
      Stfs(local + 48, Fadds(px, IntToFloat(int32_t(n20))));
      Stfs(local + 52,
           Fadds(py, IntToFloat(int32_t(Node(node).pivot_y()))));
      MultiplyInto(context, local, 0x8259E940);
      r3 = context;
    }
  }
  // Turned by the rotation (degrees).
  const double threshold = double(G::near_zero());
  {
    const double angle = Lfs(Node(node).rotation_address());
    if (BeyondThreshold(angle, zero, threshold)) {
      const uint32_t turn = frame + 240;
      Identity(turn);
      Stfs(frame + 80, Fmuls(angle, double(G::degrees())));
      matrix_math::RotateZ(turn, turn, frame + 80, frame - 80);
      MultiplyInto(context, turn, 0x8259EA08);
      r3 = context;
    }
  }
  // Scaled, unless both factors are near zero; +108 set when either isn't
  // 1.0.
  {
    const double sx = Lfs(Node(node).scale_x_address());
    if (BeyondThreshold(sx, zero, threshold) ||
        BeyondThreshold(Lfs(Node(node).scale_y_address()), zero, threshold)) {
      Identity(local);
      // lfs then stfs: the words' bits (as CopyWord).
      CopyWord(Node(node).scale_x_address(), local + 0);
      CopyWord(Node(node).scale_y_address(), local + 20);
      MultiplyInto(context, local, 0x8259EAD8);
      r3 = context;
      const double one = double(G::one());
      if (!(Lfs(Node(node).scale_x_address()) == one) ||
          !(Lfs(Node(node).scale_y_address()) == one)) {
        c.set_scaled(1);
      }
    }
  }
  // Translated back by the pivot.
  {
    const uint32_t n20 = Node(node).pivot_x();
    if (n20 != 0 || Node(node).pivot_y() != 0) {
      Identity(local);
      const uint32_t n24 = Node(node).pivot_y();
      Stfs(local + 56, zero);
      Stfs(local + 48, IntToFloat(int32_t(0u - n20)));
      Stfs(local + 52, IntToFloat(int32_t(0u - n24)));
      MultiplyInto(context, local, 0x8259EBC4);
      r3 = context;
    }
  }
  return r3;
}

uint32_t CopyDrawContext(uint32_t context, uint32_t parent) {
  ITC_NATIVE_RUNNING("sub_8259D9F8 DrawContext::Copy");
  // The w column set to 1.0 first (then overwritten by the copy), as read.
  const Context to(context), from(parent);
  const float one = G::one();
  to.set_matrix_3(one);
  to.set_matrix_7(one);
  to.set_matrix_11(one);
  to.set_matrix_15(one);
  matrix_math::Copy(to.matrix_address(), from.matrix_address());
  interpolation::BlendState_OnCopy(to.matrix_address(), from.matrix_address(), 0x8259DA30);
  // The clip, the color, +96..+108, word by word.
  for (uint32_t at = 0; at < 48; at += 4) {
    guest::StoreU32(to.clip_left_address() + at,
                    guest::LoadU32(from.clip_left_address() + at));
  }
  return context;
}

uint32_t CopyQueuedText(uint32_t to, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_8259E620 TextItem::CopyQueued");
  const schema::UiMovieTextItem t(to), f(from);
  t.set_node(f.node());
  matrix_math::Copy(t.matrix_address(), f.matrix_address());
  interpolation::BlendState_OnCopy(t.matrix_address(), f.matrix_address(), 0x8259E650);
  CopyWord(f.depth_address(), t.depth_address());  // lfs then stfs: its bits
  for (uint32_t at = 0; at < 32; at += 4) {  // the color and the clip
    guest::StoreU32(t.red_address() + at, guest::LoadU32(f.red_address() + at));
  }
  return to;
}

bool TextListFull(uint32_t list) {
  const schema::UiMovieDrawTextList l(list);
  return l.count() >= l.capacity();
}

bool AppendText(uint32_t list, uint32_t item, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8259F700 TextList::Append");
  if (TextListFull(list)) {
    return false;  // the grow (sub_8259EC58, never seen in play): the original
  }
  const schema::UiMovieDrawTextList l(list);
  const uint32_t count = l.count();
  const uint32_t slot = (count << 7) + l.items().address;
  l.set_count(count + 1);
  *r3 = CopyQueuedText(slot, item);
  return true;
}

bool QueueText(uint32_t context, uint32_t node, uint32_t layer, uint32_t frame,
               uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8259F770 UiMovie::QueueText");
  const Context c(context);
  const uint32_t list = c.text_list().address;
  if (list != 0 && TextListFull(list)) {
    return false;
  }
  *r3 = ApplyTransform(context, node, frame - 352);
  if (c.text_list().address == 0) {
    return true;
  }
  const schema::UiMovieTextItem item(frame + 96);
  MakeTextItem(item.address);
  item.set_node(node);
  matrix_math::Copy(item.matrix_address(), c.matrix_address());
  interpolation::BlendState_OnCopy(item.matrix_address(), c.matrix_address(), 0x8259F7B4);
  uint32_t color[4], clip[4];
  for (uint32_t i = 0; i < 4; i++) {
    color[i] = guest::LoadU32(c.red_address() + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    clip[i] = guest::LoadU32(c.clip_left_address() + 4 * i);
  }
  // The layer as an unsigned 32-bit integer (std then lfd, fcfid, frsp).
  const double depth = double(float(double(int64_t(uint64_t(layer)))));
  item.set_depth(float(Fmuls(depth, double(G::layer_depth()))));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(item.red_address() + 4 * i, color[i]);
  }
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(item.clip_0_address() + 4 * i, clip[i]);
  }
  uint32_t appended = 0;
  AppendText(c.text_list().address, item.address, &appended);
  *r3 = ClearTextItem(item.address);
  return true;
}

namespace {

// fctidz, as the translation does it: NaN gives INT64_MIN, above INT64_MAX
// INT64_MAX, else truncated (out of range INT64_MIN).
int64_t Fctidz(double x) {
  if (std::isnan(x)) {
    return INT64_MIN;
  }
  if (x > double(INT64_MAX)) {
    return INT64_MAX;
  }
  return simde_mm_cvttsd_si64(simde_mm_set_sd(x));
}

// A word as lfd of its std (zero-extended) then fcfid then frsp.
double UnsignedToFloat(uint32_t n) {
  return double(float(double(int64_t(uint64_t(n)))));
}

// The texture the context's table (at [[context + 96]]) holds at the index
// in `entry`.
uint32_t TextureAt(uint32_t context, uint32_t entry) {
  const uint32_t index = schema::UiMovieTextureEntry(entry).texture() << 2;
  const uint32_t table = Context(context).textures().data();
  return guest::LoadU32(table + index);
}

// The quad's clip (+100 the flag, +104..+110 the rectangle as 16-bit
// integers, sorted) from the context's (+64..+76; +72 isn't checked for
// "none", as read).
void QuadClip(uint32_t context, uint32_t quad) {
  const Context c(context);
  const Quad q(quad);
  const uint32_t c64 = c.clip_left();
  if (c64 == 0 && c.clip_top() == 0 && c.clip_bottom() == 0) {
    q.set_clipped(0);
    q.set_clip_0(0);
    q.set_clip_1(0);
    q.set_clip_2(0);
    q.set_clip_3(0);
    return;
  }
  const int16_t left = int16_t(c64);
  const int16_t right = int16_t(c.clip_right());
  const int16_t top = int16_t(c.clip_top());
  const int16_t bottom = int16_t(c.clip_bottom());
  q.set_clip_0(uint16_t(left));
  q.set_clipped(1);
  q.set_clip_2(uint16_t(right));
  q.set_clip_1(uint16_t(top));
  q.set_clip_3(uint16_t(bottom));
  if (left > right) {
    q.set_clip_0(uint16_t(right));
    q.set_clip_2(uint16_t(left));
  }
  if (top > bottom) {
    q.set_clip_1(uint16_t(bottom));
    q.set_clip_3(uint16_t(top));
  }
}

// Swaps the 16-byte texture coordinate entries a and b.
void SwapEntries(uint32_t a, uint32_t b) {
  uint32_t ta[4], tb[4];
  for (uint32_t i = 0; i < 4; i++) {
    ta[i] = guest::LoadU32(a + 4 * i);
    tb[i] = guest::LoadU32(b + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(a + 4 * i, tb[i]);
    guest::StoreU32(b + 4 * i, ta[i]);
  }
}

// The vertices' w (+12 of each of the 16 positions from `positions`) = 1.0.
void SetWs(uint32_t positions) {
  const uint32_t one = guest::LoadU32(G::one_address());
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(positions + 12 + 16 * i, one);
  }
}

}  // namespace

uint32_t PictureTextureOf(uint32_t context, uint32_t picture) {
  if (guest::LoadU32(picture + 140) == 0) {
    return 0;
  }
  return TextureAt(context, PictureTexture(picture, Node(picture).texture_entry()));
}

QuadCall BuildPicture(uint32_t context, uint32_t picture, uint32_t layer,
                      uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8259ED40 UiMovie::DrawPicture");
  const Quad q(frame + 112);
  const uint32_t quad = q.address;
  const uint32_t positions = frame + 240, colors = frame + 496, coords = frame + 752;
  const uint32_t one = guest::LoadU32(G::one_address());
  const uint32_t zero = guest::LoadU32(G::zero_address());
  SetWs(positions);
  MakeQuad(quad);
  ApplyTransform(context, picture, frame - 352);
  q.set_vertex_count(4);
  q.set_positions(positions);
  q.set_colors(colors);
  q.set_coords(coords);
  const uint32_t texture = PictureTextureOf(context, picture);
  q.set_texture(guest::LoadU32(texture));
  // The colors, each vertex the context's, as integers.
  const Context ctx(context);
  const double scale = double(G::color_to_integer());
  for (uint32_t v = 0; v < 64; v += 16) {
    for (uint32_t c = 0; c < 16; c += 4) {
      const double value = Fmuls(Lfs(ctx.red_address() + c), scale);
      guest::StoreU32(colors + v + c, uint32_t(uint64_t(Fctidz(value))));
    }
  }
  // The corners (0, 0), (+100, 0), (0, +104), (+100, +104), transformed.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t p = positions + 16 * i;
    if (i & 1) {
      CopyWord(Node(picture).width_address(), p + 0);
    } else {
      guest::StoreU32(p + 0, zero);
    }
    if (i & 2) {
      CopyWord(Node(picture).height_address(), p + 4);
    } else {
      guest::StoreU32(p + 4, zero);
    }
    guest::StoreU32(p + 8, zero);
    guest::StoreU32(p + 12, one);
  }
  for (uint32_t i = 0; i < 4; i++) {
    debug::Vec4Transform(positions + 16 * i, context, positions + 16 * i);
  }
  // The texture coordinates: the pixel rectangle +116..+128 over the
  // texture's size, else (a size of 0) 0 to 1.
  const uint32_t width = guest::LoadU32(texture + 4);
  if (width != 0 && guest::LoadU32(texture + 8) != 0) {
    const auto ratio = [&](uint32_t pixel, uint32_t size_at) {
      const double size = UnsignedToFloat(guest::LoadU32(texture + size_at));
      return Fdivs(IntToFloat(int32_t(guest::LoadU32(picture + pixel))), size);
    };
    Stfs(coords + 0, ratio(116, 4));
    Stfs(coords + 4, ratio(120, 8));
    Stfs(coords + 16, ratio(124, 4));
    Stfs(coords + 20, ratio(120, 8));
    Stfs(coords + 32, ratio(116, 4));
    Stfs(coords + 36, ratio(128, 8));
    Stfs(coords + 48, ratio(124, 4));
    Stfs(coords + 52, ratio(128, 8));
  } else {
    guest::StoreU32(coords + 0, zero);
    guest::StoreU32(coords + 4, zero);
    guest::StoreU32(coords + 16, one);
    guest::StoreU32(coords + 20, zero);
    guest::StoreU32(coords + 32, zero);
    guest::StoreU32(coords + 36, one);
    guest::StoreU32(coords + 48, one);
    guest::StoreU32(coords + 52, one);
  }
  if (Node(picture).flip_x() != 0) {  // flipped across
    SwapEntries(coords + 0, coords + 16);
    SwapEntries(coords + 32, coords + 48);
  }
  if (Node(picture).flip_y() != 0) {  // flipped down
    SwapEntries(coords + 0, coords + 32);
    SwapEntries(coords + 16, coords + 48);
  }
  QuadClip(context, quad);
  q.set_blend_mode(0);
  q.set_unknown_72(0);
  q.set_texture_stage(ctx.scaled());
  const double depth = Fmuls(UnsignedToFloat(layer), double(G::layer_depth()));
  return {quad, Node(picture).layer_offset(), depth};
}

QuadCall BuildPrimitive(uint32_t context, uint32_t primitive, uint32_t layer,
                        uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8259F348 UiMovie::DrawPrimitive");
  const Quad q(frame + 96);
  const uint32_t quad = q.address;
  const Context ctx(context);
  const uint32_t positions = frame + 272, colors = frame + 528, coords = frame + 784;
  const uint32_t one = guest::LoadU32(G::one_address());
  const uint32_t zero = guest::LoadU32(G::zero_address());
  SetWs(positions);
  MakeQuad(quad);
  ApplyTransform(context, primitive, frame - 352);
  q.set_positions(positions);
  q.set_vertex_count(guest::LoadU32(primitive + 116));
  q.set_colors(colors);
  q.set_coords(coords);
  uint32_t texture = 0;
  if (Node(primitive).unknown_112() != 0) {
    // +104 read as a word: a primitive's texture entry.
    const uint32_t entry = guest::LoadU32(Node(primitive).height_address());
    texture = TextureAt(context, PrimitiveTexture(primitive, entry));
  }
  q.set_texture(texture != 0 ? guest::LoadU32(texture) : 0);
  const double to_integer = double(G::color_to_integer());
  const double scale = double(G::color_scale());
  for (uint32_t i = 0; i < guest::LoadU32(primitive + 116); i++) {
    const uint32_t vertex = PrimitiveVertex(primitive, i);
    const uint32_t p = positions + 16 * i, c = colors + 16 * i, t = coords + 16 * i;
    CopyWord(Vertex(vertex).x_address(), p + 0);
    CopyWord(Vertex(vertex).y_address(), p + 4);
    guest::StoreU32(p + 8, zero);
    guest::StoreU32(p + 12, one);
    debug::Vec4Transform(p, context, p);
    if (texture != 0) {
      Stfs(t + 0,
           Fdivs(IntToFloat(int32_t(Vertex(vertex).unknown_24())),
                 UnsignedToFloat(guest::LoadU32(texture + 4))));
      Stfs(t + 4,
           Fdivs(IntToFloat(int32_t(Vertex(vertex).unknown_28())),
                 UnsignedToFloat(guest::LoadU32(texture + 8))));
    } else {
      guest::StoreU32(t + 0, zero);
      guest::StoreU32(t + 4, zero);
    }
    // The vertex's color (integers, then its float alpha) times the
    // context's, squared with +100 set, as integers.
    const double alpha =
        Fmuls(Lfs(Vertex(vertex).alpha_address()), double(ctx.alpha()));
    double r = Fmuls(Fmuls(IntToFloat(int32_t(Vertex(vertex).red())), scale),
                     double(ctx.red()));
    double g = Fmuls(Fmuls(IntToFloat(int32_t(Vertex(vertex).green())), scale),
                     double(ctx.green()));
    double b = Fmuls(Fmuls(IntToFloat(int32_t(Vertex(vertex).blue())), scale),
                     double(ctx.blue()));
    if (guest::LoadU32(primitive + 100) != 0) {
      r = Fmuls(r, r);
      g = Fmuls(g, g);
      b = Fmuls(b, b);
    }
    guest::StoreU32(c + 0, uint32_t(uint64_t(Fctidz(Fmuls(r, to_integer)))));
    guest::StoreU32(c + 4, uint32_t(uint64_t(Fctidz(Fmuls(g, to_integer)))));
    guest::StoreU32(c + 8, uint32_t(uint64_t(Fctidz(Fmuls(b, to_integer)))));
    guest::StoreU32(c + 12, uint32_t(uint64_t(Fctidz(Fmuls(alpha, to_integer)))));
  }
  QuadClip(context, quad);
  q.set_unknown_72(0);
  q.set_blend_mode(guest::LoadU32(primitive + 100));
  q.set_texture_stage(ctx.scaled());
  const double depth = Fmuls(UnsignedToFloat(layer), double(G::layer_depth()));
  return {quad, Node(primitive).layer_offset(), depth};
}

uint32_t DrawNode(uint32_t context, uint32_t scene, uint32_t node, uint64_t layer,
                  uint32_t stack, DrawNodeCall call, void* user) {
  ITC_NATIVE_RUNNING("sub_8259F848 UiMovie::DrawNode");
  constexpr uint32_t kGrp = 0x47525000, kPic = 0x50494300, kPrm = 0x50524D00,
                     kSce = 0x53434500, kTxt = 0x54585400;
  const uint32_t frame = stack - 272;
  const uint32_t copy = frame + 80;
  uint32_t r3 = ApplyTransform(context, node, frame - 352);
  const uint32_t first = guest::LoadU32(Node(node).width_address());
  int32_t i = int32_t(guest::LoadU32(Node(node).height_address()) + first - 1);
  if (i < int32_t(first)) {
    return r3;
  }
  const double threshold = double(G::near_zero());
  const double zero = double(G::zero());
  do {
    const uint32_t child = Child(scene, uint32_t(i));
    r3 = child;
    if (Node(child).visible() != 0 &&
        BeyondThreshold(Lfs(Node(child).scale_x_address()), zero, threshold) &&
        BeyondThreshold(Lfs(Node(child).scale_y_address()), zero, threshold)) {
      r3 = CopyDrawContext(copy, context);
      const uint32_t tag = Node(child).tag();
      const auto child_layer = [&] { return uint64_t(Node(child).layer_offset()) + layer; };
      if (tag == kGrp) {
        r3 = DrawNode(copy, scene, child, child_layer(), frame, call, user);
      } else if (tag == kPic) {
        r3 = call(user, DrawCall::kPicture, copy, scene, child, child_layer(), frame, 0x8259F9C0);
      } else if (tag == kPrm) {
        r3 = call(user, DrawCall::kPrimitive, copy, scene, child, child_layer(), frame,
                  0x8259F9A4);
      } else if (tag == kSce) {
        r3 = DrawNode(copy, child, child, child_layer(), frame, call, user);
      } else if (tag == kTxt) {
        const uint64_t text_layer = child_layer();
        uint32_t queued = 0;
        if ((text_layer >> 32) == 0 &&
            QueueText(copy, child, uint32_t(text_layer), frame - 272, &queued)) {
          r3 = queued;
        } else {
          r3 = call(user, DrawCall::kText, copy, scene, child, text_layer, frame, 0x8259F980);
        }
      }
    }
    i--;
  } while (!(i < int32_t(guest::LoadU32(Node(node).width_address()))));
  return r3;
}

}  // namespace ui_movies
