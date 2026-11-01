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
#include "hooks/models.h"
#include "hooks/render_queue.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <cstdint>
#include <bit>
#include <limits>
#include <utility>
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

// --- RenderQueue::Add and PassList::Add ---

constexpr uint32_t kItemsA = 1536;  // up to 25 items of 20 bytes each
constexpr uint32_t kItemsB = 2560;
constexpr uint32_t kMaxItems = 25;

void NativePassListAdd(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      rq::PassListAdd(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r6.u32, ctx.r1.u32);
}
void NativeAdd(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  hooks::NativeRenderQueueAdd(ctx, base);
}

// A pass list at `list` in `words` with its items at `items` (a count of
// up to kMaxItems, some used) and its buckets' links: random words, the
// buckets' first and last 0 or items.
void MakeItems(Context& h, Random& r, std::vector<uint32_t>& words, uint32_t list, uint32_t items,
               uint32_t buckets_at) {
  auto at = [&](uint32_t offset) -> uint32_t& { return words[offset / 4]; };
  const uint32_t count = r.Below(kMaxItems + 1);
  at(list + 8) = count;
  at(list + 16) = r.Below(4) == 0 ? count : r.Below(count + 1);
  at(list + 20) = h.data + items;
  for (uint32_t b = 0; b < at(list + 12); b++) {
    const uint32_t bucket = buckets_at + 16 * b;
    if (r.Below(2) == 0) {
      at(bucket + 0) = 0;
    } else {
      at(bucket + 0) = h.data + items + 20 * r.Below(kMaxItems);
      at(bucket + 4) = h.data + items + 20 * r.Below(kMaxItems);
    }
    at(bucket + 12) = r.Below(100);
  }
}

// The scratch area by role: the floats the functions store (the state's
// sequence, the items' keys and sequences) as floats, the rest exactly.
void ScratchOutputs(uint8_t* base, uint32_t data, Outputs& out) {
  std::vector<uint8_t> bytes = Read(base, data, kScratchBytes);
  std::vector<uint32_t> floats = {kState + 84};
  for (uint32_t items : {kItemsA, kItemsB}) {
    for (uint32_t i = 0; i < kMaxItems; i++) {
      floats.push_back(items + 20 * i + 8);
      floats.push_back(items + 20 * i + 12);
    }
  }
  for (uint32_t f : floats) {
    out.Float(guest::LoadU32(base, data + f));
    for (int b = 0; b < 4; b++) bytes[f + b] = 0;
  }
  out.Bytes(bytes);
}

void Adds(Context& h) {
  Random& r = h.random;
  {
    Results results("PassList::Add (sub_8258EB88)");
    for (int c = 0; c < 64; c++) {
      std::vector<uint32_t> words = MakeQueue(h, r, false, 0, r.Below(33));
      MakeItems(h, r, words, kListA, kItemsA, kBucketsA);
      const uint32_t bucket = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(34);
      const uint64_t chunk_key = (uint64_t(uint32_t(r.Next())) << 32) | RandomFloat(r);
      const uint64_t sequence = (uint64_t(RandomFloat(r)) << 32) | uint32_t(r.Next());
      // The caller's frame, where PassList::Add stores r4 and r5.
      const uint32_t sp = h.data - 1024 - 144;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32s(h.base, h.data, words.data(), uint32_t(words.size()));
        Fill(h.base, sp, 144);
        PPCContext t = CallContext(h);
        t.r1.u64 = sp;
        t.r3.u64 = h.data + kListA;
        t.r4.u64 = chunk_key;
        t.r5.u64 = sequence;
        t.r6.u64 = bucket;
        (v == 0 ? __imp__sub_8258EB88 : NativePassListAdd)(t, h.base);
        ScratchOutputs(h.base, h.data, out[v]);
        out[v].Bytes(Read(h.base, sp + 32, 16));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}: bucket {}", c, bucket));
    }
    results.Report();
  }
  {
    Results results("RenderQueue::Add (sub_8258EDC8)");
    for (int c = 0; c < 96; c++) {
      const bool fake = r.Below(6) == 0;
      const uint32_t index = r.Below(2);
      std::vector<uint32_t> words = MakeQueue(h, r, fake, index, r.Below(33));
      MakeItems(h, r, words, kListA, kItemsA, kBucketsA);
      MakeItems(h, r, words, kListB, kItemsB, kBucketsB);
      words[(kState + 84) / 4] = RandomFloat(r);
      const uint32_t chunk = r.Below(6) == 0 ? 0 : uint32_t(r.Next());
      // A bucket above 2,056 or a key past +-10,000.0 now and then (the
      // warning's path: the original in both).
      const uint32_t bucket = r.Below(10) == 0 ? 2057 + r.Below(100) : r.Below(34);
      double key = std::bit_cast<float>(RandomFloat(r));
      if (r.Below(10) == 0) key = (r.Below(2) ? 1.0 : -1.0) * (10000.0 + r.Below(3));
      std::vector<uint8_t> frame(144);
      for (uint8_t& b : frame) b = uint8_t(r.Next());
      const uint32_t stack = h.data - 1024;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32s(h.base, h.data, words.data(), uint32_t(words.size()));
        Write(h.base, stack - 144, frame.data(), 144);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + kQueue;
        t.r4.u64 = chunk;
        t.r5.u64 = bucket;
        t.f1.f64 = key;
        (v == 0 ? __imp__sub_8258EDC8 : NativeAdd)(t, h.base);
        ScratchOutputs(h.base, h.data, out[v]);
        // Add's frame: what PassList::Add stores at +32 and +40, Add's own
        // {chunk, key} and {sequence, the leftover} at +80 and +88.
        const uint32_t f = stack - 144;
        out[v].Word(guest::LoadU32(h.base, f + 32));
        out[v].Float(guest::LoadU32(h.base, f + 36));
        out[v].Float(guest::LoadU32(h.base, f + 40));
        out[v].Word(guest::LoadU32(h.base, f + 44));
        out[v].Word(guest::LoadU32(h.base, f + 80));
        out[v].Float(guest::LoadU32(h.base, f + 84));
        out[v].Float(guest::LoadU32(h.base, f + 88));
        out[v].Word(guest::LoadU32(h.base, f + 92));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: {} vtable, chunk {:08X}, bucket {}, key {}", c,
                                fake ? "fake" : "real", chunk, bucket, key));
    }
    results.Report();
  }
}

