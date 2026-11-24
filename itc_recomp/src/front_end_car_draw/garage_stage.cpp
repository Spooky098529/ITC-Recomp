// The garage stage's small helpers and commands
// (docs/functions/front-end-car-draw.md), step for step as the translated
// code (see front_end_car_draw.h).

#include "front_end_car_draw/front_end_car_draw.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "debug/frame_state_diff.h"
#include "car_scene_draw/ops.h"
#include "guest.h"
#include "load_poll/load_poll.h"
#include "menu_sounds/menu_sounds.h"
#include "native/buffers.h"
#include "schema/cameras.h"
#include "schema/car_draw.h"
#include "schema/front_end_car_draw.h"
#include "schema/load_poll.h"
#include "schema/scene_models.h"
#include "schema/stand_models.h"
#include "schema/type6_type7_modes.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace front_end_car_draw {

namespace {

using namespace car_scene_draw::ops;

using G = schema::FrontEndCarDrawGlobals;
using Entry = schema::GarageAnimEntry;
using List = schema::GarageAnimList;
using Values = schema::GarageAnimValues;
using Slot = schema::StandSlot;
using Stage = schema::GarageStage;
using Model = schema::SceneModel;
using Camera = schema::GarageCamera;
using Preset = schema::CameraPreset;
using Kit = schema::LightKitRecord;

constexpr uint32_t kOpenCue = 15;             // MenuSound::Play's cue for the opening

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kTan = 0x825A8A50;
constexpr uint32_t kApplyRecord = 0x820DCBC0;
constexpr uint32_t kDrawShape = 0x82596B18;
constexpr uint32_t kOpenDoors = 0x8217B6B8;
constexpr uint32_t kCloseDoors = 0x8217B740;

int32_t LoadS(uint8_t* base, uint32_t address) { return int32_t(Load(base, address)); }

// The frame `bytes` below `stack`, its back chain written (stwu's): kept
// where a guest call is made with it (E5 step 6).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// The frame's address alone, for a function whose frame holds nothing and
// makes no guest call with it (its callees' frames below it; E5 step 6).
constexpr uint32_t FrameAt(uint32_t stack, uint32_t bytes) { return stack - bytes; }

// The car scene ([0x826C2E44]).
uint32_t Scene(uint8_t* base) { return schema::CarDrawGlobals::scene(base).address; }

// The stage's scene model: ScenePools::GetB([0x826C2E44], 7, [[holder] + 8]).
uint32_t StageModel(uint8_t* base, uint32_t holder) {
  return vehicle_accessors::ScenePoolsGetB(base, Scene(base), 7,
                                           Slot(base, holder).stage().model_index());
}

// A yaw or pitch wrapped: below -π to π, above π to -π (NaN kept).
double Wrap(uint8_t* base, double v) {
  const double minus_pi = double(G::minus_pi(base));
  if (v < minus_pi) {
    return double(G::pi(base));
  }
  if (v > double(G::pi(base))) {
    return minus_pi;
  }
  return v;
}

// MenuSound::Play's plain C++ from the frame at `sp`, its slot call through
// `calls`.
uint64_t PlayMenuSound(uint8_t* base, const Calls& calls, uint32_t cue, uint32_t sp) {
  const menu_sounds::Calls sounds{
      const_cast<Calls*>(&calls),
      [](void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
         uint32_t lr) -> uint64_t {
        return static_cast<Calls*>(user)->call(function, args, set, nullptr, 0, r1, lr,
                                               nullptr);
      }};
  return menu_sounds::PlayMenuSound(base, cue, sp, sounds);
}

}  // namespace

uint64_t Memcpy(uint8_t* base, const Calls& calls, uint32_t to, uint32_t from, uint32_t bytes,
                uint32_t sp, uint32_t lr) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    const uint64_t regs[3] = {to, from, bytes};
    return calls.call(kMemcpy, regs, 3, nullptr, 0, sp, lr, nullptr);
  }
  return to;
}

