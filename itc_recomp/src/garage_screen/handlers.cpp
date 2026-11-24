// The garage screen's machine handlers (docs/functions/garage-screen.md):
// see garage_screen.h.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace garage_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemCommit = 4096;
constexpr uint32_t kReadWrite = 4;
constexpr uint32_t kMemRelease = 0x8000;

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191C58 GarageScreen::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const Machine m(base, machine);
  m.set_loaded(0);
  const PageObject pages(base, uint32_t(calls.Call(kVirtualAlloc, frame, 0x82191C8C, 0,
                                                    PageObject::kSize, kMemCommit, kReadWrite)));
  G::set_pages(base, pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  const uint32_t list =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82191CC8, 0, 72, kMemCommit, kReadWrite));
  G::set_list(base, list);
  if (list != 0) {
    const MenuList l(base, list);
    l.set_enabled(0);
    l.set_player(0);
  }
  const uint32_t list2 =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82191CF4, 0, 72, kMemCommit, kReadWrite));
  G::set_list2(base, list2);
  if (list2 != 0) {
    const MenuList l(base, list2);
    l.set_enabled(0);
    l.set_player(0);
  }
  // A host object (E5 step 3) in place of the VirtualAlloc at 0x82191D20;
  // the global and r3 hold its handle.
  const ScreenData data = ScreenData::New(base);
  const uint64_t r3 = data.address;
  G::set_data(base, data);
  data.set_set_up(0);
  data.set_busy_shown(0);
  data.set_turning(0);
  data.set_doors(0);
  data.set_doors2(0);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191618 GarageScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82191D70 GarageScreen::WaitForLoad");
  const Machine m(base, uint32_t(r3));
  Frame(base, sp, 96);
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
  }
  if (m.loaded() != 0) {
    m.set_state(m.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t ReadSave(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82191BB0 GarageScreen::ReadSave");
  Frame(base, sp, 96);
  const SaveData save(base, vehicle_accessors::SaveDataOf(base, 0));
  const ScreenData data = Data(base);
  // The career's pick (20 bytes), the money, the weekday (signed) and the
  // day (at most 999).
  std::memcpy(data.pick(), guest::Pointer(base, save.pick_head_address()), 20);
  const int32_t day = int32_t(save.day());
  Data(base).set_money(save.money());
  Data(base).set_weekday(uint32_t(day - (day / 7) * 7));
  Data(base).set_day(uint32_t(day > 999 ? 999 : day));
  const uint32_t again = vehicle_accessors::SaveDataOf(base, 0);
  Data(base).set_save(again);
  return again;
}

uint64_t SetHeader(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192C80 GarageScreen::SetHeader");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  calls.Call(kResetPose, frame, 0x82192CB8, movie);
  // UiMovie::SetChildNameText on the host copy of the pick: its
  // SetNodeText (or nothing), as the original's.
  uint64_t args[8];
  if (ui_movies::ChildNameTextArgs(base, movie, 15, 6,
                                   ui_movies::NameRecordFromBytes(Data(base).pick()), args)) {
    calls.CallArgs(ui_movies::kSetNodeTextFunction, frame, 0x82192CD4, args);
  }
  calls.Call(kSetNumberText, frame, 0x82192CEC, movie, 15, 9, Data(base).money());
  calls.CallArgs(kSetText, frame, 0x82192D08, {movie, 15, 13, 8, Data(base).day(), 0, 0, 0});
  const uint32_t node = ui_movies::FindNode(base, movie, 15, 11);
  Node(base, node).set_texture_entry(Data(base).weekday());
  Data(base).set_stand_shown(0);
  Data(base).set_stand_model(0);
  Data(base).set_set_up(1);
  return node;
}

uint64_t Start(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193DC0 GarageScreen::Start");
  const uint32_t frame = Frame(base, sp, 96);
  ReadSave(base, frame);
  SetHeader(base, frame, calls);
  const PageObject pages = PagesOf(base);
  const uint32_t before = pages.page();
  if (before != 0) {
    pages.set_previous_page(before);
  }
  pages.set_step(0);
  pages.set_queued_step(1);
  pages.set_next_page(0);
  pages.set_page(kNone);
  return calls.Call(kFadeIn, frame, 0x82193E24, G::fade_in_argument(base), 0, 0, 0);
}

uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821927E8 GarageScreen::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8219281C, movie);
  const Machine m(base, machine);
  if (m.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x82192830, kMovie);
    m.set_loaded(0);
  }
  return r3;
}

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192850 GarageScreen::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Machine(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  calls.Call(kFinishAll, frame, 0x82192890, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x82192898, kMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821915B0 GarageScreen::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x821915D8, PagesOf(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x821915EC, List(base), 0, kMemRelease);
  // Deleted in place of the VirtualFree at 0x82191600 (r3 = 1, as it
  // returned).
  Data(base).Delete();
  const uint64_t r3 = 1;
  // The second list [0x826D8428] isn't freed (nor the lists' flags blocks),
  // as the original.
  G::set_pages(base, 0u);
  G::set_list(base, 0u);
  G::set_data(base, 0u);
  return r3;
}

}  // namespace garage_screen
