// The options screens' state machine Inits for the type 2 and 4 modes
// (docs/functions/options-screen.md) as plain C++: each original's stores
// and copies in its frame and its StateMachine::Add calls replayed in its
// order from its step table (init_tables.h), as the type 6 and 7 modes'
// Inits are (src/type67_modes/inits.cpp). The functors' storage words the
// originals never write are copied as the frame holds them (B7 changes no
// behavior).

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

#include <cstdlib>

namespace options_screen {

namespace {

// A value: a constant, or an offset from the object (r3) or the frame.
struct Value {
  enum Base : uint8_t { kConst, kObject, kFrame } base;
  uint32_t value;
};
constexpr Value C(uint32_t value) { return {Value::kConst, value}; }
constexpr Value O(uint32_t offset) { return {Value::kObject, offset}; }
constexpr Value F(uint32_t offset) { return {Value::kFrame, offset}; }

// kStore: [a] = b; kCopy: [a] = [b]; kAdd: StateMachine::Add(machine c,
// key a, functor part b).
enum class Op : uint8_t { kStore, kCopy, kAdd };
struct Step {
  Op op;
  Value a;
  Value b;
  Value c = C(0);
};

#include "options_screen/init_tables.h"

template <size_t n>
InitResult Run(uint8_t* base, const Step (&steps)[n], uint32_t object, uint32_t sp,
               uint32_t frame_bytes) {
  const uint32_t frame = sp - frame_bytes;
  guest::StoreU32(base, frame, sp);  // the back chain
  auto at = [&](const Value& v) {
    switch (v.base) {
      case Value::kConst:
        return v.value;
      case Value::kObject:
        return object + v.value;
      case Value::kFrame:
        break;
    }
    return frame + v.value;
  };
  InitResult result{0, object, false};
  for (const Step& step : steps) {
    switch (step.op) {
      case Op::kStore:
        guest::StoreU32(base, at(step.a), at(step.b));
        break;
      case Op::kCopy:
        guest::StoreU32(base, at(step.a), guest::LoadU32(base, at(step.b)));
        break;
      case Op::kAdd:
        result.machine = at(step.c);
        result.r3 = game_modes::StateMachineAdd(base, result.machine, at(step.a), at(step.b),
                                                &result.replaced);
        break;
    }
  }
  return result;
}

}  // namespace

const InitInfo kInits[kInitCount] = {
    {0x821C9E00, "Type2Options::Init", 976, 27},
    {0x821CA458, "Type4Options::Init", 992, 26},
    {0x821C8578, "Type2ControllerOptions::Init", 800, 26},
    {0x821C8A60, "Type4ControllerOptions::Init", 800, 26},
};

#define ITC_OPTIONS_INIT(address, name, bytes)            \
  case 0x##address: {                                     \
    ITC_NATIVE_RUNNING("sub_" #address " " name);         \
    return Run(base, k##address, object, sp, bytes);      \
  }

InitResult RunInit(uint8_t* base, uint32_t address, uint32_t object, uint32_t sp) {
  switch (address) {
    ITC_OPTIONS_INIT(821C9E00, "Type2Options::Init", 976)
    ITC_OPTIONS_INIT(821CA458, "Type4Options::Init", 992)
    ITC_OPTIONS_INIT(821C8578, "Type2ControllerOptions::Init", 800)
    ITC_OPTIONS_INIT(821C8A60, "Type4ControllerOptions::Init", 800)
    default:
      break;
  }
  ITC_LOG_ERROR("options_screen: no Init at {:08X}", address);
  std::abort();
}

#undef ITC_OPTIONS_INIT

}  // namespace options_screen
