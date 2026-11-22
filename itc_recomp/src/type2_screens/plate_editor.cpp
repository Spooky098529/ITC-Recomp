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
  const schema::Type2ScreensMode ring(base, mode);
  const uint32_t count = ring.message_count() + 1;
  const uint32_t start = ring.message_start();
  ring.set_message_count(count);
  const schema::ModeMessage slot(base, ring.messages_address() + 12 * ((start + count - 1) % 50));
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

void LeaveToHub(uint8_t* base, uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821D6A20 Screen::LeaveToHub");
  Post(base, mode, 6);
  schema::StateMachine(base, machine).set_state(9);
}

uint64_t PlateCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198210 PlateEditor::Create");
  const uint32_t frame = Frame(base, sp, 112);
  schema::PlateEditor(base, machine).set_loaded(0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82198244, 0, 28, 4096, 4));
  Globals::set_plate_pages(base, pages);
  const schema::PageObject page_object(base, pages);
  page_object.set_next_page(0);
  page_object.set_page(0);
  page_object.set_previous_page(0);
  page_object.set_step_started(0);
  page_object.set_unknown_20(0xFFFFFFFFu);
  page_object.set_step(0xFFFFFFFFu);
  const uint32_t list = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82198280, 0, 72, 4096, 4));
  Globals::set_plate_list(base, list);
  if (list != 0) {
    MenuList(base, list).set_enabled(0u);
    MenuList(base, list).set_player(0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x821982AC, 0, 76, 4096, 4);
  const uint32_t data = uint32_t(r3);
  Globals::set_plate_data(base, data);
  const schema::PlateEditorData plate(base, data);
  plate.set_unknown_0(0);
  plate.set_next_state(21);
  schema::PlateEditor(base, machine).set_state(schema::PlateEditor(base, machine).reenter() != 0 ? 1 : 9);
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
  if (schema::PlateEditor(base, machine).loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kPlateMovie);
    if (uint32_t(r3) != 0) {
      schema::PlateEditor(base, machine).set_loaded(1);
    }
  }
  if (schema::PlateEditor(base, machine).loaded() != 0) {
    schema::PlateEditor(base, machine).set_state(schema::PlateEditor(base, machine).reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t PlateClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198498 PlateEditor::Close");
  const uint32_t frame = Frame(base, sp, 112);
  schema::PlateEditor(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  calls.Call(kFinishAll, frame, 0x821984D8, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x821984E0, kPlateMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t PlateDestroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198118 PlateEditor::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x82198140, Globals::plate_pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82198154, Globals::plate_list(base).address, 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x82198168, Globals::plate_data(base).address, 0, kMemRelease);
  Globals::set_plate_pages(base, 0);
  Globals::set_plate_list(base, 0);
  Globals::set_plate_data(base, 0);
  return r3;
}

void PlateOnMessage32(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82198190 PlateEditor::OnMessage32");
  PagesGo(base, Globals::plate_pages(base).address, 4, 4, 0xFFFFFFFFu);
}

void PlateLeavePage(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82198988 PlateEditor::LeavePage");
  Globals::plate_data(base).set_next_state(9);
  Post(base, mode, 31);
}

uint32_t PlateCharLineKind(uint8_t* base, uint32_t chars) {
  ITC_NATIVE_RUNNING("sub_821981C8 PlateEditor::CharLineKind");
  const uint32_t at = Globals::plate_data(base).edited();
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
    const uint32_t product =
        uint32_t(int32_t(schema::Type2SystemTime(base, frame + 80).milliseconds()) * int32_t(drawn));
    return uint8_t(product % count);
  };
  const schema::Type2Plate made(base, plate);
  made.set_region(next(0x82198378, 14));
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU8(base, made.digits_address() + i, next(0x821983A8, 10));
  }
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU8(base, made.chars_address() + i, next(0x821983E4, 38));
  }
  return plate;
}

uint64_t PlateLoad(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198420 PlateEditor::LoadPlate");
  const uint32_t frame = Frame(base, sp, 96);
  schema::PlateEditorData data = Globals::plate_data(base);
  if (data.make_random() != 0) {
    r3 = PlateRandom(base, data.start_plate_address(), frame, calls);
    data.address = Globals::plate_data(base).address;
  }
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, data.plate_address() + 4 * i, Load(base, data.start_plate_address() + 4 * i));
  }
  return r3;
}

uint64_t PlateResetMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821989E8 PlateEditor::ResetMovie");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  Globals::plate_data(base).set_region_rows(5);
  Globals::plate_data(base).set_char_rows(4);
  return calls.Call(kResetPose, frame, 0x82198A30, movie);
}

uint64_t PlateStart(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199BB8 PlateEditor::Start");
  const uint32_t frame = Frame(base, sp, 96);
  PlateLoad(base, r3, frame, calls);
  r3 = PlateResetMovie(base, frame, calls);
  PagesGo(base, Globals::plate_pages(base).address, 0, 0, 1);
  return r3;
}

uint32_t PlateShowRegion(uint8_t* base, uint32_t region, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199898 PlateEditor::ShowRegion");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t node = ui_movies::FindNode(base, movie, 2, 0);
  Node(base, node).set_texture_entry(region);
  return node;
}

uint32_t PlateShowDigits(uint8_t* base, uint32_t digits, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821998F0 PlateEditor::ShowDigits");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(base, movie, 2, 2);
  uint32_t r3 = parent;
  for (int32_t i = 0; i < int32_t(ChildCount(base, parent)); i++) {
    const uint32_t first = FirstChild(base, parent);
    const uint32_t digit = Load(base, digits + 4 * uint32_t(i));
    r3 = ui_movies::FindNode(base, movie, 2, first + uint32_t(i));
    if (digit != 10) {
      Node(base, r3).set_texture_entry(digit);
      Node(base, r3).set_visible(1);
    } else {
      Node(base, r3).set_visible(0);
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
    const uint32_t first = FirstChild(base, parent);
    const uint32_t c = Load(base, chars + 4 * i);
    r3 = ui_movies::FindNode(base, movie, 2, i + first);
    if (int32_t(c) < 37) {
      Node(base, r3).set_texture_entry(c);
      Node(base, r3).set_visible(1);
    } else {
      Node(base, r3).set_visible(0);
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
  for (int32_t i = 0; i < int32_t(ChildCount(base, parent)); i++) {
    r3 = ui_movies::FindNode(base, movie, 1, FirstChild(base, parent) + uint32_t(i));
    Node(base, r3).set_visible(i == int32_t(field) ? 1 : 0);
  }
  return r3;
}

uint64_t PlateFinishRegionAnim(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821992D8 PlateEditor::FinishRegionAnim");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t selection = race_screens::ListSelection(base, Globals::plate_list(base).address);
  const uint32_t named = Globals::plate_data(base).column() + selection * 3 + 2;
  return calls.Call(kFinishNamed, frame, 0x82199338, movie, named);
}

uint64_t PlateFinishDigitAnim(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199410 PlateEditor::FinishDigitAnim");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kPlateMovie, frame + 80);
  const uint32_t selection = race_screens::ListSelection(base, Globals::plate_list(base).address);
  const uint32_t named = Globals::plate_data(base).column() + (selection << 2) + 15;
  return calls.Call(kFinishNamed, frame, 0x82199470, movie, named);
}

}  // namespace type2_screens
