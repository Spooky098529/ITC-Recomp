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
#include "vehicle_accessors/vehicle_accessors.h"

namespace front_end_car_draw {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kHalfDegree = 0x82009EA8;  // π/360 (0.5°)
constexpr uint32_t kMinusPi = 0x820029C4;
constexpr uint32_t kPi = 0x82002240;
constexpr uint32_t kDegree = 0x82002948;      // π/180
constexpr uint32_t kFocal = 0x82009EB8;       // 640.0
constexpr uint32_t k360 = 0x82002244;         // 360.0
constexpr uint32_t kSixty = 0x82000F14;       // 60.0
constexpr uint32_t kTwenty = 0x82002568;      // 20.0

constexpr uint32_t kMaxima = 0x82635E90;     // the cars' opening maxima (48 bytes a car)
constexpr uint32_t kOpenCue = 15;             // MenuSound::Play's cue for the opening

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kTan = 0x825A8A50;
constexpr uint32_t kApplyRecord = 0x820DCBC0;
constexpr uint32_t kDrawShape = 0x82596B18;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kOpenDoors = 0x8217B6B8;
constexpr uint32_t kCloseDoors = 0x8217B740;

int32_t LoadS(uint8_t* base, uint32_t address) { return int32_t(Load(base, address)); }

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// The stage's scene model: ScenePools::GetB([0x826C2E44], 7, [[holder] + 8]).
uint32_t StageModel(uint8_t* base, uint32_t holder) {
  return vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7,
                                           Load(base, Load(base, holder) + 8));
}

