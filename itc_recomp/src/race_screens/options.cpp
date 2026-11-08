// The pause menu's main and options pages (docs/functions/race-screens.md):
// see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t PauseMovie(uint8_t* base) { return Get(base, Get(base, kPauseScreen)); }

// Four words copied from `from` to `to`, one at a time.
void CopyFour(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t k = 0; k < 16; k += 4) {
    guest::StoreU32(base, to + k, guest::LoadU32(base, from + k));
  }
}

// UiMovie::SetNodeText(movie, node, id) through its override, its varargs 0.
uint64_t NodeText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint32_t node,
                  uint64_t id) {
  return calls.Call(kSetNodeText, frame, lr, movie, node, id);
}

}  // namespace

uint64_t PauseMenuMainPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220D898 PauseMenu::MainPage");
  const uint32_t frame = Frame(base, sp, 208);
  // The debug texts (the kind's title, the items, the cursor at the list's
  // selection) go to the empty sub_82106540 (audio.md): only their loads,
  // which change nothing, are left out with them.
  calls.Call(kListUpdate, frame, 0x8220D970, Get(base, kPauseList));
  const uint32_t selection = ListSelection(base, Get(base, kPauseList));
  // MenuList::GetPreviousSelection (sub_8220BC28): the word at +20.
  const uint32_t before = guest::LoadU32(base, Get(base, kPauseList) + kListSelectionBefore);
  if (selection != before) {
    calls.Call(kPagesGo, frame, 0x8220D9A4, Get(base, kPausePages), 0, 4, kNone);
    calls.Call(kMenuSoundPlay, frame, 0x8220D9AC, 4);
  } else if (uint32_t(calls.Call(kListPressed152, frame, 0x8220D9B8, Get(base, kPauseList))) != 0 ||
             uint32_t(calls.Call(kListCancelled, frame, 0x8220D9C8, Get(base, kPauseList))) != 0) {
    guest::StoreU32(base, Get(base, kPauseData) + 140, 1);
    calls.Call(kPagesGo, frame, 0x8220DB80, Get(base, kPausePages), 5, 5, kNone);
    calls.Call(kMenuSoundPlay, frame, 0x8220DB88, 2);
    return 0;
  }
  if (uint32_t(calls.Call(kListAccepted, frame, 0x8220D9D8, Get(base, kPauseList))) == 0) {
    return 0;
  }
  calls.Call(kMenuSoundPlay, frame, 0x8220D9E8, 0);
  const uint32_t at = ListSelection(base, Get(base, kPauseList));
  const uint32_t kind = guest::LoadU32(base, Get(base, kPauseData) + 132);
  const uint32_t item = guest::LoadU32(base, kPauseItems + ((kind * 6 + at) << 2));
  auto go = [&](uint64_t page, uint64_t transition, uint64_t queued) {
    calls.Call(kPagesGo, frame, 0x8220DA54, Get(base, kPausePages), page, transition, queued);
    return 0;
  };
  // The items that end or restart the race: the answer (+140), then the
  // confirm dialog in `mode` and page 4.
  auto confirm = [&](uint32_t answer, uint32_t mode) {
    guest::StoreU32(base, Get(base, kPauseData) + 140, answer);
    RaceDialogOpen(base, mode, 0, frame, calls);
    return go(4, kNone, kNone);
  };
  switch (item) {
    case 0:  // sub_8220DA38, the resume (merged)
      guest::StoreU32(base, Get(base, kPauseData) + 140, 1);
      return go(5, 5, kNone);
    case 1:
      return go(1, 6, 7);
    case 2:
      return go(2, 11, 12);
    case 3:
      return go(3, 16, 17);
    case 4: {
      // Stages::Copy of the current stage into the frame's +80 (its plain
      // C++, or the original through its adapter where it declines).
      const uint32_t stages = Get(base, kRaceData) + 0x59620;
      if (!vehicle_accessors::StagesCopy(base, frame + 80, stages, 0xFFFFFFFFu)) {
        calls.Call(kStagesCopy, frame, 0x8220DAA4, frame + 80, stages, kNone);
      }
      uint32_t answer = 4;
      if (int32_t(guest::LoadU32(base, Get(base, kRaceData) + 0xA3AF0)) == 6 &&
          int32_t(guest::LoadU32(base, frame + 80)) != 1 &&
          guest::LoadU32(base, kStageWord) == 0) {
        answer = 5;
      }
      return confirm(answer, 4);
    }
    case 5:
      return confirm(2, 1);
    case 6:
      return confirm(3, 3);
    case 7:
      return confirm(6, 2);
    case 8:
      return confirm(7, 2);
    case 9:
      return confirm(9, 2);
    case 10:
      return confirm(4, 5);
    default:
      guest::StoreU32(base, Get(base, kPauseData) + 140, 0);
      return 0;
  }
}

