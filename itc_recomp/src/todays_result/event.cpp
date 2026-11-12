// Today's event's machine (docs/functions/todays-result.md): see
// todays_result.h. Most of it never ran in play (no unlock was made at the
// end of a day in the sessions measured): ported with its callers, as the
// garage screen's paths were (the user's call of 2026-11-09).

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "todays_result/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace todays_result {

namespace {

uint32_t Pages(uint8_t* base) { return Load(base, kEventPages); }
uint32_t Data(uint8_t* base) { return Load(base, kEventData); }

// The image lists: list A at the data's +4 (count +188), list B at +192
// (count +376).
constexpr uint32_t kListA = 4;
constexpr uint32_t kListB = 192;
constexpr uint32_t kListCount = 184;  // from the list

// Whether bit `index` of the words at `bits` is set (a negative index none).
bool BitSet(uint8_t* base, uint32_t bits, int32_t index) {
  if (index < 0) {
    return false;
  }
  return (Load(base, bits + 4 * uint32_t(index >> 5)) & (1u << (index & 31))) != 0;
}

uint32_t ImagesLoaded(uint8_t* base, uint32_t list) {
  const uint32_t count = Load(base, list + kListCount);
  for (uint32_t k = 0; k < count; k++) {
    if (Load(base, Load(base, list + 4 * k)) == 0) {
      return 0;
    }
  }
  return 1;
}

// The movie's +28 object's +36 and +32 put back from the data's +400 and
// +404 (Unload's and Leave's).
void PutBackImages(uint8_t* base, uint32_t movie) {
  Store(base, Load(base, movie + 28) + 36, Load(base, Data(base) + 400));
  Store(base, Load(base, movie + 28) + 32, Load(base, Data(base) + 404));
}

}  // namespace

uint32_t ImagesLoadedA(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_8225C0A0 TodaysEvent::ImagesLoadedA");
  return ImagesLoaded(base, data + kListA);
}

uint32_t ImagesLoadedB(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_8225C0E8 TodaysEvent::ImagesLoadedB");
  return ImagesLoaded(base, data + kListB);
}

uint64_t MarkImages(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225BAB8 TodaysEvent::MarkImages");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  uint64_t r3 = save;
  const uint32_t unlocks = save + kUnlocks;
  for (uint32_t k = 0; k < Load(base, unlocks + 768); k++) {
    const uint32_t unlock = unlocks + 12 * k;
    const uint32_t table = Load(base, unlock + 4);
    uint32_t entry = Load(base, unlock + 8);
    Store(base, frame + 80, Load(base, unlock + 0));
    while (true) {
      const uint32_t record = data_tables::Entry56(base, table, entry);
      r3 = record;
      const uint32_t bits = Data(base) + 380;
      const int32_t image = int32_t(Load(base, record + 16));
      if (image >= 0) {
        const uint32_t word = bits + 4 * uint32_t(image >> 5);
        Store(base, word, Load(base, word) | (1u << (image & 31)));
      }
      const uint32_t next = Load(base, record + 8);
      if (next == 0) {
        break;
      }
      entry += next;
    }
  }
  return r3;
}

