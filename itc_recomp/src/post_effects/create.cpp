// The post effects' making natively (post_effects.h,
// docs/functions/post-effects.md): PostEffects::Create, the constructors of
// the glow, the color filter and the motion blur, and the render textures
// they ask for. Call for call as the recompiled functions; a frame whose
// words are handed on (the surfaces' parameters, the identity, the
// constructors' callers' r1) is where the original's is (`stack` is the
// guest r1 at its entry); the others aren't made (E5 step 6).

#include "post_effects/post_effects.h"

#include "guest.h"

#include "base/log.h"
#include "app_start_up/start_up.h"
#include "c_runtime/strings.h"
#include "graphics/resources.h"
#include "graphics/video_output.h"
#include "memory/c_runtime_heap.h"
#include "models/xpr.h"
#include "schema/graphics_resources.h"
#include "schema/post_effects.h"

#include <cstdlib>

namespace post_effects {

namespace {

using G = schema::PostEffectsGlobals;
using schema::PostEffect;
using schema::RenderTexture;

// The frames of the originals.
constexpr uint32_t kCreateFrame = 128;
constexpr uint32_t kInitBootFrame = 112;
constexpr uint32_t kGlowFrame = 144;
constexpr uint32_t kMotionBlurFrame = 224;
// RenderTexture::Create's (144), RenderTextures::Acquire's (160) and the
// color filter's (96) aren't made (E5 step 6): nothing in them is read or
// handed on.

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
// The same, as the value a view's setter takes.
float Single(uint8_t* base, uint32_t from) {
  return float(double(guest::LoadFloat(base, from)));
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
  const PostEffect e(base, effect);
  e.set_vtable(PostEffect::kVtable);
  Require(c_runtime::Memcpy(base, e.kind_address(), entry, 36), "memcpy of the table entry");
}

// Device::CreateSurface's 12-byte parameters at `at`: the EDRAM base, 0, 0
// (as the originals write them: a doubleword 0, a word 0, then the base).
void EdramParameters(uint8_t* base, uint32_t at) {
  Store(base, at, 0);
  Store(base, at + 4, 0);
  Store(base, at + 8, 0);
  Store(base, at, G::edram_base(base));
}

}  // namespace

uint32_t RenderTextureCreate(uint8_t* base, uint32_t stack, uint32_t entry, uint32_t format,
                             uint32_t width, uint32_t height, uint32_t id, uint32_t key0,
                             uint32_t key1) {
  ITC_NATIVE_RUNNING("sub_821330D0 RenderTexture::Create");
  (void)stack;
  const RenderTexture t(base, entry);
  t.set_id(id);
  t.set_format(format);
  t.set_key0(key0);
  t.set_key1(key1);
  t.set_width(width);
  t.set_height(height);
  const uint32_t header = New(base, 52);
  // XGSetTextureHeader(width, height, 1 level, usage 0, format, no
  // addresses). (The original's stack words for it, +84 to +108 of its
  // frame, were read by nothing but its own call: not written, E5 step 6.)
  const uint32_t f = t.format();
  const uint32_t h = t.height();
  const uint32_t w = t.width();
  t.set_header(header);
  Require(graphics::XgHeaderCopyFits(header), "XGSetTextureHeader");
  const uint32_t size = graphics::XGSetTextureHeader(base, w, h, 1, 0, f, 0, 0, 0, header, 0, 0);
  // XPhysicalAlloc(size, -1, 0, 0x404).
  const uint32_t memory = memory::crt::XPhysicalAllocEx(base, size, 0, 0xFFFFFFFFu, 0, 0x404);
  t.set_memory(memory);
  if (memory == 0) {
    return 0;
  }
  models::RelocateResource(base, t.header().address, memory);
  return 1;
}

uint32_t RenderTexturesAcquire(uint8_t* base, uint32_t stack, uint32_t id, uint32_t format,
                               uint32_t width, uint32_t height, uint32_t key0, uint32_t key1) {
  ITC_NATIVE_RUNNING("sub_82133190 RenderTextures::Acquire");
  const RenderTexture pooled(base, G::render_textures_address() + id * 36);
  const uint32_t entry = pooled.address;
  uint32_t texture = 0;
  if (int32_t(id) != 20 && pooled.users() == 0) {
    RenderTextureCreate(base, stack, entry, format, width, height, id, key0, key1);
    texture = entry;
  } else if (int32_t(id) != 20 && pooled.format() == format && pooled.width() == width &&
             pooled.height() == height && pooled.key0() == key0 && pooled.key1() == key1) {
    texture = entry;
  } else {
    // Id 20, or the id in use with another key: a new entry of its own,
    // id 20 (made at address 0 when operator new fails, as the original).
    texture = New(base, 36);
    if (texture != 0) {
      const RenderTexture own(base, texture);
      own.set_users(0);
      own.set_header(0u);
      own.set_memory(0);
    }
    RenderTextureCreate(base, stack, texture, format, width, height, 20, key0, key1);
  }
  const RenderTexture acquired(base, texture);
  acquired.set_users(acquired.users() + 1);
  return texture;
}

uint32_t GlowConstruct(uint8_t* base, uint32_t stack, uint32_t glow, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82138610 Glow::Glow");
  const uint32_t frame = stack - kGlowFrame;
  ConstructBase(base, glow, entry);
  const schema::Glow g(base, glow);
  g.set_no_history(0);
  g.set_updated(0);
  g.set_reset(1);
  g.set_skipped(0);
  g.set_vertex_count(0);
  g.set_vertex_buffer(0);
  g.set_vtable(schema::Glow::kVtable);
  GlareDefConstruct(base, g.glare_address());
  // The render textures and surfaces, +8352 to +8432, cleared.
  for (uint32_t at = g.frame_texture_address(); at <= g.surface_1_address(); at += 4) {
    Store(base, at, 0);
  }
  // The parameters' defaults (the image's constants).
  g.set_c33_x_second(Single(base, 0x8200882C));
  g.set_c33_x_first(Single(base, 0x82008834));
  g.set_c34_x(Single(base, 0x82002D14));
  g.set_c34_y(Single(base, 0x82008928));
  g.set_strength(Single(base, 0x82008830));
  g.set_c33_z(Single(base, 0x82002C40));
  g.set_history_weight(Single(base, 0x82008834));
  g.set_param_8464(Single(base, 0x82002A3C));
  g.set_star_step(Single(base, 0x8200882C));
  g.set_star_attenuation(Single(base, 0x82002208));
  g.set_c33_w(Single(base, 0x82000A34));
  GlareDefInit(base, g.glare_address(), 4);
  constexpr uint32_t kFrameFormat = 0x1A20015A;
  constexpr uint32_t kSmallFormat = 0x2DA2ABA4;
  auto width = [&]() { return g.width(); };
  auto height = [&]() { return g.height(); };
  auto acquire = [&](uint32_t id, uint32_t format, uint32_t w, uint32_t h) {
    return RenderTexturesAcquire(base, frame, id, format, w, h, 0, 0);
  };
  g.set_frame_texture(acquire(1, kFrameFormat, width(), height()));
  g.set_eighth_texture(acquire(6, kFrameFormat, width() >> 3, height() >> 3));
  // line_texture_0 to 5.
  for (uint32_t i = 0; i < 6; i++) {
    Store(base, g.line_texture_0_address() + 4 * i,
          acquire(7 + i, kFrameFormat, width() >> 2, height() >> 2));
  }
  g.set_star_texture(acquire(13, kFrameFormat, width() >> 2, height() >> 2));
  g.set_texture_64(acquire(14, kSmallFormat, 64, 64));
  g.set_texture_16(acquire(15, kSmallFormat, 16, 16));
  g.set_texture_4(acquire(16, kSmallFormat, 4, 4));
  g.set_texture_1(acquire(17, kSmallFormat, 1, 1));
  g.set_final_texture_1(acquire(18, kSmallFormat, 1, 1));
  g.set_history(acquire(20, kFrameFormat, width() >> 2, height() >> 2));
  // The surfaces, all with the same parameters at frame + 80.
  constexpr uint32_t kSurfaceFormat = 0x1A2201BF;
  const uint32_t parameters = frame + 80;
  EdramParameters(base, parameters);
  auto surface = [&](uint32_t w, uint32_t h, uint32_t format) {
    return graphics::CreateSurface(base, w, h, format, 0, parameters);
  };
  g.set_quarter_surface(surface(width() >> 2, height() >> 2, kSurfaceFormat));
  g.set_eighth_surface(surface(width() >> 3, height() >> 3, kSurfaceFormat));
  g.set_surface_64(surface(64, 64, kSmallFormat));
  g.set_surface_16(surface(16, 16, kSmallFormat));
  g.set_surface_4(surface(4, 4, kSmallFormat));
  g.set_surface_1(surface(1, 1, kSmallFormat));
  return glow;
}

uint32_t ColorFilterConstruct(uint8_t* base, uint32_t stack, uint32_t filter, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82135008 ColorFilter::ColorFilter");
  ConstructBase(base, filter, entry);
  const schema::ColorFilter f(base, filter);
  f.set_reset(1);
  f.set_vtable(schema::ColorFilter::kVtable);
  f.set_no_history(0);
  f.set_updated(0);
  f.set_skipped(0);
  f.set_vertex_count(0);
  f.set_vertex_buffer(0);
  f.set_frame_texture(0u);
  f.set_bool_0(1);
  f.set_bool_1(0);
  constexpr uint32_t kOne = 0x8200882C, kZero = 0x82000A34;
  f.set_color0_r(Single(base, kOne));
  f.set_color0_g(Single(base, kOne));
  f.set_color0_b(Single(base, kOne));
  f.set_color0_a(Single(base, kZero));
  f.set_color1_r(Single(base, kZero));
  f.set_color1_g(Single(base, kOne));
  f.set_color1_b(Single(base, kOne));
  f.set_color1_a(Single(base, kOne));
  f.set_strength(Single(base, kZero));
  f.set_c38_z(Single(base, kZero));
  f.set_c38_x(Single(base, kZero));
  f.set_frame_texture(RenderTexturesAcquire(base, stack, 1, 0x1A20015A, f.width(), f.height(),
                                            f.x(), f.y()));
  return filter;
}

uint32_t MotionBlurConstruct(uint8_t* base, uint32_t stack, uint32_t blur, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82136130 MotionBlur::MotionBlur");
  const uint32_t frame = stack - kMotionBlurFrame;
  ConstructBase(base, blur, entry);
  const schema::MotionBlur b(base, blur);
  constexpr uint32_t kOne = 0x8200882C;
  b.set_no_history(0);
  b.set_updated(0);
  b.set_reset(1);
  b.set_skipped(0);
  b.set_vertex_count(0);
  b.set_vertex_buffer(0);
  b.set_vtable(schema::MotionBlur::kVtable);
  b.set_velocity_texture(0u);
  b.set_frame_texture(0u);
  b.set_velocity_surface(0u);
  b.set_depth_surface(0u);
  // The two matrices' diagonals' last 1.0s, then both made the identity
  // (memcpy from the identity built in the frame at +112).
  for (uint32_t matrix : {b.view_projection_address(), b.previous_view_projection_address()}) {
    for (uint32_t row = 0; row < 4; row++) {
      CopySingle(base, matrix + 16 * row + 12, kOne);
    }
  }
  b.set_c40_x(Single(base, 0x82002A10));
  b.set_c40_y(Single(base, 0x82008810));
  b.set_c40_z(Single(base, 0x82008834));
  b.set_scale(Single(base, 0x82008828));
  if (b.camera_pass() != 0) {
    b.set_c40_x(Single(base, 0x82008834));
    b.set_c40_y(Single(base, 0x82002CBC));
    b.set_scale(Single(base, 0x820021F8));
  }
  const uint32_t identity = frame + 112;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreFloat(base, identity + 16 * row + 4 * column, row == column ? 1.0f : 0.0f);
    }
  }
  Require(c_runtime::Memcpy(base, b.view_projection_address(), identity, 64),
          "memcpy of the identity");
  Require(c_runtime::Memcpy(base, b.previous_view_projection_address(), identity, 64),
          "memcpy of the identity");
  auto width = [&]() { return b.width(); };
  auto height = [&]() { return b.height(); };
  b.set_velocity_texture(
      RenderTexturesAcquire(base, frame, 20, 0x2D200199, width() >> 1, height() >> 1, 0, 0));
  b.set_frame_texture(RenderTexturesAcquire(base, frame, 1, 0x1A20015A, width(), height(),
                                            b.x(), b.y()));
  // The first surface's parameters at frame + 96 (all 0), the second's at
  // frame + 80 (the EDRAM base).
  Store(base, frame + 96, 0);
  Store(base, frame + 100, 0);
  Store(base, frame + 104, 0);
  b.set_velocity_surface(
      graphics::CreateSurface(base, width() >> 1, height() >> 1, 0x2D20AB8D, 0, frame + 96));
  EdramParameters(base, frame + 80);
  b.set_depth_surface(
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
  const schema::PostEffectTableEntry table(base, G::effect_table_address() + index * 36);
  const uint32_t entry = table.address;
  const uint32_t kind = table.kind();
  static constexpr uint32_t kSizes[6] = {8480, 8112, 8128, 8224, 8068, 8068};
  if (kind <= 5) {
    const uint32_t at = G::effects_address() + (index << 2);
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
      app_start_up::CreateVertexDeclaration(base, G::vertex_elements_address(), calls.memcpy_call);
  Require(declaration.has_value(), "CreateVertexDeclaration");
  G::set_vertex_declaration(base, *declaration);
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
