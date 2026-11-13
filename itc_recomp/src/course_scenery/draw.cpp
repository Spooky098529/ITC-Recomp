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
#include "sprites/billboards.h"

#include <initializer_list>
#include <utility>

namespace course_scenery {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kFrame = 640;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kViewCopy = 0x8265ECA0;  // also the original's r24 throughout
constexpr uint32_t kDrawKeyA = 0x8296B90C;  // the description's +24
constexpr uint32_t kDrawKeyB = 0x8296B900;  // its +28
constexpr uint32_t kViewports = 0x826C2CE8;
// The pass table: 20 entries of six words (the player, the viewport entry,
// the camera pass, the two keys, the mask): 0 to 2 the course set's model,
// 3 to 5 its list at +528, 6 to 15 and 16 to 18 the areas, 16 to 18 the
// set's lists and light groups, 19 the minimap.
constexpr uint32_t kPassTable = 0x82603060;
constexpr uint32_t kRangeX = 0x82001FCC;
constexpr uint32_t kRangeY = 0x82006804;
constexpr uint32_t kClearColor = kCameraPasses + 11812;  // pass 19's +564
constexpr uint32_t kClearDepth = kCameraPasses + 11820;  // its +572
constexpr uint32_t kProviderClearAll = 0x82594328;
constexpr uint32_t kProviderSet = 0x82594718;
constexpr uint32_t kModelAnimate = 0x82594B60;
constexpr uint32_t kModelDrawShape = 0x82596B18;
constexpr uint32_t kDrawShape = 0x82147D88;

void FlushOff() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

bool ShadowPass(int32_t type) { return type == 10 || type == 13 || type == 16; }

}  // namespace

