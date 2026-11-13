// The save devices' slot handlers natively (slot_handlers.h,
// docs/functions/save-interface.md).

#include "save_interface/slot_handlers.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/last_error.h"
#include "save_interface/save_interface.h"
#include "saves/slot_steps.h"

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

}  // namespace

uint32_t WritePrologue(uint8_t* base, const CallSite& at, uint32_t frame_bytes, int first) {
  const uint64_t regs[5] = {at.r27, at.r28, at.r29, at.r30, at.r31};
  Store(base, at.sp - 8, at.lr);
  for (int n = first; n < 32; n++) {
    Store64(base, at.sp - 8 * (32 - n) - 8, regs[n - 27]);
  }
  Store(base, at.sp - frame_bytes, at.sp);
  return at.sp - frame_bytes;
}

uint32_t SlotWaitForUser(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4870 SaveSlot::WaitForUser");
  const int32_t user = int32_t(Load(base, slot + 956));
  return user >= 0 && user <= 3 ? 3 : 0;
}

uint32_t SlotFailed(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4890 SaveSlot::Failed");
  if (Load(base, slot + 980) == 0) {
    return 2;
  }
  Store(base, slot + 980, 0);
  Store(base, slot + 992, 0);
  return 3;
}

uint32_t SlotWaitForDevice(uint8_t* base, uint32_t slot, const CallSite& at) {
  ITC_NATIVE_RUNNING("sub_820A5878 SaveSlot::WaitForDevice");
  WritePrologue(base, at, 96, 31);
  if (Load(base, slot + 960) != 0) {
    return saves::slot::ShowDeviceSelector(base, slot) == 1 ? 4 : 1;
  }
  return Load(base, slot + 36) != 0 ? 3 : 1;
}

uint32_t SlotReady(uint8_t* base, uint32_t slot, const CallSite& at) {
  ITC_NATIVE_RUNNING("sub_820A58D8 SaveSlot::Ready");
  WritePrologue(base, at, 96, 31);
  if (Load(base, slot + 36) == 0) {
    return 1;
  }
  if (Load(base, slot + 960) != 0) {
    return saves::slot::ShowDeviceSelector(base, slot) == 1 ? 4 : 3;
  }
  if (Load(base, slot + 964) != 0) {
    Store(base, slot + 96, 1);
    Store(base, slot + 964, 0);
    return 5;
  }
  if (Load(base, slot + 968) != 0) {
    Store(base, slot + 948, 1);
    Store(base, slot + 968, 0);
    return 7;
  }
  if (Load(base, slot + 972) != 0) {
    Store(base, slot + 948, 1);
    Store(base, slot + 972, 0);
    return 8;
  }
  if (Load(base, slot + 976) != 0) {
    Store(base, slot + 948, 9);
    Store(base, slot + 976, 0);
    return 9;
  }
  return 3;
}

uint32_t SlotEnumerateStep(uint8_t* base, uint32_t devices, uint32_t slot, const CallSite& at) {
  ITC_NATIVE_RUNNING("sub_820A59C0 SaveSlot::EnumerateStep");
  WritePrologue(base, at, 112, 30);
  const uint32_t step = Load(base, slot + 96);
  if (step == 1) {
    const int32_t r3 = saves::slot::Enumerate(base, devices, slot);
    if (r3 == 1) {
      Store(base, slot + 96, 2);
    } else if (r3 == -2) {
      Store(base, slot + 96, 0);
      return 3;
    } else if (r3 == -4) {
      Store(base, slot + 984, 0);
      Store(base, slot + 96, 2);
    }
    return 5;
  }
  if (step == 2) {
    const int32_t r3 = saves::slot::WaitEnumerate(base, slot);
    if (r3 == 1) {
      Store(base, slot + 96, 0);
      Store(base, slot + 128, 1);
      return 6;
    }
    if (r3 == -6) {
      Store(base, slot + 96, 0);
      return 3;
    }
  }
  return 5;
}

void DevicesOnEvent(uint8_t* base, uint32_t devices, uint32_t loop, uint32_t event,
                    const CallSite& at,
                    const std::function<void(uint32_t offset, uint32_t key, const CallSite& at)>&
                        call) {
  ITC_NATIVE_RUNNING("sub_820A4800 SaveDevices::OnEvent");
  const uint32_t sp = WritePrologue(base, at, 112, 30);
  if (Load(base, event) != 1) {
    return;
  }
  CallSite site = at;
  site.sp = sp;
  site.r30 = devices;
  site.r31 = loop;
  site.lr = 0x820A4840;
  call(16, devices + 4, site);
  site.lr = 0x820A4858;
  call(12, devices + 8, site);
}

uint32_t FrameworkDeviceList(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820908D8 Framework::DeviceList");
  return FrameworkUsers(base, object - 20);
}

uint32_t XGetOverlappedResult(uint8_t* base, uint32_t overlapped, uint32_t result, uint32_t wait,
                              const CallSite& at, const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_82295C98 XGetOverlappedResult");
  const uint32_t sp = WritePrologue(base, at, 112, 30);
  if (Load(base, overlapped) == 997) {
    uint32_t r3 = 258;
    if (wait != 0) {
      const uint32_t event = Load(base, overlapped + 12);
      if (event != 0) {
        CallSite site = at;
        site.sp = sp;
        site.lr = 0x82295CDC;
        site.r30 = result;
        site.r31 = overlapped;
        r3 = kernel.wait(event, site);
      }
    }
    if (r3 == 258) {
      return 996;
    }
    if (r3 != 0) {
      return last_error::Get(base, kernel.pcr);
    }
  }
  if (result != 0) {
    Store(base, result, Load(base, overlapped + 4));
  }
  return Load(base, overlapped);
}

uint32_t GetOverlappedResult(uint8_t* base, uint32_t handle, uint32_t overlapped, uint32_t bytes,
                             uint32_t wait, const CallSite& at, const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_82295F20 GetOverlappedResult");
  const uint32_t sp = WritePrologue(base, at, 112, 30);
  if (Load(base, overlapped) == 259) {
    uint32_t r3 = 258;
    if (wait != 0) {
      const uint32_t event = Load(base, overlapped + 16);
      CallSite site = at;
      site.sp = sp;
      site.lr = 0x82295F68;
      site.r30 = bytes;
      site.r31 = overlapped;
      r3 = kernel.wait(event != 0 ? event : handle, site);
    }
    if (r3 == 258) {
      last_error::Set(base, kernel.pcr, 996);
      return 0;
    }
    if (r3 != 0) {
      return 0;
    }
  }
  Store(base, bytes, Load(base, overlapped + 4));
  const uint32_t status = Load(base, overlapped);
  if (int32_t(status) < 0) {
    last_error::SetFromStatus(base, kernel.pcr, status);
    return 0;
  }
  return 1;
}

}  // namespace save_interface
