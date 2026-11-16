// The morph jobs' register steps (src/morph_jobs/,
// docs/functions/morph-jobs.md), shared by the adapters
// (src/hooks/morph_jobs.cpp, behind --native_morph_jobs) and the harness
// (without the flag).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "morph_jobs/morph_jobs.h"

namespace hooks {

// Whether the job makers' plain C++ can run on this thread:
// --native_morph_jobs, with operator new natively (--native_c_runtime_heap
// over the native heap).
bool MorphJobsMakersNative();

// The constructor's guest calls through `ctx` (the post through the pool
// interface's vtable, memcpy's original), r1 and lr put back after.
morph_jobs::ConstructCalls MorphJobsConstructCalls(lifted::Regs& ctx, uint8_t* base);

// The makers' (sub_820AC7F0, sub_820AC8A0) and the constructors'
// (sub_820ACC08, sub_820ACAA8) steps; the makers run the original where
// their plain C++ declines (operator new).
void MorphJobsMake(lifted::Regs& ctx, uint8_t* base, morph_jobs::Kind kind);
void MorphJobsConstruct(lifted::Regs& ctx, uint8_t* base, morph_jobs::Kind kind);

// The deleting destructors (sub_820ACB90, sub_820AC9E8, sub_82090E40 and
// the +4 interface's sub_82090E38): the stores, then operator delete with
// bit 0 of r4 (natively over the native heap, else the original); r3 the
// object.
void MorphJobsDestroy(lifted::Regs& ctx, uint8_t* base);
void MorphJobsDestroyBase(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemDestroy(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemInterfaceDestroy(lifted::Regs& ctx, uint8_t* base);

// The job's slots: the wait (Job::Wait on the item, through the function
// table), the released byte, its set, the run (Morph::Blend's step with
// --native_second_set, else its original; the skinning blend's original),
// and the run thunk.
void MorphJobsWait(lifted::Regs& ctx, uint8_t* base);
void MorphJobsIsReleased(lifted::Regs& ctx, uint8_t* base);
void MorphJobsSetReleased(lifted::Regs& ctx, uint8_t* base);
void MorphJobsRun(lifted::Regs& ctx, uint8_t* base);
void MorphJobsRunThunk(lifted::Regs& ctx, uint8_t* base);

// The work item's slots and the +4 interface's slot 1.
void MorphJobsWorkItemPosted(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemStarted(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemFinished(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemSkip(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemIsSkipped(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemInterfaceIsSkipped(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemRun(lifted::Regs& ctx, uint8_t* base);
void MorphJobsWorkItemCall(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
