// See mapping.h. One memory file (memfd_create: nothing in /dev/shm),
// 4.5 GB plus a page, mapped as nine views into one reservation:
//
//   guest addresses            file offset         what
//   0x00010000-0x3FFFFFFF      0x00010000          4 KB pages (our layer's range at 0x10000)
//   0x40000000-0x7EFFFFFF      0x40000000          64 KB pages
//   0x80000000-0x8FFFFFFF      0x80000000          the image (at 0x82000000)
//   0x90000000-0x9FFFFFFF      0x80000000          the same, as the SDK mapped it
//   (elsewhere), 512 MB + 4 KB  0x100000000         physical (memory::g_physical,
//                                                  since E5 step 7; guest.h translates
//                                                  0x7F000000 (physical 0-16 MB),
//                                                  0xA0000000, 0xC0000000 and
//                                                  0xE0000000 (+ 4 KB) to it)
//
// Page 0's 64 KB stay unmapped (the SDK protected them: --protect_zero), so
// a null pointer faults. The file's pages are only backed once touched.

#include "memory/mapping.h"
#include "memory/handles.h"
#include "guest.h"

#include "base/log.h"

#include <sys/mman.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <mutex>

namespace memory {

namespace {

constexpr uint64_t kGuestBytes = 0x100000000ull;
constexpr uint64_t kPhysicalBytes = 0x20000000ull;
constexpr uint64_t kReserveBytes = kGuestBytes + kPhysicalBytes;
constexpr uint64_t kFileBytes = kReserveBytes + 0x1000;
// Where the SDK's mapping landed (its first try); anywhere else when it's
// taken.
constexpr uint64_t kPreferredBase = 0x100000000ull;

struct View {
  uint64_t start;  // guest address (or past 4 GB: the flat physical range)
  uint64_t end;    // exclusive
  uint64_t offset;  // in the file
};

// Since E5 step 7 the physical memory (the GPU's: textures, vertex and
// index data, shaders, the command buffer, the tiling workers' signals) isn't
// part of guest memory: it's mapped on its own (memory::g_physical, guest.h),
// the renderer reads it there, and the guest's addresses for it (0x7F000000,
// 0xA0000000, 0xC0000000, 0xE0000000) are translated by guest.h; their
// views here stay unmapped.
constexpr View kViews[] = {
    {0x00010000, 0x40000000, 0x00010000},
    {0x40000000, 0x7F000000, 0x40000000},
    {0x80000000, 0x90000000, 0x80000000},
    {0x90000000, 0xA0000000, 0x80000000},
};
constexpr uint64_t kPhysicalFileOffset = 0x100000000ull;

uint8_t* base = nullptr;

bool Map() {
  const int file = memfd_create("itc_guest_memory", MFD_CLOEXEC);
  if (file < 0) {
    ITC_LOG_ERROR("guest memory: memfd_create failed: {}", std::strerror(errno));
    return false;
  }
  if (ftruncate(file, off_t(kFileBytes)) != 0) {
    ITC_LOG_ERROR("guest memory: ftruncate failed: {}", std::strerror(errno));
    close(file);
    return false;
  }
  constexpr int kReserveFlags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
  void* reserved = MAP_FAILED;
  if (kPreferredBase != 0) {
    reserved = mmap(reinterpret_cast<void*>(kPreferredBase), kReserveBytes, PROT_NONE,
                    kReserveFlags | MAP_FIXED_NOREPLACE, -1, 0);
  }
  if (reserved == MAP_FAILED) {
    reserved = mmap(nullptr, kReserveBytes, PROT_NONE, kReserveFlags, -1, 0);
  }
  if (reserved == MAP_FAILED) {
    ITC_LOG_ERROR("guest memory: reserving {:#x} bytes failed: {}", kReserveBytes,
                  std::strerror(errno));
    close(file);
    return false;
  }
  uint8_t* mapped = static_cast<uint8_t*>(reserved);
  for (const View& view : kViews) {
    void* at = mapped + view.start;
    void* result = mmap(at, view.end - view.start, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED,
                        file, off_t(view.offset));
    if (result != at) {
      ITC_LOG_ERROR("guest memory: mapping {:#x}-{:#x} failed: {}", view.start, view.end,
                    std::strerror(errno));
      munmap(reserved, kReserveBytes);
      close(file);
      return false;
    }
  }
  // The physical memory, on its own (and its 4 KB past the end, which the
  // 0xE0000000 addresses reach: they're offset by 4 KB).
  void* physical = mmap(nullptr, kPhysicalBytes + 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, file,
                        off_t(kPhysicalFileOffset));
  if (physical == MAP_FAILED) {
    ITC_LOG_ERROR("guest memory: mapping the physical memory failed: {}", std::strerror(errno));
    munmap(reserved, kReserveBytes);
    close(file);
    return false;
  }
  g_physical = static_cast<uint8_t*>(physical);
  // The views keep the file.
  close(file);
  base = mapped;
  ITC_LOG_INFO("guest memory: mapped at {}", static_cast<void*>(base));
  // The handles' range, inaccessible (docs/e5-native.md step 3; handles.h).
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
