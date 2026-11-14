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
#include "c_runtime/exit.h"
#include "hooks/c_runtime_core.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <algorithm>
#include <bit>
#include <cstring>
#include <mutex>
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

// _ioinit on the game's globals, saved and put back: each side's new block
// compared by contents, then freed.
void IoInitCases(Context& h) {
  Results results("_ioinit (sub_825B4668)");
  if (!HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _ioinit: the heap isn't on the layer");
    return;
  }
  for (int c = 0; c < 6; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved saved(h, {{cc::kNhandle, 4}, {cc::kPioinfo, 4}});
      Store(h.base, cc::kNhandle, uint32_t(h.random.Next()));
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825B4668 : hooks::CoreIoInit)(t, h.base);
      const uint32_t block = Load(h.base, cc::kPioinfo);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Word(Load(h.base, cc::kNhandle));
      out[v].Bytes(Read(h.base, block, 32 * 44));
      saved.Restore();
      memory::crt::Free(block);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// _initterm_e over the game's table (__onexitinit, _ioinit) on saved
// globals, the exit lock held: each side's new exit table and handle block
// compared, then freed, the game's put back.
void InittermECases(Context& h) {
  Results results("_initterm_e (sub_82297AC0)");
  if (!HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _initterm_e: the heap isn't on the layer");
    return;
  }
  std::lock_guard<std::recursive_mutex> exit_lock(c_runtime::ExitMutex());
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved saved(h, {{cc::kNhandle, 4},
                      {cc::kPioinfo, 4},
                      {c_runtime::kExitTableEnd, 4},
                      {c_runtime::kExitTableBegin, 4}});
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_82297AC0 : hooks::CoreInittermE)(t, h.base);
      const uint32_t block = Load(h.base, cc::kPioinfo);
      const uint32_t table = Load(h.base, c_runtime::kExitTableBegin);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r3.u32);
      out[v].Word(Load(h.base, cc::kNhandle));
      out[v].Bytes(Read(h.base, block, 32 * 44));
      out[v].Word(Load(h.base, c_runtime::kExitTableEnd) - table);
      out[v].Bytes(Read(h.base, table, 128));
      out[v].Word(memory::Size(table));
      saved.Restore();
      memory::crt::Free(block);
      memory::crt::Free(table);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// _initstdio on saved globals (_nstream, __piob, _iob, the first handle
// block's first three entries' OS handles set per case): each side's new
// __piob compared by contents, then freed.
void InitStdioCases(Context& h) {
  Results results("_initstdio (sub_825AD4D0)");
  if (!HeapOnLayer()) {
    ITC_LOG_ERROR("harness: c_runtime_core: _initstdio: the heap isn't on the layer");
    return;
  }
  const uint32_t handles = Load(h.base, cc::kPioinfo);
  if (handles == 0) {
    ITC_LOG_ERROR("harness: c_runtime_core: _initstdio: no handle block");
    return;
  }
  static const uint32_t kHandles[] = {0xFFFFFFFFu, 0xFFFFFFFEu, 0, 5, 0xF8000010u};
  for (int c = 0; c < 30; c++) {
    const uint32_t k = h.random.Below(4);
    const uint32_t count = k == 0 ? 0 : k == 1 ? h.random.Below(20) : k == 2 ? 20 + h.random.Below(600) : 512;
    uint32_t os[3];
    for (auto& o : os) {
      o = kHandles[h.random.Below(5)];
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved saved(h, {{cc::kNstream, 4}, {cc::kPiob, 4}, {cc::kIob, 640}, {handles, 132}});
      Store(h.base, cc::kNstream, count);
      for (uint32_t i = 0; i < 3; i++) {
        Store(h.base, handles + 44 * i, os[i]);
      }
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825AD4D0 : hooks::CoreInitStdio)(t, h.base);
      const uint32_t n = Load(h.base, cc::kNstream);
      const uint32_t piob = Load(h.base, cc::kPiob);
      out[v].Word(t.r3.u32);
      out[v].Word(n);
      out[v].Bytes(Read(h.base, piob, 4 * n));
      out[v].Word(memory::Size(piob));
      out[v].Bytes(Read(h.base, cc::kIob, 640));
      saved.Restore();
      memory::crt::Free(piob);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

void FfMsgBannerCases(Context& h) {
  Results results("_FF_MSGBANNER (sub_825AEDE8)");
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, 256);
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825AEDE8 : hooks::CoreFfMsgBanner)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, 256));
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// A critical section's words: the type and lock count, recursion count and
// owner (compared exactly; the same thread on both sides).
std::vector<uint8_t> Section(Context& h, uint32_t section) { return Read(h.base, section, 28); }

void InitSection(Context& h, uint32_t section) {
  PPCContext t = CallContext(h);
  t.r3.u64 = section;
  __imp__RtlInitializeCriticalSection(t, h.base);
}

// The stream locks: each case takes a lock and gives it back, both through
// one side: a stream outside _iob (its own critical section at +32, in the
// scratch area), or one of _iob's whose lock is made at boot (the lock
// table's 16 + index with +4 = 1; others would be made here for good).
void LockFileCases(Context& h) {
  Results one("_lock_file and _unlock_file (sub_825AD638, sub_825AD688)");
  Results two("_lock_file2 and _unlock_file2 (sub_825AD670, sub_825AD6C0)");
  std::vector<uint32_t> boot_streams;
  for (uint32_t i = 0; i < 20; i++) {
    if (Load(h.base, cc::kLockTable + 8 * (16 + i) + 4) == 1 &&
        Load(h.base, cc::kLockTable + 8 * (16 + i)) != 0) {
      boot_streams.push_back(i);
    }
  }
  const uint32_t stream = h.data + 256;
  for (int c = 0; c < 24; c++) {
    const bool in_iob = !boot_streams.empty() && c % 2 == 1;
    const uint32_t index = in_iob ? boot_streams[h.random.Below(uint32_t(boot_streams.size()))]
                                  : 20 + h.random.Below(500);
    const uint32_t target = in_iob ? cc::kIob + 32 * index : stream;
    const uint32_t section =
        in_iob ? Load(h.base, cc::kLockTable + 8 * (16 + index)) : stream + 32;
    for (int which = 0; which < 2; which++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        if (!in_iob) {
          InitSection(h, section);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = which == 0 ? target : index;
        t.r4.u64 = target;
        if (which == 0) {
          (v == 0 ? __imp__sub_825AD638 : hooks::CoreLockFile)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_825AD670 : hooks::CoreLockFile2)(t, h.base);
        }
        out[v].Bytes(Section(h, section));
        PPCContext u = CallContext(h);
        u.r3.u64 = which == 0 ? target : index;
        u.r4.u64 = target;
        if (which == 0) {
          (v == 0 ? __imp__sub_825AD688 : hooks::CoreUnlockFile)(u, h.base);
        } else {
          (v == 0 ? __imp__sub_825AD6C0 : hooks::CoreUnlockFile2)(u, h.base);
        }
        out[v].Bytes(Section(h, section));
      }
      (which == 0 ? one : two).Check(out[0], out[1], "case " + std::to_string(c));
    }
  }
  one.Report();
  two.Report();
}

