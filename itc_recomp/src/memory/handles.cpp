#include "memory/handles.h"

#include "base/log.h"
#include "memory/host_copy.h"
#include "guest.h"

#include <algorithm>
#include <atomic>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <vector>


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
  uint32_t element = 0;  // an array's element size; 0 for one object
};

Entry* g_entries = nullptr;            // kGranules, allocated once
std::mutex g_mutex;
uint32_t g_next = 0;                   // the first granule never handed out
// Freed spans by their granule count (reused first fit by count).
std::vector<std::pair<uint32_t, uint32_t>> g_free;  // (first granule, count)

uint32_t Granule(uint32_t handle) { return (handle - kHandlesStart) / kHandleGranule; }

}  // namespace

bool SetUpHandles() {
  g_entries = new Entry[kGranules]();
  ITC_LOG_INFO("handles: {:#x} to {:#x}", kHandlesStart, kHandlesEnd);
  return true;
}

uint32_t MakeHandle(void* object, uint32_t size, const char* type, const Layout* layout,
                    uint32_t element) {
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
    g_entries[first + i] = {object, handle, size, type, layout, element};
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
  *info = {e.object, value - e.start, e.size, e.type, e.layout,
           e.element ? e.element : std::max<uint32_t>(e.size, 1)};
  return true;
}

namespace {
const Layout kBlobLayout = {nullptr, 0};
}  // namespace

bool IsBlobLayout(const Layout* layout) { return layout == &kBlobLayout; }

uint32_t NewBlob(uint32_t size, const char* type) {
  // Whole 4 KB pages, as VirtualAlloc gives them: the game reads past what it
  // asked for in places (a menu's flags read by index past its count), which
  // the page's slack covered on the 360. A handle's span is pages anyway.
  const uint32_t rounded = std::max<uint32_t>(1, (size + kHandleGranule - 1) / kHandleGranule) *
                           kHandleGranule;
  void* bytes = ::operator new(rounded);
  std::memset(bytes, 0, rounded);
  return MakeHandle(bytes, rounded, type, &kBlobLayout);
}

uint8_t* BlobPointer(uint32_t value, uint32_t* left) {
  if (!IsHandle(value) || !g_entries) {
    return nullptr;
  }
  const Entry& e = g_entries[Granule(value)];
  const uint32_t offset = value - e.start;
  if (!e.object || e.layout != &kBlobLayout || offset > e.size) {
    return nullptr;
  }
  if (left) {
    *left = e.size - offset;
  }
  return static_cast<uint8_t*>(e.object) + offset;
}

void* HandleOwn(uint32_t value, const Layout* layout) {
  if (!IsHandle(value) || !g_entries) {
    return nullptr;
  }
  const Entry& e = g_entries[Granule(value)];
  const uint32_t offset = value - e.start;
  if (!e.object || e.layout != layout || offset >= std::max<uint32_t>(e.size, 1)) {
    return nullptr;
  }
  if (offset % (e.element ? e.element : std::max<uint32_t>(e.size, 1)) != 0) {
    return nullptr;
  }
  return static_cast<uint8_t*>(e.object) + offset;
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
