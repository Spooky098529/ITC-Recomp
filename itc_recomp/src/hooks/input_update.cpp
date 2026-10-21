// Input::Update (sub_820990F0) natively: the edge between the recompiled
// game and our plain C++ input (src/input/). In the recompiled order, it
//
// 1. reads the four physical pads (input::PhysicalPad_Read, replacing
//    sub_82591C18), copying each pad's fields in and out of guest memory;
// 2. builds the five logical pads (input::LogicalPad_*, replacing
//    sub_82591918) from their bound devices' values;
// 3. updates each processor and 4. each listener's first object
//    (input::InputProcessor_Update, replacing sub_82592188, for the class
//    every one of them has in a race; any other object is called).
//
// sub_82592188 is reached through a vtable, so its override below is an
// adapter for any recompiled caller.
//
// The manager and its pads stay in guest memory: the processors, the
// listeners and the action mappers read them there. See "Input::Update" in
// docs/pause-menu-fix.md for the layout.

#include "input_update.h"

#include "input/input_processor.h"
#include "input/logical_pad.h"
#include "input/physical_pad.h"

#include "base/log.h"

namespace input_hooks {
namespace {

constexpr uint32_t kLogicalPads = 8, kLogicalPadBytes = 196;
constexpr uint32_t kPhysicalPads = 988, kPhysicalPadBytes = 212;
constexpr uint32_t kProcessors = 1840, kProcessorBytes = 416;
constexpr uint32_t kListenerSlots = 3924, kListenerSlotBytes = 12;

// The devices' vtable slot 1 (their values' address) for the two kinds bound
// in every race (see "Step 0 results" in docs/pause-menu-fix.md), done here;
// any other device is called.
constexpr uint32_t kPhysicalPadValues = 0x820988B8;  // device + 8
constexpr uint32_t kLogicalPadValues = 0x82591B30;   // device + 36
// The processors' and listeners' slot 1 (vtable 0x8206C0C4).
constexpr uint32_t kProcessorUpdate = 0x82592188;

// The return addresses of the recompiled calls, for the guest code called.
constexpr uint32_t kReturnFromDeviceValues = 0x82591970;
constexpr uint32_t kReturnFromProcessor = 0x8209916C;
constexpr uint32_t kReturnFromListener = 0x820991C8;

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

// A physical pad (212 bytes): the fields PhysicalPad_Read touches, in and out.
void ReadPhysicalPad(uint8_t* base, uint32_t address, uint32_t user) {
  input::PhysicalPad pad;
  pad.flags = guest::LoadU32(base, address + 4);
  LoadMotor(base, address + 168, pad.motors[0]);
  LoadMotor(base, address + 180, pad.motors[1]);
  bool caps_read = input::PhysicalPad_Read(pad, user);
  guest::StoreU32(base, address + 4, pad.flags);
  for (int i = 0; i < input::kPadValues; i++) {
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

// A device's values' address: its vtable slot 1 (r3 = device).
uint32_t DeviceValues(PPCContext& ctx, uint8_t* base, uint32_t device) {
  uint32_t function = guest::LoadU32(base, guest::LoadU32(base, device) + 4);
  if (function == kPhysicalPadValues) {
    return device + 8;
  }
  if (function == kLogicalPadValues) {
    return device + 36;
  }
  ctx.r3.u64 = device;
  ctx.lr = kReturnFromDeviceValues;
  REX_CALL_INDIRECT_FUNC(function);
  return ctx.r3.u32;
}

// A logical pad (196 bytes): four bindings {u8 enabled at +4 + 8k, device at
// +8 + 8k}, values at +36.
void BuildLogicalPad(PPCContext& ctx, uint8_t* base, uint32_t pad) {
  float values[input::kPadValues] = {};
  for (uint32_t k = 0; k < 4; k++) {
    uint32_t device = guest::LoadU32(base, pad + 8 + 8 * k);
    if (!guest::LoadU8(base, pad + 4 + 8 * k) || !device) {
      continue;
    }
    float device_values[input::kPadValues];
    guest::CopyFloats(base, DeviceValues(ctx, base, device), device_values, input::kPadValues);
    input::LogicalPad_Add(values, device_values);
  }
  input::LogicalPad_Clamp(values);
  for (int i = 0; i < input::kPadValues; i++) {
    guest::StoreFloat(base, pad + 36 + 4 * i, values[i]);
  }
}

// A processor (416 bytes): the fields InputProcessor_Update reads, in, and
// those it writes, out.
void LoadProcessor(uint8_t* base, uint32_t object, input::InputProcessor& p) {
  guest::CopyFloats(base, object + 16, p.dead_zone, 4);
  p.held = guest::LoadU32(base, object + 272);
  p.repeat_delay = int32_t(guest::LoadU32(base, object + 276));
  p.repeat_period = int32_t(guest::LoadU32(base, object + 280));
  for (int i = 0; i < 32; i++) {
    p.held_ticks[i] = int32_t(guest::LoadU32(base, object + 284 + 4 * i));
  }
}

void StoreFloats(uint8_t* base, uint32_t address, const float* values, int count) {
  for (int i = 0; i < count; i++) {
    guest::StoreFloat(base, address + 4 * i, values[i]);
  }
}

void StoreProcessor(uint8_t* base, uint32_t object, const input::InputProcessor& p) {
  StoreFloats(base, object + 32, p.raw, 8);
  StoreFloats(base, object + 64, p.adjusted, 8);
  StoreFloats(base, object + 96, p.direction[0], 4);
  StoreFloats(base, object + 112, p.direction[1], 4);
  StoreFloats(base, object + 128, p.length, 2);
  StoreFloats(base, object + 136, p.buttons, 32);
  guest::StoreU32(base, object + 264, p.pressed);
  guest::StoreU32(base, object + 268, p.released);
  guest::StoreU32(base, object + 272, p.held);
  for (int i = 0; i < 32; i++) {
    guest::StoreU32(base, object + 284 + 4 * i, uint32_t(p.held_ticks[i]));
  }
}

// sub_82592188 natively (r3 = object, r4 = its logical pad). Returns the
// pad's values' address, which the original leaves in r3. The original
// stores the directions with stvx, which drops the address's low 4 bits;
// objects are 16-byte aligned, and any that isn't is left to the original.
bool UpdateProcessor(PPCContext& ctx, uint8_t* base, uint32_t object, uint32_t pad,
                     uint32_t* values_address) {
  if (object & 0xF) {
    return false;
  }
  *values_address = DeviceValues(ctx, base, pad);
  float values[input::kPadValues];
  guest::CopyFloats(base, *values_address, values, input::kPadValues);
  input::InputProcessor processor;
  LoadProcessor(base, object, processor);
  input::InputProcessor_Update(processor, values);
  StoreProcessor(base, object, processor);
  return true;
}

// Calls `object`'s vtable slot 1 with r4 = its logical pad.
void UpdateWithPad(PPCContext& ctx, uint8_t* base, uint32_t object, uint32_t pad,
                   uint32_t return_address) {
  uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object) + 4);
  uint32_t values_address;
  if (function == kProcessorUpdate && UpdateProcessor(ctx, base, object, pad, &values_address)) {
    return;
  }
  ctx.r3.u64 = object;
  ctx.r4.u64 = pad;
  ctx.lr = return_address;
  REX_CALL_INDIRECT_FUNC(function);
}

}  // namespace

void InputUpdate_Run(PPCContext& ctx, uint8_t* base) {
  uint32_t manager = ctx.r3.u32;
  uint64_t saved_lr = ctx.lr;
  // The recompiled function's frame, so the guest code called finds the
  // stack as it did.
  guest::StackFrame frame(ctx, 144);

  for (uint32_t i = 0; i < 4; i++) {
    ReadPhysicalPad(base, manager + kPhysicalPads + kPhysicalPadBytes * i, i);
  }
  for (uint32_t j = 0; j < 5; j++) {
    BuildLogicalPad(ctx, base, manager + kLogicalPads + kLogicalPadBytes * j);
  }
  for (uint32_t j = 0; j < 5; j++) {
    UpdateWithPad(ctx, base, manager + kProcessors + kProcessorBytes * j,
                  manager + kLogicalPads + kLogicalPadBytes * j, kReturnFromProcessor);
  }
  for (uint32_t j = 0; j < 5; j++) {
    // A list {head, count}; only the first node's object is updated.
    uint32_t slot = manager + kListenerSlots + kListenerSlotBytes * j;
    if (!guest::LoadU32(base, slot + 4)) {
      continue;
    }
    uint32_t head = guest::LoadU32(base, slot);
    uint32_t node = guest::LoadU32(base, head + 4);
    if (node == head) {
      // An empty list with a count; the game traps here (twi 31).
      ITC_LOG_ERROR("Input::Update: listener slot {} has a count but no node", j);
      continue;
    }
    UpdateWithPad(ctx, base, guest::LoadU32(base, node + 8),
                  manager + kLogicalPads + kLogicalPadBytes * j, kReturnFromListener);
  }
  ctx.lr = saved_lr;
}

}  // namespace input_hooks

extern "C" REX_FUNC(sub_82592188) {
  REX_FUNC_PROLOGUE();
  uint32_t values_address;
  if (input_hooks::UpdateProcessor(ctx, base, ctx.r3.u32, ctx.r4.u32, &values_address)) {
    ctx.r3.u64 = values_address;
  } else {
    __imp__sub_82592188(ctx, base);
  }
}
