// The save devices' construction and teardown natively (save_devices.h,
// docs/functions/save-interface.md).

#include "save_interface/save_devices.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "user/network.h"

namespace save_interface {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

constexpr uint32_t kDelegateVtable = 0x82002850;  // a member delegate
constexpr uint32_t kFunctorBase = 0x82000ABC;     // the functors' base
constexpr uint32_t kDevicesUpdate = 0x820A4610;   // SaveDevices::Update
constexpr uint32_t kDevicesEvent = 0x820A4800;    // probably the event handler
constexpr uint32_t kInitialName = 0x820021E0;     // copied into +16
constexpr uint32_t kMallocMost = 0xFFFFF000;      // malloc's largest size

// operator new[] after the checks said it can: the layer aborts when out of
// memory, so it doesn't fail here.
uint32_t NewArray(uint8_t* base, uint32_t bytes, const char* what) {
  const auto block = memory::crt::OperatorNewArray(base, bytes);
  if (!block) {
    ITC_LOG_ERROR("{}: operator new[] refused {} bytes after its checks", what, bytes);
    return 0;
  }
  return *block;
}

}  // namespace

bool CanAllocBuffers(uint8_t* base, uint32_t saves) {
  return memory::crt::GetProcessHeap(base) != 0 &&
         uint64_t(saves) * kSaveRecordBytes <= kMallocMost;
}

void SlotAllocBuffers(uint8_t* base, uint32_t slot, uint32_t saves) {
  ITC_NATIVE_RUNNING("sub_820A4560 SaveSlot::AllocBuffers");
  Store(base, slot + 80, NewArray(base, saves * kSaveRecordBytes, "SaveSlot::AllocBuffers"));
  Store(base, slot + 84, NewArray(base, saves * 4, "SaveSlot::AllocBuffers"));
}

std::optional<uint32_t> SaveDevicesConstruct(
    uint8_t* base, uint32_t devices, uint32_t block, uint32_t saves, uint32_t frame,
    const std::function<void(uint32_t loop, uint32_t offset, uint32_t key, uint32_t delegate)>&
        add) {
  if (!CanAllocBuffers(base, saves)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A6160 SaveDevices::SaveDevices");
  const uint32_t tick = frame + 80;
  const uint32_t listener = frame + 96;
  Store(base, tick + 4, devices);
  Store(base, devices + 0, kDevicesVtable);
  Store(base, tick + 0, kDelegateVtable);
  Store(base, devices + 12, Load(base, block + 4));
  Store(base, tick + 8, kDevicesUpdate);
  Store(base, devices + 4, Load(base, block + 8));
  Store(base, devices + 8, Load(base, block + 12));
  add(Load(base, block + 0), 8, devices + 4, tick);
  // The tick handler's delegate destroyed (its base vtable), the listener's
  // built.
  Store(base, listener + 4, devices);
  Store(base, tick + 0, kFunctorBase);
  Store(base, listener + 0, kDelegateVtable);
  Store(base, listener + 8, kDevicesEvent);
  add(Load(base, block + 0), 4, devices + 8, listener);
  Store(base, listener + 0, kFunctorBase);
  // XNotifyCreateListener(1) left out: no listener (the user's call,
  // 2026-10-30).
  Store(base, devices + 56, user::kNoListener);
  Store(base, devices + 52, 512);
  Store(base, devices + 48, kSlotCount);
  const uint32_t array = NewArray(base, 8 + kSlotCount * kSlotBytes, "SaveDevices::SaveDevices");
  uint32_t slots = 0;
  if (array != 0) {
    Store(base, array + 0, kSlotCount);
    slots = array + 8;
    for (uint32_t i = 0; i < kSlotCount; i++) {
      const uint32_t slot = slots + kSlotBytes * i;
      Store(base, slot + 36, 0);
      Store(base, slot + 0, kSaveSlotVtable);
      for (uint32_t at = 960; at <= 984; at += 4) {
        Store(base, slot + at, 0);
      }
      Store64(base, slot + 40, 0);
      Store(base, slot + 988, 3);
      Store(base, slot + 96, 0);
      Store(base, slot + 948, 0);
      Store(base, slot + 992, 0);
    }
  }
  Store(base, devices + 44, slots);
  Store(base, devices + 68, saves);
  for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, devices + 48)); i++) {
    // SaveSlot::AllocBuffers' prologue (__savegprlr_29) below this frame:
    // the return address, r29 (i), r30 (0) and r31 (the save devices), and
    // its back chain.
    Store(base, frame - 8, 0x820A62F8);
    Store64(base, frame - 32, i);
    Store64(base, frame - 24, 0);
    Store64(base, frame - 16, devices);
    Store(base, frame - 112, frame);
    SlotAllocBuffers(base, Load(base, devices + 44) + kSlotBytes * i, Load(base, devices + 68));
  }
  Store(base, devices + 72, 0);
  Store(base, devices + 88, 0x820A58D8);
  Store(base, devices + 112, 0x820A6010);
  Store(base, devices + 60, 1024);
  Store(base, devices + 64, 1);
  Store(base, devices + 76, 0x820A4870);
  Store(base, devices + 80, 0x820A5878);
  Store(base, devices + 84, 0x820A4890);
  Store(base, devices + 92, 0x820A48B8);
  Store(base, devices + 96, 0x820A59C0);
  Store(base, devices + 100, 0x820A5A88);
  Store(base, devices + 104, 0x820A5BE8);
  Store(base, devices + 108, 0x820A5E68);
  for (uint32_t i = 0;; i++) {
    const uint8_t c = guest::LoadU8(base, kInitialName + i);
    guest::StoreU8(base, devices + 16 + i, c);
    if (c == 0) {
      break;
    }
  }
  return devices;
}

