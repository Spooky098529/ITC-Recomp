// The options screen's steps on its movie (docs/functions/options-screen.md):
// see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// The movie's calls from a step's frame, each with the original's return
// address. Registers the originals don't set are passed as 0 (none of the
// callees reads them).
struct Movie {
  const Calls& calls;
  uint32_t frame;
  uint32_t m;
  void Play(uint32_t lr, uint64_t sub, uint64_t index) const {
    calls.Call(kPlay, frame, lr, m, sub, index, 0);
  }
  void Loop(uint32_t lr, uint64_t sub, uint64_t index) const {
    calls.Call(kLoop, frame, lr, m, sub, index);
  }
  void LoopLast(uint32_t lr) const { calls.Call(kLoopLast, frame, lr, m); }
  void PlayNamed(uint32_t lr, uint64_t named) const {
    calls.Call(kPlayNamed, frame, lr, m, named, 0, 0);
  }
  void LoopNamed(uint32_t lr, uint64_t named) const {
    calls.Call(kLoopNamed, frame, lr, m, named);
  }
  void Finish(uint32_t lr, uint64_t sub, uint64_t index) const {
    calls.Call(kFinish, frame, lr, m, sub, index);
  }
  void FinishNamed(uint32_t lr, uint64_t named) const {
    calls.Call(kFinishNamed, frame, lr, m, named);
  }
  uint64_t Run(uint32_t lr) const { return calls.Call(kRun, frame, lr, m, 0); }
};

// A step's frame (`bytes` below `sp`, its back chain), `m`, the first
// pass's work when the step word is 0 (then the word 1), UiMovie::Run(m, 0)
// returning to `run_lr`; 1.
template <typename First>
uint64_t Step(uint8_t* base, uint32_t step, uint32_t sp, uint32_t bytes, uint32_t run_lr,
              const Calls& calls, First first) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  const Movie movie{calls, frame, FindMovie(base, frame)};
  if (LoadU32(base, step) == 0) {
    first(movie, frame);
    StoreU32(base, step, 1);
  }
  movie.Run(run_lr);
  return 1;
}

uint32_t Settings(uint8_t* base) { return LoadU32(base, kSettings); }
// The menu's cursor (+0) as the originals load it (zero-extended), for the
// indexes they add to it.
uint64_t Cursor(uint8_t* base) { return LoadU32(base, LoadU32(base, kMenu) + 0); }

}  // namespace

uint64_t OpenCategories(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BD468 Options::OpenCategories");
  return Step(base, step, sp, 128, 0x821BD578, calls, [&](const Movie& m, uint32_t) {
    m.Play(0x821BD4C0, 23, 1);
    m.Loop(0x821BD4D0, 23, 1);
    const uint32_t context = LoadU32(base, Settings(base) + 4);
    if (context == 1) {
      m.Play(0x821BD5A4, 16, 0);
      m.Loop(0x821BD520, 16, 0);
    } else if (context < 5) {
      m.Play(0x821BD510, 17, 0);
      m.Loop(0x821BD520, 17, 0);
    }
    m.Play(0x821BD54C, 21, Cursor(base) + 2);
    m.Loop(0x821BD564, 21, Cursor(base) + 2);
  });
}

uint64_t ReturnToCategories(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BD5B0 Options::ReturnToCategories");
  return Step(base, step, sp, 128, 0x821BD670, calls, [&](const Movie& m, uint32_t) {
    m.Play(0x821BD608, 23, 1);
    m.Loop(0x821BD618, 23, 1);
    m.Play(0x821BD644, 21, Cursor(base) + 2);
    m.Loop(0x821BD65C, 21, Cursor(base) + 2);
  });
}

