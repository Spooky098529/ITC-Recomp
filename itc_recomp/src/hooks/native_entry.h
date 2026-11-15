// A hand port's entry in the dispatch table (docs/rexglue-removal.md,
// "Before E1: the rewiring", stage 3): what was its adapter's body, run on
// our register view (lifted::Regs, named `ctx` so the body reads as it did)
// made from a call's arguments, without the SDK's context.
//
//   ITC_NATIVE_ENTRY(820B8CE0, ITC_FLAG(native_vehicle_accessors)) {
//     ctx.r3.u64 = uint32_t(ContactOtherObject(base, ctx.r3.u32));
//   }
//
// defines the entry, registers it under the address with the flag that
// turns it on (off: lifted::Call runs the recompiled original through the
// SDK), and the REX_FUNC the SDK's function table still reaches (stage 4
// removes it): the original when the flag is off, else the entry through
// lifted::Enter.

#pragma once

#include "guest.h"
#include "lifted/regs.h"
#include "lifted/sdk.h"

#define ITC_NATIVE_ENTRY(address, flag)                                                  \
  static void itc_entry_body_##address(lifted::Regs& ctx, uint8_t* base);                \
  static lifted::Result itc_entry_##address(uint8_t* base, const lifted::Args& args) {    \
    lifted::Regs ctx(args);                                                              \
    itc_entry_body_##address(ctx, base);                                                 \
    return ctx.result();                                                                 \
  }                                                                                      \
  [[maybe_unused]] static const bool itc_entry_registered_##address =                    \
      lifted::RegisterEntry(0x##address, itc_entry_##address, &(flag));                  \
  extern "C" REX_FUNC(sub_##address) {                                                   \
    if (!(flag)) {                                                                       \
      __imp__sub_##address(ctx, base);                                                   \
      return;                                                                            \
    }                                                                                    \
    lifted::Enter(ctx, base, itc_entry_##address);                                       \
  }                                                                                      \
  static void itc_entry_body_##address([[maybe_unused]] lifted::Regs& ctx,               \
                                       [[maybe_unused]] uint8_t* base)
