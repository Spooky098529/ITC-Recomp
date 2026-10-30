// The app's start-up natively: adapters in front of src/app_start_up/
// (docs/functions/app-start-up.md).

#include "hooks/app_start_up.h"

#include "app_start_up/start_up.h"
#include "base/flags.h"
#include "guest.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "strings_and_font/strings_and_font.h"

#include <cstddef>

using guest_call::InFrame;

ITC_FLAG_BOOL(native_app_start_up, true, "ITC",
              "Run the app's start-up (docs/functions/app-start-up.md: ReadError::SetApp "
              "sub_8209AC50, sub_8209AC30, sub_8209F580, sub_8225FB70, Movies::Set "
              "sub_820A4360 (these five without adapters: App::StartUp's plain C++ calls them) and, with --native_heap, CreateVertexDeclaration sub_822A8980; the "
              "network manager's set-up Network::SetUp sub_8214E190 with SignIn::SetUp "
              "sub_8214F1C0, Network::StartUp sub_82148868, XNetStartup sub_82575B08 (its "
              "WSAStartup entry sub_82459C10 merged) and XNet::Configure sub_82577560; with "
              "--native_video_output, --native_graphics_resources and --native_heap, the "
              "scene target SceneTarget::Create sub_8209B378, SceneTarget::SceneTarget "
              "sub_8209B070 and Tiling::LargestTile sub_8209ADD8, and Billboards::Init "
              "sub_8209BE88; with them, Text::Init sub_8210ADD0 with Font::Init sub_82109A00, "
              "Font::Font sub_82109228 and Font::CreateShaders sub_821088B8, no adapters of their "
              "own; and App::StartUp sub_82106548, its callees through their adapters; with "
              "--native_heap, Movie::Movie sub_820AEA88 and MovieWrapper::MovieWrapper "
              "sub_82593DC0, the wrapper's critical section a host mutex for every user, "
              "through ScopedLock sub_8258E580 and sub_8258E5B8 for that section) "
              "natively instead of through the translated code (the same reads and writes; "
              "checked by --run_harnesses=app_start_up)");

namespace asu = app_start_up;

hooks::StartUpRecorder hooks::g_start_up_recorder = nullptr;

// ReadError::SetApp (sub_8209AC50), sub_8209AC30, sub_8209F580,
// sub_8225FB70 and Movies::Set (sub_820A4360) have no adapters: only
// App::StartUp's plain C++ calls them in play (Movies::Set's original where
// its plain C++ hands back).

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
      // r9 as the original's memcpy leaves it in play (0): Font::CreateShaders
      // hands it to the shader compile as its flags.
      ctx.r9.u64 = 0;
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
  if (hooks::StartUpRecorded(ctx, 0x8214E190)) {
    return;
  }
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
  if (hooks::StartUpRecorded(ctx, 0x8209B378)) {
    return;
  }
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
  if (hooks::StartUpRecorded(ctx, 0x8209BE88)) {
    return;
  }
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

// --- The text's set-up ---

ITC_FLAG_DECLARE(bool, native_strings_and_font);

bool hooks::TextInitNative() { return DeviceUsersNative(); }

namespace {

// Runs `call` with r1 at `frame` (the frame the original makes the call
// from), then puts r1 back.
template <typename Call>
uint32_t At(PPCContext& ctx, uint32_t frame, Call call) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame;
  const uint32_t r3 = call();
  ctx.r1.u64 = stack;
  return r3;
}

