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
#include "track_zones/track_zones.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <array>
#include <cmath>
#include <cstdlib>

namespace car_draw {

namespace {

namespace sl = scenery_lists;

constexpr uint32_t kRecordList = 0x8296B918;
constexpr uint32_t kGroup0Key = 0x8296B90C;
constexpr uint32_t kGroup1Key = 0x8296B900;
constexpr uint32_t kDepthKey = 0x8296B908;

constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  guest::StoreU32(base, frame, stack);
  return frame;
}

// A slot, which the checks before made sure of.
uint32_t Take(uint8_t* base, uint32_t list, uint32_t count) {
  const uint32_t slot = command_list::AllocateSlots(base, list, count);
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

std::optional<Draw> ModelDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model,
                              uint32_t view_projection, uint32_t world, uint32_t shape,
                              uint32_t depth_too, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820D04E8 Model::Draw");
  const uint32_t list = guest::LoadU32(base, kRecordList);
  // Every slot it would take, checked before anything is done: the two
  // entries, RecordObject's 11, RecordSceneLights' 65, the groups'
  // materials and the depth entry.
  {
    const auto group0 = sl::ModelRecordMaterialsSlotCount(base, model, shape, 0, 1);
    const auto group1 = sl::ModelRecordMaterialsSlotCount(base, model, shape, 1, 1);
    if (!group0 || !group1) {
      return std::nullopt;
    }
    uint32_t slots = 6 + 11 + 65 + *group0 + *group1;
    if (int32_t(depth_too) != 0) {
      slots += 2 + sl::RecordShapeDepthSlotCount(base, model, shape, 0, 0, 0);
    }
    if (!sl::CommandListHasRoom(base, list, slots)) {
      return std::nullopt;
    }
  }
  const uint32_t frame = Frame(base, stack, kModelDrawFrame);
  Draw draw;
  if (guest::LoadU32(base, model + 28) == 0) {
    draw.r3 = model;
    return draw;
  }
  const uint32_t visible =
      sl::IsShapeVisible(base, model, view_projection, world, shape, frame - 96);
  if ((visible & 0xFF) == 0) {
    draw.r3 = visible;
    return draw;
  }
  command_list::OpenChunk(base, list);
  const uint32_t entry0 = Take(base, list, 3);
  const uint32_t entry1 = Take(base, list, 3);
  command_list::CloseChunk(base, list);
  // The recorder's wrapper's context: the return address and r24-r31 there
  // (the caller's r24, the world matrix, the shape, the view-projection,
  // 0x82970000, the second and first entries, the model).
  interpolation::BlendState_BeforeRecordObject(base, view_projection, world);
  interpolation::BlendState_SetRecordContext({0x820D0564, caller_r24, world, shape,
                                              view_projection, 0x82970000u, entry1, entry0,
                                              model});
  const auto constants = sl::RecordObject(base, view_projection, world);
  interpolation::BlendState_SetRecordContext({});
  interpolation::BlendState_AfterRecordObject(base);
  const auto lights = sl::RecordSceneLights(base);
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
  const Group groups[2] = {{0, entry0, kGroup0Key, 0x820D05EC},
                           {1, entry1, kGroup1Key, 0x820D0654}};
  for (const Group& g : groups) {
    if (models::MaterialCount(base, model, shape, g.group) == 0) {
      // r3 the count's.
      draw.queued = false;
      draw.r3 = 0;
      continue;
    }
    guest::StoreU16(base, g.entry + 2, 3);
    guest::StoreU32(base, g.entry + 4, *constants);
    guest::StoreU16(base, g.entry + 18, 3);
    guest::StoreU32(base, g.entry + 20, *lights);
    guest::StoreU16(base, g.entry + 34, 4);
    Frame(base, frame, sl::kModelRecordMaterialsFrame);  // its back chain, as its stwu
    const auto materials =
        sl::ModelRecordMaterials(base, calls.draw, record_frame, model, shape, g.group, 1);
    if (!materials) {
      Declined("Model::RecordMaterials");
    }
    guest::StoreU32(base, g.entry + 36, *materials);
    draw.r3 = calls.draw.submit(calls.draw.user, g.entry, guest::LoadU32(base, g.key),
                                Lfs(base, *constants + 72), frame, g.lr);
    draw.queued = true;
  }
  if (int32_t(depth_too) == 0) {
    return draw;
  }
  command_list::OpenChunk(base, list);
  const uint32_t entry = Take(base, list, 2);
  command_list::CloseChunk(base, list);
  if (models::MaterialCount(base, model, shape, 0) == 0) {
    draw.queued = false;
    draw.r3 = 0;
    return draw;
  }
  guest::StoreU16(base, entry + 2, 3);
  guest::StoreU32(base, entry + 4, *constants);
  guest::StoreU16(base, entry + 18, 4);
  const auto depth = sl::RecordShapeDepth(base, model, shape, 0, 0, 0);
  if (!depth) {
    Declined("Model::RecordShapeDepth");
  }
  guest::StoreU32(base, entry + 20, *depth);
  draw.r3 = calls.draw.submit(calls.draw.user, entry, guest::LoadU32(base, kDepthKey),
                              Lfs(base, *constants + 72), frame, 0x820D06E4);
  draw.queued = true;
  return draw;
}

// --- CarPart::Draw ---

std::optional<Draw> CarPartDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t part,
                                uint32_t view_projection, uint32_t world, uint32_t r6,
                                uint32_t r7, uint32_t caller_r24, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820D06F0 CarPart::Draw");
  const uint32_t model = guest::LoadU32(base, part + 4);
  if (model == 0) {
    Draw draw;
    draw.r3 = 0;
    return draw;
  }
  const uint32_t shape = guest::LoadU32(base, part + 228);
  if (int32_t(r7) == 0) {
    return ModelDraw(base, calls, stack, model, view_projection, world, shape, r6, caller_r24);
  }
  // Model::DrawShapeDepth(model, r4, r5, shape, 0, 0, 0): what its
  // recorder's wrapper keeps is the return address 0x82596D9C and r24-r31 =
  // r9, r8, r7, r6, r28, r3, r5, r4 at its entry.
  const std::array<uint32_t, 9> context = {0x82596D9C, 0, 0, 0, shape, caller_r28,
                                           model,      world, view_projection};
  const uint32_t frame = stack - kDrawShapeDepthFrame;
  const auto depth = sl::DrawShapeDepth(base, model, view_projection, world, shape, 0, 0, 0,
                                        frame, context);
  if (!depth) {
    return std::nullopt;
  }
  Frame(base, stack, kDrawShapeDepthFrame);  // its back chain, as its stwu
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

uint32_t SceneModelIsNearPlayerHeight(uint8_t* base, const Calls& calls, uint32_t stack,
                                      uint32_t pass, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820DAB58 SceneModel::IsNearPlayerHeight");
  constexpr uint32_t kRaceData = 0x826C5B2C;
  constexpr uint32_t kRaceKind = 0xA3AF0;
  constexpr uint32_t kRaceWord = 0x59614;
  constexpr uint32_t kSecondPlayer = 0x826C7824;
  constexpr uint32_t kHeightApart = 0x82002DF4;
  const uint32_t frame = Frame(base, stack, kNearPlayerHeightFrame);
  if (vehicle_accessors::VehicleGet(base, 0) == 0) {
    return 1;
  }
  const uint32_t index = guest::LoadU32(base, pass + 0);
  uint32_t player = 0;
  if (index == 2) {
    player = 1;
  } else if (index >= 4) {
    return 1;
  } else {
    const uint32_t race = guest::LoadU32(base, kRaceData);
    if (int32_t(guest::LoadU32(base, race + kRaceKind)) == 6 &&
        int32_t(guest::LoadU32(base, race + kRaceWord)) == 2) {
      player = guest::LoadU32(base, kSecondPlayer) == 0 ? 1 : 0;
    }
  }
  const uint32_t vehicle = track_zones::RacersVehicle(base, player);
  if (vehicle == 0) {
    return 1;
  }
  const float zero = guest::LoadFloat(base, kZero);
  guest::StoreFloat(base, frame + 80, zero);
  guest::StoreFloat(base, frame + 84, zero);
  guest::StoreFloat(base, frame + 88, zero);
  guest::StoreFloat(base, frame + 92, guest::LoadFloat(base, kOne));
  if (!native::Buffers_Memcpy(base, frame + 96, vehicle + 3312, 64, 0x820DAC1C)) {
    calls.memcpy(frame + 96, vehicle + 3312, 64, frame, 0x820DAC1C);
  }
  debug::Vec4Transform(base, frame + 80, frame + 96, frame + 80);
  const double y = Lfs(base, frame + 84);
  const double apart = std::fabs(double(float(y - Lfs(base, position + 4))));
  if (apart > Lfs(base, kHeightApart)) {
    return 0;
  }
  return 1;
}

}  // namespace car_draw
