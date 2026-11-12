// The type 9 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/todays-result.md, the globals in
// docs/known-data.md): the career's end of the day. The mode's Init and
// delegate functions, its +32 IsLoaded, today's result's machine (the type 9
// block's +0, UI movie 27) and today's event's (+9640, UI movie 0) with
// their pages and transitions. Plain C++ over guest memory (the machines,
// the screens' blocks, the mode's message ring and the movies are shared
// with the recompiled code), with the originals' loads and stores in their
// order and their frames' words. `sp` is the caller's stack pointer: the
// back chain of the original's frame and its locals are written below it as
// the original leaves them; its register saves aren't.
// Adapters: src/hooks/todays_result.cpp (behind --native_todays_result),
// register steps shared with the harness in src/hooks/todays_result.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides (the UI movie calls, VirtualAlloc and VirtualFree,
// the loading screen, the fades, the screen start, Strings::Load,
// GameMessages::Dispatch, and the pages and transitions the tables hold).
// SaveData::Of, UiMovies::Find and IsLoaded are their plain C++. The empty
// sub_82106540 (a debug print) the Updates call first is left out: it
// returns at once.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace todays_result {

using Calls = race_hud_object::Calls;

// The machines' words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The mode's fields: the delegate's holder (+4) and the delegate (+8), the
// dispatcher (+36: the handlers, the count +200; its ring +204 is the
// mode's +240), the message ring (+240) and the block (+848).
constexpr uint32_t kDispatcher = 36;
constexpr uint32_t kRing = 240;
constexpr uint32_t kBlock = 848;
// The block (19,300 bytes): today's result at +0, today's event at +9640,
// the shown machine's index (+19280: 27 today's result, 0 today's event),
// +19284.
constexpr uint32_t kBlockBytes = 19300;
constexpr uint32_t kEventAt = 9640;
constexpr uint32_t kShown = 19280;
constexpr uint32_t kResultIndex = 27;

// The save data's fields Kind reads: the unlocks just made's count and the
// movie queued (-1 none).
constexpr uint32_t kUnlockCount = 0x1CA78;
constexpr uint32_t kQueuedMovie = 0x1D524;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kDispatch = 0x821062E8;
constexpr uint32_t kLoadingShow = 0x822478D0;
constexpr uint32_t kLoadingRun = 0x82247898;
constexpr uint32_t kLoadingFinish = 0x82247A60;
constexpr uint32_t kScreenStart = 0x8211AD00;
constexpr uint32_t kStringsLoad = 0x8210A8D0;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kFadeOut = 0x8209D0C8;
// The fades' passes FadeIn and FadeOut get.
constexpr uint32_t kFadesIn = 0x82001BEC;
constexpr uint32_t kFadesOut = 0x82001BF0;

// --- The mode (mode.cpp) ---

// sub_8225D6D0 ModeType9::Init (r3 the mode): see the row; both machines'
// InitStates as plain C++, the other callees through their overrides.
// ScreenFades::FadeIn's r3. 112-byte frame.
uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8225B358 TodaysResult::InitStates and sub_8225CF30
// TodaysEvent::InitStates: their 13 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in their frames
// (`frame`, the originals' stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitStatesFrame = 976;
uint32_t ResultInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t EventInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
// sub_8225D658 ModeType9::FindShown: of the indexes 0 to 46, 0 (today's
// event, +9640) and 27 (today's result, +0): the last in state 5 or 4 into
// the block's +19280. A leaf.
void FindShown(uint8_t* base, uint32_t block);
// sub_8225BA48 TodaysEvent::Kind: SaveData::Of(0)'s unlocks just made
// counted and a movie 2 to 5 queued: 1; 6: 2; else 0. 96-byte frame.
uint32_t Kind(uint8_t* base, uint32_t sp);
// The delegate functions (r3 the 8-byte answer, r4 the mode; each returns
// the answer): sub_8225D918 WaitForLoads (128-byte frame), sub_8225DC38
// NextMode (144), sub_8225DB70 LeaveScreen (128), sub_8225DA68 WaitForShown
// (112) and sub_8225DE78 Leaving (128, never seen in play).
void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8225D6A8 ModeType9::IsLoaded (r3 the mode + 32): 1 when both
// machines' movies are loaded. A leaf, never seen in play.
uint32_t IsLoaded(uint8_t* base, uint32_t object);

}  // namespace todays_result
