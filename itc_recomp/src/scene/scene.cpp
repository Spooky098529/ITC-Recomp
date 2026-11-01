// The car scene (see scene.h), load for load and store for store as the
// recompiled functions.

#include "scene/scene.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstdlib>

namespace scene {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A member whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Scene: {} refused after its checks", what);
    std::abort();
  }
}

constexpr uint32_t kFirstSetPools = 7;
constexpr uint32_t kFirstSetLists = 56;
constexpr uint32_t kDestroyAFrameBytes = 128;
constexpr uint32_t kMaxNodes = 4096;

}  // namespace

// --- The small pieces ---

void SetUpConstruct(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820D0D50 SceneSetUp::SceneSetUp");
  Store(base, record + 36, 0);
  Store(base, record + 40, 0);
}

std::optional<uint32_t> FreePagesIn(uint8_t* base, uint32_t at) {
  ITC_NATIVE_RUNNING("sub_820D09A0 FreePagesIn");
  const uint32_t pages = Load(base, at);
  if (pages == 0 || !memory::IsPages(pages)) {
    return std::nullopt;
  }
  memory::FreePages(pages);
  return 1;
}

void StartTeardownA(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820C3C80 ScenePools::StartTeardownA");
  Store(base, Load(base, kFirstSet) + 4, 4);
  Store(base, Load(base, kFirstSet) + 0, 2);
}

std::optional<uint64_t> StartTeardown(uint8_t* base,
                                      const std::function<uint64_t(uint32_t set)>& release_all) {
  ITC_NATIVE_RUNNING("sub_820C4D88 CarScene::StartTeardown");
  if (Load(base, kScene) == 0) {
    return std::nullopt;
  }
  Store(base, Load(base, kScene) + 0, 4);
  Store(base, Load(base, kScene) + 48, 2);
  StartTeardownA(base);
  return release_all(Load(base, 0x826C5510));  // sub_820F0E50: the accessories' set
}

std::optional<uint32_t> SetInteriorWord(uint8_t* base, uint32_t scene, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_820C8950 CarScene::SetInteriorWord");
  const int32_t count = int32_t(Load(base, scene + 5036));
  std::optional<uint32_t> r3;
  for (int32_t i = 0; i < count; i++) {
    const uint32_t model = vehicle_accessors::ListAt(base, scene + 5028, uint32_t(i));
    r3 = model;
    if (Load(base, model + 5072) != 0) {
      Store(base, Load(base, model + 5068) + 11616, word);
    }
  }
  return r3;
}

// --- The first set's teardown ---

bool DestroyAFreeable(uint8_t* base, uint32_t set) {
  for (uint32_t pool = 0; pool < kFirstSetPools; pool++) {
    uint32_t node = Load(base, set + kFirstSetLists + 12 * pool);
    for (uint32_t n = 0; node != 0; n++) {
      if (n == kMaxNodes || !memory::IsPages(node)) {
        return false;
      }
      node = Load(base, node + 8);
    }
  }
  return true;
}

void DestroyA(uint8_t* base, uint32_t set, uint32_t sp,
              const std::function<void(uint32_t object, uint32_t frame)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820C3CF8 ScenePools::DestroyA");
  const uint32_t frame = sp - kDestroyAFrameBytes;
  Store(base, frame, sp);  // the back chain, for the destructors' guest calls
  for (uint32_t pool = 0; pool < kFirstSetPools; pool++) {
    const uint32_t list = pool * 12 + set + kFirstSetLists;
    for (;;) {
      const uint32_t object = vehicle_accessors::ListAt(base, list, 0);
      if (object == 0) {
        break;
      }
      // The pool's index is compared against 0 to 7 by branches that all
      // take this path.
      destroy(object, frame);
      Require(memory::IsPages(object) && memory::FreePages(object), "VirtualFree");
    }
  }
}

}  // namespace scene
