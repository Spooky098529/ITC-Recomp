// The stage's small setters (docs/functions/stand-models.md), step for step
// as the translated code (see stand_models.h).

#include "stand_models/stand_models.h"

#include "base/log.h"
#include "car_scene_draw/ops.h"
#include "debug/frame_state_diff.h"
#include "guest.h"
#include "load_poll/load_poll.h"
#include "scene_models/apply.h"
#include "scene_models/scene_models.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace stand_models {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kSetUpB = 0x820C5B28;
constexpr uint32_t kApplyRecord = 0x820DA758;
constexpr uint32_t kWheelsSetRecord = 0x820D8EC0;
constexpr uint32_t kSetTexture = 0x820D17D8;
constexpr uint32_t kApplyWheelPaint = 0x820D9C50;
constexpr uint32_t kApplyPaint = 0x820D9B68;
constexpr uint32_t kDecalScale = 0x82001BE0;

// Flush-to-zero off, as the callees' plain C++ expects at entry.
void FlushOff() {
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}

// A doubleword (ld, std): its high word first.
uint64_t Load64(uint8_t* base, uint32_t address) {
  return uint64_t(Load(base, address)) << 32 | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// SceneModel::ApplyRecord from `frame`: its plain C++, or where it declines
// (the race's path) its adapter, which runs the original.
uint64_t ApplyRecord(uint8_t* base, const GuestCall& call, uint32_t model, uint32_t frame,
                     uint32_t lr) {
  if (const auto r3 = scene_models::SceneModelApplyRecord(base, call, model, frame)) {
    return *r3;
  }
  return call(kApplyRecord, {model}, frame, lr);
}

}  // namespace

uint32_t StageModel(uint8_t* base, uint32_t holder) {
  const uint32_t stage = Load(base, holder);
  return vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7, Load(base, stage + 8));
}

uint64_t MakeModel(uint8_t* base, const GuestCall& call, uint64_t r3, uint32_t block,
                   uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217FCB0 GarageStage::MakeModel");
  const uint32_t frame = Frame(base, stack, 4384);
  const uint32_t holder = uint32_t(r3);
  const uint32_t scene = Load(base, kScene);
  if (Load(base, Load(base, holder) + 16) != 0) {
    return r3;
  }
  const uint32_t record = frame + 80;
  scene_models::SceneModelRecordConstruct(base, record);
  scene_models::SceneModelRecordReset(base, record);
  Store(base, frame + 96, block);
  const int32_t index = vehicle_accessors::ScenePoolsClaimB(base, scene, 7);
  Store(base, Load(base, holder) + 8, uint32_t(index));
  const uint32_t at = Load(base, Load(base, holder) + 8);
  const uint32_t model =
      uint32_t(call(kSetUpB, {scene, 7, at, record}, frame, 0x8217FD2C));
  Store(base, model + 4296, word);
  Store(base, Load(base, holder) + 6056, word);
  Store(base, Load(base, holder) + 16, 1);
  Store(base, Load(base, kHolder) + 20, 1);
  return model;
}

uint32_t SetInteriorClass(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178FE8 GarageStage::SetInteriorClass");
  Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  const int32_t v = int32_t(value);
  Store(base, model + 3808, v >= 7 ? 2 : v >= 3 ? 1 : 0);
  return model;
}

uint32_t SetWord5016(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179598 GarageStage::SetWord5016");
  Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  Store(base, model + 5016, value);
  return model;
}

uint32_t SetWheelWord(uint8_t* base, uint32_t holder, uint32_t value, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179A18 GarageStage::SetWheelWord");
  Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  Store(base, model + 4076, value);
  return model;
}

double LightValue(uint8_t* base, uint32_t points) {
  ITC_NATIVE_RUNNING("sub_820DF1A0 SceneParts::LightValue");
  return Lfs(base, points + 27664);
}

uint32_t SetLightsOn(uint8_t* base, uint32_t holder, uint32_t flag, uint32_t stack, double* f1) {
  ITC_NATIVE_RUNNING("sub_82179420 GarageStage::SetLightsOn");
  Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  if (flag != 0) {
    Store(base, model + 4976, 1);
    Stfs(base, model + 4980, Lfs(base, kZero));
  } else {
    Store(base, model + 4976, 0);
    const uint32_t points = Load(base, model + 5060) + 176;
    const double value = LightValue(base, points);
    Stfs(base, model + 4980, value);
    *f1 = value;
    return points;  // r3 as LightValue (a leaf) leaves it
  }
  return model;
}

