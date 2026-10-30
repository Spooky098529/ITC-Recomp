// The 5,232-byte records' constructors (data_load.h,
// docs/functions/data-load.md): 1.0 and 0.0 from .rdata into their float
// fields (probably the w words of vectors and matrices), nothing else.

#include "data_load/data_load.h"

#include "base/log.h"
#include "guest.h"

#include <bit>
#include <cstddef>

namespace data_load {

namespace {

// The float at `address` as lfs (to a double) and stfs leave its bits.
uint32_t FloatWord(uint8_t* base, uint32_t address) {
  const double value = double(std::bit_cast<float>(guest::LoadU32(base, address)));
  return std::bit_cast<uint32_t>(float(value));
}

template <size_t N>
void StoreAll(uint8_t* base, uint32_t object, const uint16_t (&offsets)[N], uint32_t word) {
  for (const uint16_t offset : offsets) {
    guest::StoreU32(base, object + offset, word);
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

uint32_t Part464Construct(uint8_t* base, uint32_t part) {
  ITC_NATIVE_RUNNING("sub_821042A0 Part464::Part464");
  StoreAll(base, part, kPart464Zero, FloatWord(base, kZeroFloat));
  return part;
}

uint32_t Member4080Construct(uint8_t* base, uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82104690 Member4080::Member4080");
  Part464Construct(base, member + 16);
  Part464Construct(base, member + 480);
  StoreAll(base, member, kMember4080Zero, FloatWord(base, kZeroFloat));
  return member;
}

uint32_t Member1136Construct(uint8_t* base, uint32_t member) {
  ITC_NATIVE_RUNNING("sub_82102888 Member1136::Member1136");
  StoreAll(base, member, kMember1136Zero, FloatWord(base, kZeroFloat));
  return member;
}

uint32_t Record5232Construct(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82104C20 Record5232::Record5232");
  const uint32_t one = FloatWord(base, kOneFloat);
  StoreAll(base, record, kRecordOne, one);
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU32(base, record + 252 + 96 * row + 16 * k, one);
    }
  }
  StoreAll(base, record, kRecordZero, FloatWord(base, kZeroFloat));
  Member1136Construct(base, record + 1136);
  Member4080Construct(base, record + 4080);
  return record;
}

}  // namespace data_load
