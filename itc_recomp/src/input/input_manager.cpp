// The input manager's set-up and accessors (see input_manager.h), store for
// store as the recompiled functions.

#include "input/input_manager.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "std_list/ptr_list.h"
#include "model_animation/model_animation.h"
#include "schema/app_states.h"
#include "schema/input.h"
#include "schema/model_animation.h"

#include <cstdlib>
#include <cstring>

namespace input {

namespace {

constexpr uint32_t kProcessorBaseVtable = 0x8206CC00;

constexpr uint32_t kHalf = 0x82001BE4;  // the game's float 0.5
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kOne = 0x82000A30;   // 1.0

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

// lfs then stfs: the float through a double, as the translated code does.
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  const double value = double(guest::LoadFloat(base, from));
  guest::StoreFloat(base, to, float(value));
}

}  // namespace

void LogicalPadConstruct(uint8_t* base, uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_82591B38 LogicalPad::LogicalPad");
  const schema::LogicalPad p(base, pad);
  p.set_vtable(schema::LogicalPad::kVtable);
  for (uint32_t i = 0; i < 8; i++) {
    Store(base, p.bindings_address() + 4 * i, 0);
  }
}

void PhysicalPadConstruct(uint8_t* base, uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_82591B68 PhysicalPad::PhysicalPad");
  const schema::PhysicalPad p(base, pad);
  p.set_vtable(schema::PhysicalPad::kVtable);
  p.set_unknown_4(0);
  model_animation::RecordClear(base, p.record_a_address());
  model_animation::RecordClear(base, p.record_b_address());
}

void LogicalPadBind(uint8_t* base, uint32_t pad, uint32_t index, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_825918F0 LogicalPad::Bind");
  const schema::InputPadBinding binding(
      base, schema::LogicalPad(base, pad).bindings_address() + (index << 3));
  binding.set_enabled(1);
  binding.set_device(device);
}

uint32_t LogicalPadValues(uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_82591B30 LogicalPad::Values");
  return schema::LogicalPad(nullptr, pad).values_address();
}

uint32_t PhysicalPadValues(uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_820988B8 PhysicalPad::Values");
  return schema::PhysicalPad(nullptr, pad).values_address();
}

void InputProcessorBaseConstruct(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_825920D0 InputProcessorBase::InputProcessorBase");
  const schema::InputProcessor q(base, p);
  q.set_vtable(kProcessorBaseVtable);
  CopyFloat(base, q.dead_zone_x_address(), kHalf);
  CopyFloat(base, q.dead_zone_y_address(), kHalf);
  CopyFloat(base, q.dead_zone_trigger_address(), kZero);
  CopyFloat(base, q.dead_zone_unused_address(), kZero);
  // The six vectors' w (raw_3, raw_7, adjusted_3, adjusted_7, the directions').
  for (uint32_t w = q.raw_3_address(); w <= q.right_direction_w_address(); w += 16) {
    CopyFloat(base, w, kOne);
  }
  q.set_pressed(0);
  q.set_released(0);
  q.set_held(0);
  q.set_repeat_delay(20);
  q.set_repeat_period(4);
  // memset (sub_825A71D0), as its adapter does it.
  guest::ForEachHostPart(base, q.held_ticks_address(), 128,
                         [](uint8_t* host, uint32_t, uint32_t bytes) { std::memset(host, 0, bytes); });
}

void InputProcessorConstruct(uint8_t* base, uint32_t processor) {
  ITC_NATIVE_RUNNING("sub_8258CE60 InputProcessor::InputProcessor");
  InputProcessorBaseConstruct(base, processor);
  schema::InputProcessor(base, processor).set_vtable(schema::InputProcessor::kVtable);
}

void InputProcessorAssign(uint8_t* base, uint32_t destination, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_82098D60 InputProcessor::Assign");
  const schema::InputProcessor to(base, destination), from(base, source);
  // The dead zones to the directions (+16 to +127) by doublewords, in the
  // original's order.
  for (uint32_t t = to.dead_zone_x_address(), f = from.dead_zone_x_address();
       t < to.left_length_address(); t += 4, f += 4) {
    Store(base, t, Load(base, f));
  }
  // The lengths and the 32 values (to +263), as floats.
  for (uint32_t t = to.left_length_address(), f = from.left_length_address();
       t < to.pressed_address(); t += 4, f += 4) {
    CopyFloat(base, t, f);
  }
  // The button words, the delay and period, the 32 counters (to +411).
  for (uint32_t t = to.pressed_address(), f = from.pressed_address();
       t < to.held_ticks_address() + 128; t += 4, f += 4) {
    Store(base, t, Load(base, f));
  }
}

