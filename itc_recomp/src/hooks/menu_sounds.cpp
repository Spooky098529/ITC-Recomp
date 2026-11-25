// The menu sounds natively: adapters in front of src/menu_sounds/
// (docs/functions/menu-sounds.md), with the register steps the harness
// shares (hooks/menu_sounds.h). About 270 recompiled functions call
// MenuSound::Play and 29 Sound::Play, so the adapters stay.

#include "hooks/menu_sounds.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "menu_sounds/menu_sounds.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_menu_sounds, true, "ITC",
              "Run the menu sounds (docs/functions/menu-sounds.md: MenuSound::Play sub_821103F0 "
              "and Sound::Play sub_82175850, the sound manager's slot 10 (PlayCue2D) through the "
              "function table; SoundOptions::SetEffectsVolume sub_821B9C20 and SetMusicVolume "
              "sub_821B9CF0, its slots 29 and 30 (GetCategory, SetCategoryVolume) through the "
              "function table) natively instead of through the translated code (the same words; "
              "checked by --run_harnesses=menu_sounds)");

namespace hooks {

namespace {

// The guest calls the plain C++ makes (menu_sounds::Calls), through this
// context: r1, the first `set` of r3 to r6 and lr set, the function run through the function
// table (its override when it has one), r1 put back. The adapter's lr is
// put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  menu_sounds::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[4], int set,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    lifted::Temp* registers[4] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6};
    for (int i = 0; i < set; i++) {
      registers[i]->u64 = args[i];
    }
    ctx.lr = lr;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  lifted::Regs& ctx_;
  uint64_t lr_;
};

}  // namespace

ITC_REGS_STEP(MenuSoundPlay) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = menu_sounds::PlayMenuSound(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(SoundPlay) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = menu_sounds::PlaySound(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(SoundOptionsSetEffectsVolume) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = menu_sounds::SetEffectsVolume(ctx.r3.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(SoundOptionsSetMusicVolume) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = menu_sounds::SetMusicVolume(ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_MENU_SOUNDS(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_menu_sounds)) { \
    hooks::step(ctx);              \
  }

ITC_MENU_SOUNDS(821103F0, MenuSoundPlay)
ITC_MENU_SOUNDS(82175850, SoundPlay)
ITC_MENU_SOUNDS(821B9C20, SoundOptionsSetEffectsVolume)
ITC_MENU_SOUNDS(821B9CF0, SoundOptionsSetMusicVolume)
