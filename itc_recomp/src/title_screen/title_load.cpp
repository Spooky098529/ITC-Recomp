// The title load's machine (docs/functions/title-screen.md): see
// title_screen.h.

#include "base/log.h"
#include "title_screen/internal.h"

namespace title_screen {

namespace {

// The pages' table in Draw's frame: the debug sequence's four, PostClose,
// WaitForArchive.
constexpr uint32_t kPageTable[6] = {0x82220CD8, 0x82220D60, 0x82220DE8,
                                    0x82220E70, 0x82221078, 0x822204C0};
// The transitions' table in Update's frame: the debug sequence's twelve,
// PlayLogo.
constexpr uint32_t kTransitionTable[13] = {
    0x822204F8, 0x82220580, 0x82220610, 0x82220698, 0x82220720, 0x822207B0, 0x82220840,
    0x822208D0, 0x82220960, 0x822209F0, 0x82220A80, 0x82220B10, 0x822210C8};

}  // namespace

uint64_t LoadCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220BA8 TitleLoad::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const Machine m(base, machine);
  m.set_loaded(0);
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82220BDC, 0, 68, 0x1000, 4);
  G::set_load_block(base, uint32_t(r3));
  schema::TitleLoadBlock(base, uint32_t(r3)).set_unknown_0(0);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t LoadOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822204B0 TitleLoad::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kLoadMovie, kLoadAsset);
}

uint64_t LoadWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82220C18 TitleLoad::WaitForMovie");
  const Machine m(base, uint32_t(r3));
  Frame(base, sp, 96);
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kLoadMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
  }
  if (m.loaded() != 0) {
    m.set_state(m.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t LoadStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220C88 TitleLoad::Start");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = calls.Call(kBootDataLoad, frame, 0x82220CA0, G::boot_data(base));
  G::load_block(base).set_tick_count(0);
  G::load_block(base).set_page(kNone);
  G::load_block(base).set_transition(12);
  return r3;
}

uint64_t LoadDraw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221198 TitleLoad::Draw");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t page = G::load_block(base).page();
  for (uint32_t k = 0; k < 6; k++) {
    Store(base, frame + 80 + 4 * k, kPageTable[k]);
  }
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x82221218, mode);
}

uint64_t LoadUpdate(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221228 TitleLoad::Update");
  const uint32_t frame = Frame(base, sp, 208);
  const auto block = G::load_block(base);
  const uint32_t transition = block.transition();
  for (uint32_t k = 0; k < 13; k++) {
    Store(base, frame + 80 + 4 * k, kTransitionTable[k]);
  }
  if (transition != kNone) {
    // No bounds check: the word at the table's index.
    return calls.Call(Load(base, frame + 80 + (transition << 2)), frame, 0x82221318,
                      block.transition_started_address());
  }
  // The original's r3 holds the table's eighth entry here (sign-extended).
  uint64_t r3 = uint64_t(int64_t(int32_t(kTransitionTable[7])));
  if (G::pad_blocked0(base) == 0 && G::pad_blocked1(base) == 0) {
    return r3;
  }
  block.set_page(5);
  G::load_block(base).set_transition_started(0);
  r3 = calls.Call(kQueueLength, frame, 0x82221354);
  if (uint32_t(r3) == 0) {
    Post(base, mode, 5);
  }
  return r3;
}

uint64_t LoadPlayLogo(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822210C8 TitleLoad::PlayLogo");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kLoadMovie, frame + 80);
  if (Started(base, started).started() == 0) {
    calls.CallArgs(kPlayNamed, frame, 0x82221124, {movie, 0, 0, 0, 0, 0, 0, 0});
    calls.CallArgs(kFadeIn, frame, 0x82221140, {G::fades_passes(base), 0, 0, 0, 0, 0, 0, 0});
    Started(base, started).set_started(1);
  }
  const uint64_t r3 = calls.Call(kRun, frame, 0x82221154, movie, 0);
  if (uint32_t(r3) == 0) {
    G::load_block(base).set_transition(kNone);
    G::load_block(base).set_page(5);
    Started(base, started).set_started(0);
  }
  return r3;
}

uint64_t LoadWaitForArchive(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822204C0 TitleLoad::WaitForArchive");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = calls.Call(kQueueLength, frame, 0x822204D0);
  if (uint32_t(r3) == 0) {
    G::load_block(base).set_page(4);
  }
  return r3;
}

void LoadPostClose(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82221078 TitleLoad::PostClose");
  Post(base, mode, 5);
}

uint64_t LoadUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220EF8 TitleLoad::Unload");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kLoadMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x82220F2C, movie);
  const Machine m(base, machine);
  if (m.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x82220F40, kLoadMovie);
    m.set_loaded(0);
  }
  return r3;
}

void LoadOpenTitle(uint8_t* base, uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82220F60 TitleLoad::OpenTitle");
  G::set_entered(base, 1);
  Post(base, mode, 8);
  Machine(base, machine).set_state(9);
}

uint64_t LoadLeave(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220FC8 TitleLoad::Leave");
  const uint32_t frame = Frame(base, sp, 112);
  Machine(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, kLoadMovie, frame + 80);
  uint64_t r3 = movie;
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x82221010, movie);
    r3 = calls.Call(kMoviesDelete, frame, 0x82221018, kLoadMovie);
  }
  Post(base, mode, 4);
  return r3;
}

uint64_t LoadDestroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220468 TitleLoad::Destroy");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x82220490, G::load_block(base).address, 0, kMemRelease);
  G::set_load_block(base, 0u);
  return r3;
}

}  // namespace title_screen
