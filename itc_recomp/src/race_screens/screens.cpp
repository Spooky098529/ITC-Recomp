// The screens' makers and teardowns, the pause menu's data and the answers
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "alt_hud/alt_hud.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "career/career.h"
#include "guest.h"
#include "helpers/debug_output.h"
#include "memory/heap.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <initializer_list>

namespace race_screens {

namespace {

using namespace camera_modes::steps;

void Frame(uint8_t* base, uint32_t sp, uint32_t bytes) { guest::StoreU32(base, sp - bytes, sp); }

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

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
    guest::StoreU32(base, list + 68, 0);
    guest::StoreU32(base, list + 60, 0);
  }
}

// A four-block screen's teardown: when the screen's global is set,
// MenuList::Release(list), then each block freed and its global 0 (screen,
// data, list, pages); else the text.
std::optional<uint64_t> DestroyFour(uint8_t* base, uint32_t sp, uint32_t screen, uint32_t data,
                                    uint32_t pages, uint32_t list, uint32_t text) {
  const uint32_t frame = sp - 128;
  if (Get(base, screen) == 0) {
    Frame(base, sp, 128);
    return DebugText(text);
  }
  if (!AllPages(base, {screen, data, list, pages}) ||
      !alt_hud::MenuListReleasable(base, Get(base, list))) {
    return std::nullopt;
  }
  Frame(base, sp, 128);
  alt_hud::MenuListRelease(base, Get(base, list), frame);
  uint64_t r3 = 0;
  for (const uint32_t global : {screen, data, list, pages}) {
    r3 = Free(base, global);
    guest::StoreU32(base, global, 0);
  }
  return r3;
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

uint32_t PauseData(uint8_t* base) { return Get(base, kPauseData); }

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
  guest::StoreU32(base, kPauseScreen, Pages(4));
  guest::StoreU32(base, kPauseData, Pages(160));
  const uint32_t list = Pages(72);
  guest::StoreU32(base, kPauseList, list);
  ListInit(base, list);
  const uint32_t pages = Pages(28);
  guest::StoreU32(base, kPausePages, pages);
  PagesInit(base, pages, 0);
  return pages;
}

uint64_t PauseMenu4Create(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210868 PauseMenu4::Create");
  Frame(base, sp, 96);
  guest::StoreU32(base, kPause4Screen, Pages(4));
  guest::StoreU32(base, kPause4Data, Pages(80));
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
  guest::StoreU32(base, kAfterScreen, Pages(4));
  guest::StoreU32(base, kAfterData, Pages(24));
  const uint32_t pages = Pages(28);
  guest::StoreU32(base, kAfterPages, pages);
  PagesInit(base, pages, 1);
  const uint32_t list = Pages(72);
  guest::StoreU32(base, kAfterList, list);
  ListInit(base, list);
  return list;
}

uint64_t RaceDialogCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217D70 RaceDialog::Create");
  Frame(base, sp, 96);
  const uint32_t list = Pages(72);
  guest::StoreU32(base, kDialogList, list);
  ListInit(base, list);
  const uint32_t pages = Pages(28);
  guest::StoreU32(base, kDialogPages, pages);
  PagesInit(base, pages, 0);
  const uint32_t data = Pages(40);
  guest::StoreU32(base, kDialogData, data);
  guest::StoreU32(base, data + 24, 0);
  guest::StoreU32(base, data + 12, uint32_t(-1));
  guest::StoreU32(base, data + 16, uint32_t(-1));
  guest::StoreU32(base, data + 20, uint32_t(-1));
  guest::StoreU32(base, data + 32, 0);
  guest::StoreU32(base, data + 36, 0);
  return data;
}

uint64_t CareerResultsCreate(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82219138 CareerResults::Create");
  Frame(base, sp, 96);
  const uint32_t block = Pages(212);
  guest::StoreU32(base, kCareerResults, block);
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
  return DestroyFour(base, sp, kPauseScreen, kPauseData, kPausePages, kPauseList,
                     kTextBase - 13100);
}

std::optional<uint64_t> PauseMenu4Destroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220FC98 PauseMenu4::Destroy");
  return DestroyFour(base, sp, kPause4Screen, kPause4Data, kPause4Pages, kPause4List,
                     kTextBase - 12988);
}

std::optional<uint64_t> AfterRaceMenuDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8221A3E8 AfterRaceMenu::Destroy");
  return DestroyFour(base, sp, kAfterScreen, kAfterData, kAfterPages, kAfterList,
                     kTextBase + 1188);
}

std::optional<uint64_t> RaceDialogDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82217CD0 RaceDialog::Destroy");
  // No check for an unset global: the three blocks freed, then their
  // globals 0.
  if (!AllPages(base, {kDialogPages, kDialogList, kDialogData}) ||
      !alt_hud::MenuListReleasable(base, Get(base, kDialogList))) {
    return std::nullopt;
  }
  const uint32_t frame = sp - 112;
  Frame(base, sp, 112);
  alt_hud::MenuListRelease(base, Get(base, kDialogList), frame);
  uint64_t r3 = 0;
  for (const uint32_t global : {kDialogPages, kDialogList, kDialogData}) {
    r3 = Free(base, global);
  }
  guest::StoreU32(base, kDialogPages, 0);
  guest::StoreU32(base, kDialogList, 0);
  guest::StoreU32(base, kDialogData, 0);
  return r3;
}

