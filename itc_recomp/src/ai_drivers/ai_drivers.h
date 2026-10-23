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

// --- Set-up, modes and the plan ---

// AiDriver::SetParams (sub_82138D00): +500 = the object; the skill set from
// its +120 (or from nothing: SetSkillSource).
void SetParams(uint8_t* base, uint32_t driver, uint32_t object);
// AiDriver::SetSkill (sub_821393D8): LoadSkill, +6084 = the word at `word`,
// +12 = the skill's halfword +4 as a float, the vehicle's +2432 = 1.0.
void SetSkill(uint8_t* base, uint32_t driver, uint32_t word);
// AiDriver::ClearPlan (sub_82139820): the 48 bytes from +32 to their
// defaults; when the path's +5524 or +5528 isn't -1, both = -1 and
// ResetPath. Returns the r3 the original leaves (the path, or ResetPath's).
uint32_t ClearPlan(uint8_t* base, uint32_t driver);
// AiDriver::LimitSpeedBlend (sub_82139B40): `pair` = (a, b); when a is at
// least the float at `t`, a blended toward t by the weight at `w` (written
// at frame + 80, `frame` the original's, 96 bytes below the caller's stack
// pointer); LimitSpeed with it and b. Returns LimitSpeed's f1.
double LimitSpeedBlend(uint8_t* base, uint32_t driver, uint32_t speed, uint32_t pair,
                       uint32_t t, uint32_t w, uint32_t slices, uint32_t frame);
// AiPath::SetMode (sub_8213D508): with a new mode (the word at `mode`): +5520
// = it, the normalizer +5532 (NormalizeByLane for 1, NormalizeByBranch for
// 0), ResetPath. Returns the r3 the original leaves.
uint32_t SetMode(uint8_t* base, uint32_t path, uint32_t mode);
// AiPath::ResetFor (sub_8213E658): +0 = the driver, +5516 = 0.0, mode 1 with
// the lane normalizer, +5524 and +5528 = -1; ResetPath (whose r3 it
// returns).
uint32_t ResetFor(uint8_t* base, uint32_t path, uint32_t driver);
// AiPath::NormalizeByLane (sub_8213B428): the position normalized by lane
// (NormalizeLane, or NormalizeLaneMasked with masks); when on the route, the
// entry's slice record (+52), position (+16) and lane (+20) set and
// SetLaneOffset. 1, else 0.
uint32_t NormalizeByLane(uint8_t* base, uint32_t entry, uint32_t position, uint32_t lane,
                         uint32_t masks);
// AiDriver::UpdateLane (sub_82144EF8): +8 = the lane of the vehicle's track
// point at its route position, +4 = that position clamped (written at frame
// + 80: `frame` the original's, 128 bytes below the caller's stack pointer).
void UpdateLane(uint8_t* base, uint32_t driver, uint32_t frame);

// --- The other racers' records and the states ---

// AiDriver::ResetOthers (sub_82145A48): ClearOthers, then each other
// driver's record of this one reset the same way. Returns the r3 the
// original leaves (the last other driver, or the driver).
uint64_t ResetOthers(uint8_t* base, uint32_t driver);
// AiDriver::ClearCarState (sub_82146588): +80 to +156 = 0.0 (+92 = 1.0),
// +164 and +168 = 500.0, +480 = 0; ClearOthers.
void ClearCarState(uint8_t* base, uint32_t driver);
// AiDriver::UpdateState4 (sub_82146220): PickTarget; a gap of 0.0 up to
// 50.0 sets the speed at `speed` to 500.0, one of -50.0 up to 0.0 to 20.0.
void UpdateState4(uint8_t* base, uint32_t driver, uint32_t speed);
// AiDriver::UpdateState5 (sub_82146388): PickTargetBehind; above -30.0 (the
// float at 0x82008E48), the speed at `speed` lowered to that racer's speed x
// 0.7. Returns the r3 the original leaves.
uint64_t UpdateState5(uint8_t* base, uint32_t driver, uint32_t speed);
// AiDriver::IsSettled (sub_82146408): 1 with no state or no target (+480 =
// -1, PickTarget); in state 2, 0 with the gap above 20.0 or below -20.0; in
// state 3, above or below the floats at 0x820027A8 and 0x82008E5C; else 1.
uint32_t IsSettled(uint8_t* base, uint32_t driver);
// AiOther::Mirror (sub_82145610): out +0 to +8 = the record's negated, +20,
// +32, +33 copied, states 1 and 3 swapped, UpdateOtherFlags, +16 = the float
// at `value`.
void MirrorOther(uint8_t* base, uint32_t out, uint32_t record, uint32_t value);

