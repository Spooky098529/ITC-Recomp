// The framework's adapter steps other overrides share
// (src/hooks/framework.cpp, docs/functions/framework.md).

#pragma once

#include "guest.h"

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

}  // namespace hooks
