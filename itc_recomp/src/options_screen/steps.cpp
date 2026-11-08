// The options screen's steps on its movie (docs/functions/options-screen.md):
// see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "native/buffers.h"
#include "ui_movies/game.h"

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
  void Play(uint32_t lr, uint64_t sub, uint64_t index, uint64_t r6 = 0) const {
    calls.Call(kPlay, frame, lr, m, sub, index, r6);
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
  // The help line (sub-scene 2's child 0): SetText with string `id` (its
  // unset varargs r7 to r10 passed as 0, the user's call, 2026-10-30),
  // ClearText, and the scroll reset (sub_8211C5F8).
  void SetHelp(uint32_t lr, uint64_t id) const { calls.Call(kSetText, frame, lr, m, 2, 0, id); }
  void ClearHelp(uint32_t lr) const { calls.Call(kClearText, frame, lr, m, 2, 0); }
  void ResetHelpScroll(uint32_t lr) const { calls.Call(kResetTextScroll, frame, lr, m, 2, 0); }
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

constexpr uint32_t kMinusOne = 0xFFFFFFFFu;
// -1 as the originals' `li` leaves it in a 64-bit register.
constexpr uint64_t kMinusOne64 = ~uint64_t(0);
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

namespace {

// The options' menu's cursor (+0), the cursor before (+4) and the direction
// (+8), as the steps load them.
struct Cursor2 {
  uint32_t now, before;
};
Cursor2 Cursors(uint8_t* base) {
  const uint32_t menu = LoadU32(base, kMenu);
  return {LoadU32(base, menu + 0), LoadU32(base, menu + 4)};
}
int32_t Direction(uint8_t* base) { return int32_t(LoadU32(base, LoadU32(base, kMenu) + 8)); }

// The category table (0x8200A578, 7 words a context) at the context `o` +4
// and the menu's cursor, both loaded again for each read as the original.
uint32_t Category(uint8_t* base) {
  const uint32_t index = LoadU32(base, Settings(base) + 4) * 7 + LoadU32(base, LoadU32(base, kMenu));
  return LoadU32(base, 0x8200A578 + (index << 2));
}

}  // namespace

uint64_t MoveCategoryCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C2BE0 Options::MoveCategoryCursor");
  return Step(base, step, sp, 160, 0x821C2E18, calls, [&](const Movie& m, uint32_t frame) {
    const Cursor2 c = Cursors(base);
    const uint32_t title = ui_movies::FindNode(base, m.m, kMinusOne, 28);
    StoreU32(base, title + 108, Category(base));
    const uint32_t title2 = ui_movies::FindNode(base, m.m, kMinusOne, 29);
    StoreU32(base, title2 + 108, Category(base));
    m.Finish(0x821C2CB4, kMinusOne64, 2);
    m.Play(0x821C2CD4, kMinusOne64, 2, 1);
    const uint32_t menu = LoadU32(base, kMenu);
    m.Finish(0x821C2CEC, 21, uint64_t(LoadU32(base, menu + 4)) + 2);
    m.Play(0x821C2D14, 21, uint64_t(LoadU32(base, LoadU32(base, kMenu) + 0)) + 2, 1);
    m.Loop(0x821C2D2C, 21, uint64_t(LoadU32(base, LoadU32(base, kMenu) + 0)) + 2);
    m.Finish(0x821C2D3C, 21, 12);
    m.Finish(0x821C2D4C, 21, 13);
    const int32_t last = int32_t(LoadU32(base, LoadU32(base, kMenu) + 16));
    const int32_t now = int32_t(c.now), before = int32_t(c.before);
    if ((now == 0 && before == last) || (now == last && before == 0)) {
      SetCategoryArrow(base, uint64_t(c.now) * 40, frame);
    } else {
      const int32_t direction = Direction(base);
      if (direction == 1) {
        m.Play(0x821C2DC0, 21, 13);
      } else if (direction == -1) {
        m.Play(0x821C2DC0, 21, 12);
      }
    }
    m.SetHelp(0x821C2DF4, uint64_t(Category(base)) + 982);
    m.ResetHelpScroll(0x821C2E04);
  });
}

