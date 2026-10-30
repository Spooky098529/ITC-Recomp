// The location screen's register steps (src/location_screen/,
// docs/functions/location-screen.md), shared by the adapters
// (src/hooks/location_screen.cpp, behind --native_location_screen) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "location_screen/location_screen.h"

#include <memory>

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

// sub_8222F258 InitStates: r3 = the machine; r3 as the last
// StateMachine::Add leaves it.
void LocationScreenInitStates(PPCContext& ctx, uint8_t* base);

// The second migration's guest calls (location_screen::Calls) through
// `ctx`: Career::EventQuery and Race::SetUp through their register steps
// (hooks::CareerEventQuery, RaceSetupSetUp: their plain C++), the others
// through their overrides; the race event's tests and FormatV's calls
// through `ctx` too. lr put back when it goes.
class RaceEventGuest;
class FormatGuestCalls;
class LocationScreenCalls {
 public:
  LocationScreenCalls(PPCContext& ctx, uint8_t* base);
  ~LocationScreenCalls();
  LocationScreenCalls(const LocationScreenCalls&) = delete;
  LocationScreenCalls& operator=(const LocationScreenCalls&) = delete;
  const location_screen::Calls& get() const { return calls_; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[4], uint32_t r1,
                       uint32_t lr);
  PPCContext& ctx_;
  uint8_t* base_;
  uint64_t lr_;
  std::unique_ptr<RaceEventGuest> event_;
  std::unique_ptr<FormatGuestCalls> format_;
  location_screen::Calls calls_;
};

// The second migration's (r3 as the originals leave it): sub_822280D8 Page9
// and sub_82228138 Page11 (r4 = the machine), sub_82227430 NextOpponentText,
// sub_8222C270 LoadPicture (r3 = the screen, r4 = the id, r5 = the list),
// sub_82228438 Picture::Free (r3 = the picture), sub_82228498 FreePictures
// (r3 = the screen).
void LocationScreenPage9(PPCContext& ctx, uint8_t* base);
void LocationScreenPage11(PPCContext& ctx, uint8_t* base);
void LocationScreenNextOpponentText(PPCContext& ctx, uint8_t* base);
void LocationScreenLoadPicture(PPCContext& ctx, uint8_t* base);
void LocationScreenPictureFree(PPCContext& ctx, uint8_t* base);
void LocationScreenFreePictures(PPCContext& ctx, uint8_t* base);

// Whether the texts' functions run natively: the UI movies' text setters'
// conditions (--native_ui_movies_game, --native_c_runtime_heap,
// --native_heap) and the process heap made. The originals otherwise.
bool LocationScreenTextsNative(uint8_t* base);
// sub_8222CD60 ResetMovie, sub_8222A978 ShowUnlockEntry, sub_8222B048
// ShowListEntry (r3 not read); sub_8222CFF0 Transition29, sub_8222D1A8
// Transition25, sub_8222D610 Transition2, sub_8222E6E0 Transition14,
// sub_8222E8D8 Transition16 (r3 = the step's address).
void LocationScreenResetMovie(PPCContext& ctx, uint8_t* base);
void LocationScreenShowUnlockEntry(PPCContext& ctx, uint8_t* base);
void LocationScreenShowListEntry(PPCContext& ctx, uint8_t* base);
void LocationScreenTransition29(PPCContext& ctx, uint8_t* base);
void LocationScreenTransition25(PPCContext& ctx, uint8_t* base);
void LocationScreenTransition2(PPCContext& ctx, uint8_t* base);
void LocationScreenTransition14(PPCContext& ctx, uint8_t* base);
void LocationScreenTransition16(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
