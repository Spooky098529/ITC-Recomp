// The panel screens' handlers (docs/functions/type67-screens.md): see
// type67_screens.h. The car list (+9644) and the group pair (+9648) are
// called through their vtables (Slot).

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "guest.h"
#include "schema/car_list.h"
#include "schema/type67_screens.h"
#include "ui_movies/game.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using G = schema::Type67ScreensGlobals;
using guest::StoreU32;
using PS = schema::PanelScreen;
using T67PS = schema::Type67PanelScreen;

// The dialog's UI movie and its default cursor word.
constexpr uint32_t kDialogMovie = 16, kDialogCursor = 40960;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  StoreU32(sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint64_t PanelScreenOnEnter(uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249D18 PanelScreen::OnEnter");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  PS(screen).set_movie_loaded(0);
  Slot(PS(screen).car_list().address, 8, r4, frame, 0x82249D48, calls);
  const uint64_t result =
      Slot(PS(screen).group_pair().address, 12, std::nullopt, frame, 0x82249D5C, calls);
  PS(screen).set_state(PS(screen).open_at_start() != 0 ? 1 : 9);
  return result;
}

uint64_t PanelScreenOpenMovie(uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248CD0 PanelScreen::OpenMovie");
  return Slot(r3, 52, r4, sp, lr, calls);
}

uint64_t PanelScreenWaitForMovie(uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249D88 PanelScreen::WaitForMovie");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  const uint64_t loaded = Slot(r3, 8, r4, frame, 0x82249DAC, calls);
  PS(screen).set_movie_loaded(uint32_t(loaded));
  if (uint32_t(loaded) != 0) {
    uint32_t state;
    if (PS(screen).opens_itself() != 0) {
      state = 3;
    } else {
      state = PS(screen).open_at_start() != 0 ? 9 : 4;
    }
    PS(screen).set_state(state);
  }
  return loaded;
}

uint64_t PanelScreenOpen(uint64_t r3, uint32_t sp, uint32_t lr,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249DF8 PanelScreen::Open");
  const uint32_t screen = uint32_t(r3);
  if (PS(screen).movie_loaded() == 0) {
    PS(screen).set_state(1);
    return r3;
  }
  const uint32_t list = PS(screen).car_list().address;
  PS(screen).set_state(4);
  return Slot(list, 52, 1, sp, lr, calls);
}

uint64_t PanelScreenShow(uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248CE0 PanelScreen::Show");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  if (PS(screen).opens_itself() == 0) {
    const uint64_t list_tab = Slot(r3, 24, r4, frame, 0x82248D10, calls);
    Slot(PS(screen).car_list().address, 60, list_tab, frame, 0x82248D2C, calls);
    const uint64_t entry = Slot(r3, 28, std::nullopt, frame, 0x82248D40, calls);
    Slot(PS(screen).car_list().address, 64, entry, frame, 0x82248D5C, calls);
    const uint64_t slot = Slot(r3, 32, std::nullopt, frame, 0x82248D70, calls);
    Slot(PS(screen).car_list().address, 68, slot, frame, 0x82248D8C, calls);
    Slot(PS(screen).car_list().address, 12, std::nullopt, frame, 0x82248DA0, calls);
  } else {
    Slot(PS(screen).car_list().address, 12, r4, frame, 0x82248DA0, calls);
  }
  Slot(PS(screen).car_list().address, 4, std::nullopt, frame, 0x82248DB4, calls);
  return Slot(PS(screen).group_pair().address, 20, std::nullopt, frame, 0x82248DC8,
              calls);
}

uint64_t PanelScreenUpdate(uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248C48 PanelScreen::Update");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  bool first = true;
  // The first call's r4 is the handler's; the later ones' what the last
  // call left.
  auto next_r4 = [&first, r4]() -> std::optional<uint64_t> {
    if (first) {
      first = false;
      return r4;
    }
    return std::nullopt;
  };
  if (const uint32_t pair = PS(screen).group_pair().address; pair != 0) {
    r3 = Slot(pair, 8, next_r4(), frame, 0x82248C7C, calls);
  }
  if (const uint32_t list = PS(screen).car_list().address; list != 0) {
    r3 = Slot(list, 24, next_r4(), frame, 0x82248C9C, calls);
  }
  if (const uint32_t pair = PS(screen).group_pair().address; pair != 0) {
    r3 = Slot(pair, 4, next_r4(), frame, 0x82248CBC, calls);
  }
  return r3;
}

uint64_t PanelScreenOnMessage24(uint64_t r3, uint32_t sp, uint32_t lr,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248DF8 PanelScreen::OnMessage24");
  return Slot(PS(uint32_t(r3)).car_list().address, 44, 0, sp, lr, calls);
}

