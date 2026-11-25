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
#include "location_screen/internal.h"
#include "mode_tick/mode_tick.h"
#include "schema/career.h"
#include "schema/race_event.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

using namespace detail;

uint32_t ResetMovie(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222CD60 LocationScreen::ResetMovie");
  const uint32_t frame = sp - 192;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  // Node 9's five texts and children: the original's tables at the frame's
  // +96 and +128, read only here (values since E5 step 6).
  constexpr uint32_t kTexts[5] = {138, 139, 111, 140, 141};
  ui_movies::ResetPose(movie);
  calls.Call(kFinishAll, frame, 0x8222CDE8, movie);
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  SetText(calls, frame, movie, 0xFFFFFFFFu, 35,
          Screen().location_index() + 369);
  SetText(calls, frame, movie, 0xFFFFFFFFu, 36, schema::SaveData(save).mode() + 128);
  for (uint32_t i = 0; i < 5; i++) {
    SetText(calls, frame, movie, 9, 5 + i, kTexts[i]);
  }
  const schema::FrontEndMenu menu = G::menu();
  if (Screen().special_car() == 0 || int32_t(menu.cursor()) != 2) {
    PlayNamed(movie, menu.cursor() + 13);
    LoopLast(frame, movie);
  }
  return ShowPictures(frame);
}

uint32_t Transition29(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222CFF0 LocationScreen::Transition29");
  const uint32_t frame = sp - 112;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    mode_tick::ScreenFadesFadeIn(30, 0, 0, 0, 0);
    ui_movies::Play(movie, 0xFFFFFFFFu, 0, 0, 0, 0, 0);
    HidePictures(frame);
    guest::StoreU32(step, 1);
  }
  const uint32_t done = Fading() ? 0 : 1;
  Run(calls, frame, 0x8222D09C, movie);
  if (done != 0) {
    SetText(calls, frame, movie, 0, 0, 573);
    Required(ui_movies::SetTextScrollSpeed(movie, 0, 0, 300,
                                           double(guest::LoadFloat(0x82000A30))),
             "sub_8211C4F0");
    Required(ui_movies::ScrollText(movie, 0, 0, 1), "UiMovie::ScrollText");
    SetCarColor(frame);
  }
  return done;
}

uint32_t ShowUnlockEntry(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222A978 LocationScreen::ShowUnlockEntry");
  const uint32_t frame = sp - 128;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  const schema::SaveData save_data(save);
  const uint32_t list = save_data.unlocks_shown_address();
  const uint32_t n = Screen().unlock_position();
  if (n >= save_data.unlocks_shown_count()) {
    return save;
  }
  const uint32_t entry = n * 12 + list;
  const schema::CareerEntry56 e56(data_tables::Entry56(guest::LoadU32(entry + 4), guest::LoadU32(entry + 8)));
  Screen().set_entry_table(guest::LoadU32(entry + 4));
  Screen().set_entry_offset(guest::LoadU32(entry + 8));
  SetText(calls, frame, movie, 4, 1, e56.text() + 4650);
  SetText(calls, frame, movie, 4, 2, guest::LoadU32(entry + 8) + 6683);
  SetPictureTexture(movie, 6, 1, PictureA(e56.image()));
  SetPictureTexture(movie, 6, 0, PictureB(e56.image()));
  const schema::LocationScreen screen = Screen();
  screen.set_unlock_position(screen.unlock_position() + 1);
  if (Required(ui_movies::HasMoreText(movie, 4, 2), "sub_8211D8D8") != 0) {
    return ShowNode(movie, 4, 0);
  }
  return HideNode(movie, 4, 0);
}

uint32_t Transition25(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D1A8 LocationScreen::Transition25");
  // The original's 112-byte frame held no slots (E5 step 6): the calls are
  // made from `sp`, their frames below it.
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    ShowUnlockEntry(sp, calls);
    FinishNamed(calls, sp, 0x8222D1F8, movie, 5);
    PlayNamed(movie, 5);
    LoopLast(sp, movie);
    mode_tick::ScreenFadesFadeIn(60, 0, 0, 0, 0);
    PlayNamed(movie, 95);
    HidePictures(sp);
    guest::StoreU32(step, 1);
  }
  if (Fading()) {
    return 0;
  }
  if (uint32_t(Run(calls, sp, 0x8222D298, movie)) != 0) {
    return 0;
  }
  const schema::PageObject pages = G::pages();
  const uint32_t page = pages.page();
  if (int32_t(page) != 13) {
    pages.set_previous_page(page);
  }
  pages.set_step(0xFFFFFFFFu);
  pages.set_page(13);
  pages.set_queued_step(0xFFFFFFFFu);
  return 1;
}

