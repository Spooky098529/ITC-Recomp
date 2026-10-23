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
// step: 1 or none, AccumulateSteer; 2, SteerIntoCorner; 3, SteerOutOfCorner
// (both never seen in play; tail calls, from `stack`, the caller's stack
// pointer). Returns the r3 the original leaves; `f1` set (and `f1_set`)
// when SteerAtCornerPoint's atan2 ran.
uint64_t UpdateSteerInput(uint8_t* base, uint32_t driver, uint32_t stack, double* f1,
                          bool* f1_set);
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

// --- The states and the drivers' resets ---

// AiDriver::Restart (sub_821395D0): ResetControl (its frame 112 bytes below
// `frame`, the original's, 112 bytes below the caller's stack pointer),
// ResetPath, the state +36 = the old +32, +32 = 5, the halfword +40 = the
// word at `mask`. Returns ResetPath's r3.
uint32_t Restart(uint8_t* base, uint32_t driver, uint32_t mask, uint32_t frame);
// AiDrivers::ResetAll (sub_82139750): the mask of the racers without flag
// 0x10 (bit [racer + 20], written at frame + 80: `frame` the original's,
// 128 bytes below the caller's stack pointer), then Restart for each such
// racer's driver with it. Returns the r3 the original leaves (the count, or
// the last Restart's).
uint32_t ResetAll(uint8_t* base, uint32_t frame);
// AiDriver::SetState2or3 (sub_82139E40): ResetControl, ClearPlan,
// ResetPath; +32 = 3 when the word at `flag` is set, else 2 (the old into
// +36); +40 = the word at `word`. `frame` 112 bytes down. Returns
// ResetPath's r3.
uint32_t SetState2or3(uint8_t* base, uint32_t driver, uint32_t word, uint32_t flag,
                      uint32_t frame);
// AiDriver::SetStateFromList (sub_82139EA8): ResetControl, ClearPlan; by
// the count at `count` and where the driver's +0 is in the list (see the
// row), ResetPath and the state 2 or 3 with +40. `frame` 128 bytes down.
// Returns the r3 the original leaves (ClearPlan's or ResetPath's).
uint32_t SetStateFromList(uint8_t* base, uint32_t driver, uint32_t word, uint32_t count,
                          uint32_t list, uint32_t frame);
// AiDriver::SetState4 (sub_82139FA0): ResetControl, ClearPlan, ResetPath;
// +32 = 4 (the old into +36), +40 = the word at `word`. `frame` 112 bytes
// down. Returns ResetPath's r3.
uint32_t SetState4(uint8_t* base, uint32_t driver, uint32_t word, uint32_t frame);
// AiDrivers::ClearAll (sub_8213A140): every racer's driver ResetControl and
// ClearPlan. `frame` 128 bytes down. Returns the r3 the original leaves (the
// count, or the last ClearPlan's).
uint32_t ClearAll(uint8_t* base, uint32_t frame);

// --- The corners' joins ---

// AiPath::JoinCorners (sub_8213BCD0): the slopes joining corner record a's
// exit to b's entry (a +28, b +32), the flags a +36 and b +37 set, the
// halfwords +14 (the count at `count` raised to the gap first). Returns b's
// word +4 (the r3 the original leaves).
uint32_t JoinCorners(uint8_t* base, uint32_t a, uint32_t b, uint32_t count);
// AiPath::JoinCornersShort (sub_8213BF28): the same for corners closer than
// the gap (the count at `count` read, not changed). Returns b's word +4.
uint32_t JoinCornersShort(uint8_t* base, uint32_t a, uint32_t b, uint32_t count);
// AiPath::JoinCornerPair (sub_8213DD40): with both records' data and
// tables, the gap (a +10 - b +10 - b's data +0, written at frame + 80:
// `frame` the original's, 112 bytes below the caller's stack pointer):
// negative, JoinCorners when the directions (+16) differ (1, `r3` set),
// else 2 (sub_8213BDF0, b's data +28 below 100.0) or 3 (sub_8213BE80), for
// the caller to run the translated step (never seen in play) with r6 =
// frame + 80; positive and at most 100.0 times the slice length,
// JoinCornersShort (1, `r3` set). 0 otherwise (r3 kept).
uint32_t JoinCornerPair(uint8_t* base, uint32_t a, uint32_t b, uint32_t frame, uint32_t* r3);

