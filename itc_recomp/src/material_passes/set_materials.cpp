// PartModel::SetMaterials and what only it calls (see material_passes.h):
// the fields a mask picks set on the materials a category or shared pair
// picks, through the original's frames.

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "guest.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

uint64_t Load64(uint8_t* base, uint32_t a) {
  return (uint64_t(guest::LoadU32(base, a)) << 32) | guest::LoadU32(base, a + 4);
}
void Store64(uint8_t* base, uint32_t a, uint64_t value) {
  guest::StoreU32(base, a, uint32_t(value >> 32));
  guest::StoreU32(base, a + 4, uint32_t(value));
}
int32_t LoadS(uint8_t* base, uint32_t address) {
  return int32_t(guest::LoadU32(base, address));
}

// ld, ld, std, std: 16 bytes from `from` to `to`.
void Copy16(uint8_t* base, uint32_t to, uint32_t from) {
  const uint64_t a = Load64(base, from + 0);
  const uint64_t b = Load64(base, from + 8);
  Store64(base, to + 0, a);
  Store64(base, to + 8, b);
}

}  // namespace

uint32_t MaterialTotal(uint8_t* base, uint32_t model, uint64_t shape) {
  ITC_NATIVE_RUNNING("sub_82594AC0 Model::MaterialTotal");
  const uint32_t block = guest::LoadU32(base, uint32_t(shape * 544 + guest::LoadU32(base, model + 40)) + 396);
  return guest::LoadU32(base, block);
}

uint64_t MaterialAt(uint8_t* base, uint32_t model, uint64_t shape, uint64_t index) {
  ITC_NATIVE_RUNNING("sub_82594B00 Model::MaterialAt");
  const uint64_t offset = uint32_t(index << 9) & 0xFFFFFE00u;
  const uint64_t block =
      guest::LoadU32(base, uint32_t(shape * 544 + guest::LoadU32(base, model + 40)) + 396);
  return block + offset + 16;
}

void SetMaterialFields(uint8_t* base, uint32_t frame, const uint64_t args[8]) {
  ITC_NATIVE_RUNNING("sub_820D3250 PartModel::SetMaterialFields");
  static constexpr uint32_t kHomes[8] = {16, 24, 32, 48, 56, 64, 72, 80};
  for (int i = 0; i < 8; i++) {
    Store64(base, frame + kHomes[i], args[i]);
  }
  const uint32_t material = guest::LoadU32(base, frame + 116);
  if (LoadS(base, frame + 16) != 0) {
    Copy16(base, material + 0, frame + 48);
  }
  if (LoadS(base, frame + 20) != 0) {
    Copy16(base, material + 32, frame + 64);
  }
  if (LoadS(base, frame + 24) != 0) {
    Copy16(base, material + 16, frame + 80);
  }
  if (LoadS(base, frame + 28) != 0) {
    Stfs(base, material + 80, Lfs(base, frame + 96));
  }
  if (LoadS(base, frame + 32) != 0) {
    Stfs(base, material + 84, Lfs(base, frame + 100));
  }
  if (LoadS(base, frame + 36) != 0) {
    Stfs(base, material + 88, Lfs(base, frame + 104));
  }
}

bool SetMaterials(uint8_t* base, uint32_t model, const uint64_t regs[7], uint32_t sp,
                  std::optional<uint64_t>* r3) {
  ITC_NATIVE_RUNNING("sub_820D3318 PartModel::SetMaterials");
  const uint32_t frame = sp - 256;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  for (int i = 0; i < 4; i++) {
    Store64(base, sp + 64 + 8 * i, regs[3 + i]);  // r7 to r10 in the caller's frame
  }
  if (model == 0 || LoadS(base, sp + 132) != -1) {
    return true;
  }
  const int32_t shapes = LoadS(base, model + 412);
  if (shapes <= 0) {
    return true;
  }
  const int32_t pair = LoadS(base, sp + 148);
  const int32_t category = LoadS(base, sp + 140);
  const uint64_t third = Load64(base, sp + 96);
  for (int32_t shape = 0; shape < shapes; shape++) {
    const int32_t count = int32_t(MaterialTotal(base, model, uint64_t(int64_t(shape))));
    *r3 = uint32_t(count);
    for (int32_t i = 0; i < count; i++) {
      const uint64_t at = MaterialAt(base, model, uint64_t(int64_t(shape)), uint64_t(int64_t(i)));
      *r3 = at;
      const uint32_t material = uint32_t(at);
      bool pick;
      if (category >= 0 && category == LoadS(base, material + 408)) {
        pick = true;
      } else if (pair >= 0 && uint32_t(pair) == guest::LoadU32(base, material + 392)) {
        pick = true;
      } else {
        pick = category >= -1 && pair == -1;
      }
      if (!pick) {
        continue;
      }
      guest::StoreU32(base, frame + 116, material);
      if (!c_runtime::Memcpy(base, frame + 88, sp + 104, 24)) {
        return false;
      }
      const uint64_t args[8] = {regs[0], regs[1], regs[2], regs[3],
                                regs[4], regs[5], regs[6], third};
      SetMaterialFields(base, frame, args);
      *r3 = regs[0];
    }
  }
  return true;
}

}  // namespace material_passes
