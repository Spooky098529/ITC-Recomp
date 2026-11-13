// The course scenery natively: adapters in front of src/course_scenery/
// (docs/functions/course-scenery.md). Each leaves r3 as the original does.
// Functions reached only from native callers have no adapters (the flag
// switches their callers): AnimationSet::Advance (sub_82146B20) and
// Animation::Advance (sub_82146AC0), from TextureAnimations::Advance
// (sub_820F11A8), whose override is the per-frame logic's wrap
// (src/frame_pacing/per_frame_logic.cpp: skipped on render-only passes);
// PoolObject::SetKey (sub_820FBA00), from ObjectPools::Acquire;
// PoolObject::KeyEquals (sub_820FBA60), from ObjectPools::Find; and
// PoolObject::IsVisible (sub_820FB908), IsExpiring (sub_820FB920),
// ObjectPools::Pool (sub_820FBB10) and Release (sub_820FC038), from
// SceneryList::ReleaseUnused; and, since every caller seen in play became
// native, SceneryList::Add (sub_820F1888, from Scenery::StartLoad,
// CollectAreas and CollectModels), PoolObject::PoolObject (sub_820FBB20,
// from the pools' objects' constructors) and ModelList::Init (sub_820F1A20,
// from CourseSet::CourseSet and InitParts); and, since Scenery::Update
// became native, SceneryList::Clear (sub_820F1588), Sort (sub_820F1930),
// Stream (sub_820F16C0), ReleaseUnused (sub_820F17C0), Scenery::
// LimitLargeModels (sub_820F2B80), CollectAreas (sub_820F3098),
// CollectModels (sub_820F32D0) and PoolObject::SetVisible (sub_820FB938);
// and, since Scenery::IsLoaded, Update and SetupPools are native (B7 loop,
// 2026-11-12), PoolObject::InUse (sub_820FB8E0), IsLoaded (sub_820FB8F0) and
// SetExpiring (sub_820FB968), ObjectPools::Object (sub_820FC0E8), Acquire
// (sub_820FBF08), Find (sub_820FBE80), Load (sub_820FBFD0) and Init
// (sub_820FBBE8), SceneryConfig::ModelAsset (sub_820F1530), Scenery::
// PoolSizes (sub_820F2030) and Init (sub_820F1360) and SceneryList::LoadAll
// (sub_820F1598); and, since Scenery::Unload and Destroy are native (B7
// loop, 2026-11-12), ObjectPools::Shutdown (sub_820FBDC8) and Scenery::
// ClearGlobal (sub_820F1480); and, since the pools' loads and CourseSet::
// OnLoaded are native (B7 loop, 2026-11-12), PoolObject::AllocData
// (sub_820FB8B8), ClearLoadFlags (sub_820FB998) and MarkLoaded
// (sub_820FB9A8), Scenery::AllocPhysical (sub_820F1490) and FreePhysical
// (sub_820F14E8) and CourseSet::Parse (sub_820F1A70).

#include "guest.h"

#include "base/flags.h"
#include "hooks/archive.h"
#include "hooks/billboards.h"
#include "hooks/cameras.h"
#include "hooks/car_scene_draw.h"
#include "hooks/course_scenery.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/scenery_lists.h"
#include "native/buffers.h"
#include "course_scenery/course_scenery.h"

ITC_FLAG_DECLARE(bool, native_cameras);
ITC_FLAG_DECLARE(bool, native_data_files);
ITC_FLAG_DECLARE(bool, native_post_effects);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_model_animation);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);