void PlatesHide(uint8_t* base, uint32_t points) {
  ITC_NATIVE_RUNNING("sub_820DF190 Plates::Hide");
  const schema::FrontEndPlatePoints p(base, points);
  p.set_front_plate_set(0);
  p.set_rear_plate_set(0);
}

void PlatesShow(uint8_t* base, uint32_t points) {
  ITC_NATIVE_RUNNING("sub_820DF180 Plates::Show");
  const schema::FrontEndPlatePoints p(base, points);
  p.set_front_plate_set(1);
  p.set_rear_plate_set(1);
}

uint32_t GarageStageIsBusy(uint8_t* base, const Calls& calls, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179BE0 GarageStage::IsBusy");
  const uint32_t frame = FrameAt(stack, 96);
  const uint32_t model = StageModel(base, holder);
  const uint64_t loaded = load_poll::SceneModelPollLoad(base, model, frame, *calls.poll);
  if (int32_t(loaded) == 0) {
    return 1;
  }
  return int32_t(Slot(base, holder).stage().pending_block()) == -1 ? 0 : 1;
}

uint32_t GarageStageSyncPlates(uint8_t* base, const Calls& calls, uint32_t holder,
                               uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217AFE8 GarageStage::SyncPlates");
  const uint32_t frame = FrameAt(stack, 112);
  const uint32_t model = StageModel(base, holder);
  const uint32_t busy = GarageStageIsBusy(base, calls, holder, frame);
  if (busy == 1) {
    return busy;
  }
  const schema::SceneParts parts = Model(base, model).parts();
  const uint32_t want = Slot(base, holder).stage().plates_shown();
  const uint32_t points = parts.points_address();
  if (want == schema::PartsPoints(base, points).front_plate_set()) {
    return busy;
  }
  if (want == 1) {
    PlatesShow(base, points);
  } else {
    PlatesHide(base, points);
  }
  return points;
}

uint32_t GarageStageSetDrawWord(uint8_t* base, uint32_t holder, uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178B40 GarageStage::SetDrawWord");
  (void)stack;  // no frame (E5 step 6: the original's held nothing)
  const uint32_t model = StageModel(base, holder);
  Slot(base, holder).stage().set_draw_word(word);
  Model(base, model).set_draw_enabled(word);
  return model;
}

void GarageStageStepAngles(uint8_t* base, uint32_t command, uint32_t angles) {
  ITC_NATIVE_RUNNING("sub_82179C88 GarageStage::StepAngles");
  const Values a(base, angles);
  const int32_t c = int32_t(command);
  switch (c) {
    case 9:
    case 10: {
      const double step = double(G::half_degree(base));
      const double yaw = double(a.yaw());
      const double v = c == 9 ? Fadds(yaw, step) : Fsubs(yaw, step);
      a.set_yaw(float(Wrap(base, v)));
      return;
    }
    case 11:
    case 12: {
      const double step = double(G::half_degree(base));
      const double angle = double(a.angle_z());
      const double v = c == 11 ? Fadds(angle, step) : Fsubs(angle, step);
      a.set_angle_z(float(Wrap(base, v)));
      return;
    }
    case 13: {
      const double zero = double(G::zero(base));
      a.set_angle_x(float(zero));
      a.set_angle_z(float(zero));
      a.set_yaw(float(zero));
      return;
    }
    default:
      return;
  }
}

