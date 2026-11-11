// The type 2 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type2-mode.md, its block's fields in
// docs/known-data.md): its Init, the delegate functions Modes::Tick runs
// through ModeDelegate::Invoke and the mode's slot thunks, its +32
// interface and the helpers over its block of 14 screen machines (+848).
// Plain C++ over guest memory (the mode, its block, its message ring and
// delegate are shared with the recompiled code), with the originals' loads
// and stores in their order and their frames' words. `sp` is the caller's
// stack pointer: the back chain of the original's frame and its locals are
// written below it as the original leaves them; its register saves aren't.
// Adapters: src/hooks/type2_mode.cpp (behind --native_type2_mode), register
// steps shared with the harness in src/hooks/type2_mode.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides: GameMessages::Dispatch, LoadingScreen::Run,
// Finish and Show, Sound::Play, StandScene::PollTeardown and Create, the
// hub's sub_8218F620 (never seen in play), Strings::Load,
// Archive::WaitIdle, VirtualAlloc, the 14 machines' Inits, ScreenFades::
// FadeIn, Career::NextDay, the screen start sub_8211AD00, and the slot
// thunks' targets that aren't the type 2 mode's. SaveData::Of and
// Type2Dialog::TakesInput are their plain C++.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace type2_mode {

using Calls = race_hud_object::Calls;

// The mode's fields: the delegate's holder (+4) and the delegate (+8: its
// vtable, the mode, the function, the adjustment), the dispatcher (+36: the
// handlers, the count +236), the message ring (+240: 50 entries of 12
// bytes, the count +840, the start +844) and the block (+848).
constexpr uint32_t kDispatcher = 36;
constexpr uint32_t kRing = 240;
constexpr uint32_t kBlock = 848;
// The block's words: the save screen's state, the loading flag
// AllLoaded tests, the shown screen (FindShown's index) and the first one,
// and the next mode's 8-byte answer.
constexpr uint32_t kSaveScreenState = 0x27B44;
constexpr uint32_t kBlockLoaded = 0x27B48;
constexpr uint32_t kShown = 0x27B68;
constexpr uint32_t kFirstShown = 0x27B6C;
constexpr uint32_t kNextAnswer = 0x27B74;
constexpr uint32_t kBlockBytes = 162688;

// The delegate's vtable (ModeDelegate::Invoke) and the thunks.
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kNextModeThunk = 0x82235C08;
constexpr uint32_t kSlot8Thunk = 0x8223D4B0;
constexpr uint32_t kSlot9Thunk = 0x8223D4C0;
constexpr uint32_t kSlot10Thunk = 0x8223D4A0;
constexpr uint32_t kSlot11Thunk = 0x8222FC60;
constexpr uint32_t kSlot12Thunk = 0x8222FC70;
constexpr uint32_t kSlot13Thunk = 0x8222FC80;

// Globals: the pad blocked (two words: the system UI, probably), the
// front-end menu to show on return, the mode before, the race data, the
// options settings and the dialog's status.
constexpr uint32_t kPadBlocked0 = 0x826C7960;
constexpr uint32_t kPadBlocked1 = 0x826C7964;
constexpr uint32_t kLeavingFlag = 0x826364EC;
constexpr uint32_t kModeBefore = 0x826032E0;
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kOptionsSettings = 0x826D8478;
constexpr uint32_t kDialogStatus = 0x82636204;

// The callees by address.
constexpr uint32_t kDispatch = 0x821062E8;
constexpr uint32_t kLoadingRun = 0x82247898;
constexpr uint32_t kLoadingFinish = 0x82247A60;
constexpr uint32_t kLoadingShow = 0x822478D0;
constexpr uint32_t kSoundPlay = 0x82175850;
constexpr uint32_t kPollTeardown = 0x82179AE8;
constexpr uint32_t kHubPickItem2 = 0x8218F620;
constexpr uint32_t kScreenStart = 0x8211AD00;
constexpr uint32_t kStringsLoad = 0x8210A8D0;
constexpr uint32_t kArchiveWaitIdle = 0x8209D730;
constexpr uint32_t kStandSceneCreate = 0x82180B00;
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kCareerNextDay = 0x82116AF0;

// --- The block's helpers (block.cpp) ---

