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
#include "exit_table.h"

#include "base/flags.h"
#include "base/log.h"
#include "c_runtime/exit.h"
#include "cameras/cameras.h"
#include "hooks/c_runtime.h"
#include "command_list/command_list.h"
#include "hooks/cameras.h"
#include "hooks/heap.h"
#include "graphics/resources.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <bit>
#include <functional>
#include <iterator>
#include <limits>
#include <mutex>
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

void NativeCameraUpdate(PPCContext& ctx, uint8_t* base) {
  if (!hooks::CameraUpdate(ctx, base)) {
    __imp__sub_82593568(ctx, base);
  }
}
void NativeCameraConstruct(PPCContext& ctx, uint8_t* base) {
  if (!hooks::CameraConstruct(ctx, base)) {
    __imp__sub_82593908(ctx, base);
  }
}

void NativeViewportUpdateEntries(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::ViewportUpdateEntries(base, ctx.r3.u32);
}
void NativeViewportUpdate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::ViewportUpdate(base, ctx.r3.u32);
}
void NativeViewportSet(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::ViewportSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
}
void NativeViewportLookAt(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::ViewportLookAt(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// As the adapters: the guest calls made by hooks/cameras.cpp, the work the
// plain C++'s.
void NativePassUpdate(PPCContext& ctx, uint8_t* base) {
  if (!hooks::CameraPassUpdate(ctx, base)) {
    __imp__sub_820A3978(ctx, base);
  }
}

// As the adapter: the fields natively, then the Update wrapper's override.
void NativeFromViewport(PPCContext& ctx, uint8_t* base) {
  const uint32_t pass = ctx.r3.u32;
  ctx.fpscr.disableFlushMode();
  cm::FromViewport(base, pass, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = pass;
  sub_820A3978(ctx, base);
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

// Camera::Update on a camera in the scratch area: its angles, position,
// size, field of view and planes random or a camera's (small angles, the
// game's sizes). The view and its copy (+16 to +79, +208 to +271), the
// projection, the view-projection and +368 to +395 are compared as floats.
void CameraUpdateCases(Context& h) {
  Results results("sub_82593568 Camera::Update");
  for (int i = 0; i < 600; i++) {
    const uint32_t camera = h.data + 64 + 4 * h.random.Below(4);
    const uint32_t kind = h.random.Below(3);
    const bool game = h.random.Below(2);
    Case(
        h, results, camera, {{16, 272}, {368, 396}},
        [&] {
          RandomWords(h, camera, 100);
          CameraInputs(h, camera + 368, camera + 392, camera + 388, camera + 384, kind);
          if (game) {
            for (uint32_t k = 0; k < 3; k++) {
              guest::StoreU32(h.base, camera + 288 + 4 * k,
                              FloatBits(float(int32_t(h.random.Below(4000)) - 2000) / 300.0f));
              guest::StoreU32(h.base, camera + 272 + 4 * k,
                              FloatBits(float(int32_t(h.random.Below(1u << 20)) - (1 << 19)) / 64.0f));
            }
            guest::StoreU32(h.base, camera + 284, FloatBits(1.0f));
            guest::StoreU32(h.base, camera + 376, FloatBits(h.random.Below(2) ? 1280.0f : 640.0f));
            guest::StoreU32(h.base, camera + 380, FloatBits(h.random.Below(2) ? 720.0f : 480.0f));
          }
        },
        [&](PPCContext& t) { t.r3.u64 = camera; }, __imp__sub_82593568, NativeCameraUpdate,
        fmt::format("case {} kind {} game {}", i, kind, game));
  }
  results.Report();
}

// Camera::Camera on a camera in the scratch area full of random words.
void CameraConstructCases(Context& h) {
  Results results("sub_82593908 Camera::Camera");
  for (int i = 0; i < 40; i++) {
    const uint32_t camera = h.data + 64 + 4 * h.random.Below(4);
    Case(
        h, results, camera, {{16, 272}, {272, 304}, {368, 396}},
        [&] { RandomWords(h, camera, 100); },
        [&](PPCContext& t) { t.r3.u64 = camera; }, __imp__sub_82593908, NativeCameraConstruct,
        fmt::format("case {}", i));
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

// A made-up scene for the viewport's recording, in the scratch area: the
// scene's vtable slot 8 and the manager's slots 1 and 2 are getters of the
// game's (sub_8248CA78 returns [r3 + 284], sub_82379320 [r3 + 124]), and the
// queue's slot 4 is BillboardMaterial::SetLights (sub_8209F5C8: +32 = r4,
// +36 = r5 unless r4 is 0), which keeps the chunk and the key it's handed.
// The list as the sprites' harness makes it.
struct FakeScene {
  static constexpr uint32_t kScene = 0, kSceneVtable = 320, kManager = 400, kManagerVtable = 720,
                            kQueue = 760, kQueueVtable = 800, kList = 840, kPass = 1024,
                            kSlots = 2048;
  // kind: 0 room, 1 a chunk already open on the list, 2 another open (nested),
  // 3 just room. (A full list isn't made: the original then writes its
  // header at guest address 0.)
  static void Set(Context& h, uint32_t kind, uint32_t used) {
    const uint32_t d = h.data;
    guest::StoreU32(h.base, d + kScene, d + kSceneVtable);
    guest::StoreU32(h.base, d + kSceneVtable + 32, 0x8248CA78);
    guest::StoreU32(h.base, d + kScene + 284, d + kManager);
    guest::StoreU32(h.base, d + kManager, d + kManagerVtable);
    guest::StoreU32(h.base, d + kManagerVtable + 4, 0x8248CA78);
    guest::StoreU32(h.base, d + kManagerVtable + 8, 0x82379320);
    guest::StoreU32(h.base, d + kManager + 284, d + kList);
    guest::StoreU32(h.base, d + kManager + 124, d + kQueue);
    guest::StoreU32(h.base, d + kQueue, d + kQueueVtable);
    guest::StoreU32(h.base, d + kQueueVtable + 16, 0x8209F5C8);
    const uint32_t list = d + kList;
    guest::StoreU32(h.base, list + 8, kind == 3 ? used + 5 : used + 1000);
    guest::StoreU32(h.base, list + 12, used);
    guest::StoreU32(h.base, list + 32, d + kSlots);
    guest::StoreU8(h.base, list + 52, kind == 1 ? 1 : 0);
    if (kind == 1) {
      guest::StoreU32(h.base, list + 44, d + kSlots + 16);
    }
    guest::StoreU32(h.base, command_list::kOpenChunks, kind == 2 ? 1 : 0);
    guest::StoreU32(h.base, 0x826BABA0, d + kScene);
  }
};

// The scene global and the open chunks' count, kept and put back.
class SceneGlobals {
 public:
  explicit SceneGlobals(uint8_t* base)
      : base_(base),
        scene_(guest::LoadU32(base, 0x826BABA0)),
        open_(guest::LoadU32(base, command_list::kOpenChunks)) {}
  ~SceneGlobals() { Restore(); }
  void Restore() const {
    guest::StoreU32(base_, 0x826BABA0, scene_);
    guest::StoreU32(base_, command_list::kOpenChunks, open_);
  }

 private:
  uint8_t* base_;
  uint32_t scene_, open_;
};

uint32_t RandomKind(Random& r) {
  return r.Below(4) ? 0 : r.Below(4);
}

// Viewport::UpdateEntries, Update, Set and LookAt on a viewport in the
// scratch area (1,872 bytes, aligned or 4, 8 or 12 bytes on) full of random
// words: its eye, target, up and fourth vectors random or a camera's, the
// view mode (+1860) and the passes' mode (CameraPasses::Mode) the game's or
// random. Set's and LookAt's vectors after the viewport, or inside it (its
// own vectors, copied over each other). Everything but the view mode is
// compared as floats.
void ViewportEntriesCases(Context& h) {
  const Globals globals(h.base);
  struct Kind {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Kind kinds[] = {
      {"sub_820AA550 Viewport::UpdateEntries", __imp__sub_820AA550, NativeViewportUpdateEntries},
      {"sub_820AAE40 Viewport::Update", __imp__sub_820AAE40, NativeViewportUpdate},
      {"sub_820AAE48 Viewport::Set", __imp__sub_820AAE48, NativeViewportSet},
      {"sub_820AAF68 Viewport::LookAt", __imp__sub_820AAF68, NativeViewportLookAt},
  };
  for (const Kind& kind : kinds) {
    Results results(kind.name);
    for (int i = 0; i < 300; i++) {
      const uint32_t viewport = h.data + 64 + 4 * h.random.Below(4);
      const bool game = h.random.Below(2);
      const uint32_t view_mode = h.random.Below(4) ? h.random.Below(3) : uint32_t(h.random.Next());
      const uint32_t mode = RandomMode(h.random);
      const bool inside = h.random.Below(4) == 0;
      uint32_t vectors[4];
      for (uint32_t k = 0; k < 4; k++) {
        vectors[k] = inside ? viewport + 1600 + 16 * h.random.Below(4) : h.data + 2048 + 16 * k;
      }
      Case(
          h, results, viewport, {{0, 1860}, {1864, 1872}},
          [&] {
            RandomWords(h, viewport, 468);
            RandomWords(h, h.data + 2048, 16);
            if (game) {
              for (uint32_t k = 0; k < 3; k++) {
                const float e = float(int32_t(h.random.Below(1u << 20)) - (1 << 19)) / 64.0f;
                const float t = e + float(int32_t(h.random.Below(1u << 12)) - (1 << 11)) / 64.0f;
                for (uint32_t at : {viewport + 1600, h.data + 2048}) {
                  guest::StoreU32(h.base, at + 4 * k, FloatBits(e));
                  guest::StoreU32(h.base, at + 16 + 4 * k, FloatBits(t));
                  guest::StoreU32(h.base, at + 32 + 4 * k, FloatBits(k == 1 ? 1.0f : 0.0f));
                  guest::StoreU32(h.base, at + 48 + 4 * k, FloatBits(t));
                }
                guest::StoreU32(h.base, viewport + 1664 + 4 * k, FloatBits(t));
              }
              guest::StoreU32(h.base, viewport + 1856, FloatBits(8000.0f));
            }
            guest::StoreU32(h.base, viewport + 1860, view_mode);
            guest::StoreU32(h.base, cm::kMode, mode);
          },
          [&](PPCContext& t) {
            t.r3.u64 = viewport;
            t.r4.u64 = vectors[0];
            t.r5.u64 = vectors[1];
            t.r6.u64 = vectors[2];
            t.r7.u64 = vectors[3];
          },
          kind.recompiled, kind.native,
          fmt::format("case {} game {} view mode {} mode {} inside {}", i, game, int32_t(view_mode),
                      int32_t(mode), inside),
          {}, &globals, kScratchBytes);
    }
    results.Report();
  }
}

void RecordViewportCases(Context& h) {
  const SceneGlobals scene(h.base);
  Results results("sub_820A0978 CameraPass::RecordViewport");
  const uint32_t pass = h.data + FakeScene::kPass;
  for (int i = 0; i < 200; i++) {
    const uint32_t kind = RandomKind(h.random);
    const uint32_t used = h.random.Below(2) ? 0 : h.random.Below(100000);
    const uint32_t key = h.random.Below(2) ? (h.random.Below(2) ? 150 : 371) : uint32_t(h.random.Next());
    Case(
        h, results, pass, {},
        [&] {
          FakeScene::Set(h, kind, used);
          RandomWords(h, pass + 496, 4);
          if (h.random.Below(2)) {
            guest::StoreU32(h.base, pass + 496, 0);
            guest::StoreU32(h.base, pass + 500, 0);
            guest::StoreU32(h.base, pass + 504, 1280);
            guest::StoreU32(h.base, pass + 508, 720);
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = pass;
          t.r4.u64 = key;
        },
        __imp__sub_820A0978, hooks::CameraPassRecordViewport,
        fmt::format("case {} kind {} used {} key {}", i, kind, used, key),
        [&](Outputs& out) {
          out.Word(guest::LoadU32(h.base, command_list::kOpenChunks));
          scene.Restore();
        },
        nullptr, kScratchBytes);
  }
  results.Report();
}

void NativeRecordBegin(PPCContext& ctx, uint8_t* base) {
  if (!hooks::CameraPassRecordBegin(ctx, base)) {
    __imp__sub_820A0CC0(ctx, base);
  }
}
void NativeRecordEnd(PPCContext& ctx, uint8_t* base) {
  if (!hooks::CameraPassRecordEnd(ctx, base)) {
    __imp__sub_820A1C30(ctx, base);
  }
}

// The list's kinds for the record passes: FakeScene's 0 to 2, and 3 room
// for 33 slots exactly (FakeScene's own 3 has room for 4, and the original
// then writes a header at guest address 0).
void SetRecordScene(Context& h, uint32_t kind, uint32_t used) {
  FakeScene::Set(h, kind == 3 ? 0 : kind, used);
  if (kind == 3) {
    guest::StoreU32(h.base, h.data + FakeScene::kList + 8, used + 33);
  }
}

// CameraPass::RecordBegin and RecordEnd on a pass in the scratch area: each
// type (and others), its surfaces, clear color and depth random words, pass
// 10 enabled or not (type 20's clear), the list as FakeScene makes it. The
// chunks are compared, and the last chunk and key handed to the made-up
// render queue (its +32, +36), and the open chunks' count.
void RecordPassCases(Context& h) {
  const SceneGlobals scene(h.base);
  const Globals globals(h.base);
  struct Kind {
    const char* name;
    GuestFunction recompiled, native;
  };
  const Kind kinds[] = {
      {"sub_820A0CC0 CameraPass::RecordBegin", __imp__sub_820A0CC0, NativeRecordBegin},
      {"sub_820A1C30 CameraPass::RecordEnd", __imp__sub_820A1C30, NativeRecordEnd},
  };
  const uint32_t pass = h.data + FakeScene::kPass;
  for (const Kind& k : kinds) {
    Results results(k.name);
    for (int i = 0; i < 400; i++) {
      const uint32_t type = h.random.Below(8) ? h.random.Below(23) : uint32_t(h.random.Next());
      const uint32_t kind = h.random.Below(4) ? 0 : h.random.Below(4);
      const uint32_t used = h.random.Below(2) ? 0 : h.random.Below(100000);
      const bool pass10 = h.random.Below(2);
      Case(
          h, results, pass, {},
          [&] {
            SetRecordScene(h, kind, used);
            RandomWords(h, pass + 560, 8);
            guest::StoreU32(h.base, pass + 584, type);
            guest::StoreU32(h.base, cm::Pass(10) + 588, pass10 ? 1 : 0);
          },
          [&](PPCContext& t) { t.r3.u64 = pass; }, k.recompiled, k.native,
          fmt::format("case {} type {} kind {} used {} pass 10 {}", i, int32_t(type), kind, used,
                      pass10),
          [&](Outputs& out) {
            out.Word(guest::LoadU32(h.base, command_list::kOpenChunks));
            scene.Restore();
          },
          &globals, kScratchBytes);
    }
    results.Report();
  }
}

// CameraPasses::RecordBegins and RecordEnds over the game's 21 passes with
// up to three of them enabled (any), the list as above.
void RecordPassesCases(Context& h) {
  const SceneGlobals scene(h.base);
  const Globals globals(h.base);
  struct Kind {
    const char* name;
    GuestFunction recompiled;
    void (*native)(PPCContext&, uint8_t*);
  };
  const Kind kinds[] = {
      {"sub_8209F8A0 CameraPasses::RecordBegins", __imp__sub_8209F8A0,
       hooks::CameraPassesRecordBegins},
      {"sub_8209F900 CameraPasses::RecordEnds", __imp__sub_8209F900, hooks::CameraPassesRecordEnds},
  };
  for (const Kind& k : kinds) {
    Results results(k.name);
    for (int i = 0; i < 100; i++) {
      const uint32_t kind = h.random.Below(4) ? 0 : h.random.Below(3);
      uint32_t enabled[3];
      const uint32_t count = h.random.Below(4);
      for (uint32_t j = 0; j < 3; j++) {
        enabled[j] = h.random.Below(21);
      }
      Case(
          h, results, h.data, {},
          [&] {
            SetRecordScene(h, kind, 0);
            for (uint32_t p = 0; p < cm::kPassCount; p++) {
              guest::StoreU32(h.base, cm::Pass(p) + 588, 0);
            }
            for (uint32_t j = 0; j < count; j++) {
              guest::StoreU32(h.base, cm::Pass(enabled[j]) + 588, 1);
            }
          },
          [](PPCContext&) {}, k.recompiled, k.native,
          fmt::format("case {} kind {} passes {} {} {} ({})", i, kind, enabled[0], enabled[1],
                      enabled[2], count),
          [&](Outputs& out) {
            out.Word(guest::LoadU32(h.base, command_list::kOpenChunks));
            scene.Restore();
          },
          &globals, kScratchBytes);
    }
    results.Report();
  }
}

void RecordClearCases(Context& h) {
  const SceneGlobals scene(h.base);
  Results results("sub_8209FA38 RecordClear");
  for (int i = 0; i < 200; i++) {
    const uint32_t kind = RandomKind(h.random);
    const uint32_t used = h.random.Below(2) ? 0 : h.random.Below(100000);
    const uint64_t key = h.random.Below(2) ? 49 : h.random.Next();
    const uint64_t flags = h.random.Below(2) ? 49 : h.random.Next();
    const uint64_t color = h.random.Below(2) ? 0xFF000000 : h.random.Next();
    const uint64_t depth = h.random.Below(2) ? h.random.Below(2) : h.random.Next();
    Case(
        h, results, h.data, {},
        [&] {
          FakeScene::Set(h, kind, used);
          // Room for its 21 slots in "just room".
          if (kind == 3) {
            guest::StoreU32(h.base, h.data + FakeScene::kList + 8, used + 22);
          }
        },
        [&](PPCContext& t) {
          t.r4.u64 = key;
          t.r5.u64 = flags;
          t.r6.u64 = color;
          t.r7.u64 = depth;
        },
        __imp__sub_8209FA38, hooks::RecordClear,
        fmt::format("case {} kind {} used {} depth {:X}", i, kind, used, depth),
        [&](Outputs& out) {
          out.Word(guest::LoadU32(h.base, command_list::kOpenChunks));
          scene.Restore();
        },
        nullptr, kScratchBytes);
  }
  results.Report();
}

// CameraPass::Update on a pass in the scratch area with the library camera's
// vtable (slot 0 Camera::SetViewportSize), each type, the made-up scene for
// types 0 to 2's viewport. The view (+16 to +79), the projection, the
// view-projection, the floats Camera::SetViewportSize stores and the vectors
// at +464 and +480 are compared as floats.
void PassUpdateCases(Context& h) {
  const SceneGlobals scene(h.base);
  Results results("sub_820A3978 CameraPass::Update");
  const uint32_t pass = h.data + FakeScene::kPass;
  for (int i = 0; i < 600; i++) {
    const uint32_t type = h.random.Below(8) ? h.random.Below(22) : uint32_t(h.random.Next());
    const uint32_t kind = RandomKind(h.random);
    const bool game = h.random.Below(2);
    Case(
        h, results, pass, {{16, 208}, {368, 396}, {464, 496}},
        [&] {
          FakeScene::Set(h, kind, h.random.Below(1000));
          RandomWords(h, pass, 148);
          guest::StoreU32(h.base, pass, 0x82001EC0);
          guest::StoreU32(h.base, pass + 584, type);
          if (game) {
            // A camera's: eye and target apart, up (0, 1, 0), ordinary
            // fields of view, sizes and planes.
            for (uint32_t k = 0; k < 3; k++) {
              const float e = float(int32_t(h.random.Below(1u << 20)) - (1 << 19)) / 64.0f;
              const float t = e + float(int32_t(h.random.Below(1u << 12)) - (1 << 11)) / 64.0f;
              guest::StoreU32(h.base, pass + 400 + 4 * k, FloatBits(e));
              guest::StoreU32(h.base, pass + 416 + 4 * k, FloatBits(t));
              guest::StoreU32(h.base, pass + 432 + 4 * k, FloatBits(k == 1 ? 1.0f : 0.0f));
            }
            guest::StoreU32(h.base, pass + 448,
                            FloatBits(float(int32_t(h.random.Below(2000)) - 1000) / 300.0f));
            guest::StoreU32(h.base, pass + 368, FloatBits(1.0908f));
            guest::StoreU32(h.base, pass + 384, FloatBits(1.0f));
            guest::StoreU32(h.base, pass + 388, FloatBits(5000.0f));
            guest::StoreU32(h.base, pass + 504, 1280);
            guest::StoreU32(h.base, pass + 508, 720);
          } else {
            guest::StoreU32(h.base, pass + 504, RandomSize(h.random));
            guest::StoreU32(h.base, pass + 508, RandomSize(h.random));
          }
        },
        [&](PPCContext& t) { t.r3.u64 = pass; }, __imp__sub_820A3978, NativePassUpdate,
        fmt::format("case {} type {} kind {} game {}", i, int32_t(type), kind, game),
        [&](Outputs& out) {
          out.Word(guest::LoadU32(h.base, command_list::kOpenChunks));
          scene.Restore();
        },
        nullptr, kScratchBytes);
  }
  results.Report();
}

// CameraPass::FromViewport: a viewports block (its pointer pointed at it for
// the case) with two viewports in the scratch area, a pass as Update's
// cases make it, the entry's fields random or a camera's.
void FromViewportCases(Context& h) {
  const SceneGlobals scene(h.base);
  const Globals globals(h.base);
  Results results("sub_820AA4A8 CameraPass::FromViewport");
  const uint32_t pass = h.data + FakeScene::kPass;
  const uint32_t block = h.data + 1632, viewports[2] = {h.data + 1664, h.data + 2112};
  for (int i = 0; i < 300; i++) {
    const uint32_t type = h.random.Below(22);
    const uint32_t kind = RandomKind(h.random);
    const uint32_t player = h.random.Below(2), entry = h.random.Below(5);
    const bool game = h.random.Below(2);
    Case(
        h, results, pass, {{16, 208}, {368, 396}, {464, 496}},
        [&] {
          FakeScene::Set(h, kind, h.random.Below(1000));
          RandomWords(h, pass, 148);
          guest::StoreU32(h.base, pass, 0x82001EC0);
          guest::StoreU32(h.base, pass + 584, type);
          guest::StoreU32(h.base, pass + 504, 1280);
          guest::StoreU32(h.base, pass + 508, 720);
          guest::StoreU32(h.base, block + 0, viewports[0]);
          guest::StoreU32(h.base, block + 4, viewports[1]);
          guest::StoreU32(h.base, cm::kViewports, block);
          const uint32_t at = viewports[player] + 80 * entry;
          RandomWords(h, at, 20);
          if (game) {
            for (uint32_t k = 0; k < 3; k++) {
              const float e = float(int32_t(h.random.Below(1u << 20)) - (1 << 19)) / 64.0f;
              const float t = e + float(int32_t(h.random.Below(1u << 12)) - (1 << 11)) / 64.0f;
              guest::StoreU32(h.base, at + 16 + 4 * k, FloatBits(e));
              guest::StoreU32(h.base, at + 32 + 4 * k, FloatBits(t));
              guest::StoreU32(h.base, at + 48 + 4 * k, FloatBits(k == 1 ? 1.0f : 0.0f));
            }
            guest::StoreU32(h.base, at + 64, FloatBits(float(int32_t(h.random.Below(2000)) - 1000) / 300.0f));
            guest::StoreU32(h.base, at + 68, FloatBits(666.33f));
            guest::StoreU32(h.base, at + 72, FloatBits(5000.0f));
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = pass;
          t.r4.u64 = player;
          t.r5.u64 = entry;
        },
        __imp__sub_820AA4A8, NativeFromViewport,
        fmt::format("case {} type {} kind {} player {} entry {} game {}", i, type, kind, player,
                    entry, game),
        [&](Outputs& out) {
          out.Word(guest::LoadU32(h.base, command_list::kOpenChunks));
          scene.Restore();
        },
        &globals, kScratchBytes);
  }
  results.Report();
}

}  // namespace

// CameraPasses::MakeRenderTarget on every id (13 above the range), id 1 in
// modes 0 to 2, with the old texture a fake resource in the scratch area
// (its count 2 or more, so Release only counts it down) or none. The 16
// globals at 0x826BABC0 (the mode and the targets) are saved and put back,
// and the textures made freed, so the game's are untouched. The device lock
// is the game's (taken and given back on this thread). Compared: r3, the
// fake's count, and each global: a new texture's header with its addresses'
// top 20 bits cleared (they differ by allocation), others exactly.
void MakeRenderTargetCases(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Results results("sub_8209FBF0 CameraPasses::MakeRenderTarget");
  constexpr uint32_t kGlobals = 0x826BABC0, kWords = 16;
  uint32_t saved[kWords];
  for (uint32_t i = 0; i < kWords; i++) {
    saved[i] = guest::LoadU32(h.base, kGlobals + 4 * i);
  }
  const uint32_t fake = h.data + 16;
  for (int c = 0; c < 32; c++) {
    const uint32_t id = c < 14 ? uint32_t(c) : c < 17 ? 1 : h.random.Below(14);
    const uint32_t mode = c >= 14 && c < 17 ? uint32_t(c - 14) : h.random.Below(3);
    const bool old = c % 2 == 1 || c >= 17;
    const uint32_t count = 2 + h.random.Below(4);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (uint32_t i = 0; i < kWords; i++) {
        guest::StoreU32(h.base, kGlobals + 4 * i, 0);
      }
      guest::StoreU32(h.base, cm::kMode, mode);
      guest::StoreU32(h.base, fake + 0, 0);
      guest::StoreU32(h.base, fake + 4, count);
      static constexpr uint32_t kTargets[] = {0x826BABE8, 0, 0x826BABDC, 0x826BABC8, 0x826BABD4,
                                              0x826BABFC, 0x826BABF8, 0x826BABF4, 0x826BABF0,
                                              0x826BABD0, 0x826BABD8, 0x826BABE0, 0x826BABC4};
      if (old && id <= 12) {
        const uint32_t global = id != 1 ? kTargets[id] : mode == 0 ? 0x826BABEC : 0x826BABCC;
        guest::StoreU32(h.base, global, fake);
      }
      uint32_t before[kWords];
      for (uint32_t i = 0; i < kWords; i++) {
        before[i] = guest::LoadU32(h.base, kGlobals + 4 * i);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = id;
      bool ran = true;
      if (v == 0) {
        const bool flag = ITC_FLAG(native_cameras);
        ITC_FLAG(native_cameras) = false;
        __imp__sub_8209FBF0(t, h.base);
        ITC_FLAG(native_cameras) = flag;
      } else {
        ran = hooks::CameraPassesMakeRenderTarget(t, h.base);
      }
      out[v].Word(ran);
      out[v].Word(t.r3.u32);
      out[v].Word(guest::LoadU32(h.base, fake + 4));
      for (uint32_t i = 0; i < kWords; i++) {
        const uint32_t word = guest::LoadU32(h.base, kGlobals + 4 * i);
        if (word == before[i] || word == 0) {
          out[v].Word(word);
          continue;
        }
        out[v].Word(word & 0xFFF);
        std::vector<uint8_t> header = Read(h.base, word, 52);
        const uint32_t data = guest::LoadU32(h.base, word + 32) & 0xFFFFF000;
        const uint32_t mips = guest::LoadU32(h.base, word + 48) & 0xFFFFF000;
        out[v].Word(data != 0);
        out[v].Word(mips != 0);
        for (int k = 0; k < 3; k++) {
          header[32 + k] = 0;
          header[48 + k] = 0;
        }
        header[35] &= 0x0F;
        header[51] &= 0x0F;
        out[v].Bytes(header);
        if (mips) {
          memory::crt::XMemFree(mips, 0xBC800000);
        }
        memory::crt::XMemFree(data, 0xBC800000);
        memory::crt::XMemFree(word, 0x24800000);
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: id {} mode {} old {}", c, id, mode, old ? "set" : "none"));
  }
  for (uint32_t i = 0; i < kWords; i++) {
    guest::StoreU32(h.base, kGlobals + 4 * i, saved[i]);
  }
  results.Report();
}

// CameraPass::SetUpSurfaces on a pass in the scratch area of every type
// (-1 to 21), in modes 0 to 2, at the game's sizes, its old surfaces fake
// resources (counts of 2 or more, so Release only counts them down) or none.
// As MakeRenderTargetCases: the 16 globals saved and put back, the textures
// and surfaces made freed (the level 0 surfaces the game drops leak, as
// they do in the game), the device lock the game's; types 5-9 in mode 1
// get a cube map made first (pass 4's, which they don't make). Types 0-2's surfaces
// are the device's (AddRef'ed: released again after). Compared: the fakes'
// counts, each global (a new texture's header, its addresses' top 20 bits
// cleared; where the header is isn't, as the leaks move the allocations),
// and +576 and +580: a new surface's header (a cube face's
// parent, +24, as whether it's the texture made), others exactly. r3 isn't
// compared (no caller reads it; the native code leaves the last guest
// call's).
void SetUpSurfacesCases(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Results results("sub_820A2F40 CameraPass::SetUpSurfaces");
  constexpr uint32_t kGlobals = 0x826BABC0, kWords = 16;
  uint32_t saved[kWords];
  for (uint32_t i = 0; i < kWords; i++) {
    saved[i] = guest::LoadU32(h.base, kGlobals + 4 * i);
  }
  const uint32_t fakes = h.data + 16, pass = h.data + 1024;
  static constexpr uint32_t kSizes[][2] = {{1280, 720}, {1024, 1024}, {512, 512}, {256, 256},
                                           {640, 360}};
  for (int c = 0; c < 100; c++) {
    const int32_t type = c < 23 ? c - 1 : int32_t(h.random.Below(23)) - 1;
    const uint32_t mode = c < 23 ? 0 : c < 46 ? 1 : h.random.Below(3);
    const uint32_t* size = kSizes[h.random.Below(5)];
    const bool old = h.random.Below(2);
    const uint32_t count = 2 + h.random.Below(4);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i < kWords; i++) {
        guest::StoreU32(h.base, kGlobals + 4 * i, 0);
      }
      guest::StoreU32(h.base, cm::kMode, mode);
      for (uint32_t o : {fakes, fakes + 32}) {
        guest::StoreU32(h.base, o + 0, 0);
        guest::StoreU32(h.base, o + 4, count);
      }
      guest::StoreU32(h.base, pass + 504, size[0]);
      guest::StoreU32(h.base, pass + 508, size[1]);
      guest::StoreU32(h.base, pass + 576, old ? fakes : 0);
      guest::StoreU32(h.base, pass + 580, old ? fakes + 32 : 0);
      guest::StoreU32(h.base, pass + 584, uint32_t(type));
      if (mode == 1 && type >= 5 && type <= 9) {
        // Types 5-9 don't make render target 1: the cube map pass 4 made.
        guest::StoreU32(h.base, 0x826BABCC,
                        graphics::CreateTexture(h.base, 64, 64, 6, 1, 1, 0x182801B6, 18));
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = pass;
      bool ran = true;
      if (v == 0) {
        const bool flag = ITC_FLAG(native_cameras);
        ITC_FLAG(native_cameras) = false;
        __imp__sub_820A2F40(t, h.base);
        ITC_FLAG(native_cameras) = flag;
      } else {
        ran = hooks::CameraPassSetUpSurfaces(t, h.base);
      }
      out[v].Word(ran);
      out[v].Word(guest::LoadU32(h.base, fakes + 4));
      out[v].Word(guest::LoadU32(h.base, fakes + 36));
      // The textures made (to free after the surfaces).
      std::vector<uint32_t> textures;
      for (uint32_t i = 0; i < kWords; i++) {
        const uint32_t word = guest::LoadU32(h.base, kGlobals + 4 * i);
        if (i == 0 || word == 0) {  // the mode
          out[v].Word(word);
          continue;
        }
        textures.push_back(word);
        out[v].Word(1);
        std::vector<uint8_t> header = Read(h.base, word, 52);
        out[v].Word((guest::LoadU32(h.base, word + 32) & 0xFFFFF000) != 0);
        out[v].Word((guest::LoadU32(h.base, word + 48) & 0xFFFFF000) != 0);
        for (int k = 0; k < 3; k++) {
          header[32 + k] = 0;
          header[48 + k] = 0;
        }
        header[35] &= 0x0F;
        header[51] &= 0x0F;
        out[v].Bytes(header);
      }
      const bool device = type >= 0 && type <= 2;
      const bool made = type == 3 || type == 4 || (type >= 10 && type <= 20);
      for (uint32_t field : {576u, 580u}) {
        const uint32_t surface = guest::LoadU32(h.base, pass + field);
        const bool cube_face = field == 576 && type >= 6 && type <= 9 && mode == 1;
        if (surface == 0 || !(made || cube_face)) {
          out[v].Word(surface);
          if (device && surface != 0) {
            PPCContext r = CallContext(h);
            r.r3.u64 = surface;
            sub_822A67B0(r, h.base);
          }
          continue;
        }
        // Its address differs by allocation (the dropped level 0 surfaces
        // leak, so each side's come at other addresses): whether it's set.
        out[v].Word(1);
        std::vector<uint8_t> header = Read(h.base, surface, 48);
        if (cube_face) {
          const uint32_t parent = guest::LoadU32(h.base, surface + 24);
          out[v].Word(std::find(textures.begin(), textures.end(), parent) != textures.end());
          for (int k = 24; k < 28; k++) {
            header[k] = 0;
          }
        }
        out[v].Bytes(header);
        memory::crt::XMemFree(surface, 0x24800000);
      }
      for (uint32_t texture : textures) {
        const uint32_t data = guest::LoadU32(h.base, texture + 32) & 0xFFFFF000;
        const uint32_t mips = guest::LoadU32(h.base, texture + 48) & 0xFFFFF000;
        if (mips) {
          memory::crt::XMemFree(mips, 0xBC800000);
        }
        memory::crt::XMemFree(data, 0xBC800000);
        memory::crt::XMemFree(texture, 0x24800000);
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: type {} mode {} {} x {} old {}", c, type, mode, size[0],
                              size[1], old ? "set" : "none"));
  }
  for (uint32_t i = 0; i < kWords; i++) {
    guest::StoreU32(h.base, kGlobals + 4 * i, saved[i]);
  }
  results.Report();
}

// CameraPass::Init on a pass in the scratch area (FakeScene's, for
// CameraPass::Update's viewport chunk) of every type (-1 to 21, then
// random), in modes 0 to 2, the pass's words random first. As
// SetUpSurfacesCases: the 16 globals saved and put back, the textures and
// surfaces made freed (types 0-2's device surfaces released again), the
// device lock the game's; types 5-9 in mode 1 get a cube map made first.
// Compared: the scratch area but +576 and +580 exactly, but for the
// pass's float ranges as Update's cases have them (Update reads the random
// words left there, NaNs among them) as floats, +576 and +580 as SetUpSurfacesCases does, each global, the open
// chunks' count and r3.
// CameraPasses::Init and InitPasses with CameraPass::Init recorded, not
// run (hooks::camera_pass_init_calls), so the game's passes stay as they
// are: the 16 words at 0x826BABC0 random first (the mode 0, 1, 2 or any,
// 0x826BABE4 any), pass 4's field of view random; SetMode runs (the
// original's on the recompiled side). Compared: the recorded calls, the 16
// words, pass 4's field of view (a float), r3, r4 and f1. The words and
// the field of view put back after.
void PassesInitCases(Context& h) {
  constexpr uint32_t kGlobals = 0x826BABC0, kWords = 16;
  const uint32_t fov_at = cm::Pass(4) + 368;
  uint32_t saved[kWords];
  for (uint32_t i = 0; i < kWords; i++) {
    saved[i] = guest::LoadU32(h.base, kGlobals + 4 * i);
  }
  const uint32_t saved_fov = guest::LoadU32(h.base, fov_at);
  for (bool all : {true, false}) {
    Results results(all ? "sub_8209F9B8 CameraPasses::Init" : "sub_8209F960 CameraPasses::InitPasses");
    for (int c = 0; c < 40; c++) {
      uint32_t words[kWords];
      for (uint32_t& w : words) {
        w = uint32_t(h.random.Next());
      }
      words[0] = c < 30 ? uint32_t(c % 3) : uint32_t(h.random.Next());
      const uint32_t fov = FloatWord(h.random);
      const double f1 = double(std::bit_cast<float>(FloatWord(h.random)));
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (uint32_t i = 0; i < kWords; i++) {
          guest::StoreU32(h.base, kGlobals + 4 * i, words[i]);
        }
        guest::StoreU32(h.base, fov_at, fov);
        std::vector<std::array<uint32_t, 3>> calls;
        hooks::camera_pass_init_calls = &calls;
        PPCContext t = CallContext(h);
        t.r3.u64 = 0x12345678;
        t.r4.u64 = 0x9ABCDEF0;
        t.f1.f64 = f1;
        if (v == 0) {
          const bool flag = ITC_FLAG(native_cameras);
          ITC_FLAG(native_cameras) = false;
          (all ? __imp__sub_8209F9B8 : __imp__sub_8209F960)(t, h.base);
          ITC_FLAG(native_cameras) = flag;
        } else {
          hooks::CameraPassesInit(t, h.base, all);
        }
        hooks::camera_pass_init_calls = nullptr;
        out[v].Word(uint32_t(calls.size()));
        for (const auto& call : calls) {
          for (uint32_t w : call) {
            out[v].Word(w);
          }
        }
        for (uint32_t i = 0; i < kWords; i++) {
          out[v].Word(guest::LoadU32(h.base, kGlobals + 4 * i));
        }
        out[v].Float(guest::LoadU32(h.base, fov_at));
        out[v].Word(t.r3.u32);
        out[v].Word(t.r4.u32);
        out[v].Double(t.f1.u64);
      }
      results.Check(out[0], out[1], fmt::format("case {} mode {:08X}", c, words[0]));
    }
    results.Report();
  }
  for (uint32_t i = 0; i < kWords; i++) {
    guest::StoreU32(h.base, kGlobals + 4 * i, saved[i]);
  }
  guest::StoreU32(h.base, fov_at, saved_fov);
}

void InitPassCases(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const SceneGlobals scene(h.base);
  Results results("sub_820A3DB0 CameraPass::Init");
  constexpr uint32_t kGlobals = 0x826BABC0, kWords = 16;
  uint32_t saved[kWords];
  for (uint32_t i = 0; i < kWords; i++) {
    saved[i] = guest::LoadU32(h.base, kGlobals + 4 * i);
  }
  const uint32_t pass = h.data + FakeScene::kPass;
  for (int c = 0; c < 72; c++) {
    const uint32_t type = c < 23   ? uint32_t(c - 1)
                          : c < 46 ? uint32_t(c - 24)
                          : h.random.Below(6) ? h.random.Below(22) : uint32_t(h.random.Next());
    const uint32_t mode = c < 23 ? 0 : c < 46 ? 1 : h.random.Below(3);
    const uint32_t used = h.random.Below(1000);
    std::vector<uint32_t> words(148);
    for (uint32_t& w : words) {
      w = uint32_t(h.random.Next());
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      FakeScene::Set(h, 0, used);
      guest::StoreU32s(h.base, pass, words.data(), 148);
      for (uint32_t i = 0; i < kWords; i++) {
        guest::StoreU32(h.base, kGlobals + 4 * i, 0);
      }
      guest::StoreU32(h.base, cm::kMode, mode);
      if (mode == 1 && type >= 5 && type <= 9) {
        guest::StoreU32(h.base, 0x826BABCC,
                        graphics::CreateTexture(h.base, 64, 64, 6, 1, 1, 0x182801B6, 18));
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = pass;
      t.r4.u64 = type;
      if (v == 0) {
        const bool flag = ITC_FLAG(native_cameras);
        ITC_FLAG(native_cameras) = false;
        __imp__sub_820A3DB0(t, h.base);
        ITC_FLAG(native_cameras) = flag;
      } else {
        hooks::CameraPassInit(t, h.base);
      }
      out[v].Word(t.r3.u32);
      out[v].Word(guest::LoadU32(h.base, command_list::kOpenChunks));
      // The pass's float ranges (CameraPass::Update's, from the random words
      // there: NaNs among them) as floats, the rest exactly.
      out[v].Bytes(Read(h.base, h.data, FakeScene::kPass));
      uint32_t at = 0;
      for (auto [from, to] : {std::pair{16u, 208u}, {368u, 396u}, {464u, 496u}}) {
        out[v].Bytes(Read(h.base, pass + at, from - at));
        out[v].Floats(h.base, pass + from, (to - from) / 4);
        at = to;
      }
      out[v].Bytes(Read(h.base, pass + at, 576 - at));
      out[v].Bytes(Read(h.base, pass + 584, kScratchBytes - FakeScene::kPass - 584));
      std::vector<uint32_t> textures;
      for (uint32_t i = 0; i < kWords; i++) {
        const uint32_t word = guest::LoadU32(h.base, kGlobals + 4 * i);
        if (i == 0 || word == 0) {
          out[v].Word(word);
          continue;
        }
        textures.push_back(word);
        out[v].Word(1);
        std::vector<uint8_t> header = Read(h.base, word, 52);
        for (int k = 0; k < 3; k++) {
          header[32 + k] = 0;
          header[48 + k] = 0;
        }
        header[35] &= 0x0F;
        header[51] &= 0x0F;
        out[v].Bytes(header);
      }
      const bool device = type <= 2;
      const bool made = type == 3 || type == 4 || (type >= 10 && type <= 20);
      for (uint32_t field : {576u, 580u}) {
        const uint32_t surface = guest::LoadU32(h.base, pass + field);
        const bool cube_face = field == 576 && type >= 6 && type <= 9 && mode == 1;
        if (surface == 0 || !(made || cube_face)) {
          out[v].Word(surface);
          if (device && surface != 0) {
            PPCContext r = CallContext(h);
            r.r3.u64 = surface;
            sub_822A67B0(r, h.base);
          }
          continue;
        }
        out[v].Word(1);
        std::vector<uint8_t> header = Read(h.base, surface, 48);
        if (cube_face) {
          const uint32_t parent = guest::LoadU32(h.base, surface + 24);
          out[v].Word(std::find(textures.begin(), textures.end(), parent) != textures.end());
          for (int k = 24; k < 28; k++) {
            header[k] = 0;
          }
        }
        out[v].Bytes(header);
        memory::crt::XMemFree(surface, 0x24800000);
      }
      for (uint32_t texture : textures) {
        const uint32_t data = guest::LoadU32(h.base, texture + 32) & 0xFFFFF000;
        const uint32_t mips = guest::LoadU32(h.base, texture + 48) & 0xFFFFF000;
        if (mips) {
          memory::crt::XMemFree(mips, 0xBC800000);
        }
        memory::crt::XMemFree(data, 0xBC800000);
        memory::crt::XMemFree(texture, 0x24800000);
      }
      scene.Restore();
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: type {} mode {}", c, int32_t(type), mode));
  }
  for (uint32_t i = 0; i < kWords; i++) {
    guest::StoreU32(h.base, kGlobals + 4 * i, saved[i]);
  }
  results.Report();
}

// --- Viewports::Init (with Viewport::Init): on the game's viewports
// pointer, camera pass 0's near plane, the three vectors and their guard,
// and the exit table, each put back after a side; the new blocks compared
// by their contents, then freed. ---

void NativeViewportsInit(PPCContext& ctx, uint8_t* base) {
  if (const auto viewport = hooks::ViewportsInit(ctx, base)) {
    ctx.r3.u64 = *viewport;
  } else {
    __imp__sub_820AB358(ctx, base);
  }
}

void ViewportsInitCases(Context& h) {
  Results results("sub_820AB358 Viewports::Init, sub_820AAFF0 Viewport::Init");
  if (!hooks::NativeHeapActive() || !hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: cameras: --native_heap or native atexit off: Viewports::Init's "
                 "cases skipped");
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  constexpr uint32_t kVectors = 0x826C2CF0, kVectorBytes = 0x34;  // to the guard, 0x826C2D20
  constexpr uint32_t kNear = 0x826BAC00 + 384;
  const uint32_t saved_viewports = guest::LoadU32(h.base, cm::kViewports);
  const uint32_t saved_near = guest::LoadU32(h.base, kNear);
  const auto saved_vectors = Read(h.base, kVectors, kVectorBytes);
  for (int c = 0; c < 12; c++) {
    // 0-1: as the game has them (made); then each guard bit cleared at
    // random over random bytes.
    std::vector<uint8_t> vectors = saved_vectors;
    if (c >= 2) {
      for (auto& byte : vectors) {
        byte = uint8_t(h.random.Next());
      }
      uint32_t guard = uint32_t(h.random.Next());
      if (c == 2) {
        guard &= ~7u;
      }
      for (int k = 0; k < 4; k++) {
        vectors[0x30 + k] = uint8_t(guard >> (24 - 8 * k));
      }
    }
    const uint32_t near_plane = c >= 2 ? uint32_t(h.random.Next()) : saved_near;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, kVectors, vectors.data(), kVectorBytes);
      guest::StoreU32(h.base, kNear, near_plane);
      const uint32_t mark = exit_table::Mark(h.base);
      PPCContext t = CallContext(h);
      (v == 0 ? __imp__sub_820AB358 : NativeViewportsInit)(t, h.base);
      const uint32_t block = guest::LoadU32(h.base, cm::kViewports);
      const uint32_t first = guest::LoadU32(h.base, block);
      const uint32_t second = guest::LoadU32(h.base, block + 4);
      out[v].Word(guest::LoadU32(h.base, block + 8));
      out[v].Bytes(Read(h.base, first, 1872));
      out[v].Bytes(Read(h.base, second, 1872));
      out[v].Word(t.r3.u32 == second);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(guest::LoadU32(h.base, kNear));
      out[v].Bytes(Read(h.base, kVectors, kVectorBytes));
      exit_table::Take(h.base, mark, out[v]);
      memory::FreePages(first);
      memory::FreePages(second);
      memory::FreePages(block);
    }
    guest::StoreU32(h.base, cm::kViewports, saved_viewports);
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  guest::StoreU32(h.base, cm::kViewports, saved_viewports);
  guest::StoreU32(h.base, kNear, saved_near);
  Write(h.base, kVectors, saved_vectors.data(), kVectorBytes);
  results.Report();
}

void Cameras(Context& h) {
  SetFieldOfViewCases(h);
  BuildProjectionCases(h);
  SetViewportSizeCases(h);
  CameraUpdateCases(h);
  CameraConstructCases(h);
  PassStateCases(h);
  ViewportCases(h);
  ViewportEntriesCases(h);
  RecordViewportCases(h);
  RecordClearCases(h);
  PassUpdateCases(h);
  FromViewportCases(h);
  RecordPassCases(h);
  RecordPassesCases(h);
  MakeRenderTargetCases(h);
  SetUpSurfacesCases(h);
  InitPassCases(h);
  PassesInitCases(h);
  ViewportsInitCases(h);
}

}  // namespace harness
