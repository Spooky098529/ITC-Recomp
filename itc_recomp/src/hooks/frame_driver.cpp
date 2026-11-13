// The frame driver and the app's scene natively: adapters in front of
// src/frame_driver/ (docs/functions/frame-driver.md), with the register steps
// the harness shares (hooks/frame_driver.h).

#include "hooks/frame_driver.h"

#include "app_states/app.h"
#include "base/flags.h"
#include "framework/run_loop.h"
#include "graphics/device_state.h"
#include "hooks/device_state_calls.h"
#include "hooks/app_states.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/last_error.h"
#include "frame_driver/frame_driver.h"
#include "frame_driver/thread.h"

ITC_FLAG_DECLARE(bool, native_video_output);
ITC_FLAG_DECLARE(bool, native_device_state);

ITC_FLAG_BOOL(native_frame_driver, true, "ITC",
              "Run the frame driver at framework + 76 and the app's scene at framework + 80 "
              "(docs/functions/frame-driver.md): FrameDriver::GetVideoOutput sub_824960E0, "
              "FrameDriver::AddListener sub_820922B8 (the delegate's copy and, with --native_heap, "
              "the request pushed), FrameDriver::ApplyRequest sub_82091FF0 (type 0 with "
              "--native_heap; a removal, never seen in play, the original), "
              "FrameDriver::CallListener sub_82091A98 (the delegate a guest call) and, with "
              "--native_video_output, FrameDriver::DrawFrame sub_82091E58 (no adapter: "
              "FrameDriver::ThreadLoop's override calls it; the listeners guest calls), "
              "Scene::Update sub_820988C8 (inside its --log_boot_modes wrap; with sub_82591838, "
              "no adapter; the render queue's slots guest calls) and Scene::Draw sub_82098940 (with "
              "VideoOutput::BeginFrame sub_8258D958, no adapter: Device::SetShaderGprAllocation's "
              "plain C++ with --native_device_state; the render queue's replay a guest call, its "
              "wraps kept), Thread::Thread sub_8258DE88 and Thread::Begin sub_8258DF18 (with "
              "CreateThread sub_825BA158 and sub_825BA380, SetThreadPriority sub_822977B0, "
              "XSetThreadProcessor sub_82297888 and ResumeThread sub_825BA118, no adapters: "
              "the kernel imports guest calls as the original makes them) natively "
              "instead of through the translated code (the same results; checked by "
              "--run_harnesses=frame_driver)");

namespace fd = frame_driver;

