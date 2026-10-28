// XGraphics' surface compression (src/texture_compression/,
// docs/functions/texture-compression.md) against the recompiled functions.
// Each case writes the same inputs into the scratch area (filled with 0xCD)
// for both versions, runs the original (with --native_texture_compression
// off, so its callees are the originals too) or the plain C++ through the
// register steps (hooks/texture_compression.h for the functions whose
// adapters are kept, Native<Name> below for those whose adapters went), and
// compares with
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
// Dxt45::EncodeBlock (sub_82422A68) and EncodeBlockPremultiplied
// (sub_82422960) get such blocks with the premultiplied switch 0 and 1;
// the Rows group compares the recompiled Dxt45::EncodeBlock (flag off) with
// ours. A8R8G8B8Codec::DecodeRow (sub_823F2620) gets rows of random widths,
// pitches and bytes, with and without a color key and sRGB. DxtCodec::
// EncodeRow (sub_823F6298) runs end to end over small surfaces with codecs
// built by the recompiled DxtCodec::DxtCodec, then the DXT codec's
// destructors; the plain codecs' destructors run on codecs built by the
// recompiled ImageCodec::ImageCodec. The codecs made (ImageCodec::ImageCodec,
// Prepare, DxtCodec::DxtCodec, ImageCodec::Create over all its formats and
// some it refuses) run on descriptions with random and edge fields; the
// codecs' bytes are compared with their pointers by block.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/texture_compression.h"
#include "memory/heap.h"
#include "texture_compression/codecs.h"
#include "texture_compression/dxt45.h"
#include "texture_compression/dxt_alpha.h"
#include "texture_compression/dxt_color.h"

#include <fmt/format.h>

#include <array>
#include <bit>
#include <cmath>
#include <initializer_list>
#include <limits>
#include <string>
#include <utility>
#include <vector>

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

namespace tc = texture_compression;

// The register steps of the functions whose adapters went (they're Native,
// reached only through their native callers' plain C++): what the adapters
// did, without the flag.

// DxtAlpha::FitEndpoints sub_824207D8: r3 = the low end's float, r4 = the
// high end's, r5 = 16 floats, r6 = the steps (6 or 8; the original for any
// other count). r3 left.
void NativeFitAlphaEndpoints(PPCContext& ctx, uint8_t* base) {
  if (ctx.r6.u32 != 6 && ctx.r6.u32 != 8) {
    __imp__sub_824207D8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  float values[16];
  for (uint32_t i = 0; i < 16; i++) {
    values[i] = guest::LoadFloat(base, ctx.r5.u32 + 4 * i);
  }
  float low;
  float high;
  tc::FitAlphaEndpoints(base, values, ctx.r6.u32, &low, &high);
  guest::StoreFloat(base, ctx.r3.u32, low);
  guest::StoreFloat(base, ctx.r4.u32, high);
}

// DxtColor::PackR5G6B5 sub_824206E8: r3 = a float4 color (red, green, blue
// read). r3 = the 5:6:5 color.
void NativePackR5G6B5(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  float color[3];
  for (uint32_t i = 0; i < 3; i++) {
    color[i] = guest::LoadFloat(base, ctx.r3.u32 + 4 * i);
  }
  ctx.r3.u64 = tc::PackR5G6B5(base, color);
}

// DxtColor::FitEndpoints sub_82420AB8: r3 = the low end's three floats, r4
// the high end's, r5 = 16 float4 pixels, r6 = the colors (3 or 4; the
// original for any other count). r3, r4 and r6 left, r5 + 8 (as the
// original leaves it).
void NativeFitColorEndpoints(PPCContext& ctx, uint8_t* base) {
  if (ctx.r6.u32 != 3 && ctx.r6.u32 != 4) {
    __imp__sub_82420AB8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  float pixels[16][3];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 3; c++) {
      pixels[i][c] = guest::LoadFloat(base, ctx.r5.u32 + tc::kPixelBytes * i + 4 * c);
    }
  }
  float low[3];
  float high[3];
  tc::FitColorEndpoints(base, pixels, ctx.r6.u32, ctx.r1.u32, low, high);
  for (uint32_t c = 0; c < 3; c++) {
    guest::StoreFloat(base, ctx.r3.u32 + 4 * c, low[c]);
  }
  for (uint32_t c = 0; c < 3; c++) {
    guest::StoreFloat(base, ctx.r4.u32 + 4 * c, high[c]);
  }
  ctx.r5.s64 = ctx.r5.s64 + 8;  // the original walks the pixels from r5 + 8
}

// DxtAlpha::EncodeBlock sub_82422178: r3 = the 8-byte block, r4 = 16 float4
// pixels (their alphas at +12), r5 = dither (nonzero diffuses the error).
// r3 = 0.
void NativeEncodeAlphaBlock(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  float alphas[16];
  for (uint32_t i = 0; i < 16; i++) {
    alphas[i] = guest::LoadFloat(base, ctx.r4.u32 + tc::kPixelBytes * i + tc::kAlphaOffset);
  }
  uint8_t block[8];
  tc::EncodeAlphaBlock(base, alphas, ctx.r5.s32, block);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU8(base, ctx.r3.u32 + i, block[i]);
  }
  ctx.r3.u64 = 0;
}

// DxtColor::EncodeBlock sub_82420F50: r3 = the 8-byte block, r4 = 16 float4
// pixels, r5 = 1-bit alpha (nonzero), r6 = dither (nonzero). r6 and r3 also
// stored in their slots of the caller's frame (stw r6,1084(r1) and stw
// r7,1060(r1) past its 1040-byte frame: r1 + 44, r1 + 20) as the original
// does. r3 = 0.
void NativeEncodeColorBlock(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  guest::StoreU32(base, ctx.r1.u32 + 44, ctx.r6.u32);
  guest::StoreU32(base, ctx.r1.u32 + 20, ctx.r3.u32);
  float pixels[16][4];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 4; c++) {
      pixels[i][c] = guest::LoadFloat(base, ctx.r4.u32 + tc::kPixelBytes * i + 4 * c);
    }
  }
  uint8_t block[8];
  tc::EncodeColorBlock(base, pixels, ctx.r5.u32 != 0, ctx.r6.s32, ctx.r1.u32, block);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU8(base, ctx.r3.u32 + i, block[i]);
  }
  ctx.r3.u64 = 0;
}

