// The user info screen natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/user-info.md, the globals in
// docs/known-data.md): the type 2 mode's machine at its block's +0x102D8,
// the player's record over UI movie 10. Plain C++ over guest memory (the
// machine, its three blocks, the mode's message ring and the movie are
// shared with the recompiled code), with the originals' loads and stores in
// their order and their frames' words. `sp` is the caller's stack pointer:
// the back chain of the original's frame and its locals are written below
// it as the original leaves them; its register saves aren't.
// Adapters: src/hooks/user_info.cpp (behind --native_user_info), register
// steps shared with the harness in src/hooks/user_info.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides: UiMovies::Open and Delete, UiMovie::Play, Run,
// FinishAll, SetText and SetNumberText (SetText's unset varargs 0: the
// user's call for the parking area, 2026-10-30), Menu::Step and Pressed,
// MenuSound::Play, PlayTime::Add, VirtualAlloc and VirtualFree, and the page
// and transitions UpdatePage and Update call through their tables.
// UiMovies::Find and IsLoaded and SaveData::Of are their plain C++.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace user_info {

using Calls = race_hud_object::Calls;

// The screen's blocks: the page object (28 bytes: +0 the page UpdatePage
// runs, -1 while a transition runs; +4 the page after it; +8 the page
// before; +12 the running transition, -1 none; +16 the queued one; +20; +24
// the transition's started word), the menu (68: a front-end menu, items 0
// to 7) and the data (60: +0 1 once loaded, +4 the state Update moves the
// machine to, 21 none; +8 to +56 the record).
constexpr uint32_t kPages = 0x826D84C4;
constexpr uint32_t kMenu = 0x826D84C8;
constexpr uint32_t kData = 0x826D84CC;
// UI movie 10 and its asset.
constexpr uint32_t kMovie = 10;
constexpr uint32_t kAsset = 22552;

// The machine's words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNumberText = 0x8211C740;
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kPlayTimeAdd = 0x821109B8;

// --- The machine's handlers (handlers.cpp) ---

// sub_8220AA50 UserInfo::Create (r3 the machine): +9608 = 0; the page
// object (28 bytes), the menu (68) and the data (60) VirtualAlloc'ed into
// their globals with their first words; +9604 = 1 or 9 by +9612. The last
// VirtualAlloc's r3. 112-byte frame.
uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8220AA40 UserInfo::OpenMovie: UiMovies::Open(10, 22552) (a tail
// call, from the caller's frame with its return address). Its r3.
uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_8220B070 UserInfo::WaitForLoad (r3 the machine): while +9608 is 0,
// +9608 = 1 once movie 10 is loaded; once set, the data's +0 = 1 and +9604
// = 9. IsLoaded's r3 when called, else `r3`. 96-byte frame.
uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8220AB60 UserInfo::Reenter (r3 the machine): with +9608 set, the page
// object to transition 0 (+8 the page before when nonzero), +9604 = 4; else
// +9604 = 1. A leaf.
void Reenter(uint8_t* base, uint32_t machine);
// sub_8220ABB8 UserInfo::Close (r3 the machine, r4 the mode): +9604 = 7,
// UiMovie::FinishAll(movie 10), UiMovies::Delete(10), message 4 posted.
// Delete's r3. 112-byte frame.
uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8220A9D8 UserInfo::Destroy: the three blocks VirtualFree'd, the
// globals 0. The last VirtualFree's r3. 112-byte frame.
uint64_t Destroy(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220B260 UserInfo::UpdatePage (r3 the machine, r4 the mode): the two
// pages' addresses at the frame's +80; unless the page is -1, the page's
// handler called with r3 the mode (through the guest context). Its r3,
// else `r3`. 96-byte frame.
uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8220B2C0 UserInfo::Update (r3 the machine): the empty sub_82106540
// left out (it returns at once); movie 10; the running transition (the
// page object's +12) called with the page object's +24's address (through
// the guest context; a negative one counts as done), the queued one
// started on a nonzero answer; with none (-1), UiMovie::Run(movie, 0); then
// the data's +4 into +9604 when it isn't 21 (+4 = 21). The transition's or
// Run's r3, or the movie. 112-byte frame.
uint64_t Update(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);

// --- The page and transitions (pages.cpp) ---

// sub_8220AFC0 UserInfo::SetPlayTimeText: PlayTime::Add(the frame's +80,
// SaveData::Of(0) + 48) into the data's +44 and +48; UiMovie::SetText(movie
// 10, -1, 23, 529, the hours (at most 9999), the bytes +48 and +49). Its
// r3. 112-byte frame.
uint64_t SetPlayTimeText(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220B0D8 UserInfo::MenuPage (r3 the mode): Menu::Step, SetPlayTimeText
// (its plain C++); on Menu::Pressed 158 or 164, the page object to
// transition 1, message 6 posted and MenuSound::Play(2). The last call's
// r3. 112-byte frame.
uint64_t MenuPage(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8220AD08 UserInfo::SetTexts: movie 10's texts from the data (the
// floats as doubles in r7 and f1 through the frame's +48, the truncated
// distance through its +80). The last call's r3. 192-byte frame.
uint64_t SetTexts(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8220B1C8 UserInfo::Opening and sub_8220AC60 UserInfo::Closing (r3 the
// page object's +24): the first call SetTexts (its plain C++, the opening
// only) and UiMovie::Play(movie 10, -1, 0 or 1, 0, 0, 0, 0), the word 1;
// then UiMovie::Run(movie, 0): 1 when it answers 0 (the closing then sets
// the data's +4 = 9), else 0. 112-byte frames.
uint64_t Opening(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t Closing(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);

// --- The machine's set-up (init.cpp) ---
//
// sub_8220B408 UserInfo::InitStates: 12 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its frame
// (`frame`, the original's stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitFrame = 912;
uint32_t InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace user_info
