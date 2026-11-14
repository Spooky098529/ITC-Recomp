// The C runtime's core (src/c_runtime/core.cpp,
// docs/functions/c-runtime-core.md) against the recompiled functions.
//
// These call kernel imports with side effects (TLS slots, critical
// sections) and work on globals every thread shares, so each case gives both
// sides the same fresh state and puts the game's back right after:
//
// - __crtTlsAlloc: a slot allocated, compared, then freed, so the other side
//   gets the same index.
// - __set_flsgetvalue and _getptd_noexit on the harness's thread (the main
//   thread): its getter slot and data slot set per case (0, the game's, or
//   a random word for the getter slot), the last error set to a random word;
//   compared: r3, both slots and the last error after. A thread with no data
//   (its data slot 0): each side's new block is compared by contents, then
//   freed and the game's put back. The store failing: [0x82971164] pointed
//   at the shared "returns 0" (sub_825C8388) for the case.
// - _freefls and _mtdeletelocks: blocks from the C runtime's heap (with
//   --native_heap and --native_c_runtime_heap) are compared by whether the
//   layer still holds them after the call. _mtdeletelocks runs on the lock
//   table with only the entries not made at boot (all 0 in play) given test
//   blocks for the case, then put back.
// - _mtterm: the two TLS indexes 0, -1 or fresh slots for the case (the
//   game's put back right after); compared: both indexes after, whether each
//   fresh slot was freed (allocated again with the same index), and the lock
//   table's test entries as _mtdeletelocks'.
//
// Nothing here is a float: all exact.

#include "harness.h"

#include "base/log.h"
#include "c_runtime/core.h"
#include "hooks/c_runtime_core.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <string>
#include <vector>

