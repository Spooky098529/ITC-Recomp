#include "last_error.h"
#include "hooks/native_entry.h"

#include "base/log.h"
#include "guest.h"

#include <atomic>

namespace last_error {

void Set(uint32_t pcr, uint32_t error) {
  if (guest::LoadU32(pcr + 336)) {
    return;
  }
  guest::StoreU32(guest::LoadU32(pcr + 256) + 352, error);
}

uint32_t Get(uint32_t pcr) {
  ITC_NATIVE_RUNNING("sub_82296EC0 GetLastError");
  if (guest::LoadU32(pcr + 336)) {
    return 0;
  }
  return guest::LoadU32(guest::LoadU32(pcr + 256) + 352);
}

namespace {

// RtlNtStatusToDosError for the statuses our code can pass (plug in data:
// docs/rexglue-removal.md, "E4: the plan", step 2): the failures our
// imports answer (src/threads/) and the file opens' (game_data/), with the
// conversion's general rules; the rest come only from imports nothing of
// ours answers, which stop the program. The numbers are the kernel's.
uint32_t StatusToError(uint32_t status) {
  if (status == 0 || (status & 0x20000000) != 0) {
    return status;  // success, or a customer code: as it is
  }
  if ((status >> 16) == 0x8007 || (status >> 16) == 0xC001) {
    return status & 0xFFFF;  // a Win32 error wrapped as a status
  }
  switch (status) {
    case 0xC0000008:  // STATUS_INVALID_HANDLE
      return 6;       // ERROR_INVALID_HANDLE
    case 0xC000000D:  // STATUS_INVALID_PARAMETER
      return 87;      // ERROR_INVALID_PARAMETER
    case 0xC0000035:  // STATUS_OBJECT_NAME_COLLISION
      return 183;     // ERROR_ALREADY_EXISTS
    case 0xC00000BA:  // STATUS_FILE_IS_A_DIRECTORY
      return 5;       // ERROR_ACCESS_DENIED
    default:
      break;
  }
  static std::atomic<bool> logged{false};
  if (!logged.exchange(true)) {
    ITC_LOG_WARN("last error: no error number for status {:08X} (the first such)", status);
  }
  return 317;  // ERROR_MR_MID_NOT_FOUND, as the kernel answers an unknown one
}

}  // namespace

uint32_t SetFromStatus(uint32_t pcr, uint32_t status) {
  const uint32_t error = StatusToError(status);
  Set(pcr, error);
  return error;
}

}  // namespace last_error

// SetLastError: r3 = the error (kept in r3).
ITC_NATIVE_ENTRY_ALWAYS(82296E70) {
  
  last_error::Set(ctx.r13.u32, ctx.r3.u32);
}

// The same, reached by a jump (16 callers).
ITC_NATIVE_ENTRY_ALWAYS(82296FD0) {
  
  last_error::Set(ctx.r13.u32, ctx.r3.u32);
}

// SetLastError from an NTSTATUS: r3 = the status; returns the DOS error.
ITC_NATIVE_ENTRY_ALWAYS(82296E88) {
  
  ctx.r3.u64 = last_error::SetFromStatus(ctx.r13.u32, ctx.r3.u32);
}

// GetLastError, reached by a jump to sub_82296EC0 (after failed file and
// allocation calls): r3 the error. The original also leaves r11 ([PCR+336],
// or the current thread when that's 0): a leftover for recompiled callers,
// none now. sub_82296EC0 itself has no adapter: its only caller in play is
// this one.
ITC_NATIVE_ENTRY_ALWAYS(82295F18) {
  
  ctx.r3.u64 = last_error::Get(ctx.r13.u32);
}
