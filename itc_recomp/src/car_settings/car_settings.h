// The car settings screen natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/car-settings.md): the machine the type 2
// front-end mode sets up at its block's +0x173D0 (and the type 5 mode at its
// +848 block's +0), UI movie 3, where the selected garage car's settings are
// adjusted page by page. Plain C++ over guest memory (the machine, its three
// blocks, the mode's message ring and the movies are shared with the
// recompiled code), with the originals' loads and stores in their order and
// their frames' words. `sp` is the caller's stack pointer: the back chain of
// the original's frame and its locals are written below it as the original
// leaves them; its register saves aren't. Adapters: src/hooks/car_settings.cpp
// (behind --native_car_settings), register steps shared with the harness in
// src/hooks/car_settings.h.
//
// Only the handlers seen in play are here (the two Inits, Create, OpenMovie,
// WaitForMovie, Close, Destroy); the machine's other functors (Start, Draw,
// Update, the replies, Unload, Leave, the re-entries) were never seen in play
// and stay recompiled: the Inits add them as functors, as the original does.
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0) by address, to the callees' overrides. UiMovies::Find
// and UiMovies::IsLoaded are their plain C++.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace car_settings {

using Calls = race_hud_object::Calls;

// The blocks (globals, VirtualAlloc'ed by Create, freed by Destroy): the page
// object (28 bytes), the menu (68: a front-end menu) and the screen's data
// (764).
constexpr uint32_t kPages = 0x826D8458;
constexpr uint32_t kMenu = 0x826D845C;
constexpr uint32_t kData = 0x826D8460;
constexpr uint32_t kPagesBytes = 28;
constexpr uint32_t kMenuBytes = 68;
constexpr uint32_t kDataBytes = 764;

constexpr uint32_t kMovie = 3;
constexpr uint32_t kAsset = 22554;

// The machine's words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;

// The current mode's type (2 or 5 post Close's message).
constexpr uint32_t kModeType = 0x826032DC;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kFinishAll = 0x8211DFF8;

// --- The machine's handlers (handlers.cpp) ---
//
// sub_821AA2F8 CarSettings::Create ((0, 1); r3 the machine): +9608 = 0, the
// three blocks VirtualAlloc'ed with their first words, state 1 or 9. The
// last VirtualAlloc's r3. 112-byte frame.
uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_821A8DD0 CarSettings::OpenMovie ((1, 1) -> 2): UiMovies::Open(3,
// 22554) as a tail call (no frame: the caller's sp and lr). Open's r3.
uint64_t OpenMovie(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_821AA490 CarSettings::WaitForMovie ((2, 1)): +9608 once movie 3 is
// loaded, then state 9 or 4. IsLoaded's r3 when called, else `r3`. 96-byte
// frame.
uint64_t WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821AAF70 CarSettings::Close ((5, 4), (9, 4), (10, 4); r3 the machine,
// r4 the mode): state 7; movie 3, when there, UiMovie::FinishAll and
// UiMovies::Delete(3); message 4 posted on the mode's ring for mode types 2
// and 5. Delete's r3, or 0 (the missing movie). 112-byte frame (+80 the
// movie's id).
uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_821AA408 CarSettings::Destroy ((7, 4) -> 6; r3 the machine): state 11
// when the data's +576 is 1 (a test run set up), else 6; the three blocks
// VirtualFree'd and their globals 0. The last VirtualFree's r3. 128-byte
// frame.
uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);

// --- The machine's set-ups (init.cpp) ---
//
// sub_821B6FC0 CarSettings::Init (the type 2 mode's) and sub_821B7878
// CarSettings::InitType5 (the type 5 mode's): 18 and 17 transitions added to
// `machine` (StateMachine::Add's plain C++) from functors built in its frame
// (`frame`, the original's stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitFrame = 1296;
constexpr uint32_t kInitType5Frame = 1232;
uint32_t Init(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t InitType5(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace car_settings
