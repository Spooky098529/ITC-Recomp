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