std::optional<uint32_t> SaveDevicesCreate(
    uint8_t* base, uint32_t saves, const std::function<void(uint32_t devices)>& construct) {
  if (!CanAllocBuffers(base, saves)) {
    return std::nullopt;
  }
  const auto devices = memory::crt::OperatorNew(base, 116);
  if (!devices) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A63C8 SaveDevices::Create");
  if (*devices != 0) {
    construct(*devices);
  }
  return *devices;
}

uint32_t SlotArrayReset(uint8_t* base, uint32_t holder,
                        const std::function<uint32_t(uint32_t slot)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820A55F0 SlotArray::Reset");
  const uint32_t slots = Load(base, holder);
  if (slots == 0) {
    return holder;
  }
  uint32_t r3 = slots - 8;
  if (Load(base, r3) != 0) {
    r3 = destroy(slots);
  } else if (!memory::crt::OperatorDeleteArray(r3)) {
    ITC_LOG_ERROR("SlotArray::Reset: operator delete[] refused {:08X}", r3);
  }
  Store(base, holder, 0);
  return r3;
}

uint32_t SaveDevicesDestroy(uint8_t* base, uint32_t devices,
                            const std::function<uint32_t(uint32_t slot)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820A56E8 SaveDevices::Destroy");
  Store(base, devices + 0, kDevicesVtable);
  // CloseHandle on +56 left out: no listener is made (user::kNoListener).
  uint32_t r3 = devices + 44;
  if (Load(base, r3) != 0) {
    r3 = SlotArrayReset(base, r3, destroy);
  }
  Store(base, devices + 0, kDevicesBaseVtable);
  return r3;
}

uint32_t SaveDevicesDelete(uint8_t* base, uint32_t devices, uint32_t flags,
                           const std::function<uint32_t(uint32_t slot)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820A6110 SaveDevices::~SaveDevices");
  SaveDevicesDestroy(base, devices, destroy);
  if ((flags & 1) != 0 && !memory::crt::OperatorDelete(devices)) {
    ITC_LOG_ERROR("SaveDevices::~SaveDevices: operator delete refused {:08X}", devices);
  }
  return devices;
}

}  // namespace save_interface
