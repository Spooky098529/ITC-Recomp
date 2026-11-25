// The save devices' slot handlers natively (slot_handlers.h,
// docs/functions/save-interface.md).

#include "save_interface/slot_handlers.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/last_error.h"
#include "save_interface/save_interface.h"
#include "saves/slot_steps.h"
#include "schema/achievements.h"
#include "schema/c_runtime_core.h"
#include "schema/files_and_saves.h"
#include "schema/save_interface.h"

namespace save_interface {

namespace {

// The save slot (files-and-saves.toml's SaveSlot), and the XOVERLAPPED
// (achievements.toml's AchievementOverlapped) and Win32 OVERLAPPED
// (c-runtime-core.toml's CrtOverlapped) the result reads take.
using schema::SaveSlot;
using XOverlapped = schema::AchievementOverlapped;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}

}  // namespace

uint32_t WritePrologue(const CallSite& at, uint32_t frame_bytes, int first) {
  const uint64_t regs[5] = {at.r27, at.r28, at.r29, at.r30, at.r31};
  Store(at.sp - 8, at.lr);
  for (int n = first; n < 32; n++) {
    Store64(at.sp - 8 * (32 - n) - 8, regs[n - 27]);
  }
  Store(at.sp - frame_bytes, at.sp);
  return at.sp - frame_bytes;
}

uint32_t SlotWaitForUser(uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4870 SaveSlot::WaitForUser");
  const int32_t user = int32_t(SaveSlot(slot).user());
  return user >= 0 && user <= 3 ? 3 : 0;
}

uint32_t SlotFailed(uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_820A4890 SaveSlot::Failed");
  if (SaveSlot(slot).failure_acknowledged() == 0) {
    return 2;
  }
  SaveSlot(slot).set_failure_acknowledged(0);
  SaveSlot(slot).set_failure_reason(0);
  return 3;
}

uint32_t SlotWaitForDevice(uint32_t slot, const CallSite& at) {
  ITC_NATIVE_RUNNING("sub_820A5878 SaveSlot::WaitForDevice");
  WritePrologue(at, 96, 31);
  if (SaveSlot(slot).request_device_selector() != 0) {
    return saves::slot::ShowDeviceSelector(slot) == 1 ? 4 : 1;
  }
  return SaveSlot(slot).device() != 0 ? 3 : 1;
}

uint32_t SlotReady(uint32_t slot, const CallSite& at) {
  ITC_NATIVE_RUNNING("sub_820A58D8 SaveSlot::Ready");
  WritePrologue(at, 96, 31);
  if (SaveSlot(slot).device() == 0) {
    return 1;
  }
  if (SaveSlot(slot).request_device_selector() != 0) {
    return saves::slot::ShowDeviceSelector(slot) == 1 ? 4 : 3;
  }
  if (SaveSlot(slot).request_enumeration() != 0) {
    SaveSlot(slot).set_enumeration_step(1);
    SaveSlot(slot).set_request_enumeration(0);
    return 5;
  }
  if (SaveSlot(slot).request_save() != 0) {
    SaveSlot(slot).set_step(1);
    SaveSlot(slot).set_request_save(0);
    return 7;
  }
  if (SaveSlot(slot).request_load() != 0) {
    SaveSlot(slot).set_step(1);
    SaveSlot(slot).set_request_load(0);
    return 8;
  }
  if (SaveSlot(slot).request_delete() != 0) {
    SaveSlot(slot).set_step(9);
    SaveSlot(slot).set_request_delete(0);
    return 9;
  }
  return 3;
}

