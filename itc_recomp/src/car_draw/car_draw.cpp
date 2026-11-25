// The car draw's part draws and the height cull as plain C++
// (docs/functions/car-draw.md); see car_draw.h.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "debug/frame_state_diff.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "models/model.h"
#include "native/buffers.h"
#include "schema/car_draw.h"
#include "schema/frame_driver.h"
#include "schema/models.h"
#include "schema/race_states.h"
#include "schema/race_update.h"
#include "track_zones/track_zones.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <array>
#include <cmath>
#include <cstdlib>

namespace car_draw {

namespace {

namespace sl = scenery_lists;

constexpr uint32_t kRecordList = schema::FrameDriverGlobals::scene_list_address();
using G = schema::CarDrawGlobals;

constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0

double Lfs(uint32_t address) {
  return double(guest::LoadFloat(address));
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  guest::StoreU32(frame, stack);
  return frame;
}

// A slot, which the checks before made sure of.
uint32_t Take(uint32_t list, uint32_t count) {
  const uint32_t slot = command_list::AllocateSlots(list, count);
  if (slot == 0) {
    ITC_LOG_ERROR("car_draw: the command list refused a slot after its checks");
    std::abort();
  }
  return slot;
}

[[noreturn]] void Declined(const char* what) {
  ITC_LOG_ERROR("car_draw: {} declined after its checks", what);
  std::abort();
}

}  // namespace

// --- Model::Draw ---

std::optional<Draw> ModelDraw(const Calls& calls, uint32_t stack, uint32_t model,
                              uint32_t view_projection, uint32_t world, uint32_t shape,
                              uint32_t depth_too, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820D04E8 Model::Draw");
  const uint32_t list = guest::LoadU32(kRecordList);
  // Every slot it would take, checked before anything is done: the two
  // entries, RecordObject's 11, RecordSceneLights' 65, the groups'
  // materials and the depth entry.
  {
    const auto group0 = sl::ModelRecordMaterialsSlotCount(model, shape, 0, 1);
    const auto group1 = sl::ModelRecordMaterialsSlotCount(model, shape, 1, 1);
    if (!group0 || !group1) {
      return std::nullopt;
    }
    uint32_t slots = 6 + 11 + 65 + *group0 + *group1;
    if (int32_t(depth_too) != 0) {
      slots += 2 + sl::RecordShapeDepthSlotCount(model, shape, 0, 0, 0);
    }
    if (!sl::CommandListHasRoom(list, slots)) {
      return std::nullopt;
    }
  }
  const uint32_t frame = Frame(stack, kModelDrawFrame);
  Draw draw;
  if (schema::Model(model).header().address == 0) {
    draw.r3 = model;
    return draw;
  }
  const uint32_t visible =
      sl::IsShapeVisible(model, view_projection, world, shape, frame - 96);
  if ((visible & 0xFF) == 0) {
    draw.r3 = visible;
    return draw;
  }
  command_list::OpenChunk(list);
  const uint32_t entry0 = Take(list, 3);
  const uint32_t entry1 = Take(list, 3);
  command_list::CloseChunk(list);
  // The recorder's wrapper's context: the return address and r24-r31 there
  // (the caller's r24, the world matrix, the shape, the view-projection,
  // 0x82970000, the second and first entries, the model).
  interpolation::BlendState_BeforeRecordObject(view_projection, world);
  interpolation::BlendState_SetRecordContext({0x820D0564, caller_r24, world, shape,
                                              view_projection, 0x82970000u, entry1, entry0,
                                              model});
  const auto constants = sl::RecordObject(view_projection, world);
  interpolation::BlendState_SetRecordContext({});
  interpolation::BlendState_AfterRecordObject();
  const auto lights = sl::RecordSceneLights();
  if (!constants || !lights) {
    Declined("Model::Draw's recorders");
  }
  // Model::RecordMaterials' frame below this one (it saves its arguments in
  // this frame).
  const uint32_t record_frame = frame - sl::kModelRecordMaterialsFrame;
  struct Group {
    int32_t group;
    uint32_t entry;
    uint32_t key;
    uint32_t lr;
  };
  const Group groups[2] = {{0, entry0, G::key_group0_address(), 0x820D05EC},
                           {1, entry1, G::key_group1_address(), 0x820D0654}};
  for (const Group& g : groups) {
    if (models::MaterialCount(model, shape, g.group) == 0) {
      // r3 the count's.
      draw.queued = false;
      draw.r3 = 0;
      continue;
    }
    guest::StoreU16(g.entry + 2, 3);
    guest::StoreU32(g.entry + 4, *constants);
    guest::StoreU16(g.entry + 18, 3);
    guest::StoreU32(g.entry + 20, *lights);
    guest::StoreU16(g.entry + 34, 4);
    Frame(frame, sl::kModelRecordMaterialsFrame);  // its back chain, as its stwu
    const auto materials =
        sl::ModelRecordMaterials(calls.draw, record_frame, model, shape, g.group, 1);
    if (!materials) {
      Declined("Model::RecordMaterials");
    }
    guest::StoreU32(g.entry + 36, *materials);
    draw.r3 = calls.draw.submit(calls.draw.user, g.entry, guest::LoadU32(g.key),
                                Lfs(*constants + 72), frame, g.lr);
    draw.queued = true;
  }
  if (int32_t(depth_too) == 0) {
    return draw;
  }
  command_list::OpenChunk(list);
  const uint32_t entry = Take(list, 2);
  command_list::CloseChunk(list);
  if (models::MaterialCount(model, shape, 0) == 0) {
    draw.queued = false;
    draw.r3 = 0;
    return draw;
  }
  guest::StoreU16(entry + 2, 3);
  guest::StoreU32(entry + 4, *constants);
  guest::StoreU16(entry + 18, 4);
  const auto depth = sl::RecordShapeDepth(model, shape, 0, 0, 0);
  if (!depth) {
    Declined("Model::RecordShapeDepth");
  }
  guest::StoreU32(entry + 20, *depth);
  draw.r3 = calls.draw.submit(calls.draw.user, entry, G::key_depth(),
                              Lfs(*constants + 72), frame, 0x820D06E4);
  draw.queued = true;
  return draw;
}

// --- CarPart::Draw ---

std::optional<Draw> CarPartDraw(const Calls& calls, uint32_t stack, uint32_t part,
                                uint32_t view_projection, uint32_t world, uint32_t r6,
                                uint32_t r7, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820D06F0 CarPart::Draw");
  const schema::CarPart record(part);
  const uint32_t model = record.model();
  if (model == 0) {
    Draw draw;
    draw.r3 = 0;
    return draw;
  }
  const uint32_t shape = record.shape();
  if (int32_t(r7) == 0) {
    return ModelDraw(calls, stack, model, view_projection, world, shape, r6, caller_r24);
  }
  // Model::DrawShapeDepth(model, r4, r5, shape, 0, 0, 0): what its
  // recorder's wrapper keeps is the return address 0x82596D9C and r24-r31 =
  // r9, r8, r7, r6, r28, r3, r5, r4 at its entry.
  const std::array<uint32_t, 9> context = {0x82596D9C, 0, 0, 0, shape, caller_r28,
                                           model,      world, view_projection};
  const uint32_t frame = stack - kDrawShapeDepthFrame;
  const auto depth = sl::DrawShapeDepth(model, view_projection, world, shape, 0, 0, 0,
                                        frame, context);
  if (!depth) {
    return std::nullopt;
  }
  Frame(stack, kDrawShapeDepthFrame);  // its back chain, as its stwu
  Draw draw;
  if (!depth->submit) {
    draw.r3 = depth->r3;
    return draw;
  }
  draw.r3 = calls.draw.submit(calls.draw.user, depth->entry, depth->key, depth->depth, frame,
                              0x82596E60);
  draw.queued = true;
  return draw;
}

// --- SceneModel::IsNearPlayerHeight ---

uint32_t SceneModelIsNearPlayerHeight(const Calls& calls, uint32_t stack,
                                      uint32_t pass, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820DAB58 SceneModel::IsNearPlayerHeight");
  constexpr uint32_t kHeightApart = 0x82002DF4;
  const uint32_t frame = Frame(stack, kNearPlayerHeightFrame);
  if (vehicle_accessors::VehicleGet(0) == 0) {
    return 1;
  }
  const uint32_t index = guest::LoadU32(pass + 0);
  uint32_t player = 0;
  if (index == 2) {
    player = 1;
  } else if (index >= 4) {
    return 1;
  } else {
    const schema::RaceData race = schema::RaceStatesGlobals::race_data();
    if (int32_t(race.kind()) == 6 && int32_t(race.added_count()) == 2) {
      player = schema::RaceUpdateGlobals::followed() == 0 ? 1 : 0;
    }
  }
  const uint32_t vehicle = track_zones::RacersVehicle(player);
  if (vehicle == 0) {
    return 1;
  }
  const float zero = guest::LoadFloat(kZero);
  guest::StoreFloat(frame + 80, zero);
  guest::StoreFloat(frame + 84, zero);
  guest::StoreFloat(frame + 88, zero);
  guest::StoreFloat(frame + 92, guest::LoadFloat(kOne));
  if (!native::Buffers_Memcpy(frame + 96, vehicle + 3312, 64, 0x820DAC1C)) {
    calls.memcpy(frame + 96, vehicle + 3312, 64, frame, 0x820DAC1C);
  }
  debug::Vec4Transform(frame + 80, frame + 96, frame + 80);
  const double y = Lfs(frame + 84);
  const double apart =
      std::fabs(double(float(y - double(schema::NearHeightPosition(position).y()))));
  if (apart > Lfs(kHeightApart)) {
    return 0;
  }
  return 1;
}

}  // namespace car_draw
