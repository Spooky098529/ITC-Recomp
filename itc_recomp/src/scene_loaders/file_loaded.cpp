// The cockpit body's and the traffic body's texture binds (see
// file_loaded.h), load for load and store for store as the recompiled
// functions.

#include "scene_loaders/file_loaded.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "guest.h"
#include "models/part_model.h"
#include "models/xpr.h"
#include "scene_loaders/pools.h"

namespace scene_loaders {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kCockpitNames = 0x826018C8;  // the cars' names, by block
constexpr uint32_t kTrafficNames = 0x82601980;  // the traffic cars' names
constexpr uint32_t kTwoStrings = 0x82004FE8;    // "%s%s"
constexpr uint32_t kNumbered = 0x82004FA4;      // "%s%s%d"
constexpr uint32_t kInteOn = 0x82005008;        // "_inte_on"
constexpr uint32_t kInteOnNumbered = 0x82004FFC;  // "_inte_on_0"
constexpr uint32_t kTrafficSuffix = 0x820050AC;  // "_ex_0"

// sprintf into the frame's +80, called with r5 to r7 = `args` (64-bit, as
// the registers hold them; r8 to r10, which the calls don't set, as 0: only
// dead frame words differ), spilled into the frame's parameter area (+32,
// 8-byte slots) as sprintf spills them, its own frames below.
bool Print(uint8_t* base, uint32_t frame, uint32_t format, const uint64_t (&args)[3]) {
  for (uint32_t i = 0; i < 6; i++) {
    const uint64_t value = i < 3 ? args[i] : 0;
    Store(base, frame + 32 + 8 * i, uint32_t(value >> 32));
    Store(base, frame + 36 + 8 * i, uint32_t(value));
  }
  int32_t written;
  return c_runtime::Sprintf(base, frame + 80, format, frame + 32, frame - 144 - 1328, &written);
}

// A lis/addi constant as the register holds it (sign-extended).
uint64_t Constant(uint32_t address) { return uint64_t(int64_t(int32_t(address))); }

bool Loading(uint8_t* base, uint32_t state) { return int32_t(Load(base, state)) == 2; }

}  // namespace

std::optional<uint32_t> CockpitBindTextures(uint8_t* base, uint32_t cockpit, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D6108 Cockpit::BindTextures");
  for (uint32_t i = 0; i < 6; i++) {
    if (Loading(base, cockpit + 556 + 24 * i)) {
      return 0;
    }
  }
  for (uint32_t i = 0; i < 2; i++) {
    if (Loading(base, cockpit + 420 + 24 * i)) {
      return 0;
    }
  }
  if (Loading(base, cockpit + 196)) {
    return 0;
  }
  const uint32_t frame = stack - kCockpitBindFrameBytes;
  const uint32_t number = Load(base, cockpit + 48);
  const int32_t block = int32_t(Load(base, Load(base, cockpit + 24) + 32));
  const uint32_t name = Load(base, (uint32_t(block) << 2) + kCockpitNames);
  const bool printed =
      number == 0 ? Print(base, frame, kTwoStrings, {name, Constant(kInteOn), number})
                  : Print(base, frame, kNumbered, {name, Constant(kInteOnNumbered), number});
  if (!printed) {
    return std::nullopt;
  }
  const uint32_t xpr = cockpit + 448;
  int32_t index = -1;
  if (block < 18) {
    index = XprFindByName(base, xpr, frame + 80);
    Store(base, cockpit + 3504, uint32_t(models::XprGetTexture(base, xpr, uint32_t(index))));
  }
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t part = cockpit + 688 + 464 * i;
    Store(base, part + 48, cockpit + 496);
    if (block < 18) {
      Store(base, part + 452, uint32_t(models::XprGetTexture(base, xpr, uint32_t(index))));
    }
  }
  return 1;
}

std::optional<uint32_t> TrafficBindTextures(uint8_t* base, uint32_t traffic, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D7D38 TrafficCar::BindTextures");
  if (Loading(base, traffic + 3764)) {
    return 0;
  }
  const uint32_t frame = stack - kTrafficBindFrameBytes;
  const uint32_t xpr = traffic + 4016;
  for (uint32_t i = 0; i < 6; i++) {
    const uint32_t part = traffic + 4256 + 464 * i;
    Store(base, part + 48, traffic + 4064);
    models::PartModelSetVinylTextures(base, part, Load(base, traffic + 3728), 0);
    const uint32_t number = Load(base, traffic + 192);
    if (int32_t(number) <= 0 || int32_t(Load(base, traffic + 7056)) == -1) {
      continue;
    }
    const uint32_t name = Load(base, (Load(base, traffic + 32) << 2) + kTrafficNames);
    if (!Print(base, frame, kNumbered, {name, Constant(kTrafficSuffix), number})) {
      return std::nullopt;
    }
    const int32_t index = XprFindByName(base, xpr, frame + 80);
    Store(base, traffic + 7060, uint32_t(index));
    if (index == -1) {
      continue;
    }
    Store(base, part + 460, uint32_t(models::XprGetTexture(base, xpr, uint32_t(index))));
    Store(base, part + 436, Load(base, traffic + 7056));
  }
  return 1;
}

}  // namespace scene_loaders
