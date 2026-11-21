// The car scene draw's small leaves (docs/functions/car-scene-draw.md): the
// pass description's light, the scene models' beams and the player's model,
// step for step as the translated code (see car_scene_draw.h).

#include "car_scene_draw/car_scene_draw.h"

#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "material_passes/material_passes.h"
#include "native/buffers.h"
#include "schema/car_draw.h"
#include "schema/car_scene_draw.h"
#include "schema/race_states.h"
#include "schema/scene_models.h"

namespace car_scene_draw {

namespace {

using namespace ops;

using G = schema::CarSceneDrawGlobals;

constexpr uint32_t kSetLightFrame = 192;
constexpr uint32_t kBeamColorFrame = 112;
constexpr uint32_t kSetLightMemcpyReturn = 0x820C4F54;

// The scene model's light bits (its draw block's flags, +4292, read with ld
// at +4288) have the high beams (0x40).
bool HighBeams(uint8_t* base, uint32_t model) {
  const schema::SceneModel m(base, model);
  return (schema::CarDrawBlock(base, m.draw_block_address()).flags() & 0x40) != 0;
}

}  // namespace

uint32_t ScenePassSetLight(uint8_t* base, uint32_t pass, uint32_t direction, double distance,
                           uint32_t stack, const MemcpyCall& memcpy) {
  ITC_NATIVE_RUNNING("sub_820C4EB8 ScenePass::SetLight");
  const uint32_t f = stack - kSetLightFrame;
  const schema::ScenePass p(base, pass);
  Store(base, f, stack);  // the back chain, as stwu
  const simde__m128 v11 = Lvx(base, direction);
  {
    FlushToZero flush;
    const simde__m128 dot = Dot3(v11, v11);
    // The identity's rows, as the translation builds them (vupkd3d128 of 0
    // and four vpermwi128).
    const simde__m128 v13 = simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f);
    Stvx(base, f + 96, Permute<0x15>(v13));
    Stvx(base, f + 112, Permute<0x45>(v13));
    Stvx(base, f + 128, Permute<0x51>(v13));
    Stvx(base, f + 144, Permute<0x54>(v13));
    Stvx(base, p.light_x_address(), Normalized(v11, dot));
  }
  // The identity to +112.
  if (!native::Buffers_Memcpy(base, p.light_matrix_address(), f + 96, 64,
                              kSetLightMemcpyReturn)) {
    memcpy(p.light_matrix_address(), f + 96, 64, f, kSetLightMemcpyReturn);
  }
  // Its last row (+160): minus the distance times the direction, w 1.0.
  const double minus = Fneg(distance);
  double x = double(p.light_x());
  double y = double(p.light_y());
  const double one = double(G::one(base));
  Stfs(base, f + 92, one);
  x = Fmuls(x, minus);
  Stfs(base, f + 80, x);
  const double z = double(p.light_z());
  y = Fmuls(y, minus);
  Stfs(base, f + 84, y);
  Stfs(base, f + 88, Fmuls(z, minus));
  Copy16(base, p.light_shift_xy_address(), f + 80);
  p.set_light_shift_w(float(one));
  return p.light_matrix_address();
}

uint32_t SceneModelBeamColor(uint8_t* base, uint32_t out, uint32_t model, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DAA38 SceneModel::BeamColor");
  const uint32_t f = stack - kBeamColorFrame;
  Store(base, f, stack);  // the back chain, as stwu
  const bool high = HighBeams(base, model);
  const schema::SceneModel m(base, model);
  const uint32_t sub = m.parts_aero7();
  const uint32_t index = m.block();
  if (!high) {
    material_passes::TableVector(base, out, index, sub);
    return out;
  }
  material_passes::TableVector(base, f + 80, index, sub);
  double x = Lfs(base, f + 80);
  double y = Lfs(base, f + 84);
  const double scale = double(G::beam_scale(base));
  x = Fmuls(x, scale);
  Stfs(base, out + 0, x);
  double z = Lfs(base, f + 88);
  y = Fmuls(y, scale);
  z = Fmuls(z, scale);
  Stfs(base, out + 4, y);
  Stfs(base, out + 8, z);
  Stfs(base, out + 12, double(G::one(base)));
  return out;
}

void SceneModelBeam(uint8_t* base, uint32_t out, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820DAAC8 SceneModel::Beam");
  const bool high = HighBeams(base, model);
  Copy16Interleaved(base, out, high ? G::scene(base).high_beam_direction_lo_address()
                                       : G::scene(base).low_beam_direction_address());
}

void SceneModelBeamShape(uint8_t* base, uint32_t out, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820DAB10 SceneModel::BeamShape");
  const bool high = HighBeams(base, model);
  Copy16Interleaved(base, out, high ? G::scene(base).high_beam_third_lo_address()
                                       : G::scene(base).low_beam_third_address());
}

uint32_t CarScenePlayerModel(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C4FB8 CarScene::PlayerModel");
  if (index != 0) {
    return index;
  }
  const schema::RaceData race = G::race_data(base);
  if (race.kind() != 6 || race.added_count() != 2) {
    return 0;
  }
  return G::second_human(base) == 0 ? 1 : 0;
}

}  // namespace car_scene_draw
