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
// The race data's global, its stage list's offset, and the event's global.
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kStageList = 0x59620;
constexpr uint32_t kEvent = 0x826C6078;
// The racer records (the race data's +48): 93 of kRacerRecordBytes, then the
// count (+0x595E0), the players' count (+0x595E4), +0x595E8 (the traffic's
// flag) and +0x595EC.
constexpr uint32_t kRacerRecordBytes = 3936;
constexpr uint32_t kRacersCount = 0x595E0;

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
// Car::SetupVehicle (sub_820CD5C8): Vehicle::SetupFromCar(data, car), a tail
// call (`frame` is SetupFromCar's 368-byte frame under the caller's r1;
// car_physics::VehicleSetupFromCar). False, with nothing written, where it
// refuses (its ApplyTuning's memcpy): the original runs there. `f1` gets
// what it leaves; r3 comes back as `frame` - 1008 + 352.
bool CarSetupVehicle(uint8_t* base, uint32_t car, uint32_t data, uint32_t frame, double* f1);

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

// --- The race data's small pieces (src/race_setup/race_data.cpp) ---

// sub_82101630: r3 = a location, `value` (r4, a byte of the item): value +
// 11, 13, 15, 17, 19 or 21 for locations 14 to 19 (the 64-bit sum), else 8.
uint64_t RivalLocation(int32_t location, uint64_t value);
// Race::AddStage (sub_82101828): unless `type` is 4, the race data's stage
// list's stage at its count (+800) gets +0 = type, and the count up.
void AddStage(uint8_t* base, uint32_t type);
// Stages::AddRacer (sub_820FFAB8): stage `stage` of the list (-1: the one at
// its count +800): its slot at +76 gets +44 + 8 x slot = racer, +48 + 8 x
// slot = word, and +76 up (each index loaded again after each store, as the
// original).
void StagesAddRacer(uint8_t* base, uint32_t list, uint32_t racer, uint32_t word, int32_t stage);
// sub_82101D20: a 972-byte entry's +792 = 0.0, +796 = 0 (the race data's
// records' element constructor; c_runtime::VectorConstruct calls this).
void RaceDataEntryConstruct(uint8_t* base, uint32_t entry);
// RaceData::RaceData (sub_821021B8): the 77 records' four 972-byte entries
// each (from +0x59AEC, 3,936 apart) constructed by the array helper
// (c_runtime::VectorConstruct with sub_82101D20). Returns the race data.
uint32_t RaceDataConstruct(uint8_t* base, uint32_t data);
// sub_820FF558: a 76-byte record cleared (memset of 64 bytes, then +60, +64,
// +72, +68 = 0). Returns the record (memset's r3).
uint32_t LapRecordReset(uint8_t* base, uint32_t record);
// RunningMean::Add (sub_82101698): {+0 a float mean, +4 a count}: the count
// += weight; unless that's 0, mean = fdivs(fmadds(old count, mean,
// fmuls(weight, value)), new count) (the counts converted to single).
void RunningMeanAdd(uint8_t* base, uint32_t mean, double value, uint32_t weight);
// RunningMean::Add's arithmetic on host values: the mean after adding
// `value` with `weight` to {mean, count} (the counts' sum not 0).
float MeanAfterAdd(float mean, uint32_t count, double value, uint32_t weight);
// sub_82101EB0: a 972-byte entry: +0, +4, +8 = -1, +12, +16 = 0; ten 76-byte
// records from +20 (sub_820FF558); +780, +784 = 0; +792 a running mean set
// to (0.0, 1) (RunningMean::Add of 0.0 with weight 1 on (0.0, 0), on the
// original's stack: here on host values); +800, +816 to +828 = 0.0; +804,
// +812, +808, +968 = 0, halfword +964 = 0, +832 = 0.
void EntryReset(uint8_t* base, uint32_t entry);
// CourseRecord::Reset (sub_82105C90): a 408-byte course record:
// sub_820FF558(+332); halfwords +4i + 2 = i, +4i = i + 1 for i < 10; +40, +48
// = 0, +36, +44, +46, +42 = -1. Returns sub_820FF558's (the record + 332).
uint32_t CourseRecordReset(uint8_t* base, uint32_t record);
// Time::Pack (sub_82101700): milliseconds (r3, all 64 bits as the original
// computes with them) packed: the sign in bit 31, minutes in bits 16-30,
// seconds in 10-15, milliseconds in 0-9. Its stack word (the original's r11
// starts from r1 - 16) leaves nothing in the result.
uint64_t TimePack(uint64_t milliseconds);
// sub_82110AC0: r3 = save data +48, mode: halfword +5796 + 14 x mode up
// (below 9999); when +6008 isn't the mode, +6008 = mode and halfword +6012 =
// 1, else +6012 up (below 9999).
void CountRace(uint8_t* base, uint32_t stats, uint32_t mode);
// RaceEvent::FirstOwned (sub_821430A0): the id (halfword +0, sign-extended)
// of the first item of the event's list (+156, halfword +20 entries of 12
// bytes) with +8 nonzero, byte +91 1 and a save data record (SaveData::Of(0)
// + 0x199F8 + 12 x id: halfwords +0, +4, +2 summed, to 16 bits) of 0 or
// below (the mapped row had it above 0: the original returns at `ble`);
// else -1.
int32_t EventFirstOwned(uint8_t* base, uint32_t event);
// RaceEvent::CurrentFirstOwned (sub_821434C8): RaceEvent::FirstOwned of
// [0x826C6078], or -1 when it's 0.
int32_t EventCurrentFirstOwned(uint8_t* base);
// RaceEvent::Current (sub_821434E8): [0x826C6078].
uint32_t EventCurrent(uint8_t* base);
// RaceEvent::Count (sub_821434F8): its halfword +20 (zero-extended).
uint32_t EventCount(uint8_t* base);

