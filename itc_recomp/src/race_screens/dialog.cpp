// The confirm dialog's pages, transitions and draw
// (docs/functions/race-screens.md): see race_screens.h.

#include "race_screens/race_screens.h"
#include "race_screens/ui_node.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "schema/save_interface.h"
#include "schema/type10_mode.h"
#include "replay_snapshots/replay_snapshots.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <optional>

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

using G = schema::RaceScreensGlobals;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  guest::StoreU32(sp - bytes, sp);
  return sp - bytes;
}

// UiMovies::Find(14) as the originals make it: the id stored at `key` (a
// frame word), the movie table's pair for it, its movie. `pair` gets the
// pair's address (the originals keep it in a register).
uint32_t DialogMovie(uint32_t key, uint32_t* pair = nullptr) {
  guest::StoreU32(key, 14);
  const uint32_t found = ui_movies::PairsFind(ui_movies::kMovieTable, key);
  if (pair != nullptr) {
    *pair = found;
  }
  return guest::LoadU32(found);
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

uint64_t RaceDialogQuestion(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82217F40 RaceDialog::Question");
  const uint32_t frame = Frame(sp, 112);
  calls.Call(kListUpdate, frame, 0x82217F58, G::dialog_list().address);
  const uint32_t list = G::dialog_list().address;
  if (schema::MenuList(list).move() != 0) {
    Go(calls, frame, 0x82217F80, G::dialog_pages().address, 1, 2, kNone);
    calls.Call(kMenuSoundPlay, frame, 0x82217F88, 4);
  } else if (uint32_t(calls.Call(kListAccepted, frame, 0x82217F90, list)) != 0) {
    const uint32_t selection = ListSelection(G::dialog_list().address);
    calls.Call(kMenuSoundPlay, frame, 0x82217FAC, 0);
    const schema::RaceDialogData data = G::dialog_data();
    const int32_t mode = int32_t(data.message());
    // Back to the question (page 1, transition 1, then 0): the mode 6, +20 1.
    auto ask_again = [&](uint32_t lr) {
      Go(calls, frame, lr, G::dialog_pages().address, 1, 1, 0);
      G::dialog_data().set_message(6);
      G::dialog_data().set_next_open_style(1);
    };
    // The device asked for again: page 3, SaveSlots::RequestDevice(0)
    // through its override.
    auto request_device = [&]() {
      Go(calls, frame, 0x822180E4, G::dialog_pages().address, 3, kNone, kNone);
      calls.Call(kRequestDevice, frame, 0x822180EC, 0);
    };
    switch (mode) {
      case 1:
        if (selection == 0) {
          Go(calls, frame, 0x8221811C, G::dialog_pages().address, 4, kNone, kNone);
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
          data.set_unknown_24(0);
          Go(calls, frame, 0x82218094, G::dialog_pages().address, 2, 1, 0);
          G::dialog_data().set_message(9);
          G::dialog_data().set_next_open_style(2);
          G::dialog_data().set_unknown_8(0);
          // Replays::Delete (never seen in play): the original.
          calls.Call(kReplaysDelete, frame, 0x822180C0,
                     G::dialog_data().slot());
        }
        break;
      case 6:
        if (selection == 0) {
          Go(calls, frame, 0x8221811C, G::dialog_pages().address, 5, 1, kNone);
        } else {
          data.set_unknown_24(0);
          Go(calls, frame, 0x82218054, G::dialog_pages().address, 2, 1, 0);
          G::dialog_data().set_message(0);
          G::dialog_data().set_next_open_style(2);
        }
        break;
      case 8:
        if (selection == 0) {
          Go(calls, frame, 0x8221811C, G::dialog_pages().address, 5, 1, kNone);
        } else {
          request_device();
        }
        break;
      case 13:
        data.set_yes(selection == 0 ? 1 : 0);
        Go(calls, frame, 0x8221811C, G::dialog_pages().address, 5, 1, kNone);
        break;
      default:
        break;
    }
  }
  calls.Call(kListCancelled, frame, 0x82218158, G::dialog_list().address);
  return 0;
}

uint64_t RaceDialogOpenAnim(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822186A8 RaceDialog::OpenAnim");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = DialogMovie(frame + 80);
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    const uint32_t mode = G::dialog_data().open_style();
    if (mode == 0) {
      PlayNamed(calls, frame, 0x82218730, movie, 15);
    } else if (mode == 1) {
      const uint32_t selection = ListSelection(G::dialog_list().address);
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
  const schema::RaceDialogData data = G::dialog_data();
  data.set_open_style(data.next_open_style());
  return 1;
}

uint64_t RaceDialogMove(uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218788 RaceDialog::Move");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = DialogMovie(frame + 80);
  if (guest::LoadU32(step) == 0) {
    guest::StoreU32(step, 1);
    const uint32_t selection = ListSelection(G::dialog_list().address);
    // MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
    const uint32_t previous = G::dialog_list().selection_before();
    calls.Call(kFinishNamed, frame, 0x822187F0, movie, uint64_t(previous) + 2);
    PlayNamed(calls, frame, 0x82218810, movie, uint64_t(selection) + 2);
    calls.Call(kLoopNamed, frame, 0x8221881C, movie, uint64_t(selection) + 2);
  }
  Run(calls, frame, 0x82218828, movie);
  return 1;
}

uint64_t RaceDialogDraw(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218F30 RaceDialog::Draw");
  const uint32_t frame = Frame(sp, 128);
  uint32_t pair = 0;
  const uint32_t movie = DialogMovie(frame + 80, &pair);
  const uint32_t pages = G::dialog_pages().address;
  // The transitions' table: ShowMessage, OpenAnim, Move.
  const uint32_t table[3] = {0x82218C10, 0x822186A8, 0x82218788};
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(frame + 88 + 4 * i, table[i]);
  }
  const int32_t transition = int32_t(schema::PageObject(pages).step());
  if (transition == -1) {
    return Run(calls, frame, 0x82218FE0, movie);
  }
  if (transition < 0) {
    PagesFinish(pages, 1);
    return pages;
  }
  // The handler (any index read from the frame as the original reads it),
  // with the registers the original has at its bctrl.
  const uint32_t handler = guest::LoadU32((uint32_t(transition) << 2) + frame + 88);
  const uint32_t step = schema::PageObject(pages).step_started_address();
  const uint64_t regs[8] = {step, uint64_t(frame) + 80, step,
                            uint64_t(int64_t(int32_t(table[0]))),
                            uint64_t(int64_t(int32_t(table[1]))),
                            uint64_t(int64_t(int32_t(table[2]))), pair, uint64_t(frame) + 88};
  const uint64_t result = calls.CallArgs(handler, frame, 0x82218FBC, regs);
  const uint32_t now = G::dialog_pages().address;
  PagesFinish(now, uint32_t(result));
  return now;
}

uint64_t RaceDialogSetText(uint64_t id, const uint64_t varargs[7], uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218838 RaceDialog::SetText");
  constexpr uint32_t kSetNodeTextV = 0x8211CBD8;
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(sp + 24 + 8 * i, uint32_t(varargs[i] >> 32));
    guest::StoreU32(sp + 28 + 8 * i, uint32_t(varargs[i]));
  }
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = DialogMovie(frame + 80);
  const uint32_t node = ui_movies::FindNode(movie, 0, 1);
  const uint32_t first = ui_movies::Child(node, 0);
  const uint32_t second = ui_movies::Child(node, 1);
  const uint32_t third = ui_movies::Child(second, 3);
  const uint32_t fourth = ui_movies::Child(second, 4);
  guest::StoreU32(frame + 80, sp + 24);
  calls.Call(kSetNodeTextV, frame, 0x822188EC, movie, first, id, guest::LoadU32(frame + 80));
  calls.Call(kSetNodeText, frame, 0x822188FC, movie, third, 2);
  return calls.Call(kSetNodeText, frame, 0x8221890C, movie, fourth, 3);
}

