// The car scene's draw of one scene model as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/car-draw.md,
// the fields in docs/known-data.md). The data is guest data shared with the
// recompiled code, so these work on guest memory with the originals' loads
// and stores in their order: the scalar float work as the translated code's
// doubles rounded to single (fused where the original fuses), the vector
// work on SSE vectors with flush-to-zero on as the translation turns it on
// (built with -ffp-model=strict, CMakeLists.txt). Each takes `stack`, the
// guest r1 at entry, and keeps the original's frame: its back chain written
// as stwu does (not its saved registers), its temporaries at the original's
// offsets, its callees run from it (so the world matrices and products the
// recorder and the interpolation see sit at the same guest addresses).
// Expect flush-to-zero off at entry (the adapters turn it off). Adapters:
// src/hooks/car_draw.cpp; each function's adapter falls back to its
// original where the plain C++ declines (nullopt, with nothing done).
//
// Part 1 (the shared leaves): Model::Draw, CarPart::Draw, Matrix::Shadow,
// CarLighting::Gather, LightKit::BlinkLevel, BlinkTimers::Advance and
// SceneModel::IsNearPlayerHeight. Native callers call these directly (only
// with the callees' plain C++ native: hooks::CarDrawCalleesNative), and
// where one declines, make the original's guest call from the same frame.

#pragma once

#include "scenery_lists/scenery_lists.h"

#include <cstdint>
#include <functional>
#include <optional>

namespace car_draw {

// What the plain C++ can't do itself: the guest calls it makes from the
// original's frame.
struct Calls {
  // The render queue's slot 4 and the draws' guest calls (the scenery
  // lists' DrawCalls, hooks::SceneryListsDrawCalls).
  scenery_lists::DrawCalls draw;
  // memcpy's original (sub_825A7410) from the frame at `sp` with `lr`,
  // where native::Buffers_Memcpy declines.
  std::function<void(uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr)>
      memcpy;
  // Lights::FindNearest's original (sub_820F4128; r3 to r9 `regs`, f1 to
  // f3) from the frame at `sp` with `lr`, where its plain C++ declines (a
  // NaN cone); its r3.
  std::function<uint64_t(const uint64_t regs[7], double f1, double f2, double f3, uint32_t sp,
                         uint32_t lr)>
      find_nearest;
  // Any guest function `function` from the frame at `sp` with `lr`, r3 on
  // from `regs` (`count` of them, all 64 bits); its r3 (64 bits). The
  // callees still recompiled (the morph's job maker, the light kit's LEDs
  // and spots) and the originals where a plain C++ declines.
  std::function<uint64_t(uint32_t function, const uint64_t* regs, int count, uint32_t sp,
                         uint32_t lr)>
      call;
};

// What a function leaves in r3: in the context already (`in_ctx`, the last
// guest call's or render queue call's), or `r3` (all 64 bits).
struct Left {
  bool in_ctx = false;
  uint64_t r3 = 0;
};

// What a draw leaves in r3: the last render queue call's r3 (`queued`; the
// adapter leaves the guest call's registers), or `r3`.
using Draw = scenery_lists::SceneryDraw;

// --- The part draws (car_draw.cpp) ---

// Model::Draw's frame.
constexpr uint32_t kModelDrawFrame = 176;
// Model::DrawShapeDepth's frame (CarPart::Draw's depth pass tail-calls it).
constexpr uint32_t kDrawShapeDepthFrame = 160;

// Model::Draw (sub_820D04E8): r3 = the model, r4 = the view-projection, r5
// = the world matrix, r6 = the shape, r7 = draw the depth entry too.
// Nothing when the model's +28 is 0 (r3 the model) or
// Model::IsShapeVisible answers 0 (its product at the frame's - 96; r3 its
// answer); then on the command list [0x8296B918] a chunk with two 3-slot
// entries, Recorder::RecordObject (inside the interpolation's hooks, with
// the context its wrapper keeps: the return address 0x820D0564 and r24-r31
// = `caller_r24`, the world matrix, the shape, the view-projection,
// 0x82970000, the second and first entries, the model) and
// RecordSceneLights; each group (0, then 1) with materials: its entry's +2
// = 3, +4 the object's header, +18 = 3, +20 the lights' header, +34 = 4,
// +36 = Model::RecordMaterials(model, shape, group, 1) (its frame 288 below
// this one), handed to the render queue with the key [0x8296B90C] (group 0,
// return address 0x820D05EC) or [0x8296B900] (0x820D0654) and the header's
// +72 as the depth; with `depth_too`, a 2-slot chunk (+2 = 3, +4 the
// header, +18 = 4, +20 = Model::RecordShapeDepth(model, shape, 0, 0, 0))
// when group 0 has materials, with [0x8296B908] (0x820D06E4). r3 as the
// original leaves it (0 where the last material count was 0). nullopt, with
// nothing done, where a recorder would decline (a vertex declaration the
// plain C++ leaves to the original, more than 4 textures on a material, a
// chunk open on another list, no room for every slot).
std::optional<Draw> ModelDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t model,
                              uint32_t view_projection, uint32_t world, uint32_t shape,
                              uint32_t depth_too, uint32_t caller_r24);

