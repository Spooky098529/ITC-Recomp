// The course set's lists of models and the model draws natively: adapters in
// front of src/scenery_lists/ (docs/functions/scenery-lists.md), with the
// register steps the harness shares (hooks/scenery_lists.h). The parts the
// recompiler split off a function (sub_82147A20 of sub_82147958, sub_8228F484
// of sub_8228F408) are ported with it and have no adapters, nor do the
// functions only native callers reach (the flag switches their callers):
// VertexDeclarations::ForDepth (sub_82589970), from RecordShapeDepth, and
// ModelList::Load and LoadPoints (sub_82148428, sub_82148528), from
// CourseSet::Parse (src/course_scenery/), and ModelList::IsBEntryOn and
// IsCEntryOn (sub_82147850, sub_82147958), from Scenery::DrawSetLists
// (src/course_scenery/collect.cpp); their register steps are the harness's.

#include "hooks/scenery_lists.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/models.h"
#include "scenery_lists/scenery_lists.h"

#include <array>
#include <optional>

// The command list's diagnostics (blend_state.cpp) see who allocates slots:
// with any of them on, the recorders' originals run.
ITC_FLAG_DECLARE(bool, log_overlay_uploads);
ITC_FLAG_DECLARE(bool, log_slot_callers);
ITC_FLAG_DECLARE(bool, log_recording_by_pass);
ITC_FLAG_DECLARE(bool, native_models);

