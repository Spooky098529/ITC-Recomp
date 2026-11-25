// The type 7 list screen's own slots and destructors (Type7ListScreen, over
// ColorListScreen; docs/functions/type67-screens.md): its item texts, title,
// details, marks and item infos, its menus' set-up, the back and race set-up
// it leaves through, and the destructors. See type67_screens.h.
// UiMovies::Find, UiMovie::FindNode, MenuList::Selection and SetSelection,
// MenuSound's Sound::Play, memcpy and the race set-up's functions
// (RaceData::ResetGlobal, Stages::Clear and AddRacer, Race::SetRoute,
// AddRacer, AddStage and PlaceRacers, RacerRecord::Init, Car::SetModel and
// Refresh, the race data's setters, SaveData::Of) through their plain C++,
// each with the frame the original's call gives it; a guest call from the
// original's frame with its return address where that plain C++ declines.
// MenuList::Init's plain C++ where it frees natively, else a guest call;
// UiMovie::SetText through Calls::set_text; UiMovie::ClearText, the sound
// manager's slot 13 and operator delete guest calls.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "schema/boot_mode.h"
#include "schema/menu_sounds.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "menu_sounds/menu_sounds.h"
#include "native/buffers.h"
#include "race_screens/race_screens.h"
#include "race_setup/race_setup.h"
#include "save_data/save_data.h"
#include "type67_screens/list_pages.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace type67_screens {

namespace {

using namespace pages;
using guest::LoadU32;
using guest::StoreU32;
using T7LS = schema::Type7ListScreen;

constexpr uint32_t kListInit = 0x8220BAC0, kClearText = 0x8211C470, kMemcpy = 0x825A7410;
// The item texts' string ids, the title's (3), the item records (28 bytes
// each: +8 the details' count, +12 on their indexes) and the details'
// string ids by index.
constexpr uint32_t kItemTexts = 0x82637054;
constexpr uint32_t kTitleTexts = 0x82637194;
constexpr uint32_t kItemRecords = 0x82637098;
constexpr uint32_t kDetailTexts = 0x82637078;
// The race set-up's guest fallbacks.
constexpr uint32_t kResetGlobal = 0x821021A8, kRaceSetRoute = 0x82101D38,
                   kCarRefresh = 0x820C4998, kAddRacer = 0x821017C0, kPlaceRacers = 0x82102048;
constexpr uint32_t kSetUpRaceFrame = 4064;

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  const uint64_t six[6] = {args[0], args[1], args[2], args[3], 0, 0};
  return static_cast<const Calls*>(user)->call_set(function, six, set, r1, lr);
}

// MenuList::Init on `list` with the ids at the frame's +84 and +92 written
// first (the original's stack arguments): its plain C++ where it frees the
// old block natively, else a guest call.
void InitList(const Calls& calls, uint32_t list, uint32_t frame, uint32_t lr,
              const save_data::MenuListArgs& a) {
  StoreU32(frame + 92, a.id6);
  StoreU32(frame + 84, a.id5);
  if (calls.native_heap && save_data::MenuListInitNative(list)) {
    save_data::MenuListInit(list, a);
    return;
  }
  const uint64_t args[8] = {list, a.count, a.shown, a.r6, a.r7, a.r8, a.r9, a.r10};
  calls.call8(kListInit, args, frame, lr);
}

uint64_t ClearText(const Calls& calls, uint32_t movie, uint32_t sub_scene, uint32_t child,
                   uint32_t frame, uint32_t lr) {
  const uint64_t args[6] = {movie, sub_scene, child, 0, 0, 0};
  return calls.call_set(kClearText, args, 3, frame, lr);
}

// Car::Refresh's original where Race::PlaceRacers' plain C++ hands it back.
struct Refresh {
  const Calls* calls;
  uint32_t r1;
};

void RefreshThrough(void* user, uint32_t car) {
  const Refresh& r = *static_cast<const Refresh*>(user);
  Call1(*r.calls, kCarRefresh, car, r.r1, 0x82101DC8);
}

}  // namespace

uint64_t Type7ListScreenSetItemText(uint64_t r3, uint64_t r4, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82243238 Type7ListScreen::SetItemText");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieOf(uint32_t(r3), frame);
  const uint32_t texts = (uint32_t(r4) << 2) + kItemTexts;
  uint64_t result = 0;
  for (uint32_t i = 0; i < 4; i++) {
    result = calls.set_text(frame, 0x82243290, movie, 21, 5 + i, LoadU32(texts + 4 * i));
  }
  return result;
}

uint64_t Type7ListScreenClearItemText(uint64_t r3, uint64_t r4, uint32_t sp,
                                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822432A8 Type7ListScreen::ClearItemText");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieOf(uint32_t(r3), frame);
  uint64_t result = 0;
  for (int32_t i = 0; i < 3; i++) {
    if (i == int32_t(uint32_t(r4))) continue;
    result = ClearText(calls, movie, 21, uint32_t(5 + i), frame, 0x822432F4);
  }
  return result;
}

