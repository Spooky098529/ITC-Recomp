// The input manager's set-up and accessors (see input_manager.h), store for
// store as the recompiled functions.

#include "input/input_manager.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "std_list/ptr_list.h"
#include "model_animation/model_animation.h"

#include <cstdlib>
#include <cstring>

namespace input {

namespace {

constexpr uint32_t kLogicalPadVtable = 0x8206CBEC;
constexpr uint32_t kPhysicalPadVtable = 0x8206CBF4;
constexpr uint32_t kProcessorBaseVtable = 0x8206CC00;
constexpr uint32_t kProcessorVtable = 0x8206C0C4;

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
  Store(base, pad, kLogicalPadVtable);
  for (uint32_t i = 0; i < 8; i++) {
    Store(base, pad + 4 + 4 * i, 0);
  }
}

void PhysicalPadConstruct(uint8_t* base, uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_82591B68 PhysicalPad::PhysicalPad");
  Store(base, pad, kPhysicalPadVtable);
  Store(base, pad + 4, 0);
  model_animation::RecordClear(base, pad + 168);
  model_animation::RecordClear(base, pad + 180);
}

void LogicalPadBind(uint8_t* base, uint32_t pad, uint32_t index, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_825918F0 LogicalPad::Bind");
  guest::StoreU8(base, pad + (index << 3) + 4, 1);
  Store(base, pad + ((index + 1) << 3), device);
}

uint32_t LogicalPadValues(uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_82591B30 LogicalPad::Values");
  return pad + 36;
}

uint32_t PhysicalPadValues(uint32_t pad) {
  ITC_NATIVE_RUNNING("sub_820988B8 PhysicalPad::Values");
  return pad + 8;
}

void InputProcessorBaseConstruct(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_825920D0 InputProcessorBase::InputProcessorBase");
  Store(base, p, kProcessorBaseVtable);
  CopyFloat(base, p + 16, kHalf);
  CopyFloat(base, p + 20, kHalf);
  CopyFloat(base, p + 24, kZero);
  CopyFloat(base, p + 28, kZero);
  for (uint32_t w = p + 44; w <= p + 124; w += 16) {
    CopyFloat(base, w, kOne);
  }
  Store(base, p + 264, 0);
  Store(base, p + 268, 0);
  Store(base, p + 272, 0);
  Store(base, p + 276, 20);
  Store(base, p + 280, 4);
  // memset (sub_825A71D0), as its adapter does it.
  guest::ForEachHostPart(base, p + 284, 128,
                         [](uint8_t* host, uint32_t, uint32_t bytes) { std::memset(host, 0, bytes); });
}

void InputProcessorConstruct(uint8_t* base, uint32_t processor) {
  ITC_NATIVE_RUNNING("sub_8258CE60 InputProcessor::InputProcessor");
  InputProcessorBaseConstruct(base, processor);
  Store(base, processor, kProcessorVtable);
}

void InputProcessorAssign(uint8_t* base, uint32_t destination, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_82098D60 InputProcessor::Assign");
  // +16 to +127 by doublewords, in the original's order.
  for (uint32_t at = 16; at < 128; at += 4) {
    Store(base, destination + at, Load(base, source + at));
  }
  // The lengths and the 32 values, as floats.
  for (uint32_t at = 128; at < 264; at += 4) {
    CopyFloat(base, destination + at, source + at);
  }
  // The button words, the delay and period, the 32 counters.
  for (uint32_t at = 264; at < 412; at += 4) {
    Store(base, destination + at, Load(base, source + at));
  }
}

uint32_t InputProcessorAnyPressed(uint8_t* base, uint32_t processor, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_82592500 InputProcessor::AnyPressed");
  return (Load(base, processor + 264) & mask) != 0 ? 1 : 0;
}

uint32_t InputProcessorPressedOrRepeat(uint8_t* base, uint32_t processor, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_82592520 InputProcessor::PressedOrRepeat");
  uint32_t bit = 0;
  while (bit < 32 && !(mask & (1u << bit))) {
    bit++;
  }
  if (bit == 32 || !(Load(base, processor + 272) & (1u << bit))) {
    return 0;
  }
  const int32_t ticks = int32_t(Load(base, processor + 284 + 4 * bit));
  const int32_t delay = int32_t(Load(base, processor + 276));
  return ticks < delay ? (ticks == 1 ? 1 : 0) : (ticks == delay ? 1 : 0);
}

uint32_t InputProcessorTestPressed(uint8_t* base, uint32_t processor, uint32_t mask_address) {
  ITC_NATIVE_RUNNING("sub_8258CEB0 InputProcessor::TestPressed");
  return InputProcessorAnyPressed(base, processor, Load(base, mask_address));
}

uint32_t InputProcessorTestRepeat(uint8_t* base, uint32_t processor, uint32_t mask_address) {
  ITC_NATIVE_RUNNING("sub_8258CEE0 InputProcessor::TestRepeat");
  return InputProcessorPressedOrRepeat(base, processor, Load(base, mask_address));
}

uint32_t ManagerProcessor(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82098CB8 InputManager::GetProcessor");
  return manager + kManagerProcessors + kProcessorBytes * index;
}

uint32_t ManagerLogicalPad(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82098CD0 InputManager::GetLogicalPad");
  return manager + kManagerLogicalPads + kLogicalPadBytes * index;
}

uint32_t ManagerPhysicalPad(uint32_t manager, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82098FE0 InputManager::GetPhysicalPad");
  return manager + kManagerPhysicalPads + kPhysicalPadBytes * index;
}

std::optional<uint32_t> ManagerAllocate(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820993E8 Input::Create");
  return memory::crt::OperatorNew(base, kManagerBytes);
}

bool ManagerConstructMembers(uint8_t* base, uint32_t manager, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820991E0 InputManager::InputManager");
  if (!std_list::CanAllocate(base, 1)) {
    return false;
  }
  Store(base, manager, kManagerVtable);
  for (uint32_t i = 0; i < 5; i++) {
    LogicalPadConstruct(base, manager + kManagerLogicalPads + kLogicalPadBytes * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    PhysicalPadConstruct(base, manager + kManagerPhysicalPads + kPhysicalPadBytes * i);
  }
  for (uint32_t i = 0; i < 5; i++) {
    InputProcessorConstruct(base, manager + kManagerProcessors + kProcessorBytes * i);
  }
  for (uint32_t i = 0; i < 5; i++) {
    if (!std_list::Construct(base, manager + kManagerListeners + kListenerListBytes * i)) {
      ITC_LOG_ERROR("InputManager: a listener list refused after its checks");
      std::abort();
    }
  }
  Store(base, manager + 4, Load(base, block + 4));
  return true;
}

void ManagerBindPads(uint8_t* base, uint32_t manager) {
  const uint32_t pads = manager + kManagerLogicalPads;
  for (uint32_t i = 0; i < 4; i++) {
    LogicalPadBind(base, pads + kLogicalPadBytes * i, 0,
                   manager + kManagerPhysicalPads + kPhysicalPadBytes * i);
  }
  const uint32_t any = pads + kLogicalPadBytes * 4;
  for (uint32_t i = 0; i < 4; i++) {
    LogicalPadBind(base, any, i, pads + kLogicalPadBytes * i);
  }
}

bool ManagerAddListener(uint8_t* base, uint32_t manager, uint32_t index, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_82099430 InputManager::AddListener");
  const uint32_t list = manager + kManagerListeners + kListenerListBytes * index;
  if (Load(base, list + std_list::kSize) != 0 || !std_list::CanAllocate(base, 1)) {
    return false;
  }
  if (!std_list::Insert(base, list, Load(base, list + std_list::kHead), listener)) {
    ITC_LOG_ERROR("InputManager::AddListener: the node refused after its checks");
    std::abort();
  }
  InputProcessorAssign(base, listener, manager + kManagerProcessors + kProcessorBytes * index);
  return true;
}

}  // namespace input
