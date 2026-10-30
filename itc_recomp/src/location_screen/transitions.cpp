// The location screen's page transitions and the movie helpers they share
// that set texts (docs/functions/location-screen.md): UiMovie::SetText and
// the other text setters through their plain C++ (the adapters checked the
// UI movies' heap first: hooks::LocationScreenTextsNative), the stops and
// runs through `calls`. SetText's unset varargs (r7 to r10) are passed as 0
// (the user's call, 2026-10-30: only dead words of the caller's parameter
// area differ).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstdlib>

namespace location_screen {

namespace {

constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kRun = 0x8211E1B8;

uint32_t Screen(uint8_t* base) { return guest::LoadU32(base, kScreen); }

// The movie with id `id`: the id at the frame's +80, then UiMovies::Find.
uint32_t Movie(uint8_t* base, uint32_t frame, uint32_t id) {
  guest::StoreU32(base, frame + 80, id);
  return guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

uint32_t Required(const std::optional<uint32_t>& r3, const char* what) {
  if (!r3) {
    ITC_LOG_ERROR("location screen: {} refused after the adapter's checks", what);
    std::abort();
  }
  return *r3;
}

// UiMovie::SetText from the frame at `frame` (its r1), no arguments.
uint32_t SetText(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t movie,
                 uint32_t sub_scene, uint32_t child, uint32_t id) {
  static constexpr uint64_t kNoArguments[4] = {0, 0, 0, 0};
  return Required(ui_movies::SetText(base, *calls.format, frame, movie, sub_scene, child, id,
                                     kNoArguments),
                  "UiMovie::SetText");
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

// UiMovie::SetNodeColor: the node (sub_scene, child)'s color.
uint32_t SetNodeColor(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                      uint32_t color) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  ui_movies::SetColor(base, node, color);
  return node;
}

// UiMovie::SetPictureTexture: the picture's texture swapped; the old one.
uint32_t SetPictureTexture(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                           uint32_t texture) {
  const uint32_t picture = ui_movies::FindNode(base, movie, sub_scene, child);
  const uint32_t slot = ui_movies::PictureTextureSlot(base, movie, picture);
  const uint32_t old = guest::LoadU32(base, slot);
  guest::StoreU32(base, slot, texture);
  return old;
}

uint32_t PlayNamed(uint8_t* base, uint32_t movie, uint32_t named, uint32_t chained = 0,
                   uint32_t reversed = 0) {
  return ui_movies::PlayNamed(base, movie, named, chained, reversed, 0, 0);
}

// UiMovie::LoopLast called from the frame at `frame`: its r3 (the slot it
// moved to, else a copy in its 176-byte frame or the movie).
uint32_t LoopLast(uint8_t* base, uint32_t frame, uint32_t movie) {
  const ui_movies::Moved moved = ui_movies::LoopLast(base, movie);
  if (moved.slot) {
    return *moved.slot;
  }
  return moved.looked ? frame - 176 + 80 : movie;
}

void FinishNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint32_t named) {
  calls.Call(kFinishNamed, frame, lr, movie, named);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

// Whether a fade runs (either fade's counts word set).
bool Fading(uint8_t* base) {
  return (guest::LoadU32(base, mode_tick::kFadeOutCounts) |
          guest::LoadU32(base, mode_tick::kFadeInCounts)) != 0;
}

// LocationScreen::OpponentHasCard from the frame at `frame`: its plain C++,
// or its adapter through `calls` where the item's copy can't be native.
uint32_t HasCard(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr) {
  if (const auto r3 = OpponentHasCard(base, frame)) {
    return *r3;
  }
  return uint32_t(calls.Call(0x82227708, frame, lr, 0));
}

}  // namespace

uint32_t ResetMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222CD60 LocationScreen::ResetMovie");
  const uint32_t frame = sp - 192;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  constexpr uint32_t kTexts[5] = {138, 139, 111, 140, 141};
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, frame + 96 + 4 * i, kTexts[i]);
    guest::StoreU32(base, frame + 128 + 4 * i, 5 + i);
  }
  ui_movies::ResetPose(base, movie);
  calls.Call(kFinishAll, frame, 0x8222CDE8, movie);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  SetText(base, calls, frame, movie, 0xFFFFFFFFu, 35,
          guest::LoadU32(base, Screen(base) + 852) + 369);
  SetText(base, calls, frame, movie, 0xFFFFFFFFu, 36, guest::LoadU32(base, save + 6104) + 128);
  for (uint32_t i = 0; i < 5; i++) {
    SetText(base, calls, frame, movie, 9, guest::LoadU32(base, frame + 128 + 4 * i),
            guest::LoadU32(base, frame + 96 + 4 * i));
  }
  const uint32_t menu = guest::LoadU32(base, kMenu);
  if (guest::LoadU32(base, Screen(base) + 892) == 0 ||
      int32_t(guest::LoadU32(base, menu)) != 2) {
    PlayNamed(base, movie, guest::LoadU32(base, menu) + 13);
    LoopLast(base, frame, movie);
  }
  return ShowPictures(base, frame);
}

