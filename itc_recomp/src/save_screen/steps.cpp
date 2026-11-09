// The save screen's dialog steps 1 and 2 (docs/functions/save-screen.md):
// see save_screen.h. Steps 0 and 3 to 6 are in src/front_end_screen_helpers/.

#include "base/log.h"
#include "save_screen/internal.h"

namespace save_screen {

uint64_t MoveSelection(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A5118 SaveScreen::MoveSelection");
  Fn f(base, sp, 128, calls);
  const uint32_t movie = FindMovie(base, 44, f.frame + 80);
  if (int32_t(Load(base, step)) == 0) {
    const uint64_t selection = f.Call(kMenuListSelection, 0x821A515C, List(base));
    const uint64_t previous = f.Call(kMenuListPrevious, 0x821A5168, List(base));
    f.Call(kFinishNamed, 0x821A5178, movie, previous + 2);
    const uint64_t named = selection + 2;
    f.Call(kPlayNamed, 0x821A5198, movie, named, 0, 0);
    f.Call(kLoopNamed, 0x821A51A4, movie, named);
    Store(base, step, 1);
  }
  f.Call(kRun, 0x821A51B8, movie, 0);
  return 1;
}

uint64_t CloseDialog(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A5038 SaveScreen::CloseDialog");
  Fn f(base, sp, 128, calls);
  const uint32_t movie = FindMovie(base, 44, f.frame + 80);
  if (int32_t(Load(base, step)) == 0) {
    const uint32_t state = Load(base, Data(base) + 16);
    if (state == 0) {
      const uint64_t selection = f.Call(kMenuListSelection, 0x821A50B0, List(base));
      f.Call(kFinishNamed, 0x821A50C0, movie, selection + 2);
      f.Call(kPlayNamed, 0x821A50DC, movie, 14, 0, 0);
    } else if (state < 3) {
      if (state == 2) {
        f.Call(kFinishNamed, 0x821A509C, movie, 0);
      }
      f.Call(kPlayNamed, 0x821A50DC, movie, 15, 0, 0);
    }
    f.Call(kMenuSoundPlay, 0x821A50E4, 5);
    Store(base, step, 1);
  }
  const bool done = uint32_t(f.Call(kRun, 0x821A50F8, movie, 0)) == 0;
  if (done) {
    const uint32_t data = Data(base);
    Store(base, data + 16, Load(base, data + 20));
  }
  return done ? 1 : 0;
}

}  // namespace save_screen
