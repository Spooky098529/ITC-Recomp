// The type 8 list screen (Type8ListScreen over Type8ListBase over
// NodeListScreen; docs/functions/type8-screens.md): see type8_screens.h.
// Read from the generated code. UiMovies::Find, IsLoaded, UiMovie::FindNode,
// ShowNode and HideNode, MenuList::Selection, SetSelection and Init (where
// it frees natively), MenuSound::Play, Sound::Play, ScreenFades::FadeIn and
// FadeOut, the list screens' helpers, the node tables' and NodeListScreen's
// item loader by their plain C++; UiMovie::FinishNamed, PlayNamed,
// LoopNamed and Run, MenuList::Update, Accepted and Cancelled, the sound
// manager's slots 13 and 14 and operator delete guest calls; UiMovie::SetText
// through Calls::set_text; the screen's, the node table's and the list
// part's slots through type67_screens::Slot, the tables' entries through
// SlotNative where they hold one of ours.

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "native/buffers.h"
#include "race_screens/race_screens.h"
#include "race_setup/race_setup.h"
#include "save_data/save_data.h"
#include "screen_constructors/screen_constructors.h"
#include "type67_screens/list_pages.h"
#include "type8_screens/part_helpers.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace type8_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

namespace ts = type67_screens;
namespace pg = type67_screens::pages;
namespace rs = race_screens;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint64_t kMinusOne = ~uint64_t(0);
// The course names' string ids (6 a row, rows overlapping) and the item
// infos (16 bytes each).
constexpr uint32_t kCourseTexts = 0x82010D70;
constexpr uint32_t kItemInfos = 0x82010D90;

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  const uint64_t six[6] = {args[0], args[1], args[2], args[3], 0, 0};
  return static_cast<const Calls*>(user)->call_set(function, six, set, r1, lr);
}

// A node shown (1) or hidden (0): UiMovie::ShowNode or HideNode's plain C++
// (the node's +80); the node.
uint64_t SetShown(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  StoreU32(base, node + 80, shown);
  return node;
}

// FinishNamed, PlayNamed (chained 1) and LoopNamed of the screen's word at
// `field` when it's >= 0 (each call reading it again).
void Replay(uint8_t* base, const Calls& calls, uint32_t screen, uint32_t movie, uint32_t field,
            uint32_t frame, const uint32_t lr[3]) {
  if (int32_t(LoadU32(base, screen + field)) < 0) return;
  calls.call(kFinishNamed, frame, lr[0], movie, LoadU32(base, screen + field));
  pg::Play(calls, movie, LoadU32(base, screen + field), 1, frame, lr[1]);
  calls.call(kLoopNamed, frame, lr[2], movie, LoadU32(base, screen + field));
}

// The scroll text of the kind list's selection, [+10820 + 4 × it].
uint32_t KindText(uint8_t* base, uint32_t screen) {
  return LoadU32(base, screen + ((rs::ListSelection(base, screen + 10144) + 2705) << 2));
}

// The race set-up's guest fallbacks and the float 0.0 Race::AddRacer gets.
constexpr uint32_t kResetGlobal = 0x821021A8, kStagesSetRoute = 0x82101E90,
                   kRaceSetRoute = 0x82101D38, kMemcpy = 0x825A7410, kCarRefresh = 0x820C4998,
                   kAddRacer = 0x821017C0, kPlaceRacers = 0x82102048;
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kSetUpRaceFrame = 4096;
constexpr uint32_t kGarageCarBytes = 1972;

// Car::Refresh's original where Race::PlaceRacers' plain C++ hands it back.
struct Refresh {
  const Calls* calls;
  uint32_t r1;
};

void RefreshThrough(void* user, uint32_t car) {
  const Refresh& r = *static_cast<const Refresh*>(user);
  pg::Call1(*r.calls, kCarRefresh, car, r.r1, 0x82101DC8);
}

// UiMovie::Run(m, 0) == 0.
uint64_t RunDone(const Calls& calls, uint32_t movie, uint32_t frame, uint32_t lr) {
  return uint32_t(calls.call(ts::kRun, frame, lr, movie, 0)) == 0 ? 1 : 0;
}

// The page block's moves with the stores in the originals' order: "page P
// after step S" (+9660, +9656, +9648, +9644) and "page P" (+9656, +9660,
// +9644 or, where the original orders them so, +9656, +9644, +9660:
// pg::ToPage).
void StepThenPage(uint8_t* base, uint32_t screen, uint32_t page, uint32_t step) {
  const uint32_t now = LoadU32(base, screen + 9644);
  if (now != page) StoreU32(base, screen + 9652, now);
  StoreU32(base, screen + 9660, kNone);
  StoreU32(base, screen + 9656, step);
  StoreU32(base, screen + 9648, page);
  StoreU32(base, screen + 9644, kNone);
}

}  // namespace

// --- The constructors and destructors ---

uint64_t ListBaseConstruct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822574D0 Type8ListBase::Type8ListBase");
  const uint32_t screen = uint32_t(r3);
  part_detail::Frame(base, sp, 96);
  screen_constructors::NodeListScreen(base, screen);
  StoreU32(base, screen, kListBaseVtable);
  for (uint32_t i = 0; i < 3; i++) StoreU32(base, screen + 10820 + 4 * i, 545 + i);
  StoreU32(base, screen + 10832, 548);
  StoreU32(base, screen + 10836, 549);
  StoreU32(base, screen + 10840, kNone);
  return r3;
}

