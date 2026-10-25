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
// (sub_825947D0), from TextureProvider::TextureProvider.

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "models/memory_blocks.h"
#include "models/model.h"
#include "models/texture_provider.h"
#include "models/xpr.h"
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
              "plain C++ called directly; Model::MaterialCount sub_82594AD8, Model::Material "
              "sub_82594B20, ShaderFile::Parse sub_822A8568, VertexShader::CopyHeader sub_822A9FD8 and "
              "PixelShader::CopyHeader sub_822A9EA8 (in src/shader_objects/), "
              "TextureHeader::Relocate sub_8235A990 (with sub_822A5B68), with --native_heap "
              "Memory::AllocPhysical sub_8258E138 (in src/models/memory_blocks.cpp), the shared shaders sub_825896D8 and sub_82589700, the vertex "
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

namespace {

// The functions that allocate or free: only over the allocation layer, and
// with the host lock.
bool NativeLifetime() {
  return ITC_FLAG(native_models) && hooks::NativeHeapActive() && hooks::ModelsHostLock();
}

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

// Memory::AllocPhysical: r3 = bytes, r4 = XMemAlloc's attributes; returns the
// block or 0.
extern "C" REX_FUNC(sub_8258E138) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime()) {
    __imp__sub_8258E138(ctx, base);
    return;
  }
  ctx.r3.u64 = models::AllocPhysical(base, ctx.r3.u32, ctx.r4.u32);
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

// TextureHeader::Relocate: r3 = the header, r4 and r5 the base and mip
// addresses; r3 = the level count (as the original leaves it).
extern "C" REX_FUNC(sub_8235A990) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_models)) {
    __imp__sub_8235A990(ctx, base);
    return;
  }
  ctx.r3.u64 = models::RelocateTexture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