uint32_t Transition29(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222CFF0 LocationScreen::Transition29");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    mode_tick::ScreenFadesFadeIn(base, 30, 0, 0, 0, 0);
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 0, 0, 0, 0, 0);
    HidePictures(base, frame);
    guest::StoreU32(base, step, 1);
  }
  const uint32_t done = Fading(base) ? 0 : 1;
  Run(calls, frame, 0x8222D09C, movie);
  if (done != 0) {
    SetText(base, calls, frame, movie, 0, 0, 573);
    Required(ui_movies::SetTextScrollSpeed(base, movie, 0, 0, 300,
                                           double(guest::LoadFloat(base, 0x82000A30))),
             "sub_8211C4F0");
    Required(ui_movies::ScrollText(base, movie, 0, 0, 1), "UiMovie::ScrollText");
    SetCarColor(base, frame);
  }
  return done;
}

uint32_t ShowUnlockEntry(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222A978 LocationScreen::ShowUnlockEntry");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t list = save + 0x1C778;
  const uint32_t n = guest::LoadU32(base, Screen(base) + 868);
  if (n >= guest::LoadU32(base, list + 768)) {
    return save;
  }
  const uint32_t entry = n * 12 + list;
  const uint32_t e56 = data_tables::Entry56(base, guest::LoadU32(base, entry + 4),
                                            guest::LoadU32(base, entry + 8));
  guest::StoreU32(base, Screen(base) + 876, guest::LoadU32(base, entry + 4));
  guest::StoreU32(base, Screen(base) + 872, guest::LoadU32(base, entry + 8));
  SetText(base, calls, frame, movie, 4, 1, guest::LoadU32(base, e56 + 12) + 4650);
  SetText(base, calls, frame, movie, 4, 2, guest::LoadU32(base, entry + 8) + 6683);
  SetPictureTexture(base, movie, 6, 1, PictureA(base, guest::LoadU32(base, e56 + 16)));
  SetPictureTexture(base, movie, 6, 0, PictureB(base, guest::LoadU32(base, e56 + 16)));
  const uint32_t screen = Screen(base);
  guest::StoreU32(base, screen + 868, guest::LoadU32(base, screen + 868) + 1);
  if (Required(ui_movies::HasMoreText(base, movie, 4, 2), "sub_8211D8D8") != 0) {
    return ShowNode(base, movie, 4, 0);
  }
  return HideNode(base, movie, 4, 0);
}

uint32_t Transition25(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D1A8 LocationScreen::Transition25");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    ShowUnlockEntry(base, frame, calls);
    FinishNamed(calls, frame, 0x8222D1F8, movie, 5);
    PlayNamed(base, movie, 5);
    LoopLast(base, frame, movie);
    mode_tick::ScreenFadesFadeIn(base, 60, 0, 0, 0, 0);
    PlayNamed(base, movie, 95);
    HidePictures(base, frame);
    guest::StoreU32(base, step, 1);
  }
  if (Fading(base)) {
    return 0;
  }
  if (uint32_t(Run(calls, frame, 0x8222D298, movie)) != 0) {
    return 0;
  }
  const uint32_t pages = guest::LoadU32(base, kPages);
  const uint32_t page = guest::LoadU32(base, pages);
  if (int32_t(page) != 13) {
    guest::StoreU32(base, pages + 8, page);
  }
  guest::StoreU32(base, pages + 12, 0xFFFFFFFFu);
  guest::StoreU32(base, pages + 0, 13);
  guest::StoreU32(base, pages + 16, 0xFFFFFFFFu);
  return 1;
}

