// The license plate editor's set-up, teardown and movie helpers
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "race_overlays/race_overlays.h"
#include "race_screens/race_screens.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"

namespace type2_screens {

namespace {
constexpr uint32_t kMemRelease = 0x8000;
}  // namespace

void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  Store(base, slot + 0, id);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}

void LeaveToHub(uint8_t* base, uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821D6A20 Screen::LeaveToHub");
  Post(base, mode, 6);
  Store(base, machine + kMachineState, 9);
}

uint64_t PlateCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198210 PlateEditor::Create");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82198244, 0, 28, 4096, 4));
  Store(base, kPlatePages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 24, 0);
  Store(base, pages + 20, 0xFFFFFFFFu);
  Store(base, pages + 12, 0xFFFFFFFFu);
  const uint32_t list = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82198280, 0, 72, 4096, 4));
  Store(base, kPlateList, list);
  if (list != 0) {
    Store(base, list + 68, 0);
    Store(base, list + 60, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x821982AC, 0, 76, 4096, 4);
  const uint32_t data = uint32_t(r3);
  Store(base, kPlateData, data);
  Store(base, data + 0, 0);
  Store(base, data + 72, 21);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t PlateOpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198180 PlateEditor::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kPlateMovie, 22556);
}

uint64_t PlateWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821982F0 PlateEditor::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  if (Load(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kPlateMovie);
    if (uint32_t(r3) != 0) {
      Store(base, machine + kMachineLoaded, 1);
    }
  }
  if (Load(base, machine + kMachineLoaded) != 0) {
    Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t PlateClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198498 PlateEditor::Close");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  calls.Call(kFinishAll, frame, 0x821984D8, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x821984E0, kPlateMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t PlateDestroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198118 PlateEditor::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x82198140, Load(base, kPlatePages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82198154, Load(base, kPlateList), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x82198168, Load(base, kPlateData), 0, kMemRelease);
  Store(base, kPlatePages, 0);
  Store(base, kPlateList, 0);
  Store(base, kPlateData, 0);
  return r3;
}

void PlateOnMessage32(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82198190 PlateEditor::OnMessage32");
  PagesGo(base, Load(base, kPlatePages), 4, 4, 0xFFFFFFFFu);
}

void PlateLeavePage(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82198988 PlateEditor::LeavePage");
  Store(base, Load(base, kPlateData) + 72, 9);
  Post(base, mode, 31);
}

uint32_t PlateCharLineKind(uint8_t* base, uint32_t chars) {
  ITC_NATIVE_RUNNING("sub_821981C8 PlateEditor::CharLineKind");
  const uint32_t at = Load(base, Load(base, kPlateData) + 56);
  if (at == 0) {
    return 0;
  }
  if (Load(base, chars + (at << 2) - 4) != 10) {
    return 1;
  }
  // cntlzw of at - 3, its bit 5 (32: at was 3) moved to bit 1.
  return at == 3 ? 2 : 0;
}

uint64_t PlateRandom(uint8_t* base, uint32_t plate, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198360 PlateEditor::RandomPlate");
  const uint32_t frame = Frame(base, sp, 128);
  // Each value: rand() x the milliseconds (the SYSTEMTIME's +14), the
  // product's low word modulo the count (unsigned).
  auto next = [&](uint32_t lr, uint32_t count) {
    calls.Call(kGetLocalTime, frame, lr, frame + 80);
    const uint32_t drawn = race_overlays::Rand(base);
    const uint32_t product = uint32_t(int32_t(guest::LoadU16(base, frame + 94)) * int32_t(drawn));
    return uint8_t(product % count);
  };
  guest::StoreU8(base, plate + 1, next(0x82198378, 14));
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU8(base, plate + 2 + i, next(0x821983A8, 10));
  }
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU8(base, plate + 5 + i, next(0x821983E4, 38));
  }
  return plate;
}

uint64_t PlateLoad(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198420 PlateEditor::LoadPlate");
  const uint32_t frame = Frame(base, sp, 96);
  uint32_t data = Load(base, kPlateData);
  if (Load(base, data + 48) != 0) {
    r3 = PlateRandom(base, data + 24, frame, calls);
    data = Load(base, kPlateData);
  }
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, data + 4 + 4 * i, Load(base, data + 24 + 4 * i));
  }
  return r3;
}

uint64_t PlateResetMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821989E8 PlateEditor::ResetMovie");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  Store(base, Load(base, kPlateData) + 64, 5);
  Store(base, Load(base, kPlateData) + 68, 4);
  return calls.Call(kResetPose, frame, 0x82198A30, movie);
}

uint64_t PlateStart(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199BB8 PlateEditor::Start");
  const uint32_t frame = Frame(base, sp, 96);
  PlateLoad(base, r3, frame, calls);
  r3 = PlateResetMovie(base, frame, calls);
  PagesGo(base, Load(base, kPlatePages), 0, 0, 1);
  return r3;
}

uint32_t PlateShowRegion(uint8_t* base, uint32_t region, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199898 PlateEditor::ShowRegion");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t node = ui_movies::FindNode(base, movie, 2, 0);
  Store(base, node + 108, region);
  return node;
}

uint32_t PlateShowDigits(uint8_t* base, uint32_t digits, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821998F0 PlateEditor::ShowDigits");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(base, movie, 2, 2);
  uint32_t r3 = parent;
  for (int32_t i = 0; i < int32_t(Load(base, parent + 104)); i++) {
    const uint32_t first = Load(base, parent + 100);
    const uint32_t digit = Load(base, digits + 4 * uint32_t(i));
    r3 = ui_movies::FindNode(base, movie, 2, first + uint32_t(i));
    if (digit != 10) {
      Store(base, r3 + 108, digit);
      Store(base, r3 + 80, 1);
    } else {
      Store(base, r3 + 80, 0);
    }
  }
  return r3;
}

uint32_t PlateShowChars(uint8_t* base, uint32_t chars, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199990 PlateEditor::ShowChars");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(base, movie, 2, 1);
  uint32_t r3 = parent;
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t first = Load(base, parent + 100);
    const uint32_t c = Load(base, chars + 4 * i);
    r3 = ui_movies::FindNode(base, movie, 2, i + first);
    if (int32_t(c) < 37) {
      Store(base, r3 + 108, c);
      Store(base, r3 + 80, 1);
    } else {
      Store(base, r3 + 80, 0);
    }
  }
  return r3;
}

uint32_t PlateMarkField(uint8_t* base, uint32_t field, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199AA8 PlateEditor::MarkField");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(base, movie, 1, 0);
  uint32_t r3 = parent;
  for (int32_t i = 0; i < int32_t(Load(base, parent + 104)); i++) {
    r3 = ui_movies::FindNode(base, movie, 1, Load(base, parent + 100) + uint32_t(i));
    Store(base, r3 + 80, i == int32_t(field) ? 1 : 0);
  }
  return r3;
}

uint64_t PlateFinishRegionAnim(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821992D8 PlateEditor::FinishRegionAnim");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t selection = race_screens::ListSelection(base, Load(base, kPlateList));
  const uint32_t named = Load(base, Load(base, kPlateData) + 52) + selection * 3 + 2;
  return calls.Call(kFinishNamed, frame, 0x82199338, movie, named);
}

uint64_t PlateFinishDigitAnim(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199410 PlateEditor::FinishDigitAnim");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t selection = race_screens::ListSelection(base, Load(base, kPlateList));
  const uint32_t named = Load(base, Load(base, kPlateData) + 52) + (selection << 2) + 15;
  return calls.Call(kFinishNamed, frame, 0x82199470, movie, named);
}

}  // namespace type2_screens