uint32_t SlotEnumerateStep(uint32_t devices, uint32_t slot, const CallSite& at) {
  ITC_NATIVE_RUNNING("sub_820A59C0 SaveSlot::EnumerateStep");
  WritePrologue(at, 112, 30);
  const uint32_t step = SaveSlot(slot).enumeration_step();
  if (step == 1) {
    const int32_t r3 = saves::slot::Enumerate(devices, slot);
    if (r3 == 1) {
      SaveSlot(slot).set_enumeration_step(2);
    } else if (r3 == -2) {
      SaveSlot(slot).set_enumeration_step(0);
      return 3;
    } else if (r3 == -4) {
      SaveSlot(slot).set_enumerated_saves(0);
      SaveSlot(slot).set_enumeration_step(2);
    }
    return 5;
  }
  if (step == 2) {
    const int32_t r3 = saves::slot::WaitEnumerate(slot);
    if (r3 == 1) {
      SaveSlot(slot).set_enumeration_step(0);
      SaveSlot(slot).set_check_step(1);
      return 6;
    }
    if (r3 == -6) {
      SaveSlot(slot).set_enumeration_step(0);
      return 3;
    }
  }
  return 5;
}

void DevicesOnEvent(uint32_t devices, uint32_t loop, uint32_t event,
                    const CallSite& at,
                    const std::function<void(uint32_t offset, uint32_t key, const CallSite& at)>&
                        call) {
  ITC_NATIVE_RUNNING("sub_820A4800 SaveDevices::OnEvent");
  const uint32_t sp = WritePrologue(at, 112, 30);
  if (Load(event) != 1) {
    return;
  }
  CallSite site = at;
  site.sp = sp;
  site.r30 = devices;
  site.r31 = loop;
  site.lr = 0x820A4840;
  call(16, schema::SaveDevices(devices).tick_key_address(), site);
  site.lr = 0x820A4858;
  call(12, schema::SaveDevices(devices).listener_key_address(), site);
}

uint32_t FrameworkDeviceList(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820908D8 Framework::DeviceList");
  return FrameworkUsers(object - 20);
}

uint32_t XGetOverlappedResult(uint32_t overlapped, uint32_t result, uint32_t wait,
                              const CallSite& at, const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_82295C98 XGetOverlappedResult");
  const uint32_t sp = WritePrologue(at, 112, 30);
  const XOverlapped o(overlapped);
  if (o.internal_low() == 997) {
    uint32_t r3 = 258;
    if (wait != 0) {
      const uint32_t event = o.event();
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
      return last_error::Get(kernel.pcr);
    }
  }
  if (result != 0) {
    Store(result, o.internal_high());
  }
  return o.internal_low();
}

uint32_t GetOverlappedResult(uint32_t handle, uint32_t overlapped, uint32_t bytes,
                             uint32_t wait, const CallSite& at, const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_82295F20 GetOverlappedResult");
  const uint32_t sp = WritePrologue(at, 112, 30);
  const schema::CrtOverlapped o(overlapped);
  if (o.internal() == 259) {
    uint32_t r3 = 258;
    if (wait != 0) {
      const uint32_t event = o.event();
      CallSite site = at;
      site.sp = sp;
      site.lr = 0x82295F68;
      site.r30 = bytes;
      site.r31 = overlapped;
      r3 = kernel.wait(event != 0 ? event : handle, site);
    }
    if (r3 == 258) {
      last_error::Set(kernel.pcr, 996);
      return 0;
    }
    if (r3 != 0) {
      return 0;
    }
  }
  Store(bytes, o.internal_high());
  const uint32_t status = o.internal();
  if (int32_t(status) < 0) {
    last_error::SetFromStatus(kernel.pcr, status);
    return 0;
  }
  return 1;
}

