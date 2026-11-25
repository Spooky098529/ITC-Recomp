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
#include "schema/car_physics.h"
#include "schema/cameras.h"
#include "schema/course_scenery.h"
#include "schema/minimap.h"
#include "schema/models.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/scenery_lists.h"
#include "schema/track_route.h"
#include "scenery_lists/scenery_lists.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <initializer_list>

namespace minimap {

namespace {

using namespace car_scene_draw::ops;
using G = schema::MinimapGlobals;
using List = schema::MinimapList80;
using Entry = schema::MinimapRacerEntry;
using Color = schema::MinimapColor;
using RaceData = schema::RaceData;
using ModelList = schema::ModelList;

constexpr uint32_t kFrame = 28336;
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
void Identity(uint32_t at) {
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t col = 0; col < 4; col++) {
      Store(at + 16 * row + 4 * col, row == col ? 0x3F800000u : 0u);
    }
  }
}

class Drawer {
 public:
  Drawer(const DrawCalls& calls, uint32_t f, uint32_t caller_r24)
      : calls_(calls), f_(f), caller_r24_(caller_r24) {}
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
    if (!Scaling(out, vector)) {
      Call(kScaling, {out, vector}, lr);
    }
    FlushOff();
  }
  void Translate(uint32_t out, uint32_t vector) {
    car_physics::MatrixMultiplyTranslation(out, out, vector, f_ - 192);
    FlushOff();
  }
  void Map(uint32_t set);
  void Points(uint32_t set, uint32_t pass);
  void CarMarkers(uint32_t pass);

  const DrawCalls& calls_;
  uint32_t f_;
  uint32_t caller_r24_;
  double one_ = 0, zero_ = 0;
};

void Drawer::Run(uint32_t manager, uint32_t description) {
  const uint32_t f = f_;
  const schema::SceneryPassDesc desc(description);
  const uint32_t set = schema::Scenery(manager).course_set().address;  // r26
  const uint32_t pass =
      desc.camera_pass() * 592u + schema::CamerasGlobals::passes_address();  // r14
  const schema::CameraPass camera(pass);
  one_ = double(G::one());    // f31
  zero_ = double(G::zero());  // f30
  // The compiler's dead stores (overwritten before they're read).
  for (uint32_t at = 364; at <= 476; at += 16) {
    Stfs(f + at, one_);
  }
  if (ModelList(schema::CourseSet(set).list_b_address()).count() == 0) {
    return;
  }
  // The identity at +112, copied to +352; the pass's view-projection at
  // +416 and view at 0x8265ECA0 (both copies noted for interpolation), the
  // keys.
  Identity(f + 112);
  for (uint32_t at = 0; at < 64; at += 4) {
    Store(f + 352 + at, Load(f + 112 + at));
  }
  matrix_math::Copy(f + 416, camera.view_projection_address());
  interpolation::BlendState_OnCopy(f + 416, camera.view_projection_address(), 0x820F5820);
  FlushOff();
  const uint32_t key_b = desc.key_b();
  const uint32_t key_a = desc.key_a();
  G::set_draw_key_a(key_a);
  G::set_draw_key_b(key_b);
  matrix_math::Copy(G::view_copy_address(), camera.view_address());
  interpolation::BlendState_OnCopy(G::view_copy_address(), camera.view_address(),
                                   0x820F5848);
  FlushOff();
  Map(set);
  Points(set, pass);
  CarMarkers(pass);
}

