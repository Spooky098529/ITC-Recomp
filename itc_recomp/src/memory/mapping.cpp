// See mapping.h. Since E5 step 9 there is no guest mapping: the memory the
// game uses is three blocks of host memory of their own, and guest.h
// translates the guest's addresses into them in every load, store and
// guest::Pointer:
//
//   guest addresses                      block
//   0x00010000 + 384 MiB                 the heap (memory::g_heap: our
//                                        allocation layer's range, heap.h)
//   0x82000000 + 0x9B0000                the image (memory::g_image: its data
//                                        compiled in, image/image.h)
//   0x7F000000 (physical 0-16 MB),       the physical memory (memory::g_physical,
//   0xA0000000, 0xC0000000, 0xE0000000   since step 7; + 4 KB at 0xE0000000)
//   (+ 4 KB)
//
// The stacks, thread blocks, blobs and host objects are elsewhere in host
// memory, behind handles (memory/handles.h). `base` (still passed through
// the code, until step 10) is a 4 GB reservation nothing is mapped in: a
// raw `base + address` faults, named by the fault handler
// (memory/handles.cpp), as a null pointer does.

#include "memory/mapping.h"
#include "memory/handles.h"
#include "guest.h"

#include "base/log.h"

#include <sys/mman.h>

#include <cerrno>
#include <cstring>
#include <mutex>

namespace memory {

namespace {

constexpr uint64_t kGuestBytes = 0x100000000ull;
constexpr uint64_t kPhysicalBytes = 0x20000000ull;

uint8_t* base = nullptr;

// A block of host memory, read-write; its pages are only backed once
// touched (zeroed).
uint8_t* MapBlock(uint64_t bytes, const char* what) {
  void* block = mmap(nullptr, bytes, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
  if (block == MAP_FAILED) {
    ITC_LOG_ERROR("memory: mapping {} ({:#x} bytes) failed: {}", what, bytes,
                  std::strerror(errno));
    return nullptr;
  }
  return static_cast<uint8_t*>(block);
}

bool Map() {
  void* reserved = mmap(nullptr, kGuestBytes, PROT_NONE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
  if (reserved == MAP_FAILED) {
    ITC_LOG_ERROR("memory: reserving the fault range failed: {}", std::strerror(errno));
    return false;
  }
  g_heap = MapBlock(kHeapBytes, "the heap");
  g_image = MapBlock(kImageBytes, "the image");
  // The physical memory and its 4 KB past the end, which the 0xE0000000
  // addresses reach (they're offset by 4 KB).
  g_physical = MapBlock(kPhysicalBytes + 0x1000, "the physical memory");
  if (!g_heap || !g_image || !g_physical) {
    return false;
  }
  base = static_cast<uint8_t*>(reserved);
  ITC_LOG_INFO("memory: heap at {}, image at {}, physical at {}; fault range at {}",
               static_cast<void*>(g_heap), static_cast<void*>(g_image),
               static_cast<void*>(g_physical), static_cast<void*>(base));
  // The handles' table and the fault handler (docs/e5-native.md step 3;
  // handles.h).
  ProtectHandles(base);
  return true;
}

}  // namespace

bool MapGuest() {
  static std::once_flag once;
  static bool mapped = false;
  std::call_once(once, [] { mapped = Map(); });
  return mapped;
}

uint8_t* Base() {
  return base;
}

}  // namespace memory