uint64_t EventCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225BE50 TodaysEvent::Create");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8225BE7C, 0, 28, 0x1000, 4));
  Store(base, kEventPages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  Store(base, pages + 24, 0);
  const uint32_t menu = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8225BEB8, 0, 68, 0x1000, 4));
  Store(base, kEventMenu, menu);
  for (const uint32_t field : {12u, 20u, 0u, 4u}) {
    Store(base, menu + field, 0);
  }
  Store(base, menu + 16, 4);
  for (const uint32_t field : {8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + field, 0);
  }
  const uint32_t data = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8225BF14, 0, 68, 0x1000, 4));
  Store(base, kEventData, data);
  Store(base, data + 0, 0);
  Store(base, data + 396, kNone);
  Store(base, data + 188, 0);
  Store(base, data + 376, 0);
  Store(base, data + 380, 0);
  Store(base, data + 384, 0);
  const uint32_t kind = Kind(base, frame);
  if (kind == 0) {
    Store(base, machine + kMachineLoaded, 1);
    Store(base, machine + kMachineState, 9);
    return kind;
  }
  const uint64_t r3 = MarkImages(base, frame);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t LoadImagePair(uint8_t* base, uint32_t data, uint32_t index, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225BD10 TodaysEvent::LoadImagePair");
  const uint32_t frame = Frame(base, sp, 144);
  uint64_t r3 = 0;
  struct Pair {
    uint32_t asset, list, lrs[5];
  };
  static constexpr Pair kPairs[2] = {
      {kImageAssetA, kListA, {0x8225BD30, 0x8225BD48, 0x8225BD58, 0x8225BD6C, 0x8225BDB8}},
      {kImageAssetB, kListB, {0x8225BDC4, 0x8225BDDC, 0x8225BDEC, 0x8225BE00, 0x8225BE48}},
  };
  for (const Pair& pair : kPairs) {
    const uint32_t asset = pair.asset + index;
    const uint64_t size = calls.Call(kAssetSize, frame, pair.lrs[0], asset);
    const uint32_t buffer =
        uint32_t(calls.Call(kVirtualAlloc, frame, pair.lrs[1], 0, size, 0x1000, 4));
    calls.Call(kXMemSet, frame, pair.lrs[2], buffer, 0, size);
    const uint32_t record =
        uint32_t(calls.Call(kVirtualAlloc, frame, pair.lrs[3], 0, 16, 0x1000, 4));
    Store(base, record + 4, index);
    Store(base, record + 0, 0);
    Store(base, record + 8, 0);
    Store(base, record + 12, 0);
    const uint32_t list = data + pair.list;
    const uint32_t count = Load(base, list + kListCount);
    Store(base, list + kListCount, count + 1);
    Store(base, list + (count << 2), record);
    const uint64_t args[8] = {asset, buffer, kImageLoaded, record, 0, 0, 0, 0};
    r3 = calls.CallArgs(kArchiveLoad, frame, pair.lrs[4], args);
  }
  return r3;
}

uint64_t LoadImages(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C008 TodaysEvent::LoadImages");
  const uint32_t frame = Frame(base, sp, 112);
  uint32_t data = Data(base);
  for (uint32_t image = 0; image < kImages; image++) {
    if (BitSet(base, data + 380, int32_t(image))) {
      LoadImagePair(base, data, image, frame, calls);
      data = Data(base);
    }
  }
  return data;
}

uint64_t EventOpenMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C130 TodaysEvent::OpenMovie");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMoviesOpen, frame, 0x8225C148, kEventMovie, kEventAsset);
  return LoadImages(base, frame, calls);
}

uint64_t EventWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225C160 TodaysEvent::WaitForMovie");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kEventMovie);
    if (uint32_t(r3) != 0) {
      const uint32_t data = Data(base);
      r3 = ImagesLoadedA(base, data);
      if (uint32_t(r3) != 0) {
        r3 = ImagesLoadedB(base, data);
        if (uint32_t(r3) != 0) {
          Store(base, machine + kMachineLoaded, 1);
        }
      }
    }
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 9 : 4);
  }
  return r3;
}

