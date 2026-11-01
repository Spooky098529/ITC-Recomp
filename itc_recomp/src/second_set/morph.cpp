// The morph and the tire's table helpers (see morph.h), load for load and
// store for store as the recompiled functions.

#include "second_set/morph.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "native/buffers.h"

#include <bit>
#include <cstdlib>
#include <cstring>

namespace second_set {

namespace {

using namespace camera_modes::steps;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>((uint64_t(Load(base, address)) << 32) | Load(base, address + 4));
}

// lvx: the aligned 16 bytes holding `address`, word 0 in the top lane.
simde__m128 Lvx(uint8_t* base, uint32_t address) {
  const uint32_t at = address & ~0xFu;
  return VectorOf(Load(base, at), Load(base, at + 4), Load(base, at + 8), Load(base, at + 12));
}

uint32_t ShapeWord(uint8_t* base, uint32_t model, uint32_t shape, uint32_t offset) {
  return Load(base, shape * 544 + Load(base, model + 40) + offset);
}

// The blends' copies: whether Buffers_Memcpy will copy (checked before
// anything changes), and the copy.
bool WillCopy(uint32_t to, uint32_t from, uint32_t bytes) {
  return native::Buffers_NativeMemcpy() &&
         (bytes == 0 || !c_runtime::MayShare(to, bytes, from, bytes));
}
void Copy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, caller)) {
    ITC_LOG_ERROR("Second set: a morph's memcpy refused after its checks");
    std::abort();
  }
}

// The weight at `weight` (lfs) into the frame's +80, +84, +88 (stfs).
void WeightVector(uint8_t* base, uint32_t sp, uint32_t weight) {
  const double w = Lfs(base, weight);
  Stfs(base, sp + 80, w);
  Stfs(base, sp + 84, w);
  Stfs(base, sp + 88, w);
}

// The frame's sizes word after Blend7's empty loops: the first multiple of
// 128 at or past `bytes` (0 for 0).
uint32_t Lines(uint32_t bytes) {
  uint32_t at = 0;
  if (bytes != 0) {
    do {
      at += 128;
    } while (at < bytes);
  }
  return at;
}

BlendResult Blend6(uint8_t* base, uint32_t morph, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8258B578 Morph::Blend6");
  const uint32_t sp = stack - 128;
  const uint32_t bytes = Load(base, morph + 116) << 4;
  const uint32_t out = Load(base, morph + 8);
  Copy(base, out, Load(base, morph + 0), bytes, 0x8258B5A8);
  BlendResult result{out, Flush::kUnchanged};
  const uint32_t targets = Load(base, morph + 112);
  if (targets == 0) {
    return result;
  }
  const uint32_t vertices = Load(base, morph + 116);
  Stfs(base, sp + 92, Lfs(base, 0x82000A34));
  result.flush = Flush::kOff;
  for (uint32_t i = 0; i < targets; i++) {
    WeightVector(base, sp, morph + 80 + 4 * i);
    result.flush = Flush::kOff;
    if (vertices == 0) {
      continue;
    }
    result.r3 = sp + 80;
    const simde__m128 weight = Lvx(base, sp + 80);
    const uint32_t from = Load(base, morph + 0) - out;
    const uint32_t target = Load(base, morph + 16 + 4 * i) - out;
    uint32_t at = out;
    for (uint32_t k = 0; k < vertices; k++) {
      const simde__m128 t = Lvx(base, target + at);
      const simde__m128 b = Lvx(base, from + at);
      FlushToZero ftz;
      const simde__m128 d = simde_mm_sub_ps(t, b);
      const simde__m128 o = Lvx(base, at);
      Stvx(base, at, simde_mm_add_ps(simde_mm_mul_ps(d, weight), o));
      at += 16;
    }
    result.flush = Flush::kOn;
  }
  return result;
}

