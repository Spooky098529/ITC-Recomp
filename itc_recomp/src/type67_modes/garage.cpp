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