// AiPath::AddCornersAt (sub_8213E970): of the corners listed on the path
// entry's slice (+52: +58 the count, 4-byte entries from +60), the first not
// among the path's records whose data (the route file's +16 table) ends past
// the listed slice and SharesSections, added (at most 8: InitCorner, then
// JoinCornerPair with the one before; `frame` the original's, 144 bytes
// below the caller's stack pointer) and the count +5162 raised. 0 when done
// (`r3` the r3 the original leaves); 2 or 3 when JoinCornerPair's
// translated step is to run (r3 = the path, r4 = `a`, r5 = `b`, from 112
// bytes below `frame`), then AddCornersAtFinish.
uint32_t AddCornersAt(uint8_t* base, uint32_t path, uint32_t entry, uint32_t frame,
                      uint32_t* r3, uint32_t* a, uint32_t* b);
// AddCornersAt's last step: the count +5162 raised by one.
void AddCornersAtFinish(uint8_t* base, uint32_t path);

// --- The states' speeds ---

// AiDriver::UpdateState2 (sub_82145F48): PickTarget; by the gap to it, the
// target speed at `speed` lowered (ahead) or raised (behind) to a speed
// from the target's (see the row). Returns the r3 the original leaves (the
// driver, or AiDriver::Of's).
uint64_t UpdateState2(uint8_t* base, uint32_t driver, uint32_t speed);
// AiDriver::UpdateState3 (sub_821460C8): the same with state 3's bands.
uint64_t UpdateState3(uint8_t* base, uint32_t driver, uint32_t speed);
// AiDriver::UpdateState6 (sub_821462A0, never seen in play, so without a
// row: UpdateState's state 6): PickTargetBehind; with one in state 6 too,
// the target speed at `speed` lowered to (its speed + the gap closed at this
// one's rate) x 0.7. Returns the r3 the original leaves (the driver, or
// AiDriver::Of's).
uint64_t UpdateState6(uint8_t* base, uint32_t driver, uint32_t speed);
// AiDriver::UpdateState (sub_82139AC8): by the state +32, UpdateState2, 3,
// 4 or 5 with `speed`; in state 6, +6088 lowered to the speed less 10.0 (at
// least 73.0), then UpdateState6. Returns the r3 the original leaves.
uint64_t UpdateState(uint8_t* base, uint32_t driver, uint32_t speed);

// --- The target speed ---

// AiDriver::LimitForCorner (sub_8213A620): the corner record's +20 from
// the driver's +144 against the ring entry at the corner's start, its kind
// cleared (ClearCornerKind), LimitByCurve on the next ring entries, then the
// target speed limited (LimitSpeedBlend) by the corner's speeds at its
// entry, middle and exit (see the row). The words passed by address are in
// the original's frame (`frame`, 192 bytes below the caller's stack
// pointer: +80, +84, +88, +92). Returns the r3 the original leaves; `f1`
// set (and `f1_set`) when LimitSpeedBlend ran.
uint64_t LimitForCorner(uint8_t* base, uint32_t driver, uint32_t scale, uint32_t record,
                        uint32_t frame, double* f1, bool* f1_set);
// AiDriver::UpdateTargetSpeed (sub_8213AB98): +6088 from the vehicle's
// flags (or +12 with +16 bit 2), UpdateState with it (with a state), then
// LimitForCorner for each corner record (+5674, from +5676). `frame` 128
// bytes down. Returns the r3 the original leaves; `f1` as LimitForCorner's.
uint64_t UpdateTargetSpeed(uint8_t* base, uint32_t driver, uint32_t scale, uint32_t frame,
                           double* f1, bool* f1_set);
