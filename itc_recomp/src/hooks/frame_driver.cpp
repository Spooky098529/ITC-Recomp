// The frame driver and the app's scene natively: adapters in front of
// src/frame_driver/ (docs/functions/frame-driver.md), with the register steps
// the harness shares (hooks/frame_driver.h).

#include "hooks/frame_driver.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

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
#include "frame_driver/scene_set_up.h"
#include "frame_driver/thread.h"
#include "lifted/sdk.h"

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
              "the kernel imports guest calls as the original makes them) and "
              "VideoOutput::UnbindAll sub_8258D9F8 (the setters guest calls through their "
              "adapters) and, with --native_video_output, Scene::Scene sub_82098A98 (with "
              "CommandList::Init sub_8258CD28, ColorRect::SetTargets sub_82178B28, "
              "DrawGlobals::Init sub_82589E78 and PrimitiveDecl::Init sub_82591858, no adapters; "
              "the interfaces' slots, the render queue's constructor and Init, "
              "CreateVertexDeclaration, operator new and delete and the memory blocks' makers "
              "guest calls through their adapters) natively "
              "instead of through the translated code (the same results; checked by "
              "--run_harnesses=frame_driver)");

namespace fd = frame_driver;

namespace hooks {

namespace {

bool Heap() { return hooks::NativeHeapActive(); }
uint32_t High(uint64_t value) { return uint32_t(value >> 32); }

}  // namespace

bool NativeFrameDriver() { return ITC_FLAG(native_frame_driver); }

ITC_REGS_STEP(FrameDriverGetVideoOutput) {
  ctx.r3.u64 = fd::GetVideoOutput(ctx.r3.u32);
}

ITC_REGS_STEP(FrameDriverAddListener) {
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
      lifted::original::c8209A930(ctx, base);
      return;
    }
    ctx.r3.u64 = *inserted ? frame - 16 : vector;
  });
}

ITC_REGS_STEP(FrameDriverApplyRequest) {
  std::optional<bool> inserted;
  const uint32_t type = High(ctx.r4.u64);
  if (type != 0 || (Heap() && framework::CanAllocate(base))) {
    inserted = fd::ApplyRequest(base, ctx.r3.u32, type, ctx.r4.u32, High(ctx.r5.u64));
  }
  if (!inserted) {
    lifted::original::c82091FF0(ctx, base);
    return;
  }
  guest::StoreU64(base, ctx.r1.u32 + 32, ctx.r4.u64);
  guest::StoreU64(base, ctx.r1.u32 + 40, ctx.r5.u64);
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 32;
  }
}

ITC_REGS_STEP(FrameDriverCallListener) {
  const uint32_t driver = ctx.r3.u32;
  const uint64_t entry = ctx.r4.u64;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  uint32_t answer = 0;
  guest_call::InFrame(ctx, base, 96, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = fd::CallListener(base, driver, entry, frame, calls);
  });
  ctx.r3.u64 = answer;
}

ITC_REGS_STEP(FrameDriverDrawFrame) {
  if (!NativeFrameDriver() || !ITC_FLAG(native_video_output)) {
    lifted::original::c82091E58(ctx, base);
    return;
  }
  const uint32_t driver = ctx.r3.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(144, uint32_t(ctx.lr), [&](uint32_t frame) {
    fd::DrawFrame(base, driver, frame, calls);
  });
}

ITC_REGS_STEP(AppSceneUpdate) {
  const uint32_t scene = ctx.r3.u32;
  const app_states::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t) { fd::SceneUpdate(base, scene, calls); });
}

ITC_REGS_STEP(AppSceneDraw) {
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
        lifted::original::c822A8A60(ctx, base);
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

fd::ThreadCalls ThreadCallsOn(lifted::Regs& ctx, uint8_t* base) {
  fd::ThreadCalls calls;
  calls.import = [&ctx, base](fd::Import import, std::initializer_list<uint64_t> args,
                              uint32_t return_address) {
    lifted::Temp* reg = &ctx.r3;  // r3 to r10 are consecutive in the view
    for (uint64_t arg : args) {
      (reg++)->u64 = arg;
    }
    ctx.lr = return_address;
    switch (import) {
      case fd::Import::kExCreateThread:
        lifted::Sdk(ctx, base, __imp__ExCreateThread);
        break;
      case fd::Import::kObReferenceObjectByHandle:
        lifted::Sdk(ctx, base, __imp__ObReferenceObjectByHandle);
        break;
      case fd::Import::kKeSetBasePriorityThread:
        lifted::Sdk(ctx, base, __imp__KeSetBasePriorityThread);
        break;
      case fd::Import::kObDereferenceObject:
        lifted::Sdk(ctx, base, __imp__ObDereferenceObject);
        break;
      case fd::Import::kKeSetAffinityThread:
        lifted::Sdk(ctx, base, __imp__KeSetAffinityThread);
        break;
      case fd::Import::kNtResumeThread:
        lifted::Sdk(ctx, base, __imp__NtResumeThread);
        break;
    }
    return ctx.r3.u32;
  };
  calls.in_frame = [&ctx, base](uint32_t bytes, uint32_t return_address,
                                const std::function<void(uint32_t)>& body) {
    // (r14 to r31 were kept and put back around the body: nothing to keep
    // without a context)
    guest_call::InFrame(ctx, base, bytes, return_address, body);
  };
  calls.set_last_error_from_status = [&ctx, base](uint32_t status) {
    last_error::SetFromStatus(base, ctx.r13.u32, status);
  };
  return calls;
}

}  // namespace