uint64_t MoveHudCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C2E28 Options::MoveHudCursor");
  return Step(base, step, sp, 160, 0x821C2FF8, calls, [&](const Movie& m, uint32_t frame) {
    const Cursor2 c = Cursors(base);
    const int32_t now = int32_t(c.now), before = int32_t(c.before);
    const auto ok = [&]() { return int32_t(LoadU32(base, Settings(base) + 660)) == 1; };
    if (before < 4) {
      m.Finish(0x821C2E90, 5, uint64_t(c.before) + 4);
    } else {
      m.Finish(0x821C2EB0, 5, ok() ? 1 : 2);
      // Options::SetHudArrow sub_821BE4A0 (never seen in play: the original).
      if (now == 0) {
        calls.Call(kSetHudArrow, frame, 0x821C2ED0, 0);
      } else if (now == 3) {
        calls.Call(kSetHudArrow, frame, 0x821C2ED0, 120);
      }
      StoreU32(base, Settings(base) + 660, 1);
    }
    uint64_t index;
    if (now < 4) {
      index = uint64_t(c.now) + 4;
      m.Play(0x821C2F04, 5, index);
    } else if (ok()) {
      index = 1;
      m.Play(0x821C2F24, 5, index);
    } else {
      index = 2;
      m.Play(0x821C2F34, 5, index);
    }
    m.Loop(0x821C2F44, 5, index);
    m.FinishNamed(0x821C2F50, 15);
    m.FinishNamed(0x821C2F5C, 14);
    if (before != 4 && now != 4) {
      const int32_t direction = Direction(base);
      if (direction == 1) {
        m.PlayNamed(0x821C2FA8, 15);
      } else if (direction == -1) {
        m.PlayNamed(0x821C2FA8, 14);
      }
    }
    if (now < 4) {
      m.SetHelp(0x821C2FC4, uint64_t(c.now) + 989);
      m.ResetHelpScroll(0x821C2FD4);
    } else {
      m.ClearHelp(0x821C2FE8);
    }
  });
}

uint64_t MoveSoundCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C3008 Options::MoveSoundCursor");
  return Step(base, step, sp, 144, 0x821C326C, calls, [&](const Movie& m, uint32_t frame) {
    const Cursor2 c = Cursors(base);
    const auto ok_or_default = [&]() -> uint64_t {
      return int32_t(LoadU32(base, Settings(base) + 660)) == 1 ? 4 : 3;
    };
    // Options::SetSoundArrow sub_821BE7A8 (never seen in play: the original).
    switch (c.before) {
      case 0: m.Finish(0x821C30FC, 6, 1); break;
      case 1: m.Finish(0x821C30FC, 6, 2); break;
      case 2:
        m.Finish(0x821C30B4, 6, ok_or_default());
        if (c.now == 0) {
          calls.Call(kSetSoundArrow, frame, 0x821C30D4, 0);
        } else if (c.now == 1) {
          calls.Call(kSetSoundArrow, frame, 0x821C30D4, 40);
        }
        StoreU32(base, Settings(base) + 660, 0);
        break;
      case 3:
        m.Finish(0x821C307C, 6, 5);
        if (c.now == 0) {
          calls.Call(kSetSoundArrow, frame, 0x821C308C, 0);
        }
        break;
      default: break;
    }
    if (c.now <= 3) {
      static constexpr uint64_t kItems[4] = {1, 2, 0, 5};
      m.Play(0x821C317C, 6, c.now == 2 ? ok_or_default() : kItems[c.now]);
      m.LoopLast(0x821C3184);
    }
    m.FinishNamed(0x821C3190, 89);
    m.FinishNamed(0x821C319C, 90);
    if (c.before != 2 && c.now != 2 && c.before != 3 && c.now != 3) {
      const int32_t direction = Direction(base);
      if (direction == 1) {
        m.PlayNamed(0x821C31F8, 90);
      } else if (direction == -1) {
        m.PlayNamed(0x821C31F8, 89);
      }
    }
    switch (c.now) {
      case 0: m.SetHelp(0x821C3248, 993); break;
      case 1: m.SetHelp(0x821C3248, 994); break;
      case 2: m.ClearHelp(0x821C3228); break;
      case 3: m.SetHelp(0x821C3248, 995); break;
      default: break;
    }
    m.ResetHelpScroll(0x821C3258);
  });
}

uint64_t ChangeSoundValue(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C3278 Options::ChangeSoundValue");
  return Step(base, step, sp, 128, 0x821C3388, calls, [&](const Movie& m, uint32_t frame) {
    if (int32_t(LoadU32(base, LoadU32(base, kMenu) + 0)) == 2) {
      if (int32_t(LoadU32(base, Settings(base) + 660)) == 1) {
        m.Finish(0x821C32E8, 6, 3);
        m.Play(0x821C3318, 6, 4);
      } else {
        m.Finish(0x821C32F8, 6, 4);
        m.Play(0x821C3318, 6, 3);
      }
      m.LoopLast(0x821C3320);
      return;
    }
    const uint32_t music = ui_movies::FindNode(base, m.m, 6, 6);
    const uint32_t effects = ui_movies::FindNode(base, m.m, 6, 7);
    ShowVolumeNumber(base, music, LoadU32(base, Settings(base) + 252), frame, calls);
    ShowVolumeNumber(base, effects, LoadU32(base, Settings(base) + 256), frame, calls);
    ShowMusicName(base, LoadU32(base, Settings(base) + 688), frame, calls);
  });
}

