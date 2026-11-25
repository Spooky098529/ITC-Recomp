// The C runtime's start-up pieces natively (start_up.h), storing what the
// translated code stores, in the same order.

#include "c_runtime/start_up.h"

#include "base/log.h"

#include "guest.h"
#include "schema/c_runtime.h"
#include "schema/heap.h"

namespace c_runtime {

void InitFloatConversions() {
  ITC_NATIVE_RUNNING("sub_825A6DB0 _cfltcvt_init");
  static constexpr uint32_t kFunctions[10] = {
      0x825AEB70, 0x825ADF20, 0x825ADF10, 0x825ADF18, 0x825ADE90,
      0x825AEB70, 0x825AEB20, 0x825ADEB0, 0x825ADDD8, 0x825ADD38,
  };
  for (uint32_t i = 0; i < 10; i++) {
    guest::StoreU32(schema::CRuntimeGlobals::float_conversions_address() + 4 * i,
                    kFunctions[i]);
  }
}

void SetHandler82971170(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_825AD140 (sets 0x82971170)");
  schema::HeapGlobals::set_new_handler(value);
}

void SetHandler82971178(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_825AEE18 (sets 0x82971178)");
  schema::CRuntimeGlobals::set_unknown_82971178(value);
}

void SetInvalidParameterHandler(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_825AFCB8 (sets 0x82978320)");
  schema::CRuntimeGlobals::set_invalid_parameter_handler(value);
}

void SetCriticalSectionInitializer(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_825B3560 (sets 0x8297132C)");
  schema::CRuntimeGlobals::set_critical_section_initializer(value);
}

void SetDefaultCriticalSectionInitializer() {
  ITC_NATIVE_RUNNING("sub_825B3548 (sets 0x82971328)");
  schema::CRuntimeGlobals::set_default_critical_section_initializer(0x825B3498);
}

void InitPointers() {
  ITC_NATIVE_RUNNING("sub_825AA398 _init_pointers");
  SetHandler82971170(0);
  SetCriticalSectionInitializer(0);
  SetInvalidParameterHandler(0);
  SetHandler82971178(0);
  SetDefaultCriticalSectionInitializer();
  schema::CRuntimeGlobals::set_aexit_rtn(0x825AA378);
}

int32_t InitMbcTable() {
  using schema::CRuntimeGlobals;
  if (CRuntimeGlobals::mbctable_done() == 0) {
    const uint32_t info = CRuntimeGlobals::mbc_info().address;
    CRuntimeGlobals::set_mbc_code_page(-3);
    CRuntimeGlobals::set_mbc_is_set(0);
    SetSbcs(info);
    CRuntimeGlobals::set_mbctable_done(1);
  }
  return 0;
}

void SetSbcs(uint32_t info) {
  ITC_NATIVE_RUNNING("sub_825B68C8 setSBCS");
  const schema::MbcInfo to(info);
  const schema::MbcInfo from(schema::CRuntimeGlobals::c_mbc_info_address());
  for (uint32_t i = 0; i < 257; i++) {
    guest::StoreU8(to.mbctype_address() + i, 0);
  }
  to.set_code_page(0);
  to.set_is_mb_code_page(0);
  to.set_lcid(0);
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU16(to.lead_byte_ranges_address() + 2 * i, 0);
  }
  for (uint32_t i = 0; i < 257; i++) {
    guest::StoreU8(to.mbctype_address() + i, guest::LoadU8(from.mbctype_address() + i));
  }
  for (uint32_t i = 0; i < 256; i++) {
    guest::StoreU8(to.mbcasemap_address() + i,
                   guest::LoadU8(from.mbcasemap_address() + i));
  }
}

}  // namespace c_runtime
