// A model's named node turned into a point record, as plain C++
// (docs/functions/scene-loaders.md): ModelPoint::Find (sub_820D02D0). Guest
// data shared with the recompiled code, so it works on guest memory with the
// original's loads and stores in their order, its frame's words included
// (its saved registers and v127's slot aren't written). Adapter:
// src/hooks/scene_loaders.cpp.

#pragma once

#include <cstdint>
#include <functional>

namespace scene_loaders {

constexpr uint32_t kModelPointFrameBytes = 320;

// Skeleton::UpdateMatrices' original (r3 = the skeleton), from the frame
// with the original's return address, where its plain C++ declines.
using UpdateMatricesCall = std::function<void(uint32_t skeleton)>;

// ModelPoint::Find (sub_820D02D0): r3 = the point record, r4 = a part model.
// The record's +228 = -1, +0, +8, +4 = 0; the identity built in the frame
// (+80, 16-byte aligned rows) and copied to +16 (memcpy, 64 bytes). With the
// model's +408 set: the identity again, copied to the frame's +160 (eight
// doublewords) and turned about y by the float at 0x82002A50 (stored at the
// frame's +144; Matrix::RotateY, its turn at the frame's - 80); the skeleton
// (+112) posed (ResetPose, LocalMatrices, UpdateMatrices) and its nodes
// (NodeCount) searched for the record's +160 (NameMatches with [skeleton +
// 84] + 160n). Found: +4 = the model, +0 = the skeleton, +16 = the node's
// matrix (Matrix::Copy from [skeleton + 148] + 64n), the frame's +92, +108,
// +124, +140 = 1.0, +64 to +72 times the float at 0x82001FBC (0.1), the
// frame's +80 = +16 x the turned matrix (Matrix::Multiply), copied to +16,
// +144 = +64 (16 bytes), +80 = Matrix::Inverse(+16), +224 = n; returns 1.
// Else 0. The copies and the multiply noted for interpolation with the
// original's return addresses. `stack` is the guest r1 at entry. Only
// where memcpy is native (native::Buffers_NativeMemcpy, the adapter's
// check).
uint32_t ModelPointFind(uint8_t* base, uint32_t point, uint32_t model, uint32_t stack,
                        const UpdateMatricesCall& update_matrices);

}  // namespace scene_loaders
