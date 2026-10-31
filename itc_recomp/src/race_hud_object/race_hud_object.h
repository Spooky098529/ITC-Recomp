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

namespace track_zones {
struct OwnerCalls;
}

namespace race_hud_object {

// The HUD's fields.
constexpr uint32_t kHudPlayer = 16;     // the player's index
constexpr uint32_t kHudVehicle = 440;   // Vehicle::Get(+16)
constexpr uint32_t kHudDashboard = 608; // the dashboard movie (id 0)
constexpr uint32_t kHudTimer = 624;     // the timer movie (id 13)

// The two viewports (a pointer to two; +1860 the camera view).
constexpr uint32_t kViewports = 0x826C2CE8;

// The race data ([0x826C5B2C]: +4 the course, the racers' records 3,936
// bytes each from +0, +0xA3AF4 the race mode) and the track zones' owner.
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRecordBytes = 3936;
constexpr uint32_t kRaceMode = 0xA3AF4;

// The guest functions called through `Calls`: memcpy where its plain C++
// refuses, CameraPasses::RenderTarget through its override (the
// diagnostics' wrapper, as the cameras' native callers call it),
// TrackZones::Create and Destroy through their overrides (Create's plain
// C++ takes guest calls of its own; Destroy's where it refuses), and the
// extra gauges' digits, never seen in play (the originals).
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kRenderTarget = 0x8209F7A8;
constexpr uint32_t kTrackZonesCreate = 0x8228E788;
constexpr uint32_t kTrackZonesDestroy = 0x8228B500;
constexpr uint32_t kGaugeFloat = 0x82214330;
constexpr uint32_t kGaugeInt119 = 0x822144B0;
constexpr uint32_t kGaugeInt149 = 0x822145A0;
// The signs' movie calls: UiMovie::FinishNamed (its native version makes
// UiMovie::ApplyAnimation's guest call), SetText and ClearText (their
// adapters pick the plain C++ or the original by the UI movies' and the
// heap's flags) through their overrides.
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kClearText = 0x8211C470;
constexpr uint32_t kTrackZones = 0x828161EC;  // the zones' owner

// A guest function `function` called with r3 to r10 (0 where not given:
// the varargs UiMovie::SetText's unset r7 to r10 pass 0, the user's call
// for the parking area, 2026-10-30) and, when `f1` isn't null, f1 (the
// other registers as they are), from the frame `r1` with the return
// address `lr` (the original's); its r3.
struct Calls {
  void* user;
  uint64_t (*call)(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                   uint32_t r1, uint32_t lr);

