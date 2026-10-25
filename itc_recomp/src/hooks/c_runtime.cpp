// The game's statically linked C runtime (docs/functions/c-runtime.md): the
// stack probe dropped, memset natively, and adapters in front of the string
// and character helpers in src/c_runtime/strings.cpp, and in front of the
// compiler's array helpers in src/c_runtime/arrays.cpp, and atexit and the
// exit lock in src/c_runtime/exit.cpp.

#include "guest.h"
#include "hooks/c_runtime.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"

#include "c_runtime/arrays.h"
#include "c_runtime/exit.h"
#include "c_runtime/format.h"
#include "c_runtime/parse.h"
#include "c_runtime/start_up.h"
#include "c_runtime/strings.h"

#include "base/flags.h"

#include <cstring>




ITC_FLAG_BOOL(native_c_runtime, true, "ITC",
              "Run the C runtime's helpers natively instead of through the translated code: "
              "memset (sub_825A71D0); XAPI's vectorized memset (sub_822962B0, XMemSet); wcslen, wcscpy, wcscat and wcscmp "
              "(sub_825A9958, sub_825A7F30, sub_825A7EF8, sub_825A7EB0); mbtowc's single-byte "
              "path (sub_825B7EC8); strncpy, strcpy_s, strncmp, _stricmp, _strnicmp and its compare, "
              "strstr (sub_825A7F50, sub_825A9DD8, sub_825AACD0, sub_825AA100, sub_825AC710, "
              "sub_825AC6B0, sub_825A98A8); the second memcpy (sub_825A7830); memmove and memcpy_s "
              "(sub_825AA840, sub_825A7270); memmove_s (sub_825A7348); toupper, tolower, "
              "isalpha, isdigit, isalnum and isleadbyte (sub_825A8600, sub_825A8618, sub_825AB020, "
              "sub_825AB040, sub_825AB0A0, sub_825B6898); strtoul and strtoxl (sub_825A85E0, "
              "sub_825A82C8); ___dtold and _ld12tod (sub_825B6378, sub_825B69B8); atof, _atof_l, "
              "_fltin2, __strgtold12_l and __mtold12 (sub_825AB1C0, sub_825AB0C0, sub_825AFDC8, "
              "sub_825B36B8, sub_825B7F10); _fltout2 and $I10_OUTPUT (sub_825B6458, sub_825B8B48); %f's "
              "_cfltcvt_l, _cftof_l, _cftof2_l and _fptostr (sub_825AEB20, sub_825AE8E0, sub_825AE708, "
              "sub_825B61E8); write_char "
              "(sub_825AF000), write_multi_char (sub_825AF098), the wide write_char and _putwc_nolock "
              "(sub_825B1368, sub_825B7A80); __iob_func (sub_825AD4C0); the start-up's _cfltcvt_init (sub_825A6DB0, "
              "sub_825A6E48), _init_pointers' setters (sub_825AD140, sub_825AEE18, sub_825AFCB8, "
              "sub_825B3560, sub_825B3548), __initmbctable and setSBCS (sub_825B6958, sub_825B68C8); "
              "the compiler's vector constructor and destructor iterators (sub_820911A8, "
              "sub_820900C0: the elements' native functions called as plain C++, the empty "
              "destructor sub_82106540 skipped, any other called in the translated code); "
              "the exit lock (the C runtime's lock 8) as a host mutex for every user, through "
              "_lock sub_825B3430 and _unlock sub_825B32D0 for lock 8 (chosen at start-up, "
              "other locks the game's), and with --native_heap and --native_c_runtime_heap "
              "atexit (sub_825A7068) and _onexit (sub_825A6F60) over the game's table, and "
              "the table's set-up __onexitinit (sub_825A6F00) by the native calloc "
              "(checked by "
              "--run_harnesses=c_runtime). Those reached only from native callers have no "
              "adapters, and the flag switches their callers: wcslen, wcscmp, mbtowc, the wide write_char, "
              "strcpy_s, __ascii_strnicmp, "
              "strtoxl, ___dtold, _ld12tod, _atof_l, _fltin2, __strgtold12_l, __mtold12, $I10_OUTPUT, "
              "_fltout2, _cftof_l, _cftof2_l, _fptostr, _putwc_nolock, _cfltcvt_init, setSBCS "
              "and _onexit");

