// The save screen's replay pages (kinds 2 to 5) and kind 7's controller
// pick (docs/functions/save-screen.md): see save_screen.h.

#include "base/log.h"
#include "save_screen/internal.h"

namespace save_screen {

namespace {

// Straight to page 15 (the device's selector) and RequestDevice(0).
void ToDevicePage(Fn& f, uint32_t lr) {
  Jump(15);
  f.Call(kRequestDevice, lr, 0);
}

}  // namespace

uint64_t Page12(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A3A08 SaveScreen::Page12");
  Fn f(sp, 128, calls, mode);
  f.Call(kMenuListUpdate, 0x821A3A20, List());
  if (f.CallInt(kMenuListAccepted, 0x821A3A28, List()) != 0) {
    f.Call(kMenuSoundPlay, 0x821A3A48, 0);
    const bool first = f.CallInt(kMenuListSelection, 0x821A3A50, List()) == 0;
    const auto data = Data();
    switch (int32_t(data.message())) {
      case 12:
        data.set_answer(first ? 1 : 0);
        Go(16, 2, kNone);
        break;
      case 6:
        if (first) {
          Go(16, 2, kNone);
        } else {
          ToDevicePage(f, 0x821A3AD0);
        }
        break;
      case 5:
        if (first) {
          Go(16, 2, kNone);
        } else {
          Go(14, 2, 4);
          Message(0, 2);
        }
        break;
      case 3:
        if (first) {
          ToDevicePage(f, 0x821A3AD0);
        } else {
          Go(12, 2, 4);
          Message(5, 0);
        }
        break;
      case 1:
        if (first) {
          Go(14, 2, 4);
          Message(7, 2);
          f.Call(kReplaysLoad, 0x821A3BD8, Data().replay_index());
        } else {
          data.set_answer(3);
          Go(16, 2, kNone);
        }
        break;
      default:
        break;
    }
  }
  f.Call(kMenuListCancelled, 0x821A3C18, List());
  if (G::list().move() != 0) {
    f.Call(kMenuSoundPlay, 0x821A3C30, 4);
    Go(12, 1, kNone);
  }
  return f.r3;
}

uint64_t Page13(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A3C60 SaveScreen::Page13");
  Fn f(sp, 96, calls, mode);
  Data().set_wait(Data().wait() + 1);
  if (int32_t(Data().wait()) >= 45 && int32_t(Data().message()) == 2) {
    ToDevicePage(f, 0x821A3DD4);
    return f.r3;
  }
  if (f.CallInt(kMenuListAccepted, 0x821A3CAC, List()) == 0) {
    return f.r3;
  }
  f.Call(kMenuSoundPlay, 0x821A3CBC, 0);
  switch (int32_t(Data().message())) {
    case 2:
      ToDevicePage(f, 0x821A3DD4);
      break;
    case 4:
      Go(12, 2, 4);
      Message(6, 0);
      break;
    case 8:
    case 10:
      Go(16, 2, kNone);
      break;
    case 9:
    case 11:
      Go(12, 2, 4);
      Message(5, 0);
      break;
    default:
      break;
  }
  return f.r3;
}

uint64_t Page14(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A67E8 SaveScreen::Page14");
  Fn f(sp, 96, calls, mode);
  const int32_t state = f.CallInt(kSlotState, 0x821A6800, 0);
  const int32_t message = int32_t(Data().message());
  if (message != 0 && message != 7) {
    return f.r3;
  }
  Data().set_wait(Data().wait() + 1);
  if (int32_t(Data().wait()) < 90) {
    return f.r3;
  }
  if (state == 1) {
    Go(12, 2, 4);
    Message(3, 0);
    return f.r3;
  }
  if (message == 0) {
    if (state != 3 || uint32_t(f.Call(kSlotPending, 0x821A69A8, 0)) != 0) {
      return f.r3;
    }
    const int32_t built = f.CallInt(kReplaysBuildList, 0x821A69B4, f.r3);
    if (built == 3) {
      Go(13, 2, 4);
      Message(9, 1);
      f.Call(kRequest980, 0x821A6AD8, 0);
    } else if (built == 7) {
      if (int32_t(G::save_buffers().count()) == 0) {
        Go(13, 2, 4);
        Message(4, 1);
      } else {
        const auto data = Data();
        data.set_answer(int32_t(data.kind2_flag()) != 0 ? 1 : 2);
        Go(16, 2, kNone);
      }
    }
    return f.r3;
  }
  // The message 7: a replay's load.
  if (state == 2) {
    if (f.CallInt(kWord992, 0x821A6858, 0) == 4) {
      const int32_t word = f.CallInt(kReplayWord84, 0x821A686C, Data().replay_index());
      Go(13, 2, 4);
      Message(word == 0 ? 9 : 11, 1);
    }
    f.Call(kRequest980, 0x821A6AD8, 0);
  } else if (state == 3) {
    if (uint32_t(f.Call(kSlotPending, 0x821A68A4, 0)) != 0) {
      return f.r3;
    }
    f.Call(kReplayCopyFrom, 0x821A68C4, G::kept_replay(),
           G::save_buffers().replay());
    Go(13, 2, 4);
    Message(8, 1);
    Data().set_answer(1);
  }
  return f.r3;
}

uint64_t Page15(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A3DE8 SaveScreen::Page15");
  Fn f(sp, 96, calls, mode);
  if (f.CallInt(kSlotState, 0x821A3DFC, 0) == 4) {
    return f.r3;
  }
  if (uint32_t(f.Call(kSlotDevice, 0x821A3E0C, 0)) != 0) {
    Data().set_wait(0);
    Go(14, 2, 4);
    Message(0, 2);
  } else {
    Go(12, 2, 4);
    Message(6, 0);
  }
  return f.r3;
}

uint64_t Page24(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A4AC0 SaveScreen::Page24");
  Fn f(sp, 128, calls, mode);
  using Controllers = schema::SaveInterfaceGlobals;
  using Entries = schema::RacerControllersGlobals;
  uint32_t entries[4];
  for (uint32_t i = 0; i < 4; i++) {
    entries[i] = Entries::mapper_entries_address() +
                 schema::PlayerMapperEntry::kSize * Load(Controllers::controllers_address() + 4 * i);
  }
  // The mapper's slot 10 (ActionMapper::Pressed) for `action`, unless the
  // entry's +4 is 1.
  auto pressed = [&](uint32_t entry, uint32_t action, uint32_t lr) {
    const schema::PlayerMapperEntry e(entry);
    if (int32_t(e.unread()) == 1) {
      return false;
    }
    const auto mapper = e.mapper();
    const uint32_t slot = Load(mapper.vtable() + 40);
    return (f.Call(slot, lr, mapper.address, action) & 0xFF) != 0;
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
    Data().set_controller_pick(uint32_t(player));
    Go(25, 2, kNone);
  }
  return f.r3;
}

}  // namespace save_screen
