// The AI drivers natively: adapters in front of src/ai_drivers/
// (docs/functions/ai-drivers.md). Each leaves r3 (and f1 where the original
// leaves a value there) as the original does; those returning nothing leave
// r3 as it came, as the originals do. Functions whose recompiled callers have
// all been made native have no adapters (the flag switches their callers):
// AiDriver::ClearOtherIndexes (sub_82144ED8) and AiPath::ClearCorners
// (sub_8213C720), from AiPath::Reset; AiDriver::SpeedOverDistance
// (sub_821450E0) and AiDriver::CornerFactor (sub_821453A0), from
// AiDriver::LimitSpeed; LimitSpeed (sub_82139248), from LimitSpeedBlend;
// AiSkill::Load (sub_8213F730), from AiDriver::SetSkill; AiSkill::SetSource
// (sub_8213F900), from AiDriver::SetParams; AiPathEntry::SetLaneOffset
// (sub_8213B390), from AiPath::NormalizeByLane; AiDriver::ClearOthers
// (sub_821459B0), from ResetOthers and ClearCarState; and
// AiDriver::PickTargetBehind (sub_821451B8), from UpdateState5; IsFree
// (sub_821464B8), from UpdateAgainstPlayer; ClearFollow (sub_82146508) and
// ClearCarState (sub_82146588), from ResetFollow; IsSettled (sub_82146408),
// from AiDrivers::AllSettled; AiSkill::Reset (sub_8213F698), from
// ResetToMode1, ResetToMode0, AiSkill::Clear, Load and SetSource; SetSkill
// (sub_821393D8), from StartRace; AiPath::SetMode (sub_8213D508), from
// ResetToMode1, ResetToMode0 and StartRace; and AiOther::UpdateFlags
// (sub_82145450), from AiOther::Update and Mirror; ResetFollow
// (sub_821467B8), AiPath::ResetFor (sub_8213E658) and ResetOthers
// (sub_82145A48), from AiDriver::Reset; SetFlag2 (sub_82138D20), from
// ResetToMode1, ResetToMode0, ResetControl and StartRace; ResetToMode1
// (sub_82139450), from ResetControl; AccumulateSteer (sub_82138F00), from
// UpdateSteerInput; ClearPlan (sub_82139820), from StartRace, SetState2or3,
// SetStateFromList, SetState4 and ClearAll; Restart (sub_821395D0), from
// ResetAll; AiCorner::Init (sub_8213C698) and JoinCornerPair (sub_8213DD40),
// from AddCornersAt; JoinCorners (sub_8213BCD0) and JoinCornersShort
// (sub_8213BF28), from JoinCornerPair; UpdateState2, 3, 4 and 5
// (sub_82145F48, sub_821460C8, sub_82146220, sub_82146388), from
// UpdateState; and PickTarget (sub_82145118), from UpdateState2, 3 and 4 and
// IsSettled; LimitByCurve (sub_821390D0), LimitSpeedBlend (sub_82139B40) and
// AiPath::ClearCornerKind (sub_8213C710), from LimitForCorner; UpdateState
// (sub_82139AC8) and LimitForCorner (sub_8213A620), from UpdateTargetSpeed;
// and UpdateTargetSpeed (sub_8213AB98), from UpdateSpeed.

#include "guest.h"

#include "ai_drivers/ai_drivers.h"
#include "base/flags.h"

