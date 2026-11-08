// The car dealer natively (B7 in docs/rexglue-removal.md; the functions are
// in docs/functions/car-dealer.md, the globals in docs/known-data.md): the
// type 2 mode's screen machine at its block's +0x19978 where the player buys
// a car. Plain C++ over guest memory (the machine, the page object, the
// menus, the dealer's data and the movies are shared with the recompiled
// code), with the originals' loads and stores in their order and their
// frames' words. `sp` is the caller's stack pointer: the back chain of the
// original's frame and its locals are written below it as the original
// leaves them; its register saves aren't. Adapters: src/hooks/car_dealer.cpp
// (behind --native_car_dealer), register steps shared with the harness in
// src/hooks/car_dealer.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0) by address, to the callees' overrides. UiMovies::Find,
// UiMovie::FindNode and UiMovie::Child are their plain C++. The varargs
// UiMovie::SetText calls pass 0 for the arguments the call sites leave unset
// (the user's call for the parking area, 2026-10-30: only dead words of the
// caller's parameter area differ).

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace car_dealer {

using Calls = race_hud_object::Calls;

// The globals: the stand's holder ([0x826D83E0]: +0 the garage stage), the
// page object, the two menus, the dealer's data, the spec sheet's object
// ([0x826D8438], another screen's: CheckLimits' words).
constexpr uint32_t kStandHolder = 0x826D83E0;
constexpr uint32_t kPages = 0x826D83E8;
constexpr uint32_t kListMenu = 0x826D83EC;
constexpr uint32_t kColorMenu = 0x826D83F0;
constexpr uint32_t kData = 0x826D83F4;
constexpr uint32_t kLimits = 0x826D8438;

// The machine's words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kScrollText = 0x8211C570;
constexpr uint32_t kScrollReset = 0x8211C5F8;
constexpr uint32_t kCarModelsBlockOf = 0x8215AAD0;
constexpr uint32_t kStageReloadModel = 0x8217F880;

// --- The machine's set-up (init_states.cpp) ---

// sub_82185920 CarDealer::InitStates: the 23 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its 1,632-byte
// frame (`frame`, the original's stores in order). Returns the last Add's
// r3 (an index when `replaced`, else the new transition).
constexpr uint32_t kInitStatesFrame = 1632;
uint32_t InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

// --- The handlers and small helpers (handlers.cpp) ---

// sub_82180D68 CarDealer::Create (r3 the machine): +9608 = 0; the page
// object (28 bytes), the two menus (72 each) and the data (1,196)
// VirtualAlloc'ed into their globals with their first words; +9604 = 1 or
// 9 by +9612. The last VirtualAlloc's r3. 112-byte frame.
uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82180C08 CarDealer::OpenMovie: UiMovies::Open(4, 22555) (a tail
// call). Its r3.
uint64_t OpenMovie(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_82180F20 CarDealer::WaitForLoad (r3 the machine): +9608 = 1 once
// movie 4 is loaded, then +9604 = 9 or 4 by +9612. r3 IsLoaded's when
// called, else the machine (`r3`). 96-byte frame.
uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82181B98 CarDealer::Page4 (r3 the mode): message 9 posted on its ring.
void Page4(uint8_t* base, uint32_t mode);
// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id);
// sub_82180C48 CarDealer::AfterDetail: with the data's +12 0, transition 20
// (list +44 0) or transitions 10 then 11 to page 2.
void AfterDetail(uint8_t* base);
// sub_821812B8 CarDealer::CheckLimits: [0x826D8438] +48 = 1, +44 by the
// selected car's spec.
void CheckLimits(uint8_t* base);
// sub_82181330 CarDealer::ReloadStandModel: GarageStage::ReloadModel(stage,
// CarModels::BlockOf(the selected car's id)) into the data's +32, the color
// (+56, +60) 0. ReloadModel's r3. 96-byte frame.
uint64_t ReloadStandModel(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The movie 4 setters (texts.cpp) ---

// sub_82182158 CarDealer::SetKindBar and sub_82182770 SetColorBar: node
// (8, 0) or (26, 0) +16 = `value` as a float (its 64-bit integer left at
// the frame's +80). The node (r3). 112-byte frames.
uint64_t SetKindBar(uint8_t* base, uint64_t value, uint32_t sp);
uint64_t SetColorBar(uint8_t* base, uint64_t value, uint32_t sp);
// sub_82182DD0 CarDealer::SetScrollArrows: (7, 3) and (7, 4) shown by the
// scroll and the count. The second node (r3). 128-byte frame.
uint64_t SetScrollArrows(uint8_t* base, uint64_t scroll, uint64_t count, uint32_t sp);
// sub_82182C28 CarDealer::SetHeader: (2, 1) and (2, 2) +108 = `a`, `b`.
// The second node (r3). 128-byte frame.
uint64_t SetHeader(uint8_t* base, uint32_t a, uint32_t b, uint32_t sp);
// sub_82182B30 CarDealer::SetPrompts: sub-scene 16's three prompts by the
// pairs (r3, r4), (r5, r6), (r7, r8). The last FindNode's r3. 176-byte
// frame.
uint64_t SetPrompts(uint8_t* base, const uint32_t args[6], uint32_t sp);
// sub_82199B38 HelpLine::Set: movie 4's (17, 0) text string `id`
// (UiMovie::SetText, its unset varargs 0), ScrollText(17, 0, 1) and the
// scroll reset sub_8211C5F8. The reset's r3. 112-byte frame.
uint64_t HelpLineSet(uint8_t* base, uint64_t id, uint32_t sp, const Calls& calls);

}  // namespace car_dealer
