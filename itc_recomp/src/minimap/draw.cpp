// Minimap::Draw (sub_820F5740, minimap.h; docs/functions/minimap.md): the
// map, the point markers and the car markers into the minimap's camera
// pass, step for step as the translated code (built with strict floats,
// CMakeLists.txt). Its 28,336-byte frame keeps the original's temporaries
// at their offsets (the identity and the view-projection copy the map's
// draws read, the markers' matrices and colors, the five pools of car
// markers); its callees' plain C++ runs with the frames and return addresses
// their adapters would give them. The blink clock's step is the original's;
// frame interpolation's wrap (src/frame_pacing/per_frame_logic.cpp) puts it
// back on render-only passes around this body, as around the original.

#include "minimap/minimap.h"

#include "car_scene_draw/car_scene_draw.h"
#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "c_runtime/math.h"
#include "car_physics/car_physics.h"
#include "debug/frame_state_diff.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"
#include "scenery_lists/scenery_lists.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <initializer_list>

namespace minimap {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kFrame = 28336;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kViewCopy = 0x8265ECA0;
constexpr uint32_t kDrawKeyA = 0x8296B90C;
constexpr uint32_t kDrawKeyB = 0x8296B900;
constexpr uint32_t kRangeNear = 0x82002814;
constexpr uint32_t kRangeFar = 0x82006804;
constexpr uint32_t kLowest = 0x82006808;      // -1000.0: points below aren't drawn
constexpr uint32_t kMarkerScale = 0x82002A0C;  // the markers' x and z scale
constexpr uint32_t kQuarterTurn = 0x82001FB4;  // pi / 2
constexpr uint32_t kRaceMode = 0xA3AF0;        // the race data's mode
constexpr uint32_t kStages = 0x59620;          // the race data's stages
constexpr uint32_t kHumanFlag = 0x826C7824;
constexpr uint32_t kBlinkFrames = 0x826C5AA4;
constexpr uint32_t kBlinkPhase = 0x826C5AA8;
constexpr uint32_t kColors = 0x826C5AB0;      // the five types' colors
constexpr uint32_t kColorsGuard = 0x826C5B00;
constexpr uint32_t kColorsCleanUp = 0x825DAEF0;
constexpr uint32_t kSetWsToOne = 0x8209B038;
constexpr uint32_t kEmptyDestructor = 0x82106540;

// The pools: five of 5,488 bytes from +688 (+0 the type, +16 the color, +32
// the List80, its data from +368).
constexpr uint32_t kPools = 688, kPoolBytes = 5488;

// The originals' callees (their adapters run the originals where the plain
// C++ declines).
constexpr uint32_t kDrawShape = 0x82147D88;
constexpr uint32_t kScaling = 0x820F2EF8;
constexpr uint32_t kRacerMarkerType = 0x820F2D28;
constexpr uint32_t kList80Copy = 0x820F51A8;
constexpr uint32_t kStagesCopy = 0x820FFD88;

void FlushOff() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

// The identity's rows at `at` (the vupkd3d128 shuffles' words).
void Identity(uint8_t* base, uint32_t at) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t col = 0; col < 4; col++) {
      Store(base, at + 16 * row + 4 * col, row == col ? 0x3F800000u : 0u);
    }
  }
}

class Drawer {
 public:
  Drawer(uint8_t* base, const DrawCalls& calls, uint32_t f, uint32_t caller_r24)
      : base_(base), calls_(calls), f_(f), caller_r24_(caller_r24) {}
  void Run(uint32_t manager, uint32_t description);

 private:
  uint64_t Call(uint32_t function, std::initializer_list<uint64_t> regs, uint32_t lr,
                std::initializer_list<double> fregs = {}) {
    const uint64_t r3 = calls_.calls->call(function, regs.begin(), int(regs.size()),
                                           fregs.begin(), int(fregs.size()), f_, lr);
    FlushOff();
    return r3;
  }
  void Scale(uint32_t out, uint32_t vector, uint32_t lr) {
    if (!Scaling(base_, out, vector)) {
      Call(kScaling, {out, vector}, lr);
    }
    FlushOff();
  }
  void Translate(uint32_t out, uint32_t vector) {
    car_physics::MatrixMultiplyTranslation(base_, out, out, vector, f_ - 192);
    FlushOff();
  }
  void Map(uint32_t set);
  void Points(uint32_t set, uint32_t pass);
  void CarMarkers(uint32_t pass);

