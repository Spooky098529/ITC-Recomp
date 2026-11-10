// The type 2 mode's race menu natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/race-menu.md, the machine's words in
// docs/known-data.md's "race menu"): the machine at the type 2 mode block's
// +9640, UI movie 1, where the career's races start (the race mode, the
// event, its variant, the question, then Career::SetUpEvent). Plain C++ over
// guest memory (the machine, its three blocks, the mode's message ring and
// the movies are shared with the recompiled code), with the originals' loads
// and stores in their order and their frames' words. `sp` is the caller's
// stack pointer: the back chain of the original's frame and its locals are
// written below it as the original leaves them; its register saves aren't.
// Adapters: src/hooks/race_menu.cpp (behind --native_race_menu), register
// steps shared with the harness in src/hooks/race_menu.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0, or all eight) by address, to the callees' overrides.
// UiMovies::Find, UiMovies::IsLoaded and SaveData::Of are their plain C++.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace race_menu {

using Calls = race_hud_object::Calls;

// The blocks (globals, VirtualAlloc'ed by Create): the page object (28
// bytes: +0 the page Draw runs, -1 while a transition runs; +4 the page
// after it; +8 the page before; +12 the transition Update runs, -1 none;
// +16 the one queued after it; +20; +24 the transition's set-up done), a
// second page object (28; Draw runs Back when its +0 is 7) and the menu (68:
// a front-end menu).
constexpr uint32_t kPages = 0x826D8404;
constexpr uint32_t kPages2 = 0x826D8408;
constexpr uint32_t kMenu = 0x826D840C;

constexpr uint32_t kMovie = 1;
constexpr uint32_t kAsset = 22546;

// The machine's words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;
constexpr uint32_t kPage = 9640;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kFadeOut = 0x8209D0C8;
// The boot movies' list (boot-mode.md).
constexpr uint32_t kBootList = 0x826C5BAC;

// --- The machine's handlers (handlers.cpp) ---
//
// sub_82188E20 RaceMenu::Create (r3 the machine): the machine's words, the
// debug list (+9656), the three blocks VirtualAlloc'ed with their first
// words, state 1 or 9. The last VirtualAlloc's r3. 144-byte frame.
uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_821889E0 RaceMenu::OpenMovie: UiMovies::Open(1, 22546), +9608 = 0.
// Open's r3. 96-byte frame.
uint64_t OpenMovie(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_821890D8 RaceMenu::WaitForMovie: +9608 once movie 1 is loaded, then
// +9780 = 1 and state 9 or 4. IsLoaded's r3 when called, else `r3`.
// 112-byte frame.
uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821895F0 RaceMenu::OnYes and sub_82189648 OnNo: +9788 = 1, player 0's
// save +0x1F870 = 1 (yes) or 0, +10108 = 0 or 1. SaveData::Of's r3.
// 112-byte frames.
uint64_t OnAnswer(uint8_t* base, uint32_t machine, bool yes, uint32_t sp);
// sub_82188938 RaceMenu::OnBack: back to page 5 or 4 through transition 12.
// A leaf (r3 left).
void OnBack(uint8_t* base, uint32_t machine);
// sub_821896A0 RaceMenu::OnDialogClosed: when answered, the fade out,
// UiMovie::Run and page 10; +9784 = 1. Run's r3, or Find's (the pair's
// address). 128-byte frame.
uint64_t OnDialogClosed(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8218AF08 RaceMenu::Unload: UiMovie::FinishAll(movie 1); with +9616
// set, UiMovies::Delete(1) and Delete(16), +9608 = 0. The last call's r3.
// 128-byte frame.
uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8218AF80 RaceMenu::LeaveToHub (r4 the mode): message +9808 posted,
// state 9. A leaf (r3 left).
void LeaveToHub(uint8_t* base, uint32_t machine, uint32_t mode);
// sub_82188FE0 RaceMenu::Destroy: the three blocks VirtualFree'd; on day 2
// the boot movies' list and state 20, else 11 or 12. The last VirtualFree's
// r3. 112-byte frame.
uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82188A20 RaceMenu::Back: the page from the history, both page objects
// reset. A leaf (r3 left).
void Back(uint8_t* base, uint32_t machine);
// sub_82189158 RaceMenu::SetEventRange: +9840, +9844 and +9848 by the
// progress bits (+10148). A leaf (r3 left).
void SetEventRange(uint8_t* base, uint32_t machine);

// --- The machine's set-up (init.cpp) ---
//
// sub_8218CE10 RaceMenu::Init: 17 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its frame
// (`frame`, the original's stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitFrame = 1232;
uint32_t Init(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace race_menu