ITC_FLAG_BOOL(native_scenery_lists, true, "ITC",
              "Run the course set's lists of models and the model draws "
              "(docs/functions/scenery-lists.md: ModelList::Release sub_82147FB8 (Model::Release "
              "and the models' vtable slot 0 guest calls), ModelList::Load "
              "sub_82148428 and ModelList::LoadPoints sub_82148528 (no adapters: only "
              "CourseSet::Parse's plain C++ calls them, src/course_scenery/), SceneryModel::ResetDrawState "
              "sub_82146AA8, AnimationSet::CurrentFrame sub_82146B80, ModelList::IsBEntryOn "
              "sub_82147850 and ModelList::IsCEntryOn sub_82147958 (with sub_82147A20) (no adapters: "
              "Scenery::DrawSetLists' plain C++ calls them, src/course_scenery/), "
              "Race::CourseLayout sub_8228F408 (with sub_8228F484), PointList::IsEntryOn "
              "sub_82147B18, PassDescription::SetBillboardPass sub_82148650 (with sub_8209BD40), "
              "VertexDeclarations::ForDepth sub_82589970 (no adapter: only RecordShapeDepth's plain "
              "C++ calls it), Model::IsShapeVisible sub_825951E0, "
              "Recorder::RecordObject sub_825953D0 (inside its interpolation wrapper, "
              "src/interpolation/blend_state.cpp), Recorder::RecordSceneLights sub_82595490 "
              "(with SceneLights::RecordDirections sub_8258B980 and RecordProjectors "
              "sub_82589B58, no adapters: only it calls them), Model::RecordShapeDepth "
              "sub_825960D0, Model::DrawShapeDepth sub_82596D48 (RecordObject with the "
              "interpolation's hooks; the render queue's slot 4 a guest call), "
              "SceneryModel::RecordShape sub_82146BD0, SceneryModel::DrawShape sub_82147D88 and "
              "Model::DrawShape sub_82596B18 (with Model::RecordShape sub_82595550's plain C++; "
              "that one's adapter under --native_models) "
              "(the model's slot 1, the texture provider's slots 5 and 6 and the render "
              "queue's slot 4 guest calls; the material animation, never seen in play, the "
              "original's), TextureProvider::ClearAll "
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

namespace {

// ModelList::Load's guest calls where a callee's plain C++ declines, with
// the original's return addresses.
struct ModelListLoadGuest {
  PPCContext& ctx;
  uint8_t* base;

  static uint32_t Allocate(void* self, uint32_t size) {
    auto& c = *static_cast<ModelListLoadGuest*>(self);
    guest_call::InFrame(c.ctx, c.base, 512, 0x821484C0, [&](uint32_t) {
      c.ctx.r3.u64 = size;
      __imp__sub_825A6880(c.ctx, c.base);
    });
    return c.ctx.r3.u32;
  }
  static void Construct(void* self, uint32_t model) {
    auto& c = *static_cast<ModelListLoadGuest*>(self);
    guest_call::InFrame(c.ctx, c.base, 512, 0x821484D0, [&](uint32_t) {
      c.ctx.r3.u64 = model;
      __imp__sub_825948C0(c.ctx, c.base);
    });
  }
  static void Load(void* self, uint32_t model, uint32_t entry) {
    auto& c = *static_cast<ModelListLoadGuest*>(self);
    guest_call::InFrame(c.ctx, c.base, 512, 0x82148500, [&](uint32_t) {
      c.ctx.r3.u64 = model;
      c.ctx.r4.u64 = entry;
      c.ctx.r5.u64 = 0;
      __imp__sub_82596920(c.ctx, c.base);
    });
  }
};

template <typename Run>
void ModelListLoadSteps(PPCContext& ctx, uint8_t* base, uint32_t frame_bytes, Run run) {
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - frame_bytes;
  guest::StoreU32(base, frame, ctx.r1.u32);  // the back chain, as stwu
  ctx.fpscr.disableFlushMode();
  ModelListLoadGuest guest{ctx, base};
  hooks::ModelLoadGuest model_guest{ctx, base};
  scenery_lists::LoadCalls calls;
  calls.allocate = &ModelListLoadGuest::Allocate;
  calls.construct = &ModelListLoadGuest::Construct;
  calls.load = &ModelListLoadGuest::Load;
  calls.model_load = model_guest.Calls();
  calls.user = &guest;
  const uint32_t list = ctx.r3.u32, container = ctx.r4.u32;
  ctx.r3.u64 = run(list, container, frame, calls);
  ctx.lr = lr;
}

}  // namespace

void SceneryListsModelListLoad(PPCContext& ctx, uint8_t* base) {
  if (!ModelsNativeLifetime()) {
    __imp__sub_82148428(ctx, base);
    return;
  }
  ModelListLoadSteps(ctx, base, 160, [&](uint32_t list, uint32_t container, uint32_t frame,
                                         const scenery_lists::LoadCalls& calls) {
    return scenery_lists::ModelListLoad(base, list, container, frame, calls);
  });
}

void SceneryListsModelListLoadPoints(PPCContext& ctx, uint8_t* base) {
  if (!ModelsNativeLifetime()) {
    __imp__sub_82148528(ctx, base);
    return;
  }
  ModelListLoadSteps(ctx, base, 192, [&](uint32_t list, uint32_t container, uint32_t frame,
                                         const scenery_lists::LoadCalls& calls) {
    return scenery_lists::ModelListLoadPoints(base, list, container, frame, calls);
  });
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
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = scenery_lists::IsShapeVisible(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                             ctx.r6.u32, ctx.r1.u32 - 96);
  ctx.fpscr.disableFlushMode();
}

void SceneryListsRecordObjectSteps(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
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
  ctx.fpscr.disableFlushMode();
  const auto header = scenery_lists::RecordObject(base, ctx.r4.u32, ctx.r5.u32);
  if (!header) {
    return false;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *header;
  return true;
}

void SceneryListsRecordSceneLights(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto header =
      SlotDiagnostics() ? std::nullopt : scenery_lists::RecordSceneLights(base);
  if (!header) {
    __imp__sub_82595490(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *header;
}

void SceneryListsRecordShapeDepth(PPCContext& ctx, uint8_t* base) {
  const auto first =
      SlotDiagnostics() ? std::nullopt
                        : scenery_lists::RecordShapeDepth(base, ctx.r3.u32, ctx.r4.u32,
                                                          ctx.r5.s32, ctx.r7.u32, ctx.r8.u32);
  if (!first) {
    __imp__sub_825960D0(ctx, base);
    return;
  }
  ctx.r3.u64 = *first;
}

void SceneryListsDrawShapeDepth(PPCContext& ctx, uint8_t* base) {
  // The plain C++'s scalar float work expects flush-to-zero off, as the
  // original sets it.
  ctx.fpscr.disableFlushMode();
  constexpr uint32_t kFrame = 160;
  // What the recorder's wrapper would keep: the call's return address and
  // r24-r31 there (r9, r8, r7, r6, the caller's r28, the model, the world
  // matrix and the view-projection).
  const std::array<uint32_t, 9> context = {0x82596D9C,  ctx.r9.u32, ctx.r8.u32,
                                           ctx.r7.u32,  ctx.r6.u32, ctx.r28.u32,
                                           ctx.r3.u32,  ctx.r5.u32, ctx.r4.u32};
  const auto draw =
      SlotDiagnostics()
          ? std::nullopt
          : scenery_lists::DrawShapeDepth(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                          ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
                                          ctx.r1.u32 - kFrame, context);
  if (!draw) {
    __imp__sub_82596D48(ctx, base);
    return;
  }
  if (!draw->submit) {
    // r3 stays the model when its +28 is 0 (nothing ran).
    if (draw->r3 != ctx.r3.u32) {
      ctx.r3.u64 = draw->r3;
      ctx.fpscr.disableFlushMode();
    }
    return;
  }
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, kFrame, 0x82596E60, [&](uint32_t) {
    const uint32_t queue = guest::LoadU32(base, scenery_lists::kRenderQueue);
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = draw->depth;
    ctx.r4.u64 = draw->entry;
    ctx.r3.u64 = queue;
    ctx.r5.u64 = draw->key;
    REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, queue) + 16));
  });
  ctx.lr = lr;
}

