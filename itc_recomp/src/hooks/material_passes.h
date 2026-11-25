// The material passes' register steps (src/material_passes/,
// docs/functions/material-passes.md), shared by the adapters
// (src/hooks/material_passes.cpp, behind --native_material_passes) and the
// harness (without the flag). Each leaves r3 as the original does and runs
// the original where the plain C++ refuses.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Color::HsvToRgb sub_820D0D60: r3 = out, r4 = the color (r3 left).
void MaterialPassesHsvToRgb(lifted::Regs& ctx);
// sub_820D35D0: r3 = out, r4 = the index, r5 = the sub-index (r3 left).
void MaterialPassesTableVector(lifted::Regs& ctx);
// sub_820D3600 and sub_820D3638: r3 = a value; r3 = 1 or 0.
void MaterialPassesIsBlockSetA(lifted::Regs& ctx);
void MaterialPassesIsBlockSetB(lifted::Regs& ctx);
// sub_820D3728: r3 = an index; r3 = the record's address.
void MaterialPassesRecordAt(lifted::Regs& ctx);
// Matrix::TransposeRotation sub_820D1CB0: r3 = out, r4 = the matrix (r3
// left: memcpy's, the destination).
void MaterialPassesTransposeRotation(lifted::Regs& ctx);

// The passes: r3 = the part model; SetTexture sub_820D17D8 r4 = the
// texture, r5 = the shape (-1 every shape); FlipCulling sub_820D1950;
// ScaleBlended sub_820D1A68, SetPair1002Alpha sub_820D1B38 and SetAlpha
// sub_820D1C00 r4 = the shape, f1 = the value. r3 as the original leaves it.
void MaterialPassesSetTexture(lifted::Regs& ctx);
void MaterialPassesFlipCulling(lifted::Regs& ctx);
void MaterialPassesScaleBlended(lifted::Regs& ctx);
void MaterialPassesSetPair1002Alpha(lifted::Regs& ctx);
void MaterialPassesSetAlpha(lifted::Regs& ctx);

// PartModel::SetMaterials sub_820D3318: r3 = the part model, r4 to r10 and
// the caller's frame the arguments (material_passes.h); r3 as the original
// leaves it.
void MaterialPassesSetMaterials(lifted::Regs& ctx);

// PartModel::SetPaint sub_820D0EA8 and SetPaintBody sub_820D1378: r3 = the
// part model, r4 to r9 the three colors; ApplyPalette sub_820D2EA8: r3 =
// the part model. r3 as the original leaves it.
void MaterialPassesSetPaint(lifted::Regs& ctx);
void MaterialPassesSetPaintBody(lifted::Regs& ctx);
void MaterialPassesApplyPalette(lifted::Regs& ctx);
// PaintPalette::Init sub_820D2330: no arguments; r3 as the original leaves
// it.
void MaterialPassesPaintPaletteInit(lifted::Regs& ctx);

// Projector::Matrices sub_820D1EB8: r3 = the object, r4 = the matrix, r5 =
// the scale matrix (out), r6 = the view matrix (out), r7 = the point, r9 =
// the side; r3 = 0 or 1.
void MaterialPassesProjectorMatrices(lifted::Regs& ctx);

}  // namespace hooks
