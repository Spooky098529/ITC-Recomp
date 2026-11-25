// sub_8211BEE8's quad as plain C++ (game.h; docs/functions/ui-movies-game.md).
// Built with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).

#include "ui_movies/game.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "debug/frame_state_diff.h"
#include "graphics/resources.h"
#include "schema/graphics_resources.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies_game.h"
#include "ui_movies/ui_movies.h"

#include <simde/x86/sse2.h>

#include <climits>
#include <cmath>

namespace ui_movies {

using namespace car_physics::ops;

namespace {

constexpr uint32_t kOne = 0x82000A30;         // 1.0
constexpr uint32_t kScale = 0x82001070;       // the scale matrix's
constexpr uint32_t kAlphaScale = 0x82000C44;  // the alpha's to an integer

constexpr uint32_t kPositions = 384, kColors = 640, kCoords = 896;
constexpr uint32_t kIdentity = 96, kMatrix = 320, kDesc = 288;

// fctiwz, as the translation does it (draw.cpp's).
int32_t Fctiwz(double x) {
  if (std::isnan(x)) {
    return INT32_MIN;
  }
  if (x >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  return simde_mm_cvttsd_si32(simde_mm_set_sd(x));
}

// A 64-bit integer as lfd then fcfid then frsp.
double ToFloat(int64_t n) {
  return double(float(double(n)));
}

void StoreU64(uint32_t address, uint64_t value) {
  guest::StoreU32(address, uint32_t(value >> 32));
  guest::StoreU32(address + 4, uint32_t(value));
}

}  // namespace

void EffectBegin(uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8211BEE8 (a full-frame effect drawn)");
  const double one = Lfs(kOne);
  for (uint32_t k = 0; k < 16; k++) {
    Stfs(frame + kPositions + 12 + 16 * k, one);
  }
  MakeQuad(frame + kEffectQuad);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(frame + kIdentity + 4 * i, i % 5 == 0 ? 0x3F800000u : 0);
  }
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(frame + kMatrix + i, guest::LoadU32(frame + kIdentity + i));
  }
  const double scale = Lfs(kScale);
  Stfs(frame + kMatrix, scale);
  Stfs(frame + kMatrix + 20, scale);
}

double EffectQuad(uint32_t frame, uint32_t primitive, uint32_t texture) {
  const schema::UiMovieQuad quad(frame + kEffectQuad);
  // The primitive node: its +116 is the vertex count (UiMovieNode's
  // texture_left word, read as an integer).
  const schema::UiMovieNode prim(primitive);
  const schema::LevelDesc desc(frame + kDesc);
  graphics::TextureLevelDesc(texture, 0, frame + kDesc);
  quad.set_texture(texture);
  quad.set_positions(frame + kPositions);
  quad.set_colors(frame + kColors);
  quad.set_coords(frame + kCoords);
  quad.set_vertex_count(guest::LoadU32(prim.texture_left_address()));
  const double zero = Lfs(schema::UiMoviesGameGlobals::zero_address());
  if (guest::LoadU32(prim.texture_left_address()) != 0) {
    const uint32_t width = desc.width();
    const uint32_t height = desc.height();
    StoreU64(frame + 88, width);
    StoreU64(frame + 80, height);
    const double alpha_scale = Lfs(kAlphaScale);
    const double w = ToFloat(int64_t(uint64_t(width)));
    const double h = ToFloat(int64_t(uint64_t(height)));
    const double one = Lfs(kOne);
    uint32_t k = 0;
    do {
      const uint32_t vertex = PrimitiveVertex(primitive, k);
      const uint32_t p = frame + kPositions + 16 * k;
      const uint32_t t = frame + kCoords + 16 * k;
      const uint32_t c = frame + kColors + 16 * k;
      const double x_offset = Lfs(prim.x_address());
      Stfs(p + 0, Fadds(Lfs(vertex + 0), x_offset));
      const double y_offset = Lfs(prim.y_address());
      Stfs(p + 4, Fadds(Lfs(vertex + 4), y_offset));
      Stfs(p + 8, zero);
      Stfs(p + 12, one);
      debug::Vec4Transform(p, frame + kMatrix, p);
      k++;
      const int32_t u = int32_t(guest::LoadU32(vertex + 24));
      StoreU64(frame + 80, uint64_t(int64_t(u)));
      Stfs(t + 0, Fdivs(ToFloat(u), w));
      const int32_t v = int32_t(guest::LoadU32(vertex + 28));
      StoreU64(frame + 88, uint64_t(int64_t(v)));
      Stfs(t + 4, Fdivs(ToFloat(v), h));
      guest::StoreU32(c + 0, prim.red());
      guest::StoreU32(c + 4, prim.green());
      guest::StoreU32(c + 8, prim.blue());
      guest::StoreU32(c + 12,
                      uint32_t(Fctiwz(Fmuls(Lfs(prim.alpha_address()), alpha_scale))));
      quad.set_clipped(0);
      quad.set_clip_0(0);
      quad.set_clip_1(0);
      quad.set_clip_2(0);
      quad.set_clip_3(0);
    } while (k < guest::LoadU32(prim.texture_left_address()));
  }
  quad.set_unknown_72(0);
  quad.set_blend_mode(4);
  return zero;
}

}  // namespace ui_movies
