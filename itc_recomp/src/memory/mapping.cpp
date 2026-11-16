// See mapping.h. One memory file (memfd_create: nothing in /dev/shm),
// 4.5 GB plus a page, mapped as nine views into one reservation:
//
//   guest addresses            file offset         what
//   0x00010000-0x3FFFFFFF      0x00010000          4 KB pages (our layer's range at 0x10000)
//   0x40000000-0x7EFFFFFF      0x40000000          64 KB pages
//   0x7F000000-0x7FFFFFFF      0x100000000         physical 0-16 MB (the L2 area, the
//                                                  tiling workers' signals)
//   0x80000000-0x8FFFFFFF      0x80000000          the image (at 0x82000000)
//   0x90000000-0x9FFFFFFF      0x80000000          the same, as the SDK mapped it
//   0xA0000000-0xBFFFFFFF      0x100000000         physical, 64 KB pages
//   0xC0000000-0xDFFFFFFF      0x100000000         physical, 16 MB pages
//   0xE0000000-0xFFFFFFFF      0x100001000         physical + 4 KB, 4 KB pages
//   base + 4 GB, 512 MB        0x100000000         physical, flat (the renderer's)
//
// Page 0's 64 KB stay unmapped (the SDK protected them: --protect_zero), so
// a null pointer faults. The file's pages are only backed once touched.

#include "memory/mapping.h"

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
// Where the SDK's mapping landed (its first try). 0 for now: the SDK's
// runtime, still made, maps its own there with MAP_FIXED over anything.
constexpr uint64_t kPreferredBase = 0;

struct View {
  uint64_t start;  // guest address (or past 4 GB: the flat physical range)
  uint64_t end;    // exclusive
  uint64_t offset;  // in the file
};

constexpr View kViews[] = {
    {0x00010000, 0x40000000, 0x00010000},
    {0x40000000, 0x7F000000, 0x40000000},
    {0x7F000000, 0x80000000, 0x100000000},
    {0x80000000, 0x90000000, 0x80000000},
    {0x90000000, 0xA0000000, 0x80000000},
    {0xA0000000, 0xC0000000, 0x100000000},
    {0xC0000000, 0xE0000000, 0x100000000},
    {0xE0000000, 0x100000000, 0x100001000},
    {0x100000000, 0x120000000, 0x100000000},
};

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
  // The views keep the file.
  close(file);
  base = mapped;
  ITC_LOG_INFO("guest memory: mapped at {}", static_cast<void*>(base));
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
