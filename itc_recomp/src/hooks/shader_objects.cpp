// The shader objects natively: adapters in front of src/shader_objects/
// (docs/functions/shader-objects.md).

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "models/memory_blocks.h"
#include "shader_objects/shader_objects.h"

ITC_FLAG_BOOL(native_shader_objects, true, "ITC",
              "Run the graphics library's shader objects (docs/functions/shader-objects.md: "
              "with --native_heap VertexShader::Create sub_822AA018 "
              "(with VertexShader::Init sub_822A8740's plain C++, no adapter: only native callers "
              "call it; its original, for an object in the physical views or a fetch table "
              "sharing memory with its fields, never seen in play, a guest call) and "
              "PixelShader::Create sub_822A9EE8 (PixelShader::Init sub_822A85B8's work inline, as "
              "the original's; its plain C++ has no adapter: only native callers call it; their microcode copied as memcpy's override "
              "copies it, recorded for the renderer); with --native_heap and the memory blocks' host "
              "lock (--native_models at start-up), the memory blocks' Memory::AllocVirtual "
              "sub_8258DFF0, Memory::AllocPhysicalEx sub_8258E098 and PendingFrees::FreeDue "
              "sub_8258E1D8 (a block the layer can't free natively, never seen in play, the "
              "original's)) natively instead of through the translated code "
              "(the same writes; checked by --run_harnesses=shader_objects)");

// VertexShader::Init (sub_822A8740) and PixelShader::Init (sub_822A85B8)
// have no adapters: only native callers reach them in play (Mesh::LoadShaders,
// Shaders::OnPairLoaded and VertexShader::Create call their plain C++).

// --- The memory blocks ---

namespace {

// Only over the allocation layer, with the memory blocks' host lock.
bool NativeBlocks() {
  return ITC_FLAG(native_shader_objects) && hooks::NativeHeapActive() && hooks::ModelsHostLock();
}

}  // namespace

// Memory::AllocVirtual: r3 = bytes; returns the block or 0.
extern "C" REX_FUNC(sub_8258DFF0) {
  REX_FUNC_PROLOGUE();
  if (!NativeBlocks()) {
    __imp__sub_8258DFF0(ctx, base);
    return;
  }
  ctx.r3.u64 = models::AllocVirtual(base, ctx.r3.u32);
}

// Memory::AllocPhysicalEx: r3 = bytes, r4 and r5 the lowest and highest
// physical address, r6 the alignment, r7 the protection; returns the block or
// 0.
extern "C" REX_FUNC(sub_8258E098) {
  REX_FUNC_PROLOGUE();
  if (!NativeBlocks()) {
    __imp__sub_8258E098(ctx, base);
    return;
  }
  ctx.r3.u64 = models::AllocPhysicalEx(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                       ctx.r7.u32);
}

// PendingFrees::FreeDue: no arguments, no return value (r3 is the lock word's
// address the original leaves, read by no caller).
extern "C" REX_FUNC(sub_8258E1D8) {
  REX_FUNC_PROLOGUE();
  if (!NativeBlocks() || !models::FreeDue(base)) {
    __imp__sub_8258E1D8(ctx, base);
  }
}

// --- The creates ---

namespace {

struct CreateContext {
  PPCContext& ctx;
  uint8_t* base;
  // VertexShader::Init's original, from a frame as the create's own.
  static void Init(void* user, uint32_t object, uint32_t microcode) {
    auto& c = *static_cast<CreateContext*>(user);
    guest_call::InFrame(c.ctx, c.base, 128, 0x822AA0CC, [&](uint32_t) {
      c.ctx.r3.u64 = object;
      c.ctx.r4.u64 = microcode;
      __imp__sub_822A8740(c.ctx, c.base);
    });
  }
};

}  // namespace

// VertexShader::Create and PixelShader::Create: r3 = the shader file;
// returns the object or 0.
extern "C" REX_FUNC(sub_822AA018) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_shader_objects) && hooks::NativeHeapActive()) {
    const uint64_t lr = ctx.lr;
    CreateContext context{ctx, base};
    shader_objects::CreateCalls calls;
    calls.init = &CreateContext::Init;
    calls.user = &context;
    const auto object = shader_objects::VertexShaderCreate(base, ctx.r3.u32, calls);
    ctx.lr = lr;
    if (object) {
      ctx.r3.u64 = *object;
      return;
    }
  }
  __imp__sub_822AA018(ctx, base);
}

extern "C" REX_FUNC(sub_822A9EE8) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_shader_objects) && hooks::NativeHeapActive()) {
    if (const auto object = shader_objects::PixelShaderCreate(base, ctx.r3.u32)) {
      ctx.r3.u64 = *object;
      return;
    }
  }
  __imp__sub_822A9EE8(ctx, base);
}