  uint8_t* base_;
  const DrawCalls& calls_;
  uint32_t f_;
  uint32_t caller_r24_;
  double one_ = 0, zero_ = 0;
};

void Drawer::Run(uint32_t manager, uint32_t description) {
  const uint32_t f = f_;
  const uint32_t set = Load(base_, manager + 112);  // r26
  const uint32_t pass = Load(base_, description + 20) * 592u + kCameraPasses;  // r14
  one_ = Lfs(base_, kOne);    // f31
  zero_ = Lfs(base_, kZero);  // f30
  // The compiler's dead stores (overwritten before they're read).
  for (uint32_t at = 364; at <= 476; at += 16) {
    Stfs(base_, f + at, one_);
  }
  if (Load(base_, set + 1064) == 0) {
    return;
  }
  // The identity at +112, copied to +352; the pass's view-projection at
  // +416 and view at 0x8265ECA0 (both copies noted for interpolation), the
  // keys.
  Identity(base_, f + 112);
  for (uint32_t at = 0; at < 64; at += 4) {
    Store(base_, f + 352 + at, Load(base_, f + 112 + at));
  }
  matrix_math::Copy(base_, f + 416, pass + 144);
  interpolation::BlendState_OnCopy(base_, f + 416, pass + 144, 0x820F5820);
  FlushOff();
  const uint32_t key_b = Load(base_, description + 28);
  const uint32_t key_a = Load(base_, description + 24);
  Store(base_, kDrawKeyA, key_a);
  Store(base_, kDrawKeyB, key_b);
  matrix_math::Copy(base_, kViewCopy, pass + 16);
  interpolation::BlendState_OnCopy(base_, kViewCopy, pass + 16, 0x820F5848);
  FlushOff();
  Map(set);
  Points(set, pass);
  CarMarkers(pass);
}

// The first n models of the set's list at +1064, each shape drawn.
void Drawer::Map(uint32_t set) {
  const uint32_t f = f_;
  const uint32_t list = set + 1064;
  int32_t count = int32_t(Load(base_, list));  // r27
  const uint32_t race = Load(base_, kRaceData);
  if (race != 0 && int32_t(Load(base_, race + kRaceMode)) == 1) {
    const uint32_t flags = Load(base_, race + 20);
    if (flags & 0x20) {
      count = 4;
    } else if ((flags & 0x4) && (flags & 0x8)) {
      count = 3;
    } else {
      count = (flags & 0x10) ? 2 : 1;
    }
  }
  const double far_range = Lfs(base_, kRangeFar);    // f28
  const double near_range = Lfs(base_, kRangeNear);  // f29
  const uint32_t nodes = list + 4;  // r29
  for (int32_t i = 0; i < count; i++) {
    auto node_at = [&](uint32_t at) {
      return (uint32_t(int32_t(int16_t(guest::LoadU16(base_, at)))) << 2) + nodes;
    };
    uint32_t node = node_at(nodes + 260);
    for (uint32_t k = uint32_t(i); k != 0; k--) {
      node = node_at(node);
    }
    const int32_t slot = int16_t(guest::LoadU16(base_, node + 2));
    const uint32_t model = Load(base_, (uint32_t(slot + 67) << 2) + nodes);
    Store(base_, model + 48, 0);
    for (uint32_t j = 0; int32_t(j) < int32_t(Load(base_, model + 412)); j++) {
      Stfs(base_, f + 256, near_range);
      Stfs(base_, f + 260, far_range);
      Stfs(base_, f + 264, zero_);
      Stfs(base_, f + 268, zero_);
      const uint32_t frame = f - scenery_lists::kSceneryDrawShapeFrame;
      const uint32_t chain = Load(base_, frame);
      Store(base_, frame, f);
      const auto drawn = scenery_lists::SceneryDrawShape(base_, *calls_.draws, frame, model,
                                                         f + 416, f + 352, j, 0, 0, f + 256, 0,
                                                         caller_r24_);
      FlushOff();
      if (!drawn) {
        Store(base_, frame, chain);
        Call(kDrawShape, {model, f + 416, f + 352, j, 0, 0, f + 256, 0}, 0x820F5960);
      }
    }
  }
}

