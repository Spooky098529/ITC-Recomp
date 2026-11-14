// The confirm dialog's pages, transitions and draw
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <optional>

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// UiMovies::Find(14) as the originals make it: the id stored at `key` (a
// frame word), the movie table's pair for it, its movie. `pair` gets the
// pair's address (the originals keep it in a register).
uint32_t DialogMovie(uint8_t* base, uint32_t key, uint32_t* pair = nullptr) {
  guest::StoreU32(base, key, 14);
  const uint32_t found = ui_movies::PairsFind(base, ui_movies::kMovieTable, key);
  if (pair != nullptr) {
    *pair = found;
  }
  return guest::LoadU32(base, found);
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

uint64_t Go(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t pages, uint64_t page,
            uint64_t transition, uint64_t queued) {
  return calls.Call(kPagesGo, frame, lr, pages, page, transition, queued);
}

}  // namespace

uint64_t RaceDialogQuestion(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82217F40 RaceDialog::Question");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kListUpdate, frame, 0x82217F58, Get(base, kDialogList));
  const uint32_t list = Get(base, kDialogList);
  if (guest::LoadU32(base, list + kListMove) != 0) {
    Go(calls, frame, 0x82217F80, Get(base, kDialogPages), 1, 2, kNone);
    calls.Call(kMenuSoundPlay, frame, 0x82217F88, 4);
  } else if (uint32_t(calls.Call(kListAccepted, frame, 0x82217F90, list)) != 0) {
    const uint32_t selection = ListSelection(base, Get(base, kDialogList));
    calls.Call(kMenuSoundPlay, frame, 0x82217FAC, 0);
    const uint32_t data = Get(base, kDialogData);
    const int32_t mode = int32_t(guest::LoadU32(base, data + 12));
    // Back to the question (page 1, transition 1, then 0): the mode 6, +20 1.
    auto ask_again = [&](uint32_t lr) {
      Go(calls, frame, lr, Get(base, kDialogPages), 1, 1, 0);
      guest::StoreU32(base, Get(base, kDialogData) + 12, 6);
      guest::StoreU32(base, Get(base, kDialogData) + 20, 1);
    };
    // The device asked for again: page 3, SaveSlots::RequestDevice(0)
    // through its override.
    auto request_device = [&]() {
      Go(calls, frame, 0x822180E4, Get(base, kDialogPages), 3, kNone, kNone);
      calls.Call(kRequestDevice, frame, 0x822180EC, 0);
    };
    switch (mode) {
      case 1:
        if (selection == 0) {
          Go(calls, frame, 0x8221811C, Get(base, kDialogPages), 4, kNone, kNone);
        } else {
          ask_again(0x82218138);
        }
        break;
      case 4:
        if (selection == 0) {
          request_device();
        } else {
          ask_again(0x82218138);
        }
        break;
      case 5:
        if (selection != 0) {
          ask_again(0x82218138);
        } else {
          guest::StoreU32(base, data + 24, 0);
          Go(calls, frame, 0x82218094, Get(base, kDialogPages), 2, 1, 0);
          guest::StoreU32(base, Get(base, kDialogData) + 12, 9);
          guest::StoreU32(base, Get(base, kDialogData) + 20, 2);
          guest::StoreU32(base, Get(base, kDialogData) + 8, 0);
          // Replays::Delete (never seen in play): the original.
          calls.Call(kReplaysDelete, frame, 0x822180C0,
                     guest::LoadU32(base, Get(base, kDialogData) + 4));
        }
        break;
      case 6:
        if (selection == 0) {
          Go(calls, frame, 0x8221811C, Get(base, kDialogPages), 5, 1, kNone);
        } else {
          guest::StoreU32(base, data + 24, 0);
          Go(calls, frame, 0x82218054, Get(base, kDialogPages), 2, 1, 0);
          guest::StoreU32(base, Get(base, kDialogData) + 12, 0);
          guest::StoreU32(base, Get(base, kDialogData) + 20, 2);
        }
        break;
      case 8:
        if (selection == 0) {
          Go(calls, frame, 0x8221811C, Get(base, kDialogPages), 5, 1, kNone);
        } else {
          request_device();
        }
        break;
      case 13:
        guest::StoreU32(base, data + 0, selection == 0 ? 1 : 0);
        Go(calls, frame, 0x8221811C, Get(base, kDialogPages), 5, 1, kNone);
        break;
      default:
        break;
    }
  }
  calls.Call(kListCancelled, frame, 0x82218158, Get(base, kDialogList));
  return 0;
}

