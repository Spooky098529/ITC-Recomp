// XGraphics' surface compression (src/texture_compression/,
// docs/functions/texture-compression.md) against the recompiled functions.
// Each case writes the same inputs into the scratch area (filled with 0xCD)
// for both versions, runs the original (with --native_texture_compression
// off, so its callees are the originals too) or the plain C++ through the
// register steps (hooks/texture_compression.h), and compares with
// harness::Outputs: the block's bytes and the scratch area exactly, the
// fitted ends (stored with stfs) as floats.
//
// The pixels' alphas come from the generators below: random in 0-1, the
// game's real input (bytes decoded as k/255, both as k * (1/255) and k /
// 255), all equal, all 0 or 1, only 0 and 1, two values, values clustered a
// few 1/255 steps apart, just inside and outside 0 and 1, negative and above
// 1, NaNs, infinities, denormals and -0.0, and random bit patterns. Each
// block runs without dithering and with it (r5 = 1, 0x80000000 and random).
// The last group runs Dxt45::EncodeBlock (sub_82422A68, recompiled, with its
// color half) over rows of blocks on one stack, premultiplied and not, so a
// difference in what the alpha half leaves on the stack for the next block's
// color half would show.

#include "harness.h"

#include "base/flags.h"
#include "hooks/texture_compression.h"
#include "memory/heap.h"
#include "texture_compression/dxt_alpha.h"

#include <fmt/format.h>

#include <array>
#include <bit>
#include <cmath>
#include <limits>
#include <string>

ITC_FLAG_DECLARE(bool, native_texture_compression);