ITC_FLAG_BOOL(native_course_scenery, true, "ITC",
              "Run the course scenery (docs/functions/course-scenery.md: PoolObject::AllocData "
              "sub_820FB8B8 (no adapter), InUse sub_820FB8E0 (no adapter), IsLoaded sub_820FB8F0 (no adapter), IsVisible sub_820FB908, "
              "IsExpiring sub_820FB920, SetVisible sub_820FB938 (no adapter), SetExpiring sub_820FB968 (no adapter), "
              "ClearLoadFlags sub_820FB998 (no adapter), MarkLoaded sub_820FB9A8 (no adapter), Reset sub_820FB9D8, Key "
              "sub_820FB9F0, PoolObject sub_820FBB20 (no adapter), SetKey sub_820FBA00, KeyEquals "
              "sub_820FBA60, AreaLights::Key sub_820F1C20, AreaModel::Key sub_820F1CD8, "
              "ObjectPools::Pool sub_820FBB10, Object sub_820FC0E8 (no adapter), Acquire sub_820FBF08 (no adapter), Find "
              "sub_820FBE80 (no adapter), Load sub_820FBFD0 (no adapter), Release sub_820FC038, Init sub_820FBBE8 (no adapter), "
              "Shutdown sub_820FBDC8 (no adapter), SceneryConfig::ModelAsset sub_820F1530 (no adapter), Scenery::PoolSizes "
              "sub_820F2030 (no adapter) (Archive::AssetSize's plain C++), SceneryList::Clear "
              "sub_820F1588 (no adapter), Add sub_820F1888 (no adapter), Sort sub_820F1930 (no adapter), "
              "Scenery::LimitLargeModels sub_820F2B80 (no adapter), ModelList::Init sub_820F1A20 (no adapter), Scenery::ClearGlobal sub_820F1480 (no adapter), "
              "Scenery::AllocPhysical sub_820F1490 and FreePhysical sub_820F14E8 (no adapters; with the native "
              "heap; XPhysicalSize answered by the layer), Scenery::Create sub_820F2F88 and Setup "
              "sub_820F3020 with SetupPools sub_820F2720 (no adapter: only Setup calls it; with "
              "--native_models' lifetime and memcpy native), Scenery::Init sub_820F1360 (no adapter), "
              "SceneryList::ReleaseUnused sub_820F17C0 (no adapter), SceneryList::LoadAll sub_820F1598 (no adapter) and Stream "
              "sub_820F16C0 (no adapter) (Archive::QueueLength's plain C++ with the archive's native queue), "
              "Scenery::StartLoad sub_820F1E00, Scenery::IsLoaded sub_820F1F68 (no adapter), "
              "CourseSet::Load sub_820F2520, AreaLights::Load sub_820F22F8 and AreaModel::Load "
              "sub_820F23C0 (with the archive's native queue and the native heap; called "
              "directly by ObjectPools::Load), CourseSet::OnLoaded sub_820F22B8 (as the "
              "parses, with the native heap), "
              "Scenery::Unload sub_820F2B38 with UnloadPools sub_820F1D20 (no adapter) and Destroy sub_820F2AF8 "
              "(with the native heap), "
              "Scenery::CollectAreas sub_820F3098 (no adapter), Scenery::CollectModels sub_820F32D0 (no adapter), "
              "Scenery::Update sub_820F4558 (CameraPass::FromViewport's plain C++, CameraPass::Update "
              "a guest call), "
              "Scenery::DrawSetLists sub_820F52A8 (no adapter; the scenery lists' draws' plain C++ called "
              "directly, their adapters where it declines), "
              "Scenery::RecordHeadlightProjectors sub_820F34C8 (no adapter; the headlight slots and "
              "projectors' chunks, its callees' plain C++ called directly inside their wraps' "
              "checks, the render manager's getters and queue guest calls; with --native_cameras, "
              "--native_matrix_math and --native_vehicle_accessors and the command list's "
              "diagnostics off, else its original), "
              "Scenery::DrawArea sub_820F3B70 (no adapter; the headlights', the scenery models' draws', the "
              "light groups' and their callees' plain C++; with those flags, --native_billboards, "
              "--native_post_effects and --native_data_files, else its original), "
              "Scenery::Draw sub_820F6068 (in its frame-state wrap; its callees' plain C++ "
              "called directly, TextureAnimations::Advance and Minimap::Draw through their "
              "per-frame wraps, RecordClear through its adapter's work; as DrawArea's flags), "
              "TextureAnimations::Advance sub_820F11A8 (in the per-frame logic's wrap), "
              "AnimationSet::Advance sub_82146B20, Animation::Advance sub_82146AC0, AnimationSet::FindByTexture sub_82146A38, "
              "AreaLights::Parse sub_820F11C0 (no adapter: only its OnLoaded calls it) and OnLoaded "
              "sub_820F1C30; with --native_models CourseSet::Parse sub_820F1A70 (no adapter), AreaModel::Parse "
              "sub_820F1258 (no adapter: only its OnLoaded calls it) and OnLoaded sub_820F1CE8, "
              "Scenery::MakePoolObject sub_820F2638 (with CourseSet::CourseSet sub_820F2478, "
              "CourseSet::InitParts sub_820F2230 and AreaModel::AreaModel sub_820F1C68, no "
              "adapters: only it calls them), CourseSet::Reset sub_820F1138, AreaModel::Reset "
              "sub_820F12F8 and AreaModel::~Model sub_820F1208, the model class's and the model lists' plain C++ called directly, "
              "their originals where it declines guest calls) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=course_scenery). Those reached only from native callers have no "
              "adapters, and the flag switches their callers: sub_82146B20, sub_82146AC0, "
              "sub_820FBA00, sub_820FBA60, sub_820FB908, sub_820FB920, sub_820FBB10, "
              "sub_820FC038, sub_820F1888, sub_820FBB20, sub_820F1A20, sub_820F1588, sub_820F1930, "
              "sub_820F16C0, sub_820F17C0, sub_820F2B80, sub_820F3098, sub_820F32D0, sub_820FB938, "
              "sub_820FB8E0, sub_820FB8F0, sub_820FB968, sub_820FC0E8, sub_820FBF08, sub_820FBE80, "
              "sub_820FBFD0, sub_820FBBE8, sub_820F1530, sub_820F2030, sub_820F1360, sub_820F1598, sub_820F1D20, sub_820FBDC8, sub_820F1480, sub_820FB8B8, sub_820FB998, sub_820FB9A8, sub_820F1490, sub_820F14E8, sub_820F1A70, sub_820F52A8, sub_820F34C8, sub_820F3B70");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_COURSE_SCENERY(address, ...)     \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_course_scenery)) {  \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    __VA_ARGS__;                             \
  }