// Dxt45::EncodeBlockPremultiplied sub_82422960: r3 = the 16-byte block, r4 =
// 16 float4 pixels, r5 = dither. r3 = 0.
void NativeEncodeDxt45BlockPremultiplied(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  float pixels[16][4];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 4; c++) {
      pixels[i][c] = guest::LoadFloat(base, ctx.r4.u32 + tc::kPixelBytes * i + 4 * c);
    }
  }
  uint8_t block[16];
  tc::EncodeDxt45BlockPremultiplied(base, pixels, ctx.r5.s32, ctx.r1.u32, block);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU8(base, ctx.r3.u32 + i, block[i]);
  }
  ctx.r3.u64 = 0;
}

// ImageCodec::~ImageCodec sub_823EA070 and DxtCodec::~DxtCodec sub_823EA770:
// r3 = the codec; the original where the plain C++ refuses. r3 as the
// original leaves it.
void NativeImageCodecDestruct(PPCContext& ctx, uint8_t* base) {
  const auto r3 = tc::ImageCodecDestruct(base, ctx.r3.u32);
  if (!r3) {
    __imp__sub_823EA070(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void NativeDxtCodecDestruct(PPCContext& ctx, uint8_t* base) {
  const auto r3 = tc::DxtCodecDestruct(base, ctx.r3.u32);
  if (!r3) {
    __imp__sub_823EA770(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

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
        NativeEncodeAlphaBlock(t, h.base);
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
        NativeFitAlphaEndpoints(t, h.base);
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
        NativePackR5G6B5(t, h.base);
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
        NativeFitColorEndpoints(t, h.base);
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
        NativeEncodeColorBlock(t, h.base);
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

// Rows of 16 DXT4/5 blocks on one stack: the recompiled Dxt45::EncodeBlock
// with its halves the originals (flag off) against ours.
void Rows(Context& h) {
  Results results("Dxt45::EncodeBlock rows (sub_82422A68)");
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
        if (v == 0) {
          __imp__sub_82422A68(t, h.base);
        } else {
          hooks::TextureCompressionEncodeDxt45Block(t, h.base);
        }
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


// Dxt45::EncodeBlock (sub_82422A68, the premultiplied switch 0 and 1) or
// EncodeBlockPremultiplied (sub_82422960) on single blocks: colors from
// Pixels() and FarOff(), some premultiplied, alphas from Alphas(), dither 0,
// 1, 0x80000000 and random; the caller's frame and the stack below the call
// filled with the same random words for both versions.
void EncodeDxt45(Context& h, bool premultiplied_only) {
  Results results(premultiplied_only ? "Dxt45::EncodeBlockPremultiplied (sub_82422960)"
                                     : "Dxt45::EncodeBlock (sub_82422A68)");
  constexpr uint32_t kStackBytes = 4096;
  constexpr uint32_t kFrameBytes = 64;
  constexpr int kKindsHere = kColorKinds + 1;  // Pixels()'s and FarOff()
  const uint32_t saved = guest::LoadU32(h.base, kPremultiplied);
  const int count = premultiplied_only ? 3000 : 6000;
  for (int c = 0; c < count; c++) {
    const uint32_t premultiplied = premultiplied_only ? uint32_t(h.random.Below(2)) : uint32_t(c % 2);
    const int32_t dither = Dither(h.random, (c / 2) % 4);
    const int kind = (c / 8) % kKindsHere;
    const Pixels16 colors = kind < kColorKinds ? Pixels(h.random, kind) : FarOff(h.random);
    const Block16 alphas = Alphas(h.random, int(h.random.Below(kKinds)));
    const bool scaled = h.random.Below(3) == 0;
    Pixels4 pixels;
    for (uint32_t i = 0; i < 16; i++) {
      const float a = std::isfinite(alphas[i]) ? std::fmin(std::fmax(alphas[i], 0.0f), 1.0f) : 1.0f;
      for (uint32_t ch = 0; ch < 3; ch++) pixels[i][ch] = scaled ? colors[i][ch] * a : colors[i][ch];
      pixels[i][3] = alphas[i];
    }
    std::array<uint32_t, (kStackBytes + kFrameBytes) / 4> stack;
    for (uint32_t& word : stack) word = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      WritePixels4(h.base, h.data + kPixels, pixels);
      PPCContext t = CallContext(h);
      for (uint32_t i = 0; i < stack.size(); i++) {
        guest::StoreU32(h.base, t.r1.u32 - kStackBytes + 4 * i, stack[i]);
      }
      guest::StoreU32(h.base, kPremultiplied, premultiplied);
      t.r3.u64 = h.data + kBlock;
      t.r4.u64 = h.data + kPixels;
      t.r5.s64 = dither;
      if (v == 0) {
        ITC_FLAG(native_texture_compression) = false;
        (premultiplied_only ? __imp__sub_82422960 : __imp__sub_82422A68)(t, h.base);
        ITC_FLAG(native_texture_compression) = true;
      } else if (premultiplied_only) {
        NativeEncodeDxt45BlockPremultiplied(t, h.base);
      } else {
        hooks::TextureCompressionEncodeDxt45Block(t, h.base);
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, CallContext(h).r1.u32, kFrameBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: kind {}, premultiplied {}, dither {:X}, scaled {}", c, kind,
                              premultiplied, uint32_t(dither), scaled));
  }
  guest::StoreU32(h.base, kPremultiplied, saved);
  results.Report();
}

// The codecs' fields the harness sets (texture_compression/codecs.h).
namespace cf = texture_compression::codec;

// A8R8G8B8Codec::DecodeRow (sub_823F2620) on rows of random widths (0 to
// 80), pitches, rows and slices, the bytes random or every value in turn;
// without a color key or sRGB (in play), and with them (the original's
// path on both sides).
void DecodeRow(Context& h) {
  Results results("A8R8G8B8Codec::DecodeRow (sub_823F2620)");
  constexpr uint32_t kCodec = 0;
  constexpr uint32_t kData = 256;
  constexpr uint32_t kDataBytes = 8192;
  constexpr uint32_t kOut = kData + kDataBytes;
  constexpr uint32_t kOutBytes = 80 * 16 + 64;
  constexpr uint32_t kAreaBytes = kOut + kOutBytes;
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  if (area == 0) {
    results.Report();
    return;
  }
  for (int c = 0; c < 2000; c++) {
    const uint32_t width = c % 9 == 0 ? uint32_t(h.random.Below(5)) : 1 + uint32_t(h.random.Below(80));
    const uint32_t pitch = width * 4 + 4 * uint32_t(h.random.Below(8));
    const uint32_t rows = 1 + uint32_t(h.random.Below(6));
    const uint32_t slices = 1 + uint32_t(h.random.Below(3));
    const uint32_t slice_pitch = pitch * rows + 4 * uint32_t(h.random.Below(4));
    const uint32_t row = uint32_t(h.random.Below(rows));
    const uint32_t slice = uint32_t(h.random.Below(slices));
    std::vector<uint8_t> data(kDataBytes);
    const bool every = c % 4 == 1;
    for (uint32_t i = 0; i < kDataBytes; i++) {
      data[i] = every ? uint8_t(i * 7 + c) : uint8_t(h.random.Next());
    }
    const uint32_t from = row * pitch + slice * slice_pitch;
    // A color key (one of the row's pixels, decoded) on one case in five, sRGB
    // in on one in seven.
    const bool keyed = c % 5 == 4 && width != 0;
    const bool srgb = c % 7 == 6;
    uint32_t key_bytes[4] = {};
    if (keyed) {
      const uint32_t at = from + 4 * uint32_t(h.random.Below(width));
      for (uint32_t ch = 0; ch < 4; ch++) key_bytes[ch] = data[at + (ch + 1) % 4];
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, area, kAreaBytes);
      Fill(h.base, area + kCodec, 128, 0);
      Write(h.base, area + kData, data.data(), kDataBytes);
      const uint32_t codec = area + kCodec;
      guest::StoreU32(h.base, codec + 8, 1);  // the channel kind
      guest::StoreU32(h.base, codec + cf::kSrgb, srgb ? 1 : 0);
      guest::StoreU32(h.base, codec + 20, srgb ? uint32_t(c % 2) : 0);
      guest::StoreU32(h.base, codec + cf::kColorKey, keyed ? 0xFF000000u | uint32_t(c) : 0);
      for (uint32_t ch = 0; ch < 4; ch++) {
        guest::StoreFloat(h.base, codec + 36 + 4 * ch, float(key_bytes[ch]) * (1.0f / 255.0f));
      }
      guest::StoreU32(h.base, codec + cf::kData, area + kData);
      guest::StoreU32(h.base, codec + cf::kRowPitch, pitch);
      guest::StoreU32(h.base, codec + cf::kSlicePitch, slice_pitch);
      guest::StoreU32(h.base, codec + cf::kWidth, width);
      PPCContext t = CallContext(h);
      t.r3.u64 = codec;
      t.r4.u64 = row;
      t.r5.u64 = slice;
      t.r6.u64 = area + kOut;
      if (v == 0) {
        ITC_FLAG(native_texture_compression) = false;
        __imp__sub_823F2620(t, h.base);
        ITC_FLAG(native_texture_compression) = true;
      } else {
        hooks::TextureCompressionDecodeA8R8G8B8Row(t, h.base);
      }
      out[v].Bytes(Read(h.base, area, kOut));
      out[v].Floats(h.base, area + kOut, width * 4);
      out[v].Bytes(Read(h.base, area + kOut + width * 16, kOutBytes - width * 16));
      out[v].Word(t.r3.u32 - area);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: width {}, pitch {}, row {}, slice {}, keyed {}, srgb {}", c,
                              width, pitch, row, slice, keyed, srgb));
  }
  memory::Free(area);
  results.Report();
}

// Guest addresses each side made on its own (the cache, its block rows, the
// codec, its error row), compared by which block and where in it.
class Blocks {
 public:
  void Add(uint32_t address, uint32_t size) {
    if (address == 0) return;
    for (const auto& [a, s] : blocks_) {
      if (a == address) return;
    }
    blocks_.emplace_back(address, size);
  }
  // Into `out`: 0 for null, the block's number and the offset in it, the
  // offset in the shared area, or "elsewhere".
  void Put(Outputs& out, uint32_t address, uint32_t area, uint32_t area_bytes) const {
    if (address == 0) {
      out.Word(0);
      out.Word(0);
      return;
    }
    for (uint32_t i = 0; i < blocks_.size(); i++) {
      const auto& [a, s] = blocks_[i];
      if (address >= a && address - a < (s == 0 ? 1 : s)) {
        out.Word(i + 1);
        out.Word(address - a);
        return;
      }
    }
    if (address >= area && address - area < area_bytes) {
      out.Word(0x1000);
      out.Word(address - area);
      return;
    }
    out.Word(0x2000);
    out.Word(0);
  }

 private:
  std::vector<std::pair<uint32_t, uint32_t>> blocks_;
};

// DxtCodec::EncodeRow (sub_823F6298) end to end, with the DXT codec's
// destructors (sub_823EA770, sub_823F6BC0) after: a codec built by the
// recompiled DxtCodec::DxtCodec (sub_823F5FA0) for each side from the same
// description (DXT4/5 mostly, DXT1 and DXT5A whose block encoders go
// through the guest call; surfaces of 1 to 20 pixels a side and 1 or 2
// slices, boxes the whole surface or cut anywhere, so the destination's old
// blocks are decoded; dither 0 or 1; the premultiplied switch 0 or 1; the
// rows fed in order, all of them or only some), comparing r3 after each row,
// the destination's bytes, the codec's 256 bytes and its cache (each
// entry's mask, its block row's float4s; the pointers by which block they
// are), then the destructor's r3, the vtable it leaves and which blocks it
// freed.
void EncodeRow(Context& h) {
  Results results("DxtCodec::EncodeRow (sub_823F6298) with ~DxtCodec (sub_823EA770, sub_823F6BC0)");
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("texture_compression harness: EncodeRow skipped without the native heap");
    results.Report();
    return;
  }
  constexpr uint32_t kDescription = 0;  // 84 bytes
  constexpr uint32_t kDestination = 128;
  constexpr uint32_t kDestinationBytes = 4096;
  constexpr uint32_t kRow = kDestination + kDestinationBytes;
  constexpr uint32_t kRowBytes = 20 * 16 + 64;
  constexpr uint32_t kAreaBytes = kRow + kRowBytes;
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  if (area == 0) {
    results.Report();
    return;
  }
  const uint32_t saved = guest::LoadU32(h.base, kPremultiplied);
  for (int c = 0; c < 600; c++) {
    const uint32_t format = c % 8 == 7 ? 0x1A200052u : c % 8 == 6 ? 0x1A20007Bu : 0x1A200054u;
    const uint32_t block_bytes = format == 0x1A200054u ? 16 : 8;
    const uint32_t width = 1 + uint32_t(h.random.Below(20));
    const uint32_t height = 1 + uint32_t(h.random.Below(20));
    const uint32_t depth = h.random.Below(4) == 0 ? 2 : 1;
    uint32_t box[6] = {0, 0, width, height, 0, depth};
    if (h.random.Below(3) != 0) {
      box[0] = uint32_t(h.random.Below(width));
      box[2] = box[0] + 1 + uint32_t(h.random.Below(width - box[0]));
      box[1] = uint32_t(h.random.Below(height));
      box[3] = box[1] + 1 + uint32_t(h.random.Below(height - box[1]));
      box[4] = uint32_t(h.random.Below(depth));
      box[5] = box[4] + 1 + uint32_t(h.random.Below(depth - box[4]));
    }
    const uint32_t box_width = box[2] - box[0];
    const uint32_t box_height = box[3] - box[1];
    const uint32_t across = (width + 3) / 4;
    const uint32_t down = (height + 3) / 4;
    const uint32_t pitch = across * block_bytes + 16 * uint32_t(h.random.Below(2));
    const uint32_t slice_pitch = pitch * down + 64 * uint32_t(h.random.Below(2));
    const uint32_t dither = h.random.Below(3) == 0 ? 1 : 0;
    const uint32_t premultiplied = uint32_t(h.random.Below(2));
    const uint32_t all_rows = box_height * (box[5] - box[4]);
    const uint32_t fed = h.random.Below(5) == 0 ? uint32_t(h.random.Below(all_rows + 1)) : all_rows;
    const int destructor = int(h.random.Below(3));
    std::vector<uint8_t> destination(kDestinationBytes);
    for (uint8_t& b : destination) b = uint8_t(h.random.Next());
    std::vector<float> rows(size_t(all_rows) * box_width * 4);
    const int value_kind = int(h.random.Below(4));
    for (float& f : rows) {
      f = value_kind == 3 && h.random.Below(8) == 0 ? Special(h.random)
          : value_kind == 2                         ? Byte(h.random.Below(256), false)
                                                    : Unit(h.random);
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, area, kAreaBytes);
      Write(h.base, area + kDestination, destination.data(), kDestinationBytes);
      const uint32_t d = area + kDescription;
      Fill(h.base, d, 84, 0);
      guest::StoreU32(h.base, d + 0, area + kDestination);
      guest::StoreU32(h.base, d + 4, format);
      guest::StoreU32(h.base, d + 8, pitch);
      guest::StoreU32(h.base, d + 12, slice_pitch);
      const uint32_t extent[6] = {0, 0, width, height, 0, depth};
      for (uint32_t i = 0; i < 6; i++) {
        guest::StoreU32(h.base, d + 16 + 4 * i, extent[i]);
        guest::StoreU32(h.base, d + 40 + 4 * i, box[i]);
      }
      guest::StoreU32(h.base, d + 64, dither);
      guest::StoreU32(h.base, kPremultiplied, premultiplied);
      const uint32_t codec = memory::Alloc(256, 0);
      Fill(h.base, codec, 256);
      PPCContext t = CallContext(h);
      t.r3.u64 = codec;
      t.r4.u64 = d;
      __imp__sub_823F5FA0(t, h.base);
      Blocks blocks;
      blocks.Add(codec, 256);
      blocks.Add(guest::LoadU32(h.base, codec + cf::kErrorRow), 1);
      auto load = [&](uint32_t offset) { return guest::LoadU32(h.base, codec + offset); };
      const uint32_t entries = load(cf::kBlockRows) * load(cf::kSlices);
      // The heap blocks the cache and its block rows will get, filled with
      // 0xCD first as the scratch area is: the original reads float4s of the
      // right edge's block that nothing wrote (it decodes that block into the
      // first block's), the heap's leftovers otherwise, which differ between
      // the two runs. The layer keeps its records off the guest's memory and
      // takes the first block that fits, so the same sizes made and freed
      // just before come back in the same order with their bytes.
      {
        std::vector<uint32_t> primed;
        primed.push_back(memory::Alloc(entries * 8, 0));
        for (uint32_t e = 0; e < entries + 2; e++) {
          primed.push_back(memory::Alloc(load(cf::kBlockColumns) * 256, 0));
        }
        for (uint32_t i = 0; i < primed.size(); i++) {
          Fill(h.base, primed[i], i == 0 ? entries * 8 : load(cf::kBlockColumns) * 256);
        }
        for (uint32_t block : primed) memory::Free(block);
      }
      auto note = [&]() {
        const uint32_t cache = load(cf::kCache);
        blocks.Add(cache, entries * 8);
        if (cache != 0) {
          for (uint32_t e = 0; e < entries; e++) {
            blocks.Add(guest::LoadU32(h.base, cache + 8 * e + 4), load(cf::kBlockColumns) * 256);
          }
        }
      };
      ITC_FLAG(native_texture_compression) = v == 1;
      for (uint32_t k = 0; k < fed; k++) {
        for (uint32_t i = 0; i < box_width * 4; i++) {
          guest::StoreFloat(h.base, area + kRow + 4 * i, rows[size_t(k) * box_width * 4 + i]);
        }
        PPCContext r = CallContext(h);
        r.r3.u64 = codec;
        r.r4.u64 = k % box_height;
        r.r5.u64 = k / box_height;
        r.r6.u64 = area + kRow;
        note();
        if (v == 0) {
          __imp__sub_823F6298(r, h.base);
        } else {
          hooks::TextureCompressionDxtEncodeRow(r, h.base);
        }
        note();
        blocks.Put(out[v], r.r3.u32, area, kAreaBytes);
        out[v].Word(uint32_t(r.r3.u64 >> 32));
      }
      out[v].Bytes(Read(h.base, area, kAreaBytes));
      // The codec, its pointers by block.
      for (uint32_t off = 0; off < 256; off += 4) {
        if (off == cf::kCache || off == cf::kErrorRow) {
          blocks.Put(out[v], load(off), area, kAreaBytes);
        } else {
          out[v].Word(load(off));
        }
      }
      const uint32_t cache = load(cf::kCache);
      std::vector<uint32_t> held;
      if (cache != 0) {
        for (uint32_t e = 0; e < entries; e++) {
          const uint32_t pointer = guest::LoadU32(h.base, cache + 8 * e + 4);
          out[v].Word(guest::LoadU32(h.base, cache + 8 * e));
          blocks.Put(out[v], pointer, area, kAreaBytes);
          if (pointer != 0) {
            out[v].Bytes(Read(h.base, pointer, load(cf::kBlockColumns) * 256));
            held.push_back(pointer);
          }
        }
      }
      // The destructor, and which blocks it freed.
      std::vector<uint32_t> owned = held;
      owned.push_back(cache);
      owned.push_back(load(cf::kErrorRow));
      PPCContext x = CallContext(h);
      x.r3.u64 = codec;
      x.r4.u64 = destructor == 0 ? 1 : 2;
      if (destructor == 2) {
        (v == 0 ? GuestFunction(__imp__sub_823EA770) : NativeDxtCodecDestruct)(
            x, h.base);
      } else {
        (v == 0 ? GuestFunction(__imp__sub_823F6BC0) : hooks::TextureCompressionDxtCodecDelete)(
            x, h.base);
      }
      ITC_FLAG(native_texture_compression) = true;
      blocks.Put(out[v], x.r3.u32, area, kAreaBytes);
      for (uint32_t block : owned) {
        out[v].Word(block != 0 && memory::IsHeapBlock(block) ? 1 : 0);
      }
      if (destructor != 0) {
        out[v].Word(guest::LoadU32(h.base, codec));
        memory::Free(codec);
      } else {
        out[v].Word(memory::IsHeapBlock(codec) ? 1 : 0);
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: format {:08X}, {} x {} x {}, box {} {} {} {} {} {}, dither {}, "
                              "premultiplied {}, rows {} of {}, destructor {}",
                              c, format, width, height, depth, box[0], box[1], box[2], box[3],
                              box[4], box[5], dither, premultiplied, fed, all_rows, destructor));
  }
  guest::StoreU32(h.base, kPremultiplied, saved);
  memory::Free(area);
  results.Report();
}

// The plain codecs' destructors: ImageCodec::~ImageCodec (sub_823EA070) and
// the scalar deleting destructor (sub_823F6B70, flags 1 and 0) on A8R8G8B8
// codecs built by the recompiled ImageCodec::ImageCodec (sub_823EA800), with
// and without the error row (diffusion), the palette and row blocks (+56,
// +88) made or not; r3 (by block), the vtable left and which blocks were
// freed.
void ImageCodecDestructors(Context& h) {
  Results results("ImageCodec::~ImageCodec (sub_823EA070, sub_823F6B70)");
  if (!hooks::NativeHeapActive()) {
    results.Report();
    return;
  }
  const uint32_t area = memory::Alloc(256, 0);
  if (area == 0) {
    results.Report();
    return;
  }
  for (int c = 0; c < 240; c++) {
    const int destructor = c % 3;
    const bool diffusion = (c / 3) % 2 == 1;
    const bool palette = (c / 6) % 2 == 1;
    const bool row = (c / 12) % 2 == 1;
    const uint32_t width = 1 + uint32_t(h.random.Below(64));
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, area, 256, 0);
      guest::StoreU32(h.base, area + 4, 0x18280086);
      guest::StoreU32(h.base, area + 8, width * 4);
      guest::StoreU32(h.base, area + 12, width * 4);
      const uint32_t extent[6] = {0, 0, width, 1, 0, 1};
      for (uint32_t i = 0; i < 6; i++) {
        guest::StoreU32(h.base, area + 16 + 4 * i, extent[i]);
        guest::StoreU32(h.base, area + 40 + 4 * i, extent[i]);
      }
      guest::StoreU32(h.base, area + 68, diffusion ? 1 : 0);
      const uint32_t codec = memory::Alloc(128, 0);
      Fill(h.base, codec, 128);
      PPCContext t = CallContext(h);
      t.r3.u64 = codec;
      t.r4.u64 = area;
      t.r5.u64 = 32;
      t.r6.u64 = 1;
      __imp__sub_823EA800(t, h.base);
      guest::StoreU32(h.base, codec, 0x8203B808);
      if (palette) guest::StoreU32(h.base, codec + cf::kPalette, memory::Alloc(4096, 0));
      if (row) guest::StoreU32(h.base, codec + cf::kRow, memory::Alloc(width * 16, 0));
      Blocks blocks;
      blocks.Add(codec, 128);
      const uint32_t owned[3] = {guest::LoadU32(h.base, codec + cf::kPalette),
                                 guest::LoadU32(h.base, codec + cf::kRow),
                                 guest::LoadU32(h.base, codec + cf::kErrorRow)};
      for (uint32_t b : owned) blocks.Add(b, 1);
      PPCContext x = CallContext(h);
      x.r3.u64 = codec;
      x.r4.u64 = destructor == 0 ? 1 : 0;
      ITC_FLAG(native_texture_compression) = v == 1;
      if (destructor == 2) {
        (v == 0 ? GuestFunction(__imp__sub_823EA070) : NativeImageCodecDestruct)(
            x, h.base);
      } else {
        (v == 0 ? GuestFunction(__imp__sub_823F6B70) : hooks::TextureCompressionImageCodecDelete)(
            x, h.base);
      }
      ITC_FLAG(native_texture_compression) = true;
      blocks.Put(out[v], x.r3.u32, area, 256);
      for (uint32_t b : owned) out[v].Word(b != 0 && memory::IsHeapBlock(b) ? 1 : 0);
      if (destructor != 0) {
        out[v].Word(guest::LoadU32(h.base, codec));
        memory::Free(codec);
      } else {
        out[v].Word(memory::IsHeapBlock(codec) ? 1 : 0);
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: destructor {}, diffusion {}, palette {}, row {}, width {}", c,
                              destructor, diffusion, palette, row, width));
  }
  memory::Free(area);
  results.Report();
}

