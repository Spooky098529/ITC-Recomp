// The camera director's state machine and views (docs/functions/
// camera-modes.md): CameraDirector::InitStates. Plain C++ over guest
// memory, the originals' loads and stores in their order.

#include "camera_modes/camera_modes.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "track_zones/track_zones.h"

namespace camera_modes {

namespace {

// --- CameraDirector::InitStates's transitions ---
//
// As RaceWorld::Init (src/race_world/race_world_init.cpp), the original
// builds each transition's functor and key in its own stack frame (2,368
// bytes) and copies the functor's six storage words from a temporary there,
// some of which it never writes: the transitions keep whatever the stack
// held. So the stores and copies are replayed in the original's frame, in
// its order (the table below, read from the generated code), and the
// transitions get the same words. The functor's member function pointer is
// stored as two words (+80 and +84) and read back as one doubleword, whose
// halves the original stores apart: those are the kCopy steps. The frame's
// back chain and saved registers aren't written.

// A value: a constant, or an offset from the director or the frame.
struct Value {
  enum Base : uint8_t { kConst, kDirector, kFrame } base;
  uint32_t value;
};
constexpr Value C(uint32_t value) { return {Value::kConst, value}; }
constexpr Value M(uint32_t offset) { return {Value::kDirector, offset}; }
constexpr Value F(uint32_t offset) { return {Value::kFrame, offset}; }

// kStore: [a] = b; kCopy: one word from a to b; kCopy6: six words from a to
// b, word by word; kAdd: StateMachine::Add(the director's +16, key a,
// functor part b).
enum class Op : uint8_t { kStore, kCopy, kCopy6, kAdd };
struct Step {
  Op op;
  Value a;
  Value b;
};

constexpr Step kInitSteps[] = {
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(80), C(0x82152338)},
    {Op::kStore, F(584), M(0)},
    {Op::kStore, F(580), C(0x82010978)},
    {Op::kStore, M(9620), C(0x00000001)},
    {Op::kCopy, F(84), F(592)},
    {Op::kCopy, F(80), F(588)},
    {Op::kStore, F(576), F(580)},
    {Op::kCopy6, F(580), F(836)},
    {Op::kStore, F(80), C(0x00000001)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(832), F(836)},
    {Op::kStore, F(860), C(0x00000003)},
    {Op::kAdd, F(80), F(832)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(452), C(0x82010978)},
    {Op::kStore, F(456), M(0)},
    {Op::kStore, F(80), C(0x821596B0)},
    {Op::kCopy, F(84), F(464)},
    {Op::kCopy, F(80), F(460)},
    {Op::kStore, F(448), F(452)},
    {Op::kCopy6, F(452), F(1348)},
    {Op::kStore, F(80), C(0x00000003)},
    {Op::kStore, F(84), C(0x00000003)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1344), F(1348)},
    {Op::kStore, F(1372), C(0x00000004)},
    {Op::kAdd, F(80), F(1344)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(196), C(0x82010978)},
    {Op::kStore, F(200), M(0)},
    {Op::kStore, F(80), C(0x821594B8)},
    {Op::kCopy, F(84), F(208)},
    {Op::kCopy, F(80), F(204)},
    {Op::kStore, F(192), F(196)},
    {Op::kCopy6, F(196), F(900)},
    {Op::kStore, F(924), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000004)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(896), F(900)},
    {Op::kAdd, F(80), F(896)},
    {Op::kStore, F(1956), C(0x820069BC)},
    {Op::kStore, F(1952), F(1956)},
    {Op::kCopy6, F(1956), F(1604)},
    {Op::kStore, F(80), C(0x00000004)},
    {Op::kStore, F(1628), C(0x00000005)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(84), C(0x00000005)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1600), F(1604)},
    {Op::kAdd, F(80), F(1600)},
    {Op::kStore, F(2212), C(0x820069BC)},
    {Op::kStore, F(2208), F(2212)},
    {Op::kCopy6, F(2212), F(772)},
    {Op::kStore, F(80), C(0x00000004)},
    {Op::kStore, F(84), C(0x0000000E)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(768), F(772)},
    {Op::kStore, F(796), C(0x0000000A)},
    {Op::kAdd, F(80), F(768)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(708), C(0x82010978)},
    {Op::kStore, F(712), M(0)},
    {Op::kStore, F(80), C(0x82152400)},
    {Op::kCopy, F(84), F(720)},
    {Op::kCopy, F(80), F(716)},
    {Op::kStore, F(704), F(708)},
    {Op::kCopy6, F(708), F(1412)},
    {Op::kStore, F(80), C(0x00000004)},
    {Op::kStore, F(84), C(0x0000002B)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1408), F(1412)},
    {Op::kStore, F(1436), C(0x00000026)},
    {Op::kAdd, F(80), F(1408)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(260), C(0x82010978)},
    {Op::kStore, F(264), M(0)},
    {Op::kStore, F(80), C(0x82152400)},
    {Op::kCopy, F(84), F(272)},
    {Op::kCopy, F(80), F(268)},
    {Op::kStore, F(256), F(260)},
    {Op::kCopy6, F(260), F(1028)},
    {Op::kStore, F(1052), C(0x00000026)},
    {Op::kStore, F(80), C(0x00000004)},
    {Op::kStore, F(84), C(0x0000002E)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1024), F(1028)},
    {Op::kAdd, F(80), F(1024)},
    {Op::kStore, F(2020), C(0x820069BC)},
    {Op::kStore, F(2016), F(2020)},
    {Op::kCopy6, F(2020), F(1732)},
    {Op::kStore, F(80), C(0x00000004)},
    {Op::kStore, F(84), C(0x0000002C)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1728), F(1732)},
    {Op::kStore, F(1756), C(0x00000020)},
    {Op::kAdd, F(80), F(1728)},
    {Op::kStore, F(2148), C(0x820069BC)},
    {Op::kStore, F(2144), F(2148)},
    {Op::kCopy6, F(2148), F(1092)},
    {Op::kStore, F(1116), C(0x00000026)},
    {Op::kStore, F(80), C(0x00000004)},
    {Op::kStore, F(84), C(0x00000011)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1088), F(1092)},
    {Op::kAdd, F(80), F(1088)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(516), C(0x82010978)},
    {Op::kStore, F(520), M(0)},
    {Op::kStore, F(80), C(0x821594B8)},
    {Op::kCopy, F(84), F(528)},
    {Op::kCopy, F(80), F(524)},
    {Op::kStore, F(512), F(516)},
    {Op::kCopy6, F(516), F(1476)},
    {Op::kStore, F(1500), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1472), F(1476)},
    {Op::kAdd, F(80), F(1472)},
    {Op::kStore, F(1892), C(0x820069BC)},
    {Op::kStore, F(1888), F(1892)},
    {Op::kCopy6, F(1892), F(1156)},
    {Op::kStore, F(1180), C(0x00000026)},
    {Op::kStore, F(80), C(0x00000005)},
    {Op::kStore, F(84), C(0x00000007)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1152), F(1156)},
    {Op::kAdd, F(80), F(1152)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(324), C(0x82010978)},
    {Op::kStore, F(328), M(0)},
    {Op::kStore, F(80), C(0x821594B8)},
    {Op::kCopy, F(84), F(336)},
    {Op::kCopy, F(80), F(332)},
    {Op::kStore, F(320), F(324)},
    {Op::kCopy6, F(324), F(1668)},
    {Op::kStore, F(1692), C(0x00000000)},
    {Op::kStore, F(80), C(0x0000000A)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1664), F(1668)},
    {Op::kAdd, F(80), F(1664)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(644), C(0x82010978)},
    {Op::kStore, F(648), M(0)},
    {Op::kStore, F(80), C(0x821524B8)},
    {Op::kCopy, F(84), F(656)},
    {Op::kCopy, F(80), F(652)},
    {Op::kStore, F(640), F(644)},
    {Op::kCopy6, F(644), F(1220)},
    {Op::kStore, F(80), C(0x0000000A)},
    {Op::kStore, F(84), C(0x0000000A)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1216), F(1220)},
    {Op::kStore, F(1244), C(0x0000000B)},
    {Op::kAdd, F(80), F(1216)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(388), C(0x82010978)},
    {Op::kStore, F(392), M(0)},
    {Op::kStore, F(80), C(0x82159948)},
    {Op::kCopy, F(84), F(400)},
    {Op::kCopy, F(80), F(396)},
    {Op::kStore, F(384), F(388)},
    {Op::kCopy6, F(388), F(1540)},
    {Op::kStore, F(1564), C(0x00000000)},
    {Op::kStore, F(80), C(0x0000000B)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1536), F(1540)},
    {Op::kAdd, F(80), F(1536)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(100), C(0x82010978)},
    {Op::kStore, F(104), M(0)},
    {Op::kStore, F(80), C(0x821524D8)},
    {Op::kCopy, F(84), F(112)},
    {Op::kCopy, F(80), F(108)},
    {Op::kStore, F(96), F(100)},
    {Op::kCopy6, F(100), F(1284)},
    {Op::kStore, F(1308), C(0x00000026)},
    {Op::kStore, F(80), C(0x0000000B)},
    {Op::kStore, F(84), C(0x0000000B)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1280), F(1284)},
    {Op::kAdd, F(80), F(1280)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(132), C(0x82010978)},
    {Op::kStore, F(136), M(0)},
    {Op::kStore, F(80), C(0x821594B8)},
    {Op::kCopy, F(84), F(144)},
    {Op::kCopy, F(80), F(140)},
    {Op::kStore, F(128), F(132)},
    {Op::kCopy6, F(132), F(1796)},
    {Op::kStore, F(1820), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1792), F(1796)},
    {Op::kAdd, F(80), F(1792)},
    {Op::kStore, F(1924), C(0x820069BC)},
    {Op::kStore, F(1920), F(1924)},
    {Op::kCopy6, F(1924), F(804)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x0000001D)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(800), F(804)},
    {Op::kStore, F(828), C(0x00000018)},
    {Op::kAdd, F(80), F(800)},
    {Op::kStore, F(1988), C(0x820069BC)},
    {Op::kStore, F(1984), F(1988)},
    {Op::kCopy6, F(1988), F(868)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x00000031)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(864), F(868)},
    {Op::kStore, F(892), C(0x00000025)},
    {Op::kAdd, F(80), F(864)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(164), C(0x82010978)},
    {Op::kStore, F(168), M(0)},
    {Op::kStore, F(80), C(0x821597F0)},
    {Op::kCopy, F(84), F(176)},
    {Op::kCopy, F(80), F(172)},
    {Op::kStore, F(160), F(164)},
    {Op::kCopy6, F(164), F(932)},
    {Op::kStore, F(956), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x00000025)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(928), F(932)},
    {Op::kAdd, F(80), F(928)},
    {Op::kStore, F(2052), C(0x820069BC)},
    {Op::kStore, F(2048), F(2052)},
    {Op::kCopy6, F(2052), F(996)},
    {Op::kStore, F(1020), C(0x0000000A)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x0000000E)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(992), F(996)},
    {Op::kAdd, F(80), F(992)},
    {Op::kStore, F(2116), C(0x820069BC)},
    {Op::kStore, F(2112), F(2116)},
    {Op::kCopy6, F(2116), F(1060)},
    {Op::kStore, F(1084), C(0x0000000A)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x00000018)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1056), F(1060)},
    {Op::kAdd, F(80), F(1056)},
    {Op::kStore, F(2180), C(0x820069BC)},
    {Op::kStore, F(2176), F(2180)},
    {Op::kCopy6, F(2180), F(1124)},
    {Op::kStore, F(1148), C(0x00000020)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x0000002C)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1120), F(1124)},
    {Op::kAdd, F(80), F(1120)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(228), C(0x82010978)},
    {Op::kStore, F(232), M(0)},
    {Op::kStore, F(80), C(0x82152400)},
    {Op::kCopy, F(84), F(240)},
    {Op::kCopy, F(80), F(236)},
    {Op::kStore, F(224), F(228)},
    {Op::kCopy6, F(228), F(1188)},
    {Op::kStore, F(1212), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x0000002B)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1184), F(1188)},
    {Op::kAdd, F(80), F(1184)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(292), C(0x82010978)},
    {Op::kStore, F(296), M(0)},
    {Op::kStore, F(80), C(0x82153160)},
    {Op::kCopy, F(84), F(304)},
    {Op::kCopy, F(80), F(300)},
    {Op::kStore, F(288), F(292)},
    {Op::kCopy6, F(292), F(1252)},
    {Op::kStore, F(80), C(0x00000026)},
    {Op::kStore, F(84), C(0x0000001B)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1248), F(1252)},
    {Op::kStore, F(1276), C(0x00000015)},
    {Op::kAdd, F(80), F(1248)},
    {Op::kStore, F(2244), C(0x820069BC)},
    {Op::kStore, F(2240), F(2244)},
    {Op::kCopy6, F(2244), F(1316)},
    {Op::kStore, F(1340), C(0x00000026)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x0000001D)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1312), F(1316)},
    {Op::kAdd, F(80), F(1312)},
    {Op::kStore, F(2084), C(0x820069BC)},
    {Op::kStore, F(2080), F(2084)},
    {Op::kCopy6, F(2084), F(1380)},
    {Op::kStore, F(1404), C(0x00000025)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x00000031)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1376), F(1380)},
    {Op::kAdd, F(80), F(1376)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(356), C(0x82010978)},
    {Op::kStore, F(360), M(0)},
    {Op::kStore, F(80), C(0x82106540)},
    {Op::kCopy, F(84), F(368)},
    {Op::kCopy, F(80), F(364)},
    {Op::kStore, F(352), F(356)},
    {Op::kCopy6, F(356), F(1444)},
    {Op::kStore, F(1468), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1440), F(1444)},
    {Op::kAdd, F(80), F(1440)},
    {Op::kStore, F(1860), C(0x820069BC)},
    {Op::kStore, F(1856), F(1860)},
    {Op::kCopy6, F(1860), F(1508)},
    {Op::kStore, F(1532), C(0x00000020)},
    {Op::kStore, F(80), C(0x00000018)},
    {Op::kStore, F(84), C(0x0000002C)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1504), F(1508)},
    {Op::kAdd, F(80), F(1504)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(420), C(0x82010978)},
    {Op::kStore, F(424), M(0)},
    {Op::kStore, F(80), C(0x821594B8)},
    {Op::kCopy, F(84), F(432)},
    {Op::kCopy, F(80), F(428)},
    {Op::kStore, F(416), F(420)},
    {Op::kCopy6, F(420), F(1572)},
    {Op::kStore, F(1596), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000015)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1568), F(1572)},
    {Op::kAdd, F(80), F(1568)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(484), C(0x82010978)},
    {Op::kStore, F(488), M(0)},
    {Op::kStore, F(80), C(0x821597F0)},
    {Op::kCopy, F(84), F(496)},
    {Op::kCopy, F(80), F(492)},
    {Op::kStore, F(480), F(484)},
    {Op::kCopy6, F(484), F(1636)},
    {Op::kStore, F(1660), C(0x00000026)},
    {Op::kStore, F(80), C(0x00000015)},
    {Op::kStore, F(84), C(0x0000001A)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1632), F(1636)},
    {Op::kAdd, F(80), F(1632)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(548), C(0x82010978)},
    {Op::kStore, F(552), M(0)},
    {Op::kStore, F(80), C(0x821597F0)},
    {Op::kCopy, F(84), F(560)},
    {Op::kCopy, F(80), F(556)},
    {Op::kStore, F(544), F(548)},
    {Op::kCopy6, F(548), F(1700)},
    {Op::kStore, F(1724), C(0x00000026)},
    {Op::kStore, F(80), C(0x00000015)},
    {Op::kStore, F(84), C(0x00000013)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1696), F(1700)},
    {Op::kAdd, F(80), F(1696)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(612), C(0x82010978)},
    {Op::kStore, F(616), M(0)},
    {Op::kStore, F(80), C(0x82153150)},
    {Op::kCopy, F(84), F(624)},
    {Op::kCopy, F(80), F(620)},
    {Op::kStore, F(608), F(612)},
    {Op::kCopy6, F(612), F(1764)},
    {Op::kStore, F(1788), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000020)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1760), F(1764)},
    {Op::kAdd, F(80), F(1760)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(676), C(0x82010978)},
    {Op::kStore, F(680), M(0)},
    {Op::kStore, F(80), C(0x82106540)},
    {Op::kCopy, F(84), F(688)},
    {Op::kCopy, F(80), F(684)},
    {Op::kStore, F(672), F(676)},
    {Op::kCopy6, F(676), F(1828)},
    {Op::kStore, F(1852), C(0x00000000)},
    {Op::kStore, F(80), C(0x00000025)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(1824), F(1828)},
    {Op::kAdd, F(80), F(1824)},
    {Op::kStore, F(84), C(0x00000000)},
    {Op::kStore, F(740), C(0x82010978)},
    {Op::kStore, F(744), M(0)},
    {Op::kStore, F(80), C(0x82153098)},
    {Op::kCopy, F(84), F(752)},
    {Op::kCopy, F(80), F(748)},
    {Op::kStore, F(736), F(740)},
    {Op::kCopy6, F(740), F(964)},
    {Op::kStore, F(80), C(0x00000025)},
    {Op::kStore, F(960), F(964)},
    {Op::kStore, F(84), C(0x00000032)},
    {Op::kStore, F(88), C(0x00000000)},
    {Op::kStore, F(92), C(0x00000000)},
    {Op::kStore, F(988), C(0x00000023)},
    {Op::kAdd, F(80), F(960)},
    {Op::kStore, M(9628), C(0x00000000)},
    {Op::kStore, M(9632), C(0x00000000)},
    {Op::kStore, M(9624), C(0x00000000)},
};

}  // namespace

