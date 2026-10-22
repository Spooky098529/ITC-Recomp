// The matrix and quaternion math (src/matrix_math/matrix_math.cpp) against
// the recompiled functions: matrices, vectors and quaternions in a block of
// the scratch area, filled anew for each case, with the output apart from
// the inputs, on one of them, the inputs on each other, and everything 4
// bytes on (the vector unit's aligned loads and stores, and the copies'
// overlaps). The whole block, r3 and (where a function returns one) f1 are
// compared. Three groups: numbers only (infinities, denormals and signed
// zeros among them), finite numbers with one NaN word put in the block or
// f1, and NaNs anywhere (expected to differ: where two NaNs meet in one
// operation, which comes out depends on the order the compiler gave the
// operands, in the translated code as in ours (the Xenon keeps the first;
// x86 keeps the first of its own order, and LLVM treats dpps, mulss and
// addss as commutable), and the translated code, built without
// -ffp-model=strict, may also flip a NaN's sign where LLVM rewrites a
// subtraction; the game's matrices hold no NaNs).

#include "harness.h"

#include "base/flags.h"
#include "matrix_math/matrix_math.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <iterator>
#include <string>

ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_vector_library);
ITC_FLAG_DECLARE(bool, native_math);

namespace harness {

namespace {

namespace mm = matrix_math;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_VECTOR(name, call)         \
  void name(PPCContext& ctx, uint8_t* base) { \
    call;                                     \
    ctx.fpscr.enableFlushMode();              \
  }
#define ITC_NATIVE_SCALAR(name, call)         \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    call;                                     \
  }

ITC_NATIVE_VECTOR(NativeMultiply, mm::Multiply(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_SCALAR(NativeCopy, mm::Copy(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_VECTOR(NativeInverse, mm::Inverse(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_VECTOR(NativeVec4Transform,
                  mm::Vec4Transform(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_SCALAR(NativeVec3SwapXZ, mm::Vec3SwapXZ(base, ctx.r3.u32))
ITC_NATIVE_SCALAR(NativeSetWsToOne, mm::SetWsToOne(base, ctx.r3.u32))
// Matrix::MultiplyUnaligned and Matrix::MultiplyTo (its tail call, merged
// into Matrix::MultiplyReversed) return out in r3.
ITC_NATIVE_VECTOR(NativeMultiplyUnaligned,
                  mm::MultiplyUnaligned(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_VECTOR(NativeMultiplyReversed,
                  mm::MultiplyReversed(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_SCALAR(NativeInverseRotate,
                  mm::InverseRotate(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
// r3 comes back as out + 48 from both (the last Vec4::Copy's destination).
ITC_NATIVE_SCALAR(NativeSet, mm::Set(base, ctx.r3.u32, ctx.r4.u32); ctx.r3.u64 = ctx.r3.u32 + 48)
ITC_NATIVE_SCALAR(NativeSetIdentity, mm::SetIdentity(base, ctx.r3.u32);
                  ctx.r3.u64 = ctx.r3.u32 + 48)
ITC_NATIVE_SCALAR(NativeFromQuaternion, mm::FromQuaternion(base, ctx.r3.u32, ctx.r4.u32);
                  ctx.r3.u64 = ctx.r4.u32)
ITC_NATIVE_SCALAR(NativeLengthSquared,
                  ctx.f1.f64 = double(mm::QuaternionLengthSquared(base, ctx.r3.u32)))
ITC_NATIVE_SCALAR(NativeProjectPoint,
                  mm::ProjectPoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_SCALAR(NativeQuaternionFromMatrix,
                  mm::QuaternionFromMatrix(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_SCALAR(NativeQuaternionMultiply,
                  mm::QuaternionMultiply(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_SCALAR(NativeQuaternionNormalizeSelf, mm::QuaternionNormalizeSelf(base, ctx.r3.u32))
ITC_NATIVE_SCALAR(NativeSinCos,
                  ctx.f1.f64 = mm::SinCos(base, ctx.f1.f64, ctx.r4.u32, ctx.r5.u32))

// Runs the recompiled version with the flags off, so the functions it calls
// that are ours (Vec4::Copy, sqrtf, sin, cos) run the originals too.
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool matrices = ITC_FLAG(native_matrix_math);
  const bool vectors = ITC_FLAG(native_vector_library);
  const bool math = ITC_FLAG(native_math);
  ITC_FLAG(native_matrix_math) = false;
  ITC_FLAG(native_vector_library) = false;
  ITC_FLAG(native_math) = false;
  recompiled(t, base);
  ITC_FLAG(native_matrix_math) = matrices;
  ITC_FLAG(native_vector_library) = vectors;
  ITC_FLAG(native_math) = math;
}

// NaNs of both kinds and signs, infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001,
                                   0x7F800000, 0xFF800000, 0x00000001, 0x807FFFFF,
                                   0x00000000, 0x80000000, 0x7F7FFFFF, 0x3F800000};

// Float words: odd floats, random bits, and ordinary values (-64 to 64)
// so the arithmetic is mostly on numbers.
uint32_t FloatWord(Random& r) {
  switch (r.Below(5)) {
    case 0:
      return kOddFloats[r.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(r.Next());
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 8192.0f);
  }
}

// The same without NaNs.
uint32_t NumberWord(Random& r) {
  uint32_t word;
  do {
    word = FloatWord(r);
  } while (std::isnan(std::bit_cast<float>(word)));
  return word;
}

// Finite numbers only (no infinities either), so no NaN is made from
// them (inf - inf, 0 x inf) to meet the one NaN put in.
uint32_t FiniteWord(Random& r) {
  uint32_t word;
  do {
    word = FloatWord(r);
  } while (!std::isfinite(std::bit_cast<float>(word)));
  return word;
}

Random Seeded(uint64_t seed) {
  Random r;
  for (uint64_t i = 0; i < seed; i++) {
    r.Next();
  }
  return r;
}

constexpr uint32_t kBlockBytes = 0x400;
constexpr uint32_t kA = 0x100, kB = 0x200, kOut = 0x300;

}  // namespace

void MatrixMath(Context& h) {
  // The block 16-byte aligned, as the game's matrices are.
  const uint32_t block = (h.data + 15) & ~15u;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    bool compare_f1;
  } functions[] = {
      {"Matrix::Multiply (sub_8209BD50)", __imp__sub_8209BD50, NativeMultiply, false},
      {"Matrix::Copy (sub_820A0AC0)", __imp__sub_820A0AC0, NativeCopy, false},
      {"Matrix::Inverse (sub_820A9988)", __imp__sub_820A9988, NativeInverse, false},
      {"Vec4::Transform (sub_820AB560)", __imp__sub_820AB560, NativeVec4Transform, false},
      {"Vec3::SwapXZ (sub_820AB688)", __imp__sub_820AB688, NativeVec3SwapXZ, false},
      {"SetWsToOne (sub_8209B038)", __imp__sub_8209B038, NativeSetWsToOne, false},
      {"Matrix::MultiplyUnaligned (sub_822B8F78)", __imp__sub_822B8F78, NativeMultiplyUnaligned,
       false},
      {"Matrix::MultiplyTo (sub_82293598)", __imp__sub_82293598, NativeMultiplyUnaligned, false},
      {"Matrix::MultiplyReversed (sub_82293988)", __imp__sub_82293988, NativeMultiplyReversed,
       false},
      {"Matrix::InverseRotate (sub_82293998)", __imp__sub_82293998, NativeInverseRotate, false},
      {"Matrix::Set (sub_822935A0)", __imp__sub_822935A0, NativeSet, false},
      {"Matrix::SetIdentity (sub_822936E0)", __imp__sub_822936E0, NativeSetIdentity, false},
      {"Matrix::FromQuaternion (sub_822936F0)", __imp__sub_822936F0, NativeFromQuaternion,
       false},
      {"Quaternion::LengthSquared (sub_82293CB0)", __imp__sub_82293CB0, NativeLengthSquared,
       true},
      // f1 is a scratch register in the next two (left a matrix element or a
      // square root); callers don't read it, so it isn't compared.
      {"Matrix::ProjectPoint (sub_82293600)", __imp__sub_82293600, NativeProjectPoint, false},
      {"Quaternion::FromMatrix (sub_82293AD8)", __imp__sub_82293AD8, NativeQuaternionFromMatrix,
       false},
      {"Quaternion::Multiply (sub_82293D78)", __imp__sub_82293D78, NativeQuaternionMultiply,
       false},
      {"Quaternion::NormalizeSelf (sub_82293F10)", __imp__sub_82293F10,
       NativeQuaternionNormalizeSelf, false},
      {"SinCos (sub_820B0E10)", __imp__sub_820B0E10, NativeSinCos, true},
  };
  // r3, r4, r5 (as out, a, b) for each layout.
  const struct {
    uint32_t out, a, b;
    const char* name;
  } layouts[] = {
      {kOut, kA, kB, "apart"},
      {kA, kA, kB, "out on a"},
      {kB, kA, kB, "out on b"},
      {kOut, kA, kA, "a on b"},
      {kOut + 4, kA + 4, kB + 8, "unaligned"},
      {kA + 4, kA, kB, "out 4 after a"},
      {kA, kA + 4, kB, "out 4 before a"},
  };
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN",
                                     ", NaNs anywhere (expected to differ)"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      for (const auto& layout : layouts) {
        for (uint64_t seed = 0; seed < 30; seed++) {
          Random fill = Seeded(seed * 11 + 5 * uint64_t(group) + 1);
          auto word = [group](Random& r) {
            return group == kNaNs ? FloatWord(r) : group == kOneNaN ? FiniteWord(r) : NumberWord(r);
          };
          const double f1 = double(std::bit_cast<float>(word(fill)));
          // The one NaN: in the block, or (one case in ten) in f1.
          const uint32_t nan_word = kNaNWords[fill.Below(std::size(kNaNWords))];
          const uint32_t nan_at = fill.Below(kBlockBytes / 4 + kBlockBytes / 40);
          std::vector<uint8_t> result[2];
          for (int v = 0; v < 2; v++) {
            Random words = Seeded(seed * 13 + 7 * uint64_t(group) + 3);
            for (uint32_t i = 0; i < kBlockBytes; i += 4) {
              guest::StoreU32(h.base, block + i, word(words));
            }
            if (group == kOneNaN && nan_at < kBlockBytes / 4) {
              guest::StoreU32(h.base, block + 4 * nan_at, nan_word);
            }
            PPCContext t = CallContext(h);
            // Flush-to-zero off in the context's copy of the control word
            // too (as the vehicle accessors' harness).
            t.fpscr.disableFlushModeUnconditional();
            t.r3.u64 = block + layout.out;
            t.r4.u64 = block + layout.a;
            t.r5.u64 = block + layout.b;
            t.f1.f64 = group == kOneNaN && nan_at >= kBlockBytes / 4
                           ? double(std::bit_cast<float>(nan_word))
                           : f1;
            if (v == 0) {
              RunRecompiled(f.recompiled, t, h.base);
            } else {
              f.native(t, h.base);
            }
            result[v] = Read(h.base, block, kBlockBytes);
            for (int i = 0; i < 4; i++) {
              result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
            }
            for (int i = 0; f.compare_f1 && i < 8; i++) {
              result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
            }
          }
          results.Check(result[0], result[1],
                        fmt::format("{} seed {} f1 {:016X}", layout.name, seed,
                                    std::bit_cast<uint64_t>(f1)));
        }
      }
      results.Report();
    }
  }
}

}  // namespace harness
