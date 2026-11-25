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
bool HighBeams(uint32_t model) {
  const schema::SceneModel m(model);
  return (schema::CarDrawBlock(m.draw_block_address()).flags() & 0x40) != 0;
}

}  // namespace

uint32_t ScenePassSetLight(uint32_t pass, uint32_t direction, double distance,
                           uint32_t stack, const MemcpyCall& memcpy) {
  ITC_NATIVE_RUNNING("sub_820C4EB8 ScenePass::SetLight");
  const uint32_t f = stack - kSetLightFrame;
  const schema::ScenePass p(pass);
  Store(f, stack);  // the back chain, as stwu
  const simde__m128 v11 = Lvx(direction);
  {
    FlushToZero flush;
    const simde__m128 dot = Dot3(v11, v11);
    // The identity's rows, as the translation builds them (vupkd3d128 of 0
    // and four vpermwi128).
    const simde__m128 v13 = simde_mm_set_ps(3.0f, 3.0f, 0.0f, 1.0f);
    Stvx(f + 96, Permute<0x15>(v13));
    Stvx(f + 112, Permute<0x45>(v13));
    Stvx(f + 128, Permute<0x51>(v13));
    Stvx(f + 144, Permute<0x54>(v13));
    Stvx(p.light_x_address(), Normalized(v11, dot));
  }
  // The identity to +112.
  if (!native::Buffers_Memcpy(p.light_matrix_address(), f + 96, 64,
                              kSetLightMemcpyReturn)) {
    memcpy(p.light_matrix_address(), f + 96, 64, f, kSetLightMemcpyReturn);
  }
  // Its last row (+160): minus the distance times the direction, w 1.0.
  const double minus = Fneg(distance);
  double x = double(p.light_x());
  double y = double(p.light_y());
  const double one = double(G::one());
  Stfs(f + 92, one);
  x = Fmuls(x, minus);
  Stfs(f + 80, x);
  const double z = double(p.light_z());
  y = Fmuls(y, minus);
  Stfs(f + 84, y);
  Stfs(f + 88, Fmuls(z, minus));
  Copy16(p.light_shift_xy_address(), f + 80);
  p.set_light_shift_w(float(one));
  return p.light_matrix_address();
}

uint32_t SceneModelBeamColor(uint32_t out, uint32_t model, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DAA38 SceneModel::BeamColor");
  const uint32_t f = stack - kBeamColorFrame;
  Store(f, stack);  // the back chain, as stwu
  const bool high = HighBeams(model);
  const schema::SceneModel m(model);
  const uint32_t sub = m.parts_aero7();
  const uint32_t index = m.block();
  if (!high) {
    material_passes::TableVector(out, index, sub);
    return out;
  }
  material_passes::TableVector(f + 80, index, sub);
  double x = Lfs(f + 80);
  double y = Lfs(f + 84);
  const double scale = double(G::beam_scale());
  x = Fmuls(x, scale);
  Stfs(out + 0, x);
  double z = Lfs(f + 88);
  y = Fmuls(y, scale);
  z = Fmuls(z, scale);
  Stfs(out + 4, y);
  Stfs(out + 8, z);
  Stfs(out + 12, double(G::one()));
  return out;
}

void SceneModelBeam(uint32_t out, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820DAAC8 SceneModel::Beam");
  const bool high = HighBeams(model);
  Copy16Interleaved(out, high ? G::scene().high_beam_direction_lo_address()
                                       : G::scene().low_beam_direction_address());
}

void SceneModelBeamShape(uint32_t out, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820DAB10 SceneModel::BeamShape");
  const bool high = HighBeams(model);
  Copy16Interleaved(out, high ? G::scene().high_beam_third_lo_address()
                                       : G::scene().low_beam_third_address());
}

uint32_t CarScenePlayerModel(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C4FB8 CarScene::PlayerModel");
  if (index != 0) {
    return index;
  }
  const schema::RaceData race = G::race_data();
  if (race.kind() != 6 || race.added_count() != 2) {
    return 0;
  }
  return G::second_human() == 0 ? 1 : 0;
}

}  // namespace car_scene_draw
