// The tuning screen's machine natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/tuning-machine.md, the screen's fields in
// docs/known-data.md's "tuning screen"): the type 2 mode's tuning screen
// machine (the mode block's +19792; the machine is the screen itself, its
// +9604 the state), its Init and the handlers of its transitions. Plain C++
// over guest memory (the screen, the mode's message ring and the movies are
// shared with the recompiled code), with the originals' loads and stores in
// their order and their frames' words. `sp` is the caller's stack pointer:
// the back chain of the original's frame and its locals are written below
// it as the original leaves them; its register saves aren't. Adapters:
// src/hooks/tuning_machine.cpp (behind --native_tuning_machine), register
// steps shared with the harness in src/hooks/tuning_machine.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0, or all eight) by address, to the callees' overrides.
// UiMovies::Find and IsLoaded are their plain C++. The varargs
// UiMovie::SetText calls pass 0 for the arguments the call sites leave unset
// (the user's call for the parking area, 2026-10-30: only dead words of the
// caller's parameter area differ).

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace tuning_machine {

using Calls = race_hud_object::Calls;

// The machine's (the screen's) words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The screen's blocks: the command queue (28 bytes), the category menu and
// a second menu (68 bytes each), VirtualAlloc'ed by Create; the decal
// images at +10048.
constexpr uint32_t kQueue = 10036;
constexpr uint32_t kMenu = 10040;
constexpr uint32_t kMenu2 = 10044;
constexpr uint32_t kImages = 10048;

// The movies: the shop, the vinyl editor, the sticker editor; the type 2
// mode's yes/no dialog.
constexpr uint32_t kShopMovie = 2;
constexpr uint32_t kVinylMovie = 13;
constexpr uint32_t kStickerMovie = 14;
constexpr uint32_t kDialogMovie = 16;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kUnloadAll = 0x82208738;  // DecalImages::UnloadAll

// --- The machine's handlers (handlers.cpp) ---
//
// sub_821FE540 TuningMachine::ZeroRecord16 (r4 a 16-byte record): +0, +4 =
// 0, +8, +12 = the float word [0x82000A34]. A leaf (r3 left).
void ZeroRecord16(uint8_t* base, uint32_t record);
// sub_821FE590 DecalImages::Reset (r3 the decal images): the 200 36-byte
// entries' +0 = -1, +4 to +32 = 0; +8852 = 0. A leaf (r3 left).
void ResetImages(uint8_t* base, uint32_t images);
// sub_821CE4C8 TuningMachine::Create (r3 the screen): the screen's words
// reset (its row), ZeroRecord16 twice, the command queue and the two menus
// VirtualAlloc'ed (unchecked) and their first words, DecalImages::Reset;
// +9604 = 1 or 9 by +9612. r3 the decal images (Reset leaves it; all 64
// bits from `r3`). 144-byte frame (the screen also kept at sp + 20).
uint64_t Create(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_821CBDB0 TuningMachine::OpenMovies (r3 the screen): UiMovies::Open
// of 2, 13 and 14 (assets 22547 to 22549); +9608 = 0. The last Open's r3.
// 96-byte frame.
uint64_t OpenMovies(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls);
// sub_821CE9E8 TuningMachine::WaitForMovies (r3 the screen): while +9608
// is 0, IsLoaded of 2, 13 and 14 (stopping at the first not loaded), +9608
// = 1 once all are; with +9608 set, +9708 = 1 and +9604 = 9 or 4 by +9612.
// r3 the last IsLoaded's when called, else `r3`. 112-byte frame.
uint64_t WaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821D8898 TuningMachine::OnDialogClosed (r3 the screen, r4 the mode):
// in state 11 (+9652), movie 16's texts (SetText(-1, 0, 15), (1, 3, 2), (1,
// 4, 3); their unset varargs 0), its +40960 = 0 and message 34 posted;
// always +9728 = 1. The last SetText's r3, else `r3`. 128-byte frame.
uint64_t OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                        const Calls& calls);
// sub_821D69A0 TuningMachine::Unload (r3 the screen): with +9608,
// UiMovie::FinishAll(movie 2); with +9616, UiMovies::Delete of 2, 13 and 14
// and +9608 = 0. The last call's r3, else `r3`. 112-byte frame.
uint64_t Unload(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_821D6A78 TuningMachine::Close (r3 the machine, r4 the mode): +9604 =
// 7, message 4 posted. A leaf (r3 left).
void Close(uint8_t* base, uint32_t machine, uint32_t mode);
// sub_821CBD40 TuningMachine::Destroy (r3 the screen):
// DecalImages::UnloadAll(+10048), VirtualFree of the queue and the two
// menus. The last VirtualFree's r3. 112-byte frame.
uint64_t Destroy(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_821F8950 DecalImages::SavePlaceholders14 (r3 the decal images): movie
// 14's placeholder textures ([[[m +28] +640]], [[[m +28] +636]]) into
// +8844 and +8848. r3 Find's (the pair's address). 112-byte frame.
uint32_t SavePlaceholders14(uint8_t* base, uint32_t images, uint32_t sp);
// sub_82208130 DecalImages::SavePlaceholders (r3 the decal images): movie
// 13's ([[[m +28] +724]], [[[m +28] +700]]) into +8836 and +8840, then
// SavePlaceholders14. Its r3. 112-byte frame.
uint32_t SavePlaceholders(uint8_t* base, uint32_t images, uint32_t sp);

// --- The machine's set-up (init.cpp) ---
//
// sub_821E7568 TuningMachine::Init: 17 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its frame
// (`frame`, the original's stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitFrame = 1232;
uint32_t Init(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

// --- The screen's set-up's callees (set_up.cpp) ---
//
// sub_821DE280 TuningScreen::LoadInteriorSlots (r3 the screen): when the
// selected garage car's id (its +12) is 10, +25192 = 0 (the interior's
// fifth slot unavailable). A leaf, no frame (r3 left).
void LoadInteriorSlots(uint8_t* base, uint32_t screen);
// sub_821DA4B0 TuningScreen::LoadCurrentChoices (r3 the screen): the
// selected garage car's choices into the screen's current choice words (the
// inverse of TuningScreen::StoreToGarageCar), the paint from the car or its
// spec (Params::GetCarSpec's plain C++), the vinyl layers' angles (the
// float words times [0x82002244], truncated), the mirror and sticker tables
// in its frame (+80 to +108) read unchecked by the car's words. 192-byte
// frame. `r1` the caller's (all 64 bits); r3 as the original leaves it: the
// frame's +80 (r1's high word kept).
constexpr uint32_t kLoadCurrentChoicesFrame = 192;
uint64_t LoadCurrentChoices(uint8_t* base, uint32_t screen, uint64_t r1);

// --- The owned parts (owned_parts.cpp) ---
//
// sub_821DBAB0 TuningScreen::LoadOwnedParts (r3 the screen): the screen's
// owned words, each 1 or 0 from its bit in the selected garage car's
// bitset (its +1904) or the save data's (+0x1996C), the inverse of
// TuningScreen::StoreOwnedParts (tuning_pages.h; its row): the power items
// by the part set [+33900] (none written outside 0 to 4), tire 0 and a few
// words always 1, the interior's by [+25172], the wheels' flags cleared and
// set through TestAndSetWheelFlag's plain C++, the interior item 3's rows
// cleared and set by the {3, 2, 1, 0} table it keeps at frame + 80.
// 160-byte frame (the back chain and the table written). r3: the last
// TestAndSetWheelFlag's, else 35960 (a leftover of the tires' loop).
constexpr uint32_t kLoadOwnedPartsFrame = 160;
uint64_t LoadOwnedParts(uint8_t* base, uint32_t screen, uint32_t sp);

// --- The prices (prices.cpp) ---
//
// sub_821DAD58 TuningScreen::SetPrices (r3 the screen): the parts' prices
// from the selected garage car's price (its +1900): converted to single
// (fcfid, frsp), times rodata floats (fmuls), truncated (fctiwz) into the
// part set's rows (+33904 + 40 x [+33900] and +34524 + 28 x [+33900], the
// part set read again for each store, unchecked; the car's price itself at
// +33916 + 40 x set) and the other items' rows (+34964 to +36792, +25248,
// +25400 to +27831 and +33584 to +33719, with fixed words 0, 50, 100 and
// 200); +36280 0 when +25172 is set. No frame: the conversions' words left
// below `sp` (sp - 160 to sp - 137) as the original leaves them; its
// register saves (r17 on, sp - 128) aren't written. A leaf (r3 left).
void SetPrices(uint8_t* base, uint32_t screen, uint32_t sp);

}  // namespace tuning_machine
