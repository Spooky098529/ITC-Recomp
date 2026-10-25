// The framework's adapter steps other overrides share
// (src/hooks/framework.cpp, docs/functions/framework.md).

#pragma once

#include "guest.h"

#include "framework/main_loop.h"

namespace hooks {

// Whether --native_framework is on.
bool NativeFramework();
// TickHandler::Invoke (sub_82099A98; r3 = the run loop, r4 = the map
// node's key and delegate): the delegate's slot 1 as a guest call from a
// 96-byte frame like the original's. For its Wrapped override in
// src/frame_pacing/game_messages.cpp.
void TickHandlerInvoke(PPCContext& ctx, uint8_t* base);
// The delegate thunk sub_82096340 (r3 = the object): its slot 1 as a tail
// call. For its Wrapped override in src/debug/frame_state_diff.cpp.
void DelegateSlot1Thunk(PPCContext& ctx, uint8_t* base);

// The main loop's guest calls made through `ctx` (src/framework/main_loop.h),
// and the register steps of the entry and the main loop (their adapters'
// work, the harness's native side): main (sub_82090108), sub_82090378,
// sub_8209ABE8, App::Run (sub_8209A760), ForEachTickHandler (sub_82099D50)
// and ForEachRequest (sub_82099EE8), each from a frame like the original's.
framework::MainLoopCalls MainLoopCalls(PPCContext& ctx, uint8_t* base);
void Main(PPCContext& ctx, uint8_t* base);
void RunFrameworkAt(PPCContext& ctx, uint8_t* base);
void RunFramework(PPCContext& ctx, uint8_t* base);
void AppRun(PPCContext& ctx, uint8_t* base);
void ForEachTickHandler(PPCContext& ctx, uint8_t* base);
void ForEachRequest(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
