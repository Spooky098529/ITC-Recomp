// The save screen's kind 0 pages and the end pages
// (docs/functions/save-screen.md): see save_screen.h.

#include "base/log.h"
#include "save_screen/internal.h"

namespace save_screen {

namespace {

// Straight to page 3 (the device's selector) and SaveSlots::RequestDevice(0).
void ToDevicePage(Fn& f, uint32_t page, uint32_t lr) {
  Jump(f.base, page);
  f.Call(kRequestDevice, lr, 0);
}

}  // namespace

uint64_t Page0(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A28E0 SaveScreen::Page0");
  Fn f(base, sp, 128, calls, mode);
  f.Call(kMenuListUpdate, 0x821A28F8, List(base));
  if (f.CallInt(kMenuListAccepted, 0x821A2900, List(base)) != 0) {
    f.Call(kMenuSoundPlay, 0x821A291C, 0);
    const uint64_t selection = f.Call(kMenuListSelection, 0x821A2924, List(base));
    const bool first = int32_t(uint32_t(selection)) == 0;
    switch (int32_t(Load(base, Data(base) + 12))) {
      case 1:
      case 5:
        if (first) {
          Go(base, 2, 2, 0);
          Message(base, 8, 2);
          f.Call(kGameDataSave, 0x821A2A64, selection);
        } else {
          Go(base, 0, 2, 0);
          Message(base, 6, 0);
        }
        break;
      case 4:
        if (first) {
          ToDevicePage(f, 3, 0x821A29B4);
        } else {
          Go(base, 0, 2, 0);
          Message(base, 6, 0);
        }
        break;
      case 6:
        if (first) {
          Go(base, 4, 2, kNone);
        } else {
          Go(base, 2, 2, 0);
          Message(base, 0, 2);
        }
        break;
      case 11:
        if (first) {
          Go(base, 4, 2, kNone);
        } else {
          ToDevicePage(f, 3, 0x821A29B4);
        }
        break;
      default:
        break;
    }
  }
  f.Call(kMenuListCancelled, 0x821A2AAC, List(base));
  if (Load(base, List(base) + 24) != 0) {
    f.Call(kMenuSoundPlay, 0x821A2AC4, 4);
    Go(base, 0, 1, kNone);
  }
  return f.r3;
}

uint64_t Page1(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A2AF8 SaveScreen::Page1");
  Fn f(base, sp, 96, calls, mode);
  Store(base, Data(base) + 24, Load(base, Data(base) + 24) + 1);
  if (int32_t(Load(base, Data(base) + 24)) >= 45 && int32_t(Load(base, Data(base) + 12)) == 3) {
    ToDevicePage(f, 3, 0x821A2C48);
    return f.r3;
  }
  if (f.CallInt(kMenuListAccepted, 0x821A2B44, List(base)) == 0) {
    return f.r3;
  }
  f.Call(kMenuSoundPlay, 0x821A2B54, 0);
  switch (int32_t(Load(base, Data(base) + 12))) {
    case 2:
      Go(base, 4, 2, kNone);
      break;
    case 3:
      ToDevicePage(f, 3, 0x821A2C48);
      break;
    case 7:
    case 10:
      Go(base, 0, 2, 0);
      Message(base, 6, 0);
      break;
    case 9:
      Go(base, 4, 2, kNone);
      Store(base, Data(base) + 4, 1);
      break;
    default:
      break;
  }
  return f.r3;
}

uint64_t Page2(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A2C98 SaveScreen::Page2");
  Fn f(base, sp, 96, calls, mode);
  const int32_t state = f.CallInt(kSlotState, 0x821A2CB0, 0);
  if (state == 1) {
    Go(base, 0, 2, 0);
    Message(base, 4, 0);
    return f.r3;
  }
  Store(base, Data(base) + 24, Load(base, Data(base) + 24) + 1);
  if (int32_t(Load(base, Data(base) + 24)) < 90) {
    return f.r3;
  }
  const int32_t message = int32_t(Load(base, Data(base) + 12));
  if (message == 0) {
    switch (uint32_t(f.Call(kGameDataCheck, 0x821A2E34, 0))) {
      case 0:
        Go(base, 0, 2, 0);
        Message(base, 1, 0);
        break;
      case 1:
        Go(base, 0, 2, 0);
        Message(base, 5, 0);
        break;
      case 2:
        Go(base, 1, 2, 0);
        Message(base, 7, 1);
        break;
      default:
        break;
    }
  } else if (message == 8 && state != 7) {
    if (state == 2) {
      const int32_t word = f.CallInt(kWord992, 0x821A2D54, 0);
      if (word == 2) {
        Go(base, 1, 2, 0);
        Message(base, 10, 1);
      } else if (word == 3) {
        Go(base, 1, 2, 0);
        Message(base, 7, 1);
      }
      f.Call(kRequest980, 0x821A2DD8, 0);
    } else if (state == 3) {
      Go(base, 1, 2, 0);
      Message(base, 9, 1);
    }
  }
  return f.r3;
}

uint64_t Page3(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A2EE8 SaveScreen::Page3");
  Fn f(base, sp, 96, calls, mode);
  if (f.CallInt(kSlotState, 0x821A2EFC, 0) == 4) {
    return f.r3;
  }
  if (uint32_t(f.Call(kSlotDevice, 0x821A2F0C, 0)) != 0) {
    Go(base, 2, 2, 0);
    Message(base, 0, 2);
  } else {
    Go(base, 0, 2, 0);
    Message(base, 11, 0);
  }
  return f.r3;
}

namespace {

void End(uint8_t* base, uint32_t mode, uint32_t id) {
  if (id != 0) {
    Post(base, mode, id);
  }
  Store(base, Data(base) + 32, 9);
}

}  // namespace

uint64_t Page4(uint8_t* base, uint64_t mode) {
  ITC_NATIVE_RUNNING("sub_821A4D30 SaveScreen::Page4");
  const bool yes = int32_t(Load(base, Data(base) + 4)) == 1;
  const int32_t type = int32_t(Load(base, kModeType));
  uint32_t id = 0;
  if (type == 2) {
    id = yes ? 44 : 45;
  } else if (type == 6) {
    id = yes ? 30 : 31;
  }
  End(base, uint32_t(mode), id);
  return mode;
}

uint64_t Page9(uint8_t* base, uint64_t mode) {
  ITC_NATIVE_RUNNING("sub_821A4DE0 SaveScreen::Page9");
  const int32_t type = int32_t(Load(base, kModeType));
  End(base, uint32_t(mode), type == 2 ? 47 : type == 4 ? 14 : 0);
  return mode;
}

uint64_t Page16(uint8_t* base, uint64_t mode) {
  ITC_NATIVE_RUNNING("sub_821A4E60 SaveScreen::Page16");
  const int32_t answer = int32_t(Load(base, Data(base) + 4));
  End(base, uint32_t(mode), answer == 1 ? 11 : answer == 2 ? 13 : answer == 3 ? 14 : 12);
  return mode;
}

uint64_t Page25(uint8_t* base, uint64_t mode) {
  ITC_NATIVE_RUNNING("sub_821A4FD8 SaveScreen::Page25");
  End(base, uint32_t(mode), 16);
  return mode;
}

}  // namespace save_screen
