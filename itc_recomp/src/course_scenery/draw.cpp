// Scenery::Draw (sub_820F6068, course_scenery.h;
// docs/functions/course-scenery.md): the course scenery's draw for every
// camera pass of the pass table at 0x82603060, step for step as the
// translated code (built with strict floats, CMakeLists.txt). Its 640-byte
// frame keeps the original's temporaries at their offsets (the pass
// description at +80, the identity at +128, the world matrices at +208 and
// +272, the view-projections at +336 and +400, the range at +192: its
// callees read them by guest address and frame interpolation follows the
// copies); its callees' plain C++ runs with the registers their adapters
// would hand the interpolation's hooks.

#include "course_scenery/course_scenery.h"

#include "car_scene_draw/car_scene_draw.h"
#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "game_data/data_file.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "model_animation/model_animation.h"
#include "scenery_lists/scenery_lists.h"
#include "schema/camera_modes.h"
#include "schema/cameras.h"
#include "schema/course_scenery.h"
#include "schema/models.h"
#include "schema/scenery_lists.h"
#include "sprites/billboards.h"

#include <initializer_list>
#include <utility>

namespace course_scenery {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kFrame = 640;
// The view copy, also the original's r24 throughout.
constexpr uint32_t kViewCopy = schema::SceneryListsGlobals::view_address();
// The pass table: 20 entries of six words (the player, the viewport entry,
// the camera pass, the two keys, the mask): 0 to 2 the course set's model,
// 3 to 5 its list at +528, 6 to 15 and 16 to 18 the areas, 16 to 18 the
// set's lists and light groups, 19 the minimap.
constexpr uint32_t kProviderClearAll = 0x82594328;
constexpr uint32_t kProviderSet = 0x82594718;
constexpr uint32_t kModelAnimate = 0x82594B60;
constexpr uint32_t kModelDrawShape = 0x82596B18;
constexpr uint32_t kDrawShape = 0x82147D88;

void FlushOff() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

bool ShadowPass(int32_t type) { return type == 10 || type == 13 || type == 16; }

using Manager = schema::Scenery;
using VisibleArea = schema::SceneryVisibleArea;
using G = schema::CourseSceneryGlobals;
using Pass = schema::CameraPass;
using Entry = schema::SceneryPassEntry;
using Desc = schema::SceneryPassDesc;
using Shape = schema::SceneryModel;

// The camera pass `index` (592 bytes each from 0x826BAC00).
Pass PassAt(uint32_t index) {
  return Pass(index * Pass::kSize + schema::CamerasGlobals::passes_address());
}
// The pass table's entry `entry` (G::pass_table).
Entry EntryAt(uint32_t entry) {
  return Entry(G::pass_table_address() + Entry::kSize * entry);
}

}  // namespace

