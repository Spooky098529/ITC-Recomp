// The list screens' list part (ListPart over ListPartBase) and its inner
// part (ListPartInner over ListPartInnerBase), the two-item prompt the list
// screens open before setting the race (docs/functions/type67-screens.md):
// see type67_screens.h. UiMovies::Find, UiMovie::PlayNamed, LoopLast,
// MenuList::Selection, SetSelection and SaveData::Of through their plain
// C++; MenuSound::Play's plain C++ with its slot call through Calls;
// UiMovie::ResetPose, Run, FinishAll, FinishNamed, MenuList::Update,
// Accepted and Cancelled guest calls to their overrides; UiMovie::SetText
// through Calls::set_text; MenuList::Init and Release by their plain C++
// where they free natively, else guest calls.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
#include "save_data/save_data.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kListUpdate = 0x8220C1E0;
constexpr uint32_t kListAccepted = 0x8220BD50;
constexpr uint32_t kListCancelled = 0x8220BDE0;
constexpr uint32_t kListInit = 0x8220BAC0;
// Player 0's options' +20 in the save data.
constexpr uint32_t kOptionsWord = 0x1F870;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// The movie of the id at `id_word` (the frame word the original stores it
// in): UiMovies::Find.
uint32_t MovieAt(uint8_t* base, uint32_t id_word, uint32_t id) {
  StoreU32(base, id_word, id);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, id_word));
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  return static_cast<const Calls*>(user)->call_set(function, args, set, r1, lr);
}

uint64_t PlaySound(uint8_t* base, uint32_t cue, uint32_t frame, const Calls& calls) {
  return menu_sounds::PlayMenuSound(base, cue, frame,
                                    menu_sounds::Calls{const_cast<Calls*>(&calls), &SoundThunk});
}

// A call through a function pointer (a vtable slot or a table entry) to
// `target` with r3 and, where the original sets them, r4 and r5, from the
// frame at `frame` returning to `lr`: the plain C++ where the target is one
// of ours, else a guest call. Its r3.
uint64_t CallTarget(uint8_t* base, uint32_t target, uint64_t r3, std::optional<uint64_t> r4,
                    std::optional<uint64_t> r5, uint32_t frame, uint32_t lr, const Calls& calls) {
  if (SlotIsNative(target, r4.has_value())) {
    if (r4) {
      calls.set_r4(*r4);
    }
    return *SlotNative(base, target, r3, r4, frame, lr, calls);
  }
  const uint64_t args[6] = {r3, r4.value_or(0), r5.value_or(0), 0, 0, 0};
  return calls.call_set(target, args, r5 ? 3 : (r4 ? 2 : 1), frame, lr);
}

// The inner part's vtable slot (byte offset `slot`).
uint64_t InnerSlot(uint8_t* base, uint32_t inner, uint32_t slot, std::optional<uint64_t> r4,
                   std::optional<uint64_t> r5, uint32_t frame, uint32_t lr, const Calls& calls) {
  const uint32_t target = LoadU32(base, LoadU32(base, inner) + slot);
  return CallTarget(base, target, inner, r4, r5, frame, lr, calls);
}

// operator delete on `object` from `frame` returning to `lr` (r4 the flags,
// as the original leaves them).
void Delete(uint64_t object, uint64_t flags, uint32_t frame, uint32_t lr, const Calls& calls) {
  const uint64_t args[6] = {object, flags, 0, 0, 0, 0};
  calls.call_set(car_list::kOperatorDelete, args, 2, frame, lr);
}

// The page or step queued with the one just done (the inner's +4 to +28):
// no step queued (+20 -1): +16 = -1, +4 = +8; else +24 = 0, +16 = +20,
// +20 = -1; then +28 = 0.
void MoveOn(uint8_t* base, uint32_t inner) {
  const uint32_t queued = LoadU32(base, inner + 20);
  if (queued == 0xFFFFFFFFu) {
    const uint32_t after = LoadU32(base, inner + 8);
    StoreU32(base, inner + 16, 0xFFFFFFFFu);
    StoreU32(base, inner + 4, after);
  } else {
    StoreU32(base, inner + 24, 0);
    StoreU32(base, inner + 16, queued);
    StoreU32(base, inner + 20, 0xFFFFFFFFu);
  }
  StoreU32(base, inner + 28, 0);
}

}  // namespace

// --- The destructors ---

