// RaceSounds::Init (sub_82177378, docs/functions/race-sounds.md) as plain
// C++: the race kind (+9652) from the race data, the scene models' seven
// sound slots reset with their two buffers allocated (XPhysicalAlloc) and
// cleared, CarSounds::Init, the state (+9604) = 1, then the race sounds'
// transitions added (StateMachine::Add), the kind deciding which. As
// RaceHudMachine::Init (src/race_hud_machine/race_hud_machine_init.cpp),
// the original builds each transition's functor and key in its own stack
// frame (3,280 bytes) and copies the functor's six storage words from a
// temporary there, three of which it never writes (five for the transitions
// without a handler, vtable 0x820069BC): the transitions keep whatever the
// stack held. So the stores and copies are replayed in the original's
// frame, in its order (the table below, decoded from the generated code by
// a script that followed its registers for each kind), and the transitions
// get the same words. The frame's back chain is written (as the race
// sounds' other functions write theirs); its saved registers aren't.

#include "race_sounds/race_sounds.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

#include <cstring>

namespace race_sounds {

namespace {

// A value: a constant, or an offset from the machine or the frame.
struct Value {
  enum Base : uint8_t { kConst, kMachine, kFrame } base;
  uint32_t value;
};
constexpr Value C(uint32_t value) { return {Value::kConst, value}; }
constexpr Value M(uint32_t offset) { return {Value::kMachine, offset}; }
constexpr Value F(uint32_t offset) { return {Value::kFrame, offset}; }

// The kinds a step runs for: 5, 1, 2, 4 and 6, the others.
constexpr uint8_t k5 = 1, k1 = 2, k246 = 4, kOther = 8, kAll = 15;

uint8_t KindClass(uint32_t kind) {
  switch (int32_t(kind)) {
    case 5:
      return k5;
    case 1:
      return k1;
    case 2:
    case 4:
    case 6:
      return k246;
    default:
      return kOther;
  }
}

// kStore: [a] = b; kCopy6: six words from a to b, word by word; kAdd:
// StateMachine::Add(machine, key a, functor part b).
enum class Op : uint8_t { kStore, kCopy6, kAdd };
struct Step {
  Op op;
  Value a;
  Value b;
  uint8_t kinds;
};

// The transitions, in order: (state, message) -> next state (0: stay), the
// handler (functor vtable 0x82011918 with the machine and the handler; none:
// 0x820069BC), the kinds that add it. The original tests the kind three
// times: 5 gives the first transition its other next state (its own
// frame words), 1 adds the race's (23) transitions and the stage's (17)
// with their own frame words, 2, 4 and 6 the race end's from 38 and the
// stage's with theirs.
constexpr Step kSteps[] = {
    // (1, 4) -> 24 0x82176618, k5
    {Op::kStore, M(9604), C(0x00000001), k5},
    {Op::kStore, F(172), C(0x82176618), k5},
    {Op::kStore, F(164), C(0x82011918), k5},
    {Op::kStore, F(168), M(0), k5},
    {Op::kStore, F(160), F(164), k5},
    {Op::kCopy6, F(164), F(2660), k5},
    {Op::kStore, F(2684), C(0x00000018), k5},
    {Op::kStore, F(2656), F(2660), k5},
    {Op::kStore, F(80), C(0x00000001), k5},
    {Op::kStore, F(84), C(0x00000004), k5},
    {Op::kStore, F(88), C(0x00000000), k5},
    {Op::kStore, F(92), C(0x00000000), k5},
    {Op::kAdd, F(80), F(2656), k5},
    // (1, 4) -> 38 0x82176618, k1 | k246 | kOther
    {Op::kStore, M(9604), C(0x00000001), k1 | k246 | kOther},
    {Op::kStore, F(812), C(0x82176618), k1 | k246 | kOther},
    {Op::kStore, F(804), C(0x82011918), k1 | k246 | kOther},
    {Op::kStore, F(808), M(0), k1 | k246 | kOther},
    {Op::kStore, F(800), F(804), k1 | k246 | kOther},
    {Op::kCopy6, F(804), F(1508), k1 | k246 | kOther},
    {Op::kStore, F(1532), C(0x00000026), k1 | k246 | kOther},
    {Op::kStore, F(1504), F(1508), k1 | k246 | kOther},
    {Op::kStore, F(80), C(0x00000001), k1 | k246 | kOther},
    {Op::kStore, F(84), C(0x00000004), k1 | k246 | kOther},
    {Op::kStore, F(88), C(0x00000000), k1 | k246 | kOther},
    {Op::kStore, F(92), C(0x00000000), k1 | k246 | kOther},
    {Op::kAdd, F(80), F(1504), k1 | k246 | kOther},
    // (38, 0) -> 0 0x821767A8, kAll
    {Op::kStore, F(1316), C(0x82011918), kAll},
    {Op::kStore, F(1320), M(0), kAll},
    {Op::kStore, F(1324), C(0x821767A8), kAll},
    {Op::kStore, F(1312), F(1316), kAll},
    {Op::kCopy6, F(1316), F(1636), kAll},
    {Op::kStore, F(1660), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x00000026), kAll},
    {Op::kStore, F(84), C(0x00000000), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1632), F(1636), kAll},
    {Op::kAdd, F(80), F(1632), kAll},
    // (38, 29) -> 24 0x82175AE8, kAll
    {Op::kStore, F(228), C(0x82011918), kAll},
    {Op::kStore, F(232), M(0), kAll},
    {Op::kStore, F(236), C(0x82175AE8), kAll},
    {Op::kStore, F(224), F(228), kAll},
    {Op::kCopy6, F(228), F(2340), kAll},
    {Op::kStore, F(2364), C(0x00000018), kAll},
    {Op::kStore, F(80), C(0x00000026), kAll},
    {Op::kStore, F(84), C(0x0000001D), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2336), F(2340), kAll},
    {Op::kAdd, F(80), F(2336), kAll},
    // (38, 31) -> 24 0x82175C00, kAll
    {Op::kStore, F(868), C(0x82011918), kAll},
    {Op::kStore, F(872), M(0), kAll},
    {Op::kStore, F(876), C(0x82175C00), kAll},
    {Op::kStore, F(864), F(868), kAll},
    {Op::kCopy6, F(868), F(1700), kAll},
    {Op::kStore, F(1724), C(0x00000018), kAll},
    {Op::kStore, F(80), C(0x00000026), kAll},
    {Op::kStore, F(84), C(0x0000001F), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1696), F(1700), kAll},
    {Op::kAdd, F(80), F(1696), kAll},
    // (38, 49) -> 37 none(0x820069BC), kAll
    {Op::kStore, F(3140), C(0x820069BC), kAll},
    {Op::kStore, F(3136), F(3140), kAll},
    {Op::kCopy6, F(3140), F(2852), kAll},
    {Op::kStore, F(80), C(0x00000026), kAll},
    {Op::kStore, F(84), C(0x00000031), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2848), F(2852), kAll},
    {Op::kStore, F(2876), C(0x00000025), kAll},
    {Op::kAdd, F(80), F(2848), kAll},
    // (38, 44) -> 32 0x82175CA0, kAll
    {Op::kStore, F(292), C(0x82011918), kAll},
    {Op::kStore, F(296), M(0), kAll},
    {Op::kStore, F(300), C(0x82175CA0), kAll},
    {Op::kStore, F(288), F(292), kAll},
    {Op::kCopy6, F(292), F(1764), kAll},
    {Op::kStore, F(80), C(0x00000026), kAll},
    {Op::kStore, F(84), C(0x0000002C), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1760), F(1764), kAll},
    {Op::kStore, F(1788), C(0x00000020), kAll},
    {Op::kAdd, F(80), F(1760), kAll},
    // (38, 14) -> 10 0x821770C0, k1
    {Op::kStore, F(1188), C(0x82011918), k1},
    {Op::kStore, F(1192), M(0), k1},
    {Op::kStore, F(1196), C(0x821770C0), k1},
    {Op::kStore, F(1184), F(1188), k1},
    {Op::kCopy6, F(1188), F(2404), k1},
    {Op::kStore, F(2428), C(0x0000000A), k1},
    {Op::kStore, F(80), C(0x00000026), k1},
    {Op::kStore, F(84), C(0x0000000E), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(2400), F(2404), k1},
    {Op::kAdd, F(80), F(2400), k1},
    // (38, 26) -> 23 0x82176968, k1
    {Op::kStore, F(356), C(0x82011918), k1},
    {Op::kStore, F(360), M(0), k1},
    {Op::kStore, F(364), C(0x82176968), k1},
    {Op::kStore, F(352), F(356), k1},
    {Op::kCopy6, F(356), F(1828), k1},
    {Op::kStore, F(1852), C(0x00000017), k1},
    {Op::kStore, F(80), C(0x00000026), k1},
    {Op::kStore, F(84), C(0x0000001A), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(1824), F(1828), k1},
    {Op::kAdd, F(80), F(1824), k1},
    // (23, 0) -> 0 0x821769D0, k1
    {Op::kStore, F(932), C(0x82011918), k1},
    {Op::kStore, F(936), M(0), k1},
    {Op::kStore, F(940), C(0x821769D0), k1},
    {Op::kStore, F(928), F(932), k1},
    {Op::kCopy6, F(932), F(2724), k1},
    {Op::kStore, F(2748), C(0x00000000), k1},
    {Op::kStore, F(80), C(0x00000017), k1},
    {Op::kStore, F(84), C(0x00000000), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(2720), F(2724), k1},
    {Op::kAdd, F(80), F(2720), k1},
    // (23, 34) -> 30 0x82176BA0, k1
    {Op::kStore, F(420), C(0x82011918), k1},
    {Op::kStore, F(424), M(0), k1},
    {Op::kStore, F(428), C(0x82176BA0), k1},
    {Op::kStore, F(416), F(420), k1},
    {Op::kCopy6, F(420), F(1892), k1},
    {Op::kStore, F(1916), C(0x0000001E), k1},
    {Op::kStore, F(80), C(0x00000017), k1},
    {Op::kStore, F(84), C(0x00000022), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(1888), F(1892), k1},
    {Op::kAdd, F(80), F(1888), k1},
    // (23, 36) -> 30 0x82176BA0, k1
    {Op::kStore, F(1444), C(0x82011918), k1},
    {Op::kStore, F(1448), M(0), k1},
    {Op::kStore, F(1452), C(0x82176BA0), k1},
    {Op::kStore, F(1440), F(1444), k1},
    {Op::kCopy6, F(1444), F(2468), k1},
    {Op::kStore, F(2492), C(0x0000001E), k1},
    {Op::kStore, F(80), C(0x00000017), k1},
    {Op::kStore, F(84), C(0x00000024), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(2464), F(2468), k1},
    {Op::kAdd, F(80), F(2464), k1},
    // (23, 19) -> 38 0x82176BA0, k1
    {Op::kStore, F(484), C(0x82011918), k1},
    {Op::kStore, F(488), M(0), k1},
    {Op::kStore, F(492), C(0x82176BA0), k1},
    {Op::kStore, F(480), F(484), k1},
    {Op::kCopy6, F(484), F(1956), k1},
    {Op::kStore, F(1980), C(0x00000026), k1},
    {Op::kStore, F(80), C(0x00000017), k1},
    {Op::kStore, F(84), C(0x00000013), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(1952), F(1956), k1},
    {Op::kAdd, F(80), F(1952), k1},
    // (23, 29) -> 25 0x82175AE8, k1
    {Op::kStore, F(996), C(0x82011918), k1},
    {Op::kStore, F(1000), M(0), k1},
    {Op::kStore, F(1004), C(0x82175AE8), k1},
    {Op::kStore, F(992), F(996), k1},
    {Op::kCopy6, F(996), F(2980), k1},
    {Op::kStore, F(3004), C(0x00000019), k1},
    {Op::kStore, F(80), C(0x00000017), k1},
    {Op::kStore, F(84), C(0x0000001D), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(2976), F(2980), k1},
    {Op::kAdd, F(80), F(2976), k1},
    // (23, 31) -> 25 0x82175C00, k1
    {Op::kStore, F(548), C(0x82011918), k1},
    {Op::kStore, F(552), M(0), k1},
    {Op::kStore, F(556), C(0x82175C00), k1},
    {Op::kStore, F(544), F(548), k1},
    {Op::kCopy6, F(548), F(2020), k1},
    {Op::kStore, F(2044), C(0x00000019), k1},
    {Op::kStore, F(80), C(0x00000017), k1},
    {Op::kStore, F(84), C(0x0000001F), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(2016), F(2020), k1},
    {Op::kAdd, F(80), F(2016), k1},
    // (23, 58) -> 25 0x82175C00, k1
    {Op::kStore, F(1252), C(0x82011918), k1},
    {Op::kStore, F(1256), M(0), k1},
    {Op::kStore, F(1260), C(0x82175C00), k1},
    {Op::kStore, F(1248), F(1252), k1},
    {Op::kCopy6, F(1252), F(2532), k1},
    {Op::kStore, F(2556), C(0x00000019), k1},
    {Op::kStore, F(80), C(0x00000017), k1},
    {Op::kStore, F(84), C(0x0000003A), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(2528), F(2532), k1},
    {Op::kAdd, F(80), F(2528), k1},
    // (38, 36) -> 30 0x82176BA0, k246
    {Op::kStore, F(612), C(0x82011918), k246},
    {Op::kStore, F(616), M(0), k246},
    {Op::kStore, F(620), C(0x82176BA0), k246},
    {Op::kStore, F(608), F(612), k246},
    {Op::kCopy6, F(612), F(2084), k246},
    {Op::kStore, F(2108), C(0x0000001E), k246},
    {Op::kStore, F(80), C(0x00000026), k246},
    {Op::kStore, F(84), C(0x00000024), k246},
    {Op::kStore, F(88), C(0x00000000), k246},
    {Op::kStore, F(92), C(0x00000000), k246},
    {Op::kStore, F(2080), F(2084), k246},
    {Op::kAdd, F(80), F(2080), k246},
    // (38, 34) -> 30 0x82176BA0, k246
    {Op::kStore, F(1060), C(0x82011918), k246},
    {Op::kStore, F(1064), M(0), k246},
    {Op::kStore, F(1068), C(0x82176BA0), k246},
    {Op::kStore, F(1056), F(1060), k246},
    {Op::kCopy6, F(1060), F(2788), k246},
    {Op::kStore, F(2812), C(0x0000001E), k246},
    {Op::kStore, F(80), C(0x00000026), k246},
    {Op::kStore, F(84), C(0x00000022), k246},
    {Op::kStore, F(88), C(0x00000000), k246},
    {Op::kStore, F(92), C(0x00000000), k246},
    {Op::kStore, F(2784), F(2788), k246},
    {Op::kAdd, F(80), F(2784), k246},
    // (30, 0) -> 0 0x82176C10, kAll
    {Op::kStore, F(676), C(0x82011918), kAll},
    {Op::kStore, F(680), M(0), kAll},
    {Op::kStore, F(684), C(0x82176C10), kAll},
    {Op::kStore, F(672), F(676), kAll},
    {Op::kCopy6, F(676), F(2148), kAll},
    {Op::kStore, F(2172), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x0000001E), kAll},
    {Op::kStore, F(84), C(0x00000000), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2144), F(2148), kAll},
    {Op::kAdd, F(80), F(2144), kAll},
    // (30, 40) -> 38 0x82176E60, kAll
    {Op::kStore, F(1380), C(0x82011918), kAll},
    {Op::kStore, F(1384), M(0), kAll},
    {Op::kStore, F(1388), C(0x82176E60), kAll},
    {Op::kStore, F(1376), F(1380), kAll},
    {Op::kCopy6, F(1380), F(2596), kAll},
    {Op::kStore, F(2620), C(0x00000026), kAll},
    {Op::kStore, F(80), C(0x0000001E), kAll},
    {Op::kStore, F(84), C(0x00000028), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2592), F(2596), kAll},
    {Op::kAdd, F(80), F(2592), kAll},
    // (30, 49) -> 37 none(0x820069BC), kAll
    {Op::kStore, F(3108), C(0x820069BC), kAll},
    {Op::kStore, F(3104), F(3108), kAll},
    {Op::kCopy6, F(3108), F(2212), kAll},
    {Op::kStore, F(2236), C(0x00000025), kAll},
    {Op::kStore, F(80), C(0x0000001E), kAll},
    {Op::kStore, F(84), C(0x00000031), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2208), F(2212), kAll},
    {Op::kAdd, F(80), F(2208), kAll},
    // (30, 44) -> 32 0x82175CA0, kAll
    {Op::kStore, F(740), C(0x82011918), kAll},
    {Op::kStore, F(744), M(0), kAll},
    {Op::kStore, F(748), C(0x82175CA0), kAll},
    {Op::kStore, F(736), F(740), kAll},
    {Op::kCopy6, F(740), F(2916), kAll},
    {Op::kStore, F(2940), C(0x00000020), kAll},
    {Op::kStore, F(80), C(0x0000001E), kAll},
    {Op::kStore, F(84), C(0x0000002C), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2912), F(2916), kAll},
    {Op::kAdd, F(80), F(2912), kAll},
    // (30, 42) -> 17 0x82176EC8, kAll
    {Op::kStore, F(1124), C(0x82011918), kAll},
    {Op::kStore, F(1128), M(0), kAll},
    {Op::kStore, F(1132), C(0x82176EC8), kAll},
    {Op::kStore, F(1120), F(1124), kAll},
    {Op::kCopy6, F(1124), F(2276), kAll},
    {Op::kStore, F(80), C(0x0000001E), kAll},
    {Op::kStore, F(84), C(0x0000002A), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2272), F(2276), kAll},
    {Op::kStore, F(2300), C(0x00000011), kAll},
    {Op::kAdd, F(80), F(2272), kAll},
    // (30, 31) -> 0 0x82175C00, kAll
    {Op::kStore, F(100), C(0x82011918), kAll},
    {Op::kStore, F(104), M(0), kAll},
    {Op::kStore, F(108), C(0x82175C00), kAll},
    {Op::kStore, F(96), F(100), kAll},
    {Op::kCopy6, F(100), F(1540), kAll},
    {Op::kStore, F(1564), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x0000001E), kAll},
    {Op::kStore, F(84), C(0x0000001F), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1536), F(1540), kAll},
    {Op::kAdd, F(80), F(1536), kAll},
    // (37, 0) -> 0 0x82175A40, kAll
    {Op::kStore, F(132), C(0x82011918), kAll},
    {Op::kStore, F(136), M(0), kAll},
    {Op::kStore, F(140), C(0x82175A40), kAll},
    {Op::kStore, F(128), F(132), kAll},
    {Op::kCopy6, F(132), F(1604), kAll},
    {Op::kStore, F(1628), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x00000025), kAll},
    {Op::kStore, F(84), C(0x00000000), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1600), F(1604), kAll},
    {Op::kAdd, F(80), F(1600), kAll},
    // (37, 50) -> 35 0x821772D8, kAll
    {Op::kStore, F(196), C(0x82011918), kAll},
    {Op::kStore, F(200), M(0), kAll},
    {Op::kStore, F(204), C(0x821772D8), kAll},
    {Op::kStore, F(192), F(196), kAll},
    {Op::kCopy6, F(196), F(1668), kAll},
    {Op::kStore, F(80), C(0x00000025), kAll},
    {Op::kStore, F(1664), F(1668), kAll},
    {Op::kStore, F(84), C(0x00000032), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1692), C(0x00000023), kAll},
    {Op::kAdd, F(80), F(1664), kAll},
    // (24, 0) -> 0 0x82106540, kAll
    {Op::kStore, F(260), C(0x82011918), kAll},
    {Op::kStore, F(264), M(0), kAll},
    {Op::kStore, F(268), C(0x82106540), kAll},
    {Op::kStore, F(256), F(260), kAll},
    {Op::kCopy6, F(260), F(1732), kAll},
    {Op::kStore, F(1756), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x00000018), kAll},
    {Op::kStore, F(84), C(0x00000000), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1728), F(1732), kAll},
    {Op::kAdd, F(80), F(1728), kAll},
    // (24, 29) -> 38 0x82175B80, kAll
    {Op::kStore, F(324), C(0x82011918), kAll},
    {Op::kStore, F(328), M(0), kAll},
    {Op::kStore, F(332), C(0x82175B80), kAll},
    {Op::kStore, F(320), F(324), kAll},
    {Op::kCopy6, F(324), F(1796), kAll},
    {Op::kStore, F(1820), C(0x00000026), kAll},
    {Op::kStore, F(80), C(0x00000018), kAll},
    {Op::kStore, F(84), C(0x0000001D), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1792), F(1796), kAll},
    {Op::kAdd, F(80), F(1792), kAll},
    // (24, 32) -> 38 0x82175C58, kAll
    {Op::kStore, F(388), C(0x82011918), kAll},
    {Op::kStore, F(392), M(0), kAll},
    {Op::kStore, F(396), C(0x82175C58), kAll},
    {Op::kStore, F(384), F(388), kAll},
    {Op::kCopy6, F(388), F(1860), kAll},
    {Op::kStore, F(1884), C(0x00000026), kAll},
    {Op::kStore, F(80), C(0x00000018), kAll},
    {Op::kStore, F(84), C(0x00000020), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1856), F(1860), kAll},
    {Op::kAdd, F(80), F(1856), kAll},
    // (24, 44) -> 32 0x82175CA0, kAll
    {Op::kStore, F(452), C(0x82011918), kAll},
    {Op::kStore, F(456), M(0), kAll},
    {Op::kStore, F(460), C(0x82175CA0), kAll},
    {Op::kStore, F(448), F(452), kAll},
    {Op::kCopy6, F(452), F(1924), kAll},
    {Op::kStore, F(1948), C(0x00000020), kAll},
    {Op::kStore, F(80), C(0x00000018), kAll},
    {Op::kStore, F(84), C(0x0000002C), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1920), F(1924), kAll},
    {Op::kAdd, F(80), F(1920), kAll},
    // (24, 49) -> 37 none(0x820069BC), kAll
    {Op::kStore, F(3076), C(0x820069BC), kAll},
    {Op::kStore, F(3072), F(3076), kAll},
    {Op::kCopy6, F(3076), F(1988), kAll},
    {Op::kStore, F(2012), C(0x00000025), kAll},
    {Op::kStore, F(80), C(0x00000018), kAll},
    {Op::kStore, F(84), C(0x00000031), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1984), F(1988), kAll},
    {Op::kAdd, F(80), F(1984), kAll},
    // (24, 14) -> 10 0x821770C0, kAll
    {Op::kStore, F(516), C(0x82011918), kAll},
    {Op::kStore, F(520), M(0), kAll},
    {Op::kStore, F(524), C(0x821770C0), kAll},
    {Op::kStore, F(512), F(516), kAll},
    {Op::kCopy6, F(516), F(2052), kAll},
    {Op::kStore, F(2076), C(0x0000000A), kAll},
    {Op::kStore, F(80), C(0x00000018), kAll},
    {Op::kStore, F(84), C(0x0000000E), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2048), F(2052), kAll},
    {Op::kAdd, F(80), F(2048), kAll},
    // (25, 0) -> 0 0x82106540, kAll
    {Op::kStore, F(580), C(0x82011918), kAll},
    {Op::kStore, F(584), M(0), kAll},
    {Op::kStore, F(588), C(0x82106540), kAll},
    {Op::kStore, F(576), F(580), kAll},
    {Op::kCopy6, F(580), F(2116), kAll},
    {Op::kStore, F(2140), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x00000019), kAll},
    {Op::kStore, F(84), C(0x00000000), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2112), F(2116), kAll},
    {Op::kAdd, F(80), F(2112), kAll},
    // (25, 29) -> 23 0x82175B80, kAll
    {Op::kStore, F(644), C(0x82011918), kAll},
    {Op::kStore, F(648), M(0), kAll},
    {Op::kStore, F(652), C(0x82175B80), kAll},
    {Op::kStore, F(640), F(644), kAll},
    {Op::kCopy6, F(644), F(2180), kAll},
    {Op::kStore, F(2204), C(0x00000017), kAll},
    {Op::kStore, F(80), C(0x00000019), kAll},
    {Op::kStore, F(84), C(0x0000001D), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2176), F(2180), kAll},
    {Op::kAdd, F(80), F(2176), kAll},
    // (25, 32) -> 23 0x82175C58, kAll
    {Op::kStore, F(708), C(0x82011918), kAll},
    {Op::kStore, F(712), M(0), kAll},
    {Op::kStore, F(716), C(0x82175C58), kAll},
    {Op::kStore, F(704), F(708), kAll},
    {Op::kCopy6, F(708), F(2244), kAll},
    {Op::kStore, F(2268), C(0x00000017), kAll},
    {Op::kStore, F(80), C(0x00000019), kAll},
    {Op::kStore, F(84), C(0x00000020), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2240), F(2244), kAll},
    {Op::kAdd, F(80), F(2240), kAll},
    // (25, 58) -> 23 0x82175C58, kAll
    {Op::kStore, F(772), C(0x82011918), kAll},
    {Op::kStore, F(776), M(0), kAll},
    {Op::kStore, F(780), C(0x82175C58), kAll},
    {Op::kStore, F(768), F(772), kAll},
    {Op::kCopy6, F(772), F(2308), kAll},
    {Op::kStore, F(2332), C(0x00000017), kAll},
    {Op::kStore, F(80), C(0x00000019), kAll},
    {Op::kStore, F(84), C(0x0000003A), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2304), F(2308), kAll},
    {Op::kAdd, F(80), F(2304), kAll},
    // (25, 44) -> 32 0x82175CA0, kAll
    {Op::kStore, F(836), C(0x82011918), kAll},
    {Op::kStore, F(840), M(0), kAll},
    {Op::kStore, F(844), C(0x82175CA0), kAll},
    {Op::kStore, F(832), F(836), kAll},
    {Op::kCopy6, F(836), F(2372), kAll},
    {Op::kStore, F(2396), C(0x00000020), kAll},
    {Op::kStore, F(80), C(0x00000019), kAll},
    {Op::kStore, F(84), C(0x0000002C), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2368), F(2372), kAll},
    {Op::kAdd, F(80), F(2368), kAll},
    // (25, 49) -> 37 none(0x820069BC), kAll
    {Op::kStore, F(3044), C(0x820069BC), kAll},
    {Op::kStore, F(3040), F(3044), kAll},
    {Op::kCopy6, F(3044), F(2436), kAll},
    {Op::kStore, F(2460), C(0x00000025), kAll},
    {Op::kStore, F(80), C(0x00000019), kAll},
    {Op::kStore, F(84), C(0x00000031), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2432), F(2436), kAll},
    {Op::kAdd, F(80), F(2432), kAll},
    // (25, 14) -> 10 0x821770C0, kAll
    {Op::kStore, F(900), C(0x82011918), kAll},
    {Op::kStore, F(904), M(0), kAll},
    {Op::kStore, F(908), C(0x821770C0), kAll},
    {Op::kStore, F(896), F(900), kAll},
    {Op::kCopy6, F(900), F(2500), kAll},
    {Op::kStore, F(2524), C(0x0000000A), kAll},
    {Op::kStore, F(80), C(0x00000019), kAll},
    {Op::kStore, F(84), C(0x0000000E), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2496), F(2500), kAll},
    {Op::kAdd, F(80), F(2496), kAll},
    // (32, 4) -> 38 0x82175CF8, kAll
    {Op::kStore, F(964), C(0x82011918), kAll},
    {Op::kStore, F(968), M(0), kAll},
    {Op::kStore, F(972), C(0x82175CF8), kAll},
    {Op::kStore, F(960), F(964), kAll},
    {Op::kCopy6, F(964), F(2564), kAll},
    {Op::kStore, F(2588), C(0x00000026), kAll},
    {Op::kStore, F(80), C(0x00000020), kAll},
    {Op::kStore, F(84), C(0x00000004), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2560), F(2564), kAll},
    {Op::kAdd, F(80), F(2560), kAll},
    // (17, 0) -> 0 0x82176F28, k1
    {Op::kStore, F(1028), C(0x82011918), k1},
    {Op::kStore, F(1032), M(0), k1},
    {Op::kStore, F(1036), C(0x82176F28), k1},
    {Op::kStore, F(1024), F(1028), k1},
    {Op::kCopy6, F(1028), F(2628), k1},
    {Op::kStore, F(2652), C(0x00000000), k1},
    {Op::kStore, F(80), C(0x00000011), k1},
    {Op::kStore, F(84), C(0x00000000), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(2624), F(2628), k1},
    {Op::kAdd, F(80), F(2624), k1},
    // (17, 26) -> 23 0x82175D20, k1
    {Op::kStore, F(1092), C(0x82011918), k1},
    {Op::kStore, F(1096), M(0), k1},
    {Op::kStore, F(1100), C(0x82175D20), k1},
    {Op::kStore, F(1088), F(1092), k1},
    {Op::kCopy6, F(1092), F(2692), k1},
    {Op::kStore, F(2716), C(0x00000017), k1},
    {Op::kStore, F(80), C(0x00000011), k1},
    {Op::kStore, F(84), C(0x0000001A), k1},
    {Op::kStore, F(88), C(0x00000000), k1},
    {Op::kStore, F(92), C(0x00000000), k1},
    {Op::kStore, F(2688), F(2692), k1},
    {Op::kAdd, F(80), F(2688), k1},
    // (17, 0) -> 0 0x82176F28, k246
    {Op::kStore, F(1156), C(0x82011918), k246},
    {Op::kStore, F(1160), M(0), k246},
    {Op::kStore, F(1164), C(0x82176F28), k246},
    {Op::kStore, F(1152), F(1156), k246},
    {Op::kCopy6, F(1156), F(2756), k246},
    {Op::kStore, F(2780), C(0x00000000), k246},
    {Op::kStore, F(80), C(0x00000011), k246},
    {Op::kStore, F(84), C(0x00000000), k246},
    {Op::kStore, F(88), C(0x00000000), k246},
    {Op::kStore, F(92), C(0x00000000), k246},
    {Op::kStore, F(2752), F(2756), k246},
    {Op::kAdd, F(80), F(2752), k246},
    // (17, 26) -> 38 0x82175D20, k246
    {Op::kStore, F(1220), C(0x82011918), k246},
    {Op::kStore, F(1224), M(0), k246},
    {Op::kStore, F(1228), C(0x82175D20), k246},
    {Op::kStore, F(1216), F(1220), k246},
    {Op::kCopy6, F(1220), F(2820), k246},
    {Op::kStore, F(2844), C(0x00000026), k246},
    {Op::kStore, F(80), C(0x00000011), k246},
    {Op::kStore, F(84), C(0x0000001A), k246},
    {Op::kStore, F(88), C(0x00000000), k246},
    {Op::kStore, F(92), C(0x00000000), k246},
    {Op::kStore, F(2816), F(2820), k246},
    {Op::kAdd, F(80), F(2816), k246},
    // (10, 0) -> 0 0x82177130, kAll
    {Op::kStore, F(1284), C(0x82011918), kAll},
    {Op::kStore, F(1288), M(0), kAll},
    {Op::kStore, F(1292), C(0x82177130), kAll},
    {Op::kStore, F(1280), F(1284), kAll},
    {Op::kCopy6, F(1284), F(2884), kAll},
    {Op::kStore, F(2908), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x0000000A), kAll},
    {Op::kStore, F(84), C(0x00000000), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2880), F(2884), kAll},
    {Op::kAdd, F(80), F(2880), kAll},
    // (10, 10) -> 23 0x82175D48, kAll
    {Op::kStore, F(1348), C(0x82011918), kAll},
    {Op::kStore, F(1352), M(0), kAll},
    {Op::kStore, F(1356), C(0x82175D48), kAll},
    {Op::kStore, F(1344), F(1348), kAll},
    {Op::kCopy6, F(1348), F(2948), kAll},
    {Op::kStore, F(2972), C(0x00000017), kAll},
    {Op::kStore, F(80), C(0x0000000A), kAll},
    {Op::kStore, F(84), C(0x0000000A), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(2944), F(2948), kAll},
    {Op::kAdd, F(80), F(2944), kAll},
    // (10, 31) -> 0 0x82175C00, kAll
    {Op::kStore, F(1412), C(0x82011918), kAll},
    {Op::kStore, F(1416), M(0), kAll},
    {Op::kStore, F(1420), C(0x82175C00), kAll},
    {Op::kStore, F(1408), F(1412), kAll},
    {Op::kCopy6, F(1412), F(3012), kAll},
    {Op::kStore, F(3036), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x0000000A), kAll},
    {Op::kStore, F(84), C(0x0000001F), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(3008), F(3012), kAll},
    {Op::kAdd, F(80), F(3008), kAll},
    // (10, 32) -> 0 0x82175C58, kAll
    {Op::kStore, F(1476), C(0x82011918), kAll},
    {Op::kStore, F(1480), M(0), kAll},
    {Op::kStore, F(1484), C(0x82175C58), kAll},
    {Op::kStore, F(1472), F(1476), kAll},
    {Op::kCopy6, F(1476), F(1572), kAll},
    {Op::kStore, F(1596), C(0x00000000), kAll},
    {Op::kStore, F(80), C(0x0000000A), kAll},
    {Op::kStore, F(84), C(0x00000020), kAll},
    {Op::kStore, F(88), C(0x00000000), kAll},
    {Op::kStore, F(92), C(0x00000000), kAll},
    {Op::kStore, F(1568), F(1572), kAll},
    {Op::kAdd, F(80), F(1568), kAll},
};

