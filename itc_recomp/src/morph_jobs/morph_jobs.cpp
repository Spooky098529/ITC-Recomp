// The morph jobs as plain C++ (docs/functions/morph-jobs.md); see
// morph_jobs.h.

#include "morph_jobs/morph_jobs.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/app_states.h"
#include "schema/framework.h"
#include "schema/morph_jobs.h"

namespace morph_jobs {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// The work item's fields both constructors write twice: +8 and +12 = 0, the
// bytes +16 = 0, +17 = 1, +18 = 0.
void ItemState(const schema::MorphJob& j) {
  j.set_item_worker_value(0);
  j.set_item_state(0);
  j.set_item_skip(0);
  j.set_item_post_flag(1);
  j.set_item_posted(0);
}

}  // namespace

uint32_t ConstructJob(uint8_t* base, Kind kind, uint32_t job, uint32_t interface,
                      uint32_t record, uint32_t stack, const ConstructCalls& calls) {
  const bool morph = kind == Kind::kMorph;
  if (morph) {
    ITC_NATIVE_RUNNING("sub_820ACC08 MorphJob::MorphJob");
  } else {
    ITC_NATIVE_RUNNING("sub_820ACAA8 SkinJob::SkinJob");
  }
  const uint32_t frame = Frame(base, stack, kConstructFrame);
  const schema::MorphJob j(base, job);
  const uint32_t item = j.item_vtable_address();
  j.set_released(0);
  j.set_vtable(kJobVtable);
  j.set_item_interface(0x82000AB4);
  j.set_item_worker_value(0);
  j.set_item_state(0);
  j.set_item_skip(0);
  j.set_item_post_flag(1);
  j.set_item_posted(0);
  j.set_item_vtable(kWorkItemVtable);
  j.set_item_interface(kWorkItemInterface);
  j.set_delegate_vtable(kDelegateVtable);
  j.set_delegate_object(0u);
  j.set_delegate_target(0);
  const uint32_t to = morph ? j.morph_record_address() : j.skin_influence_counts_address();
  const uint32_t bytes = morph ? 128 : 48;
  const uint32_t memcpy_lr = morph ? 0x820ACC94 : 0x820ACB34;
  if (!native::Buffers_Memcpy(base, to, record, bytes, memcpy_lr)) {
    calls.memcpy(to, record, bytes, frame, memcpy_lr);
  }
  const uint32_t stack_word = Load(base, frame + 100);
  j.set_kind(morph ? 1 : 0);
  ItemState(j);
  j.set_item_waiter(stack_word);
  j.set_delegate_target(kRunThunk);
  j.set_delegate_object(job);
  calls.post(interface, item, frame, morph ? 0x820ACCE0 : 0x820ACB80);
  return job;
}

std::optional<uint32_t> MakeJob(uint8_t* base, Kind kind, uint32_t maker, uint32_t record,
                                uint32_t stack, const ConstructCalls& calls) {
  // operator new(224) at 0x820AC814 (0x820AC8C4 for the skinning job): the
  // job in host memory (E5 step 4), its handle the r3 the original kept.
  // Freed by the deleting destructor's operator delete (the game's frees
  // take handles).
  const uint32_t job = schema::MorphJob::New(base).address;
  if (kind == Kind::kMorph) {
    ITC_NATIVE_RUNNING("sub_820AC7F0 PoolMaker2::MakeMorphJob");
  } else {
    ITC_NATIVE_RUNNING("sub_820AC8A0 PoolMaker::MakeSkinJob");
  }
  const uint32_t frame = Frame(base, stack, kMakeFrame);
  if (job == 0) {
    return 0u;
  }
  return ConstructJob(base, kind, job,
                      schema::MorphJobMaker(base, maker).worker_pool().address, record,
                      frame, calls);
}

