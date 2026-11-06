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
// CollectModels (sub_820F32D0) and PoolObject::SetVisible (sub_820FB938).

#include "guest.h"

#include "base/flags.h"
#include "hooks/archive.h"
#include "hooks/course_scenery.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/scenery_lists.h"
#include "native/buffers.h"
#include "course_scenery/course_scenery.h"

ITC_FLAG_BOOL(native_course_scenery, true, "ITC",
              "Run the course scenery (docs/functions/course-scenery.md: PoolObject::AllocData "
              "sub_820FB8B8, InUse sub_820FB8E0, IsLoaded sub_820FB8F0, IsVisible sub_820FB908, "
              "IsExpiring sub_820FB920, SetVisible sub_820FB938 (no adapter), SetExpiring sub_820FB968, "
              "ClearLoadFlags sub_820FB998, MarkLoaded sub_820FB9A8, Reset sub_820FB9D8, Key "
              "sub_820FB9F0, PoolObject sub_820FBB20 (no adapter), SetKey sub_820FBA00, KeyEquals "
              "sub_820FBA60, AreaLights::Key sub_820F1C20, AreaModel::Key sub_820F1CD8, "
              "ObjectPools::Pool sub_820FBB10, Object sub_820FC0E8, Acquire sub_820FBF08, Find "
              "sub_820FBE80, Load sub_820FBFD0, Release sub_820FC038, Init sub_820FBBE8, "
              "Shutdown sub_820FBDC8, SceneryConfig::ModelAsset sub_820F1530, Scenery::PoolSizes "
              "sub_820F2030 (Archive::AssetSize's plain C++), SceneryList::Clear "
              "sub_820F1588 (no adapter), Add sub_820F1888 (no adapter), Sort sub_820F1930 (no adapter), "
              "Scenery::LimitLargeModels sub_820F2B80 (no adapter), ModelList::Init sub_820F1A20 (no adapter), Scenery::ClearGlobal sub_820F1480, "
              "Scenery::AllocPhysical sub_820F1490 and FreePhysical sub_820F14E8 (with the native "
              "heap; XPhysicalSize answered by the layer), Scenery::Create sub_820F2F88 and Setup "
              "sub_820F3020 with SetupPools sub_820F2720 (no adapter: only Setup calls it; with "
              "--native_models' lifetime and memcpy native), Scenery::Init sub_820F1360, "
              "SceneryList::ReleaseUnused sub_820F17C0 (no adapter), SceneryList::LoadAll sub_820F1598 and Stream "
              "sub_820F16C0 (no adapter) (Archive::QueueLength's plain C++ with the archive's native queue), "
              "Scenery::StartLoad sub_820F1E00, "
              "Scenery::CollectAreas sub_820F3098 (no adapter), Scenery::CollectModels sub_820F32D0 (no adapter), "
              "Scenery::Update sub_820F4558 (CameraPass::FromViewport's plain C++, CameraPass::Update "
              "a guest call), "
              "Scenery::DrawSetLists sub_820F52A8 (the scenery lists' draws' plain C++ called "
              "directly, their adapters where it declines), "
              "TextureAnimations::Advance sub_820F11A8 (in the per-frame logic's wrap), "
              "AnimationSet::Advance sub_82146B20, Animation::Advance sub_82146AC0, "
              "AreaLights::Parse sub_820F11C0 (no adapter: only its OnLoaded calls it) and OnLoaded "
              "sub_820F1C30; with --native_models CourseSet::Parse sub_820F1A70, AreaModel::Parse "
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
              "sub_820F16C0, sub_820F17C0, sub_820F2B80, sub_820F3098, sub_820F32D0, sub_820FB938");

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
          }};
}

bool hooks::CourseScenerySetUpNative() {
  return hooks::ModelsNativeLifetime() && native::Buffers_NativeMemcpy();
}

// The calls the plain C++ makes into code that takes the context.
#define ITC_SCENERY_CALLS        \
  GuestCalls call{&ctx, base};   \
  const Calls calls { &call, CallGuest }

// --- Pool objects: r3 = the object ---

// r3 = the object's +4, r4 = a size.
ITC_COURSE_SCENERY(820FB8B8, ctx.r3.u64 = AllocData(base, ctx.r3.u32, ctx.r4.u32))
ITC_COURSE_SCENERY(820FB8E0, ctx.r3.u64 = InUse(base, ctx.r3.u32))
ITC_COURSE_SCENERY(820FB8F0, ctx.r3.u64 = IsLoaded(base, ctx.r3.u32))
ITC_COURSE_SCENERY(820FB968, SetExpiring(base, ctx.r3.u32, ctx.r4.u32))
ITC_COURSE_SCENERY(820FB998, ClearLoadFlags(base, ctx.r3.u32))
ITC_COURSE_SCENERY(820FB9A8, MarkLoaded(base, ctx.r3.u32))
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

