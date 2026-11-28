// The app's start-up natively: adapters in front of src/app_start_up/
// (docs/functions/app-start-up.md).

#include "hooks/app_start_up.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"
#include "hooks/direct_slots.h"

#include "app_start_up/start_up.h"
#include "base/flags.h"
#include "guest.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "schema/app_start_up.h"
#include "strings_and_font/strings_and_font.h"

#include <cstddef>
#include <cstring>
#include "lifted/sdk.h"

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
              "sub_82593DC0 (no adapter: only Movie::Movie's plain C++ calls it), the wrapper's critical section a host mutex for every user, "
              "through ScopedLock sub_8258E580 and sub_8258E5B8 for that section; AudioManager::Create "
              "sub_82119B38 with AudioManager::AudioManager sub_821197E0 and "
              "AudioEngine::StartXAudio sub_8259CD50, no adapters of their own; the second camera "
              "class sub_820A8B88 with its camera sub_820A9E28, list sub_820A98C0 (sub_820A9198 "
              "and sub_820A8F80 merged) and entry sub_820A75E8, no adapters of their own; App::App "
              "sub_82094590, its callees through their adapters; App::Create sub_82094AB8; the app's "
              "interface slots sub_82379338, sub_82379350 (the getters of +108 and +112), "
              "sub_82094368 and sub_820944A8 (the framework's first worker pool, tail calls); the "
              "scene's render manager's slot 8 sub_820944D8 with sub_82094338 and the frame "
              "driver's slots sub_82094498 and sub_82094480 with sub_82094320 (tail calls through the "
              "framework's +36 interface)) "
              "natively instead of through the translated code (the same reads and writes; "
              "checked by --run_harnesses=app_start_up)");

namespace asu = app_start_up;

hooks::StartUpRecorder hooks::g_start_up_recorder = nullptr;

bool hooks::StartUpRecordedOnView(lifted::Regs& ctx, uint32_t address) {
  return hooks::g_start_up_recorder(ctx, address);
}

// ReadError::SetApp (sub_8209AC50), sub_8209AC30, sub_8209F580,
// sub_8225FB70 and Movies::Set (sub_820A4360) have no adapters: only
// App::StartUp's plain C++ calls them in play (Movies::Set's original where
// its plain C++ hands back).

