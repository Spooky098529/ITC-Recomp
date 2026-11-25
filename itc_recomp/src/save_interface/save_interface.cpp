// The save system's interface natively (save_interface.h,
// docs/functions/save-interface.md).

#include "save_interface/save_interface.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "framework/framework.h"
#include "guest.h"
#include "schema/app_start_up.h"
#include "schema/files_and_saves.h"
#include "schema/framework.h"
#include "schema/save_interface.h"

namespace save_interface {

namespace {

using G = schema::SaveInterfaceGlobals;
// The save slot (files-and-saves.toml's SaveSlot).
using schema::SaveSlot;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }

// A byte copy with its terminator, as the request setters' loops.
void CopyBytes(uint32_t destination, uint32_t source) {
  for (;;) {
    const uint8_t c = guest::LoadU8(source++);
    guest::StoreU8(destination++, c);
    if (c == 0) {
      return;
    }
  }
}

// The interface's save devices, through its +12 (nullopt where a target
// isn't ours).
std::optional<uint32_t> Devices() {
  const uint32_t interface = G::interface<schema::View>().address;
  if (VirtualTarget(interface, 12) != kAppDevices) {
    return std::nullopt;
  }
  return AppDevices(interface);
}

// The save devices when their slot at `offset` is `target`.
std::optional<uint32_t> DevicesWith(uint32_t offset, uint32_t target) {
  const auto devices = Devices();
  if (!devices || VirtualTarget(*devices, offset) != target) {
    return std::nullopt;
  }
  return devices;
}

// The interface's users, through its +8.
std::optional<uint32_t> Users() {
  const uint32_t interface = G::interface<schema::View>().address;
  if (VirtualTarget(interface, 8) != kAppUsers) {
    return std::nullopt;
  }
  return AppUsers(interface);
}

// The request setters' common part: the slot's base re-read from the
// devices' +44 before every store, as the original does; `set_request` the
// request word's setter (+968 the save's, +972 the load's).
std::optional<uint64_t> RequestCommon(uint32_t devices, uint32_t slot,
                                      const Request& request, uint32_t thumbnail,
                                      uint32_t thumbnail_size,
                                      void (SaveSlot::*set_request)(uint32_t) const) {
  if (VirtualTarget(devices, 28) != kSlotState) {
    return std::nullopt;
  }
  if (SlotState(devices, slot) != 3) {
    return 0;
  }
  const uint32_t offset = slot * 1000;
  const schema::SaveDevices d(devices);
  const auto s = [&] { return SaveSlot(d.slots<schema::View>().address + offset); };
  CopyDisplayName(s().display_name_address(), request.display_name);
  CopyBytes(s().file_name_address(), request.file_name);
  CopyBytes(s().file_path_address(), request.path);
  s().set_data(request.data);
  s().set_data_size(request.size);
  s().set_thumbnail(thumbnail);
  s().set_thumbnail_size(thumbnail_size);
  (s().*set_request)(1);
  return 1;
}

}  // namespace

uint32_t VirtualTarget(uint32_t object, uint32_t offset) {
  return Load(guest::VtableWord(object) + offset);
}

