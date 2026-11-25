#include "memory/c_runtime_heap.h"

#include "guest.h"

#include "base/log.h"
#include "memory/heap.h"
#include "schema/heap.h"

#include <cstring>

namespace memory::crt {

namespace {

// The largest size malloc, calloc and realloc take (-4096).
constexpr uint32_t kMaxSize = 0xFFFFF000;
// XMemAlloc's attributes.
constexpr uint32_t kPhysical = 0x80000000;
constexpr uint32_t kZero = 0x40000000;
constexpr uint32_t kMemLargePages = 0x20000000;

}  // namespace

uint32_t GetProcessHeap() {
  ITC_NATIVE_RUNNING("sub_8229B670 GetProcessHeap");
  return schema::HeapGlobals::process_heap().address;
}

std::optional<uint32_t> Malloc(uint32_t size) {
  ITC_NATIVE_RUNNING("sub_825A9A90 malloc");
  if (size > kMaxSize || GetProcessHeap() == 0) {
    return std::nullopt;
  }
  const uint32_t block = memory::Alloc(size == 0 ? 1 : size, 0);
  if (block == 0) {
    return std::nullopt;
  }
  return block;
}

bool Free(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_825A9B58 free");
  return block == 0 || memory::Free(block);
}

std::optional<uint32_t> CallocImpl(uint32_t count, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_825AC248 _calloc_impl");
  if (count != 0 && kMaxSize / count < size) {
    return std::nullopt;
  }
  uint32_t bytes = count * size;
  if (bytes == 0) {
    bytes = 1;
  }
  const uint32_t block = memory::Alloc(bytes, memory::kHeapZeroMemory);
  if (block == 0) {
    return std::nullopt;
  }
  return block;
}

std::optional<uint32_t> Calloc(uint32_t count, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_825AC320 calloc");
  return CallocImpl(count, size);
}

std::optional<uint32_t> Realloc(uint32_t block, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_825AEBC8 realloc");
  if (block == 0) {
    return Malloc(size);
  }
  if (size == 0) {
    if (!Free(block)) {
      return std::nullopt;
    }
    return 0;
  }
  if (size > kMaxSize) {
    return std::nullopt;
  }
  const uint32_t result = memory::Realloc(block, size, 0);
  if (result == 0) {
    return std::nullopt;
  }
  return result;
}

std::optional<uint32_t> Msize(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_825AED30 _msize");
  if (block == 0) {
    return std::nullopt;
  }
  return memory::Size(block);
}

uint32_t LocalAlloc(uint32_t flags, uint32_t size) {
  ITC_NATIVE_RUNNING("sub_82297F48 LocalAlloc");
  return memory::Alloc(size, (flags >> 3) & memory::kHeapZeroMemory);
}

uint32_t LocalFree(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82297F90 LocalFree");
  return memory::Free(block) ? 0 : block;
}

uint32_t XMemAlloc(uint32_t size, uint32_t attributes) {
  ITC_NATIVE_RUNNING("sub_822976F8 XMemAlloc");
  if (!(attributes & kPhysical)) {
    return LocalAlloc((attributes >> 24) & 0x40, size);
  }
  // The alignment's field (bits 24-27, a power of two) is 12 (4 KiB) when 0.
  if ((attributes & 0x0F000000) == 0) {
    attributes |= 0x0C000000;
  }
  const uint32_t alignment = 1u << ((attributes >> 24) & 0xF);
  uint32_t protection = guest::LoadU32(schema::HeapGlobals::physical_protections_address() + ((attributes >> 28) & 3) * 4);
  // XPhysicalAlloc (sub_822964C8) with any physical address.
  if (schema::HeapGlobals::no_large_physical_pages() != 0) {
    protection &= ~kMemLargePages;
  }
  const uint32_t block = memory::AllocPhysical(size, 0, 0xFFFFFFFF, alignment, protection);
  if (block != 0 && (attributes & kZero)) {
    guest::ForEachHostPart(block, size, [](uint8_t* host, uint32_t, uint32_t bytes) {
      std::memset(host, 0, bytes);
    });
  }
  return block;
}

uint32_t XPhysicalAllocEx(uint32_t size, uint32_t low, uint32_t high,
                          uint32_t alignment, uint32_t protection) {
  if (schema::HeapGlobals::no_large_physical_pages() != 0) {
    protection &= ~kMemLargePages;
  }
  return memory::AllocPhysical(size, low, high, alignment, protection);
}

uint32_t XMemFree(uint32_t block, uint32_t attributes) {
  ITC_NATIVE_RUNNING("sub_82297790 XMemFree");
  if (!(attributes & kPhysical)) {
    return LocalFree(block);
  }
  if (block != 0) {
    // XPhysicalFree; its adapter leaves r3 as it was.
    memory::FreePhysical(block);
  }
  return block;
}

std::optional<uint32_t> OperatorNew(uint32_t size) {
  ITC_NATIVE_RUNNING("sub_825A6880 operator new");
  return Malloc(size);
}

std::optional<uint32_t> OperatorNewArray(uint32_t size) {
  ITC_NATIVE_RUNNING("sub_825A6940 operator new[]");
  return OperatorNew(size);
}

bool OperatorDelete(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_825A70E8 operator delete");
  return Free(block);
}

bool OperatorDeleteArray(uint32_t block) {
  ITC_NATIVE_RUNNING("sub_825A71C0 operator delete[]");
  return OperatorDelete(block);
}

}  // namespace memory::crt