ITC_FLAG_BOOL(native_memcpy, true, "ITC",
              "Copy natively in the C runtime's memcpy (sub_825A7410), inside its override in "
              "src/native/buffers.cpp that records copies for the renderer's memory mirrors: the "
              "host's memcpy where the two ranges don't share memory, the translated code "
              "otherwise (checked by --run_harnesses=c_runtime)");

ITC_FLAG_DECLARE(bool, native_c_runtime_heap);

bool hooks::CRuntimeExitHostLock() {
  static const bool host = ITC_FLAG(native_c_runtime);
  return host;
}

bool hooks::NativeAtexit() {
  return ITC_FLAG(native_c_runtime) && hooks::CRuntimeExitHostLock() &&
         ITC_FLAG(native_c_runtime_heap) && hooks::NativeHeapActive();
}

// --- The exit lock ---
//
// For lock 8 only, whatever --native_c_runtime says now
// (hooks::CRuntimeExitHostLock is fixed for the run, from the first _lock
// call at boot): every user of it takes the host mutex, the recompiled ones
// (the exit path) here. Other locks go to the originals. Lock 8 is made at
// boot (its table entry's +4 is 1), so the original's lazy creation
// (sub_825B32F0) is never its path.

// _lock: r3 = the lock number; r3 = its critical section (the original's
// argument to RtlEnterCriticalSection, which leaves r3).
extern "C" REX_FUNC(sub_825B3430) {
  REX_FUNC_PROLOGUE();
  if (hooks::CRuntimeExitHostLock() && ctx.r3.u32 == c_runtime::kExitLock) {
    ctx.r3.u64 = c_runtime::LockExit(base);
    return;
  }
  __imp__sub_825B3430(ctx, base);
}

// _unlock: r3 = the lock number; r3 = its critical section, as _lock's.
extern "C" REX_FUNC(sub_825B32D0) {
  REX_FUNC_PROLOGUE();
  if (hooks::CRuntimeExitHostLock() && ctx.r3.u32 == c_runtime::kExitLock) {
    ctx.r3.u64 = c_runtime::UnlockExit(base);
    return;
  }
  __imp__sub_825B32D0(ctx, base);
}

// atexit: r3 = the function; returns 0, or -1 when the table can't take it.
// _onexit (sub_825A6F60) has no adapter: only atexit calls it.
extern "C" REX_FUNC(sub_825A7068) {
  REX_FUNC_PROLOGUE();
  if (hooks::NativeAtexit()) {
    if (const auto result = c_runtime::Atexit(base, ctx.r3.u32)) {
      ctx.r3.s64 = *result;
      return;
    }
  }
  __imp__sub_825A7068(ctx, base);
}

// __onexitinit: no arguments; r3 = 0, or 24 when calloc fails. Natively
// with --native_c_runtime and the C runtime's heap on the layer (the native
// calloc).
extern "C" REX_FUNC(sub_825A6F00) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_c_runtime) && ITC_FLAG(native_c_runtime_heap) &&
      hooks::NativeHeapActive()) {
    if (const auto result = c_runtime::InitExitTable(base)) {
      ctx.r3.u64 = *result;
      return;
    }
  }
  __imp__sub_825A6F00(ctx, base);
}

int32_t hooks::GuestAtexit::Run(void* self, uint32_t function) {
  auto& call = *static_cast<GuestAtexit*>(self);
  call.ctx.r3.u64 = function;
  __imp__sub_825A7068(call.ctx, call.base);
  return call.ctx.r3.s32;
}

// __chkstk: r12 = minus the frame about to be made (4 KB or more). Reads one
// word every 4,096 bytes below r1 down to the new frame, touching the guard
// pages on the 360; the guest stack here is ordinary memory, so the reads do
// nothing and are left out. The callers only use r12 and r1 after it, both
// unchanged either way. Dropped: no fallback to the original.
extern "C" REX_FUNC(sub_825A9934) {
  REX_FUNC_PROLOGUE();
}