BlendResult Blend7(uint8_t* base, uint32_t morph, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8258B678 Morph::Blend7");
  const uint32_t sp = stack - 160;
  const uint32_t bytes = Load(base, morph + 116) << 4;
  Store(base, sp + 84, bytes);
  Copy(base, Load(base, morph + 8), Load(base, morph + 0), bytes, 0x8258B6A4);
  const uint32_t out2 = Load(base, morph + 12);
  Copy(base, out2, Load(base, morph + 4), bytes, 0x8258B6B4);
  BlendResult result{out2, Flush::kUnchanged};
  const uint32_t targets = Load(base, morph + 112);
  if (targets != 0) {
    const uint32_t vertices = Load(base, morph + 116);
    Stfs(base, sp + 108, Lfs(base, 0x82000A34));
    for (uint32_t i = 0; i < targets; i++) {
      const double w = Lfs(base, morph + 80 + 4 * i);
      Stfs(base, sp + 96, w);
      Stfs(base, sp + 100, w);
      Stfs(base, sp + 104, w);
      result.flush = Flush::kOff;
      if (vertices == 0) {
        continue;
      }
      const uint32_t base1 = Load(base, morph + 0);
      const uint32_t target1 = Load(base, morph + 16 + 4 * i);
      const uint32_t second = Load(base, morph + 48 + 4 * i);
      const uint32_t base2 = Load(base, morph + 4);
      const uint32_t out1 = Load(base, morph + 8);
      result.r3 = uint64_t(second) - uint64_t(target1);
      const simde__m128 weight = Lvx(base, sp + 96);
      for (uint32_t k = 0; k < vertices; k++) {
        const uint32_t step = 16 * k;
        const simde__m128 s = Lvx(base, second + step);
        const simde__m128 b2 = Lvx(base, base2 + step);
        FlushToZero ftz;
        const simde__m128 d2 = simde_mm_sub_ps(s, b2);
        const simde__m128 b1 = Lvx(base, base1 + step);
        const simde__m128 t1 = Lvx(base, target1 + step);
        const simde__m128 d1 = simde_mm_sub_ps(t1, b1);
        const simde__m128 o2 = Lvx(base, out2 + step);
        const simde__m128 o1 = Lvx(base, out1 + step);
        const simde__m128 n2 = simde_mm_add_ps(simde_mm_mul_ps(d2, weight), o2);
        const simde__m128 n1 = simde_mm_add_ps(simde_mm_mul_ps(d1, weight), o1);
        Stvx(base, out2 + step, n2);
        Stvx(base, out1 + step, n1);
      }
      result.flush = Flush::kOn;
    }
  }
  const uint32_t lines = Lines(bytes);
  Store(base, sp + 80, lines);
  return result;
}

}  // namespace

uint32_t ModelStreamData(uint8_t* base, uint32_t model, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82594A60 Model::StreamData");
  return ShapeWord(base, model, shape, 388);
}

uint32_t ModelStreamSize(uint8_t* base, uint32_t model, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82594A78 Model::StreamSize");
  return ShapeWord(base, model, shape, 268);
}

uint32_t ModelStreamData2(uint8_t* base, uint32_t model, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82594A90 Model::StreamData2");
  return ShapeWord(base, model, shape, 392);
}

uint32_t ModelStreamSize2(uint8_t* base, uint32_t model, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82594AA8 Model::StreamSize2");
  return ShapeWord(base, model, shape, 284);
}

Flush MorphSetUp(uint8_t* base, uint32_t model, uint32_t morph, uint32_t shape,
                 uint32_t indices, uint32_t weights, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82594C18 Morph::SetUp");
  const uint32_t at = shape * 544 + Load(base, model + 40);
  const uint32_t first = Load(base, at + 400);
  const uint32_t second = Load(base, at + 404);
  if (count >= 8) {
    count = 7;
  }
  Store(base, morph + 0, first + 16);
  Store(base, morph + 4, second + 16);
  Store(base, morph + 116, Load(base, first + 0));
  Store(base, morph + 112, count);
  Store(base, morph + 120, Load(base, second + 4));
  Flush flush = Flush::kUnchanged;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t target = Load(base, Load(base, indices + 4 * i) * 544 +
                                           Load(base, model + 40) + 400);
    if (Load(base, morph + 116) != Load(base, target + 0)) {
      ITC_LOG_WARN("Second set: Morph::SetUp's debug message 0x8206CD20 ({} against {})",
                   Load(base, morph + 116), Load(base, target + 0));
    }
    if (Load(base, morph + 120) != Load(base, second + 4)) {
      ITC_LOG_WARN("Second set: Morph::SetUp's debug message 0x8206CD4C ({} against {})",
                   Load(base, morph + 120), Load(base, target + 4));
    }
    Store(base, morph + 48 + 4 * i, second + 16);
    Store(base, morph + 16 + 4 * i, target + 16);
    Stfs(base, morph + 80 + 4 * i, Lfs(base, weights + 4 * i));
    flush = Flush::kOff;
  }
  return flush;
}

std::optional<BlendResult> MorphBlend(uint8_t* base, uint32_t morph, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8258BD28 Morph::Blend");
  const uint32_t kind = Load(base, morph + 120);
  if (kind == 8) {
    return std::nullopt;
  }
  if (kind != 6 && kind != 7) {
    return BlendResult{morph, Flush::kUnchanged};
  }
  const uint32_t bytes = Load(base, morph + 116) << 4;
  if (!WillCopy(Load(base, morph + 8), Load(base, morph + 0), bytes) ||
      (kind == 7 && !WillCopy(Load(base, morph + 12), Load(base, morph + 4), bytes))) {
    return std::nullopt;
  }
  return kind == 6 ? Blend6(base, morph, stack) : Blend7(base, morph, stack);
}