uint32_t SetDecal(uint8_t* base, uint32_t holder, uint32_t entry, uint32_t a, uint32_t b,
                  double f1, double f2, double f3, double f4, uint32_t stack, uint32_t at) {
  if (at == kDecalA) {
    ITC_NATIVE_RUNNING("sub_82179818 GarageStage::SetDecalA");
  } else {
    ITC_NATIVE_RUNNING("sub_821798D8 GarageStage::SetDecalB");
  }
  const uint32_t frame = Frame(base, stack, 160);
  Store(base, frame + 80, 2);
  Store(base, frame + 84, 1);
  Store(base, frame + 88, 0);
  const uint32_t model = StageModel(base, holder);
  // The table read unchecked, as the original (rlwinm: the index's low 30
  // bits times 4, wrapping).
  const uint32_t word = Load(base, frame + 80 + (entry << 2));
  const uint32_t e = model + (word << 6);
  Stfs(base, e + at + 16, f1);
  Store(base, e + at + 0, a);
  Stfs(base, e + at + 20, f2);
  Store(base, e + at + 4, b);
  Stfs(base, e + at + 32, f3);
  Stfs(base, e + at + 52, f4);
  Stfs(base, e + at + 48, b != 0 ? Fmuls(f4, Lfs(base, kDecalScale)) : f4);
  return model;
}

uint64_t SetPart(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t slot,
                 uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178F00 GarageStage::SetPart");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  // The jump table (case 0 the split piece sub_82178F68): the parts
  // record's words, none for 13.
  static constexpr uint32_t kAt[16] = {3744, 3748, 3752, 3756, 3760, 3764, 3768, 3768,
                                       3772, 3772, 3776, 3780, 3784, 0,    3788, 3792};
  if (slot <= 15 && kAt[slot] != 0) {
    Store(base, model + kAt[slot], word);
  }
  return ApplyRecord(base, call, model, frame, 0x82178FD0);
}

uint64_t SetInteriorWord(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t kind,
                         uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179050 GarageStage::SetInteriorWord");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  const int32_t k = int32_t(kind);
  if (k == 1) {
    Store(base, model + 3908, word);
  } else if (k == 4) {
    Store(base, model + 3900, word);
  } else if (k == 5) {
    Store(base, model + 3896, word);
  }
  return ApplyRecord(base, call, model, frame, 0x821790B8);
}

uint64_t SetInteriorLevel(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t value,
                          uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821791C0 GarageStage::SetInteriorLevel");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  const int32_t v = int32_t(value);
  Store(base, model + 3908, v < 3 ? 0 : v < 6 ? 1 : 2);
  Store(base, model + 3912, v < 1 ? 0 : 1);
  return ApplyRecord(base, call, model, frame, 0x82179238);
}

uint64_t SetInteriorPair(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t a,
                         uint32_t b, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179250 GarageStage::SetInteriorPair");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  Store(base, model + 3904, a);
  Store(base, model + 3900, b);
  return ApplyRecord(base, call, model, frame, 0x82179294);
}

uint64_t ClearChoice(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t choice,
                     uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179378 GarageStage::ClearChoice");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  Store(base, choice * 12 + model + 3812, 0);
  return ApplyRecord(base, call, model, frame, 0x821793BC);
}

uint64_t SetAero(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t word,
                 uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179548 GarageStage::SetAero");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  Store(base, model + 3728, word);
  return ApplyRecord(base, call, model, frame, 0x82179580);
}

uint64_t ReloadDecals(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178EC0 GarageStage::ReloadDecals");
  const uint32_t frame = Frame(base, stack, 96);
  return scene_models::SceneModelReloadDecals(base, call, StageModel(base, holder), frame);
}

uint64_t SetDecalWord(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t word,
                      uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821793D0 GarageStage::SetDecalWord");
  const uint32_t frame = Frame(base, stack, 112);
  Store(base, StageModel(base, holder) + 3740, word);
  return scene_models::SceneModelReloadDecals(base, call, StageModel(base, holder), frame);
}

