// The career's location screen natively (docs/functions/location-screen.md):
// plain C++ over the guest layout of the screen (912 bytes at [0x82814E50]),
// its page object and menus, store for store as the recompiled functions.
// Adapters in src/hooks/location_screen.cpp (behind --native_location_screen),
// register steps shared with the harness in src/hooks/location_screen.h.

#pragma once

#include <cstdint>
#include <optional>

namespace location_screen {

// The screen's globals (pointers).
constexpr uint32_t kPages = 0x82814E48;    // the page object (28 bytes)
constexpr uint32_t kMenu = 0x82814E4C;     // the location's menu (68 bytes)
constexpr uint32_t kScreen = 0x82814E50;   // the screen (912 bytes)
constexpr uint32_t kListMenu = 0x82814E54;      // the location's list
constexpr uint32_t kOpponentMenu = 0x82814E58;  // the opponents
constexpr uint32_t kConfirmMenu = 0x82814E5C;   // race, the card, back
// The race data (a pointer) and its racer records' offset.
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRacerRecords = 0x59AE8;
// The save data's unlock list's count.
constexpr uint32_t kUnlockCount = 0x1CA78;
// The save data's item records (12 bytes each; +2 a halfword).
constexpr uint32_t kItemRecords = 0x199F8;
// The frames of the functions that copy an item (3,008 bytes) onto their
// stack.
constexpr uint32_t kItemFrame = 3120;

// --- Small helpers (no calls) ---

// sub_8228F0A0 LocationScreen::SceneryConfig: (a + 2) x 3 + b (64-bit, as
// the original's addi, mulli and add).
uint64_t SceneryConfig(uint64_t a, uint64_t b);
// sub_82227690 LocationScreen::KindText: a string id by kind (1 to 7) from
// `index` (64-bit, as the original's addi), else 0.
uint64_t KindText(uint32_t kind, uint64_t index);
// sub_822291E8 LocationScreen::IsPictureQueued: 1 when ring A (+140, 32
// words from +272, +268 of them) holds `id`, else 0.
uint32_t IsPictureQueued(uint8_t* base, uint32_t id);
// sub_82229108 LocationScreen::PictureA and sub_82229158
// LocationScreen::PictureB: the texture record (+8) of the list's picture
// with id `id`, else the default (+544 or +548).
uint32_t PictureA(uint8_t* base, uint32_t id);
uint32_t PictureB(uint8_t* base, uint32_t id);
// sub_82227158 LocationScreen::OnMessage14: +880 and +884 by the location's
// menu's cursor.
void OnMessage14(uint8_t* base);

// --- The lists' steps ---

// sub_822281D8 LocationScreen::HasMoreUnlocks: 1 when +868 is below player
// 0's unlock count (unsigned), else 0.
uint32_t HasMoreUnlocks(uint8_t* base);
// sub_82227618 LocationScreen::NextUnlockEntry: Entry56(+876, +872)'s next
// (+8) added to +872 (1), or +872 and +876 cleared (0).
uint32_t NextUnlockEntry(uint8_t* base);
// sub_82227708 LocationScreen::OpponentHasCard: the picked opponent's item
// (RaceEvent::GroupItem of +804 and +800, +800 stored at frame + 80) copied
// to frame + 96 (memcpy); 1 when its halfword +88 is 4 or 5. `sp` the
// caller's stack pointer (the frame is kItemFrame below it). nullopt, with
// nothing done, where the copy can't be native (the original runs).
std::optional<uint32_t> OpponentHasCard(uint8_t* base, uint32_t sp);
// sub_822271F8 LocationScreen::NextResultText: racer 0's item copied to
// frame + 80; its 20-byte entry k (12, or 10 or 11 by the human racer's
// outcome and an item record) stepped at +872 as NextUnlockEntry. nullopt,
// with nothing done, where the copy can't be native.
std::optional<uint32_t> NextResultText(uint8_t* base, uint32_t sp);

}  // namespace location_screen