namespace {

// A call from a function whose frame is at `sp`, with its r29 to r31.
CallSite From(const CallSite& at, uint32_t sp, uint32_t lr, uint64_t r29, uint64_t r30,
              uint64_t r31) {
  CallSite site = at;
  site.sp = sp;
  site.lr = lr;
  site.r29 = r29;
  site.r30 = r30;
  site.r31 = r31;
  return site;
}

// WaitCloseItem, WaitDelete and WaitThumbnail: XGetOverlappedResult on
// `overlapped` from a 96-byte frame (no saves), its call returning to `lr`.
int32_t PollOverlapped(uint32_t overlapped, int32_t error, uint32_t lr,
                       const CallSite& at, const KernelCalls& kernel) {
  const uint32_t sp = WritePrologue(at, 96, 32);
  CallSite site = at;
  site.sp = sp;
  site.lr = lr;
  const uint32_t r3 = XGetOverlappedResult(overlapped, 0, 0, site, kernel);
  if (r3 == 0) {
    return 1;
  }
  return r3 == 996 ? 0 : error;
}

// WaitWrite and WaitRead: GetOverlappedResult, then GetLastError.
// `full` the answer for a last error of 112 (the write's), else `error`.
int32_t PollFile(uint32_t slot, int32_t error, int32_t full, uint32_t lr,
                 const CallSite& at, const KernelCalls& kernel) {
  const uint32_t sp = WritePrologue(at, 112, 30);
  const uint32_t overlapped = SaveSlot(slot).file_overlapped_address();
  const uint32_t done =
      GetOverlappedResult(SaveSlot(slot).file_handle(), overlapped, SaveSlot(slot).file_bytes_address(), 0,
                          From(at, sp, lr, at.r29, at.r30, overlapped), kernel);
  const uint32_t last = last_error::Get(kernel.pcr);
  if (done != 0) {
    return 1;
  }
  if (last == 996 || last == 997) {
    return 0;
  }
  return last == 112 ? full : error;
}

// The machines' claim on the save devices' +72: true when this slot holds
// it.
bool Claim(uint32_t devices, uint32_t slot) {
  if (schema::SaveDevices(devices).busy_slot() == 0) {
    schema::SaveDevices(devices).set_busy_slot(slot);
  }
  return schema::SaveDevices(devices).busy_slot() == slot;
}

}  // namespace

int32_t SlotWaitCloseItem(uint32_t slot, const CallSite& at,
                          const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A4C08 SaveSlot::WaitCloseItem");
  return PollOverlapped(SaveSlot(slot).open_address(), -8, 0x820A4C28, at, kernel);
}

int32_t SlotWaitDelete(uint32_t slot, const CallSite& at,
                       const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5290 SaveSlot::WaitDelete");
  return PollOverlapped(SaveSlot(slot).content_op_address(), -12, 0x820A52B0, at, kernel);
}

int32_t SlotWaitThumbnail(uint32_t slot, const CallSite& at,
                          const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5190 SaveSlot::WaitThumbnail");
  return PollOverlapped(SaveSlot(slot).content_op_address(), -9, 0x820A51B0, at, kernel);
}

int32_t SlotWaitClose(uint32_t slot, const CallSite& at,
                      const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5070 SaveSlot::WaitClose");
  const uint32_t sp = WritePrologue(at, 96, 31);
  CallSite site = at;
  site.sp = sp;
  site.r31 = slot;
  site.lr = 0x820A5094;
  const uint32_t r3 = XGetOverlappedResult(SaveSlot(slot).content_op_address(), 0, 0, site, kernel);
  if (r3 == 996) {
    return 0;
  }
  const bool done = r3 == 0;
  const uint32_t file = SaveSlot(slot).file_handle();
  if (int32_t(file) != -1) {
    site.lr = done ? 0x820A50AC : 0x820A50DC;
    kernel.close(file, site);
  }
  const uint32_t event = SaveSlot(slot).file_event();
  if (event != 0) {
    site.lr = done ? 0x820A50BC : 0x820A50EC;
    kernel.close(event, site);
  }
  return done ? 1 : -8;
}

int32_t SlotWaitWrite(uint32_t slot, const CallSite& at,
                      const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A4E68 SaveSlot::WaitWrite");
  return PollFile(slot, -9, -10, 0x820A4E94, at, kernel);
}

int32_t SlotWaitRead(uint32_t slot, const CallSite& at, const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A4FF8 SaveSlot::WaitRead");
  return PollFile(slot, -11, -11, 0x820A5024, at, kernel);
}

