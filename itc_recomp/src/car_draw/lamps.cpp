// The head and tail lights with their camera-facing glows as plain C++
// (docs/functions/car-draw.md: PartsPoints::DrawTailLights and
// DrawHeadlights); see car_draw.h. Built with -ffp-model=strict
// (CMakeLists.txt): the scalar floats as the translated code's doubles
// rounded to single (fused where the original fuses), the one vector add
// with flush-to-zero on as the translation turns it on; the matrix work is
// matrix_math's, noted for frame interpolation as the wrappers note the
// originals' calls.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "material_passes/material_passes.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"

#include <cmath>
#include <initializer_list>

namespace car_draw {

using namespace camera_modes::steps;

// The identity's four rows (vupkd3d of zeros and its shuffles), stored as
// stvx stores them.
void StoreIdentity(uint8_t* base, uint32_t at) {
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 4; row++) {
    Stvx(base, at + 16 * row,
         VectorOf(row == 0 ? kOneBits : 0, row == 1 ? kOneBits : 0, row == 2 ? kOneBits : 0,
                  row == 3 ? kOneBits : 0));
  }
}

// `bytes` from `from` to `to`, doubleword by doubleword in order (ld, std).
void CopyDoublewords(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 8) {
    const uint32_t hi = guest::LoadU32(base, from + i);
    const uint32_t lo = guest::LoadU32(base, from + i + 4);
    guest::StoreU32(base, to + i, hi);
    guest::StoreU32(base, to + i + 4, lo);
  }
}

namespace {

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kCullFlip = 0x826C5104;
constexpr uint32_t kOne = 0x82000A30;    // 1.0
constexpr uint32_t kZero = 0x82000A34;   // 0.0
constexpr uint32_t kHalf = 0x82001BE4;   // 0.5
constexpr uint32_t kTwo = 0x820021E8;    // 2.0
constexpr uint32_t kTransposeRotation = 0x820D1CB0;
constexpr uint32_t kSetMaterials = 0x820D3318;
constexpr uint32_t kProjectorMatrices = 0x820D1EB8;
constexpr uint32_t kModelDraw = 0x820D04E8;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// The block's flag bits (the low word of its +0 doubleword, ld'd each time).
uint32_t Bits(uint8_t* base, uint32_t block) {
  return Load(base, block + 4);
}

// [0x826C5104] = !x (cntlzw and the shift): Model::RecordMaterials' cull
// swap flipped around the lamps' glows.
void Flip(uint8_t* base) {
  Store(base, kCullFlip, Load(base, kCullFlip) == 0 ? 1 : 0);
}

// 1.0 (lfs'd once) into each of the frame's words at `offsets`.
void Ones(uint8_t* base, uint32_t frame, std::initializer_list<uint32_t> offsets) {
  const double one = Lfs(base, kOne);
  for (uint32_t at : offsets) {
    Stfs(base, frame + at, one);
  }
}

simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
}

void Identity(uint8_t* base, uint32_t at) {
  StoreIdentity(base, at);
}
void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes) {
  CopyDoublewords(base, to, from, bytes);
}

// An identity built at `at` and copied to `to` (the original's fresh
// matrices).
void FreshIdentity(uint8_t* base, uint32_t to, uint32_t at) {
  Identity(base, at);
  Copy(base, to, at, 64);
}

// Matrix::Multiply and Matrix::Copy, noted as their wrappers note the
// originals' calls returning to `lr`; r3 their out.
Left Multiply(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t lr) {
  matrix_math::Multiply(base, out, a, b);
  interpolation::BlendState_OnMultiply(base, out, a, b, lr);
  return Left{false, out};
}
Left CopyMatrix(uint8_t* base, uint32_t to, uint32_t from, uint32_t lr) {
  matrix_math::Copy(base, to, from);
  interpolation::BlendState_OnCopy(base, to, from, lr);
  return Left{false, to};
}

// memcpy (sub_825A7410) from the frame at `sp` returning to `lr`: natively,
// else its original.
void Memcpy(uint8_t* base, const Calls& calls, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t sp, uint32_t lr) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
}