uint32_t ShowListEntry(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222B048 LocationScreen::ShowListEntry");
  const uint32_t frame = sp - 288;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  const uint32_t record = Screen().location_index() * 100 + save + 0x1AD20;
  Copy(calls, frame, 0x8222B0A8, frame + 96, record, 100);
  uint32_t r3 = 0;
  for (uint32_t k = 0; k < 8; k++) {
    HideNode(movie, 10, 5 + k);
    HideNode(movie, 0xFFFFFFFFu, 27 + k);
    r3 = HideNode(movie, 0xFFFFFFFFu, 19 + k);
  }
  const uint32_t count = guest::LoadU32(frame + 192);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t table = guest::LoadU32(frame + 100 + 12 * i);
    const uint32_t index = guest::LoadU32(frame + 104 + 12 * i);
    const schema::CareerEntry56 e56(data_tables::Entry56(table, index));
    const uint32_t flags = ((table * 192 + index + 27614) << 2) + save;
    const bool hidden = guest::LoadU32(flags) == 2;
    ShowNode(movie, 10, 5 + i);
    ShowNode(movie, 0xFFFFFFFFu, 27 + i);
    if (i == G::list_menu().cursor()) {
      SetPictureTexture(movie, 6, 1, PictureA(e56.image()));
      SetPictureTexture(movie, 6, 0, PictureB(e56.image()));
    }
    if (hidden) {
      HideNode(movie, 0xFFFFFFFFu, 19 + i);
    } else {
      ShowNode(movie, 0xFFFFFFFFu, 19 + i);
    }
    r3 = SetText(calls, frame, movie, 10, 5 + i, e56.text() + 4650);
  }
  return r3;
}

uint32_t Transition2(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D610 LocationScreen::Transition2");
  const uint32_t frame = sp - 160;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  // The cursor's texts (573 to 577): the original's table at the frame's
  // +96, read only here (values since E5 step 6).
  constexpr uint32_t kTexts[5] = {573, 574, 575, 576, 577};
  if (guest::LoadU32(step) == 0) {
    FinishNamed(calls, frame, 0x8222D67C, movie, 24);
    FinishNamed(calls, frame, 0x8222D688, movie, 23);
    schema::FrontEndMenu menu = G::menu();
    const int32_t cursor = int32_t(menu.cursor());
    const int32_t previous = int32_t(menu.previous());
    if (previous == 0 && cursor == 4) {
      SetCursor(frame, 160);
    } else if (cursor == 0 && previous == 4) {
      SetCursor(frame, 0);
    } else {
      const int32_t moved = int32_t(menu.moved());
      if (moved == 1) {
        PlayNamed(movie, 24);
      } else if (moved == -1) {
        PlayNamed(movie, 23);
      }
    }
    menu = G::menu();
    const uint32_t named = menu.cursor() + 13;
    FinishNamed(calls, frame, 0x8222D724, movie, menu.previous() + 13);
    menu = G::menu();
    if (int32_t(Screen().special_car()) == 1 &&
        int32_t(menu.cursor()) == 2) {
      PlayNamed(movie, 37);
      FinishNamed(calls, frame, 0x8222D770, movie, 37);
      PlayNamed(movie, 38, 1, 1);
      FinishNamed(calls, frame, 0x8222D798, movie, 38);
      SetText(calls, frame, movie, 0, 0, 588);
    } else {
      // The menu's cursor is 0 to 4 (its last item); past the table the
      // original read the frame's word there (kept, never seen).
      const uint32_t cursor_now = menu.cursor();
      const uint32_t text =
          cursor_now < 5 ? kTexts[cursor_now]
                         : guest::LoadU32(frame + 96 + ((cursor_now << 2) & 0xFFFFFFFCu));
      SetText(calls, frame, movie, 0, 0, text);
      PlayNamed(movie, named);
      LoopLast(frame, movie);
    }
    Required(ui_movies::ResetTextScroll(movie, 0, 0), "sub_8211C5F8");
    guest::StoreU32(step, 1);
  }
  Run(calls, frame, 0x8222D818, movie);
  return 1;
}