uint64_t EnterHud(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BD728 Options::EnterHud");
  return Step(base, step, sp, 128, 0x821BD7F0, calls, [&](const Movie& m, uint32_t) {
    const uint64_t cursor = Cursor(base);
    uint64_t index;
    if (int32_t(cursor) < 4) {
      index = cursor + 4;
      m.Play(0x821BD798, 5, index);
    } else if (int32_t(LoadU32(base, Settings(base) + 660)) == 1) {
      index = 2;
      m.Play(0x821BD7BC, 5, index);
    } else {
      index = 1;
      m.Play(0x821BD7CC, 5, index);
    }
    m.Loop(0x821BD7DC, 5, index);
  });
}

uint64_t ChangeHudValue(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BD800 Options::ChangeHudValue");
  return Step(base, step, sp, 112, 0x821BD8EC, calls, [&](const Movie& m, uint32_t) {
    if (int32_t(Cursor(base)) != 4) {
      return;
    }
    if (int32_t(LoadU32(base, Settings(base) + 660)) == 1) {
      m.Finish(0x821BD878, 5, 2);
      m.Play(0x821BD898, 5, 1);
      m.Loop(0x821BD8D8, 5, 1);
    } else {
      m.Finish(0x821BD8A8, 5, 1);
      m.Play(0x821BD8C8, 5, 2);
      m.Loop(0x821BD8D8, 5, 2);
    }
  });
}

uint64_t EnterSound(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BD908 Options::EnterSound");
  return Step(base, step, sp, 112, 0x821BD984, calls, [&](const Movie& m, uint32_t) {
    m.Play(0x821BD968, 6, 1);
    m.LoopLast(0x821BD970);
  });
}

uint64_t EnterSliders(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BD9A0 Options::EnterSliders");
  return Step(base, step, sp, 112, 0x821BDA1C, calls, [&](const Movie& m, uint32_t) {
    m.PlayNamed(0x821BD9FC, 226);
    m.LoopNamed(0x821BDA08, 226);
  });
}

uint64_t EnterButtons(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BDAD0 Options::EnterButtons");
  // The button pictures' animations (423, 425, ..., 455) at the frame's +96,
  // written on every pass before the step word is read.
  const uint32_t frame = sp - 208;
  for (uint32_t i = 0; i < 17; i++) {
    StoreU32(base, frame + 96 + 4 * i, 423 + 2 * i);
  }
  return Step(base, step, sp, 208, 0x821BDC0C, calls, [&](const Movie& m, uint32_t frame) {
    m.PlayNamed(0x821BDBAC, 407);
    m.LoopNamed(0x821BDBB8, 407);
    const uint32_t picture =
        LoadU32(base, frame + 96 + (LoadU32(base, Settings(base) + 284) << 2));
    m.PlayNamed(0x821BDBEC, picture);
    m.LoopNamed(0x821BDBF8, picture);
  });
}

uint64_t EnterAnalog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BDE88 Options::EnterAnalog");
  // The analog pictures' animations (674, 676, ..., 694) at the frame's +96,
  // written on every pass before the step word is read.
  const uint32_t frame = sp - 176;
  for (uint32_t i = 0; i < 11; i++) {
    StoreU32(base, frame + 96 + 4 * i, 674 + 2 * i);
  }
  return Step(base, step, sp, 176, 0x821BDF9C, calls, [&](const Movie& m, uint32_t frame) {
    m.PlayNamed(0x821BDF34, 658);
    m.LoopNamed(0x821BDF40, 658);
    const uint32_t index = AnalogPicture(LoadU32(base, Settings(base) + 380));
    const uint32_t picture = LoadU32(base, frame + 96 + (index << 2));
    m.PlayNamed(0x821BDF7C, picture);
    m.LoopNamed(0x821BDF88, picture);
  });
}

