// The C runtime's start-up pieces natively (start_up.h), storing what the
// translated code stores, in the same order.

#include "c_runtime/start_up.h"

#include "base/log.h"

#include "guest.h"

namespace c_runtime {

void InitFloatConversions(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825A6DB0 _cfltcvt_init");
  static constexpr uint32_t kTable = 0x8266BA5C;
  static constexpr uint32_t kFunctions[10] = {
      0x825AEB70, 0x825ADF20, 0x825ADF10, 0x825ADF18, 0x825ADE90,
      0x825AEB70, 0x825AEB20, 0x825ADEB0, 0x825ADDD8, 0x825ADD38,
  };
  for (uint32_t i = 0; i < 10; i++) {
    guest::StoreU32(base, kTable + 4 * i, kFunctions[i]);
  }
}

void SetHandler82971170(uint8_t* base, uint32_t value) {
  guest::StoreU32(base, 0x82971170, value);
}

void SetHandler82971178(uint8_t* base, uint32_t value) {
  guest::StoreU32(base, 0x82971178, value);
}

void SetInvalidParameterHandler(uint8_t* base, uint32_t value) {
  guest::StoreU32(base, 0x82978320, value);
}

void SetCriticalSectionInitializer(uint8_t* base, uint32_t value) {
  guest::StoreU32(base, 0x8297132C, value);
}

void SetDefaultCriticalSectionInitializer(uint8_t* base) {
  guest::StoreU32(base, 0x82971328, 0x825B3498);
}

void InitPointers(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825AA398 _init_pointers");
  SetHandler82971170(base, 0);
  SetCriticalSectionInitializer(base, 0);
  SetInvalidParameterHandler(base, 0);
  SetHandler82971178(base, 0);
  SetDefaultCriticalSectionInitializer(base);
  guest::StoreU32(base, 0x8266B7B0, 0x825AA378);
}

int32_t InitMbcTable(uint8_t* base) {
  static constexpr uint32_t kDone = 0x82978338;
  if (guest::LoadU32(base, kDone) == 0) {
    const uint32_t info = guest::LoadU32(base, 0x8266C450);
    guest::StoreU32(base, 0x8297136C, uint32_t(-3));
    guest::StoreU32(base, 0x8297135C, 0);
    SetSbcs(base, info);
    guest::StoreU32(base, kDone, 1);
  }
  return 0;
}

void SetSbcs(uint8_t* base, uint32_t info) {
  ITC_NATIVE_RUNNING("sub_825B68C8 setSBCS");
  static constexpr uint32_t kCLocale = 0x8266C028;
  for (uint32_t i = 0; i < 257; i++) {
    guest::StoreU8(base, info + 28 + i, 0);
  }
  guest::StoreU32(base, info + 4, 0);
  guest::StoreU32(base, info + 8, 0);
  guest::StoreU32(base, info + 12, 0);
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU16(base, info + 16 + 2 * i, 0);
  }
  for (uint32_t i = 0; i < 257; i++) {
    guest::StoreU8(base, info + 28 + i, guest::LoadU8(base, kCLocale + 28 + i));
  }
  for (uint32_t i = 0; i < 256; i++) {
    guest::StoreU8(base, info + 285 + i, guest::LoadU8(base, kCLocale + 285 + i));
  }
}

}  // namespace c_runtime
