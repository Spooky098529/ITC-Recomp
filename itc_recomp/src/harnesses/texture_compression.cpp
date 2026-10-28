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
// color half would show; since batch 2a its color half reaches our
// DxtColor::FitEndpoints and PackR5G6B5 too.
//
// DxtColor::PackR5G6B5 (sub_824206E8) gets colors whose channels are random,
// bytes decoded, at the 31 and 63 steps' rounding boundaries (and the floats
// either side), and every edge (0, -0.0, 1 and the floats beside them,
// negative, above 1, NaNs, infinities, denormals, random bits).
// DxtColor::FitEndpoints (sub_82420AB8) gets blocks of 16 pixels from the
// color generators in Pixels() in both modes (3 and 4 colors), with the stack
// below the call filled with the same random words for both versions (a
// pixel far below the low end makes the original read it).
// DxtColor::EncodeBlock (sub_82420F50) gets blocks of 16 float4 pixels: the
// colors from Pixels() and FarOff(), some premultiplied (times their
// alphas), the alphas from Alphas() or around the alpha reference
// (0x8263B7E8, set to the game's value, 0.5, 0, 1, 128/255 and a NaN in
// turn, then put back), each with 1-bit alpha off and on (r5 = 1 and
// 0x80000000) and dither 0, 1, 0x80000000 and random; the stack below the
// call filled with the same random words for both versions; then blocks
// found by trying whose FitEndpoints reads a step index from the stack.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/texture_compression.h"
#include "memory/heap.h"
#include "texture_compression/dxt_alpha.h"
#include "texture_compression/dxt_color.h"

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

// A color channel at one of PackR5G6B5's rounding boundaries (where
// v x 31 or 63 + 0.5 reaches a whole number), or a float beside it.
float Boundary(Random& r) {
  const float scale = r.Below(2) == 0 ? 31.0f : 63.0f;
  const float k = float(r.Below(uint32_t(scale) + 1));
  float v;
  switch (r.Below(3)) {
    case 0: v = (k + 0.5f) / scale; break;
    case 1: v = float((double(k) + 0.5) / double(scale)); break;
    default: v = (k - 0.5f) / scale; break;
  }
  switch (r.Below(3)) {
    case 0: return v;
    case 1: return std::nextafter(v, 2.0f);
    default: return std::nextafter(v, -2.0f);
  }
}

float Channel(Random& r, int kind) {
  switch (kind) {
    case 0: return Unit(r);
    case 1: return Byte(r.Below(256), r.Below(2) == 0);
    case 2: return Boundary(r);
    case 3: return Special(r);
    case 4: return Unit(r) * 3.0f - 1.0f;
    case 5: return std::bit_cast<float>(uint32_t(r.Next()));
    default: {  // a premultiplied byte
      return Byte(r.Below(256), false) * Byte(r.Below(256), false);
    }
  }
}

void PackR5G6B5(Context& h) {
  Results results("DxtColor::PackR5G6B5 (sub_824206E8)");
  for (int c = 0; c < 4000; c++) {
    std::array<float, 4> color;
    const int kind = c % 7;
    for (float& v : color) {
      // Mostly the case's kind, some channels from the others.
      v = Channel(h.random, h.random.Below(4) == 0 ? int(h.random.Below(7)) : kind);
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreFloat(h.base, h.data + 4 * i, color[i]);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data;
      if (v == 0) {
        ITC_FLAG(native_texture_compression) = false;
        __imp__sub_824206E8(t, h.base);
        ITC_FLAG(native_texture_compression) = true;
      } else {
        hooks::TextureCompressionPackR5G6B5(t, h.base);
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: kind {}, {:08X} {:08X} {:08X}", c, kind,
                              std::bit_cast<uint32_t>(color[0]), std::bit_cast<uint32_t>(color[1]),
                              std::bit_cast<uint32_t>(color[2])));
  }
  results.Report();
}

constexpr int kColorKinds = 20;

using Pixels16 = std::array<std::array<float, 3>, 16>;

std::array<float, 3> RandomColor(Random& r) {
  return {Unit(r), Unit(r), Unit(r)};
}

std::array<float, 3> ByteColor(Random& r) {
  return {Byte(r.Below(256), false), Byte(r.Below(256), false), Byte(r.Below(256), false)};
}

