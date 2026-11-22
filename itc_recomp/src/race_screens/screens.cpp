// The screens' makers and teardowns, the pause menu's data and the answers
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "alt_hud/alt_hud.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "career/career.h"
#include "guest.h"
#include "schema/career.h"
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

void Frame(uint8_t* base, uint32_t sp, uint32_t bytes) { guest::StoreU32(base, sp - bytes, sp); }

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

using G = schema::RaceScreensGlobals;

// VirtualAlloc(0, size, MEM_COMMIT, read-write) over the allocation layer.
uint32_t Pages(uint32_t size) { return memory::AllocPages(size, 4096); }

// Whether each global's block is the layer's pages (VirtualFree frees it).
bool AllPages(uint8_t* base, std::initializer_list<uint32_t> globals) {
  for (const uint32_t global : globals) {
    if (!memory::IsPages(Get(base, global))) {
      return false;
    }
  }
  return true;
}

// VirtualFree(block, 0, MEM_RELEASE)'s r3 for the layer's pages.
uint64_t Free(uint8_t* base, uint32_t global) {
  return memory::FreePages(Get(base, global)) ? 1 : 0;
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
void ListInit(uint8_t* base, uint32_t list) {
  if (list != 0) {
    const schema::MenuList l(base, list);
    l.set_enabled(0);
    l.set_player(0);
  }
}

// A four-block screen's teardown: when the screen's global is set,
// MenuList::Release(list), then each block freed and its global 0 (screen,
// data, list, pages); else the text.
// `free_data`, when given, frees the data block in host memory (E5 step 3)
// and returns what the original's VirtualFree returned (1), in its place in
// the order.
std::optional<uint64_t> DestroyFour(uint8_t* base, uint32_t sp, uint32_t screen, uint32_t data,
                                    uint32_t pages, uint32_t list, uint32_t text,
                                    uint64_t (*free_data)(uint8_t*) = nullptr) {
  const uint32_t frame = sp - 128;
  if (Get(base, screen) == 0) {
    Frame(base, sp, 128);
    return DebugText(text);
  }
  const bool data_ok = free_data ? memory::IsHandle(Get(base, data)) : AllPages(base, {data});
  if (!data_ok || !AllPages(base, {screen, list, pages}) ||
      !alt_hud::MenuListReleasable(base, Get(base, list))) {
    return std::nullopt;
  }
  Frame(base, sp, 128);
  alt_hud::MenuListRelease(base, Get(base, list), frame);
  uint64_t r3 = 0;
  for (const uint32_t global : {screen, data, list, pages}) {
    r3 = (free_data && global == data) ? free_data(base) : Free(base, global);
    guest::StoreU32(base, global, 0);
  }
  return r3;
}

// The pause menu's data in host memory (PauseMenu::Create): deleted in
// place of the original's VirtualFree at 0x8220C524, which returned 1.
uint64_t FreePauseData(uint8_t* base) {
  G::pause_data(base).Delete();
  return 1;
}

// Kind 4's pause menu's data in host memory (PauseMenu4::Create): deleted in
// place of the original's VirtualFree at 0x8220FCF4, which returned 1.
uint64_t FreePause4Data(uint8_t* base) {
  schema::PauseMenu4Data(base, guest::LoadU32(base, kPause4Data)).Delete();
  return 1;
}

// A one-block screen's teardown: freed and its global 0, or the text.
std::optional<uint64_t> DestroyOne(uint8_t* base, uint32_t sp, uint32_t global, uint32_t text) {
  if (Get(base, global) == 0) {
    Frame(base, sp, 96);
    return DebugText(text);
  }
  if (!AllPages(base, {global})) {
    return std::nullopt;
  }
  Frame(base, sp, 96);
  const uint64_t r3 = Free(base, global);
  guest::StoreU32(base, global, 0);
  return r3;
}

schema::PauseMenuData PauseData(uint8_t* base) { return G::pause_data(base); }

// memcpy natively, or through `calls` where its plain C++ refuses.
void Copy(uint8_t* base, const Calls& calls, uint32_t r1, uint32_t lr, uint32_t destination,
          uint32_t source, uint32_t size) {
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    calls.Call(kMemcpy, r1, lr, destination, source, size);
  }
}

