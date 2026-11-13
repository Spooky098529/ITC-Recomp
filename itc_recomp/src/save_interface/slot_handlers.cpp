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
int32_t PollOverlapped(uint8_t* base, uint32_t overlapped, int32_t error, uint32_t lr,
                       const CallSite& at, const KernelCalls& kernel) {
  const uint32_t sp = WritePrologue(base, at, 96, 32);
  CallSite site = at;
  site.sp = sp;
  site.lr = lr;
  const uint32_t r3 = XGetOverlappedResult(base, overlapped, 0, 0, site, kernel);
  if (r3 == 0) {
    return 1;
  }
  return r3 == 996 ? 0 : error;
}

// WaitWrite and WaitRead: GetOverlappedResult, then GetLastError.
// `full` the answer for a last error of 112 (the write's), else `error`.
int32_t PollFile(uint8_t* base, uint32_t slot, int32_t error, int32_t full, uint32_t lr,
                 const CallSite& at, const KernelCalls& kernel) {
  const uint32_t sp = WritePrologue(base, at, 112, 30);
  const uint32_t overlapped = slot + 472;
  const uint32_t done =
      GetOverlappedResult(base, Load(base, slot + 496), overlapped, slot + 500, 0,
                          From(at, sp, lr, at.r29, at.r30, overlapped), kernel);
  const uint32_t last = last_error::Get(base, kernel.pcr);
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
bool Claim(uint8_t* base, uint32_t devices, uint32_t slot) {
  if (Load(base, devices + 72) == 0) {
    Store(base, devices + 72, slot);
  }
  return Load(base, devices + 72) == slot;
}

}  // namespace

int32_t SlotWaitCloseItem(uint8_t* base, uint32_t slot, const CallSite& at,
                          const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A4C08 SaveSlot::WaitCloseItem");
  return PollOverlapped(base, slot + 100, -8, 0x820A4C28, at, kernel);
}

int32_t SlotWaitDelete(uint8_t* base, uint32_t slot, const CallSite& at,
                       const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5290 SaveSlot::WaitDelete");
  return PollOverlapped(base, slot + 136, -12, 0x820A52B0, at, kernel);
}

int32_t SlotWaitThumbnail(uint8_t* base, uint32_t slot, const CallSite& at,
                          const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5190 SaveSlot::WaitThumbnail");
  return PollOverlapped(base, slot + 136, -9, 0x820A51B0, at, kernel);
}

int32_t SlotWaitClose(uint8_t* base, uint32_t slot, const CallSite& at,
                      const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5070 SaveSlot::WaitClose");
  const uint32_t sp = WritePrologue(base, at, 96, 31);
  CallSite site = at;
  site.sp = sp;
  site.r31 = slot;
  site.lr = 0x820A5094;
  const uint32_t r3 = XGetOverlappedResult(base, slot + 136, 0, 0, site, kernel);
  if (r3 == 996) {
    return 0;
  }
  const bool done = r3 == 0;
  const uint32_t file = Load(base, slot + 496);
  if (int32_t(file) != -1) {
    site.lr = done ? 0x820A50AC : 0x820A50DC;
    kernel.close(file, site);
  }
  const uint32_t event = Load(base, slot + 492);
  if (event != 0) {
    site.lr = done ? 0x820A50BC : 0x820A50EC;
    kernel.close(event, site);
  }
  return done ? 1 : -8;
}

int32_t SlotWaitWrite(uint8_t* base, uint32_t slot, const CallSite& at,
                      const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A4E68 SaveSlot::WaitWrite");
  return PollFile(base, slot, -9, -10, 0x820A4E94, at, kernel);
}

int32_t SlotWaitRead(uint8_t* base, uint32_t slot, const CallSite& at, const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A4FF8 SaveSlot::WaitRead");
  return PollFile(base, slot, -11, -11, 0x820A5024, at, kernel);
}

