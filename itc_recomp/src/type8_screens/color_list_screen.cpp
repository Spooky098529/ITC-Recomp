// The type 8 color list screen (Type8ColorListScreen over ColorListScreen;
// docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code. ColorListScreen's constructor, destructor and item
// loader, ListScreen::OnToggleMoved, the color list part's, UiMovies::Find,
// MenuList::Init (where it frees natively) and SetSelection, Sound::Play and
// memcpy by their plain C++; UiMovies::Open through Calls::open,
// UiMovie::SetText through Calls::set_text; UiMovie::ClearText, the sound
// manager's slots 13 and 14, operator delete and memcpy where its plain C++
// declines guest calls; the screen's slot 28 through type67_screens::Slot.

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "car_specs/car_specs.h"
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "native/buffers.h"
#include "race_screens/race_screens.h"
#include "race_setup/race_setup.h"
#include "schema/screen_constructors.h"
#include "schema/type8_screens.h"
#include "screen_constructors/screen_constructors.h"
#include "type67_screens/list_pages.h"
#include "type8_screens/part_helpers.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace type8_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

namespace ts = type67_screens;
namespace pg = type67_screens::pages;
using G = schema::Type8ScreensGlobals;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemcpy = 0x825A7410;
// The item records (28 bytes each: +8 a count, +12 and +16 the details'
// indexes, +20 and +24 the colors' bits).
constexpr uint32_t kItemRecords = 0x82010BE0;
constexpr uint32_t kRecordBytes = 28;
// The item texts' string ids (3 a row, rows overlapping), the details'
// string ids by index and the labels' (3).
constexpr uint32_t kItemTexts = 0x82010B90;
constexpr uint32_t kDetailTexts = 0x82010BB4;
constexpr uint32_t kLabelTexts = 0x82010BD4;
constexpr uint32_t kClearText = 0x8211C470;

uint64_t ClearText(const Calls& calls, uint32_t movie, uint32_t sub_scene, uint32_t child,
                   uint32_t frame, uint32_t lr) {
  const uint64_t args[6] = {movie, sub_scene, child, 0, 0, 0};
  return calls.call_set(kClearText, args, 3, frame, lr);
}

// The race set-up's guest fallbacks and the float 0.0 Race::AddRacer gets.
constexpr uint32_t kResetGlobal = 0x821021A8, kStagesSetRoute = 0x82101E90,
                   kRaceSetRoute = 0x82101D38, kCarRefresh = 0x820C4998,
                   kAddRacer = 0x821017C0, kPlaceRacers = 0x82102048;
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kSetUpRaceFrame = 4112;
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

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  const uint64_t six[6] = {args[0], args[1], args[2], args[3], 0, 0};
  return static_cast<const Calls*>(user)->call_set(function, six, set, r1, lr);
}

// slw: 1 shifted left by `amount`'s low six bits (0 from 32 up).
uint32_t Bit(uint32_t amount) {
  return (amount & 0x20) != 0 ? 0 : 1u << (amount & 0x1F);
}

}  // namespace

// --- The constructor and destructor ---