uint64_t ListPartBaseDestruct(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82252420 ListPartBase::~ListPartBase");
  StoreU32(base, uint32_t(r3), kListPartBaseVtable);
  return r3;
}

uint64_t ListPartInnerBaseDestruct(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82252600 ListPartInnerBase::~ListPartInnerBase");
  StoreU32(base, uint32_t(r3), kListPartInnerBaseVtable);
  return r3;
}

uint64_t ListPartBaseDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252610 ListPartBase::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kListPartBaseVtable);
  if ((flags & 1) != 0) {
    Delete(r3, flags, frame, 0x8225263C, calls);
  }
  return r3;
}

uint64_t ListPartInnerBaseDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags,
                                           uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252658 ListPartInnerBase::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kListPartInnerBaseVtable);
  if ((flags & 1) != 0) {
    Delete(r3, flags, frame, 0x82252684, calls);
  }
  return r3;
}

uint64_t ListPartInnerDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822526F8 ListPartInner::~ListPartInner");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kListPartInnerVtable);
  ReleaseList(base, r3 + 40, frame, 0x82252720, calls);
  return ListPartInnerBaseDestruct(base, r3);
}

uint64_t ListPartInnerDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822529B0 ListPartInner::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  ListPartInnerDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    Delete(r3, flags, frame, 0x822529E0, calls);
  }
  return r3;
}

uint64_t ListPartDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252968 ListPart::~ListPart");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, uint32_t(r3), kListPartVtable);
  ListPartInnerDestruct(base, r3 + 20, frame, calls);
  return ListPartBaseDestruct(base, r3);
}

uint64_t ListPartDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252A38 ListPart::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  ListPartDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    Delete(r3, flags, frame, 0x82252A68, calls);
  }
  return r3;
}

// --- The part's slots ---

uint64_t ListPartOpen(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252430 ListPartBase::Open");
  const uint32_t part = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t inner = LoadU32(base, part + 16);
  if (inner != 0) {
    InnerSlot(base, inner, 4, LoadU32(base, part + 12), std::nullopt, frame, 0x82252468, calls);
  }
  return Slot(base, r3, 36, std::nullopt, frame, 0x8225247C, calls);
}

uint64_t ListPartPick(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252490 ListPartBase::Pick");
  const uint32_t part = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t inner = LoadU32(base, part + 16);
  if (inner == 0) {
    return 1;
  }
  const uint64_t answer =
      InnerSlot(base, inner, 8, LoadU32(base, part + 12), r4, frame, 0x822524D0, calls);
  if (int32_t(uint32_t(answer)) != 0) {
    const uint32_t now = LoadU32(base, part + 16);
    StoreU32(base, part + 4, LoadU32(base, now + 32));
    StoreU32(base, part + 8, LoadU32(base, now + 36));
  }
  return answer;
}

uint64_t ListPartInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252500 ListPartBase::Input");
  const uint32_t part = uint32_t(r3);
  const uint32_t inner = LoadU32(base, part + 16);
  if (inner == 0) {
    return r3;
  }
  return InnerSlot(base, inner, 12, LoadU32(base, part + 12), r4, sp, lr, calls);
}

uint64_t ListPartClose(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252530 ListPartBase::Close");
  const uint32_t part = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint32_t inner = LoadU32(base, part + 16);
  if (inner != 0) {
    InnerSlot(base, inner, 20, std::nullopt, std::nullopt, frame, 0x82252564, calls);
  }
  return Slot(base, r3, 40, std::nullopt, frame, 0x82252578, calls);
}

uint64_t ListPartIsIdle(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252590 ListPartBase::IsIdle");
  const uint32_t inner = LoadU32(base, uint32_t(r3) + 16);
  if (inner == 0) {
    return 1;
  }
  return InnerSlot(base, inner, 16, std::nullopt, std::nullopt, sp, lr, calls);
}

uint64_t ListPartCanLeave(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822525B8 ListPartBase::CanLeave");
  const uint32_t inner = LoadU32(base, uint32_t(r3) + 16);
  if (inner == 0) {
    return 1;
  }
  return InnerSlot(base, inner, 24, std::nullopt, std::nullopt, sp, lr, calls);
}

// --- The inner part's slots ---

