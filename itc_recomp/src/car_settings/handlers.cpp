// The car settings screen's machine handlers seen in play
// (docs/functions/car-settings.md): see car_settings.h.

#include "car_settings/car_settings.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace car_settings {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemCommit = 4096;
constexpr uint32_t kReadWrite = 4;
constexpr uint32_t kMemRelease = 0x8000;

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The original's frame below `sp`, its back chain written.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  Store(base, slot + 8, 0);
  Store(base, slot + 4, 0);
  Store(base, slot + 0, id);
}

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821AA2F8 CarSettings::Create");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(
      calls.Call(kVirtualAlloc, frame, 0x821AA32C, 0, kPagesBytes, kMemCommit, kReadWrite));
  Store(base, kPages, pages);
  for (const uint32_t field : {4u, 0u, 8u, 24u}) {
    Store(base, pages + field, 0);
  }
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  const uint32_t menu = uint32_t(
      calls.Call(kVirtualAlloc, frame, 0x821AA368, 0, kMenuBytes, kMemCommit, kReadWrite));
  Store(base, kMenu, menu);
  for (const uint32_t field : {12u, 20u, 0u, 4u}) {
    Store(base, menu + field, 0);
  }
  Store(base, menu + 16, 4);
  for (const uint32_t field : {8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + field, 0);
  }
  const uint64_t r3 =
      calls.Call(kVirtualAlloc, frame, 0x821AA3C4, 0, kDataBytes, kMemCommit, kReadWrite);
  const uint32_t data = uint32_t(r3);
  Store(base, kData, data);
  Store(base, data + 748, 0);
  Store(base, data + 760, 21);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A8DD0 CarSettings::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821AA490 CarSettings::WaitForMovie");
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

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821AAF70 CarSettings::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  Store(base, frame + 80, kMovie);
  const uint32_t movie = Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  uint64_t r3 = movie;
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x821AAFB8, movie);
    r3 = calls.Call(kMoviesDelete, frame, 0x821AAFC0, kMovie);
  }
  const uint32_t type = Load(base, kModeType);
  if (type == 2 || type == 5) {
    Post(base, mode, 4);
  }
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821AA408 CarSettings::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, machine + kMachineState, Load(base, Load(base, kData) + 576) == 1 ? 11 : 6);
  calls.Call(kVirtualFree, frame, 0x821AA450, Load(base, kPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x821AA464, Load(base, kMenu), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x821AA474, Load(base, kData), 0, kMemRelease);
  Store(base, kPages, 0);
  Store(base, kMenu, 0);
  Store(base, kData, 0);
  return r3;
}

}  // namespace car_settings