// AiDriver::UpdateSpeed (sub_8213ACD0): the scale 1.0 + the skill's
// halfword +10 (+6058, within -100 and 100) x 0.0025, written at frame + 80
// (`frame` 96 bytes down), then UpdateTargetSpeed with it.
uint64_t UpdateSpeed(uint8_t* base, uint32_t driver, uint32_t frame, double* f1, bool* f1_set);

// --- Steering at the target and the pedals ---

// AiDriver::SteerToTarget (sub_82138D78): the look-ahead (the speed x 0.155,
// or 0.225 when IsSlow, at least 12.5, plus +156 at least 0.0) in slices
// (at most 78) into +5654, TargetPoint into +6096; with one, the angle to it
// (+6116), +152 and +6112 (see the row). 1, or 0 without one; `f1` the f1
// the original leaves.
uint32_t SteerToTarget(uint8_t* base, uint32_t driver, double* f1);
// AiDriver::UpdatePedals (sub_8213A428): ThrottleForSpeed, the throttle and
// brake requests eased (see the row; EaseThrottleForSlide's flag at frame +
// 80: `frame` the original's, 112 bytes below the caller's stack pointer),
// then +6124 and +6128 moved toward them by at most 0.2 and kept in 0.0 to
// 1.0.
void UpdatePedals(uint8_t* base, uint32_t driver, uint32_t frame);

// --- The drivers' table and the path's lines and ranges ---

// AiDrivers::Create (sub_8213B1E0): the table of 7 drivers from operator
// new (each InitPath) into [0x826C6074], zeroed, then each driver's skill
// cleared, +500 = 0, ResetDriver, +0 = its index and +160 = Vehicle::Get of
// it (when there is one). `r3` = the r3 the original leaves (the last
// ResetDriver's, or 0 without a table). False, with nothing done, when
// operator new fails natively (the original runs then).
bool Create(uint8_t* base, uint64_t* r3);
// AiPath::ApplyCornerLine (sub_8213BB58): for the corner record at the
// index at `index`, the ring entries' +48 from the corner's table (+4) from
// its position +14 on, with the slopes +28 and +32 (see the row).
void ApplyCornerLine(uint8_t* base, uint32_t path, uint32_t index);
// AiPath::ApplyCornerLines (sub_8213CFC8): ApplyCornerLine for each corner
// record (the index written at frame + 80: `frame` the original's, 112
// bytes below the caller's stack pointer).
void ApplyCornerLines(uint8_t* base, uint32_t path, uint32_t frame);
// AiPath::KeepToCornerSide (sub_8213CA18): the ring entries from the
// record's +10 (at least 1) up to the count at `count` kept to the corner's
// side (+40 raised, or +44 lowered, by the margin at `margin` and the shift
// at `shift`).
void KeepToCornerSide(uint8_t* base, uint32_t path, uint32_t record, uint32_t count,
                      uint32_t margin, uint32_t shift);
// AiPath::SetRanges (sub_8213D670): the shift (the driver's +152 x 0.2 in
// -1.0 to 1.0, 0.0 in mode 1) and margin (+116 + 0.6) at frame + 80 and +
// 84 (`frame` 128 bytes down), entries 1 to the count at `count` - 1
// widened (Widen), KeepToCornerSide for each corner record (the margin the
// driver's +128). Returns the r3 the original leaves.
uint32_t SetRanges(uint8_t* base, uint32_t path, uint32_t count, uint32_t frame);
// AiPath::DropOffRouteCorners (sub_8213DC08): the corner records kept
// while SharesSections; the rest cleared and the count +5162 cut (see the
// row). Returns the r3 the original leaves (the path, or the last
// SharesSections' answer, 0 for a record without data).
uint32_t DropOffRouteCorners(uint8_t* base, uint32_t path);

// --- The path's offsets, narrowing and branches ---

// AiPath::SpreadOffset (sub_8213B890): the amount at `amount` spread over
// the ring entries from the index at `last` - 1 back to the one at `first`
// (each entry's +24 moved by its share); at an entry that would leave its
// range (+32 to +36), the rest spread again before it (recursively), the
// entry set to the range's end and the leftover added to that entry once a
// step after it (see the row). The index and leftover are passed by address
// in the original's frame (`frame`, 128 bytes below the caller's stack
// pointer: +80, +88).
void SpreadOffset(uint8_t* base, uint32_t path, uint32_t first, uint32_t last, uint32_t amount,
                  uint32_t frame);
