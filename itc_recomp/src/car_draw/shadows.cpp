// The car's shadow shapes into the depth passes as plain C++
// (docs/functions/car-draw.md: PartsPoints::DrawShadows); see car_draw.h.
// Built with -ffp-model=strict (CMakeLists.txt); the matrix work is
// matrix_math's, noted for frame interpolation as the wrappers note the
// originals' calls.

#include "car_draw/car_draw.h"

#include "base/log.h"
#include "guest.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "native/buffers.h"
#include "schema/cameras.h"
#include "schema/car_draw.h"
#include "schema/models.h"
#include "schema/second_set.h"

#include <array>

namespace car_draw {

namespace {

namespace sl = scenery_lists;

using G = schema::CarDrawGlobals;
using Scene = schema::CarScene;
// A key of the scene's pass description.
using Key = uint32_t (Scene::*)() const;

constexpr uint32_t kCameraPasses = schema::CamerasGlobals::passes_address();
constexpr uint32_t kOne = 0x82000A30;  // 1.0
constexpr uint32_t kDrawShapeDepth = 0x82596D48;
// r28 through the draws: the keys' page (lis r28,-32106, sign-extended).
constexpr uint64_t kKeysPage = 0xFFFFFFFF82970000ull;

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(frame, stack);
  return frame;
}

// memcpy (sub_825A7410) from the frame at `sp` returning to `lr`: natively,
// else its original. r3 the destination either way.
Left Memcpy(const Calls& calls, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t sp, uint32_t lr) {
  if (!native::Buffers_Memcpy(to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
  return Left{false, to};
}

// A camera pass by its index: 0 (as the original's) when the address's low
// word is 0 or the pass isn't enabled (+588).
uint32_t EnabledPass(uint32_t index) {
  const uint32_t camera = kCameraPasses + index * schema::CameraPass::kSize;
  if (camera == 0 || int32_t(schema::CameraPass(camera).enabled()) == 0) {
    return 0;
  }
  return camera;
}

// Model::DrawShapeDepth(model, view_projection, world, shape, group, 0, 0)
// from `frame` returning to `lr`: its plain C++ (its recorder's context as
// its wrapper keeps it: the return address 0x82596D9C and r24-r31 = r9, r8,
// r7, r6, r28, r3, r5, r4), else its original from the same frame.
Left ShapeDepth(const Calls& calls, uint32_t frame, uint32_t model,
                uint32_t view_projection, uint32_t world, uint32_t shape, uint32_t group,
                uint32_t lr) {
  const std::array<uint32_t, 9> context = {0x82596D9C, 0,     0,     group,          shape,
                                           uint32_t(kKeysPage), model, world, view_projection};
  const uint32_t depth_frame = frame - kDrawShapeDepthFrame;
  const auto depth = sl::DrawShapeDepth(model, view_projection, world, shape, group, 0, 0,
                                        depth_frame, context);
  if (!depth) {
    const uint64_t regs[7] = {model, view_projection, world, shape, group, 0, 0};
    calls.call_r28(kDrawShapeDepth, regs, 7, frame, lr, kKeysPage);
    return Left{true, 0};
  }
  Store(depth_frame, frame);  // its back chain, as its stwu
  if (!depth->submit) {
    return Left{false, depth->r3};
  }
  calls.draw.submit(calls.draw.user, depth->entry, depth->key, depth->depth, depth_frame,
                    0x82596E60);
  return Left{true, 0};
}

// One set's four draws: shadow_a_00 (record 0's shape, +228) and
// shadow_b_00 (record 3's, +948), each for group 0 then 1, record 0's model.
struct SetSites {
  uint32_t multiply[4];  // the lights' sets' products (0: none)
  uint32_t draw[4];
};

}  // namespace

Left PartsDrawShadows(const Calls& calls, uint32_t stack, uint32_t group,
                      uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820E1D88 PartsPoints::DrawShadows");
  const uint32_t frame = Frame(stack, kPartsShadowsFrame);
  Left left;
  left.r3 = group;
  // The gathered record at +1040: its ten matrices' w words 1.0 first.
  const float one = guest::LoadFloat(kOne);
  for (uint32_t i = 0; i < 10; i++) {
    for (uint32_t at = 1100; at < 1164; at += 16) {
      guest::StoreFloat(frame + at + 64 * i, one);
    }
  }
  // In a race, which of up to three nearby lights cast a shadow (+1040 to
  // +1048) and their planar shadows (+1088 + 64i). Outside one those words
  // are what the stack held, read as the original does.
  if (int32_t(G::scene().model_set()) == 1) {
    left = Left{false, CarLightingGather(calls, frame, frame + 1040, block + 16, 1)};
  }
  const Scene d = G::scene();  // the pass description at its +5072
  const schema::PartsPoints points(group);
  const uint32_t model = points.record0_model();
  if (model == 0 || points.record3_model() == 0) {
    return left;
  }
  left = Memcpy(calls, frame + 80, block + 16, 64, frame, 0x820E1E1C);
  const uint32_t world = frame + 80;
  // Each set under its key: the keys saved to d +76 and +80, both set to
  // it, put back after; `saved` the depth key the original keeps in r27.
  uint32_t saved;
  auto draws = [&](Key key_of, uint32_t view_projection, uint32_t light, const SetSites& sites) {
    d.set_pass_saved_depth_key(saved);
    d.set_pass_saved_other_key(G::other_key());
    const uint32_t key = (d.*key_of)();
    G::set_depth_key(key);
    G::set_other_key(key);
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t shape = i < 2 ? points.record0_shape() : points.record3_shape();
      uint32_t drawn = world;
      if (light != 0) {
        // The products at +592, +336, +720, +464 (light 0), +912, +208,
        // +848, +272 (1), +400, +528, +656, +784 (2): the world x the
        // light's shadow, its 1.0 w's written first.
        static constexpr uint32_t kProducts[3][4] = {
            {592, 336, 720, 464}, {912, 208, 848, 272}, {400, 528, 656, 784}};
        const uint32_t index = (light - (frame + 1088)) / 64;
        drawn = frame + kProducts[index][i];
        for (uint32_t at = 12; at < 64; at += 16) {
          guest::StoreFloat(drawn + at, one);
        }
        matrix_math::Multiply(drawn, world, light);
        interpolation::BlendState_OnMultiply(drawn, world, light, sites.multiply[i]);
      }
      left = ShapeDepth(calls, frame, model, view_projection, drawn, shape, i & 1,
                        sites.draw[i]);
    }
    G::set_other_key(d.pass_saved_other_key());
    saved = d.pass_saved_depth_key();
    G::set_depth_key(saved);
  };
  // Set 1: the camera pass at d +4 (bit 0x4 of d +60), its view-projection
  // at +976, key d +32.
  if (EnabledPass(d.pass_shadow_camera_a()) != 0 && (d.pass_mask() & 0x4) != 0) {
    const uint32_t camera = EnabledPass(d.pass_shadow_camera_a());
    left = Memcpy(calls, frame + 976, schema::CameraPass(camera).view_projection_address(), 64, frame, 0x820E1E90);
    saved = G::depth_key();
    draws(&Scene::pass_shadow_key_a, frame + 976, 0,
          SetSites{{0, 0, 0, 0}, {0x820E1ECC, 0x820E1EEC, 0x820E1F0C, 0x820E1F2C}});
  } else {
    saved = G::depth_key();
  }
  // The rest with the camera pass at d +8's view-projection (+144).
  if (EnabledPass(d.pass_shadow_camera_b()) == 0) {
    return left;
  }
  const uint32_t camera = EnabledPass(d.pass_shadow_camera_b());
  left = Memcpy(calls, frame + 144, schema::CameraPass(camera).view_projection_address(), 64, frame, 0x820E1F98);
  // Set 2 (bit 0x10, key d +36): the block's world.
  if ((d.pass_mask() & 0x10) != 0) {
    draws(&Scene::pass_shadow_key_b, frame + 144, 0,
          SetSites{{0, 0, 0, 0}, {0x820E1FDC, 0x820E1FFC, 0x820E201C, 0x820E203C}});
  }
  // Sets 3 to 5 (bits 0x20, 0x40, 0x80, keys d +40, +44, +48): flattened
  // away from each light the gather found casting a shadow.
  if (int32_t(Load(frame + 1040)) != 0 && (d.pass_mask() & 0x20) != 0) {
    draws(&Scene::pass_light0_shadow_key, frame + 144, frame + 1088,
          SetSites{{0x820E20A0, 0x820E20E4, 0x820E2128, 0x820E216C},
                   {0x820E20C0, 0x820E2104, 0x820E2148, 0x820E218C}});
  }
  if (int32_t(Load(frame + 1044)) != 0 && (d.pass_mask() & 0x40) != 0) {
    draws(&Scene::pass_light1_shadow_key, frame + 144, frame + 1152,
          SetSites{{0x820E21F0, 0x820E2234, 0x820E2278, 0x820E22BC},
                   {0x820E2210, 0x820E2254, 0x820E2298, 0x820E22DC}});
  }
  if (int32_t(Load(frame + 1048)) != 0 && (d.pass_mask() & 0x80) != 0) {
    draws(&Scene::pass_light2_shadow_key, frame + 144, frame + 1216,
          SetSites{{0x820E2340, 0x820E2384, 0x820E23C8, 0x820E240C},
                   {0x820E2360, 0x820E23A4, 0x820E23E8, 0x820E242C}});
  }
  return left;
}

// --- The traffic body's top ---

Left SceneTrafficDraw(const Calls& calls, uint32_t stack, uint32_t traffic,
                      uint32_t block, uint32_t caller_r24) {
  ITC_NATIVE_RUNNING("sub_820E6A40 SceneTraffic::Draw");
  const uint32_t frame = Frame(stack, kSceneTrafficFrame);
  Left left;
  // The identity at +80, copied to +144 (the world below).
  StoreIdentity(frame + 80);
  CopyDoublewords(frame + 144, frame + 80, 64);
  // The gathered record at +464: its ten matrices' w words 1.0 first.
  const float one = guest::LoadFloat(kOne);
  for (uint32_t i = 0; i < 10; i++) {
    for (uint32_t at = 524; at < 588; at += 16) {
      guest::StoreFloat(frame + at + 64 * i, one);
    }
  }
  // In a race, which of up to three nearby lights cast a shadow (+464 to
  // +472) and their planar shadows (+512 + 64i). Outside one those words are
  // what the stack held, read as the original does.
  if (int32_t(schema::SceneTraffic(traffic).scene().model_set()) == 1) {
    left = Left{false, CarLightingGather(calls, frame, frame + 464, block + 16, 1)};
  }
  left = TrafficPointsDraw(calls, frame, schema::SceneTraffic(traffic).points_address(), block, caller_r24);
  const Scene d = G::scene();  // the pass description at its +5072
  // The world: the identity times the block's world, in place.
  matrix_math::Multiply(frame + 144, frame + 144, block + 16);
  interpolation::BlendState_OnMultiply(frame + 144, frame + 144, block + 16, 0x820E6B28);
  left = Left{false, frame + 144};
  const uint32_t world = frame + 144;
  // Each draw under its key: the keys saved to d +76 and +80, both set to
  // it, put back after; `saved` the depth key the original keeps in r30.
  uint32_t saved = G::depth_key();
  auto keys = [&](Key key_of) {
    d.set_pass_saved_depth_key(saved);
    const uint32_t other = G::other_key();
    const uint32_t key = (d.*key_of)();
    d.set_pass_saved_other_key(other);
    G::set_depth_key(key);
    G::set_other_key(key);
  };
  auto restore = [&]() {
    G::set_other_key(d.pass_saved_other_key());
    saved = d.pass_saved_depth_key();
    G::set_depth_key(saved);
  };
  // The lights' sets: the gathered word, d +60's bit, the key, the product
  // (its 1.0 w's written first), the light's shadow matrix and the sites.
  struct Light {
    uint32_t word, bit;
    Key key_of;
    uint32_t product, shadow, multiply, draw;
  };
  static constexpr Light kLights[3] = {
      {464, 0x20, &Scene::pass_light0_shadow_key, 80, 512, 0x820E6D18, 0x820E6D38},
      {468, 0x40, &Scene::pass_light1_shadow_key, 272, 576, 0x820E6D98, 0x820E6DB8},
      {472, 0x80, &Scene::pass_light2_shadow_key, 208, 640, 0x820E6E18, 0x820E6E38},
  };
  for (uint32_t m = 0; m < 6; m++) {
    const uint32_t models = schema::SceneTraffic(traffic).part_models_address() + 4 * m;
    const int32_t shapes = int32_t(schema::Model(Load(models)).shape_count());
    for (int32_t s = 0; s < shapes; s++) {
      const uint32_t shape = uint32_t(s);
      // The camera pass at d +4 (bit 0x4 of d +60): its view-projection at
      // +400, groups 0 and 1, key d +32.
      if (EnabledPass(d.pass_shadow_camera_a()) != 0 && (d.pass_mask() & 0x4) != 0) {
        const uint32_t camera = EnabledPass(d.pass_shadow_camera_a());
        left = Memcpy(calls, frame + 400, schema::CameraPass(camera).view_projection_address(), 64, frame, 0x820E6BB8);
        keys(&Scene::pass_shadow_key_a);
        left = ShapeDepth(calls, frame, Load(models), frame + 400, world, shape, 0,
                          0x820E6BF0);
        left = ShapeDepth(calls, frame, Load(models), frame + 400, world, shape, 1,
                          0x820E6C10);
        restore();
      }
      // The rest with the camera pass at d +8's view-projection (+336).
      const uint32_t camera = EnabledPass(d.pass_shadow_camera_b());
      if (camera == 0) {
        continue;
      }
      left = Memcpy(calls, frame + 336, schema::CameraPass(camera).view_projection_address(), 64, frame, 0x820E6C74);
      // Bit 0x10, key d +36: the block's world.
      if ((d.pass_mask() & 0x10) != 0) {
        keys(&Scene::pass_shadow_key_b);
        left = ShapeDepth(calls, frame, Load(models), frame + 336, world, shape, 0,
                          0x820E6CB8);
        restore();
      }
      // Bits 0x20, 0x40, 0x80: flattened away from each light the gather
      // found casting a shadow.
      for (const Light& light : kLights) {
        if (int32_t(Load(frame + light.word)) == 0 ||
            (d.pass_mask() & light.bit) == 0) {
          continue;
        }
        keys(light.key_of);
        const uint32_t product = frame + light.product;
        for (uint32_t at = 12; at < 64; at += 16) {
          guest::StoreFloat(product + at, one);
        }
        matrix_math::Multiply(product, world, frame + light.shadow);
        interpolation::BlendState_OnMultiply(product, world, frame + light.shadow,
                                             light.multiply);
        left = ShapeDepth(calls, frame, Load(models), frame + 336, product, shape, 0,
                          light.draw);
        restore();
      }
    }
  }
  return left;
}

}  // namespace car_draw
