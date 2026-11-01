// The render queue's methods (src/graphics/render_queue.h,
// docs/functions/device-shaders.md) against the recompiled functions. The
// queue, its state, pass lists and buckets are made up in the scratch area
// over random words: the queue's vtable the real one (0x8206CB8C) or, now
// and then, one in the scratch area whose slot 7 is Replaying's address (so
// the guest call path runs). Compared: the scratch area (and the device
// block for Init), r3 and, for QueueEntry::Sequence, f1; floats the
// functions load and store as floats with harness::Outputs.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/render_queue.h"
#include "hooks/render_queue.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <cstdint>
#include <bit>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_render_queue);

namespace harness {

namespace {

namespace rq = graphics::render_queue;

// The scratch area's layout.
constexpr uint32_t kQueue = 0;
constexpr uint32_t kFakeVtable = 64;    // 10 slots
constexpr uint32_t kDrawerVtable = 112;
constexpr uint32_t kState = 128;        // 88 bytes
constexpr uint32_t kListA = 256;        // 36 bytes each
constexpr uint32_t kListB = 320;
constexpr uint32_t kBucketsA = 512;     // up to 32 buckets
constexpr uint32_t kBucketsB = 1024;
constexpr uint32_t kToggled = 2048;     // the drawer's slot 0 stand-in's word
constexpr uint32_t kResources = 3072;   // Init's render targets, 32 bytes each

constexpr uint32_t kDeviceBytes = 12288;

void NativeFlip(PPCContext& ctx, uint8_t* base) { rq::Flip(base, ctx.r3.u32); }
void NativeRecording(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rq::Recording(base, ctx.r3.u32);
}
void NativeReplaying(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rq::Replaying(base, ctx.r3.u32);
}
void NativeSequence(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(rq::Sequence(base, ctx.r3.u32));
}
void NativeClearRecording(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  hooks::NativeRenderQueueClearRecording(ctx, base);
}

uint32_t RandomFloat(Random& r) {
  switch (r.Below(10)) {
    case 0:
      return 0x7FC00000u | uint32_t(r.Next() & 0x3FFFFF);  // a NaN
    case 1:
      return 0xFF800000u;  // -infinity
    case 2:
      return 0;
    default: {
      const float f = float(int32_t(r.Below(200001)) - 100000) / 7.0f;
      return std::bit_cast<uint32_t>(f);
    }
  }
}

// The random scratch area with a queue at kQueue: its vtable (the real one
// or the fake), state and two lists with `buckets` buckets each.
std::vector<uint32_t> MakeQueue(Context& h, Random& r, bool fake_vtable, uint32_t index,
                                uint32_t buckets) {
  std::vector<uint32_t> words(kScratchBytes / 4);
  for (uint32_t& w : words) w = uint32_t(r.Next());
  auto at = [&](uint32_t offset) -> uint32_t& { return words[offset / 4]; };
  at(kQueue + 0) = fake_vtable ? h.data + kFakeVtable : rq::kVtable;
  at(kQueue + 4) = h.data + kState;
  at(kQueue + 8) = h.data + kListA;
  at(kQueue + 12) = h.data + kListB;
  at(kFakeVtable + 28) = 0x8258E618;  // slot 7: Replaying, a guest call
  at(kState + 0) = index;
  at(kListA + 12) = buckets;
  at(kListA + 24) = h.data + kBucketsA;
  at(kListB + 12) = buckets;
  at(kListB + 24) = h.data + kBucketsB;
  return words;
}

void SmallMethods(Context& h) {
  Random& r = h.random;
  struct Method {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Method methods[] = {
      {"RenderQueue::Flip (sub_8258E5E0)", __imp__sub_8258E5E0, NativeFlip},
      {"RenderQueue::Recording (sub_8258E5F8)", __imp__sub_8258E5F8, NativeRecording},
      {"RenderQueue::Replaying (sub_8258E618)", __imp__sub_8258E618, NativeReplaying},
  };
  for (const Method& m : methods) {
    Results results(m.name);
    for (int c = 0; c < 32; c++) {
      // Replaying reads +8 + 4 x the index: kept inside the queue's words.
      const uint32_t index = c < 3 ? uint32_t(c) : r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2);
      const uint32_t safe = m.native == NativeReplaying ? (index & 3) : index;
      const std::vector<uint32_t> words = MakeQueue(h, r, false, safe, 0);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32s(h.base, h.data, words.data(), uint32_t(words.size()));
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + kQueue;
        (v == 0 ? m.recompiled : m.native)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}: index {:08X}", c, safe));
    }
    results.Report();
  }
  {
    Results results("QueueEntry::Sequence (sub_8258E5C0)");
    for (int c = 0; c < 32; c++) {
      const uint32_t entry = h.data + 4 * r.Below(64);
      const uint32_t bits = RandomFloat(r);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        guest::StoreU32(h.base, entry + 8, bits);
        PPCContext t = CallContext(h);
        t.r3.u64 = entry;
        (v == 0 ? __imp__sub_8258E5C0 : NativeSequence)(t, h.base);
        out[v].Double(t.f1.u64);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}: {:08X}", c, bits));
    }
    results.Report();
  }
}

