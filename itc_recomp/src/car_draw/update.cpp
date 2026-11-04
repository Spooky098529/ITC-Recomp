// Car::DrawUpdate as plain C++ (docs/functions/car-draw.md: the root of the
// car draw); see car_draw.h. Built with -ffp-model=strict (CMakeLists.txt):
// the boost's subtraction as the translated code's double rounded to single.
// Its callees run as their plain C++, each inside the checks its diagnostics'
// wrap (src/debug/frame_state_diff.cpp) puts around the original's call
// (--debug_hide, --find_per_frame_state), so those keep naming them.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "debug/frame_state_diff.h"
#include "guest.h"
#include "load_poll/load_poll.h"
#include "second_set/objects.h"

#include <rex/platform/fpscr.h>

#include <cstring>

namespace car_draw {

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kOne = 0x82000A30;    // 1.0
constexpr uint32_t kZero = 0x82000A34;   // 0.0
constexpr uint32_t kBehind = 0x82002A24;  // the cull's least w
// The render queue's sort keys the light kit draws under.
constexpr uint32_t kKeyA = 0x8296B90C;
constexpr uint32_t kKeyB = 0x8296B900;
// The scene lights' per-object slots (2 to 5 cleared before the wheels).
constexpr uint32_t kSlotsAmbient = 0x8296BA60;
constexpr uint32_t kSlotsColorsA = 0x8296BAE0;
constexpr uint32_t kSlotsColorsB = 0x8296BB60;
constexpr uint32_t kSlotsDirections = 0x8296BBE0;
constexpr uint32_t kSlots240 = 0x8296BC60;
// r24 and r28 as the original holds them at the wheels' and the interior's
// calls (lis's sign-extended addresses of the slots).
constexpr uint64_t kWheelsR24 = 0xFFFFFFFF8296BAE0ull;
constexpr uint64_t kInteriorR28 = 0xFFFFFFFF8296BA60ull;
// r28 at the other draws (lis -32148).
constexpr uint32_t kDrawsR28 = 0x826C0000;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
int32_t LoadS(uint8_t* base, uint32_t address) {
  return int32_t(guest::LoadU32(base, address));
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// An ld/std pair: the doubleword as it sits in guest memory.
void CopyDoubleword(uint8_t* base, uint32_t to, uint32_t from) {
  uint64_t word;
  std::memcpy(&word, base + from, 8);
  std::memcpy(base + to, &word, 8);
}

// The camera pass `index` (0 when its address's low word is 0 or it isn't
// enabled, +588).
uint32_t EnabledPass(uint8_t* base, uint32_t index) {
  const uint32_t pass = kCameraPasses + index * 592;
  if (pass == 0 || LoadS(base, pass + 588) == 0) {
    return 0;
  }
  return pass;
}

// Flush-to-zero off, as the callees' plain C++ expects at entry (the
// originals' adapters turn it off; the load poll's guest calls may leave it
// on).
void FlushOff() {
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}

// A callee's call inside its diagnostics' wrap: nothing when --debug_hide
// names it (r3 then the argument the original set, `r3`), else `body`
// between FrameStateDiff_Before and After. Returns r3 as it's left.
template <typename Body>
Left Wrapped(uint8_t* base, uint32_t function, uint64_t r3, Body body) {
  if (debug::IsHidden(function)) {
    return Left{false, r3};
  }
  debug::FrameStateDiff_Before(base);
  FlushOff();
  const Left left = body();
  FlushOff();
  debug::FrameStateDiff_After(base, "draw step part", function);
  return left;
}
// The same for a callee that returns r3 itself.
template <typename Body>
Left WrappedValue(uint8_t* base, uint32_t function, uint64_t r3, Body body) {
  return Wrapped(base, function, r3, [&] { return Left{false, uint64_t(body())}; });
}

}  // namespace

Left CarDrawUpdate(uint8_t* base, const Calls& calls, load_poll::Calls& poll,
                   const second_set::ReleaseCalls& release, uint32_t stack, uint64_t model_r3,
                   uint64_t caller_r24, bool advance_blink) {
  ITC_NATIVE_RUNNING("sub_820DAC68 Car::DrawUpdate");
  const uint32_t model = uint32_t(model_r3);
  const uint32_t frame = stack - kCarDrawUpdateFrame;
  Store(base, frame, stack);  // its back chain, as its stwu
  Left left{false, model_r3};
  auto r3_of = [&](uint32_t value) { return uint64_t(value); };

  // The boost: 1.0 off +4992.
  const double one = double(guest::LoadFloat(base, kOne));
  guest::StoreFloat(base, model + 4992,
                    float(double(guest::LoadFloat(base, model + 4992)) - one));
  if (LoadS(base, model + 4296) == 0 || LoadS(base, model + 5028) != 0) {
    return left;
  }
  const float zero = guest::LoadFloat(base, kZero);
  const uint32_t block = model + 4288;

  // In a race: the model's position through the current camera pass's
  // view-projection; behind the camera, nothing more; then the height cull.
  if (LoadS(base, Load(base, model + 5116) + 4) != 0) {
    const uint32_t d = Load(base, kScene) + 5072;  // the pass description
    const uint32_t position = frame + 80, projected = frame + 160;
    guest::StoreFloat(base, frame + 80, zero);
    guest::StoreFloat(base, frame + 84, zero);
    guest::StoreFloat(base, frame + 88, zero);
    guest::StoreFloat(base, frame + 92, float(one));
    guest::StoreFloat(base, frame + 172, float(one));
    debug::Vec4Transform(base, position, model + 4304, position);
    const uint32_t pass = EnabledPass(base, Load(base, d));
    debug::Vec4Transform(base, projected, pass + 144, position);
    left = Left{false, projected};
    FlushOff();
    if (double(guest::LoadFloat(base, frame + 172)) <
        double(guest::LoadFloat(base, kBehind))) {
      return left;
    }
    left = WrappedValue(base, 0x820DAB58, d, [&] {
      return SceneModelIsNearPlayerHeight(base, calls, frame, d, position);
    });
    if (int32_t(left.r3) == 0) {
      Store(base, d + 60, Load(base, d + 60) & 0xFFFFFF07u);
    }
  }

  // Traffic: its body's and wheels' finishers until both are done, the
  // apply steps on the call that finds them done, then their draws.
  if (LoadS(base, model + 32) == 3) {
    if (LoadS(base, model + 5036) == 0) {
      if (LoadS(base, model + 5096) == 0) {
        const uint32_t body = Load(base, model + 5092);
        left = WrappedValue(base, 0x820E5A68, body, [&] {
          return load_poll::SceneTrafficFinish(base, body, frame, poll);
        });
        Store(base, model + 5096, uint32_t(left.r3));
        if (int32_t(left.r3) != 0) {
          const uint32_t traffic = Load(base, model + 5092);
          const uint32_t wheels = Load(base, model + 5100);
          Store(base, wheels + 16780, Load(base, traffic + 9760) + 7072);
        }
      }
      if (LoadS(base, model + 5104) == 0) {
        const uint32_t wheels = Load(base, model + 5100);
        left = WrappedValue(base, 0x820E7328, wheels, [&] {
          return load_poll::SceneWheelsFinish(base, wheels, frame, poll);
        });
        Store(base, model + 5104, uint32_t(left.r3));
      }
      if (LoadS(base, model + 5096) != 0 && LoadS(base, model + 5104) != 0) {
        Store(base, model + 5036, 1);
      }
      if (LoadS(base, model + 5036) == 0) {
        return left;
      }
      if (LoadS(base, model + 5104) != 0) {
        const uint32_t wheels = Load(base, model + 5100);
        left = WrappedValue(base, 0x820E8908, wheels, [&] {
          return load_poll::SceneWheelsSetUpPoints(base, wheels, wheels, frame, poll);
        });
      }
      left = WrappedValue(base, 0x820D9EA0, model_r3, [&] {
        return load_poll::SceneModelApplyPalettes(base, model, model_r3, frame, poll);
      });
    }
    const uint32_t body = Load(base, model + 5092);
    left = Wrapped(base, 0x820E6A40, body, [&] {
      return SceneTrafficDraw(base, calls, frame, body, block, uint32_t(caller_r24));
    });
    const uint32_t wheels = Load(base, model + 5100);
    left = Wrapped(base, 0x820ECE28, wheels, [&] {
      return SceneWheelsDraw(base, calls, frame, wheels, block, caller_r24);
    });
    return left;
  }

  // Cars, until ready: each finisher whose flag is 0, its answer the flag;
  // ready once all are set (the cockpit's only in a race); on the call it
  // becomes ready, the apply steps once. Not ready: nothing more.
  if (LoadS(base, model + 5036) == 0) {
    auto finish = [&](uint32_t flag, uint32_t object_at, uint32_t function, auto step) {
      if (LoadS(base, model + flag) != 0) {
        return;
      }
      const uint32_t object = Load(base, model + object_at);
      left = WrappedValue(base, function, object, [&] { return step(object); });
      Store(base, model + flag, uint32_t(left.r3));
    };
    if (LoadS(base, model + 5052) == 0) {
      const uint32_t car = Load(base, Load(base, model + 5048) + 24);
      left = WrappedValue(base, 0x820D4048, car,
                          [&] { return load_poll::CarModelIsReady(base, car); });
      Store(base, model + 5052, uint32_t(left.r3));
    }
    if (LoadS(base, Load(base, Load(base, model + 5048) + 24) + 4088) == 3) {
      Store(base, model + 5056, 1);
    }
    finish(5064, 5060, 0x820E2E68,
           [&](uint32_t o) { return load_poll::ScenePartsFinish(base, o, frame, poll); });
    finish(5072, 5068, 0x820E4140,
           [&](uint32_t o) { return load_poll::SceneInteriorFinish(base, o, frame, poll); });
    finish(5080, 5076, 0x820DBFF8,
           [&](uint32_t o) { return load_poll::SceneCockpitFinish(base, o, frame, poll); });
    finish(5088, 5084, 0x820DDA60,
           [&](uint32_t o) { return load_poll::SceneLightsFinish(base, o, frame, poll); });
    finish(5104, 5100, 0x820E7328,
           [&](uint32_t o) { return load_poll::SceneWheelsFinish(base, o, frame, poll); });
    const bool race = LoadS(base, Load(base, model + 5116) + 4) != 0;
    auto set = [&](uint32_t at) { return LoadS(base, model + at) != 0; };
    if (set(5052) && set(5064) && set(5072) && set(5056) && (!race || set(5080)) &&
        set(5104) && set(5088)) {
      Store(base, model + 5036, 1);
    }
    if (!set(5036)) {
      return left;
    }
    left = WrappedValue(base, 0x820D9EA0, model_r3, [&] {
      return load_poll::SceneModelApplyPalettes(base, model, model_r3, frame, poll);
    });
    if (set(5104)) {
      const uint32_t wheels = Load(base, model + 5100);
      left = WrappedValue(base, 0x820E8908, wheels, [&] {
        return load_poll::SceneWheelsSetUpPoints(base, wheels, wheels, frame, poll);
      });
    }
    const uint32_t first = Load(base, Load(base, model + 5060) + 104);
    left = WrappedValue(base, 0x820D72D8, first, [&] {
      return load_poll::CarPartsSetVinylTextures(base, first, first, frame);
    });
    left = WrappedValue(base, 0x820D9B68, model_r3, [&] {
      return load_poll::SceneModelApplyPaint(base, model, model_r3, frame);
    });
    if (set(5036) && LoadS(base, model + 32) != 3) {
      const uint32_t kit = Load(base, model + 5084);
      left = Left{false, r3_of(kit)};
      if (kit != 0) {
        left = WrappedValue(base, 0x820DCBC0, kit, [&] {
          return load_poll::SceneLightsApplyRecord(base, kit, model + 4224, frame, poll);
        });
      }
    }
    left = WrappedValue(base, 0x820D9C50, model_r3, [&] {
      return load_poll::SceneModelApplyWheelPaint(base, model, model_r3, frame, poll);
    });
  }

  // The second wheels, once claimed (+5032) and finished: the old ones
  // released, the new ones set up and painted.
  if (LoadS(base, model + 5032) != 0) {
    const uint32_t next = Load(base, model + 5108);
    left = WrappedValue(base, 0x820E7328, next, [&] {
      return load_poll::SceneWheelsFinish(base, next, frame, poll);
    });
    Store(base, model + 5112, uint32_t(left.r3));
    if (int32_t(left.r3) != 0) {
      const uint32_t set = Load(base, model + 5116);
      const uint32_t index = Load(base, Load(base, model + 5100) + 20);
      left = WrappedValue(base, 0x820C5C88, set, [&] {
        return second_set::ReleaseB(base, set, 5, index, frame, 0x820DB01C, release);
      });
      const uint32_t wheels = Load(base, model + 5108);
      Store(base, model + 5032, 0);
      Store(base, model + 5112, 0);
      Store(base, model + 5108, 0);
      Store(base, model + 5100, wheels);
      left = WrappedValue(base, 0x820E8908, wheels, [&] {
        return load_poll::SceneWheelsSetUpPoints(base, wheels, wheels, frame, poll);
      });
      const uint32_t first = Load(base, Load(base, model + 5100) + 28);
      left = WrappedValue(base, 0x820D92A0, first, [&] {
        return load_poll::WheelsApplyPalette(base, first, frame, poll);
      });
      left = WrappedValue(base, 0x820D9C50, model_r3, [&] {
        return load_poll::SceneModelApplyWheelPaint(base, model, model_r3, frame, poll);
      });
    }
  }

  // The cockpit view's model: only the cockpit, once finished.
  if (LoadS(base, model + 32) == 2 && Load(base, model + 4300) != 0) {
    const uint32_t cockpit = Load(base, model + 5076);
    left = Left{false, r3_of(cockpit)};
    if (cockpit == 0) {
      return left;
    }
    if (LoadS(base, model + 5080) == 0) {
      left = WrappedValue(base, 0x820DBFF8, cockpit, [&] {
        return load_poll::SceneCockpitFinish(base, cockpit, frame, poll);
      });
      Store(base, model + 5080, uint32_t(left.r3));
    }
    if (LoadS(base, model + 5080) == 0) {
      return left;
    }
    const uint32_t drawn = Load(base, model + 5076);
    return Wrapped(base, 0x820DC498, drawn, [&] {
      return SceneCockpitDraw(base, calls, frame, drawn, block, uint32_t(caller_r24),
                              kDrawsR28);
    });
  }

  // The light kit under the scene's key (+5124) when its camera pass is
  // enabled, the render queue's two keys saved in the scene and put back.
  {
    const uint32_t scene = Load(base, kScene);
    if (EnabledPass(base, Load(base, scene + 5084)) != 0) {
      const uint32_t key = Load(base, scene + 5124);
      Store(base, scene + 5136, Load(base, kKeyA));
      Store(base, scene + 5140, Load(base, kKeyB));
      Store(base, kKeyA, key);
      Store(base, kKeyB, key);
      const uint32_t kit = Load(base, model + 5084);
      left = Wrapped(base, 0x820DDE58, kit, [&] {
        return SceneLightsDraw(base, calls, frame, kit, block, advance_blink,
                               uint32_t(caller_r24), kDrawsR28);
      });
      const uint32_t now = Load(base, kScene);
      const uint32_t b = Load(base, now + 5140), a = Load(base, now + 5136);
      Store(base, kKeyB, b);
      Store(base, kKeyA, a);
    }
  }
  const uint32_t parts = Load(base, model + 5060);
  left = Wrapped(base, 0x820E2EE8, parts, [&] {
    return ScenePartsDraw(base, calls, frame, parts, block, uint32_t(caller_r24));
  });

  // The per-object light slots 2 to 5: colors and directions 0, the ambient
  // (0, 0, 1, 0), the +240 block's the scene's lighting +176, through the
  // frame's +96 to +159.
  {
    const uint32_t lighting = Load(base, kScene) + 5248;
    for (uint32_t at = 96; at < 152; at += 4) {
      guest::StoreFloat(base, frame + at, zero);
    }
    guest::StoreFloat(base, frame + 152, float(one));
    guest::StoreFloat(base, frame + 156, zero);
    for (uint32_t i = 32; i < 96; i += 16) {
      CopyDoubleword(base, kSlotsColorsA + i, frame + 96);
      CopyDoubleword(base, kSlotsColorsB + i, frame + 112);
      CopyDoubleword(base, kSlotsDirections + i, frame + 128);
      CopyDoubleword(base, kSlotsColorsB + i + 8, frame + 120);
      CopyDoubleword(base, kSlotsDirections + i + 8, frame + 136);
      CopyDoubleword(base, kSlotsColorsA + i + 8, frame + 104);
      CopyDoubleword(base, kSlots240 + i, lighting + 176);
      CopyDoubleword(base, kSlots240 + i + 8, lighting + 184);
      CopyDoubleword(base, kSlotsAmbient + i, frame + 144);
      CopyDoubleword(base, kSlotsAmbient + i + 8, frame + 152);
    }
  }
  const uint32_t wheels = Load(base, model + 5100);
  left = Wrapped(base, 0x820ECE28, wheels, [&] {
    return SceneWheelsDraw(base, calls, frame, wheels, block, kWheelsR24);
  });
  const uint32_t interior = Load(base, model + 5068);
  left = Wrapped(base, 0x820E53F0, interior, [&] {
    return SceneInteriorDraw(base, calls, frame, interior, block, uint32_t(kWheelsR24),
                             uint32_t(kInteriorR28));
  });
  return left;
}

}  // namespace car_draw