uint32_t SlotCheckStep(uint32_t devices, uint32_t slot, const CallSite& at,
                       const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5A88 SaveSlot::CheckStep");
  const uint32_t sp = WritePrologue(at, 112, 29);
  if (!Claim(devices, slot)) {
    return 6;
  }
  const auto next_item = [&] { SaveSlot(slot).set_check_item(SaveSlot(slot).check_item() + 1); };
  switch (SaveSlot(slot).check_step()) {
    case 1: {
      for (uint32_t i = 0; i < SaveSlot(slot).enumerated_saves(); i++) {
        Store(SaveSlot(slot).open_failed() + 4 * i, 1);
      }
      SaveSlot(slot).set_check_item(0);
      SaveSlot(slot).set_check_step(2);
      return 6;
    }
    case 2: {
      const uint32_t item = SaveSlot(slot).check_item();
      if (item < SaveSlot(slot).enumerated_saves()) {
        const int32_t r3 = saves::slot::OpenItem(slot, item);
        if (r3 == 1) {
          SaveSlot(slot).set_check_step(3);
          return 6;
        }
        if (r3 != -2) {
          next_item();
          return 6;
        }
      }
      SaveSlot(slot).set_check_step(0);
      schema::SaveDevices(devices).set_busy_slot(0);
      return 3;
    }
    case 3: {
      const int32_t r3 = saves::slot::WaitOpenItem(slot);
      if (r3 == 1) {
        Store(SaveSlot(slot).open_failed() + 4 * SaveSlot(slot).check_item(), 0);
        SaveSlot(slot).set_check_step(4);
      } else if (r3 == -7) {
        SaveSlot(slot).set_check_step(2);
        next_item();
      }
      return 6;
    }
    case 4: {
      if (SlotWaitCloseItem(slot, From(at, sp, 0x820A5AEC, 6, devices, slot), kernel) !=
          0) {
        SaveSlot(slot).set_check_step(2);
        next_item();
      }
      return 6;
    }
    default:
      return 6;
  }
}

