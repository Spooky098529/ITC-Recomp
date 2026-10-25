// The game modes' adapters' guest-calling work (hooks/game_modes.cpp), for
// the harness too.

#pragma once

#include "guest.h"

namespace hooks {

// StateMachine::Add, Find and Handle (sub_82106158, sub_821061F0,
// sub_82106280) with r3 as the originals leave it.
void GameModesAdd(PPCContext& ctx, uint8_t* base);
void GameModesFind(PPCContext& ctx, uint8_t* base);
void GameModesHandle(PPCContext& ctx, uint8_t* base);

// sub_8215DE10 (the race overlays' state machine's reset handler): r3 = the
// state machine; each overlay's slot 3 a guest call.
void GameModesResetOverlays(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
