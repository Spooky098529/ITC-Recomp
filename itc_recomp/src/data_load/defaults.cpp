// The save defaults' bit sets (data_load.h, docs/functions/data-load.md):
// Defaults::Init and its two fills. The original sets most bits one by one
// (an inlined bitset::set) and ORs fixed masks into the other words; every
// set is cleared first, so the bits listed here are what each holds after.

#include "data_load/data_load.h"

#include "base/log.h"
#include "guest.h"

#include <cstddef>

namespace data_load {

namespace {

struct Range {
  uint32_t first;
  uint32_t end;  // one past the last bit
};

void Clear(uint8_t* base, uint32_t set, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, set + 4 * i, 0);
  }
}

// bitset::set for bits first to end - 1 (all positive here).
void SetBits(uint8_t* base, uint32_t set, uint32_t first, uint32_t end) {
  for (uint32_t bit = first; bit < end; bit++) {
    const uint32_t word = set + 4 * (bit >> 5);
    guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (bit & 31)));
  }
}

template <size_t N>
void SetRanges(uint8_t* base, uint32_t set, const Range (&ranges)[N]) {
  for (const Range& r : ranges) {
    SetBits(base, set, r.first, r.end);
  }
}

// 0x826C3044 after its fixed masks and bits 404-409: the first bit of most
// of the 48 sets' ranges.
constexpr Range kBits3044[] = {
    {0, 1},     {10, 11},   {20, 21},   {27, 28},   {34, 35},   {41, 42},   {48, 49},
    {55, 56},   {62, 63},   {69, 70},   {76, 77},   {80, 81},   {84, 85},   {88, 89},
    {95, 96},   {108, 109}, {119, 120}, {130, 131}, {141, 142}, {152, 153}, {170, 171},
    {174, 175}, {178, 179}, {182, 183}, {186, 187}, {189, 190}, {389, 390}, {391, 392},
    {395, 396}, {400, 401}, {404, 411}, {415, 416}, {420, 421}, {425, 426}, {430, 431},
    {435, 436}};
// 0x826C50A8 after its fixed masks and bits 189-388, 410-434 and 435-453.
constexpr Range kBits50A8[] = {
    {0, 3},     {10, 13},   {20, 23},   {27, 30},   {34, 37},   {41, 44},   {48, 51},
    {55, 58},   {62, 65},   {69, 72},   {76, 78},   {80, 82},   {84, 86},   {88, 91},
    {95, 97},   {100, 101}, {104, 105}, {108, 114}, {119, 125}, {130, 136}, {141, 147},
    {152, 164}, {170, 187}, {189, 391}, {395, 399}, {400, 454}};
// The 48 sets' ranges (the original's jump table on the index; 29 none).
constexpr Range kSetRanges[48] = {
    {0, 20},    {0, 10},    {10, 20},   {20, 34},   {20, 27},   {27, 34},   {34, 41},
    {41, 48},   {48, 55},   {55, 62},   {62, 69},   {69, 76},   {76, 88},   {76, 80},
    {80, 84},   {84, 88},   {88, 95},   {95, 108},  {95, 96},   {96, 100},  {100, 104},
    {104, 108}, {108, 119}, {119, 130}, {130, 141}, {141, 152}, {152, 170}, {152, 158},
    {158, 164}, {0, 0},     {170, 174}, {174, 178}, {178, 182}, {182, 186}, {186, 189},
    {189, 389}, {389, 391}, {391, 395}, {395, 400}, {400, 404}, {404, 410}, {410, 435},
    {410, 415}, {415, 420}, {420, 425}, {425, 430}, {430, 435}, {435, 454}};
// The sets ORed into 0x826C4064 (the original's list on its frame).
constexpr uint32_t kOred[5] = {17, 35, 38, 41, 47};

}  // namespace

void Bits224Fill(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820CCD10 Bits224::Fill");
  Clear(base, set, 7);
  SetBits(base, set, 0, 199);
}

void Bits480Fill(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_820CCD68 Bits480::Fill");
  Clear(base, set, 15);
  SetBits(base, set, 0, 454);
}

uint32_t DefaultsInit(uint8_t* base, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820CCDC0 Defaults::Init");
  Clear(base, kTable3044, 15);
  SetRanges(base, kTable3044, kBits3044);
  Clear(base, kTable3C24, 7);
  SetBits(base, kTable3C24, 0, 61);
  Bits224Fill(base, kTable50E4);
  Clear(base, kTable50A8, 15);
  SetRanges(base, kTable50A8, kBits50A8);
  for (uint32_t i = 0; i < 48; i++) {
    const uint32_t set = kSets3080 + 60 * i;
    Clear(base, set, 15);
    SetBits(base, set, kSetRanges[i].first, kSetRanges[i].end);
  }
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, frame + 80 + 4 * i, kOred[i]);
  }
  Clear(base, kDefaultsBase, 15);
  for (const uint32_t index : kOred) {
    const uint32_t set = kSets3080 + 60 * index;
    for (uint32_t w = 0; w < 15; w++) {
      const uint32_t word = kDefaultsBase + 4 * w;
      guest::StoreU32(base, word, guest::LoadU32(base, word) | guest::LoadU32(base, set + 4 * w));
    }
  }
  Bits480Fill(base, kDefaultsBase);
  // andc of each word with itself.
  Clear(base, kDefaultsBase, 15);
  return kDefaultsBase;
}

}  // namespace data_load