uint32_t DirectorInitStates(uint8_t* base, uint32_t director, uint32_t frame, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82159950 CameraDirector::InitStates");
  auto at = [&](const Value& v) {
    switch (v.base) {
      case Value::kConst:
        return v.value;
      case Value::kDirector:
        return director + v.value;
      case Value::kFrame:
        break;
    }
    return frame + v.value;
  };
  uint32_t r3 = 0;
  for (const Step& step : kInitSteps) {
    switch (step.op) {
      case Op::kStore:
        guest::StoreU32(base, at(step.a), at(step.b));
        break;
      case Op::kCopy:
        guest::StoreU32(base, at(step.b), guest::LoadU32(base, at(step.a)));
        break;
      case Op::kCopy6:
        for (uint32_t i = 0; i < 6; i++) {
          guest::StoreU32(base, at(step.b) + 4 * i, guest::LoadU32(base, at(step.a) + 4 * i));
        }
        break;
      case Op::kAdd:
        r3 = game_modes::StateMachineAdd(base, director + kMachine, at(step.a), at(step.b),
                                         replaced);
        break;
    }
  }
  return r3;
}

uint32_t DirectorSetMode(uint8_t* base, uint32_t director, uint32_t view, uint32_t frame,
                         const SetModeCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821591F0 CameraDirector::SetMode");
  const uint32_t first_vehicle = track_zones::RacersVehicle(base, 0);
  guest::StoreU32(base, frame + 80, first_vehicle);
  const uint32_t second_vehicle = track_zones::RacersVehicle(base, 1);
  const uint32_t viewports = guest::LoadU32(base, kViewports);
  guest::StoreU32(base, frame + 84, second_vehicle);
  const uint32_t first = guest::LoadU32(base, viewports + 0);
  const uint32_t second = guest::LoadU32(base, viewports + 4);
  guest::StoreU32(base, director + kView, view);
  guest::StoreU32(base, first + 1860, view);
  guest::StoreU32(base, second + 1860, guest::LoadU32(base, director + kView));
  ResetRoll(base);
  // ResetRoll's r3: viewport 1 (its adapter's), what a view past 5 leaves.
  uint32_t r3 = guest::LoadU32(base, guest::LoadU32(base, kViewports) + 4);
  const uint32_t current = director + kCurrentCamera;
  const uint32_t current2 = director + kCurrentCamera2;
  const uint32_t mode = guest::LoadU32(base, director + kView);
  guest::StoreU32(base, current, 0);
  guest::StoreU32(base, current2, 0);
  // The followed player's vehicle from the two kept in the frame (a player
  // past 1 reads the frame's next words, as the original does).
  auto followed = [&] {
    return guest::LoadU32(base, frame + 80 + 4 * guest::LoadU32(base, director + kFollowed));
  };
  // The view's object made in place; 0 where its address is 0 (never in play).
  auto make = [&](uint32_t view_index, uint32_t offset, uint32_t viewport, uint32_t vehicle,
                  uint32_t back) -> uint32_t {
    const uint32_t object = director + offset;
    if (object == 0) {
      return 0;
    }
    return calls.construct(view_index, object, viewport, vehicle, back);
  };
  struct View {
    uint32_t offset;
    uint32_t back;
  };
  static constexpr View kViews[6] = {{12976, 0x821593C0}, {11856, 0x8215939C},
                                     {9648, 0x82159378},  {67760, 0x82159300},
                                     {13360, 0x821592D8}, {56896, 0x821592B4}};
  if (mode > 5) {
    return r3;
  }
  if (mode == 2 && guest::LoadU32(base, guest::LoadU32(base, kViewports) + 8) != 0) {
    // Split screen: player 0's and player 1's in-car views.
    r3 = make(2, 9648, first, first_vehicle, 0x82159328);
    guest::StoreU32(base, current, r3);
    r3 = make(2, 10752, second, second_vehicle, 0x8215934C);
    guest::StoreU32(base, current2, r3);
    return r3;
  }
  r3 = director + kViews[mode].offset == 0
           ? 0
           : make(mode, kViews[mode].offset, first, followed(), kViews[mode].back);
  guest::StoreU32(base, current, r3);
  return r3;
}

}  // namespace camera_modes
