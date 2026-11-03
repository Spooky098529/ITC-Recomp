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

namespace {

using namespace camera_modes::steps;

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kCullFlip = 0x826C5104;
constexpr uint32_t kOne = 0x82000A30;    // 1.0
constexpr uint32_t kZero = 0x82000A34;   // 0.0
constexpr uint32_t kHalf = 0x82001BE4;   // 0.5
constexpr uint32_t kTwo = 0x820021E8;    // 2.0
constexpr uint32_t kOneBits = 0x3F800000;
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

// The identity's four rows (vupkd3d of zeros and its shuffles), stored as
// stvx stores them.
void Identity(uint8_t* base, uint32_t at) {
  for (uint32_t row = 0; row < 4; row++) {
    Stvx(base, at + 16 * row,
         VectorOf(row == 0 ? kOneBits : 0, row == 1 ? kOneBits : 0, row == 2 ? kOneBits : 0,
                  row == 3 ? kOneBits : 0));
  }
}

// `bytes` from `from` to `to`, doubleword by doubleword in order (ld, std).
void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 8) {
    const uint32_t hi = Load(base, from + i);
    const uint32_t lo = Load(base, from + i + 4);
    Store(base, to + i, hi);
    Store(base, to + i + 4, lo);
  }
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
  Left left;
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

  // The 1.0 w's of the view's transposed rotation and of the matrix at +336.
  const double one = Lfs(base, kOne);
  for (uint32_t at : {668u, 684u, 700u, 716u, 348u}) {
    Stfs(base, frame + at, one);
  }
  Flip(base);
  for (uint32_t at : {364u, 380u, 396u, 412u, 428u, 444u, 460u}) {
    Stfs(base, frame + at, one);
  }
  left = ViewTransposed(base, calls, frame, pass, 0x820DFA28);
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
  if ((bits & 0x40) == 0 && ((bits & 0x1) == 0 || (bits & 0x20) != 0)) {
    Flip(base);
    return left;
  }

  // The glows: the scene's glow model in red at the four tl**_0k_ef nodes.
  const uint32_t model = Load(base, kScene) + 480;
  left = GlowColor(base, calls, frame, model, 176, 0x820DFB34, 0x820DFB58);
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
    Multiply(base, frame + 720, frame + 336, world, 0x820DFC0C);
    CopyMatrix(base, frame + 336, frame + 720, 0x820DFC18);
    Ones(base, frame, {988, 1004, 1020, 1036});
    Copy(base, frame + 448, frame + 384, 16);
    Multiply(base, frame + 976, frame + 656, frame + 400, 0x820DFC50);
    CopyMatrix(base, frame + 336, frame + 976, 0x820DFC5C);
    // The scale (+848), the projector's view (+1296) and the world's
    // rotation (+1168, its last row (0, 0, 0, 1), copied to +1360).
    FreshIdentity(base, frame + 848, frame + 528);
    FreshIdentity(base, frame + 1296, frame + 464);
    FreshIdentity(base, frame + 1168, frame + 592);
    left = CopyMatrix(base, frame + 1168, world, 0x820DFD70);
    Stfs(base, frame + 176, zero);
    Stfs(base, frame + 180, zero);
    Stfs(base, frame + 184, zero);
    Stfs(base, frame + 188, one);
    Copy(base, frame + 1216, frame + 176, 16);
    Memcpy(base, calls, frame + 1360, frame + 1168, 64, frame, 0x820DFDA8);
    if (Project(base, calls, frame, pass, frame + 848, 0, 0x820DFDCC, left) == 0) {
      continue;
    }
    TowardEye(base, frame, pass, {924, 940, 956, 972});
    Multiply(base, frame + 912, frame + 848, frame + 336, 0x820DFED8);
    Ones(base, frame, {1116, 1132, 1148, 1164});
    Multiply(base, frame + 1104, frame + 912, frame + 1232, 0x820DFEF8);
    CopyMatrix(base, frame + 336, frame + 1104, 0x820DFF04);
    // The world: the scale x the view's transposed rotation x the
    // translation at +400.
    Ones(base, frame, {1052, 1068, 1084, 1100});
    Multiply(base, frame + 1040, frame + 848, frame + 656, 0x820DFF24);
    Ones(base, frame, {796, 812, 828, 844});
    Multiply(base, frame + 784, frame + 1040, frame + 400, 0x820DFF44);
    left = CopyMatrix(base, frame + 336, frame + 784, 0x820DFF50);
    // Small glows fade: each shape's alpha |s d| x 2.0 first (NaN draws
    // plainly, as the original's bge).
    if (std::fabs(Lfs(base, frame + 848)) < half) {
      for (int32_t shape = 0; shape < shapes; shape++) {
        const double alpha = Fmuls(std::fabs(Lfs(base, frame + 848)), two);
        left = Left{false, material_passes::SetAlpha(base, model, uint32_t(shape), alpha, frame)};
        left = GlowDraw(base, calls, frame, model, vp, uint32_t(shape), 0, 0x820DFF9C);
      }
    } else {
      for (int32_t shape = 0; shape < shapes; shape++) {
        left = GlowDraw(base, calls, frame, model, vp, uint32_t(shape), 0, 0x820DFFCC);
      }
    }
  }
  Flip(base);
  return left;
}

}  // namespace car_draw