uint64_t MoveSlidersCursor(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C3398 Options::MoveSlidersCursor");
  return Step(base, step, sp, 144, 0x821C35E4, calls, [&](const Movie& m, uint32_t frame) {
    const Cursor2 c = Cursors(base);
    const auto last_row = [&]() -> uint64_t {
      return LoadU32(base, Settings(base) + 660) == 0 ? 229 : 230;
    };
    // Options::SetSlidersArrow sub_821BEDC8 (never seen in play: the
    // original).
    if (c.before < 3) {
      m.FinishNamed(0x821C3468, 226 + c.before);
    } else if (c.before == 3) {
      m.FinishNamed(0x821C341C, last_row());
      StoreU32(base, Settings(base) + 660, 0);
      if (c.now == 0) {
        calls.Call(kSetSlidersArrow, frame, 0x821C3448, 0);
      } else if (c.now == 2) {
        calls.Call(kSetSlidersArrow, frame, 0x821C3448, 80);
      }
    }
    if (c.now <= 3) {
      static constexpr uint32_t kPlayLr[3] = {0x821C3528, 0x821C3504, 0x821C34E0};
      uint64_t named;
      if (c.now < 3) {
        named = 226 + c.now;
        m.PlayNamed(kPlayLr[c.now], named);
      } else {
        named = last_row();
        m.PlayNamed(named == 229 ? 0x821C34AC : 0x821C34BC, named);
      }
      m.LoopNamed(0x821C3534, named);
    }
    m.FinishNamed(0x821C3540, 182);
    m.FinishNamed(0x821C354C, 183);
    if (c.now == 3) {
      m.ClearHelp(0x821C35D0);
      return;
    }
    if (c.before != 3) {
      const int32_t direction = Direction(base);
      if (direction == 1) {
        m.PlayNamed(0x821C3598, 183);
      } else if (direction == -1) {
        m.PlayNamed(0x821C3598, 182);
      }
    }
    m.SetHelp(0x821C35AC, uint64_t(c.now) + 996);
    m.ResetHelpScroll(0x821C35BC);
  });
}

uint64_t ChangeSlidersValue(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C35F0 Options::ChangeSlidersValue");
  return Step(base, step, sp, 112, 0x821C36D4, calls, [&](const Movie& m, uint32_t frame) {
    if (int32_t(LoadU32(base, LoadU32(base, kMenu) + 0)) != 3) {
      ShowSliderNumbers(base, frame, calls);
      return;
    }
    if (LoadU32(base, Settings(base) + 660) == 0) {
      m.FinishNamed(0x821C3664, 230);
      m.PlayNamed(0x821C3680, 229);
      m.LoopNamed(0x821C36B8, 229);
    } else {
      m.FinishNamed(0x821C3690, 229);
      m.PlayNamed(0x821C36AC, 230);
      m.LoopNamed(0x821C36B8, 230);
    }
  });
}

