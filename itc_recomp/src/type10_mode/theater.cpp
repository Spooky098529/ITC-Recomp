// The replay theater's machine handlers (docs/functions/type10-mode.md):
// see type10_mode.h.

#include "base/log.h"
#include "type10_mode/internal.h"

namespace type10_mode {

namespace {

// The pages' table in Draw's frame: ListPage, AskList, AskLoad, LeavePage,
// the empty sub_82106540, WaitPage.
constexpr uint32_t kPageTable[6] = {0x82289768, 0x82289958, 0x822899E0,
                                    0x82289A70, 0x82106540, 0x8228A4B0};
// The transitions' table in Update's frame (+96 on): Intro, FadeOut,
// FadeInList, ShowCursor, MoveCursor, HideCursor, PickReplay.
constexpr uint32_t kTransitionTable[7] = {0x82289AC0, 0x82289BD0, 0x82289CC0, 0x82289D70,
                                          0x8228A658, 0x82289E08, 0x82289EB8};

// The page object to the page after (`after`) through `transition`, +16
// -1, +0 -1; +8 the page unless it is `unless`. The stores in OnDone's
// order: +12, +16, +0, +4.
void Through(uint8_t* base, uint32_t unless, uint32_t transition, uint32_t after) {
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, unless);
  Store(base, pages + 12, transition);
  Store(base, pages + 16, kNone);
  Store(base, pages + 0, kNone);
  Store(base, pages + 4, after);
}

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289408 ReplayTheater::Create");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8228943C, 0, 28, 0x1000, 4));
  Store(base, kPages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 24, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  const uint32_t list = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82289478, 0, 72, 0x1000, 4));
  Store(base, kList, list);
  if (list != 0) {
    Store(base, list + 68, 0);
    Store(base, list + 60, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x822894A4, 0, 16, 0x1000, 4);
  const uint32_t data = uint32_t(r3);
  Store(base, kData, data);
  Store(base, data + 0, 0);
  Store(base, data + 12, 0);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822891F8 ReplayTheater::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82289570 ReplayTheater::WaitForMovie");
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

uint64_t StartPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A508 ReplayTheater::StartPage");
  const uint32_t frame = Frame(base, sp, 96);
  static constexpr uint32_t kHidden[3] = {kNone, kNone, kNone};
  const uint64_t r3 = SetPrompts(base, kHidden, kHidden, frame, calls);
  Store(base, Data(base) + 0, 1);
  Store(base, Data(base) + 8, 0);
  return r3;
}

uint64_t Start(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228AA58 ReplayTheater::Start");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kSaveBuffersCreate, frame, 0x8228AA68);
  const uint64_t r3 = StartPage(base, frame, calls);
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 1);
  Store(base, pages + 16, kNone);
  Store(base, pages + 12, 0);
  Store(base, pages + 4, 1);
  Store(base, pages + 0, kNone);
  return r3;
}

uint64_t Draw(uint8_t* base, uint64_t r3, uint64_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A868 ReplayTheater::Draw");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t page = Load(base, Pages(base) + 0);
  for (uint32_t k = 0; k < 6; k++) {
    Store(base, frame + 80 + 4 * k, kPageTable[k]);
  }
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x8228A8E8, dispatcher,
                    dispatcher);
}

uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A8F8 ReplayTheater::Update");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame + 80);
  const uint32_t pages = Pages(base);
  for (uint32_t k = 0; k < 7; k++) {
    Store(base, frame + 96 + 4 * k, kTransitionTable[k]);
  }
  const uint32_t running = Load(base, pages + 12);
  if (running == kNone) {
    if (Load(base, pages + 0) == 3) {
      return movie;
    }
    return calls.Call(kRun, frame, 0x8228AA40, movie, 0);
  }
  uint64_t r3 = movie;
  uint32_t p = pages;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(base, frame + 96 + (running << 2)), frame, 0x8228A9C4, pages + 24);
    p = Pages(base);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t queued = Load(base, p + 16);
  if (queued == kNone) {
    if (int32_t(running) >= 0) {
      Store(base, p + 0, Load(base, p + 4));
      Store(base, p + 12, queued);
    } else {
      const uint32_t next = Load(base, p + 4);
      Store(base, p + 12, kNone);
      Store(base, p + 0, next);
    }
  } else {
    Store(base, p + 16, kNone);
    Store(base, p + 20, 0);
    Store(base, p + 12, queued);
  }
  Store(base, p + 24, 0);
  return r3;
}

