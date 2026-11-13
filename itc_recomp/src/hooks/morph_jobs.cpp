// The morph jobs natively: adapters in front of src/morph_jobs/
// (docs/functions/morph-jobs.md), with the register steps the harness shares
// (hooks/morph_jobs.h). The job's and the work item's slots are reached
// through vtables from recompiled code (the worker pools' post and loop,
// Delegate::Invoke, command 22, the replay's release list), so their
// adapters stay; the makers' slot 1 also from Model::MorphShape's plain C++
// (src/car_draw/morph.cpp), which calls the plain C++.

#include "hooks/morph_jobs.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "hooks/second_set.h"
#include "memory/c_runtime_heap.h"

ITC_FLAG_BOOL(native_morph_jobs, true, "ITC",
              "Run the morph jobs (docs/functions/morph-jobs.md: the pool makers' slot 1 "
              "PoolMaker2::MakeMorphJob sub_820AC7F0 and PoolMaker::MakeSkinJob sub_820AC8A0 "
              "with operator new natively, the constructors sub_820ACC08 and sub_820ACAA8 (no "
              "adapters: only the makers call them; the post to the worker pool a guest call "
              "from their frame), the job's slots sub_820ACB90, sub_820AC9B8, sub_820AC9D0, "
              "sub_820AC9D8, sub_820AC998, sub_820AC9C0 and its base's destructor sub_820AC9E8, "
              "and the 36-byte work item's slots sub_82090DD0, sub_82090DE8, sub_82090DF8, "
              "sub_82090E08, sub_82090E18, sub_82090CB8, sub_820ACA30, sub_82090E40, "
              "sub_82090E38 and sub_82090DC8) natively instead of through the translated code "
              "(the same writes; Model::MorphShape's plain C++ calls the maker's plain C++ "
              "with the flag; checked by --run_harnesses=morph_jobs)");
ITC_FLAG_DECLARE(bool, native_c_runtime_heap);
ITC_FLAG_DECLARE(bool, native_second_set);

namespace hooks {

namespace {

namespace mj = morph_jobs;

// A tail call: r3 (and r4 when `with_r4`) set, r11 the function and r12 the
// vtable where the original leaves them, the function run through the
// function table; lr is the caller's.
void TailCall(PPCContext& ctx, uint8_t* base, const mj::TailCall& call, bool with_r4,
              bool with_r12) {
  ctx.r3.u64 = call.r3;
  if (with_r4) {
    ctx.r4.u64 = call.r4;
  }
  if (with_r12) {
    ctx.r12.u64 = call.vtable;
  }
  ctx.r11.u64 = call.function;
  ctx.ctr.u64 = call.function;
  REX_CALL_INDIRECT_FUNC(call.function);
}

// operator delete on `object` from a 96-byte frame returning to `lr`:
// natively over the native heap, else (or where the layer refuses it) the
// original.
void Delete(PPCContext& ctx, uint8_t* base, uint32_t object, uint32_t lr) {
  if (ITC_FLAG(native_c_runtime_heap) && NativeHeapActive() &&
      memory::crt::OperatorDelete(object)) {
    return;
  }
  const uint64_t stack = ctx.r1.u64, link = ctx.lr;
  ctx.r1.u64 = ctx.r1.u32 - 96;
  ctx.r3.u64 = object;
  ctx.lr = lr;
  __imp__sub_825A70E8(ctx, base);
  ctx.r1.u64 = stack;
  ctx.lr = link;
}

}  // namespace

bool MorphJobsMakersNative() {
  return ITC_FLAG(native_morph_jobs) && ITC_FLAG(native_c_runtime_heap) && NativeHeapActive();
}

mj::ConstructCalls MorphJobsConstructCalls(PPCContext& ctx, uint8_t* base) {
  mj::ConstructCalls calls;
  calls.post = [&ctx, base](uint32_t interface, uint32_t item, uint32_t sp, uint32_t lr) {
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = interface;
    ctx.r4.u64 = item;
    ctx.r5.u64 = 1;
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, interface) + 4);
    ctx.r11.u64 = function;
    ctx.ctr.u64 = function;
    ctx.lr = lr;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.lr = link;
  };
  calls.memcpy = [&ctx, base](uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp,
                              uint32_t lr) {
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = to;
    ctx.r4.u64 = from;
    ctx.r5.u64 = bytes;
    ctx.lr = lr;
    __imp__sub_825A7410(ctx, base);
    ctx.r1.u64 = stack;
    ctx.lr = link;
  };
  return calls;
}

void MorphJobsMake(PPCContext& ctx, uint8_t* base, mj::Kind kind) {
  const auto job = mj::MakeJob(base, kind, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32,
                               MorphJobsConstructCalls(ctx, base));
  if (!job) {
    if (kind == mj::Kind::kMorph) {
      __imp__sub_820AC7F0(ctx, base);
    } else {
      __imp__sub_820AC8A0(ctx, base);
    }
    return;
  }
  ctx.r3.u64 = *job;
}

void MorphJobsConstruct(PPCContext& ctx, uint8_t* base, mj::Kind kind) {
  const uint64_t r3 = ctx.r3.u64;
  mj::ConstructJob(base, kind, uint32_t(r3), ctx.r4.u32, ctx.r5.u32, ctx.r1.u32,
                   MorphJobsConstructCalls(ctx, base));
  ctx.r3.u64 = r3;
}

void MorphJobsDestroy(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  mj::DestroyJob(base, uint32_t(r3), ctx.r1.u32);
  if ((ctx.r4.u32 & 1) != 0) {
    Delete(ctx, base, uint32_t(r3), 0x820ACBF0);
  }
  ctx.r3.u64 = r3;
}