uint32_t SlotSaveStep(uint32_t devices, uint32_t slot, const CallSite& at,
                      const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5BE8 SaveSlot::SaveStep");
  const uint32_t sp = WritePrologue(at, 112, 29);
  if (!Claim(devices, slot)) {
    return 7;
  }
  const auto site = [&](uint32_t lr) { return From(at, sp, lr, devices, 7, slot); };
  // The end of a step that failed or finished: the state given back.
  const auto release = [&] { schema::SaveDevices(devices).set_busy_slot(0); };
  // A file step's error: +952 (1 failed, 2 no room), then the close.
  const auto to_close = [&](uint32_t error) {
    SaveSlot(slot).set_step_error(error);
    SaveSlot(slot).set_step(5);
    return 7u;
  };
  // After the close's or the thumbnail's wait (`r3` its answer, not 0):
  // the enumeration asked for again and the machine left (`thumbnail`:
  // with no error and a thumbnail (+940), on to it instead).
  const auto finish = [&](int32_t r3, bool thumbnail) -> uint32_t {
    if (r3 != 1 && int32_t(SaveSlot(slot).step_error()) == 0) {
      SaveSlot(slot).set_step_error(1);
    }
    const int32_t error = int32_t(SaveSlot(slot).step_error());
    if (thumbnail && error == 0 && SaveSlot(slot).thumbnail() != 0) {
      SaveSlot(slot).set_step(7);
      return 7;
    }
    SaveSlot(slot).set_request_enumeration(1);
    SaveSlot(slot).set_step(0);
    if (error == 0) {
      release();
      return 3;
    }
    SaveSlot(slot).set_failure_reason(error == 1 ? 2 : 3);
    release();
    return 2;
  };
  switch (SaveSlot(slot).step()) {
    case 1: {
      const int32_t r3 = saves::slot::Create(devices, slot, 1);
      SaveSlot(slot).set_step_error(0);
      if (r3 == 1) {
        SaveSlot(slot).set_step(2);
        return 7;
      }
      if (r3 == -2) {
        SaveSlot(slot).set_step(0);
        release();
        return 3;
      }
      if (r3 == -5 || r3 == -99) {
        SaveSlot(slot).set_step(0);
        SaveSlot(slot).set_failure_reason(2);
        release();
        return 2;
      }
      if (r3 == -10) {
        SaveSlot(slot).set_failure_reason(3);
        SaveSlot(slot).set_step(0);
        release();
        return 2;
      }
      return 7;
    }
    case 2: {
      const int32_t r3 = saves::slot::WriteFile(slot);
      if (r3 == 1) {
        SaveSlot(slot).set_step(3);
        return 7;
      }
      if (r3 == -3 || r3 == -7 || r3 == -99) {
        return to_close(1);
      }
      return 7;
    }
    case 3: {
      const int32_t r3 = SlotWaitWrite(slot, site(0x820A5D90), kernel);
      if (r3 == 1) {
        SaveSlot(slot).set_step(5);
        return 7;
      }
      if (r3 == -9) {
        return to_close(1);
      }
      if (r3 == -10) {
        return to_close(2);
      }
      return 7;
    }
    case 5:
      saves::slot::ContentClose(SaveSlot(slot).content_op_address());
      SaveSlot(slot).set_step(6);
      return 7;
    case 6: {
      const int32_t r3 = SlotWaitClose(slot, site(0x820A5D08), kernel);
      return r3 == 0 ? 7 : finish(r3, true);
    }
    case 7: {
      if (saves::slot::SetThumbnail(slot) == 1) {
        SaveSlot(slot).set_step(8);
        return 7;
      }
      SaveSlot(slot).set_step(0);
      SaveSlot(slot).set_failure_reason(2);
      release();
      return 2;
    }
    case 8: {
      const int32_t r3 = SlotWaitThumbnail(slot, site(0x820A5C64), kernel);
      return r3 == 0 ? 7 : finish(r3, false);
    }
    default:
      return 7;
  }
}

uint32_t SlotLoadStep(uint32_t devices, uint32_t slot, const CallSite& at,
                      const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5E68 SaveSlot::LoadStep");
  const uint32_t sp = WritePrologue(at, 112, 29);
  if (!Claim(devices, slot)) {
    return 8;
  }
  const auto site = [&](uint32_t lr) { return From(at, sp, lr, 8, devices, slot); };
  const auto failed = [&] {
    SaveSlot(slot).set_failure_reason(4);
    schema::SaveDevices(devices).set_busy_slot(0);
    return 2u;
  };
  const auto to_close = [&] {
    SaveSlot(slot).set_step(5);
    SaveSlot(slot).set_step_error(1);
    return 8u;
  };
  switch (SaveSlot(slot).step()) {
    case 1: {
      const int32_t r3 = saves::slot::Create(devices, slot, 0);
      SaveSlot(slot).set_step_error(0);
      if (r3 == 1) {
        SaveSlot(slot).set_step(2);
        return 8;
      }
      if (r3 == -2) {
        SaveSlot(slot).set_step(0);
        schema::SaveDevices(devices).set_busy_slot(0);
        return 3;
      }
      if (r3 == -5 || r3 == -99) {
        SaveSlot(slot).set_step(0);
        return failed();
      }
      return 8;
    }
    case 2: {
      const int32_t r3 = saves::slot::ReadFile(slot);
      if (r3 == 1) {
        SaveSlot(slot).set_step(4);
        return 8;
      }
      return r3 == -3 || r3 == -7 || r3 == -99 ? to_close() : 8;
    }
    case 4: {
      const int32_t r3 = SlotWaitRead(slot, site(0x820A5F40), kernel);
      if (r3 == 1) {
        SaveSlot(slot).set_step(5);
        return 8;
      }
      return r3 == -11 ? to_close() : 8;
    }
    case 5:
      saves::slot::ContentClose(SaveSlot(slot).content_op_address());
      SaveSlot(slot).set_step(6);
      return 8;
    case 6: {
      const int32_t r3 = SlotWaitClose(slot, site(0x820A5ED4), kernel);
      if (r3 == -8 && int32_t(SaveSlot(slot).step_error()) == 0) {
        SaveSlot(slot).set_step_error(1);
      } else if (r3 == 0) {
        return 8;
      }
      SaveSlot(slot).set_step(0);
      if (int32_t(SaveSlot(slot).step_error()) == 0) {
        schema::SaveDevices(devices).set_busy_slot(0);
        return 3;
      }
      return failed();
    }
    default:
      return 8;
  }
}

