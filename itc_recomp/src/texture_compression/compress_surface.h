// XGraphics' XGCompressSurface and XGGetBlockDimensions
// (docs/functions/texture-compression.md): plain C++ over the image
// descriptions and the blit object on the guest stack where the original
// keeps them (the codecs read them there, and the words it never writes are
// the stack's leftovers, as in the original). Only while --native_heap
// routes the game's heap to the layer (hooks::NativeHeapActive()).

#pragma once

#include <cstdint>
#include <optional>

#include "texture_compression/codecs.h"
#include "texture_compression/dxt45.h"

namespace texture_compression {

// XGCompressSurface's stack frame and where it keeps its locals (offsets
// from its r1): the filter, the destination point when none is given, the
// blit object, the two descriptions, the source rectangle when none is
// given and the block's size. Its caller's frame holds the stack arguments
// (+84 the source format, +92 the source rectangle, +100 the flags) and the
// five words it saves there (+20, +28, +36, +44, +52: r3 to r7).
namespace compress {
constexpr uint32_t kFrameBytes = 560;
constexpr uint32_t kFilter = 128;
constexpr uint32_t kPoint = 136;
constexpr uint32_t kBlitter = 144;
constexpr uint32_t kDestination = 160;  // the destination's description (84 bytes)
constexpr uint32_t kRect = 256;
constexpr uint32_t kBlockHeight = 272;
constexpr uint32_t kBlockWidth = 276;
constexpr uint32_t kSource = 288;  // the source's description (84 bytes)
// The caller's frame.
constexpr uint32_t kSourceFormat = 84;
constexpr uint32_t kSourceRect = 92;
constexpr uint32_t kFlags = 100;
}  // namespace compress

// XGCompressSurface's register arguments.
struct CompressArguments {
  uint32_t destination;   // r3
  uint32_t pitch;         // r4
  uint32_t width;         // r5
  uint32_t height;        // r6
  uint32_t format;        // r7, the destination's GPU format word
  uint32_t point;         // r8, the destination point (null: 0, 0)
  uint32_t source;        // r9
  uint32_t source_pitch;  // r10
  double alpha_reference;  // f1
};

// The alpha reference (0x8263B7E8, a float) XGCompressSurface sets for the
// DXT block encoders, with the premultiplied switch (kPremultiplied,
// dxt45.h).
constexpr uint32_t kAlphaReference = 0x8263B7E8;

// sub_8236D3C8 XGGetBlockDimensions: GpuFormat::BlockSize (sub_822A4410's
// plain C++) for the GPU format `format`, its width stored at `width` and
// height at `height` where they aren't null. The original's r3: `format`
// (what BlockSize leaves).
uint32_t GetBlockDimensions(uint8_t* base, uint32_t format, uint32_t width, uint32_t height);

// sub_8235AAC0 XGCompressSurface: r3 to r7 saved in the caller's frame; the
// blit object made (Blitter::Blitter); the filter triangle with dither
// (0x80004), or triangle (4) with flags 0x10; the source rectangle (null:
// 0, 0, width, height) and destination point (null: 0, 0) on its stack; the
// block's size (XGGetBlockDimensions); the destination's description (the
// data, the format by the GPU format: DXT1 0x1A200052 for 18 and 51, DXT2/3
// ...53 for 19 and 52, DXT4/5 ...54 for 20 and 53, DXT3A ...7A for 58, DXT5A
// ...7B for 59, YUV 0x1A20004B/4C for 11 and 12 with flags 4, else
// 0x1828004B/4C; another format leaves the stack's word), the source's
// (A8R8G8B8 0x18280086 for GPU format 6, else A32B32G32R32F 0x1A22AAA6; its
// +64 and +68 left as the stack has them), the alpha reference and the
// premultiplied switch (flags & 1), Blitter::Init's plain C++ with the
// filter, Blitter::Release's. `stack` is the guest r1 at entry (its caller's
// frame; its callees run at `stack` - 560). The original's r3: Init's.
// nullopt, with nothing done, for what never ran in play (the whole original
// runs then): DXN (49) and CTX1 (60) destinations (XGCopySurface into an
// XMemAlloc buffer), a source that must be copied first (not 8-in-32, or
// its channels not in the standard order: HeapAlloc and
// XGEndianSwapSurface), a DXT or YUV destination that must be swapped after
// (not 8-in-16).
std::optional<uint32_t> CompressSurface(uint8_t* base, const CompressArguments& arguments,
                                        uint32_t stack, const GuestCall& call);

}  // namespace texture_compression
