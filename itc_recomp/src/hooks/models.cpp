// The model class natively: adapters in front of src/models/
// (docs/functions/models.md). The functions that allocate or free run
// natively only while --native_heap routes the game's heap to the allocation
// layer, and hand the paths left to the original back to it from the start,
// nothing having been done. Functions reached only from native callers have
// no adapters (the flag switches their callers): PendingFrees::IncSize
// (sub_8258E2F8), from PendingFrees::Insert; PendingFrees::Insert
// (sub_8258E398), from Memory::FreeLater and FreePhysicalLater;
// Memory::FreeLater and FreePhysicalLater (sub_8258E410, sub_8258E498), from
// XPR::Release and Model::Release;
// TextureProvider::Allocate and UninitializedCopy (sub_825942B8,
// sub_82594380), from Reserve and InsertN; TextureProvider::Assign
// (sub_825947D0), from TextureProvider::TextureProvider; Memory::AllocPhysical
// (sub_8258E138), from XPR::Create; TextureHeader::Relocate (sub_8235A990),
// from Resource::Relocate and XPR::Create; Mesh::LoadShaders (sub_82594D20)
// and Mesh::Parse (sub_825963F0), from Mesh::Parse and Model::Load.

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/model_load.h"
#include "models/part_model.h"
#include "models/texture_provider.h"
#include "models/xpr.h"
#include "model_animation/model_animation.h"
#include "shader_objects/shader_objects.h"

ITC_FLAG_BOOL(native_models, true, "ITC",
              "Run the model class's pieces (docs/functions/models.md: the memory blocks' "
              "lock, the critical section at 0x8296BD38, as a host mutex for every user, "
              "through ScopedLock sub_8258E580 and sub_8258E5B8 for that critical section "
              "(chosen at start-up, other critical sections the game's); with --native_heap "
              "Memory::FreeLater sub_8258E410 and FreePhysicalLater sub_8258E498 (no adapters: "
              "only XPR::Release and Model::Release call them), the pending "
              "frees' Insert sub_8258E398 and IncSize sub_8258E2F8 (no adapters: only the "
              "put-off frees call Insert, and only Insert IncSize); XPR::XPR sub_82596E68, and with --native_heap XPR::Release "
              "sub_825970A8 and XPR::~XPR sub_82597148; the texture provider's vector: "
              "Clear sub_82590D68, and with --native_heap Allocate sub_825942B8, "
              "UninitializedCopy sub_82594380 (no adapters: only Reserve and InsertN call "
              "them), Reserve sub_82594570, InsertN sub_82590EF0 and Assign sub_825947D0 (no "
              "adapter: only the constructor calls it), TextureProvider::TextureProvider "
              "sub_82594850 and ~TextureProvider sub_825943E0; Model::Model sub_825948C0, "
              "Model::Release sub_82594970 and Model::~Model sub_82594F58, their members' "
              "plain C++ called directly, and the car part model's sub_820CE928 and "
              "sub_820CE990 over them; the part model's sub_820CE9C8; Model::MaterialCount sub_82594AD8, Model::Material "
              "sub_82594B20, ShaderFile::Parse sub_822A8568, VertexShader::CopyHeader sub_822A9FD8 and "
              "PixelShader::CopyHeader sub_822A9EA8 (in src/shader_objects/), "
              "TextureHeader::Relocate sub_8235A990 (with sub_822A5B68; no adapter: only "
              "Resource::Relocate and XPR::Create call it), Resource::Relocate sub_8235A9F8, "
              "with --native_heap XPR::Create sub_82596F68 and Mesh::LoadShaders sub_82594D20 "
              "(no adapter: only Mesh::Parse calls it; their copies as memcpy's override makes them, recorded for the renderer; "
              "VertexShader::Init's original a guest call where its plain C++ declines), "
              "with --native_heap "
              "Model::Load sub_82596920 (with Mesh::Parse sub_825963F0, no adapter: only it "
              "calls it; the callees' originals, where their plain C++ declines, guest calls), "
              "SkeletonBase::SetUp sub_8259A9E0 (with NodeFile::View sub_825A21A8, no adapter: "
              "only it calls it) and Skeleton::Bind sub_82599A20 (in src/model_animation/), "
              "Memory::AllocPhysical sub_8258E138 (in src/models/memory_blocks.cpp; no adapter: only "
              "XPR::Create calls it), the shared shaders sub_825896D8 and sub_82589700, the vertex "
              "declaration sub_82589728 (its message for a type pair the tables don't cover, "
              "never seen in play, the original's), Model::RecordShape sub_82595550 (in "
              "src/scenery_lists/model_draws.cpp, adapter in src/hooks/scenery_lists.cpp: the "
              "model's slot 1 and the texture provider's slots 5 and 6 guest calls; the "
              "material animation, never seen in play, the original's)) natively instead of through the "
              "translated code (the same writes; "
              "checked by --run_harnesses=models and, for Model::RecordShape, scenery_lists)");

