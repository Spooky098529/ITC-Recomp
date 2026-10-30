// The location screen's UI movie helpers and the transitions that only run
// its movies (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "ui_movies/game.h"

namespace location_screen {

namespace {

// The movie with id `id`: the id stored at the frame's +80, then
// UiMovies::Find over the movie table, as the original.
uint32_t FindMovie(uint8_t* base, uint32_t frame, uint32_t id) {
  guest::StoreU32(base, frame + 80, id);
  return guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

uint32_t ShowNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  guest::StoreU32(base, node + 80, 1);
  return node;
}

uint32_t HideNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  guest::StoreU32(base, node + 80, 0);
  return node;
}

uint32_t SetNodeTexture(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                        uint32_t texture) {
  return ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, sub_scene, child),
                                   texture);
}

// SetCursor's and ListNode's: node (sub_scene, 0)'s +16 = the value as a
// float (lfd of the doubleword, fcfid, frsp).
uint32_t SetNodeFloat16(uint8_t* base, uint32_t sp, uint32_t sub_scene, uint32_t value) {
  const uint32_t frame = sp - 112;
  const uint32_t movie = FindMovie(base, frame, kMenusMovie);
  guest::StoreU32(base, frame + 80, 0);
  guest::StoreU32(base, frame + 84, value);
  const float f = float(double(int64_t(uint64_t(value))));
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, 0);
  guest::StoreFloat(base, node + 16, f);
  return node;
}

}  // namespace

uint32_t ShowPictures(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8222A060 LocationScreen::ShowPictures");
  const uint32_t movie = FindMovie(base, sp - 112, kMenusMovie);
  SetNodeTexture(base, movie, 1, 4, 0);
  SetNodeTexture(base, movie, 1, 6, 1);
  SetNodeTexture(base, movie, 1, 5, 0);
  SetNodeTexture(base, movie, 1, 7, 1);
  ShowNode(base, movie, 1, 6);
  ShowNode(base, movie, 1, 7);
  HideNode(base, movie, 1, 8);
  HideNode(base, movie, 1, 9);
  HideNode(base, movie, 1, 13);
  return HideNode(base, movie, 1, 14);
}

uint32_t HidePictures(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8222A150 LocationScreen::HidePictures");
  const uint32_t movie = FindMovie(base, sp - 112, kMenusMovie);
  SetNodeTexture(base, movie, 1, 4, 0);
  SetNodeTexture(base, movie, 1, 5, 0);
  for (uint32_t child : {6u, 7u, 8u, 9u, 13u}) {
    HideNode(base, movie, 1, child);
  }
  return HideNode(base, movie, 1, 14);
}

uint32_t SetCarColor(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8222AC18 LocationScreen::SetCarColor");
  const uint32_t movie = FindMovie(base, sp - 96, kMenusMovie);
  const uint32_t color =
      int32_t(guest::LoadU32(base, guest::LoadU32(base, kScreen) + 892)) == 1 ? 2 : 0;
  const uint32_t node = ui_movies::FindNode(base, movie, 9, 7);
  ui_movies::SetColor(base, node, color);
  return node;
}

uint32_t SetCursor(uint8_t* base, uint32_t sp, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8222BFE8 LocationScreen::SetCursor");
  return SetNodeFloat16(base, sp, 9, value);
}

uint32_t ListNode(uint8_t* base, uint32_t sp, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8222C058 LocationScreen::ListNode");
  return SetNodeFloat16(base, sp, 10, value);
}

uint32_t ShowArrows(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8222C0C8 LocationScreen::ShowArrows");
  const uint32_t movie = FindMovie(base, sp - 112, kMenusMovie);
  const uint32_t screen = guest::LoadU32(base, kScreen);
  const int32_t count = int32_t(guest::LoadU32(base, screen + 808));
  if (count == 1) {
    HideNode(base, movie, 0xFFFFFFFFu, 17);
    return HideNode(base, movie, 0xFFFFFFFFu, 18);
  }
  const int32_t picked = int32_t(guest::LoadU32(base, screen + 800));
  if (picked == 0) {
    ShowNode(base, movie, 0xFFFFFFFFu, 18);
    return HideNode(base, movie, 0xFFFFFFFFu, 17);
  }
  if (picked == count - 1) {
    ShowNode(base, movie, 0xFFFFFFFFu, 17);
    return HideNode(base, movie, 0xFFFFFFFFu, 18);
  }
  ShowNode(base, movie, 0xFFFFFFFFu, 17);
  return ShowNode(base, movie, 0xFFFFFFFFu, 18);
}

uint64_t RunCardMovie(uint8_t* base, uint32_t sp, const RunCall& run) {
  ITC_NATIVE_RUNNING("sub_822291A8 LocationScreen::RunCardMovie");
  const uint32_t movie = FindMovie(base, sp - 96, kCardMovie);
  return run.run(run.user, movie, 0, 96, 0x822291D8);
}

uint32_t Transition1(uint8_t* base, uint32_t sp, uint32_t step, const RunCall& run) {
  ITC_NATIVE_RUNNING("sub_8222AD50 LocationScreen::Transition1");
  const uint32_t movie = FindMovie(base, sp - 112, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    mode_tick::ScreenFadesFadeOut(base, guest::LoadU32(base, 0x82001BF0), 0, 0, 0, 0);
    guest::StoreU32(base, step, 1);
  }
  const uint32_t fades = guest::LoadU32(base, mode_tick::kFadeOutCounts) |
                         guest::LoadU32(base, mode_tick::kFadeInCounts);
  run.run(run.user, movie, 0, 112, 0x8222ADE0);
  return fades != 0 ? 1 : 0;
}

uint32_t Transition18(uint8_t* base, uint32_t sp, uint32_t step, const RunCall& run) {
  ITC_NATIVE_RUNNING("sub_8222BE60 LocationScreen::Transition18");
  const uint32_t frame = sp - 128;
  const uint32_t menus = FindMovie(base, frame, kMenusMovie);
  const uint32_t card = FindMovie(base, frame, kCardMovie);
  if (guest::LoadU32(base, step) == 0) {
    ui_movies::PlayNamed(base, card, 53, 0, 0, 0, 0);
    guest::StoreU32(base, step, 1);
  }
  run.run(run.user, menus, 0, 128, 0x8222BEE8);
  if (uint32_t(run.run(run.user, card, 0, 128, 0x8222BEF4)) != 0) {
    return 0;
  }
  guest::StoreU32(base, guest::LoadU32(base, kConfirmMenu) + 24, 0);
  return 1;
}

}  // namespace location_screen
