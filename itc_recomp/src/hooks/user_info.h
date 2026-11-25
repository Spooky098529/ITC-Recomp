// The user info screen's register steps (src/user_info/,
// docs/functions/user-info.md), shared by the adapters
// (src/hooks/user_info.cpp, behind --native_user_info) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// The machine's handlers (r3 the machine, r4 the mode where read).
void UserInfoCreate(lifted::Regs& ctx);
void UserInfoOpenMovie(lifted::Regs& ctx);
void UserInfoWaitForLoad(lifted::Regs& ctx);
void UserInfoReenter(lifted::Regs& ctx);
void UserInfoClose(lifted::Regs& ctx);
void UserInfoDestroy(lifted::Regs& ctx);
void UserInfoUpdatePage(lifted::Regs& ctx);
void UserInfoUpdate(lifted::Regs& ctx);
// The page (r3 the mode) and its helper (no arguments).
void UserInfoMenuPage(lifted::Regs& ctx);
void UserInfoSetPlayTimeText(lifted::Regs& ctx);
// The transitions (r3 the started word's address) and the texts (no
// arguments).
void UserInfoOpening(lifted::Regs& ctx);
void UserInfoClosing(lifted::Regs& ctx);
void UserInfoSetTexts(lifted::Regs& ctx);
// InitStates (r3 the machine).
void UserInfoInitStates(lifted::Regs& ctx);

}  // namespace hooks
