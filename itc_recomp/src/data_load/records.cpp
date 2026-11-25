// The 5,232-byte records' constructors (data_load.h,
// docs/functions/data-load.md): 1.0 and 0.0 from .rdata into their float
// fields (probably the w words of vectors and matrices), nothing else.

#include "data_load/data_load.h"

#include "base/log.h"
#include "guest.h"
#include "schema/data_load.h"
#include "schema/replay_snapshots.h"

#include <bit>
#include <cstddef>

namespace data_load {

namespace {

using G = schema::DataLoadGlobals;

// A float lfs loaded (to a double), as stfs leaves its bits.
uint32_t FloatWord(float loaded) {
  const double value = double(loaded);
  return std::bit_cast<uint32_t>(float(value));
}

template <size_t N>
void StoreAll(uint32_t object, const uint16_t (&offsets)[N], uint32_t word) {
  for (const uint16_t offset : offsets) {
    guest::StoreU32(object + offset, word);
  }
}

constexpr uint16_t kPart464Zero[] = {12,  28,  44,  60,  76,  92,  108, 124, 140, 156,
                                     188, 220, 252, 284, 316, 348, 396, 412, 428, 444};
constexpr uint16_t kMember4080Zero[] = {956, 972, 1068, 1084, 1116, 1148};
constexpr uint16_t kMember1136Zero[] = {
    12,   28,   44,   76,   108,  124,  140,  348,  364,  380,  396,  476,  492,
    524,  540,  556,  572,  748,  844,  860,  892,  908,  924,  940,  1116, 1212,
    1228, 1260, 1276, 1292, 1308, 1484, 1580, 1596, 1628, 1644, 1660, 1676, 1852,
    1884, 1900, 1916, 1932, 1948, 1964, 2012, 2028, 2044, 2060, 2076, 2092, 2428,
    2444, 2492, 2508, 2524, 2540, 2556, 2572, 2876, 2892, 2908, 2924};
constexpr uint16_t kRecordOne[] = {156, 172, 188, 204};
constexpr uint16_t kRecordZero[] = {636, 652, 684, 716, 748, 764, 796,
                                    812, 828, 972, 988, 1020, 1100, 1116};

}  // namespace

uint32_t Part464Construct(uint32_t part) {
  ITC_NATIVE_RUNNING("sub_821042A0 Part464::Part464");
  StoreAll(part, kPart464Zero, FloatWord(G::zero()));
  return part;
}

uint32_t Member4080Construct(uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82104690 Member4080::Member4080");
  const schema::ReplayCollisionState state(member);
  Part464Construct(state.shape_previous_address());
  Part464Construct(state.shape_current_address());
  StoreAll(member, kMember4080Zero, FloatWord(G::zero()));
  return member;
}

uint32_t Member1136Construct(uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82102888 Member1136::Member1136");
  StoreAll(member, kMember1136Zero, FloatWord(G::zero()));
  return member;
}

uint32_t Record5232Construct(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82104C20 Record5232::Record5232");
  const uint32_t one = FloatWord(G::one());
  StoreAll(record, kRecordOne, one);
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU32(record + 252 + 96 * row + 16 * k, one);
    }
  }
  StoreAll(record, kRecordZero, FloatWord(G::zero()));
  const schema::ReplayRecord view(record);
  Member1136Construct(view.physics_address());
  Member4080Construct(view.collision_address());
  return record;
}

}  // namespace data_load
