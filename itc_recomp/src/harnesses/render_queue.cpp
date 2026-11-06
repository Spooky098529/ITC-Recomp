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
#include "hooks/single_commands.h"
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

// --- The replay: Partition, QuickSort, ChunkChain::Run and Replay ---
//
// A 64 KB block from the allocation layer holds the world: a queue (the
// real vtable, so slot 8 is Replaying), its state with a release list
// (room for 64, so PtrVector::PushBack never allocates), two pass lists
// with up to 6 buckets of up to 6 items, chains of chunks of every kind
// (1 and 2, never seen in play, run by the original's walkers in both), and
// objects whose vtable is {Flip, a no-op, a no-op answering 0, Flip}
// (sub_8258E5E0 toggles the word the object's +4 points at; sub_825C8388
// returns 0). The commands are the game's own handlers that touch only
// what the world holds: 21 (a skip), 28 (each object's slot 0), 22 (each
// object onto the release list) and, in the replay only, 5 and 6 (which
// write into the state through [0x82970BBC]); the single commands 0 and 29
// to 36 (no-ops). The tables are checked to hold those handlers first.
// Keys come from a few values (runs of equal keys), never NaN: the
// original never ends on a NaN key, as the native one.
//
// Replay writes the globals the command handlers read
// (0x8296BD8C-0x82970BC3, with the entries array): the cases run with the
// game's device lock held (VideoOutput::LockDevice, so the render thread
// isn't replaying), the range saved and put back after.

constexpr uint32_t kWorldBytes = 65536;
constexpr uint32_t kWQueue = 0;
constexpr uint32_t kWState = 64;
constexpr uint32_t kWLists[2] = {256, 320};
constexpr uint32_t kWBuckets[2] = {512, 640};  // 6 buckets each
constexpr uint32_t kWRelease = 1024;          // 64 slots
constexpr uint32_t kWItems = 2048;            // 2 x 36 items of 20 bytes
constexpr uint32_t kWObjects = 4096;          // 16 objects of 32 bytes
constexpr uint32_t kWObjectVtable = 4608;
constexpr uint32_t kWToggles = 4672;          // 16 words
constexpr uint32_t kWArray = 4800;            // Partition's array: 40 pointers
constexpr uint32_t kWEntries = 5120;          // its entries: 40 of 64 bytes
constexpr uint32_t kWChains = 8192;           // to 36864
constexpr uint32_t kWLists2 = 36864;          // the kinds 3 and 4 command lists, to 65536

constexpr uint32_t kFlip = 0x8258E5E0;
constexpr uint32_t kNoOp = 0x825C8388;
constexpr uint32_t kFloat48Getter = 0x820B68A0;  // RigidBody's float at +48: a guest call

constexpr uint32_t kGlobals = 0x8296BD8C;
constexpr uint32_t kGlobalsBytes = 0x82970BC4 - 0x8296BD8C;

struct World {
  uint32_t b = 0;
  std::vector<uint32_t> words = std::vector<uint32_t>(kWorldBytes / 4);
  uint32_t& at(uint32_t offset) { return words[offset / 4]; }
  uint32_t chain_cursor = kWChains;
  uint32_t list_cursor = kWLists2;
  int pushes = 0;  // command 22's objects, at most 64 for the release list
  bool replay = false;  // commands 5 and 6 allowed
};

uint32_t Object(Random& r, World& w) { return w.b + kWObjects + 32 * r.Below(16); }

