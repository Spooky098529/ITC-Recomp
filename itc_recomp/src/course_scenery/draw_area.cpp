// Scenery::DrawArea (sub_820F3B70, course_scenery.h;
// docs/functions/course-scenery.md): a visible area's models and light
// groups for one pass, step for step as the translated code (built with
// strict floats, CMakeLists.txt). Its 400-byte frame keeps the original's
// temporaries at their offsets (the identity at +80 and +208, the camera
// pass's view-projection at +144: its callees read them by guest address
// and frame interpolation follows the copy); its callees' plain C++ runs
// with the registers their adapters would hand the interpolation's hooks.

#include "course_scenery/course_scenery.h"

#include "car_scene_draw/car_scene_draw.h"
#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "game_data/data_file.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "post_effects/post_effects.h"
#include "scenery_lists/scenery_lists.h"
#include "sprites/billboards.h"

#include <array>
#include <initializer_list>

namespace course_scenery {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kFrame = 400;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kViewCopy = 0x8265ECA0;
constexpr uint32_t kDepthKey = 0x8296B908;    // 163 here
constexpr uint32_t kDrawKeyA = 0x8296B90C;    // the description's +24
constexpr uint32_t kDrawKeyB = 0x8296B900;    // its +28
constexpr uint32_t kShadowKeyA = 0x8296B8F8;  // +28 in the shadow passes, 116 or 119 after
constexpr uint32_t kShadowKeyB = 0x8296B8FC;  // +24 in the shadow passes, 116 or 119 after
constexpr uint32_t kTextureProviderSet = 0x82594718;
constexpr uint32_t kDrawShape = 0x82147D88;
constexpr uint32_t kDrawShapeDepth = 0x82596D48;
constexpr uint32_t kDepthFrame = 160;  // Model::DrawShapeDepth's
constexpr uint32_t kDepthContextReturn = 0x82596D9C;
constexpr uint32_t kDepthSubmitReturn = 0x82596E60;

void FlushOff() { Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask)); }

bool ShadowPass(int32_t type) { return type == 10 || type == 13 || type == 16; }

}  // namespace

