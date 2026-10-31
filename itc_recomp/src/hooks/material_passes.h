// The material passes' register steps (src/material_passes/,
// docs/functions/material-passes.md), shared by the adapters
// (src/hooks/material_passes.cpp, behind --native_material_passes) and the
// harness (without the flag). Each leaves r3 as the original does and runs
// the original where the plain C++ refuses.

#pragma once

#include "guest.h"

namespace hooks {

// Color::HsvToRgb sub_820D0D60: r3 = out, r4 = the color (r3 left).
void MaterialPassesHsvToRgb(PPCContext& ctx, uint8_t* base);
// sub_820D35D0: r3 = out, r4 = the index, r5 = the sub-index (r3 left).
void MaterialPassesTableVector(PPCContext& ctx, uint8_t* base);
// sub_820D3600 and sub_820D3638: r3 = a value; r3 = 1 or 0.
void MaterialPassesIsBlockSetA(PPCContext& ctx, uint8_t* base);
void MaterialPassesIsBlockSetB(PPCContext& ctx, uint8_t* base);
// sub_820D3728: r3 = an index; r3 = the record's address.
void MaterialPassesRecordAt(PPCContext& ctx, uint8_t* base);
// Matrix::TransposeRotation sub_820D1CB0: r3 = out, r4 = the matrix (r3
// left: memcpy's, the destination).
void MaterialPassesTransposeRotation(PPCContext& ctx, uint8_t* base);

// The passes: r3 = the part model; SetTexture sub_820D17D8 r4 = the
// texture, r5 = the shape (-1 every shape); FlipCulling sub_820D1950;
// ScaleBlended sub_820D1A68, SetPair1002Alpha sub_820D1B38 and SetAlpha
// sub_820D1C00 r4 = the shape, f1 = the value. r3 as the original leaves it.
void MaterialPassesSetTexture(PPCContext& ctx, uint8_t* base);
void MaterialPassesFlipCulling(PPCContext& ctx, uint8_t* base);
void MaterialPassesScaleBlended(PPCContext& ctx, uint8_t* base);
void MaterialPassesSetPair1002Alpha(PPCContext& ctx, uint8_t* base);
void MaterialPassesSetAlpha(PPCContext& ctx, uint8_t* base);

// PartModel::SetMaterials sub_820D3318: r3 = the part model, r4 to r10 and
// the caller's frame the arguments (material_passes.h); r3 as the original
// leaves it.
void MaterialPassesSetMaterials(PPCContext& ctx, uint8_t* base);

// PartModel::SetPaint sub_820D0EA8 and SetPaintBody sub_820D1378: r3 = the
// part model, r4 to r9 the three colors; ApplyPalette sub_820D2EA8: r3 =
// the part model. r3 as the original leaves it.
void MaterialPassesSetPaint(PPCContext& ctx, uint8_t* base);
void MaterialPassesSetPaintBody(PPCContext& ctx, uint8_t* base);
void MaterialPassesApplyPalette(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
