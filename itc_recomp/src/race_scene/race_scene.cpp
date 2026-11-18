// The race scene's object (RaceScene::, docs/functions/race-scene.md) as
// plain C++ over guest memory: its making. See race_scene.h.

#include "race_scene/race_scene.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "c_runtime/math.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "post_effects/post_effects.h"
#include "scene_models/scene_models.h"
#include "schema/race_scene.h"

namespace race_scene {

namespace {

using G = schema::RaceSceneGlobals;

// stfs: a float word through a double, as the translated code.
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

// A 16-byte vector of four doubles stored as floats.
void Stfs4(uint8_t* base, uint32_t address, double x, double y, double z, double w) {
  Stfs(base, address + 0, x);
  Stfs(base, address + 4, y);
  Stfs(base, address + 8, z);
  Stfs(base, address + 12, w);
}

}  // namespace

uint32_t SceneConstruct(uint8_t* base, uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_8215D6C8 RaceScene::RaceScene");
  for (uint32_t i = 0; i < 7; i++) {
    scene_models::SceneModelRecordConstruct(base, scene + 48 + 4288 * i);
  }
  const double one = double(G::one(base));
  for (uint32_t i = 0; i < 11; i++) {
    const uint32_t at = scene + 30080 + 44 + 96 * i;
    Stfs(base, at - 16, one);
    Stfs(base, at + 0, one);
    Stfs(base, at + 16, one);
    Stfs(base, at + 32, one);
    Stfs(base, at + 48, one);
  }
  return scene;
}

bool SceneCreate(uint8_t* base) {
  G::set_scene(base, 0);
  const auto block = memory::crt::OperatorNew(base, kSceneBytes);
  if (!block) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8215D738 RaceScene::Create");
  G::set_scene(base, *block != 0 ? SceneConstruct(base, *block) : 0);
  return true;
}

// --- The light set ---

void LightsInitEntry(uint8_t* base, uint32_t entry, int32_t index) {
  ITC_NATIVE_RUNNING("sub_8215C368 RaceLights::InitEntry");
  if (index >= 11) {
    return;
  }
  guest::StoreU32(base, entry + 0, 0);
  const double c = double(G::one(base));
  const double z = double(G::zero(base));
  if (index < 3) {
    const double e = double(G::light_kind1_value(base));
    guest::StoreU32(base, entry + 0, 1);
    Stfs4(base, entry + 32, e, e, e, c);
    Stfs4(base, entry + 48, c, c, c, c);
  } else {
    guest::StoreU32(base, entry + 0, 2);
    Stfs4(base, entry + 32, double(G::light_kind2_x(base)), z, z, c);
    Stfs4(base, entry + 48, c, c, c, c);
    Stfs4(base, entry + 64, double(G::light_kind2_64_x(base)), double(G::light_kind2_64_y(base)),
          c, c);
    const double inner = c_runtime::Cos(G::light_cone_inner(base));
    const double outer = c_runtime::Cos(G::light_cone_outer(base));
    Stfs4(base, entry + 80, double(float(outer)), double(float(inner)), z, c);
  }
  const double p = double(G::light_color_p(base));
  const double q = double(G::light_color_q(base));
  switch (index) {
    case 1:
      Stfs4(base, entry + 16, p, p, q, c);
      break;
    case 2:
      Stfs4(base, entry + 16, q, p, p, c);
      break;
    case 3:
      Stfs4(base, entry + 16, p, p, p, c);
      break;
    case 4:
      Stfs4(base, entry + 16, z, p, q, c);
      break;
    case 5:
      Stfs4(base, entry + 16, q, p, z, c);
      break;
    case 6:
      Stfs4(base, entry + 16, p, p, z, c);
      break;
    default:
      Stfs4(base, entry + 16, q, p, q, c);
      break;
  }
}

void LightsSetUp(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_8215CF78 RaceLights::SetUp");
  guest::StoreU32(base, set + 0, 0);
  guest::StoreU32(base, set + 1072, 0);
  guest::StoreU32(base, set + 8, 0);
  guest::StoreU32(base, set + 4, 0);
  for (int32_t i = 0; i < 11; i++) {
    const uint32_t entry = set + 16 + 96 * uint32_t(i);
    LightsInitEntry(base, entry, i);
    const uint32_t kind = guest::LoadU32(base, entry + 0);
    if (kind == 1) {
      guest::StoreU32(base, set + 4, guest::LoadU32(base, set + 4) + 1);
    } else if (kind == 2) {
      guest::StoreU32(base, set + 8, guest::LoadU32(base, set + 8) + 1);
    } else {
      continue;
    }
    guest::StoreU32(base, set + 0, guest::LoadU32(base, set + 0) + 1);
  }
}

double LightsLastF1(uint8_t* base) { return c_runtime::Cos(G::light_cone_outer(base)); }

// --- The post effects' parameters ---

namespace {

constexpr uint32_t kGlowSetters = 0x82136EF8;  // slot 2; 8 bytes apart to slot 12
constexpr uint32_t kFilterSet8120 = 0x82135148;  // slot 8

bool GlowReady(uint8_t* base, uint32_t glow) {
  if (glow == 0) {
    return false;
  }
  const uint32_t vtable = guest::LoadU32(base, glow);
  for (uint32_t k = 2; k <= 12; k++) {
    if (guest::LoadU32(base, vtable + 4 * k) != kGlowSetters + 8 * (k - 2)) {
      return false;
    }
  }
  return true;
}

bool FilterReady(uint8_t* base, uint32_t filter) {
  return filter != 0 && guest::LoadU32(base, guest::LoadU32(base, filter) + 32) == kFilterSet8120;
}

// The 11 setters' calls on one glow (slots 2 to 12: the glow's +8436 to
// +8476), each float loaded before its call.
void GlowSetAll(uint8_t* base, uint32_t glow, uint32_t entry) {
  const schema::RaceGlowParams p(base, entry);
  post_effects::GlowSet(base, glow, 8436, double(p.c33_x_second()));
  post_effects::GlowSet(base, glow, 8440, double(p.c33_x_first()));
  post_effects::GlowSet(base, glow, 8444, double(p.c34_x()));
  post_effects::GlowSet(base, glow, 8448, double(p.c34_y()));
  post_effects::GlowSet(base, glow, 8452, double(p.strength()));
  post_effects::GlowSet(base, glow, 8456, double(p.c33_z()));
  post_effects::GlowSet(base, glow, 8460, double(p.history_weight()));
  post_effects::GlowSet(base, glow, 8464, double(p.param_8464()));
  post_effects::GlowSet(base, glow, 8468, double(p.star_step()));
  post_effects::GlowSet(base, glow, 8472, double(p.star_attenuation()));
  post_effects::GlowSet(base, glow, 8476, double(p.c33_w()));
}

}  // namespace

bool SplitScreen(uint8_t* base) {
  return guest::LoadU32(base, G::viewports(base).address + 8) != 0;
}

bool EffectParamsReady(uint8_t* base) {
  namespace pe = post_effects;
  const uint32_t* glows;
  const uint32_t* filters;
  static constexpr uint32_t kSplitGlows[] = {5, 9}, kSplitFilters[] = {6, 10};
  static constexpr uint32_t kGlows[] = {1}, kFilters[] = {2};
  const bool split = SplitScreen(base);
  glows = split ? kSplitGlows : kGlows;
  filters = split ? kSplitFilters : kFilters;
  const uint32_t count = split ? 2 : 1;
  for (uint32_t i = 0; i < count; i++) {
    if (!GlowReady(base, guest::LoadU32(base, pe::kEffects + 4 * glows[i])) ||
        !FilterReady(base, guest::LoadU32(base, pe::kEffects + 4 * filters[i]))) {
      return false;
    }
  }
  return true;
}

uint32_t GlowsSetParams(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_8228F0B0 Glows::SetParams");
  if (SplitScreen(base)) {
    const uint32_t first = post_effects::Get(base, 5);
    const uint32_t second = post_effects::Get(base, 9);
    GlowSetAll(base, first, entry);
    GlowSetAll(base, second, entry);
    return second;
  }
  const uint32_t glow = post_effects::Get(base, 1);
  GlowSetAll(base, glow, entry);
  return glow;
}

uint32_t SetEffectParams(uint8_t* base, uint32_t kind, uint32_t index, double* f1) {
  ITC_NATIVE_RUNNING("sub_8228F5C0 RaceScene::SetEffectParams");
  const uint32_t entry =
      int32_t(kind) < 8
          ? G::glow_params_address() + schema::RaceGlowParams::kSize * (3 * kind + index)
          : G::glow_params_other_address();
  GlowsSetParams(base, entry);
  const double value = double(G::filter_value(base));
  *f1 = value;
  if (SplitScreen(base)) {
    const uint32_t first = post_effects::Get(base, 6);
    const uint32_t second = post_effects::Get(base, 10);
    post_effects::ColorFilterSet(base, first, 8120, value);
    post_effects::ColorFilterSet(base, second, 8120, value);
    return second;
  }
  const uint32_t filter = post_effects::Get(base, 2);
  post_effects::ColorFilterSet(base, filter, 8120, value);
  return filter;
}

// --- The machine's toggle ---

uint32_t TogglePause(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215C750 RaceSceneMachine::TogglePause");
  const schema::RaceSceneMachine m(base, machine);
  m.set_paused(m.paused() == 0 ? 1 : 0);
  return post_effects::ToggleRaceEffects(base, G::scene(base).address);
}

std::optional<uint32_t> Resume(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215D0D8 RaceSceneMachine::Resume");
  const schema::RaceSceneMachine m(base, machine);
  if (m.paused() == 0) {
    return std::nullopt;
  }
  m.set_paused(0);
  return post_effects::ToggleRaceEffects(base, G::scene(base).address);
}

// --- The scene's freeing ---

void DestroyCars(uint8_t* base, uint32_t cars) {
  ITC_NATIVE_RUNNING("sub_8215D548 RaceScene::DestroyCars");
  constexpr uint32_t kEmptyDestructor = 0x82106540;
  // Each slot's record (cars + 32 + 4,288 i): its arrays from the last.
  struct Array {
    uint32_t offset, size, count;
  };
  static constexpr Array kArrays[] = {
      {3264, 64, 6}, {2880, 64, 6}, {2496, 64, 6}, {2112, 64, 6}, {1728, 64, 6}, {1344, 64, 6},
      {960, 64, 6},  {576, 64, 6},  {384, 64, 3},  {192, 64, 3},  {128, 16, 3},  {80, 16, 3},
      {32, 16, 3},
  };
  for (int32_t i = 6; i >= 0; i--) {
    const uint32_t record = cars + 32 + 4288 * uint32_t(i);
    for (const Array& a : kArrays) {
      c_runtime::VectorDestruct(
          base, record + a.offset, a.size, a.count, kEmptyDestructor,
          [](void*, uint64_t, uint64_t) {
            ITC_LOG_ERROR("race_scene: the car slots' destructor isn't the empty one");
          },
          nullptr);
    }
  }
}

bool DestroyReady(uint8_t* base) {
  const uint32_t scene = G::scene(base).address;
  return scene == 0 || memory::IsHeapBlock(scene);
}

uint32_t Destroy(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8215D790 RaceScene::Destroy");
  const uint32_t scene = G::scene(base).address;
  if (scene != 0) {
    DestroyCars(base, scene + 16);
    memory::crt::OperatorDelete(scene);
  }
  return scene;
}

}  // namespace race_scene
