// The type 2 dialog's steps (docs/functions/type2-dialog.md): the ten Update
// runs from its table, each over its dialog's movie (16 to 19), r3 the page
// object's started word. UiMovies::Find is its plain C++; MenuSound::Play
// and the movie's calls go through their overrides, from frames like the
// originals'.

#include "type2_dialog/type2_dialog.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace type2_dialog {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// The original's frame below `sp`, its back chain written.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// UiMovies::Find with the id stored at the frame's +80: the movie.
uint32_t FindMovie(uint8_t* base, uint32_t frame, uint32_t id) {
  StoreU32(base, frame + 80, id);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

// The highlights' table {1, 2} at the frame's +80 (stored +80 then +84, or
// the other order) and the entry for the menu's word at `field` (+0 the
// cursor, +4 the previous one).
uint32_t Highlight(uint8_t* base, uint32_t frame, uint32_t field, bool high_first) {
  if (high_first) {
    StoreU32(base, frame + 84, 2);
    StoreU32(base, frame + 80, 1);
  } else {
    StoreU32(base, frame + 80, 1);
    StoreU32(base, frame + 84, 2);
  }
  const uint32_t index = LoadU32(base, LoadU32(base, kMenu) + field);
  return LoadU32(base, frame + 80 + (index << 2));
}

uint32_t Done(uint64_t run) { return uint32_t(run) == 0 ? 1 : 0; }

}  // namespace

uint64_t OpenConfirmStep(uint8_t* base, uint32_t started, bool second, uint32_t sp,
                         const Calls& calls) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_821879F8 Type2Dialog::OpenConfirm2Step");
  } else {
    ITC_NATIVE_RUNNING("sub_82187728 Type2Dialog::OpenConfirmStep");
  }
  // The calls' return addresses: Play(5), PlayNamed, Run, PlayNamed, LoopLast.
  static constexpr uint32_t kLr[2][5] = {
      {0x8218776C, 0x82187788, 0x82187798, 0x821877E0, 0x821877E8},
      {0x82187A3C, 0x82187A58, 0x82187A68, 0x82187AB0, 0x82187AB8}};
  const uint32_t* lr = kLr[second ? 1 : 0];
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame, second ? 18 : 16);
  if (LoadU32(base, started) == 0) {
    calls.Call(kMenuSoundPlay, frame, lr[0], 5);
    calls.CallArgs(kPlayNamed, frame, lr[1], {movie, 5, 0, 0, 0, 0, 0, 0});
    StoreU32(base, started, 1);
  }
  const uint32_t done = Done(calls.Call(kRun, frame, lr[2], movie, 0));
  if (done != 0) {
    const uint32_t named = Highlight(base, frame, 0, false);
    calls.CallArgs(kPlayNamed, frame, lr[3], {movie, named, 0, 0, 0, 0, 0, 0});
    calls.Call(kLoopLast, frame, lr[4], movie);
  }
  return done;
}

uint64_t CloseConfirmStep(uint8_t* base, uint32_t started, bool second, uint32_t sp,
                          const Calls& calls) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82187AC8 Type2Dialog::CloseConfirm2Step");
  } else {
    ITC_NATIVE_RUNNING("sub_821877F8 Type2Dialog::CloseConfirmStep");
  }
  // Play(5), PlayNamed, Run, FinishAll.
  static constexpr uint32_t kLr[2][4] = {{0x82187840, 0x8218785C, 0x82187870, 0x82187884},
                                         {0x82187B10, 0x82187B2C, 0x82187B40, 0x82187B64}};
  const uint32_t* lr = kLr[second ? 1 : 0];
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame, second ? 18 : 16);
  if (LoadU32(base, started) == 0) {
    calls.Call(kMenuSoundPlay, frame, lr[0], 5);
    calls.CallArgs(kPlayNamed, frame, lr[1], {movie, 6, 0, 0, 0, 0, 0, 0});
    StoreU32(base, started, 1);
  }
  const uint32_t done = Done(calls.Call(kRun, frame, lr[2], movie, 0));
  if (done != 0) {
    if (second) {
      StoreU32(base, LoadU32(base, kData) + 4, 9);
      calls.Call(kFinishAll, frame, lr[3], movie);
    } else {
      calls.Call(kFinishAll, frame, lr[3], movie);
      StoreU32(base, LoadU32(base, kData) + 4, 9);
    }
  }
  return done;
}

