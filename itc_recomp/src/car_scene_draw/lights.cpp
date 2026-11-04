// The car scene draw's small leaves (docs/functions/car-scene-draw.md): the
// pass description's light, the scene models' beams and the player's model,
// step for step as the translated code (see car_scene_draw.h).

#include "car_scene_draw/car_scene_draw.h"

#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "material_passes/material_passes.h"
#include "native/buffers.h"

namespace car_scene_draw {

namespace {

using namespace ops;

constexpr uint32_t kScene = 0x826C2E44;     // the car scene
constexpr uint32_t kRaceData = 0x826C5B2C;  // the race data
constexpr uint32_t kSecondHuman = 0x826C7824;  // the word Minimap::Draw reads for the human car
constexpr uint32_t kBeamScale = 0x820029D4;  // the high beams' color factor

constexpr uint32_t kSetLightFrame = 192;
constexpr uint32_t kBeamColorFrame = 112;
constexpr uint32_t kSetLightMemcpyReturn = 0x820C4F54;

// The scene model's light bits (its +4292, read with ld at +4288) have the
// high beams (0x40).
bool HighBeams(uint8_t* base, uint32_t model) { return (Load(base, model + 4292) & 0x40) != 0; }

}  // namespace

uint32_t ScenePassSetLight(uint8_t* base, uint32_t pass, uint32_t direction, double distance,
                           uint32_t stack, const MemcpyCall& memcpy) {
  ITC_NATIVE_RUNNING("sub_820C4EB8 ScenePass::SetLight");
  const uint32_t f = stack - kSetLightFrame;
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
    Stvx(base, pass + 96, Normalized(v11, dot));
  }
  // The identity to +112.
  if (!native::Buffers_Memcpy(base, pass + 112, f + 96, 64, kSetLightMemcpyReturn)) {
    memcpy(pass + 112, f + 96, 64, f, kSetLightMemcpyReturn);
  }
  // Its last row (+160): minus the distance times the direction, w 1.0.
  const double minus = Fneg(distance);
  double x = Lfs(base, pass + 96);
  double y = Lfs(base, pass + 100);
  const double one = Lfs(base, kOne);
  Stfs(base, f + 92, one);
  x = Fmuls(x, minus);
  Stfs(base, f + 80, x);
  const double z = Lfs(base, pass + 104);
  y = Fmuls(y, minus);
  Stfs(base, f + 84, y);
  Stfs(base, f + 88, Fmuls(z, minus));
  Copy16(base, pass + 160, f + 80);
  Stfs(base, pass + 172, one);
  return pass + 112;
}

uint32_t SceneModelBeamColor(uint8_t* base, uint32_t out, uint32_t model, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820DAA38 SceneModel::BeamColor");
  const uint32_t f = stack - kBeamColorFrame;
  Store(base, f, stack);  // the back chain, as stwu
  const bool high = HighBeams(base, model);
  const uint32_t sub = Load(base, model + 3776);
  const uint32_t index = Load(base, model + 48);
  if (!high) {
    material_passes::TableVector(base, out, index, sub);
    return out;
  }
  material_passes::TableVector(base, f + 80, index, sub);
  double x = Lfs(base, f + 80);
  double y = Lfs(base, f + 84);
  const double scale = Lfs(base, kBeamScale);
  x = Fmuls(x, scale);
  Stfs(base, out + 0, x);
  double z = Lfs(base, f + 88);
  y = Fmuls(y, scale);
  z = Fmuls(z, scale);
  Stfs(base, out + 4, y);
  Stfs(base, out + 8, z);
  Stfs(base, out + 12, Lfs(base, kOne));
  return out;
}

void SceneModelBeam(uint8_t* base, uint32_t out, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820DAAC8 SceneModel::Beam");
  const bool high = HighBeams(base, model);
  Copy16Interleaved(base, out, Load(base, kScene) + (high ? 5408 : 5392));
}

void SceneModelBeamShape(uint8_t* base, uint32_t out, uint32_t model) {
  ITC_NATIVE_RUNNING("sub_820DAB10 SceneModel::BeamShape");
  const bool high = HighBeams(base, model);
  Copy16Interleaved(base, out, Load(base, kScene) + (high ? 5472 : 5456));
}

uint32_t CarScenePlayerModel(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C4FB8 CarScene::PlayerModel");
  if (index != 0) {
    return index;
  }
  const uint32_t race = Load(base, kRaceData);
  if (Load(base, race + 0xA3AF0) != 6 || Load(base, race + 0x59614) != 2) {
    return 0;
  }
  return Load(base, kSecondHuman) == 0 ? 1 : 0;
}

}  // namespace car_scene_draw