// The first n models of the set's list at +1064, each shape drawn.
void Drawer::Map(uint32_t set) {
  const uint32_t f = f_;
  const ModelList list(schema::CourseSet(set).list_b_address());
  int32_t count = int32_t(list.count());  // r27
  const uint32_t race = G::race_data();
  if (race != 0 && int32_t(RaceData(race).kind()) == 1) {
    const uint32_t flags = RaceData(race).branch_flags();
    if (flags & 0x20) {
      count = 4;
    } else if ((flags & 0x4) && (flags & 0x8)) {
      count = 3;
    } else {
      count = (flags & 0x10) ? 2 : 1;
    }
  }
  const double far_range = double(G::range_far());    // f28
  const double near_range = double(G::range_near());  // f29
  const uint32_t nodes = list.nodes_address();  // r29
  for (int32_t i = 0; i < count; i++) {
    auto node_at = [&](uint32_t at) {
      return (uint32_t(int32_t(int16_t(guest::LoadU16(at)))) << 2) + nodes;
    };
    uint32_t node = node_at(list.head_address());
    for (uint32_t k = uint32_t(i); k != 0; k--) {
      node = node_at(node);
    }
    const int32_t slot = int16_t(guest::LoadU16(node + 2));
    const uint32_t model = Load(list.slots_address() + (uint32_t(slot) << 2));
    const schema::Model m(model);
    m.set_texture_provider(0);
    for (uint32_t j = 0; int32_t(j) < int32_t(m.shape_count()); j++) {
      Stfs(f + 256, near_range);
      Stfs(f + 260, far_range);
      Stfs(f + 264, zero_);
      Stfs(f + 268, zero_);
      const uint32_t frame = f - scenery_lists::kSceneryDrawShapeFrame;
      const uint32_t chain = Load(frame);
      Store(frame, f);
      const auto drawn = scenery_lists::SceneryDrawShape(*calls_.draws, frame, model,
                                                         f + 416, f + 352, j, 0, 0, f + 256, 0,
                                                         caller_r24_);
      FlushOff();
      if (!drawn) {
        Store(frame, chain);
        Call(kDrawShape, {model, f + 416, f + 352, j, 0, 0, f + 256, 0}, 0x820F5960);
      }
    }
  }
}

// The point markers: points 0 to 2 of each accepted entry in one color,
// point 3 in another, each above -1000.0 put on the ground.
void Drawer::Points(uint32_t set, uint32_t pass) {
  const uint32_t f = f_;
  const uint32_t list = schema::CourseSet(set).points_address();  // r31
  const ModelList points(list);
  if (int32_t(points.point_group_count()) <= 0) {
    return;
  }
  const double lowest = double(G::lowest());  // f29
  // The scale vector (s, 1.0, s, 1.0) at `scale`.
  auto scale_vector = [&](uint32_t scale) {
    const double s = double(G::marker_scale());
    Stfs(scale + 0, s);
    Stfs(scale + 4, one_);
    Stfs(scale + 8, s);
    Stfs(scale + 12, one_);
  };
  auto place = [&](uint32_t entry, uint32_t point) {
    SetListPoint(list, f + 176, entry, point, 0);
    if (Lfs(f + 180) < lowest) {
      return;
    }
    Stfs(f + 180, zero_);
    Translate(f + 288, f + 176);
    const uint32_t frame = f - kPointMarkerFrame;
    Store(frame, f);  // its back chain, as stwu
    DrawPointMarker(pass, f + 192, f + 288, frame, *calls_.point_marker);
    FlushOff();
  };
  for (uint32_t i = 0; int32_t(i) < int32_t(points.point_group_count()); i++) {
    if (scenery_lists::PointListIsEntryOn(int32_t(i)) == 0) {
      continue;
    }
    Stfs(f + 188, one_);
    Stfs(f + 300, one_);
    Store(f + 200, 255);
    Stfs(f + 316, one_);
    Store(f + 204, 255);
    Stfs(f + 332, one_);
    Stfs(f + 348, one_);
    Store(f + 192, 168);
    Store(f + 196, 124);
    for (uint32_t point = 0; point < 3; point++) {
      scale_vector(f + 224);
      Scale(f + 288, f + 224, 0x820F5A04);
      place(i, point);
    }
    Store(f + 104, 255);
    Store(f + 108, 255);
    Store(f + 96, 103);
    Store(f + 100, 252);
    scale_vector(f + 240);
    for (uint32_t w = 0; w < 16; w += 4) {  // the color to +192 (two ld/std)
      Store(f + 192 + w, Load(f + 96 + w));
    }
    Scale(f + 288, f + 240, 0x820F5AA8);
    place(i, 3);
  }
}

