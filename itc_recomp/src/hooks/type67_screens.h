// The type 6 and 7 modes' list, panel and node screens' register steps
// (src/type67_screens/, docs/functions/type67-screens.md), shared by the
// adapters (src/hooks/type67_screens.cpp, behind --native_type67_screens)
// and the harness (without the flag). Each takes the registers its original
// takes (r3 the screen or object, r4 the dispatcher where the original
// reads it) and leaves r3 as the original does; guest calls go through
// `ctx` (hooks::Type67Calls).

#pragma once

#include "guest.h"
#include "type67_screens/type67_screens.h"

namespace hooks {

// The plain C++'s calls: hooks::Type67Calls, and --native_heap for
// MenuList::Release's plain C++.
type67_screens::Calls Type67ScreensCalls(PPCContext& ctx, uint8_t* base);

// The list screens.
void ListScreenOnEnter(PPCContext& ctx, uint8_t* base);
void ListScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void ListScreenWaitForMovie(PPCContext& ctx, uint8_t* base);
void ListScreenLeave(PPCContext& ctx, uint8_t* base);
void ListScreenOnLeft(PPCContext& ctx, uint8_t* base);
void ListScreenIsLoaded(PPCContext& ctx, uint8_t* base);
void Type6ListScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void Type7ListScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void Type6ListScreenRelease(PPCContext& ctx, uint8_t* base);
void Type7ListScreenRelease(PPCContext& ctx, uint8_t* base);

// The node screen.
void NodeScreenOnEnter(PPCContext& ctx, uint8_t* base);
void NodeScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void NodeScreenWaitForMovie(PPCContext& ctx, uint8_t* base);
void NodeScreenLeave(PPCContext& ctx, uint8_t* base);
void NodeScreenOnLeft(PPCContext& ctx, uint8_t* base);
void Type6NodeScreenIsLoaded(PPCContext& ctx, uint8_t* base);
void Type6NodeScreenOpenMovie(PPCContext& ctx, uint8_t* base);
void Type6NodeScreenRelease(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
