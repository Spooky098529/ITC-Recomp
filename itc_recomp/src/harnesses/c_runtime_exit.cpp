// The C runtime's exit table and exit lock (src/c_runtime/exit.cpp,
// docs/functions/c-runtime.md) against the recompiled functions.
//
// atexit works on the game's own table (begin 0x82978334, end 0x82978330):
// the harness holds the exit lock's host mutex around each group (a
// recursive mutex, so both sides take it again, the original through the
// _lock adapter), and puts the table back after each side. Three groups: in
// place on the game's table while its block has room (the end and the words
// written put back); on a copy of the game's table in a block of its own
// (the globals pointed at it, then back), with enough calls to grow it; and
// on made-up tables of other sizes and fills, with the failures (end below
// begin, the size overflowing). Grown tables sit at different addresses on
// each side: they're compared by their contents, size and the end's offset.
// Nothing here is a float: all exact.
//
// __onexitinit (the table's set-up at boot): on the game's globals, saved
// and put back after each side; each side's new table compared by contents
// and size, then freed.
//
// _lock and _unlock: each case takes and gives back one lock (8, and other
// numbers made at boot) through the original or the adapter's steps; held
// in between (lock 8's host mutex: another thread can't take it; others:
// the critical section's recursion count and owner set), r3 after each, and
// the lock's critical section and the lock table after.

#include "harness.h"

#include "base/log.h"
#include "c_runtime/exit.h"
#include "hooks/c_runtime.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"

#include <fmt/format.h>

#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace harness {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// atexit through the adapter's steps, without its flag.
void NativeAtexit(PPCContext& ctx, uint8_t* base) {
  if (const auto result = c_runtime::Atexit(base, ctx.r3.u32)) {
    ctx.r3.s64 = *result;
    return;
  }
  __imp__sub_825A7068(ctx, base);
}

// Calls atexit with each function on one side; each r3's 64 bits kept.
void Calls(int v, Context& h, const std::vector<uint32_t>& functions, Outputs& out) {
  for (uint32_t function : functions) {
    PPCContext t = CallContext(h);
    t.r3.u64 = function;
    (v == 0 ? __imp__sub_825A7068 : NativeAtexit)(t, h.base);
    out.Word(t.r3.u32);
    out.Word(uint32_t(t.r3.u64 >> 32));
  }
}

// The table as it's left: the end's offset from begin, its block's size
// (when begin is a block) and the words from begin to end.
void TakeTable(uint8_t* base, Outputs& out, bool block) {
  const uint32_t begin = Load(base, c_runtime::kExitTableBegin);
  const uint32_t end = Load(base, c_runtime::kExitTableEnd);
  out.Word(end - begin);
  if (block) {
    out.Word(memory::crt::Msize(begin).value_or(0xFFFFFFFF));
    out.Bytes(Read(base, begin, end - begin));
  }
}

std::vector<uint32_t> Functions(Context& h, uint32_t count) {
  std::vector<uint32_t> functions;
  for (uint32_t i = 0; i < count; i++) {
    // Mostly addresses like the clean-ups', some 0 (atexit takes it).
    functions.push_back(h.random.Below(8) == 0 ? 0 : 0x825D0000 + 4 * h.random.Below(0x4000));
  }
  return functions;
}

