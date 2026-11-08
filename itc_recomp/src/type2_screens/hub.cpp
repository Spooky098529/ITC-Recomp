// The hub's handlers and helpers (docs/functions/type2-screens.md): see
// type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "build_screen/build_screen.h"
#include "data_tables/data_tables.h"
#include "type2_screens/internal.h"
#include "type67_modes/type67_modes.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kNone = 0xFFFFFFFFu;
// The debug print's format WaitForLoad hands the empty sub_82106540.
constexpr uint32_t kWaitFormat = 0x8200A1F8;
// Player 0's notice lists in the save data: 12-byte entries, their counts
// after them.
constexpr uint32_t kNotices = 0x1C778;
constexpr uint32_t kNoticeCount = 0x1CA78;
constexpr uint32_t kNotices2 = 0x1CA84;
constexpr uint32_t kNotice2Count = 2688;  // from kNotices2

uint32_t Data(uint8_t* base) { return Load(base, kHubData); }

// Movie 0's object's two image words put back from the data's +440, +444.
void PutBackImages(uint8_t* base, uint32_t movie) {
  Store(base, Load(base, movie + 28) + 36, Load(base, Data(base) + 440));
  Store(base, Load(base, movie + 28) + 32, Load(base, Data(base) + 444));
}

}  // namespace

void HubOnYes(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8218D6A8 Type2Hub::OnYes");
  Store(base, Data(base) + 28, 0);
}

void HubOnNo(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8218D6C0 Type2Hub::OnNo");
  Store(base, Data(base) + 28, 1);
}

uint32_t HubImagesLoadedA(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_8218E2D8 Type2Hub::ImagesLoadedA");
  const uint32_t count = Load(base, data + 216);
  for (uint32_t i = 0; i < count; i++) {
    if (Load(base, Load(base, data + 32 + 4 * i)) == 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t HubImagesLoadedB(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_8218E320 Type2Hub::ImagesLoadedB");
  const uint32_t count = Load(base, data + 404);
  for (uint32_t i = 0; i < count; i++) {
    if (Load(base, Load(base, data + 220 + 4 * i)) == 0) {
      return 0;
    }
  }
  return 1;
}

uint64_t HubWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E420 Type2Hub::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Load(base, machine + kMachineLoaded) == 0) {
    bool loaded = false;
    for (const uint32_t id : {0u, 9u, 10u}) {
      r3 = ui_movies::IsLoaded(base, id);
      if (uint32_t(r3) == 0) {
        break;
      }
      if (id == 10) {
        const uint32_t data = Data(base);
        r3 = HubImagesLoadedA(base, data);
        if (r3 != 0) {
          r3 = HubImagesLoadedB(base, data);
          loaded = r3 != 0;
        }
      }
    }
    if (loaded) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  uint32_t count = Load(base, kHubCounter);
  if (((count / 15) & 1) == 0) {
    r3 = calls.Call(kEmpty, frame, 0x8218E4D4, 100, 30, 0, kWaitFormat);
    count = Load(base, kHubCounter);
  }
  count += 1;
  const uint32_t loaded = Load(base, machine + kMachineLoaded);
  Store(base, kHubCounter, count);
  // The patched comparison: 1, not the original's 60 (the PATCH: in
  // generated/, Gliniak's Xenia patch for the black screen after quest
  // races).
  if (loaded != 0 && count >= 1) {
    const uint32_t reenter = Load(base, machine + kMachineReenter);
    Store(base, kHubCounter, 0);
    Store(base, machine + kMachineState, reenter != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t HubFreeImage(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218D650 Type2Hub::FreeImage");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t image = Load(base, record + 8);
  Store(base, record + 0, 0);
  calls.Call(kVirtualFree, frame, 0x8218D680, image, 0, kMemRelease);
  return calls.Call(kVirtualFree, frame, 0x8218D694, Load(base, record + 12), 0, kMemRelease);
}

void HubOpenSelected(uint8_t* base, uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8218E5B0 Type2Hub::OpenSelected");
  static constexpr uint32_t kMessages[7] = {8, 12, 18, 10, 23, 21, 14};
  const uint32_t item = Load(base, Load(base, kHubMenu) + 0);
  Post(base, mode, item < 7 ? kMessages[item] : 51);
  Store(base, machine + kMachineState, 9);
}

void HubReenter(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8218D9B8 Type2Hub::Reenter");
  if (Load(base, machine + kMachineLoaded) == 0) {
    Store(base, machine + kMachineState, 1);
    return;
  }
  const uint32_t transition = Load(base, Data(base) + 0) != 0 ? 4 : 0;
  PagesGo(base, Load(base, kHubPages), 0, transition, 1);
  Store(base, machine + kMachineState, 4);
}

void HubPostMessage5(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8218E7A4 Type2Hub::PostMessage5");
  Post(base, mode, 5);
}

void HubChoose(uint8_t* base, uint32_t mode, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8218E768 Type2Hub::Choose");
  const uint32_t item = Load(base, Load(base, kHubMenu) + 0);
  if (item > 7) {
    return;
  }
  if (item < 7) {
    HubPostMessage5(base, mode);
    return;
  }
  Store(base, machine + kMachineState, 10);
  Store(base, Data(base) + 4, 1);
}

uint64_t HubUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E530 Type2Hub::Unload");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, frame + 80, kHubMovie);
  Store(base, Data(base) + 20, 0);
  const uint32_t movie = Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8218E570, movie);
  PutBackImages(base, movie);
  if (Load(base, machine + kMachineDelete) != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8218E5A4, kHubMovie);
    Store(base, machine + kMachineLoaded, 0);
  }
  return r3;
}

uint64_t HubClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E688 Type2Hub::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x8218E6D8, movie);
  }
  PutBackImages(base, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8218E704, kHubMovie);
  Post(base, mode, 4);
  return r3;
}

