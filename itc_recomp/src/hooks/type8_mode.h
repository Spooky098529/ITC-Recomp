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
void Type8DialogOnEnter(PPCContext& ctx, uint8_t* base);
void Type8DialogOnEnter(lifted::Regs& ctx, uint8_t* base);
void Type8DialogOpenMovies(PPCContext& ctx, uint8_t* base);
void Type8DialogOpenMovies(lifted::Regs& ctx, uint8_t* base);
void Type8DialogWaitForMovies(PPCContext& ctx, uint8_t* base);
void Type8DialogWaitForMovies(lifted::Regs& ctx, uint8_t* base);
void Type8DialogDraw(PPCContext& ctx, uint8_t* base);
void Type8DialogDraw(lifted::Regs& ctx, uint8_t* base);
void Type8DialogUpdate(PPCContext& ctx, uint8_t* base);
void Type8DialogUpdate(lifted::Regs& ctx, uint8_t* base);
void Type8DialogOpenConfirm(PPCContext& ctx, uint8_t* base);
void Type8DialogOpenConfirm(lifted::Regs& ctx, uint8_t* base);
void Type8DialogOpenNotice(PPCContext& ctx, uint8_t* base);
void Type8DialogOpenNotice(lifted::Regs& ctx, uint8_t* base);
void Type8DialogDrawWaiting(PPCContext& ctx, uint8_t* base);
void Type8DialogDrawWaiting(lifted::Regs& ctx, uint8_t* base);
void Type8DialogConfirmPage(PPCContext& ctx, uint8_t* base);
void Type8DialogConfirmPage(lifted::Regs& ctx, uint8_t* base);
void Type8DialogNoticePage(PPCContext& ctx, uint8_t* base);
void Type8DialogNoticePage(lifted::Regs& ctx, uint8_t* base);
void Type8DialogOpenConfirmStep(PPCContext& ctx, uint8_t* base);
void Type8DialogOpenConfirmStep(lifted::Regs& ctx, uint8_t* base);

// The first screen.
void Type8Screen0Construct(PPCContext& ctx, uint8_t* base);
void Type8Screen0Construct(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0Destruct(PPCContext& ctx, uint8_t* base);
void Type8Screen0Destruct(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0DeletingDestruct(PPCContext& ctx, uint8_t* base);
void Type8Screen0DeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0OnEnter(PPCContext& ctx, uint8_t* base);
void Type8Screen0OnEnter(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0OpenMovie(PPCContext& ctx, uint8_t* base);
void Type8Screen0OpenMovie(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0WaitForMovie(PPCContext& ctx, uint8_t* base);
void Type8Screen0WaitForMovie(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0Suspend(PPCContext& ctx, uint8_t* base);
void Type8Screen0Suspend(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0PickYes(PPCContext& ctx, uint8_t* base);
void Type8Screen0PickYes(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0PickNo(PPCContext& ctx, uint8_t* base);
void Type8Screen0PickNo(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0PickBack(PPCContext& ctx, uint8_t* base);
void Type8Screen0PickBack(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0OnDialogClosed(PPCContext& ctx, uint8_t* base);
void Type8Screen0OnDialogClosed(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0Leave(PPCContext& ctx, uint8_t* base);
void Type8Screen0Leave(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0OnLeft(PPCContext& ctx, uint8_t* base);
void Type8Screen0OnLeft(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0Confirm(PPCContext& ctx, uint8_t* base);
void Type8Screen0Confirm(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0ConfirmPage(PPCContext& ctx, uint8_t* base);
void Type8Screen0ConfirmPage(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0ChosenPage(PPCContext& ctx, uint8_t* base);
void Type8Screen0ChosenPage(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0ShowPage(PPCContext& ctx, uint8_t* base);
void Type8Screen0ShowPage(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0PlaceCursor(PPCContext& ctx, uint8_t* base);
void Type8Screen0PlaceCursor(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0Draw(PPCContext& ctx, uint8_t* base);
void Type8Screen0Draw(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0MenuPage(PPCContext& ctx, uint8_t* base);
void Type8Screen0MenuPage(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0OpeningStep(PPCContext& ctx, uint8_t* base);
void Type8Screen0OpeningStep(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0ItemStep(PPCContext& ctx, uint8_t* base);
void Type8Screen0ItemStep(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0CursorStep(PPCContext& ctx, uint8_t* base);
void Type8Screen0CursorStep(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0FadeOutStep(PPCContext& ctx, uint8_t* base);
void Type8Screen0FadeOutStep(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0UpdateStand(PPCContext& ctx, uint8_t* base);
void Type8Screen0UpdateStand(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0Update(PPCContext& ctx, uint8_t* base);
void Type8Screen0Update(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0SetTexts(PPCContext& ctx, uint8_t* base);
void Type8Screen0SetTexts(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0CarCard(PPCContext& ctx, uint8_t* base);
void Type8Screen0CarCard(lifted::Regs& ctx, uint8_t* base);
void Type8Screen0Show(PPCContext& ctx, uint8_t* base);
void Type8Screen0Show(lifted::Regs& ctx, uint8_t* base);

// The mode's slots (r3 the answer, r4 the mode), its +32 interface (r3 the
// mode + 32, r4 a number for ForceExit), AllLoaded (r3 the member) and
// SaveData::FreeTwoPlayerBlock (no arguments).
void Type8ModeAllLoaded(PPCContext& ctx, uint8_t* base);
void Type8ModeAllLoaded(lifted::Regs& ctx, uint8_t* base);
void Type8FreeTwoPlayerBlock(PPCContext& ctx, uint8_t* base);
void Type8FreeTwoPlayerBlock(lifted::Regs& ctx, uint8_t* base);
void Type8ModeRelease(PPCContext& ctx, uint8_t* base);
void Type8ModeRelease(lifted::Regs& ctx, uint8_t* base);
void Type8ModeWaitForLoads(PPCContext& ctx, uint8_t* base);
void Type8ModeWaitForLoads(lifted::Regs& ctx, uint8_t* base);
void Type8ModeNextMode(PPCContext& ctx, uint8_t* base);
void Type8ModeNextMode(lifted::Regs& ctx, uint8_t* base);
void Type8ModePollTeardown(PPCContext& ctx, uint8_t* base);
void Type8ModePollTeardown(lifted::Regs& ctx, uint8_t* base);
void Type8ModeTearDown(PPCContext& ctx, uint8_t* base);
void Type8ModeTearDown(lifted::Regs& ctx, uint8_t* base);
void Type8ModeInterrupt(PPCContext& ctx, uint8_t* base);
void Type8ModeInterrupt(lifted::Regs& ctx, uint8_t* base);
void Type8ModeForceExit(PPCContext& ctx, uint8_t* base);
void Type8ModeForceExit(lifted::Regs& ctx, uint8_t* base);
void Type8ModeInit(PPCContext& ctx, uint8_t* base);
void Type8ModeInit(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