void MorphJobsDestroyBase(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  mj::DestroyJobBase(base, uint32_t(r3), ctx.r1.u32);
  if ((ctx.r4.u32 & 1) != 0) {
    Delete(ctx, base, uint32_t(r3), 0x820ACA14);
  }
  ctx.r3.u64 = r3;
}

void MorphJobsWorkItemDestroy(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  mj::WorkItemDestroy(base, uint32_t(r3), ctx.r1.u32);
  if ((ctx.r4.u32 & 1) != 0) {
    Delete(ctx, base, uint32_t(r3), 0x82090E90);
  }
  ctx.r3.u64 = r3;
}

void MorphJobsWorkItemInterfaceDestroy(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = ctx.r3.s64 - 4;
  MorphJobsWorkItemDestroy(ctx, base);
}

void MorphJobsWait(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = ctx.r3.s64 + mj::kItem;
  sub_82094B10(ctx, base);  // Job::Wait (--find_menu_selection's wrap)
}

void MorphJobsIsReleased(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = mj::IsReleased(base, ctx.r3.u32);
}

void MorphJobsSetReleased(PPCContext& ctx, uint8_t* base) {
  mj::SetReleased(base, ctx.r3.u32);
  ctx.r11.u64 = 1;
}

void MorphJobsRun(PPCContext& ctx, uint8_t* base) {
  const mj::RunTarget run = mj::Run(base, ctx.r3.u32);
  ctx.r11.u64 = run.kind_word;
  ctx.r3.s64 = ctx.r3.s64 + (run.morph ? 56 : 8);  // run.record, all 64 bits
  if (!run.morph) {
    sub_8258BCD8(ctx, base);  // the skinning blend (not seen in play)
  } else if (ITC_FLAG(native_second_set)) {
    SecondSetMorphBlend(ctx, base);
  } else {
    __imp__sub_8258BD28(ctx, base);
  }
}

void MorphJobsRunThunk(PPCContext& ctx, uint8_t* base) {
  TailCall(ctx, base, mj::RunThunk(base, ctx.r3.u64), false, true);
}

void MorphJobsWorkItemPosted(PPCContext& ctx, uint8_t* base) {
  mj::WorkItemPosted(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  ctx.r11.u64 = 1;
}

void MorphJobsWorkItemStarted(PPCContext& ctx, uint8_t* base) {
  mj::WorkItemStarted(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r11.u64 = 2;
}

void MorphJobsWorkItemFinished(PPCContext& ctx, uint8_t* base) {
  mj::WorkItemFinished(base, ctx.r3.u32);
  ctx.r11.u64 = 3;
}

void MorphJobsWorkItemSkip(PPCContext& ctx, uint8_t* base) {
  mj::WorkItemSkip(base, ctx.r3.u32);
  ctx.r11.u64 = 1;
}

void MorphJobsWorkItemIsSkipped(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = mj::WorkItemIsSkipped(base, ctx.r3.u32);
}

void MorphJobsWorkItemInterfaceIsSkipped(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = mj::WorkItemIsSkipped(base, ctx.r3.u32 - 4);
}

void MorphJobsWorkItemRun(PPCContext& ctx, uint8_t* base) {
  TailCall(ctx, base, mj::WorkItemRun(base, ctx.r3.u64), true, false);
}

void MorphJobsWorkItemCall(PPCContext& ctx, uint8_t* base) {
  TailCall(ctx, base, mj::WorkItemCall(base, ctx.r3.u64), false, false);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_MORPH_JOBS(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_morph_jobs)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

// The makers: also with operator new natively.
extern "C" REX_FUNC(sub_820AC7F0) {
  REX_FUNC_PROLOGUE();
  if (!hooks::MorphJobsMakersNative()) {
    __imp__sub_820AC7F0(ctx, base);
    return;
  }
  hooks::MorphJobsMake(ctx, base, morph_jobs::Kind::kMorph);
}

extern "C" REX_FUNC(sub_820AC8A0) {
  REX_FUNC_PROLOGUE();
  if (!hooks::MorphJobsMakersNative()) {
    __imp__sub_820AC8A0(ctx, base);
    return;
  }
  hooks::MorphJobsMake(ctx, base, morph_jobs::Kind::kSkin);
}

ITC_MORPH_JOBS(820ACB90, MorphJobsDestroy)
ITC_MORPH_JOBS(820AC9E8, MorphJobsDestroyBase)
ITC_MORPH_JOBS(820AC9B8, MorphJobsWait)
ITC_MORPH_JOBS(820AC9D0, MorphJobsIsReleased)
ITC_MORPH_JOBS(820AC9D8, MorphJobsSetReleased)
ITC_MORPH_JOBS(820AC998, MorphJobsRun)
ITC_MORPH_JOBS(820AC9C0, MorphJobsRunThunk)
ITC_MORPH_JOBS(82090DD0, MorphJobsWorkItemPosted)
ITC_MORPH_JOBS(82090DE8, MorphJobsWorkItemStarted)
ITC_MORPH_JOBS(82090DF8, MorphJobsWorkItemFinished)
ITC_MORPH_JOBS(82090E08, MorphJobsWorkItemSkip)
ITC_MORPH_JOBS(82090CB8, MorphJobsWorkItemIsSkipped)
ITC_MORPH_JOBS(82090DC8, MorphJobsWorkItemInterfaceIsSkipped)
ITC_MORPH_JOBS(82090E18, MorphJobsWorkItemRun)
ITC_MORPH_JOBS(820ACA30, MorphJobsWorkItemCall)
ITC_MORPH_JOBS(82090E40, MorphJobsWorkItemDestroy)
ITC_MORPH_JOBS(82090E38, MorphJobsWorkItemInterfaceDestroy)
