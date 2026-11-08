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
  void Run(uint32_t lr) const { calls.Call(kRun, frame, lr, m, 0); }
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

}  // namespace options_screen
