// The pause menu's transitions and item helpers
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The pause menu's movie: the screen's +0.
uint32_t PauseMovie(uint8_t* base) { return Get(base, Get(base, kPauseScreen)); }

// UiMovie::Play(movie, sub_scene, index, chained, 0, 0, 0) through its
// override.
uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t sub_scene,
              uint64_t index, uint64_t chained = 0) {
  const uint64_t args[8] = {movie, sub_scene, index, chained, 0, 0, 0, 0};
  return calls.CallArgs(kPlay, frame, lr, args);
}

// UiMovie::PlayNamed(movie, named, 0, 0, 0, 0) through its override.
uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  const uint64_t args[8] = {movie, named, 0, 0, 0, 0, 0, 0};
  return calls.CallArgs(kPlayNamed, frame, lr, args);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

uint32_t Selection(uint8_t* base) { return ListSelection(base, Get(base, kPauseList)); }

// A transition's frame: its first call's work when the step word is 0 (then
// the word 1), the movie run once; Run's r3.
template <typename Work>
uint64_t Transition(uint8_t* base, uint32_t step, uint32_t frame, uint32_t lr,
                    const Calls& calls, Work&& work) {
  const uint32_t movie = PauseMovie(base);
  if (guest::LoadU32(base, step) == 0) {
    work(movie);
    guest::StoreU32(base, step, 1);
  }
  return Run(calls, frame, lr, movie);
}

}  // namespace

uint64_t PauseMenuRunTen(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C5E0 PauseMenu::RunTen");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = PauseMovie(base);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, Get(base, kPauseData) + 152, 0);
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8220C62C, movie);
  const uint32_t data = Get(base, kPauseData);
  guest::StoreU32(base, data + 152, guest::LoadU32(base, data + 152) + 1);
  return int32_t(guest::LoadU32(base, Get(base, kPauseData) + 152)) >= 10 ? 1 : 0;
}

uint64_t PauseMenuBackToMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C668 PauseMenu::BackToMain");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220C6F0, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x8220C6C0, movie, 7, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x8220C6DC, movie, 7,
               uint64_t(Selection(base)) + 1);
  });
  return 1;
}

uint64_t PauseMenuCloseMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C700 PauseMenu::CloseMain");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t run = Transition(base, step, frame, 0x8220C794, calls, [&](uint32_t movie) {
    calls.Call(kFinish, frame, 0x8220C750, movie, 7,
               uint64_t(Selection(base)) + 1);
    calls.Call(kFinish, frame, 0x8220C760, movie, kNone, 0);
    Play(calls, frame, 0x8220C780, movie, kNone, 2);
  });
  return uint32_t(run) == 0 ? 1 : 0;
}

uint64_t PauseMenuMoveMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C7B8 PauseMenu::MoveMain");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220C85C, calls, [&](uint32_t movie) {
    // MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
    const uint32_t previous = guest::LoadU32(base, Get(base, kPauseList) + kListSelectionBefore);
    calls.Call(kFinish, frame, 0x8220C800, movie, 7, uint64_t(previous) + 1);
    Play(calls, frame, 0x8220C82C, movie, 7, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x8220C848, movie, 7,
               uint64_t(Selection(base)) + 1);
  });
  return 1;
}

uint64_t PauseMenuShowOptionsRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C868 PauseMenu::ShowOptionsRow");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220C924, calls, [&](uint32_t movie) {
    const int32_t selection = int32_t(Selection(base));
    uint64_t row;
    if (selection < 4) {
      row = uint64_t(uint32_t(selection)) + 4;
      Play(calls, frame, 0x8220C8C8, movie, 8, row);
    } else if (int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 1) {
      row = 2;
      Play(calls, frame, 0x8220C8F0, movie, 8, row);
    } else {
      row = 1;
      Play(calls, frame, 0x8220C900, movie, 8, row);
    }
    calls.Call(kLoop, frame, 0x8220C910, movie, 8, row);
  });
  return 1;
}

