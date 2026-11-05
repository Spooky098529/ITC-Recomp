// FramePasses::Record's part 3 (record.h): slots 256 to 358 of the chunk,
// heads 11 to 17, step for step as the recompiled code from the
// AllocateSlots call returning to 0x82097654 to the CloseChunk returning to
// 0x82097E8C. Commands (docs/gpu-functions.md): 1 render states, 2 sampler
// states, 36 a chain's end; the constants recorder (this's +20) after each
// of the last two heads' samplers. The LOD bias float goes through the
// frame's +84, and the first sampler loop's counter through its +80, as in
// the original.

#include "frame_passes/record.h"

#include <initializer_list>
#include <utility>

namespace frame_passes::record {

namespace {

// The LOD bias the samplers take, at the frame's +88 (0x82970000) - 18188.
constexpr uint32_t kLodBiasOffset = 18188;

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

// A chain's end: command 36.
void End(Recorder& r) { r.Slot3(36, 0, 0); }

// A float stored as a word of the frame (the original's stfs there; the
// slots read it back with LoadFrame).
void FrameFloat(Recorder& r, uint32_t offset, float value) {
  guest::StoreFloat(r.base, r.frame + offset, value);
}

// Heads 16 and 17: alpha blend, alpha test, alpha reference 0, alpha
// function 6, Z enable, Z write and fill mode 0, Z function 6, the
// high-precision blend off; then 21 sampler states: samplers 0-3's LOD bias
// (through the frame's +84), min, mag and mip filters 1 (head 16 keeps the
// loop's counter at the frame's +80), sampler 6's min and mag filters 1, mip
// filter 2, address U and V 2; the constants recorder.
void SamplerPass(Recorder& r, bool counter_in_frame) {
  RenderStates(r, {{60, 0},
                   {96, 0},
                   {100, 0},
                   {104, 6},
                   {40, 0},
                   {48, 0},
                   {52, 0},
                   {44, 6},
                   {308, 0},
                   {312, 0},
                   {316, 0},
                   {320, 0}});
  r.Slot3(2, 21, 0);
  if (counter_in_frame) {
    r.StoreFrame(80, 0);
  }
  for (uint32_t sampler = 0; sampler < 4; sampler++) {
    {
      const uint32_t s = r.Slot();
      r.Store(s + 0, sampler);
      r.Store(s + 4, 28);  // LOD bias
      FrameFloat(r, 84, LoadConverted(r.base, r.LoadFrame(88) - kLodBiasOffset));
      r.Store(s + 8, r.LoadFrame(84));
    }
    r.Slot3(sampler, 20, 1);  // min filter
    r.Slot3(sampler, 16, 1);  // mag filter
    r.Slot3(sampler, 24, 1);  // mip filter
    if (counter_in_frame) {
      r.StoreFrame(80, sampler + 1);
    }
  }
  r.Slot3(6, 20, 1);  // min filter
  r.Slot3(6, 16, 1);  // mag filter
  r.Slot3(6, 24, 2);  // mip filter
  r.Slot3(6, 0, 2);   // address U
  r.Slot3(6, 4, 2);   // address V
  r.Constants();
  End(r);
}

}  // namespace

void RecordPart3(Recorder& r) {
  // Heads 11 to 13 (the frame's +132 to +140): color write mask 2, 4, 8.
  r.StoreFrame(132, r.Head());
  RenderStates(r, {{212, 2}});
  End(r);
  r.StoreFrame(136, r.Head());
  RenderStates(r, {{212, 4}});
  End(r);
  r.StoreFrame(140, r.Head());
  RenderStates(r, {{212, 8}});
  End(r);

  // Head 14 (the frame's +144): color write mask 15, Z enable 1.
  r.StoreFrame(144, r.Head());
  RenderStates(r, {{212, 15}, {40, 1}});
  End(r);

  // Head 15 (kept in a register only): Z enable 0.
  r.Head();
  RenderStates(r, {{40, 0}});
  End(r);

  // Head 16 (the frame's +92) and head 17 (the frame's +80).
  r.StoreFrame(92, r.Head());
  SamplerPass(r, true);
  r.StoreFrame(80, r.Head());
  SamplerPass(r, false);
}

}  // namespace frame_passes::record
