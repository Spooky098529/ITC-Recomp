// The post effects' making natively (post_effects.h,
// docs/functions/post-effects.md): PostEffects::Create, the constructors of
// the glow, the color filter and the motion blur, and the render textures
// they ask for. Call for call as the recompiled functions; each function's
// frame is where the original's is (`stack` is the guest r1 at its entry),
// and the words the original writes there are written there too.

#include "post_effects/post_effects.h"

#include "guest.h"

#include "base/log.h"
#include "app_start_up/start_up.h"
#include "c_runtime/strings.h"
#include "graphics/resources.h"
#include "graphics/video_output.h"
#include "memory/c_runtime_heap.h"
#include "models/xpr.h"

#include <cstdlib>

namespace post_effects {

namespace {

constexpr uint32_t kRenderTextures = 0x826C5DC8;  // 20 entries of 36 bytes
constexpr uint32_t kTable = 0x826174A8;           // 15 entries of 36 bytes
constexpr uint32_t kBaseVtable = 0x82008838;

// The frames of the originals.
constexpr uint32_t kCreateFrame = 128;
constexpr uint32_t kInitBootFrame = 112;
constexpr uint32_t kAcquireFrame = 160;
constexpr uint32_t kTextureCreateFrame = 144;
constexpr uint32_t kGlowFrame = 144;
constexpr uint32_t kColorFilterFrame = 96;
constexpr uint32_t kMotionBlurFrame = 224;

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// A single-precision constant (lfs) stored (stfs): its bits through a
// double, as the originals copy them.
void CopySingle(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, float(double(guest::LoadFloat(base, from))));
}

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Post effects: {} refused after its checks", what);
    std::abort();
  }
}

// operator new's plain C++; 0 when it fails (logged: the original's new
// handler and std::bad_alloc, never seen).
uint32_t New(uint8_t* base, uint32_t bytes) {
  const auto block = memory::crt::OperatorNew(base, bytes);
  if (!block) {
    ITC_LOG_ERROR("Post effects: operator new refused {} bytes", bytes);
    return 0;
  }
  return *block;
}

// The common start of the effects' constructors: the base vtable, the table
// entry's 36 bytes copied to +4 (memcpy).
void ConstructBase(uint8_t* base, uint32_t effect, uint32_t entry) {
  Store(base, effect + 0, kBaseVtable);
  Require(c_runtime::Memcpy(base, effect + 4, entry, 36), "memcpy of the table entry");
}

// Device::CreateSurface's 12-byte parameters at `at`: the EDRAM base, 0, 0
// (as the originals write them: a doubleword 0, a word 0, then the base).
void EdramParameters(uint8_t* base, uint32_t at) {
  Store(base, at, 0);
  Store(base, at + 4, 0);
  Store(base, at + 8, 0);
  Store(base, at, Load(base, kEdramBase));
}

}  // namespace

uint32_t RenderTextureCreate(uint8_t* base, uint32_t stack, uint32_t entry, uint32_t format,
                             uint32_t width, uint32_t height, uint32_t id, uint32_t key0,
                             uint32_t key1) {
  ITC_NATIVE_RUNNING("sub_821330D0 RenderTexture::Create");
  const uint32_t frame = stack - kTextureCreateFrame;
  Store(base, entry + 32, id);
  Store(base, entry + 0, format);
  Store(base, entry + 12, key0);
  Store(base, entry + 16, key1);
  Store(base, entry + 4, width);
  Store(base, entry + 8, height);
  const uint32_t header = New(base, 52);
  // XGSetTextureHeader(width, height, 1 level, usage 0, format, no
  // addresses): its stack words, in the original's order.
  const uint32_t f = Load(base, entry + 0);
  const uint32_t h = Load(base, entry + 8);
  const uint32_t w = Load(base, entry + 4);
  Store(base, frame + 92, header);
  Store(base, frame + 108, 0);
  Store(base, frame + 100, 0);
  Store(base, frame + 84, 0);
  Store(base, entry + 24, header);
  Require(graphics::XgHeaderCopyFits(header), "XGSetTextureHeader");
  const uint32_t size = graphics::XGSetTextureHeader(base, w, h, 1, 0, f, 0, 0, 0, header, 0, 0);
  // XPhysicalAlloc(size, -1, 0, 0x404).
  const uint32_t memory = memory::crt::XPhysicalAllocEx(base, size, 0, 0xFFFFFFFFu, 0, 0x404);
  Store(base, entry + 28, memory);
  if (memory == 0) {
    return 0;
  }
  models::RelocateResource(base, Load(base, entry + 24), memory);
  return 1;
}

