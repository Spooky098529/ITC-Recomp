// The car scene's draw as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/car-scene-draw.md, the fields in
// docs/known-data.md). The data is guest data shared with the recompiled
// code, so these work on guest memory with the originals' loads and stores
// in their order: the scalar float work as the translated code's doubles
// rounded to single (fused where the original fuses), the vector work on SSE
// vectors with flush-to-zero on as the translation turns it on (built with
// -ffp-model=strict, CMakeLists.txt). Each takes `stack`, the guest r1 at
// entry, and keeps the original's frame: its back chain written as stwu does
// (not its saved registers), its temporaries at the original's offsets (the
// leaves' below r1, as the original's), its callees run from it. Expect
// flush-to-zero off at entry (the adapters turn it off) and leave it off.
// Adapters: src/hooks/car_scene_draw.cpp; a function that declines (nullopt,
// with nothing done that the original wouldn't redo the same) falls back to
// its original there.
//
// Part 1: the geometry library under TextureProjection::Fit (sub_82598768,
// still recompiled): the frustum, the planes' crossings, the box over a
// std::vector of 16-byte points and the vector's growth.

#pragma once

#include <cstdint>
#include <optional>

namespace car_scene_draw {

// --- The frustum and the planes (geometry.cpp) ---

// sub_825976C8 Plane::Intersect3: out = the point where the planes a, b and
// c (x, y, z, d) meet, w 1.0; 1, or 0 with out unwritten when their triple
// product is 0, denormal, infinite or NaN. Its temporaries below `stack`.
uint32_t PlaneIntersect3(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t c,
                         uint32_t stack);
// sub_82597878 Frustum::FromMatrix: the 256-byte frustum at `out` from the
// matrix (its six planes, their sign codes and eight corners). Returns out.
uint32_t FrustumFromMatrix(uint8_t* base, uint32_t out, uint32_t matrix, uint32_t stack);
// sub_82597E58 Plane::SegmentCrossing: out = where the line from a through b
// crosses the plane, w 1.0; 1, or 0 with out unwritten when it runs along
// the plane. Its temporaries below `stack`.
uint32_t PlaneSegmentCrossing(uint8_t* base, uint32_t out, uint32_t plane, uint32_t b,
                              uint32_t a, uint32_t stack);

// --- The box (geometry.cpp) ---

// sub_825971E8 Vector16::Size: (last - first) / 16 (srawi), 0 with no storage.
uint32_t Vector16Size(uint8_t* base, uint32_t vector);
// sub_82597150 Box::Extend: the 32-byte box (min, max) grown to the point.
void BoxExtend(uint8_t* base, uint32_t box, uint32_t point);
// sub_82597210 Box::FromPoints: the box over the vector's points. Returns box.
uint32_t BoxFromPoints(uint8_t* base, uint32_t box, uint32_t vector, uint32_t stack);

// --- std::vector of 16-byte elements (vector16.cpp) ---
// The vector: +4 first, +8 last, +12 end of storage. Each returns what the
// original leaves in r3, or nullopt (nothing done) where the original takes
// a path the plain C++ leaves to it: _Xlen (sub_825981C8) and bad_alloc's
// copy (sub_82091200), never run in play, and operator new's and delete's
// error paths (or the native heap off).

// sub_82598158 Vector16::Allocate: operator new(16 x count).
std::optional<uint32_t> Vector16Allocate(uint8_t* base, uint32_t count, uint32_t stack);
// sub_82598288 Vector16::InsertN: n copies of the 16 bytes at `value` at
// `where`.
std::optional<uint32_t> Vector16InsertN(uint8_t* base, uint32_t vector, uint32_t where,
                                        uint32_t n, uint32_t value, uint32_t stack);
// sub_82598648 Vector16::Insert: one copy at `where`; the iterator to it at
// `out`. Returns out.
std::optional<uint32_t> Vector16Insert(uint8_t* base, uint32_t out, uint32_t vector,
                                       uint32_t where, uint32_t value, uint32_t stack);
// sub_825986B8 Vector16::PushBack.
std::optional<uint32_t> Vector16PushBack(uint8_t* base, uint32_t vector, uint32_t value,
                                         uint32_t stack);

}  // namespace car_scene_draw
