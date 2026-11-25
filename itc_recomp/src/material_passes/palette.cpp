// PaintPalette::Init (sub_820D2330, see material_passes.h): the paint
// palette's 16 entries filled with the image's float constants. The
// original loops over the entries with a jump table on the index (0-15;
// its loop never reaches the table's trap), each case building its three
// colors in its frame and copying them into the entry by doublewords, then
// storing the three floats; a few constants it keeps in its frame or in
// registers across the loop. Written here as data: the same constants
// (each through lfs and stfs) at the same places in the palette; the
// frame's colors and locals, which only it reads, left out (E5 step 6: each
// color's words stored straight into the entry, the same bits as the copy
// by doublewords). Nothing written overlaps anything read (the palette and
// .rdata), so the order of the writes doesn't change the result. Checked by the harness against the original (all 16 cases run on
// every call; the scene's +4 zero and not).

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/car_draw.h"
#include "schema/material_passes.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

using G = schema::MaterialPassesGlobals;

// The float constants in .rdata the palette is built from, named by value.
constexpr uint32_t k0 = G::zero_address();             // 0.0
constexpr uint32_t k0_0166669991 = G::value_0_016667_address();  // 0.016667
constexpr uint32_t k0_0166699998 = G::value_0_01667_address();  // 0.01667
constexpr uint32_t k0_0509800017 = G::value_0_05098_address();  // 0.05098
constexpr uint32_t k0_09375 = G::value_0_09375_address();       // 0.09375
constexpr uint32_t k0_100000001 = G::value_0_1_address();   // 0.1
constexpr uint32_t k0_125 = G::value_0_125_address();         // 0.125
constexpr uint32_t k0_159999996 = G::value_0_16_address();   // 0.16
constexpr uint32_t k0_160133004 = G::value_0_160133_address();   // 0.160133
constexpr uint32_t k0_199986994 = G::value_0_199987_address();   // 0.199987
constexpr uint32_t k0_200000003 = G::value_0_2_address();   // 0.2
constexpr uint32_t k0_25 = G::value_0_25_address();          // 0.25
constexpr uint32_t k0_300000012 = G::value_0_3_address();   // 0.3
constexpr uint32_t k0_375 = G::value_0_375_address();         // 0.375
constexpr uint32_t k0_495579988 = G::value_0_49558_address();   // 0.49558
constexpr uint32_t k0_496679991 = G::value_0_49668_address();   // 0.49668
constexpr uint32_t k0_5 = G::half_address();           // 0.5
constexpr uint32_t k0_503125012 = G::value_0_503125_address();   // 0.503125
constexpr uint32_t k0_546875 = G::value_0_546875_address();      // 0.546875
constexpr uint32_t k0_578125 = G::value_0_578125_address();      // 0.578125
constexpr uint32_t k0_600000024 = G::value_0_6_address();   // 0.6
constexpr uint32_t k0_640625 = G::value_0_640625_address();      // 0.640625
constexpr uint32_t k0_671875 = G::value_0_671875_address();      // 0.671875
constexpr uint32_t k0_703125 = G::value_0_703125_address();      // 0.703125
constexpr uint32_t k0_800000012 = G::value_0_8_address();   // 0.8
constexpr uint32_t k0_828125 = G::value_0_828125_address();      // 0.828125
constexpr uint32_t k0_90625 = G::value_0_90625_address();       // 0.90625
constexpr uint32_t k0_921875 = G::value_0_921875_address();      // 0.921875
constexpr uint32_t k1 = G::one_address();             // 1.0
constexpr uint32_t k1_20000005 = G::value_1_2_address();    // 1.2
constexpr uint32_t k2 = G::value_2_address();             // 2.0
constexpr uint32_t k3 = G::value_3_address();             // 3.0
constexpr uint32_t k7 = G::value_7_address();             // 7.0
constexpr uint32_t k10 = G::value_10_address();            // 10.0
constexpr uint32_t k18 = G::value_18_address();            // 18.0
constexpr uint32_t k20 = G::value_20_address();            // 20.0
constexpr uint32_t k22_2999992 = G::value_22_3_address();    // 22.3
constexpr uint32_t k24 = G::value_24_address();            // 24.0
constexpr uint32_t k54 = G::value_54_address();            // 54.0
constexpr uint32_t k100 = G::value_100_address();           // 100.0
constexpr uint32_t k105_000038 = G::value_105_address();    // 105.00004
constexpr uint32_t k115 = G::value_115_address();           // 115.0

struct Entry {
  uint32_t colors[3][4];  // +0, +16, +32: (hue, saturation, value, 1.0)
  uint32_t floats[3];     // +48 to +56 (+60 isn't written)
  uint32_t built[3];      // where the original builds each color in its frame
};