// Stages::Reset (sub_82101860): a 1,224-byte stage list cleared (memset),
// then ten 80-byte stages: +4 = 0, +0 = 4 (the type), +8 = 0, four slots
// from +44 of (-1, 2), +76 = 0; +800, +804, +1220, +808 = 0;
// CourseRecord::Reset(+812). Returns CourseRecord::Reset's r3.
uint32_t StagesReset(uint8_t* base, uint32_t list);
// Stages::Clear (sub_82101900): Stages::Reset keeping the list's +1220.
// Returns Stages::Reset's r3.
uint32_t StagesClear(uint8_t* base, uint32_t list);
// RacerRecord::Init (sub_820FF808): Car::Init, then +3920 = [0x82000A34]
// (0.0), +3912, +3916 = -1, halfwords +3904, +3906 = -1, +3924 = 0. Returns
// Car::Init's r3.
uint32_t RacerRecordInit(uint8_t* base, uint32_t record);

// --- The course routes (src/race_setup/course_routes.cpp) ---

// CourseRoutes::Get (sub_820FFF50)'s frame: 4,480 bytes, the table of 23
// kRouteBytes entries from +80, the out pointer's doubleword at +4312, the
// index word at +4508 (in the caller's frame).
constexpr uint32_t kRoutesFrameBytes = 4480;
constexpr uint32_t kRouteBytes = 184;
constexpr uint32_t kRoutesOutWord = 4312;
constexpr uint32_t kRoutesIndexWord = 4508;
// CourseRoutes::Get (sub_820FFF50): the original's table of 23 routes written
// into its stack frame `frame` (the caller's r1 - 4480) store by store, the
// index and `out` (r3, all 64 bits) stored in it, then entry `index` (not
// checked) copied to `out` by memcpy's native copy (native::Buffers_Memcpy).
// `source` gets the entry's address, `result` the original's r3 (out, loaded
// back from the frame). False, with the table written but nothing copied,
// where memcpy's override would run the original: the adapter makes the copy
// through memcpy.
bool CourseRoutesGet(uint8_t* base, uint64_t out, uint32_t index, uint32_t frame,
                     uint32_t* source, uint64_t* result);


// --- The routes (src/race_setup/course_routes.cpp) ---
//
// Route::Set, Route::Reset and Race::StartPosition copy a course route
// (CourseRoutes::Get into their own frame, then memcpy of it to their frame
// + 96), leaving both copies in their frame as the originals do; `frame` is
// their stack frame (the caller's r1 - 496, or - 5856 for
// Race::StartPosition), CourseRoutes::Get's frame below it. False, with
// nothing written, where memcpy's override would run the original
// (RouteCopiesNative): the adapters run the original then.

