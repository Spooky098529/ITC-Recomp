// The garage screen's stand helpers never seen in play
// (docs/functions/garage-screen.md): see garage_screen.h.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"
#include "native/buffers.h"

namespace garage_screen {

namespace {

constexpr uint32_t kMemcpy = 0x825A7410;

// The busy marker's animations (movie 5): `named` played (r5 to r8 0).
uint64_t Play(uint32_t movie, uint64_t named, uint32_t frame, uint32_t lr, const Calls& calls) {
  return calls.CallArgs(kPlayNamed, frame, lr, {movie, named, 0, 0, 0, 0, 0, 0});
}

// A toggle's word in the data: set, it goes 0 and command `off` runs; else
// it goes 1 and `on` does (tail calls: from the caller's `sp` and `lr`).
// `field` is the word's address getter (turning, doors or doors2).
uint64_t Toggle(uint32_t (ScreenData::*get)() const,
                void (ScreenData::*set)(uint32_t) const, uint32_t off, uint32_t on, uint32_t sp,
                uint32_t lr, const Calls& calls) {
  const ScreenData data = Data();
  if ((data.*get)() != 0) {
    (data.*set)(0);
    return StageCommand(off, sp, lr, calls);
  }
  (data.*set)(1);
  return StageCommand(on, sp, lr, calls);
}

// The turn stopped (the data's +88 = 0, command 1), then `command`.
uint64_t StopThen(uint32_t command, uint32_t sp, uint32_t lr1, uint32_t lr2,
                  const Calls& calls) {
  const uint32_t frame = Frame(sp, 96);
  Data().set_turning(0);
  StageCommand(1, frame, lr1, calls);
  return StageCommand(command, frame, lr2, calls);
}

}  // namespace

uint64_t CopyGarageCar(uint64_t out, uint32_t garage, uint32_t index,
                       uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192770 GarageScreen::CopyGarageCar");
  const uint32_t frame = Frame(sp, 96);
  const schema::CareerGarageList list(garage);
  uint32_t link = garage + 4 * uint32_t(int32_t(int16_t(list.head())));
  for (uint32_t n = index; n != 0; n--) {
    link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(link))));
  }
  const int32_t slot = int16_t(guest::LoadU16(link + 2));
  const uint32_t from = list.cars_address() + 1968 * uint32_t(slot);
  if (!native::Buffers_Memcpy(uint32_t(out), from, 1968, 0x821927CC)) {
    calls.Call(kMemcpy, frame, 0x821927CC, out, from, 1968);
  }
  return out;
}

uint64_t ShowBusy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193C58 GarageScreen::ShowBusy");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kMovie, frame + 80);
  Play(movie, 122, frame, 0x82193CA0, calls);
  Play(movie, 120, frame, 0x82193CBC, calls);
  return calls.Call(kLoopNamed, frame, 0x82193CC8, movie, 120);
}

uint64_t HideBusy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193CE0 GarageScreen::HideBusy");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kMovie, frame + 80);
  calls.Call(kFinishNamed, frame, 0x82193D18, movie, 120);
  return Play(movie, 123, frame, 0x82193D34, calls);
}

uint64_t HideBusyNow(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193D48 GarageScreen::HideBusyNow");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kMovie, frame + 80);
  calls.Call(kFinishNamed, frame, 0x82193D80, movie, 120);
  Play(movie, 123, frame, 0x82193D9C, calls);
  return calls.Call(kFinishNamed, frame, 0x82193DA8, movie, 123);
}

uint64_t ResetStand(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191B20 GarageScreen::ResetStand");
  const uint32_t frame = Frame(sp, 112);
  StageCommand(1, frame, 0x82191B40, calls);
  Data().set_turning(0);
  uint64_t r3 = StageCommand(15, frame, 0x82191B60, calls);
  // The doors closed where open (exactly 1).
  if (int32_t(Data().doors()) == 1) {
    Data().set_doors(0);
    r3 = StageCommand(3, frame, 0x82191B84, calls);
  }
  if (int32_t(Data().doors2()) == 1) {
    Data().set_doors2(0);
    r3 = StageCommand(5, frame, 0x82191BA8, calls);
  }
  return r3;
}

uint64_t SetUpView(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194AC8 GarageScreen::SetUpView");
  const uint32_t frame = Frame(sp, 96);
  StageCommand(58, frame, 0x82194AEC, calls);
  Stage().set_reflection(0);
  return SetHelpLine(553, frame, calls);
}

uint64_t ToggleTurn(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821918D0 GarageScreen::ToggleTurn");
  return Toggle(&ScreenData::turning, &ScreenData::set_turning, 1, 0, sp, lr, calls);
}

uint64_t ToggleDoors(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191920 GarageScreen::ToggleDoors");
  return Toggle(&ScreenData::doors, &ScreenData::set_doors, 3, 2, sp, lr, calls);
}

uint64_t ToggleDoors2(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191970 GarageScreen::ToggleDoors2");
  return Toggle(&ScreenData::doors2, &ScreenData::set_doors2, 5, 4, sp, lr, calls);
}

uint64_t TurnLeft(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821919C0 GarageScreen::TurnLeft");
  return StopThen(9, sp, 0x821919F4, 0x82191A04, calls);
}

uint64_t TurnRight(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191A18 GarageScreen::TurnRight");
  return StopThen(10, sp, 0x82191A4C, 0x82191A5C, calls);
}

}  // namespace garage_screen