uint32_t RenderTexturesAcquire(uint8_t* base, uint32_t stack, uint32_t id, uint32_t format,
                               uint32_t width, uint32_t height, uint32_t key0, uint32_t key1) {
  ITC_NATIVE_RUNNING("sub_82133190 RenderTextures::Acquire");
  const uint32_t frame = stack - kAcquireFrame;
  const uint32_t entry = kRenderTextures + id * 36;
  uint32_t texture = 0;
  if (int32_t(id) != 20 && Load(base, entry + 20) == 0) {
    RenderTextureCreate(base, frame, entry, format, width, height, id, key0, key1);
    texture = entry;
  } else if (int32_t(id) != 20 && Load(base, entry + 0) == format &&
             Load(base, entry + 4) == width && Load(base, entry + 8) == height &&
             Load(base, entry + 12) == key0 && Load(base, entry + 16) == key1) {
    texture = entry;
  } else {
    // Id 20, or the id in use with another key: a new entry of its own,
    // id 20 (made at address 0 when operator new fails, as the original).
    texture = New(base, 36);
    if (texture != 0) {
      Store(base, texture + 20, 0);
      Store(base, texture + 24, 0);
      Store(base, texture + 28, 0);
    }
    RenderTextureCreate(base, frame, texture, format, width, height, 20, key0, key1);
  }
  Store(base, texture + 20, Load(base, texture + 20) + 1);
  return texture;
}

uint32_t GlowConstruct(uint8_t* base, uint32_t stack, uint32_t glow, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82138610 Glow::Glow");
  const uint32_t frame = stack - kGlowFrame;
  ConstructBase(base, glow, entry);
  Store(base, glow + 44, 0);
  Store(base, glow + 48, 0);
  Store(base, glow + 40, 1);
  Store(base, glow + 52, 0);
  Store(base, glow + 8056, 0);
  Store(base, glow + 8060, 0);
  Store(base, glow + 0, 0x820088B0);
  GlareDefConstruct(base, glow + 8064);
  for (uint32_t at = 8352; at <= 8432; at += 4) {
    Store(base, glow + at, 0);
  }
  // The parameters' defaults (the image's constants).
  static constexpr uint32_t kDefaults[11] = {0x8200882C, 0x82008834, 0x82002D14, 0x82008928,
                                             0x82008830, 0x82002C40, 0x82008834, 0x82002A3C,
                                             0x8200882C, 0x82002208, 0x82000A34};
  for (uint32_t i = 0; i < 11; i++) {
    CopySingle(base, glow + 8436 + 4 * i, kDefaults[i]);
  }
  GlareDefInit(base, glow + 8064, 4);
  constexpr uint32_t kFrameFormat = 0x1A20015A;
  constexpr uint32_t kSmallFormat = 0x2DA2ABA4;
  auto width = [&]() { return Load(base, glow + 16); };
  auto height = [&]() { return Load(base, glow + 20); };
  auto acquire = [&](uint32_t id, uint32_t format, uint32_t w, uint32_t h) {
    return RenderTexturesAcquire(base, frame, id, format, w, h, 0, 0);
  };
  Store(base, glow + 8352, acquire(1, kFrameFormat, width(), height()));
  Store(base, glow + 8356, acquire(6, kFrameFormat, width() >> 3, height() >> 3));
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, glow + 8360 + 4 * i, acquire(7 + i, kFrameFormat, width() >> 2, height() >> 2));
  }
  Store(base, glow + 8384, acquire(13, kFrameFormat, width() >> 2, height() >> 2));
  Store(base, glow + 8388, acquire(14, kSmallFormat, 64, 64));
  Store(base, glow + 8392, acquire(15, kSmallFormat, 16, 16));
  Store(base, glow + 8396, acquire(16, kSmallFormat, 4, 4));
  Store(base, glow + 8400, acquire(17, kSmallFormat, 1, 1));
  Store(base, glow + 8404, acquire(18, kSmallFormat, 1, 1));
  Store(base, glow + 8408, acquire(20, kFrameFormat, width() >> 2, height() >> 2));
  // The surfaces, all with the same parameters at frame + 80.
  constexpr uint32_t kSurfaceFormat = 0x1A2201BF;
  const uint32_t parameters = frame + 80;
  EdramParameters(base, parameters);
  auto surface = [&](uint32_t w, uint32_t h, uint32_t format) {
    return graphics::CreateSurface(base, w, h, format, 0, parameters);
  };
  Store(base, glow + 8412, surface(width() >> 2, height() >> 2, kSurfaceFormat));
  Store(base, glow + 8416, surface(width() >> 3, height() >> 3, kSurfaceFormat));
  Store(base, glow + 8420, surface(64, 64, kSmallFormat));
  Store(base, glow + 8424, surface(16, 16, kSmallFormat));
  Store(base, glow + 8428, surface(4, 4, kSmallFormat));
  Store(base, glow + 8432, surface(1, 1, kSmallFormat));
  return glow;
}