uint64_t RaceDialogOpen(uint32_t mode, uint64_t player, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218B30 RaceDialog::Open");
  const uint32_t frame = Frame(sp, 128);
  if (uint32_t(player) < 1) {
    G::dialog_list().set_player(0);
  } else if (uint32_t(player) == 1) {
    G::dialog_list().set_player(1);
  }
  if (mode <= 5) {
    const uint64_t varargs[7] = {player, 0, 0, 0, 0, 0, 0};
    RaceDialogSetText(1954 + mode, varargs, frame, calls);
  }
  G::dialog_data().set_message(13);
  G::dialog_data().set_open_style(1);
  G::dialog_data().set_selection(1);
  const uint64_t r3 = Go(calls, frame, 0x82218BF8, G::dialog_pages().address, 1, 0, kNone);
  G::dialog_data().set_showing(1);
  G::dialog_data().set_close_asked(0);
  return r3;
}

uint64_t RaceDialogSetNote(uint64_t id, const uint64_t varargs[7], uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218918 RaceDialog::SetNote");
  constexpr uint32_t kSetNodeTextV = 0x8211CBD8;
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(sp + 24 + 8 * i, uint32_t(varargs[i] >> 32));
    guest::StoreU32(sp + 28 + 8 * i, uint32_t(varargs[i]));
  }
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = DialogMovie(frame + 80);
  const uint32_t node = ui_movies::Child(ui_movies::FindNode(movie, 0, 2), 1);
  guest::StoreU32(frame + 80, sp + 24);
  return calls.Call(kSetNodeTextV, frame, 0x822189A0, movie, node, id,
                    guest::LoadU32(frame + 80));
}

