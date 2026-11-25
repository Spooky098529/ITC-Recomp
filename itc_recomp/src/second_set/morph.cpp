// The morph and the tire's table helpers (see morph.h), load for load and
// store for store as the recompiled functions.

#include "second_set/morph.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/models.h"
#include "schema/second_set.h"

#include <bit>
#include <cstdlib>
#include <cstring>

namespace second_set {

namespace {

using namespace camera_modes::steps;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
double LoadDouble(uint32_t address) {
  return std::bit_cast<double>((uint64_t(Load(address)) << 32) | Load(address + 4));
}

// lvx: the aligned 16 bytes holding `address`, word 0 in the top lane.
simde__m128 Lvx(uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(at), Load(at + 4), Load(at + 8), Load(at + 12));
}

// Shape `shape` of `model` (the model's +40, 544 bytes a shape).
schema::ModelShape Shape(uint32_t model, uint32_t shape) {
  return schema::ModelShape(shape * schema::ModelShape::kSize + schema::Model(model).shapes().address);
}

// The blends' copies: whether Buffers_Memcpy will copy (checked before
// anything changes), and the copy.
bool WillCopy(uint32_t to, uint32_t from, uint32_t bytes) {
  return native::Buffers_NativeMemcpy() &&
         (bytes == 0 || !c_runtime::MayShare(to, bytes, from, bytes));
}
void Copy(uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (!native::Buffers_Memcpy(to, from, bytes, caller)) {
    ITC_LOG_ERROR("Second set: a morph's memcpy refused after its checks");
    std::abort();
  }
}

// The weights' vector the originals build in their frames (three stfs of
// the weight, the fourth word 0.0, then lvx): a C++ value, as only the
// blend reads it. `weight` the float's bits as stfs stores them.
simde__m128 WeightVector(uint32_t weight, uint32_t zero) {
  return VectorOf(weight, weight, weight, zero);
}

BlendResult Blend6(uint32_t morph, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8258B578 Morph::Blend6");
  // The original's frame (128 bytes) held only the weights' vector at +80,
  // which only it read: a C++ value now. r3 as it leaves it: that slot's
  // address.
  const uint32_t weight_slot = stack - 128 + 80;
  const schema::Morph m(morph);
  const uint32_t bytes = m.vertices() << 4;
  const uint32_t out = m.out1().address;
  Copy(out, m.base_stream(), bytes, 0x8258B5A8);
  BlendResult result{out, Flush::kUnchanged};
  const uint32_t targets = m.count();
  if (targets == 0) {
    return result;
  }
  const uint32_t vertices = m.vertices();
  const uint32_t zero = Bits(Lfs(0x82000A34));
  result.flush = Flush::kOff;
  for (uint32_t i = 0; i < targets; i++) {
    const uint32_t w = Bits(Lfs(m.weights_address() + 4 * i));
    result.flush = Flush::kOff;
    if (vertices == 0) {
      continue;
    }
    result.r3 = weight_slot;
    const simde__m128 weight = WeightVector(w, zero);
    const uint32_t from = m.base_stream() - out;
    const uint32_t target = Load(m.targets_address() + 4 * i) - out;
    uint32_t at = out;
    for (uint32_t k = 0; k < vertices; k++) {
      const simde__m128 t = Lvx(target + at);
      const simde__m128 b = Lvx(from + at);
      FlushToZero ftz;
      const simde__m128 d = simde_mm_sub_ps(t, b);
      const simde__m128 o = Lvx(at);
      Stvx(at, simde_mm_add_ps(simde_mm_mul_ps(d, weight), o));
      at += 16;
    }
    result.flush = Flush::kOn;
  }
  return result;
}

BlendResult Blend7(uint32_t morph, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8258B678 Morph::Blend7");
  // The original's frame (160 bytes) held the sizes at +80 and +84 (never
  // read back) and the weights' vector at +96, which only it read: a C++
  // value now, the sizes' stores dropped.
  (void)stack;
  const schema::Morph m(morph);
  const uint32_t bytes = m.vertices() << 4;
  Copy(m.out1().address, m.base_stream(), bytes, 0x8258B6A4);
  const uint32_t out2 = m.out2().address;
  Copy(out2, m.base_stream2(), bytes, 0x8258B6B4);
  BlendResult result{out2, Flush::kUnchanged};
  const uint32_t targets = m.count();
  if (targets != 0) {
    const uint32_t vertices = m.vertices();
    const uint32_t zero = Bits(Lfs(0x82000A34));
    for (uint32_t i = 0; i < targets; i++) {
      const uint32_t w = Bits(Lfs(m.weights_address() + 4 * i));
      result.flush = Flush::kOff;
      if (vertices == 0) {
        continue;
      }
      const uint32_t base1 = m.base_stream();
      const uint32_t target1 = Load(m.targets_address() + 4 * i);
      const uint32_t second = Load(m.target_seconds_address() + 4 * i);
      const uint32_t base2 = m.base_stream2();
      const uint32_t out1 = m.out1().address;
      result.r3 = uint64_t(second) - uint64_t(target1);
      const simde__m128 weight = WeightVector(w, zero);
      for (uint32_t k = 0; k < vertices; k++) {
        const uint32_t step = 16 * k;
        const simde__m128 s = Lvx(second + step);
        const simde__m128 b2 = Lvx(base2 + step);
        FlushToZero ftz;
        const simde__m128 d2 = simde_mm_sub_ps(s, b2);
        const simde__m128 b1 = Lvx(base1 + step);
        const simde__m128 t1 = Lvx(target1 + step);
        const simde__m128 d1 = simde_mm_sub_ps(t1, b1);
        const simde__m128 o2 = Lvx(out2 + step);
        const simde__m128 o1 = Lvx(out1 + step);
        const simde__m128 n2 = simde_mm_add_ps(simde_mm_mul_ps(d2, weight), o2);
        const simde__m128 n1 = simde_mm_add_ps(simde_mm_mul_ps(d1, weight), o1);
        Stvx(out2 + step, n2);
        Stvx(out1 + step, n1);
      }
      result.flush = Flush::kOn;
    }
  }
  return result;
}

}  // namespace