void Draw(const scenery_lists::DrawCalls& draws,
          const car_scene_draw::SceneDrawCalls& calls, const DrawSteps& steps, uint32_t stack,
          uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_820F6068 Scenery::Draw");
  const uint32_t f = stack - kFrame;
  Store(f, stack);  // the back chain, as stwu
  const uint32_t description = f + 80;
  const Desc desc(description);
  const Manager m(manager);
  const uint32_t provider = m.texture_provider_address();  // r27
  const schema::CourseSet s = m.course_set();              // r15
  const uint32_t set = s.address;
  const uint32_t textures = s.xpr_address();  // r18
  // The visible areas (the manager's +216, r16): area `a`.
  auto area_at = [&](uint32_t a) {
    return VisibleArea(m.visible_areas().address + VisibleArea::kSize * a);
  };

  auto guest_call = [&](uint32_t function, std::initializer_list<uint32_t> args, uint32_t lr) {
    draws.call(draws.user, function, args.begin(), int(args.size()), f, lr);
    FlushOff();
  };
  // The texture provider's slots 2 and 4 (its plain C++ where they are the
  // game's, else guest calls); the vtable read again for each.
  auto provider_clear = [&](uint32_t lr) {
    const uint32_t function = m.texture_provider().clear_all();
    if (function == kProviderClearAll) {
      scenery_lists::TextureProviderClearAll(provider);
    } else {
      guest_call(function, {provider}, lr);
    }
  };
  auto provider_set = [&](uint32_t slot, uint32_t lr) {
    const uint32_t function = m.texture_provider().set();
    if (function != kProviderSet ||
        !scenery_lists::TextureProviderSet(provider, slot, textures)) {
      guest_call(function, {provider, slot, textures}, lr);
    }
  };
  // The description's +12 to +32 from table entry `entry`, word by word.
  auto describe = [&](uint32_t entry) {
    const uint32_t at = EntryAt(entry).address;
    for (uint32_t i = 0; i < 6; i++) {
      Store(desc.player_address() + 4 * i, Load(at + 4 * i));
    }
  };
  auto enabled = [&](uint32_t entry) {
    return PassAt(EntryAt(entry).camera_pass()).enabled() != 0;
  };
  auto copy = [&](uint32_t to, uint32_t from, uint32_t lr) {
    matrix_math::Copy(to, from);
    interpolation::BlendState_OnCopy(to, from, lr);
    FlushOff();
  };
  // The identity at +128 and its copy at `world` (ld and std).
  auto identity = [&](uint32_t world) {
    for (uint32_t row = 0; row < 4; row++) {
      for (uint32_t col = 0; col < 4; col++) {
        Store(f + 128 + 16 * row + 4 * col, row == col ? 0x3F800000u : 0u);
      }
    }
    for (uint32_t at = 0; at < 64; at += 4) {
      Store(world + at, Load(f + 128 + at));
    }
  };
  auto headlights = [&]() {
    RecordHeadlightProjectors(calls, f, description);
    FlushOff();
  };

  // The texture provider cleared; with +196's 0x4000 clear, the texture
  // animations advanced (inside their per-frame wrap) and every visible
  // area's models animated.
  provider_clear(0x820F60A0);
  if ((~m.mask() & 0x4000) != 0) {
    steps.advance(f, set);
    FlushOff();
    for (uint32_t a = 0; a < m.visible_count(); a++) {
      const VisibleArea area = area_at(a);
      if (area.model_count() == 0) {
        continue;
      }
      for (uint32_t n = 0; n < area.model_count(); n++) {
        const uint32_t model =
            schema::AreaModel(Load(area.models_address() + 4 * n)).model().address;
        if (steps.native_model_animation) {
          model_animation::ModelAnimate(model);
        } else {
          guest_call(kModelAnimate, {model}, 0x820F60F4);
        }
        FlushOff();
      }
    }
  }
  provider_set(0, 0x820F613C);

  const double one = double(G::one());  // f31
  // The course set's model (+96) with the eye's translation, +196's 0x10.
  if ((m.mask() & 0x10) != 0) {
    const double zero = double(G::zero());  // f30
    const double range_y = double(G::range_y());  // f28
    const double range_x = double(G::range_x());  // f29
    for (uint32_t entry = 0; entry < 3; entry++) {
      if (!enabled(entry)) {
        continue;
      }
      const Pass pass = PassAt(EntryAt(entry).camera_pass());
      for (uint32_t at : {220u, 236u, 252u, 268u, 348u, 364u, 380u, 396u}) {
        Stfs(f + at, one);
      }
      const uint32_t model = s.model_address();  // r29
      if (model == 0) {
        break;  // the original's addic. test (the set at -96)
      }
      describe(entry);
      headlights();
      const uint32_t world = f + 208, view_projection = f + 336;
      identity(world);
      Stfs(f + 256, double(pass.eye_x()));
      Stfs(f + 260, double(pass.eye_y()));
      Stfs(f + 264, double(pass.eye_z()));
      copy(view_projection, pass.view_projection_address(), 0x820F628C);
      provider_set(1, 0x820F62A8);
      G::set_key_a(desc.key_a());
      G::set_key_b(desc.key_b());
      copy(kViewCopy, pass.view_address(), 0x820F62C4);
      schema::Model(model).set_texture_provider(provider);
      for (uint32_t i = 0; int32_t(i) < int32_t(schema::Model(model).shape_count()); i++) {
        const uint32_t range = f + 192;
        Stfs(range + 0, range_x);
        Stfs(range + 4, range_y);
        Stfs(range + 8, zero);
        Stfs(range + 12, zero);
        const uint32_t frame = f - scenery_lists::kModelDrawShapeFrame;
        const uint32_t chain = Load(frame);
        Store(frame, f);
        const auto drawn = scenery_lists::ModelDrawShape(draws, frame, model,
                                                         view_projection, world, i, 0, 0, range,
                                                         kViewCopy);
        FlushOff();
        if (!drawn) {
          Store(frame, chain);
          guest_call(kModelDrawShape, {model, view_projection, world, i, 0, 0, range},
                     0x820F6308);
        }
      }
    }
  }

  // The course set's list at +528, +196's 0x20: each model nearer than the
  // viewport entry's far distance (+72) drawn with the identity.
  if ((m.mask() & 0x20) != 0) {
    for (uint32_t entry = 3; entry < 6; entry++) {
      if (!enabled(entry)) {
        continue;
      }
      const Pass pass = PassAt(EntryAt(entry).camera_pass());
      for (uint32_t at : {284u, 300u, 316u, 332u, 412u, 428u, 444u, 460u}) {
        Stfs(f + at, one);
      }
      const uint32_t list = s.list_a_address();  // r26
      if (list == 0) {
        break;  // the original's addic. test (the set at -528)
      }
      describe(entry);
      headlights();
      const uint32_t world = f + 272, view_projection = f + 400;
      identity(world);
      copy(view_projection, pass.view_projection_address(), 0x820F6424);
      provider_set(1, 0x820F6440);
      G::set_key_a(desc.key_a());
      G::set_key_b(desc.key_b());
      copy(kViewCopy, pass.view_address(), 0x820F645C);
      const Entry row = EntryAt(entry);
      for (uint32_t i = 0; int32_t(i) < int32_t(schema::ModelList(list).count()); i++) {
        const uint32_t model = ListModel(list + 4, i);
        scenery_lists::ResetDrawState(model);
        const schema::ViewportEntry viewport(Load(schema::CamerasGlobals::viewports().first_address() +
                                 4 * row.player()) +
                      schema::ViewportEntry::kSize * row.viewport_entry());
        // The model's first shape's +236 (not in models.toml's ModelShape).
        if (Lfs(Shape(model).shapes().address + 236) > double(viewport.far())) {
          continue;
        }
        Shape(model).set_texture_provider(provider);
        const uint32_t frame = f - scenery_lists::kSceneryDrawShapeFrame;
        const uint32_t chain = Load(frame);
        Store(frame, f);
        const auto drawn = scenery_lists::SceneryDrawShape(draws, frame, model, view_projection, world, 0, 0, 0, 0, 0, kViewCopy);
        FlushOff();
        if (!drawn) {
          Store(frame, chain);
          guest_call(kDrawShape, {model, view_projection, world, 0, 0, 0, 0, 0}, 0x820F650C);
        }
      }
    }
  }

  // Each visible area for entries 6 to 15, then 16 to 18 (the original's
  // r28, the area's offset, its depth draws' context).
  for (const auto& [first, count] : {std::pair{6u, 10u}, std::pair{16u, 3u}}) {
    for (uint32_t a = 0; a < m.visible_count(); a++) {
      const uint32_t area = area_at(a).address;
      if (VisibleArea(area).model_count() == 0) {
        continue;
      }
      for (uint32_t entry = first; entry < first + count; entry++) {
        if (!enabled(entry)) {
          continue;
        }
        describe(entry);
        steps.set_r24(PassAt(desc.camera_pass()).address);
        DrawArea(draws, calls, f, manager, description, area, VisibleArea::kSize * a);
        steps.set_r24(kViewCopy);
        FlushOff();
      }
    }
  }

  // The set's lists for entries 16 to 18.
  for (uint32_t entry = 16; entry < 19; entry++) {
    if (!enabled(entry)) {
      continue;
    }
    describe(entry);
    steps.set_r24(DrawSetListsViewProjection(description));
    DrawSetLists(draws, f, manager, description);
    steps.set_r24(kViewCopy);
    FlushOff();
  }

  // The set's light groups (+1592) for entries 16 to 18 outside the shadow
  // passes, +196's 0x400.
  if ((m.mask() & 0x400) != 0) {
    for (uint32_t entry = 16; entry < 19; entry++) {
      const Pass pass = PassAt(EntryAt(entry).camera_pass());
      if (pass.enabled() == 0 || ShadowPass(int32_t(pass.type()))) {
        continue;
      }
      const uint32_t lights = s.light_groups().address;  // r31
      if (lights == 0) {
        continue;
      }
      describe(entry);
      headlights();
      scenery_lists::SetBillboardPass(description, desc.camera_pass_object(), desc.key_b());
      for (uint32_t i = 0; i < __builtin_bswap32(schema::SceneryDataFile(lights).count());
           i++) {
        const uint32_t animations = s.texture_animations().address;
        const uint32_t groups = game_data::GetEntry(lights, i, false);
        sprites::DrawLightGroups(calls, f, description, groups, textures, animations);
        FlushOff();
      }
    }
  }

  // The minimap's pass (entry 19): its clear, the headlights, the minimap
  // (inside its per-frame wrap).
  if (enabled(19)) {
    steps.record_clear(f, 103, 49, PassAt(19).clear_color(), PassAt(19).clear_depth());
    FlushOff();
    describe(19);
    headlights();
    steps.minimap(f, manager, description);
    FlushOff();
  }
}

}  // namespace course_scenery
