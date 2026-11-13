// The front end's power and torque graph as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/power-graph.md): two sets of 32 stack sprites, one line
// segment each, from a car data's torque curve, recorded as two-vertex
// lines on the scene's command list. They work on guest memory with the
// originals' loads and stores (the sprites live in the original's frame,
// which the adapter makes). Their guest calls (the render manager's
// getters, the render queue's Add, the original Line::RecordFlat where the
// native one declines) are the adapter's (src/hooks/power_graph.cpp).

#pragma once

#include <cstdint>
#include <functional>
#include <utility>

namespace front_end_car_draw {
struct Calls;
}

namespace power_graph {

// The line recorder's render queue (+0) and command list (+4), stored by
// every Line::RecordFlat.
constexpr uint32_t kLineLists = 0x826D83D8;
// The camera pass the lines are recorded with (pass 0).
constexpr uint32_t kPass = 0x826BAC00;
// The sort key the lines are queued with.
constexpr uint32_t kKey = 683;

// PowerGraph::Draw's frame (its r1 less this, below its caller's) and
// Line::RecordFlat's (below PowerGraph::Draw's).
constexpr uint32_t kDrawFrame = 9648;
constexpr uint32_t kLineFrame = 128;
// The two sets of sprites in PowerGraph::Draw's frame (144 bytes each).
constexpr uint32_t kTorqueSprites = 4976;
constexpr uint32_t kPowerSprites = 368;
constexpr uint32_t kSpriteBytes = 144;
// What PowerGraph::Draw leaves in r3: the last array destructor
// iterator's first element (the torque set's sprite 0 + 80).
constexpr uint32_t kDrawR3 = kTorqueSprites + 80;
// Return addresses in PowerGraph::Draw of its two Line::RecordFlat calls
// (torque, power).
constexpr uint32_t kTorqueLineReturn = 0x8217C9FC;
constexpr uint32_t kPowerLineReturn = 0x8217CA14;

// Line::RecordFlat's guest calls, made by the adapter.
struct LineCalls {
  // From Line::RecordFlat's frame `frame`: [0x826BABA0]'s slot 8 and its
  // answer's slot 1 (the command list), then slot 8 again and its answer's
  // slot 2 (the render queue); returns {the list, the queue}.
  std::function<std::pair<uint32_t, uint32_t>(uint32_t frame)> list_and_queue;
  // From `frame`: the queue's slot 4 (RenderQueue::Add) with r4 = `chunk`,
  // r5 = `key`, f1 = 0.0 (0x82000A34).
  std::function<void(uint32_t frame, uint32_t queue, uint32_t chunk, uint32_t key)> add;
  // From PowerGraph::Draw's frame: the original Line::RecordFlat (r4 =
  // `sprite`, r5 = the pass, r6 = the key, r3 as Draw's), returning to `lr`.
  std::function<void(uint32_t sprite, uint32_t lr)> original;
};

// sub_8217B508 Line::RecordFlat: r4 = `sprite` (+0 its world matrix, +80
// and +96 the ends' x, y, z, +112 and +128 their colors as words), r5 =
// `pass`, r6 = `key`, its frame at `frame`. The list and queue from the
// getters, stored at kLineLists + 4 and kLineLists; then on the list a
// chunk: OpenChunk, a header slot, Primitive::RecordTransform(pass, sprite)
// with its product at `frame` - 80 (on [0x826D83D4]'s list), [13, 2, 1, 0],
// the two vertices (AllocateVertices(2), with its header: x, y, z and the
// color as ARGB from the color words' low bytes; +16 to +23 not written), a
// slot with +0 = 15, CloseChunk, the header's +0, +2, +4, +12 = 0 and +8 =
// ChunkSlots - 1; the queue's Add. False after the getters and their
// stores, with nothing else done, where the original's command list calls
// would print a message (a chunk open on any list, no room for the 12
// slots) or Primitive::RecordTransform's list isn't this one: the original
// then runs (its getters and stores the same again).
bool LineRecordFlat(uint8_t* base, uint32_t sprite, uint32_t pass, uint32_t key, uint32_t frame,
                    const LineCalls& calls);

// The segment count PowerGraph::Draw records: fctiwz of car data +1280 ×
// 0.0025 + 1.0 (one fused multiply-add, rounded to single).
int32_t SegmentCount(uint8_t* base, uint32_t car);
// Whether PowerGraph::Draw can run natively: its count is 32 or less (past
// 32 the original's lines read its saved registers and its caller's frame).
bool DrawReady(uint8_t* base, uint32_t car);

// sub_8217C708 PowerGraph::Draw: f1 = `x` (the graph's left), f2 = `y`
// (its baseline), r6 = `car` (a car data), its frame at `frame`. The two
// sets of sprites reset (Sprite::ResetStyle2), the torque set at +4976
// first, then the power set at +368; sprites 0 to 30 of both: +80 x = x +
// i × 6.4, +96 x = x + (i + 1) × 6.4, +84, +100 = y, +88, +104 = 0.0; the
// 32 points into the frame's +112 (torque in kgf·m) and +240 (power in PS,
// 0.0 below 0.0); points 0 to 25: the heights (y - torque × 3.2, y - power
// × 0.32) at segment i's +100 and segment i + 1's +84, and the colors
// (orange, cyan); the count n, and when n > 1 segments 1 to n - 1 recorded
// (torque, then power) through Line::RecordFlat (the original through
// `calls` where it declines); the sprites' empty destructors (the power
// set, then the torque set, 31 to 0). The frame's +80 to +103 left as the
// original's conversions leave them. Only after DrawReady.
void Draw(uint8_t* base, uint32_t frame, double x, double y, uint32_t car,
          const LineCalls& calls);

// TuningScreen::DrawPowerGraph's frame (its r1 less this, below its
// caller's).
constexpr uint32_t kTuningFrame = 112;
// Where TuningScreen::DrawPowerGraph's PowerGraph::Draw returns.
constexpr uint32_t kTuningDrawReturn = 0x821DF430;
// TuningScreen::DrawPowerGraph's PowerGraph::Draw: r3 = the front end's
// car holder's first word ([[0x826D83E0]]), f1 = 1042.0 (0x8200AD98), f2 =
// 520.0 (0x8200AD9C), r6 = the screen + 18912 (its car data).
struct TuningDraw {
  uint32_t r3;
  double x, y;
  uint32_t car;
};
// sub_821DF350 TuningScreen::DrawPowerGraph's test, its frame at `frame`:
// movie 2 found in the movie table (UiMovies::Find, the key 2 at `frame` +
// 80); with the screen's state (+9652) 5, UiMovie::AnimationTime(movie, -1,
// 5) above 22.0, or (asked again) equal to the no-time float (0x82001BE0),
// and +9656 0 and the byte +9660 2 or less; with state 6, +9656 0 and the
// byte +9660 2. Whether to draw, `r3` the original's r3 when it doesn't
// (Find's answer, or the movie after AnimationTime), and `movie` (the
// original's r30 at the draw: PowerGraph::Draw saves it in its frame, where
// a segment count above 32 reads it back).
bool TuningScreenShowsGraph(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t* r3,
                            uint32_t* movie);
// The draw's arguments for `screen`.
TuningDraw TuningScreenDraw(uint8_t* base, uint32_t screen);

// CarList::DrawEntryPowerGraph's frame (its r1 less this, below its
// caller's), where its car object (+1936) and car data (+80) live.
constexpr uint32_t kEntryFrame = 3920;
constexpr uint32_t kEntryCar = 1936;
constexpr uint32_t kEntryCarData = 80;
// Where its PowerGraph::Draw returns, and the r31 it holds there (lis r31,
// 0x826E: PowerGraph::Draw saves it in its frame).
constexpr uint32_t kEntryDrawReturn = 0x822506E8;
constexpr uint32_t kEntryR31 = 0x826E0000;
// The word that says whether the car list's graph is shown.
constexpr uint32_t kEntryShown = 0x82815EC0;
// sub_82250670 CarList::DrawEntryPowerGraph's set-up, r3 = `entry` (an entry
// of the car list screen's list), its frame at `frame`: false (nothing done)
// when [kEntryShown] is 0; else the car object at frame + 1936
// (GarageCar::Reset, then Car::SetModelAndClass with the entry's +8432) and
// the car data at frame + 80 (CarData::CarData, then CarData::FromGarageCar
// from the car), each callee's frame below `frame` as the original's (the
// car makers' callees through `calls` where their plain C++ declines);
// true: PowerGraph::Draw is then called with CarListEntryDraw's arguments.
bool CarListEntrySetUp(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t entry,
                       uint32_t frame);
// The draw's arguments: r3 = the front end's car holder's first word
// ([[0x826D83E0]]), f1 = 1042.0 (0x8200AD98), f2 = 500.0 (0x82001FC0), r6 =
// the car data at `frame` + 80.
TuningDraw CarListEntryDraw(uint8_t* base, uint32_t frame);

// CarList::DrawCarPowerGraph's frame (its r1 less this, below its caller's),
// its car data at +80 (as the entry's: CarListEntryDraw gives the draw's
// arguments), and where its PowerGraph::Draw returns (r31 kEntryR31 there
// too).
constexpr uint32_t kCarFrame = 1952;
constexpr uint32_t kCarDrawReturn = 0x82250764;
// sub_82250700 CarList::DrawCarPowerGraph's set-up, r3 = `car` (a garage
// car), its frame at `frame`: false (nothing done) when [kEntryShown] is 0;
// else the car data at frame + 80 (CarData::CarData, then
// CarData::FromGarageCar from the car), each callee's frame below `frame`
// as the original's (FromGarageCar's callees through `calls` where their
// plain C++ declines); true: PowerGraph::Draw is then called with
// CarListEntryDraw's arguments.
bool CarListCarSetUp(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t car,
                     uint32_t frame);

// A guest call PowerGraph::SetFromCar makes where a callee's plain C++
// declines (memcpy, Car::Refresh, Vehicle::SetupFromCar): r3 to r5, from
// the frame `frame`, returning to `lr`.
using SetFromCarCall = std::function<void(uint32_t function, uint64_t r3, uint64_t r4,
                                          uint64_t r5, uint32_t frame, uint32_t lr)>;
// SetFromCar's frame (below its caller's r1) and its record's global.
constexpr uint32_t kSetFromCarFrame = 6960;
constexpr uint32_t kPowerGraphScreen = 0x82815EBC;  // [it] + 8: the record
// sub_82250488 PowerGraph::SetFromCar (r3 `car`, a garage car; r4 `tab`):
// the power graph screen's record ([0x82815EBC] + 8) filled from the car,
// as PowerGraph::SetEntry fills it from a car list entry. In its frame
// (`sp` - 6960, its back chain written): a car object at +3008 (Car::Init,
// the car's 1,884 bytes copied, Car::Refresh; its spec s = +3008 + 1888), a
// car data at +1152 (CarData::CarData, Vehicle::SetupFromCar) and a power
// curve at +80 (PowerCurve::FromEngine on the car data's +1268, ShiftPeak
// 0), each callee's frame below as the original's. The record: +80 = tab;
// +60 to +76 the car's +1568 to +1584; +0 = fctiwz(car +1892); +4 =
// fctiwz(data +0 + (data +400 + data +672) x [0x820021E8]); +8, +12, +16 = s
// +76, +84, +80; +20 = the car's id; +24, +28 = s bytes +129, +130; +32 by s
// byte +68 (0 and 4: 6 (its split-off case sub_822505CC), 1 to 3: 2 to 4,
// 5: 0, 6: 1, 7: 5; above 7 not written); +36 = s byte +69 set; +40 = s +8;
// +44, +48 = fctiwz(curve +1056, +1060); +52 = curve +1048 x [0x82009EC0];
// +56 = fctiwz(curve +1052). Returns the curve's address (+80: r3 as
// ShiftPeak leaves it).
uint32_t SetFromCar(uint8_t* base, uint32_t car, uint32_t tab, uint32_t sp,
                    const SetFromCarCall& call);

}  // namespace power_graph
