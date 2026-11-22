// The panel screens' group pair and panel groups, and the panel screens'
// vtable slots without guest calls of their own beyond UiMovies::Open and
// Delete (docs/functions/type67-screens.md): see type67_screens.h.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "schema/type67_screens.h"
#include "ui_movies/game.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using G = schema::Type67ScreensGlobals;
using guest::StoreU32;
using Group = schema::PanelGroup;
using MS = schema::MachineScreen;
using PS = schema::PanelScreen;
using Pair = schema::GroupPair;
using Single = schema::SingleGroup;
using T67PS = schema::Type67PanelScreen;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The car list's plain C++ guest calls through these calls (r3 to r6 set,
// as car_list::Calls::Call sets them).
uint64_t ThroughCalls(void* user, uint32_t function, const uint64_t args[8], const double*,
                      uint32_t r1, uint32_t lr) {
  const Calls& calls = *static_cast<const Calls*>(user);
  const uint64_t set[6] = {args[0], args[1], args[2], args[3], 0, 0};
  return calls.call_set(function, set, 4, r1, lr);
}

car_list::Calls CarListCalls(const Calls& calls) {
  return {const_cast<Calls*>(&calls), &ThroughCalls};
}

}  // namespace

// --- The panel groups and the group pair ---

uint64_t PanelGroupMovie(uint8_t* base, uint64_t group, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82258FC8 PanelGroup::Movie");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, frame + 80, Group(base, uint32_t(group)).movie());
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

uint64_t PanelGroupRun(uint8_t* base, uint64_t group, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259008 PanelGroup::Run");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, frame + 80, Group(base, uint32_t(group)).movie());
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  return calls.call(kRun, frame, 0x8225903C, movie, 0);
}

uint64_t GroupPairSelectA(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82249C30 GroupPair::SelectA");
  const uint32_t pair = uint32_t(r3);
  Pair(base, pair).set_current(Pair(base, pair).group_a().address);
  return r3;
}

uint64_t GroupPairReset(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249C40 GroupPair::Reset");
  const uint32_t pair = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t group = Pair(base, pair).group_a().address;
  Pair(base, pair).set_current(group);
  const uint32_t movie = uint32_t(PanelGroupMovie(base, group, frame));
  // r4 as PanelGroup::Movie leaves it (its frame's +80).
  return calls.call(kResetPose, frame, 0x82249C60, movie, frame - 96 + 80);
}

uint64_t GroupPairRun(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249BE0 GroupPair::Run");
  return PanelGroupRun(base, Pair(base, uint32_t(r3)).current().address, sp, calls);
}

uint64_t GroupPairWaitForFade(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82249BA8 GroupPair::WaitForFade");
  if (G::fade_wait(base) == 0) {
    return r3;
  }
  if ((G::fade_out_count(base) | G::fade_in_count(base)) != 0) {
    return r3;
  }
  G::set_fade_wait(base, 0);
  return r3;
}

uint64_t GroupPairFinishAll(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249C70 GroupPair::FinishAll");
  const uint32_t pair = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t movie_a =
      uint32_t(PanelGroupMovie(base, Pair(base, pair).group_a().address, frame));
  calls.call(kFinishAll, frame, 0x82249C90, movie_a, frame - 96 + 80);
  const uint32_t movie_b =
      uint32_t(PanelGroupMovie(base, Pair(base, pair).group_b().address, frame));
  return calls.call(kFinishAll, frame, 0x82249C9C, movie_b, frame - 96 + 80);
}

uint64_t GroupPairDestruct(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82249C20 GroupPair::~GroupPair");
  Pair(base, uint32_t(r3)).set_vtable(kGroupPairBaseVtable);
  return r3;
}

uint64_t SingleGroupConstruct(uint8_t* base, uint64_t r3, uint32_t group) {
  ITC_NATIVE_RUNNING("sub_82249CB0 SingleGroup::SingleGroup");
  Single(base, uint32_t(r3)).set_group(group);
  Single(base, uint32_t(r3)).set_vtable(kSingleGroupVtable);
  return r3;
}

uint64_t SingleGroupReset(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249CC8 SingleGroup::Reset");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie =
      uint32_t(PanelGroupMovie(base, Single(base, uint32_t(r3)).group().address, frame));
  // r4 as PanelGroup::Movie leaves it (its frame's +80).
  return calls.call(kResetPose, frame, 0x82249CE0, movie, frame - 96 + 80);
}

uint64_t PanelFadeIn(uint8_t* base, uint64_t passes, uint32_t sp, uint32_t lr,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249BE8 PanelFade::In");
  G::set_fade_wait(base, 1);
  // A tail call of ScreenFades::FadeIn(passes, 0, 0, 0, 0).
  const uint64_t args[6] = {passes, 0, 0, 0, 0, 0};
  return calls.call_set(kScreenFadeIn, args, 5, sp, lr);
}

uint64_t MachineScreenDestruct(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8224CB18 MachineScreen::~MachineScreen");
  MS(base, uint32_t(r3)).set_vtable(kMachineScreenVtable);
  return r3;
}

uint64_t PanelGroupDestruct(uint8_t* base, uint64_t group) {
  ITC_NATIVE_RUNNING("sub_82231CC0 PanelGroup::~PanelGroup");
  Group(base, uint32_t(group)).set_vtable(kPanelGroupVtable);
  return car_list::GroupBaseDestruct(base, group);
}

uint64_t PanelGroupDeletingDestruct(uint8_t* base, uint64_t group, uint64_t flags, uint32_t sp,
                                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82240348 PanelGroup::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  Group(base, uint32_t(group)).set_vtable(kPanelGroupVtable);
  car_list::GroupBaseDestruct(base, group);
  if (flags & 1) {
    const uint64_t args[6] = {group, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x82240384);
  }
  return group;
}

uint64_t PanelScreenDestruct(uint8_t* base, uint64_t screen, bool type7, uint32_t sp,
                             const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82240168 Type7PanelScreen::~Type7PanelScreen");
  } else {
    ITC_NATIVE_RUNNING("sub_82230668 Type6PanelScreen::~Type6PanelScreen");
  }
  const uint32_t frame = Frame(base, sp, 96);
  T67PS(base, uint32_t(screen))
      .set_vtable(type7 ? kType7PanelScreenVtable : kType6PanelScreenVtable);
  GroupPairDestruct(base, screen + 11728);
  car_list::SetDestruct(base, screen + 9752, frame, CarListCalls(calls));
  PanelGroupDestruct(base, screen + 9708);
  T67PS(base, uint32_t(screen)).set_group_a_vtable(kPanelGroupVtable);
  car_list::GroupBaseDestruct(base, screen + 9664);
  return MachineScreenDestruct(base, screen);
}

uint64_t Type6PanelScreenDeletingDestruct(uint8_t* base, uint64_t screen, uint64_t flags,
                                          uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230B00 Type6PanelScreen::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  PanelScreenDestruct(base, screen, false, frame, calls);
  if (flags & 1) {
    const uint64_t args[6] = {screen, flags, 0, 0, 0, 0};
    calls.call_set(car_list::kOperatorDelete, args, 2, frame, 0x82230B30);
  }
  return screen;
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
      PS(base, uint32_t(r3)).set_state(9);
      message = 5;
      break;
    case 0x8223E708:
      ITC_NATIVE_RUNNING("sub_8223E708 Type6PanelScreen::Back");
      PS(base, uint32_t(r3)).set_state(9);
      message = 6;
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
