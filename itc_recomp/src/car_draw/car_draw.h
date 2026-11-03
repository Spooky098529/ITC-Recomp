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
  // As `call`, with r28 = `r28` during the call (put back after): what
  // CarPart::Draw's depth pass hands its recorder's context.
  std::function<uint64_t(uint32_t function, const uint64_t* regs, int count, uint32_t sp,
                         uint32_t lr, uint32_t r28)>
      call_r28;
  // As `call`, with r24 = `r24` (all 64 bits) during the call (put back
  // after): what Model::Draw hands its recorder's context, for draws whose
  // caller changed r24 (the head and tail lights' glows).
  std::function<uint64_t(uint32_t function, const uint64_t* regs, int count, uint32_t sp,
                         uint32_t lr, uint64_t r24)>
      call_r24;
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

// --- The parts' small draws (parts.cpp) ---
//
// Part 3: the PartsPoints draws of docs/functions/car-draw.md that only
// draw records (CarPart::Draw), with PartModel::SetPair1002Alpha's
// window alphas, the number plates' textures, the bonnet's hinge and the
// mirrors' depth draws. r3 = the group (or the record), r4 = the scene
// model's block (+4 its flag bits, +16 the world, +628 to +640 its
// alphas and angle), r5 = the camera pass (its +144 the view-projection).
// Each keeps the original's frame (its back chain, its temporaries) and
// draws from it; `caller_r24` is r24 at its entry (Model::Draw's recorder
// context; none of them changes it). CarPart::Draw's original runs from
// the same frame (with the same return address) where its plain C++
// declines. Each returns r3 as the original leaves it: the group where it
// draws nothing, else the last draw's.

// CarPart::Draw(part, view_projection, world, 0, depth) from `frame`
// returning to `lr`: its plain C++, else its original from that frame (r28 =
// `r28` during it, for the depth pass's recorder context). What it leaves in
// r3. The parts' draws share it.
Left PartsDrawRecord(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t part,
                     uint32_t view_projection, uint32_t world, uint32_t depth, uint32_t r24,
                     uint32_t r28, uint32_t lr);

// The frames.
constexpr uint32_t kPartsSmallFrame = 112;
constexpr uint32_t kPartsFrame = 128;
constexpr uint32_t kPartsBonnetFrame = 528;
constexpr uint32_t kPartsShadowFrame = 256;

// PartsPoints::DrawFrontBumperLights (sub_820DE1C8, a 128-byte frame):
// records 0, 1 (r6 0) or 2, by the block's bits 0x1, 0x2, 0x4 records 3, 4,
// 5, then SetPair1002Alpha(record 6, the block's +636) and record 6.
Left PartsDrawFrontBumperLights(uint8_t* base, const Calls& calls, uint32_t stack,
                                uint32_t group, uint32_t block, uint32_t pass, uint32_t r6,
                                uint32_t caller_r24);