bool hooks::ModelsHostLock() {
  static const bool host = ITC_FLAG(native_models);
  return host;
}

// The functions that allocate or free: only over the allocation layer, and
// with the host lock.
bool hooks::ModelsNativeLifetime() {
  return ITC_FLAG(native_models) && hooks::NativeHeapActive() && hooks::ModelsHostLock();
}

namespace {

bool NativeLifetime() { return hooks::ModelsNativeLifetime(); }

}  // namespace

// --- The memory blocks' lock ---
//
// For the critical section at 0x8296BD38 only, whatever --native_models says
// now (hooks::ModelsHostLock is fixed for the run): every user of it takes
// the host mutex, the recompiled ones here. Other critical sections go to
// the originals.

// ScopedLock::ScopedLock: r3 = the lock word, r4 = the critical section; r3
// left as it came.
extern "C" REX_FUNC(sub_8258E580) {
  REX_FUNC_PROLOGUE();
  if (ctx.r4.u32 == models::kMemoryBlocksLock && hooks::ModelsHostLock()) {
    models::ScopedLockEnter(base, ctx.r3.u32, ctx.r4.u32);
    return;
  }
  __imp__sub_8258E580(ctx, base);
}

// ScopedLock::~ScopedLock: r3 = the lock word; r3 = the critical section (the
// original's load before its LeaveCriticalSection).
extern "C" REX_FUNC(sub_8258E5B8) {
  REX_FUNC_PROLOGUE();
  if (hooks::ModelsHostLock() &&
      guest::LoadU32(base, ctx.r3.u32) == models::kMemoryBlocksLock) {
    ctx.r3.u64 = models::ScopedLockLeave(base, ctx.r3.u32);
    return;
  }
  __imp__sub_8258E5B8(ctx, base);
}

// --- The XPR object ---

// XPR::XPR: r3 = the object (left).
extern "C" REX_FUNC(sub_82596E68) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_82596E68(ctx, base);
    return;
  }
  models::XprConstruct(base, ctx.r3.u32);
}

// XPR::Release and XPR::~XPR: r3 = the object; r3 as the original leaves it.
#define ITC_MODELS_LIFETIME(address, function)                       \
  extern "C" REX_FUNC(sub_##address) {                               \
    REX_FUNC_PROLOGUE();                                             \
    if (NativeLifetime()) {                                          \
      if (const auto r3 = models::function(base, ctx.r3.u32)) {      \
        ctx.r3.u64 = *r3;                                            \
        return;                                                      \
      }                                                              \
    }                                                                \
    __imp__sub_##address(ctx, base);                                 \
  }

ITC_MODELS_LIFETIME(825970A8, XprRelease)
ITC_MODELS_LIFETIME(82597148, XprDestruct)

// --- The texture provider ---

namespace {

void SetR3(PPCContext& ctx, const models::R3& r3) {
  if (!r3.kept) {
    ctx.r3.u64 = r3.value;
  }
}

}  // namespace

// TextureProvider::Reserve: r3 = the vector, r4 = the count.
extern "C" REX_FUNC(sub_82594570) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    if (const auto r3 = models::PointersReserve(base, ctx.r3.u32, ctx.r4.u32)) {
      SetR3(ctx, *r3);
      return;
    }
  }
  __imp__sub_82594570(ctx, base);
}

