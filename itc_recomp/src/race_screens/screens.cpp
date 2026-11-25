// The screens' makers and teardowns, the pause menu's data and the answers
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "alt_hud/alt_hud.h"
#include "base/log.h"
#include "camera_modes/steps.h"
#include "career/career.h"
#include "guest.h"
#include "schema/career.h"
#include "schema/menu_helpers.h"
#include "schema/race_event.h"
#include "schema/race_screens.h"
#include "schema/race_screens_unseen.h"
#include "helpers/debug_output.h"
#include "memory/handles.h"
#include "memory/heap.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <initializer_list>

namespace race_screens {

namespace {

using namespace camera_modes::steps;

uint32_t Get(uint32_t global) { return guest::LoadU32(global); }

using G = schema::RaceScreensGlobals;

// VirtualAlloc(0, size, MEM_COMMIT, read-write) over the allocation layer.
uint32_t Pages(uint32_t size) { return memory::AllocPages(size, 4096); }

// Whether each global's block is the layer's pages or a host object's handle
// (E5 step 4: VirtualFree frees either, memory::FreePages).
bool AllPages(std::initializer_list<uint32_t> globals) {
  for (const uint32_t global : globals) {
    const uint32_t block = Get(global);
    if (!memory::IsPages(block) && !memory::IsHandle(block)) {
      return false;
    }
  }
  return true;
}

// VirtualFree(block, 0, MEM_RELEASE)'s r3 for the layer's pages.
uint64_t Free(uint32_t global) {
  return memory::FreePages(Get(global)) ? 1 : 0;
}

// OutputDebugStringA(text): discarded (helpers::DebugOutput); r3 the text,
// sign-extended as the original's lis and addi leave it.
uint64_t DebugText(uint32_t text) {
  helpers::DebugOutput(nullptr);
  return uint64_t(int64_t(int32_t(text)));
}

// The texts the teardowns print when their screen's global is 0.
constexpr uint32_t kTextBase = 0x82010000;

// A 4-byte screen, its data, its page object (ScreenPages::Init(.., page))
// and its menu list (+68 and +60 = 0), in the order `order` gives: the
// pause menus' (screen, data, list, pages) and the menu after the race's
// (screen, data, pages, list).
void ListInit(uint32_t list) {
  if (list != 0) {
    const schema::MenuList l(list);
    l.set_enabled(0);
    l.set_player(0);
  }
}

// A four-block screen's teardown: when the screen's global is set,
// MenuList::Release(list), then each block freed and its global 0 (screen,
// data, list, pages); else the text.
// `free_data` and `free_screen`, when given, free the data or screen block
// in host memory (E5 step 3) and return what the original's VirtualFree
// returned (1), in its place in the order.
std::optional<uint64_t> DestroyFour(uint32_t sp, uint32_t screen, uint32_t data,
                                    uint32_t pages, uint32_t list, uint32_t text,
                                    uint64_t (*free_data)() = nullptr,
                                    uint64_t (*free_screen)() = nullptr) {
  // The original's 128-byte frame (only its back chain) left out: no guest
  // call is made from it (MenuList::Release has no frame of its own; E5
  // step 6).
  if (Get(screen) == 0) {
    return DebugText(text);
  }
  const bool data_ok = free_data ? memory::IsHandle(Get(data)) : AllPages({data});
  const bool screen_ok =
      free_screen ? memory::IsHandle(Get(screen)) : AllPages({screen});
  if (!data_ok || !screen_ok || !AllPages({list, pages}) ||
      !alt_hud::MenuListReleasable(Get(list))) {
    return std::nullopt;
  }
  alt_hud::MenuListRelease(Get(list), sp);
  uint64_t r3 = 0;
  for (const uint32_t global : {screen, data, list, pages}) {
    r3 = (free_data && global == data)       ? free_data()
         : (free_screen && global == screen) ? free_screen()
                                             : Free(global);
    guest::StoreU32(global, 0);
  }
  return r3;
}

// The pause menu's data in host memory (PauseMenu::Create): deleted in
// place of the original's VirtualFree at 0x8220C524, which returned 1.
uint64_t FreePauseData() {
  G::pause_data().Delete();
  return 1;
}

// The pause menu's screen in host memory (PauseMenu::Create): deleted in
// place of the original's VirtualFree at 0x8220C508, which returned 1.
uint64_t FreePauseScreen() {
  G::pause_screen().Delete();
  return 1;
}

// Kind 4's pause menu's data in host memory (PauseMenu4::Create): deleted in
// place of the original's VirtualFree at 0x8220FCF4, which returned 1.
uint64_t FreePause4Data() {
  schema::PauseMenu4Data(guest::LoadU32(kPause4Data)).Delete();
  return 1;
}

// A one-block screen's teardown: freed and its global 0, or the text.
// The original's 96-byte frame (only its back chain) left out: no guest
// call is made from it (E5 step 6).
std::optional<uint64_t> DestroyOne([[maybe_unused]] uint32_t sp, uint32_t global,
                                   uint32_t text) {
  if (Get(global) == 0) {
    return DebugText(text);
  }
  if (!AllPages({global})) {
    return std::nullopt;
  }
  const uint64_t r3 = Free(global);
  guest::StoreU32(global, 0);
  return r3;
}

schema::PauseMenuData PauseData() { return G::pause_data(); }

// A float clamped to [0x82000A34 (0.0), upper] as the original compares
// (below the lower bound the lower one, above the upper the upper one, a NaN
// kept), then truncated (fctiwz).
uint32_t ClampTruncate(uint32_t address, uint32_t upper) {
  double f0 = Lfs(address);
  const double lower = Lfs(0x82000A34);
  if (f0 < lower) {
    f0 = lower;
  } else {
    const double high = Lfs(upper);
    if (f0 > high) {
      f0 = high;
    }
  }
  return uint32_t(Fctiwz(f0));
}

}  // namespace

// --- The makers ---

uint64_t PauseMenuCreate(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D7E8 PauseMenu::Create");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  // The screen and the data live in host memory (E5 step 3): the original's
  // VirtualAlloc(4) at 0x8220D808; the global holds its handle.
  G::set_pause_screen(schema::PauseMenuScreen::New());
  // The data lives in host memory (E5 step 3): the original's
  // VirtualAlloc(160) at 0x8220D824; the global holds its handle.
  G::set_pause_data(schema::PauseMenuData::New());
  const uint32_t list = schema::MenuList::New().address;  // host (E5 step 5): VirtualAlloc(72)
  G::set_pause_list(list);
  ListInit(list);
  const uint32_t pages = schema::PageObject::New().address;  // host (E5 step 5): VirtualAlloc(28)
  G::set_pause_pages(pages);
  PagesInit(pages, 0);
  return pages;
}

uint64_t PauseMenu4Create(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210868 PauseMenu4::Create");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  guest::StoreU32(kPause4Screen, Pages(4));
  // The data lives in host memory (E5 step 3): the original's
  // VirtualAlloc(80) at 0x822108A4; the global holds its handle.
  guest::StoreU32(kPause4Data, schema::PauseMenu4Data::New().address);
  const uint32_t list = schema::MenuList::New().address;  // host (E5 step 5): VirtualAlloc(72)
  guest::StoreU32(kPause4List, list);
  ListInit(list);
  const uint32_t pages = schema::PageObject::New().address;  // host (E5 step 5): VirtualAlloc(28)
  guest::StoreU32(kPause4Pages, pages);
  PagesInit(pages, 0);
  return pages;
}

uint64_t AfterRaceMenuCreate(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221B1F0 AfterRaceMenu::Create");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  // The screen and the data live in host memory (E5 step 4): the original's
  // VirtualAlloc(4) at 0x8221B210 and VirtualAlloc(24) at 0x8221B22C; the
  // globals hold their handles.
  G::set_after_screen(schema::AfterRaceMenuScreen::New());
  G::set_after_data(schema::AfterRaceMenuData::New());
  const uint32_t pages = schema::PageObject::New().address;  // host (E5 step 5): VirtualAlloc(28)
  G::set_after_pages(pages);
  PagesInit(pages, 1);
  const uint32_t list = schema::MenuList::New().address;  // host (E5 step 5): VirtualAlloc(72)
  G::set_after_list(list);
  ListInit(list);
  return list;
}

uint64_t RaceDialogCreate(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217D70 RaceDialog::Create");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  const uint32_t list = schema::MenuList::New().address;  // host (E5 step 5): VirtualAlloc(72)
  G::set_dialog_list(list);
  ListInit(list);
  const uint32_t pages = schema::PageObject::New().address;  // host (E5 step 5): VirtualAlloc(28)
  G::set_dialog_pages(pages);
  PagesInit(pages, 0);
  // The data lives in host memory (E5 step 4): the original's
  // VirtualAlloc(40) at 0x82217DE8; the global holds its handle.
  const schema::RaceDialogData data = schema::RaceDialogData::New();
  G::set_dialog_data(data);
  data.set_unknown_24(0);
  data.set_message(uint32_t(-1));
  data.set_open_style(uint32_t(-1));
  data.set_next_open_style(uint32_t(-1));
  data.set_showing(0);
  data.set_close_asked(0);
  return data.address;
}

uint64_t CareerResultsCreate(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82219138 CareerResults::Create");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  // The screen lives in host memory (E5 step 4): the original's
  // VirtualAlloc(212) at 0x82219158; the global holds its handle, freed by
  // Destroy's VirtualFree (memory::FreePages takes handles).
  const schema::CareerResultsScreen block = schema::CareerResultsScreen::New();
  G::set_career_results(block);
  return block.address;
}

uint64_t RaceResultsCreate(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221F390 RaceResults::Create");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  const uint32_t block = Pages(52);
  guest::StoreU32(kRaceResults, block);
  return block;
}

uint64_t SplitResultsCreate(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221FA30 SplitResults::Create");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  const uint32_t block = Pages(40);
  guest::StoreU32(kSplitResults, block);
  return block;
}

uint64_t Kind6MenuCreate(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221BDD0 Kind6Menu::Create");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  const uint32_t block = Pages(360);
  guest::StoreU32(kKind6Menu, block);
  guest::StoreU32(kKind6Word, 0);
  guest::StoreU32(block + 340, 1);
  guest::StoreU32(Get(kKind6Menu) + 344, 0);
  const uint32_t again = Get(kKind6Menu);
  guest::StoreU32(again + 356, 0);
  return block;
}

// --- The teardowns ---

std::optional<uint64_t> PauseMenuDestroy(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220C4C8 PauseMenu::Destroy");
  return DestroyFour(sp, G::pause_screen_address(), G::pause_data_address(),
                     G::pause_pages_address(), G::pause_list_address(), kTextBase - 13100,
                     FreePauseData, FreePauseScreen);
}

std::optional<uint64_t> PauseMenu4Destroy(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220FC98 PauseMenu4::Destroy");
  return DestroyFour(sp, kPause4Screen, kPause4Data, kPause4Pages, kPause4List,
                     kTextBase - 12988, FreePause4Data);
}

std::optional<uint64_t> AfterRaceMenuDestroy(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221A3E8 AfterRaceMenu::Destroy");
  return DestroyFour(sp, G::after_screen_address(), G::after_data_address(),
                     G::after_pages_address(), G::after_list_address(), kTextBase + 1188);
}

std::optional<uint64_t> RaceDialogDestroy(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217CD0 RaceDialog::Destroy");
  // No check for an unset global: the three blocks freed, then their
  // globals 0.
  if (!AllPages({G::dialog_pages_address(), G::dialog_list_address(), G::dialog_data_address()}) ||
      !alt_hud::MenuListReleasable(G::dialog_list().address)) {
    return std::nullopt;
  }
  // The original's 112-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  alt_hud::MenuListRelease(G::dialog_list().address, sp);
  uint64_t r3 = 0;
  for (const uint32_t global :
       {G::dialog_pages_address(), G::dialog_list_address(), G::dialog_data_address()}) {
    r3 = Free(global);
  }
  G::set_dialog_pages(0);
  G::set_dialog_list(0);
  G::set_dialog_data(0);
  return r3;
}

std::optional<uint64_t> CareerResultsDestroy(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82219170 CareerResults::Destroy");
  return DestroyOne(sp, G::career_results_address(), kTextBase + 924);
}

std::optional<uint64_t> RaceResultsDestroy(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221F3C8 RaceResults::Destroy");
  return DestroyOne(sp, kRaceResults, kTextBase + 1360);
}

std::optional<uint64_t> SplitResultsDestroy(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221FA68 SplitResults::Destroy");
  return DestroyOne(sp, kSplitResults, kTextBase + 1420);
}

std::optional<uint64_t> Kind6MenuDestroy(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221BE30 Kind6Menu::Destroy");
  return DestroyOne(sp, kKind6Menu, kTextBase + 1360);
}

std::optional<uint64_t> AfterRaceMenuClose(uint32_t sp) {
  const uint32_t list = G::after_list().address;
  if (!alt_hud::MenuListReleasable(list)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8221A498 AfterRaceMenu::Close");
  return alt_hud::MenuListRelease(list, sp);
}

// --- The pause menu's data and the answers ---

uint64_t PauseMenuLoadData(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220F840 PauseMenu::LoadData");
  // The original's 6,176-byte frame left out (E5 step 6): it held only the
  // stats' copy (below), and no guest call is made from it.
  (void)sp;
  (void)calls;
  // Player 0's save data (race-event.toml's SaveData): its options from
  // +0x1F85C.
  const schema::SaveData options(vehicle_accessors::SaveDataOf(0));
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  uint64_t r3 = save;
  // Each store reads [0x826D84D4] after its load, as the original does.
  {
    const uint32_t value = options.hud_option_0();
    PauseData().set_hud_option_0(value);
  }
  {
    const uint32_t value = options.hud_option_1();
    PauseData().set_hud_option_1(value);
  }
  {
    const uint32_t value = options.hud_option_2();
    PauseData().set_hud_option_2(value);
  }
  {
    const schema::PauseMenuData data = PauseData();
    data.set_hud_option_3(options.hud_option_3());
  }
  PauseData().set_hud_default_0(1);
  PauseData().set_hud_default_1(1);
  PauseData().set_hud_default_2(1);
  PauseData().set_hud_default_3(1);
  auto signed_byte = [](uint8_t byte) { return uint32_t(int32_t(int8_t(byte))); };
  {
    const uint32_t value = signed_byte(options.effects_volume());  // +28
    PauseData().set_music_volume_saved(value);
  }
  {
    const uint32_t value = signed_byte(options.music_volume());  // +29
    PauseData().set_effects_volume_saved(value);
  }
  PauseData().set_music_volume_default(15);
  PauseData().set_effects_volume_default(15);
  {
    const uint32_t value = signed_byte(options.setting_0());
    PauseData().set_adjust_saved_0(value);
  }
  {
    const uint32_t value = signed_byte(options.setting_1());
    PauseData().set_adjust_saved_1(value);
  }
  {
    const uint32_t value = signed_byte(options.setting_2());
    PauseData().set_adjust_saved_2(value);
  }
  PauseData().set_adjust_default_0(0);
  PauseData().set_adjust_default_1(0);
  PauseData().set_adjust_default_2(0);
  const int32_t kind = int32_t(PauseData().kind());
  if (kind != 0 && kind != 1 && kind != 2) {
    return r3;
  }
  // The career's numbers, from the save data's stats and the player's
  // vehicle. (The stats, +48, are career.toml's CareerSave's:
  // race-event.toml's SaveData has no field there.) The original copied the
  // stats' 6,044 bytes to its frame's +80 (memcpy, return address
  // 0x8220F978) and read the win streak from the copy three times; nothing
  // writes the stats in between, so they're read in place (E5 step 6).
  const uint32_t stats = schema::CareerSave(save).stats_address();
  const uint32_t vehicle = vehicle_accessors::VehicleGet(0);
  {
    const uint32_t value = schema::SaveData(save).money();
    PauseData().set_money(value);
  }
  {
    const int32_t day = int32_t(schema::SaveData(save).day());
    const int32_t week = day / 7;
    PauseData().set_weekday(uint32_t(day) - uint32_t(week) * 7);
  }
  {
    int32_t day = int32_t(schema::SaveData(save).day());
    if (day > 999) {
      day = 999;
    }
    PauseData().set_day(uint32_t(day));
  }
  int32_t streak = 0;
  r3 = career::StatsWinStreak(stats);
  const int32_t first = int16_t(uint16_t(r3));
  if (first >= 0) {
    r3 = career::StatsWinStreak(stats);
    if (int16_t(uint16_t(r3)) > 999) {
      streak = 999;
    } else {
      r3 = career::StatsWinStreak(stats);
      streak = int16_t(uint16_t(r3));
    }
  }
  PauseData().set_win_streak(uint32_t(streak));
  {
    const uint32_t value = ClampTruncate(vehicle + 6248, kTextBase - 13064);
    PauseData().set_car_value_a(value);
  }
  {
    const uint32_t value = ClampTruncate(vehicle + 6244, kTextBase - 13068);
    PauseData().set_car_value_b(value);
  }
  return r3;
}

uint64_t PauseMenuSaveOptions(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D6A0 PauseMenu::SaveOptions");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  // The options (race-event.toml's SaveData from +0x1F85C; it names the
  // bytes +28 and +29 the other way round from the pause data).
  const schema::SaveData options(save);
  options.set_hud_option_0(PauseData().hud_option_0());
  options.set_hud_option_1(PauseData().hud_option_1());
  options.set_hud_option_2(PauseData().hud_option_2());
  options.set_hud_option_3(PauseData().hud_option_3());
  options.set_effects_volume(uint8_t(PauseData().music_volume_saved()));
  options.set_music_volume(uint8_t(PauseData().effects_volume_saved()));
  options.set_setting_0(uint8_t(PauseData().adjust_saved_0()));
  options.set_setting_1(uint8_t(PauseData().adjust_saved_1()));
  options.set_setting_2(uint8_t(PauseData().adjust_saved_2()));
  return save;
}

uint64_t PauseMenuClose(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D890 PauseMenu::Close");
  return PauseMenuSaveOptions(sp);
}

uint64_t PauseMenuSaveHudOptions(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D740 PauseMenu::SaveHudOptions");
  // The original's 96-byte frame (only its back chain) left out: no guest
  // call is made from it (E5 step 6).
  (void)sp;
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  const schema::SaveData options(save);
  options.set_hud_option_0(PauseData().hud_edit_0());
  options.set_hud_option_1(PauseData().hud_edit_1());
  options.set_hud_option_2(PauseData().hud_edit_2());
  options.set_hud_option_3(PauseData().hud_edit_3());
  return save;
}

uint32_t PauseMenuAnswerPage() {
  ITC_NATIVE_RUNNING("sub_8220C5D0 PauseMenu::AnswerPage");
  return PauseData().answer();
}

uint32_t AfterRaceMenuAnswerPage() {
  ITC_NATIVE_RUNNING("sub_8221A720 AfterRaceMenu::AnswerPage");
  return G::after_data().answer();
}

uint32_t RaceDialogAnswer() {
  ITC_NATIVE_RUNNING("sub_82217D40 RaceDialog::Answer");
  G::dialog_data().set_showing(0);
  return G::dialog_data().yes() == 1 ? 1 : 2;
}

}  // namespace race_screens