// Commands at `cursor` (bytes into the block), ending with 36; returns the
// cursor after the end slot.
uint32_t BuildCommands(Random& r, World& w, uint32_t cursor) {
  const uint32_t count = r.Below(4);
  for (uint32_t c = 0; c < count; c++) {
    const uint32_t slot = cursor;
    for (uint32_t i = 0; i < 4; i++) w.at(slot + 4 * i) = uint32_t(r.Next());
    uint32_t extra = 0;
    // Outside the replay only 21 and 28: 22 would push onto the game's
    // release list, 5 and 6 write through the game's [0x82970BBC].
    switch (r.Below(w.replay ? 5 : 2)) {
      case 0: {  // 21: skips (r4 + 15) >> 4 slots
        const uint32_t bytes = r.Below(48);
        w.at(slot + 0) = 21;
        w.at(slot + 4) = bytes;
        extra = (bytes + 15) >> 4;
        break;
      }
      case 1: {  // 28: the objects at payload + 4 + 16 j
        const uint32_t n = r.Below(4);
        w.at(slot + 0) = 28;
        w.at(slot + 4) = n;
        for (uint32_t j = 0; j < n; j++) w.at(slot + 20 + 16 * j) = Object(r, w);
        extra = n;
        break;
      }
      case 2: {  // 22: the objects with +4 = 1 onto the release list
        const uint32_t n = w.pushes + 3 < 64 ? r.Below(4) : 0;
        w.at(slot + 0) = 22;
        w.at(slot + 4) = n;
        for (uint32_t j = 0; j < n; j++) {
          w.at(slot + 20 + 16 * j) = Object(r, w);
          w.at(slot + 24 + 16 * j) = r.Below(3) == 0 ? 0 : 1;
        }
        w.pushes += int(n);
        extra = n;
        break;
      }
      case 3: {  // 5: state +24, +28 = the payload and r4
        const uint32_t r4 = r.Below(3);
        w.at(slot + 0) = 5;
        w.at(slot + 4) = r4;
        extra = (r4 * 24 + 15) >> 4;
        break;
      }
      default: {  // 6: state +32 to +40 = the payload, r4 and r5
        const uint32_t r4 = r.Below(3);
        const uint32_t r5 = r.Below(2) == 0 ? 0 : uint32_t(r.Next());
        w.at(slot + 0) = 6;
        w.at(slot + 4) = r4;
        w.at(slot + 8) = r5;
        extra = ((r5 == 0 ? 2 : 4) * r4 + 15) >> 4;
        break;
      }
    }
    cursor = slot + 16 * (extra + 1);
  }
  w.at(cursor) = 36;
  return cursor + 16;
}

// A chain of chunks at the chain cursor; returns its first item's address.
uint32_t BuildChain(Random& r, World& w, int depth = 0) {
  const uint32_t item = w.chain_cursor;
  for (uint32_t i = 0; i < 4; i++) w.at(item + 4 * i) = uint32_t(r.Next());
  const uint32_t kinds[] = {0, 1, 2, 3, 3, 3, 4, 5, 6};
  uint32_t kind = kinds[r.Below(9)];
  if (depth >= 4 && (kind == 1 || kind == 2 || kind == 3)) kind = 6;
  // Room left: a chunk with its commands takes at most ~400 bytes.
  if (w.chain_cursor + 1024 > kWLists2) kind = 6;
  if (w.list_cursor + 512 > kWorldBytes && (kind == 3 || kind == 4)) kind = 6;
  w.at(item) = (uint32_t(r.Next()) & 0xFFFF0000u) | kind;
  w.chain_cursor = item + 16;
  switch (kind) {
    case 0: {  // single commands: no-ops
      const uint32_t count = r.Below(5);
      w.at(item + 8) = count;
      const uint32_t ids[] = {0, 29, 30, 31, 32, 33, 34, 35, 36};
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t slot = item + 16 * (i + 1);
        w.at(slot) = ids[r.Below(9)];
        for (uint32_t k = 1; k < 4; k++) w.at(slot + 4 * k) = uint32_t(r.Next());
      }
      w.chain_cursor = item + 16 * (count + 1);
      break;
    }
    case 1:  // inline commands, then the next item after the end slot
      w.chain_cursor = BuildCommands(r, w, item + 16);
      BuildChain(r, w, depth + 1);
      break;
    case 2: {  // inline commands, then the next item by +4
      w.chain_cursor = BuildCommands(r, w, item + 16);
      w.at(item + 4) = r.Below(3) == 0 ? 0 : w.b + w.chain_cursor;
      if (w.at(item + 4) != 0) BuildChain(r, w, depth + 1);
      break;
    }
    case 3:  // the commands at +4, then the next item at +16
      w.at(item + 4) = w.b + w.list_cursor;
      w.list_cursor = BuildCommands(r, w, w.list_cursor);
      BuildChain(r, w, depth + 1);
      break;
    case 4:
      w.at(item + 4) = w.b + w.list_cursor;
      w.list_cursor = BuildCommands(r, w, w.list_cursor);
      break;
    case 5:
      w.chain_cursor = BuildCommands(r, w, item + 16);
      break;
    default:
      break;
  }
  return w.b + item;
}

