// The other in-race screens natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/race-screens.md): the pause menu, the
// confirm dialog, the career's results, the menu after the race, the
// makers and teardowns of four screens never seen in play, and the menu
// list's and page object's helpers the front end shares. Plain C++ over
// guest memory (the screens, their lists and page objects are shared with
// the recompiled code), with the originals' loads and stores in their
// order. `sp` is the caller's stack pointer: the back chain of the
// original's frame is written there as the original leaves it; its
// register saves aren't. Adapters: src/hooks/race_screens.cpp (and, for
// the functions logged by --log_pause_menu and --log_pause_movie, their
// logging overrides in src/debug/, which call the adapters' steps).
//
// The screens' updates run on logic passes and their draws on every pass,
// as the originals do (the machine's handlers call them; see the file's
// intro and docs/pause-menu-fix.md): nothing here changes when they run.

#pragma once

#include "race_hud_object/race_hud_object.h"

#include <cstdint>

namespace race_screens {

using Calls = race_hud_object::Calls;

// The action mappers' table (8 bytes an entry: the mapper, a word +4: 1
// when it isn't read) and each controller's entry index.
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
// The mapper's read slots the menu list calls (vtable 0x82007F78):
// slot 10 (+40) ActionMapper::Pressed, slot 12 (+48) ActionMapper::Fired.
constexpr uint32_t kPressedSlot = 0x8210BF40;
constexpr uint32_t kFiredSlot = 0x8210C010;

// The menu list's fields (72 bytes; see the file's intro).
constexpr uint32_t kListCount = 0;
constexpr uint32_t kListScroll = 4;
constexpr uint32_t kListScrollBefore = 8;
constexpr uint32_t kListShown = 12;
constexpr uint32_t kListSelection = 16;
constexpr uint32_t kListSelectionBefore = 20;
constexpr uint32_t kListMove = 24;
constexpr uint32_t kListSide = 28;
constexpr uint32_t kListWrapped = 32;
constexpr uint32_t kListDown = 36;
constexpr uint32_t kListUp = 40;
constexpr uint32_t kListLeft = 44;
constexpr uint32_t kListRight = 48;
constexpr uint32_t kListAccept = 52;
constexpr uint32_t kListBack = 56;
constexpr uint32_t kListPlayer = 60;
constexpr uint32_t kListWraps = 64;
constexpr uint32_t kListBlock = 68;

// --- The page object (menu_list.cpp) ---

// ScreenPages::Init (sub_8220BA98): +0, +4, +8 = `page`, +20 and +12 = -1,
// +24 = 0.
void PagesInit(uint8_t* base, uint32_t pages, uint32_t page);
// ScreenPages::Go (sub_8220BA68): `transition` -1: +0 = `page`, +12 and +16
// = -1; else +12 = `transition`, +16 = `queued`, +4 = `page`, +0 = -1.
void PagesGo(uint8_t* base, uint32_t pages, uint32_t page, uint32_t transition, uint32_t queued);
// ScreenPages::Finish (sub_8220BA28): nothing when `result` is 0; else with
// no queued transition (+16 -1) +12 = -1 and +0 = +4, or the queued one
// running (+20 = 0, +12 = it, +16 = -1); then +24 = 0.
void PagesFinish(uint8_t* base, uint32_t pages, uint32_t result);

// --- The menu list (menu_list.cpp) ---

// MenuList::Selection (sub_8220BC20): +16.
uint32_t ListSelection(uint8_t* base, uint32_t list);
// MenuList::SetSelection (sub_8220BC30): +20 = the old +16, +16 = `selection`.
void ListSetSelection(uint8_t* base, uint32_t list, uint32_t selection);
// MenuList::SetEnabled and IsEnabled (sub_8220BC00, sub_8220BC10): the
// item's word in the block (+68).
void ListSetEnabled(uint8_t* base, uint32_t list, uint32_t item, uint32_t value);
uint32_t ListIsEnabled(uint8_t* base, uint32_t list, uint32_t item);
// MenuList::Fit (sub_8220BC40): the scroll and selection refitted; `quiet`
// (r4) nonzero keeps the move word 0 and the "before" words the new ones.
void ListFit(uint8_t* base, uint32_t list, uint32_t quiet);
// MenuList::Fired (sub_8220C020): whether the list's player's mapper fired
// `action` (slot 12; a direction's two ids, 153/167, 154/168, 155/169,
// 156/170, each tried in turn), 0 when the entry's +4 is 1. The slot's
// plain C++ when `native_slots` and it's ActionMapper::Fired, else the slot
// through `calls`.
uint32_t ListFired(uint8_t* base, uint32_t list, uint32_t action, uint32_t sp, bool native_slots,
                   const Calls& calls);
// MenuList::Update (sub_8220C1E0): left, right, up and down (Fired on +44,
// +48, +40, +36) moving the side word or the selection over the enabled
// items, wrapping when +64 is set, the scroll kept around it; returns -1.
uint64_t ListUpdate(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                    const Calls& calls);
// MenuList::Accepted, Cancelled, Pressed152 and Pressed166 (sub_8220BD50,
// sub_8220BDE0, sub_8220BE70, sub_8220BF90): 1 when the list's player's
// mapper answers slot 10 (Pressed) on `action` (+52, +56, 152 or 166) and
// the list's move word (+24) is 0, else 0 (0 when the entry's +4 is 1).
uint32_t ListAccepted(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                      const Calls& calls);
uint32_t ListCancelled(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                       const Calls& calls);
uint32_t ListPressed152(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                        const Calls& calls);
uint32_t ListPressed166(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                        const Calls& calls);

}  // namespace race_screens