uint32_t HubSetPrompts(uint8_t* base, const uint32_t args[6], uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218F530 Type2Hub::SetPrompts");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  // The pairs' first values at +88 (r3, r5, r7), their second at +104 (r4,
  // r6, r8), stored in the original's order.
  Store(base, frame + 92, args[2]);
  Store(base, frame + 104, args[1]);
  Store(base, frame + 108, args[3]);
  Store(base, frame + 88, args[0]);
  Store(base, frame + 96, args[4]);
  Store(base, frame + 112, args[5]);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t prompt = ui_movies::FindNode(base, movie, 10, i);
    const uint32_t first = ui_movies::FindNode(base, movie, 10, Load(base, prompt + 100));
    r3 = ui_movies::FindNode(base, movie, 10, Load(base, prompt + 100) + 1);
    const uint32_t a = Load(base, frame + 88 + 4 * i);
    if (a != kNone) {
      const uint32_t b = Load(base, frame + 104 + 4 * i);
      if (b != kNone) {
        Store(base, prompt + 80, 1);
        Store(base, first + 108, a);
        Store(base, r3 + 108, b);
        continue;
      }
    }
    Store(base, prompt + 80, 0);
  }
  return r3;
}

uint32_t HubBack(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218F698 Type2Hub::Back");
  const uint32_t pages = Load(base, kHubPages);
  const uint32_t before = Load(base, pages + 0);
  if (before != 0) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 12, kNone);
  Store(base, pages + 0, 0);
  Store(base, pages + 16, kNone);
  static constexpr uint32_t kPrompts[6] = {0, 0, 6, 2, 7, 3};
  return HubSetPrompts(base, kPrompts, sp);
}

uint64_t HubSetUpStatus(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E810 Type2Hub::SetUpStatus");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const int32_t day = int32_t(Load(base, save + 6100));
  calls.Call(kSetChildNameText, frame, 0x8218E860, movie, 8, 6, save + 8);
  calls.Call(kSetNumberText, frame, 0x8218E874, movie, 8, 9, Load(base, save + 6116));
  const uint64_t args[8] = {movie, 8, 13, 8, uint32_t(day > 999 ? 999 : day), 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8218E898, args);
  const uint32_t node = ui_movies::FindNode(base, movie, 8, 11);
  Store(base, node + 108, uint32_t(day % 7));
  const uint64_t r3 = type67_modes::GarageStageSetPlates(
      base, Load(base, Load(base, build_screen::kStandHolder)), 1);
  Store(base, Data(base) + 0, 1);
  Store(base, Data(base) + 4, 0);
  Store(base, Data(base) + 8, 0);
  return r3;
}

uint32_t HubNextNotice(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218DBD0 Type2Hub::NextNotice");
  Frame(base, sp, 96);
  const uint32_t data = Data(base);
  const uint32_t entry = Load(base, data + 436);
  if (entry != kNone) {
    const uint32_t next = Load(base, data_tables::Entry56(base, Load(base, data + 432), entry) + 8);
    if (next != 0) {
      const uint32_t now = Data(base);
      Store(base, now + 436, next + Load(base, now + 436));
      return 0;
    }
  }
  const uint32_t list = vehicle_accessors::SaveDataOf(base, 0) + kNotices;
  const uint32_t count = Load(base, list + 768);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t at = list + 12 * i;
    if (Load(base, at + 0) == 0) {
      Store(base, at + 0, 1);
      const uint32_t to = Data(base) + 428;
      Store(base, to + 0, Load(base, at + 0));
      Store(base, to + 4, Load(base, at + 4));
      Store(base, to + 8, Load(base, at + 8));
      return 1;
    }
  }
  Store(base, vehicle_accessors::SaveDataOf(base, 0) + kNoticeCount, 0);
  return 2;
}

uint32_t HubNextNotice2(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218DD78 Type2Hub::NextNotice2");
  Frame(base, sp, 96);
  const uint32_t list = vehicle_accessors::SaveDataOf(base, 0) + kNotices2;
  for (uint32_t i = 0; i < Load(base, list + kNotice2Count); i++) {
    const uint32_t at = list + 12 * i;
    if (Load(base, at + 0) == 0) {
      Store(base, at + 0, 1);
      const uint32_t to = Data(base) + 416;
      Store(base, to + 0, Load(base, at + 0));
      Store(base, to + 4, Load(base, at + 4));
      Store(base, to + 8, Load(base, at + 8));
      return 1;
    }
  }
  Store(base, list + kNotice2Count, 0);
  return 0;
}

uint32_t HubStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218F6E0 Type2Hub::Start");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  Store(base, Data(base) + 440, Load(base, Load(base, movie + 28) + 36));
  Store(base, Data(base) + 444, Load(base, Load(base, movie + 28) + 32));
  HubSetUpStatus(base, frame, calls);
  uint32_t r3 = HubNextNotice(base, frame);
  if (r3 != 2) {
    PagesGo(base, Load(base, kHubPages), 4, 5, 1);
  } else {
    r3 = HubNextNotice2(base, frame);
    PagesGo(base, Load(base, kHubPages), r3 != 0 ? 5 : 0, 0, 1);
  }
  Store(base, Data(base) + 20, 0);
  return r3;
}

}  // namespace type2_screens