uint32_t NextUnlockEntry(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225BB70 TodaysEvent::NextUnlockEntry");
  Frame(base, sp, 96);
  const uint32_t data = Data(base);
  const uint32_t entry = Load(base, data + 396);
  if (entry != kNone) {
    const uint32_t record = data_tables::Entry56(base, Load(base, data + 392), entry);
    const uint32_t next = Load(base, record + 8);
    if (next != 0) {
      const uint32_t at = Data(base) + 396;
      Store(base, at, next + Load(base, at));
      return 0;
    }
  }
  const uint32_t unlocks = vehicle_accessors::SaveDataOf(base, 0) + kUnlocks;
  const uint32_t count = Load(base, unlocks + 768);
  for (uint32_t k = 0; k < count; k++) {
    const uint32_t unlock = unlocks + 12 * k;
    if (Load(base, unlock + 0) == 0) {
      Store(base, unlock + 0, 1);
      const uint32_t shown = Data(base) + 388;
      Store(base, shown + 0, Load(base, unlock + 0));
      Store(base, shown + 4, Load(base, unlock + 4));
      Store(base, shown + 8, Load(base, unlock + 8));
      return 1;
    }
  }
  Store(base, vehicle_accessors::SaveDataOf(base, 0) + kUnlockCount, 0);
  return 2;
}

uint64_t SetUpTexts(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C648 TodaysEvent::SetUpTexts");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const int32_t day = int32_t(Load(base, save + 6100));
  calls.Call(kSetChildNameText, frame, 0x8225C698, movie, 8, 6, save + 8);
  calls.Call(kSetNumberText, frame, 0x8225C6AC, movie, 8, 9, Load(base, save + 6116));
  // SetText's varargs past the day 0 (the user's call of 2026-10-30).
  const uint64_t args[8] = {movie, 8, 13, 8, uint64_t(uint32_t(day > 999 ? 999 : day)), 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8225C6D0, args);
  Store(base, ui_movies::FindNode(base, movie, 8, 11) + 108, uint32_t(day - (day / 7) * 7));
  Store(base, ui_movies::FindNode(base, movie, 10, 6) + 80, 0);
  Store(base, ui_movies::FindNode(base, movie, 10, 8) + 80, 0);
  Store(base, ui_movies::FindNode(base, movie, 10, 7) + 80, 0);
  const uint32_t last = ui_movies::FindNode(base, movie, 10, 9);
  Store(base, last + 80, 0);
  Store(base, Data(base) + 0, 1);
  return last;
}

uint64_t EventStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C990 TodaysEvent::Start");
  const uint32_t frame = Frame(base, sp, 112);
  // Sound::Play(&bank 1, &cue 2).
  Store(base, frame + 80, 2);
  Store(base, frame + 84, 1);
  calls.Call(kSoundPlay, frame, 0x8225C9BC, frame + 84, frame + 80);
  const uint32_t movie = FindMovie(base, frame + 84, kEventMovie);
  Store(base, Data(base) + 400, Load(base, Load(base, movie + 28) + 36));
  Store(base, Data(base) + 404, Load(base, Load(base, movie + 28) + 32));
  SetUpTexts(base, frame, calls);
  const uint32_t answer = NextUnlockEntry(base, frame);
  if (answer == 2) {
    return answer;
  }
  const uint32_t pages = Pages(base);
  const uint32_t page = Load(base, pages + 0);
  if (page != 0) {
    Store(base, pages + 8, page);
  }
  Store(base, pages + 12, 0);
  Store(base, pages + 4, 0);
  Store(base, pages + 16, kNone);
  Store(base, pages + 0, kNone);
  return answer;
}

uint64_t EventUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C1F8 TodaysEvent::Unload");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, frame + 80, kEventMovie);
  uint64_t r3 = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = Load(base, uint32_t(r3));
  if (movie != 0) {
    r3 = calls.Call(kFinishAll, frame, 0x8225C234, movie);
    PutBackImages(base, movie);
  }
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8225C26C, kEventMovie);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

