// XGraphics' image codecs natively (docs/functions/texture-compression.md):
// A8R8G8B8Codec::DecodeRow sub_823F2620, DxtCodec::EncodeRow sub_823F6298,
// the destructors sub_823EA070, sub_823EA770, sub_823F6B70 and
// sub_823F6BC0, and the codecs made: ImageCodec::ImageCodec sub_823EA800,
// Prepare sub_823EAA90, DxtCodec::DxtCodec sub_823F5FA0 and
// ImageCodec::Create sub_823F6C60; as the translated code does them, on the
// codecs in guest memory.

#include "texture_compression/codecs.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/texture_compression.h"
#include "texture_compression/dxt45.h"
#include "texture_compression/steps.h"

#include <cstring>
#include <vector>

namespace texture_compression {

using namespace steps;

namespace {

using G = schema::TextureCompressionGlobals;

constexpr uint32_t kInv255 = G::inverse_255_address();  // probably 1/255
constexpr uint32_t kMaxAlloc = 0xFFFFF000;  // malloc's largest size

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

bool Freeable(uint32_t block) { return block == 0 || memory::IsHeapBlock(block); }

// operator delete as its adapter does it (r3 left: the block).
void Delete(uint32_t block, const char* what) {
  if (!memory::crt::OperatorDelete(block)) {
    ITC_LOG_ERROR("{}: operator delete refused {:08X}", what, block);
  }
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes, [](uint8_t* host, uint32_t, uint32_t size) {
    std::memset(host, 0, size);
  });
}

// The 16 bytes of a float4 copied word by word (lwz, stw).
void CopyPixel(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t w = 0; w < 16; w += 4) {
    Store(base, to + w, Load(base, from + w));
  }
}

// The cache's size for `count` entries (8 bytes each; over 0x1FFFFFFF the
// original asks operator new for 0xFFFFFFFF), and a block row's (256 bytes a
// block column; over 0x0FFFFFFF float4s, 0xFFFFFFFF).
uint32_t CacheBytes(uint32_t count) { return count > 0x1FFFFFFF ? 0xFFFFFFFF : count << 3; }
uint32_t BlockRowBytes(uint32_t columns) {
  const uint32_t pixels = columns << 4;
  return pixels > 0x0FFFFFFF ? 0xFFFFFFFF : pixels << 4;
}

// The destination's address of block column `x` (pixels) on pixel row `y`
// of slice `z`: data + (x >> 2) x +136 + (y >> 2) x +96 + z x +100.
uint32_t BlockAddress(uint8_t* base, uint32_t codec, uint32_t x, uint32_t y, uint32_t z) {
  const schema::DxtCodec c(base, codec);
  return (x >> 2) * c.block_bytes() + (y >> 2) * c.row_pitch() + c.slice_pitch() * z +
         c.data().address;
}

// The DXT codec's block encode (+144): Dxt45::EncodeBlock's plain C++ for
// sub_82422A68, else the original through `call`. Its r3.
uint32_t EncodeBlock(uint8_t* base, uint32_t function, uint32_t block, uint32_t pixels,
                     uint32_t dither, uint32_t frame, const GuestCall& call) {
  if (function != kDxt45EncodeBlock) {
    return call(function, frame, block, pixels, dither, 0);
  }
  float values[16][4];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 4; c++) {
      values[i][c] = guest::LoadFloat(base, pixels + 16 * i + 4 * c);
    }
  }
  uint8_t bytes[16];
  EncodeDxt45Block(base, values, int32_t(dither), frame, bytes);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU8(base, block + i, bytes[i]);
  }
  return 0;
}

}  // namespace