uint64_t PauseMenuToggleOptions(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C930 PauseMenu::ToggleOptions");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220CA0C, calls, [&](uint32_t movie) {
    if (int32_t(Selection(base)) != 4) {
      return;
    }
    const bool on = int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 1;
    const uint64_t from = on ? 2 : 1, to = on ? 1 : 2;
    calls.Call(kFinish, frame, on ? 0x8220C998 : 0x8220C9C8, movie, 8, from);
    Play(calls, frame, on ? 0x8220C9B8 : 0x8220C9E8, movie, 8, to);
    calls.Call(kLoop, frame, 0x8220C9F8, movie, 8, to);
  });
  return 1;
}

uint64_t PauseMenuShowVolumeRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220CA28 PauseMenu::ShowVolumeRow");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220CA94, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x8220CA78, movie, 9, 1);
    calls.Call(kLoopLast, frame, 0x8220CA80, movie);
  });
  return 1;
}

uint64_t PauseMenuShowAdjustRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220CAB0 PauseMenu::ShowAdjustRow");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220CB1C, calls, [&](uint32_t movie) {
    PlayNamed(calls, frame, 0x8220CAFC, movie, 192);
    calls.Call(kLoopNamed, frame, 0x8220CB08, movie, 192);
  });
  return 1;
}

uint64_t PauseMenuConfirmPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C578 PauseMenu::ConfirmPage");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t answer = int32_t(uint32_t(RaceDialogUpdate(base, r4, frame, calls)));
  if (answer == 1) {
    calls.Call(kPagesGo, frame, 0x8220C5BC, Get(base, kPausePages), 5, 5, kNone);
  } else if (answer == 2) {
    calls.Call(kPagesGo, frame, 0x8220C5BC, Get(base, kPausePages), 0, kNone, kNone);
  }
  return 0;
}

uint64_t PauseMenuShowItems(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220CB38 PauseMenu::ShowItems");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = PauseMovie(base);
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 0);
  uint64_t r3 = node;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, node + 104)); i++) {
    const uint32_t kind = guest::LoadU32(base, Get(base, kPauseData) + 132);
    const uint32_t item = guest::LoadU32(base, kPauseItems + (kind * 6 + uint32_t(i)) * 4);
    const uint64_t child = uint64_t(guest::LoadU32(base, node + 100)) + uint64_t(i);
    if (int32_t(item) != -1 && i < 6) {
      r3 = calls.Call(kSetText, frame, 0x8220CBC0, movie, 7, child, uint64_t(item) + 1989);
    } else {
      const uint32_t hidden = ui_movies::FindNode(base, movie, 7, uint32_t(child));
      guest::StoreU32(base, hidden + 80, 0);
      r3 = hidden;
    }
  }
  return r3;
}

uint32_t PauseMenuSetArrowA(uint8_t* base, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220CBF8 PauseMenu::SetArrowA");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t node = ui_movies::FindNode(base, PauseMovie(base), 8, 0);
  const int64_t word = int64_t(int32_t(uint32_t(value)));
  guest::StoreU32(base, frame + 80, uint32_t(uint64_t(word) >> 32));
  guest::StoreU32(base, frame + 84, uint32_t(word));
  const uint32_t child = ui_movies::Child(base, node, 0);
  guest::StoreU32(base, child + 16, std::bit_cast<uint32_t>(float(double(word))));
  return child;
}

