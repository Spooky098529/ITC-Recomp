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

#include <array>

namespace car_draw {

namespace {

namespace sl = scenery_lists;

constexpr uint32_t kScene = 0x826C2E44;
constexpr uint32_t kCameraPasses = 0x826BAC00;
constexpr uint32_t kDepthKey = 0x8296B8FC;
constexpr uint32_t kOtherKey = 0x8296B8F8;
constexpr uint32_t kOne = 0x82000A30;  // 1.0
constexpr uint32_t kDrawShapeDepth = 0x82596D48;
// r28 through the draws: the keys' page (lis r28,-32106, sign-extended).
constexpr uint64_t kKeysPage = 0xFFFFFFFF82970000ull;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The frame `bytes` below `stack`, its back chain written (stwu's).
uint32_t Frame(uint8_t* base, uint32_t stack, uint32_t bytes) {
  const uint32_t frame = stack - bytes;
  Store(base, frame, stack);
  return frame;
}

// memcpy (sub_825A7410) from the frame at `sp` returning to `lr`: natively,
// else its original. r3 the destination either way.
Left Memcpy(uint8_t* base, const Calls& calls, uint32_t to, uint32_t from, uint32_t bytes,
            uint32_t sp, uint32_t lr) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, lr)) {
    calls.memcpy(to, from, bytes, sp, lr);
  }
  return Left{false, to};
}

// A camera pass by its index: 0 (as the original's) when the address's low
// word is 0 or the pass isn't enabled (+588).
uint32_t EnabledPass(uint8_t* base, uint32_t index) {
  const uint32_t camera = kCameraPasses + index * 592;
  if (camera == 0 || int32_t(Load(base, camera + 588)) == 0) {
    return 0;
  }
  return camera;
}

