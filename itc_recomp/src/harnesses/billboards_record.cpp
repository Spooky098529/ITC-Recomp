// The light billboards' recording (src/sprites/billboard_record.cpp):
// Billboards::RecordState (sub_8209C1A0), Record (sub_8209CC38) and Draw
// (sub_8209CDB0) against the recompiled ones, under the billboards harness,
// with a random generator of their own (they run last, so their cases shift
// no other function's inputs).
//
// The world, in one allocation (the same addresses for both versions,
// filled with the same bytes before each): the render manager
// ([0x826BABA0]: its slot 8 and the answer's slots 1 and 2 the game's
// getters) with a command list of 2,048 slots and a render queue of the real
// class (its vtable 0x8206CB8C) with 512 items and 2,057 buckets in each
// pass list (as the headlights' harness builds them); the billboard set
// (+0 a camera pass: its view +16, projection +80, eye +400, look-at +416
// and draw distance +548; +4 the layer); the material (every alpha and blend
// mode the original tests and some it doesn't, a texture or none, 0 to 40
// lights around the eye). The vertex ring's globals are given random counts
// as the vertex builder's harness does.
//
// RecordState has no adapter: its native side is its plain C++ through the
// register steps here. Record's recompiled side runs with the vertex
// builder's adapter as the game has it; Draw's recompiled side calls
// Record's wrap, so it runs with --native_matrix_math off (Record's
// original there; Matrix::Multiply's original gives the same words, its
// harness).
//
// Compared (the command list's uploads as floats, the rest exactly): the
// allocation, the ring's window and globals, the open chunks' count, the
// frames' back chains and temporaries (up to their saved registers' slots)
// and r3. RecordState's frame only in its own cases: under Record it is dead
// stack the later callees' frames overwrite with the guest registers they
// save (Record's r26 in the render queue's), which the plain C++ doesn't
// hold.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_scene_draw/car_scene_draw.h"
#include "hooks/billboards.h"
#include "hooks/car_scene_draw.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "sprites/billboards.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <limits>
#include <tuple>
#include <vector>

ITC_FLAG_DECLARE(bool, native_matrix_math);

namespace harness {

namespace {

constexpr uint32_t kStackBytes = 4096;

// The allocation's layout.
constexpr uint32_t kManager = 0x000, kManagerVtable = 0x100;
constexpr uint32_t kObject = 0x200, kObjectVtable = 0x300;
constexpr uint32_t kList = 0x400;
constexpr uint32_t kQueue = 0x500, kState = 0x540, kListA = 0x5C0, kListB = 0x600;
constexpr uint32_t kSet = 0x700, kMaterial = 0x740, kPass = 0x800, kLights = 0xB00;
constexpr uint32_t kMostLights = 40;
constexpr uint32_t kSlots = 0x2000, kSlotCount = 2048;
constexpr uint32_t kItemsA = 0xA000, kItemsB = 0xC800, kItemCount = 512;
constexpr uint32_t kBucketsA = 0xF000, kBucketsB = 0x17100, kBucketCount = 2057;
constexpr uint32_t kWorldBytes = 0x20000;

constexpr uint32_t kGetter284 = 0x8248CA78;  // returns [r3 + 284]
constexpr uint32_t kGetter124 = 0x82379320;  // returns [r3 + 124]
constexpr uint32_t kQueueVtable = 0x8206CB8C;
constexpr uint32_t kOpenChunks = 0x8296BD28;
constexpr uint32_t kRenderApp = 0x826BABA0;

// The vertex ring and its globals.
constexpr uint32_t kRingCount = 0x826A509C;
constexpr uint32_t kRingHandle = 0x826A50A8;
constexpr uint32_t kRingIndex = 0x826A5098;
constexpr uint32_t kRingVertices = 0x8266D098;
constexpr uint32_t kRingBytes = 2 * 4096 * 28;

// The frames and their temporaries' ends (their saved registers above).
constexpr uint32_t kStateFrame = 368, kStateTemps = 288;
constexpr uint32_t kRecordFrame = 160, kRecordTemps = 88;
constexpr uint32_t kDrawFrame = 96, kDrawTemps = 88;

class Saved {
 public:
  explicit Saved(uint8_t* base)
      : base_(base),
        ring_(Read(base, kRingVertices, kRingBytes)),
        words_{guest::LoadU32(base, kRingCount), guest::LoadU32(base, kRingIndex),
               guest::LoadU32(base, kRingHandle), guest::LoadU32(base, kOpenChunks),
               guest::LoadU32(base, kRenderApp)} {}
  ~Saved() {
    Write(base_, kRingVertices, ring_.data(), kRingBytes);
    const uint32_t at[] = {kRingCount, kRingIndex, kRingHandle, kOpenChunks, kRenderApp};
    for (int i = 0; i < 5; i++) {
      guest::StoreU32(base_, at[i], words_[i]);
    }
  }

