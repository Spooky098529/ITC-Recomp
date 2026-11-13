// The morph jobs as plain C++ (docs/functions/morph-jobs.md); see
// morph_jobs.h.

#include "morph_jobs/morph_jobs.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "native/buffers.h"

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
void ItemState(uint8_t* base, uint32_t item) {
  Store(base, item + 8, 0);
  Store(base, item + 12, 0);
  guest::StoreU8(base, item + 16, 0);
  guest::StoreU8(base, item + 17, 1);
  guest::StoreU8(base, item + 18, 0);
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
  const uint32_t item = job + kItem;
  guest::StoreU8(base, job + 4, 0);
  Store(base, job + 0, kJobVtable);
  Store(base, item + 4, 0x82000AB4);
  Store(base, item + 8, 0);
  Store(base, item + 12, 0);
  guest::StoreU8(base, item + 16, 0);
  guest::StoreU8(base, item + 17, 1);
  guest::StoreU8(base, item + 18, 0);
  Store(base, item + 0, kWorkItemVtable);
  Store(base, item + 4, kWorkItemInterface);
  Store(base, item + 24, kDelegateVtable);
  Store(base, item + 28, 0);
  Store(base, item + 32, 0);
  const uint32_t to = morph ? job + 56 : job + 8;
  const uint32_t bytes = morph ? 128 : 48;
  const uint32_t memcpy_lr = morph ? 0x820ACC94 : 0x820ACB34;
  if (!native::Buffers_Memcpy(base, to, record, bytes, memcpy_lr)) {
    calls.memcpy(to, record, bytes, frame, memcpy_lr);
  }
  const uint32_t stack_word = Load(base, frame + 100);
  Store(base, job + 220, morph ? 1 : 0);
  ItemState(base, item);
  Store(base, item + 20, stack_word);
  Store(base, item + 32, kRunThunk);
  Store(base, item + 28, job);
  calls.post(interface, item, frame, morph ? 0x820ACCE0 : 0x820ACB80);
  return job;
}

std::optional<uint32_t> MakeJob(uint8_t* base, Kind kind, uint32_t maker, uint32_t record,
                                uint32_t stack, const ConstructCalls& calls) {
  const auto job = memory::crt::OperatorNew(base, 224);
  if (!job) {
    return std::nullopt;
  }
  if (kind == Kind::kMorph) {
    ITC_NATIVE_RUNNING("sub_820AC7F0 PoolMaker2::MakeMorphJob");
  } else {
    ITC_NATIVE_RUNNING("sub_820AC8A0 PoolMaker::MakeSkinJob");
  }
  const uint32_t frame = Frame(base, stack, kMakeFrame);
  if (*job == 0) {
    return 0u;
  }
  return ConstructJob(base, kind, *job, Load(base, maker + 4), record, frame, calls);
}

void DestroyJob(uint8_t* base, uint32_t job, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ACB90 MorphJob::~MorphJob");
  Frame(base, stack, 96);
  const uint32_t item = job + kItem;
  Store(base, item + 24, 0x82000ABC);
  Store(base, item + 4, 0x82000BA8);
  Store(base, item + 4, 0x82000AB4);
  Store(base, item + 0, 0x82000EB8);
  Store(base, job + 0, kJobBaseVtable);
}

void DestroyJobBase(uint8_t* base, uint32_t job, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820AC9E8 Job::~Job");
  Frame(base, stack, 96);
  Store(base, job + 0, kJobBaseVtable);
}

uint32_t IsReleased(uint8_t* base, uint32_t job) {
  ITC_NATIVE_RUNNING("sub_820AC9D0 MorphJob::IsReleased");
  return guest::LoadU8(base, job + 4);
}

void SetReleased(uint8_t* base, uint32_t job) {
  ITC_NATIVE_RUNNING("sub_820AC9D8 MorphJob::SetReleased");
  guest::StoreU8(base, job + 4, 1);
}

RunTarget Run(uint8_t* base, uint32_t job) {
  ITC_NATIVE_RUNNING("sub_820AC998 MorphJob::Run");
  const uint32_t kind_word = Load(base, job + 220);
  if (kind_word != 0) {
    return {true, job + 56, kind_word};
  }
  return {false, job + 8, kind_word};
}

void WorkItemPosted(uint8_t* base, uint32_t item, uint32_t waiter, uint32_t r5) {
  ITC_NATIVE_RUNNING("sub_82090DD0 WorkItem::Posted");
  Store(base, item + 20, waiter);
  guest::StoreU8(base, item + 17, uint8_t(r5));
  Store(base, item + 12, 1);
  guest::StoreU8(base, item + 18, 1);
}

void WorkItemStarted(uint8_t* base, uint32_t item, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82090DE8 WorkItem::Started");
  Store(base, item + 8, value);
  Store(base, item + 12, 2);
}

void WorkItemFinished(uint8_t* base, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_82090DF8 WorkItem::Finished");
  Store(base, item + 12, 3);
}

void WorkItemSkip(uint8_t* base, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_82090E08 WorkItem::Skip");
  guest::StoreU8(base, item + 16, 1);
}

uint32_t WorkItemIsSkipped(uint8_t* base, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_82090CB8 WorkItem::IsSkipped");
  return guest::LoadU8(base, item + 16);
}

void WorkItemDestroy(uint8_t* base, uint32_t item, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82090E40 WorkItem::~WorkItem");
  Frame(base, stack, 96);
  Store(base, item + 24, 0x82000ABC);
  Store(base, item + 4, 0x82000BA8);
  Store(base, item + 4, 0x82000AB4);
  Store(base, item + 0, 0x82000EB8);
}

TailCall WorkItemRun(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82090E18 WorkItem::Run");
  const uint32_t item = uint32_t(r3);
  const uint64_t r4 = item != 0 ? r3 + 4 : 0;
  const uint32_t vtable = Load(base, item);
  return {Load(base, vtable + 28), r3, r4, vtable};
}

TailCall WorkItemCall(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820ACA30 WorkItem::Call");
  const uint64_t delegate = r3 + 24;
  const uint32_t vtable = Load(base, uint32_t(delegate));
  return {Load(base, vtable + 4), delegate, 0, vtable};
}

TailCall RunThunk(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820AC9C0 MorphJob::RunThunk");
  const uint32_t vtable = Load(base, uint32_t(r3));
  return {Load(base, vtable + 16), r3, 0, vtable};
}

}  // namespace morph_jobs
