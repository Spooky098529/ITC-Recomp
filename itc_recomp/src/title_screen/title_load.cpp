// The title load's machine (docs/functions/title-screen.md): see
// title_screen.h.

#include "base/log.h"
#include "title_screen/internal.h"

namespace title_screen {

namespace {

// The pages' table the original built in Draw's frame (a C++ array since E5
// step 6): the debug sequence's four, PostClose, WaitForArchive.
constexpr uint32_t kPageTable[6] = {0x82220CD8, 0x82220D60, 0x82220DE8,
                                    0x82220E70, 0x82221078, 0x822204C0};
// The transitions' table the original built in Update's frame (a C++ array
// since E5 step 6): the debug sequence's twelve, PlayLogo.
constexpr uint32_t kTransitionTable[13] = {
    0x822204F8, 0x82220580, 0x82220610, 0x82220698, 0x82220720, 0x822207B0, 0x82220840,
    0x822208D0, 0x82220960, 0x822209F0, 0x82220A80, 0x82220B10, 0x822210C8};

}  // namespace

uint64_t LoadCreate(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220BA8 TitleLoad::Create");
  // No guest frame (E5 step 6): nothing here calls guest code.
  (void)sp;
  const Machine m(machine);
  m.set_loaded(0);
  // The block lives in host memory (E5 step 3): the original's
  // VirtualAlloc(68) at 0x82220BDC (zeroed, as New's); the global holds its
  // handle, returned as the original returned the address.
  (void)calls;
  const schema::TitleLoadBlock block = schema::TitleLoadBlock::New();
  const uint64_t r3 = block.address;
  G::set_load_block(block);
  block.set_unknown_0(0);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t LoadOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822204B0 TitleLoad::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kLoadMovie, kLoadAsset);
}

uint64_t LoadWaitForMovie(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82220C18 TitleLoad::WaitForMovie");
  const Machine m{uint32_t(r3)};
  (void)sp;  // no guest frame (E5 step 6): nothing here calls guest code
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(kLoadMovie);
    if (uint32_t(r3) != 0) {
      m.set_loaded(1);
    }
  }
  if (m.loaded() != 0) {
    m.set_state(m.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t LoadStart(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220C88 TitleLoad::Start");
  const uint32_t frame = Frame(sp, 96);
  const uint64_t r3 = calls.Call(kBootDataLoad, frame, 0x82220CA0, G::boot_data());
  G::load_block().set_tick_count(0);
  G::load_block().set_page(kNone);
  G::load_block().set_transition(12);
  return r3;
}

uint64_t LoadDraw(uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221198 TitleLoad::Draw");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t page = G::load_block().page();
  if (page == kNone) {
    return r3;
  }
  // No bounds check in the original: past the table, the frame's word at
  // the index, as it read it.
  const uint32_t function = page < 6 ? kPageTable[page] : Load(frame + 80 + (page << 2));
  return calls.Call(function, frame, 0x82221218, mode);
}

uint64_t LoadUpdate(uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82221228 TitleLoad::Update");
  const uint32_t frame = Frame(sp, 208);
  const auto block = G::load_block();
  const uint32_t transition = block.transition();
  if (transition != kNone) {
    // No bounds check in the original: past the table, the frame's word at
    // the index, as it read it.
    // r3: the started word's place, the handle + 16 (the block is in host
    // memory, E5 step 3); the transitions (PlayLogo, the debug steps, only
    // reached from this table) read the word through the global's view.
    const uint32_t function = transition < 13 ? kTransitionTable[transition]
                                              : Load(frame + 80 + (transition << 2));
    return calls.Call(function, frame, 0x82221318, block.address + 16);
  }
  // The original's r3 holds the table's eighth entry here (sign-extended).
  uint64_t r3 = uint64_t(int64_t(int32_t(kTransitionTable[7])));
  if (G::pad_blocked0() == 0 && G::pad_blocked1() == 0) {
    return r3;
  }
  block.set_page(5);
  G::load_block().set_transition_started(0);
  r3 = calls.Call(kQueueLength, frame, 0x82221354);
  if (uint32_t(r3) == 0) {
    Post(mode, 5);
  }
  return r3;
}

uint64_t LoadPlayLogo(uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822210C8 TitleLoad::PlayLogo");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kLoadMovie, frame + 80);
  // `started` is the load block's +16 (Update's only caller): the block is
  // in host memory (E5 step 3), so the word is read through its view.
  (void)started;
  if (G::load_block().transition_started() == 0) {
    calls.CallArgs(kPlayNamed, frame, 0x82221124, {movie, 0, 0, 0, 0, 0, 0, 0});
    calls.CallArgs(kFadeIn, frame, 0x82221140, {G::fades_passes(), 0, 0, 0, 0, 0, 0, 0});
    G::load_block().set_transition_started(1);
  }
  const uint64_t r3 = calls.Call(kRun, frame, 0x82221154, movie, 0);
  if (uint32_t(r3) == 0) {
    G::load_block().set_transition(kNone);
    G::load_block().set_page(5);
    G::load_block().set_transition_started(0);
  }
  return r3;
}

uint64_t LoadWaitForArchive(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822204C0 TitleLoad::WaitForArchive");
  const uint32_t frame = Frame(sp, 96);
  const uint64_t r3 = calls.Call(kQueueLength, frame, 0x822204D0);
  if (uint32_t(r3) == 0) {
    G::load_block().set_page(4);
  }
  return r3;
}

void LoadPostClose(uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82221078 TitleLoad::PostClose");
  Post(mode, 5);
}

uint64_t LoadUnload(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220EF8 TitleLoad::Unload");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kLoadMovie, frame + 80);
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x82220F2C, movie);
  const Machine m(machine);
  if (m.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x82220F40, kLoadMovie);
    m.set_loaded(0);
  }
  return r3;
}

void LoadOpenTitle(uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82220F60 TitleLoad::OpenTitle");
  G::set_entered(1);
  Post(mode, 8);
  Machine(machine).set_state(9);
}

uint64_t LoadLeave(uint32_t machine, uint32_t mode, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220FC8 TitleLoad::Leave");
  const uint32_t frame = Frame(sp, 112);
  Machine(machine).set_state(7);
  const uint32_t movie = FindMovie(kLoadMovie, frame + 80);
  uint64_t r3 = movie;
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x82221010, movie);
    r3 = calls.Call(kMoviesDelete, frame, 0x82221018, kLoadMovie);
  }
  Post(mode, 4);
  return r3;
}

uint64_t LoadDestroy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82220468 TitleLoad::Destroy");
  (void)sp;  // no guest frame (E5 step 6): nothing here calls guest code
  // The host block (Create): the original's VirtualFree at 0x82220490,
  // which returned 1.
  (void)calls;
  G::load_block().Delete();
  const uint64_t r3 = 1;
  G::set_load_block(0u);
  return r3;
}

}  // namespace title_screen
