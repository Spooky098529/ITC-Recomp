// The title screen's register steps (src/title_screen/,
// docs/functions/title-screen.md), shared by the adapters
// (src/hooks/title_screen_native.cpp, behind --native_title_screen) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Whether --native_title_screen is on.
bool TitleScreenNative();

// The title load's handlers (r3 the machine, r4 the mode where read), its
// page PostClose (r3 the mode) and WaitForArchive (no arguments), its
// transition PlayLogo (r3 the started word's address) and its Init (r3 the
// machine).
void TitleLoadCreate(lifted::Regs& ctx, uint8_t* base);
void TitleLoadOpenMovie(lifted::Regs& ctx, uint8_t* base);
void TitleLoadWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void TitleLoadStart(lifted::Regs& ctx, uint8_t* base);
void TitleLoadDraw(lifted::Regs& ctx, uint8_t* base);
void TitleLoadUpdate(lifted::Regs& ctx, uint8_t* base);
void TitleLoadPlayLogo(lifted::Regs& ctx, uint8_t* base);
void TitleLoadWaitForArchive(lifted::Regs& ctx, uint8_t* base);
void TitleLoadPostClose(lifted::Regs& ctx, uint8_t* base);
void TitleLoadUnload(lifted::Regs& ctx, uint8_t* base);
void TitleLoadOpenTitle(lifted::Regs& ctx, uint8_t* base);
void TitleLoadLeave(lifted::Regs& ctx, uint8_t* base);
void TitleLoadDestroy(lifted::Regs& ctx, uint8_t* base);
void TitleLoadInit(lifted::Regs& ctx, uint8_t* base);
// TitleScreen::Init (r3 the machine).
void TitleScreenInit(lifted::Regs& ctx, uint8_t* base);
// The title screen's handlers (r3 the machine, r4 the mode where read), its
// set-up (no arguments) and its pages LoadGameData (r3 the mode) and
// LeavePage (r3 the mode, r4 the machine).
void TitleScreenCreate(lifted::Regs& ctx, uint8_t* base);
void TitleScreenOpenMovie(lifted::Regs& ctx, uint8_t* base);
void TitleScreenWaitForMovie(lifted::Regs& ctx, uint8_t* base);
void TitleScreenStart(lifted::Regs& ctx, uint8_t* base);
void TitleScreenSetUp(lifted::Regs& ctx, uint8_t* base);
void TitleScreenDraw(lifted::Regs& ctx, uint8_t* base);
void TitleScreenUpdateTransitions(lifted::Regs& ctx, uint8_t* base);
void TitleScreenOnGameDataLoaded(lifted::Regs& ctx, uint8_t* base);
void TitleScreenOnInterrupt(lifted::Regs& ctx, uint8_t* base);
void TitleScreenOnResume(lifted::Regs& ctx, uint8_t* base);
void TitleScreenUnload(lifted::Regs& ctx, uint8_t* base);
void TitleScreenOpenNext(lifted::Regs& ctx, uint8_t* base);
void TitleScreenLeave(lifted::Regs& ctx, uint8_t* base);
void TitleScreenDestroy(lifted::Regs& ctx, uint8_t* base);
void TitleScreenLoadGameData(lifted::Regs& ctx, uint8_t* base);
void TitleScreenLeavePage(lifted::Regs& ctx, uint8_t* base);
// The transitions (r3 the started word's address), the menus' set-ups (no
// arguments) and the menu pages (r3 the mode).
void TitleScreenIntro(lifted::Regs& ctx, uint8_t* base);
void TitleScreenPressStartLoop(lifted::Regs& ctx, uint8_t* base);
void TitleScreenTransition2(lifted::Regs& ctx, uint8_t* base);
void TitleScreenLeavePressStart(lifted::Regs& ctx, uint8_t* base);
void TitleScreenShowMainMenu(lifted::Regs& ctx, uint8_t* base);
void TitleScreenShowMainMenuAtOnce(lifted::Regs& ctx, uint8_t* base);
void TitleScreenHighlightMain(lifted::Regs& ctx, uint8_t* base);
void TitleScreenMoveMainCursor(lifted::Regs& ctx, uint8_t* base);
void TitleScreenTransition9(lifted::Regs& ctx, uint8_t* base);
void TitleScreenTransition10(lifted::Regs& ctx, uint8_t* base);
void TitleScreenShowCareerMenu(lifted::Regs& ctx, uint8_t* base);
void TitleScreenHighlightCareer(lifted::Regs& ctx, uint8_t* base);
void TitleScreenMoveCareerCursor(lifted::Regs& ctx, uint8_t* base);
void TitleScreenBackFromCareer(lifted::Regs& ctx, uint8_t* base);
void TitleScreenShowMenu4(lifted::Regs& ctx, uint8_t* base);
void TitleScreenBackFromMenu4(lifted::Regs& ctx, uint8_t* base);
void TitleScreenMoveMenu4Cursor(lifted::Regs& ctx, uint8_t* base);
void TitleScreenSetUpMainMenu(lifted::Regs& ctx, uint8_t* base);
void TitleScreenSetUpCareerMenu(lifted::Regs& ctx, uint8_t* base);
void TitleScreenSetUpMenu4(lifted::Regs& ctx, uint8_t* base);
void TitleScreenMainMenu(lifted::Regs& ctx, uint8_t* base);
void TitleScreenCareerMenu(lifted::Regs& ctx, uint8_t* base);
void TitleScreenMenu4(lifted::Regs& ctx, uint8_t* base);
// The mode: its Init (r3 the mode), the delegate functions (r3 the answer,
// r4 the mode), the +32 interface (r3 the mode + 32, r4 ForceExit's number)
// and the block's helpers (r3 the block, r4 MachineOf's index).
void ModeType4Init(lifted::Regs& ctx, uint8_t* base);
void ModeType4Restart(lifted::Regs& ctx, uint8_t* base);
void ModeType4NextMode(lifted::Regs& ctx, uint8_t* base);
void ModeType4LeaveScreen(lifted::Regs& ctx, uint8_t* base);
void ModeType4WaitForLeave(lifted::Regs& ctx, uint8_t* base);
void ModeType4Leaving(lifted::Regs& ctx, uint8_t* base);
void ModeType4NextModeAnswer(lifted::Regs& ctx, uint8_t* base);
void ModeType4Interrupt(lifted::Regs& ctx, uint8_t* base);
void ModeType4ForceExit(lifted::Regs& ctx, uint8_t* base);
void ModeType4Resume(lifted::Regs& ctx, uint8_t* base);
void ModeType4MachineOf(lifted::Regs& ctx, uint8_t* base);
void ModeType4AllLoaded(lifted::Regs& ctx, uint8_t* base);
void ModeType4FindShown(lifted::Regs& ctx, uint8_t* base);
// "Press start": TitleScreen::Update (r3 the mode, r4 the machine; its
// override, the wrap in hooks/title_screen.cpp, calls it with the flag
// on), OnControllerPicked (no arguments) and Players::AssignController (r3
// the player, r4 the controller; no adapter).
void TitleScreenPressStart(lifted::Regs& ctx, uint8_t* base);
void TitleScreenOnControllerPicked(lifted::Regs& ctx, uint8_t* base);
void PlayersAssignController(lifted::Regs& ctx, uint8_t* base);
// The title load's debug sequence: its pages (r3 the mode) and steps (r3
// the started word), 0 to 3 and 0 to 11.
template <uint32_t kWhich>
void TitleLoadDebugPage(lifted::Regs& ctx, uint8_t* base);
template <uint32_t kWhich>
void TitleLoadDebugStep(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