uint32_t PauseMenuToggleOption(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220CDA0 PauseMenu::ToggleOption");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t list = Get(base, kPauseList);
  const int32_t side = int32_t(guest::LoadU32(base, list + kListSide));
  const uint32_t selection = ListSelection(base, list);
  const uint32_t data = Get(base, kPauseData);
  for (const uint32_t field : {36u, 40u, 44u, 48u}) {
    guest::StoreU32(base, frame + 80 + (field - 36), data + field);
  }
  guest::StoreU32(base, frame + 96, data + 144);
  if (side != -1 && side != 1) {
    return 0;
  }
  const uint32_t edit = guest::LoadU32(base, ((selection << 2) + frame + 80));
  const int32_t value = int32_t(guest::LoadU32(base, edit));
  if (side == -1 ? value != 0 : value != 1) {
    return 0;
  }
  guest::StoreU32(base, edit, side == -1 ? 1 : 0);
  PauseMenuShowOptions(base, frame);
  return 1;
}

uint64_t PauseMenuOptionsPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220DB98 PauseMenu::OptionsPage");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kListUpdate, frame, 0x8220DBB8, Get(base, kPauseList));
  if (guest::LoadU32(base, Get(base, kPauseList) + kListMove) != 0) {
    calls.Call(kPagesGo, frame, 0x8220DBE0, Get(base, kPausePages), 1, 8, kNone);
    calls.Call(kMenuSoundPlay, frame, 0x8220DBE8, 4);
    return 0;
  }
  if (PauseMenuToggleOption(base, frame) != 0) {
    calls.Call(kPagesGo, frame, 0x8220DC28, Get(base, kPausePages), 1, 9, kNone);
    PauseMenuSaveHudOptions(base, frame);
    calls.Call(kMenuSoundPlay, frame, 0x8220DC34, 4);
    return 8;
  }
  // Back to the list with the options kept: ShowOptions, Go(-1, 0, then 10),
  // SaveOptions.
  auto back = [&]() -> uint64_t {
    PauseMenuShowOptions(base, frame);
    calls.Call(kPagesGo, frame, 0x8220DCB8, Get(base, kPausePages), kNone, 0, 10);
    PauseMenuSaveOptions(base, frame);
    return 8;
  };
  if (uint32_t(calls.Call(kListAccepted, frame, 0x8220DC44, Get(base, kPauseList))) != 0) {
    calls.Call(kMenuSoundPlay, frame, 0x8220DC54, 0);
    if (int32_t(ListSelection(base, Get(base, kPauseList))) == 4) {
      const uint32_t data = Get(base, kPauseData);
      if (int32_t(guest::LoadU32(base, data + 144)) == 1) {
        CopyFour(base, data, data + 36);
        return back();
      }
    }
    if (int32_t(ListSelection(base, Get(base, kPauseList))) == 4) {
      const uint32_t data = Get(base, kPauseData);
      if (int32_t(guest::LoadU32(base, data + 144)) == 0) {
        CopyFour(base, data + 36, data + 72);
        PauseMenuShowOptions(base, frame);
        PauseMenuSaveHudOptions(base, frame);
        return 8;
      }
    }
    ListSetSelection(base, Get(base, kPauseList), 4);
    guest::StoreU32(base, Get(base, kPauseData) + 144, 1);
    calls.Call(kPagesGo, frame, 0x8220DD4C, Get(base, kPausePages), 1, 8, kNone);
    return 0;
  }
  if (uint32_t(calls.Call(kListCancelled, frame, 0x8220DD58, Get(base, kPauseList))) == 0) {
    return 0;
  }
  calls.Call(kMenuSoundPlay, frame, 0x8220DD68, 2);
  const uint32_t data = Get(base, kPauseData);
  CopyFour(base, data + 36, data);
  return back();
}

uint64_t PauseMenuAdjustPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220EBD8 PauseMenu::AdjustPage");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kListUpdate, frame, 0x8220EBF8, Get(base, kPauseList));
  uint64_t transition = 0;
  if (guest::LoadU32(base, Get(base, kPauseList) + kListMove) != 0) {
    transition = 18;
  } else if (PauseMenuStepAdjust(base, frame) != 0) {
    transition = 19;
  }
  if (transition != 0) {
    calls.Call(kPagesGo, frame, 0x8220EC34, Get(base, kPausePages), 3, transition, kNone);
    calls.Call(kMenuSoundPlay, frame, 0x8220EC3C, 4);
    return 0;
  }
  // Three words copied from `from` to `to`, one at a time.
  auto copy_three = [&](uint32_t to, uint32_t from) {
    for (uint32_t k = 0; k < 12; k += 4) {
      guest::StoreU32(base, to + k, guest::LoadU32(base, from + k));
    }
  };
  // Back to the list: Go(-1, 0, then 20).
  auto back = [&]() {
    calls.Call(kPagesGo, frame, 0x8220ED70, Get(base, kPausePages), kNone, 0, 20);
    return 0;
  };
  if (uint32_t(calls.Call(kListAccepted, frame, 0x8220EC48, Get(base, kPauseList))) != 0) {
    calls.Call(kMenuSoundPlay, frame, 0x8220EC58, 0);
    if (int32_t(ListSelection(base, Get(base, kPauseList))) == 3) {
      const uint32_t data = Get(base, kPauseData);
      if (guest::LoadU32(base, data + 144) == 0) {
        copy_three(data + 24, data + 60);
        PauseMenuShowAdjustB(base, frame, calls);
        return back();
      }
    }
    if (int32_t(ListSelection(base, Get(base, kPauseList))) == 3) {
      const uint32_t data = Get(base, kPauseData);
      if (int32_t(guest::LoadU32(base, data + 144)) == 1) {
        copy_three(data + 60, data + 96);
        PauseMenuShowAdjust(base, frame, calls);
        return 0;
      }
    }
    ListSetSelection(base, Get(base, kPauseList), 3);
    guest::StoreU32(base, Get(base, kPauseData) + 144, 0);
    calls.Call(kPagesGo, frame, 0x8220ED70, Get(base, kPausePages), 3, 18, kNone);
    return 0;
  }
  if (uint32_t(calls.Call(kListCancelled, frame, 0x8220ED1C, Get(base, kPauseList))) == 0) {
    return 0;
  }
  calls.Call(kMenuSoundPlay, frame, 0x8220ED2C, 2);
  const uint32_t data = Get(base, kPauseData);
  copy_three(data + 60, data + 24);
  PauseMenuShowAdjust(base, frame, calls);
  return back();
}

