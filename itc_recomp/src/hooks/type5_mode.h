// The type 5 mode's register steps (src/type5_mode/,
// docs/functions/type5-mode.md), shared by the adapters
// (src/hooks/type5_mode.cpp, behind --native_type5_mode) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The dialog's steps (r3 the started word's address).
void Type5DialogOpenConfirmStep(PPCContext& ctx, uint8_t* base);
void Type5DialogOpenConfirmStep(lifted::Regs& ctx, uint8_t* base);
void Type5DialogCloseConfirmStep(PPCContext& ctx, uint8_t* base);
void Type5DialogCloseConfirmStep(lifted::Regs& ctx, uint8_t* base);
void Type5DialogCloseNoticeStep(PPCContext& ctx, uint8_t* base);
void Type5DialogCloseNoticeStep(lifted::Regs& ctx, uint8_t* base);
void Type5DialogOpenConfirm2Step(PPCContext& ctx, uint8_t* base);
void Type5DialogOpenConfirm2Step(lifted::Regs& ctx, uint8_t* base);
void Type5DialogCloseConfirm2Step(PPCContext& ctx, uint8_t* base);
void Type5DialogCloseConfirm2Step(lifted::Regs& ctx, uint8_t* base);
void Type5DialogCloseNotice2Step(PPCContext& ctx, uint8_t* base);
void Type5DialogCloseNotice2Step(lifted::Regs& ctx, uint8_t* base);
void Type5DialogMoveCursorStep(PPCContext& ctx, uint8_t* base);
void Type5DialogMoveCursorStep(lifted::Regs& ctx, uint8_t* base);
void Type5DialogStep9(PPCContext& ctx, uint8_t* base);
void Type5DialogStep9(lifted::Regs& ctx, uint8_t* base);
// The dialog's pages (r3 the dispatcher).
void Type5DialogConfirmPage(PPCContext& ctx, uint8_t* base);
void Type5DialogConfirmPage(lifted::Regs& ctx, uint8_t* base);
void Type5DialogNoticePage(PPCContext& ctx, uint8_t* base);
void Type5DialogNoticePage(lifted::Regs& ctx, uint8_t* base);
void Type5DialogNotice2Page(PPCContext& ctx, uint8_t* base);
void Type5DialogNotice2Page(lifted::Regs& ctx, uint8_t* base);
// The dialog's handlers (r3 the machine, r4 the dispatcher where read).
void Type5DialogDraw(PPCContext& ctx, uint8_t* base);
void Type5DialogDraw(lifted::Regs& ctx, uint8_t* base);
void Type5DialogUpdate(PPCContext& ctx, uint8_t* base);
void Type5DialogUpdate(lifted::Regs& ctx, uint8_t* base);
void Type5DialogOnEnter(PPCContext& ctx, uint8_t* base);
void Type5DialogOnEnter(lifted::Regs& ctx, uint8_t* base);
void Type5DialogWaitForMovies(PPCContext& ctx, uint8_t* base);
void Type5DialogWaitForMovies(lifted::Regs& ctx, uint8_t* base);
void Type5DialogOnInterrupt(PPCContext& ctx, uint8_t* base);
void Type5DialogOnInterrupt(lifted::Regs& ctx, uint8_t* base);
void Type5DialogOpenConfirm(PPCContext& ctx, uint8_t* base);
void Type5DialogOpenConfirm(lifted::Regs& ctx, uint8_t* base);
void Type5DialogOpenNotice(PPCContext& ctx, uint8_t* base);
void Type5DialogOpenNotice(lifted::Regs& ctx, uint8_t* base);
void Type5DialogOpenConfirm2(PPCContext& ctx, uint8_t* base);
void Type5DialogOpenConfirm2(lifted::Regs& ctx, uint8_t* base);
void Type5DialogOpenNotice2(PPCContext& ctx, uint8_t* base);
void Type5DialogOpenNotice2(lifted::Regs& ctx, uint8_t* base);
void Type5DialogOnLeft(PPCContext& ctx, uint8_t* base);
void Type5DialogOnLeft(lifted::Regs& ctx, uint8_t* base);
// The dialog's Init (r3 the machine).
void Type5DialogInit(PPCContext& ctx, uint8_t* base);
void Type5DialogInit(lifted::Regs& ctx, uint8_t* base);
// The mode: Init (r3 the mode), FindShown (r3 the block), the delegate
// functions (r3 the answer, r4 the mode), the slot 14 thunk, and the +32
// interface (r3 the mode + 32, r4 a number for ForceExit).
void ModeType5Init(PPCContext& ctx, uint8_t* base);
void ModeType5Init(lifted::Regs& ctx, uint8_t* base);
void ModeType5FindShown(PPCContext& ctx, uint8_t* base);
void ModeType5FindShown(lifted::Regs& ctx, uint8_t* base);
void ModeType5WaitForLoads(PPCContext& ctx, uint8_t* base);
void ModeType5WaitForLoads(lifted::Regs& ctx, uint8_t* base);
void ModeType5NextMode(PPCContext& ctx, uint8_t* base);
void ModeType5NextMode(lifted::Regs& ctx, uint8_t* base);
void ModeType5LeaveScreen(PPCContext& ctx, uint8_t* base);
void ModeType5LeaveScreen(lifted::Regs& ctx, uint8_t* base);
void ModeType5WaitForShown(PPCContext& ctx, uint8_t* base);
void ModeType5WaitForShown(lifted::Regs& ctx, uint8_t* base);
void ModeType5Leaving(PPCContext& ctx, uint8_t* base);
void ModeType5Leaving(lifted::Regs& ctx, uint8_t* base);
void ModeSlot14Thunk(PPCContext& ctx, uint8_t* base);
void ModeSlot14Thunk(lifted::Regs& ctx, uint8_t* base);
void ModeType5Interrupt(PPCContext& ctx, uint8_t* base);
void ModeType5Interrupt(lifted::Regs& ctx, uint8_t* base);
void ModeType5ForceExit(PPCContext& ctx, uint8_t* base);
void ModeType5ForceExit(lifted::Regs& ctx, uint8_t* base);
void ModeType5Resume(PPCContext& ctx, uint8_t* base);
void ModeType5Resume(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