uint64_t GarageAnimsAdd(uint8_t* base, const Calls& calls, uint32_t list, uint32_t type,
                        uint32_t value, uint32_t index, uint32_t word, double steps,
                        uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217F9E0 GarageAnims::Add");
  const uint32_t f = Frame(base, stack, 656);
  const uint32_t entry = f + 80;   // the entry looked at (28 bytes)
  const uint32_t kept = f + 112;   // type 1's kept entries
  const uint32_t kept_count = f + 560;
  const List l(base, list);
  uint64_t r3 = list;
  auto copy = [&](uint32_t to, uint32_t from, uint32_t lr) {
    r3 = Memcpy(base, calls, to, from, 28, f, lr);
  };

  if (int32_t(type) == 1) {
    // Popped from the end; the non-turns kept (in reverse), the earliest of
    // them put back alone.
    // n is the original's r28: decremented from its register, the loop's
    // test reloading the count.
    uint32_t n = l.count();
    uint32_t count = 0;
    if (n != 0) {
      do {
        n = n - 1;
        l.set_count(n);
        const uint32_t e = list + 28 * n;
        copy(entry, e, 0x8217FA48);
        if (LoadS(base, entry) != 0) {
          Store(base, kept_count, count + 1);
          copy(kept + 28 * count, e, 0x8217FA78);
          count = Load(base, kept_count);
        }
      } while (l.count() != 0);
    }
    l.set_count(0);
    if (count == 0) {
      return r3;
    }
    Store(base, kept_count, count - 1);
    l.set_count(1);
    copy(list, kept + 28 * (count - 1), 0x8217FBC0);
    return r3;
  }

  const uint32_t count = l.count();
  // An entry of the same type: nothing.
  if (count != 0) {
    uint32_t i = 0, e = list;
    do {
      copy(entry, e, 0x8217FAD8);
      if (LoadS(base, entry) == int32_t(type)) {
        return r3;
      }
      i++;
      e += 28;
    } while (i < l.count());
  }
  // One of the opposite type: overwritten in place.
  if (count != 0) {
    uint32_t i = 0, e = list;
    do {
      const Entry en(base, e);
      const int32_t t = en.type();
      const int32_t y = int32_t(type);
      if ((t == 2 && y == 3) || (t == 3 && y == 2) || (t == 4 && y == 5) || (t == 5 && y == 4)) {
        en.set_steps(float(steps));
        en.set_type(int32_t(type));
        en.set_value(value);
        en.set_word(word);
        en.set_done(0);
        return r3;
      }
      i++;
      e += 28;
    } while (i < l.count());
  }
  // A new entry at the end.
  Stfs(base, entry + 4, steps);
  Store(base, entry + 0, type);
  Store(base, entry + 12, value);
  Store(base, entry + 8, word);
  Store(base, entry + 20, 0);
  const double zero = double(G::zero(base));
  Store(base, entry + 16, index);
  Stfs(base, entry + 24, zero);
  if (int32_t(type) == 6) {
    // Type 6's value is the garage camera (the stage's +112).
    const double angle = Fsubs(double(G::three_sixty(base)), double(Camera(base, value).orbit()));
    Stfs(base, entry + 24, Fdivs(angle, steps));
  }
  l.set_count(count + 1);
  copy(list + 28 * count, entry, 0x8217FBC0);
  return r3;
}

