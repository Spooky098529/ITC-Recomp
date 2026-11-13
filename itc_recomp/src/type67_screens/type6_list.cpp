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
using guest::StoreU32;

constexpr uint32_t kListInit = 0x8220BAC0;
// The passes word ScreenFades::FadeOut is given.
constexpr uint32_t kFadeOutPasses = 0x82001BF0;
// The item texts' string ids (7 a row) and the item infos (16 bytes each).
constexpr uint32_t kItemTexts = 0x826366FC;
constexpr uint32_t kItemInfos = 0x82636720;
// The race set-up's guest fallbacks and the float 0.0 Race::AddRacer gets.
constexpr uint32_t kResetGlobal = 0x821021A8, kStagesSetRoute = 0x82101E90,
                   kRaceSetRoute = 0x82101D38, kMemcpy = 0x825A7410, kCarRefresh = 0x820C4998,
                   kAddRacer = 0x821017C0, kPlaceRacers = 0x82102048;
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kSetUpRaceFrame = 4080;
// The race data's words SetUpRace sets.
constexpr uint32_t kRaceWordA = 0xA3AF0, kRaceWordB = 0xA3AF4;
// Player 0's save data word the list part's answer goes to.
constexpr uint32_t kSaveAnswer = 0x1F870;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The screen's movie: [+9680] at the frame's +80, then UiMovies::Find.
uint32_t MovieOf(uint8_t* base, uint32_t screen, uint32_t frame) {
  StoreU32(base, frame + 80, LoadU32(base, screen + 9680));
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
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
void InitList(uint8_t* base, const Calls& calls, uint32_t list, uint32_t frame, uint32_t lr,
              const save_data::MenuListArgs& a) {
  StoreU32(base, frame + 92, a.id6);
  StoreU32(base, frame + 84, a.id5);
  if (calls.native_heap && save_data::MenuListInitNative(base, list)) {
    save_data::MenuListInit(base, list, a);
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

uint64_t Type6ListScreenSetItemTexts(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822336C0 Type6ListScreen::SetItemTexts");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, uint32_t(r3), frame);
  const uint32_t texts = (uint32_t(r4) << 2) + kItemTexts;
  uint64_t result = 0;
  for (uint32_t i = 0; i < 7; i++) {
    result = calls.set_text(frame, 0x82233718, movie, 8, 5 + i, LoadU32(base, texts + 4 * i));
  }
  return result;
}

uint64_t Type6ListScreenShowMarks(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82233730 Type6ListScreen::ShowMarks");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieOf(base, uint32_t(r3), frame);
  const uint32_t which = uint32_t(r4);
  uint32_t node = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 17);
  StoreU32(base, node + 80, which != 0 ? 1 : 0);
  node = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 18);
  StoreU32(base, node + 80, which + 6 - 8 != 0 ? 1 : 0);
  return node;
}

uint64_t Type6ListScreenItemInfo(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82237008 Type6ListScreen::ItemInfo");
  const uint32_t item = uint32_t(r4);
  if (item > 7) return r3;
  const uint32_t from = (item << 4) + kItemInfos;
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreU32(base, info + i, LoadU32(base, from + i));
  }
  return r3;
}

uint64_t NodeScreenItemInfo(uint8_t* base, uint64_t r3, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_82259998 NodeScreen::ItemInfo");
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreU32(base, info + i, 0xFFFFFFFFu);
  }
  return r3;
}

uint64_t Type6ListScreenSetUpMenus(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82233378 Type6ListScreen::SetUpMenus");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 144);
  InitList(base, calls, screen + 10216, frame, 0x822333BC, {8, 6, 1, 154, 153, 155, 156, 159, 158});
  InitList(base, calls, screen + 10288, frame, 0x822333EC, {2, 2, 1, 156, 155, 154, 153, 159, 158});
  race_screens::ListSetSelection(base, screen + 10288, 1);
  InitList(base, calls, screen + 10144, frame, 0x82233428, {3, 3, 1, 162, 161, 155, 156, 159, 158});
  race_screens::ListSetSelection(base, screen + 10144, 0);
  Slot(base, r3, 8, 0, frame, 0x8223344C, calls);
  calls.set_r4(0);
  return NodeListScreenLoadItem(base, r3, 0, frame, calls);
}