// AiPath::ClampOffset (sub_8213CF58): an entry's +24 outside +32 to +36:
// the difference spread (SpreadOffset with the indexes at `first` and
// `last`; the amount at frame + 80, `frame` 96 bytes down), then +24 = the
// range's end.
void ClampOffset(uint8_t* base, uint32_t path, uint32_t entry, uint32_t last, uint32_t first,
                 uint32_t frame);
// AiPathEntry::NarrowToOtherEx (sub_8213C4C0): NarrowToOther with a margin
// a side (at `low` and `high`), grown by the step at `step` while below the
// limit at `limit` when its side was narrowed, else reset to the step. 1
// when a side was narrowed, else 0.
uint32_t NarrowToOtherEx(uint8_t* base, uint32_t entry, uint32_t other, uint32_t step,
                         uint32_t offset, uint32_t low, uint32_t high, uint32_t limit);
// AiPath::NarrowBehind (sub_8213D140): `entry` narrowed to `other`
// (NarrowToOtherEx, the offset -(other +28)), then back along the ring
// while each was narrowed (the margins and offset in the original's frame:
// `frame`, 128 bytes down, +80, +84, +88). Returns the last answer.
uint32_t NarrowBehind(uint8_t* base, uint32_t path, uint32_t other, uint32_t entry,
                      uint32_t step, uint32_t limit, uint32_t frame);
// AiPath::CompareBranches (sub_8213C788): -1 to take section a, 1 to take
// b, 0 for no preference (see the row).
int32_t CompareBranches(uint8_t* base, uint32_t path, uint32_t a, uint32_t b, uint32_t a_number,
                        uint32_t b_number, uint32_t masks);
// AiPath::NormalizeByBranch (sub_8213C908): the position normalized (along
// the preferred sections or by the masks); on the route, the entry's slice
// record (+52), position (+16), lane (+20), offsets +24 and +48 = 0.0 and
// point (+0 to +8) set. 1, else 0.
uint32_t NormalizeByBranch(uint8_t* base, uint32_t path, uint32_t entry, uint32_t position,
                           uint32_t lane, uint32_t masks);

// --- Making room for the other racers ---

// AiPath::MeasureOthers (sub_8213B650): a 20-byte record at `out` for each
// racer listed at the driver's +452 (Race::RacerCount of them, negative ones
// skipped): +0 the driver's record of it's ring index (+26), +2 the entry
// after it (or the look-ahead +5140), +4 the other's offset across the road,
// +8 the entries' +28 summed up to it (the sum and the entry kept from one
// racer to the next), +12 the two drivers' +116 added, +16 = 0.0; a ring
// index past the entries sets that list slot to -1 and ends it. Returns the
// r3 the original leaves (the last other driver, or the count).
uint64_t MeasureOthers(uint8_t* base, uint32_t path, uint32_t out);
// AiPath::AvoidOnLeft (sub_8213CB30): with `a` a MeasureOthers record and
// `record` the driver's record of that racer: AdjustForOther when the
// driver is to its right, then a line to the other car's left side, the
// ring entries' +40 raised to it, or BlockLeft on each entry up to a's +2
// (see the row; the original's frame `frame`, 176 bytes below the caller's
// stack pointer: +80, +88, +96). Returns the r3 the original leaves.
uint64_t AvoidOnLeft(uint8_t* base, uint32_t path, uint32_t a, uint32_t record, uint32_t frame);
// AiPath::AvoidOnRight (sub_8213CD50): AvoidOnLeft mirrored (`frame` 160
// bytes down).
uint64_t AvoidOnRight(uint8_t* base, uint32_t path, uint32_t a, uint32_t record, uint32_t frame);
// AiPath::ChooseSideForOther (sub_8213D788): by the record's kind (+24) and
// the room each side, a's +16 = the other's offset plus or minus its width
// and AvoidOnLeft or AvoidOnRight (from `stack`, the caller's stack
// pointer, as the original's tail calls). Returns their r3, or the path.
uint64_t ChooseSideForOther(uint8_t* base, uint32_t path, uint32_t a, uint32_t record,
                            uint32_t stack);
