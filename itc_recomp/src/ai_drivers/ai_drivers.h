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

}  // namespace ai_drivers