// TextureProvider::InsertN: r3 = the vector, r4 = where, r5 = the count, r6 =
// the value's address.
extern "C" REX_FUNC(sub_82590EF0) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    if (const auto r3 = models::PointersInsertN(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                guest::LoadU32(base, ctx.r6.u32))) {
      SetR3(ctx, *r3);
      return;
    }
  }
  __imp__sub_82590EF0(ctx, base);
}

// TextureProvider::Clear: r3 = the vector; r3 = its first element.
extern "C" REX_FUNC(sub_82590D68) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_82590D68(ctx, base);
    return;
  }
  ctx.r3.u64 = models::PointersClear(base, ctx.r3.u32);
}

// TextureProvider::TextureProvider: r3 = the provider (left), r4 = the slot
// count (-1: 64).
extern "C" REX_FUNC(sub_82594850) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !models::TextureProviderConstruct(base, ctx.r3.u32, ctx.r4.s32)) {
    __imp__sub_82594850(ctx, base);
  }
}

// TextureProvider::~TextureProvider: r3 = the provider; r3 = its storage.
ITC_MODELS_LIFETIME(825943E0, TextureProviderDestruct)

// --- The model ---

// Model::Model: r3 = the model (left).
extern "C" REX_FUNC(sub_825948C0) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    ctx.fpscr.disableFlushMode();
    if (models::ModelConstruct(base, ctx.r3.u32)) {
      return;
    }
  }
  __imp__sub_825948C0(ctx, base);
}

// Model::Release and Model::~Model: r3 = the model; r3 as the original
// leaves it (XPR::Release's, XPR::~XPR's).
ITC_MODELS_LIFETIME(82594970, ModelRelease)
ITC_MODELS_LIFETIME(82594F58, ModelDestruct)

// --- The car part model ---

// The part model's constructor: r3 = the model (left).
extern "C" REX_FUNC(sub_820CE928) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    ctx.fpscr.disableFlushMode();  // as Model::Model's adapter
    if (models::PartModelConstruct(base, ctx.r3.u32)) {
      return;
    }
  }
  __imp__sub_820CE928(ctx, base);
}

// The part model's destructor: r3 = the model; r3 as Model::~Model leaves it.
ITC_MODELS_LIFETIME(820CE990, PartModelDestruct)

// sub_820CE9C8: r3 = the model (left).
extern "C" REX_FUNC(sub_820CE9C8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_820CE9C8(ctx, base);
    return;
  }
  models::PartModelClear(base, ctx.r3.u32);
}

// --- The materials and the shared shaders ---

namespace hooks {

void ModelsMaterialCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::MaterialCount(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32);
}

void ModelsMaterial(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::Material(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32);
}

void ModelsSharedVertexShader(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::SharedVertexShader(base, ctx.r3.u32);
}

void ModelsSharedPixelShader(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = models::SharedPixelShader(base, ctx.r3.u32);
}

void ModelsVertexDeclaration(PPCContext& ctx, uint8_t* base) {
  const auto declaration =
      models::VertexDeclaration(base, ctx.r3.u32, ctx.r4.s32, ctx.r5.u32);
  if (!declaration) {
    __imp__sub_82589728(ctx, base);
    return;
  }
  ctx.r3.u64 = *declaration;
}

}  // namespace hooks

#define ITC_MODELS_ACCESSOR(address, steps)  \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_models)) {          \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    hooks::steps(ctx, base);                 \
  }

ITC_MODELS_ACCESSOR(82594AD8, ModelsMaterialCount)
ITC_MODELS_ACCESSOR(82594B20, ModelsMaterial)
ITC_MODELS_ACCESSOR(825896D8, ModelsSharedVertexShader)
ITC_MODELS_ACCESSOR(82589700, ModelsSharedPixelShader)
ITC_MODELS_ACCESSOR(82589728, ModelsVertexDeclaration)

// ShaderFile::Parse: r3 = the file, r4 = the record. No return value.
extern "C" REX_FUNC(sub_822A8568) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_822A8568(ctx, base);
    return;
  }
  shader_objects::ParseFile(base, ctx.r3.u32, ctx.r4.u32);
}