// Route::Set's and Route::Reset's frame and where CourseRoutes::Get's out
// goes in it.
constexpr uint32_t kRouteFrameBytes = 496;
constexpr uint32_t kRouteFrameOut = 288;
constexpr uint32_t kStartFrameBytes = 5856;
// Whether both copies of course route `index` (CourseRoutes::Get's into
// frame + out, memcpy's from there into frame + 96) are made natively.
bool RouteCopiesNative(uint32_t frame, uint32_t out, uint32_t index);
// Route::Set (sub_82101A10): a 16-byte route: +0 = index, +4 = location
// (index + 20 when location is 48, signed compare); the course route copied
// (CourseRoutes::Get(index), memcpy); +8 = its word +0, +12 = its last
// position (sub_820FFEB0 into frame + 80, copied by halfwords). The
// original's r3 is frame + 80.
bool RouteSet(uint8_t* base, uint32_t route, uint32_t index, uint32_t location, uint32_t frame);
// Route::Reset (sub_82101990): as Route::Set with index 0 and +4 = 20.
bool RouteReset(uint8_t* base, uint32_t route, uint32_t frame);
// Race::StartPosition (sub_82101A98, with the switch piece sub_82101ADC): a
// location's start position: locations 14 to 46 the first word of a course
// route (indexes 11, 13, 15, 17, 19, 21, then 0 to 22, each case's out at
// its own place in the frame); others a fixed pair stored as halfwords at
// frame + 80 and + 82 and loaded back as a word; outside 1 to 46, (135, 0).
bool StartPosition(uint8_t* base, uint32_t location, uint32_t frame, uint32_t* result);
// Whether Race::StartPosition at `frame` would be native.
bool StartPositionNative(uint32_t location, uint32_t frame);


// --- The racers and the records (src/race_setup/race_data.cpp) ---

// Racers::Reset (sub_820FF978): the racer records' +0x595E0 to +0x595EC = 0,
// then RacerRecord::Init on all 93. Returns the last one's r3.
uint32_t RacersReset(uint8_t* base, uint32_t records);
// Whether Racers::Add's copy of `car` would be native (always, when the car
// id isn't added).
bool RacersAddNative(uint8_t* base, uint32_t records, uint32_t car);
// Racers::Add (sub_820FF860): unless `car`'s id (+12, signed) is 0 to 71
// (then -1), the record at the count: RacerRecord::Init, 3,904 bytes from
// the car (memcpy's native copy), +3920 = float(value), +3924 = word, the
// count up (the record's address worked out again from the count before
// each step, as the original). Returns the count before (zero-extended).
// Only after RacersAddNative said yes.
int64_t RacersAdd(uint8_t* base, uint32_t records, uint32_t car, double value, uint32_t word);
// Race::AddRacer (sub_821017C0): with the racers' +0x595E4 (racers added
// this way) 0 the record's flags +3888 |= 0x1, with 1 |= 0x80; a nonzero
// `flag` |= 0x400; +0x595E4 up; then Racers::Add (a tail call). Returns its
// r3. Only after AddRacerNative said yes.
bool AddRacerNative(uint8_t* base, uint32_t record);
int64_t AddRacer(uint8_t* base, uint32_t record, uint32_t flag, double value, uint32_t word);
// sub_82101F68: one of the race data's 77 records (`frame` its frame, the
// caller's r1 - 128): +3912 = 0, +3908 = 1, +0, +3924, +3920 = 0; its four
// 972-byte entries at +4 reset (sub_82101EB0) with their +784 and +788 =
// 12000; +3932 = 0; Route::Reset(+3892) (its frame below this one). False,
// with nothing written, where Route::Reset's copies wouldn't be native.
bool RecordResetNative(uint32_t frame);
bool RecordReset(uint8_t* base, uint32_t record, uint32_t frame);

// Race::SetRoute (sub_82101D38): Route::Set(the race data + 4, index,
// location), a tail call (`frame` is Route::Set's: the caller's r1 - 496).
bool RaceSetRoute(uint8_t* base, uint32_t index, uint32_t location, uint32_t frame);
// Stages::SetRoute (sub_82101E90): Route::Set(stage `stage` (-1: the list's
// +804) + 24, index, location), a tail call.
bool StagesSetRoute(uint8_t* base, uint32_t list, uint32_t index, uint32_t location,
                    uint32_t stage, uint32_t frame);