using namespace course_scenery;

namespace {

struct GuestCalls {
  PPCContext* ctx;
  uint8_t* base;
};

uint32_t CallGuest(void* user, uint32_t function, const uint32_t* args, int count,
                   uint32_t frame, uint32_t lr) {
  auto& call = *static_cast<GuestCalls*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  const uint64_t saved = ctx.r1.u64;
  ctx.r1.u64 = frame;
  ctx.lr = lr;
  PPCRegister* registers[5] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7};
  for (int i = 0; i < count; i++) {
    registers[i]->u64 = args[i];
  }
  ctx.ctr.u64 = function;
  REX_CALL_INDIRECT_FUNC(function);
  ctx.r1.u64 = saved;
  return ctx.r3.u32;
}

// Archive::Load for the pools' loads' plain C++ (Calls::load), or null
// where they may not run (the flag off, the archive's queue or the heap not
// native).
const archive::LoadCall* SceneryLoads(uint8_t* base) {
  if (!ITC_FLAG(native_course_scenery) || !hooks::NativeHeapActive() ||
      !hooks::NativeArchiveQueue()) {
    return nullptr;
  }
  static const archive::LoadCall load = hooks::NativeArchiveLoad(base);
  return &load;
}

// -1 as the originals' li r3,-1 leaves it (64 bits).
void SetIndex(PPCContext& ctx, uint32_t index) {
  if (index == 0xFFFFFFFFu) {
    ctx.r3.s64 = -1;
  } else {
    ctx.r3.u64 = index;
  }
}

}  // namespace

course_scenery::Calls hooks::CourseSceneryCalls(CourseSceneryGuest* guest) {
  static_assert(sizeof(CourseSceneryGuest) == sizeof(GuestCalls));
  return {guest, [](void* user, uint32_t function, const uint32_t* args, int count,
                    uint32_t frame, uint32_t lr) {
            auto* g = static_cast<CourseSceneryGuest*>(user);
            GuestCalls call{g->ctx, g->base};
            return CallGuest(&call, function, args, count, frame, lr);
          },
          SceneryLoads(guest->base)};
}