// A block of 16 pixels' colors of the given kind.
Pixels16 Pixels(Random& r, int kind) {
  Pixels16 p{};
  switch (kind) {
    case 0:  // random in 0-1
      for (auto& px : p) px = RandomColor(r);
      break;
    case 1:  // bytes decoded (k * (1/255) or k / 255)
    case 2:
      for (auto& px : p) {
        for (float& v : px) v = Byte(r.Below(256), kind == 2);
      }
      break;
    case 3: {  // premultiplied: bytes times a byte alpha, often 0 or 1
      for (auto& px : p) {
        uint32_t alpha = r.Below(256);
        if (r.Below(4) == 0) alpha = r.Below(2) * 255;
        for (float& v : px) v = Byte(r.Below(256), false) * Byte(alpha, false);
      }
      break;
    }
    case 4: {  // all equal
      const auto color = r.Below(2) ? RandomColor(r) : ByteColor(r);
      p.fill(color);
      break;
    }
    case 5: {  // two colors
      const auto a = r.Below(2) ? RandomColor(r) : ByteColor(r);
      const auto b = r.Below(2) ? RandomColor(r) : ByteColor(r);
      for (auto& px : p) px = r.Below(2) ? a : b;
      break;
    }
    case 6: {  // a gray ramp
      const float start = Unit(r);
      const float step = (Unit(r) - 0.5f) / 8.0f;
      for (uint32_t i = 0; i < 16; i++) {
        const float g = start + step * float(r.Below(2) ? i : r.Below(16));
        p[i] = {g, g, g};
      }
      break;
    }
    case 7: {  // one channel varying
      const auto color = RandomColor(r);
      const uint32_t channel = r.Below(3);
      for (auto& px : p) {
        px = color;
        px[channel] = r.Below(2) ? Unit(r) : Byte(r.Below(256), false);
      }
      break;
    }
    case 8: {  // clustered: a few 1/255 steps or less apart
      const auto center = RandomColor(r);
      const float spread = float(1 + r.Below(6)) / 255.0f;
      for (auto& px : p) {
        for (uint32_t c = 0; c < 3; c++) px[c] = center[c] + (Unit(r) - 0.5f) * spread;
      }
      break;
    }
    case 9: {  // a gradient between two colors, with noise (a picture's)
      const auto a = RandomColor(r);
      const auto b = RandomColor(r);
      const float noise = float(r.Below(4)) / 64.0f;
      for (uint32_t i = 0; i < 16; i++) {
        const float t = float((i & 3) + (i >> 2)) / 6.0f;
        for (uint32_t c = 0; c < 3; c++) {
          const float v = a[c] + (b[c] - a[c]) * t + (Unit(r) - 0.5f) * noise;
          p[i][c] = Byte(uint32_t(std::fmin(std::fmax(v, 0.0f), 1.0f) * 255.0f + 0.5f), false);
        }
      }
      break;
    }
    case 10: {  // bytes in a narrow window, often touching 0 or 255
      const uint32_t width = 1 + r.Below(24);
      uint32_t low = r.Below(256 - width);
      if (r.Below(4) == 0) low = 0;
      if (r.Below(4) == 0) low = 255 - width;
      for (auto& px : p) {
        for (float& v : px) v = Byte(low + r.Below(width + 1), false);
      }
      break;
    }
    case 11:  // NaNs, infinities, denormals, -0.0 among random
      for (auto& px : p) {
        for (float& v : px) v = r.Below(4) == 0 ? Special(r) : Unit(r);
      }
      break;
    case 12:  // negative and above 1
      for (auto& px : p) {
        for (float& v : px) v = Unit(r) * 3.0f - 1.0f;
      }
      break;
    case 13:  // random bit patterns
      for (auto& px : p) {
        for (float& v : px) v = std::bit_cast<float>(uint32_t(r.Next()));
      }
      break;
    case 14: {  // three or four colors
      std::array<std::array<float, 3>, 4> palette;
      for (auto& color : palette) color = r.Below(2) ? RandomColor(r) : ByteColor(r);
      const uint32_t count = 3 + r.Below(2);
      for (auto& px : p) px = palette[r.Below(count)];
      break;
    }
    case 15:  // large magnitudes
      for (auto& px : p) {
        for (float& v : px) v = (Unit(r) - 0.5f) * std::ldexp(1.0f, int(r.Below(140)));
      }
      break;
    case 16:  // the box's corners (0 and 1 in each channel)
      for (auto& px : p) {
        for (float& v : px) v = float(r.Below(2));
      }
      break;
    case 17: {  // a cluster with one or two pixels far off
      const auto center = RandomColor(r);
      for (auto& px : p) {
        for (uint32_t c = 0; c < 3; c++) px[c] = center[c] + (Unit(r) - 0.5f) / 32.0f;
      }
      for (uint32_t n = 1 + r.Below(2); n > 0; n--) p[r.Below(16)] = RandomColor(r);
      break;
    }
    case 18: {  // most pixels at one end, a few spread toward the other
      const auto a = RandomColor(r);
      const auto b = RandomColor(r);
      for (auto& px : p) {
        const float t = r.Below(4) == 0 ? Unit(r) : 0.0f;
        for (uint32_t c = 0; c < 3; c++) px[c] = a[c] + (b[c] - a[c]) * t;
      }
      break;
    }
    default: {  // tiny ranges near the smallest extents (denormal steps)
      const auto center = RandomColor(r);
      const float spread = std::ldexp(1.0f, -int(10 + r.Below(120)));
      for (auto& px : p) {
        for (uint32_t c = 0; c < 3; c++) px[c] = center[c] + (Unit(r) - 0.5f) * spread;
      }
      break;
    }
  }
  return p;
}

