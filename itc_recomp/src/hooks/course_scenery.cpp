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
// SceneryList::ReleaseUnused.

#include "guest.h"

#include "base/flags.h"
#include "hooks/models.h"
#include "course_scenery/course_scenery.h"

ITC_FLAG_BOOL(native_course_scenery, true, "ITC",
              "Run the course scenery (docs/functions/course-scenery.md: PoolObject::AllocData "
              "sub_820FB8B8, InUse sub_820FB8E0, IsLoaded sub_820FB8F0, IsVisible sub_820FB908, "
              "IsExpiring sub_820FB920, SetVisible sub_820FB938, SetExpiring sub_820FB968, "
              "ClearLoadFlags sub_820FB998, MarkLoaded sub_820FB9A8, Reset sub_820FB9D8, Key "
              "sub_820FB9F0, PoolObject sub_820FBB20, SetKey sub_820FBA00, KeyEquals "
              "sub_820FBA60, AreaLights::Key sub_820F1C20, AreaModel::Key sub_820F1CD8, "
              "ObjectPools::Pool sub_820FBB10, Object sub_820FC0E8, Acquire sub_820FBF08, Find "
              "sub_820FBE80, Load sub_820FBFD0, Release sub_820FC038, Init sub_820FBBE8, "
              "Shutdown sub_820FBDC8, SceneryConfig::ModelAsset sub_820F1530, SceneryList::Clear "
              "sub_820F1588, Add sub_820F1888, Sort sub_820F1930, Scenery::LimitLargeModels "
              "sub_820F2B80, ModelList::Init sub_820F1A20, Scenery::ClearGlobal sub_820F1480, Scenery::Init sub_820F1360, "
              "SceneryList::ReleaseUnused sub_820F17C0, Scenery::StartLoad sub_820F1E00, "
              "TextureAnimations::Advance sub_820F11A8 (in the per-frame logic's wrap), "
              "AnimationSet::Advance sub_82146B20, Animation::Advance sub_82146AC0, "
              "AreaLights::Parse sub_820F11C0 and OnLoaded sub_820F1C30; with --native_models "
              "CourseSet::Parse sub_820F1A70, AreaModel::Parse sub_820F1258 and OnLoaded "
              "sub_820F1CE8, the model class's and the model lists' plain C++ called directly, "
              "their originals where it declines guest calls) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=course_scenery). Those reached only from native callers have no "
              "adapters, and the flag switches their callers: sub_82146B20, sub_82146AC0, "
              "sub_820FBA00, sub_820FBA60, sub_820FB908, sub_820FB920, sub_820FBB10, "
              "sub_820FC038");

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

// The calls the plain C++ makes into code that takes the context.
#define ITC_SCENERY_CALLS        \
  GuestCalls call{&ctx, base};   \
  const Calls calls { &call, CallGuest }

// --- Pool objects: r3 = the object ---

// r3 = the object's +4, r4 = a size.
ITC_COURSE_SCENERY(820FB8B8, ctx.r3.u64 = AllocData(base, ctx.r3.u32, ctx.r4.u32))
ITC_COURSE_SCENERY(820FB8E0, ctx.r3.u64 = InUse(base, ctx.r3.u32))
ITC_COURSE_SCENERY(820FB8F0, ctx.r3.u64 = IsLoaded(base, ctx.r3.u32))
ITC_COURSE_SCENERY(820FB938, SetVisible(base, ctx.r3.u32, ctx.r4.u32))
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
// r4 = its own size, r5 = the data size, r6 = its index.
ITC_COURSE_SCENERY(820FBB20, ConstructObject(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                             ctx.r6.u32))

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
// r3 = the manager (not read), r4 = a list.
ITC_COURSE_SCENERY(820F1588, ListClear(base, ctx.r4.u32))
// f1 = a distance, r6 = an area, r7 = a model, r8 = a mask.
ITC_COURSE_SCENERY(820F1888, ctx.r3.u64 = ListAdd(base, ctx.r4.u32, ctx.f1.f64, ctx.r6.u32,
                                                  ctx.r7.u32, ctx.r8.u32, ctx.r3.u32))
// r5, r6 = the first and last item.
ITC_COURSE_SCENERY(820F1930, ListSort(base, ctx.r4.u32, ctx.r5.s32, ctx.r6.s32))
// r3 = the manager; leaves r3 = the manager + 100. The original's register
// save (r27 to r31 and the link register below r1) is made first, as its
// table's reads past the 5th area read those.
ITC_COURSE_SCENERY(820F2B80, ctx.r12.u64 = ctx.lr; __savegprlr_27(ctx, base);
                   LimitLargeModels(base, ctx.r3.u32, ctx.r1.u32);
                   ctx.r3.u64 = ctx.r3.u64 + 100)
ITC_COURSE_SCENERY(820F1A20, ModelListInit(base, ctx.r3.u32))
ITC_COURSE_SCENERY(820F1480, ClearGlobal(base))
// Scenery::Init: r3 = the manager; r3 comes back as the original's last
// call (XMemSet) leaves it: its destination, [0x826C5514] + 144.
ITC_COURSE_SCENERY(820F1360, Init(base, ctx.r3.u32);
                   ctx.r3.u64 = guest::LoadU32(base, kManager) + 144u)
// r3 = the manager, r4 = a list.
ITC_COURSE_SCENERY(820F17C0, ITC_SCENERY_CALLS;
                   ctx.r3.u64 = ReleaseUnused(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32))
// r3 = the manager.
ITC_COURSE_SCENERY(820F1E00, ITC_SCENERY_CALLS;
                   ctx.r3.u64 = StartLoad(base, calls, ctx.r1.u32, ctx.r3.u32))

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

// AreaLights::Parse: r3 = the area's lights; r3 the entry (or 0).
ITC_COURSE_SCENERY(820F11C0, ctx.r3.u64 = AreaLightsParse(base, ctx.r3.u32))

// AreaLights::OnLoaded (Archive::Load's callback): r6 = the area's lights;
// r3 = them, as PoolObject::MarkLoaded leaves it.
ITC_COURSE_SCENERY(820F1C30, guest::StoreU32(base, ctx.r1.u32 - 96, ctx.r1.u32);
                   ctx.r3.u64 = AreaLightsOnLoaded(base, ctx.r6.u32))

// AreaModel::Parse: r3 = the area's model; r3 as the original leaves it.
extern "C" REX_FUNC(sub_820F1258) {
  REX_FUNC_PROLOGUE();
  if (!NativeParses()) {
    __imp__sub_820F1258(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ITC_SCENERY_CALLS;
  ctx.r3.u64 = AreaModelParse(base, calls, ctx.r1.u32, ctx.r3.u32);
}

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