// The codecs made: ImageCodec::ImageCodec (sub_823EA800), ImageCodec::Prepare
// (sub_823EAA90), DxtCodec::DxtCodec (sub_823F5FA0) and ImageCodec::Create
// (sub_823F6C60), each side run through its version (the flag off for the
// recompiled, so its callees are the originals too), on descriptions with
// random and edge fields.

namespace dd = texture_compression::description;

// ImageCodec::Create's formats (without bit 8; each is taken with it too).
constexpr uint32_t kCreateFormats[] = {
    0x04900002, 0x0800004A, 0x18280043, 0x1828004B, 0x1828004C, 0x1828004F, 0x18280086,
    0x182800B6, 0x1A20004B, 0x1A20004C, 0x1A200052, 0x1A200053, 0x1A200054, 0x1A20005A,
    0x1A20007A, 0x1A20007B, 0x1A200086, 0x1A2000B6, 0x1A202AB6, 0x1A20AA5A, 0x1A20AA86,
    0x1A220058, 0x1A22AA60, 0x1A22AAA6, 0x28000002, 0x28000058, 0x28280043, 0x28280044,
    0x2828004F, 0x28280086, 0x2A200086, 0x2A200A45, 0x2A200A86, 0x2D200099, 0x2D20AA4A,
    0x2D20AA99, 0x2D22AA9F, 0x2D22AAA5, 0x2DA2AA5E, 0x2DA2AAA4,
};
constexpr uint32_t kDxtFormats[] = {0x1A200052, 0x1A200053, 0x1A200054, 0x1A20007A, 0x1A20007B};