bool hooks::CourseScenerySetUpNative() {
  return hooks::ModelsNativeLifetime() && native::Buffers_NativeMemcpy();
}

// The calls the plain C++ makes into code that takes the context.
#define ITC_SCENERY_CALLS        \
  GuestCalls call{&ctx, base};   \
  const Calls calls { &call, CallGuest, SceneryLoads(base) }

// --- Pool objects: r3 = the object ---

// The vtables' slots stay reached through them: the reset (slot 2 of the
// base and pool 3's) and the keys (slot 0), r4 = &the size.
ITC_COURSE_SCENERY(820FB9D8, ResetObject(base, ctx.r3.u32))
ITC_COURSE_SCENERY(820FB9F0, uint32_t size = 0;
                   ctx.r3.u64 = PoolObjectKey(ctx.r3.u32, &size);
                   guest::StoreU32(base, ctx.r4.u32, size))
ITC_COURSE_SCENERY(820F1C20, uint32_t size = 0;
                   ctx.r3.u64 = AreaLightsKey(ctx.r3.u32, &size);
                   guest::StoreU32(base, ctx.r4.u32, size))
ITC_COURSE_SCENERY(820F1CD8, uint32_t size = 0;
                   ctx.r3.u64 = AreaModelKey(ctx.r3.u32, &size);
                   guest::StoreU32(base, ctx.r4.u32, size))

// --- The pools: r3 = the pool set, r4 = a pool ---

// --- The scenery's lists ---

// Scenery::Create: no arguments; r3 as Init leaves it. Scenery::Setup: r3 =
// the manager, r4 = the configuration; r3 = the visible areas' block. The
// originals run where their plain C++ may not (CourseScenerySetUpNative).
extern "C" REX_FUNC(sub_820F2F88) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_course_scenery) || !hooks::CourseScenerySetUpNative()) {
    __imp__sub_820F2F88(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = Create(base);
}
extern "C" REX_FUNC(sub_820F3020) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_course_scenery) || !hooks::CourseScenerySetUpNative()) {
    __imp__sub_820F3020(ctx, base);
    return;
  }
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = Setup(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}
// Scenery::Unload: r3 = the manager; r3 as the original leaves it (the
// manager + 116). From the race scene's sub_8215AA88 and the front end's
// sub_82229678 (recompiled). UnloadPools (sub_820F1D20) has no adapter:
// only Unload calls it. The original runs when the native heap isn't
// active.
extern "C" REX_FUNC(sub_820F2B38) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_course_scenery)) {
    ITC_SCENERY_CALLS;
    uint32_t r3 = 0;
    if (Unload(base, calls, ctx.r1.u32, ctx.r3.u32, &r3)) {
      ctx.r3.u64 = r3;
      return;
    }
  }
  __imp__sub_820F2B38(ctx, base);
}
// Scenery::Destroy: no arguments; r3 = 0. From sub_8215AA88 and the front
// end's sub_82228548 (recompiled).
extern "C" REX_FUNC(sub_820F2AF8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_course_scenery) || !hooks::NativeHeapActive()) {
    __imp__sub_820F2AF8(ctx, base);
    return;
  }
  ctx.r3.u64 = Destroy(base);
}
// Scenery::IsLoaded (sub_820F1F68) has no adapter (B7 loop, 2026-11-12):
// its callers in play, the race's loading's handlers (CourseSetup::
// WaitCourse and EndTick, --native_race_loading) and the location screen's
// PollLoad (sub_82229238, --native_location_screen), call its plain C++.
// r3 = the manager.
ITC_COURSE_SCENERY(820F1E00, ITC_SCENERY_CALLS;
                   ctx.r3.u64 = StartLoad(base, calls, ctx.r1.u32, ctx.r3.u32))

// --- What a view sees ---