// sub_82196FC0 ModeType2::MachineOf (with sub_82197000, its case 0): the
// machine `index` names (the 64-bit block plus its offset), or 0. A leaf.
uint64_t MachineOf(uint64_t block, uint64_t index);
// sub_82197130 ModeType2::AllLoaded: 1 when the 11 machines' +9608 words
// and the block's +0x27B48 are all nonzero, else 0. A leaf.
uint32_t AllLoaded(uint8_t* base, uint32_t block);
// sub_82197218 ModeType2::FindShown: the last of machines 0 to 7 in state 4
// or 5 into the block's +0x27B68. MachineOf(block, 7)'s r3. 96-byte frame.
uint64_t FindShown(uint8_t* base, uint64_t block, uint32_t sp);

// --- The delegate functions (delegates.cpp; r3 the 8-byte answer, r4 the
// mode; each returns the answer) ---

// sub_821970C0 ModeType2::TearDown: the answer cleared; once
// StandScene::PollTeardown answers nonzero, the block's +0x27B74 8 bytes.
// 112-byte frame.
void TearDown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82197C88 ModeType2::LeaveScreen: FindShown, message 3, Dispatch, the
// delegate to Slot12Thunk. 128-byte frame.
void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_821977E0 ModeType2::AfterLeaveScreen: FindShown, the delegate to
// NextModeThunk. 96-byte frame.
void AfterLeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp);
// sub_82197A30 ModeType2::Restart: the delegate to Slot9Thunk, message 1,
// Dispatch. 112-byte frame.
void Restart(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82198008 ModeType2::Leaving: message 4, Dispatch; with every machine
// MachineOf finds for 0 to 46 in state 6, 11, 14, 12 or 20, the delegate to
// Slot11Thunk. 128-byte frame.
void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82197D50 ModeType2::NextMode: FindShown; the shown machine's state 8
// sets the delegate to Slot10Thunk, 10 queues message 4; Draw (unless the
// pad is blocked) and Update queued, Dispatch; then 11, 12, the hub's or
// the car dealer's 14, or 20 write the next mode's answer (the frame's
// doubleword) and set the delegate to Slot11Thunk. 144-byte frame.
void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82197B00 ModeType2::WaitForLoads: FindShown, LoadingScreen::Run(-1);
// once AllLoaded, LoadingScreen::Finish(-1), the first screen's message, the
// delegate to NextModeThunk and Sound::Play (bank 1's cue 2); then Draw
// queued and Dispatch. 144-byte frame.
void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls);

// The slot thunks (sub_8223D4B0 Mode::Slot8Thunk, sub_8223D4C0 Slot9Thunk,
// sub_8223D4A0 Slot10Thunk, sub_8222FC60 Slot11Thunk, sub_8222FC70
// Slot12Thunk, sub_8222FC80 Slot13Thunk): the object's vtable slot `slot`
// (8 to 13) on (r3, r4): the type 2 mode's delegate functions their plain
// C++, any other a guest call from `sp` returning to `lr` (a tail call).
// The callee's r3.
uint64_t SlotThunk(uint8_t* base, uint32_t slot, uint64_t r3, uint64_t r4, uint32_t sp,
                   uint32_t lr, const Calls& calls);

// --- The +32 interface (delegates.cpp; r3 the mode + 32) ---

// sub_82197858 ModeType2::Interrupt: 0 until AllLoaded; then message 48
// queued (no dispatch) with the save screen in state 5 or the options'
// +712 0, or when the dialog isn't hidden (status 3: 1) and TakesInput
// answers nonzero. 0 or 1. 96-byte frame.
uint32_t Interrupt(uint8_t* base, uint32_t object, uint32_t sp);
// sub_82197930 ModeType2::ForceExit (r4 a number): 0 or 1, the delegate to
// Slot13Thunk and the answer {1, .., 4} (the leaving flag 1); 2 or 3, the
// same with 12; others nothing. The doubleword built below the stack
// pointer (sp - 16) as the original does. A leaf.
void ForceExit(uint8_t* base, uint32_t object, uint32_t number, uint32_t sp);

// --- The mode's Init (init.cpp) ---

// sub_82197278 ModeType2::Init (r3 the mode): see the row. LoadingScreen::
// Show's r3. 224-byte frame (and the caller's +20 = the mode).
constexpr uint32_t kInitFrame = 224;
uint64_t Init(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);

}  // namespace type2_mode