uint64_t ColorList8Construct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223DFC0 Type8ColorListScreen::Type8ColorListScreen");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 144);
  const schema::Type8ColorListScreen cl(base, s);
  screen_constructors::ColorListScreen(base, s);
  cl.set_vtable(kColorListVtable);
  type8_screens::ColorPartConstruct(base, r3 + kColorListPart, frame);
  cl.set_part(s + kColorListPart);
  // The words in the original's order (the node table's +8 and +12 stored
  // directly, through the screen).
  cl.set_unknown_9908(140);
  cl.set_arrow_up_anim(100);
  cl.set_arrow_down_anim(101);
  cl.set_unknown_9832(165);
  cl.set_unknown_9856(170);
  cl.set_selection_anim(87);
  cl.set_unknown_9840(166);
  cl.set_unknown_9844(168);
  cl.set_table_node_set_a(12);
  cl.set_table_grid_b1(67);
  cl.set_table_grid_a1(79);
  cl.set_unknown_9864(kNone);
  cl.set_part_text(13);
  cl.set_unknown_9848(167);
  cl.set_unknown_9852(169);
  cl.set_unknown_9868(47);
  cl.set_unknown_9872(171);
  cl.set_unknown_9888(kNone);
  cl.set_unknown_9904(139);
  cl.set_table_grid_a2(76);
  cl.set_table_grid_a3(77);
  cl.set_table_grid_a5(78);
  cl.set_table_grid_b5(66);
  cl.set_toggle_anim_on(6);
  cl.set_unknown_9860(0);
  cl.set_unknown_9876(5);
  cl.set_unknown_9920(20);
  cl.set_unknown_9924(21);
  cl.set_toggle_anim_off(7);
  cl.set_part_node_set(6);
  cl.set_part_node(1);
  cl.set_prompt_node(7);
  cl.set_table_node_set_b(11);
  cl.set_table_grid_b2(64);
  cl.set_table_grid_b3(65);
  cl.set_table_grid_a6(80);
  cl.set_unknown_9720(11);
  cl.set_table_grid_b6(68);
  cl.set_table_grid_a7(81);
  cl.set_table_node_base_a(5);
  // The node table's address, loaded here (its +296 store uses it).
  const uint32_t table = cl.node_table().address;
  cl.set_table_grid_b7(69);
  cl.set_selection_node(3);
  cl.set_table_node_base_b(20);
  cl.set_selection_node_set(16);
  cl.set_unknown_9716(10);
  cl.set_unknown_9712(0);
  cl.set_arrow_a_node(8);
  cl.set_arrow_b_node(9);
  cl.set_arrow_on_frame(0);
  cl.set_arrow_off_frame(1);
  cl.set_marker_node(0);
  StoreU32(base, table + 296, 6);
  for (uint32_t i = 1; i < 4; i++) {
    StoreU32(base, cl.node_table().address + 296 + 4 * i, 6 + i);  // +300 to +308: 7 to 9
  }
  cl.set_selection_entries(2);
  cl.set_unknown_9788(1);
  // 15 items through the table at +10432 directly: +16 + 4i = 48 + i,
  // +156 + 4i = 1 + i, +312 + 4i = the record's color mask.
  for (uint32_t i = 0; i < 15; i++) {
    const uint32_t count_word = kItemRecords + 8 + kRecordBytes * i;
    StoreU32(base, cl.table_item_anims_address() + 4 * i, 48 + i);
    StoreU32(base, cl.table_item_nodes_address() + 4 * i, 1 + i);
    const int32_t count = int32_t(LoadU32(base, count_word));
    uint32_t mask = 0;
    if (count >= 2) {
      const uint32_t high = Bit(LoadU32(base, count_word + 16));
      mask = high | Bit(LoadU32(base, count_word + 12));
    } else if (count > 0) {
      mask = Bit(LoadU32(base, count_word + 12));
    }
    StoreU32(base, cl.table_item_masks_address() + 4 * i, mask);
  }
  // Three runs: +9936 + 4i = 129 + i, +9976 + 4i = 90 + i, +10016 + 4i =
  // 10 + i.
  for (uint32_t i = 0; i < 3; i++) {
    StoreU32(base, cl.item_anims_address() + 4 * i, 129 + i);
    StoreU32(base, cl.second_anims_address() + 4 * i, 90 + i);
    StoreU32(base, cl.third_anims_address() + 4 * i, 10 + i);
  }
  cl.set_color_cursor_node_set(9);
  cl.set_unknown_9684(0);
  cl.set_color_cursor_child(0);
  cl.set_icons_node_set(2);
  cl.set_scroll_text_node_set(1);
  cl.set_unknown_10056(141);
  cl.set_scroll_text_child(0);
  cl.set_unknown_10060(142);
  cl.set_selection_array(0x8263700C);
  cl.set_unknown_10064(143);
  cl.set_unknown_10104(157);
  cl.set_unknown_10096(155);
  cl.set_unknown_10100(156);
  cl.set_show_part_nodes(1);
  cl.set_icons_child(4);
  cl.set_unknown_9928(152);
  cl.set_unknown_9932(151);
  cl.set_item_node_set(21);
  cl.set_item_cursor_child(0);
  return r3;
}

