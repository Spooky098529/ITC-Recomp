// The stand's scene model holders as plain C++ (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/stand-models.md, the fields in
// docs/known-data.md: "garage stage", "scene model", "scene model record").
// The holder (`[0x826D83E0]`'s +0 word: its +0 the stage), the stage, its
// scene model (pool 7 of the second set, [0x826C2E44], index the stage's
// +8) and the record are guest data shared with the recompiled code (the
// front end's screens), so these work on guest memory with the originals'
// loads and stores in their order: the scalar float work as the translated
// code's doubles rounded to single (built with -ffp-model=strict and
// -ffp-contract=off, CMakeLists.txt). Each that has a frame takes `stack`,
// the guest r1 at entry, and keeps the original's frame: its back chain
// written as stwu does (not its saved registers), its temporaries at the
// original's offsets, its callees run from it. Callees already ours with
// plain C++ that needs no context (ScenePools::GetB and ClaimB, the
// record's defaults) are called as plain C++; the others through `call`
// (their adapters, from the frame, with the original's return address).
// Expect flush-to-zero off at entry and leave it off. Adapters:
// src/hooks/stand_models.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <optional>

namespace stand_models {

// A guest function `function` (through the function table: its override
// when it has one) with r3 on from `args` (all 64 bits each), r1 = `frame`
// and the link register `lr` (the original's return address); its r3.
// Flush-to-zero off after it.
using GuestCall = std::function<uint64_t(uint32_t function, std::initializer_list<uint64_t> args,
                                         uint32_t frame, uint32_t lr)>;

constexpr uint32_t kScene = 0x826C2E44;   // [it] the second set (the car scene)
constexpr uint32_t kHolder = 0x826D83E0;  // [it] the stand's holder

// The stage's scene model: ScenePools::GetB(scene, 7, the stage's +8).
uint32_t StageModel(uint8_t* base, uint32_t holder);

// --- The stage's small setters (src/stand_models/setters.cpp) ---

// sub_8217FCB0 GarageStage::MakeModel: r3 = the holder (`r3`, all 64 bits),
// r4 = a model block, r5 = a draw word. Its 4,384-byte frame below `stack`:
// nothing unless the stage's +16 is 0; else a record at frame + 80
// (SceneModelRecord::SceneModelRecord, Reset), its +16 = `block`; the
// stage's +8 = ScenePools::ClaimB(scene, 7); ScenePools::SetUpB(scene, 7,
// it, the record) through `call`; the model's +4296 and the stage's +6056 =
// `word`, the stage's +16 = 1, [kHolder]'s +20 = 1. Returns r3 as the
// original leaves it (`r3`, or SetUpB's).
uint64_t MakeModel(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t block,
                   uint32_t word, uint32_t stack);
// sub_82178FE8 GarageStage::SetInteriorClass: the model's +3808 = 0 below 3,
// 1 for 3 to 6, 2 from 7 (signed). sub_82179598 GarageStage::SetWord5016:
// +5016 = `value`. sub_82179A18 GarageStage::SetWheelWord: +4076 =
// `value`. Each a 96-byte frame below `stack`; returns the model (r3).
uint32_t SetInteriorClass(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack);
uint32_t SetWord5016(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack);
uint32_t SetWheelWord(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack);
// sub_820DF1A0 SceneParts::LightValue: the float at `points` + 27664 (f1).
double LightValue(uint8_t* base, uint32_t points);
// sub_82179420 GarageStage::SetLightsOn: `flag` set: the model's +4976 = 1,
// +4980 = 0.0 ([0x82000A34]); else +4976 = 0, +4980 = LightValue([+5060] +
// 176), whose f1 goes to `*f1` (left alone on the other path). Its 112-byte
// frame below `stack`. Returns r3 as the original leaves it: the model, or
// the point block on the second path.
uint32_t SetLightsOn(uint8_t* base, uint32_t holder, uint32_t flag, uint32_t stack, double* f1);
// sub_82179818 GarageStage::SetDecalA and sub_821798D8 SetDecalB: the
// table {2, 1, 0} at frame + 80 (its 160-byte frame below `stack`), indexed
// by `entry` unchecked; e = the model + 64 x the word + 224 (A) or + 416
// (B): e +16 = f1, +0 = `a`, +20 = f2, +4 = `b`, +32 = f3, +52 = f4, +48 =
// f4 x [0x82001BE0] when `b` is set, else f4. Returns the model (r3).
uint32_t SetDecal(uint8_t* base, uint32_t holder, uint32_t entry, uint32_t a, uint32_t b,
                  double f1, double f2, double f3, double f4, uint32_t stack, uint32_t at);
inline constexpr uint32_t kDecalA = 224, kDecalB = 416;

// --- The setters that re-apply the record (src/stand_models/setters.cpp):
// a word or two of the stage's model's record, then SceneModel::ApplyRecord
// (its plain C++ from the frame; where it declines, the race's path, its
// adapter through `call`), whose r3 they return; or ReloadDecals ---

// sub_82178F00 GarageStage::SetPart (its case 0 the split piece
// sub_82178F68, merged): the parts record's word for `slot` (0 +3744, 1 to
// 5 +3748 to +3764, 6 and 7 +3768, 8 and 9 +3772, 10 to 12 +3776 to +3784,
// 13 none, 14 +3788, 15 +3792; none above 15, unsigned) = `word`. 112-byte
// frame.
uint64_t SetPart(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t slot,
                 uint32_t word, uint32_t stack);
// sub_82179050 GarageStage::SetInteriorWord: `kind` 1 +3908, 4 +3900, 5
// +3896 (signed; none for the others) = `word`. 112-byte frame.
uint64_t SetInteriorWord(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t kind,
                         uint32_t word, uint32_t stack);
// sub_821791C0 GarageStage::SetInteriorLevel: +3908 = 0 below 3, 1 below 6,
// else 2; +3912 = 0 below 1, else 1 (signed). 96-byte frame.
uint64_t SetInteriorLevel(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t value,
                          uint32_t stack);
// sub_82179250 GarageStage::SetInteriorPair: +3904 = `a`, +3900 = `b`.
// 112-byte frame.
uint64_t SetInteriorPair(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t a,
                         uint32_t b, uint32_t stack);
// sub_82179378 GarageStage::ClearChoice: +3812 + 12 `choice` = 0 (wrapping,
// unchecked). 96-byte frame.
uint64_t ClearChoice(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t choice,
                     uint32_t stack);
// sub_82179548 GarageStage::SetAero: +3728 = `word`. 96-byte frame.
uint64_t SetAero(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t word,
                 uint32_t stack);
// sub_82178EC0 GarageStage::ReloadDecals: SceneModel::ReloadDecals' plain
// C++ on the stage's model from its 96-byte frame; its r3.
uint64_t ReloadDecals(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t stack);

// --- The tire grades (src/stand_models/setters.cpp) ---

// sub_820D8F18 Wheels::SetTireTextures: PartModel::SetTexture (through
// `call`) on the first set's wheels' tires, +240 with +40 and +704 with +56,
// every shape (r5 -1). 96-byte frame; the last call's r3.
uint64_t WheelsSetTireTextures(uint8_t* base, const GuestCall& call, uint32_t wheels,
                               uint32_t stack);
// sub_820E7470 SceneWheels::SetTireGrades: a pool 5 wheels object: +44 =
// `front`, +60 = `rear`, Wheels::SetRecord([+28], +32) through `call` (+28
// loaded before the stores), then SetTireTextures([+28]) (its r3). 96-byte
// frame.
uint64_t SceneWheelsSetTireGrades(uint8_t* base, const GuestCall& call, uint32_t wheels,
                                  uint32_t front, uint32_t rear, uint32_t stack);
// sub_82179130 GarageStage::SetTireGrade: `which` 0 the model's +4080 and
// +4096, 1 +4080, 2 +4096 (signed; neither for others) = `grade`; then
// SetTireGrades(+5100, +4080, +4096) (its r3). 112-byte frame.
uint64_t SetTireGrade(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t which,
                      uint32_t grade, uint32_t stack);
// sub_821CCA38 TuningScreen::SetTireGrade: the table {0, 1, 1, 2, 2} at frame
// + 80 (its 112-byte frame below `stack`); for `which` 0 to 2,
// GarageStage::SetTireGrade([[kHolder]], which, the table's word `index`,
// read unchecked) (its r3); else `r3` (r3 at entry) left.
uint64_t TuningScreenSetTireGrade(uint8_t* base, const GuestCall& call, uint64_t r3,
                                  uint32_t which, uint32_t index, uint32_t stack);

// --- The second wheels (src/stand_models/setters.cpp) ---

// sub_82179C48 GarageStage::SecondWheelsPending: the model's +5032 (r3).
// 96-byte frame.
uint32_t SecondWheelsPending(uint8_t* base, uint32_t holder, uint32_t stack);
// sub_82179998 GarageStage::AddSecondWheels: SceneModel::AddSecondWheels'
// plain C++ on the model (its r3). 96-byte frame.
uint64_t AddSecondWheels(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t stack);
// sub_821799D8 GarageStage::ApplyWheelPaint: SceneModel::ApplyWheelPaint on
// the model through `call` (its r3). 96-byte frame.
uint64_t ApplyWheelPaint(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t stack);
// sub_821CC890 TuningScreen::StepSecondWheels: with the holder [[kHolder]]
// (loaded again for each call): not pending and the screen's +25244 1:
// AddSecondWheels, +25244 = 2 (its r3); else (Pending asked again) not
// pending and +25244 2: ApplyWheelPaint, +25244 = 0 (its r3); else r3 the
// second Pending's answer. 112-byte frame.
uint64_t StepSecondWheels(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t stack);

// --- The wheel fitting (src/stand_models/wheels.cpp) ---

// sub_82178B98 GarageStage::FitAxles: r3 = the holder, r4 = a car object.
// Its 1,968-byte frame below `stack`: a car data at frame + 80
// (CarData::CarData, then Car::SetupVehicle's tail call
// Vehicle::SetupFromCar(data, car), its plain C++ from right below the
// frame, else Car::SetupVehicle through `call`); the stage's axle records
// +6176 to +6184 = data +336 to +344, +6188 = 1.0, +6192 to +6200 = data
// +608 to +616, +6204 = 1.0 (the stage loaded again for each). Returns
// SetupFromCar's r3; `*f1` its f1 when native, nullopt (as the guest call
// left it) otherwise.
uint64_t FitAxles(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t car,
                  uint32_t stack, std::optional<double>* f1);
// sub_820E7380 SceneWheels::SetSizes: a pool 5 wheels object, `a` and `b`
// to +104 and +108, r6 to r9 (all 64 bits) stored into the caller's
// parameter area (`stack` + 48 to + 79) and read back as two 12-byte values
// (r6's two words and r7's high word; r8's and r9's) into +76 to +84 and +88
// to +96; then Wheels::SetRecord([+28], +32) through `call` (its r3).
// 112-byte frame.
uint64_t SceneWheelsSetSizes(uint8_t* base, const GuestCall& call, uint32_t wheels, uint32_t a,
                             uint32_t b, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9,
                             uint32_t stack);

}  // namespace stand_models
