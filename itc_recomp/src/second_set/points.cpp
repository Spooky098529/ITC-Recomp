// The second set's point records (see points.h), store for store as the
// recompiled functions.

#include "second_set/points.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "native/buffers.h"
#include "scene_loaders/pools.h"

#include <bit>
#include <cstdlib>

namespace second_set {

namespace {

using car_physics::ops::CopyWord;
using car_physics::ops::kOne;
using car_physics::ops::kZero;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The identity (vupkd3d128's rows, stored aligned) at `at`.
void Identity(uint8_t* base, uint32_t at) {
  const uint32_t aligned = at & ~0xFu;
  const uint32_t one = std::bit_cast<uint32_t>(1.0f);
  for (uint32_t w = 0; w < 16; w++) {
    Store(base, aligned + 4 * w, w % 5 == 0 ? one : 0);
  }
}

void Memcpy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (!native::Buffers_Memcpy(base, to, from, bytes, caller)) {
    ITC_LOG_ERROR("Second set: a point record's memcpy refused after its checks");
    std::abort();
  }
}

// The string at `name` copied, with its 0, to `to`.
void CopyString(uint8_t* base, uint32_t to, uint32_t name) {
  for (uint32_t i = 0;; i++) {
    const uint8_t ch = base[name + i];
    base[to + i] = ch;
    if (ch == 0) {
      return;
    }
  }
}

}  // namespace

void PointRecordSetNode(uint8_t* base, uint32_t record, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_820CE8D8 PointRecord::SetNode");
  CopyString(base, record + 160, name);
}

void PointRecordSetShape(uint8_t* base, uint32_t record, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_820CE900 PointRecord::SetShape");
  CopyString(base, record + 192, name);
}

uint32_t PointRecordClear(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820CEA08 PointRecord::Clear");
  const uint32_t f = stack - kClearFrameBytes;
  Store(base, record + 224, 0xFFFFFFFFu);
  Store(base, record + 228, 0xFFFFFFFFu);
  Store(base, record + 0, 0);
  Store(base, record + 4, 0);
  Store(base, record + 8, 0);
  Identity(base, f + 96);
  Memcpy(base, record + 16, f + 96, 64, 0x820CEA8C);
  Identity(base, f + 96);
  Memcpy(base, record + 80, f + 96, 64, 0x820CEAD0);
  for (const uint32_t at : {80u, 84u, 88u}) {
    CopyWord(base, kZero, f + at);
  }
  CopyWord(base, kOne, f + 92);
  for (uint32_t w = 0; w < 4; w++) {
    Store(base, record + 144 + 4 * w, Load(base, f + 80 + 4 * w));
  }
  base[record + 160] = 0;
  base[record + 192] = 0;
  return record + 80;
}

uint32_t PointRecordConstruct(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D0270 PointRecord::PointRecord");
  for (uint32_t at = 28; at <= 156; at += 16) {
    CopyWord(base, kOne, record + at);
  }
  PointRecordClear(base, record, stack - kConstructFrameBytes);
  return record;
}

uint32_t ModelPointFindShape(uint8_t* base, uint32_t record, uint32_t model, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820CEB30 ModelPoint::FindShape");
  const uint32_t f = stack - kFindShapeFrameBytes;
  Store(base, record + 228, 0xFFFFFFFFu);
  Store(base, record + 0, 0);
  Store(base, record + 4, 0);
  Store(base, record + 8, 0);
  Identity(base, f + 80);
  Memcpy(base, record + 16, f + 80, 64, 0x820CEBA4);
  const int32_t nodes = int32_t(Load(base, model + 408));
  for (int32_t n = 0; n < nodes; n++) {
    const uint32_t node = uint32_t(n) * 192 + Load(base, model + 36);
    const uint32_t index = Load(base, node + 40);
    if (int32_t(index) == -1) {
      continue;
    }
    const uint32_t shape = index * 544 + Load(base, model + 40);
    if (scene_loaders::NameMatches(base, shape, record + 192) != 0) {
      Store(base, record + 4, model);
      Store(base, record + 8, shape);
      Store(base, record + 228, Load(base, node + 40));
      return 1;
    }
  }
  return 0;
}

}  // namespace second_set
