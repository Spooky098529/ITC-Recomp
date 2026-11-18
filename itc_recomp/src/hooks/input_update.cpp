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
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "input/input_processor.h"
#include "input/logical_pad.h"
#include "input/physical_pad.h"

#include "base/log.h"
#include "lifted/sdk.h"
#include "schema/input.h"

namespace input_hooks {
namespace {

// The physical pads (model-animation.toml's PhysicalPad) and the listener
// lists (app-states.toml's PtrList) by offset until those types are switched.
constexpr uint32_t kPhysicalPadBytes = 212;
constexpr uint32_t kListenerSlotBytes = 12;

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
uint32_t DeviceValues(lifted::Regs& ctx, uint8_t* base, uint32_t device) {
  // Its vtable: its first word (a logical or a physical pad's).
  uint32_t function = schema::InputDeviceVtable(base, guest::LoadU32(base, device)).values();
  if (function == kPhysicalPadValues) {
    return device + 8;
  }
  if (function == kLogicalPadValues) {
    return schema::LogicalPad(base, device).values_address();
  }
  ctx.r3.u64 = device;
  ctx.lr = kReturnFromDeviceValues;
  lifted::CallFromContext(ctx, base, function);
  return ctx.r3.u32;
}

// A logical pad (196 bytes): four bindings {u8 enabled at +4 + 8k, device at
// +8 + 8k}, values at +36.
void BuildLogicalPad(lifted::Regs& ctx, uint8_t* base, uint32_t pad) {
  const schema::LogicalPad logical(base, pad);
  float values[input::kPadValues] = {};
  for (uint32_t k = 0; k < 4; k++) {
    const schema::InputPadBinding binding(base, logical.bindings_address() + 8 * k);
    uint32_t device = binding.device();
    if (!binding.enabled() || !device) {
      continue;
    }
    float device_values[input::kPadValues];
    guest::CopyFloats(base, DeviceValues(ctx, base, device), device_values, input::kPadValues);
    input::LogicalPad_Add(values, device_values);
  }
  input::LogicalPad_Clamp(values);
  for (int i = 0; i < input::kPadValues; i++) {
    guest::StoreFloat(base, logical.values_address() + 4 * i, values[i]);
  }
}

// A processor (416 bytes): the fields InputProcessor_Update reads, in, and
// those it writes, out.
void LoadProcessor(uint8_t* base, uint32_t object, input::InputProcessor& p) {
  const schema::InputProcessor o(base, object);
  guest::CopyFloats(base, o.dead_zone_x_address(), p.dead_zone, 4);
  p.held = o.held();
  p.repeat_delay = o.repeat_delay();
  p.repeat_period = o.repeat_period();
  for (int i = 0; i < 32; i++) {
    p.held_ticks[i] = int32_t(guest::LoadU32(base, o.held_ticks_address() + 4 * i));
  }
}

void StoreFloats(uint8_t* base, uint32_t address, const float* values, int count) {
  for (int i = 0; i < count; i++) {
    guest::StoreFloat(base, address + 4 * i, values[i]);
  }
}

void StoreProcessor(uint8_t* base, uint32_t object, const input::InputProcessor& p) {
  const schema::InputProcessor o(base, object);
  StoreFloats(base, o.raw_lx_address(), p.raw, 8);
  StoreFloats(base, o.adjusted_lx_address(), p.adjusted, 8);
  StoreFloats(base, o.left_direction_x_address(), p.direction[0], 4);
  StoreFloats(base, o.right_direction_x_address(), p.direction[1], 4);
  StoreFloats(base, o.left_length_address(), p.length, 2);
  StoreFloats(base, o.buttons_address(), p.buttons, 32);
  o.set_pressed(p.pressed);
  o.set_released(p.released);
  o.set_held(p.held);
  for (int i = 0; i < 32; i++) {
    guest::StoreU32(base, o.held_ticks_address() + 4 * i, uint32_t(p.held_ticks[i]));
  }
}

// sub_82592188 natively (r3 = object, r4 = its logical pad). Returns the
// pad's values' address, which the original leaves in r3. The original
// stores the directions with stvx, which drops the address's low 4 bits;
// objects are 16-byte aligned, and any that isn't is left to the original.
bool UpdateProcessor(lifted::Regs& ctx, uint8_t* base, uint32_t object, uint32_t pad,
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
void UpdateWithPad(lifted::Regs& ctx, uint8_t* base, uint32_t object, uint32_t pad,
                   uint32_t return_address) {
  uint32_t function = schema::InputProcessor(base, object).vtable().update();
  uint32_t values_address;
  if (function == kProcessorUpdate && UpdateProcessor(ctx, base, object, pad, &values_address)) {
    return;
  }
  ctx.r3.u64 = object;
  ctx.r4.u64 = pad;
  ctx.lr = return_address;
  lifted::CallFromContext(ctx, base, function);
}

}  // namespace

ITC_REGS_STEP(InputUpdate_Run) {
  uint32_t manager = ctx.r3.u32;
  uint64_t saved_lr = ctx.lr;
  // The recompiled function's frame, so the guest code called finds the
  // stack as it did.
  guest::StackFrame frame(ctx, 144);

  const schema::InputManager m(base, manager);
  for (uint32_t i = 0; i < 4; i++) {
    ReadPhysicalPad(base, m.physical_pads_address() + kPhysicalPadBytes * i, i);
  }
  for (uint32_t j = 0; j < 5; j++) {
    BuildLogicalPad(ctx, base, m.logical_pads_address() + schema::LogicalPad::kSize * j);
  }
  for (uint32_t j = 0; j < 5; j++) {
    UpdateWithPad(ctx, base, m.processors_address() + schema::InputProcessor::kSize * j,
                  m.logical_pads_address() + schema::LogicalPad::kSize * j, kReturnFromProcessor);
  }
  for (uint32_t j = 0; j < 5; j++) {
    // A PtrList {+4 head, +8 size} (app-states.toml's, by offset until
    // switched); only the head's previous node's object is updated (the
    // last listener, the only one in play).
    uint32_t list = m.listener_lists_address() + kListenerSlotBytes * j;
    if (!guest::LoadU32(base, list + 8)) {
      continue;
    }
    const schema::PtrListNode head(base, guest::LoadU32(base, list + 4));
    const schema::PtrListNode node = head.previous();
    if (node.address == head.address) {
      // An empty list with a count; the game traps here (twi 31).
      ITC_LOG_ERROR("Input::Update: listener slot {} has a count but no node", j);
      continue;
    }
    UpdateWithPad(ctx, base, node.value(),
                  m.logical_pads_address() + schema::LogicalPad::kSize * j, kReturnFromListener);
  }
  ctx.lr = saved_lr;
}

}  // namespace input_hooks

ITC_NATIVE_ENTRY_ALWAYS(82592188) {
  
  uint32_t values_address;
  if (input_hooks::UpdateProcessor(ctx, base, ctx.r3.u32, ctx.r4.u32, &values_address)) {
    ctx.r3.u64 = values_address;
  } else {
    lifted::original::c82592188(ctx, base);
  }
}