uint32_t Transition14(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222E6E0 LocationScreen::Transition14");
  const uint32_t frame = sp - 176;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    // Node 9's three texts (486 to 488, children 5 to 7): the original's
    // tables at the frame's +88 and +104, read only here (values since E5
    // step 6).
    // The confirm menu reset: 3 items (+16 = 2, +20 = 1, the rest 0).
    const schema::FrontEndMenu menu = G::confirm_menu();
    menu.set_first_item(0);
    menu.set_cursor(0);
    menu.set_previous(0);
    menu.set_moved(0);
    menu.set_wrapped(0);
    menu.set_visible_row(0);
    menu.set_previous_row(0);
    menu.set_side(0);
    menu.set_player(0);
    menu.set_up_down_locked(0);
    menu.set_left_right_steps(0);
    menu.set_shoulders_step(0);
    menu.set_last_item(2);
    menu.set_wraps(1);
    for (uint32_t i = 0; i < 3; i++) {
      SetText(calls, frame, movie, 9, 5 + i, 486 + i);
    }
    SetCursor(frame, 0);
    for (uint32_t i = 0; i < 5; i++) {
      FinishNamed(calls, frame, 0x8222E7D4, movie, 13 + i);
    }
    SetText(calls, frame, movie, 0, 0, 579);
    Required(ui_movies::ResetTextScroll(movie, 0, 0), "sub_8211C5F8");
    if (HasCard(calls, frame, 0x8222E808) == 0) {
      SetNodeColor(movie, 9, 6, 2);
    }
    ui_movies::Play(movie, 0xFFFFFFFFu, 11, 0, 0, 0, 0);
    ShowPictures(frame);
    guest::StoreU32(step, 1);
  }
  if (HasCard(calls, frame, 0x8222E850) == 0) {
    SetNodeColor(movie, 9, 6, 2);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222E878, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    PlayNamed(movie, 13);
    LoopLast(frame, movie);
    if (HasCard(calls, frame, 0x8222E8AC) == 0) {
      SetNodeColor(movie, 9, 6, 2);
    }
  }
  return done;
}

uint32_t Transition16(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222E8D8 LocationScreen::Transition16");
  const uint32_t frame = sp - 160;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    FinishNamed(calls, frame, 0x8222E91C, movie, 23);
    FinishNamed(calls, frame, 0x8222E928, movie, 24);
    schema::FrontEndMenu menu = G::confirm_menu();
    const int32_t cursor = int32_t(menu.cursor());
    const int32_t previous = int32_t(menu.previous());
    if (previous == 0 && cursor == 2) {
      SetCursor(frame, 80);
    } else if (cursor == 0 && previous == 2) {
      SetCursor(frame, 0);
    } else {
      const int32_t moved = int32_t(menu.moved());
      if (moved == 1) {
        PlayNamed(movie, 24);
      } else if (moved == -1) {
        PlayNamed(movie, 23);
      }
    }
    menu = G::confirm_menu();
    const uint32_t finished = menu.previous() + 13;
    const uint32_t named = menu.cursor() + 13;
    const uint32_t card = HasCard(calls, frame, 0x8222E9C0);
    menu = G::confirm_menu();
    // The cursor's text: the original's three words at the frame's +88,
    // read only here (values since E5 step 6). The menu's cursor is 0 to 2
    // (its last item); past the table the original read the frame's word
    // there (kept, never seen).
    const uint32_t text_cursor = menu.cursor();
    const uint32_t at = (text_cursor << 2) & 0xFFFFFFFCu;
    auto text_at = [&](const uint32_t (&texts)[3]) {
      return text_cursor < 3 ? texts[text_cursor] : guest::LoadU32(frame + 88 + at);
    };
    uint32_t text;
    if (card != 0) {
      text = text_at({579, 580, 581});
      FinishNamed(calls, frame, 0x8222EA00, movie, finished);
      PlayNamed(movie, named);
      LoopLast(frame, movie);
    } else {
      text = text_at({579, 591, 581});
      FinishNamed(calls, frame, 0x8222EA40, movie, finished);
      if (int32_t(G::confirm_menu().cursor()) != 1) {
        PlayNamed(movie, named);
        LoopLast(frame, movie);
      }
      SetNodeColor(movie, 9, 6, 2);
    }
    SetText(calls, frame, movie, 0, 0, text);
    Required(ui_movies::ResetTextScroll(movie, 0, 0), "sub_8211C5F8");
    guest::StoreU32(step, 1);
  }
  Run(calls, frame, 0x8222EAC0, movie);
  return 1;
}

}  // namespace location_screen