uint64_t ListPartInnerOpen(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252A88 ListPartInner::Open");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 144);
  StoreU32(base, inner + 8, 0);
  StoreU32(base, inner + 4, 0);
  StoreU32(base, inner + 12, 0);
  StoreU32(base, inner + 24, 0xFFFFFFFFu);
  StoreU32(base, inner + 16, 0xFFFFFFFFu);
  StoreU32(base, inner + 28, 0);
  StoreU32(base, inner + 16, 0);
  StoreU32(base, inner + 20, 0xFFFFFFFFu);
  StoreU32(base, inner + 8, 0);
  const uint32_t movie = MovieAt(base, frame + 96, uint32_t(r4));
  StoreU32(base, inner + 4, 0xFFFFFFFFu);
  calls.call(kResetPose, frame, 0x82252AFC, movie, frame + 96);
  calls.set_text(frame, 0x82252B10, movie, 0xFFFFFFFFu, 0, 10);
  calls.set_text(frame, 0x82252B24, movie, 1, 3, 0);
  calls.set_text(frame, 0x82252B38, movie, 1, 4, 1);
  StoreU32(base, movie + 40960, 1);
  StoreU32(base, frame + 92, 158);
  StoreU32(base, frame + 84, 159);
  const uint64_t list = r3 + 40;
  if (calls.native_heap && save_data::MenuListInitNative(base, uint32_t(list))) {
    save_data::MenuListInit(base, uint32_t(list), {2, 2, 1, 154, 153, 155, 156, 159, 158});
  } else {
    const uint64_t args[8] = {list, 2, 2, 1, 154, 153, 155, 156};
    calls.call8(kListInit, args, frame, 0x82252B80);
  }
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t selection = LoadU32(base, save + kOptionsWord) == 0 ? 1 : 0;
  race_screens::ListSetSelection(base, uint32_t(list), selection);
  StoreU32(base, inner + 112, 0);
  StoreU32(base, inner + 116, 0);
  return list;
}

uint64_t ListPartInnerPick(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                           uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252A00 ListPartInner::Pick");
  const uint32_t page = LoadU32(base, uint32_t(r3) + 4);
  if (page == 0xFFFFFFFFu) {
    return 0;
  }
  const uint32_t target = LoadU32(base, kListPartPages + (page << 2));
  return CallTarget(base, target, r3, r4, r5, sp, lr, calls);
}

uint64_t ListPartInnerInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252DF0 ListPartInner::Input");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t step = LoadU32(base, inner + 16);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (step == 0xFFFFFFFFu) {
    return calls.call(kRun, frame, 0x82252E94, movie, 0);
  }
  const uint32_t target = LoadU32(base, kListPartSteps + (step << 2));
  const uint64_t done = CallTarget(base, target, r3, r4, std::nullopt, frame, 0x82252E4C, calls);
  if (int32_t(uint32_t(done)) != 0) {
    MoveOn(base, inner);
  }
  return done;
}

uint64_t ListPartInnerIsIdle(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82252748 ListPartInner::IsIdle");
  Frame(base, sp, 96);
  return race_screens::ListSelection(base, uint32_t(r3) + 40) == 0 ? 1 : 0;
}

uint64_t ListPartInnerClose(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252740 ListPartInner::Close");
  return ReleaseList(base, r3 + 40, sp, lr, calls);
}

uint64_t ListPartInnerCanLeave(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822528C8 ListPartInner::CanLeave");
  const uint32_t inner = uint32_t(r3);
  StoreU32(base, inner + 116, 1);
  const int32_t state = int32_t(LoadU32(base, inner + 112));
  if (state == 3) {
    return 1;
  }
  if (state != 2 && state != 4) {
    StoreU32(base, inner + 112, 4);
  }
  return 0;
}

// --- The inner part's pages ---

