// The race's HUD object (RaceHud::, at +16 of the race screens' holder
// [0x826D84F0]) as plain C++ (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/race-hud-object.md): its gauges, the dashboard's
// nodes, the signs and the racers' bars. The HUD, its UI movies and the
// vehicles are guest objects shared with the recompiled code, so these work
// on guest memory, with the originals' loads and stores in their order and
// their float steps as the translated code's (camera_modes/steps.h), with
// flush-to-zero off (the adapters' disableFlushMode). `sp` is the caller's
// stack pointer: the back chain of the original's frame and its stack
// temporaries are written there as the original leaves them; its register
// saves aren't. Adapters: src/hooks/race_hud_object.cpp.

#pragma once

#include <cstdint>

namespace race_hud_object {

// The HUD's fields.
constexpr uint32_t kHudPlayer = 16;     // the player's index
constexpr uint32_t kHudVehicle = 440;   // Vehicle::Get(+16)
constexpr uint32_t kHudDashboard = 608; // the dashboard movie (id 0)

// The float 0.0 at 0x82000A34 the originals load.
constexpr uint32_t kZero = 0x82000A34;

// --- The gauges' helpers (gauges.cpp) ---

// HudCurve::Eval (sub_82212118): +96 = x (stfs); x above +88 or below +92
// puts that bound at +96 and returns (+100 left); else +100 = y linearly
// between the two points (+0 on, 8 bytes each) around x among the +108
// points, or 0.0 when none holds it.
void CurveEval(uint8_t* base, uint32_t curve, double x);
// PowInt (sub_82213C40): x to the n (doubles, by squaring from the double
// 1.0 at 0x82002250; 1 / x^-n below 0). f1.
double PowInt(uint8_t* base, double x, int32_t n);
// HudBar::Step (sub_82211FB8): the bar's value (+0) and trail (+8) moved
// toward `target` (+24 = it as a float) by at most their steps (+4, +12),
// clamped to [+16, +20]; +28 and +36 as the original sets them. The
// original's conversions leave the two directions at sp - 16 and sp - 8
// (doublewords).
void BarStep(uint8_t* base, uint32_t bar, int32_t target, uint32_t sp);

// --- The dashboard's setters (gauges.cpp) ---
//
// Each returns r3 as the original leaves it.

// RaceHud::SetGear (sub_82212DB0): node 11's picture (+108) by the gear: 7
// for 6, gear - 1 for 1 to 5, 5 for 0, 6 below 0. The node.
uint32_t SetGear(uint8_t* base, uint32_t hud, int32_t gear, uint32_t sp);
// RaceHud::SetBar (sub_82214698): +404 = value; node 35 shown when the value
// is above +412, then its x scale (+28) = +404 / (+408 - +412), else hidden.
// The node.
uint32_t SetBar(uint8_t* base, uint32_t hud, double value, uint32_t sp);
// sub_82212668 (RaceHud::ResetBar): +408 and +404 = the car's float +2420,
// +412 = 0.0 (r3 left: the HUD).
void ResetBar(uint8_t* base, uint32_t hud);
// sub_822122A0 (RaceHud::ClearWarning): +468 = 0.
void ClearWarning(uint8_t* base, uint32_t hud);
// RaceHud::SetPlayer (sub_82212690): +16 = player, +440 = Vehicle::Get of
// it. The vehicle.
uint32_t SetPlayer(uint8_t* base, uint32_t hud, uint32_t player, uint32_t sp);
// RaceHud::Touching (sub_82215820): 1 when the player's vehicle has a
// collision object (+340) with a contact of mask 0x07000000, else 0.
uint32_t Touching(uint8_t* base, uint32_t player, uint32_t sp);

}  // namespace race_hud_object
