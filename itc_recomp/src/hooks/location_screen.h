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

// The movies' helpers (r3 the node the original leaves): sub_8222A060
// ShowPictures, sub_8222A150 HidePictures, sub_8222AC18 SetCarColor,
// sub_8222BFE8 SetCursor and sub_8222C058 ListNode (r3 = the value),
// sub_8222C0C8 ShowArrows.
void LocationScreenShowPictures(PPCContext& ctx, uint8_t* base);
void LocationScreenHidePictures(PPCContext& ctx, uint8_t* base);
void LocationScreenSetCarColor(PPCContext& ctx, uint8_t* base);
void LocationScreenSetCursor(PPCContext& ctx, uint8_t* base);
void LocationScreenListNode(PPCContext& ctx, uint8_t* base);
void LocationScreenShowArrows(PPCContext& ctx, uint8_t* base);
// UiMovie::Run's callers, through its override from frames like the
// originals' (lr put back): sub_822291A8 RunCardMovie (r3 Run's),
// sub_8222AD50 Transition1 and sub_8222BE60 Transition18 (r3 = the step's
// address; r3 = 1 when done, else 0).
void LocationScreenRunCardMovie(PPCContext& ctx, uint8_t* base);
void LocationScreenTransition1(PPCContext& ctx, uint8_t* base);
void LocationScreenTransition18(PPCContext& ctx, uint8_t* base);

// sub_822895E0 ScreenMachine::Reenter (r3 = a machine; r3 left),
// sub_822279A8 Reset and sub_82227818 ResetView (r3 = the screen, the view;
// r3 as memcpy leaves it, the view's +176; ResetView without an adapter, only
// Reset's plain C++ calls it: the harness's step), sub_8222C9B0 ApplyQueuedPage
// (r3 HidePictures' or as it came), sub_8222EC08 UpdatePage (r3 = the
// machine, r4 = the dispatcher; the page's handler through ctx).
void LocationScreenReenter(PPCContext& ctx, uint8_t* base);
void LocationScreenReset(PPCContext& ctx, uint8_t* base);
void LocationScreenResetView(PPCContext& ctx, uint8_t* base);
void LocationScreenApplyQueuedPage(PPCContext& ctx, uint8_t* base);
void LocationScreenUpdatePage(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
