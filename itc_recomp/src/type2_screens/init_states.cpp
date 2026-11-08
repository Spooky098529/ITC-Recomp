// The type 2 screens' state machine set-ups (docs/functions/type2-screens.md):
// the originals' stores and copies in their frames replayed in their order
// from their step tables (init_tables.h), their transitions added through
// StateMachine::Add's plain C++, as the car dealer's and the type 6 and 7
// modes' Inits are (src/car_dealer/init_states.cpp). The functors' storage
// words the originals never write are copied as the frames hold them (B7
// changes no behavior).

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

namespace type2_screens {

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

#include "type2_screens/init_tables.h"

template <size_t N>
uint32_t RunSteps(uint8_t* base, const Step (&steps)[N], uint32_t machine, uint32_t frame,
                  uint32_t bytes, bool* replaced) {
  auto at = [&](const Value& v) {
    switch (v.base) {
      case Value::kConst:
        return v.value;
      case Value::kObject:
        return machine + v.value;
      case Value::kFrame:
        break;
    }
    return frame + v.value;
  };
  guest::StoreU32(base, frame, frame + bytes);
  uint32_t r3 = machine;
  *replaced = false;
  for (const Step& step : steps) {
    switch (step.op) {
      case Op::kStore:
        guest::StoreU32(base, at(step.a), at(step.b));
        break;
      case Op::kCopy:
        guest::StoreU32(base, at(step.a), guest::LoadU32(base, at(step.b)));
        break;
      case Op::kAdd:
        r3 = game_modes::StateMachineAdd(base, at(step.c), at(step.a), at(step.b), replaced);
        break;
    }
  }
  return r3;
}

}  // namespace

uint32_t HubInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82190B18 Type2Hub::InitStates");
  return RunSteps(base, k82190B18, machine, frame, kHubInitFrame, replaced);
}

uint32_t SheetInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_821B90D8 PowerGraphScreen2::InitStates");
  return RunSteps(base, k821B90D8, machine, frame, kSheetInitFrame, replaced);
}

uint32_t BuildInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_821A16C0 BuildScreen::InitStates");
  return RunSteps(base, k821A16C0, machine, frame, kBuildInitFrame, replaced);
}

uint32_t PlateInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8219B2E8 PlateEditor::InitStates");
  return RunSteps(base, k8219B2E8, machine, frame, kPlateInitFrame, replaced);
}

}  // namespace type2_screens