namespace harness {

namespace {

namespace cc = c_runtime::core;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kReturnsZero = 0x825C8388;

uint32_t TlsGet(Context& h, uint32_t index) {
  PPCContext t = CallContext(h);
  t.r3.u64 = index;
  __imp__KeTlsGetValue(t, h.base);
  return t.r3.u32;
}
void TlsSet(Context& h, uint32_t index, uint32_t value) {
  PPCContext t = CallContext(h);
  t.r3.u64 = index;
  t.r4.u64 = value;
  __imp__KeTlsSetValue(t, h.base);
}
uint32_t TlsAlloc(Context& h) {
  PPCContext t = CallContext(h);
  __imp__KeTlsAlloc(t, h.base);
  return t.r3.u32;
}
void TlsFree(Context& h, uint32_t index) {
  PPCContext t = CallContext(h);
  t.r3.u64 = index;
  __imp__KeTlsFree(t, h.base);
}
uint32_t GuestLastError(Context& h) {
  PPCContext t = CallContext(h);
  __imp__sub_82295F18(t, h.base);
  return t.r3.u32;
}
void SetLastError(Context& h, uint32_t error) {
  PPCContext t = CallContext(h);
  t.r3.u64 = error;
  __imp__sub_82296FD0(t, h.base);
}

bool HeapOnLayer() { return hooks::NativeHeapActive(); }
uint32_t Block(uint8_t* base, uint32_t bytes) {
  return memory::crt::Malloc(base, bytes).value_or(0);
}

void TlsAllocCases(Context& h) {
  Results results("__crtTlsAlloc (sub_825AA3F8)");
  for (int c = 0; c < 8; c++) {
    const uint32_t callback = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      PPCContext t = CallContext(h);
      t.r3.u64 = callback;
      (v == 0 ? __imp__sub_825AA3F8 : hooks::CoreCrtTlsAlloc)(t, h.base);
      out[v].Word(t.r3.u32);
      // The new slot reads 0 on this thread.
      out[v].Word(TlsGet(h, t.r3.u32));
      if (t.r3.u32 != 0xFFFFFFFFu) {
        TlsFree(h, t.r3.u32);
      }
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The getter slot's and data slot's values for a case, put back after.
struct ThreadSlots {
  Context& h;
  uint32_t getter_index, data_index, getter, data, error;
  explicit ThreadSlots(Context& h)
      : h(h),
        getter_index(Load(h.base, cc::kGetValueIndex)),
        data_index(Load(h.base, cc::kFlsIndex)),
        getter(TlsGet(h, getter_index)),
        data(TlsGet(h, data_index)),
        error(GuestLastError(h)) {}
  ~ThreadSlots() {
    TlsSet(h, getter_index, getter);
    TlsSet(h, data_index, data);
    SetLastError(h, error);
  }
};

void SetFlsGetValueCases(Context& h) {
  Results results("__set_flsgetvalue (sub_825AA400)");
  ThreadSlots slots(h);
  for (int c = 0; c < 24; c++) {
    const uint32_t value = c % 3 == 0 ? 0 : c % 3 == 1 ? slots.getter : uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      TlsSet(h, slots.getter_index, value);
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825AA400 : hooks::CoreSetFlsGetValue)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Word(TlsGet(h, slots.getter_index));
    }
    TlsSet(h, slots.getter_index, slots.getter);
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void GetptdNoexitCases(Context& h) {
  Results results("_getptd_noexit (sub_825AA4C0)");
  ThreadSlots slots(h);
  if (slots.data == 0) {
    ITC_LOG_ERROR("harness: c_runtime_core: _getptd_noexit: no thread data");
    return;
  }
  const uint32_t set_value = Load(h.base, cc::kFlsSetValue);
  for (int c = 0; c < 40; c++) {
    // 0: the data there; 1: none (made); 2: none, the store failing.
    const int kind = c % 4 == 3 ? 2 : c % 4 == 2 ? 1 : 0;
    if (kind != 0 && !HeapOnLayer()) {
      continue;
    }
    const bool getter_empty = h.random.Below(2) != 0;
    const uint32_t error = c % 5 == 0 ? 0 : uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      TlsSet(h, slots.getter_index, getter_empty ? 0 : slots.getter);
      TlsSet(h, slots.data_index, kind == 0 ? slots.data : 0);
      SetLastError(h, error);
      if (kind == 2) {
        Store(h.base, cc::kFlsSetValue, kReturnsZero);
      }
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825AA4C0 : hooks::CoreGetptdNoexit)(t, h.base);
      Store(h.base, cc::kFlsSetValue, set_value);
      const uint32_t data = t.r3.u32;
      const uint32_t slot = TlsGet(h, slots.data_index);
      out[v].Word(GuestLastError(h));
      out[v].Word(TlsGet(h, slots.getter_index));
      if (kind == 0) {
        out[v].Word(data == slots.data);
        out[v].Word(slot == slots.data);
      } else if (kind == 1) {
        if (data == 0) {
          ITC_LOG_ERROR("harness: c_runtime_core: _getptd_noexit: case {} made no data", c);
        }
        out[v].Word(data != 0 && data != slots.data);
        out[v].Word(slot == data);
        if (data != 0) {
          out[v].Bytes(Read(h.base, data, cc::kThreadDataBytes));
          memory::crt::Free(data);
        }
      } else {
        out[v].Word(data);
        out[v].Word(slot);
      }
      TlsSet(h, slots.data_index, slots.data);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void FreeFlsCases(Context& h) {
  Results results("_freefls (sub_825AA5C0)");
  if (!HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _freefls: the heap isn't on the layer");
    return;
  }
  for (int c = 0; c < 40; c++) {
    const bool none = c == 0;
    const uint32_t mask = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      std::vector<uint32_t> blocks;
      uint32_t data = 0;
      if (!none) {
        data = Block(h.base, cc::kThreadDataBytes);
        blocks.push_back(data);
        for (uint32_t i = 0; i < cc::kThreadDataBytes; i += 4) {
          Store(h.base, data + i, uint32_t(i * 0x01010101u));
        }
        int bit = 0;
        for (uint32_t offset : {36u, 44u, 52u, 60u, 68u, 72u}) {
          uint32_t buffer = 0;
          if (mask >> bit++ & 1) {
            buffer = Block(h.base, 16 + 16 * bit);
            blocks.push_back(buffer);
          }
          Store(h.base, data + offset, buffer);
        }
        uint32_t table = 0x8266BE60;
        if (mask >> bit & 1) {
          table = Block(h.base, 120);
          blocks.push_back(table);
        }
        Store(h.base, data + 92, table);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = data;
      (v == 0 ? __imp__sub_825AA5C0 : hooks::CoreFreeFls)(t, h.base);
      for (uint32_t block : blocks) {
        const bool held = memory::IsHeapBlock(block);
        out[v].Word(held);
        if (held) {
          memory::crt::Free(block);
        }
      }
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The lock table with the entries not made at boot given test blocks (or 0);
// `blocks` gets them. Returns the table as it was.
std::vector<uint8_t> TestLocks(Context& h, uint32_t mask, std::vector<uint32_t>& blocks) {
  std::vector<uint8_t> saved = Read(h.base, cc::kLockTable, 8 * cc::kLockCount);
  for (uint32_t i = 0; i < cc::kLockCount; i++) {
    const uint32_t entry = cc::kLockTable + 8 * i;
    if (Load(h.base, entry + 4) == 1) {
      continue;
    }
    uint32_t section = 0;
    if (mask >> (i % 32) & 1) {
      section = Block(h.base, 28);
      blocks.push_back(section);
    }
    Store(h.base, entry + 0, section);
  }
  return saved;
}

void MtDeleteLocksCases(Context& h) {
  Results results("_mtdeletelocks (sub_825B3260)");
  if (!HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _mtdeletelocks: the heap isn't on the layer");
    return;
  }
  for (uint32_t i = 0; i < cc::kLockCount; i++) {
    const uint32_t entry = cc::kLockTable + 8 * i;
    if (Load(h.base, entry + 4) != 1 && Load(h.base, entry) != 0) {
      ITC_LOG_ERROR("harness: c_runtime_core: lock {} made in play; cases skipped", i);
      return;
    }
  }
  for (int c = 0; c < 16; c++) {
    const uint32_t mask = c == 0 ? 0 : uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      std::vector<uint32_t> blocks;
      const std::vector<uint8_t> saved = TestLocks(h, mask, blocks);
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825B3260 : hooks::CoreMtDeleteLocks)(t, h.base);
      std::vector<uint8_t> after = Read(h.base, cc::kLockTable, 8 * cc::kLockCount);
      Write(h.base, cc::kLockTable, saved.data(), uint32_t(saved.size()));
      out[v].Bytes(after);
      for (uint32_t block : blocks) {
        const bool held = memory::IsHeapBlock(block);
        out[v].Word(held);
        if (held) {
          memory::crt::Free(block);
        }
      }
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void MtTermCases(Context& h) {
  Results results("_mtterm (sub_825AA448)");
  if (!HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _mtterm: the heap isn't on the layer");
    return;
  }
  for (uint32_t i = 0; i < cc::kLockCount; i++) {
    const uint32_t entry = cc::kLockTable + 8 * i;
    if (Load(h.base, entry + 4) != 1 && Load(h.base, entry) != 0) {
      ITC_LOG_ERROR("harness: c_runtime_core: lock {} made in play; cases skipped", i);
      return;
    }
  }
  const uint32_t fls_index = Load(h.base, cc::kFlsIndex);
  const uint32_t getter_index = Load(h.base, cc::kGetValueIndex);
  for (int c = 0; c < 12; c++) {
    // Each index: -1 (left alone) or a fresh slot.
    const bool fresh_fls = c & 1, fresh_getter = c & 2;
    const uint32_t mask = c < 4 ? 0 : uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      std::vector<uint32_t> blocks;
      const std::vector<uint8_t> saved = TestLocks(h, mask, blocks);
      const uint32_t a = fresh_fls ? TlsAlloc(h) : 0xFFFFFFFFu;
      const uint32_t b = fresh_getter ? TlsAlloc(h) : 0xFFFFFFFFu;
      Store(h.base, cc::kFlsIndex, a);
      Store(h.base, cc::kGetValueIndex, b);
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825AA448 : hooks::CoreMtTerm)(t, h.base);
      const uint32_t fls_after = Load(h.base, cc::kFlsIndex);
      const uint32_t getter_after = Load(h.base, cc::kGetValueIndex);
      Store(h.base, cc::kFlsIndex, fls_index);
      Store(h.base, cc::kGetValueIndex, getter_index);
      std::vector<uint8_t> after = Read(h.base, cc::kLockTable, 8 * cc::kLockCount);
      Write(h.base, cc::kLockTable, saved.data(), uint32_t(saved.size()));
      out[v].Word(fls_after);
      out[v].Word(getter_after);
      out[v].Bytes(after);
      for (uint32_t block : blocks) {
        const bool held = memory::IsHeapBlock(block);
        out[v].Word(held);
        if (held) {
          memory::crt::Free(block);
        }
      }
      // Freed slots come back first (the kernel's lowest free index).
      std::vector<uint32_t> again;
      for (uint32_t slot : {a, b}) {
        if (slot != 0xFFFFFFFFu) {
          again.push_back(TlsAlloc(h));
        }
      }
      size_t k = 0;
      for (uint32_t slot : {a, b}) {
        if (slot != 0xFFFFFFFFu) {
          out[v].Word(again[k] == a || again[k] == b);
          k++;
        }
      }
      for (uint32_t slot : again) {
        TlsFree(h, slot);
      }
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The thread data's cases' kinds: 0 the data there, 2 none with the store
// failing (the fallback words).
template <typename Body>
void WithThreadData(Context& h, ThreadSlots& slots, int kind, Body body) {
  const uint32_t set_value = Load(h.base, cc::kFlsSetValue);
  TlsSet(h, slots.data_index, kind == 0 ? slots.data : 0);
  if (kind == 2) {
    Store(h.base, cc::kFlsSetValue, kReturnsZero);
  }
  body();
  Store(h.base, cc::kFlsSetValue, set_value);
  TlsSet(h, slots.data_index, slots.data);
}

void ErrnoCases(Context& h) {
  ThreadSlots slots(h);
  if (slots.data == 0 || !HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _errno: no thread data or heap");
    return;
  }
  struct Fn {
    const char* name;
    GuestFunction recompiled, native;
  };
  for (const Fn& fn : {Fn{"_errno (sub_825A81E0)", __imp__sub_825A81E0, hooks::CoreErrno},
                       Fn{"__doserrno (sub_825A8218)", __imp__sub_825A8218, hooks::CoreDosErrno}}) {
    Results results(fn.name);
    for (int c = 0; c < 16; c++) {
      const int kind = c % 2 == 0 ? 0 : 2;
      const uint32_t error = uint32_t(h.random.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        SetLastError(h, error);
        WithThreadData(h, slots, kind, [&] {
          PPCContext t = CallContext(h);
          (v == 0 ? fn.recompiled : fn.native)(t, h.base);
          out[v].Word(kind == 0 ? t.r3.u32 - slots.data : t.r3.u32);
          out[v].Word(GuestLastError(h));
        });
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
}

// Windows errors: the table's, its ranges' edges and random words.
std::vector<uint32_t> OsErrors(Context& h) {
  std::vector<uint32_t> errors = {0, 1, 18, 19, 36, 37, 187, 188, 202, 203, 0xFFFFFFFFu};
  for (uint32_t i = 0; i < 45; i++) {
    errors.push_back(Load(h.base, cc::kOsErrorTable + 8 * i));
  }
  for (int i = 0; i < 60; i++) {
    errors.push_back(h.random.Below(2) ? h.random.Below(260) : uint32_t(h.random.Next()));
  }
  return errors;
}

void OsErrCases(Context& h) {
  Results results("_get_errno_from_oserr (sub_825A8178)");
  for (uint32_t error : OsErrors(h)) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      PPCContext t = CallContext(h);
      t.r3.u64 = error;
      (v == 0 ? __imp__sub_825A8178 : hooks::CoreGetErrnoFromOsErr)(t, h.base);
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], "error " + std::to_string(error));
  }
  results.Report();
}

void DosMapErrCases(Context& h) {
  Results results("_dosmaperr (sub_825A8250)");
  ThreadSlots slots(h);
  if (slots.data == 0) {
    ITC_LOG_ERROR("harness: c_runtime_core: _dosmaperr: no thread data");
    return;
  }
  const uint32_t errno_word = Load(h.base, slots.data + 8);
  const uint32_t doserrno_word = Load(h.base, slots.data + 12);
  const uint32_t errno_fallback = Load(h.base, cc::kErrnoFallback);
  const uint32_t doserrno_fallback = Load(h.base, cc::kDosErrnoFallback);
  int c = 0;
  for (uint32_t error : OsErrors(h)) {
    const int kind = c % 3 == 2 ? 2 : 0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t a : {slots.data + 8, slots.data + 12, cc::kErrnoFallback,
                         cc::kDosErrnoFallback}) {
        Store(h.base, a, 0xCDCDCDCDu);
      }
      WithThreadData(h, slots, kind, [&] {
        PPCContext t = CallContext(h);
        t.r3.u64 = error;
        (v == 0 ? __imp__sub_825A8250 : hooks::CoreDosMapErr)(t, h.base);
      });
      for (uint32_t a : {slots.data + 8, slots.data + 12, cc::kErrnoFallback,
                         cc::kDosErrnoFallback}) {
        out[v].Word(Load(h.base, a));
      }
    }
    results.Check(out[0], out[1], "case " + std::to_string(c++));
  }
  Store(h.base, slots.data + 8, errno_word);
  Store(h.base, slots.data + 12, doserrno_word);
  Store(h.base, cc::kErrnoFallback, errno_fallback);
  Store(h.base, cc::kDosErrnoFallback, doserrno_fallback);
  results.Report();
}

// A thread data block with some buffers from the layer; `blocks` gets them.
uint32_t TestThreadData(Context& h, uint32_t mask, std::vector<uint32_t>& blocks) {
  const uint32_t data = Block(h.base, cc::kThreadDataBytes);
  blocks.push_back(data);
  for (uint32_t i = 0; i < cc::kThreadDataBytes; i += 4) {
    Store(h.base, data + i, 0);
  }
  int bit = 0;
  for (uint32_t offset : {36u, 44u, 52u, 60u, 68u, 72u}) {
    if (mask >> bit++ & 1) {
      const uint32_t buffer = Block(h.base, 32);
      blocks.push_back(buffer);
      Store(h.base, data + offset, buffer);
    }
  }
  Store(h.base, data + 92, cc::kXcptActTab);
  return data;
}

void HeldOutputs(Outputs& out, const std::vector<uint32_t>& blocks) {
  for (uint32_t block : blocks) {
    const bool held = memory::IsHeapBlock(block);
    out.Word(held);
    if (held) {
      memory::crt::Free(block);
    }
  }
}

void FreePtdCases(Context& h) {
  Results results("_freeptd (sub_825AA670)");
  ThreadSlots slots(h);
  if (slots.data == 0 || !HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _freeptd: no thread data or heap");
    return;
  }
  const uint32_t fls_index = Load(h.base, cc::kFlsIndex);
  for (int c = 0; c < 32; c++) {
    // 0: r3 the block; 1: r3 0, the block in the data slot; 2: as 1 with
    // the getter slot empty; 3: r3 the block, __flsindex -1.
    const int kind = c % 4;
    const uint32_t mask = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      std::vector<uint32_t> blocks;
      const uint32_t data = TestThreadData(h, mask, blocks);
      TlsSet(h, slots.getter_index, kind == 2 ? 0 : slots.getter);
      TlsSet(h, slots.data_index, kind == 1 || kind == 2 ? data : 0x1234);
      if (kind == 3) {
        Store(h.base, cc::kFlsIndex, 0xFFFFFFFFu);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = kind == 0 || kind == 3 ? data : 0;
      (v == 0 ? __imp__sub_825AA670 : hooks::CoreFreePtd)(t, h.base);
      Store(h.base, cc::kFlsIndex, fls_index);
      const uint32_t data_slot = TlsGet(h, slots.data_index);
      out[v].Word(data_slot == data ? 1 : data_slot == 0x1234 ? 2 : data_slot);
      out[v].Word(TlsGet(h, slots.getter_index));
      TlsSet(h, slots.getter_index, slots.getter);
      TlsSet(h, slots.data_index, slots.data);
      HeldOutputs(out[v], blocks);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void CritSecCases(Context& h) {
  Results results("__crtInitCritSecAndSpinCount (sub_825B35A0)");
  Results no_spin("__crtInitCritSecNoSpinCount (sub_825B3570)");
  const uint32_t init = Load(h.base, cc::kCritSecInit);
  const uint32_t section = h.data + 256;
  for (int c = 0; c < 24; c++) {
    const uint32_t spin = c % 3 == 0 ? 4000 : uint32_t(h.random.Next());
    const bool empty = c % 2 == 1;
    std::vector<uint8_t> fill(28);
    for (auto& b : fill) {
      b = uint8_t(h.random.Next());
    }
    for (int which = 0; which < 2; which++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Write(h.base, section, fill.data(), 28);
        Store(h.base, cc::kCritSecInit, empty ? 0 : cc::kCritSecNoSpinCount);
        PPCContext t = CallContext(h);
        t.r3.u64 = section;
        t.r4.u64 = spin;
        if (which == 0) {
          (v == 0 ? __imp__sub_825B35A0 : hooks::CoreInitCritSecAndSpinCount)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_825B3570 : hooks::CoreCritSecNoSpinCount)(t, h.base);
        }
        out[v].Word(t.r3.u32);
        out[v].Bytes(Read(h.base, section, 28));
        out[v].Word(Load(h.base, cc::kCritSecInit));
      }
      (which == 0 ? results : no_spin).Check(out[0], out[1], "case " + std::to_string(c));
    }
  }
  Store(h.base, cc::kCritSecInit, init);
  results.Report();
  no_spin.Report();
}

void EnterNotifyLock(Context& h, bool enter) {
  PPCContext t = CallContext(h);
  t.r3.u64 = cc::kNotifyLock;
  (enter ? __imp__RtlEnterCriticalSection : __imp__RtlLeaveCriticalSection)(t, h.base);
}

constexpr uint32_t kEmptyFunction = 0x82106540;
constexpr uint32_t kFilterSetter = 0x82297870;  // SetUnhandledExceptionFilter
constexpr uint32_t kFilter = 0x829714A0;

void RegisterNotifyCases(Context& h) {
  Results results("XRegisterThreadNotifyRoutine (sub_82297B98)");
  const uint32_t block = h.data + 512;
  EnterNotifyLock(h, true);
  for (int c = 0; c < 8; c++) {
    // Which side adds and which removes: bit 0 and bit 1.
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      const GuestFunction add = (c & 1) ? hooks::CoreRegisterThreadNotifyRoutine : __imp__sub_82297B98;
      const GuestFunction remove = (c & 2) ? hooks::CoreRegisterThreadNotifyRoutine : __imp__sub_82297B98;
      const GuestFunction self =
          v == 0 ? __imp__sub_82297B98 : hooks::CoreRegisterThreadNotifyRoutine;
      Store(h.base, block, uint32_t(h.random.Next()));
      Store(h.base, block + 4, uint32_t(h.random.Next()));
      Store(h.base, block + 8, kEmptyFunction);
      const uint32_t last = Load(h.base, cc::kNotifyList + 4);
      PPCContext t = CallContext(h);
      t.r3.u64 = block;
      t.r4.u64 = c < 4 ? 1 : uint32_t(h.random.Next()) | 1;
      (c < 4 ? self : add)(t, h.base);
      out[v].Bytes(Read(h.base, block, 8));
      out[v].Word(Load(h.base, cc::kNotifyList + 4));
      out[v].Word(Load(h.base, last));
      PPCContext r = CallContext(h);
      r.r3.u64 = block;
      r.r4.u64 = 0;
      (c < 4 ? self : remove)(r, h.base);
      out[v].Bytes(Read(h.base, cc::kNotifyList, 8));
      out[v].Word(Load(h.base, last));
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  EnterNotifyLock(h, false);
  results.Report();
}

void CallNotifyCases(Context& h) {
  Results results("XapiCallThreadNotifyRoutines (sub_82297B38)");
  const uint32_t blocks = h.data + 512;
  EnterNotifyLock(h, true);
  const std::vector<uint8_t> head = Read(h.base, cc::kNotifyList, 8);
  const uint32_t filter = Load(h.base, kFilter);
  for (int c = 0; c < 24; c++) {
    const uint32_t count = h.random.Below(4);
    const uint32_t routines = uint32_t(h.random.Next());
    const uint32_t starting = c % 3 == 2 ? uint32_t(h.random.Next()) : c % 3;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      uint32_t previous = cc::kNotifyList;
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t b = blocks + 12 * i;
        Store(h.base, previous, b);
        Store(h.base, b + 4, previous);
        Store(h.base, b + 8, routines >> i & 1 ? kFilterSetter : kEmptyFunction);
        previous = b;
      }
      Store(h.base, previous, cc::kNotifyList);
      Store(h.base, cc::kNotifyList + 4, previous);
      Store(h.base, kFilter, 0x5EB71E11);
      PPCContext t = CallContext(h);
      t.r3.u64 = starting;
      (v == 0 ? __imp__sub_82297B38 : hooks::CoreCallThreadNotifyRoutines)(t, h.base);
      out[v].Word(Load(h.base, kFilter));
      out[v].Bytes(Read(h.base, blocks, 36));
    }
    Write(h.base, cc::kNotifyList, head.data(), 8);
    Store(h.base, kFilter, filter);
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  EnterNotifyLock(h, false);
  results.Report();
}

}  // namespace

void CRuntimeCore(Context& h) {
  TlsAllocCases(h);
  SetFlsGetValueCases(h);
  GetptdNoexitCases(h);
  FreeFlsCases(h);
  MtDeleteLocksCases(h);
  MtTermCases(h);
  ErrnoCases(h);
  OsErrCases(h);
  DosMapErrCases(h);
  FreePtdCases(h);
  CritSecCases(h);
  RegisterNotifyCases(h);
  CallNotifyCases(h);
}

}  // namespace harness
