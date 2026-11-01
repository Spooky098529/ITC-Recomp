// The graphics device's shader setters and PassList::Clear
// (src/graphics/device_shaders.h, docs/functions/device-shaders.md) against
// the recompiled functions. The device is a block of random words; the
// shaders are objects in a second block: the one bound (device +12436 or
// +12432) and the new one, the same object now and then, or 0; their counts
// at +4 at least 1 (the new) and 2 (the bound), so Release never destroys.
// The new shader's patches (vertex: at +592 + its +604; pixel: at +56 + its
// +68) are made up: the patch offset 0 now and then, the masks random or 0,
// copy records (byte offsets into the shadow at device +1024, word counts
// 1 to 16) up to a count of 0 or the end, then masked records (even counts)
// up to another count of 0 or the end, the size cutting them short now and
// then. Compared: the device block, the shaders' block and r3, all exactly
// (the setters do no float work). The recompiled SetVertexShader calls
// AddRef's and Release's overrides (the lock-free count); the entry's
// recompiled version runs with --native_device_shaders off, so its tail
// call reaches the recompiled SetVertexShader with refcount.cpp's skip.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/device_shaders.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_device_shaders);

namespace harness {

namespace {

namespace dsh = graphics::device_shaders;

constexpr uint32_t kDeviceBytes = 13312;  // through +13156, the entry's copy
constexpr uint32_t kShaderBytes = 8192;   // the shaders' block
constexpr uint32_t kNewShader = 0;        // the new shader's offset in it
constexpr uint32_t kBoundShader = 4096;   // the bound one's

dsh::ShaderCalls Calls(PPCContext& ctx, uint8_t* base) {
  dsh::ShaderCalls calls;
  calls.release = [&ctx, base](uint32_t resource) {
    ctx.r3.u64 = resource;
    ctx.lr = 0x822A883C;
    sub_822A67B0(ctx, base);
    return ctx.r3.u32;
  };
  calls.memcpy = [&ctx, base](uint32_t destination, uint32_t source, uint32_t size,
                              uint32_t at) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = source;
    ctx.r5.u64 = size;
    ctx.lr = at;
    sub_825A7410(ctx, base);
  };
  return calls;
}

void NativeSetVertexShader(PPCContext& ctx, uint8_t* base) {
  const dsh::ShaderCalls calls = Calls(ctx, base);
  ctx.r3.u64 = dsh::SetVertexShader(base, ctx.r3.u32, ctx.r4.u32, calls);
}
void NativeSetVertexShaderEntry(PPCContext& ctx, uint8_t* base) {
  const dsh::ShaderCalls calls = Calls(ctx, base);
  ctx.r3.u64 = dsh::SetVertexShaderEntry(base, ctx.r3.u32, ctx.r4.u32, calls);
}
void NativeSetPixelShader(PPCContext& ctx, uint8_t* base) {
  const dsh::ShaderCalls calls = Calls(ctx, base);
  ctx.r3.u64 = dsh::SetPixelShader(base, ctx.r3.u32, ctx.r4.u32, calls);
}
void NativeSetVertexDeclaration(PPCContext& ctx, uint8_t* base) {
  dsh::SetVertexDeclaration(base, ctx.r3.u32, ctx.r4.u32);
}
void NativePassListClear(PPCContext& ctx, uint8_t* base) {
  dsh::PassListClear(base, ctx.r3.u32);
}
void RecompiledSetVertexShaderEntry(PPCContext& ctx, uint8_t* base) {
  const bool saved = ITC_FLAG(native_device_shaders);
  ITC_FLAG(native_device_shaders) = false;
  __imp__sub_822A8958(ctx, base);
  ITC_FLAG(native_device_shaders) = saved;
}

void Put32(std::vector<uint8_t>& b, uint32_t at, uint32_t v) {
  if (b.size() < at + 4) b.resize(at + 4);
  for (int i = 0; i < 4; i++) b[at + i] = uint8_t(v >> (24 - 8 * i));
}
void Put16(std::vector<uint8_t>& b, uint32_t at, uint16_t v) {
  if (b.size() < at + 2) b.resize(at + 2);
  b[at] = uint8_t(v >> 8);
  b[at + 1] = uint8_t(v);
}

// The new shader's patches, at `header` (+12 the offset to them) in `b`.
// Returns a description.
std::string MakePatches(Random& r, std::vector<uint8_t>& b, uint32_t header) {
  if (r.Below(8) == 0) {
    Put32(b, header + 12, 0);
    return "no patches";
  }
  const uint32_t offset = 16 + 4 * r.Below(16);
  Put32(b, header + 12, offset);
  const uint32_t p = header + offset;
  for (uint32_t i = 0; i < 4; i++) {
    Put32(b, p + 4 * i, r.Below(4) == 0 ? 0 : uint32_t(r.Next()));
  }
  if (r.Below(2)) {
    Put32(b, p + 8, 0);
    Put32(b, p + 12, 0);
  }
  uint32_t at = p + 24;
  const uint32_t copies = r.Below(5), masks = r.Below(4);
  const bool end_copies = r.Below(4) != 0, end_masks = r.Below(2) != 0;
  auto shadow_offset = [&] { return uint16_t(4 * r.Below(512)); };
  for (uint32_t i = 0; i < copies; i++) {
    const uint16_t count = uint16_t(1 + r.Below(16));
    Put16(b, at, shadow_offset());
    Put16(b, at + 2, count);
    at += 4;
    for (uint32_t w = 0; w < count; w++, at += 4) Put32(b, at, uint32_t(r.Next()));
  }
  if (end_copies) {
    Put16(b, at, shadow_offset());
    Put16(b, at + 2, 0);
    at += 4;
    for (uint32_t i = 0; i < masks; i++) {
      const uint16_t count = uint16_t(2 * (1 + r.Below(8)));
      Put16(b, at, shadow_offset());
      Put16(b, at + 2, count);
      at += 4;
      for (uint32_t w = 0; w < count; w++, at += 4) Put32(b, at, uint32_t(r.Next()));
    }
    if (end_masks) {
      Put16(b, at, shadow_offset());
      Put16(b, at + 2, 0);
      at += 4;
    }
  }
  uint32_t size = at - (p + 24);
  const uint32_t cut = r.Below(6);
  if (cut == 0) {
    size = 0;
  } else if (cut == 1 && size > 0) {
    size = r.Below(size);  // the end inside a record now and then
  }
  Put32(b, p + 16, size);
  // Words after the end (more records the original mustn't read).
  for (uint32_t w = 0; w < 8; w++) Put32(b, at + 4 * w, uint32_t(r.Next()));
  return fmt::format("offset {}, {} copies{}, {} masked{}, size {}", offset, copies,
                     end_copies ? " ended" : "", end_copies ? masks : 0,
                     end_copies && end_masks ? " ended" : "", size);
}

// A shader setter's cases: `patches` is the patches' header in the shader
// object (0 for none: the declaration), `bound` the device's slot.
void RunSetter(Context& h, uint32_t device, uint32_t shaders, const char* name,
               GuestFunction recompiled, GuestFunction native, uint32_t patches, uint32_t bound,
               int cases) {
  Random& r = h.random;
  Results results(name);
  for (int c = 0; c < cases; c++) {
    std::vector<uint32_t> words(kDeviceBytes / 4);
    for (uint32_t& w : words) w = uint32_t(r.Next());
    std::vector<uint8_t> block(kShaderBytes);
    for (uint8_t& byte : block) byte = uint8_t(r.Next());
    // The objects' first word: no type 4 holding a parent (AddRef's count of
    // 1 would AddRef its +24).
    Put32(block, kNewShader, uint32_t(r.Next()) & ~0x40000000u);
    Put32(block, kBoundShader, uint32_t(r.Next()) & ~0x40000000u);
    Put32(block, kNewShader + 4, 1 + r.Below(1000));
    Put32(block, kBoundShader + 4, 2 + r.Below(1000));
    std::string what = fmt::format("case {}", c);
    if (patches != 0) {
      what += ": " + MakePatches(r, block, kNewShader + patches);
    }
    block.resize(kShaderBytes);
    const uint32_t pick_new = r.Below(8), pick_bound = r.Below(8);
    const uint32_t new_shader = pick_new == 0 ? 0 : shaders + kNewShader;
    const uint32_t bound_shader = pick_bound == 0   ? 0
                                  : pick_bound == 1 ? new_shader
                                                    : shaders + kBoundShader;
    if (bound != 0) words[bound / 4] = bound_shader;
    what += fmt::format(", new {}, bound {}", new_shader ? "set" : "0",
                        bound_shader == 0 ? "0" : bound_shader == new_shader ? "same" : "other");
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      guest::StoreU32s(h.base, device, words.data(), uint32_t(words.size()));
      Write(h.base, shaders, block.data(), kShaderBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = device;
      t.r4.u64 = new_shader;
      (v == 0 ? recompiled : native)(t, h.base);
      out[v].Bytes(Read(h.base, device, kDeviceBytes));
      out[v].Bytes(Read(h.base, shaders, kShaderBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], what);
  }
  results.Report();
}

void PassLists(Context& h) {
  Random& r = h.random;
  Results results("PassList::Clear (sub_8258EB38)");
  for (int c = 0; c < 64; c++) {
    std::vector<uint32_t> words(kScratchBytes / 4);
    for (uint32_t& w : words) w = uint32_t(r.Next());
    const uint32_t list = h.data;
    const uint32_t buckets = c < 4 ? uint32_t(c) : r.Below(200);
    words[12 / 4] = buckets;
    words[24 / 4] = h.data + 64 + 4 * r.Below(16);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, h.data, words.data(), uint32_t(words.size()));
      PPCContext t = CallContext(h);
      t.r3.u64 = list;
      (v == 0 ? __imp__sub_8258EB38 : NativePassListClear)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}: {} buckets", c, buckets));
  }
  results.Report();
}

}  // namespace

void DeviceShaders(Context& h) {
  const uint32_t device = memory::Alloc(kDeviceBytes, 0);
  const uint32_t shaders = memory::Alloc(kShaderBytes, 0);
  if (device == 0 || shaders == 0) {
    ITC_LOG_INFO("harness: device_shaders: no blocks for the device and shaders");
    memory::Free(device);
    memory::Free(shaders);
    return;
  }
  RunSetter(h, device, shaders, "Device::SetVertexShader (sub_822A8808)", __imp__sub_822A8808,
            NativeSetVertexShader, 592, 12436, 200);
  RunSetter(h, device, shaders, "D3DDevice_SetVertexShader (sub_822A8958)",
            RecompiledSetVertexShaderEntry, NativeSetVertexShaderEntry, 592, 12436, 100);
  RunSetter(h, device, shaders, "Device::SetPixelShader (sub_822A8618)", __imp__sub_822A8618,
            NativeSetPixelShader, 56, 12432, 200);
  RunSetter(h, device, shaders, "Device::SetVertexDeclaration (sub_822A8960)",
            __imp__sub_822A8960, NativeSetVertexDeclaration, 0, 11552, 32);
  memory::Free(device);
  memory::Free(shaders);
  PassLists(h);
}

}  // namespace harness