uint32_t SetSticker(uint8_t* base, uint32_t holder, uint32_t area, uint32_t slot, uint32_t word,
                    uint32_t flag, uint64_t r8, uint64_t r9, double f1, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217B3D8 GarageStage::SetSticker");
  Frame(base, stack, 128);
  // r8 and r9 into the caller's parameter area (their home slots), read
  // back from there below.
  Store64(base, stack + 64, r8);
  Store64(base, stack + 72, r9);
  const uint32_t model = StageModel(base, holder);
  // The record index's base and the code by the area (signed; any other
  // area 51 and 5).
  static constexpr uint32_t kIndex[7] = {9, 15, 21, 27, 33, 39, 45};
  static constexpr uint32_t kCode[7] = {8, 2, 1, 3, 4, 7, 6};
  const int32_t a = int32_t(area);
  const bool known = a >= 0 && a <= 6;
  const uint32_t index = slot + (known ? kIndex[a] : 51);
  const uint32_t e = model + 32 + (index << 6);
  Store(base, e + 8, known ? kCode[a] : 5);
  const uint64_t position0 = Load64(base, stack + 64);
  const uint64_t position1 = Load64(base, stack + 72);
  Store(base, e + 0, word);
  Store(base, e + 4, flag);
  const uint64_t size0 = Load64(base, stack + 96);
  const uint64_t size1 = Load64(base, stack + 104);
  Store64(base, e + 16, position0);
  Store64(base, e + 24, position1);
  Stfs(base, e + 32, f1);
  Store64(base, e + 48, size0);
  Store64(base, e + 56, size1);
  return model;
}

uint64_t WheelsSetTireTextures(uint8_t* base, const GuestCall& call, uint32_t wheels,
                               uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D8F18 Wheels::SetTireTextures");
  const uint32_t frame = Frame(base, stack, 96);
  constexpr uint64_t kEveryShape = ~0ull;
  call(kSetTexture, {wheels + 240, Load(base, wheels + 40), kEveryShape}, frame, 0x820D8F3C);
  return call(kSetTexture, {wheels + 704, Load(base, wheels + 56), kEveryShape}, frame,
              0x820D8F4C);
}

uint64_t SceneWheelsSetTireGrades(uint8_t* base, const GuestCall& call, uint32_t wheels,
                                  uint32_t front, uint32_t rear, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7470 SceneWheels::SetTireGrades");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t first = Load(base, wheels + 28);
  Store(base, wheels + 44, front);
  Store(base, wheels + 60, rear);
  call(kWheelsSetRecord, {first, wheels + 32}, frame, 0x820E749C);
  return WheelsSetTireTextures(base, call, Load(base, wheels + 28), frame);
}

uint64_t SetTireGrade(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t which,
                      uint32_t grade, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179130 GarageStage::SetTireGrade");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  const int32_t w = int32_t(which);
  if (w == 0 || w == 1) {
    Store(base, model + 4080, grade);
  }
  if (w == 0 || w == 2) {
    Store(base, model + 4096, grade);
  }
  return SceneWheelsSetTireGrades(base, call, Load(base, model + 5100), Load(base, model + 4080),
                                  Load(base, model + 4096), frame);
}

uint64_t TuningScreenSetTireGrade(uint8_t* base, const GuestCall& call, uint64_t r3,
                                  uint32_t which, uint32_t index, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821CCA38 TuningScreen::SetTireGrade");
  const uint32_t frame = Frame(base, stack, 112);
  static constexpr uint32_t kGrades[5] = {0, 1, 1, 2, 2};
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, frame + 80 + 4 * i, kGrades[i]);
  }
  const int32_t w = int32_t(which);
  if (w != 0 && w != 1 && w != 2) {
    return r3;
  }
  // The table read unchecked, as the original (the index's low 30 bits
  // times 4, wrapping).
  const uint32_t grade = Load(base, frame + 80 + (index << 2));
  return SetTireGrade(base, call, Load(base, Load(base, kHolder)), which, grade, frame);
}

uint32_t SecondWheelsPending(uint8_t* base, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179C48 GarageStage::SecondWheelsPending");
  Frame(base, stack, 96);
  return Load(base, StageModel(base, holder) + 5032);
}

uint64_t AddSecondWheels(uint8_t* base, const GuestCall& call, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179998 GarageStage::AddSecondWheels");
  const uint32_t frame = Frame(base, stack, 96);
  return scene_models::SceneModelAddSecondWheels(base, call, StageModel(base, holder), frame);
}

