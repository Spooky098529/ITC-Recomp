// XGraphics' XGCompressSurface sub_8235AAC0 and XGGetBlockDimensions
// sub_8236D3C8 natively (docs/functions/texture-compression.md), as the
// translated code does them.

#include "texture_compression/compress_surface.h"

#include "base/log.h"
#include "graphics/resources.h"
#include "guest.h"
#include "schema/texture_compression.h"
#include "texture_compression/blitter.h"

namespace texture_compression {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
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

uint32_t GetBlockDimensions(uint32_t format, uint32_t width, uint32_t height) {
  ITC_NATIVE_RUNNING("sub_8236D3C8 XGGetBlockDimensions");
  const graphics::Block block = graphics::GpuBlockSize(format);
  if (width != 0) {
    schema::TcBlockDimension(width).set_value(block.width);
  }
  if (height != 0) {
    schema::TcBlockDimension(height).set_value(block.height);
  }
  return format;
}

std::optional<uint32_t> CompressSurface(const CompressArguments& a, uint32_t stack,
                                        const GuestCall& call) {
  const uint32_t source_format = Load(stack + compress::kSourceFormat);  // r16
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
  Store(stack + 20, a.destination);
  Store(stack + 28, a.pitch);
  Store(stack + 36, a.width);
  Store(stack + 44, a.height);
  Store(stack + 52, a.format);
  const uint32_t blitter = f + compress::kBlitter;
  BlitterConstruct(blitter);
  Store(f + compress::kFilter, 4);
  uint32_t rect = Load(stack + compress::kSourceRect);  // r18
  if (rect == 0) {
    rect = f + compress::kRect;
    Store(rect + 0, 0);
    Store(rect + 4, 0);
    Store(rect + 8, a.width);
    Store(rect + 12, a.height);
  }
  const uint32_t rect_width = Load(rect + 8) - Load(rect + 0);    // r29
  const uint32_t rect_height = Load(rect + 12) - Load(rect + 4);  // r28
  uint32_t point = a.point;  // r31
  if (point == 0) {
    point = f + compress::kPoint;
    Store(point + 0, 0);
    Store(point + 4, 0);
  }
  const uint32_t flags = Load(stack + compress::kFlags);  // r26
  if ((flags & 0x10) == 0) {
    Store(f + compress::kFilter, 0x80004);
  }
  GetBlockDimensions(gpu_format, f + compress::kBlockWidth, f + compress::kBlockHeight);

  // The destination's description, in the original's order.
  const uint32_t x = Load(point + 0);
  const uint32_t y = Load(point + 4);
  const schema::ImageDescription d(f + compress::kDestination);
  d.set_data(a.destination);
  d.set_row_pitch(a.pitch);
  d.set_slice_pitch(0);
  d.set_box_left(x);
  d.set_extent_left(0);
  d.set_extent_top(0);
  d.set_extent_right(a.width);
  d.set_extent_bottom(a.height);
  d.set_box_right(x + rect_width);
  d.set_extent_front(0);
  d.set_extent_back(1);
  d.set_box_front(0);
  d.set_box_back(1);
  d.set_dither(1);
  d.set_diffusion(0);
  d.set_srgb(0);
  d.set_color_key(0);
  d.set_palette(0);
  d.set_box_top(y);
  d.set_box_bottom(y + rect_height);
  if (const uint32_t format = DestinationFormat(gpu_format, flags); format != 0) {
    d.set_format(format);
  }

  // The source's: the rectangle is its extent and its box; +64 and +68 are
  // left as the stack has them (Blitter::Init sets only its +72).
  const uint32_t left = Load(rect + 0);
  const uint32_t top = Load(rect + 4);
  const uint32_t right = Load(rect + 8);
  const uint32_t bottom = Load(rect + 12);
  const schema::ImageDescription s(f + compress::kSource);
  s.set_data(a.source);
  s.set_row_pitch(a.source_pitch);
  s.set_slice_pitch(0);
  s.set_extent_left(left);
  s.set_extent_top(top);
  s.set_extent_right(right);
  s.set_extent_bottom(bottom);
  s.set_extent_front(0);
  s.set_extent_back(1);
  for (uint32_t i = 0; i < 24; i += 4) {  // memcpy's 24 bytes
    Store(s.box_left_address() + i, Load(s.extent_left_address() + i));
  }
  s.set_srgb(0);
  s.set_color_key(0);
  s.set_palette(0);
  s.set_format(source_gpu_format == 6 ? 0x18280086 : 0x1A22AAA6);

  using G = schema::TextureCompressionGlobals;
  G::set_alpha_reference(float(a.alpha_reference));
  G::set_premultiplied(flags & 1);
  const uint32_t result = BlitterInit(blitter, d.address, s.address,
                                      Load(f + compress::kFilter), f, call);
  BlitterRelease(blitter, f, call);
  return result;
}

}  // namespace texture_compression
