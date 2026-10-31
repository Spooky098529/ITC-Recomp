// The race scene's object (RaceScene::, docs/functions/race-scene.md) as
// plain C++ over guest memory: its making. See race_scene.h.

#include "race_scene/race_scene.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "post_effects/post_effects.h"
#include "scene_models/scene_models.h"

#include <bit>

namespace race_scene {

namespace {

constexpr uint32_t kOne = 0x82000A30;  // 1.0 (as the image holds it)

// lfs and stfs: a float word through a double, as the translated code.
double Lfs(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

double Lfd(uint8_t* base, uint32_t address) {
  const uint64_t bits = (uint64_t(guest::LoadU32(base, address)) << 32) |
                        guest::LoadU32(base, address + 4);
  return std::bit_cast<double>(bits);
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
  const double one = Lfs(base, kOne);
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
  guest::StoreU32(base, kScene, 0);
  const auto block = memory::crt::OperatorNew(base, kSceneBytes);
  if (!block) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_8215D738 RaceScene::Create");
  guest::StoreU32(base, kScene, *block != 0 ? SceneConstruct(base, *block) : 0);
  return true;
}

// --- The light set ---

void LightsInitEntry(uint8_t* base, uint32_t entry, int32_t index) {
  ITC_NATIVE_RUNNING("sub_8215C368 RaceLights::InitEntry");
  if (index >= 11) {
    return;
  }
  guest::StoreU32(base, entry + 0, 0);
  const double c = Lfs(base, kOne);
  const double z = Lfs(base, 0x82000A34);  // 0.0
  if (index < 3) {
    const double e = Lfs(base, 0x82001070);
    guest::StoreU32(base, entry + 0, 1);
    Stfs4(base, entry + 32, e, e, e, c);
    Stfs4(base, entry + 48, c, c, c, c);
  } else {
    guest::StoreU32(base, entry + 0, 2);
    Stfs4(base, entry + 32, Lfs(base, 0x82001BE4), z, z, c);
    Stfs4(base, entry + 48, c, c, c, c);
    Stfs4(base, entry + 64, Lfs(base, 0x82002578), Lfs(base, 0x8200222C), c, c);
    const double inner = c_runtime::Cos(Lfd(base, 0x82009790));
    const double outer = c_runtime::Cos(Lfd(base, 0x82002FE0));
    Stfs4(base, entry + 80, double(float(outer)), double(float(inner)), z, c);
  }
  const double p = Lfs(base, 0x82001FC0);
  const double q = Lfs(base, 0x82009798);
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

double LightsLastF1(uint8_t* base) { return c_runtime::Cos(Lfd(base, 0x82002FE0)); }

// --- The post effects' parameters ---

namespace {

constexpr uint32_t kGlowSetters = 0x82136EF8;  // slot 2; 8 bytes apart to slot 12
constexpr uint32_t kGlowFirstOffset = 8436;
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

// The 11 setters' calls on one glow.
void GlowSetAll(uint8_t* base, uint32_t glow, uint32_t entry) {
  for (uint32_t k = 0; k < 11; k++) {
    post_effects::GlowSet(base, glow, kGlowFirstOffset + 4 * k, Lfs(base, entry + 4 * k));
  }
}

}  // namespace

bool SplitScreen(uint8_t* base) {
  return guest::LoadU32(base, guest::LoadU32(base, 0x826C2CE8) + 8) != 0;
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
  const uint32_t entry = int32_t(kind) < 8 ? 0x82637CC0 + 44 * (3 * kind + index) : 0x82637C94;
  GlowsSetParams(base, entry);
  const double value = Lfs(base, 0x82002204);
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

}  // namespace race_scene