uint64_t EventUnloaded(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C278 TodaysEvent::Unloaded");
  const uint32_t frame = Frame(base, sp, 144);
  // The sound manager's IsCuePlaying (slot 14) for &bank 1, &cue 2, and when
  // it answers a nonzero byte its StopCue (slot 13), as the original calls
  // them.
  uint32_t manager = Load(base, kSoundManager);
  Store(base, frame + 80, 2);
  Store(base, frame + 84, 1);
  uint64_t r3 = calls.Call(Load(base, Load(base, manager) + 56), frame, 0x8225C2BC, manager,
                           frame + 84, frame + 80);
  if ((r3 & 0xFF) != 0) {
    manager = Load(base, kSoundManager);
    Store(base, frame + 88, 2);
    Store(base, frame + 92, 1);
    r3 = calls.Call(Load(base, Load(base, manager) + 52), frame, 0x8225C2E8, manager,
                    frame + 92, frame + 88);
  }
  Post(base, dispatcher, 6);
  Store(base, machine + kMachineState, 9);
  return r3;
}

uint64_t EventLeave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C348 TodaysEvent::Leave");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x8225C398, movie);
    PutBackImages(base, movie);
  }
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8225C3C4, kEventMovie);
  Post(base, dispatcher, 4);
  return r3;
}

uint64_t FreeImages(uint8_t* base, uint32_t data, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225BC60 TodaysEvent::FreeImages");
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t r3 = data;
  struct Lrs {
    uint32_t list, free_image, free;
  };
  static constexpr Lrs kLists[2] = {{kListA, 0x8225BC9C, 0x8225BCB0},
                                    {kListB, 0x8225BCE8, 0x8225BCFC}};
  for (const Lrs& l : kLists) {
    const uint32_t list = data + l.list;
    while (Load(base, list + kListCount) != 0) {
      const uint32_t count = Load(base, list + kListCount) - 1;
      Store(base, list + kListCount, count);
      const uint32_t record = Load(base, list + (count << 2));
      calls.Call(kFreeImage, frame, l.free_image, record);
      r3 = calls.Call(kVirtualFree, frame, l.free, record, 0, kMemRelease);
    }
  }
  return r3;
}

uint64_t EventDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225BF78 TodaysEvent::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  if (Load(base, Data(base) + 408) != 0) {
    Store(base, machine + kMachineState, 20);
  }
  FreeImages(base, Data(base), frame, calls);
  calls.Call(kVirtualFree, frame, 0x8225BFC8, Load(base, kEventPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8225BFDC, Load(base, kEventMenu), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8225BFEC, Load(base, kEventData), 0, kMemRelease);
  Store(base, kEventPages, 0);
  Store(base, kEventMenu, 0);
  Store(base, kEventData, 0);
  return r3;
}

}  // namespace todays_result

namespace todays_result {

namespace {

// The pages' table in Draw's frame: EventPage, EndPage.
constexpr uint32_t kEventPageTable[2] = {0x8225C428, 0x8225C590};
// The transitions' table in Update's frame (+96 on): Intro, FadeOut,
// NextUnlock, Type2Hub::Transition9, NextPage, Type2Hub::Transition11.
constexpr uint32_t kEventTransitionTable[6] = {0x8225CB18, 0x8225C770, 0x8225CC80,
                                               0x8218F150, 0x8225CD50, 0x8218F220};
constexpr uint64_t kAll = ~uint64_t(0);

uint32_t EventPages(uint8_t* base) { return Load(base, kEventPages); }
uint32_t EventData(uint8_t* base) { return Load(base, kEventData); }

// UiMovie::Play(movie, a, b) with r6 to r9 0, as the originals pass them.
void Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t a,
          uint64_t b) {
  const uint64_t args[8] = {movie, a, b, 0, 0, 0, 0, 0};
  calls.CallArgs(kPlay, frame, lr, args);
}
uint64_t RunMovie(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}
uint64_t FadesDone(uint8_t* base) {
  return (Load(base, kFadeCount0) | Load(base, kFadeCount1)) == 0 ? 1 : 0;
}

}  // namespace

uint64_t EventDraw(uint8_t* base, uint64_t machine, uint64_t dispatcher, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C928 TodaysEvent::Draw");
  const uint32_t frame = Frame(base, sp, 96);
  Store(base, frame + 80, kEventPageTable[0]);
  Store(base, frame + 84, kEventPageTable[1]);
  const uint32_t page = Load(base, EventPages(base) + 0);
  if (page == kNone) {
    return dispatcher;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x8225C980, dispatcher,
                    machine);
}