namespace {

// Steps 1 and 2 (the buttons' and analog pages' leave), their tables
// written to the frame on every pass: the edits in menu order at +96 and
// the pictures' two animations at `first` and `second`.
struct LeaveStep {
  uint32_t bytes;          // the frame
  uint32_t edits[12];      // the edits' offsets in menu order
  uint32_t count;          // the items
  uint32_t first, second;  // the pictures' tables' offsets in the frame
  const uint32_t* first_table;
  const uint32_t* second_table;
  uint32_t pictures;       // the tables' words
  uint32_t row;            // the cursor's first animation (407 or 658)
  uint32_t ok, back;       // the last row's (OK/Default 0, else)
  uint32_t copy_to, copy_from, copy_bytes;  // the edits put back
  uint32_t lr[8];  // FinishNamed (the row, the picture's two), the last row's
                   // FinishNamed, memcpy, UiMovie::Run (two unused)
};

uint64_t RunLeave(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls, const LeaveStep& l,
                  bool analog) {
  const uint32_t frame = sp - l.bytes;
  StoreU32(base, frame, sp);
  const Movie m{calls, frame, FindMovie(base, frame)};
  const uint32_t o = Settings(base);
  for (uint32_t i = 0; i < l.count; i++) {
    StoreU32(base, frame + 96 + 4 * i, LoadU32(base, o + l.edits[i]));
  }
  for (uint32_t i = 0; i < l.pictures; i++) {
    StoreU32(base, frame + l.first + 4 * i, l.first_table[i]);
    StoreU32(base, frame + l.second + 4 * i, l.second_table[i]);
  }
  if (LoadU32(base, step) == 0) {
    StoreU32(base, o + 684, 0);
    const uint32_t menu = LoadU32(base, kMenu);
    const uint32_t cursor = LoadU32(base, menu + 0);
    if (cursor != l.count) {
      m.FinishNamed(l.lr[0], uint64_t(cursor) - uint64_t(LoadU32(base, menu + 48)) + l.row);
      uint32_t picture = LoadU32(base, frame + 96 + (cursor << 2));
      if (analog) {
        picture = AnalogPicture(picture);
      }
      const uint32_t index = picture << 2;
      m.FinishNamed(l.lr[1], LoadU32(base, frame + l.first + index));
      m.FinishNamed(l.lr[2], LoadU32(base, frame + l.second + index));
      if (analog) {
        ShowAnalogPicture(base, kMinusOne, frame);
      } else {
        ShowButtonPicture(base, kMinusOne, frame);
      }
    } else {
      m.FinishNamed(l.lr[3], LoadU32(base, Settings(base) + 660) != 0 ? l.back : l.ok);
    }
    const uint32_t settings = Settings(base);
    const uint32_t to = settings + l.copy_to, from = settings + l.copy_from;
    if (!native::Buffers_Memcpy(base, to, from, l.copy_bytes, l.lr[4])) {
      calls.Call(kMemcpy, frame, l.lr[4], to, from, l.copy_bytes);
    }
    if (analog) {
      ShowAnalogList(base, frame, calls);
      ShowAnalogClashes(base, frame, calls);
    } else {
      ShowButtonList(base, frame, calls);
      ShowButtonClashes(base, frame, calls);
    }
    StoreU32(base, step, 1);
  }
  m.Run(l.lr[5]);
  const uint32_t settings = Settings(base);
  StoreU32(base, settings + 684, LoadU32(base, settings + 684) + 1);
  return int32_t(LoadU32(base, Settings(base) + 684)) >= 10 ? 1 : 0;
}

}  // namespace

uint64_t LeaveButtons(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C2778 Options::LeaveButtons");
  static constexpr uint32_t kFirst[17] = {422, 424, 426, 428, 430, 432, 434, 436, 438,
                                          440, 442, 444, 446, 448, 450, 452, 454};
  static constexpr uint32_t kSecond[17] = {423, 425, 427, 429, 431, 433, 435, 437, 439,
                                           441, 443, 445, 447, 449, 451, 453, 455};
  static constexpr LeaveStep kLeave = {
      336, {284, 288, 292, 296, 300, 304, 320, 312, 316, 308, 324, 328},
      12, 144, 224, kFirst, kSecond, 17, 407, 353, 354, 284, 92, 48,
      {0x821C2958, 0x821C2978, 0x821C2988, 0x821C29B0, 0x821C29C4, 0x821C29E0, 0, 0}};
  return RunLeave(base, step, sp, calls, kLeave, false);
}

uint64_t LeaveAnalog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C2A10 Options::LeaveAnalog");
  // Seven words each (the others' tables have 11: picture 10 reads past
  // them, as the original).
  static constexpr uint32_t kFirst[7] = {673, 675, 677, 679, 681, 683, 693};
  static constexpr uint32_t kSecond[7] = {674, 676, 678, 680, 682, 684, 694};
  static constexpr LeaveStep kLeave = {
      240, {380, 384, 388, 396, 392, 400}, 6, 128, 160, kFirst, kSecond, 7, 658, 604, 605,
      380, 188, 24,
      {0x821C2B20, 0x821C2B48, 0x821C2B58, 0x821C2B80, 0x821C2B94, 0x821C2BB0, 0, 0}};
  return RunLeave(base, step, sp, calls, kLeave, true);
}