// --- PtrVector::PushBack and Insert ---
//
// The vector at the scratch area's +0 (+4 first, +8 last, +12 end), the
// value at +64. With room, the storage is in the scratch area and the
// whole area is compared. Without (an empty or full vector: Insert
// reallocates through operator new and delete), the storage comes from
// operator new for each version, and the vector is compared by role: its
// size, capacity, elements, the answer as an offset from first, and r3.

constexpr uint32_t kVector = 0;
constexpr uint32_t kValue = 64;
constexpr uint32_t kStorage = 256;  // up to 64 pointers

void NativePushBack(PPCContext& ctx, uint8_t* base) {
  const auto r3 = rq::PushBack(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, true);
  if (!r3) {
    __imp__sub_82591540(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void VectorOutputs(uint8_t* base, uint32_t vector, uint32_t answer, Outputs& out) {
  const uint32_t first = guest::LoadU32(base, vector + 4);
  const uint32_t last = guest::LoadU32(base, vector + 8);
  const uint32_t end = guest::LoadU32(base, vector + 12);
  out.Word(first != 0);
  out.Word(last - first);
  out.Word(end - first);
  if (first != 0) out.Bytes(Read(base, first, last - first));
  out.Word(guest::LoadU32(base, answer) - first);
}

void PushBacks(Context& h) {
  Random& r = h.random;
  Results results("PtrVector::PushBack (sub_82591540)");
  const bool allocate = hooks::ModelsNativeLifetime();
  if (!allocate) {
    ITC_LOG_INFO("harness: render_queue: PushBack's Insert cases need --native_models and the "
                 "native heap");
  }
  for (int c = 0; c < 64; c++) {
    const uint32_t kind = allocate ? r.Below(3) : 0;  // 0 room, 1 empty, 2 full
    const uint32_t size = kind == 1 ? 0 : r.Below(32);
    const uint32_t capacity = kind == 0 ? size + 1 + r.Below(32) : size;
    std::vector<uint32_t> words(kScratchBytes / 4);
    for (uint32_t& w : words) w = uint32_t(r.Next());
    std::vector<uint32_t> elements(size);
    for (uint32_t& e : elements) e = uint32_t(r.Next());
    const uint32_t value = h.data + kValue + 4 * r.Below(4);
    const uint32_t stack = h.data - 1024;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, h.data, words.data(), uint32_t(words.size()));
      uint32_t first = 0;
      if (kind == 0) {
        first = h.data + kStorage;
      } else if (kind == 2 && capacity != 0) {
        first = memory::crt::OperatorNew(h.base, capacity * 4).value_or(0);
      }
      if (first != 0 && size != 0) {
        guest::StoreU32s(h.base, first, elements.data(), size);
      }
      guest::StoreU32(h.base, h.data + kVector + 4, first);
      guest::StoreU32(h.base, h.data + kVector + 8, first == 0 ? 0 : first + 4 * size);
      guest::StoreU32(h.base, h.data + kVector + 12, first == 0 ? 0 : first + 4 * capacity);
      Fill(h.base, stack - 96, 96);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + kVector;
      t.r4.u64 = value;
      (v == 0 ? __imp__sub_82591540 : NativePushBack)(t, h.base);
      if (kind == 0) {
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      } else {
        VectorOutputs(h.base, h.data + kVector, stack - 96 + 80, out[v]);
        memory::crt::OperatorDelete(guest::LoadU32(h.base, h.data + kVector + 4));
      }
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: {}, size {}, capacity {}", c,
                              kind == 0 ? "room" : kind == 1 ? "empty" : "full", size, capacity));
  }
  results.Report();
}