std::optional<uint32_t> DecodeA8R8G8B8Row(uint8_t* base, uint32_t codec, uint32_t row,
                                          uint32_t slice, uint32_t pixels) {
  const schema::ImageCodec c(base, codec);
  if (c.color_key() != 0 || c.srgb() != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_823F2620 A8R8G8B8Codec::DecodeRow");
  uint32_t from = c.row_pitch() * row + c.slice_pitch() * slice + c.data().address;
  const uint32_t end = (c.width() << 2) + from;
  if (from < end) {
    const double scale = Lfs(base, kInv255);  // f0
    uint32_t to = pixels;
    do {
      static constexpr uint32_t kBytes[4] = {1, 2, 3, 0};  // red, green, blue, alpha
      for (uint32_t channel = 0; channel < 4; channel++) {
        const double v = Fmuls(IntToSingle(guest::LoadU8(base, from + kBytes[channel])), scale);
        guest::StoreFloat(base, to + 4 * channel, float(v));
      }
      from += 4;
      to += 16;
    } while (from < end);
  }
  return codec;
}

std::optional<uint32_t> DxtEncodeRow(uint8_t* base, uint32_t codec, uint32_t row, uint32_t slice,
                                     uint32_t pixels, uint32_t stack, const GuestCall& call) {
  const schema::DxtCodec c(base, codec);
  // The refusals first, with nothing done.
  if (c.convert_from() != 0 || c.srgb() != 0) {
    return std::nullopt;
  }
  const uint32_t z = c.box_front() + slice;  // r25
  uint32_t y = row + c.box_top();            // r27
  auto entry_of = [&]() {                               // r26
    return (((z - c.front()) * c.block_rows() + ((y - c.block_top()) >> 2)) << 3) +
           c.cache().address;
  };
  const bool row_refused = BlockRowBytes(c.block_columns()) > kMaxAlloc;
  if (c.cache().address == 0) {
    if (CacheBytes(c.block_rows() * c.slices()) > kMaxAlloc || row_refused) {
      return std::nullopt;
    }
  } else {
    const uint32_t held = schema::DxtBlockCacheEntry(base, entry_of()).pixels().address;
    if (held == 0 ? row_refused : !Freeable(held)) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_823F6298 DxtCodec::EncodeRow");
  const uint32_t frame = stack - kEncodeRowFrameBytes;
  uint32_t r3 = codec;

  // The cache, on the first row.
  if (c.cache().address == 0) {
    const auto made =
        memory::crt::OperatorNew(base, CacheBytes(c.block_rows() * c.slices()));
    const uint32_t cache = made ? *made : 0;
    if (!made) {
      ITC_LOG_ERROR("DxtCodec::EncodeRow: operator new refused the cache");
    }
    c.set_cache(cache);
    if (cache == 0) {
      return 0;
    }
    Clear(base, cache, (c.block_rows() * c.slices()) << 3);
    r3 = cache;
  }
  const schema::DxtBlockCacheEntry e(base, entry_of());
  // The block row's float4s.
  if (e.pixels().address == 0) {
    const uint32_t bytes = BlockRowBytes(c.block_columns());
    if (bytes > kMaxAlloc) {  // the cache made just now: the original's new handler
      ITC_LOG_ERROR("DxtCodec::EncodeRow: a block row of {} bytes", bytes);
      return 0;
    }
    const auto made = memory::crt::OperatorNew(base, bytes);
    const uint32_t block_row = made ? *made : 0;
    if (!made) {
      ITC_LOG_ERROR("DxtCodec::EncodeRow: operator new refused a block row");
    }
    e.set_pixels(block_row);
    if (block_row == 0) {
      return 0;
    }
    r3 = block_row;
    e.set_rows_in(0);
    c.set_cache_count(c.cache_count() + 1);
  }

  // A block row's first row: the destination's blocks the box cuts decoded
  // (+140) and the rows outside the box marked in.
  if (e.rows_in() == 0) {
    const uint32_t first = y & ~3u;  // r24
    const uint32_t top = c.box_top();
    bool all = first < top && top > c.extent_top();
    if (!all) {
      const uint32_t bottom = c.box_bottom();
      all = first + 4 > bottom && bottom < c.extent_bottom();
    }
    if (all) {
      uint32_t x = c.block_left();       // r30
      uint32_t to = e.pixels().address;  // r29
      uint32_t from = BlockAddress(base, codec, x, y, z);  // r28
      if (x < c.block_right()) {
        do {
          r3 = call(c.decode_block(), frame, to, from, 0, 0);
          x += 4;
          to += 256;
          from += c.block_bytes();
        } while (x < c.block_right());
      }
    } else {
      // The left edge into the first block; the right edge's too (the
      // original's r3 is the block row's start for both).
      const uint32_t left = c.block_left();
      const uint32_t box_left = c.box_left();
      if (left < box_left && box_left > c.extent_left()) {
        r3 = call(c.decode_block(), frame, e.pixels().address,
                  BlockAddress(base, codec, left, y, z), 0, 0);
      }
      const uint32_t right = c.block_right();
      const uint32_t box_right = c.box_right();
      if (right > box_right && box_right < c.extent_right()) {
        r3 = call(c.decode_block(), frame, e.pixels().address,
                  BlockAddress(base, codec, right - 4, y, z), 0, 0);
      }
    }
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t at = i + first;
      if (at < c.box_top() || !(at < c.box_bottom())) {
        e.set_rows_in(e.rows_in() | (1u << i));
      }
    }
  }

  // The row into the block row's float4s (row (y - +200) & 3 of each block).
  {
    const uint32_t in_block = (y - c.block_top()) & 3;  // r6
    uint32_t x = c.box_left() - c.block_left();
    const uint32_t end = c.width() + x;
    uint32_t from = pixels;
    if (x < end) {
      do {
        const uint32_t index = (((x & ~3u) | in_block) << 2) | (x & 3);
        CopyPixel(base, (index << 4) + e.pixels().address, from);
        x++;
        from += 16;
      } while (x < end);
    }
  }
  const uint32_t mask = e.rows_in() | (1u << (y & 3));
  e.set_rows_in(mask);
  if (mask != 15) {
    return r3;
  }

  // All four rows in: each block padded past the surface's edge and encoded.
  uint32_t x = c.block_left();                       // r29
  uint32_t to = BlockAddress(base, codec, x, y, z);  // r28
  uint32_t block_pixels = e.pixels().address;        // r30
  if (x < c.block_right()) {
    static constexpr uint32_t kRepeat[4] = {0, 0, 0, 1};  // the original's r1 + 80
    y &= ~3u;
    do {
      const uint32_t columns = c.extent_right() - x;  // r6
      const uint32_t rows = c.extent_bottom() - y;    // r4
      if (columns < 4 && rows != 0) {
        for (uint32_t r = 0; r < 4; r++) {
          for (uint32_t column = columns; column < 4; column++) {
            CopyPixel(base, (((r << 2) | column) << 4) + block_pixels,
                      (((r << 2) | kRepeat[column]) << 4) + block_pixels);
          }
          if (!(r + 1 < rows)) {
            break;
          }
        }
      }
      if (rows < 4) {
        for (uint32_t r = rows; r < 4; r++) {
          for (uint32_t column = 0; column < 4; column++) {
            CopyPixel(base, (((r << 2) | column) << 4) + block_pixels,
                      (((kRepeat[r] << 2) | column) << 4) + block_pixels);
          }
        }
      }
      const uint32_t dither = c.dither() != 0 || c.error_row().address != 0 ? 1 : 0;
      r3 = EncodeBlock(base, c.encode_block(), to, block_pixels, dither, frame, call);
      x += 4;
      block_pixels += 256;
      to += c.block_bytes();
    } while (x < c.block_right());
  }

  // The float4s to the next block row when it has none, else freed.
  const schema::DxtBlockCacheEntry next(base, e.address + schema::DxtBlockCacheEntry::kSize);
  const uint32_t end = ((c.slices() * c.block_rows()) << 3) + c.cache().address;
  if (next.address < end && next.rows_in() == 0 && next.pixels().address == 0) {
    const uint32_t held = e.pixels().address;
    e.set_pixels(0);
    next.set_pixels(held);
  } else {
    const uint32_t held = e.pixels().address;
    Delete(held, "DxtCodec::EncodeRow");
    r3 = held;
    c.set_cache_count(c.cache_count() - 1);
  }
  e.set_pixels(0);
  return r3;
}

namespace {

// The blocks ~DxtCodec frees from the cache, in its order.
std::vector<uint32_t> CacheBlocks(uint8_t* base, uint32_t codec) {
  const schema::DxtCodec c(base, codec);
  std::vector<uint32_t> blocks;
  if (c.cache_count() == 0) {
    return blocks;
  }
  uint32_t entry = c.cache().address;  // r28
  if (entry == 0) {
    return blocks;
  }
  for (uint32_t z = c.front(); z < c.back(); z++) {
    for (uint32_t y = c.block_top(); y < c.block_bottom(); y += 4) {
      blocks.push_back(schema::DxtBlockCacheEntry(base, entry).pixels().address);
      entry += schema::DxtBlockCacheEntry::kSize;
    }
  }
  return blocks;
}

bool ImageCodecFreeable(uint8_t* base, uint32_t codec) {
  const schema::ImageCodec c(base, codec);
  return Freeable(c.palette()) && Freeable(c.row().address) && Freeable(c.error_row().address);
}

bool DxtCodecFreeable(uint8_t* base, uint32_t codec) {
  for (uint32_t block : CacheBlocks(base, codec)) {
    if (!Freeable(block)) {
      return false;
    }
  }
  const schema::DxtCodec c(base, codec);
  return Freeable(c.decode_cache().address) && Freeable(c.cache().address) &&
         ImageCodecFreeable(base, codec);
}

uint32_t ImageCodecDestructChecked(uint8_t* base, uint32_t codec) {
  ITC_NATIVE_RUNNING("sub_823EA070 ImageCodec::~ImageCodec");
  const schema::ImageCodec c(base, codec);
  c.set_vtable(schema::ImageCodec::kVtable);
  Delete(c.palette(), "ImageCodec::~ImageCodec");
  Delete(c.row().address, "ImageCodec::~ImageCodec");
  const uint32_t last = c.error_row().address;
  Delete(last, "ImageCodec::~ImageCodec");
  return last;
}

uint32_t DxtCodecDestructChecked(uint8_t* base, uint32_t codec) {
  ITC_NATIVE_RUNNING("sub_823EA770 DxtCodec::~DxtCodec");
  const schema::DxtCodec c(base, codec);
  c.set_vtable(schema::DxtCodec::kVtable);
  for (uint32_t block : CacheBlocks(base, codec)) {
    Delete(block, "DxtCodec::~DxtCodec");
  }
  Delete(c.decode_cache().address, "DxtCodec::~DxtCodec");
  Delete(c.cache().address, "DxtCodec::~DxtCodec");
  return ImageCodecDestructChecked(base, codec);
}

}  // namespace

std::optional<uint32_t> ImageCodecDestruct(uint8_t* base, uint32_t codec) {
  if (!ImageCodecFreeable(base, codec)) {
    return std::nullopt;
  }
  return ImageCodecDestructChecked(base, codec);
}

std::optional<uint32_t> DxtCodecDestruct(uint8_t* base, uint32_t codec) {
  if (!DxtCodecFreeable(base, codec)) {
    return std::nullopt;
  }
  return DxtCodecDestructChecked(base, codec);
}

std::optional<uint32_t> ImageCodecDelete(uint8_t* base, uint32_t codec, uint32_t flags) {
  if (!ImageCodecFreeable(base, codec) || ((flags & 1) != 0 && !Freeable(codec))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_823F6B70 ImageCodec::scalar deleting destructor");
  ImageCodecDestructChecked(base, codec);
  if ((flags & 1) != 0) {
    Delete(codec, "ImageCodec::scalar deleting destructor");
  }
  return codec;
}

std::optional<uint32_t> DxtCodecDelete(uint8_t* base, uint32_t codec, uint32_t flags) {
  if (!DxtCodecFreeable(base, codec) || ((flags & 1) != 0 && !Freeable(codec))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_823F6BC0 DxtCodec::scalar deleting destructor");
  DxtCodecDestructChecked(base, codec);
  if ((flags & 1) != 0) {
    Delete(codec, "DxtCodec::scalar deleting destructor");
  }
  return codec;
}


namespace {

constexpr uint32_t kRounding = G::rounding_table_address();  // 32 floats of 0.5
constexpr uint32_t kDitherTable = G::dither_table_address();  // a 4 x 4 ordered dither's offsets
constexpr uint32_t kTiledBit = 0x100;
constexpr uint32_t kSignedUnsignedFormat = 0x2A200B45;  // kind 2 keeping sRGB (never in +4)

// The error row's size for a box `width` wide: (width + 2) x 16 bytes, or
// 0xFFFFFFFF past 0x0FFFFFFF pixels (the original's ask then).
uint32_t ErrorRowBytes(uint32_t width) {
  const uint32_t pixels = width + 2;
  return pixels > 0x0FFFFFFF ? 0xFFFFFFFF : pixels << 4;
}
// A float4 row's: width x 16 bytes, or 0xFFFFFFFF past 0x0FFFFFFF.
uint32_t RowBytes(uint32_t width) { return width > 0x0FFFFFFF ? 0xFFFFFFFF : width << 4; }

// Whether ImageCodec::ImageCodec's error row would be a size operator new
// refuses (the box's width read from the description, as the codec gets it).
bool ErrorRowRefused(uint8_t* base, uint32_t description) {
  const schema::ImageDescription d(base, description);
  if (d.diffusion() == 0) {
    return false;
  }
  const uint32_t width = d.box_right() - d.box_left();
  return ErrorRowBytes(width) > kMaxAlloc;
}

bool Overlap(uint32_t a, uint32_t a_bytes, uint32_t b, uint32_t b_bytes) {
  return uint64_t(a) < uint64_t(b) + b_bytes && uint64_t(b) < uint64_t(a) + a_bytes;
}

// memcpy's 24 bytes (the ranges don't overlap: the same bytes).
void Copy24(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t w = 0; w < 24; w += 4) {
    Store(base, to + w, Load(base, from + w));
  }
}

uint32_t ImageCodecConstructChecked(uint8_t* base, uint32_t codec, uint32_t description,
                                    uint32_t bits, uint32_t kind) {
  ITC_NATIVE_RUNNING("sub_823EA800 ImageCodec::ImageCodec");
  const schema::ImageCodec c(base, codec);
  const schema::ImageDescription d(base, description);
  c.set_vtable(schema::ImageCodec::kVtable);
  c.set_data(d.data());
  c.set_format(d.format() & ~kTiledBit);
  c.set_row_pitch(d.row_pitch());
  c.set_slice_pitch(d.slice_pitch());
  Copy24(base, c.box_left_address(), d.box_left_address());
  c.set_rounding(d.dither() != 0 ? kDitherTable : kRounding);
  const uint32_t bytes = bits >> 3;
  const uint32_t has_bits = bits != 0 ? 1 : 0;
  c.set_kind(kind);
  c.set_bytes_per_pixel(bytes);
  c.set_has_bits(has_bits);
  c.set_convert_from(0);
  c.set_row(0);
  c.set_color_key(d.color_key());
  c.set_palette_set(0);
  c.set_palette(0);
  c.set_srgb_both(0);
  c.set_srgb(d.srgb());
  if (c.color_key() != 0) {
    const uint32_t key = d.color_key();
    const double scale = Lfs(base, kInv255);  // f0
    c.set_color_key_red(float(Fmuls(IntToSingle((key >> 16) & 0xFF), scale)));
    c.set_color_key_green(float(Fmuls(IntToSingle((key >> 8) & 0xFF), scale)));
    c.set_color_key_blue(float(Fmuls(IntToSingle(key & 0xFF), scale)));
    c.set_color_key_alpha(float(Fmuls(IntToSingle(key >> 24), scale)));
  }
  if (kind == 2 && c.format() != kSignedUnsignedFormat) {
    c.set_srgb(0);
  }
  if (kind == 3) {
    c.set_srgb(0);
  }
  const uint32_t left = c.box_left();
  const uint32_t top = c.box_top();
  const uint32_t front = c.box_front();
  const uint32_t width = c.box_right() - left;
  const uint32_t height = c.box_bottom() - top;
  const uint32_t depth = c.box_back() - front;
  c.set_width(width);
  c.set_row_bytes(width * bytes);
  c.set_height(height);
  c.set_depth(depth);
  if (has_bits != 0) {
    // The data moved to the box's corner (signed products, 32-bit sums).
    const uint32_t data =
        left * bytes + front * c.slice_pitch() + top * c.row_pitch() + c.data().address;
    c.set_box_left(0);
    c.set_box_top(0);
    c.set_box_right(width);
    c.set_box_bottom(height);
    c.set_box_back(depth);
    c.set_box_front(0);
    c.set_data(data);
  }
  if (d.diffusion() != 0) {
    const auto made = memory::crt::OperatorNew(base, ErrorRowBytes(c.width()));
    const uint32_t row = made ? *made : 0;
    c.set_error_row(row);
    if (row == 0) {
      // The original would clear from address 0.
      ITC_LOG_ERROR("ImageCodec::ImageCodec: operator new refused the error row");
    } else {
      Clear(base, row, (c.width() + 2) << 4);
    }
  } else {
    c.set_error_row(0);
  }
  return codec;
}

// The DXT codec's block functions by format (+4 less 0x1A200052): the
// encoder (+144), the decoder (+140) and the block's bytes (+136).
struct DxtBlocks {
  uint32_t offset;
  uint32_t encode;
  uint32_t decode;
  uint32_t bytes;
};
constexpr DxtBlocks kDxtBlocks[] = {
    {0, 0x82421EB0, 0x82421938, 8},     // DXT1
    {1, 0x82422A50, 0x82422A20, 16},    // DXT2/3
    {2, 0x82422A68, 0x82422A38, 16},    // DXT4/5
    {40, 0x82422000, 0x82421BF0, 8},    // DXT3A
    {41, 0x82422178, 0x82421C88, 8},    // DXT5A
};

uint32_t DxtCodecConstructChecked(uint8_t* base, uint32_t codec, uint32_t description) {
  ITC_NATIVE_RUNNING("sub_823F5FA0 DxtCodec::DxtCodec");
  ImageCodecConstructChecked(base, codec, description, 0, 1);
  const schema::DxtCodec c(base, codec);
  const schema::ImageDescription d(base, description);
  const uint32_t which = (c.format() & ~kTiledBit) - 0x1A200052;
  c.set_vtable(schema::DxtCodec::kVtable);
  for (const DxtBlocks& b : kDxtBlocks) {
    if (b.offset == which) {
      c.set_block_bytes(b.bytes);
      c.set_encode_block(b.encode);
      c.set_decode_block(b.decode);
    }
  }
  Copy24(base, c.extent_left_address(), d.extent_left_address());
  c.set_unknown_232(0xFFFFFFFF);
  c.set_decode_cache(0);
  c.set_cache_count(0);
  c.set_cache(0);
  // The extent rounded out to blocks (+172 to +192).
  c.set_block_extent_bottom((c.extent_bottom() + 3) & ~3u);
  c.set_block_extent_right((c.extent_right() + 3) & ~3u);
  c.set_unknown_236(0xFFFFFFFF);
  c.set_block_extent_left(c.extent_left() & ~3u);
  c.set_block_extent_top(c.extent_top() & ~3u);
  c.set_block_extent_front(c.extent_front());
  c.set_block_extent_back(c.extent_back());
  // The box rounded out to blocks.
  const uint32_t left = c.box_left() & ~3u;
  const uint32_t top = c.box_top() & ~3u;
  const uint32_t right = (c.box_right() + 3) & ~3u;
  const uint32_t bottom = (c.box_bottom() + 3) & ~3u;
  const uint32_t front = c.box_front();
  const uint32_t back = c.box_back();
  c.set_block_left(left);
  c.set_front(front);
  c.set_block_top(top);
  c.set_block_right(right);
  c.set_block_bottom(bottom);
  c.set_back(back);
  c.set_block_columns((right - left) >> 2);
  c.set_block_rows((bottom - top) >> 2);
  c.set_slices(back - front);
  c.set_dither(d.dither());
  return codec;
}

// ImageCodec::Create's formats (the original's compare tree, each with and
// without bit 8): the codec's size, constructor, bits per pixel and kind,
// and its vtable.
enum class Maker { kPlain, kDxt, kYuv };
struct Format {
  uint32_t format;
  uint32_t size;
  Maker maker;
  uint32_t bits;
  uint32_t kind;
  uint32_t vtable;
};
constexpr Maker kPlain = Maker::kPlain;
constexpr Maker kDxt = Maker::kDxt;
constexpr Maker kYuv = Maker::kYuv;
constexpr Format kFormats[] = {
    {0x04900002, 128, kPlain, 8, 1, 0x8203B868},    // A8
    {0x0800004A, 128, kPlain, 16, 1, 0x8203B8F8},   // A8L8
    {0x18280043, 128, kPlain, 16, 1, 0x8203B848},   // A1R5G5B5
    {0x1828004B, 176, kYuv, 0, 0, 0x8203B9F8},
    {0x1828004C, 176, kYuv, 0, 0, 0x8203B9F8},
    {0x1828004F, 128, kPlain, 16, 1, 0x8203B858},   // A4R4G4B4
    {0x18280086, 128, kPlain, 32, 1, 0x8203B808},   // A8R8G8B8
    {0x182800B6, 128, kPlain, 32, 1, 0x8203B8C8},   // A2R10G10B10
    {0x1A20004B, 176, kYuv, 0, 0, 0x8203B9F8},
    {0x1A20004C, 176, kYuv, 0, 0, 0x8203B9F8},
    {0x1A200052, 256, kDxt, 0, 0, 0x8203B9E8},      // DXT1
    {0x1A200053, 256, kDxt, 0, 0, 0x8203B9E8},      // DXT2/3
    {0x1A200054, 256, kDxt, 0, 0, 0x8203B9E8},      // DXT4/5
    {0x1A20005A, 128, kPlain, 64, 1, 0x8203B8D8},   // A16B16G16R16
    {0x1A20007A, 256, kDxt, 0, 0, 0x8203B9E8},      // DXT3A
    {0x1A20007B, 256, kDxt, 0, 0, 0x8203B9E8},      // DXT5A
    {0x1A200086, 128, kPlain, 32, 1, 0x8203B898},   // A8B8G8R8
    {0x1A2000B6, 128, kPlain, 32, 1, 0x8203B888},   // A2B10G10R10
    {0x1A202AB6, 128, kPlain, 32, 2, 0x8203B968},   // A2W10V10U10
    {0x1A20AA5A, 128, kPlain, 64, 3, 0x8203B978},   // Q16W16V16U16
    {0x1A20AA86, 128, kPlain, 32, 3, 0x8203B948},   // Q8W8V8U8
    {0x1A220058, 128, kPlain, 16, 1, 0x8203B908},   // L16
    {0x1A22AA60, 128, kPlain, 64, 4, 0x8203B9A8},   // A16B16G16R16F
    {0x1A22AAA6, 128, kPlain, 128, 4, 0x8203B9D8},  // A32B32G32R32F
    {0x28000002, 128, kPlain, 8, 1, 0x8203B8E8},    // L8
    {0x28000058, 128, kPlain, 16, 1, 0x8203B908},   // L16
    {0x28280043, 128, kPlain, 16, 1, 0x8203B838},   // X1R5G5B5
    {0x28280044, 128, kPlain, 16, 1, 0x8203B828},   // R5G6B5
    {0x2828004F, 128, kPlain, 16, 1, 0x8203B878},   // X4R4G4B4
    {0x28280086, 128, kPlain, 32, 1, 0x8203B818},   // X8R8G8B8
    {0x2A200086, 128, kPlain, 32, 1, 0x8203B8A8},   // X8B8G8R8
    {0x2A200A45, 128, kPlain, 16, 2, 0x8203B928},   // L6V5U5
    {0x2A200A86, 128, kPlain, 32, 2, 0x8203B938},   // X8L8V8U8
    {0x2D200099, 128, kPlain, 32, 1, 0x8203B8B8},   // G16R16
    {0x2D20AA4A, 128, kPlain, 16, 2, 0x8203B918},   // V8U8
    {0x2D20AA99, 128, kPlain, 32, 2, 0x8203B958},   // V16U16
    {0x2D22AA9F, 128, kPlain, 32, 4, 0x8203B998},   // G16R16F
    {0x2D22AAA5, 128, kPlain, 64, 4, 0x8203B9C8},   // G32R32F
    {0x2DA2AA5E, 128, kPlain, 16, 4, 0x8203B988},   // R16F
    {0x2DA2AAA4, 128, kPlain, 32, 4, 0x8203B9B8},   // R32F
};

const Format* FindFormat(uint32_t format) {
  for (const Format& f : kFormats) {
    if (f.format == (format & ~kTiledBit)) {
      return &f;
    }
  }
  return nullptr;
}

}  // namespace

std::optional<uint32_t> ImageCodecConstruct(uint8_t* base, uint32_t codec, uint32_t description,
                                            uint32_t bits, uint32_t kind) {
  if (Overlap(codec, schema::ImageCodec::kSize, description, schema::ImageDescription::kSize) ||
      ErrorRowRefused(base, description)) {
    return std::nullopt;
  }
  return ImageCodecConstructChecked(base, codec, description, bits, kind);
}

std::optional<uint32_t> DxtCodecConstruct(uint8_t* base, uint32_t codec, uint32_t description) {
  if (Overlap(codec, schema::DxtCodec::kSize, description, schema::ImageDescription::kSize) ||
      ErrorRowRefused(base, description)) {
    return std::nullopt;
  }
  return DxtCodecConstructChecked(base, codec, description);
}

std::optional<uint32_t> PrepareCodecs(uint8_t* base, uint32_t destination, uint32_t source) {
  const schema::ImageCodec to(base, destination);
  const schema::ImageCodec from(base, source);
  const uint32_t kind = to.kind();
  const uint32_t source_kind = from.kind();
  const bool convert = kind != source_kind && kind != 4;
  const uint32_t convert_from = convert ? source_kind : to.convert_from();
  const bool row = convert_from != 0 || to.srgb() != 0;
  if (row && RowBytes(to.width()) > kMaxAlloc) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_823EAA90 ImageCodec::Prepare");
  if (convert) {
    to.set_convert_from(source_kind);
  }
  if (row) {
    const auto made = memory::crt::OperatorNew(base, RowBytes(to.width()));
    const uint32_t block = made ? *made : 0;
    to.set_row(block);
    if (block == 0) {
      ITC_LOG_ERROR("ImageCodec::Prepare: operator new refused the row");
      return kOutOfMemory;
    }
  }
  if (to.srgb() != 0 && from.srgb() != 0) {
    from.set_srgb_both(1);
    to.set_srgb_both(1);
  }
  return 0;
}

std::optional<uint32_t> CreateCodec(uint8_t* base, uint32_t description, uint32_t stack,
                                    const GuestCall& call) {
  const Format* format = FindFormat(schema::ImageDescription(base, description).format());
  if (format != nullptr &&
      (format->maker == Maker::kYuv || ErrorRowRefused(base, description))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_823F6C60 ImageCodec::Create");
  if (format == nullptr) {
    return 0;
  }
  const auto made = memory::crt::OperatorNew(base, format->size);
  if (!made || *made == 0) {
    ITC_LOG_ERROR("ImageCodec::Create: operator new refused the codec");
    return 0;
  }
  const uint32_t codec = *made;
  if (format->maker == Maker::kDxt) {
    DxtCodecConstructChecked(base, codec, description);
  } else {
    ImageCodecConstructChecked(base, codec, description, format->bits, format->kind);
  }
  const schema::ImageCodec c(base, codec);
  c.set_vtable(format->vtable);
  // +28 (the palette's) is 0 from ImageCodec::ImageCodec: no LoadPalette.
  if (c.color_key() != 0) {
    const uint32_t quantize = c.vtable().quantize_color_key();
    call(quantize, stack - kCreateFrameBytes, codec, 0, 0, 0);
  }
  return codec;
}

}  // namespace texture_compression