// A float clamped to [0x82000A34 (0.0), upper] as the original compares
// (below the lower bound the lower one, above the upper the upper one, a NaN
// kept), then truncated (fctiwz).
uint32_t ClampTruncate(uint8_t* base, uint32_t address, uint32_t upper) {
  double f0 = Lfs(base, address);
  const double lower = Lfs(base, 0x82000A34);
  if (f0 < lower) {
    f0 = lower;
  } else {
    const double high = Lfs(base, upper);
    if (f0 > high) {
      f0 = high;
    }
  }
  return uint32_t(Fctiwz(f0));
}

}  // namespace

// --- The makers ---

uint64_t PauseMenuCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D7E8 PauseMenu::Create");
  Frame(base, sp, 96);
  G::set_pause_screen(base, Pages(4));
  // The data lives in host memory (E5 step 3): the original's
  // VirtualAlloc(160) at 0x8220D824; the global holds its handle.
  G::set_pause_data(base, schema::PauseMenuData::New(base));
  const uint32_t list = Pages(72);
  G::set_pause_list(base, list);
  ListInit(base, list);
  const uint32_t pages = Pages(28);
  G::set_pause_pages(base, pages);
  PagesInit(base, pages, 0);
  return pages;
}

uint64_t PauseMenu4Create(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210868 PauseMenu4::Create");
  Frame(base, sp, 96);
  guest::StoreU32(base, kPause4Screen, Pages(4));
  // The data lives in host memory (E5 step 3): the original's
  // VirtualAlloc(80) at 0x822108A4; the global holds its handle.
  guest::StoreU32(base, kPause4Data, schema::PauseMenu4Data::New(base).address);
  const uint32_t list = Pages(72);
  guest::StoreU32(base, kPause4List, list);
  ListInit(base, list);
  const uint32_t pages = Pages(28);
  guest::StoreU32(base, kPause4Pages, pages);
  PagesInit(base, pages, 0);
  return pages;
}

uint64_t AfterRaceMenuCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221B1F0 AfterRaceMenu::Create");
  Frame(base, sp, 96);
  G::set_after_screen(base, Pages(4));
  G::set_after_data(base, Pages(24));
  const uint32_t pages = Pages(28);
  G::set_after_pages(base, pages);
  PagesInit(base, pages, 1);
  const uint32_t list = Pages(72);
  G::set_after_list(base, list);
  ListInit(base, list);
  return list;
}

uint64_t RaceDialogCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217D70 RaceDialog::Create");
  Frame(base, sp, 96);
  const uint32_t list = Pages(72);
  G::set_dialog_list(base, list);
  ListInit(base, list);
  const uint32_t pages = Pages(28);
  G::set_dialog_pages(base, pages);
  PagesInit(base, pages, 0);
  const schema::RaceDialogData data(base, Pages(40));
  G::set_dialog_data(base, data);
  data.set_unknown_24(0);
  data.set_message(uint32_t(-1));
  data.set_open_style(uint32_t(-1));
  data.set_next_open_style(uint32_t(-1));
  data.set_showing(0);
  data.set_close_asked(0);
  return data.address;
}

uint64_t CareerResultsCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82219138 CareerResults::Create");
  Frame(base, sp, 96);
  const uint32_t block = Pages(212);
  G::set_career_results(base, block);
  return block;
}

uint64_t RaceResultsCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221F390 RaceResults::Create");
  Frame(base, sp, 96);
  const uint32_t block = Pages(52);
  guest::StoreU32(base, kRaceResults, block);
  return block;
}

uint64_t SplitResultsCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221FA30 SplitResults::Create");
  Frame(base, sp, 96);
  const uint32_t block = Pages(40);
  guest::StoreU32(base, kSplitResults, block);
  return block;
}

uint64_t Kind6MenuCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221BDD0 Kind6Menu::Create");
  Frame(base, sp, 96);
  const uint32_t block = Pages(360);
  guest::StoreU32(base, kKind6Menu, block);
  guest::StoreU32(base, kKind6Word, 0);
  guest::StoreU32(base, block + 340, 1);
  guest::StoreU32(base, Get(base, kKind6Menu) + 344, 0);
  const uint32_t again = Get(base, kKind6Menu);
  guest::StoreU32(base, again + 356, 0);
  return block;
}

// --- The teardowns ---

std::optional<uint64_t> PauseMenuDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220C4C8 PauseMenu::Destroy");
  return DestroyFour(base, sp, G::pause_screen_address(), G::pause_data_address(),
                     G::pause_pages_address(), G::pause_list_address(), kTextBase - 13100,
                     FreePauseData);
}