namespace {

// The category pages' closes (steps 12, 16, 20, 28, 36): from the 112-byte
// frame, once (the step word 0) the cursor's animation finished (`finish`),
// the help line cleared, the page's close (`close`) played on sub-scene -1
// and Options::SetUpCategoryPage; then every pass UiMovie::Run, and once it
// answers 0 the page block to step 5 with the categories after and the step
// word 0. 0 (the next step is set here).
template <typename Finish>
uint64_t Close(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls,
               const uint32_t (&lr)[4], uint64_t close, Finish finish) {
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);
  const Movie m{calls, frame, FindMovie(base, frame)};
  if (LoadU32(base, step) == 0) {
    finish(m);
    m.ClearHelp(lr[0]);
    m.Play(lr[1], kMinusOne64, close);
    SetUpCategoryPage(base, frame, calls);
    StoreU32(base, step, 1);
  }
  if (int32_t(m.Run(lr[2])) == 0) {
    const uint32_t p = LoadU32(base, kPage);
    const uint32_t page = LoadU32(base, p + 0);
    if (page != 0) {
      StoreU32(base, p + 8, page);
    }
    StoreU32(base, p + 16, kMinusOne);
    StoreU32(base, p + 4, 0);
    StoreU32(base, p + 12, 5);
    StoreU32(base, p + 0, kMinusOne);
    StoreU32(base, LoadU32(base, kPage) + 24, 0);
  }
  return 0;
}

}  // namespace

uint64_t FadeIn(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C7080 Options::FadeIn");
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);
  const Movie m{calls, frame, FindMovie(base, frame)};
  if (LoadU32(base, step) == 0) {
    m.Play(0x821C70E0, kMinusOne64, 1);
    m.Play(0x821C7100, kMinusOne64, 0);
    m.Loop(0x821C7110, kMinusOne64, 0);
    // ScreenFades::FadeIn([0x82001BEC], black, layer 0).
    mode_tick::ScreenFadesFadeIn(base, LoadU32(base, 0x82001BEC), 0, 0, 0, 0);
    SetUpCategoryPage(base, frame, calls);
    StoreU32(base, step, 1);
  }
  const uint32_t run = uint32_t(m.Run(0x821C7144));
  const uint32_t fades =
      LoadU32(base, mode_tick::kFadeOutCounts) | LoadU32(base, mode_tick::kFadeInCounts);
  return run == 0 && fades == 0 ? 1 : 0;
}

uint64_t CloseHud(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C75E0 Options::CloseHud");
  static constexpr uint32_t kLr[4] = {0x821C7674, 0x821C7694, 0x821C76AC, 0};
  return Close(base, step, sp, calls, kLr, 4, [&](const Movie& m) {
    const uint64_t cursor = Cursor(base);
    uint64_t index;
    if (int32_t(cursor) < 4) {
      index = cursor + 4;
    } else {
      index = LoadU32(base, Settings(base) + 660) != 0 ? 1 : 2;
    }
    m.Finish(0x821C7664, 5, index);
  });
}

uint64_t CloseSound(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C7710 Options::CloseSound");
  static constexpr uint32_t kLr[4] = {0x821C77D0, 0x821C77F0, 0x821C7808, 0};
  return Close(base, step, sp, calls, kLr, 5, [&](const Movie& m) {
    switch (uint32_t(Cursor(base))) {
      case 0: m.Finish(0x821C77C0, 6, 1); break;
      case 1: m.Finish(0x821C77C0, 6, 2); break;
      case 2:
        m.Finish(0x821C77C0, 6, int32_t(LoadU32(base, Settings(base) + 660)) == 1 ? 4 : 3);
        break;
      case 3: m.Finish(0x821C77C0, 6, 5); break;
      default: break;
    }
  });
}

uint64_t CloseSliders(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C7868 Options::CloseSliders");
  static constexpr uint32_t kLr[4] = {0x821C7920, 0x821C7940, 0x821C7958, 0};
  return Close(base, step, sp, calls, kLr, 6, [&](const Movie& m) {
    const uint32_t cursor = uint32_t(Cursor(base));
    if (cursor < 3) {
      m.FinishNamed(0x821C7910, 226 + cursor);
    } else if (cursor == 3) {
      m.FinishNamed(0x821C7910, LoadU32(base, Settings(base) + 660) == 0 ? 229 : 230);
    }
  });
}

uint64_t CloseButtons(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C7B08 Options::CloseButtons");
  static constexpr uint32_t kLr[4] = {0x821C7B58, 0x821C7B78, 0x821C7B90, 0};
  return Close(base, step, sp, calls, kLr, 8, [](const Movie&) {});
}

uint64_t CloseAnalog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C7D18 Options::CloseAnalog");
  static constexpr uint32_t kLr[4] = {0x821C7D68, 0x821C7D88, 0x821C7DA0, 0};
  return Close(base, step, sp, calls, kLr, 12, [](const Movie&) {});
}

}  // namespace options_screen
