// The type 6 list screen's own slots and destructors (Type6ListScreen, over
// TitledListScreen; docs/functions/type67-screens.md): its item texts, marks
// and item infos, its menus' set-up, the back and race set-up it leaves
// through, and the destructors. See type67_screens.h. UiMovies::Find,
// UiMovie::FindNode, MenuList::Selection and SetSelection, MenuSound's
// Sound::Play and the race set-up's functions (RaceData::ResetGlobal,
// Stages::Clear, SetRoute and AddRacer, Race::SetRoute, AddRacer, AddStage
// and PlaceRacers, RacerRecord::Init, Car::SetModel and Refresh, memcpy, the
// race data's setters, SaveData::Of) through their plain C++, each with the
// frame the original's call gives it; a guest call from the original's frame
// with its return address where that plain C++ declines (a copy it wouldn't
// make natively). MenuList::Init's plain C++ where it frees natively, else a
// guest call; UiMovie::SetText through Calls::set_text; the sound manager's
// slot 13 and operator delete guest calls.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "schema/car_list.h"
#include "schema/boot_mode.h"
#include "schema/menu_sounds.h"
#include "schema/race_event.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "native/buffers.h"
#include "race_screens/race_screens.h"
#include "race_setup/race_setup.h"
#include "save_data/save_data.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace type67_screens {

namespace {

using guest::LoadU32;
using G = schema::Type67ScreensGlobals;
using guest::StoreU32;
using LS = schema::ListScreen;
using NS = schema::NodeScreen;
using PS = schema::PanelScreen;
using Pair = schema::GroupPair;
using T6LS = schema::Type6ListScreen;

constexpr uint32_t kListInit = 0x8220BAC0;
// The item texts' string ids (7 a row) and the item infos (16 bytes each).
constexpr uint32_t kItemTexts = 0x826366FC;
constexpr uint32_t kItemInfos = 0x82636720;
// The race set-up's guest fallbacks.
constexpr uint32_t kResetGlobal = 0x821021A8, kStagesSetRoute = 0x82101E90,
                   kRaceSetRoute = 0x82101D38, kMemcpy = 0x825A7410, kCarRefresh = 0x820C4998,
                   kAddRacer = 0x821017C0, kPlaceRacers = 0x82102048;
constexpr uint32_t kSetUpRaceFrame = 4080;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  StoreU32(sp - bytes, sp);
  return sp - bytes;
}

// The screen's movie: [+9680] at the frame's +80, then UiMovies::Find.
uint32_t MovieOf(uint32_t screen, uint32_t frame) {
  StoreU32(frame + 80, LS(screen).movie());
  return LoadU32(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80));
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  const uint64_t six[6] = {args[0], args[1], args[2], args[3], 0, 0};
  return static_cast<const Calls*>(user)->call_set(function, six, set, r1, lr);
}

// A guest call with r3 only (r4 and the others as the last call left them).
uint64_t Call1(const Calls& calls, uint32_t function, uint64_t r3, uint32_t frame, uint32_t lr) {
  const uint64_t args[6] = {r3, 0, 0, 0, 0, 0};
  return calls.call_set(function, args, 1, frame, lr);
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

uint64_t Type6ListScreenSetItemTexts(uint64_t r3, uint64_t r4, uint32_t sp,
                                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822336C0 Type6ListScreen::SetItemTexts");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieOf(uint32_t(r3), frame);
  const uint32_t texts = (uint32_t(r4) << 2) + kItemTexts;
  uint64_t result = 0;
  for (uint32_t i = 0; i < 7; i++) {
    result = calls.set_text(frame, 0x82233718, movie, 8, 5 + i, LoadU32(texts + 4 * i));
  }
  return result;
}

uint64_t Type6ListScreenShowMarks(uint64_t r3, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82233730 Type6ListScreen::ShowMarks");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieOf(uint32_t(r3), frame);
  const uint32_t which = uint32_t(r4);
  uint32_t node = ui_movies::FindNode(movie, 0xFFFFFFFFu, 17);
  schema::UiMovieNode(node).set_visible(which != 0 ? 1 : 0);
  node = ui_movies::FindNode(movie, 0xFFFFFFFFu, 18);
  schema::UiMovieNode(node).set_visible(which + 6 - 8 != 0 ? 1 : 0);
  return node;
}

uint64_t Type6ListScreenItemInfo(uint64_t r3, uint64_t r4, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82237008 Type6ListScreen::ItemInfo");
  const uint32_t item = uint32_t(r4);
  if (item > 7) return r3;
  const uint32_t from = (item << 4) + kItemInfos;
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreU32(info + i, LoadU32(from + i));
  }
  return r3;
}

uint64_t NodeScreenItemInfo(uint64_t r3, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82259998 NodeScreen::ItemInfo");
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreU32(info + i, 0xFFFFFFFFu);
  }
  return r3;
}

