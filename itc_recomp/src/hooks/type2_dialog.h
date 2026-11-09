// The type 2 dialog's register steps (src/type2_dialog/,
// docs/functions/type2-dialog.md), shared by the adapters
// (src/hooks/type2_dialog.cpp, behind --native_type2_dialog) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The steps (r3 the started word's address).
void Type2DialogOpenConfirmStep(PPCContext& ctx, uint8_t* base);
void Type2DialogCloseConfirmStep(PPCContext& ctx, uint8_t* base);
void Type2DialogOpenNoticeStep(PPCContext& ctx, uint8_t* base);
void Type2DialogCloseNoticeStep(PPCContext& ctx, uint8_t* base);
void Type2DialogOpenConfirm2Step(PPCContext& ctx, uint8_t* base);
void Type2DialogCloseConfirm2Step(PPCContext& ctx, uint8_t* base);
void Type2DialogOpenNotice2Step(PPCContext& ctx, uint8_t* base);
void Type2DialogCloseNotice2Step(PPCContext& ctx, uint8_t* base);
void Type2DialogMoveCursorStep(PPCContext& ctx, uint8_t* base);
void Type2DialogMoveCursor2Step(PPCContext& ctx, uint8_t* base);
// The pages (r3 the mode).
void Type2DialogConfirmPage(PPCContext& ctx, uint8_t* base);
void Type2DialogNoticePage(PPCContext& ctx, uint8_t* base);
void Type2DialogConfirm2Page(PPCContext& ctx, uint8_t* base);
void Type2DialogNotice2Page(PPCContext& ctx, uint8_t* base);
// The machine's handlers (r3 the machine, r4 the mode where read).
void Type2DialogDraw(PPCContext& ctx, uint8_t* base);
void Type2DialogUpdate(PPCContext& ctx, uint8_t* base);
void Type2DialogOnEnter(PPCContext& ctx, uint8_t* base);
void Type2DialogWaitForMovies(PPCContext& ctx, uint8_t* base);
void Type2DialogShown(PPCContext& ctx, uint8_t* base);
void Type2DialogWait(PPCContext& ctx, uint8_t* base);
void Type2DialogCancel(PPCContext& ctx, uint8_t* base);
void Type2DialogOpenConfirm(PPCContext& ctx, uint8_t* base);
void Type2DialogOpenNotice(PPCContext& ctx, uint8_t* base);
void Type2DialogOpenConfirm2(PPCContext& ctx, uint8_t* base);
void Type2DialogOpenNotice2(PPCContext& ctx, uint8_t* base);
void Type2DialogOnLeft(PPCContext& ctx, uint8_t* base);
// TakesInput (no arguments).
void Type2DialogTakesInput(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