uint64_t ListScreen8Construct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223F140 Type8ListScreen::Type8ListScreen");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 112);
  ListBaseConstruct(base, r3, frame);
  StoreU32(base, s, kListScreenVtable);
  type8_screens::ListPartConstruct(base, r3 + kListScreenPart, frame);
  StoreU32(base, s + 9676, uint32_t(r3 + kListScreenPart));
  auto store = [&](uint32_t at, uint32_t value) { StoreU32(base, s + at, value); };
  // The node table's words through [+9672], read again where the original
  // reads it again.
  auto table = [&] { return LoadU32(base, s + 9672); };
  uint32_t t = table();
  static constexpr uint32_t kFirst[][2] = {
      {9832, 143}, {9848, 144}, {9852, 145}, {9856, 146}, {9868, 60},  {9872, 148},
      {9876, 5},   {9880, kNone}, {9860, 0},  {10840, 10}, {9912, 78},  {9896, 7},
      {9820, 7},   {9904, 117}, {9908, 118}, {9916, 79},  {9900, 65},  {9892, 6},
      {9888, 4},   {9728, 6},   {9824, 1},
  };
  for (const auto& w : kFirst) store(w[0], w[1]);
  StoreU32(base, t + 8, 10);
  t = table();
  StoreU32(base, t + 12, 9);
  static constexpr uint32_t kSecond[][2] = {
      {9716, 10}, {9776, 0x82637020}, {9724, 15}, {9712, 0},  {9720, 11}, {9752, 8},
      {9756, 9},  {9768, 3},          {9800, 0},  {9804, 1},  {9748, 0},
  };
  for (const auto& w : kSecond) store(w[0], w[1]);
  for (uint32_t i = 0; i < 4; i++) StoreU32(base, table() + 296 + 4 * i, 6 + i);
  store(9788, 1);
  store(9784, 2);
  for (uint32_t i = 0; i < 27; i++) {
    const uint32_t at = table() + 16 + 4 * i;
    StoreU32(base, at + 140, 1 + i);
    StoreU32(base, table() + 16 + 4 * i, 8 + i);
  }
  for (uint32_t i = 0; i < 8; i++) StoreU32(base, table() + 124 + 4 * i, 39 + i);
  for (uint32_t i = 0; i < 6; i++) store(9936 + 4 * i, 107 + i);
  t = table();
  static constexpr uint32_t kThird[][2] = {
      {9732, 2}, {9812, 4}, {9740, 1}, {9816, 0}, {9976, 68}, {9980, 69}, {9984, 70},
  };
  for (const auto& w : kThird) store(w[0], w[1]);
  StoreU32(base, t + 420, 6);
  StoreU32(base, table() + 424, 55);
  static constexpr uint32_t kLast[][2] = {
      {9708, 1}, {9928, 130}, {9932, 129}, {9744, 20}, {9828, 0}, {9736, 20}, {9808, 0},
  };
  for (const auto& w : kLast) store(w[0], w[1]);
  return r3;
}

uint64_t ListBaseDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257548 Type8ListBase::~Type8ListBase");
  StoreU32(base, uint32_t(r3), kListBaseVtable);
  return ts::NodeListScreenDestruct(base, r3, sp, calls);
}

uint64_t ListBaseDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257B20 Type8ListBase::DeletingDestructor");
  const uint32_t frame = part_detail::Frame(base, sp, 112);
  StoreU32(base, uint32_t(r3), kListBaseVtable);
  ts::NodeListScreenDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    pg::Call1(calls, car_list::kOperatorDelete, r3, frame, 0x82257B5C);
  }
  return r3;
}

uint64_t ListScreen8DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223F388 Type8ListScreen::DeletingDestructor");
  const uint32_t frame = part_detail::Frame(base, sp, 112);
  type8_screens::ListPartDestruct(base, r3 + kListScreenPart, frame, calls);
  type8_screens::ListBaseDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    pg::Call1(calls, car_list::kOperatorDelete, r3, frame, 0x8223F3C4);
  }
  return r3;
}

// --- The screen's slots ---

uint64_t ListScreen8IsLoaded(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8223F590 Type8ListScreen::IsLoaded");
  return ui_movies::IsLoaded(base, LoadU32(base, uint32_t(r3) + 9680));
}

uint64_t ListScreen8OpenMovie(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223F3E0 Type8ListScreen::OpenMovie");
  return calls.open(LoadU32(base, uint32_t(r3) + 9680), 22584, sp, lr);
}

uint64_t ListScreen8Release(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223F588 Type8ListScreen::Release");
  return calls.remove(LoadU32(base, uint32_t(r3) + 9680), sp, lr);
}

uint64_t ListScreen8SetItemTexts(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223F5D8 Type8ListScreen::SetItemTexts");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 128);
  const uint32_t movie = pg::MovieOf(base, s, frame);
  const uint32_t texts = (uint32_t(r4) << 2) + kCourseTexts;
  uint64_t result = 0;
  for (uint32_t i = 0; i < 6; i++) {
    result = calls.set_text(frame, 0x8223F630, movie, LoadU32(base, s + 9744), 5 + i,
                            LoadU32(base, texts + 4 * i));
  }
  return result;
}