namespace {

// The dialogs' menu (the second front-end menu, [0x826D8484]).
uint32_t DialogMenu(uint8_t* base) { return LoadU32(base, 0x826D8484); }

// Steps 45 and 48: the dialog menu's cursor before (+4) finished, the new
// one (+0) played, then looped (step 45) or finished (step 48). A 128-byte
// frame.
uint64_t MoveDialogCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls,
                          uint32_t first, const uint32_t lr[3], bool loop, uint32_t run_lr) {
  return Step(base, step, sp, 128, run_lr, calls, [&](const Movie& m, uint32_t) {
    const uint32_t menu = DialogMenu(base);
    const uint64_t before = uint64_t(LoadU32(base, menu + 4)) + first;
    const uint64_t now = uint64_t(LoadU32(base, menu + 0)) + first;
    m.FinishNamed(lr[0], before);
    m.PlayNamed(lr[1], now);
    if (loop) {
      m.LoopNamed(lr[2], now);
    } else {
      m.FinishNamed(lr[2], now);
    }
  });
}

// Steps 44 and 47: on the first pass MenuSound::Play(5) and the dialog's
// animations by its shown state (`state`, the settings' +620 or +640); then
// UiMovie::Run, and once it answers 0 the state = the next (+4) and 1.
struct DialogAnimations {
  uint32_t state;      // the settings' word
  uint32_t first;      // the cursor's first animation
  uint32_t open;       // state 0's
  uint32_t shown;      // states 1 and 2's
  uint32_t closing;    // state 2's finished
  uint32_t lr[5];      // MenuSound::Play, state 0's FinishNamed, states 0 and 1's PlayNamed,
                       // state 2's PlayNamed and FinishNamed
};

uint64_t ChangeDialog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls,
                      const DialogAnimations& a, uint32_t run_lr) {
  const uint32_t frame = sp - 128;
  StoreU32(base, frame, sp);
  const Movie m{calls, frame, FindMovie(base, frame)};
  if (LoadU32(base, step) == 0) {
    calls.Call(kMenuSoundPlay, frame, a.lr[0], 5);
    switch (LoadU32(base, Settings(base) + a.state)) {
      case 0:
        m.FinishNamed(a.lr[1], uint64_t(LoadU32(base, DialogMenu(base) + 0)) + a.first);
        m.PlayNamed(a.lr[2], a.open);
        break;
      case 1: m.PlayNamed(a.lr[2], a.shown); break;
      case 2:
        m.PlayNamed(a.lr[3], a.shown);
        m.FinishNamed(a.lr[4], a.closing);
        break;
      default: break;
    }
    StoreU32(base, step, 1);
  }
  if (uint32_t(m.Run(run_lr)) != 0) {
    return 0;
  }
  const uint32_t o = Settings(base);
  StoreU32(base, o + a.state, LoadU32(base, o + a.state + 4));
  return 1;
}

}  // namespace

uint64_t MoveSaveDialogCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C09B0 Options::MoveSaveDialogCursor");
  static constexpr uint32_t kLr[3] = {0x821C0A04, 0x821C0A24, 0x821C0A30};
  return MoveDialogCursor(base, step, sp, calls, 843, kLr, true, 0x821C0A44);
}

uint64_t MoveCategory6DialogCursor(uint8_t* base, uint32_t step, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C0B50 Options::MoveCategory6DialogCursor");
  static constexpr uint32_t kLr[3] = {0x821C0BA4, 0x821C0BC4, 0x821C0BD0};
  return MoveDialogCursor(base, step, sp, calls, 859, kLr, false, 0x821C0BE4);
}

uint64_t ChangeSaveDialog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C08B0 Options::ChangeSaveDialog");
  static constexpr DialogAnimations kAnimations = {
      620, 843, 855, 856, 841, {0x821C08F4, 0x821C095C, 0x821C0978, 0x821C092C, 0x821C0938}};
  return ChangeDialog(base, step, sp, calls, kAnimations, 0x821C098C);
}

uint64_t ChangeCategory6Dialog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C0A50 Options::ChangeCategory6Dialog");
  static constexpr DialogAnimations kAnimations = {
      640, 859, 871, 872, 857, {0x821C0A94, 0x821C0AFC, 0x821C0B18, 0x821C0ACC, 0x821C0AD8}};
  return ChangeDialog(base, step, sp, calls, kAnimations, 0x821C0B2C);
}

}  // namespace options_screen