uint64_t Type6ListScreenBack(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822337B0 Type6ListScreen::Back");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t dispatcher = uint32_t(r4);
  StoreU32(base, uint32_t(r3) + 9608, 9);
  // r6 as the post leaves it (the ring's start plus its count before, 64-bit),
  // passed on to the slot below.
  const uint64_t r6 = uint64_t(LoadU32(base, dispatcher + 204 + 604)) +
                      uint64_t(LoadU32(base, dispatcher + 204 + 600));
  Post(base, dispatcher, 6);
  // The music: bank 1's cue 0 stopped (the sound manager's slot 13), bank 1's
  // cue 2 played (Sound::Play), the indexes in the frame.
  StoreU32(base, frame + 80, 0);
  StoreU32(base, frame + 84, 1);
  const uint32_t manager = LoadU32(base, menu_sounds::kSoundManager);
  const uint64_t args[6] = {manager, frame + 84, frame + 80, r6, 0, 0};
  calls.call_set(LoadU32(base, LoadU32(base, manager) + 52), args, 6, frame, 0x82233840);
  StoreU32(base, frame + 92, 1);
  StoreU32(base, frame + 88, 2);
  return menu_sounds::PlaySound(base, frame + 92, frame + 88, frame,
                                {const_cast<Calls*>(&calls), &SoundThunk});
}

uint64_t Type6ListScreenSetUpRace(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822334C8 Type6ListScreen::SetUpRace");
  namespace rs = race_setup;
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, kSetUpRaceFrame);
  auto data = [base] { return LoadU32(base, rs::kRaceData); };
  if (!rs::RaceDataResetGlobal(base, frame - 112)) {
    Call1(calls, kResetGlobal, r3, frame, 0x822334DC);
  }
  rs::StagesClear(base, data() + rs::kStageList);
  vehicle_accessors::RaceDataSetWord32(base, race_screens::ListSelection(base, screen + 10144));
  StoreU32(base, data() + kRaceWordA, 2);
  StoreU32(base, data() + kRaceWordB, 4);
  // The route: the item list's +4 plus its selection, 0 to 7 (the switch's
  // cases each answer their own index), else the frame word the original
  // reads unset (its +80).
  const uint32_t index =
      LoadU32(base, screen + 10220) + race_screens::ListSelection(base, screen + 10216);
  const uint32_t route = index <= 7 ? index : LoadU32(base, frame + 80);
  if (!rs::StagesSetRoute(base, data() + rs::kStageList, route, 48, 0xFFFFFFFFu,
                          frame - rs::kRouteFrameBytes)) {
    const uint64_t args[6] = {data() + rs::kStageList, route, 48, ~uint64_t(0), 0, 0};
    calls.call_set(kStagesSetRoute, args, 4, frame, 0x822335C0);
  }
  if (!rs::RaceSetRoute(base, route, 48, frame - rs::kRouteFrameBytes)) {
    const uint64_t args[6] = {route, 48, 0, 0, 0, 0};
    calls.call_set(kRaceSetRoute, args, 2, frame, 0x822335CC);
  }
  if (race_screens::ListSelection(base, screen + 10288) == 0) {
    const int32_t kind = int32_t(race_screens::ListSelection(base, screen + 10144));
    vehicle_accessors::RaceDataSetWord59618(base, kind == 1 ? 2 : kind == 2 ? 1 : 3);
  }
  const uint32_t answer =
      uint32_t(Slot(base, LoadU32(base, screen + 9676), 20, std::nullopt, frame, 0x82233620, calls));
  // The player's racer record at the frame's +96: the type 6 car.
  const uint32_t record = frame + 96;
  rs::RacerRecordInit(base, record);
  rs::SetModel(base, record, LoadU32(base, type67_modes::kType6Car + 12));
  StoreU32(base, frame + 3988, answer);
  if (!native::Buffers_Memcpy(base, record, type67_modes::kType6Car, 1884, 0x82233654)) {
    const uint64_t args[6] = {record, type67_modes::kType6Car, 1884, 0, 0, 0};
    calls.call_set(kMemcpy, args, 3, frame, 0x82233654);
  }
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, record, &refreshed)) {
    Call1(calls, kCarRefresh, record, frame, 0x8223365C);
  }
  const double zero = double(std::bit_cast<float>(LoadU32(base, kZero)));
  if (rs::AddRacerNative(base, record)) {
    rs::AddRacer(base, record, 0, zero, 0);
  } else {
    const uint64_t args[4] = {record, 0, 0, 0};
    calls.call_f1(kAddRacer, args, zero, frame, 0x82233674);
  }
  rs::StagesAddRacer(base, data() + rs::kStageList, 0, 2, -1);
  StoreU32(base, vehicle_accessors::SaveDataOf(base, 0) + kSaveAnswer, answer);
  rs::AddStage(base, 1);
  uint64_t result;
  if (rs::PlaceRacersNative(base, frame - 128)) {
    Refresh refresh{&calls, frame - 128 - 4096};
    result = rs::PlaceRacers(base, frame - 128, RefreshThrough, &refresh);
  } else {
    const uint64_t args[6] = {1, 0, 0, 0, 0, 0};
    result = calls.call_set(kPlaceRacers, args, 1, frame, 0x822336B0);
  }
  StoreU32(base, screen + 9608, 10);
  return result;
}

