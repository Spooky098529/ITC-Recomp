// The spec sheet's and the build screen's machine handlers
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "build_screen/build_screen.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kNone = 0xFFFFFFFFu;
// The build screen's page handlers, in its UpdatePage's table (its frame's
// +80 on).
constexpr uint32_t kBuildPagesTable[5] = {0x8219EEC0, 0x8219F048, 0x8219F1C8, 0x8219B960,
                                          0x8219C3A8};

// A page object's first words as the Creates store them.
void ResetPages(uint8_t* base, uint32_t address) {
  const schema::PageObject pages(base, address);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
}

uint64_t Alloc(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t bytes) {
  return calls.Call(kVirtualAlloc, frame, lr, 0, bytes, 4096, 4);
}

}  // namespace

uint64_t SheetCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B8120 PowerGraphScreen::Create");
  const uint32_t frame = Frame(base, sp, 112);
  schema::SpecSheet(base, machine).set_loaded(0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x821B8154, 28));
  Globals::set_sheet_pages(base, pages);
  ResetPages(base, pages);
  const schema::FrontEndMenu menu = schema::FrontEndMenu(base, uint32_t(Alloc(calls, frame, 0x821B8190, 68)));
  Globals::set_sheet_menu(base, menu.address);
  menu.set_first_item(0);
  menu.set_wraps(0);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_last_item(7);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
  const uint64_t r3 = Alloc(calls, frame, 0x821B81EC, 1968);
  const schema::SpecSheetData data = schema::SpecSheetData(base, uint32_t(r3));
  Globals::set_sheet_data(base, data.address);
  data.set_movie_loaded(0);
  data.set_graph_drawn(0);
  data.set_next_state(21);
  schema::SpecSheet(base, machine).set_state(schema::SpecSheet(base, machine).reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t SheetOpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250328 PowerGraphScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kSheetMovie, 22550);
}

uint64_t SheetWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B8900 PowerGraphScreen::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  if (schema::SpecSheet(base, machine).loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kSheetMovie);
    if (uint32_t(r3) != 0) {
      schema::SpecSheet(base, machine).set_loaded(1);
    }
    return r3;
  }
  Globals::sheet_data(base).set_movie_loaded(1);
  Globals::sheet_data(base).set_graph_drawn(0);
  r3 = calls.Call(kMemset, frame, 0x821B896C, Globals::sheet_data(base).sheet_address(), 0, 1952);
  schema::SpecSheet(base, machine).set_state(9);
  return r3;
}

uint64_t SheetClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B8320 PowerGraphScreen::Close");
  const uint32_t frame = Frame(base, sp, 96);
  schema::SpecSheet(base, machine).set_state(7);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x821B8348, kSheetMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t SheetDestroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B80B8 PowerGraphScreen::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x821B80E0, Globals::sheet_pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x821B80F4, Globals::sheet_menu(base).address, 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x821B8108, Globals::sheet_data(base).address, 0, kMemRelease);
  Globals::set_sheet_pages(base, 0);
  Globals::set_sheet_menu(base, 0);
  Globals::set_sheet_data(base, 0);
  return r3;
}

uint64_t BuildCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219BD80 BuildScreen::Create");
  const uint32_t frame = Frame(base, sp, 112);
  schema::BuildScreen(base, machine).set_loaded(0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8219BDB4, 28));
  Globals::set_build_pages(base, pages);
  ResetPages(base, pages);
  const uint32_t list = uint32_t(Alloc(calls, frame, 0x8219BDF0, 72));
  Globals::set_build_list(base, list);
  if (list != 0) {
    Store(base, list + 68, 0);
    Store(base, list + 60, 0);
  }
  const uint32_t groups = uint32_t(Alloc(calls, frame, 0x8219BE1C, 72));
  Globals::set_build_group_list(base, groups);
  if (groups != 0) {
    Store(base, groups + 68, 0);
    Store(base, groups + 60, 0);
  }
  const uint64_t r3 = Alloc(calls, frame, 0x8219BE4C, schema::BuildScreenData::kSize);
  const uint32_t screen = uint32_t(r3);
  Globals::set_build_screen(base, screen);
  const schema::BuildScreenData data(base, screen);
  data.set_set_up(0);
  data.set_busy_shown(0);
  schema::BuildScreen(base, machine).set_state(schema::BuildScreen(base, machine).reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t BuildOpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B950 BuildScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kBuildMovie, 22557);
}

