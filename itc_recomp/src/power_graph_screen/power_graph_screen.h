// The type 6 and 7 modes' power graph screen natively (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/power-graph-screen.md, the globals in docs/known-data.md):
// the machine both modes register the same handlers for (UI movie 9 over
// the car the player picked, its specs as texts, the graph itself drawn by
// whoever opened it while 0x82815EC0 is set), its pages, its steps and the
// members' vtable slots that post the machine's replies. Plain C++ over
// guest memory (the member, its three blocks and the shown word, the mode's
// message ring and the movie are shared with the recompiled code), with the
// originals' loads and stores in their order and their frames' words. `sp`
// is the caller's stack pointer: the back chain of the original's frame and
// its locals are written below it as the original leaves them; its register
// saves aren't.
// Adapters: src/hooks/power_graph_screen.cpp (behind
// --native_power_graph_screen), register steps shared with the harness in
// src/hooks/power_graph_screen.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides: UiMovie::SetText, Play and Run, UiMovies::Delete,
// Menu::Step, Menu::Pressed, MenuSound::Play, VirtualAlloc and VirtualFree,
// the members' vtable slots, and the pages and steps Draw and Update call
// through their tables. UiMovies::Find, IsLoaded and UiMovie::FindNode are
// their plain C++.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace power_graph_screen {

using Calls = race_hud_object::Calls;

// The screen's blocks: the page object (28 bytes: +0 the page Draw runs, -1
// while a step runs; +4 the page after the step; +8 the page before; +12
// the step Update runs, -1 none; +16 the step queued after it; +20; +24 the
// step's started word), the menu (68: a front-end menu) and the data (92:
// +0 movie 9 loaded, +4 the state queued, 21 none, +8 to +88 the record,
// +88 its tab word); the graph shown.
constexpr uint32_t kPages = 0x82815EB4;
constexpr uint32_t kMenu = 0x82815EB8;
constexpr uint32_t kData = 0x82815EBC;
constexpr uint32_t kShown = 0x82815EC0;

// The member's words (r3 the member).
constexpr uint32_t kMachineState = 9608;
constexpr uint32_t kMachineLoaded = 9612;
constexpr uint32_t kMachineReenter = 9616;

// The movie's id.
constexpr uint32_t kMovie = 9;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kMenuStep = 0x82186568;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;

// --- The members' vtable slots (handlers.cpp; r4 the mode, r3 unchanged) ---
//
// sub_82237A88 PowerGraphScreen6::PostLeave (4), sub_82237AD8 PostClosing
// (23), sub_82237B28 PostClosed (24), sub_82246FB8 PowerGraphScreen7::
// PostLeave (3), sub_82247008 PostClosing (20): the message (id, 0, 0)
// posted on the mode's ring (+204: its count +600, start +604, 50 slots of
// 12 bytes). Leaves.
void Post(uint8_t* base, uint32_t mode, uint32_t id);
// The slot at `slot`'s address posting its message.
void SlotPost(uint8_t* base, uint32_t mode, uint32_t slot);

// --- The machine's handlers (handlers.cpp; r3 the member) ---
//
// sub_82250778 OnEnter: the three blocks VirtualAlloc'ed with their first
// words, state 1 or 9, the shown word 0. The last VirtualAlloc's r3.
// 112-byte frame.
uint64_t OnEnter(uint8_t* base, uint32_t member, uint32_t sp, const Calls& calls);
// sub_82250D70 WaitForLoad: movie 9 loaded, then the data's +0 = 1 and state
// 9. IsLoaded's r3 when called, else `r3`. 96-byte frame.
uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82250338 Shown: the shown word 0. A leaf.
void Shown(uint8_t* base);
// sub_82250890 Open: with the movie loaded, step 0 queued for page 0 (state
// 4), else state 1; the shown word 0. A leaf.
void Open(uint8_t* base, uint32_t member);
// sub_82250900 Leave (r4 the mode): state 7, UiMovies::Delete(9), the
// member's slot 1, state 7 again. The slot's r3. 112-byte frame.
uint64_t Leave(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_822502C0 OnLeft: the three blocks VirtualFree'd, the globals 0. The
// last VirtualFree's r3. 112-byte frame.
uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The pages (pages.cpp; r3 the member, r4 the mode) ---
//
// sub_82250948 MenuPage: the menu stepped; by the record's tab, buttons 158
// and 159 (below 3) or 158 and 163 (3 to 5): one pressed starts step 1
// (the close) for page 1 and calls the member's slot 2. The last call's
// r3. 112-byte frame.
uint64_t MenuPage(uint8_t* base, uint64_t member, uint64_t mode, uint32_t sp, const Calls& calls);
// sub_82250348 ClosedPage: the data's +4 = 9, then a tail call to the
// member's slot 3 (with the caller's lr `lr`). The slot's r3. A leaf.
uint64_t ClosedPage(uint8_t* base, uint64_t member, uint64_t mode, uint32_t sp, uint32_t lr,
                    const Calls& calls);
// sub_82250A20 Draw: the page from its table (112-byte frame), r3 the member
// plus the entry's adjust. The page's r3, or `r3`.
uint64_t Draw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls);

// --- The steps (steps.cpp; r3 the member, r4 the started word's address) ---
//
// sub_82251320 Step0 (the open animation): the first call SetTexts and
// UiMovie::Play(animation 0); then UiMovie::Run; done: the shown word 1.
// Returns 1 when done. 128-byte frame.
uint64_t Step0(uint8_t* base, uint64_t member, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82250AA8 Step1 (the close): the first call UiMovie::Play(animation
// 1) and the shown word 0; then UiMovie::Run. Returns 1 when done. 112-byte
// frame.
uint64_t Step1(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_82250DD8 SetTexts: movie 9's texts from the record (SetText's unset
// varargs passed as 0: the user's call of 2026-10-30), node 3 shown by the
// tab, then SetParts for tabs 2 and 5. The last call's r3. 128-byte frame.
uint64_t SetTexts(uint8_t* base, uint64_t member, uint32_t sp, const Calls& calls);
// sub_82250CD0 SetParts (r4 the record's +60): its bytes sign-extended into
// two arrays on its frame, then SetPartIcon, SetPartRow2 and SetPartRow1.
// SetPartRow1's r3. 144-byte frame.
uint64_t SetParts(uint8_t* base, uint32_t parts, uint32_t sp);
// sub_82250B48 SetPartIcon: node (3, 0)'s +108 = `value`. The node.
// 112-byte frame.
uint64_t SetPartIcon(uint8_t* base, uint32_t value, uint32_t sp);
// sub_82250BA0 SetPartRow2 and sub_82250C40 SetPartRow1 (r4 a word array):
// group node (3, 2) or (3, 1)'s children shown with the array's values, or
// hidden (10; 37 and above). The last node found. 144-byte frames.
uint64_t SetPartRow2(uint8_t* base, uint32_t values, uint32_t sp);
uint64_t SetPartRow1(uint8_t* base, uint32_t values, uint32_t sp);
// sub_822513B8 Update: the step from its table (144-byte frame), or
// UiMovie::Run; then the queued state. The step's, Run's r3 or the movie.
uint64_t Update(uint8_t* base, uint64_t member, uint32_t sp, const Calls& calls);

}  // namespace power_graph_screen