uint64_t GarageStageApplyPreset(uint8_t* base, const Calls& calls, uint64_t r3, uint32_t preset,
                                uint32_t stage, uint32_t index, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217D2B0 GarageStage::ApplyPreset");
  const uint32_t f = Frame(base, stack, 128);
  const Stage st(base, stage);
  const Camera camera(base, st.camera_address());
  if (camera.set_up() == 0) {
    st.set_waiting_preset(preset);
    return r3;
  }
  const uint32_t model = vehicle_accessors::ScenePoolsGetB(base, Scene(base), 7, index);
  const uint64_t slot = uint64_t(Model(base, model).block()) * 290 + preset;
  uint32_t e = Load(base, camera.preset_table() + uint32_t(slot << 2));
  if (e >= camera.preset_count()) {
    e = 0xFFFFFFFFu;
  }
  // The presets' pointers (the camera's +296), indexed from the camera as
  // the original does ((e + 74) << 2).
  const Preset p(base, e == 0xFFFFFFFFu ? 0 : Load(base, camera.address + ((e + 74) << 2)));
  camera.set_tilt(float(double(p.elevation())));
  camera.set_distance(float(double(p.value_8())));
  camera.set_fov(float(double(p.value_12())));
  const double x = Fmuls(double(p.value_12()), double(G::degree(base)));
  // tan as its adapter gives it: nearest rounding (its _ctrlfp) and r3 its
  // second _ctrlfp's word (0 ^ 0xF8); else the call.
  double t;
  if ((Fpscr::getcsr() & Fpscr::RoundMaskVal) == 0 && c_runtime::Tan(x, &t)) {
    r3 = 0xF8;
  } else {
    r3 = calls.call(kTan, nullptr, 0, &x, 1, f, 0x8217D368, &t);
  }
  camera.set_zoom(float(Fdivs(double(G::focal(base)), Frsp(t))));
  // The preset's vectors as doublewords (ld, std): +32 to the camera's
  // translation, +16 to its angles, +48 to the car's angles.
  // The preset is a host object (E5 step 4): its words through the view.
  const uint32_t translation = camera.translation_address(), angles = camera.angles_address();
  const uint32_t w0 = p.vector_32_x_bits(), w1 = p.vector_32_y_bits();
  Store(base, translation, w0);
  Store(base, translation + 4, w1);
  const uint32_t w2 = p.vector_32_z_bits(), w3 = p.vector_32_w_bits();
  Store(base, translation + 8, w2);
  Store(base, translation + 12, w3);
  const uint32_t w4 = p.vector_16_x_bits(), w5 = p.vector_16_y_bits();
  Store(base, angles, w4);
  Store(base, angles + 4, w5);
  const uint32_t w6 = p.vector_16_z_bits(), w7 = p.vector_16_w_bits();
  Store(base, angles + 8, w6);
  Store(base, angles + 12, w7);
  const uint32_t car = st.angle_x_address();
  const uint32_t w8 = p.angles_x_bits(), w9 = p.angles_y_bits();
  Store(base, car, w8);
  Store(base, car + 4, w9);
  const uint32_t w10 = p.angles_z_bits(), w11 = p.angles_w_bits();
  Store(base, car + 8, w10);
  Store(base, car + 12, w11);
  st.set_waiting_preset(0xFFFFFFFFu);
  return r3;
}

uint64_t GarageStageCommand(uint8_t* base, const Calls& calls, uint64_t r3, uint32_t command,
                            uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82180108 GarageStage::Command");
  const uint32_t f = FrameAt(stack, 112);
  const uint32_t holder = uint32_t(r3);
  const int32_t c = int32_t(command);
  const Slot slot(base, holder);
  // `camera`: the value the garage camera (stage + 112), else the stage's
  // values (GarageAnimValues, stage + 32).
  auto add = [&](uint32_t type, bool camera, bool value_zero, uint32_t word, uint32_t steps_at) {
    const Stage s = slot.stage();
    const uint32_t value = value_zero ? 0 : camera ? s.camera_address() : s.bonnet_address();
    return GarageAnimsAdd(base, calls, s.animations_address(), type, value, s.model_index(), word,
                          Lfs(base, steps_at), f);
  };
  if (c > 5) {
    if (c == 6 || c == 7) {
      return add(uint32_t(c), true, false, 0, G::twenty_address());
    }
    if (c == 8) {
      const List list(base, slot.stage().animations_address());
      if (list.count() != 0) {
        list.set_count(0);
      }
      list.set_count(0);
      add(6, true, false, 0, G::twenty_address());
      return add(7, false, false, 0, G::twenty_address());
    }
    if (c < 8) {
      return r3;
    }
    if (c <= 13) {
      const Stage s = slot.stage();
      GarageStageStepAngles(base, command, s.bonnet_address());
      return s.animations_address();
    }
    if (c > 64) {
      return r3;
    }
    const Stage s = slot.stage();
    return GarageStageApplyPreset(base, calls, s.animations_address(), command - 14, s.address,
                                  s.model_index(), f);
  }
  switch (command) {
    case 0:
      return add(0, false, false, 1, G::sixty_address());
    case 1:
      add(1, false, true, 1, G::sixty_address());
      return add(1, false, false, 0, G::sixty_address());
    case 2:
    case 3:
    case 4:
    case 5:
      return add(command, false, false, 0, G::sixty_address());
    default:
      return r3;  // negative: nothing
  }
}