std::optional<uint32_t> AppDevices(uint32_t interface) {
  const uint32_t object =
      schema::AppInterface16(interface).framework_interface<schema::View>().address;
  if (VirtualTarget(object, 40) != kFrameworkDevices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820943B0 App::SaveDevices");
  return framework::Word88(object);
}

std::optional<uint32_t> AppUsers(uint32_t interface) {
  const uint32_t object =
      schema::AppInterface16(interface).framework_interface<schema::View>().address;
  if (VirtualTarget(object, 36) != kFrameworkUsers) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820943C8 App::Users");
  return FrameworkUsers(object);
}

uint32_t FrameworkUsers(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82493700 word +92");
  return schema::FrameworkGetterSelf(object).word_92();  // the framework may be in host memory
}

uint32_t UsersCount(uint32_t users) {
  ITC_NATIVE_RUNNING("sub_82490B08 word +216");
  return schema::SignedInUsers(users).count();
}

uint64_t UsersSignedIn(uint32_t users, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820996D8 Users::SignedIn");
  const schema::SignedInUsers u(users);
  if (index < u.count()) {
    const uint32_t user = Load(u.order_address() + (index << 2));
    if (Load(uint32_t(u.users_address() + uint64_t(user) * 40 + 8)) != 0) {
      return user;
    }
  }
  return ~uint64_t(0);
}

uint32_t SlotAddress(uint32_t devices, uint32_t slot) {
  return schema::SaveDevices(devices).slots<schema::View>().address + slot * 1000;
}

void SlotRequestDevice(uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5750 SaveSlot::RequestDevice");
  SaveSlot(SlotAddress(devices, slot)).set_request_device_selector(1);
}

uint32_t SlotDevice(uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5768 SaveSlot::Device");
  return SaveSlot(SlotAddress(devices, slot)).device();
}

uint64_t SlotFreeBytes(uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5780 SaveSlot::FreeBytes");
  // Two word loads, as the original.
  const uint32_t at = SaveSlot(SlotAddress(devices, slot)).device_free_bytes_address();
  return (uint64_t(Load(at)) << 32) | Load(at + 4);
}

uint32_t SlotSaveCount(uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5798 SaveSlot::SaveCount");
  return SaveSlot(SlotAddress(devices, slot)).enumerated_saves();
}

uint64_t SlotSaveAt(uint32_t devices, uint32_t slot, uint64_t index) {
  ITC_NATIVE_RUNNING("sub_820A57B0 SaveSlot::SaveAt");
  return uint64_t(SaveSlot(SlotAddress(devices, slot)).enumeration_buffer()) +
         index * schema::XContentData::kSize;
}

uint32_t SlotState(uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A57F0 SaveSlot::State");
  return SaveSlot(SlotAddress(devices, slot)).state();
}

uint64_t SlotPending(uint32_t devices, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A5808 SaveSlot::Pending");
  const SaveSlot s(SlotAddress(devices, slot));
  uint64_t sum = 0;
  sum += s.request_device_selector();
  sum += s.request_enumeration();
  sum += s.request_save();
  sum += s.request_load();
  sum += s.request_delete();
  sum += s.failure_acknowledged();
  return sum;
}

void SetRootName(uint32_t devices, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_820A5580 SaveDevices::SetRootName");
  // Byte by byte with the terminator, through the save devices' layout (in
  // host memory since E5 step 5).
  const schema::SaveDevices d(devices);
  for (uint32_t i = 0;; i++) {
    const uint8_t c = guest::LoadU8(name + i);
    d.store_at(d.root_name_address() - d.address + i, 1, c);
    if (c == 0) {
      return;
    }
  }
}

std::optional<uint64_t> SlotRequestSave(uint32_t devices, uint32_t slot,
                                        const Request& request) {
  ITC_NATIVE_RUNNING("sub_820A52E0 SaveSlot::RequestSave");
  return RequestCommon(devices, slot, request, request.thumbnail, request.thumbnail_size,
                       &SaveSlot::set_request_save);
}

std::optional<uint64_t> SlotRequestLoad(uint32_t devices, uint32_t slot,
                                        const Request& request) {
  ITC_NATIVE_RUNNING("sub_820A53E8 SaveSlot::RequestLoad");
  return RequestCommon(devices, slot, request, 0, 0, &SaveSlot::set_request_load);
}

void CopyDisplayName(uint32_t destination, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_82295D48 the display name's copy");
  c_runtime::Wcscpy(destination, source);
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

std::optional<uint32_t> Init(uint32_t interface) {
  if (VirtualTarget(interface, 12) != kAppDevices) {
    return std::nullopt;
  }
  const uint32_t object =
      schema::AppInterface16(interface).framework_interface<schema::View>().address;
  if (VirtualTarget(object, 40) != kFrameworkDevices) {
    return std::nullopt;
  }
  if (VirtualTarget(schema::FrameworkGetterSelf(object).word_88(), 56) !=
      kSetRootName) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8209FFB8 SaveInterface::Init");
  G::set_interface(interface);
  const uint32_t devices = *AppDevices(interface);
  SetRootName(devices, G::root_name_address());
  return devices;
}

std::optional<uint32_t> InterfaceSlotState(uint32_t slot) {
  const auto devices = DevicesWith(28, kSlotState);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0018 SaveInterface::SlotState");
  const uint32_t state = SlotState(*devices, slot);
  return state <= 9 ? state : 0;
}

std::optional<uint32_t> InterfaceRequestDevice(uint32_t slot) {
  const auto devices = DevicesWith(4, kSlotRequestDevice);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0208 SaveInterface::RequestDevice");
  SlotRequestDevice(*devices, slot);
  return devices;
}

std::optional<uint32_t> InterfaceSaveCount(uint32_t slot) {
  const auto devices = DevicesWith(16, kSlotSaveCount);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A02C8 SaveInterface::SaveCount");
  return SlotSaveCount(*devices, slot);
}

std::optional<uint32_t> InterfaceFreeBytes(uint32_t slot) {
  const auto devices = DevicesWith(12, kSlotFreeBytes);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0320 SaveInterface::FreeBytes");
  return uint32_t(SlotFreeBytes(*devices, slot));
}

std::optional<uint64_t> InterfacePending(uint32_t slot) {
  const auto devices = DevicesWith(32, kSlotPending);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0378 SaveInterface::Pending");
  return SlotPending(*devices, slot);
}

std::optional<uint32_t> InterfaceDevice(uint32_t slot) {
  const auto devices = DevicesWith(8, kSlotDevice);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A03D0 SaveInterface::Device");
  return SlotDevice(*devices, slot);
}

std::optional<uint64_t> InterfaceSaveAt(uint32_t slot, uint64_t index) {
  const auto devices = DevicesWith(20, kSlotSaveAt);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0428 SaveInterface::SaveAt");
  return SlotSaveAt(*devices, slot, index);
}

std::optional<uint64_t> InterfaceSave(uint32_t slot, const Request& request) {
  const auto devices = DevicesWith(60, kSlotRequestSave);
  if (!devices || VirtualTarget(*devices, 28) != kSlotState) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0490 SaveInterface::Save");
  return SlotRequestSave(*devices, slot, request);
}

std::optional<uint64_t> InterfaceLoad(uint32_t slot, const Request& request) {
  const auto devices = DevicesWith(64, kSlotRequestLoad);
  if (!devices || VirtualTarget(*devices, 28) != kSlotState) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0510 SaveInterface::Load");
  return SlotRequestLoad(*devices, slot, request);
}

bool InterfaceRequestReady(bool load) {
  const auto devices =
      DevicesWith(load ? 64 : 60, load ? kSlotRequestLoad : kSlotRequestSave);
  return devices && VirtualTarget(*devices, 28) == kSlotState;
}

std::optional<uint32_t> InterfaceUserCount() {
  const auto users = Users();
  if (!users || VirtualTarget(*users, 4) != kUsersCount) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A05D8 SaveInterface::UserCount");
  return UsersCount(*users);
}

std::optional<uint64_t> InterfaceUserAt(uint32_t index) {
  const auto users = Users();
  if (!users || VirtualTarget(*users, 8) != kUsersSignedIn) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A0620 SaveInterface::UserAt");
  return UsersSignedIn(*users, index);
}

}  // namespace save_interface
