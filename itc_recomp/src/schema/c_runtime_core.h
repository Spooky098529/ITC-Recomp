// The views of docs/functions/c-runtime-core.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/c-runtime-core.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: _errno.ret
struct ErrnoSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  int32_t value() const { return int32_t(guest::LoadU32(address + 0)); }
  void set_value(int32_t v) const { guest::StoreU32(address + 0, uint32_t(v)); }
  uint32_t value_address() const { return address + 0; }
  // measure fault: _fptostr's and _cftoa_l's r3 after the _errno call is their buffer, not errno
  uint64_t fault_buffer_double() const { return guest::LoadU64(address + 0); }
  void set_fault_buffer_double(uint64_t v) const { guest::StoreU64(address + 0, v); }
  uint32_t fault_buffer_double_address() const { return address + 0; }
  // measure fault: as fault_buffer_double
  uint8_t fault_buffer_0() const { return guest::LoadU8(address + 0); }
  void set_fault_buffer_0(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t fault_buffer_0_address() const { return address + 0; }
  // measure fault: as fault_buffer_double
  uint8_t fault_buffer_1() const { return guest::LoadU8(address + 1); }
  void set_fault_buffer_1(uint8_t v) const { guest::StoreU8(address + 1, v); }
  uint32_t fault_buffer_1_address() const { return address + 1; }
  // measure fault: as fault_buffer_double
  uint8_t fault_buffer_2() const { return guest::LoadU8(address + 2); }
  void set_fault_buffer_2(uint8_t v) const { guest::StoreU8(address + 2, v); }
  uint32_t fault_buffer_2_address() const { return address + 2; }
};

