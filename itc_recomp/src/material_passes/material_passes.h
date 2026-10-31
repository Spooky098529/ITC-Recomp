// The part models' material passes and the helpers beside them
// (docs/functions/material-passes.md) as plain C++ over guest memory: the
// originals' loads and stores in their order, their float steps as the
// translated code's (camera_modes/steps.h), with flush-to-zero off (the
// adapters' disableFlushMode). Stack temporaries the originals write below
// the stack pointer `sp` (their leftovers) are written the same way; their
// register saves aren't. Adapters: src/hooks/material_passes.cpp.

#pragma once

#include <cstdint>

namespace material_passes {

// --- Colors (colors.cpp) ---

// Color::HsvToRgb (sub_820D0D60): `color` (hue in degrees, saturation,
// value: three floats) into `out` as RGB with +12 = 1.0; saturation 0.0
// puts the value in +0 to +8 (+12 left); a sector outside 0-5 (a negative
// hue) writes nothing. The color is copied to sp - 16 and the sector's
// integers go through there, as the original's.
void HsvToRgb(uint8_t* base, uint32_t out, uint32_t color, uint32_t sp);
// Color::HsvToRgb8 (sub_820C9808): the packed color at `color` (+0 the hue,
// s16 degrees; +2 the saturation, +3 the value, bytes) into three bytes at
// `out` (+3 left).
void HsvToRgb8(uint8_t* base, uint32_t out, uint32_t color);

// --- Tables and matrices (helpers.cpp) ---

// sub_820D35D0: the 16 bytes at 0x82601A00 + 16 x (4 x index + sub) copied
// to `out` (two doublewords, each loaded then stored).
void TableVector(uint8_t* base, uint32_t out, uint32_t index, uint32_t sub);
// sub_820D3600: 1 for 1, 26, 28, 33 and 36, else 0.
uint32_t IsBlockSetA(int32_t value);
// sub_820D3638: 1 for 10 and 37, else 0.
uint32_t IsBlockSetB(int32_t value);
// sub_820D3728: 0x82003680 + 48 x index, as the original's 64-bit r3.
uint64_t RecordAt(uint64_t index);
// Matrix::TransposeRotation (sub_820D1CB0): the identity built in its frame
// (sp - 176: the back chain, the rows at +80), copied to `out` by memcpy,
// then out's upper 3x3 = m's transposed (each word through lfs and stfs).
// False, with only the frame written, where c_runtime::Memcpy refuses the
// copy (the original runs there).
bool TransposeRotation(uint8_t* base, uint32_t out, uint32_t m, uint32_t sp);

}  // namespace material_passes