// r5 = an index.
ITC_COURSE_SCENERY(820FC0E8, ctx.r3.u64 = PoolObjectAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r5 = a key.
ITC_COURSE_SCENERY(820FBF08, ITC_SCENERY_CALLS;
                   SetIndex(ctx, Acquire(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                         ctx.r5.u32)))
ITC_COURSE_SCENERY(820FBE80, ITC_SCENERY_CALLS;
                   SetIndex(ctx, Find(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                      ctx.r5.u32)))
// r5 = an index.
ITC_COURSE_SCENERY(820FBFD0, ITC_SCENERY_CALLS;
                   ctx.r3.u64 = LoadObject(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                           ctx.r5.u32))

// ObjectPools::Init: r4 = the pool count, r5 = the descriptors. The original
// runs when the native heap isn't active.
extern "C" REX_FUNC(sub_820FBBE8) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_course_scenery)) {
    ITC_SCENERY_CALLS;
    uint32_t r3 = 0;
    if (Init(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &r3)) {
      ctx.r3.u64 = r3;
      return;
    }
  }
  __imp__sub_820FBBE8(ctx, base);
}

// ObjectPools::Shutdown: r3 = the pool set. The original runs when the native
// heap isn't active.
extern "C" REX_FUNC(sub_820FBDC8) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_course_scenery)) {
    ITC_SCENERY_CALLS;
    uint32_t r3 = 0;
    if (Shutdown(base, calls, ctx.r1.u32, ctx.r3.u32, &r3)) {
      ctx.r3.u64 = r3;
      return;
    }
  }
  __imp__sub_820FBDC8(ctx, base);
}

// --- The scenery's lists ---

// r3 = the configuration, r4 = an area, r5 = a model.
ITC_COURSE_SCENERY(820F1530, SetIndex(ctx, ModelAsset(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)))
// Scenery::PoolSizes: r3 = the area counts, r4 = the model counts, r5 = out,
// r6 = the configuration; from Scenery::SetupPools (recompiled).
ITC_COURSE_SCENERY(820F2030, ctx.r3.u64 = PoolSizes(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r6.u32))
ITC_COURSE_SCENERY(820F1480, ClearGlobal(base))
// Scenery::AllocPhysical: r3 = a size; returns the block. Scenery::
// FreePhysical: r3 = a block; r3 = 0, as XPhysicalFree leaves it. The
// originals run when the native heap isn't active (the layer answers the
// blocks' sizes only for blocks it gave).
extern "C" REX_FUNC(sub_820F1490) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_course_scenery) || !hooks::NativeHeapActive()) {
    __imp__sub_820F1490(ctx, base);
    return;
  }
  ctx.r3.u64 = AllocPhysical(base, ctx.r3.u32);
}
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
extern "C" REX_FUNC(sub_820F14E8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_course_scenery) || !hooks::NativeHeapActive()) {
    __imp__sub_820F14E8(ctx, base);
    return;
  }
  FreePhysical(base, ctx.r3.u32);
  ctx.r3.u64 = 0;
}
// Scenery::Init: r3 = the manager; r3 comes back as the original's last
// call (XMemSet) leaves it: its destination, [0x826C5514] + 144.
ITC_COURSE_SCENERY(820F1360, Init(base, ctx.r3.u32);
                   ctx.r3.u64 = guest::LoadU32(base, kManager) + 144u)
// SceneryList::LoadAll: r3 = the manager, r4 = a list; from Scenery::
// IsLoaded (recompiled). The queue length's plain C++ with the archive's
// native queue, else its original through `ctx`.
ITC_COURSE_SCENERY(820F1598, ITC_SCENERY_CALLS;
                   ctx.r3.u64 = ListLoadAll(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                            hooks::NativeArchiveQueue()))
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
                    }};
  ctx.r3.u64 = Update(base, calls, ctx.r1.u32, ctx.r3.u32,
                      {ctx.r25.u64, ctx.r26.u64, ctx.r3.u64}, hooks::NativeArchiveQueue());
  ctx.lr = lr;
}
ITC_COURSE_SCENERY(820F4558, hooks::CourseSceneryUpdate(ctx, base))

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

ITC_COURSE_SCENERY(820F52A8, hooks::CourseSceneryDrawSetLists(ctx, base))

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

// CourseSet::Parse: r3 = the course set; r3 as the original leaves it.
extern "C" REX_FUNC(sub_820F1A70) {
  REX_FUNC_PROLOGUE();
  if (!NativeParses()) {
    __imp__sub_820F1A70(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = CourseSetParse(base, calls, ctx.r1.u32, ctx.r3.u32);
}

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