// A yaw or pitch wrapped: below -π to π, above π to -π (NaN kept).
double Wrap(uint8_t* base, double v) {
  const double minus_pi = Lfs(base, kMinusPi);
  if (v < minus_pi) {
    return Lfs(base, kPi);
  }
  if (v > Lfs(base, kPi)) {
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
  Store(base, points + 2640, 0);
  Store(base, points + 4576, 0);
}

void PlatesShow(uint8_t* base, uint32_t points) {
  ITC_NATIVE_RUNNING("sub_820DF180 Plates::Show");
  Store(base, points + 2640, 1);
  Store(base, points + 4576, 1);
}

uint32_t GarageStageIsBusy(uint8_t* base, const Calls& calls, uint32_t holder, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179BE0 GarageStage::IsBusy");
  const uint32_t frame = Frame(base, stack, 96);
  const uint32_t model = StageModel(base, holder);
  const uint64_t loaded = load_poll::SceneModelPollLoad(base, model, frame, *calls.poll);
  if (int32_t(loaded) == 0) {
    return 1;
  }
  return LoadS(base, Load(base, holder) + 4) == -1 ? 0 : 1;
}

uint32_t GarageStageSyncPlates(uint8_t* base, const Calls& calls, uint32_t holder,
                               uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217AFE8 GarageStage::SyncPlates");
  const uint32_t frame = Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  const uint32_t busy = GarageStageIsBusy(base, calls, holder, frame);
  if (busy == 1) {
    return busy;
  }
  const uint32_t parts = Load(base, model + 5060);
  const uint32_t want = Load(base, Load(base, holder) + 6092);
  if (want == Load(base, parts + 2816)) {
    return busy;
  }
  const uint32_t points = parts + 176;
  if (want == 1) {
    PlatesShow(base, points);
  } else {
    PlatesHide(base, points);
  }
  return points;
}

uint32_t GarageStageSetDrawWord(uint8_t* base, uint32_t holder, uint32_t word, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82178B40 GarageStage::SetDrawWord");
  Frame(base, stack, 112);
  const uint32_t model = StageModel(base, holder);
  Store(base, Load(base, holder) + 6056, word);
  Store(base, model + 4296, word);
  return model;
}

void GarageStageStepAngles(uint8_t* base, uint32_t command, uint32_t angles) {
  ITC_NATIVE_RUNNING("sub_82179C88 GarageStage::StepAngles");
  const uint32_t a = angles + 48;
  const int32_t c = int32_t(command);
  switch (c) {
    case 9:
    case 10: {
      const double step = Lfs(base, kHalfDegree);
      const double v = c == 9 ? Fadds(Lfs(base, a + 4), step) : Fsubs(Lfs(base, a + 4), step);
      Stfs(base, a + 4, Wrap(base, v));
      return;
    }
    case 11:
    case 12: {
      const double step = Lfs(base, kHalfDegree);
      const double v = c == 11 ? Fadds(Lfs(base, a + 8), step) : Fsubs(Lfs(base, a + 8), step);
      Stfs(base, a + 8, Wrap(base, v));
      return;
    }
    case 13: {
      const double zero = Lfs(base, kZero);
      Stfs(base, a + 0, zero);
      Stfs(base, a + 8, zero);
      Stfs(base, a + 4, zero);
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
  uint64_t r3 = list;
  auto copy = [&](uint32_t to, uint32_t from, uint32_t lr) {
    r3 = Memcpy(base, calls, to, from, 28, f, lr);
  };

  if (int32_t(type) == 1) {
    // Popped from the end; the non-turns kept (in reverse), the earliest of
    // them put back alone.
    // n is the original's r28: decremented from its register, the loop's
    // test reloading the count.
    uint32_t n = Load(base, list + 448);
    uint32_t count = 0;
    if (n != 0) {
      do {
        n = n - 1;
        Store(base, list + 448, n);
        const uint32_t e = list + 28 * n;
        copy(entry, e, 0x8217FA48);
        if (LoadS(base, entry) != 0) {
          Store(base, kept_count, count + 1);
          copy(kept + 28 * count, e, 0x8217FA78);
          count = Load(base, kept_count);
        }
      } while (Load(base, list + 448) != 0);
    }
    Store(base, list + 448, 0);
    if (count == 0) {
      return r3;
    }
    Store(base, kept_count, count - 1);
    Store(base, list + 448, 1);
    copy(list, kept + 28 * (count - 1), 0x8217FBC0);
    return r3;
  }

  const uint32_t count = Load(base, list + 448);
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
    } while (i < Load(base, list + 448));
  }
  // One of the opposite type: overwritten in place.
  if (count != 0) {
    uint32_t i = 0, e = list;
    do {
      const int32_t t = LoadS(base, e);
      const int32_t y = int32_t(type);
      if ((t == 2 && y == 3) || (t == 3 && y == 2) || (t == 4 && y == 5) || (t == 5 && y == 4)) {
        Stfs(base, e + 4, steps);
        Store(base, e + 0, type);
        Store(base, e + 12, value);
        Store(base, e + 8, word);
        Store(base, e + 20, 0);
        return r3;
      }
      i++;
      e += 28;
    } while (i < Load(base, list + 448));
  }
  // A new entry at the end.
  Stfs(base, entry + 4, steps);
  Store(base, entry + 0, type);
  Store(base, entry + 12, value);
  Store(base, entry + 8, word);
  Store(base, entry + 20, 0);
  const double zero = Lfs(base, kZero);
  Store(base, entry + 16, index);
  Stfs(base, entry + 24, zero);
  if (int32_t(type) == 6) {
    const double angle = Fsubs(Lfs(base, k360), Lfs(base, value + 176));
    Stfs(base, entry + 24, Fdivs(angle, steps));
  }
  Store(base, list + 448, count + 1);
  copy(list + 28 * count, entry, 0x8217FBC0);
  return r3;
}

uint64_t GarageStageApplyPreset(uint8_t* base, const Calls& calls, uint64_t r3, uint32_t preset,
                                uint32_t stage, uint32_t index, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217D2B0 GarageStage::ApplyPreset");
  const uint32_t f = Frame(base, stack, 128);
  const uint32_t camera = stage + 112;
  if (Load(base, camera) == 0) {
    Store(base, stage + 6088, preset);
    return r3;
  }
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7, index);
  const uint64_t slot = uint64_t(Load(base, model + 48)) * 290 + preset;
  uint32_t e = Load(base, Load(base, camera + 292) + uint32_t(slot << 2));
  if (e >= Load(base, camera + 4392)) {
    e = 0xFFFFFFFFu;
  }
  const uint32_t p = e == 0xFFFFFFFFu ? 0 : Load(base, camera + ((e + 74) << 2));
  const uint32_t c = camera + 16;
  Stfs(base, c + 84, Lfs(base, p + 4));
  Stfs(base, c + 164, Lfs(base, p + 8));
  Stfs(base, c + 208, Lfs(base, p + 12));
  const double x = Fmuls(Lfs(base, p + 12), Lfs(base, kDegree));
  // tan as its adapter gives it: nearest rounding (its _ctrlfp) and r3 its
  // second _ctrlfp's word (0 ^ 0xF8); else the call.
  double t;
  if ((Fpscr::getcsr() & Fpscr::RoundMaskVal) == 0 && c_runtime::Tan(x, &t)) {
    r3 = 0xF8;
  } else {
    r3 = calls.call(kTan, nullptr, 0, &x, 1, f, 0x8217D368, &t);
  }
  Stfs(base, c + 92, Fdivs(Lfs(base, kFocal), Frsp(t)));
  const uint32_t w0 = Load(base, p + 32), w1 = Load(base, p + 36);
  Store(base, c + 240, w0);
  Store(base, c + 244, w1);
  const uint32_t w2 = Load(base, p + 40), w3 = Load(base, p + 44);
  Store(base, c + 248, w2);
  Store(base, c + 252, w3);
  const uint32_t w4 = Load(base, p + 16), w5 = Load(base, p + 20);
  Store(base, c + 224, w4);
  Store(base, c + 228, w5);
  const uint32_t w6 = Load(base, p + 24), w7 = Load(base, p + 28);
  Store(base, c + 232, w6);
  Store(base, c + 236, w7);
  const uint32_t w8 = Load(base, p + 48), w9 = Load(base, p + 52);
  Store(base, stage + 80, w8);
  Store(base, stage + 84, w9);
  const uint32_t w10 = Load(base, p + 56), w11 = Load(base, p + 60);
  Store(base, stage + 88, w10);
  Store(base, stage + 92, w11);
  Store(base, stage + 6088, 0xFFFFFFFFu);
  return r3;
}