std::optional<BlendResult> MorphRun(uint8_t* base, uint32_t morph, uint32_t out1, uint32_t out2,
                                    uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82594D10 Morph::Run");
  const uint32_t kind = Load(base, morph + 120);
  if (kind == 8) {
    return std::nullopt;
  }
  if (kind == 6 || kind == 7) {
    const uint32_t bytes = Load(base, morph + 116) << 4;
    if (!WillCopy(out1, Load(base, morph + 0), bytes) ||
        (kind == 7 && !WillCopy(out2, Load(base, morph + 4), bytes))) {
      return std::nullopt;
    }
  }
  Store(base, morph + 8, out1);
  Store(base, morph + 12, out2);
  return MorphBlend(base, morph, stack);
}

void TireFillEntries(uint8_t* base, uint32_t entries, uint32_t index, uint32_t value, double f1,
                     uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E6F28 TireFillEntries");
  const int64_t skip = int64_t(int32_t(index)) - 12;
  const int32_t skip32 = int32_t(uint32_t(skip));
  double f13 = Lfs(base, value);
  double f0 = Fmuls(f1, Lfs(base, 0x8200222C));
  Store64(base, stack - 16, uint64_t(int64_t(int32_t(uint32_t(skip + 12)))));
  const double f11 = Lfs(base, 0x820021E8);
  const uint32_t entry = uint32_t(uint64_t(skip) * 24) + entries;
  const double f12 = Lfs(base, 0x82002D0C);
  f0 = Fmuls(f0, f13);
  Stfs(base, entry + 0, f13);
  Stfs(base, entry + 8, f0);
  Stfs(base, entry + 4, f1);
  const double f10 = Lfs(base, 0x82002A0C);
  f13 = Single(double(int64_t(int32_t(uint32_t(skip + 12)))));
  f13 = Fmuls(f13, f12);
  Stfs(base, entry + 12, f13);
  f0 = Fmadds(f0, f11, f13);
  Stfs(base, entry + 16, f0);
  const double scale = Lfs(base, 0x82001BE4);
  for (int32_t k = 0; k < 12; k++) {
    const uint32_t at = entries + 12 + 24 * uint32_t(k);
    f13 = Lfs(base, value);
    Stfs(base, at - 12, f13);
    if (k == skip32) {
      continue;
    }
    f13 = Lfs(base, value);
    double f9 = f13;
    Stfs(base, at - 12, f13);
    Stfs(base, at + 4, f0);
    const int64_t n = int64_t(k + 12);
    Store64(base, stack - 16, uint64_t(n));
    f13 = Single(double(n));
    f13 = Fmuls(f13, f12);
    Stfs(base, at + 0, f13);
    const double f8 = Fsubs(f0, f13);
    f13 = Fmuls(f8, scale);
    Stfs(base, at - 4, f13);
    f9 = Fdivs(f13, f9);
    f13 = Fmuls(f9, f10);
    Stfs(base, at - 8, f13);
  }
}

void TireReadEntry(uint8_t* base, uint32_t index, uint32_t entries, uint32_t out4, uint32_t out1,
                   uint32_t out2, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820E7010 TireReadEntry");
  const double zero = Lfs(base, 0x82000A34);
  const uint64_t i = uint64_t(int64_t(int32_t(index))) - 12;
  Store64(base, stack - 16, uint64_t(int64_t(int32_t(uint32_t(i)))));
  const uint32_t entry = uint32_t(i * 24) + entries;
  double f13 = Single(double(int64_t(int32_t(uint32_t(i)))));
  double f0 = Fmuls(f13, Lfs(base, 0x82002A88));
  Stfs(base, out4 + 0, f0);
  const double f12a = Lfs(base, entry + 0);
  f13 = Lfs(base, 0x82006630);
  double f10 = Fsubs(f12a, f13);
  const double f12 = LoadDouble(base, 0x82006628);
  f10 = Single(f10 * f12);
  Stfs(base, out4 + 4, f10);
  const double f9 = Lfs(base, entry + 8);
  f10 = Fsubs(f9, Lfs(base, 0x8200257C));
  Stfs(base, out4 + 12, zero);
  Stfs(base, out4 + 8, Single(f10 * f12));
  Stfs(base, out1, f0);
  f0 = Lfs(base, entry + 0);
  f13 = Fsubs(f0, f13);
  f13 = Fmuls(f13, Lfs(base, 0x82001BE4));
  f0 = Fmuls(f13, Lfs(base, 0x820029D8));
  Stfs(base, out2, f0);
}

}  // namespace second_set