uint32_t PauseMenuShowOptions(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220CC60 PauseMenu::ShowOptions");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = PauseMovie(base);
  const uint32_t data = Get(base, kPauseData);
  for (const uint32_t field : {36u, 40u, 44u, 48u}) {
    guest::StoreU32(base, frame + 80 + (field - 36), guest::LoadU32(base, data + field));
  }
  guest::StoreU32(base, frame + 96, guest::LoadU32(base, data + 144));
  const uint32_t group = ui_movies::FindNode(base, movie, 8, 2);
  uint32_t r3 = group;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, group + 104)); i++) {
    const uint32_t part = ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, group + 100) + i);
    const uint32_t first = ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, part + 100));
    const uint32_t first0 = ui_movies::Child(base, first, 0);
    const uint32_t first2 = ui_movies::Child(base, first, 2);
    const uint32_t second =
        ui_movies::FindNode(base, movie, 8, guest::LoadU32(base, part + 100) + 1);
    const uint32_t second0 = ui_movies::Child(base, second, 0);
    const uint32_t second2 = ui_movies::Child(base, second, 2);
    const bool on = int32_t(guest::LoadU32(base, frame + 80 + 4 * uint32_t(i))) == 1;
    guest::StoreU32(base, first0 + 108, on ? 0 : 1);
    guest::StoreU32(base, first2 + 80, on ? 1 : 0);
    guest::StoreU32(base, second0 + 108, on ? 1 : 0);
    guest::StoreU32(base, second2 + 80, on ? 0 : 1);
    r3 = second2;
  }
  return r3;
}

uint64_t PauseMenuShowVolumes(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220D0B8 PauseMenu::ShowVolumes");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = PauseMovie(base);
  uint32_t node = ui_movies::FindNode(base, movie, 9, 5);
  calls.Call(kShowVolume, frame, 0x8220D0F8, node,
             guest::LoadU32(base, Get(base, kPauseData) + 52));
  node = ui_movies::FindNode(base, movie, 9, 6);
  return calls.Call(kShowVolume, frame, 0x8220D114, node,
                    guest::LoadU32(base, Get(base, kPauseData) + 56));
}

uint32_t PauseMenuShowSlider(uint8_t* base, uint32_t slider, uint64_t value, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220D130 PauseMenu::ShowSlider");
  using namespace camera_modes::steps;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = PauseMovie(base);
  const uint32_t text = ui_movies::Child(base, slider, 0);
  const uint64_t args[8] = {movie, text, 8, value, 0, 0, 0, 0};
  calls.CallArgs(kSetNodeText, frame, 0x8220D170, args);
  // (value + 10) as a float, times the step, truncated (stfiwx over the
  // double word's high half), then 16 less it as a float.
  const int64_t shifted = int64_t(int32_t(uint32_t(value + 10)));
  guest::StoreU32(base, frame + 84, uint32_t(shifted));
  const int32_t truncated = Fctiwz(Fmuls(Single(double(shifted)), Lfs(base, 0x8200A6AC)));
  const int64_t rest = int64_t(int32_t(16u - uint32_t(truncated)));
  guest::StoreU32(base, frame + 80, uint32_t(uint64_t(rest) >> 32));
  guest::StoreU32(base, frame + 84, uint32_t(rest));
  const uint32_t bar = ui_movies::Child(base, slider, 2);
  Stfs(base, bar + 12, Single(double(rest)));
  return bar;
}

uint64_t PauseMenuRestoreNodes(uint8_t* base, uint32_t group, uint32_t child, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220D5D0 PauseMenu::RestoreNodes");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = PauseMovie(base);
  auto node = [&](uint32_t scene) {
    const uint32_t root = guest::LoadU32(base, movie + scene);
    return ui_movies::Child(
        base, int32_t(group) == -1 ? root : ui_movies::Child(base, root, group), child);
  };
  const uint32_t file = node(ui_movies::kMovieFile);
  const uint32_t live = node(ui_movies::kMovieLive);
  const uint32_t saved = node(ui_movies::kMovieSaved);
  const uint64_t a[8] = {movie, live, file, 0, 0, 0, 0, 0};
  calls.CallArgs(kRestorePose, frame, 0x8220D67C, a);
  const uint64_t b[8] = {movie, saved, file, 0, 0, 0, 0, 0};
  return calls.CallArgs(kRestorePose, frame, 0x8220D694, b);
}

