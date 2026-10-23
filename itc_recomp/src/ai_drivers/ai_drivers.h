// The AI drivers as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/ai-drivers.md, the layouts in
// docs/known-data.md: "AI driver", "AI path", "AI corner record", "AI
// other-racer record", "AI skill"). The drivers' table ([0x826C6074], 7
// drivers of 6,160 bytes), the vehicles and the course's route are guest
// data shared with the recompiled code, so these work on guest memory, with
// the originals' loads and stores in their order and their float arithmetic
// step for step (src/car_physics/float_ops.h), so the results are the
// translated code's bit for bit. The originals' stack temporaries aren't
// written (nothing reads them after the return). Flush-to-zero off on entry
// (the adapters' job). Built with -ffp-model=strict and -ffp-contract=off
// (CMakeLists.txt). Adapters: src/hooks/ai_drivers.cpp.

#pragma once

#include <cstdint>

namespace ai_drivers {

// The drivers' table.
constexpr uint32_t kDrivers = 0x826C6074;
constexpr uint32_t kDriverBytes = 6160;

// --- Set-up and clears ---

// AiDriver::InitPath (sub_8213AD48): the route position (+4, +6) = 0, +60,
// +76, +92 = 1.0; the 80 ring entries (64 bytes from +528): +12 = 1.0, the
// halfwords +16 and +18 = 0; +6108 = 1.0.
void InitPath(uint8_t* base, uint32_t driver);
// AiDriver::Of (sub_82139728): the drivers' table + 6,160 x the word at
// `index` (64 bits, as the original's r3), 0 with no table.
uint64_t Of(uint8_t* base, uint32_t index);
// Of with the index's value (for native callers that hold it).
uint64_t OfIndex(uint8_t* base, uint32_t index);
// CarInput::Clear (sub_820C96E8): the halfwords +4 to +12, the word +0 and
// the bytes +14 to +17 = 0.
void ClearCarInput(uint8_t* base, uint32_t input);
// AiDriver::ClearFollow (sub_82146508): +12 = 500.0, the lane (+8) and the
// route position (+4) = -1, +16 = 0, the 48 bytes from +32 as ClearPlan
// sets them.
void ClearFollow(uint8_t* base, uint32_t driver);
// AiDriver::ClearOtherIndexes (sub_82144ED8): the halfword +202 of the
// seven 40-byte records from +172 = 0.
void ClearOtherIndexes(uint8_t* base, uint32_t driver);
// AiPath::ClearCorners (sub_8213C720): the corners' count (+5162) = 0 and
// the eight 44-byte records from +5164 cleared (+16 = 1.0).
void ClearCorners(uint8_t* base, uint32_t path);
// AiPath::ClearCornerKind (sub_8213C710): the record's word +40 = 0.
void ClearCornerKind(uint8_t* base, uint32_t corner);
// AiSkill::Reset (sub_8213F698): the three timed triggers (+24) and the
// seven halfwords (+4) = 0, then +20 = 100.0, +4 = 500, +6 = 80, +8 = -100.
void ResetSkill(uint8_t* base, uint32_t skill);

// --- Small reads and steps ---

// Angle::Wrap (sub_82145408): the float at `angle` above pi less 2 pi,
// below -pi plus 2 pi (once); not written otherwise.
void WrapAngle(uint8_t* base, uint32_t angle);
// RaceTime::FromTicks (sub_82101760): a tick count (30 a second) packed as
// a time: bit 31 the sign, minutes from bit 16 (15 bits), seconds from bit
// 10 (6 bits), milliseconds in the low 10 bits.
uint32_t TimeFromTicks(uint32_t ticks);
// AiDriver::SetFlag2 (sub_82138D20): +16 bit 1 set when the word at `word`
// isn't 0 (else cleared); with a vehicle (+160), the vehicle's box shape's
// halfword +114 = the word (sub_820C0450). Returns 1.
uint32_t SetFlag2(uint8_t* base, uint32_t driver, uint32_t word);
// AiDriver::IsFree (sub_821464B8): 1 when it has a vehicle without the
// flags 0x81 (or with them and +16 bit 1 set) and its state (+32) is 0 or
// 6.
uint32_t IsFree(uint8_t* base, uint32_t driver);
// AiDriver::SpeedOverDistance (sub_821450E0): the float at `distance` x
// (100.0 / the speed +100 above 100.0, else 1.0) x +136.
double SpeedOverDistance(uint8_t* base, uint32_t driver, uint32_t distance);
// AiDriver::CornerFactor (sub_821453A0): 30.0, plus 10.0 x sqrt(c - 10.0) x
// +140 x +132 when c (at `curvature`) is above 10.0, less 10.0 x (+128 - w)
// when w (at `width`) is below +128; at least 30.0.
double CornerFactor(uint8_t* base, uint32_t driver, uint32_t curvature, uint32_t width);
// AiDriver::ReadCar (sub_82144F50): the car's sizes and handling factors
// (+116 to +140) from its vehicle (+160). 1, or 0 with no vehicle.
uint32_t ReadCar(uint8_t* base, uint32_t driver);
// AiDriver::UpdateHalfWidth (sub_82145230): +116 = |cos(+148)| x +120 +
// |sin(+148)| x +124.
void UpdateHalfWidth(uint8_t* base, uint32_t driver);
// AiPath::TargetPoint (sub_8213B4D8): out (x, y, z) = ring entries k and k
// + 1's points (k the look-ahead index +5142) blended by where `ahead`
// falls between them (in slices of the route file's +8, kept in 0 to 1).
// 1, or 0 when k + 1 isn't below the entries' count (+5136).
uint32_t TargetPoint(uint8_t* base, uint32_t path, double ahead, uint32_t out);

// --- The skill (src/ai_drivers/ai_drivers.cpp) ---

// AiSkill::Clear (sub_8213F700): ResetSkill, the source (+0) and +36 = 0.
void ClearSkill(uint8_t* base, uint32_t skill);
// AiSkill::SetSource (sub_8213F900): the triggers (+24) cleared, ResetSkill,
// +36 = 0, +0 = the source (seven halfwords, or 0), ResetSkill again with
// one.
void SetSkillSource(uint8_t* base, uint32_t skill, uint32_t source);
// AiSkill::Load (sub_8213F730): with a source, its seven halfwords into +4
// to +16 and the triggers cleared; with none, ResetSkill, +6 = 100 and, when
// the race data's word +0xA3AF0 is 0, +10 = -5. Then +20 = 100.0.
void LoadSkill(uint8_t* base, uint32_t skill);

// --- Speeds and steering ---

// AiDriver::LimitSpeed (sub_82139248): the target speed (+6088) lowered to
// the speed at `speed` x CornerFactor(curvature, width), plus, for a word
// at `slices` above 0, SpeedOverDistance at slices x the slice length (the
// distance written at frame + 80: `frame` the original's, 128 bytes below
// the caller's stack pointer). Returns the f1 the original leaves.
double LimitSpeed(uint8_t* base, uint32_t driver, uint32_t speed, uint32_t curvature,
                  uint32_t width, uint32_t slices, uint32_t frame);
// AiDriver::LimitByCurve (sub_821390D0): `table` = (a pointer to a halfword
// count, the 8-byte entries); for the index at `index` in it and a record:
// the float at `fraction` raised to (+144 - entry) / (range - entry) when
// that's larger (1.0 when it's 1.0 or more), the range the record's +32
// below the entry, else its +36, and only when +144 is past the entry
// toward it.
void LimitByCurve(uint8_t* base, uint32_t driver, uint32_t table, uint32_t fraction,
                  uint32_t index, uint32_t record);
// AiDriver::EaseThrottleForSteer (sub_82139B98): |+152| x 0.1 taken off the
// throttle +6140 (0.0 at least), when above 0.
void EaseThrottleForSteer(uint8_t* base, uint32_t driver);

// --- The path's corners and points ---

// AiPath::HasKind2Ahead (sub_8213C148): 1 when a corner record before the
// first with a positive offset (+10) has the kind (+40) 2.
uint32_t HasKind2Ahead(uint8_t* base, uint32_t path);
// AiPath::IsSlow (sub_8213C0E8): the same for kinds 2 or 3.
uint32_t IsSlow(uint8_t* base, uint32_t path);
// AiPath::PreferredSections (sub_8213C358): for the signed bytes a (4 to 9)
// and b with (a - 4) x 2 + b at most 11, the list at 0x82008A08 + 64 x
// that; else 0.
uint32_t PreferredSections(uint8_t* base, uint32_t a, uint32_t b);
// AiPath::RaceAllowsSides (sub_8213D318): 1 when the race mode (the race
// data's +0xA3AF4) is 4, 7 or 8, or 5 with [0x826D82E8]'s +6100 1 or 3.
uint32_t RaceAllowsSides(uint8_t* base);
// AiPath::RefreshPoints (sub_8213B5D8): for the entries 1 to +5140 - 1,
// the point = the slice's center plus its side direction x the entry's
// offset (+24); y the center's.
void RefreshPoints(uint8_t* base, uint32_t path);
// AiPath::Reset (sub_8213D438): the ring emptied and its entries cleared,
// ClearCorners, the section list emptied; with a driver (+0),
// ClearOtherIndexes. Returns the driver word (the r3 the original leaves).
uint32_t ResetPath(uint8_t* base, uint32_t path);
// AiPathEntry::SetLaneOffset (sub_8213B390): the lane (+20) kept within the
// slice's lanes; +24 and +48 = (lane - (n - 1) x 0.5) / n x the road's
// width.
void SetLaneOffset(uint8_t* base, uint32_t entry);

// --- The other racers ---

// AiOther::UpdateFlags (sub_82145450): the flags +34 by the state +24 (see
// the row).
void UpdateOtherFlags(uint8_t* base, uint32_t record);
// AiOther::UpdateTimer (sub_821456A0): +30 counted down; 120 when the gap
// (+0) is below 30.0 and above the float at `limit` and the other's input
// ([0x826C5B30 + 4 x the low byte of the word at `index`]) has 0x20000.
void UpdateOtherTimer(uint8_t* base, uint32_t record, uint32_t index, uint32_t limit);
// AiDriver::ClearOthers (sub_821459B0): +484 to +496 cleared, the seven
// other-racer records reset and the list +452 set to -1.
void ClearOthers(uint8_t* base, uint32_t driver);
// AiDriver::PickTarget (sub_82145118): with +480 negative, +480 = the racer
// in the mask (+40), not itself, with the smallest gap of 0.0 or more, or
// failing that the negative one nearest 0.0; -1 when none.
void PickTarget(uint8_t* base, uint32_t driver);
// AiDriver::PickTargetBehind (sub_821451B8): +480 = the racer in the mask
// with the negative gap nearest 0.0; -1 when none.
void PickTargetBehind(uint8_t* base, uint32_t driver);
// AiDriver::KeepsGapFrom (sub_82145048): 1 with no state; 0 when the other
// (the word at `other`) is in the mask; in states 2 and 3, 0 when the gap
// to it is positive and its driver is in state 2 or 3; else 1.
uint32_t KeepsGapFrom(uint8_t* base, uint32_t driver, uint32_t other);

}  // namespace ai_drivers
