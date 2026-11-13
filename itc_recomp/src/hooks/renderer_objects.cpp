// The renderer and the app's scene natively: adapters in front of
// src/renderer_objects/ (docs/functions/renderer.md), with the register steps
// the harness shares (hooks/renderer_objects.h).

#include "hooks/renderer_objects.h"

#include "app_states/app.h"
#include "base/flags.h"
#include "framework/run_loop.h"
#include "graphics/device_state.h"
#include "hooks/device_state_calls.h"
#include "hooks/app_states.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "renderer_objects/renderer_objects.h"

ITC_FLAG_DECLARE(bool, native_video_output);
ITC_FLAG_DECLARE(bool, native_device_state);

ITC_FLAG_BOOL(native_renderer_objects, true, "ITC",
              "Run the renderer at framework + 76 and the app's scene at framework + 80 "
              "(docs/functions/renderer.md): Renderer::GetVideoOutput sub_824960E0, "
              "Renderer::AddListener sub_820922B8 (the delegate's copy and, with --native_heap, "
              "the request pushed), Renderer::ApplyRequest sub_82091FF0 (type 0 with "
              "--native_heap; a removal, never seen in play, the original), "
              "Renderer::CallListener sub_82091A98 (the delegate a guest call) and, with "
              "--native_video_output, Renderer::DrawFrame sub_82091E58 (no adapter: "
              "Renderer::ThreadLoop's override calls it; the listeners guest calls), "
              "Scene::Update sub_820988C8 (inside its --log_boot_modes wrap; with sub_82591838, "
              "no adapter; the render queue's slots guest calls) and Scene::Draw sub_82098940 (with "
              "VideoOutput::BeginFrame sub_8258D958, no adapter: Device::SetShaderGprAllocation's "
              "plain C++ with --native_device_state; the render queue's replay a guest call, its "
              "wraps kept) natively "
              "instead of through the translated code (the same results; checked by "
              "--run_harnesses=renderer_objects)");

namespace ro = renderer_objects;

namespace hooks {

namespace {

bool Heap() { return hooks::NativeHeapActive(); }
uint32_t High(uint64_t value) { return uint32_t(value >> 32); }

}  // namespace

bool NativeRendererObjects() { return ITC_FLAG(native_renderer_objects); }

void RendererGetVideoOutput(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ro::GetVideoOutput(ctx.r3.u32);
}

void RendererAddListener(PPCContext& ctx, uint8_t* base) {
  const uint32_t interface = ctx.r3.u32;
  const uint32_t key = ctx.r4.u32;
  const uint32_t delegate = ctx.r5.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  guest_call::InFrame(ctx, base, 112, 0x820922F0, [&](uint32_t frame) {
    const uint32_t request = frame + 80;
    const uint32_t vector = interface + (ro::kRendererRequests - ro::kRendererInterface);
    const auto inserted = ro::AddListener(base, interface, key, delegate, request, calls);
    if (!inserted) {
      ctx.r3.u64 = vector;
      ctx.r4.u64 = request;
      ctx.lr = 0x82092300;
      __imp__sub_8209A930(ctx, base);
      return;
    }
    ctx.r3.u64 = *inserted ? frame - 16 : vector;
  });
}

void RendererApplyRequest(PPCContext& ctx, uint8_t* base) {
  std::optional<bool> inserted;
  const uint32_t type = High(ctx.r4.u64);
  if (type != 0 || (Heap() && framework::CanAllocate(base))) {
    inserted = ro::ApplyRequest(base, ctx.r3.u32, type, ctx.r4.u32, High(ctx.r5.u64));
  }
  if (!inserted) {
    __imp__sub_82091FF0(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r4.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r5.u64);
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 32;
  }
}

void RendererCallListener(PPCContext& ctx, uint8_t* base) {
  const uint32_t renderer = ctx.r3.u32;
  const uint64_t entry = ctx.r4.u64;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  uint32_t answer = 0;
  guest_call::InFrame(ctx, base, 96, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = ro::CallListener(base, renderer, entry, frame, calls);
  });
  ctx.r3.u64 = answer;
}

void RendererDrawFrame(PPCContext& ctx, uint8_t* base) {
  if (!NativeRendererObjects() || !ITC_FLAG(native_video_output)) {
    __imp__sub_82091E58(ctx, base);
    return;
  }
  const uint32_t renderer = ctx.r3.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(144, uint32_t(ctx.lr), [&](uint32_t frame) {
    ro::DrawFrame(base, renderer, frame, calls);
  });
}

void AppSceneUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t scene = ctx.r3.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t) { ro::SceneUpdate(base, scene, calls); });
}

void AppSceneDraw(PPCContext& ctx, uint8_t* base) {
  const uint32_t scene = ctx.r3.u32, interface = ctx.r4.u32, device = ctx.r5.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  // Device::SetShaderGprAllocation from VideoOutput::BeginFrame's 96-byte
  // frame (its call returning to 0x8258D978), in its own 112-byte frame as
  // its adapter makes it; its original without --native_device_state.
  const ro::SetGpr set_gpr = [&](uint32_t gpr_device, uint32_t vertex, uint32_t pixel) {
    calls.in_frame(96, 0x8258D978, [&](uint32_t) {
      if (!ITC_FLAG(native_device_state)) {
        ctx.r3.u64 = gpr_device;
        ctx.r4.u64 = 0;
        ctx.r5.u64 = vertex;
        ctx.r6.u64 = pixel;
        ctx.lr = 0x8258D978;
        __imp__sub_822A8A60(ctx, base);
        return;
      }
      guest_call::InFrame(ctx, base, 112, 0x822A8AF4, [&](uint32_t) {
        graphics::device_state::SetShaderGprAllocation(
            base, gpr_device, vertex, pixel, hooks::DeviceStateCalls(ctx, base, gpr_device));
      });
    });
  };
  uint32_t video_output = 0;
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t) {
    video_output = ro::SceneDraw(base, scene, interface, device, calls, set_gpr);
  });
  ctx.r3.u64 = video_output;
}

}  // namespace hooks

#define ITC_RENDERER_OBJECTS_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {              \
    REX_FUNC_PROLOGUE();                            \
    if (!hooks::NativeRendererObjects()) {          \
      __imp__sub_##address(ctx, base);              \
      return;                                       \
    }                                               \
    hooks::step(ctx, base);                         \
  }

ITC_RENDERER_OBJECTS_ADAPTER(824960E0, RendererGetVideoOutput)
ITC_RENDERER_OBJECTS_ADAPTER(820922B8, RendererAddListener)
ITC_RENDERER_OBJECTS_ADAPTER(82091FF0, RendererApplyRequest)
ITC_RENDERER_OBJECTS_ADAPTER(82091A98, RendererCallListener)
ITC_RENDERER_OBJECTS_ADAPTER(82098940, AppSceneDraw)
