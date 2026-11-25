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
void ResetPages(uint32_t address) {
  const schema::PageObject pages(address);
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

uint64_t SheetCreate(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B8120 PowerGraphScreen::Create");
  const uint32_t frame = Frame(sp, 112);
  schema::SpecSheet(machine).set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x821B8154
  // and the VirtualAlloc(68) at 0x821B8190; the globals hold their handles.
  const uint32_t pages = schema::PageObject::New().address;
  Globals::set_sheet_pages(pages);
  ResetPages(pages);
  const schema::FrontEndMenu menu = schema::FrontEndMenu::New();
  Globals::set_sheet_menu(menu.address);
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
  // A host object (E5 step 4) in place of the VirtualAlloc(1968) at
  // 0x821B81EC; [0x826D846C] holds its handle (and r3, as the original's).
  const schema::SpecSheetData data = schema::SpecSheetData::New();
  const uint64_t r3 = data.address;
  Globals::set_sheet_data(data.address);
  data.set_movie_loaded(0);
  data.set_graph_drawn(0);
  data.set_next_state(21);
  schema::SpecSheet(machine).set_state(schema::SpecSheet(machine).reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t SheetOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250328 PowerGraphScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kSheetMovie, 22550);
}

uint64_t SheetWaitForLoad(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B8900 PowerGraphScreen::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(sp, 96);
  if (schema::SpecSheet(machine).loaded() == 0) {
    r3 = ui_movies::IsLoaded(kSheetMovie);
    if (uint32_t(r3) != 0) {
      schema::SpecSheet(machine).set_loaded(1);
    }
    return r3;
  }
  Globals::sheet_data().set_movie_loaded(1);
  Globals::sheet_data().set_graph_drawn(0);
  r3 = calls.Call(kMemset, frame, 0x821B896C, Globals::sheet_data().sheet_address(), 0, 1952);
  schema::SpecSheet(machine).set_state(9);
  return r3;
}

uint64_t SheetClose(uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B8320 PowerGraphScreen::Close");
  const uint32_t frame = Frame(sp, 96);
  schema::SpecSheet(machine).set_state(7);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x821B8348, kSheetMovie);
  Post(mode, 4);
  return r3;
}

uint64_t SheetDestroy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B80B8 PowerGraphScreen::Destroy");
  const uint32_t frame = Frame(sp, 112);
  calls.Call(kVirtualFree, frame, 0x821B80E0, Globals::sheet_pages().address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x821B80F4, Globals::sheet_menu().address, 0, kMemRelease);
  // Deleted in place of the VirtualFree at 0x821B8108 (r3 = 1, as it
  // returned; with no data, the VirtualFree of 0 as the original).
  const schema::SpecSheetData data = Globals::sheet_data();
  const uint64_t r3 =
      data.address != 0 ? 1 : calls.Call(kVirtualFree, frame, 0x821B8108, 0, 0, kMemRelease);
  data.Delete();
  Globals::set_sheet_pages(0);
  Globals::set_sheet_menu(0);
  Globals::set_sheet_data(0);
  return r3;
}

uint64_t BuildCreate(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219BD80 BuildScreen::Create");
  const uint32_t frame = Frame(sp, 112);
  schema::BuildScreen(machine).set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x8219BDB4
  // and the VirtualAlloc(72)s at 0x8219BDF0 and 0x8219BE1C; the globals hold
  // their handles.
  const uint32_t pages = schema::PageObject::New().address;
  Globals::set_build_pages(pages);
  ResetPages(pages);
  const uint32_t list = schema::MenuList::New().address;
  Globals::set_build_list(list);
  if (list != 0) {
    MenuList(list).set_enabled(0u);
    MenuList(list).set_player(0);
  }
  const uint32_t groups = schema::MenuList::New().address;
  Globals::set_build_group_list(groups);
  if (groups != 0) {
    MenuList(groups).set_enabled(0u);
    MenuList(groups).set_player(0);
  }
  const uint64_t r3 = Alloc(calls, frame, 0x8219BE4C, schema::BuildScreenData::kSize);
  const uint32_t screen = uint32_t(r3);
  Globals::set_build_screen(screen);
  const schema::BuildScreenData data(screen);
  data.set_set_up(0);
  data.set_busy_shown(0);
  schema::BuildScreen(machine).set_state(schema::BuildScreen(machine).reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t BuildOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B950 BuildScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kBuildMovie, 22557);
}