// A cluster with a few pixels far off in other directions: the blocks where a
// pixel can end up more than half a step below the low end (a negative step
// index; rare, mostly with 3 colors).
Pixels16 FarOff(Random& r) {
  const auto center = RandomColor(r);
  const float spread = std::ldexp(1.0f, -int(3 + r.Below(4)));
  Pixels16 p;
  for (auto& px : p) {
    if (r.Below(5) == 0) {
      px = RandomColor(r);
    } else {
      for (uint32_t c = 0; c < 3; c++) px[c] = center[c] + (Unit(r) - 0.5f) * spread;
    }
  }
  return p;
}

// The pixels as DxtColor::EncodeBlock hands them: each channel times the
// color weights at 0x82649B10 (fmuls).
void ApplyWeights(uint8_t* base, Pixels16& p) {
  for (auto& px : p) {
    for (uint32_t c = 0; c < 3; c++) px[c] = guest::LoadFloat(base, 0x82649B10 + 4 * c) * px[c];
  }
}

void FitColorEndpoints(Context& h) {
  Results results("DxtColor::FitEndpoints (sub_82420AB8)");
  constexpr uint32_t kColorLow = 256;
  constexpr uint32_t kColorHigh = 272;
  constexpr uint32_t kStackBytes = 2048;  // filled below the call's stack
  auto run = [&](const Pixels16& pixels, uint32_t colors, const std::string& what) {
    std::array<float, 16> w;
    for (float& v : w) v = Unit(h.random);
    std::array<uint32_t, kStackBytes / 4> stack;
    for (uint32_t& word : stack) word = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i < 16; i++) {
        for (uint32_t ch = 0; ch < 3; ch++) {
          guest::StoreFloat(h.base, h.data + kPixels + 16 * i + 4 * ch, pixels[i][ch]);
        }
        guest::StoreFloat(h.base, h.data + kPixels + 16 * i + 12, w[i]);
      }
      PPCContext t = CallContext(h);
      for (uint32_t i = 0; i < stack.size(); i++) {
        guest::StoreU32(h.base, t.r1.u32 - kStackBytes + 4 * i, stack[i]);
      }
      t.r3.u64 = h.data + kColorLow;
      t.r4.u64 = h.data + kColorHigh;
      t.r5.u64 = h.data + kPixels;
      t.r6.u64 = colors;
      if (v == 0) {
        ITC_FLAG(native_texture_compression) = false;
        __imp__sub_82420AB8(t, h.base);
        ITC_FLAG(native_texture_compression) = true;
      } else {
        hooks::TextureCompressionFitColorEndpoints(t, h.base);
      }
      out[v].Bytes(Read(h.base, h.data, kColorLow));
      out[v].Floats(h.base, h.data + kColorLow, 3);
      out[v].Bytes(Read(h.base, h.data + kColorLow + 12, 4));
      out[v].Floats(h.base, h.data + kColorHigh, 3);
      out[v].Bytes(Read(h.base, h.data + kColorHigh + 12, kScratchBytes - kColorHigh - 12));
      out[v].Word(t.r3.u32);
      out[v].Word(t.r4.u32);
      out[v].Word(t.r5.u32);
      out[v].Word(t.r6.u32);
    }
    results.Check(out[0], out[1], what);
  };
  for (int c = 0; c < 6000; c++) {
    const int kind = c % kColorKinds;
    const uint32_t colors = (c / kColorKinds) % 2 == 0 ? 4 : 3;
    const bool weighted = (c / (2 * kColorKinds)) % 2 == 0;
    Pixels16 pixels = Pixels(h.random, kind);
    if (weighted) ApplyWeights(h.base, pixels);
    run(pixels, colors, fmt::format("case {}: kind {}, colors {}, weighted {}", c, kind, colors, weighted));
  }
  // The blocks with a step index outside the steps: far-off pixels tried with
  // the plain C++ alone until 64 read the stack (at most 1,000,000 tries), each
  // then a case.
  uint64_t found = 0;
  uint32_t tries = 0;
  for (; tries < 1000000 && found < 64; tries++) {
    const uint32_t colors = h.random.Below(4) == 0 ? 4 : 3;
    Pixels16 pixels = FarOff(h.random);
    ApplyWeights(h.base, pixels);
    float low[3];
    float high[3];
    const uint64_t before = texture_compression::FitColorEndpointsStackReads();
    texture_compression::FitColorEndpoints(h.base, reinterpret_cast<const float(*)[3]>(pixels.data()),
                                           colors, CallContext(h).r1.u32, low, high);
    if (texture_compression::FitColorEndpointsStackReads() != before) {
      run(pixels, colors, fmt::format("far off {}: try {}, colors {}", found, tries, colors));
      found++;
    }
  }
  ITC_LOG_INFO("texture_compression harness: DxtColor::FitEndpoints: {} of {} far-off tries read "
               "the stack",
               found, tries);
  results.Report();
}

