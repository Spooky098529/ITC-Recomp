// RaceSceneMachine::Init (sub_8215D808, docs/functions/race-scene.md) as
// plain C++: RaceScene::Create, the state (+9604) = 1, then the race scene's
// 11 transitions added (StateMachine::Add). As RaceStates::Init
// (src/race_states/race_states_init.cpp), the original builds each
// transition's functor and key in its own stack frame (864 bytes) and
// copies the functor's six storage words from a temporary there, three of
// which it never writes (five for the transitions without a handler, vtable
// 0x820069BC): the transitions keep whatever the stack held. So the stores
// and copies are replayed in the original's frame, in its order (the table
// below, decoded from the generated code by a script that followed its
// registers), and the transitions get the same words. The frame's back
// chain and saved registers aren't written.

#include "race_scene/race_scene.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

namespace race_scene {

namespace {

// A value: a constant, or an offset from the machine or the frame.
struct Value {
  enum Base : uint8_t { kConst, kMachine, kFrame } base;
  uint32_t value;
};
constexpr Value C(uint32_t value) { return {Value::kConst, value}; }
constexpr Value M(uint32_t offset) { return {Value::kMachine, offset}; }
constexpr Value F(uint32_t offset) { return {Value::kFrame, offset}; }

// kStore: [a] = b; kCopy6: six words from a to b, word by word; kAdd:
// StateMachine::Add(machine, key a, functor part b).
enum class Op : uint8_t { kStore, kCopy6, kAdd };
struct Step {
  Op op;
  Value a;
  Value b;
};

// Each transition: (state, message) -> next state (0: stay), its handler
// (functor vtable 0x82011918 with the machine and the handler; none:
// 0x820069BC). In order: (1, 0) -> 2 OnStart; (2, 3) -> 38 OnSetUp; (38, 0)
// OnUpdate; (38, 1) OnDraw; (38, 29) and (38, 58) TogglePause; (38, 44) ->
// 32; (32, 3) -> 38 Resume; (38, 49) -> 37; (37, 0) the empty sub_82106540;
// (37, 50) -> 35 OnEnd.
constexpr Step kSteps[] = {
    {Op::kStore, F(136), M(0)},
    {Op::kStore, M(9604), C(0x00000001)},
    {Op::kStore, F(132), C(0x82011918)},
    {Op::kStore, F(140), C(0x8215D098)},
    {Op::kStore, F(128), F(132)},
    {Op::kCopy6, F(132), F(676)},
    {Op::kStore, F(80), C(0x00000001)},
    {Op::kStore, F(672), F(676)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(700), C(0x00000002)},
    {Op::kAdd, F(80), F(672)},
    {Op::kStore, F(260), C(0x82011918)},
    {Op::kStore, F(264), M(0)},
    {Op::kStore, F(268), C(0x8215D528)},
    {Op::kStore, F(256), F(260)},
    {Op::kCopy6, F(260), F(484)},
    {Op::kStore, F(80), C(0x00000002)},
    {Op::kStore, F(84), C(0x00000003)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(480), F(484)},
    {Op::kStore, F(508), C(0x00000026)},
    {Op::kAdd, F(80), F(480)},
    {Op::kStore, F(196), C(0x82011918)},
    {Op::kStore, F(200), M(0)},
    {Op::kStore, F(204), C(0x8215D538)},
    {Op::kStore, F(192), F(196)},
    {Op::kCopy6, F(196), F(612)},
    {Op::kStore, F(636), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(608), F(612)},
    {Op::kAdd, F(80), F(608)},
    {Op::kStore, F(324), C(0x82011918)},
    {Op::kStore, F(328), M(0)},
    {Op::kStore, F(332), C(0x8215C770)},
    {Op::kStore, F(320), F(324)},
    {Op::kCopy6, F(324), F(548)},
    {Op::kStore, F(572), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x00000001)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(544), F(548)},
    {Op::kAdd, F(80), F(544)},
    {Op::kStore, F(100), C(0x82011918)},
    {Op::kStore, F(104), M(0)},
    {Op::kStore, F(108), C(0x8215C750)},
    {Op::kStore, F(96), F(100)},
    {Op::kCopy6, F(100), F(420)},
    {Op::kStore, F(444), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x0000001D)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(416), F(420)},
    {Op::kAdd, F(80), F(416)},
    {Op::kStore, F(164), C(0x82011918)},
    {Op::kStore, F(168), M(0)},
    {Op::kStore, F(172), C(0x8215C750)},
    {Op::kStore, F(160), F(164)},
    {Op::kCopy6, F(164), F(452)},
    {Op::kStore, F(476), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x0000003A)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(448), F(452)},
    {Op::kAdd, F(80), F(448)},
    {Op::kStore, F(772), C(0x820069BC)},
    {Op::kStore, F(768), F(772)},
    {Op::kCopy6, F(772), F(516)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x0000002C)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(512), F(516)},
    {Op::kStore, F(540), C(0x00000020)},
    {Op::kAdd, F(80), F(512)},
    {Op::kStore, F(228), C(0x82011918)},
    {Op::kStore, F(232), M(0)},
    {Op::kStore, F(236), C(0x8215D0D8)},
    {Op::kStore, F(224), F(228)},
    {Op::kCopy6, F(228), F(580)},
    {Op::kStore, F(604), C(0x00000026)},
    {Op::kStore, F(80), C(0x00000020)},
    {Op::kStore, F(84), C(0x00000003)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(576), F(580)},
    {Op::kAdd, F(80), F(576)},
    {Op::kStore, F(740), C(0x820069BC)},
    {Op::kStore, F(736), F(740)},
    {Op::kCopy6, F(740), F(644)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x00000031)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(640), F(644)},
    {Op::kStore, F(668), C(0x00000025)},
    {Op::kAdd, F(80), F(640)},
    {Op::kStore, F(292), C(0x82011918)},
    {Op::kStore, F(296), M(0)},
    {Op::kStore, F(300), C(0x82106540)},
    {Op::kStore, F(288), F(292)},
    {Op::kCopy6, F(292), F(708)},
    {Op::kStore, F(732), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000025)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(704), F(708)},
    {Op::kAdd, F(80), F(704)},
    {Op::kStore, F(356), C(0x82011918)},
    {Op::kStore, F(360), M(0)},
    {Op::kStore, F(364), C(0x8215D7D8)},
    {Op::kStore, F(352), F(356)},
    {Op::kCopy6, F(356), F(388)},
    {Op::kStore, F(80), C(0x00000025)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(384), F(388)},
    {Op::kStore, F(84), C(0x00000032)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(412), C(0x00000023)},
    {Op::kAdd, F(80), F(384)},
};

}  // namespace

std::optional<uint32_t> MachineInit(uint8_t* base, uint32_t machine, uint32_t frame,
                                    bool* replaced) {
  if (!SceneCreate(base)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8215D808 RaceSceneMachine::Init");
  auto at = [&](const Value& v) {
    switch (v.base) {
      case Value::kConst:
        return v.value;
      case Value::kMachine:
        return machine + v.value;
      case Value::kFrame:
        break;
    }
    return frame + v.value;
  };
  uint32_t r3 = 0;
  for (const Step& step : kSteps) {
    switch (step.op) {
      case Op::kStore:
        guest::StoreU32(base, at(step.a), at(step.b));
        break;
      case Op::kCopy6:
        for (uint32_t i = 0; i < 6; i++) {
          guest::StoreU32(base, at(step.b) + 4 * i, guest::LoadU32(base, at(step.a) + 4 * i));
        }
        break;
      case Op::kAdd:
        r3 = game_modes::StateMachineAdd(base, machine, at(step.a), at(step.b), replaced);
        break;
    }
  }
  return r3;
}

}  // namespace race_scene