// PartModel::SetPair1002Alpha(the record's model, its shape, the block's
// +636) from `frame`.
Left CoverAlpha(uint8_t* base, uint32_t record, uint32_t block, uint32_t frame) {
  const double f1 = Lfs(base, block + 636);
  const uint32_t shape = Load(base, record + 228);
  const uint32_t model = Load(base, record + 4);
  return Left{false, material_passes::SetPair1002Alpha(base, model, shape, f1, frame)};
}

// Matrix::TransposeRotation(frame + 656, the pass's view): its plain C++
// (r3 its out, left as the call set it), else its original.
Left ViewTransposed(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t pass,
                    uint32_t lr) {
  const uint32_t out = frame + 656, view = pass + 16;
  if (material_passes::TransposeRotation(base, out, view, frame)) {
    return Left{false, out};
  }
  const uint64_t regs[2] = {out, view};
  calls.call(kTransposeRotation, regs, 2, frame, lr);
  return Left{true, 0};
}

// The glow model's color: the -1 shape, category and shared pair (+148,
// +140, +132), the 32 bytes at +304 copied to +96 (memcpy), then
// PartModel::SetMaterials(model, the flags at `flags`, the color at +272).
Left GlowColor(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t model,
               uint32_t flags, uint32_t memcpy_lr, uint32_t lr) {
  Store(base, frame + 148, 0xFFFFFFFFu);
  Store(base, frame + 140, 0xFFFFFFFFu);
  Store(base, frame + 132, 0xFFFFFFFFu);
  Memcpy(base, calls, frame + 96, frame + 304, 32, frame, memcpy_lr);
  const uint64_t regs[7] = {Load64(base, frame + flags),      Load64(base, frame + flags + 8),
                            Load64(base, frame + flags + 16), Load64(base, frame + 272),
                            Load64(base, frame + 280),        Load64(base, frame + 288),
                            Load64(base, frame + 296)};
  std::optional<uint64_t> r3;
  if (material_passes::SetMaterials(base, model, regs, frame, &r3)) {
    return Left{false, r3 ? *r3 : model};
  }
  const uint64_t all[8] = {model,   regs[0], regs[1], regs[2],
                           regs[3], regs[4], regs[5], regs[6]};
  calls.call(kSetMaterials, all, 8, frame, lr);
  return Left{true, 0};
}

// Projector::Matrices(pass, frame + 1360 (the world's rotation), `scale`,
// frame + 1296, frame + 448 (the node's position), 1, `side`, 1): its
// plain C++, else its original. The answer (0: facing away) and r3.
uint32_t Project(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t pass, uint32_t scale,
                 uint32_t side, uint32_t lr, Left& left) {
  const material_passes::ProjectorArgs args{pass,        frame + 1360, scale,
                                            frame + 1296, frame + 448,  side};
  if (const auto r3 = material_passes::ProjectorMatrices(base, args, frame)) {
    left = Left{false, *r3};
    return *r3;
  }
  const uint64_t regs[8] = {pass, frame + 1360, scale, frame + 1296, frame + 448, 1, side, 1};
  const uint64_t r3 = calls.call(kProjectorMatrices, regs, 8, frame, lr);
  left = Left{true, 0};
  return uint32_t(r3);
}

