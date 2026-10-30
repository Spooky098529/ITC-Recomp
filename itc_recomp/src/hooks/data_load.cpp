// The data load at boot natively: adapters in front of src/data_load/
// (docs/functions/data-load.md).

#include "base/flags.h"
#include "data_load/data_load.h"
#include "guest.h"
#include "hooks/heap.h"

ITC_FLAG_BOOL(native_data_load, true, "ITC",
              "Run the data load at boot (docs/functions/data-load.md: Defaults::Init "
              "sub_820CCDC0 with Bits224::Fill sub_820CCD10 and Bits480::Fill sub_820CCD68; "
              "Record5232::Record5232 sub_82104C20 with Member1136::Member1136 sub_82102888, "
              "Member4080::Member4080 sub_82104690 and Part464::Part464 sub_821042A0; with "
              "--native_heap, RaceData::Create sub_82102208, InputRecordings::Create "
              "sub_82105508 with InputRecordings::InputRecordings sub_821054A8 and "
              "Replay::Create sub_82105560; no adapters for the fills, the records' members "
              "and InputRecordings::InputRecordings: only their callers' plain C++ calls them) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=data_load)");

namespace dl = data_load;

extern "C" REX_FUNC(sub_820CCDC0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_data_load)) {
    __imp__sub_820CCDC0(ctx, base);
    return;
  }
  // r3 as lis and addi leave the address: sign-extended.
  ctx.r3.s64 = int32_t(dl::DefaultsInit(base, ctx.r1.u32 - 128));
}

extern "C" REX_FUNC(sub_82104C20) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_data_load)) {
    __imp__sub_82104C20(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = dl::Record5232Construct(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_82102208) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_data_load) && hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    if (const auto data = dl::RaceDataCreate(base)) {
      ctx.r3.u64 = *data;
      return;
    }
  }
  __imp__sub_82102208(ctx, base);
}

extern "C" REX_FUNC(sub_82105508) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_data_load) && hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    if (const auto block = dl::InputRecordingsCreate(base)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_82105508(ctx, base);
}

extern "C" REX_FUNC(sub_82105560) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_data_load) && hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    if (const auto r3 = dl::ReplayCreate(base)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_82105560(ctx, base);
}
