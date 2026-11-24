// The second set's point records (see points.h), store for store as the
// recompiled functions.

#include "second_set/points.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "memory/handles.h"
#include "memory/host_copy.h"
#include "native/buffers.h"
#include "scene_loaders/pools.h"
#include "schema/models.h"
#include "schema/second_set.h"

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

// The string at `name` copied, with its 0, to `to` (a record inside a host
// object, E5 step 4: its bytes copied through the handle, as they are).
void CopyString(uint8_t* base, uint32_t to, uint32_t name) {
  if (memory::IsHandle(to) || memory::IsHandle(name)) {
    uint32_t length = 0;
    while (guest::LoadU8(base, name + length) != 0) {
      length++;
    }
    memory::CopyWithHandles(base, to, name, length + 1);
    return;
  }
  for (uint32_t i = 0;; i++) {
    const uint8_t ch = guest::LoadU8(base, name + i);
    guest::StoreU8(base, to + i, ch);
    if (ch == 0) {
      return;
    }
  }
}

}  // namespace

void PointRecordSetNode(uint8_t* base, uint32_t record, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_820CE8D8 PointRecord::SetNode");
  CopyString(base, schema::SecondSetPointRecord(base, record).node_name_address(), name);
}

void PointRecordSetShape(uint8_t* base, uint32_t record, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_820CE900 PointRecord::SetShape");
  CopyString(base, schema::SecondSetPointRecord(base, record).shape_name_address(), name);
}

uint32_t PointRecordClear(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820CEA08 PointRecord::Clear");
  const uint32_t f = stack - kClearFrameBytes;
  const schema::SecondSetPointRecord r(base, record);
  Store(base, f, stack);  // stwu's back chain
  r.set_node_index(0xFFFFFFFFu);
  r.set_shape_index(0xFFFFFFFFu);
  r.set_skeleton(0);
  r.set_model(0);
  r.set_shape(0);
  Identity(base, f + 96);
  Memcpy(base, r.matrix_address(), f + 96, 64, 0x820CEA8C);
  Identity(base, f + 96);
  Memcpy(base, r.inverse_address(), f + 96, 64, 0x820CEAD0);
  for (const uint32_t at : {80u, 84u, 88u}) {
    CopyWord(base, kZero, f + at);
  }
  CopyWord(base, kOne, f + 92);
  for (uint32_t w = 0; w < 4; w++) {
    Store(base, r.position_address() + 4 * w, Load(base, f + 80 + 4 * w));
  }
  r.set_node_name_0(0);
  r.set_shape_name_0(0);
  return r.inverse_address();
}

uint32_t PointRecordConstruct(uint8_t* base, uint32_t record, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820D0270 PointRecord::PointRecord");
  // The w's of the nine rows from the matrix's: the matrix's four, the
  // inverse's four and the position's.
  const schema::SecondSetPointRecord r(base, record);
  for (uint32_t row = 0; row < 9; row++) {
    CopyWord(base, kOne, r.matrix_row0_w_address() + 16 * row);
  }
  PointRecordClear(base, record, stack - kConstructFrameBytes);
  return record;
}

uint32_t ModelPointFindShape(uint8_t* base, uint32_t record, uint32_t model, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820CEB30 ModelPoint::FindShape");
  const uint32_t f = stack - kFindShapeFrameBytes;
  const schema::SecondSetPointRecord r(base, record);
  const schema::SecondSetFindModel m(base, model);
  r.set_shape_index(0xFFFFFFFFu);
  r.set_skeleton(0);
  r.set_model(0);
  r.set_shape(0);
  Identity(base, f + 80);
  Memcpy(base, r.matrix_address(), f + 80, 64, 0x820CEBA4);
  const int32_t nodes = int32_t(m.node_count());
  for (int32_t n = 0; n < nodes; n++) {
    const schema::SecondSetModelNode node(
        base, uint32_t(n) * schema::SecondSetModelNode::kSize + m.nodes().address);
    const uint32_t index = node.shape();
    if (int32_t(index) == -1) {
      continue;
    }
    const uint32_t shape = index * schema::ModelShape::kSize + m.shapes().address;
    if (scene_loaders::NameMatches(base, shape, r.shape_name_address()) != 0) {
      r.set_model(model);
      r.set_shape(shape);
      r.set_shape_index(node.shape());
      return 1;
    }
  }
  return 0;
}

}  // namespace second_set