// The font's guest calls: Font::Font's from `font_frame`, Font::CreateShaders'
// from `shaders_frame`, Strings::Init from `text_frame` (Text::Init's).
asu::FontCalls FontCallsFor(PPCContext& ctx, uint8_t* base, uint32_t text_frame,
                            uint32_t font_frame, uint32_t shaders_frame) {
  asu::FontCalls calls;
  calls.video_output = [&ctx, base, font_frame] {
    return At(ctx, font_frame, [&] { return VideoOutput(ctx, base, 0x82109258, 0x82109268); });
  };
  calls.render_object = [&ctx, base, font_frame](int which) {
    return At(ctx, font_frame, [&] {
      const uint32_t object = VirtualCall(ctx, base, guest::LoadU32(base, asu::kRenderApp), 32,
                                          which == 0 ? 0x82109284 : 0x821092AC);
      return VirtualCall(ctx, base, object, which == 0 ? 4 : 8,
                         which == 0 ? 0x82109294 : 0x821092BC);
    });
  };
  calls.compile = [&ctx, base, shaders_frame](int which, uint32_t source, uint32_t length,
                                               uint32_t profile, uint32_t out) {
    return At(ctx, shaders_frame, [&] {
      ctx.r3.u64 = source;
      ctx.r4.u64 = length;
      ctx.r5.u64 = 0;
      ctx.r6.u64 = 0;
      ctx.r7.u64 = 0x82001BCC;  // "main"
      ctx.r8.u64 = profile;
      // The pixel shaders' r9 is set to 0; the vertex shader's is what
      // CreateVertexDeclaration leaves (memcpy's last), 0 in play (every
      // compile's r9 logged 0 with --native_app_start_up off, itc_2597).
      ctx.r9.u64 = 0;
      ctx.r10.u64 = out;
      ctx.lr = which == 0 ? 0x8210893C : 0x821089B8;
      sub_822B8F10(ctx, base);  // D3DXCompileShader (the stored shaders)
      return ctx.r3.u32;
    });
  };
  calls.buffer_data = [&ctx, base, shaders_frame](int which, uint32_t buffer) {
    return At(ctx, shaders_frame, [&] {
      return VirtualCall(ctx, base, buffer, 12, which == 0 ? 0x82108950 : 0x821089CC);
    });
  };
  calls.release = [&ctx, base, shaders_frame](int which, uint32_t buffer) {
    return At(ctx, shaders_frame, [&] {
      return VirtualCall(ctx, base, buffer, 8, which == 0 ? 0x82108970 : 0x821089EC);
    });
  };
  calls.vertex_shader = [&ctx, base, shaders_frame](uint32_t data) {
    return At(ctx, shaders_frame, [&] {
      ctx.r3.u64 = data;
      ctx.lr = 0x82108954;
      sub_822AA018(ctx, base);  // VertexShader::Create
      return ctx.r3.u32;
    });
  };
  calls.pixel_shader = [&ctx, base, shaders_frame](int, uint32_t data) {
    return At(ctx, shaders_frame, [&] {
      ctx.r3.u64 = data;
      ctx.lr = 0x821089D0;
      sub_822A9EE8(ctx, base);  // PixelShader::Create
      return ctx.r3.u32;
    });
  };
  calls.memcpy_call = [&ctx, base, shaders_frame](uint32_t to, uint32_t from, uint32_t size) {
    At(ctx, shaders_frame, [&] {
      InFrame(ctx, base, 160, 0x822A8A54, [&](uint32_t) {
        ctx.r3.u64 = to;
        ctx.r4.u64 = from;
        ctx.r5.u64 = size;
        sub_825A7410(ctx, base);  // memcpy
      });
      return 0u;
    });
  };
  calls.strings_init = [&ctx, base, text_frame] {
    // Font::Init's r3 (Font::Font's, 14), which Strings::Init leaves when
    // the table is already there.
    ctx.r3.u64 = 14;
    if (ITC_FLAG(native_strings_and_font) && hooks::NativeHeapActive()) {
      if (const uint32_t table = strings_and_font::StringsInit(base); table != 0) {
        ctx.r3.u64 = table;
      }
      return ctx.r3.u32;
    }
    return At(ctx, text_frame, [&] {
      ctx.lr = 0x8210ADE4;
      sub_8210A860(ctx, base);  // Strings::Init
      return ctx.r3.u32;
    });
  };
  return calls;
}

}  // namespace

void hooks::TextInitSteps(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  InFrame(ctx, base, 96, 0x8210ADE0, [&](uint32_t text_frame) {
    InFrame(ctx, base, 96, 0x82109A2C, [&](uint32_t init_frame) {
      const uint32_t font_frame = init_frame - 128;
      const uint32_t shaders_frame = font_frame - 160;
      guest::StoreU32(base, font_frame, init_frame);
      guest::StoreU32(base, shaders_frame, font_frame);
      r3 = asu::TextInit(base, font_frame, shaders_frame,
                         FontCallsFor(ctx, base, text_frame, font_frame, shaders_frame));
    });
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void hooks::FontConstructSteps(PPCContext& ctx, uint8_t* base) {
  const uint32_t font = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  InFrame(ctx, base, 128, 0x82109258, [&](uint32_t font_frame) {
    const uint32_t shaders_frame = font_frame - 160;
    guest::StoreU32(base, shaders_frame, font_frame);
    asu::FontConstruct(base, font, font_frame, shaders_frame,
                       FontCallsFor(ctx, base, font_frame, font_frame, shaders_frame));
  });
  ctx.lr = lr;
  ctx.r3.u64 = 14;
}

// Text::Init: no arguments (Font::Init, Font::Font and Font::CreateShaders
// have no adapters: only its plain C++ calls them in play).
extern "C" REX_FUNC(sub_8210ADD0) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x8210ADD0)) {
    return;
  }
  if (!DeviceUsersNative()) {
    __imp__sub_8210ADD0(ctx, base);
    return;
  }
  hooks::TextInitSteps(ctx, base);
}

// --- The app's start-up ---

