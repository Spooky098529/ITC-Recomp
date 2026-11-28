// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 52's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "c_runtime/core.h"
#include "c_runtime/exit.h"
#include "c_runtime/format.h"
#include "c_runtime/math.h"
#include "c_runtime/parse.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "hooks/c_runtime.h"
#include "hooks/c_runtime_core.h"
#include "hooks/scenery_lists.h"
#include "scenery_lists/scenery_lists.h"
#include "schema/c_runtime.h"

#include <optional>

// --- c_runtime ---

// _onexit: r3 = the function; r3 = it, or 0 when the table can't take it.
// Natively where atexit's adapter is (the exit lock a host mutex, the C
// runtime's heap on our layer); else, or where the port declines, the
// original.
ITC_DIRECT_ENTRY(825A6F60) {
  if (hooks::NativeAtexit()) {
    if (const auto result = c_runtime::OnExit(ctx.r3.u32)) {
      ctx.r3.u64 = *result;
      return;
    }
  }
  lifted::original::c825A6F60(ctx);
}

// _vsprintf_l: r3 = the buffer, r4 = the format, r5 = the locale, r6 = the
// va_list; r3 = the count (zero-extended), or -1. Natively only with no
// locale (as vsprintf's tail call): the port's _output_l frame is 128 and
// 1,328 bytes below r1, as the original's. Else, or where the port
// declines, the original.
ITC_DIRECT_ENTRY(825A70F0) {
  if (ctx.r5.u32 == 0) {
    if (const auto r3 = c_runtime::core::Vsprintf(ctx.r3.u32, ctx.r4.u32, ctx.r6.u32,
                                                  ctx.r1.u32, hooks::CRuntimeCoreCalls(ctx))) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c825A70F0(ctx);
}

// wcscmp: r3, r4 = the strings; r3 = -1, 0 or 1 (sign-extended).
ITC_DIRECT_ENTRY(825A7EB0) { ctx.r3.s64 = c_runtime::Wcscmp(ctx.r3.u32, ctx.r4.u32); }

// strtoxl: r3 = the locale (not read), r4 = the string, r5 = the end
// pointer out (or null), r6 = the base, r7 = the flags; r3 the value. Where
// the port declines (_errno's paths, a multibyte locale), the original.
ITC_DIRECT_ENTRY(825A82C8) {
  uint32_t end = 0;
  uint64_t result = 0;
  const uint32_t end_out = ctx.r5.u32;
  if (!c_runtime::StrToXl(ctx.r4.u32, ctx.r6.s32, ctx.r7.u32, end_out != 0, &end, &result)) {
    lifted::original::c825A82C8(ctx);
    return;
  }
  if (end_out != 0) {
    guest::StoreU32(end_out, end);
  }
  ctx.r3.u64 = result;
}

// _vsnprintf_l: r3 = the buffer, r4 = its size, r5 = the format, r6 = the
// locale, r7 = the va_list; r3 = the count (zero-extended). Natively only
// with no locale (as _vsnprintf's tail call): _output_l's frame 144 and
// 1,328 bytes below r1. Else, or where the port declines, the original.
ITC_DIRECT_ENTRY(825AAF18) {
  int32_t count = 0;
  if (ctx.r6.u32 != 0 ||
      !c_runtime::Vsnprintf(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r7.u32,
                            ctx.r1.u32 - 144 - 1328, &count)) {
    lifted::original::c825AAF18(ctx);
    return;
  }
  ctx.r3.u64 = uint32_t(count);
}

// _atof_l: r3 = the string, r4 = the locale (not read: the original reads
// the current one); f1 = the double. Where the port declines, the original.
ITC_DIRECT_ENTRY(825AB0C0) {
  uint64_t bits;
  if (!c_runtime::AtofL(ctx.r3.u32, &bits)) {
    lifted::original::c825AB0C0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.u64 = bits;
}

// __ascii_strnicmp: r3, r4 = the strings, r5 = the count; r3 the difference
// (sign-extended).
ITC_DIRECT_ENTRY(825AC6B0) {
  ctx.r3.s64 = c_runtime::AsciiStrnicmp(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// _cftof2_l: r3 = the buffer, r4 = its size, r5 = the precision, r6 =
// _fltout2's result (+0 the sign, +4 the decimal exponent), r7 = %g's flag
// (a byte), r8 = the locale (not read: the decimal point is the current
// locale's); r3 = 0. Natively as core.cpp's Cftof2Guest; where the writes
// would share memory with what it reads, the original.
ITC_DIRECT_ENTRY(825AE708) {
  const uint32_t buffer = ctx.r3.u32, result = ctx.r6.u32;
  c_runtime::GuestText text(buffer);
  bool native = buffer != 0 && ctx.r4.u32 != 0;
  if (native) {
    uint32_t words[3];
    const uint32_t point = c_runtime::DecimalPointAddress(words);
    const schema::FloatStrings strings(result);
    native = c_runtime::Cftof2(text, ctx.r5.s32, int32_t(strings.sign()),
                               strings.decimal_exponent(), ctx.r7.s8 != 0,
                               guest::LoadU8(point));
    const uint32_t end = uint32_t(text.WrittenEnd());
    native = native && !c_runtime::MayShare(result, 8, buffer, end) &&
             !c_runtime::MayShare(words[0], 4, buffer, end) &&
             !c_runtime::MayShare(words[1], 4, buffer, end) &&
             !c_runtime::MayShare(point, 1, buffer, end) && text.Commit();
  }
  if (!native) {
    lifted::original::c825AE708(ctx);
    return;
  }
  ctx.r3.u64 = 0;
}

// _output_l: r3 = the stream (+0 its pointer, +4 its room, +8 its buffer,
// +12 its flags), r4 = the format, r5 = the locale, r6 = the va_list; its
// frame 1,328 bytes below r1; r3 = the count (sign-extended). Natively only
// on a string stream (flag 0x40) whose pointer is its buffer (as
// _vsnprintf_l and _vsprintf_l make it), with a format and no locale, where
// the output lands on neither the stream nor what the port reads: the
// stream's pointer and room stored back. Else, or where the port declines,
// the original.
ITC_DIRECT_ENTRY(825AF130) {
  const uint32_t stream = ctx.r3.u32;
  bool native = stream != 0 && ctx.r4.u32 != 0 && ctx.r5.u32 == 0 &&
                (guest::LoadU32(stream + 12) & 0x40) != 0 &&
                guest::LoadU32(stream) == guest::LoadU32(stream + 8);
  int32_t count = 0;
  uint32_t end = 0;
  int32_t room = 0;
  if (native) {
    const uint32_t buffer = guest::LoadU32(stream);
    native = c_runtime::NarrowOutputToString(stream, buffer,
                                             int32_t(guest::LoadU32(stream + 4)), ctx.r4.u32,
                                             ctx.r6.u32, ctx.r1.u32 - 1328, &count, &end, &room);
  }
  if (!native) {
    lifted::original::c825AF130(ctx);
    return;
  }
  guest::StoreU32(stream, end);
  guest::StoreU32(stream + 4, uint32_t(room));
  ctx.r3.s64 = count;
}

// _set_exp: f1 = x, r4 = the exponent; f1 the result.
ITC_DIRECT_ENTRY(825AFE80) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::SetExp(ctx.f1.f64, ctx.r4.s32);
}

// rint: f1 = x; f1 rounded in the host's rounding mode.
ITC_DIRECT_ENTRY(825B0B30) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = c_runtime::Rint(ctx.f1.f64);
}

// __strgtold12_l: r3 = the 12-byte out, r4 = the end pointer out, r5 = the
// string, r6 to r9 = mult12, scale, decpt and implicit_E, r10 = the locale
// (null: _errno's path, the original's); r3 = the flags. Where the port
// declines, the original.
ITC_DIRECT_ENTRY(825B36B8) {
  c_runtime::Strgtold12Result result;
  if (ctx.r10.u32 == 0 || !c_runtime::Strgtold12(ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                                 ctx.r8.u32, ctx.r9.u32, &result)) {
    lifted::original::c825B36B8(ctx);
    return;
  }
  guest::StoreU32(ctx.r4.u32, result.end);
  const uint32_t out = ctx.r3.u32;
  guest::StoreU16(out + 10, result.value.extra);
  guest::StoreU32(out + 6, result.value.value.mantissa_low);
  guest::StoreU32(out + 2, result.value.value.mantissa_high);
  guest::StoreU16(out, result.value.value.sign_exponent);
  ctx.r3.u64 = result.flags;
}

// _fptostr: r3 = the buffer, r4 = its size, r5 = the digits, r6 =
// _fltout2's result; r3 = 0, 22 or 34 (the original, a guest call, where
// the writes would share memory with what it reads).
ITC_DIRECT_ENTRY(825B61E8) {
  ctx.r3.u64 = c_runtime::core::FpToStrGuest(ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r6.u32,
                                             hooks::CRuntimeCoreCalls(ctx));
}

// _fltout2: r3 = the double's bits, r4 = the result, r5 = the buffer, r6 =
// its size; r3 = the result.
ITC_DIRECT_ENTRY(825B6458) {
  ctx.r3.u64 = c_runtime::core::Fltout2(ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                        hooks::CRuntimeCoreCalls(ctx));
}

// _ld12tod: r3 = the 12-byte long double, r4 = the double out; r3 = 0, 1
// or 2 (sign-extended).
ITC_DIRECT_ENTRY(825B69B8) {
  const uint32_t in = ctx.r3.u32;
  const c_runtime::LongDouble12 value = {guest::LoadU16(in), guest::LoadU32(in + 2),
                                         guest::LoadU32(in + 6)};
  int32_t status;
  const uint64_t bits = c_runtime::Ld12ToDouble(value, guest::LoadU16(in + 10), &status);
  guest::StoreU32(ctx.r4.u32 + 4, uint32_t(bits));
  guest::StoreU32(ctx.r4.u32, uint32_t(bits >> 32));
  ctx.r3.s64 = status;
}

// --- scenery_lists: their register steps (hooks/scenery_lists.h) ---

ITC_DIRECT_ENTRY(82146AA8) { hooks::SceneryListsResetDrawState(ctx); }
ITC_DIRECT_ENTRY(82147850) { hooks::SceneryListsIsBEntryOn(ctx); }
ITC_DIRECT_ENTRY(82147958) { hooks::SceneryListsIsCEntryOn(ctx); }
ITC_DIRECT_ENTRY(82147B18) { hooks::SceneryListsPointListIsEntryOn(ctx); }
ITC_DIRECT_ENTRY(82147D88) { hooks::SceneryListsDrawShape(ctx); }
ITC_DIRECT_ENTRY(82148428) { hooks::SceneryListsModelListLoad(ctx); }
ITC_DIRECT_ENTRY(82148528) { hooks::SceneryListsModelListLoadPoints(ctx); }
ITC_DIRECT_ENTRY(82148650) { hooks::SceneryListsSetBillboardPass(ctx); }
ITC_DIRECT_ENTRY(82589970) { hooks::SceneryListsDepthVertexDeclaration(ctx); }
ITC_DIRECT_ENTRY(82596B18) { hooks::SceneryListsModelDrawShape(ctx); }
ITC_DIRECT_ENTRY(82596D48) { hooks::SceneryListsDrawShapeDepth(ctx); }
// SceneLights::RecordProjectors and RecordDirections: r3 = the out (no
// steps; RecordSceneLights' plain C++ calls them).
ITC_DIRECT_ENTRY(82589B58) { scenery_lists::SceneLightsRecordProjectors(ctx.r3.u32); }
ITC_DIRECT_ENTRY(8258B980) { scenery_lists::SceneLightsRecordDirections(ctx.r3.u32); }