// CarPart::Draw (sub_820D06F0): r3 = a point record (+4 the model, +228 the
// shape): nothing when the model is 0 (r3 0); with r7 set,
// Model::DrawShapeDepth(model, r4, r5, shape, 0, 0, 0) (a tail call: its
// frame below `stack`; the recorder's context the return address 0x82596D9C
// and r24-r31 = 0, 0, 0, the shape, `caller_r28`, the model, the world
// matrix, the view-projection; its queue call returns to 0x82596E60); else
// Model::Draw(model, r4, r5, shape, r6) (a tail call, the same `stack` and
// `caller_r24`). nullopt, with nothing done, where the draw declines.
std::optional<Draw> CarPartDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t part,
                                uint32_t view_projection, uint32_t world, uint32_t r6,
                                uint32_t r7, uint32_t caller_r24, uint32_t caller_r28);

// --- The lighting (lighting.cpp) ---

// Matrix::Shadow (sub_82597FE0, a leaf): r3 = out (4x4), r4 = the light (a
// 4-vector), r5 = a point on the plane, r6 = the plane's normal (16-byte
// aligned): n = the normal normalized (the vector unit's estimate and one
// Newton step; a zero length leaves its zero dot product), stored at `stack`
// - 16 and read back; d = -(n . p), dot = n . L + d L.w; out = dot I - (n,
// d) (x) L, in the original's order and roundings.
void MatrixShadow(uint8_t* base, uint32_t out, uint32_t light, uint32_t point, uint32_t normal,
                  uint32_t stack);

// CarLighting::Gather's frame.
constexpr uint32_t kGatherFrame = 864;

// CarLighting::Gather (sub_820C9170): r3 = the out record (`out`, the
// register's 64 bits; +0 to +39 ten words, +48 + 64i ten matrices), r4 =
// the world matrix, r5 = the flag (the cone test, shadows and weights); see
// the row in car-draw.md. Its temporaries in its 864-byte frame as the
// original's (the scene's lighting vectors at +224 to +287, the point at
// +112, the 10 lights' list at +288), r3 and r5 saved in the caller's frame
// (`stack` + 20, + 36). Lights::FindNearest (its original where it declines,
// return address 0x820C92F8), Vec4::Transform (debug::Vec4Transform) and cos
// as plain C++. Returns r3 as the original leaves it (64 bits).
uint64_t CarLightingGather(uint8_t* base, const Calls& calls, uint32_t stack, uint64_t out,
                           uint32_t world, uint32_t flag);

// --- The light kit's blink (blink.cpp) ---

// LightKit::BlinkLevel (sub_820DC4B0, a leaf): `counter` (r3, frames), a
// (f1, the on fraction), b (f2, the fade), the rate (f3): the period
// int(rate x [0x82002C38] + [0x82000D44]) (fused, single) at `stack` - 16
// and the counter's remainder by it at `stack` - 8 (64-bit words, as the
// original's conversions leave them; a period of 0 or a counter of INT64_MIN
// over -1 logs the trap's warning, as the translation's trap does, and goes
// on with the original's guarded division), t = the remainder over the
// period; the full level [0x82002A3C], or [0x82002A10] in a race ([scene +
// 4] 1) of mode 2 ([[0x826C5B2C] + 32]). Returns f1: the full level while t
// is below a or NaN compares fail as the original's; 0.0 when b is 0.0;
// else the larger fade, the full level above 1.0, 0.0 below 0.0, else times
// the full level.
double LightKitBlinkLevel(uint8_t* base, uint64_t counter, double a, double b, double rate,
                          uint32_t stack);

// BlinkTimers::Advance (sub_820DC958, a leaf): r3 = a light kit's point
// block (+0 its record's address): the four 64-bit counters at +8 to +32 up
// by 1, then each group whose mode (record +4, +20, +36) is 2 or above has
// its counter (+16, +24, +32) back to 0 once it reaches (rate x
// [0x820056D0] + [0x820056C8]) x [0x820056D8] (doubles, fused; the rates
// record +16, +32, +48), each counter compared through `stack` - 16 as the
// original. The skip on render-only passes (--per_tick_blink_timers) is the
// caller's: hooks::CarDrawBlinkTimersHeld.
void BlinkTimersAdvance(uint8_t* base, uint32_t block, uint32_t stack);

