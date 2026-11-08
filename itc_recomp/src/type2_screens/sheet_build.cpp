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
void ResetPages(uint8_t* base, uint32_t pages) {
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 24, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
}

uint64_t Alloc(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t bytes) {
  return calls.Call(kVirtualAlloc, frame, lr, 0, bytes, 4096, 4);
}

}  // namespace

uint64_t SheetCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B8120 PowerGraphScreen::Create");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x821B8154, 28));
  Store(base, kSheetPages, pages);
  ResetPages(base, pages);
  const uint32_t menu = uint32_t(Alloc(calls, frame, 0x821B8190, 68));
  Store(base, kSheetMenu, menu);
  for (const uint32_t at : {12u, 20u, 0u, 4u}) {
    Store(base, menu + at, 0);
  }
  Store(base, menu + 16, 7);
  for (const uint32_t at : {8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + at, 0);
  }
  const uint64_t r3 = Alloc(calls, frame, 0x821B81EC, 1968);
  const uint32_t data = uint32_t(r3);
  Store(base, kSheetData, data);
  Store(base, data + 0, 0);
  Store(base, data + 4, 0);
  Store(base, data + 8, 21);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
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
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kSheetMovie);
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
    return r3;
  }
  Store(base, Load(base, kSheetData) + 0, 1);
  Store(base, Load(base, kSheetData) + 4, 0);
  r3 = calls.Call(kMemset, frame, 0x821B896C, Load(base, kSheetData) + 16, 0, 1952);
  Store(base, machine + kMachineState, 9);
  return r3;
}

uint64_t SheetClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B8320 PowerGraphScreen::Close");
  const uint32_t frame = Frame(base, sp, 96);
  Store(base, machine + kMachineState, 7);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x821B8348, kSheetMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t SheetDestroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B80B8 PowerGraphScreen::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x821B80E0, Load(base, kSheetPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x821B80F4, Load(base, kSheetMenu), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x821B8108, Load(base, kSheetData), 0, kMemRelease);
  Store(base, kSheetPages, 0);
  Store(base, kSheetMenu, 0);
  Store(base, kSheetData, 0);
  return r3;
}

uint64_t BuildCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219BD80 BuildScreen::Create");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8219BDB4, 28));
  Store(base, kBuildPages, pages);
  ResetPages(base, pages);
  const uint32_t list = uint32_t(Alloc(calls, frame, 0x8219BDF0, 72));
  Store(base, kBuildList, list);
  if (list != 0) {
    Store(base, list + 68, 0);
    Store(base, list + 60, 0);
  }
  const uint32_t groups = uint32_t(Alloc(calls, frame, 0x8219BE1C, 72));
  Store(base, kBuildGroupList, groups);
  if (groups != 0) {
    Store(base, groups + 68, 0);
    Store(base, groups + 60, 0);
  }
  const uint64_t r3 = Alloc(calls, frame, 0x8219BE4C, kBuildScreenBytes);
  const uint32_t screen = uint32_t(r3);
  Store(base, kBuildScreen, screen);
  Store(base, screen + 0, 0);
  Store(base, screen + 0x32668, 0);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
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
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kBuildMovie);
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t BuildUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C298 BuildScreen::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kBuildMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8219C2CC, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8219C2E0, kBuildMovie);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

uint64_t BuildClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C300 BuildScreen::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, kBuildMovie, frame + 80);
  calls.Call(kFinishAll, frame, 0x8219C340, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8219C348, kBuildMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t BuildDestroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219B8C8 BuildScreen::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kMenuListRelease, frame, 0x8219B8E0, Load(base, kBuildList));
  calls.Call(kVirtualFree, frame, 0x8219B8FC, Load(base, kBuildPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8219B90C, Load(base, kBuildList), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8219B920, Load(base, kBuildGroupList), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8219B934, Load(base, kBuildScreen), 0, kMemRelease);
  Store(base, kBuildPages, 0);
  Store(base, kBuildList, 0);
  Store(base, kBuildScreen, 0);
  return r3;
}

uint32_t BuildSaveBuildStates(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8219C100 BuildScreen::SaveBuildStates");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  for (uint32_t i = 0; i < 400; i++) {
    Store(base, save + 0x19A00 + 12 * i, Load(base, Load(base, kBuildScreen) + 52 * i + 0x2C7A8));
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
  calls.Call(kListUpdate, frame, 0x8219B97C, Load(base, kBuildList));
  const uint32_t list = Load(base, kBuildList);
  if (Load(base, list + 24) != 0 || Load(base, list + 4) != Load(base, list + 8)) {
    PagesGo(base, Load(base, kBuildPages), 3, 19, kNone);
    return calls.Call(kMenuSoundPlay, frame, 0x8219BA38, 4);
  }
  uint64_t r3 = calls.Call(kListAccepted, frame, 0x8219B9A0, list);
  uint32_t sound = 0;
  if (uint32_t(r3) == 0) {
    r3 = calls.Call(kListCancelled, frame, 0x8219B9EC, Load(base, kBuildList));
    if (uint32_t(r3) == 0) {
      return r3;
    }
    sound = 2;
  }
  r3 = calls.Call(kMenuSoundPlay, frame, 0x8219B9B0, sound);
  PagesGo(base, Load(base, kBuildPages), kNone, 20, kNone);
  return r3;
}

uint64_t BuildUpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219FA80 BuildScreen::UpdatePage");
  const uint32_t frame = Frame(base, sp, 112);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, frame + 80 + 4 * i, kBuildPagesTable[i]);
  }
  const uint32_t level = Load(base, Load(base, kBuildPages) + 0);
  if (level != kNone) {
    r3 = calls.Call(Load(base, frame + 80 + (level << 2)), frame, 0x8219FAF4, mode);
  }
  return r3;
}

}  // namespace type2_screens