// VertexShader::CopyHeader and PixelShader::CopyHeader: r3 = the object, r5
// = the file's record (r4 unread). No return value.
extern "C" REX_FUNC(sub_822A9FD8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models) ||
      !shader_objects::CopyVertexHeader(base, ctx.r3.u32, ctx.r5.u32)) {
    __imp__sub_822A9FD8(ctx, base);
  }
}

extern "C" REX_FUNC(sub_822A9EA8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models) ||
      !shader_objects::CopyPixelHeader(base, ctx.r3.u32, ctx.r5.u32)) {
    __imp__sub_822A9EA8(ctx, base);
  }
}

// Resource::Relocate: r3 = the resource, r4 = the address; r3 as the
// original leaves it.
extern "C" REX_FUNC(sub_8235A9F8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_8235A9F8(ctx, base);
    return;
  }
  ctx.r3.u64 = models::RelocateResource(base, ctx.r3.u32, ctx.r4.u32);
}

// XPR::Create: r3 = the XPR object, r4 = the XPR2 file; returns 0 or
// 0x80004005.
extern "C" REX_FUNC(sub_82596F68) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    if (const auto r3 = models::XprCreate(base, ctx.r3.u32, ctx.r4.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_82596F68(ctx, base);
}

// SkeletonBase::SetUp: r3 = the skeleton, r4 = the node file, r5 = the
// matrices or 0, r6 = the instance count. Skeleton::Bind: r3 to r5 the same.
// No return value read. Flush-to-zero off, as the originals leave it.
extern "C" REX_FUNC(sub_8259A9E0) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    ctx.fpscr.disableFlushMode();
    if (model_animation::SkeletonSetUp(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32)) {
      return;
    }
  }
  __imp__sub_8259A9E0(ctx, base);
}

extern "C" REX_FUNC(sub_82599A20) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    ctx.fpscr.disableFlushMode();
    if (model_animation::SkeletonBind(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
      return;
    }
  }
  __imp__sub_82599A20(ctx, base);
}

namespace {

// The guest calls Model::Load makes where a callee's plain C++ declines,
// from a frame below the adapter's (room for the original's own frames).
void ModelLoadInit(void* user, uint32_t object, uint32_t microcode) {
  auto& c = *static_cast<hooks::ModelLoadGuest*>(user);
  guest_call::InFrame(c.ctx, c.base, 512, 0x82594DEC, [&](uint32_t) {
    c.ctx.r3.u64 = object;
    c.ctx.r4.u64 = microcode;
    __imp__sub_822A8740(c.ctx, c.base);
  });
}
void ModelLoadCall(void* user, uint32_t function, uint32_t r3, uint32_t r4, uint32_t r5,
                   uint32_t return_address) {
  auto& c = *static_cast<hooks::ModelLoadGuest*>(user);
  PPCContext& ctx = c.ctx;
  uint8_t* base = c.base;
  guest_call::InFrame(ctx, base, 512, return_address, [&](uint32_t) {
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    REX_CALL_INDIRECT_FUNC(function);
  });
}

}  // namespace

models::LoadCalls hooks::ModelLoadGuest::Calls() {
  models::LoadCalls calls;
  calls.init = &ModelLoadInit;
  calls.call = &ModelLoadCall;
  calls.user = this;
  return calls;
}

// Model::Load: r3 = the model, r4 = the part file, r5 = an XPR2 file or 0;
// r3 = the model's shape count (the original's loop counter). Flush-to-zero
// off, as the original leaves it.
extern "C" REX_FUNC(sub_82596920) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    ctx.fpscr.disableFlushMode();
    const uint64_t lr = ctx.lr;
    const uint32_t model = ctx.r3.u32, file = ctx.r4.u32, xpr = ctx.r5.u32;
    hooks::ModelLoadGuest guest{ctx, base};
    const bool done = models::ModelLoad(base, model, file, xpr, guest.Calls());
    ctx.lr = lr;
    if (done) {
      ctx.r3.u64 = guest::LoadU32(base, model + 412);
      return;
    }
    ctx.r3.u64 = model;
    ctx.r4.u64 = file;
    ctx.r5.u64 = xpr;
  }
  __imp__sub_82596920(ctx, base);
}
