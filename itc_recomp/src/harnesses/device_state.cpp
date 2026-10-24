// The graphics device's state setters (src/graphics/device_state.h,
// docs/functions/device-state.md) against the recompiled functions, on a
// block standing for the device (every word random, the render targets'
// and depth surface's slots, and the textures', null now and then or one of
// four surfaces in the scratch area, whose +28 has the formats the high-precision blend
// switches). The values: edge values first (0, 1, the fields' widths,
// all ones, float bits including NaNs, infinities and denormals), then
// random. Compared: the whole device block, the surfaces and r3; the float
// fields the setters store with float instructions are compared as floats
// (two NaNs match: the originals' float stores quiet a signaling NaN, the
// native ones copy its bits). The originals call nothing, so they run as
// they are (`__imp__`); the native steps turn the flush mode off as the
// adapters do.

#include "harness.h"

#include "base/log.h"
#include "graphics/device_state.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace harness {

namespace {

namespace ds = graphics::device_state;

constexpr uint32_t kDeviceBytes = 13312;  // through +13240, the present interval
constexpr uint32_t kSurfaces = 4;
constexpr uint32_t kSurfaceBytes = 64;

// The device's float fields the setters store as floats.
constexpr uint32_t kFloatFields[] = {10336, 10340, 10344, 10348, 10372, 10492, 10496,
                                     10564, 10568, 10572, 10576, 10704, 10708, 10712,
                                     10716, 11636, 11640, 11644};

uint32_t FloatBits(float f) { return std::bit_cast<uint32_t>(f); }

// The values tried first.
const uint32_t kEdgeValues[] = {
    0, 1, 2, 3, 4, 6, 7, 8, 0xF, 0x10, 0x1F, 0x87, 0xFF, 0x100, 0xFFFF, 0x10000,
    0x7FFFFFFF, 0x80000000u, 0xFFFFFFFFu, FloatBits(1.0f), FloatBits(-1.0f),
    FloatBits(2.0f), FloatBits(64.0f), FloatBits(0.5f), FloatBits(-0.0f),
    FloatBits(1e30f), FloatBits(-1e30f), FloatBits(3.0e9f), 0x7F800000, 0xFF800000u,
    0x7FC00000, 0x7F800001, 0xFFC00001u, 0x00000001, 0x80400000u, FloatBits(1.0e-3f)};

void DeviceOutputs(Context& h, uint32_t device, Outputs& out) {
  uint32_t at = 0;
  for (uint32_t field : kFloatFields) {
    out.Bytes(Read(h.base, device + at, field - at));
    out.Float(guest::LoadU32(h.base, device + field));
    at = field + 4;
  }
  out.Bytes(Read(h.base, device + at, kDeviceBytes - at));
}

struct Setter {
  const char* name;
  GuestFunction recompiled;
  GuestFunction native;
  bool sampler = false;       // r4 a sampler, r5 the value
  uint32_t values = 0;        // the values below this only (0: any)
};

#define ITC_NATIVE_STEP(address, name, uses_float)             \
  void Native##name(PPCContext& ctx, uint8_t* base) {          \
    ctx.fpscr.disableFlushMode();                              \
    ds::name(base, ctx.r3.u32, ctx.r4.u32);                    \
  }
ITC_RENDER_STATE_SETTERS(ITC_NATIVE_STEP)
#undef ITC_NATIVE_STEP

void NativeGetViewportEnable(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ds::GetViewportEnable(base, ctx.r3.u32);
}

#define ITC_SETTER_ENTRY(address, name, uses_float) \
  {"Device::" #name " (sub_" #address ")", __imp__sub_##address, Native##name},
const Setter kRenderStateSetters[] = {ITC_RENDER_STATE_SETTERS(ITC_SETTER_ENTRY)};
#undef ITC_SETTER_ENTRY

#define ITC_NATIVE_SAMPLER_STEP(address, name, uses_float)   \
  void Native##name(PPCContext& ctx, uint8_t* base) {        \
    ctx.fpscr.disableFlushMode();                            \
    ds::name(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);      \
  }
ITC_SAMPLER_STATE_SETTERS(ITC_NATIVE_SAMPLER_STEP)
#undef ITC_NATIVE_SAMPLER_STEP