// The objects, their vtable and toggled words; the rest of the world random.
World MakeWorld(Random& r, uint32_t block, bool replay) {
  World w;
  w.b = block;
  w.replay = replay;
  for (uint32_t& word : w.words) word = uint32_t(r.Next());
  w.at(kWObjectVtable + 0) = kFlip;
  w.at(kWObjectVtable + 4) = kNoOp;
  w.at(kWObjectVtable + 8) = kNoOp;
  w.at(kWObjectVtable + 12) = kFlip;
  for (uint32_t i = 0; i < 16; i++) {
    w.at(kWObjects + 32 * i) = block + kWObjectVtable;
    w.at(kWObjects + 32 * i + 4) = block + kWToggles + 4 * i;
  }
  return w;
}

float KeyValue(Random& r) {
  const float keys[] = {-1.0f, 0.0f, 1.0f, 2.5f, 1e30f, -std::numeric_limits<float>::infinity(),
                        std::numeric_limits<float>::infinity(), -0.0f};
  return keys[r.Below(8)];
}

// Whether the tables hold the handlers the cases use.
bool TablesAsExpected(uint8_t* base) {
  const std::pair<uint32_t, uint32_t> commands[] = {
      {5, 0x8258E9F8}, {6, 0x8258EA20}, {21, 0x8258EAD0}, {22, 0x82591798}, {28, 0x8258EAE0}};
  for (auto [id, handler] : commands) {
    if (guest::LoadU32(base, 0x8265EDE0 + 4 * id) != handler) return false;
  }
  for (uint32_t id : {0u, 29u, 30u, 31u, 32u, 33u, 34u, 35u, 36u}) {
    if (guest::LoadU32(base, 0x8265ED48 + 4 * id) != kNoOp) return false;
  }
  return true;
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
void NativeChunkChainRun(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rq::ChunkChainRun(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, hooks::RenderQueueCall(ctx, base),
                    hooks::SingleCommands(ctx, base));
}
void NativeReplay(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  hooks::NativeRenderQueueReplay(ctx, base);
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
      World w = MakeWorld(r, block, false);
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

void Chains(Context& h, uint32_t block) {
  Random& r = h.random;
  Results results("ChunkChain::Run (sub_8258EE98, with its walkers)");
  for (int c = 0; c < 128; c++) {
    World w = MakeWorld(r, block, false);
    const uint32_t chain = r.Below(16) == 0 ? 0 : BuildChain(r, w);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, block, w.words.data(), uint32_t(w.words.size()));
      PPCContext t = CallContext(h);
      t.r3.u64 = block + kWQueue;
      t.r4.u64 = chain;
      (v == 0 ? __imp__sub_8258EE98 : NativeChunkChainRun)(t, h.base);
      // r3 isn't compared: the replay doesn't read ChunkChain::Run's.
      out[v].Bytes(Read(h.base, block, kWorldBytes));
    }
    results.Check(out[0], out[1], fmt::format("case {}: chain at +{}", c, chain ? chain - block : 0));
  }
  results.Report();
}