ITC_FLAG_BOOL(native_ai_drivers, true, "ITC",
              "Run the AI drivers (docs/functions/ai-drivers.md: AiDriver::InitPath "
              "sub_8213AD48, AiDriver::Of sub_82139728, CarInput::Clear sub_820C96E8, "
              "AiDriver::ClearFollow sub_82146508, AiDriver::ClearOtherIndexes sub_82144ED8, "
              "AiPath::ClearCorners sub_8213C720, AiPath::ClearCornerKind sub_8213C710, "
              "AiSkill::Reset sub_8213F698, Angle::Wrap sub_82145408, RaceTime::FromTicks "
              "sub_82101760, AiDriver::SetFlag2 sub_82138D20, AiDriver::IsFree sub_821464B8, "
              "AiDriver::SpeedOverDistance sub_821450E0, AiDriver::CornerFactor sub_821453A0, "
              "AiDriver::ReadCar sub_82144F50, AiDriver::UpdateHalfWidth sub_82145230, "
              "AiPath::TargetPoint sub_8213B4D8, AiSkill::Clear sub_8213F700, AiSkill::SetSource "
              "sub_8213F900, AiSkill::Load sub_8213F730, AiDriver::LimitSpeed sub_82139248, "
              "AiDriver::LimitByCurve sub_821390D0, AiDriver::EaseThrottleForSteer sub_82139B98, "
              "AiPath::HasKind2Ahead sub_8213C148, AiPath::IsSlow sub_8213C0E8, "
              "AiPath::PreferredSections sub_8213C358, AiPath::RaceAllowsSides sub_8213D318, "
              "AiPath::RefreshPoints sub_8213B5D8, AiPath::Reset sub_8213D438, "
              "AiPathEntry::SetLaneOffset sub_8213B390, AiOther::UpdateFlags sub_82145450, "
              "AiOther::UpdateTimer sub_821456A0, AiDriver::ClearOthers sub_821459B0, "
              "AiDriver::PickTarget sub_82145118, AiDriver::PickTargetBehind sub_821451B8, "
              "AiDriver::KeepsGapFrom sub_82145048, AiDriver::SetParams sub_82138D00, "
              "AiDriver::SetSkill sub_821393D8, AiDriver::ClearPlan sub_82139820, "
              "AiDriver::LimitSpeedBlend sub_82139B40, AiPath::SetMode sub_8213D508, "
              "AiPath::ResetFor sub_8213E658, AiPath::NormalizeByLane sub_8213B428, "
              "AiDriver::UpdateLane sub_82144EF8, AiDriver::ResetOthers sub_82145A48, "
              "AiDriver::ClearCarState sub_82146588, AiDriver::UpdateState4 sub_82146220, "
              "AiDriver::UpdateState5 sub_82146388, AiDriver::IsSettled sub_82146408, "
              "AiOther::Mirror sub_82145610, AiPathEntry::BlockLeft sub_8213C5C8, "
              "AiPathEntry::BlockRight sub_8213C630, AiCorner::Init sub_8213C698, "
              "AiPath::WithinRoadAt sub_8213C1A0, AiDriver::ResetToMode1 sub_82139450, "
              "AiDriver::ResetToMode0 sub_821394F0, AiDriver::StartRace sub_82139DA0, "
              "AiDriver::ResetFollow sub_821467B8, AiDrivers::AllSettled sub_8213ADF8, "
              "AiDriver::AccumulateSteer sub_82138F00, AiDriver::ThrottleForSpeed sub_82139168, "
              "AiDriver::EaseThrottleForSlide sub_82139BE8, AiDriver::LimitThrottle "
              "sub_82139CB0, AiDriver::AdjustForOther sub_821452A0, "
              "AiDriver::UpdateAgainstPlayer sub_82146608, AiOther::Update sub_821454C8, "
              "AiDriver::ResetControl sub_82139580, AiDriver::Reset sub_8213AAA8, "
              "AiDriver::UpdateSteerInput sub_8213AC70 (its kinds 2 and 3, sub_8213AB40 and "
              "sub_82139A08, never seen in play, on the translated code), "
              "AiPath::DropPassedSections sub_8213BA48, AiPath::SharesSections sub_8213C010, "
              "AiPathEntry::Widen sub_8213C3A8, AiPathEntry::NarrowToOther sub_8213C420, "
              "AiPath::AdvanceCorners sub_8213D020, AiDriver::Restart sub_821395D0, "
              "AiDrivers::ResetAll sub_82139750, AiDriver::SetState2or3 sub_82139E40, "
              "AiDriver::SetStateFromList sub_82139EA8, AiDriver::SetState4 sub_82139FA0, "
              "AiDrivers::ClearAll sub_8213A140, AiPath::JoinCorners sub_8213BCD0, "
              "AiPath::JoinCornersShort sub_8213BF28, AiPath::JoinCornerPair sub_8213DD40 "
              "(its equal directions' steps, sub_8213BDF0 and sub_8213BE80, never seen in play, "
              "on the translated code), AiPath::AddCornersAt sub_8213E970, "
              "AiDriver::UpdateState2 sub_82145F48, AiDriver::UpdateState3 sub_821460C8, "
              "AiDriver::UpdateState sub_82139AC8 (with its state 6's step, sub_821462A0, never "
              "seen in play), AiDriver::LimitForCorner sub_8213A620, "
              "AiDriver::UpdateTargetSpeed sub_8213AB98, AiDriver::UpdateSpeed sub_8213ACD0, "
              "AiDriver::SteerToTarget sub_82138D78, AiDriver::UpdatePedals sub_8213A428) "
              "natively instead of "
              "through the translated code (the same words, bit for bit; checked by "
              "--run_harnesses=ai_drivers). Those "
              "reached only from native callers have no adapters, and the flag switches their "
              "callers: sub_82144ED8, sub_8213C720, sub_821450E0, sub_821453A0, sub_82139248, "
              "sub_8213F730, sub_8213F900, sub_8213B390, sub_821459B0, sub_821451B8, sub_821464B8, "
              "sub_82146508, sub_82146588, sub_82146408, sub_8213F698, sub_821393D8, sub_8213D508, "
              "sub_82145450, sub_821467B8, sub_82138D20, sub_82139450, sub_82138F00, sub_8213E658, "
              "sub_82145A48, sub_82139820, sub_821395D0, sub_8213C698, sub_8213DD40, "
              "sub_8213BCD0, sub_8213BF28, sub_82145F48, sub_821460C8, sub_82146220, sub_82146388, "
              "sub_82145118, sub_821390D0, sub_82139B40, sub_8213C710, sub_82139AC8, sub_8213A620, "
              "sub_8213AB98");

