// The garage screen's machine handlers (docs/functions/garage-screen.md):
// see garage_screen.h.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace garage_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemCommit = 4096;
constexpr uint32_t kReadWrite = 4;
constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kFadeInArgument = 0x82001BEC;

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191C58 GarageScreen::Create");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82191C8C, 0, 28, kMemCommit, kReadWrite));
  Store(base, kPages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 24, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  const uint32_t list =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82191CC8, 0, 72, kMemCommit, kReadWrite));
  Store(base, kList, list);
  if (list != 0) {
    Store(base, list + 68, 0);
    Store(base, list + 60, 0);
  }
  const uint32_t list2 =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82191CF4, 0, 72, kMemCommit, kReadWrite));
  Store(base, kList2, list2);
  if (list2 != 0) {
    Store(base, list2 + 68, 0);
    Store(base, list2 + 60, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82191D20, 0, 104, kMemCommit, kReadWrite);
  const uint32_t data = uint32_t(r3);
  Store(base, kData, data);
  for (const uint32_t field : {0u, 84u, 88u, 92u, 96u}) {
    Store(base, data + field, 0);
  }
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191618 GarageScreen::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82191D70 GarageScreen::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kMovie);
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t ReadSave(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82191BB0 GarageScreen::ReadSave");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t data = Data(base);
  // The career's pick (20 bytes), the money, the weekday (signed) and the
  // day (at most 999).
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, data + 4 + 4 * i, Load(base, save + 8 + 4 * i));
  }
  const int32_t day = int32_t(Load(base, save + 6100));
  Store(base, Data(base) + 24, Load(base, save + 6116));
  Store(base, Data(base) + 32, uint32_t(day - (day / 7) * 7));
  Store(base, Data(base) + 28, uint32_t(day > 999 ? 999 : day));
  const uint32_t again = vehicle_accessors::SaveDataOf(base, 0);
  Store(base, Data(base) + 36, again);
  return again;
}

uint64_t SetHeader(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192C80 GarageScreen::SetHeader");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  calls.Call(kResetPose, frame, 0x82192CB8, movie);
  calls.Call(kSetChildNameText, frame, 0x82192CD4, movie, 15, 6, Data(base) + 4);
  calls.Call(kSetNumberText, frame, 0x82192CEC, movie, 15, 9, Load(base, Data(base) + 24));
  calls.CallArgs(kSetText, frame, 0x82192D08,
                 {movie, 15, 13, 8, Load(base, Data(base) + 28), 0, 0, 0});
  const uint32_t node = ui_movies::FindNode(base, movie, 15, 11);
  Store(base, node + 108, Load(base, Data(base) + 32));
  Store(base, Data(base) + 72, 0);
  Store(base, Data(base) + 76, 0);
  Store(base, Data(base) + 0, 1);
  return node;
}

uint64_t Start(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193DC0 GarageScreen::Start");
  const uint32_t frame = Frame(base, sp, 96);
  ReadSave(base, frame);
  SetHeader(base, frame, calls);
  const uint32_t pages = Load(base, kPages);
  const uint32_t before = Load(base, pages + 0);
  if (before != 0) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 12, 0);
  Store(base, pages + 16, 1);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, kNone);
  return calls.Call(kFadeIn, frame, 0x82193E24, Load(base, kFadeInArgument), 0, 0, 0);
}

uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821927E8 GarageScreen::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8219281C, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x82192830, kMovie);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82192850 GarageScreen::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  calls.Call(kFinishAll, frame, 0x82192890, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x82192898, kMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821915B0 GarageScreen::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x821915D8, Load(base, kPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x821915EC, Load(base, kList), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x82191600, Load(base, kData), 0, kMemRelease);
  // The second list [0x826D8428] isn't freed (nor the lists' flags blocks),
  // as the original.
  Store(base, kPages, 0);
  Store(base, kList, 0);
  Store(base, kData, 0);
  return r3;
}

}  // namespace garage_screen
