// XGraphics' image blitter (docs/functions/texture-compression.md): the
// blit object made, released and run (Blitter::Init with the two blitters
// that run, CopyRows and ConvertRows). Plain C++ over the blit object and the
// image descriptions in guest memory; the codecs made by ImageCodec::Create's
// plain C++ (codecs.h), the native heap (memory/c_runtime_heap.h) for the
// row, so only while --native_heap routes the game's heap to the layer
// (hooks::NativeHeapActive()). What never ran in play (the filters, the
// block copy, the codecs' functions other than the A8R8G8B8 decode, the DXT
// encode and the destructors) goes through a GuestCall at the stack the
// original calls it at.

#pragma once

#include <cstdint>
#include <optional>

#include "texture_compression/codecs.h"

namespace texture_compression {

// The blit object (12 bytes, on XGCompressSurface's stack).
namespace blit {
constexpr uint32_t kSource = 0;       // the source's codec
constexpr uint32_t kDestination = 4;  // the destination's codec
constexpr uint32_t kFilter = 8;       // the filter flags
}  // namespace blit

// The stack frames the blitter's functions set up (their callees run below).
constexpr uint32_t kReleaseFrameBytes = 96;
constexpr uint32_t kInitFrameBytes = 112;
constexpr uint32_t kCopyRowsFrameBytes = 144;
constexpr uint32_t kConvertRowsFrameBytes = 128;

// The functions the blitter reaches.
constexpr uint32_t kImageCodecDelete = 0x823F6B70;  // slot 0, the plain codecs
constexpr uint32_t kDxtCodecDelete = 0x823F6BC0;    // slot 0, the DXT codecs
constexpr uint32_t kDecodeA8R8G8B8Row = 0x823F2620;  // slot 1, A8R8G8B8
constexpr uint32_t kDxtEncodeRow = 0x823F6298;       // slot 2, DXT
constexpr uint32_t kCreate = 0x823F6C60;
constexpr uint32_t kPrepare = 0x823EAA90;
constexpr uint32_t kCopyRows = 0x82369860;
constexpr uint32_t kConvertRows = 0x823699F8;
constexpr uint32_t kCopyBlocks = 0x82368D98;
constexpr uint32_t kMemcpy = 0x825A7410;
// E_FAIL and D3DERR_INVALIDCALL.
constexpr uint32_t kFail = 0x80004005;
constexpr uint32_t kInvalidCall = 0x8876086C;

// sub_82368D20 Blitter::Blitter: +0 and +4 = 0 (+4 first).
void BlitterConstruct(uint8_t* base, uint32_t blitter);

// sub_82368D30 Blitter::Release: the destination's codec (+4), then the
// source's (+0), deleted through their slot 0 (r4 = 1) when not null; the
// words left as they are. `stack` is the guest r1 at entry (the deletes run
// at `stack` - 96). The original's r3: the last delete's, or 0 when the
// source's codec is null.
uint32_t BlitterRelease(uint8_t* base, uint32_t blitter, uint32_t stack, const GuestCall& call);

// sub_82369860 Blitter::CopyRows: when the codecs' formats match (+4, bit 8
// aside), the source has no color key (+24), the boxes' sizes (+104 to
// +112) and sRGB flags (+16) match and, with a palette (the destination's
// +28), the palettes (+56) are the same block or the same 1,024 bytes: a
// block format (the destination's +12 = 0) goes to Blitter::CopyBlocks
// (sub_82368D98, through `call`: never seen in play) and its r3 returned;
// else both +16 cleared and each row's bytes (+116) copied with memcpy
// (host side where c_runtime::Memcpy takes it, else sub_825A7410 through
// `call`), slice by slice, and 0 returned. E_FAIL when it doesn't apply (in
// play: the formats differ). `stack` is the guest r1 at entry (calls at
// `stack` - 144).
uint32_t BlitterCopyRows(uint8_t* base, uint32_t blitter, uint32_t stack, const GuestCall& call);

// sub_823699F8 Blitter::ConvertRows: when the boxes' sizes match, a float4
// row (the width x 16 bytes, operator new), both +16 cleared when both are
// set, and for each slice and row the source's slot 1 (decode: A8R8G8B8Codec::
// DecodeRow's plain C++ for sub_823F2620, else through `call`), then the
// destination's slot 2 (encode: DxtCodec::EncodeRow's plain C++ for
// sub_823F6298, else through `call`), r4 = the row, r5 = the slice, r6 = the
// float4s; operator delete on the row. 0, or E_FAIL when the sizes differ
// (or operator new gives 0). `stack` is the guest r1 at entry (calls at
// `stack` - 128). nullopt, with nothing done, for a row size operator new
// refuses (the whole original runs then).
std::optional<uint32_t> BlitterConvertRows(uint8_t* base, uint32_t blitter, uint32_t stack,
                                           const GuestCall& call);

// sub_8236C630 Blitter::Init: the filter at +8, +0 and +4 cleared; a filter
// outside 1-5 or with bits 0xFF800000 refused (OutputDebugStringA twice,
// D3DERR_INVALIDCALL); the source description's sRGB (+72, 0x200000), the
// destination's sRGB (+72, 0x400000), dither (+64, 0x80000) and diffusion
// (+68, 0x100000) from the filter; the destination's codec (+4) and the
// source's (+0) by ImageCodec::Create, ImageCodec::Prepare on them, then the
// blitters in turn until one returns 0 or more: CopyRows and ConvertRows
// (their plain C++), then the filters (sub_82369B48, sub_82369DB8,
// sub_82369FF8, sub_8236A4F0, sub_8236AB00, sub_8236AEA8, sub_8236BF40,
// sub_8236B7A0: through `call`, never seen in play); a codec not made or no
// blitter taking the pair: OutputDebugStringA twice and E_FAIL. Then both
// codecs deleted through slot 0 (the destination's first) and their words
// cleared. Create's, Prepare's, CopyRows', ConvertRows' and the destructors'
// plain C++ where they take the case, else their functions through `call`.
// `stack` is the guest r1 at entry (calls at `stack` - 112). The original's
// r3: 0, Prepare's error, E_FAIL or D3DERR_INVALIDCALL.
uint32_t BlitterInit(uint8_t* base, uint32_t blitter, uint32_t destination, uint32_t source,
                     uint32_t filter, uint32_t stack, const GuestCall& call);

}  // namespace texture_compression
