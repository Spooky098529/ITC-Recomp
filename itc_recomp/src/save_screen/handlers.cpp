// The save screen's state machine handlers (docs/functions/save-screen.md):
// see save_screen.h.

#include "base/log.h"
#include "front_end_screen_helpers/front_end_screen_helpers.h"
#include "save_screen/internal.h"

namespace save_screen {

uint64_t OnEnter(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A2608 SaveScreen::OnEnter");
  Fn f(sp, 112, calls);
  Machine(machine).set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x821A263C
  // and the VirtualAlloc(72) at 0x821A2678; the globals hold their handles.
  const schema::PageObject pages = schema::PageObject::New();
  G::set_pages(pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  const uint32_t list = schema::MenuList::New().address;
  G::set_list(list);
  if (list != 0) {
    const schema::MenuList view(list);
    view.set_enabled(0);
    view.set_player(0);
  }
  // VirtualAlloc(68) in host memory (E5 step 5); its answer is OnEnter's r3.
  const schema::SaveScreenData data = schema::SaveScreenData::New();
  f.r3 = data.address;
  G::set_data(data);
  data.set_set_up(0);
  data.set_settings_stored(0);
  data.set_prompts(0);
  data.set_next_state(21);
  Machine(machine).set_state(int32_t(Machine(machine).load_at_once()) != 0 ? 1 : 9);
  return f.r3;
}

uint64_t OpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1D10 SaveScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, 44, 22588);
}

uint64_t WaitMovie(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821A26F0 SaveScreen::WaitMovie");
  const uint32_t machine = uint32_t(r3);
  Store(sp - 96, sp);
  if (int32_t(Machine(machine).loaded()) == 0) {
    r3 = ui_movies::IsLoaded(44);
    if (int32_t(uint32_t(r3)) != 0) {
      Machine(machine).set_loaded(1);
    }
  }
  if (int32_t(Machine(machine).loaded()) != 0) {
    Machine(machine).set_state(int32_t(Machine(machine).load_at_once()) != 0 ? 9 : 4);
  }
  return r3;
}

void OpenKind(uint32_t machine, uint32_t kind) {
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
  Data().set_kind(kind);
  Machine(machine).set_state(int32_t(Machine(machine).loaded()) != 0 ? 4 : 1);
}

namespace {

// The kinds' dialogs: the data's +28 = 0 and the dialog, the page block to
// `page` by `step`, MenuList::Init with the screen's ids.
uint64_t ShowWith(uint32_t sp, uint32_t page, uint32_t step, uint32_t answer,
                  uint32_t lr, const Calls& calls) {
  Fn f(sp, 128, calls);
  Data().set_cancel(0);
  Go(page, step, kNone);
  Store(f.frame + 92, 158);
  Store(f.frame + 84, 159);
  Data().set_answer(answer);
  const uint64_t args[8] = {List(), 1, 1, 0, 154, 153, 155, 156};
  f.r3 = calls.CallArgs(kMenuListInit, f.frame, lr, args);
  return f.r3;
}

}  // namespace

uint64_t ShowKind2(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1D38 SaveScreen::ShowKind2");
  Data().set_message(2);
  Data().set_dialog_state(1);
  Data().set_wait(0);
  const uint64_t r3 = ShowWith(sp, 13, 4, 0, 0x821A1DF0, calls);
  Data().set_kind2_flag(1);
  Data().set_prompts(1);
  return r3;
}

uint64_t ShowKind7(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A2138 SaveScreen::ShowKind7");
  Data().set_message(0);
  Data().set_dialog_state(0);
  const uint64_t r3 = ShowWith(sp, 24, 6, 1, 0x821A21E4, calls);
  Data().set_prompts(1);
  return r3;
}

uint64_t Show(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A6608 SaveScreen::Show");
  Fn f(sp, 96, calls, r3);
  const uint32_t kind = Data().kind();
  switch (kind) {
    case 0: {  // ShowCheck sub_821A6650, split off by the recompiler
      f.Call(kSaveBuffersCreate, 0x821A6654);
      f.r3 = front_end_screen_helpers::SaveScreenStartCheck(f.frame, calls);
      break;
    }
    case 1:
      f.Call(kSaveBuffersCreate, 0x821A6660);
      f.r3 = front_end_screen_helpers::SaveScreenStart(f.frame, calls);
      break;
    case 2:
      f.r3 = ShowKind2(f.frame, calls);
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
      f.r3 = ShowKind7(f.frame, calls);
      break;
    default:
      break;
  }
  return f.r3;
}

void Cancel() {
  ITC_NATIVE_RUNNING("sub_821A1D20 SaveScreen::Cancel");
  Data().set_cancel(1);
}

uint64_t Leave(uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A4C58 SaveScreen::Leave");
  Fn f(sp, 112, calls);
  Machine(machine).set_state(7);
  const uint32_t movie = FindMovie(44, f.frame + 80);
  f.Call(kFinishAll, 0x821A4C98, movie);
  f.Call(kMoviesDelete, 0x821A4CA0, 44);
  const int32_t type = int32_t(G::mode_type());
  if (type == 2 || type == 4 || type == 6 || type == 8 || type == 10) {
    Post(mode, 4);
  }
  return f.r3;
}

uint64_t OnLeft(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A1CA0 SaveScreen::OnLeft");
  Fn f(sp, 112, calls);
  f.Call(kMenuListRelease, 0x821A1CB8, List());
  f.Call(kVirtualFree, 0x821A1CD4, Pages().address, 0, kMemRelease);
  f.Call(kVirtualFree, 0x821A1CE4, List(), 0, kMemRelease);
  f.Call(kVirtualFree, 0x821A1CF8, Data().address, 0, kMemRelease);
  G::set_pages(0u);
  G::set_list(0u);
  G::set_data(0u);
  return f.r3;
}

}  // namespace save_screen