uint64_t RaceDialogStartSave(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822189B8 RaceDialog::StartSave");
  constexpr uint32_t kSlotState = 0x8228F9E8;
  const uint32_t frame = Frame(sp, 128);
  // SetNote's varargs (r4 to r10, unset by the original) passed as 0, as
  // the user's call for the parking area (2026-10-30): only dead frame words.
  const uint64_t varargs[7] = {0, 0, 0, 0, 0, 0, 0};
  if (int32_t(uint32_t(calls.Call(kSlotState, frame, 0x822189D4, 0))) != 0) {
    RaceDialogSetNote(70, varargs, frame, calls);
    G::dialog_data().set_message(3);
    G::dialog_data().set_unknown_24(0);
  } else {
    RaceDialogSetNote(69, varargs, frame, calls);
    G::dialog_data().set_message(2);
  }
  G::dialog_data().set_open_style(0);
  Go(calls, frame, 0x82218A44, G::dialog_pages().address, 0, 0, kNone);
  guest::StoreU32(frame + 92, 158);
  guest::StoreU32(frame + 84, 159);
  const uint64_t init[8] = {G::dialog_list().address, 1, 1, 1, 154, 153, 155, 156};
  const uint64_t r3 = calls.CallArgs(kListInit, frame, 0x82218A7C, init);
  G::dialog_data().set_yes(0);
  G::dialog_data().set_showing(1);
  G::dialog_data().set_close_asked(0);
  return r3;
}

uint64_t RaceDialogShowSaving(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218AB0 RaceDialog::ShowSaving");
  const uint32_t frame = Frame(sp, 112);
  const uint64_t varargs[7] = {0, 0, 0, 0, 0, 0, 0};
  RaceDialogSetNote(98, varargs, frame, calls);
  G::dialog_data().set_message(12);
  G::dialog_data().set_open_style(0);
  const uint64_t r3 = Go(calls, frame, 0x82218B00, G::dialog_pages().address, 0, 0, kNone);
  G::dialog_data().set_showing(1);
  G::dialog_data().set_close_asked(0);
  return r3;
}

