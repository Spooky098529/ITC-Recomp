// The title screen's register steps (src/title_screen/,
// docs/functions/title-screen.md), shared by the adapters
// (src/hooks/title_screen_native.cpp, behind --native_title_screen) and the
// harness (without the flag). Each takes the registers its original takes
// and leaves r3 as the original does; guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

// Whether --native_title_screen is on.
bool TitleScreenNative();

// The title load's handlers (r3 the machine, r4 the mode where read), its
// page PostClose (r3 the mode) and WaitForArchive (no arguments), its
// transition PlayLogo (r3 the started word's address) and its Init (r3 the
// machine).
void TitleLoadCreate(PPCContext& ctx, uint8_t* base);
void TitleLoadOpenMovie(PPCContext& ctx, uint8_t* base);
void TitleLoadWaitForMovie(PPCContext& ctx, uint8_t* base);
void TitleLoadStart(PPCContext& ctx, uint8_t* base);
void TitleLoadDraw(PPCContext& ctx, uint8_t* base);
void TitleLoadUpdate(PPCContext& ctx, uint8_t* base);
void TitleLoadPlayLogo(PPCContext& ctx, uint8_t* base);
void TitleLoadWaitForArchive(PPCContext& ctx, uint8_t* base);
void TitleLoadPostClose(PPCContext& ctx, uint8_t* base);
void TitleLoadUnload(PPCContext& ctx, uint8_t* base);
void TitleLoadOpenTitle(PPCContext& ctx, uint8_t* base);
void TitleLoadLeave(PPCContext& ctx, uint8_t* base);
void TitleLoadDestroy(PPCContext& ctx, uint8_t* base);
void TitleLoadInit(PPCContext& ctx, uint8_t* base);
// TitleScreen::Init (r3 the machine).
void TitleScreenInit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
