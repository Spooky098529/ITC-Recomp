// The save devices' construction and teardown natively (save_devices.h,
// docs/functions/save-interface.md).

#include "save_interface/save_devices.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/files_and_saves.h"
#include "schema/save_interface.h"
#include "user/network.h"

namespace save_interface {

namespace {

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}

constexpr uint32_t kDelegateVtable = 0x82002850;  // a member delegate
constexpr uint32_t kFunctorBase = 0x82000ABC;     // the functors' base
constexpr uint32_t kDevicesUpdate = 0x820A4610;   // SaveDevices::Update
constexpr uint32_t kDevicesEvent = 0x820A4800;    // probably the event handler
constexpr uint32_t kInitialName = 0x820021E0;     // copied into +16
constexpr uint32_t kMallocMost = 0xFFFFF000;      // malloc's largest size

}  // namespace

bool CanAllocBuffers(uint32_t saves) {
  return memory::crt::GetProcessHeap() != 0 &&
         uint64_t(saves) * kSaveRecordBytes <= kMallocMost;
}

void SlotAllocBuffers(uint32_t slot, uint32_t saves) {
  ITC_NATIVE_RUNNING("sub_820A4560 SaveSlot::AllocBuffers");
  const schema::SaveSlot s(slot);
  // operator new[] of each, as blobs in PC memory (E5 step 5): the slot
  // steps write and read them by address.
  s.set_enumeration_buffer(
      schema::SaveEnumerationBuffer::New(saves * kSaveRecordBytes).address);
  s.set_open_failed(schema::SaveOpenFlags::New(saves * 4).address);
}

