// The title screen's register steps (src/title_screen/,
// docs/functions/title-screen.md), shared by the adapters
// (src/hooks/title_screen_native.cpp, behind --native_title_screen) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// Whether --native_title_screen is on.
bool TitleScreenNative();

// The title load's handlers (r3 the machine, r4 the mode where read), its
// page PostClose (r3 the mode) and WaitForArchive (no arguments), its
// transition PlayLogo (r3 the started word's address) and its Init (r3 the
// machine).
void TitleLoadCreate(PPCContext& ctx, uint8_t* base);
void TitleLoadOpenMovie(PPCContext& ctx, uint8_t* base);
void TitleLoadWaitForMovie(PPCContext& ctx, uint8_t* base);
void TitleLoadStart(PPCContext& ctx, uint8_t* base);
void TitleLoadDraw(PPCContext& ctx, uint8_t* base);
void TitleLoadUpdate(PPCContext& ctx, uint8_t* base);
void TitleLoadPlayLogo(PPCContext& ctx, uint8_t* base);
void TitleLoadWaitForArchive(PPCContext& ctx, uint8_t* base);
void TitleLoadPostClose(PPCContext& ctx, uint8_t* base);
void TitleLoadUnload(PPCContext& ctx, uint8_t* base);
void TitleLoadOpenTitle(PPCContext& ctx, uint8_t* base);
void TitleLoadLeave(PPCContext& ctx, uint8_t* base);
void TitleLoadDestroy(PPCContext& ctx, uint8_t* base);
void TitleLoadInit(PPCContext& ctx, uint8_t* base);
// TitleScreen::Init (r3 the machine).
void TitleScreenInit(PPCContext& ctx, uint8_t* base);
// The title screen's handlers (r3 the machine, r4 the mode where read), its
// set-up (no arguments) and its pages LoadGameData (r3 the mode) and
// LeavePage (r3 the mode, r4 the machine).
void TitleScreenCreate(PPCContext& ctx, uint8_t* base);
void TitleScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void TitleScreenWaitForMovie(PPCContext& ctx, uint8_t* base);
void TitleScreenStart(PPCContext& ctx, uint8_t* base);
void TitleScreenSetUp(PPCContext& ctx, uint8_t* base);
void TitleScreenDraw(PPCContext& ctx, uint8_t* base);
void TitleScreenUpdateTransitions(PPCContext& ctx, uint8_t* base);
void TitleScreenOnGameDataLoaded(PPCContext& ctx, uint8_t* base);
void TitleScreenOnInterrupt(PPCContext& ctx, uint8_t* base);
void TitleScreenOnResume(PPCContext& ctx, uint8_t* base);
void TitleScreenUnload(PPCContext& ctx, uint8_t* base);
void TitleScreenOpenNext(PPCContext& ctx, uint8_t* base);
void TitleScreenLeave(PPCContext& ctx, uint8_t* base);
void TitleScreenDestroy(PPCContext& ctx, uint8_t* base);
void TitleScreenLoadGameData(PPCContext& ctx, uint8_t* base);
void TitleScreenLeavePage(PPCContext& ctx, uint8_t* base);
// The transitions (r3 the started word's address), the menus' set-ups (no
// arguments) and the menu pages (r3 the mode).
void TitleScreenIntro(PPCContext& ctx, uint8_t* base);
void TitleScreenPressStartLoop(PPCContext& ctx, uint8_t* base);
void TitleScreenTransition2(PPCContext& ctx, uint8_t* base);
void TitleScreenLeavePressStart(PPCContext& ctx, uint8_t* base);
void TitleScreenShowMainMenu(PPCContext& ctx, uint8_t* base);
void TitleScreenShowMainMenuAtOnce(PPCContext& ctx, uint8_t* base);
void TitleScreenHighlightMain(PPCContext& ctx, uint8_t* base);
void TitleScreenMoveMainCursor(PPCContext& ctx, uint8_t* base);
void TitleScreenTransition9(PPCContext& ctx, uint8_t* base);
void TitleScreenTransition10(PPCContext& ctx, uint8_t* base);
void TitleScreenShowCareerMenu(PPCContext& ctx, uint8_t* base);
void TitleScreenHighlightCareer(PPCContext& ctx, uint8_t* base);
void TitleScreenMoveCareerCursor(PPCContext& ctx, uint8_t* base);
void TitleScreenBackFromCareer(PPCContext& ctx, uint8_t* base);
void TitleScreenShowMenu4(PPCContext& ctx, uint8_t* base);
void TitleScreenBackFromMenu4(PPCContext& ctx, uint8_t* base);
void TitleScreenMoveMenu4Cursor(PPCContext& ctx, uint8_t* base);
void TitleScreenSetUpMainMenu(PPCContext& ctx, uint8_t* base);
void TitleScreenSetUpCareerMenu(PPCContext& ctx, uint8_t* base);
void TitleScreenSetUpMenu4(PPCContext& ctx, uint8_t* base);
void TitleScreenMainMenu(PPCContext& ctx, uint8_t* base);
void TitleScreenCareerMenu(PPCContext& ctx, uint8_t* base);
void TitleScreenMenu4(PPCContext& ctx, uint8_t* base);
// The mode: its Init (r3 the mode), the delegate functions (r3 the answer,
// r4 the mode), the +32 interface (r3 the mode + 32, r4 ForceExit's number)
// and the block's helpers (r3 the block, r4 MachineOf's index).
void ModeType4Init(PPCContext& ctx, uint8_t* base);
void ModeType4Restart(PPCContext& ctx, uint8_t* base);
void ModeType4NextMode(PPCContext& ctx, uint8_t* base);
void ModeType4LeaveScreen(PPCContext& ctx, uint8_t* base);
void ModeType4WaitForLeave(PPCContext& ctx, uint8_t* base);
void ModeType4Leaving(PPCContext& ctx, uint8_t* base);
void ModeType4NextModeAnswer(PPCContext& ctx, uint8_t* base);
void ModeType4Interrupt(PPCContext& ctx, uint8_t* base);
void ModeType4ForceExit(PPCContext& ctx, uint8_t* base);
void ModeType4Resume(PPCContext& ctx, uint8_t* base);
void ModeType4MachineOf(PPCContext& ctx, uint8_t* base);
void ModeType4AllLoaded(PPCContext& ctx, uint8_t* base);
void ModeType4FindShown(PPCContext& ctx, uint8_t* base);
// "Press start": TitleScreen::Update (r3 the mode, r4 the machine; its
// override, the wrap in hooks/title_screen.cpp, calls it with the flag
// on), OnControllerPicked (no arguments) and Players::AssignController (r3
// the player, r4 the controller; no adapter).
void TitleScreenPressStart(PPCContext& ctx, uint8_t* base);
void TitleScreenOnControllerPicked(PPCContext& ctx, uint8_t* base);
void PlayersAssignController(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
