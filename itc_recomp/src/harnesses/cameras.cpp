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
#include "hooks/heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <iterator>
#include <limits>
#include <cmath>
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

void NativeMode(PPCContext& ctx, uint8_t* base) {
  const int32_t which = ctx.r3.s32;
  const uint32_t mode = cm::Mode(base, which);
  if (which == 1) {
    ctx.r3.u64 = mode;
  } else {
    ctx.r3.s64 = -1;
  }
}
void NativeSetMode(PPCContext& ctx, uint8_t* base) {
  const int32_t which = ctx.r3.s32;
  const uint32_t mode = ctx.r4.u32;
  ctx.fpscr.disableFlushMode();
  if (!cm::SetMode(base, which, mode)) {
    __imp__sub_8209F738(ctx, base);
    return;
  }
  if (which == 1 && (mode == 0 || mode == 1)) {
    ctx.r3.u64 = cm::Pass(4);
    ctx.f1.f64 = double(guest::LoadFloat(base, mode == 0 ? 0x82001E8C : 0x82001E88));
  }
}
void NativeRenderTarget(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::RenderTarget(base, ctx.r3.u32);
}
void NativeEnable(PPCContext& ctx, uint8_t* base) {
  cm::Enable(base, ctx.r3.u32);
}
void NativeDisable(PPCContext& ctx, uint8_t* base) {
  cm::Disable(base, ctx.r3.u32);
}
void NativeEnableForView(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::EnableForView(base);
}
void NativeSetNear(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::SetNear(base, ctx.f1.f64);
}
// As the adapter: Resource::Release a guest call.
void NativeReleaseSurfaces(PPCContext& ctx, uint8_t* base) {
  const uint32_t pass = ctx.r3.u32;
  bool released_580 = false;
  const bool has_surfaces =
      cm::ReleaseSurfaces(base, pass, [&](uint32_t surface, uint32_t field) {
        ctx.r3.u64 = surface;
        sub_822A67B0(ctx, base);
        released_580 = field == 580;
      });
  if (!has_surfaces) {
    ctx.r3.u64 = pass;
  } else if (!released_580) {
    ctx.r3.u64 = 0;
  }
}