uint64_t RaceDialogOpenAnim(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822186A8 RaceDialog::OpenAnim");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = DialogMovie(base, frame + 80);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint32_t mode = guest::LoadU32(base, Get(base, kDialogData) + 16);
    if (mode == 0) {
      PlayNamed(calls, frame, 0x82218730, movie, 15);
    } else if (mode == 1) {
      const uint32_t selection = ListSelection(base, Get(base, kDialogList));
      calls.Call(kFinishNamed, frame, 0x82218780, movie, uint64_t(selection) + 2);
      PlayNamed(calls, frame, 0x82218730, movie, 14);
    } else if (mode == 2) {
      calls.Call(kFinishNamed, frame, 0x82218714, movie, 0);
      PlayNamed(calls, frame, 0x82218730, movie, 15);
    }
    calls.Call(kMenuSoundPlay, frame, 0x82218738, 5);
  }
  if (uint32_t(Run(calls, frame, 0x82218744, movie)) != 0) {
    return 0;
  }
  const uint32_t data = Get(base, kDialogData);
  guest::StoreU32(base, data + 16, guest::LoadU32(base, data + 20));
  return 1;
}

uint64_t RaceDialogMove(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218788 RaceDialog::Move");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = DialogMovie(base, frame + 80);
  if (guest::LoadU32(base, step) == 0) {
    guest::StoreU32(base, step, 1);
    const uint32_t selection = ListSelection(base, Get(base, kDialogList));
    // MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
    const uint32_t previous = guest::LoadU32(base, Get(base, kDialogList) + kListSelectionBefore);
    calls.Call(kFinishNamed, frame, 0x822187F0, movie, uint64_t(previous) + 2);
    PlayNamed(calls, frame, 0x82218810, movie, uint64_t(selection) + 2);
    calls.Call(kLoopNamed, frame, 0x8221881C, movie, uint64_t(selection) + 2);
  }
  Run(calls, frame, 0x82218828, movie);
  return 1;
}

uint64_t RaceDialogDraw(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218F30 RaceDialog::Draw");
  const uint32_t frame = Frame(base, sp, 128);
  uint32_t pair = 0;
  const uint32_t movie = DialogMovie(base, frame + 80, &pair);
  const uint32_t pages = Get(base, kDialogPages);
  // The transitions' table: ShowMessage, OpenAnim, Move.
  const uint32_t table[3] = {0x82218C10, 0x822186A8, 0x82218788};
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, frame + 88 + 4 * i, table[i]);
  }
  const int32_t transition = int32_t(guest::LoadU32(base, pages + 12));
  if (transition == -1) {
    return Run(calls, frame, 0x82218FE0, movie);
  }
  if (transition < 0) {
    PagesFinish(base, pages, 1);
    return pages;
  }
  // The handler (any index read from the frame as the original reads it),
  // with the registers the original has at its bctrl.
  const uint32_t handler = guest::LoadU32(base, (uint32_t(transition) << 2) + frame + 88);
  const uint64_t regs[8] = {uint64_t(pages) + 24, uint64_t(frame) + 80, uint64_t(pages) + 24,
                            uint64_t(int64_t(int32_t(table[0]))),
                            uint64_t(int64_t(int32_t(table[1]))),
                            uint64_t(int64_t(int32_t(table[2]))), pair, uint64_t(frame) + 88};
  const uint64_t result = calls.CallArgs(handler, frame, 0x82218FBC, regs);
  const uint32_t now = Get(base, kDialogPages);
  PagesFinish(base, now, uint32_t(result));
  return now;
}