uint32_t GarageAnimsTurn(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82179D78 GarageAnims::Turn");
  const Entry en(base, entry);
  const Values values = en.value();
  const double yaw = Wrap(base, Fsubs(double(values.yaw()), double(G::half_degree(base))));
  values.set_yaw(float(yaw));
  if (int32_t(en.word()) != 0) {
    return 0;
  }
  if (yaw != double(G::zero(base))) {
    return 0;
  }
  en.set_done(1);
  return 1;
}

uint32_t GarageAnimsOpen(uint8_t* base, const Calls& calls, uint32_t entry, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179DF0 GarageAnims::Open");
  const uint32_t f = FrameAt(stack, 128);
  const Entry en(base, entry);
  const uint32_t model = vehicle_accessors::ScenePoolsGetB(base, Scene(base), 7, en.model_index());
  const uint32_t car = Model(base, model).block();
  const double steps = double(en.steps());
  const Values value = en.value();
  const double now = double(value.bonnet());
  const double max = Lfs(base, G::car_limits_address() + car * 48);
  const double step = Fdivs(max, steps);
  double v = Fadds(now, step);
  const double zero = double(G::zero(base));
  if (v < zero) {
    v = zero;
  } else if (v > max) {
    v = max;
  }
  value.set_bonnet(float(v));
  if (!(v > step)) {
    PlayMenuSound(base, calls, kOpenCue, f);
  }
  if (double(value.bonnet()) < max) {
    return 0;
  }
  en.set_done(1);
  value.set_bonnet(float(max));
  return 1;
}

uint32_t GarageAnimsClose(uint8_t* base, const Calls& calls, uint32_t entry, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179EA8 GarageAnims::Close");
  const uint32_t f = FrameAt(stack, 112);
  const Entry en(base, entry);
  const uint32_t model = vehicle_accessors::ScenePoolsGetB(base, Scene(base), 7, en.model_index());
  const uint32_t car = Model(base, model).block();
  const double steps = double(en.steps());
  const Values value = en.value();
  const double now = double(value.bonnet());
  const double max = Lfs(base, G::car_limits_address() + car * 48);
  const double v = Fsubs(now, Fdivs(max, steps));
  const double zero = double(G::zero(base));
  double stored = max;
  if (v < zero) {
    stored = zero;
  } else if (!(v > max)) {
    stored = v;
  }
  value.set_bonnet(float(stored));
  if (stored > zero) {
    return 0;
  }
  en.set_done(1);
  value.set_bonnet(float(zero));
  PlayMenuSound(base, calls, kOpenCue, f);
  return 1;
}