uint64_t ColorList8DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E2B8 Type8ColorListScreen::DeletingDestructor");
  const uint32_t frame = part_detail::Frame(base, sp, 112);
  type8_screens::ColorPartDestruct(base, r3 + kColorListPart, frame, calls);
  ts::ColorListScreenDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    pg::Call1(calls, car_list::kOperatorDelete, r3, frame, 0x8223E2F4);
  }
  return r3;
}

// --- The screen's slots ---

uint64_t ColorList8OpenMovie(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E310 Type8ColorListScreen::OpenMovie");
  return calls.open(schema::Type8ColorListScreen(base, uint32_t(r3)).movie_id(), 22585, sp, lr);
}

uint64_t ColorList8ItemInfo(uint8_t* base, uint64_t r4, uint64_t r5, uint32_t sp, uint32_t lr,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E4D0 Type8ColorListScreen::ItemInfo");
  if (uint32_t(r4) > 8) return r5;
  // memcpy(r5, the item's record, 28), a tail call.
  const uint32_t from = uint32_t(r4 * kRecordBytes) + kItemRecords;
  if (native::Buffers_Memcpy(base, uint32_t(r5), from, kRecordBytes, lr)) {
    return r5;
  }
  const uint64_t args[6] = {r5, from, kRecordBytes, 0, 0, 0};
  return calls.call_set(kMemcpy, args, 3, sp, lr);
}

uint64_t ColorList8SetItemText(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E4F8 Type8ColorListScreen::SetItemText");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 128);
  const schema::Type8ColorListScreen cl(base, s);
  const uint32_t movie = pg::MovieOf(base, s, frame);
  const uint32_t texts = (uint32_t(r4) << 2) + kItemTexts;
  uint64_t result = 0;
  for (uint32_t i = 0; i < 3; i++) {
    result = calls.set_text(frame, 0x8223E550, movie, cl.item_node_set(), 5 + i,
                            LoadU32(base, texts + 4 * i));
  }
  return result;
}

uint64_t ColorList8ClearItemText(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E568 Type8ColorListScreen::ClearItemText");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 128);
  const schema::Type8ColorListScreen cl(base, s);
  const uint32_t movie = pg::MovieOf(base, s, frame);
  uint64_t result = 0;
  for (int32_t i = 0; i < 3; i++) {
    if (i == int32_t(uint32_t(r4))) continue;
    result = ClearText(calls, movie, cl.item_node_set(), uint32_t(5 + i), frame, 0x8223E5B4);
  }
  return result;
}

uint64_t ColorList8SetDetails(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E5C8 Type8ColorListScreen::SetDetails");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 160);
  const schema::Type8ColorListScreen cl(base, s);
  const uint32_t movie = part_detail::MovieAt(base, frame + 96, cl.movie_id());
  const uint32_t record = uint32_t(r4 * kRecordBytes) + kItemRecords;
  part_detail::InitList(base, cl.course_list_address(),
                        {LoadU32(base, record + 8), 2, 1, 154, 153, 155, 156, 159, 158}, frame,
                        0x8223E640, calls);
  uint64_t result = 0;
  for (int32_t i = 0; i < 2; i++) {
    if (i < int32_t(LoadU32(base, record + 8))) {
      const uint32_t index = LoadU32(base, record + 12 + 4 * uint32_t(i));
      result = calls.set_text(frame, 0x8223E678, movie, 9, uint32_t(5 + i),
                              LoadU32(base, (index << 2) + kDetailTexts));
    } else {
      result = ClearText(calls, movie, 9, uint32_t(5 + i), frame, 0x8223E680);
    }
  }
  return result;
}

