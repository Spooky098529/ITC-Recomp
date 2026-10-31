// The part models' material passes and the helpers beside them
// (docs/functions/material-passes.md) as plain C++ over guest memory: the
// originals' loads and stores in their order, their float steps as the
// translated code's (camera_modes/steps.h), with flush-to-zero off (the
// adapters' disableFlushMode). Stack temporaries the originals write below
// the stack pointer `sp` (their leftovers) are written the same way; their
// register saves aren't. Adapters: src/hooks/material_passes.cpp.

#pragma once

#include <cstdint>
#include <optional>

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

// --- The passes over a part model's materials (passes.cpp) ---
//
// Each takes the part model (nothing but the frame's back chain when 0) and
// the stack pointer (the back chain of the original's frame written), walks
// the materials of both groups and returns r3 as the original leaves it.

// PartModel::SetTexture (sub_820D17D8): +144 = 0 and +176 = `texture` on
// every material of `shape`, or of every shape for -1.
uint32_t SetTexture(uint8_t* base, uint32_t model, uint32_t texture, int32_t shape, uint32_t sp);
// PartModel::FlipCulling (sub_820D1950): +344 2 becomes 6 and 6 becomes 2,
// on every shape.
uint32_t FlipCulling(uint8_t* base, uint32_t model, uint32_t sp);
// PartModel::ScaleBlended (sub_820D1A68): group 1 of `shape`: +0 to +8
// times `factor` where +0 is below 1.1, then +12 = 1.0 and +108 = 2.
uint32_t ScaleBlended(uint8_t* base, uint32_t model, uint32_t shape, double factor, uint32_t sp);
// PartModel::SetPair1002Alpha (sub_820D1B38): +12 = `alpha` on the
// materials of `shape` whose +392 is 1002.
uint32_t SetPair1002Alpha(uint8_t* base, uint32_t model, uint32_t shape, double alpha,
                          uint32_t sp);
// PartModel::SetAlpha (sub_820D1C00): +12 = `alpha` on every material of
// `shape`.
uint32_t SetAlpha(uint8_t* base, uint32_t model, uint32_t shape, double alpha, uint32_t sp);

// --- A part model's materials' fields by a mask (set_materials.cpp) ---

// Model::MaterialTotal (sub_82594AC0): the shape's material block's +0 (the
// count of both groups).
uint32_t MaterialTotal(uint8_t* base, uint32_t model, uint64_t shape);
// Model::MaterialAt (sub_82594B00): the block + 16 + 512 x index, as the
// original's 64-bit sum.
uint64_t MaterialAt(uint8_t* base, uint32_t model, uint64_t shape, uint64_t index);
// PartModel::SetMaterialFields (sub_820D3250): its eight register arguments
// (`args`, r3 to r10) stored in its caller's frame `frame` (+16 to +80, as
// the original does), then the fields the six flags (the frame's +16 to +36)
// pick copied into the material (the frame's +116) from the frame: +0 the
// first float4 (+48), +32 the second (+64), +16 the third (+80), +80 to +88
// the three floats (+96 to +104).
void SetMaterialFields(uint8_t* base, uint32_t frame, const uint64_t args[8]);
// PartModel::SetMaterials (sub_820D3318): `regs` = r4 to r10 (the flags'
// three registers and the first two float4s); the rest from the caller's
// frame above `sp` (+96 the third float4's first half, +104 the 24 bytes
// copied for each material, +132 the shape, +140 the category, +148 the
// shared pair). Its frame's back chain and r7 to r10 stored at sp + 64 to
// sp + 88 as the original does; for each picked material, the material at
// its frame's +116, the 24 bytes copied by memcpy and SetMaterialFields.
// False where c_runtime::Memcpy refuses the copy (the original runs there,
// writing the same again); `r3` set to r3 as the original leaves it when a
// call changed it.
bool SetMaterials(uint8_t* base, uint32_t model, const uint64_t regs[7], uint32_t sp,
                  std::optional<uint64_t>* r3);

}  // namespace material_passes
