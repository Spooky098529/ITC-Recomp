// The type 12 mode's state machine set-ups (docs/functions/network-mode.md):
// the fourteen InitStates, NetScreen::InitBase and its eight base Inits.
// The originals' stores and copies in their frames replayed in their order
// from their step tables (init_tables.h), their transitions added through
// StateMachine::Add's plain C++, as the type 9 mode's Inits are
// (src/todays_result/init.cpp). The functors' storage words the originals
// never write are copied as the frames hold them (B7 changes no behavior).
// NetScreen::InitBase's call (kBase) runs the base Init nested, with its
// frame below the caller's, as the original's tail call leaves it.

#include "network_mode/network_mode.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

#include <span>

namespace network_mode {

namespace {

// A value: a constant, or an offset from the object (r3), the second
// object (r4) or the frame.
struct Value {
  enum Base : uint8_t { kConst, kObject, kArg, kFrame } base;
  uint32_t value;
};
constexpr Value C(uint32_t value) { return {Value::kConst, value}; }
constexpr Value O(uint32_t offset) { return {Value::kObject, offset}; }
constexpr Value A(uint32_t offset) { return {Value::kArg, offset}; }
constexpr Value F(uint32_t offset) { return {Value::kFrame, offset}; }

// kStore: [a] = b; kCopy: [a] = [b]; kAdd: StateMachine::Add(machine c,
// key a, functor part b); kBase: NetScreen::InitBase(r3 c, index a, r5 b).
enum class Op : uint8_t { kStore, kCopy, kAdd, kBase };
struct Step {
  Op op;
  Value a;
  Value b;
  Value c = C(0);
};

#include "network_mode/init_tables.h"

// The steps over `machine` (r3) and `object` (r4) in `frame` (its back
// chain written). r3 as the original leaves it: the last call's.
uint32_t Replay(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                uint32_t bytes, bool* replaced, std::span<const Step> steps) {
  auto at = [&](const Value& v) {
    switch (v.base) {
      case Value::kConst:
        return v.value;
      case Value::kObject:
        return machine + v.value;
      case Value::kArg:
        return object + v.value;
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
      case Op::kBase:
        InitBase(base, at(step.c), at(step.a), at(step.b), frame, replaced, &r3);
        break;
    }
  }
  return r3;
}

}  // namespace

uint32_t ModeSelectInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8227D680 NetModeSelect::InitStates");
  return Replay(base, machine, 0, frame, kModeSelectInitFrame, replaced, k8227D680);
}
uint32_t Screen1InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82276510 Type12Screen1::InitStates");
  return Replay(base, machine, 0, frame, kScreen1InitFrame, replaced, k82276510);
}
uint32_t Screen3InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8225F330 Type12Screen3::InitStates");
  return Replay(base, machine, 0, frame, kScreen3InitFrame, replaced, k8225F330);
}
uint32_t RankingInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_822873A0 NetRanking::InitStates");
  return Replay(base, machine, 0, frame, kRankingInitFrame, replaced, k822873A0);
}
uint32_t TaCourseInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82281858 NetTaCourse::InitStates");
  return Replay(base, machine, 0, frame, kTaCourseInitFrame, replaced, k82281858);
}
uint32_t Screen6InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82268410 Type12Screen6::InitStates");
  return Replay(base, machine, 0, frame, kScreen6InitFrame, replaced, k82268410);
}
uint32_t Screen7InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8226EB40 Type12Screen7::InitStates");
  return Replay(base, machine, 0, frame, kScreen7InitFrame, replaced, k8226EB40);
}
uint32_t Screen8InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82262A88 Type12Screen8::InitStates");
  return Replay(base, machine, 0, frame, kScreen8InitFrame, replaced, k82262A88);
}
uint32_t Screen9InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_822646B0 Type12Screen9::InitStates");
  return Replay(base, machine, 0, frame, kScreen9InitFrame, replaced, k822646B0);
}
uint32_t Dialog10InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82288600 Type12Dialog10::InitStates");
  return Replay(base, machine, 0, frame, kDialog10InitFrame, replaced, k82288600);
}
uint32_t Dialog11InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82277B18 Type12Dialog11::InitStates");
  return Replay(base, machine, 0, frame, kDialog11InitFrame, replaced, k82277B18);
}
uint32_t Dialog12InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8227F6C8 Type12Dialog12::InitStates");
  return Replay(base, machine, 0, frame, kDialog12InitFrame, replaced, k8227F6C8);
}
uint32_t Dialog13InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8227EA50 Type12Dialog13::InitStates");
  return Replay(base, machine, 0, frame, kDialog13InitFrame, replaced, k8227EA50);
}
uint32_t PowerGraphInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_821B96B8 PowerGraphScreen12::InitStates");
  return Replay(base, machine, 0, frame, kPowerGraphInitFrame, replaced, k821B96B8);
}

uint32_t Screen6InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                         bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82278C20 Type12Screen6::InitBase");
  return Replay(base, machine, object, frame, kInitBaseFrame, replaced, k82278C20);
}
uint32_t Screen7InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                         bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82279118 Type12Screen7::InitBase");
  return Replay(base, machine, object, frame, kInitBaseFrame, replaced, k82279118);
}
uint32_t Screen8InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                         bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8227A000 Type12Screen8::InitBase");
  return Replay(base, machine, object, frame, kInitBaseFrame, replaced, k8227A000);
}
uint32_t Screen9InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                         bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8227A4F8 Type12Screen9::InitBase");
  return Replay(base, machine, object, frame, kInitBaseFrame, replaced, k8227A4F8);
}
uint32_t Dialog10InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                          bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82279610 Type12Dialog10::InitBase");
  return Replay(base, machine, object, frame, kInitBaseFrame, replaced, k82279610);
}
uint32_t Dialog11InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                          bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82279B08 Type12Dialog11::InitBase");
  return Replay(base, machine, object, frame, kInitBaseFrame, replaced, k82279B08);
}
uint32_t Dialog12InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                          bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8227A9F0 Type12Dialog12::InitBase");
  return Replay(base, machine, object, frame, kInitBaseFrame, replaced, k8227A9F0);
}
uint32_t Dialog13InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                          bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8227AEE8 Type12Dialog13::InitBase");
  return Replay(base, machine, object, frame, kInitBaseFrame, replaced, k8227AEE8);
}

bool InitBase(uint8_t* base, uint32_t machine, uint32_t index, uint32_t object, uint32_t sp,
              bool* replaced, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8227B3E0 NetScreen::InitBase");
  using BaseInit = uint32_t (*)(uint8_t*, uint32_t, uint32_t, uint32_t, bool*);
  // The jump table's cases by index - 6 (case 0 through sub_8227B41C's jump).
  static constexpr BaseInit kCases[8] = {
      Screen6InitBase,  Screen7InitBase,  Screen8InitBase,  Screen9InitBase,
      Dialog10InitBase, Dialog11InitBase, Dialog12InitBase, Dialog13InitBase,
  };
  const uint32_t k = index - 6;
  if (k > 7) {
    return false;
  }
  *r3 = kCases[k](base, machine, object, sp - kInitBaseFrame, replaced);
  return true;
}

}  // namespace network_mode
