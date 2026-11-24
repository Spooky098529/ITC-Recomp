// The input manager's set-up and accessors (see input_manager.h), store for
// store as the recompiled functions. The manager is in host memory (E5 step
// 4), its pads and processors embedded objects: views of them are interior
// handles, read through their members (arrays past a schema field, and bytes
// fields, by offset: load_at, store_at).

#include "input/input_manager.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "std_list/ptr_list.h"
#include "model_animation/model_animation.h"
#include "schema/app_states.h"
#include "schema/input.h"
#include "schema/model_animation.h"

#include <bit>
#include <cstdlib>
#include <cstring>

namespace input {

namespace {

constexpr uint32_t kProcessorBaseVtable = 0x8206CC00;

constexpr uint32_t kHalf = 0x82001BE4;  // the game's float 0.5
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kOne = 0x82000A30;   // 1.0

// A processor's 32 hold counters (s32, +284): a bytes field, read by offset.
constexpr uint32_t kHeldTicks = 284;

// lfs: an image float through a double, as the translated code does (then
// stored with stfs by the caller's setter).
float LoadImageFloat(uint8_t* base, uint32_t from) {
  return float(double(guest::LoadFloat(base, from)));
}

// lfs then stfs of a word inside a host object: the float through a double.
uint32_t ThroughDouble(uint32_t bits) {
  return std::bit_cast<uint32_t>(float(double(std::bit_cast<float>(bits))));
}

}  // namespace

void LogicalPadConstruct(uint8_t* base, uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_82591B38 LogicalPad::LogicalPad");
  const schema::LogicalPad p(base, pad);
  p.set_vtable(schema::LogicalPad::kVtable);
  // The bindings (+4 to +35), word by word.
  for (uint32_t i = 0; i < 8; i++) {
    p.store_at(4 + 4 * i, 4, 0);
  }
}

void PhysicalPadConstruct(uint8_t* base, uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_82591B68 PhysicalPad::PhysicalPad");
  const schema::PhysicalPad p(base, pad);
  p.set_vtable(schema::PhysicalPad::kVtable);
  p.set_unknown_4(0);
  model_animation::RecordClear(base, p.record_a().address);
  model_animation::RecordClear(base, p.record_b().address);
}

void LogicalPadBind(uint8_t* base, uint32_t pad, uint32_t index, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_825918F0 LogicalPad::Bind");
  const schema::InputPadBinding binding = schema::LogicalPad(base, pad).binding(index);
  binding.set_enabled(1);
  binding.set_device(device);
}

uint32_t LogicalPadValues(uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_82591B30 LogicalPad::Values");
  return schema::LogicalPad(nullptr, pad).values().address;
}

uint32_t PhysicalPadValues(uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_820988B8 PhysicalPad::Values");
  return schema::PhysicalPad(nullptr, pad).values().address;
}

void InputProcessorBaseConstruct(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_825920D0 InputProcessorBase::InputProcessorBase");
  const schema::InputProcessor q(base, p);
  q.set_vtable(kProcessorBaseVtable);
  q.set_dead_zone_x(LoadImageFloat(base, kHalf));
  q.set_dead_zone_y(LoadImageFloat(base, kHalf));
  q.set_dead_zone_trigger(LoadImageFloat(base, kZero));
  q.set_dead_zone_unused(LoadImageFloat(base, kZero));
  // The six vectors' w (raw_3, raw_7, adjusted_3, adjusted_7, the directions').
  q.set_raw_3(LoadImageFloat(base, kOne));
  q.set_raw_7(LoadImageFloat(base, kOne));
  q.set_adjusted_3(LoadImageFloat(base, kOne));
  q.set_adjusted_7(LoadImageFloat(base, kOne));
  q.set_left_direction_w(LoadImageFloat(base, kOne));
  q.set_right_direction_w(LoadImageFloat(base, kOne));
  q.set_pressed(0);
  q.set_released(0);
  q.set_held(0);
  q.set_repeat_delay(20);
  q.set_repeat_period(4);
  // memset (sub_825A71D0) of the 32 counters: the host object's bytes field.
  std::memset(q.held_ticks(), 0, 128);
}

void InputProcessorConstruct(uint8_t* base, uint32_t processor) {
  ITC_NATIVE_RUNNING("sub_8258CE60 InputProcessor::InputProcessor");
  InputProcessorBaseConstruct(base, processor);
  schema::InputProcessor(base, processor).set_vtable(schema::InputProcessor::kVtable);
}

void InputProcessorAssign(uint8_t* base, uint32_t destination, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_82098D60 InputProcessor::Assign");
  const schema::InputProcessor to(base, destination), from(base, source);
  // By offset (both in host memory, E5 step 4), word by word as the guest's
  // bytes give them.
  // The dead zones to the directions (+16 to +127) by doublewords, in the
  // original's order.
  for (uint32_t at = 16; at < 128; at += 4) {
    to.store_at(at, 4, from.load_at(at, 4));
  }
  // The lengths and the 32 values (to +263), as floats.
  for (uint32_t at = 128; at < 264; at += 4) {
    to.store_at(at, 4, ThroughDouble(uint32_t(from.load_at(at, 4))));
  }
  // The button words, the delay and period, the 32 counters (to +411).
  for (uint32_t at = 264; at < kHeldTicks + 128; at += 4) {
    to.store_at(at, 4, from.load_at(at, 4));
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
  const int32_t ticks = int32_t(uint32_t(p.load_at(kHeldTicks + 4 * bit, 4)));
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
  return schema::InputManager(nullptr, manager).processor(index).address;
}

uint32_t ManagerLogicalPad(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82098CD0 InputManager::GetLogicalPad");
  return schema::InputManager(nullptr, manager).logical_pad(index).address;
}

uint32_t ManagerPhysicalPad(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82098FE0 InputManager::GetPhysicalPad");
  return schema::InputManager(nullptr, manager).physical_pad(index).address;
}

std::optional<uint32_t> ManagerAllocate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820993E8 Input::Create");
  // In host memory (E5 step 4): never freed (the process ends with it).
  return schema::InputManager::New(base).address;
}

bool ManagerConstructMembers(uint8_t* base, uint32_t manager, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820991E0 InputManager::InputManager");
  if (!std_list::CanAllocate(base, 1)) {
    return false;
  }
  const schema::InputManager m(base, manager);
  m.set_vtable(schema::InputManager::kVtable);
  for (uint32_t i = 0; i < 5; i++) {
    LogicalPadConstruct(base, m.logical_pad(i).address);
  }
  for (uint32_t i = 0; i < 4; i++) {
    PhysicalPadConstruct(base, m.physical_pad(i).address);
  }
  for (uint32_t i = 0; i < 5; i++) {
    InputProcessorConstruct(base, m.processor(i).address);
  }
  for (uint32_t i = 0; i < 5; i++) {
    if (!std_list::Construct(base, m.listener_list(i).address)) {
      ITC_LOG_ERROR("InputManager: a listener list refused after its checks");
      std::abort();
    }
  }
  m.set_framework_part(schema::InputManagerArgs(base, block).framework_part());
  return true;
}

void ManagerBindPads(uint8_t* base, uint32_t manager) {
  const schema::InputManager m(base, manager);
  for (uint32_t i = 0; i < 4; i++) {
    LogicalPadBind(base, m.logical_pad(i).address, 0, m.physical_pad(i).address);
  }
  const uint32_t any = m.logical_pad(4).address;
  for (uint32_t i = 0; i < 4; i++) {
    LogicalPadBind(base, any, i, m.logical_pad(i).address);
  }
}

bool ManagerAddListener(uint8_t* base, uint32_t manager, uint32_t index, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_82099430 InputManager::AddListener");
  const schema::InputManager m(base, manager);
  const schema::PtrList l = m.listener_list(index);
  const uint32_t list = l.address;
  if (l.size() != 0 || !std_list::CanAllocate(base, 1)) {
    return false;
  }
  if (!std_list::Insert(base, list, l.head<schema::PtrListNode>().address, listener)) {
    ITC_LOG_ERROR("InputManager::AddListener: the node refused after its checks");
    std::abort();
  }
  InputProcessorAssign(base, listener, m.processor(index).address);
  return true;
}

}  // namespace input