// The unit vector from +384 (the matrix at +336's last row) to the pass's
// eye (+400) at +160 (+172 1.0), an identity at +272 copied to +1232, the
// 1.0 w's at `ones`, +448 += +160 (vaddfp) and +160's 16 bytes copied to
// +1280 (+460 1.0 between): a translation the original builds and then
// draws nothing with.
void TowardEye(uint8_t* base, uint32_t frame, uint32_t pass,
               std::initializer_list<uint32_t> ones) {
  const double one = Lfs(base, kOne);
  double f12 = Lfs(base, frame + 392);
  double f0 = Fsubs(Lfs(base, pass + 408), f12);
  f12 = Lfs(base, frame + 388);
  double f13 = Fsubs(Lfs(base, pass + 404), f12);
  const double f11 = Lfs(base, frame + 384);
  f12 = Fsubs(Lfs(base, pass + 400), f11);
  Stfs(base, frame + 172, one);
  Identity(base, frame + 272);
  double length = Fmuls(f0, f0);
  length = Fmadds(f13, f13, length);
  length = Fmadds(f12, f12, length);
  length = Single(std::sqrt(length));
  const double inverse = Fdivs(one, length);
  Stfs(base, frame + 160, Fmuls(f12, inverse));
  Stfs(base, frame + 164, Fmuls(f13, inverse));
  Stfs(base, frame + 168, Fmuls(f0, inverse));
  Copy(base, frame + 1232, frame + 272, 64);
  Ones(base, frame, ones);
  {
    FlushToZero ftz;
    Stvx(base, frame + 448, simde_mm_add_ps(Lvx(base, frame + 448), Lvx(base, frame + 160)));
  }
  const uint32_t w0 = Load(base, frame + 160), w1 = Load(base, frame + 164);
  const uint32_t w2 = Load(base, frame + 168), w3 = Load(base, frame + 172);
  Stfs(base, frame + 460, one);
  Store(base, frame + 1280, w0);
  Store(base, frame + 1284, w1);
  Store(base, frame + 1288, w2);
  Store(base, frame + 1292, w3);
}

// Model::Draw(the glow model, the pass's view-projection, frame + 336,
// shape, 0) from `frame`, r24 = `r24` for its recorder's context: its plain
// C++, else its original.
Left GlowDraw(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t model,
              uint32_t view_projection, uint32_t shape, uint64_t r24, uint32_t lr) {
  const auto draw = ModelDraw(base, calls, frame, model, view_projection, frame + 336, shape, 0,
                              uint32_t(r24));
  if (!draw) {
    const uint64_t regs[5] = {model, view_projection, frame + 336, shape, 0};
    calls.call_r24(kModelDraw, regs, 5, frame, lr, r24);
    return Left{true, 0};
  }
  if (draw->queued) {
    return Left{true, 0};
  }
  return Left{false, draw->r3};
}

}  // namespace

// --- The tail lights ---

