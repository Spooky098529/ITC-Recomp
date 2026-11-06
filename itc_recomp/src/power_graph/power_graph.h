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

}  // namespace power_graph
