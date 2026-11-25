// The front end's menu helpers natively: adapters in front of
// src/menu_helpers/ (docs/functions/menu-helpers.md), with the register
// steps the harness shares (hooks/menu_helpers.h). About 113 recompiled
// front-end screens call them, so the adapters stay.

#include "hooks/menu_helpers.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "menu_helpers/menu_helpers.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_menu_helpers, true, "ITC",
              "Run the front end's menu helpers (docs/functions/menu-helpers.md: Menu::Step "
              "sub_82186568 and Menu::Pressed sub_821864C8; the player's action mapper's read "
              "slots 10 and 12 by ActionMapper::Pressed's and Fired's plain C++ with "
              "--native_action_mappers, else through the vtable) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=menu_helpers)");
ITC_FLAG_DECLARE(bool, native_action_mappers);

namespace hooks {

namespace {

// The guest calls the plain C++ makes (menu_helpers::Calls), through this
// context: r1, r3, r4 and lr set, the function run through the function
// table (its override when it has one), r1 put back. The adapter's lr is put
// back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  menu_helpers::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, uint64_t r3, uint64_t r4, uint32_t r1,
                       uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    ctx.lr = lr;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  lifted::Regs& ctx_;
  uint64_t lr_;
};

bool NativeSlots() { return ITC_FLAG(native_action_mappers); }

}  // namespace

ITC_REGS_STEP(MenuStep) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = menu_helpers::Step(ctx.r3.u32, ctx.r1.u32, NativeSlots(), guest.calls());
}

ITC_REGS_STEP(MenuPressed) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      menu_helpers::Pressed(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, NativeSlots(), guest.calls());
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_MENU_HELPERS(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_menu_helpers)) { \
    hooks::step(ctx);               \
  }

ITC_MENU_HELPERS(82186568, MenuStep)
ITC_MENU_HELPERS(821864C8, MenuPressed)
