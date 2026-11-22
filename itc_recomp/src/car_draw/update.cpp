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
#include "schema/cameras.h"
#include "schema/car_draw.h"
#include "schema/car_models.h"
#include "schema/car_scene_draw.h"
#include "schema/load_poll.h"
#include "schema/scene_models.h"
#include "schema/second_set.h"
#include "second_set/objects.h"

#include "base/float_mode.h"

#include <cstring>

namespace car_draw {

namespace {

using Fpscr = ::base::FloatMode;

using G = schema::CarDrawGlobals;
using Scene = schema::CarScene;

constexpr uint32_t kCameraPasses = schema::CamerasGlobals::passes_address();
constexpr uint32_t kOne = 0x82000A30;    // 1.0
constexpr uint32_t kZero = 0x82000A34;   // 0.0
constexpr uint32_t kBehind = 0x82002A24;  // the cull's least w
// The scene lights' per-object slots (2 to 5 cleared before the wheels).
constexpr uint32_t kSlotsAmbient = schema::CarSceneDrawGlobals::headlight_shapes_address();
constexpr uint32_t kSlotsColorsA = schema::CarSceneDrawGlobals::headlight_colors_a_address();
constexpr uint32_t kSlotsColorsB = schema::CarSceneDrawGlobals::headlight_colors_b_address();
constexpr uint32_t kSlotsDirections = schema::CarSceneDrawGlobals::headlight_directions_address();
constexpr uint32_t kSlots240 = schema::CarSceneDrawGlobals::headlight_vectors_address();
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
  const uint32_t pass = kCameraPasses + index * schema::CameraPass::kSize;
  if (pass == 0 || int32_t(schema::CameraPass(base, pass).enabled()) == 0) {
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
  const schema::SceneModel m(base, model);
  const uint32_t frame = stack - kCarDrawUpdateFrame;
  Store(base, frame, stack);  // its back chain, as its stwu
  Left left{false, model_r3};
  auto r3_of = [&](uint32_t value) { return uint64_t(value); };

  // The boost (the draw block's): 1.0 off.
  const double one = double(guest::LoadFloat(base, kOne));
  const schema::CarDrawBlock draw(base, m.draw_block_address());
  draw.set_boost(float(double(draw.boost()) - one));
  if (int32_t(m.draw_enabled()) == 0 || int32_t(m.release_pending()) != 0) {
    return left;
  }
  const float zero = guest::LoadFloat(base, kZero);
  const uint32_t block = m.draw_block_address();

  // In a race: the model's position through the current camera pass's
  // view-projection; behind the camera, nothing more; then the height cull.
  if (int32_t(m.scene().model_set()) != 0) {
    const Scene scene = G::scene(base);
    const uint32_t d = scene.pass_camera_address();  // the pass description
    const uint32_t position = frame + 80, projected = frame + 160;
    guest::StoreFloat(base, frame + 80, zero);
    guest::StoreFloat(base, frame + 84, zero);
    guest::StoreFloat(base, frame + 88, zero);
    guest::StoreFloat(base, frame + 92, float(one));
    guest::StoreFloat(base, frame + 172, float(one));
    debug::Vec4Transform(base, position, m.matrix_address(), position);
    const uint32_t pass = EnabledPass(base, scene.pass_camera());
    debug::Vec4Transform(base, projected, schema::CameraPass(base, pass).view_projection_address(), position);
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
      scene.set_pass_mask(scene.pass_mask() & 0xFFFFFF07u);
    }
  }

  // Traffic: its body's and wheels' finishers until both are done, the
  // apply steps on the call that finds them done, then their draws.
  if (int32_t(m.kind()) == 3) {
    if (int32_t(m.ready()) == 0) {
      if (int32_t(m.traffic_body_ready()) == 0) {
        const uint32_t body = m.traffic_body();
        left = WrappedValue(base, 0x820E5A68, body, [&] {
          return load_poll::SceneTrafficFinish(base, body, frame, poll);
        });
        m.set_traffic_body_ready(uint32_t(left.r3));
        if (int32_t(left.r3) != 0) {
          const schema::SceneTraffic traffic(base, m.traffic_body());
          const schema::SceneWheels wheels(base, m.wheels());
          wheels.set_tire_positions(traffic.body() + 7072);
        }
      }
      if (int32_t(m.wheels_ready()) == 0) {
        const uint32_t wheels = m.wheels();
        left = WrappedValue(base, 0x820E7328, wheels, [&] {
          return load_poll::SceneWheelsFinish(base, wheels, frame, poll);
        });
        m.set_wheels_ready(uint32_t(left.r3));
      }
      if (int32_t(m.traffic_body_ready()) != 0 && int32_t(m.wheels_ready()) != 0) {
        m.set_ready(1);
      }
      if (int32_t(m.ready()) == 0) {
        return left;
      }
      if (int32_t(m.wheels_ready()) != 0) {
        const uint32_t wheels = m.wheels();
        left = WrappedValue(base, 0x820E8908, wheels, [&] {
          return load_poll::SceneWheelsSetUpPoints(base, wheels, wheels, frame, poll);
        });
      }
      left = WrappedValue(base, 0x820D9EA0, model_r3, [&] {
        return load_poll::SceneModelApplyPalettes(base, model, model_r3, frame, poll);
      });
    }
    const uint32_t body = m.traffic_body();
    left = Wrapped(base, 0x820E6A40, body, [&] {
      return SceneTrafficDraw(base, calls, frame, body, block, uint32_t(caller_r24));
    });
    const uint32_t wheels = m.wheels();
    left = Wrapped(base, 0x820ECE28, wheels, [&] {
      return SceneWheelsDraw(base, calls, frame, wheels, block, caller_r24);
    });
    return left;
  }