ITC_NATIVE_ENTRY_ALWAYS(822A8980) {
  
  if (ITC_FLAG(native_app_start_up) && hooks::NativeHeapActive()) {
    const auto declaration =
        asu::CreateVertexDeclaration(ctx.r3.u32, [&](uint32_t to, uint32_t from, uint32_t size) {
          InFrame(ctx, 160, 0x822A8A54, [&](uint32_t) {
            ctx.r3.u64 = to;
            ctx.r4.u64 = from;
            ctx.r5.u64 = size;
            lifted::Enter(ctx, game::memcpy_825A7410);  // memcpy
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
  lifted::original::c822A8980(ctx);
}

// --- The network manager's set-up ---

namespace {

// memcpy through the guest from XNet::Configure's frame. (SignIn::SetUp,
// Network::StartUp, XNetStartup and XNet::Configure have no adapters: only
// Network::SetUp's plain C++ calls them in play.)
asu::MemcpyCall XNetMemcpy(lifted::Regs& ctx) {
  return [&ctx](uint32_t to, uint32_t from, uint32_t size) {
    InFrame(ctx, 112, 0x82577580, [&](uint32_t) {
      ctx.r3.u64 = to;
      ctx.r4.u64 = from;
      ctx.r5.u64 = size;
      lifted::Enter(ctx, game::memcpy_825A7410);  // memcpy
    });
  };
}

}  // namespace

ITC_NATIVE_ENTRY_ALWAYS(8214E190) {
  
  if (hooks::StartUpRecorded(ctx, 0x8214E190)) {
    return;
  }
  if (!ITC_FLAG(native_app_start_up)) {
    lifted::original::c8214E190(ctx);
    return;
  }
  ctx.r3.u64 = asu::NetworkSetUp(ctx.r3.u32, ctx.r1.u32 - 144, XNetMemcpy(ctx));
}

// --- The scene target ---

ITC_FLAG_DECLARE(bool, native_video_output);
ITC_FLAG_DECLARE(bool, native_graphics_resources);

namespace {

// [0x8266D088]'s slot 7, then that object's slot 3: the video output, with
// the original's return addresses.
uint32_t VideoOutput(lifted::Regs& ctx, uint32_t first, uint32_t second) {
  ctx.r3.u64 = schema::AppStartUpGlobals::scene_target_app().address;
  ctx.lr = first;
  // Slot 7's plain C++ where it's the game's (hooks::DirectSlotCall).
  const uint32_t slot7 =
      guest::LoadU32(schema::AppInterface16(ctx.r3.u32).vtable() + 28);
  if (!hooks::DirectSlotCall(ctx, slot7)) {
    lifted::CallFromContext(ctx, slot7);
  }
  ctx.ctr.u64 = guest::LoadU32(guest::VtableWord(ctx.r3.u32) + 12);
  ctx.lr = second;
  lifted::CallFromContext(ctx, ctx.ctr.u32);
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
asu::VideoOutputCall SceneTargetVideoOutput(lifted::Regs& ctx) {
  return [&ctx](int at) {
    return at == 0 ? VideoOutput(ctx, 0x8209B0B0, 0x8209B0C0)
                   : VideoOutput(ctx, 0x8209B298, 0x8209B2A8);
  };
}

}  // namespace

ITC_NATIVE_ENTRY_ALWAYS(8209B378) {
  
  if (hooks::StartUpRecorded(ctx, 0x8209B378)) {
    return;
  }
  if (!DeviceUsersNative()) {
    lifted::original::c8209B378(ctx);
    return;
  }
  const uint32_t interface = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> result;
  // SceneTarget::Create's 96-byte frame (its back chain not written since E5
  // step 6: no guest call is made from it), the constructor's 192 below.
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = stack - 96;
  InFrame(ctx, 192, 0x8209B0B0, [&](uint32_t frame) {
    result = asu::SceneTargetCreate(interface, frame, SceneTargetVideoOutput(ctx));
  });
  ctx.r1.u64 = stack;
  ctx.lr = lr;
  if (!result) {
    lifted::original::c8209B378(ctx);
    return;
  }
  ctx.r3.u64 = *result;
}

// --- The light billboards' shaders ---

namespace {

uint32_t VirtualCall(lifted::Regs& ctx, uint32_t object, uint32_t offset,
                     uint32_t return_address) {
  ctx.r3.u64 = object;
  ctx.ctr.u64 = guest::LoadU32(guest::VtableWord(object) + offset);
  ctx.lr = return_address;
  // (The render manager's slots 8 and 7 by their plain C++ where they're the
  // game's, hooks::DirectSlotCall.)
  if (!hooks::DirectSlotCall(ctx, ctx.ctr.u32)) {
    lifted::CallFromContext(ctx, ctx.ctr.u32);
  }
  return ctx.r3.u32;
}

}  // namespace

ITC_NATIVE_ENTRY_ALWAYS(8209BE88) {
  
  if (hooks::StartUpRecorded(ctx, 0x8209BE88)) {
    return;
  }
  if (!DeviceUsersNative()) {
    lifted::original::c8209BE88(ctx);
    return;
  }
  const uint64_t lr = ctx.lr;
  InFrame(ctx, 160, 0x8209BF18, [&](uint32_t frame) {
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
      lifted::Enter(ctx, game::D3DXCompileShader);  // D3DXCompileShader (the stored shaders)
      return ctx.r3.u32;
    };
    calls.buffer_data = [&](int which, uint32_t buffer) {
      return VirtualCall(ctx, buffer, 12, which == 0 ? 0x8209BF80 : 0x8209C080);
    };
    calls.release = [&](int which, uint32_t buffer) {
      static constexpr uint32_t kReturns[] = {0x8209BF3C, 0x8209BFCC, 0x8209C040, 0x8209C0CC};
      VirtualCall(ctx, buffer, 8, kReturns[which]);
    };
    calls.video_output = [&](int at) {
      static constexpr uint32_t kReturns[6][2] = {
          {0x8209BF54, 0x8209BF64}, {0x8209BFA0, 0x8209BFB0}, {0x8209C054, 0x8209C064},
          {0x8209C0A0, 0x8209C0B0}, {0x8209C0E0, 0x8209C0F0}, {0x8209C124, 0x8209C134}};
      return VideoOutput(ctx, kReturns[at][0], kReturns[at][1]);
    };
    calls.vertex_shader = [&](uint32_t data) {
      ctx.r3.u64 = data;
      ctx.lr = 0x8209BF84;
      lifted::Enter(ctx, game::VertexShader::Create);  // VertexShader::Create
      return ctx.r3.u32;
    };
    calls.pixel_shader = [&](uint32_t data) {
      ctx.r3.u64 = data;
      ctx.lr = 0x8209C084;
      lifted::Enter(ctx, game::PixelShader::Create);  // PixelShader::Create
      return ctx.r3.u32;
    };
    calls.memcpy_call = [&](uint32_t to, uint32_t from, uint32_t size) {
      InFrame(ctx, 160, 0x822A8A54, [&](uint32_t) {
        ctx.r3.u64 = to;
        ctx.r4.u64 = from;
        ctx.r5.u64 = size;
        lifted::Enter(ctx, game::memcpy_825A7410);  // memcpy
      });
    };
    asu::BillboardsInit(frame, calls);
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
uint32_t At(lifted::Regs& ctx, uint32_t frame, Call call) {
  const uint64_t stack = ctx.r1.u64;
  ctx.r1.u64 = frame;
  const uint32_t r3 = call();
  ctx.r1.u64 = stack;
  return r3;
}

// The font's guest calls: Font::Font's from `font_frame`, Font::CreateShaders'
// from `shaders_frame`, Strings::Init from `text_frame` (Text::Init's).
asu::FontCalls FontCallsFor(lifted::Regs& ctx, uint32_t text_frame,
                            uint32_t font_frame, uint32_t shaders_frame) {
  asu::FontCalls calls;
  calls.video_output = [&ctx, font_frame] {
    return At(ctx, font_frame, [&] { return VideoOutput(ctx, 0x82109258, 0x82109268); });
  };
  calls.render_object = [&ctx, font_frame](int which) {
    return At(ctx, font_frame, [&] {
      const uint32_t object =
          VirtualCall(ctx, schema::AppStartUpGlobals::render_app().address, 32,
                      which == 0 ? 0x82109284 : 0x821092AC);
      return VirtualCall(ctx, object, which == 0 ? 4 : 8,
                         which == 0 ? 0x82109294 : 0x821092BC);
    });
  };
  calls.compile = [&ctx, shaders_frame](int which, uint32_t source, uint32_t length,
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
      lifted::Enter(ctx, game::D3DXCompileShader);  // D3DXCompileShader (the stored shaders)
      return ctx.r3.u32;
    });
  };
  calls.buffer_data = [&ctx, shaders_frame](int which, uint32_t buffer) {
    return At(ctx, shaders_frame, [&] {
      return VirtualCall(ctx, buffer, 12, which == 0 ? 0x82108950 : 0x821089CC);
    });
  };
  calls.release = [&ctx, shaders_frame](int which, uint32_t buffer) {
    return At(ctx, shaders_frame, [&] {
      return VirtualCall(ctx, buffer, 8, which == 0 ? 0x82108970 : 0x821089EC);
    });
  };
  calls.vertex_shader = [&ctx, shaders_frame](uint32_t data) {
    return At(ctx, shaders_frame, [&] {
      ctx.r3.u64 = data;
      ctx.lr = 0x82108954;
      lifted::Enter(ctx, game::VertexShader::Create);  // VertexShader::Create
      return ctx.r3.u32;
    });
  };
  calls.pixel_shader = [&ctx, shaders_frame](int, uint32_t data) {
    return At(ctx, shaders_frame, [&] {
      ctx.r3.u64 = data;
      ctx.lr = 0x821089D0;
      lifted::Enter(ctx, game::PixelShader::Create);  // PixelShader::Create
      return ctx.r3.u32;
    });
  };
  calls.memcpy_call = [&ctx, shaders_frame](uint32_t to, uint32_t from, uint32_t size) {
    At(ctx, shaders_frame, [&] {
      InFrame(ctx, 160, 0x822A8A54, [&](uint32_t) {
        ctx.r3.u64 = to;
        ctx.r4.u64 = from;
        ctx.r5.u64 = size;
        lifted::Enter(ctx, game::memcpy_825A7410);  // memcpy
      });
      return 0u;
    });
  };
  calls.strings_init = [&ctx, text_frame] {
    // Font::Init's r3 (Font::Font's, 14), which Strings::Init leaves when
    // the table is already there.
    ctx.r3.u64 = 14;
    if (ITC_FLAG(native_strings_and_font) && hooks::NativeHeapActive()) {
      if (const uint32_t table = strings_and_font::StringsInit(); table != 0) {
        ctx.r3.u64 = table;
      }
      return ctx.r3.u32;
    }
    return At(ctx, text_frame, [&] {
      ctx.lr = 0x8210ADE4;
      lifted::CallFromContext(ctx, 0x8210A860);  // Strings::Init
      return ctx.r3.u32;
    });
  };
  return calls;
}

}  // namespace

void hooks::TextInitSteps(lifted::Regs& ctx) {
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  InFrame(ctx, 96, 0x8210ADE0, [&](uint32_t text_frame) {
    // Font::Init's 96-byte frame (its back chain not written since E5 step
    // 6: no guest call is made from it), Font::Font's and CreateShaders'
    // below it.
    const uint32_t init_frame = text_frame - 96;
    const uint32_t font_frame = init_frame - 128;
    const uint32_t shaders_frame = font_frame - 160;
    guest::StoreU32(font_frame, init_frame);
    guest::StoreU32(shaders_frame, font_frame);
    r3 = asu::TextInit(font_frame, shaders_frame,
                       FontCallsFor(ctx, text_frame, font_frame, shaders_frame));
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void hooks::FontConstructSteps(lifted::Regs& ctx) {
  const uint32_t font = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  InFrame(ctx, 128, 0x82109258, [&](uint32_t font_frame) {
    const uint32_t shaders_frame = font_frame - 160;
    guest::StoreU32(shaders_frame, font_frame);
    asu::FontConstruct(font, font_frame, shaders_frame,
                       FontCallsFor(ctx, font_frame, font_frame, shaders_frame));
  });
  ctx.lr = lr;
  ctx.r3.u64 = 14;
}

// Text::Init: no arguments (Font::Init, Font::Font and Font::CreateShaders
// have no adapters: only its plain C++ calls them in play).
ITC_NATIVE_ENTRY_ALWAYS(8210ADD0) {
  
  if (hooks::StartUpRecorded(ctx, 0x8210ADD0)) {
    return;
  }
  if (!DeviceUsersNative()) {
    lifted::original::c8210ADD0(ctx);
    return;
  }
  hooks::TextInitSteps(ctx);
}

// --- The app's start-up ---

void hooks::AppStartUpSteps(lifted::Regs& ctx) {
  const uint32_t interface = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  // (The original's prologue saves, the link register and r31 above its
  // frame, aren't written since E5 step 6: nothing reads them.)
  uint32_t r3 = 0;
  // (r31 = the interface in the body, as the original, reached callees only
  // as the bytes they save: not carried by the view)
  InFrame(ctx, 128, 0x82106564, [&](uint32_t frame) {
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
      lifted::CallFromContext(ctx, target);
      return ctx.r3.u32;
    };
    calls.virtual_call = [&](uint32_t offset, uint32_t return_address) {
      return VirtualCall(ctx, interface, offset, return_address);
    };
    r3 = asu::AppStartUp(interface, frame, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// App::StartUp: r3 = app + 16, from App::App once at boot.
ITC_NATIVE_ENTRY_ALWAYS(82106548) {
  
  if (hooks::StartUpRecorded(ctx, 0x82106548)) {
    return;
  }
  if (!ITC_FLAG(native_app_start_up)) {
    lifted::original::c82106548(ctx);
    return;
  }
  hooks::AppStartUpSteps(ctx);
}

// --- The movie class's construction ---

namespace {

// (The originals' prologue saves, __savegprlr_N's link register and rN to
// r31 above their frames, aren't written since E5 step 6: nothing reads
// them, and the registers were stored as 0, not carried by the view.)

}  // namespace

void hooks::MovieWrapperSteps(lifted::Regs& ctx) {
  const uint32_t wrapper = ctx.r3.u32;
  if (const auto r3 = asu::MovieWrapperConstruct(wrapper)) {
    ctx.r3.u64 = *r3;
    return;
  }
  lifted::original::c82593DC0(ctx);
}

// MovieWrapper::MovieWrapper (sub_82593DC0) has no adapter: only Movie::Movie's
// plain C++ calls it in play (its steps above), the original with the flag
// off.

void hooks::MovieConstructSteps(lifted::Regs& ctx) {
  const uint32_t movie = ctx.r3.u32;
  const uint32_t block = ctx.r4.u32;
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> result;
  InFrame(ctx, 160, 0x820AEB10, [&](uint32_t frame) {
    asu::MovieCalls calls;
    calls.virtual_call = [&](uint32_t object, uint32_t offset, uint32_t r4, uint32_t r5,
                             uint32_t return_address) {
      ctx.r4.u64 = r4;
      ctx.r5.u64 = r5;
      return VirtualCall(ctx, object, offset, return_address);
    };
    calls.wrapper = [&](uint32_t wrapper) {
      ctx.r3.u64 = wrapper;
      ctx.lr = 0x820AEB1C;
      hooks::MovieWrapperSteps(ctx);
      return ctx.r3.u32;
    };
    result = asu::MovieConstruct(movie, block, frame, calls);
  });
  ctx.lr = lr;
  if (!result) {
    ctx.r3.u64 = movie;
    ctx.r4.u64 = block;
    lifted::original::c820AEA88(ctx);
    return;
  }
  ctx.r3.u64 = *result;
}

// Movie::Movie: r3 = the movie (312 bytes), r4 = the set-up block, from
// App::App once at boot.
ITC_NATIVE_ENTRY_ALWAYS(820AEA88) {
  
  if (hooks::StartUpRecorded(ctx, 0x820AEA88)) {
    return;
  }
  if (!ITC_FLAG(native_app_start_up) || !hooks::NativeHeapActive()) {
    lifted::original::c820AEA88(ctx);
    return;
  }
  hooks::MovieConstructSteps(ctx);
}

// --- The sound manager's construction ---

namespace {

asu::AudioManagerCalls AudioManagerCallsFor(lifted::Regs& ctx, uint32_t frame) {
  asu::AudioManagerCalls calls;
  calls.call = [&ctx](uint32_t target, uint32_t sp, uint32_t r3, std::optional<uint32_t> r4,
                            std::optional<uint32_t> r5, uint32_t return_address) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = r3;
    if (r4) {
      ctx.r4.u64 = *r4;
    }
    if (r5) {
      ctx.r5.u64 = *r5;
    }
    ctx.lr = return_address;
    lifted::CallFromContext(ctx, target);
    ctx.r1.u64 = stack;
    return ctx.r3.u32;
  };
  calls.virtual_call = [&ctx, frame](uint32_t object, uint32_t offset,
                                           std::optional<uint32_t> r4, std::optional<uint32_t> r5,
                                           uint32_t return_address) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    if (r4) {
      ctx.r4.u64 = *r4;
    }
    if (r5) {
      ctx.r5.u64 = *r5;
    }
    const uint32_t r3 = VirtualCall(ctx, object, offset, return_address);
    ctx.r1.u64 = stack;
    return r3;
  };
  return calls;
}

}  // namespace

void hooks::AudioManagerCreateSteps(lifted::Regs& ctx) {
  const uint32_t owner = ctx.r3.u32, name = ctx.r4.u32, count = ctx.r5.u32;
  const uint64_t entry_r3 = ctx.r3.u64, entry_r4 = ctx.r4.u64, entry_r5 = ctx.r5.u64;
  const uint64_t lr = ctx.lr;
  const uint32_t sp = ctx.r1.u32;
  // The frames as the original's: Create's 112 bytes, the constructor's 256
  // below it, StartXAudio's 144 below that; their back chains.
  const uint32_t create_frame = sp - 112;
  const uint32_t frame = create_frame - 256;
  const uint32_t start_frame = frame - 144;
  guest::StoreU32(create_frame, sp);
  guest::StoreU32(frame, create_frame);
  guest::StoreU32(start_frame, frame);
  const auto calls = AudioManagerCallsFor(ctx, frame);
  const auto result = asu::AudioManagerCreate(owner, name, count, frame, start_frame, calls);
  ctx.r1.u64 = sp;
  ctx.lr = lr;
  if (!result) {
    ctx.r3.u64 = entry_r3;
    ctx.r4.u64 = entry_r4;
    ctx.r5.u64 = entry_r5;
    lifted::original::c82119B38(ctx);
    return;
  }
  ctx.r3.u64 = *result;
}

// AudioManager::Create: r3 = the owner (app + 12), r4 = the name, r5 = the
// bank count's address; from App::App once at boot. (AudioManager's
// constructor and AudioEngine::StartXAudio have no adapters: only its plain
// C++ calls them in play.)
ITC_NATIVE_ENTRY_ALWAYS(82119B38) {
  
  if (hooks::StartUpRecorded(ctx, 0x82119B38)) {
    return;
  }
  if (!ITC_FLAG(native_app_start_up) || !hooks::NativeHeapActive()) {
    lifted::original::c82119B38(ctx);
    return;
  }
  hooks::AudioManagerCreateSteps(ctx);
}

void hooks::StartXAudioSteps(lifted::Regs& ctx) {
  const uint32_t sp = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  // The original's 144-byte frame.
  const uint32_t frame = sp - 144;
  guest::StoreU32(frame, sp);
  const auto calls = AudioManagerCallsFor(ctx, frame);
  // r3 as the last guest call left it (XAudioInitialize's, in full).
  asu::StartXAudio(frame, calls);
  ctx.r1.u64 = sp;
  ctx.lr = lr;
}

// --- The app's second camera class ---

namespace {

asu::AppCameraCalls AppCameraCallsFor(lifted::Regs& ctx, uint64_t owner);

// The constructors (their prologue saves, lr and the owner's object as r31,
// not written since E5 step 6: nothing reads them).
uint32_t AppCameraCameraSteps(lifted::Regs& ctx, uint32_t camera, uint64_t owner) {
  const uint32_t sp = ctx.r1.u32;
  const uint32_t frame = sp - 144;
  guest::StoreU32(frame, sp);
  return asu::AppCameraCameraConstruct(camera, frame, AppCameraCallsFor(ctx, owner));
}

uint32_t AppCameraListSteps(lifted::Regs& ctx, uint32_t list, uint64_t owner) {
  (void)owner;
  if (const auto r3 = asu::AppCameraListConstruct(list)) {
    return *r3;
  }
  const uint64_t lr = ctx.lr;
  ctx.r3.u64 = list;
  ctx.lr = 0x820A8C64;
  lifted::original::c820A98C0(ctx);
  ctx.lr = lr;
  return ctx.r3.u32;
}

asu::AppCameraCalls AppCameraCallsFor(lifted::Regs& ctx, uint64_t owner) {
  asu::AppCameraCalls calls;
  calls.call = [&ctx](uint32_t target, uint32_t sp, uint32_t r3, uint32_t return_address) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = r3;
    ctx.lr = return_address;
    lifted::CallFromContext(ctx, target);
    ctx.r1.u64 = stack;
    return ctx.r3.u32;
  };
  calls.virtual_call = [&ctx](uint32_t object, uint32_t offset, uint32_t sp,
                                    std::optional<uint32_t> r4, std::optional<uint32_t> r5,
                                    std::optional<uint32_t> f1, uint32_t return_address) {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    if (r4) {
      ctx.r4.u64 = *r4;
    }
    if (r5) {
      ctx.r5.u64 = *r5;
    }
    if (f1) {
      float value;
      const uint32_t bits = *f1;
      std::memcpy(&value, &bits, 4);
      ctx.f1.f64 = double(value);
    }
    const uint32_t r3 = VirtualCall(ctx, object, offset, return_address);
    ctx.r1.u64 = stack;
    return r3;
  };
  calls.camera = [&ctx, owner](uint32_t block) {
    return AppCameraCameraSteps(ctx, block, owner);
  };
  calls.list = [&ctx, owner](uint32_t block) {
    return AppCameraListSteps(ctx, block, owner);
  };
  return calls;
}

}  // namespace

void hooks::AppCameraSteps(lifted::Regs& ctx) {
  const uint32_t object = ctx.r3.u32, block = ctx.r4.u32;
  const uint64_t entry_r3 = ctx.r3.u64, entry_r4 = ctx.r4.u64;
  const uint64_t lr = ctx.lr;
  const uint32_t sp = ctx.r1.u32;
  const uint32_t frame = sp - 224;
  guest::StoreU32(frame, sp);
  ctx.r1.u64 = frame;
  const auto result =
      asu::AppCameraConstruct(object, block, frame, AppCameraCallsFor(ctx, entry_r3));
  ctx.r1.u64 = sp;
  ctx.lr = lr;
  if (!result) {
    ctx.r3.u64 = entry_r3;
    ctx.r4.u64 = entry_r4;
    lifted::original::c820A8B88(ctx);
    return;
  }
  ctx.r3.u64 = *result;
}

void hooks::AppCameraCameraConstructSteps(lifted::Regs& ctx) {
  const uint64_t lr = ctx.lr;
  const uint32_t sp = ctx.r1.u32;
  const uint32_t frame = sp - 144;
  guest::StoreU32(frame, sp);
  const uint32_t r3 =
      asu::AppCameraCameraConstruct(ctx.r3.u32, frame, AppCameraCallsFor(ctx, 0));
  ctx.r1.u64 = sp;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

void hooks::AppCameraListConstructSteps(lifted::Regs& ctx) {
  if (const auto r3 = asu::AppCameraListConstruct(ctx.r3.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  lifted::original::c820A98C0(ctx);
}

// The second camera class: r3 = the object (368 bytes), r4 = the block
// {app + 20, app + 4}; from App::App once at boot. (Its camera, list and
// entry constructors, sub_820A9E28, sub_820A98C0 and sub_820A75E8, have no
// adapters: only its plain C++ calls them in play.)
ITC_NATIVE_ENTRY_ALWAYS(820A8B88) {
  
  if (hooks::StartUpRecorded(ctx, 0x820A8B88)) {
    return;
  }
  if (!ITC_FLAG(native_app_start_up) || !hooks::NativeHeapActive()) {
    lifted::original::c820A8B88(ctx);
    return;
  }
  hooks::AppCameraSteps(ctx);
}

// --- The app's constructor ---

void hooks::AppConstructSteps(lifted::Regs& ctx) {
  const uint32_t app = ctx.r3.u32, block = ctx.r4.u32;
  const uint64_t lr = ctx.lr;
  // (Its saved registers as it sets them for the body, r22 to r31, reached
  // callees only as the bytes they save: not carried by the view; nor kept
  // and put back.)
  uint32_t r3 = 0;
  InFrame(ctx, 240, 0x82094644, [&](uint32_t frame) {
    asu::AppCalls calls;
    calls.call = [&](uint32_t target, uint32_t a, std::optional<uint32_t> b,
                     uint32_t return_address, uint32_t r30, std::optional<uint32_t> c) {
      ctx.r3.u64 = a;
      if (b) {
        ctx.r4.u64 = *b;
      }
      if (c) {
        ctx.r5.u64 = *c;
      }
      (void)r30;  // (not carried by the view, as the other saved registers)
      ctx.lr = return_address;
      lifted::CallFromContext(ctx, target);
      return ctx.r3.u32;
    };
    calls.virtual_call = [&](uint32_t object, uint32_t offset, uint32_t a, uint32_t b,
                             uint32_t return_address) {
      ctx.r4.u64 = a;
      ctx.r5.u64 = b;
      return VirtualCall(ctx, object, offset, return_address);
    };
    r3 = asu::AppConstruct(app, block, frame, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// App::App: r3 = the app (132 bytes), r4 = the block; from App::Create once
// at boot.
ITC_NATIVE_ENTRY_ALWAYS(82094590) {
  
  if (hooks::StartUpRecorded(ctx, 0x82094590)) {
    return;
  }
  if (!ITC_FLAG(native_app_start_up) || !hooks::NativeHeapActive()) {
    lifted::original::c82094590(ctx);
    return;
  }
  hooks::AppConstructSteps(ctx);
}

// --- App::Create and the app's interface slots ---

void hooks::AppCreateSteps(lifted::Regs& ctx) {
  const uint32_t block = ctx.r3.u32;
  const uint64_t entry_r3 = ctx.r3.u64;
  const uint64_t lr = ctx.lr;
  std::optional<uint32_t> result;
  // (r31 = the block in the body: not carried by the view, nor kept)
  InFrame(ctx, 96, 0x82094AE4, [&](uint32_t) {
    result = asu::AppCreate(block, [&](uint32_t app) {
      ctx.r3.u64 = app;
      ctx.r4.u64 = entry_r3;
      ctx.lr = 0x82094AE4;
      lifted::Enter(ctx, game::App::Ctor);  // App::App (its adapter: the harness's recorder)
      return ctx.r3.u32;
    });
  });
  ctx.lr = lr;
  if (!result) {
    ctx.r3.u64 = entry_r3;
    lifted::original::c82094AB8(ctx);
    return;
  }
  ctx.r3.u64 = *result;
}

// App::Create: r3 = the block {the run loop, the framework's +36
// interface}; from Framework::Init (recompiled) once at boot.
ITC_NATIVE_ENTRY_ALWAYS(82094AB8) {
  
  if (!ITC_FLAG(native_app_start_up) || !hooks::NativeHeapActive()) {
    lifted::original::c82094AB8(ctx);
    return;
  }
  hooks::AppCreateSteps(ctx);
}

// The getters of +108 and +112 (vtable slots; r3 the object).
ITC_NATIVE_ENTRY(82379338, ITC_FLAG(native_app_start_up)) {
  ctx.r3.u64 = asu::InterfaceWord108(ctx.r3.u32);
}

ITC_NATIVE_ENTRY(82379350, ITC_FLAG(native_app_start_up)) {
  ctx.r3.u64 = asu::InterfaceWord112(ctx.r3.u32);
}

// The app's interfaces' slots to the framework's first worker pool: a tail
// call (lr the caller's; ctr the target, as the original's; r11, also the
// target there, isn't carried: none reads it).
namespace {

void TailCall(lifted::Regs& ctx, const asu::SlotCall& call) {
  ctx.r3.u64 = call.object;
  lifted::CallFromContext(ctx, call.target);
}

}  // namespace

ITC_NATIVE_ENTRY(82094368, ITC_FLAG(native_app_start_up)) {
  TailCall(ctx, asu::AppPoolSlot(ctx.r3.u32));
}

ITC_NATIVE_ENTRY(820944A8, ITC_FLAG(native_app_start_up)) {
  TailCall(ctx, asu::AppPoolSlotFromInterface16(ctx.r3.u32));
}

// The scene's render manager's slot 8 and the frame driver's slots (vtable
// slots; tail calls through the framework's +36 interface).
#define ITC_APP_SLOT(address, plain)                    \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_app_start_up)) { \
    TailCall(ctx, asu::plain(ctx.r3.u32));  \
  }

ITC_APP_SLOT(82094338, SceneSlot)
ITC_APP_SLOT(820944D8, SceneSlotFromInterface16)
ITC_APP_SLOT(82094320, FrameDriverSlot)
ITC_APP_SLOT(82094498, FrameDriverSlotFromInterface16)
ITC_APP_SLOT(82094480, FrameDriverSlotFromInterface8)

bool hooks::RenderManagerSlotDirect(lifted::Regs& ctx, uint32_t function) {
  const auto slot = asu::RenderManagerCall(function, ctx.r3.u32);
  if (!slot) {
    return false;
  }
  ctx.r3.u64 = slot->r3;
  // (r11 = the target was a leftover for recompiled callers: none now)
  ctx.ctr.u64 = slot->target;
  return true;
}

// For the fallback originals (docs/missing-functions.md): XNetStartup
// (sub_82575B08) has no adapter; no arguments, r3 = 0 or 1627. Its memcpy
// from XNet::Configure's frame below the original's 496-byte one.
ITC_DIRECT_ENTRY(82575B08) {
  const uint64_t sp = ctx.r1.u64;
  const uint64_t lr = ctx.lr;
  ctx.r1.u64 = sp - 496;
  guest::StoreU32(ctx.r1.u32, uint32_t(sp));
  const uint32_t r3 = asu::XNetStartup(XNetMemcpy(ctx));
  ctx.r1.u64 = sp;
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}