uint64_t Type6ListScreenSetUpMenus(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82233378 Type6ListScreen::SetUpMenus");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 144);
  InitList(calls, screen + 10216, frame, 0x822333BC, {8, 6, 1, 154, 153, 155, 156, 159, 158});
  InitList(calls, screen + 10288, frame, 0x822333EC, {2, 2, 1, 156, 155, 154, 153, 159, 158});
  race_screens::ListSetSelection(screen + 10288, 1);
  InitList(calls, screen + 10144, frame, 0x82233428, {3, 3, 1, 162, 161, 155, 156, 159, 158});
  race_screens::ListSetSelection(screen + 10144, 0);
  Slot(r3, 8, 0, frame, 0x8223344C, calls);
  calls.set_r4(0);
  return NodeListScreenLoadItem(r3, 0, frame, calls);
}

uint64_t Type6ListScreenBack(uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822337B0 Type6ListScreen::Back");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t dispatcher = uint32_t(r4);
  T6LS(uint32_t(r3)).set_state(9);
  // r6 as the post leaves it (the ring's start plus its count before, 64-bit),
  // passed on to the slot below.
  const schema::ModeDispatcher d(dispatcher);
  const uint64_t r6 = uint64_t(d.ring_start()) + uint64_t(d.ring_count());
  Post(dispatcher, 6);
  // The music: bank 1's cue 0 stopped (the sound manager's slot 13), bank 1's
  // cue 2 played (Sound::Play), the indexes in the frame.
  StoreU32(frame + 80, 0);
  StoreU32(frame + 84, 1);
  const auto manager = schema::MenuSoundsGlobals::sound_manager();
  const uint64_t args[6] = {manager.address, frame + 84, frame + 80, r6, 0, 0};
  calls.call_set(manager.vtable().stop_cue(), args, 6, frame, 0x82233840);
  StoreU32(frame + 92, 1);
  StoreU32(frame + 88, 2);
  return menu_sounds::PlaySound(frame + 92, frame + 88, frame,
                                {const_cast<Calls*>(&calls), &SoundThunk});
}

uint64_t Type6ListScreenSetUpRace(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822334C8 Type6ListScreen::SetUpRace");
  namespace rs = race_setup;
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, kSetUpRaceFrame);
  auto data = [] { return schema::RaceStatesGlobals::race_data().address; };
  if (!rs::RaceDataResetGlobal(frame - 112)) {
    Call1(calls, kResetGlobal, r3, frame, 0x822334DC);
  }
  rs::StagesClear(data() + rs::kStageList);
  vehicle_accessors::RaceDataSetWord32(race_screens::ListSelection(screen + 10144));
  schema::RaceStatesGlobals::race_data().set_kind(2);
  schema::RaceStatesGlobals::race_data().set_race_mode(4);
  // The route: the item list's +4 plus its selection, 0 to 7 (the switch's
  // cases each answer their own index), else the frame word the original
  // reads unset (its +80).
  const uint32_t index =
      T6LS(screen).course_list_scroll() + race_screens::ListSelection(screen + 10216);
  const uint32_t route = index <= 7 ? index : LoadU32(frame + 80);
  if (!rs::StagesSetRoute(data() + rs::kStageList, route, 48, 0xFFFFFFFFu,
                          frame - rs::kRouteFrameBytes)) {
    const uint64_t args[6] = {data() + rs::kStageList, route, 48, ~uint64_t(0), 0, 0};
    calls.call_set(kStagesSetRoute, args, 4, frame, 0x822335C0);
  }
  if (!rs::RaceSetRoute(route, 48, frame - rs::kRouteFrameBytes)) {
    const uint64_t args[6] = {route, 48, 0, 0, 0, 0};
    calls.call_set(kRaceSetRoute, args, 2, frame, 0x822335CC);
  }
  if (race_screens::ListSelection(screen + 10288) == 0) {
    const int32_t kind = int32_t(race_screens::ListSelection(screen + 10144));
    vehicle_accessors::RaceDataSetWord59618(kind == 1 ? 2 : kind == 2 ? 1 : 3);
  }
  const uint32_t answer = uint32_t(Slot(T6LS(screen).list_part().address, 20,
                                        std::nullopt, frame, 0x82233620, calls));
  // The player's racer record at the frame's +96: the type 6 car.
  const uint32_t record = frame + 96;
  rs::RacerRecordInit(record);
  rs::SetModel(record, schema::Car(type67_modes::kType6Car).car_id());
  StoreU32(frame + 3988, answer);
  if (!native::Buffers_Memcpy(record, type67_modes::kType6Car, 1884, 0x82233654)) {
    const uint64_t args[6] = {record, type67_modes::kType6Car, 1884, 0, 0, 0};
    calls.call_set(kMemcpy, args, 3, frame, 0x82233654);
  }
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(record, &refreshed)) {
    Call1(calls, kCarRefresh, record, frame, 0x8223365C);
  }
  const double zero = double(std::bit_cast<float>(schema::MenuSoundsGlobals::zero()));
  if (rs::AddRacerNative(record)) {
    rs::AddRacer(record, 0, zero, 0);
  } else {
    const uint64_t args[4] = {record, 0, 0, 0};
    calls.call_f1(kAddRacer, args, zero, frame, 0x82233674);
  }
  rs::StagesAddRacer(data() + rs::kStageList, 0, 2, -1);
  schema::SaveData(vehicle_accessors::SaveDataOf(0)).set_profile_option(answer);
  rs::AddStage(1);
  uint64_t result;
  if (rs::PlaceRacersNative(frame - 128)) {
    Refresh refresh{&calls, frame - 128 - 4096};
    result = rs::PlaceRacers(frame - 128, RefreshThrough, &refresh);
  } else {
    const uint64_t args[6] = {1, 0, 0, 0, 0, 0};
    result = calls.call_set(kPlaceRacers, args, 1, frame, 0x822336B0);
  }
  T6LS(screen).set_state(10);
  return result;
}