uint64_t Type7ListScreenSetTitle(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822433D8 Type7ListScreen::SetTitle");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieOf(uint32_t(r3), frame);
  uint64_t result = 0;
  for (uint32_t i = 0; i < 3; i++) {
    result =
        calls.set_text(frame, 0x82243428, movie, 20, 5 + i, LoadU32(kTitleTexts + 4 * i));
  }
  return result;
}

uint64_t Type7ListScreenSetDetails(uint64_t r3, uint64_t r4, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82243308 Type7ListScreen::SetDetails");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 160);
  StoreU32(frame + 96, T7LS(screen).movie());
  const uint32_t movie =
      LoadU32(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 96));
  const schema::Type7ItemInfo record(uint32_t(r4 * 28) + kItemRecords);
  const uint32_t count = record.color_count();
  InitList(calls, screen + 10216, frame, 0x82243384,
           {count, 2, 1, 154, 153, 155, 156, 159, 158});
  uint64_t result = 0;
  for (int32_t i = 0; i < 2; i++) {
    if (i < int32_t(count)) {
      const uint32_t index = LoadU32(record.detail_a_address() + 4 * uint32_t(i));
      result = calls.set_text(frame, 0x822433B8, movie, 9, uint32_t(5 + i),
                              LoadU32((index << 2) + kDetailTexts));
    } else {
      result = ClearText(calls, movie, 9, uint32_t(5 + i), frame, 0x822433C0);
    }
  }
  return result;
}

uint64_t Type7ListScreenShowMarks(uint64_t r3, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82243448 Type7ListScreen::ShowMarks");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieOf(uint32_t(r3), frame);
  const uint32_t which = uint32_t(r4);
  uint32_t node = ui_movies::FindNode(movie, 0xFFFFFFFFu, 18);
  schema::UiMovieNode(node).set_visible(which != 0 ? 1 : 0);
  node = ui_movies::FindNode(movie, 0xFFFFFFFFu, 19);
  schema::UiMovieNode(node).set_visible(which + 3 - 9 != 0 ? 1 : 0);
  return node;
}

uint64_t Type7ListScreenItemInfo(uint64_t r4, uint64_t r5, uint32_t sp,
                                 uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82242FA0 Type7ListScreen::ItemInfo");
  const uint32_t item = uint32_t(r4);
  if (item > 8) return r5;
  // memcpy(r5, the item's record, 28), a tail call.
  const uint32_t from = uint32_t(r4 * 28) + kItemRecords;
  if (native::Buffers_Memcpy(uint32_t(r5), from, 28, lr)) {
    return r5;
  }
  const uint64_t args[6] = {r5, from, 28, 0, 0, 0};
  return calls.call_set(kMemcpy, args, 3, sp, lr);
}

uint64_t Type7ListScreenSetUpMenus(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82243588 Type7ListScreen::SetUpMenus");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 144);
  InitList(calls, screen + 10144, frame, 0x822435D0, {9, 3, 1, 154, 153, 155, 156, 159, 158});
  InitList(calls, screen + 10288, frame, 0x82243600, {2, 2, 1, 156, 155, 154, 153, 159, 158});
  race_screens::ListSetSelection(screen + 10288, 1);
  InitList(calls, screen + 10360, frame, 0x8224363C, {3, 3, 1, 154, 153, 155, 156, 159, 158});
  race_screens::ListSetSelection(screen + 10360, 0);
  InitList(calls, screen + 10216, frame, 0x82243674, {2, 2, 1, 154, 153, 155, 156, 159, 158});
  Type7ListScreenSetTitle(r3, frame, calls);
  const uint32_t selection = race_screens::ListSelection(screen + 10144);
  return ColorListScreenLoadItem(r3, selection, ~uint64_t(0), frame, calls);
}

uint64_t Type7ListScreenBack(uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822434C8 Type7ListScreen::Back");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t dispatcher = uint32_t(r4);
  T7LS(uint32_t(r3)).set_state(9);
  // r6 as the post leaves it (the ring's start plus its count before, 64-bit),
  // passed on to the slot below.
  const schema::ModeDispatcher d(dispatcher);
  const uint64_t r6 = uint64_t(d.ring_start()) + uint64_t(d.ring_count());
  Post(dispatcher, 5);
  // The music: bank 1's cue 0 stopped (the sound manager's slot 13), bank 1's
  // cue 2 played (Sound::Play), the indexes in the frame.
  StoreU32(frame + 80, 0);
  StoreU32(frame + 84, 1);
  const auto manager = schema::MenuSoundsGlobals::sound_manager();
  const uint64_t args[6] = {manager.address, frame + 84, frame + 80, r6, 0, 0};
  calls.call_set(manager.vtable().stop_cue(), args, 6, frame, 0x82243558);
  StoreU32(frame + 92, 1);
  StoreU32(frame + 88, 2);
  return menu_sounds::PlaySound(frame + 92, frame + 88, frame,
                                {const_cast<Calls*>(&calls), &SoundThunk});
}