uint64_t ListPartInnerPage0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252778 ListPartInner::Page0");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  // The page left for a step (the page before kept unless it's `same`).
  auto leave = [&](uint32_t same, uint32_t step, uint32_t after) {
    const uint32_t page = LoadU32(base, inner + 4);
    if (page != same) {
      StoreU32(base, inner + 12, page);
    }
    StoreU32(base, inner + 8, after);
    StoreU32(base, inner + 16, step);
    StoreU32(base, inner + 4, 0xFFFFFFFFu);
    StoreU32(base, inner + 20, 0xFFFFFFFFu);
  };
  if (int32_t(LoadU32(base, inner + 112)) == 4) {
    const uint32_t page = LoadU32(base, inner + 4);
    if (page != 2) {
      StoreU32(base, inner + 12, page);
    }
    StoreU32(base, inner + 16, 1);
    StoreU32(base, inner + 20, 0xFFFFFFFFu);
    StoreU32(base, inner + 8, 2);
    StoreU32(base, inner + 4, 0xFFFFFFFFu);
  }
  const uint64_t list = r3 + 40;
  calls.call(kListUpdate, frame, 0x822527CC, list, 0);
  if (LoadU32(base, inner + 64) != 0) {
    PlaySound(base, 4, frame, calls);
    const uint32_t page = LoadU32(base, inner + 4);
    if (page != 0) {
      StoreU32(base, inner + 12, page);
    }
    StoreU32(base, inner + 16, 2);
    StoreU32(base, inner + 8, 0);
    StoreU32(base, inner + 4, 0xFFFFFFFFu);
    StoreU32(base, inner + 20, 0xFFFFFFFFu);
    return 0;
  }
  if (int32_t(uint32_t(calls.call(kListAccepted, frame, 0x82252808, list, 0))) != 0) {
    PlaySound(base, 0, frame, calls);
    leave(1, 1, 1);
    return 0;
  }
  if (int32_t(uint32_t(calls.call(kListCancelled, frame, 0x82252838, list, 0))) != 0) {
    PlaySound(base, 2, frame, calls);
    leave(2, 1, 2);
  }
  return 0;
}

uint64_t ListPartInnerPage1(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82252878 ListPartInner::Page1");
  const uint32_t inner = uint32_t(r3);
  const bool stay = int32_t(LoadU32(base, inner + 116)) == 0;
  StoreU32(base, inner + 36, stay ? 0 : 1);
  StoreU32(base, inner + 32, stay ? 1 : 0);
  return 1;
}

uint64_t ListPartInnerPage2(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822528A8 ListPartInner::Page2");
  const uint32_t inner = uint32_t(r3);
  StoreU32(base, inner + 32, 0);
  StoreU32(base, inner + 36, 1);
  return 1;
}

// --- The inner part's steps ---

uint64_t ListPartInnerStep0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252BB8 ListPartInner::Step0");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (int32_t(LoadU32(base, inner + 28)) == 0) {
    ui_movies::PlayNamed(base, movie, 5, 0, 0, 0, 0);
    StoreU32(base, inner + 28, 1);
    StoreU32(base, inner + 112, 0);
    PlaySound(base, 5, frame, calls);
  }
  const uint64_t done = uint32_t(calls.call(kRun, frame, 0x82252C30, movie, 0)) == 0 ? 1 : 0;
  if (done != 0) {
    const uint32_t selection = race_screens::ListSelection(base, inner + 40);
    ui_movies::PlayNamed(base, movie, LoadU32(base, kListPartNamed + (selection << 2)), 1, 0, 0,
                         0);
    ui_movies::LoopLast(base, movie);
    if (int32_t(LoadU32(base, inner + 112)) != 4) {
      StoreU32(base, inner + 112, 1);
    }
  }
  return done;
}

uint64_t ListPartInnerStep1(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252C98 ListPartInner::Step1");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (int32_t(LoadU32(base, inner + 28)) == 0) {
    ui_movies::PlayNamed(base, movie, 6, 0, 0, 0, 0);
    StoreU32(base, inner + 28, 1);
    StoreU32(base, inner + 112, 2);
    PlaySound(base, 5, frame, calls);
  }
  const uint64_t done = uint32_t(calls.call(kRun, frame, 0x82252D10, movie, 0)) == 0 ? 1 : 0;
  if (done != 0) {
    calls.call(kFinishAll, frame, 0x82252D24, movie, 0);
    StoreU32(base, inner + 112, 3);
  }
  return done;
}

uint64_t ListPartInnerStep2(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82252D38 ListPartInner::Step2");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (int32_t(LoadU32(base, inner + 28)) == 0) {
    const uint32_t list = inner + 40;
    // MenuList::GetPreviousSelection (sub_8220BC28): +20.
    const uint32_t previous = LoadU32(base, list + 20);
    calls.call(kFinishNamed, frame, 0x82252D98, movie,
               LoadU32(base, kListPartNamed + (previous << 2)));
    const uint32_t selection = race_screens::ListSelection(base, list);
    ui_movies::PlayNamed(base, movie, LoadU32(base, kListPartNamed + (selection << 2)), 1, 0, 0,
                         0);
    ui_movies::LoopLast(base, movie);
    StoreU32(base, inner + 28, 1);
  }
  calls.call(kRun, frame, 0x82252DE0, movie, 0);
  return 1;
}

}  // namespace type67_screens