uint64_t Type6ListScreenDestruct(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822332D8 Type6ListScreen::~Type6ListScreen");
  const uint32_t frame = Frame(sp, 96);
  T6LS(uint32_t(r3)).set_vtable(kType6ListScreenVtable);
  ListPartDestruct(r3 + 10824, frame, calls);
  return TitledListScreenDestruct(r3, frame, calls);
}

uint64_t Type6ListScreenDeletingDestruct(uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82233478 Type6ListScreen::DeletingDestructor");
  const uint32_t frame = Frame(sp, 112);
  Type6ListScreenDestruct(r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, 0x822334A8);
  }
  return r3;
}

// --- The panel and node screens' leaves ---

uint64_t PanelScreenSetAnswer(uint64_t r3, uint32_t answer) {
  switch (answer) {
    case 0:
      ITC_NATIVE_RUNNING("sub_822609E0 PanelScreen::OnYes");
      break;
    case 1:
      ITC_NATIVE_RUNNING("sub_822609D0 PanelScreen::OnNo");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_822306D0 PanelScreen::OnBack");
      break;
  }
  PS(uint32_t(r3)).set_dialog_answer(answer);
  return r3;
}

uint64_t PanelScreenOnDialogClosed(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822306E0 PanelScreen::OnDialogClosed");
  Frame(sp, 96);
  const uint32_t screen = uint32_t(r3);
  uint32_t dialog = 0;
  uint64_t result = r3;
  if (PS(screen).dialog_answer() == 0) {
    // ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0): its plain C++, r4 left
    // 0 as the original sets it; FadeOut's r3 (the passes).
    const uint32_t passes = G::fade_out_passes();
    mode_tick::ScreenFadesFadeOut(passes, 0, 0, 0, 0);
    calls.set_r4(0);
    result = passes;
    dialog = 2;
  }
  PS(screen).set_quit_dialog(dialog);
  return result;
}

uint64_t PanelScreenSuspend(uint64_t r3, uint32_t sp, uint32_t lr,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248DE0 PanelScreen::Suspend");
  return Slot(PS(uint32_t(r3)).group_pair().address, 28, std::nullopt, sp, lr, calls);
}

uint64_t PanelScreenOpenEmpty(uint64_t r3, uint32_t sp, uint32_t lr,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822404D8 PanelScreen::OpenEmpty");
  const uint32_t screen = uint32_t(r3);
  if (PS(screen).movie_loaded() == 0) {
    PS(screen).set_state(1);
    return r3;
  }
  const uint32_t list = PS(screen).car_list().address;
  PS(screen).set_state(4);
  return Slot(list, 52, 0, sp, lr, calls);
}

uint64_t Type6CarWord(uint32_t offset) {
  switch (offset) {
    case 1972:
      ITC_NATIVE_RUNNING("sub_822307D0 Type6PanelScreen::CarList");
      break;
    case 1976:
      ITC_NATIVE_RUNNING("sub_822307E0 Type6PanelScreen::CarEntry");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_822307F0 Type6PanelScreen::CarSlot");
      break;
  }
  return LoadU32(type67_modes::kType6Car + offset);
}

uint64_t GroupPairDeletingDestruct(uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224ACE0 GroupPair::DeletingDestructor");
  const uint32_t frame = Frame(sp, 96);
  Pair(uint32_t(r3)).set_vtable(kGroupPairBaseVtable);
  if ((flags & 1) != 0) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x8224AD0C);
  }
  return r3;
}

uint64_t NodeScreenOpen(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82259318 NodeScreen::Open");
  NS(uint32_t(r3)).set_state(4);
  return r3;
}

}  // namespace type67_screens
