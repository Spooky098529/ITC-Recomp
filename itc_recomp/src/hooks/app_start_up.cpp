// The app's start-up natively: adapters in front of src/app_start_up/
// (docs/functions/app-start-up.md).

#include "app_start_up/start_up.h"

#include "base/flags.h"
#include "guest.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"

using guest_call::InFrame;

ITC_FLAG_BOOL(native_app_start_up, true, "ITC",
              "Run the app's start-up (docs/functions/app-start-up.md: ReadError::SetApp "
              "sub_8209AC50, sub_8209AC30, sub_8209F580, sub_8225FB70, Movies::Set "
              "sub_820A4360 and, with --native_heap, CreateVertexDeclaration sub_822A8980; the "
              "network manager's set-up Network::SetUp sub_8214E190 with SignIn::SetUp "
              "sub_8214F1C0, Network::StartUp sub_82148868, XNetStartup sub_82575B08 (its "
              "WSAStartup entry sub_82459C10 merged) and XNet::Configure sub_82577560; with "
              "--native_video_output, --native_graphics_resources and --native_heap, the "
              "scene target SceneTarget::Create sub_8209B378, SceneTarget::SceneTarget "
              "sub_8209B070 and Tiling::LargestTile sub_8209ADD8, and Billboards::Init "
              "sub_8209BE88) "
              "natively instead of through the translated code (the same reads and writes; "
              "checked by --run_harnesses=app_start_up)");

namespace asu = app_start_up;

extern "C" REX_FUNC(sub_8209AC50) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8209AC50(ctx, base);
    return;
  }
  asu::ReadErrorSetApp(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_8209AC30) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8209AC30(ctx, base);
    return;
  }
  asu::SetAppInterface84(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_8209F580) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8209F580(ctx, base);
    return;
  }
  asu::SetRenderApp(base, ctx.r3.u32);
}

extern "C" REX_FUNC(sub_8225FB70) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8225FB70(ctx, base);
    return;
  }
  asu::ClearWords8110(base);
}

extern "C" REX_FUNC(sub_820A4360) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_app_start_up)) {
    if (const auto movie = asu::MoviesSet(base, ctx.r3.u32)) {
      ctx.r3.u64 = *movie;
      return;
    }
  }
  __imp__sub_820A4360(ctx, base);
}

extern "C" REX_FUNC(sub_822A8980) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_app_start_up) && hooks::NativeHeapActive()) {
    const auto declaration =
        asu::CreateVertexDeclaration(base, ctx.r3.u32, [&](uint32_t to, uint32_t from, uint32_t size) {
          InFrame(ctx, base, 160, 0x822A8A54, [&](uint32_t) {
            ctx.r3.u64 = to;
            ctx.r4.u64 = from;
            ctx.r5.u64 = size;
            sub_825A7410(ctx, base);  // memcpy
          });
        });
    if (declaration) {
      ctx.r3.u64 = *declaration;
      return;
    }
  }
  __imp__sub_822A8980(ctx, base);
}

// --- The network manager's set-up ---

namespace {

// memcpy through the guest from XNet::Configure's frame. (SignIn::SetUp,
// Network::StartUp, XNetStartup and XNet::Configure have no adapters: only
// Network::SetUp's plain C++ calls them in play.)
asu::MemcpyCall XNetMemcpy(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t to, uint32_t from, uint32_t size) {
    InFrame(ctx, base, 112, 0x82577580, [&](uint32_t) {
      ctx.r3.u64 = to;
      ctx.r4.u64 = from;
      ctx.r5.u64 = size;
      sub_825A7410(ctx, base);  // memcpy
    });
  };
}

}  // namespace

extern "C" REX_FUNC(sub_8214E190) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_8214E190(ctx, base);
    return;
  }
  ctx.r3.u64 = asu::NetworkSetUp(base, ctx.r3.u32, ctx.r1.u32 - 144, XNetMemcpy(ctx, base));
}

// --- The scene target ---

ITC_FLAG_DECLARE(bool, native_video_output);
ITC_FLAG_DECLARE(bool, native_graphics_resources);

namespace {

// [0x8266D088]'s slot 7, then that object's slot 3: the video output, with
// the original's return addresses.
uint32_t VideoOutput(PPCContext& ctx, uint8_t* base, uint32_t first, uint32_t second) {
  ctx.r3.u64 = guest::LoadU32(base, asu::kSceneTargetApp);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 28);
  ctx.lr = first;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 12);
  ctx.lr = second;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  return ctx.r3.u32;
}