uint32_t SlotDeleteStep(uint32_t devices, uint32_t slot, const CallSite& at,
                        const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A6010 SaveSlot::DeleteStep");
  const uint32_t sp = WritePrologue(at, 112, 29);
  if (!Claim(devices, slot)) {
    return 9;
  }
  const auto failed = [&] {
    SaveSlot(slot).set_failure_reason(5);
    schema::SaveDevices(devices).set_busy_slot(0);
    return 2u;
  };
  switch (SaveSlot(slot).step()) {
    case 9: {
      const int32_t r3 = saves::slot::Delete(slot);
      SaveSlot(slot).set_step_error(0);
      if (r3 == 1) {
        SaveSlot(slot).set_step(10);
        return 9;
      }
      if (r3 == -2) {
        SaveSlot(slot).set_step(0);
        schema::SaveDevices(devices).set_busy_slot(0);
        return 3;
      }
      if (r3 == -12) {
        SaveSlot(slot).set_step(0);
        return failed();
      }
      return 9;
    }
    case 10: {
      const int32_t r3 =
          SlotWaitDelete(slot, From(at, sp, 0x820A6064, 9, devices, slot), kernel);
      if (r3 == -12 && int32_t(SaveSlot(slot).step_error()) == 0) {
        SaveSlot(slot).set_step_error(1);
      } else if (r3 == 0) {
        return 9;
      }
      SaveSlot(slot).set_step(0);
      if (int32_t(SaveSlot(slot).step_error()) == 0) {
        schema::SaveDevices(devices).set_busy_slot(0);
        return 3;
      }
      return failed();
    }
    default:
      return 9;
  }
}

