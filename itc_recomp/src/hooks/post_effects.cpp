// The post effects natively: adapters in front of src/post_effects/
// (docs/functions/post-effects.md). Each leaves r3 as the original does.
// Flush-to-zero is left off, as the translated code leaves it. The setters
// are vtable slots (the glow's 2-12 and 24, the color filter's 4 and 6-8),
// so their adapters stay. Glow::Record (sub_821379D8) is frame
// interpolation's wrap (src/interpolation/glow.cpp), MotionBlur::Record
// (sub_821363F0) is recompiled with interpolation's notes at its return
// 8213653C, and PostEffects::RecordAll (sub_82133960) is the diagnostics'
// rewrite (src/debug/frame_state_diff.cpp): none of them is here.
// PostEffects::ToggleNoHistory (sub_82133A38) and ScreenObject::ToggleHidden
// (sub_8212EDE0) have no adapters: only Race::ToggleEffects' plain C++ calls
// them, and the flag switches it. StarDef::Init (sub_8212EA98) has none
// either: only GlareDef::Init's plain C++ calls it; nor RenderTexture::Release
// (sub_82133070): only RenderTextures::Release's plain C++ calls it; nor
// RenderTextures::Release (sub_821332F0), PostEffect::ReleaseSurface
// (sub_82133C60) and the destructors Glow::~Glow (sub_82136E30),
// ColorFilter::~ColorFilter (sub_821351A8) and MotionBlur::~MotionBlur
// (sub_82136390): only the destructors' and the deleting destructors' plain
// C++ calls them (the first two through hooks::PostEffectsReleaseTexture and
// PostEffectsReleaseSurface, which take the original's path where theirs
// isn't native).

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/post_effects.h"
#include "post_effects/post_effects.h"

#include <optional>

ITC_FLAG_DECLARE(bool, native_video_output);
ITC_FLAG_DECLARE(bool, native_graphics_resources);
ITC_FLAG_DECLARE(bool, native_c_runtime_heap);

ITC_FLAG_BOOL(native_post_effects, true, "ITC",
              "Run the post effects (docs/functions/post-effects.md: PostEffects::Get "
              "sub_82133AE8, MarkUpdated sub_82133A08, ToggleNoHistory sub_82133A38 (no adapter: only "
              "Race::ToggleEffects' plain C++ calls it, and the flag switches that), DestroyRace "
              "sub_82133A70, Race::ToggleEffects sub_8215B690 with ScreenObject::ToggleHidden "
              "sub_8212EDE0 (no adapter, the same), the glow's setters sub_82136EF8 to sub_82136F48 and "
              "Glow::SetFromSpeed sub_82133878, the color filter's setters sub_821350F8, "
              "sub_82135138, sub_82135140 and sub_82135148; the recording helpers "
              "PostEffect::RecordBeginStates sub_82133340, RecordEndStates sub_82133608, "
              "RecordTargets sub_82133790, RecordPixelConstants sub_82133D00, RecordPixelBool "
              "sub_82133D70, RecordTexelSize sub_82133DD8, AddQuad sub_82133E30 and RecordResolve "
              "sub_82133F58, without the command list's diagnostics; the glare's GlareDef::GlareDef "
              "sub_8212ED78, GlareDef::Init sub_8212EC90 and StarDef::Init sub_8212EA98 (no adapter: only "
              "GlareDef::Init's plain C++ calls it, and the flag switches that); "
              "RenderTexture::Release sub_82133070 (no adapter: only RenderTextures::Release's "
              "plain C++ calls it, and the flag switches that) and RenderTextures::Release sub_821332F0 (with "
              "--native_heap and --native_c_runtime_heap), PostEffect::ReleaseSurface sub_82133C60 "
              "(with --native_video_output) and TargetFilter::TargetFilter sub_82135AA8 (with "
              "--native_graphics_resources and --native_heap); the destructors Glow::~Glow "
              "sub_82136E30, ColorFilter::~ColorFilter sub_821351A8 and MotionBlur::~MotionBlur "
              "sub_82136390 (no adapters, nor for RenderTextures::Release and ReleaseSurface: only "
              "the deleting destructors' and the destructors' plain C++ call them, and the flag "
              "switches those) and their deleting ones sub_821389C8, sub_82138570 and sub_821385C0 "
              "(operator delete with --native_c_runtime_heap)) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=post_effects)");

