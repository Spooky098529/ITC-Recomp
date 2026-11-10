// The race menu's start (docs/functions/race-menu.md, RaceMenu::Start): see
// race_menu.h.

#include "race_menu/race_menu.h"

#include "base/log.h"
#include "guest.h"
#include "race_menu/internal.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_menu {

namespace {

// Player 0's save data's words.
constexpr uint32_t kSaveName = 8;          // 20 bytes
constexpr uint32_t kSaveDay = 6100;
constexpr uint32_t kSaveMoney = 6116;
constexpr uint32_t kSaveProgress = 0x199E0;

// The floats the original moves onto the nodes (lfs, stfs: copied as words).
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kFloat2228 = 0x82002228;
constexpr uint32_t kFloatA134 = 0x8200A134;
constexpr uint32_t kFloatC34 = 0x82000C34;

constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kSoundPlay = 0x82175850;  // Sound::Play, through its override
constexpr uint32_t kFadeIn = 0x8209D038;     // ScreenFades::FadeIn, through its override
constexpr uint32_t kFadeInLayer = 0x82001BEC;

// The sound manager and its vtable's slots (bytes into it).
constexpr uint32_t kSoundManager = 0x826C5C80;
constexpr uint32_t kStopCueSlot = 52;       // 13
constexpr uint32_t kIsCuePlayingSlot = 56;  // 14

// The sound manager's slot with r4 = the bank's word, r5 = the cue's.
uint64_t CallSlot(uint8_t* base, const Calls& calls, uint32_t slot, uint32_t frame, uint32_t lr,
                  uint32_t bank, uint32_t cue) {
  const uint32_t manager = Load(base, kSoundManager);
  return calls.Call(Load(base, Load(base, manager) + slot), frame, lr, manager, bank, cue);
}

}  // namespace

