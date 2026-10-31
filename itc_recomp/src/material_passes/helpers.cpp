// The small helpers beside the material passes: fixed tables by index and
// a matrix's rotation transposed (see material_passes.h).

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "guest.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

constexpr uint32_t kTableVectors = 0x82601A00;
constexpr uint64_t kRecords = 0xFFFFFFFF82003680ull;  // lis -32256 + 13952, sign-extended

}  // namespace

void TableVector(uint8_t* base, uint32_t out, uint32_t index, uint32_t sub) {
  ITC_NATIVE_RUNNING("sub_820D35D0 TableVector");
  const uint32_t from = kTableVectors + ((index * 4 + sub) << 4);
  const uint32_t a0 = guest::LoadU32(base, from + 0);
  const uint32_t a1 = guest::LoadU32(base, from + 4);
  guest::StoreU32(base, out + 0, a0);
  guest::StoreU32(base, out + 4, a1);
  const uint32_t b0 = guest::LoadU32(base, from + 8);
  const uint32_t b1 = guest::LoadU32(base, from + 12);
  guest::StoreU32(base, out + 8, b0);
  guest::StoreU32(base, out + 12, b1);
}

uint32_t IsBlockSetA(int32_t value) {
  ITC_NATIVE_RUNNING("sub_820D3600 IsBlockSetA");
  return value == 36 || value == 26 || value == 28 || value == 33 || value == 1 ? 1 : 0;
}

uint32_t IsBlockSetB(int32_t value) {
  ITC_NATIVE_RUNNING("sub_820D3638 IsBlockSetB");
  return value == 10 || value == 37 ? 1 : 0;
}

uint64_t RecordAt(uint64_t index) {
  ITC_NATIVE_RUNNING("sub_820D3728 RecordAt");
  return index * 48 + kRecords;
}

bool TransposeRotation(uint8_t* base, uint32_t out, uint32_t m, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1CB0 Matrix::TransposeRotation");
  const uint32_t frame = sp - 176;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t row = 0; row < 4; row++) {
    const uint32_t at = (frame + 80 + 16 * row) & ~0xFu;  // stvx
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU32(base, at + 4 * k, k == row ? one : 0);
    }
  }
  if (!c_runtime::Memcpy(base, out, frame + 80, 64)) {
    return false;
  }
  static constexpr uint32_t kFrom[9] = {0, 16, 32, 4, 20, 36, 8, 24, 40};
  static constexpr uint32_t kTo[9] = {0, 4, 8, 16, 20, 24, 32, 36, 40};
  for (int i = 0; i < 9; i++) {
    Stfs(base, out + kTo[i], Lfs(base, m + kFrom[i]));
  }
  return true;
}

}  // namespace material_passes
