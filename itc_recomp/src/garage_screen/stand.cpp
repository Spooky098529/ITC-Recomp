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
uint64_t Toggle(uint8_t* base, uint32_t field, uint32_t off, uint32_t on, uint32_t sp,
                uint32_t lr, const Calls& calls) {
  const uint32_t data = Data(base);
  if (Load(base, data + field) != 0) {
    Store(base, data + field, 0);
    return StageCommand(base, off, sp, lr, calls);
  }
  Store(base, data + field, 1);
  return StageCommand(base, on, sp, lr, calls);
}

// The turn stopped (the data's +88 = 0, command 1), then `command`.
uint64_t StopThen(uint8_t* base, uint32_t command, uint32_t sp, uint32_t lr1, uint32_t lr2,
                  const Calls& calls) {
  const uint32_t frame = Frame(base, sp, 96);
  Store(base, Data(base) + 88, 0);
  StageCommand(base, 1, frame, lr1, calls);
  return StageCommand(base, command, frame, lr2, calls);
}

}  // namespace

uint64_t CopyGarageCar(uint8_t* base, uint64_t out, uint32_t garage, uint32_t index,
                       uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192770 GarageScreen::CopyGarageCar");
  const uint32_t frame = Frame(base, sp, 96);
  uint32_t link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204))));
  for (uint32_t n = index; n != 0; n--) {
    link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(base, link))));
  }
  const int32_t slot = int16_t(guest::LoadU16(base, link + 2));
  const uint32_t from = garage + 1968 * uint32_t(slot) + 212;
  if (!native::Buffers_Memcpy(base, uint32_t(out), from, 1968, 0x821927CC)) {
    calls.Call(kMemcpy, frame, 0x821927CC, out, from, 1968);
  }
  return out;
}

uint64_t ShowBusy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193C58 GarageScreen::ShowBusy");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  Play(movie, 122, frame, 0x82193CA0, calls);
  Play(movie, 120, frame, 0x82193CBC, calls);
  return calls.Call(kLoopNamed, frame, 0x82193CC8, movie, 120);
}

uint64_t HideBusy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193CE0 GarageScreen::HideBusy");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  calls.Call(kFinishNamed, frame, 0x82193D18, movie, 120);
  return Play(movie, 123, frame, 0x82193D34, calls);
}

uint64_t HideBusyNow(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193D48 GarageScreen::HideBusyNow");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  calls.Call(kFinishNamed, frame, 0x82193D80, movie, 120);
  Play(movie, 123, frame, 0x82193D9C, calls);
  return calls.Call(kFinishNamed, frame, 0x82193DA8, movie, 123);
}

uint64_t ResetStand(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191B20 GarageScreen::ResetStand");
  const uint32_t frame = Frame(base, sp, 112);
  StageCommand(base, 1, frame, 0x82191B40, calls);
  Store(base, Data(base) + 88, 0);
  uint64_t r3 = StageCommand(base, 15, frame, 0x82191B60, calls);
  // The doors closed where open (exactly 1).
  if (int32_t(Load(base, Data(base) + 92)) == 1) {
    Store(base, Data(base) + 92, 0);
    r3 = StageCommand(base, 3, frame, 0x82191B84, calls);
  }
  if (int32_t(Load(base, Data(base) + 96)) == 1) {
    Store(base, Data(base) + 96, 0);
    r3 = StageCommand(base, 5, frame, 0x82191BA8, calls);
  }
  return r3;
}

uint64_t SetUpView(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194AC8 GarageScreen::SetUpView");
  const uint32_t frame = Frame(base, sp, 96);
  StageCommand(base, 58, frame, 0x82194AEC, calls);
  Store(base, Load(base, Holder(base)) + 6052, 0);
  return SetHelpLine(base, 553, frame, calls);
}

uint64_t ToggleTurn(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821918D0 GarageScreen::ToggleTurn");
  return Toggle(base, 88, 1, 0, sp, lr, calls);
}

uint64_t ToggleDoors(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191920 GarageScreen::ToggleDoors");
  return Toggle(base, 92, 3, 2, sp, lr, calls);
}

uint64_t ToggleDoors2(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191970 GarageScreen::ToggleDoors2");
  return Toggle(base, 96, 5, 4, sp, lr, calls);
}

uint64_t TurnLeft(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821919C0 GarageScreen::TurnLeft");
  return StopThen(base, 9, sp, 0x821919F4, 0x82191A04, calls);
}

uint64_t TurnRight(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191A18 GarageScreen::TurnRight");
  return StopThen(base, 10, sp, 0x82191A4C, 0x82191A5C, calls);
}

}  // namespace garage_screen
