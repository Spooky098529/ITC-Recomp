// The license plate editor's set-up, teardown and movie helpers
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "race_overlays/race_overlays.h"
#include "race_screens/race_screens.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"

namespace type2_screens {

namespace {
constexpr uint32_t kMemRelease = 0x8000;
}  // namespace

void Post(uint32_t mode, uint32_t id) {
  const schema::Type2ScreensMode ring(mode);
  const uint32_t count = ring.message_count() + 1;
  const uint32_t start = ring.message_start();
  ring.set_message_count(count);
  const game_modes::RingSlot slot{ring.messages() + 12 * ((start + count - 1) % 50)};
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

void LeaveToHub(uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821D6A20 Screen::LeaveToHub");
  Post(mode, 6);
  schema::StateMachine(machine).set_state(9);
}

uint64_t PlateCreate(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198210 PlateEditor::Create");
  const uint32_t frame = Frame(sp, 112);
  schema::PlateEditor(machine).set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x82198244
  // and the VirtualAlloc(72) at 0x82198280; the globals hold their handles.
  const uint32_t pages = schema::PageObject::New().address;
  Globals::set_plate_pages(pages);
  const schema::PageObject page_object(pages);
  page_object.set_next_page(0);
  page_object.set_page(0);
  page_object.set_previous_page(0);
  page_object.set_step_started(0);
  page_object.set_unknown_20(0xFFFFFFFFu);
  page_object.set_step(0xFFFFFFFFu);
  const uint32_t list = schema::MenuList::New().address;
  Globals::set_plate_list(list);
  if (list != 0) {
    MenuList(list).set_enabled(0u);
    MenuList(list).set_player(0);
  }
  // A host object (E5 step 4) in place of the VirtualAlloc(76) at
  // 0x821982AC; [0x826D8438] holds its handle (and r3, as the original's).
  const schema::PlateEditorData plate = schema::PlateEditorData::New();
  const uint64_t r3 = plate.address;
  Globals::set_plate_data(plate.address);
  plate.set_unknown_0(0);
  plate.set_next_state(21);
  schema::PlateEditor(machine).set_state(schema::PlateEditor(machine).reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t PlateOpenMovie(uint8_t*, uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198180 PlateEditor::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kPlateMovie, 22556);
}

uint64_t PlateWaitForLoad(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821982F0 PlateEditor::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  Frame(sp, 96);
  if (schema::PlateEditor(machine).loaded() == 0) {
    r3 = ui_movies::IsLoaded(kPlateMovie);
    if (uint32_t(r3) != 0) {
      schema::PlateEditor(machine).set_loaded(1);
    }
  }
  if (schema::PlateEditor(machine).loaded() != 0) {
    schema::PlateEditor(machine).set_state(schema::PlateEditor(machine).reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t PlateClose(uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198498 PlateEditor::Close");
  const uint32_t frame = Frame(sp, 112);
  schema::PlateEditor(machine).set_state(7);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  calls.Call(kFinishAll, frame, 0x821984D8, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x821984E0, kPlateMovie);
  Post(mode, 4);
  return r3;
}

uint64_t PlateDestroy(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198118 PlateEditor::Destroy");
  const uint32_t frame = Frame(sp, 112);
  calls.Call(kVirtualFree, frame, 0x82198140, Globals::plate_pages().address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x82198154, Globals::plate_list().address, 0, kMemRelease);
  // Deleted in place of the VirtualFree at 0x82198168 (r3 = 1, as it
  // returned; with no data, the VirtualFree of 0 as the original).
  const schema::PlateEditorData data = Globals::plate_data();
  const uint64_t r3 =
      data.address != 0 ? 1 : calls.Call(kVirtualFree, frame, 0x82198168, 0, 0, kMemRelease);
  data.Delete();
  Globals::set_plate_pages(0);
  Globals::set_plate_list(0);
  Globals::set_plate_data(0);
  return r3;
}

void PlateOnMessage32() {
  ITC_NATIVE_RUNNING("sub_82198190 PlateEditor::OnMessage32");
  PagesGo(Globals::plate_pages().address, 4, 4, 0xFFFFFFFFu);
}

void PlateLeavePage(uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82198988 PlateEditor::LeavePage");
  Globals::plate_data().set_next_state(9);
  Post(mode, 31);
}

uint32_t PlateCharLineKind(uint32_t chars) {
  ITC_NATIVE_RUNNING("sub_821981C8 PlateEditor::CharLineKind");
  const uint32_t at = Globals::plate_data().edited();
  if (at == 0) {
    return 0;
  }
  if (Load(chars + (at << 2) - 4) != 10) {
    return 1;
  }
  // cntlzw of at - 3, its bit 5 (32: at was 3) moved to bit 1.
  return at == 3 ? 2 : 0;
}

uint64_t PlateRandom(uint32_t plate, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198360 PlateEditor::RandomPlate");
  const uint32_t frame = Frame(sp, 128);
  // Each value: rand() x the milliseconds (the SYSTEMTIME's +14), the
  // product's low word modulo the count (unsigned).
  auto next = [&](uint32_t lr, uint32_t count) {
    calls.Call(kGetLocalTime, frame, lr, frame + 80);
    const uint32_t drawn = race_overlays::Rand();
    const uint32_t product =
        uint32_t(int32_t(schema::Type2SystemTime(frame + 80).milliseconds()) * int32_t(drawn));
    return uint8_t(product % count);
  };
  const schema::Type2Plate made(plate);
  made.set_region(next(0x82198378, 14));
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU8(made.digits_address() + i, next(0x821983A8, 10));
  }
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU8(made.chars_address() + i, next(0x821983E4, 38));
  }
  return plate;
}

uint64_t PlateLoad(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198420 PlateEditor::LoadPlate");
  const uint32_t frame = Frame(sp, 96);
  schema::PlateEditorData data = Globals::plate_data();
  if (data.make_random() != 0) {
    r3 = PlateRandom(data.start_plate_address(), frame, calls);
    data.address = Globals::plate_data().address;
  }
  for (uint32_t i = 0; i < 5; i++) {
    // Words of the host data (E5 step 4) as the guest's bytes give them.
    const uint32_t from = data.start_plate_address() - data.address + 4 * i;
    data.store_at(data.plate_address() - data.address + 4 * i, 4, data.load_at(from, 4));
  }
  return r3;
}

uint64_t PlateResetMovie(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821989E8 PlateEditor::ResetMovie");
  const uint32_t frame = Frame(sp, 96);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  Globals::plate_data().set_region_rows(5);
  Globals::plate_data().set_char_rows(4);
  return calls.Call(kResetPose, frame, 0x82198A30, movie);
}

uint64_t PlateStart(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199BB8 PlateEditor::Start");
  const uint32_t frame = Frame(sp, 96);
  PlateLoad(r3, frame, calls);
  r3 = PlateResetMovie(frame, calls);
  PagesGo(Globals::plate_pages().address, 0, 0, 1);
  return r3;
}

uint32_t PlateShowRegion(uint32_t region, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199898 PlateEditor::ShowRegion");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  const uint32_t node = ui_movies::FindNode(movie, 2, 0);
  Node(node).set_texture_entry(region);
  return node;
}

uint32_t PlateShowDigits(uint32_t digits, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821998F0 PlateEditor::ShowDigits");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(movie, 2, 2);
  uint32_t r3 = parent;
  for (int32_t i = 0; i < int32_t(ChildCount(parent)); i++) {
    const uint32_t first = FirstChild(parent);
    const uint32_t digit = Load(digits + 4 * uint32_t(i));
    r3 = ui_movies::FindNode(movie, 2, first + uint32_t(i));
    if (digit != 10) {
      Node(r3).set_texture_entry(digit);
      Node(r3).set_visible(1);
    } else {
      Node(r3).set_visible(0);
    }
  }
  return r3;
}

uint32_t PlateShowChars(uint32_t chars, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199990 PlateEditor::ShowChars");
  const uint32_t frame = Frame(sp, 144);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(movie, 2, 1);
  uint32_t r3 = parent;
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t first = FirstChild(parent);
    const uint32_t c = Load(chars + 4 * i);
    r3 = ui_movies::FindNode(movie, 2, i + first);
    if (int32_t(c) < 37) {
      Node(r3).set_texture_entry(c);
      Node(r3).set_visible(1);
    } else {
      Node(r3).set_visible(0);
    }
  }
  return r3;
}

uint32_t PlateMarkField(uint32_t field, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82199AA8 PlateEditor::MarkField");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  const uint32_t parent = ui_movies::FindNode(movie, 1, 0);
  uint32_t r3 = parent;
  for (int32_t i = 0; i < int32_t(ChildCount(parent)); i++) {
    r3 = ui_movies::FindNode(movie, 1, FirstChild(parent) + uint32_t(i));
    Node(r3).set_visible(i == int32_t(field) ? 1 : 0);
  }
  return r3;
}

uint64_t PlateFinishRegionAnim(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821992D8 PlateEditor::FinishRegionAnim");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  const uint32_t selection = race_screens::ListSelection(Globals::plate_list().address);
  const uint32_t named = Globals::plate_data().column() + selection * 3 + 2;
  return calls.Call(kFinishNamed, frame, 0x82199338, movie, named);
}

uint64_t PlateFinishDigitAnim(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82199410 PlateEditor::FinishDigitAnim");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = FindMovie(kPlateMovie, frame + 80);
  const uint32_t selection = race_screens::ListSelection(Globals::plate_list().address);
  const uint32_t named = Globals::plate_data().column() + (selection << 2) + 15;
  return calls.Call(kFinishNamed, frame, 0x82199470, movie, named);
}

}  // namespace type2_screens
