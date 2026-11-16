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
#include "lifted/entries.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/models.h"
#include "scenery_lists/scenery_lists.h"

#include <array>
#include <optional>
#include "lifted/sdk.h"

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
              "sub_82146AA8 (no adapter), AnimationSet::CurrentFrame sub_82146B80, ModelList::IsBEntryOn "
              "sub_82147850 and ModelList::IsCEntryOn sub_82147958 (with sub_82147A20) (no adapters: "
              "Scenery::DrawSetLists' plain C++ calls them, src/course_scenery/), "
              "Race::CourseLayout sub_8228F408 (with sub_8228F484), PointList::IsEntryOn "
              "sub_82147B18, PassDescription::SetBillboardPass sub_82148650 (with sub_8209BD40; no adapter), "
              "VertexDeclarations::ForDepth sub_82589970 (no adapter: only RecordShapeDepth's plain "
              "C++ calls it), Model::IsShapeVisible sub_825951E0, "
              "Recorder::RecordObject sub_825953D0 (inside its interpolation wrapper, "
              "src/interpolation/blend_state.cpp), Recorder::RecordSceneLights sub_82595490 "
              "(with SceneLights::RecordDirections sub_8258B980 and RecordProjectors "
              "sub_82589B58, no adapters: only it calls them), Model::RecordShapeDepth "
              "sub_825960D0, Model::DrawShapeDepth sub_82596D48 (no adapter; RecordObject with the "
              "interpolation's hooks; the render queue's slot 4 a guest call), "
              "SceneryModel::RecordShape sub_82146BD0, SceneryModel::DrawShape sub_82147D88 (no adapter) and "
              "Model::DrawShape sub_82596B18 (no adapter; with Model::RecordShape sub_82595550's plain C++; "
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
  lifted::Regs& ctx;
  uint8_t* base;
  uint32_t frame;

  uint32_t Call(uint32_t model, uint32_t lr, bool destroy) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    ctx.r3.u64 = model;
    if (destroy) {
      ctx.r4.s64 = 1;
      lifted::CallFromContext(ctx, base, guest::LoadU32(base, guest::LoadU32(base, model)));
    } else {
      lifted::Enter(ctx, base, lifted::entry::e82594970);
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

ITC_REGS_STEP(SceneryListsModelListRelease) {
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
  lifted::Regs& ctx;
  uint8_t* base;

  static uint32_t Allocate(void* self, uint32_t size) {
    auto& c = *static_cast<ModelListLoadGuest*>(self);
    guest_call::InFrame(c.ctx, c.base, 512, 0x821484C0, [&](uint32_t) {
      c.ctx.r3.u64 = size;
      lifted::Sdk(c.ctx, c.base, lifted::original::c825A6880);
    });
    return c.ctx.r3.u32;
  }
  static void Construct(void* self, uint32_t model) {
    auto& c = *static_cast<ModelListLoadGuest*>(self);
    guest_call::InFrame(c.ctx, c.base, 512, 0x821484D0, [&](uint32_t) {
      c.ctx.r3.u64 = model;
      lifted::Sdk(c.ctx, c.base, lifted::original::c825948C0);
    });
  }
  static void Load(void* self, uint32_t model, uint32_t entry) {
    auto& c = *static_cast<ModelListLoadGuest*>(self);
    guest_call::InFrame(c.ctx, c.base, 512, 0x82148500, [&](uint32_t) {
      c.ctx.r3.u64 = model;
      c.ctx.r4.u64 = entry;
      c.ctx.r5.u64 = 0;
      lifted::original::c82596920(c.ctx, c.base);
    });
  }
};

template <typename Run>
void ModelListLoadSteps(lifted::Regs& ctx, uint8_t* base, uint32_t frame_bytes, Run run) {
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

ITC_REGS_STEP(SceneryListsModelListLoad) {
  if (!ModelsNativeLifetime()) {
    lifted::original::c82148428(ctx, base);
    return;
  }
  ModelListLoadSteps(ctx, base, 160, [&](uint32_t list, uint32_t container, uint32_t frame,
                                         const scenery_lists::LoadCalls& calls) {
    return scenery_lists::ModelListLoad(base, list, container, frame, calls);
  });
}

ITC_REGS_STEP(SceneryListsModelListLoadPoints) {
  if (!ModelsNativeLifetime()) {
    lifted::original::c82148528(ctx, base);
    return;
  }
  ModelListLoadSteps(ctx, base, 192, [&](uint32_t list, uint32_t container, uint32_t frame,
                                         const scenery_lists::LoadCalls& calls) {
    return scenery_lists::ModelListLoadPoints(base, list, container, frame, calls);
  });
}

ITC_REGS_STEP(SceneryListsResetDrawState) {
  scenery_lists::ResetDrawState(base, ctx.r3.u32);
}

ITC_REGS_STEP(SceneryListsAnimationCurrentFrame) {
  ctx.r3.u64 = scenery_lists::AnimationCurrentFrame(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneryListsIsBEntryOn) {
  ctx.r3.u64 = scenery_lists::IsBEntryOn(base, ctx.r4.s32);
}

ITC_REGS_STEP(SceneryListsIsCEntryOn) {
  ctx.r3.u64 = scenery_lists::IsCEntryOn(base, ctx.r4.u32);
}

ITC_REGS_STEP(SceneryListsPointListIsEntryOn) {
  ctx.r3.u64 = scenery_lists::PointListIsEntryOn(base, ctx.r4.s32);
}

ITC_REGS_STEP(SceneryListsCourseLayout) {
  ctx.r3.s64 = scenery_lists::CourseLayout(base);
}

ITC_REGS_STEP(SceneryListsSetBillboardPass) {
  scenery_lists::SetBillboardPass(base, ctx.r3.u32, ctx.r4.u32, ctx.r6.u32);
}

ITC_REGS_STEP(SceneryListsDepthVertexDeclaration) {
  ctx.r3.u64 = scenery_lists::DepthVertexDeclaration(base, ctx.r3.s32);
}

ITC_REGS_STEP(SceneryListsTextureProviderClearAll) {
  scenery_lists::TextureProviderClearAll(base, ctx.r3.u32);
}

ITC_REGS_STEP(SceneryListsTextureProviderSet) {
  if (!scenery_lists::TextureProviderSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    lifted::original::c82594718(ctx, base);
  }
}

namespace {

bool SlotDiagnostics() {
  return ITC_FLAG(log_overlay_uploads) || ITC_FLAG(log_slot_callers) ||
         ITC_FLAG(log_recording_by_pass);
}

}  // namespace

ITC_REGS_STEP(SceneryListsIsShapeVisible) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = scenery_lists::IsShapeVisible(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                             ctx.r6.u32, ctx.r1.u32 - 96);
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(SceneryListsRecordObjectSteps) {
  ctx.fpscr.disableFlushMode();
  const auto header = SlotDiagnostics()
                          ? std::nullopt
                          : scenery_lists::RecordObject(base, ctx.r4.u32, ctx.r5.u32);
  if (!header) {
    lifted::original::c825953D0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *header;
}

bool SceneryListsRecordObject(lifted::Regs& ctx, uint8_t* base) {
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

ITC_REGS_STEP(SceneryListsRecordSceneLights) {
  ctx.fpscr.disableFlushMode();
  const auto header =
      SlotDiagnostics() ? std::nullopt : scenery_lists::RecordSceneLights(base);
  if (!header) {
    lifted::original::c82595490(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *header;
}

ITC_REGS_STEP(SceneryListsRecordShapeDepth) {
  const auto first =
      SlotDiagnostics() ? std::nullopt
                        : scenery_lists::RecordShapeDepth(base, ctx.r3.u32, ctx.r4.u32,
                                                          ctx.r5.s32, ctx.r7.u32, ctx.r8.u32);
  if (!first) {
    lifted::original::c825960D0(ctx, base);
    return;
  }
  ctx.r3.u64 = *first;
}

ITC_REGS_STEP(SceneryListsDrawShapeDepth) {
  // The plain C++'s scalar float work expects flush-to-zero off, as the
  // original sets it.
  ctx.fpscr.disableFlushMode();
  constexpr uint32_t kFrame = 160;
  // What the recorder's wrapper would keep: the call's return address and
  // r24-r31 there (r9, r8, r7, r6, the caller's r28, the model, the world
  // matrix and the view-projection).
  // (the caller's r28: diagnostic only, not carried)
  const std::array<uint32_t, 9> context = {0x82596D9C,  ctx.r9.u32, ctx.r8.u32,
                                           ctx.r7.u32,  ctx.r6.u32, 0,
                                           ctx.r3.u32,  ctx.r5.u32, ctx.r4.u32};
  const auto draw =
      SlotDiagnostics()
          ? std::nullopt
          : scenery_lists::DrawShapeDepth(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                          ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
                                          ctx.r1.u32 - kFrame, context);
  if (!draw) {
    lifted::original::c82596D48(ctx, base);
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
    lifted::CallFromContext(ctx, base, guest::LoadU32(base, guest::LoadU32(base, queue) + 16));
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
    lifted::Regs& ctx = d.ctx;
    uint8_t* base = d.base;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    lifted::Temp* registers[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                                 &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    for (int i = 0; i < count; i++) {
      registers[i]->u64 = args[i];
    }
    lifted::CallFromContext(ctx, base, function);
    ctx.r1.u64 = stack;
    // The plain C++'s scalar float work expects flush-to-zero off (the
    // originals set it before each float instruction).
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u32;
  }
  static uint32_t Submit(void* self, uint32_t entry, uint32_t key, double depth, uint32_t frame,
                         uint32_t lr) {
    auto& d = *static_cast<SceneryDrawGuest*>(self);
    lifted::Regs& ctx = d.ctx;
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
    lifted::CallFromContext(ctx, base, function);
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

ITC_REGS_STEP(SceneryListsRecordShape) {
  if (SlotDiagnostics()) {
    lifted::original::c82146BD0(ctx, base);
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
    lifted::original::c82146BD0(ctx, base);
    return;
  }
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *first;
}

ITC_REGS_STEP(SceneryListsDrawShape) {
  if (SlotDiagnostics()) {
    lifted::original::c82147D88(ctx, base);
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
  // (the caller's r24: diagnostic only, not carried)
  const auto draw = scenery_lists::SceneryDrawShape(
      base, calls, frame, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
      ctx.r8.u32, ctx.r9.u32, ctx.r10.u32, 0);
  if (!draw) {
    guest::StoreU32(base, frame, chain);
    lifted::original::c82147D88(ctx, base);
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

ITC_REGS_STEP(SceneryListsModelRecordShape) {
  if (SlotDiagnostics()) {
    lifted::original::c82595550(ctx, base);
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
    lifted::original::c82595550(ctx, base);
    return;
  }
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *first;
}

ITC_REGS_STEP(SceneryListsModelRecordMaterials) {
  if (SlotDiagnostics()) {
    lifted::original::c820CECA0(ctx, base);
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
    lifted::original::c820CECA0(ctx, base);
    return;
  }
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *first;
}

ITC_REGS_STEP(SceneryListsModelDrawShape) {
  if (SlotDiagnostics()) {
    lifted::original::c82596B18(ctx, base);
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
  // (the caller's r24: diagnostic only, not carried)
  const auto draw = scenery_lists::ModelDrawShape(base, calls, frame, ctx.r3.u32, ctx.r4.u32,
                                                  ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                                  ctx.r8.u32, ctx.r9.u32, 0);
  if (!draw) {
    guest::StoreU32(base, frame, chain);
    lifted::original::c82596B18(ctx, base);
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

ITC_REGS_STEP(SceneryListsPointListDraw) {
  if (SlotDiagnostics()) {
    lifted::original::c82148058(ctx, base);
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
  // (the caller's r24: diagnostic only, not carried)
  const auto r3 = scenery_lists::PointListDraw(base, calls, frame, ctx.r3.u32, ctx.r4.u32,
                                               ctx.r5.u32, 0, ctx.r3.u32);
  if (!r3) {
    guest::StoreU32(base, frame, chain);
    lifted::original::c82148058(ctx, base);
    return;
  }
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *r3;
}

}  // namespace hooks

#define ITC_SCENERY_LISTS(address, steps)    \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_scenery_lists)) { \
    hooks::steps(ctx, base);                 \
  }

ITC_SCENERY_LISTS(82147FB8, SceneryListsModelListRelease)
// SceneryModel::ResetDrawState (sub_82146AA8), PassDescription::
// SetBillboardPass (sub_82148650), SceneryModel::DrawShape (sub_82147D88)
// and Model::DrawShape (sub_82596B18) have no adapters (removed 2026-11-13,
// B7 loop): their callers in play (Scenery::Draw, DrawArea, DrawSetLists,
// Minimap::Draw, PointList::Draw, Floor::Draw) call their plain C++; their
// register steps stay as the harness's.
ITC_SCENERY_LISTS(82146B80, SceneryListsAnimationCurrentFrame)
ITC_SCENERY_LISTS(8228F408, SceneryListsCourseLayout)
// PointList::IsEntryOn (sub_82147B18) has no adapter (removed 2026-11-13, B7
// loop): its callers in play, Scenery::DrawSetLists and Minimap::Draw, call
// its plain C++.
ITC_SCENERY_LISTS(825951E0, SceneryListsIsShapeVisible)
ITC_SCENERY_LISTS(82595490, SceneryListsRecordSceneLights)
ITC_SCENERY_LISTS(825960D0, SceneryListsRecordShapeDepth)
// Model::DrawShapeDepth (sub_82596D48) has no adapter (removed 2026-11-13, B7
// loop): its callers in play (the car draw's, Scenery::DrawArea) call its
// plain C++; SceneryListsDrawShapeDepth stays as the harness's steps.
ITC_SCENERY_LISTS(82146BD0, SceneryListsRecordShape)
ITC_SCENERY_LISTS(82148058, SceneryListsPointListDraw)
ITC_SCENERY_LISTS(82594328, SceneryListsTextureProviderClearAll)
ITC_SCENERY_LISTS(82594718, SceneryListsTextureProviderSet)

// Model::RecordShape's row is in docs/functions/models.md: behind that file's
// flag.
ITC_NATIVE_ENTRY(82595550, ITC_FLAG(native_models)) {
  hooks::SceneryListsModelRecordShape(ctx, base);
}

// Model::RecordMaterials' row is in docs/functions/models.md too.
ITC_NATIVE_ENTRY(820CECA0, ITC_FLAG(native_models)) {
  hooks::SceneryListsModelRecordMaterials(ctx, base);
}
