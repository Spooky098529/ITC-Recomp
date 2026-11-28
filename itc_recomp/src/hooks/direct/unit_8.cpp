// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 8's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"
#include "guest.h"
#include "lifted/originals.h"

#include "c_runtime/format.h"
#include "c_runtime/parse.h"
#include "graphics/command_buffer.h"
#include "graphics/device_state.h"
#include "graphics/resources.h"
#include "track_collision/track_collision.h"

// --- graphics ---

// sub_8229B7D8: r3 the device, r4 the bits (+10788 bits 12-13).
ITC_DIRECT_ENTRY(8229B7D8) {
  graphics::device_state::SetRegisterF01Bits(ctx.r3.u32, ctx.r4.u32);
}
// Device::WriteSegmentTail: r3 the device, r4 the write position; returns
// r4 + 40.
ITC_DIRECT_ENTRY(8229E768) {
  ctx.r3.u64 = graphics::command_buffer::WriteSegmentTail(ctx.r3.u32, ctx.r4.u32);
}
// Device::SetSurfaceInfo: r3 the device, r4 the surface info.
ITC_DIRECT_ENTRY(822A3C88) {
  ctx.fpscr.disableFlushMode();
  graphics::device_state::SetSurfaceInfo(ctx.r3.u32, ctx.r4.u32);
}
// Texture::Format: r3 the texture; returns the format word.
ITC_DIRECT_ENTRY(822A44A8) { ctx.r3.u64 = graphics::TextureFormat(ctx.r3.u32); }
// TextureLayout::LevelCount: r3, r4, r5 = width, height, depth, r6 = the
// border.
ITC_DIRECT_ENTRY(822A4530) {
  ctx.r3.u64 =
      graphics::TextureLevelCount(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}
// TextureLayout::Sizes: r3-r5 width, height, depth, r6 levels, r7 format,
// r8 dimension, r9 tiled, r10 packed; the caller's stack: +84 the border,
// +100 the pitch, +108, +116, +124 pointers to the base width, base size
// and mip size out. Returns the levels (the deleted adapter's mapping).
ITC_DIRECT_ENTRY(822A46C8) {
  const uint32_t stack = ctx.r1.u32;
  const graphics::TextureSizes sizes = graphics::TextureLayoutSizes(
      ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
      ctx.r10.u32, guest::LoadU32(stack + 84), guest::LoadU32(stack + 100));
  guest::StoreU32(guest::LoadU32(stack + 108), sizes.base_width);
  guest::StoreU32(guest::LoadU32(stack + 116), sizes.base_size);
  guest::StoreU32(guest::LoadU32(stack + 124), sizes.mip_size);
  ctx.r3.u64 = sizes.levels;
}
// Texture::LevelLayout: r3 the texture, r4 the level; out: r5 the width, r6
// the height, r7 the depth, r8 the pitch, r9 the slice. r3 left (the
// original leaves LevelSize's, which its only caller, LevelDesc, doesn't
// read).
ITC_DIRECT_ENTRY(822A4B98) {
  const graphics::LevelLayout layout = graphics::TextureLevelLayout(ctx.r3.u32, ctx.r4.u32);
  guest::StoreU32(ctx.r5.u32, layout.width);
  guest::StoreU32(ctx.r6.u32, layout.height);
  guest::StoreU32(ctx.r7.u32, layout.depth);
  guest::StoreU32(ctx.r8.u32, layout.pitch);
  guest::StoreU32(ctx.r9.u32, layout.slice);
}
// Texture::LockLevel: r3 the texture, r4 the face, r5 the level, r6 the
// flags; out: r7 the address, r8 the pitch, r9 the slice, r10 the tail
// offset; returns the lock's address. Levels from 2^31 on run the original
// (the deleted adapter's split).
ITC_DIRECT_ENTRY(822A54B0) {
  if (ctx.r5.u32 >= 0x80000000u) {
    lifted::original::c822A54B0(ctx);
    return;
  }
  const graphics::LevelLock lock =
      graphics::LockTextureLevel(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
  guest::StoreU32(ctx.r7.u32, lock.address);
  guest::StoreU32(ctx.r8.u32, lock.pitch);
  guest::StoreU32(ctx.r9.u32, lock.slice);
  guest::StoreU32(ctx.r10.u32, lock.tail);
  ctx.r3.u64 = lock.r3;
}
// Device::SetFVF: r3 the device, r4 the FVF code.
ITC_DIRECT_ENTRY(822AA0E8) { graphics::device_state::SetFVF(ctx.r3.u32, ctx.r4.u32); }

// --- c_runtime ---

// _vswprintf_l: r3 = the buffer, r4 = the format, r5 = the locale (unused),
// r6 = the va_list; returns the count. _woutput_l's work from its frame
// (144 + 2,336 bytes below, as swprintf's adapter makes it); the original
// where the port declines.
ITC_DIRECT_ENTRY(825A9980) {
  int32_t count = 0;
  if (!c_runtime::Vswprintf(ctx.r3.u32, ctx.r4.u32, ctx.r6.u32,
                            ctx.r1.u32 - 144 - 2336, &count)) {
    lifted::original::c825A9980(ctx);
    return;
  }
  ctx.r3.s64 = count;
}
// _fltin2: r3 = the result (+0 flags, +4 characters used, +16 the double),
// r4 = the string, r5 = its length (unused), r8 = the locale (tested for
// null by __strgtold12_l: the original then); r3 left (the deleted
// adapter's mapping).
ITC_DIRECT_ENTRY(825AFDC8) {
  c_runtime::FltinResult result;
  if (ctx.r8.u32 == 0 || !c_runtime::Fltin2(ctx.r4.u32, &result)) {
    lifted::original::c825AFDC8(ctx);
    return;
  }
  const uint32_t out = ctx.r3.u32;
  guest::StoreU32(out, result.flags);
  guest::StoreU32(out + 16, uint32_t(result.bits >> 32));
  guest::StoreU32(out + 20, uint32_t(result.bits));
  guest::StoreU32(out + 4, result.used);
}
// ___dtold: r3 = the 12-byte long double out, r4 = the double's address;
// +10 left as it is.
ITC_DIRECT_ENTRY(825B6378) {
  const uint64_t bits =
      (uint64_t(guest::LoadU32(ctx.r4.u32)) << 32) | guest::LoadU32(ctx.r4.u32 + 4);
  const c_runtime::LongDouble12 value = c_runtime::DoubleToLd12(bits);
  const uint32_t out = ctx.r3.u32;
  guest::StoreU16(out, value.sign_exponent);
  guest::StoreU32(out + 2, value.mantissa_high);
  guest::StoreU32(out + 6, value.mantissa_low);
}
// __mtold12: r3 = the digits, r4 = their count, r5 = the 12-byte long
// double out. A mantissa of 0 (the original never ends) and a count above
// 4,096 run the original.
ITC_DIRECT_ENTRY(825B7F10) {
  const uint32_t count = ctx.r4.u32;
  c_runtime::LongDouble12Full value;
  uint8_t digits[4096];
  bool native = count <= sizeof(digits);
  if (native) {
    for (uint32_t i = 0; i < count; i++) {
      digits[i] = guest::LoadU8(ctx.r3.u32 + i);
    }
    native = c_runtime::Mtold12(digits, count, &value);
  }
  if (!native) {
    lifted::original::c825B7F10(ctx);
    return;
  }
  const uint32_t out = ctx.r5.u32;
  guest::StoreU16(out, value.value.sign_exponent);
  guest::StoreU32(out + 2, value.value.mantissa_high);
  guest::StoreU32(out + 6, value.value.mantissa_low);
  guest::StoreU16(out + 10, value.extra);
}
// $I10_OUTPUT: r3 and r4 = the 12-byte long double by value (its top 80
// bits), r5 = the digits, r6 = the flags, r7 = the FOS out (only the bytes
// it writes); returns 1 or 0. The original where the port declines.
ITC_DIRECT_ENTRY(825B8B48) {
  const c_runtime::LongDouble12 value = {
      uint16_t(ctx.r3.u64 >> 48), uint32_t(ctx.r3.u64 >> 16),
      uint32_t(ctx.r3.u64 << 16) | uint32_t(ctx.r4.u64 >> 48)};
  c_runtime::FloatOut out;
  if (!c_runtime::I10Output(value, ctx.r5.s32, ctx.r6.u32, &out)) {
    lifted::original::c825B8B48(ctx);
    return;
  }
  for (uint32_t i = 0; i < sizeof(out.bytes); i++) {
    if (out.written[i]) {
      guest::StoreU8(ctx.r7.u32 + i, out.bytes[i]);
    }
  }
  ctx.r3.u64 = uint32_t(out.result);
}

// --- track_collision ---

// TrackMesh::GroundHeight: r3 = the mesh, r4 = the point, r5 = the track
// position, r6 = the color out (or 0), r7 = the normal out (or 0); returns
// the surface kind. Its frame 240 bytes below.
ITC_DIRECT_ENTRY(820F7AA0) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = track_collision::TrackMeshGroundHeight(
      ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r1.u32 - 240);
}
// WallQuery::GatherCells: r3 = the query, r4 = the mesh; returns the cell
// count. Its frame 192 bytes below. A shape that isn't a box: the original.
ITC_DIRECT_ENTRY(820FC188) {
  ctx.fpscr.disableFlushMode();
  uint32_t count = 0;
  if (!track_collision::WallQueryGatherCells(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192,
                                             &count)) {
    lifted::original::c820FC188(ctx);
    return;
  }
  ctx.r3.u64 = count;
}
// WallQuery::TestCell: r3 = the query, r4 = the mesh, r5 = the cell (r6 not
// read); r3 not kept (its caller doesn't read it). Its frame 256 bytes
// below. A shape that isn't a box: the original.
ITC_DIRECT_ENTRY(820FDC18) {
  ctx.fpscr.disableFlushMode();
  if (!track_collision::WallQueryTestCell(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                          ctx.r1.u32 - 256)) {
    lifted::original::c820FDC18(ctx);
  }
}