#define ITC_POST_EFFECTS(address, ...)    \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_post_effects)) { \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    __VA_ARGS__;                          \
  }

using namespace post_effects;

// --- The effects: no arguments but Get's r3 = the index ---

ITC_POST_EFFECTS(82133AE8, ctx.r3.u64 = Get(base, ctx.r3.u32))
ITC_POST_EFFECTS(82133A08, MarkUpdated(base))
// The deleting destructors through the effects' slot 1 (r4 = 1), from the
// original's 112-byte frame.
ITC_POST_EFFECTS(82133A70, uint32_t r3 = 0; const uint64_t lr = ctx.lr;
                 guest_call::InFrame(ctx, base, 112, 0x82133ABC, [&](uint32_t) {
                   r3 = DestroyRace(base, [&](uint32_t function, uint32_t effect) {
                     ctx.r3.u64 = effect;
                     ctx.r4.u64 = 1;
                     ctx.lr = 0x82133ABC;
                     ctx.ctr.u64 = function;
                     REX_CALL_INDIRECT_FUNC(function);
                     return ctx.r3.u32;
                   });
                 });
                 ctx.lr = lr; ctx.r3.u64 = r3)

// r3 = the race object.
ITC_POST_EFFECTS(8215B690, ctx.r3.u64 = ToggleRaceEffects(base, ctx.r3.u32))

// --- The glow's setters: r3 = the glow, f1 = the value ---

#define ITC_GLOW_SET(address, offset)                   \
  extern "C" REX_FUNC(sub_##address) {                  \
    REX_FUNC_PROLOGUE();                                \
    if (!ITC_FLAG(native_post_effects)) {               \
      __imp__sub_##address(ctx, base);                  \
      return;                                           \
    }                                                   \
    ctx.fpscr.disableFlushMode();                       \
    GlowSet(base, ctx.r3.u32, offset, ctx.f1.f64);      \
  }

ITC_GLOW_SET(82136EF8, 8436)
ITC_GLOW_SET(82136F00, 8440)
ITC_GLOW_SET(82136F08, 8444)
ITC_GLOW_SET(82136F10, 8448)
ITC_GLOW_SET(82136F18, 8452)
ITC_GLOW_SET(82136F20, 8456)
ITC_GLOW_SET(82136F28, 8460)
ITC_GLOW_SET(82136F30, 8464)
ITC_GLOW_SET(82136F38, 8468)
ITC_GLOW_SET(82136F40, 8472)
ITC_GLOW_SET(82136F48, 8476)

// Glow::SetFromSpeed: f1 = the speed. Its three slots are called from the
// original's 112-byte frame; r3 and f1 come back as the last one leaves them.
ITC_POST_EFFECTS(82133878, ctx.fpscr.disableFlushMode(); const uint32_t glow = ctx.r3.u32;
                 const double speed = ctx.f1.f64; const uint64_t lr = ctx.lr;
                 guest_call::InFrame(ctx, base, 112, 0x821338FC, [&](uint32_t) {
                   static constexpr uint32_t kReturns[3] = {0x821338FC, 0x82133920, 0x82133944};
                   int call = 0;
                   GlowSetFromSpeed(base, glow, speed, [&](uint32_t function, double value) {
                     ctx.r3.u64 = glow;
                     ctx.f1.f64 = value;
                     ctx.lr = kReturns[call++];
                     ctx.ctr.u64 = function;
                     REX_CALL_INDIRECT_FUNC(function);
                   });
                 });
                 ctx.lr = lr)

// --- The color filter's setters: r3 = the filter ---

// f1, f2, f3 = the color.
ITC_POST_EFFECTS(821350F8, ctx.fpscr.disableFlushMode();
                 ColorFilterSetColor0(base, ctx.r3.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64))
ITC_POST_EFFECTS(82135138, ctx.fpscr.disableFlushMode();
                 ColorFilterSet(base, ctx.r3.u32, 8112, ctx.f1.f64))
