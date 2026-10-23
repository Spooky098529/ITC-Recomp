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
// UpdateTargetSpeed (sub_8213AB98), from UpdateSpeed; AiPath::TargetPoint
// (sub_8213B4D8), from SteerToTarget; and ThrottleForSpeed (sub_82139168),
// EaseThrottleForSteer (sub_82139B98), EaseThrottleForSlide (sub_82139BE8)
// and AiPath::HasKind2Ahead (sub_8213C148), from UpdatePedals; AiDriver::InitPath
// (sub_8213AD48) and AiSkill::Clear (sub_8213F700), from AiDrivers::Create;
// AiPathEntry::Widen (sub_8213C3A8) and AiPath::KeepToCornerSide
// (sub_8213CA18), from SetRanges; AiPath::SharesSections (sub_8213C010), from
// AddCornersAt and DropOffRouteCorners; and ApplyCornerLine (sub_8213BB58),
// from ApplyCornerLines; AiPath::PreferredSections (sub_8213C358), from
// CompareBranches and NormalizeByBranch; SpreadOffset (sub_8213B890), from
// ClampOffset; and AiPathEntry::NarrowToOtherEx (sub_8213C4C0), from
// NarrowBehind; AiPathEntry::BlockLeft (sub_8213C5C8) and BlockRight
// (sub_8213C630), AiDriver::AdjustForOther (sub_821452A0), from AvoidOnLeft
// and AvoidOnRight; MeasureOthers (sub_8213B650), from AvoidOthers; and
// AvoidOnLeft (sub_8213CB30), AvoidOnRight (sub_8213CD50) and
// ChooseSideForOther (sub_8213D788), from AvoidOthers (and ChooseSideForOther);
// AiSkill::TriggerStarts (sub_8213F958) and TriggerEnds (sub_8213FB48), from
// AiSkill::Update; AiDriver::UpdateSteerInput (sub_8213AC70) and
// AiPath::IsSlow (sub_8213C0E8), from UpdateSteering (and SteerToTarget);
// and UpdateSteering (sub_8213AEF0), SteerToTarget (sub_82138D78),
// UpdateSpeed (sub_8213ACD0), UpdatePedals (sub_8213A428) and LimitThrottle
// (sub_82139CB0), from AiDriver::Drive; AiPath::ContinueLine (sub_8213DB08),
// from PlanCorners and PlanLine; and ApproachCorner (sub_8213DE30),
// CornerToEntry (sub_8213E018), CornerToApex (sub_8213E1D0) and CornerToExit
// (sub_8213E360), from PlanCorners (sub_8213E7B8), from PlanLine; and
// ClampOffset (sub_8213CF58), from FollowTargets, ContinueLine,
// ApproachCorner and the CornerTo steps; AiOther::UpdateTimer (sub_821456A0),
// Mirror (sub_82145610) and Update (sub_821454C8), from
// AiDriver::CompareWith (sub_82145B08), from AiDrivers::UpdateAll.

#include "guest.h"