// memset: r3 = destination, r4 = the value (its low byte), r5 = the count
// (32 bits); returns r3. The original stores bytes up to a word boundary,
// then words, then bytes: the same bytes in the end.
extern "C" REX_FUNC(sub_825A71D0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A71D0(ctx, base);
    return;
  }
  const uint8_t value = ctx.r4.u8;
  guest::ForEachHostPart(base, ctx.r3.u32, ctx.r5.u32,
                         [&](uint8_t* host, uint32_t, uint32_t bytes) {
                           std::memset(host, value, bytes);
                         });
}

// XAPI's vectorized memset (probably XMemSet): r3 = destination, r4 = the
// value (its low byte), r5 = the count (32 bits); r3 comes back unchanged.
// The original's scratch below r1 (its vector and counters, dead after it
// returns) isn't written.
extern "C" REX_FUNC(sub_822962B0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_822962B0(ctx, base);
    return;
  }
  c_runtime::XMemSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// wcscpy: r3 = destination, r4 = source; returns r3 (left as it is).
extern "C" REX_FUNC(sub_825A7F30) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A7F30(ctx, base);
    return;
  }
  c_runtime::Wcscpy(base, ctx.r3.u32, ctx.r4.u32);
}

// wcscat: r3 = destination, r4 = source; returns r3 (left as it is).
extern "C" REX_FUNC(sub_825A7EF8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A7EF8(ctx, base);
    return;
  }
  c_runtime::Wcscat(base, ctx.r3.u32, ctx.r4.u32);
}

// strncpy: r3 = destination, r4 = source, r5 = count; returns r3.
extern "C" REX_FUNC(sub_825A7F50) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A7F50(ctx, base);
    return;
  }
  c_runtime::Strncpy(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// strncmp: r3, r4 = strings, r5 = count.
extern "C" REX_FUNC(sub_825AACD0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AACD0(ctx, base);
    return;
  }
  ctx.r3.s64 = c_runtime::Strncmp(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32);
}

// _stricmp: r3, r4 = strings.
extern "C" REX_FUNC(sub_825AA100) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AA100(ctx, base);
    return;
  }
  ctx.r3.s64 = c_runtime::Stricmp(base, ctx.r3.u32, ctx.r4.u32);
}

// _strnicmp: r3, r4 = strings, r5 = count. A null string or a count above
// INT_MAX (EINVAL through _errno, never seen in play) stays on the
// recompiled path.
extern "C" REX_FUNC(sub_825AC710) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime) || ctx.r3.u32 == 0 || ctx.r4.u32 == 0 ||
      ctx.r5.u32 > 0x7FFFFFFFu) {
    __imp__sub_825AC710(ctx, base);
    return;
  }
  ctx.r3.s64 = c_runtime::AsciiStrnicmp(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// strstr: r3 = string, r4 = what to find; returns the match (r3 moved on by
// its offset, as the original's register is) or 0.
extern "C" REX_FUNC(sub_825A98A8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A98A8(ctx, base);
    return;
  }
  const int64_t offset = c_runtime::Strstr(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r3.u64 = offset < 0 ? 0 : ctx.r3.u64 + uint64_t(offset);
}

// The second memcpy: r3 = destination, r4 = source, r5 = count; returns r3.
// Like the original, not seen by the native renderer's memory mirrors
// (unlike sub_825A7410's wrapper).
extern "C" REX_FUNC(sub_825A7830) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A7830(ctx, base);
    return;
  }
  c_runtime::CopyForward(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// memmove: r3 = destination, r4 = source, r5 = count; returns r3 (left as it
// is). Equal (as signed words, the original's cmpw) does nothing; a
// destination below the source is the original's tail call to memcpy (its
// override, which the mirrors' bookkeeping sees); above it, the backward
// copy natively where c_runtime::MemmoveDown can tell it gives the same
// bytes, else the original. The original also leaves r0, r4, r5, r7 to r10,
// ctr and cr0 changed; no caller reads them before setting them.
extern "C" REX_FUNC(sub_825AA840) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AA840(ctx, base);
    return;
  }
  if (ctx.r3.s32 == ctx.r4.s32) {
    return;
  }
  if (ctx.r3.s32 < ctx.r4.s32) {
    sub_825A7410(ctx, base);
    return;
  }
  if (!c_runtime::MemmoveDown(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_825AA840(ctx, base);
  }
}

