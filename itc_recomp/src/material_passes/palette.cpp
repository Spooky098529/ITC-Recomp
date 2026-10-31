// PaintPalette::Init (sub_820D2330, see material_passes.h): the paint
// palette's 16 entries filled with the image's float constants. The
// original loops over the entries with a jump table on the index (0-15;
// its loop never reaches the table's trap), each case building its three
// colors in its frame and copying them into the entry by doublewords, then
// storing the three floats; a few constants it keeps in its frame or in
// registers across the loop. Written here as data: the same constants
// (each through lfs and stfs) at the same places, the frame left as the
// original leaves it. Nothing written overlaps anything read (the frame,
// the palette and .rdata), so the order of the writes doesn't change the
// result. Checked by the harness against the original (all 16 cases run on
// every call; the scene's +4 zero and not).

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

constexpr uint32_t kScene = 0x826C2E44;    // the scene's pointer
constexpr uint32_t kPalette = 0x826C5110;  // 16 entries of 64 bytes
constexpr uint32_t kFrameBytes = 1184;

// The float constants in .rdata the palette is built from, named by value.
constexpr uint32_t k0 = 0x82000A34;             // 0.0
constexpr uint32_t k0_0166669991 = 0x820035C8;  // 0.016667
constexpr uint32_t k0_0166699998 = 0x820035DC;  // 0.01667
constexpr uint32_t k0_0509800017 = 0x820035C0;  // 0.05098
constexpr uint32_t k0_09375 = 0x82003598;       // 0.09375
constexpr uint32_t k0_100000001 = 0x82001FBC;   // 0.1
constexpr uint32_t k0_125 = 0x82002A88;         // 0.125
constexpr uint32_t k0_159999996 = 0x820035D4;   // 0.16
constexpr uint32_t k0_160133004 = 0x820035D0;   // 0.160133
constexpr uint32_t k0_199986994 = 0x820035A4;   // 0.199987
constexpr uint32_t k0_200000003 = 0x82002208;   // 0.2
constexpr uint32_t k0_25 = 0x82002968;          // 0.25
constexpr uint32_t k0_300000012 = 0x8200296C;   // 0.3
constexpr uint32_t k0_375 = 0x820035B4;         // 0.375
constexpr uint32_t k0_495579988 = 0x820035C4;   // 0.49558
constexpr uint32_t k0_496679991 = 0x820035E8;   // 0.49668
constexpr uint32_t k0_5 = 0x82001BE4;           // 0.5
constexpr uint32_t k0_503125012 = 0x820035D8;   // 0.503125
constexpr uint32_t k0_546875 = 0x820035BC;      // 0.546875
constexpr uint32_t k0_578125 = 0x820035E0;      // 0.578125
constexpr uint32_t k0_600000024 = 0x82002A14;   // 0.6
constexpr uint32_t k0_640625 = 0x820035E4;      // 0.640625
constexpr uint32_t k0_671875 = 0x8200359C;      // 0.671875
constexpr uint32_t k0_703125 = 0x820035EC;      // 0.703125
constexpr uint32_t k0_800000012 = 0x82001070;   // 0.8
constexpr uint32_t k0_828125 = 0x820035B8;      // 0.828125
constexpr uint32_t k0_90625 = 0x820035CC;       // 0.90625
constexpr uint32_t k0_921875 = 0x820035F0;      // 0.921875
constexpr uint32_t k1 = 0x82000A30;             // 1.0
constexpr uint32_t k1_20000005 = 0x820029D4;    // 1.2
constexpr uint32_t k2 = 0x820021E8;             // 2.0
constexpr uint32_t k3 = 0x820021E4;             // 3.0
constexpr uint32_t k7 = 0x820035B0;             // 7.0
constexpr uint32_t k10 = 0x82001FD8;            // 10.0
constexpr uint32_t k18 = 0x820035A8;            // 18.0
constexpr uint32_t k20 = 0x82002568;            // 20.0
constexpr uint32_t k22_2999992 = 0x820035AC;    // 22.3
constexpr uint32_t k24 = 0x82002228;            // 24.0
constexpr uint32_t k54 = 0x82003590;            // 54.0
constexpr uint32_t k100 = 0x82002A0C;           // 100.0
constexpr uint32_t k105_000038 = 0x82003594;    // 105.00004
constexpr uint32_t k115 = 0x820035A0;           // 115.0

struct Entry {
  uint32_t colors[3][4];  // +0, +16, +32: (hue, saturation, value, 1.0)
  uint32_t floats[3];     // +48 to +56 (+60 isn't written)
  uint32_t built[3];      // where each color is built in the frame
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

// The frame's locals: constants kept across the loop (floats, through lfs
// and stfs) and the image's base 0x82000000 (lis values kept as words).
constexpr uint32_t kBase = 0x82000000;
struct Local {
  uint32_t offset;
  uint32_t constant;  // a float's address, or 0 for kBase
};
constexpr Local kLocals[] = {
    {16, k20},          {20, k7},          {28, k0_90625},       {32, k0_600000024},
    {36, k0_503125012}, {44, 0},           {48, 0},              {52, 0},
    {56, 0},            {60, 0},           {64, 0},              {68, k0_496679991},
    {72, 0},            {76, k0_921875},   {80, 0},              {84, k0_703125},
    {88, 0},            {92, k0_640625},   {96, 0},              {100, k100},
    {104, 0},           {108, k1_20000005},
};

void Move(uint8_t* base, uint32_t to, uint32_t from) { Stfs(base, to, Lfs(base, from)); }

}  // namespace

std::optional<uint64_t> PaintPaletteInit(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D2330 PaintPalette::Init");
  const uint32_t frame = sp - kFrameBytes;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  const uint32_t scene = guest::LoadU32(base, kScene);
  guest::StoreU32(base, frame + 24, scene);
  if (scene == 0) {
    return std::nullopt;
  }
  for (const Local& local : kLocals) {
    if (local.constant == 0) {
      guest::StoreU32(base, frame + local.offset, kBase);
    } else {
      Move(base, frame + local.offset, local.constant);
    }
  }
  const bool set = int32_t(guest::LoadU32(base, scene + 4)) != 0;
  for (uint32_t e = 0; e < 16; e++) {
    const Entry& entry = kEntries[e];
    const uint32_t at = kPalette + 64 * e;
    for (uint32_t c = 0; c < 3; c++) {
      const uint32_t built = frame + entry.built[c];
      for (uint32_t i = 0; i < 4; i++) {
        Move(base, built + 4 * i, entry.colors[c][i]);
      }
      // Copied into the entry by doublewords (ld, std): the same bits.
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(base, at + 16 * c + 4 * i, guest::LoadU32(base, built + 4 * i));
      }
    }
    for (uint32_t i = 0; i < 3; i++) {
      uint32_t constant = entry.floats[i];
      for (const SceneSet& s : kSceneSet) {
        if (set && s.entry == e && s.index == i) {
          constant = s.constant;
        }
      }
      Move(base, at + 48 + 4 * i, constant);
    }
  }
  guest::StoreU32(base, frame + 40, 16);  // the loop's index, as it ends
  // r3 as left: the last case's ld of its third color's first doubleword.
  return (uint64_t(guest::LoadU32(base, frame + 864)) << 32) | guest::LoadU32(base, frame + 868);
}

}  // namespace material_passes
