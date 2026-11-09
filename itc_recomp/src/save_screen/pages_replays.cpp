// The save screen's replay pages (kinds 2 to 5) and kind 7's controller
// pick (docs/functions/save-screen.md): see save_screen.h.

#include "base/log.h"
#include "save_screen/internal.h"

namespace save_screen {

namespace {

// Straight to page 15 (the device's selector) and RequestDevice(0).
void ToDevicePage(Fn& f, uint32_t lr) {
  Jump(f.base, 15);
  f.Call(kRequestDevice, lr, 0);
}

}  // namespace

uint64_t Page12(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A3A08 SaveScreen::Page12");
  Fn f(base, sp, 128, calls, mode);
  f.Call(kMenuListUpdate, 0x821A3A20, List(base));
  if (f.CallInt(kMenuListAccepted, 0x821A3A28, List(base)) != 0) {
    f.Call(kMenuSoundPlay, 0x821A3A48, 0);
    const bool first = f.CallInt(kMenuListSelection, 0x821A3A50, List(base)) == 0;
    const uint32_t data = Data(base);
    switch (int32_t(Load(base, data + 12))) {
      case 12:
        Store(base, data + 4, first ? 1 : 0);
        Go(base, 16, 2, kNone);
        break;
      case 6:
        if (first) {
          Go(base, 16, 2, kNone);
        } else {
          ToDevicePage(f, 0x821A3AD0);
        }
        break;
      case 5:
        if (first) {
          Go(base, 16, 2, kNone);
        } else {
          Go(base, 14, 2, 4);
          Message(base, 0, 2);
        }
        break;
      case 3:
        if (first) {
          ToDevicePage(f, 0x821A3AD0);
        } else {
          Go(base, 12, 2, 4);
          Message(base, 5, 0);
        }
        break;
      case 1:
        if (first) {
          Go(base, 14, 2, 4);
          Message(base, 7, 2);
          f.Call(kReplaysLoad, 0x821A3BD8, Load(base, Data(base) + 40));
        } else {
          Store(base, data + 4, 3);
          Go(base, 16, 2, kNone);
        }
        break;
      default:
        break;
    }
  }
  f.Call(kMenuListCancelled, 0x821A3C18, List(base));
  if (Load(base, List(base) + 24) != 0) {
    f.Call(kMenuSoundPlay, 0x821A3C30, 4);
    Go(base, 12, 1, kNone);
  }
  return f.r3;
}

uint64_t Page13(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A3C60 SaveScreen::Page13");
  Fn f(base, sp, 96, calls, mode);
  Store(base, Data(base) + 24, Load(base, Data(base) + 24) + 1);
  if (int32_t(Load(base, Data(base) + 24)) >= 45 && int32_t(Load(base, Data(base) + 12)) == 2) {
    ToDevicePage(f, 0x821A3DD4);
    return f.r3;
  }
  if (f.CallInt(kMenuListAccepted, 0x821A3CAC, List(base)) == 0) {
    return f.r3;
  }
  f.Call(kMenuSoundPlay, 0x821A3CBC, 0);
  switch (int32_t(Load(base, Data(base) + 12))) {
    case 2:
      ToDevicePage(f, 0x821A3DD4);
      break;
    case 4:
      Go(base, 12, 2, 4);
      Message(base, 6, 0);
      break;
    case 8:
    case 10:
      Go(base, 16, 2, kNone);
      break;
    case 9:
    case 11:
      Go(base, 12, 2, 4);
      Message(base, 5, 0);
      break;
    default:
      break;
  }
  return f.r3;
}

uint64_t Page14(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A67E8 SaveScreen::Page14");
  Fn f(base, sp, 96, calls, mode);
  const int32_t state = f.CallInt(kSlotState, 0x821A6800, 0);
  const int32_t message = int32_t(Load(base, Data(base) + 12));
  if (message != 0 && message != 7) {
    return f.r3;
  }
  Store(base, Data(base) + 24, Load(base, Data(base) + 24) + 1);
  if (int32_t(Load(base, Data(base) + 24)) < 90) {
    return f.r3;
  }
  if (state == 1) {
    Go(base, 12, 2, 4);
    Message(base, 3, 0);
    return f.r3;
  }
  if (message == 0) {
    if (state != 3 || uint32_t(f.Call(kSlotPending, 0x821A69A8, 0)) != 0) {
      return f.r3;
    }
    const int32_t built = f.CallInt(kReplaysBuildList, 0x821A69B4, f.r3);
    if (built == 3) {
      Go(base, 13, 2, 4);
      Message(base, 9, 1);
      f.Call(kRequest980, 0x821A6AD8, 0);
    } else if (built == 7) {
      if (int32_t(Load(base, Load(base, kSaveBuffers) + 20)) == 0) {
        Go(base, 13, 2, 4);
        Message(base, 4, 1);
      } else {
        const uint32_t data = Data(base);
        Store(base, data + 4, int32_t(Load(base, data + 8)) != 0 ? 1 : 2);
        Go(base, 16, 2, kNone);
      }
    }
    return f.r3;
  }
  // The message 7: a replay's load.
  if (state == 2) {
    if (f.CallInt(kWord992, 0x821A6858, 0) == 4) {
      const int32_t word = f.CallInt(kReplayWord84, 0x821A686C, Load(base, Data(base) + 40));
      Go(base, 13, 2, 4);
      Message(base, word == 0 ? 9 : 11, 1);
    }
    f.Call(kRequest980, 0x821A6AD8, 0);
  } else if (state == 3) {
    if (uint32_t(f.Call(kSlotPending, 0x821A68A4, 0)) != 0) {
      return f.r3;
    }
    f.Call(kReplayCopyFrom, 0x821A68C4, Load(base, kKeptReplay),
           Load(base, Load(base, kSaveBuffers) + 4));
    Go(base, 13, 2, 4);
    Message(base, 8, 1);
    Store(base, Data(base) + 4, 1);
  }
  return f.r3;
}

uint64_t Page15(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A3DE8 SaveScreen::Page15");
  Fn f(base, sp, 96, calls, mode);
  if (f.CallInt(kSlotState, 0x821A3DFC, 0) == 4) {
    return f.r3;
  }
  if (uint32_t(f.Call(kSlotDevice, 0x821A3E0C, 0)) != 0) {
    Store(base, Data(base) + 24, 0);
    Go(base, 14, 2, 4);
    Message(base, 0, 2);
  } else {
    Go(base, 12, 2, 4);
    Message(base, 6, 0);
  }
  return f.r3;
}

uint64_t Page24(uint8_t* base, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A4AC0 SaveScreen::Page24");
  Fn f(base, sp, 128, calls, mode);
  uint32_t entries[4];
  for (uint32_t i = 0; i < 4; i++) {
    entries[i] = kMapperEntries + 8 * Load(base, kControllers + 4 * i);
  }
  // The mapper's slot 10 (ActionMapper::Pressed) for `action`, unless the
  // entry's +4 is 1.
  auto pressed = [&](uint32_t entry, uint32_t action, uint32_t lr) {
    if (int32_t(Load(base, entry + 4)) == 1) {
      return false;
    }
    const uint32_t mapper = Load(base, entry);
    const uint32_t slot = Load(base, Load(base, mapper) + 40);
    return (f.Call(slot, lr, mapper, action) & 0xFF) != 0;
  };
  int32_t player = -1;
  if (pressed(entries[0], 159, 0x821A4B34) || pressed(entries[0], 158, 0x821A4B60)) {
    player = 0;
  } else if (pressed(entries[1], 152, 0x821A4B8C)) {
    player = 1;
  } else if (pressed(entries[2], 152, 0x821A4BC0)) {
    player = 2;
  } else if (pressed(entries[3], 152, 0x821A4BFC)) {
    player = 3;
  }
  if (player >= 0) {
    Store(base, Data(base) + 52, uint32_t(player));
    Go(base, 25, 2, kNone);
  }
  return f.r3;
}

}  // namespace save_screen