uint64_t Start(uint8_t* base, uint64_t machine_r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218C9C8 RaceMenu::Start");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t frame = Frame(base, sp, 176);
  Store(base, machine + 9788, 0);
  Store(base, machine + 9644, 3);
  Store(base, machine + 9780, 1);
  Store(base, machine + 9796, 0);
  Store(base, machine + 9812, 0);
  Store(base, machine + 9784, 1);
  guest::StoreU16(base, machine + 9816, 0);
  for (uint32_t field = 9820; field <= 9836; field += 4) {
    Store(base, machine + field, 0);
  }
  Store(base, machine + 9972, 0);
  Store(base, machine + 9976, 0);
  Store(base, machine + 10100, 0);
  Store(base, machine + 10104, 0);
  Store(base, machine + 10108, 0);
  Store(base, machine + 9648, 0);
  Store(base, machine + kPage, 3);

  // Player 0's save data: the progress bits, the name, the money, the day
  // (at most 999) and its weekday (the remainder of a signed division).
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  Store(base, machine + 10144, save);
  Store(base, machine + 10148, Load(base, save + kSaveProgress));
  const int32_t day = int32_t(Load(base, save + kSaveDay));
  for (uint32_t k = 0; k < 20; k += 4) {
    Store(base, machine + 10112 + k, Load(base, save + kSaveName + k));
  }
  Store(base, machine + 10132, Load(base, save + kSaveMoney));
  Store(base, machine + 10136, uint32_t(day > 999 ? 999 : day));
  Store(base, machine + 10140, uint32_t(day - (day / 7) * 7));

  SetUpEvents(base, machine_r3, frame, calls);
  SetEventRange(base, machine);

  const uint32_t m = FindMovie(base, kMovie, frame + 80);
  calls.Call(kResetPose, frame, 0x8218CAE0, m);
  Refresh(base, machine_r3, frame, calls);

  // The nodes' floats, each node's stores in the original's order.
  const uint32_t zero = Load(base, kZero);
  Store(base, ui_movies::FindNode(base, m, 19, 0) + 16, zero);
  Store(base, ui_movies::FindNode(base, m, 20, 0) + 16, zero);
  Store(base, ui_movies::FindNode(base, m, 8, 0) + 16, zero);
  for (const uint32_t child : {2u, 3u, 4u}) {
    const uint32_t node = ui_movies::FindNode(base, m, 11, child);
    Store(base, node + 12, Load(base, kFloat2228));
    Store(base, node + 16, Load(base, kFloatA134));
  }
  uint32_t node = ui_movies::FindNode(base, m, 11, 0);
  Store(base, node + 16, zero);
  Store(base, node + 12, Load(base, kFloatC34));
  node = ui_movies::FindNode(base, m, 11, 1);
  Store(base, node + 12, Load(base, kFloatC34));
  Store(base, node + 16, zero);

  // The badges (11, 20) to (11, 23) hidden, then shown by the progress bits.
  static constexpr uint32_t kHideLr[] = {0x8218CBE8, 0x8218CBF8, 0x8218CC08, 0x8218CC18};
  for (uint32_t k = 0; k < 4; k++) {
    calls.Call(kHideNode, frame, kHideLr[k], m, 11, 20 + k);
  }
  const uint32_t bits = Load(base, machine + 10148);
  if ((bits & 0x20) != 0 || (bits & 0x4) != 0 || (bits & 0x8) != 0) {
    if ((bits & 0x20) != 0) {
      calls.Call(kShowNode, frame, 0x8218CC34, m, 11, 20);
    }
    calls.Call(kShowNode, frame, 0x8218CC44, m, 11, 21);
    calls.Call(kShowNode, frame, 0x8218CC54, m, 11, 22);
    calls.Call(kShowNode, frame, 0x8218CC64, m, 11, 23);
  } else if ((bits & 0x10) != 0) {
    calls.Call(kShowNode, frame, 0x8218CE00, m, 11, 21);
    calls.Call(kShowNode, frame, 0x8218CC64, m, 11, 22);
  } else {
    calls.Call(kShowNode, frame, 0x8218CC64, m, 11, 22);
  }
  node = ui_movies::FindNode(base, m, 10, 0);
  Store(base, node + 12, zero);
  Store(base, node + 16, zero);

  // The page object on page 3, the menu.
  uint32_t pages = Load(base, kPages);
  uint32_t page = Load(base, machine + kPage);
  Store(base, pages + 4, page);
  Store(base, pages + 0, page);
  Store(base, pages + 8, page);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  Store(base, pages + 24, 0);
  const uint32_t first = Load(base, machine + 9648);
  const uint32_t menu = Load(base, kMenu);
  for (const uint32_t field : {12u, 16u, 20u, 8u, 44u, 56u}) {
    Store(base, menu + field, 0);
  }
  Store(base, menu + 60, 8);
  Store(base, menu + 0, first);
  Store(base, menu + 4, first);
  for (const uint32_t field : {36u, 64u, 24u, 28u}) {
    Store(base, menu + field, 0);
  }
  // Then the page object again: transition 0, then page 3.
  pages = Load(base, kPages);
  page = Load(base, machine + kPage);
  const uint32_t old = Load(base, pages + 0);
  if (page != old) {
    Store(base, pages + 8, old);
  }
  Store(base, pages + 12, 0);
  Store(base, pages + 16, kNone);
  Store(base, pages + 4, page);
  Store(base, pages + 0, kNone);

  // The hub's music (cue 2 of bank 1) stopped when it plays; the menu's (cue
  // 0) played unless it plays.
  Store(base, frame + 88, 1);
  Store(base, frame + 84, 2);
  uint64_t r3 = CallSlot(base, calls, kIsCuePlayingSlot, frame, 0x8218CD3C, frame + 88, frame + 84);
  if (uint8_t(r3) != 0) {
    Store(base, frame + 92, 2);
    Store(base, frame + 96, 1);
    CallSlot(base, calls, kStopCueSlot, frame, 0x8218CD68, frame + 96, frame + 92);
  }
  Store(base, frame + 100, 0);
  Store(base, frame + 104, 1);
  r3 = CallSlot(base, calls, kIsCuePlayingSlot, frame, 0x8218CD8C, frame + 104, frame + 100);
  if (uint8_t(r3) == 0) {
    Store(base, frame + 80, 0);
    Store(base, frame + 108, 1);
    calls.Call(kSoundPlay, frame, 0x8218CDA8, frame + 108, frame + 80);
  }
  return calls.Call(kFadeIn, frame, 0x8218CDC4, Load(base, kFadeInLayer), 0, 0, 0);
}

}  // namespace race_menu
