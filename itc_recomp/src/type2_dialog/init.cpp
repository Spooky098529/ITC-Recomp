// The type 2 dialog's state machine set-up (docs/functions/type2-dialog.md):
// the original's stores and copies in its frame replayed in their order
// from its step table (init_tables.h), its transitions added through
// StateMachine::Add's plain C++, as the type 2 screens' Inits are
// (src/type2_screens/init_states.cpp). The functors' storage words the
// original never writes are copied as the frame holds them (B7 changes no
// behavior).

#include "type2_dialog/type2_dialog.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

namespace type2_dialog {

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

#include "type2_dialog/init_tables.h"

}  // namespace

uint32_t Init(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82188178 Type2Dialog::Init");
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
  guest::StoreU32(base, frame, frame + kInitFrame);
  uint32_t r3 = machine;
  *replaced = false;
  for (const Step& step : k82188178) {
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

}  // namespace type2_dialog
