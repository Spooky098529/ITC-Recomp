// The AI drivers natively: adapters in front of src/ai_drivers/
// (docs/functions/ai-drivers.md). Each leaves r3 (and f1 where the original
// leaves a value there) as the original does; those returning nothing leave
// r3 as it came, as the originals do. Functions whose recompiled callers have
// all been made native have no adapters (the flag switches their callers):
// AiDriver::ClearOtherIndexes (sub_82144ED8) and AiPath::ClearCorners
// (sub_8213C720), from AiPath::Reset; AiDriver::SpeedOverDistance
// (sub_821450E0) and AiDriver::CornerFactor (sub_821453A0), from
// AiDriver::LimitSpeed.

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
              "AiPath::WithinRoadAt sub_8213C1A0) natively instead of through the translated "
              "code (the same words, bit for bit; checked by --run_harnesses=ai_drivers). Those "
              "reached only from native callers have no adapters, and the flag switches their "
              "callers: sub_82144ED8, sub_8213C720, sub_821450E0, sub_821453A0");

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
ITC_AI_DRIVERS(82146508, ClearFollow(base, ctx.r3.u32))
ITC_AI_DRIVERS(8213C710, ClearCornerKind(base, ctx.r3.u32))
ITC_AI_DRIVERS(8213F698, ResetSkill(base, ctx.r3.u32))

// --- Small reads and steps ---

ITC_AI_DRIVERS(82145408, WrapAngle(base, ctx.r3.u32))
ITC_AI_DRIVERS(82101760, ctx.r3.u64 = TimeFromTicks(ctx.r3.u32))
// r4 = &the word.
ITC_AI_DRIVERS(82138D20, ctx.r3.u64 = SetFlag2(base, ctx.r3.u32, ctx.r4.u32))
ITC_AI_DRIVERS(821464B8, ctx.r3.u64 = IsFree(base, ctx.r3.u32))
ITC_AI_DRIVERS(82144F50, ctx.r3.u64 = ReadCar(base, ctx.r3.u32))
ITC_AI_DRIVERS(82145230, UpdateHalfWidth(base, ctx.r3.u32))
// f1 = the distance ahead, r4 = out.
ITC_AI_DRIVERS(8213B4D8, ctx.r3.u64 = TargetPoint(base, ctx.r3.u32, ctx.f1.f64, ctx.r4.u32))

// --- The skill (r3 kept) ---

ITC_AI_DRIVERS(8213F700, ClearSkill(base, ctx.r3.u32))
// r4 = the source or 0.
ITC_AI_DRIVERS(8213F900, SetSkillSource(base, ctx.r3.u32, ctx.r4.u32))
ITC_AI_DRIVERS(8213F730, LoadSkill(base, ctx.r3.u32))

// --- Speeds and steering (r3 kept) ---

// r4 = &the speed, r5, r6 = CornerFactor's, r7 = &the slices; f1 as the
// original leaves it; the original's frame 128 bytes down.
ITC_AI_DRIVERS(82139248, ctx.f1.f64 = LimitSpeed(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                 ctx.r6.u32, ctx.r7.u32, ctx.r1.u32 - 128))
// r4 = the table, r5 = &the fraction, r6 = &the index, r7 = the record or 0.
ITC_AI_DRIVERS(821390D0, LimitByCurve(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                      ctx.r7.u32))
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
ITC_AI_DRIVERS(8213B390, SetLaneOffset(base, ctx.r3.u32))

// --- The other racers (r3 kept, but KeepsGapFrom's) ---

ITC_AI_DRIVERS(82145450, UpdateOtherFlags(base, ctx.r3.u32))
// r4 = &the other's index, r5 = &the limit.
ITC_AI_DRIVERS(821456A0, UpdateOtherTimer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_AI_DRIVERS(821459B0, ClearOthers(base, ctx.r3.u32))
ITC_AI_DRIVERS(82145118, PickTarget(base, ctx.r3.u32))
ITC_AI_DRIVERS(821451B8, PickTargetBehind(base, ctx.r3.u32))
// r4 = &the other's index.
ITC_AI_DRIVERS(82145048, ctx.r3.u64 = KeepsGapFrom(base, ctx.r3.u32, ctx.r4.u32))

// --- Set-up, modes and the plan ---

// r4 = the object or 0; r3 left at the skill (+6048), as the original's
// tail call leaves it.
ITC_AI_DRIVERS(82138D00, SetParams(base, ctx.r3.u32, ctx.r4.u32); ctx.r3.u64 += 6048)
// r4 = &the word; r3 left at the skill.
ITC_AI_DRIVERS(821393D8, SetSkill(base, ctx.r3.u32, ctx.r4.u32); ctx.r3.u64 += 6048)
ITC_AI_DRIVERS(82139820, ctx.r3.u64 = ClearPlan(base, ctx.r3.u32))
// r4 = &the speed, r5 = &(a, b), r6 = &t, r7 = &the weight, r8 = &the
// slices; the original's frame 96 bytes down.
ITC_AI_DRIVERS(82139B40, ctx.f1.f64 = LimitSpeedBlend(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                      ctx.r6.u32, ctx.r7.u32, ctx.r8.u32,
                                                      ctx.r1.u32 - 96))
// r4 = &the mode.
ITC_AI_DRIVERS(8213D508, ctx.r3.u64 = SetMode(base, ctx.r3.u32, ctx.r4.u32))
// r4 = the driver.
ITC_AI_DRIVERS(8213E658, ctx.r3.u64 = ResetFor(base, ctx.r3.u32, ctx.r4.u32))
// The path's +5532 in mode 1 (no direct callers): r3 not read, r4 = the
// entry, r5 = the position, r6 = &the lane, r7 = the masks or 0.
ITC_AI_DRIVERS(8213B428, ctx.r3.u64 = NormalizeByLane(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                                      ctx.r7.u32))
// The original's frame 128 bytes down; r3 left at its clamped position
// (ClampPosition's out), as the original leaves it.
ITC_AI_DRIVERS(82144EF8, UpdateLane(base, ctx.r3.u32, ctx.r1.u32 - 128);
               ctx.r3.u64 = ctx.r1.u32 - 128 + 80)

// --- The other racers' records and the states ---

ITC_AI_DRIVERS(82145A48, ctx.r3.u64 = ResetOthers(base, ctx.r3.u32))
ITC_AI_DRIVERS(82146588, ClearCarState(base, ctx.r3.u32))
// r4 = &the target speed.
ITC_AI_DRIVERS(82146220, UpdateState4(base, ctx.r3.u32, ctx.r4.u32))
ITC_AI_DRIVERS(82146388, ctx.r3.u64 = UpdateState5(base, ctx.r3.u32, ctx.r4.u32))
ITC_AI_DRIVERS(82146408, ctx.r3.u64 = IsSettled(base, ctx.r3.u32))
// r3 = out, r4 = the record, r5 = &the value.
ITC_AI_DRIVERS(82145610, MirrorOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))

// --- The path's entries and corners ---

// r4 = &the position across, r5 = &the width, r6 = &out.
ITC_AI_DRIVERS(8213C5C8, BlockLeft(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
ITC_AI_DRIVERS(8213C630, BlockRight(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
// r4 = the corner's data, r5 = &the slice's corner entry, r6 = &the count.
ITC_AI_DRIVERS(8213C698, InitCorner(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
// r4 = &the offset.
ITC_AI_DRIVERS(8213C1A0, ctx.r3.u64 = WithinRoadAt(base, ctx.r3.u32, ctx.r4.u32))