// embedded; measure: __doserrno.ret
struct DosErrnoSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // the Windows error last mapped
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// image; measure: -
struct ExceptionAction : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // XcptNum
  uint32_t exception_code() const { return guest::LoadU32(address + 0); }
  void set_exception_code(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t exception_code_address() const { return address + 0; }
  // SigNum: 8 for the SIGFPE entries
  int32_t signal() const { return int32_t(guest::LoadU32(address + 4)); }
  void set_signal(int32_t v) const { guest::StoreU32(address + 4, uint32_t(v)); }
  uint32_t signal_address() const { return address + 4; }
  // XcptAction: 0 (SIG_DFL), 1 (SIG_IGN) or a handler
  uint32_t action() const { return guest::LoadU32(address + 8); }
  void set_action(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t action_address() const { return address + 8; }
};

// image; measure: _getptd_noexit.ret.+92
struct ExceptionActionTable : View {
  using View::View;
  static constexpr uint32_t kSize = 132;
  // 11 ExceptionActions of 12 bytes, searched by signal by raise
  uint32_t entries_address() const { return address + 0; }
};

// image; measure: -
struct CrtLockEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the critical section (28 bytes), 0 when not made
  uint32_t section() const { return guest::LoadU32(address + 0); }
  void set_section(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // 1: given one of 0x829711A0's sections by _mtinitlocks, never freed
  uint32_t made_at_boot() const { return guest::LoadU32(address + 4); }
  void set_made_at_boot(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t made_at_boot_address() const { return address + 4; }
};

// allocated; measure: -
struct CrtIoInfo : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // osfhnd: -1 none, -2 for the first three
  uint32_t os_handle() const { return guest::LoadU32(address + 0); }
  void set_os_handle(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t os_handle_address() const { return address + 0; }
  // bit 0 open (FOPEN), 0x02 EOF, 0x20 append, 0x40 device, 0x80 text; 0xC1 for the first three
  uint8_t osfile() const { return guest::LoadU8(address + 4); }
  void set_osfile(uint8_t v) const { guest::StoreU8(address + 4, v); }
  uint32_t osfile_address() const { return address + 4; }
  // pipech: 10 (none)
  uint8_t pipe_char() const { return guest::LoadU8(address + 5); }
  void set_pipe_char(uint8_t v) const { guest::StoreU8(address + 5, v); }
  uint32_t pipe_char_address() const { return address + 5; }
  // lockinitflag: the critical section made (counted by __lock_fhandle)
  uint32_t lock_made() const { return guest::LoadU32(address + 8); }
  void set_lock_made(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t lock_made_address() const { return address + 8; }
  // the critical section __lock_fhandle enters
  uint32_t lock_address() const { return address + 12; }
  // the text-mode bits (textmode in the bits above bit 0, unicode bit 0, probably)
  uint8_t text_mode() const { return guest::LoadU8(address + 40); }
  void set_text_mode(uint8_t v) const { guest::StoreU8(address + 40, v); }
  uint32_t text_mode_address() const { return address + 40; }
  // pipech2: 10 and 10
  uint32_t pipe_char2_address() const { return address + 41; }
};

// allocated; measure: [82978220]
struct CrtIoInfoBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 1408;
  // 32 CrtIoInfos of 44 bytes (calloc(32, 44) by _ioinit)
  uint32_t entries_address() const { return address + 0; }
};

// image; measure: [8263879C]
struct ThreadNotifyBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the next block (the head at the end)
  uint32_t next() const { return guest::LoadU32(address + 0); }
  void set_next(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  uint32_t previous() const { return guest::LoadU32(address + 4); }
  void set_previous(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  // called with r3 1 at a thread's start, 0 at its end
  uint32_t routine() const { return guest::LoadU32(address + 8); }
  void set_routine(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t routine_address() const { return address + 8; }
};

// image; measure: _initterm.r3
struct CrtInitTable : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 0: skipped
  uint32_t function() const { return guest::LoadU32(address + 0); }
  void set_function(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t function_address() const { return address + 0; }
};

// image; measure: _CxxThrowException.r4
struct CxxThrowInfo : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // bit 3 (8): a pure (/clr:pure) throw, magic 0x01994000
  uint32_t attributes() const { return guest::LoadU32(address + 0); }
  void set_attributes(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t attributes_address() const { return address + 0; }
};

// embedded; measure: __CxxUnhandledExceptionFilter.r3.+0
struct ExceptionRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  // 0xE06D7363 for a C++ exception
  uint32_t code() const { return guest::LoadU32(address + 0); }
  void set_code(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t code_address() const { return address + 0; }
  uint32_t flags() const { return guest::LoadU32(address + 4); }
  void set_flags(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t flags_address() const { return address + 4; }
  // the chained record
  uint32_t record() const { return guest::LoadU32(address + 8); }
  void set_record(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t record_address() const { return address + 8; }
  // where it was raised (ExceptionAddress; not "address", which would hide View::address)
  uint32_t exception_address() const { return guest::LoadU32(address + 12); }
  void set_exception_address(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t exception_address_address() const { return address + 12; }
  // 3 for a C++ exception
  uint32_t parameter_count() const { return guest::LoadU32(address + 16); }
  void set_parameter_count(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t parameter_count_address() const { return address + 16; }
  // the first parameter: a C++ exception's magic (0x19930520 to 0x19930522, 0x01994000)
  uint32_t magic() const { return guest::LoadU32(address + 20); }
  void set_magic(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t magic_address() const { return address + 20; }
  // the second: the thrown object
  uint32_t object() const { return guest::LoadU32(address + 24); }
  void set_object(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t object_address() const { return address + 24; }
  // the third
  template <typename T = CxxThrowInfo>
  T throw_info() const { return T(guest::LoadU32(address + 28)); }
  void set_throw_info(const View& v) const { guest::StoreU32(address + 28, v.address); }
  void set_throw_info(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t throw_info_address() const { return address + 28; }
};

// embedded; measure: __CxxUnhandledExceptionFilter.r3
struct ExceptionPointers : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = ExceptionRecord>
  T exception_record() const { return T(guest::LoadU32(address + 0)); }
  void set_exception_record(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_exception_record(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t exception_record_address() const { return address + 0; }
  uint32_t context() const { return guest::LoadU32(address + 4); }
  void set_context(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t context_address() const { return address + 4; }
};

// image; measure: std.+0
struct StdExceptionVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // probably the scalar deleting destructor
  uint32_t destructor() const { return guest::LoadU32(address + 0); }
  void set_destructor(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t destructor_address() const { return address + 0; }
  // what(): the message
  uint32_t what() const { return guest::LoadU32(address + 4); }
  void set_what(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t what_address() const { return address + 4; }
};

// embedded; measure: -
struct CrtOverlapped : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the status: 259 (STATUS_PENDING) while pending
  uint32_t internal() const { return guest::LoadU32(address + 0); }
  void set_internal(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t internal_address() const { return address + 0; }
  // the bytes moved
  uint32_t internal_high() const { return guest::LoadU32(address + 4); }
  void set_internal_high(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t internal_high_address() const { return address + 4; }
  // the file position's low word
  uint32_t offset() const { return guest::LoadU32(address + 8); }
  void set_offset(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t offset_address() const { return address + 8; }
  // and its high word
  uint32_t offset_high() const { return guest::LoadU32(address + 12); }
  void set_offset_high(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t offset_high_address() const { return address + 12; }
  // hEvent
  uint32_t event() const { return guest::LoadU32(address + 16); }
  void set_event(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t event_address() const { return address + 16; }
};

// image; measure: -
struct DebugMonitorData : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // called with a notification number and a value (10 for an unhandled exception, 39 for a fatal app exit)
  uint32_t notify() const { return guest::LoadU32(address + 24); }
  void set_notify(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t notify_address() const { return address + 24; }
};

// The image words docs/functions/c-runtime-core.md's code uses.
struct CRuntimeCoreGlobals {
  // _initterm_e's table: three function pointers (XAPI's, probably), called until one returns nonzero
  static constexpr uint32_t xapi_initializers_address() { return 0x82600000u; }
  // doexit's pre-terminators (_initterm, 0x82600294 to 0x8260029C)
  static constexpr uint32_t pre_terminators_address() { return 0x82600294u; }
  // doexit's terminators (0x826002A0 to 0x826002A4)
  static constexpr uint32_t terminators_address() { return 0x826002A0u; }
  // ExLoadedCommandLine (kernel variable import 0x1AE): GetCommandLineA's answer
  static uint32_t loaded_command_line() { return guest::LoadU32(0x820007ECu); }
  static void set_loaded_command_line(uint32_t v) { guest::StoreU32(0x820007ECu, v); }
  static constexpr uint32_t loaded_command_line_address() { return 0x820007ECu; }
  // KeDebugMonitorData's address (kernel variable import 0x59); the word there points at a DebugMonitorData, 0 with no debugger
  static uint32_t debug_monitor_data() { return guest::LoadU32(0x820007F0u); }
  static void set_debug_monitor_data(uint32_t v) { guest::StoreU32(0x820007F0u, v); }
  static constexpr uint32_t debug_monitor_data_address() { return 0x820007F0u; }
  // 0.0, _positive's compare
  static double zero_double() { return std::bit_cast<double>(guest::LoadU64(0x82002258u)); }
  static void set_zero_double(double v) { guest::StoreU64(0x82002258u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t zero_double_address() { return 0x82002258u; }
  // "e+000", %e's exponent copied by _cftoe2_l
  static constexpr uint32_t exponent_text_address() { return 0x82072F88u; }
  // called at a thread's start by the C runtime's notify routine, when set
  static uint32_t thread_start_hook() { return guest::LoadU32(0x820723A0u); }
  static void set_thread_start_hook(uint32_t v) { guest::StoreU32(0x820723A0u, v); }
  static constexpr uint32_t thread_start_hook_address() { return 0x820723A0u; }
  // and at its end
  static uint32_t thread_end_hook() { return guest::LoadU32(0x820723A4u); }
  static void set_thread_end_hook(uint32_t v) { guest::StoreU32(0x820723A4u, v); }
  static constexpr uint32_t thread_end_hook_address() { return 0x820723A4u; }
  // the thread notify list's critical section
  static constexpr uint32_t notify_lock_address() { return 0x82638780u; }
  // the thread notify list's head: +0 the first ThreadNotifyBlock, +4 the last (itself when empty)
  static constexpr uint32_t notify_list_address() { return 0x8263879Cu; }
  // errtable: 45 pairs (Windows error, errno)
  static constexpr uint32_t os_error_table_address() { return 0x8266B4F0u; }
  // errno without thread data
  static int32_t errno_fallback() { return int32_t(guest::LoadU32(0x8266B658u)); }
  static void set_errno_fallback(int32_t v) { guest::StoreU32(0x8266B658u, uint32_t(v)); }
  static constexpr uint32_t errno_fallback_address() { return 0x8266B658u; }
  // _doserrno without thread data
  static uint32_t doserrno_fallback() { return guest::LoadU32(0x8266B65Cu); }
  static void set_doserrno_fallback(uint32_t v) { guest::StoreU32(0x8266B65Cu, v); }
  static constexpr uint32_t doserrno_fallback_address() { return 0x8266B65Cu; }
  // __flsindex: the thread data's TLS index, -1 when freed
  static uint32_t fls_index() { return guest::LoadU32(0x8266B7B4u); }
  static void set_fls_index(uint32_t v) { guest::StoreU32(0x8266B7B4u, v); }
  static constexpr uint32_t fls_index_address() { return 0x8266B7B4u; }
  // __getvalueindex: the getter's TLS index, -1 when freed
  static uint32_t getvalue_index() { return guest::LoadU32(0x8266B7B8u); }
  static void set_getvalue_index(uint32_t v) { guest::StoreU32(0x8266B7B8u, v); }
  static constexpr uint32_t getvalue_index_address() { return 0x8266B7B8u; }
  // __abort_behavior: 3 in the image (bit 0 write the message, bit 1 report the fault)
  static uint32_t abort_behavior() { return guest::LoadU32(0x8266B7C4u); }
  static void set_abort_behavior(uint32_t v) { guest::StoreU32(0x8266B7C4u, v); }
  static constexpr uint32_t abort_behavior_address() { return 0x8266B7C4u; }
  // rterrs: 23 pairs (number, text), _NMSG_WRITE's
  static constexpr uint32_t run_time_errors_address() { return 0x8266BA88u; }
  // +infinity (exp's overflow)
  static double positive_infinity() { return std::bit_cast<double>(guest::LoadU64(0x8266BC30u)); }
  static void set_positive_infinity(double v) { guest::StoreU64(0x8266BC30u, std::bit_cast<uint64_t>(v)); }
  static constexpr uint32_t positive_infinity_address() { return 0x8266BC30u; }
  // _locktable: 36 CrtLockEntries
  static constexpr uint32_t lock_table_address() { return 0x8266BD40u; }
  // _XcptActTab: 11 ExceptionActions, each thread's data +92 points here
  static constexpr uint32_t exception_actions_address() { return 0x8266BE60u; }
  // _First_FPE_Indx: the first SIGFPE entry
  static int32_t first_fpe_index() { return int32_t(guest::LoadU32(0x8266BED8u)); }
  static void set_first_fpe_index(int32_t v) { guest::StoreU32(0x8266BED8u, uint32_t(v)); }
  static constexpr uint32_t first_fpe_index_address() { return 0x8266BED8u; }
  // _Num_FPE
  static int32_t fpe_count() { return int32_t(guest::LoadU32(0x8266BEDCu)); }
  static void set_fpe_count(int32_t v) { guest::StoreU32(0x8266BEDCu, uint32_t(v)); }
  static constexpr uint32_t fpe_count_address() { return 0x8266BEDCu; }
  // _XcptActTabCount (11)
  static int32_t exception_action_count() { return int32_t(guest::LoadU32(0x8266BEE4u)); }
  static void set_exception_action_count(int32_t v) { guest::StoreU32(0x8266BEE4u, uint32_t(v)); }
  static constexpr uint32_t exception_action_count_address() { return 0x8266BEE4u; }
  // __badioinfo: the CrtIoInfo of handles -1 and -2
  static constexpr uint32_t bad_io_info_address() { return 0x8266BFB8u; }
  // _exitflag: doexit's "return to caller"
  static uint8_t exit_flag() { return guest::LoadU8(0x82971144u); }
  static void set_exit_flag(uint8_t v) { guest::StoreU8(0x82971144u, v); }
  static constexpr uint32_t exit_flag_address() { return 0x82971144u; }
  // _C_Termination_Done
  static int32_t c_termination_done() { return int32_t(guest::LoadU32(0x82971148u)); }
  static void set_c_termination_done(int32_t v) { guest::StoreU32(0x82971148u, uint32_t(v)); }
  static constexpr uint32_t c_termination_done_address() { return 0x82971148u; }
  // _C_Exit_Done
  static int32_t c_exit_done() { return int32_t(guest::LoadU32(0x8297114Cu)); }
  static void set_c_exit_done(int32_t v) { guest::StoreU32(0x8297114Cu, uint32_t(v)); }
  static constexpr uint32_t c_exit_done_address() { return 0x8297114Cu; }
  // the C runtime's ThreadNotifyBlock (routine 0x825B3650), registered by _mtinit
  static constexpr uint32_t crt_notify_block_address() { return 0x82971150u; }
  // the FLS stand-ins _mtinit sets: __crtTlsAlloc (0x825AA3F8)
  static uint32_t fls_alloc() { return guest::LoadU32(0x8297115Cu); }
  static void set_fls_alloc(uint32_t v) { guest::StoreU32(0x8297115Cu, v); }
  static constexpr uint32_t fls_alloc_address() { return 0x8297115Cu; }
  // KeTlsGetValue's thunk (0x825DBBF4)
  static uint32_t fls_get_value() { return guest::LoadU32(0x82971160u); }
  static void set_fls_get_value(uint32_t v) { guest::StoreU32(0x82971160u, v); }
  static constexpr uint32_t fls_get_value_address() { return 0x82971160u; }
  // KeTlsSetValue's thunk (0x825DBC04)
  static uint32_t fls_set_value() { return guest::LoadU32(0x82971164u); }
  static void set_fls_set_value(uint32_t v) { guest::StoreU32(0x82971164u, v); }
  static constexpr uint32_t fls_set_value_address() { return 0x82971164u; }
  // KeTlsFree's thunk (0x825DBC24)
  static uint32_t fls_free() { return guest::LoadU32(0x82971168u); }
  static void set_fls_free(uint32_t v) { guest::StoreU32(0x82971168u, v); }
  static constexpr uint32_t fls_free_address() { return 0x82971168u; }
  // _cflush: counted by _stbuf and _getbuf
  static uint32_t cflush() { return guest::LoadU32(0x82971174u); }
  static void set_cflush(uint32_t v) { guest::StoreU32(0x82971174u, v); }
  static constexpr uint32_t cflush_address() { return 0x82971174u; }
  // the lock table's static critical sections (28 bytes each, up to 14: the entries made at boot, in order; entry 8's at 0x82971248)
  static constexpr uint32_t lock_sections_address() { return 0x829711A0u; }
  // _stdbuf: the console streams' two shared buffers (malloc(4096) when first used)
  static constexpr uint32_t std_buffers_address() { return 0x82971330u; }
  // the signal actions raise reads and resets under _lock(0)
  static uint32_t sigint_action() { return guest::LoadU32(0x82971338u); }
  static void set_sigint_action(uint32_t v) { guest::StoreU32(0x82971338u, v); }
  static constexpr uint32_t sigint_action_address() { return 0x82971338u; }
  static uint32_t sigbreak_action() { return guest::LoadU32(0x8297133Cu); }
  static void set_sigbreak_action(uint32_t v) { guest::StoreU32(0x8297133Cu, v); }
  static constexpr uint32_t sigbreak_action_address() { return 0x8297133Cu; }
  // __get_sigabrt's
  static uint32_t sigabrt_action() { return guest::LoadU32(0x82971340u); }
  static void set_sigabrt_action(uint32_t v) { guest::StoreU32(0x82971340u, v); }
  static constexpr uint32_t sigabrt_action_address() { return 0x82971340u; }
  static uint32_t sigterm_action() { return guest::LoadU32(0x82971344u); }
  static void set_sigterm_action(uint32_t v) { guest::StoreU32(0x82971344u, v); }
  static constexpr uint32_t sigterm_action_address() { return 0x82971344u; }
  // the unhandled exception filter before __CxxUnhandledExceptionFilter (0)
  static uint32_t cxx_previous_filter() { return guest::LoadU32(0x82971348u); }
  static void set_cxx_previous_filter(uint32_t v) { guest::StoreU32(0x82971348u, v); }
  static constexpr uint32_t cxx_previous_filter_address() { return 0x82971348u; }
  // _outputformat: bit 0 (_TWO_DIGIT_EXPONENT)
  static uint32_t output_format() { return guest::LoadU32(0x82971358u); }
  static void set_output_format(uint32_t v) { guest::StoreU32(0x82971358u, v); }
  static constexpr uint32_t output_format_address() { return 0x82971358u; }
  // SetUnhandledExceptionFilter's word
  static uint32_t unhandled_exception_filter() { return guest::LoadU32(0x829714A0u); }
  static void set_unhandled_exception_filter(uint32_t v) { guest::StoreU32(0x829714A0u, v); }
  static constexpr uint32_t unhandled_exception_filter_address() { return 0x829714A0u; }
  // _debugger_hook_dummy: _crt_debugger_hook stores 0
  static uint32_t debugger_hook_dummy() { return guest::LoadU32(0x8297820Cu); }
  static void set_debugger_hook_dummy(uint32_t v) { guest::StoreU32(0x8297820Cu, v); }
  static constexpr uint32_t debugger_hook_dummy_address() { return 0x8297820Cu; }
  // _nhandle
  static uint32_t handle_count() { return guest::LoadU32(0x82978210u); }
  static void set_handle_count(uint32_t v) { guest::StoreU32(0x82978210u, v); }
  static constexpr uint32_t handle_count_address() { return 0x82978210u; }
  // __pioinfo: 64 pointers to CrtIoInfoBlocks (the first made by _ioinit)
  static constexpr uint32_t io_info_blocks_address() { return 0x82978220u; }
  // __piob: the stream pointers (_nstream of them, the first 20 at _iob's)
  static uint32_t stream_table() { return guest::LoadU32(0x82978324u); }
  static void set_stream_table(uint32_t v) { guest::StoreU32(0x82978324u, v); }
  static constexpr uint32_t stream_table_address() { return 0x82978324u; }
  // _nstream (512)
  static int32_t stream_count() { return int32_t(guest::LoadU32(0x82978328u)); }
  static void set_stream_count(int32_t v) { guest::StoreU32(0x82978328u, uint32_t(v)); }
  static constexpr uint32_t stream_count_address() { return 0x82978328u; }
};

}  // namespace schema
