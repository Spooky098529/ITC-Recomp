// A player's save data in memory, natively (save_data.h,
// docs/functions/save-data.md).

#include "save_data/save_data.h"

#include "base/log.h"
#include "guest.h"

#include <bit>
#include <cstring>

namespace save_data {

namespace {

// The float the set-ups load with lfs (to a double) and store with stfs, as
// those leave its bits (no flush to zero).
uint32_t ZeroFloat(uint8_t* base) {
  const double value = double(std::bit_cast<float>(guest::LoadU32(base, kZeroFloat)));
  return std::bit_cast<uint32_t>(float(value));
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes, [](uint8_t* host, uint32_t, uint32_t size) {
    std::memset(host, 0, size);
  });
}

void Words(uint8_t* base, uint32_t address, uint32_t count, uint32_t value) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, address + 4 * i, value);
  }
}

void Halves(uint8_t* base, uint32_t address, uint32_t count, uint16_t value) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU16(base, address + 2 * i, value);
  }
}

}  // namespace

void StatsConstruct(uint8_t* base, uint32_t stats) {
  ITC_NATIVE_RUNNING("sub_821056C0 Stats::Stats");
  const uint32_t f = ZeroFloat(base);
  guest::StoreU32(base, stats + 4892, f);
  guest::StoreU32(base, stats + 4896, 0);
  for (uint32_t i = 0; i < 15; i++) {
    guest::StoreU32(base, stats + 4960 + 48 * i, f);
    guest::StoreU32(base, stats + 4964 + 48 * i, 0);
  }
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, stats + 5880 + 8 * i, f);
    guest::StoreU32(base, stats + 5884 + 8 * i, 0);
  }
}

void StatsResetRecords(uint8_t* base, uint32_t records) {
  ITC_NATIVE_RUNNING("sub_821136D8 Stats::ResetRecords");
  const uint32_t f = ZeroFloat(base);
  for (uint32_t i = 0; i < 15; i++) {
    const uint32_t r = records + 8 + 48 * i;  // the original's r11
    guest::StoreU32(base, r - 4, 0);
    guest::StoreU32(base, r + 0, 0);
    guest::StoreU32(base, r + 4, f);
    guest::StoreU32(base, r + 8, 0);
    guest::StoreU32(base, r + 12, 0);
    guest::StoreU32(base, r + 16, 0xFFFFFFFF);
    guest::StoreU32(base, r + 20, 0xFFFFFFFF);
    guest::StoreU32(base, r + 24, 0xFFFFFFFF);
    guest::StoreU32(base, r + 28, 0);
    guest::StoreU32(base, r + 32, 0);
    guest::StoreU32(base, r + 40, 0);
  }
  guest::StoreU32(base, records, 0);
}

uint32_t StatsReset(uint8_t* base, uint32_t s) {
  ITC_NATIVE_RUNNING("sub_82113730 Stats::Reset");
  Clear(base, s, kStatsBytes);
  Clear(base, s, 24);
  Clear(base, s + 24, 4828);
  const uint32_t f = ZeroFloat(base);
  guest::StoreU32(base, s + 4860, f);
  guest::StoreU16(base, s + 4854, 0);
  guest::StoreU16(base, s + 4852, 0);
  guest::StoreU32(base, s + 4856, 0);
  Clear(base, s + 4864, 24);
  guest::StoreU32(base, s + 4888, f);
  guest::StoreU32(base, s + 4892, f);
  guest::StoreU32(base, s + 4896, 0);
  guest::StoreU32(base, s + 4900, 0);
  guest::StoreU32(base, s + 4912, 0);
  guest::StoreU32(base, s + 4908, 0);
  Halves(base, s + 4916, 4, 0);
  Clear(base, s + 4924, 24);
  guest::StoreU32(base, s + 6032, 0);
  guest::StoreU8(base, s + 6036, 0);
  guest::StoreU8(base, s + 6037, 0);
  guest::StoreU8(base, s + 6038, 0);
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t entry = s + 5676 + 12 * i;
    guest::StoreU32(base, entry + 4, f);
    guest::StoreU32(base, entry + 0, 0);
    guest::StoreU32(base, entry + 8, 0);
  }
  guest::StoreU32(base, s + 5672, 0);
  StatsResetRecords(base, s + 4948);
  guest::StoreU32(base, s + 6008, 3);
  for (uint32_t i = 0; i < 3; i++) {
    Halves(base, s + 5796 + 14 * i, 7, 0);
  }
  Halves(base, s + 5838, 14, 0);
  Halves(base, s + 5866, 6, 0);
  guest::StoreU32(base, s + 5956, 1);
  return s + 4948;
}

