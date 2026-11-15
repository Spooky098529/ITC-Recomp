// The lift sweep's runtime: see lifted.h and sdk.h.

#include "lifted/lifted.h"
#include "lifted/sdk.h"

#include "base/log.h"

#include <cstdlib>
#include <cstring>
#include <unordered_map>

namespace lifted {

namespace {

struct Target {
  Entry entry;
  const bool* enabled;
};

std::unordered_map<uint32_t, Target>& Table() {
  static std::unordered_map<uint32_t, Target> table;
  return table;
}

// The SDK context of the innermost Enter on this thread: the one calls
// through the boundary run on.
thread_local PPCContext* current = nullptr;

PPCContext& Current() {
  if (current == nullptr) {
    ITC_LOG_ERROR("lifted: a call through the SDK boundary outside any Enter");
    std::abort();
  }
  return *current;
}

// The host's float mode into the context's copy, so the SDK's conditional
// mode switches see what the host has.
void SyncFloatMode(PPCContext& ctx) { ctx.fpscr.csr = ctx.fpscr.getcsr(); }

// Runs `invoke` on the thread's context with the call's arguments; what it
// leaves in r3 and f1. The stack pointer and r13 are put back after.
template <typename Invoke>
Result Boundary(const Args& args, Invoke invoke) {
  PPCContext& ctx = Current();
  const uint64_t r1 = ctx.r1.u64;
  const uint64_t r13 = ctx.r13.u64;
  const uint64_t lr = ctx.lr;
  // The argument registers the call overwrites, put back after it (but r3
  // and f1, its results): a recompiled caller further up may keep a value
  // in one across a call the 360's compiler knew doesn't touch it.
  uint64_t saved_r[7];
  for (int i = 0; i < 7; i++) {
    saved_r[i] = (&ctx.r4)[i].u64;
  }
  PPCRegister saved_f[12];
  std::memcpy(saved_f, &ctx.f2, sizeof(saved_f));
  PPCVRegister saved_v[13];
  std::memcpy(saved_v, &ctx.v1, sizeof(saved_v));
  ctx.r1.u64 = args.r1;
  ctx.r13.u64 = args.r13;
  ctx.lr = args.lr;
  ctx.r3.u64 = args.r[0];
  ctx.r4.u64 = args.r[1];
  ctx.r5.u64 = args.r[2];
  ctx.r6.u64 = args.r[3];
  ctx.r7.u64 = args.r[4];
  ctx.r8.u64 = args.r[5];
  ctx.r9.u64 = args.r[6];
  ctx.r10.u64 = args.r[7];
  PPCRegister* f = &ctx.f1;
  for (int i = 0; i < 13; i++) {
    f[i].f64 = args.f[i];
  }
  PPCVRegister* v = &ctx.v1;
  for (int i = 0; i < 13; i++) {
    std::memcpy(&v[i], &args.v[i], sizeof(Vec));
  }
  SyncFloatMode(ctx);
  invoke(ctx);
  SyncFloatMode(ctx);
  Result result{ctx.r3.u64, ctx.f1.f64};
  ctx.r1.u64 = r1;
  ctx.r13.u64 = r13;
  ctx.lr = lr;
  for (int i = 0; i < 7; i++) {
    (&ctx.r4)[i].u64 = saved_r[i];
  }
  std::memcpy(&ctx.f2, saved_f, sizeof(saved_f));
  std::memcpy(&ctx.v1, saved_v, sizeof(saved_v));
  return result;
}

}  // namespace

void ScalarFloatMode() {
  const uint32_t csr = PPCFPSCRRegister::Platform::getcsr();
  if ((csr & PPCFPSCRRegister::FlushMask) != 0) [[unlikely]] {
    PPCFPSCRRegister::Platform::setcsr(csr & ~uint32_t(PPCFPSCRRegister::FlushMask));
  }
}

void VectorFloatMode() {
  const uint32_t csr = PPCFPSCRRegister::Platform::getcsr();
  if ((csr & PPCFPSCRRegister::FlushMask) != PPCFPSCRRegister::FlushMask) [[unlikely]] {
    PPCFPSCRRegister::Platform::setcsr(csr | uint32_t(PPCFPSCRRegister::FlushMask));
  }
}

void EnterGlobalLock() { REX_ENTER_GLOBAL_LOCK(); }

void LeaveGlobalLock() { REX_LEAVE_GLOBAL_LOCK(); }

uint64_t CheckGlobalLock() { return REX_CHECK_GLOBAL_LOCK(); }

void Trap(uint32_t type) {
  // 20 and 26 are debug prints, 25 nothing (the SDK's handler).
  if (type != 20 && type != 26 && type != 25) {
    ITC_LOG_WARN("lifted: trap hit (type {})", type);
  }
}

void Register(const Registration* entries, size_t count, const bool* enabled) {
  auto& table = Table();
  for (size_t i = 0; i < count; i++) {
    table[entries[i].address] = Target{entries[i].entry, enabled};
  }
}

Entry Find(uint32_t address) {
  const auto& table = Table();
  if (const auto it = table.find(address); it != table.end() && *it->second.enabled) {
    return it->second.entry;
  }
  return nullptr;
}

Result Call(uint8_t* base, uint32_t address, const Args& args) {
  const auto& table = Table();
  if (const auto it = table.find(address); it != table.end() && *it->second.enabled) {
    return it->second.entry(base, args);
  }
  return Boundary(args, [&](PPCContext& ctx) {
    ctx.ctr.u64 = address;
    REX_CALL_INDIRECT_FUNC(address);
  });
}

Result Import(uint8_t* base, ImportFunction* function, const Args& args) {
  return Boundary(args, [&](PPCContext& ctx) { function(ctx, base); });
}

void Enter(PPCContext& ctx, uint8_t* base, Entry entry) {
  PPCContext* const outer = current;
  current = &ctx;
  Args args;
  args.r1 = ctx.r1.u64;
  args.r13 = ctx.r13.u64;
  args.lr = uint32_t(ctx.lr);
  args.r[0] = ctx.r3.u64;
  args.r[1] = ctx.r4.u64;
  args.r[2] = ctx.r5.u64;
  args.r[3] = ctx.r6.u64;
  args.r[4] = ctx.r7.u64;
  args.r[5] = ctx.r8.u64;
  args.r[6] = ctx.r9.u64;
  args.r[7] = ctx.r10.u64;
  const PPCRegister* f = &ctx.f1;
  for (int i = 0; i < 13; i++) {
    args.f[i] = f[i].f64;
  }
  const PPCVRegister* v = &ctx.v1;
  for (int i = 0; i < 13; i++) {
    std::memcpy(&args.v[i], &v[i], sizeof(Vec));
  }
  const Result result = entry(base, args);
  ctx.r3.u64 = result.r3;
  ctx.f1.f64 = result.f1;
  SyncFloatMode(ctx);
  current = outer;
}

void CallFromContext(PPCContext& ctx, uint8_t* base, uint32_t address) {
  ctx.ctr.u64 = address;
  if (const Entry entry = Find(address)) {
    Enter(ctx, base, entry);
    return;
  }
  REX_CALL_INDIRECT_FUNC(address);
}

}  // namespace lifted