uint64_t ListScreen8ItemInfo(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_8223F598 Type8ListScreen::ItemInfo");
  const uint32_t item = uint32_t(r4);
  if (item > 7) return r3;
  const uint32_t from = (item << 4) + kItemInfos;
  for (uint32_t i = 0; i < 16; i += 4) StoreU32(base, info + i, LoadU32(base, from + i));
  return r3;
}

uint64_t ListScreen8ShowMarks(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223F6C8 Type8ListScreen::ShowMarks");
  const uint32_t frame = part_detail::Frame(base, sp, 128);
  const uint32_t movie = pg::MovieOf(base, uint32_t(r3), frame);
  const uint32_t which = uint32_t(r4);
  uint32_t node = ui_movies::FindNode(base, movie, kNone, 7);
  if (int32_t(which) < 0) {
    StoreU32(base, node + 80, 0);
    return node;
  }
  StoreU32(base, node + 80, 1);
  node = ui_movies::FindNode(base, movie, kNone, 17);
  StoreU32(base, node + 80, which != 0 ? 1 : 0);
  node = ui_movies::FindNode(base, movie, kNone, 18);
  StoreU32(base, node + 80, which != 2 ? 1 : 0);
  return node;
}

uint64_t ListScreen8SetLabels(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223F648 Type8ListScreen::SetLabels");
  const uint32_t frame = part_detail::Frame(base, sp, 112);
  const uint32_t movie = pg::MovieOf(base, uint32_t(r3), frame);
  static constexpr uint32_t kLr[3] = {0x8223F68C, 0x8223F6A0, 0x8223F6B4};
  uint64_t result = 0;
  for (uint32_t i = 0; i < 3; i++) {
    result = calls.set_text(frame, kLr[i], movie, 19, 5 + i, 128 + i);
  }
  return result;
}

uint64_t ListScreen8SetUpMenus(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223F3F0 Type8ListScreen::SetUpMenus");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 176);
  part_detail::InitList(base, s + 10216, {8, 6, 1, 154, 153, 155, 156, 159, 158}, frame,
                        0x8223F434, calls);
  part_detail::InitList(base, s + 10288, {2, 2, 1, 156, 155, 154, 153, 159, 158}, frame,
                        0x8223F464, calls);
  rs::ListSetSelection(base, s + 10288, 1);
  part_detail::InitList(base, s + 10144, {3, 3, 1, 154, 153, 155, 156, 159, 158}, frame,
                        0x8223F49C, calls);
  ts::Slot(base, r3, 8, 0, frame, 0x8223F4B4, calls);
  ts::Slot(base, r3, 108, std::nullopt, frame, 0x8223F4C8, calls);
  calls.set_r4(0);
  ts::NodeListScreenLoadItem(base, r3, 0, frame, calls);
  ts::ListScreenOnToggleMoved(base, r3, r3 + 10288, frame, calls);
  // The music: the sound manager's IsCuePlaying (slot 14) for &bank 1,
  // &cue 2 and, when it answers a nonzero byte, its StopCue (slot 13); then
  // slot 14 for bank 1's cue 0 and, when it isn't playing, Sound::Play.
  auto slot = [&](uint32_t offset, uint32_t bank, uint32_t cue, uint32_t lr) {
    const uint32_t manager = LoadU32(base, menu_sounds::kSoundManager);
    const uint64_t args[6] = {manager, bank, cue, 0, 0, 0};
    return calls.call_set(LoadU32(base, LoadU32(base, manager) + offset), args, 3, frame, lr);
  };
  StoreU32(base, frame + 96, 2);
  StoreU32(base, frame + 100, 1);
  uint64_t result = slot(56, frame + 100, frame + 96, 0x8223F510);
  if ((result & 0xFF) != 0) {
    StoreU32(base, frame + 104, 2);
    StoreU32(base, frame + 108, 1);
    slot(52, frame + 108, frame + 104, 0x8223F53C);
  }
  StoreU32(base, frame + 116, 1);
  StoreU32(base, frame + 112, 0);
  result = slot(56, frame + 116, frame + 112, 0x8223F564);
  if ((result & 0xFF) != 0) return result;
  StoreU32(base, frame + 120, 0);
  StoreU32(base, frame + 124, 1);
  return menu_sounds::PlaySound(base, frame + 124, frame + 120, frame,
                                {const_cast<Calls*>(&calls), &SoundThunk});
}

