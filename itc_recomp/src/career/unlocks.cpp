// The unlocks' conditions and the rewards (docs/functions/career.md).

#include "career/career.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace career {

namespace {

// Bit `b` (not negative) of the bit set at `set`: word b / 32, bit b % 32.
uint32_t BitWord(uint32_t set, int32_t b) { return set + (uint32_t(b >> 5) << 2); }

bool BitSet(uint8_t* base, uint32_t set, int32_t b) {
  return (guest::LoadU32(base, BitWord(set, b)) & (1u << (b & 31))) != 0;
}

void SetBit(uint8_t* base, uint32_t set, int32_t b) {
  const uint32_t word = BitWord(set, b);
  guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (b & 31)));
}

// The item kinds' mask test: bit (the item's halfword +42 + 9) of `mask`,
// the shift from that sum's low byte as the original's slw.
bool KindAllowed(uint8_t* base, uint32_t item, uint32_t mask) {
  const uint32_t shift = uint32_t(int32_t(int16_t(guest::LoadU16(base, item + 42))) + 9) & 0xFF;
  const uint32_t bit = (shift & 0x20) != 0 ? 0 : (1u << (shift & 0x1F));
  return (bit & mask) != 0;
}

}  // namespace

bool ConditionTestFits(uint8_t* base, uint32_t sp) {
  const uint32_t to = sp - kConditionTestFrame + 80;
  const int32_t items =
      int32_t(guest::LoadU32(base, guest::LoadU32(base, data_tables::kTables) + 8));
  return items <= 0 ||
         (to < 0xA0000000u &&
          !c_runtime::MayShare(to, 3008, data_tables::Item(base, 0), 3008 * uint32_t(items)));
}

std::optional<uint32_t> ConditionTest(uint8_t* base, uint32_t condition, uint32_t location,
                                      uint32_t mask, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82112950 Condition::Test");
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const bool masked = (mask & 0x7C00) != 0;
  const uint32_t kind = guest::LoadU32(base, condition + 0);
  if (kind > 9) {
    return 0;
  }
  const int32_t value = int32_t(guest::LoadU32(base, condition + 4));
  if (value < 0) {
    return 0;
  }
  const uint32_t records = save + kItemRecords;
  switch (kind) {
    case 0:
      return int16_t(guest::LoadU16(base, records + 12 * uint32_t(value) + 2)) > 0 ? 1 : 0;
    case 1:
      return guest::LoadU16(base, records + 12 * uint32_t(value) + 2) == 0 ? 1 : 0;
    case 2: {
      const uint32_t record = records + 12 * uint32_t(value);
      return guest::LoadU16(base, record + 2) == 0 && guest::LoadU16(base, record + 0) == 0 &&
                     guest::LoadU16(base, record + 4) == 0
                 ? 1
                 : 0;
    }
    case 3:
    case 4: {
      const int32_t weekday = int32_t(guest::LoadU32(base, save + 6100)) % 7;
      return (kind == 3 ? weekday > value : weekday < value) ? 1 : 0;
    }
    case 5:
    case 6: {
      if (int32_t(location) == -1 || location >= 8) {
        return 0;
      }
      bool done;
      if (location == 0) {
        done = BitSet(base, save + 0x1CA7C, value);
      } else {
        const uint32_t table = kind == 5 ? 0x1BE78 : 0x1C178;
        done = guest::LoadU32(base, save + table + 4 * uint32_t(value)) == 2;
      }
      return (kind == 5 ? done : !done) ? 1 : 0;
    }
    default: {
      // 7 (sub_821129E0's case, merged here), 8, 9: the items counted, each
      // copied onto the stack first (only the last copy is left there).
      const uint32_t tables = data_tables::kTables;
      const uint32_t to = sp - kConditionTestFrame + 80;
      if (!ConditionTestFits(base, sp)) {
        return std::nullopt;  // nothing written yet
      }
      int32_t count = 0;
      int32_t i = 0;
      for (; i < int32_t(guest::LoadU32(base, guest::LoadU32(base, tables) + 8)); i++) {
        const uint32_t item = data_tables::Item(base, uint32_t(i));
        const uint8_t item_kind = guest::LoadU8(base, item + 35);
        bool counted;
        if (kind == 8) {
          counted = item_kind == 2;
        } else if (kind == 7) {
          counted = int8_t(item_kind) == 3 || int8_t(item_kind) == 4 || int8_t(item_kind) == 5;
        } else {
          counted = true;
        }
        if (counted && masked && !KindAllowed(base, item, mask)) {
          counted = false;
        }
        if (counted && guest::LoadU16(base, records + 12 * uint32_t(i) + 2) != 0) {
          count++;
        }
      }
      if (i > 0) {
        c_runtime::Memcpy(base, to, data_tables::Item(base, uint32_t(i - 1)), 3008);
      }
      return count < value ? 0 : 1;
    }
  }
}

