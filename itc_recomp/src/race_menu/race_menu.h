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
constexpr uint32_t kCopyForward = 0x825A7830;  // the second memcpy
constexpr uint32_t kShowNode = 0x8211AC00;
constexpr uint32_t kHideNode = 0x8211AC48;
constexpr uint32_t kSetNodeTexture = 0x8211A708;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNumberText = 0x8211C740;
constexpr uint32_t kSetChildNameText = 0x8211D930;
constexpr uint32_t kScrollText = 0x8211C570;
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

// --- The pages' tables and texts (events.cpp, refresh.cpp) ---
//
// sub_821891C8 RaceMenu::SetUpEvents (r3 the machine, its high bits kept in
// the destinations): the two 15-pair tables by the progress bits (+10148),
// built in its 288-byte frame (+80 to +199, the original's stores in order,
// some overwriting others) and copied by the second memcpy to +9980, then
// +9852. The last copy's r3 (the machine's r3 + 9852).
uint64_t SetUpEvents(uint8_t* base, uint64_t machine_r3, uint32_t sp, const Calls& calls);
// sub_8218BC18 RaceMenu::Refresh (r3 the machine): movie 1's texts for the
// page (+9640: 3 to 5; 6 nothing more). 1, or 0 for the other pages. Its
// 224-byte frame: +80 the movie's id, +96 on page 4's event names (the
// words past the list left as they were). SetText's varargs the call sites
// leave unset passed 0.
uint64_t Refresh(uint8_t* base, uint64_t machine_r3, uint32_t sp, const Calls& calls);

// --- The machine's start (start.cpp) ---
//
// sub_8218C9C8 RaceMenu::Start ((4, 1) -> 5; r3 the machine, its high bits
// kept for SetUpEvents and Refresh): the machine's words, player 0's save
// data's name, money, day and progress bits, the two tables, movie 1's pose
// reset and texts, the nodes' floats and badges, the page object on page 3
// through transition 0, the menu, the hub's music stopped and the menu's
// played (the sound manager's slots 14 and 13, Sound::Play), the fade in.
// FadeIn's r3. 176-byte frame (+80 the movie's id, then the cue; +84 to
// +108 the cues' and banks' words).
uint64_t Start(uint8_t* base, uint64_t machine_r3, uint32_t sp, const Calls& calls);

// --- Update's callees (transitions.cpp) ---
//
// The transitions (r3 the machine): each its 128-byte frame (+80 the
// movie's id) but Transition3's (176) and Transition12's (112), movie 1's
// animations through UiMovie::Finish, Play and Loop's overrides.
// sub_82189740 Transition0 (to page 3): the first time (+9812 0) the
// scene's and the kinds' animations, then (19, mode + 2). Loop's r3.
uint64_t Transition0(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82189C28 Transition3 (to page 4): the first time the nodes' floats
// and the badge's animation by the progress bits, then (10, 0) and (20,
// row + 2), the rows' children 6 to 9 marked (+80 = 1, FindNode and
// UiMovie::Child as plain C++); with +9796 set the transition ended. The
// last Child's r3.
uint64_t Transition3(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8218A7F0 Transition5 (to page 5): (20, row + 14) and (-1, 2) the first
// time, then (11, 12); with +9796 set the transition ended. Loop's r3.
uint64_t Transition5(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8218ACE8 AskConfirm (transition 11, r4 the mode): movie 16's three
// texts (SetText's unset varargs 0), its first cursor from player 0's save
// +0x1F870, page 8, message 34 posted, the transition ended. The ring
// divide's r3.
uint64_t AskConfirm(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls);
// sub_8218AE48 Transition12 (back from the question; reads no register):
// (10, 0), the transition ended. Loop's r3.
uint64_t Transition12(uint8_t* base, uint32_t sp, const Calls& calls);
// The debug pages: their calls to the empty sub_82106540 left out, the
// frame words stored for them written. sub_8218BB68 DebugKindPage (128-byte
// frame; 190); sub_82188A98 DebugEventPage (r3 the machine; 240-byte frame;
// 190, or 170 with +9720 0); sub_82188C00 DebugVariantPage (128-byte frame;
// with the menu's +8 set, page 5 through transition 10; 190).
uint64_t DebugKindPage(uint8_t* base, uint32_t sp);
uint64_t DebugEventPage(uint8_t* base, uint32_t machine, uint32_t sp);
uint64_t DebugVariantPage(uint8_t* base, uint32_t sp);
// sub_8218A928 AnimateVariantPage (r3 the machine; also transition 6): movie
// 1's animations for page 5's input, player 0's mapper asked through its
// slots 12 (153, 167 up; 154, 168 down) and 10 (159, 158) as the original
// does. The last call's r3 (Find's, the pair's address, when none).
uint64_t AnimateVariantPage(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);

// --- The machine's set-up (init.cpp) ---
//
// sub_8218CE10 RaceMenu::Init: 17 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its frame
// (`frame`, the original's stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitFrame = 1232;
uint32_t Init(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace race_menu
