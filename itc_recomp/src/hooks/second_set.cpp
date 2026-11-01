// The second set's objects natively: adapters in front of src/second_set/
// (docs/functions/second-set.md), with the register steps the harness shares
// (hooks/second_set.h).

#include "hooks/second_set.h"

#include "base/flags.h"
#include "native/buffers.h"
#include "second_set/points.h"

ITC_FLAG_BOOL(native_second_set, true, "ITC",
              "Run the second set of the car scene's objects (docs/functions/second-set.md: "
              "PointRecord::SetNode sub_820CE8D8 and SetShape sub_820CE900, with native memcpy "
              "PointRecord::Clear sub_820CEA08, PointRecord::PointRecord sub_820D0270 and "
              "ModelPoint::FindShape sub_820CEB30) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=second_set)");

namespace hooks {

namespace ss = second_set;

void SecondSetPointSetNode(PPCContext& ctx, uint8_t* base) {
  ss::PointRecordSetNode(base, ctx.r3.u32, ctx.r4.u32);
}

void SecondSetPointSetShape(PPCContext& ctx, uint8_t* base) {
  ss::PointRecordSetShape(base, ctx.r3.u32, ctx.r4.u32);
}

void SecondSetPointClear(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_820CEA08(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ss::PointRecordClear(base, ctx.r3.u32, ctx.r1.u32);
}

void SecondSetPointConstruct(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_820D0270(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ss::PointRecordConstruct(base, ctx.r3.u32, ctx.r1.u32);
}

void SecondSetFindShape(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_820CEB30(ctx, base);
    return;
  }
  ctx.r3.u64 = ss::ModelPointFindShape(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

}  // namespace hooks

#define ITC_SECOND_SET(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_second_set)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

ITC_SECOND_SET(820CE8D8, SecondSetPointSetNode)
ITC_SECOND_SET(820CE900, SecondSetPointSetShape)
ITC_SECOND_SET(820CEA08, SecondSetPointClear)
ITC_SECOND_SET(820D0270, SecondSetPointConstruct)
ITC_SECOND_SET(820CEB30, SecondSetFindShape)