#include "ai_drivers/ai_drivers.h"
#include "hooks/guest_call.h"
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
              "AiDriver::UpdateSteerInput sub_8213AC70 (with its kinds 2 and 3's steps, "
              "sub_8213AB40, sub_82139A08, sub_8213A200, sub_82138FA8 and sub_821392D8, never "
              "seen in play), "
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
              "AiDriver::SteerToTarget sub_82138D78, AiDriver::UpdatePedals sub_8213A428, "
              "AiDrivers::Create sub_8213B1E0, AiPath::ApplyCornerLine sub_8213BB58, "
              "AiPath::ApplyCornerLines sub_8213CFC8, AiPath::KeepToCornerSide sub_8213CA18, "
              "AiPath::SetRanges sub_8213D670, AiPath::DropOffRouteCorners sub_8213DC08, "
              "AiPath::SpreadOffset sub_8213B890, AiPath::ClampOffset sub_8213CF58, "
              "AiPathEntry::NarrowToOtherEx sub_8213C4C0, AiPath::NarrowBehind sub_8213D140, "
              "AiPath::CompareBranches sub_8213C788, AiPath::NormalizeByBranch sub_8213C908, "
              "AiPath::MeasureOthers sub_8213B650, AiPath::AvoidOnLeft sub_8213CB30, "
              "AiPath::AvoidOnRight sub_8213CD50, AiPath::ChooseSideForOther sub_8213D788, "
              "AiPath::AvoidOthers sub_8213E690 (with its flag 0x10's step, sub_8213D898, never "
              "seen in play), AiSkill::TriggerStarts sub_8213F958, AiSkill::TriggerEnds "
              "sub_8213FB48, AiSkill::Update sub_8213FCF8 (their conditions sub_8213F7E0 and "
              "sub_8213F868, and kind 0's steps sub_8213F9A4 and sub_8213FB98, never seen in "
              "play, ported with them), AiDriver::UpdateSteering sub_8213AEF0, AiDriver::Drive "
              "sub_8213B058, AiPath::ContinueLine sub_8213DB08, AiPath::ApproachCorner sub_8213DE30, "
              "AiPath::CornerToEntry sub_8213E018, AiPath::CornerToApex sub_8213E1D0, "
              "AiPath::CornerToExit sub_8213E360, AiPath::PlanCorners sub_8213E7B8, "
              "AiPath::PlanLine sub_8213F4B0, AiPath::BranchMasksFor sub_8213D378 (with its kinds 1, 6 "
              "and 7's step, sub_8213D3D8, never seen in play), AiPath::LeaderBranchKind "
              "sub_8213D1F0 (its kind 5's step sub_8213D2B8 with it), AiPath::FindSideSection "
              "sub_8213D550, AiPath::FollowTargets sub_8213D9D8, AiPath::FollowLeaderBranch "
              "sub_8213EB48 (its kind 5's step sub_8213EC8C with it), AiDriver::CompareWith "
              "sub_82145B08, AiDrivers::UpdateAll sub_82139628 (the drivers' updates, through "
              "their +6148, on the translated code)) "
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
              "sub_8213AB98, sub_8213B4D8, sub_82139168, sub_82139B98, sub_82139BE8, "
              "sub_8213C148, sub_8213AD48, sub_8213F700, sub_8213C3A8, sub_8213CA18, sub_8213C010, "
              "sub_8213BB58, sub_8213C358, sub_8213B890, sub_8213C4C0, sub_8213C5C8, sub_8213C630, "
              "sub_821452A0, sub_8213B650, sub_8213CB30, sub_8213CD50, sub_8213D788, sub_8213F958, "
              "sub_8213FB48, sub_8213AC70, sub_8213C0E8, sub_8213AEF0, sub_82138D78, sub_8213ACD0, "
              "sub_8213A428, sub_82139CB0, sub_8213DB08, sub_8213DE30, sub_8213E018, sub_8213E1D0, "
              "sub_8213E360, sub_8213E7B8, sub_8213CF58, sub_821456A0, sub_82145610, sub_821454C8, "
              "sub_82145B08");

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

ITC_AI_DRIVERS(82139728, ctx.r3.u64 = Of(base, ctx.r3.u32))
ITC_AI_DRIVERS(820C96E8, ClearCarInput(base, ctx.r3.u32))

// --- Small reads and steps ---

ITC_AI_DRIVERS(82145408, WrapAngle(base, ctx.r3.u32))
ITC_AI_DRIVERS(82101760, ctx.r3.u64 = TimeFromTicks(ctx.r3.u32))
ITC_AI_DRIVERS(82144F50, ctx.r3.u64 = ReadCar(base, ctx.r3.u32))
ITC_AI_DRIVERS(82145230, UpdateHalfWidth(base, ctx.r3.u32))

// --- The path's corners and points ---

ITC_AI_DRIVERS(8213D318, ctx.r3.u64 = RaceAllowsSides(base))
ITC_AI_DRIVERS(8213B5D8, RefreshPoints(base, ctx.r3.u32))
// r3 the path's driver word, as the original leaves it.
ITC_AI_DRIVERS(8213D438, ctx.r3.u64 = ResetPath(base, ctx.r3.u32))

// --- The other racers (r3 kept, but KeepsGapFrom's) ---

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


// --- The path's entries and corners ---

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

ITC_AI_DRIVERS(82146608, ctx.r3.u64 = UpdateAgainstPlayer(base, ctx.r3.u32))

// --- Resets, steering input and the path's lists ---

// The word in the original's frame (112 bytes down); r3 left at the skill
// (+6048), as ResetToMode1 leaves it.
ITC_AI_DRIVERS(82139580, ResetControl(base, ctx.r3.u32, ctx.r1.u32 - 112); ctx.r3.u64 += 6048)
ITC_AI_DRIVERS(8213AAA8, ctx.r3.u64 = ResetDriver(base, ctx.r3.u32))
ITC_AI_DRIVERS(8213BA48, DropPassedSections(base, ctx.r3.u32))
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

// --- The drivers' table and the path's lines and ranges ---