// --- The path's entries and corners ---

// AiPathEntry::BlockLeft (sub_8213C5C8): `at` -= the entry's +28, `out` =
// (out + +28) + the width at `width`, at least `at`; the entry's +40 raised
// to it, at most +44.
void BlockLeft(uint8_t* base, uint32_t entry, uint32_t at, uint32_t width, uint32_t out);
// AiPathEntry::BlockRight (sub_8213C630): BlockLeft mirrored (at most `at`;
// +44 lowered to it, at least +40).
void BlockRight(uint8_t* base, uint32_t entry, uint32_t at, uint32_t width, uint32_t out);
// AiCorner::Init (sub_8213C698): the record from a corner's data, the
// slice's corner entry and the path's entry count.
void InitCorner(uint8_t* base, uint32_t record, uint32_t data, uint32_t corner_entry,
                uint32_t count);
// AiPath::WithinRoadAt (sub_8213C1A0): 1 when the offset at `offset` is
// within the road at the ring entry the driver's +128 ahead.
uint32_t WithinRoadAt(uint8_t* base, uint32_t path, uint32_t offset);

// --- Starts and resets ---

// AiDriver::ResetToMode1 (sub_82139450): SetFlag2 with 1, +16 bits 0 and 2
// cleared, the path's mode 1 (with a vehicle), +6084 = 0, ResetSkill, +12 =
// 73.0, the vehicle's +2432 = 1.0. The words passed by address are written
// in the original's frame (`frame`, 112 bytes below the caller's stack
// pointer, at +80 and +84).
void ResetToMode1(uint8_t* base, uint32_t driver, uint32_t frame);
// AiDriver::ResetToMode0 (sub_821394F0): SetFlag2 with 0, +16 bits 0 and 2
// cleared, the path's mode 0 (with a vehicle), ResetSkill, +6084 = 0, +12 =
// the skill's halfword +4 as a float (`frame` 128 bytes down; +80, +88).
void ResetToMode0(uint8_t* base, uint32_t driver, uint32_t frame);
// AiDriver::StartRace (sub_82139DA0): +16 bit 0 cleared, SetFlag2 with 1
// when the vehicle has none of the flags 0x81, +16 bit 2 set, the path's
// mode 0 (with a vehicle), SetSkill with 1, ClearPlan (whose r3 it
// returns). `frame` 112 bytes down (+80, +84).
uint32_t StartRace(uint8_t* base, uint32_t driver, uint32_t frame);
// AiDriver::ResetFollow (sub_821467B8): ClearFollow, ClearCarState.
void ResetFollow(uint8_t* base, uint32_t driver);
// AiDrivers::AllSettled (sub_8213ADF8): for each racer with flag 0x10, its
// driver copied into the original's frame (`frame`, 6,320 bytes below the
// caller's stack pointer, the copy at +80) and IsSettled on the copy; 0 when
// one isn't; else 1 with exactly one, or when the lowest speed of the
// copies (from 70.0) plus 5.0 is above 70.0. False, with nothing done
// that's seen outside the frame, when a copy can't be made natively
// (c_runtime::Memcpy, or a frame in the physical views: the original runs
// then).
bool AllSettled(uint8_t* base, uint32_t frame, uint32_t* result);

// --- Pedals and steering ---