// Scenery::Update: r3 = the manager; from RaceScene::Update and the front
// end's camera (recompiled). The original's register saves made first
// (__savegprlr_18, f31 at r1 - 128: its LimitLargeModels' reads past its
// table can reach them), then the plain C++; its guest calls (CameraPass::
// Update) put flush-to-zero back off after, as the originals' next float
// instruction does. r3 as the original leaves it; lr put back.
void hooks::CourseSceneryUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  ctx.r12.u64 = ctx.lr;
  __savegprlr_18(ctx, base);
  guest::StoreU32(base, ctx.r1.u32 - 128, uint32_t(ctx.f31.u64 >> 32));
  guest::StoreU32(base, ctx.r1.u32 - 124, uint32_t(ctx.f31.u64));
  GuestCalls call{&ctx, base};
  const Calls calls{&call, [](void* user, uint32_t function, const uint32_t* args, int count,
                              uint32_t frame, uint32_t back) {
                      const uint32_t r3 = CallGuest(user, function, args, count, frame, back);
                      static_cast<GuestCalls*>(user)->ctx->fpscr.disableFlushMode();
                      return r3;
                    },
                    SceneryLoads(base)};
  ctx.r3.u64 = Update(base, calls, ctx.r1.u32, ctx.r3.u32,
                      {ctx.r25.u64, ctx.r26.u64, ctx.r3.u64}, hooks::NativeArchiveQueue());
  ctx.lr = lr;
}
ITC_COURSE_SCENERY(820F4558, hooks::CourseSceneryUpdate(ctx, base))

// AnimationSet::FindByTexture: r3 = the set, r4 = a texture; r3 = the index
// or -1. From the light groups' draw sub_82148658 (recompiled).
ITC_COURSE_SCENERY(82146A38, ctx.r3.u64 = FindAnimationByTexture(base, ctx.r3.u32, ctx.r4.u32))

// --- The course set's draws ---