uint64_t ListScreen8SetUpRace(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223F778 Type8ListScreen::SetUpRace");
  namespace rsu = race_setup;
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, kSetUpRaceFrame);
  auto data = [base] { return LoadU32(base, rsu::kRaceData); };
  if (!rsu::RaceDataResetGlobal(base, frame - 112)) {
    pg::Call1(calls, kResetGlobal, r3, frame, 0x8223F794);
  }
  rsu::StagesClear(base, data() + rsu::kStageList);
  StoreU32(base, data() + 0xA3AF0, 4);
  StoreU32(base, data() + 0xA3AF4, 5);
  vehicle_accessors::RaceDataSetWord32(base, rs::ListSelection(base, s + 10144));
  // The course: the course list's +4 plus its selection, 0 to 7 (the
  // switch's cases each answer their own index; case 0 is the merged piece
  // Type8ListScreen::AddRacers sub_8223F818). The recompiled switch traps
  // past 7 (the original's jump table read past its end); never in play.
  const uint32_t route =
      LoadU32(base, s + 10220) + rs::ListSelection(base, s + 10216);
  if (route > 7) {
    ITC_LOG_ERROR("Type8ListScreen::SetUpRace: course {} past the switch's cases", route);
    __builtin_trap();
  }
  if (!rsu::StagesSetRoute(base, data() + rsu::kStageList, route, 48, kNone,
                           frame - rsu::kRouteFrameBytes)) {
    const uint64_t args[6] = {data() + rsu::kStageList, route, 48, kMinusOne, 0, 0};
    calls.call_set(kStagesSetRoute, args, 4, frame, 0x8223F864);
  }
  if (!rsu::RaceSetRoute(base, route, 48, frame - rsu::kRouteFrameBytes)) {
    const uint64_t args[6] = {route, 48, 0, 0, 0, 0};
    calls.call_set(kRaceSetRoute, args, 2, frame, 0x8223F870);
  }
  // The traffic (the toggle's first item: 3).
  vehicle_accessors::RaceDataSetWord59618(
      base, rs::ListSelection(base, s + 10288) == 0 ? 3 : 0);
  // The two players' racer records at the frame's +80: the garage cars,
  // each one's +3892 whether its player's first menu (the list part's
  // blocks' +84) is at its first item.
  const double zero = double(std::bit_cast<float>(LoadU32(base, kZero)));
  const uint32_t record = frame + 80;
  for (uint32_t player = 0; player < 2; player++) {
    const uint32_t car = kGarageCars + kGarageCarBytes * player;
    const uint32_t menu = s + 10864 + (player == 0 ? 124 : 356);
    const uint32_t first = rs::ListSelection(base, menu) == 0 ? 1 : 0;
    rsu::RacerRecordInit(base, record);
    rsu::SetModel(base, record, LoadU32(base, car + 12));
    StoreU32(base, frame + 3972, first);
    if (!native::Buffers_Memcpy(base, record, car, 1884, 0x8223F8F4)) {
      const uint64_t args[6] = {record, car, 1884, 0, 0, 0};
      calls.call_set(kMemcpy, args, 3, frame, 0x8223F8F4);
    }
    uint32_t refreshed = 0;
    if (!car_specs::CarRefresh(base, record, &refreshed)) {
      pg::Call1(calls, kCarRefresh, record, frame, 0x8223F8FC);
    }
    if (rsu::AddRacerNative(base, record)) {
      rsu::AddRacer(base, record, 0, zero, 0);
    } else {
      const uint64_t args[4] = {record, 0, 0, 0};
      calls.call_f1(kAddRacer, args, zero, frame, 0x8223F910);
    }
    rsu::StagesAddRacer(base, data() + rsu::kStageList, player, 2, -1);
  }
  vehicle_accessors::StagesSetType(base, data() + rsu::kStageList, 1, kNone);
  uint64_t result;
  if (rsu::PlaceRacersNative(base, frame - 128)) {
    Refresh refresh{&calls, frame - 128 - 4096};
    result = rsu::PlaceRacers(base, frame - 128, RefreshThrough, &refresh);
  } else {
    // r3 not read (Stages::SetType's left).
    const uint64_t args[6] = {0, 0, 0, 0, 0, 0};
    result = calls.call_set(kPlaceRacers, args, 0, frame, 0x8223F94C);
  }
  // The screen leaves: message 4 posted (+9608 not written).
  ts::Post(base, uint32_t(r4), 4);
  return result;
}

// --- The tables ---

uint64_t ListBaseDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258D60 Type8ListBase::Draw");
  const uint32_t frame = part_detail::Frame(base, sp, 224);
  static constexpr uint32_t kEntries[10] = {
      ts::kEmpty, ts::kEmpty, 0x82257588, 0x82257768, 0x82258A40,
      0x82258AD0, 0x822578D8, ts::kEmpty, 0x82257900, 0x82255818};
  pg::BuildTable(base, frame, kListBasePageGuard, kListBasePages, kEntries, 10);
  const uint32_t page = LoadU32(base, uint32_t(r3) + 9644);
  if (page == kNone) return r3;
  const uint32_t entry = kListBasePages + (page << 3);
  const uint32_t target = LoadU32(base, entry);
  const uint64_t self = r3 + LoadU32(base, entry + 4);
  return pg::CallEntry(base, target, self, r4, std::nullopt, frame, 0x82258EA8, calls);
}

uint64_t ListBaseUpdate(uint8_t* base, uint64_t r3, std::optional<uint64_t> r5, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258EB0 Type8ListBase::Update");
  const uint32_t frame = part_detail::Frame(base, sp, 160);
  const uint32_t step = LoadU32(base, uint32_t(r3) + 9656);
  // r4 the guard's word as loaded (the original keeps it there).
  const uint32_t guard = LoadU32(base, kListBaseStepGuard);
  static constexpr uint32_t kEntries[6] = {0x82258158, 0x82258648, 0x82258BE0,
                                           0x82258438, 0x822580E0, 0x82258578};
  if (pg::BuildTable(base, frame, kListBaseStepGuard, kListBaseSteps, kEntries, 6)) {
    r5 = kEntries[1];
  }
  const uint32_t entry = kListBaseSteps + (step << 3);
  const uint32_t target = LoadU32(base, entry);
  const uint64_t self = r3 + LoadU32(base, entry + 4);
  return pg::CallEntry(base, target, self, guard, r5, frame, 0x82258F98, calls);
}

