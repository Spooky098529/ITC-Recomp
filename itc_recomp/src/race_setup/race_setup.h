// The race's set-up as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/race-setup.md, the layouts in
// docs/known-data.md: "car object", "the race data", "stage list", "course
// route", "route", "course record", "event"). The car objects, the racer
// records and the race data are guest data shared with the recompiled code
// (the garage, the front end, the race), so these work on guest memory, with
// the originals' loads and stores in their order and their float steps as
// the translated code does them (src/car_physics/float_ops.h). The
// originals' register saves and scratch below the stack pointer aren't
// written: nothing reads them after the return (as the post effects' and the
// course route's). Flush-to-zero off on entry (the adapters' job). Built
// with -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).
// Adapters: src/hooks/race_setup.cpp.
//
// A car object (at least 1,888 bytes; a racer record is one, 3,936 bytes,
// with the race's words at +3888 to +3924): +0 to +11 the paint (halfwords
// +0, +2, +4, +6, +8, bytes +10, +11), +12 the car id, +16 the settings
// (nine halfwords, then the setting bytes +72 to +94), +96 six 32-byte
// decals, +288 forty more, +1568 20 bytes (bytes; what they are isn't
// known), +1588 three 20-byte entries, +1648 the tuning (240 bytes).

#pragma once

#include <cstdint>

