// The morph the wheels' set-ups run on the tire, rim and brake models
// (docs/functions/second-set.md: Morph::SetUp, Run and Blend over the model
// class's shape accessors), and the tire's two table helpers, as plain C++.
// Guest data shared with the recompiled code, so these work on guest memory
// with the originals' loads and stores in their order: the scalar float work
// as the translated code's doubles rounded to single, the vector work on SSE
// vectors with flush-to-zero on as the translated code turns it on. `stack`
// is the guest r1 at entry. Built with -ffp-model=strict. Adapters:
// src/hooks/second_set.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace second_set {

// The model's shape accessors: the word at +388, +268, +392 or +284 of shape
// `shape` (544 bytes each at [model + 40]). Model::StreamData
// (sub_82594A60), StreamSize (sub_82594A78), StreamData2 (sub_82594A90),
// StreamSize2 (sub_82594AA8).
uint32_t ModelStreamData(uint8_t* base, uint32_t model, uint32_t shape);
uint32_t ModelStreamSize(uint8_t* base, uint32_t model, uint32_t shape);
uint32_t ModelStreamData2(uint8_t* base, uint32_t model, uint32_t shape);
uint32_t ModelStreamSize2(uint8_t* base, uint32_t model, uint32_t shape);

// What the flush mode is left as: unchanged, or set (the vector work's) or
// cleared (the scalar work's) by the last step that ran.
enum class Flush { kUnchanged, kOn, kOff };

// Morph::SetUp (sub_82594C18): the record at `morph` from the base shape and
// up to 7 targets (`count`, 8 and above taken as 7; their shape indices at
// `indices`, weights at `weights`): +0 and +4 the base's streams
// ([shape + 400] + 16, [shape + 404] + 16), +116 = [[shape + 400]], +120 =
// [[shape + 404] + 4], +112 the count; for each target, +16 + 4i its first
// stream, +48 + 4i the base's second, +80 + 4i its weight (lfs, stfs). A
// target whose counts differ from the base's gets the original's debug
// message (DebugPrintf, which the retail game drops), logged. r3 is left the
// model.
Flush MorphSetUp(uint8_t* base, uint32_t model, uint32_t morph, uint32_t shape,
                 uint32_t indices, uint32_t weights, uint32_t count);

// Morph::Blend (sub_8258BD28) by the record's +120: 6 Morph::Blend6
// (sub_8258B578: the first stream copied, then each target's (first stream -
// the base's) x its weight added), 7 Morph::Blend7 (sub_8258B678: both
// streams, the second with the record's second streams at +48), else
// nothing (8, sub_8258B7E0, never ran: nullopt, nothing done, for the
// original). The frame's words as the originals leave them (the weights'
// vector at +80, Blend7's sizes at +80 and +84). `flush` what the flush mode
// is left as; r3 as the originals leave it (64 bits: Blend6's the frame's +80
// once a target's vertices ran, else its memcpy's; Blend7's the last
// target's second stream less its first once its vertices ran, else the
// second memcpy's; the record for other kinds). nullopt, with nothing done, where a memcpy would
// go to the original (Buffers_Memcpy refusing).
struct BlendResult {
  uint64_t r3;
  Flush flush;
};
std::optional<BlendResult> MorphBlend(uint8_t* base, uint32_t morph, uint32_t stack);
// Morph::Run (sub_82594D10): +8 and +12 = the two outputs, then Morph::Blend
// (a tail call, the same r1).
std::optional<BlendResult> MorphRun(uint8_t* base, uint32_t morph, uint32_t out1, uint32_t out2,
                                    uint32_t stack);

// sub_820E6F28: r3 = a tire group + 12 (12 entries of 24 bytes), `index`,
// the float at `value` and `f1`: the entry index - 12 (+0 the value, +4 f1,
// +8 f1 x [0x8200222C] x the value, +12 index x [0x82002D0C], +16 the
// fused step with [0x820021E8]), then each entry's +0 the value and, but for
// the entry index - 12, its +16, +12, +8 and +4 from the entry's index (+ 12)
// and the constants [0x82002D0C], [0x82001BE4], [0x82002A0C]. The
// frame's red-zone word at r1 - 16 as it leaves it.
void TireFillEntries(uint8_t* base, uint32_t entries, uint32_t index, uint32_t value, double f1,
                     uint32_t stack);
// sub_820E7010: from entry `index` - 12 of `entries`: a float4 at `out4`
// (index - 12 x [0x82002A88], the entry's +0 and +8 against [0x82006630] and
// [0x8200257C] times the double at 0x82006628, 0.0), the first again at
// `out1`, and (the entry's +0 - [0x82006630]) x [0x82001BE4] x [0x820029D8]
// at `out2`. The red-zone word at r1 - 16 as it leaves it.
void TireReadEntry(uint8_t* base, uint32_t index, uint32_t entries, uint32_t out4, uint32_t out1,
                   uint32_t out2, uint32_t stack);

}  // namespace second_set