// --- The pages ---

uint64_t ListBasePage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257588 Type8ListBase::Page2");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 112);
  const uint64_t list = r3 + 10144;
  pg::Call1(calls, rs::kListUpdate, list, frame, 0x822575AC);
  const uint32_t kinds = uint32_t(list);
  if (LoadU32(base, s + 10168) != 0 ||
      LoadU32(base, kinds + 4) - LoadU32(base, kinds + 8) != 0) {
    // The kind moved: its scroll text, page 2 after step 1.
    pg::PlaySound(base, 4, frame, calls);
    ts::ListScreenShowSelection(base, r3, list, frame, calls);
    StepThenPage(base, s, 2, 1);
    return ts::ListScreenSetScrollText(base, r3, KindText(base, s), 1, frame, calls);
  }
  if (int32_t(uint32_t(pg::Call1(calls, rs::kListAccepted, list, frame, 0x822575D0))) != 0) {
    // Into the course list from its first item: page 3 after step 2.
    pg::PlaySound(base, 0, frame, calls);
    StepThenPage(base, s, 3, 2);
    StoreU32(base, s + 10220, 0);
    rs::ListSetSelection(base, s + 10216, 0);
    ts::Slot(base, r3, 8, LoadU32(base, s + 10220), frame, 0x8225763C, calls);
    const uint32_t first = LoadU32(base, s + 10220);
    const uint32_t selection = rs::ListSelection(base, s + 10216);
    return ts::NodeListScreenLoadItem(base, r3, uint64_t(first) + uint64_t(selection), frame,
                                      calls);
  }
  if (int32_t(uint32_t(pg::Call1(calls, rs::kListCancelled, list, frame, 0x82257664))) != 0) {
    // Back out: page 8 with the screen fades' fade-out.
    pg::PlaySound(base, 2, frame, calls);
    const uint32_t now = LoadU32(base, s + 9644);
    if (now != 8) StoreU32(base, s + 9652, now);
    StoreU32(base, s + 9656, kNone);
    StoreU32(base, s + 9660, kNone);
    StoreU32(base, s + 9644, 8);
    return pg::FadeOut(base, calls);
  }
  const uint64_t toggle = r3 + 10288;
  const uint64_t result = pg::Call1(calls, rs::kListUpdate, toggle, frame, 0x822576C4);
  if (LoadU32(base, s + 10312) == 0) return result;
  pg::PlaySound(base, 4, frame, calls);
  return ts::ListScreenOnToggleMoved(base, r3, toggle, frame, calls);
}

uint64_t ListBasePage3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82257768 Type8ListBase::Page3");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 112);
  const uint64_t list = r3 + 10216;
  pg::Call1(calls, rs::kListUpdate, list, frame, 0x8225778C);
  const uint32_t courses = uint32_t(list);
  if (LoadU32(base, s + 10240) != 0 ||
      LoadU32(base, courses + 4) - LoadU32(base, courses + 8) != 0) {
    // The course moved: its texts and item, page 3 after step 4.
    pg::PlaySound(base, 4, frame, calls);
    ts::Slot(base, r3, 8, LoadU32(base, s + 10220), frame, 0x82257870, calls);
    ts::PairedNodeTableSavePairs(base, r3 + 10360);
    const uint32_t first = LoadU32(base, s + 10220);
    const uint32_t selection = rs::ListSelection(base, courses);
    const uint64_t result =
        ts::NodeListScreenLoadItem(base, r3, uint64_t(first) + uint64_t(selection), frame, calls);
    const uint32_t now = LoadU32(base, s + 9644);
    if (now != 3) StoreU32(base, s + 9652, now);
    StoreU32(base, s + 9644, kNone);
    StoreU32(base, s + 9648, 3);
    StoreU32(base, s + 9656, 4);
    StoreU32(base, s + 9660, kNone);
    return result;
  }
  if (int32_t(uint32_t(pg::Call1(calls, rs::kListAccepted, list, frame, 0x822577B0))) != 0) {
    // Page 4 (the list part).
    const uint64_t result = pg::PlaySound(base, 0, frame, calls);
    const uint32_t now = LoadU32(base, s + 9644);
    if (now != 4) StoreU32(base, s + 9652, now);
    StoreU32(base, s + 9644, 4);
    StoreU32(base, s + 9656, kNone);
    StoreU32(base, s + 9660, kNone);
    return result;
  }
  if (int32_t(uint32_t(pg::Call1(calls, rs::kListCancelled, list, frame, 0x822577EC))) != 0) {
    // Back to the kind list: page 2 after step 3.
    pg::PlaySound(base, 2, frame, calls);
    const uint64_t result = type8_screens::PairedNodeTableClear(base, r3 + 10360, frame, calls);
    const uint32_t now = LoadU32(base, s + 9644);
    if (now != 2) StoreU32(base, s + 9652, now);
    StoreU32(base, s + 9644, kNone);
    StoreU32(base, s + 9648, 2);
    StoreU32(base, s + 9656, 3);
    StoreU32(base, s + 9660, kNone);
    return result;
  }
  const uint64_t toggle = r3 + 10288;
  const uint64_t result = pg::Call1(calls, rs::kListUpdate, toggle, frame, 0x8225782C);
  if (LoadU32(base, s + 10312) == 0) return result;
  pg::PlaySound(base, 4, frame, calls);
  return ts::ListScreenOnToggleMoved(base, r3, toggle, frame, calls);
}

