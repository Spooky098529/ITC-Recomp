// The type 8 panel screen's slots and handlers
// (docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code.

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace type8_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

}  // namespace

// --- The leaves ---

uint64_t InitWords(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82238420 Type8PanelScreen::InitWords");
  const uint32_t screen = uint32_t(r3);
  StoreU32(base, screen + 13192, 3);
  StoreU32(base, screen + 15164, 3);
  StoreU32(base, screen + 13200, 4);
  StoreU32(base, screen + 15172, 4);
  return r3;
}

uint64_t IsLoaded(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82238418 Type8PanelScreen::IsLoaded");
  return ui_movies::IsLoaded(base, 4);
}

uint64_t OpenMovies(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238440 Type8PanelScreen::OpenMovies");
  return calls.open(4, 22586, sp, lr);
}

uint64_t DeleteMovies(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238450 Type8PanelScreen::DeleteMovies");
  return calls.remove(4, sp, lr);
}

uint64_t SetWord(uint8_t* base, uint64_t r3, uint32_t function) {
  uint32_t at = 0, value = 0;
  switch (function) {
    case 0x82238458:
      ITC_NATIVE_RUNNING("sub_82238458 Type8PanelScreen::OnMessage2");
      at = 15228;
      value = 1;
      break;
    case 0x82238530:
      ITC_NATIVE_RUNNING("sub_82238530 Type8PanelScreen::OnSaveLoaded");
      at = 15232;
      value = 0;
      break;
    case 0x82238540:
      ITC_NATIVE_RUNNING("sub_82238540 Type8PanelScreen::OnAnswer0");
      at = 15244;
      value = 0;
      break;
    case 0x82238550:
      ITC_NATIVE_RUNNING("sub_82238550 Type8PanelScreen::OnAnswer1");
      at = 15244;
      value = 1;
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82238560 Type8PanelScreen::OnAnswer2");
      at = 15244;
      value = 2;
      break;
  }
  StoreU32(base, uint32_t(r3) + at, value);
  return r3;
}

uint64_t Leave(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82238B10 Type8PanelScreen::Leave");
  const uint32_t screen = uint32_t(r3);
  if ((LoadU32(base, type67_screens::kFadeCountA) | LoadU32(base, type67_screens::kFadeCountB)) !=
      0) {
    return r3;
  }
  uint32_t message = 4;
  if (LoadU32(base, screen + 15240) == 0) {
    StoreU32(base, screen + 9608, 9);
    message = 6;
  }
  type67_screens::Post(base, uint32_t(r4), message);
  StoreU32(base, screen + 9656, 0);
  return r3;
}

}  // namespace type8_screens