uint64_t GarageStageCommand(uint8_t* base, const Calls& calls, uint64_t r3, uint32_t command,
                            uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82180108 GarageStage::Command");
  const uint32_t f = Frame(base, stack, 112);
  const uint32_t holder = uint32_t(r3);
  const int32_t c = int32_t(command);
  auto add = [&](uint32_t type, uint32_t value_offset, bool value_zero, uint32_t word,
                 uint32_t steps_at) {
    const uint32_t s = Load(base, holder);
    const uint32_t value = value_zero ? 0 : s + value_offset;
    return GarageAnimsAdd(base, calls, s + 5600, type, value, Load(base, s + 8), word,
                          Lfs(base, steps_at), f);
  };
  if (c > 5) {
    if (c == 6 || c == 7) {
      return add(uint32_t(c), 112, false, 0, kTwenty);
    }
    if (c == 8) {
      const uint32_t list = Load(base, holder) + 5600;
      if (Load(base, list + 448) != 0) {
        Store(base, list + 448, 0);
      }
      Store(base, list + 448, 0);
      add(6, 112, false, 0, kTwenty);
      return add(7, 32, false, 0, kTwenty);
    }
    if (c < 8) {
      return r3;
    }
    if (c <= 13) {
      const uint32_t s = Load(base, holder);
      GarageStageStepAngles(base, command, s + 32);
      return s + 5600;
    }
    if (c > 64) {
      return r3;
    }
    const uint32_t s = Load(base, holder);
    return GarageStageApplyPreset(base, calls, s + 5600, command - 14, s, Load(base, s + 8), f);
  }
  switch (command) {
    case 0:
      return add(0, 32, false, 1, kSixty);
    case 1:
      add(1, 0, true, 1, kSixty);
      return add(1, 32, false, 0, kSixty);
    case 2:
    case 3:
    case 4:
    case 5:
      return add(command, 32, false, 0, kSixty);
    default:
      return r3;  // negative: nothing
  }
}

