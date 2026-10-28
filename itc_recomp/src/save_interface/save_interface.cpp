// The save system's interface natively (save_interface.h,
// docs/functions/save-interface.md).

#include "save_interface/save_interface.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "framework/framework.h"
#include "guest.h"

namespace save_interface {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// A byte copy with its terminator, as the request setters' loops.
void CopyBytes(uint8_t* base, uint32_t destination, uint32_t source) {
  for (;;) {
    const uint8_t c = guest::LoadU8(base, source++);
    guest::StoreU8(base, destination++, c);
    if (c == 0) {
      return;
    }
  }
}

// The interface's save devices, through its +12 (nullopt where a target
// isn't ours).
std::optional<uint32_t> Devices(uint8_t* base) {
  const uint32_t interface = Load(base, kInterfaceGlobal);
  if (VirtualTarget(base, interface, 12) != kAppDevices) {
    return std::nullopt;
  }
  return AppDevices(base, interface);
}

// The save devices when their slot at `offset` is `target`.
std::optional<uint32_t> DevicesWith(uint8_t* base, uint32_t offset, uint32_t target) {
  const auto devices = Devices(base);
  if (!devices || VirtualTarget(base, *devices, offset) != target) {
    return std::nullopt;
  }
  return devices;
}

// The interface's users, through its +8.
std::optional<uint32_t> Users(uint8_t* base) {
  const uint32_t interface = Load(base, kInterfaceGlobal);
  if (VirtualTarget(base, interface, 8) != kAppUsers) {
    return std::nullopt;
  }
  return AppUsers(base, interface);
}

// The request setters' common part: the slot's base re-read from the
// devices' +44 before every store, as the original does.
std::optional<uint64_t> RequestCommon(uint8_t* base, uint32_t devices, uint32_t slot,
                                      const Request& request, uint32_t thumbnail,
                                      uint32_t thumbnail_size, uint32_t word) {
  if (VirtualTarget(base, devices, 28) != kSlotState) {
    return std::nullopt;
  }
  if (SlotState(base, devices, slot) != 3) {
    return 0;
  }
  const uint32_t offset = slot * 1000;
  CopyDisplayName(base, Load(base, devices + 44) + offset + 504, request.display_name);
  CopyBytes(base, Load(base, devices + 44) + offset + 760, request.file_name);
  CopyBytes(base, Load(base, devices + 44) + offset + 802, request.path);
  Store(base, Load(base, devices + 44) + offset + 932, request.data);
  Store(base, Load(base, devices + 44) + offset + 936, request.size);
  Store(base, Load(base, devices + 44) + offset + 940, thumbnail);
  Store(base, Load(base, devices + 44) + offset + 944, thumbnail_size);
  Store(base, Load(base, devices + 44) + offset + word, 1);
  return 1;
}

}  // namespace

uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset) {
  return Load(base, Load(base, object) + offset);
}