void DrawArea(uint8_t* base, const scenery_lists::DrawCalls& draws,
              const car_scene_draw::SceneDrawCalls& calls, uint32_t stack, uint32_t manager,
              uint32_t description, uint32_t area, uint32_t caller_r28) {
  ITC_NATIVE_RUNNING("sub_820F3B70 Scenery::DrawArea");
  const uint32_t f = stack - kFrame;
  Store(base, f, stack);  // the back chain, as stwu
  const uint32_t set = Load(base, manager + 112);  // r18
  if ((Load(base, description + 32) & Load(base, manager + 196) & Load(base, area)) == 0) {
    return;
  }
  RecordHeadlightProjectors(base, calls, f, description);
  FlushOff();

  // The identity at +80 (1.0 stored in +144's w column first) and its copy
  // at +208, the camera pass's view-projection at +144 and its view at
  // 0x8265ECA0 (both copies noted for interpolation), the keys.
  const uint32_t pass = Load(base, description + 20) * 592u + kCameraPasses;  // r24
  const double one = Lfs(base, kOne);
  for (uint32_t at : {156u, 172u, 188u, 204u}) {
    Stfs(base, f + at, one);
  }
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t col = 0; col < 4; col++) {
      Store(base, f + 80 + 16 * row + 4 * col, row == col ? 0x3F800000u : 0u);
    }
  }
  for (uint32_t at = 0; at < 64; at += 4) {
    Store(base, f + 208 + at, Load(base, f + 80 + at));
  }
  const uint32_t view_projection = f + 144, world = f + 208;
  matrix_math::Copy(base, view_projection, pass + 144);
  interpolation::BlendState_OnCopy(base, view_projection, pass + 144, 0x820F3C3C);
  FlushOff();
  Store(base, kDepthKey, 163);
  const uint32_t key_a = Load(base, description + 24);
  const uint32_t key_b = Load(base, description + 28);
  Store(base, kDrawKeyA, key_a);
  Store(base, kDrawKeyB, key_b);
  matrix_math::Copy(base, kViewCopy, pass + 16);
  interpolation::BlendState_OnCopy(base, kViewCopy, pass + 16, 0x820F3C70);
  FlushOff();

  // Model::DrawShapeDepth: natively (its queue entry handed over from its
  // frame), else its adapter; the context its wrapper keeps has the
  // caller's r28 (`depth_r28`).
  auto draw_depth = [&](uint32_t model, uint32_t shape, uint32_t mask, uint32_t depth_r28,
                        uint32_t lr) {
    const uint32_t frame = f - kDepthFrame;
    const std::array<uint32_t, 9> context = {kDepthContextReturn, 1, mask, 0, shape,
                                             depth_r28, model, world, view_projection};
    const auto draw = scenery_lists::DrawShapeDepth(base, model, view_projection, world, shape,
                                                    0, mask, 1, frame, context);
    FlushOff();
    if (!draw) {
      // Its original (never seen in play: a chunk open on another list, or
      // no room), its recorder's context then with ctx's r28.
      const uint32_t args[7] = {model, view_projection, world, shape, 0, mask, 1};
      draws.call(draws.user, kDrawShapeDepth, args, 7, f, lr);
      FlushOff();
      return;
    }
    Store(base, frame, f);  // its back chain, as its stwu
    if (draw->submit) {
      draws.submit(draws.user, draw->entry, draw->key, draw->depth, frame, kDepthSubmitReturn);
      FlushOff();
    }
  };
  // The texture provider's slot 4 (TextureProvider::Set natively where it
  // is the game's, else a guest call).
  const uint32_t provider = manager + 228;
  auto provider_set = [&](uint32_t slot, uint32_t value, uint32_t lr) {
    const uint32_t function = Load(base, Load(base, provider) + 16);
    if (function != kTextureProviderSet ||
        !scenery_lists::TextureProviderSet(base, provider, slot, value)) {
      const uint32_t args[3] = {provider, slot, value};
      draws.call(draws.user, function, args, 3, f, lr);
    }
    FlushOff();
  };

  const uint32_t count = Load(base, area + 4);
  for (uint32_t n = 0; n < count; n++) {
    const uint32_t entry = Load(base, area + 16 + 4 * n);
    const int32_t type = int32_t(Load(base, pass + 584));
    if (ShadowPass(type)) {
      Store(base, kShadowKeyA, Load(base, description + 28));
      Store(base, kShadowKeyB, Load(base, description + 24));
      const uint32_t model = Load(base, Load(base, area + 16 + 4 * n) + 56);
      if (model == 0) {
        continue;
      }
      for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, model + 412)); i++) {
        draw_depth(model, i, 1, caller_r28, 0x820F3F44);
      }
      continue;
    }
    uint32_t slot = 1;
    const uint32_t textures = Load(base, set + 1060);
    const uint32_t model = Load(base, entry + 56);
    scenery_lists::ResetDrawState(base, model);
    if (textures != 0) {
      provider_set(1, set + 64, 0x820F3CE4);
      slot = 2;
    }
    Store(base, model + 436, textures);
    const uint32_t own = Load(base, Load(base, area + 16 + 4 * n) + 60);
    if (own != 0) {
      provider_set(slot, own, 0x820F3D14);
    }
    Store(base, model + 48, provider);
    const int32_t kind = int32_t(Load(base, pass + 584));
    const uint32_t depth_too = kind == 0 ? 1 : 0;  // r28
    const uint32_t pair_offset = (kind == 0 || kind == 1 || kind == 2) ? 0 : 28;
    for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, model + 412)); i++) {
      const uint32_t frame = f - scenery_lists::kSceneryDrawShapeFrame;
      const uint32_t chain = Load(base, frame);
      Store(base, frame, f);
      const auto drawn = scenery_lists::SceneryDrawShape(base, draws, frame, model,
                                                         view_projection, world, i, depth_too, 0,
                                                         0, pair_offset, pass);
      FlushOff();
      if (!drawn) {
        Store(base, frame, chain);
        const uint32_t args[8] = {model, view_projection, world, i, depth_too, 0, 0, pair_offset};
        draws.call(draws.user, kDrawShape, args, 8, f, 0x820F3D84);
        FlushOff();
      }
    }
    // The bloom targets' depth (types 0, 1 and 2, with post effects 3, 7
    // and 11 set and their +52 0).
    static constexpr struct {
      int32_t type;
      uint32_t effect, key, lr;
    } kDepths[3] = {{0, 3, 116, 0x820F3DF8}, {1, 7, 116, 0x820F3E6C}, {2, 11, 119, 0x820F3EE0}};
    for (const auto& d : kDepths) {
      if (int32_t(Load(base, pass + 584)) != d.type) {
        continue;
      }
      const uint32_t effect = post_effects::Get(base, d.effect);
      if (effect == 0 || Load(base, effect + 52) != 0) {
        continue;
      }
      Store(base, kShadowKeyB, d.key);
      Store(base, kShadowKeyA, d.key);
      for (uint32_t i = 0; int32_t(i) < int32_t(Load(base, model + 412)); i++) {
        draw_depth(model, i, 0, depth_too, d.lr);
      }
    }
  }

  // The area's light groups, with +196 & 0x400, outside the shadow passes.
  if ((Load(base, manager + 196) & 0x400) == 0) {
    return;
  }
  const uint32_t pass_again = Load(base, description + 20) * 592u + kCameraPasses;
  if (ShadowPass(int32_t(Load(base, pass_again + 584)))) {
    return;
  }
  const uint32_t lights = Load(base, Load(base, area + 12) + 80);
  if (lights == 0) {
    return;
  }
  scenery_lists::SetBillboardPass(base, description, Load(base, description + 8),
                                  Load(base, description + 28));
  for (uint32_t i = 0; i < __builtin_bswap32(Load(base, lights)); i++) {
    const uint32_t animations = Load(base, set + 1060);
    const uint32_t groups = game_data::GetEntry(base, lights, i, false);
    sprites::DrawLightGroups(base, calls, f, description, groups, set + 64, animations);
    FlushOff();
  }
}

}  // namespace course_scenery