// memmove_s: r3 = destination, r4 = its size, r5 = source, r6 = count; r3 =
// 0. The paths c_runtime::MemmoveS leaves to the original (its errors, a
// copy memcpy's override records for the mirrors, a copy its checks refuse)
// run it from the start, nothing having been copied.
extern "C" REX_FUNC(sub_825A7348) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_c_runtime)) {
    if (const auto r3 = c_runtime::MemmoveS(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                            ctx.r6.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_825A7348(ctx, base);
}

// memcpy_s: r3 = destination, r4 = its size, r5 = source, r6 = count;
// returns 0. A count of 0 does nothing; otherwise the copy is memcpy's (its
// override, called from where the original calls it, so the mirrors'
// bookkeeping sees the same caller). A null destination or source or a
// count above the size (memset, then EINVAL or ERANGE through _errno and
// _invalid_parameter, never seen in play) stays on the recompiled path. Its
// callers read no register but r3 after it.
extern "C" REX_FUNC(sub_825A7270) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A7270(ctx, base);
    return;
  }
  if (ctx.r6.u32 == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  if (ctx.r3.u32 == 0 || ctx.r5.u32 == 0 || ctx.r4.u32 < ctx.r6.u32) {
    __imp__sub_825A7270(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.r4.u64 = ctx.r5.u64;
  ctx.r5.u64 = ctx.r6.u64;
  ctx.lr = 0x825A72F0;
  sub_825A7410(ctx, base);
  ctx.lr = lr;
  ctx.r3.u64 = 0;
}

// toupper and tolower: r3 = character; the step added to the whole
// register, as the originals do.
extern "C" REX_FUNC(sub_825A8600) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A8600(ctx, base);
    return;
  }
  ctx.r3.u64 += uint64_t(int64_t(c_runtime::ToUpperStep(ctx.r3.s32)));
}

extern "C" REX_FUNC(sub_825A8618) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A8618(ctx, base);
    return;
  }
  ctx.r3.u64 += uint64_t(int64_t(c_runtime::ToLowerStep(ctx.r3.s32)));
}

// isalpha, isdigit and isalnum: r3 = character; the C locale's table.
extern "C" REX_FUNC(sub_825AB020) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AB020(ctx, base);
    return;
  }
  ctx.r3.u64 = c_runtime::CTypeC(base, ctx.r3.u32, 0x103);
}

extern "C" REX_FUNC(sub_825AB040) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AB040(ctx, base);
    return;
  }
  ctx.r3.u64 = c_runtime::CTypeC(base, ctx.r3.u32, 4);
}

extern "C" REX_FUNC(sub_825AB0A0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AB0A0(ctx, base);
    return;
  }
  ctx.r3.u64 = c_runtime::CTypeC(base, ctx.r3.u32, 0x107);
}

// isleadbyte: r3 = character; the current locale's table.
extern "C" REX_FUNC(sub_825B6898) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825B6898(ctx, base);
    return;
  }
  ctx.r3.u64 = c_runtime::IsLeadByte(base, ctx.r3.u32);
}

// strtoxl's work for strtoul (strtoxl itself has no adapter: strtoul, its
// only caller in play, is native): the string, the end pointer out, the
// base and the flags. Its _errno paths and a multibyte locale stay on the
// recompiled path.
namespace {

bool StrToXl(uint8_t* base, uint32_t string, uint32_t end_out, int32_t radix, uint32_t flags,
             uint64_t* result) {
  uint32_t end = 0;
  if (!c_runtime::StrToXl(base, string, radix, flags, end_out != 0, &end, result)) {
    return false;
  }
  if (end_out != 0) {
    guest::StoreU32(base, end_out, end);
  }
  return true;
}

}  // namespace

