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
// Color::HsvToRgb8 sub_820C9808: r3 = out, r4 = the packed color (r3 left).
void MaterialPassesHsvToRgb8(PPCContext& ctx, uint8_t* base);
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

}  // namespace hooks