  // Cars, until ready: each finisher whose flag is 0, its answer the flag;
  // ready once all are set (the cockpit's only in a race); on the call it
  // becomes ready, the apply steps once. Not ready: nothing more.
  if (int32_t(m.ready()) == 0) {
    // `flag` and `object` the scene model's fields' addresses.
    auto finish = [&](uint32_t flag, uint32_t object_at, uint32_t function, auto step) {
      if (LoadS(base, flag) != 0) {
        return;
      }
      const uint32_t object = Load(base, object_at);
      left = WrappedValue(base, function, object, [&] { return step(object); });
      Store(base, flag, uint32_t(left.r3));
    };
    if (int32_t(m.car_model_ready()) == 0) {
      const uint32_t car = m.car_model().car_model().address;
      left = WrappedValue(base, 0x820D4048, car,
                          [&] { return load_poll::CarModelIsReady(base, car); });
      m.set_car_model_ready(uint32_t(left.r3));
    }
    if (int32_t(m.car_model().car_model().decals_state()) == 3) {
      m.set_decals_reloaded(1);
    }
    finish(m.parts_ready_address(), m.parts_address(), 0x820E2E68,
           [&](uint32_t o) { return load_poll::ScenePartsFinish(base, o, frame, poll); });
    finish(m.interior_ready_address(), m.interior_address(), 0x820E4140,
           [&](uint32_t o) { return load_poll::SceneInteriorFinish(base, o, frame, poll); });
    finish(m.cockpit_ready_address(), m.cockpit_address(), 0x820DBFF8,
           [&](uint32_t o) { return load_poll::SceneCockpitFinish(base, o, frame, poll); });
    finish(m.light_kit_ready_address(), m.light_kit_address(), 0x820DDA60,
           [&](uint32_t o) { return load_poll::SceneLightsFinish(base, o, frame, poll); });
    finish(m.wheels_ready_address(), m.wheels_address(), 0x820E7328,
           [&](uint32_t o) { return load_poll::SceneWheelsFinish(base, o, frame, poll); });
    const bool race = int32_t(m.scene().model_set()) != 0;
    auto set = [&](uint32_t at) { return LoadS(base, at) != 0; };
    if (set(m.car_model_ready_address()) && set(m.parts_ready_address()) &&
        set(m.interior_ready_address()) && set(m.decals_reloaded_address()) &&
        (!race || set(m.cockpit_ready_address())) && set(m.wheels_ready_address()) &&
        set(m.light_kit_ready_address())) {
      m.set_ready(1);
    }
    if (!set(m.ready_address())) {
      return left;
    }
    left = WrappedValue(base, 0x820D9EA0, model_r3, [&] {
      return load_poll::SceneModelApplyPalettes(base, model, model_r3, frame, poll);
    });
    if (set(m.wheels_ready_address())) {
      const uint32_t wheels = m.wheels();
      left = WrappedValue(base, 0x820E8908, wheels, [&] {
        return load_poll::SceneWheelsSetUpPoints(base, wheels, wheels, frame, poll);
      });
    }
    const uint32_t first = m.parts().first_set_parts();
    left = WrappedValue(base, 0x820D72D8, first, [&] {
      return load_poll::CarPartsSetVinylTextures(base, first, first, frame);
    });
    left = WrappedValue(base, 0x820D9B68, model_r3, [&] {
      return load_poll::SceneModelApplyPaint(base, model, model_r3, frame);
    });
    if (set(m.ready_address()) && int32_t(m.kind()) != 3) {
      const uint32_t kit = m.light_kit().address;
      left = Left{false, r3_of(kit)};
      if (kit != 0) {
        left = WrappedValue(base, 0x820DCBC0, kit, [&] {
          return load_poll::SceneLightsApplyRecord(base, kit, m.light_kit_record_address(), frame, poll);
        });
      }
    }
    left = WrappedValue(base, 0x820D9C50, model_r3, [&] {
      return load_poll::SceneModelApplyWheelPaint(base, model, model_r3, frame, poll);
    });
  }

