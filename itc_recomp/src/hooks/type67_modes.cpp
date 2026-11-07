// The type 6 and 7 modes natively: adapters in front of src/type67_modes/
// (docs/functions/type6-type7-modes.md), with the register steps the
// harness shares (hooks/type67_modes.h). The state machine Inits are reached
// from the modes' slot 0 (recompiled) and through the screens' vtables, so
// their adapters stay.

#include "hooks/type67_modes.h"

#include "base/flags.h"
#include "type67_modes/type67_modes.h"

ITC_FLAG_BOOL(native_type67_modes, true, "ITC",
              "Run the type 6 and 7 modes (docs/functions/type6-type7-modes.md: the state "
              "machine Inits Type6Screen0::Init sub_82235138, Type7Screen0::Init sub_822454B8, "
              "Type6Dialog::Init sub_822329A8, Type7Dialog::Init sub_82242420, "
              "Type6ControllerOptions::Init sub_821C9918, Type7ControllerOptions::Init "
              "sub_821C9430, Type6Options::Init sub_821CB6E8, Type7Options::Init sub_821CB090, "
              "Type6SaveScreen::Init sub_821A8210, Type6ListScreen::InitMachine sub_8223F9A8, "
              "Type7ListScreen::InitMachine sub_822436A0, Type6PanelScreen::InitMachine "
              "sub_82230F80, Type7PanelScreen::InitMachine sub_82240998, "
              "Type6NodeScreen::InitMachine sub_82237320, PowerGraphScreen6::InitMachine "
              "sub_82237B78 and PowerGraphScreen7::InitMachine sub_822470A8, over "
              "StateMachine::Add's plain C++, their frames' stores replayed) natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=type67_modes)");

namespace hooks {

namespace t67 = type67_modes;

bool Type67ModesNative() {
  return ITC_FLAG(native_type67_modes);
}

void Type67Init(PPCContext& ctx, uint8_t* base, uint32_t address) {
  uint32_t frame_bytes = 0;
  for (const t67::InitInfo& info : t67::kInits) {
    if (info.address == address) {
      frame_bytes = info.frame;
    }
  }
  const uint64_t object_r3 = ctx.r3.u64;
  const t67::InitResult result =
      t67::RunInit(base, address, ctx.r3.u32, ctx.r1.u32 - frame_bytes);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  const uint64_t machine_r3 = object_r3 + (result.machine - ctx.r3.u32);
  ctx.r3.u64 = result.replaced ? result.r3 : machine_r3 + (result.r3 - result.machine);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_TYPE67_MODES(address, step, ...)  \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_type67_modes)) {     \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base, ##__VA_ARGS__);    \
  }

ITC_TYPE67_MODES(82235138, Type67Init, 0x82235138)
ITC_TYPE67_MODES(822454B8, Type67Init, 0x822454B8)
ITC_TYPE67_MODES(822329A8, Type67Init, 0x822329A8)
ITC_TYPE67_MODES(82242420, Type67Init, 0x82242420)
ITC_TYPE67_MODES(821C9918, Type67Init, 0x821C9918)
ITC_TYPE67_MODES(821C9430, Type67Init, 0x821C9430)
ITC_TYPE67_MODES(821CB6E8, Type67Init, 0x821CB6E8)
ITC_TYPE67_MODES(821CB090, Type67Init, 0x821CB090)
ITC_TYPE67_MODES(821A8210, Type67Init, 0x821A8210)
ITC_TYPE67_MODES(8223F9A8, Type67Init, 0x8223F9A8)
ITC_TYPE67_MODES(822436A0, Type67Init, 0x822436A0)
ITC_TYPE67_MODES(82230F80, Type67Init, 0x82230F80)
ITC_TYPE67_MODES(82240998, Type67Init, 0x82240998)
ITC_TYPE67_MODES(82237320, Type67Init, 0x82237320)
ITC_TYPE67_MODES(82237B78, Type67Init, 0x82237B78)
ITC_TYPE67_MODES(822470A8, Type67Init, 0x822470A8)
