// X3DAudio (src/car_sounds/x3daudio.h, docs/functions/car-sounds.md) against
// the recompiled functions: the vector helpers, the curve evaluation and the
// curves. Each case builds its vectors, curves, emitter and DSP settings in
// a block from the allocation layer, filled with the same random words for
// both versions, and runs the original (with --native_car_sounds off, so its
// callees are the originals too) or the plain C++ through the register steps
// (hooks/car_sounds.h). Compared with harness::Outputs: the block exactly,
// except the words the code stores with stfs, which are floats; f1 as a
// double; r3 exactly; for the curves, the 128-byte frame below the stack
// pointer exactly (the original's saved registers and default curves).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/car_sounds.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_sounds);

namespace harness {

namespace {

constexpr uint32_t kBlockBytes = 0x1000;
constexpr uint32_t kFrameBytes = 128;
constexpr uint32_t kLfeAzimuth = 0x82001FA8;

struct Block {
  Block() : address(memory::Alloc(kBlockBytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t address;
};

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Bits(float value) { return std::bit_cast<uint32_t>(value); }

// A float in [low, high].
float Uniform(Random& random, float low, float high) {
  return low + (high - low) * float(random.Below(1000001)) / 1000000.0f;
}

// Any float: random bits, a special value (zeros, infinities, NaNs quiet
// and signaling, denormals, the extremes) or an ordinary one.
uint32_t AnyFloat(Random& random) {
  static constexpr uint32_t kSpecial[] = {
      0x00000000, 0x80000000, 0x7F800000, 0xFF800000, 0x7FC00000, 0xFFC00000,
      0x7F800001, 0xFFA00001, 0x00000001, 0x807FFFFF, 0x7F7FFFFF, 0xFF7FFFFF,
      0x3F800000, 0xBF800000, 0x00800000, 0x40C90FDB};
  switch (random.Below(6)) {
    case 0:
      return uint32_t(random.Next());
    case 1:
      return kSpecial[random.Below(sizeof(kSpecial) / sizeof(kSpecial[0]))];
    default:
      return Bits(Uniform(random, -4.0f, 4.0f));
  }
}

// The words at `offsets` (in the block) compared as floats, the rest of the
// block exactly.
void BlockOutputs(Outputs& out, uint8_t* base, uint32_t block,
                  const std::vector<uint32_t>& floats) {
  std::vector<uint8_t> bytes = Read(base, block, kBlockBytes);
  for (const uint32_t offset : floats) {
    for (uint32_t i = 0; i < 4; i++) {
      bytes[offset + i] = 0;
    }
  }
  out.Bytes(bytes);
  for (const uint32_t offset : floats) {
    out.Float(guest::LoadU32(base, block + offset));
  }
}

// One case: the block (and the frame below the call's stack pointer) filled
// with random words, then `setup(random, t)` (the same random state for both
// versions) writes the inputs and sets the registers; then `outputs(out, t)`.
template <typename Setup, typename Extra>
void Case(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
          GuestFunction native, Setup setup, const std::vector<uint32_t>& floats, Extra extra,
          const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 0; i < kFrameBytes; i += 4) {
      Store(h.base, t.r1.u32 - kFrameBytes + i, uint32_t(random.Next()));
    }
    setup(random, t);
    ITC_FLAG(native_car_sounds) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_sounds) = true;
    BlockOutputs(out[v], h.base, block, floats);
    out[v].Word(t.r3.u32);
    extra(out[v], t);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// --- The vector helpers ---

// a at +0, b at +16, out at +32 (or over a or b).
void Vectors(Context& h, uint32_t block) {
  auto vectors = [&](Random& random, PPCContext& t, int c, bool realistic) {
    for (uint32_t i = 0; i < 8; i++) {
      const uint32_t at = block + (i < 4 ? 0 : 16) + 4 * (i % 4);
      Store(h.base, at,
            realistic ? Bits(Uniform(random, -600.0f, 600.0f)) : AnyFloat(random));
    }
    t.r3.u64 = block;
    t.r4.u64 = block + 16;
    switch (c % 5) {
      case 1:
        t.r5.u64 = block;  // out over a
        break;
      case 2:
        t.r5.u64 = block + 16;  // out over b
        break;
      case 3:
        t.r4.u64 = block;  // a with itself
        t.r5.u64 = block + 32;
        break;
      default:
        t.r5.u64 = block + 32;
        break;
    }
  };
  auto out_floats = [](int c) -> std::vector<uint32_t> {
    const uint32_t out = c % 5 == 1 ? 0 : c % 5 == 2 ? 16 : 32;
    return {out, out + 4, out + 8};
  };
  auto none = [](Outputs&, const PPCContext&) {};
  auto f1 = [](Outputs& out, const PPCContext& t) { out.Double(t.f1.u64); };
  struct Group {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    bool stores;
  };
  const Group groups[] = {
      {"X3DVec::Sub (sub_824819E0)", __imp__sub_824819E0, hooks::CarSoundsX3DVecSub, true},
      {"X3DVec::Length (sub_82481998)", __imp__sub_82481998, hooks::CarSoundsX3DVecLength,
       false},
      {"X3DVec::Dot (sub_824819B8)", __imp__sub_824819B8, hooks::CarSoundsX3DVecDot, false},
      {"X3DVec::Cross (sub_82481A18)", __imp__sub_82481A18, hooks::CarSoundsX3DVecCross, true},
  };
  for (const Group& group : groups) {
    Results results(group.name);
    for (int c = 0; c < 400; c++) {
      const bool realistic = c >= 300;
      auto setup = [&](Random& random, PPCContext& t) { vectors(random, t, c, realistic); };
      const std::string what =
          fmt::format("{} case {}", realistic ? "realistic" : "random", c);
      if (group.stores) {
        Case(h, results, block, group.recompiled, group.native, setup, out_floats(c), none,
             what);
      } else {
        Case(h, results, block, group.recompiled, group.native, setup, {}, f1, what);
      }
    }
    results.Report();
  }
}

// --- The curves ---

// A curve's points at `points`: `count` of them, ascending distances in
// [0, 1] with values in [0, 1] (realistic), or any floats.
void Points(uint8_t* base, Random& random, uint32_t points, uint32_t count, bool realistic) {
  float distance = 0.0f;
  for (uint32_t i = 0; i < count; i++) {
    if (realistic) {
      if (i != 0) {
        distance = i + 1 == count ? 1.0f
                                  : distance + (1.0f - distance) * Uniform(random, 0.05f, 0.6f);
      }
      Store(base, points + 8 * i, Bits(distance));
      Store(base, points + 8 * i + 4, Bits(Uniform(random, 0.0f, 1.0f)));
    } else if (random.Below(3) == 0) {
      // Ascending, with any values (some repeated distances).
      if (i != 0 && random.Below(4) != 0) {
        distance += Uniform(random, 0.0f, 0.5f);
      }
      Store(base, points + 8 * i, Bits(distance));
      Store(base, points + 8 * i + 4, AnyFloat(random));
    } else {
      Store(base, points + 8 * i, AnyFloat(random));
      Store(base, points + 8 * i + 4, AnyFloat(random));
    }
  }
}

// A double for f1 or f2: mostly a float's (as the callers' lfs give), now
// and then any double.
uint64_t AnyDouble(Random& random) {
  if (random.Below(8) == 0) {
    return random.Next();
  }
  return std::bit_cast<uint64_t>(double(std::bit_cast<float>(AnyFloat(random))));
}

// The curve at +0x100 (points +0x200, the word before them read for a
// count of 0).
void EvaluateCurves(Context& h, uint32_t block) {
  Results results("X3DAudio::EvaluateCurve (sub_82480818)");
  auto f1 = [](Outputs& out, const PPCContext& t) { out.Double(t.f1.u64); };
  for (int c = 0; c < 1200; c++) {
    const bool realistic = c >= 900;
    auto setup = [&](Random& random, PPCContext& t) {
      const uint32_t curve = block + 0x100;
      const uint32_t points = block + 0x200;
      const uint32_t count = realistic ? 2 + random.Below(10) : random.Below(17);
      Store(h.base, curve + 0, points);
      Store(h.base, curve + 4, count);
      Points(h.base, random, points, count, realistic);
      t.r3.u64 = curve;
      if (realistic) {
        t.f1.f64 = double(Uniform(random, 1.0f, 100.0f));
        t.f2.f64 = double(Uniform(random, 0.0f, 300.0f));
      } else {
        t.f1.u64 = AnyDouble(random);
        // Distances below the first point, between and past the last.
        t.f2.u64 = random.Below(2) == 0
                       ? std::bit_cast<uint64_t>(double(Uniform(random, -1.0f, 6.0f)))
                       : AnyDouble(random);
      }
    };
    Case(h, results, block, __imp__sub_82480818, hooks::CarSoundsX3DEvaluateCurve, setup, {},
         f1, fmt::format("{} case {}", realistic ? "realistic" : "random", c));
  }
  results.Report();
}

// The emitter at +0 (+52 the channels, +60 the azimuths, +64 to +80 the
// curves, +84 the scaler), the DSP settings at +0x100 (+0 the matrix, +16 to
// +24 out, +36 the distance), the matrix at +0x180, the azimuths at +0x1C0,
// the five curves at +0x200 (8 bytes each) with their points at +0x300 +
// 0x80 each.
constexpr uint32_t kEmitter = 0x0;
constexpr uint32_t kDsp = 0x100;
constexpr uint32_t kMatrix = 0x180;
constexpr uint32_t kAzimuths = 0x1C0;
constexpr uint32_t kCurves = 0x200;
constexpr uint32_t kPoints = 0x300;
constexpr uint32_t kMaxChannels = 12;

void CurveSets(Context& h, uint32_t block) {
  Results results("X3DAudio::Curves (sub_82480918)");
  const uint32_t lfe = guest::LoadU32(h.base, kLfeAzimuth);
  std::vector<uint32_t> floats = {kDsp + 16, kDsp + 20, kDsp + 24};
  for (uint32_t i = 0; i < kMaxChannels; i++) {
    floats.push_back(kMatrix + 4 * i);
  }
  auto frame = [&](Outputs& out, const PPCContext& t) {
    out.Bytes(Read(h.base, t.r1.u32 - kFrameBytes, kFrameBytes));
  };
  for (int c = 0; c < 1500; c++) {
    const bool realistic = c >= 1200;
    auto setup = [&](Random& random, PPCContext& t) {
      const uint32_t emitter = block + kEmitter;
      const uint32_t dsp = block + kDsp;
      // The flags: every combination of the five the function reads, and
      // now and then any word.
      uint32_t flags = realistic ? 0x3D : uint32_t(c % 32);
      if (!realistic && random.Below(6) == 0) {
        flags = uint32_t(random.Next());
      }
      uint32_t channels;
      if (realistic) {
        channels = random.Below(3) == 0 ? 1 : 6;
      } else {
        static constexpr uint32_t kCounts[] = {0, 1, 1, 2, 3, 4, 5, 6, 6, 8, 12};
        channels = kCounts[random.Below(sizeof(kCounts) / sizeof(kCounts[0]))];
      }
      Store(h.base, emitter + 52, channels);
      Store(h.base, emitter + 60, block + kAzimuths);
      for (uint32_t i = 0; i < kMaxChannels; i++) {
        uint32_t azimuth;
        if (realistic) {
          azimuth = i == 3 ? lfe : Bits(Uniform(random, 0.0f, 6.0f));
        } else {
          azimuth = random.Below(3) == 0 ? lfe : AnyFloat(random);
        }
        Store(h.base, block + kAzimuths + 4 * i, azimuth);
      }
      for (uint32_t k = 0; k < 5; k++) {
        const uint32_t curve = block + kCurves + 8 * k;
        const uint32_t points = block + kPoints + 0x80 * k;
        const uint32_t count = realistic ? 2 + random.Below(5) : random.Below(13);
        Store(h.base, curve + 0, points);
        Store(h.base, curve + 4, count);
        Points(h.base, random, points, count, realistic);
        const bool null = realistic ? random.Below(2) == 0 : random.Below(3) == 0;
        Store(h.base, emitter + 64 + 4 * k, null ? 0 : curve);
      }
      Store(h.base, dsp + 0, block + kMatrix);
      if (realistic) {
        Store(h.base, emitter + 84, Bits(Uniform(random, 1.0f, 50.0f)));
        Store(h.base, dsp + 36, Bits(Uniform(random, 0.0f, 300.0f)));
      } else {
        // Scalers 0, negative, NaN and infinite among the rest.
        Store(h.base, emitter + 84, AnyFloat(random));
        Store(h.base, dsp + 36,
              random.Below(2) == 0 ? Bits(Uniform(random, -1.0f, 6.0f)) : AnyFloat(random));
      }
      t.r3.u64 = uint32_t(random.Next());  // the handle: not read
      t.r4.u64 = emitter;
      t.r5.u64 = flags;
      t.r6.u64 = dsp;
    };
    Case(h, results, block, __imp__sub_82480918, hooks::CarSoundsX3DCurves, setup, floats,
         frame, fmt::format("{} case {}", realistic ? "realistic" : "random", c));
  }
  results.Report();
}

}  // namespace

void X3DAudioMath(Context& h) {
  Block block;
  if (!block.address) {
    ITC_LOG_ERROR("harness: x3daudio: no block from the allocation layer");
    return;
  }
  Vectors(h, block.address);
  EvaluateCurves(h, block.address);
  CurveSets(h, block.address);
}

}  // namespace harness