std::optional<uint64_t> PauseMenu4Destroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220FC98 PauseMenu4::Destroy");
  return DestroyFour(base, sp, kPause4Screen, kPause4Data, kPause4Pages, kPause4List,
                     kTextBase - 12988, FreePause4Data);
}

std::optional<uint64_t> AfterRaceMenuDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221A3E8 AfterRaceMenu::Destroy");
  return DestroyFour(base, sp, G::after_screen_address(), G::after_data_address(),
                     G::after_pages_address(), G::after_list_address(), kTextBase + 1188);
}

std::optional<uint64_t> RaceDialogDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217CD0 RaceDialog::Destroy");
  // No check for an unset global: the three blocks freed, then their
  // globals 0.
  if (!AllPages(base,
                {G::dialog_pages_address(), G::dialog_list_address(), G::dialog_data_address()}) ||
      !alt_hud::MenuListReleasable(base, G::dialog_list(base).address)) {
    return std::nullopt;
  }
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  alt_hud::MenuListRelease(base, G::dialog_list(base).address, frame);
  uint64_t r3 = 0;
  for (const uint32_t global :
       {G::dialog_pages_address(), G::dialog_list_address(), G::dialog_data_address()}) {
    r3 = Free(base, global);
  }
  G::set_dialog_pages(base, 0);
  G::set_dialog_list(base, 0);
  G::set_dialog_data(base, 0);
  return r3;
}

std::optional<uint64_t> CareerResultsDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82219170 CareerResults::Destroy");
  return DestroyOne(base, sp, G::career_results_address(), kTextBase + 924);
}

std::optional<uint64_t> RaceResultsDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221F3C8 RaceResults::Destroy");
  return DestroyOne(base, sp, kRaceResults, kTextBase + 1360);
}

std::optional<uint64_t> SplitResultsDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221FA68 SplitResults::Destroy");
  return DestroyOne(base, sp, kSplitResults, kTextBase + 1420);
}

std::optional<uint64_t> Kind6MenuDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221BE30 Kind6Menu::Destroy");
  return DestroyOne(base, sp, kKind6Menu, kTextBase + 1360);
}