uint64_t EventUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CDE8 TodaysEvent::Update");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  const uint32_t pages = EventPages(base);
  Store(base, frame + 116, kEventTransitionTable[5]);
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, frame + 96 + 4 * k, kEventTransitionTable[k]);
  }
  const uint32_t running = Load(base, pages + 12);
  if (running == kNone) {
    return RunMovie(calls, frame, 0x8225CF14, movie);
  }
  uint64_t r3 = movie;
  uint32_t p = pages;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(base, frame + 96 + (running << 2)), frame, 0x8225CEAC, pages + 24);
    p = EventPages(base);
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

uint64_t EventPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C428 TodaysEvent::EventPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t pressed =
      calls.Call(kMenuPressed, frame, 0x8225C44C, Load(base, kEventMenu), kAccept);
  if (uint32_t(pressed) == 0) {
    return pressed;
  }
  calls.Call(kMenuSoundPlay, frame, 0x8225C45C, 0);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  const uint64_t revealed = calls.Call(kRevealText, frame, 0x8225C488, movie, 0, 2);
  if (uint32_t(revealed) != 0) {
    const uint64_t more = calls.Call(kHasMoreText, frame, 0x8225C548, movie, 0, 2);
    const uint32_t node = ui_movies::FindNode(base, movie, 0, 0);
    Store(base, node + 80, uint32_t(more) != 0 ? 1 : 0);
    return node;
  }
  const uint32_t answer = NextUnlockEntry(base, frame);
  if (answer >= 3) {
    return answer;
  }
  const uint32_t pages = EventPages(base);
  const uint32_t page = Load(base, pages + 0);
  if (answer == 2) {
    if (page != 1) {
      Store(base, pages + 8, page);
    }
    Store(base, pages + 12, 1);
    Store(base, pages + 16, kNone);
    Store(base, pages + 4, 1);
    Store(base, pages + 0, kNone);
    return answer;
  }
  if (page != 0) {
    Store(base, pages + 8, page);
  }
  // 0: the entry's next page through Transition11, then NextPage; 1: the
  // next unlock through Transition9, then NextUnlock.
  Store(base, pages + 12, answer == 0 ? 5 : 3);
  Store(base, pages + 16, answer == 0 ? 4 : 2);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, kNone);
  return answer;
}

uint64_t QueueEventMovies(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225B9B8 TodaysEvent::QueueEventMovies");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t queued = save + kQueuedMovie;
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, kBootMovies + 4 * k, 11);
  }
  Store(base, kBootMovies + 20, 13);
  Store(base, kBootMovies + 28, 0);
  Store(base, kBootMovies + 24, 1);
  const uint32_t movie = Load(base, queued);
  Store(base, kBootMovies + 0, movie);
  if (int32_t(movie) == 6) {
    Store(base, kBootMovies + 4, 7);
  }
  Store(base, queued, kNone);
  Store(base, kBootMovies + 20, 9);
  return calls.Call(kCheckUnlocks, frame, 0x8225BA38, save);
}

uint64_t EndPage(uint8_t* base, uint32_t dispatcher, uint32_t machine, uint32_t sp,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C590 TodaysEvent::EndPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t kind = Kind(base, frame);
  if (kind == 1) {
    Post(base, dispatcher, 7);
    Store(base, EventData(base) + 408, 0);
    return kind;
  }
  Store(base, machine + kMachineState, 10);
  Store(base, EventData(base) + 408, 1);
  return QueueEventMovies(base, frame, calls);
}