std::optional<uint64_t> RaceDialogShowMessage(uint32_t step, uint32_t sp,
                                              const Calls& calls) {
  const bool first = guest::LoadU32(step) == 0;
  if (first && G::dialog_data().message() <= 11) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82218C10 RaceDialog::ShowMessage");
  const uint32_t frame = Frame(sp, 160);
  const uint32_t movie = DialogMovie(frame + 96);
  if (first) {
    guest::StoreU32(step, 1);
    const uint32_t mode = G::dialog_data().open_style();
    // Modes 0 and 2: the type-0 node 0's +80 and node 2's child 0's +80.
    auto marks = [&](uint32_t lr, uint32_t node_word, uint32_t child_word) {
      PlayNamed(calls, frame, lr, movie, 13);
      UiNode(ui_movies::FindNode(movie, 0, 0)).set_visible(node_word);
      const uint32_t node = ui_movies::FindNode(movie, 0, 2);
      UiNode(ui_movies::Child(node, 0)).set_visible(child_word);
    };
    if (mode == 0) {
      marks(0x82218E54, 0, 1);
    } else if (mode == 1) {
      PlayNamed(calls, frame, 0x82218DF0, movie, 12);
      guest::StoreU32(frame + 84, 159);
      guest::StoreU32(frame + 92, 158);
      const uint64_t init[8] = {G::dialog_list().address, 2, 2, 1, 154, 153, 155, 156};
      calls.CallArgs(kListInit, frame, 0x82218E24, init);
      ListSetSelection(G::dialog_list().address,
                       G::dialog_data().selection());
    } else if (mode == 2) {
      marks(0x82218D90, 1, 0);
      G::dialog_data().set_unknown_24(0);
    }
    calls.Call(kMenuSoundPlay, frame, 0x82218E94, 5);
  }
  const uint64_t done = uint32_t(Run(calls, frame, 0x82218EA0, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    const int32_t mode = int32_t(G::dialog_data().open_style());
    if (mode == 1) {
      const uint64_t named = uint64_t(ListSelection(G::dialog_list().address)) + 2;
      PlayNamed(calls, frame, 0x82218F14, movie, named);
      calls.Call(kLoopNamed, frame, 0x82218F20, movie, named);
    } else if (mode == 2) {
      PlayNamed(calls, frame, 0x82218EE0, movie, 0);
      calls.Call(kLoopNamed, frame, 0x82218F20, movie, 0);
    }
  }
  return done;
}

// --- The replay's save (from the slot lists' SaveReplay) ---

namespace {

// memcpy natively, or through `calls` where its plain C++ refuses.
void CopyBytes(const Calls& calls, uint32_t r1, uint32_t lr, uint32_t destination,
               uint32_t source, uint32_t size) {
  if (!c_runtime::Memcpy(destination, source, size)) {
    calls.Call(kMemcpy, r1, lr, destination, source, size);
  }
}

void CopyWord(uint32_t destination, uint32_t source) {
  guest::StoreU32(destination, guest::LoadU32(source));
}

}  // namespace

uint32_t ReplayCopyFrom(uint32_t replay, uint32_t from, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A66A8 Replay::CopyFrom");
  const uint32_t frame = Frame(sp, 112);
  // Both are kept replays (type10-mode.toml's KeptReplay).
  const schema::KeptReplay d(replay), s(from);
  CopyBytes(calls, frame, 0x821A66CC, d.address, s.address, 52);
  CopyBytes(calls, frame, 0x821A66DC, d.race_words_address(), s.race_words_address(), 32);
  CopyBytes(calls, frame, 0x821A66F0, d.race_word_59610_address(),
            s.race_word_59610_address(), 0x2B534);
  CopyBytes(calls, frame, 0x821A6708, d.stage_address(), s.stage_address(), 80);
  for (uint32_t k = 0; k < 4; k++) {
    CopyWord(d.result_3916_address() + 4 * k, s.result_3916_address() + 4 * k);
  }
  CopyWord(d.race_word_A3AE0_address(), s.race_word_A3AE0_address());
  for (uint32_t k = 0; k < 5; k++) {
    CopyWord(d.race_words_A3AE4_address() + 4 * k, s.race_words_A3AE4_address() + 4 * k);
  }
  CopyWord(d.unfilled_address(), s.unfilled_address());
  replay_snapshots::RaceSnapshotCopy(d.snapshot_address(), s.snapshot_address());
  CopyBytes(calls, frame, 0x821A67CC, d.recording_address(), s.recording_address(),
            0x107AC4);
  return d.address;
}

uint32_t RaceDialogStoreReplay(uint32_t record, uint32_t replay, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82218FF8 RaceDialog::StoreReplay");
  const uint32_t frame = Frame(sp, 112);
  // The save buffers' (save-interface.toml's SaveBuffers) replay record and
  // replay, each read through [0x828161F0] again.
  using SaveGlobals = schema::SaveInterfaceGlobals;
  CopyBytes(calls, frame, 0x8221902C,
            SaveGlobals::save_buffers().replay_record().address, record, 36);
  return ReplayCopyFrom(SaveGlobals::save_buffers().replay(), replay, frame, calls);
}

uint32_t RaceDialogAskSlot(uint32_t slot, uint32_t record, uint32_t replay,
                           uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82219058 RaceDialog::AskSlot");
  const uint32_t frame = Frame(sp, 144);
  G::dialog_data().set_slot(slot);
  // SetText's varargs: r4 and r5 the record and the replay, as the call
  // leaves them; r6 to r10 (unset) 0, as the user's call for the parking
  // area (2026-10-30): only dead frame words.
  const uint64_t varargs[7] = {record, replay, 0, 0, 0, 0, 0};
  if (slot == 0) {
    RaceDialogSetText(75, varargs, frame, calls);
    G::dialog_data().set_message(1);
  } else {
    RaceDialogSetText(77, varargs, frame, calls);
    G::dialog_data().set_message(5);
  }
  G::dialog_data().set_open_style(1);
  Go(calls, frame, 0x822190CC, G::dialog_pages().address, 1, 0, kNone);
  guest::StoreU32(frame + 92, 158);
  guest::StoreU32(frame + 84, 159);
  const uint64_t init[8] = {G::dialog_list().address, 2, 2, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x82219104, init);
  G::dialog_data().set_yes(0);
  G::dialog_data().set_showing(1);
  G::dialog_data().set_close_asked(0);
  return RaceDialogStoreReplay(record, replay, frame, calls);
}

}  // namespace race_screens
