// The alternate HUD (AltHud::, the race HUD holder's object at +1004, a
// second dashboard on UI movie 22) and the menu lists' release, as plain C++
// (B7 in docs/rexglue-removal.md; the functions are in
// docs/functions/alt-hud.md). As the race HUD object's
// (src/race_hud_object/), these work on guest memory shared with the
// recompiled code, with the originals' loads and stores in their order and
// their float steps as the translated code's (camera_modes/steps.h), with
// flush-to-zero off (the adapters' disableFlushMode). `holder` is the
// holder's +1004 (two words: +0 the object, +4 its menu list). `sp` is the
// caller's stack pointer: the back chain of the original's frame and its
// stack temporaries (the key 22 for UiMovies::Find, the conversions'
// words) are written there as the original leaves them; its register saves
// aren't. Each returns r3 as the original leaves it. Adapters:
// src/hooks/alt_hud.cpp.

#pragma once

#include "race_hud_object/race_hud_object.h"

#include <cstdint>
#include <optional>

namespace alt_hud {

using Calls = race_hud_object::Calls;

// The object's fields.
constexpr uint32_t kShown = 0;      // node 3's flag (the toggle)
constexpr uint32_t kTach = 4;       // the tachometer's curve (112 bytes)
constexpr uint32_t kBoost = 116;    // the boost's curve (112 bytes)
constexpr uint32_t kBar = 228;      // the bar's value, +232 and +236 its range
constexpr uint32_t kCar = 240;      // Vehicle::Get(+260)
constexpr uint32_t kSlots = 244;    // the three extra gauges' slots
constexpr uint32_t kPlayer = 260;   // the player

constexpr uint32_t kMovieId = 22;   // the dashboard's UI movie
// The car input records ([0x826C5B30 + 4 x player]: +4 the throttle, +6
// the brake, +8 the steering, signed halfwords).
constexpr uint32_t kInputRecords = 0x826C5B30;

// The guest functions called through `Calls`: memcpy where its plain C++
// refuses, Car::SetupVehicle where its plain C++ refuses (its override),
// UiMovie::Run (its override), operator delete where its plain C++ refuses,
// and the extra gauges' digits, never seen in play (the originals).
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kSetupVehicle = 0x820CD5C8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kOperatorDelete = 0x825A70E8;
constexpr uint32_t kGaugeFloat = 0x82216FD0;
constexpr uint32_t kGaugeInt119 = 0x82217168;
constexpr uint32_t kGaugeInt149 = 0x82217270;

// --- The menu lists (menu_list.cpp) ---

// MenuList::Release (sub_8220BBB0): when +68 (the block) is set, it freed
// (VirtualFree(block, 0, MEM_RELEASE): the allocation layer's FreePages)
// and +68 = 0, from a 96-byte frame. r3 1 after a free, else 0 (the block).
// Nullopt, with nothing written, when the block isn't the layer's pages
// (the original runs).
std::optional<uint32_t> MenuListRelease(uint8_t* base, uint32_t list, uint32_t sp);
// Whether MenuListRelease would free natively (the block 0 or the layer's).
bool MenuListReleasable(uint8_t* base, uint32_t list);
// MenuList::PressedX (sub_8220BF00): the action mapper entry of the list's
// player's controller; unless its +4 is 1, the mapper's slot 10 (+40) for
// action 160 (ActionMapper::Pressed's plain C++ when `native_slots` and
// the slot is it, else through `calls` from the 96-byte frame); 1 when
// that's true and +24 is 0, else 0.
uint32_t MenuListPressedX(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                          const Calls& calls);

// --- The set-ups (alt_hud.cpp) ---

// AltHud::PowerClass (sub_82216B08): the car's block 2 (Object::Record1888),
// a car data in the 3,040-byte frame (+1152: CarData::CarData,
// Car::SetupVehicle(car, it)), its power curve (+80: PowerCurve::FromEngine
// on the car data's +1268); 0, 1 or 2 by the curve's peak (+1056) against
// block 2's +12 times the floats at 0x820029F0 and 0x82002C40.
uint32_t PowerClass(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls);
// AltHud::SetUpGauges (sub_82216C40): the player's racer record's +60 (an
// index) and PowerClass(the record's +48) pick a tachometer curve (of three
// at 0x8200FD28, copied to the 496-byte frame's +96, +208, +320) copied to
// +4 and the dial's face for node (7, 8) +108; the boost curve (0x8200FE78)
// to +116; both evaluated at 0.0. `f1` gets the 0.0 left. +116.
uint32_t SetUpGauges(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls,
                     double* f1);
// AltHud::ShowGauge (sub_82217380): nodes (7, 12 + slot) and (7, 9 + slot)
// +80 = shown. The second.
uint32_t ShowGauge(uint8_t* base, uint32_t slot, uint32_t shown, uint32_t sp);
// AltHud::SetUpExtraGauges (sub_822178A0): the build's five entries (+1832)
// into a flag kept (type 1, or the build's +12 10) and three at the 160-byte
// frame's +80 to +88 (their addresses at +96 to +104), the slots +252 to
// +244 the first flag set of the three (cleared) or -1, each slot shown
// (ShowGauge), node (7, 1) +80 the kept flag. Node (7, 1).
uint32_t SetUpExtraGauges(uint8_t* base, uint32_t holder, uint32_t sp);
// AltHud::ShowPanels (sub_82216D78): node 3 +80 = +0, node 2 +80 = 0.
// Node 2.
uint32_t ShowPanels(uint8_t* base, uint32_t holder, uint32_t sp);
// AltHud::SetUp (sub_82217C70): +260 = player, +240 = Vehicle::Get(player),
// SetUpGauges, SetUpExtraGauges, ShowPanels. ShowPanels' r3.
uint32_t SetUp(uint8_t* base, uint32_t holder, uint32_t player, uint32_t sp, const Calls& calls,
               double* f1);

// --- The dashboard's setters (alt_hud.cpp) ---

// AltHud::SetSpeed (sub_82216E00): when `speed` is 999 or less (unsigned),
// node 21's children, last first, get their digit + 1, or 0 for leading
// zeros (PowInt(10.0, n) through fctiwz at the frame's +80). UiMovies::
// Find's (above 999), node 21 (no children) or the last child. `f1` (in and
// out) is PowInt's when it ran.
uint32_t SetSpeed(uint8_t* base, uint32_t speed, uint32_t sp, double* f1);
// AltHud::UpdateExtraGauges (sub_82217A30): for each slot (+244 to +252):
// 0 GaugeFloat(value in f1), 1 GaugeInt119(int119), 2 GaugeInt149(int149),
// the others nothing; the three through `calls` (the originals). The last
// call's r3, or the holder.
uint64_t UpdateExtraGauges(uint8_t* base, uint32_t holder, double value, uint64_t int119,
                           uint64_t int149, uint32_t sp, const Calls& calls);
// AltHud::SetGear (sub_82217460): node 23's picture: 7 for 6, gear - 1 for
// 1 to 5, 5 for 0, 6 below 0. The node.
uint32_t SetGear(uint8_t* base, int32_t gear, uint32_t sp);
// AltHud::SetTach (sub_822173E8): HudCurve::Eval(+4, value), node (7, 7)
// +36 = its output (+104) plus the float at 0x8200F720. The node.
uint32_t SetTach(uint8_t* base, uint32_t holder, double value, uint32_t sp);
// AltHud::SetNode25 (sub_822174E0): node 25 +80 = word. The node.
uint32_t SetNode25(uint8_t* base, uint32_t word, uint32_t sp);
// AltHud::SetBar (sub_822175A0): +228 = value; when above +236, node 30's
// x scale (+28) = +228 / (+232 - +236), else node 30 hidden. The node.
uint32_t SetBar(uint8_t* base, uint32_t holder, double value, uint32_t sp);
// AltHud::SetBoost (sub_82217538): HudCurve::Eval(+116, value), node (7, 5)
// +36 = its output (+216). The node.
uint32_t SetBoost(uint8_t* base, uint32_t holder, double value, uint32_t sp);
// AltHud::SetThrottle (sub_82217638) and SetBrake (sub_82217690): node 16
// or 17's x scale (+28) = value. The node.
uint32_t SetThrottle(uint8_t* base, double value, uint32_t sp);
uint32_t SetBrake(uint8_t* base, double value, uint32_t sp);
// AltHud::SetSteering (sub_822176E8): node 15's x (+12) = 188 - trunc(-value
// x the float at 0x82010368), as a float (the word and the doubleword at the
// 112-byte frame's +80). The node.
uint32_t SetSteering(uint8_t* base, double value, uint32_t sp);

// --- The tick and the rest (alt_hud.cpp) ---

// AltHud::Tick (sub_82217AC8): when MenuList::PressedX(+4), +0 flipped and
// node 3 +80 = it; then by the car (+240) and the player's input record the
// speed, the extra gauges, the gear, the tachometer, node 25, the bar, the
// boost, the throttle, the brake and the steering (their setters). 0. `f1`
// gets what the last setter leaves.
uint32_t Tick(uint8_t* base, uint32_t holder, uint32_t sp, bool native_slots, const Calls& calls,
              double* f1);
// AltHud::RunMovie (sub_82216C00): UiMovie::Run(movie 22, 0) through
// `calls` from the 96-byte frame. Run's r3.
uint64_t RunMovie(uint8_t* base, uint32_t sp, const Calls& calls);
// AltHud::Destroy (sub_82216BA8): MenuList::Release(+4), operator delete(+0),
// then when +4 isn't 0, Release on it again and operator delete(+4) (the
// deletes natively, or through `calls` where they refuse). The last
// delete's r3 (the block). Nullopt, with nothing done, when a Release
// wouldn't be native (MenuListReleasable).
std::optional<uint64_t> Destroy(uint8_t* base, uint32_t holder, uint32_t sp, const Calls& calls);

}  // namespace alt_hud
