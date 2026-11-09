// The save screen's state machine handlers (docs/functions/save-screen.md):
// see save_screen.h.

#include "base/log.h"
#include "front_end_screen_helpers/front_end_screen_helpers.h"
#include "save_screen/internal.h"

namespace save_screen {

uint64_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A2608 SaveScreen::OnEnter");
  Fn f(base, sp, 112, calls);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(f.Call(kVirtualAlloc, 0x821A263C, 0, 28, 4096, 4));
  Store(base, kPages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 24, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  const uint32_t list = uint32_t(f.Call(kVirtualAlloc, 0x821A2678, 0, 72, 4096, 4));
  Store(base, kList, list);
  if (list != 0) {
    Store(base, list + 68, 0);
    Store(base, list + 60, 0);
  }
  const uint32_t data = uint32_t(f.Call(kVirtualAlloc, 0x821A26A4, 0, 68, 4096, 4));
  Store(base, kData, data);
  Store(base, data + 0, 0);
  Store(base, data + 60, 0);
  Store(base, data + 56, 0);
  Store(base, data + 32, 21);
  Store(base, machine + kMachineState, int32_t(Load(base, machine + kMachineAtOnce)) != 0 ? 1 : 9);
  return f.r3;
}

uint64_t OpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1D10 SaveScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, 44, 22588);
}

uint64_t WaitMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821A26F0 SaveScreen::WaitMovie");
  const uint32_t machine = uint32_t(r3);
  Store(base, sp - 96, sp);
  if (int32_t(Load(base, machine + kMachineLoaded)) == 0) {
    r3 = ui_movies::IsLoaded(base, 44);
    if (int32_t(uint32_t(r3)) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (int32_t(Load(base, machine + kMachineLoaded)) != 0) {
    Store(base, machine + kMachineState,
          int32_t(Load(base, machine + kMachineAtOnce)) != 0 ? 9 : 4);
  }
  return r3;
}

void OpenKind(uint8_t* base, uint32_t machine, uint32_t kind) {
  switch (kind) {
    case 0: {
      ITC_NATIVE_RUNNING("sub_821A2760 SaveScreen::OpenKind0");
      break;
    }
    case 1: {
      ITC_NATIVE_RUNNING("sub_821A2790 SaveScreen::OpenKind1");
      break;
    }
    case 2: {
      ITC_NATIVE_RUNNING("sub_821A2820 SaveScreen::OpenKind2");
      break;
    }
    case 3: {
      ITC_NATIVE_RUNNING("sub_821A27F0 SaveScreen::OpenKind3");
      break;
    }
    case 4: {
      ITC_NATIVE_RUNNING("sub_821A2850 SaveScreen::OpenKind4");
      break;
    }
    case 5: {
      ITC_NATIVE_RUNNING("sub_821A2880 SaveScreen::OpenKind5");
      break;
    }
    case 6: {
      ITC_NATIVE_RUNNING("sub_821A27C0 SaveScreen::OpenKind6");
      break;
    }
    default: {
      ITC_NATIVE_RUNNING("sub_821A28B0 SaveScreen::OpenKind7");
      break;
    }
  }
  Store(base, Data(base) + 36, kind);
  Store(base, machine + kMachineState, int32_t(Load(base, machine + kMachineLoaded)) != 0 ? 4 : 1);
}

namespace {

// The kinds' dialogs: the data's +28 = 0 and the dialog, the page block to
// `page` by `step`, MenuList::Init with the screen's ids.
uint64_t ShowWith(uint8_t* base, uint32_t sp, uint32_t page, uint32_t step, uint32_t answer,
                  uint32_t lr, const Calls& calls) {
  Fn f(base, sp, 128, calls);
  Store(base, Data(base) + 28, 0);
  Go(base, page, step, kNone);
  Store(base, f.frame + 92, 158);
  Store(base, f.frame + 84, 159);
  Store(base, Data(base) + 4, answer);
  const uint64_t args[8] = {List(base), 1, 1, 0, 154, 153, 155, 156};
  f.r3 = calls.CallArgs(kMenuListInit, f.frame, lr, args);
  return f.r3;
}

}  // namespace

uint64_t ShowKind2(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1D38 SaveScreen::ShowKind2");
  Store(base, Data(base) + 12, 2);
  Store(base, Data(base) + 16, 1);
  Store(base, Data(base) + 24, 0);
  const uint64_t r3 = ShowWith(base, sp, 13, 4, 0, 0x821A1DF0, calls);
  Store(base, Data(base) + 8, 1);
  Store(base, Data(base) + 56, 1);
  return r3;
}

uint64_t ShowKind7(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A2138 SaveScreen::ShowKind7");
  Store(base, Data(base) + 12, 0);
  Store(base, Data(base) + 16, 0);
  const uint64_t r3 = ShowWith(base, sp, 24, 6, 1, 0x821A21E4, calls);
  Store(base, Data(base) + 56, 1);
  return r3;
}

uint64_t Show(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A6608 SaveScreen::Show");
  Fn f(base, sp, 96, calls, r3);
  const uint32_t kind = Load(base, Data(base) + 36);
  switch (kind) {
    case 0: {  // ShowCheck sub_821A6650, split off by the recompiler
      f.Call(kSaveBuffersCreate, 0x821A6654);
      f.r3 = front_end_screen_helpers::SaveScreenStartCheck(base, f.frame, calls);
      break;
    }
    case 1:
      f.Call(kSaveBuffersCreate, 0x821A6660);
      f.r3 = front_end_screen_helpers::SaveScreenStart(base, f.frame, calls);
      break;
    case 2:
      f.r3 = ShowKind2(base, f.frame, calls);
      break;
    case 3:
      f.Call(kShowKind3, 0x821A666C);
      break;
    case 4:
      f.Call(kShowKind4, 0x821A667C);
      break;
    case 5:
      f.Call(kShowKind5, 0x821A6684);
      break;
    case 6:
      f.Call(kSaveBuffersCreate, 0x821A668C);
      f.Call(kShowKind6, 0x821A6690);
      break;
    case 7:
      f.r3 = ShowKind7(base, f.frame, calls);
      break;
    default:
      break;
  }
  return f.r3;
}

void Cancel(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821A1D20 SaveScreen::Cancel");
  Store(base, Data(base) + 28, 1);
}

uint64_t Leave(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A4C58 SaveScreen::Leave");
  Fn f(base, sp, 112, calls);
  Store(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, 44, f.frame + 80);
  f.Call(kFinishAll, 0x821A4C98, movie);
  f.Call(kMoviesDelete, 0x821A4CA0, 44);
  const int32_t type = int32_t(Load(base, kModeType));
  if (type == 2 || type == 4 || type == 6 || type == 8 || type == 10) {
    Post(base, mode, 4);
  }
  return f.r3;
}

uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1CA0 SaveScreen::OnLeft");
  Fn f(base, sp, 112, calls);
  f.Call(kMenuListRelease, 0x821A1CB8, List(base));
  f.Call(kVirtualFree, 0x821A1CD4, Pages(base), 0, kMemRelease);
  f.Call(kVirtualFree, 0x821A1CE4, List(base), 0, kMemRelease);
  f.Call(kVirtualFree, 0x821A1CF8, Data(base), 0, kMemRelease);
  Store(base, kPages, 0);
  Store(base, kList, 0);
  Store(base, kData, 0);
  return f.r3;
}

}  // namespace save_screen
