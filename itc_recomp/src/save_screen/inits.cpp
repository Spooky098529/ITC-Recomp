// The save screen's state machine Inits in the type 2, 4 (the title) and 10
// modes (docs/functions/save-screen.md) as plain C++: each original's stores
// and copies in its frame and its StateMachine::Add calls replayed in its
// order from its step table (init_tables.h), as Type6SaveScreen::Init's in
// src/type67_modes/inits.cpp. The functors' storage words the originals
// never write are copied as the frame holds them (B7 changes no behavior).

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "save_screen/save_screen.h"

#include <cstdlib>

namespace save_screen {

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

#include "save_screen/init_tables.h"

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
    {0x821A6E18, "Type2SaveScreen::Init", 976, 27},
    {0x821A7470, "Type4SaveScreen::Init", 976, 27},
    {0x821A7AC8, "Type10SaveScreen::Init", 1104, 27},
};

#define ITC_SAVE_SCREEN_INIT(address, name)                \
  case 0x##address: {                                      \
    ITC_NATIVE_RUNNING("sub_" #address " " name);          \
    guest::StoreU32(base, frame, frame + info_frame);      \
    return Run(base, k##address, object, frame);           \
  }

InitResult RunInit(uint8_t* base, uint32_t address, uint32_t object, uint32_t frame) {
  uint32_t info_frame = 0;
  for (const InitInfo& info : kInits) {
    if (info.address == address) {
      info_frame = info.frame;
    }
  }
  switch (address) {
    ITC_SAVE_SCREEN_INIT(821A6E18, "Type2SaveScreen::Init")
    ITC_SAVE_SCREEN_INIT(821A7470, "Type4SaveScreen::Init")
    ITC_SAVE_SCREEN_INIT(821A7AC8, "Type10SaveScreen::Init")
    default:
      break;
  }
  ITC_LOG_ERROR("save_screen: no Init at {:08X}", address);
  std::abort();
}

#undef ITC_SAVE_SCREEN_INIT

}  // namespace save_screen
