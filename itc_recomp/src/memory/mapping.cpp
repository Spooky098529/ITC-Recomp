// See mapping.h. Since E5 step 9 there is no guest mapping, and since step
// 10 no `base`: the memory the game uses is three blocks of host memory of
// their own, and guest.h translates the guest's addresses into them in every
// load, store and guest::Pointer:
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
// memory, behind handles (memory/handles.h). Any other address (a null
// pointer's, below 0x10000) stops the program, named (NoBlock).

#include "memory/mapping.h"
#include "memory/handles.h"
#include "guest.h"

#include "base/log.h"

#include <sys/mman.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <format>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>

namespace memory {

namespace {

constexpr uint64_t kPhysicalBytes = 0x20000000ull;

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
  g_heap = MapBlock(kHeapBytes, "the heap");
  g_image = MapBlock(kImageBytes, "the image");
  // The physical memory and its 4 KB past the end, which the 0xE0000000
  // addresses reach (they're offset by 4 KB).
  g_physical = MapBlock(kPhysicalBytes + 0x1000, "the physical memory");
  if (!g_heap || !g_image || !g_physical) {
    return false;
  }
  ITC_LOG_INFO("memory: heap at {}, image at {}, physical at {}",
               static_cast<void*>(g_heap), static_cast<void*>(g_image),
               static_cast<void*>(g_physical));
  // The handles' table (docs/e5-native.md step 3; handles.h).
  SetUpHandles();
  return true;
}

}  // namespace

bool MapGuest() {
  static std::once_flag once;
  static bool mapped = false;
  std::call_once(once, [] { mapped = Map(); });
  return mapped;
}

void NoBlock(uint32_t address) {
  ITC_LOG_ERROR("memory: an access at {:08X}, in no block ({}): stopping", address,
                address < 0x10000 ? "a null pointer" : "not the heap, the image, the physical memory "
                                                       "or a handle's object");
  std::abort();
}

namespace {

// Bytes of [block, block + bytes) backed by pages now.
uint64_t ResidentBytes(const uint8_t* block, uint64_t bytes) {
  if (!block) {
    return 0;
  }
  const uint64_t page = uint64_t(sysconf(_SC_PAGESIZE));
  std::vector<unsigned char> pages((bytes + page - 1) / page);
  if (mincore(const_cast<uint8_t*>(block), bytes, pages.data()) != 0) {
    return 0;
  }
  uint64_t resident = 0;
  for (unsigned char p : pages) {
    resident += p & 1;
  }
  return resident * page;
}

// A "Name:   1234 kB" line of /proc/self/status, in bytes (0 if missing).
uint64_t StatusBytes(const std::string& status, const char* name) {
  size_t at = status.find(name);
  if (at == std::string::npos) {
    return 0;
  }
  return std::strtoull(status.c_str() + at + std::strlen(name), nullptr, 10) * 1024;
}

}  // namespace

// The GPU driver's own counters (amdgpu's sysfs files; empty elsewhere):
// VRAM and GTT (system memory the driver pins for the GPU) in use, by every
// process on the card, not this one alone.
std::string DriverMemory() {
  constexpr double kMB = 1048576.0;
  for (int card = 0; card < 8; card++) {
    std::string dir = std::format("/sys/class/drm/card{}/device/", card);
    std::ifstream vram(dir + "mem_info_vram_used");
    std::ifstream gtt(dir + "mem_info_gtt_used");
    uint64_t vram_bytes = 0;
    uint64_t gtt_bytes = 0;
    if (vram >> vram_bytes && gtt >> gtt_bytes) {
      return std::format("; driver (whole card{}) VRAM {:.1f} MB, GTT {:.1f} MB", card,
                         vram_bytes / kMB, gtt_bytes / kMB);
    }
  }
  return {};
}

std::string DescribeResident() {
  constexpr double kMB = 1048576.0;
  std::ifstream file("/proc/self/status");
  std::string status((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return std::format(
      "heap {:.1f} MB; image {:.1f} MB; physical {:.1f} MB; process {:.1f} MB (anon {:.1f}, "
      "file {:.1f}, shmem {:.1f}){}",
      ResidentBytes(g_heap, kHeapBytes) / kMB, ResidentBytes(g_image, kImageBytes) / kMB,
      ResidentBytes(g_physical, kPhysicalBytes + 0x1000) / kMB,
      StatusBytes(status, "VmRSS:") / kMB, StatusBytes(status, "RssAnon:") / kMB,
      StatusBytes(status, "RssFile:") / kMB, StatusBytes(status, "RssShmem:") / kMB, DriverMemory());
}

}  // namespace memory