// --- The height cull (car_draw.cpp) ---

// SceneModel::IsNearPlayerHeight's frame.
constexpr uint32_t kNearPlayerHeightFrame = 192;

// SceneModel::IsNearPlayerHeight (sub_820DAB58): r3 = the scene's pass
// description (+0 the camera pass index), r4 = the scene model's position:
// 1 without vehicles (Vehicle::Get(0) 0) or for passes 4 and above; else the
// player (0, or for pass 2 1, or in the race kind 6 with the race data's
// + 0x59614 2 [0x826C7824] == 0)'s vehicle (Racers::Vehicle; 1 when none),
// its matrix (+3312) copied into the frame's +96 (memcpy, return address
// 0x820DAC1C), (0, 0, 0, 1) at +80 transformed by it (Vec4::Transform in
// place); 0 when |its y - the position's y| is above [0x82002DF4], else 1.
uint32_t SceneModelIsNearPlayerHeight(uint8_t* base, const Calls& calls, uint32_t stack,
                                      uint32_t pass, uint32_t position);

// --- The morph (morph.cpp) ---
//
// Part 2: XGSetVertexBufferHeader, WorkMem::AllocVertexBuffer,
// Model::QueueJobRelease and Model::MorphShape.

// XGSetVertexBufferHeader's frame.
constexpr uint32_t kXgVertexBufferHeaderFrame = 128;

// XGSetVertexBufferHeader (sub_8235A688): r3 = the length in bytes, r4 =
// the usage, r5 = the pool (not read), r6 = the offset, r7 = the header:
// the 32 bytes cleared (memset), +0 = 1 (0x00200001 with usage bit 0x4, |
// 0x00400000 with bit 0x200), +4 = 1, +20 = 0xFFFF0000, +24 = offset | 3,
// +28 = (length & 0x03FFFFFC) | (+28's bit 0x08000000, 0 after the clear) |
// 0x10000000 | 2. Its frame's back chain. Returns r3: the header (memset's).
uint32_t XgSetVertexBufferHeader(uint8_t* base, uint32_t length, uint32_t usage, uint32_t offset,
                                 uint32_t header, uint32_t stack);

// WorkMem::AllocVertexBuffer's frame.
constexpr uint32_t kAllocVertexBufferFrame = 128;

// WorkMem::AllocVertexBuffer (sub_8258A9E8): r3 = where the header's
// address goes, r4 = bytes: from the current buffer i ([0x8296B780]), the
// header 0x82957780 + 32 x (1,280 i + [0x8296B798 + 4i]) set up
// (XGSetVertexBufferHeader(bytes, 0, 1, the bytes used, header), its frame
// below this one) and made absolute (Resource::Relocate with the buffer's
// start [0x8296B7A8 + 4i]), the header count up by 1, the bytes used
// ([0x8296B7A0 + 4i]) up by r4 and rounded up to 128, [r3] = the header.
// Returns r3: the memory, the start plus the bytes used before. nullopt,
// with nothing done, where the original would print a debug message
// (WorkMemFits false; it returns the buffer's start then).
std::optional<uint32_t> WorkMemAllocVertexBuffer(uint8_t* base, uint32_t header_out,
                                                 uint32_t bytes, uint32_t stack);

// Model::QueueJobRelease's frame.
constexpr uint32_t kQueueJobReleaseFrame = 144;

// Model::QueueJobRelease (sub_82594E88): r3 = the model (not read), r4 = a
// job: on the command list [0x8296B918] a chunk of four slots, the head
// (+2 = 4, +4 = the second slot), command 22 (+0 = 22, +4 = 1, +8 = 0), its
// argument (+4 = the job, +8 = 1) and command 36 (+4, +8 = 0), handed to
// the render queue (its slot 4, a guest call from this frame returning to
// 0x82594F4C) with the bucket [0x8296B904] and the key [0x82000A34]. r3 the
// queue call's (in the context). nullopt, with nothing done, where the
// list has no room for the four slots (or a chunk is open on another list).
std::optional<Left> ModelQueueJobRelease(uint8_t* base, const Calls& calls, uint32_t job,
                                         uint32_t stack);

// Model::MorphShape's frame.
constexpr uint32_t kMorphShapeFrame = 128;

