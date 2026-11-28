#include "base/os_memory.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace base {

#ifdef _WIN32

uint8_t* ReserveZeroed(uint64_t bytes) {
  // Committed whole (counted against the commit limit), but a page takes
  // memory only once touched.
  return static_cast<uint8_t*>(
      VirtualAlloc(nullptr, SIZE_T(bytes), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
}

void DropPages(void* start, uint64_t bytes) {
  // Decommitted and committed again: zeros when next read (MEM_RESET would
  // keep the old contents until the system takes the pages).
  VirtualFree(start, SIZE_T(bytes), MEM_DECOMMIT);
  VirtualAlloc(start, SIZE_T(bytes), MEM_COMMIT, PAGE_READWRITE);
}

uint64_t PageSize() {
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  return info.dwPageSize;
}

bool ResidentPages(const void* start, uint64_t bytes, std::vector<unsigned char>& resident) {
  const uint64_t page = PageSize();
  const size_t count = size_t((bytes + page - 1) / page);
  std::vector<PSAPI_WORKING_SET_EX_INFORMATION> info(count);
  for (size_t i = 0; i < count; i++) {
    info[i].VirtualAddress = const_cast<uint8_t*>(static_cast<const uint8_t*>(start)) + i * page;
  }
  if (!K32QueryWorkingSetEx(GetCurrentProcess(), info.data(),
                            DWORD(count * sizeof(PSAPI_WORKING_SET_EX_INFORMATION)))) {
    return false;
  }
  resident.resize(count);
  for (size_t i = 0; i < count; i++) {
    resident[i] = info[i].VirtualAttributes.Valid ? 1 : 0;
  }
  return true;
}

#else

uint8_t* ReserveZeroed(uint64_t bytes) {
  void* block = mmap(nullptr, bytes, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
  return block == MAP_FAILED ? nullptr : static_cast<uint8_t*>(block);
}

void DropPages(void* start, uint64_t bytes) {
  madvise(start, bytes, MADV_DONTNEED);
}

uint64_t PageSize() {
  return uint64_t(sysconf(_SC_PAGESIZE));
}

bool ResidentPages(const void* start, uint64_t bytes, std::vector<unsigned char>& resident) {
  const uint64_t page = PageSize();
  resident.assign(size_t((bytes + page - 1) / page), 0);
  if (mincore(const_cast<void*>(start), bytes, resident.data()) != 0) {
    return false;
  }
  for (unsigned char& r : resident) {
    r &= 1;
  }
  return true;
}

#endif

}  // namespace base