// With the scene's +4 zero; kSceneSet below for the words that change.
constexpr Entry kEntries[16] = {
    {{{k1, k0_0166669991, k0, k1}, {k1, k0_0166669991, k0, k1}, {k0_200000003, k0_160133004, k0_159999996, k1}}, {k0_5, k0_5, k0_5}, {304, 144, 688}},
    {{{k0, k0, k0_5, k1}, {k0, k0, k0_5, k1}, {k0, k0, k0, k1}}, {k2, k1, k0_496679991}, {176, 528, 208}},
    {{{k1, k0_0166699998, k0, k1}, {k1, k0_0166699998, k0, k1}, {k0_200000003, k0_160133004, k0_159999996, k1}}, {k0_300000012, k0_5, k0_5}, {784, 240, 560}},
    {{{k0, k0, k0_921875, k1}, {k0, k0, k0, k1}, {k0, k0, k0, k1}}, {k0, k0, k0_703125}, {272, 720, 112}},
    {{{k0, k0, k1, k1}, {k0, k0, k0, k1}, {k0, k0, k0, k1}}, {k0, k0_578125, k0_640625}, {592, 336, 848}},
    {{{k0, k0, k1, k1}, {k0, k0, k0_100000001, k1}, {k0_200000003, k0_160133004, k0_159999996, k1}}, {k22_2999992, k3, k1}, {368, 624, 400}},
    {{{k0, k0, k1, k1}, {k0, k0, k0, k1}, {k0, k0, k0, k1}}, {k0, k0, k0_800000012}, {752, 432, 656}},
    {{{k1, k0_0166669991, k0, k1}, {k1, k0_0166669991, k0, k1}, {k0_200000003, k0_160133004, k0_159999996, k1}}, {k0_300000012, k0_5, k0_5}, {464, 816, 496}},
    {{{k1, k0_0166669991, k0, k1}, {k1, k0_0166669991, k0, k1}, {k0_200000003, k0_160133004, k0_159999996, k1}}, {k0_300000012, k0_5, k0_5}, {128, 160, 192}},
    {{{k0, k0, k0_100000001, k1}, {k0, k0, k1, k1}, {k0, k0, k0, k1}}, {k100, k0, k0_800000012}, {224, 256, 288}},
    {{{k0, k0, k0_375, k1}, {k0, k0, k0_546875, k1}, {k0, k0, k0, k1}}, {k24, k0_578125, k0_800000012}, {320, 352, 384}},
    {{{k0, k0, k1, k1}, {k0, k0, k0_0509800017, k1}, {k0, k0, k0, k1}}, {k2, k3, k0_495579988}, {416, 448, 480}},
    {{{k0, k0, k1, k1}, {k0, k0, k0_0509800017, k1}, {k0, k0, k0, k1}}, {k54, k0_25, k0_495579988}, {512, 544, 576}},
    {{{k105_000038, k0_09375, k0_671875, k1}, {k115, k0_125, k0_25, k1}, {k0_199986994, k0_160133004, k0_159999996, k1}}, {k18, k20, k0_800000012}, {608, 640, 672}},
    {{{k0, k0, k0_828125, k1}, {k0, k0, k0_5, k1}, {k0, k0, k0, k1}}, {k10, k0_90625, k0_503125012}, {704, 736, 768}},
    {{{k1, k0_0166699998, k0, k1}, {k1, k0_0166699998, k0, k1}, {k0_200000003, k0_160133004, k0_159999996, k1}}, {k0_300000012, k0_5, k0_5}, {800, 832, 864}},
};

// The floats that take another constant when the scene's +4 isn't zero.
struct SceneSet {
  uint32_t entry;
  uint32_t index;  // into Entry::floats
  uint32_t constant;
};
constexpr SceneSet kSceneSet[3] = {
    {11, 1, k1_20000005},  // else 3.0
    {13, 1, k7},           // else 20.0
    {14, 2, k0_600000024},  // else 0.503125
};

void Move(uint32_t to, uint32_t from) { Stfs(to, Lfs(from)); }

}  // namespace

std::optional<uint64_t> PaintPaletteInit(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D2330 PaintPalette::Init");
  // The original's 1,184-byte frame left out (E5 step 6): the scene (+24),
  // its constants and the image's base (+16 to +108), the colors built
  // (Entry::built) and the loop's index (+40) were read by it alone.
  (void)sp;
  const uint32_t scene = schema::CarDrawGlobals::scene().address;
  if (scene == 0) {
    return std::nullopt;
  }
  const bool set = int32_t(schema::CarScene(scene).model_set()) != 0;
  for (uint32_t e = 0; e < 16; e++) {
    const Entry& entry = kEntries[e];
    const schema::PaintPaletteEntry at(G::paint_palette_address() + 64 * e);
    for (uint32_t c = 0; c < 3; c++) {
      for (uint32_t i = 0; i < 4; i++) {
        Move(at.color_a_address() + 16 * c + 4 * i, entry.colors[c][i]);
      }
    }
    for (uint32_t i = 0; i < 3; i++) {
      uint32_t constant = entry.floats[i];
      for (const SceneSet& s : kSceneSet) {
        if (set && s.entry == e && s.index == i) {
          constant = s.constant;
        }
      }
      Move(at.float_0_address() + 4 * i, constant);
    }
  }
  // r3 as left: the last case's ld of its third color's first doubleword
  // (entry 15's +32, the same bits as the frame's copy at +864).
  const schema::PaintPaletteEntry last(G::paint_palette_address() + 64 * 15);
  return (uint64_t(guest::LoadU32(last.color_a_address() + 32)) << 32) |
         guest::LoadU32(last.color_a_address() + 36);
}

}  // namespace material_passes
