// The type 6 and 7 modes' handlers and helpers that make no guest calls
// (docs/functions/type6-type7-modes.md): the screens' and dialogs' leaves,
// their blocks' allocation and freeing over the allocation layer, the
// garage camera's presets and the controller pages' blocks. The originals'
// loads and stores in their order; float words go through float registers
// (lfs, stfs) as the originals move them.

#include "type67_modes/type67_modes.h"
#include "type67_modes/views.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <initializer_list>

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type6Type7ModesGlobals;

// A float word through a float register and back (lfs, stfs).
uint32_t ThroughFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<uint32_t>(float(double(guest::LoadFloat(base, address))));
}

// VirtualAlloc(0, size, MEM_COMMIT, read-write) over the allocation layer.
uint32_t Pages(uint32_t size) { return memory::AllocPages(size, 4096); }

// VirtualFree(block, 0, MEM_RELEASE)'s r3 for the layer's pages.
uint64_t Free(uint32_t block) { return memory::FreePages(block) ? 1 : 0; }

bool AllPages(std::initializer_list<uint32_t> blocks) {
  for (const uint32_t block : blocks) {
    if (!memory::IsPages(block)) {
      return false;
    }
  }
  return true;
}

// The constants the poses and presets are built from.
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kPoseY = 0x82009EAC;

// The vectors a pose or preset is built from, below `sp` (-48 to -1), as
// the originals' leaves store them: (z, z, z, w) twice, then (z, `y`, z, w).
void ScratchVectors(uint8_t* base, uint32_t sp, uint32_t z, uint32_t w, uint32_t y) {
  const uint32_t words[12] = {z, z, z, w, z, z, z, w, z, y, z, w};
  for (uint32_t i = 0; i < 12; i++) {
    StoreU32(base, sp - 48 + 4 * i, words[i]);
  }
}

void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    StoreU32(base, to + 4 * i, LoadU32(base, from + 4 * i));
  }
}

// A message (three words: `message`, 0, 0) queued in a dispatcher's ring
// (+204: 50 slots of 12 bytes, the count at +600, the start at +604), with
// no overflow check, as the handlers do it inline.
uint32_t RingSlot(uint8_t* base, uint32_t dispatcher) {
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  return ring + (end - (end / 50) * 50) * 12;
}

constexpr uint32_t kMinusOne = 0xFFFFFFFF;

}  // namespace

// --- The garage stage and camera ---

uint64_t GarageStageSetPlates(uint8_t* base, uint64_t r3, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82178DE8 GarageStage::SetPlates");
  StoreU32(base, LoadU32(base, uint32_t(r3)) + 6092, value);
  return r3;
}

uint64_t GarageStageResetPose(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8217A2D8 GarageStage::ResetPose");
  const uint32_t pose = uint32_t(r3);
  const uint32_t zero = ThroughFloat(base, kZero);
  const uint32_t one = ThroughFloat(base, kOne);
  const uint32_t y = ThroughFloat(base, kPoseY);
  ScratchVectors(base, sp, zero, one, y);
  StoreU32(base, pose + 0, zero);
  CopyWords(base, pose + 16, sp - 48, 12);
  StoreU32(base, pose + 64, 0);
  return r3;
}

uint64_t CameraPresetConstruct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8217BE08 CameraPreset::CameraPreset");
  const schema::CameraPreset preset(base, uint32_t(r3));
  const uint32_t zero = ThroughFloat(base, kZero);
  const uint32_t one = ThroughFloat(base, kOne);
  ScratchVectors(base, sp, zero, one, zero);
  StoreU32(base, preset.elevation_address(), zero);
  StoreU32(base, preset.value_8_address(), zero);
  StoreU32(base, preset.value_12_address(), zero);
  // The three vectors (+16, +32, +48), twelve words.
  CopyWords(base, preset.vector_16_address(), sp - 48, 12);
  return r3;
}