// The scene models' sound slots' load callbacks' contexts (the slot's
// index) and the seven "loading" bytes.
constexpr uint32_t kSlotContext = 0x826D8334;
constexpr uint32_t kSlotLoading = 0x826D8350;
// The two buffers' sizes.
constexpr uint32_t kBuffer1Bytes = 0x6B080;
constexpr uint32_t kBuffer0Bytes = 0x5D2D0;

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// memset (sub_825A71D0) with 0, as its adapter does it.
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

}  // namespace

uint32_t Init(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls, bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82177378 RaceSounds::Init");
  const uint32_t frame = sp - kInitFrame;
  Store(base, frame, sp);
  Store(base, machine + kKind, guest::LoadU32(base, guest::LoadU32(base, kRaceData) + 0xA3AF0));
  uint64_t last = 0;
  for (uint32_t i = 0; i < kCarSoundCount; i++) {
    Store(base, kSlotArrived + 4 * i, 0);
    Store(base, kSlotAsked + 4 * i, 0);
    guest::StoreU8(base, kSlotLoading + i, 0);
    Store(base, kSlotContext + 4 * i, i);
    Store(base, kSlotBuffer1 + 4 * i, 0);
    Store(base, kSlotBuffer0 + 4 * i, 0);
    const uint64_t buffer1 = calls.Call(kXPhysicalAlloc, frame, 0x82177410, 4, kBuffer1Bytes,
                                        0xFFFFFFFFFFFFFFFFull, 2048, 4);
    Store(base, kSlotBuffer1 + 4 * i, uint32_t(buffer1));
    last = calls.Call(kXPhysicalAlloc, frame, 0x82177430, 4, kBuffer0Bytes,
                      0xFFFFFFFFFFFFFFFFull, 2048, 4);
    Store(base, kSlotBuffer0 + 4 * i, uint32_t(last));
    Clear(base, uint32_t(last), kBuffer0Bytes);
    last = guest::LoadU32(base, kSlotBuffer1 + 4 * i);
    Clear(base, uint32_t(last), kBuffer1Bytes);
  }
  // CarSounds::Init with memset's r3 to r5 left.
  calls.Call(kCarSoundsInit, frame, 0x8217746C, 3, last, 0, kBuffer1Bytes);
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
    if (!(step.kinds & KindClass(guest::LoadU32(base, machine + kKind)))) {
      continue;
    }
    switch (step.op) {
      case Op::kStore:
        Store(base, at(step.a), at(step.b));
        break;
      case Op::kCopy6:
        for (uint32_t i = 0; i < 6; i++) {
          Store(base, at(step.b) + 4 * i, guest::LoadU32(base, at(step.a) + 4 * i));
        }
        break;
      case Op::kAdd:
        r3 = game_modes::StateMachineAdd(base, machine, at(step.a), at(step.b), replaced);
        break;
    }
  }
  return r3;
}

}  // namespace race_sounds