namespace harness {

namespace {

constexpr uint32_t kPixels = 0;    // 16 float4 pixels, 256 bytes
constexpr uint32_t kBlock = 256;   // the 8-byte block (16 kept to see stray writes)
constexpr uint32_t kValues = 512;  // FitEndpoints' 16 floats
constexpr uint32_t kLow = 640;
constexpr uint32_t kHigh = 644;

// Dxt45::EncodeBlock's premultiplied switch (XGCompressSurface's flags 1).
constexpr uint32_t kPremultiplied = 0x82954660;

constexpr int kKinds = 16;

using Block16 = std::array<float, 16>;

float Unit(Random& r) {  // 0 to 1, 24 bits
  return float(r.Next() >> 40) * (1.0f / 16777216.0f);
}

float Special(Random& r) {
  static const uint32_t kBits[] = {
      0x7FC00000, 0xFFC00000, 0x7F800001, 0xFFBADBAD, 0x7F800000, 0xFF800000,
      0x00000001, 0x80000001, 0x007FFFFF, 0x00400000, 0x80000000, 0x00000000,
      0x3F800000, 0x3F7FFFFF, 0x3F800001, 0x33800000, 0xB3800000, 0x7F7FFFFF,
      0xFF7FFFFF, 0x4F000000, 0xCF000000, 0x3B808081, 0x3F7EFEFF,
  };
  return std::bit_cast<float>(kBits[r.Below(std::size(kBits))]);
}

float Byte(uint32_t k, bool divide) {
  return divide ? float(k) / 255.0f : float(k) * (1.0f / 255.0f);
}

// A block of 16 alphas of the given kind.
Block16 Alphas(Random& r, int kind) {
  Block16 a{};
  switch (kind) {
    case 0:  // random in 0-1
      for (float& v : a) v = Unit(r);
      break;
    case 1:  // bytes decoded, the whole range
    case 2:
      for (float& v : a) v = Byte(r.Below(256), kind == 2);
      break;
    case 3: {  // bytes decoded, a narrow window, often touching 0 or 255
      const uint32_t width = 1 + r.Below(24);
      uint32_t low = r.Below(256);
      if (r.Below(4) == 0) low = 0;
      if (r.Below(4) == 0) low = 255 - (width < 255 ? width : 255);
      const bool divide = r.Below(2) == 0;
      for (float& v : a) {
        const uint32_t k = low + r.Below(width + 1);
        v = Byte(k > 255 ? 255 : k, divide);
      }
      break;
    }
    case 4: {  // all equal
      const float v = r.Below(3) == 0 ? Byte(r.Below(256), false) : Unit(r);
      a.fill(v);
      break;
    }
    case 5:  // all 0 or all 1
      a.fill(r.Below(2) == 0 ? 0.0f : 1.0f);
      break;
    case 6:  // only 0 and 1
      for (float& v : a) v = r.Below(2) == 0 ? 0.0f : 1.0f;
      break;
    case 7: {  // two values
      const float x = r.Below(2) ? Unit(r) : Byte(r.Below(256), false);
      const float y = r.Below(2) ? Unit(r) : Byte(r.Below(256), false);
      for (float& v : a) v = r.Below(2) == 0 ? x : y;
      break;
    }
    case 8: {  // clustered: a few 1/255 steps or less apart
      const float center = Unit(r);
      for (float& v : a) v = center + (Unit(r) - 0.5f) * (float(1 + r.Below(4)) / 255.0f);
      break;
    }
    case 9:  // just inside and outside 0 and 1, among random
      for (float& v : a) {
        switch (r.Below(6)) {
          case 0: v = std::nextafter(0.0f, 1.0f) * float(1 + r.Below(1000)); break;
          case 1: v = -std::nextafter(0.0f, 1.0f) * float(1 + r.Below(1000)); break;
          case 2: v = std::nextafter(1.0f, 0.0f); break;
          case 3: v = std::nextafter(1.0f, 2.0f); break;
          case 4: v = r.Below(2) ? 1e-7f * float(r.Below(100)) : 1.0f - 1e-7f * float(r.Below(100)); break;
          default: v = Unit(r); break;
        }
      }
      break;
    case 10:  // negative and above 1
      for (float& v : a) v = Unit(r) * 3.0f - 1.0f;
      break;
    case 11:  // NaNs, infinities, denormals, -0.0 among random
      for (float& v : a) v = r.Below(3) == 0 ? Special(r) : Unit(r);
      break;
    case 12:  // mostly 1 (the solid block's test), one or two below
      a.fill(1.0f);
      for (uint32_t n = r.Below(3); n > 0; n--) a[r.Below(16)] = r.Below(2) ? Unit(r) : Byte(254, false);
      break;
    case 13:  // mostly 0 with a few others
      a.fill(0.0f);
      for (uint32_t n = 1 + r.Below(4); n > 0; n--) a[r.Below(16)] = Byte(r.Below(256), false);
      break;
    case 14:  // random bit patterns
      for (float& v : a) v = std::bit_cast<float>(uint32_t(r.Next()));
      break;
    default:  // large magnitudes
      for (float& v : a) v = (Unit(r) - 0.5f) * std::ldexp(1.0f, int(r.Below(140)));
      break;
  }
  return a;
}

int32_t Dither(Random& r, int c) {
  switch (c % 4) {
    case 0: return 0;
    case 1: return 1;
    case 2: return int32_t(0x80000000u);
    default: return int32_t(uint32_t(r.Next()) | 1);
  }
}

void WritePixels(uint8_t* base, uint32_t address, const Block16& alphas, const Block16& colors) {
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 3; c++) {
      // The color channels: one random value per pixel, scaled per channel.
      guest::StoreFloat(base, address + 16 * i + 4 * c, colors[i] * float(c + 1) / 3.0f);
    }
    guest::StoreFloat(base, address + 16 * i + 12, alphas[i]);
  }
}

void EncodeBlock(Context& h) {
  Results results("DxtAlpha::EncodeBlock (sub_82422178)");
  for (int c = 0; c < 4000; c++) {
    const int kind = c % kKinds;
    const Block16 alphas = Alphas(h.random, kind);
    Block16 colors;
    for (float& v : colors) v = Unit(h.random);
    const int32_t dither = Dither(h.random, c / kKinds);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      WritePixels(h.base, h.data + kPixels, alphas, colors);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + kBlock;
      t.r4.u64 = h.data + kPixels;
      t.r5.s64 = dither;
      if (v == 0) {
        ITC_FLAG(native_texture_compression) = false;
        __imp__sub_82422178(t, h.base);
        ITC_FLAG(native_texture_compression) = true;
      } else {
        hooks::TextureCompressionEncodeAlphaBlock(t, h.base);
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}: kind {}, dither {:X}", c, kind, uint32_t(dither)));
  }
  results.Report();
}