namespace {

// The tail lights' glows' call sites (the parts' and the traffic's), in
// order.
struct TailSites {
  uint32_t view, color_memcpy, color, multiply1, copy1, multiply2, copy2, copy_world,
      memcpy_world, project, multiply3, multiply4, copy3, multiply5, multiply6, copy4, fade_draw,
      draw;
};

constexpr TailSites kPartsTailSites = {
    0x820DFA28, 0x820DFB34, 0x820DFB58, 0x820DFC0C, 0x820DFC18, 0x820DFC50,
    0x820DFC5C, 0x820DFD70, 0x820DFDA8, 0x820DFDCC, 0x820DFED8, 0x820DFEF8,
    0x820DFF04, 0x820DFF24, 0x820DFF44, 0x820DFF50, 0x820DFF9C, 0x820DFFCC};
constexpr TailSites kTrafficTailSites = {
    0x820E5BF8, 0x820E5CF8, 0x820E5D1C, 0x820E5DD0, 0x820E5DDC, 0x820E5E14,
    0x820E5E20, 0x820E5F34, 0x820E5F6C, 0x820E5F90, 0x820E609C, 0x820E60BC,
    0x820E60C8, 0x820E60E8, 0x820E6108, 0x820E6114, 0x820E6160, 0x820E6190};

// What both tail lights' draws do after their lamps (the same code at the
// same frame offsets): the view's transposed rotation, the red color, then
// the glows at nodes 0 to 3 with [0x826C5104] flipped around them. They draw
// glows with bit 0x1 and not 0x20 of the block's bits, the parts' also with
// 0x40 (`any_with_0x40`).
Left TailGlows(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t group, uint32_t block,
               uint32_t pass, bool any_with_0x40, const TailSites& sites) {
  const uint32_t vp = pass + 144, world = block + 16;
  Left left;
  // The 1.0 w's of the view's transposed rotation and of the matrix at +336.
  const double one = Lfs(base, kOne);
  for (uint32_t at : {668u, 684u, 700u, 716u, 348u}) {
    Stfs(base, frame + at, one);
  }
  Flip(base);
  for (uint32_t at : {364u, 380u, 396u, 412u, 428u, 444u, 460u}) {
    Stfs(base, frame + at, one);
  }
  left = ViewTransposed(base, calls, frame, pass, sites.view);
  FreshIdentity(base, frame + 400, frame + 208);
  // The color (1, 0, 0, 1) at +160 (copied to +272) and SetMaterials'
  // flags at +176 (the first field only).
  Stfs(base, frame + 160, one);
  Stfs(base, frame + 172, one);
  Stfs(base, frame + 284, one);
  Stfs(base, frame + 300, one);
  const double zero = Lfs(base, kZero);
  Stfs(base, frame + 164, zero);
  Stfs(base, frame + 168, zero);
  Stfs(base, frame + 316, one);
  Store(base, frame + 196, 0);
  Store(base, frame + 192, 0);
  Store(base, frame + 176, 1);
  const uint32_t c0 = Load(base, frame + 160), c1 = Load(base, frame + 164);
  const uint32_t c2 = Load(base, frame + 168), c3 = Load(base, frame + 172);
  Store(base, frame + 188, 0);
  Store(base, frame + 184, 0);
  Store(base, frame + 180, 0);
  Store(base, frame + 272, c0);
  Store(base, frame + 276, c1);
  Store(base, frame + 280, c2);
  Store(base, frame + 284, c3);
  const uint32_t bits = Bits(base, block);
  if ((!any_with_0x40 || (bits & 0x40) == 0) && ((bits & 0x1) == 0 || (bits & 0x20) != 0)) {
    Flip(base);
    return left;
  }

  // The glows: the scene's glow model in red at the four tl**_0k_ef nodes.
  const uint32_t model = Load(base, kScene) + 480;
  left = GlowColor(base, calls, frame, model, 176, sites.color_memcpy, sites.color);
  const int32_t shapes = int32_t(Load(base, model + 412));
  const double two = Lfs(base, kTwo);
  const double half = Lfs(base, kHalf);
  for (uint32_t node = 0; node < 4; node++) {
    const uint32_t record = group + 240 * node;
    if (Load(base, record + 4) == 0) {
      continue;
    }
    // The node's position through the block's world, at +384 and +448.
    FreshIdentity(base, frame + 336, frame + 208);
    const uint64_t p0 = Load64(base, record + 144), p1 = Load64(base, record + 152);
    Stfs(base, frame + 732, one);
    Stfs(base, frame + 748, one);
    Stfs(base, frame + 764, one);
    Store(base, frame + 384, uint32_t(p0 >> 32));
    Store(base, frame + 388, uint32_t(p0));
    Stfs(base, frame + 780, one);
    Store(base, frame + 392, uint32_t(p1 >> 32));
    Store(base, frame + 396, uint32_t(p1));
    Multiply(base, frame + 720, frame + 336, world, sites.multiply1);
    CopyMatrix(base, frame + 336, frame + 720, sites.copy1);
    Ones(base, frame, {988, 1004, 1020, 1036});
    Copy(base, frame + 448, frame + 384, 16);
    Multiply(base, frame + 976, frame + 656, frame + 400, sites.multiply2);
    CopyMatrix(base, frame + 336, frame + 976, sites.copy2);
    // The scale (+848), the projector's view (+1296) and the world's
    // rotation (+1168, its last row (0, 0, 0, 1), copied to +1360).
    FreshIdentity(base, frame + 848, frame + 528);
    FreshIdentity(base, frame + 1296, frame + 464);
    FreshIdentity(base, frame + 1168, frame + 592);
    left = CopyMatrix(base, frame + 1168, world, sites.copy_world);
    Stfs(base, frame + 176, zero);
    Stfs(base, frame + 180, zero);
    Stfs(base, frame + 184, zero);
    Stfs(base, frame + 188, one);
    Copy(base, frame + 1216, frame + 176, 16);
    Memcpy(base, calls, frame + 1360, frame + 1168, 64, frame, sites.memcpy_world);
    if (Project(base, calls, frame, pass, frame + 848, 0, sites.project, left) == 0) {
      continue;
    }
    TowardEye(base, frame, pass, {924, 940, 956, 972});
    Multiply(base, frame + 912, frame + 848, frame + 336, sites.multiply3);
    Ones(base, frame, {1116, 1132, 1148, 1164});
    Multiply(base, frame + 1104, frame + 912, frame + 1232, sites.multiply4);
    CopyMatrix(base, frame + 336, frame + 1104, sites.copy3);
    // The world: the scale x the view's transposed rotation x the
    // translation at +400.
    Ones(base, frame, {1052, 1068, 1084, 1100});
    Multiply(base, frame + 1040, frame + 848, frame + 656, sites.multiply5);
    Ones(base, frame, {796, 812, 828, 844});
    Multiply(base, frame + 784, frame + 1040, frame + 400, sites.multiply6);
    left = CopyMatrix(base, frame + 336, frame + 784, sites.copy4);
    // Small glows fade: each shape's alpha |s d| x 2.0 first (NaN draws
    // plainly, as the original's bge).
    if (std::fabs(Lfs(base, frame + 848)) < half) {
      for (int32_t shape = 0; shape < shapes; shape++) {
        const double alpha = Fmuls(std::fabs(Lfs(base, frame + 848)), two);
        left = Left{false, material_passes::SetAlpha(base, model, uint32_t(shape), alpha, frame)};
        left = GlowDraw(base, calls, frame, model, vp, uint32_t(shape), 0, sites.fade_draw);
      }
    } else {
      for (int32_t shape = 0; shape < shapes; shape++) {
        left = GlowDraw(base, calls, frame, model, vp, uint32_t(shape), 0, sites.draw);
      }
    }
  }
  Flip(base);
  return left;
}

}  // namespace

