// The race menu's machine handlers (docs/functions/race-menu.md): see
// race_menu.h.

#include "race_menu/race_menu.h"

#include "base/log.h"
#include "race_menu/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_menu {

namespace {

constexpr uint32_t kMemCommit = 4096;
constexpr uint32_t kReadWrite = 4;
constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kFadeOutArgument = 0x82001BF0;
// Player 0's save's answer to the question.
constexpr uint32_t kSaveAnswer = 0x1F870;

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82188E20 RaceMenu::Create");
  const uint32_t frame = Frame(base, sp, 144);
  Store(base, machine + kPage, 0);
  Store(base, kPages, 0);
  Store(base, kMenu, 0);
  Store(base, machine + kMachineLoaded, 0);
  Store(base, machine + 9648, 0);
  Store(base, machine + 9652, 1);
  Store(base, machine + 9716, 0);
  Store(base, machine + 9724, 0);
  Store(base, machine + 9728, 0);
  guest::StoreU8(base, machine + 9776, 0);
  Store(base, machine + 9780, 1);
  Store(base, machine + 9784, 1);
  Store(base, machine + 9788, 0);
  Store(base, machine + 9792, 0);
  Store(base, machine + 9800, 0);
  Store(base, machine + 9804, 0);
  // The page history (11 words) and the debug list (15 words of 15).
  for (uint32_t i = 0; i < 11; i++) {
    Store(base, machine + 9732 + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 15; i++) {
    Store(base, machine + 9656 + 4 * i, 15);
  }
  Store(base, machine + 9720, 0);
  Store(base, machine + 9656, 7);
  uint32_t count = Load(base, machine + 9720) + 1;
  Store(base, machine + 9720, count);
  Store(base, machine + 4 * (count + 2414), 8);
  count = Load(base, machine + 9720) + 1;
  Store(base, machine + 9720, count);
  const uint32_t pages =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82188F08, 0, 28, kMemCommit, kReadWrite));
  Store(base, kPages, pages);
  const uint32_t pages2 =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82188F20, 0, 28, kMemCommit, kReadWrite));
  Store(base, kPages2, pages2);
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82188F3C, 0, 68, kMemCommit, kReadWrite);
  Store(base, kMenu, uint32_t(r3));
  const uint32_t p = Load(base, kPages);
  Store(base, p + 24, 0);
  Store(base, p + 20, kNone);
  Store(base, p + 4, 3);
  Store(base, p + 0, 3);
  Store(base, p + 8, 3);
  Store(base, p + 12, kNone);
  const uint32_t p2 = Load(base, kPages2);
  for (const uint32_t field : {4u, 0u, 8u, 20u, 12u}) {
    Store(base, p2 + field, kNone);
  }
  Store(base, p2 + 24, 0);
  const uint32_t menu = Load(base, kMenu);
  for (const uint32_t field : {12u, 16u, 20u, 0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + field, 0);
  }
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821889E0 RaceMenu::OpenMovie");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = calls.Call(kMoviesOpen, frame, 0x82188A00, kMovie, kAsset);
  Store(base, machine + kMachineLoaded, 0);
  return r3;
}

uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821890D8 RaceMenu::WaitForMovie");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 112);
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kMovie);
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    const uint32_t reenter = Load(base, machine + kMachineReenter);
    Store(base, machine + 9780, 1);
    Store(base, machine + kMachineState, reenter != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t OnAnswer(uint8_t* base, uint32_t machine, bool yes, uint32_t sp) {
  if (yes) {
    ITC_NATIVE_RUNNING("sub_821895F0 RaceMenu::OnYes");
  } else {
    ITC_NATIVE_RUNNING("sub_82189648 RaceMenu::OnNo");
  }
  Frame(base, sp, 112);
  Store(base, machine + 9788, 1);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  Store(base, save + kSaveAnswer, yes ? 1 : 0);
  Store(base, machine + 10108, yes ? 0 : 1);
  return save;
}

void OnBack(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82188938 RaceMenu::OnBack");
  const uint32_t before = Load(base, machine + 9644);
  Store(base, machine + 9796, 1);
  Store(base, machine + 10108, 0);
  uint32_t pages;
  if (before == 5) {
    const uint32_t choice = Load(base, machine + 10100);
    Store(base, machine + kPage, before);
    Store(base, machine + 9780, 1);
    Store(base, machine + 10104, choice);
    pages = Load(base, kPages);
    const uint32_t page = Load(base, pages + 0);
    if (page != 5) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 16, before);
    Store(base, pages + 0, kNone);
  } else if (before == 4) {
    Store(base, machine + 10100, 0);
    Store(base, machine + 10104, 0);
    Store(base, machine + kPage, before);
    Store(base, machine + 9780, 1);
    pages = Load(base, kPages);
    const uint32_t page = Load(base, pages + 0);
    if (page != 4) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 16, 3);
    Store(base, pages + 0, kNone);
  } else {
    return;
  }
  Store(base, pages + 4, before);
  Store(base, pages + 12, 12);
}