// PartsPoints::DrawRearBumperLights (sub_820DE3C0, 128): record 0, by the
// block's bits 0x1, 0x10, 0x2, 0x4 records 1, 2, 3, 4.
Left PartsDrawRearBumperLights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                               uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawLeftDoorPanel (sub_820DE6E0, 128) and DrawRightDoorPanel
// (sub_820DE8C8): nothing unless record 0 has a model; record 6 with the
// door's matrix (+6016), then with the block's bit 0x2 (left) or 0x4
// (right) record 7 + the parts record's (+6084) +28, the same matrix.
Left PartsDrawDoorPanel(uint8_t* base, const Calls& calls, bool right, uint32_t stack,
                        uint32_t group, uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawDoorWindows02 (sub_820DE760, 112; records 2 and 3, the
// block's +628) and DrawDoorWindows01 (sub_820DE948; records 4 and 5,
// +632): nothing unless record 0 has a model; SetPair1002Alpha on both,
// then both drawn with the door's matrix.
Left PartsDrawDoorWindows(uint8_t* base, const Calls& calls, bool first, uint32_t stack,
                          uint32_t group, uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawDoorsBody (sub_820DEA88, 112): record 11, then record 12
// + k where k, the parts record's (+5520) +12, is below 11 (signed).
Left PartsDrawDoorsBody(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                        uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawDoorsWindows (sub_820DEAF0, 128): SetPair1002Alpha on
// records 0 and 1 (the block's +632), records 0, 1, 2, then with bit 0x2
// record 3 + k, with 0x4 record 7 + k (k the parts record's +28).
Left PartsDrawDoorsWindows(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawFendersBody (sub_820DED88, 112): records 5 and 6.
Left PartsDrawFendersBody(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                          uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawFenderLights (sub_820DEDD8, 128): records 0, 1 (r6 0) or
// 2, by bits 0x2 and 0x4 records 3 and 4.
Left PartsDrawFenderLights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t r6, uint32_t caller_r24);
// PartsPoints::DrawCabinWindow02 (sub_820DEF38, 112): with bit 0x8 record
// 0; SetPair1002Alpha(record 3, +628), record 3.
Left PartsDrawCabinWindow02(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                            uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawCabinWindow01 (sub_820DEFA8, 128):
// SetPair1002Alpha(record 2, +632), record 2, with bit 0x8 record 1.
Left PartsDrawCabinWindow01(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                            uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawEngine (sub_820DF070, 112): records 0 and 1.
Left PartsDrawEngine(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                     uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawFrontBumper (sub_820DF308, 112) and DrawRearBumper
// (sub_820DF458): the body (record 7, or 5 at the rear), then the number
// plate's model m (+1924, or +1444) gets its texture (+456: the group's
// +2644, or +1684, with the group's +2640, or +1680, set, m set, the race
// mode not 7 or 8 and [0x82816E3C] 0; else XPR::GetTexture(the scene's
// +4648, 0)) and +432 = 1 around the plate's draw (record 8 + the car
// record's (+2648) +3684, or record 6). The front draws no plate without
// m; the rear still writes m's +456 and +432 (guest words 456 and 432
// with no model), as the original.
Left PartsDrawFrontBumper(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                          uint32_t block, uint32_t pass, uint32_t caller_r24);
Left PartsDrawRearBumper(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                         uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawBonnet (sub_820E0998, 528): nothing unless record 0 has
// a model; in the front end (the scene's +4 0) the world opened about the
// hinge: the block's +640 clamped to [0.0, pi] (at +80), an identity (+96,
// copied to +160) turned by it (Matrix::RotateX), times record 0's
// inverse (+80, copied to +352) on the left, its matrix (+16, copied to
// +416) and the block's world on the right (Matrix::Multiply and Copy,
// noted for interpolation, the 1.0 w's written before each multiply as
// the original); in a race the block's world. Then record 1 with it.
Left PartsDrawBonnet(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                     uint32_t block, uint32_t pass, uint32_t caller_r24);
// The doors' and the headlight body's frames.
constexpr uint32_t kPartsDoorFrame = 720;
constexpr uint32_t kPartsHeadlightBodyFrame = 784;

// PartsPoints::DrawLeftDoor (sub_820E0B40, 720) and DrawRightDoor
// (sub_820E0E38, the same code; the block's +676 and +680 for +660 and
// +664, the turn by -theta): nothing unless record 0 has a model (r3 the
// group); theta (+660) and phi (+664) clamped to [0.0, pi]; the door's
// matrix kept at the group's +6016 for the later door draws: with the car
// record's ([group + 6080]) +3688 0, record 0's inverse (+80, memcpy'd to
// +544) x the identity turned by theta about y (+480, Matrix::RotateY) x
// its matrix (+16, to +608), else the identity turned by -phi about x and
// theta about y (+288) about record 1's matrices (+256 to +544, +320 to
// +608); then x the block's world (into +224, copied back). Records 11, 12
// (or 13 with +3688 set) and 14 + k (k the parts record's +12, below 11)
// drawn with it, r28 = the pass. The multiplies and copies noted for
// interpolation with the original's return addresses.
Left PartsDrawDoor(uint8_t* base, const Calls& calls, bool right, uint32_t stack,
                   uint32_t group, uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawHeadlightBody (sub_820E13E0, 784): nothing unless record
// 0 has a model (r3 the group); the pop-up's matrix kept at the group's
// +3120 for DrawHeadlights: the angle, with the block's +688 set its +692
// clamped to [0.0, the group's +3184], else +3184 or 0.0 with the block's
// bits 0x1 or 0x40; record 0's inverse x the identity turned by -angle
// about x x its matrix x the block's world (the identity at +480 or +608
// by the path). Record 12 drawn with it, r28 = the pass.
Left PartsDrawHeadlightBody(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                            uint32_t block, uint32_t pass, uint32_t caller_r24);
// PartsPoints::DrawMirror (sub_820E1138, 256; `world` its r6) and
// DrawWithShadow (sub_820E1260; `world` the block's +16): the record
// drawn, then when the pass description's (the scene's +5072) camera pass
// (+16) is enabled and its +60 has bit 0x100, that pass's view-projection
// (memcpy to +144) times the description's +112 on the left (into +80,
// its w's 1.0 first; copied back to +144), the render queue's keys
// [0x8296B8FC] and [0x8296B8F8] saved to its +76 and +80 and set to its
// +56, the record drawn depth-only with it (r28 = the world for its
// recorder's context) and the keys put back.
Left PartsDrawShadowed(uint8_t* base, const Calls& calls, bool mirror, uint32_t stack,
                       uint32_t record, uint32_t pass, uint32_t world, uint32_t caller_r24);

// --- The head and tail lights (lamps.cpp) ---
//
// Part 4: PartsPoints::DrawTailLights and DrawHeadlights with their
// camera-facing glows: the lamps' records through CarPart::Draw, then, by
// the block's bits, the scene's glow model ([0x826C2E44] + 480) colored
// (PartModel::SetMaterials) and drawn through Model::Draw at each glow node,
// turned to face the camera (Matrix::TransposeRotation of the pass's view,
// Projector::Matrices), with [0x826C5104] flipped around them. The matrices
// built in the original's frame at its offsets (the identities as stvx
// stores them, copied doubleword by doubleword), the multiplies and copies
// matrix_math's noted for interpolation with the original's return
// addresses; the material passes' and the projector's plain C++ (their
// originals from this frame where they decline). The glows' Model::Draw
// sees r24 = 0 (the tail lights) or 0x826C0000 (the headlights), as the
// original's registers hold it there; the lamps' records `caller_r24`.

constexpr uint32_t kPartsTailLightsFrame = 1568;
constexpr uint32_t kPartsHeadlightsFrame = 1552;

// PartsPoints::DrawTailLights (sub_820DF8B0): r3 = the group (pb + 27680),
// r4 = the block, r5 = the camera pass; `caller_r28` r28 at entry (the
// lamps' CarPart::Draw sees it). See the row in car-draw.md.
Left PartsDrawTailLights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                         uint32_t block, uint32_t pass, uint32_t caller_r24,
                         uint32_t caller_r28);
// PartsPoints::DrawHeadlights (sub_820E1690): r3 = the group (pb + 24480),
// r4 = the block, r5 = the camera pass, r6 and r7 the color table's index
// and sub-index (TableVector). Nothing when record 0 has no model (r3 the
// group).
Left PartsDrawHeadlights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                         uint32_t block, uint32_t pass, uint64_t index, uint64_t sub,
                         uint32_t caller_r24);

}  // namespace car_draw