// The point markers: points 0 to 2 of each accepted entry in one color,
// point 3 in another, each above -1000.0 put on the ground.
void Drawer::Points(uint32_t set, uint32_t pass) {
  const uint32_t f = f_;
  const uint32_t list = set + 3180;  // r31
  if (int32_t(Load(base_, list + 620)) <= 0) {
    return;
  }
  const double lowest = Lfs(base_, kLowest);  // f29
  // The scale vector (s, 1.0, s, 1.0) at `scale`.
  auto scale_vector = [&](uint32_t scale) {
    const double s = Lfs(base_, kMarkerScale);
    Stfs(base_, scale + 0, s);
    Stfs(base_, scale + 4, one_);
    Stfs(base_, scale + 8, s);
    Stfs(base_, scale + 12, one_);
  };
  auto place = [&](uint32_t entry, uint32_t point) {
    SetListPoint(base_, list, f + 176, entry, point, 0);
    if (Lfs(base_, f + 180) < lowest) {
      return;
    }
    Stfs(base_, f + 180, zero_);
    Translate(f + 288, f + 176);
    const uint32_t frame = f - kPointMarkerFrame;
    Store(base_, frame, f);  // its back chain, as stwu
    DrawPointMarker(base_, pass, f + 192, f + 288, frame, *calls_.point_marker);
    FlushOff();
  };
  for (uint32_t i = 0; int32_t(i) < int32_t(Load(base_, list + 620)); i++) {
    if (scenery_lists::PointListIsEntryOn(base_, int32_t(i)) == 0) {
      continue;
    }
    Stfs(base_, f + 188, one_);
    Stfs(base_, f + 300, one_);
    Store(base_, f + 200, 255);
    Stfs(base_, f + 316, one_);
    Store(base_, f + 204, 255);
    Stfs(base_, f + 332, one_);
    Stfs(base_, f + 348, one_);
    Store(base_, f + 192, 168);
    Store(base_, f + 196, 124);
    for (uint32_t point = 0; point < 3; point++) {
      scale_vector(f + 224);
      Scale(f + 288, f + 224, 0x820F5A04);
      place(i, point);
    }
    Store(base_, f + 104, 255);
    Store(base_, f + 108, 255);
    Store(base_, f + 96, 103);
    Store(base_, f + 100, 252);
    scale_vector(f + 240);
    for (uint32_t w = 0; w < 16; w += 4) {  // the color to +192 (two ld/std)
      Store(base_, f + 192 + w, Load(base_, f + 96 + w));
    }
    Scale(f + 288, f + 240, 0x820F5AA8);
    place(i, 3);
  }
}

