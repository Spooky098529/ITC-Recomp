// The DXT color block's helpers (DXT1's block and the color half of DXT2-5),
// from XGraphics' surface compression (docs/functions/texture-compression.md).
// Plain C++ over host arrays; the game's constants and tables are read from
// guest memory as the original reads them. The color block's encoder itself
// (DxtColor::EncodeBlock sub_82420F50) is still recompiled and reaches these
// through the adapters (src/hooks/texture_compression.cpp).

#pragma once

#include <cstdint>

namespace texture_compression {

// sub_824206E8 DxtColor::PackR5G6B5: red, green and blue each clamped to
// 0.0-1.0 (a NaN kept), times 31, 63 and 31 plus 0.5, truncated (fctiwz) and
// packed into 5:6:5 (red highest) as the original's 32-bit shifts do it (a
// NaN's 0x80000000 included). The original's r3.
uint16_t PackR5G6B5(uint8_t* base, const float color[3]);

// sub_82420AB8 DxtColor::FitEndpoints (probably): the two end colors of 16
// pixels (red, green, blue: the float4 pixels' first three lanes) for
// `colors` 3 or 4 (the adapter hands any other count to the original): the
// pixels' bounds (starting at the color weights at 0x82649B10, the scaled
// channels' tops, and 0.0), turned to
// the box diagonal that best follows the pixels, then refined up to 8 times
// by least squares against the steps between them (tables 0x8203EADC and
// 0x8203EAD0 for 3 colors, 0x8203EAC0 and 0x8203EAB0 for 4). Not clamped.
//
// `stack` is the guest stack pointer at the call (the original's r1): a
// pixel more than half a step below the low end gets a negative step index,
// and the original then reads its step's weights before the tables (guest
// memory, read here too) and its step's color below its steps' values on
// the stack (r1 - 256 + 16 x index): the words it wrote there are given as
// it leaves them, the others read from guest memory at `stack`.
void FitColorEndpoints(uint8_t* base, const float pixels[16][3], uint32_t colors, uint32_t stack,
                       float low[3], float high[3]);

// How many step indices FitColorEndpoints has read from the stack so far
// (for the harness's log: whether its cases reach that path).
uint64_t FitColorEndpointsStackReads();

}  // namespace texture_compression
