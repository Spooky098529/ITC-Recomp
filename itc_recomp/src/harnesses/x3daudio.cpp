// X3DAudio (src/car_sounds/x3daudio.h, docs/functions/car-sounds.md) against
// the recompiled functions: the vector helpers, the curve evaluation, the
// curves, Pan, the matrix, the cone, Doppler and X3DAudioCalculate. Each
// case builds its vectors, curves, emitter and DSP settings in
// a block from the allocation layer, filled with the same random words for
// both versions, and runs the original (with --native_car_sounds off, so its
// callees are the originals too) or the plain C++ through the register steps
// (hooks/car_sounds.h). Compared with harness::Outputs: the block exactly,
// except the words the code stores with stfs, which are floats; f1 as a
// double; r3 exactly; for the curves, the 128-byte frame below the stack
// pointer exactly (the original's saved registers and default curves); for
// Pan its 192-byte frame exactly (the saves and the azimuth tables), for
// Matrix its 304-byte frame and Pan's below it (the words its float
// instructions store as floats); for Cone, Doppler and X3DAudioCalculate
// their frames (Calculate's with its callees' below it) the same way.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/car_sounds.h"
#include "memory/heap.h"
#include "sound_cues/sound_cues.h"

#include <fmt/format.h>

#include <xmmintrin.h>

