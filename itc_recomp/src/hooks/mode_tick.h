// The mode tick's register steps that make guest calls, shared by the
// adapters (src/hooks/mode_tick.cpp) and the harness (without the flag).

#pragma once

#include "guest.h"

namespace hooks {

// ScreenFades::Update (sub_8209D130): r3 = 1 while any fade's count isn't
// 0, else 0. Its recording's guest calls (the scene's getters, the render
// queue's submits) from frames like the original's (416 bytes, the color
// rectangle at +112; RecordScissor's 160, RecordFlat's 368); the original
// with the command list's diagnostics on, and its callees' originals where
// their chunks wouldn't fit.
void ModeTickScreenFadesUpdate(PPCContext& ctx, uint8_t* base);

// Music::Play (sub_821063B0): its names' lookup natively, the call through
// [0x826C5C80]'s slot 4 from a 288-byte frame like the original's; r3 as
// that call leaves it.
void ModeTickMusicPlay(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
