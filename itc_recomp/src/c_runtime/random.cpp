// The C runtime's rand (docs/functions/c-runtime.md): see random.h.

#include "c_runtime/random.h"

#include "base/log.h"
#include "guest.h"

namespace c_runtime {

namespace {

constexpr uint32_t kGetLastError = 0x82295F18;
constexpr uint32_t kSetLastError = 0x82296FD0;
constexpr uint32_t kTlsGetValue = 0x825DBBF4;  // KeTlsGetValue's thunk
constexpr uint32_t kTlsSetValue = 0x825DBC04;  // KeTlsSetValue's thunk
constexpr uint32_t kGetptd = 0x825AA580;
constexpr uint32_t kGetterSlot = 0x8266B7B8;   // the TLS index holding the getter
constexpr uint32_t kDataIndex = 0x8266B7B4;    // the index the getter is given
constexpr uint32_t kGetter = 0x82971160;       // the getter set at start-up

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  guest::StoreU32(base, frame, sp);
  return frame;
}

}  // namespace

uint32_t ThreadData(uint8_t* base, uint32_t sp, uint32_t lr, const ThreadDataCall& call) {
  const uint32_t getptd = Frame(base, sp, 96);
  const uint32_t noexit = Frame(base, getptd, 112);
  const uint64_t saved = call.Call(kGetLastError, noexit, 0x825AA4D0, 0, 0);
  // sub_825AA400 (96 bytes below): the getter put in its slot when empty.
  const uint32_t init = Frame(base, noexit, 96);
  if (uint32_t(call.Call(kTlsGetValue, init, 0x825AA41C, 1, guest::LoadU32(base, kGetterSlot))) ==
      0) {
    call.Call(kTlsSetValue, init, 0x825AA434, 2, guest::LoadU32(base, kGetterSlot),
              guest::LoadU32(base, kGetter));
  }
  const uint32_t getter =
      uint32_t(call.Call(kTlsGetValue, noexit, 0x825AA4E4, 1, guest::LoadU32(base, kGetterSlot)));
  const uint32_t data =
      uint32_t(call.Call(getter, noexit, 0x825AA4F8, 1, guest::LoadU32(base, kDataIndex)));
  call.Call(kSetLastError, noexit, 0x825AA570, 1, saved);
  if (data != 0) {
    return data;
  }
  // No data yet: the original allocates it (never seen in play).
  return uint32_t(call.Call(kGetptd, sp, lr, 0, 0));
}

uint64_t Rand(uint8_t* base, uint32_t sp, const ThreadDataCall& call) {
  ITC_NATIVE_RUNNING("sub_825A9BB8 rand");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t data = ThreadData(base, frame, 0x825A9BC8, call);
  const uint32_t seed = guest::LoadU32(base, data + 20) * 214013u + 2531011u;
  guest::StoreU32(base, data + 20, seed);
  return (seed >> 16) & 0x7FFF;
}

}  // namespace c_runtime