uint32_t ProgressClear(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_82113BD8 Progress::Clear");
  Clear(base, p, kProgressBytes);
  guest::StoreU32(base, p + 0, 0);
  guest::StoreU32(base, p + 4, 0);
  for (uint32_t at : {p + 8, p + 28}) {
    guest::StoreU8(base, at + 0, 0);
    guest::StoreU32(base, at + 4, 6);
    guest::StoreU16(base, at + 8, 0);
    guest::StoreU8(base, at + 10, 1);
    guest::StoreU32(base, at + 12, 6);
    guest::StoreU16(base, at + 16, 0);
    guest::StoreU8(base, at + 18, 1);
  }
  const uint32_t r3 = StatsReset(base, p + kStats);
  guest::StoreU32(base, p + 6092, 0);
  guest::StoreU32(base, p + 6096, 0);
  guest::StoreU32(base, p + 6108, 14);
  guest::StoreU32(base, p + 6112, 48);
  guest::StoreU32(base, p + 0x1AD1C, 4);
  guest::StoreU32(base, p + 0x1AD18, 0);
  guest::StoreU32(base, p + 6100, 0);
  guest::StoreU32(base, p + 6116, 3000);
  // The garage list: 50 nodes {+0 next, +2 car}.
  const uint32_t garage = p + 6120;
  for (uint32_t i = 0; i < 50; i++) {
    guest::StoreU16(base, garage + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, garage + 4 * i + 0, uint16_t(i + 1));
  }
  guest::StoreU16(base, garage + 200, 0);
  guest::StoreU16(base, garage + 208, 0);
  guest::StoreU16(base, garage + 196, 0xFFFF);
  guest::StoreU16(base, garage + 204, 0xFFFF);
  guest::StoreU16(base, garage + 206, 0xFFFF);
  guest::StoreU16(base, garage + 202, 0xFFFF);
  guest::StoreU32(base, p + 0x1991C, 50);
  guest::StoreU32(base, p + 0x19920, 0);
  Words(base, p + 0x19924, 3, 0);
  Words(base, p + 0x19930, 15, 0);
  Words(base, p + 0x199A8, 7, 0);
  Words(base, p + 0x199C4, 7, 0);
  guest::StoreU32(base, p + 0x199E0, 0);
  guest::StoreU32(base, p + 0x199E4, 0);
  // The 400 item records of 12 bytes.
  for (uint32_t i = 0; i < 400; i++) {
    const uint32_t item = p + 0x199F8 + 12 * i;
    guest::StoreU32(base, item + 8, 0);
    guest::StoreU16(base, item + 6, 0);
    guest::StoreU16(base, item + 4, 0);
    guest::StoreU16(base, item + 2, 0);
    guest::StoreU16(base, item + 0, 0);
  }
  guest::StoreU32(base, p + 0x1ACE0, 0);
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, p + 0x1AD80 + 100 * i, 0);
  }
  Words(base, p + 0x1AF78, 1536, 0);
  guest::StoreU32(base, p + 0x1D524, 0xFFFFFFFF);
  guest::StoreU32(base, p + 0x1CA78, 0);
  guest::StoreU32(base, p + 0x1CA7C, 0);
  guest::StoreU32(base, p + 0x1CA80, 0);
  guest::StoreU32(base, p + 0x1D528, 0);
  guest::StoreU32(base, p + 0x1D504, 0);
  Words(base, p + 0x1D508, 7, 0);
  return r3;
}

void BitSet400SetAll(uint8_t* base, uint32_t set) {
  ITC_NATIVE_RUNNING("sub_821133A8 BitSet400::SetAll");
  Words(base, set, 13, 0);
  for (uint32_t i = 0; i < 400; i++) {
    const uint32_t word = set + 4 * (i >> 5);
    guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (i & 31)));
  }
}

uint32_t DefaultsTable50A8() {
  ITC_NATIVE_RUNNING("sub_820CCCC0 Defaults::Table50A8");
  return kTable50A8;
}

uint32_t DefaultsTable3044() {
  ITC_NATIVE_RUNNING("sub_820CCCD0 Defaults::Table3044");
  return kTable3044;
}

uint32_t DefaultsTable3E34() {
  ITC_NATIVE_RUNNING("sub_820CCCE0 Defaults::Table3E34");
  return kTable3E34;
}

uint32_t DefaultsTable3C24() {
  ITC_NATIVE_RUNNING("sub_820CCCF0 Defaults::Table3C24");
  return kTable3C24;
}

uint32_t DefaultsTable50E4() {
  ITC_NATIVE_RUNNING("sub_820CCD00 Defaults::Table50E4");
  return kTable50E4;
}

}  // namespace save_data
