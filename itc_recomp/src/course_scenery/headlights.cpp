// Scenery::RecordHeadlightProjectors (sub_820F34C8, course_scenery.h;
// docs/functions/course-scenery.md): the cars' headlight slots and up to
// five headlight projectors on the scene's command list, step for step as
// the translated code (built with strict floats, CMakeLists.txt; the float
// and vector steps are the car scene draw's, car_scene_draw/ops.h). Its
// 640-byte frame keeps the original's temporaries at their offsets (its
// callees and frame interpolation read them by guest address); its callees'
// plain C++ runs inside the checks their diagnostics' wraps put around the
// originals, and the chunks are recorded with the original's calls (the
// render manager's getters before every command list call).

#include "course_scenery/course_scenery.h"

#include "car_scene_draw/car_scene_draw.h"
#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "cameras/cameras.h"
#include "command_list/command_list.h"
#include "debug/frame_state_diff.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <initializer_list>

namespace course_scenery {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kFrame = 640;
constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kRenderApp = 0x826BABA0;  // the render manager
constexpr uint32_t kCameraPasses = 0x826BAC00;
// The headlight slots: eight 16-byte vectors each.
constexpr uint32_t kSlotsPosition = 0x8296B9E0;
constexpr uint32_t kSlotsShape = 0x8296BA60;
constexpr uint32_t kSlotsColor = 0x8296BAE0;
constexpr uint32_t kSlotsColor2 = 0x8296BB60;
constexpr uint32_t kSlotsDirection = 0x8296BBE0;
constexpr uint32_t kSlotsBeam = 0x8296BC60;
constexpr uint32_t kSceneLight = 0x8265ED00;  // the scene's +5328
constexpr uint32_t kEyeCopy = 0x8265ECE0;     // the pass's eye
constexpr uint32_t kQueueKeyA = 0x8296B90C;
constexpr uint32_t kQueueKeyB = 0x8296B900;
constexpr uint32_t kNear = 0x82001FBC;
constexpr uint32_t kFar = 0x82002FA8;

// The originals' callees (their wraps in src/debug/frame_state_diff.cpp and
// src/hooks/vehicle_accessors.cpp, and the originals where the plain C++
// declines).
constexpr uint32_t kListAt = 0x82593FC0;
constexpr uint32_t kBeam = 0x820DAAC8;
constexpr uint32_t kBeamShape = 0x820DAB10;
constexpr uint32_t kBeamColor = 0x820DAA38;
constexpr uint32_t kBuild = 0x82597610;
constexpr uint32_t kOpenChunk = 0x8258C9D0;
constexpr uint32_t kAllocateSlots = 0x8258CAA0;
constexpr uint32_t kAllocateBytes = 0x8258CB98;
constexpr uint32_t kCameraPassUpdate = 0x820A3978;

// Flush-to-zero off, as the callees' plain C++ expects at entry.
void FlushOff() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

// A wrapped callee's call: nothing when --debug_hide names it (r3 then
// `r3`, the argument the original set), else `body` between
// FrameStateDiff_Before and After, flush-to-zero off around it. Returns r3.
template <typename Body>
uint64_t Wrapped(uint8_t* base, uint32_t function, uint64_t r3, Body body) {
  if (debug::IsHidden(function)) {
    return r3;
  }
  debug::FrameStateDiff_Before(base);
  FlushOff();
  const uint64_t left = body();
  FlushOff();
  debug::FrameStateDiff_After(base, "draw step part", function);
  return left;
}

class Recorder {
 public:
  Recorder(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls, uint32_t f)
      : base_(base), calls_(calls), f_(f) {}

  uint64_t Run(uint32_t description);