// DxtColor::EncodeBlock's alpha reference (in .data).
constexpr uint32_t kAlphaReference = 0x8263B7E8;

// 16 alphas around the alpha reference: exactly it, the floats beside it,
// a little or far below or above, all below, or the generators' kinds.
Block16 AlphasAround(Random& r, float reference) {
  Block16 a{};
  const uint32_t mode = r.Below(4);
  for (float& v : a) {
    uint32_t pick = r.Below(6);
    if (mode == 0) pick = 4;  // all below
    switch (pick) {
      case 0: v = reference; break;
      case 1: v = std::nextafter(reference, -2.0f); break;
      case 2: v = std::nextafter(reference, 2.0f); break;
      case 3: v = reference + (Unit(r) - 0.5f) / 64.0f; break;
      case 4: v = r.Below(2) ? reference - 1.0f - Unit(r) : std::nextafter(reference, -2.0f); break;
      default: v = Unit(r); break;
    }
  }
  if (mode == 1) a[r.Below(16)] = reference + 1.0f;  // one above among many below
  return a;
}

using Pixels4 = std::array<std::array<float, 4>, 16>;

void WritePixels4(uint8_t* base, uint32_t address, const Pixels4& p) {
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 4; c++) guest::StoreFloat(base, address + 16 * i + 4 * c, p[i][c]);
  }
}

