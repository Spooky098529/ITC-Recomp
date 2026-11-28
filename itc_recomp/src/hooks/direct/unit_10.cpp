// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 10's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "c_runtime/format.h"
#include "guest.h"
#include "hooks/heap.h"
#include "hooks/texture_compression.h"
#include "texture_compression/blitter.h"
#include "texture_compression/compress_surface.h"

namespace tc = texture_compression;

// --- texture_compression ---

// Blitter::Blitter: r3 = the blit object; r3 left.
ITC_DIRECT_ENTRY(82368D20) { tc::BlitterConstruct(ctx.r3.u32); }
// Blitter::Release: r3 = the blit object, its deletes at r1 - 96; r3 the
// last delete's (or 0). The original without the native heap.
ITC_DIRECT_ENTRY(82368D30) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c82368D30(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const tc::GuestCall call = hooks::TextureCompressionGuestCall(ctx);
  const uint32_t stack = ctx.r1.u32;
  ctx.r3.u64 = tc::BlitterRelease(ctx.r3.u32, stack, call);
}
// Blitter::Init: r3 = the blit object, r4, r5 = the destination's and the
// source's descriptions, r6 = the filter; r3 sign-extended (its errors made
// by lis). The original without the native heap.
ITC_DIRECT_ENTRY(8236C630) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c8236C630(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const tc::GuestCall call = hooks::TextureCompressionGuestCall(ctx);
  const uint32_t blitter = ctx.r3.u32, destination = ctx.r4.u32, source = ctx.r5.u32,
                 filter = ctx.r6.u32, stack = ctx.r1.u32;
  ctx.r3.s64 = int32_t(tc::BlitterInit(blitter, destination, source, filter, stack, call));
}
// XGGetBlockDimensions: r3 = the GPU format, r4, r5 = the width's and
// height's addresses (or null); r3 left (the format).
ITC_DIRECT_ENTRY(8236D3C8) { tc::GetBlockDimensions(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32); }

// --- c_runtime ---

// _woutput_l: r3 = the stream (+0 its pointer, +4 its count, +12 its
// flags), r4 = the format, r5 = the locale, r6 = the va_list; its frame
// 2,336 bytes below. Native only on a fresh string stream of INT_MAX bytes
// (as _vswprintf_l makes it) with a format and no locale: the stream's
// pointer and count stored back, the count of characters returned. Else, or where the port
// declines, the original.
ITC_DIRECT_ENTRY(825B13D0) {
  const uint32_t stream = ctx.r3.u32;
  int32_t count = 0;
  uint32_t end = 0;
  int32_t room = 0;
  if (stream == 0 || ctx.r4.u32 == 0 || ctx.r5.u32 != 0 ||
      (guest::LoadU32(stream + 12) & 0x40) == 0 ||
      guest::LoadU32(stream + 4) != 0x7FFFFFFF ||
      !c_runtime::WideOutputToString(guest::LoadU32(stream), ctx.r4.u32, ctx.r6.u32,
                                     ctx.r1.u32 - 2336, &count, &end, &room)) {
    lifted::original::c825B13D0(ctx);
    return;
  }
  guest::StoreU32(stream, end);
  guest::StoreU32(stream + 4, uint32_t(room));
  ctx.r3.s64 = count;
}
