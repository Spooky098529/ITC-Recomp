// The type 8 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/type8-mode.md, the fields in
// docs/known-data.md: "type 8 mode", "type 8 first screen and dialog"): the
// mode's slots, its +32 interface, its first screen (Type8Screen0) and its
// dialog (Type8Dialog). Plain C++ over guest memory (the mode, its member
// and machines are shared with the recompiled code), with the originals'
// loads and stores in their order. Each function takes the stack pointer the
// original is entered with (`sp`); the back chain of an original's frame is
// written as it leaves it, its register saves aren't. Guest calls go through
// type67_modes::Calls (hooks::Type67Calls), by address to the callees'
// overrides. Each returns r3 as the original leaves it. Adapters:
// src/hooks/type8_mode.cpp (behind --native_type8_mode), register steps
// shared with the harness in src/hooks/type8_mode.h.

#pragma once

#include "type67_modes/type67_modes.h"

#include <cstdint>

namespace type8_mode {

using Calls = type67_modes::Calls;

// The asking player the confirms set (Type8Dialog::OpenConfirm reads it).
constexpr uint32_t kDialogPlayer = 0x82814E6C;

// --- The dialog (dialog.cpp): r3 the dialog (the member's +9792, its
// machine at +0: the state +9604); r4 the dispatcher where read ---

// sub_8223A408 Type8Dialog::OnEnter (a leaf): its words reset, the state 1
// when +9612 is set, else 9. r3 left.
uint64_t DialogOnEnter(uint8_t* base, uint64_t r3);
// sub_8223A398 Type8Dialog::OpenMovies (96-byte frame): UiMovies::Open of
// movies 16 to 19 (assets 22559 to 22562) and 20 (22587). Open's r3.
uint64_t DialogOpenMovies(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8223ADC0 Type8Dialog::WaitForMovies (96): with +9608 0, it = 1 once
// movies 16 to 20 are loaded (UiMovies::IsLoaded's plain C++, stopping at
// the first not); else +9736 = 1 and the state 9. r3 as left.
uint64_t DialogWaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223AC00 Type8Dialog::Draw (96): the page (+9640)'s handler from the
// two-entry table in the frame (ConfirmPage, NoticePage; a guest call with
// r3 and r4), nothing for page -1. The page's r3, or r3 left.
uint64_t DialogDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_8223AC58 Type8Dialog::Update (160): as Type6Dialog::Update with its
// own step 0 (OpenConfirmStep) and the closed message 13, no shown word.
uint64_t DialogUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                      const Calls& calls);
// sub_8223A510 Type8Dialog::OpenConfirm (112): movie 16 (+9744) reset
// (UiMovie::ResetPose), the asking player to +9732, the cursors, the pages
// cleared, state 4 through step 0 when loaded, else 1. ResetPose's r3.
uint64_t DialogOpenConfirm(uint8_t* base, uint32_t dialog, uint32_t sp);
// sub_8223A4A0 Type8Dialog::OpenNotice (a leaf): movie 19, page 1 through
// step 2 when loaded (state 4), else state 1. r3 left.
uint64_t DialogOpenNotice(uint8_t* base, uint64_t r3);
// sub_8223A5F0 Type8Dialog::DrawWaiting (128): with a reopen queued
// (+9748), +9748 = 0 and OpenConfirm's work without the player; ResetPose's
// r3, else r3 left.
uint64_t DialogDrawWaiting(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223A6B8 Type8Dialog::ConfirmPage (112): Menu::Step and Pressed's
// plain C++ on +9668; accept (159): MenuSound 0, the close step 1, message 9
// (cursor 0) or 10; back (158): MenuSound 2, step 1, message 12; moved
// (+9676): MenuSound 4, the cursor step 4. MenuSound's r3, else Pressed's.
uint64_t DialogConfirmPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                           const Calls& calls);
// sub_8223A858 Type8Dialog::NoticePage (112): Menu::Step; on 159 or 158
// (Pressed(159) asked again), MenuSound 0 or 2, step 3 with page 1 next,
// message 11. MenuSound's r3, else Pressed's.
uint64_t DialogNoticePage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                          const Calls& calls);
// sub_8223A940 Type8Dialog::OpenConfirmStep (128): Dialog::OpenConfirmStep
// with a UiMovie::ResetPose before the cursor's highlight. 1 when done,
// else 0.
uint64_t DialogOpenConfirmStep(uint8_t* base, uint32_t dialog, uint32_t sp, const Calls& calls);

}  // namespace type8_mode