// A queue for the replay: its state, release list and the two pass lists.
void MakeReplayQueue(Random& r, World& w, uint32_t index) {
  const uint32_t b = w.b;
  w.at(kWQueue + 0) = rq::kVtable;
  w.at(kWQueue + 4) = b + kWState;
  w.at(kWQueue + 8) = b + kWLists[0];
  w.at(kWQueue + 12) = b + kWLists[1];
  w.at(kWState + 0) = index;
  // The release list: the vector at state + 68 (+4 first, +8 last, +12 end).
  const uint32_t size = r.Below(8);
  w.at(kWState + 68 + 4) = b + kWRelease;
  w.at(kWState + 68 + 8) = b + kWRelease + 4 * size;
  w.at(kWState + 68 + 12) = b + kWRelease + 4 * 64;
  uint32_t item = kWItems;
  for (int l = 0; l < 2; l++) {
    const uint32_t list = kWLists[l];
    const uint32_t buckets = r.Below(7);
    w.at(list + 12) = buckets;
    w.at(list + 24) = b + kWBuckets[l];
    for (uint32_t k = 0; k < buckets; k++) {
      const uint32_t bucket = kWBuckets[l] + 16 * k;
      const uint32_t items = r.Below(4) == 0 ? 0 : 1 + r.Below(6);
      w.at(bucket + 0) = items == 0 ? 0 : b + item;
      for (uint32_t i = 0; i < items; i++, item += 20) {
        w.at(item + 0) = i + 1 < items ? b + item + 20 : 0;
        w.at(item + 4) = r.Below(8) == 0 ? 0 : BuildChain(r, w);
        w.at(item + 8) = std::bit_cast<uint32_t>(KeyValue(r));
        w.at(item + 12) = r.Below(3) == 0 ? std::bit_cast<uint32_t>(KeyValue(r)) : RandomFloat(r);
      }
    }
  }
}

// The game's video output, as the device's users reach it ([0x8266D088]'s
// slot 7, then that object's slot 3).
uint32_t GameVideoOutput(Context& h) {
  PPCContext t = CallContext(h);
  t.r3.u64 = guest::LoadU32(h.base, 0x8266D088);
  uint8_t* base = h.base;
  {
    PPCContext& ctx = t;
    ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 28);
    REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 12);
    REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  }
  return t.r3.u32;
}

void Replays(Context& h, uint32_t block) {
  Random& r = h.random;
  const uint32_t video_output = GameVideoOutput(h);
  PPCContext lock = CallContext(h);
  lock.r3.u64 = video_output;
  sub_8258D8C8(lock, h.base);  // VideoOutput::LockDevice
  const uint32_t locked_device = lock.r3.u32;
  const std::vector<uint8_t> saved = Read(h.base, kGlobals, kGlobalsBytes);
  Results results("RenderQueue::Replay (sub_82591238)");
  for (int c = 0; c < 64; c++) {
    World w = MakeWorld(r, block, true);
    MakeReplayQueue(r, w, r.Below(2));
    const uint32_t device = block + 32;  // only stored and passed on (command 28's r4)
    const uint32_t output = uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h.base, block, w.words.data(), uint32_t(w.words.size()));
      Write(h.base, kGlobals, saved.data(), kGlobalsBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = block + kWQueue;
      t.r4.u64 = device;
      t.r5.u64 = output;
      (v == 0 ? __imp__sub_82591238 : NativeReplay)(t, h.base);
      // r3 isn't compared: Scene::Draw doesn't read it.
      out[v].Bytes(Read(h.base, block, kWorldBytes));
      out[v].Bytes(Read(h.base, kGlobals, 12 + 4 * 64));       // the release list, device, entries
      out[v].Bytes(Read(h.base, 0x82970BB8, 12));              // the drawer, state + 12, targets
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  Write(h.base, kGlobals, saved.data(), kGlobalsBytes);
  results.Report();
  guest::StoreU32(h.base, h.data, locked_device);
  lock.r3.u64 = video_output;
  lock.r4.u64 = h.data;
  sub_8258D908(lock, h.base);  // VideoOutput::UnlockDevice
}

void ReplayGroup(Context& h) {
  if (!TablesAsExpected(h.base)) {
    ITC_LOG_INFO("harness: render_queue: the command tables don't hold the expected handlers");
    return;
  }
  const uint32_t block = memory::Alloc(kWorldBytes, 0);
  if (block == 0) {
    ITC_LOG_INFO("harness: render_queue: no block for the replay's world");
    return;
  }
  Sorts(h, block);
  Chains(h, block);
  Replays(h, block);
  memory::Free(block);
}

}  // namespace

void RenderQueue(Context& h) {
  SmallMethods(h);
  ClearRecording(h);
  Init(h);
  Adds(h);
  PushBacks(h);
  ReplayGroup(h);
}

}  // namespace harness