namespace {

// The unlock set's bit offsets by reward kind, for the kinds that only test
// and set one bit (value + offset).
constexpr int32_t kOneBit[42] = {
    0,   0,   34,  41,  48,  55,  62,  69,  76,  80,  84,  88,  0,   0,
    0,   0,   108, 119, 130, 141, 0,   170, 174, 178, 182, 0,   189, 389,
    391, 395, 400, 404, 410, 415, 420, 425, 430, 435, 0,   0,   0,   0,
};

void AppendReward(uint8_t* base, uint32_t save, uint32_t index) {
  const uint32_t list = save + 0x1CA84;
  const uint32_t entry = list + 12 * guest::LoadU32(base, list + 2688);
  guest::StoreU32(base, entry + 0, 0);
  guest::StoreU32(base, entry + 4, 8);
  guest::StoreU32(base, entry + 8, index);
  guest::StoreU32(base, list + 2688, guest::LoadU32(base, list + 2688) + 1);
}

}  // namespace

uint32_t RewardsGrant(uint8_t* base, uint32_t save, uint32_t index, uint32_t kind,
                      uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82114C08 Rewards::Grant");
  if (int32_t(index) >= 0) {
    SetBit(base, save + 0x1D508, int32_t(index));
  }
  if (kind > 41) {
    return save;
  }
  const uint32_t unlocks = save + kUnlockSet;
  bool fresh = false;
  // A bit tested (new when clear, or negative) and set (when not negative).
  const auto test_and_set = [&](uint32_t set, uint32_t offset) {
    const int32_t b = int32_t(value + offset);
    if (b < 0) {
      fresh = true;
      return;
    }
    if (!BitSet(base, set, b)) {
      fresh = true;
    }
    SetBit(base, set, b);
  };
  // A bit set only.
  const auto set_only = [&](uint32_t offset) {
    const int32_t b = int32_t(value + offset);
    if (b >= 0) {
      SetBit(base, unlocks, b);
    }
  };
  switch (kind) {
    case 0:  // sub_82114C7C's case, merged here
      test_and_set(unlocks, 0);
      set_only(10);
      break;
    case 1:
      test_and_set(unlocks, 20);
      set_only(27);
      break;
    case 12:
    case 13:
    case 14:
    case 15:
      test_and_set(unlocks, 95);
      set_only(99);
      set_only(103);
      break;
    case 20:
      test_and_set(unlocks, 152);
      set_only(158);
      break;
    case 25: {
      const uint32_t word = guest::LoadU32(base, unlocks + 20);
      if ((word & 0x08000000u) == 0) {
        fresh = true;
      }
      guest::StoreU32(base, unlocks + 20, word | 0x08000000u);
      guest::StoreU32(base, unlocks + 20, word | 0x18000000u);
      break;
    }
    case 38: {
      const uint32_t set = save + 0x19924;
      const int32_t b = int32_t(value);
      if (b >= 0 && BitSet(base, set, b)) {
        return save;
      }
      if (b >= 0) {
        SetBit(base, set, b);
      }
      AppendReward(base, save, index);
      return save;
    }
    case 39:
      AppendReward(base, save, index);
      return save;
    case 40:
      test_and_set(save + 0x199A8, 0);
      break;
    case 41:
      test_and_set(save + 0x199C4, 0);
      break;
    default:
      test_and_set(unlocks, uint32_t(kOneBit[kind]));
      break;
  }
  if (fresh) {
    AppendReward(base, save, index);
  }
  return save;
}

}  // namespace career
