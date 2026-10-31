// The race scene's object (RaceScene::, docs/functions/race-scene.md) as
// plain C++ over guest memory: its making. See race_scene.h.

#include "race_scene/race_scene.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
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

}  // namespace race_scene