void InPlace(Context& h) {
  Results results("atexit, _onexit (sub_825A7068, sub_825A6F60, in place on the game's table)");
  const uint32_t begin = Load(h.base, c_runtime::kExitTableBegin);
  const uint32_t end = Load(h.base, c_runtime::kExitTableEnd);
  const uint32_t size = memory::crt::Msize(begin).value_or(0);
  const uint32_t room = size > end - begin ? (size - (end - begin)) / 4 : 0;
  ITC_LOG_INFO("harness: c_runtime_exit: the game's table {:08X}-{:08X}, block {} bytes, room "
               "for {}",
               begin, end, size, room);
  for (int c = 0; c < 24 && room > 0; c++) {
    const auto functions = Functions(h, 1 + h.random.Below(std::min(room, 4u)));
    const auto words = Read(h.base, end, 4 * uint32_t(functions.size()));
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Calls(v, h, functions, out[v]);
      out[v].Word(Load(h.base, c_runtime::kExitTableBegin));
      TakeTable(h.base, out[v], true);
      Store(h.base, c_runtime::kExitTableBegin, begin);
      Store(h.base, c_runtime::kExitTableEnd, end);
      Write(h.base, end, words.data(), uint32_t(words.size()));
    }
    results.Check(out[0], out[1], fmt::format("case {} ({} calls)", c, functions.size()));
  }
  results.Report();
}

// A table of `size` bytes with `used` bytes filled from `fill` (random
// words past it), made for one side; the globals pointed at it.
uint32_t MakeTable(uint8_t* base, uint32_t size, uint32_t used,
                   const std::vector<uint8_t>& fill) {
  const uint32_t table = memory::crt::Malloc(base, size).value_or(0);
  if (table != 0) {
    Write(base, table, fill.data(), size);
    Store(base, c_runtime::kExitTableBegin, table);
    Store(base, c_runtime::kExitTableEnd, table + used);
  }
  return table;
}

void Copies(Context& h, Results& results, const char* kind, int cases, uint32_t size,
            uint32_t used, const std::vector<uint8_t>& fill, uint32_t calls) {
  (void)cases;
  const auto functions = Functions(h, calls);
  Outputs out[2];
  bool made = true;
  for (int v = 0; v < 2; v++) {
    if (MakeTable(h.base, size, used, fill) == 0) {
      made = false;
      break;
    }
    Calls(v, h, functions, out[v]);
    TakeTable(h.base, out[v], true);
    memory::crt::Free(Load(h.base, c_runtime::kExitTableBegin));
  }
  if (!made) {
    ITC_LOG_WARN("harness: c_runtime_exit: a {}-byte table not allocated", size);
    return;
  }
  results.Check(out[0], out[1],
                fmt::format("{} ({} bytes, {} used, {} calls)", kind, size, used, calls));
}

void OnCopies(Context& h) {
  Results results("atexit, _onexit (sub_825A7068, sub_825A6F60, a copy of the game's table)");
  const uint32_t begin = Load(h.base, c_runtime::kExitTableBegin);
  const uint32_t end = Load(h.base, c_runtime::kExitTableEnd);
  const uint32_t size = memory::crt::Msize(begin).value_or(0);
  if (size == 0) {
    ITC_LOG_WARN("harness: c_runtime_exit: the game's table has no block: its copies skipped");
    return;
  }
  const auto fill = Read(h.base, begin, size);
  const uint32_t room = size > end - begin ? (size - (end - begin)) / 4 : 0;
  for (int c = 0; c < 12; c++) {
    // Up to the room, and past it to grow the table (once or twice).
    const uint32_t calls = 1 + h.random.Below(room + size / 4 + 8);
    Copies(h, results, "copy", c, size, end - begin, fill, calls);
  }
  Store(h.base, c_runtime::kExitTableBegin, begin);
  Store(h.base, c_runtime::kExitTableEnd, end);
  results.Report();
}

