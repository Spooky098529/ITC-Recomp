// The user info screen's register steps (src/user_info/,
// docs/functions/user-info.md), shared by the adapters
// (src/hooks/user_info.cpp, behind --native_user_info) and the harness
// (without the flag). Each takes the registers its original takes and leaves
// r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// The machine's handlers (r3 the machine, r4 the mode where read).
void UserInfoCreate(PPCContext& ctx, uint8_t* base);
void UserInfoOpenMovie(PPCContext& ctx, uint8_t* base);
void UserInfoWaitForLoad(PPCContext& ctx, uint8_t* base);
void UserInfoReenter(PPCContext& ctx, uint8_t* base);
void UserInfoClose(PPCContext& ctx, uint8_t* base);
void UserInfoDestroy(PPCContext& ctx, uint8_t* base);
void UserInfoUpdatePage(PPCContext& ctx, uint8_t* base);
void UserInfoUpdate(PPCContext& ctx, uint8_t* base);
// The page (r3 the mode) and its helper (no arguments).
void UserInfoMenuPage(PPCContext& ctx, uint8_t* base);
void UserInfoSetPlayTimeText(PPCContext& ctx, uint8_t* base);
// The transitions (r3 the started word's address) and the texts (no
// arguments).
void UserInfoOpening(PPCContext& ctx, uint8_t* base);
void UserInfoClosing(PPCContext& ctx, uint8_t* base);
void UserInfoSetTexts(PPCContext& ctx, uint8_t* base);
// InitStates (r3 the machine).
void UserInfoInitStates(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
