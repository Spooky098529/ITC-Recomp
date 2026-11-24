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
// The manager is in host memory (E5 step 4), its pads and processors
// embedded objects reached through interior handles; the values and the
// processors' arrays past a schema field are read and written by offset
// (load_at, store_at). See "Input::Update" in docs/pause-menu-fix.md for the
// layout.

#include "input_update.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "input/input_processor.h"
#include "input/logical_pad.h"
#include "input/physical_pad.h"

#include "base/log.h"
#include "lifted/sdk.h"
#include "schema/app_states.h"
#include "schema/input.h"
#include "schema/action_mappers.h"
#include "schema/model_animation.h"

#include <bit>

namespace input_hooks {
namespace {

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

// A motor: model-animation.toml's PhysicalPadRecord (+0 the speed, +4 held,
// +8 the ticks left).
void LoadMotor(uint8_t* base, uint32_t address, input::PadMotor& motor) {
  const schema::PhysicalPadRecord r(base, address);
  motor.speed = r.unknown_0();
  motor.held = int32_t(r.unknown_4());
  motor.ticks_left = int32_t(r.unknown_8());
}

void StoreMotor(uint8_t* base, uint32_t address, const input::PadMotor& motor) {
  const schema::PhysicalPadRecord r(base, address);
  r.set_unknown_0(motor.speed);
  r.set_unknown_4(uint32_t(motor.held));
  r.set_unknown_8(uint32_t(motor.ticks_left));
}

// The pad's XInput gamepad at `at` (inside the capabilities, by offset).
void StoreGamepad(const schema::PhysicalPad& g, uint32_t at, const input::XInputGamepad& gamepad) {
  g.store_at(at + 0, 2, gamepad.buttons);
  g.store_at(at + 2, 1, gamepad.left_trigger);
  g.store_at(at + 3, 1, gamepad.right_trigger);
  g.store_at(at + 4, 2, uint16_t(gamepad.thumb_lx));
  g.store_at(at + 6, 2, uint16_t(gamepad.thumb_ly));
  g.store_at(at + 8, 2, uint16_t(gamepad.thumb_rx));
  g.store_at(at + 10, 2, uint16_t(gamepad.thumb_ry));
}

// The first `count` of a device's 40 values at `values` (guest memory, or
// a host pad's: ActionPadValues is dual), by offset, into `out`.
void LoadValues(uint8_t* base, uint32_t values, float* out, int count) {
  const schema::ActionPadValues v(base, values);
  for (int i = 0; i < count; i++) {
    out[i] = std::bit_cast<float>(uint32_t(v.load_at(4 * i, 4)));
  }
}

// A physical pad (212 bytes): the fields PhysicalPad_Read touches, in and out.
void ReadPhysicalPad(uint8_t* base, uint32_t address, uint32_t user) {
  const schema::PhysicalPad g(base, address);
  input::PhysicalPad pad;
  pad.flags = g.unknown_4();
  LoadMotor(base, g.record_a().address, pad.motors[0]);
  LoadMotor(base, g.record_b().address, pad.motors[1]);
  bool caps_read = input::PhysicalPad_Read(pad, user);
  g.set_unknown_4(pad.flags);
  const auto values = g.values();
  for (int i = 0; i < input::kPadValues; i++) {
    values.store_at(4 * i, 4, std::bit_cast<uint32_t>(pad.values[i]));
  }
  StoreMotor(base, g.record_a().address, pad.motors[0]);
  StoreMotor(base, g.record_b().address, pad.motors[1]);
  if (caps_read) {
    // The XInput capabilities at +192: no field in PhysicalPad yet.
    constexpr uint32_t caps = 192;
    g.store_at(caps + 0, 1, pad.caps.type);
    g.store_at(caps + 1, 1, pad.caps.sub_type);
    g.store_at(caps + 2, 2, pad.caps.flags);
    StoreGamepad(g, caps + 4, pad.caps.gamepad);
    g.store_at(caps + 16, 2, pad.caps.left_motor);
    g.store_at(caps + 18, 2, pad.caps.right_motor);
  }
}

// A device's values' address: its vtable slot 1 (r3 = device).
uint32_t DeviceValues(lifted::Regs& ctx, uint8_t* base, uint32_t device) {
  // Its vtable: its first word (a logical or a physical pad's).
  uint32_t function = schema::InputDeviceVtable(base, guest::VtableWord(base, device)).values();
  if (function == kPhysicalPadValues) {
    return schema::PhysicalPad(base, device).values().address;
  }
  if (function == kLogicalPadValues) {
    return schema::LogicalPad(base, device).values().address;
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
    const schema::InputPadBinding binding = logical.binding(k);
    uint32_t device = binding.device();
    if (!binding.enabled() || !device) {
      continue;
    }
    float device_values[input::kPadValues];
    LoadValues(base, DeviceValues(ctx, base, device), device_values, input::kPadValues);
    input::LogicalPad_Add(values, device_values);
  }
  input::LogicalPad_Clamp(values);
  const auto out = logical.values();
  for (int i = 0; i < input::kPadValues; i++) {
    out.store_at(4 * i, 4, std::bit_cast<uint32_t>(values[i]));
  }
}

// A processor (416 bytes): the fields InputProcessor_Update reads, in, and
// those it writes, out.
// The processor's offsets (schema/input.h's InputProcessor; in host memory,
// E5 step 4, its arrays read and written by offset).
constexpr uint32_t kRaw = 32;
constexpr uint32_t kAdjusted = 64;
constexpr uint32_t kLeftDirection = 96;
constexpr uint32_t kRightDirection = 112;
constexpr uint32_t kLengths = 128;
constexpr uint32_t kButtons = 136;
constexpr uint32_t kHeldTicks = 284;

void LoadProcessor(uint8_t* base, uint32_t object, input::InputProcessor& p) {
  const schema::InputProcessor o(base, object);
  p.dead_zone[0] = o.dead_zone_x();
  p.dead_zone[1] = o.dead_zone_y();
  p.dead_zone[2] = o.dead_zone_trigger();
  p.dead_zone[3] = o.dead_zone_unused();
  p.held = o.held();
  p.repeat_delay = o.repeat_delay();
  p.repeat_period = o.repeat_period();
  for (int i = 0; i < 32; i++) {
    p.held_ticks[i] = int32_t(uint32_t(o.load_at(kHeldTicks + 4 * i, 4)));
  }
}

void StoreFloats(const schema::InputProcessor& o, uint32_t at, const float* values, int count) {
  for (int i = 0; i < count; i++) {
    o.store_at(at + 4 * i, 4, std::bit_cast<uint32_t>(values[i]));
  }
}

void StoreProcessor(uint8_t* base, uint32_t object, const input::InputProcessor& p) {
  const schema::InputProcessor o(base, object);
  StoreFloats(o, kRaw, p.raw, 8);
  StoreFloats(o, kAdjusted, p.adjusted, 8);
  StoreFloats(o, kLeftDirection, p.direction[0], 4);
  StoreFloats(o, kRightDirection, p.direction[1], 4);
  StoreFloats(o, kLengths, p.length, 2);
  StoreFloats(o, kButtons, p.buttons, 32);
  o.set_pressed(p.pressed);
  o.set_released(p.released);
  o.set_held(p.held);
  for (int i = 0; i < 32; i++) {
    o.store_at(kHeldTicks + 4 * i, 4, uint32_t(p.held_ticks[i]));
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
  LoadValues(base, *values_address, values, input::kPadValues);
  input::InputProcessor processor;
  LoadProcessor(base, object, processor);
  input::InputProcessor_Update(processor, values);
  StoreProcessor(base, object, processor);
  return true;
}

// Calls `object`'s vtable slot 1 with r4 = its logical pad.
void UpdateWithPad(lifted::Regs& ctx, uint8_t* base, uint32_t object, uint32_t pad,
                   uint32_t return_address) {
  // Its vtable word (a listener may be any object: read from either shape).
  uint32_t function = schema::InputProcessorVtable(base, guest::VtableWord(base, object)).update();
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
    ReadPhysicalPad(base, m.physical_pad(i).address, i);
  }
  for (uint32_t j = 0; j < 5; j++) {
    BuildLogicalPad(ctx, base, m.logical_pad(j).address);
  }
  for (uint32_t j = 0; j < 5; j++) {
    UpdateWithPad(ctx, base, m.processor(j).address, m.logical_pad(j).address,
                  kReturnFromProcessor);
  }
  for (uint32_t j = 0; j < 5; j++) {
    // A PtrList (app-states.toml's); only the head's previous node's object
    // is updated (the last listener, the only one in play).
    const schema::PtrList list = m.listener_list(j);
    if (!list.size()) {
      continue;
    }
    const auto head = list.head<schema::PtrListNode>();
    const schema::PtrListNode node = head.previous();
    if (node.address == head.address) {
      // An empty list with a count; the game traps here (twi 31).
      ITC_LOG_ERROR("Input::Update: listener slot {} has a count but no node", j);
      continue;
    }
    UpdateWithPad(ctx, base, node.value(), m.logical_pad(j).address, kReturnFromListener);
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