void MadeUp(Context& h) {
  Results results("atexit, _onexit (sub_825A7068, sub_825A6F60, made-up tables)");
  const uint32_t begin = Load(h.base, c_runtime::kExitTableBegin);
  const uint32_t end = Load(h.base, c_runtime::kExitTableEnd);
  static const uint32_t kSizes[] = {4, 8, 12, 16, 40, 128, 512, 1000, 2044, 2048, 2052, 4096,
                                    6000};
  for (int c = 0; c < 40; c++) {
    const uint32_t size = kSizes[h.random.Below(std::size(kSizes))];
    // Full, nearly full, empty, a random fill, or an end off a word.
    uint32_t used = 0;
    switch (c % 5) {
      case 0: used = size & ~3u; break;
      case 1: used = size >= 4 ? (size & ~3u) - 4 : 0; break;
      case 2: used = 0; break;
      case 3: used = 4 * h.random.Below(size / 4 + 1); break;
      default: used = size >= 4 ? 4 * h.random.Below(size / 4) + 1 + h.random.Below(3) : 0; break;
    }
    if (used > size) {
      used = size;
    }
    std::vector<uint8_t> fill(size);
    for (auto& byte : fill) {
      byte = uint8_t(h.random.Next());
    }
    Copies(h, results, "made-up", c, size, used, fill, 1 + h.random.Below(4));
  }
  // The failures: end below begin, and end - begin + 4 overflowing (no
  // memory touched).
  struct Failure {
    uint32_t begin, end;
  };
  static const Failure kFailures[] = {
      {0x82978400, 0x829783F0}, {0x10, 0x0}, {0x0, 0xFFFFFFFD}, {0x0, 0xFFFFFFFC},
      {0x0, 0xFFFFFFFF}};
  int f = 0;
  for (const Failure& failure : kFailures) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Store(h.base, c_runtime::kExitTableBegin, failure.begin);
      Store(h.base, c_runtime::kExitTableEnd, failure.end);
      Calls(v, h, {0x825DB3F0}, out[v]);
      out[v].Word(Load(h.base, c_runtime::kExitTableBegin));
      out[v].Word(Load(h.base, c_runtime::kExitTableEnd));
    }
    results.Check(out[0], out[1],
                  fmt::format("failure {} ({:08X}, {:08X})", f++, failure.begin, failure.end));
  }
  Store(h.base, c_runtime::kExitTableBegin, begin);
  Store(h.base, c_runtime::kExitTableEnd, end);
  results.Report();
}

// --- _lock and _unlock ---

void NativeLock(PPCContext& ctx, uint8_t* base) {
  if (ctx.r3.u32 == c_runtime::kExitLock) {
    ctx.r3.u64 = c_runtime::LockExit(base);
    return;
  }
  __imp__sub_825B3430(ctx, base);
}
void NativeUnlock(PPCContext& ctx, uint8_t* base) {
  if (ctx.r3.u32 == c_runtime::kExitLock) {
    ctx.r3.u64 = c_runtime::UnlockExit(base);
    return;
  }
  __imp__sub_825B32D0(ctx, base);
}

// Whether the lock is held: the host mutex (another thread can't take it)
// for lock 8 on the native side, else its critical section's recursion
// count (+20) and owner (+24).
uint32_t Held(uint8_t* base, int v, uint32_t number, uint32_t critical_section) {
  if (v == 1 && number == c_runtime::kExitLock) {
    bool taken = false;
    std::thread other([&] {
      taken = c_runtime::ExitMutex().try_lock();
      if (taken) {
        c_runtime::ExitMutex().unlock();
      }
    });
    other.join();
    return taken ? 0 : 1;
  }
  return Load(base, critical_section + 20) != 0 && Load(base, critical_section + 24) != 0 ? 1
                                                                                          : 0;
}