bool Known(uint32_t format) {
  for (uint32_t f : kCreateFormats) {
    if (f == (format & ~0x100u)) return true;
  }
  return false;
}

struct Description {
  uint32_t words[21];
};

// A random value for a flag word: 0, 1 or any.
uint32_t FlagWord(Random& r) {
  const uint32_t k = r.Below(4);
  return k < 2 ? 0 : k == 2 ? 1 : uint32_t(r.Next()) | 1;
}

// A description: the extent 1 to 300 wide (sometimes 4096 or 70,000, or
// 0), the box the whole extent, cut anywhere, at its edges, empty, or
// random words; random pitches and data; dither, diffusion and sRGB 0, 1
// or any; a color key 0 or random (`keyed`); the palette 0 or random.
// Diffusion only for boxes up to 4,096 wide (wider ones make the original
// ask operator new for more than it gives).
Description RandomDescription(Random& r, uint32_t format, bool keyed, bool sane_box = false) {
  Description d{};
  auto& w = d.words;
  w[0] = r.Below(4) == 0 ? 0 : uint32_t(r.Next());
  w[1] = format;
  const uint32_t pk = r.Below(4);
  w[2] = pk == 0 ? uint32_t(r.Next()) : pk == 1 ? 0 : 4 * (1 + r.Below(1200));
  w[3] = r.Below(4) == 0 ? uint32_t(r.Next()) : w[2] * (1 + r.Below(300));
  const uint32_t wk = r.Below(16);
  const uint32_t width = wk == 0 ? 4096 : wk == 1 && !sane_box ? 70000 : wk == 2 ? 0 : 1 + r.Below(300);
  const uint32_t height = r.Below(16) == 0 ? 0 : 1 + r.Below(300);
  const uint32_t depth = r.Below(4) == 0 ? 1 + r.Below(4) : 1;
  const uint32_t extent[6] = {0, 0, width, height, 0, depth};
  uint32_t box[6] = {0, 0, width, height, 0, depth};
  const uint32_t bk = r.Below(8);
  if (bk == 1 || bk == 2) {  // cut anywhere inside
    const uint32_t sizes[3] = {width, height, depth};
    const uint32_t lo[3] = {0, 1, 4};
    const uint32_t hi[3] = {2, 3, 5};
    for (int a = 0; a < 3; a++) {
      if (sizes[a] == 0) continue;
      box[lo[a]] = r.Below(sizes[a]);
      box[hi[a]] = box[lo[a]] + 1 + r.Below(sizes[a] - box[lo[a]]);
    }
  } else if (bk == 3) {  // at the edges: one pixel at the far corner, or empty
    box[0] = width == 0 ? 0 : width - 1;
    box[1] = height == 0 ? 0 : height - 1;
    if (r.Below(2) == 0) {
      box[2] = box[0];
      box[3] = box[1];
    }
  } else if (bk == 4 && !sane_box) {  // random words (right below left and the like)
    for (uint32_t& b : box) b = r.Below(2) == 0 ? uint32_t(r.Next()) : r.Below(400);
  }
  for (int i = 0; i < 6; i++) {
    w[4 + i] = extent[i];
    w[10 + i] = box[i];
  }
  w[16] = FlagWord(r);                                    // dither
  const uint32_t box_width = box[2] - box[0];
  w[17] = box_width <= 4096 ? FlagWord(r) : 0;            // diffusion
  w[18] = FlagWord(r);                                    // sRGB
  w[19] = keyed && r.Below(2) == 0 ? uint32_t(r.Next()) | 1 : 0;  // the color key
  w[20] = r.Below(2) == 0 ? 0 : uint32_t(r.Next());       // the palette
  return d;
}