uint64_t PauseMenuMoveOptions(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220DD98 PauseMenu::MoveOptions");
  const uint32_t frame = Frame(base, sp, 144);
  Transition(base, step, frame, 0x8220DF24, calls, [&](uint32_t movie) {
    const uint32_t list = Get(base, kPauseList);
    const uint32_t selection = ListSelection(base, list);
    const uint32_t previous = guest::LoadU32(base, Get(base, kPauseList) + kListSelectionBefore);
    if (int32_t(previous) < 4) {
      calls.Call(kFinish, frame, 0x8220DDFC, movie, 8, uint64_t(previous) + 4);
    } else {
      const bool on = int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 1;
      calls.Call(kFinish, frame, 0x8220DE1C, movie, 8, on ? 1 : 2);
      if (int32_t(selection) == 0) {
        PauseMenuSetArrowA(base, 0, frame);
      } else if (int32_t(selection) == 3) {
        PauseMenuSetArrowA(base, 120, frame);
      }
      guest::StoreU32(base, Get(base, kPauseData) + 144, 1);
    }
    uint64_t row;
    if (int32_t(selection) < 4) {
      row = uint64_t(selection) + 4;
      Play(calls, frame, 0x8220DE70, movie, 8, row);
    } else if (int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 1) {
      row = 1;
      Play(calls, frame, 0x8220DE90, movie, 8, row);
    } else {
      row = 2;
      Play(calls, frame, 0x8220DEA0, movie, 8, row);
    }
    calls.Call(kLoop, frame, 0x8220DEB0, movie, 8, row);
    calls.Call(kFinishNamed, frame, 0x8220DEBC, movie, 20);
    calls.Call(kFinishNamed, frame, 0x8220DEC8, movie, 19);
    if (int32_t(previous) != 4 && int32_t(selection) != 4) {
      const int32_t move = int32_t(guest::LoadU32(base, Get(base, kPauseList) + kListMove));
      if (move == 1) {
        PlayNamed(calls, frame, 0x8220DF14, movie, 20);
      } else if (move == -1) {
        PlayNamed(calls, frame, 0x8220DF14, movie, 19);
      }
    }
  });
  return 1;
}

uint64_t PauseMenuMoveAdjust(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220E1F8 PauseMenu::MoveAdjust");
  const uint32_t frame = Frame(base, sp, 144);
  Transition(base, step, frame, 0x8220E408, calls, [&](uint32_t movie) {
    const uint32_t previous = guest::LoadU32(base, Get(base, kPauseList) + kListSelectionBefore);
    const uint32_t selection = ListSelection(base, Get(base, kPauseList));
    if (previous < 3) {
      calls.Call(kFinishNamed, frame, 0x8220E2C4, movie, 192 + previous);
    } else if (previous == 3) {
      const bool off = int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 0;
      calls.Call(kFinishNamed, frame, 0x8220E278, movie, off ? 195 : 196);
      guest::StoreU32(base, Get(base, kPauseData) + 144, 0);
      if (int32_t(selection) == 0) {
        calls.Call(kArrowB, frame, 0x8220E2A4, 0);
      } else if (int32_t(selection) == 2) {
        calls.Call(kArrowB, frame, 0x8220E2A4, 80);
      }
    }
    if (selection <= 3) {
      uint64_t named;
      uint32_t lr;
      if (selection < 3) {
        named = 192 + selection;
        constexpr uint32_t kLr[3] = {0x8220E384, 0x8220E360, 0x8220E33C};
        lr = kLr[selection];
      } else if (int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 0) {
        named = 195;
        lr = 0x8220E308;
      } else {
        named = 196;
        lr = 0x8220E318;
      }
      PlayNamed(calls, frame, lr, movie, named);
      calls.Call(kLoopNamed, frame, 0x8220E390, movie, named);
    }
    calls.Call(kFinishNamed, frame, 0x8220E39C, movie, 148);
    calls.Call(kFinishNamed, frame, 0x8220E3A8, movie, 149);
    if (int32_t(selection) != 3 && int32_t(previous) != 3) {
      const int32_t move = int32_t(guest::LoadU32(base, Get(base, kPauseList) + kListMove));
      if (move == 1) {
        PlayNamed(calls, frame, 0x8220E3F4, movie, 149);
      } else if (move == -1) {
        PlayNamed(calls, frame, 0x8220E3F4, movie, 148);
      }
    }
  });
  return 1;
}