// --- The sorts: Partition and QuickSort ---
//
// A 64 KB block from the allocation layer holds the world: an array of
// pointers to entries whose keys come from a few values (runs of equal
// keys), never NaN: the original never ends on a NaN key, as the native
// one. ChunkChain::Run and Replay have no cases: the replay dispatches to
// our own command handlers (src/render/commands.cpp, the conventional
// renderer, docs/conventional-renderer.md), which aren't compared with the
// recompiled ones (its decision 10).

constexpr uint32_t kWorldBytes = 65536;
constexpr uint32_t kWArray = 4800;            // Partition's array: 40 pointers
constexpr uint32_t kWEntries = 5120;          // its entries: 40 of 64 bytes

constexpr uint32_t kFloat48Getter = 0x820B68A0;  // RigidBody's float at +48: a guest call


struct World {
  uint32_t b = 0;
  std::vector<uint32_t> words = std::vector<uint32_t>(kWorldBytes / 4);
  uint32_t& at(uint32_t offset) { return words[offset / 4]; }
};

// The world's words random.
World MakeWorld(Random& r, uint32_t block) {
  World w;
  w.b = block;
  for (uint32_t& word : w.words) word = uint32_t(r.Next());
  return w;
}

float KeyValue(Random& r) {
  const float keys[] = {-1.0f, 0.0f, 1.0f, 2.5f, 1e30f, -std::numeric_limits<float>::infinity(),
                        std::numeric_limits<float>::infinity(), -0.0f};
  return keys[r.Below(8)];
}

void NativePartition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = uint64_t(int64_t(rq::Partition(base, ctx.r3.u32, ctx.r4.s32, ctx.r5.s32,
                                              ctx.r6.u32, ctx.r1.u32,
                                              hooks::RenderQueueCall(ctx, base))));
}
void NativeQuickSort(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rq::QuickSort(base, ctx.r3.u32, ctx.r4.s32, ctx.r5.s32, ctx.r6.u32, ctx.r1.u32,
                hooks::RenderQueueCall(ctx, base));
}
void Sorts(Context& h, uint32_t block) {
  Random& r = h.random;
  struct Sort {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Sort sorts[] = {
      {"RenderQueue::Partition (sub_8258EC38)", __imp__sub_8258EC38, NativePartition},
      {"RenderQueue::QuickSort (sub_82590CB0)", __imp__sub_82590CB0, NativeQuickSort},
  };
  const uint32_t getters[] = {0x820B6708, 0x8258E5C0, kFloat48Getter};
  for (const Sort& sort : sorts) {
    Results results(sort.name);
    for (int c = 0; c < 96; c++) {
      World w = MakeWorld(r, block);
      const uint32_t n = 1 + r.Below(40);
      for (uint32_t i = 0; i < n; i++) {
        const uint32_t entry = kWEntries + 64 * r.Below(40);
        w.at(kWArray + 4 * i) = block + entry;
      }
      for (uint32_t e = 0; e < 40; e++) {
        const uint32_t entry = kWEntries + 64 * e;
        for (uint32_t field : {4u, 8u, 48u}) {
          w.at(entry + field) = r.Below(6) == 0 ? RandomFloat(r)
                                                : std::bit_cast<uint32_t>(KeyValue(r));
        }
      }
      const int32_t lo = int32_t(r.Below(n));
      const int32_t hi = r.Below(8) == 0 ? lo : lo + int32_t(r.Below(n - uint32_t(lo)));
      const uint32_t getter = getters[r.Below(3)];
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32s(h.base, block, w.words.data(), uint32_t(w.words.size()));
        PPCContext t = CallContext(h);
        t.r3.u64 = block + kWArray;
        t.r4.s64 = lo;
        t.r5.s64 = hi;
        t.r6.u64 = getter;
        (v == 0 ? sort.recompiled : sort.native)(t, h.base);
        out[v].Bytes(Read(h.base, block, kWorldBytes));
        if (sort.native == NativePartition) out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: {} entries, {} to {}, getter {:08X}", c, n, lo, hi,
                                getter));
    }
    results.Report();
  }
}

void SortGroup(Context& h) {
  const uint32_t block = memory::Alloc(kWorldBytes, 0);
  if (block == 0) {
    ITC_LOG_INFO("harness: render_queue: no block for the sorts' world");
    return;
  }
  Sorts(h, block);
  memory::Free(block);
}

}  // namespace

void RenderQueue(Context& h) {
  SmallMethods(h);
  ClearRecording(h);
  Init(h);
  Adds(h);
  PushBacks(h);
  SortGroup(h);
}

}  // namespace harness