Left PartsDrawTailLights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                         uint32_t block, uint32_t pass, uint32_t caller_r24,
                         uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820DF8B0 PartsPoints::DrawTailLights");
  const uint32_t frame = Frame(base, stack, kPartsTailLightsFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  auto lamp = [&](uint32_t record, uint32_t lr) {
    return PartsDrawRecord(base, calls, frame, group + record, vp, world, 0, caller_r24,
                           caller_r28, lr);
  };
  // The lamps: on, brake, reversing, the winkers; the cover.
  if (Bits(base, block) & 0x1) {
    lamp(1920, 0x820DF900);
  }
  if (Bits(base, block) & 0x8) {
    lamp(2160, 0x820DF928);
  }
  if (Bits(base, block) & 0x10) {
    lamp(2400, 0x820DF950);
  }
  if (Bits(base, block) & 0x2) {
    lamp(2640, 0x820DF978);
  }
  if (Bits(base, block) & 0x4) {
    lamp(2880, 0x820DF9A0);
  }
  CoverAlpha(base, group + 3120, block, frame);
  lamp(3120, 0x820DF9D0);
  return TailGlows(base, calls, frame, group, block, pass, true, kPartsTailSites);
}

Left TrafficDrawTailLights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24,
                           uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E5B08 TrafficPoints::DrawTailLights");
  const uint32_t frame = Frame(base, stack, kTrafficTailLightsFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  auto lamp = [&](uint32_t record, uint32_t lr) {
    return PartsDrawRecord(base, calls, frame, group + record, vp, world, 0, caller_r24,
                           caller_r28, lr);
  };
  // The lamps: on, brake; the cover (no alpha call).
  if (Bits(base, block) & 0x1) {
    lamp(2160, 0x820E5B58);
  }
  if (Bits(base, block) & 0x8) {
    lamp(2400, 0x820E5B80);
  }
  lamp(2640, 0x820E5BA0);
  return TailGlows(base, calls, frame, group, block, pass, false, kTrafficTailSites);
}

// --- The headlights ---

