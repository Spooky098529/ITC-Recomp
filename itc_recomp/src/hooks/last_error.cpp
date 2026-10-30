#include "last_error.h"

#include "base/log.h"
#include "guest.h"

#include <rex/kernel/xboxkrnl/error.h>

namespace last_error {

void Set(uint8_t* base, uint32_t pcr, uint32_t error) {
  if (guest::LoadU32(base, pcr + 336)) {
    return;
  }
  guest::StoreU32(base, guest::LoadU32(base, pcr + 256) + 352, error);
}

uint32_t Get(uint8_t* base, uint32_t pcr) {
  ITC_NATIVE_RUNNING("sub_82296EC0 GetLastError");
  if (guest::LoadU32(base, pcr + 336)) {
    return 0;
  }
  return guest::LoadU32(base, guest::LoadU32(base, pcr + 256) + 352);
}

uint32_t SetFromStatus(uint8_t* base, uint32_t pcr, uint32_t status) {
  const uint32_t error = rex::kernel::xboxkrnl::xeRtlNtStatusToDosError(status);
  Set(base, pcr, error);
  return error;
}

}  // namespace last_error

// SetLastError: r3 = the error (kept in r3).
extern "C" REX_FUNC(sub_82296E70) {
  REX_FUNC_PROLOGUE();
  last_error::Set(base, ctx.r13.u32, ctx.r3.u32);
}

// The same, reached by a jump (16 callers).
extern "C" REX_FUNC(sub_82296FD0) {
  REX_FUNC_PROLOGUE();
  last_error::Set(base, ctx.r13.u32, ctx.r3.u32);
}

// SetLastError from an NTSTATUS: r3 = the status; returns the DOS error.
extern "C" REX_FUNC(sub_82296E88) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = last_error::SetFromStatus(base, ctx.r13.u32, ctx.r3.u32);
}

// GetLastError: r3 the error; r11 left as the original leaves it ([PCR+336],
// or the current thread when that's 0).
extern "C" REX_FUNC(sub_82296EC0) {
  REX_FUNC_PROLOGUE();
  const uint32_t in_dpc = guest::LoadU32(base, ctx.r13.u32 + 336);
  ctx.r11.u64 = in_dpc ? in_dpc : guest::LoadU32(base, ctx.r13.u32 + 256);
  ctx.r3.u64 = last_error::Get(base, ctx.r13.u32);
}

// The same, reached by a jump (after failed file and allocation calls).
extern "C" REX_FUNC(sub_82295F18) {
  REX_FUNC_PROLOGUE();
  const uint32_t in_dpc = guest::LoadU32(base, ctx.r13.u32 + 336);
  ctx.r11.u64 = in_dpc ? in_dpc : guest::LoadU32(base, ctx.r13.u32 + 256);
  ctx.r3.u64 = last_error::Get(base, ctx.r13.u32);
}