uint32_t GarageAnimsTurn(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_82179D78 GarageAnims::Turn");
  const uint32_t angles = Load(base, entry + 12) + 48;
  const double yaw = Wrap(base, Fsubs(Lfs(base, angles + 4), Lfs(base, kHalfDegree)));
  Stfs(base, angles + 4, yaw);
  if (LoadS(base, entry + 8) != 0) {
    return 0;
  }
  if (yaw != Lfs(base, kZero)) {
    return 0;
  }
  Store(base, entry + 20, 1);
  return 1;
}

uint32_t GarageAnimsOpen(uint8_t* base, const Calls& calls, uint32_t entry, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179DF0 GarageAnims::Open");
  const uint32_t f = Frame(base, stack, 128);
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7, Load(base, entry + 16));
  const uint32_t car = Load(base, model + 48);
  const double steps = Lfs(base, entry + 4);
  const uint32_t value = Load(base, entry + 12);
  const double now = Lfs(base, value);
  const double max = Lfs(base, kMaxima + car * 48);
  const double step = Fdivs(max, steps);
  double v = Fadds(now, step);
  const double zero = Lfs(base, kZero);
  if (v < zero) {
    v = zero;
  } else if (v > max) {
    v = max;
  }
  Stfs(base, value, v);
  if (!(v > step)) {
    PlayMenuSound(base, calls, kOpenCue, f);
  }
  if (Lfs(base, value) < max) {
    return 0;
  }
  Store(base, entry + 20, 1);
  Stfs(base, value, max);
  return 1;
}

uint32_t GarageAnimsClose(uint8_t* base, const Calls& calls, uint32_t entry, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179EA8 GarageAnims::Close");
  const uint32_t f = Frame(base, stack, 112);
  const uint32_t model =
      vehicle_accessors::ScenePoolsGetB(base, Load(base, kScene), 7, Load(base, entry + 16));
  const uint32_t car = Load(base, model + 48);
  const double steps = Lfs(base, entry + 4);
  const uint32_t value = Load(base, entry + 12);
  const double now = Lfs(base, value);
  const double max = Lfs(base, kMaxima + car * 48);
  const double v = Fsubs(now, Fdivs(max, steps));
  const double zero = Lfs(base, kZero);
  double stored = max;
  if (v < zero) {
    stored = zero;
  } else if (!(v > max)) {
    stored = v;
  }
  Stfs(base, value, stored);
  if (stored > zero) {
    return 0;
  }
  Store(base, entry + 20, 1);
  Stfs(base, value, zero);
  PlayMenuSound(base, calls, kOpenCue, f);
  return 1;
}

uint32_t GarageAnimsRun(uint8_t* base, const Calls& calls, uint32_t list, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217D120 GarageAnims::Run");
  const uint32_t f = Frame(base, stack, 624);
  uint32_t any = 0;
  if (Load(base, list + 448) == 0) {
    return any;
  }
  uint32_t i = 0, e = list, count = 0;
  do {
    const int32_t type = LoadS(base, e);
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
    count = Load(base, list + 448);
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
      Store(base, list + 448, n);
      const uint32_t from = list + 28 * n;
      Memcpy(base, calls, entry, from, 28, f, 0x8217D21C);
      if (LoadS(base, entry + 20) == 0) {
        Store(base, kept_count, k + 1);
        Memcpy(base, calls, kept + 28 * k, from, 28, f, 0x8217D24C);
        k = Load(base, kept_count);
      }
    } while (Load(base, list + 448) != 0);
  }
  Store(base, list + 448, 0);
  while (k != 0) {
    k = k - 1;
    const uint32_t at = Load(base, list + 448);
    Store(base, kept_count, k);
    Store(base, list + 448, at + 1);
    Memcpy(base, calls, list + 28 * at, kept + 28 * k, 28, f, 0x8217D298);
  }
  return any;
}

