// The type 6 and 7 modes' state machine Inits (docs/functions/
// type6-type7-modes.md) as plain C++: each original's stores and copies in
// its frame and its StateMachine::Add calls replayed in its order from its
// step table (init_tables.h), as RaceFlow::Init's and
// LocationScreen::InitStates' are. The functors' storage words the
// originals never write are copied as the frame holds them (B7 changes no
// behavior).

#include "type67_modes/type67_modes.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

#include <cstdlib>

namespace type67_modes {

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

#include "type67_modes/init_tables.h"

template <size_t n>
InitResult Run(uint8_t* base, const Step (&steps)[n], uint32_t object, uint32_t frame) {
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
    {0x82235138, "Type6Screen0::Init", 1568, 25},
    {0x822454B8, "Type7Screen0::Init", 1456, 23},
    {0x822329A8, "Type6Dialog::Init", 1104, 27},
    {0x82242420, "Type7Dialog::Init", 1168, 27},
    {0x821C9918, "Type6ControllerOptions::Init", 800, 26},
    {0x821C9430, "Type7ControllerOptions::Init", 800, 26},
    {0x821CB6E8, "Type6Options::Init", 976, 27},
    {0x821CB090, "Type7Options::Init", 976, 27},
    {0x821A8210, "Type6SaveScreen::Init", 912, 27},
    {0x8223F9A8, "Type6ListScreen::InitMachine", 992, 26},
    {0x822436A0, "Type7ListScreen::InitMachine", 992, 25},
    {0x82230F80, "Type6PanelScreen::InitMachine", 1648, 24},
    {0x82240998, "Type7PanelScreen::InitMachine", 1648, 23},
    {0x82237320, "Type6NodeScreen::InitMachine", 992, 26},
    {0x82237B78, "PowerGraphScreen6::InitMachine", 1056, 26},
    {0x822470A8, "PowerGraphScreen7::InitMachine", 1056, 25},
};

#define ITC_TYPE67_INIT(address, name)                     \
  case 0x##address: {                                      \
    ITC_NATIVE_RUNNING("sub_" #address " " name);          \
    return Run(base, k##address, object, frame);           \
  }

InitResult RunInit(uint8_t* base, uint32_t address, uint32_t object, uint32_t frame) {
  switch (address) {
    ITC_TYPE67_INIT(82235138, "Type6Screen0::Init")
    ITC_TYPE67_INIT(822454B8, "Type7Screen0::Init")
    ITC_TYPE67_INIT(822329A8, "Type6Dialog::Init")
    ITC_TYPE67_INIT(82242420, "Type7Dialog::Init")
    ITC_TYPE67_INIT(821C9918, "Type6ControllerOptions::Init")
    ITC_TYPE67_INIT(821C9430, "Type7ControllerOptions::Init")
    ITC_TYPE67_INIT(821CB6E8, "Type6Options::Init")
    ITC_TYPE67_INIT(821CB090, "Type7Options::Init")
    ITC_TYPE67_INIT(821A8210, "Type6SaveScreen::Init")
    ITC_TYPE67_INIT(8223F9A8, "Type6ListScreen::InitMachine")
    ITC_TYPE67_INIT(822436A0, "Type7ListScreen::InitMachine")
    ITC_TYPE67_INIT(82230F80, "Type6PanelScreen::InitMachine")
    ITC_TYPE67_INIT(82240998, "Type7PanelScreen::InitMachine")
    ITC_TYPE67_INIT(82237320, "Type6NodeScreen::InitMachine")
    ITC_TYPE67_INIT(82237B78, "PowerGraphScreen6::InitMachine")
    ITC_TYPE67_INIT(822470A8, "PowerGraphScreen7::InitMachine")
    default:
      break;
  }
  ITC_LOG_ERROR("type67_modes: no Init at {:08X}", address);
  std::abort();
}

#undef ITC_TYPE67_INIT

}  // namespace type67_modes
