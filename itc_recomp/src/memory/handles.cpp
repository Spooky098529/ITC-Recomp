#include "memory/handles.h"

#include "base/log.h"

#include <algorithm>
#include <atomic>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <vector>

#include <sys/mman.h>
#include <unistd.h>

namespace memory {
namespace {

constexpr uint32_t kGranules = (kHandlesEnd - kHandlesStart) / kHandleGranule;

// A granule's entry: the object whose handle span covers it.
struct Entry {
  void* object = nullptr;
  uint32_t start = 0;  // the object's handle
  uint32_t size = 0;
  const char* type = nullptr;
  const Layout* layout = nullptr;
};

Entry* g_entries = nullptr;            // kGranules, allocated once
std::atomic<uint8_t*> g_base{nullptr};
std::mutex g_mutex;
uint32_t g_next = 0;                   // the first granule never handed out
// Freed spans by their granule count (reused first fit by count).
std::vector<std::pair<uint32_t, uint32_t>> g_free;  // (first granule, count)
struct sigaction g_previous {};

uint32_t Granule(uint32_t handle) { return (handle - kHandlesStart) / kHandleGranule; }

// Async-signal-safe enough for a last message: snprintf into a stack
// buffer, write(2) to stderr, then the log's own line.
void OnFault(int sig, siginfo_t* info, void* context) {
  uint8_t* base = g_base.load(std::memory_order_relaxed);
  const uintptr_t at = reinterpret_cast<uintptr_t>(info->si_addr);
  const uintptr_t lo = reinterpret_cast<uintptr_t>(base) + kHandlesStart;
  const uintptr_t hi = reinterpret_cast<uintptr_t>(base) + kHandlesEnd;
  if (base && at >= lo && at < hi) {
    const uint32_t guest = uint32_t(at - reinterpret_cast<uintptr_t>(base));
    const Entry* e = g_entries ? &g_entries[Granule(guest)] : nullptr;
    char text[256];
    if (e && e->object) {
      std::snprintf(text, sizeof(text),
                    "handle fault: a raw access at %08X, +%u into a %s (handle %08X, %u bytes): "
                    "code reaching a host object through guest memory\n",
                    guest, guest - e->start, e->type, e->start, e->size);
    } else {
      std::snprintf(text, sizeof(text), "handle fault: a raw access at %08X (no live handle)\n",
                    guest);
    }
    ITC_LOG_ERROR("{}", text);
    ssize_t ignored = write(STDERR_FILENO, text, std::strlen(text));
    (void)ignored;
    std::_Exit(3);
  }
  // Not ours: as before.
  if (g_previous.sa_flags & SA_SIGINFO) {
    if (g_previous.sa_sigaction) {
      g_previous.sa_sigaction(sig, info, context);
      return;
    }
  } else if (g_previous.sa_handler != SIG_DFL && g_previous.sa_handler != SIG_IGN) {
    g_previous.sa_handler(sig);
    return;
  }
  signal(sig, SIG_DFL);
  raise(sig);
}

}  // namespace

bool ProtectHandles(uint8_t* base) {
  g_entries = new Entry[kGranules]();
  if (mprotect(base + kHandlesStart, kHandlesEnd - kHandlesStart, PROT_NONE) != 0) {
    ITC_LOG_ERROR("handles: can't protect {:#x} to {:#x}", kHandlesStart, kHandlesEnd);
    return false;
  }
  struct sigaction action {};
  action.sa_sigaction = OnFault;
  action.sa_flags = SA_SIGINFO | SA_ONSTACK;
  sigemptyset(&action.sa_mask);
  sigaction(SIGSEGV, &action, &g_previous);
  g_base.store(base, std::memory_order_relaxed);
  ITC_LOG_INFO("handles: {:#x} to {:#x}, inaccessible", kHandlesStart, kHandlesEnd);
  return true;
}

uint32_t MakeHandle(void* object, uint32_t size, const char* type, const Layout* layout) {
  const uint32_t count = std::max<uint32_t>(1, (size + kHandleGranule - 1) / kHandleGranule);
  std::lock_guard lock(g_mutex);
  uint32_t first = UINT32_MAX;
  for (auto it = g_free.begin(); it != g_free.end(); ++it) {
    if (it->second == count) {
      first = it->first;
      g_free.erase(it);
      break;
    }
  }
  if (first == UINT32_MAX) {
    if (g_next + count > kGranules) {
      ITC_LOG_ERROR("handles: the range is full ({} for a {})", size, type);
      std::abort();
    }
    first = g_next;
    g_next += count;
  }
  const uint32_t handle = kHandlesStart + first * kHandleGranule;
  for (uint32_t i = 0; i < count; i++) {
    g_entries[first + i] = {object, handle, size, type, layout};
  }
  return handle;
}

void ReleaseHandle(uint32_t handle) {
  if (handle == 0) {
    return;
  }
  std::lock_guard lock(g_mutex);
  if (!IsHandle(handle) || g_entries[Granule(handle)].start != handle) {
    ITC_LOG_ERROR("handles: release of {:08X}, not a live handle", handle);
    return;
  }
  const Entry e = g_entries[Granule(handle)];
  const uint32_t first = Granule(handle);
  const uint32_t count = std::max<uint32_t>(1, (e.size + kHandleGranule - 1) / kHandleGranule);
  for (uint32_t i = 0; i < count; i++) {
    g_entries[first + i] = {};
  }
  g_free.emplace_back(first, count);
}

bool FreeHandle(uint32_t handle) {
  void* object = nullptr;
  {
    std::lock_guard lock(g_mutex);
    if (!IsHandle(handle) || !g_entries || !g_entries[Granule(handle)].object ||
        g_entries[Granule(handle)].start != handle) {
      ITC_LOG_ERROR("handles: a free of {:08X}, not a live handle's start", handle);
      return false;
    }
    object = g_entries[Granule(handle)].object;
  }
  ReleaseHandle(handle);
  // The schema's host structs are plain data made with `new T()`.
  ::operator delete(object);
  return true;
}

bool DescribeHandle(uint32_t value, HandleInfo* info) {
  if (!IsHandle(value) || !g_entries) {
    return false;
  }
  const Entry& e = g_entries[Granule(value)];
  if (!e.object || value - e.start >= std::max<uint32_t>(e.size, 1)) {
    return false;
  }
  *info = {e.object, value - e.start, e.size, e.type, e.layout};
  return true;
}

void* HandleObject(uint32_t handle) {
  if (handle == 0) {
    return nullptr;
  }
  if (IsHandle(handle) && g_entries) {
    // A value inside the object's span names the member there: host
    // structs keep the guest offsets (tools/schema_gen.py), so an embedded
    // object's or an interface's address is the owner's handle + offset.
    const Entry& e = g_entries[Granule(handle)];
    if (e.object && handle - e.start < std::max<uint32_t>(e.size, 1)) {
      return static_cast<uint8_t*>(e.object) + (handle - e.start);
    }
  }
  static std::atomic<bool> logged{false};
  if (!logged.exchange(true)) {
    ITC_LOG_ERROR("handles: {:08X} isn't a live handle (a guest address where a host object "
                  "was expected?)",
                  handle);
  }
  return nullptr;
}

}  // namespace memory