// The values as EncodeBlock hands them (the alphas rounded to 1/255 steps)
// or raw.
Block16 Values(Random& r, int c) {
  Block16 a = Alphas(r, c % kKinds);
  if ((c / kKinds) % 2 == 0) {
    for (float& v : a) {
      if (std::isfinite(v)) {
        v = float(int(std::fmin(std::fmax(v, -1e6f), 1e6f) * 255.0f + 0.5f)) * (1.0f / 255.0f);
      }
    }
  }
  return a;
}

void FitEndpoints(Context& h) {
  Results results("DxtAlpha::FitEndpoints (sub_824207D8)");
  for (int c = 0; c < 3000; c++) {
    const Block16 values = Values(h.random, c);
    const uint32_t steps = (c / (2 * kKinds)) % 2 == 0 ? 6 : 8;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i < 16; i++) {
        guest::StoreFloat(h.base, h.data + kValues + 4 * i, values[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + kLow;
      t.r4.u64 = h.data + kHigh;
      t.r5.u64 = h.data + kValues;
      t.r6.u64 = steps;
      if (v == 0) {
        ITC_FLAG(native_texture_compression) = false;
        __imp__sub_824207D8(t, h.base);
        ITC_FLAG(native_texture_compression) = true;
      } else {
        hooks::TextureCompressionFitAlphaEndpoints(t, h.base);
      }
      out[v].Bytes(Read(h.base, h.data, kLow));
      out[v].Floats(h.base, h.data + kLow, 2);
      out[v].Bytes(Read(h.base, h.data + kHigh + 4, kScratchBytes - kHigh - 4));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r6.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}: kind {}, steps {}", c, c % kKinds, steps));
  }
  results.Report();
}

// Rows of 16 DXT4/5 blocks through the recompiled Dxt45::EncodeBlock on one
// stack: its alpha half the original (flag off) or ours.
void Rows(Context& h) {
  Results results("Dxt45::EncodeBlock rows (sub_82422A68, the alpha half)");
  constexpr uint32_t kBlocks = 16;
  const uint32_t area = memory::Alloc(kBlocks * 256 + kBlocks * 16, 0);
  if (area == 0) {
    results.Report();
    return;
  }
  const uint32_t output = area + kBlocks * 256;
  const uint32_t saved = guest::LoadU32(h.base, kPremultiplied);
  for (int c = 0; c < 96; c++) {
    const bool premultiplied = c % 2 == 1;
    const int32_t dither = (c / 2) % 2;
    std::array<Block16, kBlocks> alphas;
    std::array<Block16, kBlocks> colors;
    for (uint32_t b = 0; b < kBlocks; b++) {
      alphas[b] = Alphas(h.random, h.random.Below(kKinds));
      for (uint32_t i = 0; i < 16; i++) {
        colors[b][i] = Unit(h.random);
        if (premultiplied && std::isfinite(alphas[b][i])) {
          colors[b][i] *= std::fmin(std::fmax(alphas[b][i], 0.0f), 1.0f);
        }
      }
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, area, kBlocks * 256 + kBlocks * 16);
      for (uint32_t b = 0; b < kBlocks; b++) {
        WritePixels(h.base, area + 256 * b, alphas[b], colors[b]);
      }
      guest::StoreU32(h.base, kPremultiplied, premultiplied ? 1 : 0);
      ITC_FLAG(native_texture_compression) = v == 1;
      PPCContext t = CallContext(h);
      for (uint32_t b = 0; b < kBlocks; b++) {
        t.r3.u64 = output + 16 * b;
        t.r4.u64 = area + 256 * b;
        t.r5.s64 = dither;
        __imp__sub_82422A68(t, h.base);
        out[v].Word(t.r3.u32);
      }
      ITC_FLAG(native_texture_compression) = true;
      out[v].Bytes(Read(h.base, area, kBlocks * 256 + kBlocks * 16));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: premultiplied {}, dither {}", c, premultiplied, dither));
  }
  guest::StoreU32(h.base, kPremultiplied, saved);
  memory::Free(area);
  results.Report();
}

}  // namespace

void TextureCompression(Context& h) {
  FitEndpoints(h);
  EncodeBlock(h);
  Rows(h);
}

}  // namespace harness
