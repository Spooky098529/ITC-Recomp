// The course set's lists' and the model draws' register steps
// (src/scenery_lists/, docs/functions/scenery-lists.md), shared by the
// adapters (src/hooks/scenery_lists.cpp, behind --native_scenery_lists) and
// the harness (without the flag). Each leaves r3 as the original does, and
// runs the original where the plain C++ declines.

#pragma once

#include "guest.h"

namespace hooks {

// ModelList::Release (sub_82147FB8): r3 = the list; the guest calls from a
// 128-byte frame like the original's.
void SceneryListsModelListRelease(PPCContext& ctx, uint8_t* base);
// ModelList::Load (sub_82148428): r3 = the list, r4 = the container;
// ModelList::LoadPoints (sub_82148528): the same. Natively only where
// Model::Load's adapter is (hooks::ModelsNativeLifetime), else the original;
// the callees' originals, where their plain C++ declines, guest calls from
// 512 bytes below r1 with the original's return addresses. Flush-to-zero
// off, as Model::Load leaves it.
void SceneryListsModelListLoad(PPCContext& ctx, uint8_t* base);
void SceneryListsModelListLoadPoints(PPCContext& ctx, uint8_t* base);
// SceneryModel::ResetDrawState (sub_82146AA8): r3 = the model (left).
void SceneryListsResetDrawState(PPCContext& ctx, uint8_t* base);
// AnimationSet::CurrentFrame (sub_82146B80): r3 = the set, r4 = the index.
void SceneryListsAnimationCurrentFrame(PPCContext& ctx, uint8_t* base);
// ModelList::IsBEntryOn, IsCEntryOn, PointList::IsEntryOn (sub_82147850,
// sub_82147958, sub_82147B18): r4 = the entry.
void SceneryListsIsBEntryOn(PPCContext& ctx, uint8_t* base);
void SceneryListsIsCEntryOn(PPCContext& ctx, uint8_t* base);
void SceneryListsPointListIsEntryOn(PPCContext& ctx, uint8_t* base);
// Race::CourseLayout (sub_8228F408): r3 = the layout (sign-extended).
void SceneryListsCourseLayout(PPCContext& ctx, uint8_t* base);
// PassDescription::SetBillboardPass (sub_82148650): r3 = the description, r4
// and r6 its words (r3 left).
void SceneryListsSetBillboardPass(PPCContext& ctx, uint8_t* base);
// VertexDeclarations::ForDepth (sub_82589970): r3 = the stream type. No
// adapter (the harness's steps).
void SceneryListsDepthVertexDeclaration(PPCContext& ctx, uint8_t* base);
// TextureProvider::ClearAll and Set (sub_82594328, sub_82594718): r3 = the
// provider (left); Set's r4 = the slot, r5 = the value.
void SceneryListsTextureProviderClearAll(PPCContext& ctx, uint8_t* base);
void SceneryListsTextureProviderSet(PPCContext& ctx, uint8_t* base);

// Model::IsShapeVisible (sub_825951E0): r3 = the model, r4 = the
// view-projection, r5 = the world matrix, r6 = the shape; the product in a
// 192-byte frame like the original's (at r1 - 96).
void SceneryListsIsShapeVisible(PPCContext& ctx, uint8_t* base);
// Recorder::RecordObject (sub_825953D0): r4 = the view-projection, r5 = the
// world matrix; r3 = the header slot. False, with nothing done, where the
// original must run (--native_scenery_lists off, the command list's
// diagnostics on, or the plain C++ declines). Called from its wrapper
// (src/interpolation/blend_state.cpp), inside the interpolation's hooks.
bool SceneryListsRecordObject(PPCContext& ctx, uint8_t* base);
// The same without the flag (the harness's steps): the original runs where
// the plain C++ declines.
void SceneryListsRecordObjectSteps(PPCContext& ctx, uint8_t* base);
// Recorder::RecordSceneLights (sub_82595490): r3 = the header slot.
void SceneryListsRecordSceneLights(PPCContext& ctx, uint8_t* base);

// Model::RecordShapeDepth (sub_825960D0): r3 = the model, r4 = the shape,
// r5 = the group, r7 = the mask, r8 = the byte; r3 = the chunk's first slot
// or 0.
void SceneryListsRecordShapeDepth(PPCContext& ctx, uint8_t* base);
// Model::DrawShapeDepth (sub_82596D48): r3 = the model, r4 = the
// view-projection, r5 = the world matrix, r6 = the shape, r7 = the group,
// r8 = the mask, r9 = the byte; the render queue's slot 4 a guest call from
// a 160-byte frame like the original's.
void SceneryListsDrawShapeDepth(PPCContext& ctx, uint8_t* base);

// SceneryModel::RecordShape (sub_82146BD0): r3 = the model, r4 = the
// shape, r5 = the group, r6 = whether to set the shaders, r7 = pixel c76,
// r8 = the range, r9 = the shared pair's offset; the guest calls from a
// 304-byte frame like the original's.
void SceneryListsRecordShape(PPCContext& ctx, uint8_t* base);
// SceneryModel::DrawShape (sub_82147D88): r3 = the model, r4 = the
// view-projection, r5 = the world matrix, r6 = the shape, r7 = the depth
// pass too, r8 = pixel c76, r9 = the range, r10 = the pair offset; from a
// 208-byte frame.
void SceneryListsDrawShape(PPCContext& ctx, uint8_t* base);

// Model::RecordShape (sub_82595550, behind --native_models): r3 = the
// model, r4 = the shape, r5 = the group, r6 = whether to set the shaders,
// r7 = pixel c76, r8 = the range; the guest calls from a 304-byte frame like
// the original's.
void SceneryListsModelRecordShape(PPCContext& ctx, uint8_t* base);
// Model::DrawShape (sub_82596B18): r3 = the model, r4 = the view-projection,
// r5 = the world matrix, r6 = the shape, r7 = the depth pass too, r8 =
// pixel c76, r9 = the range; from a 208-byte frame.
void SceneryListsModelDrawShape(PPCContext& ctx, uint8_t* base);

// PointList::Draw (sub_82148058): r3 = the list, r4 = the entry, r5 = the
// camera pass; from a 464-byte frame.
void SceneryListsPointListDraw(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