uint32_t ModelStreamData(uint32_t model, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82594A60 Model::StreamData");
  return Shape(model, shape).stream1_offset();
}

uint32_t ModelStreamSize(uint32_t model, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82594A78 Model::StreamSize");
  // Stream 1's descriptor's +12 (models' ModelShape has no field there).
  return Load(Shape(model, shape).stream1_address() + 12);
}

uint32_t ModelStreamData2(uint32_t model, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82594A90 Model::StreamData2");
  return Shape(model, shape).stream2_offset();
}

uint32_t ModelStreamSize2(uint32_t model, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82594AA8 Model::StreamSize2");
  // Stream 2's descriptor's +12 (models' ModelShape has no field there).
  return Load(Shape(model, shape).stream2_address() + 12);
}

Flush MorphSetUp(uint32_t model, uint32_t morph, uint32_t shape,
                 uint32_t indices, uint32_t weights, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82594C18 Morph::SetUp");
  const schema::ModelShape at = Shape(model, shape);
  const uint32_t first = at.skin_array_a();
  const uint32_t second = at.skin_array_b();
  if (count >= 8) {
    count = 7;
  }
  const schema::Morph m(morph);
  m.set_base_stream(first + 16);
  m.set_base_stream2(second + 16);
  m.set_vertices(Load(first + 0));
  m.set_count(count);
  m.set_kind(Load(second + 4));
  Flush flush = Flush::kUnchanged;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t target = Shape(model, Load(indices + 4 * i)).skin_array_a();
    if (m.vertices() != Load(target + 0)) {
      ITC_LOG_WARN("Second set: Morph::SetUp's debug message 0x8206CD20 ({} against {})",
                   m.vertices(), Load(target + 0));
    }
    if (m.kind() != Load(second + 4)) {
      ITC_LOG_WARN("Second set: Morph::SetUp's debug message 0x8206CD4C ({} against {})",
                   m.kind(), Load(target + 4));
    }
    Store(m.target_seconds_address() + 4 * i, second + 16);
    Store(m.targets_address() + 4 * i, target + 16);
    Stfs(m.weights_address() + 4 * i, Lfs(weights + 4 * i));
    flush = Flush::kOff;
  }
  return flush;
}

std::optional<BlendResult> MorphBlend(uint32_t morph, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8258BD28 Morph::Blend");
  const schema::Morph m(morph);
  const uint32_t kind = m.kind();
  if (kind == 8) {
    return std::nullopt;
  }
  if (kind != 6 && kind != 7) {
    return BlendResult{morph, Flush::kUnchanged};
  }
  const uint32_t bytes = m.vertices() << 4;
  if (!WillCopy(m.out1().address, m.base_stream(), bytes) ||
      (kind == 7 && !WillCopy(m.out2().address, m.base_stream2(), bytes))) {
    return std::nullopt;
  }
  return kind == 6 ? Blend6(morph, stack) : Blend7(morph, stack);
}

