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
// VertexDeclarations::ForDepth (sub_82589970): r3 = the stream type.
void SceneryListsDepthVertexDeclaration(PPCContext& ctx, uint8_t* base);
// TextureProvider::ClearAll and Set (sub_82594328, sub_82594718): r3 = the
// provider (left); Set's r4 = the slot, r5 = the value.
void SceneryListsTextureProviderClearAll(PPCContext& ctx, uint8_t* base);
void SceneryListsTextureProviderSet(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
