// The location screen's register steps (src/location_screen/,
// docs/functions/location-screen.md), shared by the adapters
// (src/hooks/location_screen.cpp, behind --native_location_screen) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// sub_8228F0A0 LocationScreen::SceneryConfig: r3, r4; r3.
void LocationScreenSceneryConfig(PPCContext& ctx, uint8_t* base);
// sub_82227690 LocationScreen::KindText: r3 = the kind, r4 = an index; r3.
void LocationScreenKindText(PPCContext& ctx, uint8_t* base);
// sub_822291E8 LocationScreen::IsPictureQueued, sub_82229108 PictureA,
// sub_82229158 PictureB: r3 = a picture id; r3.
void LocationScreenIsPictureQueued(PPCContext& ctx, uint8_t* base);
void LocationScreenPictureA(PPCContext& ctx, uint8_t* base);
void LocationScreenPictureB(PPCContext& ctx, uint8_t* base);
// sub_82227158 LocationScreen::OnMessage14: r3 left.
void LocationScreenOnMessage14(PPCContext& ctx, uint8_t* base);
// sub_822281D8 HasMoreUnlocks, sub_82227618 NextUnlockEntry: r3 = 0 or 1.
void LocationScreenHasMoreUnlocks(PPCContext& ctx, uint8_t* base);
void LocationScreenNextUnlockEntry(PPCContext& ctx, uint8_t* base);
// sub_82227708 OpponentHasCard, sub_822271F8 NextResultText: r3 = 0 or 1
// (the original where the item's copy can't be native).
void LocationScreenOpponentHasCard(PPCContext& ctx, uint8_t* base);
void LocationScreenNextResultText(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