namespace {

// The draws' guest calls (scenery_lists::DrawCalls), each from the frame and
// with the return address the plain C++ gives.
struct DrawContext {

  static uint32_t Call(void* self, uint32_t function, const uint32_t* args, int count,
                       uint32_t frame, uint32_t lr) {
    auto& d = *static_cast<SceneryDrawGuest*>(self);
    PPCContext& ctx = d.ctx;
    uint8_t* base = d.base;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    PPCRegister* registers[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                                 &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    for (int i = 0; i < count; i++) {
      registers[i]->u64 = args[i];
    }
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    // The plain C++'s scalar float work expects flush-to-zero off (the
    // originals set it before each float instruction).
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u32;
  }
  static uint32_t Submit(void* self, uint32_t entry, uint32_t key, double depth, uint32_t frame,
                         uint32_t lr) {
    auto& d = *static_cast<SceneryDrawGuest*>(self);
    PPCContext& ctx = d.ctx;
    uint8_t* base = d.base;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    const uint32_t queue = guest::LoadU32(base, scenery_lists::kRenderQueue);
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = depth;
    ctx.r4.u64 = entry;
    ctx.r3.u64 = queue;
    ctx.r5.u64 = key;
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, queue) + 16);
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u32;
  }
};

}  // namespace

bool SceneryListsSlotDiagnostics() {
  return SlotDiagnostics();
}

scenery_lists::DrawCalls SceneryListsDrawCalls(SceneryDrawGuest* guest) {
  scenery_lists::DrawCalls calls;
  calls.call = &DrawContext::Call;
  calls.submit = &DrawContext::Submit;
  calls.user = guest;
  return calls;
}