std::optional<uint32_t> AppDevices(uint8_t* base, uint32_t interface) {
  const uint32_t object = Load(base, interface + 8);
  if (VirtualTarget(base, object, 40) != kFrameworkDevices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820943B0 App::SaveDevices");
  return framework::Word88(base, object);
}

std::optional<uint32_t> AppUsers(uint8_t* base, uint32_t interface) {
  const uint32_t object = Load(base, interface + 8);
  if (VirtualTarget(base, object, 36) != kFrameworkUsers) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820943C8 App::Users");
  return FrameworkUsers(base, object);
}

uint32_t FrameworkUsers(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82493700 word +92");
  return Load(base, object + 92);
}

uint32_t UsersCount(uint8_t* base, uint32_t users) {
  ITC_NATIVE_RUNNING("sub_82490B08 word +216");
  return Load(base, users + 216);
}

uint64_t UsersSignedIn(uint8_t* base, uint32_t users, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820996D8 Users::SignedIn");
  if (index < Load(base, users + 216)) {
    const uint32_t user = Load(base, users + ((index + 50) << 2));
    if (Load(base, uint32_t(users + uint64_t(user) * 40 + 8)) != 0) {
      return user;
    }
  }
  return ~uint64_t(0);
}

uint32_t SlotAddress(uint8_t* base, uint32_t devices, uint32_t slot) {
  return Load(base, devices + 44) + slot * 1000;
}

void SlotRequestDevice(uint8_t* base, uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5750 SaveSlot::RequestDevice");
  Store(base, SlotAddress(base, devices, slot) + 960, 1);
}

uint32_t SlotDevice(uint8_t* base, uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5768 SaveSlot::Device");
  return Load(base, SlotAddress(base, devices, slot) + 36);
}

uint64_t SlotFreeBytes(uint8_t* base, uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5780 SaveSlot::FreeBytes");
  const uint32_t at = SlotAddress(base, devices, slot) + 40;
  return (uint64_t(Load(base, at)) << 32) | Load(base, at + 4);
}

uint32_t SlotSaveCount(uint8_t* base, uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5798 SaveSlot::SaveCount");
  return Load(base, SlotAddress(base, devices, slot) + 984);
}

uint64_t SlotSaveAt(uint8_t* base, uint32_t devices, uint32_t slot, uint64_t index) {
  ITC_NATIVE_RUNNING("sub_820A57B0 SaveSlot::SaveAt");
  return uint64_t(Load(base, SlotAddress(base, devices, slot) + 80)) + index * 308;
}

uint32_t SlotState(uint8_t* base, uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A57F0 SaveSlot::State");
  return Load(base, SlotAddress(base, devices, slot) + 988);
}

uint64_t SlotPending(uint8_t* base, uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5808 SaveSlot::Pending");
  const uint32_t at = SlotAddress(base, devices, slot);
  uint64_t sum = 0;
  for (uint32_t word = 960; word <= 980; word += 4) {
    sum += Load(base, at + word);
  }
  return sum;
}

void SetRootName(uint8_t* base, uint32_t devices, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_820A5580 SaveDevices::SetRootName");
  CopyBytes(base, devices + 16, name);
}

std::optional<uint64_t> SlotRequestSave(uint8_t* base, uint32_t devices, uint32_t slot,
                                        const Request& request) {
  ITC_NATIVE_RUNNING("sub_820A52E0 SaveSlot::RequestSave");
  return RequestCommon(base, devices, slot, request, request.thumbnail, request.thumbnail_size,
                       968);
}

std::optional<uint64_t> SlotRequestLoad(uint8_t* base, uint32_t devices, uint32_t slot,
                                        const Request& request) {
  ITC_NATIVE_RUNNING("sub_820A53E8 SaveSlot::RequestLoad");
  return RequestCommon(base, devices, slot, request, 0, 0, 972);
}

void CopyDisplayName(uint8_t* base, uint32_t destination, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_82295D48 the display name's copy");
  c_runtime::Wcscpy(base, destination, source);
}

uint64_t ContentSize(uint32_t bytes, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82295C18 XContentCalculateSize");
  const uint32_t headers = ((((count << 6) + 4159) << 1) & 0xFFFFE000u);
  const uint32_t data = (bytes + 4095) & 0xFFFFF000u;
  const uint32_t blocks = ((headers + data) >> 12) & 0xFFFFF;
  const uint32_t first = (blocks + 169) / 170;
  const uint32_t second = first > 1 ? (first + 169) / 170 : 0;
  const uint32_t third = second > 1 ? (second + 169) / 170 : 0;
  const uint32_t total = ((third + second + first) << 1) + blocks;
  return (uint64_t(total) + 10) << 12;
}

std::optional<uint32_t> Init(uint8_t* base, uint32_t interface) {
  if (VirtualTarget(base, interface, 12) != kAppDevices) {
    return std::nullopt;
  }
  const uint32_t object = Load(base, interface + 8);
  if (VirtualTarget(base, object, 40) != kFrameworkDevices) {
    return std::nullopt;
  }
  if (VirtualTarget(base, Load(base, object + 88), 56) != kSetRootName) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8209FFB8 SaveInterface::Init");
  Store(base, kInterfaceGlobal, interface);
  const uint32_t devices = *AppDevices(base, interface);
  SetRootName(base, devices, kRootName);
  return devices;
}

std::optional<uint32_t> InterfaceSlotState(uint8_t* base, uint32_t slot) {
  const auto devices = DevicesWith(base, 28, kSlotState);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0018 SaveInterface::SlotState");
  const uint32_t state = SlotState(base, *devices, slot);
  return state <= 9 ? state : 0;
}

std::optional<uint32_t> InterfaceRequestDevice(uint8_t* base, uint32_t slot) {
  const auto devices = DevicesWith(base, 4, kSlotRequestDevice);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0208 SaveInterface::RequestDevice");
  SlotRequestDevice(base, *devices, slot);
  return devices;
}

std::optional<uint32_t> InterfaceSaveCount(uint8_t* base, uint32_t slot) {
  const auto devices = DevicesWith(base, 16, kSlotSaveCount);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A02C8 SaveInterface::SaveCount");
  return SlotSaveCount(base, *devices, slot);
}

std::optional<uint32_t> InterfaceFreeBytes(uint8_t* base, uint32_t slot) {
  const auto devices = DevicesWith(base, 12, kSlotFreeBytes);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0320 SaveInterface::FreeBytes");
  return uint32_t(SlotFreeBytes(base, *devices, slot));
}

std::optional<uint64_t> InterfacePending(uint8_t* base, uint32_t slot) {
  const auto devices = DevicesWith(base, 32, kSlotPending);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0378 SaveInterface::Pending");
  return SlotPending(base, *devices, slot);
}

std::optional<uint32_t> InterfaceDevice(uint8_t* base, uint32_t slot) {
  const auto devices = DevicesWith(base, 8, kSlotDevice);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A03D0 SaveInterface::Device");
  return SlotDevice(base, *devices, slot);
}

std::optional<uint64_t> InterfaceSaveAt(uint8_t* base, uint32_t slot, uint64_t index) {
  const auto devices = DevicesWith(base, 20, kSlotSaveAt);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0428 SaveInterface::SaveAt");
  return SlotSaveAt(base, *devices, slot, index);
}

std::optional<uint64_t> InterfaceSave(uint8_t* base, uint32_t slot, const Request& request) {
  const auto devices = DevicesWith(base, 60, kSlotRequestSave);
  if (!devices || VirtualTarget(base, *devices, 28) != kSlotState) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0490 SaveInterface::Save");
  return SlotRequestSave(base, *devices, slot, request);
}

std::optional<uint64_t> InterfaceLoad(uint8_t* base, uint32_t slot, const Request& request) {
  const auto devices = DevicesWith(base, 64, kSlotRequestLoad);
  if (!devices || VirtualTarget(base, *devices, 28) != kSlotState) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0510 SaveInterface::Load");
  return SlotRequestLoad(base, *devices, slot, request);
}

bool InterfaceRequestReady(uint8_t* base, bool load) {
  const auto devices =
      DevicesWith(base, load ? 64 : 60, load ? kSlotRequestLoad : kSlotRequestSave);
  return devices && VirtualTarget(base, *devices, 28) == kSlotState;
}

std::optional<uint32_t> InterfaceUserCount(uint8_t* base) {
  const auto users = Users(base);
  if (!users || VirtualTarget(base, *users, 4) != kUsersCount) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A05D8 SaveInterface::UserCount");
  return UsersCount(base, *users);
}

std::optional<uint64_t> InterfaceUserAt(uint8_t* base, uint32_t index) {
  const auto users = Users(base);
  if (!users || VirtualTarget(base, *users, 8) != kUsersSignedIn) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0620 SaveInterface::UserAt");
  return UsersSignedIn(base, *users, index);
}

}  // namespace save_interface