uint64_t SceneModelApplyLightKit(uint8_t* base, const Calls& calls, uint32_t model,
                                 uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D9D10 SceneModel::ApplyLightKit");
  if (LoadS(base, model + 5036) == 0 || LoadS(base, model + 32) == 3) {
    return model;
  }
  const uint32_t kit = Load(base, model + 5084);
  if (kit == 0) {
    return 0;
  }
  // The tail call, inside its wrap (src/debug/frame_state_diff.cpp).
  if (debug::IsHidden(kApplyRecord)) {
    return kit;
  }
  debug::FrameStateDiff_Before(base);
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  const uint64_t r3 = load_poll::SceneLightsApplyRecord(base, kit, model + 4224, stack, *calls.poll);
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
  debug::FrameStateDiff_After(base, "draw step part", kApplyRecord);
  return r3;
}

uint64_t GarageStageSetLightKit(uint8_t* base, const Calls& calls, uint32_t holder,
                                uint32_t group, uint32_t word, uint32_t index, double f1,
                                double f2, double f3, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82179678 GarageStage::SetLightKit");
  const uint32_t f = Frame(base, stack, 176);
  const uint32_t m = StageModel(base, holder);
  const uint32_t table[8] = {0, 4, 3, 5, 2, 7, 1, 6};
  for (uint32_t i = 0; i < 8; i++) {
    Store(base, f + 80 + 4 * i, table[i]);
  }
  const int32_t g = int32_t(group);
  bool known = true;
  switch (g) {
    case 1:
      Stfs(base, m + 4248, f1);
      Store(base, m + 4244, word);
      Stfs(base, m + 4252, f2);
      Stfs(base, m + 4256, f3);
      break;
    case 2:
      Stfs(base, m + 4264, f1);
      Store(base, m + 4260, word);
      Stfs(base, m + 4268, f2);
      Stfs(base, m + 4272, f3);
      break;
    case 3:
      Stfs(base, m + 4248, f1);
      Store(base, m + 4244, word);
      Stfs(base, m + 4252, f2);
      Store(base, m + 4260, word);
      Stfs(base, m + 4256, f3);
      Stfs(base, m + 4264, f1);
      Stfs(base, m + 4268, f2);
      Stfs(base, m + 4272, f3);
      [[fallthrough]];
    case 0:
      Stfs(base, m + 4240, f3);
      Store(base, m + 4228, word);
      Stfs(base, m + 4236, f2);
      Stfs(base, m + 4232, f1);
      break;
    default:
      known = false;
      break;
  }
  if (known) {
    Store(base, m + 4224, Load(base, f + 80 + (index << 2)));
  }
  return SceneModelApplyLightKit(base, calls, m, f);
}

uint64_t GarageStageClearLightKit(uint8_t* base, const Calls& calls, uint32_t holder,
                                  uint32_t group, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_821795E0 GarageStage::ClearLightKit");
  const uint32_t f = Frame(base, stack, 96);
  const uint32_t m = StageModel(base, holder);
  switch (int32_t(group)) {
    case 0:
      Store(base, m + 4228, 0);
      break;
    case 1:
      Store(base, m + 4244, 0);
      break;
    case 2:
      Store(base, m + 4260, 0);
      break;
    case 3:
      Store(base, m + 4244, 0);
      Store(base, m + 4260, 0);
      Store(base, m + 4228, 0);
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
  if (int32_t(word) == 0 || LoadS(base, scene + 4) != 0) {
    return r3;
  }
  // The last row (0, 0, 0, 1) at +80, copied as two doublewords.
  const double zero = Lfs(base, kZero);
  Stfs(base, f + 80, zero);
  Stfs(base, f + 84, zero);
  Stfs(base, f + 88, zero);
  Stfs(base, f + 92, Lfs(base, kOne));
  const uint32_t w0 = Load(base, f + 80), w1 = Load(base, f + 84);
  const uint32_t w2 = Load(base, f + 88), w3 = Load(base, f + 92);
  Store(base, matrix + 48, w0);
  Store(base, matrix + 52, w1);
  Store(base, matrix + 56, w2);
  Store(base, matrix + 60, w3);
  const int32_t count = LoadS(base, scene + 4572);
  const uint32_t model = scene + 4160;
  const uint32_t view_projection = kCameraPasses + 144;
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
