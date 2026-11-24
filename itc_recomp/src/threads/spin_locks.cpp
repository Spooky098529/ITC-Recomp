// The kernel's spin locks and IRQL, for the game (E1, step 2:
// docs/rexglue-removal.md, "E1: the plan").
//
// A spin lock is a word in the game's memory holding its owner (the
// owner's PCR address, r13, big-endian; 0 when free), as the SDK keeps it:
// taken with a host compare-and-swap on that word, so the game's view of
// it doesn't change. IRQL is plugged in: no game code reads the PCR's IRQL
// byte (+0x18; only +256 and +268 are read), and what KeRaiseIrqlToDpcLevel
// answers only goes back to KfLowerIrql, so raising answers 0 and lowering
// does nothing.

#include "imports/native_imports.h"
#include "threads/threads.h"

#include "base/log.h"

#include <atomic>
#include <bit>
#include <iterator>
#include <thread>

namespace threads {

namespace {

std::atomic_ref<uint32_t> LockWord(uint8_t* base, uint64_t address) {
  return std::atomic_ref<uint32_t>(*reinterpret_cast<uint32_t*>(guest::Pointer(base, uint32_t(address))));
}

uint32_t Owner(const lifted::Args& args) {
  return std::byteswap(uint32_t(args.r13));
}

void Acquire(uint8_t* base, const lifted::Args& args) {
  auto word = LockWord(base, args.r[0]);
  const uint32_t self = Owner(args);
  for (;;) {
    uint32_t expected = 0;
    if (word.compare_exchange_weak(expected, self, std::memory_order_acquire)) {
      return;
    }
    std::this_thread::yield();
  }
}

void Release(uint8_t* base, const lifted::Args& args) {
  LockWord(base, args.r[0]).store(0, std::memory_order_release);
}

// KeRaiseIrqlToDpcLevel(): the old IRQL, plugged in.
lifted::Result RaiseIrqlToDpcLevel(uint8_t*, const lifted::Args&) {
  ITC_NATIVE_RUNNING("import KeRaiseIrqlToDpcLevel");
  return {0, 0.0};
}

// KfLowerIrql(old): nothing to do.
lifted::Result LowerIrql(uint8_t*, const lifted::Args&) {
  ITC_NATIVE_RUNNING("import KfLowerIrql");
  return {0, 0.0};
}

// KeAcquireSpinLockAtRaisedIrql(lock).
lifted::Result AcquireSpinLockAtRaisedIrql(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeAcquireSpinLockAtRaisedIrql");
  Acquire(base, args);
  return {0, 0.0};
}

// KeReleaseSpinLockFromRaisedIrql(lock).
lifted::Result ReleaseSpinLockFromRaisedIrql(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeReleaseSpinLockFromRaisedIrql");
  Release(base, args);
  return {0, 0.0};
}

// KeTryToAcquireSpinLockAtRaisedIrql(lock): 1 when taken, else 0.
lifted::Result TryToAcquireSpinLockAtRaisedIrql(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeTryToAcquireSpinLockAtRaisedIrql");
  uint32_t expected = 0;
  const bool taken = LockWord(base, args.r[0])
                         .compare_exchange_strong(expected, Owner(args), std::memory_order_acquire);
  return {taken ? 1u : 0u, 0.0};
}

// KfAcquireSpinLock(lock): the old IRQL (plugged in, 0).
lifted::Result AcquireSpinLock(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KfAcquireSpinLock");
  Acquire(base, args);
  return {0, 0.0};
}

// KfReleaseSpinLock(lock, old IRQL).
lifted::Result ReleaseSpinLock(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KfReleaseSpinLock");
  Release(base, args);
  return {0, 0.0};
}

const imports::NativeImportEntry kEntries[] = {
    {__imp__KeRaiseIrqlToDpcLevel, RaiseIrqlToDpcLevel},
    {__imp__KfLowerIrql, LowerIrql},
    {__imp__KeAcquireSpinLockAtRaisedIrql, AcquireSpinLockAtRaisedIrql},
    {__imp__KeReleaseSpinLockFromRaisedIrql, ReleaseSpinLockFromRaisedIrql},
    {__imp__KeTryToAcquireSpinLockAtRaisedIrql, TryToAcquireSpinLockAtRaisedIrql},
    {__imp__KfAcquireSpinLock, AcquireSpinLock},
    {__imp__KfReleaseSpinLock, ReleaseSpinLock},
};

const bool kRegistered = imports::RegisterNativeImports(kEntries, std::size(kEntries), Enabled);

}  // namespace

}  // namespace threads