void EncodeColorBlock(Context& h) {
  Results results("DxtColor::EncodeBlock (sub_82420F50)");
  constexpr uint32_t kStackBytes = 4096;  // filled below the call's stack
  constexpr uint32_t kFrameBytes = 64;    // the caller's frame (r6's and r3's slots)
  const uint32_t saved = guest::LoadU32(h.base, kAlphaReference);
  const std::array<uint32_t, 6> references = {saved, 0x3F000000, 0x00000000, 0x3F800000,
                                              0x3F008081, 0x7FC00000};
  {
    std::string tables;
    for (uint32_t i = 0; i < 7; i++) {
      tables += fmt::format(" {:X}", guest::LoadU32(h.base, 0x8203EAE8 + 4 * i));
    }
    ITC_LOG_INFO("texture_compression harness: alpha reference {:08X}, index tables at 8203EAE8:{}",
                 saved, tables);
  }
  auto run = [&](const Pixels4& pixels, uint32_t one_bit, int32_t dither, const std::string& what) {
    std::array<uint32_t, kStackBytes / 4> stack;
    for (uint32_t& word : stack) word = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      WritePixels4(h.base, h.data + kPixels, pixels);
      PPCContext t = CallContext(h);
      Fill(h.base, t.r1.u32, kFrameBytes);
      for (uint32_t i = 0; i < stack.size(); i++) {
        guest::StoreU32(h.base, t.r1.u32 - kStackBytes + 4 * i, stack[i]);
      }
      t.r3.u64 = h.data + kBlock;
      t.r4.u64 = h.data + kPixels;
      t.r5.u64 = one_bit;
      t.r6.s64 = dither;
      if (v == 0) {
        ITC_FLAG(native_texture_compression) = false;
        __imp__sub_82420F50(t, h.base);
        ITC_FLAG(native_texture_compression) = true;
      } else {
        hooks::TextureCompressionEncodeColorBlock(t, h.base);
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, CallContext(h).r1.u32, kFrameBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], what);
  };
  constexpr int kKindsHere = kColorKinds + 1;  // Pixels()'s and FarOff()
  for (int c = 0; c < 8000; c++) {
    const uint32_t reference = references[(c / (4 * kKindsHere)) % references.size()];
    guest::StoreU32(h.base, kAlphaReference, reference);
    const int kind = c % kKindsHere;
    const uint32_t one_bit = (c / kKindsHere) % 2 == 0 ? 0 : (c / (2 * kKindsHere)) % 2 == 0 ? 1 : 0x80000000u;
    const int32_t dither = Dither(h.random, int(h.random.Below(4)));
    const Pixels16 colors = kind < kColorKinds ? Pixels(h.random, kind) : FarOff(h.random);
    const Block16 alphas = h.random.Below(2) ? AlphasAround(h.random, std::bit_cast<float>(reference))
                                             : Alphas(h.random, int(h.random.Below(kKinds)));
    const bool premultiplied = h.random.Below(3) == 0;
    Pixels4 pixels;
    for (uint32_t i = 0; i < 16; i++) {
      const float a = std::isfinite(alphas[i]) ? std::fmin(std::fmax(alphas[i], 0.0f), 1.0f) : 1.0f;
      for (uint32_t ch = 0; ch < 3; ch++) pixels[i][ch] = premultiplied ? colors[i][ch] * a : colors[i][ch];
      pixels[i][3] = alphas[i];
    }
    run(pixels, one_bit, dither,
        fmt::format("case {}: kind {}, 1-bit {:X}, dither {:X}, reference {:08X}, premultiplied {}", c,
                    kind, one_bit, uint32_t(dither), reference, premultiplied));
  }
  // The blocks whose FitEndpoints gets a step index outside the steps: far-off
  // pixels tried with the plain C++ alone until 64 read the stack (at most
  // 1,000,000 tries), each then a case.
  guest::StoreU32(h.base, kAlphaReference, 0x3F000000);
  uint64_t found = 0;
  uint32_t tries = 0;
  for (; tries < 1000000 && found < 64; tries++) {
    const Pixels16 colors = FarOff(h.random);
    const bool one_bit = h.random.Below(4) != 0;
    const int32_t dither = int32_t(h.random.Below(2));
    Pixels4 pixels;
    for (uint32_t i = 0; i < 16; i++) {
      for (uint32_t ch = 0; ch < 3; ch++) pixels[i][ch] = colors[i][ch];
      pixels[i][3] = 1.0f;
    }
    for (uint32_t n = h.random.Below(4); n > 0; n--) pixels[h.random.Below(16)][3] = 0.0f;
    uint8_t block[8];
    const uint64_t before = texture_compression::FitColorEndpointsStackReads();
    texture_compression::EncodeColorBlock(h.base, reinterpret_cast<const float(*)[4]>(pixels.data()),
                                          one_bit, dither, CallContext(h).r1.u32, block);
    if (texture_compression::FitColorEndpointsStackReads() != before) {
      run(pixels, one_bit ? 1 : 0, dither,
          fmt::format("far off {}: try {}, 1-bit {}, dither {}", found, tries, one_bit, dither));
      found++;
    }
  }
  ITC_LOG_INFO("texture_compression harness: DxtColor::EncodeBlock: {} of {} far-off tries read "
               "the stack",
               found, tries);
  guest::StoreU32(h.base, kAlphaReference, saved);
  results.Report();
}

// Rows of 16 DXT4/5 blocks through the recompiled Dxt45::EncodeBlock on one
// stack: its alpha and color halves the originals (flag off) or ours.
void Rows(Context& h) {
  Results results("Dxt45::EncodeBlock rows (sub_82422A68, the alpha and color halves)");
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
  PackR5G6B5(h);
  FitColorEndpoints(h);
  EncodeColorBlock(h);
  Rows(h);
}

}  // namespace harness