uint32_t GarageAnimsRun(uint8_t* base, const Calls& calls, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217D120 GarageAnims::Run");
  const uint32_t f = Frame(base, stack, 624);
  const List l(base, list);
  uint32_t any = 0;
  if (l.count() == 0) {
    return any;
  }
  uint32_t i = 0, e = list, count = 0;
  do {
    const int32_t type = Entry(base, e).type();
    bool ran = true;
    uint64_t r3 = 0;
    const uint64_t regs[2] = {list, e};
    switch (type) {
      case 0:
        r3 = GarageAnimsTurn(base, e);
        break;
      case 2:
        r3 = GarageAnimsOpen(base, calls, e, f);
        break;
      case 3:
        r3 = GarageAnimsClose(base, calls, e, f);
        break;
      case 4:
        r3 = calls.call(kOpenDoors, regs, 2, nullptr, 0, f, 0x8217D190, nullptr);
        break;
      case 5:
        r3 = calls.call(kCloseDoors, regs, 2, nullptr, 0, f, 0x8217D180, nullptr);
        break;
      default:
        ran = false;
        break;
    }
    if (ran && int32_t(r3) != 0) {
      any = 1;
    }
    i++;
    e += 28;
    count = l.count();
  } while (i < count);
  if (any == 0) {
    return any;
  }
  // The list rebuilt without the done ones: popped from the end into +80,
  // the others kept at +112 on (their count at +560), pushed back.
  const uint32_t entry = f + 80, kept = f + 112, kept_count = f + 560;
  uint32_t n = count, k = 0;
  if (n != 0) {
    do {
      n = n - 1;
      l.set_count(n);
      const uint32_t from = list + 28 * n;
      Memcpy(base, calls, entry, from, 28, f, 0x8217D21C);
      if (LoadS(base, entry + 20) == 0) {
        Store(base, kept_count, k + 1);
        Memcpy(base, calls, kept + 28 * k, from, 28, f, 0x8217D24C);
        k = Load(base, kept_count);
      }
    } while (l.count() != 0);
  }
  l.set_count(0);
  while (k != 0) {
    k = k - 1;
    const uint32_t at = l.count();
    Store(base, kept_count, k);
    l.set_count(at + 1);
    Memcpy(base, calls, list + 28 * at, kept + 28 * k, 28, f, 0x8217D298);
  }
  return any;
}

uint64_t SceneModelApplyLightKit(uint8_t* base, const Calls& calls, uint32_t model,
                                 uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9D10 SceneModel::ApplyLightKit");
  const Model m(base, model);
  if (int32_t(m.ready()) == 0 || int32_t(m.kind()) == 3) {
    return model;
  }
  const uint32_t kit = m.light_kit().address;
  if (kit == 0) {
    return 0;
  }
  // The tail call, inside its wrap (src/debug/frame_state_diff.cpp).
  if (debug::IsHidden(kApplyRecord)) {
    return kit;
  }
  debug::FrameStateDiff_Before(base);
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  const uint64_t r3 = load_poll::SceneLightsApplyRecord(base, kit, m.light_kit_record_address(),
                                                        stack, *calls.poll);
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  debug::FrameStateDiff_After(base, "draw step part", kApplyRecord);
  return r3;
}

uint64_t GarageStageSetLightKit(uint8_t* base, const Calls& calls, uint32_t holder,
                                uint32_t group, uint32_t word, uint32_t index, double f1,
                                double f2, double f3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179678 GarageStage::SetLightKit");
  // The original's frame held the texture table (+80 to +108), a local now
  // (E5 step 6); ApplyLightKit's callee makes its frame below it.
  const uint32_t f = FrameAt(stack, 176);
  const uint32_t m = StageModel(base, holder);
  const Kit k(base, Model(base, m).light_kit_record_address());
  static constexpr uint32_t kTable[8] = {0, 4, 3, 5, 2, 7, 1, 6};
  const int32_t g = int32_t(group);
  bool known = true;
  switch (g) {
    case 1:
      k.set_sides_on(float(f1));
      k.set_sides_mode(word);
      k.set_sides_fade(float(f2));
      k.set_sides_rate(float(f3));
      break;
    case 2:
      k.set_rear_on(float(f1));
      k.set_rear_mode(word);
      k.set_rear_fade(float(f2));
      k.set_rear_rate(float(f3));
      break;
    case 3:
      k.set_sides_on(float(f1));
      k.set_sides_mode(word);
      k.set_sides_fade(float(f2));
      k.set_rear_mode(word);
      k.set_sides_rate(float(f3));
      k.set_rear_on(float(f1));
      k.set_rear_fade(float(f2));
      k.set_rear_rate(float(f3));
      [[fallthrough]];
    case 0:
      k.set_front_rate(float(f3));
      k.set_front_mode(word);
      k.set_front_fade(float(f2));
      k.set_front_on(float(f1));
      break;
    default:
      known = false;
      break;
  }
  if (known) {
    // Indexed unchecked, as the original: past the table (never in play) the
    // stack's word there.
    k.set_texture(index < 8 ? kTable[index] : Load(base, f + 80 + (index << 2)));
  }
  return SceneModelApplyLightKit(base, calls, m, f);
}