ITC_POST_EFFECTS(82135140, ctx.fpscr.disableFlushMode();
                 ColorFilterSet(base, ctx.r3.u32, 8116, ctx.f1.f64))
ITC_POST_EFFECTS(82135148, ctx.fpscr.disableFlushMode();
                 ColorFilterSet(base, ctx.r3.u32, 8120, ctx.f1.f64))

// --- The recording helpers: r3 = the effect, r4 = the command list ---

// Runs the original when the plain C++ has no answer (the list full).
#define ITC_POST_EFFECTS_RECORD(address, ...)                 \
  extern "C" REX_FUNC(sub_##address) {                        \
    REX_FUNC_PROLOGUE();                                      \
    std::optional<uint32_t> r3;                               \
    if (ITC_FLAG(native_post_effects)) {                      \
      r3 = __VA_ARGS__;                                       \
    }                                                         \
    if (!r3) {                                                \
      __imp__sub_##address(ctx, base);                        \
      return;                                                 \
    }                                                         \
    ctx.r3.u64 = *r3;                                         \
  }

ITC_POST_EFFECTS_RECORD(82133340, RecordBeginStates(base, ctx.r4.u32))
ITC_POST_EFFECTS_RECORD(82133608, RecordEndStates(base, ctx.r4.u32))
// r5 = the surface, r6 = the depth surface.
ITC_POST_EFFECTS_RECORD(82133790, RecordTargets(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
// r5 = the 16 bytes, r6 = the register, r7 = the count.
ITC_POST_EFFECTS_RECORD(82133D00,
                        RecordPixelConstants(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32))
// r5 = the value, r6 = the register.
ITC_POST_EFFECTS_RECORD(82133D70, RecordPixelBool(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
// f1 = width, f2 = height.
ITC_POST_EFFECTS_RECORD(82133DD8, (ctx.fpscr.disableFlushMode(),
                                   RecordTexelSize(base, ctx.r4.u32, ctx.f1.f64, ctx.f2.f64)))
// r5 = the texture, r6, r7 = the flags, r8 = by the view's rectangle.
ITC_POST_EFFECTS_RECORD(82133F58, RecordResolve(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                ctx.r6.u32, ctx.r7.u32, ctx.r8.u32 != 0))

// PostEffect::AddQuad: r4 = width, r5 = height, f1 = the value, r7 = at the
// view's position.
ITC_POST_EFFECTS(82133E30, ctx.fpscr.disableFlushMode();
                 ctx.r3.u64 = AddQuad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64,
                                      ctx.r7.u32 != 0))

// --- The glare's definition ---

// r3 = the glare definition.
ITC_POST_EFFECTS(8212ED78, ctx.fpscr.disableFlushMode(); GlareDefConstruct(base, ctx.r3.u32))
// r4 = the glare type.
ITC_POST_EFFECTS(8212EC90, ctx.fpscr.disableFlushMode();
                 ctx.r3.u64 = GlareDefInit(base, ctx.r3.u32, ctx.r4.u32))

// --- The render textures and surfaces ---

namespace {

// Whether operator delete and XPhysicalFree are the layer's.
bool FreesNative() {
  return ITC_FLAG(native_c_runtime_heap) && hooks::NativeHeapActive();
}

// The video output, as the device's users reach it: `[0x8266D088]`'s vtable
// slot 7, then that object's slot 3, as guest calls with the original's
// return addresses. r3 = the video output after.
uint32_t VideoOutputCall(PPCContext& ctx, uint8_t* base, uint32_t first, uint32_t second) {
  ctx.r3.u64 = guest::LoadU32(base, 0x8266D088);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 28);
  ctx.lr = first;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 12);
  ctx.lr = second;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  return ctx.r3.u32;
}

// RenderTextures::Release (r3 = the address of an effect's entry pointer)
// natively, or the original where the frees aren't native; its adapter's
// body, which the destructors' adapters call too.
}  // namespace

namespace hooks {

void PostEffectsReleaseTexture(PPCContext& ctx, uint8_t* base) {
  std::optional<uint32_t> r3;
  if (FreesNative()) {
    r3 = RenderTexturesRelease(base, ctx.r3.u32);
  }
  if (!r3) {
    __imp__sub_821332F0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

// PostEffect::ReleaseSurface (r3 = the address of an effect's surface
// pointer) natively, its guest calls from a frame like the original's (112
// bytes), or the original without the native device lock; its adapter's
// body, as ReleaseTexture.
void PostEffectsReleaseSurface(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_video_output)) {
    __imp__sub_82133C60(ctx, base);
    return;
  }
  const uint32_t at = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  guest_call::InFrame(ctx, base, 112, 0x82133C90, [&](uint32_t frame) {
    const SurfaceCalls calls{
        [&](int which) {
          return which == 0 ? VideoOutputCall(ctx, base, 0x82133C90, 0x82133CA0)
                            : VideoOutputCall(ctx, base, 0x82133CCC, 0x82133CDC);
        },
        [&](uint32_t resource) {
          ctx.r3.u64 = resource;
          ctx.lr = 0x82133CB0;
          sub_822A67B0(ctx, base);
        }};
    r3 = ReleaseSurface(base, frame, at, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

// The destructors' calls: the two above, r3 = the field's address.
DestroyCalls PostEffectsDestroyCalls(PPCContext& ctx, uint8_t* base) {
  return {[&ctx, base](uint32_t at) {
            ctx.r3.u64 = at;
            PostEffectsReleaseSurface(ctx, base);
            return ctx.r3.u32;
          },
          [&ctx, base](uint32_t at) {
            ctx.r3.u64 = at;
            PostEffectsReleaseTexture(ctx, base);
            return ctx.r3.u32;
          }};
}

}  // namespace hooks

// TargetFilter::TargetFilter: r3 = the object, r4 = the table entry. The
// surface's parameters in a frame like the original's (112 bytes).
extern "C" REX_FUNC(sub_82135AA8) {
  REX_FUNC_PROLOGUE();
  std::optional<uint32_t> r3;
  if (ITC_FLAG(native_post_effects) && ITC_FLAG(native_graphics_resources) &&
      hooks::NativeHeapActive()) {
    const uint32_t filter = ctx.r3.u32;
    const uint32_t entry = ctx.r4.u32;
    const uint64_t lr = ctx.lr;
    guest_call::InFrame(ctx, base, 112, 0x82135AD4, [&](uint32_t frame) {
      r3 = TargetFilterConstruct(base, frame, filter, entry);
    });
    ctx.lr = lr;
  }
  if (!r3) {
    __imp__sub_82135AA8(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

// --- The destructors ---

// The deleting destructors (vtable slot 1): r4 bit 0 = delete. The
// original where the delete isn't native.
#define ITC_POST_EFFECT_DELETE(address, kind)                                          \
  extern "C" REX_FUNC(sub_##address) {                                                 \
    REX_FUNC_PROLOGUE();                                                               \
    if (!ITC_FLAG(native_post_effects) || ((ctx.r4.u32 & 1) != 0 && !FreesNative())) { \
      __imp__sub_##address(ctx, base);                                                 \
      return;                                                                          \
    }                                                                                  \
    const uint64_t lr = ctx.lr;                                                        \
    const uint32_t effect = ctx.r3.u32;                                                \
    const uint32_t flags = ctx.r4.u32;                                                 \
    std::optional<uint32_t> r3;                                                        \
    guest_call::InFrame(ctx, base, 112, 0, [&](uint32_t) {                             \
      r3 = DeleteEffect(base, kind, effect, flags, hooks::PostEffectsDestroyCalls(ctx, base));       \
    });                                                                                \
    ctx.lr = lr;                                                                       \
    if (!r3) {                                                                         \
      __imp__sub_##address(ctx, base);                                                 \
      return;                                                                          \
    }                                                                                  \
    ctx.r3.u64 = *r3;                                                                  \
  }

ITC_POST_EFFECT_DELETE(821389C8, EffectKind::kGlow)
ITC_POST_EFFECT_DELETE(82138570, EffectKind::kColorFilter)
ITC_POST_EFFECT_DELETE(821385C0, EffectKind::kMotionBlur)