uint64_t ListBasePage4(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258A40 Type8ListBase::Page4");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 96);
  const uint32_t part = LoadU32(base, s + 9676);
  if (part != 0) {
    // Slot 1 (ListPartBase::Open) with r4 as the page was given it.
    ts::Slot(base, part, 4, std::nullopt, frame, 0x82258A70, calls);
    StoreU32(base, s + 9696, 1);
  }
  ts::ListScreenSetScrollText(base, r3, LoadU32(base, s + 10840), 1, frame, calls);
  pg::ToPage(base, s, 5);
  return type8_screens::ListBaseDimList(base, r3, 1, frame, calls);
}

uint64_t ListBasePage5(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258AD0 Type8ListBase::Page5");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 96);
  const uint32_t part = LoadU32(base, s + 9676);
  if (part == 0) {
    pg::ToPage(base, s, 9);
    return part;
  }
  // Slot 2 (ListPartBase::Pick) with the page's r4.
  const uint64_t picked = ts::Slot(base, part, 8, r4, frame, 0x82258B00, calls);
  if (int32_t(uint32_t(picked)) == 0) return picked;
  const uint32_t now = LoadU32(base, s + 9676);
  if (LoadU32(base, now + 4) != 0) {
    // Yes: page 6, the race set.
    pg::ToPage(base, s, 6);
    StoreU32(base, s + 9696, 0);
    StoreU32(base, s + 9688, 1);
    return picked;
  }
  if (LoadU32(base, now + 8) == 0) return picked;
  // Back: page 3, the course list's scroll text, the list undimmed.
  pg::ToPage(base, s, 3);
  StoreU32(base, s + 9696, 0);
  ts::ListScreenSetScrollText(base, r3, LoadU32(base, s + 10836), 1, frame, calls);
  return type8_screens::ListBaseDimList(base, r3, 0, frame, calls);
}

uint64_t ListBasePage6(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822578D8 Type8ListBase::Page6");
  pg::ToPage(base, uint32_t(r3), 9);
  return r3;
}

uint64_t ListBasePage8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82257900 Type8ListBase::Page8");
  if ((LoadU32(base, ts::kFadeCountA) | LoadU32(base, ts::kFadeCountB)) != 0) return r3;
  pg::ToPage(base, uint32_t(r3), 9);
  return r3;
}

// --- The steps ---

uint64_t ListBaseStep0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258158 Type8ListBase::Step0");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 128);
  const uint32_t movie = pg::MovieOf(base, s, frame);
  if (LoadU32(base, s + 9668) == 0) {
    // FinishNamed and PlayNamed (chained 1) of +9864, +9888, +9832.
    static constexpr uint32_t kPlayed[3][3] = {{9864, 0x822581A4, 0x822581C0},
                                               {9888, 0x822581D4, 0x822581F0},
                                               {9832, 0x82258204, 0x82258220}};
    for (const auto& p : kPlayed) {
      if (int32_t(LoadU32(base, s + p[0])) < 0) continue;
      calls.call(kFinishNamed, frame, p[1], movie, LoadU32(base, s + p[0]));
      pg::Play(calls, movie, LoadU32(base, s + p[0]), 1, frame, p[2]);
    }
    static constexpr uint32_t kLr9856[3] = {0x82258234, 0x82258250, 0x8225825C};
    static constexpr uint32_t kLr9860[3] = {0x82258270, 0x8225828C, 0x82258298};
    Replay(base, calls, s, movie, 9856, frame, kLr9856);
    Replay(base, calls, s, movie, 9860, frame, kLr9860);
    for (uint32_t i = 0; i < 10; i++) {
      const uint32_t named = LoadU32(base, s + 9976 + 4 * i);
      if (int32_t(named) >= 0) calls.call(kFinishNamed, frame, 0x822582B4, movie, named);
    }
    ts::ListScreenClearScrollText(base, r3, frame, calls);
    ts::ListScreenSetScrollText(base, r3, KindText(base, s), 1, frame, calls);
    // Slot 26 (ShowMarks) with r4 = -1.
    ts::Slot(base, r3, 104, kMinusOne, frame, 0x82258304, calls);
    mode_tick::ScreenFadesFadeIn(base, LoadU32(base, kFadeInPasses), 0, 0, 0, 0);
    calls.set_r4(0);
    StoreU32(base, s + 9668, 1);
  }
  uint64_t done = RunDone(calls, movie, frame, 0x82258334);
  if (int32_t(LoadU32(base, s + 9832)) < 0 && int32_t(LoadU32(base, s + 9864)) < 0) done = 1;
  if (done == 0) return 0;
  static constexpr uint32_t kLr9868[3] = {0x82258374, 0x82258390, 0x8225839C};
  static constexpr uint32_t kLr9876[3] = {0x822583B0, 0x822583CC, 0x822583D8};
  static constexpr uint32_t kLr9880[3] = {0x822583EC, 0x82258408, 0x82258414};
  Replay(base, calls, s, movie, 9868, frame, kLr9868);
  Replay(base, calls, s, movie, 9876, frame, kLr9876);
  Replay(base, calls, s, movie, 9880, frame, kLr9880);
  // ListScreen::LoopSecondNode and ShowToggle.
  ts::ListScreenLoopNode(base, r3, r3 + 10144, 9976, frame, calls);
  ts::ListScreenShowToggle(base, r3, r3 + 10288, frame, calls);
  return done;
}