void NativePassSetZoom(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::PassSetZoom(base, ctx.r3.u32, ctx.f1.f64);
}
void NativeReleaseViewports(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !cm::ReleaseViewports(base)) {
    __imp__sub_820AA430(ctx, base);
    return;
  }
  ctx.r3.u64 = 1;
}
void NativeViewportSetZoom(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::ViewportSetZoom(base, ctx.r3.u32, ctx.f1.f64);
}
void NativeViewportSetRoll(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cm::ViewportSetRoll(base, ctx.r3.u32, ctx.f1.f64);
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

// The game's camera globals the functions change (the mode, the passes'
// enabled words, pass 0's near plane, pass 4's field of view, the viewports'
// pointer), kept before a harness and put back after each run.
class Globals {
 public:
  explicit Globals(uint8_t* base) : base_(base) {
    Keep(cm::kMode, 4);
    Keep(cm::kViewports, 4);
    Keep(cm::Pass(0) + 384, 4);
    Keep(cm::Pass(4) + 368, 4);
    for (uint32_t i = 0; i < cm::kPassCount; i++) {
      Keep(cm::Pass(i) + 588, 4);
    }
  }
  ~Globals() { Restore(); }
  void Restore() const {
    for (const auto& [address, bytes] : kept_) {
      Write(base_, address, bytes.data(), uint32_t(bytes.size()));
    }
  }

 private:
  void Keep(uint32_t address, uint32_t bytes) { kept_.push_back({address, Read(base_, address, bytes)}); }
  uint8_t* base_;
  std::vector<std::pair<uint32_t, std::vector<uint8_t>>> kept_;
};

// One case through both versions: the scratch area filled with 0xCD,
// `setup()` writes the inputs (from h.random, the same for both), `args(t)`
// sets the registers. `extra(out)` adds outputs outside the scratch area
// (the game's globals), and `globals` puts those back after each run.
void Case(Context& h, Results& results, uint32_t object, std::vector<FloatRange> floats,
          const std::function<void()>& setup, const std::function<void(PPCContext&)>& args,
          GuestFunction recompiled, GuestFunction native, const std::string& what,
          const std::function<void(Outputs&)>& extra = {}, const Globals* globals = nullptr,
          uint32_t compared = kCompared) {
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
    for (uint32_t at = h.data; at < h.data + compared; at += 4) {
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
    if (extra) {
      extra(out[v]);
    }
    if (globals) {
      globals->Restore();
    }
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

// A mode word: the two the game sets, others and random bits.
uint32_t RandomMode(Random& r) {
  static constexpr uint32_t kModes[] = {0, 1, 2, 0xFFFFFFFF, 0x80000000};
  return r.Below(2) ? kModes[r.Below(std::size(kModes))] : uint32_t(r.Next());
}
int32_t RandomWhich(Random& r) {
  static constexpr int32_t kWhich[] = {1, 1, 1, 0, 2, -1};
  return r.Below(4) ? kWhich[r.Below(std::size(kWhich))] : int32_t(r.Next());
}

// The camera passes' state: the mode, the render targets, enabling, the near
// plane and the surfaces' release, on the game's globals (kept and put back).
void PassStateCases(Context& h) {
  const Globals globals(h.base);
  auto mode_word = [&](Outputs& out) { out.Word(guest::LoadU32(h.base, cm::kMode)); };
  {
    Results results("sub_8209F718 CameraPasses::Mode");
    for (int i = 0; i < 100; i++) {
      const int32_t which = RandomWhich(h.random);
      const uint32_t mode = RandomMode(h.random);
      Case(
          h, results, h.data, {}, [&] { guest::StoreU32(h.base, cm::kMode, mode); },
          [&](PPCContext& t) { t.r3.s64 = which; }, __imp__sub_8209F718, NativeMode,
          fmt::format("case {} which {} mode {:08X}", i, which, mode),
          mode_word, &globals);
    }
    results.Report();
  }
  {
    Results results("sub_8209F738 CameraPasses::SetMode");
    for (int i = 0; i < 100; i++) {
      const int32_t which = RandomWhich(h.random);
      const uint32_t mode = RandomMode(h.random);
      const uint32_t fov = FloatWord(h.random);
      Case(
          h, results, h.data, {},
          [&] {
            guest::StoreU32(h.base, cm::kMode, 0x12345678);
            guest::StoreU32(h.base, cm::Pass(4) + 368, fov);
          },
          [&](PPCContext& t) {
            t.r3.s64 = which;
            t.r4.u64 = mode;
          },
          __imp__sub_8209F738, NativeSetMode,
          fmt::format("case {} which {} mode {:08X}", i, which, mode),
          [&](Outputs& out) {
            mode_word(out);
            out.Float(guest::LoadU32(h.base, cm::Pass(4) + 368));
          },
          &globals);
    }
    results.Report();
  }
  {
    Results results("sub_8209F7A8 CameraPasses::RenderTarget");
    for (int i = 0; i < 200; i++) {
      const uint32_t id = h.random.Below(3) ? h.random.Below(15) : uint32_t(h.random.Next());
      const uint32_t mode = RandomMode(h.random);
      Case(
          h, results, h.data, {}, [&] { guest::StoreU32(h.base, cm::kMode, mode); },
          [&](PPCContext& t) { t.r3.u64 = id; }, __imp__sub_8209F7A8, NativeRenderTarget,
          fmt::format("case {} id {} mode {:08X}", i, int32_t(id), mode), mode_word, &globals);
    }
    results.Report();
  }
  {
    Results enable("sub_820A0968 CameraPass::Enable");
    Results disable("sub_820A0958 CameraPass::Disable");
    for (int i = 0; i < 50; i++) {
      const uint32_t pass = h.data + 4 * h.random.Below(64);
      Case(
          h, enable, pass, {}, [&] { RandomWords(h, h.data, 256); },
          [&](PPCContext& t) { t.r3.u64 = pass; }, __imp__sub_820A0968, NativeEnable,
          fmt::format("case {}", i));
      Case(
          h, disable, pass, {}, [&] { RandomWords(h, h.data, 256); },
          [&](PPCContext& t) { t.r3.u64 = pass; }, __imp__sub_820A0958, NativeDisable,
          fmt::format("case {}", i));
    }
    enable.Report();
    disable.Report();
  }
  {
    // A viewports block and viewport 0 in the scratch area, the passes'
    // enabled words random.
    Results results("sub_820AA1A8 CameraPasses::EnableForView");
    const uint32_t block = h.data, viewport = h.data + 64;
    for (int i = 0; i < 100; i++) {
      const uint32_t split = h.random.Below(3) ? 0 : (h.random.Below(2) ? 1 : uint32_t(h.random.Next()));
      const uint32_t view_mode = h.random.Below(4) ? h.random.Below(4) : uint32_t(h.random.Next());
      const uint32_t mode = RandomMode(h.random);
      uint32_t enabled[cm::kPassCount];
      for (uint32_t& word : enabled) {
        word = h.random.Below(2) ? h.random.Below(2) : uint32_t(h.random.Next());
      }
      Case(
          h, results, h.data, {},
          [&] {
            guest::StoreU32(h.base, block + 0, viewport);
            guest::StoreU32(h.base, block + 8, split);
            guest::StoreU32(h.base, viewport + 1860, view_mode);
            guest::StoreU32(h.base, cm::kViewports, block);
            guest::StoreU32(h.base, cm::kMode, mode);
            for (uint32_t p = 0; p < cm::kPassCount; p++) {
              guest::StoreU32(h.base, cm::Pass(p) + 588, enabled[p]);
            }
          },
          [](PPCContext&) {}, __imp__sub_820AA1A8, NativeEnableForView,
          fmt::format("case {} split {} view mode {} mode {:08X}", i, split, view_mode, mode),
          [&](Outputs& out) {
            for (uint32_t p = 0; p < cm::kPassCount; p++) {
              out.Word(guest::LoadU32(h.base, cm::Pass(p) + 588));
            }
          },
          &globals);
    }
    results.Report();
  }
  {
    Results results("sub_820AA368 CameraPasses::SetNear");
    for (int i = 0; i < 100; i++) {
      const double near = RandomF1(h.random);
      Case(
          h, results, h.data, {}, [] {}, [&](PPCContext& t) { t.f1.f64 = near; },
          __imp__sub_820AA368, NativeSetNear, fmt::format("case {} near {}", i, near),
          [&](Outputs& out) { out.Float(guest::LoadU32(h.base, cm::Pass(0) + 384)); }, &globals);
    }
    results.Report();
  }
  {
    // Fake resources in the scratch area with counts of 2 or more, so
    // Release only counts down (a count reaching 0 would destroy them).
    Results results("sub_820A0C40 CameraPass::ReleaseSurfaces");
    const uint32_t pass = h.data + 256, objects = h.data + 16;
    for (int i = 0; i < 100; i++) {
      const int32_t type = int32_t(h.random.Below(24)) - 1;
      const uint32_t first = h.random.Below(3) ? objects : 0;
      const uint32_t second = h.random.Below(3) ? objects + 32 : 0;
      const uint32_t count = 2 + h.random.Below(5);
      Case(
          h, results, pass, {},
          [&] {
            for (uint32_t o : {objects, objects + 32}) {
              guest::StoreU32(h.base, o + 0, 0);
              guest::StoreU32(h.base, o + 4, count);
            }
            guest::StoreU32(h.base, pass + 576, first);
            guest::StoreU32(h.base, pass + 580, second);
            guest::StoreU32(h.base, pass + 584, uint32_t(type));
          },
          [&](PPCContext& t) { t.r3.u64 = pass; }, __imp__sub_820A0C40, NativeReleaseSurfaces,
          fmt::format("case {} type {} surfaces {:08X} {:08X}", i, type, first, second));
    }
    results.Report();
  }
}

// A field in each of a viewport's 20 entries, as float ranges.
std::vector<FloatRange> EntryFields(uint32_t offset) {
  std::vector<FloatRange> ranges;
  for (uint32_t entry = 0; entry < 20; entry++) {
    ranges.push_back({80 * entry + offset, 80 * entry + offset + 4});
  }
  return ranges;
}

// CameraPass::SetZoom and the viewports' zoom, roll and release.
void ViewportCases(Context& h) {
  {
    Results results("sub_820AA378 CameraPass::SetZoom");
    for (int i = 0; i < 300; i++) {
      const uint32_t pass = h.data + 64 + 4 * h.random.Below(4);
      static constexpr int32_t kHeights[] = {720, 480, 1024, 512, 256, 0, -1, 1};
      const uint32_t height =
          h.random.Below(2) ? uint32_t(kHeights[h.random.Below(std::size(kHeights))])
                            : uint32_t(h.random.Next());
      const double zoom = h.random.Below(3) ? double(std::bit_cast<float>(FloatWord(h.random)))
                                            : RandomF1(h.random);
      Case(
          h, results, pass, {{368, 372}, {516, 520}},
          [&] {
            RandomWords(h, pass, 148);
            guest::StoreU32(h.base, pass + 508, height);
          },
          [&](PPCContext& t) {
            t.r3.u64 = pass;
            t.f1.f64 = zoom;
          },
          __imp__sub_820AA378, NativePassSetZoom,
          fmt::format("case {} height {} zoom {}", i, int32_t(height), zoom));
    }
    results.Report();
  }
  {
    Results results("sub_820AAE90 Viewport::SetZoom");
    for (int i = 0; i < 200; i++) {
      const uint32_t viewport = h.data + 64 + 4 * h.random.Below(4);
      const double zoom = RandomF1(h.random);
      const uint32_t distance = h.random.Below(2) ? FloatBits(8000.0f) : FloatWord(h.random);
      Case(
          h, results, viewport, EntryFields(68),
          [&] {
            RandomWords(h, viewport, 468);
            guest::StoreU32(h.base, viewport + 1856, distance);
          },
          [&](PPCContext& t) {
            t.r3.u64 = viewport;
            t.f1.f64 = zoom;
          },
          __imp__sub_820AAE90, NativeViewportSetZoom,
          fmt::format("case {} zoom {} distance {:08X}", i, zoom, distance), {}, nullptr, 2048);
    }
    results.Report();
  }
  {
    // Angles from which the original's loops end: numbers within 10,000
    // and NaNs (an infinite or huge one never leaves the first loop).
    Results results("sub_820AA2D0 Viewport::SetRoll");
    static constexpr double kAngles[] = {0.0, -0.0, 3.14159265, -3.14159265, 6.2831853, -6.2831853,
                                         10000.0, -10000.0, 1e-40, -1e-45, 1.5, 
                                         std::numeric_limits<double>::quiet_NaN()};
    for (int i = 0; i < 300; i++) {
      const uint32_t viewport = h.data + 64 + 4 * h.random.Below(4);
      double angle;
      if (h.random.Below(2)) {
        angle = kAngles[h.random.Below(std::size(kAngles))];
      } else {
        const uint32_t word = FloatWord(h.random);
        const float f = std::bit_cast<float>(word);
        angle = (std::isnan(f) || std::fabs(f) <= 10000.0f) ? double(f) : double(f) * 0.0;
      }
      Case(
          h, results, viewport, EntryFields(64), [&] { RandomWords(h, viewport, 468); },
          [&](PPCContext& t) {
            t.r3.u64 = viewport;
            t.f1.f64 = angle;
          },
          __imp__sub_820AA2D0, NativeViewportSetRoll, fmt::format("case {} angle {}", i, angle), {},
          nullptr, 2048);
    }
    results.Report();
  }
  if (hooks::NativeHeapActive()) {
    // A viewports block and its two viewports from the allocation layer, the
    // global pointed at them for the case (and put back).
    const Globals globals(h.base);
    Results results("sub_820AA430 Viewports::Release");
    for (int i = 0; i < 20; i++) {
      uint32_t blocks[3] = {};
      Case(
          h, results, h.data, {},
          [&] {
            blocks[0] = memory::AllocPages(12, 4096);
            blocks[1] = memory::AllocPages(1872, 4096);
            blocks[2] = memory::AllocPages(1872, 4096);
            guest::StoreU32(h.base, blocks[0] + 0, blocks[1]);
            guest::StoreU32(h.base, blocks[0] + 4, blocks[2]);
            guest::StoreU32(h.base, blocks[0] + 8, h.random.Below(2));
            guest::StoreU32(h.base, cm::kViewports, blocks[0]);
          },
          [](PPCContext&) {}, __imp__sub_820AA430, NativeReleaseViewports,
          fmt::format("case {}", i),
          [&](Outputs& out) {
            out.Word(guest::LoadU32(h.base, cm::kViewports));
            for (uint32_t block : blocks) {
              out.Word(memory::IsPages(block) ? 1 : 0);
            }
          },
          &globals);
    }
    results.Report();
  }
}

}  // namespace

void Cameras(Context& h) {
  SetFieldOfViewCases(h);
  BuildProjectionCases(h);
  SetViewportSizeCases(h);
  PassStateCases(h);
  ViewportCases(h);
}

}  // namespace harness