// _unlock_fhandle: a handle's entry in the first block, its critical
// section (+12) made and entered for the case, the entry put back after.
void UnlockFhandleCases(Context& h) {
  Results results("_unlock_fhandle (sub_825B7A58)");
  const uint32_t handles = Load(h.base, cc::kPioinfo);
  if (handles == 0) {
    ITC_LOG_ERROR("harness: c_runtime_core: _unlock_fhandle: no handle block");
    return;
  }
  for (int c = 0; c < 32; c++) {
    const uint32_t handle = h.random.Below(32);
    const uint32_t entry = handles + 44 * handle;
    const int depth = 1 + int(h.random.Below(3));
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved saved(h, {{entry, 44}});
      InitSection(h, entry + 12);
      for (int d = 0; d < depth; d++) {
        PPCContext e = CallContext(h);
        e.r3.u64 = entry + 12;
        __imp__RtlEnterCriticalSection(e, h.base);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = handle;
      (v == 0 ? __imp__sub_825B7A58 : hooks::CoreUnlockFhandle)(t, h.base);
      out[v].Bytes(Read(h.base, entry, 44));
      saved.Restore();
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The exit routine _amsg_exit calls ([0x8266B7B0], _exit in play) pointed
// at a guest function that returns for the case: the shared "returns 0" or
// _ValidateExecute (r3 != 0, so 1 for its 255), put back after.
constexpr uint32_t kValidateExecute = 0x825B8910;

struct ExitStub {
  Context& h;
  uint32_t saved;
  ExitStub(Context& h, uint32_t stub) : h(h), saved(Load(h.base, cc::kExitRoutine)) {
    Store(h.base, cc::kExitRoutine, stub);
  }
  ~ExitStub() { Store(h.base, cc::kExitRoutine, saved); }
};

// _amsg_exit and _fptrap: run-time error numbers in and out of the message
// table, the exit routine stubbed; r3 compared.
void AmsgExitCases(Context& h) {
  Results results("_amsg_exit (sub_825AA158)");
  for (int c = 0; c < 40; c++) {
    const uint32_t number =
        c < 23 ? Load(h.base, 0x8266BA88 + 8 * c) : uint32_t(h.random.Below(300));
    const uint32_t stub = c % 2 == 0 ? kReturnsZero : kValidateExecute;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      ExitStub exit(h, stub);
      PPCContext t = CallContext(h);
      t.r3.u64 = number;
      (v == 0 ? __imp__sub_825AA158 : hooks::CoreAmsgExit)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Word(Load(h.base, cc::kExitRoutine));
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
  Results fptrap("_fptrap (sub_825B64F8)");
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      ExitStub exit(h, c % 2 == 0 ? kReturnsZero : kValidateExecute);
      PPCContext t = CallContext(h);
      t.r3.u64 = uint32_t(h.random.Next());
      (v == 0 ? __imp__sub_825B64F8 : hooks::CoreFptrap)(t, h.base);
      out[v].Word(t.r3.u32);
    }
    fptrap.Check(out[0], out[1], "case " + std::to_string(c));
  }
  fptrap.Report();
}

// _getptd on the harness's thread: its data there (returned), or none with
// the store failing (_getptd_noexit's 0, then _amsg_exit(16) with the exit
// routine stubbed); r3 and the last error compared.
void GetptdCases(Context& h) {
  Results results("_getptd (sub_825AA580)");
  ThreadSlots slots(h);
  if (slots.data == 0) {
    ITC_LOG_ERROR("harness: c_runtime_core: _getptd: no thread data");
    return;
  }
  for (int c = 0; c < 16; c++) {
    const int kind = c % 2 == 0 ? 0 : 2;
    const uint32_t error = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      SetLastError(h, error);
      ExitStub exit(h, c % 4 < 2 ? kReturnsZero : kValidateExecute);
      WithThreadData(h, slots, kind, [&] {
        PPCContext t = CallContext(h);
        (v == 0 ? __imp__sub_825AA580 : hooks::CoreGetptd)(t, h.base);
        out[v].Word(kind == 0 ? uint32_t(t.r3.u32 == slots.data) : t.r3.u32);
        out[v].Word(GuestLastError(h));
      });
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// _invalid_parameter's path with a handler set (its trap path, with none,
// is checked by reading: the user's call, 2026-11-14): the handler the
// shared "returns 0", strrchr (r3 a string in the scratch area, r4 a
// character) or the second memcpy (r3, r4, r5: a copy inside the scratch
// area), so r3 to r5 reach it as given; r3 and the scratch area compared.
void InvalidParameterCases(Context& h) {
  Results results("_invalid_parameter (sub_825AFCC8)");
  const uint32_t saved = Load(h.base, cc::kInvalidParameterHandler);
  for (int c = 0; c < 48; c++) {
    const int kind = c % 3;
    const uint32_t handler = kind == 0 ? kReturnsZero : kind == 1 ? 0x825A8630 : 0x825A7830;
    const uint32_t r3 = h.data + h.random.Below(64);
    const uint32_t r4 = kind == 1 ? h.random.Below(256) : h.data + 128 + h.random.Below(64);
    const uint32_t r5 = h.random.Below(48);
    const uint32_t r6 = uint32_t(h.random.Next()), r7 = uint32_t(h.random.Next());
    std::vector<uint8_t> fill(256);
    for (auto& b : fill) {
      b = uint8_t(h.random.Below(8) == 0 ? 0 : 1 + h.random.Below(255));
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, h.data, fill.data(), 256);
      Store(h.base, cc::kInvalidParameterHandler, handler);
      PPCContext t = CallContext(h);
      t.r3.u64 = r3;
      t.r4.u64 = r4;
      t.r5.u64 = r5;
      t.r6.u64 = r6;
      t.r7.u64 = r7;
      (v == 0 ? __imp__sub_825AFCC8 : hooks::CoreInvalidParameter)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, h.data, 256));
      Store(h.base, cc::kInvalidParameterHandler, saved);
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The debug monitor (KeDebugMonitorData's word, at [0x820007F0]: the SDK's
// system heap, 0 in play) attached for a case: a block in the scratch area
// whose +24 is `routine`; put back after. _set_abort_behavior (sub_825AD120)
// as a routine or filter leaves a trace of its r3 and r4 in its global
// ([0x8266B7C4]), and returns its old value.
constexpr uint32_t kAbortBehavior = 0x8266B7C4;
constexpr uint32_t kSetAbortBehavior = 0x825AD120;

struct Monitor {
  Context& h;
  uint32_t word, saved;
  Monitor(Context& h, uint32_t routine)
      : h(h), word(Load(h.base, cc::kDebugMonitorData)), saved(Load(h.base, word)) {
    if (routine != 0) {
      Store(h.base, h.data + 1024 + 24, routine);
      Store(h.base, word, h.data + 1024);
    } else {
      Store(h.base, word, 0);
    }
  }
  ~Monitor() { Store(h.base, word, saved); }
};

uint32_t PickRoutine(Context& h) {
  const uint32_t pick = h.random.Below(3);
  return pick == 0 ? 0 : pick == 1 ? kReturnsZero : kSetAbortBehavior;
}

// UnhandledExceptionFilter: no monitor or one returning 0 or the abort
// word's old value; the filter none, the shared "returns 0" or
// _set_abort_behavior (the abort word -1 now and then, so it returns -1);
// compared: r3, the abort word and the filter word.
void UnhandledFilterCases(Context& h) {
  Results results("UnhandledExceptionFilter (sub_82297940)");
  const uint32_t filter_saved = Load(h.base, cc::kUnhandledFilter);
  const uint32_t abort_saved = Load(h.base, kAbortBehavior);
  for (int c = 0; c < 60; c++) {
    const uint32_t routine = PickRoutine(h);
    const uint32_t filter = PickRoutine(h);
    const uint32_t abort = c % 3 == 0 ? 0xFFFFFFFFu : c % 3 == 1 ? 0 : uint32_t(h.random.Next());
    const uint32_t pointers = uint32_t(h.random.Next());
    const uint32_t r4 = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Monitor monitor(h, routine);
      Store(h.base, cc::kUnhandledFilter, filter);
      Store(h.base, kAbortBehavior, abort);
      PPCContext t = CallContext(h);
      t.r3.u64 = pointers;
      t.r4.u64 = r4;
      (v == 0 ? __imp__sub_82297940 : hooks::CoreUnhandledExceptionFilter)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(Load(h.base, kAbortBehavior));
      out[v].Word(Load(h.base, cc::kUnhandledFilter));
    }
    Store(h.base, cc::kUnhandledFilter, filter_saved);
    Store(h.base, kAbortBehavior, abort_saved);
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The debug monitor notification: no monitor, or one that's the shared
// "returns 0" or _set_abort_behavior (its global then a trace of both
// calls' r4: the code with 0x02200000, the float's bits); f1 edges and
// random doubles (a float, so its rounding is compared through the word
// stored in the caller's frame, exactly: it's a store, not a float the
// function computes with); compared: that word, the abort word.
void DebugMonitorCases(Context& h) {
  Results results("the debug monitor notification (sub_825B9BA8)");
  const uint32_t abort_saved = Load(h.base, kAbortBehavior);
  static const uint64_t kEdges[] = {0, 0x8000000000000000ull, 0x3FF0000000000000ull,
                                    0x7FF0000000000000ull, 0x7FF8000000000000ull,
                                    0x47EFFFFFF0000000ull, 0x36A0000000000000ull,
                                    0x3FF0000010000000ull};
  for (int c = 0; c < 60; c++) {
    const uint32_t routine = PickRoutine(h);
    const uint32_t code = uint32_t(h.random.Next());
    const uint64_t value = c < 8 ? kEdges[c] : h.random.Next();
    const uint32_t abort = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Monitor monitor(h, routine);
      Store(h.base, kAbortBehavior, abort);
      PPCContext t = CallContext(h);
      Store(h.base, t.r1.u32 + 28, 0xCDCDCDCDu);
      t.r3.u64 = code;
      t.f1.u64 = value;
      (v == 0 ? __imp__sub_825B9BA8 : hooks::CoreDebugMonitorNotify)(t, h.base);
      out[v].Word(Load(h.base, t.r1.u32 + 28));
      out[v].Word(Load(h.base, kAbortBehavior));
    }
    Store(h.base, kAbortBehavior, abort_saved);
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// __CxxRestoreUnhandledExceptionFilter: the previous and current filter
// words random; r3 and both compared, put back after.
void CxxRestoreFilterCases(Context& h) {
  Results results("__CxxRestoreUnhandledExceptionFilter (sub_825B4D90)");
  const uint32_t filter_saved = Load(h.base, cc::kUnhandledFilter);
  const uint32_t previous_saved = Load(h.base, cc::kCxxPreviousFilter);
  for (int c = 0; c < 20; c++) {
    const uint32_t filter = uint32_t(h.random.Next());
    const uint32_t previous = c % 4 == 0 ? 0 : uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Store(h.base, cc::kUnhandledFilter, filter);
      Store(h.base, cc::kCxxPreviousFilter, previous);
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_825B4D90 : hooks::CoreCxxRestoreUnhandledExceptionFilter)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Word(Load(h.base, cc::kUnhandledFilter));
      out[v].Word(Load(h.base, cc::kCxxPreviousFilter));
    }
    Store(h.base, cc::kUnhandledFilter, filter_saved);
    Store(h.base, cc::kCxxPreviousFilter, previous_saved);
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// Whether the C runtime's lock `n` was made at boot (its table entry's +4 is
// 1 and its section set): only those are taken here (_lock would make the
// others for good).
bool BootLock(Context& h, uint32_t n) {
  return Load(h.base, cc::kLockTable + 8 * n + 4) == 1 && Load(h.base, cc::kLockTable + 8 * n) != 0;
}
void GuestLock(Context& h, uint32_t n, bool lock) {
  PPCContext t = CallContext(h);
  t.r3.u64 = n;
  (lock ? sub_825B3430 : sub_825B32D0)(t, h.base);
}

// __lock_fhandle: a handle in the first block, its entry's +8 0 (the section
// made here) or 1 (made for the case), lock 10 taken around a make; compared:
// r3, the entry (the section entered) and the handle's word in the caller's
// frame (+20); the section left and the entry put back after.
void LockFhandleCases(Context& h) {
  Results results("__lock_fhandle (sub_825B7958)");
  const uint32_t handles = Load(h.base, cc::kPioinfo);
  if (handles == 0 || !BootLock(h, 10)) {
    ITC_LOG_ERROR("harness: c_runtime_core: __lock_fhandle: no handle block or lock 10");
    return;
  }
  for (int c = 0; c < 32; c++) {
    const uint32_t handle = h.random.Below(32);
    const uint32_t entry = handles + 44 * handle;
    const bool made = c % 2 == 1;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved saved(h, {{entry, 44}});
      Store(h.base, entry + 8, made ? 1 : 0);
      if (made) {
        InitSection(h, entry + 12);
      }
      PPCContext t = CallContext(h);
      Store(h.base, t.r1.u32 + 20, 0xCDCDCDCDu);
      t.r3.u64 = handle;
      (v == 0 ? __imp__sub_825B7958 : hooks::CoreLockFhandle)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, entry, 44));
      out[v].Word(Load(h.base, t.r1.u32 + 20));
      if (t.r3.u32 != 0) {
        PPCContext u = CallContext(h);
        u.r3.u64 = entry + 12;
        __imp__RtlLeaveCriticalSection(u, h.base);
      }
      saved.Restore();
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The __finally funclets, each after the lock it gives back was taken (only
// locks made at boot); r31 or r12 at a frame in the scratch area of random
// words; compared: the registers each sets.
void FinallyCases(Context& h) {
  {
    Results results("__lock_fhandle's __finally (sub_825B7A20)");
    for (int c = 0; c < 16 && BootLock(h, 10); c++) {
      for (uint32_t i = 0; i < 256; i += 4) {
        Store(h.base, h.data + 512 + i, uint32_t(h.random.Next()));
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        GuestLock(h, 10, true);
        PPCContext t = CallContext(h);
        t.r31.u64 = h.data + 512;
        t.r12.u64 = h.data + 640;
        (v == 0 ? __imp__sub_825B7A20 : hooks::CoreLockFhandleFinally)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(t.r29.u32);
        out[v].Word(t.r11.u32);
        out[v].Word(t.r31.u32);
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
  {
    Results results("raise's __finally (sub_825B4C8C)");
    for (int c = 0; c < 16 && BootLock(h, 0); c++) {
      const uint32_t locked = c % 2 == 0 ? 0 : uint32_t(h.random.Next()) | 1;
      const uint32_t r3 = uint32_t(h.random.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        if (locked != 0) {
          GuestLock(h, 0, true);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = r3;
        t.r28.u64 = locked;
        t.r31.u64 = 0x1234;
        t.r12.u64 = h.data + 640;
        (v == 0 ? __imp__sub_825B4C8C : hooks::CoreRaiseFinally)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(t.r28.u32);
        out[v].Word(t.r31.u32);
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
  struct Fn {
    const char* name;
    GuestFunction recompiled;
  };
  for (const Fn& fn : {Fn{"_fcloseall's __finally (sub_825B4F38)", __imp__sub_825B4F38},
                       Fn{"flsall's outer __finally (sub_825ACEB4)", __imp__sub_825ACEB4}}) {
    Results results(fn.name);
    for (int c = 0; c < 8 && BootLock(h, 1); c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        GuestLock(h, 1, true);
        PPCContext t = CallContext(h);
        (v == 0 ? fn.recompiled : hooks::CoreUnlockStreamsFinally)(t, h.base);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
  {
    Results results("flsall's inner __finally (sub_825ACF04)");
    std::vector<uint32_t> boot_streams;
    for (uint32_t i = 0; i < 20; i++) {
      if (BootLock(h, 16 + i)) {
        boot_streams.push_back(i);
      }
    }
    for (int c = 0; c < 16 && !boot_streams.empty(); c++) {
      const uint32_t index = boot_streams[h.random.Below(uint32_t(boot_streams.size()))];
      const uint32_t section = Load(h.base, cc::kLockTable + 8 * (16 + index));
      for (uint32_t i = 0; i < 256; i += 4) {
        Store(h.base, h.data + 512 + i, uint32_t(h.random.Next()));
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        GuestLock(h, 16 + index, true);
        PPCContext t = CallContext(h);
        t.r28.u64 = index;
        t.r29.u64 = cc::kPiob;
        t.r31.u64 = 0x5678;
        t.r12.u64 = h.data + 640;
        (v == 0 ? __imp__sub_825ACF04 : hooks::CoreFlsallInnerFinally)(t, h.base);
        for (const PPCRegister* r : {&t.r3, &t.r4, &t.r10, &t.r11, &t.r27, &t.r28, &t.r29, &t.r31}) {
          out[v].Word(r->u32);
        }
        out[v].Bytes(Section(h, section));
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
}

// _free_osfhnd and _get_osfhandle: handles -2, -1, past _nhandle and in the
// first block, whose entry's flags (+4) and OS handle (+0) are set for the
// case; errno and _doserrno (the harness thread's) random before; the
// invalid parameter handler the shared "returns 0" (its trap otherwise
// logs); compared: r3 (64 bits), the entry, errno and _doserrno.
void OsHandleCases(Context& h) {
  const uint32_t handles = Load(h.base, cc::kPioinfo);
  ThreadSlots slots(h);
  if (handles == 0 || slots.data == 0) {
    ITC_LOG_ERROR("harness: c_runtime_core: _free_osfhnd: no handle block or thread data");
    return;
  }
  const uint32_t nhandle = Load(h.base, cc::kNhandle);
  const uint32_t handler = Load(h.base, cc::kInvalidParameterHandler);
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "_free_osfhnd (sub_825B77E8)" : "_get_osfhandle (sub_825B7880)");
    for (int c = 0; c < 48; c++) {
      const uint32_t pick = h.random.Below(8);
      const uint32_t handle = pick == 0   ? 0xFFFFFFFEu
                              : pick == 1 ? 0xFFFFFFFFu
                              : pick == 2 ? nhandle + h.random.Below(64)
                                          : h.random.Below(32);
      const uint32_t entry = handles + 44 * (handle & 31);
      const uint8_t flags = uint8_t(h.random.Next());
      const uint32_t os = h.random.Below(3) == 0 ? 0xFFFFFFFFu : uint32_t(h.random.Next());
      const uint32_t err = uint32_t(h.random.Next()), doserr = uint32_t(h.random.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Saved saved(h, {{entry, 44}, {slots.data + 8, 8}});
        guest::StoreU8(h.base, entry + 4, flags);
        Store(h.base, entry + 0, os);
        Store(h.base, slots.data + 8, err);
        Store(h.base, slots.data + 12, doserr);
        Store(h.base, cc::kInvalidParameterHandler, kReturnsZero);
        PPCContext t = CallContext(h);
        t.r3.u64 = handle;
        if (which == 0) {
          (v == 0 ? __imp__sub_825B77E8 : hooks::CoreFreeOsfhnd)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_825B7880 : hooks::CoreGetOsfhandle)(t, h.base);
        }
        Store(h.base, cc::kInvalidParameterHandler, handler);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Bytes(Read(h.base, entry, 8));
        out[v].Bytes(Read(h.base, slots.data + 8, 8));
        saved.Restore();
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
}

// exp: f1 edges (the overflow and underflow bounds, infinities, NaNs,
// denormals) and random doubles, small and any; compared: f1 and its scratch
// words below the stack pointer (-32 a word, or a double when scaled; -24 and
// -16 doubles), set to 0xCD first.
void ExpCases(Context& h) {
  Results results("exp (sub_825ABE30)");
  static const uint64_t kEdges[] = {
      0, 0x8000000000000000ull, 0x3FF0000000000000ull, 0xBFF0000000000000ull,
      0x7FF0000000000000ull, 0xFFF0000000000000ull, 0x7FF8000000000000ull,
      0xFFF8000000000123ull, 0x0000000000000001ull, 0x8010000000000000ull,
      0x40862E42FEFA39EFull, 0x40862E42FEFA39F0ull, 0x40862E42FEFA39EEull,
      0xC086232BDD7ABCD2ull, 0xC086232BDD7ABCD3ull, 0xC0874910D52D3051ull,
      0x4090000000000000ull, 0xC090000000000000ull, 0x3FE62E42FEFA39EFull,
      0x43E0000000000000ull, 0xC3E0000000000000ull, 0x41E0000000000000ull};
  constexpr int kEdgeCount = int(sizeof(kEdges) / sizeof(kEdges[0]));
  for (int c = 0; c < 400; c++) {
    uint64_t bits;
    if (c < kEdgeCount) {
      bits = kEdges[c];
    } else if (c % 3 == 0) {
      bits = h.random.Next();
    } else {
      const double x = (double(h.random.Below(1u << 30)) / double(1u << 30) - 0.5) *
                       (c % 3 == 1 ? 1500.0 : 8.0);
      bits = std::bit_cast<uint64_t>(x);
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      PPCContext t = CallContext(h);
      Fill(h.base, t.r1.u32 - 32, 24);
      t.f1.u64 = bits;
      (v == 0 ? __imp__sub_825ABE30 : hooks::CoreExp)(t, h.base);
      out[v].Double(t.f1.u64);
      out[v].Word(Load(h.base, t.r1.u32 - 32));
      out[v].Word(Load(h.base, t.r1.u32 - 28));
      out[v].Double(uint64_t(Load(h.base, t.r1.u32 - 24)) << 32 | Load(h.base, t.r1.u32 - 20));
      out[v].Double(uint64_t(Load(h.base, t.r1.u32 - 16)) << 32 | Load(h.base, t.r1.u32 - 12));
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// A number's text as the float conversions write it: an optional sign,
// digits, maybe a point and more digits (zeros often), maybe an exponent
// ('e' or 'E', a sign, digits), or a hex form ("0x" digits "p" exponent);
// sometimes a bare 'e' first or empty.
std::string NumberText(Context& h) {
  std::string text;
  const uint32_t kind = h.random.Below(10);
  if (kind == 0) {
    return h.random.Below(2) ? "" : "e+05";
  }
  if (h.random.Below(4) == 0) {
    text += '-';
  }
  if (kind == 1) {
    text += "0x1";
  }
  const uint32_t digits = 1 + h.random.Below(8);
  for (uint32_t i = 0; i < digits; i++) {
    text += char('0' + (h.random.Below(3) == 0 ? 0 : h.random.Below(10)));
  }
  if (h.random.Below(3) != 0) {
    text += '.';
    const uint32_t fraction = h.random.Below(8);
    for (uint32_t i = 0; i < fraction; i++) {
      text += char('0' + (h.random.Below(2) == 0 ? 0 : h.random.Below(10)));
    }
  }
  if (h.random.Below(2) != 0) {
    text += kind == 1 ? 'p' : (h.random.Below(2) ? 'e' : 'E');
    text += h.random.Below(2) ? '+' : '-';
    text += std::to_string(h.random.Below(400));
  }
  return text;
}

// _forcdecpt_l and _cropzeros_l: such texts in the scratch area (0xCD after
// them); compared: r3 and the 64 bytes.
void NumberTextCases(Context& h) {
  struct Fn {
    const char* name;
    GuestFunction recompiled, native;
    bool no_point;  // _forcdecpt_l's texts have no point yet
  };
  for (const Fn& fn : {Fn{"_forcdecpt_l (sub_825ADD38)", __imp__sub_825ADD38, hooks::CoreForcdecpt, true},
                       Fn{"_cropzeros_l (sub_825ADDD8)", __imp__sub_825ADDD8, hooks::CoreCropzeros, false}}) {
    Results results(fn.name);
    for (int c = 0; c < 300; c++) {
      std::string text = NumberText(h);
      if (fn.no_point) {
        // (an empty text would send it past its terminator: never given)
        std::erase(text, '.');
        if (text.empty()) {
          text = "0";
        }
      }
      const uint32_t at = h.data + 1;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, 64);
        WriteText(h, at, text);
        PPCContext t = CallContext(h);
        t.r3.u64 = at;
        (v == 0 ? fn.recompiled : fn.native)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Bytes(Read(h.base, h.data, 64));
      }
      results.Check(out[0], out[1], "case " + std::to_string(c) + " \"" + text + "\"");
    }
    results.Report();
  }
}

// _ld12tof: 12-byte long doubles around the float's range (exponents
// 16383 - 160 to 16383 + 140, so rounding, denormals, underflow and overflow
// come up), zeros and long double denormals (exponent 0), 32767 and random
// bits, with random extra bits; compared: r3 and 8 bytes at the output (the
// float, and the word after it, which it leaves alone).
void Ld12ToFloatCases(Context& h) {
  Results results("_ld12tof (sub_825B70D0)");
  const uint32_t from = h.data + 64, to = h.data + 96;
  for (int c = 0; c < 400; c++) {
    const uint32_t kind = h.random.Below(8);
    uint16_t sign_exponent;
    if (kind == 0) {
      sign_exponent = 0;
    } else if (kind == 1) {
      sign_exponent = 0x7FFF;
    } else if (kind == 2) {
      sign_exponent = uint16_t(h.random.Next());
    } else {
      sign_exponent = uint16_t(16383 - 160 + h.random.Below(300));
    }
    if (h.random.Below(2) != 0) {
      sign_exponent |= 0x8000;
    }
    uint32_t high = uint32_t(h.random.Next()), low = uint32_t(h.random.Next());
    uint16_t extra = uint16_t(h.random.Next());
    const uint32_t shape = h.random.Below(6);
    if (shape == 0) {
      high = low = 0;
      extra = 0;
    } else if (shape == 1) {
      high |= 0x80000000u;
      low &= 0xFFFFFF00u;  // near a float's halfway point
      high = (high & 0xFFFFFF00u) | (h.random.Below(2) ? 0x80 : 0x7F);
      extra = 0;
    } else if (shape < 5) {
      high |= 0x80000000u;  // normalized, as the parse leaves it
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU16(h.base, from + 0, sign_exponent);
      Store(h.base, from + 2, high);
      Store(h.base, from + 6, low);
      guest::StoreU16(h.base, from + 10, extra);
      Fill(h.base, to, 8);
      PPCContext t = CallContext(h);
      t.r3.u64 = from;
      t.r4.u64 = to;
      (v == 0 ? __imp__sub_825B70D0 : hooks::CoreLd12ToFloat)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Float(Load(h.base, to));
      out[v].Word(Load(h.base, to + 4));
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// _atodbl_l, _atoflt_l and _fassign_l: number strings (fixed edges: the
// float's and double's limits, under- and overflow, signs, spaces, junk,
// hex, nothing) and random ones; compared: r3 and 12 bytes at the output.
void AtoNumberCases(Context& h) {
  static const char* kTexts[] = {
      "0", "1", "-1", "1.5", "  12.25", "\t-0.0", "3.4028234e38", "3.4028236e38", "1e39",
      "-1e39", "1.17549435e-38", "1.4e-45", "7e-46", "1e-46", "1.7976931348623157e308",
      "1.8e308", "1e309", "4.9e-324", "2e-324", "1e-400", "abc", "", "+", ".", "1e", "1e+",
      "0x10", "123456789012345678901234567890", ".5e-3", "5.e2", "1d10", "-.0001E-2"};
  constexpr int kTextCount = int(sizeof(kTexts) / sizeof(kTexts[0]));
  struct Fn {
    const char* name;
    GuestFunction recompiled, native;
    int kind;  // 0 _atodbl_l, 1 _atoflt_l, 2 _fassign_l
  };
  const uint32_t at = h.data + 128, to = h.data + 64;
  for (const Fn& fn : {Fn{"_atodbl_l (sub_825B6098)", __imp__sub_825B6098, hooks::CoreAtodbl, 0},
                       Fn{"_atoflt_l (sub_825B6140)", __imp__sub_825B6140, hooks::CoreAtoflt, 1},
                       Fn{"_fassign_l (sub_825ADEB0)", __imp__sub_825ADEB0, hooks::CoreFassign, 2}}) {
    Results results(fn.name);
    for (int c = 0; c < 200; c++) {
      std::string text;
      if (c < kTextCount) {
        text = kTexts[c];
      } else {
        text = NumberText(h);
        std::replace(text.begin(), text.end(), 'x', '1');
        std::replace(text.begin(), text.end(), 'p', 'e');
      }
      const uint32_t is_double = h.random.Below(3) == 0 ? 0 : uint32_t(h.random.Next()) | 1;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, to, 12);
        WriteText(h, at, text);
        PPCContext t = CallContext(h);
        if (fn.kind == 2) {
          t.r3.u64 = c % 2 == 0 ? 0 : is_double;
          t.r4.u64 = to;
          t.r5.u64 = at;
          t.r6.u64 = uint32_t(h.random.Next());
        } else {
          t.r3.u64 = to;
          t.r4.u64 = at;
          t.r5.u64 = uint32_t(h.random.Next());
        }
        (v == 0 ? fn.recompiled : fn.native)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        const bool doubled = fn.kind == 0 || (fn.kind == 2 && c % 2 == 1);
        if (doubled) {
          out[v].Double(uint64_t(Load(h.base, to)) << 32 | Load(h.base, to + 4));
        } else {
          out[v].Float(Load(h.base, to));
          out[v].Word(Load(h.base, to + 4));
        }
        out[v].Word(Load(h.base, to + 8));
      }
      results.Check(out[0], out[1], "case " + std::to_string(c) + " \"" + text + "\"");
    }
    results.Report();
  }
}


// _fileno: a null stream or one in the scratch area with a random handle
// word (+16); errno random before, the invalid parameter handler the shared
// "returns 0"; compared: r3 (64 bits) and errno.
void FilenoCases(Context& h) {
  Results results("_fileno (sub_825B0E88)");
  ThreadSlots slots(h);
  if (slots.data == 0) {
    ITC_LOG_ERROR("harness: c_runtime_core: _fileno: no thread data");
    return;
  }
  const uint32_t handler = Load(h.base, cc::kInvalidParameterHandler);
  const uint32_t stream = h.data + 256;
  for (int c = 0; c < 24; c++) {
    const bool null = c % 3 == 0;
    const uint32_t word = uint32_t(h.random.Next()), err = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Saved saved(h, {{slots.data + 8, 4}});
      Store(h.base, stream + 16, word);
      Store(h.base, slots.data + 8, err);
      Store(h.base, cc::kInvalidParameterHandler, kReturnsZero);
      PPCContext t = CallContext(h);
      t.r3.u64 = null ? 0 : stream;
      (v == 0 ? __imp__sub_825B0E88 : hooks::CoreFileno)(t, h.base);
      Store(h.base, cc::kInvalidParameterHandler, handler);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(Load(h.base, slots.data + 8));
      saved.Restore();
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  results.Report();
}

// The __finally funclets of the stream and handle functions, each after the
// lock it gives back was taken: a handle's entry in the first block (its
// section made and entered for the case, put back after) for _close's,
// _write's and _commit's; for fclose's, a stream outside _iob (its own
// section at +32) or one of _iob's whose lock was made at boot; for doexit's,
// the exit lock (the host mutex, through _lock) when r28 isn't 0. Compared:
// r3, the section and the registers the funclets keep.
void StreamFinallyCases(Context& h) {
  struct Fn {
    const char* name;
    GuestFunction recompiled, native;
    int reg;  // 30 or 27: the handle's register
  };
  const uint32_t handles = Load(h.base, cc::kPioinfo);
  for (const Fn& fn : {Fn{"_close's __finally (sub_825B0E50)", __imp__sub_825B0E50,
                          hooks::CoreCloseFinally, 30},
                       Fn{"_write's __finally (sub_825B1304)", __imp__sub_825B1304,
                          hooks::CoreCloseFinally, 30},
                       Fn{"_commit's __finally (sub_825B4978)", __imp__sub_825B4978,
                          hooks::CoreCommitFinally, 27}}) {
    Results results(fn.name);
    for (int c = 0; c < 16 && handles != 0; c++) {
      const uint32_t handle = h.random.Below(32);
      const uint32_t entry = handles + 44 * handle;
      const int depth = 1 + int(h.random.Below(3));
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Saved saved(h, {{entry, 44}});
        InitSection(h, entry + 12);
        for (int d = 0; d < depth; d++) {
          PPCContext e = CallContext(h);
          e.r3.u64 = entry + 12;
          __imp__RtlEnterCriticalSection(e, h.base);
        }
        PPCContext t = CallContext(h);
        (fn.reg == 30 ? t.r30 : t.r27).u64 = handle;
        t.r31.u64 = 0x1234;
        t.r12.u64 = h.data + 640;
        (v == 0 ? fn.recompiled : fn.native)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(t.r27.u32);
        out[v].Word(t.r30.u32);
        out[v].Word(t.r31.u32);
        out[v].Bytes(Read(h.base, entry, 44));
        saved.Restore();
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
  {
    Results results("fclose's __finally (sub_825A9664)");
    std::vector<uint32_t> boot_streams;
    for (uint32_t i = 0; i < 20; i++) {
      if (BootLock(h, 16 + i)) {
        boot_streams.push_back(i);
      }
    }
    const uint32_t own = h.data + 256;
    for (int c = 0; c < 16; c++) {
      const bool in_iob = !boot_streams.empty() && c % 2 == 1;
      const uint32_t index = in_iob ? boot_streams[h.random.Below(uint32_t(boot_streams.size()))] : 0;
      const uint32_t stream = in_iob ? cc::kIob + 32 * index : own;
      const uint32_t section =
          in_iob ? Load(h.base, cc::kLockTable + 8 * (16 + index)) : own + 32;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        if (in_iob) {
          GuestLock(h, 16 + index, true);
        } else {
          InitSection(h, section);
          PPCContext e = CallContext(h);
          e.r3.u64 = section;
          __imp__RtlEnterCriticalSection(e, h.base);
        }
        PPCContext t = CallContext(h);
        t.r30.u64 = stream;
        t.r31.u64 = 0x5678;
        t.r12.u64 = h.data + 640;
        (v == 0 ? __imp__sub_825A9664 : hooks::CoreFcloseFinally)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(t.r30.u32);
        out[v].Word(t.r31.u32);
        out[v].Bytes(Section(h, section));
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
  {
    Results results("doexit's unwind funclet (sub_825AA338)");
    for (int c = 0; c < 16; c++) {
      const uint32_t locked = c % 2 == 0 ? 0 : uint32_t(h.random.Next()) | 1;
      const uint32_t r3 = uint32_t(h.random.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        if (locked != 0) {
          GuestLock(h, 8, true);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = r3;
        t.r28.u64 = locked;
        t.r31.u64 = 0x9ABC;
        t.r12.u64 = h.data + 640;
        (v == 0 ? __imp__sub_825AA338 : hooks::CoreExitUnlockFinally)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(t.r28.u32);
        out[v].Word(t.r31.u32);
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
}


// _close_nolock, _close and _commit: handles -2, -1, past _nhandle and in
// the first block, whose entry (its flags +4, the OS handle +0, the lock +8
// made with its section) is set for the case; the OS handle an event of the
// side's own (closed by the call), a word that's no handle (the close fails)
// or -1; handles 1 and 2 sometimes given the same OS handle with the other
// open. errno, _doserrno and the last error random before, the invalid
// parameter handler the shared "returns 0". Compared: r3 (64 bits), the
// entries (the OS handle only when the side's event isn't in it), errno,
// _doserrno, the last error and the handle's word in the caller's frame.
void CloseCases(Context& h) {
  const uint32_t handles = Load(h.base, cc::kPioinfo);
  ThreadSlots slots(h);
  if (handles == 0 || slots.data == 0 || !BootLock(h, 10)) {
    ITC_LOG_ERROR("harness: c_runtime_core: _close: no handle block, thread data or lock 10");
    return;
  }
  const uint32_t nhandle = Load(h.base, cc::kNhandle);
  const uint32_t handler = Load(h.base, cc::kInvalidParameterHandler);
  struct Fn {
    const char* name;
    GuestFunction recompiled, native;
    bool caller_frame;  // stores the handle at the caller's frame + 20
  };
  for (const Fn& fn : {Fn{"_close_nolock (sub_825B0C28)", __imp__sub_825B0C28,
                          hooks::CoreCloseNolock, false},
                       Fn{"_close (sub_825B0D10)", __imp__sub_825B0D10, hooks::CoreClose, true},
                       Fn{"_commit (sub_825B4828)", __imp__sub_825B4828, hooks::CoreCommit, true}}) {
    Results results(fn.name);
    const bool nolock = fn.native == hooks::CoreCloseNolock;
    for (int c = 0; c < 48; c++) {
      const uint32_t pick = h.random.Below(10);
      uint32_t handle = pick == 0   ? 0xFFFFFFFEu
                        : pick == 1 ? 0xFFFFFFFFu
                        : pick == 2 ? nhandle + h.random.Below(64)
                        : pick < 5  ? 1 + h.random.Below(2)
                                    : h.random.Below(32);
      if (nolock && (pick < 3)) {
        handle = h.random.Below(32);  // _close_nolock takes only handles in range
      }
      const uint32_t entry = handles + 44 * (handle & 31);
      const uint32_t other = handles + 44 * (handle == 1 ? 2 : 1);
      // The OS handle: 0 an event of the side's own, 1 a word that's no
      // handle, 2 -1.
      const uint32_t os_kind = h.random.Below(3);
      const uint32_t junk = 0x12340000u + h.random.Below(0x10000);
      const uint8_t flags = uint8_t(h.random.Next()) | (h.random.Below(4) != 0 ? 1 : 0);
      const bool pair = (handle == 1 || handle == 2) && h.random.Below(2) == 0;
      const bool made = h.random.Below(2) == 0;
      const uint32_t err = uint32_t(h.random.Next()), doserr = uint32_t(h.random.Next());
      const uint32_t last = uint32_t(h.random.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Saved saved(h, {{handles, 44 * 3}, {entry, 44}, {slots.data + 8, 8}});
        const uint32_t os = os_kind == 0 ? MakeEvent(h, 0, 0) : os_kind == 1 ? junk : 0xFFFFFFFFu;
        guest::StoreU8(h.base, entry + 4, flags);
        Store(h.base, entry + 0, os);
        Store(h.base, entry + 8, made ? 1 : 0);
        if (made) {
          InitSection(h, entry + 12);
        }
        if (pair) {
          guest::StoreU8(h.base, other + 4, 1);
          Store(h.base, other + 0, os);
        }
        Store(h.base, slots.data + 8, err);
        Store(h.base, slots.data + 12, doserr);
        SetLastError(h, last);
        Store(h.base, cc::kInvalidParameterHandler, kReturnsZero);
        PPCContext t = CallContext(h);
        Store(h.base, t.r1.u32 + 20, 0xCDCDCDCDu);
        t.r3.u64 = handle;
        (v == 0 ? fn.recompiled : fn.native)(t, h.base);
        Store(h.base, cc::kInvalidParameterHandler, handler);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        const bool own = os_kind == 0;
        const uint32_t os_after = Load(h.base, entry + 0);
        out[v].Word(own && os_after == os ? 0x0505u : os_after);
        out[v].Bytes(Read(h.base, entry + 4, 8));
        out[v].Bytes(Read(h.base, entry + 12, 28));
        if (pair) {
          const uint32_t other_os = Load(h.base, other + 0);
          out[v].Word(own && other_os == os ? 0x0505u : other_os);
          out[v].Word(guest::LoadU8(h.base, other + 4));
        }
        out[v].Bytes(Read(h.base, slots.data + 8, 8));
        out[v].Word(GuestLastError(h));
        if (fn.caller_frame) {
          out[v].Word(Load(h.base, t.r1.u32 + 20));
        }
        // An event the call didn't close is closed here.
        if (own && os != 0) {
          PPCContext k = CallContext(h);
          k.r3.u64 = os;
          __imp__NtClose(k, h.base);
        }
        saved.Restore();
      }
      results.Check(out[0], out[1], "case " + std::to_string(c));
    }
    results.Report();
  }
  SetLastError(h, slots.error);
}

// FlushFileBuffers: an event, 0 or a word that's no handle (the SDK answers
// every one with success); the last error random before; compared: r3 and
// the last error.
void FlushCases(Context& h) {
  Results results("FlushFileBuffers (sub_825BA510)");
  const uint32_t error = GuestLastError(h);
  for (int c = 0; c < 24; c++) {
    const int kind = c % 3;
    const uint32_t junk = 0x12340000u + h.random.Below(0x10000);
    const uint32_t last = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      SetLastError(h, last);
      const uint32_t event = kind == 0 ? MakeEvent(h, 0, 0) : kind == 1 ? 0 : junk;
      PPCContext t = CallContext(h);
      t.r3.u64 = event;
      (v == 0 ? __imp__sub_825BA510 : hooks::CoreFlushFileBuffers)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(GuestLastError(h));
      if (kind == 0 && event != 0) {
        PPCContext k = CallContext(h);
        k.r3.u64 = event;
        __imp__NtClose(k, h.base);
      }
    }
    results.Check(out[0], out[1], "case " + std::to_string(c));
  }
  SetLastError(h, error);
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
  IoInitCases(h);
  InittermECases(h);
  InitStdioCases(h);
  FfMsgBannerCases(h);
  LockFileCases(h);
  UnlockFhandleCases(h);
  AmsgExitCases(h);
  GetptdCases(h);
  InvalidParameterCases(h);
  UnhandledFilterCases(h);
  DebugMonitorCases(h);
  CxxRestoreFilterCases(h);
  LockFhandleCases(h);
  FinallyCases(h);
  OsHandleCases(h);
  ExpCases(h);
  NumberTextCases(h);
  Ld12ToFloatCases(h);
  AtoNumberCases(h);
  FilenoCases(h);
  StreamFinallyCases(h);
  CloseCases(h);
  FlushCases(h);
}

}  // namespace harness
