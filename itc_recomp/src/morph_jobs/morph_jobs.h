// The morph jobs as plain C++ (docs/functions/morph-jobs.md): the 224-byte
// job the pool makers' slot 1 builds for Model::MorphShape (and
// Model::SkinShape, never seen in play), the job's vtable slots, and the
// 36-byte work item every worker pool job holds (vtable 0x820066DC). The
// worker pools themselves stay recompiled: the post (the pool interface's
// slot 1) is a guest call from the constructor's frame, as
// DebugText::DebugText's and FramePasses::FramePasses' are. Adapters in
// src/hooks/morph_jobs.cpp, behind --native_morph_jobs.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace morph_jobs {

// The job's vtable (0x8200286C) and its base's (0x8200285C).
constexpr uint32_t kJobVtable = 0x8200286C;
constexpr uint32_t kJobBaseVtable = 0x8200285C;
// The work item's vtable and its +4 interface's.
constexpr uint32_t kWorkItemVtable = 0x820066DC;
constexpr uint32_t kWorkItemInterface = 0x82002908;
// The delegate's vtable (Delegate::Invoke at its slot 1).
constexpr uint32_t kDelegateVtable = 0x82002850;
// The work item's offset in the job.
constexpr uint32_t kItem = 184;

// The pool makers' slot 1 and the job's run thunk (the delegate's target).
constexpr uint32_t kMakeMorphJob = 0x820AC7F0;
constexpr uint32_t kRunThunk = 0x820AC9C0;

// The frames: the makers' and the constructors'.
constexpr uint32_t kMakeFrame = 112;
constexpr uint32_t kConstructFrame = 176;

// Which job: the morph's (sub_820AC7F0 and sub_820ACC08: a 128-byte record
// at +56, +220 = 1) or the skinning's (sub_820AC8A0 and sub_820ACAA8: a
// 48-byte record at +8, +220 = 0).
enum class Kind { kMorph, kSkin };

// The guest calls the constructor makes from its frame.
struct ConstructCalls {
  // The pool interface's slot 1 (the post): r3 = `interface`, r4 = the
  // item, r5 = 1, from the frame at `sp` returning to `lr`.
  std::function<void(uint32_t interface, uint32_t item, uint32_t sp, uint32_t lr)> post;
  // memcpy's original (sub_825A7410) from the frame at `sp` with `lr`,
  // where native::Buffers_Memcpy declines.
  std::function<void(uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr)>
      memcpy;
};

// MorphJob::MorphJob (sub_820ACC08) and SkinJob::SkinJob (sub_820ACAA8): r3 =
// `job`, r4 = `interface` (a worker pool's), r5 = `record`; `stack` is r1 at
// entry. The job's vtable and released byte, the work item at +184 (its
// fields, written twice as the original does, its +20 the word the
// constructor's 176-byte frame holds at +100, never set), the record copied,
// +220 the kind, the delegate's object and target (sub_820AC9C0), then the
// post. Returns the job.
uint32_t ConstructJob(uint8_t* base, Kind kind, uint32_t job, uint32_t interface,
                      uint32_t record, uint32_t stack, const ConstructCalls& calls);

// PoolMaker2::MakeMorphJob (sub_820AC7F0) and PoolMaker::MakeSkinJob
// (sub_820AC8A0): r3 = `maker`, r4 = `record`; `stack` is r1 at entry.
// operator new(224) and the constructor with the maker's +4, from a 112-byte
// frame; 0 when operator new gives 0. nullopt, with nothing done, where
// operator new's plain C++ declines (the original then runs).
std::optional<uint32_t> MakeJob(uint8_t* base, Kind kind, uint32_t maker, uint32_t record,
                                uint32_t stack, const ConstructCalls& calls);

// MorphJob::~MorphJob's (sub_820ACB90) stores: the work item's destructors
// inline (+208 = 0x82000ABC, +188 = 0x82000BA8 then 0x82000AB4, +184 =
// 0x82000EB8), +0 = the base's vtable. The adapter frees the job with bit 0
// of the flags. Back chain of its 96-byte frame below `stack`.
void DestroyJob(uint8_t* base, uint32_t job, uint32_t stack);
// Job::~Job's (sub_820AC9E8) store: +0 = the base's vtable; its 96-byte
// frame's back chain.
void DestroyJobBase(uint8_t* base, uint32_t job, uint32_t stack);

// MorphJob::IsReleased (sub_820AC9D0): the byte +4.
uint32_t IsReleased(uint8_t* base, uint32_t job);
// MorphJob::SetReleased (sub_820AC9D8): the byte +4 = 1.
void SetReleased(uint8_t* base, uint32_t job);

// MorphJob::Run (sub_820AC998): what it tail-calls. Morph::Blend
// (sub_8258BD28) on job + 56 when +220 isn't 0, else the skinning blend
// (sub_8258BCD8) on job + 8; `kind_word` the +220 word (r11).
struct RunTarget {
  bool morph;
  uint32_t record;
  uint32_t kind_word;
};
RunTarget Run(uint8_t* base, uint32_t job);

// --- The work item (vtable 0x820066DC) ---

// WorkItem::Posted (sub_82090DD0): +20 = `waiter`, the byte +17 = `r5`'s low
// byte, +12 = 1, the byte +18 = 1.
void WorkItemPosted(uint8_t* base, uint32_t item, uint32_t waiter, uint32_t r5);
// WorkItem::Started (sub_82090DE8): +8 = `value`, +12 = 2.
void WorkItemStarted(uint8_t* base, uint32_t item, uint32_t value);
// WorkItem::Finished (sub_82090DF8): +12 = 3.
void WorkItemFinished(uint8_t* base, uint32_t item);
// WorkItem::Skip (sub_82090E08): the byte +16 = 1.
void WorkItemSkip(uint8_t* base, uint32_t item);
// WorkItem::IsSkipped (sub_82090CB8): the byte +16.
uint32_t WorkItemIsSkipped(uint8_t* base, uint32_t item);
// WorkItem::~WorkItem's (sub_82090E40) stores: +24 = 0x82000ABC, +4 =
// 0x82000BA8 then 0x82000AB4, +0 = 0x82000EB8; its 96-byte frame's back
// chain. The adapter frees it with bit 0 of the flags.
void WorkItemDestroy(uint8_t* base, uint32_t item, uint32_t stack);

// A tail call through a vtable: the function, and r4 for WorkItem::Run.
struct TailCall {
  uint32_t function;
  uint64_t r3;  // all 64 bits, as the original leaves them
  uint64_t r4;
  uint32_t vtable;  // r12 where the original leaves the vtable there
};
// WorkItem::Run (sub_82090E18): r4 = r3 + 4 (all 64 bits; 0 for a null
// item), then the item's slot 7.
TailCall WorkItemRun(uint8_t* base, uint64_t r3);
// WorkItem::Call (sub_820ACA30): r3 + 24 (all 64 bits; the delegate), its
// slot 1.
TailCall WorkItemCall(uint8_t* base, uint64_t r3);
// MorphJob::RunThunk (sub_820AC9C0): the job's slot 4 (r12 its vtable), r3
// as it came.
TailCall RunThunk(uint8_t* base, uint64_t r3);

}  // namespace morph_jobs
