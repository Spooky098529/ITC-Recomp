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
// them, and the flag switches it.

#include "guest.h"

#include "base/flags.h"
#include "hooks/guest_call.h"
#include "post_effects/post_effects.h"

#include <optional>

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
              "sub_82133F58, without the command list's diagnostics) natively instead of through the "
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
ITC_POST_EFFECTS(82133A70, uint32_t r3 = 0;
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
                 ctx.r3.u64 = r3)

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
                 const double speed = ctx.f1.f64;
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
                 }))

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
