// The type 5 dialog's steps (docs/functions/type5-mode.md): the eight of
// Update's table that are its own (steps 2 and 6 are the type 2 dialog's),
// each over its dialog's movie (16 to 19), r3 the page object's started
// word. UiMovies::Find is its plain C++; MenuSound::Play and the movie's
// calls go through their overrides, from frames like the originals'.

#include "type5_mode/type5_mode.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/type5_mode.h"
#include "ui_movies/game.h"

namespace type5_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type5ModeGlobals;

// The original's frame below `sp`, its back chain written.
uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(frame, sp);
  return frame;
}

// UiMovies::Find with the id the original stores at its frame's +80 (read
// only by the find: a value since E5 step 6): the movie.
uint32_t FindMovie(uint32_t id) {
  return LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, id));
}

uint32_t Done(uint64_t run) { return uint32_t(run) == 0 ? 1 : 0; }

}  // namespace

uint64_t OpenConfirmStep(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822261E0 Type5Dialog::OpenConfirmStep");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(16);
  if (LoadU32(started) == 0) {
    calls.Call(kMenuSoundPlay, frame, 0x82226224, 5);
    calls.CallArgs(kPlayNamed, frame, 0x82226240, {movie, 5, 0, 0, 0, 0, 0, 0});
    StoreU32(started, 1);
  }
  const uint32_t done = Done(calls.Call(kRun, frame, 0x82226250, movie, 0));
  if (done != 0) {
    calls.Call(kResetPose, frame, 0x82226264, movie);
    // The highlights' table {1, 2} at the frame's +80 (handed to PlayNamed:
    // it stays on the stack), the cursor's entry.
    StoreU32(frame + 80, 1);
    StoreU32(frame + 84, 2);
    const uint32_t cursor = G::menu().cursor();
    const uint32_t named = LoadU32(frame + 80 + (cursor << 2));
    calls.CallArgs(kPlayNamed, frame, 0x822262A0, {movie, named, 0, 0, 0, 0, 2, frame + 80});
    calls.Call(kLoopLast, frame, 0x822262A8, movie);
  }
  return done;
}

uint64_t CloseConfirmStep(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822262B8 Type5Dialog::CloseConfirmStep");
  // The original's 112-byte frame held only the find's id (a value now):
  // the guest calls run from `sp` (E5 step 6).
  const uint32_t frame = sp;
  const uint32_t movie = FindMovie(16);
  if (LoadU32(started) == 0) {
    calls.Call(kMenuSoundPlay, frame, 0x82226300, 5);
    calls.CallArgs(kPlayNamed, frame, 0x8222631C, {movie, 6, 0, 0, 0, 0, 0, 0});
    StoreU32(started, 1);
  }
  const uint32_t done = Done(calls.Call(kRun, frame, 0x82226330, movie, 0));
  if (done != 0) {
    calls.Call(kFinishAll, frame, 0x82226344, movie);
    G::data().set_queued_state(9);
  }
  return done;
}

uint64_t SimpleStep(uint32_t started, Simple which, uint32_t sp,
                    const Calls& calls) {
  struct Shape {
    uint32_t movie, animation;
    bool close;
    uint32_t lr[3];  // Play(5), PlayNamed, Run
  };
  static constexpr Shape kShapes[4] = {
      {17, 3, true, {0x822263B8, 0x822263D4, 0x822263E8}},
      {18, 5, false, {0x82226468, 0x82226484, 0x82226498}},
      {18, 6, true, {0x82226500, 0x8222651C, 0x82226530}},
      {19, 3, true, {0x822265B0, 0x822265CC, 0x822265E0}},
  };
  switch (which) {
    case Simple::kCloseNotice:
      ITC_NATIVE_RUNNING("sub_82226370 Type5Dialog::CloseNoticeStep");
      break;
    case Simple::kOpenConfirm2:
      ITC_NATIVE_RUNNING("sub_82226420 Type5Dialog::OpenConfirm2Step");
      break;
    case Simple::kCloseConfirm2:
      ITC_NATIVE_RUNNING("sub_822264B8 Type5Dialog::CloseConfirm2Step");
      break;
    case Simple::kCloseNotice2:
      ITC_NATIVE_RUNNING("sub_82226568 Type5Dialog::CloseNotice2Step");
      break;
  }
  const Shape& s = kShapes[static_cast<int>(which)];
  // The original's 112-byte frame held only the find's id (a value now):
  // the guest calls run from `sp` (E5 step 6).
  const uint32_t frame = sp;
  const uint32_t movie = FindMovie(s.movie);
  if (LoadU32(started) == 0) {
    calls.Call(kMenuSoundPlay, frame, s.lr[0], 5);
    calls.CallArgs(kPlayNamed, frame, s.lr[1], {movie, s.animation, 0, 0, 0, 0, 0, 0});
    StoreU32(started, 1);
  }
  const uint32_t done = Done(calls.Call(kRun, frame, s.lr[2], movie, 0));
  if (s.close && done != 0) {
    G::data().set_queued_state(9);
  }
  return done;
}

uint64_t MoveCursorStep(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82226618 Type5Dialog::MoveCursorStep");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(16);
  if (LoadU32(started) == 0) {
    // The highlights' table {1, 2} at the frame's +80 (+84 stored first;
    // handed to the movie's calls: it stays on the stack).
    StoreU32(frame + 84, 2);
    StoreU32(frame + 80, 1);
    const uint32_t previous = G::menu().previous();
    calls.CallArgs(kFinishNamed, frame, 0x82226680,
                   {movie, LoadU32(frame + 80 + (previous << 2)), 0, 0, 0, 0, 2,
                    frame + 80});
    const uint32_t cursor = G::menu().cursor();
    calls.CallArgs(kPlayNamed, frame, 0x822266AC,
                   {movie, LoadU32(frame + 80 + (cursor << 2)), 0, 0, 0, 0, 0, frame + 80});
    calls.Call(kLoopLast, frame, 0x822266B4, movie);
    StoreU32(started, 1);
  }
  calls.Call(kRun, frame, 0x822266C4, movie, 0);
  return 1;
}

uint64_t Step9(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822266D0 Type5Dialog::Step9");
  // The original's 96-byte frame (only its back chain) left out: the call
  // runs from `sp` (E5 step 6).
  calls.Call(kMenuSoundPlay, sp, 0x822266E4, 5);
  return 1;
}

}  // namespace type5_mode