uint64_t BuildWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BE98 BuildScreen::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  if (schema::BuildScreen(base, machine).loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kBuildMovie);
    if (uint32_t(r3) != 0) {
      schema::BuildScreen(base, machine).set_loaded(1);
    }
  }
  if (schema::BuildScreen(base, machine).loaded() != 0) {
    schema::BuildScreen(base, machine).set_state(schema::BuildScreen(base, machine).reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t BuildUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C298 BuildScreen::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kBuildMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8219C2CC, movie);
  if (schema::BuildScreen(base, machine).delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8219C2E0, kBuildMovie);
    schema::BuildScreen(base, machine).set_loaded(0);
  }
  return r3;
}

uint64_t BuildClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C300 BuildScreen::Close");
  const uint32_t frame = Frame(base, sp, 112);
  schema::BuildScreen(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, kBuildMovie, frame + 80);
  calls.Call(kFinishAll, frame, 0x8219C340, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8219C348, kBuildMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t BuildDestroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B8C8 BuildScreen::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuListRelease, frame, 0x8219B8E0, Globals::build_list(base).address);
  calls.Call(kVirtualFree, frame, 0x8219B8FC, Globals::build_pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8219B90C, Globals::build_list(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8219B920, Globals::build_group_list(base).address, 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8219B934, Globals::build_screen(base).address, 0, kMemRelease);
  Globals::set_build_pages(base, 0);
  Globals::set_build_list(base, 0);
  Globals::set_build_screen(base, 0);
  return r3;
}

uint32_t BuildSaveBuildStates(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219C100 BuildScreen::SaveBuildStates");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  for (uint32_t i = 0; i < 400; i++) {
    const uint32_t build = Globals::build_screen(base).builds_address() + schema::BuildRecord::kSize * i;
    Store(base, save + 0x19A00 + 12 * i, schema::BuildRecord(base, build).saved_flags());
  }
  return save;
}

uint32_t BuildLeavePage(uint8_t* base, uint32_t mode, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219C3A8 BuildScreen::LeavePage");
  const uint32_t frame = Frame(base, sp, 96);
  build_screen::SetDark(base, frame, Load(base, Load(base, build_screen::kStandHolder)), 0);
  const uint32_t r3 = BuildSaveBuildStates(base, frame);
  Post(base, mode, 20);
  return r3;
}

uint64_t BuildDetailsPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B960 BuildScreen::DetailsPage");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kListUpdate, frame, 0x8219B97C, Globals::build_list(base).address);
  const uint32_t list = Globals::build_list(base).address;
  if (Load(base, list + 24) != 0 || Load(base, list + 4) != Load(base, list + 8)) {
    PagesGo(base, Globals::build_pages(base).address, 3, 19, kNone);
    return calls.Call(kMenuSoundPlay, frame, 0x8219BA38, 4);
  }
  uint64_t r3 = calls.Call(kListAccepted, frame, 0x8219B9A0, list);
  uint32_t sound = 0;
  if (uint32_t(r3) == 0) {
    r3 = calls.Call(kListCancelled, frame, 0x8219B9EC, Globals::build_list(base).address);
    if (uint32_t(r3) == 0) {
      return r3;
    }
    sound = 2;
  }
  r3 = calls.Call(kMenuSoundPlay, frame, 0x8219B9B0, sound);
  PagesGo(base, Globals::build_pages(base).address, kNone, 20, kNone);
  return r3;
}

uint64_t BuildUpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219FA80 BuildScreen::UpdatePage");
  const uint32_t frame = Frame(base, sp, 112);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, frame + 80 + 4 * i, kBuildPagesTable[i]);
  }
  const uint32_t level = Globals::build_pages(base).page();
  if (level != kNone) {
    r3 = calls.Call(Load(base, frame + 80 + (level << 2)), frame, 0x8219FAF4, mode);
  }
  return r3;
}

}  // namespace type2_screens