// AiDriver::AccumulateSteer (sub_82138F00): +6136 += +6112 + +6116, then
// the correction by +152 (see the row).
void AccumulateSteer(uint8_t* base, uint32_t driver);
// AiDriver::ThrottleForSpeed (sub_82139168): the throttle (+6140) and brake
// (+6144) from the speed against the target speed (see the row).
void ThrottleForSpeed(uint8_t* base, uint32_t driver);
// AiDriver::EaseThrottleForSlide (sub_82139BE8): the throttle eased by the
// slide angle +6112 (`flag`: a word, 0.2 when set, else 0.05).
void EaseThrottleForSlide(uint8_t* base, uint32_t driver, uint32_t flag);
// AiDriver::LimitThrottle (sub_82139CB0): +6124 kept within 0.0 and the
// skill's +6 x 0.01, then capped by the vehicle's rpm against its limit.
void LimitThrottle(uint8_t* base, uint32_t driver);
// AiDriver::AdjustForOther (sub_821452A0): the speed limits +168 and +164
// lowered for a racer ahead closing in (see the row).
void AdjustForOther(uint8_t* base, uint32_t driver, uint32_t record, uint32_t mode);
// AiDriver::UpdateAgainstPlayer (sub_82146608): +484 to +488 from the
// driver's record of the first other racer with the flags 0x81, then the
// stuck count (+492, +16 bit 5). Returns the r3 the original leaves.
uint64_t UpdateAgainstPlayer(uint8_t* base, uint32_t driver);
// AiOther::Update (sub_821454C8): the record's change, rate, state and time
// to meet from the new gap (at `gap`) and the cars' length (at `length`),
// then +0 = the gap and UpdateOtherFlags.
void UpdateOther(uint8_t* base, uint32_t record, uint32_t gap, uint32_t length);

// --- Resets, steering input and the path's lists ---

// AiDriver::ResetControl (sub_82139580): +16 bit 0 cleared, SetFlag2 with 1
// (the word in the original's frame: `frame`, 112 bytes below the caller's
// stack pointer, +80), ResetToMode1 (its frame 112 bytes below that).
void ResetControl(uint8_t* base, uint32_t driver, uint32_t frame);
// AiDriver::Reset (sub_8213AAA8): ResetFollow, ResetFor on the path, the
// control fields +6088 to +6144 cleared (+6108 = 1.0, byte +6132 = 0), the
// update pointer +6148 = 0x8213A1B8, ResetOthers (whose r3 it returns).
uint64_t ResetDriver(uint8_t* base, uint32_t driver);
// AiDriver::UpdateSteerInput (sub_8213AC70): the first corner record with
// data (+5676, 44 bytes each, +5674 the count) of kind 1, 2 or 3 picks the
// step: 1 or none, AccumulateSteer (done here; returns 0); 2 or 3 (never
// seen in play: sub_8213AB40, sub_82139A08) return the kind with the record
// in `record`, for the caller to run the translated step with it in r4.
uint32_t UpdateSteerInput(uint8_t* base, uint32_t driver, uint32_t* record);
// AiPath::DropPassedSections (sub_8213BA48): the section list (+5144,
// +5146) up to the driver's route section dropped, or emptied when it isn't
// listed.
void DropPassedSections(uint8_t* base, uint32_t path);
// AiPath::SharesSections (sub_8213C010): 1 when the path's section list
// and the record's (+2 the count, halfwords from +36) run the same way from
// the first section they share; else 0.
uint32_t SharesSections(uint8_t* base, uint32_t path, uint32_t record);
// AiPathEntry::Widen (sub_8213C3A8): +40 = +32 + margin, +44 = +36 -
// margin, the shift added to +40 (positive) or +44; crossed, both their
// average.
void Widen(uint8_t* base, uint32_t entry, uint32_t margin, uint32_t shift);
// AiPathEntry::NarrowToOther (sub_8213C420): with another entry and its
// record, the entry's range (+32/+40, +36/+44) narrowed to the other's
// widened by the margin, kept within the record's +24 and +28 and moved by
// the offset.
void NarrowToOther(uint8_t* base, uint32_t entry, uint32_t other, uint32_t margin,
                   uint32_t offset);
// AiPath::AdvanceCorners (sub_8213D020): each corner record's +12 and +10
// lowered by the slices at `slices`; a record whose +12 isn't positive
// removed (the later ones moved down, the last cleared) and the count
// +5162 lowered. `r3` = the r3 the original leaves (the path, or the last
// move's destination, memcpy's). False, with nothing done, where the moves
// can't be memcpy's native copies (the records in the physical views or
// across one's end: the original runs then).
bool AdvanceCorners(uint8_t* base, uint32_t path, uint32_t slices, uint32_t* r3);

}  // namespace ai_drivers