uint64_t PauseMenuSetUpOptions(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220E530 PauseMenu::SetUpOptions");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = PauseMovie(base);
  guest::StoreU32(base, frame + 92, 158);
  guest::StoreU32(base, frame + 84, 159);
  const uint64_t init[8] = {Get(base, kPauseList), 5, 5, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x8220E580, init);
  {
    const uint32_t data = Get(base, kPauseData);
    CopyFour(base, data + 36, data);
  }
  guest::StoreU32(base, Get(base, kPauseData) + 144, 1);
  PauseMenuRestoreNodes(base, 0xFFFFFFFFu, 8, frame, calls);
  const uint32_t labels = ui_movies::FindNode(base, movie, 8, 0);
  for (uint32_t k = 0; k < 4; k++) {
    NodeText(calls, frame, 0x8220E5FC, movie, ui_movies::Child(base, labels, 5 + k), 2000 + k);
  }
  const uint32_t marks = ui_movies::FindNode(base, movie, 8, 1);
  NodeText(calls, frame, 0x8220E634, movie, ui_movies::Child(base, marks, 3), 4);
  NodeText(calls, frame, 0x8220E650, movie, ui_movies::Child(base, marks, 4), 7);
  const uint32_t rows = ui_movies::FindNode(base, movie, 8, 2);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, rows + 104)); i++) {
    const uint32_t row =
        ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, rows + 100) + uint32_t(i));
    const uint32_t on = ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, row + 100));
    NodeText(calls, frame, 0x8220E6B4, movie, ui_movies::Child(base, on, 1), 0);
    const uint32_t off = ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, row + 100) + 1);
    NodeText(calls, frame, 0x8220E6E0, movie, ui_movies::Child(base, off, 1), 1);
  }
  return PauseMenuShowOptions(base, frame);
}

uint64_t PauseMenuSetAdjust(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220F1B8 PauseMenu::SetAdjust");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = PauseMovie(base);
  if (guest::LoadU32(base, step) == 0) {
    if (int32_t(ListSelection(base, Get(base, kPauseList))) == 3) {
      const bool off = int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 0;
      const uint64_t from = off ? 196 : 195, to = off ? 195 : 196;
      calls.Call(kFinishNamed, frame, off ? 0x8220F21C : 0x8220F248, movie, from);
      const uint64_t args[8] = {movie, to, 0, 0, 0, 0, 0, 0};
      calls.CallArgs(kPlayNamed, frame, off ? 0x8220F238 : 0x8220F264, args);
      calls.Call(kLoopNamed, frame, 0x8220F270, movie, to);
    } else {
      PauseMenuShowAdjust(base, frame, calls);
    }
    guest::StoreU32(base, step, 1);
  }
  calls.Call(kRun, frame, 0x8220F28C, movie, 0);
  return 1;
}

uint64_t PauseMenuOpenOptions(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220EEF0 PauseMenu::OpenOptions");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = PauseMovie(base);
  if (guest::LoadU32(base, step) == 0) {
    const uint32_t selection = ListSelection(base, Get(base, kPauseList));
    guest::StoreU32(base, Get(base, kPauseData) + 148, selection);
    calls.Call(kFinish, frame, 0x8220EF4C, movie, 7, uint64_t(selection) + 1);
    const uint64_t play[8] = {movie, kNone, 3, 0, 0, 0, 0, 0};
    calls.CallArgs(kPlay, frame, 0x8220EF6C, play);
    PauseMenuSetUpOptions(base, frame, calls);
    guest::StoreU32(base, step, 1);
  }
  return uint32_t(calls.Call(kRun, frame, 0x8220EF84, movie, 0)) == 0 ? 1 : 0;
}

}  // namespace race_screens
