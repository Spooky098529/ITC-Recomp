// The type 8 mode's register steps (src/type8_mode/,
// docs/functions/type8-mode.md), shared by the adapters
// (src/hooks/type8_mode.cpp, behind --native_type8_mode) and the harness
// (without the flag). Each takes the registers its original takes (r3 the
// dialog, screen or mode, r4 the dispatcher where read) and leaves r3 as the
// original does; guest calls go through `ctx` (hooks::Type67Calls).

#pragma once

#include "guest.h"

namespace hooks {

// The dialog.
void Type8DialogOnEnter(PPCContext& ctx, uint8_t* base);
void Type8DialogOpenMovies(PPCContext& ctx, uint8_t* base);
void Type8DialogWaitForMovies(PPCContext& ctx, uint8_t* base);
void Type8DialogDraw(PPCContext& ctx, uint8_t* base);
void Type8DialogUpdate(PPCContext& ctx, uint8_t* base);
void Type8DialogOpenConfirm(PPCContext& ctx, uint8_t* base);
void Type8DialogOpenNotice(PPCContext& ctx, uint8_t* base);
void Type8DialogDrawWaiting(PPCContext& ctx, uint8_t* base);
void Type8DialogConfirmPage(PPCContext& ctx, uint8_t* base);
void Type8DialogNoticePage(PPCContext& ctx, uint8_t* base);
void Type8DialogOpenConfirmStep(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