 private:
  uint64_t Call(uint32_t function, std::initializer_list<uint64_t> regs, uint32_t lr,
                std::initializer_list<double> fregs = {}) {
    const uint64_t r3 = calls_.call(function, regs.begin(), int(regs.size()), fregs.begin(),
                                    int(fregs.size()), f_, lr);
    FlushOff();
    return r3;
  }
  uint64_t VtableCall(uint32_t object, uint32_t slot, uint32_t lr,
                      std::initializer_list<uint64_t> more = {},
                      std::initializer_list<double> fregs = {}) {
    uint64_t regs[4] = {object, 0, 0, 0};
    int count = 1;
    for (const uint64_t r : more) {
      regs[count++] = r;
    }
    const uint32_t function = Load(base_, Load(base_, object) + 4 * slot);
    const uint64_t r3 =
        calls_.call(function, regs, count, fregs.begin(), int(fregs.size()), f_, lr);
    FlushOff();
    return r3;
  }
  // The render manager's getters (slot 8, then `slot` of its answer) with
  // the return addresses x and x + 16: the command list (1) or the render
  // queue (2).
  uint32_t Get(uint32_t slot, uint32_t x) {
    const uint32_t object = uint32_t(VtableCall(Load(base_, kRenderApp), 8, x));
    return uint32_t(VtableCall(object, slot, x + 16));
  }
  void Open(uint32_t x) {
    const uint32_t list = Get(1, x);
    if (command_list::OpenChunk(base_, list) == command_list::OpenResult::kNested) {
      Call(kOpenChunk, {list}, x + 0x14);
    }
  }
  uint32_t Allocate(uint32_t x) {
    const uint32_t list = Get(1, x);
    const uint32_t first = command_list::AllocateSlots(base_, list, 1);
    if (first != 0) {
      return first;
    }
    // A full list: the original prints its message and returns 0.
    return uint32_t(Call(kAllocateSlots, {list, 1}, x + 0x18));
  }
  uint32_t AllocateBytes(uint32_t bytes, uint32_t x) {
    const uint32_t list = Get(1, x);
    const uint32_t first = command_list::AllocateBytes(base_, list, bytes, false);
    if (first != 0) {
      return first;
    }
    return uint32_t(Call(kAllocateBytes, {list, bytes, 0}, x + 0x1C));
  }
  void Transform(uint32_t out, uint32_t m, uint32_t v) {
    debug::Vec4Transform(base_, out, m, v);
    FlushOff();
  }
  uint8_t* base_;
  const car_scene_draw::SceneDrawCalls& calls_;
  uint32_t f_;
};

uint64_t Recorder::Run(uint32_t description) {
  const uint32_t f = f_;
  // The camera pass from the viewport, then CameraPass::Update from
  // FromViewport's frame (as its adapter).
  const uint32_t pass = Load(base_, description + 20) * 592u + kCameraPasses;
  Store(base_, description + 8, pass);
  cameras::FromViewport(base_, pass, Load(base_, description + 12),
                        Load(base_, description + 16));
  {
    const uint32_t from_viewport = f - 112;
    Store(base_, from_viewport, f);
    const uint64_t regs[1] = {pass};
    calls_.call(kCameraPassUpdate, regs, 1, nullptr, 0, from_viewport, 0x820AA534);
    FlushOff();
  }
  uint64_t r3 = vehicle_accessors::VehicleGet(base_, 0);
  if (r3 != 0) {
    const uint32_t scene0 = Load(base_, kScene);
    const uint32_t lights = scene0 + 5248;  // the original's r31
    const double zero = Lfs(base_, kZero);  // f30
    const double one = Lfs(base_, kOne);    // f31
    for (uint32_t at : {96u, 100u, 104u, 108u, 80u, 84u, 88u, 92u}) {
      Stfs(base_, f + at, zero);
    }
    for (uint32_t i = 0; i < 128; i += 16) {
      Copy16(base_, kSlotsColor + i, f + 96);
      Copy16(base_, kSlotsColor2 + i, f + 80);
    }
    Copy16(base_, f + 208, lights + 128);
    Stfs(base_, f + 128, zero);
    Stfs(base_, f + 136, zero);
    Stfs(base_, f + 140, zero);
    Stfs(base_, f + 132, one);
    Stfs(base_, f + 124, one);
    Stfs(base_, f + 156, one);
    const int32_t count = int32_t(Load(base_, scene0 + 5036));
    const uint32_t limit = count < 8 ? uint32_t(count) : 8u;  // the original's r17
    const double far_plane = Lfs(base_, kFar);  // f28
    const double near_plane = Lfs(base_, kNear);  // f29
    uint32_t found = 0;  // r18
    for (uint32_t n = 0; n < limit; n++) {
      const uint32_t list = Load(base_, kScene) + 5028;
      const uint32_t model = uint32_t(Wrapped(base_, kListAt, list, [&] {
        return uint64_t(vehicle_accessors::ListAt(base_, list, n));
      }));
      r3 = model;
      if (model == 0 || int32_t(Load(base_, model + 16)) != 3 || Load(base_, model + 5048) == 0) {
        continue;
      }
      const int32_t lit = int32_t(Load(base_, model + 32));
      if (lit != 2 && lit != 1) {
        continue;
      }
      r3 = Wrapped(base_, kBeam, f + 320, [&] {
        car_scene_draw::SceneModelBeam(base_, f + 320, model);
        return uint64_t(f + 320);
      });
      int32_t slot = int32_t(found);
      found++;
      if (slot >= 8) {
        break;
      }
      if (Load(base_, description + 20) == 2) {
        if (slot == 0) {
          slot = 1;
        } else if (slot == 1) {
          slot = 0;
        }
      }
      // The car's matrix at +256; the eye (+112) = the scene's +5376
      // through it, then its last row cleared: the beam (+144) and up
      // (+128) turned by it.
      if (!native::Buffers_Memcpy(base_, f + 256, model + 4304, 64, 0x820F36D0)) {
        calls_.memcpy(f + 256, model + 4304, 64, f, 0x820F36D0);
      }
      FlushOff();
      Transform(f + 112, f + 256, f + 208);
      Stfs(base_, f + 316, zero);
      Stfs(base_, f + 312, zero);
      Stfs(base_, f + 308, zero);
      Stfs(base_, f + 304, zero);
      Transform(f + 144, f + 256, f + 320);
      Transform(f + 128, f + 256, f + 128);
      // Minus the beam (+80), normalized (+96); the scene's +5424 to +240.
      for (uint32_t i = 0; i < 16; i += 4) {
        Stfs(base_, f + 80 + i, Fneg(Lfs(base_, f + 144 + i)));
      }
      {
        const simde__m128 v = Lvx(base_, f + 80);
        Copy16(base_, f + 240, lights + 176);
        FlushToZero flush;
        Stvx(base_, f + 96, Normalized(v, Dot3(v, v)));
      }
      Wrapped(base_, kBeamShape, f + 400, [&] {
        car_scene_draw::SceneModelBeamShape(base_, f + 400, model);
        return uint64_t(f + 400);
      });
      Copy16(base_, f + 224, f + 400);
      // The two colors scaled by the scene's +48 and +64 (w 1.0).
      const uint32_t color = uint32_t(Wrapped(base_, kBeamColor, f + 416, [&] {
        return uint64_t(car_scene_draw::SceneModelBeamColor(base_, f + 416, model, f));
      }));
      Stfs(base_, f + 172, one);
      for (uint32_t i = 0; i < 12; i += 4) {
        Stfs(base_, f + 160 + i, Fmuls(Lfs(base_, color + i), Lfs(base_, lights + 48 + i)));
      }
      const uint32_t color2 = uint32_t(Wrapped(base_, kBeamColor, f + 432, [&] {
        return uint64_t(car_scene_draw::SceneModelBeamColor(base_, f + 432, model, f));
      }));
      r3 = color2;
      for (uint32_t i = 0; i < 12; i += 4) {
        Stfs(base_, f + 176 + i, Fmuls(Lfs(base_, lights + 64 + i), Lfs(base_, color2 + i)));
      }
      Stfs(base_, f + 188, one);
      // The slot's six vectors.
      const uint32_t at = uint32_t(slot) << 4;
      Copy16(base_, kSlotsPosition + at, f + 112);
      Copy16(base_, kSlotsColor + at, f + 160);
      Copy16(base_, kSlotsDirection + at, f + 96);
      Copy16(base_, kSlotsColor2 + at, f + 176);
      Copy16(base_, kSlotsBeam + at, f + 240);
      Copy16(base_, kSlotsShape + at, f + 224);
      if (slot >= 5) {
        // The original's last r3: the slots' copy's first doubleword (ld).
        r3 = uint64_t(Load(base_, f + 112)) << 32 | Load(base_, f + 116);
        continue;
      }
      // The projector: from the eye through the eye less the direction.
      Stfs(base_, f + 348, one);
      Stfs(base_, f + 364, one);
      Stfs(base_, f + 380, one);
      Stfs(base_, f + 396, one);
      for (uint32_t i = 0; i < 12; i += 4) {
        Stfs(base_, f + 192 + i, Fsubs(Lfs(base_, f + 112 + i), Lfs(base_, f + 96 + i)));
      }
      Stfs(base_, f + 204, one);
      const uint32_t out = f + 336, eye = f + 112, target = f + 192, up = f + 128;
      const double fov = Lfs(base_, lights + 188);
      if (car_scene_draw::PerspectiveDeclines(base_, fov)) {
        Call(kBuild, {out, eye, target, up}, 0x820F3944, {fov, one, near_plane, far_plane});
      } else {
        Wrapped(base_, kBuild, out, [&] {
          if (!car_scene_draw::ProjectorBuild(base_, out, eye, target, up, fov, one,
                                              near_plane, far_plane, f)) {
            ITC_LOG_ERROR("course_scenery: Projector::Build declined after its check");
          }
          return uint64_t(out);
        });
      }
      // Its chunk: a header, command 7 vertex c(0x77 + 4 x slot) x4, the
      // matrix (Matrix::Copy, noted for interpolation as its wrapper does),
      // command 36.
      Open(0x820F3958);
      const uint32_t chunk = Allocate(0x820F3980);
      Store(base_, chunk + 4, 0);
      guest::StoreU16(base_, chunk + 2, 5);
      {
        const uint32_t command = Allocate(0x820F39BC);
        const uint32_t reg = ((uint32_t(slot) << 10) + 0x7700u) | 0x10000u | 4u;
        Store(base_, command + 0, 7);
        Store(base_, command + 4, 4);
        Store(base_, command + 8, reg);
      }
      const uint32_t matrix = AllocateBytes(64, 0x820F3A0C);
      matrix_math::Copy(base_, matrix, out);
      interpolation::BlendState_OnCopy(base_, matrix, out, 0x820F3A30);
      FlushOff();
      {
        const uint32_t command = Allocate(0x820F3A44);
        Store(base_, command + 4, 0);
        Store(base_, command + 8, 0);
        Store(base_, command + 0, 36);
      }
      command_list::CloseChunk(base_, Get(1, 0x820F3A80));
      const bool second = Load(base_, description + 20) == 2;
      const uint32_t x = second ? 0x820F3AB4 : 0x820F3AD0;
      const uint32_t object = uint32_t(VtableCall(Load(base_, kRenderApp), 8, x));
      const uint32_t queue = uint32_t(VtableCall(object, 2, x + 16));
      r3 = VtableCall(queue, 4, 0x820F3AFC, {chunk, second ? 371u : 150u}, {zero});
    }
    Copy16Interleaved(base_, kSceneLight, lights + 80);
  }
  Store(base_, kQueueKeyA, Load(base_, description + 24));
  Store(base_, kQueueKeyB, Load(base_, description + 28));
  Copy16Interleaved(base_, kEyeCopy, Load(base_, description + 8) + 400);
  return r3;
}

}  // namespace

uint64_t RecordHeadlightProjectors(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                                   uint32_t stack, uint32_t description) {
  ITC_NATIVE_RUNNING("sub_820F34C8 Scenery::RecordHeadlightProjectors");
  const uint32_t f = stack - kFrame;
  Store(base, f, stack);  // the back chain, as stwu
  return Recorder(base, calls, f).Run(description);
}

}  // namespace course_scenery
