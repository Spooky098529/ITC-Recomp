// The front end's menu helpers natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/menu-helpers.md): Menu::Step and
// Menu::Pressed, which about a hundred front-end screens step their menus
// (68-byte objects, "front-end menu" in docs/known-data.md) and read their
// buttons with. Plain C++ over guest memory with the originals' frame words.
// The player's action mapper is asked through its vtable as the original
// does: the slot's plain C++ (action-mappers.md) when the vtable holds
// ActionMapper::Pressed or Fired and the mappers are native, else the slot
// through the function table (`Calls`). Adapters: src/hooks/menu_helpers.cpp.

#pragma once

#include <cstdint>

namespace menu_helpers {

constexpr uint32_t kMapperEntries = 0x826BAB80;      // 8 bytes a player: the mapper, blocked
constexpr uint32_t kControllerEntries = 0x826BABA4;  // a word a player: its entry's index
// The mapper's read slots the helpers call, and the functions their native
// paths stand for.
constexpr uint32_t kPressedOffset = 40;  // slot 10
constexpr uint32_t kFiredOffset = 48;    // slot 12
constexpr uint32_t kPressedSlot = 0x8210BF40;
constexpr uint32_t kFiredSlot = 0x8210C010;

// The menu's fields (docs/known-data.md's "front-end menu").
constexpr uint32_t kCursor = 0;
constexpr uint32_t kPrevious = 4;
constexpr uint32_t kMoved = 8;
constexpr uint32_t kFirst = 12;
constexpr uint32_t kLast = 16;
constexpr uint32_t kWraps = 20;
constexpr uint32_t kUpDownLocked = 24;
constexpr uint32_t kSidesStep = 28;
constexpr uint32_t kShouldersStep = 32;
constexpr uint32_t kSide = 36;
constexpr uint32_t kWrapped = 40;
constexpr uint32_t kPlayer = 64;

// A guest function called through the function table from the frame at
// `r1`, with r3 and r4 given (the other registers left as they are: the
// original doesn't set them) and the link register `lr` (the original's
// return address); its r3 (64 bits). The adapter's job
// (hooks/menu_helpers.cpp).
struct Calls {
  void* user;
  uint64_t (*call)(void* user, uint32_t function, uint64_t r3, uint64_t r4, uint32_t r1,
                   uint32_t lr);

  uint64_t Call(uint32_t function, uint32_t r1, uint32_t lr, uint64_t r3, uint64_t r4) const {
    return call(user, function, r3, r4, r1, lr);
  }
};

// Menu::Step (sub_82186568): r3 = the menu, `sp` the caller's stack pointer
// (its 128-byte frame's back chain written at sp - 128; its register saves
// above that aren't). Moves the cursor from the player's mapper and returns
// +8 (1, -1 or 0) as the original's r3.
uint32_t Step(uint8_t* base, uint32_t menu, uint32_t sp, bool native_slots, const Calls& calls);

// Menu::Pressed (sub_821864C8): 1 when the last step didn't move the menu
// nor ask for a side, the player's entry isn't blocked and the mapper's
// slot 10 answers nonzero for `action` (r4 as the caller gives it, all 64
// bits passed on to a guest slot); else 0. Its 96-byte frame's back chain at
// sp - 96 (its lr save above it isn't written).
uint32_t Pressed(uint8_t* base, uint32_t menu, uint64_t action, uint32_t sp, bool native_slots,
                 const Calls& calls);

}  // namespace menu_helpers
