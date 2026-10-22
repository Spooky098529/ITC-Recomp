#include "sprites/billboards.h"

#include "guest.h"

namespace sprites {

void InitBillboardMaterial(uint8_t* base, uint32_t material) {
  guest::StoreU32(base, material + 8, 0);
  guest::StoreU32(base, material + 0, 1);
  guest::StoreU32(base, material + 4, 1);
  guest::StoreU32(base, material + 12, 0);
  guest::StoreU32(base, material + 16, 0);
  guest::StoreU32(base, material + 32, 0);
  guest::StoreU32(base, material + 36, 0);
}

void SetBillboardLights(uint8_t* base, uint32_t material, uint32_t count, uint32_t lights) {
  if (count == 0) {
    return;
  }
  guest::StoreU32(base, material + 32, count);
  guest::StoreU32(base, material + 36, lights);
}

}  // namespace sprites