uint32_t PauseMenuColorItems(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220E700 PauseMenu::ColorItems");
  Frame(base, sp, 128);
  const uint32_t movie = PauseMovie(base);
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 0);
  uint32_t r3 = node;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, node + 104)); i++) {
    const bool enabled = ListIsEnabled(base, Get(base, kPauseList), uint32_t(i)) != 0;
    const uint32_t color[3] = {enabled ? 205u : 255u, enabled ? 235u : 255u,
                               enabled ? 67u : 255u};
    for (uint32_t k = 0; k < 3; k++) {
      r3 = ui_movies::FindNode(base, movie, 7, guest::LoadU32(base, node + 100) + uint32_t(i));
      guest::StoreU32(base, r3 + 56 + 4 * k, color[k]);
    }
  }
  return r3;
}

uint32_t PauseMenuShowAdjust(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220E888 PauseMenu::ShowAdjust");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t data = Get(base, kPauseData);
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, frame + 80 + 4 * k, guest::LoadU32(base, data + 60 + 4 * k));
  }
  const uint32_t group = ui_movies::FindNode(base, PauseMovie(base), 10, 0);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slider = ui_movies::Child(base, group, i + 4);
    r3 = PauseMenuShowSlider(base, slider, guest::LoadU32(base, frame + 80 + 4 * i), frame, calls);
  }
  return r3;
}

uint64_t PauseMenuShowMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220EE28 PauseMenu::ShowMain");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x8220EEE4, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x8220EE70, movie, kNone, 0);
    calls.Call(kLoop, frame, 0x8220EE80, movie, kNone, 0);
    Play(calls, frame, 0x8220EEB0, movie, 7, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x8220EECC, movie, 7, uint64_t(Selection(base)) + 1);
    PauseMenuColorItems(base, frame);
  });
  return 1;
}

uint64_t PauseMenuOpen(uint8_t* base, uint32_t screen, uint64_t kind, uint64_t flag, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220FA88 PauseMenu::Open");
  const uint32_t frame = Frame(base, sp, 112);
  guest::StoreU32(base, Get(base, kPauseData) + 132, uint32_t(kind));
  guest::StoreU32(base, frame + 80, 4);
  guest::StoreU32(base, Get(base, kPauseData) + 156, uint32_t(flag));
  const uint32_t movie =
      guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  guest::StoreU32(base, screen, movie);
  calls.Call(kResetPose, frame, 0x8220FAD8, movie);
  PauseMenuLoadData(base, frame, calls);
  calls.Call(kPagesGo, frame, 0x8220FAF4, Get(base, kPausePages), 0, 1, 2);
  return calls.Call(kMenuSoundPlay, frame, 0x8220FAFC, 6);
}

uint64_t PauseMenuOpenMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220ED90 PauseMenu::OpenMain");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = PauseMovie(base);
  if (guest::LoadU32(base, step) == 0) {
    Play(calls, frame, 0x8220EDE0, movie, kNone, 1);
    guest::StoreU32(base, Get(base, kPauseData) + 148, 0);
    PauseMenuSetUpMain(base, frame, calls);
    guest::StoreU32(base, step, 1);
  }
  return uint32_t(Run(calls, frame, 0x8220EE08, movie)) == 0 ? 1 : 0;
}

namespace {

// The three Closes (transitions 10, 15 and 20): on their first call
// `finish` (the row's animation finished), animation `animation` played,
// SetUpMain, the step word 1; when the run answers 0, ScreenPages::Go(0,
// transition 3) and the step word 0. Return 0.
template <typename Finish>
uint64_t CloseToMain(uint8_t* base, uint32_t step, uint32_t frame, uint64_t animation,
                     uint32_t play_lr, uint32_t run_lr, uint32_t go_lr, const Calls& calls,
                     Finish&& finish) {
  const uint32_t movie = PauseMovie(base);
  if (guest::LoadU32(base, step) == 0) {
    finish(movie);
    Play(calls, frame, play_lr, movie, kNone, animation);
    PauseMenuSetUpMain(base, frame, calls);
    guest::StoreU32(base, step, 1);
  }
  if (uint32_t(Run(calls, frame, run_lr, movie)) == 0) {
    calls.Call(kPagesGo, frame, go_lr, Get(base, kPausePages), 0, 3, kNone);
    guest::StoreU32(base, Get(base, kPausePages) + 24, 0);
  }
  return 0;
}

}  // namespace