std::optional<BlendResult> MorphRun(uint32_t morph, uint32_t out1, uint32_t out2,
                                    uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82594D10 Morph::Run");
  const schema::Morph m(morph);
  const uint32_t kind = m.kind();
  if (kind == 8) {
    return std::nullopt;
  }
  if (kind == 6 || kind == 7) {
    const uint32_t bytes = m.vertices() << 4;
    if (!WillCopy(out1, m.base_stream(), bytes) ||
        (kind == 7 && !WillCopy(out2, m.base_stream2(), bytes))) {
      return std::nullopt;
    }
  }
  m.set_out1(out1);
  m.set_out2(out2);
  return MorphBlend(morph, stack);
}

void TireFillEntries(uint32_t entries, uint32_t index, uint32_t value, double f1,
                     uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E6F28 TireFillEntries");
  // The original's red-zone word at r1 - 16 (fctidz's store, never read
  // back but by it) is a C++ value now.
  (void)stack;
  const int64_t skip = int64_t(int32_t(index)) - 12;
  const int32_t skip32 = int32_t(uint32_t(skip));
  double f13 = Lfs(value);
  double f0 = Fmuls(f1, Lfs(0x8200222C));
  const double f11 = Lfs(0x820021E8);
  const uint32_t entry = uint32_t(uint64_t(skip) * 24) + entries;
  const double f12 = Lfs(0x82002D0C);
  f0 = Fmuls(f0, f13);
  Stfs(entry + 0, f13);
  Stfs(entry + 8, f0);
  Stfs(entry + 4, f1);
  const double f10 = Lfs(0x82002A0C);
  f13 = Single(double(int64_t(int32_t(uint32_t(skip + 12)))));
  f13 = Fmuls(f13, f12);
  Stfs(entry + 12, f13);
  f0 = Fmadds(f0, f11, f13);
  Stfs(entry + 16, f0);
  const double scale = Lfs(0x82001BE4);
  for (int32_t k = 0; k < 12; k++) {
    const uint32_t at = entries + 12 + 24 * uint32_t(k);
    f13 = Lfs(value);
    Stfs(at - 12, f13);
    if (k == skip32) {
      continue;
    }
    f13 = Lfs(value);
    double f9 = f13;
    Stfs(at - 12, f13);
    Stfs(at + 4, f0);
    const int64_t n = int64_t(k + 12);
    f13 = Single(double(n));
    f13 = Fmuls(f13, f12);
    Stfs(at + 0, f13);
    const double f8 = Fsubs(f0, f13);
    f13 = Fmuls(f8, scale);
    Stfs(at - 4, f13);
    f9 = Fdivs(f13, f9);
    f13 = Fmuls(f9, f10);
    Stfs(at - 8, f13);
  }
}

void TireReadEntry(uint32_t index, uint32_t entries, uint32_t out4, uint32_t out1,
                   uint32_t out2, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7010 TireReadEntry");
  // The original's red-zone word at r1 - 16 (fctidz's store, read back only
  // by it) is a C++ value now.
  (void)stack;
  const double zero = Lfs(0x82000A34);
  const uint64_t i = uint64_t(int64_t(int32_t(index))) - 12;
  const uint32_t entry = uint32_t(i * 24) + entries;
  double f13 = Single(double(int64_t(int32_t(uint32_t(i)))));
  double f0 = Fmuls(f13, Lfs(0x82002A88));
  Stfs(out4 + 0, f0);
  const double f12a = Lfs(entry + 0);
  f13 = Lfs(0x82006630);
  double f10 = Fsubs(f12a, f13);
  const double f12 = LoadDouble(0x82006628);
  f10 = Single(f10 * f12);
  Stfs(out4 + 4, f10);
  const double f9 = Lfs(entry + 8);
  f10 = Fsubs(f9, Lfs(0x8200257C));
  Stfs(out4 + 12, zero);
  Stfs(out4 + 8, Single(f10 * f12));
  Stfs(out1, f0);
  f0 = Lfs(entry + 0);
  f13 = Fsubs(f0, f13);
  f13 = Fmuls(f13, Lfs(0x82001BE4));
  f0 = Fmuls(f13, Lfs(0x820029D8));
  Stfs(out2, f0);
}

}  // namespace second_set