uint32_t InputProcessorAnyPressed(uint8_t* base, uint32_t processor, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_82592500 InputProcessor::AnyPressed");
  return (schema::InputProcessor(base, processor).pressed() & mask) != 0 ? 1 : 0;
}

uint32_t InputProcessorPressedOrRepeat(uint8_t* base, uint32_t processor, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_82592520 InputProcessor::PressedOrRepeat");
  uint32_t bit = 0;
  while (bit < 32 && !(mask & (1u << bit))) {
    bit++;
  }
  const schema::InputProcessor p(base, processor);
  if (bit == 32 || !(p.held() & (1u << bit))) {
    return 0;
  }
  const int32_t ticks = int32_t(Load(base, p.held_ticks_address() + 4 * bit));
  const int32_t delay = p.repeat_delay();
  return ticks < delay ? (ticks == 1 ? 1 : 0) : (ticks == delay ? 1 : 0);
}

uint32_t InputProcessorTestPressed(uint8_t* base, uint32_t processor, uint32_t mask_address) {
  ITC_NATIVE_RUNNING("sub_8258CEB0 InputProcessor::TestPressed");
  return InputProcessorAnyPressed(base, processor, schema::InputButtonMask(base, mask_address).mask());
}

uint32_t InputProcessorTestRepeat(uint8_t* base, uint32_t processor, uint32_t mask_address) {
  ITC_NATIVE_RUNNING("sub_8258CEE0 InputProcessor::TestRepeat");
  return InputProcessorPressedOrRepeat(base, processor,
                                       schema::InputButtonMask(base, mask_address).mask());
}

uint32_t ManagerProcessor(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82098CB8 InputManager::GetProcessor");
  return schema::InputManager(nullptr, manager).processors_address() + schema::InputProcessor::kSize * index;
}

uint32_t ManagerLogicalPad(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82098CD0 InputManager::GetLogicalPad");
  return schema::InputManager(nullptr, manager).logical_pads_address() + schema::LogicalPad::kSize * index;
}

uint32_t ManagerPhysicalPad(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82098FE0 InputManager::GetPhysicalPad");
  return schema::InputManager(nullptr, manager).physical_pads_address() + schema::PhysicalPad::kSize * index;
}

std::optional<uint32_t> ManagerAllocate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820993E8 Input::Create");
  return memory::crt::OperatorNew(base, schema::InputManager::kSize);
}

bool ManagerConstructMembers(uint8_t* base, uint32_t manager, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820991E0 InputManager::InputManager");
  if (!std_list::CanAllocate(base, 1)) {
    return false;
  }
  const schema::InputManager m(base, manager);
  m.set_vtable(schema::InputManager::kVtable);
  for (uint32_t i = 0; i < 5; i++) {
    LogicalPadConstruct(base, m.logical_pads_address() + schema::LogicalPad::kSize * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    PhysicalPadConstruct(base, m.physical_pads_address() + schema::PhysicalPad::kSize * i);
  }
  for (uint32_t i = 0; i < 5; i++) {
    InputProcessorConstruct(base, m.processors_address() + schema::InputProcessor::kSize * i);
  }
  for (uint32_t i = 0; i < 5; i++) {
    if (!std_list::Construct(base, m.listener_lists_address() + schema::PtrList::kSize * i)) {
      ITC_LOG_ERROR("InputManager: a listener list refused after its checks");
      std::abort();
    }
  }
  m.set_framework_part(schema::InputManagerArgs(base, block).framework_part());
  return true;
}

void ManagerBindPads(uint8_t* base, uint32_t manager) {
  const schema::InputManager m(base, manager);
  const uint32_t pads = m.logical_pads_address();
  for (uint32_t i = 0; i < 4; i++) {
    LogicalPadBind(base, pads + schema::LogicalPad::kSize * i, 0,
                   m.physical_pads_address() + schema::PhysicalPad::kSize * i);
  }
  const uint32_t any = pads + schema::LogicalPad::kSize * 4;
  for (uint32_t i = 0; i < 4; i++) {
    LogicalPadBind(base, any, i, pads + schema::LogicalPad::kSize * i);
  }
}

bool ManagerAddListener(uint8_t* base, uint32_t manager, uint32_t index, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_82099430 InputManager::AddListener");
  const schema::InputManager m(base, manager);
  const uint32_t list = m.listener_lists_address() + schema::PtrList::kSize * index;
  const schema::PtrList l(base, list);
  if (l.size() != 0 || !std_list::CanAllocate(base, 1)) {
    return false;
  }
  if (!std_list::Insert(base, list, l.head<schema::PtrListNode>().address, listener)) {
    ITC_LOG_ERROR("InputManager::AddListener: the node refused after its checks");
    std::abort();
  }
  InputProcessorAssign(base, listener, m.processors_address() + schema::InputProcessor::kSize * index);
  return true;
}

}  // namespace input