uint64_t PauseMenuCloseOptions(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220EFA8 PauseMenu::CloseOptions");
  const uint32_t frame = Frame(base, sp, 112);
  return CloseToMain(base, step, frame, 4, 0x8220F03C, 0x8220F054, 0x8220F074, calls,
                     [&](uint32_t movie) {
                       const int32_t selection = int32_t(Selection(base));
                       uint64_t row;
                       if (selection < 4) {
                         row = uint64_t(uint32_t(selection)) + 4;
                       } else {
                         row = guest::LoadU32(base, Get(base, kPauseData) + 144) == 0 ? 2 : 1;
                       }
                       calls.Call(kFinish, frame, 0x8220F01C, movie, 8, row);
                     });
}

uint64_t PauseMenuCloseVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220F0A0 PauseMenu::CloseVolume");
  const uint32_t frame = Frame(base, sp, 112);
  return CloseToMain(base, step, frame, 6, 0x8220F154, 0x8220F16C, 0x8220F18C, calls,
                     [&](uint32_t movie) {
                       const uint32_t selection = Selection(base);
                       if (selection >= 3) {
                         return;
                       }
                       uint64_t row = selection + 1;
                       if (selection == 2) {
                         row = int32_t(guest::LoadU32(base, Get(base, kPauseData) + 144)) == 1 ? 4
                                                                                                : 3;
                       }
                       calls.Call(kFinish, frame, 0x8220F134, movie, 9, row);
                     });
}

uint64_t PauseMenuCloseAdjust(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220F2A8 PauseMenu::CloseAdjust");
  const uint32_t frame = Frame(base, sp, 112);
  return CloseToMain(base, step, frame, 8, 0x8220F360, 0x8220F378, 0x8220F398, calls,
                     [&](uint32_t movie) {
                       const uint32_t selection = Selection(base);
                       if (selection > 3) {
                         return;
                       }
                       uint64_t named = 192 + selection;
                       if (selection == 3) {
                         named =
                             guest::LoadU32(base, Get(base, kPauseData) + 144) == 0 ? 195 : 196;
                       }
                       calls.Call(kFinishNamed, frame, 0x8220F340, movie, named);
                     });
}

uint64_t PauseMenuShowCareer(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220D418 PauseMenu::ShowCareer");
  using namespace camera_modes::steps;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = PauseMovie(base);
  const uint32_t panel = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 0);
  const uint32_t kind = guest::LoadU32(base, Get(base, kPauseData) + 132);
  guest::StoreU32(base, panel + 80, kind <= 2 ? 1 : 0);
  // UiMovie::SetText(movie, -1, child, id, value) through its override, its
  // unset varargs 0 (the user's call, 2026-10-30).
  auto text = [&](uint32_t lr, uint64_t child, uint64_t id, uint64_t value) {
    const uint64_t args[8] = {movie, kNone, child, id, value, 0, 0, 0};
    calls.CallArgs(kSetText, frame, lr, args);
  };
  text(0x8220D498, 28, 8, guest::LoadU32(base, Get(base, kPauseData) + 120));
  // The car's two values (+124, +128) as floats times the double at
  // 0x8200A510, truncated (stfiwx over the double word's high half).
  const double scale = std::bit_cast<double>(
      (uint64_t(guest::LoadU32(base, 0x8200A510)) << 32) | guest::LoadU32(base, 0x8200A514));
  auto scaled = [&](uint32_t field) {
    const int64_t word = int64_t(int32_t(guest::LoadU32(base, Get(base, kPauseData) + field)));
    guest::StoreU32(base, frame + 84, uint32_t(word));
    const int32_t truncated = Fctiwz(Fmuls(Single(double(word)), scale));
    guest::StoreU32(base, frame + 80, uint32_t(truncated));
    return uint64_t(uint32_t(truncated));
  };
  text(0x8220D4E4, 29, 8, scaled(124));
  text(0x8220D528, 30, 8, scaled(128));
  calls.Call(kSetNumberText, frame, 0x8220D540, movie, kNone, 21,
             guest::LoadU32(base, Get(base, kPauseData) + 108));
  text(0x8220D55C, 26, 8, guest::LoadU32(base, Get(base, kPauseData) + 116));
  text(0x8220D570, 31, 2009, 0);
  text(0x8220D584, 32, 2010, 0);
  text(0x8220D598, 33, 2011, 0);
  const uint32_t weekday = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 24);
  guest::StoreU32(base, weekday + 108, guest::LoadU32(base, Get(base, kPauseData) + 112));
  return weekday;
}

