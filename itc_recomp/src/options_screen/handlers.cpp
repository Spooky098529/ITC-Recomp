// The options screen's machine handlers (docs/functions/options-screen.md):
// see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kMinusOne = 0xFFFFFFFFu;

// VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE) over the allocation
// layer's pages.
uint32_t Pages(uint32_t size) { return memory::AllocPages(size, 4096); }

// A page block (28 bytes): +4, +0, +8 = 0, +20, +12 = -1, +24 = 0.
void SetUpPageBlock(uint8_t* base, uint32_t block) {
  StoreU32(base, block + 4, 0);
  StoreU32(base, block + 0, 0);
  StoreU32(base, block + 8, 0);
  StoreU32(base, block + 20, kMinusOne);
  StoreU32(base, block + 12, kMinusOne);
  StoreU32(base, block + 24, 0);
}

// A front-end menu (68 bytes) of `items` items.
void SetUpMenu(uint8_t* base, uint32_t menu, uint32_t items) {
  StoreU32(base, menu + 12, 0);
  StoreU32(base, menu + 20, 0);
  StoreU32(base, menu + 0, 0);
  StoreU32(base, menu + 4, 0);
  StoreU32(base, menu + 16, items);
  for (const uint32_t field : {8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    StoreU32(base, menu + field, 0);
  }
}

}  // namespace

uint32_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BB9A8 Options::OnEnter");
  StoreU32(base, sp - 128, sp);
  StoreU32(base, machine + 9608, 0);
  const uint32_t page = Pages(28);
  StoreU32(base, kPage, page);
  SetUpPageBlock(base, page);
  const uint32_t menu = Pages(68);
  StoreU32(base, kMenu, menu);
  SetUpMenu(base, menu, 6);
  const uint32_t settings = Pages(716);
  StoreU32(base, kSettings, settings);
  SettingsReset(base, settings, sp - 128);
  for (const uint32_t global : {0x826D847Cu, 0x826D8480u}) {
    const uint32_t block = Pages(28);
    StoreU32(base, global, block);
    SetUpPageBlock(base, block);
  }
  const uint32_t pages_menu = Pages(68);
  StoreU32(base, 0x826D8484, pages_menu);
  SetUpMenu(base, pages_menu, 1);
  const uint32_t last = Pages(28);
  StoreU32(base, 0x826D8488, last);
  SetUpPageBlock(base, last);
  StoreU32(base, machine + 9604, LoadU32(base, machine + 9612) != 0 ? 1 : 9);
  return last;
}

uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BBBA0 Options::WaitForMovie");
  const uint32_t machine = uint32_t(r3);
  StoreU32(base, sp - 96, sp);
  if (LoadU32(base, machine + 9608) == 0) {
    r3 = ui_movies::IsLoaded(base, 7);
    if (uint32_t(r3) != 0) {
      StoreU32(base, machine + 9608, 1);
    }
  }
  if (LoadU32(base, machine + 9608) != 0) {
    StoreU32(base, machine + 9604, LoadU32(base, machine + 9612) != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821B9F88 Options::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, 7, 22553);
}

uint64_t OnClosed(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BCE38 Options::OnClosed");
  StoreU32(base, sp - 112, sp);
  const uint64_t r3 = SettingsWrite(base, sp - 112, calls);
  uint32_t message = 0;
  switch (int32_t(LoadU32(base, kModeType))) {
    case 2:
    case 6:
    case 8: message = 6; break;
    case 7: message = 5; break;
    case 4: message = 8; break;
    default: break;
  }
  if (message != 0) {
    // The dispatcher's ring (+204: 50 slots of 12 bytes, the count at +600,
    // the start at +604), no overflow check.
    const uint32_t ring = dispatcher + 204;
    const uint32_t count = LoadU32(base, ring + 600) + 1;
    const uint32_t end = LoadU32(base, ring + 604) + count - 1;
    StoreU32(base, ring + 600, count);
    const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
    StoreU32(base, slot + 8, 0);
    StoreU32(base, slot + 4, 0);
    StoreU32(base, slot + 0, message);
  }
  StoreU32(base, machine + 9604, 9);
  return r3;
}

}  // namespace options_screen