uint64_t Type7ListScreenSetUpRace(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82243018 Type7ListScreen::SetUpRace");
  namespace rs = race_setup;
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, kSetUpRaceFrame);
  auto data = [] { return schema::RaceStatesGlobals::race_data().address; };
  if (!rs::RaceDataResetGlobal(frame - 112)) {
    Call1(calls, kResetGlobal, r3, frame, 0x8224302C);
  }
  rs::StagesClear(data() + rs::kStageList);
  vehicle_accessors::RaceDataSetWord32(race_screens::ListSelection(screen + 10360));
  schema::RaceStatesGlobals::race_data().set_kind(3);
  schema::RaceStatesGlobals::race_data().set_race_mode(6);
  // The route (the merged switch piece Type7ListScreen::AddRacers
  // sub_822430BC is its case 0): by the item list's +4 plus its selection,
  // 0 to 14 (else 0), the color list's selection plus 0, 2, 4, 8 or 10, or
  // 6, 7 and 12 to 19.
  const uint32_t index =
      T7LS(screen).course_list_scroll() + race_screens::ListSelection(screen + 10144);
  auto colors = [&] { return uint64_t(race_screens::ListSelection(screen + 10216)); };
  uint64_t route = 0;
  switch (index) {
    case 0:
      route = colors();
      break;
    case 1:
      route = colors() + 2;
      break;
    case 2:
      route = colors() + 4;
      break;
    case 3:
      route = 6;
      break;
    case 4:
      route = 7;
      break;
    case 5:
      route = colors() + 8;
      break;
    case 6:
      route = colors() + 10;
      break;
    default:
      if (index <= 14) route = index + 5;
      break;
  }
  if (!rs::RaceSetRoute(0, uint32_t(route), frame - rs::kRouteFrameBytes)) {
    const uint64_t args[6] = {0, route, 0, 0, 0, 0};
    calls.call_set(kRaceSetRoute, args, 2, frame, 0x82243164);
  }
  vehicle_accessors::RaceDataSetWord59618(race_screens::ListSelection(screen + 10288) == 0 ? 4 : 0);
  vehicle_accessors::RaceDataSetWord5961C(0);
  const uint32_t answer = uint32_t(Slot(T7LS(screen).list_part().address, 20,
                                        std::nullopt, frame, 0x82243198, calls));
  // The player's racer record at the frame's +80: the type 7 car.
  const uint32_t record = frame + 80;
  rs::RacerRecordInit(record);
  rs::SetModel(record, schema::Car(kType7Car).car_id());
  StoreU32(frame + 3972, answer);
  if (!native::Buffers_Memcpy(record, kType7Car, 1884, 0x822431CC)) {
    const uint64_t args[6] = {record, kType7Car, 1884, 0, 0, 0};
    calls.call_set(kMemcpy, args, 3, frame, 0x822431CC);
  }
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(record, &refreshed)) {
    Call1(calls, kCarRefresh, record, frame, 0x822431D4);
  }
  const double zero = double(std::bit_cast<float>(schema::MenuSoundsGlobals::zero()));
  if (rs::AddRacerNative(record)) {
    rs::AddRacer(record, 0, zero, 0);
  } else {
    const uint64_t args[4] = {record, 0, 0, 0};
    calls.call_f1(kAddRacer, args, zero, frame, 0x822431EC);
  }
  rs::StagesAddRacer(data() + rs::kStageList, 0, 2, -1);
  schema::SaveData(vehicle_accessors::SaveDataOf(0)).set_profile_option(answer);
  rs::AddStage(4);
  uint64_t result;
  if (rs::PlaceRacersNative(frame - 128)) {
    Refresh refresh{&calls, frame - 128 - 4096};
    result = rs::PlaceRacers(frame - 128, RefreshThrough, &refresh);
  } else {
    const uint64_t args[6] = {4, 0, 0, 0, 0, 0};
    result = calls.call_set(kPlaceRacers, args, 1, frame, 0x82243228);
  }
  T7LS(screen).set_state(10);
  return result;
}

uint64_t Type7ListScreenDestruct(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82242EC8 Type7ListScreen::~Type7ListScreen");
  const uint32_t frame = Frame(sp, 96);
  T7LS(uint32_t(r3)).set_vtable(kType7ListScreenVtable);
  ListPartDestruct(r3 + 11040, frame, calls);
  return ColorListScreenDestruct(r3, frame, calls);
}

uint64_t Type7ListScreenDeletingDestruct(uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82242FC8 Type7ListScreen::DeletingDestructor");
  const uint32_t frame = Frame(sp, 112);
  Type7ListScreenDestruct(r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, 0x82242FF8);
  }
  return r3;
}

}  // namespace type67_screens
