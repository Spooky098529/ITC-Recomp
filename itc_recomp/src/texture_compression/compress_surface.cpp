// XGraphics' XGCompressSurface sub_8235AAC0 and XGGetBlockDimensions
// sub_8236D3C8 natively (docs/functions/texture-compression.md), as the
// translated code does them.

#include "texture_compression/compress_surface.h"

#include "base/log.h"
#include "graphics/resources.h"
#include "guest.h"
#include "texture_compression/blitter.h"

namespace texture_compression {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The destination's description format for a GPU format, or 0 for one
// XGCompressSurface leaves the stack's word for.
uint32_t DestinationFormat(uint32_t gpu_format, uint32_t flags) {
  switch (gpu_format) {
    case 18:
    case 51:
      return 0x1A200052;
    case 19:
    case 52:
      return 0x1A200053;
    case 20:
    case 53:
      return 0x1A200054;
    case 58:
      return 0x1A20007A;
    case 59:
      return 0x1A20007B;
    case 11:
      return ((flags & 4) != 0 ? 0x1A200000 : 0x18280000) | 0x4B;
    case 12:
      return ((flags & 4) != 0 ? 0x1A200000 : 0x18280000) | 0x4C;
    default:
      return 0;
  }
}

// The GPU formats whose destination is swapped after the blit unless it's
// 8-in-16 (the same list as above).
bool SwappedAfter(uint32_t gpu_format) { return DestinationFormat(gpu_format, 0) != 0; }

}  // namespace

uint32_t GetBlockDimensions(uint8_t* base, uint32_t format, uint32_t width, uint32_t height) {
  ITC_NATIVE_RUNNING("sub_8236D3C8 XGGetBlockDimensions");
  const graphics::Block block = graphics::GpuBlockSize(format);
  if (width != 0) {
    Store(base, width, block.width);
  }
  if (height != 0) {
    Store(base, height, block.height);
  }
  return format;
}

std::optional<uint32_t> CompressSurface(uint8_t* base, const CompressArguments& a, uint32_t stack,
                                        const GuestCall& call) {
  const uint32_t source_format = Load(base, stack + compress::kSourceFormat);  // r16
  const uint32_t gpu_format = a.format & 0x3F;                                 // r14
  const uint32_t source_gpu_format = source_format & 0x3F;                     // r27
  // The paths never seen in play: the whole original.
  if (gpu_format == 49 || gpu_format == 60) {
    return std::nullopt;  // DXN, CTX1: XGCopySurface into an XMemAlloc buffer
  }
  const uint32_t swizzle = (source_format >> 18) & 0xFFF;
  if (((source_format >> 6) & 3) != 2 ||
      swizzle != (source_gpu_format == 6 ? 0x60Au : 0x688u)) {
    return std::nullopt;  // the source copied and swapped first
  }
  if (SwappedAfter(gpu_format) && ((a.format >> 6) & 3) != 1) {
    return std::nullopt;  // the destination swapped after
  }
  ITC_NATIVE_RUNNING("sub_8235AAC0 XGCompressSurface");
  const uint32_t f = stack - compress::kFrameBytes;
  // r3 to r7 into the caller's frame.
  Store(base, stack + 20, a.destination);
  Store(base, stack + 28, a.pitch);
  Store(base, stack + 36, a.width);
  Store(base, stack + 44, a.height);
  Store(base, stack + 52, a.format);
  const uint32_t blitter = f + compress::kBlitter;
  BlitterConstruct(base, blitter);
  Store(base, f + compress::kFilter, 4);
  uint32_t rect = Load(base, stack + compress::kSourceRect);  // r18
  if (rect == 0) {
    rect = f + compress::kRect;
    Store(base, rect + 0, 0);
    Store(base, rect + 4, 0);
    Store(base, rect + 8, a.width);
    Store(base, rect + 12, a.height);
  }
  const uint32_t rect_width = Load(base, rect + 8) - Load(base, rect + 0);    // r29
  const uint32_t rect_height = Load(base, rect + 12) - Load(base, rect + 4);  // r28
  uint32_t point = a.point;  // r31
  if (point == 0) {
    point = f + compress::kPoint;
    Store(base, point + 0, 0);
    Store(base, point + 4, 0);
  }
  const uint32_t flags = Load(base, stack + compress::kFlags);  // r26
  if ((flags & 0x10) == 0) {
    Store(base, f + compress::kFilter, 0x80004);
  }
  GetBlockDimensions(base, gpu_format, f + compress::kBlockWidth, f + compress::kBlockHeight);

  // The destination's description, in the original's order.
  const uint32_t x = Load(base, point + 0);
  const uint32_t y = Load(base, point + 4);
  const uint32_t d = f + compress::kDestination;
  Store(base, d + description::kData, a.destination);
  Store(base, d + description::kRowPitch, a.pitch);
  Store(base, d + description::kSlicePitch, 0);
  Store(base, d + description::kBox + 0, x);
  Store(base, d + description::kExtent + 0, 0);
  Store(base, d + description::kExtent + 4, 0);
  Store(base, d + description::kExtent + 8, a.width);
  Store(base, d + description::kExtent + 12, a.height);
  Store(base, d + description::kBox + 8, x + rect_width);
  Store(base, d + description::kExtent + 16, 0);
  Store(base, d + description::kExtent + 20, 1);
  Store(base, d + description::kBox + 16, 0);
  Store(base, d + description::kBox + 20, 1);
  Store(base, d + description::kDither, 1);
  Store(base, d + description::kDiffusion, 0);
  Store(base, d + description::kSrgb, 0);
  Store(base, d + description::kColorKey, 0);
  Store(base, d + description::kPalette, 0);
  Store(base, d + description::kBox + 4, y);
  Store(base, d + description::kBox + 12, y + rect_height);
  if (const uint32_t format = DestinationFormat(gpu_format, flags); format != 0) {
    Store(base, d + description::kFormat, format);
  }

  // The source's: the rectangle is its extent and its box; +64 and +68 are
  // left as the stack has them (Blitter::Init sets only its +72).
  const uint32_t left = Load(base, rect + 0);
  const uint32_t top = Load(base, rect + 4);
  const uint32_t right = Load(base, rect + 8);
  const uint32_t bottom = Load(base, rect + 12);
  const uint32_t s = f + compress::kSource;
  Store(base, s + description::kData, a.source);
  Store(base, s + description::kRowPitch, a.source_pitch);
  Store(base, s + description::kSlicePitch, 0);
  Store(base, s + description::kExtent + 0, left);
  Store(base, s + description::kExtent + 4, top);
  Store(base, s + description::kExtent + 8, right);
  Store(base, s + description::kExtent + 12, bottom);
  Store(base, s + description::kExtent + 16, 0);
  Store(base, s + description::kExtent + 20, 1);
  for (uint32_t i = 0; i < 24; i += 4) {  // memcpy's 24 bytes
    Store(base, s + description::kBox + i, Load(base, s + description::kExtent + i));
  }
  Store(base, s + description::kSrgb, 0);
  Store(base, s + description::kColorKey, 0);
  Store(base, s + description::kPalette, 0);
  Store(base, s + description::kFormat, source_gpu_format == 6 ? 0x18280086 : 0x1A22AAA6);

  guest::StoreFloat(base, kAlphaReference, float(a.alpha_reference));
  Store(base, kPremultiplied, flags & 1);
  const uint32_t result =
      BlitterInit(base, blitter, d, s, Load(base, f + compress::kFilter), f, call);
  BlitterRelease(base, blitter, f, call);
  return result;
}

}  // namespace texture_compression
