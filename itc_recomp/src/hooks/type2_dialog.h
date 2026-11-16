// The type 2 dialog's register steps (src/type2_dialog/,
// docs/functions/type2-dialog.md), shared by the adapters
// (src/hooks/type2_dialog.cpp, behind --native_type2_dialog) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The steps (r3 the started word's address).
void Type2DialogOpenConfirmStep(lifted::Regs& ctx, uint8_t* base);
void Type2DialogCloseConfirmStep(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOpenNoticeStep(lifted::Regs& ctx, uint8_t* base);
void Type2DialogCloseNoticeStep(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOpenConfirm2Step(lifted::Regs& ctx, uint8_t* base);
void Type2DialogCloseConfirm2Step(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOpenNotice2Step(lifted::Regs& ctx, uint8_t* base);
void Type2DialogCloseNotice2Step(lifted::Regs& ctx, uint8_t* base);
void Type2DialogMoveCursorStep(lifted::Regs& ctx, uint8_t* base);
void Type2DialogMoveCursor2Step(lifted::Regs& ctx, uint8_t* base);
// The pages (r3 the mode).
void Type2DialogConfirmPage(lifted::Regs& ctx, uint8_t* base);
void Type2DialogNoticePage(lifted::Regs& ctx, uint8_t* base);
void Type2DialogConfirm2Page(lifted::Regs& ctx, uint8_t* base);
void Type2DialogNotice2Page(lifted::Regs& ctx, uint8_t* base);
// The machine's handlers (r3 the machine, r4 the mode where read).
void Type2DialogDraw(lifted::Regs& ctx, uint8_t* base);
void Type2DialogUpdate(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOnEnter(lifted::Regs& ctx, uint8_t* base);
void Type2DialogWaitForMovies(lifted::Regs& ctx, uint8_t* base);
void Type2DialogShown(lifted::Regs& ctx, uint8_t* base);
void Type2DialogWait(lifted::Regs& ctx, uint8_t* base);
void Type2DialogCancel(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOpenConfirm(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOpenNotice(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOpenConfirm2(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOpenNotice2(lifted::Regs& ctx, uint8_t* base);
void Type2DialogOnLeft(lifted::Regs& ctx, uint8_t* base);
// Init (r3 the machine).
void Type2DialogInit(lifted::Regs& ctx, uint8_t* base);
// TakesInput (no arguments).
void Type2DialogTakesInput(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