void WriteDescription(uint8_t* base, uint32_t address, const Description& d) {
  for (uint32_t i = 0; i < 21; i++) {
    guest::StoreU32(base, address + 4 * i, d.words[i]);
  }
}

std::string DescriptionText(const Description& d) {
  std::string text;
  for (uint32_t v : d.words) text += fmt::format(" {:08X}", v);
  return text;
}

// A codec's words into `out`: the pointers at `pointers` by block, the
// color key's floats (+36 to +48) as floats, the rest exactly.
void PutCodec(Outputs& out, uint8_t* base, uint32_t codec, uint32_t bytes, const Blocks& blocks,
              std::initializer_list<uint32_t> pointers, uint32_t area, uint32_t area_bytes) {
  for (uint32_t off = 0; off < bytes; off += 4) {
    bool pointer = false;
    for (uint32_t p : pointers) pointer = pointer || p == off;
    if (pointer) {
      blocks.Put(out, guest::LoadU32(base, codec + off), area, area_bytes);
    } else if (off >= cf::kColorKeyFloats && off < cf::kColorKeyFloats + 16) {
      out.Float(guest::LoadU32(base, codec + off));
    } else {
      out.Word(guest::LoadU32(base, codec + off));
    }
  }
}

// A heap block a codec holds: whether it is one, its size and bytes; then
// freed.
void PutAndFree(Outputs& out, uint8_t* base, uint32_t block) {
  const bool heap = block != 0 && memory::IsHeapBlock(block);
  out.Word(heap ? 1 : 0);
  if (heap) {
    const uint32_t size = memory::Size(block);
    out.Word(size);
    out.Bytes(Read(base, block, size));
    memory::Free(block);
  }
}