// AiPath::AvoidByLane (sub_8213D898, never seen in play, so without a row:
// AvoidOthers' flag 0x10): with the other within its width + 1.0 of the
// driver, a's +16 = the center of the lane beside the next ring entry's
// (toward the driver), then AvoidOnLeft or AvoidOnRight (`frame` 96 bytes
// down). Returns their r3, or the path.
uint64_t AvoidByLane(uint8_t* base, uint32_t path, uint32_t a, uint32_t record, uint32_t frame);
// AiPath::AvoidOthers (sub_8213E690): seven MeasureOthers records in the
// original's frame (`frame`, 272 bytes below the caller's stack pointer,
// from +80), then for each listed racer by the flags of the driver's record
// of it: ChooseSideForOther, AvoidOnLeft and AvoidOnRight, or AvoidByLane
// (see the row). Returns the r3 the original leaves.
uint64_t AvoidOthers(uint8_t* base, uint32_t path, uint32_t frame);

// --- The skill's timed triggers ---

// sub_8213F7E0 (never seen in play, so without a row: a trigger condition,
// kinds 4 and 5): the float +824, truncated, of the race state's record
// (+2184: a count, 972-byte records from +2188, matched by their word +0)
// keyed by the driver's +500 object's halfword +0; 0 with none.
uint32_t SkillEntryWord(uint8_t* base, uint32_t driver);
// sub_8213F868 (never seen in play, so without a row: kinds 6 and 7): the
// same record's word +784 as a float x the float at 0x8200805C; 0.0 with
// none.
double SkillEntryValue(uint8_t* base, uint32_t driver);
// AiSkill::TriggerStarts (sub_8213F958): 1 when the trigger's condition
// (by its kind, 0 to 14) holds (see the row), else 0. `f1` set (and
// `f1_set`) for kinds 6 and 7, as the original leaves it.
uint32_t TriggerStarts(uint8_t* base, uint32_t driver, uint32_t trigger, double* f1,
                       bool* f1_set);
// AiSkill::TriggerEnds (sub_8213FB48): 1 when it ends: the conditions
// reversed; kinds 10 to 14 count the state's halfword (at `state`) down
// and end at 0; 1 for kinds above 14.
uint32_t TriggerEnds(uint8_t* base, uint32_t state, uint32_t driver, uint32_t trigger,
                     double* f1, bool* f1_set);
// AiSkill::Update (sub_8213FCF8): with a source and +36 = 1, the three
// triggers started (timer 150, active, their value added to +20) or ended
// (inactive, taken off). 1 when one changed.
uint32_t UpdateSkill(uint8_t* base, uint32_t skill, uint32_t driver, double* f1, bool* f1_set);

// --- Steering through the corners and driving ---
//
// The corner kinds 2 and 3's steps (never seen in play, so without rows;
// AiDriver::UpdateSteerInput's): each takes the driver and a corner record
// (+0 its data, +10 its offset, +16 its direction, +40 its kind).

