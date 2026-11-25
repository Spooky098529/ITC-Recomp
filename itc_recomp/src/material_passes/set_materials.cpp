// PartModel::SetMaterials and what only it calls (see material_passes.h):
// the fields a mask picks set on the materials a category or shared pair
// picks (the original's frames' words values since E5 step 6).

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/material_passes.h"
#include "schema/models.h"

#include <bit>

namespace material_passes {

using namespace camera_modes::steps;

namespace {

uint64_t Load64(uint32_t a) {
  return (uint64_t(guest::LoadU32(a)) << 32) | guest::LoadU32(a + 4);
}
void Store64(uint32_t a, uint64_t value) {
  guest::StoreU32(a, uint32_t(value >> 32));
  guest::StoreU32(a + 4, uint32_t(value));
}
int32_t LoadS(uint32_t address) {
  return int32_t(guest::LoadU32(address));
}

using Material = schema::MaterialRecord;

}  // namespace

uint32_t MaterialTotal(uint32_t model, uint64_t shape) {
  ITC_NATIVE_RUNNING("sub_82594AC0 Model::MaterialTotal");
  const uint32_t shapes = schema::Model(model).shapes().address;
  const uint32_t block =
      schema::ModelShape(uint32_t(shape * schema::ModelShape::kSize + shapes)).materials().address;
  return schema::MaterialBlock(block).record_count();
}

uint64_t MaterialAt(uint32_t model, uint64_t shape, uint64_t index) {
  ITC_NATIVE_RUNNING("sub_82594B00 Model::MaterialAt");
  const uint64_t offset = uint32_t(index << 9) & 0xFFFFFE00u;
  const uint32_t shapes = schema::Model(model).shapes().address;
  const uint64_t block =
      schema::ModelShape(uint32_t(shape * schema::ModelShape::kSize + shapes)).materials().address;
  return block + offset + 16;
}

void SetMaterialFields(uint32_t material, const uint64_t args[8],
                       const uint32_t copied[6]) {
  ITC_NATIVE_RUNNING("sub_820D3250 PartModel::SetMaterialFields");
  // The original's homes in its caller's frame (args at +16, +24, +32, +48
  // to +80) and the words read back from them: the flags the words +16 to
  // +36 (the halves of args 0 to 2), the float4s at +48 (args 3, 4), +64
  // (args 5, 6) and +80 (arg 7 and the copy's first doubleword), the floats
  // at +96 to +104 (the copy's words 2 to 4).
  const auto high = [](uint64_t v) { return int32_t(uint32_t(v >> 32)); };
  const auto low = [](uint64_t v) { return int32_t(uint32_t(v)); };
  const auto word64 = [&](int i) { return (uint64_t(copied[i]) << 32) | copied[i + 1]; };
  const auto single = [&](int i) { return double(std::bit_cast<float>(copied[i])); };
  const Material m(material);
  if (high(args[0]) != 0) {
    m.set_color_xy(args[3]);
    m.set_color_zw(args[4]);
  }
  if (low(args[0]) != 0) {
    m.set_pixel_c66_xy(args[5]);
    m.set_pixel_c66_zw(args[6]);
  }
  if (high(args[1]) != 0) {
    m.set_pixel_c65_xy(args[7]);
    m.set_pixel_c65_zw(word64(0));
  }
  if (low(args[1]) != 0) {
    m.set_pixel_c64_x(float(single(2)));
  }
  if (high(args[2]) != 0) {
    m.set_pixel_c64_y(float(single(3)));
  }
  if (low(args[2]) != 0) {
    m.set_pixel_c64_z(float(single(4)));
  }
}

bool SetMaterials(uint32_t model, const uint64_t regs[7], uint32_t sp,
                  std::optional<uint64_t>* r3) {
  ITC_NATIVE_RUNNING("sub_820D3318 PartModel::SetMaterials");
  // The original's 256-byte frame left out (E5 step 6): the material (+116),
  // the 24 bytes it copies for each material (+88) and SetMaterialFields'
  // homes (+16 to +80) were read by it and SetMaterialFields alone (values
  // here). The caller's frame (its stack arguments, above `sp`) stays.
  for (int i = 0; i < 4; i++) {
    Store64(sp + 64 + 8 * i, regs[3 + i]);  // r7 to r10 in the caller's frame
  }
  if (model == 0 || LoadS(sp + 132) != -1) {
    return true;
  }
  const int32_t shapes = int32_t(schema::PartModel(model).shape_count());
  if (shapes <= 0) {
    return true;
  }
  const int32_t pair = LoadS(sp + 148);
  const int32_t category = LoadS(sp + 140);
  const uint64_t third = Load64(sp + 96);
  for (int32_t shape = 0; shape < shapes; shape++) {
    const int32_t count = int32_t(MaterialTotal(model, uint64_t(int64_t(shape))));
    *r3 = uint32_t(count);
    for (int32_t i = 0; i < count; i++) {
      const uint64_t at = MaterialAt(model, uint64_t(int64_t(shape)), uint64_t(int64_t(i)));
      *r3 = at;
      const uint32_t material = uint32_t(at);
      const Material m(material);
      bool pick;
      if (category >= 0 && category == int32_t(m.category())) {
        pick = true;
      } else if (pair >= 0 && uint32_t(pair) == m.shared_pair()) {
        pick = true;
      } else {
        pick = category >= -1 && pair == -1;
      }
      if (!pick) {
        continue;
      }
      // memcpy's 24 bytes from the caller's frame (+104), as words.
      uint32_t copied[6];
      for (uint32_t w = 0; w < 6; w++) {
        copied[w] = guest::LoadU32(sp + 104 + 4 * w);
      }
      const uint64_t args[8] = {regs[0], regs[1], regs[2], regs[3],
                                regs[4], regs[5], regs[6], third};
      SetMaterialFields(material, args, copied);
      *r3 = regs[0];
    }
  }
  return true;
}

}  // namespace material_passes