// The car markers: the five types' colors made once, five pools of markers
// sorted by type and the blink phase, drawn pool by pool; the blink clock.
void Drawer::CarMarkers(uint32_t pass) {
  const uint32_t f = f_;
  const uint32_t guard = G::type_colors_guard();
  if ((guard & 1) == 0) {
    G::set_type_colors_guard(guard | 1);
    static constexpr uint32_t kTable[20] = {255, 241, 0,   255, 255, 66,  23,  255, 0,  255,
                                            255, 255, 255, 80,  255, 255, 34,  172, 54, 255};
    for (uint32_t i = 0; i < 20; i++) {
      Store(G::type_colors_address() + 4 * i, kTable[i]);
    }
    c_runtime::AtexitFrom(kColorsCleanUp, calls_.atexit);
    FlushOff();
  }
  for (uint32_t k = 0; k < 5; k++) {
    c_runtime::VectorConstruct(f + 1056 + kPoolBytes * k, 64, 80, kSetWsToOne, nullptr,
                               nullptr);
  }
  for (uint32_t k = 0; k < 5; k++) {
    const uint32_t at = f + 712 + kPoolBytes * k;  // the original's r11
    Store(at - 24, 0xFFFFFFFFu);
    Store(at - 8, 0);
    Store(at - 4, 0);
    Store(at + 0, 0);
    Store(at + 4, 1);
    const List l(at + 8);  // the pool's list
    const uint32_t nodes = l.nodes_address();
    for (uint32_t n = 0; n < 80; n++) {
      guest::StoreU16(nodes + 4 * n + 2, uint16_t(n));
      guest::StoreU16(nodes + 4 * n + 0, uint16_t(n + 1));
    }
    guest::StoreU16(nodes + 316, 0xFFFF);  // node 79's next
    l.set_head(0xFFFF);
    l.set_unknown_326(0xFFFF);
    l.set_free_head(0);
    l.set_unknown_322(0xFFFF);
    l.set_count(0);
  }

  if (vehicle_accessors::VehicleGet(0) != 0) {
    uint32_t manager = G::racers_manager();
    // The player's branch number: byte +3 of its section.
    const uint32_t player = RaceFindPlayerEntry(manager, 1);
    Store(f + 80, Entry(player).position());
    const uint32_t section = uint32_t(guest::LoadU16(f + 80)) << 2;
    const uint32_t sections = schema::TrackRouteFile(G::route()).sections();
    const int32_t branch =
        schema::TrackRouteSection(Load(section + sections)).branch();  // r20
    const double quarter = double(G::quarter_turn());  // f29
    for (uint32_t i = 0; int32_t(i) < int32_t(schema::RaceManager(manager).entry_count());
         i++) {
      const uint32_t entry =
          schema::RaceManager(manager).entries_address() + 28 * i;  // r27
      const uint32_t flags = Entry(entry).flags();
      uint32_t vehicle = 0;  // r28
      int32_t type = 0;      // r30
      if (flags & 0xC) {
        const bool human = (flags & 0x4) == 0;  // the 0x8 entries'
        vehicle = RaceRacerVehicle(i);
        const uint32_t race = G::race_data();
        if (int32_t(RaceData(race).kind()) == 6) {
          const uint32_t stage = f + 544;
          if (!debug::IsHidden(kStagesCopy)) {
            debug::FrameStateDiff_Before();
            const bool copied =
                vehicle_accessors::StagesCopy(stage, RaceData(race).stages_address(),
                                              0xFFFFFFFFu);
            FlushOff();
            debug::FrameStateDiff_After("draw step part", kStagesCopy);
            if (!copied) {
              Call(kStagesCopy,
                   {stage, RaceData(race).stages_address(), 0xFFFFFFFFFFFFFFFFull},
                   human ? 0x820F5D6C : 0x820F5D14);
            }
          }
          if (int32_t(Load(stage)) != 1) {
            const uint32_t humans = G::human_flag();
            if (human ? humans != 0 : humans == 0) {
              type = 1;
            }
          }
        }
      } else if ((flags & 0x1) != 0 && (flags & 0x20) == 0) {
        const auto marker_type = RacerMarkerType(entry, branch);
        type = marker_type ? *marker_type
                           : int32_t(Call(kRacerMarkerType, {entry, uint64_t(int64_t(branch))},
                                          0x820F5DA4));
        if (type == -1) {
          manager = G::racers_manager();
          continue;
        }
      } else {
        continue;
      }
      // Its pool by the type and the blink phase.
      const int32_t sum = type + int32_t(G::blink_phase());
      const int32_t index = sum - (sum / 5) * 5;
      const uint32_t pool = f + kPools + kPoolBytes * uint32_t(index);
      if (int32_t(Load(pool)) == -1) {
        Store(pool, uint32_t(type));
        const Color from((uint32_t(type) << 4) + G::type_colors_address());
        const Color to(pool + 16);
        to.set_red(from.red());
        to.set_green(from.green());
        to.set_blue(from.blue());
        to.set_alpha(from.alpha());
      }
      // The marker's matrix at +112.
      for (uint32_t at : {124u, 140u, 156u, 172u}) {
        Stfs(f + at, one_);
      }
      Identity(f + 112);
      if (vehicle != 0) {
        const uint32_t vehicle_matrix = schema::Vehicle(vehicle).matrix_3312_address();
        if (!native::Buffers_Memcpy(f + 480, vehicle_matrix, 64, 0x820F5E6C)) {
          calls_.calls->memcpy(f + 480, vehicle_matrix, 64, f, 0x820F5E6C);
        }
        FlushOff();
        const double s = double(G::marker_scale());
        Stfs(f + 532, zero_);
        Stfs(f + 272, s);
        Stfs(f + 276, one_);
        Stfs(f + 280, s);
        Stfs(f + 284, one_);
        Scale(f + 112, f + 272, 0x820F5E90);
        Stfs(f + 80, quarter);
        matrix_math::RotateY(f + 112, f + 112, f + 80, f - 80);
        FlushOff();
        matrix_math::Multiply(f + 112, f + 112, f + 480);
        interpolation::BlendState_OnMultiply(f + 112, f + 112, f + 480, 0x820F5EB4);
        FlushOff();
      } else {
        const uint32_t sections_now = schema::TrackRouteFile(G::route()).sections();
        const uint32_t at_section = uint32_t(Entry(entry).section()) << 2;
        const int32_t at_slice = int16_t(Entry(entry).slice());
        const schema::TrackRouteSlice slice(schema::TrackRouteSection(Load(at_section + sections_now))
                           .slices_address() +
                       uint32_t(at_slice) * 84u);
        const double angle =
            c_runtime::Atan2(double(slice.forward_x()), double(slice.forward_z()));
        FlushOff();
        Stfs(f + 96, double(slice.center_x()));
        Stfs(f + 104, double(slice.center_z()));
        const double s = double(G::marker_scale());
        Stfs(f + 208, s);
        Stfs(f + 216, s);
        Stfs(f + 108, one_);
        Stfs(f + 100, zero_);
        Stfs(f + 212, one_);
        Stfs(f + 220, one_);
        Stfs(f + 80, Frsp(angle));
        Scale(f + 112, f + 208, 0x820F5F28);
        matrix_math::RotateY(f + 112, f + 112, f + 80, f - 80);
        FlushOff();
        Translate(f + 112, f + 96);
      }
      if (List(pool + 32).count() < 80) {
        const uint32_t list = pool + 32;
        List80Insert(list, f + 112, uint32_t(int32_t(int16_t(List(list).count()))));
        FlushOff();
      }
      manager = G::racers_manager();
    }
  }

  // Each pool's markers, in order.
  for (uint32_t k = 0; k < 5; k++) {
    const uint32_t list = f + 720 + kPoolBytes * k;  // r31
    if (int32_t(Load(list - 32)) == -1) {
      continue;
    }
    for (uint32_t j = 0; j < uint32_t(int32_t(int16_t(List(list).count()))); j++) {
      if (!List80Copy(f + 624, list, j)) {
        Call(kList80Copy, {f + 624, list, j}, 0x820F5FBC);
      }
      FlushOff();
      const uint32_t frame = f - kCarMarkerFrame;
      Store(frame, f);  // its back chain, as stwu
      DrawCarMarker(pass, list - 16, f + 624, frame, *calls_.car_marker);
      FlushOff();
    }
  }

  // The blink clock.
  const uint32_t frames = G::blink_frames() + 1;
  G::set_blink_frames(frames);
  if (frames > 4) {
    const uint32_t phase = G::blink_phase() + 1;
    G::set_blink_frames(0);
    G::set_blink_phase(phase);
    if (int32_t(phase) > 5) {
      G::set_blink_phase(0);
    }
  }
  // The pools' destructor iterator: the empty destructor, nothing.
  for (uint32_t k = 5; k-- > 0;) {
    c_runtime::VectorDestruct(f + 1056 + kPoolBytes * k, 64, 80, kEmptyDestructor,
                              nullptr, nullptr);
  }
}

}  // namespace

void Draw(const DrawCalls& calls, uint32_t stack, uint32_t manager,
          uint32_t description, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820F5740 Minimap::Draw");
  const uint32_t f = stack - kFrame;
  Store(f, stack);  // the back chain, as stwu (its stack probe left out)
  Drawer(calls, f, caller_r24).Run(manager, description);
}

}  // namespace minimap