uint64_t OnDialogClosed(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821896A0 RaceMenu::OnDialogClosed");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t pair = FindPair(base, kMovie, frame + 80);
  uint64_t r3 = pair;
  const uint32_t movie = Load(base, pair);
  if (Load(base, machine + 9788) != 0) {
    calls.Call(kFadeOut, frame, 0x821896F4, Load(base, kFadeOutArgument), 0, 0, 0);
    r3 = calls.Call(kRun, frame, 0x82189700, movie, 0);
    Store(base, machine + kPage, 10);
    const uint32_t pages = Load(base, kPages);
    const uint32_t page = Load(base, pages + 0);
    if (page != 10) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 0, 10);
    Store(base, pages + 12, kNone);
    Store(base, pages + 16, kNone);
  }
  Store(base, machine + 9784, 1);
  return r3;
}

uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218AF08 RaceMenu::Unload");
  const uint32_t frame = Frame(base, sp, 128);
  // The movies to delete (1 and 16) beside the key.
  Store(base, frame + 88, 1);
  Store(base, frame + 92, 16);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8218AF44, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    for (uint32_t k = 0; k < 2; k++) {
      r3 = calls.Call(kMoviesDelete, frame, 0x8218AF60, Load(base, frame + 88 + 4 * k));
    }
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

void LeaveToHub(uint8_t* base, uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8218AF80 RaceMenu::LeaveToHub");
  Post(base, mode, Load(base, machine + 9808));
  Store(base, machine + kMachineState, 9);
}

uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82188FE0 RaceMenu::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x82189014, Load(base, kPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82189028, Load(base, kPages2), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8218903C, Load(base, kMenu), 0, kMemRelease);
  uint32_t state;
  if (Load(base, machine + 10136) == 2) {
    // The boot movies: movie 1, then the mode after them.
    for (uint32_t k = 0; k < 5; k++) {
      Store(base, kBootList + 4 * k, 11);
    }
    Store(base, kBootList + 20, 13);
    Store(base, kBootList + 28, 0);
    Store(base, kBootList + 24, 1);
    Store(base, kBootList + 0, 1);
    Store(base, kBootList + 20, Load(base, machine + 9792) == 1 ? 1 : 5);
    state = 20;
  } else {
    state = Load(base, machine + 9792) == 1 ? 11 : 12;
  }
  Store(base, machine + kMachineState, state);
  return r3;
}

void Back(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82188A20 RaceMenu::Back");
  const uint8_t depth = uint8_t(guest::LoadU8(base, machine + 9776) + 255);
  guest::StoreU8(base, machine + 9776, depth);
  const uint32_t page = Load(base, machine + 9732 + 4 * uint32_t(depth));
  Store(base, machine + kPage, page);
  const uint32_t pages = Load(base, kPages);
  Store(base, pages + 4, page);
  Store(base, pages + 0, page);
  Store(base, pages + 8, page);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  Store(base, pages + 24, 0);
  const uint32_t pages2 = Load(base, kPages2);
  for (const uint32_t field : {4u, 0u, 8u, 20u, 12u}) {
    Store(base, pages2 + field, kNone);
  }
  Store(base, pages2 + 24, 0);
  Store(base, machine + 9780, 1);
}

void SetEventRange(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82189158 RaceMenu::SetEventRange");
  const uint32_t bits = Load(base, machine + 10148);
  uint32_t last, variants;
  if ((bits & 0x20) != 0) {
    last = 14;
    variants = 9;
    Store(base, machine + 9840, 0);
  } else if ((bits & 0x4) != 0 || (bits & 0x8) != 0) {
    last = 12;
    variants = 8;
    Store(base, machine + 9840, 0);
  } else {
    Store(base, machine + 9840, 0);
    if ((bits & 0x10) != 0) {
      last = 7;
      variants = 5;
    } else {
      last = 5;
      variants = 4;
    }
  }
  Store(base, machine + 9848, variants);
  Store(base, machine + 9844, last);
}

}  // namespace race_menu