uint64_t Type6ListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822332D8 Type6ListScreen::~Type6ListScreen");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kType6ListScreenVtable);
  ListPartDestruct(base, r3 + 10824, frame, calls);
  return TitledListScreenDestruct(base, r3, frame, calls);
}

uint64_t Type6ListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82233478 Type6ListScreen::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  Type6ListScreenDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, 0x822334A8);
  }
  return r3;
}

// --- The panel and node screens' leaves ---

uint64_t PanelScreenSetAnswer(uint8_t* base, uint64_t r3, uint32_t answer) {
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
  StoreU32(base, uint32_t(r3) + 9660, answer);
  return r3;
}

uint64_t PanelScreenOnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822306E0 PanelScreen::OnDialogClosed");
  Frame(base, sp, 96);
  const uint32_t screen = uint32_t(r3);
  uint32_t dialog = 0;
  uint64_t result = r3;
  if (LoadU32(base, screen + 9660) == 0) {
    // ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0): its plain C++, r4 left
    // 0 as the original sets it; FadeOut's r3 (the passes).
    const uint32_t passes = LoadU32(base, kFadeOutPasses);
    mode_tick::ScreenFadesFadeOut(base, passes, 0, 0, 0, 0);
    calls.set_r4(0);
    result = passes;
    dialog = 2;
  }
  StoreU32(base, screen + 9656, dialog);
  return result;
}

uint64_t PanelScreenSuspend(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248DE0 PanelScreen::Suspend");
  return Slot(base, LoadU32(base, uint32_t(r3) + 9648), 28, std::nullopt, sp, lr, calls);
}

uint64_t PanelScreenOpenEmpty(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822404D8 PanelScreen::OpenEmpty");
  const uint32_t screen = uint32_t(r3);
  if (LoadU32(base, screen + 9612) == 0) {
    StoreU32(base, screen + 9608, 1);
    return r3;
  }
  const uint32_t list = LoadU32(base, screen + 9644);
  StoreU32(base, screen + 9608, 4);
  return Slot(base, list, 52, 0, sp, lr, calls);
}

uint64_t Type6CarWord(uint8_t* base, uint32_t offset) {
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
  return LoadU32(base, type67_modes::kType6Car + offset);
}

uint64_t GroupPairDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8224ACE0 GroupPair::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kGroupPairBaseVtable);
  if ((flags & 1) != 0) {
    const uint64_t args[6] = {r3, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x8224AD0C);
  }
  return r3;
}

uint64_t NodeScreenOpen(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82259318 NodeScreen::Open");
  StoreU32(base, uint32_t(r3) + 10064, 4);
  return r3;
}

}  // namespace type67_screens