// Defines an adapter: `body` uses `ctx` and `base`.
#define ITC_AI_DRIVERS(address, body)   \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_ai_drivers)) { \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    ctx.fpscr.disableFlushMode();       \
    body;                               \
  }

using namespace ai_drivers;

// --- Set-up and clears (r3 kept) ---

ITC_AI_DRIVERS(8213AD48, InitPath(base, ctx.r3.u32))
ITC_AI_DRIVERS(82139728, ctx.r3.u64 = Of(base, ctx.r3.u32))
ITC_AI_DRIVERS(820C96E8, ClearCarInput(base, ctx.r3.u32))

// --- Small reads and steps ---

ITC_AI_DRIVERS(82145408, WrapAngle(base, ctx.r3.u32))
ITC_AI_DRIVERS(82101760, ctx.r3.u64 = TimeFromTicks(ctx.r3.u32))
ITC_AI_DRIVERS(82144F50, ctx.r3.u64 = ReadCar(base, ctx.r3.u32))
ITC_AI_DRIVERS(82145230, UpdateHalfWidth(base, ctx.r3.u32))
// f1 = the distance ahead, r4 = out.
ITC_AI_DRIVERS(8213B4D8, ctx.r3.u64 = TargetPoint(base, ctx.r3.u32, ctx.f1.f64, ctx.r4.u32))

// --- The skill (r3 kept) ---

ITC_AI_DRIVERS(8213F700, ClearSkill(base, ctx.r3.u32))

// --- Speeds and steering (r3 kept) ---

ITC_AI_DRIVERS(82139B98, EaseThrottleForSteer(base, ctx.r3.u32))

// --- The path's corners and points ---

ITC_AI_DRIVERS(8213C148, ctx.r3.u64 = HasKind2Ahead(base, ctx.r3.u32))
ITC_AI_DRIVERS(8213C0E8, ctx.r3.u64 = IsSlow(base, ctx.r3.u32))
// r3, r4 = &the bytes; the list's address sign-extended, as the original's.
ITC_AI_DRIVERS(8213C358, ctx.r3.s64 = int32_t(PreferredSections(base, ctx.r3.u32, ctx.r4.u32)))
ITC_AI_DRIVERS(8213D318, ctx.r3.u64 = RaceAllowsSides(base))
ITC_AI_DRIVERS(8213B5D8, RefreshPoints(base, ctx.r3.u32))
// r3 the path's driver word, as the original leaves it.
ITC_AI_DRIVERS(8213D438, ctx.r3.u64 = ResetPath(base, ctx.r3.u32))

// --- The other racers (r3 kept, but KeepsGapFrom's) ---

