// The race scene's object (RaceScene::, docs/functions/race-scene.md) as
// plain C++ over guest memory: its making. See race_scene.h.

#include "race_scene/race_scene.h"

#include <bit>

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "c_runtime/math.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/handles.h"
#include "memory/heap.h"
#include "post_effects/post_effects.h"
#include "scene_models/scene_models.h"
#include "schema/camera_modes.h"
#include "schema/post_effects.h"
#include "schema/race_scene.h"
#include "schema/race_update.h"

namespace race_scene {

namespace {

using G = schema::RaceSceneGlobals;

// stfs into a light entry's vector (a bytes field, guest order) at the
// entry's `offset`, a float word through a double, as the translated code;
// through the view (the entry is in host memory, E5 step 4).
void Stfs(const schema::RaceLightEntry& entry, uint32_t offset, double value) {
  entry.store_at(offset, 4, std::bit_cast<uint32_t>(float(value)));
}

// A 16-byte vector of four doubles stored as floats.
void Stfs4(const schema::RaceLightEntry& entry, uint32_t offset, double x, double y, double z,
           double w) {
  Stfs(entry, offset + 0, x);
  Stfs(entry, offset + 4, y);
  Stfs(entry, offset + 8, z);
  Stfs(entry, offset + 12, w);
}

// A field's offset in its view.
uint32_t At(const schema::View& view, uint32_t field_address) {
  return field_address - view.address;
}

}  // namespace

uint32_t SceneConstruct(uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_8215D6C8 RaceScene::RaceScene");
  const schema::RaceScene s(scene);
  for (uint32_t i = 0; i < 7; i++) {
    const schema::RaceCarSlot slot = s.car_slots(i);
    scene_models::SceneModelRecordConstruct(slot.record_address());
  }
  const double one = double(G::one());
  const schema::RaceLights lights = s.lights();
  for (uint32_t i = 0; i < 11; i++) {
    // The w of the entry's five vectors.
    const schema::RaceLightEntry e = lights.entries(i);
    Stfs(e, At(e, e.direction_address()) + 12, one);
    Stfs(e, At(e, e.second_address()) + 12, one);
    Stfs(e, At(e, e.third_address()) + 12, one);
    Stfs(e, At(e, e.fourth_address()) + 12, one);
    Stfs(e, At(e, e.cone_address()) + 12, one);
  }
  return scene;
}

bool SceneCreate() {
  G::set_scene(0);
  // Host memory (E5 step 4): the original's operator new(31,168).
  static_assert(schema::RaceScene::kSize == kSceneBytes);
  const uint32_t block = schema::RaceScene::New().address;
  ITC_NATIVE_RUNNING("sub_8215D738 RaceScene::Create");
  G::set_scene(SceneConstruct(block));
  return true;
}

// --- The light set ---

void LightsInitEntry(uint32_t entry_address, int32_t index) {
  ITC_NATIVE_RUNNING("sub_8215C368 RaceLights::InitEntry");
  if (index >= 11) {
    return;
  }
  const schema::RaceLightEntry entry(entry_address);
  entry.set_kind(0);
  const double c = double(G::one());
  const double z = double(G::zero());
  if (index < 3) {
    const double e = double(G::light_kind1_value());
    entry.set_kind(1);
    Stfs4(entry, At(entry, entry.second_address()), e, e, e, c);
    Stfs4(entry, At(entry, entry.third_address()), c, c, c, c);
  } else {
    entry.set_kind(2);
    Stfs4(entry, At(entry, entry.second_address()), double(G::light_kind2_x()), z, z, c);
    Stfs4(entry, At(entry, entry.third_address()), c, c, c, c);
    Stfs4(entry, At(entry, entry.fourth_address()), double(G::light_kind2_64_x()),
          double(G::light_kind2_64_y()), c, c);
    const double inner = c_runtime::Cos(G::light_cone_inner());
    const double outer = c_runtime::Cos(G::light_cone_outer());
    Stfs4(entry, At(entry, entry.cone_address()), double(float(outer)), double(float(inner)), z, c);
  }
  const double p = double(G::light_color_p());
  const double q = double(G::light_color_q());
  switch (index) {
    case 1:
      Stfs4(entry, At(entry, entry.direction_address()), p, p, q, c);
      break;
    case 2:
      Stfs4(entry, At(entry, entry.direction_address()), q, p, p, c);
      break;
    case 3:
      Stfs4(entry, At(entry, entry.direction_address()), p, p, p, c);
      break;
    case 4:
      Stfs4(entry, At(entry, entry.direction_address()), z, p, q, c);
      break;
    case 5:
      Stfs4(entry, At(entry, entry.direction_address()), q, p, z, c);
      break;
    case 6:
      Stfs4(entry, At(entry, entry.direction_address()), p, p, z, c);
      break;
    default:
      Stfs4(entry, At(entry, entry.direction_address()), q, p, q, c);
      break;
  }
}

void LightsSetUp(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_8215CF78 RaceLights::SetUp");
  const schema::RaceLights lights(set);
  lights.set_count(0);
  lights.set_unknown_1072(0);
  lights.set_kind2_count(0);
  lights.set_kind1_count(0);
  for (int32_t i = 0; i < 11; i++) {
    const schema::RaceLightEntry entry = lights.entries(uint32_t(i));
    LightsInitEntry(entry.address, i);
    const uint32_t kind = entry.kind();
    if (kind == 1) {
      lights.set_kind1_count(lights.kind1_count() + 1);
    } else if (kind == 2) {
      lights.set_kind2_count(lights.kind2_count() + 1);
    } else {
      continue;
    }
    lights.set_count(lights.count() + 1);
  }
}

double LightsLastF1() { return c_runtime::Cos(G::light_cone_outer()); }

// --- The post effects' parameters ---

namespace {

constexpr uint32_t kGlowSetters = 0x82136EF8;  // slot 2; 8 bytes apart to slot 12
constexpr uint32_t kFilterSet8120 = 0x82135148;  // slot 8

bool GlowReady(uint32_t glow) {
  if (glow == 0) {
    return false;
  }
  const schema::PostEffectVtable vtable = schema::PostEffect(glow).vtable();
  // Slots 2 to 12: Glow::Set8436 to Set8476.
  for (uint32_t k = 2; k <= 12; k++) {
    if (guest::LoadU32(vtable.c33_x_second_setter_address() + 4 * (k - 2)) !=
        kGlowSetters + 8 * (k - 2)) {
      return false;
    }
  }
  return true;
}

bool FilterReady(uint32_t filter) {
  // Slot 8 (+32): ColorFilter::Set8120 (PostEffectVtable names it only for the glow).
  return filter != 0 &&
         guest::LoadU32(schema::PostEffect(filter).vtable().address + 32) ==
             kFilterSet8120;
}

// The 11 setters' calls on one glow (slots 2 to 12: the glow's +8436 to
// +8476), each float loaded before its call.
void GlowSetAll(uint32_t glow, uint32_t entry) {
  const schema::RaceGlowParams p(entry);
  post_effects::GlowSet(glow, 8436, double(p.c33_x_second()));
  post_effects::GlowSet(glow, 8440, double(p.c33_x_first()));
  post_effects::GlowSet(glow, 8444, double(p.c34_x()));
  post_effects::GlowSet(glow, 8448, double(p.c34_y()));
  post_effects::GlowSet(glow, 8452, double(p.strength()));
  post_effects::GlowSet(glow, 8456, double(p.c33_z()));
  post_effects::GlowSet(glow, 8460, double(p.history_weight()));
  post_effects::GlowSet(glow, 8464, double(p.param_8464()));
  post_effects::GlowSet(glow, 8468, double(p.star_step()));
  post_effects::GlowSet(glow, 8472, double(p.star_attenuation()));
  post_effects::GlowSet(glow, 8476, double(p.c33_w()));
}

}  // namespace

bool SplitScreen() {
  return G::viewports().split() != 0;
}

bool EffectParamsReady() {
  using PE = schema::PostEffectsGlobals;
  const uint32_t* glows;
  const uint32_t* filters;
  static constexpr uint32_t kSplitGlows[] = {5, 9}, kSplitFilters[] = {6, 10};
  static constexpr uint32_t kGlows[] = {1}, kFilters[] = {2};
  const bool split = SplitScreen();
  glows = split ? kSplitGlows : kGlows;
  filters = split ? kSplitFilters : kFilters;
  const uint32_t count = split ? 2 : 1;
  for (uint32_t i = 0; i < count; i++) {
    if (!GlowReady(guest::LoadU32(PE::effects_address() + 4 * glows[i])) ||
        !FilterReady(guest::LoadU32(PE::effects_address() + 4 * filters[i]))) {
      return false;
    }
  }
  return true;
}

uint32_t GlowsSetParams(uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_8228F0B0 Glows::SetParams");
  if (SplitScreen()) {
    const uint32_t first = post_effects::Get(5);
    const uint32_t second = post_effects::Get(9);
    GlowSetAll(first, entry);
    GlowSetAll(second, entry);
    return second;
  }
  const uint32_t glow = post_effects::Get(1);
  GlowSetAll(glow, entry);
  return glow;
}

uint32_t SetEffectParams(uint32_t kind, uint32_t index, double* f1) {
  ITC_NATIVE_RUNNING("sub_8228F5C0 RaceScene::SetEffectParams");
  const uint32_t entry =
      int32_t(kind) < 8
          ? G::glow_params_address() + schema::RaceGlowParams::kSize * (3 * kind + index)
          : G::glow_params_other_address();
  GlowsSetParams(entry);
  const double value = double(G::filter_value());
  *f1 = value;
  if (SplitScreen()) {
    const uint32_t first = post_effects::Get(6);
    const uint32_t second = post_effects::Get(10);
    post_effects::ColorFilterSet(first, 8120, value);
    post_effects::ColorFilterSet(second, 8120, value);
    return second;
  }
  const uint32_t filter = post_effects::Get(2);
  post_effects::ColorFilterSet(filter, 8120, value);
  return filter;
}

// --- The machine's toggle ---

uint32_t TogglePause(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215C750 RaceSceneMachine::TogglePause");
  const schema::RaceSceneMachine m(machine);
  m.set_paused(m.paused() == 0 ? 1 : 0);
  return post_effects::ToggleRaceEffects(G::scene().address);
}

std::optional<uint32_t> Resume(uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215D0D8 RaceSceneMachine::Resume");
  const schema::RaceSceneMachine m(machine);
  if (m.paused() == 0) {
    return std::nullopt;
  }
  m.set_paused(0);
  return post_effects::ToggleRaceEffects(G::scene().address);
}

// --- The scene's freeing ---

void DestroyCars(uint32_t cars) {
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
  // `cars` is the scene's car slot count (+16); the slots follow at +32.
  const schema::RaceScene scene(cars - 16);
  for (int32_t i = 6; i >= 0; i--) {
    const uint32_t record = scene.car_slots(uint32_t(i)).record_address();
    for (const Array& a : kArrays) {
      c_runtime::VectorDestruct(record + a.offset, a.size, a.count, kEmptyDestructor,
          [](void*, uint64_t, uint64_t) {
            ITC_LOG_ERROR("race_scene: the car slots' destructor isn't the empty one");
          },
          nullptr);
    }
  }
}

bool DestroyReady() {
  const uint32_t scene = G::scene().address;
  return scene == 0 || memory::IsHeapBlock(scene);
}

uint32_t Destroy() {
  ITC_NATIVE_RUNNING("sub_8215D790 RaceScene::Destroy");
  const uint32_t scene = G::scene().address;
  if (scene != 0) {
    const schema::RaceScene object(scene);
    DestroyCars(object.car_slot_count_address());
    // The original's operator delete: host memory (E5 step 4, made by
    // SceneCreate or the lifted Create's host_alloc); a guest block only from
    // a lifted Create not yet re-emitted.
    if (memory::IsHandle(scene)) {
      object.Delete();
    } else {
      memory::crt::OperatorDelete(scene);
    }
  }
  return scene;
}

}  // namespace race_scene
