// The car scene's draw (src/car_scene_draw/, docs/functions/car-scene-draw.md)
// against the recompiled functions. The originals run with
// --native_car_scene_draw off (so their callees are the originals too), the
// plain C++ through the register steps (hooks/car_scene_draw.h). Inputs go
// in the scratch area, the stack below the call's r1 is filled with the
// same bytes before each version, and the vectors' storage comes from the
// allocation layer (operator new's plain C++), each version building its
// own: words pointing into it are compared by the block's number and the
// offset. Compared with harness::Outputs: what the functions store with
// stfs, stvx or as copies of such words (the planes, points, boxes and their
// temporaries below r1) as floats, the rest (r3, codes, the vectors' words,
// back chains) exactly. The saved registers' slots aren't compared (the
// plain C++ doesn't write them).

#include "harness.h"

#include "base/flags.h"
#include "hooks/car_scene_draw.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <limits>
#include <optional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_scene_draw);

namespace harness {

namespace {

constexpr uint32_t kStackBytes = 1024;  // below the call's r1, filled per version

struct FlagOff {
  FlagOff() : saved_(ITC_FLAG(native_car_scene_draw)) { ITC_FLAG(native_car_scene_draw) = false; }
  ~FlagOff() { ITC_FLAG(native_car_scene_draw) = saved_; }
  bool saved_;
};

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void StoreFloat(uint8_t* base, uint32_t address, float value) {
  guest::StoreU32(base, address, std::bit_cast<uint32_t>(value));
}

// --- Random floats ---

float Uniform(Random& random, float low, float high) {
  return low + (high - low) * float(random.Below(1u << 24)) / float(1u << 24);
}
// Mostly ordinary values, sometimes a special one (0, -0, NaN, infinities,
// a denormal, a huge or tiny value).
float Value(Random& random, float scale) {
  switch (random.Below(40)) {
    case 0:
      return 0.0f;
    case 1:
      return -0.0f;
    case 2:
      return std::numeric_limits<float>::quiet_NaN();
    case 3:
      return std::numeric_limits<float>::infinity();
    case 4:
      return -std::numeric_limits<float>::infinity();
    case 5:
      return std::bit_cast<float>(uint32_t(random.Below(0x7FFFFF) + 1));
    case 6:
      return Uniform(random, -1.0f, 1.0f) * 1e30f;
    case 7:
      return Uniform(random, -1.0f, 1.0f) * 1e-30f;
    default:
      return Uniform(random, -scale, scale);
  }
}
// Ordinary values only.
float Plain(Random& random, float scale) { return Uniform(random, -scale, scale); }

// The heap blocks a case made or met, numbered in the order added (as the
// app states' harness).
struct Blocks {
  std::vector<uint32_t> list;
  std::vector<uint32_t> sizes;
  uint32_t Add(uint32_t block) {
    list.push_back(block);
    sizes.push_back(block != 0 && memory::IsHeapBlock(block) ? memory::Size(block) : 4);
    return block;
  }
  uint32_t New(uint8_t* base, uint32_t bytes) {
    const auto block = memory::crt::OperatorNew(base, bytes);
    return Add(block ? *block : 0);
  }
  bool Known(uint32_t block) const {
    for (uint32_t b : list) {
      if (b == block) {
        return true;
      }
    }
    return false;
  }
  // A word into a block (or just past its end) as 0xB1000000 + 0x1000 x its
  // number + the offset; others as they are.
  uint32_t Of(uint32_t word) const {
    for (uint32_t i = 0; i < list.size(); i++) {
      if (list[i] != 0 && word >= list[i] && word - list[i] <= sizes[i]) {
        return 0xB1000000u + 0x1000 * i + (word - list[i]);
      }
    }
    return word;
  }
  // Each block's state (live and its size, or freed), then the live ones freed.
  void Out(Outputs& out) const {
    for (uint32_t block : list) {
      const bool live = block != 0 && memory::IsHeapBlock(block);
      out.Word(live ? 1 : 0);
      if (live) {
        out.Word(memory::Size(block));
      }
    }
    for (uint32_t block : list) {
      if (block != 0 && memory::IsHeapBlock(block)) {
        memory::crt::OperatorDelete(block);
      }
    }
  }
};

// One case through both versions: the scratch area and the stack below r1
// filled, `setup(blocks)` writes the inputs (the same both times: it's
// handed a copy of the generator's state), `args(t)` sets the registers,
// the original (flag off) or the step runs, then `collect(out, t, blocks)`
// keeps what's compared (and frees the blocks: Blocks::Out).
template <typename Setup, typename Args, typename Collect>
void Case(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, Collect collect, const std::string& what) {
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    h.random = start;
    Fill(h.base, h.data, kScratchBytes);
    PPCContext t = CallContext(h);
    Fill(h.base, t.r1.u32 - kStackBytes, kStackBytes);
    Blocks blocks;
    setup(blocks);
    args(t);
    {
      std::optional<FlagOff> off;
      if (v == 0) {
        off.emplace();
      }
      t.fpscr.disableFlushMode();
      (v == 0 ? recompiled : native)(t, h.base);
    }
    collect(out[v], t, blocks);
    after = h.random;
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

// --- The planes and the frustum ---

// A plane (x, y, z, d): a random direction and distance, sometimes with
// special values.
void WritePlane(Context& h, uint32_t at, bool special) {
  for (uint32_t i = 0; i < 16; i += 4) {
    StoreFloat(h.base, at + i, special ? Value(h.random, 10.0f) : Plain(h.random, 10.0f));
  }
}

void Planes(Context& h) {
  const uint32_t out = h.data;
  const uint32_t a = h.data + 16;
  const uint32_t b = h.data + 32;
  const uint32_t c = h.data + 48;
  {
    // Random planes; every eighth case one plane a copy of another
    // (parallel: no point); a quarter with special values; some with c a
    // scaled copy of a (a zero triple product).
    Results results("Plane::Intersect3 (sub_825976C8)");
    for (int k = 0; k < 400; k++) {
      Case(
          h, results,
          [&](Blocks&) {
            const bool special = k % 4 == 3;
            WritePlane(h, a, special);
            WritePlane(h, b, special);
            WritePlane(h, c, special);
            if (k % 8 == 1) {
              for (uint32_t i = 0; i < 12; i += 4) {
                Store(h.base, b + i, Load(h.base, a + i));
              }
            } else if (k % 8 == 5) {
              const float scale = Plain(h.random, 3.0f);
              for (uint32_t i = 0; i < 12; i += 4) {
                StoreFloat(h.base, c + i,
                           std::bit_cast<float>(Load(h.base, a + i)) * scale);
              }
            } else if (k % 16 == 6) {
              // A tiny triple product (denormal).
              for (uint32_t i = 0; i < 12; i += 4) {
                StoreFloat(h.base, a + i, std::bit_cast<float>(Load(h.base, a + i)) * 1e-14f);
              }
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = out;
            t.r4.u64 = a;
            t.r5.u64 = b;
            t.r6.u64 = c;
          },
          __imp__sub_825976C8, hooks::CarSceneDrawPlaneIntersect3,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            o.Floats(h.base, out, 4);
            o.Floats(h.base, t.r1.u32 - 80, 20);
            o.Word(t.r3.u32);
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
  {
    // Lines from a through b: random, along the plane (b - a in it), a = b
    // (no direction), special values.
    Results results("Plane::SegmentCrossing (sub_82597E58)");
    for (int k = 0; k < 400; k++) {
      Case(
          h, results,
          [&](Blocks&) {
            const bool special = k % 4 == 3;
            WritePlane(h, a, special);  // the plane
            for (uint32_t i = 0; i < 16; i += 4) {
              StoreFloat(h.base, b + i, special ? Value(h.random, 50.0f) : Plain(h.random, 50.0f));
              StoreFloat(h.base, c + i, special ? Value(h.random, 50.0f) : Plain(h.random, 50.0f));
            }
            if (k % 8 == 1) {
              for (uint32_t i = 0; i < 16; i += 4) {
                Store(h.base, b + i, Load(h.base, c + i));
              }
            } else if (k % 8 == 5) {
              // b = a + a direction across the plane's normal (n x r).
              float n[3], r[3], d[3];
              for (int i = 0; i < 3; i++) {
                n[i] = std::bit_cast<float>(Load(h.base, a + 4 * i));
                r[i] = Plain(h.random, 1.0f);
              }
              d[0] = n[1] * r[2] - n[2] * r[1];
              d[1] = n[2] * r[0] - n[0] * r[2];
              d[2] = n[0] * r[1] - n[1] * r[0];
              for (int i = 0; i < 3; i++) {
                StoreFloat(h.base, b + 4 * i,
                           std::bit_cast<float>(Load(h.base, c + 4 * i)) + d[i]);
              }
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = out;
            t.r4.u64 = a;
            t.r5.u64 = b;
            t.r6.u64 = c;
          },
          __imp__sub_82597E58, hooks::CarSceneDrawPlaneSegmentCrossing,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            o.Floats(h.base, out, 4);
            o.Floats(h.base, t.r1.u32 - 32, 8);
            o.Word(t.r3.u32);
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
}

// A row-vector perspective (Matrix::Perspective's layout) times a random
// turn and shift, as the fit's projections: or random words.
void WriteMatrix(Context& h, uint32_t at, int kind) {
  float m[16] = {};
  if (kind == 0 || kind == 1) {
    const float fov = Uniform(h.random, 0.3f, 1.6f);
    const float aspect = Uniform(h.random, 0.5f, 2.5f);
    const float near_plane = Uniform(h.random, 0.05f, 2.0f);
    const float far_plane = near_plane + Uniform(h.random, 1.0f, 1000.0f);
    const float ys = 1.0f / std::tan(fov * 0.5f);
    float p[16] = {};
    p[0] = ys / aspect;
    p[5] = ys;
    p[10] = far_plane / (far_plane - near_plane);
    p[11] = 1.0f;
    p[14] = -near_plane * far_plane / (far_plane - near_plane);
    if (kind == 0) {
      for (int i = 0; i < 16; i++) {
        m[i] = p[i];
      }
    } else {
      // A view: a turn about y and x, then a shift, times the perspective.
      const float yaw = Uniform(h.random, -3.1f, 3.1f);
      const float pitch = Uniform(h.random, -1.5f, 1.5f);
      const float cy = std::cos(yaw), sy = std::sin(yaw);
      const float cp = std::cos(pitch), sp = std::sin(pitch);
      float v[16] = {cy, sy * sp, -sy * cp, 0, 0, cp, sp, 0, sy, -cy * sp, cy * cp, 0,
                     Plain(h.random, 9000.0f), Plain(h.random, 100.0f),
                     Plain(h.random, 9000.0f), 1};
      for (int r = 0; r < 4; r++) {
        for (int col = 0; col < 4; col++) {
          float sum = 0;
          for (int i = 0; i < 4; i++) {
            sum += v[4 * r + i] * p[4 * i + col];
          }
          m[4 * r + col] = sum;
        }
      }
    }
  } else if (kind == 2) {
    for (int i = 0; i < 16; i++) {
      m[i] = Plain(h.random, 4.0f);
    }
  } else if (kind == 3) {
    for (int i = 0; i < 16; i++) {
      m[i] = Value(h.random, 4.0f);
    }
  }  // kind 4: all zero
  for (uint32_t i = 0; i < 16; i++) {
    StoreFloat(h.base, at + 4 * i, m[i]);
  }
}

void Frustum(Context& h) {
  const uint32_t out = h.data;
  const uint32_t matrix = h.data + 512;
  Results results("Frustum::FromMatrix (sub_82597878)");
  for (int k = 0; k < 300; k++) {
    const int kind = k < 100 ? 0 : k < 200 ? 1 : k < 260 ? 2 : k < 295 ? 3 : 4;
    Case(
        h, results, [&](Blocks&) { WriteMatrix(h, matrix, kind); },
        [&](PPCContext& t) {
          t.r3.u64 = out;
          t.r4.u64 = matrix;
        },
        __imp__sub_82597878, hooks::CarSceneDrawFrustumFromMatrix,
        [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
          const uint32_t f = t.r1.u32 - 336;
          o.Floats(h.base, out, 24);                   // the planes
          o.Bytes(Read(h.base, out + 96, 32));         // their codes (+120 on unwritten)
          o.Floats(h.base, out + 128, 32);             // the corners
          o.Word(Load(h.base, f));                     // the back chain
          o.Bytes(Read(h.base, f + 4, 76));            // the linkage area (unwritten)
          o.Floats(h.base, f + 80, 40);                // the temporaries
          o.Floats(h.base, f - 80, 20);                // Intersect3's
          o.Word(t.r3.u32);
          blocks.Out(o);
        },
        fmt::format("kind {} case {}", kind, k));
  }
  results.Report();
}

// --- The box ---

void Box(Context& h) {
  const uint32_t box = h.data;
  const uint32_t point = h.data + 32;
  const uint32_t vector = h.data + 48;
  const uint32_t points = h.data + 64;
  {
    Results results("Box::Extend (sub_82597150)");
    for (int k = 0; k < 300; k++) {
      Case(
          h, results,
          [&](Blocks&) {
            const bool special = k % 3 == 2;
            for (uint32_t i = 0; i < 32; i += 4) {
              StoreFloat(h.base, box + i, special ? Value(h.random, 20.0f) : Plain(h.random, 20.0f));
            }
            for (uint32_t i = 0; i < 16; i += 4) {
              StoreFloat(h.base, point + i,
                         special ? Value(h.random, 20.0f) : Plain(h.random, 20.0f));
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = box;
            t.r4.u64 = point;
          },
          __imp__sub_82597150, hooks::CarSceneDrawBoxExtend,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            o.Floats(h.base, box, 8);
            o.Word(t.r3.u32);
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
  {
    // A vector with no storage, or first anywhere and last 0 to 40 elements
    // on (sometimes not a whole element, or before first).
    Results results("Vector16::Size (sub_825971E8)");
    for (int k = 0; k < 200; k++) {
      Case(
          h, results,
          [&](Blocks&) {
            const uint32_t first = k % 10 == 0 ? 0 : uint32_t(h.random.Next());
            uint32_t last = first + 16 * h.random.Below(41);
            if (k % 7 == 3) {
              last = first + h.random.Below(700);
            } else if (k % 11 == 4) {
              last = first - 16 * h.random.Below(41);
            }
            Store(h.base, vector + 4, first);
            Store(h.base, vector + 8, last);
          },
          [&](PPCContext& t) { t.r3.u64 = vector; }, __imp__sub_825971E8,
          hooks::CarSceneDrawVector16Size,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            o.Word(t.r3.u32);
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
  {
    // 0 to 40 points in the scratch area (no storage at all every tenth
    // case), some with special values.
    Results results("Box::FromPoints (sub_82597210)");
    for (int k = 0; k < 300; k++) {
      Case(
          h, results,
          [&](Blocks&) {
            const uint32_t count = h.random.Below(41);
            const bool special = k % 4 == 3;
            for (uint32_t i = 0; i < 16 * count; i += 4) {
              StoreFloat(h.base, points + i,
                         special ? Value(h.random, 300.0f) : Plain(h.random, 300.0f));
            }
            const uint32_t first = k % 10 == 0 ? 0 : points;
            Store(h.base, vector + 4, first);
            Store(h.base, vector + 8, first + 16 * count);
            Store(h.base, vector + 12, first + 16 * count);
            // A box with something in it already (overwritten).
            for (uint32_t i = 0; i < 32; i += 4) {
              StoreFloat(h.base, box + i, Plain(h.random, 5.0f));
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = box;
            t.r4.u64 = vector;
          },
          __imp__sub_82597210, hooks::CarSceneDrawBoxFromPoints,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            o.Floats(h.base, box, 8);
            o.Word(Load(h.base, t.r1.u32 - 128));  // the back chain
            o.Word(t.r3.u32);
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
}

// --- The vector ---

void Vector(Context& h) {
  const uint32_t vector = h.data;
  const uint32_t result = h.data + 16;
  const uint32_t value = h.data + 32;
  // A vector of `size` random elements in a heap block with room for
  // `spare` more (no storage when both are 0, half the time); returns the
  // element at `index` (clamped to the size).
  auto build = [&](Blocks& blocks, uint32_t size, uint32_t spare, uint32_t index) {
    uint32_t block = 0;
    if (size + spare != 0 || h.random.Below(2) != 0) {
      block = blocks.New(h.base, 16 * (size + spare));
      for (uint32_t i = 0; i < 4 * (size + spare); i++) {
        Store(h.base, block + 4 * i, uint32_t(h.random.Next()));
      }
    }
    Store(h.base, vector + 0, uint32_t(h.random.Next()));  // the allocator's word
    Store(h.base, vector + 4, block);
    Store(h.base, vector + 8, block + 16 * size);
    Store(h.base, vector + 12, block + 16 * (size + spare));
    for (uint32_t i = 0; i < 16; i += 4) {
      Store(h.base, value + i, uint32_t(h.random.Next()));
    }
    return block + 16 * std::min(index, size);
  };
  // The vector by number, its storage's words up to last, the scratch's
  // words, the frames' back chains and words named, the blocks' state.
  auto collect = [&](Outputs& out, Blocks& blocks, const std::vector<uint32_t>& frame_words) {
    const uint32_t first = Load(h.base, vector + 4);
    const uint32_t last = Load(h.base, vector + 8);
    if (first != 0 && memory::IsHeapBlock(first)) {
      if (!blocks.Known(first)) {
        blocks.Add(first);
      }
      if (last >= first && last - first <= 0x1000) {
        out.Bytes(Read(h.base, first, last - first));
      }
    }
    for (uint32_t at = 0; at < 48; at += 4) {
      out.Word(blocks.Of(Load(h.base, h.data + at)));
    }
    for (uint32_t at : frame_words) {
      out.Word(blocks.Of(Load(h.base, at)));
    }
  };
  {
    Results results("Vector16::Allocate (sub_82598158)");
    for (int k = 0; k < 60; k++) {
      Case(
          h, results, [&](Blocks&) {},
          [&](PPCContext& t) {
            t.r3.u64 = vector;
            t.r4.u64 = k % 10 == 0 ? 0 : h.random.Below(k % 3 == 0 ? 4000 : 40);
          },
          __imp__sub_82598158, hooks::CarSceneDrawVector16Allocate,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            blocks.Add(t.r3.u32);
            o.Word(blocks.Of(t.r3.u32));
            o.Word(Load(h.base, t.r1.u32 - 112));  // the back chain
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
  {
    Results results("Vector16::InsertN (sub_82598288)");
    for (int k = 0; k < 400; k++) {
      uint32_t where = 0, count = 0;
      Case(
          h, results,
          [&](Blocks& blocks) {
            const uint32_t size = h.random.Below(k % 3 == 0 ? 3 : 20);
            count = h.random.Below(k % 4 == 0 ? 30 : 6);
            where = build(blocks, size, h.random.Below(k % 2 == 0 ? 4 : 40),
                          h.random.Below(size + 1));
          },
          [&](PPCContext& t) {
            t.r3.u64 = vector;
            t.r4.u64 = where;
            t.r5.u64 = count;
            t.r6.u64 = value;
          },
          __imp__sub_82598288, hooks::CarSceneDrawVector16InsertN,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            const uint32_t f = t.r1.u32 - 144;
            collect(o, blocks, {f, f + 80, f + 84, f + 88, f + 92, f - 112});
            o.Word(blocks.Of(t.r3.u32));
            blocks.Out(o);
          },
          fmt::format("count {} case {}", count, k));
    }
    results.Report();
  }
  {
    Results results("Vector16::Insert (sub_82598648)");
    for (int k = 0; k < 200; k++) {
      uint32_t where = 0;
      Case(
          h, results,
          [&](Blocks& blocks) {
            const uint32_t size = h.random.Below(20);
            where = build(blocks, size, h.random.Below(3), h.random.Below(size + 1));
          },
          [&](PPCContext& t) {
            t.r3.u64 = result;
            t.r4.u64 = vector;
            t.r5.u64 = where;
            t.r6.u64 = value;
          },
          __imp__sub_82598648, hooks::CarSceneDrawVector16Insert,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            const uint32_t f = t.r1.u32 - 112;
            collect(o, blocks, {f, f - 144, f - 144 + 80, f - 144 + 92});
            o.Word(blocks.Of(t.r3.u32));
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
  {
    // Sizes 0 to 20 with 0 to 2 spare (no storage sometimes), 30 in a row
    // onto one vector at the end (growing past its capacity several times).
    Results results("Vector16::PushBack (sub_825986B8)");
    for (int k = 0; k < 200; k++) {
      Case(
          h, results,
          [&](Blocks& blocks) {
            const uint32_t size = h.random.Below(20);
            build(blocks, size, h.random.Below(3), 0);
          },
          [&](PPCContext& t) {
            t.r3.u64 = vector;
            t.r4.u64 = value;
          },
          __imp__sub_825986B8, hooks::CarSceneDrawVector16PushBack,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            const uint32_t f = t.r1.u32 - 96;
            collect(o, blocks, {f, f + 80, f - 112, f - 112 - 144, f - 112 - 144 + 80});
            o.Word(blocks.Of(t.r3.u32));
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    for (int k = 0; k < 10; k++) {
      Case(
          h, results, [&](Blocks& blocks) { build(blocks, 0, 0, 0); },
          [&](PPCContext& t) {
            t.r3.u64 = vector;
            t.r4.u64 = value;
          },
          [](PPCContext& t, uint8_t* base) {
            const PPCContext start = t;
            for (int i = 0; i < 30; i++) {
              t = start;
              StoreFloat(base, start.r4.u32, float(i));
              __imp__sub_825986B8(t, base);
            }
          },
          [](PPCContext& t, uint8_t* base) {
            const PPCContext start = t;
            for (int i = 0; i < 30; i++) {
              t = start;
              StoreFloat(base, start.r4.u32, float(i));
              hooks::CarSceneDrawVector16PushBack(t, base);
            }
          },
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            collect(o, blocks, {});
            o.Word(blocks.Of(t.r3.u32));
            blocks.Out(o);
          },
          fmt::format("thirty case {}", k));
    }
    results.Report();
  }
}


// --- Part 2: the projectors ---

// A vector of `scale`: ordinary, or with special values.
void WriteVector(Context& h, uint32_t at, float scale, bool special, float w = 1.0f) {
  for (uint32_t i = 0; i < 12; i += 4) {
    StoreFloat(h.base, at + i, special ? Value(h.random, scale) : Plain(h.random, scale));
  }
  StoreFloat(h.base, at + 12, w);
}

// Eye, target and up for a look-at: random, special values, the eye on the
// target (no forward), up along the forward (no right), up's w random.
void WriteView(Context& h, uint32_t eye, uint32_t target, uint32_t up, int k) {
  const bool special = k % 4 == 3;
  WriteVector(h, eye, 9000.0f, special);
  WriteVector(h, target, 9000.0f, special);
  WriteVector(h, up, 1.0f, special, Plain(h.random, 2.0f));
  if (k % 8 == 0) {
    // A target near the eye (a headlight's few meters).
    for (uint32_t i = 0; i < 12; i += 4) {
      StoreFloat(h.base, target + i,
                 std::bit_cast<float>(Load(h.base, eye + i)) + Plain(h.random, 5.0f));
    }
    StoreFloat(h.base, up + 0, 0.0f);
    StoreFloat(h.base, up + 4, 1.0f);
    StoreFloat(h.base, up + 8, 0.0f);
  } else if (k % 16 == 1) {
    for (uint32_t i = 0; i < 16; i += 4) {
      Store(h.base, target + i, Load(h.base, eye + i));
    }
  } else if (k % 16 == 5) {
    const float scale = Plain(h.random, 3.0f);
    for (uint32_t i = 0; i < 12; i += 4) {
      StoreFloat(h.base, up + i,
                 (std::bit_cast<float>(Load(h.base, target + i)) -
                  std::bit_cast<float>(Load(h.base, eye + i))) *
                     scale);
    }
  }
}

// f registers as doubles of floats (the callers pass lfs values and
// single-precision results).
double Single(float value) { return double(value); }

void Projectors(Context& h) {
  const uint32_t out = h.data;
  const uint32_t eye = h.data + 64;
  const uint32_t target = h.data + 96;
  const uint32_t up = h.data + 128;
  {
    // Every tenth case the eye 4 bytes past its 16 (lfs reads it there, lvx
    // the aligned 16 holding it).
    Results results("Matrix::LookAt (sub_82597378)");
    for (int k = 0; k < 400; k++) {
      const bool shifted = k % 10 == 9;
      Case(
          h, results,
          [&](Blocks&) {
            WriteView(h, eye, target, up, k);
            StoreFloat(h.base, eye + 16, Plain(h.random, 9000.0f));
          },
          [&](PPCContext& t) {
            t.r3.u64 = out;
            t.r4.u64 = shifted ? eye + 4 : eye;
            t.r5.u64 = target;
            t.r6.u64 = up;
          },
          __imp__sub_82597378, hooks::CarSceneDrawLookAt,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            o.Floats(h.base, out, 16);
            o.Floats(h.base, t.r1.u32 - 32, 8);
            o.Word(t.r3.u32);
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
  {
    // Fields of view of the projectors and the fit; special values (a NaN
    // or infinity sends tan to its error path: the original runs), near =
    // far, aspect 0. r3 isn't compared: the original leaves tan's _ctrlfp
    // word, read by no caller.
    Results results("Matrix::Perspective (sub_825972A8)");
    for (int k = 0; k < 300; k++) {
      double f[4];
      Case(
          h, results,
          [&](Blocks&) {
            const bool special = k % 4 == 3;
            f[0] = Single(special ? Value(h.random, 3.0f) : Uniform(h.random, 0.05f, 3.1f));
            f[1] = Single(special ? Value(h.random, 3.0f) : Uniform(h.random, 0.3f, 3.0f));
            f[2] = Single(special ? Value(h.random, 5.0f) : Uniform(h.random, 0.01f, 5.0f));
            f[3] = Single(special ? Value(h.random, 2000.0f) : float(f[2]) + Uniform(h.random, 1.0f, 2000.0f));
            if (k % 16 == 1) {
              f[3] = f[2];
            } else if (k % 16 == 5) {
              f[1] = 0.0;
            } else if (k % 16 == 9) {
              f[0] = 0.0;
            }
          },
          [&](PPCContext& t) {
            t.r3.u64 = out;
            t.f1.f64 = f[0];
            t.f2.f64 = f[1];
            t.f3.f64 = f[2];
            t.f4.f64 = f[3];
          },
          __imp__sub_825972A8, hooks::CarSceneDrawPerspective,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            o.Floats(h.base, out, 16);
            o.Word(Load(h.base, t.r1.u32 - 128));  // the back chain
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
}

// --- Part 2: the root's leaves ---

// A guest word put back when the scope ends.
struct SavedWord {
  SavedWord(uint8_t* base, uint32_t address)
      : base_(base), address_(address), value_(Load(base, address)) {}
  ~SavedWord() { Store(base_, address_, value_); }
  SavedWord(const SavedWord&) = delete;
  SavedWord& operator=(const SavedWord&) = delete;

 private:
  uint8_t* base_;
  uint32_t address_;
  uint32_t value_;
};

constexpr uint32_t kSceneGlobal = 0x826C2E44;
constexpr uint32_t kRaceDataGlobal = 0x826C5B2C;
constexpr uint32_t kSecondHumanGlobal = 0x826C7824;

void Leaves(Context& h) {
  {
    // The direction random, special, zero, tiny (its length's square
    // denormal) or 4 bytes past its 16; the distance random or special.
    Results results("ScenePass::SetLight (sub_820C4EB8)");
    const uint32_t pass = h.data + 256;
    const uint32_t direction = h.data + 64;
    for (int k = 0; k < 300; k++) {
      double distance = 0;
      const bool shifted = k % 10 == 9;
      Case(
          h, results,
          [&](Blocks&) {
            const bool special = k % 4 == 3;
            WriteVector(h, direction, 2.0f, special, Plain(h.random, 2.0f));
            if (k % 16 == 1) {
              for (uint32_t i = 0; i < 12; i += 4) {
                StoreFloat(h.base, direction + i, 0.0f);
              }
            } else if (k % 16 == 5) {
              for (uint32_t i = 0; i < 12; i += 4) {
                StoreFloat(h.base, direction + i, Plain(h.random, 1e-20f));
              }
            }
            for (uint32_t i = 0; i < 192; i += 4) {
              StoreFloat(h.base, pass + i, Plain(h.random, 10.0f));
            }
            distance = Single(special ? Value(h.random, 500.0f) : Plain(h.random, 500.0f));
          },
          [&](PPCContext& t) {
            t.r3.u64 = pass;
            t.r4.u64 = shifted ? direction + 4 : direction;
            t.f1.f64 = distance;
          },
          __imp__sub_820C4EB8, hooks::CarSceneDrawSetLight,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            const uint32_t f = t.r1.u32 - 192;
            o.Floats(h.base, pass, 48);
            o.Word(Load(h.base, f));  // the back chain
            o.Floats(h.base, f + 80, 20);
            o.Word(t.r3.u32);
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
  // The scene models and the scene in a block of their own (the scene's
  // beams at +5392 to +5487), the scene global pointed at it meanwhile.
  const uint32_t world = memory::Alloc(0x4000, 0);
  const uint32_t scene = world;
  const uint32_t model = world + 0x2000;
  const uint32_t out = h.data;
  auto write_model = [&](int k) {
    Store(h.base, model + 4288, uint32_t(h.random.Next()));
    const uint32_t bits = uint32_t(h.random.Next());
    Store(h.base, model + 4292, k % 2 == 0 ? bits | 0x40 : bits & ~0x40u);
    Store(h.base, model + 3776, h.random.Below(4));
    Store(h.base, model + 48, h.random.Below(16));
    for (uint32_t i = 5392; i < 5488; i += 4) {
      StoreFloat(h.base, scene + i, Plain(h.random, 10.0f));
    }
  };
  {
    Results results("SceneModel::BeamColor (sub_820DAA38)");
    for (int k = 0; k < 200; k++) {
      Case(
          h, results, [&](Blocks&) { write_model(k); },
          [&](PPCContext& t) {
            t.r3.u64 = out;
            t.r4.u64 = model;
          },
          __imp__sub_820DAA38, hooks::CarSceneDrawBeamColor,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            const uint32_t f = t.r1.u32 - 112;
            if ((Load(h.base, model + 4292) & 0x40) != 0) {
              o.Floats(h.base, out, 4);  // the color scaled (stfs)
            } else {
              o.Bytes(Read(h.base, out, 16));  // the table's words copied
            }
            o.Word(Load(h.base, f));  // the back chain
            o.Bytes(Read(h.base, f + 80, 16));
            o.Word(t.r3.u32);
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
  }
  {
    SavedWord saved(h.base, kSceneGlobal);
    Store(h.base, kSceneGlobal, scene);
    const struct {
      const char* name;
      GuestFunction recompiled;
      GuestFunction native;
    } beams[] = {
        {"SceneModel::Beam (sub_820DAAC8)", __imp__sub_820DAAC8, hooks::CarSceneDrawBeam},
        {"SceneModel::BeamShape (sub_820DAB10)", __imp__sub_820DAB10,
         hooks::CarSceneDrawBeamShape},
    };
    for (const auto& beam : beams) {
      Results results(beam.name);
      for (int k = 0; k < 100; k++) {
        Case(
            h, results, [&](Blocks&) { write_model(k); },
            [&](PPCContext& t) {
              t.r3.u64 = out;
              t.r4.u64 = model;
            },
            beam.recompiled, beam.native,
            [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
              o.Bytes(Read(h.base, out, 16));
              o.Word(t.r3.u32);
              blocks.Out(o);
            },
            fmt::format("case {}", k));
      }
      results.Report();
    }
  }
  memory::Free(world);
  {
    // The race data in a block of its own (+0xA3AF0 the race kind, +0x59614),
    // the race data global and [0x826C7824] set meanwhile.
    const uint32_t race = memory::Alloc(0xA4000, 0);
    SavedWord saved_race(h.base, kRaceDataGlobal);
    SavedWord saved_human(h.base, kSecondHumanGlobal);
    Store(h.base, kRaceDataGlobal, race);
    Results results("CarScene::PlayerModel (sub_820C4FB8)");
    for (int k = 0; k < 200; k++) {
      uint32_t index = 0;
      Case(
          h, results,
          [&](Blocks&) {
            index = k % 4 == 3 ? uint32_t(h.random.Next()) : 0;
            Store(h.base, race + 0xA3AF0, h.random.Below(3) != 0 ? 6 : h.random.Below(10));
            Store(h.base, race + 0x59614, h.random.Below(3) != 0 ? 2 : h.random.Below(4));
            Store(h.base, kSecondHumanGlobal,
                  h.random.Below(2) != 0 ? 0 : uint32_t(h.random.Next()));
          },
          [&](PPCContext& t) { t.r3.u64 = index; }, __imp__sub_820C4FB8,
          hooks::CarSceneDrawPlayerModel,
          [&](Outputs& o, const PPCContext& t, Blocks& blocks) {
            o.Word(t.r3.u32);
            o.Word(uint32_t(t.r3.u64 >> 32));
            blocks.Out(o);
          },
          fmt::format("case {}", k));
    }
    results.Report();
    memory::Free(race);
  }
}

}  // namespace

void CarSceneDraw(Context& h) {
  Planes(h);
  Frustum(h);
  Box(h);
  Vector(h);
  Projectors(h);
  Leaves(h);
}

}  // namespace harness
