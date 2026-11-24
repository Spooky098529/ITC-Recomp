// The stand scene's teardown and the garage camera's presets load
// (docs/functions/type6-type7-modes.md) as plain C++ over the allocation
// layer: Viewports::Release's, XMemSet's and Archive::Toc's plain C++,
// Archive::Load through the caller's LoadCall.

#include "type67_modes/type67_modes.h"
#include "type67_modes/views.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "cameras/cameras.h"
#include "guest.h"
#include "memory/heap.h"
#include "scene/scene.h"
#include "schema/archive.h"
#include "schema/car_draw.h"
#include "schema/type6_type7_modes.h"
#include "schema/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type6Type7ModesGlobals;

// Whether a slot's blocks are all the layer's pages (StandSlot::Free frees
// its block and its camera's presets).
bool SlotIsPages(uint8_t* base, uint32_t slot) {
  if (!memory::IsPages(slot)) {
    return false;
  }
  const uint32_t block = schema::StandSlot(base, slot).stage<schema::View>().address;
  if (block == 0) {
    return true;
  }
  if (!memory::IsPages(block)) {
    return false;
  }
  const schema::GarageCamera camera(base, schema::GarageStage(base, block).camera_address());
  const uint32_t count = camera.preset_count();
  for (uint32_t i = 0; i < count; i++) {
    if (!memory::IsPages(LoadU32(base, camera.presets_address() + 4 * i))) {
      return false;
    }
  }
  return true;
}

}  // namespace

int64_t GarageCameraLoadPresets(uint8_t* base, uint32_t camera,
                                const std::function<int64_t(uint32_t, uint32_t, uint32_t,
                                                            uint32_t)>& load) {
  ITC_NATIVE_RUNNING("sub_821802D8 GarageCamera::LoadPresets");
  // The asset's entry: (22600 + 1) x 16 into the TOC.
  const schema::ArchiveTocEntry entry(base, archive::Toc(base, kPresetsAsset) + 0x60000 - 31600);
  uint32_t size = entry.unpacked_size();
  if (size == 0) {
    size = entry.stored_size();
  }
  // The asset's buffer, a blob (E5 step 5: PC memory in the guest's byte
  // order) in place of the original's VirtualAlloc (0 for 0 bytes, as the
  // layer's pages gave); GarageCamera::OnPresetsLoaded frees it.
  const uint32_t block = size == 0 ? 0 : schema::GaragePresetsData::New(base, size).address;
  c_runtime::XMemSet(base, block, 0, size);
  return load(kPresetsAsset, block, kOnPresetsLoaded, camera);
}

int64_t GarageCameraInit(uint8_t* base, uint32_t camera, uint32_t sp, const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180918 GarageCamera::Init");
  const uint32_t frame = sp - kGarageCameraInitFrame;
  GarageCameraReset(base, camera, frame - kGarageCameraResetFrame);
  const schema::GarageCamera cam(base, camera);
  cam.set_preset_count(0);
  cam.set_preset_table(0x82619428);
  cam.set_set_up(0);
  return GarageCameraLoadPresets(base, camera, load);
}

void GarageStageInit(uint8_t* base, uint32_t stage, uint32_t index, uint32_t sp,
                     const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180968 GarageStage::Init");
  const uint32_t f = sp - kGarageStageInitFrame;
  // lfs/stfs of the game's 0.0 and 1.0.
  using F = schema::StringsAndFontGlobals;
  const uint32_t zero = std::bit_cast<uint32_t>(float(double(F::zero(base))));
  const uint32_t one = std::bit_cast<uint32_t>(float(double(F::one(base))));
  const schema::GarageStage st(base, stage);
  StoreU32(base, f + 80, zero);
  st.set_index(index);
  StoreU32(base, f + 84, zero);
  st.set_model_claimed(0);
  StoreU32(base, f + 88, zero);
  st.set_pending_block(0xFFFFFFFF);
  st.set_reflection(1);
  StoreU32(base, f + 92, one);
  st.set_draw_word(1);
  StoreU32(base, st.unknown_12_address(), zero);  // the word as loaded (stfs)
  st.set_plates_shown(1);
  st.set_model_index(0xFFFFFFFF);
  st.set_dark(0);
  st.set_waiting_preset(0xFFFFFFFF);
  // +6064 to +6084, then the floats +6096 to +6160.
  for (uint32_t i = 0; i <= 20; i += 4) {
    StoreU32(base, st.unknown_6064_address() + i, 0);
  }
  for (uint32_t i = 0; i <= 64; i += 4) {
    StoreU32(base, st.front_axle_height_address() + i, zero);
  }
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreU32(base, st.front_axle_x_address() + i, LoadU32(base, f + 80 + i));
  }
  StoreU32(base, f + 96, zero);
  StoreU32(base, f + 100, zero);
  StoreU32(base, f + 104, zero);
  StoreU32(base, f + 108, one);
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreU32(base, st.rear_axle_x_address() + i, LoadU32(base, f + 96 + i));
  }
  GarageStageResetPose(base, st.bonnet_address(), f);
  GarageCameraInit(base, st.camera_address(), f, load);
  GarageLightsInit(base, st.lights_address(), f - kGarageLightsFrame);
  st.set_animation_count(0);
}

bool StandSlotInit(uint8_t* base, uint32_t slot, uint32_t index, uint32_t sp,
                   const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180AA0 StandSlot::Init");
  const schema::StandSlot s(base, slot);
  if (s.stage<schema::View>().address != 0) {
    return false;
  }
  const uint32_t stage = memory::AllocPages(kGarageStageBytes, 4096);
  s.set_stage(stage);
  GarageStageInit(base, stage, index, sp - kStandSlotInitFrame, load);
  return true;
}