// The original where operator new fails natively.
extern "C" REX_FUNC(sub_8213B1E0) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_ai_drivers)) {
    ctx.fpscr.disableFlushMode();
    uint64_t r3 = 0;
    if (Create(base, &r3)) {
      ctx.r3.u64 = r3;
      return;
    }
  }
  __imp__sub_8213B1E0(ctx, base);
}
// The original's frame 112 bytes down (r3 kept).
ITC_AI_DRIVERS(8213CFC8, ApplyCornerLines(base, ctx.r3.u32, ctx.r1.u32 - 112))
// r4 = &the count; the original's frame 128 bytes down.
ITC_AI_DRIVERS(8213D670, ctx.r3.u64 = SetRanges(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 128))
ITC_AI_DRIVERS(8213DC08, ctx.r3.u64 = DropOffRouteCorners(base, ctx.r3.u32))

// --- The path's offsets, narrowing and branches ---

// r4 = the entry after, r5 = the entry, r6 = &the step, r7 = &the limit; the
// original's frame 128 bytes down.
ITC_AI_DRIVERS(8213D140, ctx.r3.u64 = NarrowBehind(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r6.u32, ctx.r7.u32, ctx.r1.u32 - 128))
// r4, r5 = the sections, r6, r7 = &their numbers, r8 = the masks or 0.
ITC_AI_DRIVERS(8213C788, ctx.r3.s64 = CompareBranches(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                      ctx.r6.u32, ctx.r7.u32, ctx.r8.u32))
// The path's +5532 in mode 0 (no direct callers): r4 = the entry, r5 = the
// position, r6 = &the lane, r7 = the masks or 0.
ITC_AI_DRIVERS(8213C908, ctx.r3.u64 = NormalizeByBranch(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                        ctx.r6.u32, ctx.r7.u32))

// --- Making room for the other racers ---

// The original's frame 272 bytes down.
ITC_AI_DRIVERS(8213E690, ctx.r3.u64 = AvoidOthers(base, ctx.r3.u32, ctx.r1.u32 - 272))

// --- The skill's timed triggers (f1 the last trigger's value of kinds 6 and
// 7, as the original leaves it) ---

// r3 = the skill, r4 = its driver.
ITC_AI_DRIVERS(8213FCF8, {
  double f1 = 0;
  bool f1_set = false;
  ctx.r3.u64 = UpdateSkill(base, ctx.r3.u32, ctx.r4.u32, &f1, &f1_set);
  if (f1_set) {
    ctx.f1.f64 = f1;
  }
})

// --- Steering through the corners and driving ---

// r4 = the car's input record; the original's frame 160 bytes down.
ITC_AI_DRIVERS(8213B058, {
  double f1 = 0;
  ctx.r3.u64 = Drive(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 160, &f1);
  ctx.f1.f64 = f1;
})

// --- The path's line through the corners ---

// The original's frame 96 bytes down.
ITC_AI_DRIVERS(8213F4B0, ctx.r3.u64 = PlanLine(base, ctx.r3.u32, ctx.r1.u32 - 96))

// --- The path's branch kinds and lines ---

// r3 = &the kind; the list's address sign-extended, as the original's lis
// leaves it.
ITC_AI_DRIVERS(8213D378, ctx.r3.s64 = int32_t(BranchMasksFor(base, ctx.r3.u32)))
// r4 = &the kind, r5 = &the best.
ITC_AI_DRIVERS(8213D1F0, ctx.r3.u64 = LeaderBranchKind(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r4 = out, r5 = the point, r6 = the masks; the original's frame 208 bytes
// down.
ITC_AI_DRIVERS(8213D550, ctx.r3.u64 = FindSideSection(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                      ctx.r6.u32, ctx.r1.u32 - 208))
// The original's frame 128 bytes down (r3 kept).
ITC_AI_DRIVERS(8213D9D8, FollowTargets(base, ctx.r3.u32, ctx.r1.u32 - 128))
ITC_AI_DRIVERS(8213EB48, ctx.r3.u64 = FollowLeaderBranch(base, ctx.r3.u32))

// --- The drivers' updates and the pairs ---


namespace {
struct UpdateCall {
  PPCContext* ctx;
  uint8_t* base;
};
// A driver's update (its +6148), called from a frame like UpdateAll's (176
// bytes, returning to 0x82139684) with r3 = the driver, as the original.
uint64_t CallDriverUpdate(void* user, uint64_t driver, uint32_t function) {
  auto& call = *static_cast<UpdateCall*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  guest_call::InFrame(ctx, base, 176, 0x82139684, [&](uint32_t) {
    ctx.r3.u64 = driver;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
  });
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u64;
}
}  // namespace

// The updates are recompiled code (through the driver's +6148); the
// original's frame 176 bytes down.
ITC_AI_DRIVERS(82139628, {
  UpdateCall call;
  call.ctx = &ctx;
  call.base = base;
  ctx.r3.u64 = UpdateAll(base, ctx.r1.u32 - 176, CallDriverUpdate, &call);
})
