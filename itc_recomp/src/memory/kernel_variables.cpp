// The kernel's variables the game reads, plugged in (E2 step 2,
// docs/rexglue-removal.md "E2: the plan", step 1's table): each read slot
// of the image's import table points at a block of ours with the data the
// game uses (what the SDK left there in play), instead of the SDK's kernel
// data. The slots nothing that runs reads (VdGpuClockInMHz,
// VdHSIOCalibrationLock, XexExecutableModuleHandle, KeTimeStampBundle) are
// left as the loader bound them.

#include "memory/kernel_variables.h"

#include "memory/heap.h"

#include "base/flags.h"
#include "base/log.h"
#include "guest.h"

#include <cstring>

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>
#include <string_view>

ITC_FLAG_BOOL(native_kernel_variables, true, "ITC",
              "Point the image's imports of the kernel variables the game reads at data of "
              "ours (src/memory/kernel_variables.cpp, E2 in docs/rexglue-removal.md) instead "
              "of the SDK's");

namespace memory {

namespace {

constexpr uint32_t kVdGlobalDevice = 0x820007B0;
constexpr uint32_t kKeCertMonitorData = 0x820007B4;
constexpr uint32_t kExLoadedCommandLine = 0x820007EC;
constexpr uint32_t kKeDebugMonitorData = 0x820007F0;
constexpr uint32_t kExThreadObjectType = 0x820007FC;
constexpr uint32_t kXboxKrnlVersion = 0x8200088C;

uint32_t Block(uint32_t size) {
  return Alloc(size, kHeapZeroMemory);
}

}  // namespace

void BindKernelVariables(uint8_t* base) {
  if (!ITC_FLAG(native_kernel_variables)) {
    return;
  }
  // The loader leaves the import table's page read-only: writable while
  // the slots are bound, then as it was (through the SDK's heap until our
  // own mapping, step 4).
  constexpr uint32_t kPage = kVdGlobalDevice & ~0xFFFu;
  constexpr uint32_t kPageBytes = 0x1000;
  rex::memory::BaseHeap* heap = REX_KERNEL_MEMORY()->LookupHeap(kPage);
  uint32_t protection = 0;
  if (!heap || !heap->QueryProtect(kPage, &protection) ||
      !heap->Protect(kPage, kPageBytes,
                     rex::memory::kMemoryProtectRead | rex::memory::kMemoryProtectWrite)) {
    ITC_LOG_ERROR("kernel variables: the import table's page can't be made writable");
    return;
  }
  // VdGlobalDevice: a word the game writes itself (the device).
  guest::StoreU32(base, kVdGlobalDevice, Block(16));
  // KeCertMonitorData and KeDebugMonitorData: no monitor, zeros (their
  // readers test the first word).
  guest::StoreU32(base, kKeCertMonitorData, Block(64));
  guest::StoreU32(base, kKeDebugMonitorData, Block(64));
  // ExLoadedCommandLine: the command line GetCommandLineA answers.
  constexpr std::string_view kCommandLine = "\"default.xex\"";
  const uint32_t command_line = Block(64);
  std::memcpy(base + command_line, kCommandLine.data(), kCommandLine.size());
  guest::StoreU32(base, kExLoadedCommandLine, command_line);
  // ExThreadObjectType: a type record only passed back to
  // ObReferenceObjectByHandle (which ours ignores); its tag as the SDK's.
  const uint32_t thread_type = Block(32);
  std::memcpy(base + thread_type + 0x18, "Thrd", 4);
  guest::StoreU32(base, kExThreadObjectType, thread_type);
  // XboxKrnlVersion: major, minor, build, QFE (16 bits each).
  const uint32_t version = Block(16);
  guest::StoreU16(base, version + 0, 2);
  guest::StoreU16(base, version + 2, 0xFFFF);
  guest::StoreU16(base, version + 4, 0xFFFF);
  guest::StoreU16(base, version + 6, 0x8000);
  guest::StoreU32(base, kXboxKrnlVersion, version);
  heap->Protect(kPage, kPageBytes, protection);
  ITC_LOG_INFO("kernel variables: 6 bound to data of ours");
}

}  // namespace memory
