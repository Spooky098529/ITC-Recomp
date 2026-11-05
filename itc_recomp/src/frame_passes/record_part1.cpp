// FramePasses::Record's part 1 (record.h): slots 0 to 153 of the chunk,
// heads 0 to 5, step for step as the recompiled code from the AllocateSlots
// call returning to 0x820964A8 to the one returning to 0x82096EA0 (where
// part 2 starts). Commands (docs/gpu-functions.md): 1 render states, 2
// sampler states, 8/9 shaders, 23 targets, 26 Clear, 29 targets back, 30
// viewport, 34/35 save/restore, 36 a chain's end.

#include "frame_passes/record.h"

#include "models/model.h"

#include <initializer_list>
#include <utility>

namespace frame_passes::record {

namespace {

// The floats the chunks read: 0.0 (the Clear's Z, the viewport's near Z,
// the original's f31), 1.0 (the viewport's far Z) and the LOD bias.
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kOne = 0x82000A30;
constexpr uint32_t kLodBias = 0x8296B8F4;

// A float as the original moves it: loaded to a double, stored as a float
// (the host conversion quiets signaling NaNs as the recompiled code does).
float LoadConverted(uint8_t* base, uint32_t address) {
  const double value = double(guest::LoadFloat(base, address));
  return float(value);
}

// Command 1 with these render states: [1, count, 0], then [state, value]
// slots (+8, +12 left as they were).
void RenderStates(Recorder& r, std::initializer_list<std::pair<uint32_t, uint32_t>> states) {
  r.Slot3(1, uint32_t(states.size()), 0);
  for (const auto& [state, value] : states) {
    r.Slot2(state, value);
  }
}

// The state resets of heads 1 and 2: alpha blend (60), alpha test (96),
// alpha reference (100) 0, alpha function (104) 4, Z enable (40) and Z
// write (48) 1, fill mode (52) 0, Z function (44) 6, the high-precision
// blend per target (308-320).
void StateResets(Recorder& r, uint32_t blend, uint32_t test, uint32_t high_precision) {
  RenderStates(r, {{60, blend},
                   {96, test},
                   {100, 0},
                   {104, 4},
                   {40, 1},
                   {48, 1},
                   {52, 0},
                   {44, 6},
                   {308, high_precision},
                   {312, high_precision},
                   {316, high_precision},
                   {320, high_precision}});
}

// Sampler 6's states (+12 left as it was): [6, state, value].
void Sampler6(Recorder& r, uint32_t state, uint32_t value) { r.Slot3(6, state, value); }

// A chain's end: command 36.
void End(Recorder& r) { r.Slot3(36, 0, 0); }

}  // namespace

void RecordPart1(Recorder& r) {
  // Head 0: the targets back (command 29, mask 0x8F) and the Clear.
  r.Head();
  r.Slot3(29, 1, 0);
  r.Slot1(0x8F);
  r.Slot3(26, 1, 0);
  // The Clear's 280 bytes: flags 49, color 0xFF000000, Z 0.0, stencil 0
  // (+12 to +267 left as they were).
  const uint32_t clear = r.Bytes(280);
  const float zero = LoadConverted(r.base, kZero);  // the original's f31
  guest::StoreFloat(r.base, clear + 272, zero);
  r.Store(clear + 276, 0);
  r.Store(clear + 268, 0xFF000000u);
  r.Store(clear + 4, 0);
  r.Store(clear + 0, 0);
  r.Store(clear + 8, 49);
  End(r);

  // Head 1 (kept at the frame's +104): the state resets, samplers 0-3's
  // LOD bias, filters and mip filter, sampler 6's, the constants.
  r.StoreFrame(104, r.Head());
  StateResets(r, 0, 0, 0);
  r.Slot3(2, 21, 0);
  r.StoreFrame(88, 0x82970000u);
  for (uint32_t sampler = 0; sampler < 4; sampler++) {
    // The LOD bias (28) through the frame's +84.
    const uint32_t s = r.Slot2(sampler, 28);
    guest::StoreFloat(r.base, r.frame + 84, LoadConverted(r.base, kLodBias));
    r.Store(s + 8, r.LoadFrame(84));
    r.Slot3(sampler, 20, 1);  // min filter
    r.Slot3(sampler, 16, 1);  // mag filter
    r.Slot3(sampler, 24, 1);  // mip filter
  }
  Sampler6(r, 20, 1);
  Sampler6(r, 16, 1);
  Sampler6(r, 24, 2);
  Sampler6(r, 0, 2);  // address U
  Sampler6(r, 4, 2);  // address V
  r.Constants();
  End(r);

  // Head 2 (only in a register): the state resets with alpha blend and
  // test on, the high-precision blend on, the constants.
  r.Head();
  StateResets(r, 1, 1, 1);
  r.Constants();
  End(r);

  // Head 3 (the frame's +96): save (34), an empty slot, render targets 0-3
  // = 0 (23), the shared shader pair 0's vertex shader (8) and a pixel
  // shader 0 (9).
  r.StoreFrame(96, r.Head());
  r.Slot3(34, 1, 0);
  r.Slot();
  r.Slot3(23, 4, 0);
  for (uint32_t target = 0; target < 4; target++) {
    r.Slot2(target, 0);
  }
  r.Slot3(8, 1, 0);
  r.Slot1(models::SharedVertexShader(r.base, 0));
  r.Slot3(9, 1, 0);
  r.Slot1(0);
  End(r);

  // Head 4 (the frame's +100): restore (35) and an empty slot.
  r.StoreFrame(100, r.Head());
  r.Slot3(35, 1, 0);
  r.Slot();
  End(r);

  // Head 5 (the frame's +112): alpha blend on, alpha test, Z enable, Z
  // write and fill mode 0, Z function 6, the high-precision blend on;
  // samplers 0-3's filters 0 and mip filter 2 (the counter at the frame's
  // +80), sampler 6's address U and V 2, the samplers recorder, the
  // viewport (0, 0, 1280 x 720, Z 0.0 to 1.0).
  r.StoreFrame(112, r.Head());
  RenderStates(r, {{60, 1},
                   {96, 0},
                   {40, 0},
                   {48, 0},
                   {52, 0},
                   {44, 6},
                   {308, 1},
                   {312, 1},
                   {316, 1},
                   {320, 1}});
  r.Slot3(2, 14, 0);
  r.StoreFrame(80, 0);
  for (uint32_t sampler = 0; sampler < 4; sampler++) {
    r.Slot3(sampler, 20, 0);  // min filter
    r.Slot3(sampler, 16, 0);  // mag filter
    r.Slot3(sampler, 24, 2);  // mip filter
    r.StoreFrame(80, sampler + 1);
  }
  Sampler6(r, 0, 2);
  Sampler6(r, 4, 2);
  r.Samplers();
  r.Slot3(30, 1, 0);
  const uint32_t viewport = r.Slot();
  guest::StoreFloat(r.base, viewport + 8, zero);
  guest::StoreU16(r.base, viewport + 0, 0);
  guest::StoreFloat(r.base, viewport + 12, LoadConverted(r.base, kOne));
  guest::StoreU16(r.base, viewport + 2, 0);
  guest::StoreU16(r.base, viewport + 6, 720);
  guest::StoreU16(r.base, viewport + 4, 1280);
  End(r);
}

}  // namespace frame_passes::record
