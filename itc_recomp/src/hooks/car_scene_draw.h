// The car scene's draw's register steps (src/car_scene_draw/,
// docs/functions/car-scene-draw.md), shared by the adapters
// (src/hooks/car_scene_draw.cpp, behind --native_car_scene_draw) and the
// harness (without the flag). Each leaves r3 as the original does and the
// flush mode off (the original's last float step), and runs the original
// where the plain C++ declines.

#pragma once

#include "guest.h"

namespace hooks {

// Part 1: TextureProjection::Fit's geometry library. None of these has an
// adapter (their callers in play are native): the harness's steps.
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

// Part 2: the projectors and the root's leaves. Matrix::LookAt sub_82597378
// (r3 = out, r4 = eye, r5 = target, r6 = up) and Matrix::Perspective
// sub_825972A8 (r3 = out, f1 to f4 = field of view, aspect, near, far; r3
// as it was: the original's is tan's _ctrlfp word, read by no caller); no
// adapters (their callers are native): the harness's steps.
void CarSceneDrawLookAt(PPCContext& ctx, uint8_t* base);
void CarSceneDrawPerspective(PPCContext& ctx, uint8_t* base);
// The wrapped ones (their wraps in src/debug/frame_state_diff.cpp call
// these under the flag): Projector::Build sub_82597610 (r3 = out, r4 = eye,
// r5 = target, r6 = up, f1 to f4 = field of view, aspect, near, far) and
// BuildOrtho sub_82597518 (f1 to f4 = width, height, near, far), r3 = out,
// flush-to-zero left on (Matrix::Multiply's); ScenePass::SetLight
// sub_820C4EB8 (r3 = the pass description, r4 = the direction, f1 = the
// distance; r3 = memcpy's), SceneModel::BeamColor sub_820DAA38, Beam
// sub_820DAAC8 and BeamShape sub_820DAB10 (r3 = out, r4 = the scene model;
// r3 = out), CarScene::PlayerModel sub_820C4FB8 (r3 = the index, its high
// word dropped: callers pass lwz's).
void CarSceneDrawProjectorBuild(PPCContext& ctx, uint8_t* base);
void CarSceneDrawProjectorBuildOrtho(PPCContext& ctx, uint8_t* base);
void CarSceneDrawSetLight(PPCContext& ctx, uint8_t* base);
void CarSceneDrawBeamColor(PPCContext& ctx, uint8_t* base);
void CarSceneDrawBeam(PPCContext& ctx, uint8_t* base);
void CarSceneDrawBeamShape(PPCContext& ctx, uint8_t* base);
void CarSceneDrawPlayerModel(PPCContext& ctx, uint8_t* base);

// Part 3: TextureProjection::Fit sub_82598768 (wrapped: its wrap calls this
// under the flag): r3 = out, r4 = the camera pass, r5 = the direction, f1 to
// f5 = the high and low heights, the far cut, the least distance and the
// distance's scale; r3 as the original leaves it, flush-to-zero on.
void CarSceneDrawTextureProjectionFit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