uint64_t PauseMenuSetUpMain(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220E418 PauseMenu::SetUpMain");
  const uint32_t frame = Frame(base, sp, 144);
  {
    const uint32_t data = Get(base, kPauseData);
    guest::StoreU32(base, data + 136, 6);
    const uint32_t row = kPauseItems + guest::LoadU32(base, data + 132) * 24;
    for (uint32_t i = 0; i < 6; i++) {
      if (int32_t(guest::LoadU32(base, row + 4 * i)) == -1) {
        guest::StoreU32(base, data + 136, i);
        break;
      }
    }
  }
  const uint64_t count = guest::LoadU32(base, Get(base, kPauseData) + 136);
  guest::StoreU32(base, frame + 84, 159);
  guest::StoreU32(base, frame + 92, 158);
  const uint64_t init[8] = {Get(base, kPauseList), count, count, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x8220E4B0, init);
  ListSetSelection(base, Get(base, kPauseList),
                   guest::LoadU32(base, Get(base, kPauseData) + 148));
  if (guest::LoadU32(base, Get(base, kPauseData) + 156) != 0) {
    for (int32_t i = 0; i < int32_t(guest::LoadU32(base, Get(base, kPauseData) + 136)); i++) {
      const uint32_t kind = guest::LoadU32(base, Get(base, kPauseData) + 132);
      if (int32_t(guest::LoadU32(base, kPauseItems + ((kind * 6 + uint32_t(i)) << 2))) == 4) {
        ListSetEnabled(base, Get(base, kPauseList), uint32_t(i), 0);
      }
    }
  }
  PauseMenuShowItems(base, frame, calls);
  return PauseMenuShowCareer(base, frame, calls);
}

uint64_t PauseMenuShowVolumesB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220E810 PauseMenu::ShowVolumesB");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = PauseMovie(base);
  const uint32_t effects = ui_movies::FindNode(base, movie, 9, 5);
  calls.Call(kShowVolumeBar, frame, 0x8220E850, effects,
             guest::LoadU32(base, Get(base, kPauseData) + 52));
  const uint32_t music = ui_movies::FindNode(base, movie, 9, 6);
  return calls.Call(kShowVolumeBar, frame, 0x8220E86C, music,
                    guest::LoadU32(base, Get(base, kPauseData) + 56));
}

uint64_t PauseMenuShowAdjustB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220E908 PauseMenu::ShowAdjustB");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = PauseMovie(base);
  const uint32_t data = Get(base, kPauseData);
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, frame + 80 + 4 * k, guest::LoadU32(base, data + 60 + 4 * k));
  }
  const uint32_t node = ui_movies::FindNode(base, movie, 10, 0);
  uint64_t r3 = 0;
  for (uint32_t k = 0; k < 3; k++) {
    r3 = calls.Call(kShowSliderMark, frame, 0x8220E96C, ui_movies::Child(base, node, k + 4),
                    guest::LoadU32(base, frame + 80 + 4 * k));
  }
  return r3;
}

}  // namespace race_screens