uint64_t BuildWaitForLoad(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219BE98 BuildScreen::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  Frame(sp, 96);
  if (schema::BuildScreen(machine).loaded() == 0) {
    r3 = ui_movies::IsLoaded(kBuildMovie);
    if (uint32_t(r3) != 0) {
      schema::BuildScreen(machine).set_loaded(1);
    }
  }
  if (schema::BuildScreen(machine).loaded() != 0) {
    schema::BuildScreen(machine).set_state(schema::BuildScreen(machine).reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t BuildUnload(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C298 BuildScreen::Unload");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kBuildMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8219C2CC, movie);
  if (schema::BuildScreen(machine).delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8219C2E0, kBuildMovie);
    schema::BuildScreen(machine).set_loaded(0);
  }
  return r3;
}

uint64_t BuildClose(uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C300 BuildScreen::Close");
  const uint32_t frame = Frame(sp, 112);
  schema::BuildScreen(machine).set_state(7);
  const uint32_t movie = FindMovie(kBuildMovie, frame + 80);
  calls.Call(kFinishAll, frame, 0x8219C340, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8219C348, kBuildMovie);
  Post(mode, 4);
  return r3;
}

uint64_t BuildDestroy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B8C8 BuildScreen::Destroy");
  const uint32_t frame = Frame(sp, 112);
  calls.Call(kMenuListRelease, frame, 0x8219B8E0, Globals::build_list().address);
  calls.Call(kVirtualFree, frame, 0x8219B8FC, Globals::build_pages().address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8219B90C, Globals::build_list().address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8219B920, Globals::build_group_list().address, 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8219B934, Globals::build_screen().address, 0, kMemRelease);
  Globals::set_build_pages(0);
  Globals::set_build_list(0);
  Globals::set_build_screen(0);
  return r3;
}

uint32_t BuildSaveBuildStates(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219C100 BuildScreen::SaveBuildStates");
  Frame(sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  for (uint32_t i = 0; i < 400; i++) {
    const uint32_t build = Globals::build_screen().builds_address() + schema::BuildRecord::kSize * i;
    Store(SaveData(save).item_record_word_address() + 12 * i,
          schema::BuildRecord(build).saved_flags());
  }
  return save;
}

uint32_t BuildLeavePage(uint32_t mode, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219C3A8 BuildScreen::LeavePage");
  const uint32_t frame = Frame(sp, 96);
  build_screen::SetDark(frame, schema::StandHolder(Load(build_screen::kStandHolder)).stage_block_0(), 0);
  const uint32_t r3 = BuildSaveBuildStates(frame);
  Post(mode, 20);
  return r3;
}

uint64_t BuildDetailsPage(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B960 BuildScreen::DetailsPage");
  const uint32_t frame = Frame(sp, 96);
  calls.Call(kListUpdate, frame, 0x8219B97C, Globals::build_list().address);
  const uint32_t list = Globals::build_list().address;
  if (MenuList(list).move() != 0 || MenuList(list).scroll() != MenuList(list).scroll_before()) {
    PagesGo(Globals::build_pages().address, 3, 19, kNone);
    return calls.Call(kMenuSoundPlay, frame, 0x8219BA38, 4);
  }
  uint64_t r3 = calls.Call(kListAccepted, frame, 0x8219B9A0, list);
  uint32_t sound = 0;
  if (uint32_t(r3) == 0) {
    r3 = calls.Call(kListCancelled, frame, 0x8219B9EC, Globals::build_list().address);
    if (uint32_t(r3) == 0) {
      return r3;
    }
    sound = 2;
  }
  r3 = calls.Call(kMenuSoundPlay, frame, 0x8219B9B0, sound);
  PagesGo(Globals::build_pages().address, kNone, 20, kNone);
  return r3;
}

uint64_t BuildUpdatePage(uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219FA80 BuildScreen::UpdatePage");
  const uint32_t frame = Frame(sp, 112);
  for (uint32_t i = 0; i < 5; i++) {
    Store(frame + 80 + 4 * i, kBuildPagesTable[i]);
  }
  const uint32_t level = Globals::build_pages().page();
  if (level != kNone) {
    r3 = calls.Call(Load(frame + 80 + (level << 2)), frame, 0x8219FAF4, mode);
  }
  return r3;
}

}  // namespace type2_screens
