// The views of docs/functions/c-runtime.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/c-runtime.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: _dosmaperr.ret
struct ThreadData : View {
  using View::View;
  static constexpr uint32_t kSize = 196;
  uint32_t thread_id() const { return guest::LoadU32(address + 0); }
  void set_thread_id(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t thread_id_address() const { return address + 0; }
  // -1
  uint32_t thread_handle() const { return guest::LoadU32(address + 4); }
  void set_thread_handle(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t thread_handle_address() const { return address + 4; }
  // errno (_errno returns its address)
  int32_t errno_value() const { return int32_t(guest::LoadU32(address + 8)); }
  void set_errno_value(int32_t v) const { guest::StoreU32(address + 8, uint32_t(v)); }
  uint32_t errno_value_address() const { return address + 8; }
  // _doserrno
  uint32_t doserrno() const { return guest::LoadU32(address + 12); }
  void set_doserrno(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t doserrno_address() const { return address + 12; }
  // rand's seed, 1 at first
  uint32_t rand_seed() const { return guest::LoadU32(address + 20); }
  void set_rand_seed(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t rand_seed_address() const { return address + 20; }
  // the exception action table (0x8266BE60, _XcptActTab, unless copied)
  uint32_t exception_actions() const { return guest::LoadU32(address + 92); }
  void set_exception_actions(uint32_t v) const { guest::StoreU32(address + 92, v); }
  uint32_t exception_actions_address() const { return address + 92; }
  // raise saves it, clears it and puts it back around an FPE handler (_tpxcptinfoptrs, probably)
  uint32_t exception_pointers() const { return guest::LoadU32(address + 96); }
  void set_exception_pointers(uint32_t v) const { guest::StoreU32(address + 96, v); }
  uint32_t exception_pointers_address() const { return address + 96; }
  // raise saves it, sets 140 and puts it back around an FPE handler, which is given it (_tfpecode, probably)
  uint32_t fpe_code() const { return guest::LoadU32(address + 100); }
  void set_fpe_code(uint32_t v) const { guest::StoreU32(address + 100, v); }
  uint32_t fpe_code_address() const { return address + 100; }
  uint32_t terminate_handler() const { return guest::LoadU32(address + 108); }
  void set_terminate_handler(uint32_t v) const { guest::StoreU32(address + 108, v); }
  uint32_t terminate_handler_address() const { return address + 108; }
};

// image; measure: [8266BC28]
struct Locale : View {
  using View::View;
  static constexpr uint32_t kSize = 204;
  // the longest multibyte character (above 1 goes to sub_825AFD98, never seen)
  int32_t mb_cur_max() const { return int32_t(guest::LoadU32(address + 172)); }
  void set_mb_cur_max(int32_t v) const { guest::StoreU32(address + 172, uint32_t(v)); }
  uint32_t mb_cur_max_address() const { return address + 172; }
  // the character type table: halfwords, 1 upper, 2 lower, 4 digit, 8 space, 0x100 alpha, 0x8000 lead byte
  uint32_t ctype_table() const { return guest::LoadU32(address + 200); }
  void set_ctype_table(uint32_t v) const { guest::StoreU32(address + 200, v); }
  uint32_t ctype_table_address() const { return address + 200; }
};

// image; measure: [8266C024]
struct Lconv : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the decimal point's text ('.')
  template <typename T = CString>
  T decimal_point() const { return T(guest::LoadU32(address + 0)); }
  void set_decimal_point(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_decimal_point(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t decimal_point_address() const { return address + 0; }
};

// image; measure: -
struct MbcInfo : View {
  using View::View;
  static constexpr uint32_t kSize = 541;
  // probably
  uint32_t ref_count() const { return guest::LoadU32(address + 0); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t ref_count_address() const { return address + 0; }
  // probably (mbcodepage); 0 after setSBCS
  uint32_t code_page() const { return guest::LoadU32(address + 4); }
  void set_code_page(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t code_page_address() const { return address + 4; }
  // probably; 0 after setSBCS
  uint32_t is_mb_code_page() const { return guest::LoadU32(address + 8); }
  void set_is_mb_code_page(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t is_mb_code_page_address() const { return address + 8; }
  // probably; 0 after setSBCS
  uint32_t lcid() const { return guest::LoadU32(address + 12); }
  void set_lcid(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t lcid_address() const { return address + 12; }
  // six halfwords (mbulinfo, probably), cleared by setSBCS
  uint32_t lead_byte_ranges_address() const { return address + 16; }
  // the multibyte character types
  uint32_t mbctype_address() const { return address + 28; }
  // the case map
  uint32_t mbcasemap_address() const { return address + 285; }
};

// image; measure: -
struct FloatConversionTable : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // 0x825AEB70
  uint32_t cfltcvt() const { return guest::LoadU32(address + 0); }
  void set_cfltcvt(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t cfltcvt_address() const { return address + 0; }
  // 0x825ADF20
  uint32_t cropzeros() const { return guest::LoadU32(address + 4); }
  void set_cropzeros(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t cropzeros_address() const { return address + 4; }
  // 0x825ADF10
  uint32_t fassign() const { return guest::LoadU32(address + 8); }
  void set_fassign(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t fassign_address() const { return address + 8; }
  // 0x825ADF18
  uint32_t forcdecpt() const { return guest::LoadU32(address + 12); }
  void set_forcdecpt(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t forcdecpt_address() const { return address + 12; }
  // 0x825ADE90
  uint32_t positive() const { return guest::LoadU32(address + 16); }
  void set_positive(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t positive_address() const { return address + 16; }
  // 0x825AEB70 (_cfltcvt again)
  uint32_t cldcvt() const { return guest::LoadU32(address + 20); }
  void set_cldcvt(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t cldcvt_address() const { return address + 20; }
  // 0x825AEB20, _cfltcvt_l: the printf engines call through it
  uint32_t cfltcvt_l() const { return guest::LoadU32(address + 24); }
  void set_cfltcvt_l(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t cfltcvt_l_address() const { return address + 24; }
  // 0x825ADEB0
  uint32_t fassign_l() const { return guest::LoadU32(address + 28); }
  void set_fassign_l(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t fassign_l_address() const { return address + 28; }
  // 0x825ADDD8: cuts zeros for %g
  uint32_t cropzeros_l() const { return guest::LoadU32(address + 32); }
  void set_cropzeros_l(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t cropzeros_l_address() const { return address + 32; }
  // 0x825ADD38: forces the point for # with precision 0
  uint32_t forcdecpt_l() const { return guest::LoadU32(address + 36); }
  void set_forcdecpt_l(uint32_t v) const { guest::StoreU32(address + 36, v); }
  uint32_t forcdecpt_l_address() const { return address + 36; }
};

// embedded; measure: _output_l.r3, _woutput_l.r3
struct Stream : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // where the next character goes
  uint32_t pointer() const { return guest::LoadU32(address + 0); }
  void set_pointer(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t pointer_address() const { return address + 0; }
  // the room left (_cnt)
  uint32_t room() const { return guest::LoadU32(address + 4); }
  void set_room(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t room_address() const { return address + 4; }
  // the buffer's base
  uint32_t buffer() const { return guest::LoadU32(address + 8); }
  void set_buffer(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t buffer_address() const { return address + 8; }
  // 0x40 a string
  uint32_t flags() const { return guest::LoadU32(address + 12); }
  void set_flags(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t flags_address() const { return address + 12; }
  // the file handle (_file)
  uint32_t file() const { return guest::LoadU32(address + 16); }
  void set_file(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t file_address() const { return address + 16; }
  // the buffer's size (_bufsiz)
  uint32_t buffer_size() const { return guest::LoadU32(address + 24); }
  void set_buffer_size(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t buffer_size_address() const { return address + 24; }
  // a temporary file's name, freed at close (_tmpfname)
  uint32_t temp_name() const { return guest::LoadU32(address + 28); }
  void set_temp_name(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t temp_name_address() const { return address + 28; }
};

// embedded; measure: _fltout2.r4, _cftof2_l.r6, _fptostr.r6
struct FloatStrings : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // the sign character
  uint32_t sign() const { return guest::LoadU32(address + 0); }
  void set_sign(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t sign_address() const { return address + 0; }
  int32_t decimal_exponent() const { return int32_t(guest::LoadU32(address + 4)); }
  void set_decimal_exponent(int32_t v) const { guest::StoreU32(address + 4, uint32_t(v)); }
  uint32_t decimal_exponent_address() const { return address + 4; }
  // $I10_OUTPUT's result (0 for an infinity or NaN)
  uint32_t flag() const { return guest::LoadU32(address + 8); }
  void set_flag(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t flag_address() const { return address + 8; }
  // the digits' buffer
  template <typename T = CString>
  T mantissa() const { return T(guest::LoadU32(address + 12)); }
  void set_mantissa(const View& v) const { guest::StoreU32(address + 12, v.address); }
  void set_mantissa(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t mantissa_address() const { return address + 12; }
};

// embedded; measure: sub_825B8B48.r7
struct FloatOutput : View {
  using View::View;
  static constexpr uint32_t kSize = 26;
  // the decimal exponent
  int16_t exponent() const { return int16_t(guest::LoadU16(address + 0)); }
  void set_exponent(int16_t v) const { guest::StoreU16(address + 0, uint16_t(v)); }
  uint32_t exponent_address() const { return address + 0; }
  // '-' or ' '
  uint8_t sign() const { return guest::LoadU8(address + 2); }
  void set_sign(uint8_t v) const { guest::StoreU8(address + 2, v); }
  uint32_t sign_address() const { return address + 2; }
  // the digits' count
  uint8_t count() const { return guest::LoadU8(address + 3); }
  void set_count(uint8_t v) const { guest::StoreU8(address + 3, v); }
  uint32_t count_address() const { return address + 3; }
  // the digits and a terminator
  uint32_t digits_address() const { return address + 4; }
};

// embedded; measure: ___dtold.r3
struct Ld12 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the sign and 15-bit exponent
  uint16_t sign_exponent() const { return guest::LoadU16(address + 0); }
  void set_sign_exponent(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t sign_exponent_address() const { return address + 0; }
  uint32_t mantissa_high() const { return guest::LoadU32(address + 2); }
  void set_mantissa_high(uint32_t v) const { guest::StoreU32(address + 2, v); }
  uint32_t mantissa_high_address() const { return address + 2; }
  uint32_t mantissa_low() const { return guest::LoadU32(address + 6); }
  void set_mantissa_low(uint32_t v) const { guest::StoreU32(address + 6, v); }
  uint32_t mantissa_low_address() const { return address + 6; }
};

// embedded; measure: ___dtold.r4, _cftof_l.r3, sub_825B0470.r4
struct DoubleValue : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  double value() const { return std::bit_cast<double>(guest::LoadU64(address + 0)); }
  void set_value(double v) const { guest::StoreU64(address + 0, std::bit_cast<uint64_t>(v)); }
  uint32_t value_address() const { return address + 0; }
  uint64_t bits() const { return guest::LoadU64(address + 0); }
  void set_bits(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t bits_address() const { return address + 0; }
  // the sign, exponent and the mantissa's top 20 bits
  uint32_t high() const { return guest::LoadU32(address + 0); }
  void set_high(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t high_address() const { return address + 0; }
  uint16_t sign_exponent() const { return guest::LoadU16(address + 0); }
  void set_sign_exponent(uint16_t v) const { guest::StoreU16(address + 0, v); }
  uint32_t sign_exponent_address() const { return address + 0; }
  uint32_t low() const { return guest::LoadU32(address + 4); }
  void set_low(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t low_address() const { return address + 4; }
};

// embedded; measure: sub_825B0068.r7, sub_825B0068.r8
struct FpOperand : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  double as_double() const { return std::bit_cast<double>(guest::LoadU64(address + 0)); }
  void set_as_double(double v) const { guest::StoreU64(address + 0, std::bit_cast<uint64_t>(v)); }
  uint32_t as_double_address() const { return address + 0; }
  float as_float() const { return guest::LoadFloat(address + 0); }
  void set_as_float(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t as_float_address() const { return address + 0; }
};

// embedded; measure: sub_825B0068.r3
struct FpIeeeRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // probably the rounding mode, precision and operation
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // probably (the exception's cause bits)
  uint32_t cause() const { return guest::LoadU32(address + 4); }
  void set_cause(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t cause_address() const { return address + 4; }
  // probably
  uint32_t enable() const { return guest::LoadU32(address + 8); }
  void set_enable(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t enable_address() const { return address + 8; }
  // probably
  uint32_t status() const { return guest::LoadU32(address + 12); }
  void set_status(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t status_address() const { return address + 12; }
};

// embedded; measure: sub_825B0068.r4
struct FpControlWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: _decomp.r4
struct ExponentOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  int32_t exponent() const { return int32_t(guest::LoadU32(address + 0)); }
  void set_exponent(int32_t v) const { guest::StoreU32(address + 0, uint32_t(v)); }
  uint32_t exponent_address() const { return address + 0; }
};

// embedded; measure: strtoxl.r5
struct EndPointerOut : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = CString>
  T end() const { return T(guest::LoadU32(address + 0)); }
  void set_end(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_end(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t end_address() const { return address + 0; }
};

// allocated; measure: [82978330]
struct ExitTable : View {
  using View::View;
  static constexpr uint32_t kSize = 128;
  // read 4 bytes below the table by the exit path (sub_825AA218, never run; probably its walk down from the end)
  uint32_t unknown_m4() const { return guest::LoadU32(address + -4); }
  void set_unknown_m4(uint32_t v) const { guest::StoreU32(address + -4, v); }
  uint32_t unknown_m4_address() const { return address + -4; }
  // the first function registered (0 when made)
  uint32_t first() const { return guest::LoadU32(address + 0); }
  void set_first(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t first_address() const { return address + 0; }
};

// embedded; measure: strncmp.r3, strncmp.r4, strstr.r4, strncpy.r4, _stricmp.r3, mbtowc.r4, _cftof_l.r4, strtoxl.r4, _output_l.r4, _fltout2.r5, new<861>@_output_l, new<861>@_woutput_l, [820734CC], [8266C024].+0, lstrcmpA.r3, lstrcmpA.r4, CompareStringA.r6
struct CString : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  uint32_t chars_address() const { return address + 0; }
};

// embedded; measure: wcscmp.r3, wcscmp.r4, wcscat.r3, wcscat.r4, wcscpy.r3, wcscpy.r4, wcslen.r3, mbtowc.r3, _woutput_l.r4, [820734D0]
struct WideString : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  // halfwords
  uint32_t units_address() const { return address + 0; }
};

// embedded; measure: memcpy_825A7410.r3, memcpy_825A7410.r4, memcpy_825A7830.r3, memcpy_825A7830.r4, XMemCpy_82295890.r3, XMemCpy_82295890.r4, XMemCpy_82296FE0.r3, XMemCpy_82296FE0.r4, XMemSet.r3, XPR.+16.+12
struct ByteBuffer : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  uint32_t bytes_address() const { return address + 0; }
};

// The image words docs/functions/c-runtime.md's code uses.
struct CRuntimeGlobals {
  // "(null)", printed for a null %s
  template <typename T = CString>
  static T narrow_null() { return T(guest::LoadU32(0x820734CCu)); }
  static void set_narrow_null(const View& v) { guest::StoreU32(0x820734CCu, v.address); }
  static void set_narrow_null(uint32_t v) { guest::StoreU32(0x820734CCu, v); }
  static constexpr uint32_t narrow_null_address() { return 0x820734CCu; }
  // L"(null)"
  template <typename T = WideString>
  static T wide_null() { return T(guest::LoadU32(0x820734D0u)); }
  static void set_wide_null(const View& v) { guest::StoreU32(0x820734D0u, v.address); }
  static void set_wide_null(uint32_t v) { guest::StoreU32(0x820734D0u, v); }
  static constexpr uint32_t wide_null_address() { return 0x820734D0u; }
  // the printf engines' table: a character's class (from ' ', the low nibble) and, at class × 8 + state, the next state (the high nibble)
  static constexpr uint32_t format_states_address() { return 0x820734D8u; }
  // "1#QNAN" (probably)
  static constexpr uint32_t qnan_text_address() { return 0x82074490u; }
  // "1#INF" (probably)
  static constexpr uint32_t inf_text_address() { return 0x82074498u; }
  // "1#IND" (probably)
  static constexpr uint32_t ind_text_address() { return 0x820744A0u; }
  // "1#SNAN" (probably)
  static constexpr uint32_t snan_text_address() { return 0x820744A8u; }
  // XMemCpy's byte loop's step (1)
  static uint32_t xmemcpy_step() { return guest::LoadU32(0x82638308u); }
  static void set_xmemcpy_step(uint32_t v) { guest::StoreU32(0x82638308u, v); }
  static constexpr uint32_t xmemcpy_step_address() { return 0x82638308u; }
  // XMemSet's step (16)
  static uint32_t xmemset_stride() { return guest::LoadU32(0x8263830Cu); }
  static void set_xmemset_stride(uint32_t v) { guest::StoreU32(0x8263830Cu, v); }
  static constexpr uint32_t xmemset_stride_address() { return 0x8263830Cu; }
  // _aexit_rtn: a KeBugCheck, then _exit (0x825AA378) once _init_pointers ran
  static uint32_t aexit_rtn() { return guest::LoadU32(0x8266B7B0u); }
  static void set_aexit_rtn(uint32_t v) { guest::StoreU32(0x8266B7B0u, v); }
  static constexpr uint32_t aexit_rtn_address() { return 0x8266B7B0u; }
  // _iob: 20 Streams of 32 bytes (stdin, stdout, stderr first); __iob_func returns it
  static constexpr uint32_t streams_address() { return 0x8266B7D0u; }
  // _cfltcvt_tab: a FloatConversionTable
  static constexpr uint32_t float_conversions_address() { return 0x8266BA5Cu; }
  // the C locale's data: a Locale
  static constexpr uint32_t c_locale_address() { return 0x8266BB48u; }
  template <typename T = Locale>
  static T current_locale() { return T(guest::LoadU32(0x8266BC28u)); }
  static void set_current_locale(const View& v) { guest::StoreU32(0x8266BC28u, v.address); }
  static void set_current_locale(uint32_t v) { guest::StoreU32(0x8266BC28u, v); }
  static constexpr uint32_t current_locale_address() { return 0x8266BC28u; }
  // __lconv (probably): its decimal point
  template <typename T = Lconv>
  static T lconv() { return T(guest::LoadU32(0x8266C024u)); }
  static void set_lconv(const View& v) { guest::StoreU32(0x8266C024u, v.address); }
  static void set_lconv(uint32_t v) { guest::StoreU32(0x8266C024u, v); }
  static constexpr uint32_t lconv_address() { return 0x8266C024u; }
  // the C locale's multibyte info: an MbcInfo
  static constexpr uint32_t c_mbc_info_address() { return 0x8266C028u; }
  // the current multibyte info (the C locale's at boot)
  template <typename T = MbcInfo>
  static T mbc_info() { return T(guest::LoadU32(0x8266C450u)); }
  static void set_mbc_info(const View& v) { guest::StoreU32(0x8266C450u, v.address); }
  static void set_mbc_info(uint32_t v) { guest::StoreU32(0x8266C450u, v); }
  static constexpr uint32_t mbc_info_address() { return 0x8266C450u; }
  // _pow10pos: 12-byte long doubles in rows of seven by the exponent's three-bit groups
  static constexpr uint32_t pow10_positive_address() { return 0x8266C548u; }
  // _pow10neg
  static constexpr uint32_t pow10_negative_address() { return 0x8266C6A8u; }
  // a C runtime handler pointer (sub_825AEE18 sets it; 0 at boot)
  static uint32_t unknown_82971178() { return guest::LoadU32(0x82971178u); }
  static void set_unknown_82971178(uint32_t v) { guest::StoreU32(0x82971178u, v); }
  static constexpr uint32_t unknown_82971178_address() { return 0x82971178u; }
  // probably: 0x825B3498, stored by sub_825B3548
  static uint32_t default_critical_section_initializer() { return guest::LoadU32(0x82971328u); }
  static void set_default_critical_section_initializer(uint32_t v) { guest::StoreU32(0x82971328u, v); }
  static constexpr uint32_t default_critical_section_initializer_address() { return 0x82971328u; }
  // probably: read by sub_825B35A0; 0 at boot
  static uint32_t critical_section_initializer() { return guest::LoadU32(0x8297132Cu); }
  static void set_critical_section_initializer(uint32_t v) { guest::StoreU32(0x8297132Cu, v); }
  static constexpr uint32_t critical_section_initializer_address() { return 0x8297132Cu; }
  // probably __ismbcodepage: 0 from __initmbctable
  static uint32_t mbc_is_set() { return guest::LoadU32(0x8297135Cu); }
  static void set_mbc_is_set(uint32_t v) { guest::StoreU32(0x8297135Cu, v); }
  static constexpr uint32_t mbc_is_set_address() { return 0x8297135Cu; }
  // probably __mbcodepage: -3 (_MB_CP_ANSI) from __initmbctable
  static int32_t mbc_code_page() { return int32_t(guest::LoadU32(0x8297136Cu)); }
  static void set_mbc_code_page(int32_t v) { guest::StoreU32(0x8297136Cu, uint32_t(v)); }
  static constexpr uint32_t mbc_code_page_address() { return 0x8297136Cu; }
  // _invalid_parameter's handler, cleared at boot, never set
  static uint32_t invalid_parameter_handler() { return guest::LoadU32(0x82978320u); }
  static void set_invalid_parameter_handler(uint32_t v) { guest::StoreU32(0x82978320u, v); }
  static constexpr uint32_t invalid_parameter_handler_address() { return 0x82978320u; }
  // atexit's table: one past the last function
  static uint32_t exit_table_end() { return guest::LoadU32(0x82978330u); }
  static void set_exit_table_end(uint32_t v) { guest::StoreU32(0x82978330u, v); }
  static constexpr uint32_t exit_table_end_address() { return 0x82978330u; }
  template <typename T = ExitTable>
  static T exit_table_begin() { return T(guest::LoadU32(0x82978334u)); }
  static void set_exit_table_begin(const View& v) { guest::StoreU32(0x82978334u, v.address); }
  static void set_exit_table_begin(uint32_t v) { guest::StoreU32(0x82978334u, v); }
  static constexpr uint32_t exit_table_begin_address() { return 0x82978334u; }
  // __initmbctable's done flag
  static uint32_t mbctable_done() { return guest::LoadU32(0x82978338u); }
  static void set_mbctable_done(uint32_t v) { guest::StoreU32(0x82978338u, v); }
  static constexpr uint32_t mbctable_done_address() { return 0x82978338u; }
};

}  // namespace schema
