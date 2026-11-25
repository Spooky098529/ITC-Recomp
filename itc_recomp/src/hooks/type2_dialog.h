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
void Type2DialogOpenConfirmStep(lifted::Regs& ctx);
void Type2DialogCloseConfirmStep(lifted::Regs& ctx);
void Type2DialogOpenNoticeStep(lifted::Regs& ctx);
void Type2DialogCloseNoticeStep(lifted::Regs& ctx);
void Type2DialogOpenConfirm2Step(lifted::Regs& ctx);
void Type2DialogCloseConfirm2Step(lifted::Regs& ctx);
void Type2DialogOpenNotice2Step(lifted::Regs& ctx);
void Type2DialogCloseNotice2Step(lifted::Regs& ctx);
void Type2DialogMoveCursorStep(lifted::Regs& ctx);
void Type2DialogMoveCursor2Step(lifted::Regs& ctx);
// The pages (r3 the mode).
void Type2DialogConfirmPage(lifted::Regs& ctx);
void Type2DialogNoticePage(lifted::Regs& ctx);
void Type2DialogConfirm2Page(lifted::Regs& ctx);
void Type2DialogNotice2Page(lifted::Regs& ctx);
// The machine's handlers (r3 the machine, r4 the mode where read).
void Type2DialogDraw(lifted::Regs& ctx);
void Type2DialogUpdate(lifted::Regs& ctx);
void Type2DialogOnEnter(lifted::Regs& ctx);
void Type2DialogWaitForMovies(lifted::Regs& ctx);
void Type2DialogShown(lifted::Regs& ctx);
void Type2DialogWait(lifted::Regs& ctx);
void Type2DialogCancel(lifted::Regs& ctx);
void Type2DialogOpenConfirm(lifted::Regs& ctx);
void Type2DialogOpenNotice(lifted::Regs& ctx);
void Type2DialogOpenConfirm2(lifted::Regs& ctx);
void Type2DialogOpenNotice2(lifted::Regs& ctx);
void Type2DialogOnLeft(lifted::Regs& ctx);
// Init (r3 the machine).
void Type2DialogInit(lifted::Regs& ctx);
// TakesInput (no arguments).
void Type2DialogTakesInput(lifted::Regs& ctx);

}  // namespace hooks
