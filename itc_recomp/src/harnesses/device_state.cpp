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
  memory::Free(device);
}

}  // namespace harness
