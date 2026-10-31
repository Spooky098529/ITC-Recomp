// The passes over a part model's materials that set one field or a few
// (see material_passes.h). Each walks a shape's two groups
// (Model::MaterialCount and Model::Material, models/model.h) as the
// original does and returns r3 as it leaves it (the last call's result, or
// the part model when nothing was called).

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "models/model.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

constexpr uint32_t kOne = 0x82000A30;           // 1.0
constexpr uint32_t kBlendedLimit = 0x82002CB0;  // 1.1

int32_t LoadS(uint8_t* base, uint32_t address) {
  return int32_t(guest::LoadU32(base, address));
}

// stwu's back chain at the bottom of the original's frame.
void BackChain(uint8_t* base, uint32_t sp, uint32_t frame_bytes) {
  guest::StoreU32(base, sp - frame_bytes, sp);
}

// Both groups of one shape: `each(material)` on each material of group 0,
// then group 1; r3 as the original leaves it after the two counts.
template <typename Each>
uint32_t EachMaterial(uint8_t* base, uint32_t model, uint32_t shape, Each each) {
  const int32_t first = int32_t(models::MaterialCount(base, model, shape, 0));
  const int32_t second = int32_t(models::MaterialCount(base, model, shape, 1));
  uint32_t r3 = uint32_t(second);
  for (int32_t i = 0; i < first; i++) {
    r3 = models::Material(base, model, shape, uint32_t(i), 0);
    each(r3);
  }
  for (int32_t i = 0; i < second; i++) {
    r3 = models::Material(base, model, shape, uint32_t(i), 1);
    each(r3);
  }
  return r3;
}

// Every shape (the part model's +412): r3 the model when there are none.
template <typename Each>
uint32_t EachShape(uint8_t* base, uint32_t model, Each each) {
  const int32_t shapes = LoadS(base, model + 412);
  uint32_t r3 = model;
  for (int32_t shape = 0; shape < shapes; shape++) {
    r3 = EachMaterial(base, model, uint32_t(shape), each);
  }
  return r3;
}

}  // namespace

uint32_t SetTexture(uint8_t* base, uint32_t model, uint32_t texture, int32_t shape,
                    uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D17D8 PartModel::SetTexture");
  BackChain(base, sp, 160);
  if (model == 0) {
    return model;
  }
  auto set = [&](uint32_t material) {
    guest::StoreU32(base, material + 144, 0);
    guest::StoreU32(base, material + 176, texture);
  };
  if (shape == -1) {
    return EachShape(base, model, set);
  }
  return EachMaterial(base, model, uint32_t(shape), set);
}

uint32_t FlipCulling(uint8_t* base, uint32_t model, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1950 PartModel::FlipCulling");
  BackChain(base, sp, 144);
  if (model == 0) {
    return model;
  }
  return EachShape(base, model, [&](uint32_t material) {
    const uint32_t cull = guest::LoadU32(base, material + 344);
    if (cull == 2) {
      guest::StoreU32(base, material + 344, 6);
    } else if (cull == 6) {
      guest::StoreU32(base, material + 344, 2);
    }
  });
}

uint32_t ScaleBlended(uint8_t* base, uint32_t model, uint32_t shape, double factor,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1A68 PartModel::ScaleBlended");
  BackChain(base, sp, 144);
  if (model == 0) {
    return model;
  }
  models::MaterialCount(base, model, shape, 0);  // read and not used
  const int32_t second = int32_t(models::MaterialCount(base, model, shape, 1));
  uint32_t r3 = uint32_t(second);
  if (second <= 0) {
    return r3;
  }
  const double one = Lfs(base, kOne);
  const double limit = Lfs(base, kBlendedLimit);
  for (int32_t i = 0; i < second; i++) {
    r3 = models::Material(base, model, shape, uint32_t(i), 1);
    const double x = Lfs(base, r3 + 0);
    if (x < limit) {
      Stfs(base, r3 + 0, Fmuls(x, factor));
      const double y = Lfs(base, r3 + 4);
      const double z = Lfs(base, r3 + 8);
      Stfs(base, r3 + 4, Fmuls(y, factor));
      Stfs(base, r3 + 8, Fmuls(z, factor));
    }
    Stfs(base, r3 + 12, one);
    guest::StoreU32(base, r3 + 108, 2);
  }
  return r3;
}

uint32_t SetPair1002Alpha(uint8_t* base, uint32_t model, uint32_t shape, double alpha,
                          uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1B38 PartModel::SetPair1002Alpha");
  BackChain(base, sp, 144);
  if (model == 0) {
    return model;
  }
  return EachMaterial(base, model, shape, [&](uint32_t material) {
    if (guest::LoadU32(base, material + 392) == 1002) {
      Stfs(base, material + 12, alpha);
    }
  });
}

uint32_t SetAlpha(uint8_t* base, uint32_t model, uint32_t shape, double alpha, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1C00 PartModel::SetAlpha");
  BackChain(base, sp, 144);
  if (model == 0) {
    return model;
  }
  return EachMaterial(base, model, shape,
                      [&](uint32_t material) { Stfs(base, material + 12, alpha); });
}

}  // namespace material_passes