  uint64_t Call(uint32_t function, uint32_t r1, uint32_t lr, uint64_t r3 = 0, uint64_t r4 = 0,
                uint64_t r5 = 0, uint64_t r6 = 0, const double* f1 = nullptr) const {
    const uint64_t args[8] = {r3, r4, r5, r6, 0, 0, 0, 0};
    return call(user, function, args, f1, r1, lr);
  }
};

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

// --- The dashboard's digits, needles and set-ups (dashboard.cpp) ---
//
// Each returns r3 as the original leaves it; `f1` (in and out) is f1 as the
// original leaves it where a call to PowInt may change it.

// RaceHud::SetTach (sub_82212D58): HudCurve::Eval(+20, value), node (0, 7)
// +36 = its output (+120) plus the float at 0x8200F720. The node.
uint32_t SetTach(uint8_t* base, uint32_t hud, double value, uint32_t sp);
// RaceHud::SetBoost (sub_82212E18): HudCurve::Eval(+132, value), node
// (0, 5) +36 = its output (+232). The node.
uint32_t SetBoost(uint8_t* base, uint32_t hud, double value, uint32_t sp);
// RaceHud::SetSpeed (sub_82214260): when `speed` is 999 or less (unsigned),
// node 9's children (from its +100, +104 of them), last first, get their
// digit + 1, or 0 for leading zeros (below PowInt(10.0, n) through
// fctiwz, at the frame's +80). The HUD (above 999), node 9 (no children) or
// the last child.
uint32_t SetSpeed(uint8_t* base, uint32_t hud, uint32_t speed, uint32_t sp, double* f1);
// RaceHud::SetTime (sub_82213750): the packed time at `time` (clamped to
// 59:59.999) into the timer movie's nodes 15/14, 17/16 and 20/19/18 (digit
// + 1). The last node.
uint32_t SetTime(uint8_t* base, uint32_t hud, uint32_t time, uint32_t sp);
// RaceHud::SetDistance (sub_82214A50): `*f1` clamped to [0.0, the float at
// 0x8200F740], its tenths rounded up (by the hundredths) into the timer
// movie's nodes 11, 10 and 9 (leading zeros 0). The last node.
uint32_t SetDistance(uint8_t* base, uint32_t hud, uint32_t sp, double* f1);
// RaceHud::Reset (sub_822121C0): node 1's children's and node 5's alpha
// (+68) 0.0, +492 = 0, +460 = -1, node 19 hidden. Node 19.
uint32_t Reset(uint8_t* base, uint32_t hud, uint32_t sp);
// RaceHud::ShowGauge (sub_82212D08): nodes (0, 12 + slot) and (0, 9 + slot)
// +80 = shown. The second.
uint32_t ShowGauge(uint8_t* base, uint32_t hud, uint32_t slot, uint32_t shown, uint32_t sp);
// RaceHud::SetUpExtraGauges (sub_82213DB0): the build's five entries (+1832,
// 12 bytes each) into +476 and three flags at the frame's +80 to +88 (their
// addresses at +96 to +104), the slots +488 to +480 the first flag set of
// the three (cleared) or -1, each slot shown (ShowGauge), node (0, 1) by +8
// and +476. Node (0, 1).
uint32_t SetUpExtraGauges(uint8_t* base, uint32_t hud, uint32_t sp);
// RaceHud::ApplyOptions (sub_82212768): four words of player 0's save data
// (+0x1F85C) to +0 to +12, then nodes 4 (when +472, read before the copy),
// 3, (0, 3), (0, 4), (0, 1) and 1 shown by them. Node 1.
uint32_t ApplyOptions(uint8_t* base, uint32_t hud, uint32_t sp);
// RaceHud::UpdateView (sub_82212B68): +432 = viewport 0's +1860, nodes 0
// and 4 and +472 by it and +12. Node 4.
uint32_t UpdateView(uint8_t* base, uint32_t hud, uint32_t sp);

// --- The set-ups and the teardown (setup.cpp) ---
//
// Each returns r3 as the original leaves it.

// RaceHud::SetUpGauges (sub_82213C90): the three tachometer curves
// (0x8200D1F8, 112 bytes each) copied to the frame's +80, +192 and +304;
// the player's racer record's +60 (an index) and +1704 (0-2 gives 0, 3-6
// 1, 7 and up 2) pick the curve (the table at 0x8200D3B8 + 16 x index, its
// word +0) copied to +20 and the dial's face (its word 4 + 4 x the kind)
// for node (0, 8) +108; the boost curve (0x8200D348) to +132; both
// evaluated at 0.0. The copies through memcpy's plain C++, or its guest
// call where that refuses. +132 (HudCurve::Eval's r3).
uint32_t SetUpGauges(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls);
// RaceHud::SetUpMirror (sub_822135B0): the swapped-out texture put back
// when +588 is set, +592 = render target 11 (through `calls`), its level
// 0's size (Texture::LevelDesc into the frame's +80) at +596 and +600,
// then +592's address swapped in as the dashboard's texture 56 and the old
// one at +588. The old one.
uint32_t SetUpMirror(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls);
// RaceHud::TearDown (sub_822141A0): TrackZones::Destroy (its plain C++, or
// its override where that refuses), then the mirror's texture put back
// when +588 is set. SwapTexture's r3, else Destroy's.
uint64_t TearDown(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls);
// RaceHud::SetUpSigns (sub_822122B0): the 24 course tables' addresses
// (0x8200D9D0 + 312 x k, in the original's order) at the frame's +80 to
// +172, +420 = the one of Race::CourseLayout (the third's for -1),
// TrackZones::Create (through `calls`), +416 and +428 = -1, +424 = 1.
// Create's r3.
uint64_t SetUpSigns(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls);
// RaceHud::UpdateExtraGauges (sub_82215160): for each slot (+480 to +488):
// 0 GaugeFloat(value in f1), 1 GaugeInt119(int119), 2 GaugeInt149(int149)
// (f1 left as it is), the others nothing; the three never ran in play:
// through `calls` (the originals). The last call's r3, or the HUD.
uint64_t UpdateExtraGauges(uint8_t* base, uint32_t hud, double value, uint64_t int119,
                           uint64_t int149, uint32_t sp, const Calls& calls);

// --- The signs (signs.cpp) ---
//
// The zones through track_zones' plain C++ (TrackZones::Update and
// IsInside), their zones' slots that aren't the native ones through
// `zone_calls`. Each returns r3 as the original leaves it.

// RaceHud::UpdateSign (sub_82212F00): see docs/functions/race-hud-object.md.
// `flag` is the original's r4 (the race flow's).
uint64_t UpdateSign(uint8_t* base, uint32_t hud, uint32_t flag, uint32_t sp, const Calls& calls,
                    const track_zones::OwnerCalls& zone_calls);
// RaceHud::UpdatePlaceName (sub_82213510): when the zone at +428 isn't
// inside any more, the first of 32 to 69 inside (or -1); when it changed,
// node 17's text string 1980 + it (UiMovie::SetText) or cleared
// (ClearText), +428 = it.
uint64_t UpdatePlaceName(uint8_t* base, uint32_t hud, uint32_t sp, const Calls& calls,
                         const track_zones::OwnerCalls& zone_calls);
// RaceHud::UpdateSigns (sub_82214718): unless the course (race data +4) is
// 8, 9 or 10 (r3 the HUD), TrackZones::Update, UpdateSign and
// UpdatePlaceName.
uint64_t UpdateSigns(uint8_t* base, uint32_t hud, uint32_t flag, uint32_t sp, const Calls& calls,
                     const track_zones::OwnerCalls& zone_calls);

}  // namespace race_hud_object
