// The parking area's state machine set-up, LocationScreen::InitStates
// (docs/functions/location-screen.md): the original's stores replayed in
// order, its 19 transitions added through StateMachine::Add's plain C++ (as
// RaceOverlaysMachine::Init's in src/game_modes/race_overlays_machine.cpp).
// The step table was made from the translated function by following its
// registers (constants, the machine's and the frame's offsets) and its six
// word copy loops.

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

namespace location_screen {

namespace {

// A value: a constant, or an offset from the state machine or the frame.
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

constexpr Step kSteps[] = {
    {Op::kStore, F(680), M(0)},
    {Op::kStore, F(676), C(0x82011918)},
    {Op::kStore, F(684), C(0x82227A50)},
    {Op::kStore, F(672), F(676)},
    {Op::kCopy6, F(676), F(708)},
    {Op::kStore, F(732), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000000)},
    {Op::kStore, F(84), C(0x00000001)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(704), F(708)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kAdd, F(80), F(704)},
    {Op::kStore, F(644), C(0x82011918)},
    {Op::kStore, F(648), M(0)},
    {Op::kStore, F(652), C(0x8222C300)},
    {Op::kStore, F(640), F(644)},
    {Op::kCopy6, F(644), F(1252)},
    {Op::kStore, F(1248), F(1252)},
    {Op::kStore, F(84), C(0x00000001)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1276), C(0x00000002)},
    {Op::kStore, F(80), C(0x00000001)},
    {Op::kAdd, F(80), F(1248)},
    {Op::kStore, F(196), C(0x82011918)},
    {Op::kStore, F(200), M(0)},
    {Op::kStore, F(204), C(0x82229238)},
    {Op::kStore, F(192), F(196)},
    {Op::kCopy6, F(196), F(804)},
    {Op::kStore, F(828), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000002)},
    {Op::kStore, F(84), C(0x00000001)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(800), F(804)},
    {Op::kAdd, F(80), F(800)},
    {Op::kStore, F(452), C(0x82011918)},
    {Op::kStore, F(456), M(0)},
    {Op::kStore, F(460), C(0x8222F0B0)},
    {Op::kStore, F(448), F(452)},
    {Op::kCopy6, F(452), F(1060)},
    {Op::kStore, F(1056), F(1060)},
    {Op::kStore, F(84), C(0x00000001)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1084), C(0x00000005)},
    {Op::kStore, F(80), C(0x00000004)},
    {Op::kAdd, F(80), F(1056)},
    {Op::kStore, F(260), C(0x82011918)},
    {Op::kStore, F(264), M(0)},
    {Op::kStore, F(268), C(0x8222EC08)},
    {Op::kStore, F(256), F(260)},
    {Op::kCopy6, F(260), F(868)},
    {Op::kStore, F(892), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000001)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(864), F(868)},
    {Op::kAdd, F(80), F(864)},
    {Op::kStore, F(580), C(0x82011918)},
    {Op::kStore, F(584), M(0)},
    {Op::kStore, F(588), C(0x8222ED98)},
    {Op::kStore, F(576), F(580)},
    {Op::kCopy6, F(580), F(1188)},
    {Op::kStore, F(1212), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1184), F(1188)},
    {Op::kAdd, F(80), F(1184)},
    {Op::kStore, F(324), C(0x82011918)},
    {Op::kStore, F(328), M(0)},
    {Op::kStore, F(332), C(0x822291A8)},
    {Op::kStore, F(320), F(324)},
    {Op::kCopy6, F(324), F(932)},
    {Op::kStore, F(956), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000005)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(928), F(932)},
    {Op::kAdd, F(80), F(928)},
    {Op::kStore, F(516), C(0x82011918)},
    {Op::kStore, F(520), M(0)},
    {Op::kStore, F(524), C(0x82227158)},
    {Op::kStore, F(512), F(516)},
    {Op::kCopy6, F(516), F(1124)},
    {Op::kStore, F(1148), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x0000000E)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1120), F(1124)},
    {Op::kAdd, F(80), F(1120)},
    {Op::kStore, F(388), C(0x82011918)},
    {Op::kStore, F(392), M(0)},
    {Op::kStore, F(396), C(0x822271B8)},
    {Op::kStore, F(384), F(388)},
    {Op::kCopy6, F(388), F(996)},
    {Op::kStore, F(1020), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x0000000F)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(992), F(996)},
    {Op::kAdd, F(80), F(992)},
    {Op::kStore, F(132), C(0x82011918)},
    {Op::kStore, F(136), M(0)},
    {Op::kStore, F(140), C(0x822271B8)},
    {Op::kStore, F(128), F(132)},
    {Op::kCopy6, F(132), F(740)},
    {Op::kStore, F(764), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000011)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(736), F(740)},
    {Op::kAdd, F(80), F(736)},
    {Op::kStore, F(164), C(0x82011918)},
    {Op::kStore, F(168), M(0)},
    {Op::kStore, F(172), C(0x8222C9B0)},
    {Op::kStore, F(160), F(164)},
    {Op::kCopy6, F(164), F(772)},
    {Op::kStore, F(796), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000012)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(768), F(772)},
    {Op::kAdd, F(80), F(768)},
    {Op::kStore, F(228), C(0x82011918)},
    {Op::kStore, F(232), M(0)},
    {Op::kStore, F(236), C(0x822271D8)},
    {Op::kStore, F(224), F(228)},
    {Op::kCopy6, F(228), F(836)},
    {Op::kStore, F(860), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000014)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(832), F(836)},
    {Op::kAdd, F(80), F(832)},
    {Op::kStore, F(292), C(0x82011918)},
    {Op::kStore, F(296), M(0)},
    {Op::kStore, F(300), C(0x82229578)},
    {Op::kStore, F(288), F(292)},
    {Op::kCopy6, F(292), F(900)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000006)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(896), F(900)},
    {Op::kStore, F(924), C(0x00000008)},
    {Op::kAdd, F(80), F(896)},
    {Op::kStore, F(356), C(0x82011918)},
    {Op::kStore, F(360), M(0)},
    {Op::kStore, F(364), C(0x82229610)},
    {Op::kStore, F(352), F(356)},
    {Op::kCopy6, F(356), F(964)},
    {Op::kStore, F(988), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000008)},
    {Op::kStore, F(84), C(0x00000003)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(960), F(964)},
    {Op::kAdd, F(80), F(960)},
    {Op::kStore, F(420), C(0x82011918)},
    {Op::kStore, F(424), M(0)},
    {Op::kStore, F(428), C(0x822895E0)},
    {Op::kStore, F(416), F(420)},
    {Op::kCopy6, F(420), F(1028)},
    {Op::kStore, F(1052), C(0x00000000)},
    {Op::kStore, F(84), C(0x00000007)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1024), F(1028)},
    {Op::kStore, F(80), C(0x00000009)},
    {Op::kAdd, F(80), F(1024)},
    {Op::kStore, F(484), C(0x82011918)},
    {Op::kStore, F(488), M(0)},
    {Op::kStore, F(492), C(0x82229678)},
    {Op::kStore, F(480), F(484)},
    {Op::kCopy6, F(484), F(1092)},
    {Op::kStore, F(1116), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000004)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1088), F(1092)},
    {Op::kAdd, F(80), F(1088)},
    {Op::kStore, F(548), C(0x82011918)},
    {Op::kStore, F(552), M(0)},
    {Op::kStore, F(556), C(0x82229678)},
    {Op::kStore, F(544), F(548)},
    {Op::kCopy6, F(548), F(1156)},
    {Op::kStore, F(1180), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000009)},
    {Op::kStore, F(84), C(0x00000004)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1152), F(1156)},
    {Op::kAdd, F(80), F(1152)},
    {Op::kStore, F(612), C(0x82011918)},
    {Op::kStore, F(616), M(0)},
    {Op::kStore, F(620), C(0x82229678)},
    {Op::kStore, F(608), F(612)},
    {Op::kCopy6, F(612), F(1220)},
    {Op::kStore, F(1244), C(0x00000000)},
    {Op::kStore, F(1216), F(1220)},
    {Op::kStore, F(84), C(0x00000004)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(80), C(0x0000000A)},
    {Op::kAdd, F(80), F(1216)},
    {Op::kStore, F(100), C(0x82011918)},
    {Op::kStore, F(104), M(0)},
    {Op::kStore, F(108), C(0x82228548)},
    {Op::kStore, F(96), F(100)},
    {Op::kCopy6, F(100), F(1284)},
    {Op::kStore, F(1280), F(1284)},
    {Op::kStore, F(84), C(0x00000004)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1308), C(0x00000006)},
    {Op::kStore, F(80), C(0x00000007)},
    {Op::kAdd, F(80), F(1280)},
    {Op::kStore, M(9604), C(0x00000000)},
};

}  // namespace

uint32_t InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_8222F258 LocationScreen::InitStates");
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

}  // namespace location_screen