// AiDriver::LimitSlide (sub_821392D8): above 100.0 km/h (or 20.0 and the
// target speed x the float at 0x82001070), with the steering ratio toward
// the corner above 0.2: the yaw against it below the float at 0x82002A10
// takes the throttle request off and scales +6136; with the flag at `flag`
// set, also the handbrake byte +6132 (see the code).
void LimitSlide(uint8_t* base, uint32_t driver, uint32_t record, uint32_t flag);
// AiDriver::SteerByRoom (sub_82138FA8): the room on the corner's side at the
// first ring entry (into the float at `out`); above +116 and turning toward
// the corner, +6136 = +6112 + +6136 + the angle +6116 x 0.5 (scaled down by
// the room against +128), returning 1; else AccumulateSteer, returning 1
// when only the turn wasn't toward it, else 0.
uint32_t SteerByRoom(uint8_t* base, uint32_t driver, uint32_t record, uint32_t out);
// AiDriver::SteerOutOfCorner (sub_82139A08, kind 3): above 20.0 km/h,
// SteerByRoom (the room at frame + 80: `frame` 96 bytes below the caller's
// stack pointer) and the throttle request cut by the angles; else
// AccumulateSteer. Returns the r3 the original leaves.
uint64_t SteerOutOfCorner(uint8_t* base, uint32_t driver, uint32_t record, uint32_t frame);
// AiDriver::SteerAtCornerPoint (sub_8213A200): the ring entry at the index
// at `at` (at least the look-ahead +5654): with room on the corner's side,
// +6136 steered at its point (atan2, the angle wrapped at frame + 80:
// `frame` 128 bytes down), then LimitSlide (the flag 0 at frame + 84); with
// none, kind 3 and SteerOutOfCorner. Returns the r3 the original leaves.
uint64_t SteerAtCornerPoint(uint8_t* base, uint32_t driver, uint32_t record, uint32_t at,
                            uint32_t frame, double* f1, bool* f1_set);
// AiDriver::SteerIntoCorner (sub_8213AB40, kind 2): past the corner's exit
// (its data's +52 + the offset below 0), kind 3 and SteerOutOfCorner; else
// SteerAtCornerPoint at its entry (+50 + the offset, at frame + 80: `frame`
// 96 bytes down).
uint64_t SteerIntoCorner(uint8_t* base, uint32_t driver, uint32_t record, uint32_t frame,
                         double* f1, bool* f1_set);
// AiDriver::UpdateSteering (sub_8213AEF0): UpdateSteerInput (from `frame`,
// 112 bytes below the caller's stack pointer), the request +6136 corrected
// by the yaw +104 and scaled by +108 or +112, and the steering +6120 moved
// toward it by at most 0.2 within -1.0 to 1.0 (see the row). Returns IsSlow's
// answer (the r3 the original leaves).
uint32_t UpdateSteering(uint8_t* base, uint32_t driver, uint32_t frame, double* f1,
                        bool* f1_set);
// AiDriver::Drive (sub_8213B058): the car's input record at `input` from the
// driver (see the row; `frame` 160 bytes below the caller's stack pointer).
// Returns the r3 the original leaves (the driver); `f1` as the original
// leaves it.
uint64_t Drive(uint8_t* base, uint32_t driver, uint32_t input, uint32_t frame, double* f1);

// --- The path's line through the corners ---
//
// Each moves the ring entries' offsets (+24) from the index at `index` on
// (advanced as it goes), each from the entry before (at `previous`, a word
// updated to each entry in turn; ContinueLine's starts at `entry`), kept in
// the entry's range (+40 to +44), the step from the one before limited to
// the path's +5516, then ClampOffset (from the original's frame: `frame`,
// 112 or 160 bytes below the caller's stack pointer, ClampOffset's 96 below
// it). The others return 1 when they stopped at the look-ahead's end
// (+5140), else 0.

// AiPath::ContinueLine (sub_8213DB08): up to +5140, each entry's offset the
// one before's plus its +28 (the first index at `first`; `frame` 112 down).
void ContinueLine(uint8_t* base, uint32_t path, uint32_t entry, uint32_t index, uint32_t first,
                  uint32_t frame);
// AiPath::ApproachCorner (sub_8213DE30): up to the corner record's entry
// (+10 plus one), the offsets moved in even steps toward the target there
// (the entry's +48, or the corner's table's first offset) x 0.8, each step
// limited to half +5516 (`frame` 160 down; the start index at frame + 80).
uint32_t ApproachCorner(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                        uint32_t previous, uint32_t frame);
// AiPath::CornerToEntry (sub_8213E018): up to the corner's turn-in (its
// data's +50 plus the offset +10), toward each entry's +48 in even steps (by
// the corner's side +16).
uint32_t CornerToEntry(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                       uint32_t previous, uint32_t frame);
// AiPath::CornerToApex (sub_8213E1D0): the same up to the data's +52.
uint32_t CornerToApex(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                      uint32_t previous, uint32_t frame);
// AiPath::CornerToExit (sub_8213E360): the same up to the data's +54, the
// steps x 0.8.
uint32_t CornerToExit(uint8_t* base, uint32_t path, uint32_t record, uint32_t index,
                      uint32_t previous, uint32_t frame);