void hooks::AppStartUpSteps(PPCContext& ctx, uint8_t* base) {
  const uint32_t interface = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  const uint64_t r31 = ctx.r31.u64;
  // The original's prologue: the link register and r31 kept above its frame.
  guest::StoreU32(base, ctx.r1.u32 - 8, uint32_t(lr));
  guest::StoreU32(base, ctx.r1.u32 - 16, uint32_t(r31 >> 32));
  guest::StoreU32(base, ctx.r1.u32 - 12, uint32_t(r31));
  uint32_t r3 = 0;
  InFrame(ctx, base, 128, 0x82106564, [&](uint32_t frame) {
    ctx.r31.u64 = interface;
    asu::StartUpCalls calls;
    calls.call = [&](uint32_t target, std::optional<uint32_t> a, std::optional<uint32_t> b,
                     uint32_t return_address) {
      if (a) {
        ctx.r3.u64 = *a;
      }
      if (b) {
        ctx.r4.u64 = *b;
      }
      ctx.lr = return_address;
      REX_CALL_INDIRECT_FUNC(target);
      return ctx.r3.u32;
    };
    calls.virtual_call = [&](uint32_t offset, uint32_t return_address) {
      return VirtualCall(ctx, base, interface, offset, return_address);
    };
    r3 = asu::AppStartUp(base, interface, frame, calls);
  });
  ctx.r31.u64 = r31;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// App::StartUp: r3 = app + 16, from App::App once at boot.
extern "C" REX_FUNC(sub_82106548) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up)) {
    __imp__sub_82106548(ctx, base);
    return;
  }
  hooks::AppStartUpSteps(ctx, base);
}

// --- The movie class's construction ---

namespace {

// The original's __savegprlr_N: the link register at r1 - 8 and rN to r31
// (doublewords) below it, above the frame it's about to make.
void SaveRegisters(PPCContext& ctx, uint8_t* base, int first) {  // first >= 14
  // r14 to r31 are consecutive in PPCContext (r3 comes first there).
  static_assert(offsetof(PPCContext, r31) - offsetof(PPCContext, r14) == 17 * sizeof(PPCRegister));
  const uint32_t sp = ctx.r1.u32;
  guest::StoreU32(base, sp - 8, uint32_t(ctx.lr));
  const PPCRegister* regs = &ctx.r14;
  for (int n = first; n < 32; n++) {
    const uint64_t value = regs[n - 14].u64;
    const uint32_t at = sp - 8 * (32 - n) - 8;
    guest::StoreU32(base, at, uint32_t(value >> 32));
    guest::StoreU32(base, at + 4, uint32_t(value));
  }
}

}  // namespace

void hooks::MovieWrapperSteps(PPCContext& ctx, uint8_t* base) {
  const uint32_t wrapper = ctx.r3.u32;
  // The original's prologue: lr, r30 and r31 above its 112-byte frame.
  SaveRegisters(ctx, base, 30);
  if (const auto r3 = asu::MovieWrapperConstruct(base, wrapper)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_82593DC0(ctx, base);
}

// MovieWrapper::MovieWrapper: r3 = the 8 bytes, from Movie::Movie.
extern "C" REX_FUNC(sub_82593DC0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up) || !hooks::NativeHeapActive()) {
    __imp__sub_82593DC0(ctx, base);
    return;
  }
  hooks::MovieWrapperSteps(ctx, base);
}

void hooks::MovieConstructSteps(PPCContext& ctx, uint8_t* base) {
  const uint32_t movie = ctx.r3.u32;
  const uint32_t block = ctx.r4.u32;
  const uint64_t lr = ctx.lr;
  // The original's prologue (__savegprlr_28): lr and r28 to r31.
  SaveRegisters(ctx, base, 28);
  std::optional<uint32_t> result;
  InFrame(ctx, base, 160, 0x820AEB10, [&](uint32_t frame) {
    asu::MovieCalls calls;
    calls.virtual_call = [&](uint32_t object, uint32_t offset, uint32_t r4, uint32_t r5,
                             uint32_t return_address) {
      ctx.r4.u64 = r4;
      ctx.r5.u64 = r5;
      return VirtualCall(ctx, base, object, offset, return_address);
    };
    calls.wrapper = [&](uint32_t wrapper) {
      ctx.r3.u64 = wrapper;
      ctx.lr = 0x820AEB1C;
      hooks::MovieWrapperSteps(ctx, base);
      return ctx.r3.u32;
    };
    result = asu::MovieConstruct(base, movie, block, frame, calls);
  });
  ctx.lr = lr;
  if (!result) {
    ctx.r3.u64 = movie;
    ctx.r4.u64 = block;
    __imp__sub_820AEA88(ctx, base);
    return;
  }
  ctx.r3.u64 = *result;
}

// Movie::Movie: r3 = the movie (312 bytes), r4 = the set-up block, from
// App::App once at boot.
extern "C" REX_FUNC(sub_820AEA88) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_app_start_up) || !hooks::NativeHeapActive()) {
    __imp__sub_820AEA88(ctx, base);
    return;
  }
  hooks::MovieConstructSteps(ctx, base);
}
