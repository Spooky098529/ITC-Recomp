// The game's statically linked C runtime (docs/functions/c-runtime.md): the
// stack probe dropped, memset natively, and adapters in front of the string
// and character helpers in src/c_runtime/strings.cpp.

#include "guest.h"

#include "c_runtime/strings.h"

#include "base/flags.h"

#include <cstring>

ITC_FLAG_BOOL(native_c_runtime, true, "ITC",
              "Run the C runtime's helpers natively instead of through the translated code: "
              "the stack probe (__chkstk, sub_825A9934) is skipped, as its reads of the guest "
              "stack have no effect; memset (sub_825A71D0); wcslen, wcscpy, wcscat and wcscmp "
              "(sub_825A9958, sub_825A7F30, sub_825A7EF8, sub_825A7EB0); mbtowc's single-byte "
              "path (sub_825B7EC8); strncpy, strcpy_s, strncmp, _stricmp, _strnicmp and its compare, "
              "strstr (sub_825A7F50, sub_825A9DD8, sub_825AACD0, sub_825AA100, sub_825AC710, "
              "sub_825AC6B0, sub_825A98A8); the second memcpy (sub_825A7830); toupper, tolower, "
              "isalpha, isdigit, isalnum and isleadbyte (sub_825A8600, sub_825A8618, sub_825AB020, "
              "sub_825AB040, sub_825AB0A0, sub_825B6898); strtoul and strtoxl (sub_825A85E0, "
              "sub_825A82C8) (checked by "
              "--run_harnesses=c_runtime)");

// __chkstk: r12 = minus the frame about to be made (4 KB or more). Reads one
// word every 4,096 bytes below r1 down to the new frame, touching the guard
// pages on the 360; the guest stack here is ordinary memory, so the reads do
// nothing and are left out. The callers only use r12 and r1 after it, both
// unchanged either way.
extern "C" REX_FUNC(sub_825A9934) {
  REX_FUNC_PROLOGUE();

  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A9934(ctx, base);
  }
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

// wcslen: r3 = UTF-16 string; returns its length.
extern "C" REX_FUNC(sub_825A9958) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A9958(ctx, base);
    return;
  }
  ctx.r3.s64 = c_runtime::Wcslen(base, ctx.r3.u32);
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

// wcscmp: r3, r4 = UTF-16 strings; returns -1, 0 or 1.
extern "C" REX_FUNC(sub_825A7EB0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825A7EB0(ctx, base);
    return;
  }
  ctx.r3.s64 = c_runtime::Wcscmp(base, ctx.r3.u32, ctx.r4.u32);
}

// mbtowc (the C locale's single-byte path): r3 = wide character out (or 0),
// r4 = the bytes, r5 = their count; returns 0 or 1.
extern "C" REX_FUNC(sub_825B7EC8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825B7EC8(ctx, base);
    return;
  }
  ctx.r3.s64 = c_runtime::Mbtowc(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
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

// strcpy_s: r3 = destination, r4 = its size, r5 = source; returns 0. Its
// error paths (_errno and _invalid_parameter, never seen in play) and an
// overlapping copy upward stay on the recompiled path. (The original also
// saves the link register and r31 below the stack pointer: not done here.)
extern "C" REX_FUNC(sub_825A9DD8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime) ||
      !c_runtime::StrcpyS(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_825A9DD8(ctx, base);
    return;
  }
  ctx.r3.u64 = 0;
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

// __ascii_strnicmp: r3, r4 = strings, r5 = count.
extern "C" REX_FUNC(sub_825AC6B0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_c_runtime)) {
    __imp__sub_825AC6B0(ctx, base);
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

// strtoxl: r3 = locale (unused: the original reads the current one), r4 =
// string, r5 = end pointer out, r6 = base, r7 = flags. Its _errno paths and
// a multibyte locale stay on the recompiled path. (The original also saves
// r23-r31 below the stack pointer: not done here.)
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

extern "C" REX_FUNC(sub_825A82C8) {
  REX_FUNC_PROLOGUE();
  uint64_t result = 0;
  if (!ITC_FLAG(native_c_runtime) ||
      !StrToXl(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32, ctx.r7.u32, &result)) {
    __imp__sub_825A82C8(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

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
