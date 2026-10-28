// XGraphics' image codecs (docs/functions/texture-compression.md): the
// codecs made (ImageCodec::Create, the constructors, Prepare), the A8R8G8B8
// codec's row decode, the DXT codec's row encode (its block cache) and the
// codecs' destructors. Plain C++ over the codec objects in guest memory,
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
constexpr uint32_t kFormat = 4;      // the description's, without bit 8
constexpr uint32_t kKind = 8;        // channels: 1 unsigned, 2 mixed signed, 3 signed, 4 float
constexpr uint32_t kHasBits = 12;    // bits per pixel isn't 0 (not a block format)
constexpr uint32_t kSrgb = 16;       // sRGB (in for a decode, out for an encode)
constexpr uint32_t kSrgbBoth = 20;   // both sides sRGB (ImageCodec::Prepare)
constexpr uint32_t kColorKey = 24;   // the color key (0: none)
constexpr uint32_t kPaletteSet = 28;  // 0 (ImageCodec::Create's LoadPalette test)
constexpr uint32_t kData = 32;
constexpr uint32_t kColorKeyFloats = 36;  // red, green, blue, alpha
constexpr uint32_t kRounding = 52;   // the rounding (or dither) table
constexpr uint32_t kPalette = 56;    // operator new (ImageCodec::LoadPalette)
constexpr uint32_t kBoxLeft = 60;    // the box: left, top, right, bottom, front, back
constexpr uint32_t kBoxTop = 64;
constexpr uint32_t kBoxRight = 68;
constexpr uint32_t kBoxBottom = 72;
constexpr uint32_t kBoxFront = 76;
constexpr uint32_t kBoxBack = 80;
constexpr uint32_t kConvertFrom = 84;  // the other side's channel kind (ConvertRange)
constexpr uint32_t kRow = 88;          // operator new (ImageCodec::Prepare)
constexpr uint32_t kErrorRow = 92;     // operator new (error diffusion)
constexpr uint32_t kRowPitch = 96;
constexpr uint32_t kSlicePitch = 100;
constexpr uint32_t kWidth = 104;       // the box's width
constexpr uint32_t kHeight = 108;
constexpr uint32_t kDepth = 112;
constexpr uint32_t kRowBytes = 116;    // width x bytes per pixel
constexpr uint32_t kBytesPerPixel = 120;
constexpr uint32_t kYuvBuffer = 128;   // YuvCodec's two pixels (operator new)
// The DXT codec's.
constexpr uint32_t kBlockBytes = 136;
constexpr uint32_t kDecodeBlock = 140;  // a function: r3 = 16 float4s, r4 = the block
constexpr uint32_t kEncodeBlock = 144;  // a function: r3 = the block, r4 = 16 float4s, r5 = dither
constexpr uint32_t kExtentX0 = 148;     // the surface: 0, 0, width, height, 0, depth
constexpr uint32_t kExtentY0 = 152;
constexpr uint32_t kExtentX1 = 156;
constexpr uint32_t kExtentY1 = 160;
constexpr uint32_t kBlockExtent = 172;  // the extent rounded out to blocks (6 words)
constexpr uint32_t kBlockLeft = 196;    // the box rounded out to blocks (pixels)
constexpr uint32_t kBlockTop = 200;
constexpr uint32_t kBlockRight = 204;
constexpr uint32_t kBlockBottom = 208;
constexpr uint32_t kFront = 212;
constexpr uint32_t kBack = 216;
constexpr uint32_t kBlockColumns = 220;
constexpr uint32_t kBlockRows = 224;
constexpr uint32_t kSlices = 228;
constexpr uint32_t kDxt232 = 232;         // -1 from DxtCodec::DxtCodec
constexpr uint32_t kDxt236 = 236;         // -1 from DxtCodec::DxtCodec
constexpr uint32_t kDxtUnknown240 = 240;  // operator delete'd by ~DxtCodec; nothing seen sets it
constexpr uint32_t kCacheCount = 244;     // block rows held
constexpr uint32_t kCache = 248;          // 8 bytes a block row: the rows-in mask, the 16 x 16 float4s
constexpr uint32_t kDither = 252;
}  // namespace codec

