// A hand port's entry in the dispatch table (docs/rexglue-removal.md,
// "Before E1: the rewiring", stages 3 and 4): what was its adapter's body,
// run on our register view (lifted::Regs, named `ctx` so the body reads as
// it did) made from a call's arguments, without the SDK's context.
//
//   ITC_NATIVE_ENTRY(820B8CE0, ITC_FLAG(native_vehicle_accessors)) {
//     ctx.r3.u64 = uint32_t(ContactOtherObject(base, ctx.r3.u32));
//   }
//
// defines the entry and registers it under the address. The flag that once
// turned it on (off: the recompiled original) is no longer read: there's no
// recompiled code since the rewiring's stage 5. No override in the SDK's
// function table since stage 4: native code reaches it through the
// dispatch table (lifted::Call).
#pragma once

#include "guest.h"
#include "lifted/regs.h"
#include "lifted/sdk.h"

#define ITC_NATIVE_ENTRY_DEFINE(address, enabled)                                        \
  static void itc_entry_body_##address(lifted::Regs& ctx, uint8_t* base);                \
  static lifted::Result itc_entry_##address(uint8_t* base, const lifted::Args& args) {    \
    lifted::Regs ctx(args);                                                              \
    itc_entry_body_##address(ctx, base);                                                 \
    return ctx.result();                                                                 \
  }                                                                                      \
  [[maybe_unused]] static const bool itc_entry_registered_##address =                    \
      lifted::RegisterEntry(0x##address, itc_entry_##address, (enabled));

#define ITC_NATIVE_ENTRY_BODY(address)                                                   \
  static void itc_entry_body_##address([[maybe_unused]] lifted::Regs& ctx,               \
                                       [[maybe_unused]] uint8_t* base)

#define ITC_NATIVE_ENTRY(address, flag)                                                  \
  ITC_NATIVE_ENTRY_DEFINE(address, &(flag))                                              \
  ITC_NATIVE_ENTRY_BODY(address)

// The same for an adapter whose body checks its own conditions (a flag
// among others, a diagnostic's skip): registered always on, the body's
// fallbacks to the original calling it through lifted::Sdk.
#define ITC_NATIVE_ENTRY_ALWAYS(address)                                                 \
  ITC_NATIVE_ENTRY_DEFINE(address, &lifted::kAlwaysOn)                                   \
  ITC_NATIVE_ENTRY_BODY(address)

// An entry our threads start at (a thread's start routine: the list in
// "The kernel's entries"; once also an override in the SDK's function
// table, gone with the SDK's context in E4): an entry like any other.
#define ITC_KERNEL_ENTRY(address, flag) ITC_NATIVE_ENTRY(address, flag)
#define ITC_KERNEL_ENTRY_ALWAYS(address) ITC_NATIVE_ENTRY_ALWAYS(address)

// A step on our register view (an adapter's body split out):
// `name(lifted::Regs&, uint8_t*)`. Followed by the body. (It once also
// defined a form on the SDK's context for the harnesses: gone in E4.)
#define ITC_REGS_STEP(name) \
  void name([[maybe_unused]] lifted::Regs& ctx, [[maybe_unused]] uint8_t* base)

// A step or an original named as a value (a table, a ternary, a
// std::function): its form on our register view.
#define ITC_REGS_FN(name) static_cast<void (*)(lifted::Regs&, uint8_t*)>(&name)