std::optional<uint64_t> AfterRaceMenuClose(uint8_t* base, uint32_t sp) {
  const uint32_t list = G::after_list(base).address;
  if (!alt_hud::MenuListReleasable(base, list)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8221A498 AfterRaceMenu::Close");
  return alt_hud::MenuListRelease(base, list, sp);
}

// --- The pause menu's data and the answers ---

uint64_t PauseMenuLoadData(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220F840 PauseMenu::LoadData");
  const uint32_t frame = sp - 6176;
  Frame(base, sp, 6176);
  // Player 0's save data (race-event.toml's SaveData): its options from
  // +0x1F85C.
  const schema::SaveData options(base, vehicle_accessors::SaveDataOf(base, 0));
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  uint64_t r3 = save;
  // Each store reads [0x826D84D4] after its load, as the original does.
  {
    const uint32_t value = options.hud_option_0();
    PauseData(base).set_hud_option_0(value);
  }
  {
    const uint32_t value = options.hud_option_1();
    PauseData(base).set_hud_option_1(value);
  }
  {
    const uint32_t value = options.hud_option_2();
    PauseData(base).set_hud_option_2(value);
  }
  {
    const schema::PauseMenuData data = PauseData(base);
    data.set_hud_option_3(options.hud_option_3());
  }
  PauseData(base).set_hud_default_0(1);
  PauseData(base).set_hud_default_1(1);
  PauseData(base).set_hud_default_2(1);
  PauseData(base).set_hud_default_3(1);
  auto signed_byte = [](uint8_t byte) { return uint32_t(int32_t(int8_t(byte))); };
  {
    const uint32_t value = signed_byte(options.effects_volume());  // +28
    PauseData(base).set_music_volume_saved(value);
  }
  {
    const uint32_t value = signed_byte(options.music_volume());  // +29
    PauseData(base).set_effects_volume_saved(value);
  }
  PauseData(base).set_music_volume_default(15);
  PauseData(base).set_effects_volume_default(15);
  {
    const uint32_t value = signed_byte(options.setting_0());
    PauseData(base).set_adjust_saved_0(value);
  }
  {
    const uint32_t value = signed_byte(options.setting_1());
    PauseData(base).set_adjust_saved_1(value);
  }
  {
    const uint32_t value = signed_byte(options.setting_2());
    PauseData(base).set_adjust_saved_2(value);
  }
  PauseData(base).set_adjust_default_0(0);
  PauseData(base).set_adjust_default_1(0);
  PauseData(base).set_adjust_default_2(0);
  const int32_t kind = int32_t(PauseData(base).kind());
  if (kind != 0 && kind != 1 && kind != 2) {
    return r3;
  }
  // The career's numbers, from a copy of the save data's stats on the
  // stack (frame + 80) and the player's vehicle.
  // (The stats, +48, are career.toml's CareerSave's: race-event.toml's
  // SaveData has no field there.)
  Copy(base, calls, frame, 0x8220F978, frame + 80, schema::CareerSave(base, save).stats_address(),
       6044);
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, 0);
  {
    const uint32_t value = schema::SaveData(base, save).money();
    PauseData(base).set_money(value);
  }
  {
    const int32_t day = int32_t(schema::SaveData(base, save).day());
    const int32_t week = day / 7;
    PauseData(base).set_weekday(uint32_t(day) - uint32_t(week) * 7);
  }
  {
    int32_t day = int32_t(schema::SaveData(base, save).day());
    if (day > 999) {
      day = 999;
    }
    PauseData(base).set_day(uint32_t(day));
  }
  int32_t streak = 0;
  r3 = career::StatsWinStreak(base, frame + 80);
  const int32_t first = int16_t(uint16_t(r3));
  if (first >= 0) {
    r3 = career::StatsWinStreak(base, frame + 80);
    if (int16_t(uint16_t(r3)) > 999) {
      streak = 999;
    } else {
      r3 = career::StatsWinStreak(base, frame + 80);
      streak = int16_t(uint16_t(r3));
    }
  }
  PauseData(base).set_win_streak(uint32_t(streak));
  {
    const uint32_t value = ClampTruncate(base, vehicle + 6248, kTextBase - 13064);
    PauseData(base).set_car_value_a(value);
  }
  {
    const uint32_t value = ClampTruncate(base, vehicle + 6244, kTextBase - 13068);
    PauseData(base).set_car_value_b(value);
  }
  return r3;
}

uint64_t PauseMenuSaveOptions(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D6A0 PauseMenu::SaveOptions");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  // The options (race-event.toml's SaveData from +0x1F85C; it names the
  // bytes +28 and +29 the other way round from the pause data).
  const schema::SaveData options(base, save);
  options.set_hud_option_0(PauseData(base).hud_option_0());
  options.set_hud_option_1(PauseData(base).hud_option_1());
  options.set_hud_option_2(PauseData(base).hud_option_2());
  options.set_hud_option_3(PauseData(base).hud_option_3());
  options.set_effects_volume(uint8_t(PauseData(base).music_volume_saved()));
  options.set_music_volume(uint8_t(PauseData(base).effects_volume_saved()));
  options.set_setting_0(uint8_t(PauseData(base).adjust_saved_0()));
  options.set_setting_1(uint8_t(PauseData(base).adjust_saved_1()));
  options.set_setting_2(uint8_t(PauseData(base).adjust_saved_2()));
  return save;
}

uint64_t PauseMenuClose(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D890 PauseMenu::Close");
  return PauseMenuSaveOptions(base, sp);
}

uint64_t PauseMenuSaveHudOptions(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D740 PauseMenu::SaveHudOptions");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const schema::SaveData options(base, save);
  options.set_hud_option_0(PauseData(base).hud_edit_0());
  options.set_hud_option_1(PauseData(base).hud_edit_1());
  options.set_hud_option_2(PauseData(base).hud_edit_2());
  options.set_hud_option_3(PauseData(base).hud_edit_3());
  return save;
}

uint32_t PauseMenuAnswerPage(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8220C5D0 PauseMenu::AnswerPage");
  return PauseData(base).answer();
}

uint32_t AfterRaceMenuAnswerPage(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8221A720 AfterRaceMenu::AnswerPage");
  return G::after_data(base).answer();
}

uint32_t RaceDialogAnswer(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82217D40 RaceDialog::Answer");
  G::dialog_data(base).set_showing(0);
  return G::dialog_data(base).yes() == 1 ? 1 : 2;
}

}  // namespace race_screens