uint64_t RaceDialogSetText(uint8_t* base, uint64_t id, const uint64_t varargs[7], uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218838 RaceDialog::SetText");
  constexpr uint32_t kSetNodeTextV = 0x8211CBD8;
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, sp + 24 + 8 * i, uint32_t(varargs[i] >> 32));
    guest::StoreU32(base, sp + 28 + 8 * i, uint32_t(varargs[i]));
  }
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = DialogMovie(base, frame + 80);
  const uint32_t node = ui_movies::FindNode(base, movie, 0, 1);
  const uint32_t first = ui_movies::Child(base, node, 0);
  const uint32_t second = ui_movies::Child(base, node, 1);
  const uint32_t third = ui_movies::Child(base, second, 3);
  const uint32_t fourth = ui_movies::Child(base, second, 4);
  guest::StoreU32(base, frame + 80, sp + 24);
  calls.Call(kSetNodeTextV, frame, 0x822188EC, movie, first, id, guest::LoadU32(base, frame + 80));
  calls.Call(kSetNodeText, frame, 0x822188FC, movie, third, 2);
  return calls.Call(kSetNodeText, frame, 0x8221890C, movie, fourth, 3);
}

uint64_t RaceDialogOpen(uint8_t* base, uint32_t mode, uint64_t player, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218B30 RaceDialog::Open");
  const uint32_t frame = Frame(base, sp, 128);
  if (uint32_t(player) < 1) {
    guest::StoreU32(base, Get(base, kDialogList) + kListPlayer, 0);
  } else if (uint32_t(player) == 1) {
    guest::StoreU32(base, Get(base, kDialogList) + kListPlayer, 1);
  }
  if (mode <= 5) {
    const uint64_t varargs[7] = {player, 0, 0, 0, 0, 0, 0};
    RaceDialogSetText(base, 1954 + mode, varargs, frame, calls);
  }
  guest::StoreU32(base, Get(base, kDialogData) + 12, 13);
  guest::StoreU32(base, Get(base, kDialogData) + 16, 1);
  guest::StoreU32(base, Get(base, kDialogData) + 28, 1);
  const uint64_t r3 = Go(calls, frame, 0x82218BF8, Get(base, kDialogPages), 1, 0, kNone);
  guest::StoreU32(base, Get(base, kDialogData) + 32, 1);
  guest::StoreU32(base, Get(base, kDialogData) + 36, 0);
  return r3;
}

uint64_t RaceDialogSetNote(uint8_t* base, uint64_t id, const uint64_t varargs[7], uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218918 RaceDialog::SetNote");
  constexpr uint32_t kSetNodeTextV = 0x8211CBD8;
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, sp + 24 + 8 * i, uint32_t(varargs[i] >> 32));
    guest::StoreU32(base, sp + 28 + 8 * i, uint32_t(varargs[i]));
  }
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = DialogMovie(base, frame + 80);
  const uint32_t node = ui_movies::Child(base, ui_movies::FindNode(base, movie, 0, 2), 1);
  guest::StoreU32(base, frame + 80, sp + 24);
  return calls.Call(kSetNodeTextV, frame, 0x822189A0, movie, node, id,
                    guest::LoadU32(base, frame + 80));
}

uint64_t RaceDialogStartSave(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822189B8 RaceDialog::StartSave");
  constexpr uint32_t kSlotState = 0x8228F9E8;
  const uint32_t frame = Frame(base, sp, 128);
  // SetNote's varargs (r4 to r10, unset by the original) passed as 0, as
  // the user's call for the parking area (2026-10-30): only dead frame words.
  const uint64_t varargs[7] = {0, 0, 0, 0, 0, 0, 0};
  if (int32_t(uint32_t(calls.Call(kSlotState, frame, 0x822189D4, 0))) != 0) {
    RaceDialogSetNote(base, 70, varargs, frame, calls);
    guest::StoreU32(base, Get(base, kDialogData) + 12, 3);
    guest::StoreU32(base, Get(base, kDialogData) + 24, 0);
  } else {
    RaceDialogSetNote(base, 69, varargs, frame, calls);
    guest::StoreU32(base, Get(base, kDialogData) + 12, 2);
  }
  guest::StoreU32(base, Get(base, kDialogData) + 16, 0);
  Go(calls, frame, 0x82218A44, Get(base, kDialogPages), 0, 0, kNone);
  guest::StoreU32(base, frame + 92, 158);
  guest::StoreU32(base, frame + 84, 159);
  const uint64_t init[8] = {Get(base, kDialogList), 1, 1, 1, 154, 153, 155, 156};
  const uint64_t r3 = calls.CallArgs(kListInit, frame, 0x82218A7C, init);
  guest::StoreU32(base, Get(base, kDialogData) + 0, 0);
  guest::StoreU32(base, Get(base, kDialogData) + 32, 1);
  guest::StoreU32(base, Get(base, kDialogData) + 36, 0);
  return r3;
}