uint64_t ColorList8SetLabels(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E698 Type8ColorListScreen::SetLabels");
  const uint32_t frame = part_detail::Frame(base, sp, 128);
  const uint32_t movie = pg::MovieOf(base, uint32_t(r3), frame);
  uint64_t result = 0;
  for (uint32_t i = 0; i < 3; i++) {
    result = calls.set_text(frame, 0x8223E6E8, movie, 20, 5 + i,
                            LoadU32(base, kLabelTexts + 4 * i));
  }
  return result;
}

uint64_t ColorList8SetUpMenus(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E320 Type8ColorListScreen::SetUpMenus");
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, 176);
  const schema::Type8ColorListScreen cl(base, s);
  part_detail::InitList(base, cl.item_list_address(), {9, 3, 1, 154, 153, 155, 156, 159, 158},
                        frame, 0x8223E364, calls);
  part_detail::InitList(base, cl.toggle_address(), {2, 2, 1, 156, 155, 154, 153, 159, 158}, frame,
                        0x8223E394, calls);
  race_screens::ListSetSelection(base, cl.toggle_address(), 1);
  part_detail::InitList(base, cl.kind_list_address(), {3, 3, 1, 154, 153, 155, 156, 159, 158},
                        frame, 0x8223E3CC, calls);
  part_detail::InitList(base, cl.course_list_address(), {2, 2, 1, 154, 153, 155, 156, 159, 158},
                        frame, 0x8223E3F8, calls);
  // Slot 28 (SetLabels), r4 as the calls before leave it.
  ts::Slot(base, r3, 112, std::nullopt, frame, 0x8223E40C, calls);
  ts::ColorListScreenLoadItem(base, r3, 0, ~uint64_t(0), frame, calls);
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
  uint64_t result = slot(56, frame + 100, frame + 96, 0x8223E458);
  if ((result & 0xFF) != 0) {
    StoreU32(base, frame + 104, 2);
    StoreU32(base, frame + 108, 1);
    slot(52, frame + 108, frame + 104, 0x8223E484);
  }
  StoreU32(base, frame + 116, 1);
  StoreU32(base, frame + 112, 0);
  result = slot(56, frame + 116, frame + 112, 0x8223E4AC);
  if ((result & 0xFF) != 0) return result;
  StoreU32(base, frame + 120, 0);
  StoreU32(base, frame + 124, 1);
  return menu_sounds::PlaySound(base, frame + 124, frame + 120, frame,
                                {const_cast<Calls*>(&calls), &SoundThunk});
}

