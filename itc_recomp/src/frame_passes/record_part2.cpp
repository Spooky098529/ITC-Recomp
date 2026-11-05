// FramePasses::Record's part 2 (record.h): slots 154 to 255 of the chunk,
// heads 6 to 10, step for step as the recompiled code from the AllocateSlots
// call returning to 0x82096EA0 to the one returning to 0x82097654 (where
// part 3 starts). Commands (docs/gpu-functions.md): 1 render states, 2
// sampler states, 8/9 shaders, 23 targets, 34/35 save/restore, 36 a chain's
// end. The shaders' slots and two depth-bias floats go through the frame's
// +80 and +84, as in the original.

#include "frame_passes/record.h"

#include "models/model.h"

#include <initializer_list>
#include <utility>

namespace frame_passes::record {

namespace {

// The floats this part reads: 0.0 (the original's f31, loaded at part 1's
// Clear), -8.0 (slope-scaled depth bias) and -0.0001 (depth bias).
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kSlopeBias = 0x82000F18;
constexpr uint32_t kDepthBias = 0x82000F1C;

// A float as the original moves it: loaded to a double, stored as a float.
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

// Sampler 6's states (+12 left as it was): [6, state, value].
void Sampler6(Recorder& r, uint32_t state, uint32_t value) { r.Slot3(6, state, value); }

// A chain's end: command 36.
void End(Recorder& r) { r.Slot3(36, 0, 0); }

// A float stored as a word of the frame (the original's stfs there; the
// slots read it back with LoadFrame).
void FrameFloat(Recorder& r, uint32_t offset, float value) {
  guest::StoreFloat(r.base, r.frame + offset, value);
}

// The shared shader pair 0 (commands 8 and 9), each shader's slot kept at
// the frame's +84 across the getter and read back to store it.
void VertexShader0(Recorder& r) {
  r.Slot3(8, 1, 0);
  r.StoreFrame(84, r.Slot());
  r.Store(r.LoadFrame(84), models::SharedVertexShader(r.base, 0));
}
void PixelShader0Slot(Recorder& r) {
  r.StoreFrame(84, r.Slot());
  r.Store(r.LoadFrame(84), models::SharedPixelShader(r.base, 0));
}

}  // namespace

void RecordPart2(Recorder& r) {
  // Head 6 (the frame's +108): as head 5 without the viewport: alpha blend
  // on, alpha test, Z enable, Z write and fill mode 0, Z function 6, the
  // high-precision blend on; samplers 0-3's filters 0 and mip filter 2 (the
  // counter at the frame's +80), sampler 6's address U and V 2, the
  // samplers recorder.
  r.StoreFrame(108, r.Head());
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
  Sampler6(r, 0, 2);  // address U
  Sampler6(r, 4, 2);  // address V
  r.Samplers();
  End(r);

  // Head 7 (the frame's +116): save (34), an empty slot, render targets 0-3
  // = 0 (23), the shared shader pair 0 (the pixel shader's command 9 with 2
  // slots, the second 0), then alpha blend 1, alpha test 1, alpha reference
  // 0, alpha function 6, Z enable and Z write 1, fill mode 0, Z function 6,
  // depth bias -0.0001 (208) and slope-scaled depth bias -8.0 (204), both
  // through the frame's +80 and +84.
  r.StoreFrame(116, r.Head());
  r.Slot3(34, 1, 0);
  r.Slot();
  r.Slot3(23, 4, 0);
  for (uint32_t target = 0; target < 4; target++) {
    r.Slot2(target, 0);
  }
  VertexShader0(r);
  r.Slot3(9, 2, 0);
  PixelShader0Slot(r);
  r.Slot1(0);
  r.Slot3(1, 10, 0);
  r.Slot2(60, 1);
  r.Slot2(96, 1);
  r.Slot2(100, 0);
  r.Slot2(104, 6);
  r.Slot2(40, 1);
  r.Slot2(48, 1);
  r.Slot2(52, 0);
  {
    const uint32_t s = r.Slot();
    FrameFloat(r, 80, LoadConverted(r.base, kDepthBias));
    r.Store(s + 0, 44);
    r.Store(s + 4, 6);
  }
  {
    const uint32_t s = r.Slot();
    FrameFloat(r, 84, LoadConverted(r.base, kSlopeBias));
    r.Store(s + 0, 208);
    r.Store(s + 4, r.LoadFrame(80));
  }
  r.Slot2(204, r.LoadFrame(84));
  End(r);

  // Head 8 (the frame's +120): the depth biases back to 0.0 (the original's
  // f31) through the frame's +80 and +84.
  r.StoreFrame(120, r.Head());
  const float zero = LoadConverted(r.base, kZero);
  {
    const uint32_t s = r.Slot();
    FrameFloat(r, 80, zero);
    r.Store(s + 0, 1);
    r.Store(s + 4, 2);
    r.Store(s + 8, 0);
  }
  {
    const uint32_t s = r.Slot();
    FrameFloat(r, 84, zero);
    r.Store(s + 4, r.LoadFrame(80));
    r.Store(s + 0, 208);
  }
  r.Slot2(204, r.LoadFrame(84));
  End(r);

  // Head 9 (the frame's +124): restore (35) and an empty slot.
  r.StoreFrame(124, r.Head());
  r.Slot3(35, 1, 0);
  r.Slot();
  End(r);

  // Head 10 (the frame's +128): the shared shader pair 0, then alpha blend
  // 0, alpha test 1, alpha reference 0, alpha function 6, Z enable 0, Z
  // write 1, fill mode 0, Z function 6, color write mask 1 (212).
  r.StoreFrame(128, r.Head());
  VertexShader0(r);
  r.Slot3(9, 1, 0);
  PixelShader0Slot(r);
  RenderStates(r, {{60, 0},
                   {96, 1},
                   {100, 0},
                   {104, 6},
                   {40, 0},
                   {48, 1},
                   {52, 0},
                   {44, 6},
                   {212, 1}});
  End(r);
}

}  // namespace frame_passes::record