uint32_t ShowListEntry(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222B048 LocationScreen::ShowListEntry");
  const uint32_t frame = sp - 288;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t record = guest::LoadU32(base, Screen(base) + 852) * 100 + save + 0x1AD20;
  Copy(base, calls, frame, 0x8222B0A8, frame + 96, record, 100);
  uint32_t r3 = 0;
  for (uint32_t k = 0; k < 8; k++) {
    HideNode(base, movie, 10, 5 + k);
    HideNode(base, movie, 0xFFFFFFFFu, 27 + k);
    r3 = HideNode(base, movie, 0xFFFFFFFFu, 19 + k);
  }
  const uint32_t count = guest::LoadU32(base, frame + 192);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t table = guest::LoadU32(base, frame + 100 + 12 * i);
    const uint32_t index = guest::LoadU32(base, frame + 104 + 12 * i);
    const uint32_t e56 = data_tables::Entry56(base, table, index);
    const uint32_t flags = ((table * 192 + index + 27614) << 2) + save;
    const bool hidden = guest::LoadU32(base, flags) == 2;
    ShowNode(base, movie, 10, 5 + i);
    ShowNode(base, movie, 0xFFFFFFFFu, 27 + i);
    if (i == guest::LoadU32(base, guest::LoadU32(base, kListMenu))) {
      SetPictureTexture(base, movie, 6, 1, PictureA(base, guest::LoadU32(base, e56 + 16)));
      SetPictureTexture(base, movie, 6, 0, PictureB(base, guest::LoadU32(base, e56 + 16)));
    }
    if (hidden) {
      HideNode(base, movie, 0xFFFFFFFFu, 19 + i);
    } else {
      ShowNode(base, movie, 0xFFFFFFFFu, 19 + i);
    }
    r3 = SetText(base, calls, frame, movie, 10, 5 + i, guest::LoadU32(base, e56 + 12) + 4650);
  }
  return r3;
}

uint32_t Transition2(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D610 LocationScreen::Transition2");
  const uint32_t frame = sp - 160;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, frame + 96 + 4 * i, 573 + i);
  }
  if (guest::LoadU32(base, step) == 0) {
    FinishNamed(calls, frame, 0x8222D67C, movie, 24);
    FinishNamed(calls, frame, 0x8222D688, movie, 23);
    uint32_t menu = guest::LoadU32(base, kMenu);
    const int32_t cursor = int32_t(guest::LoadU32(base, menu));
    const int32_t previous = int32_t(guest::LoadU32(base, menu + 4));
    if (previous == 0 && cursor == 4) {
      SetCursor(base, frame, 160);
    } else if (cursor == 0 && previous == 4) {
      SetCursor(base, frame, 0);
    } else {
      const int32_t moved = int32_t(guest::LoadU32(base, menu + 8));
      if (moved == 1) {
        PlayNamed(base, movie, 24);
      } else if (moved == -1) {
        PlayNamed(base, movie, 23);
      }
    }
    menu = guest::LoadU32(base, kMenu);
    const uint32_t named = guest::LoadU32(base, menu) + 13;
    FinishNamed(calls, frame, 0x8222D724, movie, guest::LoadU32(base, menu + 4) + 13);
    menu = guest::LoadU32(base, kMenu);
    if (int32_t(guest::LoadU32(base, Screen(base) + 892)) == 1 &&
        int32_t(guest::LoadU32(base, menu)) == 2) {
      PlayNamed(base, movie, 37);
      FinishNamed(calls, frame, 0x8222D770, movie, 37);
      PlayNamed(base, movie, 38, 1, 1);
      FinishNamed(calls, frame, 0x8222D798, movie, 38);
      SetText(base, calls, frame, movie, 0, 0, 588);
    } else {
      const uint32_t text =
          guest::LoadU32(base, frame + 96 + ((guest::LoadU32(base, menu) << 2) & 0xFFFFFFFCu));
      SetText(base, calls, frame, movie, 0, 0, text);
      PlayNamed(base, movie, named);
      LoopLast(base, frame, movie);
    }
    Required(ui_movies::ResetTextScroll(base, movie, 0, 0), "sub_8211C5F8");
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8222D818, movie);
  return 1;
}

