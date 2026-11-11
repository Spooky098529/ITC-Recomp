// The title load's and the title screen's state machine set-ups
// (docs/functions/title-screen.md): the originals' stores and copies in
// their frames replayed in their order from their step tables
// (init_tables.h), their transitions added through StateMachine::Add's plain
// C++, as the type 2 screens' Inits are (src/type2_screens/init_states.cpp).
// The functors' storage words the originals never write are copied as the
// frame holds them (B7 changes no behavior).

#include "title_screen/title_screen.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

#include <span>

namespace title_screen {

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

#include "title_screen/init_tables.h"

uint32_t Run(uint8_t* base, std::span<const Step> steps, uint32_t machine, uint32_t frame,
             uint32_t frame_bytes, bool* replaced) {
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
  guest::StoreU32(base, frame, frame + frame_bytes);
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

uint32_t LoadInit(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_822213B0 TitleLoad::Init");
  return Run(base, k822213B0, machine, frame, kLoadInitFrame, replaced);
}

uint32_t ScreenInit(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82224270 TitleScreen::Init");
  return Run(base, k82224270, machine, frame, kScreenInitFrame, replaced);
}

}  // namespace title_screen
