// The type 6 and 7 list screens' and the type 6 node screen's handlers and
// vtable slots (docs/functions/type67-screens.md): see type67_screens.h.

#include "type67_screens/type67_screens.h"

#include "alt_hud/alt_hud.h"
#include "base/log.h"
#include "guest.h"
#include "schema/type67_screens.h"
#include "ui_movies/game.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using LS = schema::ListScreen;
using NS = schema::NodeScreen;

// UiMovies::IsLoaded(id) as 0 or 1 (the originals' cntlzw, rlwinm, xori).
uint64_t Loaded(uint32_t id) { return ui_movies::IsLoaded(id) != 0 ? 1 : 0; }

// A handler's frame (`bytes` below `sp`) with its back chain.
uint32_t Frame(uint32_t sp, uint32_t bytes) {
  StoreU32(sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint64_t ReleaseList(uint64_t list_r3, uint32_t frame, uint32_t lr,
                     const Calls& calls) {
  const uint32_t list = uint32_t(list_r3);
  if (calls.native_heap && alt_hud::MenuListReleasable(list)) {
    return *alt_hud::MenuListRelease(list, frame);
  }
  const uint64_t args[6] = {list_r3, 0, 0, 0, 0, 0};
  return calls.call_set(kMenuListRelease, args, 1, frame, lr);
}

// --- The list screens ---

uint64_t ListScreenOnEnter(uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251618 ListScreen::OnEnter");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  LS(screen).set_movie_loaded(0);
  LS(screen).set_race_set(0);
  const uint64_t result = Slot(r3, 32, r4, frame, 0x82251648, calls);
  LS(screen).set_state(LS(screen).open_at_start() != 0 ? 1 : 9);
  return result;
}

uint64_t ListScreenOpenMovie(uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251508 ListScreen::OpenMovie");
  LS(uint32_t(r3)).set_movie_loaded(0);
  return Slot(r3, 48, r4, sp, lr, calls);
}

uint64_t ListScreenWaitForMovie(uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251678 ListScreen::WaitForMovie");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  if (LS(screen).movie_loaded() == 0) {
    LS(screen).set_movie_loaded(uint32_t(Slot(r3, 4, r4, frame, 0x822516B0, calls)));
  }
  if (LS(screen).movie_loaded() != 0) {
    LS(screen).set_movie_ready(1);
    LS(screen).set_state(LS(screen).open_at_start() != 0 ? 9 : 4);
  }
  return Slot(r3, 52, r4, frame, 0x822516F8, calls);
}

uint64_t ListScreenLeave(uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251760 ListScreen::Leave");
  const uint32_t frame = Frame(sp, 96);
  const uint64_t result = Slot(r3, 72, r4, frame, 0x82251784, calls);
  LS(uint32_t(r3)).set_state(7);
  return result;
}

uint64_t ListScreenOnLeft(uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82251710 ListScreen::OnLeft");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  const uint64_t result = Slot(r3, 36, r4, frame, 0x82251734, calls);
  LS(screen).set_state(int32_t(LS(screen).race_set()) == 1 ? 11 : 6);
  return result;
}

uint64_t ListScreenIsLoaded(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82242F70 ListScreen::IsLoaded");
  Frame(sp, 96);
  return Loaded(LS(uint32_t(r3)).movie());
}

uint64_t ListScreenOpenAsset(uint64_t r3, bool type7, uint32_t sp, uint32_t lr,
                             const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82242F10 Type7ListScreen::OpenMovie");
  } else {
    ITC_NATIVE_RUNNING("sub_82233320 Type6ListScreen::OpenMovie");
  }
  return calls.open(LS(uint32_t(r3)).movie(), type7 ? 22570 : 22565, sp, lr);
}

uint64_t ListScreenRelease(uint64_t r3, bool type7, uint32_t sp,
                           const Calls& calls) {
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  uint64_t result;
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82242F20 Type7ListScreen::Release");
    calls.remove(LS(screen).movie(), frame, 0x82242F3C);
    ReleaseList(r3 + 10144, frame, 0x82242F44, calls);
    ReleaseList(r3 + 10288, frame, 0x82242F4C, calls);
    ReleaseList(r3 + 10360, frame, 0x82242F54, calls);
    result = ReleaseList(r3 + 10216, frame, 0x82242F5C, calls);
  } else {
    ITC_NATIVE_RUNNING("sub_82233330 Type6ListScreen::Release");
    calls.remove(LS(screen).movie(), frame, 0x8223334C);
    ReleaseList(r3 + 10216, frame, 0x82233354, calls);
    ReleaseList(r3 + 10288, frame, 0x8223335C, calls);
    result = ReleaseList(r3 + 10144, frame, 0x82233364, calls);
  }
  return result;
}

// --- The node screen ---

uint64_t NodeScreenOnEnter(uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822591B0 NodeScreen::OnEnter");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  NS(screen).set_movie_loaded(0);
  const uint64_t result = Slot(r3, 36, r4, frame, 0x822591DC, calls);
  NS(screen).set_state(NS(screen).open_at_start() != 0 ? 1 : 9);
  return result;
}

uint64_t NodeScreenOpenMovie(uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822590D0 NodeScreen::OpenMovie");
  NS(uint32_t(r3)).set_movie_loaded(0);
  return Slot(r3, 52, r4, sp, lr, calls);
}

uint64_t NodeScreenWaitForMovie(uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259208 NodeScreen::WaitForMovie");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  if (NS(screen).movie_loaded() == 0) {
    NS(screen).set_movie_loaded(uint32_t(Slot(r3, 12, r4, frame, 0x82259240, calls)));
  }
  if (NS(screen).movie_loaded() != 0) {
    NS(screen).set_state(NS(screen).open_at_start() != 0 ? 9 : 4);
  }
  return Slot(r3, 56, r4, frame, 0x82259280, calls);
}

uint64_t NodeScreenLeave(uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822592D8 NodeScreen::Leave");
  const uint32_t frame = Frame(sp, 96);
  const uint64_t result = Slot(r3, 76, r4, frame, 0x822592FC, calls);
  NS(uint32_t(r3)).set_state(7);
  return result;
}

uint64_t NodeScreenOnLeft(uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259298 NodeScreen::OnLeft");
  const uint32_t frame = Frame(sp, 96);
  const uint64_t result = Slot(r3, 40, r4, frame, 0x822592BC, calls);
  NS(uint32_t(r3)).set_state(6);
  return result;
}

uint64_t NodeScreenIsLoaded(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82236FD8 Type6NodeScreen::IsLoaded");
  Frame(sp, 96);
  return Loaded(NS(uint32_t(r3)).movie());
}

uint64_t NodeScreenOpenAsset(uint64_t r3, uint32_t sp, uint32_t lr,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82236EB0 Type6NodeScreen::OpenMovie");
  return calls.open(NS(uint32_t(r3)).movie(), 22566, sp, lr);
}

uint64_t NodeScreenRelease(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82236EC0 Type6NodeScreen::Release");
  const uint32_t frame = Frame(sp, 96);
  calls.remove(NS(uint32_t(r3)).movie(), frame, 0x82236EDC);
  ReleaseList(r3 + 10132, frame, 0x82236EE4, calls);
  return ReleaseList(r3 + 10204, frame, 0x82236EEC, calls);
}

}  // namespace type67_screens