std::optional<uint64_t> CareerResultsDestroy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82219170 CareerResults::Destroy");
  return DestroyOne(base, sp, kCareerResults, kTextBase + 924);
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
  const uint32_t list = Get(base, kAfterList);
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
  const uint32_t options = vehicle_accessors::SaveDataOf(base, 0) + 0x1F85C;
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  uint64_t r3 = save;
  guest::StoreU32(base, PauseData(base) + 0, guest::LoadU32(base, options + 0));
  guest::StoreU32(base, PauseData(base) + 4, guest::LoadU32(base, options + 4));
  guest::StoreU32(base, PauseData(base) + 8, guest::LoadU32(base, options + 8));
  {
    const uint32_t data = PauseData(base);
    guest::StoreU32(base, data + 12, guest::LoadU32(base, options + 12));
  }
  for (const uint32_t field : {72u, 76u, 80u, 84u}) {
    guest::StoreU32(base, PauseData(base) + field, 1);
  }
  auto signed_byte = [&](uint32_t offset) {
    return uint32_t(int32_t(int8_t(guest::LoadU8(base, options + offset))));
  };
  {
    const uint32_t value = signed_byte(28);
    guest::StoreU32(base, PauseData(base) + 16, value);
  }
  {
    const uint32_t value = signed_byte(29);
    guest::StoreU32(base, PauseData(base) + 20, value);
  }
  guest::StoreU32(base, PauseData(base) + 88, 15);
  guest::StoreU32(base, PauseData(base) + 92, 15);
  for (const auto& [offset, field] : {std::pair{36u, 24u}, {37u, 28u}, {38u, 32u}}) {
    const uint32_t value = signed_byte(offset);
    guest::StoreU32(base, PauseData(base) + field, value);
  }
  for (const uint32_t field : {96u, 100u, 104u}) {
    guest::StoreU32(base, PauseData(base) + field, 0);
  }
  const int32_t kind = int32_t(guest::LoadU32(base, PauseData(base) + 132));
  if (kind != 0 && kind != 1 && kind != 2) {
    return r3;
  }
  // The career's numbers, from a copy of the save data's stats on the
  // stack (frame + 80) and the player's vehicle.
  Copy(base, calls, frame, 0x8220F978, frame + 80, save + 48, 6044);
  const uint32_t vehicle = vehicle_accessors::VehicleGet(base, 0);
  {
    const uint32_t value = guest::LoadU32(base, save + 6116);
    guest::StoreU32(base, PauseData(base) + 108, value);
  }
  {
    const int32_t day = int32_t(guest::LoadU32(base, save + 6100));
    const int32_t week = day / 7;
    guest::StoreU32(base, PauseData(base) + 112, uint32_t(day) - uint32_t(week) * 7);
  }
  {
    int32_t day = int32_t(guest::LoadU32(base, save + 6100));
    if (day > 999) {
      day = 999;
    }
    guest::StoreU32(base, PauseData(base) + 116, uint32_t(day));
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
  guest::StoreU32(base, PauseData(base) + 120, uint32_t(streak));
  {
    const uint32_t value = ClampTruncate(base, vehicle + 6248, kTextBase - 13064);
    guest::StoreU32(base, PauseData(base) + 124, value);
  }
  {
    const uint32_t value = ClampTruncate(base, vehicle + 6244, kTextBase - 13068);
    guest::StoreU32(base, PauseData(base) + 128, value);
  }
  return r3;
}

uint64_t PauseMenuSaveOptions(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220D6A0 PauseMenu::SaveOptions");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t options = save + 0x1F85C;
  for (const uint32_t field : {0u, 4u, 8u, 12u}) {
    guest::StoreU32(base, options + field, guest::LoadU32(base, PauseData(base) + field));
  }
  for (const auto& [field, offset] :
       {std::pair{16u, 28u}, {20u, 29u}, {24u, 36u}, {28u, 37u}, {32u, 38u}}) {
    guest::StoreU8(base, options + offset, uint8_t(guest::LoadU32(base, PauseData(base) + field)));
  }
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
  const uint32_t options = save + 0x1F85C;
  for (const uint32_t k : {0u, 4u, 8u, 12u}) {
    guest::StoreU32(base, options + k, guest::LoadU32(base, PauseData(base) + 36 + k));
  }
  return save;
}

uint32_t PauseMenuAnswerPage(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8220C5D0 PauseMenu::AnswerPage");
  return guest::LoadU32(base, PauseData(base) + 140);
}

uint32_t AfterRaceMenuAnswerPage(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8221A720 AfterRaceMenu::AnswerPage");
  return guest::LoadU32(base, Get(base, kAfterData) + 12);
}

uint32_t RaceDialogAnswer(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82217D40 RaceDialog::Answer");
  guest::StoreU32(base, Get(base, kDialogData) + 32, 0);
  return guest::LoadU32(base, Get(base, kDialogData) + 0) == 1 ? 1 : 2;
}

}  // namespace race_screens
