// The light glows' billboards' register steps shared by their adapters
// (src/hooks/billboards.cpp, Billboards::Record's wrap in
// src/debug/frame_state_diff.cpp) and the harness; docs/functions/sprites.md.

#pragma once

#include "guest.h"

namespace hooks {

// Whether the recording's plain C++ may run: --native_billboards and
// --native_matrix_math on, the command list's diagnostics off.
bool BillboardsNative();

// Billboards::Record (sub_8209CC38): r3 = the set, r4 = the material; r3
// as the original leaves it, lr put back, flush-to-zero off. The plain C++
// only (its wrap makes the checks).
void BillboardsRecord(PPCContext& ctx, uint8_t* base);

// Billboards::Draw (sub_8209CDB0): r3 = the set, r4 = the material; r3 = 0
// or 1. The plain C++ (Record inside its wrap's checks).
void BillboardsDraw(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