uint64_t ListBaseStep1(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258648 Type8ListBase::Step1");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 144);
  const uint32_t movie = pg::MovieOf(base, s, frame);
  const uint32_t kinds = s + 10144;
  const uint32_t selection = rs::ListSelection(base, kinds);
  // MenuList::GetPreviousSelection (sub_8220BC28): +20.
  const uint32_t previous = LoadU32(base, kinds + 20);
  const int32_t moved = int32_t(selection - previous);
  if (LoadU32(base, s + 9668) == 0) {
    if (moved != 0) {
      // The kinds' named animations, [+9976 + 4i]: the previous one's
      // finished, the new one's replayed.
      if (int32_t(previous) >= 0) {
        const uint32_t named = LoadU32(base, s + ((previous + 2494) << 2));
        if (int32_t(named) >= 0) calls.call(kFinishNamed, frame, 0x822586CC, movie, named);
      }
      if (int32_t(selection) >= 0) {
        const uint32_t named = LoadU32(base, s + ((selection + 2494) << 2));
        if (int32_t(named) >= 0) {
          calls.call(kFinishNamed, frame, 0x822586F4, movie, named);
          pg::Play(calls, movie, named, 1, frame, 0x82258710);
          calls.call(kLoopNamed, frame, 0x8225871C, movie, named);
        }
      }
      // The direction's arrow (+9916 down, +9912 up) played once a step
      // moved, the other finished.
      const uint32_t play = LoadU32(base, s + (moved > 0 ? 9916 : 9912));
      const uint32_t other = LoadU32(base, s + (moved > 0 ? 9912 : 9916));
      if (int32_t(other) >= 0) calls.call(kFinishNamed, frame, 0x82258748, movie, other);
      if (int32_t(play) >= 0) {
        const uint32_t sign = uint32_t(moved >> 31);
        const uint32_t count = (uint32_t(moved) ^ sign) - sign;
        for (uint32_t k = count; k != 0; k--) {
          calls.call(kFinishNamed, frame, 0x82258770, movie, play);
          pg::Play(calls, movie, play, 0, frame, 0x8225878C);
        }
        if (count > 1) calls.call(kFinishNamed, frame, 0x822587A8, movie, play);
      }
    }
    StoreU32(base, s + 9668, 1);
  }
  calls.call(ts::kRun, frame, 0x822587BC, movie, 0);
  return 1;
}

uint64_t ListBaseStep2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258BE0 Type8ListBase::Step2");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 128);
  const uint32_t movie = pg::MovieOf(base, s, frame);
  if (LoadU32(base, s + 9668) == 0) {
    if (int32_t(LoadU32(base, s + 9884)) >= 0) {
      pg::Play(calls, movie, LoadU32(base, s + 9884), 1, frame, 0x82258C3C);
      calls.call(kLoopNamed, frame, 0x82258C48, movie, LoadU32(base, s + 9884));
    }
    if (int32_t(LoadU32(base, s + 9848)) >= 0) {
      pg::Play(calls, movie, LoadU32(base, s + 9848), 0, frame, 0x82258C6C);
    }
    ts::ColorListScreenPlaceColorCursor(base, r3, rs::ListSelection(base, s + 10216), frame);
    // The node table's slot 1, then PairedNodeTable::Refresh.
    const uint64_t table = r3 + 10360;
    ts::Slot(base, table, 4, std::nullopt, frame, 0x82258C98, calls);
    ts::PairedNodeTableRefresh(base, table, frame, calls);
    ts::ListScreenSetScrollText(base, r3, LoadU32(base, s + 10836), 1, frame, calls);
    StoreU32(base, s + 9668, 1);
  }
  uint64_t done = RunDone(calls, movie, frame, 0x82258CC4);
  const uint32_t named = LoadU32(base, s + 9848);
  if (int32_t(named) < 0) done = 1;
  if (done == 0) return 0;
  if (int32_t(named) >= 0) calls.call(kFinishNamed, frame, 0x82258CF4, movie, named);
  // ListScreen::LoopItemNode.
  ts::ListScreenLoopNode(base, r3, r3 + 10216, 9936, frame, calls);
  static constexpr uint32_t kLr9872[3] = {0x82258D14, 0x82258D30, 0x82258D3C};
  Replay(base, calls, s, movie, 9872, frame, kLr9872);
  // Slot 26 (ShowMarks) with r4 = +10220.
  ts::Slot(base, r3, 104, LoadU32(base, s + 10220), frame, 0x82258D54, calls);
  return done;
}