namespace race_setup {

// The image's floats the originals load with lfs.
constexpr uint32_t kOne = 0x82000A30;      // 1.0
constexpr uint32_t kZero = 0x82000A34;     // 0.0
constexpr uint32_t kHundred = 0x82002A0C;  // 100.0
constexpr uint32_t kTwenty = 0x82002568;   // 20.0
constexpr uint32_t kTorqueScale = 0x8200222C;  // not read
constexpr uint32_t kGearScale = 0x82002204;    // not read

// The game's rand seed (race_overlays::Rand).
constexpr uint32_t kRandSeed = 0x82638188;

// --- The car object's parts (src/race_setup/car.cpp) ---

// Car::ResetDecals40 (sub_820CC4A0): forty 32-byte decal entries from
// `decals` (a car object's +288): +0, +4 = 1.0, +8, +12, +16 = 0.0,
// halfword +20 and bytes +22, +23 = 0, +24 = -1, +28 = 0.
void ResetDecals40(uint8_t* base, uint32_t decals);
// Car::ResetDecals6 (sub_820CCBF0): six such entries (a car object's +96).
void ResetDecals6(uint8_t* base, uint32_t decals);
// Car::ResetSettings (sub_820CD760): a car object's +16: bytes +56 to +59 =
// 2, +60 = 3, +61 = 7, +62 to +76 = 0, +77 = 10, +78 = 0, then nine
// halfwords from +0 = 0.
void ResetSettings(uint8_t* base, uint32_t settings);
// sub_820CCC70: a car object's +1568 (20 bytes): word +16 = 0, bytes +0,
// +1, +12 = 0, +2 = 5, +3, +4 = 0, +5 = 1, +6 = 2, +7 = 1, +8 to +11 = 2.
void Reset1568(uint8_t* base, uint32_t bytes);
// CarTuning::Reset (sub_820CD618): a car object's +1648 (240 bytes): +0 =
// 0, +4 = (flag != 0), +8 = 0, +12 = (+4 == 1), +16 to +164 = 0, +168 = 3,
// +172 = 0, five 12-byte entries from +176 = (1, 0, 0).
void TuningReset(uint8_t* base, uint32_t tuning, uint32_t flag);
// CarDecal::FromBuild (sub_820CDC18, with its setters sub_820CC458,
// sub_820CC460, sub_820CC468, sub_820CC470, sub_820CC478 and sub_820CC450
// merged): a 32-byte decal entry from a car build's 36-byte one: +20 =
// halfword build +22 (the word +20's low half) << 16 | byte +27 << 8 | byte
// +31; +8, +12 = build +8, +12; when build +0 is below 0.0, +0 = -(it) and
// +28 = 1, else +28 = 0 (+0 left); +4 = build +4; +16 = build +16
// (Object344::SetFloat16's plain C++); +24 = build +32 (sub_820E70B8's).
void DecalFromBuild(uint8_t* base, uint32_t decal, uint32_t build);
// Car::Clear (sub_820CD7F0): +12 = 0; CarTuning::Reset(+1648, 0); halfwords
// +2, +0, +6, +8 = 0, byte +10 = 0, halfword +4 = -1, byte +11 = 0;
// Car::ResetSettings(+16), Car::ResetDecals40(+288), Car::ResetDecals6(+96),
// sub_820CCC70(+1568); three 20-byte entries from +1588: +8, +12, +16 =
// 0.0, +0, +4 = 0. Returns the car + 1568 (the original's r3).
uint32_t CarClear(uint8_t* base, uint32_t car);
// Car::Init (sub_820CD5D8): Car::Clear, then +3888 = 0 (the flags), +3892 =
// 1. Returns Car::Clear's r3.
uint32_t CarInit(uint8_t* base, uint32_t car);

// Car::SetModel (sub_820CD8A8): +12 = the car id; unless Params::Exists is
// 0 (then returns 0), from its spec (Params::GetCarSpec): CarTuning::Reset(
// +1648, spec byte +709 != 0); +36 = the final drive (spec +372), +40 to +68
// eight gear ratios (+388); setting bytes +76, +77 = spec +632, +636, +74,
// +75 = +800, +808, +72 = +796 then +804 (+72 written twice, +73 left);
// with the drive type (+356) neither 0 nor 1, +92 = 1 and +93 = the low byte
// of fctiwz((1.0 - spec +364) x [0x82002A0C] x [0x82002568] x [0x8200222C])
// (each step rounded to single), else +92 = 0; the tuning words +1676,
// +1796, +1684, +1680, +1700, +1704 from spec +772 to +792; the three +1588
// entries from spec +812 (+8 to +16 converted from integers to floats); the
// tuning words +1716, +1712, +1724, +1720 from spec +884 to +896; the five
// +1824 entries from spec +900 (+8 = the word, +0 = the next / 3 + 1, +4 =
// it % 3); +1570, +1571 = 5, 0 when spec +648 <= 2000, +716 <= 4700, +720 <=
// 2000 and +724 <= 1700 (unsigned), else 3, 3; +1572 = 0; at random (the
// game's rand): +1569 = 0 to 13, +1573 = 1 to 10, +1574 and +1575 = 1 to 10
// after a 10, else 1 to 9, +1576 = 1 to 9; +1580 = 1 for cars 56, 64 to 68
// and 70. Returns the last rand() (the original's r3).
uint32_t SetModel(uint8_t* base, uint32_t car, uint32_t id);
// Car::FromBuild (sub_820CDCE0): a car object from a car build (an item's
// +160, at least 2,812 bytes): Car::Clear; +12 = build +2788 and
// Car::SetModel with it; the paint (halfwords +0, +2 = build +2792, +8 =
// +2800, bytes +10, +11 = +2804, +2808, halfword +6 = +2796); the decals
// (CarDecal::FromBuild): +96 + 32k from +392 + 36k and +192 + 32k from +500
// + 36k (k < 3), then 40 from +288 by groups of five from +608, +968,
// +1328, +1688, +1868, +2048, +2408, +2588; the final drive and the first
// six gear ratios (+36 to +60) each x fma(build +4i, [0x82002204], 1.0)
// (rounded to single, then the product) unless the build's float is 0.0;
// setting bytes +72 to +94 from build words +28 to +116; +1568 (20 bytes)
// from +2768; the three +1588 entries from +332 (+8 to +16 converted); the
// tuning words +1656 to +1812 from +120 to +268; the wheel numbers +1728,
// +1736 = Params::FirstWheelOfModel(build +232 / 10) + build +232 % 10 (and
// +240), +1732, +1740 = +236, +244. Returns the last FirstWheelOfModel (the
// original's r3).
uint32_t FromBuild(uint8_t* base, uint32_t car, uint32_t build);

}  // namespace race_setup