void SceneryListsRecordShape(PPCContext& ctx, uint8_t* base) {
  if (SlotDiagnostics()) {
    __imp__sub_82146BD0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - scenery_lists::kRecordShapeFrame;
  SceneryDrawGuest context{ctx, base};
  scenery_lists::DrawCalls calls;
  calls.call = &DrawContext::Call;
  calls.submit = &DrawContext::Submit;
  calls.user = &context;
  const uint32_t model = ctx.r3.u32, shape = ctx.r4.u32, set_shaders = ctx.r6.u32;
  const uint32_t c76 = ctx.r7.u32, range = ctx.r8.u32, pair_offset = ctx.r9.u32;
  const int32_t group = ctx.r5.s32;
  // The back chain first (as stwu): the plain C++ writes into the frame.
  const uint32_t chain = guest::LoadU32(base, frame);
  guest::StoreU32(base, frame, ctx.r1.u32);
  const auto first = scenery_lists::SceneryRecordShape(base, calls, frame, model, shape, group,
                                                       set_shaders, c76, range, pair_offset);
  if (!first) {
    guest::StoreU32(base, frame, chain);
    __imp__sub_82146BD0(ctx, base);
    return;
  }
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *first;
}

void SceneryListsDrawShape(PPCContext& ctx, uint8_t* base) {
  if (SlotDiagnostics()) {
    __imp__sub_82147D88(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - scenery_lists::kSceneryDrawShapeFrame;
  SceneryDrawGuest context{ctx, base};
  scenery_lists::DrawCalls calls;
  calls.call = &DrawContext::Call;
  calls.submit = &DrawContext::Submit;
  calls.user = &context;
  const uint32_t chain = guest::LoadU32(base, frame);
  guest::StoreU32(base, frame, ctx.r1.u32);
  const auto draw = scenery_lists::SceneryDrawShape(
      base, calls, frame, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
      ctx.r8.u32, ctx.r9.u32, ctx.r10.u32, ctx.r24.u32);
  if (!draw) {
    guest::StoreU32(base, frame, chain);
    __imp__sub_82147D88(ctx, base);
    return;
  }
  ctx.lr = lr;
  // r3 is the last queue call's, or as the original leaves it (the model
  // when its +28 is 0 and nothing ran).
  if (draw->queued) {
    ctx.fpscr.disableFlushMode();
  } else if (draw->r3 != ctx.r3.u32) {
    ctx.r3.u64 = draw->r3;
    ctx.fpscr.disableFlushMode();
  }
}

void SceneryListsModelRecordShape(PPCContext& ctx, uint8_t* base) {
  if (SlotDiagnostics()) {
    __imp__sub_82595550(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - scenery_lists::kModelRecordShapeFrame;
  SceneryDrawGuest context{ctx, base};
  scenery_lists::DrawCalls calls;
  calls.call = &DrawContext::Call;
  calls.submit = &DrawContext::Submit;
  calls.user = &context;
  // The back chain first (as stwu): the plain C++ writes into the frame.
  const uint32_t chain = guest::LoadU32(base, frame);
  guest::StoreU32(base, frame, ctx.r1.u32);
  const auto first = scenery_lists::ModelRecordShape(base, calls, frame, ctx.r3.u32, ctx.r4.u32,
                                                     ctx.r5.s32, ctx.r6.u32, ctx.r7.u32,
                                                     ctx.r8.u32);
  if (!first) {
    guest::StoreU32(base, frame, chain);
    __imp__sub_82595550(ctx, base);
    return;
  }
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *first;
}

void SceneryListsModelRecordMaterials(PPCContext& ctx, uint8_t* base) {
  if (SlotDiagnostics()) {
    __imp__sub_820CECA0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - scenery_lists::kModelRecordMaterialsFrame;
  SceneryDrawGuest context{ctx, base};
  const scenery_lists::DrawCalls calls = SceneryListsDrawCalls(&context);
  // The back chain first (as stwu): the plain C++ writes into the frame.
  const uint32_t chain = guest::LoadU32(base, frame);
  guest::StoreU32(base, frame, ctx.r1.u32);
  const auto first = scenery_lists::ModelRecordMaterials(base, calls, frame, ctx.r3.u32,
                                                         ctx.r4.u32, ctx.r5.s32, ctx.r6.u32);
  if (!first) {
    guest::StoreU32(base, frame, chain);
    __imp__sub_820CECA0(ctx, base);
    return;
  }
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *first;
}

void SceneryListsModelDrawShape(PPCContext& ctx, uint8_t* base) {
  if (SlotDiagnostics()) {
    __imp__sub_82596B18(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - scenery_lists::kModelDrawShapeFrame;
  SceneryDrawGuest context{ctx, base};
  scenery_lists::DrawCalls calls;
  calls.call = &DrawContext::Call;
  calls.submit = &DrawContext::Submit;
  calls.user = &context;
  const uint32_t chain = guest::LoadU32(base, frame);
  guest::StoreU32(base, frame, ctx.r1.u32);
  const auto draw = scenery_lists::ModelDrawShape(base, calls, frame, ctx.r3.u32, ctx.r4.u32,
                                                  ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                                  ctx.r8.u32, ctx.r9.u32, ctx.r24.u32);
  if (!draw) {
    guest::StoreU32(base, frame, chain);
    __imp__sub_82596B18(ctx, base);
    return;
  }
  ctx.lr = lr;
  // r3 is the last queue call's, or as the original leaves it (the model
  // when its +28 is 0 and nothing ran).
  if (draw->queued) {
    ctx.fpscr.disableFlushMode();
  } else if (draw->r3 != ctx.r3.u32) {
    ctx.r3.u64 = draw->r3;
    ctx.fpscr.disableFlushMode();
  }
}

void SceneryListsPointListDraw(PPCContext& ctx, uint8_t* base) {
  if (SlotDiagnostics()) {
    __imp__sub_82148058(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t frame = ctx.r1.u32 - scenery_lists::kPointListDrawFrame;
  SceneryDrawGuest context{ctx, base};
  scenery_lists::DrawCalls calls;
  calls.call = &DrawContext::Call;
  calls.submit = &DrawContext::Submit;
  calls.user = &context;
  const uint32_t chain = guest::LoadU32(base, frame);
  guest::StoreU32(base, frame, ctx.r1.u32);
  const auto r3 = scenery_lists::PointListDraw(base, calls, frame, ctx.r3.u32, ctx.r4.u32,
                                               ctx.r5.u32, ctx.r24.u32, ctx.r3.u32);
  if (!r3) {
    guest::StoreU32(base, frame, chain);
    __imp__sub_82148058(ctx, base);
    return;
  }
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *r3;
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
ITC_SCENERY_LISTS(8228F408, SceneryListsCourseLayout)
ITC_SCENERY_LISTS(82147B18, SceneryListsPointListIsEntryOn)
ITC_SCENERY_LISTS(82148650, SceneryListsSetBillboardPass)
ITC_SCENERY_LISTS(825951E0, SceneryListsIsShapeVisible)
ITC_SCENERY_LISTS(82595490, SceneryListsRecordSceneLights)
ITC_SCENERY_LISTS(825960D0, SceneryListsRecordShapeDepth)
ITC_SCENERY_LISTS(82596D48, SceneryListsDrawShapeDepth)
ITC_SCENERY_LISTS(82146BD0, SceneryListsRecordShape)
ITC_SCENERY_LISTS(82147D88, SceneryListsDrawShape)
ITC_SCENERY_LISTS(82148058, SceneryListsPointListDraw)
ITC_SCENERY_LISTS(82596B18, SceneryListsModelDrawShape)
ITC_SCENERY_LISTS(82594328, SceneryListsTextureProviderClearAll)
ITC_SCENERY_LISTS(82594718, SceneryListsTextureProviderSet)

// Model::RecordShape's row is in docs/functions/models.md: behind that file's
// flag.
extern "C" REX_FUNC(sub_82595550) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_82595550(ctx, base);
    return;
  }
  hooks::SceneryListsModelRecordShape(ctx, base);
}

// Model::RecordMaterials' row is in docs/functions/models.md too.
extern "C" REX_FUNC(sub_820CECA0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_820CECA0(ctx, base);
    return;
  }
  hooks::SceneryListsModelRecordMaterials(ctx, base);
}