// Model::MorphShape (sub_82595110): r3 = the model, r4 = a morph record
// (Morph::SetUp's), r5 = the base shape's index (64 bits: the shape at
// [model + 40] + 544 x r5): stream 0's bytes ([[shape + 400] + 12]) from the
// work memory (the header's address to the shape's +412, the memory to
// +416), stream 1's the same (+420, +424) unless 0 (+424 = 0); the record's
// +8 and +12 = +416 and +424. With the pool maker [0x8296B914], its slot 1
// (a guest call, r3 the maker, r4 the record, returning to 0x825951A4)
// makes the job, which goes to [shape + 408] + 20 and to
// Model::QueueJobRelease; without one, Morph::Blend(record) (its plain C++,
// else its original returning to 0x825951CC) and [+408] + 20 = 0. Either
// way +428 = 1. nullopt, with nothing done, where an allocation would print
// its debug message or the release has no room.
std::optional<Left> ModelMorphShape(uint8_t* base, const Calls& calls, uint32_t model,
                                    uint32_t morph, uint64_t shape_index, uint32_t stack);

// --- The light kit (light_kit.cpp) ---
//
// Part 2: SceneLights::Draw, LightPoints::DrawFront, DrawSides, DrawRear and
// their neons. The LEDs and spots (not seen in play) and
// LightKit::ChaseLevel stay guest calls from the original's frames.

// SceneLights::Draw's frame, LightPoints::Draw*'s and the neons'.
constexpr uint32_t kSceneLightsDrawFrame = 976;
constexpr uint32_t kLightPointsDrawFrame = 304;
constexpr uint32_t kNeonDrawFrame = 112;

// The light kit's three groups: the front bumper (kit + 144, the record's
// mode +4, its fractions and rate +8 to +16), the side skirts (+4480, +20,
// +24 to +32) and the rear bumper (+8816, +36, +40 to +48).
enum class LightGroup { kFront, kSides, kRear };

// LightPoints::DrawFrontNeon (sub_820DCC10), DrawSidesNeon (sub_820DD0B0),
// DrawRearNeon (sub_820DD5C0): r3 = the group (+0 the record), r4 = its
// counter, r5 = the block, r6 = the view-projection: the level
// LightKit::BlinkLevel(counter, the record's fractions and rate) (at this
// frame), PartModel::SetAlpha(group +20, +244, level), CarPart::Draw(group
// + 16, r6, the block's +16, 0, 0) (its original where it declines,
// returning after its call). `caller_r24` and `caller_r28` the registers
// CarPart::Draw sees (its callers don't change them).
Left LightPointsDrawNeon(uint8_t* base, const Calls& calls, LightGroup which, uint32_t stack,
                         uint32_t group, uint64_t counter, uint32_t block,
                         uint32_t view_projection, uint32_t caller_r24, uint32_t caller_r28);

// LightPoints::DrawFront (sub_820DDAE8), DrawSides (sub_820DDB98),
// DrawRear (sub_820DDC48): by the record's mode (unsigned): 0 nothing (r3
// the group); 1 the neon; up to 6 the LEDs (sub_820DCC70, sub_820DD110,
// sub_820DD620); up to 9 (the sides 11) the spots (sub_820DCF98,
// sub_820DD438, sub_820DD948); above, nothing. Each with the
// view-projection copied (memcpy) into this frame (+208 the neon's, +80 the
// LEDs', +144 the spots'). The LEDs and spots are guest calls.
Left LightPointsDraw(uint8_t* base, const Calls& calls, LightGroup which, uint32_t stack,
                     uint32_t group, uint64_t counter, uint32_t block, uint32_t view_projection,
                     uint32_t caller_r24, uint32_t caller_r28);

// SceneLights::Draw (sub_820DDE58): r3 = the light kit, r4 = the scene
// model's block; see the row in car-draw.md. The local SceneModelBlock at
// the frame's +208 (its w's at +144), nothing more without bit 0x8 of the
// scene's ([0x826C2E44]) pass mask +5132; the block copied in (memcpy); the
// view-projection at +144 (Matrix::Copy, noted for interpolation): camera
// pass 0's in the front end (the kit's scene's +4 0; the copy's +68 the
// float at 0x820029D8, its world the identity first with the block's +724
// set), else the scene's +5084's (address 144 when disabled);
// BlinkTimers::Advance(kit + 96) when the kit's scene's +5048 is 0 and
// `advance_blink` (false on a render-only pass with --per_tick_blink_timers:
// hooks::CarDrawBlinkTimersHeld); [0x826C5104] flipped, the three groups
// drawn (each with a fresh copy of the view-projection at +80) and it
// flipped back.
Left SceneLightsDraw(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t kit,
                     uint32_t block, bool advance_blink, uint32_t caller_r24,
                     uint32_t caller_r28);

}  // namespace car_draw
