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
bool SlotIsPages(uint32_t slot) {
  if (!memory::IsPages(slot)) {
    return false;
  }
  const uint32_t block = schema::StandSlot(slot).stage<schema::View>().address;
  if (block == 0) {
    return true;
  }
  if (!memory::IsPages(block)) {
    return false;
  }
  const schema::GarageCamera camera(schema::GarageStage(block).camera_address());
  const uint32_t count = camera.preset_count();
  for (uint32_t i = 0; i < count; i++) {
    if (!memory::IsPages(LoadU32(camera.presets_address() + 4 * i))) {
      return false;
    }
  }
  return true;
}

}  // namespace

int64_t GarageCameraLoadPresets(uint32_t camera,
                                const std::function<int64_t(uint32_t, uint32_t, uint32_t,
                                                            uint32_t)>& load) {
  ITC_NATIVE_RUNNING("sub_821802D8 GarageCamera::LoadPresets");
  // The asset's entry: (22600 + 1) x 16 into the TOC.
  const schema::ArchiveTocEntry entry(archive::Toc(kPresetsAsset) + 0x60000 - 31600);
  uint32_t size = entry.unpacked_size();
  if (size == 0) {
    size = entry.stored_size();
  }
  // The asset's buffer, a blob (E5 step 5: PC memory in the guest's byte
  // order) in place of the original's VirtualAlloc (0 for 0 bytes, as the
  // layer's pages gave); GarageCamera::OnPresetsLoaded frees it.
  const uint32_t block = size == 0 ? 0 : schema::GaragePresetsData::New(size).address;
  c_runtime::XMemSet(block, 0, size);
  return load(kPresetsAsset, block, kOnPresetsLoaded, camera);
}

int64_t GarageCameraInit(uint32_t camera, uint32_t sp, const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180918 GarageCamera::Init");
  const uint32_t frame = sp - kGarageCameraInitFrame;
  GarageCameraReset(camera, frame - kGarageCameraResetFrame);
  const schema::GarageCamera cam(camera);
  cam.set_preset_count(0);
  cam.set_preset_table(0x82619428);
  cam.set_set_up(0);
  return GarageCameraLoadPresets(camera, load);
}

void GarageStageInit(uint32_t stage, uint32_t index, uint32_t sp,
                     const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180968 GarageStage::Init");
  const uint32_t f = sp - kGarageStageInitFrame;
  // lfs/stfs of the game's 0.0 and 1.0.
  using F = schema::StringsAndFontGlobals;
  const uint32_t zero = std::bit_cast<uint32_t>(float(double(F::zero())));
  const uint32_t one = std::bit_cast<uint32_t>(float(double(F::one())));
  const schema::GarageStage st(stage);
  // The two axle vectors the original builds at its frame's +80 and +96,
  // read back only here: C++ arrays since E5 step 6.
  const uint32_t front[4] = {zero, zero, zero, one};
  const uint32_t rear[4] = {zero, zero, zero, one};
  st.set_index(index);
  st.set_model_claimed(0);
  st.set_pending_block(0xFFFFFFFF);
  st.set_reflection(1);
  st.set_draw_word(1);
  StoreU32(st.unknown_12_address(), zero);  // the word as loaded (stfs)
  st.set_plates_shown(1);
  st.set_model_index(0xFFFFFFFF);
  st.set_dark(0);
  st.set_waiting_preset(0xFFFFFFFF);
  // +6064 to +6084, then the floats +6096 to +6160.
  for (uint32_t i = 0; i <= 20; i += 4) {
    StoreU32(st.unknown_6064_address() + i, 0);
  }
  for (uint32_t i = 0; i <= 64; i += 4) {
    StoreU32(st.front_axle_height_address() + i, zero);
  }
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(st.front_axle_x_address() + 4 * i, front[i]);
  }
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(st.rear_axle_x_address() + 4 * i, rear[i]);
  }
  GarageStageResetPose(st.bonnet_address(), f);
  GarageCameraInit(st.camera_address(), f, load);
  GarageLightsInit(st.lights_address(), f - kGarageLightsFrame);
  st.set_animation_count(0);
}

bool StandSlotInit(uint32_t slot, uint32_t index, uint32_t sp,
                   const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180AA0 StandSlot::Init");
  const schema::StandSlot s(slot);
  if (s.stage<schema::View>().address != 0) {
    return false;
  }
  const uint32_t stage = memory::AllocPages(kGarageStageBytes, 4096);
  s.set_stage(stage);
  GarageStageInit(stage, index, sp - kStandSlotInitFrame, load);
  return true;
}

