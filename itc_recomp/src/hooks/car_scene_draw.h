// The car scene's draw's register steps (src/car_scene_draw/,
// docs/functions/car-scene-draw.md), shared by the adapters
// (src/hooks/car_scene_draw.cpp, behind --native_car_scene_draw) and the
// harness (without the flag). Each leaves r3 as the original does and the
// flush mode off (the original's last float step), and runs the original
// where the plain C++ declines.

#pragma once

#include "guest.h"

namespace hooks {

// Part 1: TextureProjection::Fit's geometry library (its only caller, Fit
// sub_82598768, still recompiled). Intersect3's, Extend's, Size's, Insert's,
// InsertN's and Allocate's steps have no adapter (their callers in play are
// native): the harness's.
// Frustum::FromMatrix sub_82597878: r3 = the frustum, r4 = the matrix.
void CarSceneDrawFrustumFromMatrix(PPCContext& ctx, uint8_t* base);
// Plane::Intersect3 sub_825976C8: r3 = out, r4, r5, r6 = the planes.
void CarSceneDrawPlaneIntersect3(PPCContext& ctx, uint8_t* base);
// Plane::SegmentCrossing sub_82597E58: r3 = out, r4 = the plane, r5 = b,
// r6 = a.
void CarSceneDrawPlaneSegmentCrossing(PPCContext& ctx, uint8_t* base);
// Box::FromPoints sub_82597210 (r3 = the box, r4 = the vector), Box::Extend
// sub_82597150 (r3 = the box, r4 = the point), Vector16::Size sub_825971E8
// (r3 = the vector).
void CarSceneDrawBoxFromPoints(PPCContext& ctx, uint8_t* base);
void CarSceneDrawBoxExtend(PPCContext& ctx, uint8_t* base);
void CarSceneDrawVector16Size(PPCContext& ctx, uint8_t* base);
// Vector16::PushBack sub_825986B8 (r3 = the vector, r4 = the value),
// Insert sub_82598648 (r3 = out, r4 = the vector, r5 = where, r6 = the
// value), InsertN sub_82598288 (r3 = the vector, r4 = where, r5 = n, r6 =
// the value) and Allocate sub_82598158 (r4 = the count).
void CarSceneDrawVector16PushBack(PPCContext& ctx, uint8_t* base);
void CarSceneDrawVector16Insert(PPCContext& ctx, uint8_t* base);
void CarSceneDrawVector16InsertN(PPCContext& ctx, uint8_t* base);
void CarSceneDrawVector16Allocate(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
