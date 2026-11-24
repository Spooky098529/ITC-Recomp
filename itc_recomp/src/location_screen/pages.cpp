// The location screen's state: its reset, the page object's steps and the
// page update (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "guest.h"
#include "schema/location_screen.h"
#include "schema/menu_helpers.h"

#include <array>
#include <bit>

namespace location_screen {

namespace {

using G = schema::LocationScreenGlobals;

schema::LocationScreen Screen(uint8_t* base) { return G::screen(base); }

// A float word through a float register and back (lfs, stfs).
uint32_t ThroughFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<uint32_t>(float(double(guest::LoadFloat(base, address))));
}

void StoreIdentity(uint8_t* base, uint32_t matrix) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, matrix + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
}

// The pages' input handlers (Page0 to Page15; page 10 the empty
// sub_82106540).
constexpr std::array<uint32_t, 16> kPageHandlers = {
    0x8222CA48, 0x82228A90, 0x82229798, 0x82228B80, 0x82229888, 0x82228CC0,
    0x82229A48, 0x82229B38, 0x82228068, 0x822280D8, 0x82106540, 0x82228138,
    0x82229CF0, 0x82229E98, 0x82228E18, 0x82229B98};

}  // namespace

void Reenter(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_822895E0 ScreenMachine::Reenter");
  const schema::LocationScreenMachine m(base, machine);
  m.set_state(m.loaded() != 0 ? 4 : 1);
}

bool ResetNative(uint32_t screen) {
  return screen < 0xA0000000u - 800;
}

void ResetView(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82227818 LocationScreen::ResetView");
  const schema::LocationScreenView v(base, view);
  const uint32_t zero = ThroughFloat(base, 0x82000A34);
  const uint32_t one = ThroughFloat(base, 0x82000A30);
  v.set_unknown_0(0);
  v.set_unknown_4(0);
  v.set_unknown_12(0);
  v.set_unknown_16(1);
  v.set_unknown_8(1);
  // The four vectors (camera_vector_1, camera_vector_0, vector_64, vector_80).
  for (uint32_t at : {v.camera_vector_1_address(), v.camera_vector_0_address(),
                      v.vector_64_address(), v.vector_80_address()}) {
    guest::StoreU32(base, at + 0, zero);
    guest::StoreU32(base, at + 4, zero);
    guest::StoreU32(base, at + 8, zero);
    guest::StoreU32(base, at + 12, one);
  }
  // The floats' bits as lfs and stfs leave them.
  for (uint32_t at : {v.unknown_96_address(), v.unknown_100_address(),
                      v.camera_float_0_address(), v.camera_float_1_address()}) {
    guest::StoreU32(base, at, zero);
  }
  StoreIdentity(base, v.matrix0_address());
  StoreIdentity(base, v.matrix1_address());
}

void Reset(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_822279A8 LocationScreen::Reset");
  const schema::LocationScreen s(base, screen);
  s.set_unknown_0(0);
  s.set_scenery(0u);
  s.set_picked_opponent(0);
  s.set_teardown_started(0);
  s.set_teardown_countdown(0);
  s.set_location(14);
  ResetView(base, s.view_address());
  s.set_picture_count_a(0);
  s.set_picture_count_b(0);
  s.set_default_texture_a(0);
  s.set_default_texture_b(0);
  s.set_opponent_count(0);
  s.set_group_kind(4);
  s.set_location_index(0);
  s.set_unlock_position(0);
  s.set_entry_offset(0);
  s.set_entry_table(0);
  s.set_list_count(0);
  s.set_queued_page(0xFFFFFFFFu);
  s.set_queued_transition(0xFFFFFFFFu);
  s.set_item2_taken(0);
  s.set_won_card_item(0);
  s.set_more_opponent_text(0);
  s.set_special_car(0);
  s.set_restart(0);
}

std::optional<uint32_t> ApplyQueuedPage(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8222C9B0 LocationScreen::ApplyQueuedPage");
  std::optional<uint32_t> r3;
  if (Screen(base).queued_page() == 0) {
    r3 = HidePictures(base, sp - 96);
  }
  const schema::LocationScreen screen = Screen(base);
  const uint32_t page = screen.queued_page();
  const uint32_t transition = screen.queued_transition();
  const schema::PageObject pages = G::pages(base);
  const uint32_t current = pages.page();
  if (page != current) {
    pages.set_previous_page(current);
  }
  pages.set_queued_step(0xFFFFFFFFu);
  if (transition == 0xFFFFFFFFu) {
    pages.set_page(page);
    pages.set_step(0xFFFFFFFFu);
  } else {
    pages.set_step(transition);
    pages.set_next_page(page);
    pages.set_page(0xFFFFFFFFu);
  }
  Screen(base).set_queued_page(0xFFFFFFFFu);
  Screen(base).set_queued_transition(0xFFFFFFFFu);
  return r3;
}

void UpdatePage(uint8_t* base, uint32_t sp, uint32_t machine, uint32_t dispatcher,
                const PageCall& call) {
  ITC_NATIVE_RUNNING("sub_8222EC08 LocationScreen::UpdatePage");
  // The handlers' table: the original's 16 words at its 240-byte frame's
  // +80, read only here (a value since E5 step 6). A page past the table
  // reads the frame's word there, as the original (never seen: pages are
  // 0 to 15).
  const uint32_t frame = sp - 240;
  const uint32_t start = G::pages(base).page();
  if (start != 0xFFFFFFFFu) {
    const uint32_t handler = start < kPageHandlers.size()
                                 ? kPageHandlers[start]
                                 : guest::LoadU32(base, frame + 80 + (start << 2));
    call.call(call.user, handler, dispatcher, machine, 240, 0x8222ED08);
  }
  const schema::LocationScreen screen = Screen(base);
  const schema::LocationScreenView view(base, screen.view_address());
  // The view's camera_vector_1 (its position, probably) copied.
  for (uint32_t k = 0; k < 16; k += 4) {
    guest::StoreU32(base, G::view_copy_address() + k,
                    guest::LoadU32(base, view.camera_vector_1_address() + k));
  }
  if (screen.restart() == 0) {
    return;
  }
  screen.set_restart(0);
  const schema::PageObject pages = G::pages(base);
  const uint32_t transition = pages.step();
  const uint32_t page = pages.page();
  if (transition != 0xFFFFFFFFu) {
    const uint32_t next = pages.next_page();
    if (next != page) {
      pages.set_previous_page(page);
    }
    pages.set_step(transition);
    pages.set_next_page(next);
    pages.set_page(0xFFFFFFFFu);
  } else {
    if (start != page) {
      pages.set_previous_page(page);
    }
    pages.set_page(start);
    pages.set_step(0xFFFFFFFFu);
  }
  pages.set_queued_step(0xFFFFFFFFu);
}

}  // namespace location_screen
