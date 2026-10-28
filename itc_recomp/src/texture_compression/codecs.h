// XGraphics' image codecs (docs/functions/texture-compression.md): the
// A8R8G8B8 codec's row decode, the DXT codec's row encode (its block cache)
// and the codecs' destructors. Plain C++ over the codec objects in guest
// memory (made by the recompiled ImageCodec::Create and the constructors),
// allocating through the native heap (memory/c_runtime_heap.h, the user's
// call of 2026-10-22); only call these while --native_heap routes the game's
// heap to the layer (hooks::NativeHeapActive()).

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace texture_compression {

// The codecs' fields (ImageCodec::ImageCodec sub_823EA800, DxtCodec::DxtCodec
// sub_823F5FA0).
namespace codec {
constexpr uint32_t kVtable = 0;
constexpr uint32_t kSrgb = 16;       // sRGB (in for a decode, out for an encode)
constexpr uint32_t kColorKey = 24;   // the color key (0: none)
constexpr uint32_t kData = 32;
constexpr uint32_t kPalette = 56;    // operator new (ImageCodec::LoadPalette)
constexpr uint32_t kBoxLeft = 60;    // the box: left, top, right, bottom, front, back
constexpr uint32_t kBoxTop = 64;
constexpr uint32_t kBoxRight = 68;
constexpr uint32_t kBoxBottom = 72;
constexpr uint32_t kBoxFront = 76;
constexpr uint32_t kConvertFrom = 84;  // the other side's channel kind (ConvertRange)
constexpr uint32_t kRow = 88;          // operator new (ImageCodec::Prepare)
constexpr uint32_t kErrorRow = 92;     // operator new (error diffusion)
constexpr uint32_t kRowPitch = 96;
constexpr uint32_t kSlicePitch = 100;
constexpr uint32_t kWidth = 104;       // the box's width
// The DXT codec's.
constexpr uint32_t kBlockBytes = 136;
constexpr uint32_t kDecodeBlock = 140;  // a function: r3 = 16 float4s, r4 = the block
constexpr uint32_t kEncodeBlock = 144;  // a function: r3 = the block, r4 = 16 float4s, r5 = dither
constexpr uint32_t kExtentX0 = 148;     // the surface: 0, 0, width, height, 0, depth
constexpr uint32_t kExtentY0 = 152;
constexpr uint32_t kExtentX1 = 156;
constexpr uint32_t kExtentY1 = 160;
constexpr uint32_t kBlockLeft = 196;    // the box rounded out to blocks (pixels)
constexpr uint32_t kBlockTop = 200;
constexpr uint32_t kBlockRight = 204;
constexpr uint32_t kBlockBottom = 208;
constexpr uint32_t kFront = 212;
constexpr uint32_t kBack = 216;
constexpr uint32_t kBlockColumns = 220;
constexpr uint32_t kBlockRows = 224;
constexpr uint32_t kSlices = 228;
constexpr uint32_t kDxtUnknown240 = 240;  // operator delete'd by ~DxtCodec; nothing seen sets it
constexpr uint32_t kCacheCount = 244;     // block rows held
constexpr uint32_t kCache = 248;          // 8 bytes a block row: the rows-in mask, the 16 x 16 float4s
constexpr uint32_t kDither = 252;
}  // namespace codec

constexpr uint32_t kImageCodecVtable = 0x8203B778;
constexpr uint32_t kDxtCodecVtable = 0x8203B9E8;
// The functions the codecs reach that the plain C++ knows.
constexpr uint32_t kDxt45EncodeBlock = 0x82422A68;
constexpr uint32_t kApplyColorKey = 0x823EADB0;
constexpr uint32_t kSrgbToLinear = 0x823EB180;

// A guest function's original (the codec's block functions), run with r1 =
// `stack` and r3, r4, r5 as given; its r3.
using GuestCall =
    std::function<uint32_t(uint32_t address, uint32_t stack, uint32_t r3, uint32_t r4, uint32_t r5)>;

// sub_823F2620 A8R8G8B8Codec::DecodeRow (slot 1 of the A8R8G8B8 vtable):
// row `row` of slice `slice` (from the data, +32, + row x +96 + slice x
// +100: signed products, 32-bit sums), each pixel's bytes 1, 2, 3, 0 (red,
// green, blue, alpha) times 1/255 (0x82002200) into the float4s at `pixels`,
// +104 pixels. The original's r3 (the codec). nullopt, with nothing done,
// when the codec has a color key (+24) or sRGB in (+16): ApplyColorKey
// (sub_823EADB0) and SrgbToLinear (sub_823EB180) never ran in play, so the
// whole original runs then.
std::optional<uint32_t> DecodeA8R8G8B8Row(uint8_t* base, uint32_t codec, uint32_t row,
                                          uint32_t slice, uint32_t pixels);

// DxtCodec::EncodeRow's stack frame.
constexpr uint32_t kEncodeRowFrameBytes = 192;

// sub_823F6298 DxtCodec::EncodeRow (slot 2 of the DXT vtable): row `row` of
// slice `slice` of the box (the float4s at `pixels`, +104 of them) into the
// codec's block cache (+248, made on the first row: operator new and
// memset; each block row's 16 x 16 float4s by operator new, +244 counting
// them); a block row's first row decodes the destination's blocks the box
// cuts (+140, through `call`); once its four rows are in, the blocks past the
// surface's edge padded by repeating pixels (columns and rows 0, 0, 0, 1),
// each block encoded (+144: Dxt45::EncodeBlock's plain C++ when it's
// sub_82422A68, else through `call`), and its float4s handed to the next
// block row when that one has none, else freed. `stack` is the guest r1 at
// entry (the calls run at `stack` - 192). The original's r3 (what its last
// call left, or the codec). nullopt, with nothing done, when it would
// convert the row (+84: ConvertRange sub_823EA0C0; +16: LinearToSrgb
// sub_823EAE40; neither ran in play), allocate a size operator new refuses,
// or free a block the heap didn't give: the whole original runs then.
std::optional<uint32_t> DxtEncodeRow(uint8_t* base, uint32_t codec, uint32_t row, uint32_t slice,
                                     uint32_t pixels, uint32_t stack, const GuestCall& call);

// sub_823EA070 ImageCodec::~ImageCodec: the base vtable back (0x8203B778),
// operator delete on +56, +88 and +92. The original's r3 (+92, as operator
// delete leaves it). nullopt, with nothing done, when one isn't 0 or a heap
// block.
std::optional<uint32_t> ImageCodecDestruct(uint8_t* base, uint32_t codec);
// sub_823EA770 DxtCodec::~DxtCodec: the DXT vtable back (0x8203B9E8); with
// block rows held (+244), each cache entry's float4s (over the block rows
// +200 to +208 by 4, the slices +212 to +216) freed; operator delete on +240
// and +248; ~ImageCodec. Its r3, or nullopt as ImageCodecDestruct.
std::optional<uint32_t> DxtCodecDestruct(uint8_t* base, uint32_t codec);
// sub_823F6B70 and sub_823F6BC0 (slot 0, the scalar deleting destructors of
// the plain and DXT codecs): the destructor, then operator delete on the
// codec with bit 0 of `flags`. The original's r3 (the codec), or nullopt as
// above.
std::optional<uint32_t> ImageCodecDelete(uint8_t* base, uint32_t codec, uint32_t flags);
std::optional<uint32_t> DxtCodecDelete(uint8_t* base, uint32_t codec, uint32_t flags);

}  // namespace texture_compression