#include <algorithm>
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
          const std::string& what, uint32_t frame_bytes = kFrameBytes) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 0; i < frame_bytes; i += 4) {
      Store(h.base, t.r1.u32 - frame_bytes + i, uint32_t(random.Next()));
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

// --- Pan and the matrix ---

// The handle at +0 (from X3DAudioInitialize, or a count of 0 by hand), the
// listener at +0x20, the emitter at +0x80, its azimuths at +0x100, the DSP
// settings at +0x180 and the matrix at +0x200 (up to 8 by 8).
constexpr uint32_t kHandle = 0x0;
constexpr uint32_t kListener = 0x20;
constexpr uint32_t kMxEmitter = 0x80;
constexpr uint32_t kMxAzimuths = 0x100;
constexpr uint32_t kMxDsp = 0x180;
constexpr uint32_t kMxMatrix = 0x200;
constexpr uint32_t kMxMaxChannels = 8;
constexpr uint32_t kPanFrame = 192;
constexpr uint32_t kMatrixFrame = 304 + 192;

// The handle: X3DAudioInitialize for one of the masks with an azimuth table
// (2 speakers: stereo 3, 0x9 with an LFE, 0x30; 6: 5.1 63, 0x60F, 0x3F0),
// now and then a count of 0 (no speakers: nothing searched). The handle's
// other counts (mono, quad, 4.0, 7.1: 1, 4, 8 speakers) make the original
// read its table at address 0, which the zero page faults on, so they can't
// run here (nor in the game: the car sounds' handle is 5.1).
uint32_t MakeHandle(uint8_t* base, Random& random, uint32_t handle, bool realistic) {
  static constexpr uint32_t kMasks[] = {63, 63, 0x60F, 0x3F0, 3, 0x9, 0x30};
  const uint32_t mask =
      realistic ? 63 : kMasks[random.Below(sizeof(kMasks) / sizeof(kMasks[0]))];
  sound_cues::X3DAudioInitialize(base, mask, 343.5, handle);
  if (!realistic && random.Below(12) == 0) {
    Store(base, handle + 0, 0);
  }
  return guest::LoadU32(base, handle + 0);
}

// An azimuth for Pan's f1: over the whole circle and past it, the edges (0,
// ±pi, 2pi as floats and doubles), the speakers' own azimuths and the
// doubles beside them, NaNs and infinities, any double.
uint64_t PanAzimuth(uint8_t* base, Random& random, bool realistic) {
  static constexpr uint32_t kTables[] = {0x8201E2B0, 0x8201E2A8, 0x8204D2F0, 0x82002CD8,
                                         0x82002258, 0x8204D2E8, 0x8204D2E0, 0x8204D2D8};
  constexpr double kPi = 3.14159265358979323846;
  if (realistic) {
    // atan2's range, rounded to single as Matrix gives it.
    return std::bit_cast<uint64_t>(double(Uniform(random, -float(kPi), float(kPi))));
  }
  switch (random.Below(8)) {
    case 0: {
      static constexpr double kEdges[] = {0.0,           -0.0,          kPi,
                                          -kPi,          2 * kPi,       -2 * kPi,
                                          double(float(kPi)), -double(float(kPi)),
                                          double(float(2 * kPi)), kPi / 2, -kPi / 2,
                                          3 * kPi / 2};
      return std::bit_cast<uint64_t>(kEdges[random.Below(sizeof(kEdges) / sizeof(kEdges[0]))]);
    }
    case 1:
    case 2: {
      const uint32_t at = kTables[random.Below(8)];
      uint64_t bits = uint64_t(guest::LoadU32(base, at)) << 32 | guest::LoadU32(base, at + 4);
      // The speaker's azimuth, the doubles either side, or minus it.
      switch (random.Below(5)) {
        case 0:
          bits += 1;
          break;
        case 1:
          bits -= 1;
          break;
        case 2:
          bits ^= 0x8000000000000000ull;
          break;
        case 3:
          bits = std::bit_cast<uint64_t>(double(float(std::bit_cast<double>(bits))));
          break;
        default:
          break;
      }
      return bits;
    }
    case 3:
      return AnyDouble(random);
    default:
      return std::bit_cast<uint64_t>(double(Uniform(random, -7.0f, 13.0f)));
  }
}

// The DSP settings: the matrix, `sources` by `destinations`, filled with
// volumes (realistic) or any floats.
void MakeDsp(uint8_t* base, Random& random, uint32_t block, uint32_t sources,
             uint32_t destinations, bool realistic) {
  const uint32_t dsp = block + kMxDsp;
  Store(base, dsp + 0, block + kMxMatrix);
  Store(base, dsp + 8, sources);
  Store(base, dsp + 12, destinations);
  for (uint32_t i = 0; i < kMxMaxChannels * kMxMaxChannels; i++) {
    Store(base, block + kMxMatrix + 4 * i,
          realistic ? Bits(Uniform(random, 0.0f, 1.0f)) : AnyFloat(random));
  }
}

std::vector<uint32_t> MatrixFloats() {
  std::vector<uint32_t> floats;
  for (uint32_t i = 0; i < kMxMaxChannels * kMxMaxChannels; i++) {
    floats.push_back(kMxMatrix + 4 * i);
  }
  return floats;
}

void Pans(Context& h, uint32_t block) {
  Results results("X3DAudio::Pan (sub_82480F80)");
  // Cases 1000 to 1499 from each of the four rounding modes (MXCSR set
  // before the call, compared without its exception flags and put back
  // after): where it isn't nearest, tan runs through its adapter (which sets
  // nearest and puts the mode back).
  const uint32_t saved = _mm_getcsr();
  for (int c = 0; c < 2000; c++) {
    const bool realistic = c >= 1500;
    const uint32_t mode = c >= 1000 && c < 1500 ? uint32_t(c % 4) : 0;
    auto frame = [&](Outputs& out, const PPCContext& t) {
      out.Bytes(Read(h.base, t.r1.u32 - kPanFrame, kPanFrame));
      out.Word(_mm_getcsr() & ~0x3Fu);
      _mm_setcsr(saved);
    };
    auto setup = [&](Random& random, PPCContext& t) {
      const uint32_t speakers = MakeHandle(h.base, random, block + kHandle, realistic);
      const uint32_t sources = realistic ? 1 + random.Below(2) : 1 + random.Below(kMxMaxChannels);
      // As many destinations as speakers, or any count up to 8.
      const uint32_t destinations =
          realistic || random.Below(2) == 0 ? (speakers == 0 ? 6 : speakers)
                                            : random.Below(kMxMaxChannels + 1);
      MakeDsp(h.base, random, block, sources, destinations, realistic);
      t.r3.u64 = block + kHandle;
      t.r4.u64 = uint32_t(random.Next());  // not read
      t.r5.u64 = block + kMxDsp;
      t.r6.u64 = random.Below(sources);
      t.f1.u64 = PanAzimuth(h.base, random, realistic);
      // The registers its prologue saves.
      t.r28.u64 = random.Next();
      t.r29.u64 = random.Next();
      t.r30.u64 = random.Next();
      t.r31.u64 = random.Next();
      t.f31.u64 = random.Next();
      if (mode != 0) {
        _mm_setcsr((saved & ~0x603Fu) | (mode << 13));
        t.fpscr.csr = _mm_getcsr();
      }
    };
    Case(h, results, block, __imp__sub_82480F80, hooks::CarSoundsX3DPan, setup, MatrixFloats(),
         frame, fmt::format("{} case {}", realistic ? "realistic" : "random", c), kPanFrame);
  }
  results.Report();
}

// A unit vector (realistic) or any floats; now and then zero.
void Vector(uint8_t* base, Random& random, uint32_t at, bool realistic) {
  if (!realistic && random.Below(4) == 0) {
    for (uint32_t i = 0; i < 3; i++) {
      Store(base, at + 4 * i, random.Below(3) == 0 ? 0 : AnyFloat(random));
    }
    return;
  }
  float v[3];
  float length = 0.0f;
  do {
    for (float& x : v) {
      x = Uniform(random, -1.0f, 1.0f);
    }
    length = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  } while (length < 0.05f);
  for (uint32_t i = 0; i < 3; i++) {
    Store(base, at + 4 * i, Bits(v[i] / length));
  }
}

void Matrices(Context& h, uint32_t block) {
  Results results("X3DAudio::Matrix (sub_82481390)");
  const uint32_t lfe = guest::LoadU32(h.base, kLfeAzimuth);
  std::vector<uint32_t> floats = MatrixFloats();
  auto frame = [&](Outputs& out, const PPCContext& t) {
    // Matrix's frame at +192 in the region: the words its stfs store (the
    // channel's position at +104, the cross products at +120 and +152, the
    // difference at +136) as floats.
    const uint32_t region = t.r1.u32 - kMatrixFrame;
    std::vector<uint8_t> bytes = Read(h.base, region, kMatrixFrame);
    static constexpr uint32_t kFloats[] = {104, 108, 112, 120, 124, 128,
                                           136, 140, 144, 152, 156, 160};
    for (const uint32_t offset : kFloats) {
      for (uint32_t i = 0; i < 4; i++) {
        bytes[192 + offset + i] = 0;
      }
    }
    out.Bytes(bytes);
    for (const uint32_t offset : kFloats) {
      out.Float(guest::LoadU32(h.base, region + 192 + offset));
    }
  };
  for (int c = 0; c < 2500; c++) {
    const bool realistic = c >= 1800;
    auto setup = [&](Random& random, PPCContext& t) {
      const uint32_t speakers = MakeHandle(h.base, random, block + kHandle, realistic);
      const uint32_t listener = block + kListener;
      const uint32_t emitter = block + kMxEmitter;
      // The listener: front, top, position (meters), velocity.
      Vector(h.base, random, listener + 0, realistic);
      const uint32_t shape = random.Below(10);
      if (shape == 0) {
        // Top parallel to the front (or its opposite).
        for (uint32_t i = 0; i < 3; i++) {
          const uint32_t x = guest::LoadU32(h.base, listener + 4 * i);
          Store(h.base, listener + 12 + 4 * i, random.Below(2) == 0 ? x : x ^ 0x80000000u);
        }
      } else {
        Vector(h.base, random, listener + 12, realistic);
      }
      for (uint32_t i = 0; i < 3; i++) {
        Store(h.base, listener + 24 + 4 * i,
              realistic || random.Below(3) != 0 ? Bits(Uniform(random, -800.0f, 800.0f))
                                                : AnyFloat(random));
      }
      // The emitter: front, top, position near the listener (or on it),
      // its channels, radius and azimuths.
      Vector(h.base, random, emitter + 4, realistic);
      Vector(h.base, random, emitter + 16, realistic);
      for (uint32_t i = 0; i < 3; i++) {
        const float at = std::bit_cast<float>(guest::LoadU32(h.base, listener + 24 + 4 * i));
        uint32_t bits;
        if (shape == 1) {
          bits = Bits(at);  // on the listener
        } else if (shape == 2) {
          bits = Bits(at + Uniform(random, -0.001f, 0.001f));  // all but on it
        } else if (realistic || random.Below(4) != 0) {
          bits = Bits(at + Uniform(random, -60.0f, 60.0f));
        } else {
          bits = AnyFloat(random);
        }
        Store(h.base, emitter + 28 + 4 * i, bits);
      }
      uint32_t channels;
      if (realistic) {
        channels = random.Below(3) == 0 ? 6 : 1;
      } else {
        channels = random.Below(12) == 0 ? 0 : 1 + random.Below(kMxMaxChannels);
      }
      Store(h.base, emitter + 52, channels);
      Store(h.base, emitter + 56,
            realistic || random.Below(3) != 0 ? Bits(Uniform(random, 0.0f, 5.0f))
                                              : AnyFloat(random));
      Store(h.base, emitter + 60, block + kMxAzimuths);
      constexpr float kPi = 3.14159265f;
      static constexpr float k51[] = {7 * kPi / 4, kPi / 4, 0.0f, 0.0f, 5 * kPi / 4, 3 * kPi / 4};
      for (uint32_t i = 0; i < kMxMaxChannels; i++) {
        uint32_t azimuth;
        if (realistic) {
          azimuth = i == 3 ? lfe : Bits(i < 6 ? k51[i] : 0.0f);
        } else {
          switch (random.Below(5)) {
            case 0:
              azimuth = lfe;
              break;
            case 1:
              azimuth = AnyFloat(random);
              break;
            default:
              azimuth = Bits(Uniform(random, -1.0f, 7.0f));
              break;
          }
        }
        Store(h.base, block + kMxAzimuths + 4 * i, azimuth);
      }
      const uint32_t sources = channels == 0 ? 1 : channels;
      const uint32_t destinations =
          realistic || random.Below(2) == 0 ? (speakers == 0 ? 6 : speakers)
                                            : random.Below(kMxMaxChannels + 1);
      MakeDsp(h.base, random, block, sources, destinations, realistic);
      t.r3.u64 = block + kHandle;
      t.r4.u64 = listener;
      t.r5.u64 = emitter;
      t.r6.u64 = block + kMxDsp;
      // The callee-saved registers the prologues save (and Pan's f31).
      t.r26.u64 = random.Next();
      t.r27.u64 = random.Next();
      t.r28.u64 = random.Next();
      t.r29.u64 = random.Next();
      t.r30.u64 = random.Next();
      t.r31.u64 = random.Next();
      t.f31.u64 = random.Next();
      t.f30.u64 = random.Next();
      t.f23.u64 = random.Next();
    };
    Case(h, results, block, __imp__sub_82481390, hooks::CarSoundsX3DMatrix, setup, floats, frame,
         fmt::format("{} case {}", realistic ? "realistic" : "random", c), kMatrixFrame);
  }
  results.Report();
}

// --- The cone, Doppler and X3DAudioCalculate ---

// The layout above (the handle, listener, emitter, azimuths, DSP settings
// and matrix), with the cone at +0x300, the emitter's five curves at +0x340
// and their points at +0x400 (0x80 each).
constexpr uint32_t kCone = 0x300;
constexpr uint32_t kCxCurves = 0x340;
constexpr uint32_t kCxPoints = 0x400;
constexpr uint32_t kConeFrame = 160;
constexpr uint32_t kDopplerFrame = 144;
// Calculate's 144-byte frame and the deepest of its callees' below it
// (Matrix's 304 and Pan's 192).
constexpr uint32_t kCalculateFrame = 144 + 304 + 192;

// The region below the stack pointer exactly, but the words at `floats`
// (offsets in it) as floats.
void FrameOutputs(Outputs& out, uint8_t* base, uint32_t region, uint32_t bytes_count,
                  const std::vector<uint32_t>& floats) {
  std::vector<uint8_t> bytes = Read(base, region, bytes_count);
  for (const uint32_t offset : floats) {
    for (uint32_t i = 0; i < 4; i++) {
      bytes[offset + i] = 0;
    }
  }
  out.Bytes(bytes);
  for (const uint32_t offset : floats) {
    out.Float(guest::LoadU32(base, region + offset));
  }
}

// The DSP settings' words the code stores with stfs, and the matrix's.
std::vector<uint32_t> DspFloats() {
  std::vector<uint32_t> floats = MatrixFloats();
  for (const uint32_t offset : {16u, 20u, 24u, 28u, 36u, 40u, 44u}) {
    floats.push_back(kMxDsp + offset);
  }
  return floats;
}

float Distance(uint8_t* base, uint32_t a, uint32_t b) {
  float sum = 0.0f;
  for (uint32_t i = 0; i < 3; i++) {
    const float d = std::bit_cast<float>(guest::LoadU32(base, a + 4 * i)) -
                    std::bit_cast<float>(guest::LoadU32(base, b + 4 * i));
    sum += d * d;
  }
  return std::sqrt(sum);
}

// The listener (front, top, position, velocity at +0, +12, +24, +36) and
// the emitter (front +4, top +16, position +28, velocity +40): positions in
// meters, the emitter near the listener, on it (shape 1) or all but on it
// (shape 2); velocities toward, away, across or zero (realistic), or any
// floats. Returns the shape.
uint32_t Scene(uint8_t* base, Random& random, uint32_t block, bool realistic) {
  const uint32_t listener = block + kListener;
  const uint32_t emitter = block + kMxEmitter;
  Vector(base, random, listener + 0, realistic);
  Vector(base, random, listener + 12, realistic);
  for (uint32_t i = 0; i < 3; i++) {
    Store(base, listener + 24 + 4 * i,
          realistic || random.Below(3) != 0 ? Bits(Uniform(random, -800.0f, 800.0f))
                                            : AnyFloat(random));
  }
  Vector(base, random, emitter + 4, realistic);
  Vector(base, random, emitter + 16, realistic);
  const uint32_t shape = random.Below(10);
  for (uint32_t i = 0; i < 3; i++) {
    const float at = std::bit_cast<float>(guest::LoadU32(base, listener + 24 + 4 * i));
    uint32_t bits;
    if (shape == 1) {
      bits = Bits(at);
    } else if (shape == 2) {
      bits = Bits(at + Uniform(random, -0.001f, 0.001f));
    } else if (realistic || random.Below(4) != 0) {
      bits = Bits(at + Uniform(random, -60.0f, 60.0f));
    } else {
      bits = AnyFloat(random);
    }
    Store(base, emitter + 28 + 4 * i, bits);
  }
  // The velocities: along the line between them (toward or away), any
  // direction, or zero.
  for (const uint32_t at : {listener + 36, emitter + 40}) {
    const uint32_t kind = random.Below(realistic ? 4 : 6);
    if (kind == 0) {
      for (uint32_t i = 0; i < 3; i++) {
        Store(base, at + 4 * i, 0);
      }
    } else if (kind == 1 || kind == 2) {
      const float speed = Uniform(random, 0.0f, 90.0f) * (kind == 1 ? 1.0f : -1.0f);
      const float length = Distance(base, listener + 24, emitter + 28);
      for (uint32_t i = 0; i < 3; i++) {
        const float d = std::bit_cast<float>(guest::LoadU32(base, listener + 24 + 4 * i)) -
                        std::bit_cast<float>(guest::LoadU32(base, emitter + 28 + 4 * i));
        Store(base, at + 4 * i, Bits(length > 0.0f ? d / length * speed : 0.0f));
      }
    } else if (kind == 3) {
      for (uint32_t i = 0; i < 3; i++) {
        Store(base, at + 4 * i, Bits(Uniform(random, -60.0f, 60.0f)));
      }
    } else {
      for (uint32_t i = 0; i < 3; i++) {
        Store(base, at + 4 * i, AnyFloat(random));
      }
    }
  }
  return shape;
}

// The DSP settings' distance (+36): the true one (realistic), or now and
// then 0 or any float.
void SceneDistance(uint8_t* base, Random& random, uint32_t block, bool realistic) {
  uint32_t bits = Bits(Distance(base, block + kListener + 24, block + kMxEmitter + 28));
  if (!realistic) {
    switch (random.Below(6)) {
      case 0:
        bits = 0;
        break;
      case 1:
        bits = AnyFloat(random);
        break;
      default:
        break;
    }
  }
  Store(base, block + kMxDsp + 36, bits);
}

// The cone: the angles over [0, 2pi] (inner within outer, realistic), with
// the edges 0 and X3DAUDIO_2PI and inner beyond outer now and then, or any
// floats; the six values in [0, 1] or any floats.
void MakeCone(uint8_t* base, Random& random, uint32_t cone, bool realistic) {
  const uint32_t two_pi = guest::LoadU32(base, kLfeAzimuth);
  const float two_pi_f = std::bit_cast<float>(two_pi);
  float inner = Uniform(random, 0.0f, two_pi_f);
  float outer = Uniform(random, inner, two_pi_f);
  uint32_t inner_bits = Bits(inner);
  uint32_t outer_bits = Bits(outer);
  switch (random.Below(realistic ? 10 : 8)) {
    case 0:
      inner_bits = two_pi;
      outer_bits = two_pi;
      break;
    case 1:
      outer_bits = 0;
      break;
    case 2:
      inner_bits = 0;
      break;
    case 3:
      outer_bits = two_pi;
      break;
    case 4:
      inner_bits = 0;
      outer_bits = 0;
      break;
    case 5:
      if (!realistic) {
        std::swap(inner_bits, outer_bits);
      }
      break;
    case 6:
      if (!realistic) {
        inner_bits = AnyFloat(random);
        outer_bits = AnyFloat(random);
      }
      break;
    default:
      break;
  }
  Store(base, cone + 0, inner_bits);
  Store(base, cone + 4, outer_bits);
  for (uint32_t i = 2; i < 8; i++) {
    Store(base, cone + 4 * i,
          realistic || random.Below(5) != 0 ? Bits(Uniform(random, 0.0f, 1.0f))
                                            : AnyFloat(random));
  }
}

// The DSP settings' +16 to +44 (the filters, Doppler and the velocities'
// words): values in [0, 1] or any floats.
void DspValues(uint8_t* base, Random& random, uint32_t block, bool realistic) {
  for (uint32_t offset = 16; offset <= 44; offset += 4) {
    if (offset == 36) {
      continue;
    }
    Store(base, block + kMxDsp + offset,
          realistic || random.Below(4) != 0 ? Bits(Uniform(random, 0.0f, 1.0f))
                                            : AnyFloat(random));
  }
}

// The flags: every combination of the five the code reads (the subsets of
// 0x3D) by case, and now and then any word (0x2 and 0x40 cleared where the
// native path needs them clear).
uint32_t Flags(Random& random, int c, bool realistic, bool clear_others) {
  if (realistic) {
    return 0x3D;
  }
  static constexpr uint32_t kBits[] = {0x1, 0x4, 0x8, 0x10, 0x20};
  uint32_t flags = 0;
  for (uint32_t i = 0; i < 5; i++) {
    if ((uint32_t(c) >> i) & 1) {
      flags |= kBits[i];
    }
  }
  if (random.Below(6) == 0) {
    flags = uint32_t(random.Next());
    if (clear_others) {
      flags &= ~0x42u;
    }
  }
  return flags;
}

// The callee-saved registers the prologues save.
void CalleeSaved(Random& random, PPCContext& t) {
  t.r26.u64 = random.Next();
  t.r27.u64 = random.Next();
  t.r28.u64 = random.Next();
  t.r29.u64 = random.Next();
  t.r30.u64 = random.Next();
  t.r31.u64 = random.Next();
  t.f23.u64 = random.Next();
  t.f24.u64 = random.Next();
  t.f25.u64 = random.Next();
  t.f26.u64 = random.Next();
  t.f27.u64 = random.Next();
  t.f28.u64 = random.Next();
  t.f29.u64 = random.Next();
  t.f30.u64 = random.Next();
  t.f31.u64 = random.Next();
}

void Cones(Context& h, uint32_t block) {
  Results results("X3DAudio::Cone (sub_82480BA8)");
  auto frame = [&](Outputs& out, const PPCContext& t) {
    FrameOutputs(out, h.base, t.r1.u32 - kConeFrame, kConeFrame, {88, 92, 96});
  };
  for (int c = 0; c < 2000; c++) {
    const bool realistic = c >= 1500;
    auto setup = [&](Random& random, PPCContext& t) {
      MakeHandle(h.base, random, block + kHandle, realistic);
      Scene(h.base, random, block, realistic);
      SceneDistance(h.base, random, block, realistic);
      MakeCone(h.base, random, block + kCone, realistic);
      Store(h.base, block + kMxEmitter + 0, block + kCone);
      MakeDsp(h.base, random, block, 1, 6, realistic);
      DspValues(h.base, random, block, realistic);
      t.r3.u64 = block + kHandle;
      t.r4.u64 = block + kListener;
      t.r5.u64 = block + kMxEmitter;
      t.r6.u64 = Flags(random, c, realistic, false);
      t.r7.u64 = block + kMxDsp;
      CalleeSaved(random, t);
    };
    Case(h, results, block, __imp__sub_82480BA8, hooks::CarSoundsX3DCone, setup, DspFloats(),
         frame, fmt::format("{} case {}", realistic ? "realistic" : "random", c), kConeFrame);
  }
  results.Report();
}

void Dopplers(Context& h, uint32_t block) {
  Results results("X3DAudio::Doppler (sub_82480E90)");
  auto frame = [&](Outputs& out, const PPCContext& t) {
    FrameOutputs(out, h.base, t.r1.u32 - kDopplerFrame, kDopplerFrame, {80, 84, 88});
  };
  for (int c = 0; c < 2000; c++) {
    const bool realistic = c >= 1500;
    auto setup = [&](Random& random, PPCContext& t) {
      const uint32_t handle = block + kHandle;
      const uint32_t listener = block + kListener;
      const uint32_t emitter = block + kMxEmitter;
      MakeHandle(h.base, random, handle, realistic);
      Scene(h.base, random, block, realistic);
      SceneDistance(h.base, random, block, realistic);
      // The emitter's Doppler scaler: 1, 0 or another (any float).
      uint32_t scaler = Bits(1.0f);
      switch (random.Below(5)) {
        case 0:
          scaler = 0;
          break;
        case 1:
          scaler = Bits(Uniform(random, 0.0f, 4.0f));
          break;
        case 2:
          if (!realistic) {
            scaler = AnyFloat(random);
          }
          break;
        default:
          break;
      }
      Store(h.base, emitter + 88, scaler);
      // The speed of sound's edge: a velocity along the line at the
      // handle's +12 or +8 (or the floats either side), or past them.
      if (random.Below(4) == 0) {
        const float length = Distance(h.base, listener + 24, emitter + 28);
        const uint32_t at = random.Below(2) == 0 ? listener + 36 : emitter + 40;
        uint32_t edge = guest::LoadU32(h.base, handle + (random.Below(2) == 0 ? 12 : 8));
        switch (random.Below(4)) {
          case 0:
            edge += 1;
            break;
          case 1:
            edge -= 1;
            break;
          case 2:
            edge = Bits(std::bit_cast<float>(edge) * 2.0f);
            break;
          default:
            break;
        }
        const float speed = std::bit_cast<float>(edge) * (random.Below(2) == 0 ? 1.0f : -1.0f);
        for (uint32_t i = 0; i < 3; i++) {
          const float d = std::bit_cast<float>(guest::LoadU32(h.base, listener + 24 + 4 * i)) -
                          std::bit_cast<float>(guest::LoadU32(h.base, emitter + 28 + 4 * i));
          Store(h.base, at + 4 * i, Bits(length > 0.0f ? d / length * speed : 0.0f));
        }
        Store(h.base, emitter + 88, Bits(1.0f));
      }
      if (!realistic && random.Below(8) == 0) {
        Store(h.base, handle + 8, AnyFloat(random));
        Store(h.base, handle + 12, AnyFloat(random));
      }
      MakeDsp(h.base, random, block, 1, 6, realistic);
      DspValues(h.base, random, block, realistic);
      t.r3.u64 = handle;
      t.r4.u64 = listener;
      t.r5.u64 = emitter;
      t.r6.u64 = block + kMxDsp;
      CalleeSaved(random, t);
    };
    Case(h, results, block, __imp__sub_82480E90, hooks::CarSoundsX3DDoppler, setup, DspFloats(),
         frame, fmt::format("{} case {}", realistic ? "realistic" : "random", c), kDopplerFrame);
  }
  results.Report();
}

void Calculates(Context& h, uint32_t block) {
  Results results("X3DAudioCalculate (sub_82481880)");
  const uint32_t lfe = guest::LoadU32(h.base, kLfeAzimuth);
  // The words the float instructions store, in the region below the stack
  // pointer: Calculate's vector, Curves' default curves, Cone's and
  // Doppler's vectors, Matrix's locals (frames overlap: a word a later
  // callee saves there is compared as a float too).
  std::vector<uint32_t> frame_floats;
  const uint32_t calc = kCalculateFrame - 144;
  for (const uint32_t o : {80u, 84u, 88u}) {
    frame_floats.push_back(calc + o);
  }
  for (const uint32_t o : {96u, 100u, 104u, 108u}) {
    frame_floats.push_back(calc - 128 + o);
  }
  for (const uint32_t o : {88u, 92u, 96u}) {
    frame_floats.push_back(calc - 160 + o);
  }
  for (const uint32_t o : {80u, 84u, 88u}) {
    const uint32_t at = calc - 144 + o;
    if (std::find(frame_floats.begin(), frame_floats.end(), at) == frame_floats.end()) {
      frame_floats.push_back(at);
    }
  }
  for (const uint32_t o : {104u, 108u, 112u, 120u, 124u, 128u, 136u, 140u, 144u, 152u, 156u, 160u}) {
    frame_floats.push_back(calc - 304 + o);
  }
  auto frame = [&](Outputs& out, const PPCContext& t) {
    FrameOutputs(out, h.base, t.r1.u32 - kCalculateFrame, kCalculateFrame, frame_floats);
  };
  for (int c = 0; c < 3000; c++) {
    const bool realistic = c >= 2200;
    auto setup = [&](Random& random, PPCContext& t) {
      const uint32_t speakers = MakeHandle(h.base, random, block + kHandle, realistic);
      const uint32_t emitter = block + kMxEmitter;
      Scene(h.base, random, block, realistic);
      // Channels: one (with or without a cone) or six (realistic), any
      // count up to 8.
      uint32_t channels;
      if (realistic) {
        channels = random.Below(2) == 0 ? 1 : 6;
      } else {
        static constexpr uint32_t kCounts[] = {0, 1, 1, 1, 2, 3, 4, 5, 6, 6, 8};
        channels = kCounts[random.Below(sizeof(kCounts) / sizeof(kCounts[0]))];
      }
      Store(h.base, emitter + 52, channels);
      Store(h.base, emitter + 56,
            realistic || random.Below(3) != 0 ? Bits(Uniform(random, 0.0f, 5.0f))
                                              : AnyFloat(random));
      Store(h.base, emitter + 60, block + kMxAzimuths);
      constexpr float kPi = 3.14159265f;
      static constexpr float k51[] = {7 * kPi / 4, kPi / 4, 0.0f, 0.0f, 5 * kPi / 4, 3 * kPi / 4};
      for (uint32_t i = 0; i < kMxMaxChannels; i++) {
        uint32_t azimuth;
        if (realistic) {
          azimuth = i == 3 ? lfe : Bits(i < 6 ? k51[i] : 0.0f);
        } else {
          switch (random.Below(5)) {
            case 0:
              azimuth = lfe;
              break;
            case 1:
              azimuth = AnyFloat(random);
              break;
            default:
              azimuth = Bits(Uniform(random, -1.0f, 7.0f));
              break;
          }
        }
        Store(h.base, block + kMxAzimuths + 4 * i, azimuth);
      }
      // The cone: null in half the cases.
      MakeCone(h.base, random, block + kCone, realistic);
      Store(h.base, emitter + 0, random.Below(2) == 0 ? 0 : block + kCone);
      // The five curves (null ones take the defaults), the scalers.
      for (uint32_t k = 0; k < 5; k++) {
        const uint32_t curve = block + kCxCurves + 8 * k;
        const uint32_t points = block + kCxPoints + 0x80 * k;
        const uint32_t count = realistic ? 2 + random.Below(5) : random.Below(13);
        Store(h.base, curve + 0, points);
        Store(h.base, curve + 4, count);
        Points(h.base, random, points, count, realistic);
        const bool null = random.Below(realistic ? 2 : 3) == 0;
        Store(h.base, emitter + 64 + 4 * k, null ? 0 : curve);
      }
      Store(h.base, emitter + 84,
            realistic || random.Below(4) != 0 ? Bits(Uniform(random, 1.0f, 50.0f))
                                              : AnyFloat(random));
      Store(h.base, emitter + 88,
            random.Below(5) == 0 ? 0
            : realistic || random.Below(4) != 0 ? Bits(Uniform(random, 0.0f, 2.0f))
                                                : AnyFloat(random));
      const uint32_t sources = channels == 0 ? 1 : channels;
      const uint32_t destinations =
          realistic || random.Below(2) == 0 ? (speakers == 0 ? 6 : speakers)
                                            : random.Below(kMxMaxChannels + 1);
      MakeDsp(h.base, random, block, sources, destinations, realistic);
      DspValues(h.base, random, block, realistic);
      t.r3.u64 = block + kHandle;
      t.r4.u64 = block + kListener;
      t.r5.u64 = emitter;
      t.r6.u64 = Flags(random, c, realistic, true);
      t.r7.u64 = block + kMxDsp;
      CalleeSaved(random, t);
    };
    Case(h, results, block, __imp__sub_82481880, hooks::CarSoundsX3DCalculate, setup,
         DspFloats(), frame, fmt::format("{} case {}", realistic ? "realistic" : "random", c),
         kCalculateFrame);
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
  Pans(h, block.address);
  Matrices(h, block.address);
  Cones(h, block.address);
  Dopplers(h, block.address);
  Calculates(h, block.address);
}

}  // namespace harness