void Locks(Context& h) {
  Results results("_lock, _unlock (sub_825B3430, sub_825B32D0)");
  const uint32_t entry = c_runtime::kLockTable + 8 * c_runtime::kExitLock;
  ITC_LOG_INFO("harness: c_runtime_exit: lock 8's entry: critical section {:08X}, kind {}",
               Load(h.base, entry), Load(h.base, entry + 4));
  // Lock 8, and other numbers the game takes (made at boot: a null entry
  // would be made by sub_825B32F0, never run in play).
  static const uint32_t kNumbers[] = {8, 0, 1, 10, 14};
  int c = 0;
  for (uint32_t number : kNumbers) {
    const uint32_t critical_section = Load(h.base, c_runtime::kLockTable + 8 * number);
    if (critical_section == 0) {
      ITC_LOG_WARN("harness: c_runtime_exit: lock {} not made: its cases skipped", number);
      continue;
    }
    for (int depth = 1; depth <= 2; depth++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (int d = 0; d < depth; d++) {
          PPCContext t = CallContext(h);
          t.r3.u64 = number;
          (v == 0 ? __imp__sub_825B3430 : NativeLock)(t, h.base);
          out[v].Word(t.r3.u32);
          out[v].Word(uint32_t(t.r3.u64 >> 32));
          out[v].Word(Held(h.base, v, number, critical_section));
        }
        for (int d = 0; d < depth; d++) {
          PPCContext t = CallContext(h);
          t.r3.u64 = number;
          (v == 0 ? __imp__sub_825B32D0 : NativeUnlock)(t, h.base);
          out[v].Word(t.r3.u32);
          out[v].Word(uint32_t(t.r3.u64 >> 32));
          out[v].Word(Held(h.base, v, number, critical_section));
        }
        out[v].Bytes(Read(h.base, critical_section, 28));
        out[v].Bytes(Read(h.base, c_runtime::kLockTable, 288));
      }
      results.Check(out[0], out[1], fmt::format("case {} (lock {}, depth {})", c++, number, depth));
    }
  }
  results.Report();
}

// __onexitinit through the adapter's steps, without its flags.
void NativeInitExitTable(PPCContext& ctx, uint8_t* base) {
  if (const auto result = c_runtime::InitExitTable(base)) {
    ctx.r3.u64 = *result;
    return;
  }
  __imp__sub_825A6F00(ctx, base);
}

// __onexitinit on the game's globals, saved and put back after each side
// (begin and end set before it to the game's, or made-up values, which it
// overwrites). Each side's new table is compared by the end's offset, its
// block's size and its first 128 bytes (calloc's, so cleared), then freed.
void InitCases(Context& h) {
  Results results("__onexitinit (sub_825A6F00, on the game's globals, saved and put back)");
  const uint32_t saved_begin = Load(h.base, c_runtime::kExitTableBegin);
  const uint32_t saved_end = Load(h.base, c_runtime::kExitTableEnd);
  for (int c = 0; c < 8; c++) {
    const uint32_t begin = c < 2 ? saved_begin : uint32_t(h.random.Next());
    const uint32_t end = c < 2 ? saved_end : uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Store(h.base, c_runtime::kExitTableBegin, begin);
      Store(h.base, c_runtime::kExitTableEnd, end);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.random.Next();  // not read
      (v == 0 ? __imp__sub_825A6F00 : NativeInitExitTable)(t, h.base);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      const uint32_t table = Load(h.base, c_runtime::kExitTableBegin);
      out[v].Word(Load(h.base, c_runtime::kExitTableEnd) - table);
      out[v].Word(table == 0 ? 0 : 1);
      if (table != 0) {
        out[v].Word(memory::crt::Msize(table).value_or(0xFFFFFFFF));
        out[v].Bytes(Read(h.base, table, 128));
        memory::crt::Free(table);
      }
      Store(h.base, c_runtime::kExitTableBegin, saved_begin);
      Store(h.base, c_runtime::kExitTableEnd, saved_end);
    }
    results.Check(out[0], out[1], fmt::format("case {} (begin {:08X}, end {:08X})", c, begin, end));
  }
  results.Report();
}

}  // namespace

void CRuntimeExit(Context& h) {
  if (!hooks::CRuntimeExitHostLock()) {
    ITC_LOG_WARN("harness: c_runtime_exit: --native_c_runtime was off at start-up: skipped (lock "
                 "8 is the game's critical section)");
    return;
  }
  Locks(h);
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_WARN("harness: c_runtime_exit: --native_heap off: atexit's cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  InPlace(h);
  OnCopies(h);
  MadeUp(h);
  InitCases(h);
}

}  // namespace harness