namespace hooks {

namespace {

bool Heap() { return hooks::NativeHeapActive(); }
uint32_t High(uint64_t value) { return uint32_t(value >> 32); }

}  // namespace

bool NativeFrameDriver() { return ITC_FLAG(native_frame_driver); }

void FrameDriverGetVideoOutput(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = fd::GetVideoOutput(ctx.r3.u32);
}

void FrameDriverAddListener(PPCContext& ctx, uint8_t* base) {
  const uint32_t interface = ctx.r3.u32;
  const uint32_t key = ctx.r4.u32;
  const uint32_t delegate = ctx.r5.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  guest_call::InFrame(ctx, base, 112, 0x820922F0, [&](uint32_t frame) {
    const uint32_t request = frame + 80;
    const uint32_t vector = interface + (fd::kDriverRequests - fd::kDriverInterface);
    const auto inserted = fd::AddListener(base, interface, key, delegate, request, calls);
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

void FrameDriverApplyRequest(PPCContext& ctx, uint8_t* base) {
  std::optional<bool> inserted;
  const uint32_t type = High(ctx.r4.u64);
  if (type != 0 || (Heap() && framework::CanAllocate(base))) {
    inserted = fd::ApplyRequest(base, ctx.r3.u32, type, ctx.r4.u32, High(ctx.r5.u64));
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

void FrameDriverCallListener(PPCContext& ctx, uint8_t* base) {
  const uint32_t driver = ctx.r3.u32;
  const uint64_t entry = ctx.r4.u64;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  uint32_t answer = 0;
  guest_call::InFrame(ctx, base, 96, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = fd::CallListener(base, driver, entry, frame, calls);
  });
  ctx.r3.u64 = answer;
}

void FrameDriverDrawFrame(PPCContext& ctx, uint8_t* base) {
  if (!NativeFrameDriver() || !ITC_FLAG(native_video_output)) {
    __imp__sub_82091E58(ctx, base);
    return;
  }
  const uint32_t driver = ctx.r3.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(144, uint32_t(ctx.lr), [&](uint32_t frame) {
    fd::DrawFrame(base, driver, frame, calls);
  });
}

void AppSceneUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t scene = ctx.r3.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t) { fd::SceneUpdate(base, scene, calls); });
}

void AppSceneDraw(PPCContext& ctx, uint8_t* base) {
  const uint32_t scene = ctx.r3.u32, interface = ctx.r4.u32, device = ctx.r5.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  // Device::SetShaderGprAllocation from VideoOutput::BeginFrame's 96-byte
  // frame (its call returning to 0x8258D978), in its own 112-byte frame as
  // its adapter makes it; its original without --native_device_state.
  const fd::SetGpr set_gpr = [&](uint32_t gpr_device, uint32_t vertex, uint32_t pixel) {
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
    video_output = fd::SceneDraw(base, scene, interface, device, calls, set_gpr);
  });
  ctx.r3.u64 = video_output;
}

namespace {

fd::ThreadCalls ThreadCallsOn(PPCContext& ctx, uint8_t* base) {
  fd::ThreadCalls calls;
  calls.import = [&ctx, base](fd::Import import, std::initializer_list<uint64_t> args,
                              uint32_t return_address) {
    int reg = 3;
    for (uint64_t arg : args) {
      guest_call::Gpr(ctx, reg++).u64 = arg;
    }
    ctx.lr = return_address;
    switch (import) {
      case fd::Import::kExCreateThread:
        __imp__ExCreateThread(ctx, base);
        break;
      case fd::Import::kObReferenceObjectByHandle:
        __imp__ObReferenceObjectByHandle(ctx, base);
        break;
      case fd::Import::kKeSetBasePriorityThread:
        __imp__KeSetBasePriorityThread(ctx, base);
        break;
      case fd::Import::kObDereferenceObject:
        __imp__ObDereferenceObject(ctx, base);
        break;
      case fd::Import::kKeSetAffinityThread:
        __imp__KeSetAffinityThread(ctx, base);
        break;
      case fd::Import::kNtResumeThread:
        __imp__NtResumeThread(ctx, base);
        break;
    }
    return ctx.r3.u32;
  };
  calls.in_frame = [&ctx, base](uint32_t bytes, uint32_t return_address,
                                const std::function<void(uint32_t)>& body) {
    uint64_t kept[18];
    for (int i = 0; i < 18; i++) {
      kept[i] = (&ctx.r14)[i].u64;
    }
    guest_call::InFrame(ctx, base, bytes, return_address, body);
    for (int i = 0; i < 18; i++) {
      (&ctx.r14)[i].u64 = kept[i];
    }
  };
  calls.set_last_error_from_status = [&ctx, base](uint32_t status) {
    last_error::SetFromStatus(base, ctx.r13.u32, status);
  };
  return calls;
}

}  // namespace

void ThreadConstruct(PPCContext& ctx, uint8_t* base) { fd::ThreadConstruct(base, ctx.r3.u32); }

void ThreadBegin(PPCContext& ctx, uint8_t* base) {
  const uint32_t thread = ctx.r3.u32, object = ctx.r4.u32, processor = ctx.r5.u32,
                 priority = ctx.r6.u32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = fd::ThreadBegin(base, thread, object, processor, priority, calls);
}

void CreateThreadStep(PPCContext& ctx, uint8_t* base) {
  const uint32_t stack = ctx.r4.u32, entry = ctx.r5.u32, parameter = ctx.r6.u32,
                 flags = ctx.r7.u32, processor = ctx.r8.u32, id = ctx.r9.u32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = fd::CreateThread(base, stack, entry, parameter, flags, processor, id, calls);
}

void SetThreadPriorityStep(PPCContext& ctx, uint8_t* base) {
  const uint32_t handle = ctx.r3.u32;
  const int32_t priority = ctx.r4.s32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = fd::SetThreadPriority(base, handle, priority, calls);
}

void XSetThreadProcessorStep(PPCContext& ctx, uint8_t* base) {
  const uint32_t handle = ctx.r3.u32, processor = ctx.r4.u32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = uint64_t(int64_t(int32_t(fd::XSetThreadProcessor(base, handle, processor, calls))));
}

void ResumeThreadStep(PPCContext& ctx, uint8_t* base) {
  const uint32_t handle = ctx.r3.u32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = uint64_t(int64_t(int32_t(fd::ResumeThread(base, handle, calls))));
}

}  // namespace hooks

#define ITC_FRAME_DRIVER_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {              \
    REX_FUNC_PROLOGUE();                            \
    if (!hooks::NativeFrameDriver()) {          \
      __imp__sub_##address(ctx, base);              \
      return;                                       \
    }                                               \
    hooks::step(ctx, base);                         \
  }

ITC_FRAME_DRIVER_ADAPTER(824960E0, FrameDriverGetVideoOutput)
ITC_FRAME_DRIVER_ADAPTER(820922B8, FrameDriverAddListener)
ITC_FRAME_DRIVER_ADAPTER(82091FF0, FrameDriverApplyRequest)
ITC_FRAME_DRIVER_ADAPTER(82091A98, FrameDriverCallListener)
ITC_FRAME_DRIVER_ADAPTER(82098940, AppSceneDraw)
ITC_FRAME_DRIVER_ADAPTER(8258DE88, ThreadConstruct)
ITC_FRAME_DRIVER_ADAPTER(8258DF18, ThreadBegin)
