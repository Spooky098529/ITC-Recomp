// The physical pad read (sub_82591C18), as an adapter from the recompiled
// Input::Update to our plain C++ read (src/input/physical_pad.cpp): it
// copies the pad's fields in from guest memory, reads, and copies them back
// big-endian, where the recompiled logical pads read the values. r3 = pad
// (212 bytes), r4 = pointer to the user index; nothing reads its result.
// Once Input::Update is native, it calls input::PhysicalPad_Read directly
// and this goes.

#include "guest.h"
#include "input/physical_pad.h"

namespace {

void LoadMotor(uint8_t* base, uint32_t address, input::PadMotor& motor) {
  motor.speed = guest::LoadFloat(base, address + 0);
  motor.held = int32_t(guest::LoadU32(base, address + 4));
  motor.ticks_left = int32_t(guest::LoadU32(base, address + 8));
}

void StoreMotor(uint8_t* base, uint32_t address, const input::PadMotor& motor) {
  guest::StoreFloat(base, address + 0, motor.speed);
  guest::StoreU32(base, address + 4, uint32_t(motor.held));
  guest::StoreU32(base, address + 8, uint32_t(motor.ticks_left));
}

void StoreGamepad(uint8_t* base, uint32_t address, const input::XInputGamepad& gamepad) {
  guest::StoreU16(base, address + 0, gamepad.buttons);
  guest::StoreU8(base, address + 2, gamepad.left_trigger);
  guest::StoreU8(base, address + 3, gamepad.right_trigger);
  guest::StoreU16(base, address + 4, uint16_t(gamepad.thumb_lx));
  guest::StoreU16(base, address + 6, uint16_t(gamepad.thumb_ly));
  guest::StoreU16(base, address + 8, uint16_t(gamepad.thumb_rx));
  guest::StoreU16(base, address + 10, uint16_t(gamepad.thumb_ry));
}

void Read(uint8_t* base, uint32_t address, uint32_t user) {
  input::PhysicalPad pad;
  pad.flags = guest::LoadU32(base, address + 4);
  LoadMotor(base, address + 168, pad.motors[0]);
  LoadMotor(base, address + 180, pad.motors[1]);
  bool caps_read = input::PhysicalPad_Read(pad, user);
  guest::StoreU32(base, address + 4, pad.flags);
  for (int i = 0; i < 40; i++) {
    guest::StoreFloat(base, address + 8 + 4 * i, pad.values[i]);
  }
  StoreMotor(base, address + 168, pad.motors[0]);
  StoreMotor(base, address + 180, pad.motors[1]);
  if (caps_read) {
    uint32_t caps = address + 192;
    guest::StoreU8(base, caps + 0, pad.caps.type);
    guest::StoreU8(base, caps + 1, pad.caps.sub_type);
    guest::StoreU16(base, caps + 2, pad.caps.flags);
    StoreGamepad(base, caps + 4, pad.caps.gamepad);
    guest::StoreU16(base, caps + 16, pad.caps.left_motor);
    guest::StoreU16(base, caps + 18, pad.caps.right_motor);
  }
}

}  // namespace

extern "C" REX_FUNC(sub_82591C18) {
  REX_FUNC_PROLOGUE();
  Read(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32));
}