void hooks::CourseSceneryDrawSetLists(PPCContext& ctx, uint8_t* base) {
  if (hooks::SceneryListsSlotDiagnostics()) {
    __imp__sub_820F52A8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint64_t r24 = ctx.r24.u64;
  ctx.r24.u64 = DrawSetListsViewProjection(base, ctx.r4.u32);
  hooks::SceneryDrawGuest guest{ctx, base};
  const scenery_lists::DrawCalls calls = hooks::SceneryListsDrawCalls(&guest);
  const uint32_t r3 = DrawSetLists(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
  ctx.r24.u64 = r24;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

// Scenery::DrawSetLists (sub_820F52A8) has no adapter (removed 2026-11-13, B7
// loop): its only caller in play, Scenery::Draw, calls its plain C++;
// CourseSceneryDrawSetLists stays as the harness's steps.

// Scenery::RecordHeadlightProjectors: r4 = the pass description; r3 as the
// original leaves it, lr put back, flush-to-zero off (the harness's steps:
// its callers in play, Scenery::DrawArea and Scenery::Draw, call its plain
// C++, and its adapter is removed, 2026-11-13). The original
// runs where its callees' plain C++ may not (their flags) or with the
// command list's diagnostics on (as the car scene's draw).
void hooks::CourseSceneryRecordHeadlightProjectors(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_cameras) || !ITC_FLAG(native_matrix_math) ||
      !ITC_FLAG(native_vehicle_accessors) || hooks::SceneryListsSlotDiagnostics()) {
    __imp__sub_820F34C8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx, base);
  const uint64_t r3 = RecordHeadlightProjectors(base, calls, ctx.r1.u32, ctx.r4.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}


// Scenery::DrawArea: r3 = the manager, r4 = the pass description, r5 = the
// area; lr put back, flush-to-zero off (r3 isn't read by its caller,
// Scenery::Draw). The original runs where the headlights' or the light
// groups' plain C++ may not, or without --native_post_effects,
// --native_data_files or --native_scenery_lists. ctx's r24 is the camera
// pass while it runs (as the original's), for SceneryModel::DrawShape's
// adapter where its plain C++ declines.
bool hooks::CourseSceneryDrawAreaNative() {
  return ITC_FLAG(native_course_scenery) && ITC_FLAG(native_cameras) &&
         ITC_FLAG(native_matrix_math) && ITC_FLAG(native_vehicle_accessors) &&
         ITC_FLAG(native_post_effects) && ITC_FLAG(native_data_files) &&
         hooks::LightGroupsNative() && !hooks::SceneryListsSlotDiagnostics();
}

void hooks::CourseSceneryDrawArea(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint64_t r24 = ctx.r24.u64;
  ctx.r24.u64 = guest::LoadU32(base, ctx.r4.u32 + 20) * 592u + 0x826BAC00u;
  hooks::SceneryDrawGuest guest{ctx, base};
  const scenery_lists::DrawCalls draws = hooks::SceneryListsDrawCalls(&guest);
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx, base);
  DrawArea(base, draws, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r28.u32);
  ctx.r24.u64 = r24;
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
}

// Scenery::DrawArea (sub_820F3B70) has no adapter (removed 2026-11-13, B7
// loop): its only caller in play, Scenery::Draw, calls its plain C++;
// CourseSceneryDrawArea stays as the harness's steps.

// Scenery::Draw: r3 = the manager. Its adapter is its frame-state wrap
// (src/debug/frame_state_diff.cpp), which calls this where
// CourseSceneryDrawNative holds. ctx's r24 is the original's (0x8265ECA0)
// while it runs, the camera pass for DrawArea and the view-projection for
// DrawSetLists (their callees' adapters read it where their plain C++
// declines), then put back.
bool hooks::CourseSceneryDrawNative() {
  return hooks::CourseSceneryDrawAreaNative();
}

void hooks::CourseSceneryDraw(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint64_t r24 = ctx.r24.u64;
  ctx.r24.u64 = 0x8265ECA0;
  hooks::SceneryDrawGuest guest{ctx, base};
  const scenery_lists::DrawCalls draws = hooks::SceneryListsDrawCalls(&guest);
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx, base);
  // A call into our own code from `frame` with the original's return
  // address, r1 put back.
  auto in_frame = [&](uint32_t frame, uint32_t back, auto&& run) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.lr = back;
    run();
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  DrawSteps steps;
  steps.native_model_animation = ITC_FLAG(native_model_animation);
  steps.set_r24 = [&](uint32_t value) { ctx.r24.u64 = value; };
  steps.advance = [&](uint32_t frame, uint32_t set) {
    in_frame(frame, 0x820F60B8, [&] {
      ctx.r3.u64 = set;
      sub_820F11A8(ctx, base);  // the per-frame logic's wrap
    });
  };
  steps.record_clear = [&](uint32_t frame, uint32_t key, uint32_t flags, uint32_t color,
                           uint32_t depth) {
    in_frame(frame, 0x820F6840, [&] {
      ctx.r3.u64 = 19;
      ctx.r4.u64 = key;
      ctx.r5.u64 = flags;
      ctx.r6.u64 = color;
      ctx.r7.u64 = depth;
      hooks::RecordClear(ctx, base);
    });
  };
  steps.minimap = [&](uint32_t frame, uint32_t manager, uint32_t description) {
    in_frame(frame, 0x820F6888, [&] {
      ctx.r3.u64 = manager;
      ctx.r4.u64 = description;
      sub_820F5740(ctx, base);  // the per-frame logic's wrap
    });
  };
  Draw(base, draws, calls, steps, ctx.r1.u32, ctx.r3.u32);
  ctx.r24.u64 = r24;
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
}

// --- The files' parses ---
//
// CourseSet::Parse and AreaModel::Parse (with its callback) call the model
// class's plain C++, so they run natively only where Model::Load's adapter
// does. Flush-to-zero off, as Model::Load leaves it.

namespace {

bool NativeParses() {
  return ITC_FLAG(native_course_scenery) && hooks::ModelsNativeLifetime();
}

}  // namespace

// AreaLights::OnLoaded (Archive::Load's callback): r6 = the area's lights;
// r3 = them, as PoolObject::MarkLoaded leaves it.
ITC_COURSE_SCENERY(820F1C30, guest::StoreU32(base, ctx.r1.u32 - 96, ctx.r1.u32);
                   ctx.r3.u64 = AreaLightsOnLoaded(base, ctx.r6.u32))