uint32_t ColorFilterConstruct(uint8_t* base, uint32_t stack, uint32_t filter, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82135008 ColorFilter::ColorFilter");
  const uint32_t frame = stack - kColorFilterFrame;
  ConstructBase(base, filter, entry);
  Store(base, filter + 40, 1);
  Store(base, filter + 0, 0x82008860);
  for (uint32_t at : {44u, 48u, 52u, 8056u, 8060u, 8064u}) {
    Store(base, filter + at, 0);
  }
  Store(base, filter + 8068, 1);
  Store(base, filter + 8072, 0);
  constexpr uint32_t kOne = 0x8200882C, kZero = 0x82000A34;
  static constexpr uint32_t kValues[11] = {kOne,  kOne,  kOne,  kZero, kZero, kOne,
                                           kOne,  kOne,  kZero, kZero, kZero};
  for (uint32_t i = 0; i < 11; i++) {
    CopySingle(base, filter + 8080 + 4 * i, kValues[i]);
  }
  Store(base, filter + 8064,
        RenderTexturesAcquire(base, frame, 1, 0x1A20015A, Load(base, filter + 16),
                              Load(base, filter + 20), Load(base, filter + 24),
                              Load(base, filter + 28)));
  return filter;
}

uint32_t MotionBlurConstruct(uint8_t* base, uint32_t stack, uint32_t blur, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82136130 MotionBlur::MotionBlur");
  const uint32_t frame = stack - kMotionBlurFrame;
  ConstructBase(base, blur, entry);
  constexpr uint32_t kOne = 0x8200882C;
  Store(base, blur + 44, 0);
  Store(base, blur + 48, 0);
  Store(base, blur + 40, 1);
  Store(base, blur + 52, 0);
  Store(base, blur + 8056, 0);
  Store(base, blur + 8060, 0);
  Store(base, blur + 0, 0x820088A4);
  for (uint32_t at = 8064; at <= 8076; at += 4) {
    Store(base, blur + at, 0);
  }
  // The two matrices' diagonals' last 1.0s, then both made the identity
  // (memcpy from the identity built in the frame at +112).
  for (uint32_t matrix : {8080u, 8144u}) {
    for (uint32_t row = 0; row < 4; row++) {
      CopySingle(base, blur + matrix + 16 * row + 12, kOne);
    }
  }
  CopySingle(base, blur + 8208, 0x82002A10);
  CopySingle(base, blur + 8212, 0x82008810);
  CopySingle(base, blur + 8216, 0x82008834);
  CopySingle(base, blur + 8220, 0x82008828);
  if (Load(base, blur + 36) != 0) {
    CopySingle(base, blur + 8208, 0x82008834);
    CopySingle(base, blur + 8212, 0x82002CBC);
    CopySingle(base, blur + 8220, 0x820021F8);
  }
  const uint32_t identity = frame + 112;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreFloat(base, identity + 16 * row + 4 * column, row == column ? 1.0f : 0.0f);
    }
  }
  Require(c_runtime::Memcpy(base, blur + 8080, identity, 64), "memcpy of the identity");
  Require(c_runtime::Memcpy(base, blur + 8144, identity, 64), "memcpy of the identity");
  auto width = [&]() { return Load(base, blur + 16); };
  auto height = [&]() { return Load(base, blur + 20); };
  Store(base, blur + 8064,
        RenderTexturesAcquire(base, frame, 20, 0x2D200199, width() >> 1, height() >> 1, 0, 0));
  Store(base, blur + 8068,
        RenderTexturesAcquire(base, frame, 1, 0x1A20015A, width(), height(),
                              Load(base, blur + 24), Load(base, blur + 28)));
  // The first surface's parameters at frame + 96 (all 0), the second's at
  // frame + 80 (the EDRAM base).
  Store(base, frame + 96, 0);
  Store(base, frame + 100, 0);
  Store(base, frame + 104, 0);
  Store(base, blur + 8072,
        graphics::CreateSurface(base, width() >> 1, height() >> 1, 0x2D20AB8D, 0, frame + 96));
  EdramParameters(base, frame + 80);
  Store(base, blur + 8076,
        graphics::CreateSurface(base, width() >> 1, height() >> 1, 0x1A220197, 0, frame + 80));
  return blur;
}