// MaxAnisotropy reads the image's table by the value: values below 256
// (the table's 17 entries and the image's words after them).
#define ITC_SAMPLER_ENTRY(address, name, uses_float)                                     \
  {"Device::" #name " (sub_" #address ")", __imp__sub_##address,                        \
   Native##name, true, std::string_view(#name) == "SetMaxAnisotropy" ? 256u : 0u},
const Setter kSamplerStateSetters[] = {ITC_SAMPLER_STATE_SETTERS(ITC_SAMPLER_ENTRY)};
#undef ITC_SAMPLER_ENTRY

// Samplers past the 26 now and then (the dirty bit's shift past the mask
// from 44 on).
const uint32_t kOddSamplers[] = {26, 27, 31, 43, 44, 45, 50};

// One setter's cases on the device block.
void RunSetter(Context& h, uint32_t device, const Setter& setter, int cases) {
  Random& r = h.random;
  Results results(setter.name);
  auto surface = [&](uint32_t i) { return h.data + kSurfaceBytes * i; };
  for (int c = 0; c < cases; c++) {
    std::vector<uint32_t> words(kDeviceBytes / 4);
    for (uint32_t& w : words) {
      w = uint32_t(r.Next());
    }
    // The render targets and the depth surface: null or a surface.
    for (uint32_t slot = ds::kRenderTargets; slot <= ds::kDepthSurface; slot += 4) {
      words[slot / 4] = r.Below(3) == 0 ? 0 : surface(r.Below(kSurfaces));
    }
    // The textures (a stage's slot read by the mip levels): null or a
    // surface (their +44 random).
    for (uint32_t stage = 0; stage <= 50; stage++) {
      words[(ds::kTextures + 4 * stage) / 4] = r.Below(3) == 0 ? 0 : surface(r.Below(kSurfaces));
    }
    // Floats now and then in the float fields the setters read back.
    for (uint32_t field : {10704u, 10708u, 10712u, 10716u}) {
      const uint32_t pick = r.Below(4);
      words[field / 4] = pick == 0 ? 0 : pick == 1 ? 0x80000000u
                         : pick == 2 ? kEdgeValues[r.Below(std::size(kEdgeValues))]
                                     : words[field / 4];
    }
    std::vector<uint32_t> surfaces(kSurfaces * kSurfaceBytes / 4);
    for (uint32_t& w : surfaces) {
      w = uint32_t(r.Next());
    }
    for (uint32_t k = 0; k < kSurfaces; k++) {
      static const uint32_t kFormats[] = {2, 3, 10, 12, 2, 3, 10, 12, 0, 6};
      const uint32_t format = kFormats[r.Below(std::size(kFormats))];
      uint32_t& info = surfaces[k * kSurfaceBytes / 4 + 7];
      info = (info & ~0xF0000u) | (format << 16);
    }
    uint32_t value = size_t(c) < std::size(kEdgeValues)
                         ? kEdgeValues[c]
                         : (r.Below(2) ? uint32_t(r.Next()) : r.Below(32));
    if (setter.values != 0) {
      value %= setter.values;
    }
    const uint32_t sampler =
        r.Below(8) == 0 ? kOddSamplers[r.Below(std::size(kOddSamplers))] : r.Below(26);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      guest::StoreU32s(h.base, h.data, surfaces.data(), uint32_t(surfaces.size()));
      guest::StoreU32s(h.base, device, words.data(), uint32_t(words.size()));
      PPCContext t = CallContext(h);
      t.r3.u64 = device;
      if (setter.sampler) {
        t.r4.u64 = sampler;
        t.r5.u64 = value;
      } else {
        t.r4.u64 = value;
      }
      (v == 0 ? setter.recompiled : setter.native)(t, h.base);
      DeviceOutputs(h, device, out[v]);
      out[v].Bytes(Read(h.base, h.data, kSurfaces * kSurfaceBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1],
                  setter.sampler ? fmt::format("case {}: sampler {}, value {:08X}", c, sampler, value)
                                 : fmt::format("case {}: value {:08X}", c, value));
  }
  results.Report();
}

// --- The bound objects and the shader constants ---
//
// The scratch area holds eight objects (buffers, textures, the resources
// fenced: +0 their flags, +8 their fence, +24/+28 a buffer's address and
// size, +28-+48 a texture's fetch constant), the fence list, the constants'
// source and GetViewport's out. The device block is random but for the
// slots (null or an object), the command position (0 half the time), the
// fence mask (0, all ones or random) and the fence list (in the scratch
// area, never full: a full one runs the original, sub_822AD790 not seen in
// play). Compared: the device block, the scratch area and, for GetViewport,
// r3; the void setters' r3 isn't (the originals leave the fence list's
// position there when they queue a fence, which their callers don't read).
// The fence list entry's bit 31 is the original's uninitialized stack word
// (the native one writes 0): left out of the comparison and compared in its
// own group, expected to differ (the entry is in the command buffer, which
// nothing reads since the emulated GPU went).

constexpr uint32_t kObjects = 8;
constexpr uint32_t kObjectBytes = 64;
constexpr uint32_t kList = 1024;      // the fence list, in the scratch area
constexpr uint32_t kConstants = 2048;  // the constants' source
constexpr uint32_t kOut = 3584;        // GetViewport's out

struct BoundCase {
  std::vector<uint32_t> device;
  std::vector<uint32_t> scratch;
  uint32_t list = 0;  // the fence list's position before
};

BoundCase MakeBoundCase(Context& h, Random& r) {
  BoundCase c;
  c.device.resize(kDeviceBytes / 4);
  for (uint32_t& w : c.device) {
    w = uint32_t(r.Next());
  }
  auto object = [&] { return h.data + kObjectBytes * r.Below(kObjects); };
  for (uint32_t slot = 12180; slot < 12288 + 4 * 32; slot += 4) {  // indices, streams, textures
    c.device[slot / 4] = r.Below(3) == 0 ? 0 : object();
  }
  c.device[10780 / 4] = r.Below(2) == 0 ? 0 : uint32_t(r.Next());
  const uint32_t pick = r.Below(3);
  c.device[10784 / 4] = pick == 0 ? 0 : pick == 1 ? 0xFFFFFFFFu : uint32_t(r.Next());
  c.list = h.data + kList + 8 * r.Below(32);
  c.device[13168 / 4] = c.list;
  c.device[13172 / 4] = c.list + 8 * (1 + r.Below(4));
  c.scratch.resize(kScratchBytes / 4);
  for (uint32_t& w : c.scratch) {
    w = uint32_t(r.Next());
  }
  return c;
}

// The device block and the scratch area, the fence list entry's bit 31
// left out (in `bit31` instead).
void BoundOutputs(Context& h, uint32_t device, const BoundCase& c, Outputs& out,
                  Outputs& bit31) {
  out.Bytes(Read(h.base, device, kDeviceBytes));
  std::vector<uint8_t> scratch = Read(h.base, h.data, kScratchBytes);
  const uint32_t entry = c.list - h.data;
  bit31.Word(scratch[entry] & 0x80);
  scratch[entry] &= 0x7F;
  out.Bytes(scratch);
}

template <typename Args>
void RunBound(Context& h, uint32_t device, const char* name, GuestFunction recompiled,
              GuestFunction native, int cases, Args args, bool fences, bool compare_r3 = false) {
  Random& r = h.random;
  Results results(name);
  Results bit31_results(std::string(name) +
                        ", the fence list entry's bit 31 (expected to differ)");
  for (int i = 0; i < cases; i++) {
    BoundCase c = MakeBoundCase(h, r);
    Random args_random = r;  // the same arguments for both versions
    r.Next();
    Outputs out[2], bit31[2];
    for (int v = 0; v < 2; v++) {
      Random a = args_random;
      guest::StoreU32s(h.base, h.data, c.scratch.data(), uint32_t(c.scratch.size()));
      PPCContext t = CallContext(h);
      t.r3.u64 = device;
      args(t, a, c);
      guest::StoreU32s(h.base, device, c.device.data(), uint32_t(c.device.size()));
      (v == 0 ? recompiled : native)(t, h.base);
      BoundOutputs(h, device, c, out[v], bit31[v]);
      if (compare_r3) {
        out[v].Word(t.r3.u32);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", i));
    bit31_results.Check(bit31[0], bit31[1], fmt::format("case {}", i));
  }
  results.Report();
  if (fences) {
    bit31_results.Report();
  }
}

void NativeGetViewport(PPCContext& ctx, uint8_t* base) {
  if (!ds::GetViewport(base, ctx.r3.u32, ctx.r4.u32)) {
    ITC_LOG_INFO("harness: device_state: GetViewport fell back");
    return;
  }
  ctx.r3.u64 = ctx.r4.u32;
}
void NativeSetStreamSource(PPCContext& ctx, uint8_t* base) {
  if (!ds::SetStreamSource(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                           ctx.r8.u64)) {
    ITC_LOG_INFO("harness: device_state: SetStreamSource fell back");
  }
}
void NativeSetIndices(PPCContext& ctx, uint8_t* base) {
  if (!ds::SetIndices(base, ctx.r3.u32, ctx.r4.u32)) {
    ITC_LOG_INFO("harness: device_state: SetIndices fell back");
  }
}
void NativeSetTexture(PPCContext& ctx, uint8_t* base) {
  if (!ds::SetTexture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u64)) {
    ITC_LOG_INFO("harness: device_state: SetTexture fell back");
  }
}
void NativeSetBlendControl(PPCContext& ctx, uint8_t* base) {
  ds::SetBlendControl(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
void NativeSetVertexShaderConstantF(PPCContext& ctx, uint8_t* base) {
  ds::SetVertexShaderConstantF(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                               ctx.r7.u64);
}
void NativeSetPixelShaderConstantF(PPCContext& ctx, uint8_t* base) {
  ds::SetPixelShaderConstantF(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                              ctx.r7.u64);
}
void NativeSetPixelShaderConstantB(PPCContext& ctx, uint8_t* base) {
  ds::SetPixelShaderConstantB(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

void BoundObjects(Context& h, uint32_t device) {
  auto object = [&](Random& a) {
    return a.Below(4) == 0 ? 0 : h.data + kObjectBytes * a.Below(kObjects);
  };
  RunBound(h, device, "Device::GetViewport (sub_822A3740)", __imp__sub_822A3740,
           NativeGetViewport, 32,
           [&](PPCContext& t, Random& a, BoundCase&) { t.r4.u64 = h.data + kOut + a.Below(8); },
           false, true);
  RunBound(h, device, "Device::SetStreamSource (sub_822A3758)", __imp__sub_822A3758,
           NativeSetStreamSource, 400, [&](PPCContext& t, Random& a, BoundCase& c) {
             const uint32_t stream = a.Below(16);
             t.r4.u64 = stream;
             t.r5.u64 = object(a);
             t.r6.u64 = a.Below(2) ? a.Below(256) : uint32_t(a.Next());
             const uint32_t stride = a.Below(4) == 0 ? uint32_t(a.Next()) : 4 * a.Below(32);
             t.r7.u64 = stride;
             t.r8.u64 = a.Next();
             // The vertex shader's stride now and then the same.
             if (a.Below(2)) {
               const uint32_t at = 11568 + stream;
               uint32_t& w = c.device[at / 4];
               const int shift = 24 - 8 * int(at % 4);
               w = (w & ~(0xFFu << shift)) | (((stride >> 2) & 0xFF) << shift);
             }
           },
           true);
  RunBound(h, device, "Device::SetIndices (sub_822A3908)", __imp__sub_822A3908,
           NativeSetIndices, 200,
           [&](PPCContext& t, Random& a, BoundCase&) { t.r4.u64 = object(a); }, true);
  RunBound(h, device, "Device::SetTexture (sub_822A6008)", __imp__sub_822A6008,
           NativeSetTexture, 400, [&](PPCContext& t, Random& a, BoundCase&) {
             t.r4.u64 = a.Below(26);
             t.r5.u64 = object(a);
             t.r6.u64 = a.Next();
           },
           true);
  RunBound(h, device, "Device::SetBlendControl (sub_822A3B30)", __imp__sub_822A3B30,
           NativeSetBlendControl, 200, [&](PPCContext& t, Random& a, BoundCase&) {
             t.r4.u64 = a.Below(8) == 0 ? a.Below(200) : a.Below(4);
             t.r5.u64 = uint32_t(a.Next());
           },
           false);
  for (int pixel = 0; pixel < 2; pixel++) {
    RunBound(h, device,
             pixel ? "Device::SetPixelShaderConstantF (sub_822A8308)"
                   : "Device::SetVertexShaderConstantF (sub_822A81F8)",
             pixel ? __imp__sub_822A8308 : __imp__sub_822A81F8,
             pixel ? NativeSetPixelShaderConstantF : NativeSetVertexShaderConstantF, 300,
             [&](PPCContext& t, Random& a, BoundCase&) {
               const uint32_t count = a.Below(4) == 0 ? a.Below(4) : a.Below(32);
               t.r4.u64 = a.Below(256 - count);
               t.r5.u64 = h.data + kConstants + a.Below(16);
               t.r6.u64 = count;
               t.r7.u64 = a.Next();
             },
             false);
  }
  RunBound(h, device, "Device::SetPixelShaderConstantB (sub_822A8470)", __imp__sub_822A8470,
           NativeSetPixelShaderConstantB, 200, [&](PPCContext& t, Random& a, BoundCase&) {
             const uint32_t count = 1 + a.Below(40);
             t.r4.u64 = a.Below(256 - count);
             t.r5.u64 = h.data + kConstants + 4 * a.Below(64);
             t.r6.u64 = count;
           },
           false);
}

}  // namespace

void DeviceState(Context& h) {
  const uint32_t device = memory::Alloc(kDeviceBytes, 0);
  if (device == 0) {
    ITC_LOG_INFO("harness: device_state: no block for the device");
    return;
  }
  for (const Setter& setter : kRenderStateSetters) {
    RunSetter(h, device, setter, 64);
  }
  for (const Setter& setter : kSamplerStateSetters) {
    RunSetter(h, device, setter, 96);
  }
  RunSetter(h, device,
            {"Device::GetRenderState_ViewportEnable (sub_822A2470)", __imp__sub_822A2470,
             NativeGetViewportEnable},
            32);
  BoundObjects(h, device);
  memory::Free(device);
}

}  // namespace harness