uint64_t CameraPresetCopy(uint8_t* base, uint32_t source, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8217D7D0 CameraPreset::Copy");
  const uint32_t copy = Pages(64);
  CameraPresetConstruct(base, copy, sp - 96);
  for (uint32_t offset = 4; offset <= 60; offset += 4) {
    StoreU32(base, copy + offset, ThroughFloat(base, source + offset));
  }
  return copy;
}

namespace {

bool PresetsArePages(uint8_t* base, uint32_t address) {
  const schema::GarageCamera camera(base, address);
  const uint32_t count = camera.preset_count();
  for (uint32_t i = 0; i < count; i++) {
    if (!memory::IsPages(LoadU32(base, camera.presets_address() + 4 * i))) {
      return false;
    }
  }
  return true;
}

uint64_t FreePresets(uint8_t* base, uint64_t r3) {
  const schema::GarageCamera camera(base, uint32_t(r3));
  while (camera.preset_count() != 0) {
    const uint32_t count = camera.preset_count() - 1;
    camera.set_preset_count(count);
    r3 = Free(LoadU32(base, camera.presets_address() + 4 * count));
  }
  return r3;
}

}  // namespace

std::optional<uint64_t> GarageCameraFreePresets(uint8_t* base, uint64_t r3) {
  if (!PresetsArePages(base, uint32_t(r3))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8217D760 GarageCamera::FreePresets");
  return FreePresets(base, r3);
}

std::optional<uint64_t> GarageCameraOnPresetsLoaded(uint8_t* base, uint32_t data, uint32_t camera,
                                                    uint32_t sp) {
  if (!memory::IsPages(data)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8217FBE8 GarageCamera::OnPresetsLoaded");
  const schema::GaragePresetsData loaded(base, data);
  const schema::GarageCamera cam(base, camera);
  uint32_t count = loaded.count();
  uint32_t record = loaded.records_offset() + data;
  while (count != 0) {
    const uint32_t copy = uint32_t(CameraPresetCopy(base, record, sp - 128));
    const uint32_t presets = cam.preset_count();
    count--;
    record += schema::CameraPreset::kSize;
    cam.set_preset_count(presets + 1);
    StoreU32(base, cam.presets_address() + 4 * presets, copy);
  }
  cam.set_set_up(1);
  return Free(data);
}

std::optional<uint64_t> StandSlotFree(uint8_t* base, uint64_t r3) {
  const schema::StandSlot slot(base, uint32_t(r3));
  const uint32_t block = slot.stage<schema::View>().address;
  if (block == 0) {
    ITC_NATIVE_RUNNING("sub_821800B0 StandSlot::Free");
    return r3;
  }
  if (!memory::IsPages(block) || !PresetsArePages(base, block + 112)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821800B0 StandSlot::Free");
  FreePresets(base, block + 112);
  const uint64_t freed = Free(slot.stage<schema::View>().address);
  slot.set_stage(0u);
  return freed;
}

// --- The controller pages ---

uint64_t ControllerOptionsOnEnter(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_821BB120 ControllerOptions::OnEnter");
  schema::ControllerOptions(base, machine).set_loaded(1);
  const schema::FrontEndMenu menu(base, Pages(68));
  G::set_controller_menu(base, menu);
  menu.set_first_item(0);
  menu.set_last_item(6);
  menu.set_wraps(0);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
  G::controller_menu(base).set_player(1);
  const schema::PageObject page(base, Pages(28));
  G::set_controller_page(base, page);
  page.set_next_page(0);
  page.set_page(0);
  page.set_previous_page(0);
  page.set_step_started(0);
  page.set_unknown_20(kMinusOne);
  page.set_step(kMinusOne);
  return page.address;
}

std::optional<uint64_t> ControllerOptionsOnLeft(uint8_t* base) {
  if (!AllPages({G::controller_menu(base).address, G::controller_page(base).address})) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821BB1F8 ControllerOptions::OnLeft");
  Free(G::controller_menu(base).address);
  const uint64_t r3 = Free(G::controller_page(base).address);
  G::set_controller_menu(base, 0u);
  G::set_controller_page(base, 0u);
  return r3;
}

uint64_t ControllerOptionsShow(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BB260 ControllerOptions::Show");
  const schema::PageObject page = G::controller_page(base);
  const uint32_t current = page.page();
  if (current != kMinusOne) {
    page.set_previous_page(current);
  }
  page.set_page(kMinusOne);
  page.set_step(kMinusOne);
  page.set_queued_step(kMinusOne);
  return r3;
}

uint64_t ControllerOptionsOpen(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BC660 ControllerOptions::Open");
  schema::ControllerOptions(base, uint32_t(r3)).set_state(4);
  return r3;
}

uint64_t ControllerOptionsLeave(uint8_t* base, uint64_t r3, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_821BFB48 ControllerOptions::Leave");
  schema::ControllerOptions(base, uint32_t(r3)).set_state(7);
  uint32_t message;
  switch (int32_t(G::mode_type(base))) {
    case 2:
    case 4:
    case 6:
    case 8:
      message = 4;
      break;
    case 7:
      message = 3;
      break;
    default:
      return r3;
  }
  const uint32_t slot = RingSlot(base, dispatcher);
  StoreU32(base, slot + 8, 0);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 0, message);
  return r3;
}

uint64_t ControllerOptionsStepSliders(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821BB290 ControllerOptions::StepSliders");
  const schema::FrontEndMenu menu = G::controller_menu(base);
  const int32_t side = menu.side();
  const uint32_t cursor = menu.cursor();
  if ((side != -1 && side != 1) || cursor >= 3) {
    return 0;
  }
  // The sliders (-10 to 10) at +272 to +280: no change at a bound.
  const uint32_t settings = G::controller_settings<schema::View>(base).address;
  const uint32_t field = settings + 272 + 4 * cursor;
  const int32_t value = int32_t(LoadU32(base, field));
  if (side == -1 ? value <= -10 : value >= 10) {
    return 0;
  }
  StoreU32(base, field, uint32_t(value + side));
  return 1;
}

uint64_t ControllerOptionsStepSide(uint8_t* base, uint32_t offset, uint32_t last) {
  if (offset == 672) {
    ITC_NATIVE_RUNNING("sub_821BB3B8 ControllerOptions::StepButtons");
  } else {
    ITC_NATIVE_RUNNING("sub_821BB590 ControllerOptions::StepAnalog");
  }
  const schema::FrontEndMenu menu = G::controller_menu(base);
  const uint32_t side = uint32_t(menu.side());
  const uint32_t cursor = menu.cursor();
  StoreU32(base, G::controller_settings<schema::View>(base).address + offset, 0);
  if (side != kMinusOne && side != 1) {
    return 0;
  }
  StoreU32(base, G::controller_settings<schema::View>(base).address + offset, side);
  return cursor == last ? 0 : 1;
}

uint64_t ControllerOptionsSetMarkY(uint8_t* base, uint64_t y, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821C0238 ControllerOptions::SetMarkY");
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);
  StoreU32(base, frame + 80, 7);
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint32_t node = ui_movies::FindNode(base, movie, 8, 5);
  const uint32_t child = ui_movies::Child(base, ui_movies::Child(base, node, 0), 0);
  // The int's doubleword stored over the key for the conversion (std, lfd,
  // fcfid, frsp).
  const int64_t value = int32_t(uint32_t(y));
  StoreU32(base, frame + 80, uint32_t(uint64_t(value) >> 32));
  StoreU32(base, frame + 84, uint32_t(uint64_t(value)));
  guest::StoreFloat(base, child + 16, float(double(value)));
  return child;
}

// --- The screens' and dialogs' leaves ---

uint64_t PanelScreenInitWords(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82230740 PanelScreen::InitWords");
  StoreU32(base, uint32_t(r3) + 11672, 4);
  StoreU32(base, uint32_t(r3) + 11680, 9);
  return r3;
}

uint64_t ScreenMachineWait(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82277068 ScreenMachine::Wait");
  schema::ScreenMachine(base, uint32_t(r3)).set_state(9);
  return r3;
}

uint64_t ScreenWait(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822508F0 Screen::Wait");
  schema::Type67OffsetScreen(base, uint32_t(r3)).set_state(9);
  return r3;
}

uint64_t DialogQueueReopen(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822419D0 Type7Dialog::QueueReopen");
  schema::Type67Dialog(base, uint32_t(r3)).set_reopen(1);
  return r3;
}

uint64_t ScreenReenter(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8223B740 Screen::Reenter");
  const schema::Type67OffsetScreen screen(base, uint32_t(r3));
  screen.set_state(int32_t(screen.loaded()) != 0 ? 4 : 1);
  return r3;
}

uint64_t DialogShown(uint8_t* base, uint64_t r3, uint32_t shown) {
  if (shown == 9756) {
    ITC_NATIVE_RUNNING("sub_82231F48 Type6Dialog::Shown");
  } else {
    ITC_NATIVE_RUNNING("sub_822419C0 Type7Dialog::Shown");
  }
  SetDialogStatus(schema::Type67Dialog(base, uint32_t(r3)), shown != 9756, 0);
  return r3;
}

uint64_t DialogOnEnter(uint8_t* base, uint64_t r3, uint32_t shown) {
  if (shown == 9756) {
    ITC_NATIVE_RUNNING("sub_82231F98 Type6Dialog::OnEnter");
  } else {
    ITC_NATIVE_RUNNING("sub_82241A08 Type7Dialog::OnEnter");
  }
  const schema::Type67Dialog dialog(base, uint32_t(r3));
  const bool type7 = shown != 9756;
  dialog.set_loaded(0);
  dialog.set_next_page(0);
  dialog.set_page(0);
  dialog.set_previous_page(0);
  dialog.set_page_word_20(kMinusOne);
  dialog.set_step(kMinusOne);
  dialog.set_step_started(0);
  dialog.set_is_notice(0);
  dialog.set_menu_first_item(0);
  dialog.set_menu_last_item(1);
  dialog.set_menu_wraps(1);
  dialog.set_menu_cursor(0);
  dialog.set_menu_previous(0);
  dialog.set_menu_moved(0);
  dialog.set_menu_wrapped(0);
  dialog.set_menu_visible_row(0);
  dialog.set_menu_previous_row(0);
  dialog.set_menu_side(0);
  dialog.set_menu_player(0);
  dialog.set_menu_up_down_locked(0);
  dialog.set_menu_left_right_steps(0);
  dialog.set_menu_shoulders_step(0);
  dialog.set_unknown_9736(0);
  dialog.set_queued_state(21);
  dialog.set_movie(0);
  const uint32_t open = dialog.reenter();
  dialog.set_reopen(0);
  if (int32_t(open) != 0) {
    dialog.set_state(1);
    SetDialogStatus(dialog, type7, kMinusOne);
  } else {
    dialog.set_state(9);
    SetDialogStatus(dialog, type7, 3);
  }
  return r3;
}

uint64_t DialogWaitForMovies(uint8_t* base, uint64_t r3, uint32_t shown) {
  if (shown == 9756) {
    ITC_NATIVE_RUNNING("sub_82232910 Type6Dialog::WaitForMovies");
  } else {
    ITC_NATIVE_RUNNING("sub_82242388 Type7Dialog::WaitForMovies");
  }
  const schema::Type67Dialog dialog(base, uint32_t(r3));
  if (int32_t(dialog.loaded()) != 0) {
    dialog.set_unknown_9736(1);
    dialog.set_state(9);
    SetDialogStatus(dialog, shown != 9756, 3);
    return r3;
  }
  for (uint32_t id = 16; id <= 19; id++) {
    r3 = ui_movies::IsLoaded(base, id);
    if (int32_t(r3) == 0) {
      return r3;
    }
  }
  dialog.set_loaded(1);
  return r3;
}

uint64_t Screen0OnEnter(uint8_t* base, uint32_t screen, uint32_t d, uint32_t items) {
  if (d == 0) {
    ITC_NATIVE_RUNNING("sub_822339F0 Type6Screen0::OnEnter");
  } else {
    ITC_NATIVE_RUNNING("sub_82243F58 Type7Screen0::OnEnter");
  }
  const Screen0View s(base, screen, d != 0);
  s.set_words(Pages(76));
  s.set_pages(Pages(28));
  const uint32_t list = Pages(68);
  const schema::PageObject page = s.pages();
  s.set_menu(list);
  if (page) {
    page.set_next_page(0);
    page.set_page(0);
    page.set_previous_page(0);
    page.set_unknown_20(kMinusOne);
    page.set_step(kMinusOne);
    page.set_step_started(0);
  }
  const schema::Type67Screen0Words words = s.words();
  words.set_pick(kMinusOne);
  words.set_unknown_68(0);
  words.set_loading_shown(0);
  const schema::FrontEndMenu l = s.menu();
  s.set_item_picked(0);
  l.set_first_item(0);
  l.set_last_item(items);
  l.set_wraps(1);
  l.set_cursor(0);
  l.set_previous(0);
  l.set_moved(0);
  l.set_wrapped(0);
  l.set_list_wrapped(0);
  l.set_visible_row(0);
  l.set_previous_row(0);
  l.set_first_row(0);
  l.set_rows_shown(items + 1);
  l.set_side(0);
  l.set_player(0);
  l.set_up_down_locked(0);
  l.set_left_right_steps(0);
  l.set_shoulders_step(0);
  s.set_state(int32_t(s.reenter()) != 0 ? 1 : 9);
  return list;
}

std::optional<uint64_t> Screen0OnLeft(uint8_t* base, uint32_t screen, uint32_t d) {
  const Screen0View s(base, screen, d != 0);
  if (!AllPages({s.words().address, s.pages().address, s.menu().address})) {
    return std::nullopt;
  }
  if (d == 0) {
    ITC_NATIVE_RUNNING("sub_82233870 Type6Screen0::OnLeft");
  } else {
    ITC_NATIVE_RUNNING("sub_82243E08 Type7Screen0::OnLeft");
  }
  Free(s.words().address);
  Free(s.pages().address);
  const uint64_t r3 = Free(s.menu().address);
  s.set_words(0);
  s.set_pages(0);
  s.set_menu(0);
  return r3;
}

uint64_t Screen0WaitForMovie(uint8_t* base, uint64_t r3, uint32_t d) {
  if (d == 0) {
    ITC_NATIVE_RUNNING("sub_82233B18 Type6Screen0::WaitForMovie");
  } else {
    ITC_NATIVE_RUNNING("sub_82244080 Type7Screen0::WaitForMovie");
  }
  const Screen0View s(base, uint32_t(r3), d != 0);
  if (int32_t(s.loaded()) != 0) {
    s.set_state(9);
    return r3;
  }
  const uint32_t loaded = ui_movies::IsLoaded(base, 22);
  if (int32_t(loaded) != 0) {
    s.set_loaded(1);
  }
  return loaded;
}

uint64_t Screen0Pick(uint8_t* base, uint64_t r3, uint32_t menu, uint32_t pick) {
  if (menu == 9644) {
    ITC_NATIVE_RUNNING("sub_82243F28 Type7Screen0::PickYes");
  } else if (pick == 1) {
    ITC_NATIVE_RUNNING("sub_82233990 Type6Screen0::PickYes");
  } else {
    ITC_NATIVE_RUNNING("sub_822339A0 Type6Screen0::PickNo");
  }
  Screen0View(base, uint32_t(r3), menu == 9644).words().set_pick(pick);
  return r3;
}

uint64_t Type6Screen0GoOn(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822339C0 Type6Screen0::GoOn");
  const schema::PageObject page = schema::Type6Screen0(base, uint32_t(r3)).pages();
  const uint32_t current = page.page();
  if (current != 3) {
    page.set_previous_page(current);
  }
  page.set_step(3);
  page.set_queued_step(kMinusOne);
  page.set_next_page(3);
  page.set_page(kMinusOne);
  return r3;
}

}  // namespace type67_modes