uint32_t SlotCheckStep(uint8_t* base, uint32_t devices, uint32_t slot, const CallSite& at,
                       const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5A88 SaveSlot::CheckStep");
  const uint32_t sp = WritePrologue(base, at, 112, 29);
  if (!Claim(base, devices, slot)) {
    return 6;
  }
  const auto next_item = [&] { Store(base, slot + 132, Load(base, slot + 132) + 1); };
  switch (Load(base, slot + 128)) {
    case 1: {
      for (uint32_t i = 0; i < Load(base, slot + 984); i++) {
        Store(base, Load(base, slot + 84) + 4 * i, 1);
      }
      Store(base, slot + 132, 0);
      Store(base, slot + 128, 2);
      return 6;
    }
    case 2: {
      const uint32_t item = Load(base, slot + 132);
      if (item < Load(base, slot + 984)) {
        const int32_t r3 = saves::slot::OpenItem(base, slot, item);
        if (r3 == 1) {
          Store(base, slot + 128, 3);
          return 6;
        }
        if (r3 != -2) {
          next_item();
          return 6;
        }
      }
      Store(base, slot + 128, 0);
      Store(base, devices + 72, 0);
      return 3;
    }
    case 3: {
      const int32_t r3 = saves::slot::WaitOpenItem(base, slot);
      if (r3 == 1) {
        Store(base, Load(base, slot + 84) + 4 * Load(base, slot + 132), 0);
        Store(base, slot + 128, 4);
      } else if (r3 == -7) {
        Store(base, slot + 128, 2);
        next_item();
      }
      return 6;
    }
    case 4: {
      if (SlotWaitCloseItem(base, slot, From(at, sp, 0x820A5AEC, 6, devices, slot), kernel) !=
          0) {
        Store(base, slot + 128, 2);
        next_item();
      }
      return 6;
    }
    default:
      return 6;
  }
}