uint64_t PanelScreenLeave(uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249E38 PanelScreen::Leave");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  Slot(PS(screen).car_list().address, 28, r4, frame, 0x82249E68, calls);
  Slot(PS(screen).group_pair().address, 28, std::nullopt, frame, 0x82249E7C, calls);
  const uint64_t result = Slot(r3, 72, r4, frame, 0x82249E94, calls);
  PS(screen).set_state(7);
  return result;
}

uint64_t PanelScreenOnLeft(uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248B08 PanelScreen::OnLeft");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  Slot(PS(screen).car_list().address, 16, r4, frame, 0x82248B30, calls);
  return Slot(PS(screen).group_pair().address, 16, std::nullopt, frame, 0x82248B44,
              calls);
}

uint64_t PanelScreenDrawList(uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248B58 PanelScreen::DrawList");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  if (PS(screen).car_list().address != 0) {
    Slot(PS(screen).car_list().address, 20, r4, frame, 0x82248B94, calls);
    uint32_t pick = 0;
    if (uint32_t(Slot(PS(screen).car_list().address, 32, std::nullopt, frame,
                      0x82248BA8, calls)) != 0) {
      pick = 12;
    } else if (uint32_t(Slot(PS(screen).car_list().address, 36, std::nullopt, frame,
                             0x82248BD0, calls)) != 0) {
      pick = 16;
    } else if (uint32_t(Slot(PS(screen).car_list().address, 40, std::nullopt, frame,
                             0x82248BF8, calls)) != 0) {
      pick = 20;
    }
    if (pick != 0) {
      Slot(r3, pick, r4, frame, 0x82248C18, calls);
    }
  }
  return Slot(r3, 40, r4, frame, 0x82248C30, calls);
}

uint64_t Type7PanelScreenDrawList(uint64_t r3, uint64_t r4, uint32_t sp,
                                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82240908 Type7PanelScreen::DrawList");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  uint64_t result = PanelScreenDrawList(r3, r4, frame, calls);
  if (T67PS(screen).ask_quit() != 0) {
    result = Type7PanelScreenAskQuit(r3, r4, frame, calls);
  }
  return result;
}

uint64_t Type7PanelScreenDraw(uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82240958 Type7PanelScreen::Draw");
  const uint32_t screen = uint32_t(r3);
  const int32_t quit = int32_t(T67PS(screen).quit_dialog());
  if (quit == 0) {
    return Type7PanelScreenDrawList(r3, r4, sp, calls);
  }
  if (quit == 2 && (G::fade_out_count() | G::fade_in_count()) == 0) {
    T67PS(screen).set_state(10);
  }
  return r3;
}

namespace {

// The panel screens' AskQuit: the dialog's texts (movie 16: 27, then 2 and
// 3), its cursor word 1, message `message` posted, +9656 = 1, +9660 = -1;
// the return addresses type 7's less `delta`. -1 (the r3 it leaves).
uint64_t AskQuit(uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls,
                 uint32_t message, uint32_t delta) {
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 128);
  StoreU32(frame + 80, kDialogMovie);
  const uint32_t movie =
      LoadU32(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80));
  calls.set_text(frame, 0x8224055C - delta, movie, 0xFFFFFFFFu, 0, 27);
  calls.set_text(frame, 0x82240570 - delta, movie, 1, 3, 2);
  calls.set_text(frame, 0x82240584 - delta, movie, 1, 4, 3);
  StoreU32(movie + kDialogCursor, 1);
  Post(uint32_t(r4), message);
  T67PS(screen).set_quit_dialog(1);
  T67PS(screen).set_dialog_answer(0xFFFFFFFFu);
  return ~uint64_t(0);
}

}  // namespace

uint64_t Type7PanelScreenAskQuit(uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82240518 Type7PanelScreen::AskQuit");
  return AskQuit(r3, r4, sp, calls, 6, 0);
}

uint64_t Type6PanelScreenAskQuit(uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230BA8 Type6PanelScreen::AskQuit");
  return AskQuit(r3, r4, sp, calls, 7, 0x82240518 - 0x82230BA8);
}

uint64_t Type6PanelScreenDrawList(uint64_t r3, uint64_t r4, uint32_t sp,
                                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230EF0 Type6PanelScreen::DrawList");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  uint64_t result = PanelScreenDrawList(r3, r4, frame, calls);
  if (T67PS(screen).ask_quit() != 0) {
    result = Type6PanelScreenAskQuit(r3, r4, frame, calls);
  }
  return result;
}

uint64_t Type6PanelScreenDraw(uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230F40 Type6PanelScreen::Draw");
  const uint32_t screen = uint32_t(r3);
  const int32_t quit = int32_t(T67PS(screen).quit_dialog());
  if (quit == 0) {
    // A tail call.
    return Type6PanelScreenDrawList(r3, r4, sp, calls);
  }
  if (quit == 2 && (G::fade_out_count() | G::fade_in_count()) == 0) {
    T67PS(screen).set_state(10);
  }
  return r3;
}

}  // namespace type67_screens
