// The menu lists' release and X press (docs/functions/alt-hud.md): see
// alt_hud.h.

#include "alt_hud/alt_hud.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "memory/heap.h"
#include "schema/action_mappers.h"
#include "schema/race_screens.h"
#include "schema/racer_controllers.h"

namespace alt_hud {

namespace {

// The action mappers' table (schema::PlayerMapperEntry, 8 bytes an entry)
// and each controller's entry index: RacerControllersGlobals' mapper_entries
// and player_entries.
using RC = schema::RacerControllersGlobals;
constexpr uint32_t kActionX = 160;
constexpr uint32_t kPressedSlot = 0x8210BF40;  // ActionMapper::Pressed, slot 10

}  // namespace

bool MenuListReleasable(uint32_t list) {
  const uint32_t block = schema::MenuList(list).enabled().address;
  return block == 0 || memory::IsPages(block);
}

std::optional<uint32_t> MenuListRelease(uint32_t list,
                                        [[maybe_unused]] uint32_t sp) {
  if (!MenuListReleasable(list)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8220BBB0 MenuList::Release");
  // No frame (E5 step 6): the original's held only its back chain.
  const schema::MenuList menu(list);
  const uint32_t block = menu.enabled().address;
  if (block == 0) {
    return 0;
  }
  memory::FreePages(block);
  menu.set_enabled(0u);
  return 1;
}

uint32_t MenuListPressedX(uint32_t list, uint32_t sp, bool native_slots,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BF00 MenuList::PressedX");
  // No frame (E5 step 6): the slot's call made from `sp` (no stack
  // arguments; it reads none of the caller's frame).
  const uint32_t frame = sp;
  const schema::MenuList menu(list);
  const uint32_t index = guest::LoadU32(menu.player() * 4 + RC::player_entries_address());
  const schema::PlayerMapperEntry entry(index * 8 + RC::mapper_entries_address());
  if (int32_t(entry.unread()) == 1) {
    return 0;
  }
  const uint32_t mapper = entry.mapper().address;
  // ActionMapper's vtable slot 10 (+40): no schema type for the vtable.
  const uint32_t function =
      guest::LoadU32(schema::ActionMapper(mapper).vtable() + 40);
  uint64_t r3;
  if (native_slots && function == kPressedSlot) {
    r3 = action_mappers::Pressed(mapper, kActionX);
    // The read slots' adapters' line for --log_pause_menu.
    debug::PauseMenuLog_NoteSlotRead(10, mapper, kActionX, 0x8220BF5C, double(uint32_t(r3)));
  } else {
    r3 = calls.Call(function, frame, 0x8220BF5C, mapper, kActionX);
  }
  if ((r3 & 0xFF) == 0) {
    return 0;
  }
  return int32_t(menu.move()) == 0 ? 1 : 0;
}

}  // namespace alt_hud