// Model::DrawShapeDepth(model, view_projection, world, shape, group, 0, 0)
// from `frame` returning to `lr`: its plain C++ (its recorder's context as
// its wrapper keeps it: the return address 0x82596D9C and r24-r31 = r9, r8,
// r7, r6, r28, r3, r5, r4), else its original from the same frame.
Left ShapeDepth(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t model,
                uint32_t view_projection, uint32_t world, uint32_t shape, uint32_t group,
                uint32_t lr) {
  const std::array<uint32_t, 9> context = {0x82596D9C, 0,     0,     group,          shape,
                                           uint32_t(kKeysPage), model, world, view_projection};
  const uint32_t depth_frame = frame - kDrawShapeDepthFrame;
  const auto depth = sl::DrawShapeDepth(base, model, view_projection, world, shape, group, 0, 0,
                                        depth_frame, context);
  if (!depth) {
    const uint64_t regs[7] = {model, view_projection, world, shape, group, 0, 0};
    calls.call_r28(kDrawShapeDepth, regs, 7, frame, lr, kKeysPage);
    return Left{true, 0};
  }
  Store(base, depth_frame, frame);  // its back chain, as its stwu
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

Left PartsDrawShadows(uint8_t* base, const Calls& calls, uint32_t stack, uint32_t group,
                      uint32_t block) {
  ITC_NATIVE_RUNNING("sub_820E1D88 PartsPoints::DrawShadows");
  const uint32_t frame = Frame(base, stack, kPartsShadowsFrame);
  Left left;
  left.r3 = group;
  // The gathered record at +1040: its ten matrices' w words 1.0 first.
  const float one = guest::LoadFloat(base, kOne);
  for (uint32_t i = 0; i < 10; i++) {
    for (uint32_t at = 1100; at < 1164; at += 16) {
      guest::StoreFloat(base, frame + at + 64 * i, one);
    }
  }
  // In a race, which of up to three nearby lights cast a shadow (+1040 to
  // +1048) and their planar shadows (+1088 + 64i). Outside one those words
  // are what the stack held, read as the original does.
  if (int32_t(Load(base, Load(base, kScene) + 4)) == 1) {
    left = Left{false, CarLightingGather(base, calls, frame, frame + 1040, block + 16, 1)};
  }
  const uint32_t scene = Load(base, kScene);
  const uint32_t model = Load(base, group + 4);
  if (model == 0 || Load(base, group + 724) == 0) {
    return left;
  }
  left = Memcpy(base, calls, frame + 80, block + 16, 64, frame, 0x820E1E1C);
  const uint32_t world = frame + 80;
  const uint32_t d = scene + 5072;  // the pass description
  // Each set under its key: the keys saved to d +76 and +80, both set to
  // it, put back after; `saved` the depth key the original keeps in r27.
  uint32_t saved;
  auto draws = [&](uint32_t key_at, uint32_t view_projection, uint32_t light,
                   const SetSites& sites) {
    Store(base, d + 76, saved);
    Store(base, d + 80, Load(base, kOtherKey));
    const uint32_t key = Load(base, d + key_at);
    Store(base, kDepthKey, key);
    Store(base, kOtherKey, key);
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t shape = Load(base, group + (i < 2 ? 228 : 948));
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
          guest::StoreFloat(base, drawn + at, one);
        }
        matrix_math::Multiply(base, drawn, world, light);
        interpolation::BlendState_OnMultiply(base, drawn, world, light, sites.multiply[i]);
      }
      left = ShapeDepth(base, calls, frame, model, view_projection, drawn, shape, i & 1,
                        sites.draw[i]);
    }
    Store(base, kOtherKey, Load(base, d + 80));
    saved = Load(base, d + 76);
    Store(base, kDepthKey, saved);
  };
  // Set 1: the camera pass at d +4 (bit 0x4 of d +60), its view-projection
  // at +976, key d +32.
  if (EnabledPass(base, Load(base, d + 4)) != 0 && (Load(base, d + 60) & 0x4) != 0) {
    const uint32_t camera = EnabledPass(base, Load(base, d + 4));
    left = Memcpy(base, calls, frame + 976, camera + 144, 64, frame, 0x820E1E90);
    saved = Load(base, kDepthKey);
    draws(32, frame + 976, 0,
          SetSites{{0, 0, 0, 0}, {0x820E1ECC, 0x820E1EEC, 0x820E1F0C, 0x820E1F2C}});
  } else {
    saved = Load(base, kDepthKey);
  }
  // The rest with the camera pass at d +8's view-projection (+144).
  if (EnabledPass(base, Load(base, d + 8)) == 0) {
    return left;
  }
  const uint32_t camera = EnabledPass(base, Load(base, d + 8));
  left = Memcpy(base, calls, frame + 144, camera + 144, 64, frame, 0x820E1F98);
  // Set 2 (bit 0x10, key d +36): the block's world.
  if ((Load(base, d + 60) & 0x10) != 0) {
    draws(36, frame + 144, 0,
          SetSites{{0, 0, 0, 0}, {0x820E1FDC, 0x820E1FFC, 0x820E201C, 0x820E203C}});
  }
  // Sets 3 to 5 (bits 0x20, 0x40, 0x80, keys d +40, +44, +48): flattened
  // away from each light the gather found casting a shadow.
  if (int32_t(Load(base, frame + 1040)) != 0 && (Load(base, d + 60) & 0x20) != 0) {
    draws(40, frame + 144, frame + 1088,
          SetSites{{0x820E20A0, 0x820E20E4, 0x820E2128, 0x820E216C},
                   {0x820E20C0, 0x820E2104, 0x820E2148, 0x820E218C}});
  }
  if (int32_t(Load(base, frame + 1044)) != 0 && (Load(base, d + 60) & 0x40) != 0) {
    draws(44, frame + 144, frame + 1152,
          SetSites{{0x820E21F0, 0x820E2234, 0x820E2278, 0x820E22BC},
                   {0x820E2210, 0x820E2254, 0x820E2298, 0x820E22DC}});
  }
  if (int32_t(Load(base, frame + 1048)) != 0 && (Load(base, d + 60) & 0x80) != 0) {
    draws(48, frame + 144, frame + 1216,
          SetSites{{0x820E2340, 0x820E2384, 0x820E23C8, 0x820E240C},
                   {0x820E2360, 0x820E23A4, 0x820E23E8, 0x820E242C}});
  }
  return left;
}

}  // namespace car_draw