// strtoul: r3 = string, r4 = end pointer out, r5 = base; strtoxl unsigned.
extern "C" REX_FUNC(sub_825A85E0) {
  REX_FUNC_PROLOGUE();
  uint64_t result = 0;
  if (!ITC_FLAG(native_c_runtime) ||
      !StrToXl(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, 1, &result)) {
    __imp__sub_825A85E0(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

// atof: r3 = the string; returns the double in f1 (_atof_l's work: _atof_l
// has no adapter, as atof is its only caller in play). A null string and a
// multibyte locale's space test run the original. Its callers read only f1
// after it (the original leaves r3 pointing into its finished frame).
namespace {

bool Atof(PPCContext& ctx, uint8_t* base) {
  uint64_t bits;
  if (!ITC_FLAG(native_c_runtime) || !c_runtime::AtofL(base, ctx.r3.u32, &bits)) {
    return false;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.u64 = bits;
  return true;
}

}  // namespace

extern "C" REX_FUNC(sub_825AB1C0) {
  REX_FUNC_PROLOGUE();
  if (!Atof(ctx, base)) {
    __imp__sub_825AB1C0(ctx, base);
  }
}

// _cftof_l's work for _cfltcvt_l (_cftof_l has no adapter: _cfltcvt_l's 'f'
// is its only call in play). r3 = the double's address, r4 = the buffer, r5
// = its size, r6 = the precision, r7 = the locale (not used); returns 0.
// c_runtime::Cftof does _fltout2, _fptostr and _cftof2_l natively; where it
// can't (the error paths through _errno, never seen in play; a buffer in the
// physical views or across two views), the original runs. The original
// leaves r4 to r12 changed; its callers (_cfltcvt_l's and sub_825AEB70's
// tail calls, so _output_l) read none of them. (Its frame, with _fltout2's
// result and digits, is below the caller's stack pointer: not written.)
namespace {
void CftofL(PPCContext& ctx, uint8_t* base) {
  if (!c_runtime::Cftof(base, REX_LOAD_U64(ctx.r3.u32), ctx.r4.u32, ctx.r5.u32, ctx.r6.s32)) {
    __imp__sub_825AE8E0(ctx, base);
    return;
  }
  ctx.r3.u64 = 0;
}
}  // namespace

// _cfltcvt_l: r3 = the double's address, r4 = the buffer, r5 = its size, r6
// = the format character, r7 = the precision, r8 = caps, r9 = the locale.
// The original's dispatch, with its register moves: 'e' and 'E' to
// sub_825AE1A8, 'f' to _cftof_l (natively, above), 'a' and 'A' to
// sub_825AE2C0, the rest ('g', 'G') to sub_825AE9D0; only 'f' was seen in
// play, the others stay recompiled. Its one caller, _output_l's indirect
// call through the table at 0x8266BA5C, reads no register after it.
extern "C" REX_FUNC(sub_825AEB20) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AEB20(ctx, base);
    return;
  }
  const int32_t format = ctx.r6.s32;
  ctx.r11.u64 = ctx.r6.u64;
  ctx.r6.u64 = ctx.r7.u64;
  ctx.r7.u64 = ctx.r8.u64;
  ctx.r8.u64 = ctx.r9.u64;
  if (format == 'e' || format == 'E') {
    sub_825AE1A8(ctx, base);
  } else if (format == 'f') {
    ctx.r7.u64 = ctx.r8.u64;
    CftofL(ctx, base);
  } else if (format == 'a' || format == 'A') {
    sub_825AE2C0(ctx, base);
  } else {
    sub_825AE9D0(ctx, base);
  }
}

// write_char: r3 = the character, r4 = the stream, r5 = the count. Where
// the original would call _flsbuf (no room left; never seen in play), the
// original runs instead. r3 as the original leaves it: the byte when stored.
extern "C" REX_FUNC(sub_825AF000) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AF000(ctx, base);
    return;
  }
  const int32_t done = c_runtime::WriteChar(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  if (done < 0) {
    __imp__sub_825AF000(ctx, base);
    return;
  }
  if (done == 1) {
    ctx.r3.u64 = ctx.r3.u32 & 0xFF;
  }
}

// write_multi_char: r3 = the string, r4 = its length, r5 = the stream, r6 =
// the count. Where the original might reach _flsbuf or _errno (never seen in
// play), or the buffer lands on the stream or the count, the original runs.
// r3 is left as the original leaves it (the last character written, when
// any); its callers read no register it changes.
extern "C" REX_FUNC(sub_825AF098) {
  REX_FUNC_PROLOGUE();
  int32_t last;
  if (!ITC_FLAG(native_c_runtime) ||
      !c_runtime::WriteMultiChar(base, ctx.r3.u32, ctx.r4.s32, ctx.r5.u32, ctx.r6.u32, &last)) {
    __imp__sub_825AF098(ctx, base);
    return;
  }
  if (last >= 0) {
    ctx.r3.u64 = uint32_t(last);
  }
}

// __iob_func: returns the stream table (sign-extended, as the original's
// lis and addi leave it).
extern "C" REX_FUNC(sub_825AD4C0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AD4C0(ctx, base);
    return;
  }
  ctx.r3.s64 = int32_t(c_runtime::kStreams);
}