// Whether the device's native users can run: the device lock, the surface
// and texture makers native.
bool DeviceUsersNative() {
  return ITC_FLAG(native_app_start_up) && ITC_FLAG(native_video_output) &&
         ITC_FLAG(native_graphics_resources) && hooks::NativeHeapActive();
}

// (SceneTarget::SceneTarget has no adapter: only SceneTarget::Create's plain
// C++ calls it in play.)
asu::VideoOutputCall SceneTargetVideoOutput(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](int at) {
    return at == 0 ? VideoOutput(ctx, base, 0x8209B0B0, 0x8209B0C0)
                   : VideoOutput(ctx, base, 0x8209B298, 0x8209B2A8);
  };
}

}  // namespace

extern "C" REX_FUNC(sub_8209B378) {
  REX_FUNC_PROLOGUE();
  if (!DeviceUsersNative()) {
    __imp__sub_8209B378(ctx, base);
    return;
  }
  const uint32_t interface = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> result;
  InFrame(ctx, base, 96, 0x8209B3A4, [&](uint32_t) {
    InFrame(ctx, base, 192, 0x8209B0B0, [&](uint32_t frame) {
      result = asu::SceneTargetCreate(base, interface, frame, SceneTargetVideoOutput(ctx, base));
    });
  });
  ctx.lr = lr;
  if (!result) {
    __imp__sub_8209B378(ctx, base);
    return;
  }
  ctx.r3.u64 = *result;
}

// --- The light billboards' shaders ---

namespace {

uint32_t VirtualCall(PPCContext& ctx, uint8_t* base, uint32_t object, uint32_t offset,
                     uint32_t return_address) {
  ctx.r3.u64 = object;
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, object) + offset);
  ctx.lr = return_address;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  return ctx.r3.u32;
}

}  // namespace

extern "C" REX_FUNC(sub_8209BE88) {
  REX_FUNC_PROLOGUE();
  if (!DeviceUsersNative()) {
    __imp__sub_8209BE88(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  InFrame(ctx, base, 160, 0x8209BF18, [&](uint32_t frame) {
    asu::BillboardCalls calls;
    calls.compile = [&](int which, uint32_t source, uint32_t length, uint32_t entry,
                        uint32_t profile, uint32_t out) {
      ctx.r3.u64 = source;
      ctx.r4.u64 = length;
      ctx.r5.u64 = 0;
      ctx.r6.u64 = 0;
      ctx.r7.u64 = entry;
      ctx.r8.u64 = profile;
      ctx.r9.u64 = 0;
      ctx.r10.u64 = out;
      ctx.lr = which == 0 ? 0x8209BF18 : 0x8209C01C;
      sub_822B8F10(ctx, base);  // D3DXCompileShader (the stored shaders)
      return ctx.r3.u32;
    };
    calls.buffer_data = [&](int which, uint32_t buffer) {
      return VirtualCall(ctx, base, buffer, 12, which == 0 ? 0x8209BF80 : 0x8209C080);
    };
    calls.release = [&](int which, uint32_t buffer) {
      static constexpr uint32_t kReturns[] = {0x8209BF3C, 0x8209BFCC, 0x8209C040, 0x8209C0CC};
      VirtualCall(ctx, base, buffer, 8, kReturns[which]);
    };
    calls.video_output = [&](int at) {
      static constexpr uint32_t kReturns[6][2] = {
          {0x8209BF54, 0x8209BF64}, {0x8209BFA0, 0x8209BFB0}, {0x8209C054, 0x8209C064},
          {0x8209C0A0, 0x8209C0B0}, {0x8209C0E0, 0x8209C0F0}, {0x8209C124, 0x8209C134}};
      return VideoOutput(ctx, base, kReturns[at][0], kReturns[at][1]);
    };
    calls.vertex_shader = [&](uint32_t data) {
      ctx.r3.u64 = data;
      ctx.lr = 0x8209BF84;
      sub_822AA018(ctx, base);  // VertexShader::Create
      return ctx.r3.u32;
    };
    calls.pixel_shader = [&](uint32_t data) {
      ctx.r3.u64 = data;
      ctx.lr = 0x8209C084;
      sub_822A9EE8(ctx, base);  // PixelShader::Create
      return ctx.r3.u32;
    };
    calls.memcpy_call = [&](uint32_t to, uint32_t from, uint32_t size) {
      InFrame(ctx, base, 160, 0x822A8A54, [&](uint32_t) {
        ctx.r3.u64 = to;
        ctx.r4.u64 = from;
        ctx.r5.u64 = size;
        sub_825A7410(ctx, base);  // memcpy
      });
    };
    asu::BillboardsInit(base, frame, calls);
  });
  ctx.lr = lr;
}