uint64_t ColorList8SetUpRace(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8223E760 Type8ColorListScreen::SetUpRace");
  namespace rsu = race_setup;
  namespace rs = race_screens;
  const uint32_t s = uint32_t(r3), frame = part_detail::Frame(base, sp, kSetUpRaceFrame);
  const schema::Type8ColorListScreen cl(base, s);
  auto data = [base] { return LoadU32(base, rsu::kRaceData); };
  if (!rsu::RaceDataResetGlobal(base, frame - 112)) {
    pg::Call1(calls, kResetGlobal, r3, frame, 0x8223E780);
  }
  rsu::StagesClear(base, data() + rsu::kStageList);
  StoreU32(base, data() + 0xA3AF0, 4);
  StoreU32(base, data() + 0xA3AF4, 5);
  vehicle_accessors::RaceDataSetWord32(base, rs::ListSelection(base, cl.kind_list_address()));
  // The route base by the item list's +4 plus its selection (the
  // recompiled switch; cases 0 and 9 to 15 are the merged switch piece
  // Type8ColorListScreen::AddRacers sub_8223E804, base 0). Past 15 the
  // recompiled switch traps; never in play.
  const uint32_t index = cl.item_list_scroll() + rs::ListSelection(base, cl.item_list_address());
  static constexpr uint32_t kBases[16] = {0, 2, 4, 6, 7, 8, 10, 12, 13, 0, 0, 0, 0, 0, 0, 0};
  if (index > 15) {
    ITC_LOG_ERROR("Type8ColorListScreen::SetUpRace: item {} past the switch's cases", index);
    __builtin_trap();
  }
  const uint32_t route = kBases[index] + rs::ListSelection(base, cl.course_list_address());
  if (!rsu::RaceSetRoute(base, 0, route, frame - rsu::kRouteFrameBytes)) {
    const uint64_t args[6] = {0, route, 0, 0, 0, 0};
    calls.call_set(kRaceSetRoute, args, 2, frame, 0x8223E864);
  }
  if (!rsu::StagesSetRoute(base, data() + rsu::kStageList, 0, route, kNone,
                           frame - rsu::kRouteFrameBytes)) {
    const uint64_t args[6] = {data() + rsu::kStageList, 0, route, ~uint64_t(0), 0, 0};
    calls.call_set(kStagesSetRoute, args, 4, frame, 0x8223E87C);
  }
  // The traffic (the toggle's first item: 3).
  vehicle_accessors::RaceDataSetWord59618(
      base, rs::ListSelection(base, cl.toggle_address()) == 0 ? 3 : 0);
  // The two players' racer records at the frame's +80: the garage cars,
  // each one's +3892 the list part's slot 11 (IsFirstChoice), its stage
  // word slot 12's (ThirdChoice).
  const double zero = double(std::bit_cast<float>(LoadU32(base, kZero)));
  const uint32_t record = frame + 80;
  const uint64_t part = r3 + kColorListPart;
  for (uint32_t player = 0; player < 2; player++) {
    const uint32_t car = G::garage_cars_address() + kGarageCarBytes * player;
    const uint32_t first =
        uint32_t(ts::Slot(base, part, 44, player, frame, 0x8223E8D0, calls));
    const uint32_t third =
        uint32_t(ts::Slot(base, part, 48, player, frame, 0x8223E8EC, calls));
    rsu::RacerRecordInit(base, record);
    rsu::SetModel(base, record, LoadU32(base, car + 12));
    StoreU32(base, frame + 3972, first);
    if (!native::Buffers_Memcpy(base, record, car, 1884, 0x8223E918)) {
      const uint64_t args[6] = {record, car, 1884, 0, 0, 0};
      calls.call_set(kMemcpy, args, 3, frame, 0x8223E918);
    }
    uint32_t refreshed = 0;
    if (!car_specs::CarRefresh(base, record, &refreshed)) {
      pg::Call1(calls, kCarRefresh, record, frame, 0x8223E920);
    }
    if (rsu::AddRacerNative(base, record)) {
      rsu::AddRacer(base, record, 0, zero, 0);
    } else {
      const uint64_t args[4] = {record, 0, 0, 0};
      calls.call_f1(kAddRacer, args, zero, frame, 0x8223E934);
    }
    rsu::StagesAddRacer(base, data() + rsu::kStageList, player, third, -1);
  }
  vehicle_accessors::StagesSetType(base, data() + rsu::kStageList, 0, kNone);
  uint64_t result;
  if (rsu::PlaceRacersNative(base, frame - 128)) {
    Refresh refresh{&calls, frame - 128 - 4096};
    result = rsu::PlaceRacers(base, frame - 128, RefreshThrough, &refresh);
  } else {
    // r3 not read (Stages::SetType's left).
    const uint64_t args[6] = {0, 0, 0, 0, 0, 0};
    result = calls.call_set(kPlaceRacers, args, 0, frame, 0x8223E974);
  }
  // The screen leaves: +9608 = 10, message 4 posted.
  cl.set_state(10);
  ts::Post(base, uint32_t(r4), 4);
  return result;
}

}  // namespace type8_screens
