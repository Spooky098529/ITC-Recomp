// TuningScreen::LoadUnlockedParts (docs/functions/tuning-machine.md): see
// tuning_machine.h. The unlocked counterpart of LoadOwnedParts
// (owned_parts.cpp): each unlocked word 1 or 0 from its bit in the save
// data's bitset at +0x19930, in the original's order (the save's pointer
// and the part set read again at each word).

#include "tuning_machine/tuning_machine.h"

#include "base/log.h"
#include "guest.h"
#include "tuning_screen/tuning_screen.h"

namespace tuning_machine {

namespace {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kUnlockedBits = 0x19930;  // the save data's unlocked bitset

}  // namespace

void LoadUnlockedParts(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CFFC8 TuningScreen::LoadUnlockedParts");
  const uint32_t s = screen;
  // Bit n of the save's bitset (word n >> 5's 1 << (n & 31)): 1 or 0.
  auto bit = [&](uint32_t n) {
    const uint32_t set = Load(base, s + tuning_screen::kSaveData) + kUnlockedBits;
    return (Load(base, set + (n >> 5) * 4) >> (n & 31)) & 1;
  };
  auto set = [&](uint32_t offset, uint32_t value) { Store(base, s + offset, value); };
  // `count` words from s + `from`, word k bit `first` + k.
  auto words = [&](uint32_t from, uint32_t count, uint32_t first) {
    for (uint32_t k = 0; k < count; k++) {
      set(from + 4 * k, bit(first + k));
    }
  };
  // The power items 0 and 1 of the part set [+33900] (read at each word):
  // 1 and 3 from one half of the bits, 0, 2 and 4 the other, other sets
  // nothing written.
  for (uint32_t i = 0; i < 10; i++) {
    const uint32_t v = Load(base, s + 33900);
    const uint32_t at = (v * 10 + i + 8526) * 4;
    if (v == 1 || v == 3) {
      set(at, bit(i));
    } else if (v == 0 || v == 2 || v == 4) {
      set(at, bit(i + 10));
    }
  }
  for (uint32_t j = 20; j < 27; j++) {
    const uint32_t v = Load(base, s + 33900);
    const uint32_t at = (v * 7 + j + 8646) * 4;
    if (v == 1 || v == 3) {
      set(at, bit(j));
    } else if (v == 0 || v == 2 || v == 4) {
      set(at, bit(j + 7));
    }
  }
  words(34992, 7, 34);
  words(35084, 7, 41);
  words(35200, 7, 48);
  words(35288, 7, 55);
  words(35376, 7, 62);
  words(35464, 7, 69);
  for (uint32_t j = 0; j < 10; j++) {
    set(35568 + 4 * j, bit(j < 4 ? 76 + j : j < 7 ? 77 + j : 78 + j));
  }
  words(35680, 7, 88);
  // The tires: tire 0 from bit 95, tire i (1 to 4) unlocked when any of
  // bits 95 + i, 99 + i and 103 + i is set (tested in that order).
  set(35800, bit(95));
  for (uint32_t i = 1; i < 5; i++) {
    set(35800 + 4 * i, (bit(95 + i) != 0 || bit(99 + i) != 0 || bit(103 + i) != 0) ? 1 : 0);
  }
  // +25164: cleared, then set from bit 187 (word 5's 1 << 27).
  set(25164, 0);
  if (bit(187) != 0) {
    set(25164, 1);
  }
  words(36576, 3, 186);
}

}  // namespace tuning_machine
