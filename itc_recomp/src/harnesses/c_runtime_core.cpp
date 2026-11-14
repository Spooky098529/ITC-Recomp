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

#include <cstring>
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

// The boot locks' count (lock table entries with +4 = 1).
uint32_t BootLocks(Context& h) {
  uint32_t n = 0;
  for (uint32_t i = 0; i < cc::kLockCount; i++) {
    n += Load(h.base, cc::kLockTable + 8 * i + 4) == 1;
  }
  return n;
}

// Guest ranges saved and put back.
struct Saved {
  Context& h;
  std::vector<std::pair<uint32_t, std::vector<uint8_t>>> ranges;
  Saved(Context& h, std::initializer_list<std::pair<uint32_t, uint32_t>> list) : h(h) {
    for (auto [address, bytes] : list) {
      ranges.emplace_back(address, Read(h.base, address, bytes));
    }
  }
  void Restore() {
    for (auto& [address, bytes] : ranges) {
      Write(h.base, address, bytes.data(), uint32_t(bytes.size()));
    }
  }
};

void MtInitLocksCases(Context& h) {
  Results results("_mtinitlocks (sub_825B31D8)");
  const uint32_t n = BootLocks(h);
  for (int c = 0; c < 12; c++) {
    // 0: the initializer set; 1: 0 (set by the first call); 2: one that fails.
    const int kind = c % 3;
    std::vector<uint8_t> fill(28 * n);
    for (auto& b : fill) {
      b = uint8_t(h.random.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved saved(h, {{cc::kLockTable, 8 * cc::kLockCount},
                      {cc::kLockSections, 28 * n},
                      {cc::kCritSecInit, 4}});
      Write(h.base, cc::kLockSections, fill.data(), 28 * n);
      Store(h.base, cc::kCritSecInit,
            kind == 0 ? cc::kCritSecNoSpinCount : kind == 1 ? 0 : kReturnsZero);
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825B31D8 : hooks::CoreMtInitLocks)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, cc::kLockTable, 8 * cc::kLockCount));
      out[v].Bytes(Read(h.base, cc::kLockSections, 28 * n));
      out[v].Word(Load(h.base, cc::kCritSecInit));
      saved.Restore();
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The start and end hooks (0x820723A0, 0x820723A4) are read-only image
// words (the empty function): only the data slot is set per case.
void ThreadNotifyCases(Context& h) {
  Results results("the C runtime's thread notify routine (sub_825B3650)");
  ThreadSlots slots(h);
  if (slots.data == 0 || !HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: thread notify: no thread data or heap");
    return;
  }
  for (int c = 0; c < 24; c++) {
    const uint32_t starting = c % 3 == 0 ? 1 : c % 3 == 1 ? 0 : uint32_t(h.random.Next()) | 2;
    const bool getter_empty = c % 4 == 3;
    const uint32_t mask = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      std::vector<uint32_t> blocks;
      const uint32_t data = TestThreadData(h, mask, blocks);
      TlsSet(h, slots.data_index, data);
      TlsSet(h, slots.getter_index, getter_empty ? 0 : slots.getter);
      PPCContext t = CallContext(h);
      t.r3.u64 = starting;
      (v == 0 ? __imp__sub_825B3650 : hooks::CoreThreadNotify)(t, h.base);
      const uint32_t data_slot = TlsGet(h, slots.data_index);
      out[v].Word(data_slot == data ? 1 : data_slot);
      out[v].Word(TlsGet(h, slots.getter_index));
      TlsSet(h, slots.getter_index, slots.getter);
      TlsSet(h, slots.data_index, slots.data);
      HeldOutputs(out[v], blocks);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// _mtinit on the game's globals, saved and put back around each side (the
// harness holding the notify list's lock, so the C runtime's notify block,
// linked again at the tail, is put back before any thread starts): its two
// new TLS slots and the main thread's new data are compared, then freed.
void MtInitCases(Context& h) {
  Results results("_mtinit (sub_825AA708)");
  if (!HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _mtinit: the heap isn't on the layer");
    return;
  }
  const uint32_t n = BootLocks(h);
  EnterNotifyLock(h, true);
  for (int c = 0; c < 6; c++) {
    std::vector<uint8_t> fill(28 * n);
    for (auto& b : fill) {
      b = uint8_t(h.random.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      const uint32_t last = Load(h.base, cc::kNotifyList + 4);
      Saved saved(h, {{cc::kFlsAlloc, 16},
                      {cc::kFlsIndex, 8},
                      {0x82971170, 4},
                      {0x82971178, 4},
                      {0x82978320, 4},
                      {0x82971328, 8},
                      {0x8266B7B0, 4},
                      {cc::kLockTable, 8 * cc::kLockCount},
                      {cc::kLockSections, 28 * n},
                      {cc::kCrtNotifyBlock, 12},
                      {cc::kNotifyList, 8},
                      {last, 4}});
      Write(h.base, cc::kLockSections, fill.data(), 28 * n);
      if (c % 2 == 1) {
        Store(h.base, cc::kCritSecInit, uint32_t(h.random.Next()));
      }
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825AA708 : hooks::CoreMtInit)(t, h.base);
      const uint32_t getter_index = Load(h.base, cc::kGetValueIndex);
      const uint32_t fls_index = Load(h.base, cc::kFlsIndex);
      const uint32_t data = TlsGet(h, fls_index);
      out[v].Word(t.r3.u32);
      out[v].Word(getter_index);
      out[v].Word(fls_index);
      out[v].Word(TlsGet(h, getter_index));
      for (const auto& range : saved.ranges) {
        if (range.first != cc::kFlsIndex) {
          out[v].Bytes(Read(h.base, range.first, uint32_t(range.second.size())));
        }
      }
      out[v].Bytes(Read(h.base, data, cc::kThreadDataBytes));
      saved.Restore();
      memory::crt::Free(data);
      TlsFree(h, fls_index);
      TlsFree(h, getter_index);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  EnterNotifyLock(h, false);
  results.Report();
}

std::string RandomText(Context& h, uint32_t length, const char* alphabet) {
  std::string text;
  const size_t n = std::strlen(alphabet);
  for (uint32_t i = 0; i < length; i++) {
    text.push_back(alphabet[h.random.Below(uint32_t(n))]);
  }
  return text;
}

void WriteText(Context& h, uint32_t address, const std::string& text) {
  Write(h.base, address, text.data(), uint32_t(text.size()));
  guest::StoreU8(h.base, address + uint32_t(text.size()), 0);
}

void StrtolCases(Context& h) {
  Results results("strtol (sub_825A85C0) and atol (sub_825AAF08)");
  ThreadSlots slots(h);  // errno paths write the main thread's errno
  const uint32_t errno_word = slots.data != 0 ? Load(h.base, slots.data + 8) : 0;
  const uint32_t text = h.data + 128, end_out = h.data + 64;
  static const char* kPrefixes[] = {"", " ", "  \t", "-", "+", "0x", "-0x", "0X", "0", " -0"};
  for (int c = 0; c < 600; c++) {
    std::string s = kPrefixes[h.random.Below(10)];
    s += RandomText(h, h.random.Below(c % 50 == 0 ? 30 : 12),
                    c % 3 == 0 ? "0123456789" : "0123456789abcdefxyzABCZ -+");
    const bool atol = c % 4 == 0;
    // Bases out of range go to _invalid_parameter's trap: left out.
    static const int32_t kRadixes[] = {0, 2, 8, 10, 16, 36};
    const int32_t radix =
        c % 7 == 0 ? int32_t(2 + h.random.Below(35)) : kRadixes[h.random.Below(6)];
    const bool want_end = h.random.Below(2) != 0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      WriteText(h, text, s);
      Store(h.base, end_out, 0xCDCDCDCDu);
      if (slots.data != 0) {
        Store(h.base, slots.data + 8, 0);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = text;
      t.r4.u64 = want_end ? end_out : 0;
      t.r5.u64 = uint64_t(int64_t(radix));
      if (atol) {
        (v == 0 ? __imp__sub_825AAF08 : hooks::CoreAtol)(t, h.base);
      } else {
        (v == 0 ? __imp__sub_825A85C0 : hooks::CoreStrtol)(t, h.base);
      }
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Word(Load(h.base, end_out));
      out[v].Word(slots.data != 0 ? Load(h.base, slots.data + 8) : 0);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c) + " \"" + s + "\"");
  }
  if (slots.data != 0) {
    Store(h.base, slots.data + 8, errno_word);
  }
  results.Report();
}

void StrrchrCases(Context& h) {
  Results results("strrchr (sub_825A8630)");
  const uint32_t text = h.data + 128;
  for (int c = 0; c < 400; c++) {
    const std::string s = RandomText(h, h.random.Below(40), "aab.c.\xff\x80x");
    uint32_t ch;
    switch (c % 5) {
      case 0: ch = 0; break;
      case 1: ch = uint8_t(".ax\xff\x80"[h.random.Below(5)]); break;
      case 2: ch = uint32_t(int32_t(int8_t("\xff\x80"[h.random.Below(2)]))); break;
      case 3: ch = 0x100 | uint8_t('a'); break;
      default: ch = uint32_t(h.random.Next()); break;
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      WriteText(h, text, s);
      PPCContext t = CallContext(h);
      t.r3.u64 = text;
      t.r4.u64 = ch;
      (v == 0 ? __imp__sub_825A8630 : hooks::CoreStrrchr)(t, h.base);
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void InittermCases(Context& h) {
  Results results("_initterm (sub_825AA1B8)");
  const uint32_t table = h.data + 128;
  const uint32_t filter = Load(h.base, kFilter);
  for (int c = 0; c < 40; c++) {
    const uint32_t count = h.random.Below(8);
    std::vector<uint32_t> entries;
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t k = h.random.Below(3);
      entries.push_back(k == 0 ? 0 : k == 1 ? kEmptyFunction : kFilterSetter);
    }
    const uint32_t end = c % 9 == 8 ? table - 4 : table + 4 * count;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t i = 0; i < count; i++) {
        Store(h.base, table + 4 * i, entries[i]);
      }
      Store(h.base, kFilter, 0x5EB71E11);
      PPCContext t = CallContext(h);
      t.r3.u64 = table;
      t.r4.u64 = end;
      (v == 0 ? __imp__sub_825AA1B8 : hooks::CoreInitterm)(t, h.base);
      out[v].Word(Load(h.base, kFilter));
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  Store(h.base, kFilter, filter);
  results.Report();
}

// A function over one global word: the word set to `before`, r3 and r4 set,
// both versions run; r3 and the word compared, the word put back.
void GlobalCases(Context& h, const char* name, uint32_t global, GuestFunction recompiled,
                 GuestFunction native, bool writable = true) {
  Results results(name);
  const uint32_t saved = Load(h.base, global);
  for (int c = 0; c < 40; c++) {
    const uint32_t before = c == 0 ? saved : uint32_t(h.random.Next());
    const uint32_t r3 = c % 5 == 0 ? 0 : uint32_t(h.random.Next());
    const uint32_t r4 = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      if (writable) {
        Store(h.base, global, before);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = r3;
      t.r4.u64 = r4;
      (v == 0 ? recompiled : native)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Word(Load(h.base, global));
    }
    if (writable) {
      Store(h.base, global, saved);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void PositiveCases(Context& h) {
  Results results("_positive (sub_825ADE90)");
  const uint32_t number = h.data + 128;
  static const uint64_t kEdges[] = {0, 0x8000000000000000ull, 1, 0x8000000000000001ull,
                                    0x7FF0000000000000ull, 0xFFF0000000000000ull,
                                    0x7FF8000000000000ull, 0xFFF8000000000001ull,
                                    0x3FF0000000000000ull, 0xBFF0000000000000ull};
  for (int c = 0; c < 200; c++) {
    const uint64_t bits = c < 10 ? kEdges[c] : h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      StoreU64(h.base, number, bits);
      PPCContext t = CallContext(h);
      t.r3.u64 = number;
      (v == 0 ? __imp__sub_825ADE90 : hooks::CorePositive)(t, h.base);
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void NmsgWriteCases(Context& h) {
  Results results("_NMSG_WRITE (sub_825AEDA0)");
  // Its message goes to OutputDebugStringA, which drops it: compared, the
  // scratch area after (nothing written) for numbers in and out of the table.
  for (int c = 0; c < 46; c++) {
    const uint32_t number =
        c < 23 ? Load(h.base, 0x8266BA88 + 8 * c) : uint32_t(h.random.Below(300));
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, 256);
      PPCContext t = CallContext(h);
      t.r3.u64 = number;
      (v == 0 ? __imp__sub_825AEDA0 : hooks::CoreNmsgWrite)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, 256));
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void BufferCases(Context& h) {
  Results getbuf("_getbuf (sub_825B4790)");
  Results freebuf("_freebuf (sub_825B0EE0)");
  if (!HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _getbuf: the heap isn't on the layer");
    return;
  }
  const uint32_t stream = h.data + 128;
  const uint32_t cflush = Load(h.base, 0x82971174);
  for (int c = 0; c < 40; c++) {
    std::vector<uint32_t> words(8);
    for (auto& w : words) {
      w = uint32_t(h.random.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t i = 0; i < 8; i++) {
        Store(h.base, stream + 4 * i, words[i]);
      }
      Store(h.base, 0x82971174, 7);
      PPCContext t = CallContext(h);
      t.r3.u64 = stream;
      (v == 0 ? __imp__sub_825B4790 : hooks::CoreGetbuf)(t, h.base);
      const uint32_t buffer = Load(h.base, stream + 8);
      const bool held = buffer != 0 && memory::IsHeapBlock(buffer);
      out[v].Word(held ? 1 : buffer == stream + 20 ? 2 : buffer);
      out[v].Word(Load(h.base, stream + 0) == buffer);
      for (uint32_t offset : {4u, 12u, 16u, 20u, 24u, 28u}) {
        out[v].Word(Load(h.base, stream + offset));
      }
      out[v].Word(Load(h.base, 0x82971174));
      if (held) {
        out[v].Word(memory::Size(buffer));
        memory::crt::Free(buffer);
      }
    }
    getbuf.Check(out[0], out[1], "case " + std::to_string(c));
  }
  Store(h.base, 0x82971174, cflush);
  for (int c = 0; c < 60; c++) {
    std::vector<uint32_t> words(8);
    for (auto& w : words) {
      w = uint32_t(h.random.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t i = 0; i < 8; i++) {
        Store(h.base, stream + 4 * i, words[i]);
      }
      uint32_t block = 0;
      const uint32_t flags = words[3];
      if ((flags & 0x83) != 0 && (flags & 8) != 0) {
        block = Block(h.base, 64);
        Store(h.base, stream + 8, block);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = stream;
      (v == 0 ? __imp__sub_825B0EE0 : hooks::CoreFreebuf)(t, h.base);
      std::vector<uint8_t> after = Read(h.base, stream, 32);
      if (block != 0 && Load(h.base, stream + 8) == block) {
        Store(h.base, stream + 8, 1);  // kept: the same on both sides
      }
      out[v].Bytes(Read(h.base, stream, 32));
      if (block != 0) {
        HeldOutputs(out[v], {block});
      }
    }
    freebuf.Check(out[0], out[1], "case " + std::to_string(c));
  }
  getbuf.Report();
  freebuf.Report();
}

uint32_t MakeEvent(Context& h, uint32_t type, uint32_t state) {
  PPCContext t = CallContext(h);
  t.r3.u64 = h.data + 32;
  t.r4.u64 = 0;
  t.r5.u64 = type;
  t.r6.u64 = state;
  __imp__NtCreateEvent(t, h.base);
  return int32_t(t.r3.u32) < 0 ? 0 : Load(h.base, h.data + 32);
}

void HandleCases(Context& h) {
  Results close("CloseHandle (sub_82294E20)");
  Results reset("ResetEvent (sub_825BA168)");
  const uint32_t error = GuestLastError(h);
  for (int c = 0; c < 30; c++) {
    // 0: an event of its own; 1: 0; 2: a word that's no handle.
    const int kind = c % 3;
    const uint32_t junk = 0x12340000u + h.random.Below(0x10000);
    const uint32_t type = h.random.Below(2), state = h.random.Below(2);
    for (int which = 0; which < 2; which++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        SetLastError(h, 0xABCD);
        const uint32_t event = kind == 0 ? MakeEvent(h, type, state) : kind == 1 ? 0 : junk;
        PPCContext t = CallContext(h);
        t.r3.u64 = event;
        if (which == 0) {
          (v == 0 ? __imp__sub_82294E20 : hooks::CoreCloseHandle)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_825BA168 : hooks::CoreResetEvent)(t, h.base);
        }
        out[v].Word(t.r3.u32);
        out[v].Word(GuestLastError(h));
        if (kind == 0 && event != 0) {
          // Signaled after? (a wait with a 0 timeout)
          StoreU64(h.base, h.data + 40, 0);
          PPCContext w = CallContext(h);
          w.r3.u64 = event;
          w.r4.u64 = 1;
          w.r5.u64 = 0;
          w.r6.u64 = h.data + 40;
          __imp__NtWaitForSingleObjectEx(w, h.base);
          out[v].Word(which == 1 ? w.r3.u32 : int32_t(w.r3.u32) < 0);
          if (which == 1) {
            PPCContext k = CallContext(h);
            k.r3.u64 = event;
            __imp__NtClose(k, h.base);
          }
        }
      }
      (which == 0 ? close : reset).Check(out[0], out[1], "case " + std::to_string(c));
    }
  }
  SetLastError(h, error);
  close.Report();
  reset.Report();
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
  MtInitLocksCases(h);
  ThreadNotifyCases(h);
  MtInitCases(h);
  StrtolCases(h);
  StrrchrCases(h);
  InittermCases(h);
  GlobalCases(h, "_set_abort_behavior (sub_825AD120)", 0x8266B7C4, __imp__sub_825AD120,
              hooks::CoreSetAbortBehavior);
  GlobalCases(h, "__get_sigabrt (sub_825B49B0)", 0x82971340, __imp__sub_825B49B0,
              hooks::CoreGetSigabrt);
  GlobalCases(h, "_crt_debugger_hook (sub_825B68B8)", 0x8297820C, __imp__sub_825B68B8,
              hooks::CoreCrtDebuggerHook);
  GlobalCases(h, "_ValidateExecute (sub_825B8910)", h.data + 64, __imp__sub_825B8910,
              hooks::CoreValidateExecute);
  GlobalCases(h, "SetUnhandledExceptionFilter (sub_82297870)", kFilter, __imp__sub_82297870,
              hooks::CoreSetUnhandledExceptionFilter);
  GlobalCases(h, "GetCommandLineA (sub_822979D0)", 0x820007EC, __imp__sub_822979D0,
              hooks::CoreGetCommandLineA, false);
  PositiveCases(h);
  NmsgWriteCases(h);
  BufferCases(h);
  HandleCases(h);
}

}  // namespace harness
