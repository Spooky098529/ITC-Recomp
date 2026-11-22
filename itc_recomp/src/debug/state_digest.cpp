#include "state_digest.h"

#include "base/flags.h"
#include "base/hash.h"
#include "base/log.h"
#include "guest.h"
#include "input/physical_pad.h"
#include "memory/mapping.h"

#include <bit>
#include <cstdint>
#include <cstring>

ITC_FLAG_BOOL(state_digest, false, "ITC",
              "Log a hash of the racers' state each race tick, to compare runs");

namespace debug {
namespace {

// race-world.toml: the racers' manager and its racer list.
constexpr uint32_t kRacersManager = 0x826D82F4;
constexpr uint32_t kRacers = 2308;       // 7 pointers to MinimapRacerEntry
constexpr uint32_t kRacerCount = 2364;
// minimap.toml's MinimapRacerEntry.
constexpr uint32_t kEntryVehicleIndex = 20;
// vehicle-accessors.md's Vehicle::Get: [0x826C2E34] + 6,272 x index.
constexpr uint32_t kVehicles = 0x826C2E34;
constexpr uint32_t kVehicleStride = 6272;
// car-physics.toml's Vehicle: the body's matrix (its position in the last
// row) and the squared speed.
constexpr uint32_t kVehicleMatrix = 3312;
constexpr uint32_t kVehicleSpeedSquared = 2440;

uint64_t g_tick = 0;

float LoadFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(guest::LoadU32(base, address));
}

}  // namespace

void StateDigest_OnRaceEntered() {
  g_tick = 0;
}

void StateDigest_OnRaceTick() {
  if (!ITC_FLAG(state_digest)) {
    return;
  }
  uint8_t* base = memory::Base();
  const uint64_t tick = g_tick++;
  const uint32_t manager = guest::LoadU32(base, kRacersManager);
  const uint32_t vehicles = guest::LoadU32(base, kVehicles);
  if (manager == 0 || vehicles == 0) {
    ITC_LOG_INFO("state digest: tick {} no racers", tick);
    return;
  }
  uint32_t count = guest::LoadU32(base, manager + kRacerCount);
  if (count > 7) {
    count = 7;
  }
  uint64_t hash = count;
  float player[3] = {0, 0, 0};
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t entry = guest::LoadU32(base, manager + kRacers + 4 * i);
    if (entry == 0) {
      continue;
    }
    const uint32_t index = guest::LoadU32(base, entry + kEntryVehicleIndex);
    if (index == 0xFFFFFFFFu) {
      continue;
    }
    const uint32_t vehicle = vehicles + kVehicleStride * index;
    uint8_t state[64 + 4];
    std::memcpy(state, base + vehicle + kVehicleMatrix, 64);
    std::memcpy(state + 64, base + vehicle + kVehicleSpeedSquared, 4);
    hash = base::Hash64(state, sizeof(state), hash ^ (uint64_t(index) << 32));
    if (i == 0) {
      for (int k = 0; k < 3; k++) {
        player[k] = LoadFloat(base, vehicle + kVehicleMatrix + 48 + 4 * k);
      }
    }
  }
  ITC_LOG_INFO("state digest: tick {} racers {} hash {:016x} pad {:016x} player {:.3f} {:.3f} {:.3f}",
               tick, count, hash, input::PhysicalPad_LastReadHash(0), player[0], player[1],
               player[2]);
}

}  // namespace debug