// ImageCodec::ImageCodec on a codec in the scratch area (filled with 0xCD):
// bits 0, 8 to 128, or random; kinds 1 to 4 or random; the codec's 128
// bytes, r3, and the error row (its size and bytes).
void ImageCodecConstruct(Context& h) {
  Results results("ImageCodec::ImageCodec (sub_823EA800)");
  if (!hooks::NativeHeapActive()) {
    results.Report();
    return;
  }
  constexpr uint32_t kDescription = 0;
  constexpr uint32_t kCodec = 128;
  constexpr uint32_t kAreaBytes = 256;
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  if (area == 0) {
    results.Report();
    return;
  }
  for (int c = 0; c < 1000; c++) {
    const uint32_t fk = h.random.Below(4);
    const uint32_t format = fk == 0   ? uint32_t(h.random.Next())
                            : fk == 1 ? kCreateFormats[h.random.Below(40)] | 0x100
                                      : kCreateFormats[h.random.Below(40)];
    const Description d = RandomDescription(h.random, format, true);
    static constexpr uint32_t kBits[] = {0, 8, 16, 32, 64, 128};
    const uint32_t bk = h.random.Below(8);
    const uint32_t bits = bk < 6 ? kBits[bk] : bk == 6 ? h.random.Below(256) : uint32_t(h.random.Next());
    const uint32_t kind = h.random.Below(5) == 0 ? uint32_t(h.random.Below(7)) : 1 + h.random.Below(4);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, area, kAreaBytes);
      WriteDescription(h.base, area + kDescription, d);
      PPCContext t = CallContext(h);
      t.r3.u64 = area + kCodec;
      t.r4.u64 = area + kDescription;
      t.r5.u64 = bits;
      t.r6.u64 = kind;
      ITC_FLAG(native_texture_compression) = v == 1;
      if (v == 0) {
        __imp__sub_823EA800(t, h.base);
      } else {
        hooks::TextureCompressionImageCodecConstruct(t, h.base);
      }
      ITC_FLAG(native_texture_compression) = true;
      Blocks blocks;
      const uint32_t error_row = guest::LoadU32(h.base, area + kCodec + cf::kErrorRow);
      blocks.Add(error_row, 1);
      blocks.Put(out[v], t.r3.u32, area, kAreaBytes);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      PutCodec(out[v], h.base, area + kCodec, 128, blocks, {cf::kErrorRow}, area, kAreaBytes);
      out[v].Bytes(Read(h.base, area, kCodec));  // the description, untouched
      PutAndFree(out[v], h.base, error_row);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: bits {}, kind {}, description{}", c, bits, kind,
                              DescriptionText(d)));
  }
  memory::Free(area);
  results.Report();
}