namespace {

// SceneModel::ApplyPaint (`wheels` false) or ApplyWheelPaint on `model`
// from `frame` with r3 the model: with `poll` (the load poll's flags on),
// its plain C++ inside its diagnostics' wrap (--debug_hide, nothing then,
// r3 the model as set for the call; --find_per_frame_state, as
// src/debug/frame_state_diff.cpp wrapped it); else its original through
// `call` with `lr`.
uint64_t Paint(uint8_t* base, const GuestCall& call, load_poll::Calls* poll, uint32_t model,
               bool wheels, uint32_t frame, uint32_t lr) {
  const uint32_t function = wheels ? kApplyWheelPaint : kApplyPaint;
  if (poll == nullptr) {
    return call(function, {model}, frame, lr);
  }
  if (debug::IsHidden(function)) {
    return model;
  }
  debug::FrameStateDiff_Before(base);
  FlushOff();
  const uint64_t r3 =
      wheels ? load_poll::SceneModelApplyWheelPaint(base, model, model, frame, *poll)
             : load_poll::SceneModelApplyPaint(base, model, model, frame);
  FlushOff();
  debug::FrameStateDiff_After(base, "draw step part", function);
  return r3;
}

}  // namespace

uint64_t ApplyWheelPaint(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                         uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821799D8 GarageStage::ApplyWheelPaint");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  return Paint(base, call, poll, model, true, frame, 0x82179A04);
}

uint64_t SetColorPiece(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                       uint32_t holder, uint32_t piece, double f1, double f2, double f3,
                       uint32_t stack) {
  switch (piece) {
    case 0: ITC_NATIVE_RUNNING("sub_8217B078 GarageStage::SetColor0"); break;
    case 1: ITC_NATIVE_RUNNING("sub_8217B108 GarageStage::SetColor1"); break;
    case 2: ITC_NATIVE_RUNNING("sub_8217B198 GarageStage::SetColor2"); break;
    case 3: ITC_NATIVE_RUNNING("sub_8217B228 GarageStage::SetColor3"); break;
    case 4: ITC_NATIVE_RUNNING("sub_8217B2B8 GarageStage::SetColor4"); break;
    default: ITC_NATIVE_RUNNING("sub_8217B348 GarageStage::SetColor5"); break;
  }
  const uint32_t frame = Frame(base, stack, 128);
  const uint32_t model = StageModel(base, holder);
  Stfs(base, frame + 80, f1);
  Stfs(base, frame + 84, f2);
  Stfs(base, frame + 88, f3);
  Stfs(base, frame + 92, Lfs(base, kOne));
  // The record's paint value 0 to 2 (+64, +80, +96): pieces 3 to 5 write
  // the same body values as 0 to 2, then apply the wheels' paint, which
  // reads +112 on (the original's, kept).
  Copy16(base, model + 64 + 16 * (piece % 3), frame + 80);
  const uint32_t lr = 0x8217B0E8 + 0x90 * piece;  // each piece's call + 4
  return Paint(base, call, poll, model, piece >= 3, frame, lr);
}

uint64_t SetColor(uint8_t* base, const GuestCall& call, load_poll::Calls* poll, uint64_t r3,
                  uint32_t which, double f1, double f2, double f3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217C6C8 GarageStage::SetColor");
  // Unsigned: above 5 nothing (r3 left); else the piece's tail call from
  // the same stack pointer.
  if (which > 5) {
    return r3;
  }
  return SetColorPiece(base, call, poll, uint32_t(r3), which, f1, f2, f3, stack);
}

uint64_t StepSecondWheels(uint8_t* base, const GuestCall& call, load_poll::Calls* poll,
                          uint32_t screen, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821CC890 TuningScreen::StepSecondWheels");
  const uint32_t frame = Frame(base, stack, 112);
  const auto holder = [&] { return Load(base, Load(base, kHolder)); };
  if (SecondWheelsPending(base, holder(), frame) == 0 &&
      int32_t(Load(base, screen + 25244)) == 1) {
    const uint64_t r3 = AddSecondWheels(base, call, holder(), frame);
    Store(base, screen + 25244, 2);
    return r3;
  }
  const uint32_t pending = SecondWheelsPending(base, holder(), frame);
  if (pending != 0 || int32_t(Load(base, screen + 25244)) != 2) {
    return pending;
  }
  const uint64_t r3 = ApplyWheelPaint(base, call, poll, holder(), frame);
  Store(base, screen + 25244, 0);
  return r3;
}

}  // namespace stand_models
