// The stand scene's teardown and the garage camera's presets load
// (docs/functions/type6-type7-modes.md) as plain C++ over the allocation
// layer: Viewports::Release's, XMemSet's and Archive::Toc's plain C++,
// Archive::Load through the caller's LoadCall.

#include "type67_modes/type67_modes.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "cameras/cameras.h"
#include "guest.h"
#include "memory/heap.h"

#include <bit>

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// Whether a slot's blocks are all the layer's pages (StandSlot::Free frees
// its block and its camera's presets).
bool SlotIsPages(uint8_t* base, uint32_t slot) {
  if (!memory::IsPages(slot)) {
    return false;
  }
  const uint32_t block = LoadU32(base, slot);
  if (block == 0) {
    return true;
  }
  if (!memory::IsPages(block)) {
    return false;
  }
  const uint32_t camera = block + 112;
  const uint32_t count = LoadU32(base, camera + 4392);
  for (uint32_t i = 0; i < count; i++) {
    if (!memory::IsPages(LoadU32(base, camera + 296 + 4 * i))) {
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
  const uint32_t entry = archive::Toc(base, kPresetsAsset) + 0x60000 - 31600;
  uint32_t size = LoadU32(base, entry + 8);
  if (size == 0) {
    size = LoadU32(base, entry + 4);
  }
  const uint32_t block = memory::AllocPages(size, 4096);
  c_runtime::XMemSet(base, block, 0, size);
  return load(kPresetsAsset, block, kOnPresetsLoaded, camera);
}

int64_t GarageCameraInit(uint8_t* base, uint32_t camera, uint32_t sp, const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180918 GarageCamera::Init");
  const uint32_t frame = sp - kGarageCameraInitFrame;
  GarageCameraReset(base, camera, frame - kGarageCameraResetFrame);
  StoreU32(base, camera + 4392, 0);
  StoreU32(base, camera + 292, 0x82619428);
  StoreU32(base, camera + 0, 0);
  return GarageCameraLoadPresets(base, camera, load);
}

void GarageStageInit(uint8_t* base, uint32_t stage, uint32_t index, uint32_t sp,
                     const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180968 GarageStage::Init");
  const uint32_t f = sp - kGarageStageInitFrame;
  // lfs/stfs of the game's 0.0 and 1.0.
  const uint32_t zero = std::bit_cast<uint32_t>(float(double(guest::LoadFloat(base, 0x82000A34))));
  const uint32_t one = std::bit_cast<uint32_t>(float(double(guest::LoadFloat(base, 0x82000A30))));
  StoreU32(base, f + 80, zero);
  StoreU32(base, stage + 0, index);
  StoreU32(base, f + 84, zero);
  StoreU32(base, stage + 16, 0);
  StoreU32(base, f + 88, zero);
  StoreU32(base, stage + 4, 0xFFFFFFFF);
  StoreU32(base, stage + 6052, 1);
  StoreU32(base, f + 92, one);
  StoreU32(base, stage + 6056, 1);
  StoreU32(base, stage + 12, zero);
  StoreU32(base, stage + 6092, 1);
  StoreU32(base, stage + 8, 0xFFFFFFFF);
  StoreU32(base, stage + 6060, 0);
  StoreU32(base, stage + 6088, 0xFFFFFFFF);
  for (uint32_t offset = 6064; offset <= 6084; offset += 4) {
    StoreU32(base, stage + offset, 0);
  }
  for (uint32_t offset = 6096; offset <= 6160; offset += 4) {
    StoreU32(base, stage + offset, zero);
  }
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreU32(base, stage + 6176 + i, LoadU32(base, f + 80 + i));
  }
  StoreU32(base, f + 96, zero);
  StoreU32(base, f + 100, zero);
  StoreU32(base, f + 104, zero);
  StoreU32(base, f + 108, one);
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreU32(base, stage + 6192 + i, LoadU32(base, f + 96 + i));
  }
  GarageStageResetPose(base, stage + 32, f);
  GarageCameraInit(base, stage + 112, f, load);
  GarageLightsInit(base, stage + 4512, f - kGarageLightsFrame);
  StoreU32(base, stage + 6048, 0);
}

bool StandSlotInit(uint8_t* base, uint32_t slot, uint32_t index, uint32_t sp,
                   const LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_82180AA0 StandSlot::Init");
  if (LoadU32(base, slot) != 0) {
    return false;
  }
  const uint32_t stage = memory::AllocPages(kGarageStageBytes, 4096);
  StoreU32(base, slot, stage);
  GarageStageInit(base, stage, index, sp - kStandSlotInitFrame, load);
  return true;
}

std::optional<uint64_t> StandSceneRelease(uint8_t* base) {
  const uint32_t holder = LoadU32(base, kStandHolder);
  if (!memory::IsPages(holder)) {
    return std::nullopt;
  }
  for (uint32_t offset = 0; offset < 12; offset += 4) {
    const uint32_t slot = LoadU32(base, holder + offset);
    if (slot != 0 && !SlotIsPages(base, slot)) {
      return std::nullopt;
    }
  }
  if (!cameras::ReleaseViewports(base)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82180738 StandScene::Release");
  uint32_t r3 = LoadU32(base, kStandHolder);
  for (uint32_t offset = 0; offset < 12; offset += 4) {
    const uint32_t slot = LoadU32(base, offset + r3);
    if (slot == 0) {
      continue;
    }
    StandSlotFree(base, slot);
    memory::FreePages(LoadU32(base, offset + LoadU32(base, kStandHolder)));
    StoreU32(base, offset + LoadU32(base, kStandHolder), 0);
    r3 = LoadU32(base, kStandHolder);
  }
  const uint64_t freed = memory::FreePages(r3) ? 1 : 0;
  StoreU32(base, kStandHolder, 0);
  return freed;
}

}  // namespace type67_modes
