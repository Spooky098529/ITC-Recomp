// The lift sweep's runtime: see lifted.h and sdk.h.

#include "base/float_mode.h"
#include "lifted/lifted.h"
#include "lifted/sdk.h"

#include "base/log.h"
#include "imports/native_imports.h"

#include <cstdlib>
#include <cstring>
#include <string_view>
#include <mutex>
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

// The lifted originals (lifted/originals.cpp), for an address with no entry
// of its own: a function with plain C++ but no adapter, reached by address
// from lifted code (docs/e5-native.md, step 0).
std::unordered_map<uint32_t, Entry>& Fallbacks() {
  static std::unordered_map<uint32_t, Entry> fallbacks;
  return fallbacks;
}

}  // namespace

void ScalarFloatMode() {
  const uint32_t csr = ::base::FloatMode::getcsr();
  if ((csr & ::base::FloatMode::FlushMask) != 0) [[unlikely]] {
    ::base::FloatMode::setcsr(csr & ~uint32_t(::base::FloatMode::FlushMask));
  }
}

void VectorFloatMode() {
  const uint32_t csr = ::base::FloatMode::getcsr();
  if ((csr & ::base::FloatMode::FlushMask) != ::base::FloatMode::FlushMask) [[unlikely]] {
    ::base::FloatMode::setcsr(csr | uint32_t(::base::FloatMode::FlushMask));
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
  if (const auto it = table.find(address); it != table.end()) {
    return it->second.entry;
  }
  const auto& fallbacks = Fallbacks();
  if (const auto it = fallbacks.find(address); it != fallbacks.end()) {
    return it->second;
  }
  return nullptr;
}

void RegisterFallbacks(const Registration* entries, size_t count) {
  auto& fallbacks = Fallbacks();
  for (size_t i = 0; i < count; i++) {
    fallbacks[entries[i].address] = entries[i].entry;
  }
}

void NoOriginal(uint32_t address) {
  ITC_LOG_ERROR("lifted: {:08X}'s original was asked for (a flag off or a case the port "
                "declines), and there's none: the recompiled code is gone and it couldn't be "
                "lifted",
                address);
  std::abort();
}

// A call to an address with no entry in the dispatch table: no function of
// ours (the rewiring: every function the game runs has one, and nothing is
// recompiled since stage 5): logged, and the program stops.
[[noreturn]] void Unknown(uint32_t address) {
  ITC_LOG_ERROR("lifted: a call to {:08X}, which has no entry in the dispatch table", address);
  std::abort();
}

Result Call(uint8_t* base, uint32_t address, const Args& args) {
  const auto& table = Table();
  if (const auto it = table.find(address); it != table.end()) {
    return it->second.entry(base, args);
  }
  const auto& fallbacks = Fallbacks();
  if (const auto it = fallbacks.find(address); it != fallbacks.end()) {
    return it->second(base, args);
  }
  Unknown(address);
}

// An SDK import with no answer of ours: the SDK's runtime isn't made since
// E2 (docs/rexglue-removal.md, "E2: the plan", step 5), so its imports have
// no kernel to run on: logged by name, and the program stops (as a call to
// an address with no entry). None is called in play (--log_imports).
[[noreturn]] void NoAnswer(ImportFunction* function, uint32_t lr) {
  ITC_LOG_ERROR("lifted: the import {} (called from {:08X}) has no answer of ours, and the "
                "SDK's runtime isn't made",
                ImportName(function), lr);
  std::abort();
}

Result Import(uint8_t* base, ImportFunction* function, const Args& args) {
  if (LoggingImports()) {
    NoteImport(function, args);
  }
  if (const imports::NativeImport native = imports::FindNativeImport(function)) {
    return native(base, args);
  }
  NoAnswer(function, args.lr);
}

Result ImportFromSdk(uint8_t*, ImportFunction* function, const Args& args) {
  NoAnswer(function, args.lr);
}

bool RegisterEntry(uint32_t address, Entry entry, const bool* enabled) {
  auto& table = Table();
  if (table.contains(address)) {
    ITC_LOG_ERROR("lifted: {:08X} registered twice", address);
    return false;
  }
  table[address] = Target{entry, enabled};
  return true;
}

void Sdk(Regs& regs, uint8_t* base, ImportFunction* function) {
  regs.Apply(Import(base, function, regs.ToArgs()));
}

}  // namespace lifted
