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
void TitleLoadCreate(lifted::Regs& ctx);
void TitleLoadOpenMovie(lifted::Regs& ctx);
void TitleLoadWaitForMovie(lifted::Regs& ctx);
void TitleLoadStart(lifted::Regs& ctx);
void TitleLoadDraw(lifted::Regs& ctx);
void TitleLoadUpdate(lifted::Regs& ctx);
void TitleLoadPlayLogo(lifted::Regs& ctx);
void TitleLoadWaitForArchive(lifted::Regs& ctx);
void TitleLoadPostClose(lifted::Regs& ctx);
void TitleLoadUnload(lifted::Regs& ctx);
void TitleLoadOpenTitle(lifted::Regs& ctx);
void TitleLoadLeave(lifted::Regs& ctx);
void TitleLoadDestroy(lifted::Regs& ctx);
void TitleLoadInit(lifted::Regs& ctx);
// TitleScreen::Init (r3 the machine).
void TitleScreenInit(lifted::Regs& ctx);
// The title screen's handlers (r3 the machine, r4 the mode where read), its
// set-up (no arguments) and its pages LoadGameData (r3 the mode) and
// LeavePage (r3 the mode, r4 the machine).
void TitleScreenCreate(lifted::Regs& ctx);
void TitleScreenOpenMovie(lifted::Regs& ctx);
void TitleScreenWaitForMovie(lifted::Regs& ctx);
void TitleScreenStart(lifted::Regs& ctx);
void TitleScreenSetUp(lifted::Regs& ctx);
void TitleScreenDraw(lifted::Regs& ctx);
void TitleScreenUpdateTransitions(lifted::Regs& ctx);
void TitleScreenOnGameDataLoaded(lifted::Regs& ctx);
void TitleScreenOnInterrupt(lifted::Regs& ctx);
void TitleScreenOnResume(lifted::Regs& ctx);
void TitleScreenUnload(lifted::Regs& ctx);
void TitleScreenOpenNext(lifted::Regs& ctx);
void TitleScreenLeave(lifted::Regs& ctx);
void TitleScreenDestroy(lifted::Regs& ctx);
void TitleScreenLoadGameData(lifted::Regs& ctx);
void TitleScreenLeavePage(lifted::Regs& ctx);
// The transitions (r3 the started word's address), the menus' set-ups (no
// arguments) and the menu pages (r3 the mode).
void TitleScreenIntro(lifted::Regs& ctx);
void TitleScreenPressStartLoop(lifted::Regs& ctx);
void TitleScreenTransition2(lifted::Regs& ctx);
void TitleScreenLeavePressStart(lifted::Regs& ctx);
void TitleScreenShowMainMenu(lifted::Regs& ctx);
void TitleScreenShowMainMenuAtOnce(lifted::Regs& ctx);
void TitleScreenHighlightMain(lifted::Regs& ctx);
void TitleScreenMoveMainCursor(lifted::Regs& ctx);
void TitleScreenTransition9(lifted::Regs& ctx);
void TitleScreenTransition10(lifted::Regs& ctx);
void TitleScreenShowCareerMenu(lifted::Regs& ctx);
void TitleScreenHighlightCareer(lifted::Regs& ctx);
void TitleScreenMoveCareerCursor(lifted::Regs& ctx);
void TitleScreenBackFromCareer(lifted::Regs& ctx);
void TitleScreenShowMenu4(lifted::Regs& ctx);
void TitleScreenBackFromMenu4(lifted::Regs& ctx);
void TitleScreenMoveMenu4Cursor(lifted::Regs& ctx);
void TitleScreenSetUpMainMenu(lifted::Regs& ctx);
void TitleScreenSetUpCareerMenu(lifted::Regs& ctx);
void TitleScreenSetUpMenu4(lifted::Regs& ctx);
void TitleScreenMainMenu(lifted::Regs& ctx);
void TitleScreenCareerMenu(lifted::Regs& ctx);
void TitleScreenMenu4(lifted::Regs& ctx);
// The mode: its Init (r3 the mode), the delegate functions (r3 the answer,
// r4 the mode), the +32 interface (r3 the mode + 32, r4 ForceExit's number)
// and the block's helpers (r3 the block, r4 MachineOf's index).
void ModeType4Init(lifted::Regs& ctx);
void ModeType4Restart(lifted::Regs& ctx);
void ModeType4NextMode(lifted::Regs& ctx);
void ModeType4LeaveScreen(lifted::Regs& ctx);
void ModeType4WaitForLeave(lifted::Regs& ctx);
void ModeType4Leaving(lifted::Regs& ctx);
void ModeType4NextModeAnswer(lifted::Regs& ctx);
void ModeType4Interrupt(lifted::Regs& ctx);
void ModeType4ForceExit(lifted::Regs& ctx);
void ModeType4Resume(lifted::Regs& ctx);
void ModeType4MachineOf(lifted::Regs& ctx);
void ModeType4AllLoaded(lifted::Regs& ctx);
void ModeType4FindShown(lifted::Regs& ctx);
// "Press start": TitleScreen::Update (r3 the mode, r4 the machine; its
// override, the wrap in hooks/title_screen.cpp, calls it with the flag
// on), OnControllerPicked (no arguments) and Players::AssignController (r3
// the player, r4 the controller; no adapter).
void TitleScreenPressStart(lifted::Regs& ctx);
void TitleScreenOnControllerPicked(lifted::Regs& ctx);
void PlayersAssignController(lifted::Regs& ctx);
// The title load's debug sequence: its pages (r3 the mode) and steps (r3
// the started word), 0 to 3 and 0 to 11.
template <uint32_t kWhich>
void TitleLoadDebugPage(lifted::Regs& ctx);
template <uint32_t kWhich>
void TitleLoadDebugStep(lifted::Regs& ctx);

}  // namespace hooks
