// The C runtime's rand (docs/functions/c-runtime.md): see random.h.

#include "c_runtime/random.h"

#include "base/log.h"
#include "guest.h"
#include "schema/c_runtime.h"
#include "schema/c_runtime_core.h"

namespace c_runtime {

namespace {

constexpr uint32_t kGetLastError = 0x82295F18;
constexpr uint32_t kSetLastError = 0x82296FD0;
constexpr uint32_t kTlsGetValue = 0x825DBBF4;  // KeTlsGetValue's thunk
constexpr uint32_t kTlsSetValue = 0x825DBC04;  // KeTlsSetValue's thunk
constexpr uint32_t kGetptd = 0x825AA580;

}  // namespace

uint32_t ThreadData(uint8_t* base, uint32_t sp, uint32_t lr, const ThreadDataCall& call) {
  // The originals' frames (_getptd's 96 bytes, _getptd_noexit's 112 and
  // sub_825AA400's 96, each only its back chain) left out: their calls run
  // from `sp` (E5 step 6; none reads its caller's frame).
  const uint64_t saved = call.Call(kGetLastError, sp, 0x825AA4D0, 0, 0);
  // sub_825AA400: the getter (__getvalueindex's slot) put in its slot when
  // empty; the getter is given __flsindex.
  using Core = schema::CRuntimeCoreGlobals;
  if (uint32_t(call.Call(kTlsGetValue, sp, 0x825AA41C, 1, Core::getvalue_index(base))) == 0) {
    call.Call(kTlsSetValue, sp, 0x825AA434, 2, Core::getvalue_index(base),
              Core::fls_get_value(base));
  }
  const uint32_t getter =
      uint32_t(call.Call(kTlsGetValue, sp, 0x825AA4E4, 1, Core::getvalue_index(base)));
  const uint32_t data = uint32_t(call.Call(getter, sp, 0x825AA4F8, 1, Core::fls_index(base)));
  call.Call(kSetLastError, sp, 0x825AA570, 1, saved);
  if (data != 0) {
    return data;
  }
  // No data yet: the original allocates it (never seen in play).
  return uint32_t(call.Call(kGetptd, sp, lr, 0, 0));
}

uint64_t Rand(uint8_t* base, uint32_t sp, const ThreadDataCall& call) {
  ITC_NATIVE_RUNNING("sub_825A9BB8 rand");
  // The original's 96-byte frame (only its back chain) left out (E5 step 6).
  const uint32_t data = ThreadData(base, sp, 0x825A9BC8, call);
  const schema::ThreadData thread_data(base, data);
  const uint32_t seed = thread_data.rand_seed() * 214013u + 2531011u;
  thread_data.set_rand_seed(seed);
  return (seed >> 16) & 0x7FFF;
}

}  // namespace c_runtime
