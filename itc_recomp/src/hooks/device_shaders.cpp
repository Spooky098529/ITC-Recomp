// The graphics device's shader setters and the render queue's pass lists
// natively: adapters in front of src/graphics/device_shaders.h
// (docs/functions/device-shaders.md). Each leaves r3 as the original does.
//
// Device::SetVertexShader (sub_822A8808) was Wrapped by src/native/refcount.cpp
// (the rebind skip: AddRef and Release of the shader already bound skipped);
// the native one skips the pair itself. Its only caller in play is
// sub_822A8958, whose adapter calls its plain C++, so it has no adapter of
// its own; the entry's fallback runs it recompiled with the wrapper's skip.
// PassList::PassList (sub_82590B38) likewise: only RenderQueue::RenderQueue
// calls it.

#include "guest.h"

#include "base/flags.h"
#include "graphics/device_shaders.h"
#include "hooks/heap.h"
#include "native/refcount.h"

ITC_FLAG_BOOL(native_device_shaders, true, "ITC",
              "Run the graphics device's shader setters (docs/functions/device-shaders.md: "
              "D3DDevice_SetVertexShader sub_822A8958 with Device::SetVertexShader "
              "sub_822A8808, SetPixelShader sub_822A8618, SetVertexDeclaration sub_822A8960) "
              "and the render queue: RenderQueue::RenderQueue sub_825915E0, PassList::PassList "
              "sub_82590B38 (both with the native heap only) and PassList::Clear sub_8258EB38 natively instead of through "
              "the translated code (the same writes to the bound objects, the register shadow "
              "and the dirty masks; checked by --run_harnesses=device_shaders)");
ITC_FLAG_DECLARE(bool, refcount_global_lock);

namespace dsh = graphics::device_shaders;

namespace {

// Release and memcpy through the guest, with the original's return addresses.
dsh::ShaderCalls Calls(PPCContext& ctx, uint8_t* base, uint32_t release_at) {
  dsh::ShaderCalls calls;
  calls.release = [&ctx, base, release_at](uint32_t resource) {
    ctx.r3.u64 = resource;
    ctx.lr = release_at;
    sub_822A67B0(ctx, base);
    return ctx.r3.u32;
  };
  calls.memcpy = [&ctx, base](uint32_t destination, uint32_t source, uint32_t size,
                              uint32_t at) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = source;
    ctx.r5.u64 = size;
    ctx.lr = at;
    sub_825A7410(ctx, base);
  };
  return calls;
}

// The recompiled SetVertexShader with refcount.cpp's rebind skip around it
// (the wrapper as it was), for the entry's fallback.
void RecompiledSetVertexShader(PPCContext& ctx, uint8_t* base) {
  if (ITC_FLAG(refcount_global_lock)) {
    __imp__sub_822A8808(ctx, base);
    return;
  }
  const uint32_t shader = ctx.r4.u32;
  const bool same = shader && shader == guest::LoadU32(base, ctx.r3.u32 + 12436);
  refcount::NoteSetVertexShader(same);
  if (same) refcount::SetUnchangedShader(shader);
  __imp__sub_822A8808(ctx, base);
  refcount::SetUnchangedShader(0);
}

}  // namespace

extern "C" REX_FUNC(sub_822A8958) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_device_shaders)) {
    // The original's store, then its tail call with refcount.cpp's skip.
    guest::StoreU32(base, ctx.r3.u32 + 13156, ctx.r4.u32);
    RecompiledSetVertexShader(ctx, base);
    return;
  }
  const dsh::ShaderCalls calls = Calls(ctx, base, 0x822A883C);
  ctx.r3.u64 = dsh::SetVertexShaderEntry(base, ctx.r3.u32, ctx.r4.u32, calls);
}

extern "C" REX_FUNC(sub_822A8618) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_device_shaders)) {
    __imp__sub_822A8618(ctx, base);
    return;
  }
  const dsh::ShaderCalls calls = Calls(ctx, base, 0);
  ctx.r3.u64 = dsh::SetPixelShader(base, ctx.r3.u32, ctx.r4.u32, calls);
}

extern "C" REX_FUNC(sub_822A8960) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_device_shaders)) {
    __imp__sub_822A8960(ctx, base);
    return;
  }
  dsh::SetVertexDeclaration(base, ctx.r3.u32, ctx.r4.u32);  // r3: the device, as given
}

extern "C" REX_FUNC(sub_8258EB38) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_device_shaders)) {
    __imp__sub_8258EB38(ctx, base);
    return;
  }
  dsh::PassListClear(base, ctx.r3.u32);  // r3: the list, as given
}

// r3: the queue, as given (the original returns it).
extern "C" REX_FUNC(sub_825915E0) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_device_shaders) && hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    if (dsh::RenderQueueConstruct(base, ctx.r3.u32)) {
      return;
    }
  }
  __imp__sub_825915E0(ctx, base);
}
