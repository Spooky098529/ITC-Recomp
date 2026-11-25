// The save screen's kind 1 pages (the game data's load and save) and the
// profile's settings stored into the save data
// (docs/functions/save-screen.md): see save_screen.h.

#include "base/log.h"
#include "save_screen/internal.h"

namespace save_screen {

uint64_t StoreSettings(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A24B0 SaveScreen::StoreSettings");
  Fn f(sp, 224, calls);
  const auto data = Data();
  if (int32_t(data.settings_stored()) != 0) {
    return 1;
  }
  if (G::profile_wanted() != 0) {
    if (G::profile_read() == 997) {
      return 0;
    }
    const schema::SaveData save(uint32_t(f.Call(kSaveDataOf, 0x821A2500, 0)));
    const auto profile = G::profile_reader().settings();
    save.set_profile_option(profile.setting15_value() == 0 ? 1 : 0);
    const uint32_t control = profile.setting16_value();
    uint32_t word = 0;
    if (control == 0) {
      word = 2;
    } else if (control == 1) {
      word = 0;
    } else if (control == 2) {
      word = 1;
    } else {
      word = Load(f.frame + 80);  // the caller's leftover
    }
    save.set_camera_view(word);
    // The options' +44 to +103 (no field yet: SaveData's 129160).
    const uint32_t block = save.options_address() + 44, copy = f.frame + 96;
    const bool first = int32_t(profile.setting17_value()) == 0;
    f.Call(kMemcpy, 0x821A2590, copy, block, 60);
    Store(f.frame + 104, first ? 13 : 9);
    Store(f.frame + 108, first ? 9 : 13);
    f.Call(kMemcpy, 0x821A25A8, block, copy, 60);
    const bool second = int32_t(profile.setting18_value()) == 0;
    f.Call(kMemcpy, 0x821A25D8, copy, block, 60);
    Store(f.frame + 100, second ? 14 : 7);
    Store(f.frame + 132, second ? 7 : 14);
    f.Call(kMemcpy, 0x821A25F0, block, copy, 60);
    Data().set_settings_stored(1);
    return 1;
  }
  data.set_settings_stored(1);
  return 1;
}

uint64_t Page6(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A32C0 SaveScreen::Page6");
  Fn f(sp, 96, calls, mode);
  auto to_device = [&] {
    Jump(8);
    f.Call(kRequestDevice, 0x821A33D0, 0);
  };
  Data().set_wait(Data().wait() + 1);
  if (int32_t(Data().wait()) >= 45 && int32_t(Data().message()) == 0) {
    f.r3 = StoreSettings(f.frame, calls);
    if (int32_t(uint32_t(f.r3)) != 0) {
      to_device();
      return f.r3;
    }
  }
  if (f.CallInt(kMenuListAccepted, 0x821A3318, List()) == 0) {
    return f.r3;
  }
  f.Call(kMenuSoundPlay, 0x821A3328, 0);
  switch (int32_t(Data().message())) {
    case 0:
      f.r3 = StoreSettings(f.frame, calls);
      if (int32_t(uint32_t(f.r3)) != 0) {
        to_device();
      }
      break;
    case 5:
    case 6:
    case 7:
    case 11:
      Go(9, 2, kNone);
      break;
    default:
      break;
  }
  return f.r3;
}

uint64_t Page7(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A33E8 SaveScreen::Page7");
  Fn f(sp, 96, calls, mode);
  const int32_t state = f.CallInt(kSlotState, 0x821A3400, 0);
  Data().set_wait(Data().wait() + 1);
  if (int32_t(Data().wait()) < 90) {
    return f.r3;
  }
  const int32_t message = int32_t(Data().message());
  if (message == 10) {
    if (state == 1) {
      Go(5, 2, 3);
      Message(1, 0);
    } else if (state == 2) {
      const int32_t word = f.CallInt(kWord992, 0x821A3488, 0);
      if (word == 2) {
        Go(5, 2, 3);
        Message(16, 0);
      } else if (word == 3) {
        Go(5, 2, 3);
        Message(15, 0);
      }
      f.Call(kRequest980, 0x821A3690, 0);
    } else if (state == 3) {
      Go(6, 2, 3);
      Message(11, 1);
    }
  } else if (message == 9) {
    if (state == 1) {
      Go(5, 2, 3);
      Message(2, 0);
    } else {
      const int32_t check = f.CallInt(kGameDataCheck, 0x821A35B0, 0);
      if (check == 0) {
        f.Call(kGameDataSave, 0x821A35E4, f.r3);
        Go(7, 2, 3);
        Message(10, 2);
      } else if (check == 2) {
        Go(5, 2, 3);
        Message(15, 0);
      }
    }
  } else if (message == 4) {
    if (state == 1) {
      Go(5, 2, 3);
      Message(1, 0);
    } else if (state == 2) {
      if (f.CallInt(kWord992, 0x821A3654, 0) == 4) {
        Jump(11);
      }
      f.Call(kRequest980, 0x821A3690, 0);
    } else if (state == 3) {
      if (uint32_t(f.Call(kSlotPending, 0x821A36A4, 0)) == 0) {
        const uint64_t save = f.Call(kSaveDataOf, 0x821A36B0, f.r3);
        f.Call(kMemcpy, 0x821A36C8, save, G::save_buffers().game_data(), kGameDataBytes);
        Go(6, 2, 3);
        Message(6, 1);
      }
    }
  }
  return f.r3;
}

uint64_t Page8(uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A3728 SaveScreen::Page8");
  Fn f(sp, 96, calls, mode);
  if (f.CallInt(kSlotState, 0x821A373C, 0) == 4) {
    return f.r3;
  }
  if (uint32_t(f.Call(kSlotDevice, 0x821A374C, 0)) == 0) {
    Go(9, 2, kNone);
    return f.r3;
  }
  const uint32_t check = uint32_t(f.Call(kGameDataCheck, 0x821A375C, 0));
  if (check == 0 || check == 2) {
    Go(5, 2, 3);
    Message(8, 0);
  } else if (check == 1) {
    Go(7, 2, 3);
    Message(4, 2);
    f.Call(kGameDataLoad, 0x821A3820, 0);
  }
  return f.r3;
}

}  // namespace save_screen