// The start-up's pieces (src/c_runtime/start_up.cpp), each run once at boot.

// Branches to _cfltcvt_init.
extern "C" REX_FUNC(sub_825A6E48) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A6E48(ctx, base);
    return;
  }
  c_runtime::InitFloatConversions(base);
}

// _init_pointers' setters: r3 = the value.
extern "C" REX_FUNC(sub_825AD140) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AD140(ctx, base);
    return;
  }
  c_runtime::SetHandler82971170(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_825AEE18) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AEE18(ctx, base);
    return;
  }
  c_runtime::SetHandler82971178(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_825AFCB8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AFCB8(ctx, base);
    return;
  }
  c_runtime::SetInvalidParameterHandler(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_825B3560) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825B3560(ctx, base);
    return;
  }
  c_runtime::SetCriticalSectionInitializer(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_825B3548) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825B3548(ctx, base);
    return;
  }
  c_runtime::SetDefaultCriticalSectionInitializer(base);
}

// __initmbctable: returns 0.
extern "C" REX_FUNC(sub_825B6958) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825B6958(ctx, base);
    return;
  }
  ctx.r3.s64 = c_runtime::InitMbcTable(base);
}


// --- The compiler's array helpers (src/c_runtime/arrays.cpp) ---

namespace {

struct ArrayCall {
  PPCContext* ctx;
  uint8_t* base;
  uint32_t return_address;  // after the original's bctrl
};

// An element's function the native code doesn't know (sub_82101D20, the
// race data's records, in play), called through its pointer from a frame
// like the original's (128 bytes, its back chain written), r3 = the element.
void CallElement(void* user, uint64_t function, uint64_t element) {
  auto& call = *static_cast<ArrayCall*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  guest_call::InFrame(ctx, base, 128, call.return_address, [&](uint32_t) {
    ctx.r3.u64 = element;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  });
}

}  // namespace

// `vector constructor iterator': r3 = the array, r4 = the element size,
// r5 = the count, r6 = the constructor.
extern "C" REX_FUNC(sub_820911A8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_820911A8(ctx, base);
    return;
  }
  ArrayCall call{&ctx, base, 0x820911D4};
  c_runtime::VectorConstruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, CallElement,
                             &call);
}

// `vector destructor iterator': r3 = the array, r4 = the element size,
// r5 = the count, r6 = the destructor.
extern "C" REX_FUNC(sub_820900C0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_820900C0(ctx, base);
    return;
  }
  ArrayCall call{&ctx, base, 0x820900F4};
  c_runtime::VectorDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, CallElement,
                            &call);
}