uint64_t NoticeStep(uint8_t* base, uint32_t started, bool second, bool close, uint32_t sp,
                    const Calls& calls) {
  if (second) {
    if (close) {
      ITC_NATIVE_RUNNING("sub_82187C18 Type2Dialog::CloseNotice2Step");
    } else {
      ITC_NATIVE_RUNNING("sub_82187B80 Type2Dialog::OpenNotice2Step");
    }
  } else if (close) {
    ITC_NATIVE_RUNNING("sub_82187948 Type2Dialog::CloseNoticeStep");
  } else {
    ITC_NATIVE_RUNNING("sub_821878B0 Type2Dialog::OpenNoticeStep");
  }
  // Play(5), PlayNamed, Run: OpenNotice, CloseNotice, OpenNotice2,
  // CloseNotice2.
  static constexpr uint32_t kLr[4][3] = {{0x821878F8, 0x82187914, 0x82187928},
                                         {0x82187990, 0x821879AC, 0x821879C0},
                                         {0x82187BC8, 0x82187BE4, 0x82187BF8},
                                         {0x82187C60, 0x82187C7C, 0x82187C90}};
  const uint32_t* lr = kLr[(second ? 2 : 0) + (close ? 1 : 0)];
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame, second ? 19 : 17);
  if (LoadU32(base, started) == 0) {
    calls.Call(kMenuSoundPlay, frame, lr[0], 5);
    calls.CallArgs(kPlayNamed, frame, lr[1], {movie, close ? 3u : 2u, 0, 0, 0, 0, 0, 0});
    StoreU32(base, started, 1);
  }
  const uint32_t done = Done(calls.Call(kRun, frame, lr[2], movie, 0));
  if (close && done != 0) {
    StoreU32(base, LoadU32(base, kData) + 4, 9);
  }
  return done;
}

uint64_t MoveCursorStep(uint8_t* base, uint32_t started, bool second, uint32_t sp,
                        const Calls& calls) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82187D80 Type2Dialog::MoveCursor2Step");
  } else {
    ITC_NATIVE_RUNNING("sub_82187CC8 Type2Dialog::MoveCursorStep");
  }
  // FinishNamed, PlayNamed, LoopLast or LoopNamed, Run.
  static constexpr uint32_t kLr[2][4] = {{0x82187D30, 0x82187D5C, 0x82187D64, 0x82187D74},
                                         {0x82187DE8, 0x82187E14, 0x82187E30, 0x82187E40}};
  const uint32_t* lr = kLr[second ? 1 : 0];
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame, second ? 18 : 16);
  if (LoadU32(base, started) == 0) {
    const uint32_t previous = Highlight(base, frame, 4, true);
    calls.Call(kFinishNamed, frame, lr[0], movie, previous);
    const uint32_t cursor = LoadU32(base, LoadU32(base, kMenu) + 0);
    calls.CallArgs(kPlayNamed, frame, lr[1],
                   {movie, LoadU32(base, frame + 80 + (cursor << 2)), 0, 0, 0, 0, 0, 0});
    if (second) {
      const uint32_t again = LoadU32(base, LoadU32(base, kMenu) + 0);
      calls.Call(kLoopNamed, frame, lr[2], movie, LoadU32(base, frame + 80 + (again << 2)));
    } else {
      calls.Call(kLoopLast, frame, lr[2], movie);
    }
    StoreU32(base, started, 1);
  }
  calls.Call(kRun, frame, lr[3], movie, 0);
  return 1;
}

}  // namespace type2_dialog