// The original's 96-byte frames (here and in DestroyJobBase and
// WorkItemDestroy) hold only their back chains, which nothing reads
// (operator delete, from the adapter, makes its own): not written (E5 step
// 6).
void DestroyJob(uint8_t* base, uint32_t job, [[maybe_unused]] uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ACB90 MorphJob::~MorphJob");
  const schema::MorphJob j(base, job);
  j.set_delegate_vtable(0x82000ABC);
  j.set_item_interface(0x82000BA8);
  j.set_item_interface(0x82000AB4);
  j.set_item_vtable(0x82000EB8);
  j.set_vtable(kJobBaseVtable);
}

void DestroyJobBase(uint8_t* base, uint32_t job, [[maybe_unused]] uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820AC9E8 Job::~Job");
  schema::MorphJob(base, job).set_vtable(kJobBaseVtable);
}

uint32_t IsReleased(uint8_t* base, uint32_t job) {
  ITC_NATIVE_RUNNING("sub_820AC9D0 MorphJob::IsReleased");
  return schema::MorphJob(base, job).released();
}

void SetReleased(uint8_t* base, uint32_t job) {
  ITC_NATIVE_RUNNING("sub_820AC9D8 MorphJob::SetReleased");
  schema::MorphJob(base, job).set_released(1);
}

RunTarget Run(uint8_t* base, uint32_t job) {
  ITC_NATIVE_RUNNING("sub_820AC998 MorphJob::Run");
  const schema::MorphJob j(base, job);
  const uint32_t kind_word = j.kind();
  if (kind_word != 0) {
    return {true, j.morph_record_address(), kind_word};
  }
  return {false, j.skin_influence_counts_address(), kind_word};
}

void WorkItemPosted(uint8_t* base, uint32_t item, uint32_t waiter, uint32_t r5) {
  ITC_NATIVE_RUNNING("sub_82090DD0 WorkItem::Posted");
  const schema::WorkItem i(base, item);
  i.set_waiter(waiter);
  i.set_post_flag(uint8_t(r5));
  i.set_state(1);
  i.set_posted(1);
}

void WorkItemStarted(uint8_t* base, uint32_t item, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82090DE8 WorkItem::Started");
  const schema::WorkItem i(base, item);
  i.set_worker_value(value);
  i.set_state(2);
}

void WorkItemFinished(uint8_t* base, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_82090DF8 WorkItem::Finished");
  schema::WorkItem(base, item).set_state(3);
}

void WorkItemSkip(uint8_t* base, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_82090E08 WorkItem::Skip");
  schema::WorkItem(base, item).set_skip(1);
}

uint32_t WorkItemIsSkipped(uint8_t* base, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_82090CB8 WorkItem::IsSkipped");
  return schema::WorkItem(base, item).skip();
}

void WorkItemDestroy(uint8_t* base, uint32_t item, [[maybe_unused]] uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82090E40 WorkItem::~WorkItem");
  const schema::WorkItem i(base, item);
  i.set_delegate_vtable(0x82000ABC);
  i.set_interface(0x82000BA8);
  i.set_interface(0x82000AB4);
  i.set_vtable(0x82000EB8);
}

TailCall WorkItemRun(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82090E18 WorkItem::Run");
  const uint32_t item = uint32_t(r3);
  const uint64_t r4 = item != 0 ? r3 + 4 : 0;
  const auto vtable = schema::WorkItem(base, item).vtable();
  return {vtable.call(), r3, r4, vtable.address};
}

TailCall WorkItemCall(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820ACA30 WorkItem::Call");
  const schema::WorkItem i(base, uint32_t(r3));
  const uint64_t delegate = r3 + 24;
  // The delegate's vtable (app-states.toml's AppDelegateVtable): slot 1.
  const schema::AppDelegateVtable vtable(base, i.delegate_vtable());
  return {vtable.invoke(), delegate, 0, vtable.address};
}

TailCall RunThunk(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820AC9C0 MorphJob::RunThunk");
  const auto vtable = schema::MorphJob(base, uint32_t(r3)).vtable();
  return {vtable.run(), r3, 0, vtable.address};
}

}  // namespace morph_jobs