// The car markers: the five types' colors made once, five pools of markers
// sorted by type and the blink phase, drawn pool by pool; the blink clock.
void Drawer::CarMarkers(uint32_t pass) {
  const uint32_t f = f_;
  const uint32_t guard = Load(base_, kColorsGuard);
  if ((guard & 1) == 0) {
    Store(base_, kColorsGuard, guard | 1);
    static constexpr uint32_t kTable[20] = {255, 241, 0,   255, 255, 66,  23,  255, 0,  255,
                                            255, 255, 255, 80,  255, 255, 34,  172, 54, 255};
    for (uint32_t i = 0; i < 20; i++) {
      Store(base_, kColors + 4 * i, kTable[i]);
    }
    c_runtime::AtexitFrom(base_, kColorsCleanUp, calls_.atexit);
    FlushOff();
  }
  for (uint32_t k = 0; k < 5; k++) {
    c_runtime::VectorConstruct(base_, f + 1056 + kPoolBytes * k, 64, 80, kSetWsToOne, nullptr,
                               nullptr);
  }
  for (uint32_t k = 0; k < 5; k++) {
    const uint32_t at = f + 712 + kPoolBytes * k;  // the original's r11
    Store(base_, at - 24, 0xFFFFFFFFu);
    Store(base_, at - 8, 0);
    Store(base_, at - 4, 0);
    Store(base_, at + 0, 0);
    Store(base_, at + 4, 1);
    const uint32_t nodes = at + 8;
    for (uint32_t n = 0; n < 80; n++) {
      guest::StoreU16(base_, nodes + 4 * n + 2, uint16_t(n));
      guest::StoreU16(base_, nodes + 4 * n + 0, uint16_t(n + 1));
    }
    guest::StoreU16(base_, nodes + 316, 0xFFFF);
    guest::StoreU16(base_, nodes + 324, 0xFFFF);
    guest::StoreU16(base_, nodes + 326, 0xFFFF);
    guest::StoreU16(base_, nodes + 320, 0);
    guest::StoreU16(base_, nodes + 322, 0xFFFF);
    guest::StoreU16(base_, nodes + 328, 0);
  }

  if (vehicle_accessors::VehicleGet(base_, 0) != 0) {
    uint32_t manager = Load(base_, kRaceManager);
    // The player's branch number: byte +3 of its section.
    const uint32_t player = RaceFindPlayerEntry(base_, manager, 1);
    Store(base_, f + 80, Load(base_, player + 8));
    const uint32_t section = uint32_t(guest::LoadU16(base_, f + 80)) << 2;
    const uint32_t sections = Load(base_, Load(base_, kRoute) + 12);
    const int32_t branch = guest::LoadU8(base_, Load(base_, section + sections) + 3);  // r20
    const double quarter = Lfs(base_, kQuarterTurn);  // f29
    for (uint32_t i = 0; int32_t(i) < int32_t(Load(base_, manager + 2368)); i++) {
      const uint32_t entry = manager + 28 * i + 4;  // r27
      const uint32_t flags = Load(base_, entry);
      uint32_t vehicle = 0;  // r28
      int32_t type = 0;      // r30
      if (flags & 0xC) {
        const bool human = (flags & 0x4) == 0;  // the 0x8 entries'
        vehicle = RaceRacerVehicle(base_, i);
        const uint32_t race = Load(base_, kRaceData);
        if (int32_t(Load(base_, race + kRaceMode)) == 6) {
          const uint32_t stage = f + 544;
          if (!debug::IsHidden(kStagesCopy)) {
            debug::FrameStateDiff_Before(base_);
            const bool copied =
                vehicle_accessors::StagesCopy(base_, stage, race + kStages, 0xFFFFFFFFu);
            FlushOff();
            debug::FrameStateDiff_After(base_, "draw step part", kStagesCopy);
            if (!copied) {
              Call(kStagesCopy, {stage, race + kStages, 0xFFFFFFFFFFFFFFFFull},
                   human ? 0x820F5D6C : 0x820F5D14);
            }
          }
          if (int32_t(Load(base_, stage)) != 1) {
            const uint32_t humans = Load(base_, kHumanFlag);
            if (human ? humans != 0 : humans == 0) {
              type = 1;
            }
          }
        }
      } else if ((flags & 0x1) != 0 && (flags & 0x20) == 0) {
        const auto marker_type = RacerMarkerType(base_, entry, branch);
        type = marker_type ? *marker_type
                           : int32_t(Call(kRacerMarkerType, {entry, uint64_t(int64_t(branch))},
                                          0x820F5DA4));
        if (type == -1) {
          manager = Load(base_, kRaceManager);
          continue;
        }
      } else {
        continue;
      }
      // Its pool by the type and the blink phase.
      const int32_t sum = type + int32_t(Load(base_, kBlinkPhase));
      const int32_t index = sum - (sum / 5) * 5;
      const uint32_t pool = f + kPools + kPoolBytes * uint32_t(index);
      if (int32_t(Load(base_, pool)) == -1) {
        Store(base_, pool, uint32_t(type));
        const uint32_t color = (uint32_t(type) << 4) + kColors;
        for (uint32_t w = 0; w < 16; w += 4) {
          Store(base_, pool + 16 + w, Load(base_, color + w));
        }
      }
      // The marker's matrix at +112.
      for (uint32_t at : {124u, 140u, 156u, 172u}) {
        Stfs(base_, f + at, one_);
      }
      Identity(base_, f + 112);
      if (vehicle != 0) {
        if (!native::Buffers_Memcpy(base_, f + 480, vehicle + 3312, 64, 0x820F5E6C)) {
          calls_.calls->memcpy(f + 480, vehicle + 3312, 64, f, 0x820F5E6C);
        }
        FlushOff();
        const double s = Lfs(base_, kMarkerScale);
        Stfs(base_, f + 532, zero_);
        Stfs(base_, f + 272, s);
        Stfs(base_, f + 276, one_);
        Stfs(base_, f + 280, s);
        Stfs(base_, f + 284, one_);
        Scale(f + 112, f + 272, 0x820F5E90);
        Stfs(base_, f + 80, quarter);
        matrix_math::RotateY(base_, f + 112, f + 112, f + 80, f - 80);
        FlushOff();
        matrix_math::Multiply(base_, f + 112, f + 112, f + 480);
        interpolation::BlendState_OnMultiply(base_, f + 112, f + 112, f + 480, 0x820F5EB4);
        FlushOff();
      } else {
        const uint32_t sections_now = Load(base_, Load(base_, kRoute) + 12);
        const uint32_t at_section = uint32_t(guest::LoadU16(base_, entry + 8)) << 2;
        const int32_t at_slice = int16_t(guest::LoadU16(base_, entry + 10));
        const uint32_t slice = Load(base_, at_section + sections_now) + uint32_t(at_slice) * 84u;
        const double angle = c_runtime::Atan2(Lfs(base_, slice + 168), Lfs(base_, slice + 172));
        FlushOff();
        Stfs(base_, f + 96, Lfs(base_, slice + 124));
        Stfs(base_, f + 104, Lfs(base_, slice + 132));
        const double s = Lfs(base_, kMarkerScale);
        Stfs(base_, f + 208, s);
        Stfs(base_, f + 216, s);
        Stfs(base_, f + 108, one_);
        Stfs(base_, f + 100, zero_);
        Stfs(base_, f + 212, one_);
        Stfs(base_, f + 220, one_);
        Stfs(base_, f + 80, Frsp(angle));
        Scale(f + 112, f + 208, 0x820F5F28);
        matrix_math::RotateY(base_, f + 112, f + 112, f + 80, f - 80);
        FlushOff();
        Translate(f + 112, f + 96);
      }
      if (guest::LoadU16(base_, pool + 360) < 80) {
        const uint32_t list = pool + 32;
        List80Insert(base_, list, f + 112, uint32_t(int32_t(int16_t(guest::LoadU16(base_, list + 328)))));
        FlushOff();
      }
      manager = Load(base_, kRaceManager);
    }
  }

  // Each pool's markers, in order.
  for (uint32_t k = 0; k < 5; k++) {
    const uint32_t list = f + 720 + kPoolBytes * k;  // r31
    if (int32_t(Load(base_, list - 32)) == -1) {
      continue;
    }
    for (uint32_t j = 0; j < uint32_t(int32_t(int16_t(guest::LoadU16(base_, list + 328)))); j++) {
      if (!List80Copy(base_, f + 624, list, j)) {
        Call(kList80Copy, {f + 624, list, j}, 0x820F5FBC);
      }
      FlushOff();
      const uint32_t frame = f - kCarMarkerFrame;
      Store(base_, frame, f);  // its back chain, as stwu
      DrawCarMarker(base_, pass, list - 16, f + 624, frame, *calls_.car_marker);
      FlushOff();
    }
  }

  // The blink clock.
  const uint32_t frames = Load(base_, kBlinkFrames) + 1;
  Store(base_, kBlinkFrames, frames);
  if (frames > 4) {
    const uint32_t phase = Load(base_, kBlinkPhase) + 1;
    Store(base_, kBlinkFrames, 0);
    Store(base_, kBlinkPhase, phase);
    if (int32_t(phase) > 5) {
      Store(base_, kBlinkPhase, 0);
    }
  }
  // The pools' destructor iterator: the empty destructor, nothing.
  for (uint32_t k = 5; k-- > 0;) {
    c_runtime::VectorDestruct(base_, f + 1056 + kPoolBytes * k, 64, 80, kEmptyDestructor,
                              nullptr, nullptr);
  }
}

}  // namespace

void Draw(uint8_t* base, const DrawCalls& calls, uint32_t stack, uint32_t manager,
          uint32_t description, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820F5740 Minimap::Draw");
  const uint32_t f = stack - kFrame;
  Store(base, f, stack);  // the back chain, as stwu (its stack probe left out)
  Drawer(base, calls, f, caller_r24).Run(manager, description);
}

}  // namespace minimap