uint64_t StandSceneCreate(uint8_t* base, uint32_t cars, uint32_t sp, const LoadCall& load,
                          const StandSceneCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82180B00 StandScene::Create");
  const uint32_t frame = sp - kStandSceneCreateFrame;
  StoreU32(base, frame, sp);
  calls.viewports_init(frame);
  const uint64_t created = calls.scene_create(frame);
  if (G::stand_holder<schema::View>(base).address != 0) {
    return created;
  }
  // A host object (E5 step 5) in place of the VirtualAlloc(24) at
  // 0x82180B3C; the global holds its handle.
  const uint32_t holder = schema::StandHolder::New(base).address;
  G::set_stand_holder(base, holder);
  schema::StandHolder(base, holder).set_unknown_12(0);
  G::stand_holder(base).set_unknown_16(0);
  G::stand_holder(base).set_model_made(0);
  // The three slots, stage_block_0 to 2.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = memory::AllocPages(4, 4096);
    // The holder's words through its view (a host object, E5 step 5).
    G::stand_holder(base).store_at(4 * i, 4, slot);
    StandSlotInit(base, uint32_t(G::stand_holder(base).load_at(4 * i, 4)), i, frame, load);
  }
  const uint32_t record = frame + 80;
  scene::SetUpConstruct(base, record);
  for (uint32_t offset = 0; offset < 40; offset += 4) {
    StoreU32(base, record + offset, 0);
  }
  StoreU32(base, record + 40, 0);
  for (const uint32_t offset : {4u, 8u, 12u, 20u, 24u, 28u}) {
    StoreU32(base, record + offset, cars);
  }
  StoreU32(base, record + 32, 0);
  calls.scene_set_up(schema::CarDrawGlobals::scene<schema::View>(base).address, record, frame);
  return record;
}

std::optional<uint64_t> StandSceneRelease(uint8_t* base) {
  const uint32_t holder = G::stand_holder<schema::View>(base).address;
  if (!memory::IsPages(holder)) {
    return std::nullopt;
  }
  for (uint32_t offset = 0; offset < 12; offset += 4) {
    const uint32_t slot = uint32_t(schema::StandHolder(base, holder).load_at(offset, 4));
    if (slot != 0 && !SlotIsPages(base, slot)) {
      return std::nullopt;
    }
  }
  if (!cameras::ReleaseViewports(base)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82180738 StandScene::Release");
  uint32_t r3 = G::stand_holder<schema::View>(base).address;
  for (uint32_t offset = 0; offset < 12; offset += 4) {
    const uint32_t slot = uint32_t(schema::StandHolder(base, r3).load_at(offset, 4));
    if (slot == 0) {
      continue;
    }
    StandSlotFree(base, slot);
    memory::FreePages(uint32_t(G::stand_holder(base).load_at(offset, 4)));
    G::stand_holder(base).store_at(offset, 4, 0);
    r3 = G::stand_holder<schema::View>(base).address;
  }
  const uint64_t freed = memory::FreePages(r3) ? 1 : 0;
  G::set_stand_holder(base, 0u);
  return freed;
}

uint64_t StandScenePollTeardown(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82179AE8 StandScene::PollTeardown");
  constexpr uint32_t kCarSceneUpdate = 0x820C9608, kCarSceneDraw = 0x820C5DE8;
  constexpr uint32_t kStartTeardown = 0x820C4D88;
  const uint32_t frame = sp - kPollTeardownFrame;
  StoreU32(base, frame, sp);  // the back chain, for the guest calls
  schema::StandHolder h = G::stand_holder(base);
  const uint32_t scene = schema::CarDrawGlobals::scene<schema::View>(base).address;
  if (h.unknown_12() != 0) {
    h.set_unknown_16(h.unknown_16() - 1);
    h = G::stand_holder(base);
    if (int32_t(h.unknown_16()) > 0) {
      calls.call(kCarSceneUpdate, frame, 0x82179BC4, scene, 0);
      return 0;
    }
    h.set_unknown_12(0);
    return 1;
  }
  const uint32_t done = scene::LoadsDone(
      base, scene, frame,
      [&](uint32_t function, uint32_t at, uint32_t lr, uint32_t r3, uint32_t r4) {
        return calls.call(function, at, lr, r3, r4);
      });
  h = G::stand_holder(base);
  if (done == 0 && h.model_made() != 0) {
    for (uint32_t offset = 0; offset < 12; offset += 4) {
      const schema::StandSlot slot(base, LoadU32(base, h.stage_block_0_address() + offset));
      const uint32_t index = slot.stage().model_index();
      if (int32_t(index) == -1) {
        continue;
      }
      const uint32_t current = schema::CarDrawGlobals::scene<schema::View>(base).address;
      const uint32_t model = vehicle_accessors::ScenePoolsGetB(base, current, 7, index);
      schema::SceneModel(base, model).set_draw_enabled(0);
      calls.call(kCarSceneUpdate, frame, 0x82179B64, model, 0);
      calls.call(kCarSceneDraw, frame, 0x82179B70, current, 0);
      h = G::stand_holder(base);
    }
    return 0;
  }
  h.set_unknown_12(1);
  G::stand_holder(base).set_unknown_16(10);
  calls.call(kStartTeardown, frame, 0x82179BA0, done, 0);  // r3 as LoadsDone left it
  return 0;
}

}  // namespace type67_modes