// AiPath::PlanCorners (sub_8213E7B8): for each corner record with data and
// a table: the index (at `index`) moved back to the corner's offset +10 when
// past it, or on to the end index (at `end`) when before it (the previous
// entry the ring entry before, in the word at frame + 172, the caller's,
// where the original keeps r4: `frame` 144 bytes below the caller's stack
// pointer); then ApproachCorner, CornerToEntry, CornerToApex and
// CornerToExit in turn, returning 1 at the first that reaches the
// look-ahead's end; the end index set to the corner's exit (+54) plus +10.
// After the last, the end = the index and ContinueLine. Returns the r3 the
// original leaves (1, or the path).
uint32_t PlanCorners(uint8_t* base, uint32_t path, uint32_t entry, uint32_t index, uint32_t end,
                     uint32_t frame);
// AiPath::PlanLine (sub_8213F4B0): from the ring's first entry (0 with
// none), with both indexes 1 (at frame + 84 and + 80, `frame` 96 bytes
// down), PlanCorners with corner records, else ContinueLine. Returns their
// r3.
uint32_t PlanLine(uint8_t* base, uint32_t path, uint32_t frame);

// --- The path's branch kinds and lines ---

// AiPath::BranchMasksFor (sub_8213D378): the branch masks list for the kind
// at `kind` (0 to 9) under the race data's flags ([0x826C5B2C] + 20): the
// list at 0x82008970 + 16 for kinds 1, 6, 7 with 0x4 or 0x8, else (and for
// kinds 2, 8) + 32 with 0x10, else (and for kinds 3, 9) + 48 with 0x20,
// else + 0 (also with none of 0x4, 0x8, 0x10, 0x20 or a kind above 9).
uint32_t BranchMasksFor(uint8_t* base, uint32_t kind);
// AiPath::LeaderBranchKind (sub_8213D1F0): with one of the race data's flags
// 0x4, 0x8, 0x10, 0x20, a driver vehicle, and no kind yet (the word at
// `kind` -1) or the driver's +488 above the float at `best`: the kind for
// the branch number of its route section (KindForBranch) into `kind`, +488
// into `best`. Returns the r3 the original leaves (the path, 0, the vehicle
// or its identity record).
uint32_t LeaderBranchKind(uint8_t* base, uint32_t path, uint32_t kind, uint32_t best);
// AiPath::FindSideSection (sub_8213D550): from the vehicle's route position
// (clamped, at frame + 80: `frame` 208 bytes below the caller's stack
// pointer), each of its section's links 6 to 9 that CompareBranches prefers
// (the current position at frame + 84) tried at the same slice (Normalize,
// OnSlice with the point) and taken when on it; out = the last position
// taken. Nothing written with a negative section or slice. Returns the r3
// the original leaves (64 bits: CompareBranches' -1 sign-extended).
uint64_t FindSideSection(uint8_t* base, uint32_t path, uint32_t out, uint32_t point,
                         uint32_t masks, uint32_t frame);
// AiPath::FollowTargets (sub_8213D9D8): entries 1 to +5140 - 1, each one's
// offset the entry before's target +48, kept in range, the step from the
// one before's offset plus its +28 limited to +5516, then ClampOffset (the
// index and 1 at frame + 80 and + 84, `frame` 128 bytes down).
void FollowTargets(uint8_t* base, uint32_t path, uint32_t frame);
// AiPath::FollowLeaderBranch (sub_8213EB48): the kind for the branch of the
// first racer whose vehicle has the flags 0x81, under the conditions in the
// row (else -1); when it differs from +5528, +5528 = it and ResetPath.
// Returns the r3 the original leaves.
uint32_t FollowLeaderBranch(uint8_t* base, uint32_t path);

}  // namespace ai_drivers