// --- The race data's reset and the start (src/race_setup/race_data.cpp) ---

// RaceData::Reset (sub_821020F0; `frame` its frame, the caller's r1 - 112;
// r3 isn't read): the race data from [0x826C5B2C] loaded again before each
// step: Route::Reset(+4) (its frame below), +28, +32, +20, +24 = 0;
// Racers::Reset(+48); Stages::Reset(+0x59620); sub_82101FD0(+0x59AE8) (its
// frame below, 112 bytes); +0xA3AD0 = 1, +0xA3AD4 to +0xA3AE0 = 0,
// +0xA3AE4 to +0xA3AEC = 0, +0xA3AF0 = 7, +0xA3AF4 = 2. False, with nothing
// written, where a route's copies wouldn't be native (RaceDataResetNative).
bool RaceDataResetNative(uint32_t frame);
bool RaceDataReset(uint8_t* base, uint32_t frame);
// RaceData::ResetGlobal (sub_821021A8): RaceData::Reset, a tail call (the
// same frame).
bool RaceDataResetGlobal(uint8_t* base, uint32_t frame);
// sub_82101FD0: the race data's 77 records (`frame` its frame): cleared
// (memset, 0x49FE8 bytes), +0x49FE0 and +0x49FE4 = 0, then sub_82101F68 on
// each (3,936 bytes apart; their frames below). Only after
// RaceDataResetNative said yes.
void RecordsReset(uint8_t* base, uint32_t records, uint32_t frame);

// Car::Refresh as the original, where its plain C++ refuses (a guest call
// the adapter makes, with the car in r3).
using GuestRefresh = void (*)(void* user, uint32_t car);
// Racers::SetStart (sub_82101D50; `frame` its 4,096-byte frame): the
// position = Race::StartPosition(location) (its frame below); with the
// records' +0x595E8 (the traffic's flag) not 0, 16 cars (ids 55 to 70) built
// in a record at frame + 80 (RacerRecord::Init, Car::SetModel, flags |= 4,
// Car::Refresh, +3908 = [0x8200692C], halfwords +3904, +3906 = 0) each copied
// (memcpy) to the record at the count, the count up; then each record below
// the count (loaded again each time, signed) with flag 0x1 or 0x80 gets +3904
// = the position and +3908 = 0.0 for types 2 and 3 (signed), else
// [0x8200692C]; with flag 0x2 only +3908. Returns 1. Only after
// SetStartNative said yes.
bool SetStartNative(uint8_t* base, uint32_t records, uint32_t location, uint32_t frame);
uint32_t SetStart(uint8_t* base, uint32_t records, uint32_t location, uint32_t type,
                  uint32_t frame, GuestRefresh refresh, void* user);
// Race::PlaceRacers (sub_82102048; `frame` its 128-byte frame):
// Racers::SetStart(the race data + 48, its +8, the current stage's type)
// (its frame below). False from PlaceRacersNative where the race data's
// +0xA3AF4 is 0 or 4 (the save data's course record copied first,
// sub_82105BF8: never seen in play, the original runs) or Racers::SetStart
// wouldn't be native. Returns Racers::SetStart's 1.
bool PlaceRacersNative(uint8_t* base, uint32_t frame);
uint32_t PlaceRacers(uint8_t* base, uint32_t frame, GuestRefresh refresh, void* user);


