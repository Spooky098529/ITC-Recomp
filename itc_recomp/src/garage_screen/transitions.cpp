// The garage screen's transitions seen in play (docs/functions/garage-screen.md):
// see garage_screen.h. The others (Update's table) stay the originals.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"
#include "race_screens/race_screens.h"

namespace garage_screen {

namespace {

// The animation `named` played (r5 to r8 0) and, with `loop_lr`, looped.
void PlayLoop(uint32_t movie, uint64_t named, uint32_t frame, uint32_t play_lr,
              uint32_t loop_lr, const Calls& calls) {
  calls.Call(kPlayNamed, frame, play_lr, movie, named, 0, 0);
  calls.Call(kLoopNamed, frame, loop_lr, movie, named);
}

}  // namespace

uint64_t Transition0(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82195BA0 GarageScreen::Transition0");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    calls.Call(kPlayNamed, frame, 0x82195BFC, movie, 131, 0, 0);
    PlayLoop(movie, 128, frame, 0x82195C18, 0x82195C24, calls);
    PlayLoop(movie, 125, frame, 0x82195C40, 0x82195C4C, calls);
    PlayLoop(movie, 126, frame, 0x82195C68, 0x82195C74, calls);
    PlayLoop(movie, 45, frame, 0x82195C90, 0x82195C9C, calls);
    PlayLoop(movie, 49, frame, 0x82195CB8, 0x82195CC4, calls);
    const uint32_t data = Data(base);
    Store(base, data + 44, 0);
    Store(base, data + 48, 0);
    SetUpLists(base, frame, calls);
    Store(base, started, 1);
  }
  return uint32_t(calls.Call(kRun, frame, 0x82195CF0, movie, 0)) == 0 ? 1 : 0;
}

uint64_t Transition1(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192D60 GarageScreen::Transition1");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint64_t named = uint64_t(race_screens::ListSelection(base, Load(base, kList))) + 67;
    PlayLoop(movie, named, frame, 0x82192DC8, 0x82192DD4, calls);
    PlayLoop(movie, 77, frame, 0x82192DF0, 0x82192DFC, calls);
    Store(base, started, 1);
  }
  calls.Call(kRun, frame, 0x82192E10, movie, 0);
  return 1;
}

uint64_t Transition3(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192E20 GarageScreen::Transition3");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  if (Load(base, started) == 0) {
    const uint64_t named = uint64_t(race_screens::ListSelection(base, Load(base, kList))) + 67;
    calls.Call(kFinishNamed, frame, 0x82192E7C, movie, named);
    calls.Call(kFinishNamed, frame, 0x82192E88, movie, 77);
    calls.Call(kFinishNamed, frame, 0x82192E94, movie, 128);
    calls.Call(kFadeOut, frame, 0x82192EB0, Load(base, 0x82001BF0), 0, 0, 0);
    Store(base, started, 1);
  }
  calls.Call(kRun, frame, 0x82192EC4, movie, 0);
  // Done once both fades' counts are 0.
  return (Load(base, 0x826A50E4) | Load(base, 0x826A50DC)) == 0 ? 1 : 0;
}

}  // namespace garage_screen