uint32_t Create(uint8_t* base, uint32_t stack, uint32_t device, uint32_t index,
                const CreateCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82138A18 PostEffects::Create");
  namespace vo = graphics::video_output;
  const uint32_t frame = stack - kCreateFrame;
  const uint32_t local = stack + 20;  // the device word, in the caller's frame
  Store(base, local, device);
  const bool lock = device == 0;
  uint32_t r3 = device;
  if (lock) {
    r3 = vo::LockDevice(base, calls.video_output(0));
    Store(base, local, r3);
  }
  const uint32_t entry = kTable + index * 36;
  const uint32_t kind = Load(base, entry + 0);
  static constexpr uint32_t kSizes[6] = {8480, 8112, 8128, 8224, 8068, 8068};
  if (kind <= 5) {
    const uint32_t at = kEffects + (index << 2);
    if (Load(base, at) == 0) {
      const uint32_t object = New(base, kSizes[kind]);
      r3 = 0;
      if (object != 0) {
        switch (kind) {
          case 0: r3 = GlowConstruct(base, frame, object, entry); break;
          case 2: r3 = ColorFilterConstruct(base, frame, object, entry); break;
          case 3: r3 = MotionBlurConstruct(base, frame, object, entry); break;
          case 5: {
            const auto made = TargetFilterConstruct(base, frame - 112, object, entry);
            Require(made.has_value(), "TargetFilter::TargetFilter");
            r3 = *made;
            break;
          }
          default:
            // Kinds 1 and 4 (never made in play): the recompiled
            // constructors.
            r3 = calls.construct(kind == 1 ? 0x82134410 : 0x82133B10, object, entry,
                                 Load(base, local));
            break;
        }
      }
      Store(base, at, r3);
    }
  }
  if (lock) {
    r3 = calls.video_output(1);
    vo::UnlockDevice(base, r3, local);
  }
  return r3;
}

uint32_t InitBoot(uint8_t* base, uint32_t stack, uint32_t device, const InitBootCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82138C38 PostEffects::InitBoot");
  namespace vo = graphics::video_output;
  const uint32_t frame = stack - kInitBootFrame;
  const uint32_t local = stack + 20;  // the device word, in the caller's frame
  Store(base, local, device);
  const bool lock = device == 0;
  if (lock) {
    Store(base, local, vo::LockDevice(base, calls.video_output(0)));
  }
  const auto declaration =
      app_start_up::CreateVertexDeclaration(base, 0x826176C4, calls.memcpy_call);
  Require(declaration.has_value(), "CreateVertexDeclaration");
  Store(base, kVertexDeclaration, *declaration);
  const uint32_t effect_device = Load(base, local);
  uint32_t r3 = calls.in_create_frame(
      [&](const CreateCalls& create) { return Create(base, frame, effect_device, 14, create); });
  if (lock) {
    r3 = calls.video_output(1);
    vo::UnlockDevice(base, r3, local);
  }
  return r3;
}

}  // namespace post_effects