// r4 = &the other's index, r5 = &the limit.
ITC_AI_DRIVERS(821456A0, UpdateOtherTimer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r4 = &the other's index.
ITC_AI_DRIVERS(82145048, ctx.r3.u64 = KeepsGapFrom(base, ctx.r3.u32, ctx.r4.u32))

// --- Set-up, modes and the plan ---

// r4 = the object or 0; r3 left at the skill (+6048), as the original's
// tail call leaves it.
ITC_AI_DRIVERS(82138D00, SetParams(base, ctx.r3.u32, ctx.r4.u32); ctx.r3.u64 += 6048)
// The path's +5532 in mode 1 (no direct callers): r3 not read, r4 = the
// entry, r5 = the position, r6 = &the lane, r7 = the masks or 0.
ITC_AI_DRIVERS(8213B428, ctx.r3.u64 = NormalizeByLane(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                                      ctx.r7.u32))
// The original's frame 128 bytes down; r3 left at its clamped position
// (ClampPosition's out), as the original leaves it.
ITC_AI_DRIVERS(82144EF8, UpdateLane(base, ctx.r3.u32, ctx.r1.u32 - 128);
               ctx.r3.u64 = ctx.r1.u32 - 128 + 80)

// --- The other racers' records and the states ---

// r3 = out, r4 = the record, r5 = &the value.
ITC_AI_DRIVERS(82145610, MirrorOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// --- The path's entries and corners ---

// r4 = &the position across, r5 = &the width, r6 = &out.
ITC_AI_DRIVERS(8213C5C8, BlockLeft(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
ITC_AI_DRIVERS(8213C630, BlockRight(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
// r4 = &the offset.
ITC_AI_DRIVERS(8213C1A0, ctx.r3.u64 = WithinRoadAt(base, ctx.r3.u32, ctx.r4.u32))

// --- Starts and resets ---

// The words passed by address in the original's frame (128 bytes down); r3
// left at the skill (+6048), as the original leaves it.
ITC_AI_DRIVERS(821394F0, ResetToMode0(base, ctx.r3.u32, ctx.r1.u32 - 128); ctx.r3.u64 += 6048)
ITC_AI_DRIVERS(82139DA0, ctx.r3.u64 = StartRace(base, ctx.r3.u32, ctx.r1.u32 - 112))
// The copies in the original's frame (6,320 bytes down); the original where
// one can't be made natively.
extern "C" REX_FUNC(sub_8213ADF8) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_ai_drivers)) {
    ctx.fpscr.disableFlushMode();
    uint32_t result = 0;
    if (AllSettled(base, ctx.r1.u32 - 6320, &result)) {
      ctx.r3.u64 = result;
      return;
    }
  }
  __imp__sub_8213ADF8(ctx, base);
}

// --- Pedals and steering (r3 kept) ---

ITC_AI_DRIVERS(82139168, ThrottleForSpeed(base, ctx.r3.u32))
// r4 = &the flag.
ITC_AI_DRIVERS(82139BE8, EaseThrottleForSlide(base, ctx.r3.u32, ctx.r4.u32))
ITC_AI_DRIVERS(82139CB0, LimitThrottle(base, ctx.r3.u32))
// r4 = the record, r5 = &the mode.
ITC_AI_DRIVERS(821452A0, AdjustForOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_AI_DRIVERS(82146608, ctx.r3.u64 = UpdateAgainstPlayer(base, ctx.r3.u32))
// r4 = &the new gap, r5 = &the length.
ITC_AI_DRIVERS(821454C8, UpdateOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// --- Resets, steering input and the path's lists ---

// The word in the original's frame (112 bytes down); r3 left at the skill
// (+6048), as ResetToMode1 leaves it.
ITC_AI_DRIVERS(82139580, ResetControl(base, ctx.r3.u32, ctx.r1.u32 - 112); ctx.r3.u64 += 6048)
ITC_AI_DRIVERS(8213AAA8, ctx.r3.u64 = ResetDriver(base, ctx.r3.u32))
// Kinds 2 and 3 (never seen in play): the translated steps, tail-called as
// the original does, with the record in r4.
extern "C" REX_FUNC(sub_8213AC70) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ai_drivers)) {
    __imp__sub_8213AC70(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  uint32_t record = 0;
  const uint32_t kind = UpdateSteerInput(base, ctx.r3.u32, &record);
  if (kind == 2) {
    ctx.r4.u64 = record;
    sub_8213AB40(ctx, base);
  } else if (kind == 3) {
    ctx.r4.u64 = record;
    sub_82139A08(ctx, base);
  }
}
ITC_AI_DRIVERS(8213BA48, DropPassedSections(base, ctx.r3.u32))
// r4 = the record.
ITC_AI_DRIVERS(8213C010, ctx.r3.u64 = SharesSections(base, ctx.r3.u32, ctx.r4.u32))
// r4 = &the margin, r5 = &the shift.
ITC_AI_DRIVERS(8213C3A8, Widen(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r4 = the other entry or 0, r5 = &the margin, r6 = &the offset.
ITC_AI_DRIVERS(8213C420, NarrowToOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
// r4 = &the slices; the original where the records' moves can't be made
// natively.
extern "C" REX_FUNC(sub_8213D020) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_ai_drivers)) {
    ctx.fpscr.disableFlushMode();
    uint32_t r3 = 0;
    if (AdvanceCorners(base, ctx.r3.u32, ctx.r4.u32, &r3)) {
      ctx.r3.u64 = r3;
      return;
    }
  }
  __imp__sub_8213D020(ctx, base);
}

// --- The states and the drivers' resets (the originals' frames: 112 or 128
// bytes down) ---

ITC_AI_DRIVERS(82139750, ctx.r3.u64 = ResetAll(base, ctx.r1.u32 - 128))
// r4 = &the word, r5 = &the flag.
ITC_AI_DRIVERS(82139E40, ctx.r3.u64 = SetState2or3(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r1.u32 - 112))
// r4 = &the word, r5 = &the count, r6 = the list.
ITC_AI_DRIVERS(82139EA8, ctx.r3.u64 = SetStateFromList(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                       ctx.r6.u32, ctx.r1.u32 - 128))
// r4 = &the word.
ITC_AI_DRIVERS(82139FA0, ctx.r3.u64 = SetState4(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112))
ITC_AI_DRIVERS(8213A140, ctx.r3.u64 = ClearAll(base, ctx.r1.u32 - 128))

// --- The corners' joins ---

namespace {
// JoinCornerPair's equal directions (never seen in play, from AddCornersAt):
// the translated step 2 (sub_8213BDF0) or 3 (sub_8213BE80), called from
// JoinCornerPair's frame (`frame`, its back chain `chain`) as the original does, with r6 =
// &the gap there; r3, r4 and r5 as set.
void RunJoinStep(PPCContext& ctx, uint8_t* base, uint32_t step, uint32_t frame,
                 uint32_t chain) {
  const uint32_t saved = ctx.r1.u32;
  guest::StoreU32(base, frame, chain);
  ctx.r1.u32 = frame;
  ctx.r6.u64 = frame + 80;
  if (step == 2) {
    sub_8213BDF0(ctx, base);
  } else {
    sub_8213BE80(ctx, base);
  }
  ctx.r1.u32 = saved;
}
}  // namespace

// r4 = the path entry; the original's frame 144 bytes down (JoinCornerPair's
// 112 below it).
extern "C" REX_FUNC(sub_8213E970) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ai_drivers)) {
    __imp__sub_8213E970(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t path = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 144;
  uint32_t r3 = 0, a = 0, b = 0;
  const uint32_t step = AddCornersAt(base, path, ctx.r4.u32, frame, &r3, &a, &b);
  if (step >= 2) {
    guest::StoreU32(base, frame, ctx.r1.u32);
    ctx.r3.u64 = path;
    ctx.r4.u64 = a;
    ctx.r5.u64 = b;
    RunJoinStep(ctx, base, step, frame - 112, frame);
    AddCornersAtFinish(base, path);
    return;
  }
  ctx.r3.u64 = r3;
}

// --- The target speed (f1 LimitSpeedBlend's, when it ran) ---

// The original's frame 96 bytes down.
ITC_AI_DRIVERS(8213ACD0, {
  double f1 = 0;
  bool f1_set = false;
  ctx.r3.u64 = UpdateSpeed(base, ctx.r3.u32, ctx.r1.u32 - 96, &f1, &f1_set);
  if (f1_set) {
    ctx.f1.f64 = f1;
  }
})

// --- Steering at the target and the pedals ---

// f1 as the original leaves it.
ITC_AI_DRIVERS(82138D78, {
  double f1 = 0;
  ctx.r3.u64 = SteerToTarget(base, ctx.r3.u32, &f1);
  ctx.f1.f64 = f1;
})
// r3 kept; the original's frame 112 bytes down.
ITC_AI_DRIVERS(8213A428, UpdatePedals(base, ctx.r3.u32, ctx.r1.u32 - 112))