uint64_t RaceDialogShowSaving(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218AB0 RaceDialog::ShowSaving");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t varargs[7] = {0, 0, 0, 0, 0, 0, 0};
  RaceDialogSetNote(base, 98, varargs, frame, calls);
  guest::StoreU32(base, Get(base, kDialogData) + 12, 12);
  guest::StoreU32(base, Get(base, kDialogData) + 16, 0);
  const uint64_t r3 = Go(calls, frame, 0x82218B00, Get(base, kDialogPages), 0, 0, kNone);
  guest::StoreU32(base, Get(base, kDialogData) + 32, 1);
  guest::StoreU32(base, Get(base, kDialogData) + 36, 0);
  return r3;
}

std::optional<uint64_t> RaceDialogShowMessage(uint8_t* base, uint32_t step, uint32_t sp,
                                              const Calls& calls) {
  const bool first = guest::LoadU32(base, step) == 0;
  if (first && guest::LoadU32(base, Get(base, kDialogData) + 12) <= 11) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82218C10 RaceDialog::ShowMessage");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t movie = DialogMovie(base, frame + 96);
  if (first) {
    guest::StoreU32(base, step, 1);
    const uint32_t mode = guest::LoadU32(base, Get(base, kDialogData) + 16);
    // Modes 0 and 2: the type-0 node 0's +80 and node 2's child 0's +80.
    auto marks = [&](uint32_t lr, uint32_t node_word, uint32_t child_word) {
      PlayNamed(calls, frame, lr, movie, 13);
      guest::StoreU32(base, ui_movies::FindNode(base, movie, 0, 0) + 80, node_word);
      const uint32_t node = ui_movies::FindNode(base, movie, 0, 2);
      guest::StoreU32(base, ui_movies::Child(base, node, 0) + 80, child_word);
    };
    if (mode == 0) {
      marks(0x82218E54, 0, 1);
    } else if (mode == 1) {
      PlayNamed(calls, frame, 0x82218DF0, movie, 12);
      guest::StoreU32(base, frame + 84, 159);
      guest::StoreU32(base, frame + 92, 158);
      const uint64_t init[8] = {Get(base, kDialogList), 2, 2, 1, 154, 153, 155, 156};
      calls.CallArgs(kListInit, frame, 0x82218E24, init);
      ListSetSelection(base, Get(base, kDialogList),
                       guest::LoadU32(base, Get(base, kDialogData) + 28));
    } else if (mode == 2) {
      marks(0x82218D90, 1, 0);
      guest::StoreU32(base, Get(base, kDialogData) + 24, 0);
    }
    calls.Call(kMenuSoundPlay, frame, 0x82218E94, 5);
  }
  const uint64_t done = uint32_t(Run(calls, frame, 0x82218EA0, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    const int32_t mode = int32_t(guest::LoadU32(base, Get(base, kDialogData) + 16));
    if (mode == 1) {
      const uint64_t named = uint64_t(ListSelection(base, Get(base, kDialogList))) + 2;
      PlayNamed(calls, frame, 0x82218F14, movie, named);
      calls.Call(kLoopNamed, frame, 0x82218F20, movie, named);
    } else if (mode == 2) {
      PlayNamed(calls, frame, 0x82218EE0, movie, 0);
      calls.Call(kLoopNamed, frame, 0x82218F20, movie, 0);
    }
  }
  return done;
}

}  // namespace race_screens