uint64_t OnDone(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228AAB0 ReplayTheater::OnDone");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t asked = Load(base, Data(base) + 4);
  switch (asked) {
    case 0: {
      r3 = BuildList(base, frame, calls);
      const uint32_t pages = Pages(base);
      KeepPage(base, pages, 0);
      Store(base, pages + 12, 2);
      Store(base, pages + 16, 3);
      Store(base, pages + 0, kNone);
      Store(base, pages + 4, 0);
      break;
    }
    case 1: {
      Store(base, Data(base) + 8, 1);
      const uint32_t pages = Pages(base);
      KeepPage(base, pages, 3);
      Store(base, pages + 12, 1);
      Store(base, pages + 16, kNone);
      Store(base, pages + 4, 3);
      Store(base, pages + 0, kNone);
      r3 = calls.Call(kSetUpRace, frame, 0x8228AB8C, Load(base, kKeptReplay),
                      Load(base, kRaceData), Load(base, kRecordings));
      break;
    }
    case 2:
      Through(base, 1, 5, 1);
      break;
    case 3:
      Through(base, 3, 1, 3);
      break;
    default:
      break;
  }
  return r3;
}

void OnFailed(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82289208 ReplayTheater::OnFailed");
  const uint32_t data = Data(base);
  const uint32_t interrupted = Load(base, data + 12);
  const uint32_t asked = Load(base, data + 4);
  const uint32_t pages = Pages(base);
  uint32_t transition;
  if (interrupted == 0) {
    if (asked <= 1) {
      KeepPage(base, pages, 3);
      Store(base, pages + 4, 3);
      transition = 1;
    } else if (asked == 2) {
      KeepPage(base, pages, 1);
      Store(base, pages + 4, 1);
      transition = 5;
    } else if (asked == 3) {
      KeepPage(base, pages, 0);
      Store(base, pages + 0, 0);
      Store(base, pages + 12, kNone);
      Store(base, pages + 16, kNone);
      return;
    } else {
      return;
    }
  } else {
    const int32_t a = int32_t(asked);
    if (a != 1 && a != 2 && a != 3) {
      return;
    }
    KeepPage(base, pages, kNone);
    Store(base, pages + 4, kNone);
    transition = 5;
  }
  Store(base, pages + 0, kNone);
  Store(base, pages + 12, transition);
  Store(base, pages + 16, kNone);
}

uint64_t OnRetry(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228ABE0 ReplayTheater::OnRetry");
  const uint32_t frame = Frame(base, sp, 96);
  if (int32_t(Load(base, Data(base) + 4)) != 1) {
    return r3;
  }
  r3 = BuildList(base, frame, calls);
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 0);
  Store(base, pages + 16, kNone);
  Store(base, pages + 12, 3);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, kNone);
  return r3;
}

void OnBack(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82289308 ReplayTheater::OnBack");
  if (int32_t(Load(base, Data(base) + 4)) != 1) {
    return;
  }
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 0);
  Store(base, pages + 16, kNone);
  Store(base, pages + 12, 3);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, kNone);
}

void OnInterrupt(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82289358 ReplayTheater::OnInterrupt");
  Store(base, Data(base) + 12, 1);
  const uint32_t pages = Pages(base);
  if (Load(base, pages + 0) != 0) {
    return;
  }
  // The page is 0 here: +8 = 0 (its "unless -1" test passes).
  Store(base, pages + 8, 0);
  Store(base, pages + 16, kNone);
  Store(base, pages + 12, 5);
  Store(base, pages + 4, kNone);
  Store(base, pages + 0, kNone);
}

void OnResume(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822893A8 ReplayTheater::OnResume");
  Store(base, Data(base) + 12, 0);
  const uint32_t pages = Pages(base);
  KeepPage(base, pages, 1);
  Store(base, pages + 12, kNone);
  Store(base, pages + 0, 1);
  Store(base, pages + 16, kNone);
}

uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289600 ReplayTheater::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x82289634, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x82289648, kMovie);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

void Wait(uint8_t* base, uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82289668 ReplayTheater::Wait");
  Post(base, dispatcher, 4);
  Store(base, machine + kMachineState, 9);
}

uint64_t Leave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822896C0 ReplayTheater::Leave");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  calls.Call(kSaveBuffersDestroy, frame, 0x822896E0);
  const uint32_t movie = FindMovie(base, frame + 80);
  calls.Call(kFinishAll, frame, 0x82289700, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x82289708, kMovie);
  Post(base, dispatcher, 4);
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822894E8 ReplayTheater::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, machine + kMachineState, Load(base, Data(base) + 8) != 0 ? 11 : 14);
  calls.Call(kVirtualFree, frame, 0x82289530, Load(base, kPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82289544, Load(base, kList), 0, kMemRelease);
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x82289554, Load(base, kData), 0, kMemRelease);
  Store(base, kPages, 0);
  Store(base, kList, 0);
  Store(base, kData, 0);
  return r3;
}

}  // namespace type10_mode
