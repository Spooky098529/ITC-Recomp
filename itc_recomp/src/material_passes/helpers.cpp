// The small helpers beside the material passes: fixed tables by index and
// a matrix's rotation transposed (see material_passes.h).

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "schema/material_passes.h"
#include "schema/matrix_math.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

using G = schema::MaterialPassesGlobals;

// lis -32256 + 13952, sign-extended.
constexpr uint64_t kRecords = uint64_t(int64_t(int32_t(G::part_records_address())));

}  // namespace

void TableVector(uint32_t out, uint32_t index, uint32_t sub) {
  ITC_NATIVE_RUNNING("sub_820D35D0 TableVector");
  const uint32_t from = G::table_vectors_address() + ((index * 4 + sub) << 4);
  const uint32_t a0 = guest::LoadU32(from + 0);
  const uint32_t a1 = guest::LoadU32(from + 4);
  guest::StoreU32(out + 0, a0);
  guest::StoreU32(out + 4, a1);
  const uint32_t b0 = guest::LoadU32(from + 8);
  const uint32_t b1 = guest::LoadU32(from + 12);
  guest::StoreU32(out + 8, b0);
  guest::StoreU32(out + 12, b1);
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

bool TransposeRotation(uint32_t out, uint32_t m, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1CB0 Matrix::TransposeRotation");
  const uint32_t frame = sp - 176;
  guest::StoreU32(frame, sp);  // the back chain, as stwu
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t row = 0; row < 4; row++) {
    const uint32_t at = (frame + 80 + 16 * row) & ~0xFu;  // stvx
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU32(at + 4 * k, k == row ? one : 0);
    }
  }
  if (!c_runtime::Memcpy(out, frame + 80, 64)) {
    return false;
  }
  // out's upper 3x3 = m's transposed, each word through lfs and stfs.
  const schema::MaterialPassesMatrix from(m);
  const schema::Matrix to(out);
  Stfs(to.m00_address(), double(from.m00()));
  Stfs(to.m01_address(), double(from.m10()));
  Stfs(to.m02_address(), double(from.m20()));
  Stfs(to.m10_address(), double(from.m01()));
  Stfs(to.m11_address(), double(from.m11()));
  Stfs(to.m12_address(), double(from.m21()));
  Stfs(to.m20_address(), double(from.m02()));
  Stfs(to.m21_address(), double(from.m12()));
  Stfs(to.m22_address(), double(from.m22()));
  return true;
}

}  // namespace material_passes