ITC_REGS_STEP(ThreadConstruct) { fd::ThreadConstruct(base, ctx.r3.u32); }

ITC_REGS_STEP(ThreadBegin) {
  const uint32_t thread = ctx.r3.u32, object = ctx.r4.u32, processor = ctx.r5.u32,
                 priority = ctx.r6.u32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = fd::ThreadBegin(base, thread, object, processor, priority, calls);
}

ITC_REGS_STEP(CreateThreadStep) {
  const uint32_t stack = ctx.r4.u32, entry = ctx.r5.u32, parameter = ctx.r6.u32,
                 flags = ctx.r7.u32, processor = ctx.r8.u32, id = ctx.r9.u32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = fd::CreateThread(base, stack, entry, parameter, flags, processor, id, calls);
}

ITC_REGS_STEP(SetThreadPriorityStep) {
  const uint32_t handle = ctx.r3.u32;
  const int32_t priority = ctx.r4.s32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = fd::SetThreadPriority(base, handle, priority, calls);
}

ITC_REGS_STEP(XSetThreadProcessorStep) {
  const uint32_t handle = ctx.r3.u32, processor = ctx.r4.u32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = uint64_t(int64_t(int32_t(fd::XSetThreadProcessor(base, handle, processor, calls))));
}

ITC_REGS_STEP(ResumeThreadStep) {
  const uint32_t handle = ctx.r3.u32;
  const fd::ThreadCalls calls = ThreadCallsOn(ctx, base);
  ctx.r3.u64 = uint64_t(int64_t(int32_t(fd::ResumeThread(base, handle, calls))));
}

ITC_REGS_STEP(VideoOutputUnbindAll) {
  const uint32_t video_output = ctx.r3.u32;
  fd::UnbindCalls calls;
  auto setter = [&](uint32_t function, uint32_t device, uint32_t return_address) {
    ctx.r3.u64 = device;
    ctx.lr = return_address;
    lifted::CallFromContext(ctx, base, function);
  };
  calls.set_texture = [&](uint32_t device, uint32_t stage, uint64_t mask) {
    ctx.r4.u64 = stage;
    ctx.r5.u64 = 0;
    ctx.r6.u64 = mask;
    setter(0x822A6008, device, 0x8258DA30);
  };
  calls.set_stream_source = [&](uint32_t device, uint32_t stream, uint64_t mask) {
    ctx.r4.u64 = stream;
    ctx.r5.u64 = 0;
    ctx.r6.u64 = 0;
    ctx.r7.u64 = 0;
    ctx.r8.u64 = mask;
    setter(0x822A3758, device, 0x8258DA70);
  };
  calls.set_indices = [&](uint32_t device) {
    ctx.r4.u64 = 0;
    setter(0x822A3908, device, 0x8258DA88);
  };
  calls.set_pixel_shader = [&](uint32_t device) {
    ctx.r4.u64 = 0;
    setter(0x822A8618, device, 0x8258DA94);
  };
  calls.set_vertex_shader = [&](uint32_t device) {
    ctx.r4.u64 = 0;
    setter(0x822A8958, device, 0x8258DAA0);
  };
  // The registers set are the original's nonvolatile ones, which reached
  // callees only as the bytes they save: not carried by the view (nor kept
  // and put back around the frame).
  calls.set_register = [](int, uint64_t) {};
  guest_call::InFrame(ctx, base, 112, 0x8258DA30,
                      [&](uint32_t) { fd::VideoOutputUnbindAll(base, video_output, calls); });
}