void ClearRecording(Context& h) {
  Random& r = h.random;
  Results results("RenderQueue::ClearRecording (sub_8258ED78)");
  for (int c = 0; c < 48; c++) {
    const bool fake = r.Below(4) == 0;
    const uint32_t index = c < 2 ? uint32_t(c) : r.Below(3) == 0 ? uint32_t(r.Next()) & ~3u : r.Below(2);
    // The fake slot 7 (Replaying) reads +8 + 4 x the index: 0 or 1 there.
    const uint32_t used = fake ? (index & 1) : index;
    const uint32_t buckets = r.Below(33);
    const std::vector<uint32_t> words = MakeQueue(h, r, fake, used, buckets);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, h.data, words.data(), uint32_t(words.size()));
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + kQueue;
      (v == 0 ? __imp__sub_8258ED78 : NativeClearRecording)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kState + 84));
      out[v].Float(guest::LoadU32(h.base, h.data + kState + 84));
      out[v].Bytes(Read(h.base, h.data + kState + 88, kScratchBytes - kState - 88));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: {} vtable, index {:08X}, {} buckets",
                              c, fake ? "fake" : "real", used, buckets));
  }
  results.Report();
}

void Init(Context& h) {
  Random& r = h.random;
  const uint32_t device = memory::Alloc(kDeviceBytes, 0);
  if (device == 0) {
    ITC_LOG_INFO("harness: render_queue: no block for the device");
    return;
  }
  Results results("RenderQueue::Init (sub_8258ED00)");
  for (int c = 0; c < 24; c++) {
    std::vector<uint32_t> words = MakeQueue(h, r, false, r.Below(2), 0);
    auto at = [&](uint32_t offset) -> uint32_t& { return words[offset / 4]; };
    // The drawer's slot 0 stand-in: Flip, toggling the word at its +4's.
    at(kState + 4) = h.data + kDrawerVtable;
    at(kDrawerVtable) = 0x8258E5E0;
    at(kState + 8) = h.data + kToggled;
    at(kToggled) = r.Below(2);
    std::vector<uint32_t> device_words(kDeviceBytes / 4);
    for (uint32_t& w : device_words) w = uint32_t(r.Next());
    for (uint32_t i = 0; i < 5; i++) {
      const uint32_t resource = kResources + 32 * i;
      // No type 4 holding a parent (AddRef of a count of 1 would AddRef +24).
      at(resource) &= ~0x40000000u;
      at(resource + 4) = 1 + r.Below(1000);
      device_words[(12184 + 4 * i) / 4] = r.Below(4) == 0 ? 0 : h.data + resource;
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, h.data, words.data(), uint32_t(words.size()));
      guest::StoreU32s(h.base, device, device_words.data(), uint32_t(device_words.size()));
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + kQueue;
      t.r4.u64 = device;
      (v == 0 ? __imp__sub_8258ED00 : hooks::NativeRenderQueueInit)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, device, kDeviceBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
  memory::Free(device);
}

}  // namespace

void RenderQueue(Context& h) {
  SmallMethods(h);
  ClearRecording(h);
  Init(h);
}

}  // namespace harness