void Draw(uint8_t* base, const scenery_lists::DrawCalls& draws,
          const car_scene_draw::SceneDrawCalls& calls, const DrawSteps& steps, uint32_t stack,
          uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_820F6068 Scenery::Draw");
  const uint32_t f = stack - kFrame;
  Store(base, f, stack);  // the back chain, as stwu
  const uint32_t description = f + 80;
  const uint32_t provider = manager + 228;  // r27
  const uint32_t areas = manager + 216;     // r16
  const uint32_t set = Load(base, manager + 112);  // r15
  const uint32_t textures = set + 64;              // r18

  auto guest_call = [&](uint32_t function, std::initializer_list<uint32_t> args, uint32_t lr) {
    draws.call(draws.user, function, args.begin(), int(args.size()), f, lr);
    FlushOff();
  };
  // The texture provider's slots 2 and 4 (its plain C++ where they are the
  // game's, else guest calls); the vtable read again for each.
  auto provider_clear = [&](uint32_t lr) {
    const uint32_t function = Load(base, Load(base, provider) + 8);
    if (function == kProviderClearAll) {
      scenery_lists::TextureProviderClearAll(base, provider);
    } else {
      guest_call(function, {provider}, lr);
    }
  };
  auto provider_set = [&](uint32_t slot, uint32_t lr) {
    const uint32_t function = Load(base, Load(base, provider) + 16);
    if (function != kProviderSet ||
        !scenery_lists::TextureProviderSet(base, provider, slot, textures)) {
      guest_call(function, {provider, slot, textures}, lr);
    }
  };
  auto pass_of = [&](uint32_t index) { return index * 592u + kCameraPasses; };
  // The description's +12 to +32 from table entry `entry`.
  auto describe = [&](uint32_t entry) {
    const uint32_t at = kPassTable + 24 * entry;
    for (uint32_t i = 0; i < 6; i++) {
      Store(base, description + 12 + 4 * i, Load(base, at + 4 * i));
    }
  };
  auto enabled = [&](uint32_t entry) {
    return Load(base, pass_of(Load(base, kPassTable + 24 * entry + 8)) + 588) != 0;
  };
  auto copy = [&](uint32_t to, uint32_t from, uint32_t lr) {
    matrix_math::Copy(base, to, from);
    interpolation::BlendState_OnCopy(base, to, from, lr);
    FlushOff();
  };
  // The identity at +128 and its copy at `world` (ld and std).
  auto identity = [&](uint32_t world) {
    for (uint32_t row = 0; row < 4; row++) {
      for (uint32_t col = 0; col < 4; col++) {
        Store(base, f + 128 + 16 * row + 4 * col, row == col ? 0x3F800000u : 0u);
      }
    }
    for (uint32_t at = 0; at < 64; at += 4) {
      Store(base, world + at, Load(base, f + 128 + at));
    }
  };
  auto headlights = [&]() {
    RecordHeadlightProjectors(base, calls, f, description);
    FlushOff();
  };

  // The texture provider cleared; with +196's 0x4000 clear, the texture
  // animations advanced (inside their per-frame wrap) and every visible
  // area's models animated.
  provider_clear(0x820F60A0);
  if ((~Load(base, manager + 196) & 0x4000) != 0) {
    steps.advance(f, set);
    FlushOff();
    for (uint32_t a = 0; a < Load(base, areas); a++) {
      const uint32_t area = Load(base, areas + 8) + 272 * a;
      if (Load(base, area + 4) == 0) {
        continue;
      }
      for (uint32_t n = 0; n < Load(base, area + 4); n++) {
        const uint32_t model = Load(base, Load(base, area + 16 + 4 * n) + 56);
        if (steps.native_model_animation) {
          model_animation::ModelAnimate(base, model);
        } else {
          guest_call(kModelAnimate, {model}, 0x820F60F4);
        }
        FlushOff();
      }
    }
  }
  provider_set(0, 0x820F613C);

  const double one = Lfs(base, kOne);  // f31
  // The course set's model (+96) with the eye's translation, +196's 0x10.
  if ((Load(base, manager + 196) & 0x10) != 0) {
    const double zero = Lfs(base, kZero);     // f30
    const double range_y = Lfs(base, kRangeY);  // f28
    const double range_x = Lfs(base, kRangeX);  // f29
    for (uint32_t entry = 0; entry < 3; entry++) {
      if (!enabled(entry)) {
        continue;
      }
      const uint32_t pass = pass_of(Load(base, kPassTable + 24 * entry + 8));
      for (uint32_t at : {220u, 236u, 252u, 268u, 348u, 364u, 380u, 396u}) {
        Stfs(base, f + at, one);
      }
      const uint32_t model = set + 96;  // r29
      if (model == 0) {
        break;  // the original's addic. test (the set at -96)
      }
      describe(entry);
      headlights();
      const uint32_t world = f + 208, view_projection = f + 336;
      identity(world);
      Stfs(base, f + 256, Lfs(base, pass + 400));
      Stfs(base, f + 260, Lfs(base, pass + 404));
      Stfs(base, f + 264, Lfs(base, pass + 408));
      copy(view_projection, pass + 144, 0x820F628C);
      provider_set(1, 0x820F62A8);
      Store(base, kDrawKeyA, Load(base, f + 104));
      Store(base, kDrawKeyB, Load(base, f + 108));
      copy(kViewCopy, pass + 16, 0x820F62C4);
      Store(base, model + 48, provider);
      for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, model + 412)); i++) {
        const uint32_t range = f + 192;
        Stfs(base, range + 0, range_x);
        Stfs(base, range + 4, range_y);
        Stfs(base, range + 8, zero);
        Stfs(base, range + 12, zero);
        const uint32_t frame = f - scenery_lists::kModelDrawShapeFrame;
        const uint32_t chain = Load(base, frame);
        Store(base, frame, f);
        const auto drawn = scenery_lists::ModelDrawShape(base, draws, frame, model,
                                                         view_projection, world, i, 0, 0, range,
                                                         kViewCopy);
        FlushOff();
        if (!drawn) {
          Store(base, frame, chain);
          guest_call(kModelDrawShape, {model, view_projection, world, i, 0, 0, range},
                     0x820F6308);
        }
      }
    }
  }

  // The course set's list at +528, +196's 0x20: each model nearer than the
  // viewport entry's far distance (+72) drawn with the identity.
  if ((Load(base, manager + 196) & 0x20) != 0) {
    for (uint32_t entry = 3; entry < 6; entry++) {
      if (!enabled(entry)) {
        continue;
      }
      const uint32_t pass = pass_of(Load(base, kPassTable + 24 * entry + 8));
      for (uint32_t at : {284u, 300u, 316u, 332u, 412u, 428u, 444u, 460u}) {
        Stfs(base, f + at, one);
      }
      const uint32_t list = set + 528;  // r26
      if (list == 0) {
        break;  // the original's addic. test (the set at -528)
      }
      describe(entry);
      headlights();
      const uint32_t world = f + 272, view_projection = f + 400;
      identity(world);
      copy(view_projection, pass + 144, 0x820F6424);
      provider_set(1, 0x820F6440);
      Store(base, kDrawKeyA, Load(base, f + 104));
      Store(base, kDrawKeyB, Load(base, f + 108));
      copy(kViewCopy, pass + 16, 0x820F645C);
      const uint32_t row = kPassTable + 24 * entry;
      for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, list)); i++) {
        const uint32_t model = ListModel(base, list + 4, i);
        scenery_lists::ResetDrawState(base, model);
        const uint32_t viewport = Load(base, Load(base, kViewports) + 4 * Load(base, row)) +
                                  80 * Load(base, row + 4);
        if (Lfs(base, Load(base, model + 40) + 236) > Lfs(base, viewport + 72)) {
          continue;
        }
        Store(base, model + 48, provider);
        const uint32_t frame = f - scenery_lists::kSceneryDrawShapeFrame;
        const uint32_t chain = Load(base, frame);
        Store(base, frame, f);
        const auto drawn = scenery_lists::SceneryDrawShape(
            base, draws, frame, model, view_projection, world, 0, 0, 0, 0, 0, kViewCopy);
        FlushOff();
        if (!drawn) {
          Store(base, frame, chain);
          guest_call(kDrawShape, {model, view_projection, world, 0, 0, 0, 0, 0}, 0x820F650C);
        }
      }
    }
  }

  // Each visible area for entries 6 to 15, then 16 to 18 (the original's
  // r28, the area's offset, its depth draws' context).
  for (const auto& [first, count] : {std::pair{6u, 10u}, std::pair{16u, 3u}}) {
    for (uint32_t a = 0; a < Load(base, areas); a++) {
      const uint32_t area = Load(base, areas + 8) + 272 * a;
      if (Load(base, area + 4) == 0) {
        continue;
      }
      for (uint32_t entry = first; entry < first + count; entry++) {
        if (!enabled(entry)) {
          continue;
        }
        describe(entry);
        steps.set_r24(Load(base, description + 20) * 592u + kCameraPasses);
        DrawArea(base, draws, calls, f, manager, description, area, 272 * a);
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
    steps.set_r24(DrawSetListsViewProjection(base, description));
    DrawSetLists(base, draws, f, manager, description);
    steps.set_r24(kViewCopy);
    FlushOff();
  }

  // The set's light groups (+1592) for entries 16 to 18 outside the shadow
  // passes, +196's 0x400.
  if ((Load(base, manager + 196) & 0x400) != 0) {
    for (uint32_t entry = 16; entry < 19; entry++) {
      const uint32_t pass = pass_of(Load(base, kPassTable + 24 * entry + 8));
      if (Load(base, pass + 588) == 0 || ShadowPass(int32_t(Load(base, pass + 584)))) {
        continue;
      }
      const uint32_t lights = Load(base, set + 1592);  // r31
      if (lights == 0) {
        continue;
      }
      describe(entry);
      headlights();
      scenery_lists::SetBillboardPass(base, description, Load(base, f + 88), Load(base, f + 108));
      for (uint32_t i = 0; i < __builtin_bswap32(Load(base, lights)); i++) {
        const uint32_t animations = Load(base, set + 1060);
        const uint32_t groups = game_data::GetEntry(base, lights, i, false);
        sprites::DrawLightGroups(base, calls, f, description, groups, textures, animations);
        FlushOff();
      }
    }
  }

  // The minimap's pass (entry 19): its clear, the headlights, the minimap
  // (inside its per-frame wrap).
  if (enabled(19)) {
    steps.record_clear(f, 103, 49, Load(base, kClearColor), Load(base, kClearDepth));
    FlushOff();
    describe(19);
    headlights();
    steps.minimap(f, manager, description);
    FlushOff();
  }
}

}  // namespace course_scenery