std::optional<uint32_t> SaveDevicesConstruct(
    uint32_t devices, uint32_t block, uint32_t saves, uint32_t frame,
    const std::function<void(uint32_t loop, uint32_t offset, uint32_t key, uint32_t delegate)>&
        add) {
  if (!CanAllocBuffers(saves)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A6160 SaveDevices::SaveDevices");
  const schema::SaveDevices d(devices);
  const schema::SaveDevicesSetUp setup(block);
  const uint32_t tick = frame + 80;
  const uint32_t listener = frame + 96;
  Store(tick + 4, devices);
  d.set_vtable(kDevicesVtable);
  Store(tick + 0, kDelegateVtable);
  d.set_device_list(setup.device_list());
  Store(tick + 8, kDevicesUpdate);
  d.set_tick_key(setup.tick_key());
  d.set_listener_key(setup.listener_key());
  add(setup.run_loop<schema::View>().address, 8, d.tick_key_address(), tick);
  // The tick handler's delegate destroyed (its base vtable), the listener's
  // built.
  Store(listener + 4, devices);
  Store(tick + 0, kFunctorBase);
  Store(listener + 0, kDelegateVtable);
  Store(listener + 8, kDevicesEvent);
  add(setup.run_loop<schema::View>().address, 4, d.listener_key_address(), listener);
  Store(listener + 0, kFunctorBase);
  // XNotifyCreateListener(1) left out: no listener (the user's call,
  // 2026-10-30).
  d.set_listener(user::kNoListener);
  d.set_device_selector_flags(512);
  d.set_slot_count(kSlotCount);
  // operator new[] of the four slots and their count, as a blob in PC memory
  // (E5 step 5): the slots stay in the guest's layout, reached by address.
  const uint32_t array =
      schema::SaveSlotArrayBlock::New(schema::SaveSlotArrayBlock::kSize).address;
  uint32_t slots = 0;
  if (array != 0) {
    const schema::SaveSlotArrayBlock block_of_slots(array);
    block_of_slots.set_count(kSlotCount);
    slots = block_of_slots.slots_address();
    for (uint32_t i = 0; i < kSlotCount; i++) {
      const schema::SaveSlot slot(slots + kSlotBytes * i);
      slot.set_device(0);
      slot.set_vtable(kSaveSlotVtable);
      slot.set_request_device_selector(0);
      slot.set_request_enumeration(0);
      slot.set_request_save(0);
      slot.set_request_load(0);
      slot.set_request_delete(0);
      slot.set_failure_acknowledged(0);
      slot.set_enumerated_saves(0);
      // Two word stores, as the original.
      Store64(slot.device_free_bytes_address(), 0);
      slot.set_state(3);
      slot.set_enumeration_step(0);
      slot.set_step(0);
      slot.set_failure_reason(0);
    }
  }
  d.set_slots(slots);
  d.set_saves_to_enumerate(saves);
  for (uint32_t i = 0; int32_t(i) < int32_t(d.slot_count()); i++) {
    // SaveSlot::AllocBuffers' prologue (__savegprlr_29) below this frame:
    // the return address, r29 (i), r30 (0) and r31 (the save devices), and
    // its back chain.
    Store(frame - 8, 0x820A62F8);
    Store64(frame - 32, i);
    Store64(frame - 24, 0);
    Store64(frame - 16, devices);
    Store(frame - 112, frame);
    SlotAllocBuffers(d.slots<schema::View>().address + kSlotBytes * i,
                     d.saves_to_enumerate());
  }
  d.set_busy_slot(0);
  d.set_ready(0x820A58D8);
  d.set_delete_step(0x820A6010);
  d.set_content_bytes(1024);
  d.set_content_count(1);
  d.set_wait_for_user(0x820A4870);
  d.set_wait_for_device(0x820A5878);
  d.set_failed(0x820A4890);
  d.set_wait_device_selector(0x820A48B8);
  d.set_enumerate_step(0x820A59C0);
  d.set_check_step(0x820A5A88);
  d.set_save_step(0x820A5BE8);
  d.set_load_step(0x820A5E68);
  for (uint32_t i = 0;; i++) {
    const uint8_t c = guest::LoadU8(kInitialName + i);
    d.store_at(d.root_name_address() - d.address + i, 1, c);  // host memory: its layout
    if (c == 0) {
      break;
    }
  }
  return devices;
}

std::optional<uint32_t> SaveDevicesCreate(
    uint32_t saves, const std::function<void(uint32_t devices)>& construct) {
  if (!CanAllocBuffers(saves)) {
    return std::nullopt;
  }
  // operator new(116), in host memory (E5 step 5).
  const uint32_t devices = schema::SaveDevices::New().address;
  ITC_NATIVE_RUNNING("sub_820A63C8 SaveDevices::Create");
  construct(devices);
  return devices;
}

uint32_t SlotArrayReset(uint32_t holder,
                        const std::function<uint32_t(uint32_t slot)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820A55F0 SlotArray::Reset");
  const schema::SaveSlotArrayHolder h(holder);
  const uint32_t slots = h.slots<schema::View>().address;
  if (slots == 0) {
    return holder;
  }
  uint32_t r3 = schema::SaveSlot(slots).array_count_address();
  if (schema::SaveSlot(slots).array_count() != 0) {
    r3 = destroy(slots);
  } else if (!memory::crt::OperatorDeleteArray(r3)) {
    ITC_LOG_ERROR("SlotArray::Reset: operator delete[] refused {:08X}", r3);
  }
  h.set_slots(0u);
  return r3;
}

uint32_t SaveDevicesDestroy(uint32_t devices,
                            const std::function<uint32_t(uint32_t slot)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820A56E8 SaveDevices::Destroy");
  const schema::SaveDevices d(devices);
  d.set_vtable(kDevicesVtable);
  // CloseHandle on +56 left out: no listener is made (user::kNoListener).
  uint32_t r3 = d.slots_address();
  if (d.slots<schema::View>().address != 0) {
    r3 = SlotArrayReset(r3, destroy);
  }
  d.set_vtable(kDevicesBaseVtable);
  return r3;
}

uint32_t SaveDevicesDelete(uint32_t devices, uint32_t flags,
                           const std::function<uint32_t(uint32_t slot)>& destroy) {
  ITC_NATIVE_RUNNING("sub_820A6110 SaveDevices::~SaveDevices");
  SaveDevicesDestroy(devices, destroy);
  if ((flags & 1) != 0 && !memory::crt::OperatorDelete(devices)) {
    ITC_LOG_ERROR("SaveDevices::~SaveDevices: operator delete refused {:08X}", devices);
  }
  return devices;
}

}  // namespace save_interface