uint64_t ListBaseStep3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82258438 Type8ListBase::Step3");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 128);
  const uint32_t movie = pg::MovieOf(base, s, frame);
  if (LoadU32(base, s + 9668) == 0) {
    const uint32_t courses = s + 10216;
    auto course_node = [&] {
      return LoadU32(base, s + ((rs::ListSelection(base, courses) + 2484) << 2));
    };
    if (int32_t(course_node()) >= 0) {
      calls.call(kFinishNamed, frame, 0x822584B0, movie, course_node());
    }
    if (int32_t(LoadU32(base, s + 9852)) >= 0) {
      pg::Play(calls, movie, LoadU32(base, s + 9852), 0, frame, 0x822584D4);
    }
    ts::Slot(base, r3, 104, kMinusOne, frame, 0x822584EC, calls);
    ts::ListScreenSetScrollText(base, r3, KindText(base, s), 1, frame, calls);
    StoreU32(base, s + 9668, 1);
  }
  uint64_t done = RunDone(calls, movie, frame, 0x82258524);
  const uint32_t named = LoadU32(base, s + 9852);
  if (int32_t(named) < 0) done = 1;
  if (done == 0) return 0;
  if (int32_t(named) >= 0) calls.call(kFinishNamed, frame, 0x82258554, movie, named);
  const uint32_t toggle = LoadU32(base, s + 9884);
  if (int32_t(toggle) >= 0) calls.call(kFinishNamed, frame, 0x82258568, movie, toggle);
  return done;
}

// --- The dimming and the node table's clear ---

uint64_t ListBaseDimList(uint8_t* base, uint64_t r3, uint64_t dim, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822587C8 Type8ListBase::DimList");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 128);
  const uint32_t movie = pg::MovieOf(base, s, frame);
  const bool dimming = int32_t(uint32_t(dim)) != 0;
  // The toggle's named animation (+9892 for its selection 0, else +9896)
  // and the course's ([+9936 + 4 × the course list's selection]), read
  // again for each call as the original does.
  const uint32_t toggle = s + 10288, courses = s + 10216;
  auto toggle_node = [&] {
    return LoadU32(base, s + (rs::ListSelection(base, toggle) == 0 ? 9892 : 9896));
  };
  auto course_node = [&] {
    return LoadU32(base, s + ((rs::ListSelection(base, courses) + 2484) << 2));
  };
  uint64_t result = rs::ListSelection(base, toggle);
  if (int32_t(LoadU32(base, s + (uint32_t(result) == 0 ? 9892 : 9896))) >= 0) {
    if (dimming) {
      result = calls.call(kFinishNamed, frame, 0x82258850, movie, toggle_node());
    } else {
      pg::Play(calls, movie, toggle_node(), 1, frame, 0x82258884);
      result = calls.call(kLoopNamed, frame, 0x822588A8, movie, toggle_node());
    }
  }
  result = rs::ListSelection(base, courses);
  if (int32_t(LoadU32(base, s + ((uint32_t(result) + 2484) << 2))) >= 0) {
    if (dimming) {
      result = calls.call(kFinishNamed, frame, 0x822588F0, movie, course_node());
    } else {
      pg::Play(calls, movie, course_node(), 1, frame, 0x82258920);
      result = calls.call(kLoopNamed, frame, 0x82258940, movie, course_node());
    }
  }
  const uint32_t shown = dimming ? 0 : 1;
  const uint32_t prompt = LoadU32(base, s + 9820);
  if (int32_t(prompt) >= 0) result = SetShown(base, movie, kNone, prompt, shown);
  const uint32_t part = LoadU32(base, s + 9824);
  if (int32_t(part) >= 0) {
    result = SetShown(base, movie, LoadU32(base, s + 9728), part, shown);
  }
  return result;
}

uint64_t PairedNodeTableClear(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248A20 PairedNodeTable::Clear");
  const uint32_t t = uint32_t(r3), frame = part_detail::Frame(base, sp, 128);
  StoreU32(base, frame + 80, LoadU32(base, t + 4));
  const uint32_t found = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = LoadU32(base, found);
  uint64_t result = found;
  // The third pair's named animation, [+124 + 4 × +444].
  const uint32_t third = LoadU32(base, t + 444);
  if (int32_t(third) >= 0) {
    const uint32_t named = LoadU32(base, t + ((third + 31) << 2));
    if (int32_t(named) >= 0) result = calls.call(kFinishNamed, frame, 0x82248A74, movie, named);
  }
  // The first pair's item and the second's (unless the same), shown off
  // and their named animations ([+16 + 4i], read before) finished.
  const uint32_t first = LoadU32(base, t + 428);
  if (int32_t(first) >= 0) {
    const uint32_t named = LoadU32(base, t + ((first + 4) << 2));
    result = ts::NodeTableShowNode(base, r3, first, 0, 0, frame);
    if (int32_t(named) >= 0) result = calls.call(kFinishNamed, frame, 0x82248AB4, movie, named);
  }
  const uint32_t now = LoadU32(base, t + 428), second = LoadU32(base, t + 436);
  if (second != now && int32_t(second) >= 0) {
    const uint32_t named = LoadU32(base, t + ((second + 4) << 2));
    result = ts::NodeTableShowNode(base, r3, second, 0, 0, frame);
    if (int32_t(named) >= 0) result = calls.call(kFinishNamed, frame, 0x82248B00, movie, named);
  }
  return result;
}

}  // namespace type8_screens