uint64_t GarageStageClearLightKit(uint8_t* base, const Calls& calls, uint32_t holder,
                                  uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821795E0 GarageStage::ClearLightKit");
  const uint32_t f = FrameAt(stack, 96);
  const uint32_t m = StageModel(base, holder);
  const Kit k(base, Model(base, m).light_kit_record_address());
  switch (int32_t(group)) {
    case 0:
      k.set_front_mode(0);
      break;
    case 1:
      k.set_sides_mode(0);
      break;
    case 2:
      k.set_rear_mode(0);
      break;
    case 3:
      k.set_sides_mode(0);
      k.set_rear_mode(0);
      k.set_front_mode(0);
      break;
    default:
      break;
  }
  return SceneModelApplyLightKit(base, calls, m, f);
}

uint64_t FloorDraw(uint8_t* base, const Calls& calls, uint32_t scene, uint32_t matrix,
                   uint32_t word, uint32_t caller_r24, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820C8480 Floor::Draw");
  const uint32_t f = Frame(base, stack, 144);
  uint64_t r3 = scene;
  // The scene's +4572 (the floor's shape count) isn't in car-draw.toml's
  // CarScene yet: FrontEndFloorScene's until it's merged there.
  const schema::CarScene car_scene(base, scene);
  const schema::FrontEndFloorScene floor_scene(base, scene);
  if (int32_t(word) == 0 || int32_t(car_scene.model_set()) != 0) {
    return r3;
  }
  // The last row (0, 0, 0, 1), copied as two doublewords (the original's
  // frame +80 to +92; C++ words since E5 step 6, stfs's single bits).
  const double zero = double(G::zero(base));
  const uint32_t w0 = std::bit_cast<uint32_t>(float(zero)), w1 = w0;
  const uint32_t w2 = w0, w3 = std::bit_cast<uint32_t>(float(double(G::one(base))));
  Store(base, matrix + 48, w0);
  Store(base, matrix + 52, w1);
  Store(base, matrix + 56, w2);
  Store(base, matrix + 60, w3);
  const int32_t count = floor_scene.floor_shape_count();
  const uint32_t model = scene + 4160;
  const uint32_t view_projection =
      schema::CameraPass(base, schema::CamerasGlobals::passes_address()).view_projection_address();
  for (int32_t i = 0; i < count; i++) {
    // Its back chain first (as its adapter: the plain C++ writes into the
    // frame), put back where it declines.
    const uint32_t frame = f - scenery_lists::kModelDrawShapeFrame;
    const uint32_t chain = Load(base, frame);
    Store(base, frame, f);
    const auto draw = scenery_lists::ModelDrawShape(base, calls.draw, frame, model,
                                                    view_projection, matrix, uint32_t(i), 0, 0,
                                                    0, caller_r24);
    if (draw) {
      r3 = draw->r3;
    } else {
      Store(base, frame, chain);
      // Its original (through its adapter, which declines the same way),
      // with the caller's r24 as the adapter's.
      const uint64_t regs[7] = {model, view_projection, matrix, uint32_t(i), 0, 0, 0};
      r3 = calls.call(kDrawShape, regs, 7, nullptr, 0, f, 0x820C8514, nullptr);
    }
  }
  return r3;
}

}  // namespace front_end_car_draw