Left PartsDrawHeadlights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                         uint32_t block, uint32_t pass, uint64_t index, uint64_t sub,
                         uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E1690 PartsPoints::DrawHeadlights");
  const uint32_t frame = Frame(base, stack, kPartsHeadlightsFrame);
  Left left;
  left.r3 = group;
  if (Load(base, group + 4) == 0) {
    return left;
  }
  // The lamps with the pop-up's matrix (+3120): low, high, the winkers; the
  // cover. r28 is the pass here.
  const uint32_t vp = pass + 144, popup = group + 3120;
  auto lamp = [&](uint32_t record, uint32_t lr) {
    return PartsDrawRecord(base, calls, frame, group + record, vp, popup, 0, caller_r24, pass,
                           lr);
  };
  if (Bits(base, block) & 0x1) {
    lamp(1680, 0x820E16F4);
  }
  if (Bits(base, block) & 0x40) {
    lamp(1920, 0x820E171C);
  }
  if (Bits(base, block) & 0x2) {
    lamp(2160, 0x820E1744);
  }
  if (Bits(base, block) & 0x4) {
    lamp(2400, 0x820E176C);
  }
  CoverAlpha(base, group + 2640, block, frame);
  lamp(2640, 0x820E1798);

  const double one = Lfs(base, kOne);
  for (uint32_t at : {668u, 684u, 700u, 716u, 348u}) {
    Stfs(base, frame + at, one);
  }
  Flip(base);
  for (uint32_t at : {364u, 380u, 396u, 412u, 428u, 444u, 460u}) {
    Stfs(base, frame + at, one);
  }
  ViewTransposed(base, calls, frame, pass, 0x820E17F0);
  FreshIdentity(base, frame + 400, frame + 192);
  // SetMaterials' flags at +160 (the first field only) and the color from
  // the table at +256, copied to +272.
  Stfs(base, frame + 284, one);
  Stfs(base, frame + 300, one);
  Stfs(base, frame + 316, one);
  for (uint32_t at : {180u, 176u, 172u, 168u, 164u}) {
    Store(base, frame + at, 0);
  }
  material_passes::TableVector(base, frame + 256, uint32_t(index), uint32_t(sub));
  left = Left{false, frame + 256};
  const uint32_t bits = Bits(base, block);
  const uint64_t c1 = Load64(base, frame + 256 + 8);
  const uint64_t c0 = Load64(base, frame + 256);
  Store(base, frame + 160, 1);
  Store(base, frame + 272, uint32_t(c0 >> 32));
  Store(base, frame + 276, uint32_t(c0));
  Store(base, frame + 280, uint32_t(c1 >> 32));
  Store(base, frame + 284, uint32_t(c1));
  if ((bits & 0x40) == 0 && ((bits & 0x1) == 0 || (bits & 0x20) != 0)) {
    Flip(base);
    return left;
  }

  // The glows: the low beams' nodes (records 1 to 4), with the high beams
  // also 5 and 6, each facing the camera; the glows follow the block's
  // world, not the pop-up.
  const uint32_t model = Load(base, kScene) + 480;
  left = GlowColor(base, calls, frame, model, 160, 0x820E18F0, 0x820E1914);
  const int32_t shapes = int32_t(Load(base, model + 412));
  const double zero = Lfs(base, kZero);
  const uint32_t world = block + 16;
  for (uint32_t node = 1; node < 7; node++) {
    const uint32_t record = group + 240 * node;
    if (Load(base, record + 4) == 0) {
      continue;
    }
    if (node >= 5 && (Bits(base, block) & 0x40) == 0) {
      continue;
    }
    FreshIdentity(base, frame + 336, frame + 192);
    const uint64_t p0 = Load64(base, record + 144), p1 = Load64(base, record + 152);
    Stfs(base, frame + 732, one);
    Stfs(base, frame + 748, one);
    Stfs(base, frame + 764, one);
    Store(base, frame + 384, uint32_t(p0 >> 32));
    Store(base, frame + 388, uint32_t(p0));
    Stfs(base, frame + 780, one);
    Store(base, frame + 392, uint32_t(p1 >> 32));
    Store(base, frame + 396, uint32_t(p1));
    Multiply(base, frame + 720, frame + 336, world, 0x820E19D8);
    CopyMatrix(base, frame + 336, frame + 720, 0x820E19E4);
    Ones(base, frame, {1052, 1068, 1084, 1100});
    Copy(base, frame + 448, frame + 384, 16);
    Multiply(base, frame + 1040, frame + 656, frame + 400, 0x820E1A1C);
    CopyMatrix(base, frame + 336, frame + 1040, 0x820E1A28);
    // The scale (+1168), the projector's view (+1296) and the world's
    // rotation (+1104, its last row (0, 0, 0, 1), copied to +1360).
    FreshIdentity(base, frame + 1168, frame + 464);
    FreshIdentity(base, frame + 1296, frame + 528);
    FreshIdentity(base, frame + 1104, frame + 592);
    left = CopyMatrix(base, frame + 1104, world, 0x820E1B3C);
    Stfs(base, frame + 256, zero);
    Stfs(base, frame + 260, zero);
    Stfs(base, frame + 264, zero);
    Stfs(base, frame + 268, one);
    Copy(base, frame + 1152, frame + 256, 16);
    Memcpy(base, calls, frame + 1360, frame + 1104, 64, frame, 0x820E1B74);
    if (Project(base, calls, frame, pass, frame + 1168, 1, 0x820E1B98, left) == 0) {
      continue;
    }
    TowardEye(base, frame, pass, {988, 1004, 1020, 1036});
    Multiply(base, frame + 976, frame + 1168, frame + 336, 0x820E1CA4);
    Ones(base, frame, {860, 876, 892, 908});
    Multiply(base, frame + 848, frame + 976, frame + 1232, 0x820E1CC4);
    CopyMatrix(base, frame + 336, frame + 848, 0x820E1CD0);
    Ones(base, frame, {924, 940, 956, 972});
    Multiply(base, frame + 912, frame + 1168, frame + 656, 0x820E1CF0);
    Ones(base, frame, {796, 812, 828, 844});
    Multiply(base, frame + 784, frame + 912, frame + 400, 0x820E1D10);
    left = CopyMatrix(base, frame + 336, frame + 784, 0x820E1D1C);
    for (int32_t shape = 0; shape < shapes; shape++) {
      left = GlowDraw(base, calls, frame, model, vp, uint32_t(shape), 0xFFFFFFFF826C0000ull,
                      0x820E1D40);
    }
  }
  Flip(base);
  return left;
}