uint64_t ShowImages(uint8_t* base, uint32_t index, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225C848 TodaysEvent::ShowImages");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, frame + 80, kEventMovie);
  const uint32_t pair = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = Load(base, pair);
  // The texture record of the list's first record for the image, 0 none.
  auto texture = [&](uint32_t list) {
    const uint32_t count = Load(base, list + 184);
    for (uint32_t k = 0; k < count; k++) {
      const uint32_t record = Load(base, list + 4 * k);
      if (Load(base, record + 4) == index) {
        return Load(base, record + 8);
      }
    }
    return 0u;
  };
  Store(base, Load(base, movie + 28) + 36, texture(EventData(base) + 4));
  Store(base, Load(base, movie + 28) + 32, texture(EventData(base) + 192));
  return pair;
}

uint64_t ShowUnlock(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CA50 TodaysEvent::ShowUnlock");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  const uint32_t data = EventData(base);
  const uint32_t record =
      data_tables::Entry56(base, Load(base, data + 392), Load(base, data + 396));
  // SetText's varargs 0 (the user's call of 2026-10-30).
  const uint64_t title[8] = {movie, 0, 1, uint64_t(Load(base, record + 12)) + 4650, 0, 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8225CAAC, title);
  const uint64_t body[8] = {
      movie, 0, 2, uint64_t(Load(base, EventData(base) + 396)) + 6683, 0, 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8225CAC8, body);
  ShowImages(base, Load(base, record + 16), frame);
  const uint64_t more = calls.Call(kHasMoreText, frame, 0x8225CAE0, movie, 0, 2);
  const uint32_t node = ui_movies::FindNode(base, movie, 0, 0);
  Store(base, node + 80, uint32_t(more) != 0 ? 1 : 0);
  return node;
}

uint64_t EventIntro(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CB18 TodaysEvent::Intro");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x8225CB78, movie, kAll, 2);
    Play(calls, frame, 0x8225CB98, movie, 8, 0);
    calls.Call(kLoopLast, frame, 0x8225CBA0, movie);
    Play(calls, frame, 0x8225CBC0, movie, 15, 0);
    calls.Call(kLoopLast, frame, 0x8225CBC8, movie);
    calls.Call(kPlayNamed, frame, 0x8225CBE4, movie, 2);
    calls.Call(kLoopNamed, frame, 0x8225CBF0, movie, 2);
    calls.Call(kFadeIn, frame, 0x8225CC0C, Load(base, kFadesIn), 0, 0, 0);
    ShowUnlock(base, frame, calls);
    Store(base, started, 1);
  }
  const uint64_t done = uint32_t(RunMovie(calls, frame, 0x8225CC24, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    Play(calls, frame, 0x8225CC50, movie, 12, 1);
    calls.Call(kLoop, frame, 0x8225CC60, movie, 12, 1);
  }
  return done;
}

uint64_t EventFadeOut(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C770 TodaysEvent::FadeOut");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (Load(base, started) == 0) {
    calls.Call(kFinish, frame, 0x8225C7C0, movie, 8, 0);
    calls.Call(kFinish, frame, 0x8225C7D0, movie, 15, 0);
    calls.Call(kFinishNamed, frame, 0x8225C7DC, movie, 2);
    calls.Call(kFadeOut, frame, 0x8225C7F4, 60, 0, 0, 0);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8225C808, movie);
  return FadesDone(base);
}

uint64_t NextUnlock(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CC80 TodaysEvent::NextUnlock");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (Load(base, started) == 0) {
    calls.Call(kFadeIn, frame, 0x8225CCD8, 60, 0, 0, 0);
    Play(calls, frame, 0x8225CCF8, movie, kAll, 4);
    ShowUnlock(base, frame, calls);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8225CD10, movie);
  return FadesDone(base);
}

uint64_t NextPage(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CD50 TodaysEvent::NextPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x8225CDB0, movie, kAll, 4);
    ShowUnlock(base, frame, calls);
    Store(base, started, 1);
  }
  return uint32_t(RunMovie(calls, frame, 0x8225CDC8, movie)) == 0 ? 1 : 0;
}

}  // namespace todays_result