// ImageCodec::Prepare over pairs of codecs built by the recompiled
// ImageCodec::ImageCodec: kinds 1 to 4 on each side (differing or not),
// sRGB on either, the destination's +84 left 0 or set before; both codecs'
// 128 bytes (+88 and +92 by block), r3, the row made.
void Prepare(Context& h) {
  Results results("ImageCodec::Prepare (sub_823EAA90)");
  if (!hooks::NativeHeapActive()) {
    results.Report();
    return;
  }
  constexpr uint32_t kDescriptions = 0;  // two
  constexpr uint32_t kCodecs = 256;      // two of 128
  constexpr uint32_t kAreaBytes = 512;
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  if (area == 0) {
    results.Report();
    return;
  }
  for (int c = 0; c < 800; c++) {
    Description d[2];
    uint32_t kind[2];
    for (int s = 0; s < 2; s++) {
      d[s] = RandomDescription(h.random, kCreateFormats[h.random.Below(40)], false, true);
      kind[s] = 1 + h.random.Below(4);
    }
    if (h.random.Below(3) == 0) kind[1] = kind[0];
    const uint32_t convert_from = h.random.Below(4) == 0 ? 1 + h.random.Below(4) : 0;
    const uint32_t srgb_force = h.random.Below(4);  // 0: as made, 1-3: set on one or both
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, area, kAreaBytes);
      for (int s = 0; s < 2; s++) {
        WriteDescription(h.base, area + kDescriptions + 128 * s, d[s]);
        PPCContext t = CallContext(h);
        t.r3.u64 = area + kCodecs + 128 * s;
        t.r4.u64 = area + kDescriptions + 128 * s;
        t.r5.u64 = 32;
        t.r6.u64 = kind[s];
        __imp__sub_823EA800(t, h.base);
      }
      const uint32_t destination = area + kCodecs;
      const uint32_t source = area + kCodecs + 128;
      if (convert_from != 0) guest::StoreU32(h.base, destination + cf::kConvertFrom, convert_from);
      if (srgb_force & 1) guest::StoreU32(h.base, destination + cf::kSrgb, 1);
      if (srgb_force & 2) guest::StoreU32(h.base, source + cf::kSrgb, 1);
      PPCContext t = CallContext(h);
      t.r3.u64 = destination;
      t.r4.u64 = source;
      ITC_FLAG(native_texture_compression) = v == 1;
      if (v == 0) {
        __imp__sub_823EAA90(t, h.base);
      } else {
        hooks::TextureCompressionPrepare(t, h.base);
      }
      ITC_FLAG(native_texture_compression) = true;
      Blocks blocks;
      for (uint32_t codec : {destination, source}) {
        blocks.Add(guest::LoadU32(h.base, codec + cf::kRow), 1);
        blocks.Add(guest::LoadU32(h.base, codec + cf::kErrorRow), 1);
      }
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (uint32_t codec : {destination, source}) {
        PutCodec(out[v], h.base, codec, 128, blocks, {cf::kRow, cf::kErrorRow}, area, kAreaBytes);
      }
      for (uint32_t codec : {destination, source}) {
        PutAndFree(out[v], h.base, guest::LoadU32(h.base, codec + cf::kRow));
        PutAndFree(out[v], h.base, guest::LoadU32(h.base, codec + cf::kErrorRow));
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: kinds {} {}, +84 {}, sRGB set {}, destination{}, source{}",
                              c, kind[0], kind[1], convert_from, srgb_force,
                              DescriptionText(d[0]), DescriptionText(d[1])));
  }
  memory::Free(area);
  results.Report();
}

