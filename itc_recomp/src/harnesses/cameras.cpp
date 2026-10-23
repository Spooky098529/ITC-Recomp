// The camera passes and viewports (src/cameras/, docs/functions/cameras.md)
// against the recompiled functions: a camera (or pass, or viewport) in the
// scratch area, filled anew for each case, at an aligned address or 4 or 8
// bytes on (the vector unit's aligned stores), its inputs random float words
// (NaNs, infinities, denormals and signed zeros among them), ordinary values
// and the game's own. The scratch area's first 1,024 bytes and r3 are
// compared: the words the functions store as floats with harness::Outputs'
// Float (two NaNs match), the rest exactly. The originals run with
// --native_cameras off.

#include "harness.h"

#include "base/flags.h"
#include "cameras/cameras.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_cameras);

namespace harness {

namespace {

namespace cm = cameras;

constexpr uint32_t kCompared = 1024;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
void NativeSetFieldOfView(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::SetFieldOfView(base, ctx.r3.u32, ctx.f1.f64);
}
void NativeBuildProjection(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::BuildProjection(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}
void NativeSetViewportSize(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::SetViewportSize(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// Runs the recompiled version with the flag off, so the functions it calls
// that are ours run the originals too.
void RunRecompiled(GuestFunction recompiled, PPCContext& t, uint8_t* base) {
  const bool cameras = ITC_FLAG(native_cameras);
  ITC_FLAG(native_cameras) = false;
  recompiled(t, base);
  ITC_FLAG(native_cameras) = cameras;
}

// NaNs of both kinds and signs, infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001,
                                   0x7F800000, 0xFF800000, 0x00000001, 0x807FFFFF,
                                   0x00000000, 0x80000000, 0x7F7FFFFF, 0x3F800000};

// Float words: odd floats, random bits, and ordinary values (-64 to 64).
uint32_t FloatWord(Random& r) {
  switch (r.Below(5)) {
    case 0:
      return kOddFloats[r.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(r.Next());
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 8192.0f);
  }
}
uint32_t FloatBits(float value) {
  return std::bit_cast<uint32_t>(value);
}

void RandomWords(Context& h, uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(h.base, address + 4 * i, FloatWord(h.random));
  }
}

// Which words of the compared area are floats the functions store: offsets
// from the object, [from, to).
struct FloatRange {
  uint32_t from, to;
};

// One case through both versions: the scratch area filled with 0xCD,
// `setup()` writes the inputs (from h.random, the same for both), `args(t)`
// sets the registers.
void Case(Context& h, Results& results, uint32_t object, std::vector<FloatRange> floats,
          const std::function<void()>& setup, const std::function<void(PPCContext&)>& args,
          GuestFunction recompiled, GuestFunction native, const std::string& what) {
  Outputs out[2];
  // The same random inputs for both.
  const Random inputs = h.random;
  for (int v = 0; v < 2; v++) {
    h.random = inputs;
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    if (v == 0) {
      RunRecompiled(recompiled, t, h.base);
    } else {
      native(t, h.base);
    }
    for (uint32_t at = h.data; at < h.data + kCompared; at += 4) {
      bool is_float = false;
      for (const FloatRange& range : floats) {
        is_float |= at >= object + range.from && at < object + range.to;
      }
      const uint32_t word = guest::LoadU32(h.base, at);
      if (is_float) {
        out[v].Float(word);
      } else {
        out[v].Word(word);
      }
    }
    out[v].Word(t.r3.u32);
  }
  results.Check(out[0], out[1], what);
}

// Doubles for f1: the game's (single floats), odd values and random bits.
double RandomF1(Random& r) {
  static constexpr double kGame[] = {80.0, 90.0, 45.0, 0.0, -0.0, 180.0, 360.0, -540.0, 1e10,
                                     1e30, 1e300, -1e300};
  switch (r.Below(4)) {
    case 0:
      return kGame[r.Below(std::size(kGame))];
    case 1:
      return std::bit_cast<double>(r.Next());
    default:
      return double(std::bit_cast<float>(FloatWord(r)));
  }
}

void SetFieldOfViewCases(Context& h) {
  Results results("sub_82593278 Camera::SetFieldOfView");
  for (int i = 0; i < 400; i++) {
    const uint32_t camera = h.data + 64 + 4 * h.random.Below(4);
    const double degrees = RandomF1(h.random);
    Case(
        h, results, camera, {{368, 372}}, [&] { RandomWords(h, camera, 100); },
        [&](PPCContext& t) {
          t.r3.u64 = camera;
          t.f1.f64 = degrees;
        },
        __imp__sub_82593278, NativeSetFieldOfView, fmt::format("case {} degrees {}", i, degrees));
  }
  results.Report();
}

// A camera's inputs: random words, ordinary values or the game's.
void CameraInputs(Context& h, uint32_t fov, uint32_t aspect, uint32_t far, uint32_t near,
                  uint32_t kind) {
  if (kind == 0) {
    // The game's: pass 0's (near 1.0, far 5,000) and others.
    static constexpr float kFovs[] = {0.785398f, 1.0908f, 1.5708f, 0.4f, 2.5f};
    static constexpr float kAspects[] = {1.7777778f, 1.3333334f, 1.0f, 4.0f};
    guest::StoreU32(h.base, fov, FloatBits(kFovs[h.random.Below(std::size(kFovs))]));
    guest::StoreU32(h.base, aspect, FloatBits(kAspects[h.random.Below(std::size(kAspects))]));
    guest::StoreU32(h.base, near, FloatBits(h.random.Below(2) ? 1.0f : 0.1f));
    guest::StoreU32(h.base, far, FloatBits(h.random.Below(2) ? 5000.0f : 3000.0f));
    return;
  }
  for (uint32_t address : {fov, aspect, far, near}) {
    guest::StoreU32(h.base, address, FloatWord(h.random));
  }
}

void BuildProjectionCases(Context& h) {
  Results results("sub_825932E8 Camera::BuildProjection");
  for (int i = 0; i < 600; i++) {
    const uint32_t camera = h.data + 64 + 4 * h.random.Below(4);
    const uint32_t kind = h.random.Below(3);
    // The inputs at the camera's own fields, elsewhere, or inside the
    // projection (stored before they're read).
    const uint32_t layout = h.random.Below(4);
    uint32_t fov = camera + 368, aspect = camera + 392, far = camera + 388, near = camera + 384;
    if (layout == 1) {
      fov = h.data + 800;
      aspect = h.data + 812;
      far = h.data + 804;
      near = h.data + 808;
    } else if (layout == 2) {
      fov = camera + 80 + 4 * h.random.Below(16);
      aspect = camera + 80 + 4 * h.random.Below(16);
    }
    Case(
        h, results, camera, {{64, 160}},
        [&] {
          RandomWords(h, camera, 100);
          CameraInputs(h, fov, aspect, far, near, kind);
        },
        [&](PPCContext& t) {
          t.r3.u64 = camera;
          t.r4.u64 = fov;
          t.r5.u64 = aspect;
          t.r6.u64 = far;
          t.r7.u64 = near;
        },
        __imp__sub_825932E8, NativeBuildProjection,
        fmt::format("case {} kind {} layout {}", i, kind, layout));
  }
  results.Report();
}

uint32_t RandomSize(Random& r) {
  static constexpr uint32_t kSizes[] = {0,       1,          640,        480,        1280,
                                        720,     1920,       1080,       512,        128,
                                        1u << 24, (1u << 24) + 1, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF};
  return r.Below(2) ? kSizes[r.Below(std::size(kSizes))] : uint32_t(r.Next());
}

void SetViewportSizeCases(Context& h) {
  Results results("sub_82593500 Camera::SetViewportSize");
  for (int i = 0; i < 400; i++) {
    const uint32_t camera = h.data + 64 + 4 * h.random.Below(4);
    const uint32_t kind = h.random.Below(3);
    const uint32_t width = RandomSize(h.random), height = RandomSize(h.random);
    const uint64_t high = h.random.Below(2) ? (h.random.Next() << 32) : 0;
    Case(
        h, results, camera, {{64, 160}, {368, 396}},
        [&] {
          RandomWords(h, camera, 100);
          CameraInputs(h, camera + 368, camera + 392, camera + 388, camera + 384, kind);
        },
        [&](PPCContext& t) {
          t.r3.u64 = camera;
          t.r4.u64 = high | width;
          t.r5.u64 = high | height;
        },
        __imp__sub_82593500, NativeSetViewportSize,
        fmt::format("case {} {} x {} kind {}", i, width, height, kind));
  }
  results.Report();
}

}  // namespace

void Cameras(Context& h) {
  SetFieldOfViewCases(h);
  BuildProjectionCases(h);
  SetViewportSizeCases(h);
}

}  // namespace harness