namespace {

fd::SceneSetUpCalls SceneSetUpCallsOn(lifted::Regs& ctx, uint8_t* base) {
  fd::SceneSetUpCalls calls;
  auto call = [&ctx, base](uint32_t function, uint32_t return_address) {
    ctx.lr = return_address;
    lifted::CallFromContext(ctx, base, function);
    return ctx.r3.u32;
  };
  calls.vcall = [&ctx, base](uint32_t object, uint32_t byte, uint64_t r4, uint64_t r5,
                             uint32_t return_address) {
    ctx.r3.u64 = object;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    const uint32_t target = guest::LoadU32(base, guest::LoadU32(base, object) + byte);
    ctx.ctr.u64 = target;  // (r11, also the target there: none reads it)
    ctx.lr = return_address;
    lifted::CallFromContext(ctx, base, target);
    return ctx.r3.u32;
  };
  calls.render_queue_construct = [&ctx, call](uint32_t queue) {
    ctx.r3.u64 = queue;
    call(0x825915E0, 0x82098AD0);
  };
  calls.create_declaration = [&ctx, call](uint32_t elements, uint32_t return_address) {
    ctx.r3.u64 = elements;
    return call(0x822A8980, return_address);
  };
  calls.operator_new = [&ctx, call](uint32_t bytes) {
    ctx.r3.u64 = bytes;
    return call(0x825A6940, 0x8258CD88);
  };
  calls.operator_delete = [&ctx, call](uint32_t block) {
    ctx.r3.u64 = block;
    call(0x825A71C0, 0x8258CD50);
  };
  calls.alloc_physical = [&ctx, call](uint32_t bytes, uint32_t return_address) {
    ctx.r3.u64 = bytes;
    ctx.r4.u64 = 0;
    ctx.r5.u64 = 0xFFFFFFFFFFFFFFFFull;
    ctx.r6.u64 = 4;
    ctx.r7.u64 = 4;
    return call(0x8258E098, return_address);
  };
  calls.alloc_virtual = [&ctx, call](uint32_t bytes, uint32_t return_address) {
    ctx.r3.u64 = bytes;
    return call(0x8258DFF0, return_address);
  };
  calls.in_frame = [&ctx, base](uint32_t bytes, uint32_t return_address,
                                const std::function<void(uint32_t)>& body) {
    // (r14 to r31 were kept and put back around the body: nothing to keep
    // without a context)
    guest_call::InFrame(ctx, base, bytes, return_address, body);
  };
  // The registers set are the original's nonvolatile ones, which reached
  // callees only as the bytes they save: not carried by the view.
  calls.set_register = [](int, uint64_t) {};
  return calls;
}

}  // namespace

ITC_REGS_STEP(SceneConstruct) {
  if (!ITC_FLAG(native_video_output)) {
    lifted::original::c82098A98(ctx, base);
    return;
  }
  const uint32_t scene = ctx.r3.u32, block = ctx.r4.u32;
  const fd::SceneSetUpCalls calls = SceneSetUpCallsOn(ctx, base);
  calls.in_frame(192, uint32_t(ctx.lr), [&](uint32_t frame) {
    fd::SceneConstruct(base, scene, block, frame, calls);
  });
  ctx.r3.u64 = scene;
}

ITC_REGS_STEP(CommandListInitStep) {
  const uint32_t list = ctx.r3.u32, slots = ctx.r4.u32;
  const fd::SceneSetUpCalls calls = SceneSetUpCallsOn(ctx, base);
  uint32_t r3 = 0;
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t) {
    r3 = fd::CommandListInit(base, list, slots, calls);
  });
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(ColorRectSetTargetsStep) {
  fd::ColorRectSetTargets(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(DrawGlobalsInitStep) {
  const uint32_t capacity = ctx.r4.u32, count = ctx.r5.u32;
  const fd::SceneSetUpCalls calls = SceneSetUpCallsOn(ctx, base);
  fd::DrawGlobalsInit(base, capacity, count, calls);
}

ITC_REGS_STEP(PrimitiveDeclInitStep) {
  const fd::SceneSetUpCalls calls = SceneSetUpCallsOn(ctx, base);
  ctx.r3.u64 = fd::PrimitiveDeclInit(base, calls);
}

}  // namespace hooks

#define ITC_FRAME_DRIVER_ADAPTER(address, step) \
  ITC_NATIVE_ENTRY_ALWAYS(address) {              \
    \
    if (!hooks::NativeFrameDriver()) {          \
      lifted::original::c##address(ctx, base); \
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
ITC_FRAME_DRIVER_ADAPTER(8258D9F8, VideoOutputUnbindAll)
ITC_FRAME_DRIVER_ADAPTER(82098A98, SceneConstruct)
