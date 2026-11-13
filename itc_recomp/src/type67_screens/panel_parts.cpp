// The panel screens' group pair and panel groups, and the panel screens'
// vtable slots without guest calls of their own beyond UiMovies::Open and
// Delete (docs/functions/type67-screens.md): see type67_screens.h.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

// --- The panel groups and the group pair ---

uint64_t PanelGroupMovie(uint8_t* base, uint64_t group, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82258FC8 PanelGroup::Movie");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, frame + 80, LoadU32(base, uint32_t(group) + 4));
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

uint64_t PanelGroupRun(uint8_t* base, uint64_t group, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259008 PanelGroup::Run");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, frame + 80, LoadU32(base, uint32_t(group) + 4));
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  return calls.call(kRun, frame, 0x8225903C, movie, 0);
}

uint64_t GroupPairSelectA(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82249C30 GroupPair::SelectA");
  const uint32_t pair = uint32_t(r3);
  StoreU32(base, pair + 4, LoadU32(base, pair + 8));
  return r3;
}

uint64_t GroupPairReset(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249C40 GroupPair::Reset");
  const uint32_t pair = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t group = LoadU32(base, pair + 8);
  StoreU32(base, pair + 4, group);
  const uint32_t movie = uint32_t(PanelGroupMovie(base, group, frame));
  // r4 as PanelGroup::Movie leaves it (its frame's +80).
  return calls.call(kResetPose, frame, 0x82249C60, movie, frame - 96 + 80);
}

uint64_t GroupPairRun(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249BE0 GroupPair::Run");
  return PanelGroupRun(base, LoadU32(base, uint32_t(r3) + 4), sp, calls);
}

uint64_t GroupPairWaitForFade(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82249BA8 GroupPair::WaitForFade");
  if (LoadU32(base, kFadeWait) == 0) {
    return r3;
  }
  if ((LoadU32(base, kFadeCountA) | LoadU32(base, kFadeCountB)) != 0) {
    return r3;
  }
  StoreU32(base, kFadeWait, 0);
  return r3;
}

uint64_t GroupPairFinishAll(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249C70 GroupPair::FinishAll");
  const uint32_t pair = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t movie_a = uint32_t(PanelGroupMovie(base, LoadU32(base, pair + 8), frame));
  calls.call(kFinishAll, frame, 0x82249C90, movie_a, frame - 96 + 80);
  const uint32_t movie_b = uint32_t(PanelGroupMovie(base, LoadU32(base, pair + 12), frame));
  return calls.call(kFinishAll, frame, 0x82249C9C, movie_b, frame - 96 + 80);
}

uint64_t GroupPairDestruct(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82249C20 GroupPair::~GroupPair");
  StoreU32(base, uint32_t(r3), kGroupPairBaseVtable);
  return r3;
}

uint64_t SingleGroupConstruct(uint8_t* base, uint64_t r3, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82249CB0 SingleGroup::SingleGroup");
  StoreU32(base, uint32_t(r3) + 4, group);
  StoreU32(base, uint32_t(r3), kSingleGroupVtable);
  return r3;
}

uint64_t SingleGroupReset(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249CC8 SingleGroup::Reset");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie =
      uint32_t(PanelGroupMovie(base, LoadU32(base, uint32_t(r3) + 4), frame));
  // r4 as PanelGroup::Movie leaves it (its frame's +80).
  return calls.call(kResetPose, frame, 0x82249CE0, movie, frame - 96 + 80);
}

uint64_t PanelFadeIn(uint8_t* base, uint64_t passes, uint32_t sp, uint32_t lr,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249BE8 PanelFade::In");
  StoreU32(base, kFadeWait, 1);
  // A tail call of ScreenFades::FadeIn(passes, 0, 0, 0, 0).
  const uint64_t args[6] = {passes, 0, 0, 0, 0, 0};
  return calls.call_set(kScreenFadeIn, args, 5, sp, lr);
}

uint64_t MachineScreenDestruct(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224CB18 MachineScreen::~MachineScreen");
  StoreU32(base, uint32_t(r3), kMachineScreenVtable);
  return r3;
}

// --- The panel screens' slots ---

uint64_t PanelScreenIsLoaded(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82230790 PanelScreen::IsLoaded");
  Frame(base, sp, 96);
  const uint32_t a = ui_movies::IsLoaded(base, 4);
  const uint32_t b = ui_movies::IsLoaded(base, 28);
  return (a & b) & 1;
}

uint64_t PanelScreenOpenMovies(uint8_t* base, uint32_t sp, bool type7, const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_822401D0 Type7PanelScreen::OpenMovies");
  } else {
    ITC_NATIVE_RUNNING("sub_82230758 Type6PanelScreen::OpenMovies");
  }
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t first = type7 ? 22571 : 22567;
  const uint32_t lr = type7 ? 0x822401E8 : 0x82230770;
  calls.open(4, first, frame, lr);
  return calls.open(28, first + 1, frame, lr + 12);
}

uint64_t PanelScreenDeleteMovies(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82240208 PanelScreen::DeleteMovies");
  const uint32_t frame = Frame(base, sp, 96);
  calls.remove(4, frame, 0x8224021C);
  return calls.remove(28, frame, 0x82240224);
}

uint64_t PanelScreenPost(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t function) {
  uint32_t message = 0;
  switch (function) {
    case 0x82240860:
      ITC_NATIVE_RUNNING("sub_82240860 Type7PanelScreen::Back");
      StoreU32(base, uint32_t(r3) + 9608, 9);
      message = 5;
      break;
    case 0x82247058:
      ITC_NATIVE_RUNNING("sub_82247058 Type6PanelScreen::OpenPowerGraph");
      message = 21;
      break;
    default:
      ITC_NATIVE_RUNNING("sub_822408B8 Type7PanelScreen::OpenPowerGraph");
      message = 18;
      break;
  }
  Post(base, uint32_t(r4), message);
  return r3;
}

uint64_t Type7CarWord(uint8_t* base, uint32_t offset) {
  switch (offset) {
    case 1972:
      ITC_NATIVE_RUNNING("sub_82240238 Type7PanelScreen::CarList");
      break;
    case 1976:
      ITC_NATIVE_RUNNING("sub_82240248 Type7PanelScreen::CarEntry");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82240258 Type7PanelScreen::CarSlot");
      break;
  }
  return LoadU32(base, kType7Car + offset);
}

}  // namespace type67_screens
