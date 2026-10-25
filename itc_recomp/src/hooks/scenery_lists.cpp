// The course set's lists of models and the model draws natively: adapters in
// front of src/scenery_lists/ (docs/functions/scenery-lists.md), with the
// register steps the harness shares (hooks/scenery_lists.h). The parts the
// recompiler split off a function (sub_82147A20 of sub_82147958, sub_8228F484
// of sub_8228F408) are ported with it and have no adapters.

#include "hooks/scenery_lists.h"

#include "base/flags.h"
#include "scenery_lists/scenery_lists.h"

#include <optional>

// The command list's diagnostics (blend_state.cpp) see who allocates slots:
// with any of them on, the recorders' originals run.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);

ITC_FLAG_BOOL(native_scenery_lists, true, "ITC",
              "Run the course set's lists of models and the model draws "
              "(docs/functions/scenery-lists.md: ModelList::Release sub_82147FB8 (Model::Release "
              "and the models' vtable slot 0 guest calls), SceneryModel::ResetDrawState "
              "sub_82146AA8, AnimationSet::CurrentFrame sub_82146B80, ModelList::IsBEntryOn "
              "sub_82147850, ModelList::IsCEntryOn sub_82147958 (with sub_82147A20), "
              "Race::CourseLayout sub_8228F408 (with sub_8228F484), PointList::IsEntryOn "
              "sub_82147B18, PassDescription::SetBillboardPass sub_82148650 (with sub_8209BD40), "
              "VertexDeclarations::ForDepth sub_82589970, Model::IsShapeVisible sub_825951E0, "
              "Recorder::RecordObject sub_825953D0 (inside its interpolation wrapper, "
              "src/interpolation/blend_state.cpp), Recorder::RecordSceneLights sub_82595490 "
              "(with SceneLights::RecordDirections sub_8258B980 and RecordProjectors "
              "sub_82589B58, no adapters: only it calls them), TextureProvider::ClearAll "
              "sub_82594328 and TextureProvider::Set sub_82594718 (its reserve, debug prints "
              "and resize, never seen in play, the original's)) natively instead of through "
              "the translated code (the same writes; checked by "
              "--run_harnesses=scenery_lists)");

namespace hooks {

namespace {

// ModelList::Release's guest calls, from its frame with the original's
// return addresses.
struct ReleaseContext {
  PPCContext& ctx;
  uint8_t* base;
  uint32_t frame;

  uint32_t Call(uint32_t model, uint32_t lr, bool destroy) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    ctx.r3.u64 = model;
    if (destroy) {
      ctx.r4.s64 = 1;
      REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, model)));
    } else {
      sub_82594970(ctx, base);
    }
    ctx.r1.u64 = stack;
    return ctx.r3.u32;
  }
  static uint32_t Release(void* self, uint32_t model) {
    return static_cast<ReleaseContext*>(self)->Call(model, 0x82148020, false);
  }
  static uint32_t Destroy(void* self, uint32_t model) {
    return static_cast<ReleaseContext*>(self)->Call(model, 0x82148040, true);
  }
};

}  // namespace

void SceneryListsModelListRelease(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - 128;
  guest::StoreU32(base, frame, ctx.r1.u32);  // the back chain, as stwu
  ReleaseContext context{ctx, base, frame};
  scenery_lists::ReleaseCalls calls;
  calls.release = &ReleaseContext::Release;
  calls.destroy = &ReleaseContext::Destroy;
  calls.user = &context;
  // r3 is left as the last guest call leaves it (the list without models).
  scenery_lists::ModelListRelease(base, ctx.r3.u32, calls);
  ctx.lr = lr;
}

void SceneryListsResetDrawState(PPCContext& ctx, uint8_t* base) {
  scenery_lists::ResetDrawState(base, ctx.r3.u32);
}

void SceneryListsAnimationCurrentFrame(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = scenery_lists::AnimationCurrentFrame(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneryListsIsBEntryOn(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = scenery_lists::IsBEntryOn(base, ctx.r4.s32);
}

void SceneryListsIsCEntryOn(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = scenery_lists::IsCEntryOn(base, ctx.r4.u32);
}

void SceneryListsPointListIsEntryOn(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = scenery_lists::PointListIsEntryOn(base, ctx.r4.s32);
}

void SceneryListsCourseLayout(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = scenery_lists::CourseLayout(base);
}

void SceneryListsSetBillboardPass(PPCContext& ctx, uint8_t* base) {
  scenery_lists::SetBillboardPass(base, ctx.r3.u32, ctx.r4.u32, ctx.r6.u32);
}

void SceneryListsDepthVertexDeclaration(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = scenery_lists::DepthVertexDeclaration(base, ctx.r3.s32);
}

void SceneryListsTextureProviderClearAll(PPCContext& ctx, uint8_t* base) {
  scenery_lists::TextureProviderClearAll(base, ctx.r3.u32);
}

void SceneryListsTextureProviderSet(PPCContext& ctx, uint8_t* base) {
  if (!scenery_lists::TextureProviderSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_82594718(ctx, base);
  }
}

namespace {

bool SlotDiagnostics() {
  return ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
         ITC_FLAG(log_recording_by_pass);
}

}  // namespace

void SceneryListsIsShapeVisible(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = scenery_lists::IsShapeVisible(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                             ctx.r6.u32, ctx.r1.u32 - 96);
  ctx.fpscr.disableFlushMode();
}

void SceneryListsRecordObjectSteps(PPCContext& ctx, uint8_t* base) {
  const auto header = SlotDiagnostics()
                          ? std::nullopt
                          : scenery_lists::RecordObject(base, ctx.r4.u32, ctx.r5.u32);
  if (!header) {
    __imp__sub_825953D0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *header;
}

bool SceneryListsRecordObject(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_scenery_lists) || SlotDiagnostics()) {
    return false;
  }
  const auto header = scenery_lists::RecordObject(base, ctx.r4.u32, ctx.r5.u32);
  if (!header) {
    return false;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *header;
  return true;
}

void SceneryListsRecordSceneLights(PPCContext& ctx, uint8_t* base) {
  const auto header =
      SlotDiagnostics() ? std::nullopt : scenery_lists::RecordSceneLights(base);
  if (!header) {
    __imp__sub_82595490(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *header;
}

}  // namespace hooks

#define ITC_SCENERY_LISTS(address, steps)    \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_scenery_lists)) {   \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    hooks::steps(ctx, base);                 \
  }

ITC_SCENERY_LISTS(82147FB8, SceneryListsModelListRelease)
ITC_SCENERY_LISTS(82146AA8, SceneryListsResetDrawState)
ITC_SCENERY_LISTS(82146B80, SceneryListsAnimationCurrentFrame)
ITC_SCENERY_LISTS(82147850, SceneryListsIsBEntryOn)
ITC_SCENERY_LISTS(82147958, SceneryListsIsCEntryOn)
ITC_SCENERY_LISTS(8228F408, SceneryListsCourseLayout)
ITC_SCENERY_LISTS(82147B18, SceneryListsPointListIsEntryOn)
ITC_SCENERY_LISTS(82148650, SceneryListsSetBillboardPass)
ITC_SCENERY_LISTS(82589970, SceneryListsDepthVertexDeclaration)
ITC_SCENERY_LISTS(825951E0, SceneryListsIsShapeVisible)
ITC_SCENERY_LISTS(82595490, SceneryListsRecordSceneLights)
ITC_SCENERY_LISTS(82594328, SceneryListsTextureProviderClearAll)
ITC_SCENERY_LISTS(82594718, SceneryListsTextureProviderSet)
