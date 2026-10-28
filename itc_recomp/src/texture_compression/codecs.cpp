// XGraphics' image codecs natively (docs/functions/texture-compression.md):
// A8R8G8B8Codec::DecodeRow sub_823F2620, DxtCodec::EncodeRow sub_823F6298
// and the destructors sub_823EA070, sub_823EA770, sub_823F6B70 and
// sub_823F6BC0, as the translated code does them, on the codecs in guest
// memory.

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
    return call(function, frame, block, pixels, dither);
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
          r3 = call(field(codec::kDecodeBlock), frame, to, from, 0);
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
                  BlockAddress(base, codec, left, y, z), 0);
      }
      const uint32_t right = field(codec::kBlockRight);
      const uint32_t box_right = field(codec::kBoxRight);
      if (right > box_right && box_right < field(codec::kExtentX1)) {
        r3 = call(field(codec::kDecodeBlock), frame, Load(base, entry + 4),
                  BlockAddress(base, codec, right - 4, y, z), 0);
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

}  // namespace texture_compression
