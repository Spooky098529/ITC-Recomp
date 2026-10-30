// The location screen's state: its reset, the page object's steps and the
// page update (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "guest.h"

#include <array>
#include <bit>

namespace location_screen {

namespace {

uint32_t Screen(uint8_t* base) { return guest::LoadU32(base, kScreen); }

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

// UpdatePage's copy of the screen's +592 (its view's position, probably).
constexpr uint32_t kViewCopy = 0x8265ECE0;

}  // namespace

void Reenter(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_822895E0 ScreenMachine::Reenter");
  guest::StoreU32(base, machine + 9604, guest::LoadU32(base, machine + 9608) != 0 ? 4 : 1);
}

bool ResetNative(uint32_t screen) {
  return screen < 0xA0000000u - 800;
}

void ResetView(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82227818 LocationScreen::ResetView");
  const uint32_t zero = ThroughFloat(base, 0x82000A34);
  const uint32_t one = ThroughFloat(base, 0x82000A30);
  guest::StoreU32(base, view + 0, 0);
  guest::StoreU32(base, view + 4, 0);
  guest::StoreU32(base, view + 12, 0);
  guest::StoreU32(base, view + 16, 1);
  guest::StoreU32(base, view + 8, 1);
  for (uint32_t v = 0; v < 4; v++) {
    const uint32_t at = view + 32 + 16 * v;
    guest::StoreU32(base, at + 0, zero);
    guest::StoreU32(base, at + 4, zero);
    guest::StoreU32(base, at + 8, zero);
    guest::StoreU32(base, at + 12, one);
  }
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, view + 96 + 4 * k, zero);
  }
  StoreIdentity(base, view + 112);
  StoreIdentity(base, view + 176);
}

void Reset(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_822279A8 LocationScreen::Reset");
  for (uint32_t offset : {0u, 552u, 800u, 856u, 860u}) {
    guest::StoreU32(base, screen + offset, 0);
  }
  guest::StoreU32(base, screen + 864, 14);
  ResetView(base, screen + 560);
  for (uint32_t offset : {136u, 404u, 544u, 548u, 808u}) {
    guest::StoreU32(base, screen + offset, 0);
  }
  guest::StoreU32(base, screen + 804, 4);
  for (uint32_t offset : {852u, 868u, 872u, 876u, 812u}) {
    guest::StoreU32(base, screen + offset, 0);
  }
  guest::StoreU32(base, screen + 880, 0xFFFFFFFFu);
  guest::StoreU32(base, screen + 884, 0xFFFFFFFFu);
  for (uint32_t offset : {816u, 820u, 888u, 892u, 900u}) {
    guest::StoreU32(base, screen + offset, 0);
  }
}

std::optional<uint32_t> ApplyQueuedPage(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8222C9B0 LocationScreen::ApplyQueuedPage");
  std::optional<uint32_t> r3;
  if (guest::LoadU32(base, Screen(base) + 880) == 0) {
    r3 = HidePictures(base, sp - 96);
  }
  const uint32_t screen = Screen(base);
  const uint32_t page = guest::LoadU32(base, screen + 880);
  const uint32_t transition = guest::LoadU32(base, screen + 884);
  const uint32_t pages = guest::LoadU32(base, kPages);
  const uint32_t current = guest::LoadU32(base, pages + 0);
  if (page != current) {
    guest::StoreU32(base, pages + 8, current);
  }
  guest::StoreU32(base, pages + 16, 0xFFFFFFFFu);
  if (transition == 0xFFFFFFFFu) {
    guest::StoreU32(base, pages + 0, page);
    guest::StoreU32(base, pages + 12, 0xFFFFFFFFu);
  } else {
    guest::StoreU32(base, pages + 12, transition);
    guest::StoreU32(base, pages + 4, page);
    guest::StoreU32(base, pages + 0, 0xFFFFFFFFu);
  }
  guest::StoreU32(base, Screen(base) + 880, 0xFFFFFFFFu);
  guest::StoreU32(base, Screen(base) + 884, 0xFFFFFFFFu);
  return r3;
}

void UpdatePage(uint8_t* base, uint32_t sp, uint32_t machine, uint32_t dispatcher,
                const PageCall& call) {
  ITC_NATIVE_RUNNING("sub_8222EC08 LocationScreen::UpdatePage");
  const uint32_t frame = sp - 240;
  const uint32_t start = guest::LoadU32(base, guest::LoadU32(base, kPages));
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, frame + 80 + 4 * i, kPageHandlers[i]);
  }
  if (start != 0xFFFFFFFFu) {
    const uint32_t handler = guest::LoadU32(base, frame + 80 + (start << 2));
    call.call(call.user, handler, dispatcher, machine, 240, 0x8222ED08);
  }
  const uint32_t screen = Screen(base);
  for (uint32_t k = 0; k < 16; k += 4) {
    guest::StoreU32(base, kViewCopy + k, guest::LoadU32(base, screen + 592 + k));
  }
  if (guest::LoadU32(base, screen + 900) == 0) {
    return;
  }
  guest::StoreU32(base, screen + 900, 0);
  const uint32_t pages = guest::LoadU32(base, kPages);
  const uint32_t transition = guest::LoadU32(base, pages + 12);
  const uint32_t page = guest::LoadU32(base, pages + 0);
  if (transition != 0xFFFFFFFFu) {
    const uint32_t next = guest::LoadU32(base, pages + 4);
    if (next != page) {
      guest::StoreU32(base, pages + 8, page);
    }
    guest::StoreU32(base, pages + 12, transition);
    guest::StoreU32(base, pages + 4, next);
    guest::StoreU32(base, pages + 0, 0xFFFFFFFFu);
  } else {
    if (start != page) {
      guest::StoreU32(base, pages + 8, page);
    }
    guest::StoreU32(base, pages + 0, start);
    guest::StoreU32(base, pages + 12, 0xFFFFFFFFu);
  }
  guest::StoreU32(base, pages + 16, 0xFFFFFFFFu);
}

}  // namespace location_screen