// Race::SetUpRival (sub_82112DB0, with its switch piece sub_82112E2C;
// `frame` its 4,112-byte frame): item = DataTables::Item(rival); location
// 48 replaced by the item's byte +36 (0 to 9: 20, 22, 24, 25, 31, 33, 35,
// 37, 39, 41); with `add` the rival's racer record made at frame + 80
// (RacerRecord::Init, +3912 = the item's id, +3924 = its byte +37,
// Car::FromBuild(item + 160), flag 0x2, Car::Refresh) and copied to the
// records at their count (memcpy), else found (Racers::IndexOf);
// Stages::AddRacer for the player (0) and the rival; then by the item's
// halfword +88 (signed): 0, the route (Race::SetRoute(0, location) with
// `add`, Stages::SetRoute(list, 0, location, -1)) and a type 0 stage
// (Stages::SetType); 2 and 5, up to three racers from the item's +92 (-1
// ends; made with flags 0x202, or found and marked 0x800) added to the
// stage, then for 2 the route and a type 0 stage, for 5 the race data's
// +0xA3AF4 = 3 and the rival location's route (sub_82101630(location, item
// byte +90), Race::SetRoute(it, 48) with `add`, Stages::SetRoute(list, it,
// 48, -1)) and with `add` a type 1 stage; 3, the route and a type 0 stage,
// then the chain: the item's +92 list gives a stage's racers (the player and
// up to three: flags 0x302, or found and marked), its last racer's item the
// next list, until one is empty (0xFFFF), each stage of type 0; 4, +0xA3AF4 =
// 3, the rival location's route and a type 1 stage. Returns 1. The route
// frames below this one; Car::Refresh's original through `refresh` where its
// plain C++ refuses. False from SetUpRivalNative for kind 1
// (sub_820FFA88, never seen in play: the original runs) or where a copy
// wouldn't be native.
bool SetUpRivalNative(uint8_t* base, uint64_t location, uint32_t rival, uint32_t add,
                      uint32_t frame);
uint32_t SetUpRival(uint8_t* base, uint64_t location, uint32_t rival, uint32_t add,
                    uint32_t frame, GuestRefresh refresh, void* user);


// --- Race::SetUp (src/race_setup/set_up.cpp) ---

// A guest function called as the original would (r3 to r6 = `args`, f1, the
// stack pointer `r1`, the return address `lr`; the adapter's job). Returns
// r3.
using GuestCall = uint64_t (*)(void* user, uint32_t function, const uint64_t args[4], double f1,
                               uint32_t r1, uint32_t lr);
// Race::SetUp (sub_82115D98; `save` player 0's save data, `rival` an item id
// or -1, `frame` its 11,984-byte frame): save +6112 = 48; RaceData::Reset
// (RaceData::ResetGlobal); race data +0xA3AE0 = rand() % 10, +0xA3AF0 = 1,
// +0xA3AF4 = 2; its +32 = save +6104 (the mode); the player's racer record
// at frame + 7952 (Car::Init, 1,884 bytes of the garage car picked: the save's
// +0x19920-th node of its list at +6120, cars of 1,968 bytes from +6332; its
// +3892 = save data +0x1F870, Car::Refresh, Race::AddRacer with f1 the
// garage car's float +1892 and r6 save byte +8); +0x59618 = 2, 1 or 3 by the
// mode, +0x5961C = 5; Race::SetRoute(0, save +6108); then event 1 (save
// +6100): the player and a fixed rival (car 11, built at frame + 80) in a
// stage of type 0, +808 = 1, +0x59AE4 |= 8; else with a rival
// Race::SetUpRival(location, rival, 1), +808 = 1, +0x59AE4 |= 0x10; else the
// route again, +0x59AE4 |= 8, a racer record for each of the event's
// entries (built at frame + 4016: Car::FromBuild of its item's build, +3912
// its id, +3916 the entry's index, +3924 its byte +37, flags 0x2 and 0x300
// when the entry's +8 is 0, +3904 the entry's +4), then
// Race::SetUpRival(location, the first owned item, 0) or Race::AddStage(4);
// then race data +20 = save +0x199E0, +24 = the event's two bits (the table
// at 0x82008114); Race::PlaceRacers; save +5886 + 2 x (location - 14) (14 to
// 19) or + 2 x location (0 to 13) up to 9999; sub_82110AC0(save + 48, the
// mode). Returns save + 48 (the original's r3). Each callee runs with the
// frame the original's call gives it, below this one; where a callee's plain
// C++ would hand back to its original, that original is called through
// `call` (from `frame`). Only after SetUpNative said yes (memcpy's native
// copies for the garage car and the racer records).
bool SetUpNative(uint8_t* base, uint32_t save, uint32_t frame);
uint32_t SetUp(uint8_t* base, uint32_t save, uint64_t rival, uint32_t frame, GuestCall call,
               void* user);

}  // namespace race_setup
