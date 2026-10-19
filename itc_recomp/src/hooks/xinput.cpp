// The game's XInput wrappers, as adapters from the recompiled code to our
// own XInput (src/input/xinput.cpp): each takes its arguments from the
// registers, calls the plain function, stores the struct big-endian in guest
// memory and returns the code in r3. See docs/rexglue-removal.md, B1.
//
// On the 360 each wrapper sets up its arguments and jumps to a kernel
// import. Their only caller is the physical pad's read (sub_82591C18, once a
// tick per pad from Input::Update; see "Physical pad" in
// docs/pause-menu-fix.md); once that is native, it calls input::XInput_*
// directly and these go.

#include "guest.h"
#include "input/xinput.h"

namespace {

void StoreGamepad(uint8_t* base, uint32_t address, const input::XInputGamepad& gamepad) {
  guest::StoreU16(base, address + 0, gamepad.buttons);
  guest::StoreU8(base, address + 2, gamepad.left_trigger);
  guest::StoreU8(base, address + 3, gamepad.right_trigger);
  guest::StoreU16(base, address + 4, uint16_t(gamepad.thumb_lx));
  guest::StoreU16(base, address + 6, uint16_t(gamepad.thumb_ly));
  guest::StoreU16(base, address + 8, uint16_t(gamepad.thumb_rx));
  guest::StoreU16(base, address + 10, uint16_t(gamepad.thumb_ry));
}

}  // namespace

// XInputGetCapabilities: r3 = user, r4 = flags, r5 = X_INPUT_CAPABILITIES
// (20 bytes).
extern "C" REX_FUNC(sub_825BA3F0) {
  REX_FUNC_PROLOGUE();
  uint32_t out = ctx.r5.u32;
  input::XInputCapabilities caps;
  uint32_t result = input::XInput_GetCapabilities(ctx.r3.u32, ctx.r4.u32, out ? &caps : nullptr);
  if (result == input::kXInputSuccess) {
    guest::StoreU8(base, out + 0, caps.type);
    guest::StoreU8(base, out + 1, caps.sub_type);
    guest::StoreU16(base, out + 2, caps.flags);
    StoreGamepad(base, out + 4, caps.gamepad);
    guest::StoreU16(base, out + 16, caps.left_motor);
    guest::StoreU16(base, out + 18, caps.right_motor);
  }
  ctx.r3.u64 = result;
}

// XInputGetState: r3 = user, r4 = X_INPUT_STATE (16 bytes; may be 0). The
// import's flags are 0.
extern "C" REX_FUNC(sub_825BA3F8) {
  REX_FUNC_PROLOGUE();
  uint32_t out = ctx.r4.u32;
  input::XInputState state;
  uint32_t result = input::XInput_GetState(ctx.r3.u32, out ? &state : nullptr);
  if (result == input::kXInputSuccess && out) {
    guest::StoreU32(base, out + 0, state.packet_number);
    StoreGamepad(base, out + 4, state.gamepad);
  }
  ctx.r3.u64 = result;
}

// XInputSetState: r3 = user, r4 = X_INPUT_VIBRATION (left and right motor
// speeds, u16 each).
extern "C" REX_FUNC(sub_825BA408) {
  REX_FUNC_PROLOGUE();
  uint32_t vibration = ctx.r4.u32;
  ctx.r3.u64 = vibration ? input::XInput_SetState(ctx.r3.u32, guest::LoadU16(base, vibration),
                                                  guest::LoadU16(base, vibration + 2))
                         : input::kXInputBadArguments;
}
