// The AI drivers natively: adapters in front of src/ai_drivers/
// (docs/functions/ai-drivers.md). Each leaves r3 (and f1 where the original
// leaves a value there) as the original does; those returning nothing leave
// r3 as it came, as the originals do.

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
              "AiPath::TargetPoint sub_8213B4D8) natively instead of through the translated "
              "code (the same words, bit for bit; checked by --run_harnesses=ai_drivers)");

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
ITC_AI_DRIVERS(82144ED8, ClearOtherIndexes(base, ctx.r3.u32))
ITC_AI_DRIVERS(8213C720, ClearCorners(base, ctx.r3.u32))
ITC_AI_DRIVERS(8213C710, ClearCornerKind(base, ctx.r3.u32))
ITC_AI_DRIVERS(8213F698, ResetSkill(base, ctx.r3.u32))

// --- Small reads and steps ---

ITC_AI_DRIVERS(82145408, WrapAngle(base, ctx.r3.u32))
ITC_AI_DRIVERS(82101760, ctx.r3.u64 = TimeFromTicks(ctx.r3.u32))
// r4 = &the word.
ITC_AI_DRIVERS(82138D20, ctx.r3.u64 = SetFlag2(base, ctx.r3.u32, ctx.r4.u32))
ITC_AI_DRIVERS(821464B8, ctx.r3.u64 = IsFree(base, ctx.r3.u32))
// r4 = &the distance; f1.
ITC_AI_DRIVERS(821450E0, ctx.f1.f64 = SpeedOverDistance(base, ctx.r3.u32, ctx.r4.u32))
// r4 = &the curvature, r5 = &the width; f1.
ITC_AI_DRIVERS(821453A0, ctx.f1.f64 = CornerFactor(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_AI_DRIVERS(82144F50, ctx.r3.u64 = ReadCar(base, ctx.r3.u32))
ITC_AI_DRIVERS(82145230, UpdateHalfWidth(base, ctx.r3.u32))
// f1 = the distance ahead, r4 = out.
ITC_AI_DRIVERS(8213B4D8, ctx.r3.u64 = TargetPoint(base, ctx.r3.u32, ctx.f1.f64, ctx.r4.u32))
