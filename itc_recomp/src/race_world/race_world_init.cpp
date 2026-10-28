// RaceWorld::Init (sub_82174D28, docs/functions/race-world.md) as plain C++:
// the race world's state (+9604 = 1) and two-human flag (+9608), then the
// race mode's state machine's 24 transitions added (StateMachine::Add). As
// RaceOverlaysMachine::Init (src/game_modes/race_overlays_machine.cpp), the
// original builds each transition's functor and key in its own stack frame
// (1,696 bytes) and copies the functor's six storage words from a temporary
// there, three of which it never writes: the transitions keep whatever the
// stack held. So the stores and copies are replayed in the original's frame,
// in its order (the table below, read from the generated code), and the
// transitions get the same words. The frame's back chain and saved
// registers aren't written.

#include "race_world/race_world.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_world {

namespace {

// A value: a constant, or an offset from the race world (the state machine)
// or the frame.
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
    {Op::kStore, F(424), M(0)},
    {Op::kStore, F(420), C(0x82011918)},
    {Op::kStore, F(428), C(0x82170550)},
    {Op::kStore, F(416), F(420)},
    {Op::kCopy6, F(420), F(612)},
    {Op::kStore, F(80), C(0x00000001)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(608), F(612)},
    {Op::kStore, F(636), C(0x00000002)},
    {Op::kAdd, F(80), F(608)},
    {Op::kStore, F(164), C(0x82011918)},
    {Op::kStore, F(168), M(0)},
    {Op::kStore, F(172), C(0x82173A90)},
    {Op::kStore, F(160), F(164)},
    {Op::kCopy6, F(164), F(1124)},
    {Op::kStore, F(1148), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000002)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(84), C(0x00000003)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1120), F(1124)},
    {Op::kAdd, F(80), F(1120)},
    {Op::kStore, F(356), C(0x82011918)},
    {Op::kStore, F(360), M(0)},
    {Op::kStore, F(364), C(0x82106540)},
    {Op::kStore, F(352), F(356)},
    {Op::kCopy6, F(356), F(676)},
    {Op::kStore, F(700), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000002)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(672), F(676)},
    {Op::kAdd, F(80), F(672)},
    {Op::kStore, F(1444), C(0x820069BC)},
    {Op::kStore, F(1440), F(1444)},
    {Op::kCopy6, F(1444), F(548)},
    {Op::kStore, F(572), C(0x00000003)},
    {Op::kStore, F(80), C(0x00000002)},
    {Op::kStore, F(84), C(0x00000004)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(544), F(548)},
    {Op::kAdd, F(80), F(544)},
    {Op::kStore, F(228), C(0x82011918)},
    {Op::kStore, F(232), M(0)},
    {Op::kStore, F(236), C(0x82171360)},
    {Op::kStore, F(224), F(228)},
    {Op::kCopy6, F(228), F(740)},
    {Op::kStore, F(80), C(0x00000003)},
    {Op::kStore, F(84), C(0x00000005)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(736), F(740)},
    {Op::kStore, F(764), C(0x00000007)},
    {Op::kAdd, F(80), F(736)},
    {Op::kStore, F(484), C(0x82011918)},
    {Op::kStore, F(488), M(0)},
    {Op::kStore, F(492), C(0x82171360)},
    {Op::kStore, F(480), F(484)},
    {Op::kCopy6, F(484), F(1252)},
    {Op::kStore, F(1276), C(0x00000007)},
    {Op::kStore, F(80), C(0x00000003)},
    {Op::kStore, F(84), C(0x0000002E)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1248), F(1252)},
    {Op::kAdd, F(80), F(1248)},
    {Op::kStore, F(292), C(0x82011918)},
    {Op::kStore, F(296), M(0)},
    {Op::kStore, F(300), C(0x821713C0)},
    {Op::kStore, F(288), F(292)},
    {Op::kCopy6, F(292), F(804)},
    {Op::kStore, F(828), C(0x00000007)},
    {Op::kStore, F(80), C(0x00000003)},
    {Op::kStore, F(84), C(0x00000011)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(800), F(804)},
    {Op::kAdd, F(80), F(800)},
    {Op::kStore, F(1572), C(0x820069BC)},
    {Op::kStore, F(1568), F(1572)},
    {Op::kCopy6, F(1572), F(1060)},
    {Op::kStore, F(80), C(0x00000003)},
    {Op::kStore, F(84), C(0x0000002A)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1056), F(1060)},
    {Op::kStore, F(1084), C(0x00000018)},
    {Op::kAdd, F(80), F(1056)},
    {Op::kStore, F(100), C(0x82011918)},
    {Op::kStore, F(104), M(0)},
    {Op::kStore, F(108), C(0x82174BC0)},
    {Op::kStore, F(96), F(100)},
    {Op::kCopy6, F(100), F(868)},
    {Op::kStore, F(892), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000007)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(864), F(868)},
    {Op::kAdd, F(80), F(864)},
    {Op::kStore, F(132), C(0x82011918)},
    {Op::kStore, F(136), M(0)},
    {Op::kStore, F(140), C(0x82171360)},
    {Op::kStore, F(128), F(132)},
    {Op::kCopy6, F(132), F(1188)},
    {Op::kStore, F(1212), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000007)},
    {Op::kStore, F(84), C(0x0000001C)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1184), F(1188)},
    {Op::kAdd, F(80), F(1184)},
    {Op::kStore, F(1348), C(0x820069BC)},
    {Op::kStore, F(1344), F(1348)},
    {Op::kCopy6, F(1348), F(932)},
    {Op::kStore, F(956), C(0x00000018)},
    {Op::kStore, F(80), C(0x00000007)},
    {Op::kStore, F(84), C(0x0000001D)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(928), F(932)},
    {Op::kAdd, F(80), F(928)},
    {Op::kStore, F(1412), C(0x820069BC)},
    {Op::kStore, F(1408), F(1412)},
    {Op::kCopy6, F(1412), F(580)},
    {Op::kStore, F(604), C(0x00000018)},
    {Op::kStore, F(80), C(0x00000007)},
    {Op::kStore, F(84), C(0x0000001F)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(576), F(580)},
    {Op::kAdd, F(80), F(576)},
    {Op::kStore, F(1476), C(0x820069BC)},
    {Op::kStore, F(1472), F(1476)},
    {Op::kCopy6, F(1476), F(644)},
    {Op::kStore, F(668), C(0x00000018)},
    {Op::kStore, F(80), C(0x00000007)},
    {Op::kStore, F(84), C(0x0000003A)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(640), F(644)},
    {Op::kAdd, F(80), F(640)},
    {Op::kStore, F(196), C(0x82011918)},
    {Op::kStore, F(200), M(0)},
    {Op::kStore, F(204), C(0x82171308)},
    {Op::kStore, F(192), F(196)},
    {Op::kCopy6, F(196), F(708)},
    {Op::kStore, F(80), C(0x00000007)},
    {Op::kStore, F(84), C(0x0000002C)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(704), F(708)},
    {Op::kStore, F(732), C(0x00000020)},
    {Op::kAdd, F(80), F(704)},
    {Op::kStore, F(260), C(0x82011918)},
    {Op::kStore, F(264), M(0)},
    {Op::kStore, F(268), C(0x82170560)},
    {Op::kStore, F(256), F(260)},
    {Op::kCopy6, F(260), F(772)},
    {Op::kStore, F(796), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(768), F(772)},
    {Op::kAdd, F(80), F(768)},
    {Op::kStore, F(1540), C(0x820069BC)},
    {Op::kStore, F(1536), F(1540)},
    {Op::kCopy6, F(1540), F(836)},
    {Op::kStore, F(860), C(0x00000007)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x0000001D)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(832), F(836)},
    {Op::kAdd, F(80), F(832)},
    {Op::kStore, F(1604), C(0x820069BC)},
    {Op::kStore, F(1600), F(1604)},
    {Op::kCopy6, F(1604), F(900)},
    {Op::kStore, F(924), C(0x00000007)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x0000003A)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(896), F(900)},
    {Op::kAdd, F(80), F(896)},
    {Op::kStore, F(324), C(0x82011918)},
    {Op::kStore, F(328), M(0)},
    {Op::kStore, F(332), C(0x82171308)},
    {Op::kStore, F(320), F(324)},
    {Op::kCopy6, F(324), F(964)},
    {Op::kStore, F(988), C(0x00000020)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x0000002C)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(960), F(964)},
    {Op::kAdd, F(80), F(960)},
    {Op::kStore, F(388), C(0x82011918)},
    {Op::kStore, F(392), M(0)},
    {Op::kStore, F(396), C(0x82170560)},
    {Op::kStore, F(384), F(388)},
    {Op::kCopy6, F(388), F(1028)},
    {Op::kStore, F(1052), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1024), F(1028)},
    {Op::kAdd, F(80), F(1024)},
    {Op::kStore, F(1380), C(0x820069BC)},
    {Op::kStore, F(1376), F(1380)},
    {Op::kCopy6, F(1380), F(1092)},
    {Op::kStore, F(1116), C(0x00000007)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x0000001F)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1088), F(1092)},
    {Op::kAdd, F(80), F(1088)},
    {Op::kStore, F(1508), C(0x820069BC)},
    {Op::kStore, F(1504), F(1508)},
    {Op::kCopy6, F(1508), F(1156)},
    {Op::kStore, F(80), C(0x00000007)},
    {Op::kStore, F(84), C(0x00000031)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1152), F(1156)},
    {Op::kStore, F(1180), C(0x00000025)},
    {Op::kAdd, F(80), F(1152)},
    {Op::kStore, F(1316), C(0x820069BC)},
    {Op::kStore, F(1312), F(1316)},
    {Op::kCopy6, F(1316), F(1220)},
    {Op::kStore, F(1244), C(0x00000025)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x00000031)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1216), F(1220)},
    {Op::kAdd, F(80), F(1216)},
    {Op::kStore, F(452), C(0x82011918)},
    {Op::kStore, F(456), M(0)},
    {Op::kStore, F(460), C(0x82106540)},
    {Op::kStore, F(448), F(452)},
    {Op::kCopy6, F(452), F(1284)},
    {Op::kStore, F(1308), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000025)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1280), F(1284)},
    {Op::kAdd, F(80), F(1280)},
    {Op::kStore, F(516), C(0x82011918)},
    {Op::kStore, F(520), M(0)},
    {Op::kStore, F(524), C(0x82171250)},
    {Op::kStore, F(512), F(516)},
    {Op::kCopy6, F(516), F(996)},
    {Op::kStore, F(80), C(0x00000025)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(992), F(996)},
    {Op::kStore, F(84), C(0x00000032)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(1020), C(0x00000023)},
    {Op::kAdd, F(80), F(992)},
};

}  // namespace

uint32_t Init(uint8_t* base, uint32_t world, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82174D28 RaceWorld::Init");
  guest::StoreU32(base, world + 9604, 1);
  guest::StoreU32(base, world + 9608, 0);
  const uint32_t race = guest::LoadU32(base, kRaceData);
  if (int32_t(guest::LoadU32(base, race + 0xA3AF0)) == 6 &&
      int32_t(vehicle_accessors::StagesWord76(base, race + 0x59620, 0xFFFFFFFF)) >= 2) {
    guest::StoreU32(base, world + 9608, 1);
  }
  auto at = [&](const Value& v) {
    switch (v.base) {
      case Value::kConst:
        return v.value;
      case Value::kMachine:
        return world + v.value;
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
        r3 = game_modes::StateMachineAdd(base, world, at(step.a), at(step.b), replaced);
        break;
    }
  }
  return r3;
}

}  // namespace race_world
