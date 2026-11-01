// The wheels' point set-up as plain C++ (docs/functions/second-set.md):
// WheelPoints::SetUp and the tire's, rims' and brakes' set-ups under it,
// which find their point records in the part models (ModelPoint::FindShape),
// copy the car model's matrix rows into their groups and, when the wheels
// record's +8 is 0, morph the tire, rim and brake shapes to the car's sizes
// (Morph::SetUp and Run into two page buffers, copied over the shape's
// streams with memcpy, the buffers freed). Guest data shared with the
// recompiled code, so these work on guest memory with the originals' loads
// and stores in their order (their frames' words included, with each
// frame's back chain; the saved registers' slots aren't written). `stack` is
// the guest r1 at entry; each returns r3 as the original leaves it. Built
// with -ffp-model=strict; the scalar float steps run with the flush mode
// off (the morph's vector steps turn it on themselves). Only with native
// memcpy, the native heap and --native_material_passes (the adapter's
// check). Adapter: src/hooks/second_set.cpp.

#pragma once

#include "second_set/morph.h"

#include <cstdint>
#include <functional>

namespace second_set {

// What the set-ups can't do as plain C++: the originals, from the frame at
// `sp` (the caller's r1) with the call's return address `lr`. Each leaves
// the flush mode off. And what they leave behind.
struct WheelCalls {
  // Morph::Run's original (r3 the model, r4 the record, r5 and r6 the
  // outputs) where its plain C++ declines; whether it left the flush mode
  // on.
  std::function<bool(uint32_t model, uint32_t record, uint32_t out1, uint32_t out2, uint32_t sp,
                     uint32_t lr)>
      morph_run;
  // memcpy's original where Buffers_Memcpy declines.
  std::function<void(uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr)>
      memcpy;
  // FreePagesIn's original (r3 = `at`) where its plain C++ declines; its r3.
  std::function<uint64_t(uint32_t at, uint32_t sp, uint32_t lr)> free_pages_in;
  // Out: the flush mode as the original leaves it (unchanged, or as its
  // last float step set it).
  Flush flush = Flush::kUnchanged;
};

// WheelPoints::SetUpTire (sub_820E7598): r3 = a tire group, r4 = the tire
// model: FindShape on its 6 records (+368); +304 to +320 = records 1 to 5's
// +228; by the front flag (+300) the wheels record's (+8) +44, +48 and +52
// (the rear's +56, +60, +64), or, when that's -1, the table at 0x82003EF8
// (24 bytes an entry by the car record's +0; +1080 on for kind 3) into
// sub_820E6F28 (+12); +1808 = 32 bytes of the matrix at +0 (the front's
// first, the rear's second); sub_820E7010 (the index from the record or the
// table at 0x820044B0; outputs +328, +352, +356); when the wheels record's
// +8 is 0, the base shape (+596) morphed toward the five (weights +328).
uint64_t WheelsSetUpTire(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls);
// WheelPoints::SetUpRimA (sub_820E79C8): FindShape on 7 records (+32);
// +1712 = 32 bytes of the matrix at +0 (the first when +12 isn't 0, else
// the second); when the wheels record's +8 is 0, record 0's shape morphed
// toward record 1's by +16 when record 0 found its model (+36), then record
// 2's toward 3's, its second copy with the first stream's size, as the
// original does.
uint64_t WheelsSetUpRimA(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls);
// WheelPoints::SetUpRimB (sub_820E7CA0): as SetUpRimA with three morphs
// (records 0 to 1, 2 to 3, 4 to 5), none checked, each copying both
// streams by their own sizes.
uint64_t WheelsSetUpRimB(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                         WheelCalls& calls);
// WheelPoints::SetUpBrakes (sub_820E8028): FindShape on 12 records (+32);
// PartModel::ScaleBlended by 1.0 on records 10 and 11; +2912 = 32 bytes of
// the matrix at +0 (by +12) and the wheels record's +108 (or +112);
// PartModel::SetTexture (the wheels record's +88, every shape); when the
// wheels record's +8 is 0, +16 = (that word - 9, +1 for kinds 3 to 5 at
// +68, +2 for 6) x [0x82006634], and six morphs (record 2k toward 2k + 1)
// by it.
uint64_t WheelsSetUpBrakes(uint8_t* base, uint32_t group, uint32_t model, uint32_t stack,
                           WheelCalls& calls);
// WheelPoints::SetUp (sub_820E8668): r3 = the block: the tires (+32,
// +1872), the rims of the scene's model set ([[0x826C2E44] + 4]: 0 set a's
// +3712 and +5456 with SetUpRimA, else set b's +7200 and +8944 with
// SetUpRimB) and the brakes (+10688, +13632), each with its model (+16,
// +20, +24; the rear's + 464), the car model's loading object's matrix
// (+6976 of [[+0] + 24]) or +8, and +12.
uint64_t WheelPointsSetUp(uint8_t* base, uint32_t block, uint32_t stack, WheelCalls& calls);

}  // namespace second_set