// DxtCodec::DxtCodec over the five DXT formats (with and without bit 8)
// and now and then another format (its block fields left alone), on
// descriptions as above; the codec's 256 bytes (+92 by block), r3, the
// error row.
void DxtCodecConstruct(Context& h) {
  Results results("DxtCodec::DxtCodec (sub_823F5FA0)");
  if (!hooks::NativeHeapActive()) {
    results.Report();
    return;
  }
  constexpr uint32_t kDescription = 0;
  constexpr uint32_t kCodec = 128;
  constexpr uint32_t kAreaBytes = 384;
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  if (area == 0) {
    results.Report();
    return;
  }
  for (int c = 0; c < 800; c++) {
    const uint32_t fk = h.random.Below(10);
    const uint32_t format = fk == 0   ? kCreateFormats[h.random.Below(40)]
                            : fk == 1 ? uint32_t(h.random.Next())
                                      : kDxtFormats[h.random.Below(5)] | (fk >= 6 ? 0x100 : 0);
    const Description d = RandomDescription(h.random, format, true);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, area, kAreaBytes);
      WriteDescription(h.base, area + kDescription, d);
      PPCContext t = CallContext(h);
      t.r3.u64 = area + kCodec;
      t.r4.u64 = area + kDescription;
      ITC_FLAG(native_texture_compression) = v == 1;
      if (v == 0) {
        __imp__sub_823F5FA0(t, h.base);
      } else {
        hooks::TextureCompressionDxtCodecConstruct(t, h.base);
      }
      ITC_FLAG(native_texture_compression) = true;
      Blocks blocks;
      const uint32_t error_row = guest::LoadU32(h.base, area + kCodec + cf::kErrorRow);
      blocks.Add(error_row, 1);
      blocks.Put(out[v], t.r3.u32, area, kAreaBytes);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      PutCodec(out[v], h.base, area + kCodec, 256, blocks, {cf::kErrorRow}, area, kAreaBytes);
      out[v].Bytes(Read(h.base, area, kCodec));
      PutAndFree(out[v], h.base, error_row);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: format {:08X}, description{}", c, format,
                              DescriptionText(d)));
  }
  memory::Free(area);
  results.Report();
}

// ImageCodec::Create for each of its 80 formats (12 descriptions each) and
// formats it refuses (beside its own, random, 0): r3 (by block), the
// codec's size and bytes (+88, +92 and YUV's +128 by block), the blocks
// it holds, then all freed. A color key comes with sRGB off (with sRGB out,
// the plain codecs' slot 3 would encode through +88, still 0 then: the
// original writes at address 0).
void Create(Context& h) {
  Results results("ImageCodec::Create (sub_823F6C60)");
  if (!hooks::NativeHeapActive()) {
    results.Report();
    return;
  }
  constexpr uint32_t kDescription = 0;
  constexpr uint32_t kAreaBytes = 128;
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  if (area == 0) {
    results.Report();
    return;
  }
  std::vector<uint32_t> formats;
  for (uint32_t f : kCreateFormats) {
    for (int i = 0; i < 6; i++) {
      formats.push_back(f);
      formats.push_back(f | 0x100);
    }
  }
  for (int i = 0; i < 300; i++) {
    const uint32_t f = kCreateFormats[h.random.Below(40)];
    const uint32_t k = h.random.Below(6);
    uint32_t g = k == 0   ? f + 1
                 : k == 1 ? f - 1
                 : k == 2 ? f ^ (1u << h.random.Below(32))
                 : k == 3 ? 0
                 : k == 4 ? f | 0x200
                          : uint32_t(h.random.Next());
    if (!Known(g)) formats.push_back(g);
  }
  int c = 0;
  for (uint32_t format : formats) {
    // YUV's buffer is the box's width rounded to pairs: a sane box for those
    // (a right edge left of the left one makes the original ask operator
    // new for more than it gives).
    const uint32_t base_format = format & ~0x100u;
    const bool yuv = base_format == 0x1828004B || base_format == 0x1828004C ||
                     base_format == 0x1A20004B || base_format == 0x1A20004C;
    Description d = RandomDescription(h.random, format, true, yuv);
    if (d.words[19] != 0) d.words[18] = 0;  // no sRGB with a color key
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, area, kAreaBytes);
      WriteDescription(h.base, area + kDescription, d);
      PPCContext t = CallContext(h);
      t.r3.u64 = area + kDescription;
      ITC_FLAG(native_texture_compression) = v == 1;
      if (v == 0) {
        __imp__sub_823F6C60(t, h.base);
      } else {
        hooks::TextureCompressionCreate(t, h.base);
      }
      ITC_FLAG(native_texture_compression) = true;
      const uint32_t codec = t.r3.u32;
      Blocks blocks;
      blocks.Add(codec, codec == 0 ? 0 : memory::Size(codec));
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      if (codec == 0 || !memory::IsHeapBlock(codec)) {
        out[v].Word(codec);
      } else {
        const uint32_t size = memory::Size(codec);
        const bool yuv_codec = size == 176;
        uint32_t held[3] = {guest::LoadU32(h.base, codec + cf::kRow),
                            guest::LoadU32(h.base, codec + cf::kErrorRow),
                            yuv_codec ? guest::LoadU32(h.base, codec + cf::kYuvBuffer) : 0};
        for (uint32_t b : held) blocks.Add(b, 1);
        blocks.Put(out[v], codec, area, kAreaBytes);
        out[v].Word(size);
        if (yuv_codec) {
          PutCodec(out[v], h.base, codec, size, blocks, {cf::kRow, cf::kErrorRow, cf::kYuvBuffer},
                   area, kAreaBytes);
        } else {
          PutCodec(out[v], h.base, codec, size, blocks, {cf::kRow, cf::kErrorRow}, area,
                   kAreaBytes);
        }
        for (uint32_t b : held) PutAndFree(out[v], h.base, b);
        memory::Free(codec);
      }
      out[v].Bytes(Read(h.base, area, kAreaBytes));
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: format {:08X}, description{}", c, format,
                              DescriptionText(d)));
    c++;
  }
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
  EncodeDxt45(h, false);
  EncodeDxt45(h, true);
  Rows(h);
  DecodeRow(h);
  EncodeRow(h);
  ImageCodecDestructors(h);
  ImageCodecConstruct(h);
  Prepare(h);
  DxtCodecConstruct(h);
  Create(h);
}

}  // namespace harness