uint32_t SlotSaveStep(uint8_t* base, uint32_t devices, uint32_t slot, const CallSite& at,
                      const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5BE8 SaveSlot::SaveStep");
  const uint32_t sp = WritePrologue(base, at, 112, 29);
  if (!Claim(base, devices, slot)) {
    return 7;
  }
  const auto site = [&](uint32_t lr) { return From(at, sp, lr, devices, 7, slot); };
  // The end of a step that failed or finished: the state given back.
  const auto release = [&] { Store(base, devices + 72, 0); };
  // A file step's error: +952 (1 failed, 2 no room), then the close.
  const auto to_close = [&](uint32_t error) {
    Store(base, slot + 952, error);
    Store(base, slot + 948, 5);
    return 7u;
  };
  // After the close's or the thumbnail's wait (`r3` its answer, not 0):
  // the enumeration asked for again and the machine left (`thumbnail`:
  // with no error and a thumbnail (+940), on to it instead).
  const auto finish = [&](int32_t r3, bool thumbnail) -> uint32_t {
    if (r3 != 1 && int32_t(Load(base, slot + 952)) == 0) {
      Store(base, slot + 952, 1);
    }
    const int32_t error = int32_t(Load(base, slot + 952));
    if (thumbnail && error == 0 && Load(base, slot + 940) != 0) {
      Store(base, slot + 948, 7);
      return 7;
    }
    Store(base, slot + 964, 1);
    Store(base, slot + 948, 0);
    if (error == 0) {
      release();
      return 3;
    }
    Store(base, slot + 992, error == 1 ? 2 : 3);
    release();
    return 2;
  };
  switch (Load(base, slot + 948)) {
    case 1: {
      const int32_t r3 = saves::slot::Create(base, devices, slot, 1);
      Store(base, slot + 952, 0);
      if (r3 == 1) {
        Store(base, slot + 948, 2);
        return 7;
      }
      if (r3 == -2) {
        Store(base, slot + 948, 0);
        release();
        return 3;
      }
      if (r3 == -5 || r3 == -99) {
        Store(base, slot + 948, 0);
        Store(base, slot + 992, 2);
        release();
        return 2;
      }
      if (r3 == -10) {
        Store(base, slot + 992, 3);
        Store(base, slot + 948, 0);
        release();
        return 2;
      }
      return 7;
    }
    case 2: {
      const int32_t r3 = saves::slot::WriteFile(base, slot);
      if (r3 == 1) {
        Store(base, slot + 948, 3);
        return 7;
      }
      if (r3 == -3 || r3 == -7 || r3 == -99) {
        return to_close(1);
      }
      return 7;
    }
    case 3: {
      const int32_t r3 = SlotWaitWrite(base, slot, site(0x820A5D90), kernel);
      if (r3 == 1) {
        Store(base, slot + 948, 5);
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
      saves::slot::ContentClose(base, slot + 136);
      Store(base, slot + 948, 6);
      return 7;
    case 6: {
      const int32_t r3 = SlotWaitClose(base, slot, site(0x820A5D08), kernel);
      return r3 == 0 ? 7 : finish(r3, true);
    }
    case 7: {
      if (saves::slot::SetThumbnail(base, slot) == 1) {
        Store(base, slot + 948, 8);
        return 7;
      }
      Store(base, slot + 948, 0);
      Store(base, slot + 992, 2);
      release();
      return 2;
    }
    case 8: {
      const int32_t r3 = SlotWaitThumbnail(base, slot, site(0x820A5C64), kernel);
      return r3 == 0 ? 7 : finish(r3, false);
    }
    default:
      return 7;
  }
}

uint32_t SlotLoadStep(uint8_t* base, uint32_t devices, uint32_t slot, const CallSite& at,
                      const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A5E68 SaveSlot::LoadStep");
  const uint32_t sp = WritePrologue(base, at, 112, 29);
  if (!Claim(base, devices, slot)) {
    return 8;
  }
  const auto site = [&](uint32_t lr) { return From(at, sp, lr, 8, devices, slot); };
  const auto failed = [&] {
    Store(base, slot + 992, 4);
    Store(base, devices + 72, 0);
    return 2u;
  };
  const auto to_close = [&] {
    Store(base, slot + 948, 5);
    Store(base, slot + 952, 1);
    return 8u;
  };
  switch (Load(base, slot + 948)) {
    case 1: {
      const int32_t r3 = saves::slot::Create(base, devices, slot, 0);
      Store(base, slot + 952, 0);
      if (r3 == 1) {
        Store(base, slot + 948, 2);
        return 8;
      }
      if (r3 == -2) {
        Store(base, slot + 948, 0);
        Store(base, devices + 72, 0);
        return 3;
      }
      if (r3 == -5 || r3 == -99) {
        Store(base, slot + 948, 0);
        return failed();
      }
      return 8;
    }
    case 2: {
      const int32_t r3 = saves::slot::ReadFile(base, slot);
      if (r3 == 1) {
        Store(base, slot + 948, 4);
        return 8;
      }
      return r3 == -3 || r3 == -7 || r3 == -99 ? to_close() : 8;
    }
    case 4: {
      const int32_t r3 = SlotWaitRead(base, slot, site(0x820A5F40), kernel);
      if (r3 == 1) {
        Store(base, slot + 948, 5);
        return 8;
      }
      return r3 == -11 ? to_close() : 8;
    }
    case 5:
      saves::slot::ContentClose(base, slot + 136);
      Store(base, slot + 948, 6);
      return 8;
    case 6: {
      const int32_t r3 = SlotWaitClose(base, slot, site(0x820A5ED4), kernel);
      if (r3 == -8 && int32_t(Load(base, slot + 952)) == 0) {
        Store(base, slot + 952, 1);
      } else if (r3 == 0) {
        return 8;
      }
      Store(base, slot + 948, 0);
      if (int32_t(Load(base, slot + 952)) == 0) {
        Store(base, devices + 72, 0);
        return 3;
      }
      return failed();
    }
    default:
      return 8;
  }
}

uint32_t SlotDeleteStep(uint8_t* base, uint32_t devices, uint32_t slot, const CallSite& at,
                        const KernelCalls& kernel) {
  ITC_NATIVE_RUNNING("sub_820A6010 SaveSlot::DeleteStep");
  const uint32_t sp = WritePrologue(base, at, 112, 29);
  if (!Claim(base, devices, slot)) {
    return 9;
  }
  const auto failed = [&] {
    Store(base, slot + 992, 5);
    Store(base, devices + 72, 0);
    return 2u;
  };
  switch (Load(base, slot + 948)) {
    case 9: {
      const int32_t r3 = saves::slot::Delete(base, slot);
      Store(base, slot + 952, 0);
      if (r3 == 1) {
        Store(base, slot + 948, 10);
        return 9;
      }
      if (r3 == -2) {
        Store(base, slot + 948, 0);
        Store(base, devices + 72, 0);
        return 3;
      }
      if (r3 == -12) {
        Store(base, slot + 948, 0);
        return failed();
      }
      return 9;
    }
    case 10: {
      const int32_t r3 =
          SlotWaitDelete(base, slot, From(at, sp, 0x820A6064, 9, devices, slot), kernel);
      if (r3 == -12 && int32_t(Load(base, slot + 952)) == 0) {
        Store(base, slot + 952, 1);
      } else if (r3 == 0) {
        return 9;
      }
      Store(base, slot + 948, 0);
      if (int32_t(Load(base, slot + 952)) == 0) {
        Store(base, devices + 72, 0);
        return 3;
      }
      return failed();
    }
    default:
      return 9;
  }
}

}  // namespace save_interface