uint64_t StandSceneCreate(uint32_t cars, uint32_t sp, const LoadCall& load,
                          const StandSceneCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82180B00 StandScene::Create");
  const uint32_t frame = sp - kStandSceneCreateFrame;
  StoreU32(frame, sp);
  calls.viewports_init(frame);
  const uint64_t created = calls.scene_create(frame);
  if (G::stand_holder<schema::View>().address != 0) {
    return created;
  }
  // A host object (E5 step 5) in place of the VirtualAlloc(24) at
  // 0x82180B3C; the global holds its handle.
  const uint32_t holder = schema::StandHolder::New().address;
  G::set_stand_holder(holder);
  schema::StandHolder(holder).set_unknown_12(0);
  G::stand_holder().set_unknown_16(0);
  G::stand_holder().set_model_made(0);
  // The three slots, stage_block_0 to 2.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slot = memory::AllocPages(4, 4096);
    // The holder's words through its view (a host object, E5 step 5).
    G::stand_holder().store_at(4 * i, 4, slot);
    StandSlotInit(uint32_t(G::stand_holder().load_at(4 * i, 4)), i, frame, load);
  }
  const uint32_t record = frame + 80;
  scene::SetUpConstruct(record);
  for (uint32_t offset = 0; offset < 40; offset += 4) {
    StoreU32(record + offset, 0);
  }
  StoreU32(record + 40, 0);
  for (const uint32_t offset : {4u, 8u, 12u, 20u, 24u, 28u}) {
    StoreU32(record + offset, cars);
  }
  StoreU32(record + 32, 0);
  calls.scene_set_up(schema::CarDrawGlobals::scene<schema::View>().address, record, frame);
  return record;
}

std::optional<uint64_t> StandSceneRelease() {
  const uint32_t holder = G::stand_holder<schema::View>().address;
  if (!memory::IsPages(holder)) {
    return std::nullopt;
  }
  for (uint32_t offset = 0; offset < 12; offset += 4) {
    const uint32_t slot = uint32_t(schema::StandHolder(holder).load_at(offset, 4));
    if (slot != 0 && !SlotIsPages(slot)) {
      return std::nullopt;
    }
  }
  if (!cameras::ReleaseViewports()) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82180738 StandScene::Release");
  uint32_t r3 = G::stand_holder<schema::View>().address;
  for (uint32_t offset = 0; offset < 12; offset += 4) {
    const uint32_t slot = uint32_t(schema::StandHolder(r3).load_at(offset, 4));
    if (slot == 0) {
      continue;
    }
    StandSlotFree(slot);
    memory::FreePages(uint32_t(G::stand_holder().load_at(offset, 4)));
    G::stand_holder().store_at(offset, 4, 0);
    r3 = G::stand_holder<schema::View>().address;
  }
  const uint64_t freed = memory::FreePages(r3) ? 1 : 0;
  G::set_stand_holder(0u);
  return freed;
}

uint64_t StandScenePollTeardown(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82179AE8 StandScene::PollTeardown");
  constexpr uint32_t kCarSceneUpdate = 0x820C9608, kCarSceneDraw = 0x820C5DE8;
  constexpr uint32_t kStartTeardown = 0x820C4D88;
  const uint32_t frame = sp - kPollTeardownFrame;
  StoreU32(frame, sp);  // the back chain, for the guest calls
  schema::StandHolder h = G::stand_holder();
  const uint32_t scene = schema::CarDrawGlobals::scene<schema::View>().address;
  if (h.unknown_12() != 0) {
    h.set_unknown_16(h.unknown_16() - 1);
    h = G::stand_holder();
    if (int32_t(h.unknown_16()) > 0) {
      calls.call(kCarSceneUpdate, frame, 0x82179BC4, scene, 0);
      return 0;
    }
    h.set_unknown_12(0);
    return 1;
  }
  const uint32_t done = scene::LoadsDone(scene, frame,
      [&](uint32_t function, uint32_t at, uint32_t lr, uint32_t r3, uint32_t r4) {
        return calls.call(function, at, lr, r3, r4);
      });
  h = G::stand_holder();
  if (done == 0 && h.model_made() != 0) {
    for (uint32_t offset = 0; offset < 12; offset += 4) {
      const schema::StandSlot slot(LoadU32(h.stage_block_0_address() + offset));
      const uint32_t index = slot.stage().model_index();
      if (int32_t(index) == -1) {
        continue;
      }
      const uint32_t current = schema::CarDrawGlobals::scene<schema::View>().address;
      const uint32_t model = vehicle_accessors::ScenePoolsGetB(current, 7, index);
      schema::SceneModel(model).set_draw_enabled(0);
      calls.call(kCarSceneUpdate, frame, 0x82179B64, model, 0);
      calls.call(kCarSceneDraw, frame, 0x82179B70, current, 0);
      h = G::stand_holder();
    }
    return 0;
  }
  h.set_unknown_12(1);
  G::stand_holder().set_unknown_16(10);
  calls.call(kStartTeardown, frame, 0x82179BA0, done, 0);  // r3 as LoadsDone left it
  return 0;
}

}  // namespace type67_modes