// The image description the codecs are made from (84 bytes, on
// XGCompressSurface's stack; Blitter::Init sets +64 to +72 from the filter).
namespace description {
constexpr uint32_t kData = 0;
constexpr uint32_t kFormat = 4;
constexpr uint32_t kRowPitch = 8;
constexpr uint32_t kSlicePitch = 12;
constexpr uint32_t kExtent = 16;     // 0, 0, width, height, 0, depth
constexpr uint32_t kBox = 40;        // left, top, right, bottom, front, back
constexpr uint32_t kDither = 64;
constexpr uint32_t kDiffusion = 68;
constexpr uint32_t kSrgb = 72;
constexpr uint32_t kColorKey = 76;
constexpr uint32_t kPalette = 80;    // never read by Create's paths
constexpr uint32_t kBytes = 84;
}  // namespace description

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

// ImageCodec::Create's stack frame (the color key's slot 3 runs below it).
constexpr uint32_t kCreateFrameBytes = 112;
// E_OUTOFMEMORY.
constexpr uint32_t kOutOfMemory = 0x8007000E;

// sub_823EA800 ImageCodec::ImageCodec: the codec's base fields from the
// description (+0 the base vtable 0x8203B778, +4 the format without bit 8,
// +8 `kind`, +12 whether `bits` isn't 0, +16 sRGB, +20 0, +24 the color key
// with its four channels at +36 as floats (red, green, blue, alpha: each
// byte times 1/255, 0x82002200; left alone without a key), +28 0, +32 the
// data, +52 the rounding table (0x8263C768, or the dither's 0x8263C7E8),
// +56 0, +60 the box (memcpy's 24 bytes), +84, +88 0, +92 the error row
// (operator new and memset, (width + 2) x 16 bytes, with diffusion) or 0,
// +96, +100 the pitches, +104 to +112 the box's size, +116 the row's bytes,
// +120 bytes per pixel; sRGB cleared for kinds 2 (unless the format is
// 0x2A200B45, which +4 never is) and 3; with `bits`, the data moved to the
// box's corner and the box to 0, 0, 0). The original's r3 (the codec).
// nullopt, with nothing done, when the codec's 128 bytes overlap the
// description or the error row's size is one operator new refuses (the whole
// original runs then). Only with the native heap.
std::optional<uint32_t> ImageCodecConstruct(uint8_t* base, uint32_t codec, uint32_t description,
                                            uint32_t bits, uint32_t kind);
// sub_823EAA90 ImageCodec::Prepare: the destination's +84 = the source's
// kind when theirs differ and the destination's isn't float (4); with +84 or
// sRGB out (+16), its +88 = a float4 row (+104 x 16 bytes, operator new;
// none: E_OUTOFMEMORY); both +20 = 1 when both are sRGB. The original's r3
// (0 or E_OUTOFMEMORY). nullopt, with nothing done, for a row size operator
// new refuses. Only with the native heap.
std::optional<uint32_t> PrepareCodecs(uint8_t* base, uint32_t destination, uint32_t source);
// sub_823F5FA0 DxtCodec::DxtCodec: ImageCodec::ImageCodec (0 bits, kind 1),
// the DXT vtable (0x8203B9E8), by the format +136 the block's bytes, +140
// and +144 the block decoder and encoder (left alone for a format not DXT1,
// 2/3, 4/5, 3A or 5A), +148 the surface's extent (memcpy's 24 bytes), +172
// to +192 it rounded out to blocks, +196 to +216 the box so, +220 to +228
// the block columns, rows and slices, +232, +236 -1, +240 to +248 0, +252
// the dither. The original's r3 (the codec). nullopt as ImageCodecConstruct
// (the codec's 256 bytes).
std::optional<uint32_t> DxtCodecConstruct(uint8_t* base, uint32_t codec, uint32_t description);
// sub_823F6C60 ImageCodec::Create: the codec for the description's format
// (its 80 formats, 40 and each with bit 8, as a table built from the
// original's compare tree): operator new (128 bytes) and
// ImageCodec::ImageCodec with the format's bits and kind, or 256 bytes and
// DxtCodec::DxtCodec, then the format's vtable; with a color key, slot 3 (the
// key quantized) through `call` at `stack` - 112. The palette's
// LoadPalette (sub_823EAB58) is never reached: +28 is always 0 after the
// constructors. The original's r3: the codec, or 0 for a format it doesn't
// take (or operator new giving 0). nullopt, with nothing done, for the YUV
// formats (YuvCodec::YuvCodec, never seen in play) and an error row's size
// operator new refuses: the whole original runs then. Only with the native
// heap.
std::optional<uint32_t> CreateCodec(uint8_t* base, uint32_t description, uint32_t stack,
                                    const GuestCall& call);

}  // namespace texture_compression