// --- The traffic's headlights ---

Left TrafficDrawHeadlights(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                           uint32_t block, uint32_t pass, uint32_t caller_r24,
                           uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820E61D8 TrafficPoints::DrawHeadlights");
  const uint32_t frame = Frame(base, stack, kTrafficHeadlightsFrame);
  const uint32_t vp = pass + 144, world = block + 16;
  // hl00add_low with bit 0x1 (r28 the caller's), then the cover with its
  // alpha (r28 the block's world from here on).
  if (Bits(base, block) & 0x1) {
    PartsDrawRecord(base, calls, frame, group + 1200, vp, world, 0, caller_r24, caller_r28,
                    0x820E6228);
  }
  CoverAlpha(base, group + 1440, block, frame);
  PartsDrawRecord(base, calls, frame, group + 1440, vp, world, 0, caller_r24, world, 0x820E6258);

  const double one = Lfs(base, kOne);
  for (uint32_t at : {668u, 684u, 700u, 716u, 348u}) {
    Stfs(base, frame + at, one);
  }
  Flip(base);
  for (uint32_t at : {364u, 380u, 396u, 412u, 428u, 444u, 460u}) {
    Stfs(base, frame + at, one);
  }
  Left left = ViewTransposed(base, calls, frame, pass, 0x820E62B0);
  FreshIdentity(base, frame + 400, frame + 208);
  // The color (1, 1, 1, 1) at +160 (copied to +272) and SetMaterials' flags
  // at +176 (the first field only).
  for (uint32_t at : {160u, 164u, 168u, 172u, 284u}) {
    Stfs(base, frame + at, one);
  }
  Store(base, frame + 176, 1);
  const uint32_t bits = Bits(base, block);
  const uint64_t c0 = Load64(base, frame + 160);
  Stfs(base, frame + 300, one);
  const uint64_t c1 = Load64(base, frame + 168);
  Stfs(base, frame + 316, one);
  Store(base, frame + 196, 0);
  Store(base, frame + 192, 0);
  Store(base, frame + 272, uint32_t(c0 >> 32));
  Store(base, frame + 276, uint32_t(c0));
  Store(base, frame + 188, 0);
  Store(base, frame + 184, 0);
  Store(base, frame + 180, 0);
  Store(base, frame + 280, uint32_t(c1 >> 32));
  Store(base, frame + 284, uint32_t(c1));
  if ((bits & 0x1) == 0 || (bits & 0x20) != 0) {
    Flip(base);
    return left;
  }

  // The glows: the scene's glow model in white at nodes 0 to 3, each facing
  // the camera, with the block's world.
  const uint32_t model = Load(base, kScene) + 480;
  left = GlowColor(base, calls, frame, model, 176, 0x820E63A8, 0x820E63CC);
  const int32_t shapes = int32_t(Load(base, model + 412));
  const double zero = Lfs(base, kZero);
  for (uint32_t node = 0; node < 4; node++) {
    const uint32_t record = group + 240 * node;
    if (Load(base, record + 4) == 0) {
      continue;
    }
    FreshIdentity(base, frame + 336, frame + 208);
    const uint64_t p0 = Load64(base, record + 144), p1 = Load64(base, record + 152);
    Stfs(base, frame + 732, one);
    Stfs(base, frame + 748, one);
    Stfs(base, frame + 764, one);
    Store(base, frame + 384, uint32_t(p0 >> 32));
    Store(base, frame + 388, uint32_t(p0));
    Stfs(base, frame + 780, one);
    Store(base, frame + 392, uint32_t(p1 >> 32));
    Store(base, frame + 396, uint32_t(p1));
    Multiply(base, frame + 720, frame + 336, world, 0x820E6478);
    CopyMatrix(base, frame + 336, frame + 720, 0x820E6484);
    Ones(base, frame, {1052, 1068, 1084, 1100});
    Copy(base, frame + 448, frame + 384, 16);
    Multiply(base, frame + 1040, frame + 656, frame + 400, 0x820E64BC);
    CopyMatrix(base, frame + 336, frame + 1040, 0x820E64C8);
    // The scale (+1168), the projector's view (+1296) and the world's
    // rotation (+1104, its last row (0, 0, 0, 1), copied to +1360).
    FreshIdentity(base, frame + 1168, frame + 528);
    FreshIdentity(base, frame + 1296, frame + 464);
    FreshIdentity(base, frame + 1104, frame + 592);
    left = CopyMatrix(base, frame + 1104, world, 0x820E65DC);
    Stfs(base, frame + 176, zero);
    Stfs(base, frame + 180, zero);
    Stfs(base, frame + 184, zero);
    Stfs(base, frame + 188, one);
    Copy(base, frame + 1152, frame + 176, 16);
    Memcpy(base, calls, frame + 1360, frame + 1104, 64, frame, 0x820E6614);
    if (Project(base, calls, frame, pass, frame + 1168, 1, 0x820E6638, left) == 0) {
      continue;
    }
    TowardEye(base, frame, pass, {988, 1004, 1020, 1036});
    Multiply(base, frame + 976, frame + 1168, frame + 336, 0x820E6744);
    Ones(base, frame, {860, 876, 892, 908});
    Multiply(base, frame + 848, frame + 976, frame + 1232, 0x820E6764);
    CopyMatrix(base, frame + 336, frame + 848, 0x820E6770);
    Ones(base, frame, {924, 940, 956, 972});
    Multiply(base, frame + 912, frame + 1168, frame + 656, 0x820E6790);
    Ones(base, frame, {796, 812, 828, 844});
    Multiply(base, frame + 784, frame + 912, frame + 400, 0x820E67B0);
    left = CopyMatrix(base, frame + 336, frame + 784, 0x820E67BC);
    for (int32_t shape = 0; shape < shapes; shape++) {
      left = GlowDraw(base, calls, frame, model, vp, uint32_t(shape), 0, 0x820E67E0);
    }
  }
  Flip(base);
  return left;
}

}  // namespace car_draw