  // The second wheels, once claimed (+5032) and finished: the old ones
  // released, the new ones set up and painted.
  if (int32_t(m.second_wheels_claimed()) != 0) {
    const uint32_t next = m.second_wheels();
    left = WrappedValue(base, 0x820E7328, next, [&] {
      return load_poll::SceneWheelsFinish(base, next, frame, poll);
    });
    m.set_second_wheels_ready(uint32_t(left.r3));
    if (int32_t(left.r3) != 0) {
      const uint32_t set = m.scene().address;
      const uint32_t index = schema::SceneWheels(base, m.wheels()).index();
      left = WrappedValue(base, 0x820C5C88, set, [&] {
        return second_set::ReleaseB(base, set, 5, index, frame, 0x820DB01C, release);
      });
      const uint32_t wheels = m.second_wheels();
      m.set_second_wheels_claimed(0);
      m.set_second_wheels_ready(0);
      m.set_second_wheels(0);
      m.set_wheels(wheels);
      left = WrappedValue(base, 0x820E8908, wheels, [&] {
        return load_poll::SceneWheelsSetUpPoints(base, wheels, wheels, frame, poll);
      });
      const uint32_t first = schema::SceneWheels(base, m.wheels()).first_set_wheels();
      left = WrappedValue(base, 0x820D92A0, first, [&] {
        return load_poll::WheelsApplyPalette(base, first, frame, poll);
      });
      left = WrappedValue(base, 0x820D9C50, model_r3, [&] {
        return load_poll::SceneModelApplyWheelPaint(base, model, model_r3, frame, poll);
      });
    }
  }

  // The cockpit view's model: only the cockpit, once finished.
  if (int32_t(m.kind()) == 2 && m.cockpit_only() != 0) {
    const uint32_t cockpit = m.cockpit();
    left = Left{false, r3_of(cockpit)};
    if (cockpit == 0) {
      return left;
    }
    if (int32_t(m.cockpit_ready()) == 0) {
      left = WrappedValue(base, 0x820DBFF8, cockpit, [&] {
        return load_poll::SceneCockpitFinish(base, cockpit, frame, poll);
      });
      m.set_cockpit_ready(uint32_t(left.r3));
    }
    if (int32_t(m.cockpit_ready()) == 0) {
      return left;
    }
    const uint32_t drawn = m.cockpit();
    return Wrapped(base, 0x820DC498, drawn, [&] {
      return SceneCockpitDraw(base, calls, frame, drawn, block, uint32_t(caller_r24),
                              kDrawsR28);
    });
  }

  // The light kit under the scene's key (+5124) when its camera pass is
  // enabled, the render queue's two keys saved in the scene and put back.
  {
    const Scene scene = G::scene(base);
    if (EnabledPass(base, scene.pass_light_kit_camera()) != 0) {
      const uint32_t key = scene.pass_light_kit_key();
      scene.set_pass_saved_key_group0(G::key_group0(base));
      scene.set_pass_saved_key_group1(G::key_group1(base));
      G::set_key_group0(base, key);
      G::set_key_group1(base, key);
      const uint32_t kit = m.light_kit().address;
      left = Wrapped(base, 0x820DDE58, kit, [&] {
        return SceneLightsDraw(base, calls, frame, kit, block, advance_blink,
                               uint32_t(caller_r24), kDrawsR28);
      });
      const Scene now = G::scene(base);
      const uint32_t b = now.pass_saved_key_group1(), a = now.pass_saved_key_group0();
      G::set_key_group1(base, b);
      G::set_key_group0(base, a);
    }
  }
  const uint32_t parts = m.parts().address;
  left = Wrapped(base, 0x820E2EE8, parts, [&] {
    return ScenePartsDraw(base, calls, frame, parts, block, uint32_t(caller_r24));
  });

  // The per-object light slots 2 to 5: colors and directions 0, the ambient
  // (0, 0, 1, 0), the +240 block's the scene's lighting +176, through the
  // frame's +96 to +159.
  {
    const Scene lighting = G::scene(base);
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
      CopyDoubleword(base, kSlots240 + i, lighting.light_slot_vector_lo_address());
      CopyDoubleword(base, kSlots240 + i + 8, lighting.light_slot_vector_hi_address());
      CopyDoubleword(base, kSlotsAmbient + i, frame + 144);
      CopyDoubleword(base, kSlotsAmbient + i + 8, frame + 152);
    }
  }
  const uint32_t wheels = m.wheels();
  left = Wrapped(base, 0x820ECE28, wheels, [&] {
    return SceneWheelsDraw(base, calls, frame, wheels, block, kWheelsR24);
  });
  const uint32_t interior = m.interior();
  left = Wrapped(base, 0x820E53F0, interior, [&] {
    return SceneInteriorDraw(base, calls, frame, interior, block, uint32_t(kWheelsR24),
                             uint32_t(kInteriorR28));
  });
  return left;
}

}  // namespace car_draw