 private:
  uint8_t* base_;
  std::vector<uint8_t> ring_;
  uint32_t words_[5];
};

void StoreFloat(uint8_t* base, uint32_t address, float value) {
  guest::StoreU32(base, address, std::bit_cast<uint32_t>(value));
}
float Uniform(Random& r, float low, float high) {
  return low + (high - low) * float(r.Below(1u << 24)) / float(1u << 24);
}
float Value(Random& r, float scale) {
  switch (r.Below(60)) {
    case 0:
      return 0.0f;
    case 1:
      return -0.0f;
    case 2:
      return std::numeric_limits<float>::quiet_NaN();
    case 3:
      return std::numeric_limits<float>::infinity();
    case 4:
      return std::bit_cast<float>(uint32_t(r.Below(0x7FFFFF) + 1));
    default:
      return Uniform(r, -scale, scale);
  }
}

struct World {
  uint32_t b = 0;
  int32_t alpha = 0, blend = 0;
  uint32_t lights = 0;
  bool texture = false;
  uint32_t ring_index = 0, ring_count = 0;
  uint64_t seed = 0;
};

World Draw(Random& r, uint32_t b) {
  static const int32_t kAlpha[] = {16, 32, 64, 256, 1024, 1, 2, 128, 512, 2048, 0, -16};
  static const int32_t kBlend[] = {1, 2, 4, 8, 16, 0, 3, 32, -1};
  World w;
  w.b = b;
  w.alpha = r.Below(4) == 0 ? int32_t(r.Next()) : kAlpha[r.Below(12)];
  w.blend = r.Below(8) == 0 ? int32_t(r.Next()) : kBlend[r.Below(9)];
  w.lights = r.Below(6) == 0 ? 0 : 1 + r.Below(kMostLights);
  w.texture = r.Below(3) != 0;
  w.ring_index = r.Below(4) == 0 ? 1 : 0;
  w.ring_count = r.Below(4) == 0 ? 4090 + r.Below(8) : r.Below(4000);
  if (w.ring_index == 1 && w.ring_count + 4 * w.lights > 4096) {
    w.ring_count = 4096 - 4 * w.lights;
  }
  w.seed = r.Next();
  return w;
}

void Build(Context& h, const World& w) {
  uint8_t* base = h.base;
  Random r(w.seed);
  const uint32_t b = w.b;
  Fill(base, b, kWorldBytes);
  auto at = [&](uint32_t offset, uint32_t value) { guest::StoreU32(base, b + offset, value); };

  // The render manager, its getters, the command list and the queue.
  at(kManager, b + kManagerVtable);
  at(kManagerVtable + 32, kGetter284);
  at(kManager + 284, b + kObject);
  at(kObject, b + kObjectVtable);
  at(kObjectVtable + 4, kGetter284);
  at(kObjectVtable + 8, kGetter124);
  at(kObject + 284, b + kList);
  at(kObject + 124, b + kQueue);
  at(kList + 8, kSlotCount);
  at(kList + 12, r.Below(100));
  at(kList + 32, b + kSlots);
  at(kList + 44, b + kSlots);
  guest::StoreU8(base, b + kList + 52, r.Below(16) == 0 ? 1 : 0);
  at(kQueue + 0, kQueueVtable);
  at(kQueue + 4, b + kState);
  at(kQueue + 8, b + kListA);
  at(kQueue + 12, b + kListB);
  at(kState + 0, r.Below(2));
  StoreFloat(base, b + kState + 84, float(r.Below(100)));
  for (const auto& [list, items, buckets] :
       {std::tuple{kListA, kItemsA, kBucketsA}, std::tuple{kListB, kItemsB, kBucketsB}}) {
    at(list + 8, kItemCount);
    at(list + 16, r.Below(4) == 0 ? r.Below(20) : 0);
    at(list + 20, b + items);
    at(list + 12, kBucketCount);
    at(list + 24, b + buckets);
    for (uint32_t k = 0; k < kBucketCount; k++) {
      at(buckets + 16 * k + 0, 0);
      at(buckets + 16 * k + 12, 0);
    }
  }

  // The camera pass: random words, a view and a projection, the eye and
  // look-at, the draw distance.
  for (uint32_t i = 0; i < 148; i++) {
    StoreFloat(base, b + kPass + 4 * i, Value(r, 1.0f));
  }
  for (uint32_t i = 0; i < 16; i++) {
    StoreFloat(base, b + kPass + 16 + 4 * i, Value(r, 2.0f));
    StoreFloat(base, b + kPass + 80 + 4 * i, Value(r, 3.0f));
  }
  const float ex = Uniform(r, -8000, 8000), ey = Uniform(r, -50, 50), ez = Uniform(r, -8000, 8000);
  StoreFloat(base, b + kPass + 400, ex);
  StoreFloat(base, b + kPass + 404, ey);
  StoreFloat(base, b + kPass + 408, ez);
  StoreFloat(base, b + kPass + 416, ex + Uniform(r, -10, 10));
  StoreFloat(base, b + kPass + 420, ey + Uniform(r, -2, 2));
  StoreFloat(base, b + kPass + 424, ez + Uniform(r, -10, 10));
  StoreFloat(base, b + kPass + 548, r.Below(3) ? Uniform(r, 0, 600) : Uniform(r, 600, 100000));

  // The set and the material.
  at(kSet + 0, b + kPass);
  at(kSet + 4, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(600));
  at(kMaterial + 0, uint32_t(w.alpha));
  at(kMaterial + 4, uint32_t(w.blend));
  at(kMaterial + 12, w.texture ? uint32_t(r.Next()) | 1 : 0);
  at(kMaterial + 32, w.lights);
  at(kMaterial + 36, b + kLights);
  for (uint32_t k = 0; k < w.lights; k++) {
    const uint32_t l = kLights + 48 * k;
    const float reach = r.Below(4) == 0 ? 2000 : 300;
    StoreFloat(base, b + l + 0, ex + Uniform(r, -reach, reach));
    StoreFloat(base, b + l + 4, ey + Uniform(r, -20, 20));
    StoreFloat(base, b + l + 8, ez + Uniform(r, -reach, reach));
    at(l + 12, uint32_t(r.Next()));
    for (uint32_t c = 16; c < 32; c += 4) {
      StoreFloat(base, b + l + c, Uniform(r, 0, 255));
    }
    StoreFloat(base, b + l + 32, r.Below(6) == 0 ? 0.0f : Uniform(r, 0, 5));
    StoreFloat(base, b + l + 36, r.Below(6) == 0 ? 0.0f : Uniform(r, 0, 5));
    at(l + 40, uint32_t(r.Next()));
    at(l + 44, uint32_t(r.Next()));
  }

  // The globals.
  guest::StoreU32(base, kRenderApp, b + kManager);
  guest::StoreU32(base, kOpenChunks, r.Below(20) == 0 ? 1 : 0);
  guest::StoreU32(base, kRingCount, w.ring_count);
  guest::StoreU32(base, kRingIndex, w.ring_index);
  guest::StoreU32(base, kRingHandle, uint32_t(r.Next()));
}

template <typename IsFloat>
void Compare(uint8_t* base, Outputs& out, uint32_t from, uint32_t bytes, IsFloat is_float) {
  uint32_t run = 0;
  while (run < bytes) {
    const bool kind = is_float(run);
    uint32_t end = run + 4;
    while (end < bytes && is_float(end) == kind) {
      end += 4;
    }
    if (kind) {
      out.Floats(base, from + run, (end - run) / 4);
    } else {
      out.Bytes(Read(base, from + run, end - run));
    }
    run = end;
  }
}

void Collect(uint8_t* base, const World& w, const PPCContext& t,
             std::initializer_list<std::tuple<uint32_t, uint32_t, bool>> frames, Outputs& out) {
  out.Word(t.r3.u32);
  out.Word(uint32_t(t.r3.u64 >> 32));
  for (const auto& [frame, bytes, state] : frames) {
    const bool is_state = state;
    Compare(base, out, frame, bytes,
            [is_state](uint32_t o) { return is_state && o >= 96 && o < kStateTemps; });
  }
  std::vector<bool> upload(kSlotCount, false);
  for (uint32_t s = 0; s < kSlotCount; s++) {
    const uint32_t slot = w.b + kSlots + 16 * s;
    if (guest::LoadU32(base, slot) == 7 && guest::LoadU32(base, slot + 4) == 4) {
      for (uint32_t k = 1; k <= 4 && s + k < kSlotCount; k++) {
        upload[s + k] = true;
      }
      s += 4;
    }
  }
  Compare(base, out, w.b, kWorldBytes, [&](uint32_t o) {
    return o >= kSlots && o < kSlots + 16 * kSlotCount && upload[(o - kSlots) / 16];
  });
  // The ring's window from where the vertices go.
  const uint32_t start = kRingVertices + (w.ring_index * 4096 + w.ring_count) * 28;
  const uint32_t window = std::max(kRingVertices, start - 64);
  const uint32_t end = std::min(kRingVertices + kRingBytes, start + kMostLights * 4 * 28 + 64);
  out.Bytes(Read(base, window, end - window));
  out.Word(guest::LoadU32(base, kRingCount));
  out.Word(guest::LoadU32(base, kOpenChunks));
}

// RecordState's register steps (it has no adapter).
void NativeRecordState(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx, base);
  const uint32_t r3 = sprites::RecordBillboardState(base, calls, ctx.r1.u32, ctx.r3.u32,
                                                    ctx.r4.u32, ctx.r5.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

struct MatrixMathOff {
  MatrixMathOff() : saved_(ITC_FLAG(native_matrix_math)) { ITC_FLAG(native_matrix_math) = false; }
  ~MatrixMathOff() { ITC_FLAG(native_matrix_math) = saved_; }
  bool saved_;
};

}  // namespace

// The cases (called by Billboards in billboards.cpp).
void BillboardRecordCases(Context& h) {
  if (!hooks::NativeHeapActive() || !hooks::BillboardsNative()) {
    ITC_LOG_INFO("harness: Billboards::Record: skipped (no native heap or the recording's "
                 "flags off)");
    return;
  }
  Random r(0x42494C4C424F4152ull);
  const uint32_t b = memory::AllocPages(kWorldBytes, 4096);
  if (b == 0) {
    ITC_LOG_ERROR("harness: Billboards::Record: no memory for the world");
    return;
  }
  const Saved saved(h.base);
  // 0: RecordState, 1: Record, 2: Draw.
  const char* names[] = {"Billboards::RecordState (sub_8209C1A0)",
                         "Billboards::Record (sub_8209CC38)", "Billboards::Draw (sub_8209CDB0)"};
  for (int kind = 0; kind < 3; kind++) {
    Results results(names[kind]);
    int submitted = 0;
    for (int c = 0; c < 200; c++) {
      const World w = Draw(r, b);
      std::vector<uint8_t> stack_fill(kStackBytes + 64);
      for (uint8_t& byte : stack_fill) byte = uint8_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        const uint32_t stack = t.r1.u32;
        Write(h.base, stack - kStackBytes, stack_fill.data(), kStackBytes + 64);
        Build(h, w);
        t.r3.u64 = b + kSet;
        if (kind == 0) {
          t.r4.u64 = b + kList;
          t.r5.u64 = b + kMaterial;
          t.lr = 0x8209CCB0;
          (v == 0 ? __imp__sub_8209C1A0 : NativeRecordState)(t, h.base);
          Collect(h.base, w, t, {{stack - kStateFrame, kStateTemps, true}}, out[v]);
        } else if (kind == 1) {
          t.r4.u64 = b + kMaterial;
          t.lr = 0x8209CDD4;
          (v == 0 ? __imp__sub_8209CC38 : hooks::BillboardsRecord)(t, h.base);
          const uint32_t f = stack - kRecordFrame;
          Collect(h.base, w, t,
                  {{f, kRecordTemps, false}}, out[v]);
        } else {
          t.r4.u64 = b + kMaterial;
          t.lr = 0x82148A00;
          if (v == 0) {
            MatrixMathOff off;
            __imp__sub_8209CDB0(t, h.base);
          } else {
            hooks::BillboardsDraw(t, h.base);
          }
          const uint32_t f = stack - kDrawFrame;
          const uint32_t fr = f - kRecordFrame;
          Collect(h.base, w, t,
                  {{f, kDrawTemps, false}, {fr, kRecordTemps, false}}, out[v]);
        }
        if (v == 1 && w.lights != 0) {
          submitted++;
        }
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: alpha {} blend {} lights {} texture {} ring {} at {}", c,
                                w.alpha, w.blend, w.lights, w.texture, w.ring_index,
                                w.ring_count));
    }
    ITC_LOG_INFO("harness: {}: {} cases with lights", names[kind], submitted);
    results.Report();
  }
  memory::FreePages(b);
}

}  // namespace harness