uint32_t Transition14(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222E6E0 LocationScreen::Transition14");
  const uint32_t frame = sp - 176;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    for (uint32_t i = 0; i < 3; i++) {
      guest::StoreU32(base, frame + 88 + 4 * i, 486 + i);
      guest::StoreU32(base, frame + 104 + 4 * i, 5 + i);
    }
    // The confirm menu reset: 3 items (+16 = 2, +20 = 1, the rest 0).
    const uint32_t menu = guest::LoadU32(base, kConfirmMenu);
    for (uint32_t offset : {12u, 0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
      guest::StoreU32(base, menu + offset, 0);
    }
    guest::StoreU32(base, menu + 16, 2);
    guest::StoreU32(base, menu + 20, 1);
    for (uint32_t i = 0; i < 3; i++) {
      SetText(base, calls, frame, movie, 9, guest::LoadU32(base, frame + 104 + 4 * i),
              guest::LoadU32(base, frame + 88 + 4 * i));
    }
    SetCursor(base, frame, 0);
    for (uint32_t i = 0; i < 5; i++) {
      FinishNamed(calls, frame, 0x8222E7D4, movie, 13 + i);
    }
    SetText(base, calls, frame, movie, 0, 0, 579);
    Required(ui_movies::ResetTextScroll(base, movie, 0, 0), "sub_8211C5F8");
    if (HasCard(base, calls, frame, 0x8222E808) == 0) {
      SetNodeColor(base, movie, 9, 6, 2);
    }
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 11, 0, 0, 0, 0);
    ShowPictures(base, frame);
    guest::StoreU32(base, step, 1);
  }
  if (HasCard(base, calls, frame, 0x8222E850) == 0) {
    SetNodeColor(base, movie, 9, 6, 2);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222E878, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    PlayNamed(base, movie, 13);
    LoopLast(base, frame, movie);
    if (HasCard(base, calls, frame, 0x8222E8AC) == 0) {
      SetNodeColor(base, movie, 9, 6, 2);
    }
  }
  return done;
}

uint32_t Transition16(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222E8D8 LocationScreen::Transition16");
  const uint32_t frame = sp - 160;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    FinishNamed(calls, frame, 0x8222E91C, movie, 23);
    FinishNamed(calls, frame, 0x8222E928, movie, 24);
    uint32_t menu = guest::LoadU32(base, kConfirmMenu);
    const int32_t cursor = int32_t(guest::LoadU32(base, menu));
    const int32_t previous = int32_t(guest::LoadU32(base, menu + 4));
    if (previous == 0 && cursor == 2) {
      SetCursor(base, frame, 80);
    } else if (cursor == 0 && previous == 2) {
      SetCursor(base, frame, 0);
    } else {
      const int32_t moved = int32_t(guest::LoadU32(base, menu + 8));
      if (moved == 1) {
        PlayNamed(base, movie, 24);
      } else if (moved == -1) {
        PlayNamed(base, movie, 23);
      }
    }
    menu = guest::LoadU32(base, kConfirmMenu);
    const uint32_t finished = guest::LoadU32(base, menu + 4) + 13;
    const uint32_t named = guest::LoadU32(base, menu) + 13;
    const uint32_t card = HasCard(base, calls, frame, 0x8222E9C0);
    menu = guest::LoadU32(base, kConfirmMenu);
    const uint32_t at = (guest::LoadU32(base, menu) << 2) & 0xFFFFFFFCu;
    guest::StoreU32(base, frame + 88, 579);
    uint32_t text;
    if (card != 0) {
      guest::StoreU32(base, frame + 92, 580);
      guest::StoreU32(base, frame + 96, 581);
      text = guest::LoadU32(base, frame + 88 + at);
      FinishNamed(calls, frame, 0x8222EA00, movie, finished);
      PlayNamed(base, movie, named);
      LoopLast(base, frame, movie);
    } else {
      guest::StoreU32(base, frame + 92, 591);
      guest::StoreU32(base, frame + 96, 581);
      text = guest::LoadU32(base, frame + 88 + at);
      FinishNamed(calls, frame, 0x8222EA40, movie, finished);
      if (int32_t(guest::LoadU32(base, guest::LoadU32(base, kConfirmMenu))) != 1) {
        PlayNamed(base, movie, named);
        LoopLast(base, frame, movie);
      }
      SetNodeColor(base, movie, 9, 6, 2);
    }
    SetText(base, calls, frame, movie, 0, 0, text);
    Required(ui_movies::ResetTextScroll(base, movie, 0, 0), "sub_8211C5F8");
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8222EAC0, movie);
  return 1;
}

}  // namespace location_screen
