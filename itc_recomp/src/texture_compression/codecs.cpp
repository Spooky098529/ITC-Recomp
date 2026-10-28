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
#include "texture_compression/dxt45.h"
#include "texture_compression/steps.h"

#include <cstring>
#include <vector>

namespace texture_compression {

using namespace steps;

namespace {

constexpr uint32_t kInv255 = 0x82002200;  // probably 1/255
constexpr uint32_t kMaxAlloc = 0xFFFFF000;  // malloc's largest size

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Field(uint8_t* base, uint32_t codec, uint32_t offset) {
  return Load(base, codec + offset);
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
  return (x >> 2) * Field(base, codec, codec::kBlockBytes) +
         (y >> 2) * Field(base, codec, codec::kRowPitch) +
         Field(base, codec, codec::kSlicePitch) * z + Field(base, codec, codec::kData);
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
  if (Field(base, codec, codec::kColorKey) != 0 || Field(base, codec, codec::kSrgb) != 0) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_823F2620 A8R8G8B8Codec::DecodeRow");
  uint32_t from = Field(base, codec, codec::kRowPitch) * row +
                  Field(base, codec, codec::kSlicePitch) * slice + Field(base, codec, codec::kData);
  const uint32_t end = (Field(base, codec, codec::kWidth) << 2) + from;
  if (from < end) {
    const double scale = Lfs(base, kInv255);  // f0
    uint32_t to = pixels;
    do {
      static constexpr uint32_t kBytes[4] = {1, 2, 3, 0};  // red, green, blue, alpha
      for (uint32_t c = 0; c < 4; c++) {
        const double v = Fmuls(IntToSingle(guest::LoadU8(base, from + kBytes[c])), scale);
        guest::StoreFloat(base, to + 4 * c, float(v));
      }
      from += 4;
      to += 16;
    } while (from < end);
  }
  return codec;
}

std::optional<uint32_t> DxtEncodeRow(uint8_t* base, uint32_t codec, uint32_t row, uint32_t slice,
                                     uint32_t pixels, uint32_t stack, const GuestCall& call) {
  auto field = [&](uint32_t offset) { return Field(base, codec, offset); };
  // The refusals first, with nothing done.
  if (field(codec::kConvertFrom) != 0 || field(codec::kSrgb) != 0) {
    return std::nullopt;
  }
  const uint32_t z = field(codec::kBoxFront) + slice;  // r25
  uint32_t y = row + field(codec::kBoxTop);            // r27
  auto entry_of = [&]() {                               // r26
    return (((z - field(codec::kFront)) * field(codec::kBlockRows) +
             ((y - field(codec::kBlockTop)) >> 2))
            << 3) +
           field(codec::kCache);
  };
  const bool row_refused = BlockRowBytes(field(codec::kBlockColumns)) > kMaxAlloc;
  if (field(codec::kCache) == 0) {
    if (CacheBytes(field(codec::kBlockRows) * field(codec::kSlices)) > kMaxAlloc || row_refused) {
      return std::nullopt;
    }
  } else {
    const uint32_t held = Load(base, entry_of() + 4);
    if (held == 0 ? row_refused : !Freeable(held)) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_823F6298 DxtCodec::EncodeRow");
  const uint32_t frame = stack - kEncodeRowFrameBytes;
  uint32_t r3 = codec;

  // The cache, on the first row.
  if (field(codec::kCache) == 0) {
    const auto made =
        memory::crt::OperatorNew(base, CacheBytes(field(codec::kBlockRows) * field(codec::kSlices)));
    const uint32_t cache = made ? *made : 0;
    if (!made) {
      ITC_LOG_ERROR("DxtCodec::EncodeRow: operator new refused the cache");
    }
    Store(base, codec + codec::kCache, cache);
    if (cache == 0) {
      return 0;
    }
    Clear(base, cache, (field(codec::kBlockRows) * field(codec::kSlices)) << 3);
    r3 = cache;
  }
  const uint32_t entry = entry_of();
  // The block row's float4s.
  if (Load(base, entry + 4) == 0) {
    const uint32_t bytes = BlockRowBytes(field(codec::kBlockColumns));
    if (bytes > kMaxAlloc) {  // the cache made just now: the original's new handler
      ITC_LOG_ERROR("DxtCodec::EncodeRow: a block row of {} bytes", bytes);
      return 0;
    }
    const auto made = memory::crt::OperatorNew(base, bytes);
    const uint32_t block_row = made ? *made : 0;
    if (!made) {
      ITC_LOG_ERROR("DxtCodec::EncodeRow: operator new refused a block row");
    }
    Store(base, entry + 4, block_row);
    if (block_row == 0) {
      return 0;
    }
    r3 = block_row;
    Store(base, entry, 0);
    Store(base, codec + codec::kCacheCount, field(codec::kCacheCount) + 1);
  }

  // A block row's first row: the destination's blocks the box cuts decoded
  // (+140) and the rows outside the box marked in.
  if (Load(base, entry) == 0) {
    const uint32_t first = y & ~3u;  // r24
    const uint32_t top = field(codec::kBoxTop);
    bool all = first < top && top > field(codec::kExtentY0);
    if (!all) {
      const uint32_t bottom = field(codec::kBoxBottom);
      all = first + 4 > bottom && bottom < field(codec::kExtentY1);
    }
    if (all) {
      uint32_t x = field(codec::kBlockLeft);  // r30
      uint32_t to = Load(base, entry + 4);    // r29
      uint32_t from = BlockAddress(base, codec, x, y, z);  // r28
      if (x < field(codec::kBlockRight)) {
        do {
          r3 = call(field(codec::kDecodeBlock), frame, to, from, 0, 0);
          x += 4;
          to += 256;
          from += field(codec::kBlockBytes);
        } while (x < field(codec::kBlockRight));
      }
    } else {
      // The left edge into the first block; the right edge's too (the
      // original's r3 is the block row's start for both).
      const uint32_t left = field(codec::kBlockLeft);
      const uint32_t box_left = field(codec::kBoxLeft);
      if (left < box_left && box_left > field(codec::kExtentX0)) {
        r3 = call(field(codec::kDecodeBlock), frame, Load(base, entry + 4),
                  BlockAddress(base, codec, left, y, z), 0, 0);
      }
      const uint32_t right = field(codec::kBlockRight);
      const uint32_t box_right = field(codec::kBoxRight);
      if (right > box_right && box_right < field(codec::kExtentX1)) {
        r3 = call(field(codec::kDecodeBlock), frame, Load(base, entry + 4),
                  BlockAddress(base, codec, right - 4, y, z), 0, 0);
      }
    }
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t at = i + first;
      if (at < field(codec::kBoxTop) || !(at < field(codec::kBoxBottom))) {
        Store(base, entry, Load(base, entry) | (1u << i));
      }
    }
  }

  // The row into the block row's float4s (row (y - +200) & 3 of each block).
  {
    const uint32_t in_block = (y - field(codec::kBlockTop)) & 3;  // r6
    uint32_t x = field(codec::kBoxLeft) - field(codec::kBlockLeft);
    const uint32_t end = field(codec::kWidth) + x;
    uint32_t from = pixels;
    if (x < end) {
      do {
        const uint32_t index = (((x & ~3u) | in_block) << 2) | (x & 3);
        CopyPixel(base, (index << 4) + Load(base, entry + 4), from);
        x++;
        from += 16;
      } while (x < end);
    }
  }
  const uint32_t mask = Load(base, entry) | (1u << (y & 3));
  Store(base, entry, mask);
  if (mask != 15) {
    return r3;
  }

  // All four rows in: each block padded past the surface's edge and encoded.
  uint32_t x = field(codec::kBlockLeft);             // r29
  uint32_t to = BlockAddress(base, codec, x, y, z);  // r28
  uint32_t block_pixels = Load(base, entry + 4);     // r30
  if (x < field(codec::kBlockRight)) {
    static constexpr uint32_t kRepeat[4] = {0, 0, 0, 1};  // the original's r1 + 80
    y &= ~3u;
    do {
      const uint32_t columns = field(codec::kExtentX1) - x;  // r6
      const uint32_t rows = field(codec::kExtentY1) - y;     // r4
      if (columns < 4 && rows != 0) {
        for (uint32_t r = 0; r < 4; r++) {
          for (uint32_t c = columns; c < 4; c++) {
            CopyPixel(base, (((r << 2) | c) << 4) + block_pixels,
                      (((r << 2) | kRepeat[c]) << 4) + block_pixels);
          }
          if (!(r + 1 < rows)) {
            break;
          }
        }
      }
      if (rows < 4) {
        for (uint32_t r = rows; r < 4; r++) {
          for (uint32_t c = 0; c < 4; c++) {
            CopyPixel(base, (((r << 2) | c) << 4) + block_pixels,
                      (((kRepeat[r] << 2) | c) << 4) + block_pixels);
          }
        }
      }
      const uint32_t dither =
          field(codec::kDither) != 0 || field(codec::kErrorRow) != 0 ? 1 : 0;
      r3 = EncodeBlock(base, field(codec::kEncodeBlock), to, block_pixels, dither, frame, call);
      x += 4;
      block_pixels += 256;
      to += field(codec::kBlockBytes);
    } while (x < field(codec::kBlockRight));
  }

  // The float4s to the next block row when it has none, else freed.
  const uint32_t next = entry + 8;
  const uint32_t end =
      ((field(codec::kSlices) * field(codec::kBlockRows)) << 3) + field(codec::kCache);
  if (next < end && Load(base, next) == 0 && Load(base, entry + 12) == 0) {
    const uint32_t held = Load(base, entry + 4);
    Store(base, entry + 4, 0);
    Store(base, entry + 12, held);
  } else {
    const uint32_t held = Load(base, entry + 4);
    Delete(held, "DxtCodec::EncodeRow");
    r3 = held;
    Store(base, codec + codec::kCacheCount, field(codec::kCacheCount) - 1);
  }
  Store(base, entry + 4, 0);
  return r3;
}

namespace {

// The blocks ~DxtCodec frees from the cache, in its order.
std::vector<uint32_t> CacheBlocks(uint8_t* base, uint32_t codec) {
  std::vector<uint32_t> blocks;
  if (Field(base, codec, codec::kCacheCount) == 0) {
    return blocks;
  }
  uint32_t entry = Field(base, codec, codec::kCache);  // r28
  if (entry == 0) {
    return blocks;
  }
  for (uint32_t z = Field(base, codec, codec::kFront); z < Field(base, codec, codec::kBack); z++) {
    for (uint32_t y = Field(base, codec, codec::kBlockTop);
         y < Field(base, codec, codec::kBlockBottom); y += 4) {
      blocks.push_back(Load(base, entry + 4));
      entry += 8;
    }
  }
  return blocks;
}

bool ImageCodecFreeable(uint8_t* base, uint32_t codec) {
  return Freeable(Field(base, codec, codec::kPalette)) && Freeable(Field(base, codec, codec::kRow)) &&
         Freeable(Field(base, codec, codec::kErrorRow));
}

bool DxtCodecFreeable(uint8_t* base, uint32_t codec) {
  for (uint32_t block : CacheBlocks(base, codec)) {
    if (!Freeable(block)) {
      return false;
    }
  }
  return Freeable(Field(base, codec, codec::kDxtUnknown240)) &&
         Freeable(Field(base, codec, codec::kCache)) && ImageCodecFreeable(base, codec);
}

uint32_t ImageCodecDestructChecked(uint8_t* base, uint32_t codec) {
  ITC_NATIVE_RUNNING("sub_823EA070 ImageCodec::~ImageCodec");
  Store(base, codec + codec::kVtable, kImageCodecVtable);
  Delete(Field(base, codec, codec::kPalette), "ImageCodec::~ImageCodec");
  Delete(Field(base, codec, codec::kRow), "ImageCodec::~ImageCodec");
  const uint32_t last = Field(base, codec, codec::kErrorRow);
  Delete(last, "ImageCodec::~ImageCodec");
  return last;
}

uint32_t DxtCodecDestructChecked(uint8_t* base, uint32_t codec) {
  ITC_NATIVE_RUNNING("sub_823EA770 DxtCodec::~DxtCodec");
  Store(base, codec + codec::kVtable, kDxtCodecVtable);
  for (uint32_t block : CacheBlocks(base, codec)) {
    Delete(block, "DxtCodec::~DxtCodec");
  }
  Delete(Field(base, codec, codec::kDxtUnknown240), "DxtCodec::~DxtCodec");
  Delete(Field(base, codec, codec::kCache), "DxtCodec::~DxtCodec");
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

namespace desc = description;

constexpr uint32_t kRounding = 0x8263C768;  // 32 floats of 0.5
constexpr uint32_t kDitherTable = 0x8263C7E8;  // a 4 x 4 ordered dither's offsets
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
bool ErrorRowRefused(uint8_t* base, uint32_t d) {
  if (Load(base, d + desc::kDiffusion) == 0) {
    return false;
  }
  const uint32_t width = Load(base, d + desc::kBox + 8) - Load(base, d + desc::kBox);
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

uint32_t ImageCodecConstructChecked(uint8_t* base, uint32_t codec, uint32_t d, uint32_t bits,
                                    uint32_t kind) {
  ITC_NATIVE_RUNNING("sub_823EA800 ImageCodec::ImageCodec");
  auto field = [&](uint32_t offset) { return Field(base, codec, offset); };
  auto set = [&](uint32_t offset, uint32_t value) { Store(base, codec + offset, value); };
  set(codec::kVtable, kImageCodecVtable);
  set(codec::kData, Load(base, d + desc::kData));
  set(codec::kFormat, Load(base, d + desc::kFormat) & ~kTiledBit);
  set(codec::kRowPitch, Load(base, d + desc::kRowPitch));
  set(codec::kSlicePitch, Load(base, d + desc::kSlicePitch));
  Copy24(base, codec + codec::kBoxLeft, d + desc::kBox);
  set(codec::kRounding, Load(base, d + desc::kDither) != 0 ? kDitherTable : kRounding);
  const uint32_t bytes = bits >> 3;
  const uint32_t has_bits = bits != 0 ? 1 : 0;
  set(codec::kKind, kind);
  set(codec::kBytesPerPixel, bytes);
  set(codec::kHasBits, has_bits);
  set(codec::kConvertFrom, 0);
  set(codec::kRow, 0);
  set(codec::kColorKey, Load(base, d + desc::kColorKey));
  set(codec::kPaletteSet, 0);
  set(codec::kPalette, 0);
  set(codec::kSrgbBoth, 0);
  set(codec::kSrgb, Load(base, d + desc::kSrgb));
  if (field(codec::kColorKey) != 0) {
    const uint32_t key = Load(base, d + desc::kColorKey);
    const double scale = Lfs(base, kInv255);  // f0
    const uint32_t channels[4] = {(key >> 16) & 0xFF, (key >> 8) & 0xFF, key & 0xFF, key >> 24};
    for (uint32_t c = 0; c < 4; c++) {
      guest::StoreFloat(base, codec + codec::kColorKeyFloats + 4 * c,
                        float(Fmuls(IntToSingle(channels[c]), scale)));
    }
  }
  if (kind == 2 && field(codec::kFormat) != kSignedUnsignedFormat) {
    set(codec::kSrgb, 0);
  }
  if (kind == 3) {
    set(codec::kSrgb, 0);
  }
  const uint32_t left = field(codec::kBoxLeft);
  const uint32_t top = field(codec::kBoxTop);
  const uint32_t front = field(codec::kBoxFront);
  const uint32_t width = field(codec::kBoxRight) - left;
  const uint32_t height = field(codec::kBoxBottom) - top;
  const uint32_t depth = field(codec::kBoxBack) - front;
  set(codec::kWidth, width);
  set(codec::kRowBytes, width * bytes);
  set(codec::kHeight, height);
  set(codec::kDepth, depth);
  if (has_bits != 0) {
    // The data moved to the box's corner (signed products, 32-bit sums).
    const uint32_t data = left * bytes + front * field(codec::kSlicePitch) +
                          top * field(codec::kRowPitch) + field(codec::kData);
    set(codec::kBoxLeft, 0);
    set(codec::kBoxTop, 0);
    set(codec::kBoxRight, width);
    set(codec::kBoxBottom, height);
    set(codec::kBoxBack, depth);
    set(codec::kBoxFront, 0);
    set(codec::kData, data);
  }
  if (Load(base, d + desc::kDiffusion) != 0) {
    const auto made = memory::crt::OperatorNew(base, ErrorRowBytes(field(codec::kWidth)));
    const uint32_t row = made ? *made : 0;
    set(codec::kErrorRow, row);
    if (row == 0) {
      // The original would clear from address 0.
      ITC_LOG_ERROR("ImageCodec::ImageCodec: operator new refused the error row");
    } else {
      Clear(base, row, (field(codec::kWidth) + 2) << 4);
    }
  } else {
    set(codec::kErrorRow, 0);
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

uint32_t DxtCodecConstructChecked(uint8_t* base, uint32_t codec, uint32_t d) {
  ITC_NATIVE_RUNNING("sub_823F5FA0 DxtCodec::DxtCodec");
  ImageCodecConstructChecked(base, codec, d, 0, 1);
  auto field = [&](uint32_t offset) { return Field(base, codec, offset); };
  auto set = [&](uint32_t offset, uint32_t value) { Store(base, codec + offset, value); };
  const uint32_t which = (field(codec::kFormat) & ~kTiledBit) - 0x1A200052;
  set(codec::kVtable, kDxtCodecVtable);
  for (const DxtBlocks& b : kDxtBlocks) {
    if (b.offset == which) {
      set(codec::kBlockBytes, b.bytes);
      set(codec::kEncodeBlock, b.encode);
      set(codec::kDecodeBlock, b.decode);
    }
  }
  Copy24(base, codec + codec::kExtentX0, d + desc::kExtent);
  set(codec::kDxt232, 0xFFFFFFFF);
  set(codec::kDxtUnknown240, 0);
  set(codec::kCacheCount, 0);
  set(codec::kCache, 0);
  // The extent rounded out to blocks (+172 to +192).
  set(codec::kBlockExtent + 12, (field(codec::kExtentY1) + 3) & ~3u);
  set(codec::kBlockExtent + 8, (field(codec::kExtentX1) + 3) & ~3u);
  set(codec::kDxt236, 0xFFFFFFFF);
  set(codec::kBlockExtent + 0, field(codec::kExtentX0) & ~3u);
  set(codec::kBlockExtent + 4, field(codec::kExtentY0) & ~3u);
  set(codec::kBlockExtent + 16, field(codec::kExtentX0 + 16));
  set(codec::kBlockExtent + 20, field(codec::kExtentX0 + 20));
  // The box rounded out to blocks.
  const uint32_t left = field(codec::kBoxLeft) & ~3u;
  const uint32_t top = field(codec::kBoxTop) & ~3u;
  const uint32_t right = (field(codec::kBoxRight) + 3) & ~3u;
  const uint32_t bottom = (field(codec::kBoxBottom) + 3) & ~3u;
  const uint32_t front = field(codec::kBoxFront);
  const uint32_t back = field(codec::kBoxBack);
  set(codec::kBlockLeft, left);
  set(codec::kFront, front);
  set(codec::kBlockTop, top);
  set(codec::kBlockRight, right);
  set(codec::kBlockBottom, bottom);
  set(codec::kBack, back);
  set(codec::kBlockColumns, (right - left) >> 2);
  set(codec::kBlockRows, (bottom - top) >> 2);
  set(codec::kSlices, back - front);
  set(codec::kDither, Load(base, d + desc::kDither));
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
  if (Overlap(codec, 128, description, desc::kBytes) || ErrorRowRefused(base, description)) {
    return std::nullopt;
  }
  return ImageCodecConstructChecked(base, codec, description, bits, kind);
}

std::optional<uint32_t> DxtCodecConstruct(uint8_t* base, uint32_t codec, uint32_t description) {
  if (Overlap(codec, 256, description, desc::kBytes) || ErrorRowRefused(base, description)) {
    return std::nullopt;
  }
  return DxtCodecConstructChecked(base, codec, description);
}

std::optional<uint32_t> PrepareCodecs(uint8_t* base, uint32_t destination, uint32_t source) {
  auto field = [&](uint32_t codec, uint32_t offset) { return Field(base, codec, offset); };
  const uint32_t kind = field(destination, codec::kKind);
  const uint32_t source_kind = field(source, codec::kKind);
  const bool convert = kind != source_kind && kind != 4;
  const uint32_t convert_from = convert ? source_kind : field(destination, codec::kConvertFrom);
  const bool row = convert_from != 0 || field(destination, codec::kSrgb) != 0;
  if (row && RowBytes(field(destination, codec::kWidth)) > kMaxAlloc) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_823EAA90 ImageCodec::Prepare");
  if (convert) {
    Store(base, destination + codec::kConvertFrom, source_kind);
  }
  if (row) {
    const auto made = memory::crt::OperatorNew(base, RowBytes(field(destination, codec::kWidth)));
    const uint32_t block = made ? *made : 0;
    Store(base, destination + codec::kRow, block);
    if (block == 0) {
      ITC_LOG_ERROR("ImageCodec::Prepare: operator new refused the row");
      return kOutOfMemory;
    }
  }
  if (field(destination, codec::kSrgb) != 0 && field(source, codec::kSrgb) != 0) {
    Store(base, source + codec::kSrgbBoth, 1);
    Store(base, destination + codec::kSrgbBoth, 1);
  }
  return 0;
}

std::optional<uint32_t> CreateCodec(uint8_t* base, uint32_t description, uint32_t stack,
                                    const GuestCall& call) {
  const Format* format = FindFormat(Load(base, description + desc::kFormat));
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
  Store(base, codec + codec::kVtable, format->vtable);
  // +28 (the palette's) is 0 from ImageCodec::ImageCodec: no LoadPalette.
  if (Field(base, codec, codec::kColorKey) != 0) {
    const uint32_t quantize = Load(base, Field(base, codec, codec::kVtable) + 12);
    call(quantize, stack - kCreateFrameBytes, codec, 0, 0, 0);
  }
  return codec;
}

}  // namespace texture_compression
