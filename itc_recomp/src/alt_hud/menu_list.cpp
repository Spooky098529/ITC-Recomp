// The menu lists' release and X press (docs/functions/alt-hud.md): see
// alt_hud.h.

#include "alt_hud/alt_hud.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "memory/heap.h"

namespace alt_hud {

namespace {

// The action mappers' table (8 bytes an entry: the mapper, a word +4: 1
// when it isn't read) and each controller's entry index.
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
constexpr uint32_t kActionX = 160;
constexpr uint32_t kPressedSlot = 0x8210BF40;  // ActionMapper::Pressed, slot 10

}  // namespace

bool MenuListReleasable(uint8_t* base, uint32_t list) {
  const uint32_t block = guest::LoadU32(base, list + 68);
  return block == 0 || memory::IsPages(block);
}

std::optional<uint32_t> MenuListRelease(uint8_t* base, uint32_t list, uint32_t sp) {
  if (!MenuListReleasable(base, list)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8220BBB0 MenuList::Release");
  guest::StoreU32(base, sp - 96, sp);
  const uint32_t block = guest::LoadU32(base, list + 68);
  if (block == 0) {
    return 0;
  }
  memory::FreePages(block);
  guest::StoreU32(base, list + 68, 0);
  return 1;
}

uint32_t MenuListPressedX(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BF00 MenuList::PressedX");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  const uint32_t index = guest::LoadU32(base, guest::LoadU32(base, list + 60) * 4 + kControllerEntries);
  const uint32_t entry = index * 8 + kMapperEntries;
  if (int32_t(guest::LoadU32(base, entry + 4)) == 1) {
    return 0;
  }
  const uint32_t mapper = guest::LoadU32(base, entry);
  const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, mapper) + 40);
  uint64_t r3;
  if (native_slots && function == kPressedSlot) {
    r3 = action_mappers::Pressed(base, mapper, kActionX);
    // The read slots' adapters' line for --log_pause_menu.
    debug::PauseMenuLog_NoteSlotRead(10, mapper, kActionX, 0x8220BF5C, double(uint32_t(r3)));
  } else {
    r3 = calls.Call(function, frame, 0x8220BF5C, mapper, kActionX);
  }
  if ((r3 & 0xFF) == 0) {
    return 0;
  }
  return int32_t(guest::LoadU32(base, list + 24)) == 0 ? 1 : 0;
}

}  // namespace alt_hud