uint64_t DevicesUpdate(uint64_t self, uint64_t r4, const CallSite& at,
                       const KernelCalls& kernel, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_820A4610 SaveDevices::Update");
  const uint32_t devices = uint32_t(self);
  const uint32_t sp = WritePrologue(at, 144, 27);
  if (schema::SaveDevices(devices).listener() == 0) {
    return self;
  }
  // The registers the original holds at each call: r27 to r30 as it sets
  // them, r31 the save devices.
  uint64_t r27 = at.r27, r28 = at.r28, r29 = at.r29, r30 = at.r30;
  const auto site = [&](uint32_t lr) {
    CallSite c;
    c.sp = sp;
    c.lr = lr;
    c.r27 = r27;
    c.r28 = r28;
    c.r29 = r29;
    c.r30 = r30;
    c.r31 = devices;
    return c;
  };
  const auto method = [&](uint32_t object, uint32_t offset) {
    return Load(guest::VtableWord(object) + offset);
  };
  const uint32_t source = schema::SaveDevices(devices).device_list();
  const uint32_t list_function = method(source, 4);
  const uint64_t list64 = list_function == 0x820908D8
                              ? FrameworkDeviceList(source)
                              : call(list_function, source, r4, site(0x820A4640));
  const uint32_t list = uint32_t(list64);
  r29 = 0;
  r30 = list64;
  r28 = 0;
  if (int32_t(schema::SaveDevices(devices).slot_count()) > 0) {
    r27 = 0;
    do {
      const uint32_t index = uint32_t(r28);
      const uint32_t function = method(list, 8);
      const uint32_t user = function == kUsersSignedIn
                                ? uint32_t(UsersSignedIn(list, index))
                                : uint32_t(call(function, list, index, site(0x820A4674)));
      const uint32_t slot = schema::SaveDevices(devices).slots<schema::View>().address + uint32_t(r27);
      r28++;
      r27 += 1000;
      SaveSlot(slot).set_user(user);
    } while (int32_t(r28) < int32_t(schema::SaveDevices(devices).slot_count()));
  }
  // XNotifyGetNext (r1 + 80 and + 84 its outputs) left out: no notification
  // is sent; its answer, 0, is what r3 holds next.
  uint64_t r3 = 0;
  r28 = r29;
  if (int32_t(schema::SaveDevices(devices).slot_count()) <= 0) {
    return r3;
  }
  r30 = r29;
  do {
    const uint32_t index = uint32_t(r28);
    const uint32_t offset = uint32_t(r30);
    const int32_t user =
        int32_t(SaveSlot(schema::SaveDevices(devices).slots<schema::View>().address + offset).user());
    if (user < 0 || user > 3) {
      const uint32_t function = method(devices, 28);
      r3 = function == kSlotState ? SlotState(devices, index)
                                  : call(function, devices, index, site(0x820A4758));
      if (uint32_t(r3) < 5) {
        // The slot's base re-read from the devices' +44 before every store.
        const auto cleared = [&] {
          return SaveSlot(schema::SaveDevices(devices).slots<schema::View>().address + offset);
        };
        const uint32_t zero = uint32_t(r29);
        cleared().set_device(zero);
        cleared().set_state(zero);
        cleared().set_enumerated_saves(zero);
        cleared().set_request_device_selector(zero);
        cleared().set_request_enumeration(zero);
        cleared().set_request_save(zero);
        cleared().set_request_load(zero);
      }
    }
    const uint32_t slot = schema::SaveDevices(devices).slots<schema::View>().address + offset;
    // The handlers' table from +76 by the slot's state (the save devices in
    // host memory: read through their layout).
    const schema::SaveDevices owner(devices);
    const uint32_t function = uint32_t(owner.load_at(
        owner.wait_for_user_address() - owner.address + SaveSlot(slot).state() * 4, 4));
    const CallSite handler_site = site(0x820A47D8);
    switch (function) {
      case kSlotWaitForUser: r3 = SlotWaitForUser(slot); break;
      case kSlotWaitForDevice: r3 = SlotWaitForDevice(slot, handler_site); break;
      case kSlotFailed: r3 = SlotFailed(slot); break;
      case kSlotReady: r3 = SlotReady(slot, handler_site); break;
      case kSlotWaitDeviceSelector:
        r3 = uint64_t(int64_t(saves::slot::WaitDeviceSelector(slot)));
        break;
      case kSlotEnumerateStep: r3 = SlotEnumerateStep(devices, slot, handler_site); break;
      case kSlotCheckStep: r3 = SlotCheckStep(devices, slot, handler_site, kernel); break;
      case kSlotSaveStep: r3 = SlotSaveStep(devices, slot, handler_site, kernel); break;
      case kSlotLoadStep: r3 = SlotLoadStep(devices, slot, handler_site, kernel); break;
      case kSlotDeleteStep: r3 = SlotDeleteStep(devices, slot, handler_site, kernel); break;
      default: r3 = call(function, devices, slot, handler_site); break;
    }
    const uint32_t stored = schema::SaveDevices(devices).slots<schema::View>().address + offset;
    r28++;
    r30 += 1000;
    SaveSlot(stored).set_state(uint32_t(r3));
  } while (int32_t(r28) < int32_t(schema::SaveDevices(devices).slot_count()));
  return r3;
}

}  // namespace save_interface
