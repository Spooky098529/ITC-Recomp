// The type 8 mode's register steps (src/type8_mode/,
// docs/functions/type8-mode.md), shared by the adapters
// (src/hooks/type8_mode.cpp, behind --native_type8_mode) and the harness
// (without the flag). Each takes the registers its original takes (r3 the
// dialog, screen or mode, r4 the dispatcher where read) and leaves r3 as the
// original does; guest calls go through `ctx` (hooks::Type67Calls).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The dialog.
void Type8DialogOnEnter(lifted::Regs& ctx);
void Type8DialogOpenMovies(lifted::Regs& ctx);
void Type8DialogWaitForMovies(lifted::Regs& ctx);
void Type8DialogDraw(lifted::Regs& ctx);
void Type8DialogUpdate(lifted::Regs& ctx);
void Type8DialogOpenConfirm(lifted::Regs& ctx);
void Type8DialogOpenNotice(lifted::Regs& ctx);
void Type8DialogDrawWaiting(lifted::Regs& ctx);
void Type8DialogConfirmPage(lifted::Regs& ctx);
void Type8DialogNoticePage(lifted::Regs& ctx);
void Type8DialogOpenConfirmStep(lifted::Regs& ctx);

// The first screen.
void Type8Screen0Construct(lifted::Regs& ctx);
void Type8Screen0Destruct(lifted::Regs& ctx);
void Type8Screen0DeletingDestruct(lifted::Regs& ctx);
void Type8Screen0OnEnter(lifted::Regs& ctx);
void Type8Screen0OpenMovie(lifted::Regs& ctx);
void Type8Screen0WaitForMovie(lifted::Regs& ctx);
void Type8Screen0Suspend(lifted::Regs& ctx);
void Type8Screen0PickYes(lifted::Regs& ctx);
void Type8Screen0PickNo(lifted::Regs& ctx);
void Type8Screen0PickBack(lifted::Regs& ctx);
void Type8Screen0OnDialogClosed(lifted::Regs& ctx);
void Type8Screen0Leave(lifted::Regs& ctx);
void Type8Screen0OnLeft(lifted::Regs& ctx);
void Type8Screen0Confirm(lifted::Regs& ctx);
void Type8Screen0ConfirmPage(lifted::Regs& ctx);
void Type8Screen0ChosenPage(lifted::Regs& ctx);
void Type8Screen0ShowPage(lifted::Regs& ctx);
void Type8Screen0PlaceCursor(lifted::Regs& ctx);
void Type8Screen0Draw(lifted::Regs& ctx);
void Type8Screen0MenuPage(lifted::Regs& ctx);
void Type8Screen0OpeningStep(lifted::Regs& ctx);
void Type8Screen0ItemStep(lifted::Regs& ctx);
void Type8Screen0CursorStep(lifted::Regs& ctx);
void Type8Screen0FadeOutStep(lifted::Regs& ctx);
void Type8Screen0UpdateStand(lifted::Regs& ctx);
void Type8Screen0Update(lifted::Regs& ctx);
void Type8Screen0SetTexts(lifted::Regs& ctx);
void Type8Screen0CarCard(lifted::Regs& ctx);
void Type8Screen0Show(lifted::Regs& ctx);

// The mode's slots (r3 the answer, r4 the mode), its +32 interface (r3 the
// mode + 32, r4 a number for ForceExit), AllLoaded (r3 the member) and
// SaveData::FreeTwoPlayerBlock (no arguments).
void Type8ModeAllLoaded(lifted::Regs& ctx);
void Type8FreeTwoPlayerBlock(lifted::Regs& ctx);
void Type8ModeRelease(lifted::Regs& ctx);
void Type8ModeWaitForLoads(lifted::Regs& ctx);
void Type8ModeNextMode(lifted::Regs& ctx);
void Type8ModePollTeardown(lifted::Regs& ctx);
void Type8ModeTearDown(lifted::Regs& ctx);
void Type8ModeInterrupt(lifted::Regs& ctx);
void Type8ModeForceExit(lifted::Regs& ctx);
void Type8ModeInit(lifted::Regs& ctx);

}  // namespace hooks
