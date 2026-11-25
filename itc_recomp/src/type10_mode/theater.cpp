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
void Through(uint32_t unless, uint32_t transition, uint32_t after) {
  const PageObject pages = Pages();
  KeepPage(pages, unless);
  pages.set_step(transition);
  pages.set_queued_step(kNone);
  pages.set_page(kNone);
  pages.set_next_page(after);
}

}  // namespace

uint64_t Create(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289408 ReplayTheater::Create");
  const uint32_t frame = Frame(sp, 112);
  const Machine m(machine);
  m.set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x8228943C
  // and the VirtualAlloc(72) at 0x82289478; the globals hold their handles.
  const PageObject pages = PageObject::New();
  G::set_pages(pages);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_step_started(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  const schema::MenuList list = schema::MenuList::New();
  G::set_list(list);
  if (list.address != 0) {
    list.set_enabled(0u);
    list.set_player(0);
  }
  // The data block lives in host memory (E5 step 3): the original's
  // VirtualAlloc(16) at 0x822894A4; the global holds its handle.
  const schema::ReplayTheaterData data = schema::ReplayTheaterData::New();
  const uint64_t r3 = data.address;
  G::set_data(data);
  data.set_started(0);
  data.set_interrupted(0);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822891F8 ReplayTheater::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForMovie(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82289570 ReplayTheater::WaitForMovie");
  const Machine m(uint32_t(r3));
  Frame(sp, 96);
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(kMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
  }
  if (m.loaded() != 0) {
    m.set_state(m.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t StartPage(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A508 ReplayTheater::StartPage");
  const uint32_t frame = Frame(sp, 96);
  static constexpr uint32_t kHidden[3] = {kNone, kNone, kNone};
  const uint64_t r3 = SetPrompts(kHidden, kHidden, frame, calls);
  Data().set_started(1);
  Data().set_replay_loaded(0);
  return r3;
}

uint64_t Start(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228AA58 ReplayTheater::Start");
  const uint32_t frame = Frame(sp, 96);
  calls.Call(kSaveBuffersCreate, frame, 0x8228AA68);
  const uint64_t r3 = StartPage(frame, calls);
  const PageObject pages = Pages();
  KeepPage(pages, 1);
  pages.set_queued_step(kNone);
  pages.set_step(0);
  pages.set_next_page(1);
  pages.set_page(kNone);
  return r3;
}

uint64_t Draw(uint64_t r3, uint64_t dispatcher, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A868 ReplayTheater::Draw");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t page = Pages().page();
  for (uint32_t k = 0; k < 6; k++) {
    Store(frame + 80 + 4 * k, kPageTable[k]);
  }
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(Load(frame + 80 + (page << 2)), frame, 0x8228A8E8, dispatcher,
                    dispatcher);
}

uint64_t Update(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228A8F8 ReplayTheater::Update");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(frame + 80);
  const PageObject pages = Pages();
  for (uint32_t k = 0; k < 7; k++) {
    Store(frame + 96 + 4 * k, kTransitionTable[k]);
  }
  const uint32_t running = pages.step();
  if (running == kNone) {
    if (pages.page() == 3) {
      return movie;
    }
    return calls.Call(kRun, frame, 0x8228AA40, movie, 0);
  }
  uint64_t r3 = movie;
  PageObject p = pages;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(frame + 96 + (running << 2)), frame, 0x8228A9C4,
                    pages.step_started_address());
    p = Pages();
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t queued = p.queued_step();
  if (queued == kNone) {
    if (int32_t(running) >= 0) {
      p.set_page(p.next_page());
      p.set_step(queued);
    } else {
      const uint32_t next = p.next_page();
      p.set_step(kNone);
      p.set_page(next);
    }
  } else {
    p.set_queued_step(kNone);
    p.set_unknown_20(0);
    p.set_step(queued);
  }
  p.set_step_started(0);
  return r3;
}

uint64_t OnDone(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228AAB0 ReplayTheater::OnDone");
  const uint32_t frame = Frame(sp, 96);
  const uint32_t asked = Data().asked();
  switch (asked) {
    case 0: {
      r3 = BuildList(frame, calls);
      const PageObject pages = Pages();
      KeepPage(pages, 0);
      pages.set_step(2);
      pages.set_queued_step(3);
      pages.set_page(kNone);
      pages.set_next_page(0);
      break;
    }
    case 1: {
      Data().set_replay_loaded(1);
      const PageObject pages = Pages();
      KeepPage(pages, 3);
      pages.set_step(1);
      pages.set_queued_step(kNone);
      pages.set_next_page(3);
      pages.set_page(kNone);
      r3 = calls.Call(kSetUpRace, frame, 0x8228AB8C, G::kept_replay().address,
                      G::race_data(), G::recordings());
      break;
    }
    case 2:
      Through(1, 5, 1);
      break;
    case 3:
      Through(3, 1, 3);
      break;
    default:
      break;
  }
  return r3;
}

void OnFailed() {
  ITC_NATIVE_RUNNING("sub_82289208 ReplayTheater::OnFailed");
  const schema::ReplayTheaterData data = Data();
  const uint32_t interrupted = data.interrupted();
  const uint32_t asked = data.asked();
  const PageObject pages = Pages();
  uint32_t transition;
  if (interrupted == 0) {
    if (asked <= 1) {
      KeepPage(pages, 3);
      pages.set_next_page(3);
      transition = 1;
    } else if (asked == 2) {
      KeepPage(pages, 1);
      pages.set_next_page(1);
      transition = 5;
    } else if (asked == 3) {
      KeepPage(pages, 0);
      pages.set_page(0);
      pages.set_step(kNone);
      pages.set_queued_step(kNone);
      return;
    } else {
      return;
    }
  } else {
    const int32_t a = int32_t(asked);
    if (a != 1 && a != 2 && a != 3) {
      return;
    }
    KeepPage(pages, kNone);
    pages.set_next_page(kNone);
    transition = 5;
  }
  pages.set_page(kNone);
  pages.set_step(transition);
  pages.set_queued_step(kNone);
}

uint64_t OnRetry(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8228ABE0 ReplayTheater::OnRetry");
  const uint32_t frame = Frame(sp, 96);
  if (int32_t(Data().asked()) != 1) {
    return r3;
  }
  r3 = BuildList(frame, calls);
  const PageObject pages = Pages();
  KeepPage(pages, 0);
  pages.set_queued_step(kNone);
  pages.set_step(3);
  pages.set_next_page(0);
  pages.set_page(kNone);
  return r3;
}

void OnBack() {
  ITC_NATIVE_RUNNING("sub_82289308 ReplayTheater::OnBack");
  if (int32_t(Data().asked()) != 1) {
    return;
  }
  const PageObject pages = Pages();
  KeepPage(pages, 0);
  pages.set_queued_step(kNone);
  pages.set_step(3);
  pages.set_next_page(0);
  pages.set_page(kNone);
}

void OnInterrupt() {
  ITC_NATIVE_RUNNING("sub_82289358 ReplayTheater::OnInterrupt");
  Data().set_interrupted(1);
  const PageObject pages = Pages();
  if (pages.page() != 0) {
    return;
  }
  // The page is 0 here: +8 = 0 (its "unless -1" test passes).
  pages.set_previous_page(0);
  pages.set_queued_step(kNone);
  pages.set_step(5);
  pages.set_next_page(kNone);
  pages.set_page(kNone);
}

void OnResume() {
  ITC_NATIVE_RUNNING("sub_822893A8 ReplayTheater::OnResume");
  Data().set_interrupted(0);
  const PageObject pages = Pages();
  KeepPage(pages, 1);
  pages.set_step(kNone);
  pages.set_page(1);
  pages.set_queued_step(kNone);
}

uint64_t Unload(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82289600 ReplayTheater::Unload");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x82289634, movie);
  const Machine m(machine);
  if (m.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x82289648, kMovie);
    m.set_loaded(0);
  }
  return r3;
}

void Wait(uint32_t machine, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_82289668 ReplayTheater::Wait");
  Post(dispatcher, 4);
  Machine(machine).set_state(9);
}

uint64_t Leave(uint32_t machine, uint32_t dispatcher, uint32_t sp,
               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822896C0 ReplayTheater::Leave");
  const uint32_t frame = Frame(sp, 112);
  Machine(machine).set_state(7);
  calls.Call(kSaveBuffersDestroy, frame, 0x822896E0);
  const uint32_t movie = FindMovie(frame + 80);
  calls.Call(kFinishAll, frame, 0x82289700, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x82289708, kMovie);
  Post(dispatcher, 4);
  return r3;
}

uint64_t Destroy(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822894E8 ReplayTheater::Destroy");
  const uint32_t frame = Frame(sp, 128);
  Machine(machine).set_state(Data().replay_loaded() != 0 ? 11 : 14);
  calls.Call(kVirtualFree, frame, 0x82289530, Pages().address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82289544, List().address, 0, kMemRelease);
  // The host data block (Create): the original's VirtualFree at
  // 0x82289554, which returned 1.
  Data().Delete();
  const uint64_t r3 = 1;
  G::set_pages(0u);
  G::set_list(0u);
  G::set_data(0u);
  return r3;
}

}  // namespace type10_mode
