// The C runtime's core (core.h, docs/functions/c-runtime-core.md): step for
// step as the recompiled functions, the kernel imports guest calls.

#include "c_runtime/core.h"

#include "base/log.h"
#include "guest.h"

namespace c_runtime::core {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

}  // namespace

uint32_t CrtTlsAlloc(uint32_t callback, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA3F8 __crtTlsAlloc");
  return uint32_t(calls.import(Import::kKeTlsAlloc, {callback}));
}

uint32_t SetFlsGetValue(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA400 __set_flsgetvalue");
  const uint32_t value =
      uint32_t(calls.import(Import::kKeTlsGetValue, {Load(base, kGetValueIndex)}));
  if (value != 0) {
    return value;
  }
  return uint32_t(calls.import(Import::kKeTlsSetValue,
                               {Load(base, kGetValueIndex), Load(base, kFlsGetValue)}));
}

uint32_t GetptdNoexit(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA4C0 _getptd_noexit");
  const uint32_t saved = calls.get_last_error();
  SetFlsGetValue(base, calls);
  const uint32_t getter =
      uint32_t(calls.import(Import::kKeTlsGetValue, {Load(base, kGetValueIndex)}));
  uint32_t data = uint32_t(calls.guest(getter, {Load(base, kFlsIndex)}));
  if (data == 0) {
    data = calls.calloc(1, kThreadDataBytes);
    if (data != 0) {
      const uint32_t stored = uint32_t(
          calls.guest(Load(base, kFlsSetValue), {Load(base, kFlsIndex), data}));
      if (stored != 0) {
        Store(base, data + 20, 1);
        Store(base, data + 92, kXcptActTab);
        Store(base, data + 0, calls.thread_id());
        Store(base, data + 4, 0xFFFFFFFFu);
      } else {
        calls.free(data);
        data = 0;
      }
    }
  }
  calls.set_last_error(saved);
  return data;
}

void FreeFls(uint8_t* base, uint32_t data, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA5C0 _freefls");
  if (data == 0) {
    return;
  }
  for (uint32_t offset : {36u, 44u, 52u, 60u, 68u, 72u}) {
    if (const uint32_t buffer = Load(base, data + offset); buffer != 0) {
      calls.free(buffer);
    }
  }
  if (const uint32_t table = Load(base, data + 92); table != kXcptActTab) {
    calls.free(table);
  }
  calls.free(data);
}

void MtDeleteLocks(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825B3260 _mtdeletelocks");
  for (uint32_t entry = kLockTable; entry < kLockTable + 8 * kLockCount; entry += 8) {
    const uint32_t section = Load(base, entry + 0);
    if (section != 0 && Load(base, entry + 4) != 1) {
      calls.free(section);
      Store(base, entry + 0, 0);
    }
  }
}

void MtTerm(uint8_t* base, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_825AA448 _mtterm");
  if (const uint32_t index = Load(base, kFlsIndex); index != 0xFFFFFFFFu) {
    calls.guest(Load(base, kFlsFree), {index});
    Store(base, kFlsIndex, 0xFFFFFFFFu);
  }
  if (const uint32_t index = Load(base, kGetValueIndex); index != 0xFFFFFFFFu) {
    calls.import(Import::kKeTlsFree, {index});
    Store(base, kGetValueIndex, 0xFFFFFFFFu);
  }
  MtDeleteLocks(base, calls);
}

}  // namespace c_runtime::core
