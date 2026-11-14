// Type8PanelScreen::InitMachine and Type8ColorListScreen::InitMachine
// (docs/functions/type8-screens.md) as plain
// C++: the original's stores and copies in its frame and its
// StateMachine::Add calls replayed in its order from its step table
// (init_table.h), as the type 6 and 7 screens' InitMachines are
// (src/type67_modes/inits.cpp). The functors' storage words the original
// never writes are copied as the frame holds them (B7 changes no behavior).

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

namespace type8_screens {

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

#include "type8_screens/init_table.h"

// A step table run on `screen` with its frame at `frame` (`frame_bytes`).
template <size_t N>
uint32_t Run(uint8_t* base, const Step (&steps)[N], uint32_t screen, uint32_t frame,
             uint32_t frame_bytes, uint32_t* machine, bool* replaced) {
  auto at = [&](const Value& v) {
    switch (v.base) {
      case Value::kConst:
        return v.value;
      case Value::kObject:
        return screen + v.value;
      case Value::kFrame:
        break;
    }
    return frame + v.value;
  };
  guest::StoreU32(base, frame, frame + frame_bytes);
  uint32_t r3 = 0;
  for (const Step& step : steps) {
    switch (step.op) {
      case Op::kStore:
        guest::StoreU32(base, at(step.a), at(step.b));
        break;
      case Op::kCopy:
        guest::StoreU32(base, at(step.a), guest::LoadU32(base, at(step.b)));
        break;
      case Op::kAdd:
        *machine = at(step.c);
        r3 = game_modes::StateMachineAdd(base, *machine, at(step.a), at(step.b), replaced);
        break;
    }
  }
  return r3;
}

}  // namespace

uint32_t InitMachine(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t* machine,
                     bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82238E98 Type8PanelScreen::InitMachine");
  return Run(base, k82238E98, screen, frame, kInitMachineFrame, machine, replaced);
}

uint32_t ColorList8InitMachine(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t* machine,
                               bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8223E9D8 Type8ColorListScreen::InitMachine");
  return Run(base, k8223E9D8, screen, frame, kColorListInitMachineFrame, machine, replaced);
}

}  // namespace type8_screens