// AreaModel::OnLoaded (Archive::Load's callback): r6 = the area's model; r3
// = it, as PoolObject::MarkLoaded leaves it.
extern "C" REX_FUNC(sub_820F1CE8) {
  REX_FUNC_PROLOGUE();
  if (!NativeParses()) {
    __imp__sub_820F1CE8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = AreaModelOnLoaded(base, calls, ctx.r1.u32, ctx.r6.u32);
}

// --- The pools' loads (slot 1; no direct callers: ObjectPools::Load calls
// their plain C++ directly) and the course set's callback ---

// CourseSet::Load: r3 = the course set; r3 as the original leaves it.
extern "C" REX_FUNC(sub_820F2520) {
  REX_FUNC_PROLOGUE();
  const archive::LoadCall* load = SceneryLoads(base);
  if (load == nullptr) {
    __imp__sub_820F2520(ctx, base);
    return;
  }
  ctx.r3.u64 = CourseSetLoad(base, ctx.r3.u32, *load);
}
// AreaLights::Load and AreaModel::Load: r3 = the object; r3 as the
// originals leave it.
extern "C" REX_FUNC(sub_820F22F8) {
  REX_FUNC_PROLOGUE();
  const archive::LoadCall* load = SceneryLoads(base);
  if (load == nullptr) {
    __imp__sub_820F22F8(ctx, base);
    return;
  }
  ctx.r3.u64 = AreaLightsLoad(base, ctx.r3.u32, *load);
}
extern "C" REX_FUNC(sub_820F23C0) {
  REX_FUNC_PROLOGUE();
  const archive::LoadCall* load = SceneryLoads(base);
  if (load == nullptr) {
    __imp__sub_820F23C0(ctx, base);
    return;
  }
  ctx.r3.u64 = AreaModelLoad(base, ctx.r3.u32, *load);
}

// CourseSet::OnLoaded (Archive::Load's callback): r6 = the course set; r3 =
// it. As the parses (Model::Load's native lifetime), with the native heap.
extern "C" REX_FUNC(sub_820F22B8) {
  REX_FUNC_PROLOGUE();
  if (!NativeParses() || !hooks::NativeHeapActive()) {
    __imp__sub_820F22B8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = CourseSetOnLoaded(base, calls, ctx.r1.u32, ctx.r6.u32);
}

// --- The pools' objects: made and reset (as the parses' condition) ---
// CourseSet::CourseSet (sub_820F2478), CourseSet::InitParts (sub_820F2230)
// and AreaModel::AreaModel (sub_820F1C68) have no adapters: only
// MakePoolObject calls them. The others are reached through the pools'
// descriptors and vtables, so their adapters stay.

// Scenery::MakePoolObject: r3 = the pool, r4 = the address, r5 and r6 the
// sizes, r7 the index; r3 = the address.
extern "C" REX_FUNC(sub_820F2638) {
  REX_FUNC_PROLOGUE();
  if (!NativeParses()) {
    __imp__sub_820F2638(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = MakePoolObject(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                              ctx.r6.u32, ctx.r7.u32);
}

// CourseSet::Reset and AreaModel::Reset (slot 2): r3 = the object, left.
extern "C" REX_FUNC(sub_820F1138) {
  REX_FUNC_PROLOGUE();
  if (!NativeParses()) {
    __imp__sub_820F1138(ctx, base);
    return;
  }
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = CourseSetReset(base, calls, ctx.r1.u32, ctx.r3.u32);
}
extern "C" REX_FUNC(sub_820F12F8) {
  REX_FUNC_PROLOGUE();
  if (!NativeParses()) {
    __imp__sub_820F12F8(ctx, base);
    return;
  }
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = AreaModelReset(base, calls, ctx.r1.u32, ctx.r3.u32);
}

// AreaModel::~Model (slot 0 of the scenery models' vtable): r3 = the model,
// r4 = the flags; r3 = the model.
extern "C" REX_FUNC(sub_820F1208) {
  REX_FUNC_PROLOGUE();
  if (!NativeParses()) {
    __imp__sub_820F1208(ctx, base);
    return;
  }
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = SceneryModelDestruct(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}
